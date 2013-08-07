//--------------------------------------------------------------------------------------------------
// $Id $
//
// Met Regression
//
// Authors: P. Harris
//--------------------------------------------------------------------------------------------------

#ifndef PH_MVAMet_H
#define PH_MVAMet_H

#include <utility>
#include <vector>
#include <TString.h>
#include <TMatrixD.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "CMGTools/Utilities/interface/GBRForest.h"
#include "CMGTools/Common/interface/MetUtilities.h"

class MVAMet {
 public:
  typedef math::XYZVector         Vector;
  typedef math::XYZTLorentzVector LorentzVector;

  MVAMet(double iDZCut=0.1);
  ~MVAMet();
  
  enum MVAType {
    kBaseline = 0
  };
  
  void    Initialize(const edm::ParameterSet &iConfig, 
		     TString iU1Weights      ="$CMSSW_BASE/src/pharris/data/gbrmet_52.root",
		     TString iPhiWeights     ="$CMSSW_BASE/src/pharris/data/gbrmetphi_52.root",
		     TString iCovU1Weights   ="$CMSSW_BASE/src/pharris/data/gbrcovu1_52.root",
		     TString iCovU2Weights   ="$CMSSW_BASE/src/pharris/data/gbrcovu2_52.root",
		     MVAMet::MVAType  iType=kBaseline);
  
  Bool_t   IsInitialized() const { return fIsInitialized; }
  Double_t evaluatePhi();
  Double_t evaluateU1();
  Double_t evaluateCovU1();
  Double_t evaluateCovU2();

  Double_t MVAValue(  bool iPhi,
		      Float_t iPFSumEt, 
		      Float_t iU      ,
		      Float_t iUPhi   ,
		      Float_t iTKSumEt,
		      Float_t iTKU    ,
		      Float_t iTKUPhi ,
		      Float_t iNPSumEt,
		      Float_t iNPU    ,
		      Float_t iNPUPhi ,
		      Float_t iPUSumEt,
		      Float_t iPUMet  ,
		      Float_t iPUMetPhi,
		      Float_t iPCSumEt,
		      Float_t iPCU    ,
		      Float_t iPCUPhi ,
		      Float_t iJSPt1  ,
		      Float_t iJSEta1 ,
		      Float_t iJSPhi1 ,
		      Float_t iJSPt2  ,
		      Float_t iJSEta2 ,
		      Float_t iJSPhi2 ,
		      Float_t iNJet   ,
		      Float_t iNAllJet,
		      Float_t iNPV    );

  std::pair<LorentzVector,TMatrixD>  GetMet(std::vector<LorentzVector>                                       &iVis,
					    std::vector<MetUtilities::JetInfo>                               &iJets,
					    std::vector<std::pair<LorentzVector,double> >                    &iCands,
					    std::vector<Vector>                                              &iVertices,
					    bool iPrintDebug=false);
  
  MetUtilities *fUtils;
  

  std::pair<MVAMet::LorentzVector,TMatrixD> GetMet(std::vector<LorentzVector> &iVis,
						   const reco::PFMET *iPFMet,
						   const reco::PFMET *iTKMet,
						   const reco::PFMET *iNoPUMet,
						   const reco::PFMET *iPUMet,
						   const reco::PFMET *iPUCMet,
						   const LorentzVector *iLeadJet,
						   const LorentzVector *i2ndJet,
						   int iNJetsGt30,
						   int iNJetsGt1,
						   int iNGoodVtx,
						   std::vector<MetUtilities::JetInfo>                               &iJets,
						   bool iPrintDebug);

  
  protected:
    TString      fPhiMethodName;
    TString      fU1MethodName;
    TString      fCovU1MethodName;
    TString      fCovU2MethodName;
    Bool_t       fIsInitialized;
    Bool_t       f42;
    MVAType      fType;
    double  fDZCut  ;
    Float_t fSumEt  ;
    Float_t fU      ;
    Float_t fUPhi   ;
    Float_t fTKSumEt;
    Float_t fTKU    ;
    Float_t fTKUPhi ;
    Float_t fNPSumEt;
    Float_t fNPU    ;
    Float_t fNPUPhi ;
    Float_t fPUSumEt;
    Float_t fPUMet  ;
    Float_t fPUMetPhi ;
    Float_t fPCSumEt;
    Float_t fPCU    ;
    Float_t fPCUPhi ;
    Float_t fJSPt1  ;
    Float_t fJSEta1 ;
    Float_t fJSPhi1 ;
    Float_t fJSPt2  ;
    Float_t fJSEta2 ;
    Float_t fJSPhi2 ;
    Float_t fNJet   ;
    Float_t fNAllJet;
    Float_t fNPV    ;
    Float_t fUPhiMVA;
    Float_t fUMVA   ;
    
    Float_t* fPhiVals;
    Float_t* fU1Vals;
    Float_t* fCovVals;
    
    
    GBRForest *fPhiReader;
    GBRForest *fU1Reader;
    GBRForest *fCovU1Reader;
    GBRForest *fCovU2Reader;
};
#endif
