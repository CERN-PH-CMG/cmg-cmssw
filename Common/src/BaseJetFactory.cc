#include "CMGTools/Common/interface/BaseJetFactory.h"

#include <iostream>

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"

cmg::BaseJetFactory::BaseJetFactory(const edm::ParameterSet& ps):
  jetLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  btagType_(ps.getParameter<std::string>("btagType")),
  applyJecUncertainty_(ps.getParameter<bool>("applyJecUncertainty")),
  jecPath_(""),
  jecUncDirection_(0),
  JES_(0)
{
  if (applyJecUncertainty_) {
    jecPath_ = ps.getParameter<std::string>("jecPath");
    jecUncDirection_ = ps.getParameter<int>("jecUncDirection");
    edm::LogInfo("BaseJetFactory") << "Loading JEC uncertainties from '"
                                   << jecPath_ << "'" << std::endl;
    if (jecUncDirection_ == 1) {
      edm::LogInfo("BaseJetFactory")
        << "Scaling jets up by JEC uncertainty" << std::endl;
    } else if (jecUncDirection_ == -1) {
      edm::LogInfo("BaseJetFactory")
        << "Scaling jets down by JEC uncertainty" << std::endl;
    } else {
      edm::LogWarning("BaseJetFactory")
        << "Received strange value for JEC uncertainty direction "
        << "('jecUncDirection'): " << jecUncDirection_
        << " --> switching JEC uncertainty off!"
        << std::endl;
      jecUncDirection_ = 0;
    }
    edm::FileInPath jecPathFull(jecPath_);
    JES_ = new JetCorrectionUncertainty(jecPathFull.fullPath());
  }

}

cmg::BaseJetFactory::~BaseJetFactory()
{
  if (JES_) {
    delete JES_;
  }
  JES_ = 0;
}

cmg::BaseJetFactory::event_ptr cmg::BaseJetFactory::create(const edm::Event& iEvent,
                                                           const edm::EventSetup&) const
{

  edm::Handle<pat::JetCollection> jetCands;

  cmg::BaseJetFactory::event_ptr result(new cmg::BaseJetFactory::collection);
  iEvent.getByLabel(jetLabel_,jetCands);

  unsigned index = 0;
  for(pat::JetCollection::const_iterator mi = jetCands->begin();
      mi != jetCands->end(); ++mi, ++index) {

    pat::JetPtr jetPtr(jetCands,index);
    cmg::BaseJet jet(jetPtr);
    set(jetPtr, &jet);

    result->push_back(jet);
  }
  return result;
}

/// Refactor out the setting of properties so that it can be used
/// elsewhere.
void cmg::BaseJetFactory::set(const pat::JetPtr& input,
                              cmg::BaseJet* const output) const
{
    output->btag_ = input->bDiscriminator(btagType_);
    output->rawFactor_ = input->jecFactor(0);

    if (applyJecUncertainty_) {
      JES_->setJetEta(input->eta());
      // NOTE: This should be the L2L3-corrected pT.
      JES_->setJetPt(input->pt());
      reco::Candidate::LorentzVector fourVec = input->p4();
      float unc = JES_->getUncertainty(true);
      fourVec *= (1. + (jecUncDirection_ * unc));
      output->setP4(fourVec);
    }
}
