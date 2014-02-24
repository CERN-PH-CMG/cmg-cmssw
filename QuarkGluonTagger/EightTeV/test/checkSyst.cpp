#include <cstdlib>
#include <cmath>
#include "TH1D.h"
#include "../interface/QGSyst.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "DrawBase.h"
#include "fitTools.h"


bool doubleMin = true;
bool sunilTree = false;
bool use_herwig = false;
float fractionChange= +0.00;

std::string gSelection;


void drawSinglePlot( const std::string& selection, DrawBase* db, const std::string& discrim, TH1D* h1_data, TH1D* h1_qgl, TH1D* h1_qglSyst, float ptMin, float ptMax, float etaMin, float etaMax, float rhoMin, float rhoMax );
void drawEffVsPt( DrawBase* db, const std::string& discrim, float thresh1, const std::string xVarName, float etaMin, float etaMax, TH1D* h1_eff_gluon_thresh1, TH1D* h1_eff_gluon_syst_thresh1, TH1D* h1_eff_quark_thresh1, TH1D* h1_eff_quark_syst_thresh1, const std::string& suffix="" );
void drawMC_beforeAfter( DrawBase* db, const std::string& discrim, TH1D* h1_qglJet_quark, TH1D* h1_qglJetSyst_quark, TH1D* h1_qglJet_gluon, TH1D* h1_qglJetSyst_gluon, float ptMin, float ptMax, float etaMin, float etaMax, float rhoMin, float rhoMax );


int main( int argc, char* argv[] ) {


  if( argc==1 ) {
    std::cout << "USAGE: ./checkSyst [selection] [Discriminant=\"qglJet\"]" << std::endl;
    std::cout << "[selection] can be \"ZJet\" or \"DiJet\"" << std::endl;
    exit(1);
  }


  std::string selection = "ZJet";
  if( argc>1 ) {
    std::string selection_str(argv[1]);
    if( selection_str!="ZJet" && selection_str!="DiJet" ) {
      std::cout << "-> Unknown selection. Only \"ZJet\" and \"DiJet\" allowed. Exiting." << std::endl;
      exit(191);
    }
    selection = selection_str;
  }
	gSelection=selection; // propagate selection information through all the function as global var

  std::string discrim = "qglJet";
  if( argc>2 ) {
    std::string discrim_str(argv[2]);
    if( discrim_str!="qglJet" && discrim_str!="qgMLPJet" ) {
      std::cout << "-> Unknown Discriminant. Only \"qglJet\" and \"qgMLPJet\" allowed. Exiting." << std::endl;
      exit(191);
    }
    discrim = discrim_str;
  }



  std::string outfilename = "checkSyst_doubleMin";
  if( argc>3 ) {
    std::string doubleMin_str(argv[3]);
    if( doubleMin_str != "false" ) {
      doubleMin = true;
      std::cout << "-> Switching ON double min" << std::endl;
      outfilename = "checkSyst_doubleMin";
    } else {
      doubleMin = false;
      std::cout << "-> Switching OFF double min" << std::endl;
      outfilename = "checkSyst";
    }
  }
  outfilename = outfilename + "_" + selection + "_" + discrim + ".root";


  //TFile* file_eventWeights = TFile::Open("eventWeights.root");
  //TH1D* h1_eventWeights = (TH1D*)file_eventWeights->Get("eventWeights");


  //TFile* file = TFile::Open("/afs/cern.ch/user/a/amarini/work/GluonTag/ZJet/ZJet_DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1_2.root");
  std::string mcFileName;
  if( use_herwig )
    mcFileName = (selection=="ZJet") ? "sunilFlat_ZJet_Zjets_HppXXX.root" : "sunilFlat_DiJet_flatQCD_Hpp_Dijets_24Aug_ptHatWeight.root";
  else
    mcFileName = (selection=="ZJet") ? "sunilFlat_ZJet_Zjets_MGPy6_24Aug_skim.root" : "sunilFlat_DiJet_flatQCD_Py6_Dijets_24Aug_ptHatWeight.root";
  std::cout << "-> Opening MC file: " << mcFileName << std::endl;
  TFile* file = TFile::Open(mcFileName.c_str());
  TTree* tree = (TTree*)file->Get("tree_passedEvents");

  //TFile* file_data = TFile::Open("/afs/cern.ch/user/a/amarini/work/GluonTag/ZJet/ZJet_DoubleMu-Run2012AB.root");
  TChain* tree_data = new TChain("tree_passedEvents");
  //std::string dataFileName = (selection=="ZJet") ? "sunilFlat_ZJet_data2012ABCD_MuPD_12Jul.root" : "sunilFlat_DiJet_data2012ABCD_JetPD_12Jul.root";
  std::string dataFileName = (selection=="ZJet") ? "sunilFlat_ZJet_data2012ABCD_MuPD_24Aug_skim_new.root" : "sunilFlat_DiJet_data2012ABCD_MBPD_24Aug_new.root";
  std::cout << "-> Opening data file: " << dataFileName << std::endl;
  tree_data->Add(dataFileName.c_str());
  //tree_data->Add("/afs/cern.ch/user/a/amarini/work/GluonTag/ZJet/ZJet_Double*.root");
  //std::cout << "tot: " << tree_data->GetEntries() << std::endl;
  //TTree* tree_data = (TTree*)file_data->Get("tree_passedEvents");


  float eventWeight;
  int nvertex;
  int njet;
  float pt[20];
  float eta[20];
  float phi[20];
  int pdgId[20];
  float rho;
  float rhoMLP;
  int nCharged[20];
  int nNeutral[20];
  float ptD[20];
  float ptD_QC[20];
  float axis2_QC[20];
  float axis1_QC[20];
  float rmsCand_QC[20];
  float R[20];
  float pull_QC[20];
  int nCharged_QC[20];
  int nNeutral_ptCut[20];
  float qglJet[20];

  tree->SetBranchAddress("eventWeight", &eventWeight);
  tree->SetBranchAddress("nJet", &njet);
  tree->SetBranchAddress("ptJet", pt);
  tree->SetBranchAddress("etaJet", eta);
  tree->SetBranchAddress("pdgIdJet", pdgId);
  tree->SetBranchAddress("rhoPF", &rho);
  tree->SetBranchAddress("nvertex", &nvertex);
  tree->SetBranchAddress("nChargedJet", nCharged);
  tree->SetBranchAddress("nNeutralJet", nNeutral);
  tree->SetBranchAddress("ptDJet", ptD);
  tree->SetBranchAddress("ptD_QCJet", ptD_QC);
  tree->SetBranchAddress("axis1_QCJet", axis1_QC);
  tree->SetBranchAddress("axis2_QCJet", axis2_QC);
  tree->SetBranchAddress("rmsCand_QCJet", rmsCand_QC);
  tree->SetBranchAddress("RJet", R);
  tree->SetBranchAddress("pull_QCJet", pull_QC);
  tree->SetBranchAddress("nChg_QCJet", nCharged_QC);
  tree->SetBranchAddress("nNeutral_ptCutJet", nNeutral_ptCut);
  tree->SetBranchAddress(discrim.c_str(), qglJet);



  std::string systDatabaseName = "SystDatabase";
  //std::string systDatabaseName = "SystDiJetTP_2013_08_21_bugfix";
  //std::string systDatabaseName = "Syst_provaAll6580";
  //std::string systDatabaseName = "ZJetHbb_2013_07_23_res"; SystDiJetTPHerwig_2013_08_24_pandolfDir.txt  SystDiJetTP_2013_08_24_pandolfDir.txt
//  systDatabaseName = "SystDiJetTP_2013_08_24_pandolfDir2"
  //systDatabaseName = "SystDiJetTP_2013_08_24_24Aug";
  //systDatabaseName = "SystDiJetTP_2013_08_25_25Aug";

  std::string systDB_fullPath = "../data/" + systDatabaseName;
  if( use_herwig ) {systDB_fullPath += "_Hpp";
			//systDB_fullPath="../data/"+systDatabaseName;
			}
  systDB_fullPath += ".txt";
  
  std::cout << "-> Loading syst DB file: " << systDB_fullPath << std::endl;
  QGSyst qgsyst;
  qgsyst.ReadDatabaseDoubleMin(systDB_fullPath);


  std::string nameForSyst = (discrim=="qglJet") ? "QGLHisto" : "QGLMLP";
  qgsyst.SetTagger(nameForSyst);



  TFile* outfile = TFile::Open(outfilename.c_str(), "RECREATE");
  outfile->cd(); 

  TH1D* h1_ptZ = new TH1D("ptZ", "", 100, 0., 100.);
  h1_ptZ->Sumw2();
  TH1D* h1_rho = new TH1D("rho", "", 40, 0., 40.);
  h1_rho->Sumw2();


  float xMin = 0.;

  int nBins = 25;
  int nBinsFwd = 25;



  // CENTRAL

  TH1D* h1_qglJet_pt2030_eta02_rho040 = new TH1D("qglJet_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt2030_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt2030_eta02_rho040 = new TH1D("qglJetSyst_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt2030_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt2030_eta02_rho040 = new TH1D("qglJet_quark_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt2030_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt2030_eta02_rho040 = new TH1D("qglJetSyst_quark_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt2030_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt2030_eta02_rho040 = new TH1D("qglJet_gluon_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt2030_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt2030_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt2030_eta02_rho040->Sumw2();


  TH1D* h1_qglJet_pt3040_eta02_rho040 = new TH1D("qglJet_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt3040_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt3040_eta02_rho040 = new TH1D("qglJetSyst_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt3040_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt3040_eta02_rho040 = new TH1D("qglJet_quark_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt3040_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt3040_eta02_rho040 = new TH1D("qglJetSyst_quark_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt3040_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt3040_eta02_rho040 = new TH1D("qglJet_gluon_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt3040_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt3040_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt3040_eta02_rho040->Sumw2();


  TH1D* h1_qglJet_pt4050_eta02_rho040 = new TH1D("qglJet_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt4050_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt4050_eta02_rho040 = new TH1D("qglJetSyst_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt4050_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt4050_eta02_rho040 = new TH1D("qglJet_quark_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt4050_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt4050_eta02_rho040 = new TH1D("qglJetSyst_quark_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt4050_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt4050_eta02_rho040 = new TH1D("qglJet_gluon_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt4050_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt4050_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt4050_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt5065_eta02_rho040 = new TH1D("qglJet_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt5065_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt5065_eta02_rho040 = new TH1D("qglJetSyst_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt5065_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt5065_eta02_rho040 = new TH1D("qglJet_quark_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt5065_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt5065_eta02_rho040 = new TH1D("qglJetSyst_quark_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt5065_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt5065_eta02_rho040 = new TH1D("qglJet_gluon_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt5065_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt5065_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt5065_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt6580_eta02_rho040 = new TH1D("qglJet_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt6580_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt6580_eta02_rho040 = new TH1D("qglJetSyst_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt6580_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt6580_eta02_rho040 = new TH1D("qglJet_quark_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt6580_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt6580_eta02_rho040 = new TH1D("qglJetSyst_quark_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt6580_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt6580_eta02_rho040 = new TH1D("qglJet_gluon_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt6580_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt6580_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt6580_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt80100_eta02_rho040 = new TH1D("qglJet_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt80100_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt80100_eta02_rho040 = new TH1D("qglJetSyst_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt80100_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt80100_eta02_rho040 = new TH1D("qglJet_quark_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt80100_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt80100_eta02_rho040 = new TH1D("qglJetSyst_quark_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt80100_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt80100_eta02_rho040 = new TH1D("qglJet_gluon_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt80100_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt80100_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt80100_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt100250_eta02_rho040 = new TH1D("qglJet_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt100250_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt100250_eta02_rho040 = new TH1D("qglJetSyst_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt100250_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt100250_eta02_rho040 = new TH1D("qglJet_quark_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt100250_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt100250_eta02_rho040 = new TH1D("qglJetSyst_quark_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt100250_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt100250_eta02_rho040 = new TH1D("qglJet_gluon_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt100250_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt100250_eta02_rho040 = new TH1D("qglJetSyst_gluon_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt100250_eta02_rho040->Sumw2();




  // TRANSITION 

  TH1D* h1_qglJet_pt2030_eta23_rho040 = new TH1D("qglJet_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt2030_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt2030_eta23_rho040 = new TH1D("qglJetSyst_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt2030_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt2030_eta23_rho040 = new TH1D("qglJet_quark_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt2030_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt2030_eta23_rho040 = new TH1D("qglJetSyst_quark_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt2030_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt2030_eta23_rho040 = new TH1D("qglJet_gluon_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt2030_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt2030_eta23_rho040 = new TH1D("qglJetSyst_gluon_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt2030_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_pt4050_eta23_rho040 = new TH1D("qglJet_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt4050_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt4050_eta23_rho040 = new TH1D("qglJetSyst_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt4050_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt4050_eta23_rho040 = new TH1D("qglJet_quark_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt4050_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt4050_eta23_rho040 = new TH1D("qglJetSyst_quark_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt4050_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt4050_eta23_rho040 = new TH1D("qglJet_gluon_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt4050_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt4050_eta23_rho040 = new TH1D("qglJetSyst_gluon_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt4050_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_pt5065_eta23_rho040 = new TH1D("qglJet_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt5065_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt5065_eta23_rho040 = new TH1D("qglJetSyst_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt5065_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt5065_eta23_rho040 = new TH1D("qglJet_quark_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt5065_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt5065_eta23_rho040 = new TH1D("qglJetSyst_quark_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt5065_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt5065_eta23_rho040 = new TH1D("qglJet_gluon_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt5065_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt5065_eta23_rho040 = new TH1D("qglJetSyst_gluon_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt5065_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_pt6580_eta23_rho040 = new TH1D("qglJet_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt6580_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt6580_eta23_rho040 = new TH1D("qglJetSyst_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt6580_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt6580_eta23_rho040 = new TH1D("qglJet_quark_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt6580_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt6580_eta23_rho040 = new TH1D("qglJetSyst_quark_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt6580_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt6580_eta23_rho040 = new TH1D("qglJet_gluon_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt6580_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt6580_eta23_rho040 = new TH1D("qglJetSyst_gluon_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt6580_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_pt80100_eta23_rho040 = new TH1D("qglJet_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt80100_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt80100_eta23_rho040 = new TH1D("qglJetSyst_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt80100_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt80100_eta23_rho040 = new TH1D("qglJet_quark_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt80100_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt80100_eta23_rho040 = new TH1D("qglJetSyst_quark_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt80100_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt80100_eta23_rho040 = new TH1D("qglJet_gluon_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt80100_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt80100_eta23_rho040 = new TH1D("qglJetSyst_gluon_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt80100_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_pt100250_eta23_rho040 = new TH1D("qglJet_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_pt100250_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt100250_eta23_rho040 = new TH1D("qglJetSyst_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_pt100250_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt100250_eta23_rho040 = new TH1D("qglJet_quark_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_quark_pt100250_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt100250_eta23_rho040 = new TH1D("qglJetSyst_quark_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_quark_pt100250_eta23_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt100250_eta23_rho040 = new TH1D("qglJet_gluon_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJet_gluon_pt100250_eta23_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt100250_eta23_rho040 = new TH1D("qglJetSyst_gluon_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt100250_eta23_rho040->Sumw2();



  // FORWARD

  TH1D* h1_qglJet_pt2030_eta35_rho040 = new TH1D("qglJet_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt2030_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt2030_eta35_rho040 = new TH1D("qglJetSyst_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt2030_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt2030_eta35_rho040 = new TH1D("qglJet_quark_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt2030_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt2030_eta35_rho040 = new TH1D("qglJetSyst_quark_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt2030_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt2030_eta35_rho040 = new TH1D("qglJet_gluon_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt2030_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt2030_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt2030_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt3040_eta35_rho040 = new TH1D("qglJet_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt3040_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt3040_eta35_rho040 = new TH1D("qglJetSyst_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt3040_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt3040_eta35_rho040 = new TH1D("qglJet_quark_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt3040_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt3040_eta35_rho040 = new TH1D("qglJetSyst_quark_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt3040_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt3040_eta35_rho040 = new TH1D("qglJet_gluon_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt3040_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt3040_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt3040_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt4050_eta35_rho040 = new TH1D("qglJet_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt4050_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt4050_eta35_rho040 = new TH1D("qglJetSyst_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt4050_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt4050_eta35_rho040 = new TH1D("qglJet_quark_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt4050_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt4050_eta35_rho040 = new TH1D("qglJetSyst_quark_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt4050_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt4050_eta35_rho040 = new TH1D("qglJet_gluon_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt4050_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt4050_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt4050_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt5065_eta35_rho040 = new TH1D("qglJet_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt5065_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt5065_eta35_rho040 = new TH1D("qglJetSyst_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt5065_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt5065_eta35_rho040 = new TH1D("qglJet_quark_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt5065_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt5065_eta35_rho040 = new TH1D("qglJetSyst_quark_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt5065_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt5065_eta35_rho040 = new TH1D("qglJet_gluon_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt5065_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt5065_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt5065_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt6580_eta35_rho040 = new TH1D("qglJet_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt6580_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt6580_eta35_rho040 = new TH1D("qglJetSyst_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt6580_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt6580_eta35_rho040 = new TH1D("qglJet_quark_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt6580_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt6580_eta35_rho040 = new TH1D("qglJetSyst_quark_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt6580_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt6580_eta35_rho040 = new TH1D("qglJet_gluon_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt6580_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt6580_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt6580_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt80100_eta35_rho040 = new TH1D("qglJet_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt80100_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt80100_eta35_rho040 = new TH1D("qglJetSyst_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt80100_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt80100_eta35_rho040 = new TH1D("qglJet_quark_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt80100_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt80100_eta35_rho040 = new TH1D("qglJetSyst_quark_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt80100_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt80100_eta35_rho040 = new TH1D("qglJet_gluon_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt80100_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt80100_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt80100_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt100250_eta35_rho040 = new TH1D("qglJet_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_pt100250_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt100250_eta35_rho040 = new TH1D("qglJetSyst_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_pt100250_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_quark_pt100250_eta35_rho040 = new TH1D("qglJet_quark_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_quark_pt100250_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_quark_pt100250_eta35_rho040 = new TH1D("qglJetSyst_quark_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_quark_pt100250_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_gluon_pt100250_eta35_rho040 = new TH1D("qglJet_gluon_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJet_gluon_pt100250_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_gluon_pt100250_eta35_rho040 = new TH1D("qglJetSyst_gluon_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_qglJetSyst_gluon_pt100250_eta35_rho040->Sumw2();





  //syst eff histos:
  float thresh1 = (discrim=="qgMLPJet") ? 0.6 : 0.5;

  int nbins_pt = 20;
  const int nbins_pt_plusOne(nbins_pt+1);
  Double_t ptBins[nbins_pt_plusOne];
  fitTools::getBins_int( nbins_pt_plusOne, ptBins, 30., 3000.);

  TH1D* h1_effDenom_centr_quark_thresh1 = new TH1D("effDenom_centr_quark_thresh1", "", nbins_pt, ptBins);
  TH1D* h1_effDenom_uw_centr_quark_thresh1 = new TH1D("effDenom_uw_centr_quark_thresh1", "", nbins_pt, ptBins);
  TH1D* h1_effNum_centr_quark_thresh1   = new TH1D("effNum_centr_quark_thresh1", "", nbins_pt, ptBins)  ;

  TH1D* h1_effNum_centr_quark_syst_thresh1   = new TH1D("effNum_centr_quark_syst_thresh1", "", nbins_pt, ptBins)  ;
  TH1D* h1_effDenom_centr_gluon_thresh1 = new TH1D("effDenom_centr_gluon_thresh1", "", nbins_pt, ptBins);
  TH1D* h1_effDenom_uw_centr_gluon_thresh1 = new TH1D("effDenom_uw_centr_gluon_thresh1", "", nbins_pt, ptBins);

  TH1D* h1_effNum_centr_gluon_thresh1   = new TH1D("effNum_centr_gluon_thresh1", "", nbins_pt, ptBins)  ;
  TH1D* h1_effNum_centr_gluon_syst_thresh1   = new TH1D("effNum_centr_gluon_syst_thresh1", "", nbins_pt, ptBins)  ;

                                                             

  TH1D* h1_effDenom_trans_quark_thresh1 = new TH1D("effDenom_trans_quark_thresh1", "", nbins_pt, ptBins);
  TH1D* h1_effDenom_uw_trans_quark_thresh1 = new TH1D("effDenom_uw_trans_quark_thresh1", "", nbins_pt, ptBins);
  TH1D* h1_effNum_trans_quark_thresh1   = new TH1D("effNum_trans_quark_thresh1", "", nbins_pt, ptBins)  ;

  TH1D* h1_effNum_trans_quark_syst_thresh1   = new TH1D("effNum_trans_quark_syst_thresh1", "", nbins_pt, ptBins)  ;
  TH1D* h1_effDenom_trans_gluon_thresh1 = new TH1D("effDenom_trans_gluon_thresh1", "", nbins_pt, ptBins);
  TH1D* h1_effDenom_uw_trans_gluon_thresh1 = new TH1D("effDenom_uw_trans_gluon_thresh1", "", nbins_pt, ptBins);

  TH1D* h1_effNum_trans_gluon_thresh1   = new TH1D("effNum_trans_gluon_thresh1", "", nbins_pt, ptBins)  ;
  TH1D* h1_effNum_trans_gluon_syst_thresh1   = new TH1D("effNum_trans_gluon_syst_thresh1", "", nbins_pt, ptBins)  ;

                                                             
  Double_t ptBins_fwd[nbins_pt_plusOne];
  fitTools::getBins_int( nbins_pt_plusOne, ptBins_fwd, 30., 3000.);

  TH1D* h1_effDenom_fwd_quark_thresh1 = new TH1D("effDenom_fwd_quark_thresh1", "", nbins_pt, ptBins_fwd);
  TH1D* h1_effDenom_uw_fwd_quark_thresh1 = new TH1D("effDenom_uw_fwd_quark_thresh1", "", nbins_pt, ptBins_fwd);
  TH1D* h1_effNum_fwd_quark_thresh1   = new TH1D("effNum_fwd_quark_thresh1", "", nbins_pt, ptBins_fwd)  ;

  TH1D* h1_effNum_fwd_quark_syst_thresh1   = new TH1D("effNum_fwd_quark_syst_thresh1", "", nbins_pt, ptBins_fwd)  ;
  TH1D* h1_effDenom_fwd_gluon_thresh1 = new TH1D("effDenom_fwd_gluon_thresh1", "", nbins_pt, ptBins_fwd);
  TH1D* h1_effDenom_uw_fwd_gluon_thresh1 = new TH1D("effDenom_uw_fwd_gluon_thresh1", "", nbins_pt, ptBins_fwd);

  TH1D* h1_effNum_fwd_gluon_thresh1   = new TH1D("effNum_fwd_gluon_thresh1", "", nbins_pt, ptBins_fwd)  ;
  TH1D* h1_effNum_fwd_gluon_syst_thresh1   = new TH1D("effNum_fwd_gluon_syst_thresh1", "", nbins_pt, ptBins_fwd)  ;
                                                             

  // and vs rho:

  int nbins_rho = 15;
  float rhoMax = 35.;

  TH1D* h1_effDenom_vs_rho_centr_quark_thresh1 = new TH1D("effDenom_vs_rho_centr_quark_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effDenom_uw_vs_rho_centr_quark_thresh1 = new TH1D("effDenom_uw_vs_rho_centr_quark_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effNum_vs_rho_centr_quark_thresh1   = new TH1D("effNum_vs_rho_centr_quark_thresh1", "", nbins_rho, 0., rhoMax)  ;

  TH1D* h1_effNum_vs_rho_centr_quark_syst_thresh1   = new TH1D("effNum_vs_rho_centr_quark_syst_thresh1", "", nbins_rho, 0., rhoMax)  ;
  TH1D* h1_effDenom_vs_rho_centr_gluon_thresh1 = new TH1D("effDenom_vs_rho_centr_gluon_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effDenom_uw_vs_rho_centr_gluon_thresh1 = new TH1D("effDenom_uw_vs_rho_centr_gluon_thresh1", "", nbins_rho, 0., rhoMax);

  TH1D* h1_effNum_vs_rho_centr_gluon_thresh1   = new TH1D("effNum_vs_rho_centr_gluon_thresh1", "", nbins_rho, 0., rhoMax)  ;
  TH1D* h1_effNum_vs_rho_centr_gluon_syst_thresh1   = new TH1D("effNum_vs_rho_centr_gluon_syst_thresh1", "", nbins_rho, 0., rhoMax)  ;

                                                             

  TH1D* h1_effDenom_vs_rho_trans_quark_thresh1 = new TH1D("effDenom_vs_rho_trans_quark_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effDenom_uw_vs_rho_trans_quark_thresh1 = new TH1D("effDenom_uw_vs_rho_trans_quark_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effNum_vs_rho_trans_quark_thresh1   = new TH1D("effNum_vs_rho_trans_quark_thresh1", "", nbins_rho, 0., rhoMax)  ;

  TH1D* h1_effNum_vs_rho_trans_quark_syst_thresh1   = new TH1D("effNum_vs_rho_trans_quark_syst_thresh1", "", nbins_rho, 0., rhoMax)  ;
  TH1D* h1_effDenom_vs_rho_trans_gluon_thresh1 = new TH1D("effDenom_vs_rho_trans_gluon_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effDenom_uw_vs_rho_trans_gluon_thresh1 = new TH1D("effDenom_uw_vs_rho_trans_gluon_thresh1", "", nbins_rho, 0., rhoMax);

  TH1D* h1_effNum_vs_rho_trans_gluon_thresh1   = new TH1D("effNum_vs_rho_trans_gluon_thresh1", "", nbins_rho, 0., rhoMax)  ;
  TH1D* h1_effNum_vs_rho_trans_gluon_syst_thresh1   = new TH1D("effNum_vs_rho_trans_gluon_syst_thresh1", "", nbins_rho, 0., rhoMax)  ;


  TH1D* h1_effDenom_vs_rho_fwd_quark_thresh1 = new TH1D("effDenom_vs_rho_fwd_quark_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effDenom_uw_vs_rho_fwd_quark_thresh1 = new TH1D("effDenom_uw_vs_rho_fwd_quark_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effNum_vs_rho_fwd_quark_thresh1   = new TH1D("effNum_vs_rho_fwd_quark_thresh1", "", nbins_rho, 0., rhoMax)  ;

  TH1D* h1_effNum_vs_rho_fwd_quark_syst_thresh1   = new TH1D("effNum_vs_rho_fwd_quark_syst_thresh1", "", nbins_rho, 0., rhoMax)  ;
  TH1D* h1_effDenom_vs_rho_fwd_gluon_thresh1 = new TH1D("effDenom_vs_rho_fwd_gluon_thresh1", "", nbins_rho, 0., rhoMax);
  TH1D* h1_effDenom_uw_vs_rho_fwd_gluon_thresh1 = new TH1D("effDenom_uw_vs_rho_fwd_gluon_thresh1", "", nbins_rho, 0., rhoMax);

  TH1D* h1_effNum_vs_rho_fwd_gluon_thresh1   = new TH1D("effNum_vs_rho_fwd_gluon_thresh1", "", nbins_rho, 0., rhoMax)  ;
  TH1D* h1_effNum_vs_rho_fwd_gluon_syst_thresh1   = new TH1D("effNum_vs_rho_fwd_gluon_syst_thresh1", "", nbins_rho, 0., rhoMax)  ;
                                                             


  // and vs nvertex:

  int nvertexMax = 40.5;
  int nvertexMin = 0.5;
  int nbins_nvertex = 10;
  TH1D* h1_effDenom_vs_nvertex_centr_quark_thresh1 = new TH1D("effDenom_vs_nvertex_centr_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effDenom_uw_vs_nvertex_centr_quark_thresh1 = new TH1D("effDenom_uw_vs_nvertex_centr_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effNum_vs_nvertex_centr_quark_thresh1   = new TH1D("effNum_vs_nvertex_centr_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;

  TH1D* h1_effNum_vs_nvertex_centr_quark_syst_thresh1   = new TH1D("effNum_vs_nvertex_centr_quark_syst_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;
  TH1D* h1_effDenom_vs_nvertex_centr_gluon_thresh1 = new TH1D("effDenom_vs_nvertex_centr_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effDenom_uw_vs_nvertex_centr_gluon_thresh1 = new TH1D("effDenom_uw_vs_nvertex_centr_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);

  TH1D* h1_effNum_vs_nvertex_centr_gluon_thresh1   = new TH1D("effNum_vs_nvertex_centr_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;
  TH1D* h1_effNum_vs_nvertex_centr_gluon_syst_thresh1   = new TH1D("effNum_vs_nvertex_centr_gluon_syst_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;

                                                             

  TH1D* h1_effDenom_vs_nvertex_trans_quark_thresh1 = new TH1D("effDenom_vs_nvertex_trans_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effDenom_uw_vs_nvertex_trans_quark_thresh1 = new TH1D("effDenom_uw_vs_nvertex_trans_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effNum_vs_nvertex_trans_quark_thresh1   = new TH1D("effNum_vs_nvertex_trans_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;

  TH1D* h1_effNum_vs_nvertex_trans_quark_syst_thresh1   = new TH1D("effNum_vs_nvertex_trans_quark_syst_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;
  TH1D* h1_effDenom_vs_nvertex_trans_gluon_thresh1 = new TH1D("effDenom_vs_nvertex_trans_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effDenom_uw_vs_nvertex_trans_gluon_thresh1 = new TH1D("effDenom_uw_vs_nvertex_trans_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);

  TH1D* h1_effNum_vs_nvertex_trans_gluon_thresh1   = new TH1D("effNum_vs_nvertex_trans_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;
  TH1D* h1_effNum_vs_nvertex_trans_gluon_syst_thresh1   = new TH1D("effNum_vs_nvertex_trans_gluon_syst_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;

                                                             

  TH1D* h1_effDenom_vs_nvertex_fwd_quark_thresh1 = new TH1D("effDenom_vs_nvertex_fwd_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effDenom_uw_vs_nvertex_fwd_quark_thresh1 = new TH1D("effDenom_uw_vs_nvertex_fwd_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effNum_vs_nvertex_fwd_quark_thresh1   = new TH1D("effNum_vs_nvertex_fwd_quark_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;

  TH1D* h1_effNum_vs_nvertex_fwd_quark_syst_thresh1   = new TH1D("effNum_vs_nvertex_fwd_quark_syst_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;
  TH1D* h1_effDenom_vs_nvertex_fwd_gluon_thresh1 = new TH1D("effDenom_vs_nvertex_fwd_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);
  TH1D* h1_effDenom_uw_vs_nvertex_fwd_gluon_thresh1 = new TH1D("effDenom_uw_vs_nvertex_fwd_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax);

  TH1D* h1_effNum_vs_nvertex_fwd_gluon_thresh1   = new TH1D("effNum_vs_nvertex_fwd_gluon_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;
  TH1D* h1_effNum_vs_nvertex_fwd_gluon_syst_thresh1   = new TH1D("effNum_vs_nvertex_fwd_gluon_syst_thresh1", "", nbins_nvertex, nvertexMin, nvertexMax)  ;


                                                             


  int nentries = tree->GetEntries();

  for( unsigned iEntry = 0; iEntry < nentries; ++iEntry ) {

    tree->GetEntry(iEntry);

    if( njet<1 ) continue;
    if( selection=="DiJet" && njet<2 ) continue;

    //if( rho>40. ) continue;
    //if( mZ<70. || mZ>100. ) continue;
    //if( deltaPhi_jet < 3.1415-0.5 ) continue;
    //if( ptD_QCJet <=0. ) continue;
    //if( fabs(etaJet)<2.5 && betaStarJet > 0.2 * log( (float)nvertex - 0.67) ) continue;
    //if( btagged ) continue;

    //float eventWeight = eventWeight;

    //h1_ptZ->Fill( ptZ, eventWeight );
    h1_rho->Fill( rho, eventWeight );

    bool smear = true;
    std::vector<std::string> type;
    if( doubleMin ) {
      if( fabs(pdgId[0])>0 && fabs(pdgId[0])<6 ) type.push_back( "quark" );
      else if( pdgId[0]==21 ) type.push_back("gluon");
      else { // both 0 and -999
        //smearJet = false;
        smear=false;
        type.push_back("undef");
        //std::cout << "Unknown jet PDG ID (" << pdgIdPartJet << "). Will use gluon instead." << std::endl;
      }
    }


    std::vector<float> qglJetSyst;
    if( smear )
      qglJetSyst.push_back(qgsyst.Smear(pt[0], eta[0], rho, qglJet[0], type[0]));
    else
      qglJetSyst.push_back(qglJet[0]);

    std::vector<bool> discrimOK_thresh1;
    discrimOK_thresh1.push_back((discrim=="qglJet" && qglJet[0]>thresh1) || (discrim=="qgMLPJet" && qglJet[0]<thresh1));
    std::vector<bool> discrimSystOK_thresh1;
    discrimSystOK_thresh1.push_back((discrim=="qglJet" && qglJetSyst[0]>thresh1) || (discrim=="qgMLPJet" && qglJetSyst[0]<thresh1));

    if( selection=="DiJet" ) {

      bool smear2 = true;
      if( doubleMin ) {
        if( fabs(pdgId[1])>0 && fabs(pdgId[1])<6 ) type.push_back("quark");
        else if( pdgId[1]==21 ) type.push_back("gluon");
        else { // both 0 and -999
          //smearJet = false;
          smear2 = false;
          type.push_back("undef");
          //std::cout << "Unknown jet PDG ID (" << pdgIdPartJet << "). Will use gluon instead." << std::endl;
        }
      }

      if( smear2 ) {
        qglJetSyst.push_back(qgsyst.Smear(pt[1], eta[1], rho, qglJet[1], type[1]));
      } else {
        qglJetSyst.push_back(qglJet[1]);
      }
      discrimOK_thresh1.push_back((discrim=="qglJet" && qglJet[1]>thresh1) || (discrim=="qgMLPJet" && qglJet[1]<thresh1));
      discrimSystOK_thresh1.push_back((discrim=="qglJet" && qglJetSyst[1]>thresh1) || (discrim=="qgMLPJet" && qglJetSyst[1]<thresh1));
    }

	float ew=eventWeight;

    for( unsigned ijet=0; ijet<qglJetSyst.size(); ijet++ ) {

      if( ijet>1 ) break; //shouldnt be possible

      int tag_index = ijet;
      int probe_index = qglJetSyst.size()-ijet-1;
	if(type[probe_index]=="quark")
		eventWeight=ew*(1+fractionChange);
	else
		eventWeight=ew;


      if( fabs(eta[probe_index])<2. ) {


        if( type[probe_index]=="quark" ) {

          h1_effDenom_centr_quark_thresh1->Fill( pt[tag_index], eventWeight );
          h1_effDenom_uw_centr_quark_thresh1->Fill( pt[tag_index], 1 );
          if( discrimOK_thresh1[probe_index] ) h1_effNum_centr_quark_thresh1->Fill( pt[tag_index], eventWeight );
          if( discrimSystOK_thresh1[probe_index] ) h1_effNum_centr_quark_syst_thresh1->Fill( pt[tag_index], eventWeight );

          if( pt[tag_index]>30. ) {
            h1_effDenom_vs_rho_centr_quark_thresh1->Fill( rho, eventWeight );
            h1_effDenom_uw_vs_rho_centr_quark_thresh1->Fill( rho, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_rho_centr_quark_thresh1->Fill( rho, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_rho_centr_quark_syst_thresh1->Fill( rho, eventWeight );

            h1_effDenom_vs_nvertex_centr_quark_thresh1->Fill( nvertex, eventWeight );
            h1_effDenom_uw_vs_nvertex_centr_quark_thresh1->Fill( nvertex, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_centr_quark_thresh1->Fill( nvertex, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_centr_quark_syst_thresh1->Fill( nvertex, eventWeight );
          }

        } else if( type[probe_index]=="gluon" ) {

          h1_effDenom_centr_gluon_thresh1->Fill( pt[tag_index], eventWeight );
          h1_effDenom_uw_centr_gluon_thresh1->Fill( pt[tag_index], 1 );
          if( discrimOK_thresh1[probe_index] ) h1_effNum_centr_gluon_thresh1->Fill( pt[tag_index], eventWeight );
          if( discrimSystOK_thresh1[probe_index] ) h1_effNum_centr_gluon_syst_thresh1->Fill( pt[tag_index], eventWeight );

          if( pt[tag_index]>30. ) {
            h1_effDenom_vs_rho_centr_gluon_thresh1->Fill( rho, eventWeight );
            h1_effDenom_uw_vs_rho_centr_gluon_thresh1->Fill( rho, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_rho_centr_gluon_thresh1->Fill( rho, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_rho_centr_gluon_syst_thresh1->Fill( rho, eventWeight );

            h1_effDenom_vs_nvertex_centr_gluon_thresh1->Fill( nvertex, eventWeight );
            h1_effDenom_uw_vs_nvertex_centr_gluon_thresh1->Fill( nvertex, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_centr_gluon_thresh1->Fill( nvertex, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_centr_gluon_syst_thresh1->Fill( nvertex, eventWeight );
          }

        }




        if( pt[tag_index] > 20. && pt[tag_index] < 30. ) {

          h1_qglJet_pt2030_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt2030_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt2030_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt2030_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt2030_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt2030_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 30. && pt[tag_index] < 40. ) {

          h1_qglJet_pt3040_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt3040_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt3040_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt3040_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt3040_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt3040_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 40. && pt[tag_index] < 50. ) {

          h1_qglJet_pt4050_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt4050_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt4050_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt4050_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt4050_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt4050_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 50. && pt[tag_index] < 65. ) {

          h1_qglJet_pt5065_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt5065_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt5065_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt5065_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt5065_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt5065_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 65. && pt[tag_index] < 80. ) {

          h1_qglJet_pt6580_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt6580_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt6580_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt6580_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt6580_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt6580_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 80. && pt[tag_index] < 100. ) {

          h1_qglJet_pt80100_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt80100_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt80100_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt80100_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt80100_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt80100_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 100. && pt[tag_index] < 250. ) {

          h1_qglJet_pt100250_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt100250_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt100250_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt100250_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt100250_eta02_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt100250_eta02_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } 

      } else if( fabs(eta[probe_index])<3. ) {


        if( type[probe_index]=="quark" ) {

          h1_effDenom_trans_quark_thresh1->Fill( pt[tag_index], eventWeight );
          h1_effDenom_uw_trans_quark_thresh1->Fill( pt[tag_index], 1 );
          if( discrimOK_thresh1[probe_index] ) h1_effNum_trans_quark_thresh1->Fill( pt[tag_index], eventWeight );
          if( discrimSystOK_thresh1[probe_index] ) h1_effNum_trans_quark_syst_thresh1->Fill( pt[tag_index], eventWeight );

          if( pt[tag_index]>30. ) {
            h1_effDenom_vs_rho_trans_quark_thresh1->Fill( rho, eventWeight );
            h1_effDenom_uw_vs_rho_trans_quark_thresh1->Fill( rho, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_rho_trans_quark_thresh1->Fill( rho, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_rho_trans_quark_syst_thresh1->Fill( rho, eventWeight );

            h1_effDenom_vs_nvertex_trans_quark_thresh1->Fill( nvertex, eventWeight );
            h1_effDenom_uw_vs_nvertex_trans_quark_thresh1->Fill( nvertex, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_trans_quark_thresh1->Fill( nvertex, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_trans_quark_syst_thresh1->Fill( nvertex, eventWeight );
          }

        } else if( type[probe_index]=="gluon" ) {

          h1_effDenom_trans_gluon_thresh1->Fill( pt[tag_index], eventWeight );
          h1_effDenom_uw_trans_gluon_thresh1->Fill( pt[tag_index], 1 );
          if( discrimOK_thresh1[probe_index] ) h1_effNum_trans_gluon_thresh1->Fill( pt[tag_index], eventWeight );
          if( discrimSystOK_thresh1[probe_index] ) h1_effNum_trans_gluon_syst_thresh1->Fill( pt[tag_index], eventWeight );

          if( pt[tag_index]>30. ) {
            h1_effDenom_vs_rho_trans_gluon_thresh1->Fill( rho, eventWeight );
            h1_effDenom_uw_vs_rho_trans_gluon_thresh1->Fill( rho, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_rho_trans_gluon_thresh1->Fill( rho, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_rho_trans_gluon_syst_thresh1->Fill( rho, eventWeight );

            h1_effDenom_vs_nvertex_trans_gluon_thresh1->Fill( nvertex, eventWeight );
            h1_effDenom_uw_vs_nvertex_trans_gluon_thresh1->Fill( nvertex, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_trans_gluon_thresh1->Fill( nvertex, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_trans_gluon_syst_thresh1->Fill( nvertex, eventWeight );
          }

        }


        if( pt[tag_index] > 20. && pt[tag_index] < 30. ) {

          h1_qglJet_pt2030_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt2030_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt2030_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt2030_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt2030_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt2030_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 40. && pt[tag_index] < 50. ) {

          h1_qglJet_pt4050_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt4050_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt4050_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt4050_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt4050_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt4050_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 50. && pt[tag_index] < 65. ) {

          h1_qglJet_pt5065_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt5065_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt5065_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt5065_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt5065_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt5065_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 65. && pt[tag_index] < 80. ) {

          h1_qglJet_pt6580_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt6580_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt6580_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt6580_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt6580_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt6580_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 80. && pt[tag_index] < 100. ) {

          h1_qglJet_pt80100_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt80100_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt80100_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt80100_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt80100_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt80100_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 100. && pt[tag_index] < 250. ) {

          h1_qglJet_pt100250_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt100250_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt100250_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt100250_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt100250_eta23_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt100250_eta23_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } 

      } else if( fabs(eta[probe_index])>3. && fabs(eta[probe_index])<4.7 ) {

        if( type[probe_index]=="quark" ) {

          h1_effDenom_fwd_quark_thresh1->Fill( pt[tag_index], eventWeight );
          h1_effDenom_uw_fwd_quark_thresh1->Fill( pt[tag_index], 1 );
          if( discrimOK_thresh1[probe_index] ) h1_effNum_fwd_quark_thresh1->Fill( pt[tag_index], eventWeight );
          if( discrimSystOK_thresh1[probe_index] ) h1_effNum_fwd_quark_syst_thresh1->Fill( pt[tag_index], eventWeight );

          if( pt[tag_index]>30. ) {
            h1_effDenom_vs_rho_fwd_quark_thresh1->Fill( rho, eventWeight );
            h1_effDenom_uw_vs_rho_fwd_quark_thresh1->Fill( rho, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_rho_fwd_quark_thresh1->Fill( rho, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_rho_fwd_quark_syst_thresh1->Fill( rho, eventWeight );

            h1_effDenom_vs_nvertex_fwd_quark_thresh1->Fill( nvertex, eventWeight );
            h1_effDenom_uw_vs_nvertex_fwd_quark_thresh1->Fill( nvertex, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_fwd_quark_thresh1->Fill( nvertex, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_fwd_quark_syst_thresh1->Fill( nvertex, eventWeight );
          }

        } else if( type[probe_index]=="gluon" ) {

          h1_effDenom_fwd_gluon_thresh1->Fill( pt[tag_index], eventWeight );
          h1_effDenom_uw_fwd_gluon_thresh1->Fill( pt[tag_index], 1 );
          if( discrimOK_thresh1[probe_index] ) h1_effNum_fwd_gluon_thresh1->Fill( pt[tag_index], eventWeight );
          if( discrimSystOK_thresh1[probe_index] ) h1_effNum_fwd_gluon_syst_thresh1->Fill( pt[tag_index], eventWeight );

          if( pt[tag_index]>30. ) {
            h1_effDenom_vs_rho_fwd_gluon_thresh1->Fill( rho, eventWeight );
            h1_effDenom_uw_vs_rho_fwd_gluon_thresh1->Fill( rho, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_rho_fwd_gluon_thresh1->Fill( rho, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_rho_fwd_gluon_syst_thresh1->Fill( rho, eventWeight );

            h1_effDenom_vs_nvertex_fwd_gluon_thresh1->Fill( nvertex, eventWeight );
            h1_effDenom_uw_vs_nvertex_fwd_gluon_thresh1->Fill( nvertex, 1 );
            if( discrimOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_fwd_gluon_thresh1->Fill( nvertex, eventWeight );
            if( discrimSystOK_thresh1[probe_index] ) h1_effNum_vs_nvertex_fwd_gluon_syst_thresh1->Fill( nvertex, eventWeight );
          }

        }

        if( pt[tag_index] > 20. && pt[tag_index] < 30. ) {

          h1_qglJet_pt2030_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt2030_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt2030_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt2030_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt2030_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt2030_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 30. && pt[tag_index] < 40. ) {

          h1_qglJet_pt3040_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt3040_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt3040_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt3040_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt3040_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt3040_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 40. && pt[tag_index] < 50. ) {

          h1_qglJet_pt4050_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt4050_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt4050_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt4050_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt4050_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt4050_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 50. && pt[tag_index] < 65. ) {

          h1_qglJet_pt5065_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt5065_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt5065_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt5065_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt5065_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt5065_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 65. && pt[tag_index] < 80. ) {

          h1_qglJet_pt6580_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt6580_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt6580_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt6580_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt6580_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt6580_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 80. && pt[tag_index] < 100. ) {

          h1_qglJet_pt80100_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt80100_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt80100_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt80100_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt80100_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt80100_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } else if( pt[tag_index] > 100. && pt[tag_index] < 250. ) {

          h1_qglJet_pt100250_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
          h1_qglJetSyst_pt100250_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          if( type[probe_index]=="quark" ) {
            h1_qglJet_quark_pt100250_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_quark_pt100250_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          } else if( type[probe_index]=="gluon" ) {
            h1_qglJet_gluon_pt100250_eta35_rho040->Fill( qglJet[probe_index], eventWeight );
            h1_qglJetSyst_gluon_pt100250_eta35_rho040->Fill( qglJetSyst[probe_index], eventWeight );
          }

        } 

      } // eta

    } //for tag and probe

  }


  // now switch to data:
  int event;

  tree_data->SetBranchAddress("event", &event );
  tree_data->SetBranchAddress("nJet", &njet);
  tree_data->SetBranchAddress("ptJet", pt);
  tree_data->SetBranchAddress("etaJet", eta);
  tree_data->SetBranchAddress("rhoPF", &rho);
  tree_data->SetBranchAddress("ptD_QCJet", ptD_QC);
  tree_data->SetBranchAddress(discrim.c_str(), qglJet);


  TH1D* h1_data_ptZ = new TH1D("data_ptZ", "", 100, 0., 100.);
  h1_data_ptZ->Sumw2();
  TH1D* h1_data_rho = new TH1D("data_rho", "", 40, 0., 40.);
  h1_data_rho->Sumw2();

  TH1D* h1_data_qglJet_pt2030_eta02_rho040 = new TH1D("data_qglJet_pt2030_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt2030_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt3040_eta02_rho040 = new TH1D("data_qglJet_pt3040_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt3040_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt4050_eta02_rho040 = new TH1D("data_qglJet_pt4050_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt4050_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt5065_eta02_rho040 = new TH1D("data_qglJet_pt5065_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt5065_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt6580_eta02_rho040 = new TH1D("data_qglJet_pt6580_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt6580_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt80100_eta02_rho040 = new TH1D("data_qglJet_pt80100_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt80100_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt100250_eta02_rho040 = new TH1D("data_qglJet_pt100250_eta02_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt100250_eta02_rho040->Sumw2();




  TH1D* h1_data_qglJet_pt2030_eta23_rho040 = new TH1D("data_qglJet_pt2030_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt2030_eta23_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt4050_eta23_rho040 = new TH1D("data_qglJet_pt4050_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt4050_eta23_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt5065_eta23_rho040 = new TH1D("data_qglJet_pt5065_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt5065_eta23_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt6580_eta23_rho040 = new TH1D("data_qglJet_pt6580_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt6580_eta23_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt80100_eta23_rho040 = new TH1D("data_qglJet_pt80100_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt80100_eta23_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt100250_eta23_rho040 = new TH1D("data_qglJet_pt100250_eta23_rho040", "", nBins, xMin, 1.0001);
  h1_data_qglJet_pt100250_eta23_rho040->Sumw2();




  TH1D* h1_data_qglJet_pt2030_eta35_rho040 = new TH1D("data_qglJet_pt2030_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt2030_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt3040_eta35_rho040 = new TH1D("data_qglJet_pt3040_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt3040_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt4050_eta35_rho040 = new TH1D("data_qglJet_pt4050_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt4050_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt5065_eta35_rho040 = new TH1D("data_qglJet_pt5065_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt5065_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt6580_eta35_rho040 = new TH1D("data_qglJet_pt6580_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt6580_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt80100_eta35_rho040 = new TH1D("data_qglJet_pt80100_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt80100_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt100250_eta35_rho040 = new TH1D("data_qglJet_pt100250_eta35_rho040", "", nBinsFwd, xMin, 1.0001);
  h1_data_qglJet_pt100250_eta35_rho040->Sumw2();


  int nentries_data = tree_data->GetEntries();

  for( unsigned iEntry = 0; iEntry < nentries_data; ++iEntry ) {

    tree_data->GetEntry(iEntry);

    if( njet<1 ) continue;
    //if( rho>40. ) continue;

    //h1_data_ptZ->Fill( ptZ );
    h1_data_rho->Fill( rho );

    int njets_loop = (selection=="ZJet") ? 1 : 2;

    for( unsigned ijet=0; ijet<njets_loop; ijet++ ) {
    
      if( ijet>1 ) break; //shouldnt be possible
      
      int tag_index = ijet;
      int probe_index = njets_loop-ijet-1;


      if( fabs(eta[probe_index])<2. ) {

        if( pt[tag_index] > 20. && pt[tag_index] < 30. ) {

          h1_data_qglJet_pt2030_eta02_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 30. && pt[tag_index] < 40. ) {

          h1_data_qglJet_pt3040_eta02_rho040->Fill( qglJet[probe_index] );

      //  } else if( pt[tag_index] > 30. && pt[tag_index] < 40. ) {

      //    h1_data_qglJet_pt3040_eta02_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 40. && pt[tag_index] < 50. ) {

          h1_data_qglJet_pt4050_eta02_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 50. && pt[tag_index] < 65. ) {

          h1_data_qglJet_pt5065_eta02_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 65. && pt[tag_index] < 80. ) {

          h1_data_qglJet_pt6580_eta02_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 80. && pt[tag_index] < 100. ) {

          h1_data_qglJet_pt80100_eta02_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 100. && pt[tag_index] < 250. ) {

          h1_data_qglJet_pt100250_eta02_rho040->Fill( qglJet[probe_index] );

        } 

      } else if( fabs(eta[probe_index])<3. ) {

        if( pt[tag_index] > 20. && pt[tag_index] < 30. ) {

          h1_data_qglJet_pt2030_eta23_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 40. && pt[tag_index] < 50. ) {

          h1_data_qglJet_pt4050_eta23_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 50. && pt[tag_index] < 65. ) {

          h1_data_qglJet_pt5065_eta23_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 65. && pt[tag_index] < 80. ) {

          h1_data_qglJet_pt6580_eta23_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 80. && pt[tag_index] < 100. ) {

          h1_data_qglJet_pt80100_eta23_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 100. && pt[tag_index] < 250. ) {

          h1_data_qglJet_pt100250_eta23_rho040->Fill( qglJet[probe_index] );

        } 

      } else if( fabs(eta[probe_index])>3. && fabs(eta[probe_index])<4.7 ) {

        if( pt[tag_index] > 20. && pt[tag_index] < 30. ) {

          h1_data_qglJet_pt2030_eta35_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 30. && pt[tag_index] < 40. ) {

          h1_data_qglJet_pt3040_eta35_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 40. && pt[tag_index] < 50. ) {

          h1_data_qglJet_pt4050_eta35_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 50. && pt[tag_index] < 65. ) {

          h1_data_qglJet_pt5065_eta35_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 65. && pt[tag_index] < 80. ) {

          h1_data_qglJet_pt6580_eta35_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 80. && pt[tag_index] < 100. ) {

          h1_data_qglJet_pt80100_eta35_rho040->Fill( qglJet[probe_index] );

        } else if( pt[tag_index] > 100. && pt[tag_index] < 250. ) {

          h1_data_qglJet_pt100250_eta35_rho040->Fill( qglJet[probe_index] );

        } 

      } // eta

    } // for tag and probe

  }


  DrawBase* db = new DrawBase("checkSystSyst");
  db->add_dataFile(file, "data"); //ok thats the MC file but who cares
if(gSelection == "ZJet")
  db->set_lumi(18300.);
if(gSelection == "DiJet")
  db->set_lumi(.0131);

  std::string outputdir = "checkSystPlots_" + selection + "_" + systDatabaseName;
	if(fractionChange >0.0001) outputdir+=Form("fractionPlus_%.2f",fractionChange);
	if(fractionChange <-0.0001) outputdir+=Form("fractionMinus_%.2f",-fractionChange);
  if( use_herwig ) outputdir += "_Hpp";
  db->set_outputdir(outputdir);
  std::string mkdir_command = "mkdir -p " + outputdir;
  system(mkdir_command.c_str());
  
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt2030_eta02_rho040, h1_qglJet_pt2030_eta02_rho040, h1_qglJetSyst_pt2030_eta02_rho040, 20., 30., 0., 2., 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt3040_eta02_rho040, h1_qglJet_pt3040_eta02_rho040, h1_qglJetSyst_pt3040_eta02_rho040, 30., 40., 0., 2., 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt4050_eta02_rho040, h1_qglJet_pt4050_eta02_rho040, h1_qglJetSyst_pt4050_eta02_rho040, 40., 50., 0., 2., 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt5065_eta02_rho040, h1_qglJet_pt5065_eta02_rho040, h1_qglJetSyst_pt5065_eta02_rho040, 50., 65., 0., 2., 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt6580_eta02_rho040, h1_qglJet_pt6580_eta02_rho040, h1_qglJetSyst_pt6580_eta02_rho040, 65., 80., 0., 2., 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt80100_eta02_rho040,h1_qglJet_pt80100_eta02_rho040, h1_qglJetSyst_pt80100_eta02_rho040, 80., 100., 0., 2., 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt100250_eta02_rho040,h1_qglJet_pt100250_eta02_rho040, h1_qglJetSyst_pt100250_eta02_rho040, 100., 250., 0., 2., 0., 40.);

  //drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt2030_eta23_rho040, h1_qglJet_pt2030_eta23_rho040, h1_qglJetSyst_pt2030_eta23_rho040, 20., 30., 2., 3., 0., 40.);
  //drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt4050_eta23_rho040, h1_qglJet_pt4050_eta23_rho040, h1_qglJetSyst_pt4050_eta23_rho040, 40., 50., 2., 3., 0., 40.);
  //drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt5065_eta23_rho040, h1_qglJet_pt5065_eta23_rho040, h1_qglJetSyst_pt5065_eta23_rho040, 50., 65., 2., 3., 0., 40.);
  //drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt6580_eta23_rho040, h1_qglJet_pt6580_eta23_rho040, h1_qglJetSyst_pt6580_eta23_rho040, 65., 80., 2., 3., 0., 40.);
  //drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt80100_eta23_rho040,h1_qglJet_pt80100_eta23_rho040, h1_qglJetSyst_pt80100_eta23_rho040, 80., 100., 2., 3., 0., 40.);
  //drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt100250_eta23_rho040,h1_qglJet_pt100250_eta23_rho040, h1_qglJetSyst_pt100250_eta23_rho040, 100., 250., 2., 3., 0., 40.);

  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt2030_eta35_rho040, h1_qglJet_pt2030_eta35_rho040, h1_qglJetSyst_pt2030_eta35_rho040, 20., 30., 3., 4.7, 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt3040_eta35_rho040, h1_qglJet_pt3040_eta35_rho040, h1_qglJetSyst_pt3040_eta35_rho040, 30., 40., 3., 4.7, 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt4050_eta35_rho040, h1_qglJet_pt4050_eta35_rho040, h1_qglJetSyst_pt4050_eta35_rho040, 40., 50., 3., 4.7, 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt5065_eta35_rho040, h1_qglJet_pt5065_eta35_rho040, h1_qglJetSyst_pt5065_eta35_rho040, 50., 65., 3., 4.7, 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt6580_eta35_rho040, h1_qglJet_pt6580_eta35_rho040, h1_qglJetSyst_pt6580_eta35_rho040, 65., 80., 3., 4.7, 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt80100_eta35_rho040,h1_qglJet_pt80100_eta35_rho040, h1_qglJetSyst_pt80100_eta35_rho040, 80., 100., 3., 4.7, 0., 40.);
  drawSinglePlot( selection, db, discrim, h1_data_qglJet_pt100250_eta35_rho040,h1_qglJet_pt100250_eta35_rho040, h1_qglJetSyst_pt100250_eta35_rho040, 100., 250., 3., 4.7, 0., 40.);

  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt2030_eta02_rho040, h1_qglJetSyst_quark_pt2030_eta02_rho040, h1_qglJet_gluon_pt2030_eta02_rho040, h1_qglJetSyst_gluon_pt2030_eta02_rho040, 20., 30., 0., 2., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt3040_eta02_rho040, h1_qglJetSyst_quark_pt3040_eta02_rho040, h1_qglJet_gluon_pt3040_eta02_rho040, h1_qglJetSyst_gluon_pt3040_eta02_rho040, 30., 40., 0., 2., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt4050_eta02_rho040, h1_qglJetSyst_quark_pt4050_eta02_rho040, h1_qglJet_gluon_pt4050_eta02_rho040, h1_qglJetSyst_gluon_pt4050_eta02_rho040, 40., 50., 0., 2., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt5065_eta02_rho040, h1_qglJetSyst_quark_pt5065_eta02_rho040, h1_qglJet_gluon_pt5065_eta02_rho040, h1_qglJetSyst_gluon_pt5065_eta02_rho040, 50., 65., 0., 2., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt6580_eta02_rho040, h1_qglJetSyst_quark_pt6580_eta02_rho040, h1_qglJet_gluon_pt6580_eta02_rho040, h1_qglJetSyst_gluon_pt6580_eta02_rho040, 65., 80., 0., 2., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt80100_eta02_rho040, h1_qglJetSyst_quark_pt80100_eta02_rho040, h1_qglJet_gluon_pt80100_eta02_rho040, h1_qglJetSyst_gluon_pt80100_eta02_rho040, 80., 100., 0., 2., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt100250_eta02_rho040, h1_qglJetSyst_quark_pt100250_eta02_rho040, h1_qglJet_gluon_pt100250_eta02_rho040, h1_qglJetSyst_gluon_pt100250_eta02_rho040, 100., 250., 0., 2., 0., 40.);

  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt2030_eta23_rho040, h1_qglJetSyst_quark_pt2030_eta23_rho040, h1_qglJet_gluon_pt2030_eta23_rho040, h1_qglJetSyst_gluon_pt2030_eta23_rho040, 20., 30., 2., 3., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt4050_eta23_rho040, h1_qglJetSyst_quark_pt4050_eta23_rho040, h1_qglJet_gluon_pt4050_eta23_rho040, h1_qglJetSyst_gluon_pt4050_eta23_rho040, 40., 50., 2., 3., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt5065_eta23_rho040, h1_qglJetSyst_quark_pt5065_eta23_rho040, h1_qglJet_gluon_pt5065_eta23_rho040, h1_qglJetSyst_gluon_pt5065_eta23_rho040, 50., 65., 2., 3., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt6580_eta23_rho040, h1_qglJetSyst_quark_pt6580_eta23_rho040, h1_qglJet_gluon_pt6580_eta23_rho040, h1_qglJetSyst_gluon_pt6580_eta23_rho040, 65., 80., 2., 3., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt80100_eta23_rho040, h1_qglJetSyst_quark_pt80100_eta23_rho040, h1_qglJet_gluon_pt80100_eta23_rho040, h1_qglJetSyst_gluon_pt80100_eta23_rho040, 80., 100., 2., 3., 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt100250_eta23_rho040, h1_qglJetSyst_quark_pt100250_eta23_rho040, h1_qglJet_gluon_pt100250_eta23_rho040, h1_qglJetSyst_gluon_pt100250_eta23_rho040, 100., 250., 2., 3., 0., 40.);

  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt2030_eta35_rho040, h1_qglJetSyst_quark_pt2030_eta35_rho040, h1_qglJet_gluon_pt2030_eta35_rho040, h1_qglJetSyst_gluon_pt2030_eta35_rho040, 20., 30., 3., 4.7, 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt3040_eta35_rho040, h1_qglJetSyst_quark_pt3040_eta35_rho040, h1_qglJet_gluon_pt3040_eta35_rho040, h1_qglJetSyst_gluon_pt3040_eta35_rho040, 30., 40., 3., 4.7, 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt4050_eta35_rho040, h1_qglJetSyst_quark_pt4050_eta35_rho040, h1_qglJet_gluon_pt4050_eta35_rho040, h1_qglJetSyst_gluon_pt4050_eta35_rho040, 40., 50., 3., 4.7, 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt5065_eta35_rho040, h1_qglJetSyst_quark_pt5065_eta35_rho040, h1_qglJet_gluon_pt5065_eta35_rho040, h1_qglJetSyst_gluon_pt5065_eta35_rho040, 50., 65., 3., 4.7, 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt6580_eta35_rho040, h1_qglJetSyst_quark_pt6580_eta35_rho040, h1_qglJet_gluon_pt6580_eta35_rho040, h1_qglJetSyst_gluon_pt6580_eta35_rho040, 65., 80., 3., 4.7, 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt80100_eta35_rho040, h1_qglJetSyst_quark_pt80100_eta35_rho040, h1_qglJet_gluon_pt80100_eta35_rho040, h1_qglJetSyst_gluon_pt80100_eta35_rho040, 80., 100., 3., 4.7, 0., 40.);
  drawMC_beforeAfter( db, discrim, h1_qglJet_quark_pt100250_eta35_rho040, h1_qglJetSyst_quark_pt100250_eta35_rho040, h1_qglJet_gluon_pt100250_eta35_rho040, h1_qglJetSyst_gluon_pt100250_eta35_rho040, 100., 250., 3., 4.7, 0., 40.);

  // and now the syst smearing efficiencies:
  TH1D* h1_eff_centr_quark_thresh1 = new TH1D(*h1_effNum_centr_quark_thresh1);
  TH1D* h1_eff_centr_quark_syst_thresh1 = new TH1D(*h1_effNum_centr_quark_syst_thresh1);

  TH1D* h1_eff_centr_gluon_thresh1 = new TH1D(*h1_effNum_centr_gluon_thresh1);
  TH1D* h1_eff_centr_gluon_syst_thresh1 = new TH1D(*h1_effNum_centr_gluon_syst_thresh1);


  TH1D* h1_eff_trans_quark_thresh1 = new TH1D(*h1_effNum_trans_quark_thresh1);
  TH1D* h1_eff_trans_quark_syst_thresh1 = new TH1D(*h1_effNum_trans_quark_syst_thresh1);

  TH1D* h1_eff_trans_gluon_thresh1 = new TH1D(*h1_effNum_trans_gluon_thresh1);
  TH1D* h1_eff_trans_gluon_syst_thresh1 = new TH1D(*h1_effNum_trans_gluon_syst_thresh1);


  TH1D* h1_eff_fwd_quark_thresh1 = new TH1D(*h1_effNum_fwd_quark_thresh1);
  TH1D* h1_eff_fwd_quark_syst_thresh1 = new TH1D(*h1_effNum_fwd_quark_syst_thresh1);

  TH1D* h1_eff_fwd_gluon_thresh1 = new TH1D(*h1_effNum_fwd_gluon_thresh1);
  TH1D* h1_eff_fwd_gluon_syst_thresh1 = new TH1D(*h1_effNum_fwd_gluon_syst_thresh1);


  h1_eff_centr_quark_thresh1->Divide(h1_effDenom_centr_quark_thresh1);
  h1_eff_centr_quark_syst_thresh1->Divide(h1_effDenom_centr_quark_thresh1);

  h1_eff_centr_gluon_thresh1->Divide(h1_effDenom_centr_gluon_thresh1);
  h1_eff_centr_gluon_syst_thresh1->Divide(h1_effDenom_centr_gluon_thresh1);


  h1_eff_trans_quark_thresh1->Divide(h1_effDenom_trans_quark_thresh1);
  h1_eff_trans_quark_syst_thresh1->Divide(h1_effDenom_trans_quark_thresh1);

  h1_eff_trans_gluon_thresh1->Divide(h1_effDenom_trans_gluon_thresh1);
  h1_eff_trans_gluon_syst_thresh1->Divide(h1_effDenom_trans_gluon_thresh1);


  h1_eff_fwd_quark_thresh1->Divide(h1_effDenom_fwd_quark_thresh1);
  h1_eff_fwd_quark_syst_thresh1->Divide(h1_effDenom_fwd_quark_thresh1);

  h1_eff_fwd_gluon_thresh1->Divide(h1_effDenom_fwd_gluon_thresh1);
  h1_eff_fwd_gluon_syst_thresh1->Divide(h1_effDenom_fwd_gluon_thresh1);


  // set good enough errors:
  for( unsigned ibin=1; ibin<nbins_pt+1; ++ibin ) {

    h1_eff_centr_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_centr_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_centr_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_centr_quark_thresh1->GetBinContent(ibin)));
    h1_eff_centr_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_centr_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_centr_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_centr_quark_thresh1->GetBinContent(ibin)));

    h1_eff_centr_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_centr_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_centr_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_centr_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_centr_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_centr_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_centr_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_centr_gluon_thresh1->GetBinContent(ibin)));

    h1_eff_trans_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_trans_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_trans_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_trans_quark_thresh1->GetBinContent(ibin)));
    h1_eff_trans_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_trans_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_trans_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_trans_quark_thresh1->GetBinContent(ibin)));

    h1_eff_trans_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_trans_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_trans_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_trans_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_trans_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_trans_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_trans_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_trans_gluon_thresh1->GetBinContent(ibin)));

    h1_eff_fwd_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_fwd_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_fwd_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_fwd_quark_thresh1->GetBinContent(ibin)));
    h1_eff_fwd_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_fwd_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_fwd_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_fwd_quark_thresh1->GetBinContent(ibin)));

    h1_eff_fwd_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_fwd_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_fwd_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_fwd_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_fwd_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_fwd_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_fwd_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_fwd_gluon_thresh1->GetBinContent(ibin)));

  }


  drawEffVsPt(db, discrim, thresh1, "pt", 0., 2., h1_eff_centr_gluon_thresh1, h1_eff_centr_gluon_syst_thresh1, h1_eff_centr_quark_thresh1, h1_eff_centr_quark_syst_thresh1);
  drawEffVsPt(db, discrim, thresh1, "pt", 2., 3., h1_eff_trans_gluon_thresh1, h1_eff_trans_gluon_syst_thresh1, h1_eff_trans_quark_thresh1, h1_eff_trans_quark_syst_thresh1);
  drawEffVsPt(db, discrim, thresh1, "pt", 3., 4.7, h1_eff_fwd_gluon_thresh1, h1_eff_fwd_gluon_syst_thresh1, h1_eff_fwd_quark_thresh1, h1_eff_fwd_quark_syst_thresh1);



  // and vs rho:
  TH1D* h1_eff_vs_rho_centr_quark_thresh1 = new TH1D(*h1_effNum_vs_rho_centr_quark_thresh1);
  TH1D* h1_eff_vs_rho_centr_quark_syst_thresh1 = new TH1D(*h1_effNum_vs_rho_centr_quark_syst_thresh1);

  TH1D* h1_eff_vs_rho_centr_gluon_thresh1 = new TH1D(*h1_effNum_vs_rho_centr_gluon_thresh1);
  TH1D* h1_eff_vs_rho_centr_gluon_syst_thresh1 = new TH1D(*h1_effNum_vs_rho_centr_gluon_syst_thresh1);

  TH1D* h1_eff_vs_rho_trans_quark_thresh1 = new TH1D(*h1_effNum_vs_rho_trans_quark_thresh1);
  TH1D* h1_eff_vs_rho_trans_quark_syst_thresh1 = new TH1D(*h1_effNum_vs_rho_trans_quark_syst_thresh1);

  TH1D* h1_eff_vs_rho_trans_gluon_thresh1 = new TH1D(*h1_effNum_vs_rho_trans_gluon_thresh1);
  TH1D* h1_eff_vs_rho_trans_gluon_syst_thresh1 = new TH1D(*h1_effNum_vs_rho_trans_gluon_syst_thresh1);

  TH1D* h1_eff_vs_rho_fwd_quark_thresh1 = new TH1D(*h1_effNum_vs_rho_fwd_quark_thresh1);
  TH1D* h1_eff_vs_rho_fwd_quark_syst_thresh1 = new TH1D(*h1_effNum_vs_rho_fwd_quark_syst_thresh1);

  TH1D* h1_eff_vs_rho_fwd_gluon_thresh1 = new TH1D(*h1_effNum_vs_rho_fwd_gluon_thresh1);
  TH1D* h1_eff_vs_rho_fwd_gluon_syst_thresh1 = new TH1D(*h1_effNum_vs_rho_fwd_gluon_syst_thresh1);


  h1_eff_vs_rho_centr_quark_thresh1->Divide(h1_effDenom_vs_rho_centr_quark_thresh1);
  h1_eff_vs_rho_centr_quark_syst_thresh1->Divide(h1_effDenom_vs_rho_centr_quark_thresh1);

  h1_eff_vs_rho_centr_gluon_thresh1->Divide(h1_effDenom_vs_rho_centr_gluon_thresh1);
  h1_eff_vs_rho_centr_gluon_syst_thresh1->Divide(h1_effDenom_vs_rho_centr_gluon_thresh1);

  h1_eff_vs_rho_trans_quark_thresh1->Divide(h1_effDenom_vs_rho_trans_quark_thresh1);
  h1_eff_vs_rho_trans_quark_syst_thresh1->Divide(h1_effDenom_vs_rho_trans_quark_thresh1);

  h1_eff_vs_rho_trans_gluon_thresh1->Divide(h1_effDenom_vs_rho_trans_gluon_thresh1);
  h1_eff_vs_rho_trans_gluon_syst_thresh1->Divide(h1_effDenom_vs_rho_trans_gluon_thresh1);

  h1_eff_vs_rho_fwd_quark_thresh1->Divide(h1_effDenom_vs_rho_fwd_quark_thresh1);
  h1_eff_vs_rho_fwd_quark_syst_thresh1->Divide(h1_effDenom_vs_rho_fwd_quark_thresh1);

  h1_eff_vs_rho_fwd_gluon_thresh1->Divide(h1_effDenom_vs_rho_fwd_gluon_thresh1);
  h1_eff_vs_rho_fwd_gluon_syst_thresh1->Divide(h1_effDenom_vs_rho_fwd_gluon_thresh1);


  // set good enough errors:
  for( unsigned ibin=1; ibin<nbins_rho+1; ++ibin ) {

    h1_eff_vs_rho_centr_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_centr_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_centr_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_centr_quark_thresh1->GetBinContent(ibin)));
    h1_eff_vs_rho_centr_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_centr_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_centr_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_centr_quark_thresh1->GetBinContent(ibin)));

    h1_eff_vs_rho_trans_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_trans_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_trans_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_trans_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_vs_rho_trans_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_trans_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_trans_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_trans_gluon_thresh1->GetBinContent(ibin)));

    h1_eff_vs_rho_trans_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_trans_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_trans_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_trans_quark_thresh1->GetBinContent(ibin)));
    h1_eff_vs_rho_trans_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_trans_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_trans_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_trans_quark_thresh1->GetBinContent(ibin)));

    h1_eff_vs_rho_trans_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_trans_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_trans_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_trans_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_vs_rho_trans_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_trans_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_trans_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_trans_gluon_thresh1->GetBinContent(ibin)));

    h1_eff_vs_rho_fwd_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_fwd_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_fwd_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_fwd_quark_thresh1->GetBinContent(ibin)));
    h1_eff_vs_rho_fwd_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_fwd_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_fwd_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_fwd_quark_thresh1->GetBinContent(ibin)));

    h1_eff_vs_rho_fwd_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_fwd_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_fwd_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_fwd_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_vs_rho_fwd_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_rho_fwd_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_rho_fwd_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_rho_fwd_gluon_thresh1->GetBinContent(ibin)));

  }


  drawEffVsPt(db, discrim, thresh1, "rho", 0., 2., h1_eff_vs_rho_centr_gluon_thresh1, h1_eff_vs_rho_centr_gluon_syst_thresh1, h1_eff_vs_rho_centr_quark_thresh1, h1_eff_vs_rho_centr_quark_syst_thresh1);
  drawEffVsPt(db, discrim, thresh1, "rho", 2., 3., h1_eff_vs_rho_trans_gluon_thresh1, h1_eff_vs_rho_trans_gluon_syst_thresh1, h1_eff_vs_rho_trans_quark_thresh1, h1_eff_vs_rho_trans_quark_syst_thresh1);
  drawEffVsPt(db, discrim, thresh1, "rho", 3., 4.7, h1_eff_vs_rho_fwd_gluon_thresh1, h1_eff_vs_rho_fwd_gluon_syst_thresh1, h1_eff_vs_rho_fwd_quark_thresh1, h1_eff_vs_rho_fwd_quark_syst_thresh1);





  // and vs nvertex:
  TH1D* h1_eff_vs_nvertex_centr_quark_thresh1 = new TH1D(*h1_effNum_vs_nvertex_centr_quark_thresh1);
  TH1D* h1_eff_vs_nvertex_centr_quark_syst_thresh1 = new TH1D(*h1_effNum_vs_nvertex_centr_quark_syst_thresh1);

  TH1D* h1_eff_vs_nvertex_centr_gluon_thresh1 = new TH1D(*h1_effNum_vs_nvertex_centr_gluon_thresh1);
  TH1D* h1_eff_vs_nvertex_centr_gluon_syst_thresh1 = new TH1D(*h1_effNum_vs_nvertex_centr_gluon_syst_thresh1);

  TH1D* h1_eff_vs_nvertex_trans_quark_thresh1 = new TH1D(*h1_effNum_vs_nvertex_trans_quark_thresh1);
  TH1D* h1_eff_vs_nvertex_trans_quark_syst_thresh1 = new TH1D(*h1_effNum_vs_nvertex_trans_quark_syst_thresh1);

  TH1D* h1_eff_vs_nvertex_trans_gluon_thresh1 = new TH1D(*h1_effNum_vs_nvertex_trans_gluon_thresh1);
  TH1D* h1_eff_vs_nvertex_trans_gluon_syst_thresh1 = new TH1D(*h1_effNum_vs_nvertex_trans_gluon_syst_thresh1);

  TH1D* h1_eff_vs_nvertex_fwd_quark_thresh1 = new TH1D(*h1_effNum_vs_nvertex_fwd_quark_thresh1);
  TH1D* h1_eff_vs_nvertex_fwd_quark_syst_thresh1 = new TH1D(*h1_effNum_vs_nvertex_fwd_quark_syst_thresh1);

  TH1D* h1_eff_vs_nvertex_fwd_gluon_thresh1 = new TH1D(*h1_effNum_vs_nvertex_fwd_gluon_thresh1);
  TH1D* h1_eff_vs_nvertex_fwd_gluon_syst_thresh1 = new TH1D(*h1_effNum_vs_nvertex_fwd_gluon_syst_thresh1);


  h1_eff_vs_nvertex_centr_quark_thresh1->Divide(h1_effDenom_vs_nvertex_centr_quark_thresh1);
  h1_eff_vs_nvertex_centr_quark_syst_thresh1->Divide(h1_effDenom_vs_nvertex_centr_quark_thresh1);

  h1_eff_vs_nvertex_centr_gluon_thresh1->Divide(h1_effDenom_vs_nvertex_centr_gluon_thresh1);
  h1_eff_vs_nvertex_centr_gluon_syst_thresh1->Divide(h1_effDenom_vs_nvertex_centr_gluon_thresh1);

  h1_eff_vs_nvertex_trans_quark_thresh1->Divide(h1_effDenom_vs_nvertex_trans_quark_thresh1);
  h1_eff_vs_nvertex_trans_quark_syst_thresh1->Divide(h1_effDenom_vs_nvertex_trans_quark_thresh1);

  h1_eff_vs_nvertex_trans_gluon_thresh1->Divide(h1_effDenom_vs_nvertex_trans_gluon_thresh1);
  h1_eff_vs_nvertex_trans_gluon_syst_thresh1->Divide(h1_effDenom_vs_nvertex_trans_gluon_thresh1);

  h1_eff_vs_nvertex_fwd_quark_thresh1->Divide(h1_effDenom_vs_nvertex_fwd_quark_thresh1);
  h1_eff_vs_nvertex_fwd_quark_syst_thresh1->Divide(h1_effDenom_vs_nvertex_fwd_quark_thresh1);

  h1_eff_vs_nvertex_fwd_gluon_thresh1->Divide(h1_effDenom_vs_nvertex_fwd_gluon_thresh1);
  h1_eff_vs_nvertex_fwd_gluon_syst_thresh1->Divide(h1_effDenom_vs_nvertex_fwd_gluon_thresh1);


  // set good enough errors:
  for( unsigned ibin=1; ibin<nbins_nvertex+1; ++ibin ) {

    h1_eff_vs_nvertex_centr_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_centr_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_centr_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_centr_quark_thresh1->GetBinContent(ibin)));
    h1_eff_vs_nvertex_centr_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_centr_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_centr_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_centr_quark_thresh1->GetBinContent(ibin)));

    h1_eff_vs_nvertex_trans_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_vs_nvertex_trans_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_trans_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_trans_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin)));

    h1_eff_vs_nvertex_trans_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_trans_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_trans_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_trans_quark_thresh1->GetBinContent(ibin)));
    h1_eff_vs_nvertex_trans_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_trans_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_trans_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_trans_quark_thresh1->GetBinContent(ibin)));

    h1_eff_vs_nvertex_trans_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_vs_nvertex_trans_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_trans_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_trans_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_trans_gluon_thresh1->GetBinContent(ibin)));

    h1_eff_vs_nvertex_fwd_quark_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_fwd_quark_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_fwd_quark_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_fwd_quark_thresh1->GetBinContent(ibin)));
    h1_eff_vs_nvertex_fwd_quark_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_fwd_quark_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_fwd_quark_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_fwd_quark_thresh1->GetBinContent(ibin)));

    h1_eff_vs_nvertex_fwd_gluon_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_fwd_gluon_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_fwd_gluon_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_fwd_gluon_thresh1->GetBinContent(ibin)));
    h1_eff_vs_nvertex_fwd_gluon_syst_thresh1->SetBinError(ibin, sqrt(h1_eff_vs_nvertex_fwd_gluon_syst_thresh1->GetBinContent(ibin)*(1.-h1_eff_vs_nvertex_fwd_gluon_syst_thresh1->GetBinContent(ibin))/h1_effDenom_uw_vs_nvertex_fwd_gluon_thresh1->GetBinContent(ibin)));

  }


  drawEffVsPt(db, discrim, thresh1, "nvertex", 0., 2., h1_eff_vs_nvertex_centr_gluon_thresh1, h1_eff_vs_nvertex_centr_gluon_syst_thresh1, h1_eff_vs_nvertex_centr_quark_thresh1, h1_eff_vs_nvertex_centr_quark_syst_thresh1);
  drawEffVsPt(db, discrim, thresh1, "nvertex", 2., 3., h1_eff_vs_nvertex_trans_gluon_thresh1, h1_eff_vs_nvertex_trans_gluon_syst_thresh1, h1_eff_vs_nvertex_trans_quark_thresh1, h1_eff_vs_nvertex_trans_quark_syst_thresh1);
  drawEffVsPt(db, discrim, thresh1, "nvertex", 3., 4.7, h1_eff_vs_nvertex_fwd_gluon_thresh1, h1_eff_vs_nvertex_fwd_gluon_syst_thresh1, h1_eff_vs_nvertex_fwd_quark_thresh1, h1_eff_vs_nvertex_fwd_quark_syst_thresh1);


  outfile->cd();
 
  h1_ptZ->Write();
  h1_rho->Write();

  h1_data_ptZ->Write();
  h1_data_rho->Write();

  h1_qglJet_pt2030_eta02_rho040->Write();
  h1_qglJetSyst_pt2030_eta02_rho040->Write();
  
  h1_qglJet_pt3040_eta02_rho040->Write();
  h1_qglJetSyst_pt3040_eta02_rho040->Write();
  
  h1_qglJet_pt4050_eta02_rho040->Write();
  h1_qglJetSyst_pt4050_eta02_rho040->Write();
  
  h1_qglJet_pt5065_eta02_rho040->Write();
  h1_qglJetSyst_pt5065_eta02_rho040->Write();
  
  h1_qglJet_pt6580_eta02_rho040->Write();
  h1_qglJetSyst_pt6580_eta02_rho040->Write();
  
  h1_qglJet_pt80100_eta02_rho040->Write();
  h1_qglJetSyst_pt80100_eta02_rho040->Write();

  h1_qglJet_pt100250_eta02_rho040->Write();
  h1_qglJetSyst_pt100250_eta02_rho040->Write();

  h1_qglJet_pt2030_eta35_rho040->Write();
  h1_qglJetSyst_pt2030_eta35_rho040->Write();
  
  h1_qglJet_pt3040_eta35_rho040->Write();
  h1_qglJetSyst_pt3040_eta35_rho040->Write();
  
  h1_qglJet_pt4050_eta35_rho040->Write();
  h1_qglJetSyst_pt4050_eta35_rho040->Write();
  
  h1_qglJet_pt5065_eta35_rho040->Write();
  h1_qglJetSyst_pt5065_eta35_rho040->Write();
  
  h1_qglJet_pt6580_eta35_rho040->Write();
  h1_qglJetSyst_pt6580_eta35_rho040->Write();
  
  h1_qglJet_pt80100_eta35_rho040->Write();
  h1_qglJetSyst_pt80100_eta35_rho040->Write();


  h1_data_qglJet_pt2030_eta02_rho040->Write();
  
  h1_data_qglJet_pt3040_eta02_rho040->Write();
  
  h1_data_qglJet_pt4050_eta02_rho040->Write();
  
  h1_data_qglJet_pt5065_eta02_rho040->Write();
  
  h1_data_qglJet_pt6580_eta02_rho040->Write();
  
  h1_data_qglJet_pt80100_eta02_rho040->Write();

  h1_data_qglJet_pt100250_eta02_rho040->Write();

  h1_data_qglJet_pt2030_eta35_rho040->Write();

  h1_data_qglJet_pt3040_eta35_rho040->Write();

  h1_data_qglJet_pt4050_eta35_rho040->Write();
  
  h1_data_qglJet_pt5065_eta35_rho040->Write();
  
  h1_data_qglJet_pt6580_eta35_rho040->Write();
  
  h1_data_qglJet_pt80100_eta35_rho040->Write();

  outfile->Close();
  


  return 0;
  
}


void drawSinglePlot( const std::string& selection, DrawBase* db, const std::string& discrim, TH1D* h1_data, TH1D* h1_qgl, TH1D* h1_qglSyst, float ptMin, float ptMax, float etaMin, float etaMax, float rhoMin, float rhoMax ) {


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();


  h1_qgl->SetLineStyle(2);
  h1_qgl->SetLineWidth(2);
  h1_qglSyst->SetLineWidth(2);
  h1_data->SetMarkerStyle(20);

  float ymax = h1_data->GetMaximum();
  if( discrim=="qgMLPJet" )
    ymax *= 1.5;
  else
    ymax *= 1.3;

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 1.0001, 10, 0., ymax);
  if( discrim=="qgMLPJet" ) 
    h2_axes->SetXTitle("Quark-Gluon MLP");
  else
    h2_axes->SetXTitle("Quark-Gluon Likelihood Discriminant");
  h2_axes->SetYTitle("Events");

  h1_qglSyst->SetLineColor(kRed);

  float mcNorm = h1_data->Integral()/h1_qgl->Integral();
  h1_qgl->Scale(mcNorm);
  float mcSystNorm = h1_data->Integral()/h1_qglSyst->Integral();
  h1_qglSyst->Scale(mcSystNorm);

  h2_axes->Draw();

  char legendTitle[500];
  //sprintf( legendTitle, "p_{T}(%.0f-%.0f), #eta(%.1f-%.1f), #rho(%.0f-%.0f)", ptMin, ptMax, etaMin, etaMax, rhoMin, rhoMax);
  if( etaMin==0. ) 
    sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV,  |#eta| < %.0f", ptMin, ptMax, etaMax);
  else 
    if( etaMax>4. )
      sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV,  %.0f < |#eta| < %.1f", ptMin, ptMax, etaMin, etaMax);
    else
      sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV,  %.0f < |#eta| < %.0f", ptMin, ptMax, etaMin, etaMax);


  TLegend* legend = new TLegend(0.25, 0.66, 0.7, 0.9, legendTitle);
  legend->SetFillColor(0);
  legend->SetTextSize(0.04);
  if( selection=="DiJet" )
    legend->AddEntry( h1_data, "DiJet data", "p" );
  else 
    legend->AddEntry( h1_data, "Z+Jet data", "p" );
  legend->AddEntry( h1_qgl, "MC before smearing", "L" );
  legend->AddEntry( h1_qglSyst, "MC after smearing", "L" );
  legend->Draw("same");


  h1_qgl->Draw("Histo same");
  h1_qglSyst->Draw("Histo same");
  h1_data->Draw("P same");

  std::string doubleMin_str = "";
  if( doubleMin ) doubleMin_str = "DM";

  char canvasName[1023];
  sprintf( canvasName, "%s/checkSyst%s_%s_pt%.0f_%.0f_eta%.0f_%.0f_rho%.0f_%.0f.eps", db->get_outputdir().c_str(), discrim.c_str(), doubleMin_str.c_str(), ptMin, ptMax, etaMin, etaMax, rhoMin, rhoMax);

  TPaveText* label_top = db->get_labelTop();
  label_top->Draw("same");

  c1->SaveAs(canvasName);
  std::string canvasName_str(canvasName);
  std::string command = "epstopdf " + canvasName_str;
  system(command.c_str());
  //c1->SaveAs( (std::string(canvasName)+std::string(".root") ).c_str());

  delete c1;
  delete legend;
  delete h2_axes;

}



void drawEffVsPt( DrawBase* db, const std::string& discrim, float thresh1, const std::string xVarName, float etaMin, float etaMax, TH1D* h1_eff_gluon_thresh1, TH1D* h1_eff_gluon_syst_thresh1, TH1D* h1_eff_quark_thresh1, TH1D* h1_eff_quark_syst_thresh1, const std::string& suffix ) {



  char etaText[300];
  if( etaMin>0. )
    if( etaMax>4. )
      sprintf( etaText, "%.0f < |#eta| < %.1f", etaMin, etaMax );
    else
      sprintf( etaText, "%.0f < |#eta| < %.0f", etaMin, etaMax );
  else
    sprintf( etaText, "|#eta| < %.0f", etaMax );




  float xMax = h1_eff_gluon_thresh1->GetXaxis()->GetXmax();

  if( xVarName=="pt" ) {

    if( etaMax>4. )
      xMax = 125.;
    else if( etaMax>2. )
      xMax = 210.;
    else
      xMax = 520.;

  }


  std::string discrim_text = (discrim=="qgMLPJet") ? "MLP" : "LD";
  std::string operator_discrim = (discrim=="qgMLPJet") ? "<" : ">";


  float xMin = 0.;
  if( xVarName=="pt" ) xMin = 30.;
  if( xVarName=="nvertex" ) xMin = .5;
  //if( !isRho && etaMax > 4. ) xMin = 30.;

  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, xMax, 10, 0., 1.);
  if( xVarName=="rho" )
    h2_axes->SetXTitle("#rho [GeV]");
  else if( xVarName=="nvertex" ) 
    h2_axes->SetXTitle("Number of Primary Vertices");
  else {
    h2_axes->SetXTitle("Jet p_{T} [GeV]");
    h2_axes->GetXaxis()->SetMoreLogLabels();
    h2_axes->GetXaxis()->SetNoExponent();
  }
  char yAxisTitle[300];
  sprintf( yAxisTitle, "Efficiency (%s %s %.1f)", discrim_text.c_str(), operator_discrim.c_str(), thresh1);
  h2_axes->SetYTitle(yAxisTitle);


  h1_eff_gluon_thresh1->SetMarkerStyle(20);
  h1_eff_gluon_thresh1->SetMarkerSize(1.6);
  h1_eff_gluon_thresh1->SetMarkerColor(46);
  h1_eff_gluon_thresh1->SetLineColor(kBlack);

  h1_eff_gluon_syst_thresh1->SetMarkerStyle(24);
  h1_eff_gluon_syst_thresh1->SetMarkerSize(1.6);
  h1_eff_gluon_syst_thresh1->SetMarkerColor(46);
  h1_eff_gluon_syst_thresh1->SetLineColor(kBlack);

  h1_eff_quark_thresh1->SetMarkerStyle(21);
  h1_eff_quark_thresh1->SetMarkerSize(1.6);
  h1_eff_quark_thresh1->SetMarkerColor(38);
  h1_eff_quark_thresh1->SetLineColor(kBlack);

  h1_eff_quark_syst_thresh1->SetMarkerStyle(25);
  h1_eff_quark_syst_thresh1->SetMarkerSize(1.6);
  h1_eff_quark_syst_thresh1->SetMarkerColor(38);
  h1_eff_quark_syst_thresh1->SetLineColor(kBlack);

  TPaveText* labelTop = db->get_labelTop();
  TPaveText* labelEta = new TPaveText(0.22, 0.83, 0.36, 0.93, "brNDC");
  labelEta->AddText(etaText);
  labelEta->SetTextSize(0.038);
  labelEta->SetFillColor(0);

  TPaveText* labelEta_bottom = new TPaveText(0.22, 0.2, 0.36, 0.25, "brNDC");
  labelEta_bottom->AddText(etaText);
  labelEta_bottom->SetTextSize(0.038);
  labelEta_bottom->SetFillColor(0);
   
   TPaveText* labelSelection = new TPaveText(.55,.2,.65,.25,"brNDC");
   labelSelection->AddText(gSelection.c_str());
  labelSelection->SetTextSize(0.038);
  labelSelection->SetFillColor(0);

  char gluon_text[500];
  sprintf( gluon_text, "Gluon" );
  char gluon_syst_text[500];
  sprintf( gluon_syst_text, "Gluon (smeared)" );
  char quark_text[500];
  sprintf( quark_text, "Quark" );
  char quark_syst_text[500];
  sprintf( quark_syst_text, "Quark (smeared)" );


  char legendTitle[300];
  sprintf( legendTitle, "Quark-Gluon %s", discrim_text.c_str() );
  //sprintf( legendTitle, "Quark-Gluon %s, %s", discrim_text.c_str(), etaText );

  TLegend* legend = new TLegend( 0.18, 0.18, 0.5, 0.43 );
  //TLegend* legend = new TLegend( 0.18, 0.18, 0.5, 0.43, legendTitle );
  legend->SetTextSize(0.04);
  legend->SetFillColor(kWhite);
  legend->AddEntry( h1_eff_quark_thresh1, quark_text, "P");
  legend->AddEntry( h1_eff_quark_syst_thresh1, quark_syst_text, "P");
  legend->AddEntry( h1_eff_gluon_thresh1, gluon_text, "P");
  legend->AddEntry( h1_eff_gluon_syst_thresh1, gluon_syst_text, "P");

  TLegend* legend_quark = new TLegend( 0.17, 0.79, 0.45, 0.91 );
  legend_quark->SetTextSize(0.04);
  legend_quark->SetFillColor(kWhite);
  legend_quark->AddEntry( h1_eff_quark_thresh1, quark_text, "P");
  legend_quark->AddEntry( h1_eff_quark_syst_thresh1, quark_syst_text, "P");

  TLegend* legend_gluon = new TLegend( 0.55, 0.79, 0.83, 0.91 );
  legend_gluon->SetTextSize(0.04);
  legend_gluon->SetFillColor(kWhite);
  legend_gluon->AddEntry( h1_eff_gluon_thresh1, gluon_text, "P");
  legend_gluon->AddEntry( h1_eff_gluon_syst_thresh1, gluon_syst_text, "P");


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();
  if( xVarName=="pt" )
    c1->SetLogx();
  h2_axes->Draw();
  //legend->Draw("same");
  //labelEta->Draw("same");
  legend_gluon->Draw("same");
  legend_quark->Draw("same");
  labelEta_bottom->Draw("same");
  labelTop->Draw("same");
   labelSelection->Draw("same");
  //labelEta->Draw("same");
  gStyle->SetErrorX(0);
  h1_eff_gluon_thresh1->Draw("p same");
  h1_eff_gluon_syst_thresh1->Draw("p same");
  h1_eff_quark_thresh1->Draw("p same");
  h1_eff_quark_syst_thresh1->Draw("p same");


  gPad->RedrawAxis();
  
  std::string suffixVar = "";
  if(xVarName!="pt") suffixVar = "_vs" + xVarName;
  
  char canvasName[500];
  if( etaMax > 4. ) {
    sprintf( canvasName, "%s/syst_eff%s%s_fwd%s.eps", db->get_outputdir().c_str(), suffixVar.c_str(), discrim.c_str(), suffix.c_str() );
  } else if( etaMax > 2. ) {
    sprintf( canvasName, "%s/syst_eff%s%s_trans%s.eps", db->get_outputdir().c_str(), suffixVar.c_str(), discrim.c_str(), suffix.c_str() );
  } else {
    sprintf( canvasName, "%s/syst_eff%s%s_centr%s.eps", db->get_outputdir().c_str(), suffixVar.c_str(), discrim.c_str(), suffix.c_str() );
  }
  
  c1->SaveAs(canvasName);
  std::string canvasName_str(canvasName);
  std::string command = "epstopdf " + canvasName_str;
  system(command.c_str());
  c1->SaveAs( (std::string(canvasName)+std::string(".root") ).c_str());

  delete c1;
  delete h2_axes;
  delete labelEta;
  delete legend;

}



void drawMC_beforeAfter( DrawBase* db, const std::string& discrim, TH1D* h1_qglJet_quark, TH1D* h1_qglJetSyst_quark, TH1D* h1_qglJet_gluon, TH1D* h1_qglJetSyst_gluon, float ptMin, float ptMax, float etaMin, float etaMax, float rhoMin, float rhoMax ) {


  h1_qglJet_quark->SetLineColor(38);
  h1_qglJet_quark->SetLineWidth(2);
  h1_qglJet_quark->SetFillColor(38);
  h1_qglJet_quark->SetFillStyle(3004);

  h1_qglJetSyst_quark->SetMarkerColor(38);
  h1_qglJetSyst_quark->SetMarkerStyle(20);
  h1_qglJetSyst_quark->SetMarkerSize(1.3);

  h1_qglJet_gluon->SetLineColor(46);
  h1_qglJet_gluon->SetLineWidth(2);
  h1_qglJet_gluon->SetFillColor(46);
  h1_qglJet_gluon->SetFillStyle(3005);

  h1_qglJetSyst_gluon->SetMarkerColor(46);
  h1_qglJetSyst_gluon->SetMarkerStyle(21);
  h1_qglJetSyst_gluon->SetMarkerSize(1.3);


  float histoMax = h1_qglJet_quark->GetMaximum()/h1_qglJet_quark->Integral();
  if( h1_qglJetSyst_quark->GetMaximum()/h1_qglJetSyst_quark->Integral() > histoMax ) histoMax = h1_qglJetSyst_quark->GetMaximum()/h1_qglJetSyst_quark->Integral();
  if( h1_qglJet_gluon->GetMaximum()/h1_qglJet_gluon->Integral() > histoMax ) histoMax = h1_qglJet_gluon->GetMaximum()/h1_qglJet_gluon->Integral();
  if( h1_qglJetSyst_gluon->GetMaximum()/h1_qglJetSyst_gluon->Integral() > histoMax ) histoMax = h1_qglJetSyst_gluon->GetMaximum()/h1_qglJetSyst_gluon->Integral();

  float yMax = 1.3*histoMax;


  std::string discrim_text = (discrim=="qgMLPJet") ? "Quark-Gluon MLP" : "Quark-Gluon Likelihood Discriminant";

  TH2D* h2_axes = new TH2D("axes", "", 10, h1_qglJet_quark->GetXaxis()->GetXmin(), h1_qglJet_quark->GetXaxis()->GetXmax(), 10, 0., yMax);
  h2_axes->SetXTitle(discrim_text.c_str());
  h2_axes->SetYTitle("Normalized to Unity");

  char legendTitle[500];
  if( etaMin==0. )
    sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV,  |#eta| < %.0f", ptMin, ptMax, etaMax);
  else
    sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV,  %.0f < |#eta| < %.0f", ptMin, ptMax, etaMin, etaMax);

  TLegend* legend = new TLegend(0.25, 0.63, 0.7, 0.9, legendTitle);
  legend->SetFillColor(0);
  legend->SetTextSize(0.038);
  legend->AddEntry( h1_qglJet_quark, "Quark MC", "F");
  legend->AddEntry( h1_qglJetSyst_quark, "Quark MC After Smearing", "P");
  legend->AddEntry( h1_qglJet_gluon, "Gluon MC", "F");
  legend->AddEntry( h1_qglJetSyst_gluon, "Gluon MC After Smearing", "P");

  TPaveText* labelTop = db->get_labelTop();

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();
  h2_axes->Draw();
  legend->Draw("same");
  labelTop->Draw("same");

  h1_qglJet_gluon->DrawNormalized("histo same");
  h1_qglJet_quark->DrawNormalized("histo same");
  h1_qglJetSyst_gluon->DrawNormalized("p same");
  h1_qglJetSyst_quark->DrawNormalized("p same");

  gPad->RedrawAxis();

  char canvasName[500];
  sprintf( canvasName, "%s/%s_MC_beforeAfter_pt%.0f_%.0f_eta%.0f_%.0f_rho%.0f_%.0f.eps", db->get_outputdir().c_str(), discrim.c_str(), ptMin, ptMax, etaMin, etaMax, rhoMin, rhoMax);

  c1->SaveAs(canvasName);
  std::string canvasName_str(canvasName);
  std::string command = "epstopdf " + canvasName_str;
  system(command.c_str());

  delete c1;
  delete h2_axes;
  delete legend;

}


