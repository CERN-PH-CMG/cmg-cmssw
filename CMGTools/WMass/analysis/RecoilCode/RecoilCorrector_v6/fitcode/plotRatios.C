/*
### do NOTHING
root.exe -b -q -x plotRatios.C+\(1,true,false,false,false,false,false,false,0\);

### DO ITERATIVE
root.exe -b -q -x plotRatios.C+\(1,true,true,false,false,false,false,false,0\);
root.exe -b -q -x plotRatios.C+\(125,true,true,false,false,false,false,false,0\);
root.exe -b -q -x plotRatios.C+\(150,true,true,false,false,false,false,false,0\);
root.exe -b -q -x plotRatios.C+\(175,true,true,false,false,false,false,false,0\);
root.exe -b -q -x plotRatios.C+\(200,true,true,false,false,false,false,false,0\);
root.exe -b -q -x plotRatios.C+\(201,true,true,false,false,false,false,false,0\);

### DO CORR studies
root.exe -b -q -x plotRatios.C+\(1,true,false,false,false,true,true,false,0\);
root.exe -b -q -x plotRatios.C+\(1,true,false,false,false,true,false,true,0\);

### DO RAP studies
root.exe -b -q -x plotRatios.C+\(1,true,false,true,false,false,false,false,1\);
root.exe -b -q -x plotRatios.C+\(1,true,false,true,false,false,false,false,2\);
root.exe -b -q -x plotRatios.C+\(1,true,false,true,false,false,false,false,3\);

### DO PDF studies
root.exe -b -q -x plotRatios.C+\(1,true,false,false,true,false,false,false,0\);
root.exe -b -q -x plotRatios.C+\(1,true,false,false,true,false,false,false,1\);
root.exe -b -q -x plotRatios.C+\(1,true,false,false,true,false,false,false,2\);
root.exe -b -q -x plotRatios.C+\(1,true,false,false,true,false,false,false,3\);
root.exe -b -q -x plotRatios.C+\(1,true,false,false,true,false,false,false,4\);

exit
*/ 

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
 #include "TStyle.h"

 //#include <vector>
 //#include "TLorentzVector.h"

 //#include "../../NYStyle/test/NYStyle.h"
 //#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_9_4/src/MitWlnu/NYStyle/test/NYStyle.h"

 using namespace std;

bool doPol2=false;
bool doMad=false;
 int vtxBin = -1;
 int howManyToPlot = -1; 
 bool doIterClosure = false;

 bool doMuonCheck = false;

 bool doSingleDouble = false;
 bool doSinglePtStudy = false;
 bool doCorrelationB = false;
 bool doCorrelationA = false;
 bool doCorrelation = false;
 bool doRapStudies = false;
 bool doPDF = false;

int mycase=-1;

// for rapidity
// int mycase=1; // Wpos / Wneg
// int mycase=2; // Wpos / Z
// int mycase=3; // Wneg / Z

// PDF 
// int mycase=0; // gluon W/Z
// int mycase=1; // valence W/Z 
// int mycase=2; // ratio flavor Z
// int mycase=3; // ratio flavor Wpos
// int mycase=4; // ratio flavor Wpos

 const int readRecoil(std::vector<double> &iSumEt,
		      std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1Sig3Fit,
		      std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2Sig3Fit,
		      std::string iFName = "recoilfit.root",std::string iPrefix="") { 
   TFile *lFile  = new TFile(iFName.c_str());
   lFile->ls();

   //TGraph *lGraph = (TGraph *) lFile->FindObjectAny("sumet");
   const int lNBins = 201;//lGraph->GetN();

   int init = -1;
   for(int i0 = init; i0 <= lNBins; i0++) {
     if(!doRapStudies && i0!=vtxBin) continue;
       if(( (doRapStudies) && ((i0==1) || (i0==125) || (i0==150) || (i0==175) || (i0==200) || (i0==201) )) || (!doRapStudies) ) {
	 cout << i0 << endl;

	 std::string lStr = iPrefix;
	 //iSumEt.push_back(lGraph->GetY()[i0]);
	 std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10;
	 pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);
	 pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
	 //	pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
	 //	pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
	 //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
	 pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);
	 pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
	 //	pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
	 //	pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
	 //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);

       }
   }

   cout << "SIZE " << iU1Fit.size() << endl;

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
   if(iFit->GetParError(3)!= 0) lE2 += iVal*iVal*iVal*  iFit->GetParError(3);
   if(iFit->GetParError(4)!= 0) lE2 += iVal*iVal*iVal*iVal*  iFit->GetParError(4);
   if(iFit->GetParError(5)!= 0) lE2 += iVal*iVal*iVal*iVal*iVal*  iFit->GetParError(5);
   if(iFit->GetParError(6)!= 0) lE2 += iVal*iVal*iVal*iVal*iVal*iVal*  iFit->GetParError(6);

   if(lE2<0) {
     cout << "Negative Error will cause nan at iVal " << iVal << endl;
     cout << "  iFit->GetParError(0) " <<  iFit->GetParError(0) << endl;
     cout << "  iFit->GetParError(1) " <<  iFit->GetParError(1) << endl;
     cout << "  iFit->GetParError(2) " <<  iFit->GetParError(2) << endl;
     cout << "  iFit->GetParError(3) " <<  iFit->GetParError(3) << endl;
     cout << "  iFit->GetParError(4) " <<  iFit->GetParError(4) << endl;
     cout << "  iFit->GetParError(5) " <<  iFit->GetParError(5) << endl;
     cout << "  iFit->GetParError(6) " <<  iFit->GetParError(6) << endl;

   }
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
     lGVals3[i0+lNTot/2] =  iWCFit[0]->Eval(lNTot/2-1-i0)  - lE3;
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
   /*
   lL->AddEntry(lG1,"Data Z (unbinned)","lf");
   lL->AddEntry(lG2,"MC Z (unbinned)"  ,"lf");

   //  lL->AddEntry(lG0,"MC Z powheg","lf");
   lL->AddEntry(lG0,"MC W pos","lf");
   lL->AddEntry(lG3,"MC W meg","lf");
   */

   //  lL->AddEntry(lG0,"MC Z (binned)","lf");
   //  lL->AddEntry(lG3,"DATA Z (binned)"  ,"lf");

   lL->AddEntry(lG1,"DATA tkmet"  ,"lf");
   lL->AddEntry(lG2,"MC tkmet","lf");
   lL->AddEntry(lG3,"DATA pfmet","lf");
   lL->AddEntry(lG0,"MC pfmet","lf");

   /*
     lL->AddEntry(lG1,"DATA pol3"  ,"lf");
     lL->AddEntry(lG2,"MC pol3","lf");
     lL->AddEntry(lG3,"DATA pol2","lf");
     lL->AddEntry(lG0,"MC pol2","lf");
   */

   lG1->Draw("alf");
   lG2->Draw("lf");
   lG3->Draw("lf");
   lG0->Draw("lf");

   lL->Draw();
   TLine   *lLine = new TLine(1.,1.,140.,1.); lLine->SetLineStyle(kDashed);
   lLine->Draw();

   TString text1 = "NVtx";
   text1 += vtxBin;
   text1 += "_";
   text1 += iName;
   text1 += "debug.png";

   TString leg1 = "NVtx = ";
   leg1 += vtxBin;

   TLatex latexLabel;
   latexLabel.SetTextSize(0.04);
   latexLabel.SetNDC();
   latexLabel.DrawLatex(0.7, 0.2, leg1);
   latexLabel.DrawLatex(0.7, 0.3, "pol2");

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
   int lNTot = 100;
   double lXVals0[lNTot];
   double lGVals0[lNTot]; 
   double lGVals1[lNTot];
   double lGVals2[lNTot]; 

   for(int i0 = 0; i0 < lNTot/2; i0++) { 
     lXVals0[i0] =  i0;
     /*
     double lE0 = 2;//TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax)*0.1;
     double lE1 = 2;//TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax)*0.1;
     double lE2 = 2;//TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax)*0.1;
     double lE0 = 0.1;//TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax)*0.1;
     double lE1 = 0.1;//TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax)*0.1;
     double lE2 = 0.1;//TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax)*0.1;
     */
     double lE0 = TMath::Max(sqrt(getError2(i0,iWR1Fit[0])) ,iTMax)*0.1;
     double lE1 = TMath::Max(sqrt(getError2(i0,iZDR1Fit[0])),iTMax)*0.1;
     double lE2 = TMath::Max(sqrt(getError2(i0,iZMR1Fit[0])),iTMax)*0.1;
     lGVals0[i0] =  Frac(iWR1Fit[0]  ->Eval(i0),iWR2Fit[0] ->Eval(i0))  - lE0;
     lGVals1[i0] =  Frac(iZDR1Fit[0] ->Eval(i0),iZDR2Fit[0]->Eval(i0))  - lE1;
     lGVals2[i0] =  Frac(iZMR1Fit[0] ->Eval(i0),iZMR2Fit[0]->Eval(i0))  - lE2;

   }
   for(int i0 = 0; i0 < lNTot/2; i0++) { 
     lXVals0[i0+lNTot/2] =  lNTot/2-1-i0;
     /*
     double lE0 = 2;//TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax)*0.1;
     double lE1 = 2;//TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax)*0.1;
     double lE2 = 2;//TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax)*0.1;
     */
     /*
     double lE0 = 0.1;//TMath::Max(sqrt(getError2(i0,iWFit[0])) ,iTMax)*0.1;
     double lE1 = 0.1;//TMath::Max(sqrt(getError2(i0,iZDFit[0])),iTMax)*0.1;
     double lE2 = 0.1;//TMath::Max(sqrt(getError2(i0,iZMFit[0])),iTMax)*0.1;
     */
     double lE0 = TMath::Max(sqrt(getError2(i0,iWR1Fit[0])) ,iTMax)*0.1;
     double lE1 = TMath::Max(sqrt(getError2(i0,iZDR1Fit[0])),iTMax)*0.1;
     double lE2 = TMath::Max(sqrt(getError2(i0,iZMR1Fit[0])),iTMax)*0.1;
     lGVals0[i0+lNTot/2] =  Frac(iWR1Fit[0]  ->Eval(lNTot/2-1-i0),iWR2Fit[0] ->Eval(lNTot/2-1-i0))  + lE0;
     lGVals1[i0+lNTot/2] =  Frac(iZDR1Fit[0] ->Eval(lNTot/2-1-i0),iZDR2Fit[0]->Eval(lNTot/2-1-i0))  + lE1;
     lGVals2[i0+lNTot/2] =  Frac(iZMR1Fit[0] ->Eval(lNTot/2-1-i0),iZMR2Fit[0]->Eval(lNTot/2-1-i0))  + lE2;
   }
   TGraph *lG0 = new TGraph(lNTot,lXVals0,lGVals0); lG0->SetFillColor(kBlue);  lG0->SetLineColor(kBlue); lG0->SetLineWidth(5);
   TGraph *lG1 = new TGraph(lNTot,lXVals0,lGVals1); lG1->SetFillColor(kGreen); lG1->SetLineColor(kGreen);
   TGraph *lG2 = new TGraph(lNTot,lXVals0,lGVals2); lG2->SetFillColor(kRed);   lG2->SetLineColor(kRed);
   TLegend *lL = new TLegend(0.25,0.25,0.55,0.5); lL->SetFillColor(0); lL->SetBorderSize(0);
   lL->AddEntry(lG0,"binned MC","lf");
   lL->AddEntry(lG1,"unbinned DATA","lf");
   lL->AddEntry(lG2,"unbinned MC","lf");

   lG0->GetXaxis()->SetTitle("Z_{p_{T}} (GeV)");
   //  lG2->GetXaxis()->SetRangeUser(0,lNTot/2-1);
   //  lG0->GetXaxis()->SetRangeUser(0,50);
   lG0->GetYaxis()->SetTitle(iYName.c_str());//"U_{1} Response (GeV)");
   //  lG2->GetYaxis()->SetRangeUser(iYMin,iYMax);
   lG2->GetYaxis()->SetRangeUser(0.,1.);
   lG0->GetYaxis()->SetRangeUser(0.,1.);
   TCanvas *lC0 = new TCanvas(iName.c_str(),iName.c_str(),800,600); lC0->cd();
   lG0->Draw("alf");
   lG1->Draw("lf");
   lG2->Draw("lf");
   //  lG2->Draw("lf");
   //  cout << "MARIA drawFrac"<< endl;

   TString leg2 = "NVtx = ";
   leg2 += vtxBin;

   TString text2 = "NVtx";
   text2 += vtxBin;
   text2 += "_";
   text2 += iName;
   text2 += ".png";

   TLatex latexLabel;
   latexLabel.SetTextSize(0.04);
   latexLabel.SetNDC();
   latexLabel.DrawLatex(0.7, 0.2, leg2);

   lL->Draw();
   lC0->SaveAs(text2.Data());
   //lC0->SaveAs((iName+".eps").c_str());

 }

 void drawRatios(string iName,string iYName,double iYMin,double iYMax,double iTMax,
		 vector<TF1*> &iZDFit,vector<TF1*> &iZMFit,
		 vector<TF1*> &iZD1Fit,vector<TF1*> &iZM1Fit ,
		 vector<TF1*> &iZD2Fit,vector<TF1*> &iZM2Fit,
		 vector<TF1*> &iZD3Fit,vector<TF1*> &iZM3Fit
		 ) { 

   cout << "iZDFit  " << iZDFit.size() << endl;
   cout << "iZD1Fit " << iZD1Fit.size() << endl;
   cout << "iZD2Fit " << iZD2Fit.size() << endl;
   cout << "iZD3Fit " << iZD3Fit.size() << endl;

   cout << "iZMFit  " << iZMFit.size() << endl;
   cout << "iZM1Fit " << iZM1Fit.size() << endl;
   cout << "iZM2Fit " << iZM2Fit.size() << endl;
   cout << "iZM3Fit " << iZM3Fit.size() << endl;

   gStyle->SetHatchesLineWidth(3);

   const int lNTot = 50;
   double lXVals0[lNTot];
   double lGVals0[lNTot]; 
   double lGVals1[lNTot];
   double lGVals2[lNTot];
   double lGVals3[lNTot];
   double lGVals4[lNTot];
   double lGVals5[lNTot];

   for(int index=0; index<iZDFit.size(); index++) {
   //  for(int index=0; index<1; index++) {
     for(int i0 = 0; i0 < lNTot/2; i0++) { 
       // this is going forward
       lXVals0[i0] =  i0;
       //    cout << " lXVals0 1nd  "  << lXVals0[i0] << endl;
       double lE0; double lE1; double lE2; double lE3;
       if(!doRapStudies) {
	 lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax);
	 if(howManyToPlot>1) lE1 = TMath::Max(getError(i0,iZD1Fit[index],iZM1Fit[index]),iTMax);
	 if(howManyToPlot>2) lE2 = TMath::Max(getError(i0,iZD2Fit[index],iZM2Fit[index]),iTMax);
	 if(howManyToPlot>3) lE3 = TMath::Max(getError(i0,iZD3Fit[index],iZM2Fit[index]),iTMax);
	 if(iZMFit[index]->Eval(i0)==0) cout << "not defined function iZMFit " << i0 << endl;
	 if(howManyToPlot>1 && iZM1Fit[index]->Eval(i0)==0) cout << "not defined function iZM1Fit " << i0 << endl;
	 if(howManyToPlot>2 && iZM2Fit[index]->Eval(i0)==0) cout << "not defined function iZM2Fit " << i0 << endl;
	 if(howManyToPlot>3 && iZM3Fit[index]->Eval(i0)==0) cout << "not defined function iZM3Fit " << i0 << endl;

	 if(iZMFit[index]->Eval(i0)!=0) lGVals0[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0;
	 if(howManyToPlot>1 && iZM1Fit[index]->Eval(i0)!=0) lGVals1[i0] =  iZD1Fit[index]->Eval(i0)/iZM1Fit[index]->Eval(i0)  + lE1;
	 if(howManyToPlot>2 && iZM2Fit[index]->Eval(i0)!=0) lGVals2[i0] =  iZD2Fit[index]->Eval(i0)/iZM2Fit[index]->Eval(i0)  + lE2;
	 if(howManyToPlot>3 && iZM3Fit[index]->Eval(i0)!=0) lGVals3[i0] =  iZD3Fit[index]->Eval(i0)/iZM3Fit[index]->Eval(i0)  + lE3;
       }
       if(doRapStudies) {
	 if(index==0 && iZMFit[index]->Eval(i0)!=0) {  lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals0[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0; }
	 if(index==1 && iZMFit[index]->Eval(i0)!=0) {  lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals1[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0; }
	 if(index==2 && iZMFit[index]->Eval(i0)!=0) {  lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals2[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0; }
	 if(index==3 && iZMFit[index]->Eval(i0)!=0) {  lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals3[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0; }
	 if(index==4 && iZMFit[index]->Eval(i0)!=0) {  lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals4[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0; }
	 if(index==5 && iZMFit[index]->Eval(i0)!=0) {  lE0 = TMath::Max(getError(i0,iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals5[i0] =  iZDFit[index]->Eval(i0) /iZMFit[index]->Eval(i0)   + lE0; }
       }

     }
   }

   for(int index=0; index<iZDFit.size(); index++) {
   //   for(int index=0; index<1; index++) {
     for(int i0 = 0; i0 < lNTot/2; i0++) { 
       // this is going backwards
       lXVals0[i0+lNTot/2] =  lNTot/2-1-i0;
       //    cout << " lXVals0 2nd "  << lXVals0[i0+lNTot/2] << endl;
       double lE0; double lE1; double lE2; double lE3;
       if(!doRapStudies) {
	 lE0 = TMath::Max(getError(lNTot/2-1-i0,iZDFit[index] ,iZMFit[index]) ,iTMax);
	 if(howManyToPlot>1) lE1 = TMath::Max(getError(lNTot/2-1-i0,iZD1Fit[index],iZM1Fit[index]),iTMax);
	 if(howManyToPlot>2) lE2 = TMath::Max(getError(lNTot/2-1-i0,iZD2Fit[index],iZM2Fit[index]),iTMax);
	 if(howManyToPlot>3) lE3 = TMath::Max(getError(lNTot/2-1-i0,iZD3Fit[index],iZM3Fit[index]),iTMax);

	 if(iZMFit[index]->Eval(i0)==0) cout << "not defined function iZMFit " << i0 << endl;
	 if(howManyToPlot>1 && iZM1Fit[index]->Eval(i0)==0) cout << "not defined function iZM1Fit " << i0 << endl;
	 if(howManyToPlot>2 && iZM2Fit[index]->Eval(i0)==0) cout << "not defined function iZM2Fit " << i0 << endl;
	 if(howManyToPlot>3 && iZM3Fit[index]->Eval(i0)==0) cout << "not defined function iZM3Fit " << i0 << endl;

	 if(iZMFit[index]->Eval(lNTot/2-1-i0)!=0) lGVals0[i0+lNTot/2] =  iZDFit[index] ->Eval(lNTot/2-1-i0) /iZMFit[0]->Eval(lNTot/2-1-i0)  - lE0;
	 if(howManyToPlot>1 && iZM1Fit[index]->Eval(lNTot/2-1-i0)!=0) lGVals1[i0+lNTot/2] =  iZD1Fit[index]->Eval(lNTot/2-1-i0)/iZM1Fit[0]->Eval(lNTot/2-1-i0)  - lE1;
	 if(howManyToPlot>2 && iZM2Fit[index]->Eval(lNTot/2-1-i0)!=0) lGVals2[i0+lNTot/2] =  iZD2Fit[index]->Eval(lNTot/2-1-i0)/iZM2Fit[index]->Eval(lNTot/2-1-i0)  - lE2;
	 if(howManyToPlot>3 && iZM3Fit[index]->Eval(lNTot/2-1-i0)!=0) lGVals3[i0+lNTot/2] =  iZD3Fit[index]->Eval(lNTot/2-1-i0)/iZM3Fit[index]->Eval(lNTot/2-1-i0)  - lE3;
       }

       if(doRapStudies) {

	 if(index==0 && iZMFit[index]->Eval(lNTot/2-1-i0)!=0) { lE0 = TMath::Max(getError(lNTot/2-1-i0, iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals0[i0+lNTot/2] =  iZDFit[index]->Eval(lNTot/2-1-i0) /iZMFit[index]->Eval(lNTot/2-1-i0)   - lE0; }
	 if(index==1 && iZMFit[index]->Eval(lNTot/2-1-i0)!=0) { lE0 = TMath::Max(getError(lNTot/2-1-i0, iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals1[i0+lNTot/2] =  iZDFit[index]->Eval(lNTot/2-1-i0) /iZMFit[index]->Eval(lNTot/2-1-i0)   - lE0; }
	 if(index==2 && iZMFit[index]->Eval(lNTot/2-1-i0)!=0) { lE0 = TMath::Max(getError(lNTot/2-1-i0, iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals2[i0+lNTot/2] =  iZDFit[index]->Eval(lNTot/2-1-i0) /iZMFit[index]->Eval(lNTot/2-1-i0)   - lE0; }
	 if(index==3 && iZMFit[index]->Eval(lNTot/2-1-i0)!=0) { lE0 = TMath::Max(getError(lNTot/2-1-i0, iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals3[i0+lNTot/2] =  iZDFit[index]->Eval(lNTot/2-1-i0) /iZMFit[index]->Eval(lNTot/2-1-i0)   - lE0; }
	 if(index==4 && iZMFit[index]->Eval(lNTot/2-1-i0)!=0) { lE0 = TMath::Max(getError(lNTot/2-1-i0, iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals4[i0+lNTot/2] =  iZDFit[index]->Eval(lNTot/2-1-i0) /iZMFit[index]->Eval(lNTot/2-1-i0)   - lE0; }
	 if(index==5 && iZMFit[index]->Eval(lNTot/2-1-i0)!=0) { lE0 = TMath::Max(getError(lNTot/2-1-i0, iZDFit[index] ,iZMFit[index]) ,iTMax); lGVals5[i0+lNTot/2] =  iZDFit[index]->Eval(lNTot/2-1-i0) /iZMFit[index]->Eval(lNTot/2-1-i0)   - lE0; }

       }


     }
   }


   TGraph *lG0 = new TGraph(lNTot,lXVals0,lGVals0); lG0->SetFillColor(kBlue);  lG0->SetLineColor(kBlue); lG0->SetLineWidth(10);
   //  TGraph *lG0 = new TGraph(lNTot,lXVals0,lGVals0); lG0->SetFillColor(kRed);  lG0->SetLineColor(kRed); lG0->SetLineWidth(10);
   TGraph *lG1 = new TGraph(lNTot,lXVals0,lGVals1); lG1->SetFillColor(kGreen+1); lG1->SetLineColor(kGreen+1); lG1->SetLineWidth(10);
   TGraph *lG2 = new TGraph(lNTot,lXVals0,lGVals2); lG2->SetFillColor(kRed); lG2->SetLineColor(kRed); lG2->SetLineWidth(10);
   TGraph *lG3 = new TGraph(lNTot,lXVals0,lGVals3); lG3->SetFillColor(kOrange); lG3->SetLineColor(kOrange); lG3->SetLineWidth(10);
   TGraph *lG4 = new TGraph(lNTot,lXVals0,lGVals4); lG4->SetFillColor(kGray); lG4->SetLineColor(kGray); lG4->SetLineWidth(10);
   TGraph *lG5 = new TGraph(lNTot,lXVals0,lGVals5); lG5->SetFillColor(kMagenta); lG5->SetLineColor(kMagenta); lG5->SetLineWidth(10);

   lG0->GetXaxis()->SetTitle("Z_{p_{T}} (GeV)");
   lG0->SetTitle("");
   lG0->GetXaxis()->SetRangeUser(0,50);
   lG0->GetYaxis()->SetTitle(iYName.c_str());
   lG0->GetYaxis()->SetRangeUser(iYMin,iYMax);

   lG0->SetFillStyle(3325);
   lG1->SetFillStyle(3352);
   lG2->SetFillStyle(3395);
   lG3->SetFillStyle(3305);

   lG4->SetFillStyle(3344);
   lG5->SetFillStyle(3409);

   TLegend *lL = new TLegend(0.5,0.65,0.95,0.9); lL->SetFillColor(0); lL->SetBorderSize(0);
   //  TLegend *lL = new TLegend(0.3,0.65,0.95,0.9); lL->SetFillColor(0); lL->SetBorderSize(0);
   lL->SetTextSize(0.04);

   if(doIterClosure) {
     lL->AddEntry(lG0,"iter0","lf");
     lL->AddEntry(lG1,"iter1","lf");
   }

   if(doMuonCheck) {
     lL->AddEntry(lG0,"genZpt","lf");
     lL->AddEntry(lG1,"recoZpt","lf");
     lL->AddEntry(lG2,"MC ZptGen/ZrecoCorr","lf");
   }

   if(doSingleDouble) {
     lL->AddEntry(lG0,"iter0","lf");
     lL->AddEntry(lG1,"iter1 (di-gauss)","lf");
     if(howManyToPlot>2) lL->AddEntry(lG2,"iter1 (one-gauss)","lf");
   }

   if(doSinglePtStudy) {
     lL->AddEntry(lG0,"iter0","lf");
     lL->AddEntry(lG1,"iter1 u1-p0 fixed to 0","lf");
     if(howManyToPlot>2) lL->AddEntry(lG2,"iter1 u1-p0 free","lf");
     if(howManyToPlot>3) lL->AddEntry(lG3,"iter1 u1-p0 twice","lf");
   }

   if(doCorrelationA) {
     lL->AddEntry(lG0,"iter0","lf");
     lL->AddEntry(lG1,"iter1 : only U1 ","lf");
     if(howManyToPlot>2) lL->AddEntry(lG2,"iter1 : only U2","lf");
   }

   if(doCorrelationB) {
     lL->AddEntry(lG0,"iter0","lf");
     lL->AddEntry(lG1,"iter1 : radial smearing","lf");
     if(howManyToPlot>2) lL->AddEntry(lG2,"iter1 : u1/u2 smearing","lf");
   }

   if(doRapStudies) {
     lL->AddEntry(lG0,"|y(V)|<1","lf"); 
     lL->AddEntry(lG1,"1<|y(V)|<1.25","lf"); 
     lL->AddEntry(lG2,"1.25<|y(V)|<1.50","lf"); 
     lL->AddEntry(lG3,"1.50<|y(V)|<1.75","lf"); 
     lL->AddEntry(lG4,"1.75<|y(V)|<2","lf"); 
     //    lL->AddEntry(lG5,"|y(V)|>2","lf"); 
   }

   if(doPDF) {
     // 0 (gluon +X ) W/Z
     if(mycase==0) {
       lL->AddEntry(lG0,"W^{+}/Z","lf");
       if(howManyToPlot>1) lL->AddEntry(lG1,"W^{-}/Z","lf");
     }
     // 1 (main valence) W/Z
     if(mycase==1) {
       lL->AddEntry(lG0,"W^{+}/Z","lf");
       if(howManyToPlot>1) lL->AddEntry(lG1,"W^{-}/Z","lf");
     }
     // 2 (main valence) Z
     if(mycase==2) {
       lL->AddEntry(lG0,"ddar/uubar","lf");
       if(howManyToPlot>1) lL->AddEntry(lG1,"ssbar/uubar","lf");
       if(howManyToPlot>2) lL->AddEntry(lG2,"ccbar/uubar","lf");
     }
     if(mycase==3) {
       lL->AddEntry(lG0,"u-sbar/udbar","lf");
       if(howManyToPlot>1) lL->AddEntry(lG1,"cdbar/udbar","lf");
     }
     if(mycase==4) {
       lL->AddEntry(lG0,"u-sbar/udbar","lf");
       if(howManyToPlot>1) lL->AddEntry(lG1,"cdbar/udbar","lf");
     }

   }


   //  if(howManyToPlot>2) lL->AddEntry(lG2,"(DY+TOP)/DY |MZ|<91-10 1<|yZ|<2","lf");
   //  if(howManyToPlot>3) lL->AddEntry(lG3,"(DY+TOP)/DY |MZ|<91-10 1<|yZ|<2","lf");

   TLine   *lLine = new TLine(1.,1.,25.,1.);
   TCanvas *lC0 = new TCanvas(iName.c_str(),iName.c_str(),800,600); lC0->cd();
   lG0->Draw("alf");
   if(howManyToPlot>1) lG1->Draw("lf"); 
   if(howManyToPlot>2) lG2->Draw("lf");
   if(howManyToPlot>3) lG3->Draw("lf");
   if(doRapStudies) {
     lG0->Draw("alf");
     //    lG5->Draw("lf");
     lG4->Draw("lf");
     lG3->Draw("lf");
     lG2->Draw("lf");
     lG1->Draw("lf");
     lG0->Draw("lf");
   }
   lL->Draw();
   lLine->Draw();

   /*
   lL->AddEntry(lG0,"(DY+TOP)/DY |MZ|<91-20 |yZ|<1","lf");
   lL->AddEntry(lG1,"(DY+TOP)/DY |MZ|<91-10 |yZ|<1","lf");
   */

   /*
   lL->AddEntry(lG0,"PFnoPU DATA/MC","lf");
   lL->AddEntry(lG1,"PF DATA/MC","lf");
   */

   /*
   lL->AddEntry(lG0,"runA","lf");
   lL->AddEntry(lG1,"runB","lf");
   */

   //  lL->AddEntry(lG0,"old DATA/MC","lf");
   //  lL->AddEntry(lG1,"new DATA/MC","lf");

   /*
   lL->AddEntry(lG0,"W^{+}/W^{-} (1<|y(V)|<1.25)","lf");
   lL->AddEntry(lG1,"W^{+}/W^{-} (1.25<|y(V)|<1.5)","lf");
   lL->AddEntry(lG2,"W^{+}/W^{-} (1.5<|y(V)|<1.75)","lf");
   lL->AddEntry(lG3,"W^{+}/W^{-} (1.75<|y(V)|<2.0)","lf");
   */
   /*
   lL->AddEntry(lG0,"W^{-}/W^{+} (1<|y(V)|<1.25)","lf");
   lL->AddEntry(lG1,"W^{-}/W^{+} (1.25<|y(V)|<1.5)","lf");
   lL->AddEntry(lG2,"W^{-}/W^{+} (1.5<|y(V)|<1.75)","lf");
   lL->AddEntry(lG3,"W^{-}/W^{+} (1.75<|y(V)|<2.0)","lf");
   */

   /*
   lL->AddEntry(lG0,"(DY+TOP)/DY |MZ|<91-20 |yZ|<1","lf");
   lL->AddEntry(lG1,"(DY+TOP)/DY |MZ|<91-10 |yZ|<1","lf");
   lL->AddEntry(lG2,"(DY+TOP)/DY |MZ|<91-10 1<|yZ|<2","lf");
   */

   //  lL->AddEntry(lG0,"DATA/MC Z (1<|y(V)|<1.25)","lf");
   //  lL->AddEntry(lG1,"DATA/MC Z (1.25<|y(V)|<1.5)","lf");
   //  lL->AddEntry(lG2,"DATA/MC Z (1.5<|y(V)|<1.75)","lf");
   //  lL->AddEntry(lG3,"DATA/MC Z (1.75<|y(V)|<2.0)","lf");

   //  lL->AddEntry(lG0,"W^{-}/W^{+} (|y(V)|<1)","lf");
   //  lL->AddEntry(lG0,"W^{-}/W^{+} (1<|y(V)|<2)","lf");
   //  lL->AddEntry(lG1,"W^{-}/W^{+} (1<|y(V)|<2)","lf");

   /*
   lL->AddEntry(lG0,"W^{-}/Z (|y(V)|<1)","lf");
   lL->AddEntry(lG1,"W^{+}/Z (|y(V)|<1)","lf");
   //  lL->AddEntry(lG2,"W^{-}/Z (1<|y(V)|<1.5)","lf");
   //  lL->AddEntry(lG3,"W^{+}/Z (1<|y(V)|<1.5)","lf");
   lL->AddEntry(lG2,"W^{-}/Z (1<|y(V)|<2)","lf");
   lL->AddEntry(lG3,"W^{+}/Z (1<|y(V)|<2)","lf");
   */

   /*
   lL->AddEntry(lG0,"smear u ","lf");
   lL->AddEntry(lG1,"smear u1+u2","lf");
   //  lL->AddEntry(lG1,"iter1","lf");
   */

   /*
   lL->AddEntry(lG0,"iter0","lf");
   //  lL->AddEntry(lG0,"iter2","lf");
   lL->AddEntry(lG1,"iter1 (one gauss)","lf");
   lL->AddEntry(lG2,"iter1 (di gauss)","lf");
   //  lL->AddEntry(lG1,"iter1(U2 applied)","lf");
   */

   /*
   //  lL->AddEntry(lG0,"ccbar/uubar","lf");
   //  lL->AddEntry(lG1,"bbbar/uubar","lf");

   lL->AddEntry(lG0,"csbar/udbar","lf");
   lL->AddEntry(lG1,"gluon+X/udbar","lf");
   */

   /*  
   lL->AddEntry(lG0,"rap2/rap1","lf");
   lL->AddEntry(lG1,"rap3/rap1","lf");
   */

   //  lG0->GetYaxis()->SetRangeUser(0.8,1.2);

   TString leg10 = "";

   leg10 = "powheg";
   if(doMad) leg10 = "madgraph";

   TString leg1 = "";
   if(!doRapStudies) {
     if(vtxBin==1) leg1="|Y(V)| < 1";
     if(vtxBin==125) leg1="1 < |Y(V)| < 1.25";
     if(vtxBin==150) leg1="1.25 < |Y(V)| < 1.50";
     if(vtxBin==175) leg1="1.50 < |Y(V)| < 1.75";
     if(vtxBin==200) leg1="1.75 < |Y(V)| < 2";
     if(vtxBin==201) leg1="|Y(V)| > 2";
   } else {
     if(mycase==1) leg1="W^{+}/W^{-}";
     if(mycase==2) leg1="W^{+}/Z";
     if(mycase==3) leg1="W^{-}/Z";

     if(doPol2) leg1+= " pol2 ";

   }

   TString leg5 = "";
   if(doPDF) {
     if(mycase==0) leg5="gluon+X";
     if(mycase==1) leg5="Z(u-ubar)/W^{+}(u-dbar)/W^{-}(d-ubar)";
     if(mycase==2) leg5="Z";
     if(mycase==3) leg5="W^{+}";
     if(mycase==4) leg5="W^{-}";
   }

   TString leg2 = "|#eta(#mu)| < 2.1";
   //  TString leg2a = "|#eta(V)| < 1";
   //  TString leg2b = "|MZ| < 91-20";

   TString leg3 = "tkMET";
   //  TString leg3 = "pfMET";
   //  TString leg4 = "type-2 one-Gauss";
   //  TString leg4 = "type-2";
   TString leg4 = "type-2";
   //  TString leg4 = "type-2 di-Gauss absoluteRMS";
   //  if(!doSingleDouble && !doSinglePtStudy && !doCorrelation) leg4 += " di-Gauss";
   //  if(doSinglePtStudy || doCorrelation) leg4 += " one-Gauss";
   if(!doSingleDouble && !doSinglePtStudy ) leg4 += " di-Gauss";
   if(doSinglePtStudy) leg4 += " one-Gauss";


   TString text2 = "CLOSURE/RapBin_";
   if(doMad) text2 +=  "madgraph_";
   if(!doMad) text2 +=  "powheg_";
   if(doIterClosure) text2 +=  "diGauss_";
   if(doMuonCheck) text2 +=  "recoMuonCheck_";
   if(doSingleDouble) text2 +=  "diOnegauss_";
   if(doCorrelation) text2 +=  "Correlation_";
   if(doCorrelationA) text2 +=  "spot_";
   if(doCorrelationB) text2 +=  "solve_";
   if(doRapStudies) text2 +=  "WZ_";
   if(doPol2) text2 +=  "pol2_";
   if(doPDF) text2 +=  "PDF_";
   if(!doRapStudies) text2 += vtxBin;
   if(doRapStudies || doPDF) text2 += mycase;
   text2 += "_";
   text2 += iName;
   text2 += ".png";

   TString text3 = "CLOSURE/RapBin_";
   if(doMad) text3 +=  "madgraph_";
   if(!doMad) text3 +=  "powheg_";
   if(doIterClosure) text3 +=  "diGauss_";
   if(doMuonCheck) text3 +=  "recoMuonCheck_";
   if(doSingleDouble) text3 +=  "diOnegauss_";
   if(doCorrelation) text3 +=  "Correlation_";
   if(doCorrelationA) text3 +=  "spot_";
   if(doCorrelationB) text3 +=  "solve_";
   if(doRapStudies) text3 +=  "WZ_";
   if(doPol2) text3 +=  "pol2_";
   if(doPDF) text3 +=  "PDF_";
   if(!doRapStudies) text3 += vtxBin;
   if(doRapStudies || doPDF) text3 += mycase;
   text3 += "_";
   text3 += iName;
   text3 += ".pdf";

   TLatex latexLabel;
   latexLabel.SetTextSize(0.05);
   latexLabel.SetNDC();
   latexLabel.DrawLatex(0.2, 0.8, leg10);
   latexLabel.DrawLatex(0.2, 0.65, leg1);
   if(doPDF) latexLabel.DrawLatex(0.2, 0.6, leg5);

   //  latexLabel.DrawLatex(0.7, 0.15, leg2a);
   //  latexLabel.DrawLatex(0.7, 0.1, leg2b);
   latexLabel.DrawLatex(0.6, 0.2, leg2);
   latexLabel.DrawLatex(0.6, 0.25, leg3);
   latexLabel.DrawLatex(0.2, 0.7, leg4);

   lC0->SaveAs(text2.Data());
   lC0->SaveAs(text3.Data());

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

   ////
   ////
   ////
   ////

   std::string iNameZDat = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_DATA_pfnoPU_type2.root";
   std::string iNameZMC  = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genZ_pfnoPU_type2.root";
   std::string iNameW = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genWpos_pfnoPU_type2.root";
   std::string iNameWC  = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_genWneg_pfnoPU_type2_TEST.root";

   ////
   ////
   ////
   ////

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

   draw("u1" ,"U_{1} Response "           , -0.2,1.2,0.5 ,lWU1Fit,     lZDU1Fit,     lZMU1Fit,     lWCU1Fit);
   draw("u1MR" ,"U_{1} Mean Resolution (GeV)"  ,   0, 20,0.05,lWU1RMSSMFit,lZDU1RMSSMFit,lZMU1RMSSMFit,lWCU1RMSSMFit);
   draw("u1R1" ,"U_{1} Resolution 1 (GeV)"  ,     0,11,0.05  ,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit ,lWCU1RMS1Fit);
   draw("u1R2" ,"U_{1} Resolution 2 (GeV)"  ,     0,21,0.05  ,lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit ,lWCU1RMS2Fit);

   //  draw("u1" ,"U_{1} Response (GeV)"           , 0.,0.8,0.2 ,lWU1Fit,     lZDU1Fit,     lZMU1Fit,     lWCU1Fit);
   //  draw("u1R1" ,"U_{1} Resolution 1 (GeV)"  ,     0,5.0,0.05  ,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit ,lWCU1RMS1Fit);
   //  draw("u1R2" ,"U_{1} Resolution 2 (GeV)"  ,     0,5.0,0.05  ,lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit ,lWCU1RMS2Fit);

   //  cout << "DDRAWING FRACTION" << endl;
   //draw("u13R" ,"U_{1} > 3 #sigma (GeV)"    ,     0,11,0.05,lWU13SigFit ,lZDU13SigFit ,lZMU13SigFit );

   /*
   drawFrac("u1Frac" ,"U_{1} Frac #sigma_{1}" ,     0,2,0.05,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit,
	    lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit );
   */

   draw("u2" ,"U_{2} Response "         , -0.5,0.5,0.1 ,lWU2Fit     ,lZDU2Fit     ,lZMU2Fit,lWCU2Fit);
   draw("u2MR" ,"U_{2} Mean Resolution (GeV)"  ,  0,15,0.05,lWU2RMSSMFit,lZDU2RMSSMFit,lZMU2RMSSMFit,lWCU2RMSSMFit);
   draw("u2R1" ,"U_{2} Resolution 1 (GeV)"  ,     0,11,0.05,lWU2RMS1Fit ,lZDU2RMS1Fit ,lZMU2RMS1Fit ,lWCU2RMS1Fit);
   draw("u2R2" ,"U_{2} Resolution 2 (GeV)"  ,     0,21,0.05,lWU2RMS2Fit ,lZDU2RMS2Fit ,lZMU2RMS2Fit ,lWCU2RMS2Fit);

   //  draw("u2R1" ,"U_{2} Resolution 1 (GeV)"  ,     0,5.0,0.05,lWU2RMS1Fit ,lZDU2RMS1Fit ,lZMU2RMS1Fit ,lWCU2RMS1Fit);
   //  draw("u2R2" ,"U_{2} Resolution 2 (GeV)"  ,     0,5.0,0.05,lWU2RMS2Fit ,lZDU2RMS2Fit ,lZMU2RMS2Fit ,lWCU2RMS2Fit);
   /*
   //  drawFrac("u2Frac" ,"U_{2} Frac #sigma_{2}" ,     0,2,0.05,lWU2RMS1Fit ,lZDU2RMS1Fit ,lZMU2RMS1Fit,
   drawFrac("u1Frac" ,"U_{1} Frac #sigma_{2}" ,     0,2,0.05,lWU1RMS1Fit ,lZDU1RMS1Fit ,lZMU1RMS1Fit,
	    lWU1RMS2Fit ,lZDU1RMS2Fit ,lZMU1RMS2Fit );
   */
   ////  //draw("u23R" ,"U_{2} > 3 #sigma (GeV)"    ,     0,11,0.05,lWU23SigFit ,lZDU23SigFit ,lZMU23SigFit );
 }

 /*
 void plotRatiosComp(std::string iNameZMC ="recoilfits/recoilfit_zmm52X_pf_v3.root",     std::string iNameZDat ="recoilfits/recoilfit_datamm52X_pf_v3.root",
		     std::string iNameZMC1="recoilfits/recoilfit_zmm52X_mva_v3.root"   ,    std::string iNameZDat1="recoilfits/recoilfit_datamm52X_mva_v3.root") { 
 */
   //  Prep();
 void plotRatiosComp(int myVtxBin) {

   vtxBin=myVtxBin;
   howManyToPlot=0;

   //  std::string iDir = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_MAY14";
   //   std::string iDir = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_MAY23";
   //   std::string iDir = "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_MAY31";
   std::string iDir = "/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_19_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_AUG6";
   std::string iDir_bis = "/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_19_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_AUG6";

   std::string iNameZDat = "";
   std::string iNameZMC = "";
   std::string iNameZDat1 = "";
   std::string iNameZMC1 = "";
   std::string iNameZDat2 = "";
   std::string iNameZMC2 = "";
   std::string iNameZDat3 = "";
   std::string iNameZMC3 = "";

   if(doIterClosure) {

       iNameZDat.append(iDir);  iNameZDat.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat"); 
       iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat");
       
       iNameZDat1.append(iDir);  iNameZDat1.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat"); 
       iNameZMC1.append(iDir_bis);   iNameZMC1.append("_genZ_ITERATIVE_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat");

     if(doMad) {
       iNameZDat.append("_53X.root"); 
       iNameZMC.append("_53X_madgraph.root"); 
       iNameZDat1.append("_53X.root"); 
       iNameZMC1.append("_53X_madgraph.root"); 
     } else {
       iNameZDat.append("_53X.root"); 
       iNameZMC.append("_53X_powheg.root"); 
       iNameZDat1.append("_53X.root"); 
       iNameZMC1.append("_53X_powheg.root"); 
     }

     howManyToPlot=2;

  }

  if(doMuonCheck) {

    //////

    iNameZDat.append(iDir);  iNameZDat.append("_MAY22__DATA_tkmet_eta21_MZ81101_pol3_type1_doubleGauss_x2Stat_ROCHcorr.root");
    iNameZMC.append(iDir);   iNameZMC.append("_MAY22__genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type1_doubleGauss_x2Stat_madgraph.root");

    iNameZDat1.append(iDir);  iNameZDat1.append("_MAY22__DATA_tkmet_eta21_MZ81101_pol3_type1_doubleGauss_x2Stat_ROCHcorr.root");
    iNameZMC1.append(iDir);   iNameZMC1.append("_MAY22__genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type1_doubleGauss_x2Stat_ROCHcorr_madgraph.root");

    //////
    iNameZDat2.append(iDir);  iNameZDat2.append("_MAY22__genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type1_doubleGauss_x2Stat_ROCHcorr_madgraph.root");
    iNameZMC2.append(iDir);   iNameZMC2.append("_MAY22__genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type1_doubleGauss_x2Stat_madgraph.root");

    howManyToPlot=3;

  }

  if(doSinglePtStudy) {


    iNameZDat =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss3_DATA_tkmet_eta21_MZ81101_rapBin1_pol2_type2_oneGauss_x2Stat.root" ;
    iNameZMC  =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss3_genZ_tkmet_eta21_MZ81101_PDF-1_rapBin1_pol2_type2_oneGauss_x2Stat_madgraph.root" ; 
    //////
    iNameZDat1 =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss3_DATA_tkmet_eta21_MZ81101_rapBin1_pol2_type2_oneGauss_x2Stat.root";
    iNameZMC1  =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss3_genZ_tkmet_eta21_MZ81101_PDF-1_rapBin1_pol2_type2_oneGauss_x2Stat_ITERATIVE_madgraph.root";

    iNameZDat2 =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss4_DATA_tkmet_eta21_MZ81101_rapBin1_pol2_type2_oneGauss_x2Stat.root";
    iNameZMC2  =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss4_genZ_tkmet_eta21_MZ81101_PDF-1_rapBin1_pol2_type2_oneGauss_x2Stat_ITERATIVE_madgraph.root";

    iNameZDat3 =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss9_DATA_tkmet_eta21_MZ81101_rapBin1_pol2_type2_oneGauss_x2Stat.root";
    iNameZMC3  =  "/afs/cern.ch/user/d/dalfonso/scratch0/CMSSW_4_4_5_Wmass/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode/recoilfits/recoilfit_TESTDiGauss9_genZ_tkmet_eta21_MZ81101_PDF-1_rapBin1_pol2_type2_oneGauss_x2Stat_ITERATIVE_madgraph.root";

    howManyToPlot=4;

  }

  if(doSingleDouble) {

    iNameZDat.append(iDir);  iNameZDat.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_ROCHcorr_53X.root"); 
    iNameZDat1.append(iDir);  iNameZDat1.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_ROCHcorr_53X.root"); 
    iNameZDat2.append(iDir);  iNameZDat2.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_ROCHcorr_53X.root"); 


    iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ROCHcorr_53X_madgraph.root");
    iNameZMC1.append(iDir_bis);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ITERATIVE_ROCHcorr_53X_madgraph.root");
    iNameZMC2.append(iDir_bis);   iNameZMC2.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_oneGauss_x2Stat_noCorrelation_ITERATIVE_ROCHcorr_53X_madgraph.root");

    howManyToPlot=3;

  }

  if(doCorrelation) {
    
    iNameZDat.append(iDir);  iNameZDat.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_ROCHcorr_53X.root"); 
    iNameZDat1.append(iDir);  iNameZDat1.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_ROCHcorr_53X.root");
    iNameZDat2.append(iDir);  iNameZDat2.append("_DATA_tkmet_eta21_MZ81101_pol3_type2_doubleGauss_x2Stat_ROCHcorr_53X.root"); 

    //    iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ROCHcorr_53X_madgraph.root");
    iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ROCHcorr_53X_madgraph.root");

    if(doCorrelationB) {

      //     iNameZMC1.append(iDir);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_ITERATIVE_ROCHcorr_53X_madgraph.root");
      //      iNameZMC2.append(iDir);   iNameZMC2.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_ITERATIVE_ROCHcorr_53X_madgraph.root");
      //      iNameZMC2.append(iDir);   iNameZMC2.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ITERATIVE_ROCHcorr_53X_madgraph.root");
      //
      iNameZMC1.append(iDir_bis);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_ITERATIVE_ROCHcorr_53X_madgraph.root");
      //      iNameZMC1.append(iDir);   iNameZMC1.append("_recoPT_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ITERATIVE_ROCHcorr_53X_madgraph.root");
      iNameZMC2.append(iDir_bis);   iNameZMC2.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ITERATIVE_ROCHcorr_53X_madgraph.root");

    }

    if(doCorrelationA) {

      iNameZMC1.append(iDir_bis);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ITERATIVE_applyOnlyU1_ROCHcorr_53X_madgraph.root");
      iNameZMC2.append(iDir_bis);   iNameZMC2.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ITERATIVE_applyOnlyU2_ROCHcorr_53X_madgraph.root");

    }
    
    howManyToPlot=3;
    
  }

   cout << " doIterClosure " << doIterClosure << endl;
   cout << " doMuonCheck " << doMuonCheck << endl;
   cout << " doSinglePtStudy " << doSinglePtStudy << endl;
   cout << " doSingleDouble " << doSingleDouble << endl;
   cout << " doCorrelation " << doCorrelation << endl;
   cout << " doRapStudies " << doRapStudies << endl;
   cout << " doPDF " << doPDF << endl;

  if(doRapStudies) {

    howManyToPlot=1;

    if(mycase==1) {
      //  Wpos / Wneg
      if(doPol2) {
	iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF-1_pol2_type2_doubleGauss_x2Stat");
	iNameZMC.append(iDir);   iNameZMC.append("_Wneg_tkmet_eta21_PDF-1_pol2_type2_doubleGauss_x2Stat"); 
      } else {
        iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF-1_pol3_type2_doubleGauss_x2Stat");
        iNameZMC.append(iDir);   iNameZMC.append("_Wneg_tkmet_eta21_PDF-1_pol3_type2_doubleGauss_x2Stat"); 
      }

    }

    if(mycase==2) {
      //  Wpos / Z
      if(doPol2) {
	iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF-1_pol2_type2_doubleGauss_x2Stat");
	iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol2_type2_doubleGauss_x2Stat"); 
      } else {
	iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF-1_pol3_type2_doubleGauss_x2Stat");
	iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat"); 
      }

    }

    if(mycase==3) {
      //  Wneg / Z
      if(doPol2) {
	iNameZDat.append(iDir);  iNameZDat.append("_Wneg_tkmet_eta21_PDF-1_pol2_type2_doubleGauss_x2Stat");
	iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol2_type2_doubleGauss_x2Stat"); 
      } else {
	iNameZDat.append(iDir);  iNameZDat.append("_Wneg_tkmet_eta21_PDF-1_pol3_type2_doubleGauss_x2Stat");
	iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF-1_pol3_type2_doubleGauss_x2Stat"); 
      }
    }


    //    std::string MCtypeMad = "_ROCHcorr_53X_madgraph.root";
    //    std::string MCtypePow = "_ROCHcorr_53X_powheg.root";

    std::string MCtypeMad = "_53X_madgraph.root";
    std::string MCtypePow = "_53X_powheg.root";


     if(doMad) {
       iNameZDat.append(MCtypeMad); 
       iNameZMC.append(MCtypeMad); 
       iNameZDat1.append(MCtypeMad); 
       iNameZMC1.append(MCtypeMad); 
     } else {
       iNameZDat.append(MCtypePow); 
       iNameZMC.append(MCtypePow); 
       iNameZDat1.append(MCtypePow); 
       iNameZMC1.append(MCtypePow); 
     }


  }

  if(doPDF) {
    
    // 0 (gluon +X ) W/Z
    // 1 (main valence) W/Z
    // 2 (main valence) Z

    /*
    howManyToPlot=1;
    
    //      iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF0_pol3_type2_doubleGauss_x2Stat_53X_madgraph.root");
    iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF0_pol3_type2_doubleGauss_x2Stat_noCorrelation_ROCHcorr_53X_madgraph.root");
    iNameZDat.append(iDir);   iNameZDat.append("_genZ_tkmet_eta21_MZ81101_PDF1_pol3_type2_doubleGauss_x2Stat_noCorrelation_ROCHcorr_53X_madgraph.root");
    
    iNameZDat1.append(iDir);  iNameZDat1.append("_Wneg_tkmet_eta21_PDF0_pol3_type2_doubleGauss_x2Stat_53X_madgraph.root");
    iNameZMC1.append(iDir);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF0_pol3_type2_doubleGauss_x2Stat_noCorrelation_ROCHcorr_53X_madgraph.root");
    */

    if(mycase==0) {

      howManyToPlot=2;

      iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF0_pol3_type2_doubleGauss_x2Stat");
      iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF0_pol3_type2_doubleGauss_x2Stat");

      iNameZDat1.append(iDir);  iNameZDat1.append("_Wneg_tkmet_eta21_PDF0_pol3_type2_doubleGauss_x2Stat");
      iNameZMC1.append(iDir);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF0_pol3_type2_doubleGauss_x2Stat");

    }

    if(mycase==1) {

      howManyToPlot=2;

      iNameZDat.append(iDir);  iNameZDat.append("_Wpos_tkmet_eta21_PDF1_pol3_type2_doubleGauss_x2Stat");
      iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF2_pol3_type2_doubleGauss_x2Stat");

      iNameZDat1.append(iDir);  iNameZDat1.append("_Wneg_tkmet_eta21_PDF1_pol3_type2_doubleGauss_x2Stat");
      iNameZMC1.append(iDir);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF2_pol3_type2_doubleGauss_x2Stat");


    }

    if(mycase==2) {

      howManyToPlot=3;

      iNameZDat.append(iDir);   iNameZDat.append("_genZ_tkmet_eta21_MZ81101_PDF1_pol3_type2_doubleGauss_x2Stat");
      iNameZMC.append(iDir);   iNameZMC.append("_genZ_tkmet_eta21_MZ81101_PDF2_pol3_type2_doubleGauss_x2Stat");

      iNameZDat1.append(iDir);   iNameZDat1.append("_genZ_tkmet_eta21_MZ81101_PDF3_pol3_type2_doubleGauss_x2Stat");
      iNameZMC1.append(iDir);   iNameZMC1.append("_genZ_tkmet_eta21_MZ81101_PDF2_pol3_type2_doubleGauss_x2Stat");

      iNameZDat2.append(iDir);   iNameZDat2.append("_genZ_tkmet_eta21_MZ81101_PDF4_pol3_type2_doubleGauss_x2Stat");
      iNameZMC2.append(iDir);   iNameZMC2.append("_genZ_tkmet_eta21_MZ81101_PDF2_pol3_type2_doubleGauss_x2Stat");

    }

    if(mycase==3) {

      iNameZDat.append(iDir);   iNameZDat.append("_Wpos_tkmet_eta21_PDF2_pol3_type2_doubleGauss_x2Stat");
      iNameZMC.append(iDir);   iNameZMC.append("_Wpos_tkmet_eta21_PDF1_pol3_type2_doubleGauss_x2Stat");

      howManyToPlot=1;

    }

    if(mycase==4) {

      iNameZDat.append(iDir);   iNameZDat.append("_Wneg_tkmet_eta21_PDF2_pol3_type2_doubleGauss_x2Stat");
      iNameZMC.append(iDir);   iNameZMC.append("_Wneg_tkmet_eta21_PDF1_pol3_type2_doubleGauss_x2Stat");

      howManyToPlot=1;

    }

    std::string MCtypeMad = "_53X_madgraph.root";
    std::string MCtypePow = "_53X_powheg.root";


     if(doMad) {
       iNameZDat.append(MCtypeMad); 
       iNameZMC.append(MCtypeMad); 
       iNameZDat1.append(MCtypeMad); 
       iNameZMC1.append(MCtypeMad); 
       iNameZDat2.append(MCtypeMad); 
       iNameZMC2.append(MCtypeMad); 
     } else {
       iNameZDat.append(MCtypePow); 
       iNameZMC.append(MCtypePow); 
       iNameZDat1.append(MCtypePow); 
       iNameZMC1.append(MCtypePow); 
       iNameZDat2.append(MCtypePow); 
       iNameZMC2.append(MCtypePow); 
     }
    
  }


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

  vector<double> lZD2SumEt; vector<double> lZD2Weights; 
  vector<TF1*>   lZD2U1Fit; vector<TF1*> lZD2U1RMSSMFit; vector<TF1*> lZD2U1RMS1Fit; vector<TF1*> lZD2U1RMS2Fit; vector<TF1*> lZD2U13SigFit; 
  vector<TF1*>   lZD2U2Fit; vector<TF1*> lZD2U2RMSSMFit; vector<TF1*> lZD2U2RMS1Fit; vector<TF1*> lZD2U2RMS2Fit; vector<TF1*> lZD2U23SigFit;  
  vector<double> lZM2SumEt; vector<double> lZM2Weights; 
  vector<TF1*>   lZM2U1Fit; vector<TF1*> lZM2U1RMSSMFit; vector<TF1*> lZM2U1RMS1Fit; vector<TF1*> lZM2U1RMS2Fit; vector<TF1*> lZM2U13SigFit; 
  vector<TF1*>   lZM2U2Fit; vector<TF1*> lZM2U2RMSSMFit; vector<TF1*> lZM2U2RMS1Fit; vector<TF1*> lZM2U2RMS2Fit; vector<TF1*> lZM2U23SigFit; 

  vector<double> lZD3SumEt; vector<double> lZD3Weights; 
  vector<TF1*>   lZD3U1Fit; vector<TF1*> lZD3U1RMSSMFit; vector<TF1*> lZD3U1RMS1Fit; vector<TF1*> lZD3U1RMS2Fit; vector<TF1*> lZD3U13SigFit; 
  vector<TF1*>   lZD3U2Fit; vector<TF1*> lZD3U2RMSSMFit; vector<TF1*> lZD3U2RMS1Fit; vector<TF1*> lZD3U2RMS2Fit; vector<TF1*> lZD3U23SigFit;  
  vector<double> lZM3SumEt; vector<double> lZM3Weights; 
  vector<TF1*>   lZM3U1Fit; vector<TF1*> lZM3U1RMSSMFit; vector<TF1*> lZM3U1RMS1Fit; vector<TF1*> lZM3U1RMS2Fit; vector<TF1*> lZM3U13SigFit; 
  vector<TF1*>   lZM3U2Fit; vector<TF1*> lZM3U2RMSSMFit; vector<TF1*> lZM3U2RMS1Fit; vector<TF1*> lZM3U2RMS2Fit; vector<TF1*> lZM3U23SigFit; 
 
  readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU13SigFit,lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU23SigFit,iNameZDat,"PF");
  readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,iNameZMC,"PF");

  if(howManyToPlot>1) readRecoil(lZD1SumEt,lZD1U1Fit,lZD1U1RMSSMFit,lZD1U1RMS1Fit,lZD1U1RMS2Fit,lZD1U13SigFit,lZD1U2Fit,lZD1U2RMSSMFit,lZD1U2RMS1Fit,lZD1U2RMS2Fit,lZD1U23SigFit,iNameZDat1,"PF");
  if(howManyToPlot>1) readRecoil(lZM1SumEt,lZM1U1Fit,lZM1U1RMSSMFit,lZM1U1RMS1Fit,lZM1U1RMS2Fit,lZM1U13SigFit,lZM1U2Fit,lZM1U2RMSSMFit,lZM1U2RMS1Fit,lZM1U2RMS2Fit,lZM1U23SigFit,iNameZMC1,"PF");

  if(howManyToPlot>2) readRecoil(lZD2SumEt,lZD2U1Fit,lZD2U1RMSSMFit,lZD2U1RMS1Fit,lZD2U1RMS2Fit,lZD2U13SigFit,lZD2U2Fit,lZD2U2RMSSMFit,lZD2U2RMS1Fit,lZD2U2RMS2Fit,lZD2U23SigFit,iNameZDat2,"PF");
  if(howManyToPlot>2) readRecoil(lZM2SumEt,lZM2U1Fit,lZM2U1RMSSMFit,lZM2U1RMS1Fit,lZM2U1RMS2Fit,lZM2U13SigFit,lZM2U2Fit,lZM2U2RMSSMFit,lZM2U2RMS1Fit,lZM2U2RMS2Fit,lZM2U23SigFit,iNameZMC2,"PF");

  if(howManyToPlot>3) readRecoil(lZD3SumEt,lZD3U1Fit,lZD3U1RMSSMFit,lZD3U1RMS1Fit,lZD3U1RMS2Fit,lZD3U13SigFit,lZD3U2Fit,lZD3U2RMSSMFit,lZD3U2RMS1Fit,lZD3U2RMS2Fit,lZD3U23SigFit,iNameZDat3,"PF");
  if(howManyToPlot>3) readRecoil(lZM3SumEt,lZM3U1Fit,lZM3U1RMSSMFit,lZM3U1RMS1Fit,lZM3U1RMS2Fit,lZM3U13SigFit,lZM3U2Fit,lZM3U2RMSSMFit,lZM3U2RMS1Fit,lZM3U2RMS2Fit,lZM3U23SigFit,iNameZMC3,"PF");

  //correct(lZDU1RMSSMFit);   correct(lZMU1RMSSMFit);  correct(lZD1U1RMSSMFit);   correct(lZM1U1RMSSMFit);
  //correct(lZDU2RMSSMFit);   correct(lZMU2RMSSMFit);  correct(lZD1U2RMSSMFit);   correct(lZM1U2RMSSMFit);

  //correct(lZDU1RMS1Fit,lZDU1RMSSMFit); correct(lZMU1RMS1Fit,lZMU1RMSSMFit); correct(lZDU1RMS2Fit,lZD1U1RMSSMFit); correct(lZMU1RMS2Fit,lZM1U1RMSSMFit);
  //correct(lZDU2RMS1Fit,lZDU2RMSSMFit); correct(lZMU2RMS1Fit,lZMU2RMSSMFit); correct(lZDU2RMS2Fit,lZD1U2RMSSMFit); correct(lZMU2RMS2Fit,lZM1U2RMSSMFit);

  //correct(lZD1U1RMS1Fit,lZDU1RMSSMFit); correct(lZM1U1RMS1Fit,lZMU1RMSSMFit);  correct(lZD1U1RMS2Fit,lZD1U1RMSSMFit); correct(lZM1U1RMS2Fit,lZM1U1RMSSMFit);
  //correct(lZD1U2RMS1Fit,lZDU2RMSSMFit); correct(lZM1U2RMS1Fit,lZMU2RMSSMFit);  correct(lZD1U2RMS2Fit,lZD1U2RMSSMFit); correct(lZM1U2RMS2Fit,lZM1U2RMSSMFit);

  if(doIterClosure || doSingleDouble || doCorrelation || doSinglePtStudy || doMuonCheck) {
    // good for data/MC correction
    drawRatios("Ru1" ,"U_{1} Response DATA/MC"        ,   0.85, 1.2,0.01,lZDU1Fit     ,lZMU1Fit     ,/**/ lZD1U1Fit     ,lZM1U1Fit     ,/**/ lZD2U1Fit     ,lZM2U1Fit     ,/**/ lZD3U1Fit     ,lZM3U1Fit);

    drawRatios("Ru1MR" ,"U_{1} Mean Resolution DATA/MC"  , 0.9, 1.3,0.01,lZDU1RMSSMFit,lZMU1RMSSMFit,/**/ lZD1U1RMSSMFit,lZM1U1RMSSMFit,/**/ lZD2U1RMSSMFit,lZM2U1RMSSMFit,/**/ lZD3U1RMSSMFit,lZM3U1RMSSMFit);
    drawRatios("Ru2MR" ,"U_{2} Mean Resolution DATA/MC"  , 0.9, 1.3,0.01,lZDU2RMSSMFit,lZMU2RMSSMFit,/**/ lZD1U2RMSSMFit,lZM1U2RMSSMFit,/**/ lZD2U2RMSSMFit,lZM2U2RMSSMFit,/**/ lZD3U2RMSSMFit,lZM3U2RMSSMFit);

    if(doIterClosure) {

      //    cout << "not defined function Ru1R1 " << endl;
      drawRatios("Ru1R1" ,"U_{1} Resolution 1 "  ,      0.9,1.2,0.01,lZDU1RMS1Fit ,lZMU1RMS1Fit ,/**/ lZD1U1RMS1Fit ,lZM1U1RMS1Fit ,/**/ lZD2U1RMS1Fit ,lZM2U1RMS1Fit, /**/lZD3U1RMS1Fit ,lZM3U1RMS1Fit);
      //    cout << "not defined function Ru1R2 " << endl;
      drawRatios("Ru1R2" ,"U_{1} Resolution 2 "  ,      0.9,1.2,0.01,lZDU1RMS2Fit ,lZMU1RMS2Fit ,/**/ lZD1U1RMS2Fit ,lZM1U1RMS2Fit ,/**/ lZD2U1RMS2Fit ,lZM2U1RMS2Fit, /**/lZD3U1RMS2Fit ,lZM3U1RMS2Fit);
      //    cout << "not defined function Ru2R1 " << endl;
      drawRatios("Ru2R1" ,"U_{2} Resolution 1 "  ,     0.9,1.2,0.01,lZDU2RMS1Fit ,lZMU2RMS1Fit ,/**/ lZD1U2RMS1Fit ,lZM1U2RMS1Fit ,/**/ lZD2U2RMS1Fit ,lZM2U2RMS1Fit, /**/lZD3U2RMS1Fit ,lZM3U2RMS1Fit);
      //    cout << "not defined function Ru2R2 " << endl;
      drawRatios("Ru2R2" ,"U_{2} Resolution 2 "  ,     0.9,1.2,0.01,lZDU2RMS2Fit ,lZMU2RMS2Fit ,/**/ lZD1U2RMS2Fit ,lZM1U2RMS2Fit ,/**/ lZD2U2RMS2Fit ,lZM2U2RMS2Fit, /**/lZD3U2RMS2Fit ,lZM3U2RMS2Fit);

    }
  }

  if(doPDF) {
    drawRatios("Ru1" ,"U_{1} Response MC"        ,   0.75, 1.5,0.01,lZDU1Fit     ,lZMU1Fit     ,/**/ lZD1U1Fit     ,lZM1U1Fit     ,/**/ lZD2U1Fit     ,lZM2U1Fit     ,/**/ lZD3U1Fit     ,lZM3U1Fit);
    drawRatios("Ru1MR" ,"U_{1} Mean Resolution MC"  , 0.75, 1.5,0.01,lZDU1RMSSMFit,lZMU1RMSSMFit,/**/ lZD1U1RMSSMFit,lZM1U1RMSSMFit,/**/ lZD2U1RMSSMFit,lZM2U1RMSSMFit,/**/ lZD3U1RMSSMFit,lZM3U1RMSSMFit);
    drawRatios("Ru2MR" ,"U_{2} Mean Resolution MC"  , 0.75, 1.5,0.01,lZDU2RMSSMFit,lZMU2RMSSMFit,/**/ lZD1U2RMSSMFit,lZM1U2RMSSMFit,/**/ lZD2U2RMSSMFit,lZM2U2RMSSMFit,/**/ lZD3U2RMSSMFit,lZM3U2RMSSMFit);
  }

  if(doRapStudies) {
    drawRatios("Ru1" ,"U_{1} Response MC"        ,   0.9, 1.1,0.01,lZDU1Fit     ,lZMU1Fit     ,/**/ lZD1U1Fit     ,lZM1U1Fit     ,/**/ lZD2U1Fit     ,lZM2U1Fit     ,/**/ lZD3U1Fit     ,lZM3U1Fit);
    drawRatios("Ru1MR" ,"U_{1} Mean Resolution MC"  , 0.9, 1.2,0.01,lZDU1RMSSMFit,lZMU1RMSSMFit,/**/ lZD1U1RMSSMFit,lZM1U1RMSSMFit,/**/ lZD2U1RMSSMFit,lZM2U1RMSSMFit,/**/ lZD3U1RMSSMFit,lZM3U1RMSSMFit);
    drawRatios("Ru2MR" ,"U_{2} Mean Resolution MC"  , 0.9, 1.2,0.01,lZDU2RMSSMFit,lZMU2RMSSMFit,/**/ lZD1U2RMSSMFit,lZM1U2RMSSMFit,/**/ lZD2U2RMSSMFit,lZM2U2RMSSMFit,/**/ lZD3U2RMSSMFit,lZM3U2RMSSMFit);
  }

  /*
  // RANGE good for W and Z
  drawRatios("Ru1" ,"U_{1} Response"         ,     0.9,1.1,0.01,lZDU1Fit     ,lZMU1Fit     ,lZD1U1Fit     ,lZM1U1Fit , lZD2U1Fit     ,lZM2U1Fit, lZD3U1Fit     ,lZM3U1Fit);
  drawRatios("Ru1MR" ,"U_{1} Mean Resolution (GeV)"  ,   0.9,1.1,0.01,lZDU1RMSSMFit,lZMU1RMSSMFit,lZD1U1RMSSMFit,lZM1U1RMSSMFit, lZD2U1RMSSMFit, lZM2U1RMSSMFit , lZD3U1RMSSMFit, lZM3U1RMSSMFit);
  //  drawRatios("Ru2" ,"U_{2} Response"         ,    0.9,1.1,0.01,lZDU2Fit     ,lZMU2Fit     ,lZD1U2Fit     ,lZM1U2Fit);
  drawRatios("Ru2MR" ,"U_{2} Mean Resolution (GeV)"  ,  0.9,1.1,0.01,lZDU2RMSSMFit,lZMU2RMSSMFit,lZD1U2RMSSMFit,lZM1U2RMSSMFit, lZD2U2RMSSMFit, lZM2U2RMSSMFit, lZD3U2RMSSMFit, lZM3U2RMSSMFit);
  */

  //  drawRatios("Ru1" ,"U_{1} Response"         ,     0.9,1.1,0.01,lZDU1Fit     ,lZMU1Fit     ,lZD1U1Fit     ,lZM1U1Fit , lZD2U1Fit     ,lZM2U1Fit, lZD3U1Fit     ,lZM3U1Fit);
  //  drawRatios("Ru1" ,"U_{1} Response"         ,     0.9,1.15,0.01,lZDU1Fit     ,lZD1U1Fit     ,lZD2U1Fit     ,lZD3U1Fit , lZD2U1Fit     ,lZM2U1Fit, lZD3U1Fit     ,lZM3U1Fit);

  /*  W+/W-
  drawRatios("Ru1" ,"U_{1} Response"         ,     0.9, 1.15, 0.01,lZDU1Fit     ,lZD1U1Fit     ,lZD2U1Fit     ,lZD3U1Fit , lZD2U1Fit     ,lZM2U1Fit, lZD3U1Fit     ,lZM3U1Fit);
  drawRatios("Ru1MR" ,"U_{1} Mean Resolution (GeV)"  ,   0.9,1.1,0.01,lZDU1RMSSMFit,lZD1U1RMSSMFit,lZD2U1RMSSMFit,lZD3U1RMSSMFit, lZD2U1RMSSMFit, lZM2U1RMSSMFit , lZD3U1RMSSMFit, lZM3U1RMSSMFit);
  */
  /*
D->M
D1->M1

D->D1
D2->D3
  */



  /*
  // good for data/MC correction
  drawRatios("Ru1" ,"U_{1} Response "         ,     -0.5, 2.5, 0.01, lZDU1Fit     ,lZMU1Fit     ,lZD1U1Fit     ,lZM1U1Fit);
  drawRatios("Ru1MR" ,"U_{1} Mean Resolution "  ,   0.5, 1.5, 0.01, lZDU1RMSSMFit,lZMU1RMSSMFit,lZD1U1RMSSMFit,lZM1U1RMSSMFit);
  //  drawRatios("Ru2" ,"U_{2} Response DATA/MC"         ,    0.8,1.2,0.01,lZDU2Fit     ,lZMU2Fit     ,lZD1U2Fit     ,lZM1U2Fit);
  drawRatios("Ru2MR" ,"U_{2} Mean Resolution "  ,  0.5, 1.5, 0.01, lZDU2RMSSMFit,lZMU2RMSSMFit,lZD1U2RMSSMFit,lZM1U2RMSSMFit);
  */

/*
  cout << "making ratio of RMS large and small  " << endl;
  drawRatios("Ru1R1" ,"U_{1} Resolution 1 "  ,      0,2,0.01,lZDU1RMS1Fit ,lZMU1RMS1Fit ,lZD1U1RMS1Fit ,lZM1U1RMS1Fit );
  drawRatios("Ru1R2" ,"U_{1} Resolution 2 "  ,      0,2,0.01,lZDU1RMS2Fit ,lZMU1RMS2Fit ,lZD1U1RMS2Fit ,lZM1U1RMS2Fit );
  //drawRatios("Ru13R" ,"U_{1} > 3 #sigma (GeV)"    ,      0,2,0.01,lZDU13SigFit ,lZMU13SigFit ,lZD1U13SigFit ,lZM1U13SigFit );

  drawRatios("Ru2R1" ,"U_{2} Resolution 1 "  ,     0,2,0.01,lZDU2RMS1Fit ,lZMU2RMS1Fit ,lZD1U2RMS1Fit ,lZM1U2RMS1Fit );
  drawRatios("Ru2R2" ,"U_{2} Resolution 2 "  ,     0,2,0.01,lZDU2RMS2Fit ,lZMU2RMS2Fit ,lZD1U2RMS2Fit ,lZM1U2RMS2Fit );
  //drawRatios("Ru23R" ,"U_{2} > 3 #sigma (GeV)"    ,     0,2,0.01,lZDU23SigFit ,lZMU23SigFit ,lZDU23SigFit  ,lZM1U23SigFit );
  */

}

void plotRatios(int myVtxBin, bool doComp, bool myDoIter, bool doRapStudy, bool doPDFStudy, bool doCorrelationStudy, bool doCorrelationAStudy, bool doCorrelationBStudy, int Case=-1) {

  doCorrelation = doCorrelationStudy;
  doCorrelationA = doCorrelationAStudy;
  doCorrelationB = doCorrelationBStudy;

  mycase=Case;
  doRapStudies = doRapStudy;
  doPDF = doPDFStudy;

  doIterClosure = myDoIter;

  if(!doComp) plotRatiosDiGaus(myVtxBin);
  if(doComp) plotRatiosComp(myVtxBin);

}
