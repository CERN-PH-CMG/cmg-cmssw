#include "TH1F.h"
#include "TTree.h"

std::string fMVAVar = "JetID";
double getWP(double iEff,TTree *iTree,std::string iCut="") { 
  double lVal = -10;
  TH1F *lHSignal = new TH1F("wpSig","wpSig",2000,-1,1);
  //TH1F *lHBack   = new TH1F("wpBac","wpBac",2000,-1,1);
  iTree->Draw((fMVAVar+">>+wpSig").c_str(),("classID == 0"+iCut).c_str());
  iTree->Draw((fMVAVar+">>+wpSig").c_str(),("classID == 0"+iCut).c_str());
  //iTree->Draw(fMVAVar+">>+wpBac","classID == 1");
  double lSTot = lHSignal->Integral();
  //double lBTot = lHBack  ->Integral();
  for(int i0 = 0; i0 < lHSignal->GetNbinsX()+1; i0++) { 
    double pSig   = lHSignal->Integral(0,i0);
    double pSEff  = 1.-pSig/lSTot;
    //double pBack  = lHBack   ->Integral(0,i0);
    //double pBEff  = pBack/lBTot;
    if(pSEff > iEff) lVal = lHSignal->GetBinCenter(i0-1);
  }
  return lVal;
}
