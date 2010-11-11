#ifndef _AnalysisDataFormats_CMGTools_PatTypes_h_
#define _AnalysisDataFormats_CMGTools_PatTypes_h_

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"

namespace pat {
  
  typedef edm::Ptr< pat::Jet > JetPtr;
  typedef edm::Ptr< pat::Muon > MuonPtr;
  typedef edm::Ptr< pat::Electron > ElectronPtr;
  typedef edm::Ptr< pat::MET > METPtr;

}

#endif
