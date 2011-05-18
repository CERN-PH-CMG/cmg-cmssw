#ifndef plotter_h
#define plotter_h

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <vector>

#include "TPad.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TString.h"
#include "TList.h"

#endif


TLegend *showPlots(TPad *c, TList &stack, TList &spimpose, TList &data, bool buildLegend=true, TString legopt="lpf");
TLegend *showStackPlot(TPad *c, TList &stack, bool buildLegend=true, TString legopt="lpf");
TLegend *showSimplePlot(TPad *c, TList &data, bool buildLegend=true, TString legopt="lpf");
void showMCtoDataComparison(TPad *c, TList &stack, TList &data, bool doDiff=false, float yscale=1);
TString getPlotAsTable(TList *stack, TList *spimpose, TList *data);

#endif
