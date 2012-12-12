#ifndef __CMGTools_Common_JetEnergyCorrector_h__
#define __CMGTools_Common_JetEnergyCorrector_h__

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "CommonTools/Utils/interface/PtComparator.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace cmg{

template<class T>
class JetEnergyCorrector : public edm::EDProducer {

 public:
  
  typedef std::vector<T> collection;
  typedef std::auto_ptr<collection> event_ptr;
  typedef edm::View<T> view;
  
  explicit JetEnergyCorrector(const edm::ParameterSet& ps):
    levels_(ps.getParameter<std::vector<std::string> >("levels")),
    payload_(ps.getParameter<std::string>("payload")),
    src_(ps.getParameter<edm::InputTag>("src")),
    rho_(ps.getParameter<edm::InputTag>("rho")), 
    vtx_(ps.getParameter<edm::InputTag>("vertices")),
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)),
    sort_(ps.getParameter<bool>("sort")),
    initialized_(false){
    produces<collection>("");
  }
  void endJob();
  void produce(edm::Event& iEvent, const edm::EventSetup&);
 
 private:

  const std::vector<std::string> levels_;
  const std::string payload_;
  const edm::InputTag src_;
  const edm::InputTag rho_;
  const edm::InputTag vtx_;
  const bool verbose_;
  const bool sort_;
  bool initialized_;
  std::vector<JetCorrectorParameters> vpar;
  FactorizedJetCorrector *jecCor_;
  JetCorrectionUncertainty *jecUnc_;
};
}

template<class T>
void cmg::JetEnergyCorrector<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
    if(!initialized_) {
      //-------- get the parameters collection from the database --------------- 
      edm::ESHandle<JetCorrectorParametersCollection> parameters;
      iSetup.get<JetCorrectionsRecord>().get(payload_,parameters);
      //-------- fill a vector with the parameter objects of interest ----------
      for(std::vector<std::string>::const_iterator ll = levels_.begin(); ll != levels_.end(); ++ll){ 
        const JetCorrectorParameters& ip = (*parameters)[*ll];
        if(verbose_){ 
          std::cout << "Adding level " << *ll << std::endl;
        }
        vpar.push_back(ip); 
      } 
      //-------- initialize the FactorizedJetCorrector --------------------------
      jecCor_ = new FactorizedJetCorrector(vpar);
      //-------- get the Uncertainty parameter object ---------------------------
      const JetCorrectorParameters& ip = (*parameters)["Uncertainty"];
      //-------- initialize the JetCorrectionUncertainty ------------------------
      jecUnc_ = new JetCorrectionUncertainty(ip);    
      initialized_ = true; 
    }

    edm::Handle<view> cands;
    iEvent.getByLabel(src_,cands);
    
    edm::Handle< double > rhoHandle;
    iEvent.getByLabel(rho_, rhoHandle);
    double rho = *rhoHandle;

    edm::Handle<reco::VertexCollection> vtxHandle;
    iEvent.getByLabel(vtx_,vtxHandle);
    unsigned nvtx = vtxHandle->size();

    if(verbose_){
      std::cout << "correcting jet collection " << src_.label() << " with " << cands->size() << " entries." << std::endl;
    }
    typename cmg::JetEnergyCorrector<T>::event_ptr result(new collection());

    if(verbose_)
      std::cout<<"rho = "<<rho<<std::endl;

    for(unsigned int i = 0; i < cands->size(); ++i){
      const T& cand = cands->at(i);
      float raw_pt = cand.pt() * cand.rawFactor();
      jecCor_->setJetEta(cand.eta());
      jecCor_->setJetPt(raw_pt);
      jecCor_->setJetA(cand.jetArea());
      jecCor_->setJetE(cand.energy());
      jecCor_->setRho(rho);
      jecCor_->setNPV(nvtx);
      float jec = jecCor_->getCorrection();
      float cor_pt = raw_pt * jec;
      jecUnc_->setJetEta(cand.eta());
      jecUnc_->setJetPt(cor_pt);
      float unc = jecUnc_->getUncertainty(true);
      result->push_back(cand);
      result->at(i).setP4(cand.rawFactor()*jec*cand.p4());
      result->at(i).rawFactor_ = 1./jec;
      result->at(i).uncOnFourVectorScale_ = unc;
      if(verbose_)
	std::cout<<"\t raw pt: "<<raw_pt<<", eta: "<<cand.eta()<<", old pt: "<<cand.pt()<<", old unc: "<<cand.uncOnFourVectorScale()<<", area:"<<cand.jetArea()<<", new pt: "<<result->at(i).pt()<<", new unc: "<<result->at(i).uncOnFourVectorScale()<<std::endl;
    }
    if(sort_) {
      NumericSafeGreaterByPt<T> compare_rule;
      std::sort(result->begin(),result->end(),compare_rule);
    }
    iEvent.put(result);
    return;
}

template<class T>
void cmg::JetEnergyCorrector<T>::endJob()
{
  delete jecCor_;
  delete jecUnc_;
}

#endif
