
#include "TMath.h"
TFile *inf_data;
TFile *outf;
string sTmp("/tmp/mgouzevi/tmp");
string sChannel("");

double M_PI = TMath::Pi();

void ReadTree_any(char* file, char* outLabel, char* c_channel)
{
  // style definition -------------------------------
  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  string channel(c_channel);

  //  TFile *inf_data  = TFile::Open("rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  //  TFile *outf = new TFile("histogrmas_data_all.root","RECREATE");
  string outFile("histograms_");
  outFile = outFile + "" + outLabel + "_" + channel + ".root";
  sTmp = sTmp + "_" + outLabel + "_" + channel + ".root";
  sChannel = sChannel + "" + channel + "/ProcessedTree";

  outf = new TFile(outFile.c_str(),"RECREATE");




  TFile *inf_data  = TFile::Open(file);//"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  
  

 



  TTree *tr_data_large = (TTree*)inf_data->Get(sChannel.c_str());
  //  TTree *tr_data = ntupleR2S->CopyTree();


  // p_T and mass boundaries ---------------------------
  
  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 
			  74,  88, 103, 119, 137, 156, 176, 197, 220, 244, 
			 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 
			 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 
  double ptBoundaries[55] = {1, 9, 18, 29, 40, 53, 67, 81, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3500};

  Double_t tAxis[72];
  for (unsigned i = 0; i<72; i++) tAxis[i] = i*1.0-0.5;

  // histogrmas definition ------------------------------


  TH1 *h_Pthat_data = new TH1F("h_Pthat_data","PtHat_data",200, 0, 2000);



  TH2 *h_DijetMass_data_MassShape = new TH2F("h_DijetMass_data_MassShape","DijetMass_data_MassShape", 50, 0.3, 1.3, 6, -0.5, 5.5);
  TH2 *h_DijetMass_data_MassShape_pf = new TH2F("h_DijetMass_data_MassShape_pf","DijetMass_data_MassShape_pf", 50, 0.3, 1.3, 6, -0.5, 5.5);
  TH2 *h_DijetMass_data_MassShape_fat = new TH2F("h_DijetMass_data_MassShape_fat","DijetMass_data_MassShape_fat", 50, 0.3, 1.3, 6, -0.5, 5.5);

  TH2 *h_DijetMass_data_MassShape_mu10_fat = new TH2F("h_DijetMass_data_MassShape_mu10_fat","DijetMass_data_MassShape_mu10_fat", 50, 0.3, 1.3, 6, -0.5, 5.5);

  vector <double> ResonanceMass;
  ResonanceMass.push_back(500);  ResonanceMass.push_back(700);  ResonanceMass.push_back(1200);  
  ResonanceMass.push_back(2000);  ResonanceMass.push_back(3500);  ResonanceMass.push_back(4000);

  // ---------- Calo jets ---------------------

  TH1 *h_DijetMass_data = new TH1F("h_DijetMass_data","DijetMass_data",83, xAxis1);
  TH1 *h_DijetMass_data_up = new TH1F("h_DijetMass_data_up","DijetMass_data_up",83, xAxis1);
  TH1 *h_DijetMass_data_do = new TH1F("h_DijetMass_data_do","DijetMass_data_do",83, xAxis1);

  TH1 *h_DijetMass_MI_nPVe1_data = new TH1F("h_DijetMass_MI_nPVe1_data","DijetMass_MI_nPVe1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg1_data = new TH1F("h_DijetMass_MI_nPVg1_data","DijetMass_MI_nPVg1_data",83, xAxis1);
  TH2 *h_Eta_Phi_Scatter_data = new TH2F("h_Eta_Phi_Scatter","Eta_Phi_Scatter",100,-3,3,100,-3.2,3.2);
  TH2 *h_Eta_Phi_Scatter_data_1 = new TH2F("h_Eta_Phi_Scatter_1","Eta_Phi_Scatter_1",100,-3,3,100,-3.2,3.2);
  TH2 *h_Eta_Phi_Scatter_data_2 = new TH2F("h_Eta_Phi_Scatter_2","Eta_Phi_Scatter_2",100,-3,3,100,-3.2,3.2);
  TH1 *h_MET_over_sumEt_data = new TH1F("h_MET_over_sumEt_data","MET_over_sumEt_data",25,0.0,1.0001);
  TH1 *h_EMF_data = new TH1F("h_EMF_data","EMF_data",25,0.0,1.0001);
  TH1 *h_DPhi_data = new TH1F("h_DPhi_data","DPhi_data",25,0.0,3.142);
  TH1 *h_Eta_data = new TH1F("h_Eta_data","Eta_data",60,-3.0,3.0);
  TH1 *h_Eta_data_1 = new TH1F("h_Eta_data_1","Eta_data_1",60,-3.0,3.0);
  TH1 *h_Eta_data_2 = new TH1F("h_Eta_data_2","Eta_data_2",60,-3.0,3.0);

  TH1 *h_DEta_data = new TH1F("h_DEta_data","DEta_data",15,0.,1.5);
  TH1 *h_Phi_data = new TH1F("h_Phi_data","Phi_data",18,-3.142,3.142);
  TH1 *h_Phi_data_1 = new TH1F("h_Phi_data_1","Phi_data_1",18,-3.142,3.142);
  TH1 *h_Phi_data_2 = new TH1F("h_Phi_data_2","Phi_data_2",18,-3.142,3.142);

  TH1 *h_n90hits_data = new TH1F("h_n90hits_data","n90hits_data",400,-0.5,399.5);
  TH1 *h_fHPD_data = new TH1F("h_fHPD_data","fHPD_data",25,0.0,1.0001);
  TH1 *h_corPt_data = new TH1F("h_corPt_data","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_1 = new TH1F("h_corPt_data_1","corPt_data_1",54,ptBoundaries);
  TH1 *h_corPt_data_2 = new TH1F("h_corPt_data_2","corPt_data_2",54,ptBoundaries);
  TH1 *h_nTrkVtx_data = new TH1F("h_nTrkVtx_data","nTrkVtx_data",900,-0.5,899.5);
  TH1 *h_nTrkCalo_data = new TH1F("h_nTrkCalo_data","nTrkCalo_data",100,-0.5,99.5);


  // --------- PF Jets ------------------------


  TH1 *h_DijetMass_MI_nPVl1_data_pf = new TH1F("h_DijetMass_MI_nPVl1_data_pf","DijetMass_MI_nPVl1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVl3_data_pf = new TH1F("h_DijetMass_MI_nPVl3_data_pf","DijetMass_MI_nPVl3_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVl6_data_pf = new TH1F("h_DijetMass_MI_nPVl6_data_pf","DijetMass_MI_nPVl6_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg6_data_pf = new TH1F("h_DijetMass_MI_nPVg6_data_pf","DijetMass_MI_nPVg6_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg10_data_pf = new TH1F("h_DijetMass_MI_nPVg10_data_pf","DijetMass_MI_nPVg10_data",83, xAxis1);

  TH1 *h_nVtx_pf = new TH1F("h_Nvx_pf","Number of vertex",15, 0.5, 15.5);
  TH2 *h_nVtx_DijetMass_pf = new TH2F("h_nVtx_DijetMass_pf","Number of vertex",15, 0.5, 15.5, 83, xAxis1);

  TH2 *h_nVtx_fCh_pf = new TH2F("h_nVtx_fCh_pf","Number of vertex",20, .5, 20.5, 25,0.0,1.0001);
  TH2 *h_nVtx_fNh_pf = new TH2F("h_nVtx_fNh_pf","Number of vertex",20, .5, 20.5, 25,0.0,1.0001);
  TH2 *h_nVtx_fPh_pf = new TH2F("h_nVtx_fPh_pf","Number of vertex",20, .5, 20.5, 25,0.0,1.0001);
  TH2 *h_nVtx_fEl_pf = new TH2F("h_nVtx_fEl_pf","Number of vertex",20, .5, 20.5, 25,0.0,1.0001);
  TH2 *h_nVtx_fMu_pf = new TH2F("h_nVtx_fMu_pf","Number of vertex",20, .5, 20.5, 25,0.0,1.0001);


  TH1 *h_DijetMass_data_pf = new TH1F("h_DijetMass_data_pf","DijetMass_data_pf",83, xAxis1);
  TH1 *h_DijetMass_data_pf_up = new TH1F("h_DijetMass_data_pf_up","DijetMass_data_pf_up",83, xAxis1);
  TH1 *h_DijetMass_data_pf_do = new TH1F("h_DijetMass_data_pf_do","DijetMass_data_pf_do",83, xAxis1);

  TH1 *h_DEta_data_pf = new TH1F("h_DEta_data_pf","DEta_data_pf",15,0.,1.5);
  TH1 *h_DPhi_data_pf = new TH1F("h_DPhi_data_pf","DPhi_data_pf",25,0.0,3.142);
  TH1 *h_Eta_data_pf = new TH1F("h_Eta_data_pf","Eta_data_pf",60,-3.0,3.0);
  TH1 *h_Eta_data_pf_1 = new TH1F("h_Eta_data_pf_1","Eta_data_pf_1",60,-3.0,3.0);
  TH1 *h_Eta_data_pf_2 = new TH1F("h_Eta_data_pf_2","Eta_data_pf_2",60,-3.0,3.0);
  TH1 *h_Phi_data_pf = new TH1F("h_Phi_data_pf","Phi_data_pf",18,-3.142,3.142);
  TH1 *h_Phi_data_pf_1 = new TH1F("h_Phi_data_pf_1","Phi_data_pf_1",18,-3.142,3.142);
  TH1 *h_Phi_data_pf_2 = new TH1F("h_Phi_data_pf_2","Phi_data_pf_2",18,-3.142,3.142);
  TH1 *h_corPt_data_pf = new TH1F("h_corPt_data_pf","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_pf_1 = new TH1F("h_corPt_data_pf_1","corPt_data_pf_1",54,ptBoundaries);
  TH1 *h_corPt_data_pf_2 = new TH1F("h_corPt_data_pf_2","corPt_data_pf_2",54,ptBoundaries);


  //  TProfile* p_dMass_vs_DijetMass_data_fat_up = new TProfile("p_dMass_vs_DijetMass_data_fat_up", "Var up of Dijet Mass up", ,83, xAxis1);
  //  TProfile* p_dMass_vs_DijetMass_data_fat_do = new TProfile("p_dMass_vs_DijetMass_data_fat_do", "Var up of Dijet Mass do", ,83, xAxis1);

  TProfile2D* p_RvsEta_data_pf = new TProfile2D("p_RvsEta_data_pf", ";#eta;fraction", 12, -3, 3, 4, -0.5, 3.5);
  TProfile2D* p_RvsPt_barrel_data_pf = new TProfile2D("p_RvsPt_barrel_data_pf", "Barrel;p_{T} (GeV/c);fraction", 50, 0, 200, 4, -0.5, 3.5 );
  TProfile2D* p_RvsPt_endcaps_data_pf   = new TProfile2D("p_RvsPt_endcaps_data_pf", "Endcaps;p_{T} (GeV/c);fraction", 50, 0, 200, 4, -0.5, 3.5 );
  TProfile2D* p_RvsPhi_barrel_data_pf   = new TProfile2D("p_RvsPhi_barrel_data_pf", "Barrel;#phi;fraction", 50, -3.2, 3.2, 4, -0.5, 3.5 );
  TProfile2D* p_RvsPhi_endcaps_data_pf   = new TProfile2D("p_RvsPhi_endcaps_data_pf", "Endcaps;#phi;fraction", 50, -3.2, 3.2, 4, -0.5, 3.5 );

  TH1 *h_fCh_data_pf = new TH1F("h_fCh_data_pf","Charged Fraction",25,0.0,1.0001);
  TH1 *h_fNh_data_pf = new TH1F("h_fNh_data_pf","Neutral Hadron Fraction",25,0.0,1.0001);
  TH1 *h_fPh_data_pf = new TH1F("h_fPh_data_pf","Photon Fraction",25,0.0,1.0001);
  TH1 *h_fEl_data_pf = new TH1F("h_fEl_data_pf","Electrons Fraction",25,0.0,1.0001);
  TH1 *h_fMu_data_pf = new TH1F("h_fMu_data_pf","Muon Fraction",25,0.0,1.0001);

  TH1 *h_fCh_data_pf_1 = new TH1F("h_fCh_data_pf_1","Charged Fraction jet1",25,0.0,1.0001);
  TH1 *h_fNh_data_pf_1 = new TH1F("h_fNh_data_pf_1","Neutral Hadron Fraction jet1",25,0.0,1.0001);
  TH1 *h_fPh_data_pf_1 = new TH1F("h_fPh_data_pf_1","Photon Fraction jet1",25,0.0,1.0001);
  TH1 *h_fEl_data_pf_1 = new TH1F("h_fEl_data_pf_1","Electrons Fraction jet1",25,0.0,1.0001);
  TH1 *h_fMu_data_pf_1 = new TH1F("h_fMu_data_pf_1","Muon Fraction jet1",25,0.0,1.0001);

  TH1 *h_fCh_data_pf_2 = new TH1F("h_fCh_data_pf_2","Charged Fraction jet2",25,0.0,1.0001);
  TH1 *h_fNh_data_pf_2 = new TH1F("h_fNh_data_pf_2","Neutral Hadron Fraction jet2",25,0.0,1.0001);
  TH1 *h_fPh_data_pf_2 = new TH1F("h_fPh_data_pf_2","Photon Fraction jet2",25,0.0,1.0001);
  TH1 *h_fEl_data_pf_2 = new TH1F("h_fEl_data_pf_2","Electrons Fraction jet2",25,0.0,1.0001);
  TH1 *h_fMu_data_pf_2 = new TH1F("h_fMu_data_pf_2","Muon Fraction jet2",25,0.0,1.0001);


  // --------- Fat Jets ------------------------

  TH1 *h_MET_over_sumEt_fat_data = new TH1F("h_MET_over_sumEt_fat_data","MET_over_sumEt_fat_data",25,0.0,1.0001);

  TH1 *h_DijetMass_data_fat = new TH1F("h_DijetMass_data_fat","DijetMass_data_fat",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu30 = new TH1F("h_DijetMass_data_fat_mu30","DijetMass_data_fat_mu30",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu20 = new TH1F("h_DijetMass_data_fat_mu20","DijetMass_data_fat_mu20",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu10 = new TH1F("h_DijetMass_data_fat_mu10","DijetMass_data_fat_mu10",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_e30 = new TH1F("h_DijetMass_data_fat_e30","DijetMass_data_fat_e30",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_e20 = new TH1F("h_DijetMass_data_fat_e20","DijetMass_data_fat_e20",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_e10 = new TH1F("h_DijetMass_data_fat_e10","DijetMass_data_fat_e10",83, xAxis1);

  TH1 *h_DijetMass_data_fat_up = new TH1F("h_DijetMass_data_fat_up","DijetMass_data_fat_up",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu30_up = new TH1F("h_DijetMass_data_fat_mu30_up","DijetMass_data_fat_mu30_up",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu20_up = new TH1F("h_DijetMass_data_fat_mu20_up","DijetMass_data_fat_mu20_up",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu10_up = new TH1F("h_DijetMass_data_fat_mu10_up","DijetMass_data_fat_mu10_up",83, xAxis1); 

  TH1 *h_DijetMass_data_fat_do = new TH1F("h_DijetMass_data_fat_do","DijetMass_data_fat_do",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu30_do = new TH1F("h_DijetMass_data_fat_mu30_do","DijetMass_data_fat_mu30_do",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu20_do = new TH1F("h_DijetMass_data_fat_mu20_do","DijetMass_data_fat_mu20_do",83, xAxis1); 
  TH1 *h_DijetMass_data_fat_mu10_do = new TH1F("h_DijetMass_data_fat_mu10_do","DijetMass_data_fat_mu10_do",83, xAxis1); 


  TH2 *h_DijetMass_Trigger_data_pf = new TH2F("h_DijetMass_Trigger_data_pf","DijetMass_Trigger_data_pf",83, xAxis1, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_pf_Corr = new TH3F("h_DijetMass_Trigger_data_pf_Corr","DijetMass_Trigger_data_pf",83, xAxis1, 71, tAxis, 71, tAxis); 

  TH3 *h_DijetMass_Trigger_data_pf_Corr_andHT600 = new TH3F("h_DijetMass_Trigger_data_pf_Corr_andHT600","DijetMass_Trigger_data_pf_andHT600",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_pf_Corr_andHT650 = new TH3F("h_DijetMass_Trigger_data_pf_Corr_andHT650","DijetMass_Trigger_data_pf_andHT650",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_pf_Corr_andHT700 = new TH3F("h_DijetMass_Trigger_data_pf_Corr_andHT700","DijetMass_Trigger_data_pf_andHT700",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_pf_Corr_andHT750 = new TH3F("h_DijetMass_Trigger_data_pf_Corr_andHT750","DijetMass_Trigger_data_pf_andHT750",83, xAxis1, 71, tAxis, 71, tAxis); 


  TH2 *h_DijetMass_Trigger_data_fat = new TH2F("h_DijetMass_Trigger_data_fat","DijetMass_Trigger_data_fat",83, xAxis1, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr = new TH3F("h_DijetMass_Trigger_data_fat_Corr","DijetMass_Trigger_data_fat",83, xAxis1, 71, tAxis, 71, tAxis); 

  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT600 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT600","DijetMass_Trigger_data_fat_andHT600",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT650 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT650","DijetMass_Trigger_data_fat_andHT650",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT700 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT700","DijetMass_Trigger_data_fat_andHT700",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT750 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT750","DijetMass_Trigger_data_fat_andHT750",83, xAxis1, 71, tAxis, 71, tAxis); 


  TH2 *h_DijetMass_Trigger_data_fat_mu10 = new TH2F("h_DijetMass_Trigger_data_fat_mu10","DijetMass_Trigger_data_fat_mu10",83, xAxis1, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_mu10 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_mu10","DijetMass_Trigger_data_fat_mu10",83, xAxis1, 71, tAxis, 71, tAxis); 

  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT600_mu10 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT600_mu10","DijetMass_Trigger_data_fat_andHT600_mu10",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT650_mu10 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT650_mu10","DijetMass_Trigger_data_fat_andHT650_mu10",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT700_mu10 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT700_mu10","DijetMass_Trigger_data_fat_andHT700_mu10",83, xAxis1, 71, tAxis, 71, tAxis); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr_andHT750_mu10 = new TH3F("h_DijetMass_Trigger_data_fat_Corr_andHT750_mu10","DijetMass_Trigger_data_fat_andHT750_mu10",83, xAxis1, 71, tAxis, 71, tAxis); 

  TH1 *h_DijetMass_MI_nPVl1_data_fat = new TH1F("h_DijetMass_MI_nPVl1_data_fat","DijetMass_MI_nPVl1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVl3_data_fat = new TH1F("h_DijetMass_MI_nPVl3_data_fat","DijetMass_MI_nPVl3_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVl6_data_fat = new TH1F("h_DijetMass_MI_nPVl6_data_fat","DijetMass_MI_nPVl6_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg6_data_fat = new TH1F("h_DijetMass_MI_nPVg6_data_fat","DijetMass_MI_nPVg6_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg10_data_fat = new TH1F("h_DijetMass_MI_nPVg10_data_fat","DijetMass_MI_nPVg10_data",83, xAxis1);

  TH1 *h_nVtx_fat = new TH1F("h_Nvx_fat","Number of vertex",15, 0.5, 15.5);
  TH2 *h_nVtx_DijetMass_fat = new TH2F("h_nVtx_DijetMass_fat","Number of vertex",15, 0.5, 15.5, 83, xAxis1);







  TH1 *h_DpT_over_pT_data_fat_1 = new TH1F("h_DpT_over_pT_data_fat_1","DpT_over_pT_data_fat",30,0.,3.0);
  TH1 *h_DpT_data_fat_1 = new TH1F("h_DpT_data_fat_1","DpT_data_fat_1",30,0.,300.0);
  TH2 *h_DpT_over_pT_vs_pT_data_fat_1 = new TH2F("h_DpT_over_pT_vs_pT_data_fat_1","DpT_over_pT_vs_pT_data_fat",30,0.,3.0, 200, 0, 2000);

  TH1 *h_Deta_fatlead_data_fat_1 = new TH1F("h_Deta_data_fat_1","Dphi_data_fat",80,-1.,1.0);
  TH1 *h_Dphi_fatlead_data_fat_1 = new TH1F("h_Dphi_data_fat_1","Dphi_data_fat",80,-1.,1.0);

  TH1 *h_DpT_over_pT_data_fat_2 = new TH1F("h_DpT_over_pT_data_fat_2","DpT_over_pT_data_fat",30,0.,3.0);
  TH1 *h_DpT_data_fat_2 = new TH1F("h_DpT_data_fat_2","DpT_data_fat",30,0.,300.0);
  TH2 *h_DpT_over_pT_vs_pT_data_fat_2 = new TH2F("h_DpT_over_pT_vs_pT_data_fat_2","DpT_over_pT_vs_pT_data_fat",30,0.,3.0, 200, 0, 2000);

  TH1 *h_Deta_fatlead_data_fat_2 = new TH1F("h_Deta_data_fat_2","Dphi_data_fat",80,-1.,1.0);
  TH1 *h_Dphi_fatlead_data_fat_2 = new TH1F("h_Dphi_data_fat_2","Dphi_data_fat",80,-1.,1.0);





  TH1 *h_DpT_over_pT_data_fat = new TH1F("h_DpT_over_pT_data_fat","DpT_over_pT_data_fat",30,0.,3.0);
  TH1 *h_DpT_data_fat = new TH1F("h_DpT_data_fat","DpT_data_fat",30,0.,300.0);
  TH2 *h_DpT_over_pT_vs_pT_data_fat = new TH2F("h_DpT_over_pT_vs_pT_data_fat","DpT_over_pT_vs_pT_data_fat",30,0.,3.0, 200, 0, 2000);

  TH1 *h_Deta_fatlead_data_fat = new TH1F("h_Deta_data_fat","Dphi_data_fat",80,-1.,1.0);
  TH1 *h_Dphi_fatlead_data_fat = new TH1F("h_Dphi_data_fat","Dphi_data_fat",80,-1.,1.0);




  TH1 *h_Nrad_1_fat  = new TH1F("h_Nrad_1_fat","h_Nrad_1_fat", 6, -0.5, 5.5.);
  TH1 *h_Nrad_2_fat  = new TH1F("h_Nrad_2_fat","h_Nrad_2_fat", 6, -0.5, 5.5.);
  TH1 *h_Nrad_1and2_fat  = new TH1F("h_Nrad_1and2_fat","h_Nrad_1and2_fat", 6, -0.5, 5.5.);


  TH2 *h_DEta_Mass_data_fat = new TH2F("h_DEta_Mass_data_fat","DEta_Mass_data_fat", 5, 0., 1.3, 83, xAxis1);
  TH2 *h_DEta_Mass_data_large_fat = new TH2F("h_DEta_Mass_data_large_fat","DEta_Mass_data_large_fat", 10, 0., 2.5, 83, xAxis1);

  Double_t xChi[6] = {1, 1.4, 1.9, 2.40, 3.00, 3.7};

  TH2 *h_Chi_Mass_data_fat = new TH2F("h_Chi_Mass_data_fat","Chi_Mass_data_fat", 5, xChi, 83, xAxis1);
  TH2 *h_Chi_Mass_data_fat_mu10 = new TH2F("h_Chi_Mass_data_fat_mu10","Chi_Mass_data_fat_mu10", 5, xChi, 83, xAxis1);
  TH2 *h_Chi_Mass_data_fat_mu20 = new TH2F("h_Chi_Mass_data_fat_mu20","Chi_Mass_data_fat_mu20", 5, xChi, 83, xAxis1);
  TH2 *h_Chi_Mass_data_fat_mu30 = new TH2F("h_Chi_Mass_data_fat_mu30","Chi_Mass_data_fat_mu30", 5, xChi, 83, xAxis1);
  TH2 *h_Chi_Mass_data_fat_e10 = new TH2F("h_Chi_Mass_data_fat_e10","Chi_Mass_data_fat_e10", 5, xChi, 83, xAxis1);
  TH2 *h_Chi_Mass_data_fat_e20 = new TH2F("h_Chi_Mass_data_fat_e20","Chi_Mass_data_fat_e20", 5, xChi, 83, xAxis1);
  TH2 *h_Chi_Mass_data_fat_e30 = new TH2F("h_Chi_Mass_data_fat_e30","Chi_Mass_data_fat_e30", 5, xChi, 83, xAxis1);


  TH2 *h_Chi_Mass_data_large_fat = new TH2F("h_Chi_Mass_data_large_fat","Chi_Mass_data_large_fat", 10, 1., 11, 83, xAxis1);

  TH1 *h_Chi_data_fat = new TH1F("h_Chi_data_fat","Chi_data_fat",150,1.,3.75);

  TH1 *h_DEta_data_fat = new TH1F("h_DEta_data_fat","DEta_data_fat",15,0.,1.5);
  TH1 *h_DPhi_data_fat = new TH1F("h_DPhi_data_fat","DPhi_data_fat",25,0.0,3.142);
  TH1 *h_Eta_data_fat = new TH1F("h_Eta_data_fat","Eta_data_fat",60,-3.0,3.0);
  TH1 *h_Eta_data_fat_1 = new TH1F("h_Eta_data_fat_1","Eta_data_fat_1",60,-3.0,3.0);
  TH1 *h_Eta_data_fat_2 = new TH1F("h_Eta_data_fat_2","Eta_data_fat_2",60,-3.0,3.0);
  TH1 *h_Phi_data_fat = new TH1F("h_Phi_data_fat","Phi_data_fat",18,-3.142,3.142);
  TH1 *h_Phi_data_fat_1 = new TH1F("h_Phi_data_fat_1","Phi_data_fat_1",18,-3.142,3.142);
  TH1 *h_Phi_data_fat_2 = new TH1F("h_Phi_data_fat_2","Phi_data_fat_2",18,-3.142,3.142);
  TH1 *h_corPt_data_fat = new TH1F("h_corPt_data_fat","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_fat_1 = new TH1F("h_corPt_data_fat_1","corPt_data_fat_1",54,ptBoundaries);
  TH1 *h_corPt_data_fat_2 = new TH1F("h_corPt_data_fat_2","corPt_data_fat_2",54,ptBoundaries);

  TH1 *h_FatDiJet_EventYield = new TH1F("h_FatDiJet_EventYield","h_FatDiJet_EventYield",5500,130000.5,185000.5);
  TH1 *h_PFDiJet_EventYield = new TH1F("h_PFDiJet_EventYield","h_PFDiJet_EventYield",5500,130000.5,185000.5);

  TH1 *h_Cuts_Fat = new TH1F("h_Cuts_Fat","h_Cuts_Fat",5, -0.5, 4.5);
  TH1 *h_Cuts_Fat_Names = new TH1F("h_Cuts_Fat_Names","h_Cuts_Fat_Names",1, 0, 1);

  h_Cuts_Fat_Names->SetBit(TH1::kCanRebin);
  string cutNames[5]={string("data_count"), string("good_vertex_selection"), string("DiFatJet_MassCut"), string("DiFatJet_EtaCuts"), string("LeadingJets_Id")};
  for(unsigned i=0;i<5;i++){
    cout << cutNames[i].c_str() << endl;
    h_Cuts_Fat_Names->Fill(cutNames[i].c_str(),1);
  }


  // data access -------------------------------------------

  QCDEvent *Event_data = new QCDEvent();
  TBranch *branch_data = tr_data_large->GetBranch("events");
  branch_data->SetAddress(&Event_data);

  // # of entries -------------------------------------------

  unsigned NEntries_data = tr_data_large->GetEntries();
  cout<<"Reading TREE: "<<NEntries_data <<" events"<<endl;
  int decade = 0;



  TFile* temp = new TFile(sTmp.c_str(),"RECREATE");
  temp->cd();
  
  //TCut generalCut = "";

  //  TCut generalCut = "( evtHdr().runNo() > 171440 && evtHdr().runNo() < 171460 )";
  TCut generalCut = "(fatmjjcor(0) > 890.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3) || (calomjjcor(0) > 838.0 &&  fabs( (calojet(0)).eta() - (calojet(1)).eta() ) < 1.3)";

  //  TCut generalCut = "(fatmjjcor(0) > 740.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3)";

  //  TCut generalCut = "(  ( fatmjjcor(0) > 500.0 &&  fabs(fatjet(0).eta() - fatjet(1).eta()) < 1.3 ) || ( pfmjjcor(0) > 500.0 &&  fabs( pfjet(0).eta() - pfjet(1).eta()) < 1.3 ) ||  ( calomjjcor(0) > 500.0 &&  fabs(calojet(0).eta() - calojet(1).eta()) < 1.3 ) )";// && ( evtHdr().runNo() < 149443 )";


  // TTree* tr_data = tr_data_large;//->CopyTree(generalCut);
  TTree* tr_data = tr_data_large->CopyTree(generalCut);
  unsigned NEntries_data_SKIMED = tr_data->GetEntries();
  cout<<"Reading TREE: "<< NEntries_data_SKIMED <<" events"<<endl;

  // counters------------------------------------------------

  unsigned data_count(0);
  unsigned vertex1_count(0);
  unsigned vertex2_count(0);
  unsigned runNo_count(0);

  unsigned mass_count(0);
  unsigned eta_count(0);
  unsigned jetID_count(0);

  unsigned pf_mass_count(0);
  unsigned pf_eta_count(0);
  unsigned pf_jetID_count(0);

  unsigned fat_mass_count(0);
  unsigned fat_eta_count(0);
  unsigned fat_jetID_count(0);


  unsigned mc_count(0);


  // fill histograms-----------------------------------------

  for(unsigned i=0; i < NEntries_data_SKIMED;i++) {
    
    if (i%10000 == 0) {
      cout << "Is " << i << " mass is " <<  Event_data->fatmjjcor(0) << endl;
      
    }
    
    

    double progress = 100.0*i/(1.0*NEntries_data_SKIMED);
    int k = TMath::FloorNint(progress); 


    if (k > decade) 
      cout<<k<<" %"<<endl;

    decade = k;          
    
    tr_data->GetEntry(i);

    h_Pthat_data->Fill((Event_data->evtHdr()).pthat());

    //  if ((Event_data->evtHdr().runNo() < 171440 || Event_data->evtHdr().runNo() > 171460)) continue;
 

//    continue;
    /*
    bool light_cut0 =  Event_data->fatmjjcor(0) > 838.0;
    if (!light_cut0) continue;
    bool light_cut1 =  fabs( (Event_data->fatjet(0)).eta() - (Event_data->fatjet(1)).eta() ) < 1.3;
    if (!light_cut1) continue;
    */

    int nVtx = (Event_data->evtHdr()).nVtx();

    bool vertex_cut1 = (Event_data->evtHdr()).PVndof() >= 4;
    bool vertex_cut2 = (Event_data->evtHdr()).PVz() < 24 ;

//    bool jetID_cuts = ((((Event_data->calojet(0)).emf() > 0.01) || (fabs((Event_data->calojet(0)).eta())) > 2.6) && ((Event_data->calojet(0)).n90hits() > 1) && ((Event_data->calojet(0)).fHPD() < 0.98)  ) && ((((Event_data->calojet(1)).emf() > 0.01) || (fabs((Event_data->calojet(1)).eta())) > 2.6) && ((Event_data->calojet(1)).n90hits() > 1) && ((Event_data->calojet(1)).fHPD() < 0.98 ) );
    //bool runNo_cuts = true;
    bool runNo_cuts = (Event_data->evtHdr()).runNo() ==  160000 || (Event_data->evtHdr()).runNo() < 10;

 


    data_count++; 
    if (runNo_cuts) {
      runNo_count++;
      if(vertex_cut1) {
        vertex1_count++;
        if(vertex_cut2) {
          vertex2_count++;

	  
	  bool eta_cuts = fabs((Event_data->calojet(0)).eta()) < 2.5 && fabs((Event_data->calojet(1)).eta()) < 2.5;
	  bool deta_cut = fabs( (Event_data->calojet(0)).eta() - (Event_data->calojet(1)).eta() ) < 1.3;
	  bool corMass_cuts = Event_data->calomjjcor(0) > 838.0;
	  bool jetID_cuts = (Event_data->calojet(0)).looseID() == true && (Event_data->calojet(1)).looseID() == true;
	  

	  double dPhi_fat = (TMath::Pi()-fabs(fabs((Event_data->fatjet(0)).phi()-(Event_data->fatjet(1)).phi())-TMath::Pi()));
	  double dPhi_pf = (TMath::Pi()-fabs(fabs((Event_data->pfjet(0)).phi()-(Event_data->pfjet(1)).phi())-TMath::Pi()));

	  double pfMass = Event_data->pfmjjcor(0);

	  bool pf_eta_cuts = fabs((Event_data->pfjet(0)).eta()) < 2.5 && fabs((Event_data->pfjet(1)).eta()) < 2.5;
	  bool pf_deta_cut = fabs( (Event_data->pfjet(0)).eta() - (Event_data->pfjet(1)).eta() ) < 1.3;
	  bool pf_corMass_cuts = pfMass > 838.0;
	  bool pf_jetID_cuts = (Event_data->pfjet(0)).looseID() == true && (Event_data->pfjet(1)).looseID() == true;
	  bool pf_dPhi_cuts = dPhi_pf > TMath::Pi()/3;


          double fatMassUp = Event_data->fatmjjcor(1);
	  double fatMass = Event_data->fatmjjcor(0);
          double fatMassDo = Event_data->fatmjjcor(-1);

	  double fatEta1 = Event_data->fatjet(0).eta();
	  double fatEta2 = Event_data->fatjet(1).eta();
	  double deltaFatEta = fabs( fatEta1 - fatEta2 );
	

	  bool fat_eta_cuts = fabs(fatEta1) < 2.5 && fabs(fatEta2) < 2.5;
	  bool fat_deta_cut =  deltaFatEta< 1.3;
	  bool fat_corMass_cuts = fatMass > 890.0;
	  bool fat_jetID_cuts = (Event_data->fatjet(0)).looseID() == true && (Event_data->fatjet(1)).looseID() == true;
	  bool fat_dPhi_cuts = dPhi_fat > TMath::Pi()/3;


	  
	  // Calo Jets
          if(eta_cuts) {
            if (deta_cut) {
              eta_count++;
              if(corMass_cuts) {
                mass_count++;
                if(jetID_cuts) {
                  jetID_count++;

		  //		  for (int iMass = 0; iMass < 6; iMass++){
		    //		    h_DijetMass_data_MassShape->Fill(Event_data->calomjjcor(0)/ResonanceMass[iMass], iMass);
		  //		  }

		  h_DijetMass_data_up->Fill(Event_data->calomjjcor(1));
                  h_DijetMass_data->Fill(Event_data->calomjjcor(0));
                  h_DijetMass_data_do->Fill(Event_data->calomjjcor(-1));

                  if (nVtx==1) h_DijetMass_MI_nPVe1_data->Fill(Event_data->calomjjcor(0));
                  else h_DijetMass_MI_nPVg1_data->Fill(Event_data->calomjjcor(0));
                  h_Eta_Phi_Scatter_data->Fill((Event_data->calojet(0)).eta(),(Event_data->calojet(0)).phi());
                  h_Eta_Phi_Scatter_data->Fill((Event_data->calojet(1)).eta(),(Event_data->calojet(1)).phi());
                  h_Eta_Phi_Scatter_data_1->Fill((Event_data->calojet(0)).eta(),(Event_data->calojet(0)).phi());
                  h_Eta_Phi_Scatter_data_2->Fill((Event_data->calojet(1)).eta(),(Event_data->calojet(1)).phi());
                  h_MET_over_sumEt_data->Fill((Event_data->calomet()).met_o_sumet());
                  h_EMF_data->Fill((Event_data->calojet(0)).emf());
                  h_EMF_data->Fill((Event_data->calojet(1)).emf());
                  h_DPhi_data->Fill(3.14159265358979323846-fabs(fabs((Event_data->calojet(0)).phi()-(Event_data->calojet(1)).phi())-3.14159265358979323846));
                  h_Eta_data_1->Fill((Event_data->calojet(0)).eta());
                  h_Eta_data_2->Fill((Event_data->calojet(1)).eta());
                  h_DEta_data->Fill(fabs((Event_data->calojet(0)).eta()-(Event_data->calojet(1)).eta()));
                  h_Phi_data_1->Fill((Event_data->calojet(0)).phi());
                  h_Phi_data_2->Fill((Event_data->calojet(1)).phi());
                  h_n90hits_data->Fill((Event_data->calojet(0)).n90hits());
                  h_n90hits_data->Fill((Event_data->calojet(1)).n90hits());
                  h_fHPD_data->Fill((Event_data->calojet(0)).fHPD());
                  h_fHPD_data->Fill((Event_data->calojet(1)).fHPD());
                  h_corPt_data_1->Fill((Event_data->calojet(0)).ptCor());
                  h_corPt_data_2->Fill((Event_data->calojet(1)).ptCor());
                  h_nTrkVtx_data->Fill((Event_data->calojet(0)).nTrkVtx());
                  h_nTrkVtx_data->Fill((Event_data->calojet(1)).nTrkVtx());
                  h_nTrkCalo_data->Fill((Event_data->calojet(0)).nTrkCalo());
                  h_nTrkCalo_data->Fill((Event_data->calojet(1)).nTrkCalo());  
                          
                  //            h_JCFCalo_data->Fill(jetVar_data[0].chf);
                  //            h_JCFCalo_data->Fill(jetVar_data[1].chf);
		  
                }
              }
            }
          }
	  


	  //PF jets
          if(pf_eta_cuts) {
            if (pf_deta_cut) {
              pf_eta_count++;
              if(pf_corMass_cuts) {
                pf_mass_count++;
                if(pf_jetID_cuts && pf_dPhi_cuts/* && Event_data->pfmet().met_o_sumet()>0.35*//* && (Event_data->pfjet(0).elf() > 0.96 && Event_data->pfjet(1).elf() > 0.96)*/) {
                  pf_jetID_count++;
  
		  //		  for (int iMass = 0; iMass < 6; iMass++)
		    //		    h_DijetMass_data_MassShape_pf->Fill(pfMass/ResonanceMass[iMass], iMass);
		  
		  //	  cout << "Run =  " << Event_data->evtHdr().runNo() << "\t" << Event_data->evtHdr().lumi() << "\t" << Event_data->evtHdr().event() << endl;

		  h_nVtx_pf->Fill(nVtx);

		  if (nVtx == 1) h_DijetMass_MI_nPVl1_data_pf->Fill(pfMass);
		  if (nVtx < 4) h_DijetMass_MI_nPVl3_data_pf->Fill(pfMass);
		  if (nVtx < 7) h_DijetMass_MI_nPVl6_data_pf->Fill(pfMass);
		  if (nVtx  > 6) h_DijetMass_MI_nPVg6_data_pf->Fill(pfMass);
		  if (nVtx > 10) h_DijetMass_MI_nPVg10_data_pf->Fill(pfMass);

		  h_nVtx_DijetMass_pf->Fill(nVtx, pfMass);

		  h_DijetMass_data_pf_up->Fill(Event_data->pfmjjcor(1));
		  h_DijetMass_data_pf->Fill(Event_data->pfmjjcor(0));
		  h_DijetMass_data_pf_do->Fill(Event_data->pfmjjcor(-1));


                  h_DPhi_data_pf->Fill(3.14159265358979323846-fabs(fabs((Event_data->pfjet(0)).phi()-(Event_data->pfjet(1)).phi())-3.14159265358979323846));
                  h_Eta_data_pf_1->Fill((Event_data->pfjet(0)).eta());
                  h_Eta_data_pf_2->Fill((Event_data->pfjet(1)).eta());
                  h_DEta_data_pf->Fill(fabs((Event_data->pfjet(0)).eta()-(Event_data->pfjet(1)).eta()));
                  h_Phi_data_pf_1->Fill((Event_data->pfjet(0)).phi());
                  h_Phi_data_pf_2->Fill((Event_data->pfjet(1)).phi());
                  h_corPt_data_pf_1->Fill((Event_data->pfjet(0)).ptCor());
                  h_corPt_data_pf_2->Fill((Event_data->pfjet(1)).ptCor()); 
		  h_PFDiJet_EventYield->Fill((Event_data->evtHdr()).runNo());

		  for (int iCh = 0; iCh < 4; iCh++){
	
		    double frac = 0;
		    switch(iCh){
		    case 0: frac = Event_data->pfjet(0).chf(); break;//p_RvsEta->Fill(
		    case 1: frac = Event_data->pfjet(0).nhf(); break;//p_RvsEta->Fill(
		    case 2: frac = Event_data->pfjet(0).phf(); break;//p_RvsEta->Fill(
		    case 3: frac = Event_data->pfjet(0).elf(); break;//p_RvsEta->Fill(
		    default: break;
		    }

		    /*		    
		    p_RvsEta_data_pf->Fill(Event_data->pfjet(0).eta(), iCh, frac);
		    p_RvsPt_barrel_data_pf->Fill(Event_data->pfjet(0).ptCor(), iCh, frac);
		    p_RvsPt_endcaps_data_pf->Fill(Event_data->pfjet(0).ptCor(), iCh, frac);
		    p_RvsPhi_barrel_data_pf->Fill(Event_data->pfjet(0).phi(), iCh, frac);
		    p_RvsPhi_endcaps_data_pf->Fill(Event_data->pfjet(0).phi(), iCh, frac);

		    p_RvsEta_data_pf->Fill(Event_data->pfjet(1).eta(), iCh, frac);
		    p_RvsPt_barrel_data_pf->Fill(Event_data->pfjet(1).ptCor(), iCh, frac);
		    p_RvsPt_endcaps_data_pf->Fill(Event_data->pfjet(1).ptCor(), iCh, frac);
		    p_RvsPhi_barrel_data_pf->Fill(Event_data->pfjet(1).phi(), iCh, frac);
		    p_RvsPhi_endcaps_data_pf->Fill(Event_data->pfjet(1).phi(), iCh, frac);
		    */

		  }

		  h_fCh_data_pf_1->Fill((Event_data->pfjet(0)).chf());
		  h_fNh_data_pf_1->Fill((Event_data->pfjet(0)).nhf());
		  h_fPh_data_pf_1->Fill((Event_data->pfjet(0)).phf());
		  h_fEl_data_pf_1->Fill((Event_data->pfjet(0)).elf());
		  h_fMu_data_pf_1->Fill((Event_data->pfjet(0)).muf());

		  h_fCh_data_pf_2->Fill((Event_data->pfjet(1)).chf());
		  h_fNh_data_pf_2->Fill((Event_data->pfjet(1)).nhf());
		  h_fPh_data_pf_2->Fill((Event_data->pfjet(1)).phf());
		  h_fEl_data_pf_2->Fill((Event_data->pfjet(1)).elf());
		  h_fMu_data_pf_2->Fill((Event_data->pfjet(1)).muf());

		  h_nVtx_fCh_pf->Fill(nVtx, (Event_data->pfjet(0)).chf());
		  h_nVtx_fNh_pf->Fill(nVtx, (Event_data->pfjet(0)).nhf());
		  h_nVtx_fPh_pf->Fill(nVtx, (Event_data->pfjet(0)).phf());
		  h_nVtx_fEl_pf->Fill(nVtx, (Event_data->pfjet(0)).elf());
		  h_nVtx_fMu_pf->Fill(nVtx, (Event_data->pfjet(0)).muf());


		  h_nVtx_fCh_pf->Fill(nVtx, (Event_data->pfjet(1)).chf());
		  h_nVtx_fNh_pf->Fill(nVtx, (Event_data->pfjet(1)).nhf());
		  h_nVtx_fPh_pf->Fill(nVtx, (Event_data->pfjet(1)).phf());
		  h_nVtx_fEl_pf->Fill(nVtx, (Event_data->pfjet(1)).elf());
		  h_nVtx_fMu_pf->Fill(nVtx, (Event_data->pfjet(1)).muf());


		  for(unsigned itrig=0;itrig<Event_data->nTriggers();itrig++){
		    //		      cout << "itrig = " << itrig << " fired = " << Event_data->fired(itrig) << endl;
		    if (Event_data->fired(itrig) > 0)
		      h_DijetMass_Trigger_data_pf->Fill(pfMass, itrig);
		    
		    for(unsigned jtrig=0;jtrig<Event_data->nTriggers();jtrig++){
		      
		      if (Event_data->fired(itrig) > 0 && Event_data->fired(jtrig) > 0){
			h_DijetMass_Trigger_data_pf_Corr->Fill(pfMass, itrig, jtrig);
			if (Event_data->fired(45) > 0)
			  h_DijetMass_Trigger_data_pf_Corr_andHT600->Fill(pfMass, itrig, jtrig);
			if (Event_data->fired(46) > 0)
			  h_DijetMass_Trigger_data_pf_Corr_andHT650->Fill(pfMass, itrig, jtrig);
			if (Event_data->fired(51) > 0)
			  h_DijetMass_Trigger_data_pf_Corr_andHT700->Fill(pfMass, itrig, jtrig);
			if (Event_data->fired(52) > 0)
			  h_DijetMass_Trigger_data_pf_Corr_andHT750->Fill(pfMass, itrig, jtrig);
		      }
			      
		    }
		  }


		}
              }
            }
           }

	 
	  if(fat_corMass_cuts) {
	      fat_mass_count++;
	      if(fat_eta_cuts) {

		h_DEta_Mass_data_large_fat->Fill(fabs(deltaFatEta), fatMass);
		h_Chi_Mass_data_large_fat->Fill(exp(fabs(deltaFatEta)), fatMass);

		if (fat_deta_cut) {
		  fat_eta_count++;
		  if(fat_jetID_cuts && fat_dPhi_cuts/* && Event_data->pfmet().met_o_sumet()>0.35*//*&& (Event_data->pfjet(0).elf() > 0.96 && Event_data->pfjet(1).elf() > 0.96)*/) {
		    h_MET_over_sumEt_fat_data->Fill((Event_data->pfmet()).met_o_sumet());
                     
		    fat_jetID_count++;

		    //		    for (int iMass = 0; iMass < 6; iMass++)
		    //		      h_DijetMass_data_MassShape_fat->Fill(fatMass/ResonanceMass[iMass], iMass);

		    h_nVtx_fat->Fill(nVtx);

		    if (nVtx == 1) h_DijetMass_MI_nPVl1_data_fat->Fill(fatMass);
		    if (nVtx < 4) h_DijetMass_MI_nPVl3_data_fat->Fill(fatMass);
		    if (nVtx < 7) h_DijetMass_MI_nPVl6_data_fat->Fill(fatMass);
		    if (nVtx  > 6) h_DijetMass_MI_nPVg6_data_fat->Fill(fatMass);
		    if (nVtx > 10) h_DijetMass_MI_nPVg10_data_fat->Fill(fatMass);

		    h_nVtx_DijetMass_fat->Fill(nVtx, fatMass);

		    h_DijetMass_data_fat_up->Fill(fatMassUp);
		    h_DijetMass_data_fat->Fill(fatMass);
		    h_DijetMass_data_fat_do->Fill(fatMassDo);

		    h_DPhi_data_fat->Fill(dPhi_fat);
		    h_Eta_data_fat_1->Fill(fatEta1);
		    h_Eta_data_fat_2->Fill(fatEta2);
		    h_DEta_data_fat->Fill(deltaFatEta);
		    h_Phi_data_fat_1->Fill((Event_data->fatjet(0)).phi());
		    h_Phi_data_fat_2->Fill((Event_data->fatjet(1)).phi());
		    h_corPt_data_fat_1->Fill((Event_data->fatjet(0)).ptCor());
		    h_corPt_data_fat_2->Fill((Event_data->fatjet(1)).ptCor()); 
		    h_FatDiJet_EventYield->Fill((Event_data->evtHdr()).runNo());

		    h_DEta_Mass_data_fat->Fill(fabs(deltaFatEta), fatMass);
		    h_Chi_Mass_data_fat->Fill(exp(fabs(deltaFatEta)), fatMass);
		    h_Chi_data_fat->Fill(exp(fabs(deltaFatEta)));
		  

		    for(unsigned itrig=0;itrig<Event_data->nTriggers();itrig++){
		      //		      cout << "itrig = " << itrig << " fired = " << Event_data->fired(itrig) << endl;
		      if (Event_data->fired(itrig) > 0)
			  h_DijetMass_Trigger_data_fat->Fill(fatMass, itrig);
		    
			for(unsigned jtrig=0;jtrig<Event_data->nTriggers();jtrig++){

			  if (Event_data->fired(itrig) > 0 && Event_data->fired(jtrig) > 0){
			    h_DijetMass_Trigger_data_fat_Corr->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(45) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT600->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(46) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT650->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(51) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT700->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(52) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT750->Fill(fatMass, itrig, jtrig);
			  }
			      
      			}
		    }

		    
		    double muonMomentum1 = Event_data->pfjet(0).pt()*Event_data->pfjet(0).muf();
		    double muonMomentum2 = Event_data->pfjet(1).pt()*Event_data->pfjet(1).muf();

		    if (muonMomentum1 > 30 || muonMomentum2 > 30)  {
		      h_DijetMass_data_fat_mu30_up->Fill(fatMassUp);
		      h_DijetMass_data_fat_mu30->Fill(fatMass);
		      h_DijetMass_data_fat_mu30_do->Fill(fatMassDo);

		      h_Chi_Mass_data_fat_mu30->Fill(exp(fabs(deltaFatEta)), fatMass);
		    }
		    if (muonMomentum1 > 20 || muonMomentum2 > 20)  {
		      h_DijetMass_data_fat_mu20_up->Fill(fatMassUp);
		      h_DijetMass_data_fat_mu20->Fill(fatMass);
		      h_DijetMass_data_fat_mu20_up->Fill(fatMassDo);

		      h_Chi_Mass_data_fat_mu20->Fill(exp(fabs(deltaFatEta)), fatMass);
		    }
		    if (muonMomentum1 > 10 || muonMomentum2 > 10)  {
		      h_DijetMass_data_fat_mu10_up->Fill(fatMassUp);
		      h_DijetMass_data_fat_mu10->Fill(fatMass);
		      h_DijetMass_data_fat_mu10_do->Fill(fatMassDo);

		      h_Chi_Mass_data_fat_mu10->Fill(exp(fabs(deltaFatEta)), fatMass);

		      for(unsigned itrig=0;itrig<Event_data->nTriggers();itrig++){
			//		      cout << "itrig = " << itrig << " fired = " << Event_data->fired(itrig) << endl;
			if (Event_data->fired(itrig) > 0)
			  h_DijetMass_Trigger_data_fat_mu10->Fill(fatMass, itrig);
			
			for(unsigned jtrig=0;jtrig<Event_data->nTriggers();jtrig++){
			  
			  if (Event_data->fired(itrig) > 0 && Event_data->fired(jtrig) > 0){
			    h_DijetMass_Trigger_data_fat_Corr_mu10->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(45) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT600_mu10->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(46) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT650_mu10->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(51) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT700_mu10->Fill(fatMass, itrig, jtrig);
			    if (Event_data->fired(52) > 0)
			      h_DijetMass_Trigger_data_fat_Corr_andHT750_mu10->Fill(fatMass, itrig, jtrig);
			  }
			  
      			}
		      }


		      //		      for (int iMass = 0; iMass < 6; iMass++){
		      //			h_DijetMass_data_MassShape_mu10_fat->Fill(fatMass/ResonanceMass[iMass], iMass);
		      //		      }

		    }

		    double electronMomentum1 = Event_data->pfjet(0).pt()*Event_data->pfjet(0).elf();
		    double electronMomentum2 = Event_data->pfjet(1).pt()*Event_data->pfjet(1).elf();

		    if (electronMomentum1 > 30 || electronMomentum2 > 30)  {
		      h_DijetMass_data_fat_e30->Fill(fatMass);
		      //		      h_Chi_Mass_data_fat_e30->Fill(exp(fabs(deltaFatEta)), fatMass);
		    }
		    if (electronMomentum1 > 20 || electronMomentum2 > 20)  {
		      h_DijetMass_data_fat_e20->Fill(fatMass);
		      //		      h_Chi_Mass_data_fat_e20->Fill(exp(fabs(deltaFatEta)), fatMass);
		    }
		    if (electronMomentum1 > 10 || electronMomentum2 > 10)  {
		      h_DijetMass_data_fat_e10->Fill(fatMass);
		      //		      h_Chi_Mass_data_fat_e10->Fill(exp(fabs(deltaFatEta)), fatMass);
		    }




		    if ((TMath::Pi()-fabs(fabs((Event_data->fatjet(0)).phi()-(Event_data->pfjet(0)).phi())-TMath::Pi())) < 1.5 ){

		      double DpT1_over_pT = (Event_data->fatjet(0).ptCor() - Event_data->pfjet(0).ptCor())/Event_data->fatjet(0).ptCor();
		      double DpT2_over_pT = (Event_data->fatjet(1).ptCor() - Event_data->pfjet(1).ptCor())/Event_data->fatjet(1).ptCor();
		      
		      h_DpT_over_pT_data_fat_1->Fill(DpT1_over_pT);
		      h_DpT_over_pT_data_fat_1->Fill(DpT2_over_pT);
		      
		      double DpT1 = (Event_data->fatjet(0).ptCor() - Event_data->pfjet(0).ptCor());
		      double DpT2 = (Event_data->fatjet(1).ptCor() - Event_data->pfjet(1).ptCor());
		      
		      h_DpT_data_fat_1->Fill(DpT1);
		      h_DpT_data_fat_2->Fill(DpT2);

		      h_DpT_over_pT_vs_pT_data_fat_1->Fill(DpT1_over_pT, Event_data->pfjet(0).ptCor());
		      h_DpT_over_pT_vs_pT_data_fat_2->Fill(DpT2_over_pT, Event_data->pfjet(1).ptCor());


		      h_Deta_fatlead_data_fat_1->Fill(Event_data->fatjet(0).eta() - Event_data->pfjet(0).eta());
		      h_Dphi_fatlead_data_fat_1->Fill(Event_data->fatjet(0).phi() - Event_data->pfjet(0).phi());

		      h_Deta_fatlead_data_fat_2->Fill(Event_data->fatjet(1).eta() - Event_data->pfjet(1).eta());
		      h_Dphi_fatlead_data_fat_2->Fill(Event_data->fatjet(1).phi() - Event_data->pfjet(1).phi());

		    }

		    
		    reco::Particle::LorentzVector cand1 = Event_data->pfjet(0).p4();
		    reco::Particle::LorentzVector cand2 = Event_data->pfjet(1).p4();

		    int N1 = 0, N2 = 0, N1N2 = 0;

		    double rmax = 1.1;
		    for(unsigned j = 2; j<Event_data->nPFJets(); j++) {
		      reco::Particle::LorentzVector cand = Event_data->pfjet(j).p4();

		      double dR1 = deltaR(cand1.eta(),cand.eta(),cand1.phi(),cand.phi());
		      double dR2 = deltaR(cand2.eta(),cand.eta(),cand2.phi(),cand.phi());
		      if (dR1 < dR2 && dR1 < rmax) 
			N1++;
		      if (dR1 > dR2 && dR2 < rmax)
			N2++;
		      if (dR1 < rmax && dR2 < rmax)
			N1N2++;
		      
		    }


		    h_Nrad_1_fat->Fill(N1);
		    h_Nrad_2_fat->Fill(N2);
		    h_Nrad_1and2_fat->Fill(N1N2);

		  
		}
	      }
	    }
	  }




        }
      }
    }

  }

  h_Cuts_Fat->SetBinContent(1, data_count);
  h_Cuts_Fat->SetBinContent(2, vertex2_count);
  h_Cuts_Fat->SetBinContent(3, fat_mass_count);
  h_Cuts_Fat->SetBinContent(4, fat_eta_count);
  h_Cuts_Fat->SetBinContent(5, fat_jetID_count);



  // ---------- Calo ---------
  h_Eta_data->Add(h_Eta_data_1);
  h_Eta_data->Add(h_Eta_data_2);
  h_Phi_data->Add(h_Phi_data_1);
  h_Phi_data->Add(h_Phi_data_2);
  h_corPt_data->Add(h_corPt_data_1);
  h_corPt_data->Add(h_corPt_data_2);

  // ----------  PF  ---------
  h_Eta_data_pf->Add(h_Eta_data_pf_1);
  h_Eta_data_pf->Add(h_Eta_data_pf_2);
  h_Phi_data_pf->Add(h_Phi_data_pf_1);
  h_Phi_data_pf->Add(h_Phi_data_pf_2);
  h_corPt_data_pf->Add(h_corPt_data_pf_1);
  h_corPt_data_pf->Add(h_corPt_data_pf_2);
 
  h_fCh_data_pf->Add(h_fCh_data_pf_1);
  h_fCh_data_pf->Add(h_fCh_data_pf_2);
  h_fNh_data_pf->Add(h_fNh_data_pf_1);
  h_fNh_data_pf->Add(h_fNh_data_pf_2);
  h_fPh_data_pf->Add(h_fPh_data_pf_1);
  h_fPh_data_pf->Add(h_fPh_data_pf_2);
  h_fEl_data_pf->Add(h_fEl_data_pf_1);
  h_fEl_data_pf->Add(h_fEl_data_pf_2);
  h_fMu_data_pf->Add(h_fMu_data_pf_1);
  h_fMu_data_pf->Add(h_fMu_data_pf_2);

  // ----------  FAT  ---------
  h_Eta_data_fat->Add(h_Eta_data_fat_1);
  h_Eta_data_fat->Add(h_Eta_data_fat_2);
  h_Phi_data_fat->Add(h_Phi_data_fat_1);
  h_Phi_data_fat->Add(h_Phi_data_fat_2);
  h_corPt_data_fat->Add(h_corPt_data_fat_1);
  h_corPt_data_fat->Add(h_corPt_data_fat_2);
 

  h_DpT_over_pT_data_fat->Add(h_DpT_over_pT_data_fat_1);
  h_DpT_data_fat->Add(h_DpT_data_fat_1);
  h_DpT_over_pT_vs_pT_data_fat->Add(h_DpT_over_pT_vs_pT_data_fat_1);
  h_Deta_fatlead_data_fat->Add(h_Deta_fatlead_data_fat_1);
  h_Dphi_fatlead_data_fat->Add(h_Dphi_fatlead_data_fat_1);

  h_DpT_over_pT_data_fat->Add(h_DpT_over_pT_data_fat_2);
  h_DpT_data_fat->Add(h_DpT_data_fat_2);
  h_DpT_over_pT_vs_pT_data_fat->Add(h_DpT_over_pT_vs_pT_data_fat_2);
  h_Deta_fatlead_data_fat->Add(h_Deta_fatlead_data_fat_2);
  h_Dphi_fatlead_data_fat->Add(h_Dphi_fatlead_data_fat_2);

  h_Nrad_1_fat->Write();
  h_Nrad_2_fat->Write();
  h_Nrad_1and2_fat->Write();

  std::cout << "# of events = " << data_count << std::endl;
  std::cout << "# of events after runNo cut = " << runNo_count << std::endl;
  std::cout << "# of events after vertex1 cut = " << vertex1_count << std::endl;
  std::cout << "# of events after vertex2 cut = " << vertex2_count << std::endl;

  std::cout << "# of events after mass cut(calo) = " << mass_count << std::endl;
  std::cout << "# of events after eta cut(calo) = " << eta_count << std::endl;
  std::cout << "# of events after all cut(calo) = " << jetID_count << std::endl;

  std::cout << "# of events after mass cut(pf) = " << pf_mass_count << std::endl;
  std::cout << "# of events after eta cut(pf) = " << pf_eta_count << std::endl;
  std::cout << "# of events after all cut(pf) = " << pf_jetID_count << std::endl;

  std::cout << "# of events after mass cut(fat) = " << fat_mass_count << std::endl;
  std::cout << "# of events after eta cut(fat) = " << fat_eta_count << std::endl;
  std::cout << "# of events after all cut(fat) = " << fat_jetID_count << std::endl;

  outf->cd();


  // -------- GEN INFO -------

  h_Pthat_data->Write();

  
  // --------- Calo --------

  //  h_DijetMass_data->Write();

  h_DijetMass_data_up->Write();
  h_DijetMass_data->Write();
  h_DijetMass_data_do->Write();  

  h_DijetMass_MI_nPVe1_data->Write();
  h_DijetMass_MI_nPVg1_data->Write();
  h_Eta_Phi_Scatter_data->Write();
  h_Eta_Phi_Scatter_data_1->Write();
  h_Eta_Phi_Scatter_data_2->Write();
  h_MET_over_sumEt_data->Write();
  h_EMF_data->Write();
  h_DPhi_data->Write();
  h_Eta_data->Write();
  h_Eta_data_1->Write();
  h_Eta_data_2->Write();
  h_DEta_data->Write();
  h_Phi_data->Write();
  h_Phi_data_1->Write();
  h_Phi_data_2->Write();
  h_n90hits_data->Write();
  h_fHPD_data->Write();
  h_corPt_data->Write();
  h_corPt_data_1->Write();
  h_corPt_data_2->Write();
  h_nTrkVtx_data->Write();
  h_nTrkCalo_data->Write();
  h_nTrkVtx_data->Write();


  // --------- PF --------

  h_DijetMass_data_pf_up->Write();
  h_DijetMass_data_pf->Write();
  h_DijetMass_data_pf_do->Write();

  h_DEta_data_pf->Write();
  h_DPhi_data_pf->Write();

  h_Phi_data_pf->Write();
  h_Phi_data_pf_1->Write();
  h_Phi_data_pf_2->Write();

  h_Eta_data_pf->Write();
  h_Eta_data_pf_1->Write();
  h_Eta_data_pf_2->Write();
  h_corPt_data_pf->Write();
  h_corPt_data_pf_1->Write();
  h_corPt_data_pf_2->Write();

  h_fCh_data_pf->Write();
  h_fNh_data_pf->Write();
  h_fPh_data_pf->Write();
  h_fEl_data_pf->Write();
  h_fMu_data_pf->Write();
  h_fCh_data_pf_1->Write();
  h_fNh_data_pf_1->Write();
  h_fPh_data_pf_1->Write();
  h_fEl_data_pf_1->Write();
  h_fMu_data_pf_1->Write();
  h_fCh_data_pf_2->Write();
  h_fNh_data_pf_2->Write();
  h_fPh_data_pf_2->Write();
  h_fEl_data_pf_2->Write();
  h_fMu_data_pf_2->Write();

  p_RvsEta_data_pf->Write();
  p_RvsPt_barrel_data_pf->Write();
  p_RvsPt_endcaps_data_pf->Write();
  p_RvsPhi_barrel_data_pf->Write();
  p_RvsPhi_endcaps_data_pf->Write();
  h_PFDiJet_EventYield->Write();

 // --------- Fat -------

  h_DijetMass_data_fat->Write();
  h_DijetMass_data_fat_mu30->Write();
  h_DijetMass_data_fat_mu20->Write();
  h_DijetMass_data_fat_mu10->Write();
  h_DijetMass_data_fat_e30->Write();
  h_DijetMass_data_fat_e20->Write();
  h_DijetMass_data_fat_e10->Write();

  h_DijetMass_data_fat_up->Write();
  h_DijetMass_data_fat_mu30_up->Write();
  h_DijetMass_data_fat_mu20_up->Write();
  h_DijetMass_data_fat_mu10_up->Write();

  h_DijetMass_data_fat_do->Write();
  h_DijetMass_data_fat_mu30_do->Write();
  h_DijetMass_data_fat_mu20_do->Write();
  h_DijetMass_data_fat_mu10_do->Write();




  h_DEta_data_fat->Write();
  h_DPhi_data_fat->Write();

  h_Phi_data_fat->Write();
  h_Phi_data_fat_1->Write();
  h_Phi_data_fat_2->Write();

  h_Eta_data_fat->Write();
  h_Eta_data_fat_1->Write();
  h_Eta_data_fat_2->Write();
  h_corPt_data_fat->Write();
  h_corPt_data_fat_1->Write();
  h_corPt_data_fat_2->Write();
  
  h_FatDiJet_EventYield->Write();
  h_Cuts_Fat->Write();
  h_Cuts_Fat_Names->Write();
  h_MET_over_sumEt_fat_data->Write();


  h_DijetMass_MI_nPVl1_data_pf->Write();
  h_DijetMass_MI_nPVl3_data_pf->Write();
  h_DijetMass_MI_nPVl6_data_pf->Write();
  h_DijetMass_MI_nPVg6_data_pf->Write();
  h_DijetMass_MI_nPVg10_data_pf->Write();

  h_nVtx_pf->Write();


  h_nVtx_DijetMass_pf->Write();
  h_nVtx_DijetMass_fat->Write();

  h_nVtx_fCh_pf->Write();
  h_nVtx_fNh_pf->Write();
  h_nVtx_fPh_pf->Write();
  h_nVtx_fEl_pf->Write();
  h_nVtx_fMu_pf->Write();


  h_DijetMass_MI_nPVl1_data_fat->Write();
  h_DijetMass_MI_nPVl3_data_fat->Write();
  h_DijetMass_MI_nPVl6_data_fat->Write();
  h_DijetMass_MI_nPVg6_data_fat->Write();
  h_DijetMass_MI_nPVg10_data_fat->Write();




  h_DpT_over_pT_data_fat_1->Write();
  h_DpT_data_fat_1->Write();
  h_DpT_over_pT_vs_pT_data_fat_1->Write();

  h_Deta_fatlead_data_fat_1->Write();
  h_Dphi_fatlead_data_fat_1->Write();


  h_DpT_over_pT_data_fat_2->Write();
  h_DpT_data_fat_2->Write();
  h_DpT_over_pT_vs_pT_data_fat_2->Write();

  h_Deta_fatlead_data_fat_2->Write();
  h_Dphi_fatlead_data_fat_2->Write();



  h_DpT_over_pT_data_fat->Write();
  h_DpT_data_fat->Write();
  h_DpT_over_pT_vs_pT_data_fat->Write();

  h_Deta_fatlead_data_fat->Write();
  h_Dphi_fatlead_data_fat->Write();

  h_Nrad_1_fat->Write();
  h_Nrad_2_fat->Write();
  h_Nrad_1and2_fat->Write();

  h_DEta_Mass_data_fat->Write();
  h_DEta_Mass_data_large_fat->Write();


  h_Chi_Mass_data_fat->Write();
  h_Chi_Mass_data_fat_mu30->Write();
  h_Chi_Mass_data_fat_mu20->Write();
  h_Chi_Mass_data_fat_mu10->Write();
  h_Chi_Mass_data_fat_e30->Write();
  h_Chi_Mass_data_fat_e20->Write();
  h_Chi_Mass_data_fat_e10->Write();

  h_Chi_Mass_data_large_fat->Write();
  h_Chi_data_fat->Write();


  h_DijetMass_data_MassShape->Write();
  h_DijetMass_data_MassShape_pf->Write();
  h_DijetMass_data_MassShape_fat->Write();
  h_DijetMass_data_MassShape_mu10_fat->Write();


  h_DijetMass_Trigger_data_pf->Write();
  h_DijetMass_Trigger_data_pf_Corr->Write();
  h_DijetMass_Trigger_data_pf_Corr_andHT600->Write();
  h_DijetMass_Trigger_data_pf_Corr_andHT650->Write();
  h_DijetMass_Trigger_data_pf_Corr_andHT700->Write();
  h_DijetMass_Trigger_data_pf_Corr_andHT750->Write();

  h_DijetMass_Trigger_data_fat->Write();
  h_DijetMass_Trigger_data_fat_Corr->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT600->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT650->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT700->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT750->Write();

  h_DijetMass_Trigger_data_fat_mu10->Write();
  h_DijetMass_Trigger_data_fat_Corr_mu10->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT600_mu10->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT650_mu10->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT700_mu10->Write();
  h_DijetMass_Trigger_data_fat_Corr_andHT750_mu10->Write();

  h_nVtx_fat->Write();

  cout << "Written" << endl;

  //  delete branch_data;
  // delete tr_data;
  //  delete Event_data;

  outf->Close();
  temp->Close();

  //  inf_data->Close();


}




double deltaPhi2(double phi1, double phi2) { 
     double result = phi1 - phi2;
     while (result > double(M_PI)) result -= double(2*M_PI);
     while (result <= -double(M_PI)) result += double(2*M_PI);
     return result*result;
   }


double deltaEta2(double eta1, double eta2) { 
     double result = eta1 - eta2;
     return result*result;
   }

double deltaR(double eta1, double eta2, double phi1, double phi2 ){
  return sqrt( deltaEta2(eta1, eta2) + deltaPhi2(phi1, phi2) );

}
