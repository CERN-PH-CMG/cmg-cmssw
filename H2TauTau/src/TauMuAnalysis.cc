#include "CMGTools/H2TauTau/interface/TauMuAnalysis.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TLine.h>

#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Run.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm.h"
//#include "CMGTools/H2TauTau/interface/NSVfitStandaloneAlgorithm.h"

#include <TVectorD.h>

TauMuAnalysis::TauMuAnalysis():
  BaseAnalysis(),
  inputTag_(""),
  calcsvfit_(0),
  makeAllHistos_(1),
  QCDOStoSSRatio_(0),
  recoilCorr_(0)
{
}

TauMuAnalysis::TauMuAnalysis(const char * name):
  BaseAnalysis(name),
  inputTag_("cmgTauMuSelClean"),
  calcsvfit_(0),
  makeAllHistos_(1),
  QCDOStoSSRatio_(0),
  recoilCorr_(0)
{
 
}

TauMuAnalysis::~TauMuAnalysis(){
}


bool TauMuAnalysis::init(){


  if(!BaseAnalysis::init())return 0;

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){    
    cout<<"TauMuAnalysis  Initialized sample "<<samples_[i]->GetName()<<" "<<samples_[i]->GetTitle()<<endl;
  }

  //print out the trigger eff:
  //cout<<"trigger efficiencies"<<endl;
  //for(Int_t p=0;p<100;p+=10)
  //cout<<p<<" "<<triggerEff_.ratio(0,(double)p)<<" "<<triggerEff_.ratio(10,(double)p)<<" "<<triggerEff_.ratio(15,(double)p)<<endl;

  
  return 1;
}

bool TauMuAnalysis::addHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::addHistos(s))return 0;
  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauNHisto","",5,-.5,4.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassHisto","",400,0,400))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassSVFitHisto","",800,0,800))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_svFitConvergeHisto","",6,-0.5,5.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_svFitCov00Histo","",1000,0,1000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_svFitEigen0Histo","",1000,0,1000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_svFitEigen1Histo","",1000,0,1000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauEtaHisto","",120,-6,6))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauPtHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muPtHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muIsoHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDxyHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_muDzHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauPtHisto","",200,0,200))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauIsoHisto","",100,0,5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDxyHisto","",100,0,.5))))return 0; 
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_tauDzHisto","",100,0,.5))))return 0;  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_pZetaHisto","",180,-140,40))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_transverseMassHisto","",200,0,200))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_njetHisto","",10,-0.5,9.5))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_metphiHisto","",64,-3.2,3.2))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassBoostedHisto","",400,0,400))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_jetPtBoostedHisto","",500,0,500))))return 0;
  
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassVBFHisto","",400,0,400))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diJetMassVBFHisto","",1000,0,1000))))return 0;    
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diJetDeltaEtaVBFHisto","",100,0,10))))return 0;

  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassInclusiveHisto","",400,0,400))))return 0;



  return 1;
}


bool TauMuAnalysis::getHistos(TString tag){
  if(!sample_) return 0;

  if(tag!="")tag=TString("_")+tag;

  if(!(diTauNHisto_=(TH1F*)(sample_->getHisto(TString("diTauNHisto")+tag))))return 0;
  if(!(diTauMassHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassHisto")+tag))))return 0;
  if(!(diTauMassSVFitHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassSVFitHisto")+tag))))return 0;
  if(!(svFitConvergeHisto_=(TH1F*)(sample_->getHisto(TString("svFitConvergeHisto")+tag))))return 0;
  if(!(svFitCov00Histo_=(TH1F*)(sample_->getHisto(TString("svFitCov00Histo")+tag))))return 0;
  if(!(svFitEigen0Histo_=(TH1F*)(sample_->getHisto(TString("svFitEigen0Histo")+tag))))return 0;
  if(!(svFitEigen1Histo_=(TH1F*)(sample_->getHisto(TString("svFitEigen1Histo")+tag))))return 0;
  if(!(diTauEtaHisto_=(TH1F*)(sample_->getHisto(TString("diTauEtaHisto")+tag))))return 0;
  if(!(diTauPtHisto_=(TH1F*)(sample_->getHisto(TString("diTauPtHisto")+tag))))return 0;
  if(!(muPtHisto_=(TH1F*)(sample_->getHisto(TString("muPtHisto")+tag))))return 0;
  if(!(muIsoHisto_=(TH1F*)(sample_->getHisto(TString("muIsoHisto")+tag))))return 0;
  if(!(muPtHisto_=(TH1F*)(sample_->getHisto(TString("muPtHisto")+tag))))return 0;
  if(!(muDxyHisto_=(TH1F*)(sample_->getHisto(TString("muDxyHisto")+tag))))return 0;
  if(!(muDzHisto_=(TH1F*)(sample_->getHisto(TString("muDzHisto")+tag))))return 0;
  if(!(tauPtHisto_=(TH1F*)(sample_->getHisto(TString("tauPtHisto")+tag))))return 0;
  if(!(tauIsoHisto_=(TH1F*)(sample_->getHisto(TString("tauIsoHisto")+tag))))return 0;
  if(!(tauDxyHisto_=(TH1F*)(sample_->getHisto(TString("tauDxyHisto")+tag))))return 0;
  if(!(tauDzHisto_=(TH1F*)(sample_->getHisto(TString("tauDzHisto")+tag))))return 0;
  if(!(metHisto_=(TH1F*)(sample_->getHisto(TString("metHisto")+tag))))return 0;
  if(!(metphiHisto_=(TH1F*)(sample_->getHisto(TString("metphiHisto")+tag))))return 0;
  if(!(pZetaHisto_=(TH1F*)(sample_->getHisto(TString("pZetaHisto")+tag))))return 0;
  if(!(transverseMassHisto_=(TH1F*)(sample_->getHisto(TString("transverseMassHisto")+tag))))return 0;
  if(!(njetHisto_=(TH1F*)(sample_->getHisto(TString("njetHisto")+tag))))return 0;

  if(!(diTauMassBoostedHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassBoostedHisto")+tag))))return 0;
  if(!(jetPtBoostedHisto_=(TH1F*)(sample_->getHisto(TString("jetPtBoostedHisto")+tag))))return 0;
  
  if(!(diTauMassVBFHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassVBFHisto")+tag))))return 0;
  if(!(diJetMassVBFHisto_=(TH1F*)(sample_->getHisto(TString("diJetMassVBFHisto")+tag))))return 0;
  if(!(diJetDeltaEtaVBFHisto_=(TH1F*)(sample_->getHisto(TString("diJetDeltaEtaVBFHisto")+tag))))return 0;

  if(!(diTauMassInclusiveHisto_=(TH1F*)(sample_->getHisto(TString("diTauMassInclusiveHisto")+tag))))return 0;

  return 1;
}


void TauMuAnalysis::fillPFJetListLC(const cmg::TauMu * cand){
  pfJetListLC_.clear();

  //lepton clean the Jets
  std::vector<cmg::PFJet> jetlist;
  for(std::vector<cmg::PFJet>::const_iterator jet=pfJetList_.begin(); jet!=pfJetList_.end(); ++jet){
    if(reco::deltaR(jet->eta(),jet->phi(),cand->leg1().eta(),cand->leg1().phi())<0.5) continue;     
    if(reco::deltaR(jet->eta(),jet->phi(),cand->leg2().eta(),cand->leg2().phi())<0.5) continue;         
    jetlist.push_back(*jet);
  }

  //order by pt
  const cmg::PFJet * lastjet=NULL;
  for(std::vector<cmg::PFJet>::const_iterator jet1=jetlist.begin(); jet1!=jetlist.end(); ++jet1){
    const cmg::PFJet * highjet=NULL;
    for(std::vector<cmg::PFJet>::const_iterator jet2=jetlist.begin(); jet2!=jetlist.end(); ++jet2){
      if(lastjet){
	if(jet2->pt()<lastjet->pt()){
	  if(highjet){
	    if(jet2->pt()>highjet->pt())highjet=&(*jet2);
	  }else highjet=&(*jet2);
	}
      }else{
	if(highjet){
	  if(jet2->pt()>highjet->pt())highjet=&(*jet2);
	}else highjet=&(*jet2);
      }    
    }  
    pfJetListLC_.push_back(*highjet);
    lastjet=highjet;
  }
  
}

void TauMuAnalysis::applyRecoilCorr(const cmg::TauMu * cand, TVector3 * MET){
  if(!(sample_->getApplyRecoilCorr()
       && genBoson_ 
       && (genEventType_==1 || genEventType_==3 || genEventType_==5
	   || genEventType_==11 || genEventType_==13 || genEventType_==15))) return;

  TVector3 tau1PT=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),0.);
  TVector3 tau2PT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
  TVector3 genBosonPT=TVector3(genBoson_->p4().x(),genBoson_->p4().y(),0.);
  double met = MET->Mag();
  double metphi = MET->Phi();    
  TVector3 recoLeptonPT = tau2PT; //use only the muon for WJets
  if(genEventType_==1 || genEventType_==3 || genEventType_==5) recoLeptonPT += tau1PT; 
  double pU1      = 0;  //--
  double pU2      = 0;  //--
  recoilCorr_->CorrectType1(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi(),pU1,pU2,0.,pfJetListLC_.size());
  //recoilCorr_->CorrectType2(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi(),pU1,pU2,0.,pfJetListLC_.size());
  //recoilCorr_->CorrectAll(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi(),pU1,pU2,0.,pfJetListLC_.size());
  MET->SetMagThetaPhi(met,TMath::Pi()/2.,metphi);
}



float TauMuAnalysis::computePZeta(const cmg::TauMu * cand){
  if(!cand || !met_) return 0.;
  //1) zeta axis is the bisector between tau1 and tau2 momentum vectors
  //2) project visible energy onto zeta axis
  //3) project MET onto zeta axis
  TVector3 tau1PT=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),0.);
  TVector3 tau2PT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
  TVector3 metPT=TVector3(met_->p4().x(),met_->p4().y(),0.);
  applyRecoilCorr(cand,&metPT);

  TVector3 zetaAxis=(tau1PT.Unit() + tau2PT.Unit()).Unit();
  Float_t pZetaVis=(tau1PT + tau2PT)*zetaAxis;
  Float_t pZetaMET=metPT*zetaAxis;
  Float_t pZeta=pZetaVis+pZetaMET;
  
  return pZeta-1.5*pZetaVis;
}

float TauMuAnalysis::computeTransverseMass(const cmg::TauMu * cand){
  if(!cand || !met_) return 0.;

  TVector3 muonPT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
  TVector3 metPT=TVector3(met_->p4().x(),met_->p4().y(),0.);
  applyRecoilCorr(cand,&metPT);

  //return sqrt(2.0*muonPT.Mag()*metPT.Mag()*(1-cos(metPT.Phi()-muonPT.Phi())));
  return (TLorentzVector(muonPT,muonPT.Mag()) + TLorentzVector(metPT,metPT.Mag())).Mag();
}

float TauMuAnalysis::computeTauIso(const cmg::Tau * tau){
  return (tau->trackIso()+std::max( (tau->gammaIso() - tau->userData("rho")*TMath::Pi()*0.5*0.5), 0.0)); 
}

bool TauMuAnalysis::computeDiLeptonVeto(const cmg::Muon * muon){

  for(std::vector<cmg::Muon>::const_iterator m=diLeptonVetoList_->begin(); m!=diLeptonVetoList_->end(); ++m){  
    if(m->charge()==muon->charge())continue;//note this also makes sure it is not the same muon already used
    if(m->pt()<15.0)continue;
    if(fabs(m->eta())>2.1)continue;
    if(m->relIso(0.5)>0.3)continue;
    return 1;
  }
  
  return 0;
}


bool TauMuAnalysis::fillVariables(const fwlite::Event * event){

  //event level variables
  if(!BaseAnalysis::fillVariables(event))return 0;

  ///check trigger
  trigpass_=0;
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  event->getByLabel(edm::InputTag("cmgTriggerObjectSel","","PAT"),trig);
  if(sample_->getTrigPaths()->size()==0)trigpass_=1;//no trigger requirement
  for(std::vector<std::string>::const_iterator path=sample_->getTrigPaths()->begin(); path!=sample_->getTrigPaths()->end(); path++){
    //trig->begin()->printSelections(cout);
    if(trig->begin()->hasSelection(path->c_str()))
      if(trig->begin()->getSelection(path->c_str())){
	trigpass_=1;
      }
  }
  
  ///get the MET 
  edm::Handle< std::vector<cmg::BaseMET> > met;
  event->getByLabel(edm::InputTag("cmgPFMET"),met);
  met_=&(*met->begin());
  
  edm::Handle< cmg::METSignificance > metsig;
  event->getByLabel(edm::InputTag("PFMETSignificanceAK5"),metsig);  
  metsig_=metsig->significance();

  ///get the TauMu cands 
  event->getByLabel(edm::InputTag(inputTag_.c_str()),diTauList_);

  //get the muons for the di-lepton veto
  event->getByLabel(edm::InputTag("cmgMuonSel"),diLeptonVetoList_);
  
  ///get the gen Boson and set the genEventType
  genBoson_ = NULL;
  genBosonL1_ = NULL;
  genBosonL2_ = NULL;
  genEventType_=0;
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS"){  
    event->getByLabel(edm::InputTag("genParticlesStatus3"),genParticles_);    
    for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
      //cout<<g->pdgId()<<" "<<g->p4().pt()<<endl;
      if((abs(g->pdgId())==23 || abs(g->pdgId())==24) && genBoson_==NULL )
	genBoson_=&(*g);
    }
    //if(genBoson_)cout<<"genBoson_ ref = "<<genBoson_<<endl;

    if(genBoson_){      
      //determine type of generated event: Z-->ee,mumu,tautau,, W-->e nu, mu nu, tau mu
      int genTaus=0;
      int genMuons=0;
      int genElectrons=0;
      for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
	if(abs(g->pdgId())==11 && g->mother()==genBoson_) genElectrons++;
	if(abs(g->pdgId())==13 && g->mother()==genBoson_) genMuons++;
	if(abs(g->pdgId())==15 && g->mother()==genBoson_) genTaus++;
      }
      if(abs(genBoson_->pdgId())==23 && genElectrons==2)   genEventType_=1;
      if(abs(genBoson_->pdgId())==23 && genMuons==2)       genEventType_=3;
      if(abs(genBoson_->pdgId())==23 && genTaus==2)        genEventType_=5;
      if(abs(genBoson_->pdgId())==24 && genElectrons==1)   genEventType_=11;
      if(abs(genBoson_->pdgId())==24 && genMuons==1)       genEventType_=13;
      if(abs(genBoson_->pdgId())==24 && genTaus==1)        genEventType_=15;

      //get the leptons from the genBoson
      for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
	if((g->pdgId()==11 || g->pdgId()==13 || g->pdgId()==15 ) && g->mother()==genBoson_) genBosonL1_=&(*g);
	if((g->pdgId()==-11 || g->pdgId()==-13 || g->pdgId()==-15 ) && g->mother()==genBoson_) genBosonL2_=&(*g);
      }      

    }

  }


  //fill the jet list with basic selections, lepton cleaning done later for each TauMu candidate
  pfJetList_.clear();
  edm::Handle< std::vector<cmg::PFJet> > jetlist;
  event->getByLabel(edm::InputTag("cmgPFJetSel"),jetlist);
  for(std::vector<cmg::PFJet>::const_iterator jet=jetlist->begin(); jet!=jetlist->end(); ++jet){
    if(jet->pt()<30)continue;  
    if(fabs(jet->eta())>4.5)continue;        
    pfJetList_.push_back(*jet);
  }
  

  
  return 1;
}

bool TauMuAnalysis::applySelections(TString exceptcut){

  //if none are selected returns 0
  diTauSel_=NULL;

  ///check the trigger
  if(!trigpass_) return 0;
  
  //separate the Z->ll at gen level here, will speed up processing
  //cand be done because a reconstruced tau-tau candidate can only be truth-matched if there is actually a generated Z-->tau tau, for example
  if(sample_->getTruthEventType()==1 || sample_->getTruthEventType()==3 || sample_->getTruthEventType()==5
     || sample_->getTruthEventType()==11 || sample_->getTruthEventType()==13 || sample_->getTruthEventType()==15)
    if(sample_->getTruthEventType()!=genEventType_) return 0;
  

  //select TauMu cands
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){    
    
    ///SS or OS separation 
    if(sample_->getDataType()=="MC_SS" || sample_->getDataType()=="Data_SS")
      if(fabs(cand->charge())!=2.)continue;
    if(sample_->getDataType()=="MC" || sample_->getDataType()=="Data")
      if(cand->charge()!=0.)continue;

    ////selections on the tau
    //if(exceptcut!="taupt") if(cand->leg1().pt()<20.0)continue; //applied in preseleciton
    if(exceptcut!="tauagainstmuon") if(cand->leg1().tauID("againstMuonTight")!=1) continue;
    if(exceptcut!="tauiso") if(cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr")!=1.) continue; 
    //if(exceptcut!="tauiso") if(computeTauIso(&(cand->leg1()))> 2.0) continue; 
    

    ////selections on the muon
    //if(exceptcut!="mupt") if(cand->leg2().pt()<15.0)continue;//applied in preseleciton
    //if(exceptcut!="mudxy") if(cand->leg2().dxy()>0.045)continue;    
    //if(exceptcut!="mudz") if(cand->leg2().dz()>0.2)continue;        
    if(exceptcut!="muiso") if(cand->leg2().relIso()>0.1)continue;    



    ////other 
    if(exceptcut!="dileptonveto")if(computeDiLeptonVeto(&(cand->leg2()))) continue;
    //if(exceptcut!="pzeta") if(computePZeta(&(*cand))<-20.0) continue;


    //transverse mass uses recoil corrections which use number of jets
    fillPFJetListLC(&(*cand));
    if(exceptcut!="massT") if(computeTransverseMass(&(*cand))>40.0) continue;


    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;
  
  //choose the best candidate
  diTauSel_=&(*diTauSelList_.begin());
  double highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand)
    if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
      diTauSel_=&(*cand);
      highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
    }
  fillPFJetListLC(diTauSel_);

  //truth match the reconstructed candidates:
  truthEventType_=0;
  if(genBoson_){
    //Z-->ll
    if(abs(genBoson_->pdgId())==23){
      if(genBosonL1_&&genBosonL2_){
	if(((reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL1_->eta(),genBosonL1_->phi())<0.2)
	    ||(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL2_->eta(),genBosonL2_->phi())<0.2))
	   && ((reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi())<0.2)
	       ||(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi())<0.2))
	   ) truthEventType_=abs(genBosonL1_->pdgId())-10;
	else truthEventType_=6;
      }else truthEventType_=6;
    }

    //W-->lnu
    if(abs(genBoson_->pdgId())==24){
      if(genBosonL1_){
	if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi())<0.2)
	  truthEventType_=abs(genBosonL1_->pdgId());
	else truthEventType_=16;
      }else if(genBosonL2_){	  
	if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi())<0.2)
	  truthEventType_=abs(genBosonL2_->pdgId());
	else truthEventType_=16;
      }else truthEventType_=16;
    }
  }


  //separate DYToLL here
  if(sample_->getTruthEventType()>0)
    if(sample_->getTruthEventType()!=truthEventType_) return 0;


  //SM event category:
  eventCategorySM_=9;//inclusive
  if(pfJetListLC_.size()==1){//Boosted: 1 jet with pt>150 and no other jets
    if(pfJetListLC_.begin()->pt()>150.0){
      eventCategorySM_=1;
      boostedJet_=&(*(pfJetListLC_.begin()));
    }
  }
  if(pfJetListLC_.size()>=2){//VBF: 1 dijet with m>400, |eta1-eta2| < 4 and no other jet in between
    for(std::vector<cmg::PFJet>::const_iterator jet1=pfJetListLC_.begin(); jet1!=pfJetListLC_.end(); ++jet1){
      for(std::vector<cmg::PFJet>::const_iterator jet2=pfJetListLC_.begin(); jet2!=pfJetListLC_.end(); ++jet2){
	if((jet1->p4()+jet2->p4()).mass() > 400.0 
	   && fabs(jet1->eta() - jet2->eta()) > 4.0 
	   && jet1->eta()*jet2->eta() < 0.0
	   ) {
 	  bool pass=1;
	  if(pfJetListLC_.size()>2){// check there is no additional jet in the eta gap between the VBF jets
	    for(std::vector<cmg::PFJet>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	      if(jet1->eta()<jet2->eta()) 
		if(jet1->eta()<jet3->eta()&&jet3->eta()<jet2->eta()) pass=0;
	      if(jet1->eta()>jet2->eta()) 
		if(jet2->eta()<jet3->eta()&&jet3->eta()<jet1->eta()) pass=0;
	    }
	  }
	  if(pass){
	    eventCategorySM_=2;
	    VBFJet1_=&(*(jet1));
	    VBFJet2_=&(*(jet2));
	  }
	}
      }
    }
  }


  return 1;
}

bool TauMuAnalysis::fillHistos(TString tag, double weight ){
  
  if(!BaseAnalysis::fillHistos(tag,weight)) return 0;

  if(!TauMuAnalysis::getHistos(tag)) return 0;

  //fill the diTau histograms
  diTauNHisto_->Fill(diTauSelList_.size(),weight);
  
  //apply tau fake rate reweighting
  if(sample_->getApplyTauRateWeight()){
    float w=tauRate_.getScale(TauRate::Inclusive,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
    if(diTauSel_->leg1().decayMode()==0)w=tauRate_.getScale(TauRate::OneProng,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
    if(diTauSel_->leg1().decayMode()==1)w=tauRate_.getScale(TauRate::OneProngEM,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
    if(diTauSel_->leg1().decayMode()==2)w=tauRate_.getScale(TauRate::OneProngEM,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
    if(diTauSel_->leg1().decayMode()==10)w=tauRate_.getScale(TauRate::ThreeProng,diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
    weight = weight*w;
  }
  
  tree_ditaumass_=diTauSel_->mass();
  tree_taupt_=diTauSel_->leg1().pt();
  tree_mupt_=diTauSel_->leg2().pt();
  tree_transversemass_=computeTransverseMass(diTauSel_);

  ///
  diTauMassHisto_->Fill(diTauSel_->mass(),weight); 
  diTauEtaHisto_->Fill(diTauSel_->eta(),weight);
  diTauPtHisto_->Fill(diTauSel_->eta(),weight);  

  tauPtHisto_->Fill(tree_taupt_,weight); 
  tauDxyHisto_->Fill(diTauSel_->leg1().dxy(),weight); 
  tauDzHisto_->Fill(diTauSel_->leg1().dz(),weight);

  muPtHisto_->Fill(tree_mupt_,weight);
  muIsoHisto_->Fill(diTauSel_->leg2().relIso(),weight);
  muDxyHisto_->Fill(diTauSel_->leg2().dxy(),weight);
  muDzHisto_->Fill(diTauSel_->leg2().dz(),weight);

  pZetaHisto_->Fill(computePZeta(diTauSel_),weight);  
  transverseMassHisto_->Fill(tree_transversemass_,weight);  

  ///Correct the MET here
  TVector3 metCorr(met_->p4().x(),met_->p4().y(),0.);
  applyRecoilCorr(diTauSel_,&metCorr);
  tree_met_=metCorr.Mag();
    
  ///
  metHisto_->Fill(tree_met_,weight);  
  metphiHisto_->Fill(metCorr.Phi(),weight);  

  ///SVFit
  if(calcsvfit_ && tag==""){
    if(verbosity_>1) cout<<"Calculating SVFit mass"<<endl;
    NSVfitStandalone::Vector measuredMET(metCorr.x(),metCorr.y(),0);
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    NSVfitStandalone::LorentzVector p1(diTauSel_->leg1().p4());
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay,p1));    
    NSVfitStandalone::LorentzVector p2(diTauSel_->leg2().p4());
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay,p2));

    NSVfitStandaloneAlgorithm algo(measuredTauLeptons,measuredMET,metsig_,verbosity_);
    algo.maxObjFunctionCalls(5000);
    algo.fit();
    
    tree_svfitstatus_=algo.fitStatus();
    tree_svfitmass_=algo.fittedDiTauSystem().mass();
    tree_svfitedm_=algo.edm();

    diTauMassSVFitHisto_->Fill(tree_svfitmass_,weight);        
    svFitCov00Histo_->Fill((*metsig_)[0][0],weight);
    svFitConvergeHisto_->Fill(tree_svfitstatus_,weight);

    //study the matrix elements
    TVectorD eigenValues(2);
    TMatrixD eigenVectors(2,2);
    eigenVectors=metsig_->EigenVectors(eigenValues);
    tree_svfiteigenval0_=eigenValues[0];  
    tree_svfiteigenval1_=eigenValues[1];
    svFitEigen0Histo_->Fill(tree_svfiteigenval0_,weight);
    svFitEigen1Histo_->Fill(tree_svfiteigenval1_,weight);
  }
 

  
  //jets
  njetHisto_->Fill(pfJetListLC_.size(),weight);


  //category
  if(eventCategorySM_==1){
    diTauMassBoostedHisto_->Fill(diTauSel_->mass(),weight);
    jetPtBoostedHisto_->Fill(boostedJet_->pt(),weight);
  }
  if(eventCategorySM_==2){
    diTauMassVBFHisto_->Fill(diTauSel_->mass(),weight);
    diJetMassVBFHisto_->Fill((VBFJet1_->p4()+VBFJet2_->p4()).mass(),weight);
    diJetDeltaEtaVBFHisto_->Fill(fabs(VBFJet1_->eta()-VBFJet2_->eta()),weight);
  }
  if(eventCategorySM_==9)diTauMassInclusiveHisto_->Fill(diTauSel_->mass(),weight);



  return 1;
}


bool TauMuAnalysis::createHistos(TString samplename){

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++)
    if(samples_[i]->GetName()==samplename) sample_=samples_[i];
  if(!sample_){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}

  if(!addHistos(sample_))return 0;
  
  if(makeAllHistos_){
    sample_->cloneHistos("tauiso");
    sample_->cloneHistos("tauagainstmuon");
    sample_->cloneHistos("muiso");
    //sample_->cloneHistos("pzeta");
    sample_->cloneHistos("massT");
    sample_->cloneHistos("dileptonveto");
  }


  //create the flat tree
  tree_=new TTree(TString(sample_->GetName())+"_tree",TString(sample_->GetName())+"_tree",0);
  tree_->Branch("ditaumass",&tree_ditaumass_,"ditaumass/F");
  tree_->Branch("taupt",&tree_taupt_,"taupt/F");
  tree_->Branch("mupt",&tree_mupt_,"mupt/F");
  tree_->Branch("met",&tree_met_,"met/F");
  tree_->Branch("transversemass",&tree_transversemass_,"transversemass/F");
  tree_->Branch("svfitstatus",&tree_svfitstatus_,"svfitstatus/I");
  tree_->Branch("svfitmass",&tree_svfitmass_,"svfitmass/F");
  tree_->Branch("svfitedm",&tree_svfitedm_,"svfitedm/F");
  tree_->Branch("svfiteigenval0",&tree_svfiteigenval0_,"svfiteigenval0/F");
  tree_->Branch("svfiteigenval1",&tree_svfiteigenval1_,"svfiteigenval1/F");
  sample_->addTree(tree_);


  if(!sample_->getEvents()){
    cout<<" No fwlit::ChainEvent found in sample "<<sample_->GetName()<<endl;
    return 0;
  }



  ///init recoil corrector
  recoilCorr_=NULL;
  RecoilCorrector recoilCorr((const char*)(sample_->getRecoilCorrProcessFile()));
  if(sample_->getApplyRecoilCorr()){
    recoilCorr.addMCFile("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfit_zmm42X_njet.root");
    recoilCorr.addDataFile("/afs/cern.ch/user/b/benitezj/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/H2TauTau/data/recoilfit_datamm_njet.root");
    recoilCorr_=&recoilCorr;
  }
  ////use with correctAll
  //with datamm in constructor and not adding any other files correctAll over corrects 
  //with zmm in constructor and adding datamm DataFile and zmm MCFile over corrects a bit


  fwlite::ChainEvent chain=*(sample_->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file


  Int_t ievt=0;
  Int_t goodevts=0;
  for(chain.toBegin(); !chain.atEnd() && ievt < truncateEvents_; ++chain, ++ievt){
    const fwlite::Event * event = chain.event();        

    if(!fillVariables(event)) return 0;

    if(ievt%printFreq_==0)cout<<sample_->GetName()<<" "<<ievt<<" events done , run = "<<runnumber_<<" , lumiblock = "<<lumiblock_<<" , eventid = "<<eventid_<<endl;    
    
    //event level selections
    if(!BaseAnalysis::applySelections()) continue;
    
    //fill (N-1) cut histograms here
    if(makeAllHistos_){
      //if(applySelections("tauiso")) if(!fillHistos("tauiso",mcPUPWeight_))return 0;
      //if(applySelections("tauagainstmuon")) if(!fillHistos("tauagainstmuon",mcPUPWeight_))return 0;
      //if(applySelections("muiso")) if(!fillHistos("muiso",mcPUPWeight_))return 0;
      //if(applySelections("dileptonveto")) if(!fillHistos("dileptonveto",mcPUPWeight_))return 0;
      if(applySelections("massT")) if(!fillHistos("massT",mcPUPWeight_))return 0;   
    }
    //fill fully selected histograms 
    if(applySelections()){           
      if(!fillHistos("",mcPUPWeight_))return 0;
      tree_->Fill();
      goodevts++;//only for fully selected events
    }

  }
  cout<<goodevts<<" events passed selections"<<endl;
  
  //
  if(!sample_->save())return 0;
  
  
  return 1;
}


bool TauMuAnalysis::scaleHistos(){
  
  cout<<" List of Data samples: "<<endl;
  float totalDataLumi=0.;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      totalDataLumi+=(*s)->getLumi();
      cout<<(*s)->GetName()<<endl;  
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      cout<<(*s)->GetName()<<endl;  
    }
  }

  cout<<"Total Data lumi = "<<totalDataLumi<<endl;

  cout<<" List of MC samples: "<<endl;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){
      (*s)->scale((*s)->getEffCorrFactor());
      (*s)->scale(totalDataLumi/(*s)->getLumi());
      cout<<(*s)->GetName()<<endl; 
    }
  }
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC_SS"){
      (*s)->scale((*s)->getEffCorrFactor());
      (*s)->scale(totalDataLumi/(*s)->getLumi());
      cout<<(*s)->GetName()<<endl;  
    }
  }


  Float_t WJetsBelowCorr=0.94;//default values
  Float_t WJetsSSBelowCorr=1.11;

  ///////////////////////////////
  /////Determine correction factor for WJets from massT > 50, assuming binning ,200,0,200)))
  //////////////////////////////
  cout<<"Determine WJetsSS from transverse mass > 50, assuming binning ,200,0,200"<<endl;
  Float_t WJetsSSBelow=0; Float_t MCSSBelow=0; Float_t DataSSBelow=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS")
      DataSSBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);  
    else if(TString((*s)->GetName())=="WJetsToLNu_SS")
      WJetsSSBelow=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);    
    else if((*s)->getDataType()=="MC_SS") MCSSBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);
  }
  WJetsSSBelowCorr=(DataSSBelow-MCSSBelow)/WJetsSSBelow;
 
  Float_t WJetsBelow=0; Float_t MCBelow=0; Float_t DataBelow=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data")
      DataBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200); 
    else if((*s)->getDataType()=="MC"){
      if(TString((*s)->GetName())=="WJetsToLNu") WJetsBelow=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);    
      else MCBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);
    }
  }
  WJetsBelowCorr=(DataBelow-MCBelow)/WJetsBelow;


  cout<<"WJetsSS Correction = "<<WJetsSSBelowCorr<<endl;
  cout<<"WJetsOS Correction = "<<WJetsBelowCorr<<endl;
  /////////////////////////////////////////
  ////////Rescale the WJets 
  //////////////////////////////////////////
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu_SS"){
      (*s)->scale(WJetsSSBelowCorr);
    }
    if(TString((*s)->GetName())=="WJetsToLNu"){
      (*s)->scale(WJetsBelowCorr);
    }
  }


  return 1;
}


TH1F* TauMuAnalysis::getQCD(TString histoname){

  cout<<" Calculating QCD: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* hQCD=new TH1F("hQCD","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      hQCD->Add((*s)->getHistoFromFile(histoname));  
      hQCD->SetLineColor((*s)->getColor());
      hQCD->SetFillColor((*s)->getColor());
    }
    if((*s)->getDataType()=="MC_SS"){
      hQCD->Add((*s)->getHistoFromFile(histoname),-1.);
    }
  }
  
  cout<<"QCD before OS/SS rescaling = "<<hQCD->Integral()<<endl;
  hQCD->Scale(QCDOStoSSRatio_);
  cout<<"QCD after OS/SS rescaling = "<<hQCD->Integral()<<endl;
  /////////////////////////////////////
 
  return hQCD;
}


TH1F* TauMuAnalysis::getData(TString histoname){

  cout<<" Calculating Total Data: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* hData=new TH1F("hData","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      hData->Add((*s)->getHistoFromFile(histoname));  
    }
  }
  
 
  return hData;
}


TH1F* TauMuAnalysis::getDataSS(TString histoname){

  cout<<" Calculating Total Data: "<<endl;
  TH1F* href=(TH1F*)((*(samples_.begin()))->getHistoFromFile(histoname));
  if(!href){cout<<" histoname not found in first sample"<<endl; return 0;}

  TH1F* hData_SS=new TH1F("hData_SS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      hData_SS->Add((*s)->getHistoFromFile(histoname));  
    }
  }
  
 
  return hData_SS;
}



bool TauMuAnalysis::plot(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){

  
  if(!scaleHistos())return 0;

  //rebin and set colors on the histos
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Rebin(rebin); 
    }
  }

  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Rebin(rebin); 
    }
  }


  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->SetFillColor((*s)->getColor());
      hmass->SetLineColor((*s)->getLineColor());
      hmass->SetLineStyle((*s)->getLineStyle());
      hmass->SetLineWidth(1);
      hmass->Rebin(rebin);
    }
  }
  
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->SetFillColor((*s)->getColor());
      hmass->SetLineColor((*s)->getLineColor());
      hmass->SetLineStyle((*s)->getLineStyle());
      hmass->SetLineWidth(1);
      hmass->Rebin(rebin);
    }
  }



     
  /////
  TH1F* hQCD=getQCD(histoname);
  if(!hQCD){cout<<" QCD not determined "<<endl; return 0;}
  

  cout<<" Sorting the MC samples by plot order"<<endl;
  /////////////////////////
  ////sort the samples accordin to plot order
  //////////////////////////
  std::vector<Sample*> orderedMCSamples;
  unsigned int orderindex=1;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC"){      
      for(std::vector<Sample*>::const_iterator sj=samples_.begin(); sj!=samples_.end(); ++sj){
	if((*sj)->getDataType()=="MC"){
	  if((*sj)->getPlotOrder()==orderindex){
	    cout<<orderindex<<" "<<(*sj)->GetName()<<endl;
	    orderedMCSamples.push_back((*sj));
	    orderindex++;		  
	  }
	}
      }
    }
  }


  ////
  TH1F* hData=getData(histoname);
  if(!hData){cout<<" Total Data not determined "<<endl; return 0;}
  cout<<"Data "<<(int)(hData->Integral())<<endl;


  ////////////////////////
  cout<<"Creating MCStack:"<<endl;

  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(hData,"Data","p");

  THStack hMCStack("hMCStack","MC");//dont't set any of the regular histogram properties on the THStack will crash.
  hMCStack.Add(hQCD,"hist");
  legend.AddEntry(hQCD,"QCD","f");
  Int_t MCTotalYield=hQCD->Integral();
  cout<<hQCD->Integral()<<"  QCD"<<endl;
  for(std::vector<Sample*>::const_iterator s=orderedMCSamples.begin(); s!=orderedMCSamples.end(); ++s){
    TH1F* h=(TH1F*)((*s)->getHistoFromFile(histoname));    
    if(h->Integral()>0.01*hData->Integral()){
      hMCStack.Add(h,"hist");
      legend.AddEntry(h,(*s)->getPlotLabel(),(*s)->getLegendOption());
    }
    Int_t yield=(int)(h->Integral());
    MCTotalYield+=yield;
    cout<<yield<<" "<<h->GetName()<<endl;
  }
  cout<<"Total MC = "<<MCTotalYield<<endl;
  

  
  TCanvas C("C",TString("TauMuAnalysis_")+histoname+".ps");
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps[");
  
  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData->GetYaxis()->SetRangeUser(0, hData->GetMaximum()*1.2);
  if(log){
    hData->GetYaxis()->SetRangeUser(1, hData->GetMaximum()*1.3);
    C.SetLogy(1);
  }
  if(axesrange){
    hData->GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
    hData->GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
  }
  hData->SetStats(0);
  hData->GetXaxis()->SetTitle(xlabel);
  hData->GetYaxis()->SetTitle(ylabel);
  //hMC.Draw("hist");
  hData->Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData->Draw("hist pe same");
  TH1F*hMCTot=(TH1F*)(hMCStack.GetStack()->Last()->Clone("MCTot"));
  hMCTot->SetFillColor(0);
  hMCTot->SetLineColor(1);
  hMCTot->Draw("histsame");       
  hMCStack.GetHistogram()->Draw("same");
  legend.SetX1NDC(legendcoords[0]);
  legend.SetX2NDC(legendcoords[1]);
  legend.SetY1NDC(legendcoords[2]);
  legend.SetY2NDC(legendcoords[3]);
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
  /////////////////////////////////////////////////


  cout<<"Making residual plot"<<endl;
  C.Clear();
  TLine line;
  hData->Sumw2();
  hData->Add(((TH1F*)(hMCStack.GetStack()->Last())),-1);
  //hData->GetYaxis()->SetRangeUser(-5,5);
  hData->GetYaxis()->SetTitle("Data - MC");
  hData->Draw("hist pe");
  if(axesrange)
    line.DrawLine(axesrange[0],0,axesrange[1],0);
  else line.DrawLine(hData->GetXaxis()->GetXmin(),0,hData->GetXaxis()->GetXmax(),0);
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  

  


  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps]");

  return 1;

}




// bool TauMuAnalysis::plotDistribution(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){

  
//   TCanvas C("C",TString("TauMuAnalysis_")+histoname+".ps");
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps[");
  



//   TH1F* href=NULL;

//   cout<<" List of Data and MC samples: "<<endl;
//   float totalDataLumi=0.;
//   Int_t padcounter=0; 
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data"){
//       C.cd(++padcounter);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       totalDataLumi+=(*s)->getLumi();
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
//       href=hmass;
//     }
//   }

//   padcounter=0;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data_SS"){
//       C.cd(++padcounter);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
//     }
//   }

//   padcounter=0;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="MC" || (*s)->getDataType()=="Signal" ){
//       C.cd(++padcounter);
//       (*s)->scaleLumi(totalDataLumi);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->SetFillColor((*s)->getColor());
//       hmass->SetLineColor(1);
//       hmass->SetLineWidth(1);
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl; 
//     }
//   }
  
//   padcounter=0;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="MC_SS"){
//       C.cd(++padcounter);
//       (*s)->scaleLumi(totalDataLumi);
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->SetFillColor((*s)->getColor());
//       hmass->SetLineColor((*s)->getColor());
//       hmass->SetLineWidth(1);
//       cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
//     }
//   }


//   Float_t WJetsBelowCorr=1.;
//   Float_t WJetsSSBelowCorr=1.;

// //   ///////////////////////////////
// //   /////Determine correction factor for WJets from Data < -40 , assuming binning pZetaHisto","",180,-140,40)))
// //   //////////////////////////////
// //   cout<<"Determine WJetsSS from pZeta below -40: assume original binning pZetaHisto ,180,-140,40"<<endl;
// //   Float_t WJetsSSBelow=0; Float_t MCSSBelow=0; Float_t DataSSBelow=0;
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data_SS")
// //       DataSSBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);  
// //     else if(TString((*s)->GetName())=="WJetsToLNu_SS")
// //       WJetsSSBelow=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);    
// //     else if((*s)->getDataType()=="MC_SS") MCSSBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);
// //   }
// //   WJetsSSBelowCorr=(DataSSBelow-MCSSBelow)/WJetsSSBelow;
 
// //   Float_t WJetsBelow=0; Float_t MCBelow=0; Float_t DataBelow=0;
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data")
// //       DataBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100); 
// //     else if((*s)->getDataType()=="MC"){
// //       if(TString((*s)->GetName())=="WJetsToLNu") WJetsBelow=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);    
// //       else MCBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);
// //     }
// //   }
// //   WJetsBelowCorr=(DataBelow-MCBelow)/WJetsBelow;


//   ///////////////////////////////
//   /////Determine correction factor for WJets from massT > 50, assuming binning ,200,0,200)))
//   //////////////////////////////
//   cout<<"Determine WJetsSS from transverse mass > 50, assuming binning ,200,0,200"<<endl;
//   Float_t WJetsSSBelow=0; Float_t MCSSBelow=0; Float_t DataSSBelow=0;
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data_SS")
//       DataSSBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);  
//     else if(TString((*s)->GetName())=="WJetsToLNu_SS")
//       WJetsSSBelow=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);    
//     else if((*s)->getDataType()=="MC_SS") MCSSBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);
//   }
//   WJetsSSBelowCorr=(DataSSBelow-MCSSBelow)/WJetsSSBelow;
 
//   Float_t WJetsBelow=0; Float_t MCBelow=0; Float_t DataBelow=0;
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data")
//       DataBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200); 
//     else if((*s)->getDataType()=="MC"){
//       if(TString((*s)->GetName())=="WJetsToLNu") WJetsBelow=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);    
//       else MCBelow+=(*s)->getHistoFromFile("transverseMassHisto_massT")->Integral(51,200);
//     }
//   }
//   WJetsBelowCorr=(DataBelow-MCBelow)/WJetsBelow;


//   ///////////////////////////////////////////////////
//   ////Rebin only after doing necessary calculations
//   ////////////////////////////////////////////////////
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
//     ((TH1F*)((*s)->getHistoFromFile(histoname)))->Rebin(rebin);
    

// //   //////////////////////////////////////////////////////////
// //   ///Make a plot of the pZeta Distribution in the SS sample
// //   ///////////////////////////////////////////////////////////
// //   TH1F hDataSSpZeta("hDataSSpZeta","",30,-140,40);
// //   THStack hMCSSStackpZeta("hMCSSStackpZeta","MC");
// //   TLegend legendSSpZeta;
// //   legendSSpZeta.AddEntry(&hDataSSpZeta,TString("DataSS (")+(long)totalDataLumi+" pb^{-1})","p");
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data_SS"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);//before rebinning check if it is already rebinned above
// //       hDataSSpZeta.Add(h);
// //       delete h;
// //     }
// //     if((*s)->getDataType()=="MC_SS"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
// //       hMCSSStackpZeta.Add(h,"hist");
// //       legendSSpZeta.AddEntry(h,(*s)->GetName(),"f");
// //     }
// //   }
// //   C.Clear();
// //   hDataSSpZeta.SetStats(0);
// //   hDataSSpZeta.GetYaxis()->SetTitle("Evetns / 6 GeV");
// //   hDataSSpZeta.GetXaxis()->SetTitle("P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)");
// //   hDataSSpZeta.Draw("hist pe");
// //   hMCSSStackpZeta.Draw("same");
// //   legendSSpZeta.SetX1NDC(0.16);
// //   legendSSpZeta.SetX2NDC(0.45);
// //   legendSSpZeta.SetY1NDC(0.45);
// //   legendSSpZeta.SetY2NDC(0.8);
// //   legendSSpZeta.SetTextSize(.04);
// //   legendSSpZeta.Draw();
// //   hDataSSpZeta.Draw("pe same");
// //   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


// //   ///Make a plot of the pZeta Distribution in the OS sample
// //   TH1F hDataOSpZeta("hDataOSpZeta","",30,-140,40);
// //   THStack hMCOSStackpZeta("hMCOSStackpZeta","MC");
// //   TLegend legendOSpZeta;
// //   legendOSpZeta.AddEntry(&hDataOSpZeta,TString("Data (")+(long)totalDataLumi+" pb^{-1})","p");
// //   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
// //     if((*s)->getDataType()=="Data"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
// //       hDataOSpZeta.Add(h);
// //       delete h;
// //     }
// //     if((*s)->getDataType()=="MC"){
// //       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
// //       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// //       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
// //       hMCOSStackpZeta.Add(h,"hist");
// //       legendOSpZeta.AddEntry(h,(*s)->GetName(),"f");
// //     }
// //   }
// //   C.Clear();
// //   hDataOSpZeta.SetStats(0);
// //   hDataOSpZeta.GetYaxis()->SetTitle("Evetns / 6 GeV");
// //   hDataOSpZeta.GetXaxis()->SetTitle("P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)");
// //   hDataOSpZeta.Draw("hist pe");
// //   hMCOSStackpZeta.Draw("same");
// //   legendOSpZeta.SetX1NDC(0.16);
// //   legendOSpZeta.SetX2NDC(0.4);
// //   legendOSpZeta.SetY1NDC(0.45);
// //   legendOSpZeta.SetY2NDC(0.8);
// //   legendOSpZeta.SetTextSize(.04);
// //   legendOSpZeta.Draw();
// //   hDataOSpZeta.Draw("pe same");
// //   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
// //   /////////////////////////////////////////////////////////////////


//   if(histoname=="transverseMassHisto_massT"){
//     //////////////////////////////////////////////////////////
//     ///Make a plot of the mT Distribution in the SS sample
//     ///////////////////////////////////////////////////////////
//     TH1F hDataSSmassT("hDataSSmassT","",100,0,200);
//     THStack hMCSSStackmassT("hMCSSStackmassT","MC");
//     TLegend legendSSmassT;
//     legendSSmassT.SetLineColor(0);
//     legendSSmassT.SetBorderSize(1);
//     legendSSmassT.AddEntry(&hDataSSmassT,TString("DataSS (")+(long)totalDataLumi+" pb^{-1})","p");
//     for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//       if((*s)->getDataType()=="Data_SS"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);//before rebinning check if it is already rebinned above
// 	hDataSSmassT.Add(h);
// 	delete h;
//       }
//       if((*s)->getDataType()=="MC_SS"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
// 	hMCSSStackmassT.Add(h,"hist");
// 	legendSSmassT.AddEntry(h,(*s)->GetName(),"f");
//       }
//     }
//     C.Clear();
//     hDataSSmassT.SetStats(0);
//     hDataSSmassT.GetYaxis()->SetTitle("Events / 2 GeV");
//     hDataSSmassT.GetXaxis()->SetTitle("mass_{T}    (GeV)");
//     hDataSSmassT.Draw("hist pe");
//     hMCSSStackmassT.Draw("same");
//     legendSSmassT.SetX1NDC(0.50);
//     legendSSmassT.SetX2NDC(0.78);
//     legendSSmassT.SetY1NDC(0.45);
//     legendSSmassT.SetY2NDC(0.8);
//     legendSSmassT.SetTextSize(.04);
//     legendSSmassT.Draw();
//     hDataSSmassT.Draw("pe same");
//     C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


//     ///Make a plot of the massT Distribution in the OS sample
//     TH1F hDataOSmassT("hDataOSmassT","",100,0,200);
//     THStack hMCOSStackmassT("hMCOSStackmassT","MC");
//     TLegend legendOSmassT;   
//     legendOSmassT.SetLineColor(0);
//     legendOSmassT.SetBorderSize(1);
//     legendOSmassT.AddEntry(&hDataOSmassT,TString("Data (")+(long)totalDataLumi+" pb^{-1})","p");
//     for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//       if((*s)->getDataType()=="Data"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
// 	hDataOSmassT.Add(h);
// 	delete h;
//       }
//       if((*s)->getDataType()=="MC"){
// 	TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
// 	h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
// 	if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
// 	hMCOSStackmassT.Add(h,"hist");
// 	legendOSmassT.AddEntry(h,(*s)->GetName(),"f");
//       }
//     }
//     C.Clear();
//     hDataOSmassT.SetStats(0);
//     hDataOSmassT.GetYaxis()->SetTitle("Events / 2 GeV");
//     hDataOSmassT.GetXaxis()->SetTitle("mass_{T}    (GeV)");
//     hDataOSmassT.Draw("hist pe");
//     hMCOSStackmassT.Draw("same");
//     legendOSmassT.SetX1NDC(0.50);
//     legendOSmassT.SetX2NDC(0.78);
//     legendOSmassT.SetY1NDC(0.45);
//     legendOSmassT.SetY2NDC(0.8);
//     legendOSmassT.SetTextSize(.04);
//     legendOSmassT.Draw();
//     hDataOSmassT.Draw("pe same");
//     C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
//     /////////////////////////////////////////////////////////////////
//   }

  

//   cout<<"Yields: WJetsSS="<<WJetsSSBelow<<" OtherMCSS="<<MCSSBelow<<" DataSS="<<DataSSBelow<<" WJetsSSCorr="<<WJetsSSBelowCorr<<endl;
//   cout<<"Yields: WJets="<<WJetsBelow<<" OtherMC="<<MCBelow<<" Data="<<DataBelow<<" WJetsCorr="<<WJetsBelowCorr<<endl;
//   /////////////////////////////////////////
//   ////////Correct the WJets 
//   //////////////////////////////////////////
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if(TString((*s)->GetName())=="WJetsToLNu_SS"){
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->Scale(WJetsSSBelowCorr);
//     }
//     if(TString((*s)->GetName())=="WJetsToLNu"){
//       TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
//       hmass->Scale(WJetsBelowCorr);
//     }
//   }
  

  
//   cout<<" Calculating QCD: "<<endl;
//   ////////////////////////////////////
//   //Determine the QCD background
//   ///////////////////////////////////
//   TH1F hDataSS("hDataSS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   TH1F hQCD("hQCD","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   hQCD.SetLineColor(1);
//   hQCD.SetLineWidth(1);
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
//     if((*s)->getDataType()=="Data_SS"){
//       hDataSS.Add((*s)->getHistoFromFile(histoname));  
//       hQCD.SetFillColor((*s)->getColor());
//     }
//   cout<<"Data_SS = "<<hDataSS.Integral()<<endl;
//   hQCD.Add(&hDataSS);
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
//     if((*s)->getDataType()=="MC_SS"){
//       hQCD.Add((*s)->getHistoFromFile(histoname),-1.0);
//     }
//   cout<<"QCD before OS/SS rescaling = "<<hQCD.Integral()<<endl;
//   hQCD.Scale(QCDOStoSSRatio_);
//   cout<<"QCD = "<<hQCD.Integral()<<endl;
//   /////////////////////////////////////

  

//   cout<<" Sorting the MC samples by plot order"<<endl;
//   /////////////////////////
//   ////sort the samples accordin to plot order
//   //////////////////////////
//   std::vector<Sample*> orderedMCSamples;
//   unsigned int orderindex=1;
//   for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="MC"){      
//       for(std::vector<Sample*>::const_iterator sj=samples_.begin(); sj!=samples_.end(); ++sj){
// 	if((*sj)->getDataType()=="MC"){
// 	  if((*sj)->getPlotOrder()==orderindex){
// 	    cout<<orderindex<<" "<<(*sj)->GetName()<<endl;
// 	    orderedMCSamples.push_back((*sj));
// 	    orderindex++;		  
// 	  }
// 	}
//       }
//     }
//   }


//   cout<<" Merging Data: "<<endl;
//   ///////////////////////////////////
//   TH1F hData("hData","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
//   hData.SetLineWidth(1);
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data")
//       hData.Add((*s)->getHistoFromFile(histoname));  
//   }
//   cout<<"Data "<<(int)(hData.Integral())<<endl;

//   TLegend legend;
//   legend.SetLineColor(0);
//   legend.SetBorderSize(1);
//   legend.AddEntry(&hData,TString("Data  (")+(long)totalDataLumi+" pb^{-1})","p");

//   cout<<"Creating MCStack:"<<endl;
//   ////////////////////////
//   /////Stack the MC
//   ////////////////////////
//   THStack hMCStack("hMCStack","MC");//dont't set any of the regular histogram properties on the THStack will crash.
//   hMCStack.Add(&hQCD,"hist");
//   legend.AddEntry(&hQCD,"QCD","f");
//   Int_t MCTotalYield=hQCD.Integral();
//   cout<<hQCD.Integral()<<"  QCD"<<endl;
//   for(std::vector<Sample*>::const_iterator s=orderedMCSamples.begin(); s!=orderedMCSamples.end(); ++s){
//     TH1F* h=(TH1F*)((*s)->getHistoFromFile(histoname));    
//     hMCStack.Add(h,"hist");
//     legend.AddEntry(h,(*s)->GetName(),"f");
//     Int_t yield=(int)(h->Integral());
//     MCTotalYield+=yield;
//     cout<<yield<<" "<<h->GetName()<<endl;
//   }
//   cout<<"Total MC = "<<MCTotalYield<<endl;
  

//   cout<<" Creating Plot:"<<endl;
//   ///////////////////////
//   ///Make plot 
//   ///////////////////////
//   C.Clear();
//   hData.GetYaxis()->SetRangeUser(0, hData.GetMaximum()*1.2);
//   if(log){
//     hData.GetYaxis()->SetRangeUser(1, hData.GetMaximum()*1.3);
//     C.SetLogy(1);
//   }
//   if(axesrange){
//     hData.GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
//     hData.GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
//   }
//   hData.SetStats(0);
//   hData.GetXaxis()->SetTitle(xlabel);
//   hData.GetYaxis()->SetTitle(ylabel);
//   //hMC.Draw("hist");
//   hData.Draw("hist pe");
//   //////////////////////////
//   hMCStack.Draw("hist same");  
//   hData.Draw("hist pe same");
//   legend.SetX1NDC(legendcoords[0]);
//   legend.SetX2NDC(legendcoords[1]);
//   legend.SetY1NDC(legendcoords[2]);
//   legend.SetY2NDC(legendcoords[3]);
//   legend.SetTextSize(.04);
//   legend.Draw();
//   C.Update();
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
//   /////////////////////////////////////////////////


//   cout<<"Making residual plot"<<endl;
//   C.Clear();
//   TLine line;
//   hData.Sumw2();
//   hData.Add(((TH1F*)(hMCStack.GetStack()->Last())),-1);
//   //hData.GetYaxis()->SetRangeUser(-5,5);
//   hData.GetYaxis()->SetTitle("Data - MC");
//   hData.Draw("hist pe");
//   if(axesrange)
//     line.DrawLine(axesrange[0],0,axesrange[1],0);
//   else line.DrawLine(hData.GetXaxis()->GetXmin(),0,hData.GetXaxis()->GetXmax(),0);
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  

  


//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps]");

//   return 1;

// }



