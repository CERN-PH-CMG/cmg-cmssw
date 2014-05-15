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
#include "TStyle.h"
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
#include "RooHistPdf.h"
///#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"
#include "TLatex.h"

//#include "TLorentsVector.h"
#include <vector>

using namespace RooFit;

bool doPrint=true;
bool doPrintAll=false;
bool doBinned=false;

bool doMetPhiCorr=false;
bool doPosW=false;
bool doNegW=false;

bool doRunA=false;
bool doRunB=false;

TFile *fDataFile = 0;
TTree *fDataTree = 0; 

TFile *fTTbarFile    = 0;
TTree *fTTbar        = 0;

bool  fData   = false;
float fMetMax = 0; float fZPtMin = 0; float fZPtMax = 0; int fNJetSelect = 0;      
/*float   fNPV    = 0;*/ int   fNPV    = 0;
/*float fU1 = 0; float fU2 = 0; */
/*float fTKU1 = 0; float fTKU2 = 0;*/
//float fPFU1 = 0;  float fPFU2   = 0; 

double fPFU1 = 0;  double fPFU2   = 0;
double fU1 = 0; double fU2 = 0;
double fTKU1 = 0; double fTKU2 = 0;

/*float fZPt = 0; float fZPhi = 0; float fPhi = 0; */ double fZPt = 0; double fZPhi = 0; double fPhi = 0;
/*float fMet = 0;  float fMPhi = 0; */ double fMet = 0;  double fMPhi = 0;
/*float fPt1 = 0; float fPhi1 = 0; float fPt2 = 0; float fEta2 = 0; float fPhi2 = 0; */
double fPt1 = 0; double fPhi1 = 0; double fPt2 = 0; double fEta2 = 0; double fPhi2 = 0; 

int fNJet = 0; float fWeight = 0;

int fId1 = 0; int fId2 = 0; float fGPhi1 = 0; float fGPhi2 = 0; float fGEta1  = 0; float fGEta2 = 0;
int fNBTag = 0; float fMJJ = 0; /*float fJPt1 = 0;*/ double fJPt1 = 0; float fDEta = 0; int fIS2011A = 0; int fId = -1; float fVPt = 0; float fVPhi = 0;

double fZmass;
double fMuPos_phi, fMuPos_eta, fMuPos_pt, fMuPos_charge, fMuPosReliso, fMuPos_dxy;
double fMuNeg_phi, fMuNeg_eta, fMuNeg_pt, fMuNeg_charge,fMuNeg_dxy, fMuNegReliso;
int fMuPosTrg, fMuPosIsTightAndIso, fevtHasGoodVtx, fevtHasTrg, fMuNegIsTightAndIso, fMuNegTrg;

int fRun=-1;
int fLumi=-1;
int fEvent=-1;

std::vector<std::vector<std::vector<float> > > lXVals_all;  std::vector<std::vector<std::vector<float> > > lYVals_all; std::vector<std::vector<std::vector<float> > > lYTVals_all;
std::vector<std::vector<std::vector<float> > > lXEVals_all; std::vector<std::vector<std::vector<float> > > lYEVals_all; 

float ***vlXVals_all,***vlXEVals_all,***vlYVals_all,***vlYTVals_all,***vlYEVals_all; //two * are needed because it is a pofloater to a pofloater

float getPhiCorrMET(bool iType) { 

  float metx = fMet * cos( fMPhi );
  float mety = fMet * sin( fMPhi );
  
  float shiftx = 0.;
  float shifty = 0.;
  
  shiftx = (!fData) ? (-0.02245 - 0.0498*fNPV)
    : (-0.1676 + 0.2509*fNPV);
  shifty = (!fData) ? (0.1158 - 0.3201*fNPV)
    : (-0.2244 - 0.4009*fNPV);
  
  metx -= shiftx;
  mety -= shifty;

  if(!iType) return atan2( mety , metx ); // metPhi
  if(iType) return sqrt( metx*metx + mety*mety ); // met 

}



void load(TTree *iTree, int type) {

  fWeight = 1;
  iTree->ResetBranchAddresses();
  //iTree->SetBranchAddress("IS2011A",&fIS2011A);

  // MARIA this is not there only the leading jet infos
  iTree->SetBranchAddress("nbtag" ,&fNBTag); // this is not commented
  //iTree->SetBranchAddress("mjj"   ,&fMJJ);

  //iTree->SetBranchAddress("jdeta" ,&fDEta);
  //iTree->SetBranchAddress("id1_l" ,&fId1);
  //iTree->SetBranchAddress("id2_l" ,&fId2);
  //iTree->SetBranchAddress("phi_z",&fVPhi);
  //iTree->SetBranchAddress("pt_z" ,&fVPt);
  //iTree->SetBranchAddress("phi1_l",&fGPhi1);
  //iTree->SetBranchAddress("phi2_l",&fGPhi2);
  //iTree->SetBranchAddress("eta1_l",&fGEta1);
  //iTree->SetBranchAddress("eta2_l",&fGEta2);

  /*
    iTree->SetBranchAddress("eta_2" ,&fEta2);
    iTree->SetBranchAddress("phi_2" ,&fPhi2);
    iTree->SetBranchAddress("taujetpt" ,&fPt2)
    iTree->SetBranchAddress("pt_vis"  ,&fPt1);
    iTree->SetBranchAddress("phi_vis" ,&fPhi1);
  */

  /*
  // commented for now since is not good for later
  iTree->SetBranchAddress("MuNeg_eta" ,&fEta2);
  iTree->SetBranchAddress("MuNeg_phi" ,&fPhi2);
  iTree->SetBranchAddress("MuNeg_pt" ,&fPt2); 
  */

  /* 
  iTree->SetBranchAddress("MuPos_pt"  ,&fPt1); 
  iTree->SetBranchAddress("MuPos_phi" ,&fPhi1); // this maybe for the W
  */

  /*
  iTree->SetBranchAddress("Z_pt"  ,&fPt1); // this is for the Z; this is not working because of renaming
  iTree->SetBranchAddress("Z_phi" ,&fPhi1);
  */

  iTree->SetBranchAddress("pt_vis"  ,&fPt1);
  iTree->SetBranchAddress("phi_vis" ,&fPhi1);

  iTree->SetBranchAddress("run" ,&fRun);
  iTree->SetBranchAddress("lumi" ,&fLumi);
  iTree->SetBranchAddress("evt" ,&fEvent);

  //iTree->SetBranchAddress("Weight",&fWeight); // this is set to 1 fot now

  iTree->SetBranchAddress("Jet_leading_pt"  ,&fJPt1);
  //  iTree->SetBranchAddress("jetpt1"  ,&fJPt1);
  iTree->SetBranchAddress("nvtx"   ,&fNPV);
  //  iTree->SetBranchAddress("npv"   ,&fNPV);

  if((!doPosW) && (!doNegW)) {
    if(!fData) iTree->SetBranchAddress("ZGen_pt" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("ZGen_phi",&fZPhi);
    //  if(!fData) iTree->SetBranchAddress("Z_pt" ,&fZPt);
    //  if(!fData) iTree->SetBranchAddress("Z_phi",&fZPhi);
    if(fData)  iTree->SetBranchAddress("Z_pt" ,&fZPt);
    if(fData)  iTree->SetBranchAddress("Z_phi",&fZPhi);
  }

  if((doPosW) || (doNegW)) {
    if(!fData) iTree->SetBranchAddress("WGen_pt" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("WGen_phi",&fZPhi);

    //    cout << " pt W " << fZPt  << " phi W " << fZPhi << endl;
    //  if(!fData) iTree->SetBranchAddress("Z_pt" ,&fZPt);
    //  if(!fData) iTree->SetBranchAddress("Z_phi",&fZPhi);
    if(fData)  iTree->SetBranchAddress("W_pt" ,&fZPt);
    if(fData)  iTree->SetBranchAddress("W_phi",&fZPhi);
  }

  /*
    if(!fData) iTree->SetBranchAddress("pt_z" ,&fZPt);
    if(!fData) iTree->SetBranchAddress("phi_z",&fZPhi);
    if(fData)  iTree->SetBranchAddress("pt_z" ,&fZPt);
    if(fData)  iTree->SetBranchAddress("phi_z",&fZPhi);
  */

  // MARIA this is not there only the leading jet infos
  iTree->SetBranchAddress("njets",&fNJet);

  if(type==0) {
    //iTree->SetBranchAddress("u1x_mva"    ,&fU1);
    //iTree->SetBranchAddress("u2x_mva"    ,&fU2);
    //iTree->SetBranchAddress("metx_mva"   ,&fMet);
    //iTree->SetBranchAddress("metphix_mva",&fMPhi);
    iTree->SetBranchAddress("mvamet"   ,&fMet);
    iTree->SetBranchAddress("mvametphi",&fMPhi);
  } else if(type==1) {
    iTree->SetBranchAddress("trku1" ,&fU1);
    iTree->SetBranchAddress("trku2" ,&fU2);
    iTree->SetBranchAddress("trkmet",&fMet); 
  } else if(type==2) {
    //iTree->SetBranchAddress("trku1" ,&fTKU1);
    //iTree->SetBranchAddress("trku2" ,&fTKU2);
    // MARIA: I'm using those now
    iTree->SetBranchAddress("u1"  ,&fPFU1);
    iTree->SetBranchAddress("u2"  ,&fPFU2);
    //    iTree->SetBranchAddress("met",&fMet);
    //    iTree->SetBranchAddress("metphi",&fMPhi);

    iTree->SetBranchAddress("pfmet",&fMet);
    iTree->SetBranchAddress("pfmet_phi",&fMPhi);
  }


  /// these are needed for the selection

  /// Z selection 
  if((!doPosW) && (!doNegW)) {

    iTree->SetBranchAddress("Z_mass" ,&fZmass);

    iTree->SetBranchAddress("MuPos_eta" ,&fMuPos_eta);
    iTree->SetBranchAddress("MuPos_pt" ,&fMuPos_pt);
    iTree->SetBranchAddress("MuPos_charge" ,&fMuPos_charge);
    iTree->SetBranchAddress("MuPosRelIso" ,&fMuPosReliso);
    iTree->SetBranchAddress("MuPos_dxy" ,&fMuPos_dxy);
      
    iTree->SetBranchAddress("MuNeg_eta" ,&fMuNeg_eta);
    iTree->SetBranchAddress("MuNeg_pt" ,&fMuNeg_pt);
    iTree->SetBranchAddress("MuNeg_charge" ,&fMuNeg_charge);
    iTree->SetBranchAddress("MuNegRelIso" ,&fMuNegReliso);
    iTree->SetBranchAddress("MuNeg_dxy" ,&fMuNeg_dxy);
    
    iTree->SetBranchAddress("MuPosTrg" ,&fMuPosTrg);
    iTree->SetBranchAddress("MuNegTrg" ,&fMuNegTrg);
    iTree->SetBranchAddress("MuPosIsTightAndIso" ,&fMuPosIsTightAndIso);
    iTree->SetBranchAddress("MuNegIsTightAndIso" ,&fMuNegIsTightAndIso);
  }

  /// W selection 
  if((doPosW) || (doNegW)) {
    iTree->SetBranchAddress("Mu_eta" ,&fMuPos_eta);
    iTree->SetBranchAddress("Mu_pt" ,&fMuPos_pt);
    iTree->SetBranchAddress("Mu_charge" ,&fMuPos_charge);
    iTree->SetBranchAddress("MuRelIso" ,&fMuPosReliso);
    iTree->SetBranchAddress("Mu_dxy" ,&fMuPos_dxy);
    
    iTree->SetBranchAddress("MuIsTightAndIso" ,&fMuPosIsTightAndIso);
    
  }

  iTree->SetBranchAddress("evtHasGoodVtx" ,&fevtHasGoodVtx);
  iTree->SetBranchAddress("evtHasTrg" ,&fevtHasTrg);

  
}

void calculateU1U2(double &iPar, bool iType) {

  double lUX  = fMet*cos(fMPhi) + fPt1*cos(fPhi1);
  double lUY  = fMet*sin(fMPhi) + fPt1*sin(fPhi1);
  double lU   = sqrt(lUX*lUX+lUY*lUY);

  // rotate of -180 the X and Y component
  double lCos = - (lUX*cos(fZPhi) + lUY*sin(fZPhi))/lU;
  double lSin =   (lUX*sin(fZPhi) - lUY*cos(fZPhi))/lU;
  fU1 = lU*lCos;
  fU2 = lU*lSin;
  if(iType)  iPar = fU1;
  if(!iType) iPar = fU2;

  /*
  cout << "----------------" << endl;
  cout << "fU1: recomputed " << fU1 << " stored " << fPFU1 << endl;
  cout << "fU2: recomputed " << fU2 << " stored " << fPFU2 << endl;
  */

}

bool checkOdd() {

  if(fData) return true;

  if(!fData && fEvent % 2) {
    // check the odd events for the closure test
    return true;
  }

  return false;

}

bool runSelection() {

  if( fevtHasGoodVtx 
      && fevtHasTrg
      
      && fZmass>70
      && fZmass<110
      && (fMuNeg_charge != fMuPos_charge)

      && (
	  (TMath::Abs(fMuPos_eta)<2.1 && fMuPos_pt>30 && fMuPosTrg
	   && TMath::Abs(fMuNeg_eta)<2.4 && fMuNeg_pt>10

	   && fMuPosIsTightAndIso
	   && fMuPosReliso<0.12 
	   && fMuPos_dxy<0.02
	   && fMuNegIsTightAndIso
	   //	   && fMuNegReliso<0.12 // relax iso on the second muon
	   && fMuNeg_dxy<0.02
	   ) 
	   ||
	  (TMath::Abs(fMuNeg_eta)<2.1 && fMuNeg_pt>30 && fMuNegTrg
           && TMath::Abs(fMuPos_eta)<2.4 && fMuPos_pt>10

	   && fMuNegIsTightAndIso
	   && fMuNegReliso<0.12 
	   && fMuNeg_dxy<0.02
	   && fMuPosIsTightAndIso
	   //	   && fMuPosReliso<0.12  // relax iso on the second muon
	   && fMuPos_dxy<0.02

	   )
	  )
      )
    {

      //    cout << "GOOD selection" << endl;
    return true;
  }

  //  cout << "BAD selection" << endl;
  return false;

}

bool runWSelection(bool doPos) {

  float recoil=sqrt(fU1*fU1 + fU2*fU2);

  if( !fevtHasGoodVtx ) return false;
  if( !fevtHasTrg ) return false;

  /// add met and recoil
  if( fMet <= 25 ) return false;

  // do not cut on the recoil otherwise we bias the fits
  //  if( recoil >= 20 ) return false;

  /// pick separately the positive and negative
  if(doPos && fMuPos_charge<0 ) return false;
  if((!doPos) && fMuPos_charge>0 ) return false;

  /// a good muon 
  if(fMuPos_pt>30 
     //     && TMath::Abs(fMuPos_eta)<0.6 
     && TMath::Abs(fMuPos_eta)<2.1 
     && fMuPosIsTightAndIso
     && fMuPosReliso<0.12 
     && fMuPos_dxy<0.02
     ) return true;

  return false;

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

bool passId(int iId) { 

  //  if(fId == 0) return true;
  //if(iId  < 0) {if(!(fIS2011A == 0))                                {return false;}}
  //if(iId  > 0) {if(!(fIS2011A == 1))                                {return false;}}
  double lId = fabs(iId);
  //if(lId == 2) {if(fNJet == 0 || (fNJet == 1 && fJPt1 < 150))    {return true;} else {return false;}}
  //if(lId == 3) {if(fNJet == 1  && fJPt1 > 150)                   {return true;} else {return false;}}
  //if(lId == 4) {if(fNJet == 2  && fMJJ > 400 && fabs(fDEta) > 4) {return true;} else {return false;}}
  //if(lId == 5) {if(fNBTag >  0)                                  {return true;} else {return false;}}
  //if(lId == 6) {if(fNBTag ==  0)                                 {return true;} else {return false;}}

  if(doBinned) {
    if(lId == 1) {if(fNPV > 5              ) {return false; } {return true;}}
    //  if(lId == 1) {if(fNPV < 3 || fNPV > 5              ) {return false; } {return true;}}
    if(lId == 2) {if(fNPV < 6 || fNPV  > 10) {return false; } {return true;}}
    if(lId == 3) {if(fNPV < 11             ) {return false; } {return true;}}
    
    if(lId == 0) {if(fNPV > 12             ) {return false; } {return true;}}

  } else {

    if(lId == 1) {if(fNPV >1              ) {return false; } {return true;}} // include the vtx==0
    if(lId == 2) {if(fNPV !=2              ) {return false; } {return true;}}
    if(lId == 3) {if(fNPV !=3              ) {return false; } {return true;}}
    if(lId == 4) {if(fNPV !=4              ) {return false; } {return true;}}
    if(lId == 5) {if(fNPV !=5              ) {return false; } {return true;}}
    if(lId == 6) {if(fNPV !=6              ) {return false; } {return true;}}
    if(lId == 7) {if(fNPV !=7              ) {return false; } {return true;}}
    if(lId == 8) {if(fNPV !=8              ) {return false; } {return true;}}
    if(lId == 9) {if(fNPV !=9              ) {return false; } {return true;}}
    if(lId == 10) {if(fNPV !=10              ) {return false; } {return true;}}
    if(lId == 11) {if(fNPV !=11              ) {return false; } {return true;}}
    if(lId == 12) {if(fNPV !=12              ) {return false; } {return true;}}
    if(lId == 13) {if(fNPV !=13              ) {return false; } {return true;}}
    if(lId == 14) {if(fNPV !=14              ) {return false; } {return true;}}
    if(lId == 15) {if(fNPV !=15              ) {return false; } {return true;}}
    if(lId == 16) {if(fNPV !=16              ) {return false; } {return true;}}
    if(lId == 17) {if(fNPV !=17              ) {return false; } {return true;}}
    if(lId == 18) {if(fNPV !=18              ) {return false; } {return true;}}
    if(lId == 19) {if(fNPV !=19              ) {return false; } {return true;}}
    ///    if(lId >= 20) {if(fNPV !=20              ) {return false; } {return true;}} // include the vtx>20                                              
    if(lId >= 20) {if(fNPV <20              ) {return false; } {return true;}} // include the vtx>20         

  }

  //  cout << "NVertex " << fNPV << endl;
  
  return true;
  
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
  cout << "================================== 0 - 0 " << lE0 << endl;
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

void drawErrorBands(TF1 *iFit, float iXMax) { 
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
  //  lG0->SetFillColor(kYellow);
  lG0->SetFillColor(kGreen);
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

/*
//void fitCorr(bool iRMS,TTree *iTree,TCanvas *iC,float &lPar0,float &lPar1,
void fitCorr(bool iRMS,TTree *iTree,TCanvas *iC,double &lPar0,double &lPar1,
	     TF1   *iMeanFit0=0,TF1   *iMeanFit1=0,
	     TF1   *iRMSFit0 =0,TF1   *iRMSFit1 =0,
             TF1   *iCorrFit=0) { 
  iC->cd();
  fCorrMax = 70;
  const int iNBins = 7;
  TGraphErrors  **lG0  = new TGraphErrors*[iNBins]; double *lXCorr = new double[iNBins]; double *lCorr = new double[iNBins];
  std::vector<float> lXVals; std::vector<float> lY0Vals; std::vector<float> lY1Vals;
  std::vector<float> lXEVals; std::vector<float> lYEVals; 

  //  for(int i1 = 0; i1 <  iTree->GetEntries(); i1++) {
  for(int i1 = 0; i1 <  10000; i1++) {
    iTree->GetEntry(i1);
    if(!passId(fId)) continue;
    calculateU1U2(lPar0,true);
    calculateU1U2(lPar1,false);
    if(fZPt > fZPtMax) continue;
    if(fMet > fMetMax) continue;
    
    /// MARIA comment this for now
    //    if(fNJetSelect<2 && fNJet!=fNJetSelect && fNJetSelect != -1) continue;
    //    if(fNJetSelect==2 && fNJet<2) continue;
   
     double pVal0 = lPar0;
    double pVal1 = lPar1;
    if(iMeanFit0 != 0 && iRMSFit0 != 0 )   pVal0 = (lPar0-iMeanFit0->Eval(fZPt))/iRMSFit0->Eval(fZPt);
    if(iMeanFit1 != 0 && iRMSFit1 != 0 )   pVal1 = (lPar1-iMeanFit1->Eval(fZPt))/iRMSFit1->Eval(fZPt);
    //if( (pVal0 > 0 || pVal1 > 0)) continue;
    if(fWeight == 0) continue;
    double pWeight = fWeight;
    if(fData) pWeight = 1;
    //if(pVal1 > pVal0) continue;
    //if(iRMS && pVal0 > 0. && ((fPFU1 == lPar0) || (fTKU1 == lPar0))) continue;
    //if(iRMS && pVal1 > 0. && ((fPFU1 == lPar1) || (fTKU1 == lPar1))) continue;
    //double pWeight = fWeight;
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
  cin.get();
}
*/

/*
void makePdf(TTree *iTree1,double iPtMin,double iPtMax,RooDataHist *iData,RooHistPdf *iPdf,RooRealVar &iVar,TF1 *iMeanFit,float &lPar) { 
  float lPt   = 0; iTree1->SetBranchAddress("pt_z"   ,&lPt); 
  float lU1   = 0; iTree1->SetBranchAddress("u1x_mva",&lU1); 
  float lU2   = 0; iTree1->SetBranchAddress("u2x_mva",&lU2); 
  float lNJet = 0; iTree1->SetBranchAddress("njet"   ,&lNJet); 
  TH1F *lH0   = new TH1F("pH","pH",100,-100,100);
  for(int i1 = 0; i1 <  iTree1->GetEntries(); i1++) {
    iTree1->GetEntry(i1);
    //if(!passId(fId)) continue;
    if(lPt < iPtMin || lPt > iPtMax ) continue;

    // MARIA comment this for now
    //    if(fNJetSelect<2  && lNJet != fNJetSelect && fNJetSelect != -1) continue;
    //    if(fNJetSelect==2 && lNJet<2) continue;
    
    double pVal = 0;
    if(lPar == fU1) pVal = lU1;
    if(lPar == fU2) pVal = lU2;
    if(iMeanFit != 0)   pVal = (lPar - iMeanFit->Eval(fZPt));
    lH0->Fill(pVal);
  }
  iData = new RooDataHist("Data","Data",RooArgList(iVar),lH0);
  iPdf  = new RooHistPdf ("HPdf","HPdf",iVar,*iData,0);
  delete lH0;
}
*/

void fitGraph(TTree *iTree,TTree *iTree1, TCanvas *iC,
//	      float &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1* iMeanFit=0,
	      double &lPar, TF1 *iFit, TF1 *iFitS1=0, TF1 *iFitS2=0, TF1* iMeanFit=0,
	      bool iPol1 = false, 
	      bool iRMS  = false, int iType = 0) { 

  //RooFit Build a double Gaussian                                                                                                                        
  TRandom lRand(0xDEADBEEF);
  RooRealVar    lRWeight("weight","weight",0,10);
  ////  RooRealVar lRPt  ("pt","Z_{p_{T}}",10,1000);
  RooRealVar lRPt  ("pt","Z_{p_{T}}",0,fZPtMax);

  RooRealVar lRXVar("XVar","(U_{1}(Z_{p_{T}})-x_{i})/#sigma_{U1} (Z_{p_{T}})",0,-5,5);
  if(lPar!=fU1) lRXVar.SetTitle("(U_{2}(Z_{p_{T}})-x_{i})/#sigma_{U2} (Z_{p_{T}})");

  // this for the 2D Fit (is a cubic function to take into account the correlation with the pt)                                                           

  ////// those are the 2D histograms                                                                                                                      
  // ===> Those are the variable fitted                                                                                                                   
  // lA1Sig,lB1Sig,lC1Sig,lA2Sig,lB2Sig,lC2Sig,                                                                                                           
  // those are set constant (0) lD1Sig, lD2Sig, lR1Mean    
  RooRealVar    lA1Sig("a1sig","a1sig",0.8,0,1);
  RooRealVar    lB1Sig("b1sig","b1sig",0. ,-0.1,0.1);
  RooRealVar    lC1Sig("c1sig","c1sig",0. ,-0.1,0.1);  lC1Sig.setConstant(kTRUE);
  RooRealVar    lD1Sig("d1sig","d1sig",0. ,-0.1,0.1);  lD1Sig.setConstant(kTRUE);
  RooRealVar    lA2Sig("a2sig","a2sig",1.4,1,3);
  RooRealVar    lB2Sig("b2sig","b2sig",0.0 ,-0.1,0.1);
  RooRealVar    lC2Sig("c2sig","c2sig",0.0 ,-0.1,0.1);  lC2Sig.setConstant(kTRUE);
  RooRealVar    lD2Sig("d2sig","d2sig",0.0 ,-0.1,0.1);  lD2Sig.setConstant(kTRUE);
  RooRealVar    lR1Mean("mean","mean",0,-10,10); lR1Mean.setConstant(kTRUE); //                                                                           

  // ===> This is the model for the 2D                                                                                                                    
  RooFormulaVar l1Sigma("sigma1","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA1Sig,lB1Sig,lC1Sig,lRPt,lD1Sig));
  RooFormulaVar l2Sigma("sigma2","@0+@1*@3+@2*@3*@3+@4*@3*@3*@3",RooArgSet(lA2Sig,lB2Sig,lC2Sig,lRPt,lD2Sig));
  RooFormulaVar lFrac  ("frac"  ,"(@1-1.)/(@1-@0)",RooArgSet(l1Sigma,l2Sigma));
  RooGaussian   lGaus1("gaus1","gaus1",lRXVar,lR1Mean,l1Sigma);
  RooGaussian   lGaus2("gaus2","gaus2",lRXVar,lR1Mean,l2Sigma);
  RooAddPdf     lGAdd("Add","Add",lGaus1,lGaus2,lFrac);

  ////// those are the 1D histograms                                                                                                                      
  // ===> Those are the variable fitted                                                                                                                   
  // Rfrac, Rmean, Rsigma1, Rsigma2                                                                                                                       
  //   RooRealVar    lR1Sigma("Rsigma1","RSigma1",8. ,0,40);                                                                                              
  //   RooRealVar    lR2Sigma("Rsigma2","RSigma2",12.,1,40);                                                                                              
  //   RooRealVar    lR1Frac ("Rfrac"  ,"Rfrac"  ,0.5,0,1);                                                                                               
  RooRealVar    lR1Sigma("Rsigma1","RSigma1",1. ,0,40);
  RooRealVar    lR2Sigma("Rsigma2","RSigma2",12.,1,40);
  RooRealVar    lR1Frac ("Rfrac"  ,"Rfrac"  ,0.5,0.,1.0);
  RooRealVar    lR2Mean ("Rmean",  "Rmean",0,-10,10); lR2Mean.setConstant(kTRUE); //                                                                   

  // ===> This is the model for the 1D
  RooGaussian   lRGaus1("Rgaus1","Rgaus1",lRXVar,lR2Mean,lR1Sigma);
  RooGaussian   lRGaus2("Rgaus2","Rgaus2",lRXVar,lR2Mean,lR2Sigma);
  RooAddPdf     lRGAdd ("RAdd"  ,"RAdd",lRGaus1,lRGaus2,lR1Frac);

  if(iRMS){   
    for(int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
      // pVal = (lPar - iMeanFit->Eval(fZPt));
      double temp = vlYVals_all[lPar!=fU1][iRMS][iev]; // 
      if(iMeanFit != 0) temp = vlYVals_all[lPar!=fU1][iRMS][iev] - iMeanFit->Eval(vlXVals_all[lPar!=fU1][iRMS][iev]);
      vlYVals_all[lPar!=fU1][iRMS][iev] = abs(temp); // absolute value of residuals
      vlYTVals_all[lPar!=fU1][iRMS][iev] = temp;  // residuals
    }
  }

  // MARIA something to add to the plots
  TString leg = "#sigma_{mean} = ";   
  //  leg += lY0[i0];
  
  TLatex latexLabel;    
  latexLabel.SetTextSize(0.04);
  latexLabel.SetNDC();
  //    latexLabel.DrawLatex(0.2, 0.7, leg);
  
  TString leg1 = "NVtx ";  
  if(fId == 0) leg1 += " = 0-12";

  if(fId == 1) leg1 += " = 1";
  if(fId == 2) leg1 += " = 2";
  if(fId == 3) leg1 += " = 3";
  if(fId == 4) leg1 += " = 4";
  if(fId == 5) leg1 += " = 5";
  if(fId == 6) leg1 += " = 6";
  if(fId == 7) leg1 += " = 7";
  if(fId == 8) leg1 += " = 8";
  if(fId == 9) leg1 += " = 9";
  if(fId == 10) leg1 += " = 10";
  if(fId == 11) leg1 += " = 11";
  if(fId == 12) leg1 += " = 12";
  if(fId == 13) leg1 += " = 13";
  if(fId == 14) leg1 += " = 14";
  if(fId == 15) leg1 += " = 15";
  if(fId == 16) leg1 += " = 16";
  if(fId == 17) leg1 += " = 17";
  if(fId == 18) leg1 += " = 18";
  if(fId == 19) leg1 += " = 19";
  if(fId >= 20) leg1 += " >= 20";

  TString legDATA = "DATA";  
  if(!fData) legDATA = "MC";
  if(!fData && doPosW) legDATA += " Wpos";
  if(!fData && doNegW) legDATA += " Wneg";

  TString legU1U2 = "U1";  
  if(lPar!=fU1) legU1U2 = "U2";

  iC->cd();
  //Basic Fit
  //TGraphErrors *pGraphA = new TGraphErrors(lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]),(Float_t*)&(lXEVals[0]),(Float_t*)&(lYEVals[0]));
  // TGraph *pGraphA       = new TGraph      (lXVals.size(),(Float_t*)&(lXVals[0]),(Float_t*)&(lYVals[0]));

  TGraph *pGraphA       = new TGraph(lXVals_all.at(0).at(0).size(), 
                                     vlXVals_all[0][0], 
                                     vlYVals_all[lPar!=fU1][iRMS]
                                     );

  std::string lType     = "pol3"; if(iPol1) lType = "pol2"; //pol1 -->higgs 
  if(iType == 1)  lType = "pol3"; 

  lType     = "pol2";
  //http://root.cern.ch/root/html/TGraph.html
  //"EX0" When fitting a TGraphErrors or TGraphAsymErrors do not consider errors in the coordinate
  //"R"   Use the Range specified in the function range
  //"S"  The result of the fit is returned in the TFitResultPtr
  //"E"  Perform better Errors estimation using Minos technique
  
  //// THIS IS THE unbinned Fit
  TF1 *lFit = new TF1("test",lType.c_str()); //fZPtMax = 200;
  //  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SR","EXO",fZPtMin,fZPtMax); //"EXO"
  TFitResultPtr  lFitPtr = pGraphA->Fit(lFit,"SRE","",fZPtMin,fZPtMax); 
  computeFitErrors(iFit,lFitPtr,lFit,iRMS);
  pGraphA->Draw("ap"); // use colz for colorful plot
  drawErrorBands(iFit,fZPtMax);

  if(lPar==fU1) pGraphA->GetYaxis()->SetTitle("U1"); 
  if(lPar!=fU1) pGraphA->GetYaxis()->SetTitle("U2"); 
  pGraphA->GetXaxis()->SetTitle("Z p_{T}"); 

  latexLabel.DrawLatex(0.25, 0.8, leg1);
  latexLabel.DrawLatex(0.25, 0.25, legDATA);
  latexLabel.DrawLatex(0.25, 0.3, legU1U2);

  if(doPrint) {
    
    TString test="";
    
    if(iRMS==0) test += "DrawUnbinned_mean";
    if(!iRMS==0) test += "DrawUnbinned_RMS";
    
    if(lPar==fU1) test += "_U1";
    if(lPar!=fU1) test += "_U2";
    
    if(fData) test += "_data";
    if(!fData) test += "_MC";

    if(doPosW) test += "_Wpos";
    if(doNegW) test += "_Wneg";

    test += "_vtx";
    test += fId;
    test += ".png";
    
    iC->SaveAs(test.Data());
    
    /*
    if(fData) {
      
      if(fId ==1 && !iRMS && (lPar==fU1)) iC->SaveAs("DrawUnbinned_U1_mean_data.png");
      if(fId ==1 && iRMS && (lPar==fU1)) iC->SaveAs("DrawUnbinned_U1_RMS_data.png");
      
      if(fId ==1 && !iRMS && (lPar!=fU1)) iC->SaveAs("DrawUnbinned_U2_mean_data.png");
      if(fId ==1 && iRMS && (lPar!=fU1)) iC->SaveAs("DrawUnbinned_U2_RMS_data.png");
      
    } else {
      
      if(fId ==1 && !iRMS && (lPar==fU1)) iC->SaveAs("DrawUnbinned_U1_mean_MC.png");
      if(fId ==1 && iRMS && (lPar==fU1)) iC->SaveAs("DrawUnbinned_U1_RMS_MC.png");
      
      if(fId ==1 && !iRMS && (lPar!=fU1)) iC->SaveAs("DrawUnbinned_U2_mean_MC.png");
      if(fId ==1 && iRMS && (lPar!=fU1)) iC->SaveAs("DrawUnbinned_U2_RMS_MC.png");
      
    }
    */

  }

  if(!iRMS) return;

  //Build the double gaussian dataset
  std::vector<RooDataSet> lResidVals; 
  std::vector<RooDataHist> lResidVals2D; 
  std::stringstream pSS; pSS << "Crapsky" << 0;
  pSS << "2D"; RooDataHist lData2D(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar,lRPt));
  //  int lNBins = 10;
  int lNBins = 25;
  for(int i0  = 0; i0 < lNBins; i0++) { 
    RooDataSet lData(pSS.str().c_str(),pSS.str().c_str(),RooArgSet(lRXVar)); 
    lResidVals.push_back(lData); // binned (1D)
  }
  // lRPt.setBins(500);
  // lRPt.setBins(fZPtMax);
  lRPt.setBins(fZPtMax*50);
  lResidVals2D.push_back(lData2D); // unbinned (2D)
  std::vector<float> lX3SVals; std::vector<float> lXE3SVals; std::vector<float> lY3SVals; std::vector<float> lYE3SVals; //Events with sigma > 3
  std::vector<float> lX4SVals; std::vector<float> lXE4SVals; std::vector<float> lY4SVals; std::vector<float> lYE4SVals; //Events with sigma > 3
  
  for(unsigned int i0 = 0; i0 < lXVals_all.at(0).at(0).size(); i0++) {  // for all the data points
    double lYTest = lFit->Eval(vlXVals_all[lPar!=fU1][iRMS][i0])*sqrt(2*3.14159265)/2.;
    lRXVar.setVal(vlYTVals_all[lPar!=fU1][iRMS][i0]/(lYTest)); // pull on RMS
    lRPt.setVal(vlXVals_all[lPar!=fU1][iRMS][i0]);
    lRWeight.setVal(1./vlYEVals_all[lPar!=fU1][iRMS][i0]/vlYEVals_all[lPar!=fU1][iRMS][i0]);
    int pId = int(vlXVals_all[lPar!=fU1][iRMS][i0]/(fZPtMax/lNBins)); if(pId > lNBins-1) pId = lNBins-1; 
    lResidVals[pId].add(RooArgSet(lRXVar));//,lRWeight.getVal()); //Fill the Double Gaussian 1D
    lResidVals2D[0].add(RooArgSet(lRXVar,lRPt));//,lRWeight.getVal()); //Fill the Double Gaussian 2D
  }
  lGAdd.mustBeExtended();
  RooFitResult *pFR = lGAdd.fitTo(lResidVals2D[0],Save(kTRUE),ConditionalObservables(lRPt),NumCPU(2),Minos());//,Minos());//,Minos()); //Double Gaussian fit

  //// AFTER here set up for the 1D fit                                                                                                                    
  if(doPrintAll) {

    //Plot it all
    lRXVar.setBins(100); // Bins of the binned Pull
    
    double *lX   = new double[lNBins];
    double *lY0  = new double[lNBins];
    double *lY1  = new double[lNBins];
    double *lY2  = new double[lNBins];
    
    double *lEX  = new double[lNBins];
    double *lEY0 = new double[lNBins];
    double *lEY1 = new double[lNBins];
    double *lEY2 = new double[lNBins];

    double *lchi2  = new double[lNBins];
    double *myFrac = new double[lNBins];
    double *myMean = new double[lNBins];
    double *myFracE = new double[lNBins];
    double *myMeanE = new double[lNBins];

    for(int i0  = 0; i0 < lNBins; i0++) {
      lRGAdd.fitTo(lResidVals[i0],Save(kTRUE),NumCPU(2));//,Minos());//,Minos()); //Double Gaussian fit
      // lRPt.setRange(i0*10,i0*10+10);
      RooPlot *lFrame1 = lRXVar.frame(Title("XXX")) ;
      //lResidVals2D[0].plotOn(lFrame1);
      lResidVals [i0].plotOn(lFrame1,RooFit::MarkerColor(kRed));
      lRGAdd.plotOn(lFrame1);
      lRGAdd.plotOn(lFrame1,RooFit::Components(lRGaus1),RooFit::LineStyle(kDashed),RooFit::LineColor(kRed));
      lRGAdd.plotOn(lFrame1,RooFit::Components(lRGaus2),RooFit::LineStyle(kDashed),RooFit::LineColor(kViolet));
      iC->cd(); lFrame1->Draw();
      
      Double_t chi21D = lFrame1->chiSquare();
      TString chi2str = "chi2 = ";
      chi2str += chi21D;
      latexLabel.DrawLatex(0.75, 0.8, chi2str);
      TString frac1str = "frac = ";
      frac1str += lR1Frac.getVal();
      latexLabel.DrawLatex(0.75, 0.7, frac1str);

      lchi2[i0]=chi21D;
      myFrac[i0] = lR1Frac.getVal();
      myMean[i0] = lR2Mean.getVal();
      myFracE[i0] = lR1Frac.getError();
      myMeanE[i0] = lR2Mean.getError();

      //cin.get();    
      lX[i0] = (fZPtMax/lNBins)*i0;
      lEX[i0] = (fZPtMax/lNBins)/10;
      lY0[i0] = (lR1Frac .getVal()*lR1Sigma.getVal() + (1.-lR1Frac.getVal())*lR2Sigma.getVal())/* /sqrt(2*3.14159265)*2. */;
      lY1[i0] = lR1Sigma.getVal()/* /sqrt(2*3.14159265)*2. */;
      lY2[i0] = lR2Sigma.getVal()/* /sqrt(2*3.14159265)*2. */;
      lEY0[i0]  = lR1Frac .getError()*lR1Sigma.getVal()  * lR1Frac .getError()*lR1Sigma.getVal();
      lEY0[i0] += lR1Frac .getVal()  *lR1Sigma.getError()* lR1Frac .getVal()  *lR1Sigma.getError();
      lEY0[i0] += lR1Frac .getError()*lR2Sigma.getVal()  * lR1Frac .getError()*lR2Sigma.getVal();
      lEY0[i0] += (1-lR1Frac .getVal())*lR2Sigma.getError()* (1-lR1Frac .getVal())  *lR2Sigma.getError();
      lEY0[i0] = sqrt(lEY0[i0])/* /sqrt(2*3.14159265)*2. */;
      lEY1[i0] = lR1Sigma.getError()/* /sqrt(2*3.14159265)*2. */;
      lEY2[i0] = lR2Sigma.getError()/* /sqrt(2*3.14159265)*2. */;
      
      //    if(doPrintAll) {
      
      latexLabel.DrawLatex(0.25, 0.8, leg1);
      
      TString leg2 = "Zpt = ";  
      if(i0 == 0) leg2 += "0-5"; 
      if(i0 == 1) leg2 += "5-10"; 
      if(i0 == 2) leg2 += "10-15"; 
      if(i0 == 3) leg2 += "15-20"; 
      if(i0 == 4) leg2 += "20-25"; 
      if(i0 == 5) leg2 += "25-30"; 
      if(i0 == 6) leg2 += "30-35"; 
      if(i0 == 7) leg2 += "35-40"; 
      if(i0 == 8) leg2 += "40-45"; 
      if(i0 == 9) leg2 += "45-50"; 
      
      latexLabel.DrawLatex(0.25, 0.7, leg2);
      latexLabel.DrawLatex(0.25, 0.25, legDATA);
      latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

      //      stringstream pSS1; 
      TString test="";
      
      if(iFitS1==0) test += "mean";
      if(iFitS1!=0) test += "rms";
      
      if(lPar==fU1) test += "_U1";
      if(lPar!=fU1) test += "_U2";
      
      if(fData) test += "_data";
      if(!fData) test += "_MC";

      if(doPosW) test += "_Wpos";
      if(doNegW) test += "_Wneg";
      
      test += "_Zpt";
      test += i0;

      test += "_jets";
      test += fNJetSelect;
      test += "_vtx";
      test += fId;
      test += ".png";
      
      /*
	if(lPar==fU1) {
	if(fData) pSS1 << "Canvas_U1_"<< i0 << "_data_" << fNJetSelect << "_"<< fId << ".png";
	if(!fData) pSS1 << "Canvas_U1_" << i0 << "_MC_" << fNJetSelect << "_"<< fId << ".png";
	} else {
	if(fData) pSS1 << "Canvas_U2_"<< i0 << "_data_" << fNJetSelect << "_"<< fId << ".png";
	if(!fData) pSS1 << "Canvas_U2_" << i0 << "_MC_" << fNJetSelect << "_"<< fId << ".png";
	}
	iC->SaveAs(pSS1.str().c_str());
      */
      iC->SaveAs(test.Data());
      
    }
    //  }


    TGraphErrors *lM0 = new TGraphErrors(lNBins,lX,myMean,lEX,lEY0); lM0->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG0 = new TGraphErrors(lNBins,lX,lY0,lEX,lEY0); lG0->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG1 = new TGraphErrors(lNBins,lX,lY1,lEX,lEY1); lG1->SetMarkerStyle(kFullCircle);
    TGraphErrors *lG2 = new TGraphErrors(lNBins,lX,lY2,lEX,lEY2); lG2->SetMarkerStyle(kFullCircle);
    
    if(lPar==fU1) lM0->GetYaxis()->SetTitle("U1 mean");
    if(lPar!=fU1) lM0->GetYaxis()->SetTitle("U2 mean");
    if(lPar==fU1) lG0->GetYaxis()->SetTitle("U1 #sigma_{mean}");
    if(lPar!=fU1) lG0->GetYaxis()->SetTitle("U2 #sigma_{mean}");
    lG1->GetYaxis()->SetTitle("#sigma_{1}");
    lG2->GetYaxis()->SetTitle("#sigma_{2}");
    
    lM0->GetYaxis()->SetRangeUser(-0.5,0.5);
    lG0->GetYaxis()->SetRangeUser(0.5,1.5);
    lG1->GetYaxis()->SetRangeUser(0.,2.0);
    lG2->GetYaxis()->SetRangeUser(0.,2.0);
    
    lM0->GetXaxis()->SetTitle("Z p_{T}");
    lG0->GetXaxis()->SetTitle("Z p_{T}");
    lG1->GetXaxis()->SetTitle("Z p_{T}");
    lG2->GetXaxis()->SetTitle("Z p_{T}");
    
    //////
    //// cin.get();
    //////
    
    //  if(doPrintAll) {
    
    iC->cd();
    TF1 *lFitPull  = new TF1("lFitPull",   "pol2");
    TF1 *iFitPull  = new TF1("iFitPull",   "pol2");
    TFitResultPtr  lFitPtr0 = lG0->Fit(lFitPull,"SRE","", fZPtMin, fZPtMax); //"EXO"                                                                       
    computeFitErrors(iFitPull,lFitPtr0,lFitPull,iRMS);
    lG0->Draw("ape");
    drawErrorBands(iFitPull,fZPtMax);
    lG0->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    TString pSS0="G0";
    
    if(lPar==fU1) pSS0 += "_U1";
    if(lPar!=fU1) pSS0 += "_U2";
    
    if(fData) pSS0 += "_data";
    if(!fData) pSS0 += "_MC";
    
    if(doPosW) pSS0 += "_Wpos";
    if(doNegW) pSS0 += "_Wneg";
    
    pSS0 += "_";
    pSS0 += fNJetSelect;
    pSS0 += "_";
    pSS0 += fId;
    pSS0 += ".png";
    
    iC->SaveAs(pSS0.Data());



    /*
    stringstream pSS0;
    if(lPar==fU1) { 
      if(fData) pSS0 << "G0_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    } else {
      if(fData) pSS0 << "G0_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    }

    iC->SaveAs(pSS0.str().c_str());
    */

    ///////                                                                                                                                                


    /*
    stringstream pSS0;
    if(lPar==fU1) { 
      if(fData) pSS0 << "G0_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    } else {
      if(fData) pSS0 << "G0_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pSS0 << "G0_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    }

    iC->SaveAs(pSS0.str().c_str());
    */

    ///////                                                                                                                                                
    ///////                                                                                                                                                

    TF1 *lFitPullMean  = new TF1("lFitPullMean",   "pol2");
    TF1 *iFitPullMean  = new TF1("iFitPullMean",   "pol2");
    TFitResultPtr  lFitPtrM0 = lM0->Fit(lFitPullMean,"SRE","", fZPtMin, fZPtMax); //"EXO"                                                                  
    computeFitErrors(iFitPullMean,lFitPtrM0,lFitPullMean,iRMS);
    lM0->Draw("ape");
    drawErrorBands(iFitPullMean,fZPtMax);
    lM0->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);

    TString pMM0="M0";

    if(lPar==fU1) pMM0 += "_U1";
    if(lPar!=fU1) pMM0 += "_U2";

    if(fData) pMM0 += "_data";
    if(!fData) pMM0 += "_MC";

    if(doPosW) pMM0 += "_Wpos";
    if(doNegW) pMM0 += "_Wneg";

    pMM0 += "_";
    pMM0 += fNJetSelect;
    pMM0 += "_";
    pMM0 += fId;
    pMM0 += ".png";

    iC->SaveAs(pMM0.Data());


    //////
    //// cin.get();
    //////
    
    iC->cd();
    //  TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    TFitResultPtr  lFitPtr1 = lG1->Fit(lFit,"SRE","", fZPtMin, fZPtMax); //"EXO"
    computeFitErrors(iFitS1,lFitPtr1,lFit,iRMS);
    lG1->Draw("ape");
    drawErrorBands(iFitS1,fZPtMax);
    lG1->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    TString pSS1="pG1";
    
    if(lPar==fU1) pSS1 += "_U1";
    if(lPar!=fU1) pSS1 += "_U2";
    
    if(fData) pSS1 += "_data";
    if(!fData) pSS1 += "_MC";
    
    if(doPosW) pSS1 += "_Wpos";
    if(doNegW) pSS1 += "_Wneg";
    
    pSS1 += "_";
    pSS1 += fNJetSelect;
    pSS1 += "_";
    pSS1 += fId;
    pSS1 += ".png";
    
    iC->SaveAs(pSS1.Data());

    /*
     stringstream pG1;
    if(lPar==fU1) {  
    if(fData) pG1 << "pG1_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
    if(!fData) pG1 << "pG1_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    } else {
    if(fData) pG1 << "pG1_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
    if(!fData) pG1 << "pG1_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
    }
    //  iC->SaveAs("G1.png");
    iC->SaveAs(pG1.str().c_str());
    */
    
    //////
    //// cin.get();
    //////
    
    iC->cd();
    //  TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","",0,fZPtMax); 
    //    TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SR","EXO",0,fZPtMax); //"EXO"
    TFitResultPtr  lFitPtr2 = lG2->Fit(lFit,"SRE","", fZPtMin, fZPtMax); 
    computeFitErrors(iFitS2,lFitPtr2,lFit,iRMS);
    lG2->Draw("ape");
    drawErrorBands(iFitS2,fZPtMax);
    lG2->Draw("pe");

    latexLabel.DrawLatex(0.25, 0.8, leg1);
    latexLabel.DrawLatex(0.25, 0.25, legDATA);
    latexLabel.DrawLatex(0.25, 0.3, legU1U2);      

    TString pSS2="pG2";
    
    if(lPar==fU1) pSS2 += "_U1";
    if(lPar!=fU1) pSS2 += "_U2";
    
    if(fData) pSS2 += "_data";
    if(!fData) pSS2 += "_MC";
    
    if(doPosW) pSS2 += "_Wpos";
    if(doNegW) pSS2 += "_Wneg";
    
    pSS2 += "_";
    pSS2 += fNJetSelect;
    pSS2 += "_";
    pSS2 += fId;
    pSS2 += ".png";
    
    iC->SaveAs(pSS2.Data());    

    /*
      stringstream pG2; 
      if(lPar==fU1) {
      if(fData) pG2 << "pG2_U1" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pG2 << "pG2_U1" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
      } else {
      if(fData) pG2 << "pG2_U2" << "_data_" << fNJetSelect << "_"<< fId << ".png";
      if(!fData) pG2 << "pG2_U2" << "_MC_" << fNJetSelect << "_"<< fId << ".png";
      }
      
      iC->SaveAs(pG2.str().c_str());
    // cin.get();
    */

  return;
  } // end print


  // MARIA comment for now

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

void fitRecoil1(TTree * iTree, /*float &iVPar,*/ double &iVPar,
	       TF1 *iMFit, TF1 *iMRMSFit, TF1 *iRMS1Fit, TF1 *iRMS2Fit,int iType) { 
  TCanvas *lXC1 = new TCanvas("C1","C1",800,600); lXC1->cd(); 

  // fit scale
  fitGraph(iTree,fTTbar,lXC1,iVPar,iMFit   ,0       ,0       ,0    ,true ,false,iType);
  // fit RMS (uses scale fit info)
  fitGraph(iTree,fTTbar,lXC1,iVPar,iMRMSFit,iRMS1Fit,iRMS2Fit,iMFit,false,true ,iType);

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

/*
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
*/
void loopOverTree(TTree *iTree) {

  cout << "looping over tree" << endl;

  for(int i1 = 0; i1 <  iTree->GetEntries(); i1++) {
    //  for(int i1 = 0; i1 <  1e6; i1++) {
    iTree->GetEntry(i1);

    ////////    cout << "RUN " << fRun << " LUMI " << fLumi << " EVENT " << fEvent << endl;

    // for MC select only odd
    if(!fData && fEvent % 2) continue;

    if(doRunA && fEvent>175832) continue;
    if(doRunB && fEvent<175832) continue;

    if(!passId(fId)) continue;

    // this is Z selection
    if((!doPosW) && (!doNegW) && (!runSelection()))  continue;

    // this is for the W selection
    if(doPosW && (!runWSelection(true))) continue;
    if(doNegW && (!runWSelection(false))) continue;
    
    //    cout << "passed selection " << endl;

    if(doMetPhiCorr) {
      fMet  = getPhiCorrMET(1);
      fMPhi = getPhiCorrMET(2);
    }

    //if(!passMatching()) continue;
    //if(fPt2 > 30) fNJet++;
    
    /*
    //MARIA comment for now
    ////    if(fNBTag > 0) continue;
    if(iPol1 && fZPt > 50. && lPar == fU1 && lPar  > (47.5-0.95*fZPt) && iType == 0) continue; //Remove Diboson events at high pT
    */

    if(fZPt > fZPtMax) continue;
    if(fMet > fMetMax) continue;

    /* MARIA comment for now
       if(fNJetSelect<2  && fNJet != fNJetSelect && fNJetSelect != -1) continue;
       if(fNJetSelect==2 && fNJet<2) continue;
    */

    double lPar;
    // }
    bool U1U2truefalse[2]={true,false};
    bool MeanRMStruefalse[2]={false,true};
    
    for(int iU1U2=0; iU1U2<2; iU1U2++){
        std::vector<std::vector<float> > dummy;
        lXVals_all.push_back(dummy); lXEVals_all.push_back(dummy); lYVals_all.push_back(dummy); lYTVals_all.push_back(dummy); lYEVals_all.push_back(dummy);
      
      for(int iMeanRMS=0; iMeanRMS<2; iMeanRMS++){
        std::vector<float> dummy1;
        lXVals_all.at(iU1U2).push_back(dummy1); lXEVals_all.at(iU1U2).push_back(dummy1); 
        lYVals_all.at(iU1U2).push_back(dummy1); lYTVals_all.at(iU1U2).push_back(dummy1); lYEVals_all.at(iU1U2).push_back(dummy1);
        
        bool isU1=U1U2truefalse[iU1U2];
	//	bool iRMS=MeanRMStruefalse[iMeanRMS];
        
        calculateU1U2(lPar,isU1);
        double pVal = lPar;    
        // if(iRMS && iMeanFit != 0)   pVal = (lPar - iMeanFit->Eval(fZPt));
        if(fWeight == 0) continue;
        lXVals_all.at(iU1U2).at(iMeanRMS).push_back(fZPt);  

	lXEVals_all.at(iU1U2).at(iMeanRMS).push_back(2.);

        double pWeight = fWeight;
        if(fData) pWeight = 1;
        // (!iRMS) ? lYVals_all.at(iU1U2).at(iMeanRMS).push_back(pVal) : lYVals_all.at(iU1U2).at(iMeanRMS).push_back(fabs(pVal));
        lYVals_all.at(iU1U2).at(iMeanRMS).push_back(pVal);
        lYTVals_all.at(iU1U2).at(iMeanRMS).push_back(pVal); 
        lYEVals_all.at(iU1U2).at(iMeanRMS).push_back(1./sqrt(pWeight));

      }
    }
  }

  vlXVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlXEVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlYVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlYTVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  vlYEVals_all=new float**[2]; //creates a new array of pofloaters to float objects
  
  for(int iU1U2=0; iU1U2<2; ++iU1U2){
    vlXVals_all[iU1U2]=new float*[2];
    vlXEVals_all[iU1U2]=new float*[2];
    vlYVals_all[iU1U2]=new float*[2];
    vlYTVals_all[iU1U2]=new float*[2];
    vlYEVals_all[iU1U2]=new float*[2];
      for(int iMeanRMS=0; iMeanRMS<2; ++iMeanRMS){
        vlXVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlXEVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlYVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlYTVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        vlYEVals_all[iU1U2][iMeanRMS]=new float[lXVals_all.at(0).at(0).size()];
        for(int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
          vlXVals_all[iU1U2][iMeanRMS][iev]  = lXVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlXVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlXEVals_all[iU1U2][iMeanRMS][iev] = lXEVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlXEVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYVals_all[iU1U2][iMeanRMS][iev]  = lYVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlYVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYTVals_all[iU1U2][iMeanRMS][iev] = lYTVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlYTVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
          vlYEVals_all[iU1U2][iMeanRMS][iev] = lYEVals_all.at(iU1U2).at(iU1U2).at(iev);
          // cout << "vlYEVals_all["<<iU1U2<<"]["<<iMeanRMS<<"]["<<iev<<"] = "<<lXVals_all.at(iU1U2).at(iU1U2).at(iev)<<endl;
        }
      }
  }
  
  // for(int iev=0; iev<lXVals_all.at(0).at(0).size(); iev++){
    // cout << "iev= " << iev << endl;
    // for(int iU1U2=0; iU1U2<2; iU1U2++){
      // for(int iMeanRMS=0; iMeanRMS<2; iMeanRMS++){
        // cout << "iU1U2=" << iU1U2 << " iMeanRMS= " << iMeanRMS << endl;
        // cout << "lXVals_all= " << lXVals_all.at(iU1U2).at(iMeanRMS).at(iev)
             // << " lXEVals_all= " << lXEVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << " lYVals_all= " << lYVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << " lYTVals_all= " << lYTVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << " lYEVals_all= " << lYEVals_all.at(iU1U2).at(iMeanRMS).at(iev) 
             // << endl;
      // }
    // }
  // }

}

void fitRecoilMET(TTree *iTree,std::string iName,int type, int fId) { 
  std::string lPrefix="PF"; if(type == 1) lPrefix = "TK";
  load(iTree,type);

  loopOverTree(iTree);
  //  fU2=fU2*(-1);

  std::stringstream PUstring;
  PUstring << fId;

  TF1 *lU1Fit     = new TF1((lPrefix+"u1Mean_"+PUstring.str()).c_str(),   "pol2");
  TF1 *lU1MRMSFit = new TF1((lPrefix+"u1MeanRMS_"+PUstring.str()).c_str(),"pol2");
  TF1 *lU1RMS1Fit = new TF1((lPrefix+"u1RMS1_"+PUstring.str()).c_str(),   "pol2");
  TF1 *lU1RMS2Fit = new TF1((lPrefix+"u1RMS2_"+PUstring.str()).c_str(),   "pol2");
  TF1 *lU2Fit     = new TF1((lPrefix+"u2Mean_"+PUstring.str()).c_str(),   "pol2");  
  TF1 *lU2MRMSFit = new TF1((lPrefix+"u2MeanRMS_"+PUstring.str()).c_str(),"pol2");
  TF1 *lU2RMS1Fit = new TF1((lPrefix+"u2RMS1_"+PUstring.str()).c_str(),   "pol2");
  TF1 *lU2RMS2Fit = new TF1((lPrefix+"u2RMS2_"+PUstring.str()).c_str(),   "pol2");

  //  cout << "doing U1 " << endl;
  fitRecoil1(iTree,fU1,lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,type);
  //  cout << "doing U2 " << endl;
  fitRecoil1(iTree,fU2,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,type);

  writeRecoil(lU1Fit,lU1MRMSFit,lU1RMS1Fit,lU1RMS2Fit,lU2Fit,lU2MRMSFit,lU2RMS1Fit,lU2RMS2Fit,iName);

}

/*
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
*/

/*
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
*/

void fitRecoil(TTree *iTree,std::string iName,int fId) { 
  fitRecoilMET(iTree,iName,2,fId);
  //  fitRecoilMET(iTree,iName,0);
  //fitRecoilMET(iTree,iName,1); 
  //  makeCorr(iTree,iName);
}

void runRecoilFit_v6(int MCtype, int iloop) { 
  //  Prep();

  gStyle->SetOptFit(111111);

  fNJetSelect = -1; fMetMax = 500; fZPtMin = 0; fZPtMax = 50; 

  // THIS IS FOR FINE BINNING
  if(MCtype==100) {
    
    //    for(int iloop=1; iloop < 21; iloop++) {
      
      cout << "PROCESSING DY MC " << endl;
      
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/DYJets/ZTreeProducer_tree_SignalRecoSkimmed.root");
      
      fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
      fData = false; fId = iloop;
      fitRecoil(fDataTree,"recoilfits/recoilfit_genZ_inc.root", fId);

      //    }      

  }

  if(MCtype==200) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      cout << "PROCESSING DY DATA " << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
      fData = true; fId = iloop;
      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_inc.root",fId);
      
      //    }      

  }

  if(MCtype==201) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      cout << "PROCESSING DY DATA " << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
      fData = true; fId = iloop; doRunA=true;
      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_runA_inc.root",fId);
      //    }      

  }


  if(MCtype==202) {

      cout << "PROCESSING DY DATA " << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataTree = (TTree*) fDataFile->FindObjectAny("ZTreeProducer");
      fData = true; fId = iloop; doRunB=true;
      fitRecoil(fDataTree,"recoilfits/recoilfit_DATA_runB_inc.root",fId);
      //    }      

  }




  if(MCtype==300) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      doPosW=true;

      cout << "PROCESSING W MC" << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");

      fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
      fData = false; fId = iloop;
      fitRecoil(fDataTree,"recoilfits/recoilfit_genWpos_inc.root",fId);
      
      //    }      

  }


  if(MCtype==400) {

    //    for(int iloop=20; iloop < 21; iloop++) {

      doNegW=true;

      cout << "PROCESSING W MC" << endl;
      //    fDataFile = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/DATA/ZTreeProducer_tree_RecoSkimmed.root");
      fDataFile = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2012_07_02/WJets/WTreeProducer_tree_SignalRecoSkimmed.root");

      fDataTree = (TTree*) fDataFile->FindObjectAny("WTreeProducer");
      fData = false; fId = iloop;
      fitRecoil(fDataTree,"recoilfits/recoilfit_genWneg_inc.root",fId);
      
      //    }      

  }



  return;

}
