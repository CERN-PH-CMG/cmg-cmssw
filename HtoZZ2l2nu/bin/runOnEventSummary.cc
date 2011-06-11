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

  controlHistos.addHistogram( new TH1F ("redmet", ";red-E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("redmetcat0", ";red-E_{T}^{miss};Events", 100,0,500) );

  controlHistos.addHistogram( new TH1F ("met", ";type-I E_{T}^{miss};Events", 100,0,500) );  
  controlHistos.addHistogram( new TH1F ("metcat0", ";type-I E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("metcat1", ";type-I E_{T}^{miss};Events", 100,0,500) );
  
  controlHistos.addHistogram( new TH1F ("htcat1", ";H_{T};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("stcat1", ";S_{T};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("vtxpcat1", ";p_{T}^{vertex};Events", 100,0,500) );

  controlHistos.addHistogram( new TH1F ("projmet", ";projected E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("projmetcat0", ";projected E_{T}^{miss};Events", 100,0,500) );
  controlHistos.addHistogram( new TH1F ("projmetcat1", ";projected E_{T}^{miss};Events", 100,0,500) );
 
  controlHistos.addHistogram(  new TH1D ("zpt", ";p_{T}^{ll};Events", 100,0,400) );
  controlHistos.addHistogram(  new TH1D ("zeta", ";#eta^{ll};Events", 100,-5,5) );
  controlHistos.addHistogram( (TH1D *)( new TH2D ("zptvszeta", ";p_{T}^{ll};#eta^{ll}", 100,0,400,100,-5,5) ) );
  controlHistos.addHistogram( (TH1D *)( new TH2D ("zptvsmet", ";p_{T}^{ll};E_{T}^{miss}", 100,0,400,100,0,400) ) );
  controlHistos.addHistogram( (TH1D *)( new TH2D ("zetavsmet", ";#eta^{ll};E_{T}^{miss}", 100,0,400,100,0,400) ) );
  controlHistos.addHistogram( new TH1F ("redmetL", ";red-E_{T}^{miss,#parallel};Events", 100, -1.,249.) );
  controlHistos.addHistogram( new TH1F ("redmetT", ";red-E_{T}^{miss,#perp};Events", 100, -1.,249.) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetcomps", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetvszpt", ";red-E_{T}^{miss};p_{T}(Z);Events", 100, -50.,250,100, -50.,250) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redmetvszeta", ";red-E_{T}^{miss};#eta(Z);Events", 100, -50.,250,100,-0.5,5.) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("jetrecoilvsunclusteredL", ";a^{clustered}_{#parallel}; a^{unclustered}_{#parallel};Events", 100, -250,250,100, -250.,250) ) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("jetrecoilvsunclusteredT", ";a^{clustered}_{#perp}; a^{unclustered}_{#perp};Events", 100, -250,250,100, -250.,250) ) );
  controlHistos.addHistogram( new TH1D ("dphill", ";#Delta#phi(l_{1},l_{2});Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1D ("dphillcat1", ";#Delta#phi(l_{1},l_{2});Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1D ("dphivtxmetcat1", ";#Delta#phi(vertex,MET);Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1D ("drll", ";#DeltaR(l_{1},l_{2});Events",100,0,6.) );
 controlHistos.addHistogram( new TH1D ("dphizz", ";#Delta#phi(ll,E_{T}^{miss});Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1F ("mjj", ";M_{jj};Events", 100, 0.,500.) );
  controlHistos.addHistogram( new TH1F ("mlj", ";M_{lj};Events", 100, 0.,500.) );
  controlHistos.addHistogram( new TH1F ("sumjetpdilL", ";a^{clustered}_{#parallel}+p^{ll}_{T,#parallel};Events",100,-250,250) );
  controlHistos.addHistogram( new TH1F ("sumjetpdilT", ";a^{clustered}_{#perp}+p^{ll}_{T,#perp};Events",100,-250,250) );
  controlHistos.addHistogram( new TH1F ("sumjetpdil", ";a^{clustered}+p^{ll};Events",100,0,250) );

  controlHistos.addHistogram( (TH1D *)(new TH2D ("sumjetpdilcomps", ";a^{clustered}_{#parallel}+p^{ll}_{T,#parallel};a^{clustered}_{#perp}+p^{ll}_{T,#perp};Events", 100, -251.,249,100, -251.,249.) ) );

  if(useFitter)
    {
      controlHistos.addHistogram( new TH1F ("rmetfit", ";a^{clustered}+p_{T}^{ll};Events",100,-250,250) );
      controlHistos.addHistogram( new TH1F ("rmetfitsig", ";Significance of a^{clustered}+p_{T}^{ll};Events",100,0,50) );
      controlHistos.addHistogram( new TH1F ("rmetfitL", ";a^{clustered}_{L}+p_{T,L}^{ll};Events",100,-250,250) );
      controlHistos.addHistogram( new TH1F ("rmetfitLsig", ";Significance of a^{clustered}_{L}+p_{T,L}^{ll};Events",100,-26,24) );
      controlHistos.addHistogram( new TH1F ("rmetfitT", ";a^{clustered}_{T}+p_{T,T}^{ll};Events",100,-250,250) );
      controlHistos.addHistogram( new TH1F ("rmetfitTsig", ";Significance of a^{clustered}_{T}+p_{T,T}^{ll};Events",100,-26,24) );
    }

  controlHistos.addHistogram( new TH1F ("mt", ";M_{T};Events", 100, 100.,500.) );
  controlHistos.addHistogram( new TH1D ("ptasymm", ";[p_{T}(1)-p_{T}(2)]/[p_{T}(1)+p_{T}(2)];Events",100,0,1.05) );
  controlHistos.addHistogram( new TH1D ("ptlead", ";leading lepton p_{T};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("ptsublead", ";next-leading lepton p_{T};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("sumptlep", ";#Sigma p_{T} (leptons);Events",100,0,500) );
  controlHistos.addHistogram( (TH1D *)(new TH2D ("mtl2met", ";M_{T}(l_{1},MET);M_{T}(l_{2},MET);Events",100,0,250,100,0,250) ) );
  controlHistos.addHistogram( new TH1D ("summtl2met", ";#Sigma M_{T}(l_{1},MET)+M_{T}(l_{2},MET);Events",100,0,1000) );
  controlHistos.addHistogram( new TH1D ("minmtl2met", ";min{M_{T}(l_{1},MET),M_{T}(l_{2},MET)};Events",100,0,250) );
  controlHistos.addHistogram( new TH1D ("evcat", ";Event category;Events",2,0,2) );

  TString steps[]={"|M_{ll}-M_{Z}|<15 GeV/c^{2}","red-MET","b-veto","=0 jets","=1 jet","#geq 2 jets"};
  int nsteps=sizeof(steps)/sizeof(TString);
  TH1D *cutflow=new TH1D ("cutflow", ";Event selection;Events",nsteps,0,nsteps);
  for(int istep=0; istep<nsteps; istep++)
    {
      cutflow->GetXaxis()->SetBinLabel(istep+1,steps[istep]);
    }
  controlHistos.addHistogram( cutflow );
  
  //replicate monitor for categories
  TString cats[]={"ee","emu","mumu"};
  TString subcats[]={"","eq0jets","eq1jets","geq2jets"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(size_t isubcat=0;isubcat<sizeof(subcats)/sizeof(TString); isubcat++)
	{
	  controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]);
	}
    }
  
   
  //Start the reduced met fitter/computer
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
  ReducedMETFitter rmetFitter(runProcess);
  ProjectedMETComputer pmetComp;

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
      double ht(0);
      int nbjets(0);
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
	      ht += p4.pt();
	      jets.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      if(p4.pt()>25) nbjets += (ev.info1[ipart]>1.7);
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

      double vtxp=vtx[0].first.pt();
      double dphivtxmet=deltaPhi(vtx[0].first.phi(),mets[0].first.phi());

      //dijet invariant mass
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
      LorentzVector thrust=leptons[0].first-leptons[1].first;
      float ptasymm=thrust.pt()/dileptonP.pt();

      //transverse masses                                               
      float dphil2met[]={ fabs(deltaPhi(jesMetNopuP.phi(),leptons[0].first.phi())), fabs(deltaPhi(jesMetNopuP.phi(),leptons[1].first.phi())) };
      float mtl2met[]={ TMath::Sqrt(2*jesMetNopuP.pt()*leptons[0].first.pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*jesMetNopuP.pt()*leptons[1].first.pt()*(1-TMath::Cos(dphil2met[1]))) };
      float dphill=deltaPhi(leptons[0].first.phi(), leptons[1].first.phi() );
      float drll = deltaR(leptons[0].first.eta(),leptons[0].first.phi(),leptons[1].first.eta(),leptons[1].first.phi());
   
      //z kinematics
      double zpt = dileptonP.pt();
      double zeta = dileptonP.eta();

      //met
      TVector2 redjesMetNopuP(jesMetNopuP.px(),jesMetNopuP.py());
      double dphizz = reddil.DeltaPhi(redjesMetNopuP);

      //st,mht
      double st=jesMetNopuP.pt()+leptons[0].first.pt()+leptons[1].first.pt();

      //projected met
      double projmet = pmetComp.compute(leptons[0].first,leptons[1].first,mets[0].first);
      
      //redmet
      rmetComp.compute(leptons[0].first,leptons[0].second,
		       leptons[1].first,leptons[1].second,
		       jetsp4,
		       mets[0].first);

      double rmet=rmetComp.reducedMET();
      double rmetT = rmetComp.reducedMETComponents().first;
      double rmetL = rmetComp.reducedMETComponents().second;
      
      double sumjetT = rmetComp.sumJetProjComponents().first;
      double sumjetL = rmetComp.sumJetProjComponents().second;

      double dilT = rmetComp.dileptonProjComponents().first;
      double dilL = rmetComp.dileptonProjComponents().second;
      
      double metT = rmetComp.metProjComponents().first;
      double metL = rmetComp.metProjComponents().second;
      
      double unclT = rmetComp.unclProjComponents().first;
      double unclL = rmetComp.unclProjComponents().second;

      int redevCat = rmetComp.getEventCategory();
      double sumjetpdilL=sumjetL+dilL;
      double sumjetpdilT=sumjetT+dilT;
      double sumjetpdil=sqrt(pow(sumjetpdilL,2)+pow(sumjetpdilT,2));

      double rmetcut(ev.cat==dilepton::MUMU ? 35: 30 );
      double sumjetpdilLcut(ev.cat==dilepton::MUMU ? -60 : -55);
      bool passrmet(( (redevCat!=ReducedMETComputer::DILMETPJETSLIKE && rmet>rmetcut)
		      || (redevCat==ReducedMETComputer::DILMETPJETSLIKE && sumjetpdilL<sumjetpdilL) ) );
      bool passbveto(passrmet && fabs(zeta)<2.5);// && nbjets<1);
      
      //fill control histograms
      TString cats[]={"all",evcat};
      TString subCats[]={"",subcat};
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  //cutflow histogram
	  controlHistos.fillHisto("cutflow",cats[ic],0,weight);
	  if(passrmet)  
	    controlHistos.fillHisto("cutflow",cats[ic],1,weight);
	  if(passbveto)
	    {
	      controlHistos.fillHisto("cutflow",cats[ic],2,weight); 
	      int jetbin(0);
	      if(jetsp4.size()==1) jetbin++;
	      if(jetsp4.size()>1)  jetbin++;
	      controlHistos.fillHisto("cutflow",cats[ic],3+jetbin,weight);
	    }

	  //subcategory analysis
	  for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	    {
	      TString ctf=cats[ic]+subCats[isc];
	      
	      controlHistos.fillHisto("evcat",ctf, redevCat ,weight);
	      controlHistos.fillHisto("zpt",ctf, zpt,weight);
	      controlHistos.fillHisto("zeta",ctf, zeta,weight);
	      controlHistos.fill2DHisto("zptvszeta", ctf,zpt,zeta,weight);
	      controlHistos.fill2DHisto("zptvsmet", ctf, zpt, jesMetNopuP.pt(),weight);
	      controlHistos.fill2DHisto("zetavsmet", ctf, zeta, jesMetNopuP.pt(),weight);
	      controlHistos.fillHisto("mjj",ctf,mjj,weight);	    
	      controlHistos.fillHisto("met", ctf,jesMetNopuP.pt(),weight);
	      controlHistos.fillHisto("redmet", ctf,rmet,weight);	      
	      if(passrmet)
		{
		  controlHistos.fill2DHisto("mtl2met",ctf,mtl2met[0],mtl2met[1],weight);
		  controlHistos.fillHisto("summtl2met",ctf,mtl2met[0]+mtl2met[1],weight);
		  controlHistos.fillHisto("minmtl2met",ctf,min(mtl2met[0],mtl2met[1]),weight);
		  controlHistos.fillHisto("ptlead",ctf,leptons[0].first.pt(),weight);
		  controlHistos.fillHisto("ptsublead",ctf,leptons[1].first.pt(),weight);
		  controlHistos.fillHisto("sumptlep",ctf,leptons[0].first.pt()+leptons[1].first.pt(),weight);	    
		  controlHistos.fillHisto("mt",ctf,transverseMass,weight);
		  controlHistos.fillHisto("dphill",ctf,dphill,weight);
		  controlHistos.fillHisto("dphizz",ctf,dphizz,weight);
		  controlHistos.fillHisto("ptasymm",ctf,ptasymm,weight);	    
		  controlHistos.fillHisto("drll",ctf,drll,weight);
		  for(size_t ijet=0; ijet<jetsp4.size(); ijet++)
		    {
		      for(size_t ilep=0; ilep<2; ilep++)
			{
			  LorentzVector lj=leptons[ilep].first+jetsp4[ijet];
			  double mlj=lj.mass();
			  controlHistos.fillHisto("mlj",ctf,mlj,weight);
			}
		    }
		}
	      

	      //event categories
	      if(redevCat!=ReducedMETComputer::DILMETPJETSLIKE)
		{
		  controlHistos.fillHisto("projmetcat0", ctf,projmet,weight);
		  controlHistos.fillHisto("metcat0", ctf,jesMetNopuP.pt(),weight);
		  controlHistos.fillHisto("redmetcat0", ctf,rmet,weight);	      
		  controlHistos.fill2DHisto("redmetcomps", ctf,rmetL,rmetT,weight);
		  controlHistos.fillHisto("redmetL", ctf,rmetL,weight);
		  controlHistos.fillHisto("redmetT", ctf,rmetT,weight);	
		  controlHistos.fill2DHisto("redmetvszeta",ctf,rmet,fabs(zeta),weight);
		  controlHistos.fill2DHisto("redmetvszpt",ctf,rmet,zpt,weight);
		}
	      else
		{
		  controlHistos.fill2DHisto("jetrecoilvsunclusteredL",ctf,sumjetL,unclL,weight);
		  controlHistos.fill2DHisto("jetrecoilvsunclusteredT",ctf,sumjetT,unclT,weight);
		  controlHistos.fillHisto("sumjetpdilL",ctf,sumjetpdilL,weight);
		  controlHistos.fillHisto("sumjetpdilT",ctf,sumjetpdilT,weight);
		  controlHistos.fillHisto("sumjetpdil",ctf,sumjetpdil,weight);
		  controlHistos.fillHisto("sumjetpdilcomps",ctf,sumjetpdilL,sumjetpdilT,weight);
		  controlHistos.fillHisto("metcat1", ctf,jesMetNopuP.pt(),weight);
		  controlHistos.fillHisto("projmetcat1", ctf,projmet,weight);
		  controlHistos.fillHisto("htcat1", ctf,ht,weight);
		  controlHistos.fillHisto("stcat1", ctf,st,weight);
		  controlHistos.fillHisto("vtxpcat1", ctf,vtxp,weight);
		  controlHistos.fillHisto("dphivtxmetcat1",ctf,dphivtxmet,weight);
		  controlHistos.fillHisto("dphillcat1",ctf,dphill,weight);

		  if(useFitter)
		    {
		      rmetFitter.compute(leptons[0].first,leptons[0].second,
					 leptons[1].first,leptons[1].second,
					 jetsp4,
					 rmetComp.a_t,rmetComp.a_l);
		  
		      double rmetfit=rmetFitter.reducedMET().first;
		      double rmetfiterr=rmetFitter.reducedMET().second;
		      double rmetfitsig=rmetfiterr/rmetfit;
		      
		      double rmetfitL=-rmetFitter.reducedMET_long().first;
		      double rmetfitLerr=rmetFitter.reducedMET_long().second;
		      double rmetfitLsig=rmetfitLerr/rmetfitL;
		      
		      double rmetfitT=-rmetFitter.reducedMET_perp().first;
		      double rmetfitTerr=rmetFitter.reducedMET_perp().second;
		      double rmetfitTsig=rmetfitTerr/rmetfitT;
		      
		      controlHistos.fillHisto("rmetfit",ctf,rmetfit,weight);
		      controlHistos.fillHisto("rmetfitsig",ctf,rmetfitsig,weight);
		      controlHistos.fillHisto("rmetfitL",ctf,rmetfitL,weight);
		      controlHistos.fillHisto("rmetfitLsig",ctf,rmetfitLsig,weight);
		      controlHistos.fillHisto("rmetfitT",ctf,rmetfitT,weight);
		      controlHistos.fillHisto("rmetfitTsig",ctf,rmetfitTsig,weight);
		    }
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
