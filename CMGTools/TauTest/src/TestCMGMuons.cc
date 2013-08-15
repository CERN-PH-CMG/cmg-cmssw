#include "CMGTools/TauTest/interface/TestCMGMuons.h"

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


void TestCMGMuons::testMuon(TString inputtag){
  cout<<dirname_<<" "<<inputtag<<endl;

  edm::InputTag tag_((const char*)inputtag);


  std::vector<string> files;
  for(Int_t i=firstfile_;i<=lastfile_;i++){
    files.push_back((string)(dirname_+(long)i+".root"));
  }
  fwlite::ChainEvent ev(files);

  //  TFile File(dirname_,"read");
  //  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of muons / event",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of muons ");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");
  TH1F HIsolation("HIsolation"," isolation",100,0,0.3); HIsolation.GetXaxis()->SetTitle(" isolation "); HIsolation.GetXaxis()->SetTitle(" relIso ");
  TH1F HGlobal("HGlobal","Global/Tracker muon",5,-.5,4.5); HGlobal.GetXaxis()->SetTitle("");
  TH1F HPixelHits("HPixelHits","number of PixelHits",14,0,14); HPixelHits.GetXaxis()->SetTitle("number of pixel hits");
  TH1F HTrackerHits("HTrackerHits","number of TrackerHits",100,0,100); HTrackerHits.GetXaxis()->SetTitle("number of tracker hits");
  TH1F HChi2("HChi2","Muon chi2/ndf",100,0,20); HChi2.GetXaxis()->SetTitle("#chi^{2} per degree of freedom");
  TH1F Hdxy("Hdxy","vertex delta x-y",100,0,.06); Hdxy.GetXaxis()->SetTitle("x-y distance (cm)");
  TH1F Hdz("Hdz","vertex delta z",100,-.4,.4); Hdz.GetXaxis()->SetTitle("z distance (cm)");
  
  

  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle< std::vector<cmg::Muon> > candvec;
    event.getByLabel(tag_, candvec);

    //// loop over particle collection
    Int_t i=0;
    for(std::vector<cmg::Muon>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, i++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      HIsolation.Fill(cand->relIso());

      if(cand->isTracker()!=1 || cand->isGlobal()!=1)HGlobal.Fill(0);      
      if(cand->isTracker()==1)HGlobal.Fill(1);
      if(cand->isGlobal()==1)HGlobal.Fill(2);
      if(cand->isTracker()==1 && cand->isGlobal()==1)HGlobal.Fill(3);      
      

      HPixelHits.Fill(cand->numberOfValidPixelHits());
      HTrackerHits.Fill(cand->numberOfValidTrackerHits());
      HChi2.Fill(cand->normalizedChi2());
      Hdxy.Fill(cand->dxy());
      Hdz.Fill(cand->dz());
      //      Int_t numberOfValidMuonHits()
      //      Int_t numberOfMatches()

    }

    Hn.Fill(i);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",dirname_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=TString(tag_.label())+".ps";
  Canv.Print(psfile+"[");
  
  Canv.Clear();
  Hn.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Hpt.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Heta.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Hphi.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Hcharge.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HIsolation.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HGlobal.SetStats(0);
  HGlobal.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HPixelHits.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HTrackerHits.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HChi2.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Hdxy.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Hdz.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}

