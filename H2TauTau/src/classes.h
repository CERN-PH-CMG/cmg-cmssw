#include "DataFormats/Common/interface/Wrapper.h"
#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"
#include "CMGTools/H2TauTau/interface/TauMuAnalysis.h"
#include "CMGTools/H2TauTau/interface/TauMuPlotter.h"
#include "CMGTools/H2TauTau/interface/Sample.h"

namespace {
  struct CMGTools_H2TauTau {

    BaseAnalysis baseanalysis;
    TauMuAnalysis taumuanalysis;
    TauMuPlotter taumuplotter;
    Sample s;

  };
}
