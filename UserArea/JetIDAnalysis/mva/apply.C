/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
//#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

void apply(std::string iName="../Jets/f11-zjets_chs.root",std::string iId="",
	   int iJetType=0,int iMVAType=0  ,int iPUType =0,int iKinType =0,int iIPType =0,int iNParType=0,int iDRType=0,
	   int iPtRType=0,int iOrderType=0,int iPtDType=0,int iFracType=0,int iMomType=0,int iRingType=0,int iType  =0) {
  // this loads the library
  TMVA::Tools::Instance();
  TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

  
  float jetPt  = 0; if(iKinType   == 0)    {reader->AddSpectator( "jetPt"   , &jetPt);}
  float jetEta = 0; if(iKinType   == 0)    {reader->AddSpectator( "jetEta"  , &jetEta);}
  float jetPhi = 0; if(iKinType   == 0)    {reader->AddSpectator( "jetPhi"  , &jetPhi);}
  
  //PU correlation => nvtx data/MC agree better
  //factory->AddVariable( "rho"      , 'F' );        allUsedVars += "rho"   ;
  float nvtx   = 0; if(iPUType   >  0)    {reader->AddVariable( "nvtx"      , &nvtx);       }
  
  //Jet pT+eta
  if(iKinType   > 0)    {reader->AddVariable( "jetPt"     , &jetPt)     ;}
  if(iKinType   > 1)    {reader->AddVariable( "jetEta"    , &jetEta)    ;}
  if(iKinType   > 2)    {reader->AddVariable( "jetPhi"    , &jetPhi)    ;}
  //Impact parameter 
  float dZ         = 0; if(iIPType    > 0)    {reader->AddVariable( "dZ"        , &dZ)        ;}
  float d0         = 0; if(iIPType    > 1)    {reader->AddVariable( "d0"        , &d0)        ;}
  float beta       = 0; if(iIPType    > 2)    {reader->AddVariable( "beta"      , &beta)      ;}
  float betaStar   = 0; if(iIPType    > 3)    {reader->AddVariable( "betaStar"  , &betaStar)  ;}
  //Number of particles
  float nParticles = 0; if(iNParType  > 0)    {reader->AddVariable( "nParticles", &nParticles);}
  float nCharged   = 0; if(iNParType  > 1)    {reader->AddVariable( "nCharged"  , &nCharged  );} //Not Yet tried
  float nNeutrals  = 0; if(iNParType  > 1)    {reader->AddVariable( "nNeutrals" , &nNeutrals );} //Not Yet tried
  //Moments
  float dRLeadCent = 0; if(iDRType    > 0)    {reader->AddVariable( "dRLeadCent", &dRLeadCent);}
  float dRLead2nd  = 0; if(iDRType    > 0)    {reader->AddVariable( "dRLead2nd" , &dRLead2nd) ;}
  float dRMean     = 0; if(iDRType    > 0)    {reader->AddVariable( "dRMean"    , &dRMean)    ;}
  float dRMeanNeut = 0; if(iDRType    > 0)    {reader->AddVariable( "dRMeanNeut", &dRMeanNeut);}
  float dRMeanEm   = 0; if(iDRType    > 0)    {reader->AddVariable( "dRMeanEm"  , &dRMeanEm)  ;}
  float dRMeanCh   = 0; if(iDRType    > 0)    {reader->AddVariable( "dRMeanCh"  , &dRMeanCh)  ;}
  //Pt Ratios
  float leadPt     = 0; if(iPtRType   > 0 ) {reader->AddVariable( "leadPt/jetPt"    , &leadPt)           ;}
  float secondPt   = 0; if(iPtRType   > 0 ) {reader->AddVariable( "secondPt/jetPt"  , &secondPt)         ;}
  float leadNeutPt = 0; if(iPtRType   > 0 ) {reader->AddVariable( "leadNeutPt/jetPt", &leadNeutPt)       ;}
  float leadEmPt   = 0; if(iPtRType   > 0 ) {reader->AddVariable( "leadEmPt/jetPt"  , &leadEmPt)         ;}
  float leadChPt   = 0; if(iPtRType   > 0 ) {reader->AddVariable( "leadChPt/jetPt"  , &leadChPt)         ;}
  
  //Old Variables from teh original training 
                         if(iType > 0 )  {reader->AddVariable( "leadPt"          , &leadPt)           ;}
  float leadEta     = 0; if(iType > 0)   {reader->AddVariable( "leadEta"         , &leadEta)          ;}
  float leadPhi     = 0; if(iType > 0)   {reader->AddVariable( "leadPhi"         , &leadPhi)          ;}
                         if(iType > 0)   {reader->AddVariable( "secondPt"        , &secondPt)         ;}
  float secondEta   = 0; if(iType > 0)   {reader->AddVariable( "secondEta"       , &secondEta)        ;}
  float secondPhi   = 0; if(iType > 0)   {reader->AddVariable( "secondPhi"       , &secondPhi)        ;}
                         if(iType > 0)   {reader->AddVariable( "leadNeutPt"      , &leadNeutPt)       ;}
  float leadNeutEta = 0; if(iType > 0)   {reader->AddVariable( "leadNeutEta"     , &leadNeutEta)      ;}
  float leadNeutPhi = 0; if(iType > 0)   {reader->AddVariable( "leadNeutPhi"     , &leadNeutPhi)      ;}
                         if(iType > 0)   {reader->AddVariable( "leadEmPt"        , &leadEmPt)         ;}
  float leadEmEta   = 0; if(iType > 0)   {reader->AddVariable( "leadEmEta"       , &leadEmEta)        ;}
  float leadEmPhi   = 0; if(iType > 0)   {reader->AddVariable( "leadEmPhi"       , &leadEmPhi)        ;}
  float leadChEta   = 0; if(iType > 0)   {reader->AddVariable( "leadChEta"       , &leadChEta)        ;}
  float leadChPhi   = 0; if(iType > 0)   {reader->AddVariable( "leadChPhi"       , &leadChPhi)        ;}
  float leadFrac    = 0; if(iType > 0)   {reader->AddVariable( "leadFrac"        , &leadFrac)         ;}
  float jetM        = 0; if(iType > 0)   {reader->AddVariable( "jetM"            , &jetM)             ;}

  //lead+2nd+3rd+4th Info
                            if(iOrderType > 0) {reader->AddVariable( "leadFrac"      , &leadFrac)        ;}
  float leadChFrac     = 0; if(iOrderType > 0) {reader->AddVariable( "leadChFrac"    , &leadChFrac)      ;}
  float leadEmFrac     = 0; if(iOrderType > 0) {reader->AddVariable( "leadEmFrac"    , &leadEmFrac)      ;}
  float leadNeutFrac   = 0; if(iOrderType > 0) {reader->AddVariable( "leadNeutFrac"  , &leadNeutFrac)    ;}
  float secondFrac     = 0; if(iOrderType > 1) {reader->AddVariable( "secondFrac"    , &secondFrac)      ;}
  float secondChFrac   = 0; if(iOrderType > 1) {reader->AddVariable( "secondChFrac"  , &secondChFrac)    ;}
  float secondEmFrac   = 0; if(iOrderType > 1) {reader->AddVariable( "secondEmFrac"  , &secondEmFrac)    ;}
  float secondNeutFrac = 0; if(iOrderType > 1) {reader->AddVariable( "secondNeutFrac", &secondNeutFrac)  ;}
  float thirdFrac      = 0; if(iOrderType > 2) {reader->AddVariable( "thirdFrac"     , &thirdFrac)       ;}
  float thirdChFrac    = 0; if(iOrderType > 2) {reader->AddVariable( "thirdChFrac"   , &thirdChFrac)     ;}
  float thirdEmFrac    = 0; if(iOrderType > 2) {reader->AddVariable( "thirdEmFrac"   , &thirdEmFrac)     ;}
  float thirdNeutFrac  = 0; if(iOrderType > 2) {reader->AddVariable( "thirdNeutFrac" , &thirdNeutFrac)   ;}
  float fourthFrac     = 0; if(iOrderType > 3) {reader->AddVariable( "fourthFrac"    , &fourthFrac)      ;}
  float fourthChFrac   = 0; if(iOrderType > 3) {reader->AddVariable( "fourthChFrac"  , &fourthChFrac)    ;}
  float fourthEmFrac   = 0; if(iOrderType > 3) {reader->AddVariable( "fourthEmFrac"  , &fourthEmFrac)    ;}
  float fourthNeutFrac = 0; if(iOrderType > 3) {reader->AddVariable( "fourthNeutFrac", &fourthNeutFrac)  ;}
  
  //ptD ==> have not tested this
  float ptD            = 0; if(iPtDType   > 0)  {reader->AddVariable( "ptD"            , &ptD)           ;}
  
  //Fraction Variables ===> have not tested this
  float chgEMfrac      = 0; if(iFracType  > 0)  {reader->AddVariable( "chgEMfrac"      , &chgEMfrac)     ;}
  float chgHadrfrac    = 0; if(iFracType  > 0)  {reader->AddVariable( "chgHadrfrac"    , &chgHadrfrac)   ;}
  float neuEMfrac      = 0; if(iFracType  > 0)  {reader->AddVariable( "neuEMfrac"      , &neuEMfrac)     ;}
  float neuHadrfrac    = 0; if(iFracType  > 0)  {reader->AddVariable( "neuHadrfrac"    , &neuHadrfrac)   ;}
  
  //Moments ===> have not tested
  float majW           = 0; if(iMomType   > 0)  {reader->AddVariable( "majW"           , &majW)      ;}
  float minW           = 0; if(iMomType   > 0)  {reader->AddVariable( "minW"           , &minW)      ;}
  float phiW           = 0; if(iMomType   > 1)  {reader->AddVariable( "phiW"           , &phiW)      ;}
  float etaW           = 0; if(iMomType   > 1)  {reader->AddVariable( "etaW"           , &etaW)      ;}
  float jetW           = 0; if(iMomType   > 2)  {reader->AddVariable( "jetW"           , &jetW)      ;}//
  
  //Detailed Delta R info ===> have not tested this (waiting for PM)
  float chFrac01       = 0; if(iRingType  > 0) {reader->AddVariable( "chFrac01"        , &chFrac01)     ;}
  float emFrac01       = 0; if(iRingType  > 0) {reader->AddVariable( "emFrac01"        , &emFrac01)     ;}
  float neutFrac01     = 0; if(iRingType  > 0) {reader->AddVariable( "neutFrac01"      , &neutFrac01)   ;}
  float frac01         = 0; if(iRingType  > 0) {reader->AddVariable( "frac01"          , &frac01)       ;}
  float chFrac02       = 0; if(iRingType  > 1) {reader->AddVariable( "chFrac02"        , &chFrac02)     ;}
  float emFrac02       = 0; if(iRingType  > 1) {reader->AddVariable( "emFrac02"        , &emFrac02)     ;}
  float neutFrac02     = 0; if(iRingType  > 1) {reader->AddVariable( "neutFrac02"      , &neutFrac02)   ;}
  float frac02         = 0; if(iRingType  > 1) {reader->AddVariable( "frac02"          , &frac02)       ;}
  float chFrac03       = 0; if(iRingType  > 2) {reader->AddVariable( "chFrac03"        , &chFrac03)     ;}
  float emFrac03       = 0; if(iRingType  > 2) {reader->AddVariable( "emFrac03"        , &emFrac03)     ;}
  float neutFrac03     = 0; if(iRingType  > 2) {reader->AddVariable( "neutFrac03"      , &neutFrac03)   ;}
  float frac03         = 0; if(iRingType  > 2) {reader->AddVariable( "frac03"          , &frac03)       ;}
  float chFrac04       = 0; if(iRingType  > 3) {reader->AddVariable( "chFrac04"        , &chFrac04)     ;}
  float emFrac04       = 0; if(iRingType  > 3) {reader->AddVariable( "emFrac04"        , &emFrac04)     ;}
  float neutFrac04     = 0; if(iRingType  > 3) {reader->AddVariable( "neutFrac04"      , &neutFrac04)   ;}
  float frac04         = 0; if(iRingType  > 3) {reader->AddVariable( "frac04"          , &frac04)       ;}
  float chFrac05       = 0; if(iRingType  > 4) {reader->AddVariable( "chFrac05"        , &chFrac05)     ;}
  float emFrac05       = 0; if(iRingType  > 4) {reader->AddVariable( "emFrac05"        , &emFrac05)     ;}
  float neutFrac05     = 0; if(iRingType  > 4) {reader->AddVariable( "neutFrac05"      , &neutFrac05)   ;}
  float frac05         = 0; if(iRingType  > 4) {reader->AddVariable( "frac05"          , &frac05)       ;}
  
   std::string lJetName   = "JetID";
   reader->BookMVA(lJetName .c_str(),(std::string("weights/TMVAClassificationCategory_JetID_")+iId+std::string(".weights.xml")).c_str());

   TString lJetTree = "pfjetanalyzer/tree";
   if(iJetType == 1) lJetTree = "chspfjetanalyzer/tree";      
   lJetTree = "tree";
   TFile *lFile = new TFile(iName.c_str());
   TTree *lTree = (TTree*) lFile->Get(lJetTree);
   int id = 0;
   lTree->SetBranchAddress( "id"      , &id);
   //Kinematics
   lTree->SetBranchAddress( "jetPt"   , &jetPt);
   lTree->SetBranchAddress( "jetEta"  , &jetEta);
   lTree->SetBranchAddress( "jetPhi"  , &jetPhi);

   //Impact parameter 
   lTree->SetBranchAddress( "dZ"        , &dZ)        ;
   lTree->SetBranchAddress( "d0"        , &d0)        ;
   lTree->SetBranchAddress( "beta"      , &beta)      ;
   lTree->SetBranchAddress( "betaStar"  , &betaStar)  ;
   //Number of particles
   lTree->SetBranchAddress( "nParticles", &nParticles);
   lTree->SetBranchAddress( "nCharged"  , &nCharged  ); //Not Yet tried
   lTree->SetBranchAddress( "nNeutrals" , &nNeutrals  ); //Not Yet tried
   //Moments
   lTree->SetBranchAddress( "dRLeadCent", &dRLeadCent);
   lTree->SetBranchAddress( "dRLead2nd" , &dRLead2nd) ;
   lTree->SetBranchAddress( "dRMean"    , &dRMean)    ;
   lTree->SetBranchAddress( "dRMeanNeut", &dRMeanNeut);
   lTree->SetBranchAddress( "dRMeanEm"  , &dRMeanEm)  ;
   lTree->SetBranchAddress( "dRMeanCh"  , &dRMeanCh)  ;
   //Pt Ratios
   lTree->SetBranchAddress( "leadPt"    , &leadPt)    ;
   lTree->SetBranchAddress( "secondPt"  , &secondPt)  ;
   lTree->SetBranchAddress( "leadNeutPt", &leadNeutPt);
   lTree->SetBranchAddress( "leadEmPt"  , &leadEmPt)  ;
   lTree->SetBranchAddress( "leadChPt"  , &leadChPt)  ;
  
   //Old Variables from teh original trainin
   lTree->SetBranchAddress( "leadPt"          , &leadPt)           ;          
   lTree->SetBranchAddress( "leadEta"         , &leadEta)          ;
   lTree->SetBranchAddress( "leadPhi"         , &leadPhi)          ;
   lTree->SetBranchAddress( "secondPt"        , &secondPt)         ;
   lTree->SetBranchAddress( "secondEta"       , &secondEta)        ;
   lTree->SetBranchAddress( "secondPhi"       , &secondPhi)        ;
   lTree->SetBranchAddress( "leadNeutPt"      , &leadNeutPt)       ;
   lTree->SetBranchAddress( "leadNeutEta"     , &leadNeutEta)      ;
   lTree->SetBranchAddress( "leadNeutPhi"     , &leadNeutPhi)      ;
   lTree->SetBranchAddress( "leadEmPt"        , &leadEmPt)         ;
   lTree->SetBranchAddress( "leadEmEta"       , &leadEmEta)        ;
   lTree->SetBranchAddress( "leadEmPhi"       , &leadEmPhi)        ;
   lTree->SetBranchAddress( "leadChEta"       , &leadChEta)        ;
   lTree->SetBranchAddress( "leadChPhi"       , &leadChPhi)        ;
   lTree->SetBranchAddress( "leadFrac"        , &leadFrac)         ;
   lTree->SetBranchAddress( "jetM"            , &jetM)             ;
   
  //lead+2nd+3rd+4th Info
   lTree->SetBranchAddress( "leadFrac"      , &leadFrac)        ;
   lTree->SetBranchAddress( "leadChFrac"    , &leadChFrac)      ;
   lTree->SetBranchAddress( "leadEmFrac"    , &leadEmFrac)      ;
   lTree->SetBranchAddress( "leadNeutFrac"  , &leadNeutFrac)    ;
   lTree->SetBranchAddress( "secondFrac"    , &secondFrac)      ;
   lTree->SetBranchAddress( "secondChFrac"  , &secondChFrac)    ;
   lTree->SetBranchAddress( "secondEmFrac"  , &secondEmFrac)    ;
   lTree->SetBranchAddress( "secondNeutFrac", &secondNeutFrac)  ;
   lTree->SetBranchAddress( "thirdFrac"     , &thirdFrac)       ;
   lTree->SetBranchAddress( "thirdChFrac"   , &thirdChFrac)     ;
   lTree->SetBranchAddress( "thirdEmFrac"   , &thirdEmFrac)     ;
   lTree->SetBranchAddress( "thirdNeutFrac" , &thirdNeutFrac)   ;
   lTree->SetBranchAddress( "fourthFrac"    , &fourthFrac)      ;
   lTree->SetBranchAddress( "fourthChFrac"  , &fourthChFrac)    ;
   lTree->SetBranchAddress( "fourthEmFrac"  , &fourthEmFrac)    ;
   lTree->SetBranchAddress( "fourthNeutFrac", &fourthNeutFrac)  ;
   
   //ptD ==> have not tested this
   lTree->SetBranchAddress( "ptD"            , &ptD)           ;
   
   //Fraction Variables ===> have not tested this
   lTree->SetBranchAddress( "chgEMfrac"      , &chgEMfrac)     ;
   lTree->SetBranchAddress( "chgHadrfrac"    , &chgHadrfrac)   ;
   lTree->SetBranchAddress( "neuEMfrac"      , &neuEMfrac)     ;
   lTree->SetBranchAddress( "neuHadrfrac"    , &neuHadrfrac)   ;
   
   //Moments ===> have not tested
   lTree->SetBranchAddress( "majW"           , &majW)      ;
   lTree->SetBranchAddress( "minW"           , &minW)      ;
   lTree->SetBranchAddress( "phiW"           , &phiW)      ;
   lTree->SetBranchAddress( "etaW"           , &etaW)      ;
   lTree->SetBranchAddress( "jetW"           , &jetW)      ;//
   
   //Detailed Delta R info ===> have not tested this (waiting for PM)
   lTree->SetBranchAddress( "chFrac01"        , &chFrac01)     ;
   lTree->SetBranchAddress( "emFrac01"        , &emFrac01)     ;
   lTree->SetBranchAddress( "neutFrac01"      , &neutFrac01)   ;
   lTree->SetBranchAddress( "frac01"          , &frac01)       ;
   lTree->SetBranchAddress( "chFrac02"        , &chFrac02)     ;
   lTree->SetBranchAddress( "emFrac02"        , &emFrac02)     ;
   lTree->SetBranchAddress( "neutFrac02"      , &neutFrac02)   ;
   lTree->SetBranchAddress( "frac02"          , &frac02)       ;
   lTree->SetBranchAddress( "chFrac03"        , &chFrac03)     ;
   lTree->SetBranchAddress( "emFrac03"        , &emFrac03)     ;
   lTree->SetBranchAddress( "neutFrac03"      , &neutFrac03)   ;
   lTree->SetBranchAddress( "frac03"          , &frac03)       ;
   lTree->SetBranchAddress( "chFrac04"        , &chFrac04)     ;
   lTree->SetBranchAddress( "emFrac04"        , &emFrac04)     ;
   lTree->SetBranchAddress( "neutFrac04"      , &neutFrac04)   ;
   lTree->SetBranchAddress( "frac04"          , &frac04)       ;
   lTree->SetBranchAddress( "chFrac05"        , &chFrac05)     ;
   lTree->SetBranchAddress( "emFrac05"        , &emFrac05)     ;
   lTree->SetBranchAddress( "neutFrac05"      , &neutFrac05)   ;
   lTree->SetBranchAddress( "frac05"          , &frac05)       ;
   
  
   int lNEvents = lTree->GetEntries();
   TFile *lOFile = new TFile("Output.root","RECREATE");
   TTree *lOTree = lTree->CloneTree(0);
   float lJet1MVA  = 0;   
   lOTree->Branch("jmva" ,&lJet1MVA ,"lJet1MVA/F");
   for (Long64_t i0=0; i0<lNEvents;i0++) {
     if (i0 % 10000 == 0) std::cout << "--- ... Processing event: " << double(i0)/double(lNEvents) << std::endl;
      lTree->GetEntry(i0);
      if(id % 2 == 1) continue;
      leadPt     /=jetPt;
      secondPt   /=jetPt;
      leadNeutPt /=jetPt;
      leadEmPt   /=jetPt;
      leadChPt   /=jetPt;
      lJet1MVA  = float(reader->EvaluateMVA(lJetName.c_str()));
      lOTree->Fill();
   }  
   lOTree->Write();
   lOFile->Close();
   delete reader;
   iMVAType = 0; 
} 
