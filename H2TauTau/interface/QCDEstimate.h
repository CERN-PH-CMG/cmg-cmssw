#ifndef QCDEstimate_H
#define QCDEstimate_H

#include <TTree.h>
#include <TH1F.h>
#include <TString.h>

/*
-This class computes the QCD by using anti-isolated OS events in Data and subtracting the Z->tau tau, W+jets, and t-tbar MC (others are ignored).
-The trees must contain the anti-isolated events corresponding the fully released muon and tau isolation.
-The isolations use here are relIso. with deltaBetaCorr < 0.1 and  hpsPFTauDiscriminationByLooseIsolationMVA 
-No m_T selection must not have been applied to the input trees.
-The input trees must contain the following variables: muon jet pt, tau jet pt, muon isolation variable, tau isolation variable, m_T variable, SM category variable, event weight
-Ratios were computed as a function of the pt of jet corresponding to the muon/tau, the corresponding jet was matched with deltaR < 0.3
-These ratios depend on how the  best candidate selection is applied, a best candidate selection is applied separetely to events with at least one mu-tau passing both isolations and to events which dont have any fully isolated mu-tau candidates
-QCD should be estimated using QCDTypeOption=3 because it has the smallest statistical uncertainty, 1 and 2 are just for cross-check
-Statistical uncertainties not yet calculated here.
*/


class QCDEstimate {
public:
  QCDEstimate(TTree** DataTrees, unsigned int nDataTrees, //array of Data TTrees/TChains
	      TTree* ZTT, float ZTTLumiScale,//Z->Tau Tau TTree and lumi scale factor
	      TTree* WJets, float WJetsLumiScale,//
	      TTree* TTbar, float TTbarLumiScale, //
	      TString categorySelection,// to select the SM category, apply OS charge selection here if necessary, should contain also the eventweight for MC (which should be 1 for data), do not double count the lumi scale factor if your eventweight contains the lumi scale factor set the values above to 1.
	      TString MuIsoSelection,//selects the muon isolation signal region, !(MuIsoSelection) will be used to select the anti-isolated muon sample
	      TString TauIsoSelection,//selects the tau isolation signal region, !(TauIsoSelection) will be used to select the anti-isolated tau sample
	      TString mTSignalSelection,//selects the signal region in mT
	      TString mTSidebandSelection,//selects the sideband region in mT, needed to correct the MC 
	      TString mujetptVarName, TString taujetptVarName  //names for the muon and tau jet pt variables
	      );

  QCDEstimate();//default constructor, do not use
  ~QCDEstimate(){};

  TH1F * GetQCDNominal(const char * HistoName, TString variable, int nbins, float xmin , float xmax, int QCDTypeOption=3); 
  //TH1F * GetQCDForSystematic(const char * HistoName, TString variable, int nbins, float xmin , float xmax, int QCDTypeOption=3); 

private:
  
  TTree** DataTrees_;
  unsigned int nDataTrees_;
  TTree* ZTTTree_;
  TTree* WJetsTree_;
  TTree* TTbarTree_;
  float ZTTLumiScale_;
  float WJetsLumiScale_;
  float TTbarLumiScale_;
  TString categorySelection_;
  TString MuIsoSelection_;
  TString TauIsoSelection_;
  TString mTSignalSelection_;
  TString mTSidebandSelection_;

  TString qcdMuIsoRatio_;
  TString qcdTauIsoRatio_;
} ;


#endif 
