#ifndef getruninfo_h
#define getruninfo_h

#if !defined(__CINT__) || defined(__MAKECINT__)

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Run.h"
#include "DataFormats/Histograms/interface/MEtoEDMFormat.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include <vector>
#include <TFile.h>

#endif

enum InfoSummary { INITIAL_EVTS, FINAL_EVTS, XSECTION, GENERATOR_EFF };
std::vector<float> getRunInfo(TFile *f);

#endif
