#include "CMGTools/Common/interface/FatJetFactory.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"

cmg::FatJetFactory::event_ptr cmg::FatJetFactory::create(const edm::Event& iEvent, const edm::EventSetup&)
{

  typedef std::vector<cmg::FatJet> collection;
  cmg::FatJetFactory::event_ptr result(new collection);
  typedef edm::View <cmg::BaseJet > inCollection;

  edm::Handle< inCollection > cands;
  bool found = iEvent.getByLabel(collectionLabel_, cands);

  if( !found ) {
    std::cout << "collection label bla bla" << collectionLabel_ << " not found" << std::endl;
  return result;
  }

  cmg::BaseJet lead1, lead2, cand;
  std::vector< edm::Ptr<reco::Candidate> > fat1, fat2;

  size_t n = cands->size();

  for (size_t i = 0; i < n; ++i) {

    cand = *(cands->refAt(i));

    if (i == 0) {
      lead1 = cand; fat1.push_back(edm::Ptr<reco::Candidate>(cands, i));
    }
    if (i == 1) {
      lead2 = cand; fat2.push_back(edm::Ptr<reco::Candidate>(cands, i));
    }

    if (i > 1 && cand.getSelection("All Cuts")) {

      double dR1 = deltaR(lead1, cand);
      double dR2 = deltaR(lead2, cand);
      if (dR1 < dR2 && dR1 < Rmax_) fat1.push_back(edm::Ptr<reco::Candidate>(cands, i));
      else if (dR1 > dR2 && dR2 < Rmax_) fat2.push_back(edm::Ptr<reco::Candidate>(cands, i));
    }

  }

  if (n > 0) {
    cmg::FatJet fatJet1(fat1);
    cmg::FatJet fatJet2(fat2);

    set(fat1, fatJet1);
    set(fat2, fatJet2);

    if (fatJet2.pt() > fatJet1.pt()) {
      result->push_back(fatJet1); result->push_back(fatJet2);
    }
    else {
      result->push_back(fatJet2); result->push_back(fatJet1);
    }
  }

  return result;

}

void cmg::FatJetFactory::set(std::vector<edm::Ptr<reco::Candidate> > const& input,
                             cmg::FatJet& object) const
{
  multiObjectFactory_.set(input, &object);
}


