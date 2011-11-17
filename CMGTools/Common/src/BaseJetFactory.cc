#include "CMGTools/Common/interface/BaseJetFactory.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <cmath>

cmg::BaseJetFactory::BaseJetFactory(const edm::ParameterSet& ps):
  jetLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  btagType_(ps.getParameter<std::vector<std::string> >("btagType")),
  fillJec_(ps.getParameter<bool>("fillJec")),
  fillJecUncertainty_(ps.getParameter<bool>("fillJecUncertainty")),
  jecPath_(""),
  JES_(0)
{
  if(btagType_.size() > cmg::BaseJet::TagNameArray::static_size){
    edm::LogWarning("cmg::BaseJetFactory") << "Too many B-tags. The maximum number is fixed."  << std::endl;
  }  
    
  if (fillJecUncertainty_) {
    jecPath_ = ps.getParameter<std::string>("jecPath");
    edm::LogInfo("BaseJetFactory") << "Loading JEC uncertainties from '"
                                   << jecPath_ << "'" << std::endl;
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
  iEvent.getByLabel(jetLabel_, jetCands);

  unsigned index = 0;
  for(pat::JetCollection::const_iterator mi = jetCands->begin();
      mi != jetCands->end(); ++mi, ++index) {

    pat::JetPtr jetPtr(jetCands, index);
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
    for(unsigned int i = 0; i < btagType_.size(); i++){
        const std::string tag(btagType_.at(i));
        output->btag_[i] = input->bDiscriminator(tag);
        output->btagNames_[i] = tag;
    }
    output->partonFlavour_ = input->partonFlavour();
    
    output->jetArea_ = input->jetArea();

    if (fillJec_) {
      output->rawFactor_ = input->jecFactor(0);
    }

    //there is an Exception at eta > 5.0
    if (fillJecUncertainty_ && (fabs(input->eta()) <= 5.) ){
      JES_->setJetEta(input->eta());
      // NOTE: This should be the L2L3-corrected pT.
      JES_->setJetPt(input->pt());
      float unc = JES_->getUncertainty(true);
      output->uncOnFourVectorScale_ = unc;
    }
}
