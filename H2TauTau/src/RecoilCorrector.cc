
#include "CMGTools/H2TauTau/interface/RecoilCorrector.h"

RecoilCorrector::RecoilCorrector(string iNameZDat, string iNameZMC, int iSeed)
{

  lRandom = new TRandom1(iSeed);

  // get fits for sample to be corrected (i.e., W)
  lWNBins  = readRecoil(lWSumEt ,lWU1Fit ,lWU1RMSSMFit ,lWU1RMS1Fit ,lWU1RMS2Fit ,lWU13SigFit ,lWU2Fit ,lWU2RMSSMFit ,lWU2RMS1Fit ,lWU2RMS2Fit ,lWU23SigFit);
  // get fits for Z data
  lZDNBins = readRecoil(lZDSumEt,lZDU1Fit,lZDU1RMSSMFit,lZDU1RMS1Fit,lZDU1RMS2Fit,lZDU13SigFit,lZDU2Fit,lZDU2RMSSMFit,lZDU2RMS1Fit,lZDU2RMS2Fit,lZDU23SigFit,iNameZDat);
  // get fits for Z MC
  lZMNBins = readRecoil(lZMSumEt,lZMU1Fit,lZMU1RMSSMFit,lZMU1RMS1Fit,lZMU1RMS2Fit,lZMU13SigFit,lZMU2Fit,lZMU2RMSSMFit,lZMU2RMS1Fit,lZMU2RMS2Fit,lZMU23SigFit,iNameZMC);

}

void RecoilCorrector::Correct(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi)
{
    //cout << "====> Met Before ==> " << pMet << " -- " << pMPhi ;
    metDistribution(met,metphi,lGenPt,lGenPhi,lepPt,lepPhi,lRandom,
		    lWU1Fit     [0],lZDU1Fit     [0],lZMU1Fit     [0],
		    lWU1RMSSMFit[0],lZDU1RMSSMFit[0],lZMU1RMSSMFit[0],
		    lWU1RMS1Fit [0],lZDU1RMS1Fit [0],lZMU1RMS1Fit [0],
		    lWU1RMS2Fit [0],lZDU1RMS2Fit [0],lZMU1RMS2Fit [0],
		    lWU13SigFit [0],lZDU13SigFit [0],lZMU13SigFit [0],
		    lWU2RMSSMFit[0],lZDU2RMSSMFit[0],lZMU2RMSSMFit[0],
		    lWU2RMS1Fit [0],lZDU2RMS1Fit [0],lZMU2RMS1Fit [0],
		    lWU2RMS2Fit [0],lZDU2RMS2Fit [0],lZMU2RMS2Fit [0],
		    lWU23SigFit [0],lZDU23SigFit [0],lZMU23SigFit [0]);
		    //,int iFluc=0,int iMetType=1) {
    //cout << "====> Met After ==> " << pMet << " -- " << pMPhi  << endl;

}

int RecoilCorrector::readRecoil(std::vector<double> &iSumEt,
		     std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1Sig3Fit,
		     std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2Sig3Fit,
		     std::string iFName) {
  if(!getenv("CMSSW_BASE")) {
    printf("error! RecoilCorrector called without input files. Define CMSSW_BASE or add by hand.\n");
    //    assert(0);
  }
  TFile *lFile  = new TFile(iFName.c_str());
  TGraph *lGraph = (TGraph *) lFile->FindObjectAny("sumet");
  const int lNBins = lGraph->GetN();
  for(int i0 = 0; i0 < lNBins; i0++) {
    iSumEt.push_back(lGraph->GetY()[i0]);
    std::stringstream pSS1,pSS2,pSS3,pSS4,pSS5,pSS6,pSS7,pSS8,pSS9,pSS10;
    pSS1  << "u1Mean_"    << i0;  iU1Fit.push_back    ( (TF1*) lFile->FindObjectAny(pSS1.str().c_str())); //iU1Fit[i0]->SetDirectory(0);
    pSS2  << "u1MeanRMS_" << i0;  iU1MRMSFit.push_back( (TF1*) lFile->FindObjectAny(pSS2.str().c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS3  << "u1RMS1_"    << i0;  iU1RMS1Fit.push_back( (TF1*) lFile->FindObjectAny(pSS3.str().c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS4  << "u1RMS2_"    << i0;  iU1RMS2Fit.push_back( (TF1*) lFile->FindObjectAny(pSS4.str().c_str())); //iU1RMSFit[i0]->SetDirectory(0);
    pSS5  << "u1Sig3_"    << i0;  iU1Sig3Fit.push_back( (TF1*) lFile->FindObjectAny(pSS5.str().c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS6  << "u2Mean_"    << i0;  iU2Fit    .push_back( (TF1*) lFile->FindObjectAny(pSS6.str().c_str())); //iU2Fit[i0]->SetDirectory(0);
    pSS7  << "u2MeanRMS_" << i0;  iU2MRMSFit.push_back( (TF1*) lFile->FindObjectAny(pSS7.str().c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS8  << "u2RMS1_"    << i0;  iU2RMS1Fit.push_back( (TF1*) lFile->FindObjectAny(pSS8.str().c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS9  << "u2RMS2_"    << i0;  iU2RMS2Fit.push_back( (TF1*) lFile->FindObjectAny(pSS9.str().c_str())); //iU2RMSFit[i0]->SetDirectory(0);
    pSS10 << "u2Sig3_"    << i0;  iU2Sig3Fit.push_back( (TF1*) lFile->FindObjectAny(pSS10.str().c_str())); //iU2RMSFit[i0]->SetDirectory(0);
  }
  lFile->Close();
  iSumEt.push_back(1000);  
  return lNBins;
}

void RecoilCorrector::metDistribution(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
		     double iLepPt,double iLepPhi,TRandom1 *iRand,
		     TF1 *iU1RWFit   ,TF1 *iU1RZDatFit  ,TF1 *iU1RZMCFit,
		     TF1 *iU1MSWFit  ,TF1 *iU1MSZDatFit ,TF1 *iU1MSZMCFit, 
		     TF1 *iU1S1WFit  ,TF1 *iU1S1ZDatFit ,TF1 *iU1S1ZMCFit,
		     TF1 *iU1S2WFit  ,TF1 *iU1S2ZDatFit ,TF1 *iU1S2ZMCFit,
		     TF1 *iU13SWFit  ,TF1 *iU13SZDatFit ,TF1 *iU13SZMCFit,
		     TF1 *iU2MSWFit  ,TF1 *iU2MSZDatFit ,TF1 *iU2MSZMCFit, 		      
		     TF1 *iU2S1WFit  ,TF1 *iU2S1ZDatFit ,TF1 *iU2S1ZMCFit, 
		     TF1 *iU2S2WFit  ,TF1 *iU2S2ZDatFit ,TF1 *iU2S2ZMCFit, 
		     TF1 *iU23SWFit  ,TF1 *iU23SZDatFit ,TF1 *iU23SZMCFit, 
		     int iFluc,int iMetType) {
  //Important constants re-scaling of sigma on left and mean wpt of W resbos on right
  double lRescale = sqrt((TMath::Pi())/2.); double lPtMean = 16.3; //==> tuned for W bosons
  double pU1      = iU1RWFit->Eval(iGenPt);
  double pU2      = 0; //Right guys are for cumulants => code deleted
  double pSigma1_1 = iU1MSWFit->Eval(iGenPt)*lRescale*iU1S1WFit->Eval(iGenPt);
  double pSigma1_2 = iU1MSWFit->Eval(iGenPt)*lRescale*iU1S2WFit->Eval(iGenPt);
  double pFrac1    = iU1MSWFit->Eval(iGenPt)*lRescale;
  double pCorr1    = iU13SWFit->Eval(iGenPt)/iU13SWFit->Eval(lPtMean);
  double pSigma2_1 = iU2MSWFit->Eval(iGenPt)*lRescale*iU2S1WFit->Eval(iGenPt);
  double pSigma2_2 = iU2MSWFit->Eval(iGenPt)*lRescale*iU2S2WFit->Eval(iGenPt);
  double pFrac2    = iU2MSWFit->Eval(iGenPt)*lRescale;
  double pCorr2    = iU23SWFit->Eval(iGenPt)/iU23SWFit->Eval(lPtMean);
  
  //Left is Cumulant right is standard
  double pZMSigma1_1 = iU1S1ZMCFit->Eval(iGenPt)*iU1MSZMCFit->Eval(iGenPt);
  double pZMSigma1_2 = iU1S2ZMCFit->Eval(iGenPt)*iU1MSZMCFit->Eval(iGenPt);
  double pZMFrac1    = iU1MSZMCFit->Eval(iGenPt);
  double pZMSigma2_1 = iU2S1ZMCFit->Eval(iGenPt)*iU2MSZMCFit->Eval(iGenPt);
  double pZMSigma2_2 = iU2S2ZMCFit->Eval(iGenPt)*iU2MSZMCFit->Eval(iGenPt);
  double pZMFrac2    = iU2MSZMCFit->Eval(iGenPt);
  double pZMCorr1    = iU13SZMCFit->Eval(iGenPt)/iU13SZMCFit->Eval(lPtMean);
  double pZMCorr2    = iU23SZMCFit->Eval(iGenPt)/iU23SZMCFit->Eval(lPtMean);
  
  double pZDSigma1_1 = iU1S1ZDatFit->Eval(iGenPt)*iU1MSZDatFit->Eval(iGenPt);
  double pZDSigma1_2 = iU1S2ZDatFit->Eval(iGenPt)*iU1MSZDatFit->Eval(iGenPt);
  double pZDFrac1    = iU1MSZDatFit->Eval(iGenPt);
  double pZDSigma2_1 = iU2S1ZDatFit->Eval(iGenPt)*iU2MSZDatFit->Eval(iGenPt);
  double pZDSigma2_2 = iU2S2ZDatFit->Eval(iGenPt)*iU2MSZDatFit->Eval(iGenPt);
  double pZDFrac2    = iU2MSZDatFit->Eval(iGenPt);
  double pZDCorr1    = iU13SZDatFit->Eval(iGenPt)/iU13SZDatFit->Eval(lPtMean);
  double pZDCorr2    = iU23SZDatFit->Eval(iGenPt)/iU23SZDatFit->Eval(lPtMean);

  pU1       =  pU1*    (iU1RZDatFit->Eval(iGenPt)/iU1RZMCFit->Eval(iGenPt));
  pFrac1    =  pZDFrac1/pZMFrac1 * pFrac1;
  pFrac2    =  pZDFrac2/pZMFrac2 * pFrac2;
  pSigma1_1 =  pSigma1_1*pZDSigma1_1/pZMSigma1_1;
  pSigma1_2 =  pSigma1_2*pZDSigma1_2/pZMSigma1_2;
  pSigma2_1 =  pSigma2_1*pZDSigma2_1/pZMSigma2_1;
  pSigma2_2 =  pSigma2_2*pZDSigma2_2/pZMSigma2_2;
  pCorr1    =  pCorr1   *pZDCorr1/pZMCorr1;
  pCorr2    =  pCorr2   *pZDCorr2/pZMCorr2;
  
  //Uncertainty propagation
  if(iFluc != 0) { 
    double lEUR1    = getError(iGenPt,iU1RWFit ,iU1RZDatFit ,iU1RZMCFit ,true);
    double lEUS1_1  = getError(0.,iU1S1WFit,iU1S1ZDatFit,iU1S1ZMCFit    ,true);
    double lEUS1_2  = getError(0.,iU1S2WFit,iU1S2ZDatFit,iU1S2ZMCFit    ,true);
    double lEU1Frac = getError(iGenPt,iU1MSWFit,iU1MSZDatFit,iU1MSZMCFit,true);
    double lEU1Corr = getError(iGenPt,iU13SWFit,iU13SZDatFit,iU13SZMCFit,true);
    double lEUS2_1  = getError(0.,iU2S1WFit,iU2S1ZDatFit,iU2S1ZMCFit    ,true);
    double lEUS2_2  = getError(0.,iU2S2WFit,iU2S2ZDatFit,iU2S2ZMCFit    ,true);
    double lEU2Frac = getError(iGenPt,iU2MSWFit,iU2MSZDatFit,iU2MSZMCFit,true);
    double lEU2Corr = getError(iGenPt,iU23SWFit,iU23SZDatFit,iU23SZMCFit,true);
    
    //Modify all the different parameters the choice of signs makes it maximal
    pU1       = pU1       - iFluc*lEUR1;              //Recoil
    pFrac1    = pFrac1    + iFluc*(lEU1Frac);        //Mean RMS 
    pSigma1_1 = pSigma1_1 - iFluc*lEUS1_1*pFrac1;    //Sigma 1 smalles sigma
    pSigma1_2 = pSigma1_2 + iFluc*lEUS1_2*pFrac1;    //Sigma 2 (Maximal when oppsite sigma 1)
    pCorr1    = pCorr1    + iFluc*(lEU1Corr);        //Correction from > 3 <sigma> to be consistent must be same sign as sigma 2
    pFrac2    = pFrac2    + iFluc*(lEU2Frac);        //Mean RMS for U2
    pSigma2_1 = pSigma2_1 - iFluc*lEUS2_1*pFrac2;    //Sigma 1 U2
    pSigma2_2 = pSigma2_2 + iFluc*(lEUS2_2)*pFrac2;
    pCorr2    = pCorr2    + iFluc*(lEU2Corr);
  }
  //Now calcualte recoil
  double pMS1 = pFrac1;
  double pMS2 = pFrac2;
  //Caculat the proper fraction
  pFrac1 = (pFrac1-pSigma1_2)/(pSigma1_1-pSigma1_2);
  pFrac2 = (pFrac2-pSigma2_2)/(pSigma2_1-pSigma2_2);
  //Apply the correction to sigma 2
  pSigma1_2*=pCorr1;
  pSigma2_2*=pCorr2;   
  //Constant Sigma correction
  if(iMetType == 0) {
    if(iFluc != 0) pSigma1_1 *= (1 + iFluc*0.08);  pSigma2_1 *= 1 + iFluc*0.08 ;
    //Systematic on Simga variation 0.1 for muons 0.04 for electrons
    pFrac1 = (pMS1-pSigma1_2)/(pSigma1_1-pSigma1_2); //Constant Sigma 
    pFrac2 = (pMS2-pSigma2_2)/(pSigma2_1-pSigma2_2);
  }
  //Constat Fraction correction
  if(iMetType == 1) {
    if(iFluc != 0 ) {pFrac2 = pFrac2 - iFluc*0.04; pFrac1 = pFrac1 - iFluc*0.04;}
    //Systematic on fraction variation 0.1 for mu 0.04 for e-
    pSigma1_1 = (pMS1-(1-pFrac1)*pSigma1_2)/pFrac1;//(pFrac1-pSigma1_2)/(pSigma1_1-pSigma1_2); //V1
    pSigma2_1 = (pMS2-(1-pFrac2)*pSigma2_2)/pFrac2;//(pFrac2-pSigma2_2)/(pSigma2_1-pSigma2_2);
  }
  //Now sample for the MET distribution
  double lVal0 = iRand->Uniform(0,1);
  double lVal1 = iRand->Uniform(0,1);
  pU1   = (lVal0 < pFrac1)*iRand->Gaus(pU1,pSigma1_1)+(lVal0 > pFrac1)*iRand->Gaus(pU1,pSigma1_2);
  pU2   = (lVal1 < pFrac2)*iRand->Gaus(pU2,pSigma2_1)+(lVal1 > pFrac2)*iRand->Gaus(pU2,pSigma2_2);
  iMet  = calculate(0,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  iMPhi = calculate(1,iLepPt,iLepPhi,iGenPhi,pU1,pU2);
  return;
}

double RecoilCorrector::calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2) { 
  double lMX = -iEPt*cos(iEPhi) - iU1*cos(iWPhi) + iU2*sin(iWPhi);
  double lMY = -iEPt*sin(iEPhi) - iU1*sin(iWPhi) - iU2*cos(iWPhi);
  if(iMet == 0) return sqrt(lMX*lMX + lMY*lMY);
  if(iMet == 1) {if(lMX > 0) {return atan(lMY/lMX);} return (fabs(lMY)/lMY)*3.14159265 + atan(lMY/lMX); } 
  if(iMet == 2) return lMX;
  if(iMet == 3) return lMY;
  return lMY;
}

double RecoilCorrector::getCorError2(double iVal,TF1 *iFit) { 
  double lE = sqrt(iFit->GetParError(0))  + iVal*sqrt(iFit->GetParError(2));
  if(fabs(iFit->GetParError(4)) > 0) lE += iVal*iVal*sqrt(iFit->GetParError(4));
  return lE*lE;
}

double RecoilCorrector::getError2(double iVal,TF1 *iFit) { 
  double lE2 = iFit->GetParError(0) + iVal*iFit->GetParError(1) + iVal*iVal*iFit->GetParError(2);
  if(fabs(iFit->GetParError(3)) > 0) lE2 += iVal*iVal*iVal*     iFit->GetParError(3);
  if(fabs(iFit->GetParError(4)) > 0) lE2 += iVal*iVal*iVal*iVal*iFit->GetParError(4);
  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) == 0) lE2 += iVal*iVal*               iFit->GetParError(5);
  if(fabs(iFit->GetParError(5)) > 0 && iFit->GetParameter(3) != 0) lE2 += iVal*iVal*iVal*iVal*iVal*iFit->GetParError(5);
  if(fabs(iFit->GetParError(6)) > 0) lE2 += iVal*iVal*iVal*iVal*iVal*iVal*iFit->GetParError(6);
  return lE2;
}

double RecoilCorrector::getError(double iVal,TF1 *iWFit,TF1 *iZDatFit,TF1 *iZMCFit,bool iRescale) {
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
