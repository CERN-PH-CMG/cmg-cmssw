


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

//
// class declaration
//

class InconsistentMuonPFCandidateFilterPF : public edm::EDFilter {
public:
  explicit InconsistentMuonPFCandidateFilterPF(const edm::ParameterSet&);
  ~InconsistentMuonPFCandidateFilterPF();
  
private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
      // ----------member data ---------------------------
  
  const edm::InputTag   inputTagPFCandidates_; 
  const double          ptMin_;
  const double          maxPTDiff_;
  const bool            verbose_;
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
InconsistentMuonPFCandidateFilterPF::InconsistentMuonPFCandidateFilterPF(const edm::ParameterSet& iConfig)
  : inputTagPFCandidates_ ( iConfig.getParameter<edm::InputTag> ("PFCandidates")  )
  , ptMin_                ( iConfig.getParameter<double>        ("ptMin")         )
  , maxPTDiff_            ( iConfig.getParameter<double>        ("maxPTDiff")     )
  , verbose_              ( iConfig.getUntrackedParameter<bool> ("verbose",false) )
  , taggingMode_          ( iConfig.getParameter<bool> ("taggingMode") )
{
  produces<reco::PFCandidateCollection>();
  produces<bool>("Result");
}

InconsistentMuonPFCandidateFilterPF::~InconsistentMuonPFCandidateFilterPF() { }


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
InconsistentMuonPFCandidateFilterPF::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;  
  using namespace edm;

  Handle<reco::PFCandidateCollection>     pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_,pfCandidates);
  
  bool    foundMuon = false;

  auto_ptr< reco::PFCandidateCollection > 
    pOutputCandidateCollection( new reco::PFCandidateCollection ); 

  for ( unsigned i=0; i<pfCandidates->size(); i++ ) {
     
    const reco::PFCandidate & cand = (*pfCandidates)[i];

    if ( cand.particleId() != reco::PFCandidate::mu ) continue; 
    if ( cand.pt() < ptMin_ )                         continue; 

    const reco::MuonRef       muon  = cand.muonRef();
    if (  muon->isTrackerMuon()
       && muon->isGlobalMuon()
       && fabs(muon->innerTrack()->pt()/muon->globalTrack()->pt() - 1) <= maxPTDiff_
       )
      continue;

    foundMuon = true;

    pOutputCandidateCollection->push_back( cand ); 

    if ( verbose_ ) {
      cout << cand << endl;
      cout << "\t" << "tracker pT = ";
      if (muon->isTrackerMuon())  cout << muon->innerTrack()->pt();
      else                        cout << "(n/a)";
      cout << endl;
      cout << "\t" << "global fit pT = ";
      if (muon->isGlobalMuon())   cout << muon->globalTrack()->pt();
      else                        cout << "(n/a)";
      cout << endl;
    }    
  } // end loop over PF candidates
   
  iEvent.put( pOutputCandidateCollection ); 

  bool result = !foundMuon; 

  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 
  

  if(taggingMode_) return true;
  else return result; // true if no inconsistent muon is found
}

// ------------ method called once each job just before starting event loop  ------------
void 
InconsistentMuonPFCandidateFilterPF::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
InconsistentMuonPFCandidateFilterPF::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(InconsistentMuonPFCandidateFilterPF);
