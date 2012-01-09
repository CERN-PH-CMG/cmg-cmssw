#include "CMGTools/TauTest/interface/TestTrig.h"

#include <memory>
#include <iostream>
#include <vector>
#include <string.h>
#include <TROOT.h>
#include <TSystem.h>
using namespace std;

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"   
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"


#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TText.h>
#include <TString.h>



void TestTrig::testMuTauTrig(TString inputtag){
  cout<<dirname_<<" "<<inputtag<<endl;
  
  edm::InputTag tag_((const char*)inputtag);
    
  std::vector<string> files;
  for(Int_t i=firstfile_;i<=lastfile_;i++){
    files.push_back((string)(dirname_+(long)i+".root"));
  }
  fwlite::ChainEvent ev(files);


  //trigger matching
  TH1F HmassClean("HmassClean","",100,0,200); HmassClean.GetXaxis()->SetTitle(" mass ");
  TH1F HmassCleanTrigMatch("HmassCleanTrigMatch","",100,0,200); HmassCleanTrigMatch.GetXaxis()->SetTitle(" mass ");

  TH1F HmuptClean("HmuptClean","pt",50,0,100); HmuptClean.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HmuptCleanTrigMatch("HmuptCleanTrigMatch","pt",50,0,100); HmuptCleanTrigMatch.GetXaxis()->SetTitle(" p_{T} (GeV) ");

  TH1F HtauptClean("HtauptClean","pt",50,0,100); HtauptClean.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HtauptCleanTrigMatch("HtauptCleanTrigMatch","pt",50,0,100); HtauptCleanTrigMatch.GetXaxis()->SetTitle(" p_{T} (GeV) ");

  
  //HLTBeginSequence + hltL1sL1SingleMu10 + hltPreIsoMu15LooseIsoPFTau15 + hltL1SingleMu10L1Filtered0 + HLTL2muonrecoSequence + hltL2Mu10L2Filtered10 + HLTL2muonisorecoSequence + hltSingleMuIsoL2IsoFiltered10 + HLTL3muonrecoSequence + hltSingleMuIsoL3PreFiltered15 + HLTL3muonisorecoSequence + hltSingleMuIsoL3IsoFiltered15 + HLTRecoJetSequencePrePF + hltTauJet5 + HLTPFJetTriggerSequenceForTaus + HLTPFTauSequence + hltPFTau15 + hltPFTau15Track + hltPFTau15TrackLooseIso + hltOverlapFilterIsoMu15IsoPFTau15 + HLTEndSequence
  TString hltpath="HLT_IsoMu15_LooseIsoPFTau15_v8";//Aug
  TString hltfiltermu="hltSingleMuIsoL3IsoFiltered15";//hltL1sL1SingleMu10  hltL1SingleMu10L1Filtered0  hltL2Mu10L2Filtered10  hltSingleMuIsoL2IsoFiltered10 hltSingleMuIsoL3PreFiltered15  hltSingleMuIsoL3IsoFiltered15
  TString hltfiltertau="hltPFTau15TrackLooseIso";//hltTauJet5ß  hltPFTau15 hltPFTau15TrackLooseIso  hltOverlapFilterIsoMu15IsoPFTau15


  //TString hltpath="HLT_IsoMu12_LooseIsoPFTau10_v2";//SummerMC
  //TString hltpath="HLT_IsoMu12_v1";//SummerMC
  //TString hltpath="HLT_IsoMu15_LooseIsoPFTau15_v9";//Fall11MC
  //TString hltpath="HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v1";//Fall11MC
  //TString hltpath="HLT_IsoMu15_eta2p1_v1";

  float drcut=0.3;


  //loop over events.
  Int_t ievt=0;
  int nevttrig=0;
  int nevtmutrigfilt=0;
  int nevttautrigfilt=0;
  int nevtmutautrigfilt=0;
  int nevtOthermutautrigfilt=0;
  int nevtmuOthertautrigfilt=0;
  int nevtmutautrigfiltsimult=0;
  for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt){
    edm::EventBase const & event = ev;
    if(ievt%100==0)cout<<"event "<<ievt<<endl;

    edm::Handle< std::vector<cmg::TriggerObject> > trig;
    event.getByLabel(edm::InputTag("cmgTriggerObjectSel","","PAT"),trig);
    edm::Handle< std::vector<cmg::TriggerObject> > trigObjs;
    event.getByLabel(edm::InputTag("cmgTriggerObjectListSel","","PAT"),trigObjs);


//     cout<<"Trig  event "<<ievt<<" nobjs:"<<trig->size()<<endl;
//     trig->begin()->printSelections(cout);
//     return;
    
    if(!(trig->begin()->hasSelection(hltpath)
	 &&trig->begin()->getSelection(hltpath))
       ) continue;
    nevttrig++;

    //count the number of events with at least one particle passin the filters
    bool mutrigpass=0; bool othermutrigpass=0; bool tautrigpass=0; bool othertautrigpass=0; bool mutautrigpass=0;
    for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs->begin(); obj!=trigObjs->end(); obj++){
      if(obj->hasSelection(hltpath)){

	if(obj->hasSelection(hltfiltermu)) mutrigpass=1;
	else  if(obj->hasSelection(hltfiltertau)) othertautrigpass=1;

	if(obj->hasSelection(hltfiltertau)) tautrigpass=1;
	else  if(obj->hasSelection(hltfiltermu)) othermutrigpass=1;
	
	if(obj->hasSelection(hltfiltermu)&&obj->hasSelection(hltfiltertau)) mutautrigpass=1;

      }
    }
    if(mutrigpass)nevtmutrigfilt++;
    if(tautrigpass)nevttautrigfilt++;
    if(mutrigpass&&tautrigpass)nevtmutautrigfilt++;
    if(othermutrigpass&&tautrigpass)nevtOthermutautrigfilt++;
    if(mutrigpass&&othertautrigpass)nevtmuOthertautrigfilt++;
    if(mutautrigpass)nevtmutautrigfiltsimult++;

  
    edm::Handle< std::vector<cmg::Muon> > mulist;
    event.getByLabel(edm::InputTag("cmgMuonSel"),mulist);
    edm::Handle< std::vector<cmg::Tau> > taulist;
    event.getByLabel(edm::InputTag("cmgTauSel"),taulist);

    std::vector<cmg::Muon> mulistclean;
    std::vector<cmg::Tau> taulistclean;
    std::vector<cmg::Muon> mulistcleanmatch;
    std::vector<cmg::Tau> taulistcleanmatch;

    //create a list of selected muons
    for(std::vector<cmg::Muon >::const_iterator muon=mulist->begin(); muon!=mulist->end(); ++muon){
      if(1
	 && muon->isGlobal()>0.5
	 && muon->isTracker()>0.5
	 && muon->numberOfValidTrackerHits()>10
	 && muon->numberOfValidPixelHits()>=1
	 && muon->numberOfValidMuonHits()>=1
	 && muon->numberOfMatches()>=2
	 && muon->normalizedChi2()<10
	 && muon->pt()>15.0
 	 && fabs(muon->eta())<2.1
 	 && muon->relIso(0.5)<0.1	 
	 ){
	mulistclean.push_back(*muon);

	bool mumatched=0; 
	for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs->begin(); obj!=trigObjs->end(); obj++)
	  if(1
	     &&obj->hasSelection(hltpath)
	     &&obj->hasSelection(hltfiltermu)
	     &&reco::deltaR(muon->eta(),muon->phi(),obj->eta(),obj->phi())<drcut
	     )mumatched=1;
	
	if(mumatched) mulistcleanmatch.push_back(*muon);
      }
    }

    //create list of selected taus
    for(std::vector<cmg::Tau>::const_iterator tau=taulist->begin(); tau!=taulist->end(); ++tau){	  	
      if(1
	 &&tau->pt()>20.0
	 &&fabs(tau->eta())<2.3
	 &&tau->tauID("byLooseCombinedIsolationDeltaBetaCorr")>0.5
	 ){
	taulistclean.push_back(*tau);
      
	bool taumatched=0; 
	for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs->begin(); obj!=trigObjs->end(); obj++)
	  if(1
	     &&obj->hasSelection(hltpath)
	     &&obj->hasSelection(hltfiltertau)
	     &&reco::deltaR(tau->eta(),tau->phi(),obj->eta(),obj->phi())<drcut
	     )taumatched=1;
	if(taumatched)taulistcleanmatch.push_back(*tau);
      }
    }
    
    // mu-tau candidates before trigger object matching
    for(std::vector<cmg::Muon>::const_iterator muon=mulistclean.begin(); muon!=mulistclean.end(); ++muon)
      for(std::vector<cmg::Tau>::const_iterator tau=taulistclean.begin(); tau!=taulistclean.end(); ++tau)
	if(reco::deltaR(tau->eta(),tau->phi(),muon->eta(),muon->phi())>0.3){
	  HmassClean.Fill((muon->p4()+tau->p4()).mass());
	  HmuptClean.Fill(muon->pt());
	  HtauptClean.Fill(tau->pt());
	}


    //mu-tau candidates after trigger object matching
    for(std::vector<cmg::Muon>::const_iterator muon=mulistcleanmatch.begin(); muon!=mulistcleanmatch.end(); ++muon)
      for(std::vector<cmg::Tau>::const_iterator tau=taulistcleanmatch.begin(); tau!=taulistcleanmatch.end(); ++tau)
	if(reco::deltaR(tau->eta(),tau->phi(),muon->eta(),muon->phi())>0.3){
	  HmassCleanTrigMatch.Fill((muon->p4()+tau->p4()).mass());
	  HmuptCleanTrigMatch.Fill(muon->pt());
	  HtauptCleanTrigMatch.Fill(tau->pt());
	}
    
  }
  

  //print a summary of the counters
  cout<<ievt<<" Nevt processed"<<endl;
  cout<<nevttrig<<" pass trig path"<<endl;
  cout<<nevtmutrigfilt<<" pass mu trig filter"<<endl;
  cout<<nevttautrigfilt<<" pass tau trig filter"<<endl;
  cout<<nevtmutautrigfilt<<" pass mu-tau trig filters"<<endl;
  cout<<nevtmutautrigfiltsimult<<" pass mu-tau trig filters simultaneously"<<endl;
  cout<<nevtOthermutautrigfilt<<" pass Othermu-tau trig filters"<<endl;
  cout<<nevtmuOthertautrigfilt<<" pass mu-Othertau trig filters"<<endl;

  cout<<"Yield "<<HmassClean.Integral(0,HmassClean.GetNbinsX()+1)<<endl;
  cout<<"Yield "<<HmassCleanTrigMatch.Integral(0,HmassCleanTrigMatch.GetNbinsX()+1)<<endl;


  ///////save some plots
  TCanvas Canv("Canv",dirname_+"_"+tag_.label());
  TString psfile=TString(tag_.label())+"_Trig.ps";
  Canv.Print(psfile+"[");


  ///Trigger matching
  Canv.Clear();
  HmassClean.Draw("hist");
  HmassCleanTrigMatch.Draw("pesame");
  Canv.Print(psfile);

  Canv.Clear();
  HmuptClean.Draw("hist");
  HmuptCleanTrigMatch.Draw("pesame");
  Canv.Print(psfile);

  Canv.Clear();
  HtauptClean.Draw("hist");
  HtauptCleanTrigMatch.Draw("pesame");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");



}



