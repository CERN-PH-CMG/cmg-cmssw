// -*- C++ -*-
//
// Package:    HZZGenEmbedder
// Class:      HZZGenEmbedder
// 
/**\class HZZGenEmbedder HZZGenEmbedder.cc TEST/HZZGenEmbedder/src/HZZGenEmbedder.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michail Bachtis,40 1-B08,+41227678176,
//         Created:  Mon Nov 12 20:34:11 CET 2012
// $Id: HZZGenEmbedder.cc,v 1.1 2012/11/13 00:14:09 bachtis Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/MergeableCounter.h"  


//
// class declaration
//

class HZZGenEmbedder : public edm::EDProducer {
   public:
      explicit HZZGenEmbedder(const edm::ParameterSet&);
      ~HZZGenEmbedder();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
  int N4mu_;
  int N4e_;
  int N2e2mu_;
  int NZH_;
  int NWH_;
  int NTTH_;

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
HZZGenEmbedder::HZZGenEmbedder(const edm::ParameterSet& iConfig)
{


  produces<int>("processID");
  produces<int>("BR");
  produces<edm::MergeableCounter, edm::InLumi>("4e");
  produces<edm::MergeableCounter, edm::InLumi>("4mu");
  produces<edm::MergeableCounter, edm::InLumi>("2e2mu");
  produces<edm::MergeableCounter, edm::InLumi>("WH");
  produces<edm::MergeableCounter, edm::InLumi>("ZH");
  produces<edm::MergeableCounter, edm::InLumi>("ttH");
}


HZZGenEmbedder::~HZZGenEmbedder()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HZZGenEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

  int nelectrons=0;
  int nmuons=0;

  edm::Handle<reco::GenParticleCollection> genParticles;
  if(iEvent.getByLabel("genParticlesPruned",genParticles)) 
    for(reco::GenParticleCollection::const_iterator i = genParticles->begin();
	i!= genParticles->end();++i) {
      //We need this particle to have Z as a mother and Z must have Higgs as mother
      if (i->numberOfMothers()>0 && abs(i->mother(0)->pdgId())==23&&i->mother(0)->numberOfMothers()>0&&abs(i->mother(0)->mother(0)->pdgId())==25) {
	//We need to be an electron or muon
	if(abs(i->pdgId())==11)
	  nelectrons++;
	if(abs(i->pdgId())==13)
	  nmuons++;


      }

    }

  std::auto_ptr<int> outBR(new int);
  *outBR = 0;

  if (nelectrons==2&&nmuons==2) {
    *outBR=3;
    N2e2mu_++;
  }
  else if (nelectrons==4) {
    N4e_++;
    *outBR=1;
  }
  else if (nmuons==4) {
    N4mu_++;
    *outBR=2;
  }
  iEvent.put(outBR,"BR");   
  

  //DONE WITH BRS GO TO PRODUCTION
  std::auto_ptr<int> out(new int);
  *out = 0;
  edm::Handle<GenEventInfoProduct> genProduct;
  if(iEvent.getByLabel("generator",genProduct)) 
    {
      *out = genProduct->signalProcessID();
    }


  if( *out==24)
    NZH_++;
  if( *out==26)
    NWH_++;
  if( *out==121 || *out==122)
    NTTH_++;
  
  iEvent.put(out,"processID");   
}

// ------------ method called once each job just before starting event loop  ------------
void 
HZZGenEmbedder::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HZZGenEmbedder::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
HZZGenEmbedder::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HZZGenEmbedder::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HZZGenEmbedder::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  N4mu_=0;
  N4e_=0;
  N2e2mu_=0;
  NZH_=0;
  NWH_=0;
  NTTH_=0;





}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HZZGenEmbedder::endLuminosityBlock(edm::LuminosityBlock& lumi, edm::EventSetup const& iSetup)
{
  using namespace std;
  auto_ptr<edm::MergeableCounter> N4mu(new edm::MergeableCounter);
  N4mu->value = N4mu_;
  lumi.put(N4mu,"4mu");
  auto_ptr<edm::MergeableCounter> N2e2mu(new edm::MergeableCounter);
  N2e2mu->value = N2e2mu_;
  lumi.put(N2e2mu,"2e2mu");
  auto_ptr<edm::MergeableCounter> N4e(new edm::MergeableCounter);
  N4e->value = N4e_;
  lumi.put(N4e,"4e");

  auto_ptr<edm::MergeableCounter> NZH(new edm::MergeableCounter);
  NZH->value = NZH_;
  lumi.put(NZH,"ZH");
  auto_ptr<edm::MergeableCounter> NWH(new edm::MergeableCounter);
  NWH->value = NWH_;
  lumi.put(NWH,"WH");
  auto_ptr<edm::MergeableCounter> NTTH(new edm::MergeableCounter);
  NTTH->value = NTTH_;
  lumi.put(NTTH,"ttH");


}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HZZGenEmbedder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HZZGenEmbedder);
