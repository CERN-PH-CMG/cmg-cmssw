#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/TransverseMassComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
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
#include "Math/GenVector/Boost.h"

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
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //control Histos
  SelectionMonitor controlHistos;

  //the Higgs pT
  TString hptvars[]={"","std","renup","rendown","facup","facdown"};
  Float_t hptaxis[]={0,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,15.,20.,25.,30.,35.,40.,45.,50.,
		     60.,70.,80.,100.,150,200.,250.,300.,350.,400.,500,600.,700.,800.,1000.};
  for(size_t ivar=0; ivar<sizeof(hptvars)/sizeof(TString); ivar++)
    controlHistos.addHistogram(  new TH1D ("hpt"+hptvars[ivar], ";p_{T}^{H};Events", sizeof(hptaxis)/sizeof(Float_t)-1,hptaxis) );
  
  TString tiers[]={"gen","reco"};
  for(size_t itier=0; itier<2; itier++)
    {
      controlHistos.addHistogram( new TH1D (tiers[itier]+"zpt", ";"+tiers[itier]+" p_{T}^{ll};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"zvvpt", ";"+tiers[itier]+" p_{T}^{#nu#nu};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"njets", ";"+tiers[itier]+" Jets;Jets",4,0,4) );

      if(itier)
	{
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"mt", ";"+tiers[itier]+" M_{T};Events", 100, 100.,900.) );
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"prefrecoil", ";"+tiers[itier]+" h_{T};Events", 100,0,200) );
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"mtcm", ";"+tiers[itier]+" M_{T}(pCM);Events", 100, 100.,900.) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"deltacmzz", ";p_{T}^{ll}-p_{T}^{vv}(pCM)", 100,-250,250) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzpt", ";p_{T}^{ll}(pCM)", 100,0,500) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzvvpt", ";p_{T}^{#nu#nu}(pCM)", 100,0,500) );
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"mtcm400", ";"+tiers[itier]+" M_{T}(pCM 400);Events", 100, 100.,900.) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"deltacmzz400", ";p_{T}^{ll}-p_{T}^{vv}(pCM 400)", 100,-250,250) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzpt400", ";p_{T}^{ll}(pCM 400)", 100,0,500) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzvvpt400", ";p_{T}^{#nu#nu}(pCM 400)", 100,0,500) );

	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"dilcmzvvpt", ";red E_{T}^{miss}(dilepton CM)", 100,0,250) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"dilcmdphistar", ";#Delta#phi(red E_{T}^{miss},p_{T,lab}^{Z})", 100,-3.2,3.2) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"dilcmdphistarrecoil", ";#Delta#phi(red E_{T}^{miss},h_{T,lab})", 100,-3.2,3.2) );
	  
	  controlHistos.addHistogram( new TH1F (tiers[itier]+"redmet", ";"+tiers[itier]+" red-E_{T}^{miss};Events", 100, 0.,500.) );
	  controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetL", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};Events", 100, -200.,200.) );
	  controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetT", ";"+tiers[itier]+" red-E_{T}^{miss,#perp};Events", 100, -200.,200.) );
	  controlHistos.addHistogram( (TH1D *)(new TH2D (tiers[itier]+"redmetcomps", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
	}
      else
	{
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"m", ";"+tiers[itier]+" M;Events", 100, 100.,500.) );
	}
    }
  controlHistos.addHistogram(  new TH1D ("deltaprefrecoil", ";h_{T} - p_{T}^{gen}(H)", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltaphiprefrecoil", ";#phi(h_{T}) - #phi^{gen}(H)", 100,0,3.2) );


  //replicate monitor for categories
  TString cats[]={"ee","emu","mumu"};
  TString subcats[]={"","eq0jets","eq1jets","geq2jets"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(size_t isubcat=0;isubcat<sizeof(subcats)/sizeof(TString); isubcat++)
	controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]);
    }
  
  //Start the reduced met computer
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);

  //aux
  TransverseMassComputer mtComp;

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
      //      if(phys.genhiggs.size()==0) continue;

      //event categories
      TString evcat("");
      if(ev.cat==EMU)  evcat="emu";
      if(ev.cat==MUMU) evcat="mumu";
      if(ev.cat==EE)   evcat="ee";
      TString cats[]={"all",evcat};


      //weights for this event
      float puweight=ev.weight;
      float weight=puweight*ev.hptWeights[0];
     
      //the higgs
      LorentzVector higgs = phys.genhiggs[0];

      //z kinematics
      LorentzVector zll=phys.leptons[0]+phys.leptons[1];
      LorentzVector genzll=phys.genleptons[0]+phys.genleptons[1];
      LorentzVector zvv=phys.met[0];
      LorentzVector genzvv=phys.genmet[0];
      float transverseMass = mtComp.compute(zll,zvv,true);
 
      //higgs recoil
      LorentzVector clusteredRecoil(0,0,0,0);
      LorentzVector unclusteredRecoil=zvv+zll;
      LorentzVector vertexRecoil = phys.vtx;
      vertexRecoil *=-1;

      //jet activity
      LorentzVectorCollection jetsp4, genjets;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) { 
	jetsp4.push_back(phys.jets[ijet]);
	clusteredRecoil += phys.jets[ijet];
      }
      clusteredRecoil *=-1;
      for(size_t ijet=0; ijet<phys.genjets.size(); ijet++) genjets.push_back(phys.genjets[ijet]);

      //subcategory
      TString subcat("eq0jets");
      if(jetsp4.size()==1) subcat="eq1jets";
      if(jetsp4.size()>1)  subcat="geq2jets";
      TString subCats[]={"",subcat};
           
      //redmet
      rmetComp.compute(phys.leptons[0], phys.leptons[0].ptErr, phys.leptons[1], phys.leptons[1].ptErr, jetsp4, zvv );
      double rmet = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      double rmetT = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      double rmetL = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;      
      int prefrecoil = rmetComp.getPreferedRecoil(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      LorentzVector redMetp4(rmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Px(),
			     rmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Py(),
			     0,
			     rmet);
      //
      LorentzVector recoilEstimate(prefrecoil==ReducedMETComputer::CLUSTERED ? clusteredRecoil : unclusteredRecoil );

      LorentzVector unclRecoil200(recoilEstimate.px(),recoilEstimate.py(), 0,sqrt(pow(200,2)+pow(recoilEstimate.pt(),2)));
      ROOT::Math::Boost cmboost200(unclRecoil200.BoostToCM());
      LorentzVector cm200Zll(cmboost200(zll));
      LorentzVector cm200Zvv(cmboost200(zvv));
      float transverseMass200 = mtComp.compute(cm200Zll,cm200Zvv,true);

      LorentzVector unclRecoil400(recoilEstimate.px(),recoilEstimate.py(), 0,sqrt(pow(400,2)+pow(recoilEstimate.pt(),2)));
      ROOT::Math::Boost cmboost400(unclRecoil400.BoostToCM());
      LorentzVector cm400Zll(cmboost400(zll));
      LorentzVector cm400Zvv(cmboost400(zvv));
      float transverseMass400 = mtComp.compute(cm400Zll,cm400Zvv,true);

      
      ROOT::Math::Boost dilcmboost(zll.BoostToCM());
      LorentzVector dilcmRedMet(dilcmboost(redMetp4));
      float dphistar=deltaPhi(dilcmRedMet.phi(),zll.phi());
      float dphistarrec=deltaPhi(dilcmRedMet.phi(),recoilEstimate.phi());

      // cross-check (generator level)
      //       LorentzVector genRecoil2D(higgs);
      //       ROOT::Math::Boost cmgenboost(genRecoil2D.BoostToCM());
      //       LorentzVector cmGenZll(cmgenboost(genzll));
      //       LorentzVector cmGenZvv(cmgenboost(genzvv));
      
      
      //subcategory analysis
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	  {
	    TString ctf=cats[ic]+subCats[isc];

	    //higgs pT
	    for(size_t ivar=0; ivar<sizeof(hptvars)/sizeof(TString); ivar++)
	      {
		double newweight(puweight);
		if(ivar) newweight *= ev.hptWeights[ivar-1];
		controlHistos.fillHisto("hpt"+hptvars[ivar],ctf, higgs.pt(),newweight,true);  
	      }
	    
	    controlHistos.fillHisto("recozpt",ctf, zll.pt(),weight);  
	    controlHistos.fillHisto("genzpt",ctf, genzll.pt(),weight);  
	    controlHistos.fillHisto("recozvvpt",ctf, zvv.pt(),weight);  
	    controlHistos.fillHisto("genzvvpt",ctf, genzvv.pt(),weight);  
	    controlHistos.fillHisto("reconjets",ctf, jetsp4.size(),weight);      	    
	    controlHistos.fillHisto("gennjets",ctf, genjets.size(),weight);    
	    controlHistos.fillHisto("recomt",ctf, transverseMass,weight);      
	    controlHistos.fillHisto("genm",ctf, higgs.mass(),weight);      

	    //reduced MET
	    controlHistos.fill2DHisto("recoredmetcomps", ctf,rmetL,rmetT,weight);
	    controlHistos.fillHisto("recoredmetL", ctf,rmetL,weight);
	    controlHistos.fillHisto("recoredmetT", ctf,rmetT,weight);
	    controlHistos.fillHisto("recoredmet", ctf,rmet,weight);	

	    controlHistos.fillHisto("recodilcmzvvpt",ctf,dilcmRedMet.pt(),weight);
	    controlHistos.fillHisto("recodilcmdphistar",ctf,dphistar,weight);
	    controlHistos.fillHisto("recodilcmdphistarrecoil",ctf,dphistarrec,weight);

	    //higgs recoil
	    controlHistos.fillHisto("recoprefrecoil",ctf, recoilEstimate.pt(),weight);  
	    controlHistos.fillHisto("deltaprefrecoil",ctf, recoilEstimate.pt()-higgs.pt(),weight);  
	    controlHistos.fillHisto("deltaphiprefrecoil",ctf, fabs(deltaPhi(recoilEstimate.phi(),higgs.phi())),weight);  

	    //pseudo-CM distributions
	    controlHistos.fillHisto("recomtcm",ctf, transverseMass200,weight);  
	    controlHistos.fillHisto("recodeltacmzz",ctf, cm200Zll.pt()-cm200Zvv.pt(),weight);  
	    controlHistos.fillHisto("recocmzpt",ctf, cm200Zll.pt(),weight);  
	    controlHistos.fillHisto("recocmzvvpt",ctf, cm200Zvv.pt(),weight);  
	    
	    controlHistos.fillHisto("recomtcm400",ctf, transverseMass400,weight);  
	    controlHistos.fillHisto("recodeltacmzz400",ctf, cm400Zll.pt()-cm400Zvv.pt(),weight);  
	    controlHistos.fillHisto("recocmzpt400",ctf, cm400Zll.pt(),weight);  
	    controlHistos.fillHisto("recocmzvvpt400",ctf, cm400Zvv.pt(),weight);  
	  }
    }
  
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/mumu/mumu_cutflow");
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
