#ifndef CMGTOOLS_COMMON_MULTIOBJECTFACTORY_H_
#define CMGTOOLS_COMMON_MULTIOBJECTFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"
#include "CMGTools/Common/interface/SettingTool.h"

//--------------------------------------------------------------------

namespace cmg {

  class MultiObjectSettingTool :
  public SettingTool<std::vector<edm::Ptr<reco::Candidate> >, cmg::MultiObject> {

  public:
    void set(std::vector<edm::Ptr<reco::Candidate> > const& input,
             cmg::MultiObject* const object) const;

  private:
    double deltaPhiStar(std::vector<edm::Ptr<reco::Candidate> > const&
                        input) const;

    double ht(std::vector<edm::Ptr<reco::Candidate> > const&
                        input) const;
  };

} // namespace cmg

//--------------------------------------------------------------------

#endif /*CMGTOOLS_COMMON_MULTIOBJECTFACTORY_H_*/
