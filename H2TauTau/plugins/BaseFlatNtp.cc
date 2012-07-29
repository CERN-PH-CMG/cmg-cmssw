#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

BaseFlatNtp::BaseFlatNtp(const edm::ParameterSet & iConfig):
  file_(0),
  tree_(0),
  genBoson_(0),
  genBosonL1_(0),
  genBosonL2_(0),
  sampleGenEventType_(0),
  sampleTruthEventType_(0),
  genEventType_(0),
  truthEventType_(0),
  deltaRTruth_(0.3),
  signalWeightHisto_(NULL),
  btagWP_(0.679),
  corrector_(iConfig.getParameter<std::string>("fileCorrectTo")),
  mvaWeights_(iConfig.getParameter<std::string>("mvaWeights")),
  reader_(mvaWeights_.c_str()){

  //for debugging
  printSelectionPass_ = iConfig.getParameter<int>("printSelectionPass");
  cout<<"printSelectionPass_  : "<<printSelectionPass_<<endl;

  dataPeriodFlag_=iConfig.getParameter<int>("dataPeriodFlag");
  cout<<"dataPeriodFlag_  : "<<dataPeriodFlag_<<endl;

  verticesListTag_=iConfig.getParameter<edm::InputTag>("verticesListTag");
  cout<<"verticesListTag_  : "<<verticesListTag_.label()<<endl;

  trigPathsListTag_=iConfig.getParameter<edm::InputTag>("trigPathsListTag");
  cout<<"trigPathsListTag_  : "<<trigPathsListTag_.label()<<endl;

  trigObjsListTag_=iConfig.getParameter<edm::InputTag>("trigObjsListTag");
  cout<<"trigObjsListTag_  : "<<trigObjsListTag_.label()<<endl;

  pupWeightName_=iConfig.getParameter<edm::InputTag>("pupWeightName");
  cout<<"pupWeightName_  : "<<pupWeightName_.label()<<endl;

  firstRun_=iConfig.getParameter<int>("firstRun");
  cout<<"firstRun_  : "<<firstRun_<<endl;

  lastRun_=iConfig.getParameter<int>("lastRun");
  cout<<"lastRun_  : "<<lastRun_<<endl;

  dataType_=iConfig.getParameter<int>("dataType");  
  cout<<"dataType_  : "<<dataType_<<endl;

  genParticlesTag_        = iConfig.getParameter<edm::InputTag>("genParticlesTag");
  cout<<"genParticlesTag_  : "<<genParticlesTag_.label()<<endl;
  
  sampleGenEventType_     = iConfig.getParameter<int>("sampleGenEventType");
  cout<<"sampleGenEventType  : "<<sampleGenEventType_<<endl;

  sampleTruthEventType_   = iConfig.getParameter<int>("sampleTruthEventType");
  cout<<"sampleTruthEventType : "<<sampleTruthEventType_<<endl;



  ////
  tauPtCut_=iConfig.getParameter<double>("tauPtCut");  
  cout<<"tauPtCut_  : "<<tauPtCut_<<endl;

  tauEtaCut_=iConfig.getParameter<double>("tauEtaCut");  
  cout<<"tauEtaCut_  : "<<tauEtaCut_<<endl;

  muPtCut_=iConfig.getParameter<double>("muPtCut");  
  cout<<"muPtCut_  : "<<muPtCut_<<endl;

  muEtaCut_=iConfig.getParameter<double>("muEtaCut");  
  cout<<"muEtaCut_  : "<<muEtaCut_<<endl;

  metType_ = iConfig.getParameter<int>("metType");
  cout<<"metType_  : "<<metType_<<endl;

  mvaMETSigTag_ = iConfig.getParameter<edm::InputTag>("mvaMETSigTag");
  cout<<"mvaMETSigTag_  : "<<mvaMETSigTag_.label()<<endl;


  runSVFit_  =  iConfig.getParameter<int>("runSVFit");
  cout<<" runSVFit_ : "<<runSVFit_<<endl;


  //

  diTauTag_               = iConfig.getParameter<edm::InputTag>("diTauTag");
  cout<<"diTauTag_  : "<<diTauTag_.label()<<endl;
    
  diMuonVetoListTag_      = iConfig.getParameter<edm::InputTag>("diMuonVetoListTag");
  cout<<"diMuonVetoListTag : "<<diMuonVetoListTag_.label()<<endl;

  pfJetListTag_           = iConfig.getParameter<edm::InputTag>("pfJetListTag");
  cout<<"pfJetListTag_ : "<<pfJetListTag_.label()<<endl;


  //
  recoilCorreciton_ =  iConfig.getParameter<int>("recoilCorrection");
  cout<<"recoilCorreciton_  : "<<recoilCorreciton_<<endl;

  fileZmmData_ = iConfig.getParameter<std::string>("fileZmmData");
  cout<<"fileZmmData_ : "<<fileZmmData_.c_str()<<endl;
  corrector_.addDataFile( fileZmmData_);

  fileZmmMC_ = iConfig.getParameter<std::string>("fileZmmMC");
  cout<<"fileZmmMC_  : "<<fileZmmMC_.c_str()<<endl;
  corrector_.addMCFile( fileZmmMC_);

  recoiliScale_ = iConfig.getParameter<double>("recoiliScale");
  cout<<"recoiliScale_   : "<<recoiliScale_<<endl;
   
  randsigma_              = iConfig.getParameter<double>("randsigma");
  cout<<"randsigma_  : "<<randsigma_<<endl;


  //read in histogram with signal weights
  signalWeightDir_ = iConfig.getParameter<std::string>("signalWeightDir");
  cout<<"signalWeightDir_  : "<<signalWeightDir_.c_str()<<endl;
  signalWeightMass_ = iConfig.getParameter<std::string>("signalWeightMass");
  cout<<"signalWeightMass_  "<<signalWeightMass_.c_str()<<endl;
  if((int)(signalWeightMass_.compare("0"))!=0 ){//signal reweighting required
    TFile F(TString(signalWeightDir_)+"/weight_ptH_"+signalWeightMass_.c_str()+".root","read");
    TH1F* H=(TH1F*)F.Get(TString("powheg_weight/weight_hqt_fehipro_fit_")+signalWeightMass_.c_str());
    gROOT->cd();
    if(!H){
      cout<<"signalWeight requested but histogram not found in "<<F.GetName()<<endl;
      exit(0);
    }
    signalWeightHisto_=(TH1F*)H->Clone("signalWeightHisto");
  }

  cout<<"Trigger paths: "<<endl;
  for(long p=1;p<=10;p++){
    edm::InputTag path=iConfig.getParameter<edm::InputTag>((const char*)(TString("trigPath")+p));
    if(path.label()!=""){
      cout<<path.label()<<" "<<path.instance()<<" "<<path.process()<<endl;
      trigPaths_.push_back(new edm::InputTag(path));
    }
  }



}

BaseFlatNtp::~BaseFlatNtp(){

  delete signalWeightHisto_;
  delete file_;
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
    delete *path;
  }

}

void BaseFlatNtp::beginJob(){

  file_ = new edm::Service<TFileService>();
  //tree_->Branch("",&,"/I");

  tree_ = (*file_)->make<TTree>("tree","tree");

  tree_->Branch("pupWeight",&pupWeight_,"pupWeight/F");
  tree_->Branch("eventweight",&eventweight_,"eventweight/F");
  tree_->Branch("triggerEffWeight",&triggerEffWeight_,"triggerEffWeight/F"); 
  tree_->Branch("selectionEffWeight",&selectionEffWeight_,"selectionEffWeight/F"); 
  tree_->Branch("embeddedGenWeight",&embeddedGenWeight_,"embeddedGenWeight/F"); 
  tree_->Branch("btagEffWeight",&btagEffWeight_,"btagEffWeight/F");
  tree_->Branch("signalWeight",&signalWeight_,"signalWeight/F");

  tree_->Branch("genbosonmass",&genbosonmass_,"genbosonmass/F");
  tree_->Branch("genbosonpt",&genbosonpt_,"genbosonpt/F");
  tree_->Branch("genbosoneta",&genbosoneta_,"genbosoneta/F");
  tree_->Branch("genbosonphi",&genbosonphi_,"genbosonphi/F");

  tree_->Branch("runnumber",&runnumber_,"runnumber/I");
  tree_->Branch("lumiblock",&lumiblock_,"lumiblock/I");
  tree_->Branch("eventid",&eventid_,"eventid/I");

  tree_->Branch("npu",&npu_,"npu/I");
  tree_->Branch("nvtx",&nvtx_,"nvtx/I");
  tree_->Branch("vtxx",&vtxx_,"vtxx/F");
  tree_->Branch("vtxy",&vtxy_,"vtxy/F");
  tree_->Branch("vtxz",&vtxz_,"vtxz/F");


  tree_->Branch("nditau",&nditau_,"nditau/I");
  tree_->Branch("ditaumass",&ditaumass_,"ditaumass/F");
  tree_->Branch("ditaucharge",&ditaucharge_,"ditaucharge/I");
  tree_->Branch("svfitmass",&svfitmass_,"svfitmass/F");
  tree_->Branch("ditaueta",&ditaueta_,"ditaueta/F");
  tree_->Branch("ditaupt",&ditaupt_,"ditaupt/F");
  tree_->Branch("ditauphi",&ditauphi_,"ditauphi/F");
  tree_->Branch("mutaucostheta",&mutaucostheta_,"mutaucostheta/F");
  
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
  tree_->Branch("mutruth",&mutruth_,"mutruth/I");

  tree_->Branch("pfmetpt",&pfmetpt_,"pfmetpt/D");
  tree_->Branch("pfmetphi",&pfmetphi_,"pfmetphi/D");
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
  tree_->Branch("njetLepLC",&njetLepLC_,"njetLepLC/I"); 
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

  tree_->Branch("njet20",&njet20_,"njet20/I");
  tree_->Branch("nbjet",&nbjet_,"nbjet/I");
  tree_->Branch("leadBJetPt",&leadBJetPt_,"leadBJetPt/F");
  tree_->Branch("leadBJetEta",&leadBJetEta_,"leadBJetEta/F");

  tree_->Branch("vbfmva",&vbfmva_,"vbfmva/F");
  tree_->Branch("vbfvars0",&vbfvars_[0],"vbfvars0/D");
  tree_->Branch("vbfvars1",&vbfvars_[1],"vbfvars1/D");
  tree_->Branch("vbfvars2",&vbfvars_[2],"vbfvars2/D");
  tree_->Branch("vbfvars3",&vbfvars_[3],"vbfvars3/D");
  tree_->Branch("vbfvars4",&vbfvars_[4],"vbfvars4/D");
  tree_->Branch("vbfvars5",&vbfvars_[5],"vbfvars5/D");
  tree_->Branch("vbfvars6",&vbfvars_[6],"vbfvars6/D");
  tree_->Branch("vbfvars7",&vbfvars_[7],"vbfvars7/D");
  
  
  tree_->Branch("muLCleadJetPt",&muLCleadJetPt_,"muLCleadJetPt/F");
  tree_->Branch("muLCleadJetEta",&muLCleadJetEta_,"muLCleadJetEta/F");


  tree_->Branch("categoryIso",&categoryIso_,"categoryIso/I");
  tree_->Branch("categorySM",&categorySM_,"categorySM/I");
  tree_->Branch("categorySM2012",&categorySM2012_,"categorySM2012/I");

  counterall_=0;
  countertrig_=0;
  counterruns_=0;
  countergoodvtx_=0;
  countergen_=0;
}


// void BaseFlatNtp::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){
//   fillVariables(iEvent,iSetup);
//   if(!applySelections())return;
//   fill();
//   tree_->Fill();
// }

void BaseFlatNtp::endJob(){

  cout<<"counterall = "<<counterall_<<endl;
  cout<<"counterruns = "<<counterruns_<<endl;
  cout<<"countertrig = "<<countertrig_<<endl;
  cout<<"countergoodvtx = "<<countergoodvtx_<<endl;
  cout<<"countergen = "<<countergen_<<endl;

}



bool BaseFlatNtp::fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  //fill things one may need to apply selections

  iEvent_=&iEvent;

  runnumber_=iEvent_->run();
  lumiblock_=iEvent_->luminosityBlock();
  eventid_=iEvent_->id().event();

  iEvent_->getByLabel(verticesListTag_,vertices_);
  nvtx_=vertices_->size();  
  PV_=&(*(vertices_->begin()));


  iEvent_->getByLabel(trigPathsListTag_,trig_);
  //trig_->begin()->printSelections(cout);

  //get trigger object list for later
  iEvent_->getByLabel(trigObjsListTag_,trigObjs_);
  
  ///get the gen Boson and set the genEventType
  genBoson_ = NULL;
  genBosonL1_ = NULL;
  genBosonL2_ = NULL;
  genEventType_=0;
  if(dataType_==0){  
    iEvent_->getByLabel(genParticlesTag_,genParticles_);    
    for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
      //cout<<g->pdgId()<<" "<<g->p4().pt()<<endl;
      if((abs(g->pdgId())==23 || abs(g->pdgId())==24 ||  abs(g->pdgId())==25 ||  abs(g->pdgId())==36 ) && genBoson_==NULL )
	genBoson_=&(*g);
    }
    //if(genBoson_)cout<<"genBoson_ ref = "<<genBoson_<<" "<<<genBoson_->pdgId()<<" "<<genBoson_->pt()<<endl;

    if(genBoson_){      

      genbosonpt_=genBoson_->pt();
      genbosoneta_=genBoson_->eta();
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
      //cout<<genbosonmass_<<" "<<genbosonpt_<<" "<<genbosonphi_<<endl;

    }

  }

 
  return 1;
}

bool BaseFlatNtp::applySelections(){
  counterall_++;

  if((firstRun_!=0 && runnumber_<firstRun_)
     || (lastRun_!=0 && lastRun_<runnumber_)){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterruns"<<endl;
    return 0;
  }
  counterruns_++;


  if(trigPaths_.size()==0)trigpass_=1;//no trigger requirement
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){//cmg ObjetSel
    //cout<<path->label()<<" "<<path->instance()<<" "<<path->process()<<endl;
    if(trig_->begin()->hasSelection((*path)->label()))
      if(trig_->begin()->getSelection((*path)->label()))
	if(trig_->begin()->getPrescale((*path)->label())==1 || dataType_==0){
	  trigpass_=1;
	}
  }
  //cout<<firstRun_<<" "<<lastRun_<<" "<<runnumber_<<" "<<trigpass_<<endl;
  if(!trigpass_){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertrig"<<endl;
    return 0;
  }
  countertrig_++;

  
  if(nvtx_==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countergoodvtx"<<endl;
    return 0;
  }
  countergoodvtx_++;


  if( sampleGenEventType_!=0 && sampleGenEventType_!=genEventType_){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countergen"<<endl;
    return 0;
  }
  countergen_++;


  
  return 1;
}

bool BaseFlatNtp::fill(){
 
  vtxx_=PV_->x();
  vtxy_=PV_->y();
  vtxz_=PV_->z();

  pupWeight_=1.;//do not comment out needs to be used.
  npu_=-1;
  if(dataType_==0 && (pupWeightName_.label()).compare("")!=0){//if no vertex weight name is provided then leave weight to 1
    edm::Handle<double>  PupWeight;
    iEvent_->getByLabel(pupWeightName_,PupWeight);    
    pupWeight_=(*PupWeight);

    //get the number of pile up vertexes
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    iEvent_->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
      int BX = PVI->getBunchCrossing();
      if(BX == 0) {
	if(dataPeriodFlag_==2011)npu_ = PVI->getPU_NumInteractions();
	if(dataPeriodFlag_==2012)npu_ = PVI->getTrueNumInteractions();
      }
    }
  }


  //embedded samples generator weight
  embeddedGenWeight_=1.0;
  if(dataType_==2){
    if(dataPeriodFlag_==2011){
      edm::Handle< double > embeddedGenWeight;
      iEvent_->getByLabel(edm::InputTag("generator","weight",""),embeddedGenWeight);
      embeddedGenWeight_=*embeddedGenWeight;
    }
    if(dataPeriodFlag_==2012){
      edm::Handle<GenFilterInfo> genInfoEmbedded;
      iEvent_->getByLabel(edm::InputTag("generator","minVisPtFilter","EmbeddedRECO"),genInfoEmbedded);
      if(genInfoEmbedded->numEventsTried()>0) embeddedGenWeight_ =  genInfoEmbedded->filterEfficiency(); 
    }
  }  


  //fill the signalWeight
  signalWeight_=1.;
  if(signalWeightHisto_){
    if(!genBoson_){
      cout<<"signal weight requested but no genboson found in event."<<endl;
      exit(0);
    }
    if(0.<genbosonpt_&&genbosonpt_<500.)
      signalWeight_=signalWeightHisto_->GetBinContent(((int)genbosonpt_)+1);//round the the pt value down
  }



  return 1;
}


bool BaseFlatNtp::trigObjMatch(float eta, float phi, std::string path, std::string filter, int pdgid){

  if(filter.compare("")==0) return 1;//no trigger matching required

  for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs_->begin(); obj!=trigObjs_->end(); obj++){
    if(obj->hasSelection(path.c_str())){//HLT path name
      //obj->printSelections(cout);
      if(obj->hasSelection(filter.c_str())){//last filter
	if(reco::deltaR(eta,phi,obj->eta(),obj->phi())<0.3
	   && (abs(obj->pdgId())==pdgid || pdgid==-1)
	   ){
	  //obj->printSelections(cout);	  
	  //cout<<"pdg id "<<obj->pdgId()<<endl;
	  return 1;      
	}
      }
    }
  }
  
  return 0;
}


void BaseFlatNtp::fillPFJetList(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list){
  list->clear();
  for(std::vector<const cmg::PFJet *>::const_iterator jet=fulllist->begin(); jet!=fulllist->end(); ++jet){
    if((*jet)->pt()<30.0)continue;  
    if(fabs((*jet)->eta())>4.5)continue; 
    if(!checkPFJetId((*jet)))continue;
    if(!((*jet)->passPuJetId("full",PileupJetIdentifier::kLoose))) continue;
    list->push_back((*jet));
  }
}


void BaseFlatNtp::fillPFJetListB(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list){
  list->clear();
  for(std::vector<const cmg::PFJet *>::const_iterator jet=fulllist->begin(); jet!=fulllist->end(); ++jet){
    if((*jet)->pt()<20.0)continue;  
    if(fabs((*jet)->eta())>2.4)continue; 
    if(!checkPFJetId((*jet)))continue;
    list->push_back((*jet));
  }
}

void BaseFlatNtp::fillPFJetListBTag(std::vector<const cmg::PFJet * > * fulllist, std::vector<const cmg::PFJet * > * list){
  list->clear();
  for(std::vector<const cmg::PFJet *>::const_iterator jet=fulllist->begin(); jet!=fulllist->end(); ++jet){
    if((*jet)->btag("combinedSecondaryVertexBJetTags")<btagWP_)continue;//CSV medium
    list->push_back((*jet));
  }
}


void BaseFlatNtp::fillPFJetListLC(float leg1eta, float leg1phi, float leg2eta, float leg2phi, std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC){
  listLC->clear();
  for(std::vector<const cmg::PFJet *>::const_iterator jet=list->begin(); jet!=list->end(); ++jet){
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),leg1eta,leg1phi)<0.5) continue;     
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),leg2eta,leg2phi)<0.5) continue;   
    listLC->push_back(*jet);
  }
}


void BaseFlatNtp::fillPFJetListLepLC(float lepeta, float lepphi,std::vector<const cmg::PFJet * > * list, std::vector<const cmg::PFJet * > * listLC){
  listLC->clear();
  for(std::vector<const cmg::PFJet *>::const_iterator jet=list->begin(); jet!=list->end(); ++jet){
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),lepeta,lepphi)<0.5) continue;   
    listLC->push_back(*jet);
  }  
}


int BaseFlatNtp::truthMatchLeg(float legeta, float legphi){
  if(dataType_!=0) return 0;

  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==11) if(reco::deltaR(legeta,legphi,g->eta(),g->phi())<deltaRTruth_) return 1;
    if(abs(g->pdgId())==13) if(reco::deltaR(legeta,legphi,g->eta(),g->phi())<deltaRTruth_) return 3;
    if(abs(g->pdgId())==15) if(reco::deltaR(legeta,legphi,g->eta(),g->phi())<deltaRTruth_) return 5;
  }
  
  return 9;
}

const cmg::PFJet * BaseFlatNtp::findJet(std::vector<const cmg::PFJet * > * fulllist, float eta, float phi){
  for(std::vector<const cmg::PFJet *>::const_iterator jet=fulllist->begin(); jet!=fulllist->end(); ++jet)
    if(reco::deltaR((*jet)->eta(),(*jet)->phi(),eta,phi)<0.3) return *jet;
  return 0;
}

void BaseFlatNtp::printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList){
  cout<<" Begin MC print out for event: "<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
  for(std::vector<reco::GenParticle>::const_iterator g=genList->begin(); g!=genList->end(); ++g){    
    cout<<"pdgID = "<<g->pdgId()<<" , pt = "<<g->p4().pt()<<" motherRef="<<g->mother()<<endl;
  }
}


