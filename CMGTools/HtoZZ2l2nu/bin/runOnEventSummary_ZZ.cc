#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

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
#include "CMGTools/HtoZZ2l2nu/interface/BtagUncertaintyComputer.h"
// Trigger/Off-line recostruction Efficiency
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
#include "TRandom.h"
#include "TLorentzVector.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

double ErrEt( double Et, double Eta);

int main(int argc, char* argv[])
{
  //##############################################
  //########    GLOBAL INITIALIZATION     ########
  //##############################################

  // For random number
  TRandom *Gaussian = new TRandom();

  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);

  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");
 
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

  //handler for gamma processes
  GammaEventHandler *gammaEvHandler=0;
  if(mctruthmode==22){
     isMC=false;
     gammaEvHandler = new GammaEventHandler(runProcess);
  }

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
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");

  btag::UncertaintyComputer bcomp(0.837, 0.95, 0.06, 0.286, 1.15, 0.11);
  if(runSystematics) { cout << "Systematics will be computed for this analysis" << endl; }


  //isMC_VBF sample ?
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isMC_GG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isMC_VBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  std::vector<TGraph *> hWeightsGrVec;
  if(isMC_GG){
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
    TFile *fin=TFile::Open("~psilva/public/HtoZZ/HiggsQtWeights.root");
    if(fin){
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

  TString varNames[]={"","_jesup","_jesdown","_jerup","_jerdown","_puup","_pudown","_renup","_rendown","_factup","_factdown","_btagup","_btagdown"};
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

  //book the control histograms
  TH1F *h=new TH1F ("eventflow", ";Step;Events", 8,0,8);
  h->GetXaxis()->SetBinLabel(2,"Preselected");
  h->GetXaxis()->SetBinLabel(3,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(4,"b-veto");
  h->GetXaxis()->SetBinLabel(5,"Z_{pT}>25");
  h->GetXaxis()->SetBinLabel(6,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(7,"red-E_{T}^{miss}>medium");
  h->GetXaxis()->SetBinLabel(8,"red-E_{T}^{miss}>tight");
  mon.addHistogram( h );
  //Mine Event Flow    
  h=new TH1F ("ZZ_eventflow", ";;Events", 9,0,9);
  h->GetXaxis()->SetBinLabel(1,"Preselected");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<10");
  h->GetXaxis()->SetBinLabel(3,"Z_{pT}>30");
  h->GetXaxis()->SetBinLabel(4,"b-veto (cvs$<$0.244)");
  h->GetXaxis()->SetBinLabel(5,"Jet-Veto (pt$>$30)");
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>60");
  h->GetXaxis()->SetBinLabel(7,"Balance Cut");
  h->GetXaxis()->SetBinLabel(8,"Delta phi (Jet-Met)");
  h->GetXaxis()->SetBinLabel(9,"3^{rd}-lepton veto");
  //h->GetXaxis()->SetBinLabel(10,"Opp. flavor");
  mon.addHistogram( h );
  //No Weight
  h=new TH1F ("eventflow_NoWeight", ";;Events", 9,0,9);
  h->GetXaxis()->SetBinLabel(1,"Preselected");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(3,"Z_{pT}>30");
  h->GetXaxis()->SetBinLabel(4,"b-veto (cvs$<$0.244)");
  h->GetXaxis()->SetBinLabel(5,"Jet-Veto (pt$>$30)");
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>50");
  h->GetXaxis()->SetBinLabel(7,"Balance Cut");
  h->GetXaxis()->SetBinLabel(8,"Delta phi (Jet-Met)");
  h->GetXaxis()->SetBinLabel(9,"3^{rd}-lepton veto");
  mon.addHistogram( h );
  //Reduced for Plot
  h=new TH1F ("ZZ_eventflow_red", ";;Events", 7,0,7);
  h->GetXaxis()->SetBinLabel(1,"Pres, |M-M_{Z}|<10");
  h->GetXaxis()->SetBinLabel(2,"Z_{pT}>30");
  h->GetXaxis()->SetBinLabel(3,"b-veto (cvs$<$0.244)");
  h->GetXaxis()->SetBinLabel(4,"Jet-Veto (pt$>$30)");
  h->GetXaxis()->SetBinLabel(5,"red-E_{T}^{miss}>60");
  h->GetXaxis()->SetBinLabel(6,"Bal.Cut, Dphi");
  h->GetXaxis()->SetBinLabel(7,"3^{rd}-lept veto");
  mon.addHistogram( h );

  //Trigger Systematic-Scale
  h=new TH1F ("eventflow_Sys", ";;Events", 4,0,4);
  h->GetXaxis()->SetBinLabel(1,"Trigger Up");
  h->GetXaxis()->SetBinLabel(2,"Trigger Down");
  h->GetXaxis()->SetBinLabel(3,"Scale Up");
  h->GetXaxis()->SetBinLabel(4,"Scale Down");
  mon.addHistogram( h );

  //Pt Bin Event Flow
  h=new TH1F ("ZZ_eventflow_ptBin", ";;Events", 4,0,4);
  h->GetXaxis()->SetBinLabel(1,"70<Pt(Z)<90");
  h->GetXaxis()->SetBinLabel(2,"90<Pt(Z)<110");
  h->GetXaxis()->SetBinLabel(3,"110<Pt(Z)<140");
  h->GetXaxis()->SetBinLabel(4,"Pt(Z)>140");
  mon.addHistogram( h );

  h=new TH1F ("eventSubCat", ";Event Category;Events", 4,0,4);
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
  h->GetXaxis()->SetBinLabel(4,"VBF");
  mon.addHistogram( h );

  //vertex
  mon.addHistogram( new TH1F("nvtx",";Vertices;Events",25,0,25) );
  mon.addHistogram( new TH1F("nvtxPlus",";Vertices;Events",25,0,25) );
  mon.addHistogram( new TH1F("nvtxMinus",";Vertices;Events",25,0,25) );

  //pileup
  mon.addHistogram( new TH2F ("itpuvsootpu", ";In-Time Pileup; Out-of-time Pileup;Events", 30,0,30,30,0,30) );
  TString ootConds[]={"LowOOTpu","MediumOOTpu","HighOOTpu","VeryHighOOTpu"};
  for(size_t i=0; i<sizeof(ootConds)/sizeof(TString); i++){
      mon.addHistogram( new TH1F("sumEt"+ootConds[i],           ";#Sigma E_{T} [GeV];Events", 100,0,2000) );
      mon.addHistogram( new TH1F("neutSumEtFrac"+ootConds[i],   ";#Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );
  }

  //dilepton control 
  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 100,-5,5) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 100,0,400) );
  mon.addHistogram( new TH1F( "zrank",";Z hardness;Events",5,0,5) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,91-31,91+31) );
  mon.addHistogram( new TH1F( "dphill", ";#Delta#phi(l^{(1)},l^{(2)});Events",100,-3.2,3.2) );
  mon.addHistogram( new TH1F( "drll",   ";#DeltaR(l^{(1)},l^{(2)}) / (0.1 rad);Events",20,0,6) );
  mon.addHistogram( new TH1F( "dphizleadl", ";#Delta#phi(l^{(1)},Z);Events / (0.1 rad)",15,0,1.5) );
  mon.addHistogram( new TH1F( "mindrlz", ";min #DeltaR(l,Z);Events",100,0,6) );
  mon.addHistogram( new TH1F( "maxdrlz", ";max #DeltaR(l,Z);Events",100,0,6) );
  mon.addHistogram( new TH1F( "ptsum", ";#Sigma p_{T}(l^{(i)});Events", 100,0,1000));
  mon.addHistogram( new TH1F( "mtsum", ";#Sigma M_{T}(l^{(i)},E_{T}^{miss});Events / 20 GeV/c^{2}", 50,0,1000) );
  mon.addHistogram( new TH1F( "mtl1", ";M_{T}(l^{(1)},E_{T}^{miss});Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mtl2", ";M_{T}(l^{(2)},E_{T}^{miss});Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mt"  , ";M_{T},E_{T}^{miss});Events", 100,0,1000) );

  h=new TH1F ("zmassregionCtr", ";Selection region;Events", 18,0,18);
  h->GetXaxis()->SetBinLabel(1, "Z");
  h->GetXaxis()->SetBinLabel(2, "Z,b");
  h->GetXaxis()->SetBinLabel(3, "Z,b,=2l");
  h->GetXaxis()->SetBinLabel(4, "Z,b,=2l,medium");
  h->GetXaxis()->SetBinLabel(5, "Z,b,=2l,tight");
  h->GetXaxis()->SetBinLabel(6, "~Z");
  h->GetXaxis()->SetBinLabel(7, "~Z,b");
  h->GetXaxis()->SetBinLabel(8, "~Z,b,=2l");
  h->GetXaxis()->SetBinLabel(9, "~Z,b,=2l,medium");
  h->GetXaxis()->SetBinLabel(10,"~Z,b,=2l,tight");
  h->GetXaxis()->SetBinLabel(11,"Z,~b");
  h->GetXaxis()->SetBinLabel(12,"Z,~b,=2l");
  h->GetXaxis()->SetBinLabel(13,"Z,~b,=2l,medium");
  h->GetXaxis()->SetBinLabel(14,"Z,~b,=2l,tight");
  h->GetXaxis()->SetBinLabel(15,"~Z,~b");
  h->GetXaxis()->SetBinLabel(16,"~Z,~b,=2l");
  h->GetXaxis()->SetBinLabel(17,"~Z,~b,=2l,medium");
  h->GetXaxis()->SetBinLabel(18,"~Z,~b,=2l,tight");
  mon.addHistogram( h );

  //object multiplicity
  h=new TH1F("nleptons",";Leptons;Events",3,0,3);
  h->GetXaxis()->SetBinLabel(1,"=2 leptons");
  h->GetXaxis()->SetBinLabel(2,"=3 leptons");
  h->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
  mon.addHistogram( h );
  
  mon.addHistogram( new TH1F("njets"        ,";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );

  h = new TH1F ("nbtags", ";b-tag multiplicity; Events", 3,0,3);  
  h->GetXaxis()->SetBinLabel(1,"0 b-tags");
  h->GetXaxis()->SetBinLabel(2,"1 b-tags");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
  mon.addHistogram( h );
//  mon.addHistogram( new TH1F ("nbtags3leptons", ";b-tag multiplicity; Events", 3,0,3) );

  //used to optimize the b-veto
  h=new TH1F ("npassbveto", ";Pass b-tag veto; Events", 3,0,3);
  h->GetXaxis()->SetBinLabel(1,"TCHEL");
  h->GetXaxis()->SetBinLabel(2,"TCHE>2");
  h->GetXaxis()->SetBinLabel(3,"CSVL");
  mon.addHistogram( h );

  mon.addHistogram( new TH1F( "minmtjmet", ";min M_{T}(jet,E_{T}^{miss}) [GeV/c^{2}];Events",100,0,250) );
  mon.addHistogram( new TH1F( "mindrjz", ";min #DeltaR(jet,Z);Events",100,0,6) );
  mon.addHistogram( new TH1F( "minmjz", ";min M(jet,Z) [GeV/c^{2}];Events",100,0,500) );

  //Renormalization
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;

  //Met Optimization
  mon.addHistogram( new TH1F( "ZZ_RedMetInd_Best", ";IND RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetD0_Best", ";D0 RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_PFMet_Best", "; PFMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetInd_BestPUp", ";IND RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetD0_BestPUp", ";D0 RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_PFMet_BestPUp", "; PFMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetInd_BestPUm", ";IND RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetD0_BestPUm", ";D0 RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_PFMet_BestPUm", "; PFMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetInd_Best_Jesm", ";IND RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetD0_Best_Jesm", ";D0 RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_PFMet_Best_Jesm", "; PFMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetInd_Best_Jesp", ";IND RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetD0_Best_Jesp", ";D0 RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_PFMet_Best_Jesp", "; PFMet [GeV];Events", 30,0,30) );

  mon.addHistogram( new TH1F( "ZZ_RedMetInd_Opt", ";IND RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_RedMetD0_Opt", ";D0 RedMet [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_Mass_Opt", "; Z mass [GeV];Events", 30,0,30) );
  mon.addHistogram( new TH1F( "ZZ_Dphi_Opt", "; Dphi (Met-Z);Events", 30,0,30) );

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> Optim_Cuts1_met;
   std::vector<double> optim_Cuts1_dmz;
   for(double met=30;met<80;met+=2.0){
      for(double dmz=1.;dmz<20.;dmz+=1.){
               Optim_Cuts1_met.push_back(met);
               optim_Cuts1_dmz.push_back(dmz);
      }
   }

   mon.addHistogram ( new TH1F ("optim_eventflow1"  , ";cut index;yields" ,Optim_Cuts1_met.size(),0,Optim_Cuts1_met.size()) );
   TH1F* Hoptim_cuts1_Met     =  (TH1F*) mon.addHistogram( new TH1F ("Optim_cut1_met"    , ";cut index;met"    ,Optim_Cuts1_met.size(),0,Optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_dmz =  (TH1F*) mon.addHistogram( new TH1F ("Optim_cut1_dmz", ";cut index;dmz",Optim_Cuts1_met.size(),0,Optim_Cuts1_met.size()) ) ;
   for(unsigned int index=0;index<Optim_Cuts1_met.size();index++){
      Hoptim_cuts1_Met->Fill(index, Optim_Cuts1_met[index]);
      Hoptim_cuts1_dmz->Fill(index, optim_Cuts1_dmz[index]);
   }

  //---- FINAL OPTIMIZATION
   std::vector<double> optim_Cuts1_met;
   std::vector<double> optim_Cuts1_zmass;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=30.;met<160;met+=5.0){
	 if(met>90 && int(met)%10!=0)continue;
         if(met>120 && int(met)%20!=0)continue;
         for(double mz_=3.;mz_<40;mz_+=1.){
            double mz = mz_;
               optim_Cuts1_met    .push_back(met);
               optim_Cuts1_zmass  .push_back(mz);
      }
   }

   TH1F* Hoptim_cuts1_shape_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_shape_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_shape_mz   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_shape_mz"  , ";cut index;mz"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
      Hoptim_cuts1_shape_met    ->Fill(index, optim_Cuts1_met[index]);    
      Hoptim_cuts1_shape_mz  ->Fill(index, optim_Cuts1_zmass[index]);
   }

   TH1F* Hoptim_syst_shape     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs_shape"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;
   for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
   {
       Hoptim_syst_shape->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
       mon.addHistogram( new TH2F (TString("mt_shape")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 50,0,500) );
       mon.addHistogram( new TH2F (TString("mt_redMet_shape")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 32,150,950) );
       TH2F *h = (TH2F *) mon.addHistogram( new TH2F ("nonresbckg_ctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
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

  //pileup weighting
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC){
      TH1F* histo = (TH1F *) file->Get("evAnalyzer/h2zz/pileup");
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
  reweight::PoissonMeanShifter PShiftUp(+0.8);
  reweight::PoissonMeanShifter PShiftDown(-0.8);

  //check PU normalized entries 
  //evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  //TEventList *elist = (TEventList*)gDirectory->Get("elist");
  //const Int_t normEntries = (elist==0 ? 0 : elist->GetN()); 
  //if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;
 

  //event Categorizer
  EventCategory eventCategoryInst(true);
  mon.addHistogram( new TH1F("Eff_MetCutOnTop_Before", ";Eff RedMet Cut;Events", 1, 0.5, 1.5) );
  mon.addHistogram( new TH1F("Eff_MetCutOnTop_After", ";Eff RedMet Cut;Events", 1, 0.5, 1.5) );
// Preselection
  mon.addHistogram( new TH1F("ZZ_in_Mt", ";Diboson Transvers mass [GeV];Events", 50, 180., 800.) );
  mon.addHistogram( new TH1F("ZZ_in_PtZZ", ";Diboson Transvers Momentum [GeV];Events", 50, 0., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_DPhiZZ", ";Diboson DPhi [GeV];Events", 30, 0., 3.15) );
  mon.addHistogram( new TH1F("ZZ_in_DEtaZZ", ";Diboson DEta [GeV];Events", 30, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_in_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_in_NBtag", ";CVS discriminator ( Pt>20GeV ) [GeV];Events", 22, -5.1, 5.1) );
  mon.addHistogram( new TH1F("ZZ_in_NBtag_onlyMax", ";CVS discriminator ( Pt>20GeV ) [GeV];Events", 22, -5.1, 5.1) );
  mon.addHistogram( new TH1F("ZZ_in_llPt", ";Transvers momentum of lepton pair [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_in_l1Pt", ";Transvers momentum of lepton 1 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_l2Pt", ";Transvers momentum of lepton 2 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_MET1Pt", ";PF Missing transvers energy [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_in_llEta", ";#eta lepton 1;Events", 20, -4.5, 4.5) );
  mon.addHistogram( new TH1F("ZZ_in_nvtx", ";Number of Good vertex;Events", 30, 0., 30.) );
  mon.addHistogram( new TH1F("ZZ_in_njet", ";Number Jet;Events", 10, 0., 10.) );
  mon.addHistogram( new TH1F("ZZ_in_nlept", ";Number Lept;Events", 3, 2., 5.) );
  mon.addHistogram( new TH1F("ZZ_in_MetRed_IND", ";Met IND [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_in_MetRed_D0", ";Met D0 [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_in_RedIndMet_L", ";RedMet IND Long [GeV];Events", 50, -50., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_Balance", ";Met/Pt(Z);Events", 50, 0., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_RedBalance", ";RedMet/Pt(Z);Events", 50, 0., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_BalRedIndMet_L", ";Balance (RedMet IND Long);Events [GeV]", 50, -1., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_LeptId", ";Lept1+Lept2 Charge;Events", 2, 0., 2.) );
// After All selection
  mon.addHistogram( new TH1F("ZZ_fin_Mt", ";Diboson Transvers mass [GeV];Events", 50, 180., 800.) );
  mon.addHistogram( new TH1F("ZZ_fin_PtZZ", ";Diboson Transvers Momentum [GeV];Events", 50, 0., 100.) );
  mon.addHistogram( new TH1F("ZZ_fin_DPhiZZ", ";Diboson Dphi [GeV];Events", 30, 2., 3.15) );
  mon.addHistogram( new TH1F("ZZ_fin_DEtaZZ", ";Diboson DEta [GeV];Events", 30, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_fin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_finNonMass_llMass", ";Dilepton Invariant mass [GeV];Events", 50, 40., 140.) );
  mon.addHistogram( new TH1F("ZZ_fin_llPt", ";Transvers momentum of lepton pair [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_fin_l1Pt", ";Transvers momentum of lepton 1 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_fin_l2Pt", ";Transvers momentum of lepton 2 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_fin_MET1Pt", ";PF Missing transvers energy [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_fin_llEta", ";#eta lepton 1;Events", 20, -4.5, 4.5) );
  mon.addHistogram( new TH1F("ZZ_fin_nvtx", ";Number of Good vertex;Events", 30, 0., 30.) );
  mon.addHistogram( new TH1F("ZZ_fin_njet", ";Number Jet;Events", 10, 0., 10.) );
  mon.addHistogram( new TH1F("ZZ_fin_MetRed_IND", ";Met IND [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_fin_MetRed_D0", ";Met D0 [GeV];Events", 50, 0., 500.) );
// Cut Plot
  mon.addHistogram( new TH1F("ZZ_cut_llPt", ";Z Transvers momentum [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_Bveto", ";Num of B-tagged Jet (2);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_Bvetol", ";Num of B-tagged Jet (1.7);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_NBtag_onlyMax", ";CVS discriminator ( Pt>20GeV ) [GeV];Events", 22, -5.1, 5.1) );
  mon.addHistogram( new TH1F("ZZ_cut_njet", ";Num of Jet;Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_10", ";Num of Jet (Pt>10);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_15", ";Num of Jet (Pt>15);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_20", ";Num of Jet (Pt>20);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_25", ";Num of Jet (Pt>25);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_30", ";Num of Jet (Pt>30);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_aftPt_30", ";Num of Jet (Pt>30);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_MET1Pt", ";PF Missing transvers energy [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_cut_MetRed_IND", ";Met IND;Events [GeV]", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_cut_MetRed_D0", ";Met D0;Events [GeV]", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_cut_RedIndMet_L", ";RedMet IND Long;Events [GeV]", 50, -50., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_BalRedIndMet_L", ";Balance (RedMet IND Long);Events [GeV]", 50, -1., 2.) );
  mon.addHistogram( new TH1F("ZZ_cut_RedIndMet_L_yesJ", ";RedMet IND Long;Events [GeV]", 50, -50., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_BalRedIndMet_L_yesJ", ";Balance (RedMet IND Long);Events [GeV]", 50, -1., 2.) );
  mon.addHistogram( new TH1F("ZZ_cut_Balance", ";Balance;Events", 25, 0., 3.) );
  mon.addHistogram( new TH1F("ZZ_cut_Dphi_J", ";Delta Phi (J-Met);Events",  5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_Dphi_val", ";Delta Phi (J-Met);Events",  20, 0., 3.14) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept10", ";Num of Leptons, Pt > 10;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept15", ";Num of Leptons, Pt > 15;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept20", ";Num of Leptons, Pt > 17;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept", ";Num of Leptons, Pt > 10;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_cut_Zpt", ";Dilepton Pt [GeV];Events", 30, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_Zmass", ";Dilepton mass [GeV];Events", 30, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_cut_RedMetIND", ";RedMet IND [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_cut_1Zpt", ";Dilepton Pt [GeV];Events", 30, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_1Zmass", ";Dilepton mass [GeV];Events", 30, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_cut_1RedMetIND", ";RedMet IND [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_cut_2Zpt", ";Dilepton Pt [GeV];Events", 30, 40., 170.) );
  mon.addHistogram( new TH1F("ZZ_cut_2Zmass", ";Dilepton mass [GeV];Events", 30, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_cut_2RedMetIND", ";RedMet IND [GeV];Events", 50, 0., 500.) );

//CTRL BKG
  mon.addHistogram( new TH1F("Ctrl_WZ_PFMet", ";PF Met [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_RedMet", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_RedMet_3", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_MLEPT", ";RedMet [GeV];Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_MLEPT_10", ";RedMet [GeV];Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_MLEPT_20", ";RedMet [GeV];Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_RedMet_4", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WZ_Mt", ";Transvers Mass [GeV];Events", 24, 0., 120.) );
  mon.addHistogram( new TH1F("Ctrl_T_PFMet", ";PF Met [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_T_RedMet", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_T_Mass", ";Z Mass [GeV];Events", 50, 50., 450.) );
  mon.addHistogram( new TH1F("Ctrl_WW_Mass", ";Z Mass [GeV];Events", 25, 0., 200.) );
  mon.addHistogram( new TH1F("Ctrl_WW_PFMet", ";PF Met [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WW_RedMet", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WW_RedMet1", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WW_RedMet2", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WW_RedMet3", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WW_RedMet4", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_WW_RedMet5", ";RedMet [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_W_PFMet", ";PF Met [GeV];Events", 50, 0., 300.) );
  mon.addHistogram( new TH1F("Ctrl_W_RedMet", ";RedMet [GeV];Events", 50, 0., 300.) );
// Bin Pt
  mon.addHistogram( new TH1F("ZZ_Pt1bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_Pt2bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_Pt3bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_Pt4bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
// Met Systematic
  mon.addHistogram( new TH1F("Sys_MetEff_unclMet", ";# Unclustered Met;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMet_Jer", ";# Unclustered Met;Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("Sys_NewMet_Jerp", ";# RedMet after Jesp;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMet_Jerm", ";# RedMet after Jesm;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMet_Jesp", ";# RedMet after Jesp;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMet_Jesm", ";# RedMet after Jesm;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_Nlept", ";# Lept;Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("Sys_MetEff_deltaPhiJet", ";Delta Phi Jets;Events", 30, 0., 3.14) );
  mon.addHistogram( new TH1F("Sys_MetEff_Mass", ";Mass;Events", 70, 0., 300.) );
  mon.addHistogram( new TH1F("Sys_MetEff_Purity", ";# Event After Btag+Emu;Events", 1, 0., 1.) );
  mon.addHistogram( new TH1F("Sys_MetEff_Met", ";# RedMet [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_sumEt", ";Sum Et [GeV] ;Events", 70, 100., 1300.) );
  mon.addHistogram( new TH1F("Sys_MetEff_Nvtx", ";N vtx [GeV] ;Events", 30, 0., 30.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMetPx", ";RedMet x [GeV] ;Events", 30, -90., 90.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMetPy", ";RedMet y [GeV] ;Events", 30, -90., 90.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_px1nvtx", ";RedMet (nvtx<5) [GeV] ;Events", 30, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_px2nvtx", ";RedMet (nvtx<8) [GeV] ;Events", 30, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_px3nvtx", ";RedMet (nvtx<13) [GeV] ;Events", 40, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_px4nvtx", ";RedMet (nvtx>15) [GeV] ;Events", 40, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_py1nvtx", ";RedMet (nvtx<5) [GeV] ;Events", 30, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_py2nvtx", ";RedMet (nvtx<8) [GeV] ;Events", 30, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_py3nvtx", ";RedMet (nvtx<13) [GeV] ;Events", 30, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_py4nvtx", ";RedMet (nvtx>15) [GeV] ;Events", 30, -100., 100.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_1nvtx", ";RedMet (nvtx<6) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_2nvtx", ";RedMet (nvtx<9) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_3nvtx", ";RedMet (nvtx<13) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_4nvtx", ";RedMet (nvtx>13) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMetSmired", ";RedMet Smeared [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMetSmiredCyrt", ";RedMet Smeared (Ctrl sample) [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMetSmiredJER", ";RedMet Smeared into JER [GeV] ;Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("Sys_NewMetjetDiff", ";Diff for RedMet Smeared into JER [GeV] ;Events", 50, 0., 40.) );

  mon.addHistogram( new TH1F("MetEff_RedMetPt", ";RedMet Pt [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("MetEff_Met_Nj", ";# RedMet (Nj>2) [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("MetEff_Met_Phip", ";# RedMet (DPhi>1.5) [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("MetEff_Met_Phim", ";# RedMet (DPhi<1.5) [GeV] ;Events", 50, 0., 200.) );
// Cross Section
  TH1F *Norm_pu = new TH1F("Norm_pu", "Pupm Normalizarion", 1, 0., 1.);
  TH1F *Norm_pup = new TH1F("Norm_pup", "Pupm Normalizarion", 1, 0., 1.);
  TH1F *Norm_pum = new TH1F("Norm_pum", "Pupm Normalizarion", 1, 0., 1.);
  TH1F *ee_Effic = new TH1F("ee_Effic", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *ee_Effic_tot = new TH1F("ee_Effic_tot", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic = new TH1F("mumu_Effic", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_tot = new TH1F("mumu_Effic_tot", "ZZ (mumu) Efficiency", 1, 0., 1.);
  //Pu
  TH1F *ee_EfficPUm = new TH1F("ee_EfficPUm", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *ee_EfficPUm_tot = new TH1F("ee_EfficPUm_tot", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_EfficPUm = new TH1F("mumu_EfficPUm", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *mumu_EfficPUm_tot = new TH1F("mumu_EfficPUm_tot", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *ee_EfficPUp = new TH1F("ee_EfficPUp", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *ee_EfficTrig_up = new TH1F("ee_EfficTrig_up", "ZZ (ee) Efficiency Trigger up", 1, 0., 1.);
  TH1F *ee_EfficTrig_dw = new TH1F("ee_EfficTrig_dw", "ZZ (ee) Efficiency Trigger down", 1, 0., 1.);
  TH1F *ee_EfficScal_up = new TH1F("ee_EfficScal_up", "ZZ (ee) Efficiency Scale up", 1, 0., 1.);
  TH1F *ee_EfficScal_dw = new TH1F("ee_EfficScal_dw", "ZZ (ee) Efficiency Scale dw", 1, 0., 1.);
  TH1F *mumu_EfficPUp = new TH1F("mumu_EfficPUp", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *mumu_EfficTrig_up = new TH1F("mumu_EfficTrig_up", "ZZ (mumu) Efficiency Trigger up", 1, 0., 1.);
  TH1F *mumu_EfficTrig_dw = new TH1F("mumu_EfficTrig_dw", "ZZ (mumu) Efficiency Trigger down", 1, 0., 1.);
  TH1F *mumu_EfficScal_up = new TH1F("mumu_EfficScal_up", "ZZ (mumu) Efficiency Scale up", 1, 0., 1.);
  TH1F *mumu_EfficScal_dw = new TH1F("mumu_EfficScal_dw", "ZZ (mumu) Efficiency Scale dw", 1, 0., 1.);
  //JER JES
  TH1F *ee_EfficJer = new TH1F("ee_EfficJer", "ZZ (ee) Efficiency Jer", 1, 0., 1.);
  TH1F *mumu_EfficJer = new TH1F("mumu_EfficJer", "ZZ (mumu) Efficiency Jer", 1, 0., 1.);
  TH1F *ee_EfficJesm = new TH1F("ee_EfficJesm", "ZZ (ee) Efficiency Jesm", 1, 0., 1.);
  TH1F *mumu_EfficJesm = new TH1F("mumu_EfficJesm", "ZZ (mumu) Efficiency Jesm", 1, 0., 1.);
  TH1F *ee_EfficJesp = new TH1F("ee_EfficJesp", "ZZ (ee) Efficiency Jesp", 1, 0., 1.);
  TH1F *mumu_EfficJesp = new TH1F("mumu_EfficJesp", "ZZ (mumu) Efficiency Jesp", 1, 0., 1.);
  //Pt bin
  TH1F *ee_Effic_ptBin1 = new TH1F("ee_Effic_ptBin1", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_ptBin1 = new TH1F("mumu_Effic_ptBin1", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *ee_Effic_ptBin2 = new TH1F("ee_Effic_ptBin2", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_ptBin2 = new TH1F("mumu_Effic_ptBin2", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *ee_Effic_ptBin3 = new TH1F("ee_Effic_ptBin3", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_ptBin3 = new TH1F("mumu_Effic_ptBin3", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *ee_Effic_ptBin4 = new TH1F("ee_Effic_ptBin4", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_ptBin4 = new TH1F("mumu_Effic_ptBin4", "ZZ (mumu) Efficiency", 1, 0., 1.);
  // Met Smearing
  TH1F *ee_Effic_MetSmear = new TH1F("ee_Effic_MetSmear", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_MetSmear = new TH1F("mumu_Effic_MetSmear", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *ee_Effic_MetSmearm = new TH1F("ee_Effic_MetSmearm", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_MetSmearm = new TH1F("mumu_Effic_MetSmearm", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *ee_Effic_MetSmearp = new TH1F("ee_Effic_MetSmearp", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_Effic_MetSmearp = new TH1F("mumu_Effic_MetSmearp", "ZZ (mumu) Efficiency", 1, 0., 1.);

  //Fit DY
  mon.addHistogram( new TH1F("Fit_PFMet", ";RedMet [GeV]; Events", 60, 0., 80.) );
  mon.addHistogram( new TH1F("Fit_RedMet_D0", ";RedMet [GeV]; Events", 60, 0., 80.) );
  mon.addHistogram( new TH1F("Fit_RedMet_IND", ";RedMet [GeV]; Events", 60, 0., 80.) );
  //Smearing 1vtx
  TH1F **Sys_MetSmearedOnLine_px1vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_px1vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_px1vtx[isigma] = new TH1F (nameHisto.Data(),"Met px Smeared On Line 1vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_py1vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_py1vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_py1vtx[isigma] = new TH1F (nameHisto.Data(),"Met py Smeared On Line 1vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_1vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_1vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_1vtx[isigma] = new TH1F (nameHisto.Data(),"Met pt Smeared On Line 1vtx", 30, 0., 200.);
  }
  //2vtx
  TH1F **Sys_MetSmearedOnLine_px2vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_px2vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_px2vtx[isigma] = new TH1F (nameHisto.Data(),"Met px Smeared On Line 2vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_py2vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_py2vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_py2vtx[isigma] = new TH1F (nameHisto.Data(),"Met py Smeared On Line 2vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_2vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_2vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_2vtx[isigma] = new TH1F (nameHisto.Data(),"Met pt Smeared On Line 2vtx", 30, 0., 200.);
  }
  //3vtx
  TH1F **Sys_MetSmearedOnLine_px3vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_px3vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_px3vtx[isigma] = new TH1F (nameHisto.Data(),"Met px Smeared On Line 3vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_py3vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_py3vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_py3vtx[isigma] = new TH1F (nameHisto.Data(),"Met py Smeared On Line 3vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_3vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_3vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_3vtx[isigma] = new TH1F (nameHisto.Data(),"Met pt Smeared On Line 3vtx", 30, 0., 200.);
  }

  //4vtx
  TH1F **Sys_MetSmearedOnLine_px4vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_px4vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_px4vtx[isigma] = new TH1F (nameHisto.Data(),"Met px Smeared On Line 4vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_py4vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_py4vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_py4vtx[isigma] = new TH1F (nameHisto.Data(),"Met py Smeared On Line 4vtx", 30, -100., 100.);
  }
  TH1F **Sys_MetSmearedOnLine_4vtx = new TH1F*[100];
  for(int isigma=0; isigma<100; isigma++){
       char numHisto[5];
       sprintf(numHisto,"%d",isigma);
       TString nameHisto = "Sys_MetSmearedOnLine_4vtx_" + string(numHisto);
       Sys_MetSmearedOnLine_4vtx[isigma] = new TH1F (nameHisto.Data(),"Met pt Smeared On Line 4vtx", 30, 0., 200.);
  }
  // Systematic
  double NzzTot_ee = 0., NzzTot_mumu = 0., /*NzzTotPUp_ee =0.,*/ /*NzzTotPUp_mumu =0.,*/ NzzTotPUm_ee =0., NzzTotPUm_mumu =0., Nzz_ee = 0., Nzz_mumu = 0.;
  double NzzPUp_ee=0., NzzPUp_mumu=0.,NzzPUm_ee=0.,NzzPUm_mumu=0.,NzzTrig_up_ee=0.,NzzTrig_dw_ee=0.,NzzScal_up_ee=0.,NzzScal_dw_ee=0.,NzzTrig_up_mu=0.,NzzTrig_dw_mu=0.,NzzScal_up_mu=0.,NzzScal_dw_mu=0.;
  double NzzJer_ee = 0., NzzJer_mumu = 0., NzzJesm_ee = 0., NzzJesm_mumu = 0., NzzJesp_ee = 0., NzzJesp_mumu = 0.;
  double Nzz_ee_ptBin1 = 0., Nzz_ee_ptBin2 = 0., Nzz_ee_ptBin3 = 0., Nzz_ee_ptBin4 = 0.;
  double Nzz_mumu_ptBin1 = 0., Nzz_mumu_ptBin2 = 0., Nzz_mumu_ptBin3 = 0., Nzz_mumu_ptBin4 = 0.;
  double Nzz_MetSmear_ee =0., Nzz_MetSmear_mumu =0.,  Nzz_MetSmearm_ee =0., Nzz_MetSmearm_mumu =0.,  Nzz_MetSmearp_ee =0., Nzz_MetSmearp_mumu =0.;
  double Tot=0.,Tot_pup=0., Tot_pum=0.;
  //##############################################
  //########           EVENT LOOP         ########
  //##############################################


  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}
      //##############################################   EVENT LOOP STARTS   ##############################################

      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);

      //categorize events
      TString tag_cat;
      switch(ev.cat){
         case EMU : tag_cat = "emu";   break;
         case MUMU: tag_cat = "mumu";  break;
         case EE  : tag_cat = "ee";    break;
         default  : continue;

      }
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

      //int eventSubCat  = eventCategoryInst.Get(phys);
      int eventSubCat = eventCategoryInst.Get(phys, &phys.ajets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);

      //prepare the tag's vectors for histo filling
      std::vector<TString> tags_all;
      tags_all.push_back("all");

      std::vector<TString> tags_cat;
      tags_cat.push_back("all");
      tags_cat.push_back(tag_cat);

      std::vector<TString> tags_full;
      tags_full.push_back("all");
      tags_full.push_back(tag_cat);
      tags_full.push_back(tag_cat + tag_subcat);

      //OOT pu condition
      TString ootCond("");
      if(isMC){
          double sigma        = sqrt(double(2*ev.ngenITpu));
          double minCentralPu = -sigma;
          double maxCentralPu = sigma;
          double maxHighPu    = 2*sigma;
          double puDiff       = double(ev.ngenOOTpu-2*ev.ngenITpu);
          if(puDiff>=maxHighPu)         ootCond="VeryHighOOTpu";
          else if(puDiff>=maxCentralPu) ootCond="HighOOTpu";
          else if(puDiff>=minCentralPu) ootCond="MediumOOTpu";
          else                          ootCond="LowOOTpu";
      }

//      //pileup weight
//      float weight = 1.0;
//      float signalWeight=1.0;
//      double TotalWeight_plus = 1.0;
//      double TotalWeight_minus = 1.0;
//      if(isMC){
//        weight            = LumiWeights->weight(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
//        TotalWeight_plus  = PuShifters[PUUP]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
//        TotalWeight_minus = PuShifters[PUDOWN]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
//	if(isMC_VBF && use2011Id){ signalWeight = weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );  weight*=signalWeight; }
//        if(isMC_GG) {
//          for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) 
//	    ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
//	  weight *= ev.hptWeights[0];
//	}
//      }
//      Hcutflow->Fill(1,1);
//      Hcutflow->Fill(2,weight);
//      Hcutflow->Fill(3,weight*TotalWeight_minus);
//      Hcutflow->Fill(4,weight*TotalWeight_plus);
//      Hcutflow->Fill(5,signalWeight);

      //pileup and Higgs pT weight
      //float weight=ev.puWeight;
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      double vbfweight = 1.0;
      if(isMC){
        weight = LumiWeights.weight( ev.ngenITpu );
        TotalWeight_plus = PShiftUp.ShiftWeight( ev.ngenITpu );
        TotalWeight_minus = PShiftDown.ShiftWeight( ev.ngenITpu );
        if(isMC_VBF){ vbfweight = weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );  weight*=vbfweight; 
        }
        if(isMC_GG)  {
	  for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
	  weight *= ev.hptWeights[0];
	}
      }
     
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);
      Hcutflow->Fill(5,vbfweight);

      //##############################################
      //########       GLOBAL VARIABLES       ########
      //##############################################

      //lept correction Off
      //float l1corr( fabs(phys.leptons[0].id)==ELECTRON ? phys.leptons[0].ensf : 1.0 ); if( l1corr==0 || isMC ) l1corr=1;
      //float l2corr( fabs(phys.leptons[1].id)==ELECTRON ? phys.leptons[1].ensf : 1.0 ); if( l2corr==0 || isMC ) l2corr=1;
 
      float iweight=weight;

      float EEScaleFactor1 = 1., EETriggerEff1 = 1., MuMuScaleFactor1 = 1., MuMuTriggerEff1 = 1.;
      float EEScaleFactor2 = 1., EETriggerEff2 = 1., MuMuScaleFactor2 = 1., MuMuTriggerEff2 = 1.;
      float iweight_trig_up = 1., iweight_trig_dw = 1., iweight_scal_up = 1., iweight_scal_dw = 1.;
      float VariedWeight1 = 0., VariedWeight2 = 0.;
      if(isMC){
        if( tag_cat == "ee" ){
          EEScaleFactor1 = electronScaleFactor( phys.leptons[0].Pt(), fabs(phys.leptons[0].Eta()) ); EEScaleFactor2 = electronScaleFactor( phys.leptons[1].Pt(), fabs(phys.leptons[1].Eta()) );
          EETriggerEff1 = electronTriggerEfficiency( phys.leptons[0].Pt(), fabs(phys.leptons[0].Eta()) ); EETriggerEff2 = electronTriggerEfficiency( phys.leptons[1].Pt(), fabs(phys.leptons[1].Eta()) );

          VariedWeight1 = (EETriggerEff1==1.) ? 1. : (EETriggerEff1  + (EETriggerEff1/100.));          VariedWeight2 = (EETriggerEff2==1.) ? 1. : (EETriggerEff2  + (EETriggerEff2/100.));
          iweight_trig_up = iweight * EEScaleFactor1 * EEScaleFactor2 *( VariedWeight1 ) * ( VariedWeight2 );
          VariedWeight1 = (EETriggerEff1==1.) ? 1. : (EETriggerEff1  - (EETriggerEff1/100.));          VariedWeight2 = (EETriggerEff2==1.) ? 1. : (EETriggerEff2  - (EETriggerEff2/100.));
          iweight_trig_dw = iweight * EEScaleFactor1 * EEScaleFactor2 *( VariedWeight1 ) * ( VariedWeight2 );
          VariedWeight1 = (EEScaleFactor1==1.) ? 1. : ( EEScaleFactor1 + (EEScaleFactor1/100.));          VariedWeight2 = (EEScaleFactor2==1.) ? 1. : ( EEScaleFactor2 + (EEScaleFactor2/100.));
          iweight_scal_up = iweight * EETriggerEff1  * EETriggerEff2  *( VariedWeight1 )* ( VariedWeight2 );
          VariedWeight1 = (EEScaleFactor1==1.) ? 1. : ( EEScaleFactor1 - (EEScaleFactor1/100.));          VariedWeight2 = (EEScaleFactor2==1.) ? 1. : ( EEScaleFactor2 - (EEScaleFactor2/100.));
          iweight_scal_dw = iweight * EETriggerEff1  * EETriggerEff2  *( VariedWeight1 )* ( VariedWeight2 );
          //cout<<"INIT: "<<iweight<<endl;
          iweight *= (EEScaleFactor1 * EEScaleFactor2 * EETriggerEff1 * EETriggerEff2);
          //cout<<EEScaleFactor1<<" "<<EEScaleFactor2<<" "<<EETriggerEff1<<" "<<EETriggerEff2<<endl;
          //cout<<iweight<<endl;
        }
        if( tag_cat == "mumu" ){
          MuMuScaleFactor1 = muonScaleFactor( phys.leptons[0].Pt(), fabs(phys.leptons[0].Eta()) );           MuMuScaleFactor2 = muonScaleFactor( phys.leptons[1].Pt(), fabs(phys.leptons[1].Eta()) );
          MuMuTriggerEff1 = muonTriggerEfficiency( phys.leptons[0].Pt(), fabs(phys.leptons[0].Eta()) );      MuMuTriggerEff2 = muonTriggerEfficiency( phys.leptons[1].Pt(), fabs(phys.leptons[1].Eta()) );

          VariedWeight1 = (MuMuTriggerEff1==1.) ? 1. : (MuMuTriggerEff1  + (MuMuTriggerEff1/100.));          VariedWeight2 = (MuMuTriggerEff2==1.) ? 1. : (MuMuTriggerEff2  + (MuMuTriggerEff2/100.));
          iweight_trig_up = iweight * MuMuScaleFactor1 * MuMuScaleFactor2 *( VariedWeight1 ) * ( VariedWeight2 );
          VariedWeight1 = (MuMuTriggerEff1==1.) ? 1. : (MuMuTriggerEff1  - (MuMuTriggerEff1/100.));          VariedWeight2 = (MuMuTriggerEff2==1.) ? 1. : (MuMuTriggerEff2  - (MuMuTriggerEff2/100.));
          iweight_trig_dw = iweight * MuMuScaleFactor1 * MuMuScaleFactor2 *( VariedWeight1 ) * ( VariedWeight2 );
          VariedWeight1 = (MuMuScaleFactor1==1.) ? 1. : ( MuMuScaleFactor1 + (MuMuScaleFactor1/100.));          VariedWeight2 = (MuMuScaleFactor2==1.) ? 1. : ( MuMuScaleFactor2 + (MuMuScaleFactor2/100.));
          iweight_scal_up = iweight * MuMuTriggerEff1  * MuMuTriggerEff2  *( VariedWeight1 )* ( VariedWeight2);
          VariedWeight1 = (MuMuScaleFactor1==1.) ? 1. : ( MuMuScaleFactor1 - (MuMuScaleFactor1/100.));          VariedWeight2 = (MuMuScaleFactor2==1.) ? 1. : ( MuMuScaleFactor2 - (MuMuScaleFactor2/100.));
          iweight_scal_dw = iweight * MuMuTriggerEff1  * MuMuTriggerEff2  *( VariedWeight1 )* ( VariedWeight2);
          //cout<<"INIT: "<<iweight<<endl;
          iweight *= (MuMuScaleFactor1 * MuMuScaleFactor2 * MuMuTriggerEff1 * MuMuTriggerEff2);
          //cout<<MuMuScaleFactor1<<" "<<MuMuScaleFactor2<<" "<<MuMuTriggerEff1<<" "<<MuMuTriggerEff2<<endl;
          //cout<<iweight<<endl;
        }
      }

      //z+met kinematics
      LorentzVector lep1 = phys.leptons[0];
      LorentzVector lep2 = phys.leptons[1];
      LorentzVector zll  = lep1+lep2;
      LorentzVector zvv  = phys.met[0];
      Float_t dphill     = deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      Float_t mindrlz    = min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t maxdrlz    = max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t ptl1       = phys.leptons[0].pt();
      Float_t ptl2       = phys.leptons[1].pt();
      Float_t ptsum      = ptl1+ptl2;
      Float_t mtl1       = METUtils::transverseMass(phys.leptons[0],zvv,false);
      Float_t mtl2       = METUtils::transverseMass(phys.leptons[1],zvv,false);
      Float_t zmass      = zll.mass();
      Float_t zpt        = zll.pt();
      Float_t zeta       = zll.eta();
      Float_t met        = zvv.pt();
      Float_t mt         = METUtils::transverseMass(zll,zvv,true);

     //Filer: Check if it's ZZ-llvv
     float StartCut = 20., EndCut = 80., BinCut = 2.;
     int TwoEle=0, TwoNEle=0, TwoMu=0, TwoNMu=0;
     bool ZZllvvOrnotZZ = false;
     TString DatasetIn(runProcess.getParameter<string>("input"));
     if( DatasetIn.Contains("MC_ZZ") ){
          for(int i=0; i<ev.nmcparticles ;i++){
              if( (fabs(ev.mc_id[i])==23) && fabs(ev.mc_id[i+1])==11 && fabs(ev.mc_id[i+2])==11 ) TwoEle++;
              if( (fabs(ev.mc_id[i])==23) && fabs(ev.mc_id[i+1])==12 && fabs(ev.mc_id[i+2])==12 ) TwoNEle++;
              if( (fabs(ev.mc_id[i])==23) && fabs(ev.mc_id[i+1])==13 && fabs(ev.mc_id[i+2])==13 ) TwoMu++;
              if( (fabs(ev.mc_id[i])==23) && fabs(ev.mc_id[i+1])==14 && fabs(ev.mc_id[i+2])==14 ) TwoNMu++;
          }
          if( TwoEle>=1 || TwoNEle>=1 )  ZZllvvOrnotZZ = true;
          if( TwoMu>=1  || TwoNMu>=1 )   ZZllvvOrnotZZ = true;
      }
      else ZZllvvOrnotZZ = true;

      //Jets
      LorentzVectorCollection jetsP4;
      for(size_t ijet=0; ijet<phys.ajets.size(); ijet++) jetsP4.push_back( phys.ajets[ijet] );
      //Lepts & MetInfos
      METUtils::stRedMET redMetInfo;
      LorentzVector metP4             = phys.met[0];
      LorentzVector centralMetP4      = phys.met[3];
      LorentzVector assocMetP4        = phys.met[1];
      LorentzVector assocChargedMetP4 = phys.met[5];
      LorentzVector assocFwdMetP4     = phys.met[6];
      LorentzVector assocCMetP4       = phys.met[11];
      LorentzVector assocFwdCMetP4    = phys.met[12];
      LorentzVector clusteredMetP4    = -1*zll;  for(unsigned int i=0;i<jetsP4.size();i++){clusteredMetP4 -= jetsP4[i];}
      LorentzVector unclMet = (zvv - clusteredMetP4);
      LorentzVector redMetP4   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetsP4             , zvv                , false, &redMetInfo);

      double redMet = redMetP4.pt();   
      //double redMetL = redMetInfo.redMET_l; double redMetT = redMetInfo.redMET_t;
      LorentzVector redMetD0P4 = METUtils::redMET(METUtils::D0, lep1, 0, lep2, 0, jetsP4             , zvv                , false); 
      //Float_t projMet              =  METUtils::projectedMET(phys.leptons[0], phys.leptons[1], zvv).pt();
      double RedIndMet_L = redMetP4.pt()*cos( fabs(deltaPhi( redMetP4.phi(),zll.phi() )) );

      //GenJet
      std::vector<double> genJetsPt;
      for(size_t ijet=0; ijet<phys.ajets.size(); ijet++)      genJetsPt.push_back( phys.ajets[ijet].genPt );
      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> Jets;
      //Prepare Variation JER/JES   
      if( runSystematics )          METUtils::computeVariation(phys.ajets, phys.leptons, zvv, Jets, zvvs, &jecUnc);
      PhysicsObjectJetCollection aJets= Jets[0];
      LorentzVectorCollection JetsSme; for(unsigned int i=0; i<Jets[0].size(); i++) JetsSme.push_back(Jets[0][i]);
      LorentzVector MetSmeared        = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, JetsSme, zvvs[0], false, &redMetInfo);
      LorentzVector MetSmeared_jerp   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, JetsSme, zvvs[1], false, &redMetInfo);
      LorentzVector MetSmeared_jerm   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, JetsSme, zvvs[2], false, &redMetInfo);
      LorentzVector MetSmeared_jesp   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, JetsSme, zvvs[3], false, &redMetInfo);
      LorentzVector MetSmeared_jesm   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, JetsSme, zvvs[4], false, &redMetInfo);
      if( tag_cat=="ee" || tag_cat=="mumu" ){
        mon.fillHisto("Sys_NewMet_Jer",  tags_cat, isMC ? MetSmeared.pt()      : redMetP4.pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jerp", tags_cat, isMC ? MetSmeared_jerp.pt() : redMetP4.pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jerm", tags_cat, isMC ? MetSmeared_jerm.pt() : redMetP4.pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jesp", tags_cat, isMC ? MetSmeared_jesp.pt() : redMetP4.pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jesm", tags_cat, isMC ? MetSmeared_jesm.pt() : redMetP4.pt(), iweight);
      }
      //JES PFMEt for stability
      LorentzVector PFMetSmeared_jesp   = zvvs[3]; 
      LorentzVector PFMetSmeared_jesm   = zvvs[4];
      //JES METD0 for stability
      LorentzVector D0MetSmeared_jesp   = METUtils::redMET(METUtils::D0, lep1, 0, lep2, 0, jetsP4, zvvs[3], false, &redMetInfo);
      LorentzVector D0MetSmeared_jesm   = METUtils::redMET(METUtils::D0, lep1, 0, lep2, 0, jetsP4, zvvs[4], false, &redMetInfo);

      //JER My Way
      LorentzVector newMetJer;
      std::vector<LorentzVector> jetsJer;
      if( runSystematics ) newMetJer = METUtils::SmearJetFormGen(jetsP4, zvv, genJetsPt, jetsJer);
      LorentzVector MetSmeared_my   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetsP4, newMetJer, false, &redMetInfo);
      if( tag_cat == "ee" || tag_cat == "mumu" ){
            double newMetJerpt = isMC ? MetSmeared_my.pt() : redMetP4.pt();
            mon.fillHisto("Sys_NewMetSmiredJER", tags_cat, newMetJerpt, iweight);
      }
      
      //count jets and b-tags
      LorentzVector BestJet;
      float PtApp=0.;
      int njets(0),njetsinc(0);
      int nbtags(0), nbtags_tchel(0),   nbtags_tche2(0),  nbtags_csvl(0), nbtags_csvl_tight(0);
      int nheavyjets(0), nlightsjets(0);
      int Nj_10=0, Nj_15=0, Nj_20=0, Nj_25=0, Nj_30=0;
      int tmpJ = 0; double deltaPhiJet = 0.;
      float MaxBtag=-999.;
      for(size_t ijet=0; ijet<Jets[0].size(); ijet++){
          if(Jets[0][ijet].pt()>10) Nj_10++;
          if(Jets[0][ijet].pt()>15) Nj_15++;
          if(Jets[0][ijet].pt()>20) Nj_20++;
          if(Jets[0][ijet].pt()>25) Nj_25++;
          if(Jets[0][ijet].pt()>30){
             Nj_30++; if(Nj_30==1) tmpJ=ijet; if(Nj_30==2) deltaPhiJet = deltaPhi(Jets[0][ijet].phi(),Jets[0][tmpJ].phi());  
          }
          njetsinc++;
          if(fabs(Jets[0][ijet].eta())<2.5){
              if( Jets[0][ijet].btag2 > MaxBtag  ) MaxBtag = Jets[0][ijet].btag2;
              mon.fillHisto("ZZ_in_NBtag", tags_cat, Jets[0][ijet].btag2, iweight);
              njets++;
              bool passTCHEL(Jets[0][ijet].btag1>1.7);
              bool passTCHE2(Jets[0][ijet].btag1>2.0);
              bool passCSVL(Jets[0][ijet].btag2>0.244);
              bool passCSVLTight(Jets[0][ijet].btag2>0.15);
              if(Jets[0][ijet].pt()>20){
                nbtags          += passTCHE2;
                nbtags_tchel    += passTCHEL;
                nbtags_tche2    += passTCHE2;
                nbtags_csvl     += passCSVL;
                nbtags_csvl_tight     += passCSVLTight;
                nheavyjets += (fabs(Jets[0][ijet].genid)==5);
                nlightsjets += (fabs(Jets[0][ijet].genid)!=5);
                if( Jets[0][ijet].pt() > PtApp ){PtApp =  Jets[0][ijet].pt(); BestJet = Jets[0][ijet];}
              }
          }
      }
      if(MaxBtag>-9.) mon.fillHisto("ZZ_in_NBtag_onlyMax", tags_cat, MaxBtag, iweight);
      bcomp.compute(nheavyjets,nlightsjets);
      std::vector<btag::Weight_t> wgt = bcomp.getWeights();
      double p0btags = wgt[0].first;  
      double p0btags_err=wgt[0].second;
 

//      //
//      // LEPTON ANALYSIS
//      //
//      for(int ilep=0; ilep<2; ilep++){
//	  TString lepStr( fabs(phys.leptons[ilep].id)==13 ? "mu" : "e");
//
//	  //id and isolation
//	  int lpid=phys.leptons[ilep].pid;
//	  float relIso2011    = phys.leptons[ilep].relIsoRho(ev.rho);
//	  float relIso = (lepStr=="mu") ? 
//	    phys.leptons[ilep].pfRelIsoDbeta(): //muPFRelIsoCorrected2012(ev.rho25Neut):
//	    phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho);
//	  std::vector<int> passIds;
//	  std::map<int,bool> passIsos;
//	  bool hasGoodId(false), isIso(false);
//	  if(fabs(phys.leptons[ilep].id)==13)
//	    {
//	      if( hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) )    { passIds.push_back(0); passIsos[0]=(relIso<0.2); if(!use2011Id) { hasGoodId=true; isIso=passIsos[0]; } }
//	      if( hasObjectId(ev.mn_idbits[lpid], MID_TIGHT) )    { passIds.push_back(1); passIsos[1]=(relIso<0.2); }
//	      if( hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) ) { passIds.push_back(2); passIsos[2]=(relIso2011<0.15); if(use2011Id) {hasGoodId=true; isIso=passIsos[2];} }
//	      if( hasObjectId(ev.mn_idbits[lpid], MID_SOFT) )     { passIds.push_back(3); passIsos[3]=true;}
//	      if(use2011Id) 
//		{
//		  try{
//		    llScaleFactor *= muonScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
//		    llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
//		  }catch(std::string &e){
//		  }
//		}
//	      else
//		{
//		  llScaleFactor *= 1;
//		  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
//		}
//	    }
//	  else
//	    {
//	      int wps[]={EgammaCutBasedEleId::LOOSE,EgammaCutBasedEleId::MEDIUM, EID_VBTF2011, EgammaCutBasedEleId::VETO};
//	      for(int iwp=0; iwp<4; iwp++)
//		{
//		  if(iwp==2 && hasObjectId(ev.en_idbits[lpid], EID_VBTF2011)) 
//		    { 
//		      passIds.push_back(2); passIsos[2]=(relIso2011<0.10); 
//		      if(use2011Id) 
//			{ 
//			  hasGoodId=true; isIso=passIsos[2]; 
//			  try{
//			    llScaleFactor *= electronScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
//			    llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
//			  } catch(std::string &e){
//			  }
//			}
//		    }
//		  else
//		    {
//		      bool passWp = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::WorkingPoint(wps[iwp]),
//								(fabs(phys.leptons[ilep].eta())<1.4442),
//								phys.leptons[ilep].pt(), phys.leptons[ilep].eta(),
//								ev.en_detain[lpid],  ev.en_dphiin[lpid], ev.en_sihih[lpid], ev.en_hoe[lpid],
//								ev.en_ooemoop[lpid], phys.leptons[ilep].d0, phys.leptons[ilep].dZ,
//								0., 0., 0.,
//								!hasObjectId(ev.en_idbits[lpid], EID_CONVERSIONVETO),0,ev.rho);
//		      if(passWp) { 
//			passIds.push_back(iwp); 
//			passIsos[iwp]=(relIso<0.15);
//			if(wps[iwp]==EgammaCutBasedEleId::MEDIUM && !use2011Id){  hasGoodId=true; isIso=passIsos[iwp]; }
//		      }
//		      if(!use2011Id)
//			{
//			  llScaleFactor *= 1;
//			  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
//			}
//		    }
//		}
//	    }
//	  if(!hasGoodId)  passIdAndIso=false;
//	  else if(!isIso) passIdAndIso=false;     
//      }
//      if( passIdAndIso ) continue;
      

      //Isolation
      bool NoIso=false;
      for( int ilep=0; ilep<2; ilep++){
         int lpid=phys.leptons[ilep].pid;
         if(fabs(phys.leptons[ilep].id)==13){
            if( !hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) ) NoIso=true;
            float relIso2011 = phys.leptons[ilep].relIsoRho(ev.rho);
            if( relIso2011 > 0.15) NoIso=true;
         }
         if(fabs(phys.leptons[ilep].id)==11){
            if( !hasObjectId(ev.mn_idbits[lpid], EID_VBTF2011) ) NoIso=true;
            float relIso2011 = phys.leptons[ilep].relIsoRho(ev.rho);
            if( relIso2011 > 0.10) NoIso=true;
         }
      }
      if(NoIso) continue;


      //sum ETs
      float sumEt            = ev.sumEt           - ptsum;
      // float neutsumEt        = ev.neutsumEjmett;

      //DPhi
      int DphiMet_Jet=0;
      for(size_t ijet=0; ijet<Jets[0].size(); ijet++){
             if( fabs(deltaPhi(Jets[0][ijet].Phi(), zvv.phi()))<0.5 && Jets[0][ijet].Pt()>20. ) DphiMet_Jet++;
      }
      bool DphiMet_Jet_jer = true, JetVeto_jer = true;
      for(size_t ijet=0; ijet<Jets[0].size(); ijet++){
             if( fabs(deltaPhi(Jets[0][ijet].phi(), zvv.phi()))<0.5 && Jets[0][ijet].pt()>20. ) DphiMet_Jet_jer=false;
             if( Jets[0][ijet].pt()>30. ) JetVeto_jer = false;
             //if(ijet<phys.ajets.size()) cout<<"confronto phi: "<<Jets[0][ijet].phi()<<" "<<phys.ajets[ijet].phi()<<endl;
             //if(ijet<phys.ajets.size()) cout<<"confronto pt: "<<Jets[0][ijet].pt()<<" "<<phys.ajets[ijet].pt()<<endl;
      }
      bool DphiMet_Jet_jesp = true,  JetVeto_jesp = true;
      for(size_t ijet=0; ijet<Jets[3].size(); ijet++){
             if( fabs(deltaPhi(Jets[3][ijet].phi(), zvv.phi()))<0.5 && Jets[3][ijet].pt()>20. ) DphiMet_Jet_jesp=false;
             if( Jets[3][ijet].pt()>30. ) JetVeto_jesp = false;
      }
      bool DphiMet_Jet_jesm = true, JetVeto_jesm = true;
      for(size_t ijet=0; ijet<Jets[4].size(); ijet++){
             if( fabs(deltaPhi(Jets[4][ijet].phi(), zvv.phi()))<0.5 && Jets[4][ijet].pt()>20. ) DphiMet_Jet_jesm=false;
             if( Jets[4][ijet].pt()>30. ) JetVeto_jesm = false;
      }

      //##############################################
      //########     GLOBAL SELECTION         ########
      //##############################################


	      //
	      // 3rd LEPTON ANALYSIS
	      //
int nextraleptons(0);
int nlept10 = 2, nlept15 = 2, nlept20 = 2;
vector<int> Ind3lept; Ind3lept.clear();
	      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++){
		  //lepton type
		  bool isGood(false);
		  int lpid=phys.leptons[ilep].pid;
		  if(fabs(phys.leptons[ilep].id)==13){
		      if(!use2011Id){
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT) && phys.leptons[ilep].pt()>3);
		      }else{
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT2011) && phys.leptons[ilep].pt()>3);
		      }
	          }else{
		      if(!use2011Id){
			isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid])<0.15 && phys.leptons[ilep].pt()>10);
		      }else{
			  isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
		      }
		  }
		  nextraleptons += isGood;
Ind3lept.push_back((int)ilep);
if(isGood && phys.leptons[ilep].pt() > 10) nlept10++; 
if(isGood && phys.leptons[ilep].pt() > 15) nlept15++; 
if(isGood && phys.leptons[ilep].pt() > 20) nlept20++; 

	      }


//      //
//      // 3rd LEPTON ANALYSIS
//      //
//      int nextraleptons(0);
//      int nlept10 = 2, nlept15 = 2, nlept20 = 2;
//      vector<int> Ind3lept; Ind3lept.clear();
//      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++){
//	  //lepton type
//	  bool isGood(false);
//	  int lpid=phys.leptons[ilep].pid;
//	  if(fabs(phys.leptons[ilep].id)==13){
//		isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
////		isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT2011) && phys.leptons[ilep].pt()>3);
//        }else{
//		isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
//	  }
//	  nextraleptons += isGood;
//        Ind3lept.push_back((int)ilep);
//        if(isGood && phys.leptons[ilep].pt() > 10) nlept10++; 
//        if(isGood && phys.leptons[ilep].pt() > 15) nlept15++; 
//        if(isGood && phys.leptons[ilep].pt() > 20) nlept20++; 
//      }
      //Using JER Smearing
      redMet = MetSmeared.Pt();
      redMetP4 = MetSmeared;

      bool passZmass(fabs(zmass-91.) < 10.); 
      bool passZmass_old(fabs(zmass-91.) < 7.); 
      bool passZpt(zpt>30);
      bool passBveto(nbtags_csvl==0); // 0.244
      bool JetVeto( Nj_30 == 0 ); //"eq0jets"
      double MetCut = 60.; bool RedMetPt( redMet > MetCut );  bool RedMetPt_old( redMet > 70 );
      bool passMediumRedMet(50); bool passTightRedMet(84);
      bool Balance( met/zpt > 0.4 && met/zpt < 1.8 );
      bool Dphi_J( DphiMet_Jet==0 );
      bool pass3dLeptonVeto(nextraleptons==0);

      bool RedMetJer( MetSmeared.pt() > MetCut ); bool RedMetJesm( MetSmeared_jesm.pt() > MetCut ); bool RedMetJesp( MetSmeared_jesp.pt() > MetCut );
      bool Balance_jer( zvvs[0].pt()/zpt > 0.4 && zvvs[0].pt()/zpt < 1.8 ), Balance_jesp( zvvs[3].pt()/zpt > 0.4 && zvvs[3].pt()/zpt < 1.8 ),
           Balance_jesm( zvvs[4].pt()/zpt > 0.4 && zvvs[4].pt()/zpt < 1.8 );

      bool Dil( (tag_cat == "ee" || tag_cat == "mumu") );
	bool PassPreSelectionRedMet( Dil  && passZpt && passBveto && JetVeto && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionRedMet( Dil && passZmass && passZpt && passBveto && JetVeto && RedMetPt && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionSysMetJesm( Dil && passZmass && passZpt && passBveto && JetVeto_jesm && RedMetJesm && Balance_jesm && DphiMet_Jet_jesm && pass3dLeptonVeto);
	bool PassSelectionSysMetJesp( Dil && passZmass && passZpt && passBveto && JetVeto_jesp && RedMetJesp && Balance_jesp && DphiMet_Jet_jesp && pass3dLeptonVeto);
	bool PassSelectionSysMet( Dil && passZmass && passZpt && passBveto && JetVeto_jer && RedMetJer && Balance_jer && DphiMet_Jet_jer && pass3dLeptonVeto);
	bool PassSelectionNoMet( Dil && passZmass && passZpt && passBveto && JetVeto && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionRedMetNoMass( Dil && passZpt && passBveto && JetVeto && RedMetPt && Balance && Dphi_J && pass3dLeptonVeto);
	//bool SelectionNo3LeptVeto( passZmass && passZpt && passBveto && JetVeto && RedMetPt && Balance && Dphi_J );
	//Control Sample BKG
	bool sidebands( /*(zmass>50 && zmass<70 ) ||*/ (zmass>110 && zmass<200 ) );
	bool WZ_ctrl  ( passZpt && nextraleptons==1 && passZmass && passBveto );
	bool WZ_ctrl_3  ( passZpt && passZmass && passBveto);
	bool WZ_ctrl_4  ( passZpt && passZmass && passBveto && nextraleptons==1);
	bool T_ctrl   ( passZpt && nbtags_csvl>0 && sidebands );
	bool T_ctrl2  ( passZpt && nbtags_csvl>0 );
	bool WW_ctrl  ( passZpt && passBveto && JetVeto && RedMetPt && Balance && Dphi_J && pass3dLeptonVeto );
	bool WW_ctrl2 ( passZpt && passBveto && JetVeto && Balance && Dphi_J && pass3dLeptonVeto && sidebands );
	bool WW_ctrl2_1 ( passZpt && sidebands );
	bool WW_ctrl2_2 ( passZpt && sidebands && passBveto );
	bool WW_ctrl2_3 ( passZpt && sidebands && passBveto && Balance && Nj_20==0 );
	bool WW_ctrl2_4 ( passZpt && sidebands && passBveto && Balance && Nj_20==0 && Dphi_J);
	bool WW_ctrl2_5 ( passZpt && sidebands && passBveto && Balance && Nj_20==0 && Dphi_J && pass3dLeptonVeto);
	bool W_ctrl   ( passZpt && (phys.leptons[0].id*phys.leptons[1].id)>0 && sidebands && passBveto );
      bool CtrlTT   ( nbtags_csvl>1 && tag_cat == "emu" && phys.leptons.size()==2 );

      bool pt_1(zpt > 70. && zpt < 90.);
      bool pt_2(zpt > 90. && zpt < 110.);
      bool pt_3(zpt > 110. && zpt < 140);
      bool pt_4(zpt > 140.);
      int  NvtxBin1 = 6, NvtxBin2 = 9, NvtxBin3 = 13;

      bool passLooseKinematics( zpt>20 );
      bool passSideBand( !passZmass && fabs(zmass-91)<30 );

      //##############################################
      //########         GENERAL PLOTS        ########
      //##############################################

 if( tag_cat!="??" ){

      if( passZmass && passZpt && passBveto && JetVeto && Balance && Dphi_J && pass3dLeptonVeto ){
          mon.fillHisto("Eff_MetCutOnTop_Before", tags_cat, 1. , iweight);
          if(RedMetPt) mon.fillHisto("Eff_MetCutOnTop_After", tags_cat, 1. , iweight);
      }

      //Initial Selection
      if( Dil ){
        mon.fillHisto("ZZ_in_Mt", tags_cat, mt , iweight);
        mon.fillHisto("ZZ_in_PtZZ", tags_cat, (zll+zvv).Pt() , iweight);
        mon.fillHisto("ZZ_in_DPhiZZ", tags_cat, fabs(deltaPhi( zll.Phi(),zvv.Phi() )) , iweight);
        mon.fillHisto("ZZ_in_DEtaZZ", tags_cat, fabs( zll.Eta()-zvv.Eta() ) , iweight);
        mon.fillHisto("ZZ_in_llMass", tags_cat, zmass, iweight);
        mon.fillHisto("ZZ_in_llPt", tags_cat, zpt, iweight);
        mon.fillHisto("ZZ_in_l1Pt", tags_cat,ptl1 , iweight);
        mon.fillHisto("ZZ_in_l2Pt", tags_cat, ptl2, iweight);
        mon.fillHisto("ZZ_in_MET1Pt", tags_cat, met, iweight);
        mon.fillHisto("ZZ_in_llEta", tags_cat, zeta, iweight);
        mon.fillHisto("ZZ_in_nvtx", tags_cat, ev.nvtx, iweight);
        mon.fillHisto("ZZ_in_njet", tags_cat, Nj_20 , iweight);
        mon.fillHisto("ZZ_in_nlept", tags_cat, nlept10 , iweight);
        mon.fillHisto("ZZ_in_MetRed_IND", tags_cat, redMet, iweight);
        mon.fillHisto("ZZ_in_MetRed_D0", tags_cat, redMetD0P4.Pt(), iweight);
        mon.fillHisto("ZZ_in_Balance", tags_cat, met/zpt, iweight);
        mon.fillHisto("ZZ_in_RedBalance", tags_cat, redMet/zpt, iweight);
        mon.fillHisto("ZZ_in_RedIndMet_L", tags_cat, RedIndMet_L, iweight);
        mon.fillHisto("ZZ_in_BalRedIndMet_L", tags_cat, RedIndMet_L/zpt, iweight);
        if( ev.l1_id == -(ev.l2_id) ) mon.fillHisto("ZZ_in_LeptId", tags_cat, 0, iweight);
        else mon.fillHisto("ZZ_in_LeptId", tags_cat, 1, iweight);
      }

       //FinalSelection
      if( PassSelectionRedMet ){
        mon.fillHisto("ZZ_fin_Mt", tags_cat, mt , iweight);
        mon.fillHisto("ZZ_fin_PtZZ", tags_cat, (zll+zvv).Pt() , iweight);
        mon.fillHisto("ZZ_fin_DPhiZZ", tags_cat, fabs(deltaPhi( zll.Phi(),zvv.Phi() )) , iweight);
        mon.fillHisto("ZZ_fin_DEtaZZ", tags_cat, fabs( zll.Eta()-zvv.Eta() ) , iweight);
        mon.fillHisto("ZZ_fin_llMass", tags_cat, zmass, iweight);
        mon.fillHisto("ZZ_fin_llPt", tags_cat, zpt, iweight);
        mon.fillHisto("ZZ_fin_l1Pt", tags_cat,ptl1 , iweight);
        mon.fillHisto("ZZ_fin_l2Pt", tags_cat, ptl2, iweight);
        mon.fillHisto("ZZ_fin_MET1Pt", tags_cat, met, iweight);
        mon.fillHisto("ZZ_fin_llEta", tags_cat, zeta, iweight);
        mon.fillHisto("ZZ_fin_nvtx", tags_cat, ev.nvtx, iweight);
        mon.fillHisto("ZZ_fin_njet", tags_cat, Jets[0].size() , iweight);
        mon.fillHisto("ZZ_fin_MetRed_IND", tags_cat, redMet, iweight);
        mon.fillHisto("ZZ_fin_MetRed_D0", tags_cat, redMetD0P4.Pt(), iweight);
      }
      if( PassSelectionRedMetNoMass ) mon.fillHisto("ZZ_finNonMass_llMass", tags_cat, zmass, iweight); 
      //Cut Optimization
      if( Dil && passZmass  ){
      mon.fillHisto("ZZ_cut_llPt", tags_cat, zpt, iweight);
       if( passZpt ){
      mon.fillHisto("ZZ_cut_Bveto", tags_cat, nbtags, iweight);
      mon.fillHisto("ZZ_cut_Bvetol", tags_cat, nbtags_tchel, iweight);
      mon.fillHisto("ZZ_cut_Zpt", tags_cat, zpt, iweight);
      mon.fillHisto("ZZ_cut_Zmass", tags_cat, zmass, iweight);
      mon.fillHisto("ZZ_cut_RedMetIND", tags_cat, redMet, iweight);
      mon.fillHisto("ZZ_cut_njet_aftPt_30", tags_cat, Nj_30 , iweight);
        if( passBveto ){
      mon.fillHisto("ZZ_cut_njet", tags_cat, Jets[0].size() , iweight);
      mon.fillHisto("ZZ_cut_njet_10", tags_cat, Nj_10 , iweight);
      mon.fillHisto("ZZ_cut_njet_15", tags_cat, Nj_15 , iweight);
      mon.fillHisto("ZZ_cut_njet_20", tags_cat, Nj_20 , iweight);
      mon.fillHisto("ZZ_cut_njet_25", tags_cat, Nj_25 , iweight);
      mon.fillHisto("ZZ_cut_njet_30", tags_cat, Nj_30 , iweight);
      mon.fillHisto("ZZ_cut_RedIndMet_L_yesJ", tags_cat, RedIndMet_L, iweight);
      mon.fillHisto("ZZ_cut_BalRedIndMet_L_yesJ", tags_cat, RedIndMet_L/zpt, iweight);
      if(MaxBtag>-9.) mon.fillHisto("ZZ_cut_NBtag_onlyMax", tags_cat, MaxBtag, iweight);
         if( JetVeto ){
      mon.fillHisto("ZZ_cut_MET1Pt", tags_cat, met, iweight);
      mon.fillHisto("ZZ_cut_MetRed_IND", tags_cat, redMet, iweight);
      mon.fillHisto("ZZ_cut_MetRed_D0", tags_cat, redMetD0P4.Pt(), iweight);
      mon.fillHisto("ZZ_cut_RedIndMet_L", tags_cat, RedIndMet_L, iweight);
      mon.fillHisto("ZZ_cut_BalRedIndMet_L", tags_cat, RedIndMet_L/zpt, iweight);
      mon.fillHisto("ZZ_cut_1Zpt", tags_cat, zpt, iweight);
      mon.fillHisto("ZZ_cut_1Zmass", tags_cat, zmass, iweight);
      mon.fillHisto("ZZ_cut_1RedMetIND", tags_cat, redMet, iweight);
          if( RedMetPt ){
      mon.fillHisto("ZZ_cut_Balance", tags_cat, met/zpt, iweight);
            if( Balance ){
            for(size_t ijet=0; ijet<Jets[0].size(); ijet++){
                if( Jets[0][ijet].pt()>20. ) mon.fillHisto("ZZ_cut_Dphi_val", tags_cat, fabs(deltaPhi(Jets[0][ijet].phi(), zvv.phi())), iweight);
            }
      mon.fillHisto("ZZ_cut_Dphi_J", tags_cat,  DphiMet_Jet, iweight);
      mon.fillHisto("ZZ_cut_2Zpt", tags_cat, zpt, iweight);
      mon.fillHisto("ZZ_cut_2Zmass", tags_cat, zmass, iweight);
      mon.fillHisto("ZZ_cut_2RedMetIND", tags_cat, redMet, iweight);
            if( Dphi_J ){
      mon.fillHisto("ZZ_cut_Nlept10", tags_cat, nlept10, iweight);
      mon.fillHisto("ZZ_cut_Nlept15", tags_cat, nlept15, iweight);
      mon.fillHisto("ZZ_cut_Nlept20", tags_cat, nlept20, iweight);
      mon.fillHisto("ZZ_cut_Nlept", tags_cat, nextraleptons+2, iweight);
          if( pass3dLeptonVeto ){
      mon.fillHisto("ZZ_cut_llMass", tags_cat, zmass, iweight);
      }}}}}}}}

   //Pt Bin
      if( PassSelectionRedMet && pt_1 ){      mon.fillHisto("ZZ_Pt1bin_llMass", tags_cat, zmass, iweight);
                                              mon.fillHisto("ZZ_eventflow_ptBin",tags_cat,0,iweight);      }
      if( PassSelectionRedMet && pt_2 ){      mon.fillHisto("ZZ_Pt2bin_llMass", tags_cat, zmass, iweight);
                                              mon.fillHisto("ZZ_eventflow_ptBin",tags_cat,1,iweight);      }
      if( PassSelectionRedMet && pt_3 ){      mon.fillHisto("ZZ_Pt3bin_llMass", tags_cat, zmass, iweight);
                                              mon.fillHisto("ZZ_eventflow_ptBin",tags_cat,2,iweight);      }
      if( PassSelectionRedMet && pt_4 ){      mon.fillHisto("ZZ_Pt4bin_llMass", tags_cat, zmass, iweight);
                                              mon.fillHisto("ZZ_eventflow_ptBin",tags_cat,3,iweight);      }
      //CTRL region WZ
      if(WZ_ctrl){
           mon.fillHisto("Ctrl_WZ_PFMet", tags_cat, met, iweight);
           mon.fillHisto("Ctrl_WZ_RedMet", tags_cat, redMet, iweight);
           mon.fillHisto("Ctrl_WZ_Mt", tags_cat, sqrt((2*zvv.pt()*phys.leptons[Ind3lept[0]].pt())*(1-cos(deltaPhi(zvv.phi(),phys.leptons[Ind3lept[0]].phi())))), iweight);
      }
      if(WZ_ctrl_3){
           mon.fillHisto("Ctrl_WZ_RedMet_3", tags_cat, redMet, iweight);
           mon.fillHisto("Ctrl_WZ_MLEPT", tags_cat, nextraleptons+2, iweight);
           mon.fillHisto("Ctrl_WZ_MLEPT_10", tags_cat, nlept10, iweight);
           mon.fillHisto("Ctrl_WZ_MLEPT_20", tags_cat, nlept20, iweight);
      }
      if(WZ_ctrl_4) mon.fillHisto("Ctrl_WZ_RedMet_4", tags_cat, redMet, iweight);
      if(T_ctrl){
           mon.fillHisto("Ctrl_T_PFMet", tags_cat, met, iweight);
           mon.fillHisto("Ctrl_T_RedMet", tags_cat, redMet, iweight);
      }
      if(T_ctrl2)  mon.fillHisto("Ctrl_T_Mass", tags_cat, zmass, iweight);

      if(WW_ctrl)  mon.fillHisto("Ctrl_WW_Mass", tags_cat, zmass, iweight);
      if(WW_ctrl2){
           mon.fillHisto("Ctrl_WW_PFMet", tags_cat, met, iweight);
           mon.fillHisto("Ctrl_WW_RedMet", tags_cat, redMet, iweight);
      }
      if(WW_ctrl2_1) mon.fillHisto("Ctrl_WW_RedMet1", tags_cat, redMet, iweight);
      if(WW_ctrl2_2) mon.fillHisto("Ctrl_WW_RedMet2", tags_cat, redMet, iweight);
      if(WW_ctrl2_3) mon.fillHisto("Ctrl_WW_RedMet3", tags_cat, redMet, iweight);
      if(WW_ctrl2_4) mon.fillHisto("Ctrl_WW_RedMet4", tags_cat, redMet, iweight);
      if(WW_ctrl2_5) mon.fillHisto("Ctrl_WW_RedMet5", tags_cat, redMet, iweight);
      if( W_ctrl ){
           mon.fillHisto("Ctrl_W_PFMet", tags_cat, met, iweight);
           mon.fillHisto("Ctrl_W_RedMet", tags_cat, redMet, iweight);
      }


  //Met Smearing for Eff Correctio
  //Applying smearing
    double Metsmeared = 0., MetSmearedm = 0., MetSmearedp = 0., MetSmearedX=0., MetSmearedY=0., MetSmearedXm=0., MetSmearedYm=0., MetSmearedXp=0., MetSmearedYp=0.;
    if(isMC){
      if(ev.nvtx < NvtxBin1){
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 9.59/sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 0./sqrt(2) );
         Metsmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 2.81/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 0./sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));
 
         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 16.37/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 5.08/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
      else if(ev.nvtx < NvtxBin2){
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 9.21/sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 12.67/sqrt(2) );
         Metsmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 2.76/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 6.9/sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));

         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 15.67/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 18.44/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
      else if(ev.nvtx < NvtxBin3){
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 14.06/sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 11.52/sqrt(2) );
         Metsmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 11.36/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 5.03/sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));

         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 16.75/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 18.02/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
      else{
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 10.255/sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 0./sqrt(2) );
         Metsmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 5.98/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 0./sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));

         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 14.52/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 8.41/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
    }
    else Metsmeared = redMet;
    if( tag_cat != "emu" ) mon.fillHisto("Sys_NewMetSmired",tags_cat,Metsmeared,iweight);
    if( CtrlTT )  mon.fillHisto("Sys_NewMetSmiredCyrt",tags_cat,Metsmeared,iweight);

   // Best Stability MET (Only Base cut)
    if(ZZllvvOrnotZZ){
       for(int i=0; i < 30; i++){
        if( Dil && (fabs(zmass-91) < 17) && passZpt  && passBveto && JetVeto ){
         if( i==0 ){
            mon.fillHisto("ZZ_RedMetInd_Best",tags_cat,i,iweight);
            mon.fillHisto("ZZ_RedMetD0_Best",tags_cat,i,iweight);
            mon.fillHisto("ZZ_PFMet_Best",tags_cat,i,iweight);

            mon.fillHisto("ZZ_RedMetInd_BestPUp",tags_cat,i,iweight*TotalWeight_plus);
            mon.fillHisto("ZZ_RedMetD0_BestPUp",tags_cat,i,iweight*TotalWeight_plus);
            mon.fillHisto("ZZ_PFMet_BestPUp",tags_cat,i,iweight*TotalWeight_plus);
            mon.fillHisto("ZZ_RedMetInd_BestPUm",tags_cat,i,iweight*TotalWeight_minus);
            mon.fillHisto("ZZ_RedMetD0_BestPUm",tags_cat,i,iweight*TotalWeight_minus);
            mon.fillHisto("ZZ_PFMet_BestPUm",tags_cat,i,iweight*TotalWeight_minus);

            mon.fillHisto("ZZ_RedMetInd_Best_Jesm",tags_cat,i,iweight);
            mon.fillHisto("ZZ_RedMetD0_Best_Jesm",tags_cat,i,iweight);
            mon.fillHisto("ZZ_PFMet_Best_Jesm",tags_cat,i,iweight);
            mon.fillHisto("ZZ_RedMetInd_Best_Jesp",tags_cat,i,iweight);
            mon.fillHisto("ZZ_RedMetD0_Best_Jesp",tags_cat,i,iweight);
            mon.fillHisto("ZZ_PFMet_Best_Jesp",tags_cat,i,iweight);
         }
         else{
            if( redMet > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetInd_Best",tags_cat,i,iweight);
            if( redMetD0P4.Pt() > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetD0_Best",tags_cat,i,iweight);
            if( met > StartCut+BinCut*i  )  mon.fillHisto("ZZ_PFMet_Best",tags_cat,i,iweight); 

            if( redMet > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetInd_BestPUp",tags_cat,i,iweight*TotalWeight_plus);
            if( redMetD0P4.Pt() > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetD0_BestPUp",tags_cat,i,iweight*TotalWeight_plus);
            if( met > StartCut+BinCut*i  )  mon.fillHisto("ZZ_PFMet_BestPUp",tags_cat,i,iweight*TotalWeight_plus); 
            if( redMet > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetInd_BestPUm",tags_cat,i,iweight*TotalWeight_minus);
            if( redMetD0P4.Pt() > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetD0_BestPUm",tags_cat,i,iweight*TotalWeight_minus);
            if( met > StartCut+BinCut*i  )  mon.fillHisto("ZZ_PFMet_BestPUm",tags_cat,i,iweight*TotalWeight_minus); 

            if( MetSmeared_jesm.pt()  > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetInd_Best_Jesm",tags_cat,i,iweight);
            if( D0MetSmeared_jesm.pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetD0_Best_Jesm",tags_cat,i,iweight);
            if( PFMetSmeared_jesm.pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_PFMet_Best_Jesm",tags_cat,i,iweight); 
            if( MetSmeared_jesp.pt()  > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetInd_Best_Jesp",tags_cat,i,iweight);
            if( D0MetSmeared_jesp.pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetD0_Best_Jesp",tags_cat,i,iweight);
            if( PFMetSmeared_jesp.pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_PFMet_Best_Jesp",tags_cat,i,iweight); 
         } 
        }
      }
    }

  // Cut Optimization
      StartCut = 20., EndCut = 80., BinCut = 2.;
      if( Dil && passZpt  && passBveto && JetVeto && (fabs(zmass-91)<20) ){
        for(int i=0; i < 30; i++){
          if( redMet > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetInd_Opt",tags_cat,i,iweight);
          if( redMetD0P4.Pt() > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetD0_Opt",tags_cat,i,iweight);
          if( redMet>46. && fabs(zmass-91) < 5+0.5*i )  mon.fillHisto("ZZ_Mass_Opt",tags_cat,i,iweight);   //
          if( redMet>46. && (fabs(zmass-91)<10) && fabs( deltaPhi(zll.phi(),zvv.phi()) ) > 1+i*0.071333 )  mon.fillHisto("ZZ_Dphi_Opt",tags_cat,i,iweight);  //1.0472
        }
      }

  // DY Fit
      if( Dil && passZmass && passZpt && passBveto && Balance && JetVeto && Dphi_J && pass3dLeptonVeto ){
       mon.fillHisto("Fit_RedMet_IND", tags_cat, redMet, iweight);
       mon.fillHisto("Fit_RedMet_D0", tags_cat, redMetD0P4.Pt(), iweight);
       mon.fillHisto("Fit_PFMet", tags_cat, met, iweight);
      }

     // Event Flow
      if( Dil ){
         mon.fillHisto("ZZ_eventflow",tags_cat,0,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,0,1.);
      if( passZmass ){
         mon.fillHisto("ZZ_eventflow",tags_cat,1,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,0,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,1,1.);
      if( passZpt ){    
         mon.fillHisto("ZZ_eventflow",tags_cat,2,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,1,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,2,1.);
      if( passBveto ){
         mon.fillHisto("ZZ_eventflow",tags_cat,3,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,2,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,3,1.);
      if( JetVeto ){
         mon.fillHisto("ZZ_eventflow",tags_cat,4,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,3,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,4,1.);
      if( RedMetPt ){
         mon.fillHisto("ZZ_eventflow",tags_cat,5,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,4,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,5,1.);
      if( Balance ){
         mon.fillHisto("ZZ_eventflow",tags_cat,6,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,6,1.);
      if( Dphi_J ){
         mon.fillHisto("ZZ_eventflow",tags_cat,7,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,5,iweight);
         mon.fillHisto("eventflow_NoWeight",tags_cat,7,1.);
      if( pass3dLeptonVeto ){
         mon.fillHisto("ZZ_eventflow",tags_cat,8,iweight);
         mon.fillHisto("ZZ_eventflow_red",tags_cat,6,iweight);
         mon.fillHisto("eventflow_Sys",tags_cat,0,iweight_trig_up);
         mon.fillHisto("eventflow_Sys",tags_cat,1,iweight_trig_dw);
         mon.fillHisto("eventflow_Sys",tags_cat,2,iweight_scal_up);
         mon.fillHisto("eventflow_Sys",tags_cat,3,iweight_scal_dw);
         mon.fillHisto("eventflow_NoWeight",tags_cat,8,1.);
      }}}}}}}}}

      //Cross Section
      Tot+=iweight;
      Tot_pup+=iweight*TotalWeight_plus;
      Tot_pum+=iweight*TotalWeight_minus;
      if(ZZllvvOrnotZZ){
         if( tag_cat == "ee" ){ NzzTot_ee+=iweight; /*NzzTotPUp_ee+=iweight*TotalWeight_plus;*/ NzzTotPUm_ee+=iweight*TotalWeight_minus; }
         if( tag_cat == "mumu" ){ NzzTot_mumu+=iweight; /*NzzTotPUp_mumu+=iweight*TotalWeight_plus;*/ NzzTotPUm_mumu+=iweight*TotalWeight_minus; }
         if( PassSelectionRedMet && tag_cat == "ee"  ){
             Nzz_ee+=iweight; NzzPUp_ee+=iweight*TotalWeight_plus; NzzPUm_ee+=iweight*TotalWeight_minus;
             NzzTrig_up_ee+=iweight_trig_up; NzzTrig_dw_ee+=iweight_trig_dw; NzzScal_up_ee+=iweight_scal_up; NzzScal_dw_ee+=iweight_scal_dw;
         }
         if( PassSelectionRedMet && tag_cat == "mumu"  ){
             Nzz_mumu+=iweight; NzzPUp_mumu+=iweight*TotalWeight_plus; NzzPUm_mumu+=iweight*TotalWeight_minus;
             NzzTrig_up_mu+=iweight_trig_up; NzzTrig_dw_mu+=iweight_trig_dw; NzzScal_up_mu+=iweight_scal_up; NzzScal_dw_mu+=iweight_scal_dw;
         }
         if( PassSelectionSysMet && tag_cat == "ee"  ) NzzJer_ee+=iweight;
         if( PassSelectionSysMet && tag_cat == "mumu"  ) NzzJer_mumu+=iweight;
         if( PassSelectionSysMetJesm && tag_cat == "ee"  ) NzzJesm_ee+=iweight;
         if( PassSelectionSysMetJesm && tag_cat == "mumu"  ) NzzJesm_mumu+=iweight;
         if( PassSelectionSysMetJesp && tag_cat == "ee"  ) NzzJesp_ee+=iweight;
         if( PassSelectionSysMetJesp && tag_cat == "mumu"  ) NzzJesp_mumu+=iweight;

         if( PassSelectionRedMet && pt_1 && tag_cat == "ee"  ) Nzz_ee_ptBin1+=iweight;
         if( PassSelectionRedMet && pt_1 && tag_cat == "mumu"  ) Nzz_mumu_ptBin1+=iweight;
         if( PassSelectionRedMet && pt_2 && tag_cat == "ee"  ) Nzz_ee_ptBin2+=iweight;
         if( PassSelectionRedMet && pt_2 && tag_cat == "mumu"  ) Nzz_mumu_ptBin2+=iweight;
         if( PassSelectionRedMet && pt_3 && tag_cat == "ee"  ) Nzz_ee_ptBin3+=iweight;
         if( PassSelectionRedMet && pt_3 && tag_cat == "mumu"  ) Nzz_mumu_ptBin3+=iweight;
         if( PassSelectionRedMet && pt_4 && tag_cat == "ee"  ) Nzz_ee_ptBin4+=iweight;
         if( PassSelectionRedMet && pt_4 && tag_cat == "mumu"  ) Nzz_mumu_ptBin4+=iweight;
         //MetSmearing
         if( PassSelectionNoMet && Metsmeared > MetCut && tag_cat == "ee"  )  Nzz_MetSmear_ee+=iweight;
         if( PassSelectionNoMet && Metsmeared > MetCut && tag_cat == "mumu"  )  Nzz_MetSmear_mumu+=iweight;
         if( PassSelectionNoMet && MetSmearedm > MetCut && tag_cat == "ee"  )  Nzz_MetSmearm_ee+=iweight;
         if( PassSelectionNoMet && MetSmearedm > MetCut && tag_cat == "mumu"  )  Nzz_MetSmearm_mumu+=iweight;
         if( PassSelectionNoMet && MetSmearedp > MetCut && tag_cat == "ee"  )  Nzz_MetSmearp_ee+=iweight;
         if( PassSelectionNoMet && MetSmearedp > MetCut && tag_cat == "mumu"  )  Nzz_MetSmearp_mumu+=iweight;
       }

 //Met Systematic & Met eff correction
          mon.fillHisto("Sys_MetEff_Mass",tags_cat, zmass, iweight);
      if( CtrlTT ){
          mon.fillHisto("Sys_MetEff_Purity",tags_cat, 0, iweight);
          mon.fillHisto("Sys_MetEff_Nlept",tags_cat,  phys.leptons.size(), iweight);
          mon.fillHisto("Sys_MetEff_unclMet",tags_cat, unclMet.pt(), iweight);
          mon.fillHisto("Sys_MetEff_deltaPhiJet",tags_cat, fabs(deltaPhiJet), iweight);
          mon.fillHisto("Sys_MetEff_Met",tags_cat, redMet, iweight);
          mon.fillHisto("Sys_MetEff_sumEt",tags_cat, sumEt, iweight);
          mon.fillHisto("Sys_MetEff_Nvtx",tags_cat, ev.nvtx, iweight);

 //Plot with different smearing
          if( ev.nvtx<NvtxBin1  ){
            mon.fillHisto("Sys_MetEff_RedMet_px1nvtx",tags_cat, redMet*cos(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_py1nvtx",tags_cat, redMet*sin(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_1nvtx",tags_cat, redMet, iweight);
          }
          else if( ev.nvtx<NvtxBin2  ){
            mon.fillHisto("Sys_MetEff_RedMet_px2nvtx",tags_cat, redMet*cos(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_py2nvtx",tags_cat, redMet*sin(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_2nvtx",tags_cat, redMet, iweight);
          }
          else if( ev.nvtx<NvtxBin3  ){
            mon.fillHisto("Sys_MetEff_RedMet_px3nvtx",tags_cat, redMet*cos(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_py3nvtx",tags_cat, redMet*sin(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_3nvtx",tags_cat, redMet, iweight);
          }
          else{
            mon.fillHisto("Sys_MetEff_RedMet_px4nvtx",tags_cat, redMet*cos(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_py4nvtx",tags_cat, redMet*sin(redMetP4.phi()), iweight);
            mon.fillHisto("Sys_MetEff_RedMet_4nvtx",tags_cat, redMet, iweight);
          }

          mon.fillHisto("Sys_MetEff_RedMetPt",tags_cat,  sqrt( pow(redMetP4.px(),2)+pow(redMetP4.py(),2) ), iweight);
           if( fabs(deltaPhiJet) > 1.5 ) mon.fillHisto("Sys_MetEff_Met_Phip",tags_cat, redMet, iweight);
           if( fabs(deltaPhiJet) < 1.5 ) mon.fillHisto("Sys_MetEff_Met_Phim",tags_cat, redMet, iweight);
           if( Nj_30 > 2 ) mon.fillHisto("Sys_MetEff_Met_Nj",tags_cat, redMet, iweight);
          //Best Smearing
          float MetSmearedX=0., MetSmearedY=0.;
          if(/*isMC && DatasetIn.Contains("MC_TTJ")*/true ){
             double sigma=0.;
             for(int isigma=0; isigma<100; isigma++){
                sigma = double(isigma)/2.;
                MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()) ,sigma );
                MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()) ,sigma );
                if( ev.nvtx<NvtxBin1 ){
                  Sys_MetSmearedOnLine_px1vtx[isigma]->Fill(MetSmearedX,iweight);
                  Sys_MetSmearedOnLine_py1vtx[isigma]->Fill(MetSmearedY,iweight);
                  Sys_MetSmearedOnLine_1vtx[isigma]->Fill(sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2)));
                }
                else if( ev.nvtx<NvtxBin2 ){
                  Sys_MetSmearedOnLine_px2vtx[isigma]->Fill(MetSmearedX,iweight);
                  Sys_MetSmearedOnLine_py2vtx[isigma]->Fill(MetSmearedY,iweight);
                  Sys_MetSmearedOnLine_2vtx[isigma]->Fill(sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2)));
                }
                else if( ev.nvtx<NvtxBin3 ){
                  Sys_MetSmearedOnLine_px3vtx[isigma]->Fill(MetSmearedX,iweight);
                  Sys_MetSmearedOnLine_py3vtx[isigma]->Fill(MetSmearedY,iweight);
                  Sys_MetSmearedOnLine_3vtx[isigma]->Fill(sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2)));
                }
                else{
                  Sys_MetSmearedOnLine_px4vtx[isigma]->Fill(MetSmearedX,iweight);
                  Sys_MetSmearedOnLine_py4vtx[isigma]->Fill(MetSmearedY,iweight);
                  Sys_MetSmearedOnLine_4vtx[isigma]->Fill(sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2)));
                }
             }
          }
      }

      //##############################################
      //########     PLOTS FOR OPTIMIZATION   ########
      //##############################################

      if( Dil && passZpt  && passBveto && JetVeto && (fabs(zmass-91)<20) ){
         for(unsigned int index=0;index<Optim_Cuts1_met.size();index++){
                 if(MetSmeared.pt() >Optim_Cuts1_met[index] && zll.mass()>optim_Cuts1_dmz[index] )
            mon.fillHisto( "optim_eventflow1" , tags_full,    index, weight);
         }
       }

}// ?? VETO

      if(passZmass)                                                                 mon.fillHisto("zmassregionCtr",tags_full, 0,iweight);
      if(passZmass    && !passBveto)                                                mon.fillHisto("zmassregionCtr",tags_full, 1,iweight);
      if(passZmass    && !passBveto && pass3dLeptonVeto)                            mon.fillHisto("zmassregionCtr",tags_full, 2,iweight);
      if(passZmass    && !passBveto && pass3dLeptonVeto && passMediumRedMet)        mon.fillHisto("zmassregionCtr",tags_full, 3,iweight);
      if(passZmass    && !passBveto && pass3dLeptonVeto && passTightRedMet)         mon.fillHisto("zmassregionCtr",tags_full, 4,iweight);
      if(passSideBand)                                                              mon.fillHisto("zmassregionCtr",tags_full, 5,iweight);
      if(passSideBand && !passBveto)                                                mon.fillHisto("zmassregionCtr",tags_full, 6,iweight);
      if(passSideBand && !passBveto && pass3dLeptonVeto)                            mon.fillHisto("zmassregionCtr",tags_full, 7,iweight);
      if(passSideBand && !passBveto && pass3dLeptonVeto && passMediumRedMet)        mon.fillHisto("zmassregionCtr",tags_full, 8,iweight);
      if(passSideBand && !passBveto && pass3dLeptonVeto && passTightRedMet)         mon.fillHisto("zmassregionCtr",tags_full, 9,iweight);
      if(passZmass    && passBveto)                                                 mon.fillHisto("zmassregionCtr",tags_full,10,iweight);
      if(passZmass    && passBveto && pass3dLeptonVeto)                             mon.fillHisto("zmassregionCtr",tags_full,11,iweight);
      if(passZmass    && passBveto && pass3dLeptonVeto && passMediumRedMet)         mon.fillHisto("zmassregionCtr",tags_full,12,iweight);
      if(passZmass    && passBveto && pass3dLeptonVeto && passTightRedMet)          mon.fillHisto("zmassregionCtr",tags_full,13,iweight);
      if(passSideBand && passBveto)                                                 mon.fillHisto("zmassregionCtr",tags_full,14,iweight);
      if(passSideBand && passBveto && pass3dLeptonVeto)                             mon.fillHisto("zmassregionCtr",tags_full,15,iweight);
      if(passSideBand && passBveto && pass3dLeptonVeto && passMediumRedMet)         mon.fillHisto("zmassregionCtr",tags_full,16,iweight);
      if(passSideBand && passBveto && pass3dLeptonVeto && passTightRedMet)          mon.fillHisto("zmassregionCtr",tags_full,17,iweight);

      //dilepton control plots
      mon.fillHisto  ("zmass"    ,tags_full,zmass   ,iweight);
      mon.fillHisto  ("zeta"     ,tags_full,zeta    ,iweight);
      mon.fillHisto  ("zpt"      ,tags_full,zpt     ,iweight);
     // mon.fillHisto  ("zrank"    ,tags_full,zrank   ,iweight);
      mon.fillHisto("zptvszeta",tags_full,zpt,zeta,iweight);
      mon.fillHisto  ("dphill"   ,tags_full,dphill  ,iweight);
      mon.fillHisto  ("mindrlz"  ,tags_full,mindrlz ,iweight);
      mon.fillHisto  ("maxdrlz"  ,tags_full,maxdrlz ,iweight);
      mon.fillHisto  ("ptsum"    ,tags_full,ptsum   ,iweight);
      mon.fillHisto  ("mtl1"     ,tags_full,mtl1    ,iweight);
      mon.fillHisto  ("mtl2"     ,tags_full,mtl2    ,iweight);
      mon.fillHisto  ("mt"       ,tags_full,mt      ,iweight);
    
      if(passZmass && passSideBand && passLooseKinematics){
         mon.fillHisto("eventflow",tags_full,1,iweight);
         mon.fillHisto("nvtx",tags_full,ev.nvtx,iweight);
         mon.fillHisto("nvtxPlus",tags_full,ev.nvtx,iweight*TotalWeight_plus);
         mon.fillHisto("nvtxMinus",tags_full,ev.nvtx,iweight*TotalWeight_minus);
      }
      
      if(passZmass && passLooseKinematics){
         mon.fillHisto("eventflow",tags_full,2,iweight);
     
      //jet control
         mon.fillHisto("njets",tags_full,njets,iweight);
         mon.fillHisto("nbtags",tags_full, nbtags,iweight);
         mon.fillHisto("npassbveto",tags_full,0, (nbtags_tchel==0)*iweight);
         mon.fillHisto("npassbveto",tags_full,1, (nbtags_tche2==0)*iweight);
         mon.fillHisto("npassbveto",tags_full,2, (nbtags_csvl==0)*iweight);
         mon.fillHisto("zmassctrl",tags_full,passBveto+2*passMediumRedMet,iweight);
      
         mon.fillHisto("btag0proj",tags_full,0,iweight*p0btags);
         mon.fillHisto("btag0projup",tags_full,0,iweight*min(p0btags+p0btags_err,1.0));
         mon.fillHisto("btag0projdown",tags_full,0,iweight*max(p0btags-p0btags_err,0.0));

         if(passBveto){
            mon.fillHisto("eventflow",tags_full,3,iweight);

            if(passZpt){
               mon.fillHisto("eventflow",tags_full,4,iweight);

               if(pass3dLeptonVeto){                 
                  mon.fillHisto("eventflow",tags_full,5,iweight);
               
                  //sample is selected
                  if(passMediumRedMet){
                      mon.fillHisto("eventflow",tags_full,6,iweight);
                      mon.fillHisto("eventSubCat",tags_full,eventSubCat,iweight);
                      if(passTightRedMet)    mon.fillHisto("eventflow",tags_full,7,iweight);
                  }
               }
            }
         }
      }
/*
      // ################   The FINAL OPTIMIZATION ##############     
      // Base Met
      LorentzVector zvvRaw(phys.met[0]);
      LorentzVector rawClusteredMet(zll);            rawClusteredMet *= -1;
      for(size_t ijet=0; ijet<jetsP4.size(); ijet++) rawClusteredMet -= jetsP4[ijet];
      LorentzVector redMetRaw(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, rawClusteredMet, zvvRaw,false));
      //prepare variations (first variation is the baseline, corrected for JER) 
      LorentzVectorCollection zvvs1,redMets, min3Mets;
      std::vector<Float_t>  mts,mt3s,redMetLs,redMetTs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation( phys.ajets, phys.leptons, zvvRaw, jets, zvvs1, &jecUnc);
      for(size_t ivars=0; ivars<zvvs1.size(); ivars++){
	  LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
	  for(size_t ijet=0; ijet<jets[ivars].size(); ijet++) clusteredMetP4 -= jets[ivars][ijet];
	  METUtils::stRedMET redMetOut;
	  redMets.push_back(   METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4, zvvs1[ivars],false,&redMetOut) );
	  redMetLs.push_back( redMetOut.redMET_l );
	  redMetTs.push_back( redMetOut.redMET_t );
	  mts.push_back(METUtils::transverseMass(zll,zvvs1[ivars],true));
	  mt3s.push_back(phys.leptons.size()>2 ? METUtils::transverseMass(phys.leptons[2],zvvs1[ivars],false) : 0. );
	  min3Mets.push_back( min(zvvs1[ivars], min(assocMetP4,clusteredMetP4)) );
	}

      //
      //run the variations
      //
      for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
	float iweight = weight;                                               //nominal
	if(ivar==5)                        iweight *=TotalWeight_plus;        //pu up
	if(ivar==6)                        iweight *=TotalWeight_minus;       //pu down
	if(ivar<=10 && ivar>=7 && isMC_GG) iweight *=ev.hptWeights[ivar-6];   //ren/fact scales

	int nAJetsLoose(0);
	Float_t zmass=zll.mass();
	Float_t zpt=zll.pt();
	PhysicsObjectJetCollection &origJetsP4=jets[ivar>4?0:ivar];            
	LorentzVector zvv    = zvvs1[ivar>4?0:ivar];
	LorentzVector min3Met = min3Mets[ivar>4?0:ivar];
	LorentzVector RedMet = redMets[ivar>4?0:ivar];
	Float_t mt           = mts[ivar>4?0:ivar];
	int nbtags(0);
	Float_t mindphijmet(9999.);
	Float_t btagcut(2.0); if(ivar==10) btagcut=2.03; else if(ivar==11) btagcut=1.97;
	for(size_t ijet=0; ijet<origJetsP4.size(); ijet++){
	  Float_t idphijmet=fabs(deltaPhi(zvv.phi(),origJetsP4[ijet].phi()));
	  if(idphijmet<mindphijmet) mindphijmet=idphijmet;
	   nAJetsLoose += (phys.ajets[ijet].pt()>30);
	  if(origJetsP4[ijet].pt()>30){
            if( fabs(phys.ajets[ijet].eta())<2.5 ){
 	       nbtags += (phys.ajets[ijet].btag2>btagcut);                                                                                                                                
            } 
	  }
	}
	float balance=zvv.pt()/zll.pt();

	//##############################################
	//########     PRESELECTION             ########
	//##############################################
	if(zmass<40) continue; // this is required by the HZZ skim anyhow
	bool passDphijmet(mindphijmet>0.5); 
	bool passZpt(zpt>30);
	bool pass3dLeptonVeto(true); int nExtraLep(0); for(unsigned int i=2;i<phys.leptons.size();i++) { if(phys.leptons[i].pt()>10){ nExtraLep++; pass3dLeptonVeto=false;} }
	bool passBveto(nbtags==0);
	bool isZsideBand( (zll.mass()>40 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200));
	bool isZsideBandPlus( (zll.mass()>110 && zll.mass()<200));
	bool passLocalBalance(balance>0.4 && balance<1.8);
	bool passLocalDphijmet(mindphijmet>0.5);
	bool passLocalJetVeto(nAJetsLoose==0);

	  //Fill histogram for posterior optimization, or for control regions
        bool passPreselection             (passZpt && pass3dLeptonVeto && passDphijmet && passBveto && Balance);
          for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
	   float deltaZ=optim_Cuts1_zmass[index];
	   float minMet=optim_Cuts1_met[index];
	   float minZpt=30.;
	   bool passLocalMet(redMet>minMet);
	   bool passLocalZmass(fabs(zll.mass()-91)<deltaZ);
	   bool passLocalZpt(zll.pt()>minZpt);
         bool passLocalPreselectionMbvetoMzmass(pass3dLeptonVeto && passLocalDphijmet  && passLocalJetVeto && passLocalBalance  && passLocalZpt && passLocalMet);
	    
	      if(RedMet.pt()>optim_Cuts1_met[index] && fabs(zmass-91.)<optim_Cuts1_zmass[index] && mt<optim_Cuts1_mtmax[index])
	        if( passPreselection )          mon.fillHisto(TString("mt_redMet_shape")+varNames[ivar],tags_full,index, mt,iweight);
   
	      if(RedMet.pt()>optim_Cuts1_met[index] && fabs(zmass-91.)<optim_Cuts1_zmass[index] ){
		  if(PassPreSelectionRedMet                          )   mon.fillHisto(TString("mt_shape")+varNames[ivar],tags_full,index, RedMet.pt(),iweight);
	      }

            if(passLocalPreselectionMbvetoMzmass && passLocalZmass    && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,0,iweight);
            if(passLocalPreselectionMbvetoMzmass && isZsideBand       && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,1,iweight);
            if(passLocalPreselectionMbvetoMzmass && isZsideBandPlus   && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,2,iweight);
            if(passLocalPreselectionMbvetoMzmass && passLocalZmass    && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,3,iweight);
            if(passLocalPreselectionMbvetoMzmass && isZsideBand       && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,4,iweight);
            if(passLocalPreselectionMbvetoMzmass && isZsideBandPlus   && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,5,iweight);
         }
      }
*/


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
	 PhysicsObjectJetCollection &varJets=Jets[ivar>4 ? 0  : ivar];
	 PhysicsObjectJetCollection tightVarJets;
	 LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
	 int nAJetsLoose(0),nABtags(0);
	 float mindphijmet(999999.),mindphijmet15(999999.);
	 for(size_t ijet=0; ijet<varJets.size(); ijet++){
	   clusteredMetP4 -= varJets[ijet];
	   if(!hasObjectId(varJets[ijet].pid,JETID_LOOSE)) continue;
	   float idphijmet( fabs(deltaPhi(aJets[ijet].phi(),zvvs[0].phi()) ) );
	   if(idphijmet<mindphijmet15)  mindphijmet15=idphijmet;
	   if(aJets[ijet].pt()>30) if(idphijmet<mindphijmet)  mindphijmet=idphijmet;
	   tightVarJets.push_back( varJets[ijet] );
	   nAJetsLoose += (varJets[ijet].pt()>30);
	   if(varJets[ijet].pt()<20 || fabs(varJets[ijet].eta())>2.5)continue;
	   if(ivar==11)      nABtags += (varJets[ijet].btag2<0.250);
	   else if(ivar==12) nABtags += (varJets[ijet].btag2<0.240);
	   else              nABtags += (varJets[ijet].btag2<0.244);
	 }
	 float mt = METUtils::transverseMass(zll,zvv,true);
	 METUtils::stRedMET redMetInfo;
	 LorentzVector redMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0., lep2, 0., clusteredMetP4, zvv,false,&redMetInfo);
	 float balance=zvv.pt()/zll.pt();

	 bool isZsideBand( (zll.mass()>50 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200));
	 bool isZsideBandPlus( (zll.mass()>110 && zll.mass()<200));	 
	 bool passLocalBveto(nABtags==0);
	 bool passLocalJetVeto(nAJetsLoose==0);
	 bool passLocalBalance(balance>0.4 && balance<1.8);
	 bool passLocalDphijmet(mindphijmet>0.5);
	 if(nAJetsLoose==0) Dphi_J=(mindphijmet15>0.5);
	 
	 for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
	   float minMet=optim_Cuts1_met[index];
	   float minZpt=30.;
	   float deltaZ=optim_Cuts1_zmass[index];
	   bool passLocalZmass(fabs(zll.mass()-91)<deltaZ);
	   bool passLocalMet(redMet.pt()>minMet);
	   bool passLocalZpt(zll.pt()>minZpt);
	   bool passLocalPreselection            (pass3dLeptonVeto && passLocalDphijmet && passLocalBveto && passLocalJetVeto && passLocalBalance && passLocalZmass && passLocalZpt && passLocalMet);
	   bool passLocalPreselectionMbvetoMzmass(pass3dLeptonVeto && passLocalDphijmet                   && passLocalJetVeto && passLocalBalance                   && passLocalZpt && passLocalMet);
	   
	   if(passLocalPreselection)
	     {
	       mon.fillHisto(TString("mt_redMet_shape")+varNames[ivar],tags_full,index,redMet.pt(),iweight);
	       mon.fillHisto(TString("mt_shape")+varNames[ivar],tags_full,index,mt,iweight);
	     }
	   
	   if(passLocalPreselectionMbvetoMzmass && passLocalZmass    && passLocalBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,0,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBand       && passLocalBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,1,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBandPlus   && passLocalBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,2,iweight);
	   if(passLocalPreselectionMbvetoMzmass && passLocalZmass    && !passLocalBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,3,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBand       && !passLocalBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,4,iweight);
	   if(passLocalPreselectionMbvetoMzmass && isZsideBandPlus   && !passLocalBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,5,iweight);
	 }
       }
  }


      //##############################################   EVENT LOOP ENDS   ##############################################
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
  Norm_pup->Fill(0.,Tot_pup);
  Norm_pum->Fill(0.,Tot_pum);
  Norm_pu->Fill(0.,Tot);
  ee_Effic_tot->Fill(0.,NzzTot_ee);
  ee_Effic->Fill(0.,Nzz_ee);
  mumu_Effic_tot->Fill(0.,NzzTot_mumu);
  mumu_Effic->Fill(0.,Nzz_mumu);
  //PU syst
  ee_EfficPUm_tot->Fill(0.,NzzTotPUm_ee);
  ee_EfficPUm->Fill(0.,NzzPUm_ee);
  mumu_EfficPUm_tot->Fill(0.,NzzTotPUm_mumu);
  mumu_EfficPUm->Fill(0.,NzzPUm_mumu);
  ee_EfficPUp->Fill(0.,NzzPUp_ee);
  ee_EfficTrig_up->Fill(0.,NzzTrig_up_ee);
  ee_EfficTrig_dw->Fill(0.,NzzTrig_dw_ee);
  ee_EfficScal_up->Fill(0.,NzzScal_up_ee);
  ee_EfficScal_dw->Fill(0.,NzzScal_dw_ee);
  mumu_EfficPUp->Fill(0.,NzzPUp_mumu);
  mumu_EfficTrig_up->Fill(0.,NzzTrig_up_mu);
  mumu_EfficTrig_dw->Fill(0.,NzzTrig_dw_mu);
  mumu_EfficScal_up->Fill(0.,NzzScal_up_mu);
  mumu_EfficScal_dw->Fill(0.,NzzScal_dw_mu);
  //Jer Jes
  ee_EfficJer->Fill(0.,NzzJer_ee);
  mumu_EfficJer->Fill(0.,NzzJer_mumu);
  ee_EfficJesm->Fill(0.,NzzJesm_ee);
  mumu_EfficJesm->Fill(0.,NzzJesm_mumu);
  ee_EfficJesp->Fill(0.,NzzJesp_ee);
  mumu_EfficJesp->Fill(0.,NzzJesp_mumu);
  //Pt bin
  ee_Effic_ptBin1->Fill(0.,Nzz_ee_ptBin1);
  mumu_Effic_ptBin1->Fill(0.,Nzz_mumu_ptBin1);
  ee_Effic_ptBin2->Fill(0.,Nzz_ee_ptBin2);
  mumu_Effic_ptBin2->Fill(0.,Nzz_mumu_ptBin2);
  ee_Effic_ptBin3->Fill(0.,Nzz_ee_ptBin3);
  mumu_Effic_ptBin3->Fill(0.,Nzz_mumu_ptBin3);
  ee_Effic_ptBin4->Fill(0.,Nzz_ee_ptBin4);
  mumu_Effic_ptBin4->Fill(0.,Nzz_mumu_ptBin4);
  //Met Smearing
  ee_Effic_MetSmear->Fill(0.,Nzz_MetSmear_ee);
  mumu_Effic_MetSmear->Fill(0.,Nzz_MetSmear_mumu);
  ee_Effic_MetSmearm->Fill(0.,Nzz_MetSmearm_ee);
  mumu_Effic_MetSmearm->Fill(0.,Nzz_MetSmearm_mumu);
  ee_Effic_MetSmearp->Fill(0.,Nzz_MetSmearp_ee);
  mumu_Effic_MetSmearp->Fill(0.,Nzz_MetSmearp_mumu);

  ee_Effic_tot->Write();
  ee_Effic->Write();
  mumu_Effic_tot->Write();
  mumu_Effic->Write();
  Norm_pup->Write();
  Norm_pum->Write();
  Norm_pu->Write();
  //PU 
  ee_EfficPUm_tot->Write();
  ee_EfficPUm->Write();
  mumu_EfficPUm_tot->Write();
  mumu_EfficPUm->Write();
  ee_EfficPUp->Write();
  ee_EfficTrig_up->Write();
  ee_EfficTrig_dw->Write();
  ee_EfficScal_up->Write();
  ee_EfficScal_dw->Write();
  mumu_EfficPUp->Write();
  mumu_EfficTrig_up->Write();
  mumu_EfficTrig_dw->Write();
  mumu_EfficScal_up->Write();
  mumu_EfficScal_dw->Write();
  //JER JES
  ee_EfficJer->Write();
  mumu_EfficJer->Write();
  ee_EfficJesm->Write();
  mumu_EfficJesm->Write();
  ee_EfficJesp->Write();
  mumu_EfficJesp->Write();
  //PT Bin
  ee_Effic_ptBin1->Write();
  mumu_Effic_ptBin1->Write();
  ee_Effic_ptBin2->Write();
  mumu_Effic_ptBin2->Write();
  ee_Effic_ptBin3->Write();
  mumu_Effic_ptBin3->Write();
  ee_Effic_ptBin4->Write();
  mumu_Effic_ptBin4->Write();
  //Met Smearing
  ee_Effic_MetSmear->Write();
  mumu_Effic_MetSmear->Write();
  ee_Effic_MetSmearm->Write();
  mumu_Effic_MetSmearm->Write();
  ee_Effic_MetSmearp->Write();
  mumu_Effic_MetSmearp->Write();
  //Smearinf On Line
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_1vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_2vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_3vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_4vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_px1vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_px2vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_px3vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_px4vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_py1vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_py2vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_py3vtx[jR]->Write(); }
//@@  for(int jR=0; jR<100; jR++){ Sys_MetSmearedOnLine_py4vtx[jR]->Write(); }

  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);

  delete Gaussian;
}  

// pfjet resolutions. taken from AN-2010-371
double ErrEt( double Et, double Eta) {
 
 double InvPerr2;

 double N, S, C, m;
 if(fabs(Eta) < 0.5 ) {
   N = 3.96859;
   S = 0.18348;
   C = 0.;
   m = 0.62627;
 } else if( fabs(Eta) < 1. ) {
   N = 3.55226;
   S = 0.24026;
   C = 0.;
   m = 0.52571;
 } else if( fabs(Eta) < 1.5 ) {
   N = 4.54826;
   S = 0.22652;
   C = 0.;
   m = 0.58963;
 } else if( fabs(Eta) < 2. ) {
   N = 4.62622;
   S = 0.23664;
   C = 0.;
   m = 0.48738;
 } else if( fabs(Eta) < 3. ) {
   N = 2.53324;
   S = 0.34306;
   C = 0.;
   m = 0.28662;
 } else if( fabs(Eta) < 5. ) {
   N = 2.95397;
   S = 0.11619;
   C = 0.;
   m = 0.96086;
 }
 // this is the absolute resolution (squared), not sigma(pt)/pt
 // so have to multiply by pt^2, thats why m+1 instead of m-1
 InvPerr2 =  (N * fabs(N) ) + (S * S) * pow(Et, m+1.) + (C * C) * Et * Et ;
 return sqrt(InvPerr2)/Et;
}
