#include "CMGTools/H2TauTau/plugins/TauMuFlatNtp.h"


TauMuFlatNtp::TauMuFlatNtp(const edm::ParameterSet & iConfig):
  BaseFlatNtp(iConfig),
  diTauSel_(0),
  triggerEffWeight_(0.),
  selectionEffWeight_(0.),
  embeddedGenWeight_(0.),
  ditaumass_(0.),
  ditaucharge_(0.),
  ditaueta_(0.),
  ditaupt_(0.),
  svfitmass_(0.),
  taupt_(0.),
  taueta_(0.),
  tautruth_(0),
  tauehop_(0.),
  taueop_(0.),
  taudecaymode_(0),
  taudz_(0.),
  taudxy_(0.),
  mupt_(0.),
  mueta_(0.),
  muiso_(0.),
  mudz_(0.),
  mudxy_(0.),
  transversemass_(0.),
  metpt_(0.),
  metphi_(0.),  
  njet_(0),
  leadJetPt_(0.),
  leadJetEta_(0.),
  diJetMass_(0.),
  diJetDeltaEta_(0.),
  diJetEta1Eta2_(0.),
  categoryCh_(0),
  categoryMT_(0),
  categoryIso_(0),
  categorySM_(0),
  sampleGenEventType_(0),
  sampleTruthEventType_(0),
  genEventType_(0),
  truthEventType_(0),
  deltaRTruth_(0.3),
  genBoson_(0),
  genBosonL1_(0),
  genBosonL2_(0),
  counterall_(0),
  counterev_(0),
  countergen_(0),
  counterveto_(0),
  countereop_(0),
  countertaumatch_(0),
  countermumatch_(0),
  counterditau_(0),
  countertruth_(0),
  counter_(0)
{
  diTauTag_          = iConfig.getParameter<edm::InputTag>("diTauTag");
  sampleGenEventType_= iConfig.getParameter<int>("sampleGenEventType");
  sampleTruthEventType_= iConfig.getParameter<int>("sampleTruthEventType");

}


TauMuFlatNtp::~TauMuFlatNtp(){

}


void TauMuFlatNtp::beginJob(){

  BaseFlatNtp::beginJob();
  //  tree_->Branch("",&_,"/F");
  tree_->Branch("ditaumass",&ditaumass_,"ditaumass/F");
  tree_->Branch("ditaucharge",&ditaucharge_,"ditaucharge/I");
  tree_->Branch("svfitmass",&svfitmass_,"svfitmass/F");
  tree_->Branch("ditaueta",&ditaueta_,"ditaueta/F");
  tree_->Branch("ditaupt",&ditaupt_,"ditaupt/F");
  
  tree_->Branch("taupt",&taupt_,"taupt/F");
  tree_->Branch("taueta",&taueta_,"taueta/F");
  tree_->Branch("tauehop",&tauehop_,"tauehop/F");
  tree_->Branch("taueop",&taueop_,"taueop/F");
  tree_->Branch("tautruth",&tautruth_,"tautruth/I");
  tree_->Branch("taudecaymode",&taudecaymode_,"taudecaymode/I");
  tree_->Branch("tautruth",&tautruth_,"tautruth/I");
  tree_->Branch("taudz",&taudz_,"taudz/F");
  tree_->Branch("taudxy",&taudxy_,"taudxy/F");

  tree_->Branch("mupt",&mupt_,"mupt/F");
  tree_->Branch("mueta",&mueta_,"mueta/F");
  tree_->Branch("muiso",&muiso_,"muiso/F");
  tree_->Branch("mudz",&mudz_,"mudz/F");
  tree_->Branch("mudxy",&mudxy_,"mudxy/F");

  tree_->Branch("metpt",&metpt_,"metpt/F");
  tree_->Branch("metphi",&metphi_,"metphi/F");
  tree_->Branch("transversemass",&transversemass_,"transversemass/F");

  tree_->Branch("njet",&njet_,"njet/I");
  tree_->Branch("leadJetPt",&leadJetPt_,"leadJetPt/F");
  tree_->Branch("leadJetEta",&leadJetEta_,"leadJetEta/F");
  tree_->Branch("diJetMass",&diJetMass_,"diJetMass/F");
  tree_->Branch("diJetDeltaEta",&diJetDeltaEta_,"diJetDeltaEta/F");
  tree_->Branch("diJetEta1Eta2",&diJetEta1Eta2_,"diJetEta1Eta2/F");

  tree_->Branch("categoryCh",&categoryCh_,"categoryCh/I");
  tree_->Branch("categoryMT",&categoryMT_,"categoryMT/I");
  tree_->Branch("categoryIso",&categoryIso_,"categoryIso/I");
  tree_->Branch("categorySM",&categorySM_,"categorySM/I");


  //counters
  counterall_=0;
  counterev_=0;
  countergen_=0;
  counterveto_=0;
  countereop_=0;
  countertaumatch_=0;
  countermumatch_=0;
  counterditau_=0;
  countertruth_=0;
  counter_=0;
}



bool TauMuFlatNtp::fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  if(!BaseFlatNtp::fillVariables(iEvent,iSetup)) return 0;


  //embedded samples generator weight
  embeddedGenWeight_=1.0;
  if(dataType_.compare("Embedded")==0){
    edm::Handle< double > embeddedGenWeight;
    iEvent.getByLabel(edm::InputTag("generator","weight",""),embeddedGenWeight);
    embeddedGenWeight_=*embeddedGenWeight;
  }  
 
  ///get the TauMu cands 
  iEvent.getByLabel(diTauTag_,diTauList_);

  //get the muons for the di-lepton veto
  iEvent.getByLabel(edm::InputTag("cmgMuonSel"),diLeptonVetoList_);
  
  ///get the gen Boson and set the genEventType
  genBoson_ = NULL;
  genBosonL1_ = NULL;
  genBosonL2_ = NULL;
  genEventType_=0;
  if(dataType_.compare("MC")==0){  
    iEvent.getByLabel(edm::InputTag("genParticlesStatus3"),genParticles_);    
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


  return 1;
}

bool TauMuFlatNtp::applySelections(){
  counterall_++;

  if(!BaseFlatNtp::applySelections()) return 0;
  counterev_++;

  //if none are selected returns 0
  diTauSel_=NULL;
  
  //apply gen level separation here
  if( sampleGenEventType_!=0
      && sampleGenEventType_!=genEventType_) return 0;
  countergen_++;

  ////other 
  if(vetoDiLepton()) return 0;
  counterveto_++;

  //apply full selections on Tau and Mu cands
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){    
      
    ////selections on the tau
    if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.) 
      if(cand->leg1().eOverP()<0.2) continue;
    countereop_++;
 
    bool matchtau=0;
    if(trigPaths_.size()==0)matchtau=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance()))
	  matchtau=1;
    }
    if(!matchtau)continue;
    countertaumatch_++;

    ////selections on the muon
    bool matchmu=0;
    if(trigPaths_.size()==0) matchmu=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg2().eta(),cand->leg2().phi(),(*path)->label(),(*path)->process()))
	  matchmu=1;
    }
    if(!matchmu)continue;
    countermumatch_++;

    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;
  counterditau_++;
  
  //choose the best candidate
  diTauSel_=&(*diTauSelList_.begin());
  double highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand)
    if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
      diTauSel_=&(*cand);
      highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
    }

  //truth match 
  truthEventType_=0;
  if(genBoson_){
    //Z-->ll
    if(abs(genBoson_->pdgId())==23){
      if(genBosonL1_&&genBosonL2_){
	if(((reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	    ||(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_))
	   && ((reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	       ||(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_))
	   ) truthEventType_=abs(genBosonL1_->pdgId())-10;
	else truthEventType_=6;
      }else truthEventType_=6;
    }

    //W-->lnu
    if(abs(genBoson_->pdgId())==24){
      if(genBosonL1_){
	if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL1_->eta(),genBosonL1_->phi())<deltaRTruth_)
	  truthEventType_=abs(genBosonL1_->pdgId());
	else truthEventType_=16;
      }else if(genBosonL2_){	  
	if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),genBosonL2_->eta(),genBosonL2_->phi())<deltaRTruth_)
	  truthEventType_=abs(genBosonL2_->pdgId());
	else truthEventType_=16;
      }else truthEventType_=16;
    }
  }


  if(sampleTruthEventType_>0)
    if(sampleTruthEventType_!=truthEventType_) return 0;
  countertruth_++;


  counter_++;
  return 1;
}

bool TauMuFlatNtp::fill(){
  
  BaseFlatNtp::fill();

  /////mu and tau trigger efficiency weight
  triggerEffWeight_=1.;
  selectionEffWeight_=1.;
  if(dataType_.compare("MC")==0
     || dataType_.compare("Embedded")==0){
    
    ///trigger corrections
    if(trigPaths_.size()>0){//trigger applied--> apply a correction factor
      triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	                  /triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
      triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	                  /triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }else{//no trigger applied --> apply efficiency
      triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
      triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }

    //id+isolation corrections
    selectionEffWeight_ *= selectionEff_.effCorrMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

  }
  
  eventweight_=pupWeight_*embeddedGenWeight_*triggerEffWeight_*selectionEffWeight_;

  mupt_=diTauSel_->leg2().pt();
  mueta_=diTauSel_->leg2().eta();
  muiso_=diTauSel_->leg2().relIso(0.5);
  mudz_=diTauSel_->leg2().dz();
  mudxy_=diTauSel_->leg2().dxy();

  taupt_=diTauSel_->leg1().pt();
  taueta_=diTauSel_->leg1().eta();
  taudz_=diTauSel_->leg1().dz();
  taudxy_=diTauSel_->leg1().dxy();
  tautruth_=truthMatchTau();
  tauehop_=diTauSel_->leg1().eOverP();
  taueop_=diTauSel_->leg1().leadChargedHadrEcalEnergy()/diTauSel_->leg1().p();
  taudecaymode_=diTauSel_->leg1().decayMode();

  ditaumass_=diTauSel_->mass();
  ditaucharge_=diTauSel_->charge();
  ditaueta_=diTauSel_->eta();
  ditaupt_=diTauSel_->pt();
  svfitmass_=diTauSel_->massSVFit();

  transversemass_=diTauSel_->mTLeg2();
  metpt_=diTauSel_->met().pt();
  metphi_=diTauSel_->met().phi();
  
  ///define categories
  categoryCh_=0;
  if(fabs(ditaucharge_)==0.)categoryCh_=1;
  if(fabs(ditaucharge_)==2.)categoryCh_=2;

  categoryMT_=0;
  if(transversemass_<=40.0)categoryMT_=1;
  if(40.0<transversemass_&&transversemass_<=60.0)categoryMT_=2;
  if(transversemass_>60.0)categoryMT_=3;

  categoryIso_=0;
  if( muiso_ <= 0.1) categoryIso_=1;
  if(0.1 < muiso_ &&  muiso_ <= 0.3) categoryIso_=2;
  if(0.3 < muiso_ &&  muiso_ <= 0.5) categoryIso_=3;
  if( muiso_ > 0.5) categoryIso_=4;


  //lepton clean the jet list
  fillPFJetListLC(diTauSel_);

  //jet quantities independent of SM category
  njet_=pfJetListLC_.size();
  if(pfJetListLC_.size()>=1){
    leadJetPt_=pfJetListLC_.begin()->pt();
    leadJetEta_=pfJetListLC_.begin()->eta();
  }
  if(pfJetListLC_.size()>=2){
    diJetMass_=(pfJetListLC_[0].p4()+pfJetListLC_[1].p4()).mass();
    diJetDeltaEta_=pfJetListLC_[0].eta() - pfJetListLC_[1].eta();
    diJetEta1Eta2_=pfJetListLC_[0].eta()*pfJetListLC_[1].eta();
  }


  //Define SM event category:
  categorySM_=-1;
  if(pfJetListLC_.size()>=2){//VBF: two leading jets must have  m>400, |eta1-eta2| < 4 and no other jet high pt in between    
    if((pfJetListLC_[0].p4()+pfJetListLC_[1].p4()).mass() > 400.0 
       && fabs(pfJetListLC_[0].eta() - pfJetListLC_[1].eta()) > 4.0 
       && pfJetListLC_[0].eta()*pfJetListLC_[1].eta() < 0.0
       ){
      bool pass=1;
      if(pfJetListLC_.size()>2){// check there is no additional central jet
	for(std::vector<cmg::PFJet>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	  if(pfJetListLC_[0].eta()<pfJetListLC_[1].eta()) 
	    if(pfJetListLC_[0].eta()<jet3->eta()&&jet3->eta()<pfJetListLC_[1].eta()) pass=0;
	  if(pfJetListLC_[0].eta()>pfJetListLC_[1].eta()) 
	    if(pfJetListLC_[1].eta()<jet3->eta()&&jet3->eta()<pfJetListLC_[0].eta()) pass=0;
	}
      }
      if(pass) categorySM_=2;
    }
  }
  if(categorySM_!=2 && pfJetListLC_.size()>=1){//Boosted: 1 jet with pt>150 and no other jets
    if(pfJetListLC_.begin()->pt()>=150.0)
      categorySM_=1;
  }

  if(categorySM_!=2 && categorySM_!=1 && pfJetListLC_.size()<=1){//SM0
    if(pfJetListLC_.size()==1){
      if(pfJetListLC_.begin()->pt()<150.0)
	categorySM_=0;
    }else categorySM_=0;
  }


  return 1;
}

void TauMuFlatNtp::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
  fillVariables(iEvent,iSetup);
  if(!applySelections()) return;
  fill();
  tree_->Fill();
}

void TauMuFlatNtp::fillPFJetListLC(const cmg::TauMu * cand){

  pfJetListLC_.clear();

  edm::Handle< std::vector<cmg::PFJet> > jetlist;
  iEvent_->getByLabel(edm::InputTag("cmgPFJetSel"),jetlist);
  std::vector<cmg::PFJet> cleanjetlist;
  for(std::vector<cmg::PFJet>::const_iterator jet=jetlist->begin(); jet!=jetlist->end(); ++jet){
    if(jet->pt()<30.0)continue;  
    if(fabs(jet->eta())>4.5)continue;        
    if(reco::deltaR(jet->eta(),jet->phi(),cand->leg1().eta(),cand->leg1().phi())<0.5) continue;     
    if(reco::deltaR(jet->eta(),jet->phi(),cand->leg2().eta(),cand->leg2().phi())<0.5) continue;   
    cleanjetlist.push_back(*jet);
  }
 
  //order by pt
  const cmg::PFJet * lastjet=NULL;
  for(std::vector<cmg::PFJet>::const_iterator jet1=cleanjetlist.begin(); jet1!=cleanjetlist.end(); ++jet1){
    const cmg::PFJet * highjet=NULL;
    for(std::vector<cmg::PFJet>::const_iterator jet2=cleanjetlist.begin(); jet2!=cleanjetlist.end(); ++jet2){
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

bool TauMuFlatNtp::vetoDiLepton(){
  
  int nmuons=0;
  for(std::vector<cmg::Muon>::const_iterator m=diLeptonVetoList_->begin(); m!=diLeptonVetoList_->end(); ++m){  
    if(m->pt()<15.0)continue;
    if(fabs(m->eta())>2.5)continue;
    if(m->relIso(0.5)>0.3)continue;    
    if(m->isGlobal()<0.5)continue; 
    if(m->dxy()>0.45)continue; 
    if(m->dz()>0.2)continue; 
    nmuons++;
  }
  
  if(nmuons>=2) return 1;
  return 0;
}

int TauMuFlatNtp::truthMatchTau(){
  if(!diTauSel_ )return 0;
  if(dataType_.compare("MC")!=0) return 0;

  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==11) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 1;
    if(abs(g->pdgId())==13) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 3;
    if(abs(g->pdgId())==15) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 5;
  }
  
  return 9;
}



void TauMuFlatNtp::endJob(){
  BaseFlatNtp::endJob();
  cout<<"counterall = "<<counterall_<<endl;
  cout<<"counterev = "<<counterev_<<endl;
  cout<<"countergen = "<<countergen_<<endl;
  cout<<"counterveto = "<<counterveto_<<endl;
  cout<<"countereop = "<<countereop_<<endl;
  cout<<"countertaumatch = "<<countertaumatch_<<endl;
  cout<<"countermumatch = "<<countermumatch_<<endl;
  cout<<"counterditau = "<<counterditau_<<endl;
  cout<<"countertruth = "<<countertruth_<<endl;
  cout<<"counter = "<<counter_<<endl;

}

#include "FWCore/Framework/interface/MakerMacros.h"
 
DEFINE_FWK_MODULE(TauMuFlatNtp);


