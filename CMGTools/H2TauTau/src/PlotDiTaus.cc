#include "CMGTools/H2TauTau/interface/PlotDiTaus.h"

#include <memory>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
using namespace std;


#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <string.h>

PlotDiTaus::PlotDiTaus(TString inputdir):
  inputdir_(inputdir){

  //check if a file exists with the root files to be analyzed
  // Attempt to get the file attributes
  struct stat stFileInfo;
  if(stat((const char*)(inputdir_+"/summary.txt"),&stFileInfo) != 0) {
    cout<<"did not find summary.txt in "<<inputdir_<<endl;
  }
  //now read in the file names
  ifstream ntuplefiles;
  ntuplefiles.open((const char*)(inputdir_+"/summary.txt"));
  if(ntuplefiles.is_open()){
    cout<<" opened "<<inputdir_+"/summary.txt"<<endl;  
    int good=0; string coll; int evt=0;
    ntuplefiles>>good>>coll>>evt;
    while(coll!="total"){
      if(good){
	cout<<"adding:"<<coll<<endl;
	filenames_.push_back(coll);
      }
      ntuplefiles>>good>>coll>>evt;
    }
  } else{cout<<"could not open "<<inputdir_+"/summary.txt"<<endl;}
  
}

void PlotDiTaus::plotDiTau(TString inputtag){
  cout<<"plot: "<<inputdir_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);
  fwlite::ChainEvent ev(filenames_);

  TH1F Hn("Hn","",5,.5,5.5); Hn.GetXaxis()->SetTitle(" # of   #tau_{had}#tau_{had}   per event ");
  TH1F Hpt("Hpt","",20,0,100); Hpt.GetXaxis()->SetTitle(" #tau_{had}#tau_{had}     p_{T}   (GeV) ");
  TH1F Heta("Heta","",40,-4,4); Heta.GetXaxis()->SetTitle("#tau_{had}     #eta  ");
  TH1F Hphi("Hphi","",32,-3.2,3.2); Hphi.GetXaxis()->SetTitle("#tau_{had}      #phi  ");
  TH1F Hcharge("Hcharge","",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle("#tau_{had}#tau_{had}     charge  ");
  TH1F Hmass("Hmass","",40,0,200); Hmass.GetXaxis()->SetTitle(" #tau_{had}#tau_{had}    visible mass    (GeV) ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    if(ievt%100==0)cout<<ievt<<" done"<<endl;
    edm::EventBase const & event = ev;

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::DiTau> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::DiTau>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      Hmass.Fill(cand->mass());

    }

    Hn.Fill(icand);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",inputdir_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=inputdir_+"/"+tag_.label()+".ps";
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
  Hmass.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}



void PlotDiTaus::plotTauE(TString inputtag){
  cout<<"plot: "<<inputdir_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);
  fwlite::ChainEvent ev(filenames_);

  TH1F Hn("Hn","",5,.5,5.5); Hn.GetXaxis()->SetTitle(" # of   #tau_{had}-e   per event ");
  TH1F Hpt("Hpt","",20,0,100); Hpt.GetXaxis()->SetTitle(" #tau_{had}-e   p_{T} (GeV) ");
  TH1F Heta("Heta","",40,-4,4); Heta.GetXaxis()->SetTitle(" #tau_{had}-e   #eta  ");
  TH1F Hphi("Hphi","",32,-3.2,3.2); Hphi.GetXaxis()->SetTitle("#tau_{had}-e   #phi  ");
  TH1F Hcharge("Hcharge","",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle("#tau_{had}-e   charge  ");
  TH1F Hmass("Hmass","",40,0,200); Hmass.GetXaxis()->SetTitle("#tau_{had}-e    visible mass   (GeV) ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    if(ievt%1000==0)cout<<ievt<<" done"<<endl;

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::TauE> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::TauE>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      Hmass.Fill(cand->mass());

    }

    Hn.Fill(icand);
   
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",inputdir_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=inputdir_+"/"+tag_.label()+".ps";
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
  Hmass.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}


void PlotDiTaus::plotTauMu(TString inputtag){
  cout<<"plot: "<<inputdir_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);
  fwlite::ChainEvent ev(filenames_);


  TH1F Hn("Hn","",5,.5,5.5); Hn.GetXaxis()->SetTitle(" # of   #tau_{had}-#mu   per event ");
  TH1F Hpt("Hpt","",20,0,100); Hpt.GetXaxis()->SetTitle("#tau_{had}-#mu   p_{T} (GeV) ");
  TH1F Heta("Heta","",40,-4,4); Heta.GetXaxis()->SetTitle("#tau_{had}-#mu    #eta  ");
  TH1F Hphi("Hphi","",32,-3.2,3.2); Hphi.GetXaxis()->SetTitle("#tau_{had}-#mu    #phi  ");
  TH1F Hcharge("Hcharge","",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle("#tau_{had}-#mu    charge  ");
  TH1F Hmass("Hmass","",40,0,200); Hmass.GetXaxis()->SetTitle(" #tau_{had}-#mu   visible mass   (GeV) ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    if(ievt%1000==0)cout<<ievt<<" done"<<endl;

    edm::EventBase const & event = ev; 

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::TauMu> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::TauMu>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      Hmass.Fill(cand->mass());

    }

    Hn.Fill(icand);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",inputdir_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=inputdir_+"/"+tag_.label()+".ps";
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
  Hmass.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}


void PlotDiTaus::plotMuE(TString inputtag){
  cout<<"plot: "<<inputdir_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);
  fwlite::ChainEvent ev(filenames_);


  TH1F Hn("Hn","",5,.5,5.5); Hn.GetXaxis()->SetTitle(" # of   #mu-e   per event ");
  TH1F Hpt("Hpt","",20,0,100); Hpt.GetXaxis()->SetTitle("#mu-e   p_{T} (GeV) ");
  TH1F Heta("Heta","",40,-4,4); Heta.GetXaxis()->SetTitle("#mu-e    #eta  ");
  TH1F Hphi("Hphi","",32,-3.2,3.2); Hphi.GetXaxis()->SetTitle("#mu-e    #phi  ");
  TH1F Hcharge("Hcharge","",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle("#mu-e    charge  ");
  TH1F Hmass("Hmass","",40,0,200); Hmass.GetXaxis()->SetTitle(" #mu-e   visible mass   (GeV) ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    if(ievt%1000==0)cout<<ievt<<" done"<<endl;

    edm::EventBase const & event = ev; 

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::MuE> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::MuE>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      Hmass.Fill(cand->mass());

    }

    Hn.Fill(icand);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",inputdir_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=inputdir_+"/"+tag_.label()+".ps";
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
  Hmass.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}

void PlotDiTaus::plotEE(TString inputtag){
  cout<<"plot: "<<inputdir_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);
  fwlite::ChainEvent ev(filenames_);


  TH1F Hn("Hn","",5,.5,5.5); Hn.GetXaxis()->SetTitle(" # of   e-e   per event ");
  TH1F Hpt("Hpt","",20,0,100); Hpt.GetXaxis()->SetTitle("e-e   p_{T} (GeV) ");
  TH1F Heta("Heta","",40,-4,4); Heta.GetXaxis()->SetTitle("e-e    #eta  ");
  TH1F Hphi("Hphi","",32,-3.2,3.2); Hphi.GetXaxis()->SetTitle("e-e    #phi  ");
  TH1F Hcharge("Hcharge","",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle("e-e    charge  ");
  TH1F Hmass("Hmass","",40,0,200); Hmass.GetXaxis()->SetTitle(" e-e   visible mass   (GeV) ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    if(ievt%1000==0)cout<<ievt<<" done"<<endl;

    edm::EventBase const & event = ev; 

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::DiElectron> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::DiElectron>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      Hmass.Fill(cand->mass());

    }

    Hn.Fill(icand);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",inputdir_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=inputdir_+"/"+tag_.label()+".ps";
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
  Hmass.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}


void PlotDiTaus::plotMuMu(TString inputtag){
  cout<<"plot: "<<inputdir_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);
  fwlite::ChainEvent ev(filenames_);


  TH1F Hn("Hn","",5,.5,5.5); Hn.GetXaxis()->SetTitle(" # of   #mu-#mu   per event ");
  TH1F Hpt("Hpt","",20,0,100); Hpt.GetXaxis()->SetTitle("#mu-#mu   p_{T} (GeV) ");
  TH1F Heta("Heta","",40,-4,4); Heta.GetXaxis()->SetTitle("#mu-#mu    #eta  ");
  TH1F Hphi("Hphi","",32,-3.2,3.2); Hphi.GetXaxis()->SetTitle("#mu-#mu    #phi  ");
  TH1F Hcharge("Hcharge","",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle("#mu-#mu    charge  ");
  TH1F Hmass("Hmass","",40,0,200); Hmass.GetXaxis()->SetTitle(" #mu-#mu   visible mass   (GeV) ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    if(ievt%1000==0)cout<<ievt<<" done"<<endl;

    edm::EventBase const & event = ev; 

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::DiMuon> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::DiMuon>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      Hmass.Fill(cand->mass());

    }

    Hn.Fill(icand);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",inputdir_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=inputdir_+"/"+tag_.label()+".ps";
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
  Hmass.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}




