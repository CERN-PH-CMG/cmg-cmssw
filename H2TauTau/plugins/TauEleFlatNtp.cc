#include "CMGTools/H2TauTau/plugins/TauEleFlatNtp.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"
#include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"


TauEleFlatNtp::TauEleFlatNtp(const edm::ParameterSet & iConfig):
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
  genBosonL2_(0),
  corrector_(iConfig.getParameter<std::string>("fileCorrectTo"))
{
  diTauTag_               = iConfig.getParameter<edm::InputTag>("diTauTag");
  genParticlesTag_        = iConfig.getParameter<edm::InputTag>("genParticlesTag");
  pfJetListTag_           = iConfig.getParameter<edm::InputTag>("pfJetListTag");
  diMuonVetoListTag_      = iConfig.getParameter<edm::InputTag>("diMuonVetoListTag");
  sampleGenEventType_     = iConfig.getParameter<int>("sampleGenEventType");
  sampleTruthEventType_   = iConfig.getParameter<int>("sampleTruthEventType");
  randsigma_              = iConfig.getParameter<double>("randsigma");

  recoilCorreciton_ =  iConfig.getParameter<int>("recoilCorrection");
  fileCorrectTo_ = iConfig.getParameter<std::string>("fileCorrectTo");
  fileZmmData_ = iConfig.getParameter<std::string>("fileZmmData");
  fileZmmMC_ = iConfig.getParameter<std::string>("fileZmmMC");

  runSVFit_  =  iConfig.getParameter<int>("runSVFit");
}


TauEleFlatNtp::~TauEleFlatNtp(){

}


void TauEleFlatNtp::beginJob(){

  BaseFlatNtp::beginJob();
  //  tree_->Branch("",&_,"/F");
  tree_->Branch("nditau",&nditau_,"nditau/I");
  tree_->Branch("ditaumass",&ditaumass_,"ditaumass/F");
  tree_->Branch("ditaucharge",&ditaucharge_,"ditaucharge/I");
  tree_->Branch("svfitmass",&svfitmass_,"svfitmass/F");
  tree_->Branch("ditaueta",&ditaueta_,"ditaueta/F");
  tree_->Branch("ditaupt",&ditaupt_,"ditaupt/F");
  
  tree_->Branch("taumass",&taumass_,"taumass/F");
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
  tree_->Branch("tauisomva",&tauisomva_,"tauisomva/F");
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
  tree_->Branch("metpt",&metpt_,"metpt/D");
  tree_->Branch("metphi",&metphi_,"metphi/D");
  tree_->Branch("transversemass",&transversemass_,"transversemass/F");

  tree_->Branch("njet",&njet_,"njet/I");
  tree_->Branch("leadJetPt",&leadJetPt_,"leadJetPt/F");
  tree_->Branch("leadJetRawFactor",&leadJetRawFactor_,"leadJetRawFactor/F");
  tree_->Branch("leadJetEta",&leadJetEta_,"leadJetEta/F");
  tree_->Branch("subleadJetPt",&subleadJetPt_,"subleadJetPt/F");
  tree_->Branch("subleadJetEta",&subleadJetEta_,"subleadJetEta/F");
  tree_->Branch("subleadJetRawFactor",&subleadJetRawFactor_,"subleadJetRawFactor/F");
  tree_->Branch("diJetMass",&diJetMass_,"diJetMass/F");
  tree_->Branch("diJetDeltaEta",&diJetDeltaEta_,"diJetDeltaEta/F");
  tree_->Branch("diJetEta1Eta2",&diJetEta1Eta2_,"diJetEta1Eta2/F"); 
  tree_->Branch("njetingap",&njetingap_,"njetingap/I");
  tree_->Branch("nbjet",&nbjet_,"nbjet/I");
  tree_->Branch("leadBJetPt",&leadBJetPt_,"leadBJetPt/F");
  tree_->Branch("leadBJetEta",&leadBJetEta_,"leadBJetEta/F");

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
  counterpresel_=0;
  countermuvtx_=0;
  countermuid_=0;
  countermuiso_=0;
  countermumatch_=0;
  countertaueop_=0;
  countertauvtx_=0;
  countertaumuveto_=0;
  countertaueveto_=0;
  countertauiso_=0;
  countertaumatch_=0;
  counterditau_=0;
  counterbestcand_=0;
  countertruth_=0;
  counter_=0;
}



bool TauEleFlatNtp::fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  if(!BaseFlatNtp::fillVariables(iEvent,iSetup)) return 0;


  //embedded samples generator weight
  embeddedGenWeight_=1.0;
  if(dataType_==0){
    edm::Handle< double > embeddedGenWeight;
    iEvent.getByLabel(edm::InputTag("generator","weight",""),embeddedGenWeight);
    if(!embeddedGenWeight.failedToGet())
      embeddedGenWeight_=*embeddedGenWeight;
  }  
 
  ///get the TauEle cands 
  iEvent.getByLabel(diTauTag_,diTauList_);

  //get the muons for the di-lepton veto
  iEvent.getByLabel(diMuonVetoListTag_,diLeptonVetoList_);
  
  ///get the gen Boson and set the genEventType
  genBoson_ = NULL;
  genBosonL1_ = NULL;
  genBosonL2_ = NULL;
  genEventType_=0;
  if(dataType_==0){  
    iEvent.getByLabel(genParticlesTag_,genParticles_);    
    for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
      //cout<<g->pdgId()<<" "<<g->p4().pt()<<endl;
      if((abs(g->pdgId())==23 || abs(g->pdgId())==24 ||  abs(g->pdgId())==25 ) && genBoson_==NULL )
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

bool TauEleFlatNtp::applySelections(){
  counterall_++;

  //if none are selected returns 0
  diTauSel_=NULL;

  if(!BaseFlatNtp::applySelections()) return 0;
  counterev_++;

  //apply gen level separation here
  if( sampleGenEventType_!=0 && sampleGenEventType_!=genEventType_) return 0;
  countergen_++;

  ////other 
  if(vetoDiLepton()) return 0;
  counterveto_++;

  std::vector<cmg::TauEle> tmpditaulist=*diTauList_;
  diTauSelList_.clear();

  ///basic skims which should have been applied in H2TAUTAU step  
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().pt() > 20.0
       && fabs(cand->leg1().eta()) < 2.3
       && cand->leg1().tauID("decayModeFinding") > 0.5
       && cand->leg2().pt() > 20.0
       && fabs(cand->leg2().eta()) < 2.1
       )     
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0) counterpresel_++;

  //muon vtx 
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if( fabs(cand->leg2().dxy())>0.045)continue;
    if( fabs(cand->leg2().dz())>0.1 )continue;
    
    //-----------------------------------------------------------------------------------------------------------------------------Fix
//     //track.isNonnull() && track.isAvailable()
//     if(!((*(cand->leg2().sourcePtr()))->innerTrack().isNonnull()))continue;
//     if(!((*(cand->leg2().sourcePtr()))->innerTrack().isAvailable()))continue;

//     float dxy= (*(cand->leg2().sourcePtr()))->innerTrack()->dxy(PV_->position()) ;
//     float dz = (*(cand->leg2().sourcePtr()))->innerTrack()->dz(PV_->position());
//     if(fabs(dxy) > 0.045 ) continue;
//     if(fabs(dz)  > 0.2 ) continue;
    //cout<<eventid_<<" "<<(*(cand->leg2().sourcePtr()))->innerTrack()->dz((*(vertices_->begin())).position())<<endl;
    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countermuvtx_++;


  //muion id cuts
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/H2TauTau/python/proto/analyzers/TauEleAnalyzer.py?revision=1.12&view=markup
    //https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#2012_Baseline_Selection
    if(!(cand->leg2().numberOfHits()<=0
	 &&cand->leg2().passConversionVeto()==1
	 ))continue; 

    if((*(cand->leg2().sourcePtr()))->isEB()){
      if(!(cand->leg2().sigmaIetaIeta() <0.01
	   &&cand->leg2().deltaPhiSuperClusterTrackAtVtx()<0.80
	   &&cand->leg2().deltaEtaSuperClusterTrackAtVtx()<0.0017
	   &&cand->leg2().hadronicOverEm()<0.15
	   ))continue; 
    }else if((*(cand->leg2().sourcePtr()))->isEE()){
      if(!(cand->leg2().sigmaIetaIeta() <0.03
	   &&cand->leg2().deltaPhiSuperClusterTrackAtVtx()<0.70
	   &&cand->leg2().deltaEtaSuperClusterTrackAtVtx()<0.01
	   &&cand->leg2().hadronicOverEm()<0.07
	   ))continue; 
    }
    
// Use non-triggering electron MVA
// Category 	                             LOOSE ID (EMu) 	TIGHT ID (ETau)
// Cat1: pT < 20 GeV, abs(eta) < 0.8 	mva_output > 0.925 	 
// Cat2: pT < 20 GeV, 0.8 < abs(eta) < 1.479 	mva_output > 0.915 	 
// Cat3: pT < 20 GeV, abs(eta) > 1.479 	mva_output > 0.965
// Cat4: pT > 20 GeV, abs(eta) < 0.8 	mva_output > 0.905 	mva_output > 0.925
// Cat5: pT > 20 GeV, 0.8 < abs (eta) < 1.479 	mva_output > 0.955 	mva_output > 0.975
// Cat6: pT > 20 GeV, abs(eta) > 1.479 	mva_output > 0.975 	mva_output > 0.995 

    //mva id  
    float mvaid=cand->leg2().mvaNonTrigV0();
    if(fabs(cand->leg2().eta())<0.8)
      if(!(mvaid>0.925))continue; 
    if(0.8<=fabs(cand->leg2().eta())&&fabs(cand->leg2().eta())<1.479)
      if(!(mvaid>0.975))continue;
    if(1.479<=fabs(cand->leg2().eta()))
      if(!(mvaid>0.995))continue; 
    

    diTauSelList_.push_back(*cand);
      
  }
  if(diTauSelList_.size()>0)countermuid_++;


//   //muon iso cut
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     if(cand->leg2().relIso(0.5)>0.1) continue;    
//     diTauSelList_.push_back(*cand);
//   }
//   if(diTauSelList_.size()>0)countermuiso_++;

  //muon trig-match
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    bool matchmu=0;
    if(trigPaths_.size()==0) matchmu=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg2().eta(),cand->leg2().phi(),(*path)->label(),(*path)->process()))
	  matchmu=1;
    }
    
    if(matchmu) 
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countermumatch_++;

  //Tau E/P cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.)
      if(cand->leg1().eOverP()<0.2)
	continue;
    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaueop_++;

  //tau vtx
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    //if(fabs(cand->leg1().dxy())>0.045) continue;
    //if(fabs(cand->leg1().dz())>0.2 ) continue;

    //can't access the reco::track from the cmg::Tau
    //methods from here http://cmslxr.fnal.gov/lxr/source/DataFormats/TrackReco/interface/TrackBase.h#063
    reco::TrackBase::Point  vtx = cand->leg1().leadChargedHadrVertex();   
    math::XYZTLorentzVector p4 = cand->leg1().p4();
    float dxy = ( - (vtx.x()-PV_->position().x()) *  p4.y() + (vtx.y()-PV_->position().y()) *  p4.x() ) /  p4.pt();    
    float dz  = (vtx.z()-PV_->position().z()) - ((vtx.x()-PV_->position().x()) * p4.x()+(vtx.y()-PV_->position().y())*  p4.y())/ p4.pt() *  p4.z()/ p4.pt();
    if(fabs(dxy)>0.045)continue;
    if(fabs(dz)>0.2)continue;
      
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertauvtx_++;

  //Tau anti-mu cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("againstMuonLoose")>0.5)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaumuveto_++;

  //Tau anti-e cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("againstElectronMedium")>0.5
       &&cand->leg1().tauID("againstElectronMVA")>0.5
       )
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaueveto_++;


//   //isolation cut on the taus
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     //if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")<0.5)
//     if(cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") < 0.5) continue;
//     //if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")<0.5)
//     //if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")<0.5)
//     diTauSelList_.push_back(*cand);
//   }
//   if(diTauSelList_.size()>0)countertauiso_++;
  
  //Tau Trig-Match
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    bool matchtau=0;
    if(trigPaths_.size()==0)matchtau=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance()))
	  matchtau=1;
    }
    
    if(matchtau)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()>0)countertaumatch_++;
  
  /////////Isolation cuts
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    

    if(
       //-------iso cut here
       cand->leg2().chargedAllIsoWithConeVeto() < 0.1 
       //cand->leg2().relIso(0.5) < 0.1

       //-------tau iso cut here
       //if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")<0.5)
       // && cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") > 0.5
       //if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")<0.5)
       //if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")<0.5)       
       && cand->leg1().tauID("byLooseIsoMVA")>0.5

       ){
      diTauSelList_.push_back(*cand);
    }
  }
  if(diTauSelList_.size()>0)counterditau_++;

  categoryIso_=1;//category gets set to "signal" by default
  nditau_=diTauSelList_.size();
  if(diTauSelList_.size()==0){//no isolated candidates were found, see if there are any anti-isolated ones
    categoryIso_=2;//category gets set to "sideband" if there was no candidate in the signal box
    for(std::vector<cmg::TauEle>::const_iterator cand = tmpditaulist.begin(); cand != tmpditaulist.end(); ++cand)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0) return 0;//there was no event in the signal or sideband
  
  
  //choose the best candidate
  nditau_=diTauSelList_.size();//keep track of the number of candidates per event
  diTauSel_=&(*diTauSelList_.begin());
  double highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
  for(std::vector<cmg::TauEle>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand)
    if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
      diTauSel_=&(*cand);
      highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
    }
  
  counterbestcand_++;

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

bool TauEleFlatNtp::fill(){
  
  BaseFlatNtp::fill();

  /////mu and tau trigger efficiency weight
  triggerEffWeight_=1.;
  selectionEffWeight_=1.;
  if(dataType_==0 || dataType_==0){
    
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
  //muiso_=diTauSel_->leg2().relIsoAllChargedDB05(); 
  muiso_=diTauSel_->leg2().relIso(0.5);
  muisomva_=(*(diTauSel_->leg2().sourcePtr()))->userFloat("mvaIsoRings");
  mudz_=diTauSel_->leg2().dz();
  mudxy_=diTauSel_->leg2().dxy();
  mux_=diTauSel_->leg2().vertex().x();
  muy_=diTauSel_->leg2().vertex().y();
  muz_=diTauSel_->leg2().vertex().z();

  taumass_=diTauSel_->leg1().p4().M();
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
  tauiso_=diTauSel_->leg1().relIso(0.5);
  tauisomva_=diTauSel_->leg1().tauID("byRawIsoMVA");

  tauantie_=0;
  if(diTauSel_->leg1().tauID("againstElectronLoose")>0.5)tauantie_=1;
  if(diTauSel_->leg1().tauID("againstElectronMedium")>0.5)tauantie_=2;
  if(diTauSel_->leg1().tauID("againstElectronTight")>0.5)tauantie_=3;
  tauantiemva_=0;
  if(diTauSel_->leg1().tauID("againstElectronMVA")>0.5)tauantiemva_=1;
  tauantimu_=0;
  if(diTauSel_->leg1().tauID("againstMuonLoose")>0.5)tauantimu_=1;
  if(diTauSel_->leg1().tauID("againstMuonMedium")>0.5)tauantimu_=2;
  if(diTauSel_->leg1().tauID("againstMuonTight")>0.5)tauantimu_=3;
  tauisodisc_=0;
  if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=1;
  if(diTauSel_->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=2;
  if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=3;
  if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")>0.5)tauisodisc_=4;
  tauisodiscmva_=0;
  if(diTauSel_->leg1().tauID("byLooseIsoMVA")>0.5)tauisodiscmva_=1;
  if(diTauSel_->leg1().tauID("byMediumIsoMVA")>0.5)tauisodiscmva_=2;
  if(diTauSel_->leg1().tauID("byTightIsoMVA")>0.5)tauisodiscmva_=3;

  
  ditaumass_=diTauSel_->mass();
  ditaucharge_=diTauSel_->charge();
  ditaueta_=diTauSel_->eta();
  ditaupt_=diTauSel_->pt();
  svfitmass_=diTauSel_->massSVFit();

  pfmetpt_=diTauSel_->met().pt();
  pfmetphi_=diTauSel_->met().phi();
  pftransversemass_=sqrt(2*mupt_*pfmetpt_*(1-cos(muphi_-pfmetphi_)));
  metpt_=pfmetpt_;//default values 
  metphi_=pfmetphi_;//default values

  ///get the jets //need the jets here because of randomization of mT
  edm::Handle< std::vector<cmg::PFJet> > fulljetlist;
  iEvent_->getByLabel(pfJetListTag_,fulljetlist);
  
  //apply pt and eta cuts on jets
  pfJetList_.clear();
  for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
    if(jet->pt()<30.0)continue;  
    if(fabs(jet->eta())>4.5)continue; 

    //Loose PF Jet id 
    ///https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    if(!(jet->component(5).fraction() < 0.99
	 &&jet->component(4).fraction() < 0.99
	 &&jet->nConstituents() > 1
	 &&(jet->component(1).fraction() > 0 || abs(jet->eta()) > 2.4)
	 &&(jet->component(1).number() > 0 || abs(jet->eta()) > 2.4)
	 &&(jet->component(2).fraction() < 0.99 || abs(jet->eta()) > 2.4)	 
	 ))continue;

    //if(jet->getSelection("cuts_looseJetId")<0.5)continue;

    //PU jet id 
    if(!(jet->passPuJetId("full",PileupJetIdentifier::kLoose))) continue;

    pfJetList_.push_back(&(*jet));
  }

  //lepton clean the jet list //need to fill njet_ here 
  fillPFJetListLC(diTauSel_,&pfJetList_,&pfJetListLC_);
  njet_=pfJetListLC_.size();


  //run MVA met here 
  //../../Common/python/miscProducers/mvaMET/mvaMETTauEle_cfi.py  
  

  //Also the list cleaned only with the muon //njetLepLC needed in recoil correction
  fillPFJetListLepLC(diTauSel_,&pfJetList_,&pfJetListLepLC_);
  int njetLepLC_=pfJetListLepLC_.size();
  
  ///Apply recoil correction here
  if(recoilCorreciton_>0){
    corrector_.addDataFile( fileZmmData_);
    corrector_.addMCFile( fileZmmMC_);
    double u1 = 0;
    double u2 = 0;
    double fluc = 0;
    
    double lepPt  =diTauSel_->pt();
    double lepPhi =diTauSel_->phi();
    int jetMult = njet_;
    if(recoilCorreciton_==2){//for W+jets
      lepPt  =mupt_;
      lepPhi =muphi_;
      jetMult = njetLepLC_;
    }

    //cout<<fileCorrectTo_<<" "<<metpt_<<" "<<metphi_<<endl;
    corrector_.CorrectType1( metpt_, metphi_,  genBoson_->pt(), genBoson_->phi(),  lepPt, lepPhi,  u1, u2, fluc, jetMult );
    //cout<<fileCorrectTo_<<" "<<metpt_<<" "<<metphi_<<endl;
    //reco::Candidate::PolarLorentzVector newMETP4( met,0, metphi, 0);
  }
  reco::Candidate::PolarLorentzVector metCorrP4( metpt_,0,metphi_, 0);
  //smeared met and keep also the unsmeared one
  //metpt_=diTauSel_->met().pt()*( (randsigma_>0. && njet_>0  ) ? randEngine_.Gaus(1.,randsigma_) : 1.);
  //metphi_=diTauSel_->met().phi();
  transversemass_=sqrt(2*mupt_*metCorrP4.pt()*(1-cos(muphi_-metCorrP4.phi())));
  

  ///!!!!!!Need to update to new MVA Met significance matrix-----------------------------------------------------------------------------Fix
  if(runSVFit_==1){  //old svfit  
    edm::Handle< cmg::METSignificance > metsig;
    iEvent_->getByLabel(edm::InputTag("pfMetSignificance"),metsig); 
    NSVfitStandalone2011::Vector measuredMET( metCorrP4.x(), metCorrP4.y(), 0);
    std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons;
    NSVfitStandalone2011::LorentzVector p1(diTauSel_->leg1().p4());
    measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kHadDecay,p1));    
    NSVfitStandalone2011::LorentzVector p2(diTauSel_->leg2().p4());
    measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kLepDecay,p2));
    NSVfitStandaloneAlgorithm2011 algo(measuredTauLeptons,measuredMET,metsig->significance(),0);
    algo.maxObjFunctionCalls(5000);
    algo.fit();
    svfitmass_  = algo.fittedDiTauSystem().mass();
  }

  if(runSVFit_==2){    //new svfit
    edm::Handle< cmg::METSignificance > metsig;
    iEvent_->getByLabel(edm::InputTag("pfMetSignificance"),metsig); 
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, diTauSel_->leg2().p4()));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, diTauSel_->leg1().p4()));
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, metCorrP4.Vect(), *(metsig->significance()), 0);
    algo.addLogM(false);
    algo.integrate();
    svfitmass_ = algo.getMass();
  }


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

  //jet quantities independent of SM category
  if(pfJetListLC_.size()>=1){
    leadJetPt_=pfJetListLC_[0]->pt();
    leadJetEta_=pfJetListLC_[0]->eta();
    leadJetRawFactor_=pfJetListLC_[0]->rawFactor();
  }
  if(pfJetListLC_.size()>=2){
    subleadJetPt_=pfJetListLC_[1]->pt();
    subleadJetEta_=pfJetListLC_[1]->eta();
    subleadJetRawFactor_=pfJetListLC_[1]->rawFactor();
    diJetMass_=(pfJetListLC_[0]->p4()+pfJetListLC_[1]->p4()).mass();
    diJetDeltaEta_=fabs(pfJetListLC_[0]->eta() - pfJetListLC_[1]->eta());
    diJetEta1Eta2_=(pfJetListLC_[0]->eta())*(pfJetListLC_[1]->eta());
  }

  //Jets where only the muon has been removed
  if(pfJetListLepLC_.size()>=1){
    muLCleadJetPt_=pfJetListLepLC_[0]->pt();
    muLCleadJetEta_=pfJetListLepLC_[0]->eta();
  }


  //make a list of b-tagged jets  jet->btag(6) 
  pfJetListBTag_.clear();
  for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
    //cout<<jet->btag(6)<<" "<<jet->btag("combinedSecondaryVertexBJetTags")<<endl;
    //if(jet->btag(6)<0.679)continue;//CSV medium
    if(jet->btag("combinedSecondaryVertexBJetTags")<0.679)continue;//CSV medium
    if(jet->pt()<20.0)continue;  
    if(fabs(jet->eta())>2.4)continue; //
    pfJetListBTag_.push_back(&(*jet));
  }
  fillPFJetListLC(diTauSel_,&pfJetListBTag_,&pfJetListBTagLC_);
  nbjet_=pfJetListBTagLC_.size();
  if(nbjet_>0){
    leadBJetBTagProb_=pfJetListBTagLC_[0]->btag(6);
    leadBJetPt_=pfJetListBTagLC_[0]->pt();
    leadBJetEta_=pfJetListBTagLC_[0]->eta();
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
      njetingap_=0;
      if(pfJetListLC_.size()>2){// check there is no additional central jet
	for(std::vector<const cmg::PFJet *>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	  if(pfJetListLC_[0]->eta()<pfJetListLC_[1]->eta()) 
	    if(pfJetListLC_[0]->eta()<(*jet3)->eta()&&(*jet3)->eta()<pfJetListLC_[1]->eta()) njetingap_++;
	  if(pfJetListLC_[0]->eta()>pfJetListLC_[1]->eta()) 
	    if(pfJetListLC_[1]->eta()<(*jet3)->eta()&&(*jet3)->eta()<pfJetListLC_[0]->eta()) njetingap_++;
	}
      }
      if(njetingap_==0) categorySM_=2;
    }
  }
  if(categorySM_!=2 && pfJetListLC_.size()>=1){//Boosted: 1 jet with pt>150 and no other jets
    if(pfJetListLC_[0]->pt()>=150.0)///*pfJetListLC_[0]->rawFactor()
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

void TauEleFlatNtp::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
  fillVariables(iEvent,iSetup);
  if(!applySelections()) return;
  fill();
  tree_->Fill();
}

void TauEleFlatNtp::fillPFJetListLC(const cmg::TauEle * cand,std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC){

  listLC->clear();

  //std::vector<cmg::PFJet> cleanjetlist;
  for(std::vector<const cmg::PFJet *>::const_iterator jet=list->begin(); jet!=list->end(); ++jet){
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg1().eta(),cand->leg1().phi())<0.5) continue;     
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg2().eta(),cand->leg2().phi())<0.5) continue;   
    //cleanjetlist.push_back(*jet);
    listLC->push_back(*jet);
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


void TauEleFlatNtp::fillPFJetListLepLC(const cmg::TauEle * cand,std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC){

  listLC->clear();

  //std::vector<cmg::PFJet> cleanjetlist;
  for(std::vector<const cmg::PFJet *>::const_iterator jet=list->begin(); jet!=list->end(); ++jet){
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg2().eta(),cand->leg2().phi())<0.5) continue;   
    //cleanjetlist.push_back(*jet);
    listLC->push_back(*jet);
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

bool TauEleFlatNtp::vetoDiLepton(){
  
  bool muminus=0;
  bool muplus=0;
  for(std::vector<cmg::Electron>::const_iterator m=diLeptonVetoList_->begin(); m!=diLeptonVetoList_->end(); ++m){  
    if(m->pt()<=15.0)continue;
    if(fabs(m->eta())>=2.5)continue;
    if(fabs(m->dxy())>=0.045)continue; 
    if(fabs(m->dz())>=0.2)continue; 
    
    ///ID
    if(!(m->numberOfHits()<=0
	 &&m->passConversionVeto()==1
	 ))continue; 

    if((*(m->sourcePtr()))->isEB()){
      if(!(m->sigmaIetaIeta() <0.01
	   &&m->deltaPhiSuperClusterTrackAtVtx()<0.80
	   &&m->deltaEtaSuperClusterTrackAtVtx()<0.0017
	   &&m->hadronicOverEm()<0.15
	   ))continue; 
    }else if((*(m->sourcePtr()))->isEE()){
      if(!(m->sigmaIetaIeta() <0.03
	   &&m->deltaPhiSuperClusterTrackAtVtx()<0.70
	   &&m->deltaEtaSuperClusterTrackAtVtx()<0.01
	   &&m->hadronicOverEm()<0.07
	   ))continue; 
    }
    

// Cat1: pT < 20 GeV, abs(eta) < 0.8 	mva_output > 0.925 	 
// Cat2: pT < 20 GeV, 0.8 < abs(eta) < 1.479 	mva_output > 0.915 	 
// Cat3: pT < 20 GeV, abs(eta) > 1.479 	mva_output > 0.965
// Cat4: pT > 20 GeV, abs(eta) < 0.8 	mva_output > 0.905 	mva_output > 0.925
// Cat5: pT > 20 GeV, 0.8 < abs (eta) < 1.479 	mva_output > 0.955 	mva_output > 0.975
// Cat6: pT > 20 GeV, abs(eta) > 1.479 	mva_output > 0.975 	mva_output > 0.995 


    //mva id  
    float mvaid=m->mvaNonTrigV0();
    if(m->pt()<20.){
      if(fabs(m->eta())<0.8)
	if(!(mvaid>0.925))continue; 
      if(0.8<=fabs(m->eta())&&fabs(m->eta())<1.479)
	if(!(mvaid>0.915))continue;
      if(1.479<=fabs(m->eta()))
	if(!(mvaid>0.965))continue; 
    } 
    if(m->pt()>20.){
      if(fabs(m->eta())<0.8)
	if(!(mvaid>0.905))continue; 
      if(0.8<=fabs(m->eta())&&fabs(m->eta())<1.479)
	if(!(mvaid>0.955))continue;
      if(1.479<=fabs(m->eta()))
	if(!(mvaid>0.975))continue; 
    }
    
    //isolation
    if(m->relIso(0.5)>=0.3)continue;    

    if(m->charge()==-1)muminus=1;
    if(m->charge()==1)muplus=1;
  }
  
  if(muminus&&muplus) return 1;
  return 0;
}

int TauEleFlatNtp::truthMatchTau(){
  if(!diTauSel_ )return 0;
  if(dataType_!=0) return 0;

  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==11) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 1;
    if(abs(g->pdgId())==13) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 3;
    if(abs(g->pdgId())==15) if(reco::deltaR(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),g->eta(),g->phi())<deltaRTruth_) return 5;
  }
  
  return 9;
}



void TauEleFlatNtp::endJob(){
  BaseFlatNtp::endJob();
  cout<<"counterall = "<<counterall_<<endl;
  cout<<"counterev = "<<counterev_<<endl;
  cout<<"countergen = "<<countergen_<<endl;
  cout<<"counterveto = "<<counterveto_<<endl;
  cout<<"counterpresel = "<<counterpresel_<<endl;
  cout<<"countermuvtx = "<<countermuvtx_<<endl;
  cout<<"countermuid = "<<countermuid_<<endl;
  cout<<"countermuiso = "<<countermuiso_<<endl;
  cout<<"countermumatch = "<<countermumatch_<<endl;
  cout<<"countertaueop = "<<countertaueop_<<endl;
  cout<<"countertauvtx = "<<countertauvtx_<<endl;
  cout<<"countertaumuveto = "<<countertaumuveto_<<endl;
  cout<<"countertaueveto = "<<countertaueveto_<<endl;
  cout<<"countertauiso = "<<countertauiso_<<endl;
  cout<<"countertaumatch = "<<countertaumatch_<<endl;
  cout<<"counterditau = "<<counterditau_<<endl;
  cout<<"counterbestcand = "<<counterbestcand_<<endl;
  cout<<"countertruth = "<<countertruth_<<endl;
  cout<<"counter = "<<counter_<<endl;

}

#include "FWCore/Framework/interface/MakerMacros.h"
 
DEFINE_FWK_MODULE(TauEleFlatNtp);


