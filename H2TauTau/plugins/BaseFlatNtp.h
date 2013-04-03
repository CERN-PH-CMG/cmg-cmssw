#ifndef CMGH2TAUTAU_BaseFlatNtp_h
#define CMGH2TAUTAU_BaseFlatNtp_h

#include <TRandom2.h>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
#include <TROOT.h>
#include <TNamed.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TF1.h>
#include <TVector3.h>


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"


#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/SelectionEfficiency.h"
//#include "CMGTools/H2TauTau/interface/TauRate.h"

//#include "CMGTools/Common/interface/RecoilCorrector.h"
#include "CMGTools/Utilities/interface/RecoilCorrector.h"

#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"


#include "CMGTools/H2TauTau/interface/BTagEfficiency.h"
#include "CMGTools/H2TauTau/interface/BTagWeight.h"
//#include "CMGTools/H2TauTau/interface/BtagSF.h"
#include "CMGTools/RootTools/interface/BTagSF.h"


//#include "TMVA/Reader.h"
#include "CMGTools/H2TauTau/interface/VBFMVA.h"
#include "CMGTools/H2TauTau/interface/VBFMVA2012.h"

using namespace std;

class BaseFlatNtp : public edm::EDAnalyzer{


 public:
  explicit BaseFlatNtp(const edm::ParameterSet&);
  ~BaseFlatNtp();

  virtual void beginJob() ;
  //virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;

protected:

  edm::Service<TFileService> * file_;
  TTree * tree_;

  int printSelectionPass_;

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

  int dataPeriodFlag_;
  edm::InputTag verticesListTag_;
  edm::InputTag trigPathsListTag_;
  edm::InputTag trigObjsListTag_;
  int firstRun_; 
  int lastRun_; 
  int dataType_;//0=MC, 1=Data, 2=EmbeddedData, 3=EmbeddedMC

   //configurable selections
  float muPtCut_;
  float tauPtCut_;
  float muEtaCut_;
  float tauEtaCut_;
  int metType_;
  float metscale_;
  int runSVFit_;

  
  float smearSVFitMass0pi0_;
  float smearVisMass0pi0_;
  float smearSVFitMass1pi0_;
  float smearVisMass1pi0_;

  //event info
  const edm::Event * iEvent_; 

  edm::Handle< std::vector<reco::Vertex> > vertices_;
  const reco::Vertex * PV_;

  edm::Handle< std::vector<cmg::TriggerObject> > trig_;
  std::vector<edm::InputTag *>  trigPaths_;
  std::vector<edm::InputTag *>  trigPathsTest_;

  edm::InputTag genParticlesTag_;
  const reco::GenParticle * genBoson_;
  const reco::GenParticle * genBosonL1_;
  const reco::GenParticle * genBosonL2_;
  int sampleGenEventType_;
  int sampleTruthEventType_;
  int genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu
  int truthEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZOther 11=WToENu, 13=WToMuNu, 15=WToENu, 16=WOther
  float deltaRTruth_;


  edm::InputTag diTauTag_;

  edm::InputTag unscaledTauTag_;
  edm::Handle< std::vector<cmg::Tau> > unscaledTauList_;


  edm::InputTag muonVetoListTag_;
  edm::Handle< std::vector<cmg::Muon> > leptonVetoListMuon_;
  edm::InputTag electronVetoListTag_;
  edm::Handle< std::vector<cmg::Electron> > leptonVetoListElectron_;
  edm::InputTag tauVetoListTag_;
  edm::Handle< std::vector<cmg::Tau> > leptonVetoListTau_;

  edm::InputTag pfJetListTag_;
  std::vector<const cmg::PFJet * > fullJetList_;
  std::vector<const cmg::PFJet * > pfJetList_;
  std::vector<const cmg::PFJet * > pfJetListLC_;
  std::vector<const cmg::PFJet * > pfJetListLepLC_;
  const cmg::PFJet * leadJet_;
  const cmg::PFJet * subleadJet_;

  std::vector<const cmg::PFJet * > pfJetList20_;//Jets pf pT>20
  std::vector<const cmg::PFJet * > pfJetList20LC_;

  std::vector<const cmg::PFJet * > pfJetListBTagLC_;
  const cmg::PFJet * leadBJet_;

  std::vector<const cmg::PFJet * > pfJetListBTagLCLoose_;

  std::string pujetidname_;

  const cmg::METSignificance * metSig_;

  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;
  float triggerEffWeightMu_;
  float triggerEffWeightTau_;
  float triggerEffWeightsMu_[5];
  float triggerEffWeightsTau_[5];

  SelectionEfficiency selectionEff_;
  float selectionEffWeight_;
  float selectionEffWeightId_;
  float selectionEffWeightIso_;
  float selectionEffWeightsId_[5];
  float selectionEffWeightsIso_[5];

  //HQT weights for 2011 signal samples
  std::string signalWeightDir_;
  std::string signalWeightMass_;
  TH1F* signalWeightHisto_;
  float signalWeight_;


  //for MSSM
  float btagWP_;

  BTagSF btagsf;

  std::vector<const cmg::PFJet * > pfJetListBTagWeight_;
  BTagWeight btagWeight_;
  BTagEfficiency btagEff_;
  float btagEffWeight_;


  //generator variables
  int saveLHENUP_;
  int lhenup_;
  float genbosonmass_;
  float genbosonpt_;
  float genbosoneta_;
  float genbosonphi_;


  //event variables
  edm::InputTag pupWeightName_;
  float pupWeight_;
  float eventweight_;
  float embeddedGenWeight_;//for tau embedded samples

  edm::InputTag pupWeightNames_[5];
  float pupWeights_[5];

  int runnumber_;
  int lumiblock_;
  int eventid_;
  int trigPath_[10];
  int trigTest_[10];

  int npu_;
  int nvtx_;
  float vtxx_;
  float vtxy_;
  float vtxz_;


  //di-tau variables
  int   nditau_;
  float ditaumass_;
  int   ditaucharge_;
  float ditaueta_;
  float ditaupt_;
  float ditauphi_;
  float svfitmass_;
  float mutaucostheta_;
  float ditaudeltaR_;
  float ditaudeltaEta_;
  float ditaudeltaPhi_;

  //lepton variables
  float mupt_;
  float mueta_;
  float muphi_;
  float muiso_;
  float muisomva_;
  float mudz_;
  float mudxy_;
  float mux_;
  float muy_;
  float muz_;
  int   mujetmatch_;//0= no match, 1 lead jet, 2=subleading jet //check which jet list is being used !
  float mujetpt_;
  float mujeteta_;
  int   mutruth_; 
  int   mutruthstatus_;
  float mutruthpt_;
  float mutrutheta_;
  math::XYZTLorentzVector mup4_;
  int   mucharge_;
  

  //tau variables
  float taumass_;
  float taupt_;
  float taupx_;
  float taupy_;
  float taueta_;
  float tauphi_;
  int   tautruth_;
  int   tautruthstatus_;
  float tautruthpt_;
  float tautrutheta_;
  float tauehop_;
  float taueop_;
  float tauhoe_;
  int   taudecaymode_;
  float taudz_;
  float taudxy_;
  float taux_;
  float tauy_;
  float tauz_;
  int   tauantie_;
  int   tauantimu_;
  int   tauisodisc_;
  int   tauisodiscmva_;
  float tauiso_;
  float tauisomva_;
  float taujetpt_;
  float taujeteta_;
  float taujetrefpt_;
  float taujetrefeta_;
  float tauleadpt_;
  float tauleadhcal_;
  float tauleadecal_;
  math::XYZTLorentzVector taup4_;
  int   taucharge_;

  //met variables
  float pftransversemass_;
  double pfmetpt_;
  double pfmetphi_;
  float transversemass_;
  double metptraw_;//save met without recoil correction
  double metphiraw_;
  double metpt_;//double needed by recoil corrector
  double metphi_;
  float  metsigcov00_;
  float  metsigcov01_;
  float  metsigcov10_;
  float  metsigcov11_;
  float  pZeta_;
  float  pZetaVis_;
  reco::Candidate::PolarLorentzVector metP4_;

  //mva met inputs
  float  pfMetForRegression_;
  float  tkMet_;
  float  nopuMet_;
  float  puMet_;
  float  pcMet_;





  //jet variables
  int njet_;
  float leadJetPt_;
  float leadJetEta_;
  float leadJetRawFactor_;
  float leadJetPUIdMva_;
  float subleadJetPt_;
  float subleadJetEta_;
  float subleadJetRawFactor_;
  float diJetMass_;
  float diJetPt_;
  float diJetDeltaEta_;
  float diJetEta1Eta2_;
  int   njetingap_;


  //need up to 4 jets (pT20): pt, eta, phi, flavor
  int njet20_;
  float jet20pt1_;
  float jet20eta1_;
  float jet20phi1_;
  float jet20mass1_;
  float jet20tagprob1_;
  int jet20flavor1_;
  float jet20pt2_;
  float jet20eta2_;
  float jet20phi2_;
  float jet20mass2_;
  float jet20tagprob2_;
  int jet20flavor2_;
  float jet20pt3_;
  float jet20eta3_;
  float jet20phi3_;
  float jet20mass3_;
  float jet20tagprob3_;
  int jet20flavor3_;
  float jet20pt4_;
  float jet20eta4_;
  float jet20phi4_;
  float jet20mass4_;
  float jet20tagprob4_;
  int jet20flavor4_;


  int nbjet_;
  float leadBJetBTagProb_;
  float leadBJetPt_;
  float leadBJetEta_;

  int nbjetLoose_;

  int njetLepLC_;
  float muLCleadJetPt_;//jets where only the muon has been removed
  float muLCleadJetEta_;


  int categoryIso_;//
  int categorySM_;//SM search 
  int categorySM2012_;//SM search 

  



  //selection variables
  int runrangepass_;
  int trigpass_;
  int nvtxpass_;
  int geneventtypepass_;
  int baseeventpass_;
  int dilepvetopass_;
  int ptetapass_;
  int muvtxpass_;
  int muidpass_;
  int muisopass_;
  int mumatchpass_;
  int taueoppass_;
  int tauvtxpass_;
  int taumuvetopass_;
  int tauelevetopass_;
  int tauisopass_;
  int taumatchpass_;
  int ditauisopass_;


  //event counters
  int counterall_;
  int countertrig_;
  int counterruns_;
  int countergoodvtx_;
  int countergen_;


  //Utilities
  void fillTruthEventType(float taueta,float tauphi, float mueta, float muphi){
    //Z-->ll
    if(abs(genBoson_->pdgId())==23){
      if(genBosonL1_&&genBosonL2_){
	if(((reco::deltaR(taueta,tauphi,genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	    ||(reco::deltaR(taueta,tauphi,genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_))
	   && ((reco::deltaR(mueta,muphi,genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	       ||(reco::deltaR(mueta,muphi,genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_))
	   ) truthEventType_=abs(genBosonL1_->pdgId())-10;
	else truthEventType_=6;
      }else truthEventType_=6;
    }
    
    //W-->lnu
    if(abs(genBoson_->pdgId())==24){
      if(genBosonL1_){
	if(reco::deltaR(mueta,muphi,genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	  truthEventType_=abs(genBosonL1_->pdgId());
	else truthEventType_=16;
      }else if(genBosonL2_){	  
	if(reco::deltaR(mueta,muphi,genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_)
	  truthEventType_=abs(genBosonL2_->pdgId());
	else truthEventType_=16;
      }else truthEventType_=16;
    }

  }

  cmg::BaseMET diobjectmet_;
  int diobjectindex_;
  edm::InputTag mvaMETTag_;
  edm::InputTag mvaMETSigTag_;

  RecoilCorrector corrector_;
  //RecoilCorrector2012 corrector2012_;
  int recoilCorreciton_;
  double recoiliScale_;
  std::string fileZmmData_;
  std::string fileZmmMC_;
  
  void fillMET(){

    //carry the PFMET always but dont use for SVFit
    edm::Handle<std::vector< cmg::BaseMET> > pfMET;
    iEvent_->getByLabel(edm::InputTag("cmgPFMETRaw"),pfMET);
    pfmetpt_=pfMET->begin()->pt();
    pfmetphi_=pfMET->begin()->phi();

    ////Below choose the MET for SVFit and transverse mass
    if(metType_==1){//PFMET
      edm::Handle<std::vector< cmg::BaseMET> > pfMET;
      iEvent_->getByLabel(edm::InputTag("cmgPFMETRaw"),pfMET);
      metpt_=pfMET->begin()->pt();
      metphi_=pfMET->begin()->phi();

      edm::Handle< cmg::METSignificance > pfMetSigHandle;
      iEvent_->getByLabel(edm::InputTag("pfMetSignificance"),pfMetSigHandle); 
      metSig_ = &(*pfMetSigHandle);
    }
    
    if(metType_==2){//MVA MET 
      edm::Handle<std::vector< cmg::BaseMET> > mvaMETVector;
      iEvent_->getByLabel(mvaMETTag_,mvaMETVector); 
      metpt_=(mvaMETVector->at(diobjectindex_)).pt();
      metphi_=(mvaMETVector->at(diobjectindex_)).phi();

      edm::Handle< std::vector<cmg::METSignificance> > metsigVector;
      iEvent_->getByLabel(mvaMETSigTag_,metsigVector); 
      metSig_ = &(metsigVector->at(diobjectindex_));
    }
    if(metType_==3){//MVA MET with presel leptons
      edm::Handle<std::vector< cmg::BaseMET> > mvaMET;
      iEvent_->getByLabel(mvaMETTag_,mvaMET);
      metpt_=mvaMET->begin()->pt();
      metphi_=mvaMET->begin()->phi();
      
      edm::Handle< std::vector< cmg::METSignificance > > mvaMetSigHandle;
      iEvent_->getByLabel(mvaMETSigTag_,mvaMetSigHandle); 
      metSig_ = &(*(mvaMetSigHandle->begin()));
    }
	  
    if(metType_==5){//Type 1 Corrected MET
      edm::Handle<std::vector< cmg::BaseMET> > pfMET;
      iEvent_->getByLabel(edm::InputTag("cmgPFMET"),pfMET);
      metpt_=pfMET->begin()->pt();
      metphi_=pfMET->begin()->phi();
      
      edm::Handle< cmg::METSignificance > pfMetSigHandle;
      iEvent_->getByLabel(edm::InputTag("pfMetSignificance"),pfMetSigHandle); 
      metSig_ = &(*pfMetSigHandle);
    }
    
    
    if(metType_==2 || metType_==3){//Save the mva met inputs
      edm::Handle<std::vector< reco::PFMET> > mvaInputMET;
      iEvent_->getByLabel(edm::InputTag("pfMetForRegression"),mvaInputMET);
      pfMetForRegression_=mvaInputMET->begin()->pt();
      iEvent_->getByLabel(edm::InputTag("tkMet"),mvaInputMET);
      tkMet_=mvaInputMET->begin()->pt();
      iEvent_->getByLabel(edm::InputTag("nopuMet"),mvaInputMET);
      nopuMet_=mvaInputMET->begin()->pt();
      iEvent_->getByLabel(edm::InputTag("puMet"),mvaInputMET);
      puMet_=mvaInputMET->begin()->pt();
      iEvent_->getByLabel(edm::InputTag("pcMet"),mvaInputMET);
      pcMet_=mvaInputMET->begin()->pt();
    }


    if(!metSig_){
      cout<<" Unrecognized metType "<<endl;
      exit(0);
    }
    
    
    //apply MET shift first:
    // 1) find the corresponding Tau first
    // 2) calculate the difference in p4
    // 3) subtract from MET    
    float dpx=0.;
    float dpy=0.;
    for(std::vector<cmg::Tau>::const_iterator cand=unscaledTauList_->begin(); cand!=unscaledTauList_->end(); ++cand){
      if(cand->eta()==taueta_ && cand->phi()==tauphi_){
	dpx=taupx_ - cand->p4().x();
	dpy=taupy_ - cand->p4().y();
	break;
      }
    }
    //cout<<dpx<<" "<<dpy<<endl;
    math::XYZTLorentzVector unscaledmetP4(metpt_*cos(metphi_),metpt_*sin(metphi_),0,0);    
    math::XYZTLorentzVector deltaTauP4(dpx,dpy,0,0);
    math::XYZTLorentzVector scaledmetP4 = unscaledmetP4 - deltaTauP4;
    metpt_=scaledmetP4.pt();
    metphi_=scaledmetP4.phi();


    //save met without recoil correction
    metptraw_=metpt_;
    metphiraw_=metphi_;
    if(recoilCorreciton_>0){
      if(!genBoson_){
	cout<<" recoilCorrection requested but no genBoson_ available"<<endl;
	exit(0);
      }
      
      double u1 = 0.;
      double u2 = 0.;
      double fluc = 0.;
      double lepPt  = 0.;
      double lepPhi = 0.;
      int jetMult = 0;
      if(recoilCorreciton_%10==1){//for Z
	lepPt  = ditaupt_;
	lepPhi = ditauphi_;
	jetMult = njet_;
      }else if(recoilCorreciton_%10==2){//for W+jets
	lepPt  =mupt_;
	lepPhi =muphi_;
	jetMult = njetLepLC_;
      }
      
      if(recoilCorreciton_<10) 
	corrector_.CorrectType1(metpt_,metphi_,genBoson_->pt(), genBoson_->phi(),  lepPt, lepPhi,  u1, u2, fluc, recoiliScale_ , jetMult );
      else if(recoilCorreciton_<20)
	corrector_.CorrectType2(metpt_,metphi_,genBoson_->pt(), genBoson_->phi(),  lepPt, lepPhi,  u1, u2, fluc, recoiliScale_ , jetMult );
    }
    
    //met correction needed for ZEE
    if(metscale_>0.)metpt_*=metscale_;

    metP4_=reco::Candidate::PolarLorentzVector(metpt_,0,metphi_,0);
    metsigcov00_=(*(metSig_->significance()))[0][0];
    metsigcov01_=(*(metSig_->significance()))[0][1];
    metsigcov10_=(*(metSig_->significance()))[1][0];
    metsigcov11_=(*(metSig_->significance()))[1][1];
    
  }
    

  TRandom2 randEngine_; 
  double randsigma_;
  



  float vbfmva_;
  double vbfvars_[8];
  std::string mvaWeights_ ;
  VBFMVA reader_;
  void fillVBFMVA(){
    TVector3 vTau, vMu, vMET, vDiTau, vDiTauVis;
    vTau.SetPtEtaPhi(taupt_,taueta_,tauphi_);
    vMu.SetPtEtaPhi(mupt_,mueta_,muphi_);
    vMET.SetPtEtaPhi(metpt_,0,metphi_); 

    vDiTau = vTau + vMu + vMET;
    vDiTauVis = vTau + vMu;
    
    TVector3 vJet1, vJet2, vDiJet;
    //vJet1.SetPtEtaPhi(leadJet_->pt(), leadJet_->eta(), leadJet_->phi());
    //vJet2.SetPtEtaPhi(subleadJet_->pt(), subleadJet_->eta(), subleadJet_->phi());
    vJet1.SetPtEtaPhi(jet20pt1_,jet20eta1_,jet20phi1_);
    vJet2.SetPtEtaPhi(jet20pt2_,jet20eta2_,jet20phi2_);

    vDiJet = vJet1 + vJet2;

//     Double_t mjj = massPtEtaPhiM(leadJet_->pt(), leadJet_->eta(), leadJet_->phi(), leadJet_->mass(),subleadJet_->pt(), subleadJet_->eta(), subleadJet_->phi(), subleadJet_->mass());
//     Double_t dEta = fabs(leadJet_->eta() - subleadJet_->eta());
//     Double_t dPhi = deltaPhi(leadJet_->phi(), subleadJet_->phi());
    Double_t mjj = massPtEtaPhiM(jet20pt1_,jet20eta1_,jet20phi1_,jet20mass1_,jet20pt2_,jet20eta2_,jet20phi2_,jet20mass2_);
    Double_t dEta = fabs(jet20eta1_ - jet20eta2_);
    Double_t dPhi = deltaPhi(jet20phi1_,jet20phi2_);

    Double_t dPhi_hj = deltaPhi(vDiTau.Phi(), vDiJet.Phi());

    // Lorenzo's variables
    Double_t C1 = min(fabs(vDiTauVis.Eta() - jet20eta1_), fabs(vDiTauVis.Eta() - jet20eta2_));
    Double_t C2 = vDiTauVis.Pt();
    
    // Fill input vector
    vbfvars_[0] = mjj;
    vbfvars_[1] = dEta;
    vbfvars_[2] = dPhi;
    vbfvars_[3] = vDiTau.Pt();
    vbfvars_[4] = vDiJet.Pt();
    vbfvars_[5] = dPhi_hj;
    vbfvars_[6] = C1;
    vbfvars_[7] = C2;

    vbfmva_ = reader_.val(vbfvars_[0],vbfvars_[1],vbfvars_[2],vbfvars_[3],vbfvars_[4],vbfvars_[5],vbfvars_[6],vbfvars_[7]);
  }


  float vbfmva2012_;
  double vbfvars2012_[8];
  std::string mvaWeights2012_;
  VBFMVA2012 reader2012_;
  void fillVBFMVA2012(){
    TVector3 vTau, vMu, vMET, vDiTau, vDiTauVis;
    vTau.SetPtEtaPhi(taupt_,taueta_,tauphi_);
    vMu.SetPtEtaPhi(mupt_,mueta_,muphi_);
    vMET.SetPtEtaPhi(metpt_,0,metphi_); 

    vDiTau = vTau + vMu + vMET;
    vDiTauVis = vTau + vMu;
    
    TVector3 vJet1, vJet2, vDiJet;
    vJet1.SetPtEtaPhi(jet20pt1_,jet20eta1_,jet20phi1_);
    vJet2.SetPtEtaPhi(jet20pt2_,jet20eta2_,jet20phi2_);
    vDiJet = vJet1 + vJet2;


    Double_t mjj = massPtEtaPhiM(jet20pt1_,jet20eta1_,jet20phi1_,jet20mass1_,jet20pt2_,jet20eta2_,jet20phi2_,jet20mass2_);
    Double_t dEta = fabs(jet20eta1_ - jet20eta2_);

    // Lorenzo's variables
    Double_t C1 = min(fabs(vDiTauVis.Eta() - jet20eta1_), fabs(vDiTauVis.Eta() - jet20eta2_));
    Double_t C2 = vDiTauVis.Pt();
    
    // Fill input vector
    vbfvars2012_[0] = mjj;
    vbfvars2012_[1] = dEta;
    vbfvars2012_[2] = C1;
    vbfvars2012_[3] = C2;

    vbfmva2012_ = reader2012_.val(vbfvars2012_[0],vbfvars2012_[1],vbfvars2012_[2],vbfvars2012_[3]);
  }



  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  void printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList);
  
  edm::Handle< std::vector<cmg::TriggerObject> > trigObjs_;
  const cmg::TriggerObject * trigObjMatch(float eta, float phi, std::string path, std::string filter, int pdgid=-1);

  int truthMatchLeg(float legeta, float legphi,float& truthpt,float& trutheta,int& truthstatus);

  bool checkPUJetId(const cmg::PFJet * jet);
  void fillPFJetList(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetList20(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetListLC(float leg1eta, float leg1phi, float leg2eta, float leg2phi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);
  void fillPFJetListLepLC(float lepeta, float lepphi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);

  void fillPFJetListBTag(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetListBTagLoose(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);

  const cmg::PFJet * findJet(std::vector<const cmg::PFJet * > * fulllist, float eta, float phi);

  bool checkPFJetId(const cmg::PFJet * jet){
    //Loose PF Jet id : https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    if(fabs(jet->eta()) <= 2.4){
      if((jet->component(5).fraction() + jet->component(6).fraction()) < 0.99
	 &&jet->component(4).fraction() < 0.99
	 &&jet->nConstituents() > 1
	 &&jet->component(1).fraction() > 0
	 &&jet->component(1).number() > 0
	 &&jet->component(2).fraction() < 0.99 
	 ) return 1;
    } else {
      if((jet->component(5).fraction() + jet->component(6).fraction()) < 0.99
	 &&jet->component(4).fraction() < 0.99
	 &&jet->nConstituents() > 1
	 ) return 1;
    }
    return 0;
  }


  void fillJetVariables(){

    njet_=pfJetListLC_.size();
    leadJet_=0;
    subleadJet_=0;
    njetingap_=0;
    if(njet_>0)leadJet_=pfJetListLC_[0];
    if(njet_>1)subleadJet_=pfJetListLC_[1];

    //jet quantities independent of SM category
    if(pfJetListLC_.size()>=1){
      leadJetPt_=leadJet_->pt();
      leadJetEta_=leadJet_->eta();
      leadJetRawFactor_=leadJet_->rawFactor();
      leadJetPUIdMva_=leadJet_->puMva(pujetidname_.c_str());
    }
    if(pfJetListLC_.size()>=2){
      subleadJetPt_=subleadJet_->pt();
      subleadJetEta_=subleadJet_->eta();
      subleadJetRawFactor_=subleadJet_->rawFactor();
      diJetMass_=(leadJet_->p4()+subleadJet_->p4()).mass();
      diJetPt_ = (leadJet_->p4()+subleadJet_->p4()).pt();  
      diJetDeltaEta_=fabs(leadJet_->eta() - subleadJet_->eta());
      diJetEta1Eta2_=(leadJet_->eta())*(subleadJet_->eta());
     
      for(std::vector<const cmg::PFJet *>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	if(leadJet_->eta()<subleadJet_->eta()) 
	  if(leadJet_->eta()<(*jet3)->eta()&&(*jet3)->eta()<subleadJet_->eta()) njetingap_++;
	if(leadJet_->eta()>subleadJet_->eta()) 
	  if(subleadJet_->eta()<(*jet3)->eta()&&(*jet3)->eta()<leadJet_->eta()) njetingap_++;
      }
      
    }
    
    //Jets where only the muon has been removed
    njetLepLC_=pfJetListLepLC_.size();
    if(pfJetListLepLC_.size()>=1){
      muLCleadJetPt_=pfJetListLepLC_[0]->pt();
      muLCleadJetEta_=pfJetListLepLC_[0]->eta();
    }

  }

  void fillBJetVariables(){
    nbjet_=pfJetListBTagLC_.size();
    leadBJet_ = 0 ;
    if(nbjet_>0){
      leadBJet_ = pfJetListBTagLC_[0];
      leadBJetPt_ = leadBJet_->pt();
      leadBJetEta_ = leadBJet_->eta();
      leadBJetBTagProb_ = leadBJet_->btag("combinedSecondaryVertexBJetTags");
    }

    //Loose b-tag jets
    nbjetLoose_=pfJetListBTagLCLoose_.size();
    
  }

  

  void fillJetVariables20(){
    njet20_=pfJetList20LC_.size();

    jet20pt1_=0.;
    jet20eta1_=0.;
    jet20phi1_=0.;
    jet20mass1_=0.;
    jet20tagprob1_=0.;
    jet20flavor1_=0.;
    jet20pt2_=0.;
    jet20eta2_=0.;
    jet20phi2_=0.;
    jet20mass2_=0.;
    jet20tagprob2_=0.;
    jet20flavor2_=0.;
    jet20pt3_=0.;
    jet20eta3_=0.;
    jet20phi3_=0.;
    jet20mass3_=0.;
    jet20tagprob3_=0.;
    jet20flavor3_=0.;
    jet20pt4_=0.;
    jet20eta4_=0.;
    jet20phi4_=0.;
    jet20mass4_=0.;
    jet20tagprob4_=0.;
    jet20flavor4_=0.;

    if(njet20_>=1){
      jet20pt1_=pfJetList20LC_[0]->pt();
      jet20eta1_=pfJetList20LC_[0]->eta();
      jet20phi1_=pfJetList20LC_[0]->phi();
      jet20mass1_=pfJetList20LC_[0]->mass();
      jet20tagprob1_=pfJetList20LC_[0]->btag("combinedSecondaryVertexBJetTags");;
      jet20flavor1_=pfJetList20LC_[0]->partonFlavour();  
    }
    if(njet20_>=2){
      jet20pt2_=pfJetList20LC_[1]->pt();
      jet20eta2_=pfJetList20LC_[1]->eta();
      jet20phi2_=pfJetList20LC_[1]->phi();
      jet20mass2_=pfJetList20LC_[1]->mass();
      jet20tagprob2_=pfJetList20LC_[1]->btag("combinedSecondaryVertexBJetTags");;
      jet20flavor2_=pfJetList20LC_[1]->partonFlavour();  
    }

    if(njet20_>=3){
      jet20pt3_=pfJetList20LC_[2]->pt();
      jet20eta3_=pfJetList20LC_[2]->eta();
      jet20phi3_=pfJetList20LC_[2]->phi();
      jet20mass3_=pfJetList20LC_[2]->mass();
      jet20tagprob3_=pfJetList20LC_[2]->btag("combinedSecondaryVertexBJetTags");;
      jet20flavor3_=pfJetList20LC_[2]->partonFlavour();  
    }

    if(njet20_>=4){
      jet20pt4_=pfJetList20LC_[3]->pt();
      jet20eta4_=pfJetList20LC_[3]->eta();
      jet20phi4_=pfJetList20LC_[3]->phi();
      jet20mass4_=pfJetList20LC_[3]->mass();
      jet20tagprob4_=pfJetList20LC_[3]->btag("combinedSecondaryVertexBJetTags");;
      jet20flavor4_=pfJetList20LC_[3]->partonFlavour();  
    }
    
  }


  void fillBTagWeight(){
    btagEffWeight_ = 1.;
    if(dataType_!=0) return;//only done for MC

    vector<vector<BTagWeight::JetInfo> > jetinfovec;
    for (unsigned int i=0; i<pfJetList20LC_.size(); ++i) {//was pfJetListB which has eta cut 2.4, pt>20, and no Lepton cleaning
      if(abs(pfJetList20LC_[i]->eta())>2.4) continue;
      //    int index = pfJetList20LC_[i];
      double jetpt = pfJetList20LC_[i]->pt(); 
      double jeteta = pfJetList20LC_[i]->eta(); 
      double jetflavor = TMath::Abs(pfJetList20LC_[i]->partonFlavour());
      double discr = btagWP_;//1.7;
    
      double eff = 1.;
      double sf = 1.;
      if (jetflavor==5){
	eff = btagEff_.btagEFF(discr,1);
	sf =  btagEff_.btagSF(jetpt,1);
      }else if (jetflavor==4){
	eff = btagEff_.btagEFF(discr,0);
	sf =  btagEff_.btagSF(jetpt,0);
      }else{
	eff = btagEff_.mistagEFF(jetpt,jeteta);
	sf =  btagEff_.mistagSF(jetpt,jeteta);
      }
    
      BTagWeight::JetInfo jetinfo(eff,sf);
      vector<BTagWeight::JetInfo> jetInfoForAllOPs;
      jetInfoForAllOPs.push_back(jetinfo);
      jetinfovec.push_back(jetInfoForAllOPs);
    
    }
    btagEffWeight_ = btagWeight_.weight(jetinfovec);
  }
  
  //function definitions from Matthews mva
  Double_t deltaPhi(Double_t phi1, Double_t phi2){
    Double_t dphi = fabs(phi1 - phi2);
    return dphi <= TMath::Pi() ? dphi : 2*TMath::Pi() - dphi; 
  }
  Double_t massPtEtaPhiM(Double_t pt1, Double_t eta1, Double_t phi1, Double_t m1,
			 Double_t pt2, Double_t eta2, Double_t phi2, Double_t m2)
  {
    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<Double_t> > mom1(pt1, eta1, phi1, m1);
    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<Double_t> > mom2(pt2, eta2, phi2, m2);
    return (mom1+mom2).M();
  }  

  float computeDxy(reco::TrackBase::Point vtx, math::XYZTLorentzVector p4){
    //methods from here http://cmslxr.fnal.gov/lxr/source/DataFormats/TrackReco/interface/TrackBase.h#063
    return ( - (vtx.x()-PV_->position().x()) *  p4.y() + (vtx.y()-PV_->position().y()) *  p4.x() ) /  p4.pt();
  }
  float computeDz(reco::TrackBase::Point vtx, math::XYZTLorentzVector p4){
    //methods from here http://cmslxr.fnal.gov/lxr/source/DataFormats/TrackReco/interface/TrackBase.h#063
    return (vtx.z()-PV_->position().z()) - ((vtx.x()-PV_->position().x()) * p4.x()+(vtx.y()-PV_->position().y())*  p4.y())/ p4.pt() *  p4.z()/ p4.pt();
  }


  //pZeta computation //Code from Josh  
  //void compZeta(const cmg::Muon * leg1, const cmg::Tau * leg2, double metPx, double metPy, float * pZ, float * pZV){
  void compZeta(math::XYZTLorentzVector leg1, math::XYZTLorentzVector leg2, double metPx, double metPy, float * pZ, float * pZV){
    //leg1 is the muon and leg2 is the tau

    double leg1x = cos(leg1.phi());
    double leg1y = sin(leg1.phi());
    double leg2x = cos(leg2.phi());
    double leg2y = sin(leg2.phi());
    double zetaX = leg1x + leg2x;
    double zetaY = leg1y + leg2y;
    double zetaR = TMath::Sqrt(zetaX*zetaX + zetaY*zetaY);
    if ( zetaR > 0. ) {
      zetaX /= zetaR;
      zetaY /= zetaR;
    }

    double visPx = leg1.px() + leg2.px();
    double visPy = leg1.py() + leg2.py();
    //double pZetaVis = visPx*zetaX + visPy*zetaY;
    *pZV = visPx*zetaX + visPy*zetaY;
    
    double px = visPx + metPx;
    double py = visPy + metPy;
    //double pZeta = px*zetaX + py*zetaY;
    *pZ = px*zetaX + py*zetaY;
  }


  void runSVFit(){
    svfitmass_=0.;
    //check covariance matrix:
    float det=((*(metSig_->significance()))[0][0])*((*(metSig_->significance()))[1][1]) - ((*(metSig_->significance()))[1][0])*((*(metSig_->significance()))[0][1]);
    if(det<1e-8)return;

    if(runSVFit_==1){  //old svfit  
      NSVfitStandalone2011::Vector measuredMET( metP4_.x(), metP4_.y(), 0);
      std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons;
      NSVfitStandalone2011::LorentzVector p1(taup4_);
      measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kHadDecay,p1));    
      NSVfitStandalone2011::LorentzVector p2(mup4_);
      measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kLepDecay,p2));
      NSVfitStandaloneAlgorithm2011 algo(measuredTauLeptons,measuredMET,metSig_->significance(),0);
      algo.maxObjFunctionCalls(5000);
      algo.fit();
      svfitmass_  = algo.fittedDiTauSystem().mass();
    }else if(runSVFit_==2){    //new svfit
      std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
      measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, taup4_));
      measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, mup4_));
      NSVfitStandaloneAlgorithm algo(measuredTauLeptons, metP4_.Vect(), *(metSig_->significance()), 0);
      algo.addLogM(false);
      algo.integrate();
      svfitmass_ = algo.getMass();
    }else {
      cout<<" Unrecognized SVFit version "<<endl;
      exit(0);
    }

    
    if(smearSVFitMass0pi0_>0.&&taudecaymode_==0) svfitmass_ += gRandom->Gaus(0,smearSVFitMass0pi0_);
    if(smearSVFitMass1pi0_>0.&&taudecaymode_==1) svfitmass_ += gRandom->Gaus(0,smearSVFitMass1pi0_);

  }

  
  //custom electron isolation
  float electronRelIsoDBCorr(const cmg::Electron * cand){
    if(!cand) return 9999.;

    //these are ok in the cmgElectron
    float neutralhad=cand->neutralHadronIso();
    float puchhad=cand->puChargedHadronIso();

    //these two need to be recomputed with proper cone vetos
    float charged=0.;//cand->chargedAllIsoWithConeVeto();
    float photon=cand->photonIso();
    const pat::Electron * input= &(*(*(cand->sourcePtr())));

    //charged particle iso
    reco::isodeposit::AbsVetos allChargedVetoesCollection ;
    float coneSizeCh = 0.01 ;
    if (input->isEE()) coneSizeCh = 0.015 ;
    reco::isodeposit::ConeVeto allChargedVeto (reco::isodeposit::Direction(input->eta (), input->phi ()), coneSizeCh) ;
    allChargedVetoesCollection.push_back (&allChargedVeto) ;
    charged = (input->isoDeposit(pat::PfChargedAllIso)->depositAndCountWithin(0.4,allChargedVetoesCollection,false ).first);
    

    //photon iso
    reco::isodeposit::AbsVetos photonVetoesCollection ;
    float coneSizePhoton = 0.08 ;
    if (input->isEE()) coneSizePhoton = 0.08 ;
    reco::isodeposit::ConeVeto photonVeto (reco::isodeposit::Direction(input->eta (), input->phi ()), coneSizePhoton) ;
    photonVetoesCollection.push_back (&photonVeto) ;    
    photon = input->isoDeposit(pat::PfGammaIso)->depositAndCountWithin(0.4,photonVetoesCollection,false ).first ;
    
    //cout<<" "<<charged<<" "<<photon<<endl;

    if(cand->pt()>0.)
      return (charged + TMath::Max(neutralhad+photon - 0.5*puchhad,0.))/cand->pt();
    else return 9999.;
  }


  bool electronVertexCut(const cmg::Electron * cand){
    if(!((*(cand->sourcePtr()))->gsfTrack().isNonnull()))return 0;
    if(!((*(cand->sourcePtr()))->gsfTrack().isAvailable()))return 0;     
    if(fabs((*(cand->sourcePtr()))->gsfTrack()->dxy(PV_->position())) > 0.045 ) return 0;
    if(fabs((*(cand->sourcePtr()))->gsfTrack()->dz(PV_->position()))  > 0.2 ) return 0;
    
    return 1;
  }

  //electron id WP95
  bool electronIDWP95(const cmg::Electron * cand){
    ///https://twiki.cern.ch/twiki/bin/view/CMS/EgammaCutBasedIdentification#Electron_ID_Working_Points
    if(!cand) return 0;
    if((*(cand->sourcePtr()))->isEB()
       && fabs(cand->deltaEtaSuperClusterTrackAtVtx())<0.007
       && fabs(cand->deltaPhiSuperClusterTrackAtVtx())<0.800
       && cand->sigmaIetaIeta()<0.01
       && cand->hadronicOverEm()<0.15
       ) return 1;
    else if((*(cand->sourcePtr()))->isEE()
	    && fabs(cand->deltaEtaSuperClusterTrackAtVtx())<0.01
	    && fabs(cand->deltaPhiSuperClusterTrackAtVtx())<0.70
	    && cand->sigmaIetaIeta()<0.03
	    )return 1;
    else return 0;
  }

  bool electronMVATight(const cmg::Electron * cand){
     //look here https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification
     float mvaid=cand->mvaNonTrigV0();
     float eta=(*(cand->sourcePtr()))->superCluster()->eta();
     //cout<<eta<<" "<<mvaid<<endl;
     if(fabs(eta)<0.8)
       if(mvaid<0.925)return 0; 
     if(0.8<=fabs(eta)&&fabs(eta)<1.479)
       if(mvaid<0.975)return 0;
     if(1.479<=fabs(eta))
       if(mvaid<0.985)return 0; 
     
     //need to check for different pt ranges

     return 1;
  }
  bool electronMVALoose(const cmg::Electron * cand){
     float mvaid=cand->mvaNonTrigV0();
     float eta=(*(cand->sourcePtr()))->superCluster()->eta();
     if(cand->pt()<20){
       if(fabs(eta)<0.8)                  if(mvaid<0.925)return 0; 
       if(0.8<=fabs(eta)&&fabs(eta)<1.479)if(mvaid<0.915)return 0;
       if(1.479<=fabs(eta))               if(mvaid<0.965)return 0; 
     }
     if(cand->pt()>=20){
       if(fabs(eta)<0.8)                  if(mvaid<0.905)return 0; 
       if(0.8<=fabs(eta)&&fabs(eta)<1.479)if(mvaid<0.955)return 0;
       if(1.479<=fabs(eta))               if(mvaid<0.975)return 0; 
     }

    return 1;
  }

  //Muon Vertex cut
  bool muonVertexCut(const cmg::Muon * cand){
    if(!((*(cand->sourcePtr()))->innerTrack().isNonnull()))return 0;
    if(!((*(cand->sourcePtr()))->innerTrack().isAvailable()))return 0;
    if(fabs((*(cand->sourcePtr()))->innerTrack()->dxy(PV_->position())) > 0.045 ) return 0;
    if(fabs((*(cand->sourcePtr()))->innerTrack()->dz(PV_->position()))  > 0.2 ) return 0;
    return 1;
  }

  //PFMuonTight 
  bool muonIDPFMuonTight(const cmg::Muon * cand){
    if(!(cand->isGlobal()))return 0; 
    if(!((*(cand->sourcePtr()))->userFloat("isPFMuon")>0.5))return 0;
    if(!(cand->normalizedChi2() < 10))return 0;
    if(!(cand->numberOfValidMuonHits() > 0))return 0;
    if(!(cand->numberOfMatchedStations() > 1))return 0; //if(!(cand->numberOfMatches() > 1))return 0;

    if(!((*(cand->sourcePtr()))->innerTrack().isNonnull()))return 0;
    if(!((*(cand->sourcePtr()))->innerTrack().isAvailable()))return 0;
    if(!((*(cand->sourcePtr()))->innerTrack()->hitPattern().numberOfValidPixelHits() > 0))return 0; 

    if(!(cand->trackerLayersWithMeasurement() > 5))return 0;
    return 1;
  }

  // 
  bool tauVertexCut(const cmg::Tau * cand){
    if(fabs(computeDxy(cand->leadChargedHadrVertex(),cand->p4()))>0.045)return 0;
    if(fabs(computeDz(cand->leadChargedHadrVertex(),cand->p4()))>0.2)return 0;
    return 1;
  }

  //
  bool thirdLeptonVeto(){
    int nleptons=0;
    for(std::vector<cmg::Muon>::const_iterator m=leptonVetoListMuon_->begin(); m!=leptonVetoListMuon_->end(); ++m){  
      if(m->pt()<=10.0)continue;
      if(fabs(m->eta())>=2.4)continue;
      if(!muonVertexCut(&(*m)))continue;
      if(!muonIDPFMuonTight(&(*m)))continue;
      if(m->relIso(0.5,1)>=0.3)continue;        
      nleptons++;
    }
    for(std::vector<cmg::Electron>::const_iterator m=leptonVetoListElectron_->begin(); m!=leptonVetoListElectron_->end(); ++m){  
      if(m->pt()<=10.0)continue;
      if(fabs(m->eta())>=2.5)continue;
      if(!electronVertexCut(&(*m)))continue;
      if(m->numberOfHits()!=0) continue;
      //if(m->passConversionVeto()!=1) continue;
      if((*(m->sourcePtr()))->passConversionVeto()!=1)continue;
      if(!electronMVALoose(&(*m)))continue;
      if( electronRelIsoDBCorr( &(*m) )>=0.3 ) continue; 
      nleptons++;
    }
    if(nleptons>=2)return 0;
    return 1;
  }

  ////Debugging functions:
  void printMuonInfo(const cmg::Muon * cand);
  void printElectronInfo(const cmg::Electron * cand);
  void printTauInfo(const cmg::Tau * cand);
  void printMETInfo();


private:



};


#endif
