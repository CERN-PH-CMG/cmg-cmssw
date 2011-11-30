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


TLegend *showPlots(TPad *c, TList &stack, TList &spimpose, TList &data, bool buildLegend=true, TString legopt="lpf",bool forceNormalization=false, bool doIntegral=false);
TLegend *showStackPlot(TPad *c, TList &stack, bool buildLegend=true, TString legopt="lpf");
TLegend *showSimplePlot(TPad *c, TList &data, bool buildLegend=true, TString legopt="lpf");
void showMCtoDataComparison(TPad *c, TList &stack, TList &data, bool doDiff=false, float yscale=1,bool forceNormalization=false,bool doIntegral=false);
TString getPlotAsTable(TList *stack, TList *spimpose, TList *data);
std::pair<TH1D *,TH1D *> getProjections(TH2D *);
TLegend *showPlotsAndMCtoDataComparison(TPad *p, TList &stack, TList &spimpose, TList &data, bool setLogy=true,bool forceNormalization=false, bool doIntegral=false);
TH1 *getIntegralFrom(TH1 *h,TString newname);

#endif
