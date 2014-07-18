#include "DataFormats/Common/interface/Wrapper.h"
#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/QCDEstimate.h"
#include "CMGTools/H2TauTau/interface/VBFMVA.h"
#include "CMGTools/H2TauTau/interface/VBFMVA2012.h"
#include "CMGTools/H2TauTau/interface/BTagEfficiency.h"
#include "CMGTools/H2TauTau/interface/BTagWeight.h"
#include "CMGTools/H2TauTau/interface/METSignificance.h"

namespace {
  struct CMGTools_H2TauTau {

    TriggerEfficiency trigeff;
    QCDEstimate       qcdestimate;
    VBFMVA            vbfMva ;
    VBFMVA2012        vbfMva2012 ;
    BTagEfficiency    btageff;
    BTagWeight        btagw;
  };
}

namespace cmg {
    METSignificance metsig;
}