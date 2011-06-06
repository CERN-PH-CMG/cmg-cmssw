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

  //Z/redmet kinematics

  Float_t redmetBins[]={0,2,4,6,8,10,12,14,16,18,20,25,30,40,50,60,80,100,200};
  Int_t nredmetBins=sizeof(redmetBins)/sizeof(Float_t)-1;
  controlHistos.addHistogram( new TH1F ("redmet", ";red-E_{T}^{miss};Events", nredmetBins,redmetBins) );
  controlHistos.addHistogram( new TH1F ("met", ";type-I E_{T}^{miss};Events", nredmetBins,redmetBins) );
  controlHistos.addHistogram(  new TH1D ("zpt", ";p_{T}(Z);Events", 100,0,400) );
  controlHistos.addHistogram(  new TH1D ("zeta", ";#eta (Z);Events", 100,-5,5) );
  controlHistos.addHistogram( (TH1D *)( new TH2D ("zptvszeta", ";p_{T}(Z);#eta(Z)", 100,0,400,100,-5,5) ) );
  controlHistos.addHistogram( new TH1F ("redmetL", ";red-E_{T}^{miss,#parallel};Events", 100, -250.,250.) );
  controlHistos.addHistogram( new TH1F ("redmetT", ";red-E_{T}^{miss,#perp};Events", 100, -250.,250.) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetcomps", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};;Events", 100, -250.,250,100, -250.,250.) ) );
 
 /*
  controlHistos.addHistogram( new TH1F ("mt", ";M_{T};Events", 100, 100.,500.) );
  controlHistos.addHistogram( new TH1F ("mjj", ";M_{JJ};Events", 100, 0.,200.) );
  controlHistos.addHistogram( new TH1F ("met", ";MET;Events", 100, 0.,250.) );
  // controlHistos.addHistogram( new TH1F ("clustmet", ";clustered MET;Events", 100, 0.,250.) );

  // controlHistos.addHistogram( (TH1D *)(new TH2D ("sumjetvsmetL", ";#Sigma Jet_{L};MET_{L};Events", 100, -250.,250,100, -249.,1.) ) );
  //controlHistos.addHistogram( (TH1D *)(new TH2D ("sumjetvsmetT", ";#Sigma Jet_{T};MET_{T};Events", 100, -250.,250,100, -249.,1.) ) );
  //controlHistos.addHistogram( (TH1D *)( new TH2D ("zptvsdphizrmet", ";p_{T} (Z); #Delta#phi(p_{T}(Z),red-MET)", 100,-1,399,100,0,3.2) ) );
  //  TH1F *h = new TH1F ("redmetLType", ";red-MET_{L} type;Events", 2,0,2);
  //  h->GetXaxis()->SetBinLabel(1,"jet-like");
  //  h->GetXaxis()->SetBinLabel(2,"met-like");
  //  controlHistos.addHistogram( h );
  //  h = new TH1F ("redmetTType", ";red-MET_{T} type;Events", 2,0,2);
  //  h->GetXaxis()->SetBinLabel(1,"jet-like");
  //  h->GetXaxis()->SetBinLabel(2,"met-like");
  //  controlHistos.addHistogram( h );
  controlHistos.addHistogram( new TH1F ("redmetLpzpt", "; red-MET_{L}+p_{T}(Z);Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("redmetLmzpt", "; red-MET_{L}-p_{T}(Z);Events", 100,-249,251) );
  //  controlHistos.addHistogram( new TH1F ("dphizredmet", ";#Delta#phi[p_{T}(Z),red-MET];Events", 100,0,3.2) );
  //  controlHistos.addHistogram( new TH1D ("dphizrmet", ";#Delta#phi(p_{T}(Z),red-MET);Events", 100,0,3.2) );	
  // controlHistos.addHistogram( (TH1D *)( new TH2D ("redmetpzptvsdphizz", ";red-MET_{L}+p_{T}(Z); #Delta#phi[p_{T}(Z),MET]", 100,0,500,100,0,3.2) ) );
  //  controlHistos.addHistogram( (TH1D *)( new TH2D ("redmetLmzptvsdphizrmet", ";red-MET_{L}-p_{T}(Z); #Delta#phi[p_{T}(Z),red-MET]", 100,-249,251,100,0,3.2) ) );
  //  controlHistos.addHistogram( (TH1D *)( new TH2D ("redmetLmzptvsredmetpzpt", ";red-MET_{L}-p_{T}(Z); red-MET+p_{T}(Z)", 100,-249,251,100,0,500) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetLpzptvsmt", ";red-MET_{L}+p_{T}(Z);M_{T};Events",100,0,500,100, 100.,500.) ) );

  controlHistos.addHistogram( (TH1D *)(new TH2D ("dphil2met", ";#Delta#phi(l_{1},MET);#Delta#phi(l_{2},MET);Events",100,0,4,100,0,4) ) );
  controlHistos.addHistogram( new TH1D ("dphill", ";#Delta#phi(l_{1},l_{2});Events",100,0,3.2) );
  controlHistos.addHistogram( new TH1D ("ptasymm", ";[p_{T}(1)-p_{T}(2)]/[p_{T}(1)+p_{T}(2)];Events",100,0,1.05) );
  controlHistos.addHistogram( new TH1D ("ptlead", ";leading lepton p_{T};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("ptsublead", ";next-leading lepton p_{T};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("sumptlep", ";#Sigma p_{T} (leptons);Events",100,0,500) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("mtl2met", ";M_{T}(l_{1},MET);M_{T}(l_{2},MET);Events",100,0,250,100,0,250) ) );
  controlHistos.addHistogram( new TH1D ("summtl2met", ";#Sigma M_{T}(l_{1},MET)+M_{T}(l_{2},MET);Events",100,0,500) );

  TString steps[]={"|M_{ll}-M_{Z}|<15 GeV/c^{2}","p_{T}+red-MET_{L} #wedge |#eta|<2.5","red-MET_{L}+red-MET_{T}>0","p_{T}-red-MET_{L}<50","=0 jets","=1 jet","#geq 2 jets"};
  TH1D *cutflow=new TH1D ("cutflow", ";Event selection;Events",5,0,5);
  controlHistos.addHistogram( cutflow  );
  */
  //binned plots
  /*  const float zptbins[]={20.,50.,100.,7000.};
  for(size_t izptbin=0; izptbin<sizeof(zptbins)/sizeof(float); izptbin++)
    {
      char binbuf[10];
      sprintf(binbuf,"%.0f",zptbins[izptbin]);
      TString zptbin(binbuf);
      char buf[100];
      sprintf(buf,"(%3.1f<p_{T}<%3.1f)",(izptbin==0 ?  0. : zptbins[izptbin-1]),zptbins[izptbin]);
      controlHistos.addHistogram(  new TH1D ("redmetLmzpt_p"+zptbin, ";red-MET_{L}-p_{T}(Z) "+TString(buf)+";Events", 100,-200,200) );
      controlHistos.addHistogram(  new TH1D ("redmetpzpt_p"+zptbin, ";red-MET_{L}+p_{T}(Z) "+TString(buf)+";Events", 100,0,500) );
      controlHistos.addHistogram( new TH1D ("dphizrmet_p"+zptbin, ";#Delta#phi(p_{T}(Z),red-MET)"+TString(buf)+";Events", 100,0,3.2) );	
    }

  const float zetabins[]={1.5,2.4,5.0};
  for(size_t izetabin=0; izetabin<sizeof(zetabins)/sizeof(float); izetabin++)
    {
      char binbuf[10];
      sprintf(binbuf,"%.1f",zetabins[izetabin]);
      TString zetabin(binbuf);
      char buf[100];
      sprintf(buf,"(%3.1f<#eta<%3.1f)",(izetabin==0 ?  0. : zetabins[izetabin-1] ),zetabins[izetabin]);
      controlHistos.addHistogram( new TH1D ("redmetLmzpt_h"+zetabin, ";red-MET_{L}-p_{T}(Z)"+TString(buf)+";Events", 100,-200,200) );
      controlHistos.addHistogram( new TH1D ("redmetpzpt_h"+zetabin, ";red-MET_{L}+p_{T}(Z)"+TString(buf)+";Events", 100,0,500) );
      controlHistos.addHistogram( new TH1D ("dphizrmet_h"+zetabin, ";#Delta#phi(p_{T}(Z),red-MET)"+TString(buf)+";Events", 100,0,3.2) );	
    }
  */
  //replicate monitor for categories
  TString cats[]={"ee","emu","mumu"};
  TString subcats[]={"","eq0jets","eq1jets","geq2jets"};
  TString cuts[]={"cut1","cut2","cut12"};
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
      
      //no b-tagged events
      //if(nbtags>0) continue;
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

      //transverse masses                                               
      float dphil2met[]={ fabs(deltaPhi(jesMetNopuP.phi(),leptons[0].first.phi())), fabs(deltaPhi(jesMetNopuP.phi(),leptons[1].first.phi())) };
      float mtl2met[]={ TMath::Sqrt(2*jesMetNopuP.pt()*leptons[0].first.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*jesMetNopuP.pt()*leptons[1].first.pt()*(1-TMath::Cos(dphil2met[1]))) };
      float dphill=deltaPhi(leptons[0].first.phi(), leptons[1].first.phi() );
      
      //z kinematics
      double zpt = dileptonP.Pt();
      /*
	TString zptbin("");
      for(size_t ibin=0; ibin<sizeof(zptbins)/sizeof(float); ibin++)
	{
	  char binbuf[10];
	  sprintf(binbuf,"%.0f",zptbins[ibin]);
	  if(ibin==0 || (ibin && zpt>zptbins[ibin-1] && zpt<zptbins[ibin])) zptbin = binbuf; 
	}
      */
      double zeta = dileptonP.Eta();
      /*
	TString zetabin("");
	for(size_t ibin=0; ibin<sizeof(zetabins)/sizeof(float); ibin++)
	{
	  char binbuf[10];
	  sprintf(binbuf,"%.1f",zetabins[ibin]);
	  if(ibin==0 || (ibin && fabs(zeta)>zetabins[ibin-1] && fabs(zeta)<zetabins[ibin])) zetabin = binbuf; 
	}
      */

      //met
      TVector2 redjesMetNopuP(jesMetNopuP.px(),jesMetNopuP.py());
      double dphizz = reddil.DeltaPhi(redjesMetNopuP);
      
      //redmet
      rmetComp.compute(leptons[0].first,leptons[0].second,
		       leptons[1].first,leptons[1].second,
		       jetsp4,
		       mets[0].first);
      double rmet=rmetComp.reducedMET();
      double rmetL = rmetComp.reducedMETUnboundComponents().first;
      double rmetT = rmetComp.reducedMETUnboundComponents().second;
      //double dphizrmet=TMath::ACos(-rmetL/rmet);
      
      double sumjetL = rmetComp.sumJetProjComponents().first;
      double sumjetT = rmetComp.sumJetProjComponents().second;
      double calometL = rmetComp.metProjComponents().first;
      double calometT = rmetComp.metProjComponents().second;
      int recoilL = rmetComp.recoilType().first;
      int recoilT = rmetComp.recoilType().second;
      

      //fill control histograms
      TString cats[]={"all",evcat};
      TString subCats[]={"",subcat};
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  controlHistos.fillHisto("cutflow",cats[ic],1,weight);
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    {
	      TString ctf=cats[ic]+subCats[isc];
	      
	      controlHistos.fillHisto("zpt",ctf, zpt,weight,true);
	      controlHistos.fillHisto("zeta",ctf, zeta,weight,true);
	      controlHistos.fill2DHisto("zptvszeta", ctf,zpt,zeta,weight,true);

	      controlHistos.fillHisto("redmet", ctf,rmet,weight,true);
	      controlHistos.fillHisto("met", ctf,jesMetNopuP.pt(),weight,true);
	      controlHistos.fillHisto("redmetL", ctf,rmetL,weight,true);
	      controlHistos.fillHisto("redmetT", ctf,rmetL,weight,true);
	      controlHistos.fill2DHisto("redmetcomps", ctf,rmetL,rmetT,weight,true);
	    }
	}
      /*
      std::vector<TString> cuts;
      cuts.push_back("");
      bool passSum(rmetL+zpt>50);
      bool passZeta(fabs(zeta)<2.5);
      bool passCut1(passSum && passZeta);
      if(passSum) cuts.push_back("cut1");
      if(passZeta) cuts.push_back("cut2");
      if(passCut1) cuts.push_back("cut12");
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  if(passCut1) controlHistos.fillHisto("cutflow",cats[ic],2,weight);
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    for(std::vector<TString>::iterator cIt=cuts.begin(); cIt!= cuts.end(); cIt++)
	      {
		TString ctf=cats[ic]+subCats[isc]+*cIt;
		controlHistos.fill2DHisto("zptvszeta",ctf,zpt,zeta,weight);
		controlHistos.fillHisto("zpt",ctf,zpt,weight);
		controlHistos.fillHisto("zeta",ctf,zeta,weight); 	    
		controlHistos.fillHisto("redmetLpzpt",ctf,rmetL+zpt,weight);
		controlHistos.fillHisto("redmetL0",ctf,rmetL,weight);
	      }
	}

      bool passRmetL(passCut1 && rmetL>0);
      bool passRmetT(passCut1 && rmetT>0);
      bool passCut2(passRmetL && passRmetT);
      cuts.clear();
      if(passCut1) cuts.push_back("");
      if(passRmetT) cuts.push_back("cut1");
      if(passRmetL) cuts.push_back("cut2");
      if(passCut2) cuts.push_back("cut12");     
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  if(passCut2) controlHistos.fillHisto("cutflow",cats[ic],3,weight);
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    for(std::vector<TString>::iterator cIt=cuts.begin(); cIt!= cuts.end(); cIt++)
	      {
		TString ctf=cats[ic]+subCats[isc]+*cIt;
		controlHistos.fillHisto("redmetL",ctf,rmetL,weight);
		controlHistos.fillHisto("redmetT",ctf,rmetT,weight);
		//		controlHistos.fillHisto("redmet",ctf,rmet,weight);
		controlHistos.fill2DHisto("redmetcomps",ctf,rmetL,rmetT,weight);	      	    
	      }
	}
      
      //bool passDphi(passCut2 && dphizrmet>2.0);
      //bool passDiff(passCut2 && );
      bool passCut3(passCut2 && rmetL-zpt>-50);
      cuts.clear();
      if(passCut2) cuts.push_back("");
      //      if(passDphi) cuts.push_back("cut1");
      //      if(passDiff) cuts.push_back("cut2");
      if(passCut3) cuts.push_back("cut12");
      
     for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
       {
	 if(passCut3) 
	   {
	     int jetbin(0);
	     if(jetsp4.size()==1) jetbin++;
	     if(jetsp4.size()>1)  jetbin++;
	     controlHistos.fillHisto("cutflow",cats[ic],4+jetbin,weight);
	   }
	 
	 for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	   for(std::vector<TString>::iterator cIt=cuts.begin(); cIt!= cuts.end(); cIt++)
	     {
	       TString ctf=cats[ic]+subCats[isc]+*cIt;
	       controlHistos.fillHisto("mt",ctf,transverseMass,weight);
	       if(jetsp4.size()>1) controlHistos.fillHisto("mjj",ctf,mjj,weight);
	       //controlHistos.fillHisto("dphizrmet",ctf,dphizrmet,weight);
	       controlHistos.fillHisto("redmetLmzpt",ctf,rmetL-zpt,weight);
	       //	     controlHistos.fill2DHisto("redmetLmzptvsdphizrmet",ctf,rmetL-zpt,dphizrmet,weight);
	       controlHistos.fill2DHisto("redmetLpzptvsmt",ctf,rmetL+zpt,transverseMass,weight);
	       controlHistos.fillHisto("met",ctf,jesMetNopuP.pt(),weight);
	       controlHistos.fillHisto("dphill",ctf,fabs(dphill),weight);
	       controlHistos.fill2DHisto("dphil2met",ctf,fabs(dphil2met[0]),fabs(dphil2met[1]),weight);
	       controlHistos.fill2DHisto("mtl2met",ctf,mtl2met[0],mtl2met[1],weight);
	       controlHistos.fillHisto("summtl2met",ctf,mtl2met[0]+mtl2met[1],weight);
	       controlHistos.fillHisto("ptasymm",ctf,ptasymm,weight);
	       controlHistos.fillHisto("ptlead",ctf,leptons[0].first.pt(),weight);
	       controlHistos.fillHisto("ptsublead",ctf,leptons[1].first.pt(),weight);
	       controlHistos.fillHisto("sumptlep",ctf,leptons[0].first.pt()+leptons[1].first.pt(),weight);
	     }
       }
     
      */     

     //	      controlHistos.fill2DHisto("sumjetvsmetL",ctf,sumjetL,calometL,weight);	      
     //      controlHistos.fill2DHisto("sumjetvsmetT",ctf,sumjetT,calometT,weight);	      
     //      controlHistos.fillHisto("redmetLType",ctf,recoilL,weight);
     //      controlHistos.fillHisto("redmetTType",ctf,recoilT,weight);
     
     //   controlHistos.fill2DHisto("zptvsdphizrmet",ctf,zpt,dphizrmet,weight);
     // 	      controlHistos.fill2DHisto("zptvsredmet",ctf,zpt,rmet,weight);
	    
     // 	      controlHistos.fillHisto("dphizredmet",ctf,dphizrmet,weight);
     // 	      controlHistos.fill2DHisto("redmetpzptvsdphizz",ctf,rmetL+zpt,dphizrmet,weight);
     
     // 	    controlHistos.fill2DHisto("redmetLmzptvsredmetpzpt",ctf,rmetL-zpt,rmetL+zpt,weight);	    
     // 	      controlHistos.fillHisto("redmetLmzpt_h"+zetabin,ctf,rmetL-zpt,weight);
     // 	      controlHistos.fillHisto("redmetpzpt_h"+zetabin,ctf,rmetL+zpt,weight);
     // 	      controlHistos.fillHisto("dphizrmet_h"+zetabin,ctf,dphizrmet,weight);
     // 	      controlHistos.fillHisto("redmetLmzpt_p"+zptbin,ctf,rmetL-zpt,weight);
     // 	      controlHistos.fillHisto("redmetpzpt_p"+zptbin,ctf,rmetL+zpt,weight);
	    // 	      controlHistos.fillHisto("dphizrmet_p"+zptbin,ctf,dphizrmet,weight);
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
