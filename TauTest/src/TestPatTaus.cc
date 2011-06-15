#include "CMGTools/TauTest/interface/TestPatTaus.h"

#include <memory>
#include <iostream>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
using namespace std;

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
//#include "DataFormats/PatCandidates/interface/BaseMET.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/FWLite/interface/Event.h"
//#include "DataFormats/Common/interface/Ref.h"

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TDatime.h>


void TestPatTaus::testTaus(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of taus",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of taus ");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{t} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");
  TH1F HIsolation("HIsolation"," isolation",100,0,30); HIsolation.GetXaxis()->SetTitle(" particleIso ");


  TH1F HNChargedHadrCands("HNChargedHadrCands","Number of charged hadrons",5,-0.5,4.5); HNChargedHadrCands.GetXaxis()->SetTitle(" # of charged hadrons / tau ");
  TH1F HLeadChargedHadrPt("HLeadChargedHadrPt","LeadPFChargedHadrCand pt",100,0,50); HLeadChargedHadrPt.GetXaxis()->SetTitle(" p_{t} (GeV) ");
  TH1F HLeadChargedHadrEcal("HLeadChargedHadrEcal","LeadPFChargedHadrCand ECal energy",100,1,51); HLeadChargedHadrEcal.GetXaxis()->SetTitle(" energy (GeV) ");
  TH1F HLeadChargedHadrHcal("HLeadChargedHadrHcal","LeadPFChargedHadrCand HCal energy",100,1,51); HLeadChargedHadrHcal.GetXaxis()->SetTitle(" energy (GeV) ");
  TH2F HVertex("HVertex","LeadPFChargedHadrCand vertex",100,0.2,0.3,100,0.34,0.44); HVertex.GetXaxis()->SetTitle(" x (cm) "); HVertex.GetYaxis()->SetTitle(" y (cm) ");
  TH1F HLeadChargedHadrMVAEPi("HLeadChargedHadrMVAEPi","LeadPFChargedHadrCand mva_e_pi",120,-0.1,1.1); HLeadChargedHadrMVAEPi.GetXaxis()->SetTitle(" mva_e_pi ");
  TH1F HLeadChargedHadrCharge("HLeadChargedHadrCharge","LeadPFChargedHadrCand charge",5,-2.5,2.5); HLeadChargedHadrCharge.GetXaxis()->SetTitle(" charge ");

  TH1F HNGammaCands("HNGammaCands","Number of gammas",5,-0.5,4.5); HNGammaCands.GetXaxis()->SetTitle(" # of gammas / tau ");
  TH1F HLeadNeutralCandPt("HLeadNeutralCandPt"," HLeadNeutralCand pt",100,0,50); HLeadNeutralCandPt.GetXaxis()->SetTitle(" p_{t} (GeV) ");
  TH1F HLeadNeutralCandEcal("HLeadNeutralCandEcal"," HLeadNeutralCand ECal energy",100,1,51); HLeadNeutralCandEcal.GetXaxis()->SetTitle(" energy (GeV) ");

  TH1F HTauIDPass("HTtauIDPass","tauID passed",11,0,11); HTauIDPass.GetXaxis()->SetTitle(" tauID  ");
  TH1F HTauIDFail("HTtauIDFail","tauID fail",11,0,11); HTauIDFail.GetXaxis()->SetTitle(" tauID ");


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle<std::vector<pat::Tau> > particlevec;
    event.getByLabel(tag_, particlevec);

    //// loop over particle collection
    Int_t iparticle=0;
    for(std::vector<pat::Tau>::const_iterator particle=particlevec->begin(); particle!=particlevec->end(); ++particle, iparticle++){
      //cout<<particle->pt()<<" "<<particle->tauID("decayModeFinding")<<endl;
      Hpt.Fill(particle->pt());
      Heta.Fill(particle->eta());
      Hphi.Fill(particle->phi());
      Hcharge.Fill(particle->charge());
      HIsolation.Fill(particle->particleIso());
      
      //cout<<" "<<iparticle<<" ";

      /////Particle tracks
      reco::PFCandidateRefVector signalChargedHadrCands=particle->signalPFChargedHadrCands();
      HNChargedHadrCands.Fill(signalChargedHadrCands.size());

      reco::PFCandidateRef leadChargedHadrCand=particle->leadPFChargedHadrCand();//lead track
      if(leadChargedHadrCand.isNonnull()){
	HLeadChargedHadrPt.Fill(leadChargedHadrCand->pt());
	HLeadChargedHadrEcal.Fill(leadChargedHadrCand->ecalEnergy());
	HLeadChargedHadrHcal.Fill(leadChargedHadrCand->hcalEnergy());
	HLeadChargedHadrMVAEPi.Fill(leadChargedHadrCand->mva_e_pi());
	HLeadChargedHadrCharge.Fill(leadChargedHadrCand->charge());

	reco::TrackRef chHadrtrk=leadChargedHadrCand->trackRef();
	if(chHadrtrk.isAvailable()){
	  reco::TrackBase::Point vtx=chHadrtrk->referencePoint();
	  HVertex.Fill(vtx.x(),vtx.y());
 	}

      }

      ///Tau neutrals
      reco::PFCandidateRefVector signalGammaCands=particle->signalPFGammaCands();
      HNGammaCands.Fill(signalGammaCands.size());

      reco::PFCandidateRef leadNeutralCand=particle->leadPFNeutralCand();
      if(leadNeutralCand.isNonnull()){
	HLeadNeutralCandPt.Fill(leadNeutralCand->pt());
	HLeadNeutralCandEcal.Fill(leadNeutralCand->ecalEnergy());
      }
      

      

      if(particle->tauID("againstElectronLoose")==1.0)  HTauIDPass.AddBinContent(1); else HTauIDFail.AddBinContent(1);
      if(particle->tauID("againstElectronMedium")==1.0) HTauIDPass.AddBinContent(2); else HTauIDFail.AddBinContent(2);
      if(particle->tauID("againstElectronTight")==1.0)  HTauIDPass.AddBinContent(3); else HTauIDFail.AddBinContent(3);
      if(particle->tauID("againstMuonLoose")==1.0)      HTauIDPass.AddBinContent(4); else HTauIDFail.AddBinContent(4);
      if(particle->tauID("againstMuonTight")==1.0)      HTauIDPass.AddBinContent(5); else HTauIDFail.AddBinContent(5);
      if(particle->tauID("byLooseIsolation")==1.0)      HTauIDPass.AddBinContent(6); else HTauIDFail.AddBinContent(6);
      if(particle->tauID("byMediumIsolation")==1.0)     HTauIDPass.AddBinContent(7); else HTauIDFail.AddBinContent(7);
      if(particle->tauID("byTightIsolation")==1.0)      HTauIDPass.AddBinContent(8); else HTauIDFail.AddBinContent(8);
      if(particle->tauID("byVLooseIsolation")==1.0)     HTauIDPass.AddBinContent(9); else HTauIDFail.AddBinContent(9);
      if(particle->tauID("decayModeFinding")==1.0)      HTauIDPass.AddBinContent(10); else HTauIDFail.AddBinContent(10);



    }

    Hn.Fill(iparticle);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();

  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=TString(tag_.label())+".ps";
  Canv.Print(psfile+"[");
  
  //general
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


  //tracks
  Canv.Clear();
  HNChargedHadrCands.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrPt.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrEcal.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrHcal.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrMVAEPi.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadChargedHadrCharge.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HVertex.SetStats(1);
  HVertex.Draw("colz");
  Canv.Print(psfile);


  //neutrals
  Canv.Clear();
  HNGammaCands.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadNeutralCandPt.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HLeadNeutralCandEcal.Draw("hist");
  Canv.Print(psfile);


  //tauID
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



void TestPatTaus::testMuons(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of muons",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of muons / event");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{t} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");
  TH1F HIsolation("HIsolation"," isolation",100,0,30); HIsolation.GetXaxis()->SetTitle(" trackIso ");
  TH2F HVertex("HVertex","vertex",100,0.2,0.3,100,0.34,0.44); HVertex.GetXaxis()->SetTitle(" x (cm) "); HVertex.GetYaxis()->SetTitle(" y (cm) ");



  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle<std::vector<pat::Muon> > particlevec;
    event.getByLabel(tag_, particlevec);

    //// loop over particle collection
    Int_t iparticle=0;
    for(std::vector<pat::Muon>::const_iterator particle=particlevec->begin(); particle!=particlevec->end(); ++particle, iparticle++){
      //cout<<particle->pt()<<" "<<particle->tauID("decayModeFinding")<<endl;
      Hpt.Fill(particle->pt());
      Heta.Fill(particle->eta());
      Hphi.Fill(particle->phi());
      Hcharge.Fill(particle->charge());
      HIsolation.Fill(particle->trackIso());
     
      
      reco::TrackRef trk=particle->globalTrack();
      if(trk.isAvailable()){
	reco::TrackBase::Point vtx=trk->referencePoint();
	HVertex.Fill(vtx.x(),vtx.y());
      }

 
    }

    Hn.Fill(iparticle);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();

  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=TString(tag_.label())+".ps";
  Canv.Print(psfile+"[");
  
  //general
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
  HVertex.SetStats(1);
  HVertex.Draw("colz");
  Canv.Print(psfile);


 
  Canv.Print(psfile+"]");


}


void TestPatTaus::testElectrons(TString inputtag){
  cout<<filename_<<" "<<inputtag<<endl;
       
  edm::InputTag tag_((const char*)inputtag);

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F Hn("Hn","number of muons",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of electrons / event");
  TH1F Hpt("Hpt","pt",100,0,80); Hpt.GetXaxis()->SetTitle(" p_{t} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");
  TH1F HIsolation("HIsolation","isolation",100,0,30); HIsolation.GetXaxis()->SetTitle(" trackIso ");
  TH2F HVertex("HVertex","vertex",100,0.2,0.3,100,0.34,0.44); HVertex.GetXaxis()->SetTitle(" x (cm) "); HVertex.GetYaxis()->SetTitle(" y (cm) ");



  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle<std::vector<pat::Electron> > particlevec;
    event.getByLabel(tag_, particlevec);

    //// loop over particle collection
    Int_t iparticle=0;
    for(std::vector<pat::Electron>::const_iterator particle=particlevec->begin(); particle!=particlevec->end(); ++particle, iparticle++){
      //cout<<particle->pt()<<" "<<particle->tauID("decayModeFinding")<<endl;
      Hpt.Fill(particle->pt());
      Heta.Fill(particle->eta());
      Hphi.Fill(particle->phi());
      Hcharge.Fill(particle->charge());
      HIsolation.Fill(particle->trackIso());
     
      reco::GsfTrackRef trk=particle->gsfTrack();
      if(trk.isAvailable()){
	reco::TrackBase::Point vtx=trk->referencePoint();
	HVertex.Fill(vtx.x(),vtx.y());
      }
      
      
 
    }

    Hn.Fill(iparticle);


    //cout<<endl;
  }


  TDatime date;
  TString datelabel=TString("")+(long)date.GetDate()+"_"+(long)date.GetTime();

  TCanvas Canv("Canv",filename_+"_"+tag_.label()+"_"+datelabel);
  TString psfile=TString(tag_.label())+".ps";
  Canv.Print(psfile+"[");
  
  //general
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
  HVertex.SetStats(1);
  HVertex.Draw("colz");
  Canv.Print(psfile);


 
  Canv.Print(psfile+"]");


}


