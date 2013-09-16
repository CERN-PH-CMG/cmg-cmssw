


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/View.h"

// class declaration
//

class METCorrelationFilter : public edm::EDFilter {
public:
  explicit METCorrelationFilter(const edm::ParameterSet&);
  ~METCorrelationFilter();
  
private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag  inputTagMET1_; 
  edm::InputTag  inputTagMET2_; 
  
  double aPar_;
  double bPar_;

  bool verbose_;
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
METCorrelationFilter::METCorrelationFilter(const edm::ParameterSet& iConfig)
  : 
  inputTagMET1_( iConfig.getParameter<edm::InputTag>("MET1") ),
  inputTagMET2_( iConfig.getParameter<edm::InputTag>("MET2") ),
  aPar_( iConfig.getParameter<double>("aPar") ),
  bPar_( iConfig.getParameter<double>("bPar") ),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose", false) )
  
{}


METCorrelationFilter::~METCorrelationFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
METCorrelationFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;  
  using namespace edm;

  Handle< edm::View< reco::Candidate> > MET1;
  iEvent.getByLabel(inputTagMET1_,MET1);
  const reco::Candidate& met1 = *(MET1->begin());

  Handle< edm::View< reco::Candidate> > MET2;
  iEvent.getByLabel(inputTagMET2_,MET2);
  const reco::Candidate& met2 = *(MET2->begin());
  
  if(verbose_) {
    cout<<"METCorrelationFilter,"
	<<" MET1="<< met1.pt() 
	<<" MET2="<< met2.pt() <<endl;
  }
  
  if( met1.pt() > aPar_ * met2.pt() + bPar_ ) {
    if(verbose_) cout<<" passed"<<endl;
    return true;
  }
  else {
    if(verbose_) cout<<" failed"<<endl;
    return false;
  }
}

// ------------ method called once each job just before starting event loop  ------------
void 
METCorrelationFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METCorrelationFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(METCorrelationFilter);
