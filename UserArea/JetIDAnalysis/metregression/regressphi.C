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

#include "TMVA/TMVARegGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void regressphi() {
   TMVA::Tools::Instance();
   std::cout << "==> Start TMVAClassification" << std::endl;
  
   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   TMVA::Factory *factory = new TMVA::Factory( "mva", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar" );
   factory->AddVariable( "npv"                , 'F' ); 
   factory->AddVariable( "u"                  , 'F' ); 
   factory->AddVariable( "uphi"               , 'F' );
   factory->AddVariable( "chsumet/sumet"      , 'F' ); 
   factory->AddVariable( "tku"                , 'F' );
   factory->AddVariable( "tkuphi"             , 'F' );
   factory->AddVariable( "nopusumet/sumet"    , 'F' );
   factory->AddVariable( "nopuu"              , 'F' );
   factory->AddVariable( "nopuuphi"           , 'F' );
   factory->AddVariable( "pusumet/sumet"      , 'F' );
   factory->AddVariable( "pumet"              , 'F' );
   factory->AddVariable( "pumetphi"           , 'F' );
   factory->AddVariable( "pucsumet/sumet"     , 'F' );
   factory->AddVariable( "pucu"               , 'F' );
   factory->AddVariable( "pucuphi"            , 'F' );
   factory->AddVariable( "jspt_1"             , 'F' );
   factory->AddVariable( "jseta_1"            , 'F' );
   factory->AddVariable( "jsphi_1"            , 'F' );
   factory->AddVariable( "jspt_2"             , 'F' );
   factory->AddVariable( "jseta_2"            , 'F' );
   factory->AddVariable( "jsphi_2"            , 'F' );
   factory->AddVariable( "nalljet"            , 'I' );
   factory->AddVariable( "njet"               , 'I' );
    
   factory->AddTarget( "rphi_z-uphi+ 2.*TMath::Pi()*(rphi_z-uphi < -TMath::Pi()) - 2.*TMath::Pi()*(rphi_z-uphi > TMath::Pi())  " ); 
   TString  lName       = "../Jets/r11-dimu_nochs_v2.root";  TFile *lInput = TFile::Open(lName);
   TTree   *lRegress    = (TTree*)lInput    ->Get("Flat");

   Double_t lRWeight = 1.0;
   factory->AddRegressionTree( lRegress   , lRWeight );
   TCut lCut = "nbtag == 0"; //Cut to remove real MET
   //(rpt_z < 40 || (rpt_z > 40 && rpt_z+u1 < 40)) && nbtag == 0 "; ==> stronger cut to remove Real MET

   factory->PrepareTrainingAndTestTree( lCut,
					"nTrain_Regression=0:nTest_Regression=0:SplitMode=Block:NormMode=NumEvents:!V" );

   // Boosted Decision Trees
   factory->BookMethod( TMVA::Types::kBDT, "RecoilPhiRegress_data_clean2_njet",
			"!H:!V:VarTransform=None:nEventsMin=200:NTrees=100:BoostType=Grad:Shrinkage=0.1:MaxDepth=100:NNodesMax=100000:UseYesNoLeaf=F:nCuts=2000");//MaxDepth=100

   factory->TrainAllMethods();
   factory->TestAllMethods();
   factory->EvaluateAllMethods();
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;
   delete factory;
   //if (!gROOT->IsBatch()) TMVAGui( outfileName );
}
