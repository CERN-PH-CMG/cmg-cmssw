#include "CMGTools/Common/interface/BaseMETModificationFactory.h"

///Recalculate the MET using any collection of candidates
cmg::BaseMETModificationFactory::event_ptr cmg::BaseMETModificationFactory::create(const edm::Event& iEvent, const edm::EventSetup&){
  // here read a view 
  typedef edm::View<cmg::BaseMET> MetView;
  
  edm::Handle<reco::CandidateView> inputs;
  iEvent.getByLabel(inputLabel_,inputs);
  
  edm::Handle<MetView> mets;
  iEvent.getByLabel(metLabel_,mets);
  
  cmg::BaseMETModificationFactory::event_ptr result(new cmg::BaseMETModificationFactory::collection);
  
  //loop over the met objects
  for(MetView::const_iterator met = mets->begin(); met != mets->end(); ++met) {
  
    double sumEx = 0;
    double sumEy = 0;
    double sumEt = 0;
    int sumCharge = 0;  

    for(reco::CandidateView::const_iterator mi = inputs->begin(); mi != inputs->end(); ++mi) {

        const reco::Candidate& cand = *mi;

        if(cand.pt()<ptThreshold_ ) continue;

        double et = 0;
        double ex = 0;
        double ey = 0;
        cmg::BaseMETFactory<reco::Candidate>::calcET(cand, &ex, &ey, &et);
    
        sumEx += ex;
        sumEy += ey;
        sumEt += et;
        sumCharge += cand.charge();     
  }

    const LorentzVector missingEt( -sumEx, -sumEy, 0, sqrt( sumEx*sumEx + sumEy*sumEy) );
    const LorentzVector metVector(met->p4());
    const int metCharge = met->charge();
    
    if(operator_ == "+"){
        /// we add the extra contribution           
        cmg::BaseMET bm(reco::LeafCandidate(metCharge+sumCharge,metVector+missingEt));
        bm.sumEt_ = met->sumEt() + sumEt;
        result->push_back(bm);
    }else{
        ///default is '-' 
        cmg::BaseMET bm(reco::LeafCandidate(metCharge-sumCharge,metVector-missingEt));
        bm.sumEt_ = met->sumEt() - sumEt;
        result->push_back(bm); 
    }

  }

  return result;
}
