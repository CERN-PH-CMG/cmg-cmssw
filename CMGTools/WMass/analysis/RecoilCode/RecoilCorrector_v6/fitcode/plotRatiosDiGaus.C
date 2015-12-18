#include <sstream>
#include "TLine.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TF1.h"
#include "TLatex.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
//#include "../../NYStyle/test/NYStyle.h"
//#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_9_4/src/MitWlnu/NYStyle/test/NYStyle.h"

using namespace std;

int vtxBin=-1;

const int readRecoil(std::vector<double> &iSumEt,
		     std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1Sig3Fit,
		     std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2Sig3Fit,
		     std::string iFName = "recoilfit.root",std::string iPrefix="") { 
  TFile *lFile  = new TFile(iFName.c_str());
  //  lFile->ls();

  //TGraph *lGraph = (TGraph *) lFile->FindObjectAny("sumet");
  const int lNBins = 20;//lGraph->GetN();

  for(int i0 = 1; i0 <= lNBins; i0++) {
    if(i0!=vtxBin) continue;
    cout << i0 << endl;

    std::string lStr = iPrefix;
    //iSumEt.push_back(lGraph->GetY()[i0]);
    std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10;
    pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);
    
    pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);
    pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
   
  }

  lFile->Close();
  iSumEt.push_back(1000);  
  return lNBins;
}
double getCorError2(double iVal,TF1 *iFit) { 
  double lE = sqrt(iFit->GetParError(0))  + iVal*sqrt(iFit->GetParError(2));
  if(fabs(iFit->GetParError(4)) > 0) lE += iVal*iVal*sqrt(iFit->GetParError(4));
  return lE*lE;
}
double getError2(double iVal,TF1 *iFit) { 
  double lE2 = iFit->GetParError(0) + iVal*iFit->GetParError(1) + iVal*iVal*iFit->GetParError(2);
  if(fabs(iFit->GetParError(3)) > 0) lE2 += iVal*iVal*iVal*     iFit->GetParError(3);
  if(fabs(iFit->GetParError(4)) > 0) lE2 += iVal*iVal*iVal*iVal*iFit->GetParError(4);
  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) == 0) lE2 += iVal*iVal*               iFit->GetParError(5);
  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) != 0) lE2 += iVal*iVal*iVal*iVal*iVal*iFit->GetParError(5);
  if(fabs(iFit->GetParError(6)) > 0) lE2 += iVal*iVal*iVal*iVal*iVal*iVal*iFit->GetParError(6);
  return lE2;
}
double getError(double iVal,TF1 *iZDatFit,TF1 *iZMCFit) {
  double lEZD2 = getError2(iVal,iZDatFit);
  double lEZM2 = getError2(iVal,iZMCFit);
  double lZDat = iZDatFit->Eval(iVal);
  double lZMC  = iZMCFit->Eval(iVal);
  double lR    = lZDat/lZMC;
  double lER   = lR*lR/lZDat/lZDat*lEZD2 + lR*lR/lZMC/lZMC*lEZM2;
  return sqrt(lER);
}
double getError(double iVal,TF1 *iWFit,TF1 *iZDatFit,TF1 *iZMCFit,bool iRescale=true) {
  double lEW2  = getError2(iVal,iWFit);
  if(!iRescale) return sqrt(lEW2);
  double lEZD2 = getError2(iVal,iZDatFit);
  double lEZM2 = getError2(iVal,iZMCFit);
  double lZDat = iZDatFit->Eval(iVal);
  double lZMC  = iZMCFit->Eval(iVal);
  double lWMC  = iWFit->Eval(iVal);
  double lR    = lZDat/lZMC;
  double lER   = lR*lR/lZDat/lZDat*lEZD2 + lR*lR/lZMC/lZMC*lEZM2;
  double lVal  = lR*lR*lEW2 + lWMC*lWMC*lER;
  return sqrt(lVal);
}
void draw(string iName,string iYName,double iYMin,double iYMax,double iTMax,
	  vector<TF1*> &iWFit,vector<TF1*> &iZDFit,vector<TF1*> &iZMFit,vector<TF1*> &iWCFit
	  ) { 
  bool lU1 = false; if(iName =="u1") lU1 = true;
  int lNTot = 300;
  double lXVals0[lNTot];
  double lGVals0[lNTot]; 
  double lGVals1[lNTot];
  double lGVals2[lNTot]; 
  double lGVals3[lNTot]; 
  for(int i0 = 0; i0 < lNTot/2; i0++) { 
    lXVals0[i0] =  i0;
    double lE0 = TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax);
    double lE1 = TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax);
    double lE2 = TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax);
    double lE3 = TMath::Max(sqrt(getError2(i0,iWCFit[0])),iTMax);
    lGVals0[i0] =  iWFit[0]->Eval(i0)   + lE0;
    lGVals1[i0] =  iZDFit[0]->Eval(i0)  + lE1;
    lGVals2[i0] =  iZMFit[0]->Eval(i0)  + lE2;
    lGVals3[i0] =  iWCFit[0]->Eval(i0)  + lE3;
    if(lU1) lXVals0[i0] != 0 ? lGVals0[i0]/=-lXVals0[i0] : lGVals0[i0] = 0;
    if(lU1) lXVals0[i0] != 0 ? lGVals1[i0]/=-lXVals0[i0] : lGVals1[i0] = 0;
    if(lU1) lXVals0[i0] != 0 ? lGVals2[i0]/=-lXVals0[i0] : lGVals2[i0] = 0;
    if(lU1) lXVals0[i0] != 0 ? lGVals3[i0]/=-lXVals0[i0] : lGVals3[i0] = 0;
  }
  for(int i0 = 0; i0 < lNTot/2; i0++) { 
    lXVals0[i0+lNTot/2] =  lNTot/2-1-i0;
    double lE0 = TMath::Max(sqrt(getError2(lNTot/2-1-i0,iWFit[0])),iTMax) ;
    double lE1 = TMath::Max(sqrt(getError2(lNTot/2-1-i0,iZDFit[0])),iTMax);
    double lE2 = TMath::Max(sqrt(getError2(lNTot/2-1-i0,iZMFit[0])),iTMax);
    double lE3 = TMath::Max(sqrt(getError2(lNTot/2-1-i0,iWCFit[0])),iTMax);
    lGVals0[i0+lNTot/2] =  iWFit[0] ->Eval(lNTot/2-1-i0)  - lE0;
    lGVals1[i0+lNTot/2] =  iZDFit[0]->Eval(lNTot/2-1-i0)  - lE1;
    lGVals2[i0+lNTot/2] =  iZMFit[0]->Eval(lNTot/2-1-i0)  - lE2;
    lGVals3[i0+lNTot/2] =  iWCFit[0]->Eval(lNTot/2-1-i0)  - lE2;
    if(lU1) lXVals0[i0+lNTot/2] != 0 ? lGVals0[i0+lNTot/2]/=-lXVals0[i0+lNTot/2] : lGVals0[i0+lNTot/2] = 0;
    if(lU1) lXVals0[i0+lNTot/2] != 0 ? lGVals1[i0+lNTot/2]/=-lXVals0[i0+lNTot/2] : lGVals1[i0+lNTot/2] = 0;
    if(lU1) lXVals0[i0+lNTot/2] != 0 ? lGVals2[i0+lNTot/2]/=-lXVals0[i0+lNTot/2] : lGVals2[i0+lNTot/2] = 0;
    if(lU1) lXVals0[i0+lNTot/2] != 0 ? lGVals3[i0+lNTot/2]/=-lXVals0[i0+lNTot/2] : lGVals3[i0+lNTot/2] = 0;
  }
  TGraph *lG0 = new TGraph(lNTot,lXVals0,lGVals0); lG0->SetFillColor(kBlue);   lG0->SetLineColor(kBlue); lG0->SetLineWidth(5);
  TGraph *lG1 = new TGraph(lNTot,lXVals0,lGVals1); lG1->SetFillColor(kGreen);  lG1->SetLineColor(kGreen);
  TGraph *lG2 = new TGraph(lNTot,lXVals0,lGVals2); lG2->SetFillColor(kRed);    lG2->SetLineColor(kRed);
  TGraph *lG3 = new TGraph(lNTot,lXVals0,lGVals3); lG3->SetFillColor(kViolet); lG3->SetLineColor(kViolet);
  TLegend *lL = new TLegend(0.25,0.15,0.55,0.35); lL->SetFillColor(0); lL->SetBorderSize(0);
  //  lL->AddEntry(lG0,"W MC","lf");
  //lL->AddEntry(lG1,"Z Data","lf");
  //lL->AddEntry(lG2,"Z MC ","lf");

  lG1->GetXaxis()->SetTitle("Z_{p_{T}} (GeV)");
  //  lG0->GetXaxis()->SetRangeUser(0,lNTot/2-1);
  lG1->GetXaxis()->SetRangeUser(0,50); // focus for now at 50 GeV
  lG1->GetYaxis()->SetTitle(iYName.c_str());//"U_{1} Response (GeV)");
  lG1->GetYaxis()->SetRangeUser(iYMin,iYMax);

  TCanvas *lC0 = new TCanvas(iName.c_str(),iName.c_str(),800,600); lC0->cd();
  //lL->AddEntry(lG2,"MVA MC (correcting PF Type 1)"   ,"lf");
  //lL->AddEntry(lG3,"MVA Data (correcting PF Type 1)" ,"lf");
  lL->AddEntry(lG1,"Data Z","lf");
  lL->AddEntry(lG2,"MC Z"  ,"lf");

  lL->AddEntry(lG0,"MC W pos","lf");
  lL->AddEntry(lG3,"MC W meg","lf");

  lG1->Draw("alf");
  lG2->Draw("lf");
  lG3->Draw("lf");
  lG0->Draw("lf");
  lL->Draw();
  TLine   *lLine = new TLine(1.,1.,140.,1.); lLine->SetLineStyle(kDashed);
  lLine->Draw();

  TString leg1 = "NVtx = ";
  leg1 += vtxBin;

  TString text1 = "NVtx";
  text1 += vtxBin;
  text1 += "_";
  text1 += iName;
  text1 += "debug.png";

  TLatex latexLabel;
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  latexLabel.DrawLatex(0.7, 0.2, leg1);                                                                                                                                 

  lC0->SaveAs(text1.Data());
  //  lC0->SaveAs((iName+".eps").c_str());

}
double Frac(double iS1,double iS2) { 
  double lS1 = iS1; double lS2 = iS2;
  if(iS1 > iS2) {lS1 = iS2; lS2=iS1;}
  return (lS2-1.)/(lS2-lS1);
}
void drawFrac(string iName,string iYName,double iYMin,double iYMax,double iTMax,
	      vector<TF1*> &iWR1Fit,vector<TF1*> &iZDR1Fit,vector<TF1*> &iZMR1Fit,
	      vector<TF1*> &iWR2Fit,vector<TF1*> &iZDR2Fit,vector<TF1*> &iZMR2Fit) { 
  int lNTot = 300;
  double lXVals0[lNTot];
  double lGVals0[lNTot]; 
  double lGVals1[lNTot];
  double lGVals2[lNTot]; 
  for(int i0 = 0; i0 < lNTot/2; i0++) { 
    lXVals0[i0] =  i0;
    double lE0 = 2;//TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax)*0.1;
    double lE1 = 2;//TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax)*0.1;
    double lE2 = 2;//TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax)*0.1;
    lGVals0[i0] =  Frac(iWR1Fit[0]  ->Eval(i0),iWR2Fit[0] ->Eval(i0))  - lE0;
    lGVals1[i0] =  Frac(iZDR1Fit[0] ->Eval(i0),iZDR2Fit[0]->Eval(i0))  - lE1;
    lGVals2[i0] =  Frac(iZMR1Fit[0] ->Eval(i0),iZMR2Fit[0]->Eval(i0))  - lE2;
  }
  for(int i0 = 0; i0 < lNTot/2; i0++) { 
    lXVals0[i0+lNTot/2] =  lNTot/2-1-i0;
    double lE0 = 2;//TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax)*0.1;
    double lE1 = 2;//TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax)*0.1;
    double lE2 = 2;//TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax)*0.1;
    lGVals0[i0+lNTot/2] =  Frac(iWR1Fit[0]  ->Eval(lNTot/2-1-i0),iWR2Fit[0] ->Eval(lNTot/2-1-i0))  + lE0;
    lGVals1[i0+lNTot/2] =  Frac(iZDR1Fit[0] ->Eval(lNTot/2-1-i0),iZDR2Fit[0]->Eval(lNTot/2-1-i0))  + lE1;
    lGVals2[i0+lNTot/2] =  Frac(iZMR1Fit[0] ->Eval(lNTot/2-1-i0),iZMR2Fit[0]->Eval(lNTot/2-1-i0))  + lE2;
  }
  TGraph *lG0 = new TGraph(lNTot,lXVals0,lGVals0); lG0->SetFillColor(kBlue);  lG0->SetLineColor(kBlue); lG0->SetLineWidth(5);
  TGraph *lG1 = new TGraph(lNTot,lXVals0,lGVals1); lG1->SetFillColor(kGreen); lG1->SetLineColor(kGreen);
  TGraph *lG2 = new TGraph(lNTot,lXVals0,lGVals2); lG2->SetFillColor(kRed);   lG2->SetLineColor(kRed);
  TLegend *lL = new TLegend(0.25,0.25,0.55,0.5); lL->SetFillColor(0); lL->SetBorderSize(0);
  lL->AddEntry(lG0,"Data","lf");
  lL->AddEntry(lG1,"MC","lf");

  lG1->GetXaxis()->SetTitle("Z_{p_{T}} (GeV)");
  //  lG1->GetXaxis()->SetRangeUser(0,lNTot/2-1);
  lG1->GetXaxis()->SetRangeUser(0,50);
  lG1->GetYaxis()->SetTitle(iYName.c_str());//"U_{1} Response (GeV)");
  lG1->GetYaxis()->SetRangeUser(iYMin,iYMax);
  TCanvas *lC0 = new TCanvas(iName.c_str(),iName.c_str(),800,600); lC0->cd();
  lG0->Draw("alf");
  lG1->Draw("lf");
  //lG2->Draw("lf");
  lL->Draw();
  //lC0->SaveAs((iName+"pf.png").c_str());
  //lC0->SaveAs((iName+".eps").c_str());
}

void drawRatios(string iName,string iYName,double iYMin,double iYMax,double iTMax,
	  vector<TF1*> &iZDFit,vector<TF1*> &iZMFit,vector<TF1*> &iZD1Fit,vector<TF1*> &iZM1Fit) { 
  const int lNTot = 300;
  double lXVals0[lNTot];
  double lGVals0[lNTot]; 
  double lGVals1[lNTot];
  for(int i0 = 0; i0 < lNTot/2; i0++) { 
    lXVals0[i0] =  i0;
    double lE0 = TMath::Max(getError(i0,iZDFit[0] ,iZMFit[0]) ,iTMax);
    double lE1 = TMath::Max(getError(i0,iZD1Fit[0],iZM1Fit[0]),iTMax);
    lGVals0[i0] =  iZDFit[0]->Eval(i0) /iZMFit[0]->Eval(i0)   + lE0;
    lGVals1[i0] =  iZD1Fit[0]->Eval(i0)/iZM1Fit[0]->Eval(i0)  + lE1;
  }
  for(int i0 = 0; i0 < lNTot/2; i0++) { 
    lXVals0[i0+lNTot/2] =  lNTot/2-1-i0;
    double lE0 = TMath::Max(getError(lNTot/2-1-i0,iZDFit[0] ,iZMFit[0]) ,iTMax);
    double lE1 = TMath::Max(getError(lNTot/2-1-i0,iZD1Fit[0],iZM1Fit[0]),iTMax);
    lGVals0[i0+lNTot/2] =  iZDFit[0] ->Eval(lNTot/2-1-i0) /iZMFit[0]->Eval(lNTot/2-1-i0)   - lE0;
    lGVals1[i0+lNTot/2] =  iZD1Fit[0]->Eval(lNTot/2-1-i0)/iZM1Fit[0]->Eval(lNTot/2-1-i0)  - lE1;
  }
  TGraph *lG0 = new TGraph(lNTot,lXVals0,lGVals0); lG0->SetFillColor(kBlue);  lG0->SetLineColor(kBlue); lG0->SetLineWidth(5);
  TGraph *lG1 = new TGraph(lNTot,lXVals0,lGVals1); lG1->SetFillColor(kGreen); lG1->SetLineColor(kGreen);
  lG0->GetXaxis()->SetTitle("Z_{p_{T}} (GeV)");
  //  lG0->GetXaxis()->SetRangeUser(0,lNTot/2-1);
  lG0->GetXaxis()->SetRangeUser(0,50);
  lG0->GetYaxis()->SetTitle(iYName.c_str());//"U_{1} Response (GeV)");
  lG0->GetYaxis()->SetRangeUser(iYMin,iYMax);
  lG1->SetFillStyle(3344);

  TLegend *lL = new TLegend(0.6,0.25,0.90,0.5); lL->SetFillColor(0); lL->SetBorderSize(0);
  lL->AddEntry(lG0,"new DATA/MC","lf");
  lL->AddEntry(lG1,"old DATA/MC","lf");
  
  //  lG0->GetYaxis()->SetRangeUser(0.8,1.2);
  lG0->GetYaxis()->SetRangeUser(0.,2.);
  TLine   *lLine = new TLine(1.,1.,140.,1.);
  TCanvas *lC0 = new TCanvas(iName.c_str(),iName.c_str(),800,600); lC0->cd();
  lG0->Draw("alf");
  lG1->Draw("lf");
  lL->Draw();
  lLine->Draw();
  //  lC0->SaveAs((iName+"pf.png").c_str());
  //  lC0->SaveAs((iName+".eps").c_str());

  TString leg2 = "NVtx = ";
  leg2 += vtxBin;

  TString text2 = "NVtx";
  text2 += vtxBin;
  text2 += "_";
  text2 += iName;
  text2 += "debugRATIO.png";

  TLatex latexLabel;
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  latexLabel.DrawLatex(0.7, 0.2, leg2);

  lC0->SaveAs(text2.Data());

}
void correctPars(TF1 *iF1) { 
  double lRescale = sqrt(3.14159265/2.);
  for(int i1 = 0; i1 < iF1->GetNumberFreeParameters(); i1++) { 
    iF1->SetParameter(i1,iF1->GetParameter(i1)*lRescale); iF1->SetParError(i1,iF1->GetParError(i1)*lRescale);
  }
}
void correct(vector<TF1*>   &iFit) {
  for(unsigned int i0 = 0; i0 < iFit.size(); i0++) { 
    correctPars(iFit[i0]);
  }
}
void correctPars(TF1 *iF1,TF1 *iF2) { 
  double lVal = iF1->GetParameter(0); double lValE =  iF1->GetParError(0);
  for(int i1 = 0; i1 < iF1->GetNumberFreeParameters(); i1++) { 
    iF1->SetParameter(i1,iF2->GetParameter(i1)*lVal); 
    iF1->SetParError (i1,(iF2->GetParError(i1) *lVal*lVal+iF2->GetParameter(i1)*iF2->GetParameter(i1)*lValE));
  }
}
void correct(vector<TF1*>   &iFit1,vector<TF1*>   &iFit2) {
  if(iFit1.size() != iFit2.size()) return;
  for(unsigned int i0 = 0; i0 < iFit1.size(); i0++) { 
     correctPars(iFit1[i0],iFit2[i0]);
  }
}


/*
void plotRatiosDiGaus(std::string iNameZDat="recoilfits/recoilfit_higgs53X_20pv_1jet.root",std::string iNameZMC="recoilfits/recoilfit_htt53X_20pv_1jet.root",
		      std::string iNameW="recoilfits/recoilfit_higgs53X_20pv_2jet.root"    ,std::string iNameWC ="recoilfits/recoilfit_htt53X_20pv_2jet.root") { 
  //Prep();
  */

void plotRatiosDiGaus(int myVtxBin) {

  vtxBin=myVtxBin;

  /*
  std::string iNameZDat = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/recoilfit_DATA_inc.root";
  std::string iNameZMC  = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/recoilfit_genZ_inc.root";
  std::string iNameW    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/recoilfit_genWpos_inc.root";
  std::string iNameWC   = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/recoilfit_genWneg_inc.root";
  */

  std::string iNameZDat = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_DATA_inc.root"; 
  std::string iNameZMC  = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genZ_inc.root";
  std::string iNameW    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genWpos_inc.root";  
  std::string iNameWC    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genWneg_inc.root"; 

  vector<double> lWCSumEt; vector<double> lWCWeights; 
  vector<double> lWSumEt;  vector<double> lWWeights; 
  vector<double> lZDSumEt; vector<double> lZDWeights; 
  vector<double> lZMSumEt; vector<double> lZMWeights; 
  vector<TF1*>   lWU1Fit;  vector<TF1*> lWU1RMSSMFit;  vector<TF1*> lWU1RMS1Fit;  vector<TF1*> lWU1RMS2Fit;  vector<TF1*> lWU13SigFit; 
  vector<TF1*>   lWU2Fit;  vector<TF1*> lWU2RMSSMFit;  vector<TF1*> lWU2RMS1Fit;  vector<TF1*> lWU2RMS2Fit;  vector<TF1*> lWU23SigFit; 

  vector<TF1*>   lWCU1Fit;  vector<TF1*> lWCU1RMSSMFit;  vector<TF1*> lWCU1RMS1Fit;  vector<TF1*> lWCU1RMS2Fit;  vector<TF1*> lWCU13SigFit; 
  vector<TF1*>   lWCU2Fit;  vector<TF1*> lWCU2RMSSMFit;  vector<TF1*> lWCU2RMS1Fit;  vector<TF1*> lWCU2RMS2Fit;  vector<TF1*> lWCU23SigFit; 

  vector<TF1*>   lZDU1Fit; vector<TF1*> lZDU1RMSSMFit; vector<TF1*> lZDU1RMS1Fit; vector<TF1*> lZDU1RMS2Fit; vector<TF1*> lZDU13SigFit; 
  vector<TF1*>   lZDU2Fit; vector<TF1*> lZDU2RMSSMFit; vector<TF1*> lZDU2RMS1Fit; vector<TF1*> lZDU2RMS2Fit; vector<TF1*> lZDU23SigFit;  
  vector<TF1*>   lZMU1Fit; vector<TF1*> lZMU1RMSSMFit; vector<TF1*> lZMU1RMS1Fit; vector<TF1*> lZMU1RMS2Fit; vector<TF1*> lZMU13SigFit; 
  vector<TF1*>   lZMU2Fit; vector<TF1*> lZMU2RMSSMFit; vector<TF1*> lZMU2RMS1Fit; vector<TF1*> lZMU2RMS2Fit; vector<TF1*> lZMU23SigFit; 
 
  readRecoil(lWCSumEt ,lWCU1Fit ,lWCU1RMSSMFit ,lWCU1RMS1Fit ,lWCU1RMS2Fit ,lWCU13SigFit ,lWCU2Fit ,lWCU2RMSSMFit ,lWCU2RMS1Fit ,lWCU2RMS2Fit ,lWCU23SigFit ,iNameWC   ,"PF");
  
  readRecoil(lWSumEt ,lWU1Fit ,lWU1RMSSMFit ,lWU1RMS1Fit ,lWU1RMS2Fit ,lWU13SigFit ,lWU2Fit ,lWU2RMSSMFit ,lWU2RMS1Fit ,lWU2RMS2Fit ,lWU23SigFit ,iNameW   ,"PF");
  readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU13SigFit,lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU23SigFit,iNameZDat,"PF");
  readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,iNameZMC ,"PF");
  //correct(lWU1RMSSMFit);   correct(lZDU1RMSSMFit);   correct(lZMU1RMSSMFit);
  //correct(lWU2RMSSMFit);   correct(lZDU2RMSSMFit);   correct(lZMU2RMSSMFit);

  //correct(lWU1RMS1Fit,lWU1RMSSMFit); correct(lZDU1RMS1Fit,lZDU1RMSSMFit); correct(lZMU1RMS1Fit,lZMU1RMSSMFit);
  //correct(lWU2RMS1Fit,lWU2RMSSMFit); correct(lZDU2RMS1Fit,lZDU2RMSSMFit); correct(lZMU2RMS1Fit,lZMU2RMSSMFit);
  
  //correct(lWU1RMS2Fit,lWU1RMSSMFit); correct(lZDU1RMS2Fit,lZDU1RMSSMFit); correct(lZMU1RMS2Fit,lZMU1RMSSMFit);
  //correct(lWU2RMS2Fit,lWU2RMSSMFit); correct(lZDU2RMS2Fit,lZDU2RMSSMFit); correct(lZMU2RMS2Fit,lZMU2RMSSMFit);
  
  //  draw("u1" ,"U_{1} Response (GeV)"           , 0.8,1.2,0.2 ,lWU1Fit,     lZDU1Fit,     lZMU1Fit,     lWCU1Fit);
  draw("u1" ,"U_{1} Response (GeV)"           , -1.5,1.5,0.2 ,lWU1Fit,     lZDU1Fit,     lZMU1Fit,     lWCU1Fit);
  draw("u1MR" ,"U_{1} Mean Resolution (GeV)"  ,   0, 20,0.05,lWU1RMSSMFit,lZDU1RMSSMFit,lZMU1RMSSMFit,lWCU1RMSSMFit);
  /*
    draw("u1R1" ,"U_{1} Resolution 1 (GeV)"  ,     0,11,0.05  ,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit ,lWCU1RMS1Fit);
    draw("u1R2" ,"U_{1} Resolution 2 (GeV)"  ,     0,21,0.05  ,lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit ,lWCU1RMS2Fit);
  */
  draw("u1R1" ,"U_{1} Resolution 1 (GeV)"  ,     0,1.5,0.05  ,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit ,lWCU1RMS1Fit);
  draw("u1R2" ,"U_{1} Resolution 2 (GeV)"  ,     0,1.5,0.05  ,lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit ,lWCU1RMS2Fit);

  //draw("u13R" ,"U_{1} > 3 #sigma (GeV)"    ,     0,11,0.05,lWU13SigFit ,lZDU13SigFit ,lZMU13SigFit );
  //drawFrac("u1Frac" ,"U_{1} Frac #sigma_{1}" ,     0,2,0.05,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit,
  //   lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit );

  draw("u2" ,"U_{2} Response (GeV)"         , -5,5,0.2 ,lWU2Fit     ,lZDU2Fit     ,lZMU2Fit,lWCU2Fit);
  draw("u2MR" ,"U_{2} Mean Resolution (GeV)"  ,  0,20,0.05,lWU2RMSSMFit,lZDU2RMSSMFit,lZMU2RMSSMFit,lWCU2RMSSMFit);
  /*
  draw("u2R1" ,"U_{2} Resolution 1 (GeV)"  ,     0,11,0.05,lWU2RMS1Fit ,lZDU2RMS1Fit ,lZMU2RMS1Fit ,lWCU2RMS1Fit);
  draw("u2R2" ,"U_{2} Resolution 2 (GeV)"  ,     0,21,0.05,lWU2RMS2Fit ,lZDU2RMS2Fit ,lZMU2RMS2Fit ,lWCU2RMS2Fit);
  */

  draw("u2R1" ,"U_{2} Resolution 1 (GeV)"  ,     0,1.5,0.05,lWU2RMS1Fit ,lZDU2RMS1Fit ,lZMU2RMS1Fit ,lWCU2RMS1Fit);
  draw("u2R2" ,"U_{2} Resolution 2 (GeV)"  ,     0,1.5,0.05,lWU2RMS2Fit ,lZDU2RMS2Fit ,lZMU2RMS2Fit ,lWCU2RMS2Fit);

  //drawFrac("u2Frac" ,"U_{2} Frac #sigma_{2}" ,     0,2,0.05,lWU2RMS1Fit ,lZDU2RMS1Fit ,lZMU2RMS1Fit,
  //	                                                    lWU2RMS2Fit ,lZDU2RMS2Fit ,lZMU2RMS2Fit );
  //draw("u23R" ,"U_{2} > 3 #sigma (GeV)"    ,     0,11,0.05,lWU23SigFit ,lZDU23SigFit ,lZMU23SigFit );
}

/*
void plotRatiosComp(std::string iNameZMC ="recoilfits/recoilfit_zmm52X_pf_v3.root",     std::string iNameZDat ="recoilfits/recoilfit_datamm52X_pf_v3.root",
		    std::string iNameZMC1="recoilfits/recoilfit_zmm52X_mva_v3.root"   ,    std::string iNameZDat1="recoilfits/recoilfit_datamm52X_mva_v3.root") { 
*/
  //  Prep();
void plotRatiosComp(int myVtxBin) {

  vtxBin=myVtxBin;

  std::string iNameZDat = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_DATA_inc.root"; 
  std::string iNameZMC  = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genZ_inc.root";

  std::string iNameZMC1    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/testSEPT11/recoilfit_genZ_inc.root";  
  std::string iNameZDat1    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/testSEPT11/recoilfit_DATA_inc.root";

  /*
  std::string iNameZDat = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genWpos_inc.root";
  std::string iNameZMC  = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genWneg_inc.root";

  std::string iNameZMC1    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/recoilfit_genWpos_inc.root";  
  std::string iNameZDat1    = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/recoilfit_genWneg_inc.root";
  */

  vector<double> lZDSumEt; vector<double> lZDWeights; 
  vector<double> lZMSumEt; vector<double> lZMWeights; 
  vector<TF1*>   lZDU1Fit; vector<TF1*> lZDU1RMSSMFit; vector<TF1*> lZDU1RMS1Fit; vector<TF1*> lZDU1RMS2Fit; vector<TF1*> lZDU13SigFit; 
  vector<TF1*>   lZDU2Fit; vector<TF1*> lZDU2RMSSMFit; vector<TF1*> lZDU2RMS1Fit; vector<TF1*> lZDU2RMS2Fit; vector<TF1*> lZDU23SigFit;  
  vector<TF1*>   lZMU1Fit; vector<TF1*> lZMU1RMSSMFit; vector<TF1*> lZMU1RMS1Fit; vector<TF1*> lZMU1RMS2Fit; vector<TF1*> lZMU13SigFit; 
  vector<TF1*>   lZMU2Fit; vector<TF1*> lZMU2RMSSMFit; vector<TF1*> lZMU2RMS1Fit; vector<TF1*> lZMU2RMS2Fit; vector<TF1*> lZMU23SigFit; 

  vector<double> lZD1SumEt; vector<double> lZD1Weights; 
  vector<double> lZM1SumEt; vector<double> lZM1Weights; 
  vector<TF1*>   lZD1U1Fit; vector<TF1*> lZD1U1RMSSMFit; vector<TF1*> lZD1U1RMS1Fit; vector<TF1*> lZD1U1RMS2Fit; vector<TF1*> lZD1U13SigFit; 
  vector<TF1*>   lZD1U2Fit; vector<TF1*> lZD1U2RMSSMFit; vector<TF1*> lZD1U2RMS1Fit; vector<TF1*> lZD1U2RMS2Fit; vector<TF1*> lZD1U23SigFit;  
  vector<TF1*>   lZM1U1Fit; vector<TF1*> lZM1U1RMSSMFit; vector<TF1*> lZM1U1RMS1Fit; vector<TF1*> lZM1U1RMS2Fit; vector<TF1*> lZM1U13SigFit; 
  vector<TF1*>   lZM1U2Fit; vector<TF1*> lZM1U2RMSSMFit; vector<TF1*> lZM1U2RMS1Fit; vector<TF1*> lZM1U2RMS2Fit; vector<TF1*> lZM1U23SigFit; 
 
  readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU13SigFit,lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU23SigFit,iNameZDat,"PF");
  readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,iNameZMC,"PF");
  readRecoil(lZD1SumEt,lZD1U1Fit,lZD1U1RMSSMFit,lZD1U1RMS1Fit,lZD1U1RMS2Fit,lZD1U13SigFit,lZD1U2Fit,lZD1U2RMSSMFit,lZD1U2RMS1Fit,lZD1U2RMS2Fit,lZD1U23SigFit,iNameZDat1,"PF");
  readRecoil(lZM1SumEt,lZM1U1Fit,lZM1U1RMSSMFit,lZM1U1RMS1Fit,lZM1U1RMS2Fit,lZM1U13SigFit,lZM1U2Fit,lZM1U2RMSSMFit,lZM1U2RMS1Fit,lZM1U2RMS2Fit,lZM1U23SigFit,iNameZMC1,"PF");


  //correct(lZDU1RMSSMFit);   correct(lZMU1RMSSMFit);  correct(lZD1U1RMSSMFit);   correct(lZM1U1RMSSMFit);
  //correct(lZDU2RMSSMFit);   correct(lZMU2RMSSMFit);  correct(lZD1U2RMSSMFit);   correct(lZM1U2RMSSMFit);

  //correct(lZDU1RMS1Fit,lZDU1RMSSMFit); correct(lZMU1RMS1Fit,lZMU1RMSSMFit); correct(lZDU1RMS2Fit,lZD1U1RMSSMFit); correct(lZMU1RMS2Fit,lZM1U1RMSSMFit);
  //correct(lZDU2RMS1Fit,lZDU2RMSSMFit); correct(lZMU2RMS1Fit,lZMU2RMSSMFit); correct(lZDU2RMS2Fit,lZD1U2RMSSMFit); correct(lZMU2RMS2Fit,lZM1U2RMSSMFit);

  //correct(lZD1U1RMS1Fit,lZDU1RMSSMFit); correct(lZM1U1RMS1Fit,lZMU1RMSSMFit);  correct(lZD1U1RMS2Fit,lZD1U1RMSSMFit); correct(lZM1U1RMS2Fit,lZM1U1RMSSMFit);
  //correct(lZD1U2RMS1Fit,lZDU2RMSSMFit); correct(lZM1U2RMS1Fit,lZMU2RMSSMFit);  correct(lZD1U2RMS2Fit,lZD1U2RMSSMFit); correct(lZM1U2RMS2Fit,lZM1U2RMSSMFit);
  
  drawRatios("Ru1" ,"U_{1} Response (GeV)"         ,     0,2,0.01,lZDU1Fit     ,lZMU1Fit     ,lZD1U1Fit     ,lZM1U1Fit);
  drawRatios("Ru1MR" ,"U_{1} Mean Resolution (GeV)"  ,   0,2,0.01,lZDU1RMSSMFit,lZMU1RMSSMFit,lZD1U1RMSSMFit,lZM1U1RMSSMFit);
  ////  drawRatios("Ru1R1" ,"U_{1} Resolution 1 (GeV)"  ,      0,2,0.01,lZDU1RMS1Fit ,lZMU1RMS1Fit ,lZD1U1RMS1Fit ,lZM1U1RMS1Fit );
  ////  drawRatios("Ru1R2" ,"U_{1} Resolution 2 (GeV)"  ,      0,2,0.01,lZDU1RMS2Fit ,lZMU1RMS2Fit ,lZD1U1RMS2Fit ,lZM1U1RMS2Fit );
  //drawRatios("Ru13R" ,"U_{1} > 3 #sigma (GeV)"    ,      0,2,0.01,lZDU13SigFit ,lZMU13SigFit ,lZD1U13SigFit ,lZM1U13SigFit );

  drawRatios("Ru2" ,"U_{2} Response (GeV)"         ,    0,2,0.01,lZDU2Fit     ,lZMU2Fit     ,lZD1U2Fit     ,lZM1U2Fit);
  drawRatios("Ru2MR" ,"U_{2} Mean Resolution (GeV)"  ,  0,2,0.01,lZDU2RMSSMFit,lZMU2RMSSMFit,lZD1U2RMSSMFit,lZM1U2RMSSMFit);
  ////  drawRatios("Ru2R1" ,"U_{2} Resolution 1 (GeV)"  ,     0,2,0.01,lZDU2RMS1Fit ,lZMU2RMS1Fit ,lZD1U2RMS1Fit ,lZM1U2RMS1Fit );
  ////  drawRatios("Ru2R2" ,"U_{2} Resolution 2 (GeV)"  ,     0,2,0.01,lZDU2RMS2Fit ,lZMU2RMS2Fit ,lZD1U2RMS2Fit ,lZM1U2RMS2Fit );
  //drawRatios("Ru23R" ,"U_{2} > 3 #sigma (GeV)"    ,     0,2,0.01,lZDU23SigFit ,lZMU23SigFit ,lZDU23SigFit  ,lZM1U23SigFit );

}
