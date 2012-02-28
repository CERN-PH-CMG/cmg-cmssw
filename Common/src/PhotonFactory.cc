#include "CMGTools/Common/interface/PhotonFactory.h"

cmg::PhotonFactory::event_ptr
cmg::PhotonFactory::create(const edm::Event& iEvent,
                           const edm::EventSetup&)
{
  edm::Handle<pat::PhotonCollection> cands;

  cmg::PhotonFactory::event_ptr result(new cmg::PhotonFactory::collection);
  iEvent.getByLabel(photonLabel_, cands);

  for(size_t i = 0, end = cands->size(); i != end; ++i) {

    pat::PhotonPtr photonPtr(cands, i);
    cmg::Photon photon(photonPtr);
//     set(photonPtr, &photon);

    result->push_back(photon);
  }
  return result;
}

//--------------------------------------------------------------------

// void
// cmg::PhotonFactory::set(const pat::PhotonPtr& input,
//                          cmg::Photon* const output) const
// {}

//--------------------------------------------------------------------
