#include "CMGTools/TauTest/interface/TestCMGTaus.h"

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

#define NTauID 14

void TestCMGTaus::testTau(TString inputtag){
  cout<<dirname_<<" "<<inputtag<<endl;
  
  edm::InputTag tag_((const char*)inputtag);
    
  std::vector<string> files;
  if(firstfile_==0 && lastfile_==0) files.push_back((string)(dirname_));
  else 
    for(Int_t i=firstfile_;i<=lastfile_;i++){
      files.push_back((string)(dirname_+(long)i+".root"));
    }
  fwlite::ChainEvent ev(files);
  
  //TFile File(dirname_,"read");
  //fwlite::Event ev(&File);

  TH1F Hn("Hn","number of taus / event",11,-.5,10.5); Hn.GetXaxis()->SetTitle(" # of taus ");
  TH1F Hpt("Hpt","pt",100,0,100); Hpt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F Heta("Heta","eta",80,-4,4); Heta.GetXaxis()->SetTitle(" #eta  ");
  TH1F Hphi("Hphi","phi",60,-3.2,3.2); Hphi.GetXaxis()->SetTitle(" #phi  ");
  TH1F Hcharge("Hcharge","charge",5,-2.5,2.5); Hcharge.GetXaxis()->SetTitle(" charge  ");

  TH1F HLeadChargedHadrPt("HLeadChargedHadrPt","LeadPFChargedHadrCand pt",100,1,101); HLeadChargedHadrPt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HLeadChargedHadrEcalEnergy("HLeadChargedHadrEcalEnergy","LeadChargedHadrCand Ecal Energy",100,1,51); HLeadChargedHadrEcalEnergy.GetXaxis()->SetTitle(" energy (GeV) ");
  TH1F HLeadChargedHadrHcalEnergy("HLeadChargedHadrHcalEnergy","LeadChargedHadrCand Hcal Energy",100,1,51); HLeadChargedHadrHcalEnergy.GetXaxis()->SetTitle(" energy (GeV) ");
  TH1F HLeadChargedHadrCharge("HLeadChargedHadrCharge","LeadChargedHadrCand Charge",5,-2.5,2.5); HLeadChargedHadrCharge.GetXaxis()->SetTitle(" charge ");
  TH2F HVertex("HVertex","LeadChargedHadrCand vertex",100,0.2,0.3,100,0.34,0.44); HVertex.GetXaxis()->SetTitle(" x (cm) "); HVertex.GetYaxis()->SetTitle(" y (cm) ");
  TH2F HVertexDiff("HVertexDiff","TauVertex - PrimaryVtx",100,-.05,.05,100,-.05,.05); HVertexDiff.GetXaxis()->SetTitle(" x (cm) "); HVertexDiff.GetYaxis()->SetTitle(" y (cm) ");
  TH1F HVertexZDiff("HVertexZDiff"," dz ",100,-.1,.1); HVertexZDiff.GetXaxis()->SetTitle(" dz (cm)");
  TH1F HVertexRDiff("HVertexRDiff"," dxy ",100,0,.04); HVertexRDiff.GetXaxis()->SetTitle(" dxy (cm)");
  

  TH1F HLeadNeutralCandPt("HLeadNeutralCandPt"," HLeadNeutralCand pt",100,1,101); HLeadNeutralCandPt.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HLeadNeutralCandEcalEnergy("HLeadNeutralCandEcalEnergy","LeadNeutralCand Ecal Energy",100,1,101); HLeadNeutralCandEcalEnergy.GetXaxis()->SetTitle(" energy (GeV) ");

  TH1F HIsolation("HIsolation"," isolation",100,0,5); HIsolation.GetXaxis()->SetTitle(" isolation "); 
  TH1F HDecaymode("HDecaymode"," decaymode",5,-0.5,4.5); HDecaymode.GetXaxis()->SetTitle("");
  //  std::string pfTauMode[15]={"oneProng0Pi0","oneProng1Pi0","oneProng2Pi0","oneProng3Pi0","oneProng4Pi0",
  //			     "twoProng0Pi0","twoProng1Pi0","twoProng2Pi0","twoProng3Pi0","twoProng4Pi0",
  //			     "threeProng0Pi0","threeProng1Pi0","threeProng2Pi0","threeProng3Pi0","threeProng4Pi0"};
  std::string pfTauMode[5]={"oneProng0Pi0","oneProng1Pi0","oneProng2Pi0","threeProng0Pi0","other"};
  

//   TH1F HTauIDPass("HTtauIDPass","tauID passed",NTauID+1,0,NTauID+1); HTauIDPass.GetXaxis()->SetTitle(" ");
//   TH1F HTauIDFail("HTtauIDFail","tauID fail",NTauID+1,0,NTauID+1); HTauIDFail.GetXaxis()->SetTitle(" ");
//   std::string tauidname[NTauID]={  "againstElectronLoose" ,
// 			       "againstElectronMedium" ,
// 			       "againstElectronTight" ,
// 			       "againstMuonLoose"   ,
// 			       "againstMuonTight"  ,
// 			       "byLooseIsolation"  ,
// 			       "byMediumIsolation" ,
// 			       "byTightIsolation"  ,
// 			       "byVLooseIsolation"  ,
// 			       "decayModeFinding"  ,
// 			       "byVLooseCombinedIsolationDeltaBetaCorr",
// 			       "byLooseCombinedIsolationDeltaBetaCorr",
// 			       "byMediumCombinedIsolationDeltaBetaCorr",
// 			       "byTightCombinedIsolationDeltaBetaCorr"
//   };


  std::vector<string> tauidname;
  tauidname.push_back("decayModeFinding");
  tauidname.push_back("byVLooseCombinedIsolationDeltaBetaCorr");
  tauidname.push_back("byLooseCombinedIsolationDeltaBetaCorr");
  tauidname.push_back("byMediumCombinedIsolationDeltaBetaCorr");
  tauidname.push_back("byTightCombinedIsolationDeltaBetaCorr");
  tauidname.push_back("byRawIsoMVA");
  tauidname.push_back("byLooseIsoMVA");
  tauidname.push_back("byMediumIsoMVA");
  tauidname.push_back("byTightIsoMVA");
  tauidname.push_back("byIsolationMVA2raw");
  tauidname.push_back("byLooseIsolationMVA2");
  tauidname.push_back("byMediumIsolationMVA2");
  tauidname.push_back("byTightIsolationMVA2");
  tauidname.push_back("byLooseCombinedIsolationDeltaBetaCorr3Hits");
  tauidname.push_back("byMediumCombinedIsolationDeltaBetaCorr3Hits");
  tauidname.push_back("byTightCombinedIsolationDeltaBetaCorr3Hits");
  tauidname.push_back("byCombinedIsolationDeltaBetaCorrRaw3Hits");

  tauidname.push_back("againstMuonLoose");
  tauidname.push_back("againstMuonMedium");
  tauidname.push_back("againstMuonTight");
  tauidname.push_back("againstMuonLoose2");
  tauidname.push_back("againstMuonMedium2");
  tauidname.push_back("againstMuonTight2");

  tauidname.push_back("againstElectronLoose");
  tauidname.push_back("againstElectronMedium");
  tauidname.push_back("againstElectronTight");
  tauidname.push_back("againstElectronMVA");
  tauidname.push_back("againstElectronMVA2raw");
  tauidname.push_back("againstElectronVLooseMVA2");
  tauidname.push_back("againstElectronLooseMVA2");
  tauidname.push_back("againstElectronMediumMVA2");
  tauidname.push_back("againstElectronTightMVA2");
  tauidname.push_back("againstElectronMVA3raw");
  tauidname.push_back("againstElectronLooseMVA3");
  tauidname.push_back("againstElectronMediumMVA3");
  tauidname.push_back("againstElectronTightMVA3");
  tauidname.push_back("againstElectronVTightMVA3");

 
  TH1F HTauIDPass("HTtauIDPass","tauID passed",tauidname.size()+1,0,tauidname.size()+1); HTauIDPass.GetXaxis()->SetTitle(" ");
  TH1F HTauIDFail("HTtauIDFail","tauID fail",tauidname.size()+1,0,tauidname.size()+1); HTauIDFail.GetXaxis()->SetTitle(" ");


  //generator info 
  TH1F HptGen("HptGen","genJet pt",100,1,101); HptGen.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HptRes("HptRes","pt resolution",100,-30,10); HptRes.GetXaxis()->SetTitle(" p_{T} reco.  -  p_{T} gen.  (GeV) ");
  TH1F HetaGen("HetaGen","genJet eta",80,-4,4); HetaGen.GetXaxis()->SetTitle(" #eta  ");
  TH1F HetaRes("HetaRes","eta resolution",100,-0.1,0.1); HetaRes.GetXaxis()->SetTitle(" #eta reco.  -   #eta gen.  ");
  TH1F HphiGen("HphiGen","genJet phi",60,-3.2,3.2); HphiGen.GetXaxis()->SetTitle(" #phi  ");
  TH1F HphiRes("HphiRes","phi resolution",100,-0.2,0.2); HphiRes.GetXaxis()->SetTitle(" #phi reco.  -   #phi gen.  ");
  TH1F HchargeGen("HchargeGen","genJet charge",5,-2.5,2.5); HchargeGen.GetXaxis()->SetTitle(" charge  ");
  TH1F HchargeRes("HchargeRes","charge resolution",5,-2.5,2.5); HchargeRes.GetXaxis()->SetTitle(" charge  reco.  -   charge gen. ");
  TH1F HDecaymodeGen("HDecaymodeGen"," genJet decaymode",8,-0.5,7.5); HDecaymodeGen.GetXaxis()->SetTitle(" ");
  std::string genJetMode[8]={"oneProng0Pi0","oneProng1Pi0","oneProng2Pi0","oneProngOther",
			     "threeProng0Pi0","threeProng1Pi0","threeProngOther","rare"};

  TH1F HptGenTau("HptGenTau","genTau pt",100,1,101); HptGenTau.GetXaxis()->SetTitle(" p_{T} (GeV) ");
  TH1F HptTauRes("HptTauRes","pt resolution",100,-50,5); HptTauRes.GetXaxis()->SetTitle(" p_{T} reco.  -  p_{T} gen.  (GeV) ");

  //loop over events.
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    if(ievt%100==1)cout<<"event "<<ievt<<endl;

    //get the primary vertex 
    edm::Handle< std::vector<reco::Vertex> > vertices;
    event.getByLabel(edm::InputTag("offlinePrimaryVertices"),vertices);
    reco::Vertex primVtx=vertices->front();

//     //this is how to look at the tracks in the vertices
//     Int_t ivtx=1;
//     for(std::vector<reco::Vertex>::const_iterator vtx=vertices->begin(); vtx!=vertices->end(); ++vtx,ivtx++){
//       float pt2=0.0;
//       for(std::vector<reco::TrackBaseRef>::const_iterator trk=vtx->tracks_begin();trk!=vtx->tracks_end();trk++)
//       pt2 += (*trk)->pt()*(*trk)->pt();      
//       cout<<ivtx<<" "<<vtx->x()<<" "<<vtx->y()<<" "<<vtx->z()<<" "<<pt2<<endl;
//     }

   
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

      HLeadChargedHadrPt.Fill(cand->leadChargedHadrPt());
      HLeadChargedHadrCharge.Fill(cand->leadChargedHadrCharge());
      HLeadChargedHadrEcalEnergy.Fill(cand->leadChargedHadrEcalEnergy());
      HLeadChargedHadrHcalEnergy.Fill(cand->leadChargedHadrHcalEnergy());
      HVertex.Fill(cand->leadChargedHadrVertex().x(),cand->leadChargedHadrVertex().y());
      HVertexDiff.Fill(cand->leadChargedHadrVertex().x() - primVtx.x(),cand->leadChargedHadrVertex().y() - primVtx.y());
      HVertexZDiff.Fill(cand->dz());
      HVertexRDiff.Fill(cand->dxy());
      
      HLeadNeutralCandPt.Fill(cand->leadNeutralCandPt());    
      HLeadNeutralCandEcalEnergy.Fill(cand->leadNeutralCandEcalEnergy());    
    
        
      if(cand->decayMode()==0)HDecaymode.Fill(0);
      else if(cand->decayMode()==1)HDecaymode.Fill(1);
      else if(cand->decayMode()==2)HDecaymode.Fill(2);
      else if(cand->decayMode()==10)HDecaymode.Fill(3);
      else HDecaymode.Fill(4);

      for(unsigned int i=0;i<tauidname.size();i++)
	if(cand->tauID( tauidname[i] ) > 0.)  HTauIDPass.AddBinContent(i+1); 
	else HTauIDFail.AddBinContent(i+1); 

      if(cand->genJetp4().pt() > 0.0 ){
	HptGen.Fill(cand->genJetp4().pt());
	HptRes.Fill(cand->pt()-cand->genJetp4().pt());
	HetaGen.Fill(cand->genJetp4().eta());
	HetaRes.Fill(cand->eta()-cand->genJetp4().eta());
	HphiGen.Fill(cand->genJetp4().phi());
	HphiRes.Fill(cand->phi()-cand->genJetp4().phi());
	HchargeGen.Fill(cand->genJetCharge());  
	HchargeRes.Fill(cand->charge()-cand->genJetCharge());  
	for(Int_t i=0;i<8;i++)	
	  if(cand->genJetDecayMode()==genJetMode[i])HDecaymodeGen.Fill(i);
	
	if(cand->genTaup4().pt() > 0.0 ){
	  HptGenTau.Fill(cand->genTaup4().pt());
	  HptTauRes.Fill(cand->pt()-cand->genTaup4().pt());
	}
      }

    }

 
      


    Hn.Fill(icand);





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
  HVertexDiff.Draw("colz");
  Canv.Print(psfile);
  Canv.Clear();
  HVertexZDiff.Draw();
  Canv.Print(psfile);
  Canv.Clear();
  HVertexRDiff.Draw();
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

  TText text;
  text.SetTextSize(.03);
  text.SetTextAngle(-45.);

  Canv.Clear();
  HDecaymode.GetXaxis()->SetNdivisions(0);
  HDecaymode.Draw("hist");
  for(Int_t i=0;i<5;i++)
    text.DrawText(i,0.0-0.02*HDecaymode.GetMaximum(),TString(pfTauMode[i]));
  Canv.Print(psfile);


  Canv.Clear();
  HTauIDPass.SetStats(0);
  //HTauIDPass.GetXaxis()->SetNdivisions(0);
  HTauIDPass.SetLineColor(1);
  HTauIDPass.SetFillStyle(1);
  HTauIDPass.Draw("hist bar");
  text.SetTextAngle(-25.);
//   for(Int_t i=0;i<NTauID;i++)
//     text.DrawText(i+0.4,0.0-0.02*HTauIDPass.GetMaximum(),TString(tauidname[i]));
  Canv.Print(psfile);
  Canv.Clear();
  HTauIDFail.SetStats(0);
  //HTauIDFail.GetXaxis()->SetNdivisions(0);
  HTauIDFail.SetLineColor(1);
  HTauIDFail.SetFillStyle(1);
  HTauIDFail.Draw("hist bar");  
//   for(Int_t i=0;i<NTauID;i++)
//     text.DrawText(i+0.4,0.0-0.02*HTauIDFail.GetMaximum(),TString(tauidname[i]));
  Canv.Print(psfile);
  text.SetTextAngle(-45.);
  cout<<"Tau ID's indexes :"<<endl;
  for(unsigned int i=0;i<tauidname.size();i++)
    cout<<"Bin "<<i+1<<" ID Name : "<<tauidname[i]<<endl;  



  Canv.Clear();
  HptGen.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HptRes.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HetaGen.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HetaRes.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HphiGen.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HphiRes.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HchargeGen.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HchargeRes.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HDecaymodeGen.GetXaxis()->SetTitle("");
  HDecaymodeGen.GetXaxis()->SetNdivisions(0);
  HDecaymodeGen.Draw("hist");
  for(Int_t i=0;i<8;i++)
    text.DrawText(i,0.0-0.02*HDecaymodeGen.GetMaximum(),TString(genJetMode[i]));
  Canv.Print(psfile);
  Canv.Clear();
  HptGenTau.Draw("hist");
  Canv.Print(psfile);
  Canv.Clear();
  HptTauRes.Draw("hist");
  Canv.Print(psfile);


  Canv.Print(psfile+"]");


}

