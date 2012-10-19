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


  if(vetoDiLepton()){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterveto"<<endl;
    return 0;
  }
  counterveto_++;

  if(diTauList_->size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterpresel"<<endl;  
    return 0;
  }
  counterpresel_++;

  std::vector<cmg::TauMu> tmpditaulist=*diTauList_;
  diTauSelList_.clear();


  //muon vtx 
  //  tmpditaulist=diTauSelList_;
  //  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    //if( fabs(cand->leg2().dxy())>0.045)continue;
    //if( fabs(cand->leg2().dz())>0.2 )continue;
    
    //track.isNonnull() && track.isAvailable()
    if(!((*(cand->leg2().sourcePtr()))->innerTrack().isNonnull()))continue;
    if(!((*(cand->leg2().sourcePtr()))->innerTrack().isAvailable()))continue;

    if(fabs((*(cand->leg2().sourcePtr()))->innerTrack()->dxy(PV_->position())) > 0.045 ) continue;
    if(fabs((*(cand->leg2().sourcePtr()))->innerTrack()->dz(PV_->position()))  > 0.2 ) continue;
    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countermuvtx"<<endl;  
    return 0;
  }
  countermuvtx_++;



  //muion id cuts
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    
    //     //old VBTF cuts
    //     if(cand->leg2().isGlobal()
    //        &&cand->leg2().isTracker()
    //        &&cand->leg2().numberOfValidTrackerHits() > 10
    //        &&cand->leg2().numberOfValidPixelHits() > 0
    //        &&cand->leg2().numberOfValidMuonHits() > 0
    //        &&cand->leg2().numberOfMatches() > 1
    //        &&cand->leg2().normalizedChi2() < 10
    //        ){
      
    //     //"Loose Muon" 
    //     if((*(cand->leg2().sourcePtr()))->userFloat("isPFMuon")>0.5
    //        && (cand->leg2().isGlobal() || cand->leg2().isTracker())
    //        ){      
      
    //"Tight Muon" 
    if(!(cand->leg2().isGlobal()))continue; //cout<<"pass"<<endl;
    if(!((*(cand->leg2().sourcePtr()))->userFloat("isPFMuon")>0.5))continue;// cout<<"pass"<<endl;
    if(!(cand->leg2().normalizedChi2() < 10))continue; //cout<<"pass"<<endl;
    if(!(cand->leg2().numberOfValidMuonHits() > 0))continue; //cout<<"pass"<<endl;
    if(!(cand->leg2().numberOfMatches() > 1))continue; //cout<<"pass"<<endl;
    //if(!(cand->leg2().numberOfValidPixelHits() > 0))continue; //cout<<"pass"<<endl; //pixelHits is not filled right in the cmgMuon
    if(!((*(cand->leg2().sourcePtr()))->innerTrack()->hitPattern().numberOfValidPixelHits() > 0))continue; //cout<<"pass"<<endl; 
    if(!(cand->leg2().trackerLayersWithMeasurement() > 5))continue; //cout<<"pass"<<endl;
    
    diTauSelList_.push_back(*cand);
      
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countermuid"<<endl;
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
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countermumatch"<<endl;
    return 0;
  }
  countermumatch_++;



  //Tau E/P cut
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    if(cand->leg1().decayMode()==0&&cand->leg1().p()>0.)
      if(cand->leg1().eOverP()<0.2)
	continue;
    
    diTauSelList_.push_back(*cand);
  }
  if(diTauSelList_.size()==0){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertaueop"<<endl;
    return 0;
  }
  countertaueop_++;

  
  //tau vtx
  tmpditaulist=diTauSelList_;
  diTauSelList_.clear();
  for(std::vector<cmg::TauMu>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
    //if(fabs(cand->leg1().dxy())>0.045) continue;
    //if(fabs(cand->leg1().dz())>0.2 ) continue;
      
    if(fabs(computeDxy(cand->leg1().leadChargedHadrVertex(),cand->leg1().p4()))>0.045)continue;
    if(fabs(computeDz(cand->leg1().leadChargedHadrVertex(),cand->leg1().p4()))>0.2)continue;


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


  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" passed TauMu selections "<<endl;

  return 1;
}

bool TauMuFlatNtp::fill(){
  
  BaseFlatNtp::fill();
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass BaseFlatNtp fill "<<endl;

  /////mu and tau trigger efficiency weight
  triggerEffWeight_=1.;
  selectionEffWeight_=1.;
  if(dataType_==0 || dataType_==2){
    
    if(dataPeriodFlag_==2011){
      if(trigPaths_.size()>0){
	if(triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effLooseTau15MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.effIsoMu15MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }else{
	triggerEffWeight_ *= triggerEff_.effTau2011AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeight_ *= triggerEff_.effMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }
      
      //id+isolation corrections
      selectionEffWeight_ *= selectionEff_.effCorrMu2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }
    if(dataPeriodFlag_==2012){
//       if(trigPaths_.size()>0){
// 	if(triggerEff_.effTau2012MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
// 	  triggerEffWeight_ *= triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
// 	    /triggerEff_.effTau2012MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
// 	if(triggerEff_.effMu2012MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
// 	  triggerEffWeight_ *= triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
// 	    /triggerEff_.effMu2012MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
//       }else{
// 	triggerEffWeight_ *= triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
// 	triggerEffWeight_ *= triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
//       }
//       //id+isolation corrections
//       selectionEffWeight_ *= selectionEff_.effCorrMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

//       ////2012 A + B crosscheck with 53X MC
//       if(trigPaths_.size()>0){
// 	if(triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
// 	  triggerEffWeight_ *= triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
// 	    /triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());

// 	if(triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
// 	  triggerEffWeight_ *= triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
// 	    /triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
//       }else{
// 	triggerEffWeight_ *= triggerEff_.effTau2012AB(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
// 	triggerEffWeight_ *= triggerEff_.effMu2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
//       }

      //////////2012 A+B+C
      if(trigPaths_.size()>0){
	if(triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeight_ *= triggerEff_.effTau2012ABC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau2012MC53X(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());

	if(triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeight_ *= triggerEff_.effMu2012_Rebecca_TauMu_ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauMu_IsoMu1753XMC (diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }else{
	triggerEffWeight_ *= triggerEff_.effTau2012ABC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeight_ *= triggerEff_.effMu2012_Rebecca_TauMu_ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }


      //id+isolation corrections
      selectionEffWeight_ *= selectionEff_.effCorrMu2012ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
    }

  }
  
  eventweight_=pupWeight_*embeddedGenWeight_*triggerEffWeight_*selectionEffWeight_*signalWeight_;

  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass weights "<<endl;

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
  mutruth_=-1;

  taumass_=diTauSel_->leg1().p4().M();
  taupt_=diTauSel_->leg1().pt();
  taueta_=diTauSel_->leg1().eta();
  tauphi_=diTauSel_->leg1().phi();
  //taudz_=diTauSel_->leg1().dz();
  //taudxy_=diTauSel_->leg1().dxy();
  taudz_=computeDz(diTauSel_->leg1().leadChargedHadrVertex(),diTauSel_->leg1().p4());
  taudxy_=computeDxy(diTauSel_->leg1().leadChargedHadrVertex(),diTauSel_->leg1().p4());
  
  tautruth_=truthMatchLeg(diTauSel_->leg1().eta(),diTauSel_->leg1().phi());
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
  ditaucharge_=diTauSel_->charge();
  ditaueta_=diTauSel_->eta();
  ditaupt_=diTauSel_->pt();
  ditauphi_=diTauSel_->phi();
  svfitmass_=0.;//diTauSel_->massSVFit();
  mutaucostheta_=diTauSel_->leg1().p4().Vect().Dot(diTauSel_->leg2().p4().Vect());

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
  fillBTagWeight();//uses pfJetList20LC
  if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" Pass Jets fillJetVariables20 "<<endl;


  fillPFJetListBTag(&pfJetList20LC_,&pfJetListBTagLC_);
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
  bool muminus=0;
  bool muplus=0;
  for(std::vector<cmg::Muon>::const_iterator m=leptonVetoListMuon_->begin(); m!=leptonVetoListMuon_->end(); ++m){  
    if(m->pt()<=15.0)continue;
    if(fabs(m->eta())>=2.4)continue;
    if(!(m->isTracker()))continue; 
    if(!(m->isGlobal()))continue; 
    if((*(m->sourcePtr()))->userFloat("isPFMuon")<0.5) continue;
    if(fabs((*(m->sourcePtr()))->innerTrack()->dz(PV_->position()))  > 0.2 ) continue;
    if(m->relIso(0.5,1)>=0.3)continue;        

    if(m->charge()==-1)muminus=1;
    if(m->charge()==1)muplus=1;
  }
  if(muminus&&muplus) return 1;


  //3rd lepton veto
  int nleptons=0;
  for(std::vector<cmg::Muon>::const_iterator m=leptonVetoListMuon_->begin(); m!=leptonVetoListMuon_->end(); ++m){  
    if(m->pt()<=10.0)continue;
    if(fabs(m->eta())>=2.4)continue;
    if(!(m->isTracker()))continue; 
    if(!(m->isGlobal()))continue; 
    if((*(m->sourcePtr()))->userFloat("isPFMuon")<0.5) continue;
    if(!(m->normalizedChi2() < 10))continue;
    if(!(m->numberOfValidMuonHits() > 0))continue; 
    if(!(m->numberOfMatches() > 1))continue; //cout<<"pass"<<endl;
    if(!(m->trackerLayersWithMeasurement() > 5))continue; 
   
    ///this is crashing saying track is not there , so must check 
    if(!((*(m->sourcePtr()))->innerTrack().isNonnull()))continue;
    if(!((*(m->sourcePtr()))->innerTrack().isAvailable()))continue;
    if(!((*(m->sourcePtr()))->innerTrack()->hitPattern().numberOfValidPixelHits() > 0))continue;
    if(fabs((*(m->sourcePtr()))->innerTrack()->dz(PV_->position()))  > 0.2 ) continue;
    if(fabs((*(m->sourcePtr()))->innerTrack()->dxy(PV_->position())) > 0.045 ) continue;

    if(m->relIso(0.5,1)>=0.3)continue;        
    nleptons++;
  }
  for(std::vector<cmg::Electron>::const_iterator m=leptonVetoListElectron_->begin(); m!=leptonVetoListElectron_->end(); ++m){  
    if(m->pt()<=10.0)continue;
    if(fabs(m->eta())>=2.5)continue;
    if(m->numberOfHits()!=0) continue;
    if(m->passConversionVeto()!=1) continue;

    if(!((*(m->sourcePtr()))->gsfTrack().isNonnull()))continue;
    if(!((*(m->sourcePtr()))->gsfTrack().isAvailable()))continue;     
    if(fabs((*(m->sourcePtr()))->gsfTrack()->dxy(PV_->position())) > 0.045 ) continue;
    if(fabs((*(m->sourcePtr()))->gsfTrack()->dz(PV_->position()))  > 0.2 ) continue;

    
    float mvaid=m->mvaNonTrigV0();
    float eta=(*(m->sourcePtr()))->superCluster()->eta();
    if(fabs(eta)<0.8)
      if(mvaid<0.925)continue; 
    if(0.8<=fabs(eta)&&fabs(eta)<1.479)
      if(mvaid<0.975)continue;
    if(1.479<=fabs(eta))
      if(mvaid<0.985)continue; 

    if( electronRelIsoDBCorr( &(*m) )>=0.3 ) continue; 
    nleptons++;
  }
  if(nleptons>=3)return 1;


  return 0;
}


void TauMuFlatNtp::endJob(){
  BaseFlatNtp::endJob();

  cout<<"counterev = "<<counterev_<<endl;
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
  cout<<"countertruth = "<<countertruth_<<endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
 
DEFINE_FWK_MODULE(TauMuFlatNtp);


