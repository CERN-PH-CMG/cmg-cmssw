
 #include "CMGTools/H2TauTau/plugins/TauEleFlatNtp.h"
 #include "TauAnalysis/SVFitStandAlone/interface/NSVfitStandaloneAlgorithm2011.h"
 #include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"


TauEleFlatNtp::TauEleFlatNtp(const edm::ParameterSet & iConfig):
   BaseFlatNtp(iConfig)
{
}


TauEleFlatNtp::~TauEleFlatNtp(){

}


void TauEleFlatNtp::beginJob(){

  BaseFlatNtp::beginJob();
  //  tree_->Branch("",&_,"/F");

  tree_->Branch("mumvaid",&mumvaid_,"mumvaid/F");
  tree_->Branch("tauantiemva",&tauantiemva_,"tauantiemva/F");


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



 bool TauEleFlatNtp::fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup){

   if(!BaseFlatNtp::fillVariables(iEvent,iSetup)) return 0;


   ///get the TauEle cands 
   iEvent.getByLabel(diTauTag_,diTauList_);

   return 1;
 }

 bool TauEleFlatNtp::applySelections(){
 

   //if none are selected returns 0
   diTauSel_=NULL;

   //trigger, n-vertex selection
   if(!BaseFlatNtp::applySelections()){
     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail counterev"<<endl;
     return 0;
   }
   counterev_++;


   std::vector<cmg::TauEle> tmpditaulist=*diTauList_;
   diTauSelList_.clear();


   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(cand->leg2().pt()<muPtCut_)continue;
     if(fabs(cand->leg2().eta())>muEtaCut_)continue;
     if(reco::deltaR(cand->leg1().p4().eta(),cand->leg1().p4().phi(),
		     cand->leg2().p4().eta(),cand->leg2().p4().phi()
		     )<0.5)continue;

     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail Electron preselection"<<endl;  
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printElectronInfo(&(cand->leg2()));
       }
     }
     return 0;
   }
   
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(cand->leg1().tauID("decayModeFinding")<0.5 )continue;
     if(cand->leg1().pt()<tauPtCut_)continue;
     if(fabs(cand->leg1().eta())>tauEtaCut_)continue;
	
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail Tau preselection"<<endl;  
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printTauInfo(&(cand->leg1()));
       }
     }
     return 0;
   }
   

   //electron vtx cut
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(!electronVertexCut(&(cand->leg2())))continue;
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail countermuvtx"<<endl;
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printElectronInfo(&(cand->leg2()));
       }
     }

     return 0;
   }
   countermuvtx_++;

   //ele id cuts
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(cand->leg2().numberOfHits()!=0) continue;
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0 && printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail cand->leg2().numberOfHits()"<<endl;

   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     //if(cand->leg2().passConversionVeto()!=1) continue; 
     if((*(cand->leg2().sourcePtr()))->passConversionVeto()!=1)continue;
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0 && printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail passConversionVeto()"<<endl;


   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(!electronMVATight(&(cand->leg2())))continue;
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail countermuid"<<endl;
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printElectronInfo(&(cand->leg2()));
       }
     }

     return 0;
   }
   countermuid_++;

   //muon trig-match
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     bool matchmu=0;
     if(trigPaths_.size()==0) matchmu=1;//no match requirement
     for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
       if(((*path)->instance()).compare("")==0) matchmu=1;
       if(trigObjMatch(cand->leg2().eta(),cand->leg2().phi(),(*path)->label(),(*path)->process(),11))
	 matchmu=1;
     }
  
     if(matchmu) 
       diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail countermumatch"<<endl;
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printElectronInfo(&(cand->leg2()));
	 
	 //print any objects which are with 0.5 
	 for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs_->begin(); obj!=trigObjs_->end(); obj++){
	   if(reco::deltaR(cand->leg2().eta(),cand->leg2().phi(),obj->eta(),obj->phi())<0.5){
	     cout<<obj->pt()<<" "<<obj->eta()<<" "<<obj->phi()<<" "<<obj->pdgId()<<endl;
	     obj->printSelections(cout);
	     cout<<endl;
	   }
	 }

       }
     }

     return 0;
   }
   countermumatch_++;

   
   //tau vtx
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){  

     if(!tauVertexCut(&(cand->leg1())))continue;
          
     diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail countertauvtx"<<endl;
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printTauInfo(&(cand->leg1()));
       }
     }
     return 0;
   }
   countertauvtx_++;


   //Tau anti-mu cut
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     if(cand->leg1().tauID(tauAntiMuDisc_.c_str())<0.5)continue;
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
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    

     if(dataPeriodFlag_==2011){
       if(cand->leg1().tauID("againstElectronMedium")<0.5)continue;
       if(cand->leg1().tauID("againstElectronMVA")<0.5)continue;
     }

     if(
	cand->leg1().tauID(tauAntiEDisc_.c_str())>0.5
	|| passAntiEMVA(cand->leg1().tauID("againstElectronMVA3category"),cand->leg1().tauID("againstElectronMVA3raw"),0)
	){
       diTauSelList_.push_back(*cand);
     }
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail countertaueveto"<<endl;
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printTauInfo(&(cand->leg1()));
       }
     }
     return 0;
   }
   countertaueveto_++;


   /////////Tau Isolation
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     //if(cand->leg1().tauID("byLooseIsoMVA")>0.5)
     if(cand->leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")<20.0)
       {
	 diTauSelList_.push_back(*cand);
       }
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<" fail countertauiso"<<endl;
     return 0;
   }
   countertauiso_++;

   //Tau Trig-Match
   tmpditaulist=diTauSelList_;
   diTauSelList_.clear();
   for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){    
     bool matchtau=0;
     if(trigPaths_.size()==0)matchtau=1;//no match requirement
     for(std::vector<edm::InputTag*>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
       if(((*path)->instance()).compare("")==0) matchtau=1;
       if(trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance(),15)
	  || trigObjMatch(cand->leg1().eta(),cand->leg1().phi(),(*path)->label(),(*path)->instance(),0))
 	  matchtau=1;
     }
  
     if(matchtau)
       diTauSelList_.push_back(*cand);
   }
   if(diTauSelList_.size()==0){
     if(printSelectionPass_){
       cout<<runnumber_<<":"<<eventid_<<" fail countertaumatch"<<endl;
       for(std::vector<cmg::TauEle>::const_iterator cand=tmpditaulist.begin(); cand!=tmpditaulist.end(); ++cand){
	 printTauInfo(&(cand->leg1()));
       }
     }
     return 0;
   }
   countertaumatch_++;


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
	//&& cand->leg1().tauID("byLooseIsoMVA")>0.5
	&& cand->leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits")<1.5

        ){
       diTauSelList_.push_back(*cand);
     }
   }

   categoryIso_=1;//category gets set to "signal" by default
   nditau_=diTauSelList_.size();
   if(diTauSelList_.size()==0){//no isolated candidates were found, see if there are any anti-isolated ones

     if(printSelectionPass_) cout<<runnumber_<<":"<<eventid_<<" fail isolation cuts"<<endl;

     categoryIso_=2;//category gets set to "sideband" if there was no candidate in the signal box
     for(std::vector<cmg::TauEle>::const_iterator cand = tmpditaulist.begin(); cand != tmpditaulist.end(); ++cand)
       diTauSelList_.push_back(*cand);
   }


   //choose the best candidate
   nditau_=diTauSelList_.size();//keep track of the number of candidates per event


   //    diTauSel_=&(*diTauSelList_.begin());
   //    double highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
   //    for(std::vector<cmg::TauEle>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand)
   //      if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
   //        diTauSel_=&(*cand);
   //        highsumpt=diTauSel_->leg1().pt()+diTauSel_->leg2().pt();
   //      }


   diTauSel_=0;
   double highsumpt=0.;
   for(std::vector<cmg::TauEle>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand){
     if(cand->leg2().charge() != cand->leg1().charge()){//Choose first among OS pairs
       if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
	 diTauSel_=&(*cand);
	 highsumpt= diTauSel_->leg1().pt()+ diTauSel_->leg2().pt();
       }
     }
   }
   if(! diTauSel_){//if no OS pair found try SS pairs
     highsumpt=0.;
     for(std::vector<cmg::TauEle>::const_iterator cand=diTauSelList_.begin(); cand!=diTauSelList_.end(); ++cand){
       if(cand->leg2().charge() == cand->leg1().charge()){
	 if(cand->leg1().pt()+cand->leg2().pt()>highsumpt){
	   diTauSel_=&(*cand);
	   highsumpt= diTauSel_->leg1().pt()+ diTauSel_->leg2().pt();
	 }
       }
     }
   }



   ////
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
     printElectronInfo(&(diTauSel_->leg2()));
     printTauInfo(&(diTauSel_->leg1()));
   }
   
   return 1;
 }

bool TauEleFlatNtp::fill(){

  BaseFlatNtp::fill();

  /////mu and tau trigger efficiency weight
  eventweight_=1.;
  triggerEffWeight_=1.;
  triggerEffWeightMu_=1.;
  triggerEffWeightTau_=1.;
  selectionEffWeight_=1.;
  selectionEffWeightId_=1.;
  selectionEffWeightIso_=1.;
  embeddedLeptonWeight_=1.;

  for(long i=0;i<5;i++){
    triggerEffWeightsMu_[i]=1.;
    triggerEffWeightsTau_[i]=1.;
    selectionEffWeightsId_[i]=1.;
    selectionEffWeightsIso_[i]=1.;
  }

  if(dataType_==0 || dataType_==2){
  
    ///trigger corrections
    if(dataPeriodFlag_==2011){

      if(dataType_==0){
        if( triggerEff_.effMediumIsoTau20MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())> 0.)
          triggerEffWeightsTau_[2] = triggerEff_.effTau2011AB_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
            /triggerEff_.effMediumIsoTau20MC(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());

        if(triggerEff_.effEle18MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta()) > 0.)
          triggerEffWeightsMu_[2] = triggerEff_.effEle2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
            /triggerEff_.effEle18MC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }
      if(dataType_==2){
        triggerEffWeightsTau_[2] = triggerEff_.effTau2011AB_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
        triggerEffWeightsMu_[2] = triggerEff_.effEle2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }
      
      selectionEffWeightsId_[2] = selectionEff_.effCorrEle2011AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      if(dataType_==0){
        selectionEffWeightsIso_[2] = 1.;//need to separate above function
      }

    }

    if(dataPeriodFlag_==2012){
       
      if(dataType_==0){
	if(triggerEff_.eff2012Tau20MC53X_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[0] = triggerEff_.effTau2012AB_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.eff2012Tau20MC53X_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta()); 
	if(triggerEff_.eff_2012_Rebecca_TauEle_Ele2253XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[0] =  triggerEff_.effEle2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauEle_Ele2253XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	if(triggerEff_.eff2012Tau20MC53X_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[1] = triggerEff_.effTau2012ABC_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.eff2012Tau20MC53X_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.eff_2012_Rebecca_TauEle_Ele2253XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[1] = triggerEff_.effEle2012_Rebecca_TauEle_ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauEle_Ele2253XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	if(triggerEff_.effTau_eTau_MC_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[2] = triggerEff_.effTau_eTau_Data_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau_eTau_MC_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.eff_2012_Rebecca_TauEle_Ele2253XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[2] = triggerEff_.effEle_eTau_Data_2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.eff_2012_Rebecca_TauEle_Ele2253XMC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	if(triggerEff_.effTau_eTau_MC_2012ABCDSummer13(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())>0.)
	  triggerEffWeightsTau_[3] = triggerEff_.effTau_eTau_Data_2012ABCDSummer13(diTauSel_->leg1().pt(),diTauSel_->leg1().eta())
	    /triggerEff_.effTau_eTau_MC_2012ABCDSummer13(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	if(triggerEff_.effEle_eTau_MC_2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())>0.)
	  triggerEffWeightsMu_[3] = triggerEff_.effEle_eTau_Data_2012ABCDSummer13(diTauSel_->leg2().pt(),diTauSel_->leg2().eta())
	    /triggerEff_.effEle_eTau_MC_2012ABCD(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }
      if(dataType_==2){//no trigger applied --> apply Data efficiency
	triggerEffWeightsTau_[0] = triggerEff_.effTau2012AB_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[0] =   triggerEff_.effEle2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	triggerEffWeightsTau_[1] = triggerEff_.effTau2012ABC_TauEle(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[1] = triggerEff_.effEle2012_Rebecca_TauEle_ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	triggerEffWeightsTau_[2] =  triggerEff_.effTau_eTau_Data_2012D(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[2] = triggerEff_.effEle_eTau_Data_2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	triggerEffWeightsTau_[3] =  triggerEff_.effTau_eTau_Data_2012ABCDSummer13(diTauSel_->leg1().pt(),diTauSel_->leg1().eta());
	triggerEffWeightsMu_[3] = triggerEff_.effEle_eTau_Data_2012ABCDSummer13(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

	//special electron id SF for rec-hit embedded
	embeddedLeptonWeight_= selectionEff_.elecEffSFTight(diTauSel_->leg2().pt(),(*(diTauSel_->leg2().sourcePtr()))->superCluster()->eta());   
      }

      //electron id SF
      selectionEffWeightsId_[0] = selectionEff_.effCorrEle2012AB(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      selectionEffWeightsId_[1] = selectionEff_.effCorrEleID2012ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      selectionEffWeightsId_[2] = selectionEff_.effCorrEleID2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      selectionEffWeightsId_[3] = selectionEff_.effCorrEleID2012ABCDSummer13(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());

      //Electron isolation SF only for MC
      if(dataType_==0){
	selectionEffWeightsIso_[0] = 1.;//need to separate
	selectionEffWeightsIso_[1] = selectionEff_.effCorrEleIso2012ABC(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	selectionEffWeightsIso_[2] = selectionEff_.effCorrEleIso2012D(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
	selectionEffWeightsIso_[3] = selectionEff_.effCorrEleIso2012ABCDSummer13(diTauSel_->leg2().pt(),diTauSel_->leg2().eta());
      }


    }
     
  }

   
  ///////////////check some filter paths
  for(long p=0;p<10;p++){  trigPath_[p]=0;  trigTest_[p]=0;}
  int i=0;
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){//cmg ObjetSel
    if(trig_->begin()->hasSelection((*path)->label())){
      if(trig_->begin()->getSelection((*path)->label())){
	//trigPath_[i]=1;

	if(trigObjMatch(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),(*path)->label(),(*path)->process(),11)
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
	if(trigObjMatch(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),(*path)->label(),(*path)->process(),11)
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
   muiso_= electronRelIsoDBCorr( &(diTauSel_->leg2()) ); //diTauSel_->leg2().relIso(0.5);
   muisomva_=(*(diTauSel_->leg2().sourcePtr()))->userFloat("mvaIsoRings");
   mudz_=diTauSel_->leg2().dz();
   mudxy_=diTauSel_->leg2().dxy();
   mux_=diTauSel_->leg2().vertex().x();
   muy_=diTauSel_->leg2().vertex().y();
   muz_=diTauSel_->leg2().vertex().z();
   mutruth_=truthMatchLeg(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),mutruthpt_,mutrutheta_,mutruthstatus_);
   mumvaid_=diTauSel_->leg2().mvaNonTrigV0();
   mucharge_=diTauSel_->leg2().charge();


   fillTau(&(diTauSel_->leg1()));


   ditaucharge_=diTauSel_->charge();
   ditaumass_=diTauSel_->mass();
   if(smearVisMass0pi0_>0.&&taudecaymode_==0) ditaumass_ += gRandom->Gaus(0,smearVisMass0pi0_);
   if(smearVisMass1pi0_>0.&&taudecaymode_==1) ditaumass_ += gRandom->Gaus(0,smearVisMass1pi0_);
   taup4_=diTauSel_->leg1().p4();
   mup4_=diTauSel_->leg2().p4();
   fillDiTauVars();
   

   //apply pt and eta cuts on jets
   fillPFJetList(&fullJetList_,&pfJetList_);
   fillPFJetListLC(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),&pfJetList_,&pfJetListLC_);
   fillPFJetListLepLC(diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),&pfJetList_,&pfJetListLepLC_);
   fillJetVariables();

   //jets of pt>20
   fillPFJetList20(&fullJetList_,&pfJetList20_);
   fillPFJetListLC(diTauSel_->leg1().eta(),diTauSel_->leg1().phi(),diTauSel_->leg2().eta(),diTauSel_->leg2().phi(),&pfJetList20_,&pfJetList20LC_);
   fillJetVariables20();
   //fillBTagWeight();

   //B-jets
   fillPFJetListBTag(&pfJetList20LC_,&pfJetListBTagLC_);
   fillPFJetListBTagLoose(&pfJetList20LC_,&pfJetListBTagLCLoose_);
   fillBJetVariables();


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
  


   //////////////MET/////////////////
   metSig_=0;
   metpt_=0.;
   metphi_=0.;
   //needed for mva MET
   diobjectmet_=diTauSel_->met();
   diobjectindex_=0;
   for(std::vector<cmg::TauEle>::const_iterator cand=diTauList_->begin(); cand!=diTauList_->end(); ++cand){
     if(cand->mass()==diTauSel_->mass()) break;
     diobjectindex_++;
   }
   fillMET();
  
   fillDiTauMETVars();

   runSVFit();

   //VBF 
   vbfmva_=0.;
   if(njet20_>=2) fillVBFMVA();
   vbfmva2012_=0.;
   if(njet20_>=2) fillVBFMVA2012();


   return 1;
 }

 void TauEleFlatNtp::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){

   fillVariables(iEvent,iSetup);
   if(!applySelections()) return;
   fill();
   tree_->Fill();
 }



 bool TauEleFlatNtp::vetoDiLepton(){

   const cmg::Electron * mu1=0;
   const cmg::Electron * mu2=0;
   for(std::vector<cmg::Electron>::const_iterator m=leptonVetoListElectron_->begin(); m!=leptonVetoListElectron_->end(); ++m){  
     if(m->pt()<=15.0)continue;
     if(fabs(m->eta())>=2.5)continue;
     if(!electronVertexCut(&(*m)))continue;
     if(!electronIDWP95(&(*m)))continue;
     if( electronRelIsoDBCorr( &(*m) )>=0.3 ) continue; 

     if(!mu1)mu1=&(*m);
     else if(m->charge()!=mu1->charge()
	     && reco::deltaR(m->p4().eta(),m->p4().phi(),
			     mu1->p4().eta(),mu1->p4().phi()) > 0.15 ){//Andrew has this cut
       mu2=&(*m);
     }
   }

   if(mu1&&mu2){
     if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<"      fail dilepon veto"<<endl;
     //cout<<"e1 info: "<<endl; printElectronInfo(mu1);
     //cout<<"e2 info: "<<endl; printElectronInfo(mu2);
     return 1;
   }

  
  //3rd lepton veto
  if(!thirdLeptonVeto()){
    if(printSelectionPass_)cout<<runnumber_<<":"<<eventid_<<"      fail 3rd lepton veto"<<endl;
    return 1;
  }
   
   return 0;
 }


 void TauEleFlatNtp::endJob(){
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

 DEFINE_FWK_MODULE(TauEleFlatNtp);


