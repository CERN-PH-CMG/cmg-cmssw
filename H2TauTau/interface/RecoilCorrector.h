#ifndef CMGTools_H2TauTau_RecoilCorrector_H
#define CMGTools_H2TauTau_RecoilCorrector_H

#include <vector>
#include <sstream>
#include <string>
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom1.h"

//
// ** apply phil's recoil corrections **
// 
// usage: 
//    double met=rawMetValue;
//    double metphi=rawMetPhiValue;
//    RecoilCorrector corrector;
//    corrector->Correct(met,metphi,GenZPt,GenZPhi,leptonPt,leptonPhi);
//    printf("corrected met: %10.2f%10.2f\n",met,metphi);
//
// where leptonPt, leptonPhi are dilepton kinematics for z->ll and single lepton kinematics for w->lnu
//


using namespace std;

class RecoilCorrector
{

public:
  RecoilCorrector(string iNameZDat = "/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfitZDat.root",
		  string iNameZMC  = "/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfitZMC.root",
		  int iSeed=0xDEADBEEF);
  void Correct(double &met, double &metphi, double lGenPt, double lGenPhi, double lepPt, double lepPhi);
  
protected:
  int readRecoil(std::vector<double> &iSumEt,
		 std::vector<TF1*> &iU1Fit,std::vector<TF1*> &iU1MRMSFit,std::vector<TF1*> &iU1RMS1Fit,
		 std::vector<TF1*> &iU1RMS2Fit,std::vector<TF1*> &iU1Sig3Fit,
		 std::vector<TF1*> &iU2Fit,std::vector<TF1*> &iU2MRMSFit,std::vector<TF1*> &iU2RMS1Fit,
		 std::vector<TF1*> &iU2RMS2Fit,std::vector<TF1*> &iU2Sig3Fit,
		 std::string iFName = "/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfit-new.root"); 
  void metDistribution(double &iMet,double &iMPhi,double iGenPt,double iGenPhi,
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
		       int iFluc=0,int iMetType=0);
  double calculate(int iMet,double iEPt,double iEPhi,double iWPhi,double iU1,double iU2);
  double getError(double iVal,TF1 *iWFit,TF1 *iZDatFit,TF1 *iZMCFit,bool iRescale=true);
  double getError2(double iVal,TF1 *iFit);
  double getCorError2(double iVal,TF1 *iFit);


  TRandom1 *lRandom;
  vector<double> lWSumEt;  vector<double> lWWeights; 
  vector<double> lZDSumEt; vector<double> lZDWeights; 
  vector<double> lZMSumEt; vector<double> lZMWeights; 
  vector<TF1*>   lWU1Fit;  vector<TF1*> lWU1RMSSMFit;  vector<TF1*> lWU1RMS1Fit;  vector<TF1*> lWU1RMS2Fit;  vector<TF1*> lWU13SigFit; 
  vector<TF1*>   lWU2Fit;  vector<TF1*> lWU2RMSSMFit;  vector<TF1*> lWU2RMS1Fit;  vector<TF1*> lWU2RMS2Fit;  vector<TF1*> lWU23SigFit; 
  vector<TF1*>   lZDU1Fit; vector<TF1*> lZDU1RMSSMFit; vector<TF1*> lZDU1RMS1Fit; vector<TF1*> lZDU1RMS2Fit; vector<TF1*> lZDU13SigFit; 
  vector<TF1*>   lZDU2Fit; vector<TF1*> lZDU2RMSSMFit; vector<TF1*> lZDU2RMS1Fit; vector<TF1*> lZDU2RMS2Fit; vector<TF1*> lZDU23SigFit;  
  vector<TF1*>   lZMU1Fit; vector<TF1*> lZMU1RMSSMFit; vector<TF1*> lZMU1RMS1Fit; vector<TF1*> lZMU1RMS2Fit; vector<TF1*> lZMU13SigFit; 
  vector<TF1*>   lZMU2Fit; vector<TF1*> lZMU2RMSSMFit; vector<TF1*> lZMU2RMS1Fit; vector<TF1*> lZMU2RMS2Fit; vector<TF1*> lZMU23SigFit; 

  int lWNBins;
  int lZDNBins;
  int lZMNBins;


};

#endif 

