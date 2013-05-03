#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include "Cintex/Cintex.h"
#include "CMGTools/Utilities/interface/MVAMet.h"


//--------------------------------------------------------------------------------------------------
MVAMet::MVAMet(double iDZCut) :
  fPhiMethodName("PhiCorrection"),
  fU1MethodName ("U1Correction"),
  fCovU1MethodName ("CovU1"),
  fCovU2MethodName ("CovU2"),
  fIsInitialized(kFALSE),
  fDZCut  (iDZCut),
  fSumEt  (0),
  fU      (0),
  fUPhi   (0),
  fTKSumEt(0),
  fTKU    (0),
  fTKUPhi (0),
  fNPSumEt(0),
  fNPU    (0),
  fNPUPhi (0),
  fPUSumEt(0),
  fPUMet  (0),
  fPUMetPhi(0),
  fPCSumEt(0),
  fPCU    (0),
  fPCUPhi (0),
  fJSPt1  (0),
  fJSEta1 (0),
  fJSPhi1 (0),
  fJSPt2  (0),
  fJSEta2 (0),
  fJSPhi2 (0),
  fNJet   (0),
  fNAllJet(0),
  fNPV    (0),
  fPhiReader(0),
  fU1Reader (0),
  fCovU1Reader(0),
  fCovU2Reader(0) {    }

//--------------------------------------------------------------------------------------------------
MVAMet::~MVAMet()
{
  delete fPhiReader;
  delete fU1Reader;
  delete fCovU1Reader;
  delete fCovU2Reader;
}
//--------------------------------------------------------------------------------------------------
void MVAMet::Initialize(const edm::ParameterSet &iConfig, 
			TString iU1Weights, 
			TString iPhiWeights, 
			TString iCovU1Weights,
			TString iCovU2Weights,
			MVAMet::MVAType     iType) { 
  
  ROOT::Cintex::Cintex::Enable();
  fIsInitialized = kTRUE;
  fUtils         = new MetUtilities(3); // 3 is the working point id, see MetUtilities.cc
  fType          = iType;
  f42            = iU1Weights.Contains("42");
  if(f42)  {
    fUtils->fJetPtMin = 1.;
    fUtils->workingPointId_=2;
  }
  //std::cout<<"MVAMet::Initialize  fUtils  JetPtMin= "<<fUtils->fJetPtMin<<"   workingPointId_="<<fUtils->workingPointId_<<std::endl;

  TFile *lPhiForest = new TFile(iPhiWeights,"READ");
  fPhiReader = (GBRForest*)lPhiForest->Get(fPhiMethodName);
  lPhiForest->Close();

  TFile *lU1Forest = new TFile(iU1Weights,"READ");
  fU1Reader  = (GBRForest*)lU1Forest->Get(fU1MethodName);
  lU1Forest->Close();

  TFile *lCovU1Forest = new TFile(iCovU1Weights,"READ");
  fCovU1Reader       = (GBRForest*)lCovU1Forest->Get(fCovU1MethodName);
  lCovU1Forest->Close();

  TFile *lCovU2Forest = new TFile(iCovU2Weights,"READ");
  fCovU2Reader       = (GBRForest*)lCovU2Forest->Get(fCovU2MethodName);
  lCovU2Forest->Close();

  fPhiVals = new Float_t[23];
  fU1Vals  = new Float_t[25];
  fCovVals = new Float_t[26];
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::evaluatePhi() { 
  fPhiVals[0]  =  fNPV     ;
  fPhiVals[1]  =  fU       ;
  fPhiVals[2]  =  fUPhi    ;
  fPhiVals[3]  =  fTKSumEt ;
  fPhiVals[4]  =  fTKU     ;
  fPhiVals[5]  =  fTKUPhi  ;
  fPhiVals[6]  =  fNPSumEt ;
  fPhiVals[7]  =  fNPU     ;
  fPhiVals[8]  =  fNPUPhi  ;
  fPhiVals[9]  =  fPUSumEt ;
  fPhiVals[10] =  fPUMet   ;
  fPhiVals[11] =  fPUMetPhi;
  fPhiVals[12] =  fPCSumEt ;
  fPhiVals[13] =  fPCU     ;
  fPhiVals[14] =  fPCUPhi  ;
  fPhiVals[15] =  fJSPt1   ;
  fPhiVals[16] =  fJSEta1  ;
  fPhiVals[17] =  fJSPhi1  ;
  fPhiVals[18] =  fJSPt2   ;
  fPhiVals[19] =  fJSEta2  ;
  fPhiVals[20] =  fJSPhi2  ;
  fPhiVals[21] =  fNAllJet ;
  fPhiVals[22] =  fNJet    ;
  return fPhiReader->GetResponse(fPhiVals);
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::evaluateU1() { 
  fU1Vals[0]  =  fSumEt   ;     //Helps flattens res vs npv
  fU1Vals[1]  =  fNPV     ;
  fU1Vals[2]  =  fU       ;
  fU1Vals[3]  =  fUPhi    ;
  fU1Vals[4]  =  fTKSumEt ;
  fU1Vals[5]  =  fTKU     ;
  fU1Vals[6]  =  fTKUPhi  ;
  fU1Vals[7]  =  fNPSumEt ;
  fU1Vals[8]  =  fNPU     ;
  fU1Vals[9]  =  fNPUPhi  ;
  fU1Vals[10]  =  fPUSumEt ;
  fU1Vals[11] =  fPUMet   ;
  fU1Vals[12] =  fPUMetPhi;
  fU1Vals[13] =  fPCSumEt ;
  fU1Vals[14] =  fPCU     ;
  fU1Vals[15] =  fPCUPhi  ;
  fU1Vals[16] =  fJSPt1   ;
  fU1Vals[17] =  fJSEta1  ;
  fU1Vals[18] =  fJSPhi1  ;
  fU1Vals[19] =  fJSPt2   ;
  fU1Vals[20] =  fJSEta2  ;
  fU1Vals[21] =  fJSPhi2  ;
  fU1Vals[22] =  fNAllJet ;
  fU1Vals[23] =  fNJet    ;
  fU1Vals[24] =  fUPhiMVA ;
  return fU1Reader->GetResponse(fU1Vals);
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::evaluateCovU1() { 
  fCovVals[0]  =  fSumEt   ;     //Helps flattens res vs npv
  fCovVals[1]  =  fNPV     ;
  fCovVals[2]  =  fU       ;
  fCovVals[3]  =  fUPhi    ;
  fCovVals[4]  =  fTKSumEt ;
  fCovVals[5]  =  fTKU     ;
  fCovVals[6]  =  fTKUPhi  ;
  fCovVals[7]  =  fNPSumEt ;
  fCovVals[8]  =  fNPU     ;
  fCovVals[9]  =  fNPUPhi  ;
  fCovVals[10]  =  fPUSumEt ;
  fCovVals[11] =  fPUMet   ;
  fCovVals[12] =  fPUMetPhi;
  fCovVals[13] =  fPCSumEt ;
  fCovVals[14] =  fPCU     ;
  fCovVals[15] =  fPCUPhi  ;
  fCovVals[16] =  fJSPt1   ;
  fCovVals[17] =  fJSEta1  ;
  fCovVals[18] =  fJSPhi1  ;
  fCovVals[19] =  fJSPt2   ;
  fCovVals[20] =  fJSEta2  ;
  fCovVals[21] =  fJSPhi2  ;
  fCovVals[22] =  fNAllJet ;
  fCovVals[23] =  fNJet    ;
  fCovVals[24] =  fUPhiMVA ;
  fCovVals[25] =  fUMVA    ;
  //COLIN old 52 recipe:
  // return fCovU1Reader->GetResponse(fCovVals);
  //COLIN 53 
  return fCovU1Reader->GetResponse(fCovVals) * fCovU1Reader->GetResponse(fCovVals) * fUMVA * fUMVA;
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::evaluateCovU2() { 
  fCovVals[0]  =  fSumEt   ;     
  fCovVals[1]  =  fNPV     ;
  fCovVals[2]  =  fU       ;
  fCovVals[3]  =  fUPhi    ;
  fCovVals[4]  =  fTKSumEt ;
  fCovVals[5]  =  fTKU     ;
  fCovVals[6]  =  fTKUPhi  ;
  fCovVals[7]  =  fNPSumEt ;
  fCovVals[8]  =  fNPU     ;
  fCovVals[9]  =  fNPUPhi  ;
  fCovVals[10]  =  fPUSumEt ;
  fCovVals[11] =  fPUMet   ;
  fCovVals[12] =  fPUMetPhi;
  fCovVals[13] =  fPCSumEt ;
  fCovVals[14] =  fPCU     ;
  fCovVals[15] =  fPCUPhi  ;
  fCovVals[16] =  fJSPt1   ;
  fCovVals[17] =  fJSEta1  ;
  fCovVals[18] =  fJSPhi1  ;
  fCovVals[19] =  fJSPt2   ;
  fCovVals[20] =  fJSEta2  ;
  fCovVals[21] =  fJSPhi2  ;
  fCovVals[22] =  fNAllJet ;
  fCovVals[23] =  fNJet    ;
  fCovVals[24] =  fUPhiMVA ;
  fCovVals[25] =  fUMVA    ;
  //COLIN old 52 recipe
  // return fCovU2Reader->GetResponse(fCovVals);
  //COLIN 53 
  return fCovU2Reader->GetResponse(fCovVals) * fCovU2Reader->GetResponse(fCovVals) * fUMVA * fUMVA;

}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::MVAValue(  Bool_t iPhi, 
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
			    Float_t iNAllJet,
			    Float_t iNJet   ,
			    Float_t iNPV    
			    ){
  
  if (!fIsInitialized) { 
    std::cout << "Error: MVA Met not properly initialized.\n"; 
    return -9999;
  }
    
  fU        = iU      ;
  fUPhi     = iUPhi   ;
  fTKSumEt  = iTKSumEt/iPFSumEt;
  fTKU      = iTKU    ;
  fTKUPhi   = iTKUPhi ;
  fNPSumEt  = iNPSumEt/iPFSumEt;
  fNPU      = iNPU    ;
  fNPUPhi   = iNPUPhi ;
  fPUSumEt  = iPUSumEt/iPFSumEt;
  fPUMet    = iPUMet  ;
  fPUMetPhi = iPUMetPhi;
  fPCSumEt  = iPCSumEt/iPFSumEt;
  fPCU      = iPCU    ;
  fPCUPhi   = iPCUPhi ;
  fJSPt1    = iJSPt1  ;
  fJSEta1   = iJSEta1 ;
  fJSPhi1   = iJSPhi1 ;
  fJSPt2    = iJSPt2  ;
  fJSEta2   = iJSEta2 ;
  fJSPhi2   = iJSPhi2 ;
  fNAllJet  = iNAllJet;
  fNJet     = iNJet   ;
  fNPV      = iNPV    ;

  Double_t lMVA = -9999;  
  lMVA = evaluatePhi();
  if(!iPhi) fUPhiMVA = iUPhi+lMVA;
  //Not no nice feature of the training
  //fTKSumEt  /= iPFSumEt;
  //fNPSumEt  /= iPFSumEt;
  //fPUSumEt  /= iPFSumEt;
  //fPCSumEt  /= iPFSumEt; 
  if(!iPhi) lMVA  = evaluateU1();
  return lMVA;
}
std::pair<MVAMet::LorentzVector,TMatrixD> MVAMet::GetMet(std::vector<LorentzVector>                                       &iVis,
							 std::vector<MetUtilities::JetInfo>                               &iJets,
							 std::vector<std::pair<LorentzVector,double> >                    &iCands,
							 std::vector<Vector>                                              &iVertices,
							 bool iPrintDebug) { 
  
  LorentzVector lVis(0,0,0,0); double lVisSumEt = 0;
  for(int i0 = 0; i0 < int(iVis.size()); i0++) lVis      += iVis[i0];
  for(int i0 = 0; i0 < int(iVis.size()); i0++) lVisSumEt += iVis[i0].Pt();
 
  Float_t lPtVis    = lVis.Pt();
  Float_t lPhiVis   = lVis.Phi();
  fUtils->cleanJets(iVis,iJets);
  std::pair<LorentzVector,double> lPFRec  = fUtils->PFRecoil  (lVisSumEt,lVis,iCands,      fDZCut);
  std::pair<LorentzVector,double> lTKRec  = fUtils->TKRecoil  (lVisSumEt,lVis,iCands,      fDZCut);
  std::pair<LorentzVector,double> lNPRec  = fUtils->NoPURecoil(lVisSumEt,lVis,iCands,iJets,fDZCut);
  std::pair<LorentzVector,double> lPCRec  = fUtils->PUCRecoil (lVisSumEt,lVis,iCands,iJets,fDZCut);
  std::pair<LorentzVector,double> lPUMet  = fUtils->PUMet     (               iCands,iJets,0.2); //==> the others used 0.1
  
 
  int lNJets           = fUtils->NJets(iJets,30);
  int lNAllJet         = int(iJets.size());
  if(f42)     lNAllJet = fUtils->NJets(iJets,1.);
  LorentzVector *lLead = fUtils->leadPt(iJets,true); 
  LorentzVector *l2nd  = fUtils->leadPt(iJets,false); 

  fSumEt   = lPFRec.second;
  fU       = lPFRec.first.rho();
  fUPhi    = lPFRec.first.phi();
  fTKSumEt = lTKRec.second/lPFRec.second;
  fTKU     = lTKRec.first.rho();
  fTKUPhi  = lTKRec.first.phi();
  fNPSumEt = lNPRec.second/lPFRec.second;
  fNPU     = lNPRec.first.rho();
  fNPUPhi  = lNPRec.first.phi();
  fPUSumEt = lPUMet.second/lPFRec.second;
  fPUMet   = lPUMet.first.rho();
  fPUMetPhi= lPUMet.first.phi();
  fPCSumEt = lPCRec.second/lPFRec.second;
  fPCU     = lPCRec.first.rho() ;
  fPCUPhi  = lPCRec.first.phi();
  fJSPt1   = 0; if(lLead != 0) fJSPt1  = lLead->pt();
  fJSEta1  = 0; if(lLead != 0) fJSEta1 = lLead->eta();
  fJSPhi1  = 0; if(lLead != 0) fJSPhi1 = lLead->phi();
  fJSPt2   = 0; if(l2nd  != 0) fJSPt2  = l2nd ->pt();
  fJSEta2  = 0; if(l2nd  != 0) fJSEta2 = l2nd ->eta();
  fJSPhi2  = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->phi();
  fNJet    = lNJets  ;
  fNAllJet = lNAllJet;
  fNPV     = iVertices.size();
 
  Float_t lMVA = evaluatePhi();
  
  fUPhiMVA  = fUPhi + lMVA; 
  //fTKSumEt  /= lPFRec.second;
  //fNPSumEt  /= lPFRec.second;
  //fPUSumEt  /= lPFRec.second;
  //fPCSumEt  /= lPFRec.second;
  lMVA     = evaluateU1();
  fUMVA    = fU*lMVA;

  TLorentzVector lUVec (0,0,0,0);   lUVec .SetPtEtaPhiM(fUMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec (0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis ,0,lPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;

  LorentzVector  lMetVec (0,0,0,0);   lMetVec.SetCoordinates(lUVec.Px(),lUVec.Py(),lUVec.Pz(),lUVec.E());
  TMatrixD     lCov(2,2);
  //Covariance matrix perpendicular and parallel to the recoil (sort of correct)
  double lCovU1 = evaluateCovU1();
  double lCovU2 = evaluateCovU2();

  double lCos2 = cos(fUPhiMVA)*cos(fUPhiMVA);
  double lSin2 = sin(fUPhiMVA)*sin(fUPhiMVA);

  //Now Compute teh covariance matrix in X and Y 
  lCov(0,0)   =  lCovU1*lCos2+lCovU2*lSin2;
  lCov(1,0)   = -lCovU1*sin(fUPhiMVA)*cos(fUPhiMVA)+lCovU2*sin(fUPhiMVA)*cos(fUPhiMVA);
  lCov(0,1)   = lCov(1,0);
  lCov(1,1)   = lCovU1*lSin2+lCovU2*lCos2;

  if (iPrintDebug == kTRUE) {
    std::cout << "Debug MetMVA: "<<std::endl
	      <<" PF "<<  fSumEt    <<" "<<  fU        <<" "<<  fUPhi     <<std::endl
	      <<" TK "<<  fTKSumEt  <<" "<<  fTKU      <<" "<<  fTKUPhi   <<std::endl
	      <<" NPU "<<  fNPSumEt  <<" "<<  fNPU      <<" "<<  fNPUPhi   <<std::endl
	      <<" PU "<<  fPUSumEt  <<" "<<  fPUMet    <<" "<<  fPUMetPhi <<std::endl
	      <<" PUC "<<  fPCSumEt  <<" "<<  fPCU      <<" "<<  fPCUPhi   <<std::endl
	      <<" jet1 "<<  fJSPt1    <<" "<<  fJSEta1   <<" "<<  fJSPhi1   <<std::endl
	      <<" jet2 "<<  fJSPt2    <<" "<<  fJSEta2   <<" "<<  fJSPhi2   <<std::endl
	      <<" njets "<<  fNAllJet  <<std::endl
	      <<" njet30 "<<  fNJet     <<std::endl
	      <<" npv "<<  fNPV      <<std::endl
              << " === : === "
              << std::endl;
  }
  std::pair<LorentzVector,TMatrixD> lMet(lMetVec,lCov);
  return lMet;
}
//Jet Cleaning inside
std::pair<MVAMet::LorentzVector,TMatrixD> MVAMet::GetMet(std::vector<LorentzVector> &iVis,
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
							 bool iPrintDebug) { 

  LorentzVector lVis(0,0,0,0); double lSumEtVis = 0;
  for(int i0 = 0; i0 < int(iVis.size()); i0++) lVis      += iVis[i0];
  for(int i0 = 0; i0 < int(iVis.size()); i0++) lSumEtVis += iVis[i0].Pt();
 
  Float_t lPtVis    = lVis.Pt();
  Float_t lPhiVis   = lVis.Phi();

  std::pair<LorentzVector,double> lPFMet;    lPFMet.first   = iPFMet  ->p4(); lPFMet  .second = iPFMet  ->sumEt();
  std::pair<LorentzVector,double> lTKMet;    lTKMet.first   = iTKMet  ->p4(); lTKMet  .second = iTKMet  ->sumEt();
  std::pair<LorentzVector,double> lNoPUMet;  lNoPUMet.first = iNoPUMet->p4(); lNoPUMet.second = iNoPUMet->sumEt();
  std::pair<LorentzVector,double> lPUCMet;   lPUCMet.first  = iPUCMet ->p4(); lPUCMet .second = iPUCMet ->sumEt();
  std::pair<LorentzVector,double> lPUMet;    lPUMet.first   = iPUMet  ->p4(); lPUMet  .second = iPUMet  ->sumEt();
  
  //Here is the cleaning step I just added
  fUtils->cleanMet(iVis,iJets,lNoPUMet,true ,true);
  fUtils->cleanMet(iVis,iJets,lPUCMet ,false,false);
  fUtils->cleanMet(iVis,iJets,lPUMet  ,false,true);

//   std::pair<LorentzVector,double> lPFRec  = fUtils->Recoil(lSumEtVis,lPtVis,lPhiVis,lPFMet);
//   std::pair<LorentzVector,double> lTKRec  = fUtils->Recoil(lSumEtVis,lPtVis,lPhiVis,lTKMet);
//   std::pair<LorentzVector,double> lNPRec  = fUtils->Recoil(lSumEtVis,lPtVis,lPhiVis,lNoPUMet);
//   std::pair<LorentzVector,double> lPCRec  = fUtils->Recoil(lSumEtVis,lPtVis,lPhiVis,lPUCMet);
  std::pair<LorentzVector,double> lPFRec  = lPFMet;
  std::pair<LorentzVector,double> lTKRec  = lTKMet;
  std::pair<LorentzVector,double> lNPRec  = lNoPUMet;
  std::pair<LorentzVector,double> lPCRec  = lPUCMet;
  
 
  fSumEt   = lPFRec.second;
  fU       = lPFRec.first.rho();
  fUPhi    = lPFRec.first.phi();
  fTKSumEt = lTKRec.second/lPFRec.second;
  fTKU     = lTKRec.first.rho();
  fTKUPhi  = lTKRec.first.phi();
  fNPSumEt = lNPRec.second/lPFRec.second;
  fNPU     = lNPRec.first.rho();
  fNPUPhi  = lNPRec.first.phi();
  fPUSumEt = lPUMet.second/lPFRec.second;
  fPUMet   = lPUMet.first.rho();
  fPUMetPhi= lPUMet.first.phi();
  fPCSumEt = lPCRec.second/lPFRec.second;
  fPCU     = lPCRec.first.rho() ;
  fPCUPhi  = lPCRec.first.phi();
  fJSPt1   = 0; if(iLeadJet != 0) fJSPt1  = iLeadJet->pt();
  fJSEta1  = 0; if(iLeadJet != 0) fJSEta1 = iLeadJet->eta();
  fJSPhi1  = 0; if(iLeadJet != 0) fJSPhi1 = iLeadJet->phi();
  fJSPt2   = 0; if(i2ndJet  != 0) fJSPt2  = i2ndJet ->pt();
  fJSEta2  = 0; if(i2ndJet  != 0) fJSEta2 = i2ndJet ->eta();
  fJSPhi2  = 0; if(i2ndJet  != 0) fJSPhi2 = i2ndJet ->phi();
  fNJet    = iNJetsGt30;
  fNAllJet = iNJetsGt1;
  fNPV     = iNGoodVtx;
 
  Float_t lMVA = evaluatePhi();
  
  fUPhiMVA  = fUPhi + lMVA; 
  lMVA     = evaluateU1();
  fUMVA    = fU*lMVA;

  TLorentzVector lUVec (0,0,0,0);   lUVec .SetPtEtaPhiM(fUMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec (0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis ,0,lPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  TLorentzVector lFinal  (0,0,0,0); lFinal.SetPtEtaPhiM(lUVec.Pt(),0.,lUVec.Phi(),0);
  LorentzVector  lMetVec (0,0,0,0);   lMetVec.SetCoordinates(lFinal.Px(),lFinal.Py(),lFinal.Pz(),lFinal.E());
  TMatrixD     lCov(2,2);

  //Covariance matrix perpendicular and parallel to the recoil (sort of correct)
  double lCovU1 = evaluateCovU1();
  double lCovU2 = evaluateCovU2();

  double lCos2 = cos(fUPhiMVA)*cos(fUPhiMVA);
  double lSin2 = sin(fUPhiMVA)*sin(fUPhiMVA);

  //Now Compute teh covariance matrix in X and Y 
  lCov(0,0)   =  lCovU1*lCos2+lCovU2*lSin2;
  lCov(1,0)   = -lCovU1*sin(fUPhiMVA)*cos(fUPhiMVA)+lCovU2*sin(fUPhiMVA)*cos(fUPhiMVA);
  lCov(0,1)   = lCov(1,0);
  lCov(1,1)   = lCovU1*lSin2+lCovU2*lCos2;

  if (iPrintDebug == kTRUE) {
    std::cout << "Debug MetMVA: "<<std::endl
	      <<" PF "<<  fSumEt    <<" "<<  fU        <<" "<<  fUPhi     <<std::endl
	      <<" TK "<<  fTKSumEt  <<" "<<  fTKU      <<" "<<  fTKUPhi   <<std::endl
	      <<" NPU "<<  fNPSumEt  <<" "<<  fNPU      <<" "<<  fNPUPhi   <<std::endl
	      <<" PU "<<  fPUSumEt  <<" "<<  fPUMet    <<" "<<  fPUMetPhi <<std::endl
	      <<" PUC "<<  fPCSumEt  <<" "<<  fPCU      <<" "<<  fPCUPhi   <<std::endl
	      <<" jet1 "<<  fJSPt1    <<" "<<  fJSEta1   <<" "<<  fJSPhi1   <<std::endl
	      <<" jet2 "<<  fJSPt2    <<" "<<  fJSEta2   <<" "<<  fJSPhi2   <<std::endl
	      <<" njets "<<  fNAllJet  <<std::endl
	      <<" njet30 "<<  fNJet     <<std::endl
	      <<" npv "<<  fNPV      <<std::endl
              << " === : === "
              << std::endl;
  }
  std::pair<LorentzVector,TMatrixD> lMet(lMetVec,lCov);
  return lMet;
}
