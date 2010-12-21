#ifndef __CMGTools_Common_VBFEventFilter__
#define __CMGTools_Common_VBFEventFilter__
/** \class VBFEventFilter
 *
 * \authors Colin Bernet,
 *          Artur Kalinowski
 *
 * \todo: create a smarter generic filter for filtering VBFEvents, making use of the string cut parser. Just need one template instantiation to do that, probably.
 */

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


template<typename T1, typename T2>
class VBFEventFilter : public edm::EDFilter {

  typedef cmg::VBFEventT1T2<T1,T2> VBFEvent1;
  typedef std::vector<VBFEvent1> VBFEventCollection;

 public:

  explicit VBFEventFilter(const edm::ParameterSet&);
  ~VBFEventFilter();


 private:
  virtual void beginJob(const edm::EventSetup&) {}
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() {}

  // ----------member data ---------------------------


  edm::InputTag     inputTagVbfEvent_;


  double cutPtFwdJet_;   
  double cutMjjFwdJet_;  
  double cutDetaFwdJet_;  
  double cutDphiFwdJet_;  

};
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
template<typename T1, typename T2>
  VBFEventFilter<T1,T2>::VBFEventFilter(const edm::ParameterSet& iConfig): 
  inputTagVbfEvent_ ( iConfig.getParameter<edm::InputTag>("vbfEvent") ),
    cutPtFwdJet_ (  iConfig.getParameter<double>("cutPtFwdJet") ), 
    cutMjjFwdJet_ (  iConfig.getParameter<double>("cutMjjFwdJet") ), 
    cutDetaFwdJet_ (  iConfig.getParameter<double>("cutDetaFwdJet") ), 
    cutDphiFwdJet_ (  iConfig.getParameter<double>("cutDphiFwdJet") ){

 
    if(inputTagVbfEvent_ == edm::InputTag()  ) {    
    edm::LogError("")<<" VBFEventFilter: please supply a collection of 2 forwardJets";  
    throw cms::Exception( "MissingProduct");
  }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
template<typename T1, typename T2> VBFEventFilter<T1,T2>::~VBFEventFilter() {}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
template<typename T1, typename T2>
bool VBFEventFilter<T1,T2>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){

  using namespace edm;

  Handle<VBFEventCollection> vbfEvent;
  iEvent.getByLabel(inputTagVbfEvent_,vbfEvent);

  if(vbfEvent->size()<1) return false;

  const VBFEvent1 *myVBFEvent = &(*vbfEvent)[0];

  ///Apply fileter selections
  bool isCutPtFwdJet = myVBFEvent->leg1()->p4().Et()>cutPtFwdJet_   &&  myVBFEvent->leg2()->p4().Et()>cutPtFwdJet_;
  bool isCutMjjFwdJet =   myVBFEvent->mjj() > cutMjjFwdJet_;
  bool isCutDetaFwdJet =   myVBFEvent->dEta() > cutDetaFwdJet_;
  bool isCutDphiFwdJet = fabs(myVBFEvent->dPhi()) < cutDphiFwdJet_;

  LogInfo("")<<"VBF event:"
	     <<" pt1: "<< myVBFEvent->leg1()->p4().Et()
	     <<" pt2: "<< myVBFEvent->leg2()->p4().Et()
	     <<" Mjj: "<<myVBFEvent->mjj()
	     <<" dEta: "<<myVBFEvent->dEta()
	     <<" dPhi: "<<myVBFEvent->dPhi();

  LogInfo("")<<"VBF event selections:"
	     <<" isCutPtFwdJet: "<<isCutPtFwdJet
	     <<" isCutMjjFwdJet: "<<isCutMjjFwdJet
	     <<" isCutDetaFwdJet: "<<isCutDetaFwdJet
	     <<" isCutDphiFwdJet: "<<isCutDphiFwdJet;
  
  return isCutPtFwdJet && isCutMjjFwdJet && isCutDetaFwdJet && isCutDphiFwdJet;
}

#endif



