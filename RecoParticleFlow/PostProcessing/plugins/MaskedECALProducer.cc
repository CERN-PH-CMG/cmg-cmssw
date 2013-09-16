/**
  @file         MaskedECALProducer.cc
  @author       Sue Ann Koay
  @description  Makes a PFRecHit collection with items providing the locations 
                of all masked ECAL channels. See:
                https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalChannelStatus

  For Spring10 MadGraph QCD, the following need to be provided in 
  the config file:

  process.load('Configuration.StandardSequences.GeometryExtended_cff')
  process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
  process.GlobalTag.globaltag = 'START3X_V26::All'
*/


#include <string>
#include <vector>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"

// ECAL
#include "CalibCalorimetry/EcalTPGTools/interface/EcalTPGScale.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

// Geometry
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"


class MaskedECALProducer : public edm::EDProducer
{
public:
  const int minExcludeStatus;
  typedef unsigned int size;

  const edm::InputTag                       tpSource;

  MaskedECALProducer(const edm::ParameterSet& iConfig) 
    : minExcludeStatus(iConfig.getParameter<int          >("minExcludeStatus"))
    , tpSource        (iConfig.getParameter<edm::InputTag>("ecalTriggerPrimitives"))
  { 
    produces< EcalRecHitCollection > ("masked") ;
    produces< EcalRecHitCollection > ("tp"    ) ;
    produces< EcalRecHitCollection > ("noTP"  ) ;
  }

  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);

protected:
  class ECALTower {
  public:
    EcalTrigTowerDetId  towerID;
    std::vector<DetId>  detIDs;
    double              towerE;
    
    ECALTower(                                 ) : towerID(       ), towerE(0) { }
    ECALTower(const EcalTrigTowerDetId& towerID) : towerID(towerID), towerE(0) { }
  };
  typedef   std::vector<ECALTower>  ECALTowers;
}; // class MaskedECALProducer



void MaskedECALProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  EcalTPGScale                              ecalScale;    
  ecalScale .setEventSetup(iSetup );

  edm::ESHandle<EcalChannelStatus>          ecalStatus;
  iSetup.get<EcalChannelStatusRcd> ().get(ecalStatus);
  if (!ecalStatus.isValid())  throw "Failed to get ECAL channel status!";

  edm::ESHandle<EcalTrigTowerConstituentsMap> ttMap;
  iSetup.get<IdealGeometryRecord>().get(ttMap);

  edm::Handle<EcalTrigPrimDigiCollection>   tpDigis;
  iEvent.getByLabel(tpSource, tpDigis);

  //---------------------------------------------------------------------------
  
  std::auto_ptr<EcalRecHitCollection>       masked( new EcalRecHitCollection );
  std::auto_ptr<EcalRecHitCollection>       tp    ( new EcalRecHitCollection );
  std::auto_ptr<EcalRecHitCollection>       noTP  ( new EcalRecHitCollection );
  
  // Setup trigger tower storage
  ECALTowers                                towers;
  if (tpDigis.isValid()) {
    for (size iTP = 0; iTP < tpDigis->size(); ++iTP) {
      const EcalTriggerPrimitiveDigi&       tp        = (*tpDigis)[iTP];
      const double                          energy    = ecalScale.getTPGInGeV(tp.compressedEt(), tp.id());
      if (energy > 0) {
        towers.push_back(tp.id());
        towers.back().towerE                = energy;
      }
    } // end loop over trigger primitives
  }
  const size                                numTowers = towers.size();


  //____ get Bad ECAL channels_________________________________________________
  // Loop over EB detid
  for ( int ieta=-85; ieta<=85; ++ieta ){
    for ( int iphi=0; iphi<=360; ++iphi ){
      if (!EBDetId::validDetId(ieta,iphi))  continue;

      const EBDetId                         detid     = EBDetId( ieta, iphi, EBDetId::ETAPHIMODE );
      EcalChannelStatus::const_iterator     chit      = ecalStatus->find( detid );
      int                                   status    = ( chit != ecalStatus->end() ) ? chit->getStatusCode() : -1;
      if (status < minExcludeStatus)        continue;

      bool                                  hasTP     = false;
      const EcalTrigTowerDetId              towerID   = ttMap->towerOf(detid);
      for (size iTower = 0; iTower < numTowers; ++iTower)
        if (towers[iTower].towerID == towerID) {
          towers[iTower].detIDs.push_back(detid);
          hasTP                             = true;
          break;
        }
      masked->push_back(EcalRecHit(detid, status/10., 0));
      if (!hasTP && tpDigis.isValid())
        noTP->push_back(EcalRecHit(detid, status/10., 0));
    } // end loop iphi
  } // end loop ieta

  // Loop over EE detid
  for ( int ix=0; ix<=100; ++ix ){
    for ( int iy=0; iy<=100; ++iy ){
      for ( int iz=-1; iz<=1; ++iz ){
        if (iz==0)  continue;
        if (!EEDetId::validDetId(ix,iy,iz)) continue;

        const EEDetId                       detid     = EEDetId( ix, iy, iz, EEDetId::XYMODE );
        EcalChannelStatus::const_iterator   chit      = ecalStatus->find( detid );
        int                                 status    = ( chit != ecalStatus->end() ) ? chit->getStatusCode() : -1;
        if (status < minExcludeStatus)      continue;

        masked->push_back(EcalRecHit(detid, status/10., 0));

        bool                                hasTP     = false;
        const EcalTrigTowerDetId            towerID   = ttMap->towerOf(detid);
        for (size iTower = 0; iTower < numTowers; ++iTower)
          if (towers[iTower].towerID == towerID) {
            towers[iTower].detIDs.push_back(detid);
            hasTP                           = true;
            break;
          }
        masked->push_back(EcalRecHit(detid, status/10., 0));
        if (!hasTP && tpDigis.isValid())
          noTP->push_back(EcalRecHit(detid, status/10., 0));
      } // end loop iz
    } // end loop iy
  } // end loop ix
  
  
  //_____ store trigger primitive energies ____________________________________
  for (size iTower = 0; iTower < numTowers; ++iTower) {
    const ECALTower&                        tower     = towers[iTower];
    const size                              nChannels = tower.detIDs.size();
    //const double                            channelE  = tower.towerE / nChannels;
    ////std::cout << tower.towerE << " / " << nChannels << " = " << channelE << std::endl;
    for (size iChannel = 0; iChannel < nChannels; ++iChannel)
      tp->push_back(EcalRecHit(tower.detIDs[iChannel], tower.towerE, 0));
  } // end loop over trigger towers


  iEvent.put(masked, "masked");
  iEvent.put(tp    , "tp"    );
  iEvent.put(noTP  , "noTP"  );
}



DEFINE_FWK_MODULE(MaskedECALProducer);

