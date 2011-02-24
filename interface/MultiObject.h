#ifndef MULTIOBJECT_H_
#define MULTIOBJECT_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <vector>

namespace cmg {

  class MultiObject;
  class MultiObjectFactory;

  class MultiObject : public AbstractPhysicsObject {

    typedef edm::Ptr<reco::Candidate> SrcPtr;

  public:

    MultiObject(std::vector<SrcPtr> sourcePtrs) :
      sourcePtrs_(sourcePtrs)
      {
        Charge qSum = 0;
        LorentzVector p4Sum(0., 0., 0., 0.);
        for (std::vector<SrcPtr>::const_iterator
               it = sourcePtrs.begin(); it != sourcePtrs.end(); ++it) {
          qSum += (*it)->charge();
          p4Sum += (*it)->p4();
        }
        setCharge(qSum);
        setP4(p4Sum);
      }

    MultiObject() :
      AbstractPhysicsObject::AbstractPhysicsObject()
      {
      }

    std::vector<SrcPtr> const& sourcePtrs() const {
      return sourcePtrs_;
    }

    SrcPtr const* sourcePtr(size_t const ind) const {
      return &sourcePtrs_.at(ind);
    }

    size_t const numConstituents() const {
      return sourcePtrs_.size();
    }

  private:
    std::vector<SrcPtr> sourcePtrs_;

    friend class cmg::MultiObjectFactory;

  };

} // namespace cmg

#endif /*MULTIOBJECT_H_*/
