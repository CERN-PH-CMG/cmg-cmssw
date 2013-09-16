#ifndef FATJET_H_
#define FATJET_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"

namespace cmg {

class FatJet;
class FatJetFactory;

class FatJet : public MultiObject {

  typedef edm::Ptr<reco::Candidate> SrcPtr;

 public:
  FatJet() : MultiObject()
    {
    }

  FatJet(std::vector<SrcPtr> sourcePtrs) :
    MultiObject(sourcePtrs)
    {
    }

  friend class cmg::FatJetFactory;

};

} // namespace cmg

#endif /*FATJET_H_*/
