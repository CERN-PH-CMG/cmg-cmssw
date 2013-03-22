#include "CMGTools/H2TauTau/plugins/TauMuFlatNtp.h"


TauMuFlatNtp::TauMuFlatNtp(const edm::ParameterSet & iConfig):
  BaseFlatNtp(iConfig)
{

}


TauMuFlatNtp::~TauMuFlatNtp(){

}


void TauMuFlatNtp::beginJob(){

  BaseFlatNtp::beginJob();
  //  tree_->Branch("",&_,"/F");



  //counters
  counterev_=0;
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
  countertruth_=0;
}


void TauMuFlatNtp::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
  fillVariables(iEvent,iSetup);
  if(!applySelections()) return;
  fill();
  tree_->Fill();
}



bool TauMuFlatNtp::fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  if(!BaseFlatNtp::fillVariables(iEvent,iSetup)) return 0;

  ///get the TauMu cands 
  iEvent.getByLabel(diTauTag_,diTauList_);
  
  return 1;
}

bool TauMuFlatNtp::applySelections(){
  //if none are selected returns 0

  diTauSel_=NULL;

  if(!BaseFlatNtp::applySelections()){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterev"<<endl;
    return 0;
  }
  counterev_++;



  if(diTauList_->size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterpresel"<<endl;  
    return 0;
  }
  counterpresel_++;

  std::vector<cmg::TauMu> tmpditaulist=*diTauList_;
  diTauSelList_.clear();



  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg2().pt()<20.0)continue;
    if(fabs(cand->leg2().eta())>2.1)continue;
    if(reco::deltaR(cand->leg1().p4().eta(),cand->leg1().p4().phi(),
		    cand->leg2().p4().eta(),cand->leg2().p4().phi()
		    )<0.5)continue;
    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_){
      cout<<runnumber_<<":"<<eventid_<<" fail Muon preselection"<<endl;  
      for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	printMuonInfo(&(cand->leg2()));
      }
    }
    return 0;
  }

  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("decayModeFinding")<0.5 )continue;
    if(cand->leg1().pt()<20.0 )continue;
    if(fabs(cand->leg1().eta())>2.3 )continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_){
      cout<<runnumber_<<":"<<eventid_<<" fail Tau preselection"<<endl;  
      for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
      	printTauInfo(&(cand->leg1()));
      }
    }
    return 0;
  }


  //muon vtx 
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(!muonVertexCut(&(cand->leg2())))continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_){
      cout<<runnumber_<<":"<<eventid_<<" fail countermuvtx"<<endl;  
      for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	printMuonInfo(&(cand->leg2()));
      }
    }
    return 0;
  }
  countermuvtx_++;


  //muion id cuts
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(!muonIDPFMuonTight(&(cand->leg2())))continue;
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_){
      cout<<runnumber_<<":"<<eventid_<<" fail countermuid"<<endl;
      for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand)
	printMuonInfo(&(cand->leg2()));
    }

    return 0;
  }
  countermuid_++;

//   /////////Muon iso
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     if( cand->leg2().relIso(0.5,1)<0.1){
//       diTauSelList_.push_back(*cand);
//     }
//   }
//   if(diTauSelList_.size()==0){
//     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countermuiso"<<endl;
//     return 0;
//   }
//   countermuiso_++;

  //muon trig-match
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    bool matchmu=0;
    if(trigPaths_.size()==0) matchmu=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg2().eta(),cand->leg2().phi(),(*path)->label(),(*path)->process()),13)
	  matchmu=1;
    }
    
    if(matchmu) 
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_){
      cout<<runnumber_<<":"<<eventid_<<" fail countermumatch"<<endl;
      for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand)
	printMuonInfo(&(cand->leg2()));
    }
    return 0;
  }
  countermumatch_++;



//   //Tau E/P cut
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.)
//       if(cand->leg1().eOverP()<0.2)
// 	continue;
    
//     diTauSelList_.push_back(*cand);
//   }
//   if(diTauSelList_.size()==0){
//     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertaueop"<<endl;
//     return 0;
//   }
//   countertaueop_++;

  
  //tau vtx
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){  

    if(!tauVertexCut(&(cand->leg1())))continue;
    
    if(reco::deltaR(cand->leg1().p4().eta(),cand->leg1().p4().phi(),
		    cand->leg2().p4().eta(),cand->leg2().p4().phi()
		    )<0.5)continue; //Andrew has this cut
    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertauvtx"<<endl;
    return 0;
  }
  countertauvtx_++;


  //Tau anti-mu cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("againstMuonTight")>0.5)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertaumuveto"<<endl;
    return 0;
  }
  countertaumuveto_++;


  //Tau anti-e cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().tauID("againstElectronLoose")>0.5)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertaueveto"<<endl;
    return 0;
  }
  countertaueveto_++;


//   /////////Tau Isolation
//   tmpditaulist=diTauSelList_;
//   diTauSelList_.clear();
//   for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
//     if(cand->leg1().tauID("byLooseIsoMVA")>0.5){
//       diTauSelList_.push_back(*cand);
//     }
//   }
//   if(diTauSelList_.size()==0){
//     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertauiso"<<endl;
//     return 0;
//   }
//   countertauiso_++;


  //Tau Trig-Match
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    bool matchtau=0;
    if(trigPaths_.size()==0)matchtau=1;//no match requirement
    for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
      if(trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance(),15)
	 ||trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance(),0) //seems taus were set to 0 in 2012
	 )
	matchtau=1;
    }
    
    if(matchtau)
      diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertaumatch"<<endl;
    return 0;
  }
  countertaumatch_++;



  /////////Isolation cuts
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    

    if(
       //-------mu iso cut here:
       cand->leg2().relIso(0.5,1)<0.1
       //cand->leg2().relIso(0.5)<0.1
       //if(cand->leg2().sourcePtr()->userFloat("mvaIsoRings")<.xx) continue;     
       
       //------tau iso cut here
       //if(diTauSel_->leg1().tauID("byVLooseCombinedIsolationDeltaBetaCorr")<0.5)
       // && cand->leg1().tauID("byLooseCombinedIsolationDeltaBetaCorr") > 0.5
       //if(diTauSel_->leg1().tauID("byMediumCombinedIsolationDeltaBetaCorr")<0.5)
       //if(diTauSel_->leg1().tauID("byTightCombinedIsolationDeltaBetaCorr")<0.5)
       
       && cand->leg1().tauID("byLooseIsoMVA")>0.5
       ){
      diTauSelList_.push_back(*cand);
    }
  }

  categoryIso_=1;//category gets set to "signal" by default
  nditau_=diTauSelList_.size();//save number of cadidates in event
  if(diTauSelList_.size()==0){//no isolated candidates were found, see if there are any anti-isolated ones
    categoryIso_=2;//category gets set to "sideband" if there was no candidate in the signal box
    for(std::vector<cmg::TauMu>::const_iterator cand = tmpditaulist.begin(); cand != tmpditaulist.end(); ++cand)
      diTauSelList_.push_back(*cand);
  }
 
  
  //choose the best candidate
  nditau_=diTauSelList_.size();//keep track of the number of candidates per event
  diTauSel_=&(*diTauSelList_.begin());
  double highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand)
    if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
      diTauSel_=&(*cand);
      highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
    }


//   if(vetoDiLepton()){
//     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterveto"<<endl;
//     return 0;
//   }
//   counterveto_++;


  if(vetoDiLepton()){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterveto"<<endl;
    return 0;
  }
  counterveto_++;

  //truth match 
  truthEventType_=0;
  if(genBoson_)
    fillTruthEventType(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),diTauSel_->leg2().eta(),diTauSel_->leg2().phi());
  if(sampleTruthEventType_>0)
    if(sampleTruthEventType_!=truthEventType_){
      if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertruth"<<endl;
      return 0;
    }
  countertruth_++;



   if(printSelectionPass_){
     cout<<" Passed all selections : "<<endl;
     printMuonInfo(&(diTauSel_->leg2()));
     printTauInfo(&(diTauSel_->leg1()));
   }


  return 1;
}

bool TauMuFlatNtp::fill(){
  
  BaseFlatNtp::fill();
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass BaseFlatNtp fill "<<endl;

  /////mu and tau trigger efficiency weight
  eventweight_=1.;
  triggerEffWeight_=1.;
  triggerEffWeightMu_=1.;
  triggerEffWeightTau_=1.;
  selectionEffWeight_=1.;
  selectionEffWeightId_=1.;
  selectionEffWeightIso_=1.;

  for(long i=0;i<5;i++){
    triggerEffWeightsMu_[i]=1.;
    triggerEffWeightsTau_[i]=1.;
    selectionEffWeightsId_[i]=1.;
    selectionEffWeightsIso_[i]=1.;
  }

  if(dataType_==0 || dataType_==2){
    
    if(dataPeriodFlag_==2011){
      if(trigPaths_.size()>0){
	if(triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightTau_ = triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	

	if(triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightMu_ = triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

      }else{
	triggerEffWeightTau_ = triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightMu_ = triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }
      
      //id+isolation corrections
      selectionEffWeight_ = selectionEff_.effCorrMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }


    if(dataPeriodFlag_==2012){

      if(trigPaths_.size()>0){
	if(triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[0] = triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[0] =  triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	if(triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[1] = triggerEff_.effTau2012ABC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[1] = triggerEff_.effMu2012_Rebecca_TauMu_ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	if(triggerEff_.effTau_muTau_MC_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[2] = triggerEff_.effTau_muTau_Data_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau_muTau_MC_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[2] = triggerEff_.effMu_muTau_Data_2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	if(triggerEff_.effTau_muTau_MC_2012ABCD(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[3] = triggerEff_.effTau_muTau_Data_2012ABCD(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau_muTau_MC_2012ABCD(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.effMu_muTau_MC_2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[3] = triggerEff_.effMu_muTau_Data_2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.effMu_muTau_MC_2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());


      }else{
	triggerEffWeightsTau_[0] = triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[0] =  triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	triggerEffWeightsTau_[1] = triggerEff_.effTau2012ABC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[1] = triggerEff_.effMu2012_Rebecca_TauMu_ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	triggerEffWeightsTau_[2] = triggerEff_.effTau_muTau_Data_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[2] = triggerEff_.effMu_muTau_Data_2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	triggerEffWeightsTau_[3] = triggerEff_.effTau_muTau_Data_2012ABCD(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[3] = triggerEff_.effMu_muTau_Data_2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }


      ////////id weight
      selectionEffWeightsId_[0] = selectionEff_.effCorrMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      selectionEffWeightsId_[1] = selectionEff_.effCorrMuID2012ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      selectionEffWeightsId_[2] = selectionEff_.effCorrMuID2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      selectionEffWeightsId_[3] = selectionEff_.effCorrMuID2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());


      //iso correction only to MC
      if(dataType_==0){
	selectionEffWeightsIso_[0] = 1.;//need to separate the above function
	selectionEffWeightsIso_[1] = selectionEff_.effCorrMuIso2012ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	selectionEffWeightsIso_[2] = selectionEff_.effCorrMuIso2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	selectionEffWeightsIso_[3] = selectionEff_.effCorrMuIso2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }
    }

  }

  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass weights "<<endl;


  ///////////////check some filter paths
  for(long p=0;p<10;p++){  trigPath_[p]=0;  trigTest_[p]=0;}
  int i=0;
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){//cmg ObjetSel
    if(trig_->begin()->hasSelection((*path)->label())){
      if(trig_->begin()->getSelection((*path)->label())){
	//trigPath_[i]=1;

	if(trigObjMatch(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),(*path)->label(),(*path)->process(),13)
	   &&
	   (trigObjMatch(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),(*path)->label(),(*path)->instance(),15)
	     ||trigObjMatch(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),(*path)->label(),(*path)->instance(),0)
	    )
	   )trigPath_[i]=1;
	
      }
    }
    i++;
  }
  int j=0;
  for(std::vector<edm::InputTag *>::const_iterator path=trigPathsTest_.begin(); path!=trigPathsTest_.end(); path++){//cmg ObjetSel
    if(trig_->begin()->hasSelection((*path)->label())){
      if(trig_->begin()->getSelection((*path)->label())){
	
	//trigTest_[j]=1;
	if(trigObjMatch(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),(*path)->label(),(*path)->process(),13)
	   &&
	   (trigObjMatch(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),(*path)->label(),(*path)->instance(),15)
	    ||trigObjMatch(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),(*path)->label(),(*path)->instance(),0)
	    )
	   )trigTest_[i]=1;
	
      }
    }
    j++;
  }
  ///////////////



  mupt_=diTauSel_->leg2().pt();
  mueta_=diTauSel_->leg2().eta();
  muphi_=diTauSel_->leg2().phi();
  muiso_=diTauSel_->leg2().relIso(0.5,1);
  muisomva_=(*(diTauSel_->leg2().sourcePtr()))->userFloat("mvaIsoRings");
  //mudz_=diTauSel_->leg2().dz();
  //mudxy_=diTauSel_->leg2().dxy();
  mudz_=(*(diTauSel_->leg2().sourcePtr()))->innerTrack()->dz(PV_->position());
  mudxy_=(*(diTauSel_->leg2().sourcePtr()))->innerTrack()->dxy(PV_->position());
  mux_=diTauSel_->leg2().vertex().x();
  muy_=diTauSel_->leg2().vertex().y();
  muz_=diTauSel_->leg2().vertex().z();
  mutruth_=truthMatchLeg(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),mutruthpt_,mutrutheta_,mutruthstatus_);
  mucharge_=diTauSel_->leg2().charge();

  taumass_=diTauSel_->leg1().p4().M();
  taupt_=diTauSel_->leg1().pt();
  taupx_=diTauSel_->leg1().p4().x();
  taupy_=diTauSel_->leg1().p4().y();
  taueta_=diTauSel_->leg1().eta();
  tauphi_=diTauSel_->leg1().phi();
  taudz_=computeDz(diTauSel_->leg1().leadChargedHadrVertex(),diTauSel_->leg1().p4());   //taudz_=diTauSel_->leg1().dz();
  taudxy_=computeDxy(diTauSel_->leg1().leadChargedHadrVertex(),diTauSel_->leg1().p4());   //taudxy_=diTauSel_->leg1().dxy();
  taucharge_=diTauSel_->leg1().charge();

  tautruth_=truthMatchLeg(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),tautruthpt_,tautrutheta_,tautruthstatus_);
  //cout<<tautruth_<<" "<<tautruthstatus_<<endl;
  tauehop_=diTauSel_->leg1().eOverP();
  taueop_=diTauSel_->leg1().leadChargedHadrEcalEnergy()/diTauSel_->leg1().p();
  tauhoe_=diTauSel_->leg1().leadChargedHadrHcalEnergy()/diTauSel_->leg1().leadChargedHadrEcalEnergy();
  taudecaymode_=diTauSel_->leg1().decayMode();
  taux_=diTauSel_->leg1().leadChargedHadrVertex().x();
  tauy_=diTauSel_->leg1().leadChargedHadrVertex().y();
  tauz_=diTauSel_->leg1().leadChargedHadrVertex().z();
  tauiso_=diTauSel_->leg1().relIso(0.5);
  tauisomva_=diTauSel_->leg1().tauID("byRawIsoMVA");

  tauleadpt_=diTauSel_->leg1().leadChargedHadrPt();  
  tauleadhcal_=diTauSel_->leg1().leadChargedHadrHcalEnergy();
  tauleadecal_=diTauSel_->leg1().leadChargedHadrEcalEnergy();

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
  tauisodiscmva_=0;
  if(diTauSel_->leg1().tauID("byLooseIsoMVA")>0.5)tauisodiscmva_=1;
  if(diTauSel_->leg1().tauID("byMediumIsoMVA")>0.5)tauisodiscmva_=2;
  if(diTauSel_->leg1().tauID("byTightIsoMVA")>0.5)tauisodiscmva_=3;

  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass object vars "<<endl;  

  ditaumass_=diTauSel_->mass();
  if(smearVisMass0pi0_>0.&&taudecaymode_==0) ditaumass_ += gRandom->Gaus(0,smearVisMass0pi0_);
  if(smearVisMass1pi0_>0.&&taudecaymode_==1) ditaumass_ += gRandom->Gaus(0,smearVisMass1pi0_);

  ditaucharge_=diTauSel_->charge();
  ditaueta_=diTauSel_->eta();
  ditaupt_=diTauSel_->pt();
  ditauphi_=diTauSel_->phi();
  svfitmass_=0.;//diTauSel_->massSVFit();
  mutaucostheta_=diTauSel_->leg1().p4().Vect().Dot(diTauSel_->leg2().p4().Vect());
  ditaudeltaR_= reco::deltaR(diTauSel_->leg1().p4().eta(),diTauSel_->leg1().p4().phi(),
			     diTauSel_->leg2().p4().eta(),diTauSel_->leg2().p4().phi()
			     ); 
  ditaudeltaEta_=diTauSel_->leg2().p4().eta()-diTauSel_->leg1().p4().eta();;
  ditaudeltaPhi_=diTauSel_->leg2().p4().phi()-diTauSel_->leg1().p4().phi();;


  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass ditau vars"<<endl;

  ///get the jets //need the jets here because of randomization of mT
  edm::Handle< std::vector<cmg::PFJet> > eventJetList;
  iEvent_->getByLabel(pfJetListTag_,eventJetList);
  fullJetList_.clear();
  for(std::vector<cmg::PFJet>::const_iterator jet=eventJetList->begin(); jet!=eventJetList->end(); ++jet)
    fullJetList_.push_back(&(*jet));
  
  //apply pt and eta cuts on jets
  fillPFJetList(&fullJetList_,&pfJetList_);
  fillPFJetListLC(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),&pfJetList_,&pfJetListLC_);
  fillPFJetListLepLC(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),&pfJetList_,&pfJetListLepLC_);
  fillJetVariables();
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass Jets fillJetVariables "<<endl;


  fillPFJetList20(&fullJetList_,&pfJetList20_);
  fillPFJetListLC(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),&pfJetList20_,&pfJetList20LC_);
  fillJetVariables20();
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass Jets fillJetVariables20 "<<endl;


  fillPFJetListBTag(&pfJetList20LC_,&pfJetListBTagLC_);
  fillPFJetListBTagLoose(&pfJetList20LC_,&pfJetListBTagLCLoose_);
  fillBJetVariables();
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass b-tag fillJetVariables"<<endl;


  //find the jet matching to the tau
  taujetpt_=0.;
  taujeteta_=0.;
  const cmg::PFJet * taujet = findJet(&fullJetList_,diTauSel_->leg1().eta(),diTauSel_->leg1().phi());
  if(taujet) taujetpt_=taujet->pt();
  if(taujet) taujeteta_=taujet->eta();
  
  //find the jet matching to the mu
  mujetpt_=0.;
  mujeteta_=0.;
  const cmg::PFJet * mujet = findJet(&fullJetList_,diTauSel_->leg2().eta(),diTauSel_->leg2().phi());
  if(mujet) mujetpt_=mujet->pt();
  if(mujet) mujeteta_=mujet->eta();
  
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass Jets "<<endl;

  ///////here decide which met 
  metSig_=0;
  metpt_=0.;
  metphi_=0.;
  //needed for mva MET
  diobjectmet_=diTauSel_->met();
  diobjectindex_=0;
  for(std::vector<cmg::TauMu>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){
    if(cand->mass()==diTauSel_->mass()) break;
    diobjectindex_++;
  }
  fillMET();


  //
  pftransversemass_=sqrt(2*mupt_*pfmetpt_*(1-cos(muphi_-pfmetphi_)));
  transversemass_=sqrt(2*mupt_*metP4_.pt()*(1-cos(muphi_-metP4_.phi())));
  compZeta(diTauSel_->leg2().p4(),diTauSel_->leg1().p4(),metP4_.px(),metP4_.py(),&pZeta_,&pZetaVis_);

  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass MET "<<endl;
  
  //----SVFit
  taup4_=diTauSel_->leg1().p4();
  mup4_=diTauSel_->leg2().p4();
  runSVFit();

  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass SVFit  "<<endl;


  //VBF variables
  vbfmva_=0.;
  if(njet20_>=2) fillVBFMVA();
  vbfmva2012_=0.;
  if(njet20_>=2) fillVBFMVA2012();

  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Done fill "<<endl;
  return 1;
}



bool TauMuFlatNtp::vetoDiLepton(){


  const cmg::Muon * mu1=0;
  const cmg::Muon * mu2=0;
  for(std::vector<cmg::Muon>::const_iterator m=leptonVetoListMuon_->begin(); m!=leptonVetoListMuon_->end(); ++m){  
    if(m->pt()<=15.0)continue;
    if(fabs(m->eta())>=2.4)continue;

    if(!muonVertexCut(&(*m)))continue;

    if(!(m->isTracker()))continue; 
    if(!(m->isGlobal()))continue; 
    if((*(m->sourcePtr()))->userFloat("isPFMuon")<0.5) continue;

    if(m->relIso(0.5,1)>=0.3)continue;        

    if(!mu1)mu1=&(*m);
    else if(m->charge()!=mu1->charge()
	    && reco::deltaR(m->p4().eta(),m->p4().phi(),mu1->p4().eta(),mu1->p4().phi()) > 0.15 ){//Andrew has this cut
      mu2=&(*m);
    }

  }
  if(mu1&&mu2){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<"      fail dilepon veto"<<endl;
    //cout<<"e1 info: "<<endl; printElectronInfo(mu1);
    //cout<<"e2 info: "<<endl;printElectronInfo(mu2);
    return 1;
  }


  
  //3rd lepton veto
  if(!thirdLeptonVeto()){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<"      fail 3rd lepton veto"<<endl;
    return 1;
  }


  return 0;
}


void TauMuFlatNtp::endJob(){
  BaseFlatNtp::endJob();

  cout<<"counterev = "<<counterev_<<endl;
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
  cout<<"counterveto = "<<counterveto_<<endl;
  cout<<"countertruth = "<<countertruth_<<endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
 
DEFINE_FWK_MODULE(TauMuFlatNtp);


