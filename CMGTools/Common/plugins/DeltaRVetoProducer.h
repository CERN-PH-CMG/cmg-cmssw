#ifndef __CMGTools_Common_DeltaRVetoProducer__
#define __CMGTools_Common_DeltaRVetoProducer__

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <string>

namespace cmg{

/**
 * A producer that makes a copy of the inputCollection, vetoing objects that
 * lie within minDeltaR of *any* object in the vetoCollection.
 * 
 * Algorithm is order N^2, so please use reduced collections as input
 */
template <class T>
class DeltaRVetoProducer : public edm::EDProducer {

public:
  
  explicit DeltaRVetoProducer(const edm::ParameterSet & iConfig);
  virtual ~DeltaRVetoProducer() {}
  
private:
  
  void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);
  
  const edm::InputTag inputCollection_;
  const std::vector<edm::ParameterSet> vpset_;
  const bool verbose_;

  unsigned int nColl_;  
  std::vector<edm::InputTag> vetoCollection_;
  std::vector<double> minDeltaR_;
  std::vector<bool> removeMatched_;


  std::vector<std::string> selNames_;
  std::vector<bool> selFlags_;
  typedef std::vector<T> collection;

};

}

template <class T>
cmg::DeltaRVetoProducer<T>::DeltaRVetoProducer(const edm::ParameterSet & iConfig) :
  inputCollection_(iConfig.getParameter<edm::InputTag>("inputCollection")), 
  vpset_(iConfig.getParameter<std::vector<edm::ParameterSet> >("MatchingParams")),
  verbose_( iConfig.getUntrackedParameter<bool>("verbose",false ) ){
	
  nColl_=vpset_.size();
   
  // for (std::vector<edm::ParameterSet>::const_iterator iPSet = vpset_.begin();
  //		iPSet != vpset_.end(); ++iPSet) {
  for(unsigned int iColl=0;iColl<nColl_;iColl++){
    edm::ParameterSet iPSet=vpset_.at(iColl);
    vetoCollection_.push_back(iPSet.getParameter<edm::InputTag>("vetoCollection"));
    minDeltaR_.push_back(iPSet.getParameter<double>("minDeltaR"));
    removeMatched_.push_back(iPSet.getParameter<bool>("removeMatchedObject"));
  }
	
  produces<collection>();
}//end constructor

template <class T>
void cmg::DeltaRVetoProducer<T>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {

    edm::Handle<collection> input;
    iEvent.getByLabel(inputCollection_, input);
    //Order N^2 so lets hope there aren't too many veto objects. Will be fine for any sensible use
    unsigned int key = 0;
    std::auto_ptr<collection> result(new collection); 

    for(typename collection::const_iterator it = input->begin(); it != input->end(); ++it, ++key){
      bool accept = true;

 
      for(unsigned int iColl=0;iColl<nColl_;iColl++){
	edm::Handle<edm::View<reco::Candidate> > vetos;
	iEvent.getByLabel(vetoCollection_.at(iColl), vetos);
	std::string selName=vetoCollection_.at(iColl).label();
	std::string instanceName=vetoCollection_.at(iColl).instance();
	if(instanceName!="")selName=selName+"_"+instanceName;
	selNames_.push_back("matched_"+selName);


	if(verbose_&& vetos->size()==0)std::cout<<"Empty veto Collection named "<<selName.c_str()<<std::endl;

	bool matched=false;
        for(edm::View<reco::Candidate>::const_iterator jt = vetos->begin(); jt != vetos->end(); ++jt){
            const double dR = reco::deltaR(*it,*jt);
            matched = (dR < minDeltaR_.at(iColl));
            if(verbose_){
	      std::cout << "DeltaRVetoProducer (dR < "<<minDeltaR_.at(iColl)<<"): pt_Input/ptVeto("<<selName.c_str()<<")/deltaR " << it->pt() << "/" << jt->pt() << "/" << dR << " Reject: " << matched << std::endl;
            }

            if(matched){
                accept = false;
                break;
            }
	   
        }//end loop on veto Collection
	selFlags_.push_back(matched);
      }//end loop on list of veto Collections to be matched 

      //after having looped over all the veto Collections, we store in the CMG object
      //the individual results of the matching and - if none of them was positive - 
      //we save it in the output
      T accepted(*it);
      //set the source pointer for the TopProjector
      accepted.setSourceCandidatePtr( reco::CandidatePtr( input, key ) );
      bool store=true;
      for(unsigned int iColl=0;iColl<nColl_;iColl++){
	accepted.addSelection(selNames_.at(iColl).c_str(), selFlags_.at(iColl));
	if(!accept){
	  //the input object was matched to something else; check that for the
	  //collection of this smtg else it was requested to not remove the input
	  //even in case of matching
	  if(store){
	    if(selFlags_.at(iColl)&&!removeMatched_.at(iColl))store=true;
	    else store=false;
	  }
	}

      }//end 2nd  loop on list of veto Collections
      
      if(store){

	result->push_back(accepted);
      }
    }//end loop on input objects
    iEvent.put(result); 
 }//end produce

#endif

/* #include "FWCore/Framework/interface/MakerMacros.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/BaseJet.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/PFJet.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/Electron.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/Muon.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/Tau.h" */
/* typedef cmg::DeltaRVetoProducer<cmg::BaseJet> DeltaRVetoProducerBaseJet; */
/* typedef cmg::DeltaRVetoProducer<cmg::PFJet> DeltaRVetoProducerPFJet; */
/* typedef cmg::DeltaRVetoProducer<cmg::Electron> DeltaRVetoProducerElectron; */
/* typedef cmg::DeltaRVetoProducer<cmg::Muon> DeltaRVetoProducerMuon; */
/* typedef cmg::DeltaRVetoProducer<cmg::Tau> DeltaRVetoProducerTau; */

/* DEFINE_FWK_MODULE(DeltaRVetoProducerBaseJet); */
/* DEFINE_FWK_MODULE(DeltaRVetoProducerPFJet); */
/* DEFINE_FWK_MODULE(DeltaRVetoProducerElectron); */
/* DEFINE_FWK_MODULE(DeltaRVetoProducerMuon); */
/* DEFINE_FWK_MODULE(DeltaRVetoProducerTau); */
