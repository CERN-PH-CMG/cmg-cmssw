#include "CMGTools/TauTest/plugins/CMGTauEfficiency.h"


CMGTauEfficiency::CMGTauEfficiency(const edm::ParameterSet & iConfig):
  file_(0),
  tree_(0)
{
  effOrFake_=iConfig.getParameter<int>("effOrFake");
  chamonix_ =iConfig.getParameter<bool>("chamonix");

  ///Fill the OOT weight histo
  OOTWeightFall11File_=iConfig.getParameter<string>("OOTWeightFall11File");
  OOTWeightChamonixFile_=iConfig.getParameter<string>("OOTWeightChamonixFile");
  TFile RootFileOOTWeightFall11(OOTWeightFall11File_.c_str(),"read");
  TFile RootFileOOTWeightChamonix(OOTWeightChamonixFile_.c_str(),"read");
  TH2F * H2Fall11=(TH2F*)RootFileOOTWeightFall11.Get("MUON_Denom/MUON_Denom_h_pup_VS_pu");
  TH2F * H2Chamonix=(TH2F*)RootFileOOTWeightChamonix.Get("MUON_Denom/MUON_Denom_h_pup_VS_pu");
  if(!H2Fall11 || !H2Chamonix){
    cout<<" OOT reweighting histograms not found"<<endl;
  }else {
    H2ChamonixOOTWeight_=new TH2F("H2OOTWeight","",H2Fall11->GetNbinsX(),H2Fall11->GetXaxis()->GetXmin(),H2Fall11->GetXaxis()->GetXmax(),H2Fall11->GetNbinsY(),H2Fall11->GetYaxis()->GetXmin(),H2Fall11->GetYaxis()->GetXmax());
    for(int bx=1;bx<=H2Fall11->GetNbinsX();bx++){
      for(int by=1;by<=H2Fall11->GetNbinsY();by++){
	if(H2Chamonix->GetBinContent(bx,by)>0&&H2Fall11->GetBinContent(bx,by)>0){
	  float w=H2Fall11->GetBinContent(bx,by)/H2Chamonix->GetBinContent(bx,by);
	  float we=w*sqrt(pow(H2Fall11->GetBinError(bx,by)/H2Fall11->GetBinContent(bx,by),2) + pow(H2Chamonix->GetBinError(bx,by)/H2Chamonix->GetBinContent(bx,by),2));
	  if(we/w<0.5){
	    H2ChamonixOOTWeight_->SetBinContent(bx,by,w);
	    H2ChamonixOOTWeight_->SetBinError(bx,by,we);
	  }
	}
      }
    }
  }

}

CMGTauEfficiency::~CMGTauEfficiency(){
  delete file_;
  delete H2ChamonixOOTWeight_;
}

void CMGTauEfficiency::beginJob(){
  file_ = new edm::Service<TFileService>();

  tree_=(*file_)->make<TTree>("tree","tree");
  tree_->Branch("npv",&npv_,"npv/I");
  tree_->Branch("npv1",&npv1_,"npv1/I");
  tree_->Branch("gentaupt",&gentaupt_,"gentaupt/F");
  tree_->Branch("gentaueta",&gentaueta_,"gentaueta/F");
  tree_->Branch("genjetpt",&genjetpt_,"genjetpt/F");
  tree_->Branch("genjeteta",&genjeteta_,"genjeteta/F");
  tree_->Branch("genjetphi",&genjetphi_,"genjetphi/F");
  tree_->Branch("genjetcharge",&genjetcharge_,"genjetcharge/I");
  tree_->Branch("recojetdeltaR",&recojetdeltaR_,"recojetdeltaR/F");
  tree_->Branch("recojetdeltaRToPFJet",&recojetdeltaRToPFJet_,"recojetdeltaRToPFJet/F");
  tree_->Branch("genjetnmatching",&genjetnmatching_,"genjetnmatching/I");
  tree_->Branch("recojetpt",&recojetpt_,"recojetpt/F");
  tree_->Branch("recojeteta",&recojeteta_,"recojeteta/F");
  tree_->Branch("recojetiso",&recojetiso_,"recojetiso/I");
  tree_->Branch("recojetiso2",&recojetiso2_,"recojetiso2/I");
  tree_->Branch("recojetiso3",&recojetiso3_,"recojetiso3/I");
  tree_->Branch("recojetantimu",&recojetantimu_,"recojetantimu/F");
  tree_->Branch("recojetantie",&recojetantie_,"recojetantie/F");
  tree_->Branch("ootweight",&ootweight_,"ootweight/F");
  //  tree_->Branch("",&,"/");
  
  counterall_=0;

  countztt_=0;
  countttoverlap_=0;
  counttjet1_=0;
  counttjet2_=0;
  counttlep_=0;

  goodevtcounter_=0;
  ngenjets_=0;
  nrecojets_=0;
}


void CMGTauEfficiency::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  counterall_++;


  //get the genparticles 
  iEvent.getByLabel(edm::InputTag("genParticles"),genParticles_);  
  iEvent.getByLabel(edm::InputTag("genParticlesStatus3"),genParticles3_);  
  iEvent.getByLabel(edm::InputTag("genJetSel"),genJets_);  
  iEvent.getByLabel(edm::InputTag("genLeptonsStatus1"),genLeptons1_);  


  
  genJetPt_[0] =0.;//these quantities need to be reset since they are used to select the gen taus
  genJetEta_[0]=0.;
  genJetPt_[1] =0.;
  genJetEta_[1]=0.;
  
  if(effOrFake_==1){//Use Z--> TauTau for efficiency

    //findGenTauJets();//wrong method using the general gen jet collection
    if(chamonix_)findGenTauVisPartChamonix();
    else findGenTauVisPart();

  } else if(effOrFake_==2){
    findGenFakeJets(); //Use jets after removing any leptons 
  } else return;
  if(genJetPt_[0]==0. && genJetPt_[1]==0.) return; //no jets were found
  goodevtcounter_++;


  /////////////////get the number of pile up 
  edm::Handle< std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;  
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
    int BX = PVI->getBunchCrossing();    
    if(BX == 0) npv_ = PVI->getPU_NumInteractions();
    if(BX == 1) npv1_ = PVI->getPU_NumInteractions();
  }
  
  ///Fill the Chamonix OOT weight to compare to Fall11
  ootweight_=0.;
  if(npv_<50&&npv1_<50)ootweight_=H2ChamonixOOTWeight_->GetBinContent(npv_+1,npv1_+1);  
  

  //get the reco taus
  edm::Handle< std::vector<cmg::Tau> > candvec;
  iEvent.getByLabel(edm::InputTag("cmgTauSel"),candvec);
    
  //get the jets for checks
  edm::Handle< std::vector<cmg::PFJet> > pfjets;
  iEvent.getByLabel(edm::InputTag("cmgPFJetSel"),pfjets);

  ///for each gen jet see if there is a reco tau
  for(int i=0;i<2;i++){
    if(genJetPt_[i]>0.){
      ngenjets_++;

      gentaupt_=genTauPt_[i];
      gentaueta_=genTauEta_[i];
      genjetpt_=genJetPt_[i];
      genjeteta_=genJetEta_[i];
      genjetphi_=genJetPhi_[i];
      genjetcharge_=genJetCh_[i];
      genjetnmatching_=nMatchingToTau_[i];
      recojetpt_=0;
      recojeteta_=0;
      recojetiso_=0;
      recojetiso2_=0;
      recojetiso3_=0;
      recojetantimu_=0;
      recojetantie_=0;

      //find a reco tau
      float dR=0.;
      const cmg::Tau * recojet=0;
      for(std::vector<cmg::Tau>::const_iterator recoTau=candvec->begin(); recoTau!=candvec->end(); ++recoTau){
	dR=reco::deltaR(recoTau->eta(),recoTau->phi(),genjeteta_,genjetphi_);
	if(dR>0.15) continue;
	recojet=&(*recoTau);
      }
	
      if(recojet){
	nrecojets_++;

	recojetpt_=recojet->pt();
	recojeteta_=recojet->eta();
	recojetdeltaR_=dR;

	///Isolation 
	recojetiso_=1;//decayModeFinding
	if(recojet->tauID("byVLooseCombinedIsolationDeltaBetaCorr") > 0.5)	recojetiso_=2;
	if(recojet->tauID("byLooseCombinedIsolationDeltaBetaCorr") > 0.5) 	recojetiso_=3;
	if(recojet->tauID("byMediumCombinedIsolationDeltaBetaCorr") > 0.5)	recojetiso_=4;
	if(recojet->tauID("byTightCombinedIsolationDeltaBetaCorr") > 0.5) 	recojetiso_=5;

	//Old HPS Isolation discriminators
	recojetiso2_=1;//decayModeFinding
	if(recojet->tauID("byVLooseIsolation") > 0.5)	recojetiso2_=2;
	if(recojet->tauID("byLooseIsolation") > 0.5) 	recojetiso2_=3;
	if(recojet->tauID("byMediumIsolation") > 0.5)	recojetiso2_=4;
	if(recojet->tauID("byTightIsolation") > 0.5) 	recojetiso2_=5;

	//HPS Isolation with no deltaBeta Corr
	recojetiso3_=1;//decayModeFinding
	if(recojet->tauID("byVLooseIsolationDeltaBetaCorr") > 0.5)	recojetiso3_=2;
	if(recojet->tauID("byLooseIsolationDeltaBetaCorr") > 0.5) 	recojetiso3_=3;
	if(recojet->tauID("byMediumIsolationDeltaBetaCorr") > 0.5)	recojetiso3_=4;
	if(recojet->tauID("byTightIsolationDeltaBetaCorr") > 0.5) 	recojetiso3_=5;

	//anti mu discriminators
	if(recojet->tauID("againstMuonLoose") > 0.5)	recojetantimu_=1;
	if(recojet->tauID("againstMuonMedium") > 0.5) 	recojetantimu_=2;
	if(recojet->tauID("againstMuonTight") > 0.5)	recojetantimu_=3;

	//anti e discriminators
	if(recojet->tauID("againstElectronLoose") > 0.5)	recojetantie_=1;
	if(recojet->tauID("againstElectronMedium") > 0.5) 	recojetantie_=2;
	if(recojet->tauID("againstElectronTight") > 0.5)	recojetantie_=3;
	if(recojet->tauID("againstElectronMVA") > 0.5) 	        recojetantie_=4;

	//check the reco tau against the reco jet
	recojetdeltaRToPFJet_=0.;	
	for(std::vector<cmg::PFJet>::const_iterator recoPFjet=pfjets->begin(); recoPFjet!=pfjets->end(); ++recoPFjet){
	  float dltR=reco::deltaR(recoPFjet->eta(),recoPFjet->phi(),recojet->eta(),recojet->phi());
	  if(dltR>0.15) continue;
	  recojetdeltaRToPFJet_=dltR;
	}

      }

      tree_->Fill();
    }
  }

}



void CMGTauEfficiency::findGenTauVisPart(){


//   ///print all the gen particles
//   for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
//     //if its a tau follow the mothers
//     if(abs(g->pdgId())==15){
//       cout<<" gen : "<<endl;
//       cout<<" : "<<g->pdgId()<<"("<<g->status()<<")";
//       if(g->mother()){
// 	cout<<" : "<<g->mother()->pdgId()<<"("<<g->mother()->status()<<")";
// 	if(g->mother()->mother()){
// 	  cout<<" : "<<g->mother()->mother()->pdgId()<<"("<<g->mother()->mother()->status()<<")";
// 	  if(g->mother()->mother()->mother()){
// 	    cout<<" : "<<g->mother()->mother()->mother()->pdgId()<<"("<<g->mother()->mother()->mother()->status()<<")";
// 	    if(g->mother()->mother()->mother()->mother()){
// 	      cout<<" : "<<g->mother()->mother()->mother()->mother()->pdgId()<<"("<<g->mother()->mother()->mother()->mother()->status()<<")";
// 	    }
// 	  }
// 	}
//       }
//       cout<<endl;
//       cout<<" nDau "<<g->numberOfDaughters()<<endl;
//       //for(std::vector<reco::GenParticle>::const_iterator d=g->begin();d!=g->end();++d)
//       for(unsigned int i=0; i<g->numberOfDaughters(); i++){
// 	const reco::Candidate * d = g->daughter(i); 
// 	cout<<"   : "<<d->pdgId()<<"("<<d->status()<<")";

// 	cout<<" => [";
// 	for(unsigned int j=0; j<d->numberOfDaughters(); j++){
// 	  const reco::Candidate * d2 = d->daughter(j); 
// 	  cout<<" : "<<d2->pdgId()<<"("<<d2->status()<<")";
// 	}
// 	cout<<" ]";
//       }
//       cout<<endl;
      

//     }
//   }


  const reco::GenParticle * genBosonLeg_[2];
  genBosonLeg_[0]=0;
  genBosonLeg_[1]=0;
  
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(g->pdgId()==15 && g->status()==2 ) genBosonLeg_[0]=&(*g);
    if(g->pdgId()==-15 && g->status()==2 ) genBosonLeg_[1]=&(*g);
  }
  if(!genBosonLeg_[0] || !genBosonLeg_[1])return;
  countztt_++;

  //remove leptonic decays
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if( (abs(g->pdgId())==11  || abs(g->pdgId())==13) && g->status()==1  ){

      if(g->mother()){
	if(genBosonLeg_[0]==g->mother()){//leptons from tau have the tau as mother directly
	  genBosonLeg_[0]=0;
	  counttlep_++;
	}
	if(genBosonLeg_[1]==g->mother()){
	  genBosonLeg_[1]=0;
	  counttlep_++;
	}
      }
      
    }
  }  
  if(!genBosonLeg_[0] || !genBosonLeg_[1])return;



  //find the visible part of the taus by subtracting the neutrino
  const reco::GenParticle * nu_[2];
  nu_[0]=0;
  nu_[1]=0;
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==16){ //tau neutrino    
      if(g->mother()){
	if(g->mother()==genBosonLeg_[0]){ //check if the particle came from the first tau
	  nu_[0]=&(*g);
	} 
	if(g->mother()==genBosonLeg_[1]){ //check if the particle came from the second tau
	  nu_[1]=&(*g);
	} 
      }
    }
  }
  if(!nu_[0] || !nu_[1]) return;//both neutrinos must be found


  if(genBosonLeg_[0]){
    genJetPt_[0] =(genBosonLeg_[0]->p4()-nu_[0]->p4()).pt();
    genJetEta_[0]=(genBosonLeg_[0]->p4()-nu_[0]->p4()).eta();
    genJetPhi_[0]=(genBosonLeg_[0]->p4()-nu_[0]->p4()).phi();
    genJetCh_[0] =genBosonLeg_[0]->charge();
  }
  if(genBosonLeg_[1]){
    genJetPt_[1] =(genBosonLeg_[1]->p4()-nu_[1]->p4()).pt();
    genJetEta_[1]=(genBosonLeg_[1]->p4()-nu_[1]->p4()).eta();
    genJetPhi_[1]=(genBosonLeg_[1]->p4()-nu_[1]->p4()).phi();
    genJetCh_[1] =genBosonLeg_[1]->charge();
  }


}



void CMGTauEfficiency::findGenTauVisPartChamonix(){


  const reco::GenParticle * genBosonLeg_[2];
  genBosonLeg_[0]=0;
  genBosonLeg_[1]=0;
  
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(g->pdgId()==15 && g->status()==2 ) genBosonLeg_[0]=&(*g);
    if(g->pdgId()==-15 && g->status()==2 ) genBosonLeg_[1]=&(*g);
  }
  if(!genBosonLeg_[0] || !genBosonLeg_[1])return;
  countztt_++;

  //remove leptonic decays
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if( (abs(g->pdgId())==11  || abs(g->pdgId())==13) && g->status()==1  ){
      if(reco::deltaR(g->eta(),g->phi(),genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi()) < 0.2 ){
	genBosonLeg_[0]=0;
	counttlep_++;
	break;
      }
    }
  }  
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if( (abs(g->pdgId())==11  || abs(g->pdgId())==13) && g->status()==1  ){
      if(reco::deltaR(g->eta(),g->phi(),genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi()) < 0.2 ){
	genBosonLeg_[1]=0;
	counttlep_++;
	break;
      }
    }
  }  
  if(!genBosonLeg_[0] || !genBosonLeg_[1])return;


  //find the visible part of the taus by subtracting the neutrino
  const reco::GenParticle * nu_[2];
  nu_[0]=0;
  nu_[1]=0;
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==16){ //tau neutrino 
      float dr0=reco::deltaR(g->eta(),g->phi(),genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi());
      if( dr0 < 0.2){
	nu_[0]=&(*g);
	deltaRCheck_[0]=dr0;
      } 
      float dr1=reco::deltaR(g->eta(),g->phi(),genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi());
      if( dr1 < 0.2){ 
	nu_[1]=&(*g);
	deltaRCheck_[1]=dr1;      
      } 
    }
  }
  if(!nu_[0] || !nu_[1]) return;//both neutrinos must be found


  if(genBosonLeg_[0]){
    genJetPt_[0] =(genBosonLeg_[0]->p4()-nu_[0]->p4()).pt();
    genJetEta_[0]=(genBosonLeg_[0]->p4()-nu_[0]->p4()).eta();
    genJetPhi_[0]=(genBosonLeg_[0]->p4()-nu_[0]->p4()).phi();
    genJetCh_[0] =genBosonLeg_[0]->charge();
  }
  if(genBosonLeg_[1]){
    genJetPt_[1] =(genBosonLeg_[1]->p4()-nu_[1]->p4()).pt();
    genJetEta_[1]=(genBosonLeg_[1]->p4()-nu_[1]->p4()).eta();
    genJetPhi_[1]=(genBosonLeg_[1]->p4()-nu_[1]->p4()).phi();
    genJetCh_[1] =genBosonLeg_[1]->charge();
  }



}




void CMGTauEfficiency::findGenFakeJets(){
  //find the leading and subleading gen jets //Note these jets already have a pt cut >15
  
  const cmg::PhysicsObjectWithPtr<edm::Ptr<reco::GenJet> > * genJet_[2]; 
  genJet_[0]=0;
  genJet_[1]=0;

  //first remove the leptons from the jets
  std::vector<const cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > * > cleanGenJets;
  for(std::vector<cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > >::const_iterator g=genJets_->begin(); g!=genJets_->end(); ++g){    
    bool islepton=0;

    for(std::vector<reco::GenParticle>::const_iterator l=genParticles3_->begin(); l!=genParticles3_->end(); ++l)
      if( abs(l->pdgId())==11  || abs(l->pdgId())==13 || abs(l->pdgId())==15 )
        if(reco::deltaR(g->eta(),g->phi(),l->eta(),l->phi())<0.3) islepton=1;

    ////Dont remove leptons from B decays
//     for(std::vector<reco::GenParticle>::const_iterator l=genLeptons1_->begin(); l!=genLeptons1_->end(); ++l)    
//       if( abs(l->pdgId())==11  || abs(l->pdgId())==13 || abs(l->pdgId())==15 )
//         if(reco::deltaR(g->eta(),g->phi(),l->eta(),l->phi())<0.3) islepton=1;
    
    if(islepton)continue;
    cleanGenJets.push_back(&(*g));
  } 
  

  if(cleanGenJets.size()>0){//leading jet
    genJet_[0]=cleanGenJets[0];
    for(std::vector<const cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > *>::const_iterator g=cleanGenJets.begin(); g!=cleanGenJets.end(); ++g)
      if((*g)->pt()>genJet_[0]->pt())genJet_[0]=*g;
  }
  if(cleanGenJets.size()>1){//subleading jet
    genJet_[1]=cleanGenJets[0];
    for(std::vector<const cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > *>::const_iterator g=cleanGenJets.begin(); g!=cleanGenJets.end(); ++g)
      if((*g)->pt()>genJet_[1]->pt()&&(*g)->pt()<genJet_[0]->pt())genJet_[1]=*g;
  }




  if(genJet_[0]){
    genJetPt_[0]=genJet_[0]->pt();
    genJetEta_[0]=genJet_[0]->eta();
    genJetPhi_[0]=genJet_[0]->phi();
    genJetCh_[0]=genJet_[0]->charge();
  }
  if(genJet_[1]){
    genJetPt_[1]=genJet_[1]->pt();
    genJetEta_[1]=genJet_[1]->eta();
    genJetPhi_[1]=genJet_[1]->phi();
    genJetCh_[1]=genJet_[1]->charge();
  }

 
}



// void CMGTauEfficiency::findGenTauJets(){

//   const reco::GenParticle * genBosonLeg_[2];
//   genBosonLeg_[0]=0;
//   genBosonLeg_[1]=0;
//   const cmg::PhysicsObjectWithPtr<edm::Ptr<reco::GenJet> > * genJet_[2]; 
//   genJet_[0]=0;
//   genJet_[1]=0;
//   nMatchingToTau_[0]=0;//number of gen jets matching to the tau
//   nMatchingToTau_[1]=0;

//   //get the leptons from the genBoson
//   for(std::vector<reco::GenParticle>::const_iterator g=genParticles3_->begin(); g!=genParticles3_->end(); ++g){    
//     if(g->pdgId()==15 ) genBosonLeg_[0]=&(*g);
//     if(g->pdgId()==-15 ) genBosonLeg_[1]=&(*g);
//   }
//   if(!genBosonLeg_[0] || !genBosonLeg_[1])return;//User only Z-->tau tau        
//   countztt_++;

//   //taus which overlap each other may affect efficiency
//   if(genBosonLeg_[0] && genBosonLeg_[1])
//     if(reco::deltaR(genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi(),genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi())<0.5){
//       genBosonLeg_[0]=0;
//       genBosonLeg_[1]=0;
//     }
//   if(!genBosonLeg_[0] || !genBosonLeg_[1])return;
//   countttoverlap_++;

//   genTauPt_[0]=genBosonLeg_[0]->pt();
//   genTauPt_[1]=genBosonLeg_[1]->pt();

//   //find the gen jets corresponding to the taus    
//   for(std::vector<cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > >::const_iterator g=genJets_->begin(); g!=genJets_->end(); ++g){    
//     float dr1=reco::deltaR(genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi(),g->eta(),g->phi());
//     if(dr1<0.15){//check DeltaR distribution: peak is contained below 0.15
//       genJet_[0]=&(*g);
//       deltaRCheck_[0]=dr1;
//       nMatchingToTau_[0]++;
//       counttjet1_++;
//     }
//     float dr2=reco::deltaR(genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi(),g->eta(),g->phi());
//     if(dr2<0.15){
//       genJet_[1]=&(*g);
//       deltaRCheck_[1]=dr2;
//       nMatchingToTau_[1]++;
//       counttjet2_++;
//     }
//   } 
  

//   //dont use taus decaying leptonically
//   for(std::vector<reco::GenParticle>::const_iterator g=genLeptons1_->begin(); g!=genLeptons1_->end(); ++g){    
//     if( abs(g->pdgId())==11  || abs(g->pdgId())==13  ){
//       if(reco::deltaR(genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi(),g->eta(),g->phi())<0.3){
// 	genJet_[0]=0;
// 	counttlep_++;
//       }
//       if(reco::deltaR(genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi(),g->eta(),g->phi())<0.3){
// 	genJet_[1]=0;
// 	counttlep_++;
//       }
//     }
//   }


//   if(genJet_[0]){
//     genJetPt_[0]=genJet_[0]->pt();
//     genJetEta_[0]=genJet_[0]->eta();
//     genJetPhi_[0]=genJet_[0]->phi();
//     genJetCh_[0]=genJet_[0]->charge();
//   }
//   if(genJet_[1]){
//     genJetPt_[1]=genJet_[1]->pt();
//     genJetEta_[1]=genJet_[1]->eta();
//     genJetPhi_[1]=genJet_[1]->phi();
//     genJetCh_[1]=genJet_[1]->charge();
//   }

// }




void CMGTauEfficiency::endJob(){
  cout<<"counterall = "<< counterall_<<endl;
  cout<<"goodevtcounter = "<< goodevtcounter_<<endl;
  cout<<"countztt = "<< countztt_ <<endl;
  cout<<"countttoverlap = "<< countttoverlap_ <<endl;
  cout<<"counttjet1 = "<< counttjet1_ <<endl;
  cout<<"counttjet2 = "<< counttjet2_ <<endl;
  cout<<"counttlep = "<< counttlep_ <<endl;
  cout<<"ngenjets = "<< ngenjets_ <<endl;
  cout<<"nrecojets = "<< nrecojets_ <<endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CMGTauEfficiency);


//   //find the visible part of the taus from the pions and kaons
//   PtEtaPhiELorentzVectorF visPart[2];
//   for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
//     if(abs(g->pdgId())==111 //pi0
//        || abs(g->pdgId())==211 //pi+
//        || abs(g->pdgId())==321 //K+
//        || abs(g->pdgId())==310 //KS
//        || abs(g->pdgId())==130 //KL
//        || abs(g->pdgId())==311 //K0
//        ){

//       //check if the particle came from the first tau
//       if(g->mother()){
// 	if(g->mother()==genBosonLeg_[0]){
// 	  visPart[0]+=g->p4();
// 	} else if(g->mother()->mother()){
// 	  if(g->mother()->mother()==genBosonLeg_[0]){
// 	    visPart[0]+=g->p4();
// 	  }else if(g->mother()->mother()->mother()){
// 	    if(g->mother()->mother()->mother()==genBosonLeg_[0]){
// 	      visPart[0]+=g->p4();
// 	    }
// 	  }
// 	}
//       }

//       //check if the particle came from the second tau
//       if(g->mother()){
// 	if(g->mother()==genBosonLeg_[1]){
// 	  visPart[1]+=g->p4();
// 	} else if(g->mother()->mother()){
// 	  if(g->mother()->mother()==genBosonLeg_[1]){
// 	    visPart[1]+=g->p4();
// 	  }else if(g->mother()->mother()->mother()){
// 	    if(g->mother()->mother()->mother()==genBosonLeg_[1]){
// 	      visPart[1]+=g->p4();
// 	    }
// 	  }
// 	}
//       }
	      
//     }
//   }


//   if(genBosonLeg_[0]){
//     genJetPt_[0] =visPart[0]->pt();
//     genJetEta_[0]=visPart[0]->eta();
//     genJetPhi_[0]=visPart[0]->phi();
//     genJetCh_[0] =genBosonLeg_[0]->charge();
//   }
//   if(genBosonLeg_[1]){
//     genJetPt_[1] =visPart_[1]->pt();
//     genJetEta_[1]=visPart_[1]->eta();
//     genJetPhi_[1]=visPart_[1]->phi();
//     genJetCh_[1] =genBosonLeg_[1]->charge();
//   }

