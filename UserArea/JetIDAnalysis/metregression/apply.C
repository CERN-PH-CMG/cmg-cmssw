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
#include "TLorentzVector.h"
#include "TROOT.h"

#include "TMVA/TMVARegGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif
void apply(std::string iName="../Jets/f11-dimu_pt10_vMETPU_vPUMET_C2_v11.root",bool i2nd=false) {
  TMVA::Tools::Instance();
   std::cout << "Classifying" << std::endl;
   float lNPV     = 0; int   lId     = 0;
   float lSumEt   = 0; float lU1     = 0; float lU2      = 0; 
   float lTkSumEt = 0; float lTKU1   = 0; float lTKU2    = 0; 
   float lNPSumEt = 0; float lNPU1   = 0; float lNPU2    = 0; float lNPEta   = 0; 
   float lPUSumEt = 0; float lPUEta   = 0; 
   float lPCSumEt = 0; float lPCU1   = 0; float lPCU2    = 0; float lPCEta   = 0; int lNPVP  = 0;
   float lJSPt1   = 0; float lJSEta1 = 0; float lJSPhi1  = 0; float lNJet    = 0; int lNJetP = 0;
   float lJSPt2   = 0; float lJSEta2 = 0; float lJSPhi2  = 0; float lUPhiMVA = 0; 
   float lNAllJet = 0; int lNAllJetP  = 0;
   float lPUMet   = 0; float lPUMetPhi = 0;
   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    
   reader->AddVariable( "npv"              , &lNPV     );
   reader->AddVariable( "u"                , &lU1      ); 
   reader->AddVariable( "uphi"             , &lU2      );
   reader->AddVariable( "chsumet/sumet"    , &lTkSumEt ); 
   reader->AddVariable( "tku"              , &lTKU1    );
   reader->AddVariable( "tkuphi"           , &lTKU2    );
   reader->AddVariable( "nopusumet/sumet"  , &lNPSumEt );
   reader->AddVariable( "nopuu"            , &lNPU1    );
   reader->AddVariable( "nopuuphi"         , &lNPU2    );
   reader->AddVariable( "pusumet/sumet"    , &lPUSumEt );
   reader->AddVariable( "pumet"            , &lPUMet   );
   reader->AddVariable( "pumetphi"         , &lPUMetPhi);
   reader->AddVariable( "pucsumet/sumet"   , &lPCSumEt );
   reader->AddVariable( "pucu"             , &lPCU1    );
   reader->AddVariable( "pucuphi"          , &lPCU2    );
   reader->AddVariable( "jspt_1"     , &lJSPt1   );
   reader->AddVariable( "jseta_1"    , &lJSEta1  );
   reader->AddVariable( "jsphi_1"    , &lJSPhi1  );
   reader->AddVariable( "jspt_2"     , &lJSPt2   );
   reader->AddVariable( "jseta_2"    , &lJSEta2  );
   reader->AddVariable( "jsphi_2"    , &lJSPhi2  );
   reader->AddVariable( "nalljet"    , &lNAllJet );
   reader->AddVariable( "njet"       , &lNJet    );
   if(i2nd) reader->AddVariable( "uphi_mva"   , &lUPhiMVA );

   std::string lRecName      = "RecoilRegress";
   std::string lRecPhiName   = "RecoilPhiRegress";
   if(!i2nd)  reader->BookMVA(lRecPhiName .c_str(),"weights/mva_RecoilPhiRegress_baseline.weights.xml");
   if(i2nd)   reader->BookMVA(lRecName    .c_str(),"weights/mva_RecoilRegress_baseline.weights.xml");

   TFile *lFile = new TFile(iName.c_str());
   TTree *lTree = (TTree*) lFile->FindObjectAny("Flat");
   lTree->SetBranchAddress( "id"         , &lId       ); 
   lTree->SetBranchAddress( "npv"        , &lNPVP     ); 
   lTree->SetBranchAddress( "sumet"      , &lSumEt    ); 
   lTree->SetBranchAddress( "u"          , &lU1       ); 
   lTree->SetBranchAddress( "uphi"       , &lU2       );
   lTree->SetBranchAddress( "chsumet"    , &lTkSumEt  ); 
   lTree->SetBranchAddress( "tku"        , &lTKU1     );
   lTree->SetBranchAddress( "tkuphi"     , &lTKU2     );
   lTree->SetBranchAddress( "nopusumet"  , &lNPSumEt  );
   lTree->SetBranchAddress( "nopumeteta" , &lNPEta    );
   lTree->SetBranchAddress( "nopuu"      , &lNPU1     );
   lTree->SetBranchAddress( "nopuuphi"   , &lNPU2     );
   lTree->SetBranchAddress( "pusumet"    , &lPUSumEt  );
   lTree->SetBranchAddress( "pumet"      , &lPUMet    );
   lTree->SetBranchAddress( "pumetphi"   , &lPUMetPhi  );
   lTree->SetBranchAddress( "pumeteta"   , &lPUEta    );
   lTree->SetBranchAddress( "pucsumet"   , &lPCSumEt  );
   lTree->SetBranchAddress( "pucmeteta"  , &lPCEta    );
   lTree->SetBranchAddress( "pucu"       , &lPCU1     );
   lTree->SetBranchAddress( "pucuphi"    , &lPCU2     );
   lTree->SetBranchAddress( "jspt_1"     , &lJSPt1    );
   lTree->SetBranchAddress( "jseta_1"    , &lJSEta1   );
   lTree->SetBranchAddress( "jsphi_1"    , &lJSPhi1   );
   lTree->SetBranchAddress( "jspt_2"     , &lJSPt2    );
   lTree->SetBranchAddress( "jseta_2"    , &lJSEta2   );
   lTree->SetBranchAddress( "jsphi_2"    , &lJSPhi2   );
   lTree->SetBranchAddress( "njet"       , &lNJetP    );
   lTree->SetBranchAddress( "nalljet"    , &lNAllJetP );
   if(i2nd) lTree->SetBranchAddress( "uphi_mva"   , &lUPhiMVA  );

   float lPtVis   = 0; 
   float lPhiVis  = 0;
   float lPtGen   = 0;
   float lPhiGen  = 0;
   lTree->SetBranchAddress("pt_vis"     ,&lPtVis);
   lTree->SetBranchAddress("phi_vis"    ,&lPhiVis);
   lTree->SetBranchAddress("pt_z"      ,&lPtGen);
   lTree->SetBranchAddress("phi_z"     ,&lPhiGen);
   
   float lPt1 = 0; float lPhi1 = 0; float lEta1 = 0; float lM1 = 0; 
   float lPt2 = 0; float lPhi2 = 0; float lEta2 = 0; float lM2 = 0; 
   float lU1R = 0; float lU2R  = 0;
   lTree->SetBranchAddress("pt_1"       ,&lPt1);
   lTree->SetBranchAddress("eta_1"      ,&lEta1);
   lTree->SetBranchAddress("phi_1"      ,&lPhi1);
   lTree->SetBranchAddress("m_1"        ,&lM1);
   lTree->SetBranchAddress("pt_2"       ,&lPt2);
   lTree->SetBranchAddress("eta_2"      ,&lEta2);
   lTree->SetBranchAddress("phi_2"      ,&lPhi2);
   lTree->SetBranchAddress("m_2"        ,&lM2);
   if(i2nd) lTree->SetBranchAddress("u1_mva"     ,&lU1R);
   if(i2nd) lTree->SetBranchAddress("nopuu2"     ,&lU2R);
 

   int lNEvents = lTree->GetEntries();
   TFile *lOFile = new TFile("Output.root","RECREATE");
   TTree *lOTree = lTree->CloneTree(0);
   float lUMVA        = 0;   
   float lUMVAPhi     = 0;   
   float lU1MVA       = 0;   
   float lU2MVA       = 0;   
   float lMetMVA      = 0;
   float lMetPhiMVA   = 0;
   float lMMVA        = 0;
   float lGenMet      = 0;
   float lGenPhi      = 0;
   float lU1NPP       = 0;
   float lU2NPP       = 0;
   float lU1P         = 0;
   float lU2P         = 0;

   if(i2nd) lOTree->Branch("ux_mva"      ,&lUMVA       ,"lUMVA/F");
   if(i2nd) lOTree->Branch("uphix_mva"   ,&lUMVAPhi    ,"lUMVAPhi/F");
   if(i2nd) lOTree->Branch("metx_mva"    ,&lMetMVA     ,"lMetMVA/F");
   if(i2nd) lOTree->Branch("metphix_mva" ,&lMetPhiMVA  ,"lMetPhiMVA/F");
   if(i2nd) lOTree->Branch("genmetx"     ,&lGenMet     ,"lGenMet/F");
   if(i2nd) lOTree->Branch("genmetphix"  ,&lGenPhi     ,"lGenPhi/F");
   if(i2nd) lOTree->Branch("u1x_mva"     ,&lU1MVA      ,"lU1MVA/F");
   if(i2nd) lOTree->Branch("u2x_mva"     ,&lU2MVA      ,"lU2MVA/F");

   if(!i2nd) lOTree->Branch("u_mva"      ,&lUMVA       ,"lUMVA/F");
   if(!i2nd) lOTree->Branch("uphi_mva"   ,&lUMVAPhi    ,"lUMVAPhi/F");
   if(!i2nd) lOTree->Branch("met_mva"    ,&lMetMVA     ,"lMetMVA/F");
   if(!i2nd) lOTree->Branch("metphi_mva" ,&lMetPhiMVA  ,"lMetPhiMVA/F");
   if(!i2nd) lOTree->Branch("genmet"     ,&lGenMet     ,"lGenMet/F");
   if(!i2nd) lOTree->Branch("genmetphi"  ,&lGenPhi     ,"lGenPhi/F");
   if(!i2nd) lOTree->Branch("u1_mva"     ,&lU1MVA      ,"lU1MVA/F");
   if(!i2nd) lOTree->Branch("u2_mva"     ,&lU2MVA      ,"lU2MVA/F");

   lOTree->Branch("m_mva"      ,&lMMVA       ,"lMMVA/F");
   lOTree->Branch("pu1"        ,&lU1P        ,"lU1P/F");
   lOTree->Branch("pu2"        ,&lU2P        ,"lU2P/F");
   lOTree->Branch("nopupu1"    ,&lU1NPP      ,"lU1NPP/F");
   lOTree->Branch("nopupu2"    ,&lU2NPP      ,"lU2NPP/F");
   for (Long64_t i0=0; i0<lNEvents;i0++) {
     if (i0 % 10000 == 0) std::cout << "--- ... Processing event: " << double(i0)/double(lNEvents) << std::endl;
     lTree->GetEntry(i0);
     if(i2nd && lId % 2 == 1) continue;
     lTkSumEt/=lSumEt;
     lPUSumEt/=lSumEt;
     lNPSumEt/=lSumEt;
     lPCSumEt/=lSumEt;
     lNPV     = float(lNPVP);
     lNJet    = float(lNJetP);
     lNAllJet = float(lNAllJetP);
     if(i2nd  ) lUMVA            = reader->EvaluateRegression(lRecName    .c_str())[0];
     if(!i2nd ) lUMVA            = 1.;

     if(i2nd  ) lUMVAPhi         = lUPhiMVA;
     if(!i2nd ) lUMVAPhi         = lU2     + reader->EvaluateRegression(lRecPhiName .c_str())[0];
     if(lUMVAPhi >  TMath::Pi()) lUMVAPhi-=2.*TMath::Pi();
     if(lUMVAPhi < -TMath::Pi()) lUMVAPhi+=2.*TMath::Pi();
    
     TLorentzVector lUVec(0,0,0,0);   lUVec .SetPtEtaPhiM(lU1*lUMVA,0,lUMVAPhi,0);
     TLorentzVector lUVec1(0,0,0,0);  lUVec1.SetPtEtaPhiM(lU1*lUMVA,0,lUMVAPhi,0);
     TLorentzVector lGVec1(0,0,0,0);  lGVec1.SetPtEtaPhiM(lPtGen,0,lPhiGen,0);
     TLorentzVector lGVec2(0,0,0,0);  lGVec2.SetPtEtaPhiM(lPtGen,0,lPhiGen,0);
     TLorentzVector lVVec(0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis,0,lPhiVis,0);
     if(lUMVA < 0) lUVec1.RotateZ(TMath::Pi());
     if(lUMVA < 0) lUVec .RotateZ(TMath::Pi());
     lUVec1     -= lVVec;
     lMetMVA     = lUVec1.Pt();
     lMetPhiMVA  = lUVec1.Phi();
     lGVec1     -= lVVec;
     lGenMet     = lGVec1.Pt();
     lGenPhi     = lGVec1.Phi();
     lUVec.RotateZ(-lPhiGen);
     //lUVec      -= lVVec;
     lU1MVA      = -lUVec.Px();
     lU2MVA      = -lUVec.Py();
     TLorentzVector lPUVec(0,0,0,0);   lPUVec .SetPtEtaPhiM(lU1,0,lU2,0);     //lPUVec.RotateZ(-lPhiVis);
     lPUVec      -= lVVec;
     lU1P        = lPUVec.Pt();
     lU2P        = -lPUVec.Py();
     TLorentzVector lNPUVec(0,0,0,0);   lNPUVec .SetPtEtaPhiM(lNPU1,0,lNPU2,0); //     lNPUVec.RotateZ(-lPhiVis);
     lNPUVec     -= lVVec;
     lU1NPP      = lNPUVec.Pt();
     lU2NPP      = -lNPUVec.Py();
     lOTree->Fill();
   }  
   lOTree->Write();
   lOFile->Close();
   delete reader;
}
