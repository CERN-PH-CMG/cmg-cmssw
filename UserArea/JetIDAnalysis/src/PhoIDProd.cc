// -*- C++ -*-
//
// Package:    PhoIDProd
// Class:      PhoIDProd
// 
/**\class PhoIDProd PhoIDProd.cc ADavid/PhoIDProd/src/PhoIDProd.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andre Tinoco Mendes,40 2-A12,+41227676147,
//         Created:  Tue Mar 13 16:03:44 CET 2012
// $Id: PhoIDProd.cc,v 1.3 2012/03/13 19:38:21 adavidzh Exp $
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"


//
// class declaration
//

class PhoIDProd : public edm::EDProducer {
   public:
      explicit PhoIDProd(const edm::ParameterSet&);
      ~PhoIDProd();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  float getCiC4IDLevel(edm::Event&, const reco::Photon&);
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
PhoIDProd::PhoIDProd(const edm::ParameterSet& iConfig)
{
  produces<edm::ValueMap<float> >("PhoIDCiC4Level").setBranchAlias("PhoIDCiC4Level");
}


PhoIDProd::~PhoIDProd()
{
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
PhoIDProd::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;
   Handle<PhotonCollection> photons;
   iEvent.getByLabel("photons", photons);
   vector<float> cic4Levels;

   for(PhotonCollection::const_iterator photon = photons->begin();
       photon != photons->end(); ++photon) {
     float cic4Level = getCiC4IDLevel(iEvent, *photon);
     cic4Levels.push_back(cic4Level);
   }

   auto_ptr<ValueMap<float> > cic4Out(new ValueMap<float>());
   ValueMap<float>::Filler cic4Filler(*cic4Out);
   cic4Filler.insert(photons, cic4Levels.begin(), cic4Levels.end());
   cic4Filler.fill();

   // put value map into event
   iEvent.put(cic4Out, "PhoIDCiC4Level");
}

// ------------ method called once each job just before starting event loop  ------------
void 
PhoIDProd::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PhoIDProd::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
PhoIDProd::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
PhoIDProd::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
PhoIDProd::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
PhoIDProd::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PhoIDProd::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ------------ method returns the largest CiC4 Photon ID that the photon passes ----------------
float PhoIDProd::getCiC4IDLevel(edm::Event& iEvent, const reco::Photon& photon){
  //  std::cerr << "Pho R9: " << photon.r9() << std::endl;
  return photon.r9();
}
 
//define this as a plug-in
DEFINE_FWK_MODULE(PhoIDProd);
