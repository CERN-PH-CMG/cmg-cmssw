#ifndef _ueanalysis_
#define _ueanalysis_

#include "UserCode/EWKV/interface/SmartSelectionMonitor.h"
#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"

#include <vector>

#include "TString.h"

class UEAnalysis
{

public:
  UEAnalysis(SmartSelectionMonitor &mon);

  void analyze(data::PhysicsObjectCollection_t &leptons, 
	       data::PhysicsObjectCollection_t &jets,
	       data::PhysicsObjectCollection_t &met, 
	       data::PhysicsObjectCollection_t &pf,
	       data::PhysicsObjectCollection_t &mctruth,
	       float weight);

private:

  SmartSelectionMonitor *mon_;
  std::vector<TString> ueReg_;

};

#endif
