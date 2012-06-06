#include <iostream>
#include <boost/shared_ptr.hpp>

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/EfficiencyMap.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TEventList.h"
#include "TROOT.h"
 
using namespace std;

TString getJetRegion(float eta)
{
  TString reg("TK");
  if(fabs(eta)>2.5)  reg="HEin";
  if(fabs(eta)>2.75) reg="HEout";
  if(fabs(eta)>3)    reg="HF";
  return reg;
}


int main(int argc, char* argv[])
{
  //##############################################
  //########    GLOBAL INITIALIZATION     ########
  //##############################################

  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  TString cmsswRel(gSystem->Getenv("CMSSW_BASE"));
  bool use2011Id(cmsswRel.Contains("4_4_"));
  cout << cmsswRel << endl; 
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool isMC = runProcess.getParameter<bool>("isMC");
  bool runBlinded = runProcess.getParameter<bool>("runBlinded"); 
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");

  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  int fType(0);
  if(url.Contains("DoubleEle")) fType=EE;
  if(url.Contains("DoubleMu"))  fType=MUMU;
  if(url.Contains("MuEG"))      fType=EMU;
  if(url.Contains("SingleMu"))  fType=MUMU;
  bool isSingleMuPD(!isMC && url.Contains("SingleMu"));  
  
  TString outTxtUrl= outUrl + "/" + gSystem->BaseName(url) + ".txt";
  FILE* outTxtFile = NULL;
  if(!isMC)outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());

  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //jet energy scale uncertainties
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  //systematics
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  TString varNames[]={"","_jesup","_jesdown","_jerup","_jerdown","_puup","_pudown","_renup","_rendown","_factup","_factdown","_btagup","_btagdown"};//,"_lesup","_lesdown"};
  size_t nvarsToInclude(1);
  if(runSystematics)
    {
      cout << "Systematics will be computed for this analysis" << endl;
      nvarsToInclude=sizeof(varNames)/sizeof(TString);
    }

  // this is disabled for the moment
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isMC_GG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isMC_VBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  std::vector<TGraph *> hWeightsGrVec;
  if(isMC_GG && use2011Id){
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
    TString hqtWeightsFileURL = runProcess.getParameter<std::string>("hqtWeightsFile"); gSystem->ExpandPathName(hqtWeightsFileURL);
    TFile *fin=TFile::Open(hqtWeightsFileURL);
    if(fin){
      cout << "HpT weights (and uncertainties) will be applied from: " << hqtWeightsFileURL << endl;
      for(int ivar=0; ivar<5; ivar++){
	double ren=HiggsMass; if(ivar==ZZ2l2nuSummary_t::hKfactor_renDown)  ren/=2; if(ivar==ZZ2l2nuSummary_t::hKfactor_renUp)  ren *= 2;
	double fac=HiggsMass; if(ivar==ZZ2l2nuSummary_t::hKfactor_factDown) fac/=2; if(ivar==ZZ2l2nuSummary_t::hKfactor_factUp) fac *= 2;
	char buf[100]; sprintf(buf,"kfactors_mh%3.0f_ren%3.0f_fac%3.0f",HiggsMass,ren,fac);
	TGraph *gr= (TGraph *) fin->Get(buf);
	if(gr) hWeightsGrVec.push_back((TGraph *)gr->Clone());
      }
      fin->Close();
      delete fin;
    }
  }else if(isMC_VBF){
    size_t VBFStringpos =  string(url.Data()).find("VBF");
    string StringMass = string(url.Data()).substr(VBFStringpos+6,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    VBFString = string(url.Data()).substr(VBFStringpos);
  }



  //##############################################
  //########    INITIATING HISTOGRAMS     ########
  //##############################################
  SmartSelectionMonitor mon;
  TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  TH1F *h=(TH1F*) mon.addHistogram( new TH1F ("eventflow", ";Step;Events", 7,0,7) );
  h->GetXaxis()->SetBinLabel(1,"#geq 2 leptons");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(3,"p_{T}^{ll}>55");
  h->GetXaxis()->SetBinLabel(4,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(5,"b-veto"); 
  h->GetXaxis()->SetBinLabel(6,"#Delta #phi(jet,E_{T}^{miss})>0.5");
  h->GetXaxis()->SetBinLabel(7,"E_{T}^{miss}>70");

  mon.addHistogram( new TH1F( "leadpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "leadeta", ";#eta^{l};Events", 50,-2.6,2.6) );
  mon.addHistogram( new TH1F( "trailerpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "trailereta", ";#eta^{l};Events", 50,-2.6,2.6) );
  mon.addHistogram( new TH1F( "deltaleptonpt", ";|p_{T}^{1}-p_{T}^{2}|;Events", 50,0,250) );
  mon.addHistogram( new TH1F( "deltazpt", ";p_{T}^{ll}-E_{T}^{miss};Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 50,-10,10) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,40,250) );

  mon.addHistogram( new TH1F( "thirdleptonpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "thirdleptoneta", ";#eta^{l};Events", 50,-2.6,2.6) );
  mon.addHistogram( new TH1F( "nleptons", ";Leptons;Events", 3,2,4) );

  //lepton control
  Double_t effptAxis[]={0,10,20,30,40,50,75,100,150,200,500};
  const size_t nEffptAxis=sizeof(effptAxis)/sizeof(Double_t)-1;
  for(size_t ilep=0; ilep<2; ilep++)
    {
      TString lepStr(ilep==0? "mu" :"e");
      mon.addHistogram(new TH1F(lepStr+"genpt",   ";p_{T} [GeV/c];Leptons",nEffptAxis,effptAxis) );
      mon.addHistogram(new TH1F(lepStr+"geneta",   ";#eta;Leptons",50,-5,5) );
      mon.addHistogram(new TH1F(lepStr+"genpu",   ";Pileup;Leptons",50,0,50) );
      for(int iid=0; iid<4; iid++)
	{
	  TString idctr(""); idctr+=iid;
	  mon.addHistogram(new TH1F(lepStr+idctr+"pt",   ";p_{T} [GeV/c];Leptons",nEffptAxis,effptAxis) );
	  mon.addHistogram(new TH1F(lepStr+idctr+"eta",   ";#eta;Leptons",50,-5,5) );
	  mon.addHistogram(new TH1F(lepStr+idctr+"pu",   ";Pileup;Leptons",50,0,50) );
	  mon.addHistogram(new TH1F(lepStr+idctr+"isopt",   ";p_{T} [GeV/c];Leptons",nEffptAxis,effptAxis) );
	  mon.addHistogram(new TH1F(lepStr+idctr+"isoeta",   ";#eta;Leptons",50,-5,5) );
	  mon.addHistogram(new TH1F(lepStr+idctr+"isopu",   ";Pileup;Leptons",50,0,50) );
	}
      if(ilep==1)
	{
	  for(size_t ireg=0; ireg<2; ireg++)
	    { 
	      TString reg(ireg==0?"eb":"ee");
	      mon.addHistogram(new TH1F(lepStr+reg+"detain",   ";#Delta#eta_{in};Leptons",50,0,0.01) );
	      mon.addHistogram(new TH1F(lepStr+reg+"dphiin",   ";#Delta#phi_{in};Leptons",50,0,0.1) );
	      mon.addHistogram(new TH1F(lepStr+reg+"sihih",    ";#sigma_{i#eta i#eta};Leptons",50,0,0.05) );
	      mon.addHistogram(new TH1F(lepStr+reg+"sipip",    ";#sigma_{i#phi i#phi};Leptons",50,0,0.05) );
	      mon.addHistogram(new TH1F(lepStr+reg+"r9",       ";R_{9};Leptons",50,0,1.) );
	      mon.addHistogram(new TH1F(lepStr+reg+"hoe",      ";h/e;Leptons",50,0,0.2) );
	      mon.addHistogram(new TH1F(lepStr+reg+"ooemoop",  ";1/E-1/p;Leptons",100,0,0.05) );
	      mon.addHistogram(new TH1F(lepStr+reg+"eopin",    ";E/p;Leptons",100,0,2) );
	      mon.addHistogram(new TH1F(lepStr+reg+"fbrem",    ";f_{brem};Leptons",100,0,2) );
	    }
	}
      else
	{
	  mon.addHistogram(new TH1F(lepStr+"nmatches", ";Muon matches;Leptons",15,0,15) );
	  mon.addHistogram(new TH1F(lepStr+"nmuonhits", ";Muon hits;Leptons",30,0,30) );
	}
      mon.addHistogram(new TH1F(lepStr+"d0",            ";d_{0};Leptons",50,0,0.05) );
      mon.addHistogram(new TH1F(lepStr+"dZ",            ";d_{Z};Leptons",50,0,0.1) );
      mon.addHistogram(new TH1F(lepStr+"trkchi2",       ";#chi^{2};Leptons",50,0,10) );
      mon.addHistogram(new TH1F(lepStr+"trkvalidpixel",  ";Valid pixel hits;Leptons",20,0,20) );
      mon.addHistogram(new TH1F(lepStr+"trkvalidtracker",  ";Valid tracker hits;Leptons",50,0,50) );
      mon.addHistogram(new TH1F(lepStr+"losthits",         ";Lost hits;Leptons",4,0,4) );
      mon.addHistogram(new TH1F(lepStr+"reliso",           ";RelIso;Leptons",50,0,2) );
      mon.addHistogram(new TH1F(lepStr+"reliso2011",        ";RelIso(#rho);Leptons",50,0,2) );
      mon.addHistogram(new TH1F(lepStr+"truereliso",           ";RelIso;Leptons",50,0,2) );
      mon.addHistogram(new TH1F(lepStr+"truereliso2011",        ";RelIso(#rho);Leptons",50,0,2) );
      mon.addHistogram(new TH1F(lepStr+"fakereliso",           ";RelIso;Fake leptons",50,0,2) );
      mon.addHistogram(new TH1F(lepStr+"fakereliso2011",        ";RelIso(#rho);Fake leptons",50,0,2) );
    }
  
  mon.addHistogram( new TH1F( "nvtx",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "nvtxraw",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "rho",";#rho;Events",50,0,25) ); 
  mon.addHistogram( new TH1F( "rho25",";#rho(#eta<2.5);Events",50,0,25) ); 

  TString jetTypes[]={"pf"};//,"pfchs"};
  const size_t nJetTypes=sizeof(jetTypes)/sizeof(TString);
  TString jetRegs[]={"TK","HEin","HEout","HF"};
  const size_t nJetRegs=sizeof(jetRegs)/sizeof(TString);
  TString btagAlgos[]={"TCHE","CSV","JP"};
  Double_t btagAlgoMin[]={-5,0,0};
  Double_t btagAlgoMax[]={15,1.,3};
  TString jetIds[]={"pfloose","pftight","minloose","minmedium"};
  const size_t nJetIds=sizeof(jetIds)/sizeof(TString);
  Double_t jetPtBins[]={0,15,20,25,30,40,50,60,70,80,100,200,300,400,500,600,700,1000};
  Int_t nJetPtBins=sizeof(jetPtBins)/sizeof(Double_t)-1;
  Double_t jetEtaBins[]={0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.625,2.75,2.875,3.0,3.5,4.0,4.5,5.0};
  Double_t nJetEtaBins=sizeof(jetEtaBins)/sizeof(Double_t)-1;
  for(size_t i=0; i<nJetTypes; i++)
    {
      for(size_t ireg=0; ireg<nJetRegs; ireg++)
	{
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetbeta"    , ";#beta;Events",50,0,1) );
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetbetastar", ";#beta *;Events",50,0,1) );
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetdrmean"  , ";<#Delta R>;Events",50,0,0.5) );
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetptd"     , ";p_{T}D [GeV/c];Events",50,0,1) );
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetptrms"   , ";RMS p_{T} [GeV/c];Events",50,0,0.5) );
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetmva"     , ";MVA;Events",50,-1,1) );
	  mon.addHistogram( new TH1F(jetRegs[ireg]+jetTypes[i]+"jetbalance", ";Jet p_{T} / Boson p_{T};Events", 25,0,2.5) );
	  if(ireg==0) mon.addHistogram( new TH1F(jetTypes[i]+"jetbalance", ";Jet p_{T} / Boson p_{T};Events", 25,0,2.5) );
	  for(size_t iid=0; iid<nJetIds; iid++)
	    {
	      mon.addHistogram( new TH1F( jetRegs[ireg]+jetIds[iid]+jetTypes[i]+"jetpt", ";Jet p_{T} [GeV/c];Jets", nJetPtBins,jetPtBins) );
	      mon.addHistogram( new TH1F( jetRegs[ireg]+"true"+jetIds[iid]+jetTypes[i]+"jetpt", ";Jet p_{T} [GeV/c];Jets", nJetPtBins,jetPtBins) );
	      if(ireg==0) 
		{
		  mon.addHistogram( new TH1F( jetIds[iid]+jetTypes[i]+"jeteta", ";Jet |#eta|;Jets", nJetEtaBins, jetEtaBins) );
		  mon.addHistogram( new TH1F( "true"+jetIds[iid]+jetTypes[i]+"jeteta", ";Jet |#eta|;Jets", nJetEtaBins, jetEtaBins) );
		}
	    }
	}
      for(size_t ibtag=0; ibtag<3; ibtag++)
	{
	  mon.addHistogram( new TH1F(btagAlgos[ibtag]+"b"+jetTypes[i]+"jetstags",     ";b tags;Events",50,btagAlgoMin[ibtag],btagAlgoMax[ibtag]) );
	  mon.addHistogram( new TH1F(btagAlgos[ibtag]+"other"+jetTypes[i]+"jetstags", ";udscg tags;Events",50,btagAlgoMin[ibtag],btagAlgoMax[ibtag]) );
	  mon.addHistogram( new TH1F(btagAlgos[ibtag]+jetTypes[i]+"jetstags",         ";"+btagAlgos[ibtag]+";Events",50,btagAlgoMin[ibtag],btagAlgoMax[ibtag]) );
	  mon.addHistogram( new TH1F("n"+jetTypes[i]+"jetsbtags"+btagAlgos[ibtag],    ";b-tag multiplicity ("+btagAlgos[ibtag] +");Events",5,0,5) );
	}
      
      mon.addHistogram( new TH1F(jetTypes[i]+"jetpt"       , ";p_{T} [GeV/c];Events",50,0,250) );
      mon.addHistogram( new TH1F(jetTypes[i]+"jeteta"       , ";|#eta|;Events",25,0,5) );
      mon.addHistogram( new TH2F("n"+jetTypes[i]+"jetsvspu",          ";Pileup interactions;Jet multiplicity (p_{T}>30 GeV/c);Events",50,0,50,5,0,5) );
      mon.addHistogram( new TH2F("n"+jetTypes[i]+"jetstightvspu",     ";Pileup interactions;Jet multiplicity (p_{T}>30 GeV/c);Events",50,0,50,5,0,5) );
      TH1 *h=mon.addHistogram( new TH1F("n"+jetTypes[i]+"jets",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
      for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
	{
	  TString label("");
	  if(ibin==h->GetXaxis()->GetNbins()) label +="#geq";
	  else                                label +="=";
	  label += (ibin-1);
	  h->GetXaxis()->SetBinLabel(ibin,label);
	} 
      mon.addHistogram( new TH2F("n"+jetTypes[i]+"jetspuidloosevspu", ";Pileup interactions;Jet multiplicity (p_{T}>30 GeV/c);Events",50,0,50,5,0,5) );
      mon.addHistogram( new TH2F("n"+jetTypes[i]+"jetspuidmediumvspu",";Pileup interactions;Jet multiplicity (p_{T}>30 GeV/c);Events",50,0,50,5,0,5) );  

      mon.addHistogram( new TH1F(jetTypes[i]+"vbfcandjeteta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfcandjetdeta"       , ";|#Delta #eta|;Jets",50,0,10) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfcandjetpt"       , ";p_{T} [GeV/c];Jets",50,0,250) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfcandzeppenfeld"       , ";|Zeppenfeld variable|;Events",50,0,5) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfhardpt"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfhardptfinal"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      h=mon.addHistogram( new TH1F(jetTypes[i]+"vbfcjv"       , ";Central jet count;Events",3,0,3) );
      h->GetXaxis()->SetBinLabel(1,"=0 jets");
      h->GetXaxis()->SetBinLabel(2,"=1 jets");
      h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfhtcjv"       , ";Central jet H_{T} [GeV/c];Events",50,0,250) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfmjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",40,0,2000) );
      mon.addHistogram( new TH1F(jetTypes[i]+"vbfcandjetdphi"       , ";#Delta#phi;Events",20,0,3.5) );
      mon.addHistogram( new TH2F(jetTypes[i]+"vbfmjjvsdeta"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];|#Delta #eta|;Events",40,0,2000,50,0,10) );
      mon.addHistogram( new TH2F(jetTypes[i]+"vbfmjjvshardpt"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Hard p_{T} [GeV/c];Events",40,0,2000,25,0,250) );
    }


  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1D( "balance", ";E_{T}^{miss}/q_{T};Events", 25,0,2.5) );
  mon.addHistogram( new TH1F( "met_met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_met3leptons"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_metRaw"  , ";E_{T}^{miss} (raw);Events", 50,0,500) );
  mon.addHistogram( new TH2F( "met_met_vspu", ";Pileup events; E_{T}^{miss};Events", 50,0,50,200,0,500) );
  mon.addHistogram( new TH1F( "met_min3Met"  , ";min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH2F( "met_min3Met_vspu", ";Pileup events; min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,200,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH2F( "met_redMet_vspu", ";Pileup events; red(E_{T}^{miss},clustered-E_{T}^{miss};Events", 50,0,50,200,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
  

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> optim_Cuts1_met;
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=65;met<160;met+=5.0){
	 if(met>90 && int(met)%10!=0)continue;
         if(met>120 && int(met)%20!=0)continue;
         for(double mtmin_=150;mtmin_<500;mtmin_+=25){
            double mtmin = mtmin_;
            if(mtmin<=150)mtmin=0;
	    if(mtmin>350 && int(mtmin)%50!=0)continue;
            for(double mtmax=mtmin+100;mtmax<mtmin+350;mtmax+=25){
               if(mtmax>=mtmin+325)mtmax=3000;
               if(mtmin==0 && mtmax!=3000)continue;
	       if(mtmin>350 && int(mtmax)%50!=0)continue;
               if(mtmax-mtmin>200 && int(mtmax)%50!=0)continue;
               optim_Cuts1_met    .push_back(met);
               optim_Cuts1_mtmin  .push_back(mtmin);
               optim_Cuts1_mtmax  .push_back(mtmax);
            }
      }
   }
  //add last year cut
  optim_Cuts1_met.push_back( 70); optim_Cuts1_mtmin.push_back(229); optim_Cuts1_mtmax.push_back(258);
  optim_Cuts1_met.push_back( 77); optim_Cuts1_mtmin.push_back(245); optim_Cuts1_mtmax.push_back(293);
  optim_Cuts1_met.push_back( 84); optim_Cuts1_mtmin.push_back(260); optim_Cuts1_mtmax.push_back(328);
  optim_Cuts1_met.push_back( 91); optim_Cuts1_mtmin.push_back(276); optim_Cuts1_mtmax.push_back(364);
  optim_Cuts1_met.push_back( 98); optim_Cuts1_mtmin.push_back(292); optim_Cuts1_mtmax.push_back(399);
  optim_Cuts1_met.push_back(105); optim_Cuts1_mtmin.push_back(308); optim_Cuts1_mtmax.push_back(434);
  optim_Cuts1_met.push_back(112); optim_Cuts1_mtmin.push_back(323); optim_Cuts1_mtmax.push_back(470);
  optim_Cuts1_met.push_back(119); optim_Cuts1_mtmin.push_back(339); optim_Cuts1_mtmax.push_back(505);
  optim_Cuts1_met.push_back(126); optim_Cuts1_mtmin.push_back(355); optim_Cuts1_mtmax.push_back(540);
  optim_Cuts1_met.push_back(133); optim_Cuts1_mtmin.push_back(370); optim_Cuts1_mtmax.push_back(576);
  optim_Cuts1_met.push_back(140); optim_Cuts1_mtmin.push_back(386); optim_Cuts1_mtmax.push_back(611);
  optim_Cuts1_met.push_back(147); optim_Cuts1_mtmin.push_back(402); optim_Cuts1_mtmax.push_back(646);
  optim_Cuts1_met.push_back(154); optim_Cuts1_mtmin.push_back(417); optim_Cuts1_mtmax.push_back(682);
  optim_Cuts1_met.push_back(161); optim_Cuts1_mtmin.push_back(433); optim_Cuts1_mtmax.push_back(717);
  optim_Cuts1_met.push_back(168); optim_Cuts1_mtmin.push_back(449); optim_Cuts1_mtmax.push_back(752);


   TH1F* Hoptim_cuts1_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
      Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
      Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
      Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
   }

   TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;
   for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
   {
       Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
       mon.addHistogram( new TH2F (TString("mt_shapes")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];#events (/25GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 32,150,950) );
       mon.addHistogram( new TH2F (TString("mt_shapesZ10")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];#events (/25GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 1,150,950) );//only cut&count
       mon.addHistogram( new TH2F (TString("mt_shapesZ5")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];#events (/25GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 1,150,950) );//only cut&count

       //3lepton SB
       mon.addHistogram( new TH2F (TString("mt_shapes_3rdLepton")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 1,150,950) );

       if(ivar==0)mon.addHistogram( new TH2F (TString("mt_shapesBTagSB")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 32,150,950) );
       mon.addHistogram( new TH2F (TString("mt_redMet_shapes")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 32,150,950) );
       mon.addHistogram( new TH2F (TString("mt3")+varNames[ivar],";cut index;M_{T}^{3rd lepton} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 50,0,250) );
       TH2F *h=(TH2F *) mon.addHistogram( new TH2F ("nonresbckg_ctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
       h->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
       h->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
       h->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
       h->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
       h->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
       h->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");
   } 


  //##############################################
  //######## GET READY FOR THE EVENT LOOP ########
  //##############################################

  //open the file and get events tree
  ZZ2l2nuSummaryHandler evSummaryHandler;
  TFile *file = TFile::Open(url);
  printf("Looping on %s\n",url.Data());
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ){
      file->Close();
      return -1;
  }


  //check run range to compute scale factor (if not all entries are used)
  const Int_t totalEntries= evSummaryHandler.getEntries();
  float rescaleFactor( evEnd>0 ?  float(totalEntries)/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=totalEntries;
  if(evStart > evEnd ){
      file->Close();
      return -1;
  }

  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC){
      TH1F* cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      if(rescaleFactor>0) cnorm /= rescaleFactor;
      printf("cnorm = %f\n",cnorm);
  }
  Hcutflow->SetBinContent(1,cnorm);


  //pileup weighting: based on vtx for now...
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  //  bool useObservedPU( url.Contains("toZZto2L") );
  bool useObservedPU(true);
  if(isMC){
    TString puDist("evAnalyzer/h2zz/pileuptrue");
    if(useObservedPU) puDist="evAnalyzer/h2zz/pileup";
    TH1F* histo = (TH1F *) file->Get(puDist);
    if(!histo)std::cout<<"pileup histogram is null!!!\n";
    for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
    delete histo;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights=0;
  reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC)
    {
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PShiftUp = new reweight::PoissonMeanShifter(+0.8);
      PShiftDown = new reweight::PoissonMeanShifter(-0.8);
    }

  //event Categorizer
  //EventCategory eventCategoryInst(0); //inclusive analysis
  //EventCategory eventCategoryInst(1); //jet binning
  //EventCategory eventCategoryInst(2); //vbf binning
  EventCategory eventCategoryInst(3); //jet+vbf binning



  //##############################################
  //########           EVENT LOOP         ########
  //##############################################
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  //DuplicatesChecker duplicatesChecker;
  //int nDuplicates(0);
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
   
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      //if( duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      bool mustBlind = (!isMC && runBlinded && evSummaryHandler.hasSpoilerAlert(!isMC));

      //event category
      bool isSameFlavor(ev.cat==MUMU || ev.cat==EE);
      TString tag_cat;
      switch(ev.cat){
      case MUMU : tag_cat = "mumu";  break;
      case EE   : tag_cat = "ee";    break;
      case EMU  : tag_cat = "emu";   break;
      default   : continue;
      }
      //      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) && !isZZ2l2nu(ev.mccat) ) continue;
      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
      if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;

      //require compatibilitiy of the event with the PD
      bool hasEEtrigger = ev.triggerType & 0x1;
      bool hasMMtrigger = (ev.triggerType >> 1 ) & 0x1;
      bool hasEMtrigger = (ev.triggerType >> 2 ) & 0x1;
      bool hasMtrigger  = (ev.triggerType >> 3 ) & 0x1;
      if(!isMC)
	{
	  if(ev.cat!=fType) continue;

	  if(ev.cat==EE   && !hasEEtrigger) continue;
	  if(ev.cat==MUMU && !(hasMMtrigger||hasMtrigger) ) continue;
	  if(ev.cat==EMU  && !hasEMtrigger) continue;

	  //this is a safety veto for the single mu PD
	  if(isSingleMuPD) {
	    if(!hasMtrigger) continue;
	    if(hasMtrigger && hasMMtrigger) continue;
	  }
	}
      
      //prepare the tag's vectors for histo filling
      std::vector<TString> tags_full(1,"all");
      
      //pileup weight
      float weight = 1.0;
      float signalWeight=1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight            = LumiWeights->weight(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
        TotalWeight_plus  = PShiftUp->ShiftWeight(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
        TotalWeight_minus = PShiftDown->ShiftWeight(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
	if(isMC_VBF && use2011Id){ signalWeight = weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );  weight*=signalWeight; }
        if(isMC_GG) {
          for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) 
	    ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
	  weight *= ev.hptWeights[0];
	}
      }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);
      Hcutflow->Fill(5,signalWeight);

      //apply JER base corrections to jets (and compute associated variations)
      std::vector<PhysicsObjectJetCollection> variedAJets;
      LorentzVectorCollection zvvs;
      METUtils::computeVariation(phys.ajets, phys.met[0], variedAJets, zvvs, &jecUnc);


      //
      // LEPTON ANALYSIS
      //
      LorentzVector lep1=phys.leptons[0];
      LorentzVector lep2=phys.leptons[1];
      LorentzVector zll(lep1+lep2);
      bool passIdAndIso(true);
      bool passZmass(fabs(zll.mass()-91)<15);
      bool passZmass10(fabs(zll.mass()-91)<10);
      bool passZmass5(fabs(zll.mass()-91)<5);
      bool isZsideBand( (zll.mass()>40 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200));
      bool isZsideBandPlus( (zll.mass()>110 && zll.mass()<200));
      bool passZpt(zll.pt()>55);

      //check alternative selections for the dilepton
      double llScaleFactor(1.0),llTriggerEfficiency(1.0);
      for(int ilep=0; ilep<2; ilep++)
	{
	  TString lepStr( fabs(phys.leptons[ilep].id)==13 ? "mu" : "e");
	  
	  //generator level matching
	  int matchid(0);
	  LorentzVector genP4(0,0,0,0);
	  for(size_t igl=0;igl<phys.genleptons.size(); igl++) 
	    {
	      if(deltaR(phys.genleptons[igl],phys.leptons[ilep])>0.1) continue;
	      genP4=phys.genleptons[igl];
	      matchid=phys.genleptons[igl].id;
	    }
	  
	  //id and isolation
	  int lpid=phys.leptons[ilep].pid;
	  float relIso2011    = phys.leptons[ilep].relIsoRho(ev.rho);
	  float relIso = (lepStr=="mu") ? 
	    phys.leptons[ilep].pfRelIsoDbeta(): //muPFRelIsoCorrected2012(ev.rho25Neut):
	    phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho);
	  std::vector<int> passIds;
	  std::map<int,bool> passIsos;
	  bool hasGoodId(false), isIso(false);
	  if(fabs(phys.leptons[ilep].id)==13)
	    {
	      if( hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) )    { passIds.push_back(0); passIsos[0]=(relIso<0.2); if(!use2011Id) { hasGoodId=true; isIso=passIsos[0]; } }
	      if( hasObjectId(ev.mn_idbits[lpid], MID_TIGHT) )    { passIds.push_back(1); passIsos[1]=(relIso<0.2); }
	      if( hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) ) { passIds.push_back(2); passIsos[2]=(relIso2011<0.15); if(use2011Id) {hasGoodId=true; isIso=passIsos[2];} }
	      if( hasObjectId(ev.mn_idbits[lpid], MID_SOFT) )     { passIds.push_back(3); passIsos[3]=true;}
	      if(use2011Id) 
		{
		  try{
		    llScaleFactor *= muonScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
		    llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
		  }catch(std::string &e){
		  }
		}
	    }
	  else
	    {
	      int wps[]={EgammaCutBasedEleId::LOOSE,EgammaCutBasedEleId::MEDIUM, EID_VBTF2011, EgammaCutBasedEleId::VETO};
	      for(int iwp=0; iwp<4; iwp++)
		{
		  if(iwp==2 && hasObjectId(ev.en_idbits[lpid], EID_VBTF2011)) 
		    { 
		      passIds.push_back(2); passIsos[2]=(relIso2011<0.10); 
		      if(use2011Id) 
			{ 
			  hasGoodId=true; isIso=passIsos[2]; 
			  try{
			    llScaleFactor *= electronScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
			    llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
			  } catch(std::string &e){
			  }
			}
		    }
		  else
		    {
		      bool passWp = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::WorkingPoint(wps[iwp]),
								(fabs(phys.leptons[ilep].eta())<1.4442),
								phys.leptons[ilep].pt(), phys.leptons[ilep].eta(),
								ev.en_detain[lpid],  ev.en_dphiin[lpid], ev.en_sihih[lpid], ev.en_hoe[lpid],
								ev.en_ooemoop[lpid], phys.leptons[ilep].d0, phys.leptons[ilep].dZ,
								0., 0., 0.,
								!hasObjectId(ev.en_idbits[lpid], EID_CONVERSIONVETO),0,ev.rho);
		      if(passWp) { 
			passIds.push_back(iwp); 
			passIsos[iwp]=(relIso<0.15);
			if(wps[iwp]==EgammaCutBasedEleId::MEDIUM && !use2011Id){  hasGoodId=true; isIso=passIsos[iwp]; }
		      }
		    }
		}
	    }
	  if(!hasGoodId)  passIdAndIso=false;
	  else if(!isIso) passIdAndIso=false;     
	
	  //fill control histograms (constrained to the Z mass)
	  if(passZmass && isSameFlavor)
	    {
	      if(matchid!=0)
		{
		  mon.fillHisto(lepStr+"genpt",tags_full, genP4.pt(), weight,true);
		  mon.fillHisto(lepStr+"geneta",tags_full, genP4.eta(), weight);
		  mon.fillHisto(lepStr+"genpu",tags_full,ev.ngenITpu, weight);
		  for(size_t iid=0; iid<passIds.size(); iid++)
		    {
		      TString idStr(lepStr);  idStr += passIds[iid];
		      mon.fillHisto(idStr+"pt",tags_full, genP4.pt(), weight,true);
		      mon.fillHisto(idStr+"eta",tags_full, genP4.eta(), weight);
		      mon.fillHisto(idStr+"pu",tags_full,ev.ngenITpu, weight);
		      if(!passIsos[ passIds[iid] ]) continue;
		      mon.fillHisto(idStr+"isopt",tags_full,  genP4.pt(), weight,true);
		      mon.fillHisto(idStr+"isoeta",tags_full, genP4.eta(), weight);
		      mon.fillHisto(idStr+"isopu",tags_full,ev.ngenITpu, weight);
		    }
		}
	  
	      mon.fillHisto(lepStr+"d0",              tags_full,fabs(phys.leptons[ilep].d0),weight);
	      mon.fillHisto(lepStr+"dZ",              tags_full,fabs(phys.leptons[ilep].dZ),weight);
	      mon.fillHisto(lepStr+"trkchi2",         tags_full,fabs(phys.leptons[ilep].trkchi2),weight);
	      mon.fillHisto(lepStr+"trkvalidpixel",   tags_full,fabs(phys.leptons[ilep].trkValidPixelHits),weight);
	      mon.fillHisto(lepStr+"trkvalidtracker", tags_full,fabs(phys.leptons[ilep].trkValidTrackerHits),weight);
	      mon.fillHisto(lepStr+"losthits",        tags_full,fabs(phys.leptons[ilep].trkLostInnerHits),weight);

	      if(lepStr=="e")
		{
		  TString reg="ee";
		  if(fabs(phys.leptons[ilep].eta())<1.442) reg="eb";
		  mon.fillHisto(lepStr+reg+"detain",  tags_full,fabs(ev.en_detain[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"dphiin",  tags_full,fabs(ev.en_dphiin[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"sihih",   tags_full,fabs(ev.en_sihih[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"sipip",   tags_full,fabs(ev.en_sipip[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"r9",      tags_full,fabs(ev.en_r9[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"hoe",     tags_full,fabs(ev.en_hoe[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"eopin",   tags_full,fabs(ev.en_eopin[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"fbrem",   tags_full,fabs(ev.en_fbrem[lpid]),weight);
		  mon.fillHisto(lepStr+reg+"ooemoop", tags_full,fabs(ev.en_ooemoop[lpid]),weight);
		}
	      else
		{
		  mon.fillHisto(lepStr+"nmuonhits",  tags_full,fabs(ev.mn_validMuonHits[lpid]),weight);
		  mon.fillHisto(lepStr+"nmatches",   tags_full,fabs(ev.mn_nMatches[lpid]),weight);
		}
	      
	      if(hasGoodId)
		{
		  mon.fillHisto(lepStr+"reliso",     tags_full, relIso,   weight);
		  mon.fillHisto(lepStr+"reliso2011", tags_full, relIso2011, weight);
		  TString lepType(matchid!=0 ? "true":"fake");
		  if(isMC)
		    {
		      mon.fillHisto(lepStr+lepType+"reliso",     tags_full, relIso,   weight);
		      mon.fillHisto(lepStr+lepType+"reliso2011", tags_full, relIso2011, weight);
		    }
		}
	    }
	}
      
      //select dilepton
      if(!passIdAndIso) continue;
      if(isMC && use2011Id) weight *= llScaleFactor*llTriggerEfficiency;

      tags_full.push_back(tag_cat);
      mon.fillHisto("eventflow",tags_full,0,weight);
      mon.fillHisto("zmass",       tags_full, zll.mass(), weight);  
  
      //now run pre-selection
      bool pass3dLeptonVeto(true);
      bool passBveto(true);
      bool passDphijmet(true);
      int nextraleptons(0);
      std::vector<LorentzVector> extraLeptonsP4;
      if(passZmass) 
	{
	  mon.fillHisto("eventflow",   tags_full, 1,            weight);
	  mon.fillHisto("zpt"      ,   tags_full, zll.pt(),     weight);      
	  mon.fillHisto("zeta"     ,   tags_full, zll.eta(),    weight);
	  mon.fillHisto("nvtx"     ,   tags_full, ev.nvtx,      weight);
	  mon.fillHisto("nvtxraw"  ,   tags_full, ev.nvtx,      1);
	  mon.fillHisto("rho"      ,   tags_full, ev.rho,       weight);
	  mon.fillHisto("rho25"    ,   tags_full, ev.rho25Neut, weight);

	  if(passZpt) 
	    {
	      mon.fillHisto  ("eventflow",tags_full,2,weight);
	      
	      //analyze dilepton kinematics
	      LorentzVector leadingLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[0]: phys.leptons[1]);
	      LorentzVector trailerLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[1]: phys.leptons[0]);
	      mon.fillHisto("leadeta"     ,   tags_full, leadingLep.eta()   ,weight);
	      mon.fillHisto("leadpt"      ,   tags_full, leadingLep.pt()    ,weight);
	      mon.fillHisto("trailereta"  ,   tags_full, trailerLep.eta()   ,weight);
	      mon.fillHisto("trailerpt"   ,   tags_full, trailerLep.pt()    ,weight);
	      
	      //
	      // 3rd LEPTON ANALYSIS
	      //
	      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++)
		{
		  //lepton type
		  bool isGood(false);
		  int lpid=phys.leptons[ilep].pid;
		  if(fabs(phys.leptons[ilep].id)==13)
		    {
		      if(!use2011Id)
			{
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT) && phys.leptons[ilep].pt()>3);
			}
		      else
			{
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT2011) && phys.leptons[ilep].pt()>3);
			}
		    }
		  else
		    {
		      if(!use2011Id)
			{
			  isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
			}
		      else
			{
			  isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
			}
		    }
		  nextraleptons += isGood;
		  
		  if(!isGood) continue;
		  extraLeptonsP4.push_back( phys.leptons[ilep] );
		  mon.fillHisto("thirdleptoneta",   tags_full,phys.leptons[ilep].eta()   ,weight);
		  mon.fillHisto("thirdleptonpt" ,   tags_full,phys.leptons[ilep].pt()     ,weight);
		}
	      
	      pass3dLeptonVeto=(nextraleptons==0);
	      mon.fillHisto("nleptons",tags_full,2+nextraleptons,weight);
	      if(pass3dLeptonVeto)
		{
		  mon.fillHisto("eventflow",tags_full,3,weight);
		  
		  //
		  //JET ANALYSIS
		  //
		  
		  //std PF
		  PhysicsObjectJetCollection aJets=variedAJets[0];
		  PhysicsObjectJetCollection aGoodIdJets;
		  LorentzVector aClusteredMetP4(zll); aClusteredMetP4 *= -1;
		  int nAJetsLoose(0), nAJetsTight(0), nAJetsPUIdLoose(0), nAJetsPUIdMedium(0);
		  int nABtags[3]={0,0,0};
		  float mindphijmet(999999.);
		  PhysicsObjectJetCollection recoilJets;
		  for(size_t ijet=0; ijet<aJets.size(); ijet++) 
		    {
		      if(aJets[ijet].pt()>30)
			{
			  float idphijmet( fabs(deltaPhi(aJets[ijet].phi(),zvvs[0].phi()) ) );
			  if(idphijmet<mindphijmet)  mindphijmet=idphijmet;
			}
		      aClusteredMetP4 -= aJets[ijet];	  
		      if(fabs(deltaPhi(aJets[ijet].phi(),zll.phi()))>2) recoilJets.push_back( aJets[ijet] );
		      
		      bool isGoodJet    =hasObjectId(aJets[ijet].pid,JETID_LOOSE);//TIGHT);
		      if(isGoodJet)
			{
			  aGoodIdJets.push_back(aJets[ijet]);
			  mon.fillHisto("pfjetpt",  tags_full, aJets[ijet].pt(),weight);
			  mon.fillHisto("pfjeteta",  tags_full, fabs(aJets[ijet].eta()),weight);
			}
		      
		      if(aJets[ijet].pt()<30) continue;
		      if(fabs(aJets[ijet].eta())<2.5) 
			{
			  nABtags[0] += (aJets[ijet].btag1>2.0);
			  nABtags[1] += (aJets[ijet].btag2>0.244);
			  nABtags[2] += (aJets[ijet].btag3>0.275);
			  Float_t ijetbtags[]={aJets[ijet].btag1, aJets[ijet].btag2, aJets[ijet].btag3};
			  for(size_t ibtag=0; ibtag<3; ibtag++)
			    {
			      if(isMC)
				{
				  if(fabs(aJets[ijet].flavid)==5) mon.fillHisto(btagAlgos[ibtag]+"bpfjetstags",     tags_full, ijetbtags[ibtag], weight);
				  else                                 mon.fillHisto(btagAlgos[ibtag]+"otherpfjetstags", tags_full, ijetbtags[ibtag], weight);
				}
			      mon.fillHisto(btagAlgos[ibtag]+"pfjetstags",  tags_full, ijetbtags[ibtag],weight);
			    }
			}
		      if(aJets[ijet].pt()<30 ) continue;
		      nAJetsLoose      += hasObjectId(aJets[ijet].pid,JETID_LOOSE);
		      nAJetsTight      += hasObjectId(aJets[ijet].pid,JETID_TIGHT);
		      nAJetsPUIdLoose  += hasObjectId(aJets[ijet].pid,JETID_OPT_LOOSE);
		      nAJetsPUIdMedium += hasObjectId(aJets[ijet].pid,JETID_OPT_MEDIUM);
		      
		      TString reg=getJetRegion(aJets[ijet].eta());
		      mon.fillHisto(reg+"pfjetbeta",     tags_full,aJets[ijet].beta,     weight);
		      mon.fillHisto(reg+"pfjetbetastar", tags_full,aJets[ijet].betaStar, weight);
		      mon.fillHisto(reg+"pfjetdrmean",   tags_full,aJets[ijet].dRMean,   weight);
		      mon.fillHisto(reg+"pfjetptd",      tags_full,aJets[ijet].ptD,      weight);
		      mon.fillHisto(reg+"pfjetptrms",    tags_full,aJets[ijet].ptRMS,    weight);
		      mon.fillHisto(reg+"pfjetmva",      tags_full,aJets[ijet].pumva,    weight);
		    }
		  
		  for(size_t ibtag=0; ibtag<3; ibtag++) mon.fillHisto("npfjetsbtags"+btagAlgos[ibtag],  tags_full, nABtags[ibtag] ,weight);
	      
		  //b-veto
		  passBveto=(nABtags[1]==0);
		  if(passBveto)
		    {
		      mon.fillHisto("eventflow",tags_full,4,weight);
		      mon.fillHisto("npfjetsvspu",          tags_full, ev.ngenITpu, nAJetsLoose,weight);
		      mon.fillHisto("npfjetstightvspu",     tags_full, ev.ngenITpu, nAJetsTight,weight);
		      mon.fillHisto("npfjetspuidloosevspu", tags_full, ev.ngenITpu, nAJetsPUIdLoose,weight);
		      mon.fillHisto("npfjetspuidmediumvspu",tags_full, ev.ngenITpu, nAJetsPUIdMedium,weight);

		      //measure jet selection efficiency
		      if(nAJetsLoose==1 && recoilJets.size()==1)
			{
			  double balance = recoilJets[0].pt()/zll.pt();
			  bool passBalanceCut(balance>0.5 && balance<1.5);
			  TString etaReg = getJetRegion(recoilJets[0].eta());
			  bool isMatched(isMC && recoilJets[0].genPt>0);
			  std::vector<TString> passIds;
			  if( hasObjectId(recoilJets[0].pid,JETID_LOOSE) )      { passIds.push_back("pfloose");   if(isMatched) passIds.push_back("truepfloose");   }
			  if( hasObjectId(recoilJets[0].pid,JETID_TIGHT) )      { passIds.push_back("pftight");   if(isMatched) passIds.push_back("truepftight");   }
			  if( hasObjectId(recoilJets[0].pid,JETID_MIN_LOOSE) )  { passIds.push_back("minloose");  if(isMatched) passIds.push_back("trueminloose");  }
			  if( hasObjectId(recoilJets[0].pid,JETID_MIN_MEDIUM) ) { passIds.push_back("minmedium"); if(isMatched) passIds.push_back("trueminmedium"); }
			  
			  mon.fillHisto("pfjetbalance",tags_full, balance,weight);
			  mon.fillHisto(etaReg+"pfjetbalance",tags_full, balance,weight);
			  if(!passBalanceCut) continue;
			  for(size_t iids=0; iids<passIds.size(); iids++)
			    {
			      mon.fillHisto(etaReg+passIds[iids]+"pfjetpt",tags_full,recoilJets[0].pt(),weight,true);
			      mon.fillHisto(passIds[iids]+"pfjeteta",tags_full,fabs(recoilJets[0].eta()),weight,true);
			    }
			}
            
		      //sub-divide for tight jets
		      int eventSubCat  = eventCategoryInst.Get(phys,&aGoodIdJets);
		      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);
		      tags_full.push_back(tag_cat+tag_subcat);
		      if(tag_subcat!="vbf") tags_full.push_back(tag_cat + "novbf");
		      mon.fillHisto("npfjets",     tags_full, nAJetsLoose,weight);

		      passDphijmet=(mindphijmet>0.5); 
		      if(zvvs[0].pt()>50) mon.fillHisto("mindphijmet",tags_full,mindphijmet,weight);
		      if(passDphijmet) 
			{
			  mon.fillHisto("eventflow",tags_full,5,weight);
			  
			  //VBF monitoring
			  float dphijj(-1),hardpt(-1);
			  if(nAJetsLoose>=2)
			    {
			      LorentzVector vbfSyst=aGoodIdJets[0]+aGoodIdJets[1];
			      LorentzVector hardSyst=vbfSyst+zvvs[0]+zll;
			      hardpt=hardSyst.pt();
			      dphijj=deltaPhi(aGoodIdJets[0].phi(),aGoodIdJets[1].phi());
			      double maxEta=max(aGoodIdJets[0].eta(),aGoodIdJets[1].eta());
			      double minEta=max(aGoodIdJets[0].eta(),aGoodIdJets[1].eta());
			      float avgEtajj=0.5*(maxEta+minEta);
			      float detajj=maxEta-minEta;
			      mon.fillHisto("pfvbfcandjetpt",     tags_full, fabs(aGoodIdJets[0].pt()),weight);
			      mon.fillHisto("pfvbfcandjetpt",     tags_full, fabs(aGoodIdJets[1].pt()),weight);
			      mon.fillHisto("pfvbfcandjeteta",     tags_full, fabs(maxEta),weight);
			      mon.fillHisto("pfvbfcandjeteta",     tags_full, fabs(minEta),weight);
			      mon.fillHisto("pfvbfcandjetdeta",     tags_full, fabs(detajj),weight);
			      mon.fillHisto("pfvbfcandzeppenfeld",     tags_full, fabs(maxEta-avgEtajj)/fabs(detajj),weight);
			      mon.fillHisto("pfvbfcandzeppenfeld",     tags_full, fabs(minEta-avgEtajj)/fabs(detajj),weight);			      
			      if(fabs(detajj)>4.0)
				{
				  mon.fillHisto("pfvbfmjj",     tags_full, vbfSyst.mass(),weight);
				  mon.fillHisto("pfvbfmjjvsdeta",     tags_full, vbfSyst.mass(),fabs(detajj),weight);
				  mon.fillHisto("pfvbfmjjvshardpt",     tags_full, vbfSyst.mass(),hardpt,weight);
				  if(vbfSyst.mass()>500) 
				    {
				      mon.fillHisto("pfvbfhardpt",     tags_full, hardpt,weight);
				      int ncjv(0);
				      float htcjv(0);
				      for(size_t iotherjet=2; iotherjet<aGoodIdJets.size(); iotherjet++)
					{
					  if(aGoodIdJets[iotherjet].pt()<30 || aGoodIdJets[iotherjet].eta()<minEta || aGoodIdJets[iotherjet].eta()>maxEta) continue;
					  htcjv+= aGoodIdJets[iotherjet].pt();
					  ncjv++;
					}
				      mon.fillHisto("pfvbfcjv",tags_full,ncjv,weight);
				      mon.fillHisto("pfvbfhtcjv",tags_full,htcjv,weight);
				    }
				}
			    }
			  
			  //
			  //SIGNAL REGION NOW : PROCEED WITH CARE
			  //
			  bool hasVbfBlinding(!isMC && runBlinded && tag_subcat=="vbf" && zvvs[0].pt()>70);
			  if(runBlinded && (mustBlind || hasVbfBlinding) ) continue;
			  
			  if(zvvs[0].pt()>70) 
			    {
			      mon.fillHisto("eventflow",tags_full,6,weight);
			      
			      mon.fillHisto("pfvbfcandjetdphi",     tags_full, fabs(dphijj),weight);
			      mon.fillHisto("pfvbfhardptfinal",     tags_full, hardpt,weight);
			     
			    }

			  METUtils::stRedMET aRedMetOut; 
			  LorentzVector aRedMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, aClusteredMetP4, zvvs[0],false,&aRedMetOut);
			  double aRedMetL=aRedMetOut.redMET_l;
			  double aRedMetT=aRedMetOut.redMET_t;
			  double aMT=METUtils::transverseMass(zll,zvvs[0],true);
			  
			  LorentzVector assocMetP4(phys.met[1]);
			  LorentzVector min3Met( min(zvvs[0], min(assocMetP4,aClusteredMetP4)) );

			  mon.fillHisto("deltaleptonpt",tags_full, leadingLep.pt()-trailerLep.pt()    ,weight);
			  mon.fillHisto("deltazpt",tags_full, zll.pt()-zvvs[0].pt(),weight);
			  mon.fillHisto("balance",tags_full, zvvs[0].pt()/zll.pt(),weight);
			  mon.fillHisto("met_met",tags_full,zvvs[0].pt(),weight);
			  mon.fillHisto("met_met_vspu",tags_full,ev.ngenITpu,zvvs[0].pt(),weight);
			  mon.fillHisto("met_metRaw",tags_full,phys.met[0].pt(),weight);
			  mon.fillHisto("met_min3Met",tags_full,min3Met.pt(),weight);
			  mon.fillHisto("met_min3Met_vspu",tags_full,ev.ngenITpu,min3Met.pt(),weight);
			  mon.fillHisto("met_redMet",tags_full,aRedMet.pt(),weight);
			  mon.fillHisto("met_redMet_vspu",tags_full,ev.ngenITpu,aRedMet.pt(),weight);
			  mon.fillHisto("met_redMetL",tags_full,aRedMetT,weight);
			  mon.fillHisto("met_redMetT",tags_full,aRedMetL,weight);
			  mon.fillHisto("mt",tags_full,aMT,weight);
			  
			}//end passDphijmet

		    }//end passBveto

		}//end pass3rdLeptonVeto
	      else
		{
		  if(nextraleptons==1 && passBveto && passDphijmet) mon.fillHisto("met_met3leptons",tags_full,zvvs[0].pt(),weight);
		}

	    }//end passZpt

	}//end passZmass


      //
      // HISTOS FOR STATISTICAL ANALYSIS (include systematic variations)
      //
      //Fill histogram for posterior optimization, or for control regions
      bool passPreselection             (passZmass && passZpt && pass3dLeptonVeto && passDphijmet && passBveto);
      bool passPreselectionMbvetoMzmass (             passZpt && pass3dLeptonVeto && passDphijmet             );          
      bool passPreselectionM3dlep       (passZmass && passZpt                     && passDphijmet && passBveto);
      for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
        float iweight = weight;                                               //nominal
        if(ivar==5)                        iweight *=TotalWeight_plus;        //pu up
        if(ivar==6)                        iweight *=TotalWeight_minus;       //pu down
        if(ivar<=10 && ivar>=7 && isMC_GG) iweight *=ev.hptWeights[ivar-6]/ev.hptWeights[0];   //ren/fact scales   

	//recompute MET/MT if JES/JER was varied
	LorentzVector zvv    = zvvs[ivar>4?0:ivar];
	float mt3(0);
	if(nextraleptons==1) mt3 = METUtils::transverseMass(extraLeptonsP4[0],zvv,false);
	PhysicsObjectJetCollection &varJets=variedAJets[ivar>4?0:ivar];
	PhysicsObjectJetCollection tightVarJets;
	LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
	for(size_t ijet=0; ijet<varJets.size(); ijet++) 
	  {
	    clusteredMetP4 -= varJets[ijet];
	    if(!hasObjectId(varJets[ijet].pid,JETID_LOOSE)) continue;
	    tightVarJets.push_back( varJets[ijet] );
	  }
	float mt = METUtils::transverseMass(zll,zvv,true);
	LorentzVector nullP4(0,0,0,0);
	LorentzVector redMet = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, zll, 0, nullP4, 0, clusteredMetP4, zvv,true);

	//re-assign the event category if jets were varied
	int eventSubCat  = eventCategoryInst.Get(phys,&tightVarJets);
	TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);
	tags_full.clear();
	tags_full.push_back(tag_cat);
	tags_full.push_back(tag_cat+tag_subcat);
	if(tag_subcat!="vbf") tags_full.push_back(tag_cat + "novbf");
	
	bool hasVbfBlinding(!isMC && runBlinded && tag_subcat=="vbf" && zvvs[0].pt()>70);
	if(runBlinded && (mustBlind || hasVbfBlinding) ) continue;
	
	//fill shapes
	for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
	  
	  if(redMet.pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
	    if(passPreselection                                        )          mon.fillHisto(TString("mt_redMet_shapes")+varNames[ivar],tags_full,index, mt,iweight);
	  
	  if(zvv.pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
	    {
	      if(passPreselection                                                    )   mon.fillHisto(TString("mt_shapes")+varNames[ivar],tags_full,index, mt,iweight);
	      if(passPreselection && passZmass10                                     )   mon.fillHisto(TString("mt_shapesZ10")+varNames[ivar],tags_full,index, mt,iweight);
	      if(passPreselection && passZmass5                                      )   mon.fillHisto(TString("mt_shapesZ5")+varNames[ivar],tags_full,index, mt,iweight);
	      
	      //                   if(passPreselectionM3dlep       && !pass3dLeptonVeto && nExtraLep==1 && passZmass3dLepton && ivar==0)   mon.fillHisto(TString("mt_shapes_3rdLepton")+varNames[ivar],tags_full,index, mt,iweight);
	      if(passPreselectionM3dlep       && !pass3dLeptonVeto && nextraleptons==1 && ivar==0)   mon.fillHisto(TString("mt_shapes_3rdLepton")+varNames[ivar],tags_full,index, mt,iweight);
	      
	      if(passPreselectionMbvetoMzmass && passZmass   && !passBveto && ivar==0)   mon.fillHisto(TString("mt_shapesBTagSB")+varNames[ivar],tags_full,index, mt,iweight);
	      if(passPreselectionM3dlep       && !pass3dLeptonVeto && nextraleptons==1   )   mon.fillHisto(TString("mt3")+varNames[ivar],tags_full,index, mt3,iweight);
	      if(passPreselectionMbvetoMzmass && passZmass         && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,0,iweight);
	      if(passPreselectionMbvetoMzmass && isZsideBand       && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,1,iweight);
	      if(passPreselectionMbvetoMzmass && isZsideBandPlus   && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,2,iweight);
	      if(passPreselectionMbvetoMzmass && passZmass         && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,3,iweight);
	      if(passPreselectionMbvetoMzmass && isZsideBand       && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,4,iweight);
	      if(passPreselectionMbvetoMzmass && isZsideBandPlus   && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,5,iweight);
	    }
	}
      }
	}

  printf("\n"); 
  file->Close();

  //##############################################
  //########     SAVING HISTO TO FILE     ########
  //##############################################
  //save control plots to file
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





