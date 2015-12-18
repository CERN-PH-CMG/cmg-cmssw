#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <sstream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TF1.h"
#include "TMath.h"
#include "TVirtualFitter.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TRandom1.h"
#include "TGraphErrors.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"
using namespace RooFit;

TFile *fDataFile = 0;
TTree *fDataTree = 0; 

bool  fData   = false;
float fMetMax = 0; float fZPtMax = 0; int fNJetSelect = 0; 
float fZPt = 0; float fU1 = 0; float fU2 = 0; float fPFU1 = 0;  float fPFU2   = 0; float fTKU1 = 0; float fTKU2 = 0;
float fZPhi = 0; float fPhi = 0; float fMet = 0; int fNJet = 0; float fWeight = 0;
int fId1 = 0; int fId2 = 0; float fGPhi1 = 0; float fGPhi2 = 0; float fGEta1  = 0; float fGEta2 = 0; float fEta2 = 0; float fPhi2 = 0; float fMPhi = 0;
void load(TTree *iTree, int type) { 
  iTree->ResetBranchAddresses();
  iTree->SetBranchAddress("id1_l" ,&fId1);
  iTree->SetBranchAddress("id2_l" ,&fId2);
  iTree->SetBranchAddress("phi1_l",&fGPhi1);
  iTree->SetBranchAddress("phi2_l",&fGPhi2);
  iTree->SetBranchAddress("eta1_l",&fGEta1);
  iTree->SetBranchAddress("eta2_l",&fGEta2);
  iTree->SetBranchAddress("eta_2" ,&fEta2);
  iTree->SetBranchAddress("phi_2" ,&fPhi2);
  iTree->SetBranchAddress("Weight",&fWeight);
  if(!fData) iTree->SetBranchAddress("pt_z" ,&fZPt);
  if(!fData) iTree->SetBranchAddress("phi_z",&fZPhi);
  if(fData)  iTree->SetBranchAddress("rpt_z" ,&fZPt);
  if(fData)  iTree->SetBranchAddress("rphi_z",&fZPhi);
  iTree->SetBranchAddress("njet",&fNJet);
  if(type==0) {
    iTree->SetBranchAddress("u1" ,&fU1);
    iTree->SetBranchAddress("u2" ,&fU2);
    iTree->SetBranchAddress("met",&fMet);
    iTree->SetBranchAddress("metphi",&fMPhi);
  } else if(type==1) {
    iTree->SetBranchAddress("trku1" ,&fU1);
    iTree->SetBranchAddress("trku2" ,&fU2);
    iTree->SetBranchAddress("trkmet",&fMet); 
  } else if(type==2) {
    //iTree->SetBranchAddress("trku1" ,&fTKU1);
    //iTree->SetBranchAddress("trku2" ,&fTKU2);
    iTree->SetBranchAddress("u1"  ,&fPFU1);
    iTree->SetBranchAddress("u2"  ,&fPFU2);
    iTree->SetBranchAddress("met",&fMet);
    iTree->SetBranchAddress("metphi",&fMPhi);
  }
}
void calculateU1U2(float &iPar,bool iType) {
  double lUX  = fMet*cos(fMPhi) + fZPt*cos(fZPhi);
  double lUY  = fMet*sin(fMPhi) + fZPt*sin(fZPhi);
  double lU   = sqrt(lUX*lUX+lUY*lUY);
  double lCos = - (lUX*cos(fZPhi) + lUY*sin(fZPhi))/lU;
  double lSin =   (lUX*sin(fZPhi) - lUY*cos(fZPhi))/lU;
  fU1 = lU*lCos;
  fU2 = lU*lSin;
  if(iType)  iPar = fU1;
  if(!iType) iPar = fU2;
}
bool passMatching() { 
  double lDEta0  = fGEta1-fEta2;
  double lDPhi0 = fabs(fGPhi1-fPhi2); if(lDPhi0 > 2.*TMath::Pi()-lDPhi0) lDPhi0 = 2.*TMath::Pi()-lDPhi0;
  double lDR0 = sqrt(lDEta0*lDEta0+lDPhi0*lDPhi0);
  double lDEta1  = fGEta2-fEta2;
  double lDPhi1 = fabs(fGPhi2-fPhi2); if(lDPhi1 > 2.*TMath::Pi()-lDPhi1) lDPhi1 = 2.*TMath::Pi()-lDPhi1;
  double lDR1 = sqrt(lDEta1*lDEta1+lDPhi1*lDPhi1);
  return (lDR0 < 0.3 || lDR1 < 0.3);
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
  return sqrt(lVal);//+(iZMCFit->Eval(iVal)-iWFit->Eval(iVal);
}

void computeFitErrors(TF1 *iFit,TFitResultPtr &iFPtr,TF1 *iTrueFit,bool iPol0=false) {  
  bool lPol0 = iPol0;
  bool lPol2 = (iTrueFit->GetParError(2) != 0);
  bool lPol3 = (iTrueFit->GetParError(3) != 0);

  TMatrixDSym lCov = iFPtr->GetCovarianceMatrix();  
  double lE0 = (lCov)(0,0);
  double lE1 = (lCov)(0,1) + (lCov)(1,0);
  double lE2 = (lCov)(1,1);

  double lE5 = 0; if(lPol2) lE5 = 2*(lCov)(2,0);
  double lE3 = 0; if(lPol2) lE3 = 2*(lCov)(1,2);
  double lE4 = 0; if(lPol2) lE4 =  (lCov)(2,2);   //This scheme preserves the diagaonals
  double lE6 = 0;
  if(lPol3) { 
    lE2 += 2*(lCov)(0,2);
    lE3 += 2*(lCov)(0,3);
    lE4 += 2*(lCov)(1,3);
    lE5  = 2*(lCov)(2,3);
    lE6  = (lCov)(3,3);
  }
  if(lPol0) {lE0 = iTrueFit->GetParError(0)*iTrueFit->GetParError(0); lE1 = 0; lE2 = 0; lE3=0;}
  iFit->SetParameter(0,iTrueFit->GetParameter(0)); iFit->SetParError(0,lE0);
  iFit->SetParameter(1,iTrueFit->GetParameter(1)); iFit->SetParError(1,lE1);
  iFit->SetParameter(2,iTrueFit->GetParameter(2)); iFit->SetParError(2,lE2);
  iFit->SetParameter(3,iTrueFit->GetParameter(3)); iFit->SetParError(3,lE3);
  iFit->SetParameter(4,iTrueFit->GetParameter(4)); iFit->SetParError(4,lE4);
  iFit->SetParError(5,lE5);
  iFit->SetParError(6,lE6);
}

void drawErrorBands(TF1 *iFit,double iXMax) { 
  int lN = int(iXMax*5.)*2;
  TGraph *lG0 = new TGraph(lN-1);
  TGraph *lG1 = new TGraph(lN-1);
  for(int i0 = 0; i0 < lN/2; i0++) {
    double pVal = i0*0.2;
    lG0->SetPoint(i0,pVal,iFit->Eval(pVal)+sqrt(getError2(pVal,iFit)));      //Small Yellow Band
    lG1->SetPoint(i0,pVal,iFit->Eval(pVal)+sqrt(getCorError2(pVal,iFit)));   //Large Redfi Band
  }
  for(int i0 = 0; i0 < lN/2; i0++) {
    double pVal = lN/2*0.2-(i0+1)*0.2;
    lG0->SetPoint(lN/2+i0,pVal,iFit->Eval(pVal)-sqrt(getError2(pVal,iFit)));
    lG1->SetPoint(lN/2+i0,pVal,iFit->Eval(pVal)-sqrt(getCorError2(pVal,iFit)));
  }
  lG0->SetFillColor(kYellow);
  lG1->SetFillColor(kRed);
  lG1->Draw("F");
  lG0->Draw("F");
}
double fCorrMax = 70;
TGraphErrors* makeGraph(int iNBins,int iId,std::vector<float> &iXVals,std::vector<float> &iY0Vals,std::vector<float> &iY1Vals,
			std::vector<float> &iXEVals,std::vector<float> &iYEVals) { 
  std::vector<float> iPar0;    std::vector<float> iPar1;
  std::vector<float> iEPar0;   std::vector<float> iEPar1;
  for(unsigned int i0 = 0; i0 < iXVals.size(); i0++) { 
    if(iXVals[i0] < iId*(int(fCorrMax/iNBins))) continue;
    //if(iId != iNBins-1 && 
    if(iXVals[i0] > (iId+1)*(int(fCorrMax/iNBins))) continue;
    iPar0.push_back(iY0Vals[i0]); 
    iPar1.push_back(iY1Vals[i0]); 
    iEPar0.push_back(1);
    iEPar1.push_back(iYEVals[i0]);
  }
  TGraphErrors *lF0 = new TGraphErrors(iPar1.size(),&iPar0[0],&iPar1[0],&iEPar0[0],&iEPar1[0]);
  return lF0;
}
TH2F* makeHist(int iNBins,int iId,std::vector<float> &iXVals,std::vector<float> &iY0Vals,std::vector<float> &iY1Vals) { 
  TH2F *lH0 = new TH2F("X","X",30,-5,0,30,-5,0);
  for(unsigned int i0 = 0; i0 < iXVals.size(); i0++) { 
    if(iXVals[i0] < iId*(int(fCorrMax/iNBins))) continue;
    //if(iId != iNBins-1 && 
    if(iXVals[i0] > (iId+1)*(int(fCorrMax/iNBins))) continue;
    lH0->Fill(iY0Vals[i0],iY1Vals[i0]);
  }
  return lH0;
}
double angle(double lMX,double lMY) {
  if(lMX > 0) {return atan(lMY/lMX);} 
  return (fabs(lMY)/lMY)*3.14159265 + atan(lMY/lMX);
}
void fitCorr(bool iRMS,TTree *iTree,TCanvas *iC,float &lPar0,float &lPar1,
	     TF1   *iMeanFit0=0,TF1   *iMeanFit1=0,
	     TF1   *iRMSFit0 =0,TF1   *iRMSFit1 =0,
             TF1   *iCorrFit=0) { 
  iC->cd();
  fCorrMax = 70;
  const int iNBins = 7;
  TGraphErrors  **lG0  = new TGraphErrors*[iNBins]; double *lXCorr = new double[iNBins]; double *lCorr = new double[iNBins];
  std::vector<float> lXVals; std::vector<float> lY0Vals; std::vector<float> lY1Vals;
  std::vector<float> lXEVals; std::vector<float> lYEVals; 
  for(int i1 = 0; i1 <  iTree->GetEntries(); i1++) {
    iTree->GetEntry(i1);
    if(fZPt > fZPtMax) continue;
    if(fMet > fMetMax) continue;
    if(fNJetSelect<2 && fNJet!=fNJetSelect && fNJetSelect != -1) continue;
    if(fNJetSelect==2 && fNJet<2) continue;
    double pVal0 = lPar0;
    double pVal1 = lPar1;
    if(iMeanFit0 != 0 && iRMSFit0 != 0 )   pVal0 = (lPar0-iMeanFit0->Eval(fZPt))/iRMSFit0->Eval(fZPt);
    if(iMeanFit1 != 0 && iRMSFit1 != 0 )   pVal1 = (lPar1-iMeanFit1->Eval(fZPt))/iRMSFit1->Eval(fZPt);
    if( (pVal0 > 0 || pVal1 > 0)) continue;
    if(fWeight == 0) continue;
    if(fData) pWeight = 1;
    //if(pVal1 > pVal0) continue;
    //if(iRMS && pVal0 > 0. && ((fPFU1 == lPar0) || (fTKU1 == lPar0))) continue;
    //if(iRMS && pVal1 > 0. && ((fPFU1 == lPar1) || (fTKU1 == lPar1))) continue;
    double pWeight = fWeight;
    lXVals.push_back(fZPt); lXEVals.push_back(2.);
    (!iRMS) ? lY0Vals.push_back(pVal0) : lY0Vals.push_back(pVal0);
    (!iRMS) ? lY1Vals.push_back(pVal1) : lY1Vals.push_back(pVal1);
    lYEVals.push_back(1./sqrt(pWeight));
  }
  for(int i0 = 0; i0 < iNBins; i0++) lG0[i0] = makeGraph(iNBins,i0,lXVals,lY0Vals,lY1Vals,lXEVals,lYEVals);
  //for(int i0 = 2; i0 < 7; i0++) project(iNBins,i0,lXVals,lY0Vals,lY1Vals);
  for(int i0 = 0; i0 < iNBins; i0++) { 
     //lG0[i0]->Fit("pol1");
    //lG0[i0]->Draw("ap"); 
    lXCorr[i0] = i0*(fCorrMax/iNBins) + (fCorrMax/iNBins/2.);
    lG0[i0]->Fit("pol1","SR","EXO");
    lCorr [i0] = lG0[i0]->GetFunction("pol1")->GetParameter(1);
  }
  TGraph *lFinalCorr = new TGraph(iNBins,lXCorr,lCorr);
  std::string lType = "pol1";
  TF1 *lFit = new TF1("test",lType.c_str());
  TFitResultPtr  lFitPtr = lFinalCorr->Fit(lFit,"SR","",0,fCorrMax);
  computeFitErrors(iCorrFit,lFitPtr,lFit,iRMS);
  lFinalCorr->SetMarkerStyle(kFullCircle);
  lFinalCorr->Draw("ap");
  drawErrorBands(iCorrFit,100);
  lFinalCorr->Draw("p");
  iC->SaveAs("Crap.png");
  //cin.get();
}
void fitGraph(TTree *iTree, TCanvas *iC,
	      float &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1* iMeanFit=0,
	      bool iPol1 = false, 
	      bool iRMS  = false, int iType = 0) { 
  //RooFit Build a double Gaussian
  TRandom lRand(0xDEADBEEF);
  RooRealVar    lRWeight("weight","weight",0,10);
  RooRealVar lRPt  ("pt","Z_{p_{T}}",10,1000);
  RooRealVar    lA1Sig("a1sig","a1sig",0.8,0,1); 
  RooRealVar    lB1Sig("b1sig","b1sig",0. ,-0.1,0.1); 
  RooRealVar    lC1Sig("c1sig","c1sig",0. ,-0.1,0.1);  //lC1Sig.setConstant(kTRUE);
  RooRealVar    lD1Sig("d1sig","d1sig",0. ,-0.1,0.1);  lD1Sig.setConstant(kTRUE);
  RooRealVar    lA2Sig("a2sig","a2sig",1.4,1,3); 
  RooRealVar    lB2Sig("b2sig","b2sig",0.0 ,-0.1,0.1); 
  RooRealVar    lC2Sig("c2sig","c2sig",0.0 ,-0.1,0.1);  //lC2Sig.setConstant(kTRUE);
  RooRealVar    lD2Sig("d2sig","d2sig",0.0 ,-0.1,0.1);  lD2Sig.setConstant(kTRUE);
  RooFormulaVar l1Sigma("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
  RooFormulaVar l2Sigma("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
  RooFormulaVar lFrac  ("frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(l1Sigma,l2Sigma));
  RooRealVar lR1Mean("mean","mean",0,-10,10); lR1Mean.setConstant(kTRUE);
  //RooRealVar lFrac  ("frac","frac",0.8,0.,1);
  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{1} (Z_{p_{T}})",0,-5,5);
  RooGaussian   lGaus1("gaus1","gaus1",lRXVar,lR1Mean,l1Sigma);
  RooGaussian   lGaus2("gaus2","gaus2",lRXVar,lR1Mean,l2Sigma);
  RooAddPdf     lGAdd("Add","Add",lGaus1,lGaus2,lFrac);
  RooRealVar    lR1Sigma("Rsigma1","RSigma1",8. ,0,40);
  RooRealVar    lR2Sigma("Rsigma2","RSigma2",12.,1,40);
  RooRealVar    lR1Frac ("Rfrac"  ,"Rfrac"  ,0.5,0,1);
  RooRealVar    lR2Mean("Rmean",      "Rmean",0,-80,80); 
  RooGaussian   lRGaus1("Rgaus1","Rgaus1",lRXVar,lR2Mean,lR1Sigma);
  RooGaussian   lRGaus2("Rgaus2","Rgaus2",lRXVar,lR2Mean,lR2Sigma);
  RooAddPdf     lRGAdd ("RAdd"  ,"RAdd",lRGaus1,lRGaus2,lR1Frac);
  std::vector<float> lXVals;  std::vector<float> lYVals; std::vector<float> lYTVals;
  std::vector<float> lXEVals; std::vector<float> lYEVals; 
  //Fill values (if dealing with resolution we take difference from mean fit)
  for(int i1 = 0; i1 <  iTree->GetEntries(); i1++) {
    iTree->GetEntry(i1);
    //if(!passMatching()) continue;
    //calculateU1U2(lPar,lPar==fU1);
    //if(iPol1 && fZPt > 50. && lPar == fU1 && lPar  > (47.5-0.95*fZPt) && iType == 0) continue; //Remove Diboson events at high pT
    if(fZPt > fZPtMax) continue;
    if(fMet > fMetMax) continue;
    if(fNJetSelect<2  && fNJet != fNJetSelect && fNJetSelect != -1) continue;
    if(fNJetSelect==2 && fNJet<2) continue;
    double pVal = lPar;
    if(iRMS && iMeanFit != 0)   pVal = (lPar-iMeanFit->Eval(fZPt));
    if(iRMS && pVal > 0. && fU1 == lPar) {continue;}
    if(fWeight == 0) continue;
    lXVals.push_back(fZPt);  lXEVals.push_back(2.);
    if(iRMS && pVal < 0 && lPar == fU1) {
      double lCorr = (lRand.Uniform(-1,1));
      lCorr = lCorr/fabs(lCorr);
      pVal *= lCorr;
    }
    double pWeight = fWeight;
    if(fData) pWeight = 1;
    (!iRMS) ? lYVals.push_back(pVal) : lYVals.push_back(fabs(pVal));
    lYTVals.push_back(pVal); lYEVals.push_back(1./sqrt(pWeight));
  }
  iC->cd();
  
  //Basic Fit
  TGraphErrors *pGraphA = new TGraphErrors(lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]),(Float_t*)&(lXEVals[0]),(Float_t*)&(lYEVals[0]));
  //TGraph *pGraphA       = new TGraph      (lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]));
  std::string lType = "pol3"; if(iPol1) lType = "pol3"; 
  if(iType == 1)  lType = "pol2"; 
  TF1 *lFit = new TF1("test",lType.c_str()); //fZPtMax = 200;
  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
  computeFitErrors(iFit,lFitPtr,lFit,iRMS);
  pGraphA->Draw("ap");
  drawErrorBands(iFit,100);
  //iC->SaveAs("crap.png");
  //cin.get();
  
  if(iRMS) {    
    //Build the double gaussian dataset
    std::vector<RooDataSet> lResidVals; 
    std::vector<RooDataHist> lResidVals2D; 
    std::stringstream pSS; pSS << "Crapsky" << 0;
    pSS << "2D"; RooDataHist lData2D(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar,lRPt));
    for(int i0  = 0; i0 < 5; i0++) { 
      RooDataSet lData(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar)); lRPt.setBins(500);
      lResidVals.push_back(lData); 
    }
    lResidVals2D.push_back(lData2D);
    std::vector<float> lX3SVals; std::vector<float> lXE3SVals; std::vector<float> lY3SVals; std::vector<float> lYE3SVals; //Events with sigma > 3
    std::vector<float> lX4SVals; std::vector<float> lXE4SVals; std::vector<float> lY4SVals; std::vector<float> lYE4SVals; //Events with sigma > 3

    for(unsigned int i0 = 0; i0 < lXVals.size(); i0++) { 
      //if(i0 > 10000) continue;
      double lYTest = lFit->Eval(lXVals[i0])*sqrt(2*3.14159265)/2.;
      lRXVar.setVal(lYTVals[i0]/(lYTest));
      lRPt.setVal(lXVals[i0]);
      lRWeight.setVal(1./lYEVals[i0]/lYEVals[i0]);
      int pId = int(lXVals[i0]/10.); if(pId > 4) pId = 4; 
      lResidVals[pId].add(RooArgSet(lRXVar),lRWeight.getVal()); //Fill the Double Gaussian
      lResidVals2D[0].add(RooArgSet(lRXVar,lRPt),lRWeight.getVal()); //Fill the Double Gaussian
    }
    RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2));//,Minos());//,Minos()); //Double Gaussian fit
    //Plot it all
    lRXVar.setBins(100);
    for(int i0  = 0; i0 < -1; i0++) { 
      //lRGAdd.fitTo(lResidVals[i0],Save(kTRUE),NumCPU(2));//,Minos());//,Minos()); //Double Gaussian fit
      lRPt.setRange(i0*10,i0*10+10);
      RooPlot *lFrame1 = lRXVar.frame(Title("XXX")) ;
      //lResidVals2D[0].plotOn(lFrame1);
      lResidVals [i0].plotOn(lFrame1,RooFit::MarkerColor(kRed));
      lGAdd.plotOn(lFrame1);
      lGAdd.plotOn(lFrame1,RooFit::Components(lRGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
      lGAdd.plotOn(lFrame1,RooFit::Components(lRGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(kViolet));
      iC->cd(); lFrame1->Draw();
      iC->SaveAs("crap.png");
      //iC->SaveAs("crap.eps");
      //cin.get();    
    }
    TMatrixDSym   lCov = pFR->covarianceMatrix();
    
    //Now Store each sigma of the double gaussian in a fit function
    //lFit = new TF1("test","pol2");
    iFitS1->SetParameter(0,lA1Sig.getVal()); iFitS1->SetParError(0,lA1Sig.getError()*lA1Sig.getError());
                                             iFitS1->SetParError(1,2*(lCov)(0,2));
    iFitS1->SetParameter(1,lB1Sig.getVal()); iFitS1->SetParError(2,lB1Sig.getError()*lB1Sig.getError());
                                             iFitS1->SetParError(3,2*(lCov)(2,4));
    iFitS1->SetParameter(2,lC1Sig.getVal()); iFitS1->SetParError(4,lC1Sig.getError()*lC1Sig.getError());
                                             iFitS1->SetParError(5,2*(lCov)(0,4));
    iFitS1->SetParameter(3,lD1Sig.getVal()); iFitS1->SetParError(6,lD1Sig.getError()*lD1Sig.getError());
      
    iFitS2->SetParameter(0,lA2Sig.getVal()); iFitS2->SetParError(0,lA2Sig.getError()*lA2Sig.getError());
                                             iFitS2->SetParError(1,2*(lCov)(1,3));
    iFitS2->SetParameter(1,lB2Sig.getVal()); iFitS2->SetParError(2,lB2Sig.getError()*lB2Sig.getError());
                                             iFitS2->SetParError(3,2*(lCov)(3,5));
    iFitS2->SetParameter(2,lC2Sig.getVal()); iFitS2->SetParError(4,lC2Sig.getError()*lC2Sig.getError());
                                             iFitS2->SetParError(5,2*(lCov)(1,5));
    iFitS2->SetParameter(3,lD2Sig.getVal()); iFitS2->SetParError(6,lD2Sig.getError()*lD2Sig.getError());
  }
}

void fitRecoil(TTree * iTree, float &iVPar,
	       TF1 *iMFit, TF1 *iMRMSFit, TF1 *iRMS1Fit, TF1 *iRMS2Fit,int iType) { 
  TCanvas *lXC1 = new TCanvas("C1","C1",800,600); lXC1->cd(); 
  fitGraph(iTree,lXC1,iVPar,iMFit   ,0       ,0       ,0    ,true ,false,iType);
  fitGraph(iTree,lXC1,iVPar,iMRMSFit,iRMS1Fit,iRMS2Fit,iMFit,false,true ,iType);
}

void writeRecoil(TF1 *iU1Fit, TF1 *iU1MRMSFit, TF1 *iU1RMS1Fit, TF1 *iU1RMS2Fit,
		 TF1 *iU2Fit, TF1 *iU2MRMSFit, TF1 *iU2RMS1Fit, TF1 *iU2RMS2Fit,
		 std::string iName="recoilfit.root") {
  TFile *lFile = new TFile(iName.c_str(),"UPDATE");
  assert(iU1Fit);     iU1Fit    ->Write();
  assert(iU2Fit);     iU2Fit    ->Write();
  assert(iU1MRMSFit); iU1MRMSFit->Write();
  assert(iU1RMS1Fit); iU1RMS1Fit->Write();
  assert(iU1RMS2Fit); iU1RMS2Fit->Write();
  assert(iU2MRMSFit); iU2MRMSFit->Write();
  assert(iU2RMS1Fit); iU2RMS1Fit->Write();
  assert(iU2RMS2Fit); iU2RMS2Fit->Write();
  lFile->Close();
  delete lFile;
}
void writeCorr(TF1 *iU1U2PFCorr, TF1 *iU1U2TKCorr, TF1 *iPFTKU1Corr, TF1 *iPFTKU2Corr, TF1 *iPFTKUM1Corr, TF1 *iPFTKUM2Corr, 
	       std::string iName) {
  TFile *lFile = new TFile((iName).c_str(),"UPDATE");
  assert(iU1U2PFCorr);     iU1U2PFCorr->Write();
  //assert(iU1U2TKCorr);     iU1U2TKCorr->Write();
  //assert(iU1U2PFCorr);     iPFTKU1Corr->Write();
  //assert(iU1U2TKCorr);     iPFTKU2Corr->Write();
  //assert(iU1U2PFCorr);     iPFTKUM1Corr->Write();
  //assert(iU1U2TKCorr);     iPFTKUM2Corr->Write();
  lFile->Close();
  delete lFile;
}
void fitRecoilMET(TTree *iTree,std::string iName,int type) { 
  std::string lPrefix="PF"; if(type == 1) lPrefix = "TK";
  load(iTree,type);
  TF1 *lU1Fit     = new TF1((lPrefix+"u1Mean_0").c_str(),   "pol6");
  TF1 *lU1MRMSFit = new TF1((lPrefix+"u1MeanRMS_0").c_str(),"pol6");
  TF1 *lU1RMS1Fit = new TF1((lPrefix+"u1RMS1_0").c_str(),   "pol6");
  TF1 *lU1RMS2Fit = new TF1((lPrefix+"u1RMS2_0").c_str(),   "pol6");
  TF1 *lU2Fit     = new TF1((lPrefix+"u2Mean_0").c_str(),   "pol6");  
  TF1 *lU2MRMSFit = new TF1((lPrefix+"u2MeanRMS_0").c_str(),"pol6");
  TF1 *lU2RMS1Fit = new TF1((lPrefix+"u2RMS1_0").c_str(),   "pol6");
  TF1 *lU2RMS2Fit = new TF1((lPrefix+"u2RMS2_0").c_str(),   "pol6");
  fitRecoil(iTree,fU1,lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,type);
  fitRecoil(iTree,fU2,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,type);
  writeRecoil(lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,iName);
}
int readRecoil(std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,
	       std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,
	       std::string iFName,std::string iPrefix) {
  TFile *lFile  = new TFile(iFName.c_str());
  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((iPrefix+"u1Mean_0").c_str()));
  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1MeanRMS_0").c_str()));
  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1RMS1_0").c_str()));
  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u1RMS2_0").c_str()));
  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2Mean_0").c_str()));
  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2MeanRMS_0").c_str()));
  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS1_0").c_str()));
  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+"u2RMS2_0").c_str()));
  lFile->Close();
  return 1;
}
void makeCorr(TTree *iTree,std::string iName) { 
  TCanvas *lXC1 = new TCanvas("C1","C1",800,600); lXC1->cd(); 
  TF1 *lU1U2PFCorr = new TF1("u1u2pfCorr_0","pol6");
  TF1 *lU1U2TKCorr = new TF1("u1u2tkCorr_0","pol6");
  TF1 *lPFTKU1Corr = new TF1("pftku1Corr_0","pol6");
  TF1 *lPFTKU2Corr = new TF1("pftku2Corr_0","pol6");
  TF1 *lPFTKUM1Corr = new TF1("pftkum1Corr_0","pol6");
  TF1 *lPFTKUM2Corr = new TF1("pftkum2Corr_0","pol6");

  vector<TF1*>   lPFU1Fit; vector<TF1*> lPFU1RMSSMFit; vector<TF1*> lPFU1RMS1Fit; vector<TF1*> lPFU1RMS2Fit; 
  vector<TF1*>   lPFU2Fit; vector<TF1*> lPFU2RMSSMFit; vector<TF1*> lPFU2RMS1Fit; vector<TF1*> lPFU2RMS2Fit; 
  vector<TF1*>   lTKU1Fit; vector<TF1*> lTKU1RMSSMFit; vector<TF1*> lTKU1RMS1Fit; vector<TF1*> lTKU1RMS2Fit; 
  vector<TF1*>   lTKU2Fit; vector<TF1*> lTKU2RMSSMFit; vector<TF1*> lTKU2RMS1Fit; vector<TF1*> lTKU2RMS2Fit; 
  readRecoil(lPFU1Fit,lPFU1RMSSMFit,lPFU1RMS1Fit,lPFU1RMS2Fit,
	     lPFU2Fit,lPFU2RMSSMFit,lPFU2RMS1Fit,lPFU2RMS2Fit,iName,"PF");
  readRecoil(lTKU1Fit,lTKU1RMSSMFit,lTKU1RMS1Fit,lTKU1RMS2Fit,  
	     lTKU2Fit,lTKU2RMSSMFit,lTKU2RMS1Fit,lTKU2RMS2Fit,iName,"TK");
  load(iTree,2);
  fitCorr(false,iTree,lXC1,fPFU1,fPFU2,lPFU1Fit[0],lPFU2Fit[0],lPFU1RMSSMFit[0],lPFU2RMSSMFit[0],lU1U2PFCorr);
  //fitCorr(false,iTree,lXC1,fTKU1,fTKU2,lTKU1Fit[0],lTKU2Fit[0],lTKU1RMSSMFit[0],lTKU2RMSSMFit[0],lU1U2TKCorr);
  //fitCorr(false,iTree,lXC1,fPFU1,fTKU1,lPFU1Fit[0],lTKU1Fit[0],lPFU1RMSSMFit[0],lTKU1RMSSMFit[0],lPFTKU1Corr);
  //fitCorr(false,iTree,lXC1,fPFU2,fTKU2,lPFU2Fit[0],lTKU2Fit[0],lPFU2RMSSMFit[0],lTKU2RMSSMFit[0],lPFTKU2Corr);
  //fitCorr(false,iTree,lXC1,fPFU1,fTKU2,lPFU1Fit[0],lTKU2Fit[0],lPFU1RMSSMFit[0],lTKU2RMSSMFit[0],lPFTKUM1Corr);
  //fitCorr(false,iTree,lXC1,fPFU2,fTKU1,lPFU2Fit[0],lTKU1Fit[0],lPFU2RMSSMFit[0],lTKU1RMSSMFit[0],lPFTKUM2Corr);
  writeCorr(lU1U2PFCorr,lU1U2TKCorr,lPFTKU1Corr,lPFTKU2Corr,lPFTKUM1Corr,lPFTKUM2Corr,iName);
}
void fitRecoil(TTree *iTree,std::string iName) { 
  //fitRecoilMET(iTree,iName,0);
  //fitRecoilMET(iTree,iName,1); 
  makeCorr(iTree,iName);
}
void runRecoilFit_v4() { 
  Prep();
  // fDataFile = new TFile("ZSummary/ZMMMC.root");
  //fDataTree = (TTree*) fDataFile->FindObjectAny("Flat");
  //fNJetSelect = -1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_zmm42X_inc.root");
  //fNJetSelect =  0; fMetMax = 4000; fZPtMax = 90;   fitRecoil(fDataTree,"recoilfits/recoilfit_zmm42X_0jet.root");
  //fNJetSelect =  1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_zmm42X_1jet.root");
  //fNJetSelect =  2; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_zmm42X_2jet.root");
  //delete fDataFile;
  //return;
  fDataFile = new TFile("ZSummary/ZMM_A.root");
  fDataTree = (TTree*) fDataFile->FindObjectAny("Flat"); fData = true;
  fNJetSelect = -1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011a_inc.root");
  fNJetSelect =  0; fMetMax = 4000; fZPtMax = 90;   fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011a_0jet.root");
  fNJetSelect =  1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011a_1jet.root");
  fNJetSelect =  2; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011a_2jet.root");  
  delete fDataFile;
  

  fDataFile = new TFile("ZSummary/ZMM_B.root");
  fDataTree = (TTree*) fDataFile->FindObjectAny("Flat"); fData = true;
  fNJetSelect = -1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011b_inc.root");
  fNJetSelect =  0; fMetMax = 4000; fZPtMax = 90;   fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011b_0jet.root");
  fNJetSelect =  1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011b_1jet.root");
  fNJetSelect =  2; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2011b_2jet.root");  
  delete fDataFile;

  fDataFile = new TFile("ZSummary/ZMM_Inc.root");
  fDataTree = (TTree*) fDataFile->FindObjectAny("Flat"); fData = true;
  fNJetSelect = -1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_inc.root");
  fNJetSelect =  0; fMetMax = 4000; fZPtMax = 90;   fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_0jet.root");
  fNJetSelect =  1; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_1jet.root");
  fNJetSelect =  2; fMetMax = 4000; fZPtMax = 500;  fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_2jet.root");  
  delete fDataFile;
  return;

  fDataFile = new TFile("../Valentina/l_tau/zjets_mtau.root");
  fDataTree = (TTree*) fDataFile->FindObjectAny("Flat");
  //fNJetSelect = -1; fMetMax = 4000; fZPtMax = 500; fitRecoil(fDataTree,"recoilfits/recoilfit_zjets_inc.root");
  fNJetSelect = 0; fMetMax = 4000; fZPtMax = 90;  fitRecoil(fDataTree,"recoilfits/recoilfit_zjets_0jet.root");
  fNJetSelect = 1; fMetMax = 4000; fZPtMax = 500; fitRecoil(fDataTree,"recoilfits/recoilfit_zjets_1jet.root");
  fNJetSelect = 2; fMetMax = 4000; fZPtMax = 500; fitRecoil(fDataTree,"recoilfits/recoilfit_zjets_2jet.root");
  delete fDataFile;
  return;

  fDataFile = new TFile("higgs_flat_v2.root");
  fDataTree = (TTree*) fDataFile->FindObjectAny("Flat");
  fNJetSelect = -1; fMetMax = 4000; fZPtMax = 500; fitRecoil(fDataTree,"recoilfits/recoilfit_higgs_inc.root");
  fNJetSelect = 0; fMetMax = 4000; fZPtMax = 90;  fitRecoil(fDataTree,"recoilfits/recoilfit_higgs_0jet.root");
  fNJetSelect = 1; fMetMax = 4000; fZPtMax = 500; fitRecoil(fDataTree,"recoilfits/recoilfit_higgs_1jet.root");
  fNJetSelect = 2; fMetMax = 4000; fZPtMax = 500; fitRecoil(fDataTree,"recoilfits/recoilfit_higgs_2jet.root");
  delete fDataFile;
  return;

  fDataFile = new TFile("ntuples/data_select.root");
  fDataTree = (TTree*) fDataFile->FindObjectAny("Events");
  fNJetSelect = -1; fMetMax = 4000; fZPtMax = 1000; fitRecoil(fDataTree,"recoilfits/recoilfit_datamm_inc.root");
  return;
}
