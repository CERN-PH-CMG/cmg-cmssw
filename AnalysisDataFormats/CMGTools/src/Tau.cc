# include "AnalysisDataFormats/CMGTools/interface/Tau.h"

using namespace cmg;

float Tau::signalChargedFraction() const {
  
  float sumE = 0;
  float sumECharged = 0;
  const Constituents& signals = signalPFCands();
  for( unsigned i=0; i<signals.size(); ++i) {
    float energy = signals[i].energy();
    sumE += energy;
    if( fabs(signals[i].charge())>1e-9 )
      sumECharged += energy;
  }

  assert(sumE>0);

  return sumECharged / sumE;
}

float Tau::signalChargedFractionpT() const {
  
  float sumE = 0;
  float sumECharged = 0;
  const Constituents& signals = signalPFCands();
  for( unsigned i=0; i<signals.size(); ++i) {
    float energy = signals[i].pt();
    sumE += energy;
    if( fabs(signals[i].charge())>1e-9 )
      sumECharged += energy;
  }

  assert(sumE>0);

  return sumECharged / sumE;
}
