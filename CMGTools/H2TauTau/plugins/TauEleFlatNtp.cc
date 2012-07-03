
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
   btagWP_(0.679),
   btagWeight_(1),   
   sampleGenEventType_(0),
   sampleTruthEventType_(0),
   genEventType_(0),
   truthEventType_(0),
   deltaRTruth_(0.3),
   genBoson_(0),
   genBosonL1_(0),
   genBosonL2_(0),
   corrector_(iConfig.getParameter<std::string>("fileCorrectTo")),
   mvaWeights_(iConfig.getParameter<std::string>("mvaWeights")),
   reader_(mvaWeights_.c_str())   
 {

   diTauTag_               = iConfig.getParameter<edm::InputTag>("diTauTag");
   cout<<"diTauTag_  : "<<diTauTag_.label()<<endl;
   
   genParticlesTag_        = iConfig.getParameter<edm::InputTag>("genParticlesTag");
   cout<<" genParticlesTag_  : "<<genParticlesTag_.label()<<endl;
   
   pfJetListTag_           = iConfig.getParameter<edm::InputTag>("pfJetListTag");
   cout<<" pfJetListTag_ : "<<pfJetListTag_.label()<<endl;
   
   diMuonVetoListTag_      = iConfig.getParameter<edm::InputTag>("diMuonVetoListTag");
   cout<<" diMuonVetoListTag : "<<diMuonVetoListTag_.label()<<endl;
   
   sampleGenEventType_     = iConfig.getParameter<int>("sampleGenEventType");
   cout<<"sampleGenEventType  : "<<sampleGenEventType_<<endl;
   
   sampleTruthEventType_   = iConfig.getParameter<int>("sampleTruthEventType");
   cout<<" sampleTruthEventType : "<<sampleTruthEventType_<<endl;
   
   randsigma_              = iConfig.getParameter<double>("randsigma");
   cout<<"randsigma_  : "<<randsigma_<<endl;

   recoilCorreciton_ =  iConfig.getParameter<int>("recoilCorrection");
   cout<<"recoilCorreciton_  : "<<recoilCorreciton_<<endl;
   
   fileZmmData_ = iConfig.getParameter<std::string>("fileZmmData");
   cout<<" fileZmmData_ : "<<fileZmmData_.c_str()<<endl;
   corrector_.addDataFile( fileZmmData_);

   fileZmmMC_ = iConfig.getParameter<std::string>("fileZmmMC");
   cout<<"fileZmmMC_  : "<<fileZmmMC_.c_str()<<endl;
   corrector_.addMCFile( fileZmmMC_);

   recoiliScale_ = iConfig.getParameter<double>("recoiliScale");
   cout<<"recoiliScale_   : "<<recoiliScale_<<endl;

   metType_ = iConfig.getParameter<int>("metType");
   cout<<"metType_  : "<<metType_<<endl;

   runSVFit_  =  iConfig.getParameter<int>("runSVFit");
   cout<<" runSVFit_ : "<<runSVFit_<<endl;


   runSVFit_  =  iConfig.getParameter<int>("runSVFit");
 }


TauEleFlatNtp::~TauEleFlatNtp(){

}


void TauEleFlatNtp::beginJob(){

  BaseFlatNtp::beginJob();
  //  tree_->Branch("",&_,"/F");

  tree_->Branch("triggerEffWeight",&triggerEffWeight_,"triggerEffWeight/F"); 
  tree_->Branch("selectionEffWeight",&selectionEffWeight_,"selectionEffWeight/F"); 
  tree_->Branch("embeddedGenWeight",&embeddedGenWeight_,"embeddedGenWeight/F"); 
  tree_->Branch("btagEffWeight",&btagEffWeight_,"btagEffWeight/F");

  tree_->Branch("genbosonmass",&genbosonmass_,"genbosonmass/F");
  tree_->Branch("genbosonpt",&genbosonpt_,"genbosonpt/F");
  tree_->Branch("genbosonphi",&genbosonphi_,"genbosonphi/F");

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
  tree_->Branch("mumvaid",&mumvaid_,"mumvaid/F");

  tree_->Branch("pfmetpt",&pfmetpt_,"pfmetpt/F");
  tree_->Branch("pfmetphi",&pfmetphi_,"pfmetphi/F");
  tree_->Branch("pftransversemass",&pftransversemass_,"pftransversemass/F");
  tree_->Branch("metpt",&metpt_,"metpt/D");
  tree_->Branch("metphi",&metphi_,"metphi/D");
  tree_->Branch("transversemass",&transversemass_,"transversemass/F");
  tree_->Branch("metsigcov00",&metsigcov00_,"metsigcov00/F");
  tree_->Branch("metsigcov01",&metsigcov01_,"metsigcov01/F");
  tree_->Branch("metsigcov10",&metsigcov10_,"metsigcov10/F");
  tree_->Branch("metsigcov11",&metsigcov11_,"metsigcov11/F");

  tree_->Branch("pZeta",&pZeta_,"pZeta/F");
  tree_->Branch("pZetaVis",&pZetaVis_,"pZetaVis/F");



  tree_->Branch("njet",&njet_,"njet/I");
  tree_->Branch("leadJetPt",&leadJetPt_,"leadJetPt/F");
  tree_->Branch("leadJetRawFactor",&leadJetRawFactor_,"leadJetRawFactor/F");
  tree_->Branch("leadJetEta",&leadJetEta_,"leadJetEta/F");
  tree_->Branch("subleadJetPt",&subleadJetPt_,"subleadJetPt/F");
  tree_->Branch("subleadJetEta",&subleadJetEta_,"subleadJetEta/F");
  tree_->Branch("subleadJetRawFactor",&subleadJetRawFactor_,"subleadJetRawFactor/F");
  tree_->Branch("diJetMass",&diJetMass_,"diJetMass/F");
  tree_->Branch("diJetPt",&diJetPt_,"diJetPt/F"); 
  tree_->Branch("diJetDeltaEta",&diJetDeltaEta_,"diJetDeltaEta/F");
  tree_->Branch("diJetEta1Eta2",&diJetEta1Eta2_,"diJetEta1Eta2/F"); 
  tree_->Branch("njetingap",&njetingap_,"njetingap/I");
  tree_->Branch("nbjet",&nbjet_,"nbjet/I");
  tree_->Branch("leadBJetPt",&leadBJetPt_,"leadBJetPt/F");
  tree_->Branch("leadBJetEta",&leadBJetEta_,"leadBJetEta/F");

  tree_->Branch("muLCleadJetPt",&muLCleadJetPt_,"muLCleadJetPt/F");
  tree_->Branch("muLCleadJetEta",&muLCleadJetEta_,"muLCleadJetEta/F");


  tree_->Branch("vbfmva",&vbfmva_,"vbfmva/F");
  tree_->Branch("vbfvars0",&vbfvars_[0],"vbfvars0/D");
  tree_->Branch("vbfvars1",&vbfvars_[1],"vbfvars1/D");
  tree_->Branch("vbfvars2",&vbfvars_[2],"vbfvars2/D");
  tree_->Branch("vbfvars3",&vbfvars_[3],"vbfvars3/D");
  tree_->Branch("vbfvars4",&vbfvars_[4],"vbfvars4/D");
  tree_->Branch("vbfvars5",&vbfvars_[5],"vbfvars5/D");
  tree_->Branch("vbfvars6",&vbfvars_[6],"vbfvars6/D");
  tree_->Branch("vbfvars7",&vbfvars_[7],"vbfvars7/D");


  tree_->Branch("categoryCh",&categoryCh_,"categoryCh/I");
  tree_->Branch("categoryMT",&categoryMT_,"categoryMT/I");
  tree_->Branch("categoryIso",&categoryIso_,"categoryIso/I");
  tree_->Branch("categorySM",&categorySM_,"categorySM/I");
  tree_->Branch("categorySM2012",&categorySM2012_,"categorySM2012/I");


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
   if(dataType_==2){
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
       if((abs(g->pdgId())==23 || abs(g->pdgId())==24 ||  abs(g->pdgId())==25 ||  abs(g->pdgId())==36 ) && genBoson_==NULL )
 	genBoson_=&(*g);
     }
     //if(genBoson_)cout<<"genBoson_ ref = "<<genBoson_<<endl;

     if(genBoson_){      
       
       genbosonpt_=genBoson_->pt();
       genbosonphi_=genBoson_->phi();
       

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

       if(genBosonL1_ && genBosonL2_)
	 genbosonmass_=(genBosonL1_->p4()+genBosonL2_->p4()).mass();
       

     }

   }


   return 1;
 }

 bool TauEleFlatNtp::applySelections(){
   counterall_++;

   //if none are selected returns 0
   diTauSel_=NULL;

   //trigger, n-vertex selection
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
//      if( fabs(cand->leg2().dxy())>0.045)continue;
//      if( fabs(cand->leg2().dz())>0.1 )continue;

     if(!((*(cand->leg2().sourcePtr()))->gsfTrack().isNonnull()))continue;
     if(!((*(cand->leg2().sourcePtr()))->gsfTrack().isAvailable()))continue;     
     if(fabs((*(cand->leg2().sourcePtr()))->gsfTrack()->dxy(PV_->position())) > 0.045 ) continue;
     if(fabs((*(cand->leg2().sourcePtr()))->gsfTrack()->dz(PV_->position()))  > 0.2 ) continue;


     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()>0)countermuvtx_++;


   //ele id cuts
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/CMG/CMGTools/H2TauTau/python/proto/analyzers/TauEleAnalyzer.py?revision=1.12&view=markup
     //https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2012#2012_Baseline_Selection

//      Electron ID:
//     No missing inner hits
//     Conversion rejection
//     |dz| < 0.2 cm (the longitudinal impact parameter calculated with respect to the selected primary vertex)
//     In the EMu (ETau) channel: |d0| < 0.02(0.045) cm -- the impact parameter in the transverse plane
//     MVA selection: we only use the MVA for the ID, following the instructions in MultivariateElectronIdentification.
//         Use non-triggering electron MVA
//         Category 	LOOSE ID (EMu) 	TIGHT ID (ETau)
//         Cat4: pT > 20 GeV, abs(eta) < 0.8 	        mva_output > 0.905 	mva_output > 0.925
//         Cat5: pT > 20 GeV, 0.8 < abs (eta) < 1.479 	mva_output > 0.955 	mva_output > 0.975
//         Cat6: pT > 20 GeV, abs(eta) > 1.479 	        mva_output > 0.975      mva_output > 0.985
// /physicsobjects/PhysicsObjects.py

     if(cand->leg2().numberOfHits()!=0) continue;
     if(cand->leg2().passConversionVeto()!=1) continue;
     
//      //for sync?
//      if(!electronIDWP95(&(cand->leg2())))continue;     
     
     //mva id  
     //look here https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentification
     float mvaid=cand->leg2().mvaNonTrigV0();
     float eta=(*(cand->leg2().sourcePtr()))->superCluster()->eta();
     if(fabs(eta)<0.8)
       if(mvaid<0.925)continue; 
     if(0.8<=fabs(eta)&&fabs(eta)<1.479)
       if(mvaid<0.975)continue;
     if(1.479<=fabs(eta))
       if(mvaid<0.985)continue; 
     
     diTauSelList_.push_back(*cand);
    
   }
   if(diTauSelList_.size()>0)countermuid_++;

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
     if(cand->leg1().tauID("againstMuonLoose")<0.5)continue;
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()>0)countertaumuveto_++;

   //Tau anti-e cut
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(cand->leg1().tauID("againstElectronMedium")<0.5)continue;
     if(cand->leg1().tauID("againstElectronMVA")<0.5)continue;
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()>0)countertaueveto_++;
   //../python/proto/analyzers/TauEleAnalyzer.py


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
	//cand->leg2().relIso(0.5) < 0.1
	electronRelIsoDBCorr( &(cand->leg2()) ) < 0.1
	
        //-------tau iso cut here
        // && cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") > 0.5
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
   if(dataType_==0 || dataType_==2){
  
     ///trigger corrections
     if(trigPaths_.size()>0){//trigger applied--> apply a correction factor
       triggerEffWeight_ *= triggerEff_.effTau2011AB_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
 	                  /triggerEff_.effMediumIsoTau20MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
       triggerEffWeight_ *= triggerEff_.effEle2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
 	                  /triggerEff_.effEle18MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
     }else{//no trigger applied --> apply efficiency
       triggerEffWeight_ *= triggerEff_.effTau2011AB_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
       triggerEffWeight_ *= triggerEff_.effEle2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
     }

     //id+isolation corrections
     selectionEffWeight_ *= selectionEff_.effCorrEle2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

   }
   eventweight_=pupWeight_*embeddedGenWeight_*triggerEffWeight_*selectionEffWeight_;


   mupt_=diTauSel_->leg2().pt();
   mueta_=diTauSel_->leg2().eta();
   muphi_=diTauSel_->leg2().phi();
   //muiso_=diTauSel_->leg2().relIsoAllChargedDB05(); 
   muiso_= electronRelIsoDBCorr( &(diTauSel_->leg2()) ); //diTauSel_->leg2().relIso(0.5);
   muisomva_=(*(diTauSel_->leg2().sourcePtr()))->userFloat("mvaIsoRings");
   mudz_=diTauSel_->leg2().dz();
   mudxy_=diTauSel_->leg2().dxy();
   mux_=diTauSel_->leg2().vertex().x();
   muy_=diTauSel_->leg2().vertex().y();
   muz_=diTauSel_->leg2().vertex().z();
   mutruth_=truthMatchEle();
   mumvaid_=diTauSel_->leg2().mvaNonTrigV0();
 
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

   ///define control samples
   categoryCh_=0;
   if(fabs(ditaucharge_)==0.)categoryCh_=1;
   if(fabs(ditaucharge_)==2.)categoryCh_=2;

   //   pfmetpt_=diTauSel_->met().pt();
   //   pfmetphi_=diTauSel_->met().phi();

   edm::Handle<std::vector< cmg::BaseMET> > pfMET;
   iEvent_->getByLabel(edm::InputTag("cmgPFMETRaw"),pfMET);
   
   pfmetpt_=pfMET->begin()->pt();
   pfmetphi_=pfMET->begin()->phi();
   pftransversemass_=sqrt(2*mupt_*pfmetpt_*(1-cos(muphi_-pfmetphi_)));


   edm::Handle< cmg::METSignificance > pfMetSigHandle;
   iEvent_->getByLabel(edm::InputTag("pfMetSignificance"),pfMetSigHandle); 
   const cmg::METSignificance * metSig = &(*pfMetSigHandle);
   if(!metSig){ cout<<" metSig is NULL "<<endl; return 0;}
   
   metsigcov00_=(*(metSig->significance()))[0][0];
   metsigcov01_=(*(metSig->significance()))[0][1];
   metsigcov10_=(*(metSig->significance()))[1][0];
   metsigcov11_=(*(metSig->significance()))[1][1];


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

   leadJet_=0;
   subleadJet_=0;
   if(njet_>0)leadJet_=pfJetListLC_[0];
   if(njet_>1)subleadJet_=pfJetListLC_[1];
   

   //run MVA met here 
   //../../Common/python/miscProducers/mvaMET/mvaMETTauEle_cfi.py  


   //Also the list cleaned only with the muon //njetLepLC needed in recoil correction
   fillPFJetListLepLC(diTauSel_,&pfJetList_,&pfJetListLepLC_);
   int njetLepLC_=pfJetListLepLC_.size();

   //recoil corrected met
   metpt_=pfmetpt_;
   metphi_=pfmetphi_;

   ///Apply recoil correction here
   if(recoilCorreciton_>0){

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

     corrector_.CorrectType1( metpt_, metphi_,  genBoson_->pt(), genBoson_->phi(),  lepPt, lepPhi,  u1, u2, fluc,  recoiliScale_ ,  jetMult );
   }

   reco::Candidate::PolarLorentzVector metP4( metpt_,0,metphi_, 0);

   transversemass_=sqrt(2*mupt_*metP4.pt()*(1-cos(muphi_-metP4.phi())));
   compZeta(&(diTauSel_->leg2()),&(diTauSel_->leg1()),metP4.px(),metP4.py(),&pZeta_,&pZetaVis_);



   ///!!!!!!Need to update to new MVA Met significance matrix-----------------------------------------------------------------------------Fix
   if(runSVFit_==1){  //old svfit  
     NSVfitStandalone2011::Vector measuredMET( metP4.x(), metP4.y(), 0);
     std::vector<NSVfitStandalone2011::MeasuredTauLepton2011> measuredTauLeptons;
     NSVfitStandalone2011::LorentzVector p1(diTauSel_->leg1().p4());
     measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kHadDecay,p1));    
     NSVfitStandalone2011::LorentzVector p2(diTauSel_->leg2().p4());
     measuredTauLeptons.push_back(NSVfitStandalone2011::MeasuredTauLepton2011(NSVfitStandalone2011::kLepDecay,p2));
     NSVfitStandaloneAlgorithm2011 algo(measuredTauLeptons,measuredMET,metSig->significance(),0);
     algo.maxObjFunctionCalls(5000);
     algo.fit();
     svfitmass_  = algo.fittedDiTauSystem().mass();
   }

   if(runSVFit_==2){    //new svfit
     std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
     measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, diTauSel_->leg2().p4()));
     measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, diTauSel_->leg1().p4()));
     NSVfitStandaloneAlgorithm algo(measuredTauLeptons, metP4.Vect(), *(metSig->significance()), 0);
     algo.addLogM(false);
     algo.integrate();
     svfitmass_ = algo.getMass();
   }


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
   }

   //Jets where only the muon has been removed
   if(pfJetListLepLC_.size()>=1){
     muLCleadJetPt_=pfJetListLepLC_[0]->pt();
     muLCleadJetEta_=pfJetListLepLC_[0]->eta();
   }


   //make a list of b-tagged jets  jet->btag(6) 
   pfJetListBTag_.clear();
   for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
     if(jet->pt()<20.0)continue;  
     if(fabs(jet->eta())>2.4)continue; 
     if(!checkPFJetId(&(*jet)))continue;
     if(jet->btag("combinedSecondaryVertexBJetTags")<btagWP_)continue;//CSV medium     
     pfJetListBTag_.push_back(&(*jet));
   }
   fillPFJetListLC(diTauSel_,&pfJetListBTag_,&pfJetListBTagLC_);
   nbjet_=pfJetListBTagLC_.size();
   if(nbjet_>0){
     leadBJetPt_=pfJetListBTagLC_[0]->pt();
     leadBJetEta_=pfJetListBTagLC_[0]->eta();
     leadBJetBTagProb_=pfJetListBTagLC_[0]->btag("combinedSecondaryVertexBJetTags");
   }


   // KK ---------------------------------- Compute btag weight
   pfJetListBTagWeight_.clear();
   for(std::vector<cmg::PFJet>::const_iterator jet=fulljetlist->begin(); jet!=fulljetlist->end(); ++jet){
     if(jet->pt()<20.0)continue;  
     if(fabs(jet->eta())>2.4)continue; 
     if(!checkPFJetId(&(*jet)))continue;
     pfJetListBTagWeight_.push_back(&(*jet));
   }
   btagEffWeight_ = 1.;
   vector<vector<BTagWeight::JetInfo> > jetinfovec;
   for (unsigned int i=0; i<pfJetListBTagWeight_.size(); ++i) {
     double jetpt = pfJetListBTagWeight_[i]->pt(); 
     double jeteta = pfJetListBTagWeight_[i]->eta(); 
     double jetflavor = TMath::Abs(pfJetListBTagWeight_[i]->partonFlavour());
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
   // KK -----------------  



   //////////////////////
   ////SM event categories //use mu-tau cleaned jet list
   //////////////////////
   categorySM_=-1;
   if(pfJetListLC_.size()>=2){//VBF: two leading jets must have  m>400, |eta1-eta2| < 4 and no other jet high pt in between    
     if((leadJet_->p4()+subleadJet_->p4()).mass() > 400.0 
        && fabs(leadJet_->eta() - subleadJet_->eta()) > 4.0 
        && leadJet_->eta()*subleadJet_->eta() < 0.0
        ){
       njetingap_=0;
       if(pfJetListLC_.size()>2){// check there is no additional central jet
	 for(std::vector<const cmg::PFJet *>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	   if(leadJet_->eta()<subleadJet_->eta()) 
	     if(leadJet_->eta()<(*jet3)->eta()&&(*jet3)->eta()<subleadJet_->eta()) njetingap_++;
	   if(leadJet_->eta()>subleadJet_->eta()) 
	     if(subleadJet_->eta()<(*jet3)->eta()&&(*jet3)->eta()<leadJet_->eta()) njetingap_++;
	 }
       }
       if(njetingap_==0) categorySM_=2;
     }
   }
   if(categorySM_!=2 && pfJetListLC_.size()>=1){//Boosted: 1 jet with pt>150 and no other jets
     if(leadJet_->pt()>=150.0)
       categorySM_=1;
   }

   if(categorySM_!=2 && categorySM_!=1 && pfJetListLC_.size()<=1){//SM0
     if(pfJetListLC_.size()==1){
       if(leadJet_->pt()<150.0)
	 categorySM_=0;
     }else categorySM_=0;
   }


   //VBF MVA
   vbfmva_=0.;
   if(njet_>=2){
     
     TVector3 vTau, vMu, vMET, vDiTau, vDiTauVis;
     vTau.SetPtEtaPhi(diTauSel_->leg1().pt(), diTauSel_->leg1().eta(), diTauSel_->leg1().phi());
     vMu.SetPtEtaPhi(diTauSel_->leg2().pt(), diTauSel_->leg2().eta(), diTauSel_->leg2().phi());
     vMET.SetPtEtaPhi(metP4.pt(),0,metP4.phi()); 
     

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


   categorySM2012_=-1;
   if(njet_>=2){//----------------------------------VBF---------------------
     njetingap_=0;
     if(njet_>2){// check there is no additional central jet
       for(std::vector<const cmg::PFJet *>::const_iterator jet3=pfJetListLC_.begin(); jet3!=pfJetListLC_.end(); ++jet3){
	 if(leadJet_->eta()<subleadJet_->eta()) 
	   if(leadJet_->eta()<(*jet3)->eta()&&(*jet3)->eta()<subleadJet_->eta()) njetingap_++;
	 if(leadJet_->eta()>subleadJet_->eta()) 
	   if(subleadJet_->eta()<(*jet3)->eta()&&(*jet3)->eta()<leadJet_->eta()) njetingap_++;
       }
     }
     if(njetingap_==0 && vbfmva_ > 0.5 ) categorySM2012_=2;
   }
   if(categorySM2012_ == -1 && njet_>=2){//------------------VH -------------------
     if(70.< diJetMass_ && diJetMass_ < 120.0 
	&& diJetPt_ > 150.
	&& nbjet_ == 0
	&& vbfmva_ < 0.5
	) categorySM2012_=3;
   }  
   if(categorySM2012_ == -1 && njet_>=1 && nbjet_ == 0 ){//-----------1-jet
     if( taupt_<40.)categorySM2012_=1;
     else categorySM2012_=11;
   }
   if(categorySM2012_ == -1 && nbjet_>=1 && njet_ < 2 ){//-----------1-bjet
     if( taupt_<40.) categorySM2012_=4;
     else categorySM2012_=14;
   }
   if(categorySM2012_ == -1){//inclusive
     if( taupt_<40.) categorySM2012_=0;
     else categorySM2012_=10;
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


 }


 void TauEleFlatNtp::fillPFJetListLepLC(const cmg::TauEle * cand,std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC){

   listLC->clear();

   //std::vector<cmg::PFJet> cleanjetlist;
   for(std::vector<const cmg::PFJet *>::const_iterator jet=list->begin(); jet!=list->end(); ++jet){
     if(reco::deltaR((*jet)->eta(),(*jet)->phi(),cand->leg2().eta(),cand->leg2().phi())<0.5) continue;   
     //cleanjetlist.push_back(*jet);
     listLC->push_back(*jet);
   }

 }

 bool TauEleFlatNtp::vetoDiLepton(){

   bool muminus=0;
   bool muplus=0;
   for(std::vector<cmg::Electron>::const_iterator m=diLeptonVetoList_->begin(); m!=diLeptonVetoList_->end(); ++m){  
     if(m->pt()<=15.0)continue;
     if(fabs(m->eta())>=2.5)continue;
     if(fabs((*(m->sourcePtr()))->gsfTrack()->dxy(PV_->position())) > 0.045 ) continue;
     if(fabs((*(m->sourcePtr()))->gsfTrack()->dz(PV_->position()))  > 0.2 ) continue;     
     if(m->numberOfHits()!=0)return 0;
     if(m->passConversionVeto()!=1)return 0;     
     if(!electronIDWP95(&(*m)))continue;

     if( electronRelIsoDBCorr( &(*m) )>=0.3) continue; 

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

 int TauEleFlatNtp::truthMatchEle(){
   if(!diTauSel_ )return 0;
   if(dataType_!=0) return 0;

   for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
     if(abs(g->pdgId())==11) if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),g->eta(),g->phi())<deltaRTruth_) return 1;
     if(abs(g->pdgId())==13) if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),g->eta(),g->phi())<deltaRTruth_) return 3;
     if(abs(g->pdgId())==15) if(reco::deltaR(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),g->eta(),g->phi())<deltaRTruth_) return 5;
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


