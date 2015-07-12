#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TStyle.h>
#include "MitStyleRemix.hh"

void MitStyleRemix() {
  const char* author   = "$Author: perrozzi $$";
  const char* modified = "$Modified by ksung $$";
  printf(" MIT root style REMIX(%s,%s).\n",author,modified);
  printf("\n");
  printf(" Use: MakeCanvas(name,title)\n");
  printf("      InitSubPad(pad,nPad)\n");
  printf("      InitHist(hist,xTitle,yTitle,color)\n");
  printf("\n");
  SetStyle();
}

TCanvas* MakeCanvas(const char* name, const char *title, int dX, int dY)
{
  // Start with a canvas
  TCanvas *canvas = new TCanvas(name,title,0,0,dX,dY);
  // General overall stuff
  canvas->SetFillColor      (0);
  canvas->SetBorderMode     (0);
  canvas->SetBorderSize     (10);
  // Set margins to reasonable defaults
  canvas->SetLeftMargin     (0.18);
  canvas->SetRightMargin    (0.05);
  canvas->SetTopMargin      (0.08);
  canvas->SetBottomMargin   (0.15);
  // Setup a frame which makes sense
  canvas->SetFrameFillStyle (0);
  canvas->SetFrameLineStyle (0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);
  canvas->SetFrameFillStyle (0);
  canvas->SetFrameLineStyle (0);
  canvas->SetFrameBorderMode(0);
  canvas->SetFrameBorderSize(10);

  return canvas;
}

void InitSubPad(TPad* pad, int i)
{
  //printf("Pad: %p, index: %d\n",pad,i);

  pad->cd(i);
  TPad *tmpPad = (TPad*) pad->GetPad(i);
  tmpPad->SetLeftMargin  (0.18);
  tmpPad->SetTopMargin   (0.05);
  tmpPad->SetRightMargin (0.07);
  tmpPad->SetBottomMargin(0.15);
  return;
}

void InitHist(TH1 *hist, const char *xtit, const char *ytit, EColor color)
{
  hist->SetXTitle(xtit);
  hist->SetYTitle(ytit);
  hist->SetLineColor(color);
  hist->SetTitleSize  (0.055,"Y");
  hist->SetTitleOffset(1.600,"Y");
  hist->SetLabelOffset(0.014,"Y");
  hist->SetLabelSize  (0.050,"Y");
  hist->SetLabelFont  (42   ,"Y");
  hist->SetTitleSize  (0.055,"X");
  hist->SetTitleOffset(1.300,"X");
  hist->SetLabelOffset(0.014,"X");
  hist->SetLabelSize  (0.050,"X");
  hist->SetLabelFont  (42   ,"X");
  hist->SetMarkerStyle(20);
  hist->SetMarkerColor(color);
  hist->SetMarkerSize (0.6);
  // Strangely enough this cannot be set anywhere else??
  hist->GetYaxis()->SetTitleFont(42);
  hist->GetXaxis()->SetTitleFont(42);
  hist->SetTitle("");  
  return;
}

void SetStyle()
{
  TStyle *MITStyle = new TStyle("MIT-Style","The Perfect Style for Plots ;-)");
  gStyle = MITStyle;

  // Canvas
  MITStyle->SetCanvasColor     (0);
  MITStyle->SetCanvasBorderSize(10);
  MITStyle->SetCanvasBorderMode(0);
  MITStyle->SetCanvasDefH      (700);
  MITStyle->SetCanvasDefW      (700);
  MITStyle->SetCanvasDefX      (100);
  MITStyle->SetCanvasDefY      (100);

  // Pads
  MITStyle->SetPadColor       (0);
  MITStyle->SetPadBorderSize  (10);
  MITStyle->SetPadBorderMode  (0);
  MITStyle->SetPadBottomMargin(0.13);
  MITStyle->SetPadTopMargin   (0.08);
  MITStyle->SetPadLeftMargin  (0.15);
  MITStyle->SetPadRightMargin (0.05);
  MITStyle->SetPadGridX       (0);
  MITStyle->SetPadGridY       (0);
  MITStyle->SetPadTickX       (0);
  MITStyle->SetPadTickY       (0);

  // Frames
  MITStyle->SetFrameFillStyle ( 0);
  MITStyle->SetFrameFillColor ( 0);
  MITStyle->SetFrameLineColor ( 1);
  MITStyle->SetFrameLineStyle ( 0);
  MITStyle->SetFrameLineWidth ( 1);
  MITStyle->SetFrameBorderSize(10);
  MITStyle->SetFrameBorderMode( 0);

  // Histograms
  MITStyle->SetHistFillColor(2);
  MITStyle->SetHistFillStyle(0);
  MITStyle->SetHistLineColor(1);
  MITStyle->SetHistLineStyle(0);
  MITStyle->SetHistLineWidth(2);
  MITStyle->SetNdivisions(505);

  // Functions
  MITStyle->SetFuncColor(1);
  MITStyle->SetFuncStyle(0);
  MITStyle->SetFuncWidth(2);

  // Various
  MITStyle->SetMarkerStyle(20);
  MITStyle->SetMarkerColor(kBlack);
  MITStyle->SetMarkerSize (1.2);

  MITStyle->SetTitleBorderSize(0);
  MITStyle->SetTitleFillColor (0);
  MITStyle->SetTitleX         (0.2);

  MITStyle->SetTitleSize  (0.055,"X");
  MITStyle->SetTitleOffset(1.200,"X");
  MITStyle->SetLabelOffset(0.005,"X");
  MITStyle->SetLabelSize  (0.050,"X");
  MITStyle->SetLabelFont  (42   ,"X");

  MITStyle->SetStripDecimals(kFALSE);

  MITStyle->SetTitleSize  (0.055,"Y");
  MITStyle->SetTitleOffset(1.600,"Y");
  MITStyle->SetLabelOffset(0.010,"Y");
  MITStyle->SetLabelSize  (0.050,"Y");
  MITStyle->SetLabelFont  (42   ,"Y");

  MITStyle->SetTextSize   (0.055);
  MITStyle->SetTextFont   (42);

  MITStyle->SetStatFont   (42);
  MITStyle->SetTitleFont  (42);
  MITStyle->SetTitleFont  (42,"X");
  MITStyle->SetTitleFont  (42,"Y");

  MITStyle->SetOptStat    (0);
  return;
}
