#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/JetEnergyUncertaintyComputer.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

using namespace std;

//
int main(int argc, char* argv[])
{
 // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }
  
  //configure
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  //bool useFitter = runProcess.getParameter<bool>("useFitter");

  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  TString etaFileName = runProcess.getParameter<std::string>("etaResolFileName"); gSystem->ExpandPathName(etaFileName);
  JetResolution stdEtaResol(etaFileName.Data(),false);
  TString phiFileName = runProcess.getParameter<std::string>("phiResolFileName"); gSystem->ExpandPathName(phiFileName);
  JetResolution stdPhiResol(phiFileName.Data(),false);
  TString ptFileName  = runProcess.getParameter<std::string>("ptResolFileName");  gSystem->ExpandPathName(ptFileName);
  JetResolution stdPtResol(ptFileName.Data(),true);
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());
  jet::UncertaintyComputer jcomp(&stdPtResol,&stdEtaResol,&stdPhiResol,&jecUnc);

  //control Histos
  SelectionMonitor controlHistos;
  controlHistos.addHistogram( new TH1D ("qt", ";p_{T}^{#gamma} [GeV/c];Events", 100,0,250) );
  controlHistos.addHistogram( new TH1F ("metoverqt", ";type I E_{T}^{miss}/p_{T}^{#gamma};Events", 100,0,2.5) );
  controlHistos.addHistogram( new TH1F ("met", ";type-I E_{T}^{miss} [GeV];Events", 100,0,200) );  
  controlHistos.addHistogram( new TH1F ("redmet", ";red-E_{T}^{miss} [GeV];Events", 100,0,200) );  
  controlHistos.addHistogram( new TH1F ("njets", ";Jets;Events", 4,0,4) );  
  controlHistos.addHistogram( new TH1F ("dphijetmet", ";min #Delta#phi(jet,MET);Events", 100,-3.2,3.2) );  
  controlHistos.addHistogram( new TH1F ("redmetL", ";red-E_{T}^{miss,#parallel};Events", 100,-250,250) );
  controlHistos.addHistogram( new TH1F ("redmetT", ";red-E_{T}^{miss,#perp};Events", 100,-250,250) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetLvsredmetT", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
  
  //trigger categories
  Int_t photoncats[]={0,20,30,50,60,70,75,125};

  //template reweighting
  double w20[]={0.3858,5.42316,6.97017,17.5271};
  double w30[]={0.160762,3.29482,6.62396,12.3574};
  double w50[]={0.0822076,1.90228,4.71945,8.07962};
  double w60[]={0.148545,1.15286,1.63573,1.62557};
  double w70[]={0.0705784,1.33115,2.87943,7.37565};
  double w75[]={0.054427,1.23829,2.64414,4.97387};
  double w125[]={0.0126266,0.80794,1.79493,4.05077};

  const size_t nPhotonCats=sizeof(photoncats)/sizeof(Int_t);
  for(size_t icat=0; icat<nPhotonCats; icat++)
    {
      TString subcat("photon"); subcat+=photoncats[icat];
      controlHistos.initMonitorForStep(subcat);
      controlHistos.initMonitorForStep(subcat+"eq0jets");
      controlHistos.initMonitorForStep(subcat+"eq1jets");
      controlHistos.initMonitorForStep(subcat+"geq2jets");
    }

  //start the met computers
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);

  //open the file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  ZZ2l2nuSummaryHandler evSummaryHandler;
  
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }

  //check run range
  float rescaleFactor( evEnd>0 ?  float(evSummaryHandler.getEntries())/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=evSummaryHandler.getEntries();
  if(evStart > evEnd ) 
    {
      file->Close();
       return -1;
    }
  //run the analysis
  std::set<int> trigList;
  LorentzVector nullP4(0,0,0,0);
  for( int iev=evStart; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      bool isGammaEvent(phys.gammas.size()>0);
      if(!isGammaEvent && phys.leptons.size()<2) continue;

      float weight=1.0; //ev.weight;

      //event categories
      LorentzVector gamma(0,0,0,0);
      TString evcat("photon");
      int triggerThr(0);
      if(isGammaEvent)
	{
	  gamma=phys.gammas[0];
	  
	  triggerThr=(ev.cat-22)/1000;
	  trigList.insert(triggerThr);

	  bool rejectEvent(false);
	  for(size_t icat=0; icat<nPhotonCats-1; icat++)
	    {
	      if(photoncats[icat]!=triggerThr) continue;
	      if(gamma.pt()<photoncats[icat] || gamma.pt()>=photoncats[icat+1]) rejectEvent=true;
	    }
	  if(rejectEvent) 
	    {
	      //cout << "Rejecting: " << gamma.pt() << " (thr=" << triggerThr << ")" << endl;
	      continue;
	    }
	}
      else
	{
	  gamma=phys.leptons[0]+phys.leptons[1];
	  
	  //find the trigger - threshold category (assume 100% efficiency...) 
	  if(gamma.pt()>=photoncats[nPhotonCats-1]) triggerThr=photoncats[nPhotonCats-1];
	  else
	    {
	      size_t icat=0;
	      for(; icat<nPhotonCats-1; icat++)	
		if(gamma.pt()>=photoncats[icat] &&
		   gamma.pt()<photoncats[icat+1]) break;
	      triggerThr=photoncats[icat];
	    }
	}
      evcat += triggerThr;

      int jetbin(0);
      TString subcat("eq0jets");
      if(phys.jets.size()==1) { jetbin=1; subcat="eq1jets"; }
      if(phys.jets.size()>1)  { jetbin=2; subcat="geq2jets"; }
      
      //reweight to reproduce jet multiplicity
      if(isGammaEvent)
	{
	  if(phys.jets.size()>2) { jetbin=3; } 
	  if(triggerThr==20) weight *= w20[jetbin];
	  if(triggerThr==30) weight *= w30[jetbin];
	  if(triggerThr==50) weight *= w50[jetbin];
	  if(triggerThr==60) weight *= w60[jetbin];
	  if(triggerThr==70) weight *= w70[jetbin];
	  if(triggerThr==75) weight *= w75[jetbin];
	  if(triggerThr==125) weight *= w125[jetbin];
	}
      

      //jet/met kinematics and systematic variations
      LorentzVector met=phys.met[0];
      LorentzVectorCollection jetsp4;
      double mindphijetmet(1000.);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) 
	{
	  double dphi=deltaPhi(phys.jets[ijet].phi(),met.phi());
	  if(fabs(dphi)<fabs(mindphijetmet)) mindphijetmet=dphi;
	  jetsp4.push_back( phys.jets[ijet] );
	}
      /*
	jcomp.compute(jetsp4,phys.met[0]);
	LorentzVector metJER=jcomp.getVariedMet(jet::UncertaintyComputer::JER);
	LorentzVectorCollection jetsJER = jcomp.getVariedJets(jet::UncertaintyComputer::JER);
	LorentzVector metJESdown=jcomp.getVariedMet(jet::UncertaintyComputer::JES_DOWN);
	LorentzVectorCollection jetsJESdown=jcomp.getVariedJets(jet::UncertaintyComputer::JES_DOWN);
	LorentzVector metJESup=jcomp.getVariedMet(jet::UncertaintyComputer::JES_UP);
	LorentzVectorCollection jetsJESup=jcomp.getVariedJets(jet::UncertaintyComputer::JES_UP);
      */

      //redmet
      rmetComp.compute(gamma,0,nullP4,0, jetsp4, met, true );
      double redmet = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      double redmetL = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      double redmetT = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      //       int rMetCateg = rmetComp.getEventCategory();

      //systematic variations
      /*
	rmetComp.compute(phys.leptons[0],phys.leptons[0].info[0],phys.leptons[1], phys.leptons[1].info[0], jetsJER, metJER );
	double minmetJER = rmetComp.reducedMET(ReducedMETComputer::MINIMIZED);
	double redmetJER = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
	double projMetJER = pmetComp.compute(phys.leptons[0], phys.leptons[1], metJER );
	
	rmetComp.compute(phys.leptons[0],phys.leptons[0].info[0],phys.leptons[1], phys.leptons[1].info[0], jetsJESup, metJESup );
	double minmetJESup = rmetComp.reducedMET(ReducedMETComputer::MINIMIZED);
	double redmetJESup = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
	double projMetJESup = pmetComp.compute(phys.leptons[0], phys.leptons[1], metJESup );
	
	rmetComp.compute(phys.leptons[0],phys.leptons[0].info[0],phys.leptons[1], phys.leptons[1].info[0], jetsJESdown, metJESdown );
	double minmetJESdown = rmetComp.reducedMET(ReducedMETComputer::MINIMIZED);
	double redmetJESdown = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
	double projMetJESdown = pmetComp.compute(phys.leptons[0], phys.leptons[1], metJESdown );
      */

      //fill control histograms
      TString cats[]={"all",evcat};
      TString subCats[]={"",subcat};
      //if(rMetCateg==ReducedMETComputer::COLLIMATED) topCats.push_back("cat1");
      //if(rMetCateg==ReducedMETComputer::OPENANGLE) topCats.push_back("cat2");
      //if(projMetCateg==ProjectedMETComputer::OPENANGLE) topCats.push_back("cat1");
      //if(projMetCateg==ProjectedMETComputer::COLLIMATED) topCats.push_back("cat2");
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    {
	      TString ctf=cats[ic]+subCats[isc];
		
	      if(jetsp4.size()) controlHistos.fillHisto("dphijetmet",ctf, mindphijetmet,weight);
	      controlHistos.fillHisto("njets",ctf, jetsp4.size(),weight);
	      controlHistos.fillHisto("qt",ctf, gamma.pt(),weight);
	      controlHistos.fillHisto("met", ctf, met.pt(),weight);
	      controlHistos.fillHisto("redmet", ctf, redmet,weight);
	      controlHistos.fillHisto("metoverqt", ctf,met.pt()/gamma.pt(),weight);	      
 	      controlHistos.fillHisto("redmetL", ctf,redmetL,weight);	      
	      controlHistos.fillHisto("redmetT", ctf,redmetT,weight);	      
	      controlHistos.fillHisto("redmetLvsredmetT", ctf,redmetL,redmetT,weight);	      
	    }
	}
    }
  
  
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/photon/photon_cutflow");
      if(cutflowH==0) cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH)
	cnorm=cutflowH->GetBinContent(1);
      cout << "The initial number of events was " << cnorm << endl;
      if(rescaleFactor>0) 
	{
	  cout << "Warning: rescaling by " << cnorm/rescaleFactor << " because we have run partially on this sample" << endl;
	  cnorm /= rescaleFactor;
	}
    }

  for(std::set<int>::iterator it = trigList.begin();
      it != trigList.end();
      it++)
    cout << *it << " ";
  cout << endl;

  //detach all histograms from the file
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
        {
	  hit->second->SetDirectory(0);
	}
    }

  //all done with the events file
  file->Close();
  //save to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  std::map<TString, TDirectory *> outDirs;
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      TString moncat=it->first;
      moncat.ReplaceAll("eq0jets","");
      moncat.ReplaceAll("eq1jets","");
      moncat.ReplaceAll("geq2jets","");
      if(outDirs.find(moncat)==outDirs.end()) outDirs[moncat]=baseOutDir->mkdir(moncat);
      outDirs[moncat]->cd();
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
        {
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH1") 
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TH2") 
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") ) 
	    continue;
	  
	  if(isMC && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") && cnorm>0)
	    hit->second->Scale(1./cnorm);
	  
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH2") && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") )
	    fixExtremities(hit->second,true,true);
	  
	  hit->second->Write();
	}
    }
  ofile->Close();
}  
