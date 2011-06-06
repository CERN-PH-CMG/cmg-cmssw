#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"
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

typedef std::pair<LorentzVector,float> PhysicsObject; 
typedef std::vector<PhysicsObject> PhysicsObjectCollection;

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

  controlHistos.addHistogram(  new TH1D ("dphizrmet", ";#Delta#phi(Z,red-MET);Events", 100,0,3.4) );
  controlHistos.addHistogram(  new TH1D ("zpt", ";p_{T}(Z);Events", 100,0,400) );
  controlHistos.addHistogram( new TH1F ("redmetL", ";red-MET_{L};Events", 100, -250.,250.) );
  controlHistos.addHistogram( new TH1F ("redmetT", ";red-MET_{T};Events", 100, -250.,250.) );
  controlHistos.addHistogram( new TH1F ("redmet", ";red-MET;Events", 100, 0.,400.) );
  controlHistos.addHistogram( new TH1F ("met", ";MET;Events", 100, 0.,400.) );
  controlHistos.addHistogram( new TH1F ("mtwithrmet", ";M_{T};Events", 100, 100.,500.) );
  controlHistos.addHistogram( new TH1F ("redmetLmzpt", "; red-MET_{L}-p_{T}(Z);Events", 100,-249,251) );
  controlHistos.addHistogram( new TH1F ("redmetLpzpt", "; red-MET_{L}+p_{T}(Z);Events", 100,0,400) );
  controlHistos.addHistogram( new TH1D ("summtl2rmet", ";#Sigma M_{T}(l_{1},red-MET)+M_{T}(l_{2},red-MET);Events",100,0,500) );
  controlHistos.addHistogram( new TH1D ("ptlead", ";leading lepton p_{T};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("ptsublead", ";next-leading lepton p_{T};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("sumptlep", ";#Sigma p_{T} (leptons);Events",100,0,500) );

  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetLvsmet", ";red-MET_{L};MET;Events",100,-249,251,100, 0.,250.) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetvsmet", ";red-MET;MET;Events",100,-249,251,100, 0.,250.) ) );

  //replicate monitor for categories
  TString cats[]={"ee","emu","mumu"};
  TString subcats[]={"","eq0jets","eq1jets","geq2jets"};
  TString cuts[]={"cut1","cut2","cut3"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(size_t isubcat=0;isubcat<sizeof(subcats)/sizeof(TString); isubcat++)
	{
	  controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]);
	  for(size_t icut=0;icut<sizeof(cuts)/sizeof(TString); icut++)
	    {
	      controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]+cuts[icut]);
	    }
	}
    }

   
  //Start the reduced met fitter/computer
  //ReducedMETFitter rmet(runProcess);
  ReducedMETComputer rmetComp(1.,1.,1.,1.,1.);
    
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
      float weight=ev.weight;

      //get the physics object
      PhysicsObjectCollection vtx,leptons, jets, mets;
      LorentzVectorCollection jetsp4;
      LorentzVector jet1(0,0,0,0),jet2(0,0,0,0);
      LorentzVector jetsum(0,0,0,0);
      int nbtags(0);
      for(Int_t ipart=0; ipart<ev.nparticles; ipart++)
	{
	  LorentzVector p4(ev.px[ipart],ev.py[ipart],ev.pz[ipart],ev.en[ipart]);
	  switch( ev.id[ipart] )
	    {
	    case 0:
	      mets.push_back( PhysicsObject(p4,ev.info2[ipart]) );
	      break;
	    case 1:
	      jetsp4.push_back(p4);
	      jetsum += p4;
	      jets.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      if(p4.pt()>30) nbtags += (ev.info1[ipart]>2);
	      if(p4.pt()>jet1.pt()) { jet2=jet1; jet1=p4; }
	      else if(p4.pt()>jet2.pt()) { jet2=p4; }
	      break;
	    case 11: case -11: case 13: case -13:
	      leptons.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    case 500:
	      vtx.push_back( PhysicsObject(p4,p4.pt()) );
	      break;
	    }
	}
      
      double mjj(0);
      if(jet1.pt()>0 && jet2.pt()>0) {
	LorentzVector jj=jet1+jet2;
	mjj=jj.mass();
      }

      //event categories
      TString evcat("");
      if(ev.cat==dilepton::EMU)  evcat="emu";
      if(ev.cat==dilepton::MUMU) evcat="mumu";
      if(ev.cat==dilepton::EE)   evcat="ee";

      TString subcat("eq0jets");
      if(jetsp4.size()==1) subcat="eq1jets";
      if(jetsp4.size()>1)  subcat="geq2jets";

      //transverse mass
      LorentzVector dileptonP=leptons[0].first+leptons[1].first;
      TVector2 redlep1(leptons[0].first.Px(),leptons[0].first.Py());
      TVector2 redlep2(leptons[1].first.Px(),leptons[1].first.Py());
      TVector2 reddil=redlep1+redlep2;
      TVector2 bisector = (redlep1.Unit()+redlep2.Unit()).Unit();
      //double dphidilbisector=reddil.DeltaPhi(bisector);
      LorentzVector jesMetNopuP=mets[0].first;
      float rawMET = mets[0].second;
      LorentzVector transvSum=dileptonP + jesMetNopuP;
      float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.pt(),2)+pow(dileptonP.mass(),2))+TMath::Sqrt(TMath::Power(jesMetNopuP.pt(),2)+pow(dileptonP.mass(),2)),2);
      transverseMass-=TMath::Power(transvSum.pt(),2);
      transverseMass=TMath::Sqrt(transverseMass);
      float ptasymm=(leptons[0].first.pt()-leptons[1].first.pt())/(leptons[0].first.pt()+leptons[1].first.pt());
     
      //z kinematics
      double zpt = dileptonP.Pt();
      double zeta = dileptonP.Eta();

      //met
      TVector2 redjesMetNopuP(jesMetNopuP.px(),jesMetNopuP.py());
      double dphizz = reddil.DeltaPhi(redjesMetNopuP);
      
      //redmet
      rmetComp.compute(leptons[0].first,leptons[0].second,
		       leptons[1].first,leptons[1].second,
		       jetsp4,
		       mets[0].first);
      double rmet=rmetComp.reducedMET();
      double rmetL = rmetComp.reducedMETComponents().first;
      double rmetT = rmetComp.reducedMETComponents().second;
      double dphizrmet=TMath::ACos(-rmetL/rmet);
      
      double sumjetL = rmetComp.sumJetProjComponents().first;
      double sumjetT = rmetComp.sumJetProjComponents().second;
      double calometL = rmetComp.metProjComponents().first;
      double calometT = rmetComp.metProjComponents().second;
      int recoilL = rmetComp.recoilType().first;
      int recoilT = rmetComp.recoilType().second;
      
      float rx=rmetComp.reducedMETcartesian().Px();
      float ry=rmetComp.reducedMETcartesian().Py();
      LorentzVector redmetxy(rx,ry,0,sqrt(rx*rx+ry*ry));
      transvSum=dileptonP + redmetxy;
      float transverseMassWithRmet=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.pt(),2)+pow(dileptonP.mass(),2))+TMath::Sqrt(TMath::Power(redmetxy.pt(),2)+pow(dileptonP.mass(),2)),2);
      transverseMassWithRmet-=TMath::Power(transvSum.pt(),2);
      transverseMassWithRmet=TMath::Sqrt(transverseMassWithRmet);
      
      //transverse masses                                               
       float dphil2met[]={ fabs(deltaPhi(redmetxy.phi(),leptons[0].first.phi())), fabs(deltaPhi(redmetxy.phi(),leptons[1].first.phi())) };
       float mtl2met[]={ TMath::Sqrt(2*redmetxy.pt()*leptons[0].first.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*redmetxy.pt()*leptons[1].first.pt()*(1-TMath::Cos(dphil2met[1]))) };
       float dphill=deltaPhi(leptons[0].first.phi(), leptons[1].first.phi() );

      //fill control histograms
      TString cats[]={"all",evcat};
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	controlHistos.fillHisto("cutflow",cats[ic],1,weight);

      //std pre-selection
      bool passStd(nbtags<1 && zpt>25);
      TString subCats[]={"",subcat};
      std::vector<TString> cuts;
      cuts.push_back("");
      if(passStd) cuts.push_back("cut1");
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  if(passStd) controlHistos.fillHisto("cutflow",cats[ic],2,weight);
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    for(std::vector<TString>::iterator cIt=cuts.begin(); cIt!= cuts.end(); cIt++)
	      {
		TString ctf=cats[ic]+subCats[isc]+*cIt;
		controlHistos.fillHisto("mtwithrmet",ctf,transverseMassWithRmet,weight);		
		controlHistos.fillHisto("dphizrmet",ctf,dphizrmet,weight);		
		controlHistos.fillHisto("met",ctf,jesMetNopuP.pt(),weight);
		controlHistos.fillHisto("redmet",ctf,rmet,weight);
		controlHistos.fillHisto("redmetL",ctf,rmetL,weight);
		controlHistos.fillHisto("redmetT",ctf,rmetT,weight);
		controlHistos.fillHisto("zpt",ctf,zpt,weight);
		controlHistos.fillHisto("redmetLmzpt",ctf,rmetL-zpt,weight);
		controlHistos.fillHisto("redmetLpzpt",ctf,rmetL+zpt,weight);
		controlHistos.fillHisto("ptlead",ctf,leptons[0].first.pt(),weight);
		controlHistos.fillHisto("ptsublead",ctf,leptons[1].first.pt(),weight);
		controlHistos.fillHisto("sumptlep",ctf,leptons[0].first.pt()+leptons[1].first.pt(),weight);
		controlHistos.fillHisto("redmetLvsmet",ctf,rmetL,jesMetNopuP.pt(),weight);
		controlHistos.fillHisto("redmetvsmet",ctf,rmet,jesMetNopuP.pt(),weight);
	      }

	}

      //extra cut
      bool passSum(passStd && rmetL+zpt>50 && dphizrmet>1.5);
      cuts.clear();
      if(passSum) cuts.push_back("cut2");
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  if(passSum) controlHistos.fillHisto("cutflow",cats[ic],3,weight);
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    for(std::vector<TString>::iterator cIt=cuts.begin(); cIt!= cuts.end(); cIt++)
	      {
		TString ctf=cats[ic]+subCats[isc]+*cIt;
		controlHistos.fillHisto("redmetLmzpt",ctf,rmetL-zpt,weight);
		controlHistos.fillHisto("redmetLpzpt",ctf,rmetL+zpt,weight);
		controlHistos.fillHisto("mtwithrmet",ctf,transverseMassWithRmet,weight);
		controlHistos.fillHisto("dphizrmet",ctf,dphizrmet,weight);
		controlHistos.fillHisto("summtl2rmet",ctf,mtl2met[0]+mtl2met[1],weight);
		controlHistos.fillHisto("ptlead",ctf,leptons[0].first.pt(),weight);
		controlHistos.fillHisto("ptsublead",ctf,leptons[1].first.pt(),weight);
		controlHistos.fillHisto("sumptlep",ctf,leptons[0].first.pt()+leptons[1].first.pt(),weight);
		controlHistos.fillHisto("redmetT",ctf,rmetT,weight);
	      }
	}

      bool passDiff(passSum && rmetT>0 && rmetL-zpt>-25);
      cuts.clear();
      if(passDiff) cuts.push_back("cut3");
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  if(passSum) controlHistos.fillHisto("cutflow",cats[ic],3,weight);
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    for(std::vector<TString>::iterator cIt=cuts.begin(); cIt!= cuts.end(); cIt++)
	      {
		TString ctf=cats[ic]+subCats[isc]+*cIt;
		controlHistos.fillHisto("redmetLmzpt",ctf,rmetL-zpt,weight);
		controlHistos.fillHisto("redmetLpzpt",ctf,rmetL+zpt,weight);
		controlHistos.fillHisto("mtwithrmet",ctf,transverseMassWithRmet,weight);
		controlHistos.fillHisto("dphizrmet",ctf,dphizrmet,weight);
		controlHistos.fillHisto("summtl2rmet",ctf,mtl2met[0]+mtl2met[1],weight);
		controlHistos.fillHisto("ptlead",ctf,leptons[0].first.pt(),weight);
		controlHistos.fillHisto("ptsublead",ctf,leptons[1].first.pt(),weight);
		controlHistos.fillHisto("sumptlep",ctf,leptons[0].first.pt()+leptons[1].first.pt(),weight);
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
