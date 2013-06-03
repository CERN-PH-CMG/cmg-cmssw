#ifndef _btvanalysis_
#define _btvanalysis_

#include "UserCode/EWKV/interface/SmartSelectionMonitor.h"
#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"

#include <vector>

#include "TString.h"

class BTVAnalysis
{

public:
  BTVAnalysis(SmartSelectionMonitor &mon,bool runSystematics);

  void analyze(data::PhysicsObjectCollection_t &leptons, 
	       data::PhysicsObjectCollection_t &jets,
	       bool isMC, int nVtx, float weight, float weightUp, float weightDown);

private:

  SmartSelectionMonitor *mon_;
  std::vector<TString> systVars_, jetRanges_;

};

#endif
