// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/PatCandidates/interface/Electron.h>

// Variables for regression
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "RecoEgamma/EgammaTools/interface/EcalClusterLocal.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"


#include<vector>



class PATElectronRegressionEmbedder : public edm::EDProducer {
    public:
        explicit PATElectronRegressionEmbedder(const edm::ParameterSet&);
        ~PATElectronRegressionEmbedder();

    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        // ----------member data ---------------------------
        edm::InputTag electrons_;
        edm::InputTag ecalBarrelRecHitProducer_;
        edm::InputTag ecalEndcapRecHitProducer_;
};


PATElectronRegressionEmbedder::PATElectronRegressionEmbedder(const edm::ParameterSet& iConfig) :
        electrons_(iConfig.getParameter<edm::InputTag>("electrons")),
        ecalBarrelRecHitProducer_(iConfig.getUntrackedParameter<edm::InputTag>("barrelHits",edm::InputTag("reducedEcalRecHitsEB"))),
        ecalEndcapRecHitProducer_(iConfig.getUntrackedParameter<edm::InputTag>("endcapHits",edm::InputTag("reducedEcalRecHitsEE")))
{
  produces<pat::ElectronCollection>();  

}


PATElectronRegressionEmbedder::~PATElectronRegressionEmbedder() {

}


void PATElectronRegressionEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;
    using namespace std;


    edm::Handle<pat::ElectronCollection> electrons;
    iEvent.getByLabel(electrons_,electrons);

    edm::ESHandle<CaloTopology> pTopology;
    iSetup.get<CaloTopologyRecord>().get(pTopology);
    const CaloTopology *topology = pTopology.product();

    edm::ESHandle<CaloGeometry> pGeometry;
    iSetup.get<CaloGeometryRecord>().get(pGeometry);

    // Next get Ecal hits barrel
    edm::Handle<EcalRecHitCollection> ecalBarrelRecHitHandle; 
    iEvent.getByLabel(ecalBarrelRecHitProducer_,ecalBarrelRecHitHandle);
    edm::Handle<EcalRecHitCollection> ecalEndcapRecHitHandle;
    iEvent.getByLabel(ecalEndcapRecHitProducer_,ecalEndcapRecHitHandle);


    std::auto_ptr<pat::ElectronCollection> pOut(new pat::ElectronCollection);



    // ----- here is the real loop over the electrons ----
    for(std::vector<pat::Electron>::const_iterator i = electrons->begin(); i!= electrons->end();i++ ){    
      pat::Electron clone = *i;

	  // Variables for regression
	  // Code adapted from /UserCode/HiggsAnalysis/HiggsToWW2e/src/CmsSuperClusterFiller.cc

        clone.addUserInt("nBC", (int)clone.superCluster()->clustersSize());
	clone.addUserInt("nCrystals", (int)clone.superCluster()->hitsAndFractions().size());
	clone.addUserFloat("rawEnergy", (float)clone.superCluster()->rawEnergy());
	clone.addUserFloat("seedClusterEnergy", (float)clone.superCluster()->seed()->energy());
	clone.addUserFloat("seedClusterEta", (float)clone.superCluster()->seed()->eta());
	clone.addUserFloat("seedClusterPhi", (float)clone.superCluster()->seed()->phi());
	clone.addUserFloat("energy", (float)clone.superCluster()->energy());
	clone.addUserFloat("esEnergy", (float)clone.superCluster()->preshowerEnergy());
	clone.addUserFloat("phiWidth", (float)clone.superCluster()->phiWidth());
	clone.addUserFloat("etaWidth", (float)clone.superCluster()->etaWidth());
	clone.addUserFloat("eta", (float)clone.superCluster()->position().eta());
	clone.addUserFloat("theta", (float)clone.superCluster()->position().theta());
	clone.addUserFloat("phi", (float)clone.superCluster()->position().phi());

	const EcalRecHitCollection *rechits = 0;

	// seed crystal properties
	const Ptr<reco::CaloCluster> theSeed = clone.superCluster()->seed();

	float seedEta = theSeed->position().eta();

	if( fabs(seedEta) < 1.479 ) rechits = &(*ecalBarrelRecHitHandle);
	else rechits = &(*ecalEndcapRecHitHandle);

	clone.addUserFloat("eMax", EcalClusterTools::eMax( *theSeed, &(*rechits) ));
	clone.addUserFloat("e3x3", EcalClusterTools::e3x3( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e5x5", EcalClusterTools::e5x5( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2x2", EcalClusterTools::e2x2( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2nd", EcalClusterTools::e2nd( *theSeed, &(*rechits) ));
	clone.addUserFloat("e1x5", EcalClusterTools::e1x5( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2x5Max", EcalClusterTools::e2x5Max( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2x5Left", EcalClusterTools::e2x5Left( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2x5Right", EcalClusterTools::e2x5Right( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2x5Top", EcalClusterTools::e2x5Top( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e2x5Bottom", EcalClusterTools::e2x5Bottom( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("eLeft", EcalClusterTools::eLeft( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("eRight", EcalClusterTools::eRight( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("eTop", EcalClusterTools::eTop( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("eBottom", EcalClusterTools::eBottom( *theSeed, &(*rechits), topology ));
	clone.addUserFloat("e4SwissCross", ( EcalClusterTools::eLeft( *theSeed, &(*rechits), topology ) +
					     EcalClusterTools::eRight( *theSeed, &(*rechits), topology ) +
					     EcalClusterTools::eTop( *theSeed, &(*rechits), topology ) +
					     EcalClusterTools::eBottom( *theSeed, &(*rechits), topology ) ));

	// local covariances: instead of using absolute eta/phi it counts crystals normalised
	std::vector<float> vLocCov = EcalClusterTools::localCovariances( *theSeed, &(*rechits), topology );

	float covIEtaIEta = vLocCov[0];
	float covIEtaIPhi = vLocCov[1];
	float covIPhiIPhi = vLocCov[2];

	clone.addUserFloat("covIEtaIEta", covIEtaIEta);
	clone.addUserFloat("covIEtaIPhi", covIEtaIPhi);
	clone.addUserFloat("covIPhiIPhi", covIPhiIPhi);

	// seed second moments wrt principal axes:
	Cluster2ndMoments moments = EcalClusterTools::cluster2ndMoments(*theSeed, *rechits );
	clone.addUserFloat("sMaj", moments.sMaj);
	clone.addUserFloat("sMin", moments.sMin);
	// angle between sMaj and phi direction:
	clone.addUserFloat("alpha", moments.alpha);

	std::pair<DetId, float> maxRH = EcalClusterTools::getMaximum( *theSeed, &(*rechits) );
	DetId seedCrystalId = maxRH.first;
	EcalRecHitCollection::const_iterator seedRH = rechits->find(seedCrystalId);

	clone.addUserFloat("time", (float)seedRH->time());
	clone.addUserFloat("chi2", (float)seedRH->chi2());
	clone.addUserFloat("recoFlag", (int)seedRH->recoFlag());
	clone.addUserFloat("seedEnergy", (float)maxRH.second);

	EcalClusterLocal local;  
	if(clone.superCluster()->seed()->hitsAndFractions().at(0).first.subdetId()==EcalBarrel) {
	  float etacry, phicry, thetatilt, phitilt;
	  int ieta, iphi;
	  local.localCoordsEB(*clone.superCluster()->seed(),iSetup,etacry,phicry,ieta,iphi,thetatilt,phitilt);

	  clone.addUserFloat("seedieta", ieta);
	  clone.addUserFloat("seediphi", iphi);
	  clone.addUserFloat("etacryseed", etacry);
	  clone.addUserFloat("phicryseed", phicry);

	} else {
	  float xcry, ycry, thetatilt, phitilt;
	  int ix, iy;
	  local.localCoordsEE(*clone.superCluster()->seed(),iSetup,xcry,ycry,ix,iy,thetatilt,phitilt);

	  clone.addUserFloat("seedieta", ix);
	  clone.addUserFloat("seediphi", iy);
	  clone.addUserFloat("etacryseed", xcry);
	  clone.addUserFloat("phicryseed", ycry);
	}

	pOut->push_back(clone);

    }
    iEvent.put(pOut);
}

// ------------ method called once each job just before starting event loop  ------------
void PATElectronRegressionEmbedder::beginJob() { }

// ------------ method called once each job just after ending the event loop  ------------
void PATElectronRegressionEmbedder::endJob() { }


DEFINE_FWK_MODULE(PATElectronRegressionEmbedder);
