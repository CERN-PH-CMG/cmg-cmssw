#include <iostream>
#include <string>
#include <sstream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom1.h"
#include "TGraphErrors.h"
#include "TLine.h"
//#include "RooRealVar.h"
//#include "RooFormulaVar.h"
//#include "RooDataHist.h"
//#include "RooDataSet.h"
//#include "RooHistPdf.h"
//#include "RooExtendPdf.h"
//#include "RooAddPdf.h"
//#include "RooGaussian.h"
//#include "RooFFTConvPdf.h"
//#include "RooPlot.h"
#include "axisTools.hh"
#include "plotTools.hh"
//#include "../../NYStyle/test/NYStyle.h"
#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"

void roc(TH1F *iHS,TH1F *iHB,bool iWeight=false,bool iCan=true,int iColor=kBlack) { 
  const int lN = iHS->GetNbinsX()-1;
  double *lX = new double[lN];
  double *lY = new double[lN];
  for(int i0 = 1; i0 < iHS->GetNbinsX();i0++) {
    double lSVal = iHS->Integral(i0,10000)/iHS->Integral(-1,10000);
    double lBVal = iHB->Integral(i0,10000)/iHB->Integral(-1,10000);
    lY[i0-1] = lSVal;
    lX[i0-1] = lBVal;
  }
  TGraph *lG = new TGraph(lN,lX,lY); lG->SetLineColor(iColor); lG->SetLineWidth(3);
  lG->GetYaxis()->SetTitle("Signal Efficiency" );
  lG->GetXaxis()->SetTitle("Background Efficiency" );

  if(iWeight) lG->GetYaxis()->SetTitle("Signal Efficiency p_{T} equalized" );
  if(iWeight) lG->GetXaxis()->SetTitle("Background Efficiency p_{T} equalized" );

  //if(iCan) TCanvas *lC0  = new TCanvas("cX","cX",800,600);
  if(iCan)  lG->Draw("alc" );
  if(!iCan) lG->Draw("lc");
}
void plotJets_roc(std::string iId,bool iWeight=false,std::string iBaseCut="(jetPt > 20 && abs(jetEta) > 2.5 && abs(jetEta) < 2.9)",std::string iVar="jmva") { 
  Prep();
  loadfMap();
  std::stringstream lNameId=""; //lNameId << "Flat_" << lTauId << "_";
  const int lN = 4;
  std::string lName = "../../scratch/"+lNameId.str();
  TTree **lTree = new TTree*[lN]; 
  TH1F**lH = new TH1F*[lN]; 
  fString = new std::string[lN]; fWeights = new std::string[lN]; fColor = new int[lN];
  lTree[0]  = load(lName+"r11-dimu_"+iId+".root");          fString[0] = "Data";          fColor[0] = kBlack;
  lTree[1]  = load(lName+"f11-zjets_"+iId+".root");         fString[1] = "Total";         fColor[1] = kRed;
  lTree[2]  = load(lName+"f11-zjets_"+iId+".root");         fString[2] = "No Pileup";     fColor[2] = kBlue;
  lTree[3]  = load(lName+"f11-zjets_"+iId+".root");         fString[3] = "Pileup ";       fColor[3] = kGreen;
  
  for(int i0 = 0; i0 < lN; i0++) fWeights[i0] = iBaseCut;// && abs(jseta_1) > 2.5)";
  fWeights[0] += "";
  fWeights[1] += "*(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.)";
  fWeights[2] += "*(!(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.))";
  if(iWeight) fWeights[1] += "*1./PtWeight";
  if(iWeight) fWeights[2] += "*1./PtWeight";
  if(iWeight) fWeights[3] += "*1./PtWeight";
  std::string lVar =  iVar;
  double l1Scale = 1;
  for(int i0 = 0; i0 < lN; i0++)  {
    lH[i0] = draw(lVar,lTree[i0],i0,"*(jetPt > 0)");
    lH[i0]->Sumw2();
    if(i0 ==  1) l1Scale = lH[0]->Integral()/lH[1]->Integral();
    if(i0 >  0) lH[i0]->Scale(l1Scale);
  }
  lH[0]->GetYaxis()->SetRangeUser(0.00001,lH[0]->GetMaximum()*1.5); lH[0]->SetLineWidth(2);
  lH[0]->Draw("ep");
  lH[1]->SetLineStyle(kDashed);
  for(int i0 = 1; i0 < lN; i0++) { 
    lH[i0]->SetMarkerStyle(kFullCircle);  lH[i0]->SetLineWidth(2);
    lH[i0]->SetMarkerColor(fColor[i0]);
    lH[i0]->Draw("hist sames");
  }
  legend(lN,lH)->Draw();
  //return;
  TH1F**lB = new TH1F*[lN]; 
  for(int i0 = 0; i0 < lN; i0++) fWeights[i0] = iBaseCut+"*(jetPt > 20 && jetPt < 30)";// && abs(jseta_1) > 2.5)";
  fWeights[0] += "";
  fWeights[1] += "*(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.)";
  fWeights[2] += "*(!(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.))";
  if(iWeight) fWeights[1] += "*1./PtWeight";
  if(iWeight) fWeights[2] += "*1./PtWeight";
  if(iWeight) fWeights[3] += "*1./PtWeight";
  for(int i0 = 0; i0 < lN; i0++)  {
    lB[i0] = 0;
    lB[i0] = draw(lVar,lTree[i0],i0,"*(jetPt > 0)",lB[i0],"A");
    lB[i0]->Sumw2();
    if(i0 >  0) l1Scale = lB[0]->Integral()/lB[1]->Integral();
    if(i0 >  0) lB[i0]->Scale(l1Scale);
  }
  TH1F**lC = new TH1F*[lN]; 
  for(int i0 = 0; i0 < lN; i0++) fWeights[i0] = iBaseCut+"*(jetPt > 30 && jetPt < 50)";// && abs(jseta_1) > 2.5)";
  fWeights[0] += "";
  fWeights[1] += "*(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.)";
  fWeights[2] += "*(!(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.))";
  if(iWeight) fWeights[1] += "*1./PtWeight";
  if(iWeight) fWeights[2] += "*1./PtWeight";
  if(iWeight) fWeights[3] += "*1./PtWeight";
  for(int i0 = 0; i0 < lN; i0++)  {
    lC[i0] = 0;
    lC[i0] = draw(lVar,lTree[i0],i0,"*(jetPt > 0)",lC[i0],"C");
    lC[i0]->Sumw2();
    if(i0 >  0) l1Scale = lC[0]->Integral()/lC[1]->Integral();
    if(i0 >  0) lC[i0]->Scale(l1Scale);
  }
  TCanvas *lC0 = new TCanvas((lVar+"Can").c_str(),(lVar+"Can").c_str(),800,600);
  roc(lH[2],lH[3],iWeight,true);
  roc(lB[2],lB[3],iWeight,false,kRed);
  roc(lC[2],lC[3],iWeight,false,kBlue);
  TLegend *lL = new TLegend(0.5,0.2,0.9,0.6); lL->SetBorderSize(0); lL->SetFillColor(0);
  TH1F *pH0 = new TH1F("X","X",1,0,1); pH0->SetLineWidth(3);                           lL->AddEntry(pH0,"p_{T} > 10 GeV/c","l");
  TH1F *pH1 = new TH1F("Y","Y",1,0,1); pH1->SetLineWidth(3); pH1->SetLineColor(kRed);  lL->AddEntry(pH1,"30 > p_{T} > 20 GeV/c","l");
  TH1F *pH2 = new TH1F("Z","Z",1,0,1); pH2->SetLineWidth(3); pH2->SetLineColor(kBlue); lL->AddEntry(pH2,"50 > p_{T} > 30 GeV/c","l");
  lL->Draw();
  if(!iWeight) lC0->SaveAs(("Roc"+iId+".png").c_str());
  if( iWeight) lC0->SaveAs(("Roc"+iId+"PtWeight.png").c_str());
  return;
}
