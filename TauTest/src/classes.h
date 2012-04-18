#include "DataFormats/Common/interface/Wrapper.h"
#include "CMGTools/TauTest/interface/TestPatTaus.h"
#include "CMGTools/TauTest/interface/TestCMGTaus.h"
#include "CMGTools/TauTest/interface/TestCMGElectrons.h"
#include "CMGTools/TauTest/interface/TestCMGMuons.h"
#include "CMGTools/TauTest/interface/TestTrig.h"
#include "CMGTools/TauTest/interface/TestGenParticles.h"
#include "CMGTools/TauTest/interface/TauEfficiencyPlot.h"
#include "CMGTools/TauTest/interface/TauEfficiency.h"

namespace {
  struct CMGTools_TauTest {
    
    TestPatTaus testpattaus;
    TestCMGTaus testcmgtaus;
    TestCMGElectrons testcmgelectrons;
    TestCMGMuons testcmgmuons;
    TestTrig testtrig;
    TestGenParticles testgenparticles;
    TauEfficiencyPlot taueffplot;
    TauEfficiency taueff;
    
  };  
}
