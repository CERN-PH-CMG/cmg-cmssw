#include "RecoilCorrector.h"

// mytype: 0 = target file , 1 = DATA , 2 = Z MC

RecoilCorrector::RecoilCorrector(string iNameZ, int iSeed) {

  fRandom = new TRandom3(iSeed);
  readRecoil(fF1U1Fit,fF1U1RMSSMFit,fF1U1RMS1Fit,fF1U1RMS2Fit,fF1U1RMS3Fit,fF1U1FracFit, fF1U1Mean1Fit, fF1U1Mean2Fit, fF1U2Fit,fF1U2RMSSMFit,fF1U2RMS1Fit,fF1U2RMS2Fit,fF1U2RMS3Fit,fF1U2FracFit,fF1U2Mean1Fit, fF1U2Mean2Fit,iNameZ,"PF",1,0);  
  
  fId = 0; fJet = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addDataFile(std::string iNameData) { 
  readRecoil(fD1U1Fit,fD1U1RMSSMFit,fD1U1RMS1Fit,fD1U1RMS2Fit,fD1U1RMS3Fit,fD1U1FracFit, fD1U1Mean1Fit, fD1U1Mean2Fit, fD1U2Fit,fD1U2RMSSMFit,fD1U2RMS1Fit,fD1U2RMS2Fit,fD1U2RMS3Fit,fD1U2FracFit,fD1U2Mean1Fit, fD1U2Mean2Fit,iNameData,"PF",1,1);  
  fId++;   
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::addMCFile  (std::string iNameMC) { 
  fId++;
  readRecoil(fM1U1Fit,fM1U1RMSSMFit,fM1U1RMS1Fit,fM1U1RMS2Fit,fM1U1RMS3Fit,fM1U1FracFit, fM1U1Mean1Fit, fM1U1Mean2Fit, fM1U2Fit,fM1U2RMSSMFit,fM1U2RMS1Fit,fM1U2RMS2Fit,fM1U2RMS3Fit,fM1U2FracFit,fM1U2Mean1Fit, fM1U2Mean2Fit,iNameMC,"PF",1,2);  
  
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::readRecoil(/* std::vector<double> &iSumEt, */
std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,
std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1RMS3Fit,
std::vector<TF1*> &iU1FracFit,std::vector<TF1*> &iU1Mean1Fit, std::vector<TF1*> &iU1Mean2Fit,//std::vector<TF1*> &iU1Sig3Fit,
std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,
std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2RMS3Fit,
std::vector<TF1*> &iU2FracFit,std::vector<TF1*> &iU2Mean1Fit, std::vector<TF1*> &iU2Mean2Fit,//std::vector<TF1*> &iU2Sig3Fit,
std::string iFName = "recoilfit.root",std::string iPrefix="",int vtxBin=-1, int mytype=0) {

  //type=1 read U1; type=2 read U2;
  cout << "inside readRecoil" << endl;

  TFile *lFile  = new TFile(iFName.c_str());
  // lFile->ls();

  // now defined in the .h
  // const int lNBins = 2;                   

  int init = 1; // this is for the binned
  
  cout << "init " << init<< " lNBins " << lNBins<<" vtxBin " << vtxBin<< endl;

  for(int i0 = init; i0 <= lNBins; i0++) {
    // if(i0!=vtxBin) continue;
    cout << "reading bin " << i0 << endl;

    std::string lStr = iPrefix;
    //iSumEt.push_back(lGraph->GetY()[i0]);                                                                                                                     

    std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10,pSS11,pSS12,pSS13,pSS14,pSS15,pSS16;

    pSS1  << lStr << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny((pSS1.str()).c_str())); //iU1Fit[i0]->SetDirectory(0);              
    // cout << "pSS1 " << pSS1.str() << endl;
    // iU1Fit[0]->Print();
    pSS2  << lStr << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS2.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
    pSS3  << lStr << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS3.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
    pSS4  << lStr << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS4.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
    pSS5  << lStr << "u1RMS3_"    << i0;  iU1RMS3Fit.push_back( (TF1*) lFile->FindObjectAny((pSS5.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
    //pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS5.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);         
    pSS6  << lStr << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny((pSS6.str()).c_str())); //iU2Fit[i0]->SetDirectory(0);              
    pSS7  << lStr << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny((pSS7.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);           
    pSS8  << lStr << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS8.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
    pSS9  << lStr << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS9.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);     
    pSS10  << lStr << "u2RMS3_"    << i0;  iU2RMS3Fit.push_back( (TF1*) lFile->FindObjectAny((pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    //pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny((iPrefix+pSS10.str()).c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS11  << lStr << "u1Frac_"    << i0;  iU1FracFit.push_back( (TF1*) lFile->FindObjectAny((pSS11.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);     
    pSS12  << lStr << "u2Frac_"    << i0;  iU2FracFit.push_back( (TF1*) lFile->FindObjectAny((pSS12.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);           
    pSS13  << lStr << "u1Mean1_"    << i0;  iU1Mean1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS13.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS14  << lStr << "u1Mean2_"    << i0;  iU1Mean2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS14.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS15  << lStr << "u2Mean1_"    << i0;  iU2Mean1Fit.push_back( (TF1*) lFile->FindObjectAny((pSS15.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS16  << lStr << "u2Mean2_"    << i0;  iU2Mean2Fit.push_back( (TF1*) lFile->FindObjectAny((pSS16.str()).c_str())); //iU1RMSFit[i0]->SetDirectory(0);

    wU1[mytype][i0] = new RooWorkspace("wU1","wU1");
    pdfU1[mytype][i0] = (RooAddPdf*) lFile->Get(Form("AddU1Y%d",i0));
    wU1[mytype][i0]->import(*pdfU1[mytype][i0],RooFit::Silence());
    frU1[mytype][i0] = (RooFitResult*) lFile->Get(Form("fitresult_AddU1Y%d_Crapsky0_U1_2D",i0));
    // cout << "CALLING frU1[mytype][i0]->Print(\"V\")" << endl;
    // frU1[mytype][i0]->Print("V");
    // wU1[mytype][i0]->Print();
    runDiago(wU1[mytype][i0],frU1[mytype][i0],Form("AddU1Y%d",i0),pdfU1Cdf[mytype][i0]);
    
    wU2[mytype][i0] = new RooWorkspace("wU2","wU2");
    pdfU2[mytype][i0] = (RooAddPdf*) lFile->Get(Form("AddU2Y%d",i0));
    wU2[mytype][i0]->import(*pdfU2[mytype][i0],RooFit::Silence());
    frU2[mytype][i0] = (RooFitResult*) lFile->Get(Form("fitresult_AddU2Y%d_Crapsky0_U2_2D",i0));
    // wU2[mytype][i0]->Print();
    // wU2diago[mytype][i0] = wU2[mytype][i0];
    runDiago(wU2[mytype][i0],frU2[mytype][i0],Form("AddU2Y%d",i0),pdfU2Cdf[mytype][i0]);
    
  }

  cout << "read U1 size: " << iU1Fit.size() << endl;
  
  
  lFile->Close();
  // iSumEt.push_back(1000);
  // return lNBins;
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
//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::CorrectMET3gaus(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi,double &iU1,double &iU2,double iFlucU2,double iFlucU1,double iScale,int njet, bool doSingleGauss, int mytype) {

  // cout << "TYPE2: nVTX " << njet << " function size "<< fD1U1Fit.size() << endl;
  fJet = njet; 

  if(fJet >= int(fF1U1Fit.size())) fJet = int(fF1U1Fit.size()) - 1; 

  // applyCorrMET3gaus(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,
  // fF1U1Fit[fJet],
  // fD1U1Fit[fJet],  fM1U1Fit[fJet],
  // fD1U1RMSSMFit[fJet], fM1U1RMSSMFit[fJet], 
  // fD1U1RMS1Fit[fJet], fM1U1RMS1Fit[fJet], 
  // fD1U1RMS2Fit[fJet], fM1U1RMS2Fit[fJet], 
  // fD1U1RMS3Fit[fJet], fM1U1RMS3Fit[fJet],
  // fD1U1FracFit[fJet], fM1U1FracFit[fJet],
  // fD1U1Mean1Fit[fJet], fM1U1Mean1Fit[fJet],
  // fD1U1Mean2Fit[fJet], fM1U1Mean2Fit[fJet],
  // //
  // // fD1U2Fit[fJet], fM1U2Fit[fJet],
  // fD1U2RMSSMFit[fJet], fM1U2RMSSMFit[fJet],        
  // fD1U2RMS1Fit[fJet], fM1U2RMS1Fit[fJet],
  // fD1U2RMS2Fit[fJet], fM1U2RMS2Fit[fJet],
  // fD1U2RMS3Fit[fJet], fM1U2RMS3Fit[fJet],
  // fD1U2FracFit[fJet], fM1U2FracFit[fJet],
  // fD1U2Mean1Fit[fJet], fM1U2Mean1Fit[fJet],
  // fD1U2Mean2Fit[fJet], fM1U2Mean2Fit[fJet],

  // // iU1,iU2,
  // iFlucU2,iFlucU1,iScale, mytype
  // // , doSingleGauss 
  // );
  
  applyCorrMET3gausPDF(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,
  fF1U1Fit[fJet],
  fD1U1Fit[fJet],  fM1U1Fit[fJet],
  fD1U1RMSSMFit[fJet], fM1U1RMSSMFit[fJet], 
  // fD1U1RMS1Fit[fJet], fM1U1RMS1Fit[fJet], 
  // fD1U1RMS2Fit[fJet], fM1U1RMS2Fit[fJet], 
  // fD1U1RMS3Fit[fJet], fM1U1RMS3Fit[fJet],
  // fD1U1FracFit[fJet], fM1U1FracFit[fJet],
  // fD1U1Mean1Fit[fJet], fM1U1Mean1Fit[fJet],
  // fD1U1Mean2Fit[fJet], fM1U1Mean2Fit[fJet],
  //
  // fD1U2Fit[fJet], fM1U2Fit[fJet],
  fD1U2RMSSMFit[fJet], fM1U2RMSSMFit[fJet],        
  // fD1U2RMS1Fit[fJet], fM1U2RMS1Fit[fJet],
  // fD1U2RMS2Fit[fJet], fM1U2RMS2Fit[fJet],
  // fD1U2RMS3Fit[fJet], fM1U2RMS3Fit[fJet],
  // fD1U2FracFit[fJet], fM1U2FracFit[fJet],
  // fD1U2Mean1Fit[fJet], fM1U2Mean1Fit[fJet],
  // fD1U2Mean2Fit[fJet], fM1U2Mean2Fit[fJet],

  // iU1,iU2,
  iFlucU2,iFlucU1,iScale, mytype, fJet
  // , doSingleGauss 
  );
  
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::applyCorrMET3gaus(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
double iLepPt,double iLepPhi,/*TRandom3 *iRand,*/
TF1 *iU1Default,
TF1 *iU1RZDatFit,  TF1 *iU1RZMCFit,
TF1 *iU1MSZDatFit, TF1 *iU1MSZMCFit, 
TF1 *iU1S1ZDatFit, TF1 *iU1S1ZMCFit, 
TF1 *iU1S2ZDatFit, TF1 *iU1S2ZMCFit, 
TF1 *iU1S3ZDatFit, TF1 *iU1S3ZMCFit,
TF1 *iU1fracZDatFit, TF1 *iU1fracZMCFit,
TF1 *iU1mean1ZDatFit, TF1 *iU1mean1ZMCFit,
TF1 *iU1mean2ZDatFit, TF1 *iU1mean2ZMCFit,
//
TF1 *iU2MSZDatFit, TF1 *iU2MSZMCFit,
TF1 *iU2S1ZDatFit, TF1 *iU2S1ZMCFit,        
TF1 *iU2S2ZDatFit, TF1 *iU2S2ZMCFit,
TF1 *iU2S3ZDatFit, TF1 *iU2S3ZMCFit,
TF1 *iU2fracZDatFit, TF1 *iU2fracZMCFit,
TF1 *iU2mean1ZDatFit, TF1 *iU2mean1ZMCFit,
TF1 *iU2mean2ZDatFit, TF1 *iU2mean2ZMCFit,
double iFlucU2,double iFlucU1,double iScale, int mytype
){

  //  cout << "inside metType2 " << endl;
  //  bool doAbsolute=true;

  //  bool doSingleGauss=true;
  bool dodebug=false;
  bool doIterativeMet = false;
  bool invGraph = true;
  bool doSingleGauss = false;
  bool doTriGauss = true;
  bool doApplyCorr = false; // smearing along the recoil vector
  bool doOnlyU1 = false;
  bool doOnlyU2 = true;
  bool writeTree = false;
  bool doClosure = false;

  double lRescale  = sqrt((TMath::Pi())/2.);     
  double pDefU1,pDU1,pDRMSU1;
  double pDSigmaU1_1,pDSigmaU1_2,pDSigmaU1_3;
  double pDRMSU2;
  double pDSigmaU2_1,pDSigmaU2_2,pDSigmaU2_3;
  double pMU1;
  double pMRMSU1;
  double pMSigmaU1_1,pMSigmaU1_2,pMSigmaU1_3;
  double pMRMSU2;
  double pMSigmaU2_1,pMSigmaU2_2,pMSigmaU2_3;
  double meanM1U1,meanM2U1,meanM1U2,meanM2U2;
  double meanD1U1,meanD2U1,meanD1U2,meanD2U2;

  pDefU1    = iU1Default->Eval(iGenPt);
  if(iScale==0 || mytype!=1){
    pDU1       = iU1RZDatFit ->Eval(iGenPt);
  }
  if(iFlucU1==0 || mytype!=1){
    pDRMSU1    = iU1MSZDatFit->Eval(iGenPt)*lRescale;
    pDSigmaU1_1 = iU1S1ZDatFit->Eval(iGenPt)*pDRMSU1;
    pDSigmaU1_2 = iU1S2ZDatFit->Eval(iGenPt)*pDRMSU1;
    pDSigmaU1_3 = 0;
    if(iU1S3ZDatFit) pDSigmaU1_3 = iU1S3ZDatFit->Eval(iGenPt)*pDRMSU1;
    meanD1U1=iU1mean1ZDatFit->Eval(iGenPt)*pDRMSU1;
    meanD2U1=iU1mean2ZDatFit->Eval(iGenPt)*pDRMSU1;
  }
  if(iFlucU2==0 || mytype!=1){
    pDRMSU2    = iU2MSZDatFit->Eval(iGenPt)*lRescale;
    // RooFormulaVar::mean[ actualVars=(Amean1,Bmean1,Cmean1,pt) formula="@0+@1*@3+@2*@3*@3" ] = 0
    // RooFormulaVar::sigma1[ actualVars=(a1sig,b1sig,c1sig,pt,d1sig) formula="@0+@1*@3+@2*@3*@3+@4*@3*@3*@3" ] = 0.356981
    pDSigmaU2_1 = iU2S1ZDatFit->Eval(iGenPt)*pDRMSU2;
    pDSigmaU2_2 = iU2S2ZDatFit->Eval(iGenPt)*pDRMSU2;
    pDSigmaU2_3 = 0;
    if(iU2S3ZDatFit) pDSigmaU2_3 = iU2S3ZDatFit->Eval(iGenPt)*pDRMSU2;
    meanD1U2=iU2mean1ZDatFit->Eval(iGenPt)*pDRMSU2;
    meanD2U2=iU2mean2ZDatFit->Eval(iGenPt)*pDRMSU2;
  }
  if(iScale==0 || mytype!=2){
    pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  }
  if(iFlucU1==0 || mytype!=2){
    pMRMSU1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
    pMSigmaU1_1 = iU1S1ZMCFit ->Eval(iGenPt)*pMRMSU1;
    pMSigmaU1_2 = iU1S2ZMCFit ->Eval(iGenPt)*pMRMSU1;
    pMSigmaU1_3 = 0;
    if(iU1S3ZMCFit) pMSigmaU1_3 = iU1S3ZMCFit ->Eval(iGenPt)*pMRMSU1;
    meanM1U1=iU1mean1ZMCFit->Eval(iGenPt)*pMRMSU1;
    meanM2U1=iU1mean2ZMCFit->Eval(iGenPt)*pMRMSU1;
  }
  if(iFlucU2==0 || mytype!=2){
    pMRMSU2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;
    // RooFormulaVar::mean[ actualVars=(Amean1,Bmean1,Cmean1,pt) formula="@0+@1*@3+@2*@3*@3" ] = 0
    // RooFormulaVar::sigma1[ actualVars=(a1sig,b1sig,c1sig,pt,d1sig) formula="@0+@1*@3+@2*@3*@3+@4*@3*@3*@3" ] = 0.356981
    pMSigmaU2_1 = iU2S1ZMCFit ->Eval(iGenPt)*pMRMSU2;
    pMSigmaU2_2 = iU2S2ZMCFit ->Eval(iGenPt)*pMRMSU2;
    pMSigmaU2_3 = 0;
    if(iU2S3ZMCFit) pMSigmaU2_3 = iU2S3ZMCFit ->Eval(iGenPt)*pMRMSU2;
    meanM1U2=iU2mean1ZMCFit->Eval(iGenPt)*pMRMSU2;
    meanM2U2=iU2mean2ZMCFit->Eval(iGenPt)*pMRMSU2;
  }
  
  // // else{
  // cout << " -------- pdf ------- " << endl;
  // RooAddPdf*pdfU2_old = (RooAddPdf*)wU2[mytype]->pdf("AddU2");
  // // pdfU2_old->Print();
  // // pdfU2[mytype]->Print();
  // RooArgSet* compsU2 = pdfU2_old->getComponents() ;
  // // compsU2->Print();

  // cout << " -------- print sigmas ------ " << endl;
  // p.d.f.s
  // -------
  // RooAddPdf::AddU2[ frac1 * gaus1 + AddU2_recursive_fraction_gaus2 * gaus2 + AddU2_recursive_fraction_gaus3 * gaus3 ] = 0.594558
  // RooAddPdf::AddU2_eig[ frac1_eig * gaus1_eig + AddU2_recursive_fraction_gaus2_eig * gaus2_eig + AddU2_recursive_fraction_gaus3_eig * gaus3_eig ] = 0.594559
  // RooGaussian::gaus1[ x=XVar mean=mean sigma=sigma1 ] = 0.133782
  // RooGaussian::gaus1_eig[ x=XVar mean=mean sigma=sigma1_eig ] = 0.133783
  // RooGaussian::gaus2[ x=XVar mean=mean1 sigma=sigma2 ] = 0.644544
  // RooGaussian::gaus2_eig[ x=XVar mean=mean1 sigma=sigma2_eig ] = 0.644544
  // RooGaussian::gaus3[ x=XVar mean=mean1 sigma=sigma3 ] = 0.865853
  // RooGaussian::gaus3_eig[ x=XVar mean=mean1 sigma=sigma3_eig ] = 0.865852
  // RooAbsArg* frac1_ = compsU2->find("frac") ;
  // frac1_->Print();
  // frac1_ = compsU2->find("frac1") ;
  // frac1_->Print();
  // RooAbsPdf *frac2_ = compsU2->find("gaus1") ;
  // frac2_->Print();
  // frac1_ = compsU2->find("mean") ;
  // frac1_->Print();
  // frac1_ = compsU2->find("sigma1") ;
  // frac1_->Print();
  // // frac1_ = compsU2->find("AddU2_recursive_fraction_gaus2") ;
  // // frac1_->Print();
  // frac1_ = compsU2->find("mean1") ;
  // frac1_->Print();
  // frac1_ = compsU2->find("sigma2") ;
  // frac1_->Print();
  // // frac1_ = compsU2->find("AddU2_recursive_fraction_gaus3") ;
  // // frac1_->Print();
  // // frac1_ = compsU2->find("mean1") ;
  // // frac1_->Print();
  // frac1_ = compsU2->find("sigma3") ;
  // frac1_->Print();
  // // RooArgSet* frac1Var1 = frac1_->getVariables() ;
  // // cout << " print frac1_ " << endl;
  // // cout << " print frac1Var1 " << endl;
  // // frac1Var1->Print();
  // // frac1Var1->Print() ;
  
  // RooFormulaVar::frac1[ actualVars=(AFrac,BFrac,CFrac,pt,DFrac) formula="@0+@1*@3+@2*@3*@3+@4*@3*@3*@3" ] = 0.198816

  // RooFormulaVar::mean1[ actualVars=(Amean2,Bmean2,Cmean2,pt) formula="@0+@1*@3+@2*@3*@3" ] = 0
  // RooFormulaVar::sigma2[ actualVars=(a2sig,b2sig,c2sig,pt,d2sig) formula="@0+@1*@3+@2*@3*@3+@4*@3*@3*@3" ] = 0.888454
  // RooFormulaVar::sigma3[ actualVars=(a3sig,b3sig,c3sig,pt,d3sig) formula="@0+@1*@3+@2*@3*@3+@4*@3*@3*@3" ] = 1.71202


  // double myZpt=10;
  // double sigma1=frac1Var1->getRealValue("a1sig") + frac1Var1->getRealValue("b1sig")*myZpt + frac1Var1->getRealValue("c1sig")*myZpt*myZpt;

  // cout << "evaluated sigma1=" << sigma1 << " at Zpt" << myZpt<< endl;  

  // cout << " -------- new pdf ------ " << endl;
  // RooAddPdf*pdfU2_new = (RooAddPdf*)wU2[mytype]->pdf("AddU2_eig");
  // // pdfU2_new->Print();
  // RooArgSet* compsU2new = pdfU2_new->getComponents() ;
  // // compsU2new->Print();

  // cout << " -------- print sigmas ------ " << endl;
  // frac1_ = compsU2new->find("sigma1") ;
  // frac1Var1 = frac1_->getVariables() ;
  // frac1Var1->Print() ;

  // myZpt=10;
  // sigma1=frac1Var1->getRealValue("a1sig") + frac1Var1->getRealValue("b1sig")*myZpt + frac1Var1->getRealValue("c1sig")*myZpt*myZpt;

  // cout << "evaluated sigma1=" << sigma1 << " at Zpt" << myZpt<< endl;  
  
  // wU2[mytype]->Print();
  // }
  
  ///
  /// ENDING of the PARAMETERS
  ///
  ///

  //double pMMean1    = pMFrac1;
  //double pMMean2    = pMFrac2;

  // this for two gaussian
  double pDFracU1 = getFrac2gauss(pDRMSU1, pDSigmaU1_1, pDSigmaU1_2);
  double pDFracU2 = getFrac2gauss(pDRMSU2, pDSigmaU2_1, pDSigmaU2_2);
  double pMFracU1 = getFrac2gauss(pMRMSU1, pMSigmaU1_1, pMSigmaU1_2);
  double pMFracU2 = getFrac2gauss(pMRMSU2, pMSigmaU2_1, pMSigmaU2_2);

  // histoU2origFrac.Fill(pMFracU2);
  // histoU2wishFrac.Fill(pDFracU2);

  // if(iGenPt>0 && iGenPt<=5) histoU2origFrac05.Fill(pMFracU2);
  // if(iGenPt>0 && iGenPt<=5) histoU2wishFrac05.Fill(pDFracU2);
  // if(iGenPt>5 && iGenPt<=10) histoU2origFrac510.Fill(pMFracU2);
  // if(iGenPt>5 && iGenPt<=10) histoU2wishFrac510.Fill(pDFracU2);
  // if(iGenPt>10 && iGenPt<=15) histoU2origFrac1015.Fill(pMFracU2);
  // if(iGenPt>10 && iGenPt<=15) histoU2wishFrac1015.Fill(pDFracU2);
  // if(iGenPt>15 && iGenPt<=20) histoU2origFrac1520.Fill(pMFracU2);
  // if(iGenPt>15 && iGenPt<=20) histoU2wishFrac1520.Fill(pDFracU2);

  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  /////
  
  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-pDefU1;
  double pU2Diff  = pU2;

  // histoU1original.Fill(pU1);
  // histoU2original.Fill(pU2);

  if(dodebug) cout << " initial pU1 = " << pU1 << " pU2 = " << pU2 << endl;

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);
  /////  double pTU1Diff        = pU1Diff;
  
  //  cout << "------------"<< endl;
  //  cout << "diGAUSS "<< endl;
  //  cout << "  pU1ValM " << pU1ValM  << " pU1ValD " << pU1ValD  <<endl;
  //  cout << "  pU2ValM " << pU2ValM  << " pU2ValD " << pU2ValD  <<endl;
  double pU1ValMtest = 0;
  double pU2ValMtest = 0;

  double pU1ValM = 0;
  double pU2ValM = 0 ;
  double pU1ValD = 0 ;
  double pU2ValD = 0;

  //  pU2ValM         = diGausPVal(fabs(pU2Diff),1,iU2MSZMCFit ->Eval(iGenPt)*lRescale,0);
  //  pU2ValD         = oneGausPInverse(pU2ValM  ,1,iU2MSZDatFit->Eval(iGenPt)*lRescale,0); 

  //  cout << "pMFrac1=" << pMFrac1 << " pMSigma1_1=" << pMSigma1_1 << " pMSigma1_2=" << pMSigma1_2 << "MEAN2=" << iU1MSZMCFit ->Eval(iGenPt)*lRescale<< endl;
  //  cout  << "--------------------------" << endl;

  //  cout << "pU1Diff " << pU1Diff << "pU2Diff " << pU2Diff << endl;

  //  cout << "doing graph inversion " << endl;

  ///$$$$$
  ///$$$$$ Graph inversion
  ///$$$$$

  if(doTriGauss) {
    
    if(doOnlyU1 && !doOnlyU2) {
      pU1ValD         = triGausInvGraph(fabs(pU1Diff), /**/ pMRMSU1, meanM1U1, meanM2U1, iU1fracZMCFit->Eval(iGenPt), pMSigmaU1_1, pMSigmaU1_2, pMSigmaU1_3, /**/ pDRMSU1, meanD1U1, meanD2U1, iU1fracZDatFit->Eval(iGenPt), pDSigmaU1_1, pDSigmaU1_2, pDSigmaU1_3);
      pU2ValD = fabs(pU2Diff);
    }
    if(!doOnlyU1 && doOnlyU2) {
      //	cout << "Zpt " << iGenPt << endl;
      pU1ValD = fabs(pU1Diff);
      pU2ValD         = triGausInvGraph(fabs(pU2Diff), /**/ pMRMSU2, meanM1U2, meanM2U2, iU2fracZMCFit->Eval(iGenPt), pMSigmaU2_1, pMSigmaU2_2, pMSigmaU2_3, /**/ pDRMSU2, meanD1U2, meanD2U2, iU2fracZDatFit->Eval(iGenPt), pDSigmaU2_1, pDSigmaU2_2, pDSigmaU2_3);
    }

  } else if(!doSingleGauss && !doTriGauss) {

    if(doOnlyU1 && !doOnlyU2) {
      pU1ValD         = diGausInvGraph(fabs(pU1Diff), pMFracU1, pMSigmaU1_1, pMSigmaU1_2, pDFracU1, pDSigmaU1_1, pDSigmaU1_2);
      pU2ValD = fabs(pU2Diff);
    }
    if(!doOnlyU1 && doOnlyU2) {
      pU1ValD = fabs(pU1Diff);
      pU2ValD         = diGausInvGraph(fabs(pU2Diff), pMFracU2, pMSigmaU2_1, pMSigmaU2_2, pDFracU2, pDSigmaU2_1, pDSigmaU2_2);
    }
  }

  // histoDeltaU1.Fill(-pU1ValD+fabs(pU1Diff));
  // histoDeltaU2.Fill(-pU2ValD+fabs(pU2Diff));

  pU1ValD*=p1Charge;
  pU2ValD*=p2Charge;

  //  cout << "pU1ValD " << pU1ValD << "pU2ValD " << pU2ValD << endl;

  if(dodebug)   cout << " ==> Zpt: " << iGenPt << " pDef1: " << pDefU1 << " pDU1: " << pDU1 << " pMU1: " << pMU1 << endl;

  pDefU1 *= (pDU1/pMU1);

  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);

  // histoU1smeared.Fill(pU1);
  // histoU2smeared.Fill(pU2);

  if(dodebug)  cout << " after pU1 = " << pU1 << " pU2 = " << pU2 << endl;

  //  iU1   = pU1; 
  //  iU2   = pU2;

  return;
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
double iFlucU2,double iFlucU1,double iScale, int mytype, int fJet
) {

  /*
  bool dodebug=false;
  bool doIterativeMet = false;
  bool invGraph = true;
  bool doSingleGauss = false;
  bool doTriGauss = true;
  bool doApplyCorr = false; // smearing along the recoil vector
  bool doOnlyU1 = false;
  bool doOnlyU2 = true;
  bool writeTree = false;
  bool doClosure = false;
  */

  double lRescale  = sqrt((TMath::Pi())/2.);
  //  double lRescale  = 1;     // for squares

  double pDefU1    = iU1Default->Eval(iGenPt);

  double pDU1       = iU1RZDatFit ->Eval(iGenPt); // U1 average scale
  double pDRMSU1    = iU1MSZDatFit->Eval(iGenPt)*lRescale; // U1 average RMS
  double pDRMSU2    = iU2MSZDatFit->Eval(iGenPt)*lRescale; // U2 average RMS

  double pMU1       = iU1RZMCFit  ->Eval(iGenPt);
  double pMRMSU1    = iU1MSZMCFit ->Eval(iGenPt)*lRescale;
  double pMRMSU2    = iU2MSZMCFit ->Eval(iGenPt)*lRescale;

  ///
  /// ENDING of the PARAMETERS
  ///

  double pUX   = iMet*cos(iMPhi) + iLepPt*cos(iLepPhi);
  double pUY   = iMet*sin(iMPhi) + iLepPt*sin(iLepPhi);
  double pU    = sqrt(pUX*pUX+pUY*pUY);

  double pCos  = - (pUX*cos(iGenPhi) + pUY*sin(iGenPhi))/pU;
  double pSin  =   (pUX*sin(iGenPhi) - pUY*cos(iGenPhi))/pU;

  /////
  
  double pU1   = pU*pCos;
  double pU2   = pU*pSin;
  double pU1Diff  = pU1-pDefU1;
  double pU2Diff  = pU2;

  //  cout << " ------------------------------------------------------- " << endl;
  // cout << " initial pU1 = " << pU1 << " pU2 = " << pU2 << endl;

  double p1Charge        = pU1Diff/fabs(pU1Diff);
  double p2Charge        = pU2Diff/fabs(pU2Diff);

  double pU1ValD = 0 ;
  double pU2ValD = 0;
  double pUValM = 0;
  double pUValD = 0 ;

  // go to the pull space (we are on MC)
  // ????? what to do for the scale ??????

  pU1Diff = pU1Diff/pMRMSU1;
  pU1ValD = triGausInvGraphPDF(pU1Diff,iGenPt,pdfU1Cdf[2][fJet],pdfU1Cdf[1][fJet],wU1[2][fJet],wU1[1][fJet]);
  pU1ValD = pU1ValD*pDRMSU1;
  pDefU1 *= (pDU1/pMU1);


  pU2Diff = pU2Diff/pMRMSU2;
  pU2ValD = triGausInvGraphPDF(fabs(pU2Diff),iGenPt,pdfU2Cdf[2][fJet],pdfU2Cdf[1][fJet],wU2[2][fJet],wU2[1][fJet]);
  pU2ValD = pU2ValD*pDRMSU2;

  // pU1ValD*=p1Charge; // since removed the abs value
  pU2ValD*=p2Charge;

  pU1   = pDefU1             + pU1ValD;
  pU2   =                      pU2ValD;
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);

   // cout << " after pU1 = " << pU1 << " pU2 = " << pU2 << endl;

  return;

}
//-----------------------------------------------------------------------------------------------------------------------------------------
// double RecoilCorrector::triGausInvGraphPDF(double iPVal, double Zpt, RooAddPdf *pdfMC, RooAddPdf *pdfDATA, RooWorkspace *wMC, RooWorkspace *wDATA) {
double RecoilCorrector::triGausInvGraphPDF(double iPVal, double Zpt, RooAbsReal *pdfMCcdf, RooAbsReal *pdfDATAcdf, RooWorkspace *wMC, RooWorkspace *wDATA) {

  RooRealVar* myptm=wMC->var("pt");
  myptm->setVal(Zpt);

  RooRealVar* myptd=wDATA->var("pt");
  myptd->setVal(Zpt);

  RooRealVar* myXm = wMC->var("XVar");
  RooRealVar* myXd = wDATA->var("XVar");

  myXm->setVal(iPVal);
  double pVal=pdfDATAcdf->findRoot(*myXd,myXd->getMin(),myXd->getMax(),pdfMCcdf->getVal());

  // add protection for outlier since I tabulated up to 5
  if(TMath::Abs(pVal)>=5) pVal=iPVal;

  // cout << " original " << iPVal;
  // cout << "after pdfDATAcdf->findRoot" << pVal <<endl;


  return pVal;

}

//-----------------------------------------------------------------------------------------------------------------------------------------
void RecoilCorrector::runDiago(/* TFile *file_, */RooWorkspace *w, /*RooDataSet *data,*/ RooFitResult *result, TString fit, RooAbsReal *&pdfUiCdf) {
  
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
  pdfUiCdf = newpdf->createCdf(*myX1);

  
  // // DIAGONALIZER !!!
  // cout << newpdf->getVal() << endl; 
  // RooRealVar *v = w->var("eig_eig0");
  // v->Print();
  // Float_t oldVal = v->getVal();
  // v->setVal(3);
  // v->Print();
  // cout << newpdf->getVal() << endl;
  // v->setVal(-3);
  // v->Print();
  // cout << newpdf->getVal() << endl;
  
  
  // RooAbsPdf *newpdf_shifted = diago.diagonalizeWithEigenVariations(*w->pdf(Form("%s_eig",fit)),*result,0,1);
  
  // cout << "BEFORE NEW PDF DIAGO SHIFTED" << endl;
  // // newpdf->Print();
  // newpdf_shifted->Print();
  // cout << "AFTER NEW PDF DIAGO SHIFTED" << endl;
  
  // w->import(*newpdf_shifted, RooFit::RecycleConflictNodes());
  // // w->Print();
  // cout << "AFTER CALLING w->Print()" << endl;
  return;
  // // RooFitResult *res2 = newfit->fitTo(*data, RooFit::Save(1), RooFit::Minimizer("Minuit2"), RooFit::PrintLevel(-1));
  // // res2->Print("V");
}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::getFrac2gauss(double RMS, double sigma1, double sigma2) {

  return (RMS-sigma2)/(sigma1-sigma2);
  //  return (RMS*RMS-sigma2*sigma2)/(sigma1*sigma1-sigma2*sigma2);

}

//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::triGausInvGraph(double iPVal, /**/ double meanRMSMC, double iMean1MC, double iMean2MC, double iFrac1MC,double iSigma1MC,double iSigma2MC,double iSigma3MC,/**/ double meanRMSDATA, double iMean1DATA, double iMean2DATA,double iFrac1DATA,double iSigma1DATA,double iSigma2DATA,double iSigma3DATA) {

  // this for three gaussian
  double iFrac2DATA = getFrac3gauss(meanRMSDATA, iFrac1DATA, iSigma1DATA, iSigma2DATA, iSigma3DATA);
  double iFrac2MC = getFrac3gauss(meanRMSMC, iFrac1MC, iSigma1MC, iSigma2MC, iSigma3MC);

  /*
  // those parameters are stored in GeV

  cout << "meanRMSMC " << meanRMSMC << " iFrac1MC=" << iFrac1MC << " iSigma1MC=" << iSigma1MC << " iSigma2MC=" << iSigma2MC << " iSigma3MC=" << iSigma3MC << " iMean1MC=" << iMean1MC << "iMean2MC=" << iMean2MC << endl;
  cout << "meanRMSDATA " << meanRMSDATA << " iFrac1DATA=" << iFrac1DATA << " iSigma1DATA=" << iSigma1DATA << " iSigma2DATA=" << iSigma2DATA << " iSigma3DATA=" << iSigma3DATA << " iMean1DATA=" << iMean1DATA << "iMean2DATA=" << iMean2DATA << endl;
  cout << "iFrac2DATA=" << iFrac2DATA << " iFrac2MC= " << iFrac2MC << endl;
  */

  TF1 *totalMC = new TF1("totalMC","gaus(0)+gaus(3)+gaus(6)",0,100);
  totalMC->SetParameter(0,iFrac1MC); 
  totalMC->SetParameter(1,iMean1MC); // mean
  totalMC->SetParameter(2,iSigma1MC); 
  totalMC->SetParameter(3,iFrac2MC); 
  totalMC->SetParameter(4,iMean2MC); // mean
  totalMC->SetParameter(5,iSigma2MC);
  totalMC->SetParameter(6,(1-iFrac1MC-iFrac2MC)); 
  totalMC->SetParameter(7,iMean2MC); // mean
  totalMC->SetParameter(8,iSigma3MC);
  totalMC->SetNpx(2000);

  TF1 *totalDATA = new TF1("totalDATA","gaus(0)+gaus(3)+gaus(6)",0,100);
  totalDATA->SetParameter(0,iFrac1DATA); 
  totalDATA->SetParameter(1,iMean1DATA); // mean
  totalDATA->SetParameter(2,iSigma1DATA); 
  totalDATA->SetParameter(3,iFrac2DATA);
  totalDATA->SetParameter(4,iMean2DATA); // mean
  totalDATA->SetParameter(5,iSigma2DATA);
  totalDATA->SetParameter(6,(1-iFrac1DATA-iFrac2DATA));
  totalDATA->SetParameter(7,iMean2DATA); // mean
  totalDATA->SetParameter(8,iSigma3DATA);
  totalDATA->SetNpx(2000);

  /*
  TCanvas* c = new TCanvas("validatePDF","validatePDF",800,400) ;
  c->cd();

  totalMC->SetLineColor(kMagenta); 
  totalMC->Draw("hist");
  totalDATA->SetLineColor(kBlue); 
  totalDATA->Draw("hist same");
  c->SaveAs("gauss.png");
  cin.get(); 
  */
  //  cout << "constructed gauss " << endl;

  TGraph *gr_mc = new TGraph(totalMC, "I");
  TGraph *gr_data = new TGraph(totalDATA, "I");
  TGraph *gr_data_inverse = new TGraph(gr_data->GetN(),gr_data->GetY(), gr_data->GetX());

  /*
  gr_mc->SetLineColor(kMagenta);
  gr_mc->SetMarkerColor(kMagenta);
  gr_mc->Draw("A P L same");

  gr_data->SetLineColor(kGreen+1);
  gr_data->SetMarkerColor(kGreen+1);
  gr_data->Draw("P L same ");

  gr_data_inverse->SetLineColor(kCyan);
  gr_data_inverse->SetMarkerColor(kCyan);
  gr_data_inverse->Draw("P L same ");
  */

  double pVal=gr_data_inverse->Eval(gr_mc->Eval(iPVal));
  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " FROM INVERSE: " << gr_data_inverse->Eval(gr_mc->Eval(iPVal)) << endl;

  delete totalMC;
  delete totalDATA;

  delete gr_mc;
  delete gr_data;
  delete gr_data_inverse;

  return pVal;

}
//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::diGausPVal(double iVal,double iFrac,double iSigma1,double iSigma2) {

  double lVal=iFrac*TMath::Erf(iVal/iSigma1) + (1-iFrac)*TMath::Erf(iVal/iSigma2);
  if(TMath::ErfInverse(lVal)==0) return iVal;
  return lVal;


}
//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::diGausInvGraph(double iPVal,double iFracMC,double iSigma1MC,double iSigma2MC,double iFracDATA,double iSigma1DATA,double iSigma2DATA) {

  TF1 *totalMC = new TF1("totalMC","gaus(0)+gaus(3)",0,100);
  totalMC->SetParameter(0,iFracMC); 
  totalMC->SetParameter(1,0); // mean at zero
  totalMC->SetParameter(2,iSigma1MC); 
  totalMC->SetParameter(3,(1-iFracMC)); 
  totalMC->SetParameter(4,0); // mean at zero
  totalMC->SetParameter(5,iSigma2MC);
  totalMC->SetNpx(2000);

  TF1 *totalDATA = new TF1("totalDATA","gaus(0)+gaus(3)",0,100);
  totalDATA->SetParameter(0,iFracDATA); 
  totalDATA->SetParameter(1,0); 
  totalDATA->SetParameter(2,iSigma1DATA); 
  totalDATA->SetParameter(3,(1-iFracDATA));
  totalDATA->SetParameter(4,0); 
  totalDATA->SetParameter(5,iSigma2DATA);
  totalDATA->SetNpx(2000);

  TGraph *gr_mc = new TGraph(totalMC, "I");
  TGraph *gr_data = new TGraph(totalDATA, "I");
  TGraph *gr_data_inverse = new TGraph(gr_data->GetN(),gr_data->GetY(), gr_data->GetX());

  /*
  gr_mc->SetLineColor(kMagenta);
  gr_mc->SetMarkerColor(kMagenta);
  gr_mc->Draw("A P L same");
  gr_data->SetLineColor(kGreen+1);
  gr_data->SetMarkerColor(kGreen+1);
  gr_data->Draw("P L same ");
  gr_data_inverse->SetLineColor(kCyan);
  gr_data_inverse->SetMarkerColor(kCyan);
  gr_data_inverse->Draw("P L same ");
  */

  double pVal=gr_data_inverse->Eval(gr_mc->Eval(iPVal));
  //  cout << "ORIGINAL MC: " << iPVal ;
  //  cout << " FROM INVERSE: " << gr_data_inverse->Eval(gr_mc->Eval(iPVal)) << endl;

  delete totalMC;
  delete totalDATA;

  delete gr_mc;
  delete gr_data;
  delete gr_data_inverse;

  return pVal;

}
//-----------------------------------------------------------------------------------------------------------------------------------------
double RecoilCorrector::getFrac3gauss(double RMS, double f1, double sigma1, double sigma2, double sigma3) {

  sigma1=sigma1/RMS;
  sigma2=sigma2/RMS;
  sigma3=sigma3/RMS;
  
  double f2=(-1.)*( -f1*sigma1 + f1*sigma3 - sigma3 + 1. ) / ( (f1-1.)*(sigma2-sigma3) );

  return f2*(1-f1);

}
