#ifndef FATJETFACTORY_H_
#define FATJETFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/FatJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"

#include <TRandom3.h>
#include <algorithm>
#include <set>
#include <utility>

namespace cmg{

  class FatJetFactory : public cmg::Factory< cmg::FatJet >{
  public:

   

    FatJetFactory(const edm::ParameterSet& ps):
      collectionLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      Rmax_(ps.getParameter<double>("Rmax")){
    }

      //need to override from Factory to insert "typename"
      typedef cmg::Factory< cmg::FatJet >::event_ptr event_ptr;
      virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
        
      virtual ~FatJetFactory(){};


  private:
      const edm::InputTag collectionLabel_;
      const double Rmax_;

      //   typename cmg::FatJetFactory<T>::event_ptr result;
  };
}


cmg::FatJetFactory::event_ptr cmg::FatJetFactory::create(const edm::Event& iEvent, const edm::EventSetup&) const{
  
  
  
  typedef std::vector<cmg::FatJet> collection;
  cmg::FatJetFactory::event_ptr result(new collection);
  typedef edm::View <cmg::BaseJet > inCollection;

  edm::Handle< inCollection > cands;
  iEvent.getByLabel(collectionLabel_, cands);

  if( !cands->size() ){
  std::cout << "collection label " << collectionLabel_ << " not found" << std::endl;
  return result;  
  }

  //  for(inCollection::const_iterator it = candidates->begin(); it != candidates->end(); ++it){
  //   cands.push_back(edm::Ptr<reco::Candidate>(*(*it)));
  // }

  cmg::BaseJet lead1, lead2, cand;
  std::vector< edm::Ptr<reco::Candidate> > fat1, fat2;
  
  size_t n = cands->size();

  for (size_t i = 0; i < n; ++i) {

    cand = *(cands->refAt(i));


    /*
    if ((i > 1 && candRef->getSelection("All Cuts")) || (i < 2)) 
      cands.push_back(edm::Ptr<reco::Candidate>(candidates, i));
  }
    */


    if (i == 0) {lead1 = cand; fat1.push_back(edm::Ptr<reco::Candidate>(cands, i));}
    if (i == 1) {lead2 = cand; fat2.push_back(edm::Ptr<reco::Candidate>(cands, i));}
    
    if (i > 1 && cand.getSelection("All Cuts")){

      double dR1 = deltaR(lead1, cand);
      double dR2 = deltaR(lead2, cand);
      if (dR1 < dR2 && dR1 < Rmax_) fat1.push_back(edm::Ptr<reco::Candidate>(cands, i));
      else if (dR1 > dR2 && dR2 < Rmax_) fat2.push_back(edm::Ptr<reco::Candidate>(cands, i));
    }
      //   } else if (!(*it).getSelection("All Cuts")){
      //    std::cout << (*it).polarP4() << " failed the cut" << std::endl;
      // }


  }
  

  if (n > 0){
    cmg::FatJet fatJet1(fat1);
    cmg::FatJet fatJet2(fat2);
    
    if (fatJet2.pt() > fatJet1.pt()) {result->push_back(fatJet1); result->push_back(fatJet2);}
    else {result->push_back(fatJet2); result->push_back(fatJet1);}
  }

  return result;
  
}



#endif /*FATJETFACTORY_H_*/
