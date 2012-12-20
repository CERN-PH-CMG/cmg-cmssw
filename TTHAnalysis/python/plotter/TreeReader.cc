#include "TreeReader.h"
#include <algorithm>

#include <TH1F.h>

#include <iostream>
using namespace std;


void TreeReader::Loop(TString mode) {
  if (fChain == 0) return;

  // BOOK HISTOS

  TH1F::SetDefaultSumw2(kTRUE);

  // inclusive
  // TH1F* steps = new TH1F("steps","selection", 10, 0,10);
  // four leptons  
  TH1F* steps_4l = new TH1F("steps_4l","selection_4l", 10, 0,10);
  TH1F* jets_4l = new TH1F("jets_4l","jets_4l", 10, 0,10);
  // three leptons
  TH1F* steps_3l_for3j = new TH1F("steps_3l_for3j","selection_3l_for3j", 10, 0,10);
  TH1F* steps_3l_for4j5j = new TH1F("steps_3l_for4j5j","selection_3l_for4j5j", 10, 0,10);
  TH1F* jets_3l_for3j = new TH1F("jets_3l_for3j","jets_3l_for3j", 10, 0,10);
  TH1F* jets_3l_for4j5j = new TH1F("jets_3l_for4j5j","jets_3l_for4j5j", 10, 0,10);
  // two leptons
  TH1F* steps_2l_mm= new TH1F("steps_2l_mm","selection_2l_mm", 10, 0,10);
  TH1F* jets_2l_mm = new TH1F("jets_2l_mm","jets_2l_mm", 10, 0,10);
  TH1F* steps_2l_ee= new TH1F("steps_2l_ee","selection_2l_ee", 10, 0,10);
  TH1F* jets_2l_ee = new TH1F("jets_2l_ee","jets_2l_ee", 10, 0,10);
  TH1F* steps_2l_em= new TH1F("steps_2l_em","selection_2l_em", 10, 0,10);
  TH1F* jets_2l_em = new TH1F("jets_2l_em","jets_2l_em", 10, 0,10);

  // loop over events
  const Long64_t nentries = fChain->GetEntriesFast();
  double nevt_3LepGood = 0.;
  for (Long64_t jentry=0;jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEntry(jentry);

    if (mode!="") {
      if      (mode == "tautau" && GenHiggsDecayMode!=15) continue;
      else if (mode == "ZZ"     && GenHiggsDecayMode!=23) continue;
      else if (mode == "WW"     && GenHiggsDecayMode!=24) continue;
      else if (mode == "XX"     && (GenHiggsDecayMode==15||GenHiggsDecayMode==23||GenHiggsDecayMode==24)) continue;
    }

    //4lep
    if(nLepGood>=4){
      steps_4l->Fill(0.,weight);
      if(min(min(LepGood1_mva,LepGood2_mva),LepGood3_mva) >= -0.2){
 	steps_4l->Fill(1.,weight);
	if(fabs(mZ1 - 91.2)> 10.){
	  steps_4l->Fill(2.,weight);
	  if( nBJetLoose25 >= 1){
	    steps_4l->Fill(3.,weight);
	    if( nBJetLoose25 >= 2 || nBJetMedium25 >= 1){
	      steps_4l->Fill(4.,weight);
	      jets_4l->Fill(nJet25,weight);
	    }
	  }
	}
      }
    }

    //3lep for 3j
    if(nLepGood==3){
      steps_3l_for3j->Fill(0.,weight);
      if(min(LepGood1_mva,min(LepGood2_mva,LepGood3_mva)) >= -0.2){
	steps_3l_for3j->Fill(1.,weight);
	if(fabs(mZ1-91.2) > 10.){
	  steps_3l_for3j->Fill(2.,weight);
	  if(met*0.00397 + mhtJet25*0.00265 - 0.184 > 0.0 + 0.1*(mZ1 > 0)){
	    steps_3l_for3j->Fill(3.,weight);
	    if(nBJetLoose25 >= 1){
	      steps_3l_for3j->Fill(4.,weight);
	      if(nBJetLoose25 >= 2){
		steps_3l_for3j->Fill(5.,weight);
		jets_3l_for3j->Fill(nJet25,weight);
	      }
	    }
	  }
	}
      }
    }

    //3lep for 4j5j
    if(nLepGood==3){
      steps_3l_for4j5j->Fill(0.,weight);
      if(min(LepGood1_mva,min(LepGood2_mva,LepGood3_mva)) >= -0.2){
	steps_3l_for4j5j->Fill(1.,weight);
	if(fabs(mZ1-91.2) > 10.){
	  steps_3l_for4j5j->Fill(2.,weight);
	  if(nBJetLoose25 >= 1){
	    steps_3l_for4j5j->Fill(3.,weight);
	    if(nBJetLoose25 >= 2){
	      steps_3l_for4j5j->Fill(4.,weight);
	      jets_3l_for4j5j->Fill(nJet25,weight);
	    }
	  }
	}
      }
    }

    //2lep SS mm
    if(nLepGood==2 || LepGood3_mva <-0.2 ){
      steps_2l_mm->Fill(0.,weight);
      if(min(LepGood1_mva,LepGood2_mva) >= -0.2){
	steps_2l_mm->Fill(1.,weight);
	if(abs(LepGood1_pdgId)==13 && abs(LepGood2_pdgId)==13){
	  steps_2l_mm->Fill(2.,weight);
	  if(LepGood1_charge*LepGood2_charge>0){
	    steps_2l_mm->Fill(3.,weight);
	    if(LepGood1_tightCharge && LepGood2_tightCharge){
	      steps_2l_mm->Fill(4.,weight);
	      if(nBJetLoose25 >= 1){
		steps_2l_mm->Fill(5.,weight);
		if(nBJetLoose25 >= 2){
		  steps_2l_mm->Fill(6.,weight);
		  jets_2l_mm->Fill(nJet25,weight);
		}
	      }
	    }
	  }
	}
      }
    }

    //2lep SS ee
    if(nLepGood==2 || LepGood3_mva <-0.2 ){
      steps_2l_ee->Fill(0.,weight);
      if(min(LepGood1_mva,LepGood2_mva) >= -0.2){
	steps_2l_ee->Fill(1.,weight);
	if(abs(LepGood1_pdgId)==11 && abs(LepGood2_pdgId)==11){
	  steps_2l_ee->Fill(2.,weight);
	  if(LepGood1_charge*LepGood2_charge>0){
	    steps_2l_ee->Fill(3.,weight);
	    if(LepGood1_tightCharge && LepGood2_tightCharge){
	      steps_2l_ee->Fill(4.,weight);
	      if(LepGood1_innerHits == 0 && LepGood2_innerHits == 0){
		steps_2l_ee->Fill(5.,weight);
		if(fabs(minMllAFAS-91.2) > 10.){
		  steps_2l_ee->Fill(6.,weight);
		  if(met*0.00397 + mhtJet25*0.00265 - 0.184 > 0.0){
		    steps_2l_ee->Fill(7.,weight);
		    if(nBJetLoose25 >= 1){
		      steps_2l_ee->Fill(8.,weight);
		      if(nBJetLoose25 >= 2){
			steps_2l_ee->Fill(9.,weight);
			jets_2l_ee->Fill(nJet25,weight);
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }

     //2lep SS em
    if(nLepGood==2 || LepGood3_mva <-0.2 ){
      steps_2l_em->Fill(0.,weight);
      if(min(LepGood1_mva,LepGood2_mva) >= -0.2){
	steps_2l_em->Fill(1.,weight);
	if(abs(LepGood1_pdgId) != abs(LepGood2_pdgId)){
	  steps_2l_em->Fill(2.,weight);
	  if(LepGood1_charge*LepGood2_charge>0){
	    steps_2l_em->Fill(3.,weight);
	    if(LepGood1_tightCharge && LepGood2_tightCharge){
	      steps_2l_em->Fill(4.,weight);
	      if(abs(LepGood1_pdgId) == 11 && LepGood1_innerHits == 0 ||  abs(LepGood2_pdgId) == 11 && LepGood2_innerHits == 0){
		steps_2l_em->Fill(5.,weight);
		if(nBJetLoose25 >= 1){
		  steps_2l_em->Fill(6.,weight);
		  if(nBJetLoose25 >= 2){
		    steps_2l_em->Fill(7.,weight);
		    jets_2l_em->Fill(nJet25,weight);
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    

  }


 steps_4l->Write();
 jets_4l->Scale(1/jets_4l->GetEntries());
 jets_4l->Write(); 
 steps_3l_for3j->Write(); 
 steps_3l_for4j5j->Write();
 jets_3l_for3j->Scale(1/jets_3l_for3j->GetEntries());
 jets_3l_for3j->Write();
 jets_3l_for4j5j->Write();
 steps_2l_mm->Write();
 jets_2l_mm->Scale(1/jets_2l_mm->GetEntries());
 jets_2l_mm->Write(); 
 steps_2l_ee->Write();
 jets_2l_ee->Scale(1/jets_2l_ee->GetEntries());
 jets_2l_ee->Write(); 
 steps_2l_em->Write();
 jets_2l_em->Scale(1/jets_2l_em->GetEntries());
 jets_2l_em->Write(); 

}

