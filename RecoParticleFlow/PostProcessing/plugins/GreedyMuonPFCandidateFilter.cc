


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"

//
// class declaration
//

class GreedyMuonPFCandidateFilterPF : public edm::EDFilter {
public:
  explicit GreedyMuonPFCandidateFilterPF(const edm::ParameterSet&);
  ~GreedyMuonPFCandidateFilterPF();
  
private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag  inputTagPFCandidates_; 
      // ----------member data ---------------------------
  
  double eOverPMax_;

  bool verbose_;
  const bool            taggingMode_;
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
GreedyMuonPFCandidateFilterPF::GreedyMuonPFCandidateFilterPF(const edm::ParameterSet& iConfig) : 
  inputTagPFCandidates_ ( iConfig.getParameter<edm::InputTag>("PFCandidates") ),
  eOverPMax_ ( iConfig.getParameter<double>("eOverPMax") ),
  verbose_  ( iConfig.getUntrackedParameter<bool>("verbose",false) ),
  taggingMode_  ( iConfig.getParameter<bool> ("taggingMode") ) {

  produces<reco::PFCandidateCollection>();
  produces<bool>("Result");

}


GreedyMuonPFCandidateFilterPF::~GreedyMuonPFCandidateFilterPF()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
GreedyMuonPFCandidateFilterPF::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;  
  using namespace edm;

  Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_,pfCandidates);
  
  bool foundMuon = false;

  auto_ptr< reco::PFCandidateCollection > 
    pOutputCandidateCollection( new reco::PFCandidateCollection ); 

  for( unsigned i=0; i<pfCandidates->size(); i++ ) {
     
    const reco::PFCandidate & cand = (*pfCandidates)[i];

    if( cand.particleId() != 3 ) // not a muon 
      continue; 
    
    if(!PFMuonAlgo::isIsolatedMuon( cand.muonRef() ) ) // muon is not isolated
      continue; 

    double totalCaloEnergy = cand.rawEcalEnergy() +  cand.rawHcalEnergy();
    double eOverP = totalCaloEnergy/cand.p(); 
    
    if( eOverP < eOverPMax_ )
      continue; 

    foundMuon = true;

    pOutputCandidateCollection->push_back( cand ); 

    if( verbose_ ) {
      cout<<cand<<" HCAL E="<<endl;
      cout<<"\t"<<"ECAL energy "<<cand.rawEcalEnergy()<<endl;
      cout<<"\t"<<"HCAL energy "<<cand.rawHcalEnergy()<<endl;
      cout<<"\t"<<"E/p "<<eOverP<<endl;
  }    
  }
   
  iEvent.put( pOutputCandidateCollection ); 
  
  bool result = !foundMuon; 

  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  if(taggingMode_) return true;
  else return result; // true if no greedy muon is found
}

// ------------ method called once each job just before starting event loop  ------------
void 
GreedyMuonPFCandidateFilterPF::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GreedyMuonPFCandidateFilterPF::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(GreedyMuonPFCandidateFilterPF);
