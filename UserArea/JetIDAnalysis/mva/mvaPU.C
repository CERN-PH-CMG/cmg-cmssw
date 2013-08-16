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
#include "RooRealVar.h"
//#include "RooFormulaVar.h"
#include "RooDataHist.h"
//#include "RooDataSet.h"
#include "RooHistPdf.h"
//#include "RooExtendPdf.h"
#include "RooAddPdf.h"
//#include "RooGaussian.h"
//#include "RooFFTConvPdf.h"
//#include "RooPlot.h"
#include "axisTools.hh"
#include "plotTools.hh"
//#include "../../NYStyle/test/NYStyle.h"
#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"
void mvaPU(std::string iId="chs_rw_1cat",std::string iCut="(jspt_1 > 10)") { 
  Prep();
  loadfMap();
  int lTauId = 2;
  std::stringstream lNameId=""; //lNameId << "Flat_" << lTauId << "_";
  const int lN = 3;
  std::string lName = "../../scratch/"+lNameId.str();
  TTree **lTree = new TTree*[lN]; TH1F**lH = new TH1F*[lN]; 
  fString = new std::string[lN]; fWeights = new std::string[lN]; fColor = new int[lN];
  lTree[0]  = load(lName+"r11-dimu_" +iId+".root");         fString[0] = "Data";       fColor[0] = kRed;
  lTree[1]  = load(lName+"f11-zjets_"+iId+".root");         fString[1] = "No Pileup";  fColor[1] = kBlue;
  lTree[2]  = load(lName+"f11-zjets_"+iId+".root");         fString[2] = "Pileup";     fColor[2] = kGreen;

  for(int i0 = 0; i0 < lN; i0++) fWeights[i0]  = iCut;
  fWeights[0] += "";
  fWeights[1] += "*(jetPt  > 10  &&   abs(jetGenDr) < 0.25  && jetGenPt > 8.)";
  fWeights[2] += "*(!(jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.))";

  std::string lVar =  "jmva";
  TCanvas *lC0 = new TCanvas("A","A",600,600);
  cout << "=====> Cat "<< iCut << endl;
  for(int i0 = 0; i0 < lN; i0++)  {
    //if(i0 > 0) lVar = "JetID";
    if(i0 > 0) lVar = "jmva";
    lH[i0] = draw(lVar,lTree[i0],i0,"");
  }
  cout << "====> Data Total : " << lH[1]->Integral() << endl;
  //Fit Functions
  RooRealVar lRFVar("mva","mva"  ,0,-1,1);
  RooRealVar lFrac ("frac","frac",0.80,0 ,1);
  RooDataHist *lData  = new RooDataHist("data" ,"data"  ,lRFVar,lH[0]);
  RooDataHist *lHPU   = new RooDataHist("hpu"  ,"hpu"   ,lRFVar,lH[1]);
  RooDataHist *lHJets = new RooDataHist("hjets","hjets" ,lRFVar,lH[2]);

  RooHistPdf  *lJets = new RooHistPdf ("jets","jets",lRFVar,*lHPU);
  RooHistPdf  *lPU   = new RooHistPdf ("pu"  ,"pu"  ,lRFVar,*lHJets);
  RooAddPdf   *lAdd  = new RooAddPdf  ("add" ,"add" ,RooFit::RooArgList(*lJets,*lPU),RooFit::RooArgSet(lFrac)); 
  lAdd->fitTo(*lData);
  lH[1]->Scale(lH[0]->Integral()*lFrac.getVal()/lH[1]->Integral());
  lH[2]->Scale(lH[0]->Integral()*(1-lFrac.getVal())/lH[2]->Integral());
  TH1F *lTotal = lH[1]->Clone("total");
  lTotal->Add(lH[2]);
  lH[0]->SetLineColor(kBlack);
  lH[0]->GetYaxis()->SetRangeUser(0.00001,lH[0]->GetMaximum()*1.5); lH[0]->SetLineWidth(1); 
  lH[0]->SetMarkerStyle(kFullCircle);
  //lH[0]->GetYaxis()->SetRangeUser(0.00001,15000);
  lH[0]->Draw("ep");
  for(int i0 = 1; i0 < lN; i0++) { 
    lH[i0]->SetMarkerStyle(kFullCircle);  lH[i0]->SetLineWidth(3);
    lH[i0]->SetMarkerColor(fColor[i0]);
    lH[i0]->Draw("hist sames");
    cout << " ====> " << lH[i0]  << endl;
    //cout << "===> " << i0 << " -- "<< lH[i0]->Integral() << endl;
  }
  lTotal->SetLineStyle(kDashed); lTotal->SetLineWidth(2); lTotal->SetLineColor(kRed);
  lTotal->Draw("hist sames");
  legend(lN,lH)->Draw();
  lC0->SaveAs((std::string("MVA")+iId+std::string(".png")).c_str());
  
  TH1F**lEta = new TH1F*[lN];   
  std::string lVar =  "jetEta";
  TCanvas *lC0 = new TCanvas("B","B",600,600);
  for(int i0 = 0; i0 < lN; i0++)  {
    lEta[i0] = draw(lVar,lTree[i0],i0,"");
  }
  lEta[1]->Scale(lEta[0]->Integral()*lFrac.getVal()    /lEta[1]->Integral());
  lEta[2]->Scale(lEta[0]->Integral()*(1-lFrac.getVal())/lEta[2]->Integral());
  TH1F *lTotalEta = lEta[1]->Clone("totalEta");
  lTotalEta->Add(lEta[2]);
  lEta[0]->SetLineColor(kBlack);
  lEta[0]->GetYaxis()->SetRangeUser(0.00001,lEta[0]->GetMaximum()*1.5); lEta[0]->SetLineWidth(1); lEta[0]->SetMarkerStyle(kFullCircle);
  //lEta[0]->GetYaxis()->SetRangeUser(0.1,1200);
  lEta[0]->Draw("ep ");
  for(int i0 = 1; i0 < lN; i0++) { 
    lEta[i0]->SetMarkerStyle(kFullCircle);  lH[i0]->SetLineWidth(3);
    lEta[i0]->SetMarkerColor(fColor[i0]);
    if(i0  > 0) lEta[i0]->Draw("hist sames");
  }
  lTotalEta->SetLineStyle(kDashed); lTotalEta->SetLineWidth(2); lTotalEta->SetLineColor(kRed);
  lTotalEta->Draw("hist sames"); 
  lC0->SaveAs((std::string("Eta")+iId+std::string(".png")).c_str());

  TH1F**lPt = new TH1F*[lN];   
  std::string lVar =  "jetPt" ;//(jspt_1*(abs(jseta_1) > 2.5)+jsptraw_1*(abs(jseta_1) < 2.5))";
  TCanvas *lC0 = new TCanvas("C","C",600,600);
  for(int i0 = 0; i0 < lN; i0++)  {
    //if(i0 > 0) lVar =  "(jspt_1*(abs(jseta_1) > 2.5)+jsptraw_1*(abs(jseta_1) < 2.5))";
    lPt[i0] = draw(lVar,lTree[i0],i0,"");
  }
  lPt[1]->Scale(lPt[0]->Integral()*lFrac.getVal()    /lPt[1]->Integral());
  lPt[2]->Scale(lPt[0]->Integral()*(1-lFrac.getVal())/lPt[2]->Integral());
  TH1F *lTotalPt = lPt[1]->Clone("totalPt");
  lTotalPt->Add(lPt[2]);
  lPt[0]->SetLineColor(kBlack);
  lPt[0]->GetYaxis()->SetRangeUser(0.00001,lPt[0]->GetMaximum()*1.5); lPt[0]->SetLineWidth(1); lPt[0]->SetMarkerStyle(kFullCircle);
  //lPt[0]->GetYaxis()->SetRangeUser(0.00001,126000);
  //lPt[0]->GetYaxis()->SetRangeUser(0.1,4000);
  lPt[0]->Draw("ep ");
  for(int i0 = 1; i0 < lN; i0++) { 
    lPt[i0]->SetMarkerStyle(kFullCircle);  lH[i0]->SetLineWidth(3);
    lPt[i0]->SetMarkerColor(fColor[i0]);
    if(i0  > 0) lPt[i0]->Draw("hist sames");
  }
  lTotalPt->SetLineStyle(kDashed); lTotalPt->SetLineWidth(2); lTotalPt->SetLineColor(kRed);
  lTotalPt->Draw("hist sames"); 
  lC0->SaveAs((std::string("Pt")+iId+std::string(".png")).c_str());
  return;
}
