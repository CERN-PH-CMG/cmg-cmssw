#include "DataFormats/Common/interface/Wrapper.h"
#include "CMGTools/H2TauTau/interface/PlotDiTaus.h"
#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"
#include "CMGTools/H2TauTau/interface/DiTauAnalysis.h"
#include "CMGTools/H2TauTau/interface/TauMuAnalysis.h"
#include "CMGTools/H2TauTau/interface/Sample.h"

namespace {
  struct CMGTools_H2TauTau {

    PlotDiTaus plotditaus;
    BaseAnalysis baseanalysis;
    DiTauAnalysis ditauanalysis;
    TauMuAnalysis taumuanalysis;
    Sample s;

  };
}
