#ifndef __CMGTools_Common_VBFEventT1T2Producer__
#define __CMGTools_Common_VBFEventT1T2Producer__
/** \class VBFEventT1T2Producer
 *
 * \authors Colin Bernet,
 *          Artur Kalinowski
 *
 *
 */


#include <stdio.h>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2Fwd.h"


template<typename T1, typename T2>
class VBFEventT1T2Producer : public edm::EDProducer {

 public:

  typedef cmg::VBFEventT1T2<T1,T2> VBFEvent;
  typedef std::vector<VBFEvent> VBFEventCollection;


  explicit VBFEventT1T2Producer(const edm::ParameterSet&);
  ~VBFEventT1T2Producer() {}
  
 private:
  //virtual void beginJob(const edm::EventSetup&) {}
  virtual void produce(edm::Event&, const edm::EventSetup&);
  //virtual void endJob(){}
    
  // ----------member data ---------------------------

  edm::InputTag   inputTagForwardJets_;
  edm::InputTag   inputTagCentralObjects_;
  float etaMargin_;

};
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
template<typename T1, typename T2> VBFEventT1T2Producer<T1,T2>::VBFEventT1T2Producer(const edm::ParameterSet& iConfig):
    inputTagForwardJets_ ( iConfig.getParameter<edm::InputTag>("ForwardJets") ),
      inputTagCentralObjects_ ( iConfig.getParameter<edm::InputTag>("CentralObjects") ),
      etaMargin_ ( iConfig.getParameter<double>("etaMargin") ){


    produces<VBFEventCollection>("");
  }
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
template<typename T1, typename T2> void VBFEventT1T2Producer<T1,T2>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::LogInfo("")<<"VBFEventT1T2Producer<T1,T2>::produce() start";

  typedef edm::View<T1> T1View;
  edm::Handle<T1View> tagCollection;
  iEvent.getByLabel(inputTagForwardJets_,tagCollection);

  edm::LogInfo("")<<"Tag collection size: "<< tagCollection->size();

  if(tagCollection->size()<2){
    edm::LogInfo("")<<"Tag collection size: "<< tagCollection->size()<<" <2. Cannot create VBF event!";
    std::auto_ptr<VBFEventCollection> emptyVBFT1T2EventCollection(new  VBFEventCollection());
    iEvent.put(emptyVBFT1T2EventCollection);
    return;    
  }

  std::auto_ptr<VBFEventCollection> myVBFT1T2EventCollection(new  VBFEventCollection());

  ///Set the tagging objects
  VBFEvent  myVBFEvent(tagCollection->ptrAt(0), tagCollection->ptrAt(1));
  myVBFEvent.setEtaMargin(etaMargin_);

  float etaMax  = myVBFEvent.etaMax();
  float etaMin  = myVBFEvent.etaMin();
  float etaMargin = myVBFEvent.etaMargin();

  ///Add the veto objects
  typedef edm::View<T2> T2View;
  edm::Handle<T2View> vetoCollection;
  typename VBFEvent::T2PtrVector tmpObjCollection;

  std::multimap<float, int> index_;   ///Temporary index multimap to be used for
                                      ///pt sorted TPtrVector collection

  iEvent.getByLabel(inputTagCentralObjects_,vetoCollection);

  edm::LogInfo("")<<"etaMin, etaMax, etaMargin: "
		  <<etaMin<<", "<<etaMax<<", "<<etaMargin;

  for(unsigned index=0;index<vetoCollection->size();++index){

    float etaCand = (*vetoCollection)[index].p4().Eta();

    edm::LogInfo("")<<"Considering object with (eta,phi,Et): ("
		    <<(*vetoCollection)[index].p4().Eta()<<", "
		    <<(*vetoCollection)[index].p4().Phi()<<", "
		    <<(*vetoCollection)[index].p4().Et()<<")";

    if(etaCand>(etaMin + etaMargin) && 
       etaCand<(etaMax - etaMargin)) {

      ///Use -pt as the key to have descending ordering.
      index_.insert(std::pair<float,int>(-(*vetoCollection)[index].p4().Et(),index) );

      
      edm::LogInfo("")<<"Adding object with (eta,phi,Et): ("
		      <<(*vetoCollection)[index].p4().Eta()<<", "
		      <<(*vetoCollection)[index].p4().Phi()<<", "
		      <<(*vetoCollection)[index].p4().Et()<<")";

      //tmpObjCollection.push_back(vetoCollection->at(index));    
    }
  }

  std::multimap<float, int>::const_iterator ci = index_.begin();
  for(;ci!=index_.end();++ci) tmpObjCollection.push_back(vetoCollection->ptrAt(ci->second));    
  myVBFEvent.setVetoCollection(tmpObjCollection);
  
  myVBFT1T2EventCollection->push_back(myVBFEvent);

  iEvent.put(myVBFT1T2EventCollection);

  edm::LogInfo("")<<"VBFEventT1T2Producer<T1,T2>::produce() end";
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#endif

