#ifndef _CMGTools_Susy_TriggerDataMenu_H_
#define _CMGTools_Susy_TriggerDataMenu_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"

#include <cmath>
#include <iostream>
#include <memory>

#include "TRandom.h"
#include "TRandom3.h"

class TriggerDataMenu : public edm::EDFilter{

 public:
  typedef std::vector<cmg::TriggerObject> collection;

  TriggerDataMenu(const edm::ParameterSet& ps):
    triggerObjects_(ps.getParameter<edm::InputTag>("triggerObjects")),
    trigger_(ps.getParameter<edm::InputTag>("trigger")),
    random_(new TRandom3){
    produces<int>();
  }

  bool scale(const int lumi, const bool fired){
    ///Scale the trigger by the lumi where it was present in the menu
    bool result = fired;
    if(fired){
      const int ran = random_->Integer(5100);
      result = (ran <= lumi);
    }
    return result;
  }
  bool objectCut(const char* selection, const float pt, const int min, const collection& objects) const{
    ///loop on the trigger objects and find out how many there are and with what pt
    int count = 0;
    //the collection should be hltAntiKT5L2L3CorrCaloJets, so this seems OK
    for(collection::const_iterator it = objects.begin(); it != objects.end(); ++it){
      if( it->getSelectionRegExp(selection) && (it->pt() >= pt) ){
	count++;
	if(count == min) break;
      }
    }
    return count >= min;
  }
  
  bool htl_QuadJet60(const cmg::TriggerObject& hlt, const collection&){
    const bool fired = hlt.getSelectionRegExp("^HLT_QuadJet60_v[0-9]+$");
    return scale(1000,fired);
  }
  bool htl_QuadJet70(const cmg::TriggerObject& hlt, const collection&){
    return hlt.getSelectionRegExp("^HLT_QuadJet70_v[0-9]+$");
  }
  bool htl_QuadJet80(const cmg::TriggerObject& hlt, const collection& objects){
    const char* selection = "^HLT_QuadJet70_v[0-9]+$";
    const bool fired = hlt.getSelectionRegExp(selection) &&
      objectCut(selection,70.,4,objects);
    return scale(2800,fired);
  }
  bool htl_QuadJet50_Jet40(const cmg::TriggerObject& hlt, const collection& objects){
    const char* selection = "^HLT_QuadJet40_v[0-9]+$";
    const bool fired = hlt.getSelectionRegExp(selection) &&
      objectCut(selection,40.,5,objects) &&
      objectCut(selection,50.,4,objects);
    return scale(412,fired);
  }
  bool htl_QuadJet50_DiJet40(const cmg::TriggerObject& hlt, const collection& objects){
    const char* selection = "^HLT_QuadJet40_v[0-9]+$";
    const bool fired = hlt.getSelectionRegExp(selection) &&
      objectCut(selection,40.,6,objects) &&
      objectCut(selection,50.,4,objects);
    return scale(2700,fired);
  }
  bool htl_SixJet45(const cmg::TriggerObject& hlt, const collection& objects){
    const char* selection = "^HLT_QuadJet40_v[0-9]+$";
    const bool fired = hlt.getSelectionRegExp(selection) &&
      objectCut(selection,45.,6,objects);
    return scale(850,fired);
  }
  bool htl_EightJet40(const cmg::TriggerObject& hlt, const collection& objects){
    const char* selection = "^HLT_QuadJet40_v[0-9]+$";
    const bool fired = hlt.getSelectionRegExp(selection) &&
      objectCut(selection,40.,8,objects);
    return scale(850,fired);
  }

  virtual bool filter(edm::Event& iEvent, const edm::EventSetup&){
    
    edm::Handle<collection> handleTrigger;
    iEvent.getByLabel( trigger_, handleTrigger );
    cmg::TriggerObject hlt = handleTrigger->at(0);
    
    edm::Handle<collection> handleObjects;
    iEvent.getByLabel( triggerObjects_, handleObjects );

    const bool result = htl_QuadJet60(hlt, *handleObjects) ||
      htl_QuadJet70(hlt, *handleObjects) ||
      htl_QuadJet80(hlt, *handleObjects) ||
      htl_QuadJet50_Jet40(hlt, *handleObjects) ||
      htl_QuadJet50_DiJet40(hlt, *handleObjects) ||
      htl_SixJet45(hlt, *handleObjects) ||
      htl_EightJet40(hlt, *handleObjects);
    
    std::auto_ptr<int> output( new int(result) ); 
    iEvent.put( output );

    return true; 
  }

 private:
  edm::InputTag triggerObjects_;
  edm::InputTag trigger_;

  std::auto_ptr<TRandom> random_;
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TriggerDataMenu);

#endif
