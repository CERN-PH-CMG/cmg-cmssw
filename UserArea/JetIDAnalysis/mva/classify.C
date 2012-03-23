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

void classify(std::string iId="",
	      int iJetType=0,int iMVAType  =0,int iPUType =1,int iKinType =1,int iIPType =0,int iNParType=0,int iDRType=0,
	      int iPtRType=0,int iOrderType=0,int iPtDType=0,int iFracType=0,int iMomType=0,int iRingType=0,int iType  =0) {
  TMVA::Tools::Instance();
  std::cout << "==> Start TMVAClassification" << std::endl;
  
  TString allUsedVars = "";
  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TString outfileName( "TMVA.root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
  TMVA::Factory *factory = new TMVA::Factory( "TMVAClassificationCategory", outputFile,
					      "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );
  if(iKinType   == 0)    {factory->AddSpectator( "jetPt"   , 'F' ); }
  if(iKinType   == 0)    {factory->AddSpectator( "jetEta"  , 'F' ); }
  
  //PU correlation => nvtx data/MC agree better
  //factory->AddVariable( "rho"      , 'F' );        allUsedVars += "rho"   ;
  if(iPUType   >  0)    {factory->AddVariable( "nvtx"      , 'F' ); allUsedVars += "nvtx";       }
  
  //Jet pT+eta
  if(iKinType   > 0)    {factory->AddVariable( "jetPt"     , 'F' ); allUsedVars += ":jetPt"     ;}
  if(iKinType   > 1)    {factory->AddVariable( "jetEta"    , 'F' ); allUsedVars += ":jetEta"    ;}
  if(iKinType   > 2)    {factory->AddVariable( "jetPhi"    , 'F' ); allUsedVars += ":jetPhi"    ;}
  //Impact parameter 
  if(iIPType    > 0)    {factory->AddVariable( "dZ"        , 'F' ); allUsedVars += ":dZ"        ;}
  if(iIPType    > 1)    {factory->AddVariable( "d0"        , 'F' ); allUsedVars += ":d0"        ;}
  //Number of particles
  if(iNParType  > 0)    {factory->AddVariable( "nParticles", 'F' ); allUsedVars += ":nParticles";}
  if(iNParType  > 1)    {factory->AddVariable( "nCharged"  , 'F' ); allUsedVars += ":nParticles";} //Not Yet tried
  if(iNParType  > 1)    {factory->AddVariable( "nNeutral"  , 'F' ); allUsedVars += ":nParticles";} //Not Yet tried
  //Moments
  if(iDRType    > 0)    {factory->AddVariable( "dRLeadCent", 'F' ); allUsedVars += ":dRLeadCent";}
  if(iDRType    > 0)    {factory->AddVariable( "dRLead2nd" , 'F' ); allUsedVars += ":dRLead2nd" ;}
  if(iDRType    > 0)    {factory->AddVariable( "dRMean"    , 'F' ); allUsedVars += ":dRMean"    ;}
  if(iDRType    > 0)    {factory->AddVariable( "dRMeanNeut", 'F' ); allUsedVars += ":dRMeanNeut";}
  if(iDRType    > 0)    {factory->AddVariable( "dRMeanEm"  , 'F' ); allUsedVars += ":dRMeanEm"  ;}
  if(iDRType    > 0)    {factory->AddVariable( "dRMeanCh"  , 'F' ); allUsedVars += ":dRMeanCh"  ;}
  //Pt Ratios
  if(iPtRType   > 0 ) {factory->AddVariable( "leadChPt/jetPt"  , 'F' ); allUsedVars += ":leadChPt/jetPt"  ;}
  if(iPtRType   > 0 ) {factory->AddVariable( "secondPt/jetPt"  , 'F' ); allUsedVars += ":secondPt/jetPt"  ;}
  if(iPtRType   > 0 ) {factory->AddVariable( "leadNeutPt/jetPt", 'F' ); allUsedVars += ":lnept_1/jspt_1"  ;}
  if(iPtRType   > 0 ) {factory->AddVariable( "leadEmPt/jetPt"  , 'F' ); allUsedVars += ":lempt_1/jspt_1"  ;}
  if(iPtRType   > 0 ) {factory->AddVariable( "leadChPt/jetPt"  , 'F' ); allUsedVars += ":lchpt_1/jspt_1"  ;}
  
  //Old Variables from teh original training 
  if(iType > 0 )  {factory->AddVariable( "leadPt"          , 'F' ); allUsedVars += ":leadPt"           ;}
  if(iType > 0)   {factory->AddVariable( "leadEta"         , 'F' ); allUsedVars += ":leadEta"          ;}
  if(iType > 0)   {factory->AddVariable( "leadPhi"         , 'F' ); allUsedVars += ":leadPhi"          ;}
  if(iType > 0)   {factory->AddVariable( "secondPt"        , 'F' ); allUsedVars += ":secondPt"         ;}
  if(iType > 0)   {factory->AddVariable( "secondEta"       , 'F' ); allUsedVars += ":secondEta"        ;}
  if(iType > 0)   {factory->AddVariable( "secondPhi"       , 'F' ); allUsedVars += ":secondPhi"        ;}
  if(iType > 0)   {factory->AddVariable( "leadNeutPt"      , 'F' ); allUsedVars += ":leadNeutPt"       ;}
  if(iType > 0)   {factory->AddVariable( "leadNeutEta"     , 'F' ); allUsedVars += ":leadNeutEta"      ;}
  if(iType > 0)   {factory->AddVariable( "leadNeutPhi"     , 'F' ); allUsedVars += ":leadNeutPhi"      ;}
  if(iType > 0)   {factory->AddVariable( "leadEmPt"        , 'F' ); allUsedVars += ":leadEmPt"         ;}
  if(iType > 0)   {factory->AddVariable( "leadEmEta"       , 'F' ); allUsedVars += ":leadEmEta"        ;}
  if(iType > 0)   {factory->AddVariable( "leadEmPhi"       , 'F' ); allUsedVars += ":leadEmPhi"        ;}
  if(iType > 0)   {factory->AddVariable( "leadChEta"       , 'F' ); allUsedVars += ":leadChEta"        ;}
  if(iType > 0)   {factory->AddVariable( "leadChPhi"       , 'F' ); allUsedVars += ":leadChPhi"        ;}
  if(iType > 0)   {factory->AddVariable( "leadFrac"        , 'F' ); allUsedVars += ":leadFrac"         ;}
  if(iType > 0)   {factory->AddVariable( "jetM"            , 'F' ); allUsedVars += ":jetM"             ;}

  //lead+2nd+3rd+4th Info
  if(iOrderType > 0) {factory->AddVariable( "leadFrac"      , 'F' ); allUsedVars += ":leadFrac"        ;}
  if(iOrderType > 0) {factory->AddVariable( "leadChFrac"    , 'F' ); allUsedVars += ":leadChFrac"      ;}
  if(iOrderType > 0) {factory->AddVariable( "leadEmFrac"    , 'F' ); allUsedVars += ":leadEmFrac"      ;}
  if(iOrderType > 0) {factory->AddVariable( "leadNeutFrac"  , 'F' ); allUsedVars += ":leadNeutFrac"    ;}
  if(iOrderType > 1) {factory->AddVariable( "secondFrac"    , 'F' ); allUsedVars += ":secondFrac"      ;}
  if(iOrderType > 1) {factory->AddVariable( "secondChFrac"  , 'F' ); allUsedVars += ":secondChFrac"    ;}
  if(iOrderType > 1) {factory->AddVariable( "secondEmFrac"  , 'F' ); allUsedVars += ":secondEmFrac"    ;}
  if(iOrderType > 1) {factory->AddVariable( "secondNeutFrac", 'F' ); allUsedVars += ":secondNeutFrac"  ;}
  if(iOrderType > 2) {factory->AddVariable( "thirdFrac"     , 'F' ); allUsedVars += ":thirdFrac"       ;}
  if(iOrderType > 2) {factory->AddVariable( "thirdChFrac"   , 'F' ); allUsedVars += ":thirdChFrac"     ;}
  if(iOrderType > 2) {factory->AddVariable( "thirdEmFrac"   , 'F' ); allUsedVars += ":thirdEmFrac"     ;}
  if(iOrderType > 2) {factory->AddVariable( "thirdNeutFrac" , 'F' ); allUsedVars += ":thirdNeutFrac"   ;}
  if(iOrderType > 3) {factory->AddVariable( "fourthFrac"    , 'F' ); allUsedVars += ":fourthFrac"      ;}
  if(iOrderType > 3) {factory->AddVariable( "fourthChFrac"  , 'F' ); allUsedVars += ":fourthChFrac"    ;}
  if(iOrderType > 3) {factory->AddVariable( "fourthEmFrac"  , 'F' ); allUsedVars += ":fourthEmFrac"    ;}
  if(iOrderType > 3) {factory->AddVariable( "fourthNeutFrac", 'F' ); allUsedVars += ":fourthNeutFrac"  ;}
  
  //ptD ==> have not tested this
  if(iPtDType   > 0)  {factory->AddVariable( "ptD"            , 'F' ); allUsedVars += ":ptD"           ;}
  
  //Fraction Variables ===> have not tested this
  if(iFracType  > 0)  {factory->AddVariable( "chgEMfrac"      , 'F' ); allUsedVars += ":chgEMfrac"     ;}
  if(iFracType  > 0)  {factory->AddVariable( "chgHadrfrac"    , 'F' ); allUsedVars += ":chgHadrfrac"   ;}
  if(iFracType  > 0)  {factory->AddVariable( "neuEMfrac"      , 'F' ); allUsedVars += ":neuEMfrac"     ;}
  if(iFracType  > 0)  {factory->AddVariable( "neuHadrfrac"    , 'F' ); allUsedVars += ":neuHadrfrac"   ;}
  
  //Moments ===> have not tested
  if(iMomType   > 0)  {factory->AddVariable( "majW"           , 'F' ); allUsedVars += ":majW"      ;}
  if(iMomType   > 0)  {factory->AddVariable( "minW"           , 'F' ); allUsedVars += ":minW"      ;}
  if(iMomType   > 1)  {factory->AddVariable( "phiW"           , 'F' ); allUsedVars += ":phiW"      ;}
  if(iMomType   > 1)  {factory->AddVariable( "etaW"           , 'F' ); allUsedVars += ":etaW"      ;}
  if(iMomType   > 2)  {factory->AddVariable( "jetW"           , 'F' ); allUsedVars += ":jetW"      ;}//
  
  //Detailed Delta R info ===> have not tested this (waiting for PM)
  if(iRingType  > 0) {factory->AddVariable( "chFrac01"        , 'F' ); allUsedVars += ":chFrac01"     ;}
  if(iRingType  > 0) {factory->AddVariable( "emFrac01"        , 'F' ); allUsedVars += ":emFrac01"     ;}
  if(iRingType  > 0) {factory->AddVariable( "neutFrac01"      , 'F' ); allUsedVars += ":neutFrac01"   ;}
  if(iRingType  > 0) {factory->AddVariable( "frac01"          , 'F' ); allUsedVars += ":frac01"       ;}
  if(iRingType  > 1) {factory->AddVariable( "chFrac02"        , 'F' ); allUsedVars += ":chFrac02"     ;}
  if(iRingType  > 1) {factory->AddVariable( "emFrac02"        , 'F' ); allUsedVars += ":emFrac02"     ;}
  if(iRingType  > 1) {factory->AddVariable( "neutFrac02"      , 'F' ); allUsedVars += ":neutFrac02"   ;}
  if(iRingType  > 1) {factory->AddVariable( "frac02"          , 'F' ); allUsedVars += ":frac02"       ;}
  if(iRingType  > 2) {factory->AddVariable( "chFrac03"        , 'F' ); allUsedVars += ":chFrac03"     ;}
  if(iRingType  > 2) {factory->AddVariable( "emFrac03"        , 'F' ); allUsedVars += ":emFrac03"     ;}
  if(iRingType  > 2) {factory->AddVariable( "neutFrac03"      , 'F' ); allUsedVars += ":neutFrac03"   ;}
  if(iRingType  > 2) {factory->AddVariable( "frac03"          , 'F' ); allUsedVars += ":frac03"       ;}
  if(iRingType  > 3) {factory->AddVariable( "chFrac04"        , 'F' ); allUsedVars += ":chFrac04"     ;}
  if(iRingType  > 3) {factory->AddVariable( "emFrac04"        , 'F' ); allUsedVars += ":emFrac04"     ;}
  if(iRingType  > 3) {factory->AddVariable( "neutFrac04"      , 'F' ); allUsedVars += ":neutFrac04"   ;}
  if(iRingType  > 3) {factory->AddVariable( "frac04"          , 'F' ); allUsedVars += ":frac04"       ;}
  if(iRingType  > 4) {factory->AddVariable( "chFrac05"        , 'F' ); allUsedVars += ":chFrac05"     ;}
  if(iRingType  > 4) {factory->AddVariable( "emFrac05"        , 'F' ); allUsedVars += ":emFrac05"     ;}
  if(iRingType  > 4) {factory->AddVariable( "neutFrac05"      , 'F' ); allUsedVars += ":neutFrac05"   ;}
  if(iRingType  > 4) {factory->AddVariable( "frac05"          , 'F' ); allUsedVars += ":frac05"       ;}
  
  TString lJetTree = "pfjetanalyzer/tree";
  if(iJetType == 1) lJetTree = "chspfjetanalyzer/tree";
  TString lSName       = "../scratch/JetAnalysisTree_merged.root";  TFile *lSInput = TFile::Open(lSName);
  TTree   *lSignal     = (TTree*)lSInput    ->Get(lJetTree);
  TString lBName       = "../scratch/JetAnalysisTree_merged.root";  TFile *lBInput = TFile::Open(lBName);
  TTree   *lBackground = (TTree*)lBInput    ->Get(lJetTree);
  
  Double_t lSWeight = 1.0;
  Double_t lBWeight = 1.0;
  
  factory->AddSignalTree    ( lSignal,     lSWeight );
  factory->AddBackgroundTree( lBackground, lBWeight );
  TCut lSCut = "jetPt  > 10  && abs(jetGenDr) < 0.25  && jetGenPt > 8.";
  TCut lBCut = "jetPt  > 10  && abs(jetGenDr) > 0.25  ";                  //Ignore amorphous area in between
  
  //factory->SetWeightExpression("RhoWeight/PtWeight"); Missing pT and npv reweighting
  factory->PrepareTrainingAndTestTree( lSCut, lBCut,
				       "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );
  
  // Categories
  TCut    lTKCut = "abs(jetEta) < 2.5";
  TCut    lHECut = "abs(jetEta) > 2.5 && abs(jetEta) < 2.9 ";
  TCut    lHFCut = "abs(jetEta) > 2.9 ";
  TMVA::MethodCategory* lCat  = 0; 
  TMVA::MethodBase*     lBase = factory->BookMethod( TMVA::Types::kCategory, ("JetID_"+iId).c_str(),"");
  lCat  = dynamic_cast<TMVA::MethodCategory*>(lBase);
  
  //MVA Type
  TString lBDTDef   = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.6:nCuts=1000:NNodesMax=5:SeparationType=GiniIndex:UseYesNoLeaf=F:";
  TString lFISDef   = "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10";
  TString lLIKDef   = "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50";
  TMVA::Types::EMVA  lType = TMVA::Types::kBDT;
  TString            lDef  = lBDTDef;
  if(iMVAType == 1) {lType = TMVA::Types::kFisher;        lDef = lFISDef;}
  if(iMVAType == 2) {lType = TMVA::Types::kLikelihood;    lDef = lLIKDef; }
  lCat->AddMethod(lTKCut,allUsedVars,lType,"TK",lDef);
  lCat->AddMethod(lHECut,allUsedVars,lType,"HE",lDef);
  lCat->AddMethod(lHFCut,allUsedVars,lType,"HF",lDef);
  
  //The rest
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
  outputFile->Close();
  
  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;
  delete factory;
}
