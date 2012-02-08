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
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
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
 
using namespace std;
int main(int argc, char* argv[])
{
  SelectionMonitor controlHistos; //plot storage

  //start computers
  EventCategory eventClassifComp(true);

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
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);




  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");
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

  //pileup weighting
  edm::LumiReWeighting LumiWeights(runProcess.getParameter<std::string>("mcpileup"), runProcess.getParameter<std::string>("datapileup"), "pileup","pileup");
  //  std::string puWeightFile = runProcess.getParameter<std::string>("puWeightFile");
  //  if(puWeightFile.size()==0)  LumiWeights.weight3D_init();
  //  else                        LumiWeights.weight3D_init(puWeightFile);
  reweight::PoissonMeanShifter PShiftUp(+0.6);
  reweight::PoissonMeanShifter PShiftDown(-0.6);

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





  //isVBF sample ?
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isGG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isVBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  if(isGG){
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
  }
  else if(isVBF){
     size_t VBFStringpos =  string(url.Data()).find("VBF");
     string StringMass = string(url.Data()).substr(VBFStringpos+6,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
     VBFString = string(url.Data()).substr(VBFStringpos);
  }
  printf("isGG = %i     isVBF=%i  HiggsMass=%i\n",(int)isGG, (int)isVBF,(int)HiggsMass);

  //book the control histograms
  TH1F *h=new TH1F ("eventflow", ";Step;Events", 8,0,8);
  h->GetXaxis()->SetBinLabel(2,"Preselected");
  h->GetXaxis()->SetBinLabel(3,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(4,"b-veto");
  h->GetXaxis()->SetBinLabel(5,"Z_{pT}>25");
  h->GetXaxis()->SetBinLabel(6,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(7,"red-E_{T}^{miss}>medium");
  h->GetXaxis()->SetBinLabel(8,"red-E_{T}^{miss}>tight");
  controlHistos.addHistogram( h );

  h=new TH1F ("eventCategory", ";Event Category;Events", 4,0,4);
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
  h->GetXaxis()->SetBinLabel(4,"VBF");
  controlHistos.addHistogram( h );

  //vertex
  controlHistos.addHistogram( new TH1F("nvtx",";Vertices;Events",25,0,25) );
  controlHistos.addHistogram( new TH1F("nvtxPlus",";Vertices;Events",25,0,25) );
  controlHistos.addHistogram( new TH1F("nvtxMinus",";Vertices;Events",25,0,25) );

  //dilepton control 
  controlHistos.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 100,-5,5) );
  controlHistos.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 100,0,400) );
  controlHistos.addHistogram( new TH1F("zrank",";Z hardness;Events",5,0,5) );
  h=(TH1F *) new TH1F( "zmass", ";M^{ll};Events", 100,91-31,91+31);
  controlHistos.addHistogram( h );
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
  controlHistos.addHistogram( h );

  controlHistos.addHistogram( new TH1F( "dphill", ";#Delta#phi(l^{(1)},l^{(2)});Events",100,-3.2,3.2) );
  controlHistos.addHistogram( new TH1F( "drll",   ";#DeltaR(l^{(1)},l^{(2)}) / (0.1 rad);Events",20,0,6) );
  controlHistos.addHistogram( new TH1F( "dphizleadl", ";#Delta#phi(l^{(1)},Z);Events / (0.1 rad)",15,0,1.5) );
  controlHistos.addHistogram( new TH1F( "mindrlz", ";min #DeltaR(l,Z);Events",100,0,6) );
  controlHistos.addHistogram( new TH1F( "maxdrlz", ";max #DeltaR(l,Z);Events",100,0,6) );
  controlHistos.addHistogram( new TH1F( "ptsum", ";#Sigma p_{T}(l^{(i)});Events", 100,0,1000));
  controlHistos.addHistogram( new TH1F( "mtsum", ";#Sigma M_{T}(l^{(i)},E_{T}^{miss});Events / 20 GeV/c^{2}", 50,0,1000) );
  controlHistos.addHistogram( new TH1F( "mtl1", ";M_{T}(l^{(1)},E_{T}^{miss});Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F( "mtl2", ";M_{T}(l^{(2)},E_{T}^{miss});Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F( "mt"  , ";M_{T},E_{T}^{miss});Events", 100,0,1000) );


  //object multiplicity
  h=new TH1F("nleptons",";Leptons;Events",3,0,3);
  h->GetXaxis()->SetBinLabel(1,"=2 leptons");
  h->GetXaxis()->SetBinLabel(2,"=3 leptons");
  h->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
  controlHistos.addHistogram( h );
  
  controlHistos.addHistogram(new TH1F("njets",";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  controlHistos.addHistogram( new TH1F("njets3leptons",";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  controlHistos.addHistogram( new TH2F ("njetsvspu", ";Pileup events;Jets;Events", 25,0,25,5,0,5) );  
  controlHistos.addHistogram(  new TH2F ("njetsincvspu", ";Pileup events;Jets;Events", 25,0,25,5,0,5) );   
  for(size_t ibin=1; ibin<=5; ibin++){
      TString label("");
      if(ibin==5) label +="#geq";
      else        label +="=";
      label += (ibin-1);
      controlHistos.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
      controlHistos.getHisto("njets3leptons")->GetXaxis()->SetBinLabel(ibin,label);
      controlHistos.getHisto("njetsvspu")->GetXaxis()->SetBinLabel(ibin,label);
      controlHistos.getHisto("njetsincvspu")->GetXaxis()->SetBinLabel(ibin,label);
  }

  h = new TH1F ("nbtags", ";b-tag multiplicity; Events", 3,0,3);  
  h->GetXaxis()->SetBinLabel(1,"0 b-tags");
  h->GetXaxis()->SetBinLabel(2,"1 b-tags");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
  controlHistos.addHistogram( h );
  controlHistos.addHistogram( new TH1F ("nbtags3leptons", ";b-tag multiplicity; Events", 3,0,3) );

  //used to optimize the b-veto
  h=new TH1F ("npassbveto", ";Pass b-tag veto; Events", 3,0,3);
  h->GetXaxis()->SetBinLabel(1,"TCHEL");
  h->GetXaxis()->SetBinLabel(2,"TCHE>2");
  h->GetXaxis()->SetBinLabel(3,"CSVL");
  controlHistos.addHistogram( h );

  //used for GenLevel
  controlHistos.addHistogram( new TH1F( "genHiggsPt"  , ";gen Higgs p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genHiggsMass", ";gen Higgs Mass ;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genHiggsMassRaw", ";gen Higgs Mass Before VBF reweighting;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genzllPt"  , ";gen Zll p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzvvPt"  , ";gen Zvv p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzwinHiggsPt"  , ";gen Higgs p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzwinHiggsMass", ";gen Higgs Mass ;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genzwinHiggsMassRaw", ";gen Higgs Mass Before VBF reweighting;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genzwinzllPt"  , ";gen Zll p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzwinzvvPt"  , ";gen Zvv p_{T};Events",100,0,200) );

  controlHistos.addHistogram( new TH1F( "CMzllP"  , ";CM Zll p};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "CMzvvP"  , ";CM Zvv p};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "CMDeltazP"  , ";CM Zll p - Zvv p;Events",200,-200,200) );
  controlHistos.addHistogram( new TH1F( "CMiMass"  , ";CM Zll p - Zvv p;Events",100,0,600) );

  controlHistos.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",100,0,3.4) );
  controlHistos.addHistogram( new TH1F( "minmtjmet", ";min M_{T}(jet,E_{T}^{miss}) [GeV/c^{2}];Events",100,0,250) );
  controlHistos.addHistogram( new TH1F( "mindrjz", ";min #DeltaR(jet,Z);Events",100,0,6) );
  controlHistos.addHistogram( new TH1F( "minmjz", ";min M(jet,Z) [GeV/c^{2}];Events",100,0,500) );

  //met control
  std::map<TString,TString> metTypes;
//  metTypes["projMet"]             = "proj-E_{T}^{miss}";
//  metTypes["minProjMet"]          = "min-proj{E_{T}^{miss},assoc E_{T}^{miss} (charged)}";
//  metTypes["redMet"]              = "red-E_{T}^{miss}";
//  metTypes["redMetD0"]            = "red-E_{T}^{miss}(D0)";
//  metTypes["centralMet"]          = "central-E_{T}^{miss}";
//  metTypes["minCentralMet"]       = "min{E_{T}^{miss},central-E_{T}^{miss}}";
//  metTypes["assocOtherVertexMet"] = "assoc-E_{T}^{miss} (other vtx)";
//  metTypes["cleanMet"]            = "clean-E_{T}^{miss}";
//  metTypes["minCleanMet"]         = "min{E_{T}^{miss},clean-E_{T}^{miss}}";
//  metTypes["superMinMet"]         = "min{E_{T}^{miss},assoc-E_{T}^{miss},clean-E_{T}^{miss},central-E_{T}^{miss}}";
//  metTypes["redminAssocMet"]         = "red{min{E_{T}^miss,assoc-E_{T}^{miss}}, unclustered E_{T}^{miss})";
//  metTypes["assocMet5"]            = "assoc-E_{T}^{miss} (pT>5)";
//  metTypes["assocMet10"]           = "assoc-E_{T}^{miss} (pT>10)";
//  metTypes["minAssocFwdMet"]         = "min{E_{T}^miss,assoc-E_{T}^{miss} + Fwd)";
//  metTypes["assocFwdMet"]            = "assoc-E_{T}^{miss} + Fwd";
//  metTypes["assocFwdMet5"]            = "assoc-E_{T}^{miss} + Fwd (pT>5)";
//  metTypes["assocFwdMet10"]           = "assoc-E_{T}^{miss} + Fwd (pT>10)";
//  metTypes["redClusteredAssocPFMet"]         = "red(E_{T}^{miss},clustered-E_{T}^{miss},assoc E_{T}^{miss})";



  metTypes["met"]                 = "E_{T}^{miss}";
  //metTypes["centralMet"]          = "central-E_{T}^{miss}";
  metTypes["assocChargedMet"]     = "assoc-E_{T}^{miss}(charged)";
  metTypes["assocMet"]            = "assoc-E_{T}^{miss}";
  // metTypes["assocCMet"]           = "assocC-E_{T}^{miss}";
  //  metTypes["assocFwdCMet"]        = "assoc + clust Fwd E_{T}^{miss}";
  //  metTypes["assocFwdMet"]         = "assoc + Fwd E_{T}^{miss}";
  // metTypes["assocFwd2Met"]        = "assoc + Fwd2 E_{T}^{miss}";
  metTypes["clusteredMet"]        = "clustered-E_{T}^{miss}";
  // metTypes["minAssocChargedMet"]  = "min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
  // metTypes["minAssocMet"]         = "min(E_{T}^{miss},assoc-E_{T}^{miss})";
  // metTypes["minClusteredMet"]     = "min(E_{T}^{miss},clustered-E_{T}^{miss})";
 //  metTypes["minTAssocMet"]       = "min(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})";
//   metTypes["minTClusteredMet"]   = "min(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})";
//   metTypes["minAClusteredMet"]    = "min(assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
//   metTypes["min3Met"]             = "min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
//   metTypes["min4Met"]             = "min(E_{T}^{miss},assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redMet"]              = "red(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redMetD0"]            = "red-E_{T}^{miss}(D0)";
//   metTypes["redAssocChargedMet"]  = "red(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
//   metTypes["redAssocMet"]         = "red(E_{T}^{miss},assoc-E_{T}^{miss})";
//   metTypes["redClusteredMet"]     = "red(E_{T}^{miss},clustered-E_{T}^{miss})";
//   metTypes["redTAssocMet"]        = "red(assoc-E_{T}^{miss}(charged),assoc-E_{T}^{miss})";
//   metTypes["redTClusteredMet"]    = "red(assoc-E_{T}^{miss}(charged),clustered-E_{T}^{miss})";
//   metTypes["redAClusteredMet"]    = "red(assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
//   metTypes["red3Met"]             = "red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
//   metTypes["redminAssocMet"]      = "red(min(E_{T}^{miss},assoc-E_{T}^{miss}),clustered E_{T}^{miss})";
//   metTypes["mincentralAssocMet"]         = "min(cental-E_{T}^{miss},assoc-E_{T}^{miss})";


  std::map<TString,LorentzVector> metTypeValues;
  for(std::map<TString,TString>::iterator it = metTypes.begin(); it!= metTypes.end(); it++){
      metTypeValues[it->first]=LorentzVector(0,0,0,0);
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first, ";"+it->second+";Events", 100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("metinc_") + it->first, ";"+it->second+";Events", 100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("metL_") + it->first, ";"+it->second+";Events", 100,-250,250) );
      controlHistos.addHistogram( new TH1F( TString("metT_") + it->first, ";"+it->second+";Events", 100,-250,250) );
      controlHistos.addHistogram( new TH1F( TString("metoverqt_") + it->first, ";"+it->second+";Events", 100,0,5) );
      controlHistos.addHistogram( new TH1F( TString("metoverqtL_") + it->first, ";"+it->second+";Events", 100,-5,5) );
      controlHistos.addHistogram( new TH1F( TString("metoverqtT_") + it->first, ";"+it->second+";Events", 100,-5,5) );

      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"vspu", ";Pileup events;"+it->second+";Events", 25,0,25,200,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"phimetphijet", ";#phi "+it->second+";#phi jet", 10,0,3.15,10,0,3.15) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first+"mindphijmet", ";min #Delta#phi(jet,"+it->second+");Events",10,0,3.15) );
//      controlHistos.addHistogram( new TH1F( TString("metL_") + it->first, ";Long: "+it->second+";Events", 160,-300,500) );
//      controlHistos.addHistogram( new TH2F( TString("metL_") + it->first+"vspu", ";Pileup events;Long: "+it->second+";Events", 25,0,25,160,-300,500) );  
//      controlHistos.addHistogram( new TH1F( TString("metT_") + it->first, ";Trans: "+it->second+";Events", 160,-300,500) );
//      controlHistos.addHistogram( new TH2F( TString("metT_") + it->first+"vspu", ";Pileup events;Trans :"+it->second+";Events", 25,0,25,160,-300,500) );
//      controlHistos.addHistogram( new TH2F( TString("metLT_") + it->first, ";Long: " +it->second+";Trans: " + it->second, 160,-300,500,160,-300,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first + "zpt", ";" +it->second+"; z p_{T}", 40,0,200,40,0,200) );
//      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "minzpt", ";min(z p_{T}, "  +it->second+");Events", 100,0,500) );
//      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "geq080zpt", ";"+it->second+">0.8 z p_{T};Events", 100,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"geq080zptvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "geq060zpt", ";"+it->second+">0.6 z p_{T};Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"geq060zptvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
//      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "geq040zpt", ";"+it->second+">0.4 z p_{T};Events", 100,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"geq040zptvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );

//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first + "pfmet", ";" +it->second+"; pfmet", 40,0,200,40,0,200) );
//      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "leq120pfmet", ";"+it->second+"<1.2 pfmet;Events", 100,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"leq120pfmetvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
//      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "leq140pfmet", ";"+it->second+"<1.4 pfmet;Events", 100,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"leq140pfmetvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
//      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "leq160pfmet", ";"+it->second+"<1.6 z pfmet;Events", 100,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"leq160pfmetvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
    }
  controlHistos.addHistogram( new TH2F ("itpuvsootpu", ";In-Time Pileup; Out-of-time Pileup;Events", 30,0,30,30,0,30) );
  controlHistos.addHistogram( new TH2F ("redMetcomps", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 50, -251.,249,50, -251.,249.) );
  controlHistos.addHistogram( new TH1F( "redMetL", ";red-E_{T}^{miss,#parallel};Events", 100,-250,250) );
  controlHistos.addHistogram( new TH1F( "redMetT", ";red-E_{T}^{miss,#perp};Events", 100,-250,250) );
  controlHistos.addHistogram( new TH2F ("metvstkmet", ";E_{T}^{miss};assoc-E_{T}^{miss}(charged)", 50,0,500,50,0,500) );  
  controlHistos.addHistogram( new TH2F ("metvsassoc", ";E_{T}^{miss};assoc-E_{T}^{miss}", 50,0,500,50,0,500) );  
  controlHistos.addHistogram( new TH2F ("metvsclustered", ";E_{T}^{miss};clustered-E_{T}^{miss}", 50,0,500,50,0,500) );    
  controlHistos.addHistogram( new TH2F ("metvscentralMet", ";E_{T}^{miss};central-E_{T}^{miss}", 50,0,500,50,0,500) );  
  controlHistos.addHistogram( new TH2F ("centralMetvsassocMet", ";central-E_{T}^{miss};assoc-E_{T}^{miss}", 50,0,500,50,0,500) );



  
  controlHistos.addHistogram( new TH1F("sumEt",                    ";#Sigma E_{T} [GeV];Events", 100,0,2000) );
  controlHistos.addHistogram( new TH1F("chSumEt",                    ";#Sigma E_{T}^{charged} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("neutSumEt",                    ";#Sigma E_{T}^{neutral} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("primVertexSumEt",          ";Primary Vertex : #Sigma E_{T} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("otherVertexSumEt",         ";Other Vertices : #Sigma_{vtx} #Sigma E_{T} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("chSumEtFrac",              ";#Sigma E_{T}^{charged}/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("neutSumEtFrac",            ";#Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );

  TString ootConds[]={"LowOOTpu","MediumOOTpu","HighOOTpu","VeryHighOOTpu"};
  for(size_t i=0; i<sizeof(ootConds)/sizeof(TString); i++){
      controlHistos.addHistogram( new TH1F("sumEt"+ootConds[i],           ";#Sigma E_{T} [GeV];Events", 100,0,2000) );
      controlHistos.addHistogram( new TH1F("neutSumEtFrac"+ootConds[i],   ";#Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );
  }

  controlHistos.addHistogram( new TH1F("centralSumEtFrac",         ";#Sigma E_{T}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("centralChSumEtFrac",       ";#Sigma E_{T}^{charged}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("centralNeutSumEtFrac",     ";#Sigma E_{T}^{neutral}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("chPrimVertexSumEtFrac",    ";#Sigma E_{T}^{charged}(PV)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("neutPrimVertexSumEtFrac",  ";#Sigma E_{T}^{neutral}(PV)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("chOtherVertexSumEtFrac",   ";#Sigma_{oth.vtx} #Sigma E_{T}^{charged}/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("neutOtherVertexSumEtFrac", ";#Sigma_{oth.vtx} #Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );

  //cut optimization plots
  controlHistos.addHistogram( new TH1F ("cutOptMediumdphill", ";#delta #phi^{l,l};#red-E_{T}^{miss,#parallel};",25,0.,3.2) );
  controlHistos.addHistogram( new TH2F ("cutOptMediumsummtvsredMetL", ";#Sigma M_{T};#red-E_{T}^{miss,#parallel};",50,0,2000,25, -251.,249) );
  
  //cut and count
  controlHistos.addHistogram( new TH1F("btag0proj",   ";Events with 0 b-tags;Events", 1,0,1) );
  controlHistos.addHistogram( new TH1F("btag0projup",   ";Events with 0 b-tags (+);Events", 1,0,1) );
  controlHistos.addHistogram( new TH1F("btag0projdown",   ";Events with 0 b-tags (-);Events", 1,0,1) );
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
    controlHistos.addHistogram( hmet );

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
    controlHistos.addHistogram( hrmet );
    
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
    controlHistos.addHistogram( hvbf );
  }

  //optimization


  //Prepare vectors for cut optimization
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

  controlHistos.addHistogram ( new TH1F ("optim_eventflow1"  , ";cut index;yields" ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) );
  TH1F* Hoptim_cuts1_met     =  new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ;
  TH1F* Hoptim_cuts1_mindphi =  new TH1F ("optim_cut1_mindphi", ";cut index;mindphi",optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ;
  TH1F* Hoptim_cuts1_mtmin   =  new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ;
  TH1F* Hoptim_cuts1_mtmax   =  new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ;
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

  controlHistos.addHistogram ( new TH1F ("optim_eventflow2"  , ";cut index;yields" ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) );
  TH1F* Hoptim_cuts2_met     =  new TH1F ("optim_cut2_met"    , ";cut index;met"    ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ;
  TH1F* Hoptim_cuts2_mindphi =  new TH1F ("optim_cut2_mindphi", ";cut index;mindphi",optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ;
  TH1F* Hoptim_cuts2_mtmin   =  new TH1F ("optim_cut2_mtmin"  , ";cut index;mtmin"  ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ;
  TH1F* Hoptim_cuts2_mtmax   =  new TH1F ("optim_cut2_mtmax"  , ";cut index;mtmax"  ,optim_Cuts2_met.size(),0,optim_Cuts2_met.size()) ;
  for(unsigned int index=0;index<optim_Cuts2_met.size();index++){
    Hoptim_cuts2_met    ->Fill(index, optim_Cuts2_met[index]);
    Hoptim_cuts2_mindphi->Fill(index, optim_Cuts2_mindphi[index]);
    Hoptim_cuts2_mtmin  ->Fill(index, optim_Cuts2_mtmin[index]);
    Hoptim_cuts2_mtmax  ->Fill(index, optim_Cuts2_mtmax[index]);
  }

  //Renormalization
  TH1F* Hcutflow     =  new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ;

  
  //VBF
  h = new TH1F ("VBFNEventsInc", ";Selection cut;Events", 15,0,15);
  h->GetXaxis()->SetBinLabel(1,"All");
  h->GetXaxis()->SetBinLabel(2,"2Jets (30GeV)");
  h->GetXaxis()->SetBinLabel(3,"dEta");
  h->GetXaxis()->SetBinLabel(4,"inv. Mass");
  h->GetXaxis()->SetBinLabel(5,"Central lepton");
  h->GetXaxis()->SetBinLabel(6,"Jet Veto");
  h->GetXaxis()->SetBinLabel(7,"BJet Veto");
  controlHistos.addHistogram( h );
  controlHistos.addHistogram( new TH1F ("VBFdEtaInc", ";#Delta#eta", 100, 0,10) );
  controlHistos.addHistogram( new TH1F ("VBFiMassInc",";Inv. Mass", 100, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFcenLeptonVetoInc",";Central Lepton Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFcen30JetVetoInc",";Central 30 Jet Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFNBJets30Inc", ";N BJets (pT>30);Events", 10,0,10) );
  controlHistos.addHistogram( new TH1F ("VBFdEtaNM1C", ";#Delta#eta", 100, 0,10) );
  controlHistos.addHistogram( new TH1F ("VBFiMassNM1C",";Inv. Mass", 100, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFcenLeptonVetoNM1C",";Central Lepton Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFcen30JetVetoNM1C",";Central 30 Jet Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFNBJets30NM1C", ";N BJets (pT>30);Events", 10,0,10) );
  controlHistos.addHistogram( new TH2F ("VBFdEtaiMassNM1C", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  controlHistos.addHistogram( new TH2F ("VBFdEtaiMassNM1C2", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFdEta3rdlepton", ";#Delta#eta", 100, 0,10) );
  controlHistos.addHistogram( new TH1F ("VBFiMass3rdlepton",";Inv. Mass", 100, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFcen30JetVeto3rdlepton",";Central 30 Jet Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFNBJets303rdlepton", ";N BJets (pT>30);Events", 10,0,10) );

  //replicate monitor for interesting categories
  TString cats[]={"ee","mumu","all"};
  TString* subCats=new TString[eventClassifComp.GetLabelSize()];
  for(int i=0;i<eventClassifComp.GetLabelSize();i++) 
    {
      subCats[i] = eventClassifComp.GetRawLabel(i);
    }
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(int isubcat=0;isubcat<eventClassifComp.GetLabelSize(); isubcat++)
	{
	  TString ctf=cats[icat]+subCats[isubcat];
	  controlHistos.initMonitorForStep(ctf);
	}
    }
  
  cout << "We have the following categories" << endl;
  for(SelectionMonitor::StepMonitor_t::iterator it=controlHistos.getAllMonitors().begin(); 
      it != controlHistos.getAllMonitors().end(); 
      it++)
    {
      cout << it->first << endl;
    }


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

  //check PU normalized entries 
  evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  TEventList *elist = (TEventList*)gDirectory->Get("elist");
  const Int_t normEntries = (elist==0 ? 0 : elist->GetN()); 
  if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;
  
  double VBFWEIGHTINTEGRAL = 0;
    
  //run the analysis
  for( int iev=evStart; iev<evEnd; iev++){
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();

        PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      
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
        //weight = LumiWeights.weight3D( ev.ngenOOTpum1, ev.ngenITpu, ev.ngenOOTpu );        
	weight = LumiWeights.weight( ev.ngenITpu );
        TotalWeight_plus = PShiftUp.ShiftWeight( ev.ngenITpu );
        TotalWeight_minus = PShiftDown.ShiftWeight( ev.ngenITpu );
        if(isVBF)         weight *= weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );         
        if(isGG)  	  weight *= ev.hptWeights[0];
      }

      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);


  
      //
      //event categories
      //
      std::vector<TString> evcats;
      if(ev.cat==EMU)  continue;//evcats.push_back("emu");
      if(ev.cat==MUMU) evcats.push_back("mumu");
      if(ev.cat==EE)   evcats.push_back("ee");

      if(isMC && mctruthmode==1 && ev.mccat!=DY_EE && ev.mccat!=DY_MUMU)  continue;
      if(isMC && mctruthmode==2 && ev.mccat!=DY_TAUTAU) continue;
     
      int eventCategory = eventClassifComp.Get(phys);
      TString subcat    = eventClassifComp.GetLabel(eventCategory);

      bool isGammaEvent(false);
      if(gammaEvHandler)
	{
	  isGammaEvent=gammaEvHandler->isGood(phys);
	  if(mctruthmode==22 && !isGammaEvent) continue;
	  evcats.push_back("mumu");
	  evcats.push_back("ee");
	}

      //MC truth
      LorentzVector genzll(0,0,0,0), genzvv(0,0,0,0), genhiggs(0,0,0,0);
      if(isMC && phys.genleptons.size()){
         genzll=phys.genleptons[0]+phys.genleptons[1];
         genzvv=phys.genmet[0];
         genhiggs = phys.genhiggs[0];
      }

      LorentzVector zll = isGammaEvent ? gammaEvHandler->massiveGamma("ll") : phys.leptons[0]+phys.leptons[1];
      LorentzVector zvv  = phys.met[0];

      //count jets and b-tags
      int njets(0),njetsinc(0);
      int nbtags(0), nbtags_tchel(0),   nbtags_tche2(0),  nbtags_csvl(0);
      LorentzVectorCollection jetsP4;
      LorentzVector fwdClusteredMetP4(0,0,0,0);
      int nheavyjets(0), nlightsjets(0);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++){
          if(fabs(phys.jets[ijet].eta())>2.4)fwdClusteredMetP4-=phys.jets[ijet];
	  
	  jetsP4.push_back( phys.jets[ijet] );
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
      LorentzVector metP4=phys.met[0];
      LorentzVector assocChargedMetP4=phys.met[5];
      LorentzVector assocMetP4=phys.met[1];
      LorentzVector centralMetP4=phys.met[3];
      LorentzVector cleanMetP4=phys.met[4];
      LorentzVector assocOtherVertexMetP4=phys.met[7]; //? not produced any longer
      LorentzVector assocFwdMetP4=phys.met[6];
      LorentzVector assocMet5P4=phys.met[9];        //?
      LorentzVector assocFwdMet5P4=phys.met[10];    //?
      LorentzVector assocMet10P4=phys.met[11];      //?
      LorentzVector assocFwdMet10P4=phys.met[12];   //?
      LorentzVector clusteredMetP4 = -1*zll;  for(unsigned int i=0;i<jetsP4.size();i++){clusteredMetP4 -= jetsP4[i];}
      LorentzVector assocFwdCMetP4 = assocMetP4 + fwdClusteredMetP4;
      LorentzVector assocCMetP4=phys.met[13];       //?
      LorentzVector assocFwd2MetP4=phys.met[14];    //?
      if(isGammaEvent){
          assocChargedMetP4 -= zll;
          if(!phys.gammas[0].isConv) assocMetP4 -= zll;
      }

      //z+met kinematics
      Float_t dphill     = isGammaEvent ? 0 : deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      Float_t detall     = isGammaEvent ? 0 : phys.leptons[0].eta()-phys.leptons[1].eta();
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
      Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
      Float_t mt         = METUtils::transverseMass(zll,zvv,true);
      Float_t dphizleadl = isGammaEvent ? 0 : ( ptl1>ptl2 ? deltaPhi(phys.leptons[0].phi(),zll.phi()) : deltaPhi(phys.leptons[1].phi(),zll.phi()) );

      //redmet
      METUtils::stRedMET redMetInfo;      
      LorentzVector nullP4   = LorentzVector(0,0,0,0);
      LorentzVector lep1     = isGammaEvent ? zll    : phys.leptons[0];
      LorentzVector lep2     = isGammaEvent ? nullP4 : phys.leptons[1];
      LorentzVector rTMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , zvv                , isGammaEvent);
      LorentzVector rAMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , zvv                , isGammaEvent);
      LorentzVector rCMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4     , zvv                , isGammaEvent);
      LorentzVector rTAMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , assocMetP4         , isGammaEvent);
      LorentzVector rTCMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , clusteredMetP4     , isGammaEvent);
      LorentzVector rACMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4     , isGammaEvent);
      LorentzVector r3MetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4, zvv, isGammaEvent);
      LorentzVector rmAMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, min(zvv,assocMetP4), clusteredMetP4, zvv, isGammaEvent);
      LorentzVector redMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetsP4             , zvv                , isGammaEvent, &redMetInfo);
      double redMet = redMetP4.pt();   double redMetL = redMetInfo.redMET_l; double redMetT = redMetInfo.redMET_t;
      LorentzVector redMetD0P4 = METUtils::redMET(METUtils::D0, lep1, 0, lep2, 0, jetsP4             , zvv                , isGammaEvent); 

      //projected met
      Float_t projMet              =  isGammaEvent ? 0 : METUtils::projectedMET(phys.leptons[0], phys.leptons[1], zvv).pt();
      Float_t centralMet          = centralMetP4.pt();
      Float_t assocChargedMet     = assocChargedMetP4.pt();

      //met control
      metTypeValues["met"]                 = zvv;
      metTypeValues["centralMet"]          = centralMetP4;
      metTypeValues["assocChargedMet"]     = assocChargedMetP4;
      metTypeValues["assocMet"]            = assocMetP4;
      metTypeValues["assocCMet"]           = assocCMetP4;
      metTypeValues["assocFwdCMet"]        = assocFwdCMetP4;
      metTypeValues["assocFwdMet"]         = assocFwdMetP4;
      metTypeValues["assocFwd2Met"]        = assocFwd2MetP4;
      metTypeValues["clusteredMet"]        = clusteredMetP4;
      metTypeValues["minAssocChargedMet"]  = min(zvv,assocChargedMetP4);
      metTypeValues["minAssocMet"]         = min(zvv,assocMetP4);
      metTypeValues["minClusteredMet"]     = min(zvv,clusteredMetP4);
      metTypeValues["minTAssocMet"]        = min(assocChargedMetP4,assocMetP4);
      metTypeValues["minTClusteredMet"]    = min(assocChargedMetP4,clusteredMetP4);
      metTypeValues["minAClusteredMet"]    = min(assocMetP4,clusteredMetP4);
      metTypeValues["min3Met"]             = min(zvv, min(assocMetP4,clusteredMetP4));
      metTypeValues["min4Met"]             = min(min(zvv,assocChargedMetP4), min(assocMetP4,clusteredMetP4));      
      metTypeValues["redMet"]              = redMetP4; 
      metTypeValues["redMetD0"]              = redMetD0P4;
      metTypeValues["redAssocChargedMet"]  = rTMetP4;
      metTypeValues["redAssocMet"]         = rAMetP4;
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
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
	{
	  if(phys.jets[ijet].pt()>zpt) zrank++;
	  //double dphijmet=fabs(deltaPhi(zvv.phi(),phys.jets[ijet].phi()));
	  for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++)
	    {
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

      //vbf variables 
      bool isVBF        = false;
      bool Pass2Jet30   = false;
      bool PassdEtaCut  = false;
      bool PassiMCut    = false;
      bool PassBJetVeto = false;
      bool PassJetVeto  = false;
      bool PassLeptonIn = false;

      double  VBFdEta = -1;
      int     VBFCentral30Jets = 0;
      int     VBFCentralLeptons = 0;
      int     VBFNBJets=0;
      LorentzVector VBFSyst;

      if(phys.jets.size()>=2 && phys.jets [0].pt()>30.0 && phys.jets [1].pt()>30.0){
	  VBFSyst =  phys.jets[0] + phys.jets[1];
	  VBFdEta = fabs(   phys.jets[0].eta() -    phys.jets[1].eta()); if(phys.jets[0].eta()* phys.jets[1].eta()>0)VBFdEta*=-1;
	  int VBFCentral30Jets = 0;
	  double MinEta, MaxEta;
	  if(phys.jets[0].eta()<phys.jets[1].eta()){MinEta=phys.jets[0].eta(); MaxEta=phys.jets[1].eta();}else{MinEta=phys.jets[1].eta(); MaxEta=phys.jets[0].eta();}
	  if(isGammaEvent){
	      if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
	  }else{
	      if(zll.eta()>MinEta && zll.eta()<MaxEta) VBFCentralLeptons=2;
	  }
	  for(size_t ijet=2; ijet<phys.jets.size(); ijet++){
	      if(phys.jets[ijet].pt()<30)continue; 
	      if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral30Jets++; 
	      if(phys.jets[ijet].btag1>1.7) VBFNBJets++;
	      //if(phys.jets[ijet].btag2>0.244)VBFNBJets++; 
	  }
	  
	  Pass2Jet30   = true;
	  PassdEtaCut  = (fabs(VBFdEta)>4.0);
	  PassiMCut    = (VBFSyst.M()>500);
	  PassLeptonIn = (VBFCentralLeptons==2);
	  PassJetVeto  = (VBFCentral30Jets==0);
	  PassBJetVeto = (VBFNBJets==0);
	  isVBF        = (Pass2Jet30 && PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn);
      }
     
      //
      // event pre-selection
      //
      bool passLooseKinematics(zpt>20);
      bool passZmass(fabs(zmass-91)<15);
      bool passSideBand( !passZmass && fabs(zmass-91)<30 );

      bool passZpt(zpt>25);

      bool pass3dLeptonVeto(true); for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)pass3dLeptonVeto=false;}
      bool passBveto(nbtags==0);
      bool passMediumRedMet(50);
      bool passTightRedMet(84);//redMet>METUtils::getRedMETCut(eventCategory,METUtils::TIGHTWP));
      
      bool passBaseCuts(passZmass && pass3dLeptonVeto && passBveto && passMediumRedMet);
      std::vector<int> zmassRegionBins;
      if(passZmass)                                                                 zmassRegionBins.push_back(0);
      if(passZmass    && !passBveto)                                                zmassRegionBins.push_back(1);
      if(passZmass    && !passBveto && pass3dLeptonVeto)                            zmassRegionBins.push_back(2);
      if(passZmass    && !passBveto && pass3dLeptonVeto && passMediumRedMet)        zmassRegionBins.push_back(3);
      if(passZmass    && !passBveto && pass3dLeptonVeto && passTightRedMet)         zmassRegionBins.push_back(4);
      if(passSideBand)                                                              zmassRegionBins.push_back(5);
      if(passSideBand && !passBveto)                                                zmassRegionBins.push_back(6);
      if(passSideBand && !passBveto && pass3dLeptonVeto)                            zmassRegionBins.push_back(7);
      if(passSideBand && !passBveto && pass3dLeptonVeto && passMediumRedMet)        zmassRegionBins.push_back(8);
      if(passSideBand && !passBveto && pass3dLeptonVeto && passTightRedMet)         zmassRegionBins.push_back(9);
      if(passZmass    && passBveto)                                                 zmassRegionBins.push_back(10);
      if(passZmass    && passBveto && pass3dLeptonVeto)                             zmassRegionBins.push_back(11);
      if(passZmass    && passBveto && pass3dLeptonVeto && passMediumRedMet)         zmassRegionBins.push_back(12);
      if(passZmass    && passBveto && pass3dLeptonVeto && passTightRedMet)          zmassRegionBins.push_back(13);
      if(passSideBand && passBveto)                                                 zmassRegionBins.push_back(14);
      if(passSideBand && passBveto && pass3dLeptonVeto)                             zmassRegionBins.push_back(15);
      if(passSideBand && passBveto && pass3dLeptonVeto && passMediumRedMet)         zmassRegionBins.push_back(16);
      if(passSideBand && passBveto && pass3dLeptonVeto && passTightRedMet)          zmassRegionBins.push_back(17);


      //
      // compute systematic variations for JET/MET
      //
      LorentzVectorCollection metVars, redMetVars;
      std::vector<LorentzVectorCollection> jetVars;
      std::vector<int> eventCategoryVars;
      std::vector<Float_t>  mtVars;
      if(runSystematics)
	{
	  jet::computeVariation(jetsP4,phys.met[0],jetVars, metVars, &stdPtResol,&stdEtaResol,&stdPhiResol,&jecUnc);
	  for(size_t ivar=0; ivar<3; ivar++)
	    {
	      eventCategoryVars.push_back( eventClassifComp.Get(phys, &(jetVars[ivar])) );
	      
	      METUtils::stRedMET temp_redMetInfo;
	      LorentzVector temp_redMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetVars[ivar], metVars[ivar], isGammaEvent, &temp_redMetInfo);
	      //double temp_redMetL = temp_redMetInfo.redMET_l;
	      redMetVars.push_back(temp_redMetP4);
	      
	      Float_t imt     = METUtils::transverseMass(zll,metVars[ivar],true);
	      mtVars.push_back(imt);
	    }
	}
            
      //      ROOT::Math::Boost cmboost(genhiggs.BoostToCM());
      //      LorentzVector cmzll(cmboost(zll));
      //      LorentzVector cmzvv(cmboost(zvv));
      LorentzVector transverseHiggs(genhiggs.px(),genhiggs.py(), 0,sqrt(pow(genhiggs.mass(),2)+pow(genhiggs.pt(),2)));
      ROOT::Math::Boost cmboost(transverseHiggs.BoostToCM());
      LorentzVector cmzll(cmboost(zll));
      LorentzVector cmzvv(cmboost(zvv));
      
      //fill control histograms
      std::vector<TString> catsToFill;
      catsToFill.push_back("all");
      for(size_t ic=0; ic<evcats.size(); ic++) catsToFill.push_back(evcats[ic]);
      std::vector<TString> subCatsToFill;
      subCatsToFill.push_back("");
      subCatsToFill.push_back(subcat);
      for(size_t ic=0; ic<catsToFill.size(); ic++){
	for(size_t isc=0; isc<subCatsToFill.size(); isc++){
	      TString ctf=catsToFill[ic]+subCatsToFill[isc];  

	      float iweight=weight;
	      if(gammaEvHandler){
		  TString dilCh=catsToFill[ic];
		  if(dilCh=="all") dilCh="ll";
		  iweight *= gammaEvHandler->getWeight(dilCh);
              }	      

              controlHistos.fillHisto("genHiggsPt"      ,ctf,    genhiggs.pt()   ,iweight);
              controlHistos.fillHisto("genHiggsMass"    ,ctf,    genhiggs.mass() ,iweight);
              controlHistos.fillHisto("genzllPt"        ,ctf,    genzll.pt()     ,iweight);
              controlHistos.fillHisto("genzvvPt"        ,ctf,    genzvv.pt()     ,iweight);

              if(passZmass){
		controlHistos.fillHisto("genzwinHiggsPt"      ,ctf,    genhiggs.pt()   ,iweight);
		controlHistos.fillHisto("genzwinHiggsMass"    ,ctf,    genhiggs.mass() ,iweight);
		controlHistos.fillHisto("genzwinzllPt"        ,ctf,    genzll.pt()     ,iweight);
		controlHistos.fillHisto("genzwinzvvPt"        ,ctf,    genzvv.pt()     ,iweight);
              }
 	      
	      //event pre-selection
	      if(!passZmass && !passSideBand && !passLooseKinematics)                            continue;
	      
	      //VBF control
	      if(true                                                                                       )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    0                ,iweight);
	      if(Pass2Jet30                                                                                 )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    1                ,iweight);
	      if(PassdEtaCut                                                                                )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    2                ,iweight);
	      if(PassdEtaCut && PassiMCut                                                                   )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    3                ,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn                                                   )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    4                ,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto                                    )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    5                ,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto && PassBJetVeto                    )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    6                ,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto && PassBJetVeto && pass3dLeptonVeto)controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    7                ,iweight);
	      
	      if(Pass2Jet30                                                              )controlHistos.fillHisto("VBFdEtaInc"          ,ctf,    fabs(VBFdEta)    ,iweight);
	      if(PassdEtaCut                                                             )controlHistos.fillHisto("VBFiMassInc"         ,ctf,    VBFSyst.M()      ,iweight);
	      if(PassdEtaCut && PassiMCut                                                )controlHistos.fillHisto("VBFcenLeptonVetoInc" ,ctf,    VBFCentralLeptons,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn                                )controlHistos.fillHisto("VBFcen30JetVetoInc"  ,ctf,    VBFCentral30Jets ,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto                 )controlHistos.fillHisto("VBFNBJets30Inc"      ,ctf,    VBFNBJets        ,iweight);
	      
	      if(               PassiMCut && PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fillHisto("VBFdEtaNM1C"         ,ctf,    fabs(VBFdEta)    ,iweight);
	      if(PassdEtaCut              && PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fillHisto("VBFiMassNM1C"        ,ctf,    VBFSyst.M()      ,iweight);
	      if(PassdEtaCut && PassiMCut                 && PassJetVeto && PassBJetVeto )controlHistos.fillHisto("VBFcenLeptonVetoNM1C",ctf,    VBFCentralLeptons,iweight);
	      if(Pass2Jet30                                              && PassBJetVeto && !pass3dLeptonVeto){
		  controlHistos.fillHisto("VBFcen30JetVeto3rdlepton"  ,ctf,    VBFCentral30Jets ,iweight);
		  controlHistos.fillHisto("VBFNBJets303rdlepton"      ,ctf,    VBFNBJets        ,iweight);
		  controlHistos.fillHisto("VBFdEta3rdlepton"          ,ctf,    fabs(VBFdEta)    ,iweight);
		  controlHistos.fillHisto("VBFiMass3rdlepton"         ,ctf,    VBFSyst.M()      ,iweight);
              }
	      
	      if(PassdEtaCut && PassiMCut && PassLeptonIn                && PassBJetVeto )controlHistos.fillHisto("VBFcen30JetVetoNM1C" ,ctf,    VBFCentral30Jets ,iweight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto                 )controlHistos.fillHisto("VBFNBJets30NM1C"     ,ctf,    VBFNBJets        ,iweight);
	      if(                            PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fill2DHisto("VBFdEtaiMassNM1C"  ,ctf,    fabs(VBFdEta), VBFSyst.M(), iweight);
	      if(redMet>50 &&                PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fill2DHisto("VBFdEtaiMassNM1C2" ,ctf,    fabs(VBFdEta), VBFSyst.M(), iweight);
	      
	      //inclusive control
	      controlHistos.fillHisto("eventflow",ctf,1,iweight);
	      controlHistos.fillHisto("nvtx",ctf,ev.nvtx,iweight);
	      controlHistos.fillHisto("nvtxPlus",ctf,ev.nvtx,iweight*TotalWeight_plus);
	      controlHistos.fillHisto("nvtxMinus",ctf,ev.nvtx,iweight*TotalWeight_minus);
	      
	      //dilepton control plots
	      controlHistos.fillHisto("zmass",ctf,zmass,iweight);
	      for(std::vector<int>::iterator regIt = zmassRegionBins.begin(); regIt<zmassRegionBins.end(); regIt++) controlHistos.fillHisto("zmassregionCtr",ctf,*regIt,iweight);
	      controlHistos.fillHisto("zeta",ctf,zeta,iweight);
	      controlHistos.fillHisto("zpt",ctf,zpt,iweight);
	      controlHistos.fillHisto("zrank",ctf,zrank,iweight);
	      controlHistos.fill2DHisto("zptvszeta", ctf,zpt,zeta,iweight);
	      controlHistos.fillHisto("dphill",ctf,dphill,iweight);
	      controlHistos.fillHisto("mindrlz",ctf,mindrlz,iweight);
	      controlHistos.fillHisto("maxdrlz",ctf,maxdrlz,iweight);
	      controlHistos.fillHisto("ptsum",ctf,ptsum,iweight);
	      controlHistos.fillHisto("mtl1",ctf,mtl1,iweight);
	      controlHistos.fillHisto("mtl2",ctf,mtl2,iweight);
              controlHistos.fillHisto("mt",ctf,mt,iweight);
	      
	      //Z window
	      if(!passZmass)  continue;
	      controlHistos.fillHisto("eventflow",ctf,2,iweight);
	     
	      //jet control
	      controlHistos.fillHisto("njets",ctf,njets,iweight);
	      controlHistos.fill2DHisto("njetsvspu",ctf,ev.ngenITpu,njets,iweight);
	      controlHistos.fill2DHisto("njetsincvspu",ctf,ev.ngenITpu,njetsinc,iweight);
	      controlHistos.fillHisto("nbtags",ctf, nbtags,iweight);
	      controlHistos.fillHisto("npassbveto",ctf,0, (nbtags_tchel==0)*iweight);
	      controlHistos.fillHisto("npassbveto",ctf,1, (nbtags_tche2==0)*iweight);
	      controlHistos.fillHisto("npassbveto",ctf,2, (nbtags_csvl==0)*iweight);
	      controlHistos.fillHisto("zmassctrl",ctf,passBveto+2*passMediumRedMet,iweight);

	      
	      controlHistos.fillHisto("btag0proj",ctf,0,iweight*p0btags);
	      controlHistos.fillHisto("btag0projup",ctf,0,iweight*min(p0btags+p0btags_err,1.0));
	      controlHistos.fillHisto("btag0projdown",ctf,0,iweight*max(p0btags-p0btags_err,0.0));

	      if(!passBveto) continue;
	      controlHistos.fillHisto("eventflow",ctf,3,iweight);


              if(!passZpt) continue;
              controlHistos.fillHisto("eventflow",ctf,4,iweight);

	      //extra leptons
	      controlHistos.fillHisto("nleptons",ctf,ev.ln,iweight);
	      if(!pass3dLeptonVeto){
		  controlHistos.fillHisto("njets3leptons",ctf,njets,iweight);
		  controlHistos.fillHisto("nbtags3leptons",ctf,nbtags,iweight);
		  continue;
              }
	      controlHistos.fillHisto("eventflow",ctf,5,iweight);
	      

              if(isMC && genhiggs.pt()>0){
		  controlHistos.fillHisto("CMzllP"      ,ctf,    cmzll.pt()   ,iweight);
		  controlHistos.fillHisto("CMzvvP"      ,ctf,    cmzvv.pt()   ,iweight);
		  controlHistos.fillHisto("CMDeltazP"   ,ctf,    cmzll.pt()-cmzvv.pt()   ,iweight);
		  controlHistos.fillHisto("CMiMass"     ,ctf,    METUtils::transverseMass(cmzll,cmzvv,true)   ,iweight);
              }

	      for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++){
                  if(it->second.pt()>50 && metTypeValuesminJetdphi[it->first]<10){
		      controlHistos.fillHisto(TString("met_") + it->first+"mindphijmet",ctf,metTypeValuesminJetdphi[it->first], iweight);
		      // controlHistos.fill2DHisto(TString("met_") + it->first+"phimetphijet", ctf,it->second.phi(),metTypeValuesminJetphi[it->first],iweight);
                  }
		  

		  TVector2 metPt(it->second.px(), it->second.py());
		  TVector2 dileptonPt(zll.px(), zll.py());
		  TVector2 dileptonLongi( dileptonPt.Unit() );
		  TVector2 dileptonPerp( dileptonLongi.Rotate(TMath::Pi()/2) );
		  controlHistos.fillHisto(TString("metinc_") + it->first, ctf, metPt*dileptonLongi, iweight);
		  controlHistos.fillHisto(TString("metL_") + it->first, ctf, metPt*dileptonLongi, iweight);
		  controlHistos.fillHisto(TString("metT_") + it->first, ctf, metPt*dileptonPerp, iweight);
		  controlHistos.fillHisto(TString("metoverqt_") + it->first, ctf, it->second.pt()/zpt, iweight);
		  controlHistos.fillHisto(TString("metoverqtL_") + it->first, ctf, metPt*dileptonLongi/zpt, iweight);
		  controlHistos.fillHisto(TString("metoverqtT_") + it->first, ctf, metPt*dileptonPerp/zpt,iweight);


                  if(mindphijmet<0.3)continue;
		  controlHistos.fillHisto(TString("met_") + it->first, ctf,it->second.pt(),iweight);
		  

		  controlHistos.fill2DHisto(TString("met_") + it->first+"vspu", ctf,ev.ngenITpu,it->second.pt(),iweight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"zpt", ctf,it->second.pt(),zpt,iweight);
                  controlHistos.fillHisto(TString("met_") + it->first+"minzpt", ctf,std::min(it->second.pt(),zll.pt()),iweight);
                  controlHistos.fillHisto(TString("met_") + it->first+"geq080zpt", ctf,it->second.pt()>=0.8*zll.pt() ? it->second.pt() : 0.0,iweight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"geq080zptvspu", ctf,ev.ngenITpu, it->second.pt()>=0.8*zll.pt() ? it->second.pt() : 0.0,iweight);
                  controlHistos.fillHisto(TString("met_") + it->first+"geq060zpt", ctf,it->second.pt()>=0.6*zll.pt() ? it->second.pt() : 0.0,iweight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"geq060zptvspu", ctf,ev.ngenITpu,it->second.pt()>=0.6*zll.pt() ? it->second.pt() : 0.0,iweight);
                  controlHistos.fillHisto(TString("met_") + it->first+"geq040zpt", ctf,it->second.pt()>=0.4*zll.pt() ? it->second.pt() : 0.0,iweight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"geq040zptvspu", ctf,ev.ngenITpu,it->second.pt()>=0.4*zll.pt() ? it->second.pt() : 0.0,iweight);

                  TVector2 zll2DLong  = TVector2(zll.px()/zll.pt(), zll.py()/zll.pt());
                  TVector2 zll2DTrans = zll2DLong.Rotate(TMath::Pi()/2);
                  //double LongMET  = zll2DLong .Px()*it->second.px() + zll2DLong .Py()*it->second.py();
                  //double TransMET = zll2DTrans.Px()*it->second.px() + zll2DTrans.Py()*it->second.py();
		  //controlHistos.fillHisto(TString("metL_") + it->first, ctf,LongMET,iweight);
		  //controlHistos.fill2DHisto(TString("metL_") + it->first+"vspu", ctf,ev.ngenITpu,LongMET,iweight);
		  //controlHistos.fillHisto(TString("metT_") + it->first, ctf,TransMET,iweight);
		  //controlHistos.fill2DHisto(TString("metT_") + it->first+"vspu", ctf,ev.ngenITpu,TransMET,iweight);
		  //controlHistos.fill2DHisto(TString("metLT_") + it->first, ctf,LongMET,TransMET,iweight);
              }

	      controlHistos.fill2DHisto("metvstkmet", ctf,met,assocChargedMet,iweight);
	      controlHistos.fill2DHisto("metvsassoc", ctf,met,assocMetP4.pt(),iweight);
              controlHistos.fill2DHisto("metvsclustered", ctf,met,clusteredMetP4.pt(),iweight);
	      controlHistos.fill2DHisto("metvscentralMet", ctf,met,centralMet,iweight);
	      controlHistos.fillHisto("redMetL", ctf,redMetL,iweight);
	      controlHistos.fillHisto("redMetT", ctf,redMetT,iweight);
	      controlHistos.fillHisto("redMetcomps", ctf,redMetL,redMetT,iweight);	

	      if(sumEt>0){
		  controlHistos.fillHisto("sumEt",                ctf,sumEt,iweight);
		  controlHistos.fillHisto("chSumEt",              ctf,chSumEt,iweight);
		  controlHistos.fillHisto("neutSumEt",            ctf,neutsumEt,iweight);
		  controlHistos.fillHisto("primVertexSumEt",      ctf,ev.primVertexSumEt,iweight);
		  controlHistos.fillHisto("otherVertexSumEt",     ctf,ev.otherVertexSumEt,iweight);

		  if(isMC){
		      controlHistos.fillHisto("sumEt"+ootCond,ctf,sumEt,iweight);
		      controlHistos.fillHisto("neutSumEtFrac"+ootCond,ctf,neutsumEt/sumEt,iweight);
		      controlHistos.fill2DHisto("itpuvsootpu",ctf,ev.ngenITpu,ev.ngenOOTpu,iweight);
		  }
		  controlHistos.fillHisto("chSumEtFrac",          ctf,chSumEt/sumEt,iweight);
		  controlHistos.fillHisto("neutSumEtFrac",        ctf,neutsumEt/sumEt,iweight);
		  controlHistos.fillHisto("centralSumEtFrac",     ctf,sumEtcentral/sumEt,iweight);
		  controlHistos.fillHisto("centralChSumEtFrac",   ctf,chSumEtcentral/sumEt,iweight);
		  controlHistos.fillHisto("centralNeutSumEtFrac", ctf,neutSumEtcentral/sumEt,iweight);
		  controlHistos.fillHisto("chPrimVertexSumEtFrac",          ctf,ev.primVertexChSumEt/sumEt,iweight);
		  controlHistos.fillHisto("neutPrimVertexSumEtFrac",        ctf,ev.primVertexNeutSumEt/sumEt,iweight);
		  controlHistos.fillHisto("chOtherVertexSumEtFrac",          ctf,ev.otherVertexChSumEt/sumEt,iweight);
		  controlHistos.fillHisto("neutOtherVertexSumEtFrac",        ctf,ev.otherVertexNeutSumEt/sumEt,iweight);		 
              }
	      
	      controlHistos.fillHisto("mindphijmet",ctf,mindphijmet,iweight);
	      controlHistos.fillHisto("minmtjmet",ctf,minmtjmet,iweight);
	      controlHistos.fillHisto("mindrjz",ctf,mindrjz,iweight);
	      controlHistos.fillHisto("minmjz",ctf,minmjz,iweight);
	      
	      
	      //sample is selected
	      if(passMediumRedMet){
		  controlHistos.fillHisto("eventflow",ctf,6,iweight);
		  controlHistos.fillHisto("eventCategory",ctf,eventCategory,iweight);
		  controlHistos.fillHisto("cutOptMediumdphill",ctf,fabs(dphill));
		  controlHistos.fill2DHisto("cutOptMediumsummtvsredMetL",ctf,mtsum,redMetL,iweight);
		  
		  if(passTightRedMet)    controlHistos.fillHisto("eventflow",ctf,7,iweight);
              }
	      

	      //booking for optimization
              for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
		if(zvv.pt()>optim_Cuts1_met[index] && mindphijmet>optim_Cuts1_mindphi[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
		  controlHistos.fillHisto("optim_eventflow1"          ,ctf,    index, weight);
              }
	      
              for(unsigned int index=0;index<optim_Cuts2_met.size();index++){
		if(redMet>optim_Cuts2_met[index] && mindphijmet>optim_Cuts2_mindphi[index] && mt>optim_Cuts2_mtmin[index] && mt<optim_Cuts2_mtmax[index])
		  controlHistos.fillHisto("optim_eventflow2"          ,ctf,    index, weight);
              }
	      
	      
	      //
	      // CUT & COUNT ANALYSIS
	      //
	      //final selection (cut and count analysis)
	      if(zpt>25)
		{
		  bool pass130met( zvv.pt()>50 && mindphijmet>0.74 && mt>171 && mt<296);
		  bool pass150met( zvv.pt()>50 && mindphijmet>0.7  && mt>193 && mt<284);
		  bool pass170met( zvv.pt()>50 && mindphijmet>0.66 && mt>185 && mt<276);
		  bool pass200met( zvv.pt()>50 && mindphijmet>0.6  && mt>220 && mt<270);
		  bool pass300met( zvv.pt()>86 && mindphijmet>0.4  && mt>260 && mt<310);
		  bool pass400met( zvv.pt()>118 && mindphijmet>0.2 && mt>340 && mt<440);
		  bool pass500met( zvv.pt()>166 && mindphijmet>0.1 && mt>340 && mt<740); 
		  bool pass600met( zvv.pt()>188 && mindphijmet>0.1 && mt>440 && mt<740);
                  bool passSyst1met( zvv.pt()>50 && mindphijmet>0.05 && mt>150);
                  bool passSyst2met( zvv.pt()>60 && mindphijmet>0.1  && mt>170);

		  if(pass130met) controlHistos.fillHisto("finaleventflowmet",ctf,0,iweight);
		  if(pass150met) controlHistos.fillHisto("finaleventflowmet",ctf,1,iweight);
		  if(pass170met) controlHistos.fillHisto("finaleventflowmet",ctf,2,iweight);
		  if(pass200met) controlHistos.fillHisto("finaleventflowmet",ctf,3,iweight);
		  if(pass300met) controlHistos.fillHisto("finaleventflowmet",ctf,4,iweight);
		  if(pass400met) controlHistos.fillHisto("finaleventflowmet",ctf,5,iweight);
		  if(pass500met) controlHistos.fillHisto("finaleventflowmet",ctf,6,iweight);
		  if(pass600met) controlHistos.fillHisto("finaleventflowmet",ctf,7,iweight);
                  if(passSyst1met) controlHistos.fillHisto("finaleventflowmet",ctf,8,iweight);
                  if(passSyst2met) controlHistos.fillHisto("finaleventflowmet",ctf,9,iweight);
		  
		  bool pass130rmet( rCMetP4.pt()>50  && mindphijmet>0.74 && mt>171 && mt<296);
		  bool pass150rmet( rCMetP4.pt()>50  && mindphijmet>0.7  && mt>193 && mt<284);
		  bool pass170rmet( rCMetP4.pt()>50  && mindphijmet>0.66 && mt>185 && mt<276);
		  bool pass200rmet( rCMetP4.pt()>50  && mindphijmet>0.6  && mt>220 && mt<270);
		  bool pass300rmet( rCMetP4.pt()>84  && mindphijmet>0.2  && mt>260 && mt<310);
		  bool pass400rmet( rCMetP4.pt()>110 && mindphijmet>0.2  && mt>340 && mt<440);
		  bool pass500rmet( rCMetP4.pt()>156 && mindphijmet>0.1  && mt>340 && mt<740); 
		  bool pass600rmet( rCMetP4.pt()>156 && mindphijmet>0.1  && mt>440 && mt<790);
                  bool passSyst1rmet( rCMetP4.pt()>50 && mindphijmet>0.05 && mt>150);
                  bool passSyst2rmet( rCMetP4.pt()>60 && mindphijmet>0.1  && mt>170);
		  if(pass130rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,0,iweight);
		  if(pass150rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,1,iweight);
		  if(pass170rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,2,iweight);
		  if(pass200rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,3,iweight);
		  if(pass300rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,4,iweight);
		  if(pass400rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,5,iweight);
		  if(pass500rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,6,iweight);
		  if(pass600rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,7,iweight);		  
                  if(passSyst1rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,8,iweight);
                  if(passSyst2rmet) controlHistos.fillHisto("finaleventflowrmet",ctf,9,iweight);

		  bool passvbf50met( zvv.pt() > 50);
		  bool passvbf55met( zvv.pt() > 55);
		  bool passvbf60met( zvv.pt() > 60);
		  bool passvbf65met( zvv.pt() > 65);
		  bool passvbf50rmet( rCMetP4.pt() > 50);
		  bool passvbf55rmet( rCMetP4.pt() > 55);
		  bool passvbf60rmet( rCMetP4.pt() > 60);
		  bool passvbf65rmet( rCMetP4.pt() > 65);
                  bool passvbfSyst1met( zvv.pt() > 40);
                  bool passvbfSyst2met( rCMetP4.pt() > 40);

		  if(passvbf50met) controlHistos.fillHisto("finaleventflowvbf",ctf,0,iweight);
		  if(passvbf55met) controlHistos.fillHisto("finaleventflowvbf",ctf,1,iweight);
		  if(passvbf60met) controlHistos.fillHisto("finaleventflowvbf",ctf,2,iweight);
		  if(passvbf65met) controlHistos.fillHisto("finaleventflowvbf",ctf,3,iweight);
		  if(passvbf50rmet) controlHistos.fillHisto("finaleventflowvbf",ctf,4,iweight);
		  if(passvbf55rmet) controlHistos.fillHisto("finaleventflowvbf",ctf,5,iweight);
		  if(passvbf60rmet) controlHistos.fillHisto("finaleventflowvbf",ctf,6,iweight);
		  if(passvbf65rmet) controlHistos.fillHisto("finaleventflowvbf",ctf,7,iweight);		  
                  if(passvbfSyst1met) controlHistos.fillHisto("finaleventflowvbf",ctf,8,iweight);
                  if(passvbfSyst2met) controlHistos.fillHisto("finaleventflowvbf",ctf,9,iweight);


                  if(isc==0 && outTxtFile && (passvbf50met || passvbf50rmet) && string(subcat.Data())=="vbf"){
                      fprintf(outTxtFile, "<b>%s event</b> @ %s <br/>\n", subcat.Data(), url.Data());
                      fprintf(outTxtFile, "%%$Run=%i$%% %%$Lumi=%i$%% %%$Event=%i$%% <br/>\n",  ev.run,  ev.lumi, ev.event);

                       fprintf(outTxtFile, "%%$VBF  MET SELECTION: (>50)=%i (>55)=%i (>60)=%i (>65)=%i $%% <br/>\n", passvbf50met, passvbf55met, passvbf60met, passvbf65met );
                       fprintf(outTxtFile, "%%$VBF RMET SELECTION: (>50)=%i (>55)=%i (>60)=%i (>65)=%i $%% <br/>\n", passvbf50rmet, passvbf55rmet, passvbf60rmet, passvbf65rmet );



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
                








		
		  
		  //systematic variations (computed per jet bin so fill only once) 		  
		  if(runSystematics)
		    {

                      if(isc==0){
			
			//jet energy scale related variations
			TString jetVarNames[]={"jer","jesup","jesdown"};
			for(size_t ivar=0; ivar<3; ivar++)
			  {
			    TString isubcat    = eventClassifComp.GetLabel( eventCategoryVars[ivar] );
			    for(size_t kSubCat=0; kSubCat<2; kSubCat++)
			      {
				if(kSubCat==1) isubcat="";

				TString ictf= catsToFill[ic]+isubcat;
			  
				bool ipass130met( metVars[ivar].pt()>50  && mindphijmet>0.74 && mtVars[ivar]>171 && mtVars[ivar]<296);
				bool ipass150met( metVars[ivar].pt()>50  && mindphijmet>0.7  && mtVars[ivar]>193 && mtVars[ivar]<284);
				bool ipass170met( metVars[ivar].pt()>50  && mindphijmet>0.66 && mtVars[ivar]>185 && mtVars[ivar]<276);
				bool ipass200met( metVars[ivar].pt()>50  && mindphijmet>0.6  && mtVars[ivar]>220 && mtVars[ivar]<270);
				bool ipass300met( metVars[ivar].pt()>86  && mindphijmet>0.4  && mtVars[ivar]>260 && mtVars[ivar]<310);
				bool ipass400met( metVars[ivar].pt()>118 && mindphijmet>0.2  && mtVars[ivar]>340 && mtVars[ivar]<440);
				bool ipass500met( metVars[ivar].pt()>166 && mindphijmet>0.1  && mtVars[ivar]>340 && mtVars[ivar]<740); 
				bool ipass600met( metVars[ivar].pt()>188 && mindphijmet>0.1  && mtVars[ivar]>440 && mtVars[ivar]<740);
                                bool ipassSyst1met(  metVars[ivar].pt()>50 && mindphijmet>0.05 && mtVars[ivar]>150);
                                bool ipassSyst2met(  metVars[ivar].pt()>60 && mindphijmet>0.1  && mtVars[ivar]>170);
				if(ipass130met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,0,iweight);
				if(ipass150met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,1,iweight);
				if(ipass170met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,2,iweight);
				if(ipass200met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,3,iweight);
				if(ipass300met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,4,iweight);
				if(ipass400met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,5,iweight);
				if(ipass500met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,6,iweight);
				if(ipass600met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,7,iweight);
                                if(ipassSyst1met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,8,iweight);
                                if(ipassSyst2met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowmet",ictf,9,iweight);		  
			  
				bool ipass130rmet( redMetVars[ivar].pt()>50  && mindphijmet>0.74 && mtVars[ivar]>171 && mtVars[ivar]<296);
				bool ipass150rmet( redMetVars[ivar].pt()>50  && mindphijmet>0.7  && mtVars[ivar]>193 && mtVars[ivar]<284);
				bool ipass170rmet( redMetVars[ivar].pt()>50  && mindphijmet>0.66 && mtVars[ivar]>185 && mtVars[ivar]<276);
				bool ipass200rmet( redMetVars[ivar].pt()>50  && mindphijmet>0.6  && mtVars[ivar]>220 && mtVars[ivar]<270);
				bool ipass300rmet( redMetVars[ivar].pt()>84  && mindphijmet>0.2  && mtVars[ivar]>260 && mtVars[ivar]<310);
				bool ipass400rmet( redMetVars[ivar].pt()>110 && mindphijmet>0.2  && mtVars[ivar]>340 && mtVars[ivar]<440);
				bool ipass500rmet( redMetVars[ivar].pt()>156 && mindphijmet>0.1  && mtVars[ivar]>340 && mtVars[ivar]<740); 
				bool ipass600rmet( redMetVars[ivar].pt()>156 && mindphijmet>0.1  && mtVars[ivar]>440 && mtVars[ivar]<790);
                                bool ipassSyst1rmet( redMetVars[ivar].pt()>50 && mindphijmet>0.05 && mtVars[ivar]>150);
                                bool ipassSyst2rmet( redMetVars[ivar].pt()>60 && mindphijmet>0.1  && mtVars[ivar]>170);
				if(ipass130rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,0,iweight);
				if(ipass150rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,1,iweight);
				if(ipass170rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,2,iweight);
				if(ipass200rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,3,iweight);
				if(ipass300rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,4,iweight);
				if(ipass400rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,5,iweight);
				if(ipass500rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,6,iweight);
				if(ipass600rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,7,iweight);		  
                                if(ipassSyst1rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,8,iweight);
                                if(ipassSyst2rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowrmet",ictf,9,iweight);
		  			  
				bool ipassvbf50met( metVars[ivar].pt() > 50);
				bool ipassvbf55met( metVars[ivar].pt() > 55);
				bool ipassvbf60met( metVars[ivar].pt() > 60);
				bool ipassvbf65met( metVars[ivar].pt() > 65);
				bool ipassvbf50rmet( redMetVars[ivar].pt() > 50);
				bool ipassvbf55rmet( redMetVars[ivar].pt() > 55);
				bool ipassvbf60rmet( redMetVars[ivar].pt() > 60);
				bool ipassvbf65rmet( redMetVars[ivar].pt() > 65);
                                bool ipassvbfSyst1met( metVars[ivar].pt() > 40);
                                bool ipassvbfSyst2met( redMetVars[ivar].pt() > 40);
				if(ipassvbf50met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,0,iweight);
				if(ipassvbf55met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,1,iweight);
				if(ipassvbf60met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,2,iweight);
				if(ipassvbf65met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,3,iweight);
				if(ipassvbf50rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,4,iweight);
				if(ipassvbf55rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,5,iweight);
				if(ipassvbf60rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,6,iweight);
				if(ipassvbf65rmet) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,7,iweight);		  
                                if(ipassvbfSyst1met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,8,iweight);
                                if(ipassvbfSyst2met) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflowvbf",ictf,9,iweight);
			      }
			  }
		      }
		 
		      //re-weighting variations (Higgs, pileup scenario)
		      TString wgtVarNames[]={"hrenup","hrendown","hfactup","hfactdown","puup","pudown"};
		      Float_t rwgtVars[]={ isGG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]   : iweight ,
					   isGG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor] : iweight ,
					   isGG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]  : iweight ,
					   isGG ? iweight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown]/ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]: iweight ,
					   iweight*TotalWeight_plus,
					   iweight*TotalWeight_minus};
		      if(ev.hptWeights[ZZ2l2nuSummary_t::hKfactor] <0.5)
			cout << phys.genhiggs[0].pt() << " " << isGG 
			     << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor] 
			     << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp]
			     << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown]
			     << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp]
			     << " " << ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown] << endl;
		  
		      for(size_t ivar=0; ivar<sizeof(wgtVarNames)/sizeof(TString); ivar++)
			{
			  //		      TString ictf= catsToFill[ic]+subcat;
			  if(pass130met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,0,rwgtVars[ivar]);
			  if(pass150met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,1,rwgtVars[ivar]);
			  if(pass170met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,2,rwgtVars[ivar]);
			  if(pass200met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,3,rwgtVars[ivar]);
			  if(pass300met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,4,rwgtVars[ivar]);
			  if(pass400met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,5,rwgtVars[ivar]);
			  if(pass500met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,6,rwgtVars[ivar]);
			  if(pass600met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,7,rwgtVars[ivar]);
                          if(passSyst1met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,8,rwgtVars[ivar]);
                          if(passSyst2met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowmet",ctf,9,rwgtVars[ivar]);
		      
			  if(pass130rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,0,rwgtVars[ivar]);
			  if(pass150rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,1,rwgtVars[ivar]);
			  if(pass170rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,2,rwgtVars[ivar]);
			  if(pass200rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,3,rwgtVars[ivar]);
			  if(pass300rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,4,rwgtVars[ivar]);
			  if(pass400rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,5,rwgtVars[ivar]);
			  if(pass500rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,6,rwgtVars[ivar]);
			  if(pass600rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,7,rwgtVars[ivar]);		  
                          if(passSyst1rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,8,rwgtVars[ivar]);
                          if(passSyst2rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowrmet",ctf,9,rwgtVars[ivar]);

			  if(passvbf50met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,0,rwgtVars[ivar]);
			  if(passvbf55met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,1,rwgtVars[ivar]);
			  if(passvbf60met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,2,rwgtVars[ivar]);
			  if(passvbf65met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,3,rwgtVars[ivar]);
			  if(passvbf50rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,4,rwgtVars[ivar]);
			  if(passvbf55rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,5,rwgtVars[ivar]);
			  if(passvbf60rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,6,rwgtVars[ivar]);
			  if(passvbf65rmet) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,7,rwgtVars[ivar]);		  
                          if(passvbfSyst1met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,8,rwgtVars[ivar]);
                          if(passvbfSyst2met) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflowvbf",ctf,9,rwgtVars[ivar]);
			}

		    }
		}
	}
      }
  }

  
  //all done with the events file
  file->Close();
  
  //save control plots to file
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
    Hcutflow            ->Write();
    Hoptim_cuts1_met    ->Write();
    Hoptim_cuts1_mindphi->Write();
    Hoptim_cuts1_mtmin  ->Write();
    Hoptim_cuts1_mtmax  ->Write();
    Hoptim_cuts2_met    ->Write();
    Hoptim_cuts2_mindphi->Write();
    Hoptim_cuts2_mtmin  ->Write();
    Hoptim_cuts2_mtmax  ->Write();
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  std::map<TString, TDirectory *> outDirs;
  outDirs["all"]=baseOutDir->mkdir("all");
  outDirs["ee"]=baseOutDir->mkdir("ee");
  //  outDirs["emu"]=baseOutDir->mkdir("emu");
  outDirs["mumu"]=baseOutDir->mkdir("mumu");
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++){
      TString icat("all");
      if(it->first.BeginsWith("ee")) icat="ee";
      if(it->first.BeginsWith("emu")) continue;//icat="emu";
      if(it->first.BeginsWith("mumu")) icat="mumu";
      outDirs[icat]->cd();
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++){
	  hit->second->Write();
      }
  }
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





