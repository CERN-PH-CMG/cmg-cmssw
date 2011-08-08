#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/TransverseMassComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ProjectedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/JetEnergyUncertaintyComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

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
#include "TEventList.h"
 
using namespace std;

//
TH1D *getHistogramForVariable(TString variable)
{
  TH1D *h=0;
  if(variable=="dphill")         h = new TH1D( variable, ";#Delta#phi(l^{(1)},l^{(2)});Events",100,-3.2,3.2);
  if(variable=="detall")         h = new TH1D( variable, ";#Delta#eta(l^{(1)},l^{(2)});Events",100,-5,5);
  if(variable=="drll")           h = new TH1D( variable, ";#DeltaR(l^{(1)},l^{(2)});Events",100,0,6);
  if(variable=="mindrlz")        h = new TH1D( variable, ";min #DeltaR(l,Z);Events",100,0,6);
  if(variable=="maxdrlz")        h = new TH1D( variable, ";max #DeltaR(l,Z);Events",100,0,6);
  if(variable=="ptl1")           h = new TH1D( variable, ";p_{T}(l^{(1)});Events", 100,0,1000);
  if(variable=="ptl2")           h = new TH1D( variable, ";p_{T}(l^{(2)});Events", 100,0,1000);
  if(variable=="mtl1")           h = new TH1D( variable, ";M_{T}(l^{(1)},E_{T}^{miss});Events", 100,0,1000);
  if(variable=="mtl2")           h = new TH1D( variable, ";M_{T}(l^{(2)},E_{T}^{miss});Events", 100,0,1000);

  if(variable=="zmass")          h = new TH1D( variable, ";M^{ll};Events", 100,50,200);
  if(variable=="zpt")            h = new TH1D( variable, ";p_{T}^{ll};Events", 100,0,400);
  if(variable=="zeta")           h = new TH1D( variable, ";#eta^{ll};Events", 100,-5,5);
  if(variable=="met")            h = new TH1D( variable, ";E_{T}^{miss};Events", 100,0,500);
  if(variable=="dphizz")         h = new TH1D( variable, ";#Delta#phi(Z_{ll},E_{T}^{miss});Events",100,-3.2,3.2);
  if(variable=="metoverzpt")     h = new TH1D( variable, ";type I E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9);

  if(variable=="redMet")         h = new TH1D( variable, ";red-E_{T}^{miss};Events", 100,0,500);
  if(variable=="redMetL")        h = new TH1D( variable, ";red-E_{T}^{miss,#parallel};Events", 100,-250,250);
  if(variable=="redMetT")        h = new TH1D( variable, ";red-E_{T}^{miss,#perp};Events", 100,-250,250);
  if(variable=="redMetoverzpt")  h = new TH1D( variable,  ";red-E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9);

  if(variable=="projMet")        h = new TH1D( variable, ";projected E_{T}^{miss};Events", 100,0,500);
  if(variable=="projMetoverzpt") h = new TH1D( variable, ";projected E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9);

  return h;
}

//
int main(int argc, char* argv[])
{
  SelectionMonitor controlHistos; //plot storage

  //start computers
  ProjectedMETComputer pmetComp;
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
  //ReducedMETFitter rmetFitter(runProcess);
  TransverseMassComputer mtComp;
  EventCategory eventClassifComp;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");

  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //jet description
  TString etaFileName = runProcess.getParameter<std::string>("etaResolFileName"); gSystem->ExpandPathName(etaFileName);
  JetResolution stdEtaResol(etaFileName.Data(),false);
  TString phiFileName = runProcess.getParameter<std::string>("phiResolFileName"); gSystem->ExpandPathName(phiFileName);
  JetResolution stdPhiResol(phiFileName.Data(),false);
  TString ptFileName  = runProcess.getParameter<std::string>("ptResolFileName");  gSystem->ExpandPathName(ptFileName);
  JetResolution stdPtResol(ptFileName.Data(),true);
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());
  jet::UncertaintyComputer jcomp(&stdPtResol,&stdEtaResol,&stdPhiResol,&jecUnc);

  //TMVA configuration
  TMVA::Reader *tmvaReader = 0;
  bool useMVA                             = runProcess.getParameter<bool>("useMVA");
  edm::ParameterSet tmvaInput             = runProcess.getParameter<edm::ParameterSet>("tmvaInput");
  std::vector<std::string> methodList     = tmvaInput.getParameter<std::vector<std::string> >("methodList");
  std::vector<std::string> varsList       = tmvaInput.getParameter<std::vector<std::string> >("varsList");
  std::vector<int> evCategories           = tmvaInput.getParameter<std::vector<int> >("evCategories");
  std::string weightsDir                  = tmvaInput.getParameter<std::string>("weightsDir");
  std::string studyTag                    = tmvaInput.getParameter<std::string>("studyTag");

  std::vector<Float_t> discriResults(methodList.size(),0);
  Float_t pdeFoamError(0), pdeFoamSig(0), fisherProb(0), fisherRarity(0);
  std::vector<Float_t> tmvaVars(varsList.size()+1,0);

  //control histograms for variables of interest are booked by default
  for(size_t ivar=0; ivar<varsList.size(); ivar++) controlHistos.addHistogram( getHistogramForVariable( varsList[ivar] ) );

  if(useMVA)
    {
      std::cout << "==> Start TMVA Classification with " << methodList.size() << " methods and " << varsList.size() << " variables" << std::endl;

      //start the reader for the variables and methods
      tmvaReader = new TMVA::Reader( "!Color:!Silent" );
      for(size_t ivar=0; ivar<varsList.size(); ivar++)   tmvaReader->AddVariable( varsList[ivar], &tmvaVars[ivar] );
      tmvaReader->AddSpectator("eventCategory", &tmvaVars[varsList.size()]);

      //book the methods
      for(size_t imet=0; imet<methodList.size(); imet++)
	{
          //open the file with the method description
          TString weightFile = weightsDir + "/" + studyTag + ( evCategories.size()>1 ? "_Category_" + methodList[imet] : "") + TString(".weights.xml");
          gSystem->ExpandPathName(weightFile);

	  tmvaReader->BookMVA(methodList[imet], weightFile);
	  controlHistos.addHistogram( tmva::getHistogramForDiscriminator( methodList[imet] ) );
	  if(methodList[imet]=="PDEFoam") 
	    {
	      controlHistos.addHistogram( new TH1D( "PDEFoam_Err",    "PDEFoam error",        100,  0, 1 ) );
	      controlHistos.addHistogram( new TH1D( "PDEFoam_Sig",    "PDEFoam significance", 100,  0, 10 ) );
	    }
	  else if(methodList[imet]=="Fisher") 
	    {
	      controlHistos.addHistogram( new TH1D( "Fisher_Proba",  "Fisher_Proba",  100, 0, 1 ) );
	      controlHistos.addHistogram( new TH1D( "Fisher_Rarity", "Fisher_Rarity", 100, 0, 1 ) );
	    }
	}
    }
  
  //bool useFitter = runProcess.getParameter<bool>("useFitter");

  //book the other control histograms
  TH1F *h=new TH1F ("eventflow", ";Step;Events", 7,0,7);
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(3,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(4,"no SSVHEM tags");
  h->GetXaxis()->SetBinLabel(5,"red-E_{T}^{miss}>39");
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>57");
  controlHistos.addHistogram( h );

  controlHistos.addHistogram( new TH1F ("eventCategory", ";Event Category;Events", 4,0,4) );
  
  controlHistos.addHistogram( new TH1F ("nbtags_ssvhe", ";b-tag multiplicity (SSVHE); Events", 4,0,4) );  
  controlHistos.addHistogram( new TH1F ("nbtags_tche", ";b-tag multiplicity (TCHE); Events", 4,0,4) );  
  controlHistos.addHistogram( new TH1F ("nbtags", ";b-tag multiplicity (SSVHE or TCHE); Events", 4,0,4) );  

  controlHistos.addHistogram( new TProfile ("metprof", ";Pileup events;E_{T}^{miss}", 15,0,15) ); 
  controlHistos.addHistogram( new TH2F ("metvspu", ";Pileup events;E_{T}^{miss}", 15,0,15,100,0,500) );  
  controlHistos.addHistogram( new TProfile ("redMetprof", ";Pileup events;red-E_{T}^{miss}", 15,0,15) );  
  controlHistos.addHistogram( new TH2F ("redMetvspu", ";Pileup events;red-E_{T}^{miss}", 15,0,15,100,0,500) );  
  controlHistos.addHistogram( (TH1D *)(new TH2D ("redMetcomps", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
  controlHistos.addHistogram( new TProfile ("projMetprof", ";Pileup events;projected E_{T}^{miss}", 15,0,15) );  
  controlHistos.addHistogram( new TH2F ("projMetvspu", ";Pileup events;projected E_{T}^{miss}", 15,0,15,100,0,500) );  

  //replicate monitor for interesting categories
  TString cats[]={"ee","emu","mumu"};
  TString subCats[]={"","eq0jets","eq1jets","geq2jets","vbf"};
  TString subsubCats[]={""};                                  //,"jer","jesup","jesdown","nopu","flatpu","btag"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    for(size_t isubcat=0;isubcat<sizeof(subCats)/sizeof(TString); isubcat++)
      for(size_t itopcat=0;itopcat<sizeof(subsubCats)/sizeof(TString); itopcat++)
	controlHistos.initMonitorForStep(cats[icat]+subCats[isubcat]+subsubCats[itopcat]);


  //open the file and get events tree
  ZZ2l2nuSummaryHandler evSummaryHandler;
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }

  //check run range to compute scale factor (if not all entries are used)
  const Int_t totalEntries= evSummaryHandler.getEntries();
  float rescaleFactor( evEnd>0 ?  float(totalEntries)/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=totalEntries;
  if(evStart > evEnd ) 
    {
      file->Close();
      return -1;
    }

  //check PU normalized entries 
  evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  TEventList *elist = (TEventList*)gDirectory->Get("elist");
  const Int_t normEntries = elist->GetN(); 
  if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;

  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/mumu/mumu_cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      if(rescaleFactor>0) cnorm /= rescaleFactor;
    }
  cout << "xSec x Br=" << xsec << " total entries=" << totalEntries << " normalized PU entries=" << normEntries << " obtained from " << cnorm << " generated events" << endl; 
    
  // init summary tree (unweighted events for MVA training) 
  bool saveSummaryTree = runProcess.getParameter<bool>("saveSummaryTree");
  if(!isMC) saveSummaryTree=false; //temporary for now
  TFile *spyFile=0;
  TDirectory *spyDir=0;
  ofstream *outf=0;
  ZZ2l2nuSummaryHandler *spyHandler=0;
  float summaryWeight(1);
  if(saveSummaryTree && normEntries>0)
    {
      summaryWeight = xsec * float(totalEntries) / (cnorm * float(normEntries) );
      spyHandler = new ZZ2l2nuSummaryHandler;
      spyFile = TFile::Open("EventSummaries.root","UPDATE");
      TString evtag=gSystem->BaseName(url);
      evtag.ReplaceAll(".root","");
      spyFile->rmdir(evtag);
      spyDir = spyFile->mkdir(evtag);
      TTree *outT = evSummaryHandler.getTree()->CloneTree(0);
      outT->SetDirectory(spyDir);
      spyHandler->initTree(outT,false);
  
      if(!isMC) outf=new ofstream("highmetevents.txt",ios::app);  
    }
  
  //run the analysis
  for( int iev=evStart; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      float weight=ev.weight;
      if(!isMC) weight=1;

      //classify the event                                                                                                                                                                                                                  
      int eventCategory = eventClassifComp.Get(phys);
      TString subcat    = eventClassifComp.GetLabel(eventCategory);

      //MC truth
      // LorentzVector genzll(0,0,0,0), genzvv(0,0,0,0), higgs(0,0,0,0);
      // if(isMC && phys.genleptons.size()) 
      // {
      //  genzll=phys.genleptons[0]+phys.genleptons[1];
      //  genzvv=phys.genmet[0];
      //  higgs = phys.genhiggs[0];	
      //  }

      //event categories
      TString evcat("");
      if(ev.cat==dilepton::EMU)  evcat="emu";
      if(ev.cat==dilepton::MUMU) evcat="mumu";
      if(ev.cat==dilepton::EE)   evcat="ee";

      //start analysis by the jet kinematics so that one can loop over JER/JES/b-tag systematic variations
      LorentzVector recoMetP4=phys.met[0];
      LorentzVector trkMetP4=phys.met[1];
      LorentzVectorCollection recoJetsP4;
      int nbtags(0),nbtags_tche(0), nbtags_ssvhe(0);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) 
	{
	  recoJetsP4.push_back( phys.jets[ijet] );
	  if(phys.jets[ijet].pt()>30 && fabs(phys.jets[ijet].eta())<2.5)
	    {
	      bool passTCHEL(phys.jets[ijet].btag1>1.7);
	      bool passSSVHEM(phys.jets[ijet].btag3>1.74);
	      nbtags       += (passTCHEL || passSSVHEM);
	      nbtags_tche  += passTCHEL;
	      nbtags_ssvhe += passSSVHEM;
	    }
	}
      
      //uncomment the following lines when ready for evaluation of systematics
      // jcomp.compute(recoJetsP4,phys.met[0]);
      // LorentzVector metVars[] = { recoMetP4, 
      // 				  jcomp.getVariedMet(jet::UncertaintyComputer::JER), 
      // 				  jcomp.getVariedMet(jet::UncertaintyComputer::JES_DOWN), 
      // 				  jcomp.getVariedMet(jet::UncertaintyComputer::JES_UP) };
      // LorentzVectorCollection jetVars[] = {recoJetsP4,
      // 					   jcomp.getVariedJets(jet::UncertaintyComputer::JER),
      // 					   jcomp.getVariedJets(jet::UncertaintyComputer::JES_DOWN),
      //					   jcomp.getVariedJets(jet::UncertaintyComputer::JES_UP)};
      // TString systVars[]={"","}
      // for(size_t ivar=0; ivar<sizeof(metVars)/sizeof(LorentzVector); ivar++) 
      // 	{
      // 	  LorentzVector metP4=metVars[ivar];
      //          LorentzVectorCollection jetsp4=jetVars[ivar];

      LorentzVector metP4=recoMetP4; 
      LorentzVectorCollection jetsp4=recoJetsP4;

      //z+met kinematics
      LorentzVector zll=phys.leptons[0]+phys.leptons[1];
      LorentzVector zvv=metP4;
      Float_t dphill     = deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      Float_t detall     = phys.leptons[0].eta()-phys.leptons[1].eta();
      Float_t drll       = deltaR(phys.leptons[0],phys.leptons[1]);
      Float_t mindrlz    = min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t maxdrlz    = max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t ptl1       = phys.leptons[0].pt();
      Float_t ptl2       = phys.leptons[1].pt();
      Float_t mtl1       = mtComp.compute(phys.leptons[0],zvv,false);
      Float_t mtl2       = mtComp.compute(phys.leptons[1],zvv,false);
      Float_t zmass      = zll.mass();
      Float_t zpt        = zll.pt();
      Float_t zeta       = zll.eta();
      Float_t met        = zvv.pt();
      Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
      Float_t mt         = mtComp.compute(zll,zvv,true);
      Float_t metoverzpt = met/zpt;

      //redmet
      rmetComp.compute(phys.leptons[0],0,phys.leptons[1], 0, jetsp4, zvv );
      // int rMetCateg = rmetComp.getEventCategory();
      Float_t redMet         = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t redMetL        = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t redMetT        = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t redMetoverzpt  = redMet/zpt;

      //projected met
      Float_t projMet        = pmetComp.compute(phys.leptons[0], phys.leptons[1], zvv );
      Float_t projMetoverzpt = projMet/zpt;

      //minimized met
      //Float_t projTrkMet     = pmetComp.compute(phys.leptons[0],phys.leptons[1], trkMetP4);
      //Float_t minMet        = min(fabs(projMet),fabs(projTrkMet));
      // Float_t minMetoverzpt   = minMet/zpt;

      if(redMet<39)continue;


      //set the variables to be used in the MVA evaluation (independently of its use)
      for(size_t ivar=0; ivar<varsList.size(); ivar++) 
	{
	  std::string variable=varsList[ivar];
	  if(variable=="dphill")         tmvaVars[ivar]=dphill;
	  if(variable=="detall")         tmvaVars[ivar]=detall;
	  if(variable=="drll")           tmvaVars[ivar]=drll;
	  if(variable=="mindrlz")        tmvaVars[ivar]=mindrlz;
	  if(variable=="maxdrlz")        tmvaVars[ivar]=maxdrlz;
	  if(variable=="ptl1")           tmvaVars[ivar]=ptl1;
	  if(variable=="ptl2")           tmvaVars[ivar]=ptl2;
	  if(variable=="mtl1")           tmvaVars[ivar]=mtl1;
	  if(variable=="mtl2")           tmvaVars[ivar]=mtl2;
	  
	  if(variable=="zmass")          tmvaVars[ivar]=zmass;
	  if(variable=="zpt")            tmvaVars[ivar]=zpt;
	  if(variable=="zeta")           tmvaVars[ivar]=zeta;
	  if(variable=="met")            tmvaVars[ivar]=met;
	  if(variable=="dphizz")         tmvaVars[ivar]=dphizz;
	  if(variable=="metoverzpt")     tmvaVars[ivar]=metoverzpt;
	  
	  if(variable=="redMet")         tmvaVars[ivar]=redMet;
	  if(variable=="redMetL")        tmvaVars[ivar]=redMetL;
	  if(variable=="redMetT")        tmvaVars[ivar]=redMetT;
	  if(variable=="redMetoverzpt")  tmvaVars[ivar]=redMetoverzpt;
	  if(variable=="projMet")        tmvaVars[ivar]=projMet;
	  if(variable=="projMetoverzpt") tmvaVars[ivar]=projMetoverzpt;
	}
      tmvaVars[varsList.size()] = eventCategory;

      //MVA analysis
      if(useMVA)
	{
	  //evaluate the methods
	  for(size_t imet=0; imet<methodList.size(); imet++)
	    {
	      discriResults[imet]=tmvaReader->EvaluateMVA( methodList[imet] );
	     
	      //per-event error
	      if (methodList[imet]=="PDEFoam")
		{
		  pdeFoamError = tmvaReader->GetMVAError();
		  pdeFoamSig   = (pdeFoamError>1.e-50 ? discriResults[imet]/pdeFoamError : 0.);
		}
	      //probability for Fisher discriminant
	      if (methodList[imet]=="Fisher") 
		{
		  fisherProb   = tmvaReader->GetProba("Fisher");
		  fisherRarity = tmvaReader->GetRarity("Fisher");
		}
	    }
	}
      
      //event selection
      bool passZmass(fabs(zmass-91)<15);
      bool pass3dLeptonVeto(ev.ln==0);
      bool passBveto(nbtags==0);
      bool passMediumRedMet(redMet>39);
      bool passTightRedMet(redMet>57);

      //fill control histograms
      TString catsToFill[]={"all",evcat};
      TString subCatsToFill[]={"",subcat};
      TString subsubCatToFill="";  //subsubCats[ivar]
      for(size_t ic=0; ic<sizeof(catsToFill)/sizeof(TString); ic++)
	{
	  for(size_t isc=0; isc<sizeof(subCatsToFill)/sizeof(TString); isc++)
	    {
	      TString ctf=catsToFill[ic]+subCatsToFill[isc]+subsubCatToFill;

	      controlHistos.fillHisto("eventflow",ctf,2,weight);
	      if(!passZmass) continue;

	      controlHistos.fillHisto("eventflow",ctf,3,weight);
	      if(!pass3dLeptonVeto) continue;

	      controlHistos.fillHisto("eventflow",ctf,4,weight);
	      controlHistos.fillHisto("nbtags",ctf, nbtags,weight);
	      controlHistos.fillHisto("nbtags_tche",ctf, nbtags_tche,weight);
	      controlHistos.fillHisto("nbtags_ssvhe",ctf, nbtags_ssvhe,weight);
	      if(!passBveto) continue;

	      controlHistos.fillHisto("eventflow",ctf,5,weight);
	      controlHistos.fill2DHisto("zptvszeta", ctf,zll.pt(),zll.eta(),weight);
	      for(size_t ivar=0; ivar<varsList.size(); ivar++)  controlHistos.fillHisto(varsList[ivar],ctf,tmvaVars[ivar],weight);
	      controlHistos.fillHisto("eventCategory",ctf,eventCategory,weight);
	      controlHistos.fillProfile("metprof", ctf,ev.ngenITpu,met);
	      controlHistos.fill2DHisto("metvspu", ctf,ev.ngenITpu,met,weight);
	      controlHistos.fillProfile("projMetprof", ctf,ev.ngenITpu,projMet);
	      controlHistos.fill2DHisto("projMetvspu", ctf,ev.ngenITpu,projMet,weight);     
	      controlHistos.fillProfile("redMetprof", ctf,ev.ngenITpu,redMet);
	      controlHistos.fill2DHisto("redMetvspu", ctf,ev.ngenITpu,redMet,weight);
	      controlHistos.fillHisto("redMetcomps", ctf,redMetL,redMetT,weight);	
	      
	      //save summary tree now if required -> move this to after the red-MET cut
	      if(saveSummaryTree && ev.normWeight==1)
		{
		  ev.pass=passMediumRedMet+passTightRedMet;
		  ev.weight=summaryWeight;		      
		  spyHandler->fillTree();
		}
	      
	      //control for discriminators evaluated
	      for(size_t imet=0; imet<methodList.size(); imet++)
		{
		  controlHistos.fillHisto(methodList[imet],ctf,discriResults[imet],weight);
		  
		  //per-event error
		  if (methodList[imet]=="PDEFoam")
		    {
		      controlHistos.fillHisto("PDEFoam_Err",ctf,pdeFoamError,weight);
		      controlHistos.fillHisto("PDEFoam_Sig",ctf,pdeFoamSig,weight);
		    }
		  //probability for Fisher discriminant
		  if (methodList[imet]=="Fisher") 
		    {
		      controlHistos.fillHisto("Fisher_Proba",ctf, fisherProb, weight);
		      controlHistos.fillHisto("Fisher_Rarity",ctf, fisherRarity, weight);
		    }
		}
	      

	      //red-met cut
	      if(!passMediumRedMet)  continue;
	      controlHistos.fillHisto("eventflow",ctf,6,weight);
	      if(!passTightRedMet)  continue;
	      controlHistos.fillHisto("eventflow",ctf,7,weight);
	      	      
	      
	      //debug
	      if(ic==0 && isc==0 && !isMC && passTightRedMet && outf)	
		{
		  *outf << "<b>Selected event</b><br/>" << std::endl;
		  *outf << "%$Run=" <<  ev.run << "$% %$Lumi=" << ev.lumi << "$% %$Event=" << ev.event <<"$%" << "<br/>" << std::endl;
		  
		  *outf << "<i>Leptons</i>" << "<br/>" << std::endl;
		  for(size_t ilep=0; ilep<2; ilep++)
		    *outf << "%$l_{" << ilep+1 << "}=" << phys.leptons[ilep].id << "$% "
			  << "%$p_T=" << phys.leptons[ilep].pt() << "$% "
			  << "%$\\eta=" << phys.leptons[ilep].eta() << "$% "
			  << "%$\\phi=" << phys.leptons[ilep].phi() << "$% "
			  << "%$I_{neut}=" << phys.leptons[ilep].iso1 << "$% "
			  << "%$I_{ch}=" << phys.leptons[ilep].iso2  << "$% "
			  << "%$I_{pho}=" << phys.leptons[ilep].iso3 << "$% " << "<br/>" << std::endl; 
		  
		  *outf << "<i>Dilepton</i>" << "<br/>" << std::endl;
		  *outf  << "%$p_{T}^{ll}=" << zll.pt() << "$% "
			 << "%$\\eta^{ll}=" << zll.eta() << "$% "
			 << "%$\\phi^{ll}=" << zll.phi() << "$% "
			 << "%$m^{ll}=" << zll.mass() << "$% "   
			 << "%$\\Delta R(l,l)=" << drll << "$% "
			 << "%$\\Delta\\phi(l,l)=" << dphill << "$% " << "<br/>" << std::endl;
		  
		  *outf << "<i>Missing transverse energy</i>" << "<br/>" << std::endl;
		  *outf << "%$E_{T}^{miss}=" << zvv.pt() << "$% "
			<< "%$\\phi=" << zvv.phi() << "$% " << "<br/>" << std::endl;
		  *outf << "%$red-E_{T}^{miss}=" << redMet << "$% "
			<< "%$l="<< redMetL << "$% "
			<< "%$t=" << redMetT << "$% " << "<br/>" << std::endl;
		  
		  *outf << "<i>Transverse mass</i>" << "<br/>" << std::endl;
		  *outf << "%$\\sum M_T(l,E_{T}^{miss})=" << mtl1+mtl2 << "$% "
			<< "%$M_T(Z,E_{T}^{miss})=" << mt << "$% " << "<br/>" << std::endl; 
		  
		  *outf << "<i>Jet activity</i>" << "<br/>" << std::endl;
		  *outf << "%$N {jets}(p_T>15)= " << ev.jn << "$% "
			<< "%$\\rho=" << ev.rho << "$% "  << "<br/>" << std::endl;
		  
		  *outf << "------" << endl;
		}
	    }
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

  if(saveSummaryTree)
    {
      spyDir->cd();
      spyHandler->getTree()->Write();
      spyFile->Write();
      spyFile->Close();
      if(!isMC) outf->close();
    }
}  
