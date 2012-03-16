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

void apply(std::string iName="../Jets/f11-zjets_chs.root",std::string iId="",int iKinType=0,int iIPType=0,int iNParType=0,int iDRType=0,int iPtRType=0,int iType=0) { 
   // this loads the library
   TMVA::Tools::Instance();
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    
   int   lNPV    = 0; float lNPVP   = 0;
   float lJPt1   = 0; float lJEta1  = 0; float lJPhi1  = 0;  float lJM1 = 0;  float lJD01 = 0; float lJDZ1 = 0;   float lNPart1 = 0; 
   float lLPt1   = 0; float lLEta1  = 0; float lLPhi1  = 0; 
   float lSPt1   = 0; float lSEta1  = 0; float lSPhi1  = 0; 
   float lNEPt1  = 0; float lNEEta1 = 0; float lNEPhi1 = 0; 
   float lEMPt1  = 0; float lEMEta1 = 0; float lEMPhi1 = 0; 
   float lChPt1  = 0; float lChEta1 = 0; float lChPhi1 = 0;  float lLFr1   = 0;
   float lDRlC1  = 0; float lDRLS1  = 0; float lDRM1   = 0;  float lDRMNE1 = 0; float lDREM1 = 0; float lDRCH1 = 0; float lRho = 0;

   if(iKinType   == 0)    {reader->AddSpectator( "jspt_1"   , &lJEta1); }
   if(iKinType   == 0)    {reader->AddSpectator( "jseta_1"  , &lJPt1 ); }

   reader->AddVariable( "npv"      , &lNPVP ); 
   if(iKinType   > 0) {reader->AddVariable( "jspt_1"   , &lJPt1   ); }
   if(iKinType   > 1) {reader->AddVariable( "jseta_1"  , &lJEta1  ); }
   if(iKinType   > 2) {reader->AddVariable( "jsphi_1"  , &lJPhi1  ); }
   if(iIPType    > 0) {reader->AddVariable( "jdZ_1"    , &lJDZ1   ); }
   if(iIPType    > 1) {reader->AddVariable( "jd0_1"    , &lJD01   ); }
   if(iNParType  > 0) {reader->AddVariable( "npart_1"  , &lNPart1 ); }
   if(iDRType > 0)    {reader->AddVariable( "drlc_1"   , &lDRlC1  ); }
   if(iDRType > 0)    {reader->AddVariable( "drls_1"   , &lDRLS1  ); }
   if(iDRType > 0)    {reader->AddVariable( "drm_1"    , &lDRM1   ); }
   if(iDRType > 0)    {reader->AddVariable( "drmne_1"  , &lDRMNE1 ); }
   if(iDRType > 0)    {reader->AddVariable( "drem_1"   , &lDREM1  ); }
   if(iDRType > 0)    {reader->AddVariable( "drch_1"   , &lDRCH1  ); }
   if(iPtRType > 0 )  {reader->AddVariable( "lpt_1/jspt_1"    , &lLPt1  ); }
   if(iPtRType > 0 )  {reader->AddVariable( "spt_1/jspt_1"    , &lSPt1  ); }
   if(iPtRType > 0 )  {reader->AddVariable( "lnept_1/jspt_1"  , &lNEPt1 ); }
   if(iPtRType > 0 )  {reader->AddVariable( "lempt_1/jspt_1"  , &lEMPt1 ); }
   if(iPtRType > 0 )  {reader->AddVariable( "lchpt_1/jspt_1"  , &lChPt1 ); }

   if(iType > 0 ) {reader->AddVariable( "lpt_1"          , &lLPt1  ); }
   if(iType > 0) {reader->AddVariable( "leta_1"          , &lLEta1 ); }
   if(iType > 0) {reader->AddVariable( "lphi_1"          , &lLPhi1 ); }
   if(iType > 0) {reader->AddVariable( "spt_1       "    , &lSPt1  ); }
   if(iType > 0) {reader->AddVariable( "seta_1"          , &lSEta1 ); }
   if(iType > 0) {reader->AddVariable( "sphi_1"          , &lSPhi1 ); }
   if(iType > 0) {reader->AddVariable( "lnept_1"         , &lNEPt1 ); }
   if(iType > 0) {reader->AddVariable( "lneeta_1"        , &lNEEta1); }
   if(iType > 0) {reader->AddVariable( "lnephi_1"        , &lNEPhi1); }
   if(iType > 0) {reader->AddVariable( "lempt_1"         , &lEMPt1 ); }
   if(iType > 0) {reader->AddVariable( "lemeta_1"        , &lEMEta1); }
   if(iType > 0) {reader->AddVariable( "lemphi_1"        , &lEMPhi1); }
   if(iType > 0) {reader->AddVariable( "lchpt_1"         , &lChPt1 ); }
   if(iType > 0) {reader->AddVariable( "lchphi_1"        , &lChPhi1); }
   if(iType > 0) {reader->AddVariable( "lLfr_1"          , &lLFr1  ); }
   if(iType > 0) {reader->AddVariable( "jm_1"             ,&lJM1   ); }

   std::string lJetName   = "JetID";
   reader->BookMVA(lJetName .c_str(),(std::string("weights/TMVAClassificationCategory_JetID_")+iId+std::string(".weights.xml")).c_str());
   
   TFile *lFile = new TFile(("../Jets/"+iName).c_str());
   TTree *lTree = (TTree*) lFile->FindObjectAny("Flat");
   lTree->SetBranchAddress( "npv"      , &lNPV     );
   lTree->SetBranchAddress( "rho"      , &lRho     );
   lTree->SetBranchAddress( "jspt_1"   , &lJPt1    );
   lTree->SetBranchAddress( "jseta_1"  , &lJEta1   );
   lTree->SetBranchAddress( "jsphi_1"  , &lJPhi1   );     
   lTree->SetBranchAddress( "jd0_1"    , &lJD01   );
   lTree->SetBranchAddress( "jdZ_1"    , &lJDZ1   );
   lTree->SetBranchAddress( "jm_1"     , &lJM1    );
   lTree->SetBranchAddress( "npart_1"  , &lNPart1 );
   lTree->SetBranchAddress( "lpt_1"    , &lLPt1   );                                                                 
   lTree->SetBranchAddress( "leta_1"   , &lLEta1  );
   lTree->SetBranchAddress( "lphi_1"   , &lLPhi1  );                                            
   lTree->SetBranchAddress( "spt_1"    , &lSPt1   );                          
   lTree->SetBranchAddress( "seta_1"   , &lSEta1  );
   lTree->SetBranchAddress( "sphi_1"   , &lSPhi1  );                                           
   lTree->SetBranchAddress( "lnept_1"  , &lNEPt1  );                                                             
   lTree->SetBranchAddress( "lneeta_1" , &lNEEta1 );
   lTree->SetBranchAddress( "lnephi_1" , &lNEPhi1 );                                                       
   lTree->SetBranchAddress( "lempt_1"  , &lEMPt1  );                                                       
   lTree->SetBranchAddress( "lemeta_1" , &lEMEta1 );
   lTree->SetBranchAddress( "lemphi_1" , &lEMPhi1 );                                                                           
   lTree->SetBranchAddress( "lchpt_1"  , &lChPt1  );             
   lTree->SetBranchAddress( "lchphi_1" , &lChPhi1 );                                                                             
   lTree->SetBranchAddress( "lLfr_1"   , &lLFr1 );
   lTree->SetBranchAddress( "drlc_1"   , &lDRlC1 );
   lTree->SetBranchAddress( "drls_1"   , &lDRLS1 );
   lTree->SetBranchAddress( "drm_1"    , &lDRM1 );
   lTree->SetBranchAddress( "drmne_1"  , &lDRMNE1 );
   lTree->SetBranchAddress( "drem_1"   , &lDREM1 );
   lTree->SetBranchAddress( "drch_1"   , &lDRCH1 );
   
   int lNEvents = lTree->GetEntries();
   TFile *lOFile = new TFile("Output.root","RECREATE");
   TTree *lOTree = lTree->CloneTree(0);
   float lJet1MVA  = 0;   
   lOTree->Branch("jmva_1" ,&lJet1MVA ,"lJet1MVA/F");
   for (Long64_t i0=0; i0<lNEvents;i0++) {
     if (i0 % 10000 == 0) std::cout << "--- ... Processing event: " << double(i0)/double(lNEvents) << std::endl;
      lTree->GetEntry(i0);
      lNPVP = float(lNPV);
      lLPt1/=lJPt1;
      lSPt1/=lJPt1;
      lNEPt1/=lJPt1;
      lEMPt1/=lJPt1;
      lChPt1/=lJPt1;
      lJet1MVA  = float(reader->EvaluateMVA(lJetName.c_str()));
      lOTree->Fill();
   }  
   lOTree->Write();
   lOFile->Close();
   delete reader;
} 
