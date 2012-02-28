#include "CMGTools/Common/interface/MultiObjectFactory.h"

#include "DataFormats/Common/interface/View.h"

//--------------------------------------------------------------------

cmg::MultiObjectFactory::event_ptr
cmg::MultiObjectFactory::create(edm::Event const& iEvent,
                                edm::EventSetup const& iSetup)
{
  typedef std::vector<cmg::MultiObject> collection;
  cmg::MultiObjectFactory::event_ptr result(new collection);

  // There really is not much to be done here, create a multi-object
  // from the stuff in the input collection, then fill the few things
  // in it and done.

  edm::Handle<edm::View<reco::Candidate> > cands;
  iEvent.getByLabel(inputLabel_, cands);

  if (cands->size() > 0) {
    std::vector<edm::Ptr<reco::Candidate> > tmp;
    tmp.resize(cands->size());
    for (size_t i = 0, n = cands->size(); i < n; ++i) {
      tmp.at(i) = edm::Ptr<reco::Candidate>(cands, i);
    }

    cmg::MultiObject object(tmp);
    cmg::MultiObjectFactory::set(tmp, object);
    result->push_back(object);
  }

  return result;
}

//--------------------------------------------------------------------

void cmg::MultiObjectFactory::set(std::vector<edm::Ptr<reco::Candidate> >
                                  const& input, cmg::MultiObject& object) const
{
  multiObjectSettingTool_.set(input, &object);
}

//--------------------------------------------------------------------
