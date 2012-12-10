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
  TH1F* steps = new TH1F("steps","selection", 10, 0,10);
  // four leptons  

  TH1F* steps_4l_cat1 = new TH1F("steps_4l_cat1","selection_4l_1", 10, 0,10);
  TH1F* steps_4l_cat2 = new TH1F("steps_4l_cat2","selection_4l_2", 10, 0,10);
  TH1F* steps_4l_cat3 = new TH1F("steps_4l_cat3","selection_4l_3", 10, 0,10);
  TH1F* steps_4l_cat4 = new TH1F("steps_4l_cat4","selection_4l_4", 10, 0,10);
  TH1F* steps_4l_cat5 = new TH1F("steps_4l_cat5","selection_4l_5", 10, 0,10);
  TH1F* steps_4l_cat6 = new TH1F("steps_4l_cat6","selection_4l_6", 10, 0,10);
  // three leptons
  TH1F* steps_3l_cat1 = new TH1F("steps_3l_cat1","selection_3l_1", 10, 0,10);
  TH1F* steps_3l_cat2 = new TH1F("steps_3l_cat2","selection_3l_2", 10, 0,10);
  TH1F* steps_3l_cat3 = new TH1F("steps_3l_cat3","selection_3l_3", 10, 0,10);
  TH1F* steps_3l_cat4 = new TH1F("steps_3l_cat4","selection_3l_4", 10, 0,10);
  TH1F* steps_3l_cat5 = new TH1F("steps_3l_cat5","selection_3l_5", 10, 0,10);
  TH1F* steps_3l_cat6 = new TH1F("steps_3l_cat6","selection_3l_6", 10, 0,10);
  
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

    if (nLepGood>=3) nevt_3LepGood+=weight;

    // find passing candidates
    steps->Fill(0.,weight);
    
    if (nBJetLoose20>=2) {
      steps->Fill(1.,weight);
      
      if(met>30 && (mZ1>(91.+5.)|| mZ1<(91.-5.))){
	steps->Fill(2.,weight);
	
	if((max(LepGood1_sip3d,max(LepGood2_sip3d,LepGood3_sip3d)))<4.&& (max(LepGood1_relIso,max(LepGood2_relIso,LepGood3_relIso)))<0.2){
	  steps->Fill(3.,weight);

	  if (nBJetMedium20>=2) {
	    steps->Fill(4.,weight);
	  }
	}
      }
    }
  }
  steps->Write();
}

