#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include "CMG/MetAnalysis/interface/MVAMet.h"

using namespace reco;

//--------------------------------------------------------------------------------------------------
MVAMet::MVAMet(double iDZCut) :
  fPhiMethodName("PhiCorrection"),
  fU1MethodName ("U1Correction"),
  fIsInitialized(kFALSE),
  fDZCut  (iDZCut),
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
  fU1Reader (0)   {    }

//--------------------------------------------------------------------------------------------------
MVAMet::~MVAMet()
{
  fPhiReader = 0;  
  fU1Reader  = 0;
}
//--------------------------------------------------------------------------------------------------
void MVAMet::Initialize(const edm::ParameterSet &iConfig, 
			TString iU1Weights, 
			TString iPhiWeights, 
			MVAMet::MVAType     iType) { 
  
  fIsInitialized = kTRUE;
  fUtils         = new MetUtilities(iConfig);  
  fType          = iType;

  TFile *lPhiForest = new TFile(iPhiWeights,"READ");
  fPhiReader = (GBRForest*)lPhiForest->Get(fPhiMethodName);
  lPhiForest->Close();

  TFile *lU1Forest = new TFile(iU1Weights,"READ");
  fU1Reader  = (GBRForest*)lU1Forest->Get(fU1MethodName);
  lU1Forest->Close();

  fPhiVals = new Float_t[23];
  fU1Vals  = new Float_t[24];
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
  fU1Vals[0]  =  fNPV     ;
  fU1Vals[1]  =  fU       ;
  fU1Vals[2]  =  fUPhi    ;
  fU1Vals[3]  =  fTKSumEt ;
  fU1Vals[4]  =  fTKU     ;
  fU1Vals[5]  =  fTKUPhi  ;
  fU1Vals[6]  =  fNPSumEt ;
  fU1Vals[7]  =  fNPU     ;
  fU1Vals[8]  =  fNPUPhi  ;
  fU1Vals[9]  =  fPUSumEt ;
  fU1Vals[10] =  fPUMet   ;
  fU1Vals[11] =  fPUMetPhi;
  fU1Vals[12] =  fPCSumEt ;
  fU1Vals[13] =  fPCU     ;
  fU1Vals[14] =  fPCUPhi  ;
  fU1Vals[15] =  fJSPt1   ;
  fU1Vals[16] =  fJSEta1  ;
  fU1Vals[17] =  fJSPhi1  ;
  fU1Vals[18] =  fJSPt2   ;
  fU1Vals[19] =  fJSEta2  ;
  fU1Vals[20] =  fJSPhi2  ;
  fU1Vals[21] =  fNAllJet ;
  fU1Vals[22] =  fNJet    ;
  fU1Vals[23] =  fUPhiMVA ;
  return fU1Reader->GetResponse(fU1Vals);
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
  fTKSumEt  /= iPFSumEt;
  fNPSumEt  /= iPFSumEt;
  fPUSumEt  /= iPFSumEt;
  fPCSumEt  /= iPFSumEt; 
  if(!iPhi) lMVA  = evaluateU1();
  return lMVA;
}
//--------------------------------------------------------------------------------------------------
//====> Please not that the jet collection must be cleaned => all jets near leptons must be removed
//====> Please also note that the Met Collections must be cleaned of jets too
//====> Please also note that to be correct sumEt = sumEt - recoiling objets
PFMET   MVAMet::GetMet(	double iPtVis,double iPhiVis,double iSumEtVis,
			const PFMET             *iPFMet  ,
			const PFMET             *iTKMet,
			const PFMET             *iNPMet,
			const PFMET             *iPUMet,
			const PFMET             *iPCMet,
			const PFJetCollection   *iJets ,
			int                      iNPV  ,
			bool                     iPhi  ,	//Add JetCorrector here
			bool                     iPrintDebug) {
  
  PFMET lPFRec = fUtils->recoil   (iPFMet,iPtVis,iPhiVis,iSumEtVis);
  PFMET lTKRec = fUtils->recoil   (iTKMet,iPtVis,iPhiVis,iSumEtVis);
  PFMET lNPRec = fUtils->recoil   (iNPMet,iPtVis,iPhiVis,iSumEtVis);
  PFMET lPCRec = fUtils->recoil   (iPCMet,iPtVis,iPhiVis,iSumEtVis);
  
  //Count the Jets and get the 2 leading candidates
  Double_t lPt0 = 0; const PFJet *lLead = 0; 
  Double_t lPt1 = 0; const PFJet *l2nd  = 0; 
  int lNAllJet  = 0;
  int lNJet     = 0;
  
  for(int i0 = 0; i0 < (int) iJets->size(); i0++) {
    const PFJet     *pJet   = &(iJets->at(i0));
    if(!fUtils->passPFLooseId(pJet)) continue;
    lNAllJet++;
    double pPt = pJet->pt();
    if(pPt  > 30)  lNJet++;
    if(lPt0 < pPt) {lPt0 = pPt; lLead = pJet; continue;}    
    if(lPt1 < pPt) {lPt1 = pPt; l2nd  = pJet; continue;}
  }
  
  fU        = lPFRec .pt();
  fUPhi     = lPFRec .phi();
  fTKSumEt  = lTKRec .sumEt()/lPFRec.sumEt();
  fTKU      = lTKRec .pt();
  fTKUPhi   = lTKRec .phi();
  fNPSumEt  = lNPRec .sumEt()/lPFRec.sumEt();
  fNPU      = lNPRec .pt();
  fNPUPhi   = lNPRec .phi();
  fPUSumEt  = iPUMet->sumEt()/lPFRec.sumEt();
  fPUMet    = iPUMet->pt();
  fPUMetPhi = iPUMet->phi();
  fPCSumEt  = lPCRec .sumEt()/lPFRec.sumEt();
  fPCU      = lPCRec .pt()    ;
  fPCUPhi   = lPCRec .phi()   ;
  fJSPt1    = lPt0; 
  fJSEta1   = 0; if(lLead != 0) fJSEta1 = lLead->eta();
  fJSPhi1   = 0; if(lLead != 0) fJSPhi1 = lLead->phi();
  fJSPt2    = lPt1; 
  fJSEta2   = 0; if(l2nd  != 0) fJSEta2 = l2nd ->eta();
  fJSPhi2   = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->phi();
  fNJet     = lNJet   ;
  fNAllJet  = lNAllJet;
  fNPV      = iNPV    ;

  Float_t lMVA = evaluatePhi();
  if(!iPhi) fUPhiMVA = fUPhi + lMVA; 
  
  //Not no nice feature of the training
  fTKSumEt  /= lPFRec.sumEt();
  fNPSumEt  /= lPFRec.sumEt();
  fPUSumEt  /= lPFRec.sumEt();
  fPCSumEt  /= lPFRec.sumEt();
  if(!iPhi) lMVA     = evaluateU1();
  
  //Recompute the MET
  TLorentzVector lUVec(0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec(0,0,0,0);   lVVec .SetPtEtaPhiM(iPtVis ,0,iPhiVis ,0);
  if(lMVA < 0)                     lUVec .RotateZ(TMath::Pi());                                                   
  lUVec         -= lVVec;
  double lSumEt  = 0; //===> This is where we add the significance regression

  if (iPrintDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCUPhi   << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU     << " : "
	      <<  fPCUPhi  << " : "
	      <<  fJSPt1   << " : "
	      <<  fJSEta1  << " : "
	      <<  fJSPhi1  << " : "
	      <<  fJSPt2   << " : "
	      <<  fJSEta2  << " : "
	      <<  fJSPhi2  << " : "
	      <<  fNJet    << " : "
	      <<  fNAllJet << " : "
	      <<  fNPV     << " : "
              << " === : === "
              << std::endl;
  }
  Candidate::LorentzVector lMetVec(lUVec.Px(),lUVec.Py(),lUVec.Pz(),lUVec.E());
  return PFMET(iPFMet->getSpecific(),lSumEt,lMetVec,iPFMet->vertex());
}
//--------------------------------------------------------------------------------------------------
//Interms of the two candidates ===> previous conditions do not apply
PFMET MVAMet::GetMet(	double iPt1,double iPhi1,double iEta1,
			double iPt2,double iPhi2,double iEta2,
			const PFMET             *iPFMet  ,
			const PFMET             *iTKMet,
			const PFMET             *iNPMet,
			const PFMET             *iPUMet,
			const PFMET             *iPCMet,
			const PFJetCollection   *iJets ,
			int                      iNPV  ,
			//FactorizedJetCorrector *iJetCorrector,
			//const PileupEnergyDensityCol *iPUEnergyDensity,
			bool                     iPhi,
			bool                     printDebug) {
  
  TLorentzVector lVVec1(0,0,0,0);   lVVec1.SetPtEtaPhiM(iPt1,0,iPhi1 ,0);
  TLorentzVector lVVec2(0,0,0,0);   lVVec2.SetPtEtaPhiM(iPt2,0,iPhi2 ,0);
  lVVec1+=lVVec2;
  Float_t lPtVis    = lVVec1.Pt();
  Float_t lPhiVis   = lVVec1.Phi();

  PFMET lPFRec = fUtils->recoil   (iPFMet,iPt1,iPhi1,iEta1,iPt2,iPhi2,iEta2);
  PFMET lTKRec = fUtils->recoil   (iTKMet,iPt1,iPhi1,iEta1,iPt2,iPhi2,iEta2);
  PFMET lNPRec = fUtils->recoil   (iNPMet,iPt1,iPhi1,iEta1,iPt2,iPhi2,iEta2,iJets);
  PFMET lPCRec = fUtils->recoil   (iPCMet,iPt1,iPhi1,iEta1,iPt2,iPhi2,iEta2,iJets);
  PFMET lPUMet = fUtils->clean    (iPUMet,     iPhi1,iEta1,     iPhi2,iEta2,iJets);

  Double_t lPt0 = 0; const PFJet *lLead = 0; 
  Double_t lPt1 = 0; const PFJet *l2nd  = 0; 
  int lNAllJet  = 0;
  int lNJet     = 0;
  for(unsigned int i0 = 0; i0 < iJets->size(); i0++) {
    const PFJet     *pJet   = &(iJets->at(i0));
    double pDEta1 = pJet->eta() - iEta1;
    double pDPhi1 = fabs(pJet->phi() - iPhi1); if(pDPhi1 > 2.*TMath::Pi()-pDPhi1) pDPhi1 = 2.*TMath::Pi()-pDPhi1;
    double pDR1   = sqrt(pDEta1*pDEta1 + pDPhi1*pDPhi1);
    if(pDR1 < 0.5) continue;
    double pDEta2 = pJet->eta() - iEta2;
    double pDPhi2 = fabs(pJet->phi() - iPhi2); if(pDPhi2 > 2.*TMath::Pi()-pDPhi2) pDPhi2 = 2.*TMath::Pi()-pDPhi2;
    double pDR2   = sqrt(pDEta2*pDEta2 + pDPhi2*pDPhi2);
    if(pDR2 < 0.5) continue;  
    if(!fUtils->passPFLooseId(pJet)) continue;
    lNAllJet++;
    Double_t pPt = pJet->pt();
    if(pPt  > 30.)  lNJet++;
    if(lPt0 < pPt) {lPt0 = pPt; lLead = pJet; continue;}    
    if(lPt1 < pPt) {lPt1 = pPt; l2nd  = pJet; continue;}
  }
  
  fU       = lPFRec.pt();
  fUPhi    = lPFRec.phi();
  fTKSumEt = lTKRec.sumEt()/lPFRec.sumEt();
  fTKU     = lTKRec.pt();
  fTKUPhi  = lTKRec.phi();
  fNPSumEt = lNPRec.sumEt()/lPFRec.sumEt();
  fNPU     = lNPRec.pt();
  fNPUPhi  = lNPRec.phi();
  fPUSumEt = lPUMet.sumEt()/lPFRec.sumEt();
  fPUMet   = lPUMet.pt();
  fPUMetPhi= lPUMet.phi();
  fPCSumEt = lPCRec.sumEt()/lPFRec.sumEt();
  fPCU     = lPCRec.pt()    ;
  fPCUPhi  = lPCRec.phi()   ;
  fJSPt1   = lPt0; 
  fJSEta1  = 0; if(lLead != 0) fJSEta1 = lLead->eta();
  fJSPhi1  = 0; if(lLead != 0) fJSPhi1 = lLead->phi();
  fJSPt2   = lPt1; 
  fJSEta2  = 0; if(l2nd  != 0) fJSEta2 = l2nd ->eta();
  fJSPhi2  = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->phi();
  fNJet    = lNJet   ;
  fNAllJet = lNAllJet;
  fNPV     = iNPV    ;

  Float_t lMVA = evaluatePhi();
  
  if(!iPhi) fUPhiMVA = fUPhi + lMVA; 
  fTKSumEt  /= lPFRec.sumEt();
  fNPSumEt  /= lPFRec.sumEt();
  fPUSumEt  /= lPFRec.sumEt();
  fPCSumEt  /= lPFRec.sumEt();
  if(!iPhi) lMVA     = evaluateU1();

  TLorentzVector lUVec (0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec (0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis ,0,lPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  double lSumEt = 0; //===> This is where we add the significance regression
  
  //CommonMETData lOutMet;
  //lOutMet.mex   =  lUVec.Px();
  //lOutMet.mey   =  lUVec.Py();
  //lOutMet.mez   =  0;
  //lOutMet.met   =  lUVec.Pt();
  //lOutMet.phi   =  lUVec.Phi();
  //lOutMet.sumet = 0; 

  if (printDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU      << " : "
	      <<  fPCUPhi   << " : "
	      <<  fJSPt1    << " : "
	      <<  fJSEta1   << " : "
	      <<  fJSPhi1   << " : "
	      <<  fJSPt2    << " : "
	      <<  fJSEta2   << " : "
	      <<  fJSPhi2   << " : "
	      <<  fNJet     << " : "
	      <<  fNAllJet  << " : "
	      <<  fNPV      << " : "
              << " === : === "
              << std::endl;
  }
  Candidate::LorentzVector lMetVec(lUVec.Px(),lUVec.Py(),lUVec.Pz(),lUVec.E());
  return PFMET(iPFMet->getSpecific(),lSumEt,lMetVec,iPFMet->vertex());
}
std::pair<MVAMet::LorentzVector,double>  MVAMet::GetMet(std::vector<LorentzVector>                     iVis,
							std::vector<std::pair<LorentzVector,double> >  iJets,
							std::vector<std::pair<LorentzVector,double> >  iCands,
							std::vector<Vector>                            iVertices,
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
  std::pair<LorentzVector,double> lPUMet  = fUtils->PUMet     (               iCands,iJets,fDZCut);
  
  int lNJets           = fUtils->NJets(iJets,30);
  int lNAllJet         = int(iJets.size());
  LorentzVector *lLead = fUtils->leadPt(iJets,true); 
  LorentzVector *l2nd  = fUtils->leadPt(iJets,false); 
    
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
  fJSPt1   = lLead->pt();
  fJSEta1  = 0; if(lLead != 0) fJSEta1 = lLead->eta();
  fJSPhi1  = 0; if(lLead != 0) fJSPhi1 = lLead->phi();
  fJSPt2   = l2nd ->pt();
  fJSEta2  = 0; if(l2nd  != 0) fJSEta2 = l2nd ->eta();
  fJSPhi2  = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->phi();
  fNJet    = lNJets  ;
  fNAllJet = lNAllJet;
  fNPV     = iVertices.size();

  Float_t lMVA = evaluatePhi();
  
  fUPhiMVA  = fUPhi + lMVA; 
  fTKSumEt  /= lPFRec.second;
  fNPSumEt  /= lPFRec.second;
  fPUSumEt  /= lPFRec.second;
  fPCSumEt  /= lPFRec.second;
  lMVA      = evaluateU1();

  TLorentzVector lUVec (0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec (0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis ,0,lPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  double lSumEt = 0; //===> This is where we add the significance regression

  LorentzVector  lMetVec (0,0,0,0);   lMetVec.SetCoordinates(lUVec.Px(),lUVec.Py(),lUVec.Pz(),lUVec.E());
  if (iPrintDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU      << " : "
	      <<  fPCUPhi   << " : "
	      <<  fJSPt1    << " : "
	      <<  fJSEta1   << " : "
	      <<  fJSPhi1   << " : "
	      <<  fJSPt2    << " : "
	      <<  fJSEta2   << " : "
	      <<  fJSPhi2   << " : "
	      <<  fNJet     << " : "
	      <<  fNAllJet  << " : "
	      <<  fNPV      << " : "
              << " === : === "
              << std::endl;
  }
  std::pair<LorentzVector,double> lMet(lMetVec,lSumEt);
  return lMet;
}
