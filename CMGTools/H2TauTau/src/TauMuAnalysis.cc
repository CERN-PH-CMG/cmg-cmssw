#include "CMGTools/H2TauTau/interface/TauMuAnalysis.h"
#include <THStack.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TLegend.h>

#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Run.h"

#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm.h"


TauMuAnalysis::TauMuAnalysis():
  BaseAnalysis(),
  inputTag_(""),
  diTauNHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauMassSVFitHisto_(NULL),
  diTauEtaHisto_(NULL),
  diTauPtHisto_(NULL),
  muPtHisto_(NULL),
  muIsoHisto_(NULL),
  muDxyHisto_(NULL),
  muDzHisto_(NULL),
  tauPtHisto_(NULL),
  tauIsoHisto_(NULL),
  tauDxyHisto_(NULL),
  tauDzHisto_(NULL),
  metHisto_(NULL),
  pZetaHisto_(NULL),
  transverseMassHisto_(NULL)
{
}

TauMuAnalysis::TauMuAnalysis(const char * name):
  BaseAnalysis(name),
  inputTag_("cmgTauMuSelClean"),
  diTauNHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauMassSVFitHisto_(NULL),
  diTauEtaHisto_(NULL),
  muPtHisto_(NULL),
  muIsoHisto_(NULL),
  muDxyHisto_(NULL),
  muDzHisto_(NULL),
  tauPtHisto_(NULL),
  tauIsoHisto_(NULL),
  tauDxyHisto_(NULL),
  tauDzHisto_(NULL),
  metHisto_(NULL),
  pZetaHisto_(NULL),
  transverseMassHisto_(NULL)
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
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_diTauMassSVFitHisto","",400,0,400))))return 0;
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

  return 1;
}


bool TauMuAnalysis::getHistos(Sample* s, TString tag){
  if(!s) return 0;

  if(!BaseAnalysis::getHistos(s,tag))return 0;
  
  if(tag!="")tag=TString("_")+tag;

  if(!(diTauNHisto_=(TH1F*)(s->getHisto(TString("diTauNHisto")+tag))))return 0;
  if(!(diTauMassHisto_=(TH1F*)(s->getHisto(TString("diTauMassHisto")+tag))))return 0;
  if(!(diTauMassSVFitHisto_=(TH1F*)(s->getHisto(TString("diTauMassSVFitHisto")+tag))))return 0;
  if(!(diTauEtaHisto_=(TH1F*)(s->getHisto(TString("diTauEtaHisto")+tag))))return 0;
  if(!(diTauPtHisto_=(TH1F*)(s->getHisto(TString("diTauPtHisto")+tag))))return 0;
  if(!(muPtHisto_=(TH1F*)(s->getHisto(TString("muPtHisto")+tag))))return 0;
  if(!(muIsoHisto_=(TH1F*)(s->getHisto(TString("muIsoHisto")+tag))))return 0;
  if(!(muPtHisto_=(TH1F*)(s->getHisto(TString("muPtHisto")+tag))))return 0;
  if(!(muDxyHisto_=(TH1F*)(s->getHisto(TString("muDxyHisto")+tag))))return 0;
  if(!(muDzHisto_=(TH1F*)(s->getHisto(TString("muDzHisto")+tag))))return 0;
  if(!(tauPtHisto_=(TH1F*)(s->getHisto(TString("tauPtHisto")+tag))))return 0;
  if(!(tauIsoHisto_=(TH1F*)(s->getHisto(TString("tauIsoHisto")+tag))))return 0;
  if(!(tauDxyHisto_=(TH1F*)(s->getHisto(TString("tauDxyHisto")+tag))))return 0;
  if(!(tauDzHisto_=(TH1F*)(s->getHisto(TString("tauDzHisto")+tag))))return 0;
  if(!(metHisto_=(TH1F*)(s->getHisto(TString("metHisto")+tag))))return 0;
  if(!(pZetaHisto_=(TH1F*)(s->getHisto(TString("pZetaHisto")+tag))))return 0;
  if(!(transverseMassHisto_=(TH1F*)(s->getHisto(TString("transverseMassHisto")+tag))))return 0;

  return 1;
}


void TauMuAnalysis::applyRecoilCorr(const cmg::TauMu * cand, TVector3 * MET){
  if(!(sample_->getApplyRecoilCorr()
       && genBoson_ 
       && (genEventType_==5 || genEventType_==11 || genEventType_==13 || genEventType_==15))) return;

  TVector3 tau1PT=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),0.);
  TVector3 tau2PT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
  TVector3 genBosonPT=TVector3(genBoson_->p4().x(),genBoson_->p4().y(),0.);
  double met = MET->Mag();
  double metphi = MET->Phi();    
  TVector3 recoLeptonPT = tau1PT+tau2PT;
  if(genEventType_==11 || genEventType_==13 || genEventType_==15) recoLeptonPT = tau2PT; //use only the muon for WJets
  recoilCorr_.Correct(met,metphi,genBosonPT.Mag(),genBosonPT.Phi(),recoLeptonPT.Mag(),recoLeptonPT.Phi());
  MET->SetMagThetaPhi(met,TMath::Pi()/2.,metphi);
}



float TauMuAnalysis::computePZeta(const cmg::TauMu * cand){
  if(!cand || met_->size()!=1) return 0.;
  //1) zeta axis is the bisector between tau1 and tau2 momentum vectors
  //2) project visible energy onto zeta axis
  //3) project MET onto zeta axis
  TVector3 tau1PT=TVector3(cand->leg1().p4().x(),cand->leg1().p4().y(),0.);
  TVector3 tau2PT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
  TVector3 metPT=TVector3(met_->begin()->p4().x(),met_->begin()->p4().y(),0.);
  applyRecoilCorr(cand,&metPT);

  TVector3 zetaAxis=(tau1PT.Unit() + tau2PT.Unit()).Unit();
  Float_t pZetaVis=(tau1PT + tau2PT)*zetaAxis;
  Float_t pZetaMET=metPT*zetaAxis;
  Float_t pZeta=pZetaVis+pZetaMET;
  
  return pZeta-1.5*pZetaVis;
}

float TauMuAnalysis::computeTransverseMass(const cmg::TauMu * cand){
  if(!cand) return 0.;

  TVector3 muonPT=TVector3(cand->leg2().p4().x(),cand->leg2().p4().y(),0.);
  TVector3 metPT=TVector3(met_->begin()->p4().x(),met_->begin()->p4().y(),0.);
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
    if(m->relIso()>0.3)continue;
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
  event->getByLabel(edm::InputTag("cmgPFMET"),met_);
  event->getByLabel(edm::InputTag("PFMETSignificanceAK5"),metsig_);

  ///get the TauMu cands 
  event->getByLabel(edm::InputTag(inputTag_.c_str()),diTauList_);

  //get the muons for the di-lepton veto
  event->getByLabel(edm::InputTag("cmgMuonSel"),diLeptonVetoList_);
  
  ///get the gen Boson
  genBoson_ = NULL;
  genEventType_=0;
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS"){  
    edm::Handle< std::vector<reco::GenParticle> > genParticles;
    event->getByLabel(edm::InputTag("genParticlesStatus3"),genParticles);    
    for(std::vector<reco::GenParticle>::const_iterator g=genParticles->begin(); g!=genParticles->end(); ++g){    
      //cout<<g->pdgId()<<" "<<g->p4().pt()<<endl;
      if((abs(g->pdgId())==23 || abs(g->pdgId())==24) && genBoson_==NULL )
	genBoson_=&(*g);
    }
    //if(genBoson_)cout<<"genBoson_ ref = "<<genBoson_<<endl;

    //determine type of generated event
    int genTaus=0;
    int genMuons=0;
    int genElectrons=0;
    if(genBoson_){      
      for(std::vector<reco::GenParticle>::const_iterator g=genParticles->begin(); g!=genParticles->end(); ++g){    
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
    }
  }

  
  return 1;
}

bool TauMuAnalysis::applySelections(TString exceptcut){
  //this function fills diTauSelList_ with the selected candidates
  //if none are selected returns 0
  diTauSelList_.clear();

  ///check the trigger
  if(!trigpass_) return 0;
  
  //select TauMu cands
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){    
    
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
    //if(exceptcut!="pzeta") if(computePZeta(&(*cand))<-20.0) continue;
    if(exceptcut!="massT") if(computeTransverseMass(&(*cand))>40.0) continue;
    if(exceptcut!="dileptonveto")if(computeDiLeptonVeto(&(cand->leg2()))) continue;


    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;
  

  return 1;
}

bool TauMuAnalysis::fillHistos(double weight ){
  
  if(!BaseAnalysis::fillHistos(weight)) return 0;
  
  //fill the diTau histograms
  diTauNHisto_->Fill(diTauSelList_.size());
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand){
    
    if(sample_->getApplyTauRateWeight() && (genEventType_==11 || genEventType_==13 || genEventType_==15)){
      float w=tauRate_.getScale(TauRate::Inclusive,cand->leg1().pt(),cand->leg1().eta());
      if(cand->leg1().decayMode()==0)tauRate_.getScale(TauRate::OneProng,cand->leg1().pt(),cand->leg1().eta());
      if(cand->leg1().decayMode()==1)tauRate_.getScale(TauRate::OneProngEM,cand->leg1().pt(),cand->leg1().eta());
      if(cand->leg1().decayMode()==2)tauRate_.getScale(TauRate::OneProngEM,cand->leg1().pt(),cand->leg1().eta());
      if(cand->leg1().decayMode()==10)tauRate_.getScale(TauRate::ThreeProng,cand->leg1().pt(),cand->leg1().eta());
      weight = weight*w;
    }

    diTauMassHisto_->Fill(cand->mass(),weight);
    diTauEtaHisto_->Fill(cand->eta(),weight);
    diTauPtHisto_->Fill(cand->eta(),weight);  

    tauPtHisto_->Fill(cand->leg1().pt(),weight);
    tauDxyHisto_->Fill(cand->leg1().dxy(),weight);
    tauDzHisto_->Fill(cand->leg1().dz(),weight);

    muPtHisto_->Fill(cand->leg2().pt(),weight);
    muIsoHisto_->Fill(cand->leg2().relIso(),weight);
    muDxyHisto_->Fill(cand->leg2().dxy(),weight);
    muDzHisto_->Fill(cand->leg2().dz(),weight);

    pZetaHisto_->Fill(computePZeta(&(*cand)),weight);  
    transverseMassHisto_->Fill(computeTransverseMass(&(*cand)),weight);  
    
    /////correct MET
    TVector3 metPT=TVector3((&(*(met_->begin())))->p4().x(),(&(*(met_->begin())))->p4().y(),0.);
    applyRecoilCorr(&(*cand),&metPT);
    metHisto_->Fill(metPT.Mag(),weight);  

//     /////SVFit
//     NSVfitStandalone::Vector measuredMET(metPT.x(),metPT.y(),0);
//     std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
//     NSVfitStandalone::LorentzVector p1(cand->leg1().p4().x(),cand->leg1().p4().y(),cand->leg1().p4().z(),cand->leg1().p4().t());
//     NSVfitStandalone::LorentzVector p2(cand->leg2().p4().x(),cand->leg2().p4().y(),cand->leg2().p4().z(),cand->leg2().p4().t());
//     measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay,p1));    
//     measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay,p2));
//     NSVfitStandaloneAlgorithm algo(measuredTauLeptons,measuredMET,(TMatrixD&)(*(metsig_->significance())),0);
//     algo.maxObjFunctionCalls(5000);
//     //algo.addLogM(false);
//     //algo.metPower(0.5)
//     algo.fit();
//     if(algo.isValidSolution())
//       diTauMassSVFitHisto_->Fill(algo.fittedDiTauSystem().mass(),weight);    


  }
 
  return 1;
}


    ///l1=muon , l2=tau
    //NSVfitStandalone::Vector l1(l1Pt*TMath::Sin(l1Phi ),l1Pt*TMath::Cos(l1Phi ), l1Pt*TMath::Tan(2*TMath::Exp(-l1Eta)));
    //NSVfitStandalone::Vector l2(l2Pt*TMath::Sin(l2Phi ),l2Pt*TMath::Cos(l2Phi ), l2Pt*TMath::Tan(2*TMath::Exp(-l2Eta)));        
    //    measuredTauLeptons.push_back( NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ?  NSVfitStandalone::kLepDecay : NSVfitStandalone::kLepDecay, TMath::Sqrt(l1M*l1M+l1.mag2()), l1));    
    //    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, TMath::Sqrt(l2M*l2M+l2.mag2()), l2));

bool TauMuAnalysis::createHistos(TString samplename){

  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++)
    if(samples_[i]->GetName()==samplename) sample_=samples_[i];
  if(!sample_){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}

  if(!addHistos(sample_))return 0;
  
  sample_->cloneHistos("tauiso");
  sample_->cloneHistos("tauagainstmuon");
  sample_->cloneHistos("muiso");
  //sample_->cloneHistos("pzeta");
  sample_->cloneHistos("massT");
  sample_->cloneHistos("dileptonveto");

  if(!sample_->getEvents()){
    cout<<" No fwlit::ChainEvent found in sample "<<sample_->GetName()<<endl;
    return 0;
  }

  
  fwlite::ChainEvent chain=*(sample_->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file


  Int_t ievt=0;
  Int_t goodevts=0;
  for(chain.toBegin(); !chain.atEnd() && ievt < truncateEvents_; ++chain, ++ievt){
    const fwlite::Event * event = chain.event();        
    if(!fillVariables(event)) return 0;
    if(ievt%printFreq_==0)cout<<ievt<<" events done , run = "<<runnumber_<<" , lumiblock = "<<lumiblock_<<" , eventid = "<<eventid_<<endl;
    
    //event selections
    if(!BaseAnalysis::applySelections()) continue;
    
//     //fill (N-1) cut histograms here
//     if(applySelections("tauiso")){      
//       if(!getHistos(sample_,"tauiso"))return 0;
//       if(!fillHistos(mcPUPWeight_))return 0;
//     }
//     if(applySelections("tauagainstmuon")){      
//       if(!getHistos(sample_,"tauagainstmuon"))return 0;
//       if(!fillHistos(mcPUPWeight_))return 0;
//     }
//     if(applySelections("muiso")){      
//       if(!getHistos(sample_,"muiso"))return 0;
//       if(!fillHistos(mcPUPWeight_))return 0;
//     }
//     if(applySelections("dileptonveto")){      
//       if(!getHistos(sample_,"dileptonveto"))return 0;
//       if(!fillHistos(mcPUPWeight_))return 0;
//     }


    if(applySelections("massT")){      
      if(!getHistos(sample_,"massT"))return 0;
      if(!fillHistos(mcPUPWeight_))return 0;
    }
    //fill fully selected histograms 
    if(applySelections()){           
      if(!getHistos(sample_))return 0;
      if(!fillHistos(mcPUPWeight_))return 0;
      goodevts++;//only for fully selected events
      //cout<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
    }

  }
  cout<<goodevts<<" events passed selections"<<endl;
  
  //
  if(!sample_->save())return 0;
  
  
  return 1;
}


bool TauMuAnalysis::plotDistribution(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log){

  
  TCanvas C("C",TString("TauMuAnalysis_")+histoname+".ps");
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps[");
  

  Float_t QCDOStoSSRatio=1.06;
  cout<<"Warning: the following binning is assummed for pZeta distribution : 180,-140,40"<<endl;

  TH1F* href=NULL;

  cout<<" List of Data and MC samples: "<<endl;
  float totalDataLumi=0.;
  Int_t padcounter=0; 
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      C.cd(++padcounter);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      totalDataLumi+=(*s)->getLumi();
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
      href=hmass;
    }
  }

  padcounter=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      C.cd(++padcounter);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
    }
  }

  padcounter=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC" || (*s)->getDataType()=="Signal" ){
      C.cd(++padcounter);
      (*s)->scaleLumi(totalDataLumi);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->SetFillColor((*s)->getColor());
      hmass->SetLineColor(1);
      hmass->SetLineWidth(1);
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl; 
    }
  }
  
  padcounter=0;
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="MC_SS"){
      C.cd(++padcounter);
      (*s)->scaleLumi(totalDataLumi);
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->SetFillColor((*s)->getColor());
      hmass->SetLineColor((*s)->getColor());
      hmass->SetLineWidth(1);
      cout<<(*s)->GetName()<<" "<<hmass->Integral()<<endl;  
    }
  }


  Float_t WJetsBelowCorr=1.;
  Float_t WJetsSSBelowCorr=1.;

//   ///////////////////////////////
//   /////Determine correction factor for WJets from Data < -40 , assuming binning pZetaHisto","",180,-140,40)))
//   //////////////////////////////
//   cout<<"Determine WJetsSS from pZeta below -40: assume original binning pZetaHisto ,180,-140,40"<<endl;
//   Float_t WJetsSSBelow=0; Float_t MCSSBelow=0; Float_t DataSSBelow=0;
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data_SS")
//       DataSSBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);  
//     else if(TString((*s)->GetName())=="WJetsToLNu_SS")
//       WJetsSSBelow=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);    
//     else if((*s)->getDataType()=="MC_SS") MCSSBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);
//   }
//   WJetsSSBelowCorr=(DataSSBelow-MCSSBelow)/WJetsSSBelow;
 
//   Float_t WJetsBelow=0; Float_t MCBelow=0; Float_t DataBelow=0;
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data")
//       DataBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100); 
//     else if((*s)->getDataType()=="MC"){
//       if(TString((*s)->GetName())=="WJetsToLNu") WJetsBelow=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);    
//       else MCBelow+=(*s)->getHistoFromFile("pZetaHisto_pzeta")->Integral(1,100);
//     }
//   }
//   WJetsBelowCorr=(DataBelow-MCBelow)/WJetsBelow;


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


  ///////////////////////////////////////////////////
  ////Rebin only after doing necessary calculations
  ////////////////////////////////////////////////////
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    ((TH1F*)((*s)->getHistoFromFile(histoname)))->Rebin(rebin);
    

//   //////////////////////////////////////////////////////////
//   ///Make a plot of the pZeta Distribution in the SS sample
//   ///////////////////////////////////////////////////////////
//   TH1F hDataSSpZeta("hDataSSpZeta","",30,-140,40);
//   THStack hMCSSStackpZeta("hMCSSStackpZeta","MC");
//   TLegend legendSSpZeta;
//   legendSSpZeta.AddEntry(&hDataSSpZeta,TString("DataSS (")+(long)totalDataLumi+" pb^{-1})","p");
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data_SS"){
//       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
//       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
//       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);//before rebinning check if it is already rebinned above
//       hDataSSpZeta.Add(h);
//       delete h;
//     }
//     if((*s)->getDataType()=="MC_SS"){
//       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
//       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
//       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
//       hMCSSStackpZeta.Add(h,"hist");
//       legendSSpZeta.AddEntry(h,(*s)->GetName(),"f");
//     }
//   }
//   C.Clear();
//   hDataSSpZeta.SetStats(0);
//   hDataSSpZeta.GetYaxis()->SetTitle("Evetns / 6 GeV");
//   hDataSSpZeta.GetXaxis()->SetTitle("P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)");
//   hDataSSpZeta.Draw("hist pe");
//   hMCSSStackpZeta.Draw("same");
//   legendSSpZeta.SetX1NDC(0.16);
//   legendSSpZeta.SetX2NDC(0.45);
//   legendSSpZeta.SetY1NDC(0.45);
//   legendSSpZeta.SetY2NDC(0.8);
//   legendSSpZeta.SetTextSize(.04);
//   legendSSpZeta.Draw();
//   hDataSSpZeta.Draw("pe same");
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


//   ///Make a plot of the pZeta Distribution in the OS sample
//   TH1F hDataOSpZeta("hDataOSpZeta","",30,-140,40);
//   THStack hMCOSStackpZeta("hMCOSStackpZeta","MC");
//   TLegend legendOSpZeta;
//   legendOSpZeta.AddEntry(&hDataOSpZeta,TString("Data (")+(long)totalDataLumi+" pb^{-1})","p");
//   for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
//     if((*s)->getDataType()=="Data"){
//       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
//       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
//       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
//       hDataOSpZeta.Add(h);
//       delete h;
//     }
//     if((*s)->getDataType()=="MC"){
//       TH1F* h=(TH1F*)((*s)->getHistoFromFile("pZetaHisto_pzeta"));    
//       h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
//       if(h->GetXaxis()->GetNbins()!=30)h->Rebin(6);
//       hMCOSStackpZeta.Add(h,"hist");
//       legendOSpZeta.AddEntry(h,(*s)->GetName(),"f");
//     }
//   }
//   C.Clear();
//   hDataOSpZeta.SetStats(0);
//   hDataOSpZeta.GetYaxis()->SetTitle("Evetns / 6 GeV");
//   hDataOSpZeta.GetXaxis()->SetTitle("P_{#zeta} - 1.5*P_{#zeta}^{vis}    (GeV)");
//   hDataOSpZeta.Draw("hist pe");
//   hMCOSStackpZeta.Draw("same");
//   legendOSpZeta.SetX1NDC(0.16);
//   legendOSpZeta.SetX2NDC(0.4);
//   legendOSpZeta.SetY1NDC(0.45);
//   legendOSpZeta.SetY2NDC(0.8);
//   legendOSpZeta.SetTextSize(.04);
//   legendOSpZeta.Draw();
//   hDataOSpZeta.Draw("pe same");
//   C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
//   /////////////////////////////////////////////////////////////////


  
  //////////////////////////////////////////////////////////
  ///Make a plot of the mT Distribution in the SS sample
  ///////////////////////////////////////////////////////////
  TH1F hDataSSmassT("hDataSSmassT","",100,0,200);
  THStack hMCSSStackmassT("hMCSSStackmassT","MC");
  TLegend legendSSmassT;
  legendSSmassT.SetLineColor(0);
  legendSSmassT.SetBorderSize(1);
  legendSSmassT.AddEntry(&hDataSSmassT,TString("DataSS (")+(long)totalDataLumi+" pb^{-1})","p");
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data_SS"){
      TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
      h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
      if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);//before rebinning check if it is already rebinned above
      hDataSSmassT.Add(h);
      delete h;
    }
    if((*s)->getDataType()=="MC_SS"){
      TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
      h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
      if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
      hMCSSStackmassT.Add(h,"hist");
      legendSSmassT.AddEntry(h,(*s)->GetName(),"f");
    }
  }
  C.Clear();
  hDataSSmassT.SetStats(0);
  hDataSSmassT.GetYaxis()->SetTitle("Events / 2 GeV");
  hDataSSmassT.GetXaxis()->SetTitle("mass_{T}    (GeV)");
  hDataSSmassT.Draw("hist pe");
  hMCSSStackmassT.Draw("same");
  legendSSmassT.SetX1NDC(0.50);
  legendSSmassT.SetX2NDC(0.78);
  legendSSmassT.SetY1NDC(0.45);
  legendSSmassT.SetY2NDC(0.8);
  legendSSmassT.SetTextSize(.04);
  legendSSmassT.Draw();
  hDataSSmassT.Draw("pe same");
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  


  ///Make a plot of the massT Distribution in the OS sample
  TH1F hDataOSmassT("hDataOSmassT","",100,0,200);
  THStack hMCOSStackmassT("hMCOSStackmassT","MC");
  TLegend legendOSmassT;   
  legendOSmassT.SetLineColor(0);
  legendOSmassT.SetBorderSize(1);
  legendOSmassT.AddEntry(&hDataOSmassT,TString("Data (")+(long)totalDataLumi+" pb^{-1})","p");
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data"){
      TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
      h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
      if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
      hDataOSmassT.Add(h);
      delete h;
    }
    if((*s)->getDataType()=="MC"){
      TH1F* h=(TH1F*)((*s)->getHistoFromFile("transverseMassHisto_massT"));    
      h=(TH1F*)h->Clone(TString(h->GetName())+"rebin");
      if(h->GetXaxis()->GetNbins()!=100)h->Rebin(2);
      hMCOSStackmassT.Add(h,"hist");
      legendOSmassT.AddEntry(h,(*s)->GetName(),"f");
    }
  }
  C.Clear();
  hDataOSmassT.SetStats(0);
  hDataOSmassT.GetYaxis()->SetTitle("Events / 2 GeV");
  hDataOSmassT.GetXaxis()->SetTitle("mass_{T}    (GeV)");
  hDataOSmassT.Draw("hist pe");
  hMCOSStackmassT.Draw("same");
  legendOSmassT.SetX1NDC(0.50);
  legendOSmassT.SetX2NDC(0.78);
  legendOSmassT.SetY1NDC(0.45);
  legendOSmassT.SetY2NDC(0.8);
  legendOSmassT.SetTextSize(.04);
  legendOSmassT.Draw();
  hDataOSmassT.Draw("pe same");
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
  /////////////////////////////////////////////////////////////////


  

  cout<<"Yields: WJetsSS="<<WJetsSSBelow<<" OtherMCSS="<<MCSSBelow<<" DataSS="<<DataSSBelow<<" WJetsSSCorr="<<WJetsSSBelowCorr<<endl;
  cout<<"Yields: WJets="<<WJetsBelow<<" OtherMC="<<MCBelow<<" Data="<<DataBelow<<" WJetsCorr="<<WJetsBelowCorr<<endl;
  /////////////////////////////////////////
  ////////Correct the WJets 
  //////////////////////////////////////////
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if(TString((*s)->GetName())=="WJetsToLNu_SS"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Scale(WJetsSSBelowCorr);
    }
    if(TString((*s)->GetName())=="WJetsToLNu"){
      TH1F*hmass=(TH1F*)((*s)->getHistoFromFile(histoname));
      if(!hmass){cout<<" no histo found for "<<(*s)->GetName()<<endl; return 0;}
      hmass->Scale(WJetsBelowCorr);
    }
  }
  

  
  cout<<" Calculating QCD: "<<endl;
  ////////////////////////////////////
  //Determine the QCD background
  ///////////////////////////////////
  TH1F hDataSS("hDataSS","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  TH1F hQCD("hQCD","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  hQCD.SetLineColor(1);
  hQCD.SetLineWidth(1);
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="Data_SS"){
      hDataSS.Add((*s)->getHistoFromFile(histoname));  
      hQCD.SetFillColor((*s)->getColor());
    }
  cout<<"Data_SS = "<<hDataSS.Integral()<<endl;
  hQCD.Add(&hDataSS);
  for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
    if((*s)->getDataType()=="MC_SS"){
      hQCD.Add((*s)->getHistoFromFile(histoname),-1.0);
    }
  cout<<"QCD before OS/SS rescaling = "<<hQCD.Integral()<<endl;
  hQCD.Scale(QCDOStoSSRatio);
  cout<<"QCD = "<<hQCD.Integral()<<endl;
  /////////////////////////////////////

  

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


  cout<<" Merging Data: "<<endl;
  ///////////////////////////////////
  TH1F hData("hData","",href->GetXaxis()->GetNbins(),href->GetXaxis()->GetXmin(),href->GetXaxis()->GetXmax());
  hData.SetLineWidth(1);
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s){
    if((*s)->getDataType()=="Data")
      hData.Add((*s)->getHistoFromFile(histoname));  
  }
  cout<<"Data "<<(int)(hData.Integral())<<endl;

  TLegend legend;
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.AddEntry(&hData,TString("Data  (")+(long)totalDataLumi+" pb^{-1})","p");

  cout<<"Creating MCStack:"<<endl;
  ////////////////////////
  /////Stack the MC
  ////////////////////////
  THStack hMCStack("hMCStack","MC");//dont't set any of the regular histogram properties on the THStack will crash.
  hMCStack.Add(&hQCD,"hist");
  legend.AddEntry(&hQCD,"QCD","f");
  Int_t MCTotalYield=hQCD.Integral();
  cout<<hQCD.Integral()<<"  QCD"<<endl;
  for(std::vector<Sample*>::const_iterator s=orderedMCSamples.begin(); s!=orderedMCSamples.end(); ++s){
    TH1F* h=(TH1F*)((*s)->getHistoFromFile(histoname));    
    hMCStack.Add(h,"hist");
    legend.AddEntry(h,(*s)->GetName(),"f");
    Int_t yield=(int)(h->Integral());
    MCTotalYield+=yield;
    cout<<yield<<" "<<h->GetName()<<endl;
  }
  cout<<"Total MC = "<<MCTotalYield<<endl;
  

  cout<<" Creating Plot:"<<endl;
  ///////////////////////
  ///Make plot 
  ///////////////////////
  C.Clear();
  hData.GetYaxis()->SetRangeUser(0, hData.GetMaximum()*1.2);
  if(log){
    hData.GetYaxis()->SetRangeUser(1, hData.GetMaximum()*1.3);
    C.SetLogy(1);
  }
  if(axesrange){
    hData.GetXaxis()->SetRangeUser(axesrange[0],axesrange[1]);
    hData.GetYaxis()->SetRangeUser(axesrange[2],axesrange[3]);
  }
  hData.SetStats(0);
  hData.GetXaxis()->SetTitle(xlabel);
  hData.GetYaxis()->SetTitle(ylabel);
  //hMC.Draw("hist");
  hData.Draw("hist pe");
  //////////////////////////
  hMCStack.Draw("hist same");  
  hData.Draw("hist pe same");
  legend.SetX1NDC(legendcoords[0]);
  legend.SetX2NDC(legendcoords[1]);
  legend.SetY1NDC(legendcoords[2]);
  legend.SetY2NDC(legendcoords[3]);
  legend.SetTextSize(.04);
  legend.Draw();
  C.Update();
  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps");  
  /////////////////////////////////////////////////


  C.Print(outputpath_+"/TauMuAnalysis_"+histoname+".ps]");

  return 1;

}


//this function is not necessarily up to date, safer to use above function
bool  TauMuAnalysis::plotVisMass(){
  
//   TCanvas C("C","TauMuAnalysis_plotVisMass.ps");
//   C.Print("TauMuAnalysis_plotVisMass.ps[");
  
//   TString histoname="diTauMassPUPHisto";
//   float totalDataLumi=0.;
//   Float_t Nevt[100];
//   Int_t rebin=10;

//   ///////////////////////////////
//   ////plot the raw distributions
//   ///////////////////////////
//   C.Clear();
//   C.Divide(2,2);
//   Int_t padcounter=0;
//   std::vector<Sample*>::const_iterator s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     if(samples_[i]->getDataType()=="Data"){
//       C.cd(++padcounter);
//       TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
//       hmass->Rebin(rebin);
//       hmass->SetTitle(samples_[i]->GetName());
//       hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//       hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//       hmass->Draw("hist");         
//       totalDataLumi+=samples_[i]->getLumi();
//     }
//   }
//   cout<<" total Data lumi "<<totalDataLumi<<endl;
//   C.Print("TauMuAnalysis_plotVisMass.ps");  

//   C.Clear();
//   C.Divide(2,2);
//   padcounter=0;
//   s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     if(samples_[i]->getDataType()=="Data_SS"){
//       C.cd(++padcounter);
//       TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
//       hmass->Rebin(rebin);
//       hmass->SetTitle(samples_[i]->GetName());
//       hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//       hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//       hmass->Draw("hist");         
//     }
//   }
//   C.Print("TauMuAnalysis_plotVisMass.ps");  


//   C.Clear();
//   C.Divide(3,3);
//   padcounter=0;
//   s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     if(samples_[i]->getDataType()=="MC" || samples_[i]->getDataType()=="Signal" ){
//       C.cd(++padcounter);
//       samples_[i]->scaleLumi(totalDataLumi);
//       TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
//       cout<<samples_[i]->GetName()<<" "<<hmass->Integral()<<endl;  
//       hmass->Rebin(rebin);
//       hmass->SetTitle(samples_[i]->GetName());
//       hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//       hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//       hmass->Draw("hist");
//     }
//   }
//   C.Print("TauMuAnalysis_plotVisMass.ps");  

//   C.Clear();
//   C.Divide(3,3);
//   padcounter=0;
//   s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     if(samples_[i]->getDataType()=="MC_SS"){
//       C.cd(++padcounter);
//       samples_[i]->scaleLumi(totalDataLumi);
//       TH1F*hmass=(TH1F*)(samples_[i]->getHistoFromFile(histoname));
//       if(!hmass){cout<<" no histo found for "<<samples_[i]->GetName()<<endl; return 0;}
//       hmass->Rebin(rebin);
//       hmass->SetTitle(samples_[i]->GetName());
//       hmass->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//       hmass->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//       hmass->Draw("hist");
//     }
//   }
//   C.Print("TauMuAnalysis_plotVisMass.ps");  


//   /////////////////////////
//   ////table of number of events for each MC sample
//   //////////////////////////
//   s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     Nevt[i]=0;
//     if(samples_[i]->getDataType()=="MC")
//       Nevt[i]=samples_[i]->getHistoFromFile(histoname)->Integral();
//   }
//   Float_t NevtSort[100];
//   unsigned int  NevtSortIndex[100];
//   for(unsigned int i=0;i<samples_.size();i++){
//     NevtSort[i]=0.0;    NevtSortIndex[i]=100;
//     for(unsigned int j=0;j<samples_.size();j++){
//       if(samples_[j]->getDataType()=="MC")
// 	if(Nevt[j]>NevtSort[i]&&(i==0 ? 1 : Nevt[j]<NevtSort[i-1])){
// 	  NevtSort[i]=Nevt[j];	  NevtSortIndex[i]=j;
// 	}
//     }   
//   }

 


//   ////////////////////////////////////
//   //Plot the weighted Same-Sign MC and Data
//   ///////////////////////////////////
//   Sample totalMC_SS("totalMC_SS",".");
//   addHistos(&totalMC_SS);
//   TH1F*hMC_SS=(TH1F*)totalMC_SS.getHisto(histoname);
//   Sample totalData_SS("totalData_SS",".");
//   addHistos(&totalData_SS);
//   TH1F*hData_SS=(TH1F*)totalData_SS.getHisto(histoname);
//   if(!hMC_SS || !hData_SS) return 0;
//   hMC_SS->Rebin(rebin);
//   hMC_SS->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//   hMC_SS->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//   hData_SS->Rebin(rebin);
//   hData_SS->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//   hData_SS->GetXaxis()->SetTitle("m_{vis}   (GeV)");

//   THStack hMCSSStack("hMCSSStack","MCSS");

//   s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     if(samples_[i]->getDataType()=="Data_SS"){
//       hData_SS->Add(samples_[i]->getHistoFromFile(histoname));  
//       hData_SS->SetFillColor(samples_[i]->getColor());
//       hData_SS->SetLineColor(samples_[i]->getColor());
//     }
//     if(samples_[i]->getDataType()=="MC_SS"){
//       hMC_SS->Add(samples_[i]->getHistoFromFile(histoname));
//       if(samples_[i]->getHistoFromFile(histoname)->Integral()/(hData_SS->Integral())>0.01)//only plot if significant
// 	hMCSSStack.Add(samples_[i]->getHistoFromFile(histoname),"hist");
//     }
//   }

//   C.Clear();
//   hData_SS->Draw("hist pe");
//   hMCSSStack.Draw("hist same");
//   hMC_SS->Draw("hist same");
//   C.Print("TauMuAnalysis_plotVisMass.ps");  


//   ////////////////////////
//   /////Plot the stacked MC
//   ////////////////////////
//   THStack hMCStack("hMCStack","MC");
//   //hMCStack.GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//   //hMCStack.GetXaxis()->SetTitle("m_{vis}   (GeV)");

//   ///QCD background determined from SS Data - SS MC, should be biggest
//   hData_SS->Add(hMC_SS,-1.0);
//   hMCStack.Add(hData_SS,"hist");
//   ///////add MC's
//   for(unsigned int i=0;NevtSortIndex[i]!=100;i++){
//     if(samples_[i]->getHistoFromFile(histoname)->Integral()/(2*hData_SS->Integral())>0.01)//only plot if significant
//       hMCStack.Add(samples_[NevtSortIndex[i]]->getHistoFromFile(histoname),"hist");
//   }
//   C.Clear();
//   hMCStack.Draw("hist");
//   C.Print("TauMuAnalysis_plotVisMass.ps");  
//   ///////////////////////

//   ///print background yields
//   cout<<"QCD "<<hData_SS->Integral()<<endl;
//   for(unsigned int i=0;i<samples_.size();i++)
//     if(NevtSort[i]>0.)cout<<samples_[NevtSortIndex[i]]->GetName()<<" "<<floor(NevtSort[i])<<endl;   


//   ////////////////////////////////////
//   //Plot the weighted MC and Data
//   ///////////////////////////////////
//   Sample totalMC("totalMC",".");
//   addHistos(&totalMC);
//   TH1F*hMC=(TH1F*)totalMC.getHisto(histoname);
//   Sample totalData("totalData",".");
//   addHistos(&totalData);
//   TH1F*hData=(TH1F*)totalData.getHisto(histoname);
//   Sample Signal("Signal",".");
//   addHistos(&Signal);
//   TH1F*hSignal=(TH1F*)Signal.getHisto(histoname);
//   if(!hMC || !hData || !hSignal) return 0;
//   hMC->Rebin(rebin);
//   hMC->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//   hMC->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//   hData->Rebin(rebin);
//   hData->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//   hData->GetXaxis()->SetTitle("m_{vis}   (GeV)");
//   hSignal->Rebin(rebin);
//   hSignal->GetYaxis()->SetTitle(TString("Events / ")+(long)rebin+" GeV");
//   hSignal->GetXaxis()->SetTitle("m_{vis}   (GeV)");

//   ///Sum all samples
//   hMC->Add(hData_SS);
//   s=samples_.begin();
//   for(int i=0; s!=samples_.end(); ++s, i++){
//     if(samples_[i]->getDataType()=="Data"){
//       hData->Add(samples_[i]->getHistoFromFile(histoname));  
//     }
//     if(samples_[i]->getDataType()=="MC"){
//       hMC->Add(samples_[i]->getHistoFromFile(histoname));
//     }
//     if(samples_[i]->getDataType()=="Signal"){
//       hSignal->Add(samples_[i]->getHistoFromFile(histoname));
//     }
//   }


//   C.Clear();
//   hData->SetStats(0);
//   hData->Draw("hist pe");
//   hMCStack.Draw("hist same");
//   hMC->Draw("hist same");
//   hSignal->Draw("hist same");
//   hData->Draw("hist pe same");
//   C.Print("TauMuAnalysis_plotVisMass.ps");  
//   /////////////////////////////////////////////////



//   C.Print("TauMuAnalysis_plotVisMass.ps]");

  return 1;
}


//   cout<<"yields:"<<endl;
//   /////////////////////////
//   ////Print out the event yields
//   //////////////////////////
//   std::vector<Sample*> sizeOrderedMCSamples;  
//   float sizeorder=10000000;
//   for(std::vector<Sample*>::const_iterator si=samples_.begin(); si!=samples_.end(); ++si)
//     if((*si)->getDataType()=="MC"){
//       Sample* biggestSample=NULL;
//       float sizelargest=0;
//       for(std::vector<Sample*>::const_iterator sj=samples_.begin(); sj!=samples_.end(); ++sj)
// 	if((*sj)->getDataType()=="MC"){
// 	  TH1* h=(*sj)->getHistoFromFile(histoname);
// 	  if(h->Integral() < (sizeorder-0.01) && h->Integral() > (sizelargest+0.01)){	    
// 	    biggestSample= *sj ;
// 	    sizelargest=h->Integral();
// 	  }
// 	}
//       sizeOrderedMCSamples.push_back(biggestSample);    
//       sizeorder = sizelargest;
//     }

  
 
 




    ///From CMGTools factory
//    edm::Handle<edm::TriggerResults> triggerResults;
//    //const edm::InputTag triggerResultsLabel = edm::InputTag( triggerResultsLabel_.label(), triggerResultsLabel_.instance(), processName_);
//    const edm::InputTag triggerResultsLabel = edm::InputTag("TriggerResults","","PAT");
//    //const edm::InputTag triggerResultsLabel = edm::InputTag("TriggerResults");
//    event->getByLabel(triggerResultsLabel, triggerResults);
//    const edm::TriggerNames& triggerNames = event->triggerNames(*triggerResults);
//    edm::TriggerNames::Strings const& names = triggerNames.triggerNames();
//    cout<<" names.size()"<<names.size()<<endl;
//    bool trigpass=0;  
//    for(edm::TriggerNames::Strings::const_iterator it = names.begin(); it != names.end(); ++it) {
//      const unsigned int i = triggerNames.triggerIndex(*it);
//      cout<<" "<<*it<<" "<<triggerResults->wasrun(i) <<" "<<triggerResults->accept(i)<<endl;
//      if(*it==TString("HLT_IsoMu12")){
//        cout<<"trigger "<<triggerResults->wasrun(i) <<" "<<triggerResults->accept(i)<<endl;
//        if(triggerResults->wasrun(i) && triggerResults->accept(i))trigpass=1;
//      }
//    }
//    if(!trigpass)continue;

    ///From Lorenzo
//    edm::Handle<pat::TriggerEvent> triggerHandle;
//    const edm::InputTag triggerResultsTag_ = edm::InputTag("triggerResults");
//    event->getByLabel(triggerResultsTag_,triggerHandle);
//    const pat::TriggerEvent * trigger = triggerHandle.product();
//    if(!trigger){cout<<"no trigger"<<endl; return 0;}
//    const pat::TriggerPath * triggerPath =  trigger->path("HLT_IsoMu12");
//    if(!triggerPath){cout<<" no path "<<endl; continue;}
//    cout<<"trigger "<<triggerPath->wasRun()<<" "<<triggerPath->wasAccept()<<endl;
    
// m + th channel
// HLT IsoMu12
// HLT IsoMu12 LooseIsoPFTau10
      // HLT_IsoMu12_LooseIsoPFTau10_v2
      // HLT_IsoMu12_v1
      // HLT_IsoMu15_v5
      // HLT_IsoMu17_CentralJet40_BTagIP_v2    
      // HLT_IsoMu17_v5
      // HLT_IsoMu24_v1
      // HLT_IsoMu30_v1

// e + th channel
// HLT Ele15 CaloIdVT CaloIsoT TrkIdT TrkIsoT LooseIsoPFTau15
      // HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2 
      // HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2 
      // HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2 
      // HLT_Ele15_CaloIdVT_TrkIdT_LooseIsoPFTau15_v2  

// e + m channel
// HLT Mu8 Ele17 CaloIdL
// HLT Mu17 Ele8 CaloIdL
      // HLT_Mu8_Ele17_CaloIdL_v2      
      // HLT_Mu8_HT200_v3      
      // HLT_Mu8_Jet40_v3      
      // HLT_Mu8_Photon20_CaloIdVT_IsoT_v2     
      // HLT_Mu8_v1       
      // HLT_Mu17_CentralJet30_v2      
      // HLT_Mu17_CentralJet40_BTagIP_v2 
      // HLT_Mu17_DiCentralJet30_v2    
      // HLT_Mu17_Ele8_CaloIdL_v2      
      // HLT_Mu17_TriCentralJet30_v2   
