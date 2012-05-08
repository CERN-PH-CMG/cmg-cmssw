

#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"


#include "CMG/JetIDAnalysis/interface/JetTree.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TChain.h"

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>

// *** 
bool pass_level(int id, int level) { return ( id & (1 << level) ) != 0 ; }

// *** MAIN
int main(int argc, char** argv)
{ 

  if(argc<2){ std::cout << "Usage : " << argv[0] << " mycfg.py" << std::endl; exit(0); }
    
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ParameterSet> parameterSet = edm::readConfig(fileName);

  // "Input"
  edm::ParameterSet Input =  parameterSet -> getParameter<edm::ParameterSet>("Input");
  std::vector<std::string> inputFile_   = Input.getParameter<std::vector<std::string> >("inputFile");
  std::string inputTree_   = Input.getParameter<std::string>("inputTree");

  // "Output"
  edm::ParameterSet Output =  parameterSet -> getParameter<edm::ParameterSet>("Output");
  std::string outputRootFilePath_ = Output.getParameter<std::string>("outputRootFilePath");
  std::string outputRootFileName_ = Output.getParameter<std::string>("outputRootFileName");
   
  // "Options"
  edm::ParameterSet Options =  parameterSet -> getParameter<edm::ParameterSet>("Options");
  int entryMIN_            = Options.getParameter<int>("entryMIN");
  int entryMAX_            = Options.getParameter<int>("entryMAX");
  int dataFlag_            = Options.getParameter<int>("dataFlag");
  std::string mcPuFile_    = Options.getParameter<std::string>("mcPuFile");
  std::string dataPuFile_  = Options.getParameter<std::string>("dataPuFile");
  std::string mcPuHisto_   = Options.getParameter<std::string>("mcPuHisto");
  std::string dataPuHisto_ = Options.getParameter<std::string>("dataPuHisto");

  double minJetPt_         = Options.getParameter<double>("minJetPt");
  double maxJetPt_         = Options.getParameter<double>("maxJetPt");
  double minJetEta_        = Options.getParameter<double>("minJetEta");
  double maxJetEta_        = Options.getParameter<double>("maxJetEta"); 
  
  bool doNvtxReweighting  = false;
  bool doPtReweighting    = false;


  std::string inputJet_   ; 
  std::string etaRange_   ;
 
  // -- overwrite config params , if needed 
  if (argc>2){
    inputTree_         = argv[2];
    outputRootFileName_= argv[3];
    etaRange_          = argv[4];  
    minJetPt_          = atof(argv[5]);
    maxJetPt_          = atof(argv[6]);
    outputRootFileName_+="_"+etaRange_+"_pt"+argv[5]+"to"+argv[6];
 
    if (etaRange_=="TK") {
      minJetEta_=0;
      maxJetEta_=2.5;
    }

    if (etaRange_=="HEin") {
      minJetEta_=2.5;
      maxJetEta_=2.75;
    }

    if (etaRange_=="HEout") {
      minJetEta_=2.75;
      maxJetEta_=3.0;
    }

    if (etaRange_=="HF") {
      minJetEta_=3.0;
      maxJetEta_=5.0;
    }

    if (etaRange_=="ALL") {
      minJetEta_=0.0;
      maxJetEta_=5.0;
    }

  }
  if (argc>7 ){
    doNvtxReweighting  = atoi(argv[7]);
  }
  if (argc>8 ){
    doPtReweighting    = atoi(argv[8]);
  }

   //--- PU reweighting
  edm::LumiReWeighting lumiWeights_ = edm::LumiReWeighting( mcPuFile_.c_str(), dataPuFile_.c_str(), mcPuHisto_.c_str(), dataPuHisto_.c_str());


  //--- NVTX reweighting
  float ww[100];
  std::string filemc   = outputRootFileName_+".root";
  std::string filedata = "histos_DoubleMu2011_pfjets_"+etaRange_+"_pt"+argv[5]+"to"+argv[6]+".root";

  
  if (doNvtxReweighting){
    std::cout<< "file name mc: " << (outputRootFilePath_+filemc) << std::endl;
    std::cout<< "file name data: " << (outputRootFilePath_+filedata)  << std::endl;

    TFile *fmc = TFile::Open( (outputRootFilePath_+filemc).c_str() );
    TH1F *hmc = (TH1F*)fmc->Get("hNvtx");
    hmc ->SetName("hmc");

    TFile *fda = TFile::Open( (outputRootFilePath_+filedata).c_str() );
    TH1F *hda = (TH1F*)fda->Get("hNvtx");
    hda ->SetName("hda");

    hda->Divide(hda, hmc, 1./hda->GetSumOfWeights(),1./hmc->GetSumOfWeights() );
    for (int ibin = 1; ibin < hda->GetNbinsX()+1; ibin++){
      ww[ibin] = hda->GetBinContent(ibin);
    }
    outputRootFileName_=outputRootFileName_+"_reweightedNvtx";
  }

  outputRootFileName_+=".root";

  std::cout<< "Output file name: " << outputRootFileName_ << std::endl;


  // --- open tree
  std::cout << "Reading tree..." << std::endl;
  std::string treeName = inputTree_+"/tree" ;
  TChain* chain = new TChain(treeName.c_str());
  for (unsigned int ifile = 0; ifile < inputFile_.size(); ifile++ ){
    chain->Add(inputFile_.at(ifile).c_str());
  }
  
  JetTree t((chain));

  std::cout << "Number of entries : " << chain-> GetEntries() << std::endl;
 
  //--- book histograms
  TH1F *hNvtx      = new TH1F("hNvtx","Number of vertices", 100, 0, 100.);
  
  // -- jet eta, pt
  TH1F *hjetPt      = new TH1F("hjetPt","jet pt", 100, 0, 100.);
  TH1F *hjetPt_NoPU = new TH1F("hjetPt_NoPU","jet pt (NoPU)", 100, 0, 100.);
  TH1F *hjetPt_PU   = new TH1F("hjetPt_PU","jet pt (PU)", 100, 0, 100.);

  TH1F *hjetEta      = new TH1F("hjetEta","jet eta", 100, -5, 5.);
  TH1F *hjetEta_NoPU = new TH1F("hjetEta_NoPU","jet eta (NoPU)", 100, -5, 5.);
  TH1F *hjetEta_PU   = new TH1F("hjetEta_PU","jet eta (PU)", 100, -5, 5.);

  // -- lead frac
  TH1F *hleadFrac      = new TH1F("hleadFrac","lead fraction", 100, 0, 1.);
  TH1F *hleadFrac_NoPU = new TH1F("hleadFrac_NoPU","lead fraction (NoPU)", 100, 0, 1.);
  TH1F *hleadFrac_PU   = new TH1F("hleadFrac_PU","lead fraction (PU)", 100, 0, 1.);

  TH1F *hleadChFrac      = new TH1F("hleadChFrac","lead fraction", 100, 0, 1.);
  TH1F *hleadChFrac_NoPU = new TH1F("hleadChFrac_NoPU","lead fraction (NoPU)", 100, 0, 1.);
  TH1F *hleadChFrac_PU   = new TH1F("hleadChFrac_PU","lead fraction (PU)", 100, 0, 1.);

  TH1F *hleadEmFrac      = new TH1F("hleadEmFrac","lead fraction", 100, 0, 1.);
  TH1F *hleadEmFrac_NoPU = new TH1F("hleadEmFrac_NoPU","lead fraction (NoPU)", 100, 0, 1.);
  TH1F *hleadEmFrac_PU   = new TH1F("hleadEmFrac_PU","lead fraction (PU)", 100, 0, 1.);

  TH1F *hleadNeutFrac      = new TH1F("hleadNeutFrac","lead fraction", 100, 0, 1.);
  TH1F *hleadNeutFrac_NoPU = new TH1F("hleadNeutFrac_NoPU","lead fraction (NoPU)", 100, 0, 1.);
  TH1F *hleadNeutFrac_PU   = new TH1F("hleadNeutFrac_PU","lead fraction (PU)", 100, 0, 1.);


  // -- particles multiplicity
  TH1F *hnParticles      = new TH1F("hnParticles","Nparticles", 100, 0, 100.);
  TH1F *hnParticles_NoPU = new TH1F("hnParticles_NoPU","Nparticles (NoPU)", 100, 0, 100.);
  TH1F *hnParticles_PU   = new TH1F("hnParticles_PU","Nparticles (PU)", 100, 0, 100.);

  TH1F *hnCharged      = new TH1F("hnCharged","nCharged", 100, 0, 100.);
  TH1F *hnCharged_NoPU = new TH1F("hnCharged_NoPU","nCharged (NoPU)", 100, 0, 100.);
  TH1F *hnCharged_PU   = new TH1F("hnCharged_PU","nCharged (PU)", 100, 0, 100.);

  TH1F *hnNeutrals      = new TH1F("hnNeutrals","nNeutrals", 100, 0, 100.);
  TH1F *hnNeutrals_NoPU = new TH1F("hnNeutrals_NoPU","nNeutrals (NoPU)", 100, 0, 100.);
  TH1F *hnNeutrals_PU   = new TH1F("hnNeutrals_PU","nNeutrals (PU)", 100, 0, 100.);


  // -- deltaR
  TH1F *hdR2Mean      = new TH1F("hdR2Mean","dR2Mean", 100, 0, 1.);
  TH1F *hdR2Mean_NoPU = new TH1F("hdR2Mean_NoPU","dR2Mean (NoPU)", 100, 0, 1.);
  TH1F *hdR2Mean_PU   = new TH1F("hdR2Mean_PU","dR2Mean (PU)", 100, 0, 1.);

  TH1F *hdRMean      = new TH1F("hdRMean","dRMean", 100, 0, 1.);
  TH1F *hdRMean_NoPU = new TH1F("hdRMean_NoPU","dRMean (NoPU)", 100, 0, 1.);
  TH1F *hdRMean_PU   = new TH1F("hdRMean_PU","dRMean (PU)", 100, 0, 1.);

  TH1F *hdRLeadCent      = new TH1F("hdRLeadCent","dRLeadCent", 100, 0, 1.);
  TH1F *hdRLeadCent_NoPU = new TH1F("hdRLeadCent_NoPU","dRLeadCent (NoPU)", 100, 0, 1.);
  TH1F *hdRLeadCent_PU   = new TH1F("hdRLeadCent_PU","dRLeadCent (PU)", 100, 0, 1.);

  TH1F *hdRMeanNeut      = new TH1F("hdRMeanNeut","dRMeanNeut", 100, 0, 1.);
  TH1F *hdRMeanNeut_NoPU = new TH1F("hdRMeanNeut_NoPU","dRMeanNeut (NoPU)", 100, 0, 1.);
  TH1F *hdRMeanNeut_PU   = new TH1F("hdRMeanNeut_PU","dRMeanNeut (PU)", 100, 0, 1.);

  TH1F *hdRMeanEm      = new TH1F("hdRMeanEm","dRMeanEm", 100, 0, 1.);
  TH1F *hdRMeanEm_NoPU = new TH1F("hdRMeanEm_NoPU","dRMeanEm (NoPU)", 100, 0, 1.);
  TH1F *hdRMeanEm_PU   = new TH1F("hdRMeanEm_PU","dRMeanEm (PU)", 100, 0, 1.);

  TH1F *hdRMeanCh      = new TH1F("hdRMeanCh","dRMeanCh", 100, 0, 1.);
  TH1F *hdRMeanCh_NoPU = new TH1F("hdRMeanCh_NoPU","dRMeanCh (NoPU)", 100, 0, 1.);
  TH1F *hdRMeanCh_PU   = new TH1F("hdRMeanCh_PU","dRMeanCh (PU)", 100, 0, 1.);


  // -- fractions in annula of 0.1
  TH1F *hfrac01      = new TH1F("hfrac01","frac01", 100, 0, 1.);
  TH1F *hfrac01_NoPU = new TH1F("hfrac01_NoPU","frac01 (NoPU)", 100, 0, 1.);
  TH1F *hfrac01_PU   = new TH1F("hfrac01_PU","frac01 (PU)", 100, 0, 1.);
 
  TH1F *hchFrac01      = new TH1F("hchFrac01","chFrac01", 100, 0, 1.);
  TH1F *hchFrac01_NoPU = new TH1F("hchFrac01_NoPU","chFrac01 (NoPU)", 100, 0, 1.);
  TH1F *hchFrac01_PU   = new TH1F("hchFrac01_PU","chFrac01 (PU)", 100, 0, 1.);
 
  TH1F *hemFrac01      = new TH1F("hemFrac01","emFrac01", 100, 0, 1.);
  TH1F *hemFrac01_NoPU = new TH1F("hemFrac01_NoPU","emFrac01 (NoPU)", 100, 0, 1.);
  TH1F *hemFrac01_PU   = new TH1F("hemFrac01_PU","emFrac01 (PU)", 100, 0, 1.);
 
  TH1F *hneutFrac01      = new TH1F("hneutFrac01","neutFrac01", 100, 0, 1.);
  TH1F *hneutFrac01_NoPU = new TH1F("hneutFrac01_NoPU","neutFrac01 (NoPU)", 100, 0, 1.);
  TH1F *hneutFrac01_PU   = new TH1F("hneutFrac01_PU","neutFrac01 (PU)", 100, 0, 1.);

  // -- fractions in annula of 0.2
  TH1F *hfrac02      = new TH1F("hfrac02","frac02", 100, 0, 1.);
  TH1F *hfrac02_NoPU = new TH1F("hfrac02_NoPU","frac02 (NoPU)", 100, 0, 1.);
  TH1F *hfrac02_PU   = new TH1F("hfrac02_PU","frac02 (PU)", 100, 0, 1.);
 
  TH1F *hchFrac02      = new TH1F("hchFrac02","chFrac02", 100, 0, 1.);
  TH1F *hchFrac02_NoPU = new TH1F("hchFrac02_NoPU","chFrac02 (NoPU)", 100, 0, 1.);
  TH1F *hchFrac02_PU   = new TH1F("hchFrac02_PU","chFrac02 (PU)", 100, 0, 1.);
 
  TH1F *hemFrac02      = new TH1F("hemFrac02","emFrac02", 100, 0, 1.);
  TH1F *hemFrac02_NoPU = new TH1F("hemFrac02_NoPU","emFrac02 (NoPU)", 100, 0, 1.);
  TH1F *hemFrac02_PU   = new TH1F("hemFrac02_PU","emFrac02 (PU)", 100, 0, 1.);
 
  TH1F *hneutFrac02      = new TH1F("hneutFrac02","neutFrac02", 100, 0, 1.);
  TH1F *hneutFrac02_NoPU = new TH1F("hneutFrac02_NoPU","neutFrac02 (NoPU)", 100, 0, 1.);
  TH1F *hneutFrac02_PU   = new TH1F("hneutFrac02_PU","neutFrac02 (PU)", 100, 0, 1.);
 
  // -- fractions in annula of 0.3
  TH1F *hfrac03      = new TH1F("hfrac03","frac03", 100, 0, 1.);
  TH1F *hfrac03_NoPU = new TH1F("hfrac03_NoPU","frac03 (NoPU)", 100, 0, 1.);
  TH1F *hfrac03_PU   = new TH1F("hfrac03_PU","frac03 (PU)", 100, 0, 1.);
 
  TH1F *hchFrac03      = new TH1F("hchFrac03","chFrac03", 100, 0, 1.);
  TH1F *hchFrac03_NoPU = new TH1F("hchFrac03_NoPU","chFrac03 (NoPU)", 100, 0, 1.);
  TH1F *hchFrac03_PU   = new TH1F("hchFrac03_PU","chFrac03 (PU)", 100, 0, 1.);
 
  TH1F *hemFrac03      = new TH1F("hemFrac03","emFrac03", 100, 0, 1.);
  TH1F *hemFrac03_NoPU = new TH1F("hemFrac03_NoPU","emFrac03 (NoPU)", 100, 0, 1.);
  TH1F *hemFrac03_PU   = new TH1F("hemFrac03_PU","emFrac03 (PU)", 100, 0, 1.);
 
  TH1F *hneutFrac03      = new TH1F("hneutFrac03","neutFrac03", 100, 0, 1.);
  TH1F *hneutFrac03_NoPU = new TH1F("hneutFrac03_NoPU","neutFrac03 (NoPU)", 100, 0, 1.);
  TH1F *hneutFrac03_PU   = new TH1F("hneutFrac03_PU","neutFrac03 (PU)", 100, 0, 1.);

  // -- fractions in annula of 0.4
  TH1F *hfrac04      = new TH1F("hfrac04","frac04", 100, 0, 1.);
  TH1F *hfrac04_NoPU = new TH1F("hfrac04_NoPU","frac04 (NoPU)", 100, 0, 1.);
  TH1F *hfrac04_PU   = new TH1F("hfrac04_PU","frac04 (PU)", 100, 0, 1.);
 
  TH1F *hchFrac04      = new TH1F("hchFrac04","chFrac04", 100, 0, 1.);
  TH1F *hchFrac04_NoPU = new TH1F("hchFrac04_NoPU","chFrac04 (NoPU)", 100, 0, 1.);
  TH1F *hchFrac04_PU   = new TH1F("hchFrac04_PU","chFrac04 (PU)", 100, 0, 1.);
 
  TH1F *hemFrac04      = new TH1F("hemFrac04","emFrac04", 100, 0, 1.);
  TH1F *hemFrac04_NoPU = new TH1F("hemFrac04_NoPU","emFrac04 (NoPU)", 100, 0, 1.);
  TH1F *hemFrac04_PU   = new TH1F("hemFrac04_PU","emFrac04 (PU)", 100, 0, 1.);
 
  TH1F *hneutFrac04      = new TH1F("hneutFrac04","neutFrac04", 100, 0, 1.);
  TH1F *hneutFrac04_NoPU = new TH1F("hneutFrac04_NoPU","neutFrac04 (NoPU)", 100, 0, 1.);
  TH1F *hneutFrac04_PU   = new TH1F("hneutFrac04_PU","neutFrac04 (PU)", 100, 0, 1.);
 
  // -- fractions in annula of 0.5
  TH1F *hfrac05      = new TH1F("hfrac05","frac05", 100, 0, 1.);
  TH1F *hfrac05_NoPU = new TH1F("hfrac05_NoPU","frac05 (NoPU)", 100, 0, 1.);
  TH1F *hfrac05_PU   = new TH1F("hfrac05_PU","frac05 (PU)", 100, 0, 1.);
 
  TH1F *hchFrac05      = new TH1F("hchFrac05","chFrac05", 100, 0, 1.);
  TH1F *hchFrac05_NoPU = new TH1F("hchFrac05_NoPU","chFrac05 (NoPU)", 100, 0, 1.);
  TH1F *hchFrac05_PU   = new TH1F("hchFrac05_PU","chFrac05 (PU)", 100, 0, 1.);
 
  TH1F *hemFrac05      = new TH1F("hemFrac05","emFrac05", 100, 0, 1.);
  TH1F *hemFrac05_NoPU = new TH1F("hemFrac05_NoPU","emFrac05 (NoPU)", 100, 0, 1.);
  TH1F *hemFrac05_PU   = new TH1F("hemFrac05_PU","emFrac05 (PU)", 100, 0, 1.);
 
  TH1F *hneutFrac05      = new TH1F("hneutFrac05","neutFrac05", 100, 0, 1.);
  TH1F *hneutFrac05_NoPU = new TH1F("hneutFrac05_NoPU","neutFrac05 (NoPU)", 100, 0, 1.);
  TH1F *hneutFrac05_PU   = new TH1F("hneutFrac05_PU","neutFrac05 (PU)", 100, 0, 1.);
 
  // -- ptD
  TH1F *hptD      = new TH1F("hptD","ptD", 100, 0, 1.);
  TH1F *hptD_NoPU = new TH1F("hptD_NoPU","ptD (NoPU)", 100, 0, 1.);
  TH1F *hptD_PU   = new TH1F("hptD_PU","ptD (PU)", 100, 0, 1.);
 

  // -- shower shapes
  TH1F *hetaW      = new TH1F("hetaW","etaW", 400, 0, 1.);
  TH1F *hetaW_NoPU = new TH1F("hetaW_NoPU","etaW (NoPU)", 400, 0, 1.);
  TH1F *hetaW_PU   = new TH1F("hetaW_PU","etaW (PU)", 400, 0, 1.);

  TH1F *hphiW      = new TH1F("hphiW","phiW", 400, 0, 1.);
  TH1F *hphiW_NoPU = new TH1F("hphiW_NoPU","phiW (NoPU)", 400, 0, 1.);
  TH1F *hphiW_PU   = new TH1F("hphiW_PU","phiW (PU)", 400, 0, 1.);

  TH1F *hmajW      = new TH1F("hmajW","majW", 400, 0, 1.);
  TH1F *hmajW_NoPU = new TH1F("hmajW_NoPU","majW (NoPU)", 400, 0, 1.);
  TH1F *hmajW_PU   = new TH1F("hmajW_PU","majW (PU)", 400, 0, 1.);

  TH1F *hminW      = new TH1F("hminW","minW", 400, 0, 1.);
  TH1F *hminW_NoPU = new TH1F("hminW_NoPU","minW (NoPU)", 400, 0, 1.);
  TH1F *hminW_PU   = new TH1F("hminW_PU","minW (PU)", 400, 0, 1.);


  // -- beta and betaStar
  TH1F *hbeta      = new TH1F("hbeta","beta", 400, 0, 1.);
  TH1F *hbeta_NoPU = new TH1F("hbeta_NoPU","beta (NoPU)", 400, 0, 1.);
  TH1F *hbeta_PU   = new TH1F("hbeta_PU","beta (PU)", 400, 0, 1.);

  TH1F *hbetaStar      = new TH1F("hbetaStar","betaStar", 400, 0, 1.);
  TH1F *hbetaStar_NoPU = new TH1F("hbetaStar_NoPU","betaStar (NoPU)", 400, 0, 1.);
  TH1F *hbetaStar_PU   = new TH1F("hbetaStar_PU","betaStar (PU)", 400, 0, 1.);


  // -- mva output
  TH1F *hmva      = new TH1F("hmva","mva", 200, -1, 1.);
  TH1F *hmva_NoPU = new TH1F("hmva_NoPU","mva (NoPU)", 200, -1, 1.);
  TH1F *hmva_PU   = new TH1F("hmva_PU","mva (PU)", 200, -1, 1.);

  TH1F *hsimpleDiscriminant      = new TH1F("hsimpleDiscriminant","simpleDiscriminant", 200, -1, 1.);
  TH1F *hsimpleDiscriminant_NoPU = new TH1F("hsimpleDiscriminant_NoPU","simpleDiscriminant (NoPU)", 200, -1, 1.);
  TH1F *hsimpleDiscriminant_PU   = new TH1F("hsimpleDiscriminant_PU","simpleDiscriminant (PU)", 200, -1, 1.);

  TH1F *hfullDiscriminant      = new TH1F("hfullDiscriminant","fullDiscriminant", 200, -1, 1.);
  TH1F *hfullDiscriminant_NoPU = new TH1F("hfullDiscriminant_NoPU","fullDiscriminant (NoPU)", 200, -1, 1.);
  TH1F *hfullDiscriminant_PU   = new TH1F("hfullDiscriminant_PU","fullDiscriminant (PU)", 200, -1, 1.);

  TH1F *hcutbasedDiscriminant      = new TH1F("hcutbasedDiscriminant","cutbasedDiscriminant", 200, -1, 1.);
  TH1F *hcutbasedDiscriminant_NoPU = new TH1F("hcutbasedDiscriminant_NoPU","cutbasedDiscriminant (NoPU)", 200, -1, 1.);
  TH1F *hcutbasedDiscriminant_PU   = new TH1F("hcutbasedDiscriminant_PU","cutbasedDiscriminant (PU)", 200, -1, 1.);


  //--- for efficiency studies
  TH2F *hPtRatio_vs_Dphi = new TH2F("hPtRatio_vs_Dphi","hPtRatio_vs_Dphi",100,-3.2,3.2,100,0,10);

  char hname[100];
  std::string suffLevel[3] = {"Tight","Medium","Loose"};
  std::string suffId[3]    = {"simpleId","fullId","cutbasedId"};


  //--- pt ratio 
  TH1F *hPtRatio_matched_noId  = new TH1F("hPtRatio_matched_noId","Pt Ratio (MC matched jets)",100,0,10); 
  TH1F *hPtRatio_noId          = new TH1F("hPtRatio_noId","Pt Ratio",100,0,10); 
  TH1F* hPtRatio_matched[3][3];
  TH1F* hPtRatio[3][3];
   
  for ( int iid = 0; iid < 3 ; iid++ ){
    for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
      sprintf(hname,"hPtRatio_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hPtRatio_matched[iid][ilevel] = new TH1F(hname,hname,100,0,10); 
      sprintf(hname,"hPtRatio_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hPtRatio[iid][ilevel] = new TH1F(hname,hname,100,0,10); 
    }
  }

  //-- jet pt for "true" jets (= matched jets)
  TH1F *hJetPt_matched_noId = new TH1F("hJetPt_matched_noId","hJetPt_matched",100,0,100);
  TH1F *hJetPt_noId         = new TH1F("hJetPt_noId","hJetPt",100,0,100);
  TH1F *hJetPt_matched[3][3];
  TH1F *hJetPt[3][3];
  for ( int iid = 0; iid < 3 ; iid++ ){
    for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
      sprintf(hname,"hJetPt_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hJetPt_matched[iid][ilevel] = new TH1F(hname,hname,100,0,100);
      sprintf(hname,"hJetPt_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hJetPt[iid][ilevel] = new TH1F(hname,hname,100,0,100);
    }
  }

  //-- jet eta for "true" jets (= matched jets)
  TH1F *hJetEta_matched_noId = new TH1F("hJetEta_matched_noId","hJetEta_matched",100,-5,5);
  TH1F *hJetEta_noId         = new TH1F("hJetEta_noId","hJetEta",100,-5,5);
  TH1F *hJetEta_matched[3][3];
  TH1F *hJetEta[3][3];
  for ( int iid = 0; iid < 3 ; iid++ ){
    for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
      sprintf(hname,"hJetEta_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hJetEta_matched[iid][ilevel] = new TH1F(hname,hname,100,-5,5);
      sprintf(hname,"hJetEta_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hJetEta[iid][ilevel] = new TH1F(hname,hname,100,-5,5);
    }
  }

  //-- nvtx
  TH1F *hNumberOfVertices_matched_noId = new TH1F("hNumberOfVertices_matched_noId","hNumberOfVertices_matched",100,0,100);
  TH1F *hNumberOfVertices_noId         = new TH1F("hNumberOfVertices_noId","hNumberOfVertices",100,0,100);
  TH1F *hNumberOfVertices_matched[3][3];
  TH1F *hNumberOfVertices[3][3];
  for ( int iid = 0; iid < 3 ; iid++ ){
    for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
      sprintf(hname,"hNumberOfVertices_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hNumberOfVertices_matched[iid][ilevel] = new TH1F(hname,hname,100,0,100);
      sprintf(hname,"hNumberOfVertices_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hNumberOfVertices[iid][ilevel] = new TH1F(hname,hname,100,0,100);
    }
  }


  float w = 1;  

  float dphiCut = 2.5;

  for (int ientry = 0; ientry  < chain->GetEntries(); ientry++ ){
    
    t.GetEntry(ientry);
    
    if (ientry%100000==0) std::cout << "Analyzing entry : " << ientry << "\r" << std::flush;
 
    if (!t.jetLooseID) continue;
    
    if ( t.jetPt < minJetPt_ ) continue;
    if ( t.jetPt > maxJetPt_ ) continue;

    if ( fabs(t.jetEta) < minJetEta_ ) continue;
    if ( fabs(t.jetEta) > maxJetEta_ ) continue;

    
    if (dataFlag_) w = 1;
    else w = lumiWeights_.weight( t.PUit_n );
    if (doNvtxReweighting)  w*=ww[int(t.nvtx)];
    
    // -- fill histograms for leading jet
    if ( t.ijet==1 ) {
      hNvtx->Fill(t.nvtx,w);
      float ptratio = 0;
      if ( t.dimuonPt > 0. ) ptratio = t.jetPt/t.dimuonPt;

      if ( ptratio > 0. )
	hPtRatio_vs_Dphi -> Fill(t.dphiZJet,ptratio,w);
      
      // -- fill ptRatio 
      if (ptratio > 0. && fabs(t.dphiZJet)>dphiCut){
	
	// -- fill plost for jets matching to gen jets
	if ( dataFlag_==0 && ptratio > 0. && t.isMatched && t.jetGenPt > 10.){
	  hPtRatio_matched_noId -> Fill(ptratio,w);  
	  for (int ilevel = 0; ilevel < 3; ilevel++){
	    if ( pass_level(t.simpleId,ilevel)  )    hPtRatio_matched[0][ilevel] -> Fill(ptratio,w);   
	    if ( pass_level(t.fullId,ilevel)    )    hPtRatio_matched[1][ilevel] -> Fill(ptratio,w);   
	    if ( pass_level(t.cutbasedId,ilevel))    hPtRatio_matched[2][ilevel] -> Fill(ptratio,w);   
	  }
	}
	
	// -- fill plost for selected jets
	hPtRatio_noId -> Fill(ptratio,w);  
	for (int ilevel = 0; ilevel < 3; ilevel++){
	  if ( pass_level(t.simpleId,ilevel)  )  hPtRatio[0][ilevel]-> Fill(ptratio,w);   
	  if ( pass_level(t.fullId,ilevel)    )  hPtRatio[1][ilevel]-> Fill(ptratio,w);   
	  if ( pass_level(t.cutbasedId,ilevel))  hPtRatio[2][ilevel]-> Fill(ptratio,w);   
	}
      }

      // -- fill jet pT , eta, nvtx plots
      if ( ptratio > 0.5 && ptratio < 1.5 && fabs(t.dphiZJet) > dphiCut ) {
	
	// -- fill plots for selected jets matching to gen jets
	if ( dataFlag_==0 && ptratio > 0. && t.isMatched && t.jetGenPt > 10.){
	  hJetPt_matched_noId   -> Fill(t.jetPt,w);  
	  hJetEta_matched_noId  -> Fill(t.jetEta,w);
	  hNumberOfVertices_matched_noId->Fill(t.nvtx,w);
	
	  for (int ilevel = 0; ilevel < 3; ilevel++){
	    if ( pass_level(t.simpleId,ilevel)  )  {
	      hJetPt_matched[0][ilevel]   -> Fill(t.jetPt,w);  
	      hJetEta_matched[0][ilevel]  -> Fill(t.jetEta,w);
	      hNumberOfVertices_matched[0][ilevel]-> Fill(t.nvtx,w);
	    }
	    if ( pass_level(t.fullId,ilevel)    )  {
	      hJetPt_matched[1][ilevel]   -> Fill(t.jetPt,w);  
	      hJetEta_matched[1][ilevel]  -> Fill(t.jetEta,w);
	      hNumberOfVertices_matched[1][ilevel]-> Fill(t.nvtx,w);
	    }
	    if ( pass_level(t.cutbasedId,ilevel))  {
	      hJetPt_matched[2][ilevel]   -> Fill(t.jetPt,w);  
	      hJetEta_matched[2][ilevel]  -> Fill(t.jetEta,w);
	      hNumberOfVertices_matched[2][ilevel]-> Fill(t.nvtx,w);
	    }
	  }	
	}
	
	//-- fill plots for all selected jets
	hJetPt_noId   -> Fill(t.jetPt,w);  
	hJetEta_noId  -> Fill(t.jetEta,w);
	hNumberOfVertices_noId->Fill(t.nvtx,w);

	for (int ilevel = 0; ilevel < 3; ilevel++){
	  if ( pass_level(t.simpleId,ilevel) )  {
	    hJetPt[0][ilevel]-> Fill(t.jetPt,w);  
	    hJetEta[0][ilevel]-> Fill(t.jetEta,w); 
	    hNumberOfVertices[0][ilevel]->Fill(t.nvtx,w);
	  } 
	  if ( pass_level(t.fullId,ilevel)   )  {
	    hJetPt[1][ilevel]-> Fill(t.jetPt,w);  
	    hJetEta[1][ilevel]-> Fill(t.jetEta,w);  
	    hNumberOfVertices[1][ilevel]->Fill(t.nvtx,w);
	  }
	  if ( pass_level(t.cutbasedId,ilevel) ) {
	    hJetPt[2][ilevel]-> Fill(t.jetPt,w);  
	    hJetEta[2][ilevel]-> Fill(t.jetEta,w);  
	    hNumberOfVertices[2][ilevel]->Fill(t.nvtx,w);
	  }
	}
      }
      
    }
    
    //-- kinematic and jet id variables 
    hjetPt        -> Fill(t.jetPt,w);
    hjetEta       -> Fill(t.jetEta,w);
    hleadFrac     -> Fill(t.leadFrac,w); 
    hleadNeutFrac -> Fill(t.leadNeutFrac,w); 
    hleadChFrac   -> Fill(t.leadChFrac,w); 
    hleadEmFrac   -> Fill(t.leadEmFrac,w); 
    hnParticles   -> Fill(t.nParticles,w);
    hnCharged     -> Fill(t.nCharged,w);
    hnNeutrals    -> Fill(t.nNeutrals,w);
    hdR2Mean      -> Fill(t.dR2Mean,w);
    hdRMean       -> Fill(t.dRMean,w);
    hdRMeanNeut   -> Fill(t.dRMeanNeut,w);
    hdRMeanEm     -> Fill(t.dRMeanEm,w);
    hdRMeanCh     -> Fill(t.dRMeanCh,w);
    hdRLeadCent   -> Fill(t.dRLeadCent,w);
 
    hfrac01       -> Fill(t.frac01,w);
    hchFrac01     -> Fill(t.chFrac01,w);
    hemFrac01     -> Fill(t.emFrac01,w);
    hneutFrac01   -> Fill(t.neutFrac01,w);
    hfrac02       -> Fill(t.frac02,w);
    hchFrac02     -> Fill(t.chFrac02,w);
    hemFrac02     -> Fill(t.emFrac02,w);
    hneutFrac02   -> Fill(t.neutFrac02,w);
    hfrac03       -> Fill(t.frac03,w);
    hchFrac03     -> Fill(t.chFrac03,w);
    hemFrac03     -> Fill(t.emFrac03,w);
    hneutFrac03   -> Fill(t.neutFrac03,w);
    hfrac04       -> Fill(t.frac04,w);
    hchFrac04     -> Fill(t.chFrac04,w);
    hemFrac04     -> Fill(t.emFrac04,w);
    hneutFrac04   -> Fill(t.neutFrac04,w);
    hfrac05       -> Fill(t.frac05,w);
    hchFrac05     -> Fill(t.chFrac05,w);
    hemFrac05     -> Fill(t.emFrac05,w);
    hneutFrac05   -> Fill(t.neutFrac05,w);

    hptD          -> Fill(t.ptD,w);
    hetaW         -> Fill(t.etaW,w);
    hphiW         -> Fill(t.phiW,w);
    hmajW         -> Fill(t.majW,w);
    hminW         -> Fill(t.minW,w);
    hbeta         -> Fill(t.beta,w);
    hbetaStar     -> Fill(t.betaStar,w);
    hmva          -> Fill(t.mva,w);
    hsimpleDiscriminant -> Fill(t.simpleDiscriminant,w);
    hfullDiscriminant   -> Fill(t.fullDiscriminant,w);
    hcutbasedDiscriminant -> Fill(t.cutbasedDiscriminant,w);

    if ( t.isMatched ) {
      hjetPt_NoPU        -> Fill(t.jetPt,w);
      hjetEta_NoPU       -> Fill(t.jetEta,w);
      hleadFrac_NoPU     -> Fill(t.leadFrac,w);
      hleadNeutFrac_NoPU -> Fill(t.leadNeutFrac,w); 
      hleadChFrac_NoPU   -> Fill(t.leadChFrac,w); 
      hleadEmFrac_NoPU   -> Fill(t.leadEmFrac,w);  
      hnParticles_NoPU   -> Fill(t.nParticles,w);
      hnCharged_NoPU     -> Fill(t.nCharged,w);
      hnNeutrals_NoPU    -> Fill(t.nNeutrals,w);
      hdRMean_NoPU       -> Fill(t.dRMean,w);
      hdR2Mean_NoPU      -> Fill(t.dR2Mean,w);
      hdRMeanNeut_NoPU   -> Fill(t.dRMeanNeut,w);
      hdRMeanEm_NoPU     -> Fill(t.dRMeanEm,w);
      hdRMeanCh_NoPU     -> Fill(t.dRMeanCh,w);
      hdRLeadCent_NoPU   -> Fill(t.dRLeadCent,w);
      
      hfrac01_NoPU       -> Fill(t.frac01,w);
      hchFrac01_NoPU     -> Fill(t.chFrac01,w);
      hemFrac01_NoPU     -> Fill(t.emFrac01,w);
      hneutFrac01_NoPU   -> Fill(t.neutFrac01,w);
      hfrac02_NoPU       -> Fill(t.frac02,w);
      hchFrac02_NoPU     -> Fill(t.chFrac02,w);
      hemFrac02_NoPU     -> Fill(t.emFrac02,w);
      hneutFrac02_NoPU   -> Fill(t.neutFrac02,w);
      hfrac03_NoPU       -> Fill(t.frac03,w);
      hchFrac03_NoPU     -> Fill(t.chFrac03,w);
      hemFrac03_NoPU     -> Fill(t.emFrac03,w);
      hneutFrac03_NoPU   -> Fill(t.neutFrac03,w);
      hfrac04_NoPU       -> Fill(t.frac04,w);
      hchFrac04_NoPU     -> Fill(t.chFrac04,w);
      hemFrac04_NoPU     -> Fill(t.emFrac04,w);
      hneutFrac04_NoPU   -> Fill(t.neutFrac04,w);
      hfrac05_NoPU       -> Fill(t.frac05,w);
      hchFrac05_NoPU     -> Fill(t.chFrac05,w);
      hemFrac05_NoPU     -> Fill(t.emFrac05,w);
      hneutFrac05_NoPU   -> Fill(t.neutFrac05,w);     

      hptD_NoPU          -> Fill(t.ptD,w);
      hetaW_NoPU         -> Fill(t.etaW,w);
      hphiW_NoPU         -> Fill(t.phiW,w);
      hmajW_NoPU         -> Fill(t.majW,w);
      hminW_NoPU         -> Fill(t.minW,w);
      hbeta_NoPU         -> Fill(t.beta,w);
      hbetaStar_NoPU     -> Fill(t.betaStar,w);
      hmva_NoPU          -> Fill(t.mva,w);

      hsimpleDiscriminant_NoPU  -> Fill(t.simpleDiscriminant,w);
      hfullDiscriminant_NoPU    -> Fill(t.fullDiscriminant,w);
      hcutbasedDiscriminant_NoPU  -> Fill(t.cutbasedDiscriminant,w);
      
    }
    else{
      hjetPt_PU        -> Fill(t.jetPt,w);
      hjetEta_PU       -> Fill(t.jetEta,w);
      hleadFrac_PU     -> Fill(t.leadFrac,w); 
      hleadNeutFrac_PU -> Fill(t.leadNeutFrac,w); 
      hleadChFrac_PU   -> Fill(t.leadChFrac,w); 
      hleadEmFrac_PU   -> Fill(t.leadEmFrac,w);  
      hnParticles_PU   -> Fill(t.nParticles,w);
      hnCharged_PU     -> Fill(t.nCharged,w);
      hnNeutrals_PU    -> Fill(t.nNeutrals,w);
      hdR2Mean_PU      -> Fill(t.dR2Mean,w);
      hdRMean_PU       -> Fill(t.dRMean,w);
      hdRMeanNeut_PU   -> Fill(t.dRMeanNeut,w);
      hdRMeanEm_PU     -> Fill(t.dRMeanEm,w);
      hdRMeanCh_PU     -> Fill(t.dRMeanCh,w);
      hdRLeadCent_PU   -> Fill(t.dRLeadCent,w);

      hfrac01_PU       -> Fill(t.frac01,w);
      hchFrac01_PU     -> Fill(t.chFrac01,w);
      hemFrac01_PU     -> Fill(t.emFrac01,w);
      hneutFrac01_PU   -> Fill(t.neutFrac01,w);
      hfrac02_PU       -> Fill(t.frac02,w);
      hchFrac02_PU     -> Fill(t.chFrac02,w);
      hemFrac02_PU     -> Fill(t.emFrac02,w);
      hneutFrac02_PU   -> Fill(t.neutFrac02,w);
      hfrac03_PU       -> Fill(t.frac03,w);
      hchFrac03_PU     -> Fill(t.chFrac03,w);
      hemFrac03_PU     -> Fill(t.emFrac03,w);
      hneutFrac03_PU   -> Fill(t.neutFrac03,w);
      hfrac04_PU       -> Fill(t.frac04,w);
      hchFrac04_PU     -> Fill(t.chFrac04,w);
      hemFrac04_PU     -> Fill(t.emFrac04,w);
      hneutFrac04_PU   -> Fill(t.neutFrac04,w);
      hfrac05_PU       -> Fill(t.frac05,w);
      hchFrac05_PU     -> Fill(t.chFrac05,w);
      hemFrac05_PU     -> Fill(t.emFrac05,w);
      hneutFrac05_PU   -> Fill(t.neutFrac05,w);

      hptD_PU          -> Fill(t.ptD,w);
      hetaW_PU         -> Fill(t.etaW,w);
      hphiW_PU         -> Fill(t.phiW,w);
      hmajW_PU         -> Fill(t.majW,w);
      hminW_PU         -> Fill(t.minW,w);
      hbeta_PU         -> Fill(t.beta,w);
      hbetaStar_PU     -> Fill(t.betaStar,w);
      hmva_PU          -> Fill(t.mva,w);

      hsimpleDiscriminant_PU  -> Fill(t.simpleDiscriminant,w);
      hfullDiscriminant_PU    -> Fill(t.fullDiscriminant,w);
      hcutbasedDiscriminant_PU  -> Fill(t.cutbasedDiscriminant,w);

    }

  }// end loop over entries


  //--- compute efficiencies
  TH1F *hEff_vs_PtRatio_matched[3][3]; 
  TH1F *hEff_vs_PtRatio[3][3]; 
  
  TH1F *hEff_vs_JetPt_matched[3][3]; 
  TH1F *hEff_vs_JetPt[3][3]; 

  TH1F *hEff_vs_JetEta_matched[3][3]; 
  TH1F *hEff_vs_JetEta[3][3]; 

  TH1F *hEff_vs_NumberOfVertices_matched[3][3]; 
  TH1F *hEff_vs_NumberOfVertices[3][3]; 

  
  hPtRatio_noId->Sumw2();
  hPtRatio_matched_noId->Sumw2();
  hJetPt_noId->Sumw2();
  hJetPt_matched_noId->Sumw2();
  hJetEta_noId->Sumw2();
  hJetEta_matched_noId->Sumw2();

  for ( int iid = 0; iid < 3 ; iid++ ){
    for (int ilevel = 0; ilevel < 3; ilevel++){
  
      // -  matched jets --> "true" efficiency
      if (dataFlag_==0){
	hPtRatio_matched[iid][ilevel]->Sumw2();
	sprintf(hname,"hEff_vs_PtRatio_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_PtRatio_matched[iid][ilevel]=(TH1F*)hPtRatio_matched[iid][ilevel]->Clone(hname);
	hEff_vs_PtRatio_matched[iid][ilevel]->Divide(hEff_vs_PtRatio_matched[iid][ilevel],hPtRatio_matched_noId,1,1,"B");
	
	hJetPt_matched[iid][ilevel]->Sumw2();
	sprintf(hname,"hEff_vs_JetPt_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_JetPt_matched[iid][ilevel]=(TH1F*)hJetPt_matched[iid][ilevel]->Clone(hname);
	hEff_vs_JetPt_matched[iid][ilevel]->Divide(hEff_vs_JetPt_matched[iid][ilevel],hJetPt_matched_noId,1,1,"B");

	hJetEta_matched[iid][ilevel]->Sumw2();
	sprintf(hname,"hEff_vs_JetEta_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_JetEta_matched[iid][ilevel]=(TH1F*)hJetEta_matched[iid][ilevel]->Clone(hname);
	hEff_vs_JetEta_matched[iid][ilevel]->Divide(hEff_vs_JetEta_matched[iid][ilevel],hJetEta_matched_noId,1,1,"B");

	hNumberOfVertices_matched[iid][ilevel]->Sumw2();
	sprintf(hname,"hEff_vs_NumberOfVertices_matched_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_NumberOfVertices_matched[iid][ilevel]=(TH1F*)hNumberOfVertices_matched[iid][ilevel]->Clone(hname);
	hEff_vs_NumberOfVertices_matched[iid][ilevel]->Divide(hEff_vs_NumberOfVertices_matched[iid][ilevel],hNumberOfVertices_matched_noId,1,1,"B");
      }

      hPtRatio[iid][ilevel]->Sumw2();
      sprintf(hname,"hEff_vs_PtRatio_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hEff_vs_PtRatio[iid][ilevel]=(TH1F*)hPtRatio[iid][ilevel]->Clone(hname);
      hEff_vs_PtRatio[iid][ilevel]->Divide(hEff_vs_PtRatio[iid][ilevel],hPtRatio_noId,1,1,"B");
      
      hJetPt[iid][ilevel]->Sumw2();
      sprintf(hname,"hEff_vs_JetPt_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hEff_vs_JetPt[iid][ilevel]=(TH1F*)hJetPt[iid][ilevel]->Clone(hname);
      hEff_vs_JetPt[iid][ilevel]->Divide(hEff_vs_JetPt[iid][ilevel],hJetPt_noId,1,1,"B");
      
      hJetEta[iid][ilevel]->Sumw2();
      sprintf(hname,"hEff_vs_JetEta_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hEff_vs_JetEta[iid][ilevel]=(TH1F*)hJetEta[iid][ilevel]->Clone(hname);
      hEff_vs_JetEta[iid][ilevel]->Divide(hEff_vs_JetEta[iid][ilevel],hJetEta_noId,1,1,"B");
      
      hNumberOfVertices[iid][ilevel]->Sumw2();
      sprintf(hname,"hEff_vs_NumberOfVertices_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      hEff_vs_NumberOfVertices[iid][ilevel]=(TH1F*)hNumberOfVertices[iid][ilevel]->Clone(hname);
      hEff_vs_NumberOfVertices[iid][ilevel]->Divide(hEff_vs_NumberOfVertices[iid][ilevel],hNumberOfVertices_noId,1,1,"B");
      
    }
  }

  
  // save the histograms
 
  std::cout << "Saving histograms on file : " << outputRootFilePath_+outputRootFileName_ << std::endl;
    
  TFile* outputRootFile = new TFile((outputRootFilePath_+outputRootFileName_).c_str(), "RECREATE");

  outputRootFile -> cd();

  hNvtx ->Write();
  
  hjetPt        -> Write();
  hjetEta       -> Write();
  hleadFrac     -> Write();
  hleadNeutFrac -> Write();
  hleadEmFrac   -> Write();
  hleadChFrac   -> Write();
  hnParticles   -> Write();
  hnCharged     -> Write();
  hnNeutrals    -> Write();
  hdR2Mean      -> Write();
  hdRMean       -> Write();
  hdRMeanNeut   -> Write();
  hdRMeanEm     -> Write();
  hdRMeanCh     -> Write();
  hdRLeadCent   -> Write();
  hfrac01       -> Write();
  hchFrac01     -> Write();
  hemFrac01     -> Write();
  hneutFrac01   -> Write();
  hfrac02       -> Write();
  hchFrac02     -> Write();
  hemFrac02     -> Write();
  hneutFrac02   -> Write();
  hfrac03       -> Write();
  hchFrac03     -> Write();
  hemFrac03     -> Write();
  hneutFrac03   -> Write();
  hfrac04       -> Write();
  hchFrac04     -> Write();
  hemFrac04     -> Write();
  hneutFrac04   -> Write();
  hfrac05       -> Write();
  hchFrac05     -> Write();
  hemFrac05     -> Write();
  hneutFrac05   -> Write();
  hptD          -> Write();
  hetaW         -> Write();
  hphiW         -> Write();
  hmajW         -> Write();
  hminW         -> Write();
  hbeta         -> Write();
  hbetaStar     -> Write();
  hmva          -> Write();
  hsimpleDiscriminant->Write();
  hfullDiscriminant  ->Write();
  hcutbasedDiscriminant->Write();


  hjetPt_NoPU       -> Write();
  hjetEta_NoPU      -> Write();
  hleadFrac_NoPU    -> Write();
  hleadNeutFrac_NoPU-> Write();
  hleadEmFrac_NoPU  -> Write();
  hleadChFrac_NoPU  -> Write();
  hnParticles_NoPU  -> Write();
  hnCharged_NoPU    -> Write();
  hnNeutrals_NoPU   -> Write();
  hdR2Mean_NoPU     -> Write();
  hdRMean_NoPU      -> Write();
  hdRMeanNeut_NoPU  -> Write();
  hdRMeanEm_NoPU    -> Write();
  hdRMeanCh_NoPU    -> Write();
  hdRLeadCent_NoPU  -> Write();
  hfrac01_NoPU      -> Write();
  hchFrac01_NoPU    -> Write();
  hemFrac01_NoPU    -> Write();
  hneutFrac01_NoPU  -> Write();
  hfrac02_NoPU      -> Write();
  hchFrac02_NoPU    -> Write();
  hemFrac02_NoPU    -> Write();
  hneutFrac02_NoPU  -> Write();
  hfrac03_NoPU      -> Write();
  hchFrac03_NoPU    -> Write();
  hemFrac03_NoPU    -> Write();
  hneutFrac03_NoPU  -> Write();
  hfrac04_NoPU      -> Write();
  hchFrac04_NoPU    -> Write();
  hemFrac04_NoPU    -> Write();
  hneutFrac04_NoPU  -> Write();
  hfrac05_NoPU      -> Write();
  hchFrac05_NoPU    -> Write();
  hemFrac05_NoPU    -> Write();
  hneutFrac05_NoPU  -> Write();
  hptD_NoPU         -> Write();
  hetaW_NoPU        -> Write();
  hphiW_NoPU        -> Write();
  hmajW_NoPU        -> Write();
  hminW_NoPU        -> Write();
  hbeta_NoPU        -> Write();
  hbetaStar_NoPU    -> Write();
  hmva_NoPU         -> Write();
  hsimpleDiscriminant_NoPU->Write();
  hfullDiscriminant_NoPU  ->Write();
  hcutbasedDiscriminant_NoPU->Write();

  hjetPt_PU        -> Write();
  hjetEta_PU       -> Write();
  hleadFrac_PU     -> Write();
  hleadNeutFrac_PU -> Write();
  hleadEmFrac_PU   -> Write();
  hleadChFrac_PU   -> Write();
  hnParticles_PU   -> Write();
  hnCharged_PU     -> Write();
  hnNeutrals_PU    -> Write();
  hdR2Mean_PU      -> Write();
  hdRMean_PU       -> Write();
  hdRMeanNeut_PU   -> Write();
  hdRMeanEm_PU     -> Write();
  hdRMeanCh_PU     -> Write();
  hdRLeadCent_PU   -> Write();
  hfrac01_PU       -> Write();
  hchFrac01_PU     -> Write();
  hemFrac01_PU     -> Write();
  hneutFrac01_PU   -> Write();
  hfrac02_PU       -> Write();
  hchFrac02_PU     -> Write();
  hemFrac02_PU     -> Write();
  hneutFrac02_PU   -> Write();
  hfrac03_PU       -> Write();
  hchFrac03_PU     -> Write();
  hemFrac03_PU     -> Write();
  hneutFrac03_PU   -> Write();
  hfrac04_PU       -> Write();
  hchFrac04_PU     -> Write();
  hemFrac04_PU     -> Write();
  hneutFrac04_PU   -> Write();
  hfrac05_PU       -> Write();
  hchFrac05_PU     -> Write();
  hemFrac05_PU     -> Write();
  hneutFrac05_PU   -> Write();
  hptD_PU          -> Write();
  hetaW_PU         -> Write();
  hphiW_PU         -> Write();
  hmajW_PU         -> Write();
  hminW_PU         -> Write();
  hbeta_PU         -> Write();
  hbetaStar_PU     -> Write();
  hmva_PU          -> Write();
  hsimpleDiscriminant_PU->Write();
  hfullDiscriminant_PU  ->Write();
  hcutbasedDiscriminant_PU->Write();



  TDirectory *efficiency  =   outputRootFile -> mkdir("efficiency","efficiency");
  efficiency->cd();

  hPtRatio_matched_noId -> Write();   
  hJetPt_matched_noId -> Write();   
  hJetEta_matched_noId -> Write();   
  hNumberOfVertices_matched_noId -> Write();   

  hPtRatio_vs_Dphi ->Write();
  hPtRatio_noId -> Write();   
  hJetPt_noId  -> Write();   
  hJetEta_noId -> Write();   
  hNumberOfVertices_noId -> Write();   
  
  for ( int iid = 0; iid < 3 ; iid++ ){
    for (int ilevel = 0; ilevel < 3; ilevel++){
      if ( dataFlag_==0 ){
	hPtRatio_matched[iid][ilevel]->Write();   
	hJetPt_matched[iid][ilevel]->Write();   
	hJetEta_matched[iid][ilevel]->Write();   
	hNumberOfVertices_matched[iid][ilevel]->Write();   
	
	hEff_vs_PtRatio_matched[iid][ilevel]->Write(); 
	hEff_vs_JetPt_matched[iid][ilevel]->Write(); 
	hEff_vs_JetEta_matched[iid][ilevel]->Write(); 
	hEff_vs_NumberOfVertices_matched[iid][ilevel]->Write(); 
    }
      
      hPtRatio[iid][ilevel]->Write();   
      hJetPt[iid][ilevel]->Write();   
      hJetEta[iid][ilevel]->Write();   
      hNumberOfVertices[iid][ilevel]->Write();   
      
      hEff_vs_PtRatio[iid][ilevel]->Write(); 
      hEff_vs_JetPt[iid][ilevel]->Write(); 
      hEff_vs_JetEta[iid][ilevel]->Write(); 
      hEff_vs_NumberOfVertices[iid][ilevel]->Write(); 
    }
  }

  outputRootFile -> Close();
    
  return 0;


 }



 // 
