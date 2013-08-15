// -*- C++ -*-
//
// Package:    HZZCMGSkim
// Class:      HZZCMGSkim
// 
/**\class HZZCMGSkim HZZCMGSkim.cc CMGTools/HZZCMGSkim/src/HZZCMGSkim.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michail Bachtis,40 1-B08,+41227678176,
//         Created:  Fri Sep 21 21:30:33 CEST 2012
// $Id: HZZCMGSkim.cc,v 1.1 2012/09/22 11:01:40 bachtis Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

//
// class declaration
//

class HZZCMGSkim : public edm::EDFilter {
   public:
      explicit HZZCMGSkim(const edm::ParameterSet&);
      ~HZZCMGSkim();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
      int NL_;

};

//
HZZCMGSkim::HZZCMGSkim(const edm::ParameterSet& iConfig):
  NL_(iConfig.getParameter<int>("nLeptons"))
{

}


HZZCMGSkim::~HZZCMGSkim()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
HZZCMGSkim::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<std::vector<cmg::Muon> > muons;
   iEvent.getByLabel("cmgMuonSel",muons);
   Handle<std::vector<cmg::Electron> > electrons;
   iEvent.getByLabel("cmgElectronSel",electrons);
   
   int nMuons20=0;
   int nMuons10=0;
   int nMuons3=0;

   int nElectrons20=0;
   int nElectrons10=0;
   int nElectrons5=0;

   for(std::vector<cmg::Electron>::const_iterator i = electrons->begin();i!= electrons->end();++i) {
     if (i->pt()>20 )
       nElectrons20++;
     if (i->pt()>10 )
       nElectrons10++;
     if (i->pt()>7 )
       nElectrons5++;
   }

   for(std::vector<cmg::Muon>::const_iterator i = muons->begin();i!= muons->end();++i) 
     if(i->isGlobalMuon() || i->isTrackerMuon())
     {
       if (i->pt()>20 )
	 nMuons20++;
       if (i->pt()>10 )
	 nMuons10++;
       if (i->pt()>3 )
       nMuons3++;
     }

   bool decision=true;

   if((nMuons3+nElectrons5) <NL_)
     decision=false;


   if((nMuons20+nElectrons20) <1)
     decision=false;

   if((nMuons10+nElectrons10) <2)
     decision=false;




   return decision;




}

// ------------ method called once each job just before starting event loop  ------------
void 
HZZCMGSkim::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HZZCMGSkim::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
HZZCMGSkim::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
HZZCMGSkim::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
HZZCMGSkim::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
HZZCMGSkim::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HZZCMGSkim::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(HZZCMGSkim);
