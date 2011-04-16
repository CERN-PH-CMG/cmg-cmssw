#include "CMGTools/DiJetHighMass/interface/JetCountingHistograms.h"

void cmg::JetCountingHistograms::defineHistograms(){

  if(!histosInitialized_){
   
    add1DHistogram("NjetsEx", "Exclusive Numbers of Jets", 10, 0., 10, file_);

    histosInitialized_ = true;
  }
  
}

void cmg::JetCountingHistograms::fill(const type& cand){
}

void cmg::JetCountingHistograms::fill(const edm::Event& iEvent, const edm::EventSetup&){

    edm::Handle<view> cands;
    iEvent.getByLabel(label_,cands);
    
    for(view::const_iterator it = cands->begin(); it != cands->end(); ++it){
        fill(*it);   
    }

}
