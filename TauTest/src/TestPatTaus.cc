#include "CMGTools/TauTest/interface/TestPatTaus.h"

#include <memory>
#include <iostream>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
using namespace std;

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/FWLite/interface/Event.h"
//#include "DataFormats/Common/interface/Ref.h"

#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>


void TestPatTaus::test(){
  cout<<filename_<<endl;
       
  edm::InputTag taus_("selectedPatTausAK5");

  TFile File(filename_,"read");
  fwlite::Event ev(&File);

  TH1F HnTau("HnTau","number of taus",11,-.5,10.5);

  TH1F Hpt("Hpt","pt",100,0,80);
  TH1F Heta("Heta","eta",80,-4,4);
  TH1F Hphi("Hphi","phi",60,-3.2,3.2);
  TH1F HTauIDPass("HTtauIDPass","tauID passed",11,0,11);
  TH1F HTauIDFail("HTtauIDFail","tauID fail",11,0,11);


  TH1F HLeadPFCandPt("HLeadPFCandPt","LeadPFCand pt",100,0,50);
  TH1F HLeadChargedHadrPt("HLeadChargedHadr","LeadPFChargedHadrCand pt",100,0,50);
  TH1F HLeadNeutralCandPt("HLeadNeutralCandPt"," HLeadNeutralCand pt",100,0,50);
  TH1F HNChargedHadrCands("HNChargedHadrCands","Number of charged hadrons",5,0,5);
  TH1F HNGammaCands("HNGammaCands","Number of gammas",5,0,5);


  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    //cout<<"event "<<ievt<<":   ";
   
    //// Handle to the particle collection
    edm::Handle<std::vector<pat::Tau> > tauvec;
    event.getByLabel(taus_, tauvec);

    //// loop over particle collection
    Int_t itau=0;
    for(std::vector<pat::Tau>::const_iterator tau=tauvec->begin(); tau!=tauvec->end(); ++tau, itau++){
      //cout<<tau->pt()<<" "<<tau->tauID("decayModeFinding")<<endl;
      Hpt.Fill(tau->pt());
      Heta.Fill(tau->eta());
      Hphi.Fill(tau->phi());

      //cout<<" "<<itau<<" ";



      reco::PFCandidateRef leadPFCand=tau->leadPFCand();
      if(leadPFCand.isNonnull()){
	HLeadPFCandPt.Fill(leadPFCand->pt());
      }
      reco::PFCandidateRef leadChargedHadrCand=tau->leadPFChargedHadrCand();//lead track
      if(leadChargedHadrCand.isNonnull()){
	HLeadChargedHadrPt.Fill(leadChargedHadrCand->pt());
      }
      reco::PFCandidateRef leadNeutralCand=tau->leadPFNeutralCand();
      if(leadNeutralCand.isNonnull()){
	HLeadNeutralCandPt.Fill(leadNeutralCand->pt());
      }
      
      reco::PFCandidateRefVector signalChargedHadrCands=tau->signalPFChargedHadrCands();
      HNChargedHadrCands.Fill(signalChargedHadrCands.size());

      reco::PFCandidateRefVector signalGammaCands=tau->signalPFGammaCands();
      HNGammaCands.Fill(signalGammaCands.size());

      

      if(tau->tauID("againstElectronLoose")==1.0)  HTauIDPass.AddBinContent(1); else HTauIDFail.AddBinContent(1);
      if(tau->tauID("againstElectronMedium")==1.0) HTauIDPass.AddBinContent(2); else HTauIDFail.AddBinContent(2);
      if(tau->tauID("againstElectronTight")==1.0)  HTauIDPass.AddBinContent(3); else HTauIDFail.AddBinContent(3);
      if(tau->tauID("againstMuonLoose")==1.0)      HTauIDPass.AddBinContent(4); else HTauIDFail.AddBinContent(4);
      if(tau->tauID("againstMuonTight")==1.0)      HTauIDPass.AddBinContent(5); else HTauIDFail.AddBinContent(5);
      if(tau->tauID("byLooseIsolation")==1.0)      HTauIDPass.AddBinContent(6); else HTauIDFail.AddBinContent(6);
      if(tau->tauID("byMediumIsolation")==1.0)     HTauIDPass.AddBinContent(7); else HTauIDFail.AddBinContent(7);
      if(tau->tauID("byTightIsolation")==1.0)      HTauIDPass.AddBinContent(8); else HTauIDFail.AddBinContent(8);
      if(tau->tauID("byVLooseIsolation")==1.0)     HTauIDPass.AddBinContent(9); else HTauIDFail.AddBinContent(9);
      if(tau->tauID("decayModeFinding")==1.0)      HTauIDPass.AddBinContent(10); else HTauIDFail.AddBinContent(10);


    }

    HnTau.Fill(itau);


    //cout<<endl;
  }


  TCanvas Canv;
  TString psfile="TauID.ps";
  Canv.Print(psfile+"[");
  
  Canv.Clear();
  HnTau.Draw("hist");
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
  HLeadPFCandPt.Draw("hist");
  Canv.Print(psfile);
  
  Canv.Clear();
  HLeadChargedHadrPt.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HLeadNeutralCandPt.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HNChargedHadrCands.Draw("hist");
  Canv.Print(psfile);

  Canv.Clear();
  HNGammaCands.Draw("hist");
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



// 	cout<<" ";
// 	reco::PFCandidate::const_iterator leadPFCand=leadPFVec->begin();
// 	//for(reco::PFCandidate::const_iterator leadPFCand=leadPFVec->begin(); leadPFCand != leadPFVec->end() ;++leadPFCand){
// 	//	for(std::vector< reco::PFCandidate >::const_iterator leadPFCand=leadPFVec->begin(); leadPFCand != leadPFVec->end() ;++leadPFCand){
// 	//	for(reco::PFCandidateIterator leadPFCand=leadPFVec->begin(); leadPFCand != leadPFVec->end() ;++leadPFCand){
// 	for(Int_t i=0;i<10;i++){
// 	  if((reco::candidate *)leadPFCand){

// 	    cout<<" i="<<i<<":"<<leadPFCand->pt()<<" ";
// 	    HLeadPFCandPt.Fill(leadPFCand->pt());
// 	  }
// 	  leadPFCand++;
// 	}
// 	cout<<endl;
// 	//return;



//#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
//#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/EDAnalyzer.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/ServiceRegistry/interface/Service.h"
//#include "FWCore/Framework/interface/MakerMacros.h"
//#include "CommonTools/UtilAlgos/interface/TFileService.h"

   // load framework libraries
  //  gSystem->Load("libFWCoreFWLite.so");
  //  gSystem->Load("libDataFormatsFWLite");
  //  gSystem->Load("libDataFormatsPatCandidates");
  //  AutoLibraryLoader::enable();

  //edm::Wrapper<vector<pat::Tau> > * tauvec =0 ;
  //vector<pat::Tau>* tauvec;
  //pat::Tau tau;
  //for(Int_t t=0;t<10;t++)tauvec.push_back(tau);
  //cout<<"tauvec.size="<<tauvec.size()<<endl;
  //pat::Tau tauvec[10];
  //pat::TauVec tauvec; 
  //Int_t tauvec;
  //Float_t pt[15];



//   TTree* Events=(TTree*)File.Get("Events");
//   if(!Events){
//     cout<<"No Events found"<<endl;
//     return;
//   }
  

  //Events->SetBranchAddress("patTaus_selectedPatTausAK5__PAT.obj",tauvec);
  //Events->SetBranchAddress("patTaus_selectedPatTausAK5__PAT.obj.pt_",pt);
  ///Events->SetBranchStatus("*",1);
  //Events->SetBranchStatus("patTaus_selectedPatTausAK5__PAT.obj",1);



    //  Int_t nevents= Events->GetEntriesFast();
    //for(Int_t e=1; e<10; e++){
    //Int_t nb=Events->GetEntry(e);

    //if(e%100==1) cout<<"Event "<<e<<" tauvec.size= "<<tauvec.size()<<endl;
    
    //cout<<tauvec[0].pt()<<endl;
    //cout<<e<<" "<<nb<<" "<<tauvec<<" "<<pt[0]<<endl;
    
    ////loop over the taus
    //for(int t=1;t<(int)tauvec.size();t++){
    //tau = tauvec[t];
    
    //cout<<"tau.tauID decayModeFinding = "<< tau.tauID("decayModeFinding")<<endl;
    //}

    //tauvec.clear();

  // virtual void set(const pat::TauPtr& input, cmg::Tau* const output, 
//   //  
//   output->leadHadrHCalEnergy    = input->leadPFChargedHadrCand()->hcalEnergy();
//   output->leadHadrECalEnergy    = input->leadPFChargedHadrCand()->ecalEnergy();
//   output->leadChargedHadrTrkPt  = input->leadPFChargedHadrCand()->pt();//  output->leadChargedHadrTrkPt = input->leadPFChargedHadrCand()->trackRef()->pt();
//   output->leadChargedHadrMvaEPi = input->leadPFChargedHadrCand()->mva_e_pi();
//   output->leadCandHCalEnergy    = input->leadPFCand()->hcalEnergy();
//   output->leadCandMvaEPi        = input->leadPFCand()->mva_e_pi();
//   output->numberChargedHadr     = input->signalPFChargedHadrCands().size();
//   output->numberGamma           = input->signalPFGammaCands().size() ;


//Default tauID for patTaus in PatAlgos (use shrinkingCone)
//  output->tauIDleadingTrackFinding  = input->tauID("leadingTrackFinding");
//  output->tauIDleadingTrackPtCut  = input->tauID("leadingTrackPtCut");
//  output->tauIDleadingPionPtCut  = input->tauID("leadingPionPtCut");
//  output->tauIDtrackIsolation  = input->tauID("trackIsolation");
//  output->tauIDtrackIsolationUsingLeadingPion  = input->tauID("trackIsolationUsingLeadingPion");
//  output->tauIDecalIsolation  = input->tauID("ecalIsolation");
//  output->tauIDecalIsolationUsingLeadingPion  = input->tauID("ecalIsolationUsingLeadingPion");
//  output->tauIDbyIsolation  = input->tauID("byIsolation");
//  output->tauIDbyIsolationUsingLeadingPion  = input->tauID("byIsolationUsingLeadingPion");
//  output->tauIDagainstElectron  = input->tauID("againstElectron");
//  output->tauIDagainstMuon  = input->tauID("againstMuon");
//

//  /////these can be used after switching the tauProducer to HPS
//  output->tauIDdecayModeFinding = input->tauID("decayModeFinding");
//  output->tauIDbyVLooseIsolation  = input->tauID("byVLooseIsolation");
//  output->tauIDbyLooseIsolation  = input->tauID("byLooseIsolation");
//  output->tauIDbyMediumIsolation  = input->tauID("byMediumIsolation");
//  output->tauIDbyTightIsolation  = input->tauID("byTightIsolation");
//  output->tauIDagainstElectronLoose  = input->tauID("againstElectronLoose");
//  output->tauIDagainstElectronMedium  = input->tauID("againstElectronMedium");
//  output->tauIDagainstElectronTight  = input->tauID("againstElectronTight");
//  output->tauIDagainstMuonLoose  = input->tauID("againstMuonLoose");
//  output->tauIDagainstMuonTight  = input->tauID("againstMuonTight");


//hpsTauIDSources = [
//    ("leadingTrackFinding", "DiscriminationByDecayModeFinding"),
//    ("byLooseIsolation", "DiscriminationByLooseIsolation"),
//    ("byMediumIsolation", "DiscriminationByMediumIsolation"),
//    ("byTightIsolation", "DiscriminationByTightIsolation"),
//    ("againstElectron", "DiscriminationAgainstElectron"),
//    ("againstMuon", "DiscriminationAgainstMuon")]

//   output->tauIDdecayModeFinding = input->tauID("decayModeFinding");
//   output->tauIDbyLooseIsolation  = input->tauID("byLooseIsolation");
//   output->tauIDbyMediumIsolation  = input->tauID("byMediumIsolation");
//   output->tauIDbyTightIsolation  = input->tauID("byTightIsolation");
//   output->tauIDagainstElectron  = input->tauID("againstElectron");
//   output->tauIDagainstMuon  = input->tauID("againstMuon");


  
//   //other variables
//  output->decayMode = input->decayMode();


