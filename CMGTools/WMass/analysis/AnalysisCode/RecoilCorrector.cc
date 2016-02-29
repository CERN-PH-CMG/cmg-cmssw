#include "RecoilCorrector.h"

// mytype: 0 = target file , 1 = ZDATA , 2 = ZMC
RecoilCorrector::RecoilCorrector(bool doKeys, string iNameZ, string iNameZ_key, TString model_name, TString fNonClosure_name) {

  RecoilCorrector::doKeys = doKeys;
  readRecoil(fF1U1Fit,fF1U1RMSSMFit,fF1U1RMS1Fit,fF1U1RMS2Fit,fF1U1RMS3Fit,fF1U1FracFit, fF1U1Mean1Fit, fF1U1Mean2Fit, fF1U2Fit,fF1U2RMSSMFit,fF1U2RMS1Fit,fF1U2RMS2Fit,fF1U2RMS3Fit,fF1U2FracFit,fF1U2Mean1Fit, fF1U2Mean2Fit,iNameZ,iNameZ_key,"PF",RecoilCorrector::targetMC,model_name);
  
  fNonClosure = new TFile(fNonClosure_name.Data());
  hNonClosure[0][0] = (TH2D*) fNonClosure->Get("mean_U1_y1");
  hNonClosure[0][1] = (TH2D*) fNonClosure->Get("mean_U1_y2");
  hNonClosure[1][0] = (TH2D*) fNonClosure->Get("RMS_U2_y1");
  hNonClosure[1][1] = (TH2D*) fNonClosure->Get("RMS_U2_y2");
  
  // hNonClosure[0][0]->Smooth(10);
  // hNonClosure[0][1]->Smooth(10);
  // hNonClosure[1][0]->Smooth(10);
  // hNonClosure[1][1]->Smooth(10);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addDataFile(std::string iNameData, std::string iNameData_key, TString model_name)
{
  readRecoil(fD1U1Fit,fD1U1RMSSMFit,fD1U1RMS1Fit,fD1U1RMS2Fit,fD1U1RMS3Fit,fD1U1FracFit, fD1U1Mean1Fit, fD1U1Mean2Fit, fD1U2Fit,fD1U2RMSSMFit,fD1U2RMS1Fit,fD1U2RMS2Fit,fD1U2RMS3Fit,fD1U2FracFit,fD1U2Mean1Fit, fD1U2Mean2Fit,iNameData, iNameData_key, "PF",RecoilCorrector::ZDATA, model_name
  );
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addMCFile(std::string iNameMC, std::string iNameMC_key, TString model_name)
{
  readRecoil(fM1U1Fit,fM1U1RMSSMFit,fM1U1RMS1Fit,fM1U1RMS2Fit,fM1U1RMS3Fit,fM1U1FracFit, fM1U1Mean1Fit, fM1U1Mean2Fit, fM1U2Fit,fM1U2RMSSMFit,fM1U2RMS1Fit,fM1U2RMS2Fit,fM1U2RMS3Fit,fM1U2FracFit,fM1U2Mean1Fit, fM1U2Mean2Fit,iNameMC,iNameMC_key, "PF",RecoilCorrector::ZMC, model_name
  );
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readRecoil(/* std::vector<double> &iSumEt, */
std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1RMS3Fit,
std::vector<TF1*> &iU1FracFit,std::vector<TF1*> &iU1Mean1Fit, std::vector<TF1*> &iU1Mean2Fit,//std::vector<TF1*> &iU1Sig3Fit,
std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2RMS3Fit,
std::vector<TF1*> &iU2FracFit,std::vector<TF1*> &iU2Mean1Fit, std::vector<TF1*> &iU2Mean2Fit,//std::vector<TF1*> &iU2Sig3Fit,
std::string iFName , std::string iFKeyName , std::string iPrefix, int mytype,
TString model_name
) {

  //type=1 read U1; type=2 read U2;
  cout << "inside readRecoil" << iFName.c_str() << endl;
  cout << "inside readRecoil Key " << iFKeyName.c_str() << endl;

  if(mytype==0) cout << " read target"   << endl;
  if(mytype==1) cout << " read DATA"  << endl;
  if(mytype==2) cout << " read MC"  << endl;

  TFile *lFile  = new TFile(iFName.c_str());
  TFile *lFileKeys;
  if (doKeys) lFileKeys = new TFile(iFKeyName.c_str());

  // now defined in the .h
  // const int rapbins = 2;

  cout << " rapbins " << rapbins << endl;

  for(int rapbin = 0; rapbin < rapbins; rapbin++) {
    cout << "reading bin " << rapbin << endl;
    int original_file_rapbin = rapbin+1;

    std::string lStr = iPrefix;

    std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10,pSS11,pSS12,pSS13,pSS14,pSS15,pSS16;

    pSS1  << lStr << "u1Mean_"    << original_file_rapbin;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[rapbin]->SetDirectory(0);              
    // cout << "pSS1 " << pSS1.str() << endl;
    // iU1Fit[0]->Print();
    pSS2  << lStr << "u1MeanRMS_" << original_file_rapbin;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);           
    pSS3  << lStr << "u1RMS1_"    << original_file_rapbin;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);     
    pSS4  << lStr << "u1RMS2_"    << original_file_rapbin;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);     
    pSS5  << lStr << "u1RMS3_"    << original_file_rapbin;  iU1RMS3Fit.push_back( (TF1*) lFile->FindObjectAny((pSS5.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);     
    //pSS5  << "u1Sig3_"    << original_file_rapbin;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[rapbin]->SetDirectory(0);         
    pSS6  << lStr << "u2Mean_"    << original_file_rapbin;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[rapbin]->SetDirectory(0);              
    pSS7  << lStr << "u2MeanRMS_" << original_file_rapbin;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[rapbin]->SetDirectory(0);           
    pSS8  << lStr << "u2RMS1_"    << original_file_rapbin;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[rapbin]->SetDirectory(0);     
    pSS9  << lStr << "u2RMS2_"    << original_file_rapbin;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[rapbin]->SetDirectory(0);     
    pSS10  << lStr << "u2RMS3_"    << original_file_rapbin;  iU2RMS3Fit.push_back( (TF1*) lFile->FindObjectAny((pSS10.str()).c_str())); //iU2RMSFit[rapbin]->SetDirectory(0);
    //pSS10 << "u2Sig3_"    << original_file_rapbin;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[rapbin]->SetDirectory(0);
    pSS11  << lStr << "u1Frac_"    << original_file_rapbin;  iU1FracFit.push_back( (TF1*) lFile->FindObjectAny((pSS11.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);     
    pSS12  << lStr << "u2Frac_"    << original_file_rapbin;  iU2FracFit.push_back( (TF1*) lFile->FindObjectAny((pSS12.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);           
    pSS13  << lStr << "u1Mean1_"    << original_file_rapbin;  iU1Mean1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS13.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);
    pSS14  << lStr << "u1Mean2_"    << original_file_rapbin;  iU1Mean2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS14.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);
    pSS15  << lStr << "u2Mean1_"    << original_file_rapbin;  iU2Mean1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS15.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);
    pSS16  << lStr << "u2Mean2_"    << original_file_rapbin;  iU2Mean2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS16.str()).c_str())); //iU1RMSFit[rapbin]->SetDirectory(0);

    wU1[mytype][rapbin] = new RooWorkspace("wU1","wU1");
    pdfU1[mytype][rapbin] = (RooAddPdf*) lFile->Get(Form("AddU1Y%d",original_file_rapbin));
    wU1[mytype][rapbin]->import(*pdfU1[mytype][rapbin],RooFit::Silence());
    frU1[mytype][rapbin] = (RooFitResult*) lFile->Get(Form("%sU1Y%d_Crapsky0_U1_2D",model_name.Data(),original_file_rapbin));
    // cout << "CALLING frU1[mytype][rapbin]->Print(\"V\")" << endl;
    // frU1[mytype][rapbin]->Print("V");
    // wU1[mytype][rapbin]->Print();
    runDiago(wU1[mytype][rapbin],frU1[mytype][rapbin],Form("AddU1Y%d",original_file_rapbin),pdfU1Cdf[mytype][rapbin]);

    wU1key[mytype][rapbin] = new RooWorkspace("wU1key","wU1key");
    //    cout << "reading recoilKeys " << endl;
    if (doKeys) makeKeysVec(wU1key[mytype][rapbin], lFileKeys, Form("Keys_U1_%d",original_file_rapbin), pdfKeyU1Cdf[mytype][rapbin],true);

    //    RooRealVar* myptU1=wU1[mytype][rapbin]->var("pt");
    //    myptU1->setVal(10);
    //    cout <<  "U1cdf=" << pdfU1Cdf[mytype][rapbin]->getVal()    << endl;

    //    delete pdfU1[mytype][rapbin];
    //    delete frU1[mytype][rapbin];

    wU2[mytype][rapbin] = new RooWorkspace("wU2","wU2");
    pdfU2[mytype][rapbin] = (RooAddPdf*) lFile->Get(Form("AddU2Y%d",original_file_rapbin));
    wU2[mytype][rapbin]->import(*pdfU2[mytype][rapbin],RooFit::Silence());
    frU2[mytype][rapbin] = (RooFitResult*) lFile->Get(Form("%sU2Y%d_Crapsky0_U2_2D",model_name.Data(),original_file_rapbin));
    // wU2[mytype][rapbin]->Print();
    // wU2diago[mytype][rapbin] = wU2[mytype][rapbin];
    runDiago(wU2[mytype][rapbin],frU2[mytype][rapbin],Form("AddU2Y%d",original_file_rapbin),pdfU2Cdf[mytype][rapbin]);
    
    wU2key[mytype][rapbin] = new RooWorkspace("wU2key","wU2key");
    if (doKeys) makeKeysVec(wU2key[mytype][rapbin], lFileKeys, Form("Keys_U2_%d",original_file_rapbin), pdfKeyU2Cdf[mytype][rapbin],false);

    //    RooRealVar* myptU2=wU2[mytype][rapbin]->var("pt");
    //    myptU2->setVal(10);
    //    cout <<  "U2cdf= " << pdfU2Cdf[mytype][rapbin]->getVal()    << endl;

    //    delete pdfU2[mytype][rapbin];
    //    delete frU2[mytype][rapbin];

  }

  //  cout << "read U1 size: " << iU1Fit.size() << endl;
  
  
  lFile->Close();
  // iSumEt.push_back(1000);
}

//-----------------------------------------------------------------------------------------------------------------------------------------

double RecoilCorrector::NonClosure_weight(double iMet,double iMPhi,double iGenPt,double iGenPhi,double iGenRap, double iLepPt,double iLepPhi) {
  
  // cout
  // << "iMet= " << iMet
  // << " iMPhi= " << iMPhi
  // << " iGenPt= " << iGenPt
  // << " iGenPhi= " << iGenPhi
  // << " iGenRap= " << iGenRap
  // << " iLepPt= " << iLepPt
  // << " iLepPhi= " << iLepPhi
  // << endl;
  
  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  double pU1   = pU*pCos;
  double pU2   = pU*pSin; 
  double abs_pU2   = TMath::Abs(pU2); // we use the abs, i.e. the mean rms, to reduce fluctuations 
  
  // cout << "pU1= " << pU1 << " abs_pU2= " << abs_pU2 << endl;
  
  double weight_NonClosure = 1;
  int rap_bin = 0; if(iGenRap>1.2) rap_bin=1;
 
 // if(iGenRap>1) rap_bin = 1 // for the moment we use only one rapidity bin, i.e. y1

 // weight_NonClosure *= hNonClosure[0][rap_bin]->GetBinContent(hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)); // u1
 if(hNonClosure[0][rap_bin]->GetBinContent(hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)!=0))
  weight_NonClosure /= hNonClosure[0][rap_bin]->GetBinContent(hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)); // u1
  // cout << "hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)= " << hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1) << endl;
  // cout << "weight_NonClosure after u1= " << weight_NonClosure;
  if(weight_NonClosure==0) weight_NonClosure=1;
  // cout << " ------->>> " << weight_NonClosure << endl;

  // weight_NonClosure *= hNonClosure[1][rap_bin]->GetBinContent(hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2)); // u2
  if(hNonClosure[1][rap_bin]->GetBinContent(hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2))!=0)
    weight_NonClosure /= hNonClosure[1][rap_bin]->GetBinContent(hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2)); // u2
  // cout << "hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2)= " << hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2) << endl;
  // cout << "weight_NonClosure after u2= " << weight_NonClosure;
  if(weight_NonClosure==0) weight_NonClosure=1;
  // cout << " ------->>> " << weight_NonClosure << endl;
  
  return 1; // I'm tired of getting a compiler warning
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::NonClosure_scale(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,double iGenRap, double iLepPt,double iLepPhi) {
  
  // cout
  // << "iMet= " << iMet
  // << " iMPhi= " << iMPhi
  // << " iGenPt= " << iGenPt
  // << " iGenPhi= " << iGenPhi
  // << " iGenRap= " << iGenRap
  // << " iLepPt= " << iLepPt
  // << " iLepPhi= " << iLepPhi
  // << endl;
  
  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  double pU1   = pU*pCos;
  double pU2   = pU*pSin; 
  double abs_pU2   = TMath::Abs(pU2); // we use the abs, i.e. the mean rms, to reduce fluctuations 
  
  // cout << "pU1= " << pU1 << " abs_pU2= " << abs_pU2 << endl;
  
  int rap_bin = 0;
 
  // if(iGenRap>1) rap_bin = 1 // for the moment we use only one rapidity bin, i.e. y1

  if(hNonClosure[0][rap_bin]->GetBinContent(hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)!=0))
    pU1 /= hNonClosure[0][rap_bin]->GetBinContent(hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)); // u1
  // cout << "hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1)= " << hNonClosure[0][rap_bin]->FindBin(iGenPt,pU1) << endl;

  if(hNonClosure[1][rap_bin]->GetBinContent(hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2))!=0)
    pU2 /= hNonClosure[1][rap_bin]->GetBinContent(hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2)); // u2
  // cout << "hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2)= " << hNonClosure[1][rap_bin]->FindBin(iGenPt,abs_pU2) << endl;
  
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  
  // cout 
  // << "iMet= " << iMet
  // << " iMPhi= " << iMPhi
  // << " after rescaling "
  // << endl;
  
  return 1; // I'm tired of getting a compiler warning
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
void RecoilCorrector::CorrectMET3gaus(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double &iU1,double &iU2,int RecoilCorrVarDiagoParU1orU2fromDATAorMC,int RecoilCorrVarDiagoParN,int RecoilCorrVarDiagoParSigmas,int rapbin, int recoilCorrSigmas, int mytype, bool key) {

  RecoilCorrector::doKeys=key;

  // ---------------------------
  // CHANGE STAT EIGEN IF NEEDED
  // ---------------------------
  
  // cout 
    // << "applyCorrMET3gausPDF: RecoilCorrVarDiagoParU1orU2fromDATAorMC="<<RecoilCorrVarDiagoParU1orU2fromDATAorMC
    // << " RecoilCorrVarDiagoParN="<<RecoilCorrVarDiagoParN
    // << " RecoilCorrVarDiagoParSigmas="<<RecoilCorrVarDiagoParSigmas
    // <<endl;

  uint DataOrMcMap[] = {0, ZDATA, ZDATA, ZDATA, ZMC, ZMC, ZMC};
  uint DataOrMc = DataOrMcMap[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
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
  
  applyCorrMET3gausPDF(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,
  fF1U1Fit[rapbin],
  fD1U1Fit[rapbin],  fM1U1Fit[rapbin],
  fD1U1RMSSMFit[rapbin], fM1U1RMSSMFit[rapbin], 
  // fD1U1RMS1Fit[rapbin], fM1U1RMS1Fit[rapbin], 
  // fD1U1RMS2Fit[rapbin], fM1U1RMS2Fit[rapbin], 
  // fD1U1RMS3Fit[rapbin], fM1U1RMS3Fit[rapbin],
  // fD1U1FracFit[rapbin], fM1U1FracFit[rapbin],
  // fD1U1Mean1Fit[rapbin], fM1U1Mean1Fit[rapbin],
  // fD1U1Mean2Fit[rapbin], fM1U1Mean2Fit[rapbin],
  //
  // fD1U2Fit[rapbin], fM1U2Fit[rapbin],
  fD1U2RMSSMFit[rapbin], fM1U2RMSSMFit[rapbin],        
  // fD1U2RMS1Fit[rapbin], fM1U2RMS1Fit[rapbin],
  // fD1U2RMS2Fit[rapbin], fM1U2RMS2Fit[rapbin],
  // fD1U2RMS3Fit[rapbin], fM1U2RMS3Fit[rapbin],
  // fD1U2FracFit[rapbin], fM1U2FracFit[rapbin],
  // fD1U2Mean1Fit[rapbin], fM1U2Mean1Fit[rapbin],
  // fD1U2Mean2Fit[rapbin], fM1U2Mean2Fit[rapbin],

  mytype, rapbin,
  recoilCorrSigmas,
  iU1,iU2
  );
  
}


//-----------------------------------------------------------------------------------------------------------------------------------------
// RooAddPdf* pdfMCU1;
// RooAddPdf* pdfMCU2;
// RooAddPdf* pdfDATAU1;
// RooAddPdf* pdfDATAU2;

// RooWorkspace *wMCU1; 
// RooWorkspace *wMCU2; 
// RooWorkspace *wDATAU1; 
// RooWorkspace *wDATAU2; 
// NEW WITH PDFs
void RecoilCorrector::applyCorrMET3gausPDF(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
double iLepPt,double iLepPhi,/*TRandom3 *iRand,*/
TF1 *iU1Default,
TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit,
TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
//                     RooAddPdf* pdfMCU1, RooAddPdf* pdfDATAU1, 
//                     RooAddPdf* pdfMCU2, RooAddPdf* pdfDATAU2 
int mytype, int rapbin,
int nSigmas,
double &pU1,double &pU2
) {

  double lRescale  = sqrt((TMath::Pi())/2.);  // Magic normalization number
  //  double lRescale  = 1;     // for squares

  double pDefU1    = iU1Default->Eval(iGenPt);

  double pDU1       = iU1RZDatFit ->Eval(iGenPt); // U1 average scale
  double pDRMSU1    = iU1MSZDatFit->Eval(iGenPt)*lRescale; // U1 average RMS
  double pDRMSU2    = iU2MSZDatFit->Eval(iGenPt)*lRescale; // U2 average RMS

  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMRMSU1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMRMSU2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;

  //
  // ENDING of the PARAMETERS
  //

  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  // Get real values of recoil (from the pull space vars)
  double pU1noCorr = pU*pCos;
  double pU2noCorr = pU*pSin;

  // cout << " ------------------------------------------------------- " << endl;
  // cout << " initial pU1 = " << pU1noCorr << " pU2 = " << pU2noCorr << endl;

  // go to the pull space (we are on MC)
  // ????? what to do for the scale ??????
  
  // cout 
    // << "before triGausInvGraphPDF"
    // << " pdfU1Cdf[ZMC]["<<rapbin<<"]->getVal()= " << pdfU1Cdf[ZMC][rapbin]->getVal()
    // << " pdfU1Cdf[ZDATA]["<<rapbin<<"]->getVal()= " << pdfU1Cdf[ZDATA][rapbin]->getVal()
    // << " pdfU2Cdf[ZMC]["<<rapbin<<"]->getVal()= " << pdfU2Cdf[ZMC][rapbin]->getVal()
    // << " pdfU2Cdf[ZDATA]["<<rapbin<<"]->getVal()= " << pdfU2Cdf[ZDATA][rapbin]->getVal()
    // << endl;
  pdfU1Cdf[ZMC][rapbin]->getVal();
  pdfU1Cdf[ZDATA][rapbin]->getVal();
  pdfU2Cdf[ZMC][rapbin]->getVal();
  pdfU2Cdf[ZDATA][rapbin]->getVal();

  // Will contain corrected values of U1 and U2
  double pU1corr;
  double pU2corr;

  bool doAbsolute=true;

  if(doKeys && doAbsolute) {
    // triGausInvGraphKeys
    // this need the absolute space
    pU1corr = triGausInvGraphKeys(pU1noCorr,iGenPt,pdfKeyU1Cdf[ZMC][rapbin],pdfKeyU1Cdf[ZDATA][rapbin],wU1key[ZMC][rapbin],wU1key[ZDATA][rapbin],true, 50);
    pU2corr = triGausInvGraphKeys(pU2noCorr,iGenPt,pdfKeyU2Cdf[ZMC][rapbin],pdfKeyU2Cdf[ZDATA][rapbin],wU2key[ZMC][rapbin],wU2key[ZDATA][rapbin],false, 50);

  } else {
    // Relative for all
    double pU1Diff  = pU1noCorr-pDefU1;
    double pU2Diff  = pU2noCorr;
    
    pU1Diff = pU1Diff/pMRMSU1;
    pU2Diff = pU2Diff/pMRMSU2;
    
    //  std::cout << "================= " << std::endl;
    //  std::cout << " Before: pU1Diff " << pU1Diff << " pU2Diff " << pU2Diff << std::endl;
    
    if(doKeys) {
      // this need the relative space
      pU1corr = triGausInvGraphKeys(pU1Diff,iGenPt,pdfKeyU1Cdf[ZMC][rapbin],pdfKeyU1Cdf[ZDATA][rapbin],wU1key[ZMC][rapbin],wU1key[ZDATA][rapbin],true,5);
      pU2corr = triGausInvGraphKeys(pU2Diff,iGenPt,pdfKeyU2Cdf[ZMC][rapbin],pdfKeyU2Cdf[ZDATA][rapbin],wU2key[ZMC][rapbin],wU2key[ZDATA][rapbin],false,5);
    } else {
      // cout << "triGausInvGraphPDF U1" << endl;
      // this need the reduced space
      pU1corr = triGausInvGraphPDF(pU1Diff,iGenPt,pdfU1Cdf[ZMC][rapbin],pdfU1Cdf[ZDATA][rapbin],wU1[ZMC][rapbin],wU1[ZDATA][rapbin],5);
      pU2corr = triGausInvGraphPDF(pU2Diff,iGenPt,pdfU2Cdf[ZMC][rapbin],pdfU2Cdf[ZDATA][rapbin],wU2[ZMC][rapbin],wU2[ZDATA][rapbin],5);
    }
    
    // Go back to the absolute space (from pull)
    pU1corr *= pDRMSU1;
    pDefU1 *= (pDU1/pMU1);
    pU1corr = pDefU1 + pU1corr;

    pU2corr *= pDRMSU2;

  }
  
  double pU1delta = pU1corr - pU1noCorr;
  double pU2delta = pU2corr - pU2noCorr;
  
  pU1 = pU1noCorr + nSigmas * pU1delta;
  pU2 = pU2noCorr + nSigmas * pU2delta;

  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);

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

double RecoilCorrector::triGausInvGraphKeys(double iPVal, double Zpt, std::vector<RooAbsReal*> pdfKeyMCcdf, std::vector<RooAbsReal*> pdfKeyDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA, bool isU1, double max) {

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

  //  lFileKeys->ls();

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
