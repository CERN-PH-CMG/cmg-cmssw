#ifndef pileupfilter_h
#define pileupfilter_h

#include "TH1D.h"
#include "TH2D.h"
#include "TRandom.h"

namespace pileup
{
  enum PileupWeightType {MCTODATA,MCTODATAINTEGER};

  TH1D * generateWeightsFrom(std::vector<double> mc, std::vector<double> data, float &maxweight);
  std::vector<double> getWeightsFor(int npuevents, TH1D *wgtDist, double maxWeight);
}


#endif
