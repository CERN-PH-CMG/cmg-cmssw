#include "DrawBase.h"
#include <cstdlib>
#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TChain.h"



bool use_tagAndProbe = true;
bool use_ptHatWeight = true;
bool use_herwig = false;


void drawHistoWithQuarkGluonComponents( const std::string& selectionType, DrawBase* db, const std::string& treeName, const std::string& additionalCuts, const std::string& varName, const std::string& canvasSaveName, std::string axisName, const std::string& units="", float ptMin=0., float ptMax=10000., float etaMin=0., float etaMax =10., int nBins=25, float xMin=0., float xMax=1.0001, bool legendQuadrant=1, bool log_aussi=false );



int main(int argc, char* argv[]) {


  std::string selectionType;
  if( argc>1 ) {
    std::string selectionType_str(argv[1]);
    selectionType = selectionType_str;
  }

  bool use_MB = true;
  if( argc>2 ) {
    std::string MB_str(argv[2]);
    TString MB_tstr(MB_str);
    use_MB = MB_tstr.Contains("MB");
  }
 
  if( use_MB && selectionType=="DiJets" )
    std::cout << "-> Using Minimum Bias Data" << std::endl;



  if( selectionType!="ZJets" && selectionType!="DiJets" ) {
    std::cout << "Supported selections are only \"ZJets\" and \"DiJets\". Exiting." << std::endl;
    exit(11);
  }


  bool use_tnp = false;
  if( selectionType=="DiJets" && use_tagAndProbe )
    use_tnp = true;

  if( use_tnp )
    std::cout << "-> Using dijet tag and probe." << std::endl;


  bool use_pthatweight = false;
  if( selectionType=="DiJets" && use_ptHatWeight )
    use_pthatweight = true;

  if( use_pthatweight )
    std::cout << "-> Using pt_hat weights." << std::endl;


  TFile* file_data;
  TFile* file_mc;

  if( selectionType=="ZJets" ) {
    
    file_data  = TFile::Open("sunilFlat_ZJet_data2012ABCD_MuPD_24Aug_skim_new.root");
    if( use_herwig )
      file_mc = TFile::Open("sunilFlat_ZJet_Zjets_HppXXX.root");
    else
      file_mc = TFile::Open("sunilFlat_ZJet_Zjets_MGPy6_24Aug_skim.root");
  } else {
    if( use_MB )
      file_data = TFile::Open("sunilFlat_DiJet_data2012ABCD_MBPD_24Aug_new.root");
    else
      file_data = TFile::Open("sunilFlat_DiJet_data2012ABCD_JetPD_24Aug.root");
    std::string mcFileName = "sunilFlat_DiJet_flatQCD_P6_Dijets_24Aug";
    if( use_pthatweight ) {
      if( use_herwig )
        mcFileName = "sunilFlat_DiJet_flatQCD_Hpp_Dijets_24Aug_ptHatWeight";
       else
        mcFileName = "sunilFlat_DiJet_flatQCD_Py6_Dijets_24Aug_ptHatWeight";
    }
    mcFileName += ".root";
    file_mc = TFile::Open(mcFileName.c_str());
  }

  DrawBase* db = new DrawBase("qgdatamc");
if(selectionType=="ZJets")
  {
  db->set_lumi(18300.);
	std::cout<<"->Lumi is 18.3 FB"<<std::endl;
  }
if(selectionType=="DiJets")
  {
  db->set_lumi(0.0131);
	std::cout<<"->Lumi is 13.1 NB"<<std::endl;
  }

  std::string outputdir = "QGLDataMCPlots_" + selectionType;
  if( use_MB && selectionType=="DiJets" )
    outputdir += "_MB";
  if( use_tnp )
    outputdir += "_TandP";
  if( use_pthatweight )
    outputdir += "_ptHat";
  if( use_herwig )
    outputdir += "_Hpp";
  db->set_outputdir(outputdir);

  db->add_dataFile(file_data, "data");
  db->add_mcFile(file_mc, "mc", "mc_process");

  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptJet[0]", "ptJet", "Jet p_{T}", "GeV", 0., 10000., 0., 10., 100, 20., 520., 1, true);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptJet[0]", "ptJet", "Jet p_{T}", "GeV", 0., 10000., 0., 2., 100, 20., 520., 1, true);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptJet[0]", "ptJet", "Jet p_{T}", "GeV", 0., 10000., 3., 4.7, 100, 20., 320., 1, true);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "etaJet[0]", "etaJet", "Jet #eta", "", 0., 10000., 0., 10., 50., -5.5, 5.5);
//  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "etaJet[0]", "etaJet", "Jet #eta", "", 40., 10000., 0., 10., 50., -5.5, 5.5);
//  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "etaJet[0]", "etaJet", "Jet #eta", "", 80., 10000., 0., 10., 50., -5.5, 5.5);
//
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "rhoPF", "rhoPF", "Particle Flow Energy Density (#rho)", "GeV", 0., 10000., 0., 10., 80, 0., 40.);
//
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nvertex", "nvertex", "Number of Primary Vertexes", "", 0., 10000., 0., 10., 40, 0.5, 40.5);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nvertex", "nvertex", "Number of Primary Vertexes", "", 0., 10000., 0., 2.,  40, 0.5, 40.5);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nvertex", "nvertex", "Number of Primary Vertexes", "", 0., 10000., 3., 4.7, 40, 0.5, 40.5);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nvertex", "nvertex", "Number of Primary Vertexes", "", 40., 10000., 0., 10., 40, 0.5, 40.5);


  // central:
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 20., 30., 0., 2., 50);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 20., 30., 0., 2., 50, 0., 0.2);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 20., 30., 0., 2., 40, 0.5, 40.5);

//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 30., 40., 0., 2., 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 30., 40., 0., 2., 50, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 30., 40., 0., 2., 40, 0.5, 40.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 40., 50., 0., 2., 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 40., 50., 0., 2., 50, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 40., 50., 0., 2., 40, 0.5, 40.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 50., 65., 0., 2., 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 50., 65., 0., 2., 50, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 50., 65., 0., 2., 40, 0.5, 40.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 65., 80., 0., 2., 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 65., 80., 0., 2., 50, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 65., 80., 0., 2., 40, 0.5, 40.5);

  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 80., 100., 0., 2., 50);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 80., 100., 0., 2., 48, 0., 0.15);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 80., 100., 0., 2., 40, 0.5, 40.5);

//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 150., 200., 0., 2., 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 150., 200., 0., 2., 48, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 150., 200., 0., 2., 40, 0.5, 40.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 200., 600., 0., 2., 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 200., 600., 0., 2., 50, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Number of Constituents", "", 200., 600., 0., 2., 40, 0.5, 40.5);


  //// transition:
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 50., 65., 2., 3., 50);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 50., 65., 2., 3., 50, 0., 0.2);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 50., 65., 2., 3., 35, 0.5, 35.5);





  // forward:
  //int scaleBins = (selectionType=="ZJets") ? 1 : 2;
  int scaleBins = 1;
  
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 20., 30., 3., 4.7, 50);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 20., 30., 3., 4.7, 40, 0., 0.2);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 20., 30., 3., 4.7, 30, 0.5, 30.5);
  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]-1", "multiplicityCorrJet", "Multiplicity", "", 20., 30., 3., 4.7, 30, 0.5, 30.5);

//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 30., 40., 3., 4.7, 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 30., 40., 3., 4.7, 40, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 30., 40., 3., 4.7, 30, 0.5, 30.5);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]-1", "multiplicityCorrJet", "Multiplicity", "", 30., 40., 3., 4.7, 30, 0.5, 30.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 40., 50., 3., 4.7, 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 40., 50., 3., 4.7, 40, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 40., 50., 3., 4.7, 30, 0.5, 30.5);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]-1", "multiplicityCorrJet", "Multiplicity", "", 40., 50., 3., 4.7, 30, 0.5, 30.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 50., 65., 3., 4.7, 50);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 50., 65., 3., 4.7, 40, 0., 0.2);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 50., 65., 3., 4.7, 30, 0.5, 30.5);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]-1", "multiplicityCorrJet", "Multiplicity", "", 50., 65., 3., 4.7, 30, 0.5, 30.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 65., 80., 3., 4.7, 25*scaleBins);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 65., 80., 3., 4.7, 20*scaleBins, 0., 0.15);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 65., 80., 3., 4.7, 15*scaleBins, 0.5, 30.5);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]-1", "multiplicityCorrJet", "Multiplicity", "", 65., 80., 3., 4.7, 15*scaleBins, 0.5, 30.5);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "ptD_QCJet[0]", "ptDJet", "p_{T}D", "", 80., 100., 3., 4.7, 25*scaleBins);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "axis2_QCJet[0]", "axis2Jet", "#sigma_{2}", "", 80., 100., 3., 4.7, 20*scaleBins, 0., 0.12);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]", "multiplicityJet", "Multiplicity", "", 80., 100., 3., 4.7, 15*scaleBins, 0.5, 30.5);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "nChg_QCJet[0]+nNeutral_ptCutJet[0]-1", "multiplicityCorrJet", "Multiplicity", "", 80., 100., 3., 4.7, 15*scaleBins, 0.5, 30.5);



  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 30., 40., 0., 2.);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 0., 2.);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "ptJet[0]>30. && ptJet[1]>30.", "qglJet[0]", "qglJet_jetpt30", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 0., 2.);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "ptJet[0]>40. && ptJet[1]>40.", "qglJet[0]", "qglJet_jetpt40", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 0., 2.);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 65., 80., 0., 2.);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 80., 100., 0., 2.);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 150., 200., 0., 2.);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 200., 600., 0., 2.);

  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 20., 30., 3., 4.7);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 30., 40., 3., 4.7);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 3., 4.7);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 50., 65., 3., 4.7);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 65., 80., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "ptJet[0]>30. && ptJet[1]>30.", "qglJet[0]", "qglJet_jetpt30", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "ptJet[0]>40. && ptJet[1]>40.", "qglJet[0]", "qglJet_jetpt40", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 3., 4.7);
  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 80., 100., 3., 4.7);

  //drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglNoMultCorrJet[0]", "qglNoMultCorrJet", "Quark-Gluon Likelihood Discriminant", "", 20., 30., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglNoMultCorrJet[0]", "qglNoMultCorrJet", "Quark-Gluon Likelihood Discriminant", "", 30., 40., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglNoMultCorrJet[0]", "qglNoMultCorrJet", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglNoMultCorrJet[0]", "qglNoMultCorrJet", "Quark-Gluon Likelihood Discriminant", "", 50., 65., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglNoMultCorrJet[0]", "qglNoMultCorrJet", "Quark-Gluon Likelihood Discriminant", "", 65., 80., 3., 4.7);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglNoMultCorrJet[0]", "qglNoMultCorrJet", "Quark-Gluon Likelihood Discriminant", "", 80., 100., 3., 4.7);
//
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 2., 3.);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 80., 100., 2., 3.);
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "", "qglJet[0]", "qglJet", "Quark-Gluon Likelihood Discriminant", "", 200., 250., 2., 3.);
//
//  drawHistoWithQuarkGluonComponents( selectionType, db, "tree_passedEvents", "axis1_QCJet[0]*axis1_QCJet[0]+axis2_QCJet[0]*axis2_QCJet[0]<0.06", "qglJet[0]", "qglJet_PUID", "Quark-Gluon Likelihood Discriminant", "", 40., 50., 3., 4.7);

  return 0;

}












void drawHistoWithQuarkGluonComponents( const std::string& selectionType, DrawBase* db, const std::string& treeName, const std::string& additionalCuts, const std::string& varName, const std::string& canvasSaveName, std::string axisName, const std::string& units, float ptMin, float ptMax, float etaMin, float etaMax, int nBins, float xMin, float xMax, bool legendQuadrant, bool log_aussi ) {


  bool tag_and_probe = (selectionType=="DiJets" && use_tagAndProbe);


  TString varName_tstr(varName);



  TH1D* h1_data = new TH1D( "data", "", nBins, xMin, xMax );

  TH1D* h1_all = new TH1D( "all", "", nBins, xMin, xMax );
  TH1D* h1_quark = new TH1D( "quark", "", nBins, xMin, xMax );
  TH1D* h1_gluon = new TH1D( "gluon", "", nBins, xMin, xMax );
  TH1D* h1_pu = new TH1D( "pu", "", nBins, xMin, xMax );
  TH1D* h1_b = new TH1D( "b", "", nBins, xMin, xMax );

  //// these ones for all processes (to get the fractions right):
  //TH1D* h1_all_all = new TH1D( "all_all", "", nBins, xMin, xMax );
  //TH1D* h1_quark_all = new TH1D( "quark_all", "", nBins, xMin, xMax );
  //TH1D* h1_gluon_all = new TH1D( "gluon_all", "", nBins, xMin, xMax );
  //TH1D* h1_pu_all = new TH1D( "pu_all", "", nBins, xMin, xMax );
  //TH1D* h1_b_all = new TH1D( "b_all", "", nBins, xMin, xMax );


  int jet_ptindex = 0;
  if( tag_and_probe ) jet_ptindex = 1;

  std::string varName_mc(varName);
  if( varName=="nChg_QCJet[0]+nNeutral_ptCutJet[0]-1" )  // this is the fwd multiplicity correction, so apply only to data
    varName_mc = "nChg_QCJet[0]+nNeutral_ptCutJet[0]";
  

  char commonCondition[500];
  if( additionalCuts!="" )
    sprintf( commonCondition, "%s && ptJet[%d]>%f && ptJet[%d]<%f && abs(etaJet[0])>=%f && abs(etaJet[0])<%f", additionalCuts.c_str(), jet_ptindex, ptMin, jet_ptindex, ptMax, etaMin, etaMax );
  else
    sprintf( commonCondition, "ptJet[%d]>%f && ptJet[%d]<%f && abs(etaJet[0])>=%f && abs(etaJet[0])<%f", jet_ptindex, ptMin, jet_ptindex, ptMax, etaMin, etaMax );



  char allCondition[800];
  sprintf( allCondition,   "eventWeight*(%s)", commonCondition );
  char quarkCondition[800];
  sprintf( quarkCondition, "eventWeight*(abs(pdgIdJet[0])<6  && abs(pdgIdJet[0])!=0 && %s)", commonCondition );
  char gluonCondition[800];
  sprintf( gluonCondition, "eventWeight*(pdgIdJet[0]==21     && abs(pdgIdJet[0])!=0 && %s)", commonCondition );
  //char bCondition[800];
  //sprintf( bCondition,     "eventWeight*(abs(pdgIdJet[0])==5 && abs(pdgIdJet[0])!=0 && %s)", commonCondition );
  char puCondition[800];
  sprintf( puCondition,     "eventWeight*(pdgIdJet[0]==0 && %s)", commonCondition );



  TTree* treeDATA = (TTree*)(db->get_dataFile(0).file->Get(treeName.c_str()));
  treeDATA->Project( "data", varName.c_str(), commonCondition );


  TChain* treeMC= new TChain(treeName.c_str());
  //// this one to get the shapes (avoid huge QCD weights for gamma+jet):
  //TChain* treeMC_signal = new TChain(treeName.c_str());
  for( unsigned iFile=0; iFile<db->get_mcFiles().size(); ++iFile ) {
    std::string fileName(db->get_mcFile(iFile).file->GetName());
    std::string treeFullName = fileName + "/" + treeName;
    treeMC->Add(treeFullName.c_str());
    //if( iFile==0 ) //signal only
    //  treeMC_signal->Add(treeFullName.c_str());
  }

  treeMC->Project( "all",   varName_mc.c_str(), allCondition );
  treeMC->Project( "quark", varName_mc.c_str(), quarkCondition );
  treeMC->Project( "gluon", varName_mc.c_str(), gluonCondition );
  treeMC->Project( "pu", varName_mc.c_str(), puCondition );
  //treeMC->Project( "b", varName_mc.c_str(), bCondition );

  //treeMC_all->Project( "all_all",   varName.c_str(), allCondition );
  //treeMC_all->Project( "quark_all", varName.c_str(), quarkCondition );
  //treeMC_all->Project( "gluon_all", varName.c_str(), gluonCondition );
  //treeMC_all->Project( "pu_all", varName.c_str(), puCondition );
  //treeMC_all->Project( "b_all", varName.c_str(), bCondition );




  bool isJetVar = varName_tstr.Contains("Jet[0]");

  if( tag_and_probe && isJetVar ) { //add also other jet

    char commonCondition2[500];
    if( additionalCuts!="" )
      sprintf( commonCondition2, "%s && ptJet[0]>%f && ptJet[0]<%f && abs(etaJet[1])>=%f && abs(etaJet[1])<%f", additionalCuts.c_str(), ptMin, ptMax, etaMin, etaMax );
    else
      sprintf( commonCondition2, "ptJet[0]>%f && ptJet[0]<%f && abs(etaJet[1])>=%f && abs(etaJet[1])<%f", ptMin, ptMax, etaMin, etaMax );

    TH1D* h1_data2 = new TH1D( "data2", "", nBins, xMin, xMax );

    TH1D* h1_all2 = new TH1D( "all2", "", nBins, xMin, xMax );
    TH1D* h1_quark2 = new TH1D( "quark2", "", nBins, xMin, xMax );
    TH1D* h1_gluon2 = new TH1D( "gluon2", "", nBins, xMin, xMax );
    TH1D* h1_pu2 = new TH1D( "pu2", "", nBins, xMin, xMax );
    TH1D* h1_b2 = new TH1D( "b2", "", nBins, xMin, xMax );

    TString varName_tstr2(varName);
    varName_tstr2.ReplaceAll("[0]", "[1]");

    TString varName_mc_tstr2(varName_mc);
    varName_mc_tstr2.ReplaceAll("[0]", "[1]");

    treeDATA->Project( "data2", varName_tstr2, commonCondition2 );
      
    char allCondition2[800];
    sprintf( allCondition2,   "eventWeight*(%s)", commonCondition2 );
    char quarkCondition2[800];
    sprintf( quarkCondition2, "eventWeight*(abs(pdgIdJet[1])<6  && abs(pdgIdJet[1])!=0 && %s)", commonCondition2 );
    char gluonCondition2[800];
    sprintf( gluonCondition2, "eventWeight*(pdgIdJet[1]==21     && abs(pdgIdJet[1])!=0 && %s)", commonCondition2 );
    //char bCondition2[800];
    //sprintf( bCondition2,     "eventWeight*(abs(pdgIdJet[1])==5 && abs(pdgIdJet[1])!=0 && %s)", commonCondition2 );
    char puCondition2[800];
    sprintf( puCondition2,     "eventWeight*(pdgIdJet[1]==0 && %s)", commonCondition2 );

    treeMC->Project( "all2",   varName_mc_tstr2, allCondition2 );
    treeMC->Project( "quark2", varName_mc_tstr2, quarkCondition2 );
    treeMC->Project( "gluon2", varName_mc_tstr2, gluonCondition2 );
    treeMC->Project( "pu2", varName_mc_tstr2, puCondition2 );
    //treeMC->Project( "b2", varName_mc_tstr2, bCondition2 );

    h1_data->Add( h1_data2 );

    h1_all->Add( h1_all2 );
    h1_quark->Add( h1_quark2 );
    h1_gluon->Add( h1_gluon2 );
    h1_pu->Add( h1_pu2 );
    //h1_b->Add( h1_b2 );

    delete h1_data2;

    delete h1_all2;
    delete h1_quark2;
    delete h1_gluon2;
    delete h1_pu2;
    delete h1_b2;

  }




  float data_int = h1_data->Integral();
  float mc_int = h1_all->Integral();
  //float mc_int_all = h1_all_all->Integral();
  float scaleFactor = data_int/mc_int;

  float quark_fraction = h1_quark->Integral()/mc_int;
  float gluon_fraction = h1_gluon->Integral()/mc_int;
  float pu_fraction = h1_pu->Integral()/mc_int;
  float b_fraction = h1_b->Integral()/mc_int;
  float other_fraction = 1.-quark_fraction-gluon_fraction-b_fraction;


  //h1_all->Scale( h1_all_all->Integral()/h1_all->Integral() );
  //h1_gluon->Scale( h1_gluon_all->Integral()/h1_gluon->Integral() );
  //h1_pu->Scale( h1_pu_all->Integral()/h1_pu->Integral() );
  //h1_quark->Scale( h1_quark_all->Integral()/h1_quark->Integral() );
  //h1_b->Scale( h1_b_all->Integral()/h1_b->Integral() );
  

  char quarkText[300];
  sprintf( quarkText, "Quark");
  char gluonText[300];
  sprintf( gluonText, "Gluon");
  char bText[300];
  sprintf( bText, "b");
  char puText[300];
  sprintf( puText, "Unmatched+PU");
  //sprintf( puText, "Pile Up");
  char otherText[300];
  sprintf( otherText, "Undefined");

  //char quarkText[300];
  //sprintf( quarkText, "udsc (%.1f%%)", 100.*quark_fraction );
  //char gluonText[300];
  //sprintf( gluonText, "Gluons (%.1f%%)", 100.*gluon_fraction );
  //char bText[300];
  //sprintf( bText, "b (%.1f%%)", 100.*b_fraction );
  //char puText[300];
  //sprintf( puText, "Pile Up (%.1f%%)", 100.*pu_fraction );
  //char otherText[300];
  //sprintf( otherText, "Undefined (%.1f%%)", 100.*other_fraction );


  float xMin_leg = 0.32;
  float xMax_leg = 0.8;

  if( legendQuadrant==1 ) {
    xMin_leg = 0.52;
    xMax_leg = 0.93;
  }
  
  //TLegend* legend;
  //if( (ptMin !=0. || ptMax != 10000.) && (rhoMin!=0. || rhoMax !=30.) ) {
  //  char legendTitle[250];
  //  if( varName=="QGLikelihoodJet0" ) {
  //    sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV,  %.0f < #rho < %.0f GeV", ptMin, ptMax, rhoMin, rhoMax );
  //    legend = new TLegend( 0.32, 0.55, 0.8, 0.9, legendTitle );
  //  } else {
  //    sprintf( legendTitle, "#splitline{%.0f < p_{T} < %.0f GeV}{%.0f < #rho < %.0f GeV}", ptMin, ptMax, rhoMin, rhoMax );
  //    legend = new TLegend( xMin_leg, 0.5, xMax_leg, 0.9, legendTitle );
  //  }
  //} else if( ptMin !=0. && ptMax != 10000. ) {
  //  char legendTitle[150];
  //  sprintf( legendTitle, "%.0f < p_{T} < %.0f GeV", ptMin, ptMax);
  //  legend = new TLegend( xMin_leg, 0.55, xMax_leg, 0.9, legendTitle );
  //} else {
  //  legend = new TLegend( xMin_leg, 0.6, xMax_leg, 0.9 );
  //}

  std::string selectionType_text;
  if( selectionType=="ZJets" ) selectionType_text = "Z+Jets";
  if( selectionType=="DiJets" ) selectionType_text = "DiJets";

  TLegend* legend = new TLegend( xMin_leg, 0.6, xMax_leg, 0.91, selectionType_text.c_str() );
  legend->SetFillColor( kWhite );
  legend->SetTextSize(0.038);
  legend->AddEntry( h1_data, "Data", "p" );
  legend->AddEntry( h1_quark, quarkText, "F" );
  legend->AddEntry( h1_gluon, gluonText, "F" );
  legend->AddEntry( h1_pu, puText, "F" );
  //legend->AddEntry( h1_b, bText, "F" );
  //legend->AddEntry( h1_all, otherText, "F" );

  TPaveText* cutsText = new TPaveText(0.22, 0.77, 0.45, 0.93, "brNDC");
  cutsText->SetTextSize(0.038);
  cutsText->SetTextFont(42);
  cutsText->SetFillColor(0);

  char ptCuts[200];
  if( ptMin!=0. && ptMax!=10000. )
    sprintf( ptCuts, "%.0f < p_{T} < %.0f GeV", ptMin, ptMax);
  else if( ptMin!=0. && ptMax==10000. )
    sprintf( ptCuts, "p_{T} > %.0f GeV", ptMin );
  else 
    sprintf( ptCuts, "" );
  std::string ptCuts_str(ptCuts);

  char etaCuts[200];
  if( etaMin!=0. && etaMax!=10. )
    sprintf( etaCuts, "%.0f < |#eta| < %.0f", etaMin, etaMax);
  else if( etaMin==0. && etaMax!=10. )
    sprintf( etaCuts, "|#eta| < %.0f", etaMax );
  else 
    sprintf( etaCuts, "" );
  std::string etaCuts_str(etaCuts);

  char cutsText_char[500];
  if( ptCuts_str!="" && etaCuts_str!="" )
    sprintf( cutsText_char, "#splitline{%s}{%s}", ptCuts_str.c_str(), etaCuts_str.c_str());
  else if( ptCuts_str!="" )
    sprintf( cutsText_char, "%s", ptCuts_str.c_str());
  else if( etaCuts_str!="" )
    sprintf( cutsText_char, "%s", etaCuts_str.c_str());
  else 
    sprintf( cutsText_char, "" );

  cutsText->AddText(cutsText_char);

  h1_all->Rebin( db->get_rebin() );
  h1_gluon->Rebin( db->get_rebin() );
  h1_pu->Rebin( db->get_rebin() );
  h1_quark->Rebin( db->get_rebin() );
  h1_b->Rebin( db->get_rebin() );
  h1_data->Rebin( db->get_rebin() );
  
  h1_all->Scale( scaleFactor );
  h1_gluon->Scale( scaleFactor );
  h1_pu->Scale( scaleFactor );
  h1_quark->Scale( scaleFactor );
  h1_b->Scale( scaleFactor );
  
//  h1_all_all->Rebin( db->get_rebin() );
//  h1_all_gluon->Rebin( db->get_rebin() );
//  h1_all_pu->Rebin( db->get_rebin() );
//  h1_all_quark->Rebin( db->get_rebin() );
//  h1_all_b->Rebin( db->get_rebin() );
//  
//  h1_all_all->Scale( scaleFactor );
//  h1_all_gluon->Scale( scaleFactor );
//  h1_all_pu->Scale( scaleFactor );
//  h1_all_quark->Scale( scaleFactor );
//  h1_all_b->Scale( scaleFactor );
//  
  h1_data->SetMarkerStyle( 20 );
  h1_data->SetMarkerSize( 1. );
  h1_all->SetFillColor( kGray );
  h1_gluon->SetFillColor( 46 );
  h1_pu->SetFillColor( kGray );
  //h1_pu->SetFillColor( 30 );
  h1_quark->SetFillColor( 38 );
  h1_b->SetFillColor( kYellow );

//  h1_all_all->SetFillColor( kGray );
//  h1_all_gluon->SetFillColor( 46 );
//  h1_all_pu->SetFillColor( 30 );
//  h1_all_quark->SetFillColor( 38 );
//  h1_all_b->SetFillColor( kYellow );
//
  THStack* stack = new THStack();
  stack->Add(h1_pu);
  //stack->Add(h1_b);
  stack->Add(h1_gluon );
  stack->Add(h1_quark);

//  THStack* stack_all = new THStack();
//  stack_all->Add(h1_all_gluon );
//  stack_all->Add(h1_all_quark);
//  stack_all->Add(h1_all_pu);
//  stack_all->Add(h1_all_b);
//
  float dataMax = h1_data->GetMaximum();
  float mcMax = h1_all->GetMaximum();
  float yMax = (dataMax>mcMax) ? dataMax : mcMax;
  yMax *= db->get_yAxisMaxScale();


  TPaveText* cmsLabel = db->get_labelCMS();
  TPaveText* sqrtLabel = db->get_labelTop();

  char yAxisTitle[200];
  std::string units_text = (units!="") ? (" "+units) : "";
  if( (h1_data->GetBinWidth(1)) < 0.01 )
    sprintf( yAxisTitle, "Events / (%.3f%s)", h1_data->GetBinWidth(1), units_text.c_str() );
  else if( (h1_data->GetBinWidth(1)) < 0.1 )
    sprintf( yAxisTitle, "Events / (%.2f%s)", h1_data->GetBinWidth(1), units_text.c_str() );
  else if( ((int)(10.*h1_data->GetBinWidth(1)) % 10) == 0 )
    sprintf( yAxisTitle, "Events / (%.0f%s)", h1_data->GetBinWidth(1), units_text.c_str() );
  else
    sprintf( yAxisTitle, "Events / (%.1f%s)", h1_data->GetBinWidth(1), units_text.c_str() );


  if( units!="" ) axisName = axisName + " [" + units + "]";


  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, xMax, 10, 0., yMax );
  h2_axes->SetXTitle( axisName.c_str() );
  h2_axes->SetYTitle( yAxisTitle );
  if( yMax>1000. )
    h2_axes->GetYaxis()->SetTitleOffset(1.55); 

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  h2_axes->Draw();
  legend->Draw("same");
  cutsText->Draw("same");
  h1_all->Draw("same");
  stack->Draw("histo same");
  h1_data->Draw("e same");
  sqrtLabel->Draw("Same");

  gPad->RedrawAxis();

  //std::string canvasName = db->get_outputdir() + "/" + varName + "_components.eps";

  std::string etaSuffix="";
  if( etaMin!=0. || etaMax!=10. ) {
    if( etaMax>4. ) etaSuffix = "_fwd";
    else if( etaMax>2. ) etaSuffix = "_trans";
    else  etaSuffix = "_centr";
  }
  char canvasNameChar[400];
  if( ptMin!=0. || ptMax !=10000. )
    sprintf( canvasNameChar, "%s/%s_pt%d%d%s.eps", db->get_outputdir().c_str(), canvasSaveName.c_str(), (int)ptMin, (int)ptMax, etaSuffix.c_str() );
  else
    sprintf( canvasNameChar, "%s/%s%s.eps", db->get_outputdir().c_str(), canvasSaveName.c_str(), etaSuffix.c_str() );

  c1->SaveAs(canvasNameChar);
  std::string epstopdf_command(canvasNameChar);
  epstopdf_command = "epstopdf " + epstopdf_command;
  system(epstopdf_command.c_str());

  if( log_aussi ) {

    c1->Clear();
    c1->SetLogy();

    float ymin_log = 0.1;

    TH2D* h2_axes_log = new TH2D("axes_log", "", 10, xMin, xMax, 10, ymin_log, yMax*10 );
    h2_axes_log->SetXTitle( axisName.c_str() );
    h2_axes_log->SetYTitle( yAxisTitle );


    h2_axes_log->Draw();
    legend->Draw("same");
    cutsText->Draw("same");
    h1_all->Draw("same");
    stack->Draw("histo same");
    h1_data->Draw("e same");
    sqrtLabel->Draw("Same");

    gPad->RedrawAxis();

    char canvasNameChar_log[400];
    if( ptMin!=0. && ptMax !=10000. )
      sprintf( canvasNameChar_log, "%s/%s_pt%d%d%s_log.eps", db->get_outputdir().c_str(), canvasSaveName.c_str(), (int)ptMin, (int)ptMax, etaSuffix.c_str() );
    else
      sprintf( canvasNameChar_log, "%s/%s%s_log.eps", db->get_outputdir().c_str(), canvasSaveName.c_str(), etaSuffix.c_str() );

    c1->SaveAs(canvasNameChar_log);
    std::string epstopdf_command_log(canvasNameChar_log);
    epstopdf_command_log = "epstopdf " + epstopdf_command_log;
    system(epstopdf_command_log.c_str());


    delete h2_axes_log;

  }
  
  delete c1;
  delete h2_axes;

  delete h1_data;
  delete h1_all;
  delete h1_quark;
  delete h1_gluon;
  delete h1_pu;
  delete h1_b;
  
  //delete h1_all_all;
  //delete h1_quark_all;
  //delete h1_gluon_all;
  //delete h1_pu_all;
  //delete h1_b_all;
  
}

