#ifndef setstyle_h
#define setstyle_h

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <vector>

#include "TMath.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TString.h"
#include "TObjString.h"
#include "TList.h"
#include "TPaveText.h"
#include "TLine.h"
#include "TLatex.h"
#include "TClass.h"

#endif

TStyle *setStyle(bool gridOn=false);
TCanvas *getNewCanvas(TString name, TString title,bool setBatch);
TObject *formatPlot(TObject *h,
		    int color, int line, int linew, int marker, int fill,
		    bool detachFromCurrentDir, bool setSumw2,
		    int linecolor, int fillcolor, int markercolor);
void fixExtremities(TH1 *h,bool addOverflow, bool addUnderflow);
void formatForCmsPublic(TPad * c, TLegend *leg, TString title, int nsamp, float legx=0.65, float legy=0.95, float legw=0.2, float legh=0.05, TString legopt="lpf");

#endif
