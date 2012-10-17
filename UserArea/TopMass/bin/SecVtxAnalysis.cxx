#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <TH2F.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
//#include "/DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "PhysicsTools/FWLite/interface/EventContainer.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "RecoJets/JetAnalyzers/interface/myFastSimVal.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
// #include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "UserCode/SecVtxAnalysis/interface/SecVtxAnalysisTree.h"
#include <stdio.h>
#include <algorithm>

using namespace std;

//auxiliary function to list files from a directory
std::vector<string> getInputFilesFrom(string dir);
//
int main(int argc, char* argv[]) 
{
  //define the objects you're using 
  using pat::Muon;
  using pat::Jet;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // initialize command line parser
  optutl::CommandLineParser parser ("[SecVtxAnalysis]");

  // parse arguments (first set defaults)
  parser.integerValue ("maxEvents"  ) = -1;
  parser.integerValue ("outputEvery") =  100;
  parser.addOption    ("inputDir",   optutl::CommandLineParser::kString, "input directory");
  parser.parseArguments (argc, argv);
  int maxEvents                  = parser.integerValue("maxEvents");
  unsigned int outputEvery       = parser.integerValue("outputEvery");
  std::string inputDir           = parser.stringValue("inputDir");
  std::vector<string> inputFiles = getInputFilesFrom(inputDir);
  std::string outputFile         = gSystem->BaseName(inputDir.c_str())+string(".root");

  // book histograms (...use your imagination)
  fwlite::TFileService fs = fwlite::TFileService(outputFile.c_str());
  TFileDirectory dir = fs.mkdir("analysis");
  TH1F* muonPtH             = dir.make<TH1F>("muonPt"  ,      ";p_{T} [GeV];Muons",            60,   0., 300.);
  TH1F* muonRelPFIsoH       = dir.make<TH1F>("relpfiso",      ";Rel PF Iso;Muons",               100,   0., 0.2);
  TH1F* muonRelPFIsoDBetaH  = dir.make<TH1F>("relpfisodbeta", ";Rel PF Iso (#Delta#beta);Muons", 100,   0., 0.2);
  TH1F* muonChIsoH          = dir.make<TH1F>("chiso",         ";Charged isolation;Muons",        100,   0., 5);
  std::map<TString,TProfile *> muonPtProf;
  muonPtProf["relpfiso"]      = dir.make<TProfile>("relpfisoprof",      ";Vertices;<p_{T}> [GeV]",  30,0,30,50,60);
  muonPtProf["relpfisodbeta"] = dir.make<TProfile>("relpfisodbetaprof", ";Vertices;<p_{T}> [GeV]",  30,0,30,50,60);
  muonPtProf["chiso"]         = dir.make<TProfile>("chisoprof",         ";Vertices;<p_{T}> [GeV]",  30,0,30,50,60);
  muonPtProf["chisotight"]    = dir.make<TProfile>("chisotightprof",     ";Vertices;<p_{T}> [GeV]", 30,0,30,50,60);

  //add a tree with a summary of the selected events
  SecVtxAnalysisEvent_t evSummary;
  TTree *tree=fs.make<TTree>("data","Event Summary");
  initSecVtxAnalysisTree(tree,evSummary);

  // loop the events
  int ievt=0;  
  bool stopAnalysis(false);
  std::map<string,int> procFiles;
  for(unsigned int iFile=0; iFile<inputFiles.size(); ++iFile)
    {
      // open input file (can be located on castor)
      TFile* inFile = TFile::Open(inputFiles[iFile].c_str());
      if( inFile ){
	
	int nprocEvts(0);
	fwlite::Event ev(inFile);
	for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt,++nprocEvts){
	  
	  // break loop if maximal number of events is reached 
	  if(maxEvents>0 ? ievt+1>maxEvents : false) { stopAnalysis=true; break; }
	  
	  // simple event counter
	  if(outputEvery!=0 ? (ievt>0 && ievt%outputEvery==0) : false) printf("\r Processing event: %d",ievt);
	  
	  //now read the event
	  edm::EventBase const & event = ev;
	  evSummary.run=event.id().run(); evSummary.lumi=event.luminosityBlock(); evSummary.event=event.id().event();
	  
	  //pileup information: get number of pileup interactions generated in the same bx crossing
	  fwlite::Handle<std::vector<PileupSummaryInfo> > puInfoH;
	  puInfoH.getByLabel(ev,"addPileupInfo");
	  int ngenITpu(0);
	  if(puInfoH.isValid()) {
	    for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++) {
	      if(it->getBunchCrossing()==0) ngenITpu += it->getPU_NumInteractions(); 
	    }
	  }
	  evSummary.ngenITpu=ngenITpu;

	  //primary vtx
          edm::Handle<std::vector<reco::Vertex> > h_primVtx;
          event.getByLabel(std::string("goodOfflinePrimaryVertices"),h_primVtx);
          const reco::Vertex pv(h_primVtx->at(0));
	  evSummary.nvtx=h_primVtx->size();
	  
	  //analyse the muons
	  edm::Handle<std::vector<Muon> > muons;
	  event.getByLabel(std::string("selectedPatMuonsPFTriggerMatch"), muons);
          
	  int nGoodMuons(0);
	  const pat::Muon *muCandidate=0;
	  for(std::vector<Muon>::const_iterator mu=muons->begin(); mu!=muons->end(); ++mu){
	    bool isGood( mu->pt()>26 && fabs(mu->eta())<2.1 && mu->isPFMuon() && mu->isGlobalMuon());
	    if(!isGood) continue;
	    bool isPrompt( fabs(mu->dB())<0.2 && fabs(mu->vertex().z()-pv.z())<0.5 );
	    if(!isPrompt) continue;
	    bool hasGoodTrack(mu->normChi2()<10 
			      && mu->numberOfMatchedStations()>1 
			      && mu->innerTrack()->hitPattern().numberOfValidPixelHits()>0 
			      && mu->globalTrack()->hitPattern().trackerLayersWithMeasurement()>5
			      && mu->globalTrack()->hitPattern().numberOfValidMuonHits()>0);
	    if(!hasGoodTrack) continue;
	    nGoodMuons++;
	    if(muCandidate==0)                   muCandidate = &(*mu);
	    else if(muCandidate->pt()<mu->pt())  muCandidate = &(*mu);
	  }
	  if(nGoodMuons>1) continue;

	  evSummary.l_pt     = muCandidate->pt(); 
	  evSummary.l_eta    = muCandidate->eta(); 
	  evSummary.l_phi    = muCandidate->phi(); 
	  evSummary.l_en     = muCandidate->energy();
	  evSummary.l_spt    = fabs((muCandidate->innerTrack()->pt())/(muCandidate->innerTrack()->ptError()));
	  float chIso        = muCandidate->chargedHadronIso();
	  float chpuIso      = muCandidate->puChargedHadronIso();
	  float gIso         = muCandidate->photonIso();
	  float nhIso        = muCandidate->neutralHadronIso();
	  float relIso       = (chIso+gIso+nhIso)/muCandidate->pt(); 
	  float relIsoDbeta  = (chIso+max(gIso+nhIso-0.5*chpuIso,0.))/muCandidate->pt();
	  evSummary.l_relIso = relIso;
	  
	  //analyse the MET
          edm::Handle<std::vector<pat::MET> > mets;
          event.getByLabel(std::string("patMETsAK5PF"), mets);
          evSummary.met_pt  = mets->empty() ? 0 : (*mets)[0].et();
	  evSummary.met_phi = mets->empty() ? 0 : (*mets)[0].phi();

	  //analyse the jets
	  edm::Handle<std::vector<Jet> > jets;
	  event.getByLabel(std::string("selectedPatJetsAK5PF"), jets);
	  evSummary.jn=0;
	  int njets45(0), njets35(0), nbtags;
	  for(std::vector<Jet>::const_iterator jet=jets->begin(); jet!=jets->end(); ++jet)
	    {
	      bool isGood( fabs(jet->eta())<2.5 && jet->pt()>20 );
	      if(!isGood) continue;
	      
	      //b-tagging info
	      float csv=jet->bDiscriminator("combinedSecondaryVertexBJetTags");
	      int nSecVtx(0);
	      float lxy,slxy;
	      reco::SecondaryVertexTagInfo const *svTagInfos = jet->tagInfoSecondaryVertex("secondaryVertex");
	      if( svTagInfos != 0 )
		{
		  nSecVtx=svTagInfos->nVertices();
		  if(nSecVtx>0)
		    {
		      lxy=svTagInfos->flightDistance(0).value();
		      slxy=svTagInfos->flightDistance(0).error();
		    }
		}

	      evSummary.j_pt[evSummary.jn]   = jet->pt();
	      evSummary.j_eta[evSummary.jn]  = jet->eta();
	      evSummary.j_phi[evSummary.jn]  = jet->phi();
	      evSummary.j_en[evSummary.jn]   = jet->energy();
	      evSummary.j_lxy[evSummary.jn]  = lxy;
	      evSummary.j_slxy[evSummary.jn] = slxy;
	      evSummary.jn++;
	      njets45 += (jet->pt()>45);
	      njets35 += (jet->pt()>35);
	      nbtags  += (csv>0.679);

	    }
	  if(evSummary.jn<4) continue;
	  if(njets45<2)      continue;
	  if(njets35<3)      continue; 
	  if(nbtags<1)       continue;
          
	  //save selected event
	  tree->Fill();
	  
	  //fill histos
	  float weight=1.0;
	  muonPtH            ->Fill(muCandidate->pt(),weight);
	  muonRelPFIsoH      ->Fill(relIso,weight);
	  muonRelPFIsoDBetaH ->Fill(relIsoDbeta,weight);
	  muonChIsoH         ->Fill(chIso,weight);
	  if(relIso<0.12)      muonPtProf["relpfiso"]     ->Fill(evSummary.nvtx,muCandidate->pt(),weight);
	  if(relIsoDbeta<0.15) muonPtProf["relpfisodbeta"]->Fill(evSummary.nvtx,muCandidate->pt(),weight);
	  if(chIso<3)          muonPtProf["chiso"]        ->Fill(evSummary.nvtx,muCandidate->pt(),weight);
	  if(chIso<1.5)        muonPtProf["chisotight"]   ->Fill(evSummary.nvtx,muCandidate->pt(),weight);
	}
        
	// close input file
	inFile->Close();
	procFiles[inputFiles[iFile]]=nprocEvts;
      }
      
      if(stopAnalysis) break;
    }


  cout << "-----------------------------------------------------------" << endl
       << "[SecVtxAnalysis] data summary" << endl; 
  int totalEvtsProc(0);
  for(std::map<string,int>::iterator it= procFiles.begin(); it != procFiles.end(); it++)
    {
      totalEvtsProc+=it->second;
      cout << it->second << " events processed from " << it->first << endl;
    }
  cout << "-----------------------------------------------------------" << endl
       << "processed a total of " << totalEvtsProc << " from " << procFiles.size() << " files" << endl;

  return 0;
}

//
std::vector<string> getInputFilesFrom(string dir)
{
  std::vector<string> allFiles;

  string baseDir("root://eoscms//eos/cms/"); baseDir+=dir;

  //single file mode
  if(baseDir.find(".root")!=string::npos) { 
    allFiles.push_back(baseDir);
  }

  //browse directory mode
  else {
      void *dirp=gSystem->OpenDirectory(baseDir.c_str());
      const char* afile; 
      while ((afile = gSystem->GetDirEntry(dirp))) allFiles.push_back(baseDir+"/"+string(afile));
      gSystem->FreeDirectory(dirp); 
    }

  cout << "[getInputFiles] " << allFiles.size() << " file(s) is available @ " << dir << endl;
  return allFiles;
}
