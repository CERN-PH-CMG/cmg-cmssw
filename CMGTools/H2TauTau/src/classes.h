#include "DataFormats/Common/interface/Wrapper.h"
#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/VBFMVA.h"
#include "CMGTools/H2TauTau/interface/VBFMVA2012.h"
#include "CMGTools/H2TauTau/interface/METSignificance.h"

namespace {
  struct CMGTools_H2TauTau {

    TriggerEfficiency trigeff;
    VBFMVA            vbfMva ;
    VBFMVA2012        vbfMva2012 ;

    cmg::METSignificance metsig_;
    edm::Wrapper<cmg::METSignificance> metsige_;
    std::vector<cmg::METSignificance> metsigv_;
    edm::Wrapper<std::vector<cmg::METSignificance> > metsigve_;
  };
}
