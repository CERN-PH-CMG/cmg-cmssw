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

#include "TMVA/TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/MethodCategory.h"
#include "TMVA/Tools.h"
#endif

void classify(std::string iId="",int iKinType=0,int iIPType=0,int iNParType=0,int iDRType=0,int iPtRType=0,int iType=0) {
   TMVA::Tools::Instance();
  std::cout << "==> Start TMVAClassification" << std::endl;
  
  TString allUsedVars = "";
   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassificationCategory", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );
   if(iKinType   == 0)    {factory->AddSpectator( "jspt_1"   , 'F' ); }
   if(iKinType   == 0)    {factory->AddSpectator( "jseta_1"  , 'F' ); }

   factory->AddVariable( "npv"      , 'F' );        allUsedVars += "npv"   ;
   //factory->AddVariable( "rho"      , 'F' );        allUsedVars += "rho"   ;
   if(iKinType   > 0)    {factory->AddVariable( "jspt_1"   , 'F' ); allUsedVars += ":jspt_1"   ;}
   if(iKinType   > 1)    {factory->AddVariable( "jseta_1"  , 'F' ); allUsedVars += ":jseta_1"  ;}
   if(iKinType   > 2)    {factory->AddVariable( "jsphi_1"  , 'F' ); allUsedVars += ":jsphi_1"  ;}
   if(iIPType    > 0)    {factory->AddVariable( "jdZ_1"    , 'F' ); allUsedVars += ":jdZ_1"    ;}
   if(iIPType    > 1)    {factory->AddVariable( "jd0_1"    , 'F' ); allUsedVars += ":jd0_1"    ;}
   if(iNParType  > 0)    {factory->AddVariable( "npart_1"  , 'F' ); allUsedVars += ":npart_1"  ;}
   if(iDRType    > 0)    {factory->AddVariable( "drlc_1"   , 'F' ); allUsedVars += ":drlc_1"   ;}
   if(iDRType    > 0)    {factory->AddVariable( "drls_1"   , 'F' ); allUsedVars += ":drls_1"   ;}
   if(iDRType    > 0)    {factory->AddVariable( "drm_1"    , 'F' ); allUsedVars += ":drm_1"    ;}
   if(iDRType    > 0)    {factory->AddVariable( "drmne_1"  , 'F' ); allUsedVars += ":drmne_1"  ;}
   if(iDRType    > 0)    {factory->AddVariable( "drem_1"   , 'F' ); allUsedVars += ":drem_1"   ;}
   if(iDRType    > 0)    {factory->AddVariable( "drch_1"   , 'F' ); allUsedVars += ":drch_1"   ;}
   if(iPtRType   > 0 )   {factory->AddVariable( "lpt_1/jspt_1"    , 'F' ); allUsedVars += ":lpt_1/jspt_1"    ;}
   if(iPtRType   > 0 )   {factory->AddVariable( "spt_1/jspt_1"    , 'F' ); allUsedVars += ":spt_1/jspt_1"    ;}
   if(iPtRType   > 0 )   {factory->AddVariable( "lnept_1/jspt_1"  , 'F' ); allUsedVars += ":lnept_1/jspt_1"  ;}
   if(iPtRType   > 0 )   {factory->AddVariable( "lempt_1/jspt_1"  , 'F' ); allUsedVars += ":lempt_1/jspt_1"  ;}
   if(iPtRType   > 0 )   {factory->AddVariable( "lchpt_1/jspt_1"  , 'F' ); allUsedVars += ":lchpt_1/jspt_1"  ;}

   if(iType > 0 ) {factory->AddVariable( "lpt_1"           , 'F' ); allUsedVars += ":lpt_1"           ;}
   if(iType > 0)  {factory->AddVariable( "leta_1"          , 'F' ); allUsedVars += ":leta_1"          ;}
   if(iType > 0)  {factory->AddVariable( "lphi_1"          , 'F' ); allUsedVars += ":lphi_1"          ;}
   if(iType > 0)  {factory->AddVariable( "spt_1       "    , 'F' ); allUsedVars += ":spt_1"           ;}
   if(iType > 0)  {factory->AddVariable( "seta_1"          , 'F' ); allUsedVars += ":seta_1"          ;}
   if(iType > 0)  {factory->AddVariable( "sphi_1"          , 'F' ); allUsedVars += ":sphi_1"          ;}
   if(iType > 0)  {factory->AddVariable( "lnept_1"         , 'F' ); allUsedVars += ":lnept_1"         ;}
   if(iType > 0)  {factory->AddVariable( "lneeta_1"        , 'F' ); allUsedVars += ":lneeta_1"        ;}
   if(iType > 0)  {factory->AddVariable( "lnephi_1"        , 'F' ); allUsedVars += ":lnephi_1"        ;}
   if(iType > 0)  {factory->AddVariable( "lempt_1"         , 'F' ); allUsedVars += ":lempt_1"         ;}
   if(iType > 0)  {factory->AddVariable( "lemeta_1"        , 'F' ); allUsedVars += ":lemeta_1"        ;}
   if(iType > 0)  {factory->AddVariable( "lemphi_1"        , 'F' ); allUsedVars += ":lemphi_1"        ;}
   if(iType > 0)  {factory->AddVariable( "lchpt_1"         , 'F' ); allUsedVars += ":lchpt_1"         ;}
   if(iType > 0)  {factory->AddVariable( "lchphi_1"        , 'F' ); allUsedVars += ":lchphi_1" ;}
   if(iType > 0)  {factory->AddVariable( "lLfr_1"          , 'F' ); allUsedVars += ":lLfr_1"   ;}
   if(iType > 0) {factory->AddVariable( "jm_1"             , 'F' ); allUsedVars += ":jm_1"     ;}

 
   TString lSName       = "../Jets/f11-zjets_ch_chs_rw.root";  TFile *lSInput = TFile::Open(lSName);
   TTree   *lSignal     = (TTree*)lSInput    ->Get("Flat");
   TString lBName       = "../Jets/f11-zjets_ch_chs_rw.root";  TFile *lBInput = TFile::Open(lSName);
   TTree   *lBackground = (TTree*)lBInput    ->Get("Flat");
   
   // global event weights per tree (see below for setting event-wise weights)
   Double_t lSWeight = 1.0;
   Double_t lBWeight = 1.0;
 
  // You can add an arbitrary number of signal or background trees
   factory->AddSignalTree    ( lSignal,     lSWeight );
   factory->AddBackgroundTree( lBackground, lBWeight );
   TCut lBCut = "jspt_1 >  10 && genfrac_1 < 0.4  && jspt_1 < 60";
   TCut lSCut = "jspt_1 >  10 && genfrac_1 > 0.4  && jspt_1 < 60";
   lBCut = "jspt_1 >  10 && genfrac_1 < 0.15  && jspt_1 < 60";
   lSCut = "jspt_1 >  10 && genfrac_1 > 0.65  && jspt_1 < 60";

   factory->SetWeightExpression("RhoWeight/PtWeight");
   //if(iType > 4 && iType < 7) factory->SetWeightExpression("1./PtWeight");
   factory->PrepareTrainingAndTestTree( lSCut, lBCut,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // Boosted Decision Trees
   TCut    lTKCut = "abs(jseta_1) < 2.5";
   TCut    lHECut = "abs(jseta_1) > 2.5 && abs(jseta_1) < 2.9 ";
   TCut    lHFCut = "abs(jseta_1) > 2.9 ";
   TMVA::MethodCategory* lCat  = 0; 
   //TString lDef   = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5";
   TString lDef   = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.6:nCuts=1000:NNodesMax=5:SeparationType=GiniIndex:UseYesNoLeaf=F:";
   TMVA::MethodBase*     lBase = factory->BookMethod( TMVA::Types::kCategory, ("JetID_"+iId).c_str(),"");
   lCat  = dynamic_cast<TMVA::MethodCategory*>(lBase);
   lCat->AddMethod(lTKCut,allUsedVars,TMVA::Types::kBDT,"TK",lDef);
   lCat->AddMethod(lHECut,allUsedVars,TMVA::Types::kBDT,"HE",lDef);
   lCat->AddMethod(lHFCut,allUsedVars,TMVA::Types::kBDT,"HF",lDef);

   factory->TrainAllMethods();
   factory->TestAllMethods();
   factory->EvaluateAllMethods();
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;
   delete factory;
   //if (!gROOT->IsBatch()) TMVAGui( outfileName );
}
