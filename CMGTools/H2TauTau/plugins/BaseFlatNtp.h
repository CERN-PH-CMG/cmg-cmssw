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


#include "CMGTools/Common/interface/RecoilCorrector.h"

#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"


#include "CMGTools/H2TauTau/interface/BTagEfficiency.h"
#include "CMGTools/H2TauTau/interface/BTagWeight.h"

//#include "TMVA/Reader.h"
#include "CMGTools/H2TauTau/interface/VBFMVA.h"

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
  edm::InputTag pupWeightName_;
  int firstRun_; 
  int lastRun_; 
  int dataType_;//0=MC, 1=Data, 2=EmbeddedData, 3=EmbeddedMC

   //configurable selections
  float muPtCut_;
  float tauPtCut_;
  float muEtaCut_;
  float tauEtaCut_;
  int metType_;
  int runSVFit_;

  //event info
  const edm::Event * iEvent_; 

  edm::Handle< std::vector<reco::Vertex> > vertices_;
  const reco::Vertex * PV_;

  edm::Handle< std::vector<cmg::TriggerObject> > trig_;
  std::vector<edm::InputTag *>  trigPaths_;

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

  edm::InputTag diMuonVetoListTag_;


  edm::InputTag pfJetListTag_;
  std::vector<const cmg::PFJet * > fullJetList_;
  std::vector<const cmg::PFJet * > pfJetList_;
  std::vector<const cmg::PFJet * > pfJetListLC_;
  std::vector<const cmg::PFJet * > pfJetListLepLC_;
  const cmg::PFJet * leadJet_;
  const cmg::PFJet * subleadJet_;

  std::vector<const cmg::PFJet * > pfJetListB_;
  std::vector<const cmg::PFJet * > pfJetListBLC_;
  std::vector<const cmg::PFJet * > pfJetListBTagLC_;
  const cmg::PFJet * leadBJet_;

  const cmg::METSignificance * metSig_;

  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;

  SelectionEfficiency selectionEff_;
  float selectionEffWeight_;

  //HQT weights for 2011 signal samples
  std::string signalWeightDir_;
  std::string signalWeightMass_;
  TH1F* signalWeightHisto_;
  float signalWeight_;


  //for MSSM
  float btagWP_;
  std::vector<const cmg::PFJet * > pfJetListBTagWeight_;
  BTagWeight btagWeight_;
  BTagEfficiency btagEff_;
  float btagEffWeight_;


  //generator variables
  float genbosonmass_;
  float genbosonpt_;
  float genbosoneta_;
  float genbosonphi_;


  //event variables
  float pupWeight_;
  float eventweight_;
  float embeddedGenWeight_;//for tau embedded samples

  int runnumber_;
  int lumiblock_;
  int eventid_;

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
  math::XYZTLorentzVector mup4_;
  

  //tau variables
  float taumass_;
  float taupt_;
  float taueta_;
  float tauphi_;
  int   tautruth_;
  float tauehop_;
  float taueop_;
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
  math::XYZTLorentzVector taup4_;


  //met variables
  float pftransversemass_;
  double pfmetpt_;
  double pfmetphi_;
  float transversemass_;
  double metpt_;//double needed by recoil corrector
  double metphi_;
  float  metsigcov00_;
  float  metsigcov01_;
  float  metsigcov10_;
  float  metsigcov11_;
  float  pZeta_;
  float  pZetaVis_;
  reco::Candidate::PolarLorentzVector metP4_;

  //jet variables
  int njet_;
  int njetLepLC_;
  float leadJetPt_;
  float leadJetEta_;
  float leadJetRawFactor_;
  float subleadJetPt_;
  float subleadJetEta_;
  float subleadJetRawFactor_;
  float diJetMass_;
  float diJetPt_;
  float diJetDeltaEta_;
  float diJetEta1Eta2_;
  int   njetingap_;

  int njet20_;
  int nbjet_;
  float leadBJetBTagProb_;
  float leadBJetPt_;
  float leadBJetEta_;
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
  edm::InputTag mvaMETSigTag_;

  RecoilCorrector corrector_;
  int recoilCorreciton_;
  double recoiliScale_;
  std::string fileZmmData_;
  std::string fileZmmMC_;
  
  void fillMET(){
    
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
      metpt_=diobjectmet_.pt();
      metphi_=diobjectmet_.phi();
      edm::Handle< std::vector<cmg::METSignificance> > metsigVector;
      iEvent_->getByLabel(mvaMETSigTag_,metsigVector); 
      metSig_ = &(metsigVector->at(diobjectindex_));
    }
		  
    if(metType_==3){//Type 1 Corrected MET
      edm::Handle<std::vector< cmg::BaseMET> > pfMET;
      iEvent_->getByLabel(edm::InputTag("cmgPFMET"),pfMET);
      metpt_=pfMET->begin()->pt();
      metphi_=pfMET->begin()->phi();
      
      edm::Handle< cmg::METSignificance > pfMetSigHandle;
      iEvent_->getByLabel(edm::InputTag("pfMetSignificance"),pfMetSigHandle); 
      metSig_ = &(*pfMetSigHandle);
    }
    
    
    if(!metSig_){
      cout<<" Unrecognized metType "<<endl;
      exit(0);
    }
    
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
      
      //smear the met even more
      //metpt_=metpt_*( (randsigma_>0. && njet_>0  ) ? randEngine_.Gaus(1.,randsigma_) : 1.);
    }
    
    
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
    vJet1.SetPtEtaPhi(leadJet_->pt(), leadJet_->eta(), leadJet_->phi());
    vJet2.SetPtEtaPhi(subleadJet_->pt(), subleadJet_->eta(), subleadJet_->phi());
    vDiJet = vJet1 + vJet2;

    Double_t mjj = massPtEtaPhiM(leadJet_->pt(), leadJet_->eta(), leadJet_->phi(), leadJet_->mass(),subleadJet_->pt(), subleadJet_->eta(), subleadJet_->phi(), subleadJet_->mass());
    Double_t dEta = fabs(leadJet_->eta() - subleadJet_->eta());
    Double_t dPhi = deltaPhi(leadJet_->phi(), subleadJet_->phi());
    Double_t dPhi_hj = deltaPhi(vDiTau.Phi(), vDiJet.Phi());

    // Lorenzo's variables
    Double_t C1 = min(fabs(vDiTauVis.Eta() - leadJet_->eta()), fabs(vDiTauVis.Eta() - subleadJet_->eta()));
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

  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  void printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList);
  
  edm::Handle< std::vector<cmg::TriggerObject> > trigObjs_;
  bool trigObjMatch(float eta, float phi, std::string path, std::string filter, int pdgid=-1);

  int truthMatchLeg(float legeta, float legphi);

  void fillPFJetList(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetListLC(float leg1eta, float leg1phi, float leg2eta, float leg2phi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);
  void fillPFJetListLepLC(float lepeta, float lepphi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC);

  void fillPFJetListB(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);
  void fillPFJetListBTag(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list);

  const cmg::PFJet * findJet(std::vector<const cmg::PFJet * > * fulllist, float eta, float phi);

  bool checkPFJetId(const cmg::PFJet * jet){
    //Loose PF Jet id : https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    if(jet->component(5).fraction() < 0.99
       &&jet->component(4).fraction() < 0.99
       &&jet->nConstituents() > 1
       &&(jet->component(1).fraction() > 0 || abs(jet->eta()) < 2.4 )
       &&(jet->component(1).number() > 0 || abs(jet->eta()) < 2.4 )
       &&(jet->component(2).fraction() < 0.99 || abs(jet->eta()) < 2.4)        
       )return 1;
    else return 0;
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
    njet20_=pfJetListBLC_.size();
    nbjet_=pfJetListBTagLC_.size();
    leadBJet_ = 0 ;
    if(nbjet_>0){
      leadBJet_ = pfJetListBTagLC_[0];
      leadBJetPt_ = leadBJet_->pt();
      leadBJetEta_ = leadBJet_->eta();
      leadBJetBTagProb_ = leadBJet_->btag("combinedSecondaryVertexBJetTags");
    }
    
  }

  void fillBTagWeight(){
    btagEffWeight_ = 1.;
    if(dataType_!=0) return;//only done for MC

    vector<vector<BTagWeight::JetInfo> > jetinfovec;
    for (unsigned int i=0; i<pfJetListB_.size(); ++i) {
      //    int index = pfJetListB_[i];
      double jetpt = pfJetListB_[i]->pt(); 
      double jeteta = pfJetListB_[i]->eta(); 
      double jetflavor = TMath::Abs(pfJetListB_[i]->partonFlavour());
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

  }


private:



};


#endif
