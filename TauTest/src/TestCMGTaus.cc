#include "CMGTools/TauTest/interface/TestCMGTaus.h"

#include <memory>
#include <iostream>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
using namespace std;

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>


void TestCMGTaus::testTau(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of taus / event",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of taus ");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");

  TH1F HNChargedHadrCands("HNChargedHadrCands","Number of charged hadrons",5,0,5); HNChargedHadrCands.GetXaxis()->SetTitle(" # of charged hadrons / tau ");
  TH1F HLeadChargedHadrPt("HLeadChargedHadrPt","LeadPFChargedHadrCand pt",100,0,50); HLeadChargedHadrPt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HLeadChargedHadrEcalEnergy("HLeadChargedHadrEcalEnergy","LeadChargedHadrCand Ecal Energy",100,0,50); HLeadChargedHadrEcalEnergy.GetXaxis()->SetTitle(" energy (GeV) ");
  TH1F HLeadChargedHadrHcalEnergy("HLeadChargedHadrHcalEnergy","LeadChargedHadrCand Hcal Energy",100,0,50); HLeadChargedHadrHcalEnergy.GetXaxis()->SetTitle(" energy (GeV) ");
  TH1F HLeadChargedHadrCharge("HLeadChargedHadrCharge","LeadChargedHadrCand Charge",5,-2.5,2.5); HLeadChargedHadrCharge.GetXaxis()->SetTitle(" charge ");
  TH2F HVertex("HVertex","LeadChargedHadrCand vertex",100,0.2,0.3,100,0.34,0.44); HVertex.GetXaxis()->SetTitle(" x (cm) "); HVertex.GetYaxis()->SetTitle(" y (cm) ");

  TH1F HNGammaCands("HNGammaCands","Number of gammas",5,-0.5,4.5); HNGammaCands.GetXaxis()->SetTitle(" # of gammas / tau ");
  TH1F HLeadNeutralCandPt("HLeadNeutralCandPt"," HLeadNeutralCand pt",100,0,50); HLeadNeutralCandPt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HLeadNeutralCandEcalEnergy("HLeadNeutralCandEcalEnergy","LeadNeutralCand Ecal Energy",100,0,50); HLeadNeutralCandEcalEnergy.GetXaxis()->SetTitle(" energy (GeV) ");

  TH1F HIsolation("HIsolation"," isolation",100,0,20); HIsolation.GetXaxis()->SetTitle(" isolation "); HIsolation.GetXaxis()->SetTitle(" relIso ");
  TH1F HDecaymode("HDecaymode"," decaymode",30,-0.5,29.5); HDecaymode.GetXaxis()->SetTitle(" decaymode "); HDecaymode.GetXaxis()->SetTitle(" decay mode ");

  TH1F HTauIDPass("HTtauIDPass","tauID passed",11,-0.5,10.5); HTauIDPass.GetXaxis()->SetTitle(" tauID  ");
  TH1F HTauIDFail("HTtauIDFail","tauID fail",11,-0.5,10.5); HTauIDFail.GetXaxis()->SetTitle(" tauID  ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";

    //// loop over particle collection   
    edm::Handle< std::vector<cmg::Tau> > candvec;
    event.getByLabel(tag_, candvec);
    Int_t icand=0;
    for(std::vector<cmg::Tau>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, icand++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());

      HNChargedHadrCands.Fill(cand->numberChargedHadr());
      HLeadChargedHadrPt.Fill(cand->leadChargedHadrPt());
      HLeadChargedHadrCharge.Fill(cand->leadChargedHadrCharge());
      HLeadChargedHadrEcalEnergy.Fill(cand->leadChargedHadrECalEnergy());
      HLeadChargedHadrHcalEnergy.Fill(cand->leadChargedHadrHCalEnergy());
      HVertex.Fill(cand->leadChargedHadrVertex().x(),cand->leadChargedHadrVertex().y());
      
      HNGammaCands.Fill(cand->numberGamma());
      HLeadNeutralCandPt.Fill(cand->leadNeutralCandPt());    
      HLeadNeutralCandEcalEnergy.Fill(cand->leadNeutralCandECalEnergy());    
    
      HIsolation.Fill(cand->relIso());
      HDecaymode.Fill(cand->decayMode());

      if(cand->tauID("againstElectronLoose"))  HTauIDPass.AddBinContent(1); else HTauIDFail.AddBinContent(1);
      if(cand->tauID("againstElectronMedium")) HTauIDPass.AddBinContent(2); else HTauIDFail.AddBinContent(2);
      if(cand->tauID("againstElectronTight"))  HTauIDPass.AddBinContent(3); else HTauIDFail.AddBinContent(3);
      if(cand->tauID("againstMuonLoose"))      HTauIDPass.AddBinContent(4); else HTauIDFail.AddBinContent(4);
      if(cand->tauID("againstMuonTight"))      HTauIDPass.AddBinContent(5); else HTauIDFail.AddBinContent(5);
      if(cand->tauID("byLooseIsolation"))      HTauIDPass.AddBinContent(6); else HTauIDFail.AddBinContent(6);
      if(cand->tauID("byMediumIsolation"))     HTauIDPass.AddBinContent(7); else HTauIDFail.AddBinContent(7);
      if(cand->tauID("byTightIsolation"))      HTauIDPass.AddBinContent(8); else HTauIDFail.AddBinContent(8);
      if(cand->tauID("byVLooseIsolation"))     HTauIDPass.AddBinContent(9); else HTauIDFail.AddBinContent(9);
      if(cand->tauID("decayModeFinding"))      HTauIDPass.AddBinContent(10); else HTauIDFail.AddBinContent(10);


    }

    Hn.Fill(icand);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
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
  HNChargedHadrCands.Draw("hist");
  Canv.Print(psfile);  
  Canv.Clear();
  HLeadChargedHadrPt.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrCharge.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrEcalEnergy.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrHcalEnergy.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HVertex.Draw("colz");
  Canv.Print(psfile);
  

  Canv.Clear();
  HNGammaCands.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadNeutralCandPt.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadNeutralCandEcalEnergy.Draw("hist");
  Canv.Print(psfile);


  Canv.Clear();
  HIsolation.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HDecaymode.Draw("hist");
  Canv.Print(psfile);


  Canv.Clear();
  HTauIDPass.SetStats(0);
  HTauIDPass.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HTauIDFail.SetStats(0);
  HTauIDFail.Draw("hist");
  Canv.Print(psfile);

  Canv.Print(psfile+"]");


}


void TestCMGTaus::testElectron(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;

  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hn("Hn","number electrons / event",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of electrons ");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");
  TH1F HIsolation("HIsolation"," isolation",100,0,0.3); HIsolation.GetXaxis()->SetTitle(" isolation "); HIsolation.GetXaxis()->SetTitle(" relIso ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle< std::vector<cmg::Electron> > candvec;
    event.getByLabel(tag_, candvec);

    //// loop over particle collection
    Int_t i=0;
    for(std::vector<cmg::Electron>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, i++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Heta.Fill(cand->eta());
      Hphi.Fill(cand->phi());
      Hcharge.Fill(cand->charge());
      HIsolation.Fill(cand->relIso());


    }

    Hn.Fill(i);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
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

  

  Canv.Print(psfile+"]");


}






void TestCMGTaus::testMuon(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;

  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of muons / event",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of muons ");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");
  TH1F HIsolation("HIsolation"," isolation",100,0,0.3); HIsolation.GetXaxis()->SetTitle(" isolation "); HIsolation.GetXaxis()->SetTitle(" relIso ");

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

    }

    Hn.Fill(i);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
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

  

  Canv.Print(psfile+"]");


}




void TestCMGTaus::testMET(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;

  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hpt("Hpt","pt",100,0,80);
  TH1F Hphi("Hphi","phi",60,-3.2,3.2);

  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle< std::vector<cmg::BaseMET> > candvec;
    event.getByLabel(tag_, candvec);

    //// loop over particle collection
    Int_t i=0;
    for(std::vector<cmg::BaseMET>::const_iterator cand=candvec->begin(); cand!=candvec->end(); ++cand, i++){
      //cout<<cand->pt()<<" "<<cand->tauID("decayModeFinding")<<endl;

      Hpt.Fill(cand->pt());
      Hphi.Fill(cand->phi());


    }


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();
  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=TString(tag_.label())+".ps";
  Canv.Print(psfile+"[");
  
  Canv.Clear();
  Hpt.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  Hphi.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}





