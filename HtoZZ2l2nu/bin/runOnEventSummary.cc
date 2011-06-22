#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ProjectedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"

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

  //control Histos
  SelectionMonitor controlHistos;

  controlHistos.addHistogram( new TH1F ("met", ";type-I E_{T}^{miss};Events", 100,0,500) );  
  controlHistos.addHistogram( new TH1F ("redmet", ";red-E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("minmet", ";min-E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("projmet", ";projected E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("projminmet", ";projected min-E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram(  new TH1D ("zpt", ";p_{T}^{ll};Events", 100,0,400) );
  controlHistos.addHistogram(  new TH1D ("zeta", ";#eta^{ll};Events", 100,-5,5) );

  controlHistos.addHistogram( new TH1F ("metoverzpt", ";type I E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9) );
  controlHistos.addHistogram( new TH1F ("projmetoverzpt", ";projected E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9) );
  controlHistos.addHistogram( new TH1F ("minmetoverzpt", ";min-E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9) );

  controlHistos.addHistogram( new TH1D ("dphill", ";#Delta#phi(l^{(1)},l^{(2)});Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1D ("mindphilmet", ";min #Delta#phi(l^{(i)},E_{T}^{miss});Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1D ("dphizz", ";#Delta#phi(ll,E_{T}^{miss});Events",100,-3.2,3.2) );
  
  controlHistos.addHistogram(  new TH1D ("zpt", ";p_{T}^{ll};Events", 100,0,400) );
  controlHistos.addHistogram(  new TH1D ("zmass", ";M^{ll};Events", 100,71,111) );
  controlHistos.addHistogram(  new TH1D ("deltazpt", ";#Delta p_{T}^{ll};Events", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltazphi", ";#Delta #phi^{ll};Events", 100,-3.2,3.2) );
  controlHistos.addHistogram(  new TH1D ("deltazvvpt", ";#Delta p_{T}^{#nu#nu};Events", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltazvvphi", ";#Delta #phi^{#nu#nu};Events", 100,-3.2,3.2) );

  controlHistos.addHistogram(  new TH1D ("mtmetsum", ";#sum M_{T}(l,E_{T}^{miss});Events", 100,0,500) );
  controlHistos.addHistogram(  new TH1D ("mtminmetsum", ";#sum M_{T}(l,min-E_{T}^{miss});Events", 100,0,500) );
  
  controlHistos.addHistogram( (TH1D *)(new TH2D ("minmetcomps", ";min-E_{T}^{miss,#parallel};min-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("minmetvsminmetoverzpt", ";type I E_{T}^{miss};type I E_{T}^{miss}/p_{T}(Z);Events", 100, -50.,250,100, -1.,9) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("metvsmetoverzpt", ";E_{T}^{miss};E_{T}^{miss}/p_{T}(Z);Events", 100, -50.,250,100, -1.,9) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("projmetvsprojmetoverzpt", ";projected E_{T}^{miss};projected E_{T}^{miss}/p_{T}(Z);Events", 100, -50.,250,100, -1.,9) ) );
  
  //replicate monitor for categories
  TString cats[]={"ee","emu","mumu"};
  TString subcats[]={"","eq0jets","eq1jets","geq2jets"};
  TString topcats[]={"","cat1","cat2","cat3","cat4"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(size_t isubcat=0;isubcat<sizeof(subcats)/sizeof(TString); isubcat++)
	{
	  for(size_t itopcat=0;itopcat<sizeof(topcats)/sizeof(TString); itopcat++)
	    {
	      controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]+topcats[itopcat]);
	    }
	}
    }
  
   
  //start the met computers
  ProjectedMETComputer pmetComp;
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
  //ReducedMETFitter rmetFitter(runProcess);

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
  for( int iev=evStart; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      float weight=ev.weight;

      //event categories
      TString evcat("");
      if(ev.cat==dilepton::EMU)  evcat="emu";
      if(ev.cat==dilepton::MUMU) evcat="mumu";
      if(ev.cat==dilepton::EE)   evcat="ee";

      TString subcat("eq0jets");
      if(phys.jets.size()==1) subcat="eq1jets";
      if(phys.jets.size()>1)  subcat="geq2jets";

      //z kinematics
      double dphill=deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      LorentzVector zll=phys.leptons[0]+phys.leptons[1];
      LorentzVector zvv=phys.met[0];
      LorentzVector genzll(0,0,0,0), genzvv(0,0,0,0), higgs(0,0,0,0);
      if(phys.genleptons.size()) 
	{
	  genzll=phys.genleptons[0]+phys.genleptons[1];
	  genzvv=phys.genmet[0];
	  higgs = phys.genhiggs[0];	
	}
      TVector2 zal(zll.px(),zll.py());
      zal=zal.Unit();

      //redmet
      LorentzVectorCollection jetsp4;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) jetsp4.push_back( phys.jets[ijet] );
      rmetComp.compute(phys.leptons[0],phys.leptons[0].info[0],phys.leptons[1], phys.leptons[1].info[0], jetsp4, zvv );
      double redmet = rmetComp.reducedMET(ReducedMETComputer::D0);
      double minmet = rmetComp.reducedMET(ReducedMETComputer::MINIMIZED);
      double minmetL = rmetComp.reducedMETComponents(ReducedMETComputer::MINIMIZED).second;
      double minmetT = rmetComp.reducedMETComponents(ReducedMETComputer::MINIMIZED).first;
      int rMetCateg = rmetComp.getEventCategory();
      TVector2 minmetxy = rmetComp.reducedMETcartesian(ReducedMETComputer::MINIMIZED);
      LorentzVector minMetP4(minmetxy.Px(),minmetxy.Py(),0,minmet);
      double minmetzpt=minmetxy*zal;

      //projected met
      double projMet = pmetComp.compute(phys.leptons[0], phys.leptons[1], zvv );
      int projMetCateg = pmetComp.getEventCategory();
      TVector2 projMetxy = pmetComp.projectedMETcartesian();
      double projmetzpt=projMetxy*zal;
      double mindphilmet = pmetComp.minimumDeltaPhi();
      double projminmet = pmetComp.compute(phys.leptons[0], phys.leptons[1], minMetP4 );

      float dphil2met[]={ fabs(deltaPhi(zvv.phi(),phys.leptons[0].phi())), fabs(deltaPhi(zvv.phi(),phys.leptons[1].phi())) };
      float mTlmet[]={ TMath::Sqrt(2*zvv.pt()*phys.leptons[0].pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*zvv.pt()*phys.leptons[1].pt()*(1-TMath::Cos(dphil2met[1]))) };

      float dphil2minmet[]={ fabs(deltaPhi(minmetxy.Phi(),phys.leptons[0].phi())), fabs(deltaPhi(minmetxy.Phi(),phys.leptons[1].phi())) };
      float mTlminmet[]={ TMath::Sqrt(2*minmet*phys.leptons[0].pt()*(1-TMath::Cos(dphil2minmet[0]))) ,   TMath::Sqrt(2*minmet*phys.leptons[1].pt()*(1-TMath::Cos(dphil2minmet[1]))) };

      //fill control histograms
      TString cats[]={"all",evcat};
      TString subCats[]={"",subcat};
      std::vector<TString> topCats;
      topCats.push_back("");
      if(rMetCateg==ReducedMETComputer::COLLIMATED) topCats.push_back("cat1");
      if(rMetCateg==ReducedMETComputer::OPENANGLE) topCats.push_back("cat2");
      if(projMetCateg==ProjectedMETComputer::OPENANGLE) topCats.push_back("cat3");
      if(projMetCateg==ProjectedMETComputer::COLLIMATED) topCats.push_back("cat4");
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    {
	      for(size_t itc=0; itc<topCats.size(); itc++)
		{
		  TString ctf=cats[ic]+subCats[isc]+topCats[itc];

		  controlHistos.fillHisto("dphill",ctf, dphill,weight);
		  controlHistos.fillHisto("mindphilmet",ctf, mindphilmet,weight);
		  controlHistos.fillHisto("dphizz",ctf, deltaPhi(zll.phi(),zvv.phi()), weight);
	    
		  controlHistos.fillHisto("zpt",ctf, zll.pt(),weight);
		  controlHistos.fillHisto("zmass",ctf, zll.mass(),weight);
		  controlHistos.fillHisto("deltazpt",ctf, zll.pt()-genzll.pt(),weight);
		  controlHistos.fillHisto("deltazphi",ctf, deltaPhi(zll.phi(),genzll.phi()),weight);

		  controlHistos.fillHisto("met", ctf,zvv.pt(),weight);
		  controlHistos.fillHisto("deltazvvpt",ctf, zvv.pt()-genzvv.pt(),weight);
		  controlHistos.fillHisto("deltazvvphi",ctf, deltaPhi(zvv.phi(),genzvv.phi()),weight);
		  
		  controlHistos.fill2DHisto("zptvszeta", ctf,zll.pt(),zll.eta(),weight);
		  controlHistos.fill2DHisto("zptvsmet", ctf, zll.pt(), zvv.pt(),weight);
		  controlHistos.fill2DHisto("zetavsmet", ctf, zll.eta(), zvv.pt(),weight);

		  controlHistos.fillHisto("metoverzpt", ctf,zvv.pt()/zll.pt(),weight);	      

		  controlHistos.fillHisto("projmet", ctf,projMet,weight);	      
		  controlHistos.fillHisto("projmetoverzpt", ctf,projMet/zll.pt(),weight);	      
		  controlHistos.fillHisto("redmet", ctf,redmet,weight);	      
		  controlHistos.fillHisto("minmet", ctf,minmet,weight);	      
		  controlHistos.fillHisto("minmetoverzpt", ctf,minmet/zll.pt(),weight);	 
		  controlHistos.fillHisto("projminmet", ctf,projminmet,weight);	           
		  controlHistos.fillHisto("minmetcomps", ctf,minmetL,minmetT,weight);	
		  controlHistos.fillHisto("minmetvsminmetoverzpt", ctf,minmet,minmet/zll.pt(),weight);
		  controlHistos.fillHisto("metvsmetoverzpt", ctf,zvv.pt(),zvv.pt()/zll.pt(),weight);
		  controlHistos.fillHisto("projmetvsprojmetoverzpt", ctf,projMet,projMet/zll.pt(),weight);

		  controlHistos.fillHisto("mtmetsum", ctf,mTlmet[0]+mTlmet[1],weight);	      
		  controlHistos.fillHisto("mtminmetsum", ctf,mTlminmet[0]+mTlminmet[1],weight);	      

		}
	    }
	}
    }
  
  
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TString tag=gSystem->BaseName(url);
      tag.ReplaceAll(".root","");
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/"+tag+"/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);

      if(rescaleFactor>0) cnorm /= rescaleFactor;
    }

  //all done with the events file
  file->Close();

  //save to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  cout << outUrl << " " << cnorm << endl;
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  std::map<TString, TDirectory *> outDirs;
  outDirs["all"]=baseOutDir->mkdir("all");
  outDirs["ee"]=baseOutDir->mkdir("ee");
  outDirs["emu"]=baseOutDir->mkdir("emu");
  outDirs["mumu"]=baseOutDir->mkdir("mumu");
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      TString icat("all");
      if(it->first.BeginsWith("ee")) icat="ee";
      if(it->first.BeginsWith("emu")) icat="emu";
      if(it->first.BeginsWith("mumu")) icat="mumu";
      outDirs[icat]->cd();
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
        {
	  if(isMC && cnorm>0) hit->second->Scale(1./cnorm);
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH2")
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") )
	    fixExtremities(hit->second,true,true);
	  hit->second->Write();

        }
    }

  ofile->Close();
}  
