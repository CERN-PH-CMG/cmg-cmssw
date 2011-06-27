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
  bool useFitter = runProcess.getParameter<bool>("useFitter");
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
      if(itier)
	controlHistos.addHistogram( new TH1D (tiers[itier]+"unclustrecoil", ";"+tiers[itier]+" h_{T};Events", 100,0,200) );

      controlHistos.addHistogram( new TH1D (tiers[itier]+"zpt", ";"+tiers[itier]+" p_{T}^{ll};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"zvvpt", ";"+tiers[itier]+" p_{T}^{#nu#nu};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"njets", ";"+tiers[itier]+" Jets;Jets",4,0,4) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"mt", ";"+tiers[itier]+" M_{T};Events", 100, 100.,500.) );
      controlHistos.addHistogram(  new TH1D (tiers[itier]+"deltacmzz", ";p_{T}^{ll}-p_{T}^{vv}(CM)", 100,-100,100) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"summtl2met", ";"+tiers[itier]+" M_{T}(l^{i},MET)+M_{T}(l^{i},MET);Events",100,0,1000) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetLminrecoil", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel} (min. recoil);Events", 100, -200.,200.) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetTminrecoil", ";"+tiers[itier]+" red-E_{T}^{miss,#perp} (min. recoil);Events", 100, -200.,200.) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetL", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};Events", 100, -200.,200.) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetT", ";"+tiers[itier]+" red-E_{T}^{miss,#perp};Events", 100, -200.,200.) );
      controlHistos.addHistogram( (TH1D *)(new TH2D (tiers[itier]+"redmetcomps", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
      controlHistos.addHistogram( (TH1D *)(new TH2D (tiers[itier]+"redmetcompsminrecoil", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel} (min recoil);red-E_{T}^{miss,#perp} (min recoil);Events", 100, -251.,249,100, -251.,249.) ) );
    }
  controlHistos.addHistogram(  new TH1D ("deltaunclustrecoil", ";h_{T} - p_{T}^{gen}(H)", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltaphiunclustrecoil", ";#phi(h_{T}) - #phi^{gen}(H)", 100,0,3.2) );
  controlHistos.addHistogram(  new TH1D ("deltacmzpt", ";#Delta p_{T}^{ll}(CM)", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltacmzvvpt", ";#Delta p_{T}^{#nu#nu}(CM)", 100,-100,100) );

  controlHistos.addHistogram(  new TH1D ("deltamt", ";#Delta M_{T};Events", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltasummtl2met", ";#Delta #Sigma M_{T}(l^{i},MET); Events", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltazpt", ";#Delta p_{T}^{ll};Events", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltazvvpt", ";#Delta p_{T}^{#nu#nu};Events", 100,-100,100) );

  controlHistos.addHistogram(  new TH1D ("deltazptL", ";#Delta p_{T}^{ll,#parallel};Events", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltazvvptL", ";#Delta p_{T}^{#nu#nu,#parallel};Events", 100,-100,100) );

  controlHistos.addHistogram( new TH1F ("deltanjets", ";#Delta Jets;Events", 6,-1.5,4.5) );

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
      if(phys.genhiggs.size()==0) continue;

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

      //higgs recoil
      LorentzVector clusteredRecoil(0,0,0,0);
      LorentzVector unclusteredRecoil=zvv+zll;
      LorentzVector vertexRecoil = phys.vtx[0];
      vertexRecoil *=-1;

      //jet activity
      LorentzVectorCollection jetsp4, genjets;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) { 
	jetsp4.push_back(phys.jets[ijet]);
	clusteredRecoil += phys.jets[ijet];
      }
      clusteredRecoil *=-1;
      for(size_t ijet=0; ijet<phys.genjets.size(); ijet++) genjets.push_back(phys.genjets[ijet]);

      //transverse mass
      LorentzVector transvSum=zll+zvv;
      float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(zll.pt(),2)+pow(zll.mass(),2))+TMath::Sqrt(TMath::Power(zvv.pt(),2)+pow(zll.mass(),2)),2);
      transverseMass-=TMath::Power(transvSum.pt(),2);
      transverseMass=TMath::Sqrt(transverseMass);
      
      //approximate transformation to rest frame
      LorentzVector unclRecoil2D(unclusteredRecoil.px(),unclusteredRecoil.py(), 0,sqrt(pow(transverseMass,2)+pow(unclusteredRecoil.pt(),2)));
      ROOT::Math::Boost cmboost(unclRecoil2D.BoostToCM());
      LorentzVector cmZll(cmboost(zll));
      LorentzVector cmZvv(cmboost(zvv));

      float minmass(-1),massstep(10), mindiff(10000);
      for(float imass=10; imass<=500; imass+=massstep)
	{
	  LorentzVector tmpRecoil2D(unclusteredRecoil.px(),unclusteredRecoil.py(), 0,sqrt(pow(imass,2)+pow(unclusteredRecoil.pt(),2)));
	  ROOT::Math::Boost tmpcmboost(tmpRecoil2D.BoostToCM());
	  LorentzVector tmpcmZll(tmpcmboost(zll));
	  LorentzVector tmpcmZvv(tmpcmboost(zvv));
	  float ptdiff=tmpcmZll.pt()-tmpcmZvv.pt();
	  if(fabs(ptdiff)<fabs(mindiff)) 
	    {
	      mindiff=ptdiff;
	      minmass=imass;
	    }
	  else break;
	}
      cout << minmass << " " << mindiff << endl;

      LorentzVector genRecoil2D(higgs.px(),higgs.py(),0.,sqrt(pow(higgs.mass(),2)+pow(higgs.pt(),2)));
      //LorentzVector genRecoil2D(higgs);
      ROOT::Math::Boost cmgenboost(genRecoil2D.BoostToCM());
      LorentzVector cmGenZll(cmgenboost(genzll));
      LorentzVector cmGenZvv(cmgenboost(genzvv));
      

      //event categories
      TString evcat("");
      if(ev.cat==dilepton::EMU)  evcat="emu";
      if(ev.cat==dilepton::MUMU) evcat="mumu";
      if(ev.cat==dilepton::EE)   evcat="ee";
      TString cats[]={"all",evcat};

      TString subcat("eq0jets");
      if(jetsp4.size()==1) subcat="eq1jets";
      if(jetsp4.size()>1)  subcat="geq2jets";
      TString subCats[]={"",subcat};

     
      LorentzVector gentransvSum=genzll+genzvv;
      float gentransverseMass=TMath::Power(TMath::Sqrt(TMath::Power(genzll.pt(),2)+pow(genzll.mass(),2))+TMath::Sqrt(TMath::Power(genzvv.pt(),2)+pow(genzll.mass(),2)),2);
      gentransverseMass-=TMath::Power(gentransvSum.pt(),2);
      gentransverseMass=TMath::Sqrt(gentransverseMass);

      float dphil2met[]={ fabs(deltaPhi(zvv.phi(),phys.leptons[0].phi())), fabs(deltaPhi(zvv.phi(),phys.leptons[1].phi())) };
      float mtl2met[]={ TMath::Sqrt(2*zvv.pt()*phys.leptons[0].pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*zvv.pt()*phys.leptons[1].pt()*(1-TMath::Cos(dphil2met[1]))) };

      float gendphil2met[]={ fabs(deltaPhi(genzvv.phi(),phys.genleptons[0].phi())), fabs(deltaPhi(genzvv.phi(),phys.genleptons[1].phi())) };
      float genmtl2met[]={ TMath::Sqrt(2*genzvv.pt()*phys.genleptons[0].pt()*(1-TMath::Cos(gendphil2met[0]))) ,   TMath::Sqrt(2*zvv.pt()*phys.genleptons[1].pt()*(1-TMath::Cos(gendphil2met[1]))) };
      
      //redmet
      rmetComp.compute(phys.leptons[0], phys.leptons[0].info[0], phys.leptons[1], phys.leptons[1].info[0], jetsp4, zvv );
      double rmetTminrecoil = rmetComp.reducedMETComponents().first;
      double rmetLminrecoil = rmetComp.reducedMETComponents().second;
      double rmetT = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      double rmetL = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;      
      
      rmetComp.compute(phys.genleptons[0], 0., phys.genleptons[1], 0., genjets, genzvv);
      double genrmetTminrecoil = rmetComp.reducedMETComponents().first;
      double genrmetLminrecoil = rmetComp.reducedMETComponents().second;
      double genrmetT = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      double genrmetL = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;      

      //bisector
      TVector2 bisector = rmetComp.defineThrust(phys.leptons[0],0,phys.leptons[1],0).second;
      double dphill=fabs(deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi()));
      TVector2 redZll(phys.leptons[0].px()+phys.leptons[1].px(),phys.leptons[0].py()+phys.leptons[1].py());
      TVector2 redZvv(zvv.px(),zvv.py());
      double zllptL=redZll*bisector;
      double zvvptL=redZvv*bisector;
      
      TVector2 genbisector = rmetComp.defineThrust(phys.genleptons[0],0,phys.genleptons[1],0).second;
      TVector2 genredZll(phys.genleptons[0].px()+phys.genleptons[1].px(),phys.genleptons[0].py()+phys.genleptons[1].py());
      TVector2 genredZvv(genzvv.px(),genzvv.py());
      double genzllptL=genredZll*bisector;
      double genzvvptL=genredZvv*bisector;

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
	    
	    //higgs recoil
	    controlHistos.fillHisto("recounclustrecoil",ctf, unclusteredRecoil.pt(),weight);  
	    controlHistos.fillHisto("deltaunclustrecoil",ctf, unclusteredRecoil.pt()-higgs.pt(),weight);  
	    controlHistos.fillHisto("deltaphiunclustrecoil",ctf, fabs(deltaPhi(unclusteredRecoil.phi(),higgs.phi())),weight);  
	  
	    controlHistos.fillHisto("recozvvpt",ctf, zvv.pt(),weight);  
	    controlHistos.fillHisto("genzvvpt",ctf, genzvv.pt(),weight);  
	    controlHistos.fillHisto("deltazvvptL",ctf, zvvptL-genzvvptL,weight);  
	    controlHistos.fillHisto("deltazvvpt",ctf, zvv.pt()-genzvv.pt(),weight);  

	    controlHistos.fillHisto("recodeltacmzz",ctf, cmZll.pt()-cmZvv.pt(),weight);  
	    controlHistos.fillHisto("gendeltacmzz",ctf, cmGenZll.pt()-cmGenZvv.pt(),weight);  
	    controlHistos.fillHisto("deltacmzpt",ctf, cmZll.pt()-cmGenZll.pt(),weight);  
	    controlHistos.fillHisto("deltacmzvvpt",ctf, cmZvv.pt()-cmGenZvv.pt(),weight);  

	    controlHistos.fillHisto("recozpt",ctf, zll.pt(),weight);  
	    controlHistos.fillHisto("genzpt",ctf, genzll.pt(),weight);  
	    controlHistos.fillHisto("deltazptL",ctf, zllptL-genzllptL,weight);  
	    controlHistos.fillHisto("deltazpt",ctf, zll.pt()-genzll.pt(),weight);  

	    controlHistos.fillHisto("gennjets",ctf, genjets.size(),weight);    
	    controlHistos.fillHisto("reconjets",ctf, jetsp4.size(),weight);      
	    controlHistos.fillHisto("deltanjets",ctf, jetsp4.size()-genjets.size(),weight);

	    controlHistos.fillHisto("recomt",ctf, transverseMass,weight);      
	    controlHistos.fillHisto("genmt",ctf, gentransverseMass,weight);      
	    controlHistos.fillHisto("deltamt",ctf, transverseMass-gentransverseMass,weight);      
	    
	    controlHistos.fillHisto("recosummtl2met",ctf,mtl2met[0]+mtl2met[1],weight);
	    controlHistos.fillHisto("gensummtl2met",ctf,genmtl2met[0]+genmtl2met[1],weight);
	    controlHistos.fillHisto("deltasummtl2met",ctf,(mtl2met[0]+mtl2met[1])-(genmtl2met[0]+genmtl2met[1]),weight);	    
	    
	    controlHistos.fill2DHisto("recoredmetcomps", ctf,rmetL,rmetT,weight);
	    controlHistos.fillHisto("recoredmetL", ctf,rmetL,weight);
	    controlHistos.fillHisto("recoredmetT", ctf,rmetT,weight);	
	    controlHistos.fill2DHisto("genredmetcomps", ctf,genrmetL,genrmetT,weight);
	    controlHistos.fillHisto("genredmetL", ctf,genrmetL,weight);
	    controlHistos.fillHisto("genredmetT", ctf,genrmetT,weight);	
	    
	    controlHistos.fill2DHisto("recoredmetcompsminrecoil", ctf,rmetLminrecoil,rmetTminrecoil,weight);
	    controlHistos.fillHisto("recoredmetLminrecoil", ctf,rmetLminrecoil,weight);
	    controlHistos.fillHisto("recoredmetTminrecoil", ctf,rmetTminrecoil,weight);	
	    controlHistos.fill2DHisto("genredmetcompsminrecoil", ctf,genrmetLminrecoil,genrmetTminrecoil,weight);
	    controlHistos.fillHisto("genredmetLminrecoil", ctf,genrmetLminrecoil,weight);
	    controlHistos.fillHisto("genredmetTminrecoil", ctf,genrmetTminrecoil,weight);	
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
