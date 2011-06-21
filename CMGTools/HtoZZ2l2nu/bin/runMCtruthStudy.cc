#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
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
  bool useFitter = runProcess.getParameter<bool>("useFitter");
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //control Histos
  SelectionMonitor controlHistos;
  controlHistos.addHistogram(  new TH1D ("hpt", ";p_{T}^{H};Events", 100,0,400) );
  TString tiers[]={"gen","reco"};
  for(size_t itier=0; itier<2; itier++)
    {
      controlHistos.addHistogram(  new TH1D (tiers[itier]+"zmass", ";"+tiers[itier]+" M_{ll};Events", 100,71,111) );
      controlHistos.addHistogram(  new TH1D (tiers[itier]+"zpt", ";"+tiers[itier]+" p_{T}^{ll};Events", 100,0,400) );
      controlHistos.addHistogram(  new TH1D (tiers[itier]+"zvvpt", ";"+tiers[itier]+" p_{T}^{#nu#nu};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"dphizz", ";"+tiers[itier]+" #Delta#phi(ll,E_{T}^{miss});Events",100,-3.2,3.2) );
      //controlHistos.addHistogram( new TH1D (tiers[itier]+"jetpt", ";"+tiers[itier]+" p_{T}^{jet};Jets",100,0,100) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"njets", ";"+tiers[itier]+" Jets;Jets",4,0,4) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"mt", ";"+tiers[itier]+" M_{T};Events", 100, 100.,500.) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"summtl2met", ";"+tiers[itier]+" M_{T}(l^{i},MET)+M_{T}(l^{i},MET);Events",100,0,1000) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"dphizbisector", ";"+tiers[itier]+" #Delta#phi(Z,bisector);Events",100,-3.2,3.2) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"dphimetbisector", ";"+tiers[itier]+" #Delta#phi(MET,bisector);Events",100,-3.2,3.2) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"dphihbisector", ";"+tiers[itier]+" #Delta#phi(H^{0},bisector);Events",100,-3.2,3.2) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetLminrecoil", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel} (min. recoil);Events", 100, -200.,200.) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetTminrecoil", ";"+tiers[itier]+" red-E_{T}^{miss,#perp} (min. recoil);Events", 100, -200.,200.) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetL", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};Events", 100, -200.,200.) );
      controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetT", ";"+tiers[itier]+" red-E_{T}^{miss,#perp};Events", 100, -200.,200.) );
      controlHistos.addHistogram( (TH1D *)(new TH2D (tiers[itier]+"redmetcomps", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
      controlHistos.addHistogram( (TH1D *)(new TH2D (tiers[itier]+"redmetcompsminrecoil", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel} (min recoil);red-E_{T}^{miss,#perp} (min recoil);Events", 100, -251.,249,100, -251.,249.) ) );
    }
  //  controlHistos.addHistogram(  new TH1D ("deltamzz", ";M_{Z_{ll}}-M_{Z_{#nu#nu}};Events", 100,-100,100) );
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
	{
	  controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]);
	  controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]+"catalpha");
	  controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]+"catbeta");
	}
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
      float weight=ev.weight;

      //get the physics object
      PhysicsObjectCollection vtx,leptons, jets, mets;
      LorentzVectorCollection jetsp4;
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
	      jets.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    case 11: case -11: case 13: case -13:
	      leptons.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    case 500:
	      vtx.push_back( PhysicsObject(p4,p4.pt()) );
	      break;
	    }
	}

      //similar for the mc truth
      LorentzVectorCollection genhiggs,genleptons, genjets;
      LorentzVector genmet(0,0,0,0);
      for(Int_t ipart=0; ipart<ev.nmcparticles; ipart++)
	{
	  LorentzVector p4(ev.mcpx[ipart],ev.mcpy[ipart],ev.mcpz[ipart],ev.mcen[ipart]);
	  switch( ev.mcid[ipart] )
	    {
	    case 12: case -12: case 14: case -14: case 16: case -16:
	      genmet += p4;
	      break;
	    case 1:
	      genjets.push_back(p4);
	      break;
	    case 11: case -11: case 13: case -13: case 15: case -15:
	      genleptons.push_back( p4 );
	      break;
	    case 25:
	      genhiggs.push_back( p4 );
	      break;
	    }
	}
      if(genhiggs.size()==0) continue;
      
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

      //the higgs
      LorentzVector higgs = genhiggs[0];

      //z kinematics
      LorentzVector zll=leptons[0].first+leptons[1].first;
      LorentzVector genzll=genleptons[0]+genleptons[1];
      LorentzVector zvv=mets[0].first;
      LorentzVector genzvv=genmet;

      //transverse mass
      LorentzVector transvSum=zll+zvv;
      float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(zll.pt(),2)+pow(zll.mass(),2))+TMath::Sqrt(TMath::Power(zvv.pt(),2)+pow(zll.mass(),2)),2);
      transverseMass-=TMath::Power(transvSum.pt(),2);
      transverseMass=TMath::Sqrt(transverseMass);
      
      LorentzVector gentransvSum=genzll+genzvv;
      float gentransverseMass=TMath::Power(TMath::Sqrt(TMath::Power(genzll.pt(),2)+pow(genzll.mass(),2))+TMath::Sqrt(TMath::Power(genzvv.pt(),2)+pow(genzll.mass(),2)),2);
      gentransverseMass-=TMath::Power(gentransvSum.pt(),2);
      gentransverseMass=TMath::Sqrt(gentransverseMass);

      float dphil2met[]={ fabs(deltaPhi(zvv.phi(),leptons[0].first.phi())), fabs(deltaPhi(zvv.phi(),leptons[1].first.phi())) };
      float mtl2met[]={ TMath::Sqrt(2*zvv.pt()*leptons[0].first.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*zvv.pt()*leptons[1].first.pt()*(1-TMath::Cos(dphil2met[1]))) };

      float gendphil2met[]={ fabs(deltaPhi(genzvv.phi(),genleptons[0].phi())), fabs(deltaPhi(genzvv.phi(),genleptons[1].phi())) };
      float genmtl2met[]={ TMath::Sqrt(2*genzvv.pt()*genleptons[0].pt()*(1-TMath::Cos(gendphil2met[0]))) ,   TMath::Sqrt(2*zvv.pt()*genleptons[1].pt()*(1-TMath::Cos(gendphil2met[1]))) };

      //redmet
      rmetComp.compute(leptons[0].first,leptons[0].second, leptons[1].first,leptons[1].second, jetsp4, mets[0].first);
      double rmetTminrecoil = rmetComp.reducedMETComponents().first;
      double rmetLminrecoil = rmetComp.reducedMETComponents().second;
      double rmetT = rmetComp.reducedMETComponents(ReducedMETComputer::D0).first;
      double rmetL = rmetComp.reducedMETComponents(ReducedMETComputer::D0).second;      
      
      rmetComp.compute(genleptons[0], 0., genleptons[1], 0., genjets, zvv);
      double genrmetTminrecoil = rmetComp.reducedMETComponents().first;
      double genrmetLminrecoil = rmetComp.reducedMETComponents().second;
      double genrmetT = rmetComp.reducedMETComponents(ReducedMETComputer::D0).first;
      double genrmetL = rmetComp.reducedMETComponents(ReducedMETComputer::D0).second;      

      //bisector
      TVector2 bisector = rmetComp.defineThrust(leptons[0].first,0,leptons[1].first,0).second;
      double dphill=fabs(deltaPhi(leptons[0].first.phi(),leptons[1].first.phi()));
      TVector2 redZll(leptons[0].first.px()+leptons[1].first.px(),leptons[0].first.py()+leptons[1].first.py());
      TVector2 redZvv(zvv.px(),zvv.py());
      double zllptL=redZll*bisector;
      double zvvptL=redZvv*bisector;
      
      TVector2 genbisector = rmetComp.defineThrust(genleptons[0],0,genleptons[1],0).second;
      TVector2 genredZll(genleptons[0].px()+genleptons[1].px(),genleptons[0].py()+genleptons[1].py());
      TVector2 genredZvv(genzvv.px(),genzvv.py());
      double genzllptL=genredZll*bisector;
      double genzvvptL=genredZvv*bisector;

      //subcategory analysis
      TString subTopcat( (fabs(dphill) > TMath::Pi()/2) ? "catalpha":"catbeta" );
      TString topCats[]={"", subTopcat};
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	  {
	    for(size_t itc=0; itc<sizeof(topCats)/sizeof(TString); itc++)
	      {
		TString ctf=cats[ic]+subCats[isc]+topCats[itc];
		controlHistos.fillHisto("hpt",ctf, higgs.pt(),weight);  
		controlHistos.fillHisto("recozpt",ctf, zll.pt(),weight);  
		controlHistos.fillHisto("recozmass",ctf, zll.mass(),weight);  
		controlHistos.fillHisto("recozvvpt",ctf, zvv.pt(),weight);  
		controlHistos.fillHisto("recodphizz",ctf, deltaPhi(zll.phi(),zvv.phi()),weight);  
		controlHistos.fillHisto("genzpt",ctf, genzll.pt(),weight);  
		controlHistos.fillHisto("genzmass",ctf, genzll.mass(),weight);  
		controlHistos.fillHisto("genzvvpt",ctf, genzvv.pt(),weight);  
		controlHistos.fillHisto("gendphizz",ctf, deltaPhi(genzll.phi(),genzvv.phi()),weight);  
		controlHistos.fillHisto("deltazpt",ctf, zll.pt()-genzll.pt(),weight);  
		controlHistos.fillHisto("deltazvvpt",ctf, zvv.pt()-genzvv.pt(),weight);  
		controlHistos.fillHisto("deltazptL",ctf, zllptL-genzllptL,weight);  
		controlHistos.fillHisto("deltazvvptL",ctf, zvvptL-genzvvptL,weight);  
		controlHistos.fillHisto("deltanjets",ctf, jetsp4.size()-genjets.size(),weight);
		controlHistos.fillHisto("gennjets",ctf, genjets.size(),weight);    
		controlHistos.fillHisto("reconjets",ctf, jetsp4.size(),weight);      
		controlHistos.fillHisto("recomt",ctf, transverseMass,weight);      
		controlHistos.fillHisto("genmt",ctf, gentransverseMass,weight);      
		controlHistos.fillHisto("deltamt",ctf, transverseMass-gentransverseMass,weight);      
		
		controlHistos.fillHisto("recosummtl2met",ctf,mtl2met[0]+mtl2met[1],weight);
		controlHistos.fillHisto("gensummtl2met",ctf,genmtl2met[0]+genmtl2met[1],weight);
		controlHistos.fillHisto("deltasummtl2met",ctf,(mtl2met[0]+mtl2met[1])-(genmtl2met[0]+genmtl2met[1]),weight);	    
		
		controlHistos.fillHisto("recodphizbisector",ctf,deltaPhi(zll.phi(),bisector.Phi()),weight);
		controlHistos.fillHisto("recodphimetbisector",ctf,deltaPhi(zvv.phi(),bisector.Phi()),weight);
		controlHistos.fillHisto("recodphihbisector",ctf,deltaPhi(higgs.phi(),bisector.Phi()),weight);
		controlHistos.fillHisto("gendphizbisector",ctf,deltaPhi(genzll.phi(),genbisector.Phi()),weight);
		controlHistos.fillHisto("gendphimetbisector",ctf,deltaPhi(genzvv.phi(),genbisector.Phi()),weight);
		controlHistos.fillHisto("gendphihbisector",ctf,deltaPhi(higgs.phi(),genbisector.Phi()),weight);
		
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
    }
  
  
  //       LorentzVector jesMetNopuP=mets[0].first;
  //       float rawMET = mets[0].second;
  //       LorentzVector thrust=leptons[0].first-leptons[1].first;
  //       float ptasymm=thrust.pt()/dileptonP.pt();
  
  //       //transverse masses                                               
  
  //       float dphill=deltaPhi(leptons[0].first.phi(), leptons[1].first.phi() );
  //       float drll = deltaR(leptons[0].first.eta(),leptons[0].first.phi(),leptons[1].first.eta(),leptons[1].first.phi());
  
  //       //z kinematics
  //       double zpt = dileptonP.pt();
  //       double zeta = dileptonP.eta();
  
  //       //met
  //       TVector2 redjesMetNopuP(jesMetNopuP.px(),jesMetNopuP.py());
  //       double dphizz = reddil.DeltaPhi(redjesMetNopuP);
  
  //       //st,mht
  //       double st=jesMetNopuP.pt()+leptons[0].first.pt()+leptons[1].first.pt();
  
  //       //projected met
  //       double projmet = pmetComp.compute(leptons[0].first,leptons[1].first,mets[0].first);
     
  //       double rmet=rmetComp.reducedMET();
  //       double sumjetT = rmetComp.sumJetProjComponents().first;
  //       double sumjetL = rmetComp.sumJetProjComponents().second;

  //       double dilT = rmetComp.dileptonProjComponents().first;
  //       double dilL = rmetComp.dileptonProjComponents().second;
      
  //       double metT = rmetComp.metProjComponents().first;
  //       double metL = rmetComp.metProjComponents().second;
      
  //       double unclT = rmetComp.unclProjComponents().first;
  //       double unclL = rmetComp.unclProjComponents().second;

  //       int redevCat = rmetComp.getEventCategory();
  //       double sumjetpdilL=sumjetL+dilL;
  //       double sumjetpdilT=sumjetT+dilT;
  //       double sumjetpdil=sqrt(pow(sumjetpdilL,2)+pow(sumjetpdilT,2));

  //       double rmetcut(ev.cat==dilepton::MUMU ? 35: 30 );
  //       double sumjetpdilLcut(ev.cat==dilepton::MUMU ? -60 : -55);
  //       bool passrmet(( (redevCat!=ReducedMETComputer::DILMETPJETSLIKE && rmet>rmetcut)
  // 		      || (redevCat==ReducedMETComputer::DILMETPJETSLIKE && sumjetpdilL<sumjetpdilL) ) );
  //       bool passbveto(passrmet && fabs(zeta)<2.5);// && nbjets<1);
      
  //       //fill control histograms
  //       for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
  // 	{
  // 	  //cutflow histogram
  // 	  controlHistos.fillHisto("cutflow",cats[ic],0,weight);
  // 	  if(passrmet)  
  // 	    controlHistos.fillHisto("cutflow",cats[ic],1,weight);
  // 	  if(passbveto)
  // 	    {
  // 	      controlHistos.fillHisto("cutflow",cats[ic],2,weight); 
  // 	      int jetbin(0);
  // 	      if(jetsp4.size()==1) jetbin++;
  // 	      if(jetsp4.size()>1)  jetbin++;
  // 	      controlHistos.fillHisto("cutflow",cats[ic],3+jetbin,weight);
  // 	    }

  // 	  //subcategory analysis
  // 	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
  // 	    {
  // 	      TString ctf=cats[ic]+subCats[isc];
	      
  // 	      controlHistos.fillHisto("evcat",ctf, redevCat ,weight);
  // 	      controlHistos.fillHisto("zpt",ctf, zpt,weight);
  // 	      controlHistos.fillHisto("zeta",ctf, zeta,weight);
  // 	      controlHistos.fill2DHisto("zptvszeta", ctf,zpt,zeta,weight);
  // 	      controlHistos.fill2DHisto("zptvsmet", ctf, zpt, jesMetNopuP.pt(),weight);
  // 	      controlHistos.fill2DHisto("zetavsmet", ctf, zeta, jesMetNopuP.pt(),weight);
  // 	      controlHistos.fillHisto("mjj",ctf,mjj,weight);	    
  // 	      controlHistos.fillHisto("met", ctf,jesMetNopuP.pt(),weight);
  // 	      controlHistos.fillHisto("redmet", ctf,rmet,weight);	      
  // 	      if(passrmet)
  // 		{
  // 		  controlHistos.fill2DHisto("mtl2met",ctf,mtl2met[0],mtl2met[1],weight);
  // 		  controlHistos.fillHisto("summtl2met",ctf,mtl2met[0]+mtl2met[1],weight);
  // 		  controlHistos.fillHisto("minmtl2met",ctf,min(mtl2met[0],mtl2met[1]),weight);
  // 		  controlHistos.fillHisto("ptlead",ctf,leptons[0].first.pt(),weight);
  // 		  controlHistos.fillHisto("ptsublead",ctf,leptons[1].first.pt(),weight);
  // 		  controlHistos.fillHisto("sumptlep",ctf,leptons[0].first.pt()+leptons[1].first.pt(),weight);	    
  // 		  controlHistos.fillHisto("mt",ctf,transverseMass,weight);
  // 		  controlHistos.fillHisto("dphill",ctf,dphill,weight);
  // 		  controlHistos.fillHisto("dphizz",ctf,dphizz,weight);
  // 		  controlHistos.fillHisto("ptasymm",ctf,ptasymm,weight);	    
  // 		  controlHistos.fillHisto("drll",ctf,drll,weight);
  // 		  for(size_t ijet=0; ijet<jetsp4.size(); ijet++)
  // 		    {
  // 		      for(size_t ilep=0; ilep<2; ilep++)
  // 			{
  // 			  LorentzVector lj=leptons[ilep].first+jetsp4[ijet];
  // 			  double mlj=lj.mass();
  // 			  controlHistos.fillHisto("mlj",ctf,mlj,weight);
  // 			}
  // 		    }
  // 		}
	      

  // 	      //event categories
  // 	      if(redevCat!=ReducedMETComputer::DILMETPJETSLIKE)
  // 		{
  // 		  controlHistos.fillHisto("projmetcat0", ctf,projmet,weight);
  // 		  controlHistos.fillHisto("metcat0", ctf,jesMetNopuP.pt(),weight);
  // 		  controlHistos.fillHisto("redmetcat0", ctf,rmet,weight);	      
  // 		  controlHistos.fill2DHisto("redmetvszeta",ctf,rmet,fabs(zeta),weight);
  // 		  controlHistos.fill2DHisto("redmetvszpt",ctf,rmet,zpt,weight);
  // 		}
  // 	      else
  // 		{
  // 		  controlHistos.fill2DHisto("jetrecoilvsunclusteredL",ctf,sumjetL,unclL,weight);
  // 		  controlHistos.fill2DHisto("jetrecoilvsunclusteredT",ctf,sumjetT,unclT,weight);
  // 		  controlHistos.fillHisto("sumjetpdilL",ctf,sumjetpdilL,weight);
  // 		  controlHistos.fillHisto("sumjetpdilT",ctf,sumjetpdilT,weight);
  // 		  controlHistos.fillHisto("sumjetpdil",ctf,sumjetpdil,weight);
  // 		  controlHistos.fillHisto("sumjetpdilcomps",ctf,sumjetpdilL,sumjetpdilT,weight);
  // 		  controlHistos.fillHisto("metcat1", ctf,jesMetNopuP.pt(),weight);
  // 		  controlHistos.fillHisto("projmetcat1", ctf,projmet,weight);
  // 		  controlHistos.fillHisto("htcat1", ctf,ht,weight);
  // 		  controlHistos.fillHisto("stcat1", ctf,st,weight);
  // 		  controlHistos.fillHisto("vtxpcat1", ctf,vtxp,weight);
  // 		  controlHistos.fillHisto("dphivtxmetcat1",ctf,dphivtxmet,weight);
  // 		  controlHistos.fillHisto("dphillcat1",ctf,dphill,weight);

  // 		  if(useFitter)
  // 		    {
  // 		      rmetFitter.compute(leptons[0].first,leptons[0].second,
  // 					 leptons[1].first,leptons[1].second,
  // 					 jetsp4,
  // 					 rmetComp.a_t,rmetComp.a_l);
		  
  // 		      double rmetfit=rmetFitter.reducedMET().first;
  // 		      double rmetfiterr=rmetFitter.reducedMET().second;
  // 		      double rmetfitsig=rmetfiterr/rmetfit;
		      
  // 		      double rmetfitL=-rmetFitter.reducedMET_long().first;
  // 		      double rmetfitLerr=rmetFitter.reducedMET_long().second;
  // 		      double rmetfitLsig=rmetfitLerr/rmetfitL;
		      
  // 		      double rmetfitT=-rmetFitter.reducedMET_perp().first;
  // 		      double rmetfitTerr=rmetFitter.reducedMET_perp().second;
  // 		      double rmetfitTsig=rmetfitTerr/rmetfitT;
		      
  // 		      controlHistos.fillHisto("rmetfit",ctf,rmetfit,weight);
  // 		      controlHistos.fillHisto("rmetfitsig",ctf,rmetfitsig,weight);
  // 		      controlHistos.fillHisto("rmetfitL",ctf,rmetfitL,weight);
  // 		      controlHistos.fillHisto("rmetfitLsig",ctf,rmetfitLsig,weight);
  // 		      controlHistos.fillHisto("rmetfitT",ctf,rmetfitT,weight);
  // 		      controlHistos.fillHisto("rmetfitTsig",ctf,rmetfitTsig,weight);
  // 		    }
  // 		}
  // 	    }


  
  
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
