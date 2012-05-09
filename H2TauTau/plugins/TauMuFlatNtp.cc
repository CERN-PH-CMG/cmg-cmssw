#include "CMGTools/H2TauTau/plugins/TauMuFlatNtp.h"


TauMuFlatNtp::TauMuFlatNtp(const edm::ParameterSet & iConfig):
  BaseFlatNtp(iConfig),
  diTauSel_(0),
  triggerEffWeight_(0.),
  selectionEffWeight_(0.),
  embeddedGenWeight_(0.),
  sampleGenEventType_(0),
  sampleTruthEventType_(0),
  genEventType_(0),
  truthEventType_(0),
  deltaRTruth_(0.3),
  genBoson_(0),
  genBosonL1_(0),
  genBosonL2_(0)
{
  diTauTag_          = iConfig.getParameter<edm::InputTag>("diTauTag");
  genParticlesTag_          = iConfig.getParameter<edm::InputTag>("genParticlesTag");
  sampleGenEventType_= iConfig.getParameter<int>("sampleGenEventType");
  sampleTruthEventType_= iConfig.getParameter<int>("sampleTruthEventType");
  randsigma_=iConfig.getParameter<double>("randsigma");

}


TauMuFlatNtp::~TauMuFlatNtp(){

}


void TauMuFlatNtp::beginJob(){

  BaseFlatNtp::beginJob();
  //  tree_->Branch("",&_,"/F");
  tree_->Branch("nditau",&nditau_,"nditau/I");
  tree_->Branch("ditaumass",&ditaumass_,"ditaumass/F");
  tree_->Branch("ditaucharge",&ditaucharge_,"ditaucharge/I");
  tree_->Branch("svfitmass",&svfitmass_,"svfitmass/F");
  tree_->Branch("ditaueta",&ditaueta_,"ditaueta/F");
  tree_->Branch("ditaupt",&ditaupt_,"ditaupt/F");
  
  tree_->Branch("taupt",&taupt_,"taupt/F");
  tree_->Branch("taueta",&taueta_,"taueta/F");
  tree_->Branch("tauphi",&tauphi_,"tauphi/F");
  tree_->Branch("tauehop",&tauehop_,"tauehop/F");
  tree_->Branch("taueop",&taueop_,"taueop/F");
  tree_->Branch("tautruth",&tautruth_,"tautruth/I");
  tree_->Branch("taudecaymode",&taudecaymode_,"taudecaymode/I");
  tree_->Branch("taudz",&taudz_,"taudz/F");
  tree_->Branch("taudxy",&taudxy_,"taudxy/F");
  tree_->Branch("tauantie",&tauantie_,"tauantie/I");
  tree_->Branch("tauantimu",&tauantimu_,"tauantimu/I");
  tree_->Branch("tauisodisc",&tauisodisc_,"tauisodisc/I");
  tree_->Branch("tauisodiscmva",&tauisodiscmva_,"tauisodiscmva/I");
  tree_->Branch("tauiso",&tauiso_,"tauiso/F");
  tree_->Branch("taux",&taux_,"taux/F");
  tree_->Branch("tauy",&tauy_,"tauy/F");
  tree_->Branch("tauz",&tauz_,"tauz/F");
  tree_->Branch("taujetpt",&taujetpt_,"taujetpt/F");
  tree_->Branch("taujeteta",&taujeteta_,"taujeteta/F");

  tree_->Branch("mupt",&mupt_,"mupt/F");
  tree_->Branch("mueta",&mueta_,"mueta/F");
  tree_->Branch("muphi",&muphi_,"muphi/F");
  tree_->Branch("muiso",&muiso_,"muiso/F");
  tree_->Branch("muisomva",&muisomva_,"muisomva/F");
  tree_->Branch("mudz",&mudz_,"mudz/F");
  tree_->Branch("mudxy",&mudxy_,"mudxy/F");
  tree_->Branch("mux",&mux_,"mux/F");
  tree_->Branch("muy",&muy_,"muy/F");
  tree_->Branch("muz",&muz_,"muz/F");
  tree_->Branch("mujetpt",&mujetpt_,"mujetpt/F");
  tree_->Branch("mujeteta",&mujeteta_,"mujeteta/F");

  tree_->Branch("pfmetpt",&pfmetpt_,"pfmetpt/F");
  tree_->Branch("pfmetphi",&pfmetphi_,"pfmetphi/F");
  tree_->Branch("pftransversemass",&pftransversemass_,"pftransversemass/F");
  tree_->Branch("metpt",&metpt_,"metpt/F");
  tree_->Branch("metphi",&metphi_,"metphi/F");
  tree_->Branch("transversemass",&transversemass_,"transversemass/F");

  tree_->Branch("njet",&njet_,"njet/I");
  tree_->Branch("leadJetPt",&leadJetPt_,"leadJetPt/F");
  tree_->Branch("leadJetEta",&leadJetEta_,"leadJetEta/F");
  tree_->Branch("diJetMass",&diJetMass_,"diJetMass/F");
  tree_->Branch("diJetDeltaEta",&diJetDeltaEta_,"diJetDeltaEta/F");
  tree_->Branch("diJetEta1Eta2",&diJetEta1Eta2_,"diJetEta1Eta2/F");

  tree_->Branch("muLCleadJetPt",&muLCleadJetPt_,"muLCleadJetPt/F");
  tree_->Branch("muLCleadJetEta",&muLCleadJetEta_,"muLCleadJetEta/F");

  tree_->Branch("categoryCh",&categoryCh_,"categoryCh/I");
  tree_->Branch("categoryMT",&categoryMT_,"categoryMT/I");
  tree_->Branch("categoryIso",&categoryIso_,"categoryIso/I");
  tree_->Branch("categorySM",&categorySM_,"categorySM/I");


  //counters
  counterall_=0;
  counterev_=0;
  countergen_=0;
  counterveto_=0;
  countertaueop_=0;
  countertaumuveto_=0;
  countertaueveto_=0;
  countertauiso_=0;
  countertaumatch_=0;
  countermuid_=0;
  countermuiso_=0;
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
    if(!embeddedGenWeight.failedToGet())
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
    iEvent.getByLabel(genParticlesTag_,genParticles_);    
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


  diTauSelList_.clear();
  
  //Tau E/P cut
  std::vector<cmg::TauMu> tmpditaulist=*diTauList_;
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.)
      if(cand->leg1().eOverP()<0.2) continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaueop_++;

//   //tau vtx //vtx info not filled properly in V5_1_0
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     if(!(fabs(cand->leg1().dxy())<0.045)
//        || !(fabs(cand->leg1().dz())<0.2)
//        ) continue;    
//     diTauSelList_.push_back(*cand);
//   }
//   if(diTauSelList_.size()>0)countertauvtx_++;

  //Tau anti-mu cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("againstMuonTight")<0.5) continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaumuveto_++;

  //Tau anti-e cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("againstElectronLoose")<0.5) continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaueveto_++;


//   //isolation cut on the taus
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     //if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")<0.5)
//     if(cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") < 0.5) continue;
//     //if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")<0.5)
//     //if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")<0.5)
//     diTauSelList_.push_back(*cand);
//   }

  
  //Tau Trig-Match
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    bool matchtau=0;
    if(trigPaths_.size()==0)matchtau=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance()))
	  matchtau=1;
    }
    if(!matchtau)continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaumatch_++;
  
  //muon vtx 
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(!(fabs(cand->leg2().dxy())<0.045)
       || !(fabs(cand->leg2().dz())<0.2)
       ) continue;    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countermuvtx_++;


  //muion id cuts
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    //VBTF cuts
    if(!(cand->leg2().isGlobal())
       || !(cand->leg2().isTracker())
       || !(cand->leg2().numberOfValidTrackerHits() > 10)
       || !(cand->leg2().numberOfValidPixelHits() > 0)
       || !(cand->leg2().numberOfValidMuonHits() > 0)
       || !(cand->leg2().numberOfMatches() > 1)
       || !(cand->leg2().normalizedChi2() < 10)
       ) continue;    

    //PFMuon
    //if(!(cand->leg2().sourcePtr()->userFloat("isPFMuon")))continue;

    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countermuid_++;


//   //muon iso cut
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     if(cand->leg2().relIso(0.5)>0.1) continue;    
//     diTauSelList_.push_back(*cand);
//   }
//   if(diTauSelList_.size()>0)countermuiso_++;

  //muon trig-match
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    bool matchmu=0;
    if(trigPaths_.size()==0) matchmu=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg2().eta(),cand->leg2().phi(),(*path)->label(),(*path)->process()))
	  matchmu=1;
    }
    if(!matchmu) continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countermumatch_++;


  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    //-------mu iso cut here:
    if(cand->leg2().relIso(0.5)>0.1) continue;     
    //if(cand->leg2().sourcePtr()->userFloat("mvaIsoRings")<.xx) continue;     

    //------tau iso cut here
    //if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")<0.5)
    if(cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") < 0.5) continue;
    //if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")<0.5)
    //if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")<0.5)

    
    diTauSelList_.push_back(*cand);
  }

  if(diTauSelList_.size()>0)countertauiso_++;

  categoryIso_=1;//category gets set to "signal" by default
  nditau_=diTauSelList_.size();
  if(diTauSelList_.size()==0){//no isolated candidates were found, see if there are any anti-isolated ones
    categoryIso_=2;//category gets set to "sideband" if there was no candidate in the signal box
    for(std::vector<cmg::TauMu>::const_iterator cand = tmpditaulist.begin(); cand != tmpditaulist.end(); ++cand)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;//there was no event in the signal or sideband
  counterditau_++;
  
  //choose the best candidate
  nditau_=diTauSelList_.size();//keep track of the number of candidates per event
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
  muphi_=diTauSel_->leg2().phi();
  muiso_=diTauSel_->leg2().relIso(0.5);
  //muisomva_=(*(diTauSel_->leg2().sourcePtr()))->userFloat("mvaIsoRings");
  mudz_=diTauSel_->leg2().dz();
  mudxy_=diTauSel_->leg2().dxy();
  mux_=diTauSel_->leg2().vertex().x();
  muy_=diTauSel_->leg2().vertex().y();
  muz_=diTauSel_->leg2().vertex().z();

  taupt_=diTauSel_->leg1().pt();
  taueta_=diTauSel_->leg1().eta();
  tauphi_=diTauSel_->leg1().phi();
  taudz_=diTauSel_->leg1().dz();
  taudxy_=diTauSel_->leg1().dxy();
  tautruth_=truthMatchTau();
  tauehop_=diTauSel_->leg1().eOverP();
  taueop_=diTauSel_->leg1().leadChargedHadrEcalEnergy()/diTauSel_->leg1().p();
  taudecaymode_=diTauSel_->leg1().decayMode();
  taux_=diTauSel_->leg1().leadChargedHadrVertex().x();
  tauy_=diTauSel_->leg1().leadChargedHadrVertex().y();
  tauz_=diTauSel_->leg1().leadChargedHadrVertex().z();
  tauiso_=diTauSel_->leg1().relIso();

  tauantie_=0;
  if(diTauSel_->leg1().tauID("againstElectronLoose")>0.5)tauantie_=1;
  if(diTauSel_->leg1().tauID("againstElectronMedium")>0.5)tauantie_=2;
  if(diTauSel_->leg1().tauID("againstElectronTight")>0.5)tauantie_=3;
  tauantimu_=0;
  if(diTauSel_->leg1().tauID("againstMuonLoose")>0.5)tauantimu_=1;
  if(diTauSel_->leg1().tauID("againstMuonMedium")>0.5)tauantimu_=2;
  if(diTauSel_->leg1().tauID("againstMuonTight")>0.5)tauantimu_=3;
  tauisodisc_=0;
  if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=1;
  if(diTauSel_->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=2;
  if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=3;
  if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=4;
  tauisodiscmva_=diTauSel_->leg1().tauID("byRawIsoMVA");

  
  ditaumass_=diTauSel_->mass();
  ditaucharge_=diTauSel_->charge();
  ditaueta_=diTauSel_->eta();
  ditaupt_=diTauSel_->pt();
  svfitmass_=diTauSel_->massSVFit();


  ///get the jets //need the jets here because of randomization of mT
  edm::Handle< std::vector<cmg::PFJet> > fulljetlist;
  iEvent_->getByLabel(edm::InputTag("cmgPFJetSel"),fulljetlist);

  //find the jet matching to the mu
  mujetpt_=0.;
  mujeteta_=0.;
  for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
    if(reco::deltaR(jet->eta(),jet->phi(),diTauSel_->leg2().eta(),diTauSel_->leg2().phi())<0.3){
      mujetpt_=jet->pt();
      mujeteta_=jet->eta();
    }
  }

  //find the jet matching to the tau
  taujetpt_=0.;
  taujeteta_=0.;
  for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
    if(reco::deltaR(jet->eta(),jet->phi(),diTauSel_->leg1().eta(),diTauSel_->leg1().phi())<0.3){
      taujetpt_=jet->pt();
      taujeteta_=jet->eta();
    }
  }
  
  //apply pt and eta cuts on jets
  pfJetList_.clear();
  for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
    //Do we need to apply Jet energy corrections here ????
    if(jet->pt()<30.0)continue;  
    if(fabs(jet->eta())>4.5)continue;        
    pfJetList_.push_back(&(*jet));
  }

  //lepton clean the jet list //need to fill njet_ here 
  fillPFJetListLC(diTauSel_);
  njet_=pfJetListLC_.size();
  
  //smeared met and keep also the unsmeared one
  pfmetpt_=diTauSel_->met().pt();
  pfmetphi_=diTauSel_->met().phi();
  pftransversemass_=diTauSel_->mTLeg2();
  metpt_=diTauSel_->met().pt()*( (randsigma_>0. && njet_>0  ) ? randEngine_.Gaus(1.,randsigma_) : 1.);
  metphi_=diTauSel_->met().phi();
  transversemass_=sqrt(2*mupt_*metpt_*(1-cos(muphi_-metphi_)));
  

  ///define control samples
  categoryCh_=0;
  if(fabs(ditaucharge_)==0.)categoryCh_=1;
  if(fabs(ditaucharge_)==2.)categoryCh_=2;

  categoryMT_=0;
  if(transversemass_<=40.0)categoryMT_=1;
  if(40.0<transversemass_&&transversemass_<=60.0)categoryMT_=2;
  if(transversemass_>60.0)categoryMT_=3;

//   categoryIso_=0;
//   if( muiso_ <= 0.1) categoryIso_=1;
//   if(0.1 < muiso_ &&  muiso_ <= 0.3) categoryIso_=2;
//   if(0.3 < muiso_ &&  muiso_ <= 0.5) categoryIso_=3;
//   if( muiso_ > 0.5) categoryIso_=4;

 
  //jet quantities independent of SM category
  if(pfJetListLC_.size()>=1){
    leadJetPt_=pfJetListLC_[0]->pt();
    leadJetEta_=pfJetListLC_[0]->eta();
  }
  if(pfJetListLC_.size()>=2){
    diJetMass_=(pfJetListLC_[0]->p4()+pfJetListLC_[1]->p4()).mass();
    diJetDeltaEta_=pfJetListLC_[0]->eta() - pfJetListLC_[1]->eta();
    diJetEta1Eta2_=(pfJetListLC_[0]->eta())*(pfJetListLC_[1]->eta());
  }

  //Jets where only the muon has been removed
  fillPFJetListLepLC(diTauSel_);
  if(pfJetListLepLC_.size()>=1){
    muLCleadJetPt_=pfJetListLepLC_[0]->pt();
    muLCleadJetEta_=pfJetListLepLC_[0]->eta();
  }

  
  //////////////////////
  ////SM event categories //use mu-tau cleaned jet list
  //////////////////////
  categorySM_=-1;
  if(pfJetListLC_.size()>=2){//VBF: two leading jets must have  m>400, |eta1-eta2| < 4 and no other jet high pt in between    
    if((pfJetListLC_[0]->p4()+pfJetListLC_[1]->p4()).mass() > 400.0 
       && fabs(pfJetListLC_[0]->eta() - pfJetListLC_[1]->eta()) > 4.0 
       && pfJetListLC_[0]->eta()*pfJetListLC_[1]->eta() < 0.0
       ){
      bool pass=1;
      if(pfJetListLC_.size()>2){// check there is no additional central jet
	for(std::vector<const cmg::PFJet *>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	  if(pfJetListLC_[0]->eta()<pfJetListLC_[1]->eta()) 
	    if(pfJetListLC_[0]->eta()<(*jet3)->eta()&&(*jet3)->eta()<pfJetListLC_[1]->eta()) pass=0;
	  if(pfJetListLC_[0]->eta()>pfJetListLC_[1]->eta()) 
	    if(pfJetListLC_[1]->eta()<(*jet3)->eta()&&(*jet3)->eta()<pfJetListLC_[0]->eta()) pass=0;
	}
      }
      if(pass) categorySM_=2;
    }
  }
  if(categorySM_!=2 && pfJetListLC_.size()>=1){//Boosted: 1 jet with pt>150 and no other jets
    if(pfJetListLC_[0]->pt()>=150.0)
      categorySM_=1;
  }

  if(categorySM_!=2 && categorySM_!=1 && pfJetListLC_.size()<=1){//SM0
    if(pfJetListLC_.size()==1){
      if(pfJetListLC_[0]->pt()<150.0)
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

  //std::vector<cmg::PFJet> cleanjetlist;
  for(std::vector<const cmg::PFJet *>::const_iterator jet=pfJetList_.begin(); jet!=pfJetList_.end(); ++jet){
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg1().eta(),cand->leg1().phi())<0.3) continue;     
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg2().eta(),cand->leg2().phi())<0.3) continue;   
    //cleanjetlist.push_back(*jet);
    pfJetListLC_.push_back(*jet);
  }
 
//   //order by pt
//   const cmg::PFJet * lastjet=NULL;
//   for(std::vector<cmg::PFJet>::const_iterator jet1=cleanjetlist.begin(); jet1!=cleanjetlist.end(); ++jet1){
//     const cmg::PFJet * highjet=NULL;
//     for(std::vector<cmg::PFJet>::const_iterator jet2=cleanjetlist.begin(); jet2!=cleanjetlist.end(); ++jet2){
//       if(lastjet){
// 	if(jet2->pt()<lastjet->pt()){
// 	  if(highjet){
// 	    if(jet2->pt()>highjet->pt())highjet=&(*jet2);
// 	  }else highjet=&(*jet2);
// 	}
//       }else{
// 	if(highjet){
// 	  if(jet2->pt()>highjet->pt())highjet=&(*jet2);
// 	}else highjet=&(*jet2);
//       }    
//     }  
//     pfJetListLC_.push_back(*highjet);
//     lastjet=highjet;
//   }
  
}


void TauMuFlatNtp::fillPFJetListLepLC(const cmg::TauMu * cand){

  pfJetListLepLC_.clear();

  //std::vector<cmg::PFJet> cleanjetlist;
  for(std::vector<const cmg::PFJet *>::const_iterator jet=pfJetList_.begin(); jet!=pfJetList_.end(); ++jet){
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg2().eta(),cand->leg2().phi())<0.3) continue;   
    //cleanjetlist.push_back(*jet);
    pfJetListLepLC_.push_back(*jet);
  }
 
//   //order by pt
//   const cmg::PFJet * lastjet=NULL;
//   for(std::vector<cmg::PFJet>::const_iterator jet1=cleanjetlist.begin(); jet1!=cleanjetlist.end(); ++jet1){
//     const cmg::PFJet * highjet=NULL;
//     for(std::vector<cmg::PFJet>::const_iterator jet2=cleanjetlist.begin(); jet2!=cleanjetlist.end(); ++jet2){
//       if(lastjet){
// 	if(jet2->pt()<lastjet->pt()){
// 	  if(highjet){
// 	    if(jet2->pt()>highjet->pt())highjet=&(*jet2);
// 	  }else highjet=&(*jet2);
// 	}
//       }else{
// 	if(highjet){
// 	  if(jet2->pt()>highjet->pt())highjet=&(*jet2);
// 	}else highjet=&(*jet2);
//       }    
//     }  
//     pfJetListLepLC_.push_back(*highjet);
//     lastjet=highjet;
//   }
  
}

bool TauMuFlatNtp::vetoDiLepton(){
  
  int nmuons=0;
  for(std::vector<cmg::Muon>::const_iterator m=diLeptonVetoList_->begin(); m!=diLeptonVetoList_->end(); ++m){  
    if(m->pt()<=15.0)continue;
    if(fabs(m->eta())>=2.5)continue;
    if(m->relIso(0.5)>=0.3)continue;    
    if(m->isGlobal()<0.5)continue; 
    if(fabs(m->dxy())>=0.045)continue; 
    if(fabs(m->dz())>=0.2)continue; 
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
  cout<<"countertaueop = "<<countertaueop_<<endl;
  cout<<"countertauvtx = "<<countertauvtx_<<endl;
  cout<<"countertaumuveto = "<<countertaumuveto_<<endl;
  cout<<"countertaueveto = "<<countertaueveto_<<endl;
  cout<<"countertauiso = "<<countertauiso_<<endl;
  cout<<"countertaumatch = "<<countertaumatch_<<endl;
  cout<<"countermuvtx = "<<countermuvtx_<<endl;
  cout<<"countermuid = "<<countermuid_<<endl;
  cout<<"countermuiso = "<<countermuiso_<<endl;
  cout<<"countermumatch = "<<countermumatch_<<endl;
  cout<<"counterditau = "<<counterditau_<<endl;
  cout<<"countertruth = "<<countertruth_<<endl;
  cout<<"counter = "<<counter_<<endl;

}

#include "FWCore/Framework/interface/MakerMacros.h"
 
DEFINE_FWK_MODULE(TauMuFlatNtp);


