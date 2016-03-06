#include "RecoilCorrector.h"

using namespace std;

// mytype: 0 = target file , 1 = ZDATA , 2 = ZMC
RecoilCorrector::RecoilCorrector(bool loadKeys, std::string iNameZ, std::string iNameZ_key, TString model_name) {

  RecoilCorrector::loadKeys = loadKeys;
  
  readRecoil(fF1U1Fit,fF1U1RMSSMFit, fF1U2Fit,fF1U2RMSSMFit, iNameZ,iNameZ_key,"PF",RecoilCorrector::targetMC,model_name);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addDataFile(std::string iNameData, std::string iNameData_key, TString model_name)
{
  readRecoil(fD1U1Fit,fD1U1RMSSMFit, fD1U2Fit,fD1U2RMSSMFit, iNameData, iNameData_key, "PF",RecoilCorrector::ZDATA, model_name);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addMCFile(std::string iNameMC, std::string iNameMC_key, TString model_name)
{
  readRecoil(fM1U1Fit,fM1U1RMSSMFit, fM1U2Fit,fM1U2RMSSMFit, iNameMC,iNameMC_key, "PF",RecoilCorrector::ZMC, model_name);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readRecoil(
  std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
  std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
  std::string iFName , std::string iFKeyName , std::string iPrefix, int mytype,
  TString model_name
) {

  //type=1 read U1; type=2 read U2;
  cout << "inside readRecoil" << iFName.c_str() << endl;
  cout << "inside readRecoil Key " << iFKeyName.c_str() << endl;

  if(mytype==targetMC) cout << " read target" << endl;
  if(mytype==ZDATA)    cout << " read DATA"   << endl;
  if(mytype==ZMC)      cout << " read MC"     << endl;

  TFile *lFile = TFile::Open(iFName.c_str());

  for(int rapbin = 0; rapbin < rapbins; rapbin++) {
    cout << "reading bin " << rapbin << endl;
    int file_rapbin = rapbin+1;

    // Fill TF1 vectors from rootfile
    iU1Fit.push_back    ( (TF1*)lFile->FindObjectAny(Form("%su1Mean_%d",    iPrefix.c_str(), file_rapbin)));
    iU1MRMSFit.push_back( (TF1*)lFile->FindObjectAny(Form("%su1MeanRMS_%d", iPrefix.c_str(), file_rapbin)));
    iU2Fit    .push_back( (TF1*)lFile->FindObjectAny(Form("%su2Mean_%d",    iPrefix.c_str(), file_rapbin)));
    iU2MRMSFit.push_back( (TF1*)lFile->FindObjectAny(Form("%su2MeanRMS_%d", iPrefix.c_str(), file_rapbin)));

    wU1[mytype][rapbin] = new RooWorkspace(Form("wU1_type%d_Y%d", mytype, rapbin), "wU1");
    pdfU1[mytype][rapbin] = (RooAddPdf*) lFile->Get(Form("AddU1Y%d",file_rapbin));
    wU1[mytype][rapbin]->import(*pdfU1[mytype][rapbin],RooFit::Silence());
    frU1[mytype][rapbin] = (RooFitResult*) lFile->Get(Form("%sU1Y%d_Crapsky0_U1_2D",model_name.Data(),file_rapbin));

    runDiago(wU1[mytype][rapbin],frU1[mytype][rapbin],Form("AddU1Y%d",file_rapbin),pdfU1Cdf[mytype][rapbin]);


    wU2[mytype][rapbin] = new RooWorkspace(Form("wU2_type%d_Y%d", mytype, rapbin), "wU2");
    pdfU2[mytype][rapbin] = (RooAddPdf*) lFile->Get(Form("AddU2Y%d",file_rapbin));
    wU2[mytype][rapbin]->import(*pdfU2[mytype][rapbin],RooFit::Silence());
    frU2[mytype][rapbin] = (RooFitResult*) lFile->Get(Form("%sU2Y%d_Crapsky0_U2_2D",model_name.Data(),file_rapbin));

    runDiago(wU2[mytype][rapbin],frU2[mytype][rapbin],Form("AddU2Y%d",file_rapbin),pdfU2Cdf[mytype][rapbin]);
  }
  lFile->Close();
  
  if (loadKeys) {
    TFile* keysFile = TFile::Open(iFKeyName.c_str());
    
    for(int rapbin = 0; rapbin < rapbins; rapbin++) {
      cout << "reading bin " << rapbin << endl;
      int file_rapbin = rapbin+1;

      wU1key[mytype][rapbin] = new RooWorkspace("wU1key","wU1key");
      makeKeysVec(wU1key[mytype][rapbin], keysFile, Form("Keys_U1_%d",file_rapbin), pdfKeyU1Cdf[mytype][rapbin],true);
      
      wU2key[mytype][rapbin] = new RooWorkspace("wU2key","wU2key");
      makeKeysVec(wU2key[mytype][rapbin], keysFile, Form("Keys_U2_%d",file_rapbin), pdfKeyU2Cdf[mytype][rapbin],false);
    }
    keysFile->Close();
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------

double RecoilCorrector::calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2) {
  double lMX = -iEPt*cos(iEPhi) - iU1*cos(iWPhi) + iU2*sin(iWPhi);
  double lMY = -iEPt*sin(iEPhi) - iU1*sin(iWPhi) - iU2*cos(iWPhi);

  if(iMet == 0) return sqrt(lMX*lMX + lMY*lMY);
  if(iMet == 1) {if(lMX > 0) {return atan(lMY/lMX);} return (fabs(lMY)/lMY)*TMath::Pi() + atan(lMY/lMX); }
  if(iMet == 2) return lMX;
  if(iMet == 3) return lMY;
  return lMY;

}

void RecoilCorrector::reset(int RecoilCorrParMaxU1, int RecoilCorrParMaxU2, int rapBinCorr)
{
  // reset all to zero
  for(int ipar=0; ipar<RecoilCorrParMaxU1; ipar++){
    TString eig = Form("eig_eig%d",ipar);
    wU1[ZDATA][rapBinCorr]->var(eig)->setVal(0);
    wU1[ZMC][rapBinCorr]->var(eig)->setVal(0);
  }
  for(int ipar=0; ipar<RecoilCorrParMaxU2; ipar++){
    TString eig = Form("eig_eig%d",ipar);
    wU2[ZDATA][rapBinCorr]->var(eig)->setVal(0);
    wU2[ZMC][rapBinCorr]->var(eig)->setVal(0);
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::CorrectMET(double &met, double &metphi, double bosonPt, double bosonPhi, double sumLepPt, double sumLepPhi,double &iU1,double &iU2,int RecoilCorrVarDiagoParU1orU2fromDATAorMC,int RecoilCorrVarDiagoParN,int RecoilCorrVarDiagoParSigmas,int rapbin, int recoilCorrScale, bool correctWithKeys)
{
  // ---------------------------
  // CHANGE STAT EIGEN IF NEEDED
  // ---------------------------
  
  int DataOrMcMap[] = {0, ZDATA, ZDATA, ZDATA, ZMC, ZMC, ZMC};
  int DataOrMc = DataOrMcMap[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  TString eig = Form("eig_eig%d",RecoilCorrVarDiagoParN);

  switch(RecoilCorrVarDiagoParU1orU2fromDATAorMC){
    case 0:
    break;
    case 1:
    case 2:
    case 4:
    case 5:
      // cout << "wU1["<<DataOrMc<<"]["<<rapbin<<"]->var("<<eig<<")->setVal("<<RecoilCorrVarDiagoParSigmas<<");" << endl;
      wU1[DataOrMc][rapbin]->var(eig)->setVal(RecoilCorrVarDiagoParSigmas);
    break;
    case 3:
    case 6:
      // cout << "wU2["<<DataOrMc<<"]["<<rapbin<<"]->var("<<eig<<")->setVal("<<RecoilCorrVarDiagoParSigmas<<");" << endl;
      wU2[DataOrMc][rapbin]->var(eig)->setVal(RecoilCorrVarDiagoParSigmas);
    break;
  }
  
  // ---------------------------
  // CALL REAL WORKER FUNCTION
  // ---------------------------
  
  applyMETCorrection(met,metphi,bosonPt,bosonPhi,sumLepPt,sumLepPhi,
    fF1U1Fit[rapbin],
    fD1U1Fit[rapbin],  fM1U1Fit[rapbin],
    fD1U1RMSSMFit[rapbin], fM1U1RMSSMFit[rapbin],
    fD1U2RMSSMFit[rapbin], fM1U2RMSSMFit[rapbin],
    rapbin,
    recoilCorrScale,
    correctWithKeys,
    iU1,iU2
  );
  
}


//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::applyMETCorrection(
  double &iMet,double &iMETPhi,
  double bosonPt,double bosonPhi,
  double sumLepPt,double sumLepPhi,
  TF1 *iU1Default,
  TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
  TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
  TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
  int rapbin,
  int corrScale,
  bool correctWithKeys,
  double &pU1,double &pU2
) {

  double pUX   = iMet*cos(iMETPhi) + sumLepPt*cos(sumLepPhi);
  double pUY   = iMet*sin(iMETPhi) + sumLepPt*sin(sumLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(bosonPhi) + pUY*sin(bosonPhi))/pU;
  double pSin  =   (pUX*sin(bosonPhi) - pUY*cos(bosonPhi))/pU;

  // Get real values of recoil (from the pull space vars)
  double pU1noCorr = pU*pCos;
  double pU2noCorr = pU*pSin;

  // cout << " ------------------------------------------------------- " << endl;
  // cout << " initial pU1 = " << pU1noCorr << " pU2 = " << pU2noCorr << endl;

  // Will contain corrected values of U1 and U2
  double pU1corr;
  double pU2corr;

  if(correctWithKeys) {
    // this need the absolute space
    pU1corr = keysInvGraph(pU1noCorr,bosonPt,pdfKeyU1Cdf[ZMC][rapbin],pdfKeyU1Cdf[ZDATA][rapbin],true, 50);
    pU2corr = keysInvGraph(pU2noCorr,bosonPt,pdfKeyU2Cdf[ZMC][rapbin],pdfKeyU2Cdf[ZDATA][rapbin],false, 50);

  }
  else {
    // Get the absolute <-> pull space conversion variables
    
    double lRescale  = sqrt((TMath::Pi())/2.);  // Magic normalization number due to usage of absolute values while computing the overall RMS
    //  double lRescale  = 1;     // for squares

    double pDefU1    = iU1Default->Eval(bosonPt); // U1 average scale for target sample

    double pDU1       = iU1RZDatFit ->Eval(bosonPt); // U1 average scale for ZDATA
    double pDRMSU1    = iU1MSZDatFit->Eval(bosonPt)*lRescale; // U1 average RMS for ZDATA
    double pDRMSU2    = iU2MSZDatFit->Eval(bosonPt)*lRescale; // U2 average RMS for ZDATA

    double pMU1       = iU1RZMCFit  ->Eval(bosonPt); // U1 average scale for ZMC
    double pMRMSU1    = iU1MSZMCFit ->Eval(bosonPt)*lRescale; // U1 average RMS for ZMC
    double pMRMSU2    = iU2MSZMCFit ->Eval(bosonPt)*lRescale; // U2 average RMS for ZMC

    //
    // ENDING of the PARAMETERS
    //
    
    // cout 
      // << "before triGausInvGraphPDF"
      // << " pdfU1Cdf[ZMC]["<<rapbin<<"]->getVal()= " << pdfU1Cdf[ZMC][rapbin]->getVal()
      // << " pdfU1Cdf[ZDATA]["<<rapbin<<"]->getVal()= " << pdfU1Cdf[ZDATA][rapbin]->getVal()
      // << " pdfU2Cdf[ZMC]["<<rapbin<<"]->getVal()= " << pdfU2Cdf[ZMC][rapbin]->getVal()
      // << " pdfU2Cdf[ZDATA]["<<rapbin<<"]->getVal()= " << pdfU2Cdf[ZDATA][rapbin]->getVal()
      // << endl;
    
    // It shouldn't be needed, but without this it crashes
    pdfU1Cdf[ZMC][rapbin]->getVal();
    pdfU1Cdf[ZDATA][rapbin]->getVal();
    pdfU2Cdf[ZMC][rapbin]->getVal();
    pdfU2Cdf[ZDATA][rapbin]->getVal();
    
    // ==============================================================
    // Go to the pull space (from absolute)
    // In comments are the version for correcting a different MC
    // If you use the same targetMC as ZMC it simplifies to the uncommented ones
    // ==============================================================
    double pU1Diff  = pU1noCorr-pDefU1;
    double pU2Diff  = pU2noCorr;
    
    // pU1Diff = pU1Diff/pDefRMSU1;
    pU1Diff = pU1Diff/pMRMSU1;
    // pU1Diff = pU1Diff/pDefRMSU2;
    pU2Diff = pU2Diff/pMRMSU2;
    
    //  std::cout << "================= " << std::endl;
    //  std::cout << " Before: pU1Diff " << pU1Diff << " pU2Diff " << pU2Diff << std::endl;
    
    // Call correcting function in pull space
    pU1corr = triGausInvGraphPDF(pU1Diff,bosonPt,pdfU1Cdf[ZMC][rapbin],pdfU1Cdf[ZDATA][rapbin],wU1[ZMC][rapbin],wU1[ZDATA][rapbin],5);
    pU2corr = triGausInvGraphPDF(pU2Diff,bosonPt,pdfU2Cdf[ZMC][rapbin],pdfU2Cdf[ZDATA][rapbin],wU2[ZMC][rapbin],wU2[ZDATA][rapbin],5);
    
    // ==============================================================
    // Go back to the absolute space (from pull)
    // In comments are the version for correcting a different MC
    // If you use the same targetMC as ZMC it simplifies to the uncommented ones
    // ==============================================================
    
    // pU1corr *= pDefRMSU1 * (pDRMSU1/pMRMSU1);
    pU1corr *= pDRMSU1;
    // pU1corr += pDefU1 + (pDU1-pMU1);
    pU1corr += pDU1;

    // pU2corr *= pDefRMSU2 * (pDRMSU2/pMRMSU2);
    pU2corr *= pDRMSU2;

  }
  
  double pU1delta = pU1corr - pU1noCorr;
  double pU2delta = pU2corr - pU2noCorr;
  
  pU1 = pU1noCorr + corrScale * pU1delta;
  pU2 = pU2noCorr + corrScale * pU2delta;

  iMet  = calculate(0,sumLepPt,sumLepPhi,bosonPhi,pU1,pU2);
  iMETPhi = calculate(1,sumLepPt,sumLepPhi,bosonPhi,pU1,pU2);

  // cout << " after pU1 = " << pU1 << " pU2 = " << pU2 << endl;

}
//-----------------------------------------------------------------------------------------------------------------------------------------
// double RecoilCorrector::triGausInvGraphPDF(double iPVal, double Zpt, RooAddPdf *pdfMC, RooAddPdf *pdfDATA, RooWorkspace *wMC, RooWorkspace *wDATA) {
double RecoilCorrector::triGausInvGraphPDF(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA, double max) {

  if(TMath::Abs(iPVal)>=max) return iPVal;

  // fix pt for CDF
  RooRealVar* myptmCDF = (RooRealVar*) pdfMCcdf->getVariables()->find("pt");
  RooRealVar* myptdCDF = (RooRealVar*) pdfDATAcdf->getVariables()->find("pt");
  myptmCDF->setVal(Zpt);
  myptdCDF->setVal(Zpt);

  // fix pt for workspace
  RooRealVar* myptm=wMC->var("pt");
  RooRealVar* myptd=wDATA->var("pt");
  myptm->setVal(Zpt);
  myptd->setVal(Zpt);

  RooRealVar* myXm = wMC->var("XVar");
  RooRealVar* myXd = wDATA->var("XVar");
  
  myXm->setVal(iPVal);
  double pVal=pdfDATAcdf->findRoot(*myXd,myXd->getMin(),myXd->getMax(),pdfMCcdf->getVal());

  // add protection for outlier since I tabulated up to 5
  if(TMath::Abs(pVal)>=max) pVal=iPVal;
  // if(TMath::Abs(iPVal)>=5) pVal=5;

  myptd->setVal(1);
  myptm->setVal(1);
  myXm->setVal(0);
  myXd->setVal(0);

  return pVal;

}

double RecoilCorrector::keysInvGraph(double iPVal, double Zpt, std::vector<RooAbsReal*> pdfKeyMCcdf, std::vector<RooAbsReal*> pdfKeyDATAcdf, bool isU1, double max) {

  // add protection for outlier since I tabulated up to max
  if(TMath::Abs(iPVal)>=max) return iPVal;

  //  int U1U2=1;
  //  if(!isU1) U1U2=2;

  int Zptbin=int(Zpt);
  //  if(int(Zpt)==0) Zptbin=0;
  if(int(Zpt)>49) Zptbin=49;
  if(isU1 && int(Zpt)>29) Zptbin=29;

  /*
  int Zptbin=int(Zpt)-1;
  if(int(Zpt)==0) Zptbin=0;
  if(int(Zpt)>49) Zptbin=49;
  */

  //  cout << "Zptbin=" << Zptbin << " Zpt=" << Zpt << " pdfKeyMCcdf.size()=" << pdfKeyMCcdf.size() << " pdfKeyDATAcdf.size()=" << pdfKeyDATAcdf.size() << endl;

  RooRealVar* myXmCDF = (RooRealVar*) pdfKeyMCcdf[Zptbin]->getVariables()->find("XVar");
  RooRealVar* myXdCDF = (RooRealVar*) pdfKeyDATAcdf[Zptbin]->getVariables()->find("XVar");

  //  cout << "iPVal" << iPVal << " myXmCDF=" << myXmCDF->getVal() << " myXdCDF=" << myXdCDF->getVal() << endl;

  myXmCDF->setVal(iPVal);
  double pVal=pdfKeyDATAcdf[Zptbin]->findRoot(*myXdCDF, myXdCDF->getMin(), myXdCDF->getMax(), pdfKeyMCcdf[Zptbin]->getVal());

  // add protection for outlier since I tabulated up to max
  if(TMath::Abs(pVal)>=max) pVal=iPVal;

  return pVal;

}


//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::makeKeysVec(RooWorkspace *w, TFile * lFileKeys, TString fit, std::vector<RooAbsReal*> &pdfUiCdf, bool isU1) {

  //  lFile->ls();

  int Zmax=29;
  if(!isU1) Zmax=49;

  for(int Zpt=0; Zpt<=Zmax; Zpt++) {

    RooAbsPdf* pdfKey = (RooKeysPdf*) lFileKeys->Get(Form("%s_%d",fit.Data(),Zpt));
    w->import(*pdfKey, RooFit::RecycleConflictNodes(),RooFit::Silence());
    RooRealVar* myX1=w->var("XVar");

    //    RooRealVar* myX1 = (RooRealVar*) pdfKey->getVariables()->find("XVar");
    //    cout << "CDF pointer " << pdfKey->createCdf(*myX1) << endl;

    RooAbsReal * iKeyPdf = pdfKey->createCdf(*myX1);
    pdfUiCdf.push_back(iKeyPdf);
    w->import(*iKeyPdf, RooFit::RecycleConflictNodes(),RooFit::Silence());
  }

  // w->Print();
  return;

}


//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::runDiago(RooWorkspace *w, RooFitResult *result, TString fit, RooAbsReal *&pdfUiCdf) {
  
  // cout << "w= " << w << " result= " << result << " fit= " << fit << " pdfUiCdf= " << pdfUiCdf << endl;
  
  // cout << "BEFORE DIAGO 1" << " fit= " << fit<< endl;
  // w->Print();
  // cout << "AFTER CALLING w->Print()"<< " fit= " << fit << endl;
  // cout << "CALLING result->Print(\"V\")" << endl;
  // result->Print("V");
  PdfDiagonalizer *diago = new PdfDiagonalizer("eig", w, *result);
  // cout << "AFTER CALLING DIAGO 1, searching for pdf " << fit << " fit= " << fit<< endl;
  RooAddPdf* pdf_temp = (RooAddPdf*) w->pdf(fit.Data());
  // pdf_temp->Print();
  // cout << "AFTER CALLING pdf_temp"<< " fit= " << fit << endl;
  RooAbsPdf *newpdf = diago->diagonalize(*pdf_temp);
  // cout << "AFTER CALLING diagonalize" << endl;
  // newpdf->Print();
  // cout << "AFTER print after CALLING diagonalize" << endl;
  w->import(*newpdf, RooFit::RecycleConflictNodes(),RooFit::Silence());
  
  RooRealVar* myX1=w->var("XVar");
  // pdfUiCdf = newpdf->createCdf(*myX1,RooFit::ScanAllCdf());
  pdfUiCdf = newpdf->createCdf(*myX1);
  w->import(*pdfUiCdf, RooFit::RecycleConflictNodes(),RooFit::Silence());
  // w->Print();
  
  return;
}
