#include "CMGTools/TauTest/interface/TestGenParticles.h"

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


void TestGenParticles::testGenParticle(TString inputtag){
  cout<<dirname_<<" "<<inputtag<<endl;

  edm::InputTag tag_((const char*)inputtag);

  std::vector<string> files;
  for(Int_t i=firstfile_;i<=lastfile_;i++){
    files.push_back((string)(dirname_+(long)i+".root"));
  }
  fwlite::ChainEvent ev(files);

  //  TFile File(dirname_,"read");
  //  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of particles / event",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of particles ");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");

  TH1F HmotherPDGId("HmotherPDGId","mother pdg id",10,0.5,10.5);
  TH1F HmotherMass("HmotherMass","mother mass ",200,0,200);
  TH1F HmotherPt("HmotherPt","mother pt ",100,0,100);


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() && ievt<1000; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle< std::vector<reco::GenParticle> > candvec;
    event.getByLabel(tag_, candvec);

    //// loop over particle collection
    Int_t i=0;
    for(std::vector<reco::GenParticle>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, i++){
      if(abs(cand->pdgId())==15){//select only the taus
	Hpt.Fill(cand->pt());
	Heta.Fill(cand->eta());
	Hphi.Fill(cand->phi());
	Hcharge.Fill(cand->charge());

	//mother
	if(cand->mother()){
	  //cout<<cand->mother()->pdgId()<<endl;
	  int motherId=cand->mother()->pdgId();
	  if(abs(motherId)==21)HmotherPDGId.Fill(1); //gluon
	  else if(abs(motherId)==22)HmotherPDGId.Fill(2); //gamma
	  else if(abs(motherId)==23)HmotherPDGId.Fill(3); //Z
	  else if(abs(motherId)==24)HmotherPDGId.Fill(4); //W
	  else if(abs(motherId)==25 || abs(motherId)==32 || abs(motherId)==33 || abs(motherId)==34 || abs(motherId)==34 || abs(motherId)==35 || abs(motherId)==36 || abs(motherId)==37 )HmotherPDGId.Fill(5); //h0,Z',Z'',W',H0,A0,H+
	  else if(abs(motherId)==411 || abs(motherId)==431 || abs(motherId)==521 ) HmotherPDGId.Fill(6) ; //D+,Ds+,B+
	  else if(abs(motherId)==553 || abs(motherId)==100523 || abs(motherId)==200553 || abs(motherId)==300553 )  HmotherPDGId.Fill(7); //Upsilons: 1 2 3 4
	  else HmotherPDGId.Fill(10); 

	  HmotherMass.Fill(cand->mother()->mass());
	  HmotherPt.Fill(cand->mother()->pt());

	  //if(cand->mother()->mother())
	  //	    cout<<cand->mother()->mother()->pdgId()<<endl;

	}
	
	//daughters: currently daughter pointers not being saved
	//cout<<cand->numberOfDaughters()<<endl;
	//if(cand->daughter(0)){
	//	  cout<<cand->daughter(0)->pdgId()<<endl;
	//	}
	
      }


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
  HmotherPDGId.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HmotherMass.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HmotherPt.Draw("hist");
  Canv.Print(psfile);

 
  Canv.Print(psfile+"]");


}


