#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void TMVAClassification(std::string iAna="EMu") {
   TMVA::Tools::Instance();
  std::cout << "==> Start TMVAClassification" << std::endl;

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   TMVA::Factory *factory = new TMVA::Factory( "mva", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";
   //factory->AddVariable( "run"  , 'F' );
   factory->AddVariable( "l1_Px", 'F' );  //Lepton 1 Pt
   factory->AddVariable( "l1_Py", 'F' );  //Lepton 1 Eta
   factory->AddVariable( "l1_Pz", 'F' );  //Lepton 1 Phi
   factory->AddVariable( "l2_Px", 'F' );  //Lepton 2 Pt
   factory->AddVariable( "l2_Py", 'F' );  //Letpon 2 eta
   factory->AddVariable( "l2_Pz", 'F' );  //Lepton 2 Phi
   factory->AddVariable( "m_sv", 'F' );   //SV Fit mass (from local fit)
   factory->AddVariable( "m_svfit", 'F' );   //SV Fit mass (from local fit)
   //factory->AddVariable( "phi_sv", 'F' ); //SV Fit Phi  (from local fit)
   factory->AddVariable( "pt_sv", 'F' );  //SV Fit Pt
   //factory->AddVariable( "y_sv", 'F' );   //SV Fit y
   factory->AddVariable( "met"  , 'F' );  //MET
   factory->AddVariable( "mphi" , 'F' );  //Met Phi
   //factory->AddVariable( "m_true" , 'F' );
   //factory->AddVariable( "phi_vis", 'F' ); //SV Fit Mass
   //factory->AddVariable( "m_vis", 'F' ); //SV Fit Mass
   //factory->AddVariable( "pt_vis", 'F' ); //SV Fit Mass
   //factory->AddVariable( "y_vis", 'F' ); //SV Fit Mass
   
   //factory->AddSpectator( "m_true" , 'F' );
   //factory->AddSpectator( "m_svfit", 'F' );
   //std::string lAna = "EMu";//"MuTau"
   TString lSName       = "zjets_mc_3.root"; TFile *lSInput = TFile::Open( lSName );
   TTree   *lSignal     = (TTree*)lSInput    ->Get(iAna.c_str());
   TString lBName       = "higgs_mc_3.root"; TFile *lBInput = TFile::Open( lBName );
   TTree   *lBackground = (TTree*)lBInput    ->Get(iAna.c_str());
   
   // global event weights per tree (see below for setting event-wise weights)
   Double_t lSWeight = 1.0;
   Double_t lBWeight = 1.0;
   // You can add an arbitrary number of signal or background trees
   factory->AddSignalTree    ( lSignal,     lSWeight     );
   factory->AddBackgroundTree( lBackground, lBWeight );
   TCut lBCut = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut lSCut = ""; // for example: TCut mycutb = "abs(var1)<0.5";

   // Tell the factory how to use the training and testing events
   factory->PrepareTrainingAndTestTree( lSCut, lBCut,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   //factory->BookMethod( TMVA::Types::kCuts, "Cuts",
   //"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   // Use a kernel density estimator to approximate the PDFs
   //factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE",
   //"!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 

   // Use a variable-dependent mix of splines and kernel density estimator
   //factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX",
   //"!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 
   
   // Boosted Decision Trees
   factory->BookMethod( TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5" );
   //factory->BookMethod( TMVA::Types::kBDT, "BDT",
   //"!H:!V:NTrees=850:nEventsMin=150:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
   // Train MVAs using the set of training events
   factory->TrainAllMethods();
   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();
   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();
   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;
   delete factory;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVAGui( outfileName );
}
