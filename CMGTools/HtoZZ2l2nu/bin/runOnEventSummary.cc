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
 
using namespace std;



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
    TString hqtWeightsFileURL = runProcess.getParameter<std::string>("hqtWeightsFile"); gSystem->ExpandPathName(hqtWeightsFileURL);
    TFile *fin=TFile::Open(hqtWeightsFileURL);
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
  mon.addHistogram( new TH1F( "mindrlz", ";min #DeltaR(l,Z);Events",100,0,6) );
  mon.addHistogram( new TH1F( "maxdrlz", ";max #DeltaR(l,Z);Events",100,0,6) );
  mon.addHistogram( new TH1F( "ptsum", ";#Sigma p_{T}(l^{(i)});Events", 100,0,1000));
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
  mon.addHistogram( new TH1F("njets3leptons",";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH2F("njetsvspu"    ,";Pileup events;Jets;Events", 35,0,35,5,0,5) );  
  mon.addHistogram( new TH2F("njetsincvspu" ,";Pileup events;Jets;Events", 35,0,35,5,0,5) );   
  for(size_t ibin=1; ibin<=5; ibin++){
      TString label("");
      if(ibin==5) label +="#geq";
      else        label +="=";
      label += (ibin-1);
      mon.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
      mon.getHisto("njets3leptons")->GetXaxis()->SetBinLabel(ibin,label);
      mon.getHisto("njetsvspu")->GetXaxis()->SetBinLabel(ibin,label);
      mon.getHisto("njetsincvspu")->GetXaxis()->SetBinLabel(ibin,label);
  }

  h = new TH1F ("nbtags", ";b-tag multiplicity; Events", 3,0,3);  
  h->GetXaxis()->SetBinLabel(1,"0 b-tags");
  h->GetXaxis()->SetBinLabel(2,"1 b-tags");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
  mon.addHistogram( h );
  mon.addHistogram( new TH1F ("nbtags3leptons", ";b-tag multiplicity; Events", 3,0,3) );

  //used to optimize the b-veto
  h=new TH1F ("npassbveto", ";Pass b-tag veto; Events", 3,0,3);
  h->GetXaxis()->SetBinLabel(1,"TCHEL");
  h->GetXaxis()->SetBinLabel(2,"TCHE>2");
  h->GetXaxis()->SetBinLabel(3,"CSVL");
  mon.addHistogram( h );

  //used for GenLevel
  mon.addHistogram( new TH1F( "genHiggsPt"  , ";gen Higgs p_{T};Events",100,0,200) );
  mon.addHistogram( new TH1F( "genHiggsMass", ";gen Higgs Mass ;Events",100,0,600) );
  mon.addHistogram( new TH1F( "genzllPt"  , ";gen Zll p_{T};Events",100,0,200) );
  mon.addHistogram( new TH1F( "genzvvPt"  , ";gen Zvv p_{T};Events",100,0,200) );
  mon.addHistogram( new TH1F( "genzwinHiggsPt"  , ";gen Higgs p_{T};Events",100,0,200) );
  mon.addHistogram( new TH1F( "genzwinHiggsMass", ";gen Higgs Mass ;Events",100,0,600) );
  mon.addHistogram( new TH1F( "genzwinzllPt"  , ";gen Zll p_{T};Events",100,0,200) );
  mon.addHistogram( new TH1F( "genzwinzvvPt"  , ";gen Zvv p_{T};Events",100,0,200) );

  mon.addHistogram( new TH1F( "boost_cm_zll_p"  , ";CM Zll p};Events",100,0,200) );
  mon.addHistogram( new TH1F( "boost_cm_zvv_p"  , ";CM Zvv p};Events",100,0,200) );
  mon.addHistogram( new TH1F( "boost_cm_delta_p"  , ";CM Zll p - Zvv p;Events",200,-200,200) );
  mon.addHistogram( new TH1F( "boost_cm_mass"  , ";CM Zll p - Zvv p;Events",100,0,600) );

  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",100,0,3.4) );
  mon.addHistogram( new TH1F( "minmtjmet", ";min M_{T}(jet,E_{T}^{miss}) [GeV/c^{2}];Events",100,0,250) );
  mon.addHistogram( new TH1F( "mindrjz", ";min #DeltaR(jet,Z);Events",100,0,6) );
  mon.addHistogram( new TH1F( "minmjz", ";min M(jet,Z) [GeV/c^{2}];Events",100,0,500) );

  //met control
  std::map<TString,TString> metTypes;
  metTypes["met"]                 = "E_{T}^{miss}";
  metTypes["centralMet"]          = "central-E_{T}^{miss}";
  metTypes["assocChargedMet"]     = "assoc-E_{T}^{miss}(charged)";
  metTypes["assocMet"]            = "assoc-E_{T}^{miss}";
  metTypes["assocCMet"]           = "assocC-E_{T}^{miss}";
  metTypes["assocFwdMet"]         = "assoc + Fwd E_{T}^{miss}";
  metTypes["assocFwdCMet"]        = "assocC + Fwd E_{T}^{miss}";
  metTypes["clusteredMet"]        = "clustered-E_{T}^{miss}";
  metTypes["minAssocChargedMet"]  = "min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
  metTypes["minAssocMet"]         = "min(E_{T}^{miss},assoc-E_{T}^{miss})";
  metTypes["minAssocCMet"]        = "min(E_{T}^{miss},assocC-E_{T}^{miss})";
  metTypes["minAssocFwdMet"]      = "min(E_{T}^{miss},assocFwd-E_{T}^{miss})";
  metTypes["minAssocFwdCMet"]     = "min(E_{T}^{miss},assocFwdC-E_{T}^{miss})";
  metTypes["minClusteredMet"]     = "min(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["minTAssocMet"]        = "min(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})";
  metTypes["minTClusteredMet"]    = "min(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})";
  metTypes["minAClusteredMet"]    = "min(assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["min3Met"]             = "min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["min4Met"]             = "min(E_{T}^{miss},assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redMet"]              = "red(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redMetD0"]            = "red-E_{T}^{miss}(D0)";
  metTypes["redAssocChargedMet"]  = "red(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
  metTypes["redAssocMet"]         = "red(E_{T}^{miss},assoc-E_{T}^{miss})";
  metTypes["redAssocCMet"]        = "red(E_{T}^{miss},assocC-E_{T}^{miss})";
  metTypes["redAssocFwdMet"]      = "red(E_{T}^{miss},assocFwd-E_{T}^{miss})";
  metTypes["redAssocFwdCMet"]     = "red(E_{T}^{miss},assocFwdC-E_{T}^{miss})";
  metTypes["redClusteredMet"]     = "red(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redTAssocMet"]        = "red(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})";
  metTypes["redTClusteredMet"]    = "red(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})";
  metTypes["redAClusteredMet"]    = "red(assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["red3Met"]             = "red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redminAssocMet"]      = "red(min(E_{T}^{miss},assoc-E_{T}^{miss}),clustered E_{T}^{miss})";
  metTypes["mincentralAssocMet"]  = "min(cental-E_{T}^{miss},assoc-E_{T}^{miss})";


  std::map<TString,LorentzVector> metTypeValues;
  for(std::map<TString,TString>::iterator it = metTypes.begin(); it!= metTypes.end(); it++){
      metTypeValues[it->first]=LorentzVector(0,0,0,0);
      mon.addHistogram( new TH1F( TString("met_") + it->first                 , ";"+it->second+";Events", 100,0,500) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_inc"        , ";"+it->second+";Events", 100,0,500) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_L"          , ";"+it->second+";Events", 100,-250,250) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_T"          , ";"+it->second+";Events", 100,-250,250) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_overqt"     , ";"+it->second+";Events", 100,0,5) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_overqtL"    , ";"+it->second+";Events", 100,-5,5) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_overqtT"    , ";"+it->second+";Events", 100,-5,5) );
      mon.addHistogram( new TH2F( TString("met_") + it->first + "_vspu"       , ";Pileup events;"+it->second+";Events", 35,0,35,200,0,500) );
      mon.addHistogram( new TH1F( TString("met_") + it->first + "_mindphijmet", ";min #Delta#phi(jet,"+it->second+");Events",10,0,3.15) );
  }


  mon.addHistogram( new TH2F ("redMetcomps", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 50, -251.,249,50, -251.,249.) );
  mon.addHistogram( new TH1F( "redMetL", ";red-E_{T}^{miss,#parallel};Events", 100,-250,250) );
  mon.addHistogram( new TH1F( "redMetT", ";red-E_{T}^{miss,#perp};Events", 100,-250,250) );
  mon.addHistogram( new TH2F ("metvstkmet", ";E_{T}^{miss};assoc-E_{T}^{miss}(charged)", 50,0,500,50,0,500) );  
  mon.addHistogram( new TH2F ("metvsassoc", ";E_{T}^{miss};assoc-E_{T}^{miss}", 50,0,500,50,0,500) );  
  mon.addHistogram( new TH2F ("metvsclustered", ";E_{T}^{miss};clustered-E_{T}^{miss}", 50,0,500,50,0,500) );    
  mon.addHistogram( new TH2F ("metvscentralMet", ";E_{T}^{miss};central-E_{T}^{miss}", 50,0,500,50,0,500) );  

  
  mon.addHistogram( new TH1F("sumEt",                    ";#Sigma E_{T} [GeV];Events", 100,0,2000) );
  mon.addHistogram( new TH1F("chSumEt",                    ";#Sigma E_{T}^{charged} [GeV];Events", 100,0,1000) );
  mon.addHistogram( new TH1F("neutSumEt",                    ";#Sigma E_{T}^{neutral} [GeV];Events", 100,0,1000) );
  mon.addHistogram( new TH1F("primVertexSumEt",          ";Primary Vertex : #Sigma E_{T} [GeV];Events", 100,0,1000) );
  mon.addHistogram( new TH1F("otherVertexSumEt",         ";Other Vertices : #Sigma_{vtx} #Sigma E_{T} [GeV];Events", 100,0,1000) );
  mon.addHistogram( new TH1F("chSumEtFrac",              ";#Sigma E_{T}^{charged}/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("neutSumEtFrac",            ";#Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );


  mon.addHistogram( new TH1F("centralSumEtFrac",         ";#Sigma E_{T}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("centralChSumEtFrac",       ";#Sigma E_{T}^{charged}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("centralNeutSumEtFrac",     ";#Sigma E_{T}^{neutral}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("chPrimVertexSumEtFrac",    ";#Sigma E_{T}^{charged}(PV)/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("neutPrimVertexSumEtFrac",  ";#Sigma E_{T}^{neutral}(PV)/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("chOtherVertexSumEtFrac",   ";#Sigma_{oth.vtx} #Sigma E_{T}^{charged}/#Sigma E_{T};Events", 100,0,1.) );
  mon.addHistogram( new TH1F("neutOtherVertexSumEtFrac", ";#Sigma_{oth.vtx} #Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );

  //cut optimization plots
  mon.addHistogram( new TH1F ("cutOptMediumdphill", ";#delta #phi^{l,l};#red-E_{T}^{miss,#parallel};",25,0.,3.2) );
  mon.addHistogram( new TH2F ("cutOptMediumsummtvsredMetL", ";#Sigma M_{T};#red-E_{T}^{miss,#parallel};",50,0,2000,25, -251.,249) );
  
  //cut and count
  mon.addHistogram( new TH1F("btag0proj",   ";Events with 0 b-tags;Events", 1,0,1) );
  mon.addHistogram( new TH1F("btag0projup",   ";Events with 0 b-tags (+);Events", 1,0,1) );
  mon.addHistogram( new TH1F("btag0projdown",   ";Events with 0 b-tags (-);Events", 1,0,1) );
  std::vector<TString> varsToInclude;
  varsToInclude.push_back("");
  if(runSystematics){
      varsToInclude.push_back("jer");
      varsToInclude.push_back("jesup");
      varsToInclude.push_back("jesdown");
      varsToInclude.push_back("hfactup");
      varsToInclude.push_back("hfactdown");
      varsToInclude.push_back("hrenup");
      varsToInclude.push_back("hrendown");
      varsToInclude.push_back("puup");
      varsToInclude.push_back("pudown");
  }

  for(size_t ivar=0; ivar<varsToInclude.size(); ivar++){
    TH1 *hmet = new TH1F (varsToInclude[ivar]+"finaleventflowmet",";Category;Event count;",10,0,10);
    hmet->GetXaxis()->SetBinLabel(1,"m=130");
    hmet->GetXaxis()->SetBinLabel(2,"m=150");
    hmet->GetXaxis()->SetBinLabel(3,"m=170");
    hmet->GetXaxis()->SetBinLabel(4,"m=200");
    hmet->GetXaxis()->SetBinLabel(5,"m=300");
    hmet->GetXaxis()->SetBinLabel(6,"m=400");
    hmet->GetXaxis()->SetBinLabel(7,"m=500");
    hmet->GetXaxis()->SetBinLabel(8,"m=600");
    hmet->GetXaxis()->SetBinLabel(9,"syst1");
    hmet->GetXaxis()->SetBinLabel(10,"syst2");
    mon.addHistogram( hmet );

    TH1 *hrmet = new TH1F (varsToInclude[ivar]+"finaleventflowrmet",";Category;Event count;",10,0,10);
    hrmet->GetXaxis()->SetBinLabel(1,"m=130");
    hrmet->GetXaxis()->SetBinLabel(2,"m=150");
    hrmet->GetXaxis()->SetBinLabel(3,"m=170");
    hrmet->GetXaxis()->SetBinLabel(4,"m=200");
    hrmet->GetXaxis()->SetBinLabel(5,"m=300");
    hrmet->GetXaxis()->SetBinLabel(6,"m=400");
    hrmet->GetXaxis()->SetBinLabel(7,"m=500");
    hrmet->GetXaxis()->SetBinLabel(8,"m=600");
    hrmet->GetXaxis()->SetBinLabel(9,"syst1");
    hrmet->GetXaxis()->SetBinLabel(10,"syst2");
    mon.addHistogram( hrmet );
    
    TH1 *hvbf = new TH1F (varsToInclude[ivar]+"finaleventflowvbf",";Category;Event count;",10,0,10);
    hvbf->GetXaxis()->SetBinLabel(1,"MET>50");
    hvbf->GetXaxis()->SetBinLabel(2,"MET>55");
    hvbf->GetXaxis()->SetBinLabel(3,"MET>60");
    hvbf->GetXaxis()->SetBinLabel(4,"MET>65");
    hvbf->GetXaxis()->SetBinLabel(5,"red-MET>50");
    hvbf->GetXaxis()->SetBinLabel(6,"red-MET>55");
    hvbf->GetXaxis()->SetBinLabel(7,"red-MET>60");
    hvbf->GetXaxis()->SetBinLabel(8,"red-MET>65");
    hvbf->GetXaxis()->SetBinLabel(9,"syst1");
    hvbf->GetXaxis()->SetBinLabel(10,"syst2");
    mon.addHistogram( hvbf );
  } 


  //Renormalization
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ) ;

  
  //VBF
  h = new TH1F ("VBFNEventsInc", ";Selection cut;Events", 15,0,15);
  h->GetXaxis()->SetBinLabel(1,"All");
  h->GetXaxis()->SetBinLabel(2,"2Jets (30GeV)");
  h->GetXaxis()->SetBinLabel(3,"dEta");
  h->GetXaxis()->SetBinLabel(4,"inv. Mass");
  h->GetXaxis()->SetBinLabel(5,"Central lepton");
  h->GetXaxis()->SetBinLabel(6,"Jet Veto");
  h->GetXaxis()->SetBinLabel(7,"BJet Veto");
  h->GetXaxis()->SetBinLabel(8,"3rd Lepton Veto");
  h->GetXaxis()->SetBinLabel(9,"central dilepton");
  h->GetXaxis()->SetBinLabel(10,"jet-vtx = 1");
  h->GetXaxis()->SetBinLabel(11,"jet-vtx = 2");
  mon.addHistogram( h );


  mon.addHistogram( new TH1F ("VBFMinEta", ";VBFMinEta", 100, 0,10) );                                                                                                                                                             
  mon.addHistogram( new TH1F ("VBFMaxEta", ";VBFMaxEta", 100, 0,10) );     
  mon.addHistogram( new TH2F ("VBFMinEtavspu", ";VBFMinEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH2F ("VBFMaxEtavspu", ";VBFMaxEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH2F ("VBFThirdJetPtvspu", ";VBFThirdJetPtvspu",35, 0, 35, 100, 0,30) );
  mon.addHistogram( new TH2F ("VBFThirdJetEtavspu", ";VBFThirdJetEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFdEtaInc", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFiMassInc",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFcenLeptonVetoInc",";Central Lepton Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFcen30JetVetoInc",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFNBJets30Inc", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH1F ("VBFdEtaNM1C", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFiMassNM1C",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFcenLeptonVetoNM1C",";Central Lepton Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFcen30JetVetoNM1C",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFNBJets30NM1C", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH2F ("VBFdEtaiMassNM1C", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  mon.addHistogram( new TH2F ("VBFdEtaiMassNM1C2", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFdEta3rdlepton", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFiMass3rdlepton",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFcen30JetVeto3rdlepton",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFNBJets303rdlepton", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH1F ("VBFtagvspu_noveto", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk1vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk2vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtag3rd20vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtag3rd25vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu_noveto_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu_cen", ";#generated vertices", 35, 0,35) ); 
  mon.addHistogram( new TH1F ("VBFtagtk1vspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk2vspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtag3rd20vspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu_fwd", ";#generated vertices", 35, 0,35) ); 
  mon.addHistogram( new TH1F ("VBFtagtk1vspu_fwd", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk2vspu_fwd", ";#generated vertices", 35, 0,35) );

  mon.addHistogram( new TH1F ("VBFtotalvspu", ";#generated vertices", 35, 0,35) );                        
  mon.addHistogram( new TH1F ("VBFtotalvspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtotalvspu_fwd", ";#generated vertices", 35, 0,35) );

  mon.addHistogram( new TH2F ("VBFdEtavspu", ";#generated vertices;#Delta #eta", 35, 0,35,  60, -6,6) );
  mon.addHistogram( new TH2F ("VBFdEtatkvspu", ";#generated vertices;#Delta #eta", 35, 0,35,  60, -6,6) );
  mon.addHistogram( new TH2F ("VBFchargedF1vspu", ";#generated vertices;charged fraction", 35, 0,35,  100, 0,1) );
  mon.addHistogram( new TH2F ("VBFchargedF2vspu", ";#generated vertices;charged fraction", 35, 0,35,  100, 0,1) );

  mon.addHistogram( new TH2F ("VBFNCenJet15vspu", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("VBFNCenJet15vsputk", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("VBFNCenJet30vspu", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("VBFNCenJet30vsputk", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );

  mon.addHistogram( new TH1F ("VBFNJets15", ";VBFNJets15", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets15vspu", ";VBFNJets15vspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("VBFNJets30", ";VBFNJets30", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets30vspu", ";VBFNJets30vspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("VBFNJets15tk", ";VBFNJets15tk", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets15tkvspu", ";VBFNJets15tkvspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("VBFNJets30tk", ";VBFNJets30tk", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets30tkvspu", ";VBFNJets30tkvspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH2F ("VBFTaggedVsEta", ";Eta;Tagging", 20, 0,5, 100, 0, 1) );




  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> optim_Cuts1_met;
   std::vector<double> optim_Cuts1_mindphi;
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=50;met<190;met+=2.0){
      for(double mindphi=0.0;mindphi<0.80;mindphi+=0.10){
         for(double mtmin=220;mtmin<460;mtmin+=20){
            for(double mtmax=mtmin+50;mtmax<820;mtmax+=50){
               if(mtmax>=820)mtmax=3000;
               optim_Cuts1_met    .push_back(met);
               optim_Cuts1_mindphi.push_back(mindphi);
               optim_Cuts1_mtmin  .push_back(mtmin);
               optim_Cuts1_mtmax  .push_back(mtmax);
            }
         }
      }
   }

   mon.addHistogram ( new TH1F ("optim_eventflow1"  , ";cut index;yields" ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) );
   TH1F* Hoptim_cuts1_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mindphi =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mindphi", ";cut index;mindphi",optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
      Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
      Hoptim_cuts1_mindphi->Fill(index, optim_Cuts1_mindphi[index]);
      Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
      Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
   }

   std::vector<double> optim_Cuts2_met;
   std::vector<double> optim_Cuts2_mindphi;
   std::vector<double> optim_Cuts2_mtmin;
   std::vector<double> optim_Cuts2_mtmax;
   for(double met=50;met<190;met+=2.0){
      for(double mindphi=0.0;mindphi<0.80;mindphi+=0.10){
         for(double mtmin=220;mtmin<460;mtmin+=20){
            for(double mtmax=mtmin+50;mtmax<820;mtmax+=50){
               if(mtmax>=820)mtmax=3000;
               optim_Cuts2_met    .push_back(met);
               optim_Cuts2_mindphi.push_back(mindphi);
               optim_Cuts2_mtmin  .push_back(mtmin);
               optim_Cuts2_mtmax  .push_back(mtmax);
            }
         }
      }
   }

   mon.addHistogram ( new TH1F ("optim_eventflow2"  , ";cut index;yields" ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) );
   TH1F* Hoptim_cuts2_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut2_met"    , ";cut index;met"    ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ) ;
   TH1F* Hoptim_cuts2_mindphi =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut2_mindphi", ";cut index;mindphi",optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ) ;
   TH1F* Hoptim_cuts2_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut2_mtmin"  , ";cut index;mtmin"  ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ) ;
   TH1F* Hoptim_cuts2_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut2_mtmax"  , ";cut index;mtmax"  ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts2_met.size();index++){
      Hoptim_cuts2_met    ->Fill(index, optim_Cuts2_met[index]);
      Hoptim_cuts2_mindphi->Fill(index, optim_Cuts2_mindphi[index]);
      Hoptim_cuts2_mtmin  ->Fill(index, optim_Cuts2_mtmin[index]);
      Hoptim_cuts2_mtmax  ->Fill(index, optim_Cuts2_mtmax[index]);
   }
      
   std::vector<double> optim_Cuts3_pt;                                                                                                                                                                                                        
   std::vector<double> optim_Cuts3_deta;                                                                                                                                                                                                      
   std::vector<double> optim_Cuts3_imass;                                                                                                                                                                                                     
   for(double pt=20;pt<50;pt+=5.0){                                                                                                                                                                                                           
      for(double deta=2.0;deta<5.0;deta+=0.5){                                                                                                                                                                                                
         for(double imass=200;imass<600;imass+=50){                                                                                                                                                                                           
            optim_Cuts3_pt   .push_back(pt);                                                                                                                                                                                               
            optim_Cuts3_deta .push_back(deta);                                                                                                                                                                                             
            optim_Cuts3_imass.push_back(imass);                                                                                                                                                                                            
         }                                                                                                                                                                                                                                    
      }                                                                                                                                                                                                                                       
   }               

   mon.addHistogram ( new TH1F ("optim_eventflow3"  , ";cut index;yields" ,optim_Cuts3_pt.size(),0,optim_Cuts3_pt.size()) );                                                                                                        
   TH1F* Hoptim_cuts3_pt     = (TH1F*) mon.addHistogram( new TH1F ("optim_cut3_pt"     , ";cut index;pt"     ,optim_Cuts3_pt.size(),0,optim_Cuts3_pt.size()) );
   TH1F* Hoptim_cuts3_deta   = (TH1F*) mon.addHistogram(  new TH1F ("optim_cut3_deta",    ";cut index;deta"   ,optim_Cuts3_pt.size(),0,optim_Cuts3_pt.size()) );
   TH1F* Hoptim_cuts3_imass  = (TH1F*) mon.addHistogram(  new TH1F ("optim_cut3_imass"  , ";cut index;imass"  ,optim_Cuts3_pt.size(),0,optim_Cuts3_pt.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts3_pt.size();index++){                                                                                                                                                                             
      Hoptim_cuts3_pt    ->Fill(index, optim_Cuts3_pt   [index]);                                                                                                                                                                              
      Hoptim_cuts3_deta  ->Fill(index, optim_Cuts3_deta [index]);                                                                                                                                                                              
      Hoptim_cuts3_imass ->Fill(index, optim_Cuts3_imass[index]);                                                                                                                                                                              
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
  reweight::PoissonMeanShifter PShiftUp(+0.6);
  reweight::PoissonMeanShifter PShiftDown(-0.6);


  //check PU normalized entries 
  //evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  //TEventList *elist = (TEventList*)gDirectory->Get("elist");
  //const Int_t normEntries = (elist==0 ? 0 : elist->GetN()); 
  //if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;
 

  //event Categorizer
  EventCategory eventCategoryInst(true);


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
      if(isMC && mctruthmode==1)
        {
          if(getGenProcess(ev.mccat)!=Z_CH) continue;
          if(getNgenLeptons(ev.mccat,ELECTRON)<2 && getNgenLeptons(ev.mccat,MUON)<2) continue;
        }
      if(isMC && mctruthmode==2)
        {
          if(getGenProcess(ev.mccat)!=Z_CH) continue;
          if(getNgenLeptons(ev.mccat,TAU)<2) continue;
        }
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
      //########            MC TRUTH          ########
      //##############################################

      LorentzVector genzll(0,0,0,0), genzvv(0,0,0,0), genhiggs(0,0,0,0);
      if(isMC && phys.genleptons.size()){
         genzll   = phys.genleptons[0] + phys.genleptons[1];
         genzvv   = phys.genmet[0];
         genhiggs = phys.genhiggs[0];

         mon.fillHisto("genHiggsPt"      ,tags_all,    genhiggs.pt()   ,weight);
         mon.fillHisto("genHiggsMass"    ,tags_all,    genhiggs.mass() ,weight);
         mon.fillHisto("genzllPt"        ,tags_all,    genzll.pt()     ,weight);
         mon.fillHisto("genzvvPt"        ,tags_all,    genzvv.pt()     ,weight);
      }



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



      //count jets and b-tags
      int njets(0),njetsinc(0);
      int nbtags(0), nbtags_tchel(0),   nbtags_tche2(0),  nbtags_csvl(0);
      LorentzVectorCollection jetsP4;
      std::vector<double> genJetsPt;
      int nheavyjets(0), nlightsjets(0);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++){
          jetsP4.push_back( phys.jets[ijet] );
	  genJetsPt.push_back( phys.jets[ijet].genPt );
          njetsinc++;
          if(fabs(phys.jets[ijet].eta())<2.5){
              njets++;
              bool passTCHEL(phys.jets[ijet].btag1>1.7);
              bool passTCHE2(phys.jets[ijet].btag1>2.0);
              bool passCSVL(phys.jets[ijet].btag2>0.244);
              if(phys.jets[ijet].pt()>30){
                nbtags          += passTCHE2;
                nbtags_tchel    += passTCHEL;
                nbtags_tche2    += passTCHE2;
                nbtags_csvl     += passCSVL;
                nheavyjets += (fabs(phys.jets[ijet].genid)==5);
                nlightsjets += (fabs(phys.jets[ijet].genid)!=5);
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

      //met control
      metTypeValues["met"]                 = zvv;
      metTypeValues["centralMet"]          = centralMetP4;
      metTypeValues["assocChargedMet"]     = assocChargedMetP4;
      metTypeValues["assocMet"]            = assocMetP4;
      metTypeValues["assocCMet"]           = assocCMetP4;
      metTypeValues["assocFwdCMet"]        = assocFwdCMetP4;
      metTypeValues["assocFwdMet"]         = assocFwdMetP4;
      metTypeValues["clusteredMet"]        = clusteredMetP4;
      metTypeValues["minAssocChargedMet"]  = min(zvv,assocChargedMetP4);
      metTypeValues["minAssocMet"]         = min(zvv,assocMetP4);
      metTypeValues["minAssocCMet"]        = min(zvv,assocCMetP4);
      metTypeValues["minAssocFwdMet"]      = min(zvv,assocFwdMetP4);
      metTypeValues["minAssocFwdCMet"]     = min(zvv,assocFwdCMetP4);
      metTypeValues["minClusteredMet"]     = min(zvv,clusteredMetP4);
      metTypeValues["minTAssocMet"]        = min(assocChargedMetP4,assocMetP4);
      metTypeValues["minTClusteredMet"]    = min(assocChargedMetP4,clusteredMetP4);
      metTypeValues["minAClusteredMet"]    = min(assocMetP4,clusteredMetP4);
      metTypeValues["min3Met"]             = min(zvv, min(assocMetP4,clusteredMetP4));
      metTypeValues["min4Met"]             = min(min(zvv,assocChargedMetP4), min(assocMetP4,clusteredMetP4));      
      metTypeValues["redMet"]              = redMetP4; 
      metTypeValues["redMetD0"]            = redMetD0P4;
      metTypeValues["redAssocChargedMet"]  = rTMetP4;
      metTypeValues["redAssocMet"]         = rAMetP4;
      metTypeValues["redAssocCMet"]        = rACorMetP4;
      metTypeValues["redAssocFwdMet"]      = rAFwdMetP4;
      metTypeValues["redAssocFwdCMet"]     = rAFwdCorMetP4; 
      metTypeValues["redClusteredMet"]     = rCMetP4;
      metTypeValues["redTAssocMet"]        = rTAMetP4;
      metTypeValues["redTClusteredMet"]    = rTCMetP4;
      metTypeValues["redAClusteredMet"]    = rACMetP4;
      metTypeValues["red3Met"]             = r3MetP4;
      metTypeValues["redminAssocMet"]      = rmAMetP4;
      
      std::map<TString,double> metTypeValuesminJetdphi;
      std::map<TString,double> metTypeValuesminJetphi;
      for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++){metTypeValuesminJetdphi[it->first] = 9999.0; metTypeValuesminJetphi[it->first] = 9999.0;}
      int zrank(0);
      double mindphijmet(9999.), minmtjmet(9999.),mindrjz(9999.), minmjz(9999.);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++){
          if(phys.jets[ijet].pt()>zpt) zrank++;
          //double dphijmet=fabs(deltaPhi(zvv.phi(),phys.jets[ijet].phi()));
          for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++){
              double tmpdphijmet=fabs(deltaPhi(it->second.phi(),phys.jets[ijet].phi()));             
              if(metTypeValuesminJetdphi[it->first]>tmpdphijmet){metTypeValuesminJetdphi[it->first]=tmpdphijmet;  metTypeValuesminJetphi[it->first] = phys.jets[ijet].phi();}
          }
          
          double mtjmet=METUtils::transverseMass(phys.jets[ijet],zvv,false);
          if(mtjmet<minmtjmet) minmtjmet=mtjmet;

          double drjz=deltaR(zll,phys.jets[ijet]);
          if(mindrjz>drjz) mindrjz=drjz;
          LorentzVector jz=phys.jets[ijet]+zll;
          double mjz=jz.mass();
          if(mjz<minmjz) minmjz=mjz;
      }

      //sum ETs
      float sumEt            = ev.sumEt           - ptsum;
      float sumEtcentral     = ev.sumEtcentral    - ptsum;
      float chSumEtcentral   = ev.chsumEtcentral  - ptsum;
      float neutSumEtcentral = ev.neutsumEtcentral;
      float chSumEt          = ev.chsumEt         - ptsum;
      float neutsumEt        = ev.neutsumEt;
      
      mindphijmet = metTypeValuesminJetdphi["met"];      
    
      //##############################################
      //########     GLOBAL SELECTION         ########
      //##############################################

      bool passLooseKinematics(zpt>20);
      bool passZmass(fabs(zmass-91)<15);
      bool passSideBand( !passZmass && fabs(zmass-91)<30 );

      bool passZpt(zpt>25);

      bool pass3dLeptonVeto(true); for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)pass3dLeptonVeto=false;}
      bool passBveto(nbtags==0);
      bool passMediumRedMet(50);
      bool passTightRedMet(84);//redMet>METUtils::getRedMETCut(eventCategory,METUtils::TIGHTWP));
      
      //bool passBaseCuts(passZmass && pass3dLeptonVeto && passBveto && passMediumRedMet);

      //##############################################
      //########         GENERAL PLOTS        ########
      //##############################################

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
      mon.fillHisto  ("zrank"    ,tags_full,zrank   ,iweight);
      mon.fill2DHisto("zptvszeta",tags_full,zpt,zeta,iweight);
      mon.fillHisto  ("dphill"   ,tags_full,dphill  ,iweight);
      mon.fillHisto  ("mindrlz"  ,tags_full,mindrlz ,iweight);
      mon.fillHisto  ("maxdrlz"  ,tags_full,maxdrlz ,iweight);
      mon.fillHisto  ("ptsum"    ,tags_full,ptsum   ,iweight);
      mon.fillHisto  ("mtl1"     ,tags_full,mtl1    ,iweight);
      mon.fillHisto  ("mtl2"     ,tags_full,mtl2    ,iweight);
      mon.fillHisto  ("mt"       ,tags_full,mt      ,iweight);

      if(passZmass){
        mon.fillHisto("genzwinHiggsPt"      ,tags_full,    genhiggs.pt()   ,iweight);
        mon.fillHisto("genzwinHiggsMass"    ,tags_full,    genhiggs.mass() ,iweight);
        mon.fillHisto("genzwinzllPt"        ,tags_full,    genzll.pt()     ,iweight);
        mon.fillHisto("genzwinzvvPt"        ,tags_full,    genzvv.pt()     ,iweight);
      }


      if((passZmass || passSideBand) && passLooseKinematics){
         mon.fillHisto("eventflow",tags_full,1,iweight);
         mon.fillHisto("nvtx",tags_full,ev.nvtx,iweight);
         mon.fillHisto("nvtxPlus",tags_full,ev.nvtx,iweight*TotalWeight_plus);
         mon.fillHisto("nvtxMinus",tags_full,ev.nvtx,iweight*TotalWeight_minus);
      }
      
      if(passZmass && passLooseKinematics){
         mon.fillHisto("eventflow",tags_full,2,iweight);
     
      //jet control
         mon.fillHisto("njets",tags_full,njets,iweight);
         mon.fill2DHisto("njetsvspu",tags_full,ev.ngenITpu,njets,iweight);
         mon.fill2DHisto("njetsincvspu",tags_full,ev.ngenITpu,njetsinc,iweight);
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

             //extra leptons
               mon.fillHisto("nleptons",tags_full,ev.ln,iweight);
               if(!pass3dLeptonVeto){
                   mon.fillHisto("njets3leptons",tags_full,njets,iweight);
                   mon.fillHisto("nbtags3leptons",tags_full,nbtags,iweight);
               }

               if(pass3dLeptonVeto){                 
                  mon.fillHisto("eventflow",tags_full,5,iweight);
      
                  if(isMC && genhiggs.pt()>0){
                      // play with boosts
                      LorentzVector transverseHiggs(genhiggs.px(),genhiggs.py(), 0,sqrt(pow(genhiggs.mass(),2)+pow(genhiggs.pt(),2)));
                      ROOT::Math::Boost cmboost(transverseHiggs.BoostToCM());
                      LorentzVector cmzll(cmboost(zll));
                      LorentzVector cmzvv(cmboost(zvv));

                      mon.fillHisto("boost_cm_zll_p"  ,tags_cat,    cmzll.pt()                                   ,iweight);
                      mon.fillHisto("boost_cm_zvv_p"  ,tags_cat,    cmzvv.pt()                                   ,iweight);
                      mon.fillHisto("boost_cm_delta_p",tags_cat,    cmzll.pt()-cmzvv.pt()                        ,iweight);
                      mon.fillHisto("boost_cm_mass"   ,tags_cat,    METUtils::transverseMass(cmzll,cmzvv,true)   ,iweight);
                  }

                  for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++){
                      if(it->second.pt()>50 && metTypeValuesminJetdphi[it->first]<10){
                          mon.fillHisto(TString("met_") + it->first+"_mindphijmet",tags_full,metTypeValuesminJetdphi[it->first], iweight);
                       // mon.fill2DHisto(TString("met_") + it->first+"phimetphijet", tags_full,it->second.phi(),metTypeValuesminJetphi[it->first],iweight);
                      }
    
                      TVector2 metPt(it->second.px(), it->second.py());
                      TVector2 dileptonPt(zll.px(), zll.py());
                      TVector2 dileptonLongi( dileptonPt.Unit() );
                      TVector2 dileptonPerp( dileptonLongi.Rotate(TMath::Pi()/2) );
                      mon.fillHisto(TString("met_") + it->first + "_inc"    , tags_full, metPt*dileptonLongi, iweight);
                      mon.fillHisto(TString("met_") + it->first + "_L"      , tags_full, metPt*dileptonLongi, iweight);
                      mon.fillHisto(TString("met_") + it->first + "_T"      , tags_full, metPt*dileptonPerp, iweight);
                      mon.fillHisto(TString("met_") + it->first + "_overqt" , tags_full, it->second.pt()/zpt, iweight);
                      mon.fillHisto(TString("met_") + it->first + "_overqtL", tags_full, metPt*dileptonLongi/zpt, iweight);
                      mon.fillHisto(TString("met_") + it->first + "_overqtT", tags_full, metPt*dileptonPerp/zpt,iweight);

                      if(mindphijmet<0.5)continue;
                      mon.fillHisto(TString("met_") + it->first, tags_full,it->second.pt(),iweight);
          
                      mon.fill2DHisto(TString("met_") + it->first+"_vspu", tags_full,ev.ngenITpu,it->second.pt(),iweight);
                      mon.fill2DHisto(TString("met_") + it->first+"zpt", tags_full,it->second.pt(),zpt,iweight);
                      mon.fillHisto(TString("met_") + it->first+"minzpt", tags_full,std::min(it->second.pt(),zll.pt()),iweight);

                      TVector2 zll2DLong  = TVector2(zll.px()/zll.pt(), zll.py()/zll.pt());
                      TVector2 zll2DTrans = zll2DLong.Rotate(TMath::Pi()/2);
                  }

                  mon.fill2DHisto("metvstkmet", tags_full,met,assocChargedMetP4.pt(),iweight);
                  mon.fill2DHisto("metvsassoc", tags_full,met,assocMetP4.pt(),iweight);
                  mon.fill2DHisto("metvsclustered", tags_full,met,clusteredMetP4.pt(),iweight);
                  mon.fill2DHisto("metvscentralMet", tags_full,met,centralMetP4.pt(),iweight);
                  mon.fillHisto("redMetL", tags_full,redMetL,iweight);
                  mon.fillHisto("redMetT", tags_full,redMetT,iweight);
                  mon.fillHisto("redMetcomps", tags_full,redMetL,redMetT,iweight);        
          
                  if(sumEt>0){
                      mon.fillHisto("sumEt",                tags_full,sumEt,iweight);
                      mon.fillHisto("chSumEt",              tags_full,chSumEt,iweight);
                      mon.fillHisto("neutSumEt",            tags_full,neutsumEt,iweight);
                      mon.fillHisto("primVertexSumEt",      tags_full,ev.primVertexSumEt,iweight);
                      mon.fillHisto("otherVertexSumEt",     tags_full,ev.otherVertexSumEt,iweight);
          
                      if(isMC){
                          mon.fillHisto("sumEt"+ootCond,tags_full,sumEt,iweight);
                          mon.fillHisto("neutSumEtFrac"+ootCond,tags_full,neutsumEt/sumEt,iweight);
                          mon.fill2DHisto("itpuvsootpu",tags_full,ev.ngenITpu,ev.ngenOOTpu,iweight);
                      }
                      mon.fillHisto("chSumEtFrac",          tags_full,chSumEt/sumEt,iweight);
                      mon.fillHisto("neutSumEtFrac",        tags_full,neutsumEt/sumEt,iweight);
                      mon.fillHisto("centralSumEtFrac",     tags_full,sumEtcentral/sumEt,iweight);
                      mon.fillHisto("centralChSumEtFrac",   tags_full,chSumEtcentral/sumEt,iweight);
                      mon.fillHisto("centralNeutSumEtFrac", tags_full,neutSumEtcentral/sumEt,iweight);
                      mon.fillHisto("chPrimVertexSumEtFrac",          tags_full,ev.primVertexChSumEt/sumEt,iweight);
                      mon.fillHisto("neutPrimVertexSumEtFrac",        tags_full,ev.primVertexNeutSumEt/sumEt,iweight);
                      mon.fillHisto("chOtherVertexSumEtFrac",          tags_full,ev.otherVertexChSumEt/sumEt,iweight);
                      mon.fillHisto("neutOtherVertexSumEtFrac",        tags_full,ev.otherVertexNeutSumEt/sumEt,iweight);                 
                  }
                
                  mon.fillHisto("mindphijmet",tags_full,mindphijmet,iweight);
                  mon.fillHisto("minmtjmet",tags_full,minmtjmet,iweight);
                  mon.fillHisto("mindrjz",tags_full,mindrjz,iweight);
                  mon.fillHisto("minmjz",tags_full,minmjz,iweight);
                
                
                  //sample is selected
                  if(passMediumRedMet){
                      mon.fillHisto("eventflow",tags_full,6,iweight);
                      mon.fillHisto("eventSubCat",tags_full,eventSubCat,iweight);
                      mon.fillHisto("cutOptMediumdphill",tags_full,fabs(dphill));
                      mon.fill2DHisto("cutOptMediumsummtvsredMetL",tags_full,mtsum,redMetL,iweight);                    
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
      bool VBFPassJetAssocVeto = false;
      bool VBFPassCentralDilep = false;

      double  VBFMinEta = 0;                                                                                                                                                                                                                 
      double  VBFMaxEta = 0;       
      double  VBFdEta = -1;
      int     VBFCentral15Jets = 0;
      int     VBFCentral15Jetstk = 0;
      int     VBFCentral30Jets = 0;
      int     VBFCentral30Jetstk = 0;
      int     VBFCentralLeptons = 0;
      int     VBFNBJets=0;
      int     VBFNJetsAssoc=0;
      LorentzVector VBFSyst;

      if(passZmass){
          int VBFNJets15 = 0; int VBFNJets15tk = 0; for(size_t ijet=0; ijet<phys.jets.size(); ijet++){if(phys.jets[ijet].pt()>15){VBFNJets15++; if(phys.jets[ijet].chHadFrac>0.25)VBFNJets15tk++;} }
          int VBFNJets30 = 0; int VBFNJets30tk = 0; for(size_t ijet=0; ijet<phys.jets.size(); ijet++){if(phys.jets[ijet].pt()>30){VBFNJets30++; if(phys.jets[ijet].chHadFrac>0.25)VBFNJets30tk++;} }
          mon.fillHisto("VBFNJets15"  ,tags_full,    VBFNJets15 ,iweight);
          mon.fill2DHisto("VBFNJets15vspu"  ,tags_full,    ev.ngenITpu,  VBFNJets15 ,1);
          mon.fillHisto("VBFNJets30"  ,tags_full,    VBFNJets30 ,iweight);
          mon.fill2DHisto("VBFNJets30vspu"  ,tags_full,    ev.ngenITpu,  VBFNJets30 ,1);
          mon.fillHisto("VBFNJets15tk"  ,tags_full,    VBFNJets15tk ,iweight);
          mon.fill2DHisto("VBFNJets15tkvspu"  ,tags_full,    ev.ngenITpu,  VBFNJets15tk ,1);
          mon.fillHisto("VBFNJets30tk"  ,tags_full,    VBFNJets30tk ,iweight);
          mon.fill2DHisto("VBFNJets30tkvspu"  ,tags_full,    ev.ngenITpu,  VBFNJets30tk ,1);
      }


      VBFCentral30Jets = 0;
      VBFCentral30Jetstk = 0;
      VBFCentral15Jets = 0;
      VBFCentral15Jetstk = 0;
      if(passZmass){
         if(phys.jets.size()>=2){
             VBFSyst =  phys.jets[0] + phys.jets[1];
             VBFdEta = fabs(   phys.jets[0].eta() -    phys.jets[1].eta()); if(phys.jets[0].eta()* phys.jets[1].eta()>0)VBFdEta*=-1;
             double MinEta, MaxEta;

             if(phys.jets[0].eta()<phys.jets[1].eta()){MinEta=phys.jets[0].eta(); MaxEta=phys.jets[1].eta();}else{MinEta=phys.jets[1].eta(); MaxEta=phys.jets[0].eta();}
             if(isGammaEvent){  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
             }else{             if(zll.eta()>MinEta && zll.eta()<MaxEta) VBFCentralLeptons=2;
             }

             VBFNJetsAssoc=0;
             if(phys.jets[0].chHadFrac>0.25)VBFNJetsAssoc++;
             if(phys.jets[1].chHadFrac>0.25)VBFNJetsAssoc++;

             for(size_t ijet=2; ijet<phys.jets.size(); ijet++){
                 if(phys.jets[ijet].pt()<15)continue;
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral15Jets++;
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta && phys.jets[ijet].chHadFrac>0.25)VBFCentral15Jetstk++;
                 if(phys.jets[ijet].pt()<30)continue; 
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral30Jets++; 
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta && phys.jets[ijet].chHadFrac>0.25)VBFCentral30Jetstk++;
                 if(phys.jets[ijet].btag1>1.7) VBFNBJets++;
             }
          
             VBFPass2Jet30   = (phys.jets [0].pt()>30.0 && phys.jets [1].pt()>30.0);
             VBFPassdEtaCut  = VBFPass2Jet30 && (fabs(VBFdEta)>4.0);
             VBFPassiMCut    = VBFPass2Jet30 && (VBFSyst.M()>500);
             VBFPassLeptonIn = (VBFCentralLeptons==2);
             VBFPassJetVeto  = (VBFCentral30Jets==0);
             VBFPassBJetVeto = (VBFNBJets==0);
             VBFPassJetAssocVeto = (VBFNJetsAssoc>0); 
             VBFPassCentralDilep = (zll.eta()>MinEta && zll.eta()<MaxEta);
             isVBF        = (VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto && VBFPassJetVeto && VBFPassLeptonIn && VBFPassCentralDilep);
             VBFMinEta = std::min(fabs(phys.jets[0].eta()), fabs(phys.jets[1].eta()));
             VBFMaxEta = std::max(fabs(phys.jets[0].eta()), fabs(phys.jets[1].eta()));

            if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto)mon.fill2DHisto("VBFTaggedVsEta", tags_full, fabs(phys.jets [0].eta()), phys.jets[0].chHadFrac, 1);
            if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto)mon.fill2DHisto("VBFTaggedVsEta", tags_full, fabs(phys.jets [1].eta()), phys.jets[1].chHadFrac, 1);

            if(VBFPassdEtaCut && VBFPassiMCut){
               mon.fill2DHisto("VBFchargedF1vspu", tags_full, ev.ngenITpu, phys.jets[0].chHadFrac, 1);
               mon.fill2DHisto("VBFchargedF2vspu", tags_full, ev.ngenITpu, phys.jets[1].chHadFrac, 1);
            }
         }

         //VBF control
         if(true                                                                                        )mon.fillHisto("VBFNEventsInc"       ,tags_full,    0                ,iweight);
         if(VBFPass2Jet30                                                                                  )mon.fillHisto("VBFNEventsInc"       ,tags_full,    1                ,iweight);
         if(VBFPassdEtaCut                                                                                 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    2                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fillHisto("VBFNEventsInc"       ,tags_full,    3                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fillHisto("VBFNEventsInc"       ,tags_full,    4                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    5                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    6                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto )mon.fillHisto("VBFNEventsInc"       ,tags_full,    7                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep)mon.fillHisto("VBFNEventsInc"       ,tags_full,    8                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && VBFNJetsAssoc>=1 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    9                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && VBFNJetsAssoc>=2 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    10                ,iweight);


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

         mon.fillHisto("VBFtotalvspu"      ,tags_full,    ev.ngenITpu   ,1);
         if(fabs(VBFMinEta)<2.4){
            mon.fillHisto("VBFtotalvspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
         }else{
            mon.fillHisto("VBFtotalvspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
         }



	 if(VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut){
            mon.fillHisto("VBFtagvspu_noveto"      ,tags_full,    ev.ngenITpu   ,1);

            if(fabs(VBFMinEta)<2.4){
               mon.fillHisto("VBFtagvspu_noveto_cen"      ,tags_full,    ev.ngenITpu   ,1);
            }else{
               mon.fillHisto("VBFtagvspu_noveto_fwd"      ,tags_full,    ev.ngenITpu   ,1);
            } 

         }

         if(isVBF){
            mon.fillHisto("VBFMinEta"  ,tags_full,    VBFMinEta ,iweight);
            mon.fillHisto("VBFMaxEta"  ,tags_full,    VBFMaxEta ,iweight);

            mon.fill2DHisto("VBFMinEtavspu"  ,tags_full, ev.ngenITpu,   VBFMinEta ,1);
            mon.fill2DHisto("VBFMaxEtavspu"  ,tags_full, ev.ngenITpu,   VBFMaxEta ,1);

            if(phys.jets.size()>2){
               mon.fill2DHisto("VBFThirdJetPtvspu"  ,tags_full, ev.ngenITpu, phys.jets[2].pt()   ,1);
               if(phys.jets[2].pt()>20){
               mon.fill2DHisto("VBFThirdJetEtavspu"  ,tags_full, ev.ngenITpu, fabs(phys.jets[2].eta())   ,1);
               }
            }

            mon.fillHisto("VBFtagvspu"      ,tags_full,    ev.ngenITpu   ,1); 
            if(VBFNJetsAssoc>=1)mon.fillHisto("VBFtagtk1vspu"      ,tags_full,    ev.ngenITpu   ,1);
            if(VBFNJetsAssoc>=2)mon.fillHisto("VBFtagtk2vspu"      ,tags_full,    ev.ngenITpu   ,1);

            if(fabs(VBFMinEta)<2.4){
               mon.fillHisto("VBFtagvspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=1)mon.fillHisto("VBFtagtk1vspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=2)mon.fillHisto("VBFtagtk2vspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
            }else{
               mon.fillHisto("VBFtagvspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=1)mon.fillHisto("VBFtagtk1vspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=2)mon.fillHisto("VBFtagtk2vspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
            }

            if(phys.jets.size()<=2 || phys.jets[2].pt()<=20)mon.fillHisto("VBFtag3rd20vspu"      ,tags_full,    ev.ngenITpu   ,1);
            if(phys.jets.size()<=2 || phys.jets[2].pt()<=25)mon.fillHisto("VBFtag3rd25vspu"      ,tags_full,    ev.ngenITpu   ,1);
         }


         if(VBFPass2Jet30 && VBFPassiMCut)mon.fill2DHisto("VBFdEtavspu", tags_full, ev.ngenITpu, VBFdEta, 1);
         if(VBFPass2Jet30 && VBFPassiMCut && VBFNJetsAssoc>=1)mon.fill2DHisto("VBFdEtatkvspu", tags_full, ev.ngenITpu, VBFdEta, 1);

         if(VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto){
            mon.fill2DHisto("VBFNCenJet15vspu"  , tags_full, ev.ngenITpu, VBFCentral15Jets, 1);
            mon.fill2DHisto("VBFNCenJet15vsputk", tags_full, ev.ngenITpu, VBFCentral15Jetstk, 1);
            mon.fill2DHisto("VBFNCenJet30vspu"  , tags_full, ev.ngenITpu, VBFCentral30Jets, 1);
            mon.fill2DHisto("VBFNCenJet30vsputk", tags_full, ev.ngenITpu, VBFCentral30Jetstk, 1);
         }

      }






      //##############################################
      //######## CUT & COUNT FOR FINAL SEL.   ########
      //##############################################
      bool PassSelMet [] = {false, false, false, false, false, false, false, false, false, false};
      bool PassSelRMet[] = {false, false, false, false, false, false, false, false, false, false};
      bool PassSelVBF [] = {false, false, false, false, false, false, false, false, false, false};
 
      if(passZmass && passLooseKinematics && passBveto && passZpt && pass3dLeptonVeto){
         PassSelMet[0]  = (zvv.pt()> 50 && mindphijmet>0.74 && mt>171 && mt<296); if(PassSelMet[0])mon.fillHisto("finaleventflowmet",tags_full,0,iweight); //mH=130
         PassSelMet[1]  = (zvv.pt()> 50 && mindphijmet>0.70 && mt>193 && mt<284); if(PassSelMet[1])mon.fillHisto("finaleventflowmet",tags_full,1,iweight); //mH=150
         PassSelMet[2]  = (zvv.pt()> 50 && mindphijmet>0.66 && mt>185 && mt<276); if(PassSelMet[2])mon.fillHisto("finaleventflowmet",tags_full,2,iweight); //mH=170
         PassSelMet[3]  = (zvv.pt()> 50 && mindphijmet>0.60 && mt>220 && mt<270); if(PassSelMet[3])mon.fillHisto("finaleventflowmet",tags_full,3,iweight); //mH=200
         PassSelMet[4]  = (zvv.pt()> 86 && mindphijmet>0.40 && mt>260 && mt<310); if(PassSelMet[4])mon.fillHisto("finaleventflowmet",tags_full,4,iweight); //mH=300
         PassSelMet[5]  = (zvv.pt()>118 && mindphijmet>0.20 && mt>340 && mt<440); if(PassSelMet[5])mon.fillHisto("finaleventflowmet",tags_full,5,iweight); //mH=400
         PassSelMet[6]  = (zvv.pt()>166 && mindphijmet>0.10 && mt>340 && mt<740); if(PassSelMet[6])mon.fillHisto("finaleventflowmet",tags_full,6,iweight); //mH=500
         PassSelMet[7]  = (zvv.pt()>188 && mindphijmet>0.10 && mt>440 && mt<740); if(PassSelMet[7])mon.fillHisto("finaleventflowmet",tags_full,7,iweight); //mH=600
         PassSelMet[8]  = (zvv.pt()> 50 && mindphijmet>0.05 && mt>150          ); if(PassSelMet[8])mon.fillHisto("finaleventflowmet",tags_full,8,iweight); //syst1
         PassSelMet[9]  = (zvv.pt()> 60 && mindphijmet>0.10 && mt>170          ); if(PassSelMet[9])mon.fillHisto("finaleventflowmet",tags_full,9,iweight); //syst2
   
         PassSelRMet[0] = (rCMetP4.pt()> 50 && mindphijmet>0.74 && mt>171 && mt<296); if(PassSelRMet[0])mon.fillHisto("finaleventflowrmet",tags_full,0,iweight); //mH=130
         PassSelRMet[1] = (rCMetP4.pt()> 50 && mindphijmet>0.70 && mt>193 && mt<284); if(PassSelRMet[1])mon.fillHisto("finaleventflowrmet",tags_full,1,iweight); //mH=150
         PassSelRMet[2] = (rCMetP4.pt()> 50 && mindphijmet>0.66 && mt>185 && mt<276); if(PassSelRMet[2])mon.fillHisto("finaleventflowrmet",tags_full,2,iweight); //mH=170
         PassSelRMet[3] = (rCMetP4.pt()> 50 && mindphijmet>0.60 && mt>220 && mt<270); if(PassSelRMet[3])mon.fillHisto("finaleventflowrmet",tags_full,3,iweight); //mH=200
         PassSelRMet[4] = (rCMetP4.pt()> 84 && mindphijmet>0.20 && mt>260 && mt<310); if(PassSelRMet[4])mon.fillHisto("finaleventflowrmet",tags_full,4,iweight); //mH=300
         PassSelRMet[5] = (rCMetP4.pt()>110 && mindphijmet>0.20 && mt>340 && mt<440); if(PassSelRMet[5])mon.fillHisto("finaleventflowrmet",tags_full,5,iweight); //mH=400
         PassSelRMet[6] = (rCMetP4.pt()>156 && mindphijmet>0.10 && mt>340 && mt<740); if(PassSelRMet[6])mon.fillHisto("finaleventflowrmet",tags_full,6,iweight); //mH=500
         PassSelRMet[7] = (rCMetP4.pt()>156 && mindphijmet>0.10 && mt>440 && mt<790); if(PassSelRMet[7])mon.fillHisto("finaleventflowrmet",tags_full,7,iweight); //mH=600
         PassSelRMet[8] = (rCMetP4.pt()> 50 && mindphijmet>0.05 && mt>150          ); if(PassSelRMet[8])mon.fillHisto("finaleventflowrmet",tags_full,8,iweight); //syst1
         PassSelRMet[9] = (rCMetP4.pt()> 60 && mindphijmet>0.10 && mt>170          ); if(PassSelRMet[9])mon.fillHisto("finaleventflowrmet",tags_full,9,iweight); //syst2

         PassSelVBF[0]  = (zvv    .pt()>50); if(PassSelVBF[0])mon.fillHisto("finaleventflowvbf",tags_full,0,iweight); //VBF met 50
         PassSelVBF[1]  = (zvv    .pt()>55); if(PassSelVBF[1])mon.fillHisto("finaleventflowvbf",tags_full,1,iweight); //VBF met 55
         PassSelVBF[2]  = (zvv    .pt()>60); if(PassSelVBF[2])mon.fillHisto("finaleventflowvbf",tags_full,2,iweight); //VBF met 60
         PassSelVBF[3]  = (zvv    .pt()>65); if(PassSelVBF[3])mon.fillHisto("finaleventflowvbf",tags_full,3,iweight); //VBF met 65
         PassSelVBF[4]  = (rCMetP4.pt()>50); if(PassSelVBF[4])mon.fillHisto("finaleventflowvbf",tags_full,4,iweight); //VBFrmet 50
         PassSelVBF[5]  = (rCMetP4.pt()>55); if(PassSelVBF[5])mon.fillHisto("finaleventflowvbf",tags_full,5,iweight); //VBFrmet 55
         PassSelVBF[6]  = (rCMetP4.pt()>60); if(PassSelVBF[6])mon.fillHisto("finaleventflowvbf",tags_full,6,iweight); //VBFrmet 60
         PassSelVBF[7]  = (rCMetP4.pt()>65); if(PassSelVBF[7])mon.fillHisto("finaleventflowvbf",tags_full,7,iweight); //VBFrmet 65
         PassSelVBF[8]  = (zvv    .pt()>40); if(PassSelVBF[8])mon.fillHisto("finaleventflowvbf",tags_full,8,iweight); //VBF met 40
         PassSelVBF[9]  = (rCMetP4.pt()>40); if(PassSelVBF[9])mon.fillHisto("finaleventflowvbf",tags_full,9,iweight); //VBFrmet 40

         //computation for systematics
         LorentzVectorCollection metVars, redMetVars;
         std::vector<LorentzVectorCollection> jetVars;
         std::vector<int> eventCategoryVars;
         std::vector<Float_t>  mtVars;
         if(runSystematics){
	   METUtils::computeVariation(jetsP4,genJetsPt,phys.met[0],jetVars, metVars,&jecUnc);
              for(size_t ivar=0; ivar<3; ivar++){
                eventCategoryVars.push_back( eventCategoryInst.Get(phys, &(jetVars[ivar])) );
              
                 METUtils::stRedMET temp_redMetInfo;
                 LorentzVector temp_redMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetVars[ivar], metVars[ivar], isGammaEvent, &temp_redMetInfo);
                 redMetVars.push_back(temp_redMetP4);
              
                 Float_t imt     = METUtils::transverseMass(zll,metVars[ivar],true);
                 mtVars.push_back(imt);
              }

              bool iPassSelMet[10];
              bool iPassSelRMet[10];
              bool iPassSelVBF[10];
         
              //jet energy scale related variations
              TString jetVarNames[]={"jer","jesup","jesdown"};
              for(size_t ivar=0; ivar<3; ivar++){
     	         std::cout << "TESTA\n";
                 TString isubcat    = eventCategoryInst.GetLabel( eventCategoryVars[ivar] );
                 std::cout << "TESTB\n";
                 std::vector<TString> itags_full;
                 itags_full.push_back(tag_cat + isubcat);

                 std::cout << "TESTC\n";

                 iPassSelMet[0]  = (zvv.pt()> 50 && mindphijmet>0.74 && mt>171 && mt<296); if(iPassSelMet[0])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,0,iweight); //mH=130
                 iPassSelMet[1]  = (zvv.pt()> 50 && mindphijmet>0.70 && mt>193 && mt<284); if(iPassSelMet[1])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,1,iweight); //mH=150
                 iPassSelMet[2]  = (zvv.pt()> 50 && mindphijmet>0.66 && mt>185 && mt<276); if(iPassSelMet[2])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,2,iweight); //mH=170
                 iPassSelMet[3]  = (zvv.pt()> 50 && mindphijmet>0.60 && mt>220 && mt<270); if(iPassSelMet[3])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,3,iweight); //mH=200
                 iPassSelMet[4]  = (zvv.pt()> 86 && mindphijmet>0.40 && mt>260 && mt<310); if(iPassSelMet[4])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,4,iweight); //mH=300
                 iPassSelMet[5]  = (zvv.pt()>118 && mindphijmet>0.20 && mt>340 && mt<440); if(iPassSelMet[5])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,5,iweight); //mH=400
                 iPassSelMet[6]  = (zvv.pt()>166 && mindphijmet>0.10 && mt>340 && mt<740); if(iPassSelMet[6])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,6,iweight); //mH=500
                 iPassSelMet[7]  = (zvv.pt()>188 && mindphijmet>0.10 && mt>440 && mt<740); if(iPassSelMet[7])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,7,iweight); //mH=600
                 iPassSelMet[8]  = (zvv.pt()> 50 && mindphijmet>0.05 && mt>150          ); if(iPassSelMet[8])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,8,iweight); //syst1
                 iPassSelMet[9]  = (zvv.pt()> 60 && mindphijmet>0.10 && mt>170          ); if(iPassSelMet[9])mon.fillHisto(jetVarNames[ivar]+"finaleventflowmet",itags_full,9,iweight); //syst2

                 iPassSelRMet[0] = (rCMetP4.pt()> 50 && mindphijmet>0.74 && mt>171 && mt<296); if(iPassSelRMet[0])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,0,iweight); //mH=130
                 iPassSelRMet[1] = (rCMetP4.pt()> 50 && mindphijmet>0.70 && mt>193 && mt<284); if(iPassSelRMet[1])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,1,iweight); //mH=150
                 iPassSelRMet[2] = (rCMetP4.pt()> 50 && mindphijmet>0.66 && mt>185 && mt<276); if(iPassSelRMet[2])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,2,iweight); //mH=170
                 iPassSelRMet[3] = (rCMetP4.pt()> 50 && mindphijmet>0.60 && mt>220 && mt<270); if(iPassSelRMet[3])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,3,iweight); //mH=200
                 iPassSelRMet[4] = (rCMetP4.pt()> 84 && mindphijmet>0.20 && mt>260 && mt<310); if(iPassSelRMet[4])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,4,iweight); //mH=300
                 iPassSelRMet[5] = (rCMetP4.pt()>110 && mindphijmet>0.20 && mt>340 && mt<440); if(iPassSelRMet[5])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,5,iweight); //mH=400
                 iPassSelRMet[6] = (rCMetP4.pt()>156 && mindphijmet>0.10 && mt>340 && mt<740); if(iPassSelRMet[6])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,6,iweight); //mH=500
                 iPassSelRMet[7] = (rCMetP4.pt()>156 && mindphijmet>0.10 && mt>440 && mt<790); if(iPassSelRMet[7])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,7,iweight); //mH=600
                 iPassSelRMet[8] = (rCMetP4.pt()> 50 && mindphijmet>0.05 && mt>150          ); if(iPassSelRMet[8])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,8,iweight); //syst1
                 iPassSelRMet[9] = (rCMetP4.pt()> 60 && mindphijmet>0.10 && mt>170          ); if(iPassSelRMet[9])mon.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",itags_full,9,iweight); //syst2

                 iPassSelVBF[0]  = (zvv    .pt()>50); if(iPassSelVBF[0])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,0,iweight); //VBF met 50
                 iPassSelVBF[1]  = (zvv    .pt()>55); if(iPassSelVBF[1])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,1,iweight); //VBF met 55
                 iPassSelVBF[2]  = (zvv    .pt()>60); if(iPassSelVBF[2])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,2,iweight); //VBF met 60
                 iPassSelVBF[3]  = (zvv    .pt()>65); if(iPassSelVBF[3])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,3,iweight); //VBF met 65
                 iPassSelVBF[4]  = (rCMetP4.pt()>50); if(iPassSelVBF[4])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,4,iweight); //VBFrmet 50
                 iPassSelVBF[5]  = (rCMetP4.pt()>55); if(iPassSelVBF[5])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,5,iweight); //VBFrmet 55
                 iPassSelVBF[6]  = (rCMetP4.pt()>60); if(iPassSelVBF[6])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,6,iweight); //VBFrmet 60
                 iPassSelVBF[7]  = (rCMetP4.pt()>65); if(iPassSelVBF[7])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,7,iweight); //VBFrmet 65
                 iPassSelVBF[8]  = (zvv    .pt()>40); if(iPassSelVBF[8])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,8,iweight); //VBF met 40
                 iPassSelVBF[9]  = (rCMetP4.pt()>40); if(iPassSelVBF[9])mon.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",itags_full,9,iweight); //VBFrmet 40
              }
         
              //re-weighting variations (Higgs, pileup scenario)
              TString wgtVarNames[]={"hrenup","hrendown","hfactup","hfactdown","puup","pudown"};
              Float_t rwgtVars[]={ isMC_GG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]   : iweight ,
                                   isMC_GG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor] : iweight ,
                                   isMC_GG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]  : iweight ,
                                   isMC_GG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]: iweight ,
                                   iweight*TotalWeight_plus,
                                   iweight*TotalWeight_minus};

              if(ev.hptWeights[ZZ2l2nuSummary_t::hKfactor] <0.5)
                 cout << " " << phys.genhiggs[0].pt() << " " << isMC_GG 
                      << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor] 
                      << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]
                      << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]
                      << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]
                      << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown] << endl;
          
              for(size_t ivar=0; ivar<sizeof(wgtVarNames)/sizeof(TString); ivar++){
                 std::vector<TString> itags_full;
                 itags_full.push_back(tag_cat + tag_subcat);

                 if(PassSelMet[0]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,0,rwgtVars[ivar]);
                 if(PassSelMet[1]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,1,rwgtVars[ivar]);
                 if(PassSelMet[2]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,2,rwgtVars[ivar]);
                 if(PassSelMet[3]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,3,rwgtVars[ivar]);
                 if(PassSelMet[4]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,4,rwgtVars[ivar]);
                 if(PassSelMet[5]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,5,rwgtVars[ivar]);
                 if(PassSelMet[6]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,6,rwgtVars[ivar]);
                 if(PassSelMet[7]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,7,rwgtVars[ivar]);
                 if(PassSelMet[8]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,8,rwgtVars[ivar]);
                 if(PassSelMet[9]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",itags_full,9,rwgtVars[ivar]);
              
                 if(PassSelRMet[0]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,0,rwgtVars[ivar]);
                 if(PassSelRMet[1]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,1,rwgtVars[ivar]);
                 if(PassSelRMet[2]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,2,rwgtVars[ivar]);
                 if(PassSelRMet[3]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,3,rwgtVars[ivar]);
                 if(PassSelRMet[4]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,4,rwgtVars[ivar]);
                 if(PassSelRMet[5]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,5,rwgtVars[ivar]);
                 if(PassSelRMet[6]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,6,rwgtVars[ivar]);
                 if(PassSelRMet[7]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,7,rwgtVars[ivar]);                  
                 if(PassSelRMet[8]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,8,rwgtVars[ivar]);
                 if(PassSelRMet[9]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",itags_full,9,rwgtVars[ivar]);

                 if(PassSelVBF[0]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,0,rwgtVars[ivar]);
                 if(PassSelVBF[1]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,1,rwgtVars[ivar]);
                 if(PassSelVBF[2]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,2,rwgtVars[ivar]);
                 if(PassSelVBF[3]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,3,rwgtVars[ivar]);
                 if(PassSelVBF[4]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,4,rwgtVars[ivar]);
                 if(PassSelVBF[5]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,5,rwgtVars[ivar]);
                 if(PassSelVBF[6]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,6,rwgtVars[ivar]);
                 if(PassSelVBF[7]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,7,rwgtVars[ivar]);                  
                 if(PassSelVBF[8]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,8,rwgtVars[ivar]);
                 if(PassSelVBF[9]) mon.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",itags_full,9,rwgtVars[ivar]);
              }
           }
      }

      //##############################################
      //########     PLOTS FOR OPTIMIZATION   ########
      //##############################################

      if(passZmass && passLooseKinematics && passBveto && passZpt && pass3dLeptonVeto){
         for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
            if(zvv.pt()>optim_Cuts1_met[index] && mindphijmet>optim_Cuts1_mindphi[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
            mon.fillHisto("optim_eventflow1"          ,tags_full,    index, weight);
         }

         for(unsigned int index=0;index<optim_Cuts2_met.size();index++){
            if(redMet>optim_Cuts2_met[index] && mindphijmet>optim_Cuts2_mindphi[index] && mt>optim_Cuts2_mtmin[index] && mt<optim_Cuts2_mtmax[index])
            mon.fillHisto("optim_eventflow2"          ,tags_full,    index, weight);
         }

	 if(VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && zvv.pt()>=50){
            for(unsigned int index=0;index<optim_Cuts3_pt.size();index++){
               if(phys.jets.size()<2 || phys.jets [0].pt()<optim_Cuts3_pt[index] || phys.jets [1].pt()<optim_Cuts3_pt[index])continue;
               if(VBFdEta>optim_Cuts3_deta[index] && VBFSyst.M()>optim_Cuts3_imass[index])
               mon.fillHisto("optim_eventflow3"          ,tags_full,    index, weight);
            }
         }
      }


      //##############################################
      //########           EVENT LOGGING      ########
      //##############################################

      if((passZmass && passLooseKinematics && passBveto && passZpt && pass3dLeptonVeto) ){
          if(outTxtFile && ( ((PassSelVBF[0] || PassSelVBF[4]) && string(tag_subcat.Data())=="vbf") || metTypeValues["assocFwdCMet"].pt()>250 ) ){
              fprintf(outTxtFile, "<b>%s event</b> @ %s <br/>\n", tag_subcat.Data(), url.Data());
              fprintf(outTxtFile, "%%$Run=%i$%% %%$Lumi=%i$%% %%$Event=%i$%% <br/>\n",  ev.run,  ev.lumi, ev.event);

              fprintf(outTxtFile, "%%$VBF  MET SELECTION: (>50)=%i (>55)=%i (>60)=%i (>65)=%i $%% <br/>\n", PassSelVBF[0], PassSelVBF[1], PassSelVBF[2], PassSelVBF[3] );
              fprintf(outTxtFile, "%%$VBF RMET SELECTION: (>50)=%i (>55)=%i (>60)=%i (>65)=%i $%% <br/>\n", PassSelVBF[4], PassSelVBF[5], PassSelVBF[6], PassSelVBF[7] );

              fprintf(outTxtFile, "<i>Leptons</i> <br/>\n");
              for(size_t ilep=0; ilep<2; ilep++){
                 fprintf(outTxtFile, "%%$l_{%i} = %i $%% ", (int)ilep+1, phys.leptons[ilep].id);
                 fprintf(outTxtFile, "%%$p_{T}=%f $%% ", phys.leptons[ilep].pt());
                 fprintf(outTxtFile, "%%$\\eta=%f $%% ", phys.leptons[ilep].eta());
                 fprintf(outTxtFile, "%%$\\phi=%f $%% ", phys.leptons[ilep].phi());
                 fprintf(outTxtFile, "%%$I_{neu}=%f $%%", phys.leptons[ilep].nhIso);
                 fprintf(outTxtFile, "%%$I_{ch}=%f $%%", phys.leptons[ilep].chIso);
                 fprintf(outTxtFile, "%%$I_{pho}=%f $%%", phys.leptons[ilep].gIso);
                 fprintf(outTxtFile, "<br/>\n");
              }

              fprintf(outTxtFile, "<i>Dilepton</i> <br/>\n");
              fprintf(outTxtFile, "%%$p_{T}^{ll}=%f $%% ", zll.pt());
              fprintf(outTxtFile, "%%$\\eta^{ll}=%f $%% ", zll.eta());
              fprintf(outTxtFile, "%%$\\phi^{ll}=%f $%% ", zll.phi());
              fprintf(outTxtFile, "%%$m^{ll}=%f $%% ", zll.mass());
              fprintf(outTxtFile, "%%$\\Delta R(l,l)=%f $%% ", drll);
              fprintf(outTxtFile, "%%$\\Delta phi(l,l)=%f $%% ", dphill);
              fprintf(outTxtFile, "<br/>\n");
        
              fprintf(outTxtFile, "<i>Missing transverse energy</i> <br/>\n");
              fprintf(outTxtFile, "%%$E_{T}^{miss}=%f $%% %%$\\phi=%f$%% <br/>\n ", met, zvv.phi());
              fprintf(outTxtFile, "%%$red-E_{T}^{miss}=%f $%% %%$l=%f$%%  %%$t=%f$%% <br/>\n ", redMet, redMetL, redMetT);
        
              fprintf(outTxtFile, "<i>Transverse mass</i> <br/>\n");
              fprintf(outTxtFile, "%%$\\sum M_{T}(l, E_{T}^{miss})=%f $%% %%$M_{T}(Z, E_{T}^{miss})=%f $%% <br/>\n ", mtl1+mtl2, mt);
        
              fprintf(outTxtFile, "<i>Jet activity</i> <br/>\n");
              fprintf(outTxtFile, "%%$N^{p_{T}>15}_{jet}=%i $%% %%$\\rho=%f$%% <br/>\n ", ev.jn, ev.rho);
        
              fprintf(outTxtFile, "------\n");
          }
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
  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





