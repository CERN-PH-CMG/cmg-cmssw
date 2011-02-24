#ifndef HEMISPHERE_H_
#define HEMISPHERE_H_

#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"

namespace cmg {

class Hemisphere;
class HemisphereFactory;

class Hemisphere : public MultiObject {

  typedef edm::Ptr<reco::Candidate> SrcPtr;

 public:
  Hemisphere() : MultiObject()
    {
    }

  Hemisphere(std::vector<SrcPtr> sourcePtrs) :
    MultiObject(sourcePtrs)
    {
    }

  friend class cmg::HemisphereFactory;

};

} // namespace cmg

#endif /*HEMISPHERE_H_*/
