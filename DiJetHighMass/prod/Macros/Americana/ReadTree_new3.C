#include "TMath.h"


void ReadTree_new3()
{
  // style definition -------------------------------

  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  // Canvas definition ----------------------------------

  TCanvas *c_DijetMass = new TCanvas("c_DijetMass","c_DijetMass",459,358,600,602);
  TCanvas *c_DijetMass_fat = new TCanvas("c_DijetMass_fat","c_DijetMass_fat",459,358,600,602);
  TCanvas *c_DijetMass_log = new TCanvas("c_DijetMass_log","c_DijetMass_log",459,358,600,602);
  TCanvas *c_DijetMass_fat_log = new TCanvas("c_DijetMass_fat_log","c_DijetMass_fat_log",459,358,600,602);
  TCanvas *c_DijetMass_ratio = new TCanvas("c_DijetMass_ratio","c_DijetMass_ratio",459,358,600,602);
  TCanvas *c_DijetMass_ratio_fat = new TCanvas("c_DijetMass_ratio_fat","c_DijetMass_ratio_fat",459,358,600,602);
  TCanvas *c_DijetMass_ratio_log = new TCanvas("c_DijetMass_ratio_log","c_DijetMass_ratio_log",459,358,600,602);
  TCanvas *c_DijetMass_ratio_fat_log = new TCanvas("c_DijetMass_ratio_fat_log","c_DijetMass_ratio_fat_log",459,358,600,602);
  TCanvas *c_DijetMass_MI = new TCanvas("c_DijetMass_MI","c_DijetMass_MI",459,358,600,602);
  TCanvas *c_DijetMass_MI_log = new TCanvas("c_DijetMass_MI_log","c_DijetMass_MI_log",459,358,600,602);
  TCanvas *c_DijetMass_normalized_MI = new TCanvas("c_DijetMass_normalized_MI","c_DijetMass_normalized_MI",459,358,600,602);
  TCanvas *c_DijetMass_normalized_MI_log = new TCanvas("c_DijetMass_normalized_MI_log","c_DijetMass_normalized_MI_log",459,358,600,602);
  TCanvas *c_Eta_Phi_Scatter = new TCanvas("c_Eta_Phi_Scatter","c_Eta_Phi_Scatter",459,378,600,602);
  TCanvas *c_Eta_Phi_Scatter_1 = new TCanvas("c_Eta_Phi_Scatter_1","c_Eta_Phi_Scatter_1",459,378,600,602);
  TCanvas *c_Eta_Phi_Scatter_2 = new TCanvas("c_Eta_Phi_Scatter_2","c_Eta_Phi_Scatter_2",459,378,600,602);
  TCanvas *c_MET_over_sumEt = new TCanvas("c_MET_over_sumEt","c_MET_over_sumEt",459,378,600,602);
  TCanvas *c_MET_over_sumEt_log = new TCanvas("c_MET_over_sumEt_log","c_MET_over_sumEt_log",459,378,600,602);
  TCanvas *c_EMF = new TCanvas("c_EMF","c_EMF",459,378,600,602);
  TCanvas *c_fCh = new TCanvas("c_fCh","c_fCh",459,378,600,602);
  TCanvas *c_fPh = new TCanvas("c_fPh","c_fPh",459,378,600,602);
  TCanvas *c_fNh = new TCanvas("c_fNh","c_fNh",459,378,600,602);
  TCanvas *c_fEl = new TCanvas("c_fEl","c_fEl",459,378,600,602);


  TCanvas *c_DPhi = new TCanvas("c_DPhi","c_DPhi",459,378,600,602);
  TCanvas *c_DPhi_fat = new TCanvas("c_DPhi_fat","c_DPhi_fat",459,378,600,602);

  TCanvas *c_DPhi_log = new TCanvas("c_DPhi_log","c_DPhi_log",459,378,600,602);
  TCanvas *c_DPhi_fat_log = new TCanvas("c_DPhi_fat_log","c_DPhi_fat_log",459,378,600,602);

  TCanvas *c_Eta = new TCanvas("c_Eta","c_Eta",459,378,600,602);
  TCanvas *c_Eta_1 = new TCanvas("c_Eta_1","c_Eta_1",459,378,600,602);
  TCanvas *c_Eta_2 = new TCanvas("c_Eta_2","c_Eta_2",459,378,600,602);
  TCanvas *c_Eta_fat = new TCanvas("c_Eta_fat","c_Eta_fat",459,378,600,602);
  TCanvas *c_Eta_fat_1 = new TCanvas("c_Eta_fat_1","c_Eta_fat_1",459,378,600,602);
  TCanvas *c_Eta_fat_2 = new TCanvas("c_Eta_fat_2","c_Eta_fat_2",459,378,600,602);
  TCanvas *c_DEta = new TCanvas("c_DEta","c_DEta",459,378,600,602);
  TCanvas *c_DEta_fat = new TCanvas("c_DEta_fat","c_DEta_fat",459,378,600,602);
  TCanvas *c_Phi = new TCanvas("c_Phi","c_Phi",459,378,600,602);
  TCanvas *c_Phi_1 = new TCanvas("c_Phi_1","c_Phi_1",459,378,600,602);
  TCanvas *c_Phi_2 = new TCanvas("c_Phi_2","c_Phi_2",459,378,600,602);
  TCanvas *c_Phi_fat = new TCanvas("c_Phi_fat","c_Phi_fat",459,378,600,602);
  TCanvas *c_Phi_fat_1 = new TCanvas("c_Phi_fat_1","c_Phi_fat_1",459,378,600,602);
  TCanvas *c_Phi_fat_2 = new TCanvas("c_Phi_fat_2","c_Phi_fat_2",459,378,600,602);
  TCanvas *c_n90hits = new TCanvas("c_n90hits","c_n90hits",459,378,600,602);
  TCanvas *c_n90hits_log = new TCanvas("c_n90hits_log","c_n90hits_log",459,378,600,602);
  TCanvas *c_fHPD = new TCanvas("c_fHPD","c_fHPD",459,378,600,602);
  TCanvas *c_corPt = new TCanvas("c_corPt","c_corPt",459,378,600,602);
  TCanvas *c_corPt_1 = new TCanvas("c_corPt_1","c_corPt_1",459,378,600,602);
  TCanvas *c_corPt_2 = new TCanvas("c_corPt_2","c_corPt_2",459,378,600,602);
  TCanvas *c_corPt_fat = new TCanvas("c_corPt_fat","c_corPt_fat",459,378,600,602);
  TCanvas *c_corPt_fat_1 = new TCanvas("c_corPt_fat_1","c_corPt_fat_1",459,378,600,602);
  TCanvas *c_corPt_fat_2 = new TCanvas("c_corPt_fat_2","c_corPt_fat_2",459,378,600,602);
  TCanvas *c_corPt_log = new TCanvas("c_corPt_log","c_corPt_log",459,378,600,602);
  TCanvas *c_corPt_log_1 = new TCanvas("c_corPt_log_1","c_corPt_log_1",459,378,600,602);
  TCanvas *c_corPt_log_2 = new TCanvas("c_corPt_log_2","c_corPt_log_2",459,378,600,602);
  TCanvas *c_corPt_fat_log = new TCanvas("c_corPt_fat_log","c_corPt_fat_log",459,378,600,602);
  TCanvas *c_corPt_fat_log_1 = new TCanvas("c_corPt_fat_log_1","c_corPt_fat_log_1",459,378,600,602);
  TCanvas *c_corPt_fat_log_2 = new TCanvas("c_corPt_fat_log_2","c_corPt_fat_log_2",459,378,600,602);


  TCanvas *c_nTrkVtx = new TCanvas("c_nTrkVtx","c_nTrkVtx",459,378,600,602);
  TCanvas *c_nTrkCalo = new TCanvas("c_nTrkCalo","c_nTrkCalo",459,378,600,602);

  // file definition ----------------------------------

  //  TFile *inf_data  = TFile::Open("out/res/FullSample/histograms_data_HT_All.root");
  //TFile *inf_data  = TFile::Open("out/res/FullSample/histograms_data_HT_ReReco.root");
  TFile *inf_data  = TFile::Open("out/res/FullSample/histograms_data_HT_PromptReco.root");

  TFile *inf_mc_00 = TFile::Open("out/res/histograms_mc_170to300.root");
  TFile *inf_mc_01 = TFile::Open("out/res/histograms_mc_300to470.root");
  TFile *inf_mc_02 = TFile::Open("out/res/histograms_mc_470to600.root");
  TFile *inf_mc_03 = TFile::Open("out/res/histograms_mc_600to800.root");
  TFile *inf_mc_04 = TFile::Open("out/res/histograms_mc_800to1000.root");
  TFile *inf_mc_05 = TFile::Open("out/res/histograms_mc_1000to1400.root");
  TFile *inf_mc_06 = TFile::Open("out/res/histograms_mc_1400to1800.root");
  TFile *inf_mc_07 = TFile::Open("out/res/histograms_mc_1800toinf.root");

  TFile *outf = new TFile("histogrmas_201p348pbm1.root","RECREATE");

  // p_T and mass boundaries ---------------------------
  
  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 
  double ptBoundaries[55] = {1, 9, 18, 29, 40, 53, 67, 81, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3500};

  // Canvas Setting

  c_DijetMass_ratio->SetGridx();
  c_DijetMass_ratio->SetGridy();
  c_DijetMass_ratio_fat->SetGridx();
  c_DijetMass_ratio_fat->SetGridy();
  c_DijetMass_ratio_log->SetGridx();
  c_DijetMass_ratio_log->SetGridy();
  c_DijetMass_ratio_fat_log->SetGridx();
  c_DijetMass_ratio_fat_log->SetGridy();
  c_Eta_Phi_Scatter->SetRightMargin(0.1157718);

  // histogrmas definition ------------------------------

  //  TH1 *h_DijetMass_data = new TH1F("h_DijetMass_data","DijetMass_data",83, xAxis1);
  //  TH1 *h_DijetMass_data_fat = new TH1F("h_DijetMass_data_fat","DijetMass_data_fat",83, xAxis1);
  TH1 *h_DijetMass_mc = new TH1F("h_DijetMass_mc","DijetMass_mc",83, xAxis1);
  TH1 *h_DijetMass_mc_fat = new TH1F("h_DijetMass_mc_fat","DijetMass_mc_fat",83, xAxis1);
  TH1 *h_DijetMass_ratio = new TH1F("h_DijetMass_ratio","DijetMass_ratio",83, xAxis1);
  TH1 *h_DijetMass_ratio_fat = new TH1F("h_DijetMass_ratio_fat","DijetMass_ratio_fat",83, xAxis1);
  //  TH1 *h_DijetMass_MI_nPVe1_data = new TH1F("h_DijetMass_MI_nPVe1_data","DijetMass_MI_nPVe1_data",83, xAxis1);
  //  TH1 *h_DijetMass_MI_nPVg1_data = new TH1F("h_DijetMass_MI_nPVg1_data","DijetMass_MI_nPVg1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVe1_mc = new TH1F("h_DijetMass_MI_nPVe1_mc","DijetMass_MI_nPVe1_mc",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg1_mc = new TH1F("h_DijetMass_MI_nPVg1_mc","DijetMass_MI_nPVg1_mc",83, xAxis1);
  //  TH2 *h_Eta_Phi_Scatter_data = new TH2F("h_Eta_Phi_Scatter","Eta_Phi_Scatter",100,-3,3,100,-3.2,3.2);
  //  TH2 *h_Eta_Phi_Scatter_data_1 = new TH2F("h_Eta_Phi_Scatter_1","Eta_Phi_Scatter_1",100,-3,3,100,-3.2,3.2);
  //  TH2 *h_Eta_Phi_Scatter_data_2 = new TH2F("h_Eta_Phi_Scatter_2","Eta_Phi_Scatter_2",100,-3,3,100,-3.2,3.2);
  //  TH1 *h_MET_over_sumEt_data = new TH1F("h_MET_over_sumEt_data","MET_over_sumEt_data",25,0.0,1.0001);
  TH1 *h_MET_over_sumEt_mc = new TH1F("h_MET_over_sumEt_mc","MET_over_sumEt_mc",25,0.0,1.0001);
  //  TH1 *h_EMF_data = new TH1F("h_EMF_data","EMF_data",25,0.0,1.0001);
  TH1 *h_EMF_mc = new TH1F("h_EMF_mc","EMF_mc",25,0.0,1.0001);
  //  TH1 *h_DPhi_data = new TH1F("h_DPhi_data","DPhi_data",25,0.0,3.142);
  //  TH1 *h_DPhi_data_fat = new TH1F("h_DPhi_data_fat","DPhi_data_fat",25,0.0,3.142);
  TH1 *h_DPhi_mc = new TH1F("h_DPhi_mc","DPhi_mc",25,0.0,3.142);
  TH1 *h_DPhi_mc_fat = new TH1F("h_DPhi_mc_fat","DPhi_mc_fat",25,0.0,3.142);
  //  TH1 *h_Eta_data = new TH1F("h_Eta_data","Eta_data",60,-3.0,3.0);
  //  TH1 *h_Eta_data_1 = new TH1F("h_Eta_data_1","Eta_data_1",60,-3.0,3.0);
  //  TH1 *h_Eta_data_2 = new TH1F("h_Eta_data_2","Eta_data_2",60,-3.0,3.0);
  //  TH1 *h_Eta_data_fat = new TH1F("h_Eta_data_fat","Eta_data_fat",60,-3.0,3.0);
  //  TH1 *h_Eta_data_fat_1 = new TH1F("h_Eta_data_fat_1","Eta_data_fat_1",60,-3.0,3.0);
  //  TH1 *h_Eta_data_fat_2 = new TH1F("h_Eta_data_fat_2","Eta_data_fat_2",60,-3.0,3.0);
  TH1 *h_Eta_mc = new TH1F("h_Eta_mc","Eta_mc",60,-3.0,3.0);
  TH1 *h_Eta_mc_1 = new TH1F("h_Eta_mc_1","Eta_mc_1",60,-3.0,3.0);
  TH1 *h_Eta_mc_2 = new TH1F("h_Eta_mc_2","Eta_mc_2",60,-3.0,3.0);
  TH1 *h_Eta_mc_fat = new TH1F("h_Eta_mc_fat","Eta_mc_fat",60,-3.0,3.0);
  TH1 *h_Eta_mc_fat_1 = new TH1F("h_Eta_mc_fat_1","Eta_mc_fat_1",60,-3.0,3.0);
  TH1 *h_Eta_mc_fat_2 = new TH1F("h_Eta_mc_fat_2","Eta_mc_fat_2",60,-3.0,3.0);
  //  TH1 *h_DEta_data = new TH1F("h_DEta_data","DEta_data",15,0.,1.5);
  //  TH1 *h_DEta_data_fat = new TH1F("h_DEta_data_fat","DEta_data_fat",15,0.,1.5);
  TH1 *h_DEta_mc = new TH1F("h_DEta_mc","DEta_mc",15,0.,1.5);
  TH1 *h_DEta_mc_fat = new TH1F("h_DEta_mc_fat","DEta_mc_fat",15,0.,1.5);
  //  TH1 *h_Phi_data = new TH1F("h_Phi_data","Phi_data",18,-3.142,3.142);
  //  TH1 *h_Phi_data_1 = new TH1F("h_Phi_data_1","Phi_data_1",18,-3.142,3.142);
  //  TH1 *h_Phi_data_2 = new TH1F("h_Phi_data_2","Phi_data_2",18,-3.142,3.142);
  //  TH1 *h_Phi_data_fat = new TH1F("h_Phi_data_fat","Phi_data_fat",18,-3.142,3.142);
  //  TH1 *h_Phi_data_fat_1 = new TH1F("h_Phi_data_fat_1","Phi_data_fat_1",18,-3.142,3.142);
  //  TH1 *h_Phi_data_fat_2 = new TH1F("h_Phi_data_fat_2","Phi_data_fat_2",18,-3.142,3.142);
  TH1 *h_Phi_mc = new TH1F("h_Phi_mc","Phi_mc",18,-3.142,3.142);
  TH1 *h_Phi_mc_1 = new TH1F("h_Phi_mc_1","Phi_mc_1",18,-3.142,3.142);
  TH1 *h_Phi_mc_2 = new TH1F("h_Phi_mc_2","Phi_mc_2",18,-3.142,3.142);
  TH1 *h_Phi_mc_fat = new TH1F("h_Phi_mc_fat","Phi_mc_fat",18,-3.142,3.142);
  TH1 *h_Phi_mc_fat_1 = new TH1F("h_Phi_mc_fat_1","Phi_mc_fat_1",18,-3.142,3.142);
  TH1 *h_Phi_mc_fat_2 = new TH1F("h_Phi_mc_fat_2","Phi_mc_fat_2",18,-3.142,3.142);
  //  TH1 *h_n90hits_data = new TH1F("h_n90hits_data","n90hits_data",400,-0.5,399.5);
  TH1 *h_n90hits_mc = new TH1F("n90hits_mc","n90hits_mc",400,-0.5,399.5);
  //  TH1 *h_fHPD_data = new TH1F("h_fHPD_data","fHPD_data",25,0.0,1.0001);
  TH1 *h_fHPD_mc = new TH1F("h_fHPD_mc","fHPD_mc",25,0.0,1.0001);
  //  TH1 *h_corPt_data = new TH1F("h_corPt_data","corPt_data",54,ptBoundaries);
  //  TH1 *h_corPt_data_1 = new TH1F("h_corPt_data_1","corPt_data_1",54,ptBoundaries);
  //  TH1 *h_corPt_data_2 = new TH1F("h_corPt_data_2","corPt_data_2",54,ptBoundaries);
  //  TH1 *h_corPt_data_fat = new TH1F("h_corPt_data_fat","corPt_data",54,ptBoundaries);
  //  TH1 *h_corPt_data_fat_1 = new TH1F("h_corPt_data_fat_1","corPt_data_fat_1",54,ptBoundaries);
  //  TH1 *h_corPt_data_fat_2 = new TH1F("h_corPt_data_fat_2","corPt_data_fat_2",54,ptBoundaries);
  TH1 *h_corPt_mc = new TH1F("h_corPt_mc","corPt_mc",54,ptBoundaries);
  TH1 *h_corPt_mc_1 = new TH1F("h_corPt_mc_1","corPt_mc_1",54,ptBoundaries);
  TH1 *h_corPt_mc_2 = new TH1F("h_corPt_mc_2","corPt_mc_2",54,ptBoundaries);
  TH1 *h_corPt_mc_fat = new TH1F("h_corPt_mc_fat","corPt_mc_fat",54,ptBoundaries);
  TH1 *h_corPt_mc_fat_1 = new TH1F("h_corPt_mc_fat_1","corPt_mc_fat_1",54,ptBoundaries);
  TH1 *h_corPt_mc_fat_2 = new TH1F("h_corPt_mc_fat_2","corPt_mc_fat_2",54,ptBoundaries);
  //  TH1 *h_nTrkVtx_data = new TH1F("h_nTrkVtx_data","nTrkVtx_data",900,-0.5,899.5);
  TH1 *h_nTrkVtx_mc = new TH1F("nTrkVtx_mc","nTrkVtx_mc",900,-0.5,899.5);
  //  TH1 *h_nTrkCalo_data = new TH1F("h_nTrkCalo_data","nTrkCalo_data",100,-0.5,99.5);
  TH1 *h_nTrkCalo_mc = new TH1F("nTrkCalo_mc","nTrkCalo_mc",100,-0.5,99.5);
  //  TH1 *h_JCFVtx_data = new TH1F("h_JCFVtx_data","JCFVtx_data",25,-1.5,1.5);
  //  TH1 *h_JCFVtx_mc = new TH1F("h_JCFVtx_mc","JCFVtx_mc",25,-1.5,1.5);
  //  TH1 *h_JCFCalo_data = new TH1F("h_JCFCalo_data","JCFCalo_data",4000,-0.5,999.5);
  //  TH1 *h_JCFCalo_mc = new TH1F("h_JCFCalo_mc","JCFCalo_mc",4000,-0.5,999.5);

  //  TH1F *hPFCorMjj = new TH1F("PFCorMjj","PFCorMjj",500,0,5000);

  TH1 *h_fCh_mc_fat = new TH1F("h_fCh_mc_fat","fCh_mc",25,0.0,1.0001);
  TH1 *h_fNh_mc_fat = new TH1F("h_fNh_mc_fat","fNh_mc",25,0.0,1.0001);
  TH1 *h_fPh_mc_fat = new TH1F("h_fPh_mc_fat","fPh_mc",25,0.0,1.0001);
  TH1 *h_fEl_mc_fat = new TH1F("h_fEl_mc_fat","fEl_mc",25,0.0,1.0001);


  // TLegend definition -----------------------------------------------------

  TLegend *l_DijetMass = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_fat = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_fat_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_MI = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_MI_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_normalized_MI = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_normalized_MI_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_MET_over_sumEt = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_MET_over_sumEt_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_EMF = new TLegend(0.3456376,0.1602787,0.8573826,0.3292683,NULL,"brNDC");

  TLegend *l_fCh = new TLegend(0.15,0.80,0.55,0.92,NULL,"brNDC");
  TLegend *l_fNh = new TLegend(0.3456376,0.2602787,0.8573826,0.4292683,NULL,"brNDC");
  TLegend *l_fPh = new TLegend(0.50,0.80,0.95,0.92,NULL,"brNDC");

  TLegend *l_fEl = new TLegend(0.3456376,0.2602787,0.8573826,0.4292683,NULL,"brNDC");

  TLegend *l_DPhi = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_log = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_fat = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_fat_log = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_Eta = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_1 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_2 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_fat = new TLegend(0.2902685,0.8,0.8053691,0.92,NULL,"brNDC");
  TLegend *l_Eta_fat_1 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_fat_2 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_DEta = new TLegend(0.2567114,0.2195122,0.7651007,0.4198606,NULL,"brNDC");
  TLegend *l_DEta_fat = new TLegend(0.2567114,0.2195122,0.7651007,0.4198606,NULL,"brNDC");
  TLegend *l_Phi = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_1 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_2 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_fat = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_fat_1 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_fat_2 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_n90hits = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_n90hits_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_fHPD = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_log_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_log_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat_log_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat_log_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_nTrkVtx = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_nTrkCalo = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_JCFCalo = new TLegend(0.43,0.70,0.93,0.90);

  // TPave definition -------------------------------------

  TPaveText *pt_DijetMass_ratio = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_DijetMass_ratio_fat = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_DijetMass_ratio_log = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_DijetMass_ratio_fat_log = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Eta_Phi_Scatter = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Eta_Phi_Scatter_1 = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Eta_Phi_Scatter_2 = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Phi = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_1 = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_2 = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_fat = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_fat_1 = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_fat_2 = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");

  // data histogram

  TH1* h_DijetMass_data= (TH1F *) inf_data->Get("h_DijetMass_data");
  TH1* h_DijetMass_data_fat= (TH1F *) inf_data->Get("h_DijetMass_data_fat");
  TH1* h_DijetMass_MI_nPVe1_data= (TH1F *) inf_data->Get("h_DijetMass_MI_nPVe1_data");
  TH1* h_DijetMass_MI_nPVg1_data= (TH1F *) inf_data->Get("h_DijetMass_MI_nPVg1_data");
  TH2* h_Eta_Phi_Scatter_data = (TH2F *) inf_data->Get("h_Eta_Phi_Scatter");
  TH2* h_Eta_Phi_Scatter_data_1 = (TH2F *) inf_data->Get("h_Eta_Phi_Scatter_1");
  TH2* h_Eta_Phi_Scatter_data_2 = (TH2F *) inf_data->Get("h_Eta_Phi_Scatter_2");
  TH1* h_MET_over_sumEt_data= (TH1F *) inf_data->Get("h_MET_over_sumEt_data");
  TH1* h_EMF_data= (TH1F *) inf_data->Get("h_EMF_data");
  TH1* h_DPhi_data= (TH1F *) inf_data->Get("h_DPhi_data");
  TH1* h_DPhi_data_fat= (TH1F *) inf_data->Get("h_DPhi_data_fat");
  TH1* h_Eta_data= (TH1F *) inf_data->Get("h_Eta_data");
  TH1* h_Eta_data_1= (TH1F *) inf_data->Get("h_Eta_data_1");
  TH1* h_Eta_data_2= (TH1F *) inf_data->Get("h_Eta_data_2");
  TH1* h_Eta_data_fat= (TH1F *) inf_data->Get("h_Eta_data_fat");
  TH1* h_Eta_data_fat_1= (TH1F *) inf_data->Get("h_Eta_data_fat_1");
  TH1* h_Eta_data_fat_2= (TH1F *) inf_data->Get("h_Eta_data_fat_2");
  TH1* h_DEta_data= (TH1F *) inf_data->Get("h_DEta_data");
  TH1* h_DEta_data_fat= (TH1F *) inf_data->Get("h_DEta_data_fat");
  TH1* h_Phi_data= (TH1F *) inf_data->Get("h_Phi_data");
  TH1* h_Phi_data_1= (TH1F *) inf_data->Get("h_Phi_data_1");
  TH1* h_Phi_data_2= (TH1F *) inf_data->Get("h_Phi_data_2");
  TH1* h_Phi_data_fat= (TH1F *) inf_data->Get("h_Phi_data_fat");
  TH1* h_Phi_data_fat_1= (TH1F *) inf_data->Get("h_Phi_data_fat_1");
  TH1* h_Phi_data_fat_2= (TH1F *) inf_data->Get("h_Phi_data_fat_2");
  TH1* h_n90hits_data= (TH1F *) inf_data->Get("h_n90hits_data");
  TH1* h_fHPD_data= (TH1F *) inf_data->Get("h_fHPD_data");
  TH1* h_corPt_data= (TH1F *) inf_data->Get("h_corPt_data");
  TH1* h_corPt_data_1= (TH1F *) inf_data->Get("h_corPt_data_1");
  TH1* h_corPt_data_2= (TH1F *) inf_data->Get("h_corPt_data_2");
  TH1* h_corPt_data_fat= (TH1F *) inf_data->Get("h_corPt_data_fat");
  TH1* h_corPt_data_fat_1= (TH1F *) inf_data->Get("h_corPt_data_fat_1");
  TH1* h_corPt_data_fat_2= (TH1F *) inf_data->Get("h_corPt_data_fat_2");
  TH1* h_nTrkVtx_data= (TH1F *) inf_data->Get("h_nTrkVtx_data");
  TH1* h_nTrkCalo_data= (TH1F *) inf_data->Get("h_nTrkCalo_data");


  TH1 *h_fCh_data_fat = (TH1F *) inf_data->Get("h_fCh_data_pf");
  TH1 *h_fNh_data_fat = (TH1F *) inf_data->Get("h_fNh_data_pf");
  TH1 *h_fPh_data_fat = (TH1F *) inf_data->Get("h_fPh_data_pf");
  TH1 *h_fEl_data_fat = (TH1F *) inf_data->Get("h_fEl_data_pf");


    std::cout << "test1" << std::endl;
  // copy data dijet mass distribution histogram
  
  TH1F *h_DijetMass_normalized_MI_nPVe1_data    =(TH1F*)h_DijetMass_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_normalized_MI_nPVg1_data    =(TH1F*)h_DijetMass_MI_nPVg1_data->Clone();
  // it's originally 3955458(3939510) but two job losted so I used crab -report to get shi value. 
  //float event[8] = {5684160.0, 6336960.0, 4034162.0, 3939510.0, 4161710.0, 2169144.0, 2043020.0, 1058720.0};
  float event[8] = {5671529., 6308796., 3050637., 3939510., 4161710., 2169144., 2043020., 1056368.};
  float xs[8] = {2.426e+04, 1.168e+03, 7.022e+01, 1.555e+01, 1.844e+00, 3.321e-01, 1.087e-02, 3.575e-04};

  ///// inf_mc_00

  TH1F* h_DijetMass_mc_00= (TH1F *) inf_mc_00->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_00= (TH1F *) inf_mc_00->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_00= (TH1F *) inf_mc_00->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_00= (TH1F *) inf_mc_00->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_00= (TH1F *) inf_mc_00->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_00= (TH1F *) inf_mc_00->Get("h_EMF_data");
  TH1F* h_DPhi_mc_00= (TH1F *) inf_mc_00->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_00= (TH1F *) inf_mc_00->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_00= (TH1F *) inf_mc_00->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_00= (TH1F *) inf_mc_00->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_00= (TH1F *) inf_mc_00->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_00= (TH1F *) inf_mc_00->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_00= (TH1F *) inf_mc_00->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_00= (TH1F *) inf_mc_00->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_00= (TH1F *) inf_mc_00->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_00= (TH1F *) inf_mc_00->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_00= (TH1F *) inf_mc_00->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_00= (TH1F *) inf_mc_00->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_00= (TH1F *) inf_mc_00->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_00= (TH1F *) inf_mc_00->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_00= (TH1F *) inf_mc_00->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_00= (TH1F *) inf_mc_00->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_00= (TH1F *) inf_mc_00->Get("n90hits_data");
  TH1F* h_fHPD_mc_00= (TH1F *) inf_mc_00->Get("h_fHPD_data");
  TH1F* h_corPt_mc_00= (TH1F *) inf_mc_00->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_00= (TH1F *) inf_mc_00->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_00= (TH1F *) inf_mc_00->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_00= (TH1F *) inf_mc_00->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_00= (TH1F *) inf_mc_00->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_00= (TH1F *) inf_mc_00->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_00= (TH1F *) inf_mc_00->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_00= (TH1F *) inf_mc_00->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_00 =(TH1F *) inf_mc_00->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_00 =(TH1F *) inf_mc_00->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_00 =(TH1F *) inf_mc_00->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_00 =(TH1F *) inf_mc_00->Get("h_fEl_data_pf");

  std::cout << "test2" << std::endl;
  ///// inf_mc_1

  TH1F* h_DijetMass_mc_01 = (TH1F *) inf_mc_01->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_01 = (TH1F *) inf_mc_01->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_01 = (TH1F *) inf_mc_01->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_01 = (TH1F *) inf_mc_01->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_01 = (TH1F *) inf_mc_01->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_01 = (TH1F *) inf_mc_01->Get("h_EMF_data");
  TH1F* h_DPhi_mc_01 = (TH1F *) inf_mc_01->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_01 = (TH1F *) inf_mc_01->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_01 = (TH1F *) inf_mc_01->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_01 = (TH1F *) inf_mc_01->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_01 = (TH1F *) inf_mc_01->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_01 = (TH1F *) inf_mc_01->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_01 = (TH1F *) inf_mc_01->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_01 = (TH1F *) inf_mc_01->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_01 = (TH1F *) inf_mc_01->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_01 = (TH1F *) inf_mc_01->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_01 = (TH1F *) inf_mc_01->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_01 = (TH1F *) inf_mc_01->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_01 = (TH1F *) inf_mc_01->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_01 = (TH1F *) inf_mc_01->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_01 = (TH1F *) inf_mc_01->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_01 = (TH1F *) inf_mc_01->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_01 = (TH1F *) inf_mc_01->Get("n90hits_data");
  TH1F* h_fHPD_mc_01 = (TH1F *) inf_mc_01->Get("h_fHPD_data");
  TH1F* h_corPt_mc_01 = (TH1F *) inf_mc_01->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_01 = (TH1F *) inf_mc_01->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_01 = (TH1F *) inf_mc_01->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_01 = (TH1F *) inf_mc_01->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_01 = (TH1F *) inf_mc_01->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_01 = (TH1F *) inf_mc_01->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_01 = (TH1F *) inf_mc_01->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_01 = (TH1F *) inf_mc_01->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_01 =(TH1F *) inf_mc_01->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_01 =(TH1F *) inf_mc_01->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_01 =(TH1F *) inf_mc_01->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_01 =(TH1F *) inf_mc_01->Get("h_fEl_data_pf");

   std::cout << "test3" << std::endl;
  ///// inf_mc_2

  TH1F* h_DijetMass_mc_02 = (TH1F *) inf_mc_02->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_02 = (TH1F *) inf_mc_02->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_02 = (TH1F *) inf_mc_02->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_02 = (TH1F *) inf_mc_02->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_02 = (TH1F *) inf_mc_02->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_02 = (TH1F *) inf_mc_02->Get("h_EMF_data");
  TH1F* h_DPhi_mc_02 = (TH1F *) inf_mc_02->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_02 = (TH1F *) inf_mc_02->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_02 = (TH1F *) inf_mc_02->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_02 = (TH1F *) inf_mc_02->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_02 = (TH1F *) inf_mc_02->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_02 = (TH1F *) inf_mc_02->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_02 = (TH1F *) inf_mc_02->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_02 = (TH1F *) inf_mc_02->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_02 = (TH1F *) inf_mc_02->Get("h_DEta_data");
   std::cout << "test3_1" << std::endl;
  TH1F* h_DEta_mc_fat_02 = (TH1F *) inf_mc_02->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_02 = (TH1F *) inf_mc_02->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_02 = (TH1F *) inf_mc_02->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_02 = (TH1F *) inf_mc_02->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_02 = (TH1F *) inf_mc_02->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_02 = (TH1F *) inf_mc_02->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_02 = (TH1F *) inf_mc_02->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_02 = (TH1F *) inf_mc_02->Get("n90hits_data");
  TH1F* h_fHPD_mc_02 = (TH1F *) inf_mc_02->Get("h_fHPD_data");
  TH1F* h_corPt_mc_02 = (TH1F *) inf_mc_02->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_02 = (TH1F *) inf_mc_02->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_02 = (TH1F *) inf_mc_02->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_02 = (TH1F *) inf_mc_02->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_02 = (TH1F *) inf_mc_02->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_02 = (TH1F *) inf_mc_02->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_02 = (TH1F *) inf_mc_02->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_02 = (TH1F *) inf_mc_02->Get("nTrkCalo_data");
 
  TH1 *h_fCh_mc_fat_02 =(TH1F *) inf_mc_02->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_02 =(TH1F *) inf_mc_02->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_02 =(TH1F *) inf_mc_02->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_02 =(TH1F *) inf_mc_02->Get("h_fEl_data_pf");

   std::cout << "test4" << std::endl;
  ///// inf_mc_3

  TH1F* h_DijetMass_mc_03 = (TH1F *) inf_mc_03->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_03 = (TH1F *) inf_mc_03->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_03 = (TH1F *) inf_mc_03->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_03 = (TH1F *) inf_mc_03->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_03 = (TH1F *) inf_mc_03->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_03 = (TH1F *) inf_mc_03->Get("h_EMF_data");
  TH1F* h_DPhi_mc_03 = (TH1F *) inf_mc_03->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_03 = (TH1F *) inf_mc_03->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_03 = (TH1F *) inf_mc_03->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_03 = (TH1F *) inf_mc_03->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_03 = (TH1F *) inf_mc_03->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_03 = (TH1F *) inf_mc_03->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_03 = (TH1F *) inf_mc_03->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_03 = (TH1F *) inf_mc_03->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_03 = (TH1F *) inf_mc_03->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_03 = (TH1F *) inf_mc_03->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_03 = (TH1F *) inf_mc_03->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_03 = (TH1F *) inf_mc_03->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_03 = (TH1F *) inf_mc_03->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_03 = (TH1F *) inf_mc_03->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_03 = (TH1F *) inf_mc_03->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_03 = (TH1F *) inf_mc_03->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_03 = (TH1F *) inf_mc_03->Get("n90hits_data");
  TH1F* h_fHPD_mc_03 = (TH1F *) inf_mc_03->Get("h_fHPD_data");
  TH1F* h_corPt_mc_03 = (TH1F *) inf_mc_03->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_03 = (TH1F *) inf_mc_03->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_03 = (TH1F *) inf_mc_03->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_03 = (TH1F *) inf_mc_03->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_03 = (TH1F *) inf_mc_03->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_03 = (TH1F *) inf_mc_03->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_03 = (TH1F *) inf_mc_03->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_03 = (TH1F *) inf_mc_03->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_03 =(TH1F *) inf_mc_03->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_03 =(TH1F *) inf_mc_03->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_03 =(TH1F *) inf_mc_03->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_03 =(TH1F *) inf_mc_03->Get("h_fEl_data_pf");

   std::cout << "test5" << std::endl;
  ///// inf_mc_4
   
  TH1F* h_DijetMass_mc_04= (TH1F *) inf_mc_04->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_04= (TH1F *) inf_mc_04->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_04= (TH1F *) inf_mc_04->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_04= (TH1F *) inf_mc_04->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_04= (TH1F *) inf_mc_04->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_04= (TH1F *) inf_mc_04->Get("h_EMF_data");
  TH1F* h_DPhi_mc_04= (TH1F *) inf_mc_04->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_04= (TH1F *) inf_mc_04->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_04= (TH1F *) inf_mc_04->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_04= (TH1F *) inf_mc_04->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_04= (TH1F *) inf_mc_04->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_04= (TH1F *) inf_mc_04->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_04= (TH1F *) inf_mc_04->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_04= (TH1F *) inf_mc_04->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_04= (TH1F *) inf_mc_04->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_04= (TH1F *) inf_mc_04->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_04= (TH1F *) inf_mc_04->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_04= (TH1F *) inf_mc_04->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_04= (TH1F *) inf_mc_04->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_04= (TH1F *) inf_mc_04->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_04= (TH1F *) inf_mc_04->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_04= (TH1F *) inf_mc_04->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_04= (TH1F *) inf_mc_04->Get("n90hits_data");
  TH1F* h_fHPD_mc_04= (TH1F *) inf_mc_04->Get("h_fHPD_data");
  TH1F* h_corPt_mc_04= (TH1F *) inf_mc_04->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_04= (TH1F *) inf_mc_04->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_04= (TH1F *) inf_mc_04->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_04= (TH1F *) inf_mc_04->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_04= (TH1F *) inf_mc_04->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_04= (TH1F *) inf_mc_04->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_04= (TH1F *) inf_mc_04->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_04= (TH1F *) inf_mc_04->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_04 =(TH1F *) inf_mc_04->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_04 =(TH1F *) inf_mc_04->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_04 =(TH1F *) inf_mc_04->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_04 =(TH1F *) inf_mc_04->Get("h_fEl_data_pf");

   std::cout << "test6" << std::endl;
  ///// inf_mc_5
  

  TH1F* h_DijetMass_mc_05 = (TH1F *) inf_mc_05->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_05 = (TH1F *) inf_mc_05->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_05 = (TH1F *) inf_mc_05->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_05 = (TH1F *) inf_mc_05->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_05 = (TH1F *) inf_mc_05->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_05 = (TH1F *) inf_mc_05->Get("h_EMF_data");
  TH1F* h_DPhi_mc_05 = (TH1F *) inf_mc_05->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_05 = (TH1F *) inf_mc_05->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_05 = (TH1F *) inf_mc_05->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_05 = (TH1F *) inf_mc_05->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_05 = (TH1F *) inf_mc_05->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_05 = (TH1F *) inf_mc_05->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_05 = (TH1F *) inf_mc_05->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_05 = (TH1F *) inf_mc_05->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_05 = (TH1F *) inf_mc_05->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_05 = (TH1F *) inf_mc_05->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_05 = (TH1F *) inf_mc_05->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_05 = (TH1F *) inf_mc_05->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_05 = (TH1F *) inf_mc_05->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_05 = (TH1F *) inf_mc_05->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_05 = (TH1F *) inf_mc_05->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_05 = (TH1F *) inf_mc_05->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_05 = (TH1F *) inf_mc_05->Get("n90hits_data");
  TH1F* h_fHPD_mc_05 = (TH1F *) inf_mc_05->Get("h_fHPD_data");
  TH1F* h_corPt_mc_05 = (TH1F *) inf_mc_05->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_05 = (TH1F *) inf_mc_05->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_05 = (TH1F *) inf_mc_05->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_05 = (TH1F *) inf_mc_05->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_05 = (TH1F *) inf_mc_05->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_05 = (TH1F *) inf_mc_05->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_05 = (TH1F *) inf_mc_05->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_05 = (TH1F *) inf_mc_05->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_05 =(TH1F *) inf_mc_05->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_05 =(TH1F *) inf_mc_05->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_05 =(TH1F *) inf_mc_05->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_05 =(TH1F *) inf_mc_05->Get("h_fEl_data_pf");

   std::cout << "test7" << std::endl;
  ///// inf_mc_06

  TH1F* h_DijetMass_mc_06 = (TH1F *) inf_mc_06->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_06 = (TH1F *) inf_mc_06->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_06 = (TH1F *) inf_mc_06->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_06 = (TH1F *) inf_mc_06->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_06 = (TH1F *) inf_mc_06->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_06 = (TH1F *) inf_mc_06->Get("h_EMF_data");
  TH1F* h_DPhi_mc_06 = (TH1F *) inf_mc_06->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_06 = (TH1F *) inf_mc_06->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_06 = (TH1F *) inf_mc_06->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_06 = (TH1F *) inf_mc_06->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_06 = (TH1F *) inf_mc_06->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_06 = (TH1F *) inf_mc_06->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_06 = (TH1F *) inf_mc_06->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_06 = (TH1F *) inf_mc_06->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_06 = (TH1F *) inf_mc_06->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_06 = (TH1F *) inf_mc_06->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_06 = (TH1F *) inf_mc_06->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_06 = (TH1F *) inf_mc_06->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_06 = (TH1F *) inf_mc_06->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_06 = (TH1F *) inf_mc_06->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_06 = (TH1F *) inf_mc_06->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_06 = (TH1F *) inf_mc_06->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_06 = (TH1F *) inf_mc_06->Get("n90hits_data");
  TH1F* h_fHPD_mc_06 = (TH1F *) inf_mc_06->Get("h_fHPD_data");
  TH1F* h_corPt_mc_06 = (TH1F *) inf_mc_06->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_06 = (TH1F *) inf_mc_06->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_06 = (TH1F *) inf_mc_06->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_06 = (TH1F *) inf_mc_06->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_06 = (TH1F *) inf_mc_06->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_06 = (TH1F *) inf_mc_06->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_06 = (TH1F *) inf_mc_06->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_06 = (TH1F *) inf_mc_06->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_06 =(TH1F *) inf_mc_06->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_06 =(TH1F *) inf_mc_06->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_06 =(TH1F *) inf_mc_06->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_06 =(TH1F *) inf_mc_06->Get("h_fEl_data_pf");

   std::cout << "test8" << std::endl;
  ///// inf_mc_07

  TH1F* h_DijetMass_mc_07 = (TH1F *) inf_mc_07->Get("h_DijetMass_data");
  TH1F* h_DijetMass_mc_fat_07 = (TH1F *) inf_mc_07->Get("h_DijetMass_data_fat");
  TH1F* h_DijetMass_MI_nPVe1_mc_07 = (TH1F *) inf_mc_07->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc_07 = (TH1F *) inf_mc_07->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc_07 = (TH1F *) inf_mc_07->Get("h_MET_over_sumEt_data");
  TH1F* h_EMF_mc_07 = (TH1F *) inf_mc_07->Get("h_EMF_data");
  TH1F* h_DPhi_mc_07 = (TH1F *) inf_mc_07->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat_07 = (TH1F *) inf_mc_07->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc_07 = (TH1F *) inf_mc_07->Get("h_Eta_data");
  TH1F* h_Eta_mc_1_07 = (TH1F *) inf_mc_07->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2_07 = (TH1F *) inf_mc_07->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat_07 = (TH1F *) inf_mc_07->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1_07 = (TH1F *) inf_mc_07->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2_07 = (TH1F *) inf_mc_07->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc_07 = (TH1F *) inf_mc_07->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat_07 = (TH1F *) inf_mc_07->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc_07 = (TH1F *) inf_mc_07->Get("h_Phi_data");
  TH1F* h_Phi_mc_1_07 = (TH1F *) inf_mc_07->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2_07 = (TH1F *) inf_mc_07->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat_07 = (TH1F *) inf_mc_07->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1_07 = (TH1F *) inf_mc_07->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2_07 = (TH1F *) inf_mc_07->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc_07 = (TH1F *) inf_mc_07->Get("n90hits_data");
  TH1F* h_fHPD_mc_07 = (TH1F *) inf_mc_07->Get("h_fHPD_data");
  TH1F* h_corPt_mc_07 = (TH1F *) inf_mc_07->Get("h_corPt_data");
  TH1F* h_corPt_mc_1_07 = (TH1F *) inf_mc_07->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2_07 = (TH1F *) inf_mc_07->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat_07 = (TH1F *) inf_mc_07->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1_07 = (TH1F *) inf_mc_07->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2_07 = (TH1F *) inf_mc_07->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc_07 = (TH1F *) inf_mc_07->Get("nTrkVtx_data");
  TH1F* h_nTrkCalo_mc_07 = (TH1F *) inf_mc_07->Get("nTrkCalo_data");

  TH1 *h_fCh_mc_fat_07 =(TH1F *) inf_mc_07->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat_07 =(TH1F *) inf_mc_07->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat_07 =(TH1F *) inf_mc_07->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat_07 =(TH1F *) inf_mc_07->Get("h_fEl_data_pf");

   std::cout << "test9_0" << std::endl;
  // _0

   cout << "xs[0] = " << xs[0] << " event[0] = " << event[0] << endl;

  h_DijetMass_mc->Add(h_DijetMass_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_1" << std::endl;
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_00,xs[0]/event[0]);
   std::cout << "test9_0_2" << std::endl;
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_3" << std::endl;
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_4" << std::endl;
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_5" << std::endl;
  h_EMF_mc->Add(h_EMF_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_6" << std::endl;
  h_DPhi_mc->Add(h_DPhi_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_7" << std::endl;
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_00,xs[0]/event[0]);
   std::cout << "test9_0_8" << std::endl;
  h_Eta_mc->Add(h_Eta_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_9" << std::endl;
  h_Eta_mc_1->Add(h_Eta_mc_1_00,xs[0]/event[0]);
   std::cout << "test9_0_10" << std::endl;
  h_Eta_mc_2->Add(h_Eta_mc_2_00,xs[0]/event[0]);
   std::cout << "test9_0_11" << std::endl;
  h_Eta_mc_fat->Add(h_Eta_mc_fat_00,xs[0]/event[0]);
   std::cout << "test9_0_12" << std::endl;
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_00,xs[0]/event[0]);
   std::cout << "test9_0_13" << std::endl;
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_00,xs[0]/event[0]);
   std::cout << "test9_0_14" << std::endl;
  h_DEta_mc->Add(h_DEta_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_15" << std::endl;
  h_DEta_mc_fat->Add(h_DEta_mc_fat_00,xs[0]/event[0]);
   std::cout << "test9_0_16" << std::endl;
  h_Phi_mc->Add(h_Phi_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_17" << std::endl;
  h_Phi_mc_1->Add(h_Phi_mc_1_00,xs[0]/event[0]);
   std::cout << "test9_0_18" << std::endl;
  h_Phi_mc_2->Add(h_Phi_mc_2_00,xs[0]/event[0]);
   std::cout << "test9_0_19" << std::endl;
  h_Phi_mc_fat->Add(h_Phi_mc_fat_00,xs[0]/event[0]);
   std::cout << "test9_0_20" << std::endl;
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_00,xs[0]/event[0]);
   std::cout << "test9_0_21" << std::endl;
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_00,xs[0]/event[0]);
   std::cout << "test9_0_22" << std::endl;
  h_n90hits_mc->Add(h_n90hits_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_23" << std::endl;
  h_fHPD_mc->Add(h_fHPD_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_24" << std::endl;
  h_corPt_mc->Add(h_corPt_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_25" << std::endl;
  h_corPt_mc_1->Add(h_corPt_mc_1_00,xs[0]/event[0]);
   std::cout << "test9_0_26" << std::endl;
  h_corPt_mc_2->Add(h_corPt_mc_2_00,xs[0]/event[0]);
   std::cout << "test9_0_27" << std::endl;
  h_corPt_mc_fat->Add(h_corPt_mc_fat_00,xs[0]/event[0]);
   std::cout << "test9_0_28" << std::endl;
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_00,xs[0]/event[0]);
   std::cout << "test9_0_29" << std::endl;
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_00,xs[0]/event[0]);
   std::cout << "test9_0_30" << std::endl;
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_00,xs[0]/event[0]);
   std::cout << "test9_0_31" << std::endl;
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_00,xs[0]/event[0]);
  std::cout << "test9_1" << std::endl;
  h_fCh_mc_fat->Add(h_fCh_mc_fat_00,xs[0]/event[0]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_00,xs[0]/event[0]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_00,xs[0]/event[0]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_00,xs[0]/event[0]);

  // _1

  h_DijetMass_mc->Add(h_DijetMass_mc_01,xs[1]/event[1]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_01,xs[1]/event[1]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_01,xs[1]/event[1]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_01,xs[1]/event[1]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_01,xs[1]/event[1]);
  h_EMF_mc->Add(h_EMF_mc_01,xs[1]/event[1]);
  h_DPhi_mc->Add(h_DPhi_mc_01,xs[1]/event[1]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_01,xs[1]/event[1]);
  h_Eta_mc->Add(h_Eta_mc_01,xs[1]/event[1]);
  h_Eta_mc_1->Add(h_Eta_mc_1_01,xs[1]/event[1]);
  h_Eta_mc_2->Add(h_Eta_mc_2_01,xs[1]/event[1]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_01,xs[1]/event[1]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_01,xs[1]/event[1]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_01,xs[1]/event[1]);
  h_DEta_mc->Add(h_DEta_mc_01,xs[1]/event[1]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_01,xs[1]/event[1]);
  h_Phi_mc->Add(h_Phi_mc_01,xs[1]/event[1]);
  h_Phi_mc_1->Add(h_Phi_mc_1_01,xs[1]/event[1]);
  h_Phi_mc_2->Add(h_Phi_mc_2_01,xs[1]/event[1]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_01,xs[1]/event[1]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_01,xs[1]/event[1]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_01,xs[1]/event[1]);
  h_n90hits_mc->Add(h_n90hits_mc_01,xs[1]/event[1]);
  h_fHPD_mc->Add(h_fHPD_mc_01,xs[1]/event[1]);
  h_corPt_mc->Add(h_corPt_mc_01,xs[1]/event[1]);
  h_corPt_mc_1->Add(h_corPt_mc_1_01,xs[1]/event[1]);
  h_corPt_mc_2->Add(h_corPt_mc_2_01,xs[1]/event[1]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_01,xs[1]/event[1]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_01,xs[1]/event[1]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_01,xs[1]/event[1]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_01,xs[1]/event[1]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_01,xs[1]/event[1]);

  h_fCh_mc_fat->Add(h_fCh_mc_fat_01,xs[1]/event[1]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_01,xs[1]/event[1]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_01,xs[1]/event[1]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_01,xs[1]/event[1]);

  std::cout << "test9_2" << std::endl;
  // _2

  h_DijetMass_mc->Add(h_DijetMass_mc_02,xs[2]/event[2]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_02,xs[2]/event[2]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_02,xs[2]/event[2]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_02,xs[2]/event[2]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_02,xs[2]/event[2]);
  h_EMF_mc->Add(h_EMF_mc_02,xs[2]/event[2]);
  h_DPhi_mc->Add(h_DPhi_mc_02,xs[2]/event[2]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_02,xs[2]/event[2]);
  h_Eta_mc->Add(h_Eta_mc_02,xs[2]/event[2]);
  h_Eta_mc_1->Add(h_Eta_mc_1_02,xs[2]/event[2]);
  h_Eta_mc_2->Add(h_Eta_mc_2_02,xs[2]/event[2]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_02,xs[2]/event[2]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_02,xs[2]/event[2]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_02,xs[2]/event[2]);
  h_DEta_mc->Add(h_DEta_mc_02,xs[2]/event[2]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_02,xs[2]/event[2]);
  h_Phi_mc->Add(h_Phi_mc_02,xs[2]/event[2]);
  h_Phi_mc_1->Add(h_Phi_mc_1_02,xs[2]/event[2]);
  h_Phi_mc_2->Add(h_Phi_mc_2_02,xs[2]/event[2]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_02,xs[2]/event[2]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_02,xs[2]/event[2]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_02,xs[2]/event[2]);
  h_n90hits_mc->Add(h_n90hits_mc_02,xs[2]/event[2]);
  h_fHPD_mc->Add(h_fHPD_mc_02,xs[2]/event[2]);
  h_corPt_mc->Add(h_corPt_mc_02,xs[2]/event[2]);
  h_corPt_mc_1->Add(h_corPt_mc_1_02,xs[2]/event[2]);
  h_corPt_mc_2->Add(h_corPt_mc_2_02,xs[2]/event[2]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_02,xs[2]/event[2]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_02,xs[2]/event[2]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_02,xs[2]/event[2]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_02,xs[2]/event[2]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_02,xs[2]/event[2]);

  h_fCh_mc_fat->Add(h_fCh_mc_fat_02,xs[2]/event[2]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_02,xs[2]/event[2]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_02,xs[2]/event[2]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_02,xs[2]/event[2]);
  std::cout << "test9_3" << std::endl;
  // _3

  h_DijetMass_mc->Add(h_DijetMass_mc_03,xs[3]/event[3]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_03,xs[3]/event[3]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_03,xs[3]/event[3]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_03,xs[3]/event[3]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_03,xs[3]/event[3]);
  h_EMF_mc->Add(h_EMF_mc_03,xs[3]/event[3]);
  h_DPhi_mc->Add(h_DPhi_mc_03,xs[3]/event[3]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_03,xs[3]/event[3]);
  h_Eta_mc->Add(h_Eta_mc_03,xs[3]/event[3]);
  h_Eta_mc_1->Add(h_Eta_mc_1_03,xs[3]/event[3]);
  h_Eta_mc_2->Add(h_Eta_mc_2_03,xs[3]/event[3]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_03,xs[3]/event[3]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_03,xs[3]/event[3]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_03,xs[3]/event[3]);
  h_DEta_mc->Add(h_DEta_mc_03,xs[3]/event[3]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_03,xs[3]/event[3]);
  h_Phi_mc->Add(h_Phi_mc_03,xs[3]/event[3]);
  h_Phi_mc_1->Add(h_Phi_mc_1_03,xs[3]/event[3]);
  h_Phi_mc_2->Add(h_Phi_mc_2_03,xs[3]/event[3]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_03,xs[3]/event[3]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_03,xs[3]/event[3]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_03,xs[3]/event[3]);
  h_n90hits_mc->Add(h_n90hits_mc_03,xs[3]/event[3]);
  h_fHPD_mc->Add(h_fHPD_mc_03,xs[3]/event[3]);
  h_corPt_mc->Add(h_corPt_mc_03,xs[3]/event[3]);
  h_corPt_mc_1->Add(h_corPt_mc_1_03,xs[3]/event[3]);
  h_corPt_mc_2->Add(h_corPt_mc_2_03,xs[3]/event[3]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_03,xs[3]/event[3]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_03,xs[3]/event[3]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_03,xs[3]/event[3]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_03,xs[3]/event[3]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_03,xs[3]/event[3]);

  h_fCh_mc_fat->Add(h_fCh_mc_fat_03,xs[3]/event[3]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_03,xs[3]/event[3]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_03,xs[3]/event[3]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_03,xs[3]/event[3]);
  std::cout << "test9_4" << std::endl;
  // _4
  
  h_DijetMass_mc->Add(h_DijetMass_mc_04,xs[4]/event[4]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_04,xs[4]/event[4]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_04,xs[4]/event[4]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_04,xs[4]/event[4]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_04,xs[4]/event[4]);
  h_EMF_mc->Add(h_EMF_mc_04,xs[4]/event[4]);
  h_DPhi_mc->Add(h_DPhi_mc_04,xs[4]/event[4]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_04,xs[4]/event[4]);
  h_Eta_mc->Add(h_Eta_mc_04,xs[4]/event[4]);
  h_Eta_mc_1->Add(h_Eta_mc_1_04,xs[4]/event[4]);
  h_Eta_mc_2->Add(h_Eta_mc_2_04,xs[4]/event[4]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_04,xs[4]/event[4]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_04,xs[4]/event[4]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_04,xs[4]/event[4]);
  h_DEta_mc->Add(h_DEta_mc_04,xs[4]/event[4]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_04,xs[4]/event[4]);
  h_Phi_mc->Add(h_Phi_mc_04,xs[4]/event[4]);
  h_Phi_mc_1->Add(h_Phi_mc_1_04,xs[4]/event[4]);
  h_Phi_mc_2->Add(h_Phi_mc_2_04,xs[4]/event[4]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_04,xs[4]/event[4]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_04,xs[4]/event[4]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_04,xs[4]/event[4]);
  h_n90hits_mc->Add(h_n90hits_mc_04,xs[4]/event[4]);
  h_fHPD_mc->Add(h_fHPD_mc_04,xs[4]/event[4]);
  h_corPt_mc->Add(h_corPt_mc_04,xs[4]/event[4]);
  h_corPt_mc_1->Add(h_corPt_mc_1_04,xs[4]/event[4]);
  h_corPt_mc_2->Add(h_corPt_mc_2_04,xs[4]/event[4]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_04,xs[4]/event[4]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_04,xs[4]/event[4]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_04,xs[4]/event[4]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_04,xs[4]/event[4]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_04,xs[4]/event[4]);


  h_fCh_mc_fat->Add(h_fCh_mc_fat_04,xs[4]/event[4]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_04,xs[4]/event[4]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_04,xs[4]/event[4]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_04,xs[4]/event[4]);
  std::cout << "test9_5" << std::endl;
  // _5
  

  h_DijetMass_mc->Add(h_DijetMass_mc_05,xs[5]/event[5]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_05,xs[5]/event[5]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_05,xs[5]/event[5]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_05,xs[5]/event[5]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_05,xs[5]/event[5]);
  h_EMF_mc->Add(h_EMF_mc_05,xs[5]/event[5]);
  h_DPhi_mc->Add(h_DPhi_mc_05,xs[5]/event[5]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_05,xs[5]/event[5]);
  h_Eta_mc->Add(h_Eta_mc_05,xs[5]/event[5]);
  h_Eta_mc_1->Add(h_Eta_mc_1_05,xs[5]/event[5]);
  h_Eta_mc_2->Add(h_Eta_mc_2_05,xs[5]/event[5]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_05,xs[5]/event[5]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_05,xs[5]/event[5]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_05,xs[5]/event[5]);
  h_DEta_mc->Add(h_DEta_mc_05,xs[5]/event[5]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_05,xs[5]/event[5]);
  h_Phi_mc->Add(h_Phi_mc_05,xs[5]/event[5]);
  h_Phi_mc_1->Add(h_Phi_mc_1_05,xs[5]/event[5]);
  h_Phi_mc_2->Add(h_Phi_mc_2_05,xs[5]/event[5]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_05,xs[5]/event[5]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_05,xs[5]/event[5]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_05,xs[5]/event[5]);
  h_n90hits_mc->Add(h_n90hits_mc_05,xs[5]/event[5]);
  h_fHPD_mc->Add(h_fHPD_mc_05,xs[5]/event[5]);
  h_corPt_mc->Add(h_corPt_mc_05,xs[5]/event[5]);
  h_corPt_mc_1->Add(h_corPt_mc_1_05,xs[5]/event[5]);
  h_corPt_mc_2->Add(h_corPt_mc_2_05,xs[5]/event[5]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_05,xs[5]/event[5]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_05,xs[5]/event[5]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_05,xs[5]/event[5]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_05,xs[5]/event[5]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_05,xs[5]/event[5]);

  h_fCh_mc_fat->Add(h_fCh_mc_fat_05,xs[5]/event[5]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_05,xs[5]/event[5]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_05,xs[5]/event[5]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_05,xs[5]/event[5]);
  std::cout << "test9_6" << std::endl;
  // _6

  h_DijetMass_mc->Add(h_DijetMass_mc_06,xs[6]/event[6]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_06,xs[6]/event[6]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_06,xs[6]/event[6]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_06,xs[6]/event[6]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_06,xs[6]/event[6]);
  h_EMF_mc->Add(h_EMF_mc_06,xs[6]/event[6]);
  h_DPhi_mc->Add(h_DPhi_mc_06,xs[6]/event[6]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_06,xs[6]/event[6]);
  h_Eta_mc->Add(h_Eta_mc_06,xs[6]/event[6]);
  h_Eta_mc_1->Add(h_Eta_mc_1_06,xs[6]/event[6]);
  h_Eta_mc_2->Add(h_Eta_mc_2_06,xs[6]/event[6]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_06,xs[6]/event[6]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_06,xs[6]/event[6]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_06,xs[6]/event[6]);
  h_DEta_mc->Add(h_DEta_mc_06,xs[6]/event[6]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_06,xs[6]/event[6]);
  h_Phi_mc->Add(h_Phi_mc_06,xs[6]/event[6]);
  h_Phi_mc_1->Add(h_Phi_mc_1_06,xs[6]/event[6]);
  h_Phi_mc_2->Add(h_Phi_mc_2_06,xs[6]/event[6]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_06,xs[6]/event[6]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_06,xs[6]/event[6]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_06,xs[6]/event[6]);
  h_n90hits_mc->Add(h_n90hits_mc_06,xs[6]/event[6]);
  h_fHPD_mc->Add(h_fHPD_mc_06,xs[6]/event[6]);
  h_corPt_mc->Add(h_corPt_mc_06,xs[6]/event[6]);
  h_corPt_mc_1->Add(h_corPt_mc_1_06,xs[6]/event[6]);
  h_corPt_mc_2->Add(h_corPt_mc_2_06,xs[6]/event[6]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_06,xs[6]/event[6]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_06,xs[6]/event[6]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_06,xs[6]/event[6]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_06,xs[6]/event[6]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_06,xs[6]/event[6]);

  h_fCh_mc_fat->Add(h_fCh_mc_fat_06,xs[6]/event[6]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_06,xs[6]/event[6]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_06,xs[6]/event[6]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_06,xs[6]/event[6]);
  std::cout << "test9_7" << std::endl;
  // _7

  h_DijetMass_mc->Add(h_DijetMass_mc_07,xs[7]/event[7]);
  h_DijetMass_mc_fat->Add(h_DijetMass_mc_fat_07,xs[7]/event[7]);
  h_DijetMass_MI_nPVe1_mc->Add(h_DijetMass_MI_nPVe1_mc_07,xs[7]/event[7]);
  h_DijetMass_MI_nPVg1_mc->Add(h_DijetMass_MI_nPVg1_mc_07,xs[7]/event[7]);
  h_MET_over_sumEt_mc->Add(h_MET_over_sumEt_mc_07,xs[7]/event[7]);
  h_EMF_mc->Add(h_EMF_mc_07,xs[7]/event[7]);
  h_DPhi_mc->Add(h_DPhi_mc_07,xs[7]/event[7]);
  h_DPhi_mc_fat->Add(h_DPhi_mc_fat_07,xs[7]/event[7]);
  h_Eta_mc->Add(h_Eta_mc_07,xs[7]/event[7]);
  h_Eta_mc_1->Add(h_Eta_mc_1_07,xs[7]/event[7]);
  h_Eta_mc_2->Add(h_Eta_mc_2_07,xs[7]/event[7]);
  h_Eta_mc_fat->Add(h_Eta_mc_fat_07,xs[7]/event[7]);
  h_Eta_mc_fat_1->Add(h_Eta_mc_fat_1_07,xs[7]/event[7]);
  h_Eta_mc_fat_2->Add(h_Eta_mc_fat_2_07,xs[7]/event[7]);
  h_DEta_mc->Add(h_DEta_mc_07,xs[7]/event[7]);
  h_DEta_mc_fat->Add(h_DEta_mc_fat_07,xs[7]/event[7]);
  h_Phi_mc->Add(h_Phi_mc_07,xs[7]/event[7]);
  h_Phi_mc_1->Add(h_Phi_mc_1_07,xs[7]/event[7]);
  h_Phi_mc_2->Add(h_Phi_mc_2_07,xs[7]/event[7]);
  h_Phi_mc_fat->Add(h_Phi_mc_fat_07,xs[7]/event[7]);
  h_Phi_mc_fat_1->Add(h_Phi_mc_fat_1_07,xs[7]/event[7]);
  h_Phi_mc_fat_2->Add(h_Phi_mc_fat_2_07,xs[7]/event[7]);
  h_n90hits_mc->Add(h_n90hits_mc_07,xs[7]/event[7]);
  h_fHPD_mc->Add(h_fHPD_mc_07,xs[7]/event[7]);
  h_corPt_mc->Add(h_corPt_mc_07,xs[7]/event[7]);
  h_corPt_mc_1->Add(h_corPt_mc_1_07,xs[7]/event[7]);
  h_corPt_mc_2->Add(h_corPt_mc_2_07,xs[7]/event[7]);
  h_corPt_mc_fat->Add(h_corPt_mc_fat_07,xs[7]/event[7]);
  h_corPt_mc_fat_1->Add(h_corPt_mc_fat_1_07,xs[7]/event[7]);
  h_corPt_mc_fat_2->Add(h_corPt_mc_fat_2_07,xs[7]/event[7]);
  h_nTrkVtx_mc->Add(h_nTrkVtx_mc_07,xs[7]/event[7]);
  h_nTrkCalo_mc->Add(h_nTrkCalo_mc_07,xs[7]/event[7]);
  
  h_fCh_mc_fat->Add(h_fCh_mc_fat_07,xs[7]/event[7]);
  h_fNh_mc_fat->Add(h_fNh_mc_fat_07,xs[7]/event[7]);
  h_fPh_mc_fat->Add(h_fPh_mc_fat_07,xs[7]/event[7]);
  h_fEl_mc_fat->Add(h_fEl_mc_fat_07,xs[7]/event[7]);

  std::cout << "test9_8" << std::endl;

  h_DijetMass_data->Sumw2();
  h_DijetMass_data_fat->Sumw2();
  h_DijetMass_MI_nPVe1_data->Sumw2();
  h_DijetMass_MI_nPVg1_data->Sumw2();
  h_MET_over_sumEt_data->Sumw2();
  h_EMF_data->Sumw2();
  h_DPhi_data->Sumw2();
  h_DPhi_data_fat->Sumw2();
  h_Eta_data->Sumw2();
  h_Eta_data_1->Sumw2();
  h_Eta_data_2->Sumw2();
  h_Eta_data_fat->Sumw2();
  h_Eta_data_fat_1->Sumw2();
  h_Eta_data_fat_2->Sumw2();
  h_DEta_data->Sumw2();
  h_DEta_data_fat->Sumw2();
  h_Phi_data->Sumw2();
  h_Phi_data_1->Sumw2();
  h_Phi_data_2->Sumw2();
  h_Phi_data_fat->Sumw2();
  h_Phi_data_fat_1->Sumw2();
  h_Phi_data_fat_2->Sumw2();
  h_n90hits_data->Sumw2();
  h_fHPD_data->Sumw2();
  h_corPt_data->Sumw2();
  h_corPt_data_1->Sumw2();
  h_corPt_data_2->Sumw2();
  h_corPt_data_fat->Sumw2();
  h_corPt_data_fat_1->Sumw2();
  h_corPt_data_fat_2->Sumw2();
  h_nTrkVtx_data->Sumw2();
  h_nTrkCalo_data->Sumw2();

   std::cout << "test10" << std::endl;

  float DijetMass_data_weight=h_DijetMass_data->Integral();
  float DijetMass_data_fat_weight=h_DijetMass_data_fat->Integral();
  float DijetMass_mc_weight=h_DijetMass_mc->Integral();
  float DijetMass_mc_fat_weight=h_DijetMass_mc_fat->Integral();
  float DijetMass_nPVe1_data_weight=h_DijetMass_MI_nPVe1_data->Integral();
  float DijetMass_nPVg1_data_weight=h_DijetMass_MI_nPVg1_data->Integral();
  float DijetMass_nPVe1_mc_weight=h_DijetMass_MI_nPVe1_mc->Integral();
  float DijetMass_nPVg1_mc_weight=h_DijetMass_MI_nPVg1_mc->Integral();
  float MET_over_sumEt_data_weight=h_MET_over_sumEt_data->Integral();
  float MET_over_sumEt_mc_weight=h_MET_over_sumEt_mc->Integral();
  float EMF_data_weight=h_EMF_data->Integral();
  float EMF_mc_weight=h_EMF_mc->Integral();
  float DPhi_data_weight=h_DPhi_data->Integral();
  float DPhi_data_fat_weight=h_DPhi_data_fat->Integral();
  float DPhi_mc_weight=h_DPhi_mc->Integral();
  float DPhi_mc_fat_weight=h_DPhi_mc_fat->Integral();
  float Eta_data_weight=h_Eta_data->Integral();
  float Eta_data_weight_1=h_Eta_data_1->Integral();
  float Eta_data_weight_2=h_Eta_data_2->Integral();
  float Eta_data_fat_weight=h_Eta_data_fat->Integral();
  float Eta_data_fat_weight_1=h_Eta_data_fat_1->Integral();
  float Eta_data_fat_weight_2=h_Eta_data_fat_2->Integral();
  float Eta_mc_weight=h_Eta_mc->Integral();
  float Eta_mc_weight_1=h_Eta_mc_1->Integral();
  float Eta_mc_weight_2=h_Eta_mc_2->Integral();
  float Eta_mc_fat_weight=h_Eta_mc_fat->Integral();
  float Eta_mc_fat_weight_1=h_Eta_mc_fat_1->Integral();
  float Eta_mc_fat_weight_2=h_Eta_mc_fat_2->Integral();
  float DEta_data_weight=h_Eta_data->Integral();
  float DEta_data_fat_weight=h_Eta_data_fat->Integral();
  float DEta_mc_weight=h_Eta_mc->Integral();
  float DEta_mc_fat_weight=h_Eta_mc_fat->Integral();
  float Phi_data_weight=h_Phi_data->Integral();
  float Phi_data_weight_1=h_Phi_data_1->Integral();
  float Phi_data_weight_2=h_Phi_data_2->Integral();
  float Phi_data_fat_weight=h_Phi_data_fat->Integral();
  float Phi_data_fat_weight_1=h_Phi_data_fat_1->Integral();
  float Phi_data_fat_weight_2=h_Phi_data_fat_2->Integral();
  float Phi_mc_weight=h_Phi_mc->Integral();
  float Phi_mc_weight_1=h_Phi_mc_1->Integral();
  float Phi_mc_weight_2=h_Phi_mc_2->Integral();
  float Phi_mc_fat_weight=h_Phi_mc_fat->Integral();
  float Phi_mc_fat_weight_1=h_Phi_mc_fat_1->Integral();
  float Phi_mc_fat_weight_2=h_Phi_mc_fat_2->Integral();
  float n90hits_data_weight=h_n90hits_data->Integral();
  float n90hits_mc_weight=h_n90hits_mc->Integral();
  float fHPD_data_weight=h_fHPD_data->Integral();
  float fHPD_mc_weight=h_fHPD_mc->Integral();
  float corPt_data_weight=h_corPt_data->Integral();
  float corPt_data_weight_1=h_corPt_data_1->Integral();
  float corPt_data_weight_2=h_corPt_data_2->Integral();
  float corPt_data_fat_weight=h_corPt_data_fat->Integral();
  float corPt_data_fat_weight_1=h_corPt_data_fat_1->Integral();
  float corPt_data_fat_weight_2=h_corPt_data_fat_2->Integral();
  float corPt_mc_weight=h_corPt_mc->Integral();
  float corPt_mc_weight_1=h_corPt_mc_1->Integral();
  float corPt_mc_weight_2=h_corPt_mc_2->Integral();
  float corPt_mc_fat_weight=h_corPt_mc_fat->Integral();
  float corPt_mc_fat_weight_1=h_corPt_mc_fat_1->Integral();
  float corPt_mc_fat_weight_2=h_corPt_mc_fat_2->Integral();
  float nTrkVtx_data_weight=h_nTrkVtx_data->Integral();
  float nTrkVtx_mc_weight=h_nTrkVtx_mc->Integral();
  float nTrkCalo_data_weight=h_nTrkCalo_data->Integral();
  float nTrkCalo_mc_weight=h_nTrkCalo_mc->Integral();


  float fCh_data_weight = h_fCh_data_fat->Integral();
  float fCh_mc_weight = h_fCh_mc_fat->Integral();
  float fNh_data_weight = h_fNh_data_fat->Integral();
  float fNh_mc_weight = h_fNh_mc_fat->Integral();
  float fPh_data_weight = h_fPh_data_fat->Integral();
  float fPh_mc_weight = h_fPh_mc_fat->Integral();
  float fEl_data_weight = h_fEl_data_fat->Integral();
  float fEl_mc_weight = h_fEl_mc_fat->Integral();


  //  float JCFCalo_data_weight=h_JCFCalo_data->Integral();
  //  float JCFCalo_mc_weight=h_JCFCalo_mc->Integral();

  cout << "Weights of mass distri = " << DijetMass_data_weight << endl;

  string data_info("CMS 2011 Data Fat PF ( 358 pb^{-1} )");



   std::cout << "test11" << std::endl;

  // Dijet Mass distribution

  c_DijetMass->cd(0);
  h_DijetMass_data->SetTitle("");
  h_DijetMass_data->SetLineWidth(3);
  h_DijetMass_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_data->SetMarkerStyle(20);

  h_DijetMass_mc->SetTitle("");
  h_DijetMass_mc->SetLineWidth(3);
  h_DijetMass_mc->SetLineColor(42);
  h_DijetMass_mc->SetFillColor(42);
  h_DijetMass_mc->GetYaxis()->SetTitle("Events");
  h_DijetMass_mc->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_mc->Scale(DijetMass_data_weight/DijetMass_mc_weight);

  h_DijetMass_mc->Draw("");
  h_DijetMass_data->Draw("sameEP");

  l_DijetMass->SetFillColor(0);
  l_DijetMass->AddEntry( h_DijetMass_data,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_DijetMass->AddEntry( h_DijetMass_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DijetMass->Draw("same");

  c_DijetMass->Update();
  outf->Append(c_DijetMass);

  // Dijet Mass distribution (PF)
   std::cout << "test11_1" << std::endl;
  c_DijetMass_fat->cd(0);
  h_DijetMass_data_fat->SetTitle("");
  h_DijetMass_data_fat->SetLineWidth(3);
  h_DijetMass_data_fat->GetYaxis()->SetTitle("Events");
  h_DijetMass_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data_fat->GetXaxis()->SetTitle("M_{12} Fat / GeV");
  h_DijetMass_data_fat->SetMarkerStyle(20);

  h_DijetMass_mc_fat->SetTitle("");
  h_DijetMass_mc_fat->SetLineWidth(3);
  h_DijetMass_mc_fat->SetLineColor(42);
  h_DijetMass_mc_fat->SetFillColor(42);
  h_DijetMass_mc_fat->GetYaxis()->SetTitle("Events");
  h_DijetMass_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc_fat->GetXaxis()->SetTitle("M_{12} Fat / GeV");
  h_DijetMass_mc_fat->Scale(DijetMass_data_fat_weight/DijetMass_mc_fat_weight);

  h_DijetMass_mc_fat->Draw("");
  h_DijetMass_data_fat->Draw("sameEP");

  l_DijetMass_fat->SetFillColor(0);
  l_DijetMass_fat->AddEntry( h_DijetMass_data_fat, data_info.c_str(),"p");
  l_DijetMass_fat->AddEntry( h_DijetMass_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DijetMass_fat->Draw("same");

  c_DijetMass_fat->Update();
  outf->Append(c_DijetMass_fat);


  // Dijet Mass distribution (log)

  c_DijetMass_log->cd(0);

  TH1F *h_DijetMass_data_log                    =(TH1F*)h_DijetMass_data->Clone();
  TH1F *h_DijetMass_mc_log                      =(TH1F*)h_DijetMass_mc->Clone();

  c_DijetMass_log->SetLogy();

  h_DijetMass_mc_log->SetMinimum(0.2);

  h_DijetMass_mc_log->Draw("");
  h_DijetMass_data_log->Draw("sameEP");

  l_DijetMass_log->SetFillColor(0);
  l_DijetMass_log->AddEntry((TObject*)0,"2011 data","");
  l_DijetMass_log->AddEntry( h_DijetMass_data_log,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_DijetMass_log->AddEntry( h_DijetMass_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DijetMass_log->Draw("same");

  c_DijetMass_log->Update();
  outf->Append(c_DijetMass_log);

  c_DijetMass_fat_log->cd(0);

  TH1F *h_DijetMass_data_fat_log                    =(TH1F*)h_DijetMass_data_fat->Clone();
  TH1F *h_DijetMass_mc_fat_log                      =(TH1F*)h_DijetMass_mc_fat->Clone();

  c_DijetMass_fat_log->SetLogy();

  h_DijetMass_mc_fat_log->SetMinimum(0.2);

  h_DijetMass_mc_fat_log->Draw("");
  h_DijetMass_data_fat_log->Draw("sameEP");

  l_DijetMass_fat_log->SetFillColor(0);
  l_DijetMass_fat_log->AddEntry((TObject*)0,"2011 data","");
  l_DijetMass_fat_log->AddEntry( h_DijetMass_data_fat_log, data_info.c_str(),"p");
  l_DijetMass_fat_log->AddEntry( h_DijetMass_mc_fat_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DijetMass_fat_log->Draw("same");

  c_DijetMass_fat_log->Update();
  outf->Append(c_DijetMass_fat_log);

  c_DijetMass_ratio->cd(0);

  //  TH1F h_DijetMass_mc_clone_for_ratio=h_DijetMass_mc->clone();

  //  h_DijetMass_mc_clone_for_ratio->Sumw2();

  h_DijetMass_ratio->Divide(h_DijetMass_data,h_DijetMass_mc,1.0,1.0,"b");

  for(int ratio_check=1 ; ratio_check <= 83 ; ratio_check++) {
    float ratio_data = h_DijetMass_data->GetBinContent(ratio_check);
    float ratio_mc = h_DijetMass_mc->GetBinContent(ratio_check);
    float ratio_data_over_mc = h_DijetMass_data->GetBinContent(ratio_check);
    if (ratio_mc == 0 || ratio_data == 0) continue;
    //    std::cout << "Data bin = " << ratio_check << ", bincontent = " << ratio_data << ", binerror = " << pow(ratio_data,0.5) << ", mc bin = " << ratio_check << ", bincontent = " << ratio_mc << ", binerror = " << pow(ratio_mc,0.5) << ", expected bin error = " << ratio_data/ratio_mc*pow(1/ratio_data+1/ratio_mc,0.5)<<std::endl;
    //    std::cout << "bin = " << ratio_check << ", GetBinError = " << h_DijetMass_ratio->GetBinError(ratio_check) << std::endl;
    h_DijetMass_ratio->SetBinError(ratio_check,ratio_data/ratio_mc*pow(pow(1/pow(ratio_data,0.5),2)+pow(1/pow(ratio_mc,0.5),2),0.5));
    //    std::cout << "bin = " << ratio_check << ", GetBinError = " << h_DijetMass_ratio->GetBinError(ratio_check) << std::endl;
  }


  h_DijetMass_ratio->SetTitle("");
  h_DijetMass_ratio->SetLineWidth(3);
  h_DijetMass_ratio->GetYaxis()->SetTitle("Data/MC");
  h_DijetMass_ratio->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_ratio->GetXaxis()->SetTitle("M_{12} / GeV");
  h_DijetMass_ratio->SetMarkerStyle(20);
  h_DijetMass_ratio->SetMinimum(0.0);

  h_DijetMass_ratio->Draw("EP");

  TText *t_DijetMass_ratio_1 = pt_DijetMass_ratio->AddText("CMS 2011 Data ( 201.348pb^{-1} )");

  pt_DijetMass_ratio->Draw("sames");

  c_DijetMass_ratio->Update();
  outf->Append(c_DijetMass_ratio);


  c_DijetMass_ratio_fat->cd(0);

  //  TH1F h_DijetMass_mc_clone_for_ratio=h_DijetMass_mc->clone();

  //  h_DijetMass_mc_clone_for_ratio->Sumw2();

  h_DijetMass_ratio_fat->Divide(h_DijetMass_data_fat,h_DijetMass_mc_fat,1.0,1.0,"b");

  for(int ratio_check=1 ; ratio_check <= 83 ; ratio_check++) {
    float ratio_data = h_DijetMass_data_fat->GetBinContent(ratio_check);
    float ratio_mc = h_DijetMass_mc_fat->GetBinContent(ratio_check);
    float ratio_data_over_mc = h_DijetMass_data_fat->GetBinContent(ratio_check);
    if (ratio_mc == 0 || ratio_data == 0) continue;
    //    std::cout << "Data bin = " << ratio_check << ", bincontent = " << ratio_data << ", binerror = " << pow(ratio_data,0.5) << ", mc bin = " << ratio_check << ", bincontent = " << ratio_mc << ", binerror = " << pow(ratio_mc,0.5) << ", expected bin error = " << ratio_data/ratio_mc*pow(1/ratio_data+1/ratio_mc,0.5)<<std::endl;
    //    std::cout << "bin = " << ratio_check << ", GetBinError = " << h_DijetMass_ratio->GetBinError(ratio_check) << std::endl;
    h_DijetMass_ratio_fat->SetBinError(ratio_check,ratio_data/ratio_mc*pow(pow(1/pow(ratio_data,0.5),2)+pow(1/pow(ratio_mc,0.5),2),0.5));
    //    std::cout << "bin = " << ratio_check << ", GetBinError = " << h_DijetMass_ratio->GetBinError(ratio_check) << std::endl;
  }


  h_DijetMass_ratio_fat->SetTitle("");
  h_DijetMass_ratio_fat->SetLineWidth(3);
  h_DijetMass_ratio_fat->GetYaxis()->SetTitle("Data/MC");
  h_DijetMass_ratio_fat->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_ratio_fat->GetXaxis()->SetTitle("M_{12} Fat / GeV");
  h_DijetMass_ratio_fat->SetMarkerStyle(20);
  h_DijetMass_ratio_fat->SetMinimum(0.0);

  h_DijetMass_ratio_fat->Draw("EP");

  TText *t_DijetMass_ratio_fat_1 = pt_DijetMass_ratio_fat->AddText(data_info.c_str());

  pt_DijetMass_ratio_fat->Draw("sames");

  c_DijetMass_ratio_fat->Update();
  outf->Append(c_DijetMass_ratio_fat);


  // Dijet Mass Ratio (Data/MC) Calo (log)
  std::cout << "test11_9" << std::endl;
  c_DijetMass_ratio_log->cd(0);

  TH1F *h_DijetMass_ratio_log                   =(TH1F*)h_DijetMass_ratio->Clone();

  c_DijetMass_ratio_log->SetLogy();

  h_DijetMass_ratio_log->SetMinimum(0.5);
  h_DijetMass_ratio_log->Draw("EP");


  TText *t_DijetMass_ratio_log_1 = pt_DijetMass_ratio_log->AddText("CMS 2011 Data ( 201.348pb^{-1} )");

  pt_DijetMass_ratio_log->Draw("sames");


  c_DijetMass_ratio_log->Update();
  outf->Append(c_DijetMass_ratio_log);


  // Dijet Mass Ratio (Data/MC) PF (log)

  c_DijetMass_ratio_fat_log->cd(0);

  TH1F *h_DijetMass_ratio_fat_log                   =(TH1F*)h_DijetMass_ratio_fat->Clone();

  c_DijetMass_ratio_fat_log->SetLogy();

  h_DijetMass_ratio_fat_log->SetMinimum(0.5);
  h_DijetMass_ratio_fat_log->Draw("EP");


  TText *t_DijetMass_ratio_fat_log_1 = pt_DijetMass_ratio_fat_log->AddText(data_info.c_str());

  pt_DijetMass_ratio_fat_log->Draw("sames");

  std::cout << "test12" << std::endl;
  c_DijetMass_ratio_fat_log->Update();
  outf->Append(c_DijetMass_ratio_fat_log);

  // Dijet Mass Distribution (comparison b/w nPV=0 and nPV>1, Data vs MC)

  c_DijetMass_MI->cd(0);
  h_DijetMass_MI_nPVe1_data->SetTitle("");
  h_DijetMass_MI_nPVe1_data->SetLineWidth(3);
  h_DijetMass_MI_nPVe1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVe1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVe1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVe1_data->SetMarkerStyle(20);

  h_DijetMass_MI_nPVg1_data->SetTitle("");
  h_DijetMass_MI_nPVg1_data->SetLineWidth(1);
  h_DijetMass_MI_nPVg1_data->SetLineColor(2);
  h_DijetMass_MI_nPVg1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVg1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVg1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVg1_data->SetMarkerStyle(20);
  h_DijetMass_MI_nPVg1_data->SetMarkerColor(2);

  h_DijetMass_MI_nPVe1_mc->SetTitle("");
  h_DijetMass_MI_nPVe1_mc->SetLineWidth(3);
  h_DijetMass_MI_nPVe1_mc->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVe1_mc->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVe1_mc->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVe1_mc->Scale(DijetMass_nPVe1_data_weight/DijetMass_nPVe1_mc_weight);

  h_DijetMass_MI_nPVg1_mc->SetTitle("");
  h_DijetMass_MI_nPVg1_mc->SetLineWidth(3);
  h_DijetMass_MI_nPVg1_mc->SetLineColor(2);
  h_DijetMass_MI_nPVg1_mc->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVg1_mc->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVg1_mc->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVg1_mc->Scale(DijetMass_nPVg1_data_weight/DijetMass_nPVg1_mc_weight);


  h_DijetMass_MI_nPVe1_data->Draw("EP");
  h_DijetMass_MI_nPVg1_data->Draw("sameEP");
  h_DijetMass_MI_nPVe1_mc->Draw("same");
  h_DijetMass_MI_nPVg1_mc->Draw("same");
  l_DijetMass_MI->SetFillColor(0);
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVe1_data,"CMS 2011 Data ( 201.348pb^{-1} ) with 1 primary vertex","p");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVg1_data,"CMS 2011 Data ( 201.348pb^{-1} ) with more than 1 primary vertex","p");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVe1_mc,"QCD  PYTHIA6 Z2 + CMS Simulation with 1 primary vertex","f");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVg1_mc,"QCD  PYTHIA6 Z2 + CMS Simulation with more than 1 primary vertex","f");
  l_DijetMass_MI->Draw("same");

  c_DijetMass_MI->Update();
  outf->Append(c_DijetMass_MI);

  // Dijet Mass Distribution (comparison b/w nPV=0 and nPV>1, Data vs MC) (log)

  c_DijetMass_MI_log->cd(0);

  TH1F *h_DijetMass_MI_nPVe1_data_log           =(TH1F*)h_DijetMass_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_MI_nPVg1_data_log           =(TH1F*)h_DijetMass_MI_nPVg1_data->Clone();
  TH1F *h_DijetMass_MI_nPVe1_mc_log             =(TH1F*)h_DijetMass_MI_nPVe1_mc->Clone();
  TH1F *h_DijetMass_MI_nPVg1_mc_log             =(TH1F*)h_DijetMass_MI_nPVg1_mc->Clone();

  c_DijetMass_MI_log->SetLogy();

  h_DijetMass_MI_nPVe1_data_log->SetMinimum(0.5);

  h_DijetMass_MI_nPVe1_data_log->Draw("EP");
  h_DijetMass_MI_nPVg1_data_log->Draw("sameEP");
  h_DijetMass_MI_nPVe1_mc_log->Draw("same");
  h_DijetMass_MI_nPVg1_mc_log->Draw("same");
  l_DijetMass_MI_log->SetFillColor(0);
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVe1_data_log,"CMS 2011 Data ( 201.348pb^{-1} ) with 1 primary vertex","p");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVg1_data_log,"CMS 2011 Data ( 201.348pb^{-1} ) with more than 1 primary vertex","p");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVe1_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation with 1 primary vertex","f");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVg1_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation with more than 1 primary vertex","f");
  l_DijetMass_MI_log->Draw("same");

  c_DijetMass_MI_log->Update();
  outf->Append(c_DijetMass_MI_log);

  // Dijet Mass Distribution (comparison b/w nPV=0 and nPV>1)

  c_DijetMass_normalized_MI->cd(0);
  h_DijetMass_normalized_MI_nPVe1_data->SetTitle("");
  h_DijetMass_normalized_MI_nPVe1_data->SetLineWidth(3);
  h_DijetMass_normalized_MI_nPVe1_data->SetLineColor(42);
  h_DijetMass_normalized_MI_nPVe1_data->SetFillColor(42);
  h_DijetMass_normalized_MI_nPVe1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_normalized_MI_nPVe1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_normalized_MI_nPVe1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_normalized_MI_nPVe1_data->Scale(DijetMass_nPVg1_data_weight/DijetMass_nPVe1_data_weight);

  h_DijetMass_normalized_MI_nPVg1_data->SetTitle("");
  h_DijetMass_normalized_MI_nPVg1_data->SetLineWidth(3);
  h_DijetMass_normalized_MI_nPVg1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_normalized_MI_nPVg1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_normalized_MI_nPVg1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_normalized_MI_nPVg1_data->SetMarkerStyle(20);


  h_DijetMass_normalized_MI_nPVe1_data->Draw("");
  h_DijetMass_normalized_MI_nPVg1_data->Draw("sameEP");
  l_DijetMass_normalized_MI->SetFillColor(0);
  l_DijetMass_normalized_MI->AddEntry( h_DijetMass_normalized_MI_nPVe1_data,"CMS 2011 Data ( 201.348pb^{-1} ) with 1 primary vertex","f");
  l_DijetMass_normalized_MI->AddEntry( h_DijetMass_normalized_MI_nPVg1_data,"CMS 2011 Data ( 201.348pb^{-1} ) with morethan 1 primary vertex","p");
  l_DijetMass_normalized_MI->Draw("same");
  
  c_DijetMass_normalized_MI->Update();
  outf->Append(c_DijetMass_normalized_MI);

  // Dijet Mass Distribution (comparison b/w nPV=0 and nPV>1) (log)

  std::cout << "test13" << std::endl;

  c_DijetMass_normalized_MI_log->cd(0);

  TH1F *h_DijetMass_normalized_MI_nPVe1_data_log=(TH1F*)h_DijetMass_normalized_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_normalized_MI_nPVg1_data_log=(TH1F*)h_DijetMass_normalized_MI_nPVg1_data->Clone();

  c_DijetMass_normalized_MI_log->SetLogy();

  h_DijetMass_normalized_MI_nPVe1_data_log->SetMinimum(0.5);

  h_DijetMass_normalized_MI_nPVe1_data_log->Draw("");
  h_DijetMass_normalized_MI_nPVg1_data_log->Draw("sameEP");
  l_DijetMass_normalized_MI_log->SetFillColor(0);
  l_DijetMass_normalized_MI_log->AddEntry( h_DijetMass_normalized_MI_nPVe1_data_log,"CMS 2011 Data ( 201.348pb^{-1} ) with 1 primary vertex","f");
  l_DijetMass_normalized_MI_log->AddEntry( h_DijetMass_normalized_MI_nPVg1_data_log,"CMS 2011 Data ( 201.348pb^{-1} ) with morethan 1 primary vertex","p");
  l_DijetMass_normalized_MI_log->Draw("same");

  c_DijetMass_normalized_MI_log->Update();
  outf->Append(c_DijetMass_normalized_MI_log);

  // Eta_Phi_scatter for two leading jets

   std::cout << "test13_2" << std::endl;

  c_Eta_Phi_Scatter->cd(0);
  /*
   std::cout << "test13_2_1" << std::endl;
   TPaletteAxis *palette = new TPaletteAxis(3.079823,-3.213588,3.345898,3.186412,h_Eta_Phi_Scatter_data)
   std::cout << "test13_2_2" << std::endl;
  palette->SetLabelColor(1);
   std::cout << "test13_2_3" << std::endl;
  palette->SetLabelFont(42);
   std::cout << "test13_2_4" << std::endl;
  palette->SetLabelOffset(0.007);
   std::cout << "test13_2_5" << std::endl;
  palette->SetLabelSize(0.04);
   std::cout << "test13_2_6" << std::endl;
  palette->SetTitleOffset(1);
   std::cout << "test13_2_7" << std::endl;
  palette->SetTitleSize(0.06);
   std::cout << "test13_2_8" << std::endl;
  palette->SetFillColor(100);
   std::cout << "test13_2_9" << std::endl;
  palette->SetFillStyle(1001);
   std::cout << "test13_3" << std::endl;
  h_Eta_Phi_Scatter_data->GetListOfFunctions()->Add(palette,"br");
  */
  h_Eta_Phi_Scatter_data->SetTitle("");
  h_Eta_Phi_Scatter_data->SetMarkerStyle(20);
  h_Eta_Phi_Scatter_data->SetMarkerSize(0.2);
  h_Eta_Phi_Scatter_data->GetYaxis()->SetTitle("#phi");
  h_Eta_Phi_Scatter_data->GetXaxis()->SetTitle("#eta");
  h_Eta_Phi_Scatter_data->Draw("COLZ");
   std::cout << "test13_4" << std::endl;
  pt_Eta_Phi_Scatter->SetBorderSize(0);
  pt_Eta_Phi_Scatter->SetFillColor(0);

  TText *t_Eta_Phi_Scatter = pt_Eta_Phi_Scatter->AddText("CMS 2011 Data ( 201.348pb^{-1} )");

  pt_Eta_Phi_Scatter->Draw("sames");

  c_Eta_Phi_Scatter->Update();
  outf->Append(h_Eta_Phi_Scatter_data);

  // Eta_Phi_scatter for first leading jets

  c_Eta_Phi_Scatter_1->cd(0);
  std::cout << "check point 1" << std::endl;
  TPaletteAxis *palette = new TPaletteAxis(3.079823,-3.213588,3.345898,3.186412,h_Eta_Phi_Scatter_data_1);
  std::cout << "check point 2" << std::endl;
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.007);
  palette->SetLabelSize(0.04);
  palette->SetTitleOffset(1);
  palette->SetTitleSize(0.06);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  h_Eta_Phi_Scatter_data_1->GetListOfFunctions()->Add(palette,"br");
  h_Eta_Phi_Scatter_data_1->SetTitle("");
  h_Eta_Phi_Scatter_data_1->SetMarkerStyle(20);
  h_Eta_Phi_Scatter_data_1->SetMarkerSize(0.2);
  h_Eta_Phi_Scatter_data_1->GetYaxis()->SetTitle("#phi");
  h_Eta_Phi_Scatter_data_1->GetXaxis()->SetTitle("#eta");
  h_Eta_Phi_Scatter_data_1->Draw("COLZ");

  pt_Eta_Phi_Scatter_1->SetBorderSize(0);
  pt_Eta_Phi_Scatter_1->SetFillColor(0);

  TText *t_Eta_Phi_Scatter_1 = pt_Eta_Phi_Scatter_1->AddText("CMS 2011 Data ( 201.348pb^{-1} )");

  pt_Eta_Phi_Scatter_1->Draw("sames");

  c_Eta_Phi_Scatter_1->Update();
  outf->Append(h_Eta_Phi_Scatter_data_1);


  // Eta_Phi_scatter for second leading jets

  c_Eta_Phi_Scatter_2->cd(0);

  TPaletteAxis *palette = new TPaletteAxis(3.079823,-3.213588,3.345898,3.186412,h_Eta_Phi_Scatter_data_2);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.007);
  palette->SetLabelSize(0.04);
  palette->SetTitleOffset(1);
  palette->SetTitleSize(0.06);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  h_Eta_Phi_Scatter_data_2->GetListOfFunctions()->Add(palette,"br");
  h_Eta_Phi_Scatter_data_2->SetTitle("");
  h_Eta_Phi_Scatter_data_2->SetMarkerStyle(20);
  h_Eta_Phi_Scatter_data_2->SetMarkerSize(0.2);
  h_Eta_Phi_Scatter_data_2->GetYaxis()->SetTitle("#phi");
  h_Eta_Phi_Scatter_data_2->GetXaxis()->SetTitle("#eta");
  h_Eta_Phi_Scatter_data_2->Draw("COLZ");

  pt_Eta_Phi_Scatter_2->SetBorderSize(0);
  pt_Eta_Phi_Scatter_2->SetFillColor(0);

  TText *t_Eta_Phi_Scatter_2 = pt_Eta_Phi_Scatter_2->AddText("CMS 2011 Data ( 201.348pb^{-1} )");

  pt_Eta_Phi_Scatter_2->Draw("sames");

  c_Eta_Phi_Scatter_2->Update();
  outf->Append(h_Eta_Phi_Scatter_data_2);

  // MET/SemET

  c_MET_over_sumEt->cd(0);
  h_MET_over_sumEt_data->SetTitle("");
  h_MET_over_sumEt_data->GetYaxis()->SetTitle("Events");
  h_MET_over_sumEt_data->GetYaxis()->SetLabelSize(0.03);
  h_MET_over_sumEt_data->GetXaxis()->SetTitle("#slash{E}_{T}/#sum_{}E_{T}");
  h_MET_over_sumEt_data->SetMarkerStyle(20);


  h_MET_over_sumEt_mc->SetLineWidth(3);
  h_MET_over_sumEt_mc->SetTitle("");
  h_MET_over_sumEt_mc->SetFillColor(42);
  h_MET_over_sumEt_mc->SetLineColor(42);
  h_MET_over_sumEt_mc->GetYaxis()->SetTitle("Events");
  h_MET_over_sumEt_mc->GetYaxis()->SetLabelSize(0.03);
  h_MET_over_sumEt_mc->GetXaxis()->SetTitle("#slash{E}_{T}/#sum_{}E_{T}");
  h_MET_over_sumEt_mc->Scale(MET_over_sumEt_data_weight/MET_over_sumEt_mc_weight);

  h_MET_over_sumEt_mc->Draw("");
  h_MET_over_sumEt_data->Draw("sameEP");

  l_MET_over_sumEt->SetFillColor(0);
  l_MET_over_sumEt->AddEntry(h_MET_over_sumEt_data,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_MET_over_sumEt->AddEntry(h_MET_over_sumEt_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_MET_over_sumEt->Draw("sames");

  c_MET_over_sumEt->Update();
  outf->Append(c_MET_over_sumEt);

  // MET/SemET (log)

  std::cout << "test13_5" << std::endl;

  c_MET_over_sumEt_log->cd(0);

  TH1F *h_MET_over_sumEt_data_log               =(TH1F*)h_MET_over_sumEt_data->Clone();
  TH1F *h_MET_over_sumEt_mc_log                 =(TH1F*)h_MET_over_sumEt_mc->Clone();

  c_MET_over_sumEt_log->SetLogy();

  h_MET_over_sumEt_mc_log->SetMinimum(0.5);

  h_MET_over_sumEt_mc_log->Draw("");
  h_MET_over_sumEt_data_log->Draw("sameEP");

  l_MET_over_sumEt_log->SetFillColor(0);
  l_MET_over_sumEt_log->AddEntry(h_MET_over_sumEt_data_log,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_MET_over_sumEt_log->AddEntry(h_MET_over_sumEt_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_MET_over_sumEt_log->Draw("sames");

  c_MET_over_sumEt_log->Update();
  outf->Append(c_MET_over_sumEt_log);

  // EMF

  c_EMF->cd(0);
  h_EMF_data->SetTitle("");
  h_EMF_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_EMF_data->GetYaxis()->SetLabelSize(0.03);
  h_EMF_data->GetXaxis()->SetTitle("Jet EMF");
  h_EMF_data->SetMarkerStyle(20);
  h_EMF_mc->SetLineWidth(3);
  h_EMF_mc->SetTitle("");
  h_EMF_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_EMF_mc->GetYaxis()->SetLabelSize(0.03);
  h_EMF_mc->GetXaxis()->SetTitle("Jet EMF");
  h_EMF_mc->SetFillColor(42);
  h_EMF_mc->SetLineColor(42);
  h_EMF_mc->Scale(EMF_data_weight/EMF_mc_weight);

  h_EMF_mc->Draw("");
  h_EMF_data->Draw("sameEP");

  l_EMF->SetFillColor(0);
  l_EMF->AddEntry(h_EMF_data,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_EMF->AddEntry(h_EMF_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_EMF->Draw("sames");

  c_EMF->Update();
  outf->Append(c_EMF);




  c_fCh->cd(0);
  h_fCh_data_fat->SetTitle("");
  h_fCh_data_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fCh_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fCh_data_fat->GetXaxis()->SetTitle("PF Jet Ch frac");
  h_fCh_data_fat->SetMarkerStyle(20);
  h_fCh_mc_fat->SetLineWidth(3);
  h_fCh_mc_fat->SetTitle("");
  h_fCh_mc_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fCh_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fCh_mc_fat->GetXaxis()->SetTitle("PF Jet Ch frac");
  h_fCh_mc_fat->SetFillColor(42);
  h_fCh_mc_fat->SetLineColor(42);
  h_fCh_mc_fat->Scale(fCh_data_weight/fCh_mc_weight);

  h_fCh_mc_fat->Draw("");
  h_fCh_data_fat->Draw("sameEP");

  l_fCh->SetFillColor(0);
  l_fCh->AddEntry(h_fCh_data_fat,data_info.c_str(),"p");
  l_fCh->AddEntry(h_fCh_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fCh->Draw("sames");

  c_fCh->Update();
  outf->Append(c_fCh);




  c_fNh->cd(0);
  h_fNh_data_fat->SetTitle("");
  h_fNh_data_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fNh_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fNh_data_fat->GetXaxis()->SetTitle("PF Jet Nh frac");
  h_fNh_data_fat->SetMarkerStyle(20);
  h_fNh_mc_fat->SetLineWidth(3);
  h_fNh_mc_fat->SetTitle("");
  h_fNh_mc_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fNh_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fNh_mc_fat->GetXaxis()->SetTitle("PF Jet Nh frac");
  h_fNh_mc_fat->SetFillColor(42);
  h_fNh_mc_fat->SetLineColor(42);
  h_fNh_mc_fat->Scale(fNh_data_weight/fNh_mc_weight);

  h_fNh_mc_fat->Draw("");
  h_fNh_data_fat->Draw("sameEP");

  l_fNh->SetFillColor(0);
  l_fNh->AddEntry(h_fNh_data_fat,data_info.c_str(),"p");
  l_fNh->AddEntry(h_fNh_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fNh->Draw("sames");

  c_fNh->Update();
  outf->Append(c_fNh);


  c_fPh->cd(0);
  h_fPh_data_fat->SetTitle("");
  h_fPh_data_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fPh_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fPh_data_fat->GetXaxis()->SetTitle("PF Jet Ph frac");
  h_fPh_data_fat->SetMarkerStyle(20);
  h_fPh_mc_fat->SetLineWidth(3);
  h_fPh_mc_fat->SetTitle("");
  h_fPh_mc_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fPh_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fPh_mc_fat->GetXaxis()->SetTitle("PF Jet Ph frac");
  h_fPh_mc_fat->SetFillColor(42);
  h_fPh_mc_fat->SetLineColor(42);
  h_fPh_mc_fat->Scale(fPh_data_weight/fPh_mc_weight);

  h_fPh_mc_fat->Draw("");
  h_fPh_data_fat->Draw("sameEP");

  l_fPh->SetFillColor(0);
  l_fPh->AddEntry(h_fPh_data_fat,data_info.c_str(),"p");
  l_fPh->AddEntry(h_fPh_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fPh->Draw("sames");

  c_fPh->Update();
  outf->Append(c_fPh);


  c_fEl->cd(0);
  h_fEl_data_fat->SetTitle("");
  h_fEl_data_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fEl_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fEl_data_fat->GetXaxis()->SetTitle("PF Jet El frac");
  h_fEl_data_fat->SetMarkerStyle(20);
  h_fEl_mc_fat->SetLineWidth(3);
  h_fEl_mc_fat->SetTitle("");
  h_fEl_mc_fat->GetYaxis()->SetTitle("PF Jets(two leading jets)");
  h_fEl_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fEl_mc_fat->GetXaxis()->SetTitle("PF Jet El frac");
  h_fEl_mc_fat->SetFillColor(42);
  h_fEl_mc_fat->SetLineColor(42);
  h_fEl_mc_fat->Scale(fEl_data_weight/fEl_mc_weight);

  h_fEl_mc_fat->Draw("");
  h_fEl_data_fat->Draw("sameEP");

  l_fEl->SetFillColor(0);
  l_fEl->AddEntry(h_fEl_data_fat,data_info.c_str(),"p");
  l_fEl->AddEntry(h_fEl_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fEl->Draw("sames");

  c_fEl->Update();
  outf->Append(c_fEl);










  // DPhi

  c_DPhi->cd(0);
  h_DPhi_data->SetTitle("");
  h_DPhi_data->GetYaxis()->SetTitle("Events");
  h_DPhi_data->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_data->GetXaxis()->SetTitle("#Delta #phi");
  h_DPhi_data->SetMarkerStyle(20);

  h_DPhi_mc->SetLineWidth(3);
  h_DPhi_mc->SetTitle("");
  h_DPhi_mc->SetFillColor(42);
  h_DPhi_mc->SetLineColor(42);
  h_DPhi_mc->GetYaxis()->SetTitle("events");
  h_DPhi_mc->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_mc->GetXaxis()->SetTitle("#Delta #phi");
  h_DPhi_mc->Scale(DPhi_data_weight/DPhi_mc_weight);

  h_DPhi_mc->Draw("");
  h_DPhi_data->Draw("sameEP");

  l_DPhi->SetFillColor(0);
  l_DPhi->AddEntry(h_DPhi_data,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_DPhi->AddEntry(h_DPhi_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DPhi->Draw("sames");

  c_DPhi->Update();
  outf->Append(c_DPhi);

  std::cout << "test14" << std::endl;

  // DPhi PF

  c_DPhi_fat->cd(0);
  h_DPhi_data_fat->SetTitle("");
  h_DPhi_data_fat->GetYaxis()->SetTitle("Events");
  h_DPhi_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_data_fat->GetXaxis()->SetTitle("Fat Jet #Delta #phi");
  h_DPhi_data_fat->SetMarkerStyle(20);

  h_DPhi_mc_fat->SetLineWidth(3);
  h_DPhi_mc_fat->SetTitle("");
  h_DPhi_mc_fat->SetFillColor(42);
  h_DPhi_mc_fat->SetLineColor(42);
  h_DPhi_mc_fat->GetYaxis()->SetTitle("events");
  h_DPhi_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_mc_fat->GetXaxis()->SetTitle("Fat Jet #Delta #phi");
  h_DPhi_mc_fat->Scale(DPhi_data_fat_weight/DPhi_mc_fat_weight);

  h_DPhi_mc_fat->Draw("");
  h_DPhi_data_fat->Draw("sameEP");

  l_DPhi_fat->SetFillColor(0);
  l_DPhi_fat->AddEntry(h_DPhi_data_fat,data_info.c_str(),"p");
  l_DPhi_fat->AddEntry(h_DPhi_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DPhi_fat->Draw("sames");

  c_DPhi_fat->Update();
  outf->Append(c_DPhi_fat);

  // DPhi (log)

  c_DPhi_log->cd(0);

  TH1F *h_DPhi_data_log                         =(TH1F*)h_DPhi_data->Clone();
  TH1F *h_DPhi_mc_log                           =(TH1F*)h_DPhi_mc->Clone();

  c_DPhi_log->SetLogy();

  h_DPhi_mc_log->SetMinimum(0.5);

  h_DPhi_mc_log->Draw("");
  h_DPhi_data_log->Draw("sameEP");

  l_DPhi_log->SetFillColor(0);
  l_DPhi_log->AddEntry(h_DPhi_data_log,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_DPhi_log->AddEntry(h_DPhi_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DPhi_log->Draw("sames");

  c_DPhi_log->Update();
  outf->Append(c_DPhi_log);

  // DPhi (log) PF

  c_DPhi_fat_log->cd(0);

  TH1F *h_DPhi_data_fat_log                         =(TH1F*)h_DPhi_data_fat->Clone();
  TH1F *h_DPhi_mc_fat_log                           =(TH1F*)h_DPhi_mc_fat->Clone();

  c_DPhi_fat_log->SetLogy();

  h_DPhi_mc_fat_log->SetMinimum(0.5);

  h_DPhi_mc_fat_log->Draw("");
  h_DPhi_data_fat_log->Draw("sameEP");

  l_DPhi_fat_log->SetFillColor(0);
  l_DPhi_fat_log->AddEntry(h_DPhi_data_fat_log,data_info.c_str(),"p");
  l_DPhi_fat_log->AddEntry(h_DPhi_mc_fat_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DPhi_fat_log->Draw("sames");

  c_DPhi_fat_log->Update();
  outf->Append(c_DPhi_fat_log);

  // Eta distribution for two leading jets

  c_Eta->cd(0);
  h_Eta_data->SetTitle("");
  h_Eta_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Eta_data->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_data->SetMarkerStyle(20);

  h_Eta_mc->SetLineWidth(3);
  h_Eta_mc->SetTitle("");
  h_Eta_mc->SetFillColor(42);
  h_Eta_mc->SetLineColor(42);
  h_Eta_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Eta_mc->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_mc->Scale(Eta_data_weight/Eta_mc_weight);

  h_Eta_mc->Draw("");
  h_Eta_data->Draw("sameEP");

  l_Eta->SetFillColor(0);
  l_Eta->AddEntry(h_Eta_data,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_Eta->AddEntry(h_Eta_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Eta->Draw("sames");

  c_Eta->Update();
  outf->Append(c_Eta);

  // Eta distribution for first leading jets

  c_Eta_1->cd(0);
  h_Eta_data_1->SetTitle("");
  h_Eta_data_1->GetYaxis()->SetTitle("first leading jets");
  h_Eta_data_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_1->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_data_1->SetMarkerStyle(20);

  h_Eta_mc_1->SetLineWidth(3);
  h_Eta_mc_1->SetTitle("");
  h_Eta_mc_1->SetFillColor(42);
  h_Eta_mc_1->SetLineColor(42);
  h_Eta_mc_1->GetYaxis()->SetTitle("first leading jets");
  h_Eta_mc_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_1->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_mc_1->Scale(Eta_data_weight_1/Eta_mc_weight_1);

  h_Eta_mc_1->Draw("");
  h_Eta_data_1->Draw("sameEP");

  l_Eta_1->SetFillColor(0);
  l_Eta_1->AddEntry(h_Eta_data_1,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_Eta_1->AddEntry(h_Eta_mc_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Eta_1->Draw("sames");

  c_Eta_1->Update();
  outf->Append(c_Eta_1);

  // Eta distribution for second leading jets

  c_Eta_2->cd(0);
  h_Eta_data_2->SetTitle("");
  h_Eta_data_2->GetYaxis()->SetTitle("second leading jets");
  h_Eta_data_2->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_2->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_data_2->SetMarkerStyle(20);

  h_Eta_mc_2->SetLineWidth(3);
  h_Eta_mc_2->SetTitle("");
  h_Eta_mc_2->SetFillColor(42);
  h_Eta_mc_2->SetLineColor(42);
  h_Eta_mc_2->GetYaxis()->SetTitle("second leading jets");
  h_Eta_mc_2->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_2->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_mc_2->Scale(Eta_data_weight_2/Eta_mc_weight_2);

  h_Eta_mc_2->Draw("");
  h_Eta_data_2->Draw("sameEP");

  l_Eta_2->SetFillColor(0);
  l_Eta_2->AddEntry(h_Eta_data_2,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_Eta_2->AddEntry(h_Eta_mc_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Eta_2->Draw("sames");

  c_Eta_2->Update();
  outf->Append(c_Eta_2);



  // Eta distribution for two leading jets PF

  c_Eta_fat->cd(0);
  h_Eta_data_fat->SetTitle("");
  h_Eta_data_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_Eta_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_fat->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_data_fat->SetMarkerStyle(20);

  h_Eta_mc_fat->SetLineWidth(3);
  h_Eta_mc_fat->SetTitle("");
  h_Eta_mc_fat->SetFillColor(42);
  h_Eta_mc_fat->SetLineColor(42);
  h_Eta_mc_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_Eta_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_fat->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_mc_fat->Scale(Eta_data_fat_weight/Eta_mc_fat_weight);

  h_Eta_mc_fat->SetMaximum(h_Eta_mc_fat->GetMaximum()*1.35);
  h_Eta_mc_fat->Draw("");
  h_Eta_data_fat->Draw("sameEP");

  l_Eta_fat->SetFillColor(0);
  l_Eta_fat->AddEntry(h_Eta_data_fat,data_info.c_str(),"p");
  l_Eta_fat->AddEntry(h_Eta_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Eta_fat->Draw("sames");

  c_Eta_fat->Update();
  outf->Append(c_Eta_fat);













  // Eta distribution for first leading jets PF

  c_Eta_fat_1->cd(0);
  h_Eta_data_fat_1->SetTitle("");
  h_Eta_data_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_Eta_data_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_fat_1->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_data_fat_1->SetMarkerStyle(20);

  h_Eta_mc_fat_1->SetLineWidth(3);
  h_Eta_mc_fat_1->SetTitle("");
  h_Eta_mc_fat_1->SetFillColor(42);
  h_Eta_mc_fat_1->SetLineColor(42);
  h_Eta_mc_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_Eta_mc_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_fat_1->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_mc_fat_1->Scale(Eta_data_fat_weight_1/Eta_mc_fat_weight_1);

  h_Eta_mc_fat_1->Draw("");
  h_Eta_data_fat_1->Draw("sameEP");

  l_Eta_fat_1->SetFillColor(0);
  l_Eta_fat_1->AddEntry(h_Eta_data_fat_1,data_info.c_str(),"p");
  l_Eta_fat_1->AddEntry(h_Eta_mc_fat_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Eta_fat_1->Draw("sames");

  c_Eta_fat_1->Update();
  outf->Append(c_Eta_fat_1);

  // Eta distribution for second leading jets PF

  c_Eta_fat_2->cd(0);
  h_Eta_data_fat_2->SetTitle("");
  h_Eta_data_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_Eta_data_fat_2->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_fat_2->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_data_fat_2->SetMarkerStyle(20);

  h_Eta_mc_fat_2->SetLineWidth(3);
  h_Eta_mc_fat_2->SetTitle("");
  h_Eta_mc_fat_2->SetFillColor(42);
  h_Eta_mc_fat_2->SetLineColor(42);
  h_Eta_mc_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_Eta_mc_fat_2->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_fat_2->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_mc_fat_2->Scale(Eta_data_fat_weight_2/Eta_mc_fat_weight_2);

  h_Eta_mc_fat_2->Draw("");
  h_Eta_data_fat_2->Draw("sameEP");

  l_Eta_fat_2->SetFillColor(0);
  l_Eta_fat_2->AddEntry(h_Eta_data_fat_2,data_info.c_str(),"p");
  l_Eta_fat_2->AddEntry(h_Eta_mc_fat_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Eta_fat_2->Draw("sames");

  c_Eta_fat_2->Update();
  outf->Append(c_Eta_fat_2);


  // Delta Eta b/w two leading jets

  c_DEta->cd(0);
  h_DEta_data->SetTitle("");
  h_DEta_data->GetYaxis()->SetTitle("Events");
  h_DEta_data->GetYaxis()->SetLabelSize(0.03);
  h_DEta_data->GetXaxis()->SetTitle("Jet #Delta#eta");
  h_DEta_data->SetMarkerStyle(20);

  h_DEta_mc->SetLineWidth(3);
  h_DEta_mc->SetTitle("");
  h_DEta_mc->SetFillColor(42);
  h_DEta_mc->SetLineColor(42);
  h_DEta_mc->GetYaxis()->SetTitle("Events");
  h_DEta_mc->GetYaxis()->SetLabelSize(0.03);
  h_DEta_mc->GetXaxis()->SetTitle("Jet #Delta#eta");
  h_DEta_mc->Scale(DEta_data_weight/DEta_mc_weight);

  h_DEta_mc->Draw("");
  h_DEta_data->Draw("sameEP");

  l_DEta->SetFillColor(0);
  l_DEta->AddEntry(h_DEta_data,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_DEta->AddEntry(h_DEta_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DEta->Draw("sames");

  c_DEta->Update();
  outf->Append(c_DEta);

  // Delta Eta b/w two leading jets PF

  c_DEta_fat->cd(0);
  h_DEta_data_fat->SetTitle("");
  h_DEta_data_fat->GetYaxis()->SetTitle("Events");
  h_DEta_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_DEta_data_fat->GetXaxis()->SetTitle("Fat Jet #Delta#eta");
  h_DEta_data_fat->SetMarkerStyle(20);

  h_DEta_mc_fat->SetLineWidth(3);
  h_DEta_mc_fat->SetTitle("");
  h_DEta_mc_fat->SetFillColor(42);
  h_DEta_mc_fat->SetLineColor(42);
  h_DEta_mc_fat->GetYaxis()->SetTitle("Events");
  h_DEta_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_DEta_mc_fat->GetXaxis()->SetTitle("Fat Jet #Delta#eta");
  h_DEta_mc_fat->Scale(DEta_data_fat_weight/DEta_mc_fat_weight);

  h_DEta_mc_fat->Draw("");
  h_DEta_data_fat->Draw("sameEP");

  l_DEta_fat->SetFillColor(0);
  l_DEta_fat->AddEntry(h_DEta_data_fat,data_info.c_str(),"p");
  l_DEta_fat->AddEntry(h_DEta_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DEta_fat->Draw("sames");

  c_DEta_fat->Update();
  outf->Append(c_DEta_fat);

  // phi distribution for two leading jets

  c_Phi->cd(0);


  float average = Phi_data_weight/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);
  std::cout << "test15" <<std::endl;
  for (int i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data->GetBinContent(i);
    Phi_sum+=h_Phi_data->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data->GetBinContent(i)-average,2);
  }
  std::cout << "test16" << std::endl;


  h_Phi_data->SetTitle("");
  h_Phi_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Phi_data->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_data->SetMarkerStyle(20);

  h_Phi_mc->SetLineWidth(3);
  h_Phi_mc->SetTitle("");
  h_Phi_mc->SetFillColor(42);
  h_Phi_mc->SetLineColor(42);
  h_Phi_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Phi_mc->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_mc->Scale(Phi_data_weight/Phi_mc_weight);
  h_Phi_mc->SetMinimum(average*0.8);
  h_Phi_mc->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi = new TGraph(2);

  g_phi->SetFillColor(1);
  g_phi->SetLineColor(2);
  g_phi->SetLineStyle(3);
  g_phi->SetLineWidth(3);
  g_phi->SetMarkerStyle(20);
  g_phi->SetPoint(0,-3.2,average);
  g_phi->SetPoint(1,3.2,average);

  h_Phi_mc->Draw("");
  h_Phi_data->Draw("sameEP");
  g_phi->Draw("l");


  pt_Phi->SetBorderSize(0);
  pt_Phi->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_1 = pt_Phi->AddText(s_average);
  TText *t_phi_2 = pt_Phi->AddText(s_rms);
  TText *t_phi_3 = pt_Phi->AddText(s_rms_percentage);

  pt_Phi->Draw("sames");

  l_Phi->SetFillColor(0);
  l_Phi->AddEntry(h_Phi_data,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_Phi->AddEntry(h_Phi_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi->Draw("sames");

  c_Phi->Update();
  outf->Append(c_Phi);

  // phi distribution for first leading jets

  c_Phi_1->cd(0);


  float average = Phi_data_weight_1/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_1->GetBinContent(i);
    Phi_sum+=h_Phi_data_1->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_1->GetBinContent(i)-average,2);
  }
  h_Phi_data_1->SetTitle("");
  h_Phi_data_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_data_1->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_1->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_data_1->SetMarkerStyle(20);

  h_Phi_mc_1->SetLineWidth(3);
  h_Phi_mc_1->SetTitle("");
  h_Phi_mc_1->SetFillColor(42);
  h_Phi_mc_1->SetLineColor(42);
  h_Phi_mc_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_mc_1->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_1->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_mc_1->Scale(Phi_data_weight_1/Phi_mc_weight_1);
  h_Phi_mc_1->SetMinimum(average*0.8);
  h_Phi_mc_1->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi_1 = new TGraph(2);

  g_phi_1->SetFillColor(1);
  g_phi_1->SetLineColor(2);
  g_phi_1->SetLineStyle(3);
  g_phi_1->SetLineWidth(3);
  g_phi_1->SetMarkerStyle(20);
  g_phi_1->SetPoint(0,-3.2,average);
  g_phi_1->SetPoint(1,3.2,average);

  h_Phi_mc_1->Draw("");
  h_Phi_data_1->Draw("sameEP");
  g_phi_1->Draw("l");


  pt_Phi_1->SetBorderSize(0);
  pt_Phi_1->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_1 = pt_Phi_1->AddText(s_average);
  TText *t_phi_2 = pt_Phi_1->AddText(s_rms);
  TText *t_phi_3 = pt_Phi_1->AddText(s_rms_percentage);

  pt_Phi_1->Draw("sames");

  l_Phi_1->SetFillColor(0);
  l_Phi_1->AddEntry(h_Phi_data_1,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_Phi_1->AddEntry(h_Phi_mc_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi_1->Draw("sames");

  c_Phi_1->Update();
  outf->Append(c_Phi_1);

  // phi distribution for second leading jets

  c_Phi_2->cd(0);


  float average = Phi_data_weight_2/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_2->GetBinContent(i);
    Phi_sum+=h_Phi_data_2->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_2->GetBinContent(i)-average,2);
  }

  h_Phi_data_2->SetTitle("");
  h_Phi_data_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_data_2->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_2->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_data_2->SetMarkerStyle(20);

  h_Phi_mc_2->SetLineWidth(3);
  h_Phi_mc_2->SetTitle("");
  h_Phi_mc_2->SetFillColor(42);
  h_Phi_mc_2->SetLineColor(42);
  h_Phi_mc_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_mc_2->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_2->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_mc_2->Scale(Phi_data_weight_2/Phi_mc_weight_2);
  h_Phi_mc_2->SetMinimum(average*0.8);
  h_Phi_mc_2->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi_2 = new TGraph(2);

  g_phi_2->SetFillColor(1);
  g_phi_2->SetLineColor(2);
  g_phi_2->SetLineStyle(3);
  g_phi_2->SetLineWidth(3);
  g_phi_2->SetMarkerStyle(20);
  g_phi_2->SetPoint(0,-3.2,average);
  g_phi_2->SetPoint(1,3.2,average);

  h_Phi_mc_2->Draw("");
  h_Phi_data_2->Draw("sameEP");
  g_phi_2->Draw("l");


  pt_Phi_2->SetBorderSize(0);
  pt_Phi_2->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_1 = pt_Phi_2->AddText(s_average);
  TText *t_phi_2 = pt_Phi_2->AddText(s_rms);
  TText *t_phi_3 = pt_Phi_2->AddText(s_rms_percentage);

  pt_Phi_2->Draw("sames");

  l_Phi_2->SetFillColor(0);
  l_Phi_2->AddEntry(h_Phi_data_2,"CMS 2011 Data Calo( 201.348pb^{-1} )","p");
  l_Phi_2->AddEntry(h_Phi_mc_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi_2->Draw("sames");

  c_Phi_2->Update();
  outf->Append(c_Phi_2);





  // phi distribution for two leading jets PF

  c_Phi_fat->cd(0);


  float average = Phi_data_fat_weight/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_fat->GetBinContent(i);
    Phi_sum+=h_Phi_data_fat->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_fat->GetBinContent(i)-average,2);
  }

  h_Phi_data_fat->SetTitle("");
  h_Phi_data_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_Phi_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_fat->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_data_fat->SetMarkerStyle(20);

  h_Phi_mc_fat->SetLineWidth(3);
  h_Phi_mc_fat->SetTitle("");
  h_Phi_mc_fat->SetFillColor(42);
  h_Phi_mc_fat->SetLineColor(42);
  h_Phi_mc_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_Phi_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_fat->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_mc_fat->Scale(Phi_data_fat_weight/Phi_mc_fat_weight);
  h_Phi_mc_fat->SetMinimum(average*0.8);
  h_Phi_mc_fat->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi = new TGraph(2);

  g_phi->SetFillColor(1);
  g_phi->SetLineColor(2);
  g_phi->SetLineStyle(3);
  g_phi->SetLineWidth(3);
  g_phi->SetMarkerStyle(20);
  g_phi->SetPoint(0,-3.2,average);
  g_phi->SetPoint(1,3.2,average);

  h_Phi_mc_fat->Draw("");
  h_Phi_data_fat->Draw("sameEP");
  g_phi->Draw("l");


  pt_Phi_fat->SetBorderSize(0);
  pt_Phi_fat->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_fat_1 = pt_Phi_fat->AddText(s_average);
  TText *t_phi_fat_2 = pt_Phi_fat->AddText(s_rms);
  TText *t_phi_fat_3 = pt_Phi_fat->AddText(s_rms_percentage);

  pt_Phi_fat->Draw("sames");

  l_Phi_fat->SetFillColor(0);
  l_Phi_fat->AddEntry(h_Phi_data,data_info.c_str(),"p");
  l_Phi_fat->AddEntry(h_Phi_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi_fat->Draw("sames");

  c_Phi->Update();
  outf->Append(c_Phi);

  // phi distribution for first leading jets

  c_Phi_fat_1->cd(0);


  float average = Phi_data_fat_weight_1/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_fat_1->GetBinContent(i);
    Phi_sum+=h_Phi_data_fat_1->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_fat_1->GetBinContent(i)-average,2);
  }
  h_Phi_data_fat_1->SetTitle("");
  h_Phi_data_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_data_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_fat_1->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_data_fat_1->SetMarkerStyle(20);

  h_Phi_mc_fat_1->SetLineWidth(3);
  h_Phi_mc_fat_1->SetTitle("");
  h_Phi_mc_fat_1->SetFillColor(42);
  h_Phi_mc_fat_1->SetLineColor(42);
  h_Phi_mc_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_mc_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_fat_1->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_mc_fat_1->Scale(Phi_data_fat_weight_1/Phi_mc_fat_weight_1);
  h_Phi_mc_fat_1->SetMinimum(average*0.8);
  h_Phi_mc_fat_1->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi_1 = new TGraph(2);

  g_phi_1->SetFillColor(1);
  g_phi_1->SetLineColor(2);
  g_phi_1->SetLineStyle(3);
  g_phi_1->SetLineWidth(3);
  g_phi_1->SetMarkerStyle(20);
  g_phi_1->SetPoint(0,-3.2,average);
  g_phi_1->SetPoint(1,3.2,average);

  h_Phi_mc_fat_1->Draw("");
  h_Phi_data_fat_1->Draw("sameEP");
  g_phi_1->Draw("l");


  pt_Phi_fat_1->SetBorderSize(0);
  pt_Phi_fat_1->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_fat_1 = pt_Phi_fat_1->AddText(s_average);
  TText *t_phi_fat_2 = pt_Phi_fat_1->AddText(s_rms);
  TText *t_phi_fat_3 = pt_Phi_fat_1->AddText(s_rms_percentage);

  pt_Phi_fat_1->Draw("sames");

  l_Phi_fat_1->SetFillColor(0);
  l_Phi_fat_1->AddEntry(h_Phi_data_fat_1,data_info.c_str(),"p");
  l_Phi_fat_1->AddEntry(h_Phi_mc_fat_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi_fat_1->Draw("sames");

  c_Phi_fat_1->Update();
  outf->Append(c_Phi_fat_1);

  // phi distribution for second leading jets

  c_Phi_fat_2->cd(0);


  float average = Phi_data_fat_weight_2/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_fat_2->GetBinContent(i);
    Phi_sum+=h_Phi_data_fat_2->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_fat_2->GetBinContent(i)-average,2);
  }

  h_Phi_data_fat_2->SetTitle("");
  h_Phi_data_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_data_fat_2->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_fat_2->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_data_fat_2->SetMarkerStyle(20);

  h_Phi_mc_fat_2->SetLineWidth(3);
  h_Phi_mc_fat_2->SetTitle("");
  h_Phi_mc_fat_2->SetFillColor(42);
  h_Phi_mc_fat_2->SetLineColor(42);
  h_Phi_mc_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_mc_fat_2->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_fat_2->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_mc_fat_2->Scale(Phi_data_fat_weight_2/Phi_mc_fat_weight_2);
  h_Phi_mc_fat_2->SetMinimum(average*0.8);
  h_Phi_mc_fat_2->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi_2 = new TGraph(2);

  g_phi_2->SetFillColor(1);
  g_phi_2->SetLineColor(2);
  g_phi_2->SetLineStyle(3);
  g_phi_2->SetLineWidth(3);
  g_phi_2->SetMarkerStyle(20);
  g_phi_2->SetPoint(0,-3.2,average);
  g_phi_2->SetPoint(1,3.2,average);

  h_Phi_mc_fat_2->Draw("");
  h_Phi_data_fat_2->Draw("sameEP");
  g_phi_2->Draw("l");


  pt_Phi_fat_2->SetBorderSize(0);
  pt_Phi_fat_2->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_fat_1 = pt_Phi_fat_2->AddText(s_average);
  TText *t_phi_fat_2 = pt_Phi_fat_2->AddText(s_rms);
  TText *t_phi_fat_3 = pt_Phi_fat_2->AddText(s_rms_percentage);

  pt_Phi_fat_2->Draw("sames");

  l_Phi_fat_2->SetFillColor(0);
  l_Phi_fat_2->AddEntry(h_Phi_data_fat_2,data_info.c_str(),"p");
  l_Phi_fat_2->AddEntry(h_Phi_mc_fat_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi_fat_2->Draw("sames");

  c_Phi_fat_2->Update();
  outf->Append(c_Phi_fat_2);







  // n90hits distribution for two jets

  c_n90hits->cd(0);
  h_n90hits_data->SetTitle("");
  h_n90hits_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_n90hits_data->GetYaxis()->SetLabelSize(0.03);
  h_n90hits_data->GetXaxis()->SetTitle("Jet n90hits");
  h_n90hits_data->SetMarkerStyle(20);

  h_n90hits_mc->SetLineWidth(3);
  h_n90hits_mc->SetTitle("");
  h_n90hits_mc->SetFillColor(42);
  h_n90hits_mc->SetLineColor(42);
  h_n90hits_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_n90hits_mc->GetYaxis()->SetLabelSize(0.03);
  h_n90hits_mc->GetXaxis()->SetTitle("Jet n90hits");
  h_n90hits_mc->Scale(n90hits_data_weight/n90hits_mc_weight);

  h_n90hits_mc->Draw("");
  h_n90hits_data->Draw("sameEP");

  l_n90hits->SetFillColor(0);
  l_n90hits->AddEntry(h_n90hits_data,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_n90hits->AddEntry(h_n90hits_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_n90hits->Draw("sames");

  c_n90hits->Update();
  outf->Append(c_n90hits);

  c_n90hits_log->cd(0);

  TH1F *h_n90hits_data_log                      =(TH1F*)h_n90hits_data->Clone();
  TH1F *h_n90hits_mc_log                        =(TH1F*)h_n90hits_mc->Clone();

  c_n90hits_log->SetLogy();

  h_n90hits_mc_log->SetMinimum(0.5);

  h_n90hits_mc_log->Draw("");
  h_n90hits_data_log->Draw("sameEP");

  l_n90hits_log->SetFillColor(0);
  l_n90hits_log->AddEntry(h_n90hits_data_log,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_n90hits_log->AddEntry(h_n90hits_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_n90hits_log->Draw("sames");

  c_n90hits_log->Update();
  outf->Append(c_n90hits_log);

  // fHPD

  c_fHPD->cd(0);
  h_fHPD_data->SetTitle("");
  h_fHPD_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_fHPD_data->GetYaxis()->SetLabelSize(0.03);
  h_fHPD_data->GetXaxis()->SetTitle("Jet fHPD");
  h_fHPD_data->SetMarkerStyle(20);

  h_fHPD_mc->SetLineWidth(3);
  h_fHPD_mc->SetTitle("");
  h_fHPD_mc->SetFillColor(42);
  h_fHPD_mc->SetLineColor(42);
  h_fHPD_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_fHPD_mc->GetYaxis()->SetLabelSize(0.03);
  h_fHPD_mc->GetXaxis()->SetTitle("Jet fHPD");
  h_fHPD_mc->Scale(fHPD_data_weight/fHPD_mc_weight);

  h_fHPD_mc->Draw("");
  h_fHPD_data->Draw("sameEP");

  l_fHPD->SetFillColor(0);
  l_fHPD->AddEntry(h_fHPD_data,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_fHPD->AddEntry(h_fHPD_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fHPD->Draw("sames");

  c_fHPD->Update();
  outf->Append(c_fHPD);

  // corrected pt distribution for two leading jets

  c_corPt->cd(0);
  h_corPt_data->SetTitle("");
  h_corPt_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_corPt_data->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_data->SetMarkerStyle(20);

  h_corPt_mc->SetLineWidth(3);
  h_corPt_mc->SetTitle("");
  h_corPt_mc->SetFillColor(42);
  h_corPt_mc->SetLineColor(42);
  h_corPt_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_corPt_mc->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_mc->Scale(corPt_data_weight/corPt_mc_weight);

  h_corPt_mc->Draw("");
  h_corPt_data->Draw("sameEP");

  l_corPt->SetFillColor(0);
  l_corPt->AddEntry(h_corPt_data,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_corPt->AddEntry(h_corPt_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt->Draw("sames");

  c_corPt->Update();
  outf->Append(c_corPt);


  // corrected pt distribution for two leading jets PF

  c_corPt_fat->cd(0);
  h_corPt_data_fat->SetTitle("");
  h_corPt_data_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_corPt_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_fat->GetXaxis()->SetTitle("Fat Jet cor P_{T}");
  h_corPt_data_fat->SetMarkerStyle(20);

  h_corPt_mc_fat->SetLineWidth(3);
  h_corPt_mc_fat->SetTitle("");
  h_corPt_mc_fat->SetFillColor(42);
  h_corPt_mc_fat->SetLineColor(42);
  h_corPt_mc_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_corPt_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_fat->GetXaxis()->SetTitle("Fat Jet cor P_{T}");
  h_corPt_mc_fat->Scale(corPt_data_fat_weight/corPt_mc_fat_weight);

  h_corPt_mc_fat->Draw("");
  h_corPt_data_fat->Draw("sameEP");

  l_corPt_fat->SetFillColor(0);
  l_corPt_fat->AddEntry(h_corPt_data,data_info.c_str(),"p");
  l_corPt_fat->AddEntry(h_corPt_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat->Draw("sames");

  c_corPt_fat->Update();
  outf->Append(c_corPt_fat);

  // corrected pt distribution for two leading jets (log)

  c_corPt_log->cd(0);

  TH1F *h_corPt_data_log                        =(TH1F*)h_corPt_data->Clone();
  TH1F *h_corPt_mc_log                          =(TH1F*)h_corPt_mc->Clone();

  c_corPt_log->SetLogy();

  h_corPt_mc_log->SetMinimum(0.5);

  h_corPt_mc_log->Draw("");
  h_corPt_data_log->Draw("sameEP");

  l_corPt_log->SetFillColor(0);
  l_corPt_log->AddEntry((TObject*)0,"2011 data","");
  l_corPt_log->AddEntry(h_corPt_data_log,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_corPt_log->AddEntry(h_corPt_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_log->Draw("sames");

  c_corPt_log->Update();
  outf->Append(c_corPt_log);

  // corrected pt distribution for two leading jets (log) PF

  c_corPt_fat_log->cd(0);

  TH1F *h_corPt_data_fat_log                        =(TH1F*)h_corPt_data_fat->Clone();
  TH1F *h_corPt_mc_fat_log                          =(TH1F*)h_corPt_mc_fat->Clone();

  c_corPt_fat_log->SetLogy();

  h_corPt_mc_fat_log->SetMinimum(0.5);

  h_corPt_mc_fat_log->Draw("");
  h_corPt_data_fat_log->Draw("sameEP");

  l_corPt_fat_log->SetFillColor(0);
  l_corPt_fat_log->AddEntry((TObject*)0,"2011 data","");
  l_corPt_fat_log->AddEntry(h_corPt_data_fat_log,data_info.c_str(),"p");
  l_corPt_fat_log->AddEntry(h_corPt_mc_fat_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_log->Draw("sames");

  c_corPt_fat_log->Update();
  outf->Append(c_corPt_fat_log);


  // corrected pt distribution for first leading jets

  c_corPt_1->cd(0);
  h_corPt_data_1->SetTitle("");
  h_corPt_data_1->GetYaxis()->SetTitle("first leading jets");
  h_corPt_data_1->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_1->GetXaxis()->SetTitle("Jet P_{T}");
  h_corPt_data_1->SetMarkerStyle(20);

  h_corPt_mc_1->SetLineWidth(3);
  h_corPt_mc_1->SetTitle("");
  h_corPt_mc_1->SetFillColor(42);
  h_corPt_mc_1->SetLineColor(42);
  h_corPt_mc_1->GetYaxis()->SetTitle("first leading jets");
  h_corPt_mc_1->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_1->GetXaxis()->SetTitle("Jet P_{T}");
  h_corPt_mc_1->Scale(corPt_data_weight_1/corPt_mc_weight_1);

  h_corPt_mc_1->Draw("");
  h_corPt_data_1->Draw("sameEP");

  l_corPt_1->SetFillColor(0);
  l_corPt_1->AddEntry(h_corPt_data_1,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_corPt_1->AddEntry(h_corPt_mc_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_1->Draw("sames");

  c_corPt_1->Update();
  outf->Append(c_corPt_1);

  // corrected pt distribution for first leading jets PF

  c_corPt_fat_1->cd(0);
  h_corPt_data_fat_1->SetTitle("");
  h_corPt_data_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_corPt_data_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_fat_1->GetXaxis()->SetTitle("Fat Jet P_{T}");
  h_corPt_data_fat_1->SetMarkerStyle(20);

  h_corPt_mc_fat_1->SetLineWidth(3);
  h_corPt_mc_fat_1->SetTitle("");
  h_corPt_mc_fat_1->SetFillColor(42);
  h_corPt_mc_fat_1->SetLineColor(42);
  h_corPt_mc_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_corPt_mc_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_fat_1->GetXaxis()->SetTitle("Fat Jet P_{T}");
  h_corPt_mc_fat_1->Scale(corPt_data_fat_weight_1/corPt_mc_fat_weight_1);

  h_corPt_mc_fat_1->Draw("");
  h_corPt_data_fat_1->Draw("sameEP");

  l_corPt_fat_1->SetFillColor(0);
  l_corPt_fat_1->AddEntry(h_corPt_data_fat_1,data_info.c_str(),"p");
  l_corPt_fat_1->AddEntry(h_corPt_mc_fat_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_1->Draw("sames");

  c_corPt_fat_1->Update();
  outf->Append(c_corPt_fat_1);




  // corrected pt distribution for first leading jets (log)

  c_corPt_log_1->cd(0);

  TH1F *h_corPt_data_log_1                        =(TH1F*)h_corPt_data_1->Clone();
  TH1F *h_corPt_mc_log_1                          =(TH1F*)h_corPt_mc_1->Clone();

  c_corPt_log_1->SetLogy();

  h_corPt_mc_log_1->SetMinimum(0.5);

  h_corPt_mc_log_1->Draw("");
  h_corPt_data_log_1->Draw("sameEP");

  l_corPt_log_1->SetFillColor(0);
  l_corPt_log_1->AddEntry((TObject*)0,"2011 data","");
  l_corPt_log_1->AddEntry(h_corPt_data_log_1,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_corPt_log_1->AddEntry(h_corPt_mc_log_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_log_1->Draw("sames");

  c_corPt_log_1->Update();
  outf->Append(c_corPt_log_1);

  // corrected pt distribution for first leading jets (log) PF

  c_corPt_fat_log_1->cd(0);

  TH1F *h_corPt_data_fat_log_1                        =(TH1F*)h_corPt_data_fat_1->Clone();
  TH1F *h_corPt_mc_fat_log_1                          =(TH1F*)h_corPt_mc_fat_1->Clone();

  c_corPt_fat_log_1->SetLogy();

  h_corPt_mc_fat_log_1->SetMinimum(0.5);

  h_corPt_mc_fat_log_1->Draw("");
  h_corPt_data_fat_log_1->Draw("sameEP");

  l_corPt_fat_log_1->SetFillColor(0);
  l_corPt_fat_log_1->AddEntry((TObject*)0,"2011 data","");
  l_corPt_fat_log_1->AddEntry(h_corPt_data_fat_log_1,data_info.c_str(),"p");
  l_corPt_fat_log_1->AddEntry(h_corPt_mc_fat_log_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_log_1->Draw("sames");

  c_corPt_fat_log_1->Update();
  outf->Append(c_corPt_fat_log_1);

  // corrected pt distribution for second leading jets

  c_corPt_2->cd(0);
  h_corPt_data_2->SetTitle("");
  h_corPt_data_2->GetYaxis()->SetTitle("second leading jets");
  h_corPt_data_2->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_2->GetXaxis()->SetTitle("Jet P_{T}");
  h_corPt_data_2->SetMarkerStyle(20);

  h_corPt_mc_2->SetLineWidth(3);
  h_corPt_mc_2->SetTitle("");
  h_corPt_mc_2->SetFillColor(42);
  h_corPt_mc_2->SetLineColor(42);
  h_corPt_mc_2->GetYaxis()->SetTitle("second leading jets");
  h_corPt_mc_2->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_2->GetXaxis()->SetTitle("Jet P_{T}");
  h_corPt_mc_2->Scale(corPt_data_weight_2/corPt_mc_weight_2);

  h_corPt_mc_2->Draw("");
  h_corPt_data_2->Draw("sameEP");

  l_corPt_2->SetFillColor(0);
  l_corPt_2->AddEntry(h_corPt_data_2,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_corPt_2->AddEntry(h_corPt_mc_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_2->Draw("sames");

  c_corPt_2->Update();
  outf->Append(c_corPt_2);

  // corrected pt distribution for second leading jets PF

  c_corPt_fat_2->cd(0);
  h_corPt_data_fat_2->SetTitle("");
  h_corPt_data_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_corPt_data_fat_2->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_fat_2->GetXaxis()->SetTitle("Fat Jet P_{T}");
  h_corPt_data_fat_2->SetMarkerStyle(20);

  h_corPt_mc_fat_2->SetLineWidth(3);
  h_corPt_mc_fat_2->SetTitle("");
  h_corPt_mc_fat_2->SetFillColor(42);
  h_corPt_mc_fat_2->SetLineColor(42);
  h_corPt_mc_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_corPt_mc_fat_2->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_fat_2->GetXaxis()->SetTitle("Fat Jet P_{T}");
  h_corPt_mc_fat_2->Scale(corPt_data_fat_weight_2/corPt_mc_fat_weight_2);

  h_corPt_mc_fat_2->Draw("");
  h_corPt_data_fat_2->Draw("sameEP");

  l_corPt_fat_2->SetFillColor(0);
  l_corPt_fat_2->AddEntry(h_corPt_data_fat_2,data_info.c_str(),"p");
  l_corPt_fat_2->AddEntry(h_corPt_mc_fat_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_2->Draw("sames");

  c_corPt_fat_2->Update();
  outf->Append(c_corPt_fat_2);

  // corrected pt distribution for second leading jets (log)

  c_corPt_log_2->cd(0);

  TH1F *h_corPt_data_log_2                        =(TH1F*)h_corPt_data_2->Clone();
  TH1F *h_corPt_mc_log_2                          =(TH1F*)h_corPt_mc_2->Clone();

  c_corPt_log_2->SetLogy();

  h_corPt_mc_log_2->SetMinimum(0.5);

  h_corPt_mc_log_2->Draw("");
  h_corPt_data_log_2->Draw("sameEP");

  l_corPt_log_2->SetFillColor(0);
  l_corPt_log_2->AddEntry((TObject*)0,"2011 data","");
  l_corPt_log_2->AddEntry(h_corPt_data_log_2,"CMS 2011 Data Calo ( 201.348pb^{-1} )","p");
  l_corPt_log_2->AddEntry(h_corPt_mc_log_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_log_2->Draw("sames");

  c_corPt_log_2->Update();
  outf->Append(c_corPt_log_2);

  // corrected pt distribution for second leading jets (log)

  c_corPt_fat_log_2->cd(0);

  TH1F *h_corPt_data_fat_log_2                        =(TH1F*)h_corPt_data_fat_2->Clone();
  TH1F *h_corPt_mc_fat_log_2                          =(TH1F*)h_corPt_mc_fat_2->Clone();

  c_corPt_fat_log_2->SetLogy();

  h_corPt_mc_fat_log_2->SetMinimum(0.5);

  h_corPt_mc_fat_log_2->Draw("");
  h_corPt_data_fat_log_2->Draw("sameEP");

  l_corPt_fat_log_2->SetFillColor(0);
  l_corPt_fat_log_2->AddEntry((TObject*)0,"2011 data","");
  l_corPt_fat_log_2->AddEntry(h_corPt_data_fat_log_2,data_info.c_str(),"p");
  l_corPt_fat_log_2->AddEntry(h_corPt_mc_fat_log_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_log_2->Draw("sames");

  c_corPt_fat_log_2->Update();
  outf->Append(c_corPt_fat_log_2);

  // nTrkVtx distribution for two leading jets

  c_nTrkVtx->cd(0);
  h_nTrkVtx_data->SetTitle("");
  h_nTrkVtx_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkVtx_data->GetYaxis()->SetLabelSize(0.03);
  h_nTrkVtx_data->GetXaxis()->SetTitle("nTrkVtx");
  h_nTrkVtx_data->SetMarkerStyle(20);

  h_nTrkVtx_mc->SetLineWidth(3);
  h_nTrkVtx_mc->SetTitle("");
  h_nTrkVtx_mc->SetFillColor(42);
  h_nTrkVtx_mc->SetLineColor(42);
  h_nTrkVtx_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkVtx_mc->GetYaxis()->SetLabelSize(0.03);
  h_nTrkVtx_mc->GetXaxis()->SetTitle("nTrkVtx");
  h_nTrkVtx_mc->Scale(nTrkVtx_data_weight/nTrkVtx_mc_weight);

  h_nTrkVtx_mc->Draw("");
  h_nTrkVtx_data->Draw("sameEP");

  l_nTrkVtx->SetFillColor(0);
  l_nTrkVtx->AddEntry(h_nTrkVtx_data,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_nTrkVtx->AddEntry(h_nTrkVtx_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_nTrkVtx->Draw("sames");

  c_nTrkVtx->Update();
  outf->Append(c_nTrkVtx);

  // nTrkCalo distribution for two leading jets

  c_nTrkCalo->cd(0);
  h_nTrkCalo_data->SetTitle("");
  h_nTrkCalo_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkCalo_data->GetYaxis()->SetLabelSize(0.03);
  h_nTrkCalo_data->GetXaxis()->SetTitle("nTrkCalo");
  h_nTrkCalo_data->SetMarkerStyle(20);

  h_nTrkCalo_mc->SetLineWidth(3);
  h_nTrkCalo_mc->SetTitle("");
  h_nTrkCalo_mc->SetFillColor(42);
  h_nTrkCalo_mc->SetLineColor(42);
  h_nTrkCalo_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkCalo_mc->GetYaxis()->SetLabelSize(0.03);
  h_nTrkCalo_mc->GetXaxis()->SetTitle("nTrkCalo");
  h_nTrkCalo_mc->Scale(nTrkCalo_data_weight/nTrkCalo_mc_weight);

  h_nTrkCalo_mc->Draw("");
  h_nTrkCalo_data->Draw("sameEP");

  l_nTrkCalo->SetFillColor(0);
  l_nTrkCalo->AddEntry(h_nTrkCalo_data,"CMS 2011 Data ( 201.348pb^{-1} )","p");
  l_nTrkCalo->AddEntry(h_nTrkCalo_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_nTrkCalo->Draw("sames");

  c_nTrkCalo->Update();
  outf->Append(c_nTrkCalo);

  outf->Append(h_DijetMass_data);
  outf->Append(h_DijetMass_data_fat);
  outf->Append(h_DijetMass_MI_nPVe1_data);
  outf->Append(h_DijetMass_MI_nPVg1_data);
  outf->Append(h_MET_over_sumEt_data);
  outf->Append(h_EMF_data);
  outf->Append(h_DPhi_data);
  outf->Append(h_Eta_data);
  outf->Append(h_Eta_data_1);
  outf->Append(h_Eta_data_2);
  outf->Append(h_DEta_data);
  outf->Append(h_Phi_data);
  outf->Append(h_Phi_data_1);
  outf->Append(h_Phi_data_2);
  outf->Append(h_n90hits_data);
  outf->Append(h_fHPD_data);
  outf->Append(h_corPt_data);
  outf->Append(h_corPt_data_1);
  outf->Append(h_corPt_data_2);
  outf->Append(h_nTrkVtx_data);
  outf->Append(h_nTrkCalo_data);
  outf->Append(h_DijetMass_normalized_MI_nPVe1_data);
  outf->Append(h_DijetMass_normalized_MI_nPVg1_data);

  outf->Append(h_DijetMass_mc);
  outf->Append(h_DijetMass_mc_fat);
  outf->Append(h_DijetMass_MI_nPVe1_mc);
  outf->Append(h_DijetMass_MI_nPVg1_mc);
  outf->Append(h_MET_over_sumEt_mc);
  outf->Append(h_EMF_mc);
  outf->Append(h_DPhi_mc);
  outf->Append(h_Eta_mc);
  outf->Append(h_Eta_mc_1);
  outf->Append(h_Eta_mc_2);
  outf->Append(h_DEta_mc);
  outf->Append(h_Phi_mc);
  outf->Append(h_Phi_mc_1);
  outf->Append(h_Phi_mc_2);
  outf->Append(h_n90hits_mc);
  outf->Append(h_fHPD_mc);
  outf->Append(h_corPt_mc);
  outf->Append(h_corPt_mc_1);
  outf->Append(h_corPt_mc_2);
  outf->Append(h_nTrkVtx_mc);
  outf->Append(h_nTrkCalo_mc);


  c_DijetMass_fat->SaveAs("c_DijetMass_fat.png");
  c_DijetMass_fat_log->SaveAs("c_DijetMass_fat_log.png");

  c_DijetMass_ratio_fat->SaveAs("c_DijetMass_ratio_fat.png");
  c_DijetMass_ratio_fat_log->SaveAs("c_DijetMass_ratio_fat_log.png");
  //  c_DijetMass_MI_fat->SaveAs("c_DijetMass_MI_fat.png");
  //  c_DijetMass_MI_fat_log->SaveAs("c_DijetMass_MI_fat_log.png");
  // c_DijetMass_normalized_MI_fat->SaveAs("c_DijetMass_normalized_MI_fat.png");
  // c_DijetMass_normalized_MI_fat_log->SaveAs("c_DijetMass_normalized_MI_fat_log.png");
  //  c_Eta_Phi_Scatter_fat->SaveAs("c_Eta_Phi_Scatter_fat.png");
  //  c_Eta_Phi_Scatter_1_fat->SaveAs("c_Eta_Phi_Scatter_1_fat.png");
  //  c_Eta_Phi_Scatter_2_fat->SaveAs("c_Eta_Phi_Scatter_2_fat.png");
  c_DPhi_fat->SaveAs("c_DPhi_fat.png");
  c_DPhi_fat_log->SaveAs("c_DPhi_fat_log.png");
  c_Eta_fat->SaveAs("c_Eta_fat.png");
  c_Eta_fat_1->SaveAs("c_Eta_1_fat.png");
  c_Eta_fat_2->SaveAs("c_Eta_2_fat.png");
  c_DEta_fat->SaveAs("c_DEta_fat.png");
  c_Phi_fat->SaveAs("c_Phi_fat.png");
  c_Phi_fat_1->SaveAs("c_Phi_1_fat.png");
  c_Phi_fat_2->SaveAs("c_Phi_2_fat.png");
  c_corPt_fat->SaveAs("c_corPt_fat.png");
  c_corPt_fat_1->SaveAs("c_corPt_1_fat.png");
  c_corPt_fat_2->SaveAs("c_corPt_2_fat.png");
  c_corPt_fat_log->SaveAs("c_corPt_fat_log.png");
  c_corPt_fat_log_1->SaveAs("c_corPt_log_1_fat.png");
  c_corPt_fat_log_2->SaveAs("c_corPt_log_2_fat.png");
  c_fCh->SaveAs("c_fCh_fat.png");
  c_fPh->SaveAs("c_fPh_fat.png");
  c_fNh->SaveAs("c_fNh_fat.png");
  c_fEl->SaveAs("c_fEl_fat.png");

  /*
  c_DijetMass->SaveAs("c_DijetMass.png");
  c_DijetMass_log->SaveAs("c_DijetMass_log.png");
  c_DijetMass_ratio->SaveAs("c_DijetMass_ratio.png");
  c_DijetMass_ratio_log->SaveAs("c_DijetMass_ratio_log.png");
  c_DijetMass_MI->SaveAs("c_DijetMass_MI.png");
  c_DijetMass_MI_log->SaveAs("c_DijetMass_MI_log.png");
  c_DijetMass_normalized_MI->SaveAs("c_DijetMass_normalized_MI.png");
  c_DijetMass_normalized_MI_log->SaveAs("c_DijetMass_normalized_MI_log.png");
  c_Eta_Phi_Scatter->SaveAs("c_Eta_Phi_Scatter.png");
  c_Eta_Phi_Scatter_1->SaveAs("c_Eta_Phi_Scatter_1.png");
  c_Eta_Phi_Scatter_2->SaveAs("c_Eta_Phi_Scatter_2.png");
  c_MET_over_sumEt->SaveAs("c_MET_over_sumEt.png");
  c_MET_over_sumEt_log->SaveAs("c_MET_over_sumEt_log.png");
  c_EMF->SaveAs("c_EMF.png");
  c_DPhi->SaveAs("c_DPhi.png");
  c_DPhi_log->SaveAs("c_DPhi_log.png");
  c_Eta->SaveAs("c_Eta.png");
  c_Eta_1->SaveAs("c_Eta_1.png");
  c_Eta_2->SaveAs("c_Eta_2.png");
  c_DEta->SaveAs("c_DEta.png");
  c_Phi->SaveAs("c_Phi.png");
  c_Phi_1->SaveAs("c_Phi_1.png");
  c_Phi_2->SaveAs("c_Phi_2.png");
  c_n90hits->SaveAs("c_n90hits.png");
  c_n90hits_log->SaveAs("c_n90hits_log.png");
  c_fHPD->SaveAs("c_fHPD.png");
  c_corPt->SaveAs("c_corPt.png");
  c_corPt_1->SaveAs("c_corPt_1.png");
  c_corPt_2->SaveAs("c_corPt_2.png");
  c_corPt_log->SaveAs("c_corPt_log.png");
  c_corPt_log_1->SaveAs("c_corPt_log_1.png");
  c_corPt_log_2->SaveAs("c_corPt_log_2.png");
  c_nTrkVtx->SaveAs("c_nTrkVtx.png");
  c_nTrkCalo->SaveAs("c_nTrkCalo.png");
  c_fCh->SaveAs("c_fCh.png");
  c_fPh->SaveAs("c_fPh.png");
  c_fNh->SaveAs("c_fNh.png");
  c_fEl->SaveAs("c_fEl.png");
  */
  /*

  c_DijetMass->SaveAs("c_DijetMass.eps");
  c_DijetMass_log->SaveAs("c_DijetMass_log.eps");
  c_DijetMass_ratio->SaveAs("c_DijetMass_ratio.eps");
  c_DijetMass_ratio_log->SaveAs("c_DijetMass_ratio_log.eps");
  c_DijetMass_MI->SaveAs("c_DijetMass_MI.eps");
  c_DijetMass_MI_log->SaveAs("c_DijetMass_MI_log.eps");
  c_DijetMass_normalized_MI->SaveAs("c_DijetMass_normalized_MI.eps");
  c_DijetMass_normalized_MI_log->SaveAs("c_DijetMass_normalized_MI_log.eps");
  c_Eta_Phi_Scatter->SaveAs("c_Eta_Phi_Scatter.eps");
  c_Eta_Phi_Scatter_1->SaveAs("c_Eta_Phi_Scatter_1.eps");
  c_Eta_Phi_Scatter_2->SaveAs("c_Eta_Phi_Scatter_2.eps");
  c_MET_over_sumEt->SaveAs("c_MET_over_sumEt.eps");
  c_MET_over_sumEt_log->SaveAs("c_MET_over_sumEt_log.eps");
  c_EMF->SaveAs("c_EMF.eps");
  c_DPhi->SaveAs("c_DPhi.eps");
  c_DPhi_log->SaveAs("c_DPhi_log.eps");
  c_Eta->SaveAs("c_Eta.eps");
  c_Eta_1->SaveAs("c_Eta_1.eps");
  c_Eta_2->SaveAs("c_Eta_2.eps");
  c_DEta->SaveAs("c_DEta.eps");
  c_Phi->SaveAs("c_Phi.eps");
  c_Phi_1->SaveAs("c_Phi_1.eps");
  c_Phi_2->SaveAs("c_Phi_2.eps");
  c_n90hits->SaveAs("c_n90hits.eps");
  c_n90hits_log->SaveAs("c_n90hits_log.eps");
  c_fHPD->SaveAs("c_fHPD.eps");
  c_corPt->SaveAs("c_corPt.eps");
  c_corPt_1->SaveAs("c_corPt_1.eps");
  c_corPt_2->SaveAs("c_corPt_2.eps");
  c_corPt_log->SaveAs("c_corPt_log.eps");
  c_corPt_log_1->SaveAs("c_corPt_log_1.eps");
  c_corPt_log_2->SaveAs("c_corPt_log_2.eps");
  c_nTrkVtx->SaveAs("c_nTrkVtx.eps");
  c_nTrkCalo->SaveAs("c_nTrkCalo.eps");

  c_fCh->SaveAs("c_fCh.eps");
  c_fPh->SaveAs("c_fPh.eps");
  c_fNh->SaveAs("c_fNh.eps");
  c_fEl->SaveAs("c_fEl.eps");
  */

  //hPFCorMjj->Draw();
  outf->Write();
}


typedef struct 
{
  int   passLooseID;
  int   nTrkVtx;
  int   nTrkCalo;
  int   n90;
  int   n90hits;
  float rawPt;
  float corPt;
  float eta;
  float etaD;
  float y;
  float phi;
  float rawE;
  float corE;
  float trkVtxPt;
  float trkCaloPt;
  float fHPD;
  float fRBX;
  float shapeVar;
  float sigmaEta;
  float sigmaPhi;
  float emf;
  float chf;
  float gendR;
  float genPt;
  float genEta;
  float genPhi;
  float corRsp;
  float rawRsp;
} CALOJET_VAR_mc;

typedef struct 
{
  int   runNo;
  int   evtNo;
  int   lumi;
  int   bunch;
  int   njets;
  int   passLooseHcalNoise;
  int   passTightHcalNoise;
  int   nPV;
  int   PVntracks;
  float PVz;
  float PVx;
  float PVy;
  float PVndof;
  float PVchi2;
  float PVnormalizedChi2;
  float corMass;
  float rawMass; 
  float met;
  float metNoHF;
  float sumet;
  float sumetNoHF;
  float met_over_sumet; 
  float met_over_sumet_NoHF;
  float eemf;
  float echf;
  float pthat;
  float weight;
} EVENT_VAR_mc;
