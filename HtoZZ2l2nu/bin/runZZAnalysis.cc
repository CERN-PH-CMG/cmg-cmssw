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
#include "CMGTools/HtoZZ2l2nu/interface/LeptonEfficiencySF.h"

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
  
  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");

  TString url=runProcess.getParameter<std::string>("input");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  int fType(0);
  if(url.Contains("DoubleEle")) fType=EE;
  if(url.Contains("DoubleMu"))  fType=MUMU;
  if(url.Contains("MuEG"))      fType=EMU;
  if(url.Contains("SingleMu"))  fType=MUMU;
  bool isSingleMuPD(!isMC && url.Contains("SingleMu"));  
  
  TString outTxtUrl= outUrl + "/" + outFileUrl + ".txt";
  FILE* outTxtFile = NULL;
  if(!isMC)outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());

  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //lepton efficiencies
  LeptonEfficiencySF lepEff2012;

  //jet energy scale uncertainties
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  //systematics
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  TString varNames[]={"",
		      "_jerup","_jerdown",
		      "_jesup","_jesdown",
		      "_umetup","_umetdown",
		      "_lesup","_lesdown",
		      "_puup","_pudown",
		      "_btagup","_btagdown"};
  size_t nvarsToInclude(1);
  if(runSystematics)
    {
      cout << "Systematics will be computed for this analysis" << endl;
      nvarsToInclude=sizeof(varNames)/sizeof(TString);
    }




  //##############################################
  //########    INITIATING HISTOGRAMS     ########
  //##############################################
  SmartSelectionMonitor mon;

  TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  TH1 *h=mon.addHistogram( new TH1F ("eventflow", ";;Events", 10,0,10) );
  h->GetXaxis()->SetBinLabel(1,"#geq 2 leptons");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<7");
  h->GetXaxis()->SetBinLabel(3,"p_{T}^{ll}>30");
  h->GetXaxis()->SetBinLabel(4,"b-veto"); 
  h->GetXaxis()->SetBinLabel(5,"Jet veto"); 
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>70");
  h->GetXaxis()->SetBinLabel(7,"0.4<B<1.8");
  h->GetXaxis()->SetBinLabel(8,"#Delta #phi(jet,E_{T}^{miss})>0.5");
  h->GetXaxis()->SetBinLabel(9,"3^{rd}-lepton veto");

  //pileup control
  mon.addHistogram( new TH1F( "nvtx",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "nvtxraw",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "rho",";#rho;Events",50,0,25) ); 
  mon.addHistogram( new TH1F( "rho25",";#rho(#eta<2.5);Events",50,0,25) ); 

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
  mon.addHistogram( new TH1F( "leadpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "leadeta", ";#eta^{l};Events", 50,-2.6,2.6) );
  mon.addHistogram( new TH1F( "trailerpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "trailereta", ";#eta^{l};Events", 50,-2.6,2.6) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 50,-10,10) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,40,250) );
  mon.addHistogram( new TH1F( "selsync", ";Synchronization step;Events", 4,0,4) );

  //3rd lepton control 
  mon.addHistogram( new TH1F( "nleptons", ";Leptons;Events", 3,2,4) );
  mon.addHistogram( new TH1F( "thirdleptonpt", ";p_{T}^{l};Events", 10,0,250) );
  mon.addHistogram( new TH1F( "thirdleptoneta", ";#eta^{l};Events", 10,0,2.6) );
  mon.addHistogram( new TH1F( "mt3" , ";M_{T}^{3rd lepton} [GeV/c^{2}];Events",20,0,200) );


  //jet control
  mon.addHistogram( new TH1F("jetpt"       , ";p_{T} [GeV/c];Events",50,0,250) );
  mon.addHistogram( new TH1F("jeteta"       , ";|#eta|;Events",25,0,5) );
  mon.addHistogram( new TH1F("btags", ";CSV;Events",50,-0.5,1.2) );
  h=mon.addHistogram( new TH1F("njets",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  TH1 *hb=mon.addHistogram( new TH1F("nbtags",   ";b-tag multiplicity (CSV);Events",5,0,5) );
  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      TString label("");
      if(ibin==h->GetXaxis()->GetNbins()) label +="#geq";
      else                                label +="=";
      label += (ibin-1);
      h->GetXaxis()->SetBinLabel(ibin,label);
      hb->GetXaxis()->SetBinLabel(ibin,label);
    } 
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",20,0,4) );
 

  //vbf control
  TString jetIds[]={"pf","pfpuloose","pfpumvaloose"};
  for(size_t i=0; i<sizeof(jetIds)/sizeof(TString); i++)
    {
      mon.addHistogram( new TH2F(jetIds[i]+"njetsvsavginstlumi",  ";;Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5,10,0,5000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjeteta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjetpt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet1eta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet1pt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet2eta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet2pt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjetdeta"       , ";|#Delta #eta|;Jets",50,0,10) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhardpt"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhardpt50"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      h=mon.addHistogram( new TH1F(jetIds[i]+"vbfcjv"       , ";Central jet count;Events",3,0,3) );
      h->GetXaxis()->SetBinLabel(1,"=0 jets");
      h->GetXaxis()->SetBinLabel(2,"=1 jets");
      h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhtcjv"       , ";Central jet H_{T} [GeV/c];Events",50,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfpremjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfmjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfdphijj"       , ";#Delta#phi;Events",20,0,3.5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfmjj50"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfdphijj50"       , ";#Delta#phi;Events",20,0,3.5) );
    }
 
  //MET control
  mon.addHistogram( new TH1D( "balance", ";E_{T}^{miss}/q_{T};Events", 25,0,5) );
  mon.addHistogram( new TH1F( "met_redMetSB"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet_final"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet15"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet20"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram(  new TProfile("metvsavginstlumi",  "Avg. inst lumi", 60,  400,1000));
  mon.addHistogram(  new TProfile("met15vsavginstlumi",  "Avg. inst lumi", 60,  400,1000));
  mon.addHistogram(  new TProfile("met20vsavginstlumi",  "Avg. inst lumi", 60,  400,1000));
  mon.addHistogram( new TH1F( "met_redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "mt_final"  , ";M_{T};Events", 25,0,1000) );
  mon.addHistogram( new TH1F( "met_met_final"  , ";E_{T}^{miss};Events", 25,0,250) );
  mon.addHistogram( new TH1F( "zpt_final", ";p_{T}^{ll};Events", 25,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL_final"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 25,-100,400) );

  //optimization
  std::vector<double> optim_Cuts1_met; 
  std::vector<double> optim_Cuts1_zpt;
  std::vector<double> optim_Cuts1_zmass;
  std::vector<double> optim_Cuts1_jetthr;
  for(double met=50;met<100;met+=5.0){
    if(met>80 && int(met)%10!=0)continue;
    for(double pt=30;pt<100;pt+=5){
      if(pt>60 && int(pt)%10!=0)continue;
      for(double zm=5;zm<20;zm+=2.5){
      if(zm>10 && int(2*zm)%5!=0)continue;
      for(double jetpt=10; jetpt<20; jetpt+=5)
	{
	  optim_Cuts1_met    .push_back(met);
	  optim_Cuts1_zpt    .push_back(pt);
	  optim_Cuts1_zmass  .push_back(zm);
	  optim_Cuts1_jetthr.push_back(jetpt);
	}
      }
    }
  }

  TH1F* Hoptim_cuts1_met    =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met",     ";cut index;met",     optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_zpt    =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_zpt",     ";cut index;zpt",     optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_zmass  =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_zm",      ";cut index;zmass",   optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_jetthr =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_jetthr",  ";cut index;jet thr", optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
     Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
     Hoptim_cuts1_zpt    ->Fill(index, optim_Cuts1_zpt[index]);
     Hoptim_cuts1_zmass  ->Fill(index, optim_Cuts1_zmass[index]);
     Hoptim_cuts1_jetthr ->Fill(index, optim_Cuts1_jetthr[index]);
  }
  
  TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;
  for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
    {
      Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
      mon.addHistogram( new TH2F (TString("mt_shapes")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
      mon.addHistogram( new TH2F (TString("met_shapes")+varNames[ivar],";cut index;met [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
      mon.addHistogram( new TH2F (TString("zpt_shapes")+varNames[ivar],";cut index;Z p_{T} [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
      TH2F *h=(TH2F *) mon.addHistogram( new TH2F ("nonresbckg_ctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
      h->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
      h->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
      h->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
      h->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
      h->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
      h->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");
    } 
 


   std::vector<double> optim_Cuts2_z_pt;
   std::vector<double> optim_Cuts2_jet_pt; 
   std::vector<double> optim_Cuts2_eta_gap;
   std::vector<double> optim_Cuts2_dijet_mass;
   for(double z_pt=30;z_pt<=100;z_pt+=10)
     {
       for(double jet_pt=30;jet_pt<=100;jet_pt+=10)
	 {
	   for(double eta_gap=3.5;eta_gap<=5.0;eta_gap+=0.5)
	     {
	       for(double dijet_mass=400; dijet_mass<=1000; dijet_mass+=50)
                 {
                   optim_Cuts2_z_pt.push_back(z_pt);
                   optim_Cuts2_jet_pt.push_back(jet_pt);
                   optim_Cuts2_eta_gap.push_back(eta_gap);
                   optim_Cuts2_dijet_mass.push_back(dijet_mass);
                 }
            }
       }
   } 
   TH1F* Hoptim_cuts2_z_pt=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_z_pt",";cut index;z_pt",optim_Cuts2_z_pt.size(),0,optim_Cuts2_z_pt.size())) ;
   TH1F* Hoptim_cuts2_jet_pt=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_jet_pt",";cut index;jet_pt",optim_Cuts2_jet_pt.size(),0,optim_Cuts2_jet_pt.size())) ;
   TH1F* Hoptim_cuts2_eta_gap=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_eta_gap",";cut index;eta_gap",optim_Cuts2_eta_gap.size(),0,optim_Cuts2_eta_gap.size())) ;
   TH1F* Hoptim_cuts2_dijet_mass=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_dijet_mass",";cut index;dijet_mass", optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size()));

   for(unsigned int index=0;index<optim_Cuts2_z_pt.size();index++){
     Hoptim_cuts2_z_pt->Fill(index,optim_Cuts2_z_pt[index]);   
     Hoptim_cuts2_jet_pt->Fill(index,optim_Cuts2_jet_pt[index]); 
     Hoptim_cuts2_eta_gap->Fill(index,optim_Cuts2_eta_gap[index]);
     Hoptim_cuts2_dijet_mass->Fill(index,optim_Cuts2_dijet_mass[index]);
   }

   for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
     //     mon.addHistogram( new TH2F (TString("dijet_mass_shapes")+varNames[ivar],";cut index;M_{Jet1,Jet2} [GeV];#events (/50GeV)",optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size(),200,0.,1200) );
     mon.addHistogram( new TH2F (TString("dijet_mass_shapes")+varNames[ivar],";cut index;M_Z [GeV];#events (/1GeV)",optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size(),120,0,3000) );
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
  bool useObservedPU(true);
  if(isMC){
    TString puDist("evAnalyzer/h2zz/pileuptrue");
    if(useObservedPU) puDist="evAnalyzer/h2zz/pileup";
    TH1F* histo = (TH1F *) file->Get(puDist);
    if(!histo)std::cout<<"pileup histogram is null!!!\n";
    for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
    delete histo;
    if(dataPileupDistribution.size()==0) dataPileupDistribution=mcPileupDistribution;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights=0;
  PuShifter_t PuShifters;
  //  reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC){
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PuShifters=getPUshifters(dataPileupDistribution,0.05);
  }

  //event Categorizer
  EventCategory eventCategoryInst(4); //0,>=1jet, VBF


  //##############################################
  //########           EVENT LOOP         ########
  //##############################################
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  DuplicatesChecker duplicatesChecker;
  int nDuplicates(0);
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      
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
      if(!isMC){
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
        TotalWeight_plus  = PuShifters[PUUP]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
        TotalWeight_minus = PuShifters[PUDOWN]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
      }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);
      Hcutflow->Fill(5,signalWeight);

      //apply JER base corrections to jets (and compute associated variations)
      std::vector<PhysicsObjectJetCollection> variedAJets;
      LorentzVectorCollection zvvs;
      METUtils::computeVariation(phys.ajets, phys.leptons, phys.met[0], variedAJets, zvvs, &jecUnc);

      //
      //
      // BELOW FOLLOWS THE ANALYSIS OF THE MAIN SELECTION WITH N-1 PLOTS
      //
      //
      
      //
      // LEPTON ANALYSIS
      //
      LorentzVector lep1=phys.leptons[0];
      LorentzVector lep2=phys.leptons[1];
      LorentzVector zll(lep1+lep2);
      bool passIdAndIso(true);
      bool passZmass(fabs(zll.mass()-91)<7);
      bool passZsideBand( (zll.mass()>40 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200));
      //      bool passZpt(zll.pt()>30);
      bool passZpt(zll.pt()>50 && fabs(zll.pt())<1.4442);
      
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
	    phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid]);
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
	      else
		{
		  llScaleFactor *= lepEff2012.getLeptonEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),13).first;
		  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
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
		      if(!use2011Id)
			{
			  llScaleFactor *= lepEff2012.getLeptonEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),11).first;
			  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
			}
		    }
		}
	    }
	  if(!hasGoodId)  passIdAndIso=false;
	  else if(!isIso) passIdAndIso=false;     

	  //fill control histograms (constrained to the Z mass)
	  if(passZmass && isSameFlavor){
	      if(matchid!=0){
		  mon.fillHisto(lepStr+"genpt",tags_full, genP4.pt(), weight,true);
		  mon.fillHisto(lepStr+"geneta",tags_full, genP4.eta(), weight);
		  mon.fillHisto(lepStr+"genpu",tags_full,ev.ngenITpu, weight);
		  for(size_t iid=0; iid<passIds.size(); iid++){
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

	      if(lepStr=="e"){
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
	      }else{
		  mon.fillHisto(lepStr+"nmuonhits",  tags_full,fabs(ev.mn_validMuonHits[lpid]),weight);
		  mon.fillHisto(lepStr+"nmatches",   tags_full,fabs(ev.mn_nMatches[lpid]),weight);
	      }
	      
	      if(hasGoodId){
		  mon.fillHisto(lepStr+"reliso",     tags_full, relIso,   weight);
		  mon.fillHisto(lepStr+"reliso2011", tags_full, relIso2011, weight);
		  TString lepType(matchid!=0 ? "true":"fake");
		  if(isMC){
		      mon.fillHisto(lepStr+lepType+"reliso",     tags_full, relIso,   weight);
		      mon.fillHisto(lepStr+lepType+"reliso2011", tags_full, relIso2011, weight);
		  }
	      }
          }
      }
      //select dilepton
      if(!passIdAndIso) continue;
      if(isMC && use2011Id) weight *= llScaleFactor*llTriggerEfficiency;

      //
      // 3rd LEPTON ANALYSIS
      //
      int nextraleptons(0);
      std::vector<LorentzVector> extraLeptonsP4;
      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++){
	//lepton type
	bool isGood(false);
	int lpid=phys.leptons[ilep].pid;
	if(fabs(phys.leptons[ilep].id)==13){
	  if(!use2011Id){
	    isGood = (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2);
	  }else{
	    isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
	  }
	}else{
	  if(!use2011Id){
	    isGood = ( hasObjectId(ev.en_idbits[lpid],EID_LOOSE) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid])<0.15 && phys.leptons[ilep].pt()>10);
	  }else{
	    isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
	  }
	    }
	nextraleptons += isGood;
	
	if(!isGood) continue;
	extraLeptonsP4.push_back( phys.leptons[ilep] );
      }
      bool pass3dLeptonVeto(nextraleptons==0);
      
      //
      //JET ANALYSIS
      //
      PhysicsObjectJetCollection aJets= variedAJets[0];
      PhysicsObjectJetCollection aJets30;
      PhysicsObjectJetCollection aGoodIdJets;
      LorentzVector aClusteredMetP4(zll); aClusteredMetP4 *= -1;
      LorentzVector aClustered15MetP4(aClusteredMetP4),aClustered20MetP4(aClusteredMetP4);
      int nAJetsLoose(0), nABtags(0);
      float mindphijmet(999999.),mindphijmet20(999999.);
      for(size_t ijet=0; ijet<aJets.size(); ijet++) 
	{
	  float idphijmet( fabs(deltaPhi(aJets[ijet].phi(),zvvs[0].phi()) ) );
	  if(aJets[ijet].pt()>20 && idphijmet<mindphijmet20)  mindphijmet20=idphijmet;
	  if(aJets[ijet].pt()>30 && idphijmet<mindphijmet)    mindphijmet=idphijmet;
	      
	  aClusteredMetP4 -= aJets[ijet];	  
	  if(aJets[ijet].pt()>15) aClustered15MetP4 -= aJets[ijet];
	  if(aJets[ijet].pt()>20) aClustered20MetP4 -= aJets[ijet];
	  
	  bool isGoodJet    =hasObjectId(aJets[ijet].pid,JETID_LOOSE);//TIGHT);
	  if(isGoodJet)
	    {
	      aGoodIdJets.push_back(aJets[ijet]);
	      if(passZmass && passZpt)
		{
		  mon.fillHisto("jetpt",  tags_full, aJets[ijet].pt(),weight);
		  mon.fillHisto("jeteta",  tags_full, fabs(aJets[ijet].eta()),weight);
		}
	   
	  
	      if(aJets[ijet].pt()<20) continue;
	  
	      //b-tagging
	      if(fabs(aJets[ijet].eta())<2.5) 
		{
		  nABtags += (aJets[ijet].btag2>0.244);
		  if(passZmass && passZpt)	mon.fillHisto("btags",  tags_full, aJets[ijet].btag2,weight);
		}
	      
	      //jet counting
	      if(aJets[ijet].pt()<30 ) continue;
	      nAJetsLoose++;
	      aJets30.push_back(aJets[ijet]);
	    }
	}
      bool passBveto(nABtags==0);
      //bool passJetVeto(nAJetsLoose<=1);
      
      //
      // MET ANALYSIS
      //
      METUtils::stRedMET aRedMetOut; 
      LorentzVector aRedMet15=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, aClustered15MetP4, zvvs[0],false,&aRedMetOut);
      LorentzVector aRedMet20=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, aClustered20MetP4, zvvs[0],false,&aRedMetOut);
      LorentzVector aRedMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, aClusteredMetP4, zvvs[0],false,&aRedMetOut);
      double aRedMetL=aRedMetOut.redMET_l;
      double aRedMetT=aRedMetOut.redMET_t;
      float balance=zvvs[0].pt()/zll.pt();
      if(nAJetsLoose>0)
	{
	  //met+zll+sum jets ~ 0 for a boosted ZZ
	  LorentzVector recoil=zvvs[0];
	  for(size_t ijet=0; ijet<=aGoodIdJets.size(); ijet++) recoil+=aGoodIdJets[ijet];
	  balance=recoil.pt()/zll.pt();
	}
      bool passBalance(balance>0.4 && balance<1.8);
      bool passDphijmet(mindphijmet>0.5);
      if(nAJetsLoose==0) passDphijmet=(mindphijmet20>0.5);
      bool passRedMet(aRedMet.pt()>70);
      double dphil1met=fabs(deltaPhi(lep1.phi(),zvvs[0].phi()));
      double dphil2met=fabs(deltaPhi(lep2.phi(),zvvs[0].phi()));
      bool passLeptonMisReconstruction(use2011Id || zvvs[0].pt()<60 || (zvvs[0].pt()>60 && min(dphil1met,dphil2met)>0.2));

      float mt = METUtils::transverseMass(zll,zvvs[0],true);      
     
      //now fill control distributions
      tags_full.push_back(tag_cat);
      mon.fillHisto("eventflow",tags_full,0,weight);
      mon.fillHisto("zmass",       tags_full, zll.mass(), weight);  
 
      //
      // EVENT SELECTION
      //
      if(passZmass){

	//basic Z and pu control
	mon.fillHisto("eventflow",   tags_full, 1,            weight);
	mon.fillHisto("zpt"      ,   tags_full, zll.pt(),     weight);      
	mon.fillHisto("zeta"     ,   tags_full, zll.eta(),    weight);
	mon.fillHisto("nvtx"     ,   tags_full, ev.nvtx,      weight);
	mon.fillHisto("nvtxraw"  ,   tags_full, ev.nvtx,      1);
	mon.fillHisto("rho"      ,   tags_full, ev.rho,       weight);
	mon.fillHisto("rho25"    ,   tags_full, ev.rho25Neut, weight);
	
	//sync with g+jets
	mon.fillHisto("selsync",tags_full,0,weight);
	if(passZpt){
	  mon.fillHisto("selsync",tags_full,1,weight);
	  if(pass3dLeptonVeto){
	    mon.fillHisto("selsync",tags_full,2,weight);
	    if(passBveto) mon.fillHisto("selsync",tags_full,3,weight);
	  }
	}
	
	if(passZpt){
	  mon.fillHisto  ("eventflow",tags_full,2,weight);
	  
	  //analyze dilepton kinematics
	  LorentzVector leadingLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[0]: phys.leptons[1]);
	  LorentzVector trailerLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[1]: phys.leptons[0]);
	  mon.fillHisto("leadeta"     ,   tags_full, leadingLep.eta()   ,weight);
	  mon.fillHisto("leadpt"      ,   tags_full, leadingLep.pt()    ,weight);
	  mon.fillHisto("trailereta"  ,   tags_full, trailerLep.eta()   ,weight);
	  mon.fillHisto("trailerpt"   ,   tags_full, trailerLep.pt()    ,weight);

	  mon.fillHisto("nbtags",  tags_full, nABtags,weight);
	  if(passBveto)
	    {

	      //analyze also 3 lepton category
	      mon.fillHisto("nleptons",tags_full,2+nextraleptons,weight);
	      if(nextraleptons==1) 
		{
		  mon.fillHisto("thirdleptoneta",   tags_full,fabs(extraLeptonsP4[0].eta())   ,weight);
		  mon.fillHisto("thirdleptonpt" ,   tags_full,extraLeptonsP4[0].pt()     ,weight);
		  mon.fillHisto("mt3",tags_full,METUtils::transverseMass(extraLeptonsP4[0],zvvs[0],false),weight);
		}
	  

    
	      mon.fillHisto("eventflow",tags_full,3,weight);
	      mon.fillHisto("njets",    tags_full, nAJetsLoose,weight);
	      //if(passJetVeto)
	      //{
	      int evcat=eventCategoryInst.Get(phys,&aGoodIdJets);
	      tags_full.push_back(tag_cat+eventCategoryInst.GetLabel(evcat));

	      mon.fillHisto("eventflow",tags_full,4,weight);

              //VBF monitoring
	      for(size_t ijetid=0; ijetid<sizeof(jetIds)/sizeof(TString); ijetid++)
		{
		  int jetIdToApply(JETID_LOOSE);
		  if(jetIds[ijetid]=="pfpuloose")    jetIdToApply=JETID_CUTBASED_LOOSE;
		  if(jetIds[ijetid]=="pfpumvaloose") jetIdToApply=JETID_OPT_LOOSE;
		  int njetsPass=0;
		  for(size_t ijet=0;ijet<aGoodIdJets.size(); ijet++)
		    {
		      if(!hasObjectId(aGoodIdJets[ijet].pid,jetIdToApply)) continue;
		      njetsPass++;
		    }
		  mon.fillHisto(jetIds[ijetid]+"njetsvsavginstlumi", tags_full, njetsPass,ev.curAvgInstLumi,weight);
		}
              float dphijj(-1),hardpt(-1);
              if(nAJetsLoose>=2){

		for(size_t ijetid=0; ijetid<sizeof(jetIds)/sizeof(TString); ijetid++)
		  {
		    int jetIdToApply(JETID_LOOSE);
		    if(jetIds[ijetid]=="pfpuloose")    jetIdToApply=JETID_CUTBASED_LOOSE;
		    if(jetIds[ijetid]=="pfpumvaloose") jetIdToApply=JETID_OPT_LOOSE;
		    if(!hasObjectId(aGoodIdJets[0].pid,jetIdToApply) || !hasObjectId(aGoodIdJets[0].pid,jetIdToApply)) continue; 
		    
		    LorentzVector vbfSyst=aGoodIdJets[0]+aGoodIdJets[1];
		    LorentzVector hardSyst=vbfSyst+zll; //+zvvs[0]
		    hardpt=hardSyst.pt();
		    dphijj=deltaPhi(aGoodIdJets[0].phi(),aGoodIdJets[1].phi());
		    double maxEta=max(aGoodIdJets[0].eta(),aGoodIdJets[1].eta());
		    double minEta=min(aGoodIdJets[0].eta(),aGoodIdJets[1].eta());
		    float detajj=maxEta-minEta;
		    mon.fillHisto(jetIds[ijetid]+"vbfcandjetpt",  tags_full, fabs(aGoodIdJets[0].pt()),weight);
		    mon.fillHisto(jetIds[ijetid]+"vbfcandjetpt",  tags_full, fabs(aGoodIdJets[1].pt()),weight);
		    mon.fillHisto(jetIds[ijetid]+"vbfcandjet1pt", tags_full, max(aGoodIdJets[0].pt(),aGoodIdJets[1].pt()),weight);
		    mon.fillHisto(jetIds[ijetid]+"vbfcandjet2pt", tags_full, min(aGoodIdJets[0].pt(),aGoodIdJets[1].pt()),weight);
		    if(aGoodIdJets[0].pt()>30 && aGoodIdJets[1].pt()>30){
		      int ncjv(0);
		      float htcjv(0);
		      for(size_t iotherjet=2; iotherjet<aGoodIdJets.size(); iotherjet++){
                        if(aGoodIdJets[iotherjet].pt()<30 || aGoodIdJets[iotherjet].eta()<minEta || aGoodIdJets[iotherjet].eta()>maxEta) continue;
			if(!hasObjectId(aGoodIdJets[iotherjet].pid,jetIdToApply)) continue;
                        htcjv+= aGoodIdJets[iotherjet].pt();
                        ncjv++;
		      }
		      mon.fillHisto(jetIds[ijetid]+"vbfcjv",tags_full,ncjv,weight);
		      mon.fillHisto(jetIds[ijetid]+"vbfhtcjv",tags_full,htcjv,weight);
		      
		      if(ncjv==0){
                        mon.fillHisto(jetIds[ijetid]+"vbfcandjet1eta",      tags_full, max(fabs(maxEta),fabs(minEta)),weight);
			mon.fillHisto(jetIds[ijetid]+"vbfcandjet2eta",      tags_full, min(fabs(maxEta),fabs(minEta)),weight);
			mon.fillHisto(jetIds[ijetid]+"vbfcandjeteta",      tags_full, fabs(maxEta),weight);
                        mon.fillHisto(jetIds[ijetid]+"vbfcandjeteta",      tags_full, fabs(minEta),weight);
                        mon.fillHisto(jetIds[ijetid]+"vbfcandjetdeta",     tags_full, fabs(detajj),weight);
			mon.fillHisto(jetIds[ijetid]+"vbfpremjj",          tags_full, vbfSyst.mass(),weight);
                        if(fabs(detajj)>4.5){
			  mon.fillHisto(jetIds[ijetid]+"vbfmjj",          tags_full, vbfSyst.mass(),weight);
			  if(vbfSyst.mass()>450){
			    mon.fillHisto(jetIds[ijetid]+"vbfhardpt",     tags_full, hardpt,weight);
			    mon.fillHisto(jetIds[ijetid]+"vbfdphijj",     tags_full, fabs(dphijj),weight);
			  }
			}

			if(aGoodIdJets[0].pt()>50 && aGoodIdJets[1].pt()>50 && fabs(detajj)>3.5)
			  {
			    mon.fillHisto(jetIds[ijetid]+"vbfmjj50",          tags_full, vbfSyst.mass(),weight);
			    if(vbfSyst.mass()>800)
			      {
				mon.fillHisto(jetIds[ijetid]+"vbfhardpt50",     tags_full, hardpt,weight);
				mon.fillHisto(jetIds[ijetid]+"vbfdphijj50",     tags_full, fabs(dphijj),weight);
				if(hardpt>10 && hardpt<20 && outTxtFile)
				  {
				    fprintf(outTxtFile,
					    "%d:%d:%d @ %s hardPt=%f Mjj=%f pt1=%f pt2=%f nvtx=%d\n",
					    ev.run,ev.lumi,ev.event,
					    url.Data(),hardpt,vbfSyst.mass(),
					    aGoodIdJets[0].pt(),aGoodIdJets[1].pt(),
					    ev.nvtx);
				  }
			      }
			  }
		      }
		    }
		  }
	      }
	      //end VBF control
	      
	      if(passLeptonMisReconstruction)
		{
		  mon.fillHisto("met_met",tags_full,zvvs[0].pt(),weight);
		  mon.fillHisto("met_redMet",tags_full,aRedMet.pt(),weight);
		  mon.fillHisto("met_redMet15",tags_full,aRedMet15.pt(),weight);
		  mon.fillHisto("met_redMet20",tags_full,aRedMet20.pt(),weight);
		  mon.fillProfile("metvsrho",  tags_full, ev.rho, aRedMet.pt(), weight);
		  mon.fillProfile("met15vsrho",  tags_full, ev.rho, aRedMet15.pt(), weight);
		  mon.fillProfile("met20vsrho",  tags_full, ev.rho, aRedMet20.pt(), weight);
		  mon.fillHisto("met_redMetL",tags_full,aRedMetT,weight);
		  mon.fillHisto("met_redMetT",tags_full,aRedMetL,weight);		  
		  
		  if(passRedMet)
		    {
		      mon.fillHisto("eventflow",tags_full,5,weight);
		      mon.fillHisto("balance",tags_full, balance, weight);
		      
		      if(passBalance)
			{
			  mon.fillHisto("eventflow",tags_full,6,weight);
			  mon.fillHisto("mindphijmet",tags_full,nAJetsLoose==0 ? mindphijmet20:mindphijmet,weight);
			  
			  if(passDphijmet)
			    {
			      mon.fillHisto("eventflow",tags_full,7,weight);
			      if(pass3dLeptonVeto)
				{
				  mon.fillHisto("eventflow",tags_full,8,weight);
				  mon.fillHisto("mt_final",tags_full,mt,weight);
				  mon.fillHisto("met_redMet_final",tags_full,aRedMet.pt(),weight);
				  mon.fillHisto("met_redMetL_final",tags_full,aRedMetL,weight);
				  mon.fillHisto("met_met_final",tags_full,zvvs[0].pt(),weight);
				  mon.fillHisto("zpt_final",tags_full,zll.pt(),weight);
				}
			    }
			}
		    }
		}
	      //}
	    }
	}
      }
      
      //other control regions
      if(passZsideBand && passZpt && !passBveto) mon.fillHisto("met_redMetSB",tags_full,aRedMet.pt(),weight);

      //
      // HISTOS FOR STATISTICAL ANALYSIS (include systematic variations, selection is repeated for each variation)
      //
      //FIXME
       for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
         float iweight = weight;                                               //nominal
	 if(ivar==9)                         iweight *=TotalWeight_plus;        //pu up
         if(ivar==10)                        iweight *=TotalWeight_minus;       //pu down

	 //recompute MET/MT if JES/JER was varied
	 LorentzVector zvv    = zvvs[ivar>8 ? 0 : ivar];
	 PhysicsObjectJetCollection &varJets=variedAJets[ivar>4 ? 0  : ivar];
	 PhysicsObjectJetCollection tightVarJets,tightVarJets30;
	 LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
	 LorentzVector clusteredMet15P4(clusteredMetP4),clusteredMet20P4(clusteredMetP4);
	 int nAJetsLoose(0),nABtags(0);
	 float mindphijmet(999999.),mindphijmet20(999999.);
	 for(size_t ijet=0; ijet<varJets.size(); ijet++){
	   clusteredMetP4 -= varJets[ijet];
	   if(varJets[ijet].pt()>15) clusteredMet15P4 -= varJets[ijet];
	   if(varJets[ijet].pt()>20) clusteredMet20P4 -= varJets[ijet];
	   if(!hasObjectId(varJets[ijet].pid,JETID_LOOSE)) continue;
	   float idphijmet( fabs(deltaPhi(varJets[ijet].phi(),zvvs[0].phi()) ) );
	   if(varJets[ijet].pt()>20 && idphijmet<mindphijmet20)  mindphijmet20=idphijmet;
	   if(varJets[ijet].pt()>30 && idphijmet<mindphijmet)    mindphijmet=idphijmet;
	   tightVarJets.push_back( varJets[ijet] );
	   if(varJets[ijet].pt()>30)
	     {
	       nAJetsLoose ++;
	       tightVarJets30.push_back(varJets[ijet]); 
	     }
	   if(varJets[ijet].pt()<20 || fabs(varJets[ijet].eta())>2.5)continue;
	   if(ivar==11)      nABtags += (varJets[ijet].btag2>0.250);
	   else if(ivar==12) nABtags += (varJets[ijet].btag2>0.240);
	   else              nABtags += (varJets[ijet].btag2>0.244);
	 }
	 float mt = METUtils::transverseMass(zll,zvv,true);
	 METUtils::stRedMET redMetInfo;
	 LorentzVector redMet15=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0., lep2, 0., clusteredMet15P4, zvv,false,&redMetInfo);
	 LorentzVector redMet20=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0., lep2, 0., clusteredMet20P4, zvv,false,&redMetInfo);
	 LorentzVector redMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0., lep2, 0., clusteredMetP4, zvv,false,&redMetInfo);
	 float balance=zvv.pt()/zll.pt();
	 if(nAJetsLoose>0)
	   {
	     //met+zll+sum jets ~ 0 for a boosted ZZ
	     LorentzVector recoil=zvv;
	     for(size_t ijet=0; ijet<=tightVarJets.size(); ijet++) recoil+=tightVarJets[ijet];
	     balance = recoil.pt()/zll.pt();
	   }

	 //re-assign the event category;
	 int evcat=eventCategoryInst.Get(phys,&varJets);
	 tags_full.clear();
	 tags_full.push_back(tag_cat);
	 tags_full.push_back(tag_cat+eventCategoryInst.GetLabel(evcat));


	 bool isZsideBand( (zll.mass()>40 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200));
	 bool isZsideBandPlus( (zll.mass()>110 && zll.mass()<200));	 
	 bool passLocalBveto(nABtags==0);
	 //bool passLocalJetVeto(nAJetsLoose==0 || nAJetsLoose==1);
	 bool passLocalBalance(balance>0.4 && balance<1.8);
	 bool passLocalDphijmet(mindphijmet>0.5);
	 if(nAJetsLoose==0) passDphijmet=(mindphijmet20>0.5);
	 bool passLeptonMisReconstruction(use2011Id || zvvs[0].pt()<60 || (zvvs[0].pt()>60 && min(dphil1met,dphil2met)>0.2));  //this is a patch - use always raw MET
	 if(!passLeptonMisReconstruction) continue;
	 //  if(passLocalJetVeto)
	 // 	   {
	 // 	     if(nAJetsLoose==0) tags_full.push_back(tag_cat+"eq0jets");	   
	 // 	     if(nAJetsLoose==1) tags_full.push_back(tag_cat+"eq1jets");	   
// 	   }
	 for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
	   float minMet=optim_Cuts1_met[index];
	   float minZpt=optim_Cuts1_zpt[index];
	   float deltaZ=optim_Cuts1_zmass[index];
	   float jetthr=optim_Cuts1_jetthr[index];
	   bool passLocalMet(redMet.pt()>minMet);
	   if(jetthr==15) passLocalMet=(redMet15.pt()>minMet);
	   if(jetthr==20) passLocalMet=(redMet20.pt()>minMet);
	   bool passLocalZmass(fabs(zll.mass()-91)<deltaZ);
	   bool passLocalZpt(zll.pt()>minZpt);
	   bool passLocalPreselection            (pass3dLeptonVeto && passLocalDphijmet && passLocalBveto /*&& passLocalJetVeto*/ && passLocalBalance && passLocalZmass && passLocalZpt && passLocalMet);
	   bool passLocalPreselectionMbvetoMzmass(pass3dLeptonVeto && passLocalDphijmet                   /*&& passLocalJetVeto*/ && passLocalBalance                   && passLocalZpt && passLocalMet);
	   if(passLocalPreselection)
	     {
	       mon.fillHisto(TString("met_shapes")+varNames[ivar],tags_full,index,redMet.pt(),iweight);
	       mon.fillHisto(TString("mt_shapes")+varNames[ivar],tags_full,index,mt,iweight);
	       mon.fillHisto(TString("zpt_shapes")+varNames[ivar],tags_full,index,zll.pt(),iweight);
	     }
	   
	   if(passLocalPreselectionMbvetoMzmass && passLocalZmass    && passLocalBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,0,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBand       && passLocalBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,1,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBandPlus   && passLocalBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,2,iweight);
	   if(passLocalPreselectionMbvetoMzmass && passLocalZmass    && !passLocalBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,3,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBand       && !passLocalBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,4,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBandPlus   && !passLocalBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,5,iweight);
	 }

	 if(varJets.size()>1){

	   int ncjv(0);
	   double maxEta=max(varJets[0].eta(),varJets[1].eta());
	   double minEta=min(varJets[0].eta(),varJets[1].eta());
	   for(size_t iotherjet=2; iotherjet<varJets.size(); iotherjet++){
	     if(varJets[iotherjet].pt()<30 || varJets[iotherjet].eta()<minEta || varJets[iotherjet].eta()>maxEta)continue;
	     ncjv++;
	   }
	   
	   for(unsigned int index=0; index<optim_Cuts2_z_pt.size();index++){
	     if(!hasObjectId(varJets[0].pid,JETID_CUTBASED_LOOSE) || !hasObjectId(varJets[1].pid,JETID_CUTBASED_LOOSE)) continue;
	     float minZPt=optim_Cuts2_z_pt[index];
	     float minJetPt=optim_Cuts2_jet_pt[index];
	     float minHardPt    = 99999.;//optim_Cuts2_jet2_pt[index];
	     float minEtaGap=optim_Cuts2_eta_gap[index];
	     float minDijetMass=optim_Cuts2_dijet_mass[index];
	     bool passLocalZmass(fabs(zll.mass()-91)<15);
	     bool passLocalZpt(zll.pt()>minZPt && fabs(zll.eta())<1.4442); 
	     bool passLocalRedMet(aRedMet.pt()>60);
	     bool passLocalJet1Pt(varJets[0].pt()>minJetPt);
	     bool passLocalJet2Pt(varJets[1].pt()>minJetPt);
	     bool passLocalEtaGap(fabs(varJets[0].eta()-varJets[1].eta())>minEtaGap);
	     LorentzVector vbfSyst=varJets[0]+varJets[1];
	     bool passLocalDijetMass(vbfSyst.M()>minDijetMass);
	     LorentzVector hardSyst=vbfSyst+zll; //+zvvs[0]                                                                                                                                                
	     float hardpt=hardSyst.pt();
	     bool passHardPt(hardpt<minHardPt);

	     if(passLocalJet1Pt && passLocalJet2Pt && passLocalEtaGap && passLocalDijetMass && passLocalZmass && passLocalZpt && ncjv==0 && pass3dLeptonVeto && passLocalBveto){
	       mon.fillHisto(TString("dijet_mass_shapes")+varNames[ivar],tags_full,index,vbfSyst.M(),iweight);
	     }
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
  outUrl += outFileUrl + ".root";
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





