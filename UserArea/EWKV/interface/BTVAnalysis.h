#ifndef _btvanalysis_
#define _btvanalysis_

#include "UserCode/EWKV/interface/SmartSelectionMonitor.h"
#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"

#include <vector>

#include "TString.h"

struct BTVTagger_t
{
  BTVTagger_t() {}
  BTVTagger_t(const BTVTagger_t &t) {
    name=t.name;
    minVal=t.minVal;
    maxVal=t.maxVal;
    wps=t.wps;
  }

  TString name;
  float minVal,maxVal;
  std::map<TString,float> wps;
};

class BTVAnalysis
{

public:
  BTVAnalysis(SmartSelectionMonitor &mon,bool runSystematics);

  void analyze(data::PhysicsObjectCollection_t &leptons, 
	       data::PhysicsObjectCollection_t &jets,
	       bool isMC, int nVtx, float weight, float weightUp, float weightDown);

private:

  void fillCategories(bool runSystematics);

  SmartSelectionMonitor *mon_;
  std::vector<TString> systVars_, jetRanges_;
  std::vector<BTVTagger_t> taggers_;
};

#endif
