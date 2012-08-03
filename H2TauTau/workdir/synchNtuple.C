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


///////https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2012


//Preselection 
//
//Requirement Objects must pass lepton ids and default kinematic selection
//Trigger Selection : 
//mu  Tau     : look in twiki
//e   Tau     : HLT_Ele18_MediumIsoPFTau20
//e   Mu      : HLT_Mu8_Ele17_CaloIdT_CaloIsoVL || HLT_Mu17_Ele8_CaloIdT_CaloIsoVL 
//Tau Tau     : No Trigger
//
//Kinematic Selection (Take 2 highest pt leptons passing id+iso)
//mu  Tau     : 17 (muon)     |eta| < 2.1, 20 (hadronic Tau)  |eta| < 2.3
//e   Tau     : 20 (electron) |eta| < 2.1, 20 (hadronic Tau)  |eta| < 2.3
//e   Mu      : 20 (leading)  |eta| < 2.3, 10 (trailing)      |eta| < 2.1 
//Tau Tau     : 45            |eta| < 2.1, 45                 |eta| < 2.1
//
//Lepton Id 
//mu  Tau     : Muon: PF Muon Tight + |dZ| < 0.2 |d0| < 0.045 (these are wrt PV)    Tau:  decay mode + disc agains muon tight + discrimation agains electron loose
//e   Tau     : Electron : WP95(below)+ |dZ| < 0.2 |d0| < 0.045 (these are wrt PV)  Tau:  decay mode + anti-electron mva (+medium agains electrons) + disc against muon loose
//e   Mu      : Electron : Non triggring MVA(Twiki WP)                              Muon: PF Tight
//Tau Tau     : Decay mode + loose Muon + 1 loose electron + 1 MVA rejection
//
//Lepton Iso 
//mu  Tau     : Muon     : Delta Beta < 0.1               Tau  : mva id loose WP ( mva < 0.795)
//e   Tau     : electron : Delta Beta < 0.1               Tau  : mva id loose WP ( mva < 0.795)
//e   Mu      : Electron : Delta Beta < 0.15(EB)<0.1(EE)  Muon : Delta Beta < 0.15(EB)<0.1(EE)  
//Tau Tau     : tight MVA Isolation 
//
//2nd Lepton Veto
//mu Tau     : no 2 pf && (global || tracker) opposite charge muons with pT > 15 GeV and delta beta iso on each < 0.3 and delta R bewteen muons     > 0.15
//e  Tau     : no 2 WP95   opposite charge electrons                with pT > 15 GeV and delta beta iso on each < 0.3 and delta R bewteen electrons > 0.15
//e  Mu      : no Reco muon with 0.3 of the electron
//
//Jet Veto around leptons (please note this occurs AFTER selection)
//mu Tau     : no Jets around selected muon     with Delta R <  0.5 and selected Tau  with Delta R < 0.5
//e  Tau     : no Jets around selected elecron  with Delta R <  0.5 and selected Tau  with Delta R < 0.5
//e  Mu      : no Jets around selected electron with Delta R <  0.5 and selected Muon with Delta R < 0.5
//
//Jet id
//JEC        : https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections
//    Data   : GR_R_42_V23
//    MC     : MC_42_V17
//
//mu Tau     : All Jets passing PF loose id + MVA Id loose WP  + |eta| < 5.0 + pt > 30 GeV
//e  Tau     : All Jets passing PF loose id + MVA Id loose WP  + |eta| < 5.0 + pt > 30 GeV
//e  Mu      : All Jets passing PF loose id + MVA Id loose WP  + |eta| < 5.0 + pt > 30 GeV
//
//B Tag
//mu Tau     : Combined Secondary vertex Medium WP |eta| < 2.4 + pt > 20 GeV
//e  Tau     : Combined Secondary vertex Medium WP |eta| < 2.4 + pt > 20 GeV
//e  Mu      : Combined Secondary vertex Medium WP |eta| < 2.4 + pt > 20 GeV

//Footnotes
//WP95 Electron definition
//    (isEB && sigma_ieta_ieta < 0.01 && dphi < 0.80 && dEta < 0.007 && HoE<0.15)  ||  
//    (isEE && sigma_ieta_ieta )<0.03 && dphi < 0.70 && dEta < 0.010) 
//    && abs(d0wWrtPV)<0.045 && abs(dzWrtPV)<0.2 
//


void synchNtuple() {

   TTree lOTree("TauCheck","TauCheck");

   //Bookeeping
   int   lRun         = 0; lOTree.Branch("run"        ,&lRun           ,"lRun/I"     );//Run
   int   lLumi        = 0; lOTree.Branch("lumi"       ,&lLumi          ,"lLumi/I"    );//Lumi
   int   lEvt         = 0; lOTree.Branch("evt"        ,&lEvt           ,"lEvt/I"     );//Evt

   //Event Variables
   int   lNPV         = 0; lOTree.Branch("npv"        ,&lNPV           ,"lNPV/I"     );//NPV
   int   lNPU         = 0; lOTree.Branch("npu"        ,&lNPU           ,"lNPU/I"     );//NPU
   float lRho         = 0; lOTree.Branch("rho"        ,&lRho           ,"lRho/F"     );//Rho
   
   //Event Weights
   float lMCWeight    = 0; lOTree.Branch("mcweight"   ,&lMCWeight      ,"lMCWeight/F");//MC Weight (xs/nevents * additional wieght (ie pt weight for gghiggs))
   float lPUWeight    = 0; lOTree.Branch("puweight"   ,&lPUWeight      ,"lPUWeight/F");//Pielup Weight
   float lEffWeight   = 0; lOTree.Branch("effweight"  ,&lEffWeight     ,"lEffWeight/F");//Effieiency Scale factor (all components multiplied in)
   float lWeight      = 0; lOTree.Branch("weight"     ,&lWeight        ,"lWeight/F"  );//mcweight*puweight*effweight
   
   //SV Fit variables
   float lMSV         = 0; lOTree.Branch("m_sv"       ,&lMSV           ,"lMSV/F"     );//SV Fit using integration method
   float lMSVUp       = 0; lOTree.Branch("m_sv_Up"    ,&lMSVUp         ,"lMSVUp/F"   );//High Energy scale shape
   float lMSVDown     = 0; lOTree.Branch("m_sv_Down"  ,&lMSVDown       ,"lMSVDown/F" );//Low Energy Scale Shape
 
   ///First lepton :  muon for mu Tau, electron for e Tau, electron for e mu, Leading (in pT) Tau for Tau Tau
   float lPt1         = 0; lOTree.Branch("pt_1"       ,&lPt1           ,"lPt1/F"     ); //pT 
   float lPhi1        = 0; lOTree.Branch("phi_1"      ,&lPhi1          ,"lPhi1/F"    ); //Phi 
   float lEta1        = 0; lOTree.Branch("eta_1"      ,&lEta1          ,"lEta1/F"    ); //Eta 
   float lM1          = 0; lOTree.Branch("m_1"        ,&lM1            ,"lM1/F"      ); //Mass 
   float lIso1        = 0; lOTree.Branch("iso_1"      ,&lIso1          ,"lIso1/F"    ); //Delta Beta iso value 
   float lMVA1        = 0; lOTree.Branch("mva_1"      ,&lMVA1          ,"lMMVA1/F"   );//MVA id (when using electron) 0 otherwise
   float lD01         = 0; lOTree.Branch("d0_1"       ,&lD01           ,"lD01/F"      );//d0 with respect to primary vertex
   float lDZ1         = 0; lOTree.Branch("dZ_1"       ,&lDZ1           ,"lDZ1/F"      );//dZ with respect to primary vertex
   bool  lPassId1     = 0; lOTree.Branch("passid_1"   ,&lPassId1       ,"lPassId1/B" );//Whether it passes id  (not necessarily iso)
   bool  lPassIso1    = 0; lOTree.Branch("passiso_1"  ,&lPassIso1      ,"lPassIso1/B");//Whether it passes iso (not necessarily id)
   float lMt1         = 0; lOTree.Branch("mt_1"       ,&lMt1           ,"lMt1/F"     );//mT of  first lepton wrt to MVA met

   ///Second lepton :  hadronic Tau for mu Tau had for e Tau, Muon for e mu, Trailing (in pT)  Tau for Tau Tau
   float lPt2         = 0; lOTree.Branch("pt_2"       ,&lPt2           ,"lPt2/F"     );//pT
   float lPhi2        = 0; lOTree.Branch("phi_2"      ,&lPhi2          ,"lPhi2/F"    );//Phi
   float lEta2        = 0; lOTree.Branch("eta_2"      ,&lEta2          ,"lEta2/F"    );//Eta
   float lM2          = 0; lOTree.Branch("m_2"        ,&lM2            ,"lM2/F"      );//Mass (visible mass for hadronic Tau)
   float lIso2        = 0; lOTree.Branch("iso_2"      ,&lIso2          ,"lIso2/F"    );//MVA iso for hadronic Tau, Delta Beta for muon
   float lMVA2        = 0; lOTree.Branch("mva_2"      ,&lMVA2          ,"lMMVA2/F"   );//MVA id (for anti electron id)
   bool  lPassId2     = 0; lOTree.Branch("passid_2"   ,&lPassId2       ,"lPassId2/B" );//Whether it passes id  (not necessarily iso)
   bool  lPassIso2    = 0; lOTree.Branch("passiso_2"  ,&lPassIso2      ,"lPassIso2/B");//Whether it passes iso (not necessarily id)
   float lMt2         = 0; lOTree.Branch("mt_2"       ,&lMt2           ,"lMt2/F"     );//mT of 2nd lepton wrt to MVA met
   float lD02         = 0; lOTree.Branch("d0_2"       ,&lD02           ,"lD02/F"      );//d0 with respect to primary vertex
   float lDZ2         = 0; lOTree.Branch("dZ_2"       ,&lDZ2           ,"lDZ2/F"      );//dZ with respect to primary vertex

   //Met related variables
   double lMet         = 0; lOTree.Branch("met"        ,&lMet           ,"lMet/D"      ); //pfmet
   double lMetPhi      = 0; lOTree.Branch("metphi"     ,&lMetPhi        ,"lMetPhi/D"   ); //pfmet Phi
   double lMVAMet      = 0; lOTree.Branch("mvamet"     ,&lMVAMet        ,"lMVAMet/D"      ); //mvamet
   double lMVAMetPhi   = 0; lOTree.Branch("mvametphi"  ,&lMVAMetPhi     ,"lMVAMetPhi/D"   ); //mvamet Phi
   float lPZetaVis    = 0; lOTree.Branch("pzetavis"   ,&lPZetaVis      ,"lPZetaVis/F" ); //pZeta Visible
   float lPZetaMiss   = 0; lOTree.Branch("pzetamiss"  ,&lPZetaMiss     ,"lPZetaMiss/F"); //pZeta Missing
   //MET covariance matrices
   float lMetCov00   = 0; lOTree.Branch("metcov00"   ,&lMetCov00      ,"lMetCov00/F"); //pf met covariance matrix 00 
   float lMetCov01    = 0; lOTree.Branch("metcov01"   ,&lMetCov01      ,"lMetCov01/F"); //pf met covariance matrix 01 
   float lMetCov10    = 0; lOTree.Branch("metcov10"   ,&lMetCov10      ,"lMetCov10/F"); //pf met covariance matrix 10 
   float lMetCov11    = 0; lOTree.Branch("metcov11"   ,&lMetCov11      ,"lMetCov11/F"); //pf met covariance matrix 11 
   //MVAMet covariance matrices
   float lMVACov00    = 0; lOTree.Branch("mvacov00"   ,&lMVACov00      ,"lMVACov00/F"); //mva met covariance matrix 00 
   float lMVACov01    = 0; lOTree.Branch("mvacov01"   ,&lMVACov01      ,"lMVACov01/F"); //mva met covariance matrix 01 
   float lMVACov10    = 0; lOTree.Branch("mvacov10"   ,&lMVACov10      ,"lMVACov10/F"); //mva met covariance matrix 10 
   float lMVACov11    = 0; lOTree.Branch("mvacov11"   ,&lMVACov11      ,"lMVACov11/F"); //mva met covariance matrix 11 

   //First Jet   : leading jet after applying Jet energy corrections (excluding hadronic Tau)
   float lJPt1       = 0; lOTree.Branch("jpt_1"      ,&lJPt1          ,"lJPt1/F"     );//Jet Pt after corrections
   float lJEta1      = 0; lOTree.Branch("jeta_1"     ,&lJEta1         ,"lJEta1/F"    );//Jet Eta
   float lJPhi1      = 0; lOTree.Branch("jphi_1"     ,&lJPhi1         ,"lJPhi1/F"    );//Jet Phi     
   float lJPtRaw1    = 0; lOTree.Branch("jptraw_1"   ,&lJPtRaw1       ,"lJPtRaw1/F"  );//Jet Raw Pt (before corrections)
   float lJPtUnc1    = 0; lOTree.Branch("jptunc_1"   ,&lJPtUnc1       ,"lJPtUnc1/F"  );//Jet Unc (relative to Jet corrected pT)
   float lJMVA1      = 0; lOTree.Branch("jmva_1"     ,&lJMVA1         ,"lJMVA1/F"    );//Jet MVA id value
   bool  lJPass1     = 0; lOTree.Branch("jpass_1"    ,&lJPass1        ,"lJPass1/B"   );//Whether Jet pass PU Id Loose WP

   //Second Jet  : 2nd leading jet (in pt) afer applying Jet energy corrections (excluding Tau)
   float lJPt2       = 0; lOTree.Branch("jpt_2"      ,&lJPt2          ,"lJPt2/F"     );//Jet Pt after corrections
   float lJEta2      = 0; lOTree.Branch("jeta_2"     ,&lJEta2         ,"lJEta2/F"    );//Jet Eta
   float lJPhi2      = 0; lOTree.Branch("jphi_2"     ,&lJPhi2         ,"lJPhi2/F"    );//Jet Phi
   float lJPtRaw2    = 0; lOTree.Branch("jptraw_2"   ,&lJPtRaw2       ,"lJPtRaw2/F"  );//Jet Raw Pt (before corrections)
   float lJPtUnc2    = 0; lOTree.Branch("jptunc_2"   ,&lJPtUnc2       ,"lJPtUnc2/F"  );//Jet Unc (relative to Jet corrected pT)
   float lJMVA2      = 0; lOTree.Branch("jmva_2"     ,&lJMVA2         ,"lJMVA2/F"    );//Jet MVA id value
   bool  lJPass2     = 0; lOTree.Branch("jpass_2"    ,&lJPass2        ,"lJPass2/B"   );//Whether jet passes PU Id Loose WP 
   
   //B Tagged Jet : leading btagged jet (in pt) passing btag wp (pt > 20 + cvs medium)
   float lBTagPt     = 0; lOTree.Branch("bpt"        ,&lBTagPt        ,"lBTagPt/F"   );//Corrected BTag Pt
   float lBTagEta    = 0; lOTree.Branch("beta"       ,&lBTagEta       ,"lBTagEta/F"  );//Btag Eta
   float lBTagPhi    = 0; lOTree.Branch("bphi"       ,&lBTagPhi       ,"lBTagPhi/F"  );//Btag Phi
 
   //Di Jet kinematic variables for VBF selection ==> Two leading pT Jets 
   double lMJJ        = 0; lOTree.Branch("mjj"        ,&lMJJ           ,"lMJJ/D"      );//Mass Di Jet system  
   double lJDEta      = 0; lOTree.Branch("jdeta"      ,&lJDEta         ,"lJDEta/D"    );//|jeta_1-jeta_2| 
   int   lNJetInGap  = 0; lOTree.Branch("njetingap"  ,&lNJetInGap     ,"lNJetInGap/I");//# of Jets between two jets
   float lMVA        = 0; lOTree.Branch("mva"        ,&lMVA           ,"lMVA/F"      );//VBF MVA value
   
   //Variables that go into the VBF MVA
   double lJDPhi      = 0; lOTree.Branch("jdphi"      ,&lJDPhi         ,"lJDPhi/D"    );//Delta Phi between two leading jets
   double lDiJetPt    = 0; lOTree.Branch("dijetpt"    ,&lDiJetPt       ,"lDiJetPt/D"  );//Pt of the di jet system
   double lDiTauPt    = 0; lOTree.Branch("ditaupt"    ,&lDiTauPt       ,"lDiTauPt/D"  );//Pt of the mu+tau+met system
   double lDiJetPhi   = 0; lOTree.Branch("dijetphi"   ,&lDiJetPhi      ,"lDiJetPhi/D" );//Phi of the di jet system
   double lHDJetPhi   = 0; lOTree.Branch("hdijetphi"  ,&lHDJetPhi      ,"lHDJetPhi/D" );//Phi of the di jet system - Higgs system phi
   double lVisJetEta  = 0; lOTree.Branch("visjeteta"  ,&lVisJetEta     ,"lVisJetEta/D");//TMath::Min(eta_vis - jeta,eta_vis,jeta2);
   double lPtVis      = 0; lOTree.Branch("ptvis"      ,&lPtVis         ,"lPtVis/D"    );//Pt Vis
  
   //number of btags passing btag id ( pt > 20 )
   int   lNBTag      = 0; lOTree.Branch("nbtag"      ,&lNBTag         ,"lNBTag/I");

   //number of jets passing jet id ( pt > 30 )
   int   lNJets      = 0; lOTree.Branch("njets"      ,&lNJets         ,"lNJets/I");
  



   /////////Get the input sample:
   //Sample sample("ZToTauTau","output/JoseMay22");
   //Sample sample("GluGluSync","output/JoseMay22");
   //Sample sample("VBFSync","output/JoseMay22");

   //Sample sample("GluGluSync","output/V530JoseMay22");
   //Sample sample("VBFSync","output/V530JoseMay22");
  
   //Sample sample("VBFSync","output/V530JoseMay25");
   //Sample sample("GluGluSync","output/V530JoseMay25");

   //Sample sample("VBFSync","output/V530JoseMay26");
   //Sample sample("VBFSync","output/V530JoseMay26_2");
   //Sample sample("VBFSync","output/V530JoseMay26btag");
   //Sample sample("VBFSync","output/V530JoseMay28_testPFJetId");
   //Sample sample("VBFSync","output/V541May30");
   //Sample sample("VBFSync","output/TauMuV541June2_Sync");
   //Sample sample("VBFSync","output/TauMuV541June2_Sync2");
   //Sample sample("VBFSync","output/HTTSync_June13");
   //Sample sample("HiggsVBF125","/data/benitezj/Samples/TauMuV541June2_TrigEff");
   //Sample sample("HiggsVBF120","/data/benitezj/Samples/TauMuV541June2_TrigEff");

   //Sample sample("GluGluSync","/afs/cern.ch/user/b/benitezj/output/TauMu2011FlatNtpJuly23");
   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauMu2011FlatNtpJuly23");

   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauMu2011V550July28svfit");
   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauMu2012V550July29svfit");



   ////TauEle 
   //Sample sample("VBFSync","output/TauEleV541June28_Sync");
   //Sample sample("SUSYBB200","/data/benitezj/Samples/TauEleV541June29_FlatNtpJune29");
   //Sample sample("TauPlusXAug","/afs/cern.ch/user/b/benitezj/output/TauEle2011FlatNtpJuly23");
   //Sample sample("GluGluSync","/afs/cern.ch/user/b/benitezj/output/TauEle2011FlatNtpJuly23");
   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauEle2011FlatNtpJuly23");
   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauEle2011FlatNtpJuly25");
   //Sample sample("HiggsVBF125","/afs/cern.ch/user/b/benitezj/output/TauEle2011FlatNtpJuly25");

   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauEle2011V550July28svfit");
   //Sample sample("VBFSync","/afs/cern.ch/user/b/benitezj/output/TauEle2012V550July29svfit");

   //***************************************
   TChain * InputTree = sample.getTChain();  
 
   int categoryIso=0;
   InputTree->SetBranchAddress("categoryIso",&categoryIso);



   //InputTree->SetBranchAddress("",&);
   InputTree->SetBranchAddress("eventid",&lEvt);
   InputTree->SetBranchAddress("lumiblock",&lLumi);
   InputTree->SetBranchAddress("runnumber",&lRun);
   InputTree->SetBranchAddress("nvtx",&lNPV);
   InputTree->SetBranchAddress("npu",&lNPU);
   //InputTree->SetBranchAddress("rho",&);

   InputTree->SetBranchAddress("eventweight",&lWeight);
   InputTree->SetBranchAddress("pupWeight",&lPUWeight);
   InputTree->SetBranchAddress("signalWeight",&lMCWeight);

   float triggerEffWeight;
   InputTree->SetBranchAddress("triggerEffWeight",&triggerEffWeight);
   float selectionEffWeight;
   InputTree->SetBranchAddress("selectionEffWeight",&selectionEffWeight);
   float embeddedGenWeight;
   InputTree->SetBranchAddress("embeddedGenWeight",&embeddedGenWeight);

   InputTree->SetBranchAddress("mupt",&lPt1);
   InputTree->SetBranchAddress("mueta",&lEta1);
   InputTree->SetBranchAddress("muphi",&lPhi1);
   InputTree->SetBranchAddress("mudxy",&lD01);
   InputTree->SetBranchAddress("mudz",&lDZ1);
   InputTree->SetBranchAddress("muiso",&lIso1);
   InputTree->SetBranchAddress("transversemass",&lMt1);

   InputTree->SetBranchAddress("taupt",&lPt2);
   InputTree->SetBranchAddress("taueta",&lEta2);
   InputTree->SetBranchAddress("tauphi",&lPhi2);
   InputTree->SetBranchAddress("taudxy",&lD02);
   InputTree->SetBranchAddress("taudz",&lDZ2);

   //   InputTree->SetBranchAddress("metpt",&lMet);
   //   InputTree->SetBranchAddress("metphi",&lMetPhi);
   InputTree->SetBranchAddress("metpt",&lMVAMet);
   InputTree->SetBranchAddress("metphi",&lMVAMetPhi);
   
   InputTree->SetBranchAddress("metsigcov00",&lMVACov00);
   InputTree->SetBranchAddress("metsigcov01",&lMVACov01);
   InputTree->SetBranchAddress("metsigcov10",&lMVACov10);
   InputTree->SetBranchAddress("metsigcov11",&lMVACov11);
   

   InputTree->SetBranchAddress("svfitmass",&lMSV);
   
   InputTree->SetBranchAddress("njet",&lNJets);

   InputTree->SetBranchAddress("leadJetPt",&lJPt1);
   InputTree->SetBranchAddress("leadJetEta",&lJEta1);
   float leadJetRawFactor=0.;
   InputTree->SetBranchAddress("leadJetRawFactor",&leadJetRawFactor);

   InputTree->SetBranchAddress("subleadJetPt",&lJPt2);
   InputTree->SetBranchAddress("subleadJetEta",&lJEta2);

   InputTree->SetBranchAddress("nbjet",&lNBTag);
   InputTree->SetBranchAddress("leadBJetPt",&lBTagPt);
   InputTree->SetBranchAddress("leadBJetEta",&lBTagEta);
 
   //InputTree->SetBranchAddress("diJetMass",&lMJJ);
   //InputTree->SetBranchAddress("diJetDeltaEta",&lJDEta);
   InputTree->SetBranchAddress("vbfvars0",&lMJJ);
   InputTree->SetBranchAddress("vbfvars1",&lJDEta);
   InputTree->SetBranchAddress("vbfvars2",&lJDPhi);
   InputTree->SetBranchAddress("vbfvars3",&lDiTauPt);//mu+tau+met pt
   InputTree->SetBranchAddress("vbfvars4",&lDiJetPt);
   InputTree->SetBranchAddress("vbfvars5",&lHDJetPhi);
   InputTree->SetBranchAddress("vbfvars6",&lVisJetEta);
   InputTree->SetBranchAddress("vbfvars7",&lPtVis);

   InputTree->SetBranchAddress("vbfmva",&lMVA);

   //InputTree->SetBranchAddress("",&);

   for (Long64_t i0=0; i0<InputTree->GetEntries();i0++) {
     if (i0 % 20 == 0) std::cout << "--- ... Processing event: " << i0 << std::endl;
     InputTree->GetEntry(i0);

     if(categoryIso!=1) continue;
     
     lEffWeight = triggerEffWeight*selectionEffWeight*embeddedGenWeight;
     lJPtRaw1 = lJPt1*leadJetRawFactor;

     lOTree.Fill();
   }  
   TFile lOFile("Output.root","RECREATE");
   lOTree.Write();
   lOFile.Close();
}

////////Josh
// /afs/cern.ch/user/s/swanson/public/EventListsRound2/
///////Colin
// ~cbern/public/eventList.py  Output_VBF_V530JoseMay24.root -t TauCheck >> Output_VBF_V530JoseMay24.txt 
// ~cbern/public/diffEventList.py Output_VBF_V530JoseMay25.txt /afs/cern.ch/user/s/swanson/public/EventListsRound3/MuTauVBF.txt
// /afs/cern.ch/user/c/cbern/public/HTTSync/May22_c
////Matthew
// /afs/cern.ch/user/m/mhchan/public/Htt/sync/mutau_h125tt-vbf_sync_v3.root
////Lorenzo
// ~bianchi/public/HTTSync/May22/

///Scan events in the sync Ntuple
///t->Scan("d0_1:dZ_1:pt_1:eta_1:iso_1:d0_2:dZ_2:pt_2:eta_2","evt==4394","",100000,0)


void drawHistos(TTree* Tmine, TTree* Tother,TString var, int nbins, float xmin, float xmax, TString selection){
  TH1F* Hmine = new TH1F(TString("Hmine")+var,"",nbins,xmin,xmax); 
  Hmine->GetXaxis()->SetTitle(var);
  Hmine->SetLineColor(1);
  Hmine->SetStats(0);
  TH1F* Hother = new TH1F(TString("Hother")+var,"",nbins,xmin,xmax); 
  Hother->GetXaxis()->SetTitle(var);
  Hother->SetLineColor(2);
  Hother->SetStats(0);

  TText TXmine;
  TXmine.SetTextColor(1);
  TXmine.SetTextSize(.04);
  TText TXother;
  TXother.SetTextColor(2);
  TXother.SetTextSize(.04);

  Tmine->Draw(var+">>"+Hmine->GetName(),selection);
  Tother->Draw(var+">>"+Hother->GetName(),selection);

  if(Hmine->GetMaximum()>Hother->GetMaximum()){
    Hmine->Draw("hist");
    Hother->Draw("histsame");
  }else {
    Hother->Draw("hist");
    Hmine->Draw("histsame");
  }

  TXmine.DrawTextNDC(.75,.82,TString("mine ")+(long)(Hmine->Integral()));
  TXother.DrawTextNDC(.75,.77,TString("other ")+(long)(Hother->Integral()));
}

void compareSync(){
  //gStyle->SetOptStat("");

  //TFile Fmine("./Output.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/May25/Output_VBF_V530JoseMay25.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/May25/Output_GG_V530JoseMay25.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/May26/Output_VBF_V530JoseMay26.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/May28/Output_VBF_V530JoseMay26btag.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/May30/Output_VBF_V541May30.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/June13/muTauVBF.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July25/muTau_2011_VBFSync.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July28/muTau2011_VBFSync.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July28/muTau2012_VBFSync.root");

  //Colin
  //TFile Fother("/afs/cern.ch/user/c/cbern/public/HTTSync/May22_c/ggh.root","read");  
  //TFile Fother("/tmp/H2TauTauTreeProducerTauMuXCheck_tree.root","read");  
  //TTree*Tother=(TTree*)Fother.Get("H2TauTauTreeProducerTauMuXCheck");

  //Lorenzo
  //TFile Fother("/afs/cern.ch/user/b/bianchi/public/HTTSync/May24/OutputMuTau_VBFH125.root");
  //TTree*Tother=(TTree*)Fother.Get("TauCheck");

  //Matthew
  //TFile Fother("/afs/cern.ch/user/m/mhchan/public/Htt/sync/mutau_h125tt-vbf_sync_v3.root");
  //TTree*Tother=(TTree*)Fother.Get("TauCheck");
  
  //Josh
  //TFile Fother("/afs/cern.ch/user/s/swanson/public/EventListsRound4/muTauVBF.root"); 
  //TFile Fother("/afs/cern.ch/user/s/swanson/public/Sync12062012/muTauVBF.root");
  //TFile Fother("/afs/cern.ch/user/s/swanson/public/Sync42XMuTau/muTauPreselection.root");
  //TFile Fother("/afs/cern.ch/user/s/swanson/public/Sync52XMuTau/muTauPreselection.root");
  //TTree*Tother=(TTree*)Fother.Get("eventTree");

  //Andrew
  //TFile Fother("/afs/cern.ch/user/a/agilbert/public/HTTSync2/vbf_mutau_v2.root");
  //TTree*Tother=(TTree*)Fother.Get("TauCheck");




  /////////////TauEle channel 
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/TauEleJune28/Output_VBFSync.root","read");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July24/eTau_2011_TauPlusXAug.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July25/eTau_2011_GluGluSync.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July25/eTau_2011_VBFSync.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July25b/eTau_2011_VBFSync.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July25b/eTau_2011_HiggsVBF125.root");
  //TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July28/eTau2011_VBFSync.root");
  TFile Fmine("/afs/cern.ch/user/b/benitezj/public/HTTSync/July28/eTau2012_VBFSync.root");
  
  //TFile Fother("/afs/cern.ch/user/b/bianchi/public/HTTSync/May24/OutputElecTau_VBFH125.root");
  //TFile Fother("/afs/cern.ch/user/m/mhchan/public/Htt/sync/etau_h125tt-vbf_sync_v5.root");
  //TTree*Tother=(TTree*)Fother.Get("TauCheck");

  //TFile Fother("/afs/cern.ch/user/g/govoni/work/public/sync/sync_aug04_tree.root");
  //TFile Fother("/afs/cern.ch/user/g/govoni/work/public/sync/PG_VBFH125.root");
  //TFile Fother("/afs/cern.ch/user/g/govoni/work/public/sync/PG_GGH125.root");
  //TTree*Tother=(TTree*)Fother.Get("H2TauTauTreeProducerTauMuXCheck");

  //TFile Fother("/afs/cern.ch/user/s/swanson/public/Sync42XEleTau/eleTauPreselection.root");
  TFile Fother("/afs/cern.ch/user/s/swanson/public/Sync52XEleTau/eleTauPreselection.root");
  TTree*Tother=(TTree*)Fother.Get("eventTree");

  
  ////////////////
  TTree*Tmine=(TTree*)Fmine.Get("TauCheck");


  cout<<"Mine: "<<Fmine.GetName()<<endl;
  cout<<"Other: "<<Fother.GetName()<<endl;
  TCanvas C;
  C.Print("compareSync.ps[");  


  //inclusive
  TString selection="1";

  C.Clear();
  drawHistos(Tmine,Tother,"npu",50,-.5,49.5,selection);
  C.Print("compareSync.ps");
   
  C.Clear();
  drawHistos(Tmine,Tother,"npv",50,-.5,49.5,selection);
  C.Print("compareSync.ps");
   
  C.Clear();
  drawHistos(Tmine,Tother,"pt_1",200,0,200,selection);
  C.Print("compareSync.ps");
   
  C.Clear();
  drawHistos(Tmine,Tother,"eta_1",60,-3,3,selection);
  C.Print("compareSync.ps");

  C.Clear();
  drawHistos(Tmine,Tother,"pt_2",200,0,200,selection);
  C.Print("compareSync.ps");

  C.Clear();
  drawHistos(Tmine,Tother,"eta_2",60,-3,3,selection);
  C.Print("compareSync.ps");

  C.Clear();
  drawHistos(Tmine,Tother,"met",20,0,200,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"metphi",30,-3.5,3.5,selection);
  C.Print("compareSync.ps");

  C.Clear();
  drawHistos(Tmine,Tother,"m_sv",30,0,300,selection);
  C.Print("compareSync.ps");



  //   ///Jets
  selection="(njets>0)";
  C.Clear();
  drawHistos(Tmine,Tother,"jpt_1",100,0,300,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"jeta_1",100,-5,5,selection);
  C.Print("compareSync.ps");

  selection="(njets>1)";
  C.Clear();
  drawHistos(Tmine,Tother,"jpt_2",100,0,300,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"jeta_2",100,-5,5,selection);
  C.Print("compareSync.ps");

  //vbf mva
  C.Clear();
  drawHistos(Tmine,Tother,"mjj",100,0,3000,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"jdeta",100,0,10,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"jdphi",100,0,3.5,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"dijetpt",100,0,500,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"hdijetphi",100,0,3.5,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"visjeteta",100,0,10,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"ptvis",100,0,500,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"mva",40,-1,1.001,selection);
  C.Print("compareSync.ps");



  //   //b-jets
  selection="(nbtag>0)";
  C.Clear();
  drawHistos(Tmine,Tother,"bpt",100,0,300,selection);
  C.Print("compareSync.ps");
  C.Clear();
  drawHistos(Tmine,Tother,"beta",100,-5,5,selection);
  C.Print("compareSync.ps");



  C.Print("compareSync.ps]");
}
