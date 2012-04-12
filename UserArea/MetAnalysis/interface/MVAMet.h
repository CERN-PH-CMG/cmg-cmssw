//--------------------------------------------------------------------------------------------------
// $Id $
//
// Met Regression
//
// Authors: P. Harris
//--------------------------------------------------------------------------------------------------

#ifndef MVAMet_H
#define MVAMet_H

#include <TString.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

#include "CMG/MetAnalysis/interface/GBRForest.h"
#include "CMG/MetAnalysis/interface/MetUtilities.h"

using namespace reco;

class MVAMet {
  public:
    MVAMet();
    ~MVAMet();

    enum MVAType {
      kBaseline = 0
    };

    void    Initialize(const edm::ParameterSet &iConfig, 
		       TString iU1Weights    ="$CMSSW_BASE/src/MitPhysics/data/gbrmet.root",
		       TString iPhiWeights   ="$CMSSW_BASE/src/MitPhysics/data/gbrmetphi.root",
		       MVAMet::MVAType  iType=kBaseline);
        
    Bool_t   IsInitialized() const { return fIsInitialized; }
    Double_t evaluatePhi();
    Double_t evaluateU1();
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
    
    PFMET GetMet(double iPtVis,double iPhiVis,double iSumEtVis,
		 const PFMET             *iPFMet,
		 const PFMET             *iTKMet,
		 const PFMET             *iNPMet,
		 const PFMET             *iPUMet,
		 const PFMET             *iPCMet,
		 const PFJetCollection   *iJets ,
		 int                      iNPV  ,
		 bool                     iPhi  ,
		 bool                     iPrintDebug=false);
    
    //Interms of the two candidates ===> previous conditions do not apply
    PFMET GetMet(double iPt1,double iPhi1,double iEta1,
		 double iPt2,double iPhi2,double iEta2,
		 const PFMET             *iPFMet  ,
		 const PFMET             *iTKMet,
		 const PFMET             *iNPMet,
		 const PFMET             *iPUMet,
		 const PFMET             *iPCMet,
		 const PFJetCollection   *iJets ,
		 int                      iNPV  ,
		 bool                     iPhi      =false,
		 bool                     printDebug=false);
 
    
    MetUtilities *fUtils;
    
  protected:
    TString      fPhiMethodName;
    TString      fU1MethodName;
    Bool_t       fIsInitialized;
    MVAType      fType;
    
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
    
    Float_t* fPhiVals;
    Float_t* fU1Vals;
    
    
    GBRForest *fPhiReader;
    GBRForest *fU1Reader;
};
#endif
