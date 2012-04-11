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
 
using namespace std;

double ErrEt( double Et, double Eta);

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
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);

  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");
  
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
  h->GetXaxis()->SetBinLabel(4,"b-veto (2)");
  h->GetXaxis()->SetBinLabel(5,"Jet-Veto (pt>30)");
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>45");
  h->GetXaxis()->SetBinLabel(7,"Balance Cut");
  h->GetXaxis()->SetBinLabel(8,"Delta phi (Z-Met)");
  h->GetXaxis()->SetBinLabel(9,"3^{rd}-lepton veto");
  //h->GetXaxis()->SetBinLabel(10,"Opp. flavor");
  mon.addHistogram( h );

  //PU Systematic
  h=new TH1F ("eventflow_minus", ";;Events", 9,0,9);
  h->GetXaxis()->SetBinLabel(1,"Preselected");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<10");
  h->GetXaxis()->SetBinLabel(3,"Z_{pT}>30");
  h->GetXaxis()->SetBinLabel(4,"b-veto (2)");
  h->GetXaxis()->SetBinLabel(5,"Jet-Veto (pt>30)");
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>45");
  h->GetXaxis()->SetBinLabel(7,"Balance Cut");
  h->GetXaxis()->SetBinLabel(8,"Delta phi (Z-Met)");
  h->GetXaxis()->SetBinLabel(9,"3^{rd}-lepton veto");
  mon.addHistogram( h );
  h=new TH1F ("eventflow_plus", ";;Events", 9,0,9);
  h->GetXaxis()->SetBinLabel(1,"Preselected");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<10");
  h->GetXaxis()->SetBinLabel(3,"Z_{pT}>30");
  h->GetXaxis()->SetBinLabel(4,"b-veto (2)");
  h->GetXaxis()->SetBinLabel(5,"Jet-Veto (pt>30)");
  h->GetXaxis()->SetBinLabel(6,"red-E_{T}^{miss}>45");
  h->GetXaxis()->SetBinLabel(7,"Balance Cut");
  h->GetXaxis()->SetBinLabel(8,"Delta phi (Z-Met)");
  h->GetXaxis()->SetBinLabel(9,"3^{rd}-lepton veto");
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
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ) ;

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
// Preselection
  mon.addHistogram( new TH1F("ZZ_in_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_in_llPt", ";Transvers momentum of lepton pair [GeV];Events", 30, 25., 325.) );
  mon.addHistogram( new TH1F("ZZ_in_l1Pt", ";Transvers momentum of lepton 1 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_l2Pt", ";Transvers momentum of lepton 2 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_MET1Pt", ";PF Missing transvers energy [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_in_llEta", ";#eta lepton 1;Events", 20, -4.5, 4.5) );
  mon.addHistogram( new TH1F("ZZ_in_nvtx", ";Number of Good vertex;Events", 30, 0., 30.) );
  mon.addHistogram( new TH1F("ZZ_in_njet", ";Number Jet;Events", 10, 0., 10.) );
  mon.addHistogram( new TH1F("ZZ_in_MetRed_IND", ";Met IND [GeV];Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("ZZ_in_MetRed_D0", ";Met D0 [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_in_RedIndMet_L", ";RedMet IND Long [GeV];Events", 50, -50., 100.) );
  mon.addHistogram( new TH1F("ZZ_in_Balance", ";Met/Pt(Z);Events", 50, 0., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_RedBalance", ";RedMet/Pt(Z);Events", 50, 0., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_BalRedIndMet_L", ";Balance (RedMet IND Long);Events [GeV]", 50, -1., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_LeptId", ";Lept1+Lept2 Charge;Events", 2, 0., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_l1_passIso", ";Lept1 Pass Iso;Events", 2, 0., 2.) );
  mon.addHistogram( new TH1F("ZZ_in_l2_passIso", ";Lept1 Pass Iso;Events", 2, 0., 2.) );
// After All selection
  mon.addHistogram( new TH1F("ZZ_fin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_fin_llPt", ";Transvers momentum of lepton pair [GeV];Events", 30, 25., 325.) );
  mon.addHistogram( new TH1F("ZZ_fin_l1Pt", ";Transvers momentum of lepton 1 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_fin_l2Pt", ";Transvers momentum of lepton 2 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_fin_MET1Pt", ";PF Missing transvers energy [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_fin_llEta", ";#eta lepton 1;Events", 20, -4.5, 4.5) );
  mon.addHistogram( new TH1F("ZZ_fin_nvtx", ";Number of Good vertex;Events", 30, 0., 20.) );
  mon.addHistogram( new TH1F("ZZ_fin_njet", ";Number Jet;Events", 10, 0., 10.) );
  mon.addHistogram( new TH1F("ZZ_fin_MetRed_IND", ";Met IND [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_fin_MetRed_D0", ";Met D0 [GeV];Events", 50, 0., 200.) );
// Cut Plot
  mon.addHistogram( new TH1F("ZZ_cut_llPt", ";Transvers momentum of lepton 1 [GeV];Events", 25, 20., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_Bveto", ";Num of B-tagged Jet (2);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_Bvetol", ";Num of B-tagged Jet (1.7);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet", ";Num of Jet;Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_10", ";Num of Jet (Pt>10);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_15", ";Num of Jet (Pt>15);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_20", ";Num of Jet (Pt>20);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_25", ";Num of Jet (Pt>25);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_njet_30", ";Num of Jet (Pt>30);Events", 5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_MET1Pt", ";PF Missing transvers energy [GeV];Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_cut_MetRed_IND", ";Met IND;Events [GeV]", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_cut_MetRed_D0", ";Met D0;Events [GeV]", 50, 0., 200.) );
  mon.addHistogram( new TH1F("ZZ_cut_RedIndMet_L", ";RedMet IND Long;Events [GeV]", 50, -50., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_BalRedIndMet_L", ";Balance (RedMet IND Long);Events [GeV]", 50, -1., 2.) );
  mon.addHistogram( new TH1F("ZZ_cut_RedIndMet_L_yesJ", ";RedMet IND Long;Events [GeV]", 50, -50., 100.) );
  mon.addHistogram( new TH1F("ZZ_cut_BalRedIndMet_L_yesJ", ";Balance (RedMet IND Long);Events [GeV]", 50, -1., 2.) );
  mon.addHistogram( new TH1F("ZZ_cut_Balance", ";Balance;Events", 20, 0.5, 2.5) );
  mon.addHistogram( new TH1F("ZZ_cut_Dphi_J", ";Delta Phi (J-Met);Events",  5, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept10", ";Num of Leptons, Pt > 10;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept15", ";Num of Leptons, Pt > 15;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_Nlept20", ";Num of Leptons, Pt > 17;Events",  4, 2., 6.) );
  mon.addHistogram( new TH1F("ZZ_cut_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
// Bin Pt
  mon.addHistogram( new TH1F("ZZ_Pt1bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_Pt2bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_Pt3bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
  mon.addHistogram( new TH1F("ZZ_Pt4bin_llMass", ";Dilepton Invariant mass [GeV];Events", 20, 75., 110.) );
// WZ Estimation
  mon.addHistogram( new TH1F("WZEst_NLeptVeto", ";# Third leptons vetoed;Events", 1, 0., 1.) );
  mon.addHistogram( new TH1F("WZEst_Accept3Lept", ";Third leptons acceptance;Events", 1, 0., 1.) );
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
  mon.addHistogram( new TH1F("Sys_MetEff_Purity", ";# Event After Btag+Emu;Events", 2, 0., 2.) );
  mon.addHistogram( new TH1F("Sys_MetEff_Met", ";# RedMet [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_sumEt", ";Sum Et [GeV] ;Events", 70, 100., 1300.) );
  mon.addHistogram( new TH1F("Sys_MetEff_Nvtx", ";N vtx [GeV] ;Events", 30, 0., 30.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMetPx", ";RedMet x [GeV] ;Events", 30, -90., 90.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMetPy", ";RedMet y [GeV] ;Events", 30, -90., 90.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_1nvtx", ";RedMet (nvtx<5) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_2nvtx", ";RedMet (nvtx<8) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_3nvtx", ";RedMet (nvtx<13) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_MetEff_RedMet_Maxnvtx", ";RedMet (nvtx>15) [GeV] ;Events", 30, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMetSmired", ";RedMet Smeared [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMetSmired_CtrSpl", ";RedMet Smeared [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("Sys_NewMetSmiredJER", ";RedMet Smeared into JER [GeV] ;Events", 50, 0., 500.) );
  mon.addHistogram( new TH1F("Sys_NewMetjetDiff", ";Diff for RedMet Smeared into JER [GeV] ;Events", 50, 0., 40.) );

  mon.addHistogram( new TH1F("MetEff_RedMetPt", ";RedMet Pt [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("MetEff_Met_Nj", ";# RedMet (Nj>2) [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("MetEff_Met_Phip", ";# RedMet (DPhi>1.5) [GeV] ;Events", 50, 0., 200.) );
  mon.addHistogram( new TH1F("MetEff_Met_Phim", ";# RedMet (DPhi<1.5) [GeV] ;Events", 50, 0., 200.) );
// Cross Section
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
  TH1F *ee_EfficPUp_tot = new TH1F("ee_EfficPUp_tot", "ZZ (ee) Efficiency", 1, 0., 1.);
  TH1F *mumu_EfficPUp = new TH1F("mumu_EfficPUp", "ZZ (mumu) Efficiency", 1, 0., 1.);
  TH1F *mumu_EfficPUp_tot = new TH1F("mumu_EfficPUp_tot", "ZZ (mumu) Efficiency", 1, 0., 1.);
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

  TH1F *NJetsAssoc = new TH1F("NJetsAssoc", "Ratio Associated Jet", 1, 0., 1.);
/*
// Debug
  mon.addHistogram( new TH2F ("ZZ_h2_MET_BALANCE", ";MET; BALANCE", 50,0,70,50,0,2) );
  mon.addHistogram( new TH1F("ZZ_in_Mt_1lept", ";1lept Transvers Mass [GeV];Events", 50, 30., 150.) );
  mon.addHistogram( new TH1F("ZZ_in_Mt_2lept", ";2lept Transvers Mass [GeV];Events", 50, 30., 150.) );
  mon.addHistogram( new TH1F("ZZ_in_Dphil1Met", ";Dphi (W2-1Lept); Events", 50, -3.14, 3.14) );
  mon.addHistogram( new TH1F("ZZ_in_Dphil2Met", ";Dphi (W1-2Lept); Events", 50, -3.14, 3.14) );
  mon.addHistogram( new TH1F("ZZ_in_IsoEcal1", ";Ecal Lead. Lept Iso.; Events", 50, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_in_IsoEcal2", ";Ecal SubLead. Lept Iso.; Events", 50, 0., 5.) );
  mon.addHistogram( new TH1F("ZZ_in_IsoHcal1", ";Hcal Lead. Lept Iso.; Events", 50, 0., 3.) );
  mon.addHistogram( new TH1F("ZZ_in_IsoHcal2", ";Hcal SubLead. Lept Iso.; Events", 50, 0., 3.) );
  mon.addHistogram( new TH1F("ZZ_in_IsoTr1", ";Track Lead. Lept Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_IsoTr2", ";Track SubLead. Lept Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_Iso1", ";Lead. Lept RelIso.; Events", 50, 0., 0.2) );
  mon.addHistogram( new TH1F("ZZ_in_Iso2", ";SubLead. Lept RelIso.; Events", 50, 0., 0.2) );
  mon.addHistogram( new TH1F("ZZ_in_Charge1", ";Lead. Charge Lept Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_Charge2", ";SubLead. Charge Lept Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_Neu1", ";Lead. Lept Neut Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_Neu2", ";SubLead. Lept Neut Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_Gam1", ";Lead. Lept Gamma Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_Gam2", ";SubLead. Lept Gamma Iso.; Events", 50, 0., 7.) );
  mon.addHistogram( new TH1F("ZZ_in_PFIso1", ";Lead. Lept PFIso.; Events", 50, 0., 0.2) );
  mon.addHistogram( new TH1F("ZZ_in_PFIso2", ";Sublead. Lept PFIso.; Events", 50, 0., 0.2) );
  mon.addHistogram( new TH1F("ZZ_in_DPhil1Jet", ";Delta Phi (Jet-Lept1).; Events", 50, -3.14, 3.14) );
  mon.addHistogram( new TH1F("ZZ_in_DPhil2Jet", ";Delta Phi (Jet-Lept2).; Events", 50, -3.14, 3.14) );
*/
  //Fit DY
  mon.addHistogram( new TH1F("Fit_PFMet", ";RedMet [GeV]; Events", 60, 0., 80.) );
  mon.addHistogram( new TH1F("Fit_RedMet_D0", ";RedMet [GeV]; Events", 60, 0., 80.) );
  mon.addHistogram( new TH1F("Fit_RedMet_IND", ";RedMet [GeV]; Events", 60, 0., 80.) );

  // Systematic
  int NPassLeptVeto = 0;
  double Accept = 0., Tot3Lept = 0.;
  double NzzTot_ee = 0., NzzTot_mumu = 0., NzzTotPUp_ee =0., NzzTotPUp_mumu =0., NzzTotPUm_ee =0., NzzTotPUm_mumu =0., Nzz_ee = 0., Nzz_mumu = 0.;
  double NzzPUp_ee =0., NzzPUp_mumu =0.,NzzPUm_ee =0.,NzzPUm_mumu =0.;
  double NzzJer_ee = 0., NzzJer_mumu = 0., NzzJesm_ee = 0., NzzJesm_mumu = 0., NzzJesp_ee = 0., NzzJesp_mumu = 0.;
  double Nzz_ee_ptBin1 = 0., Nzz_ee_ptBin2 = 0., Nzz_ee_ptBin3 = 0., Nzz_ee_ptBin4 = 0.;
  double Nzz_mumu_ptBin1 = 0., Nzz_mumu_ptBin2 = 0., Nzz_mumu_ptBin3 = 0., Nzz_mumu_ptBin4 = 0.;
  double Nzz_MetSmear_ee =0., Nzz_MetSmear_mumu =0.,  Nzz_MetSmearm_ee =0., Nzz_MetSmearm_mumu =0.,  Nzz_MetSmearp_ee =0., Nzz_MetSmearp_mumu =0.;
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
         default  : tag_cat = "??";    break;

      }
     if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
     if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;

      bool isGammaEvent = false;
      if(gammaEvHandler){
          isGammaEvent=gammaEvHandler->isGood(phys);
          if(mctruthmode==22 && !isGammaEvent) continue;
          tag_cat = "gamma";
      }
     
      int eventSubCat  = eventCategoryInst.Get(phys);
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

      //pileup and Higgs pT weight
      //float weight=ev.puWeight;
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight = LumiWeights.weight( ev.ngenITpu );
        TotalWeight_plus = PShiftUp.ShiftWeight( ev.ngenITpu );
        TotalWeight_minus = PShiftDown.ShiftWeight( ev.ngenITpu );
        if(isMC_VBF) weight *= weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );         
        if(isMC_GG)  {
	  for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
	  weight *= ev.hptWeights[0];
	}
      }
     
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);

      //##############################################
      //########       GLOBAL VARIABLES       ########
      //##############################################

      float iweight=weight;
//FIXME
//              if(gammaEvHandler){
//                  TString dilCh=catsToFill[ic];
//                  if(dilCh=="all") dilCh="ll";
//                  iweight *= gammaEvHandler->getWeight(dilCh);
//              }              


      //z+met kinematics
      LorentzVector zll  = isGammaEvent ? gammaEvHandler->massiveGamma("ll") : phys.leptons[0]+phys.leptons[1];
      LorentzVector zvv  = phys.met[0];
      Float_t dphill     = isGammaEvent ? 0 : deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      //Float_t detall     = isGammaEvent ? 0 : phys.leptons[0].eta()-phys.leptons[1].eta();
      Float_t drll       = isGammaEvent ? 0 : deltaR(phys.leptons[0],phys.leptons[1]);
      Float_t mindrlz    = isGammaEvent ? 0 : min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t maxdrlz    = isGammaEvent ? 0 : max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t ptl1       = isGammaEvent ? 0 : phys.leptons[0].pt();
      Float_t ptl2       = isGammaEvent ? 0 : phys.leptons[1].pt();
      Float_t ptsum      = ptl1+ptl2;
      Float_t mtl1       = isGammaEvent ? 0 : METUtils::transverseMass(phys.leptons[0],zvv,false);
      Float_t mtl2       = isGammaEvent ? 0 : METUtils::transverseMass(phys.leptons[1],zvv,false);
      Float_t mtsum      = mtl1+mtl2;
      Float_t zmass      = zll.mass();
      Float_t zpt        = zll.pt();
      Float_t zeta       = zll.eta();
      Float_t met        = zvv.pt();
      //Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
      Float_t mt         = METUtils::transverseMass(zll,zvv,true);
      //Float_t dphizleadl = isGammaEvent ? 0 : ( ptl1>ptl2 ? deltaPhi(phys.leptons[0].phi(),zll.phi()) : deltaPhi(phys.leptons[1].phi(),zll.phi()) );

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
              //if( fabs(ev.mc_id[i])==11 ) TwoEle++;
              //if( fabs(ev.mc_id[i])==12 ) TwoNEle++;
              //if( fabs(ev.mc_id[i])==13 ) TwoMu++;
              //if( fabs(ev.mc_id[i])==14 ) TwoNMu++;
          }
          if( TwoEle>=1 || TwoNEle>=1 )  ZZllvvOrnotZZ = true;
          if( TwoMu>=1  || TwoNMu>=1 )   ZZllvvOrnotZZ = true;
      }
      else ZZllvvOrnotZZ = true;
      //double ZZllvvWeight=iweight;
      //if( DatasetIn.Contains("MC_ZZ") && !ZZllvvOrnotZZ  ) ZZllvvWeight=0.;
      
      //count jets and b-tags
      LorentzVector BestJet;
      float PtApp=0.;
      int njets(0),njetsinc(0);
      int nbtags(0), nbtags_tchel(0),   nbtags_tche2(0),  nbtags_csvl(0);
      LorentzVectorCollection jetsP4;
      int nheavyjets(0), nlightsjets(0);
      int Nj_10=0, Nj_15=0, Nj_20=0, Nj_25=0, Nj_30=0;
      int tmpJ = 0; double deltaPhiJet = 0.;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++){
          if(phys.jets[ijet].pt()>10) Nj_10++;
          if(phys.jets[ijet].pt()>15) Nj_15++;
          if(phys.jets[ijet].pt()>20) Nj_20++;
          if(phys.jets[ijet].pt()>25) Nj_25++;
          if(phys.jets[ijet].pt()>30){
             Nj_30++; if(Nj_30==1) tmpJ=ijet; if(Nj_30==2) deltaPhiJet = deltaPhi(phys.jets[ijet].phi(),phys.jets[tmpJ].phi());  
          }
          jetsP4.push_back( phys.jets[ijet] );
          njetsinc++;
          if(fabs(phys.jets[ijet].eta())<2.5){
              njets++;
              bool passTCHEL(phys.jets[ijet].btag1>1.7);
              bool passTCHE2(phys.jets[ijet].btag1>2.0);
              bool passCSVL(phys.jets[ijet].btag2>0.244);
              if(phys.jets[ijet].pt()>20){
                nbtags          += passTCHE2;
                nbtags_tchel    += passTCHEL;
                nbtags_tche2    += passTCHE2;
                nbtags_csvl     += passCSVL;
                nheavyjets += (fabs(phys.jets[ijet].genid)==5);
                nlightsjets += (fabs(phys.jets[ijet].genid)!=5);
                if( phys.jets[ijet].pt() > PtApp ){PtApp =  phys.jets[ijet].pt(); BestJet = phys.jets[ijet];}
              }
          }
      }
      bcomp.compute(nheavyjets,nlightsjets);
      std::vector<btag::Weight_t> wgt = bcomp.getWeights();
      double p0btags = wgt[0].first;  
      double p0btags_err=wgt[0].second;
      
      //met variables: check BaseMetSelection @ StandardSelections_cfi.py
      LorentzVector metP4             = phys.met[0];
      LorentzVector centralMetP4      = phys.met[3];
      LorentzVector assocMetP4        = phys.met[1];
      LorentzVector assocChargedMetP4 = phys.met[5];
      LorentzVector assocFwdMetP4     = phys.met[6];
      LorentzVector assocCMetP4       = phys.met[11];
      LorentzVector assocFwdCMetP4    = phys.met[12];
      LorentzVector clusteredMetP4    = -1*zll;  for(unsigned int i=0;i<jetsP4.size();i++){clusteredMetP4 -= jetsP4[i];}
      LorentzVector unclMet = (zvv - clusteredMetP4);
      if(isGammaEvent){
          assocChargedMetP4 -= zll;
          if(!phys.gammas[0].isConv) assocMetP4 -= zll;
      }

      //redmet
      METUtils::stRedMET redMetInfo;      
      LorentzVector nullP4     = LorentzVector(0,0,0,0);
      LorentzVector lep1       = isGammaEvent ? zll    : phys.leptons[0];
      LorentzVector lep2       = isGammaEvent ? nullP4 : phys.leptons[1];
      LorentzVector rTMetP4    = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , zvv                , isGammaEvent);
      LorentzVector rAMetP4    = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , zvv                , isGammaEvent);
      LorentzVector rACorMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocCMetP4         , zvv                , isGammaEvent);                                                                             
      LorentzVector rAFwdMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocFwdMetP4         , zvv                , isGammaEvent);
      LorentzVector rAFwdCorMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocFwdCMetP4         , zvv                , isGammaEvent);
      LorentzVector rCMetP4    = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4     , zvv                , isGammaEvent);
      LorentzVector rTAMetP4   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , assocMetP4         , isGammaEvent);
      LorentzVector rTCMetP4   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , clusteredMetP4     , isGammaEvent);
      LorentzVector rACMetP4   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4     , isGammaEvent);
      LorentzVector r3MetP4    = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4, zvv, isGammaEvent);
      LorentzVector rmAMetP4   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, min(zvv,assocMetP4), clusteredMetP4, zvv, isGammaEvent);
      LorentzVector redMetP4   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetsP4             , zvv                , isGammaEvent, &redMetInfo);
      double redMet = redMetP4.pt();   double redMetL = redMetInfo.redMET_l; double redMetT = redMetInfo.redMET_t;
      LorentzVector redMetD0P4 = METUtils::redMET(METUtils::D0, lep1, 0, lep2, 0, jetsP4             , zvv                , isGammaEvent); 
      //Float_t projMet              =  isGammaEvent ? 0 : METUtils::projectedMET(phys.leptons[0], phys.leptons[1], zvv).pt();
      double RedIndMet_L = redMetP4.pt()*cos( fabs(deltaPhi( redMetP4.phi(),zll.phi() )) );

      //DPhi
      int DphiMet_Jet=0;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++){
             if( fabs(deltaPhi(phys.jets[ijet].phi(),redMetP4.phi()))<0.5 && phys.jets[ijet].pt()>20. ) DphiMet_Jet++;
      }

      //sum ETs
      float sumEt            = ev.sumEt           - ptsum;
      float sumEtcentral     = ev.sumEtcentral    - ptsum;
      float chSumEtcentral   = ev.chsumEtcentral  - ptsum;
      float neutSumEtcentral = ev.neutsumEtcentral;
      float chSumEt          = ev.chsumEt         - ptsum;
     // float neutsumEt        = ev.neutsumEjmett;
    
      //Prepare Variation JES
      //GenJet
      std::vector<double> genJetsPt;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)      genJetsPt.push_back( phys.jets[ijet].genPt );
      LorentzVectorCollection zvvs;
      std::vector<LorentzVectorCollection> Jets;
      if(/*runSystematics*/ true) METUtils::computeVariation(jetsP4, genJetsPt, redMetP4, Jets, zvvs,&jecUnc);

      zvvs.insert(zvvs.begin(),redMetP4);
      Jets.insert(Jets.begin(),jetsP4);
      //std::vector<Float_t>  mts;
      //for(size_t ivar=0; ivar<(runSystematics?3:1); ivar++){
      //    Float_t imt     = METUtils::transverseMass(zll,zvvs[ivar],true);
      //    mts.push_back(imt);
      //}
      if( tag_cat=="ee" || tag_cat=="mumu" ){
        if( zpt>55 ) mon.fillHisto("Sys_NewMet_Jer",  tags_cat, zvvs[1].pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jerp", tags_cat, zvvs[2].pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jerm", tags_cat, zvvs[3].pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jesp", tags_cat, zvvs[4].pt(), iweight);
        mon.fillHisto("Sys_NewMet_Jesm", tags_cat, zvvs[5].pt(), iweight);
      }
      //Same for PFMEt
      LorentzVectorCollection MetPF;
      std::vector<LorentzVectorCollection> Jets1;
      if(/*runSystematics*/ true) METUtils::computeVariation(jetsP4, genJetsPt, phys.met[0], Jets1, MetPF,&jecUnc);
      MetPF.insert(MetPF.begin(),phys.met[0]);
      Jets1.insert(Jets1.begin(),jetsP4);
      //Same for METD0
      LorentzVectorCollection MetD0;
      std::vector<LorentzVectorCollection> Jets2;
      if(/*runSystematics*/ true) METUtils::computeVariation(jetsP4, genJetsPt, redMetD0P4, Jets2, MetD0,&jecUnc);
      MetD0.insert(MetD0.begin(),redMetD0P4);
      Jets2.insert(Jets2.begin(),jetsP4);

      //JER with association
      //vector<double> GenJet;
      //for(int ijet=0; ijet<ev.jn; ijet++){
      //    GenJet.push_back(ev.jn_genpt[ijet]);
      //}
      LorentzVector newMetJer;
      std::vector<LorentzVector> jetsJer;
      if(/*runSystematics*/ true) newMetJer = METUtils::SmearJetFormGen(jetsP4, redMetP4, genJetsPt, jetsJer);
      if( tag_cat == "ee" || tag_cat == "mumu" ){ if( zpt>55 ) mon.fillHisto("Sys_NewMetSmiredJER", tags_cat, newMetJer.pt(), iweight);}

      //##############################################
      //########     GLOBAL SELECTION         ########
      //##############################################

      //bool Flav( (ev.l1_id == -(ev.l2_id)) );
      bool passZmass(fabs(zmass-91) < 10);
      bool passZpt(zpt>30);
      bool passBveto(nbtags_csvl==0); // 0.244
      bool passBvetol(nbtags_tchel==0); //1.7
      bool JetVeto( Nj_30 == 0 ); //"eq0jets"
      double MetCut = 45.; bool RedMetPt( redMet > MetCut ); bool RedMetJer( newMetJer.pt() > MetCut ); bool RedMetJesm( zvvs[5].pt() > MetCut ); bool RedMetJesp( zvvs[4].pt() > MetCut );
      bool passMediumRedMet(50); bool passTightRedMet(84);
      bool Balance( met/zpt > 0.4 && met/zpt < 1.8 );
      bool Dphi_Z_tight( deltaPhi(zll.phi(),zvv.phi()) > 2.64  );      bool Dphi_Z( fabs( deltaPhi(zll.phi(),zvv.phi()) ) > 1.0472 );
      bool Dphi_J( DphiMet_Jet==0 );
      bool pass3dLeptonVeto(true); int nlept10 = 2, nlept15 = 2, nlept20 = 2; LorentzVector ThirdLept;
           for(unsigned int i=2; i < phys.leptons.size(); i++){
              if( phys.leptons[i].pt() > 10 ){
                  nlept10++;
                  if( pass3dLeptonVeto ) ThirdLept = phys.leptons[i];
                  pass3dLeptonVeto = false; } 
              if( phys.leptons[i].pt() > 15 ){ nlept15++; }
              if( phys.leptons[i].pt() > 20 ){ nlept20++; }
           }
      bool Dil( (tag_cat == "ee" || tag_cat == "mumu") );
	bool PassSelectionRedMet( Dil && passZmass && passZpt && passBveto && JetVeto && RedMetPt && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionSysMetJesm( Dil && passZmass && passZpt && passBveto && JetVeto && RedMetJesm && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionSysMetJesp( Dil && passZmass && passZpt && passBveto && JetVeto && RedMetJesp && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionSysMet( Dil && passZmass && passZpt && passBveto && JetVeto && RedMetJer && Balance && Dphi_J && pass3dLeptonVeto);
	bool PassSelectionNoMet( Dil && passZmass && passZpt && passBveto && JetVeto && Balance && Dphi_J && pass3dLeptonVeto);
	bool SelectionNo3LeptVeto( passZmass && passZpt && passBveto && JetVeto && RedMetPt && Balance && Dphi_J );

      bool pt_1(zpt > 70. && zpt < 90.);
      bool pt_2(zpt > 90. && zpt < 110.);
      bool pt_3(zpt > 110. && zpt < 140);
      bool pt_4(zpt > 140.);
      int NvtxBin1 = 6, NvtxBin2 = 9, NvtxBin3 = 13;

      bool passLooseKinematics( zpt>20 );
      bool passSideBand( !passZmass && fabs(zmass-91)<30 );

      //##############################################
      //########         GENERAL PLOTS        ########
      //##############################################

//Debug
/*
      mon.fill2DHisto("ZZ_h2_MET_BALANCE", tags_cat, met, met/zpt, iweight);

      mon.fillHisto("ZZ_in_Mt_1lept", tags_cat, mtl1, iweight);
      mon.fillHisto("ZZ_in_Mt_2lept", tags_cat, mtl2, iweight);
      mon.fillHisto("ZZ_in_Dphil1Met", tags_cat, deltaPhi( (zvv+phys.leptons[0]).phi() ,phys.leptons[1].phi()), iweight);
      mon.fillHisto("ZZ_in_Dphil2Met", tags_cat, deltaPhi( (zvv+phys.leptons[1]).phi() ,phys.leptons[0].phi()), iweight);

      mon.fillHisto("ZZ_in_IsoEcal1", tags_cat, phys.leptons[0].ecalIso , iweight);
      mon.fillHisto("ZZ_in_IsoEcal2", tags_cat, phys.leptons[1].ecalIso , iweight);
      mon.fillHisto("ZZ_in_IsoHcal1", tags_cat, phys.leptons[0].hcalIso , iweight);
      mon.fillHisto("ZZ_in_IsoHcal2", tags_cat, phys.leptons[1].hcalIso , iweight);
      mon.fillHisto("ZZ_in_IsoTr1", tags_cat, phys.leptons[0].trkIso , iweight);
      mon.fillHisto("ZZ_in_IsoTr2", tags_cat, phys.leptons[1].trkIso , iweight);
      double Iso1 = (phys.leptons[0].trkIso+phys.leptons[0].ecalIso+phys.leptons[0].hcalIso - pow(0.3,2)*3.14*ev.rho )/(phys.leptons[0].pt());
      double Iso2 = (phys.leptons[1].trkIso+phys.leptons[1].ecalIso+phys.leptons[1].hcalIso - pow(0.3,2)*3.14*ev.rho )/(phys.leptons[1].pt());
      mon.fillHisto("ZZ_in_Iso1", tags_cat, Iso1 , iweight);
      mon.fillHisto("ZZ_in_Iso2", tags_cat, Iso2 , iweight);

      mon.fillHisto("ZZ_in_Charge1", tags_cat, phys.leptons[0].chIso, iweight);
      mon.fillHisto("ZZ_in_Charge2", tags_cat, phys.leptons[1].chIso, iweight);
      mon.fillHisto("ZZ_in_Neu1", tags_cat, phys.leptons[0].nhIso, iweight);
      mon.fillHisto("ZZ_in_Neu2", tags_cat, phys.leptons[1].nhIso, iweight);
      mon.fillHisto("ZZ_in_Gam1", tags_cat, phys.leptons[0].gIso, iweight);
      mon.fillHisto("ZZ_in_Gam2", tags_cat, phys.leptons[1].gIso, iweight);
      double PFIso1 = ( phys.leptons[0].nhIso + phys.leptons[0].chIso + phys.leptons[0].gIso )/(phys.leptons[0].pt());
      double PFIso2 = ( phys.leptons[1].nhIso + phys.leptons[1].chIso + phys.leptons[1].gIso )/(phys.leptons[1].pt());
      mon.fillHisto("ZZ_in_PFIso1", tags_cat, PFIso1, iweight);
      mon.fillHisto("ZZ_in_PFIso2", tags_cat, PFIso2, iweight);

      mon.fillHisto("ZZ_in_DPhil1Jet", tags_cat, deltaPhi( BestJet.phi(), phys.leptons[0].phi()) , iweight);
      mon.fillHisto("ZZ_in_DPhil2Jet", tags_cat, deltaPhi( BestJet.phi(), phys.leptons[1].phi()) , iweight);
*/
 //Preselection
 if( tag_cat!="??" ){
      if( Dil ){
      mon.fillHisto("ZZ_in_llMass", tags_cat, zmass, iweight);
      mon.fillHisto("ZZ_in_llPt", tags_cat, zpt, iweight);
      mon.fillHisto("ZZ_in_l1Pt", tags_cat,ptl1 , iweight);
      mon.fillHisto("ZZ_in_l2Pt", tags_cat, ptl2, iweight);
      mon.fillHisto("ZZ_in_MET1Pt", tags_cat, met, iweight);
      mon.fillHisto("ZZ_in_llEta", tags_cat, zeta, iweight);
      mon.fillHisto("ZZ_in_nvtx", tags_cat, ev.nvtx, iweight);
      mon.fillHisto("ZZ_in_njet", tags_cat, phys.jets.size() , iweight);
      if( zpt>55 ) mon.fillHisto("ZZ_in_MetRed_IND", tags_cat, redMet, iweight);
      mon.fillHisto("ZZ_in_MetRed_D0", tags_cat, redMetD0P4.Pt(), iweight);
      mon.fillHisto("ZZ_in_Balance", tags_cat, met/zpt, iweight);
      mon.fillHisto("ZZ_in_RedBalance", tags_cat, redMet/zpt, iweight);
      mon.fillHisto("ZZ_in_RedIndMet_L", tags_cat, RedIndMet_L, iweight);
      mon.fillHisto("ZZ_in_BalRedIndMet_L", tags_cat, RedIndMet_L/zpt, iweight);
      if( ev.l1_id == -(ev.l2_id) ) mon.fillHisto("ZZ_in_LeptId", tags_cat, 0, iweight);
      else mon.fillHisto("ZZ_in_LeptId", tags_cat, 1, iweight);
      if( ev.l1_passIso ) mon.fillHisto("ZZ_in_l1_passIso", tags_cat, 0, iweight);
      else mon.fillHisto("ZZ_in_l1_passIso", tags_cat, 1, iweight);
      if( ev.l2_passIso ) mon.fillHisto("ZZ_in_l2_passIso", tags_cat, 0, iweight);
      else mon.fillHisto("ZZ_in_l2_passIso", tags_cat, 1, iweight);
 //FinalSelection
      if( PassSelectionRedMet ){
      mon.fillHisto("ZZ_fin_llMass", tags_cat, zmass, iweight);
      mon.fillHisto("ZZ_fin_llPt", tags_cat, zpt, iweight);
      mon.fillHisto("ZZ_fin_l1Pt", tags_cat,ptl1 , iweight);
      mon.fillHisto("ZZ_fin_l2Pt", tags_cat, ptl2, iweight);
      mon.fillHisto("ZZ_fin_MET1Pt", tags_cat, met, iweight);
      mon.fillHisto("ZZ_fin_llEta", tags_cat, zeta, iweight);
      mon.fillHisto("ZZ_fin_nvtx", tags_cat, ev.nvtx, iweight);
      mon.fillHisto("ZZ_fin_njet", tags_cat, phys.jets.size() , iweight);
      mon.fillHisto("ZZ_fin_MetRed_IND", tags_cat, redMet, iweight);
      mon.fillHisto("ZZ_fin_MetRed_D0", tags_cat, redMetD0P4.Pt(), iweight);
      }
  //Cut Optimization
      if( passZmass  ){
      mon.fillHisto("ZZ_cut_llPt", tags_cat, zpt, iweight);
       if( passZpt ){
      mon.fillHisto("ZZ_cut_Bveto", tags_cat, nbtags, iweight);
      mon.fillHisto("ZZ_cut_Bvetol", tags_cat, nbtags_tchel, iweight);
        if( passBveto ){
      mon.fillHisto("ZZ_cut_njet", tags_cat, phys.jets.size() , iweight);
      mon.fillHisto("ZZ_cut_njet_10", tags_cat, Nj_10 , iweight);
      mon.fillHisto("ZZ_cut_njet_15", tags_cat, Nj_15 , iweight);
      mon.fillHisto("ZZ_cut_njet_20", tags_cat, Nj_20 , iweight);
      mon.fillHisto("ZZ_cut_njet_25", tags_cat, Nj_25 , iweight);
      mon.fillHisto("ZZ_cut_njet_30", tags_cat, Nj_30 , iweight);
      mon.fillHisto("ZZ_cut_RedIndMet_L_yesJ", tags_cat, RedIndMet_L, iweight);
      mon.fillHisto("ZZ_cut_BalRedIndMet_L_yesJ", tags_cat, RedIndMet_L/zpt, iweight);
         if( JetVeto ){
      mon.fillHisto("ZZ_cut_MET1Pt", tags_cat, met, iweight);
      mon.fillHisto("ZZ_cut_MetRed_IND", tags_cat, redMet, iweight);
      mon.fillHisto("ZZ_cut_MetRed_D0", tags_cat, redMetD0P4.Pt(), iweight);
      mon.fillHisto("ZZ_cut_RedIndMet_L", tags_cat, RedIndMet_L, iweight);
      mon.fillHisto("ZZ_cut_BalRedIndMet_L", tags_cat, RedIndMet_L/zpt, iweight);
          if( RedMetPt ){
      mon.fillHisto("ZZ_cut_Balance", tags_cat, met/zpt, iweight);
           if( Balance ){
      mon.fillHisto("ZZ_cut_Dphi_J", tags_cat,  DphiMet_Jet, iweight);
            if( Dphi_J ){
      mon.fillHisto("ZZ_cut_Nlept10", tags_cat, nlept10, iweight);
      mon.fillHisto("ZZ_cut_Nlept15", tags_cat, nlept15, iweight);
      mon.fillHisto("ZZ_cut_Nlept20", tags_cat, nlept20, iweight);
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

  //Met Smearing for Eff Correction
    TRandom *Gaussian = new TRandom();
    double MetSmeared = 0., MetSmearedm = 0., MetSmearedp = 0., MetSmearedX=0., MetSmearedY=0., MetSmearedXm=0., MetSmearedYm=0., MetSmearedXp=0., MetSmearedYp=0.;
    if(isMC){
      if(ev.nvtx < NvtxBin1){
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 13./sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 13./sqrt(2) );
         MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 6.5/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 6.5/sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));
 
         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 20./sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 20./sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
      else if(ev.nvtx < NvtxBin2){
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 18.5/sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 18.5/sqrt(2) );
         MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 11.5/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 11.5/sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));

         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 24.5/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 24.5/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
      else if(ev.nvtx < NvtxBin3){
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 16./sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 16./sqrt(2) );
         MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 4.5/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 4.5/sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));

         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 23.5/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 23.5/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
      else{
         MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 8.5/sqrt(2) );
         MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 8.5/sqrt(2) );
         MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));

         MetSmearedXm = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 3.5/sqrt(2) );
         MetSmearedYm = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 3.5/sqrt(2) );
         MetSmearedm = sqrt(pow(MetSmearedXm,2)+pow(MetSmearedYm,2));

         MetSmearedXp = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 21.5/sqrt(2) );
         MetSmearedYp = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 21.5/sqrt(2) );
         MetSmearedp = sqrt(pow(MetSmearedXp,2)+pow(MetSmearedYp,2));
      }
    }
    else MetSmeared = redMet;
      if( tag_cat != "emu" ) mon.fillHisto("Sys_NewMetSmired",tags_cat,MetSmeared,iweight);


   // Best MET (Only Base cut)
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

             if( zvvs[5].pt()  > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetInd_Best_Jesm",tags_cat,i,iweight);
             if( MetD0[5].pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetD0_Best_Jesm",tags_cat,i,iweight);
             if( MetPF[5].pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_PFMet_Best_Jesm",tags_cat,i,iweight); 
             if( zvvs[4].pt()  > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetInd_Best_Jesp",tags_cat,i,iweight);
             if( MetD0[4].pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_RedMetD0_Best_Jesp",tags_cat,i,iweight);
             if( MetPF[4].pt() > StartCut+BinCut*i )  mon.fillHisto("ZZ_PFMet_Best_Jesp",tags_cat,i,iweight); 
         } 
        }
       }
    }
  // Cut Optimization
      StartCut = 20., EndCut = 80., BinCut = 2.;
      if( passZpt  && passBveto && JetVeto && (fabs(zmass-91)<20) ){
        for(int i=0; i < 30; i++){
          if( redMet > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetInd_Opt",tags_cat,i,iweight);
          if( redMetD0P4.Pt() > StartCut+BinCut*i  )  mon.fillHisto("ZZ_RedMetD0_Opt",tags_cat,i,iweight);
          if( redMet>46. && fabs(zmass-91) < 5+0.5*i )  mon.fillHisto("ZZ_Mass_Opt",tags_cat,i,iweight);   //
          if( redMet>46. && (fabs(zmass-91)<10) && fabs( deltaPhi(zll.phi(),zvv.phi()) ) > 1+i*0.071333 )  mon.fillHisto("ZZ_Dphi_Opt",tags_cat,i,iweight);  //1.0472
        }
      }

  // DY Fit
      if( passZmass && passZpt && passBveto && Balance && JetVeto && Dphi_J && pass3dLeptonVeto ){
       mon.fillHisto("Fit_RedMet_IND", tags_cat, redMet, iweight);
       mon.fillHisto("Fit_RedMet_D0", tags_cat, redMetD0P4.Pt(), iweight);
       mon.fillHisto("Fit_PFMet", tags_cat, met, iweight);
      }

     // Event Flow
      if( true ){
         mon.fillHisto("ZZ_eventflow",tags_cat,0,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,0,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,0,iweight*TotalWeight_plus);
      if( passZmass ){ 
         mon.fillHisto("ZZ_eventflow",tags_cat,1,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,1,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,1,iweight*TotalWeight_plus);
      if( passZpt ){    
         mon.fillHisto("ZZ_eventflow",tags_cat,2,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,2,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,2,iweight*TotalWeight_plus);
      if( passBveto ){
         mon.fillHisto("ZZ_eventflow",tags_cat,3,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,3,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,3,iweight*TotalWeight_plus);
      if( JetVeto ){
         mon.fillHisto("ZZ_eventflow",tags_cat,4,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,4,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,4,iweight*TotalWeight_plus);
      if( RedMetPt ){
         mon.fillHisto("ZZ_eventflow",tags_cat,5,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,5,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,5,iweight*TotalWeight_plus);
      if( Balance ){
         mon.fillHisto("ZZ_eventflow",tags_cat,6,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,6,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,6,iweight*TotalWeight_plus);
      if( Dphi_J ){
         mon.fillHisto("ZZ_eventflow",tags_cat,7,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,7,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,7,iweight*TotalWeight_plus);
      if( pass3dLeptonVeto ){
         mon.fillHisto("ZZ_eventflow",tags_cat,8,iweight);
         mon.fillHisto("eventflow_minus",tags_cat,8,iweight*TotalWeight_minus);
         mon.fillHisto("eventflow_plus",tags_cat,8,iweight*TotalWeight_plus);
         //if( Flav )mon.fillHisto("ZZ_eventflow",tags_cat,9,iweight);
      }}}}}}}}}

// Cross Section
      if(ZZllvvOrnotZZ){
         if( tag_cat == "ee" ){ NzzTot_ee+=iweight; NzzTotPUp_ee+=iweight*TotalWeight_plus; NzzTotPUm_ee+=iweight*TotalWeight_minus; }
         if( tag_cat == "mumu" ){ NzzTot_mumu+=iweight; NzzTotPUp_mumu+=iweight*TotalWeight_plus; NzzTotPUm_mumu+=iweight*TotalWeight_minus; }
         if( PassSelectionRedMet && tag_cat == "ee"  ){ Nzz_ee+=iweight; NzzPUp_ee+=iweight*TotalWeight_plus; NzzPUm_ee+=iweight*TotalWeight_minus; }
         if( PassSelectionRedMet && tag_cat == "mumu"  ){ Nzz_mumu+=iweight; NzzPUp_mumu+=iweight*TotalWeight_plus; NzzPUm_mumu+=iweight*TotalWeight_minus; }
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
         if( PassSelectionNoMet && MetSmeared > MetCut && tag_cat == "ee"  )  Nzz_MetSmear_ee+=iweight;
         if( PassSelectionNoMet && MetSmeared > MetCut && tag_cat == "mumu"  )  Nzz_MetSmear_mumu+=iweight;
         if( PassSelectionNoMet && MetSmearedm > MetCut && tag_cat == "ee"  )  Nzz_MetSmearm_ee+=iweight;
         if( PassSelectionNoMet && MetSmearedm > MetCut && tag_cat == "mumu"  )  Nzz_MetSmearm_mumu+=iweight;
         if( PassSelectionNoMet && MetSmearedp > MetCut && tag_cat == "ee"  )  Nzz_MetSmearp_ee+=iweight;
         if( PassSelectionNoMet && MetSmearedp > MetCut && tag_cat == "mumu"  )  Nzz_MetSmearp_mumu+=iweight;
       }
     }//ee or mumu

// WZ Estimation
      if( SelectionNo3LeptVeto ) if( !pass3dLeptonVeto && !isMC ) NPassLeptVeto++;
      double dr = 0.5;
      LorentzVector ThirdMCLept(0.,0.,0.,0.);
      for(int i=0; i<ev.nmcparticles; i++){
        LorentzVector p4;
        p4.SetPxPyPzE(ev.mc_px[i],ev.mc_py[i],ev.mc_pz[i],ev.mc_en[i]);
        if( deltaR(p4,ThirdLept)<dr ){
        dr = deltaR(p4,ThirdLept);
        ThirdMCLept = p4;
        }
      }
      if( fabs(ThirdMCLept.eta())<6.  && ThirdMCLept.pt()>0.1 ) Tot3Lept++;
      if( fabs(ThirdMCLept.eta())<2.5 && ThirdMCLept.pt()>10. ) Accept++;

      if( iev==(evEnd-1) ){ 
 //WZ Estimation plot
       double weightWZ = ( Tot3Lept!=0 ? (Accept/Tot3Lept)*iweight : 0. );
       mon.fillHisto("WZEst_NLeptVeto",tags_cat, 0,NPassLeptVeto*iweight);
       mon.fillHisto("WZEst_Accept3Lept",tags_cat, 0,weightWZ);
      }

 //Met Systematic & Met eff correction
          mon.fillHisto("Sys_MetEff_Mass",tags_cat, zmass, iweight);
      if( nbtags_csvl>1 && tag_cat == "emu" ){
        mon.fillHisto("Sys_MetEff_Purity",tags_cat, 0, iweight);
        mon.fillHisto("Sys_MetEff_Nlept",tags_cat,  phys.leptons.size(), iweight);
        if( phys.leptons.size()==2 ){
          mon.fillHisto("Sys_MetEff_unclMet",tags_cat, unclMet.pt(), iweight);
          mon.fillHisto("Sys_MetEff_deltaPhiJet",tags_cat, fabs(deltaPhiJet), iweight);
          mon.fillHisto("Sys_MetEff_Purity",tags_cat, 1, iweight);
          mon.fillHisto("Sys_MetEff_Met",tags_cat, redMet, iweight);
          mon.fillHisto("Sys_MetEff_sumEt",tags_cat, sumEt, iweight);
          mon.fillHisto("Sys_MetEff_Nvtx",tags_cat, ev.nvtx, iweight);

          if( ev.nvtx<NvtxBin1  ){
            mon.fillHisto("Sys_MetEff_RedMet_1nvtx",tags_cat,  redMetP4.pt(), iweight);
          }
          else if( ev.nvtx<NvtxBin2  ){
            mon.fillHisto("Sys_MetEff_RedMet_2nvtx",tags_cat,  redMetP4.pt(), iweight);
          }
          else if( ev.nvtx<NvtxBin3  ){
            mon.fillHisto("Sys_MetEff_RedMet_3nvtx",tags_cat,  redMetP4.pt(), iweight);
          }
          else{
            mon.fillHisto("Sys_MetEff_RedMet_Maxnvtx",tags_cat,  redMetP4.pt(), iweight);
          }

          mon.fillHisto("Sys_MetEff_RedMetPt",tags_cat,  sqrt( pow(redMetP4.px(),2)+pow(redMetP4.py(),2) ), iweight);
           if( fabs(deltaPhiJet) > 1.5 ) mon.fillHisto("Sys_MetEff_Met_Phip",tags_cat, redMet, iweight);
           if( fabs(deltaPhiJet) < 1.5 ) mon.fillHisto("Sys_MetEff_Met_Phim",tags_cat, redMet, iweight);
           if( Nj_30 > 2 ) mon.fillHisto("Sys_MetEff_Met_Nj",tags_cat, redMet, iweight);
          //Best Smearing
          float MetSmearedX=0., MetSmearedY=0., MetSmeared=0.;
          if(isMC){
             TRandom *Gaussian = new TRandom();
             if(ev.nvtx < NvtxBin1){
                MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 13./sqrt(2) );
                MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 13./sqrt(2) );
                MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));
             }
             if(ev.nvtx < NvtxBin2){
                MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 18.5/sqrt(2) );
                MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 18.5/sqrt(2) );
                MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));
             }
             if(ev.nvtx < NvtxBin3){
                MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 16./sqrt(2) );
                MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 16./sqrt(2) );
                MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));
             }
             else{
                MetSmearedX = Gaussian->Gaus( redMet*cos(redMetP4.phi()), 8.5/sqrt(2) );
                MetSmearedY = Gaussian->Gaus( redMet*sin(redMetP4.phi()), 8.5/sqrt(2) );
                MetSmeared = sqrt(pow(MetSmearedX,2)+pow(MetSmearedY,2));
             }
          }
          else MetSmeared = redMet;
          mon.fillHisto("Sys_NewMetSmired_CtrSpl",tags_cat,MetSmeared,iweight);
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
      mon.fill2DHisto("zptvszeta",tags_full,zpt,zeta,iweight);
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
                     // mon.fillHisto("cutOptMediumdphill",tags_full,fabs(dphill));
                     // mon.fill2DHisto("cutOptMediumsummtvsredMetL",tags_full,mtsum,redMetL,iweight);                    
                      if(passTightRedMet)    mon.fillHisto("eventflow",tags_full,7,iweight);
                  }
               }
            }
         }
      }

      //##############################################
      //########           VBF PLOTS          ########
      //##############################################

      bool isVBF        = false;
      bool VBFPass2Jet30   = false;
      bool VBFPassdEtaCut  = false;
      bool VBFPassiMCut    = false;
      bool VBFPassBJetVeto = false;
      bool VBFPassJetVeto  = false;
      bool VBFPassLeptonIn = false;

      double  VBFMinEta = 0;                                                                                                                                                                                                                 
      double  VBFMaxEta = 0;       
      double  VBFdEta = -1;
      int     VBFCentral30Jets = 0;
      int     VBFCentralLeptons = 0;
      int     VBFNBJets=0;
      LorentzVector VBFSyst;

      if(passZmass){
         if(phys.jets.size()>=2){
             VBFSyst =  phys.jets[0] + phys.jets[1];
             VBFdEta = fabs(   phys.jets[0].eta() -    phys.jets[1].eta()); if(phys.jets[0].eta()* phys.jets[1].eta()>0)VBFdEta*=-1;
             int VBFCentral30Jets = 0;
             double MinEta, MaxEta;

             if(phys.jets[0].eta()<phys.jets[1].eta()){MinEta=phys.jets[0].eta(); MaxEta=phys.jets[1].eta();}else{MinEta=phys.jets[1].eta(); MaxEta=phys.jets[0].eta();}
             if(isGammaEvent){  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
             }else{             if(zll.eta()>MinEta && zll.eta()<MaxEta) VBFCentralLeptons=2;
             }

             for(size_t ijet=2; ijet<phys.jets.size(); ijet++){
                 if(phys.jets[ijet].pt()<30)continue; 
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral30Jets++; 
                 if(phys.jets[ijet].btag1>1.7) VBFNBJets++;
             }
          
             VBFPass2Jet30   = (phys.jets [0].pt()>30.0 && phys.jets [1].pt()>30.0);
             VBFPassdEtaCut  = VBFPass2Jet30 && (fabs(VBFdEta)>4.0);
             VBFPassiMCut    = VBFPass2Jet30 && (VBFSyst.M()>500);
             VBFPassLeptonIn = (VBFCentralLeptons==2);
             VBFPassJetVeto  = (VBFCentral30Jets==0);
             VBFPassBJetVeto = (VBFNBJets==0);
             isVBF        = (VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto && VBFPassJetVeto && VBFPassLeptonIn);
             VBFMinEta = std::min(fabs(phys.jets[0].eta()), fabs(phys.jets[1].eta()));
             VBFMaxEta = std::max(fabs(phys.jets[0].eta()), fabs(phys.jets[1].eta()));
         }

         //VBF control
         if(true                                                                                        )mon.fillHisto("VBFNEventsInc"       ,tags_full,    0                ,iweight);
         if(VBFPass2Jet30                                                                                  )mon.fillHisto("VBFNEventsInc"       ,tags_full,    1                ,iweight);
         if(VBFPassdEtaCut                                                                                 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    2                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fillHisto("VBFNEventsInc"       ,tags_full,    3                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fillHisto("VBFNEventsInc"       ,tags_full,    4                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    5                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    6                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    7                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto )mon.fillHisto("VBFNEventsInc"       ,tags_full,    8                ,iweight);

         if(VBFPass2Jet30                                                                                  )mon.fillHisto("VBFdEtaInc"          ,tags_full,    fabs(VBFdEta)    ,iweight);
         if(VBFPassdEtaCut                                                                                 )mon.fillHisto("VBFiMassInc"         ,tags_full,    VBFSyst.M()      ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fillHisto("VBFcenLeptonVetoInc" ,tags_full,    VBFCentralLeptons,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fillHisto("VBFcen30JetVetoInc"  ,tags_full,    VBFCentral30Jets ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fillHisto("VBFNBJets30Inc"      ,tags_full,    VBFNBJets        ,iweight);
       
         if(               VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFdEtaNM1C"         ,tags_full,    fabs(VBFdEta)    ,iweight);
         if(VBFPassdEtaCut              && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFiMassNM1C"        ,tags_full,    VBFSyst.M()      ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                 && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFcenLeptonVetoNM1C",tags_full,    VBFCentralLeptons,iweight);
         if(VBFPass2Jet30                                              && VBFPassBJetVeto && !pass3dLeptonVeto){
             mon.fillHisto("VBFcen30JetVeto3rdlepton"  ,tags_full,    VBFCentral30Jets ,iweight);
             mon.fillHisto("VBFNBJets303rdlepton"      ,tags_full,    VBFNBJets        ,iweight);
             mon.fillHisto("VBFdEta3rdlepton"          ,tags_full,    fabs(VBFdEta)    ,iweight);
             mon.fillHisto("VBFiMass3rdlepton"         ,tags_full,    VBFSyst.M()      ,iweight);
         }
      
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                && VBFPassBJetVeto )mon.fillHisto("VBFcen30JetVetoNM1C" ,tags_full,    VBFCentral30Jets ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                 )mon.fillHisto("VBFNBJets30NM1C"     ,tags_full,    VBFNBJets        ,iweight);
         if(                            VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto )mon.fill2DHisto("VBFdEtaiMassNM1C"  ,tags_full,    fabs(VBFdEta), VBFSyst.M(), iweight);
         if(redMet>50 &&                VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto )mon.fill2DHisto("VBFdEtaiMassNM1C2" ,tags_full,    fabs(VBFdEta), VBFSyst.M(), iweight);

         if(isVBF){
            mon.fillHisto("VBFMinEta"  ,tags_full,    VBFMinEta ,iweight);
            mon.fillHisto("VBFMaxEta"  ,tags_full,    VBFMaxEta ,iweight);
            mon.fillHisto("VBFtagvspu"      ,tags_full,    ev.ngenITpu   ,iweight); 
         }
         mon.fillHisto("VBFtotalvspu"      ,tags_full,    ev.ngenITpu   ,iweight);
      }

      //##############################################   EVENT LOOP ENDS   ##############################################
  }printf("\n");
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
  ee_Effic_tot->Fill(0.,NzzTot_ee);
  ee_Effic->Fill(0.,Nzz_ee);
  mumu_Effic_tot->Fill(0.,NzzTot_mumu);
  mumu_Effic->Fill(0.,Nzz_mumu);
  //PU syst
  ee_EfficPUm_tot->Fill(0.,NzzTotPUm_ee);
  ee_EfficPUm->Fill(0.,NzzPUm_ee);
  mumu_EfficPUm_tot->Fill(0.,NzzTotPUm_mumu);
  mumu_EfficPUm->Fill(0.,NzzPUm_mumu);
  ee_EfficPUp_tot->Fill(0.,NzzTotPUp_ee);
  ee_EfficPUp->Fill(0.,NzzPUp_ee);
  mumu_EfficPUp_tot->Fill(0.,NzzTotPUp_mumu);
  mumu_EfficPUp->Fill(0.,NzzPUp_mumu);
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
  //PU 
  ee_EfficPUm_tot->Write();
  ee_EfficPUm->Write();
  mumu_EfficPUm_tot->Write();
  mumu_EfficPUm->Write();
  ee_EfficPUp_tot->Write();
  ee_EfficPUp->Write();
  mumu_EfficPUp_tot->Write();
  mumu_EfficPUp->Write();
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

  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
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
