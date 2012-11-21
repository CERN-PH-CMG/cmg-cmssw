#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CMGTools/Common/plugins/TrackIsolationFilter.h"

#include <iostream>
#include <vector>

bool TrackIsolationFilter::filter(edm::Event& iEvent, const edm::EventSetup&){

  edm::Handle<std::vector<float> > pfcandsptH;
  edm::Handle<std::vector<float> > pfcandstrkisoH;
  edm::Handle<std::vector<int> > pfcandschgH;

  iEvent.getByLabel(pfcands_trkiso_tag_,pfcandsptH);
  iEvent.getByLabel(pfcands_pt_tag_,pfcandstrkisoH);
  iEvent.getByLabel(pfcands_chg_tag_,pfcandschgH);

  bool result = true;
  for(unsigned int i = 0; i < pfcandsptH->size(); i++){
    const double pt = pfcandsptH->at(i);
    //we only consifer charged tracks with pt above the threshold
    if( (pt < pt_cut_) || (pfcandschgH->at(i) == 0) ) continue;
    const double relIso = pfcandstrkisoH->at(i)/pt;
    if(relIso <= relIso_cut_){
      //we have an isolated track
      result = false;
      break;
    }
  }
  return result;

}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackIsolationFilter);
