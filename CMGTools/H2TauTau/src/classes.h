#include "DataFormats/Common/interface/Wrapper.h"
#include "CMGTools/H2TauTau/interface/Sample.h"
#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/SelectionEfficiency.h"

namespace {
  struct CMGTools_H2TauTau {

    Sample samp;
    
    TriggerEfficiency trigeff;
    SelectionEfficiency sigeff;

  };
}
