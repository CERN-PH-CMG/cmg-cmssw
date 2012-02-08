#include "TMath.h"


void ReadTree_new3()
{
  // style definition -------------------------------

  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  //  double dLumi = 4663.;
  double dLumi = 4677.;


  // Canvas definition ----------------------------------

  TCanvas *c_DijetMass = new TCanvas("c_DijetMass","c_DijetMass",459,358,600,602);
  TCanvas *c_DijetMass_fat = new TCanvas("c_DijetMass_fat","c_DijetMass_fat",459,358,600,602);
  TCanvas *c_DijetMass_fat_mu10 = new TCanvas("c_DijetMass_fat_mu10","c_DijetMass_fat_mu10",459,358,600,602);
  TCanvas *c_DijetMass_fat_mu20 = new TCanvas("c_DijetMass_fat_mu20","c_DijetMass_fat_mu20",459,358,600,602);
  TCanvas *c_DijetMass_fat_mu30 = new TCanvas("c_DijetMass_fat_mu30","c_DijetMass_fat_mu30",459,358,600,602);
                      
  TCanvas *c_DijetMass_fat_mu10_log = new TCanvas("c_DijetMass_fat_mu10_log","c_DijetMass_fat_mu10",459,358,600,602);
  TCanvas *c_DijetMass_fat_mu20_log = new TCanvas("c_DijetMass_fat_mu20_log","c_DijetMass_fat_mu20",459,358,600,602);
  TCanvas *c_DijetMass_fat_mu30_log = new TCanvas("c_DijetMass_fat_mu30_log","c_DijetMass_fat_mu30",459,358,600,602);


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
  TCanvas *c_fMu = new TCanvas("c_fMu","c_fMu",459,378,600,602);


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

  TCanvas *c_nVtx = new TCanvas("c_nVtx_fat","c_nVtx_fat",459,378,600,602);

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

  TCanvas *c_Nrad_1_fat = new TCanvas("c_Nrad_1_fat","c_Nrad_1_fat",459,378,600,602);
  TCanvas *c_Nrad_2_fat = new TCanvas("c_Nrad_2_fat","c_Nrad_2_fat",459,378,600,602);

  TCanvas *c_DpT_data_fat = new TCanvas("c_DpT_data_fat","c_DpT_data_fat",459,378,600,602);
  TCanvas *c_DpT_over_pT_data_fat  = new TCanvas("c_DpT_over_pT_data_fat","c_DpT_over_pT_data_fat",459,378,600,602);

           
  TCanvas *c_Deta_fatlead_fat_1 = new TCanvas("c_Deta_fatlead_fat_1","c_Deta_fatlead_fat_1",459,378,600,602);
  TCanvas *c_Deta_fatlead_fat_2 = new TCanvas("c_Deta_fatlead_fat_2","c_Deta_fatlead_fat_2",459,378,600,602);

  TCanvas *c_Dphi_fatlead_fat_1 = new TCanvas("c_Dphi_fatlead_fat_1","c_Dphi_fatlead_fat_1",459,378,600,602);
  TCanvas *c_Dphi_fatlead_fat_2 = new TCanvas("c_Dphi_fatlead_fat_2","c_Dphi_fatlead_fat_2",459,378,600,602);


  // file definition ----------------------------------

  TFile *inf_data  = TFile::Open("out/res/4fb/Fat30/histograms_data_HT_Run2011AB_160404_180252_Fat30_ak5_4677pbm1.root");

  TFile *inf_mc = TFile::Open("out/res/histograms_Fat30_summer11_mc_ak5.root");

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


  // TLegend definition -----------------------------------------------------

  TLegend *l_DijetMass = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_fat = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_fat_log = new TLegend(0.43,0.70,0.93,0.90);
  
  l_DijetMass_fat_log->SetTextSize(0.03);

  TLegend *l_DijetMass_MI = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_MI_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_normalized_MI = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_normalized_MI_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_MET_over_sumEt = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_MET_over_sumEt_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_EMF = new TLegend(0.3456376,0.1602787,0.8573826,0.3292683,NULL,"brNDC");

  TLegend *l_fCh = new TLegend(0.16,0.70,0.51,0.92,NULL,"brNDC");
  TLegend *l_fNh = new TLegend(0.3456376,0.2602787,0.8573826,0.5092683,NULL,"brNDC");
  TLegend *l_fPh = new TLegend(0.49,0.70,0.89,0.92,NULL,"brNDC");
  TLegend *l_fEl = new TLegend(0.3456376,0.4002787,0.8573826,0.7092683,NULL,"brNDC");
  TLegend *l_fMu = new TLegend(0.3456376,0.702787,0.8573826,0.9092683,NULL,"brNDC");

  l_fCh->SetTextSize(0.020);
  l_fPh->SetTextSize(0.025);
  l_fNh->SetTextSize(0.03);
  l_fEl->SetTextSize(0.03);
  l_fMu->SetTextSize(0.03);

  TLegend *l_DPhi = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_log = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_fat = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_fat_log = new TLegend(0.20,0.70,0.70,0.90);

l_DPhi_fat_log->SetTextSize(0.03);

  TLegend *l_Eta = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_1 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_2 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_fat = new TLegend(0.2902685,0.8,0.8053691,0.92,NULL,"brNDC");

l_Eta_fat->SetTextSize(0.03);


  TLegend *l_Eta_fat_1 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_fat_2 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_DEta = new TLegend(0.2567114,0.2195122,0.7651007,0.4198606,NULL,"brNDC");
  TLegend *l_DEta_fat = new TLegend(0.1867114,0.7595122,0.4051007,0.8598606,NULL,"brNDC");

  l_DEta_fat->SetTextSize(0.025);

  TLegend *l_Phi = new TLegend(0.4295302,0.604669,0.9379195,0.9050174,NULL,"brNDC");
 



  TLegend *l_Phi_1 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_2 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_fat = new TLegend(0.3095302,0.604669,0.9379195,0.8050174,NULL,"brNDC");
l_Phi_fat->SetTextSize(0.03);

  TLegend *l_Phi_fat_1 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_fat_2 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_n90hits = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_n90hits_log = new TLegend(0.43,0.70,0.93,0.90);

  TLegend *l_nVtx = new TLegend(0.43,0.70,0.93,0.90);

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
  TLegend *l_corPt_fat_log = new TLegend(0.30,0.70,0.93,0.90);

  l_corPt_fat_log->SetTextSize(0.03);

  TLegend *l_corPt_fat_log_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_fat_log_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_nTrkVtx = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_nTrkCalo = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_JCFCalo = new TLegend(0.43,0.70,0.93,0.90);

  TLegend *l_Nrad_1_fat = new TLegend(0.43,0.80,0.93,0.95);


  TLegend *l_DpT_data_fat = new TLegend(0.43,0.70,0.93,0.90);
           
  TLegend *l_Deta_fatlead_fat_1 = new TLegend(0.60,0.75,0.93,0.90);
  TLegend *l_Deta_fatlead_fat_2 = new TLegend(0.60,0.75,0.93,0.90);

  TLegend *l_Dphi_fatlead_fat_1 = new TLegend(0.60,0.75,0.93,0.90);
  TLegend *l_Dphi_fatlead_fat_2 = new TLegend(0.60,0.75,0.93,0.90);



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
                                                          
  TH1* h_DijetMass_data_fat_mu10= (TH1F *) inf_data->Get("h_DijetMass_data_fat_mu10");
  TH1* h_DijetMass_data_fat_mu20= (TH1F *) inf_data->Get("h_DijetMass_data_fat_mu20");
  TH1* h_DijetMass_data_fat_mu30= (TH1F *) inf_data->Get("h_DijetMass_data_fat_mu30");


  TH1* h_DijetMass_MI_nPVe1_data= (TH1F *) inf_data->Get("h_DijetMass_MI_nPVe1_data");
  TH1* h_DijetMass_MI_nPVg1_data= (TH1F *) inf_data->Get("h_DijetMass_MI_nPVg1_data");
  TH2* h_Eta_Phi_Scatter_data = (TH2F *) inf_data->Get("h_Eta_Phi_Scatter");
  TH2* h_Eta_Phi_Scatter_data_1 = (TH2F *) inf_data->Get("h_Eta_Phi_Scatter_1");
  TH2* h_Eta_Phi_Scatter_data_2 = (TH2F *) inf_data->Get("h_Eta_Phi_Scatter_2");
  TH1* h_MET_over_sumEt_data= (TH1F *) inf_data->Get("h_MET_over_sumEt_fat_data");
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

  TH1* h_nVtx_data= (TH1F *) inf_data->Get("h_Nvx_fat");

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
  TH1 *h_fMu_data_fat = (TH1F *) inf_data->Get("h_fMu_data_pf");


  TH1 *h_Nrad_1_fat =(TH1F *) inf_data->Get("h_Nrad_1_fat");
  TH1 *h_Nrad_2_fat =(TH1F *) inf_data->Get("h_Nrad_2_fat");
  //  TH1 *h_Dphi_fatlead_data_fat = (TH1F *) inf_data->Get("h_Dphi_fatlead_data_fat");
  //  TH1 *h_Deta_fatlead_data_fat = (TH1F *) inf_data->Get("h_Deta_fatlead_data_fat");
  TH1 *h_DpT_over_pT_data_fat = (TH1F *) inf_data->Get("h_DpT_over_pT_data_fat");
  TH1 *h_DpT_data_fat  = (TH1F *) inf_data->Get("h_DpT_data_fat");

  //  TH1 *h_Dphi_fatlead_data_fat_1 = (TH1F *) inf_data->Get("h_Dphi_fatlead_data_fat_1");
  //  TH1 *h_Deta_fatlead_data_fat_1 = (TH1F *) inf_data->Get("h_Deta_fatlead_data_fat_1");
  TH1 *h_DpT_over_pT_data_fat_1 = (TH1F *) inf_data->Get("h_DpT_over_pT_data_fat_1");
  TH1 *h_DpT_data_fat_1  = (TH1F *) inf_data->Get("h_DpT_data_fat_1");

  //  TH1 *h_Dphi_fatlead_data_fat_2 = (TH1F *) inf_data->Get("h_Dphi_fatlead_data_fat_2");
  //  TH1 *h_Deta_fatlead_data_fat_2 = (TH1F *) inf_data->Get("h_Deta_fatlead_data_fat_2");
  TH1 *h_DpT_over_pT_data_fat_2 = (TH1F *) inf_data->Get("h_DpT_over_pT_data_fat_2");
  TH1 *h_DpT_data_fat_2  = (TH1F *) inf_data->Get("h_DpT_data_fat_2");


  TH1 *h_Deta_fatlead_data_fat_1  = (TH1F *) inf_data->Get("h_Deta_data_fat_1;");
  TH1 *h_Deta_fatlead_data_fat_2  = (TH1F *) inf_data->Get("h_Deta_data_fat_2;");

  TH1 *h_Dphi_fatlead_data_fat_1  = (TH1F *) inf_data->Get("h_Dphi_data_fat_1;");
  TH1 *h_Dphi_fatlead_data_fat_2  = (TH1F *) inf_data->Get("h_Dphi_data_fat_2;");





    std::cout << "test1" << std::endl;
  // copy data dijet mass distribution histogram
  
  TH1F *h_DijetMass_normalized_MI_nPVe1_data    =(TH1F*)h_DijetMass_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_normalized_MI_nPVg1_data    =(TH1F*)h_DijetMass_MI_nPVg1_data->Clone();
  // it's originally 3955458(3939510) but two job losted so I used crab -report to get shi value. 
  //float event[8] = {5684160.0, 6336960.0, 4034162.0, 3939510.0, 4161710.0, 2169144.0, 2043020.0, 1058720.0};
  float event[8] = {5671529., 6308796., 3050637., 3939510., 4161710., 2169144., 2043020., 1056368.};
  float xs[8] = {2.426e+04, 1.168e+03, 7.022e+01, 1.555e+01, 1.844e+00, 3.321e-01, 1.087e-02, 3.575e-04};

  ///// inf_mc_00
  TH1 *h_DijetMass_ratio = new TH1F("h_DijetMass_ratio","DijetMass_ratio",83, xAxis1);
  TH1 *h_DijetMass_ratio_fat = new TH1F("h_DijetMass_ratio_fat","DijetMass_ratio_fat",83, xAxis1);


  TH1F* h_DijetMass_mc= (TH1F *) inf_mc->Get("h_DijetMass_data;1");
  TH1F* h_DijetMass_mc_fat= (TH1F *) inf_mc->Get("h_DijetMass_data_fat");
      
  TH1F* h_DijetMass_mc_fat_mu10= (TH1F *) inf_mc->Get("h_DijetMass_data_fat_mu10;1");
  TH1F* h_DijetMass_mc_fat_mu20= (TH1F *) inf_mc->Get("h_DijetMass_data_fat_mu20;1");
  TH1F* h_DijetMass_mc_fat_mu30= (TH1F *) inf_mc->Get("h_DijetMass_data_fat_mu30;1");

  TH1F* h_DijetMass_MI_nPVe1_mc= (TH1F *) inf_mc->Get("h_DijetMass_MI_nPVe1_data");
  TH1F* h_DijetMass_MI_nPVg1_mc= (TH1F *) inf_mc->Get("h_DijetMass_MI_nPVg1_data");
  TH1F* h_MET_over_sumEt_mc= (TH1F *) inf_mc->Get("h_MET_over_sumEt_fat_data");
  TH1F* h_EMF_mc= (TH1F *) inf_mc->Get("h_EMF_data");
  TH1F* h_DPhi_mc= (TH1F *) inf_mc->Get("h_DPhi_data");
  TH1F* h_DPhi_mc_fat= (TH1F *) inf_mc->Get("h_DPhi_data_fat");
  TH1F* h_Eta_mc= (TH1F *) inf_mc->Get("h_Eta_data");
  TH1F* h_Eta_mc_1= (TH1F *) inf_mc->Get("h_Eta_data_1");
  TH1F* h_Eta_mc_2= (TH1F *) inf_mc->Get("h_Eta_data_2");
  TH1F* h_Eta_mc_fat= (TH1F *) inf_mc->Get("h_Eta_data_fat");
  TH1F* h_Eta_mc_fat_1= (TH1F *) inf_mc->Get("h_Eta_data_fat_1");
  TH1F* h_Eta_mc_fat_2= (TH1F *) inf_mc->Get("h_Eta_data_fat_2");
  TH1F* h_DEta_mc= (TH1F *) inf_mc->Get("h_DEta_data");
  TH1F* h_DEta_mc_fat= (TH1F *) inf_mc->Get("h_DEta_data_fat");
  TH1F* h_Phi_mc= (TH1F *) inf_mc->Get("h_Phi_data");
  TH1F* h_Phi_mc_1= (TH1F *) inf_mc->Get("h_Phi_data_1");
  TH1F* h_Phi_mc_2= (TH1F *) inf_mc->Get("h_Phi_data_2");
  TH1F* h_Phi_mc_fat= (TH1F *) inf_mc->Get("h_Phi_data_fat");
  TH1F* h_Phi_mc_fat_1= (TH1F *) inf_mc->Get("h_Phi_data_fat_1");
  TH1F* h_Phi_mc_fat_2= (TH1F *) inf_mc->Get("h_Phi_data_fat_2");
  TH1F* h_n90hits_mc= (TH1F *) inf_mc->Get("h_n90hits_data");

  TH1F* h_nVtx_mc= (TH1F *) inf_mc->Get("h_Nvx_fat");

  TH1F* h_fHPD_mc= (TH1F *) inf_mc->Get("h_fHPD_data");
  TH1F* h_corPt_mc= (TH1F *) inf_mc->Get("h_corPt_data");
  TH1F* h_corPt_mc_1= (TH1F *) inf_mc->Get("h_corPt_data_1");
  TH1F* h_corPt_mc_2= (TH1F *) inf_mc->Get("h_corPt_data_2");
  TH1F* h_corPt_mc_fat= (TH1F *) inf_mc->Get("h_corPt_data_fat");
  TH1F* h_corPt_mc_fat_1= (TH1F *) inf_mc->Get("h_corPt_data_fat_1");
  TH1F* h_corPt_mc_fat_2= (TH1F *) inf_mc->Get("h_corPt_data_fat_2");
  TH1F* h_nTrkVtx_mc= (TH1F *) inf_mc->Get("h_nTrkVtx_data");
  TH1F* h_nTrkCalo_mc= (TH1F *) inf_mc->Get("h_nTrkCalo_data");

  TH1 *h_fCh_mc_fat   =(TH1F *) inf_mc->Get("h_fCh_data_pf");
  TH1 *h_fNh_mc_fat   =(TH1F *) inf_mc->Get("h_fNh_data_pf");
  TH1 *h_fPh_mc_fat   =(TH1F *) inf_mc->Get("h_fPh_data_pf");
  TH1 *h_fEl_mc_fat   =(TH1F *) inf_mc->Get("h_fEl_data_pf");
  TH1 *h_fMu_mc_fat   =(TH1F *) inf_mc->Get("h_fMu_data_pf");

  TH1 *h_Nrad_1_fat_mc  =(TH1F *) inf_mc->Get("h_Nrad_1_fat");
  TH1 *h_Nrad_2_fat_mc  =(TH1F *) inf_mc->Get("h_Nrad_2_fat");
  //  TH1 *h_Dphi_fatlead_data_fat_mc  = (TH1F *) inf_mc->Get("h_Dphi_fatlead_data_fat");
  // TH1 *h_Deta_fatlead_data_fat_mc  = (TH1F *) inf_mc->Get("h_Deta_fatlead_data_fat");
  TH1 *h_DpT_over_pT_data_fat_mc  = (TH1F *) inf_mc->Get("h_DpT_over_pT_data_fat");
  TH1 *h_DpT_data_fat_mc   = (TH1F *) inf_mc->Get("h_DpT_data_fat");

  //  TH1 *h_Dphi_fatlead_data_fat_mc_1  = (TH1F *) inf_mc->Get("h_Dphi_fatlead_data_fat_1");
  //  TH1 *h_Deta_fatlead_data_fat_mc_1  = (TH1F *) inf_mc->Get("h_Deta_fatlead_data_fat_1");
  TH1 *h_DpT_over_pT_data_fat_mc_1  = (TH1F *) inf_mc->Get("h_DpT_over_pT_data_fat_1");
  TH1 *h_DpT_data_fat_mc_1   = (TH1F *) inf_mc->Get("h_DpT_data_fat_1");

  //  TH1 *h_Dphi_fatlead_data_fat_mc_2 = (TH1F *) inf_mc->Get("h_Dphi_fatlead_data_fat_2");
  //  TH1 *h_Deta_fatlead_data_fat_mc_2 = (TH1F *) inf_mc->Get("h_Deta_fatlead_data_fat_2");
  TH1 *h_DpT_over_pT_data_fat_mc_2 = (TH1F *) inf_mc->Get("h_DpT_over_pT_data_fat_2");
  TH1 *h_DpT_data_fat_mc_2  = (TH1F *) inf_mc->Get("h_DpT_data_fat_2");

       
  TH1 *h_Deta_fatlead_mc_fat_1  = (TH1F *) inf_mc->Get("h_Deta_data_fat_1;");
  TH1 *h_Deta_fatlead_mc_fat_2  = (TH1F *) inf_mc->Get("h_Deta_data_fat_2;");
  TH1 *h_Dphi_fatlead_mc_fat_1  = (TH1F *) inf_mc->Get("h_Dphi_data_fat_1;");
  TH1 *h_Dphi_fatlead_mc_fat_2  = (TH1F *) inf_mc->Get("h_Dphi_data_fat_2;");


  
  h_Deta_fatlead_data_fat_1->Sumw2();
  h_Deta_fatlead_data_fat_2->Sumw2();
  h_Dphi_fatlead_data_fat_1->Sumw2();
  h_Dphi_fatlead_data_fat_2->Sumw2();

  h_Nrad_1_fat->Sumw2();
  h_Nrad_2_fat->Sumw2();
  //  h_Dphi_fatlead_data_fat->Sumw2();
  //  h_Deta_fatlead_data_fat->Sumw2();
  h_DpT_over_pT_data_fat->Sumw2();
  h_DpT_data_fat->Sumw2();

  //  h_Dphi_fatlead_data_fat_1->Sumw2();
  //  h_Deta_fatlead_data_fat_1->Sumw2();
  h_DpT_over_pT_data_fat_1->Sumw2();
  h_DpT_data_fat_1->Sumw2();

  //  h_Dphi_fatlead_data_fat_2->Sumw2();
  //  h_Deta_fatlead_data_fat_2->Sumw2();
  h_DpT_over_pT_data_fat_2->Sumw2();
  h_DpT_data_fat_2->Sumw2();

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

  h_nVtx_data->Sumw2();

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

  float DijetMass_data_weight=h_DijetMass_data->Integral()+1e-10;
  float DijetMass_data_fat_weight=h_DijetMass_data_fat->Integral()+1e-10;
  float DijetMass_mc_weight=h_DijetMass_mc->Integral()+1e-10;
  float DijetMass_mc_fat_mu10_weight=h_DijetMass_mc_fat_mu10->Integral()+1e-10;
  float DijetMass_mc_fat_mu20_weight=h_DijetMass_mc_fat_mu20->Integral()+1e-10;
  float DijetMass_mc_fat_mu30_weight=h_DijetMass_mc_fat_mu30->Integral()+1e-10;


  float DijetMass_data_fat_mu10_weight=h_DijetMass_data_fat_mu10->Integral()+1e-10;
  float DijetMass_data_fat_mu20_weight=h_DijetMass_data_fat_mu20->Integral()+1e-10;
  float DijetMass_data_fat_mu30_weight=h_DijetMass_data_fat_mu30->Integral()+1e-10;

  float DijetMass_mc_fat_weight=h_DijetMass_mc_fat->Integral()+1e-10;
  float DijetMass_nPVe1_data_weight=h_DijetMass_MI_nPVe1_data->Integral()+1e-10;
  float DijetMass_nPVg1_data_weight=h_DijetMass_MI_nPVg1_data->Integral()+1e-10;
  float DijetMass_nPVe1_mc_weight=h_DijetMass_MI_nPVe1_mc->Integral()+1e-10;
  float DijetMass_nPVg1_mc_weight=h_DijetMass_MI_nPVg1_mc->Integral()+1e-10;
  float MET_over_sumEt_data_weight=h_MET_over_sumEt_data->Integral()+1e-10;
  float MET_over_sumEt_mc_weight=h_MET_over_sumEt_mc->Integral()+1e-10;
  float EMF_data_weight=h_EMF_data->Integral()+1e-10;
  float EMF_mc_weight=h_EMF_mc->Integral()+1e-10;
  float DPhi_data_weight=h_DPhi_data->Integral()+1e-10;
  float DPhi_data_fat_weight=h_DPhi_data_fat->Integral()+1e-10;
  float DPhi_mc_weight=h_DPhi_mc->Integral()+1e-10;
  float DPhi_mc_fat_weight=h_DPhi_mc_fat->Integral()+1e-10;
  float Eta_data_weight=h_Eta_data->Integral()+1e-10;
  float Eta_data_weight_1=h_Eta_data_1->Integral()+1e-10;
  float Eta_data_weight_2=h_Eta_data_2->Integral()+1e-10;
  float Eta_data_fat_weight=h_Eta_data_fat->Integral()+1e-10;
  float Eta_data_fat_weight_1=h_Eta_data_fat_1->Integral()+1e-10;
  float Eta_data_fat_weight_2=h_Eta_data_fat_2->Integral()+1e-10;
  float Eta_mc_weight=h_Eta_mc->Integral()+1e-10;
  float Eta_mc_weight_1=h_Eta_mc_1->Integral()+1e-10;
  float Eta_mc_weight_2=h_Eta_mc_2->Integral()+1e-10;
  float Eta_mc_fat_weight=h_Eta_mc_fat->Integral()+1e-10;
  float Eta_mc_fat_weight_1=h_Eta_mc_fat_1->Integral()+1e-10;
  float Eta_mc_fat_weight_2=h_Eta_mc_fat_2->Integral()+1e-10;
  float DEta_data_weight=h_Eta_data->Integral()+1e-10;
  float DEta_data_fat_weight=h_Eta_data_fat->Integral()+1e-10;
  float DEta_mc_weight=h_Eta_mc->Integral()+1e-10;
  float DEta_mc_fat_weight=h_Eta_mc_fat->Integral()+1e-10;
  float Phi_data_weight=h_Phi_data->Integral()+1e-10;
  float Phi_data_weight_1=h_Phi_data_1->Integral()+1e-10;
  float Phi_data_weight_2=h_Phi_data_2->Integral()+1e-10;
  float Phi_data_fat_weight=h_Phi_data_fat->Integral()+1e-10;
  float Phi_data_fat_weight_1=h_Phi_data_fat_1->Integral()+1e-10;
  float Phi_data_fat_weight_2=h_Phi_data_fat_2->Integral()+1e-10;
  float Phi_mc_weight=h_Phi_mc->Integral()+1e-10;
  float Phi_mc_weight_1=h_Phi_mc_1->Integral()+1e-10;
  float Phi_mc_weight_2=h_Phi_mc_2->Integral()+1e-10;
  float Phi_mc_fat_weight=h_Phi_mc_fat->Integral()+1e-10;
  float Phi_mc_fat_weight_1=h_Phi_mc_fat_1->Integral()+1e-10;
  float Phi_mc_fat_weight_2=h_Phi_mc_fat_2->Integral()+1e-10;
  float n90hits_data_weight=h_n90hits_data->Integral()+1e-10;
  float n90hits_mc_weight=h_n90hits_mc->Integral()+1e-10;

  float nVtx_data_weight=h_nVtx_data->Integral()+1e-10;
  float nVtx_mc_weight=h_nVtx_mc->Integral()+1e-10;


  float fHPD_data_weight=h_fHPD_data->Integral()+1e-10;
  float fHPD_mc_weight=h_fHPD_mc->Integral()+1e-10;
  float corPt_data_weight=h_corPt_data->Integral()+1e-10;
  float corPt_data_weight_1=h_corPt_data_1->Integral()+1e-10;
  float corPt_data_weight_2=h_corPt_data_2->Integral()+1e-10;
  float corPt_data_fat_weight=h_corPt_data_fat->Integral()+1e-10;
  float corPt_data_fat_weight_1=h_corPt_data_fat_1->Integral()+1e-10;
  float corPt_data_fat_weight_2=h_corPt_data_fat_2->Integral()+1e-10;
  float corPt_mc_weight=h_corPt_mc->Integral()+1e-10;
  float corPt_mc_weight_1=h_corPt_mc_1->Integral()+1e-10;
  float corPt_mc_weight_2=h_corPt_mc_2->Integral()+1e-10;
  float corPt_mc_fat_weight=h_corPt_mc_fat->Integral()+1e-10;
  float corPt_mc_fat_weight_1=h_corPt_mc_fat_1->Integral()+1e-10;
  float corPt_mc_fat_weight_2=h_corPt_mc_fat_2->Integral()+1e-10;
  float nTrkVtx_data_weight=h_nTrkVtx_data->Integral()+1e-10;
  float nTrkVtx_mc_weight=h_nTrkVtx_mc->Integral()+1e-10;
  float nTrkCalo_data_weight=h_nTrkCalo_data->Integral()+1e-10;
  float nTrkCalo_mc_weight=h_nTrkCalo_mc->Integral()+1e-10;


  float fCh_data_weight = h_fCh_data_fat->Integral()+1e-10;
  float fCh_mc_weight = h_fCh_mc_fat->Integral()+1e-10;
  float fNh_data_weight = h_fNh_data_fat->Integral()+1e-10;
  float fNh_mc_weight = h_fNh_mc_fat->Integral()+1e-10;
  float fPh_data_weight = h_fPh_data_fat->Integral()+1e-10;
  float fPh_mc_weight = h_fPh_mc_fat->Integral()+1e-10;
  float fEl_data_weight = h_fEl_data_fat->Integral()+1e-10;
  float fEl_mc_weight = h_fEl_mc_fat->Integral()+1e-10;
  float fMu_data_weight = h_fMu_data_fat->Integral()+1e-10;
  float fMu_mc_weight = h_fMu_mc_fat->Integral()+1e-10;

  
  float fDeta_fatlead_mc_fat_1_mc_weight = h_Deta_fatlead_mc_fat_1->Integral()+1e-10;
  float fDeta_fatlead_mc_fat_2_mc_weight = h_Deta_fatlead_mc_fat_2->Integral()+1e-10;

  float fDphi_fatlead_mc_fat_1_mc_weight = h_Dphi_fatlead_mc_fat_1->Integral()+1e-10;
  float fDphi_fatlead_mc_fat_2_mc_weight = h_Dphi_fatlead_mc_fat_2->Integral()+1e-10;


  float fNrad_1_fat_weight = h_Nrad_1_fat->Integral()+1e-10;
  float fNrad_2_fat_weight = h_Nrad_2_fat->Integral()+1e-10;
  //  float fDphi_fatlead_data_fat_weight = h_Dphi_fatlead_data_fat->Integral()+1e-10;
  //  float fDeta_fatlead_data_fat_weight = h_Deta_fatlead_data_fat->Integral()+1e-10;
  float fDpT_over_pT_data_fat_weight = h_DpT_over_pT_data_fat->Integral()+1e-10;
  float fDpT_data_fat_weight = h_DpT_data_fat->Integral()+1e-10;

  //  float fDphi_fatlead_data_fat_1_weight = h_Dphi_fatlead_data_fat_1->Integral()+1e-10;
  //  float fDeta_fatlead_data_fat_1_weight = h_Deta_fatlead_data_fat_1->Integral()+1e-10;
  float fDpT_over_pT_data_fat_1_weight = h_DpT_over_pT_data_fat_1->Integral()+1e-10;
  float fDpT_data_fat_1_weight = h_DpT_data_fat_1->Integral()+1e-10;

  // float fDphi_fatlead_data_fat_2_weight = h_Dphi_fatlead_data_fat_2->Integral()+1e-10;
  //  float fDeta_fatlead_data_fat_2_weight = h_Deta_fatlead_data_fat_2->Integral()+1e-10;
  float fDpT_over_pT_data_fat_2_weight = h_DpT_over_pT_data_fat_2->Integral()+1e-10;
  float fDpT_data_fat_2_weight = h_DpT_data_fat_2->Integral()+1e-10;


  float fNrad_1_fat_mc_weight = h_Nrad_1_fat_mc->Integral()+1e-10;
  float fNrad_2_fat_mc_weight = h_Nrad_2_fat_mc->Integral()+1e-10;
  //  float fDphi_fatlead_data_fat_mc_weight = h_Dphi_fatlead_data_fat->Integral()+1e-10;
  //  float fDeta_fatlead_data_fat_mc_weight = h_Deta_fatlead_data_fat->Integral()+1e-10;
  float fDpT_over_pT_data_fat_mc_weight = h_DpT_over_pT_data_fat_mc->Integral()+1e-10;
  float fDpT_data_fat_mc_weight = h_DpT_data_fat_mc->Integral()+1e-10;

  //  float fDphi_fatlead_data_fat_1_mc_weight = h_Dphi_fatlead_data_fat_1->Integral()+1e-10;
  //  float fDeta_fatlead_data_fat_1_mc_weight = h_Deta_fatlead_data_fat_1->Integral()+1e-10;
  float fDpT_over_pT_data_fat_1_mc_weight = h_DpT_over_pT_data_fat_mc_1->Integral()+1e-10;
  float fDpT_data_fat_1_mc_weight = h_DpT_data_fat_mc_1->Integral()+1e-10;

  //  float fDphi_fatlead_data_fat_2_mc_weight = h_Dphi_fatlead_data_fat_2->Integral()+1e-10;
  //  float fDeta_fatlead_data_fat_2_mc_weight = h_Deta_fatlead_data_fat_2->Integral()+1e-10;
  float fDpT_over_pT_data_fat_2_mc_weight = h_DpT_over_pT_data_fat_mc_2->Integral()+1e-10;
  float fDpT_data_fat_2_mc_weight = h_DpT_data_fat_mc_2->Integral()+1e-10;

  float fDpT_data_fat_mc_weight = h_DpT_data_fat_mc->Integral()+1e-10;
       
  float fDeta_fatlead_data_fat_1_data_weight = h_Deta_fatlead_data_fat_1->Integral()+1e-10;
  float fDeta_fatlead_data_fat_2_data_weight = h_Deta_fatlead_data_fat_2->Integral()+1e-10;

  float fDphi_fatlead_data_fat_1_data_weight = h_Dphi_fatlead_data_fat_1->Integral()+1e-10;
  float fDphi_fatlead_data_fat_2_data_weight = h_Dphi_fatlead_data_fat_2->Integral()+1e-10;


  //  float JCFCalo_data_weight=h_JCFCalo_data->Integral();
  //  float JCFCalo_mc_weight=h_JCFCalo_mc->Integral();

  cout << "Weights of mass distri = " << DijetMass_data_weight << endl;

  string data_info("CMS 2011 Data Fat PF ( 4.7 fb^{-1} )");

  TFile *outf = new TFile("histogrmas_3500pbm1.root","RECREATE");

   std::cout << "test11" << std::endl;


  // Dijet Mass distribution (PF)
   std::cout << "test11_1" << std::endl;
  c_DijetMass_fat->cd(0);

  gPad->SetTopMargin(0.07);

  h_DijetMass_data_fat->SetTitle("");
  h_DijetMass_data_fat->SetLineWidth(3);
  h_DijetMass_data_fat->GetYaxis()->SetTitle("# Events");
  h_DijetMass_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data_fat->GetXaxis()->SetTitle("M_{12} / GeV");
  h_DijetMass_data_fat->SetMarkerStyle(20);

  double scaleFactor = DijetMass_data_fat_weight/DijetMass_mc_fat_weight * 1./dLumi;

  cout << "Sacle Factor = " << scaleFactor << endl;

  h_DijetMass_mc_fat->SetTitle("Fat dijet invariant mass");
  h_DijetMass_mc_fat->SetLineWidth(3);
  h_DijetMass_mc_fat->SetLineColor(42);
  h_DijetMass_mc_fat->SetFillColor(42);
  h_DijetMass_mc_fat->GetYaxis()->SetTitle("# Events");
  h_DijetMass_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc_fat->GetXaxis()->SetTitle("M ( GeV/c^{2} )");
  h_DijetMass_mc_fat->Scale(DijetMass_data_fat_weight/DijetMass_mc_fat_weight);

  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 0, 4000.);

  PLOTTER->SetMaximum(h_DijetMass_mc_fat->GetMaximum()*1.1);

  PLOTTER->Draw("");
  h_DijetMass_mc_fat->Draw("SAME");
  h_DijetMass_data_fat->Draw("sameEP");

  l_DijetMass_fat->SetFillColor(0);
  l_DijetMass_fat->AddEntry( h_DijetMass_data_fat, data_info.c_str(),"p");
  l_DijetMass_fat->AddEntry( h_DijetMass_mc_fat, Form("QCD PYTHIA6 Z2, fScale = %.2f", scaleFactor),"f");
  l_DijetMass_fat->DrawClone("same");

  c_DijetMass_fat->Update();
  outf->Append(c_DijetMass_fat);


 

  c_DijetMass_fat_log->cd(0);


  gPad->SetTopMargin(0.07);
  TH1F *h_DijetMass_data_fat_log                    =(TH1F*)h_DijetMass_data_fat->Clone();
  TH1F *h_DijetMass_mc_fat_log                      =(TH1F*)h_DijetMass_mc_fat->Clone();

  c_DijetMass_fat_log->SetLogy();

  h_DijetMass_mc_fat_log->SetMinimum(0.2);


  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 500, 4000.);

  PLOTTER->SetTitle("Fat dijet invariant mass; M ( GeV/c^{2} ); # Events");
  PLOTTER->SetMaximum(h_DijetMass_mc_fat->GetMaximum()*2);
  PLOTTER->SetMinimum(0.2);

  PLOTTER->DrawClone("");
  h_DijetMass_mc_fat_log->Draw("SAME");
  h_DijetMass_data_fat_log->Draw("sameEP");

  l_DijetMass_fat_log->SetFillColor(0);
  ///  l_DijetMass_fat_log->AddEntry((TObject*)0,"2011 data","");
  l_DijetMass_fat_log->AddEntry( h_DijetMass_data_fat_log, data_info.c_str(),"p");
  l_DijetMass_fat_log->AddEntry( h_DijetMass_mc_fat_log, Form("QCD PYTHIA6 Z2, fScale = %.2f", scaleFactor),"f");
  l_DijetMass_fat_log->DrawClone("same");

  c_DijetMass_fat_log->Update();
  outf->Append(c_DijetMass_fat_log);





















  c_DijetMass_fat_mu10->cd(0);

  gPad->SetTopMargin(0.07);


  scaleFactor = DijetMass_data_fat_mu10_weight/DijetMass_mc_fat_mu10_weight * 1./dLumi;
  cout << "Sacle Factor mu10 = " << scaleFactor << endl;


  h_DijetMass_data_fat_mu10->SetTitle("");
  h_DijetMass_data_fat_mu10->SetLineWidth(3);
  h_DijetMass_data_fat_mu10->GetYaxis()->SetTitle("# Events");
  h_DijetMass_data_fat_mu10->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data_fat_mu10->GetXaxis()->SetTitle("M_{12} / GeV");
  h_DijetMass_data_fat_mu10->SetMarkerStyle(20);



  h_DijetMass_mc_fat_mu10->SetTitle("Fat_Mu10 dijet invariant mass");
  h_DijetMass_mc_fat_mu10->SetLineWidth(3);
  h_DijetMass_mc_fat_mu10->SetLineColor(42);
  h_DijetMass_mc_fat_mu10->SetFillColor(42);
  h_DijetMass_mc_fat_mu10->GetYaxis()->SetTitle("# Events");
  h_DijetMass_mc_fat_mu10->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc_fat_mu10->GetXaxis()->SetTitle("M ( GeV/c^{2} )");
  h_DijetMass_mc_fat_mu10->Scale(DijetMass_data_fat_mu10_weight/DijetMass_mc_fat_mu10_weight);

  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 0, 4000.);

  PLOTTER->SetMaximum(h_DijetMass_mc_fat_mu10->GetMaximum()*1.1);

  PLOTTER->Draw("");
  h_DijetMass_mc_fat_mu10->Draw("SAME");
  h_DijetMass_data_fat_mu10->Draw("sameEP");

  l_DijetMass_fat->Clear();
  l_DijetMass_fat->SetFillColor(0);
  l_DijetMass_fat->AddEntry( h_DijetMass_data_fat, data_info.c_str(),"p");
  l_DijetMass_fat->AddEntry( h_DijetMass_mc_fat, Form("QCD PYTHIA6 Z2, fScale = %.2f", scaleFactor),"f");
  l_DijetMass_fat->DrawClone("same");

  c_DijetMass_fat_mu10->Update();
  outf->Append(c_DijetMass_fat_mu10);





  c_DijetMass_fat_mu10_log->cd(0);


  gPad->SetTopMargin(0.07);
  TH1F *h_DijetMass_data_fat_mu10_log                    =(TH1F*)h_DijetMass_data_fat_mu10->Clone();
  TH1F *h_DijetMass_mc_fat_mu10_log                      =(TH1F*)h_DijetMass_mc_fat_mu10->Clone();

  c_DijetMass_fat_mu10_log->SetLogy();

  h_DijetMass_mc_fat_mu10_log->SetMinimum(0.2);



  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 500, 4000.);

  PLOTTER->SetTitle("Fat dijet invariant mass; M ( GeV/c^{2} ); # Events");
  PLOTTER->SetMaximum(h_DijetMass_mc_fat->GetMaximum()*2);
  PLOTTER->SetMinimum(0.2);

  PLOTTER->DrawClone("");
  h_DijetMass_mc_fat_mu10_log->Draw("SAME");
  h_DijetMass_data_fat_mu10_log->Draw("sameEP");

  l_DijetMass_fat_log->Clear();
  l_DijetMass_fat_log->AddEntry( h_DijetMass_data_fat_log, data_info.c_str(),"p");
  l_DijetMass_fat_log->AddEntry( h_DijetMass_mc_fat_log, Form("QCD PYTHIA6 Z2, fScale = %.2f", scaleFactor),"f");
  l_DijetMass_fat_log->DrawClone("same");

  c_DijetMass_fat_mu10_log->Update();
  outf->Append(c_DijetMass_fat_mu10_log);








  c_DijetMass_fat_mu20->cd(0);

  gPad->SetTopMargin(0.07);

  h_DijetMass_data_fat_mu20->SetTitle("");
  h_DijetMass_data_fat_mu20->SetLineWidth(3);
  h_DijetMass_data_fat_mu20->GetYaxis()->SetTitle("# Events");
  h_DijetMass_data_fat_mu20->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data_fat_mu20->GetXaxis()->SetTitle("M_{12} / GeV");
  h_DijetMass_data_fat_mu20->SetMarkerStyle(20);



  h_DijetMass_mc_fat_mu20->SetTitle("Fat_Mu20 dijet invariant mass");
  h_DijetMass_mc_fat_mu20->SetLineWidth(3);
  h_DijetMass_mc_fat_mu20->SetLineColor(42);
  h_DijetMass_mc_fat_mu20->SetFillColor(42);
  h_DijetMass_mc_fat_mu20->GetYaxis()->SetTitle("# Events");
  h_DijetMass_mc_fat_mu20->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc_fat_mu20->GetXaxis()->SetTitle("M ( GeV/c^{2} )");
  h_DijetMass_mc_fat_mu20->Scale(DijetMass_data_fat_mu20_weight/DijetMass_mc_fat_mu20_weight);

  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 0, 4000.);

  PLOTTER->SetMaximum(h_DijetMass_mc_fat_mu20->GetMaximum()*1.1);

  PLOTTER->Draw("");
  h_DijetMass_mc_fat_mu20->Draw("SAME");
  h_DijetMass_data_fat_mu20->Draw("sameEP");

  l_DijetMass_fat->Draw("same");

  c_DijetMass_fat_mu20->Update();
  outf->Append(c_DijetMass_fat_mu20);





  c_DijetMass_fat_mu20_log->cd(0);


  gPad->SetTopMargin(0.07);
  TH1F *h_DijetMass_data_fat_mu20_log                    =(TH1F*)h_DijetMass_data_fat_mu20->Clone();
  TH1F *h_DijetMass_mc_fat_mu20_log                      =(TH1F*)h_DijetMass_mc_fat_mu20->Clone();

  c_DijetMass_fat_mu20_log->SetLogy();

  h_DijetMass_mc_fat_mu20_log->SetMinimum(0.2);



  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 500, 4000.);

  PLOTTER->SetTitle("Fat dijet invariant mass; M ( GeV/c^{2} ); # Events");
  PLOTTER->SetMaximum(h_DijetMass_mc_fat->GetMaximum()*2);
  PLOTTER->SetMinimum(0.2);

  PLOTTER->DrawClone("");
  h_DijetMass_mc_fat_mu20_log->Draw("SAME");
  h_DijetMass_data_fat_mu20_log->Draw("sameEP");

  l_DijetMass_fat->Draw("same");

  c_DijetMass_fat_mu20_log->Update();
  outf->Append(c_DijetMass_fat_mu20_log);



  c_DijetMass_fat_mu30->cd(0);

  gPad->SetTopMargin(0.07);

  h_DijetMass_data_fat_mu30->SetTitle("");
  h_DijetMass_data_fat_mu30->SetLineWidth(3);
  h_DijetMass_data_fat_mu30->GetYaxis()->SetTitle("# Events");
  h_DijetMass_data_fat_mu30->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data_fat_mu30->GetXaxis()->SetTitle("M_{12} / GeV");
  h_DijetMass_data_fat_mu30->SetMarkerStyle(20);



  h_DijetMass_mc_fat_mu30->SetTitle("Fat_Mu30 dijet invariant mass");
  h_DijetMass_mc_fat_mu30->SetLineWidth(3);
  h_DijetMass_mc_fat_mu30->SetLineColor(42);
  h_DijetMass_mc_fat_mu30->SetFillColor(42);
  h_DijetMass_mc_fat_mu30->GetYaxis()->SetTitle("# Events");
  h_DijetMass_mc_fat_mu30->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc_fat_mu30->GetXaxis()->SetTitle("M ( GeV/c^{2} )");
  h_DijetMass_mc_fat_mu30->Scale(DijetMass_data_fat_mu30_weight/DijetMass_mc_fat_mu30_weight);

  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 0, 4000.);

  PLOTTER->SetMaximum(h_DijetMass_mc_fat_mu30->GetMaximum()*1.1);

  PLOTTER->Draw("");
  h_DijetMass_mc_fat_mu30->Draw("SAME");
  h_DijetMass_data_fat_mu30->Draw("sameEP");

  l_DijetMass_fat->Draw("same");

  c_DijetMass_fat_mu30->Update();
  outf->Append(c_DijetMass_fat_mu30);




  c_DijetMass_fat_mu30_log->cd(0);


  gPad->SetTopMargin(0.07);
  TH1F *h_DijetMass_data_fat_mu30_log                    =(TH1F*)h_DijetMass_data_fat_mu30->Clone();
  TH1F *h_DijetMass_mc_fat_mu30_log                      =(TH1F*)h_DijetMass_mc_fat_mu30->Clone();

  c_DijetMass_fat_mu30_log->SetLogy();

  h_DijetMass_mc_fat_mu30_log->SetMinimum(0.2);



  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 500, 4000.);

  PLOTTER->SetTitle("Fat dijet invariant mass; M ( GeV/c^{2} ); # Events");
  PLOTTER->SetMaximum(h_DijetMass_mc_fat->GetMaximum()*2);
  PLOTTER->SetMinimum(0.2);

  PLOTTER->DrawClone("");
  h_DijetMass_mc_fat_mu30_log->Draw("SAME");
  h_DijetMass_data_fat_mu30_log->Draw("sameEP");

  l_DijetMass_fat_log->Draw("same");

  c_DijetMass_fat_mu30_log->Update();
  outf->Append(c_DijetMass_fat_mu30_log);




















  c_DijetMass_ratio_fat->cd(0);

  gPad->SetTopMargin(0.07);
  gPad->SetRightMargin(0.05);

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
  h_DijetMass_ratio_fat->GetXaxis()->SetTitle("M_{12} ( GeV^{2} )");
  h_DijetMass_ratio_fat->SetMarkerStyle(20);
  h_DijetMass_ratio_fat->SetMinimum(0.0);

  TH1D* PLOTTER = new TH1D("PLOTTER", "", 1, 500, 4000.);

  PLOTTER->SetTitle("Data to MC ratio; M ( GeV/c^{2} ); Data/MC");
  PLOTTER->SetMaximum(2.8);
  PLOTTER->SetMinimum(0.0);

  PLOTTER->DrawClone("");

  h_DijetMass_ratio_fat->Draw("EPSAME");

  //  TText *t_DijetMass_ratio_fat_1 = pt_DijetMass_ratio_fat->AddText(data_info.c_str());

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


  TText *t_DijetMass_ratio_log_1 = pt_DijetMass_ratio_log->AddText("CMS 2011 Data ( 4.7 fb^{-1} )");

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
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVe1_data,"CMS 2011 Data ( 4.7 fb^{-1} ) with 1 primary vertex","p");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVg1_data,"CMS 2011 Data ( 4.7 fb^{-1} ) with more than 1 primary vertex","p");
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
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVe1_data_log,"CMS 2011 Data ( 4.7 fb^{-1} ) with 1 primary vertex","p");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVg1_data_log,"CMS 2011 Data ( 4.7 fb^{-1} ) with more than 1 primary vertex","p");
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
  l_DijetMass_normalized_MI->AddEntry( h_DijetMass_normalized_MI_nPVe1_data,"CMS 2011 Data ( 4.7 fb^{-1} ) with 1 primary vertex","f");
  l_DijetMass_normalized_MI->AddEntry( h_DijetMass_normalized_MI_nPVg1_data,"CMS 2011 Data ( 4.7 fb^{-1} ) with morethan 1 primary vertex","p");
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
  l_DijetMass_normalized_MI_log->AddEntry( h_DijetMass_normalized_MI_nPVe1_data_log,"CMS 2011 Data ( 4.7 fb^{-1} ) with 1 primary vertex","f");
  l_DijetMass_normalized_MI_log->AddEntry( h_DijetMass_normalized_MI_nPVg1_data_log,"CMS 2011 Data ( 4.7 fb^{-1} ) with morethan 1 primary vertex","p");
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

  TText *t_Eta_Phi_Scatter = pt_Eta_Phi_Scatter->AddText("CMS 2011 Data ( 4.7 fb^{-1} )");

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

  TText *t_Eta_Phi_Scatter_1 = pt_Eta_Phi_Scatter_1->AddText("CMS 2011 Data ( 4.7 fb^{-1} )");

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

  TText *t_Eta_Phi_Scatter_2 = pt_Eta_Phi_Scatter_2->AddText("CMS 2011 Data ( 4.7 fb^{-1} )");

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
  l_MET_over_sumEt->AddEntry(h_MET_over_sumEt_data,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
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
  l_MET_over_sumEt_log->AddEntry(h_MET_over_sumEt_data_log,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_MET_over_sumEt_log->AddEntry(h_MET_over_sumEt_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_MET_over_sumEt_log->Draw("sames");

  c_MET_over_sumEt_log->Update();
  outf->Append(c_MET_over_sumEt_log);




  c_nVtx->cd(0);
  gPad->SetTopMargin(0.07);
  gPad->SetLeftMargin(0.15);
  h_nVtx_mc->SetTitleOffset(1.2, "Y");

  h_nVtx_data->SetTitle("Number of primary vertices");
  h_nVtx_data->GetYaxis()->SetTitle("# events");
  h_nVtx_data->GetYaxis()->SetLabelSize(0.03);
  h_nVtx_data->GetXaxis()->SetTitle("n(Vtx)");
  h_nVtx_data->SetMarkerStyle(20);
  h_nVtx_mc->SetLineWidth(3);
  h_nVtx_mc->SetTitle("Number of primary vertices");
  h_nVtx_mc->GetYaxis()->SetTitle("# Jets");
  h_nVtx_mc->GetYaxis()->SetLabelSize(0.03);
  h_nVtx_mc->GetXaxis()->SetTitle("n(Vtx)");
  h_nVtx_mc->SetFillColor(42);
  h_nVtx_mc->SetLineColor(42);
  h_nVtx_mc->Scale(nVtx_data_weight/nVtx_mc_weight);

  double maximum = h_nVtx_mc->GetMaximum()*1.8;
  if (maximum < h_nVtx_data->GetMaximum()) maximum = h_nVtx_data->GetMaximum()*1.2;
  h_nVtx_mc->SetMaximum(maximum);

  h_nVtx_mc->Draw("");
  h_nVtx_data->Draw("sameEP");

  l_nVtx->SetFillColor(0);
  l_nVtx->AddEntry(h_nVtx_data,data_info.c_str(),"p");
  l_nVtx->AddEntry(h_nVtx_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_nVtx->Draw("sames");

  cout << "Difference factor = " << nVtx_data_weight/nVtx_mc_weight/2200 << endl;

  c_nVtx->Update();
  outf->Append(c_nVtx);







  // EMF


  c_fCh->cd(0);
  gPad->SetTopMargin(0.07);
  gPad->SetLeftMargin(0.15);
  h_fCh_mc_fat->SetTitleOffset(1.2, "Y");

  h_fCh_data_fat->SetTitle("Leading jets Charged Hadron fraction");
  h_fCh_data_fat->GetYaxis()->SetTitle("# Jets");
  h_fCh_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fCh_data_fat->GetXaxis()->SetTitle("f_{ch}");
  h_fCh_data_fat->SetMarkerStyle(20);
  h_fCh_mc_fat->SetLineWidth(3);
  h_fCh_mc_fat->SetTitle("Leading jets Charged Hadron fraction");
  h_fCh_mc_fat->GetYaxis()->SetTitle("# Jets");
  h_fCh_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fCh_mc_fat->GetXaxis()->SetTitle("f_{ch}");
  h_fCh_mc_fat->SetFillColor(42);
  h_fCh_mc_fat->SetLineColor(42);
  h_fCh_mc_fat->Scale(fCh_data_weight/fCh_mc_weight);

  h_fCh_mc_fat->Draw("");
  h_fCh_data_fat->Draw("sameEP");

  l_fCh->SetFillColor(0);
  l_fCh->AddEntry(h_fCh_data_fat,data_info.c_str(),"p");
  l_fCh->AddEntry(h_fCh_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fCh->Draw("sames");

  cout << "Difference factor = " << fCh_data_weight/fCh_mc_weight/3500 << endl;

  c_fCh->Update();
  outf->Append(c_fCh);




  c_fNh->cd(0);
 gPad->SetTopMargin(0.07);
 gPad->SetLeftMargin(0.15);
  h_fNh_mc_fat->SetTitleOffset(1.2, "Y");

  h_fNh_data_fat->SetTitle("Leading jets Neutral Hadron fraction");
  h_fNh_data_fat->GetYaxis()->SetTitle("# Jets");
  h_fNh_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fNh_data_fat->GetXaxis()->SetTitle("f_{nh}");
  h_fNh_data_fat->SetMarkerStyle(20);
  h_fNh_mc_fat->SetLineWidth(3);
  h_fNh_mc_fat->SetTitle("Leading jets Neutral Hadron fraction");
  h_fNh_mc_fat->GetYaxis()->SetTitle("# Jets");
  h_fNh_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fNh_mc_fat->GetXaxis()->SetTitle("f_{nh}");
  h_fNh_mc_fat->SetFillColor(42);
  h_fNh_mc_fat->SetLineColor(42);
  h_fNh_mc_fat->Scale(fNh_data_weight/fNh_mc_weight);

  if (h_fNh_mc_fat->GetMaximum() < h_fNh_data_fat->GetMaximum() ) h_fNh_mc_fat->SetMaximum(h_fNh_data_fat->GetMaximum()*1.1);

  h_fNh_mc_fat->Draw("");
  h_fNh_data_fat->Draw("sameEP");

  l_fNh->SetFillColor(0);
  l_fNh->AddEntry(h_fNh_data_fat,data_info.c_str(),"p");
  l_fNh->AddEntry(h_fNh_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fNh->Draw("sames");

  c_fNh->Update();
  outf->Append(c_fNh);


  c_fPh->cd(0);
 gPad->SetTopMargin(0.07);
 gPad->SetLeftMargin(0.15);
  h_fPh_mc_fat->SetTitleOffset(1.2, "Y");

  h_fPh_data_fat->SetTitle("");
  h_fPh_data_fat->GetYaxis()->SetTitle("# Jets");
  h_fPh_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fPh_data_fat->GetXaxis()->SetTitle("f_{#{gamma}}");
  h_fPh_data_fat->SetMarkerStyle(20);
  h_fPh_mc_fat->SetLineWidth(3);
  h_fPh_mc_fat->SetTitle("Leading jets Photon fraction");
  h_fPh_mc_fat->GetYaxis()->SetTitle("# Jets");
  h_fPh_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fPh_mc_fat->GetXaxis()->SetTitle("f_{#gamma}");
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
 gPad->SetTopMargin(0.07);
 gPad->SetLeftMargin(0.15);
  h_fEl_mc_fat->SetTitleOffset(1.2, "Y");

  h_fEl_data_fat->SetTitle("");
  h_fEl_data_fat->GetYaxis()->SetTitle("# Jets");
  h_fEl_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fEl_data_fat->GetXaxis()->SetTitle("f_{e}");
  h_fEl_data_fat->SetMarkerStyle(20);
  h_fEl_mc_fat->SetLineWidth(3);
  h_fEl_mc_fat->SetTitle("Leading jets Electron fraction");
  h_fEl_mc_fat->GetYaxis()->SetTitle("# Jets");
  h_fEl_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fEl_mc_fat->GetXaxis()->SetTitle("f_{e}");
  h_fEl_mc_fat->SetFillColor(42);
  h_fEl_mc_fat->SetLineColor(42);
  h_fEl_mc_fat->Scale(fEl_data_weight/fEl_mc_weight);

  h_fEl_mc_fat->Draw("");
  h_fEl_data_fat->Draw("sameEP");

  l_fEl->SetFillColor(0);
  l_fEl->AddEntry(h_fEl_data_fat,data_info.c_str(),"p");
  l_fEl->AddEntry(h_fEl_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fEl->Draw("sames");

  c_fEl->SetLogy();

  c_fEl->Update();
  outf->Append(c_fEl);




  c_fMu->cd(0);
  gPad->SetTopMargin(0.07);
  gPad->SetLeftMargin(0.15);
  h_fMu_mc_fat->SetTitleOffset(1.2, "Y");

  fMu_data_weight = h_fMu_data_fat->Integral(2, h_fMu_data_fat->GetNbinsX());
  fMu_mc_weight = h_fMu_mc_fat->Integral(2, h_fMu_mc_fat->GetNbinsX());


  h_fMu_data_fat->SetTitle("");
  h_fMu_data_fat->GetYaxis()->SetTitle("# Jets");
  h_fMu_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_fMu_data_fat->GetXaxis()->SetTitle("f_{e}");
  h_fMu_data_fat->SetMarkerStyle(20);
  h_fMu_mc_fat->SetLineWidth(3);
  h_fMu_mc_fat->SetTitle("Leading jets muon fraction");
  h_fMu_mc_fat->GetYaxis()->SetTitle("# Jets");
  h_fMu_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_fMu_mc_fat->GetXaxis()->SetTitle("f_{mu}");
  h_fMu_mc_fat->SetFillColor(42);
  h_fMu_mc_fat->SetLineColor(42);
  h_fMu_mc_fat->Scale(fMu_data_weight/fMu_mc_weight);

  h_fMu_mc_fat->Draw("");
  h_fMu_data_fat->Draw("sameEP");

  l_fMu->SetFillColor(0);
  l_fMu->AddEntry(h_fMu_data_fat,data_info.c_str(),"p");
  l_fMu->AddEntry(h_fMu_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_fMu->Draw("sames");

  c_fMu->SetLogy();

  c_fMu->Update();
  outf->Append(c_fMu);



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
  l_DPhi->AddEntry(h_DPhi_data,"CMS 2011 Data Calo ( 4.7 fb^{-1} )","p");
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
  h_DPhi_mc_fat->SetTitle("Azimuthal difference of the Fat jets");
  h_DPhi_mc_fat->SetFillColor(42);
  h_DPhi_mc_fat->SetLineColor(42);
  h_DPhi_mc_fat->GetYaxis()->SetTitle("# Events");
  h_DPhi_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_mc_fat->GetXaxis()->SetTitle("#Delta #phi");
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
  l_DPhi_log->AddEntry(h_DPhi_data_log,"CMS 2011 Data Calo ( 4.7 fb^{-1} )","p");
  l_DPhi_log->AddEntry(h_DPhi_mc_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DPhi_log->Draw("sames");

  c_DPhi_log->Update();
  outf->Append(c_DPhi_log);

  // DPhi (log) PF

  c_DPhi_fat_log->cd(0);

  gPad->SetTopMargin(0.07);
   gPad->SetLeftMargin(0.15);

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




  // Eta distribution for two leading jets PF

  c_Eta_fat->cd(0);

  gPad->SetTopMargin(0.07);
   gPad->SetLeftMargin(0.15);

  h_Eta_data_fat->SetTitle("");
  h_Eta_data_fat->GetYaxis()->SetTitle("Fat Jets(two leading jets)");
  h_Eta_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_fat->GetXaxis()->SetTitle("Fat Jet #eta");
  h_Eta_data_fat->SetMarkerStyle(20);

  h_Eta_mc_fat->SetLineWidth(3);
  h_Eta_mc_fat->SetTitle("Fat jets pseudorapidity");
  h_Eta_mc_fat->SetFillColor(42);
  h_Eta_mc_fat->SetLineColor(42);
  h_Eta_mc_fat->GetYaxis()->SetTitle("# Fat jets");
  h_Eta_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_fat->GetXaxis()->SetTitle("#eta");
  h_Eta_mc_fat->Scale(Eta_data_fat_weight/Eta_mc_fat_weight);

  h_Eta_mc_fat->SetTitleOffset(1.2, "Y");

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
  h_Eta_mc_fat_1->SetTitle("Fat jets pseudorapidity");
  h_Eta_mc_fat_1->SetFillColor(42);
  h_Eta_mc_fat_1->SetLineColor(42);
  h_Eta_mc_fat_1->GetYaxis()->SetTitle("# Fat jets");
  h_Eta_mc_fat_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_fat_1->GetXaxis()->SetTitle("#eta");
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




  c_DEta_fat->cd(0);

  gPad->SetTopMargin(0.07);
   gPad->SetLeftMargin(0.15);

  h_DEta_data_fat->SetTitle("");
  h_DEta_data_fat->GetYaxis()->SetTitle("# Events");
  h_DEta_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_DEta_data_fat->GetXaxis()->SetTitle("Fat Jet #Delta#eta");
  h_DEta_data_fat->SetMarkerStyle(20);

  h_DEta_mc_fat->SetLineWidth(3);
  h_DEta_mc_fat->SetTitle("Difference in pseudorapidity of the Fat jets");
  h_DEta_mc_fat->SetTitleOffset(1.1, "Y");

  h_DEta_mc_fat->SetFillColor(42);
  h_DEta_mc_fat->SetLineColor(42);
  h_DEta_mc_fat->GetYaxis()->SetTitle("# Events");
  h_DEta_mc_fat->GetYaxis()->SetLabelSize(0.03);
  h_DEta_mc_fat->GetXaxis()->SetTitle("#Delta#eta");
  h_DEta_mc_fat->Scale(DEta_data_fat_weight/DEta_mc_fat_weight);

  h_DEta_mc_fat->Draw("");
  h_DEta_data_fat->Draw("sameEP");

  l_DEta_fat->SetFillColor(0);
  l_DEta_fat->AddEntry(h_DEta_data_fat,data_info.c_str(),"p");
  l_DEta_fat->AddEntry(h_DEta_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DEta_fat->Draw("sames");

  c_DEta_fat->Update();
  outf->Append(c_DEta_fat);




  // phi distribution for two leading jets PF

  c_Phi_fat->cd(0);
  gPad->SetTopMargin(0.07);
  gPad->SetLeftMargin(0.15);


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

  h_Phi_data_fat->SetTitle("Fat jets azimuthal angle");
  h_Phi_data_fat->GetYaxis()->SetTitle("# Jets");
  h_Phi_data_fat->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_fat->GetXaxis()->SetTitle("#phi");
  h_Phi_data_fat->SetMarkerStyle(20);

  h_Phi_mc_fat->SetLineWidth(3);
  h_Phi_mc_fat->SetTitle("Fat jets azimuthal angle");
  h_Phi_mc_fat->SetFillColor(42);
  h_Phi_mc_fat->SetLineColor(42);
  h_Phi_mc_fat->GetYaxis()->SetTitle("# Fat Jets");
  h_Phi_mc_fat->GetYaxis()->SetLabelSize(0.028);
  h_Phi_mc_fat->GetXaxis()->SetTitle("#phi");
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

  //  TText *t_phi_fat_1 = pt_Phi_fat->AddText(s_average);
  //  TText *t_phi_fat_2 = pt_Phi_fat->AddText(s_rms);
  //  TText *t_phi_fat_3 = pt_Phi_fat->AddText(s_rms_percentage);

  pt_Phi_fat->Draw("sames");

  l_Phi_fat->SetFillColor(0);
  l_Phi_fat->AddEntry(h_Phi_data_fat,data_info.c_str(),"p");
  l_Phi_fat->AddEntry(h_Phi_mc_fat,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
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
  h_Phi_data_fat_1->GetYaxis()->SetLabelSize(0.028);
  h_Phi_data_fat_1->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_data_fat_1->SetMarkerStyle(20);

  h_Phi_mc_fat_1->SetLineWidth(3);
  h_Phi_mc_fat_1->SetTitle("");
  h_Phi_mc_fat_1->SetFillColor(42);
  h_Phi_mc_fat_1->SetLineColor(42);
  h_Phi_mc_fat_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_mc_fat_1->GetYaxis()->SetLabelSize(0.028);
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
  h_Phi_data_fat_2->GetYaxis()->SetLabelSize(0.028);
  h_Phi_data_fat_2->GetXaxis()->SetTitle("Fat Jet #phi");
  h_Phi_data_fat_2->SetMarkerStyle(20);

  h_Phi_mc_fat_2->SetLineWidth(3);
  h_Phi_mc_fat_2->SetTitle("");
  h_Phi_mc_fat_2->SetFillColor(42);
  h_Phi_mc_fat_2->SetLineColor(42);
  h_Phi_mc_fat_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_mc_fat_2->GetYaxis()->SetLabelSize(0.028);
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

  //  TText *t_phi_fat_1 = pt_Phi_fat_2->AddText(s_average);
  // TText *t_phi_fat_2 = pt_Phi_fat_2->AddText(s_rms);
  // TText *t_phi_fat_3 = pt_Phi_fat_2->AddText(s_rms_percentage);

  pt_Phi_fat_2->Draw("sames");

  l_Phi_fat_2->SetFillColor(0);
  l_Phi_fat_2->AddEntry(h_Phi_data_fat_2,data_info.c_str(),"p");
  l_Phi_fat_2->AddEntry(h_Phi_mc_fat_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Phi_fat_2->Draw("sames");

  c_Phi_fat_2->Update();
  outf->Append(c_Phi_fat_2);










  // corrected pt distribution for two leading jets PF

  c_corPt_fat->cd(0);

    gPad->SetTopMargin(0.07);

  h_corPt_data_fat->SetTitle("");
  h_corPt_data_fat->GetYaxis()->SetTitle("# Fat Jets");
  h_corPt_data_fat->GetYaxis()->SetLabelSize(0.028);
  h_corPt_data_fat->GetXaxis()->SetTitle("Fat Jet cor P_{T}");
  h_corPt_data_fat->SetMarkerStyle(20);

  h_corPt_mc_fat->SetLineWidth(3);
  h_corPt_mc_fat->SetTitle("Leading jets transverse momentum");
  h_corPt_mc_fat->SetFillColor(42);
  h_corPt_mc_fat->SetLineColor(42);
  h_corPt_mc_fat->GetYaxis()->SetTitle("# Fat Jets");
  h_corPt_mc_fat->GetYaxis()->SetLabelSize(0.028);
  h_corPt_mc_fat->GetXaxis()->SetTitle("p_{T} (GeV/c)");
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


  c_corPt_fat_log->cd(0);

   gPad->SetTopMargin(0.07);
  
  TH1F *h_corPt_data_fat_log                        =(TH1F*)h_corPt_data_fat->Clone();
  TH1F *h_corPt_mc_fat_log                          =(TH1F*)h_corPt_mc_fat->Clone();

  c_corPt_fat_log->SetLogy();

  h_corPt_mc_fat_log->SetMinimum(0.5);

  gPad->SetRightMargin(0.05);

  h_corPt_mc_fat_log->Draw("");
  h_corPt_data_fat_log->Draw("sameEP");

  l_corPt_fat_log->SetFillColor(0);
  //  l_corPt_fat_log->AddEntry((TObject*)0,"2011 data","");
  l_corPt_fat_log->AddEntry(h_corPt_data_fat_log,data_info.c_str(),"p");
  l_corPt_fat_log->AddEntry(h_corPt_mc_fat_log,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_log->Draw("sames");

  c_corPt_fat_log->Update();
  outf->Append(c_corPt_fat_log);


 

  c_corPt_fat_log_1->cd(0);

   gPad->SetTopMargin(0.07);
  
  TH1F *h_corPt_data_fat_log_1                        =(TH1F*)h_corPt_data_fat_1->Clone();
  TH1F *h_corPt_mc_fat_log_1                          =(TH1F*)h_corPt_mc_fat_1->Clone();

  c_corPt_fat_log_1->SetLogy();
  h_corPt_mc_fat_log_1->Scale(corPt_data_fat_weight/corPt_mc_fat_weight);


  h_corPt_mc_fat_log_1->SetMinimum(0.5);

  gPad->SetRightMargin(0.05);

  h_corPt_data_fat_log_1->SetTitle("");
  h_corPt_data_fat_log_1->GetYaxis()->SetTitle("# Events");
  h_corPt_data_fat_log_1->GetYaxis()->SetLabelSize(0.028);
  h_corPt_data_fat_log_1->GetXaxis()->SetTitle("Fat Jet cor P_{T,1}");
  h_corPt_data_fat_log_1->SetMarkerStyle(20);

  h_corPt_mc_fat_log_1->SetLineWidth(3);
  h_corPt_mc_fat_log_1->SetTitle("First jets transverse momentum");
  h_corPt_mc_fat_log_1->SetFillColor(42);
  h_corPt_mc_fat_log_1->SetLineColor(42);
  h_corPt_mc_fat_log_1->GetYaxis()->SetTitle("# Events");
  h_corPt_mc_fat_log_1->GetYaxis()->SetLabelSize(0.028);
  h_corPt_mc_fat_log_1->GetXaxis()->SetTitle("p_{T,1} (GeV/c)");


  h_corPt_mc_fat_log_1->Draw("");
  h_corPt_data_fat_log_1->Draw("sameEP");

  l_corPt_fat_log_1->SetFillColor(0);
  //  l_corPt_fat_log_1->AddEntry((TObject*)0,"2011 data","");
  l_corPt_fat_log_1->AddEntry(h_corPt_data_fat_log_1,data_info.c_str(),"p");
  l_corPt_fat_log_1->AddEntry(h_corPt_mc_fat_log_1,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_log_1->Draw("sames");

  c_corPt_fat_log_1->Update();
  outf->Append(c_corPt_fat_log_1);



  c_corPt_fat_log_2->cd(0);

   gPad->SetTopMargin(0.07);
  
  TH1F *h_corPt_data_fat_log_2                        =(TH1F*)h_corPt_data_fat_2->Clone();
  TH1F *h_corPt_mc_fat_log_2                          =(TH1F*)h_corPt_mc_fat_2->Clone();

  c_corPt_fat_log_2->SetLogy();

  h_corPt_mc_fat_log_2->SetMinimum(0.5);

  h_corPt_mc_fat_log_2->Scale(corPt_data_fat_weight/corPt_mc_fat_weight);

  h_corPt_data_fat_log_2->SetTitle("");
  h_corPt_data_fat_log_2->GetYaxis()->SetTitle("# Events");
  h_corPt_data_fat_log_2->GetYaxis()->SetLabelSize(0.028);
  h_corPt_data_fat_log_2->GetXaxis()->SetTitle("Fat Jet cor P_{T,2}");
  h_corPt_data_fat_log_2->SetMarkerStyle(20);

  h_corPt_mc_fat_log_2->SetLineWidth(3);
  h_corPt_mc_fat_log_2->SetTitle("Second jets transverse momentum");
  h_corPt_mc_fat_log_2->SetFillColor(42);
  h_corPt_mc_fat_log_2->SetLineColor(42);
  h_corPt_mc_fat_log_2->GetYaxis()->SetTitle("# Events");
  h_corPt_mc_fat_log_2->GetYaxis()->SetLabelSize(0.028);
  h_corPt_mc_fat_log_2->GetXaxis()->SetTitle("p_{T,2} (GeV/c)");


  gPad->SetRightMargin(0.05);

  h_corPt_mc_fat_log_2->Draw("");
  h_corPt_data_fat_log_2->Draw("sameEP");

  l_corPt_fat_log_2->SetFillColor(0);
  //  l_corPt_fat_log_2->AddEntry((TObject*)0,"2011 data","");
  l_corPt_fat_log_2->AddEntry(h_corPt_data_fat_log_2,data_info.c_str(),"p");
  l_corPt_fat_log_2->AddEntry(h_corPt_mc_fat_log_2,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_corPt_fat_log_2->Draw("sames");

  c_corPt_fat_log_2->Update();
  outf->Append(c_corPt_fat_log_2);




  // nTrkVtx distribution for two leading jets

  c_nTrkVtx->cd(0);
  h_nTrkVtx_data->SetTitle("");
  h_nTrkVtx_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkVtx_data->GetYaxis()->SetLabelSize(0.028);
  h_nTrkVtx_data->GetXaxis()->SetTitle("nTrkVtx");
  h_nTrkVtx_data->SetMarkerStyle(20);

  h_nTrkVtx_mc->SetLineWidth(3);
  h_nTrkVtx_mc->SetTitle("");
  h_nTrkVtx_mc->SetFillColor(42);
  h_nTrkVtx_mc->SetLineColor(42);
  h_nTrkVtx_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkVtx_mc->GetYaxis()->SetLabelSize(0.028);
  h_nTrkVtx_mc->GetXaxis()->SetTitle("nTrkVtx");
  h_nTrkVtx_mc->Scale(nTrkVtx_data_weight/nTrkVtx_mc_weight);

  h_nTrkVtx_mc->Draw("");
  h_nTrkVtx_data->Draw("sameEP");

  l_nTrkVtx->SetFillColor(0);
  l_nTrkVtx->AddEntry(h_nTrkVtx_data,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_nTrkVtx->AddEntry(h_nTrkVtx_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_nTrkVtx->Draw("sames");

  c_nTrkVtx->Update();
  outf->Append(c_nTrkVtx);

  // nTrkCalo distribution for two leading jets

  c_nTrkCalo->cd(0);
  h_nTrkCalo_data->SetTitle("");
  h_nTrkCalo_data->GetYaxis()->SetTitle("# Events");
  h_nTrkCalo_data->GetYaxis()->SetLabelSize(0.028);
  h_nTrkCalo_data->GetXaxis()->SetTitle("nTrkCalo");
  h_nTrkCalo_data->SetMarkerStyle(20);

  h_nTrkCalo_mc->SetLineWidth(3);
  h_nTrkCalo_mc->SetTitle("");
  h_nTrkCalo_mc->SetFillColor(42);
  h_nTrkCalo_mc->SetLineColor(42);
  h_nTrkCalo_mc->GetYaxis()->SetTitle("# Events");
  h_nTrkCalo_mc->GetYaxis()->SetLabelSize(0.028);
  h_nTrkCalo_mc->GetXaxis()->SetTitle("nTrkCalo");
  h_nTrkCalo_mc->Scale(nTrkCalo_data_weight/nTrkCalo_mc_weight);

  h_nTrkCalo_mc->Draw("");
  h_nTrkCalo_data->Draw("sameEP");

  l_nTrkCalo->SetFillColor(0);
  l_nTrkCalo->AddEntry(h_nTrkCalo_data,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_nTrkCalo->AddEntry(h_nTrkCalo_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_nTrkCalo->Draw("sames");

  c_nTrkCalo->Update();
  outf->Append(c_nTrkCalo);



  c_Nrad_1_fat->cd(0);
  h_Nrad_1_fat->SetTitle("");
  h_Nrad_1_fat->GetYaxis()->SetTitle("# Events");
  h_Nrad_1_fat->GetYaxis()->SetLabelSize(0.028);
  h_Nrad_1_fat->GetXaxis()->SetTitle("N of subjets in the Fat Jet1");
  h_Nrad_1_fat->SetMarkerStyle(20);

  h_Nrad_1_fat_mc->SetLineWidth(3);
  h_Nrad_1_fat_mc->SetTitle("");
  h_Nrad_1_fat_mc->SetFillColor(42);
  h_Nrad_1_fat_mc->SetLineColor(42);
  h_Nrad_1_fat_mc->GetYaxis()->SetTitle("# Events");
  h_Nrad_1_fat_mc->GetYaxis()->SetLabelSize(0.028);
  h_Nrad_1_fat_mc->GetXaxis()->SetTitle("N of subjets in the Fat Jet1");
  h_Nrad_1_fat_mc->Scale(fNrad_1_fat_weight/fNrad_1_fat_mc_weight);

  h_Nrad_1_fat_mc->SetMaximum(h_Nrad_1_fat_mc->GetMaximum()*1.2);

  h_Nrad_1_fat_mc->Draw("");
  h_Nrad_1_fat->Draw("sameEP");

  l_Nrad_1_fat->SetFillColor(0);
  l_Nrad_1_fat->AddEntry(h_Nrad_1_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_Nrad_1_fat->AddEntry(h_Nrad_1_fat_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Nrad_1_fat->Draw("sames");


  //  c_Nrad_1_fat->SetLogy(1);

  c_Nrad_1_fat->Update();
  outf->Append(c_Nrad_1_fat);


  c_Nrad_2_fat->cd(0);
  h_Nrad_2_fat->SetTitle("");
  h_Nrad_2_fat->GetYaxis()->SetTitle("# Events");
  h_Nrad_2_fat->GetYaxis()->SetLabelSize(0.028);
  h_Nrad_2_fat->GetXaxis()->SetTitle("N of subjets jets in the Fat Jet2");
  h_Nrad_2_fat->SetMarkerStyle(20);

  h_Nrad_2_fat_mc->SetLineWidth(3);
  h_Nrad_2_fat_mc->SetTitle("");
  h_Nrad_2_fat_mc->SetFillColor(42);
  h_Nrad_2_fat_mc->SetLineColor(42);
  h_Nrad_2_fat_mc->GetYaxis()->SetTitle("# Events");
  h_Nrad_2_fat_mc->GetYaxis()->SetLabelSize(0.028);
  h_Nrad_2_fat_mc->GetXaxis()->SetTitle("N of subjets jets in the Fat Jet2");
  h_Nrad_2_fat_mc->Scale(fNrad_2_fat_weight/fNrad_2_fat_mc_weight);

  h_Nrad_2_fat_mc->SetMaximum(h_Nrad_2_fat_mc->GetMaximum()*1.2);

  h_Nrad_2_fat_mc->Draw("");
  h_Nrad_2_fat->Draw("sameEP");

  //  l_Nrad_1_fat->SetFillColor(0);
  //  l_Nrad_1_fat->AddEntry(h_Nrad_2_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  //  l_Nrad_1_fat->AddEntry(h_Nrad_2_fat_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_Nrad_1_fat->Draw("sames");


  //  c_Nrad_2_fat->SetLogy(1);

  c_Nrad_2_fat->Update();
  outf->Append(c_Nrad_2_fat);



 


  c_Deta_fatlead_fat_1->cd(0);
  h_Deta_fatlead_data_fat_1->SetTitle("");
  h_Deta_fatlead_data_fat_1->GetYaxis()->SetTitle("# Events");
  h_Deta_fatlead_data_fat_1->GetYaxis()->SetLabelSize(0.028);
  h_Deta_fatlead_data_fat_1->GetXaxis()->SetTitle("#Delta #eta = #eta_{1, fat} - #eta_{1, jet}");
  h_Deta_fatlead_data_fat_1->SetMarkerStyle(20);

  h_Deta_fatlead_mc_fat_1->SetLineWidth(3);
  h_Deta_fatlead_mc_fat_1->SetTitle("");
  h_Deta_fatlead_mc_fat_1->SetFillColor(42);
  h_Deta_fatlead_mc_fat_1->SetLineColor(42);
  h_Deta_fatlead_mc_fat_1->GetYaxis()->SetTitle("# Events");
  h_Deta_fatlead_mc_fat_1->GetYaxis()->SetLabelSize(0.028);
  h_Deta_fatlead_mc_fat_1->GetXaxis()->SetTitle("#Delta #eta = #eta_{1, fat} - #eta_{1, jet}");
  h_Deta_fatlead_mc_fat_1->Scale(fDeta_fatlead_data_fat_1_data_weight/fDeta_fatlead_mc_fat_1_mc_weight);

  h_Deta_fatlead_mc_fat_1->SetMaximum(h_Deta_fatlead_data_fat_1->GetMaximum()*2);
  h_Deta_fatlead_mc_fat_1->SetMinimum(0.97);

  h_Deta_fatlead_mc_fat_1 ->Draw("");
  h_Deta_fatlead_data_fat_1->Draw("sameEP");

  l_Deta_fatlead_fat_1->SetFillColor(0);
  l_Deta_fatlead_fat_1->AddEntry(h_DpT_data_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_Deta_fatlead_fat_1->AddEntry(h_DpT_data_fat_mc,"QCD  PYTHIA6 Z2","f");
  l_Deta_fatlead_fat_1->Draw("sames");

  //  c_Nrad_1_fat->SetLogy(1);

  c_Deta_fatlead_fat_1->Update();
  outf->Append(c_Deta_fatlead_fat_1);





  c_Deta_fatlead_fat_2->cd(0);
  h_Deta_fatlead_data_fat_2->SetTitle("");
  h_Deta_fatlead_data_fat_2->GetYaxis()->SetTitle("# Events");
  h_Deta_fatlead_data_fat_2->GetYaxis()->SetLabelSize(0.028);
  h_Deta_fatlead_data_fat_2->GetXaxis()->SetTitle("#Delta #eta = #eta_{2, fat} - #eta_{2, jet}");
  h_Deta_fatlead_data_fat_2->SetMarkerStyle(20);

  h_Deta_fatlead_mc_fat_2->SetLineWidth(3);
  h_Deta_fatlead_mc_fat_2->SetTitle("");
  h_Deta_fatlead_mc_fat_2->SetFillColor(42);
  h_Deta_fatlead_mc_fat_2->SetLineColor(42);
  h_Deta_fatlead_mc_fat_2->GetYaxis()->SetTitle("# Events");
  h_Deta_fatlead_mc_fat_2->GetYaxis()->SetLabelSize(0.028);
  h_Deta_fatlead_mc_fat_2->GetXaxis()->SetTitle("#Delta #eta = #eta_{2, fat} - #eta_{2, jet}");
  h_Deta_fatlead_mc_fat_2->Scale(fDeta_fatlead_data_fat_2_data_weight/fDeta_fatlead_mc_fat_2_mc_weight);

  h_Deta_fatlead_mc_fat_2->SetMaximum(h_Deta_fatlead_data_fat_2->GetMaximum()*2);
  h_Deta_fatlead_mc_fat_1->SetMinimum(0.97);

  h_Deta_fatlead_mc_fat_2 ->Draw("");
  h_Deta_fatlead_data_fat_2->Draw("sameEP");

  l_Deta_fatlead_fat_2->SetFillColor(0);
  l_Deta_fatlead_fat_2->AddEntry(h_DpT_data_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_Deta_fatlead_fat_2->AddEntry(h_DpT_data_fat_mc,"QCD  PYTHIA6 Z2","f");
  l_Deta_fatlead_fat_2->Draw("sames");

  //  c_Nrad_2_fat->SetLogy(1);

  c_Deta_fatlead_fat_2->Update();
  outf->Append(c_Deta_fatlead_fat_2);




  c_Dphi_fatlead_fat_1->cd(0);
  h_Dphi_fatlead_data_fat_1->SetTitle("");
  h_Dphi_fatlead_data_fat_1->GetYaxis()->SetTitle("# Events");
  h_Dphi_fatlead_data_fat_1->GetYaxis()->SetLabelSize(0.028);
  h_Dphi_fatlead_data_fat_1->GetXaxis()->SetTitle("#Delta #phi = #phi_{1, fat} - #phi_{1, jet}");
  h_Dphi_fatlead_data_fat_1->SetMarkerStyle(20);

  h_Dphi_fatlead_mc_fat_1->SetLineWidth(3);
  h_Dphi_fatlead_mc_fat_1->SetTitle("");
  h_Dphi_fatlead_mc_fat_1->SetFillColor(42);
  h_Dphi_fatlead_mc_fat_1->SetLineColor(42);
  h_Dphi_fatlead_mc_fat_1->GetYaxis()->SetTitle("# Events");
  h_Dphi_fatlead_mc_fat_1->GetYaxis()->SetLabelSize(0.028);
  h_Dphi_fatlead_mc_fat_1->GetXaxis()->SetTitle("#Delta #phi = #phi_{1, fat} - #phi_{1, jet}");
  h_Dphi_fatlead_mc_fat_1->Scale(fDphi_fatlead_data_fat_1_data_weight/fDphi_fatlead_mc_fat_1_mc_weight);

  h_Dphi_fatlead_mc_fat_1->SetMaximum(h_Dphi_fatlead_data_fat_1->GetMaximum()*2);
  h_Dphi_fatlead_mc_fat_1->SetMinimum(0.97);

  h_Dphi_fatlead_mc_fat_1 ->Draw("");
  h_Dphi_fatlead_data_fat_1->Draw("sameEP");

  l_Dphi_fatlead_fat_1->SetFillColor(0);
  l_Dphi_fatlead_fat_1->AddEntry(h_DpT_data_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_Dphi_fatlead_fat_1->AddEntry(h_DpT_data_fat_mc,"QCD  PYTHIA6 Z2","f");
  l_Dphi_fatlead_fat_1->Draw("sames");

  //  c_Nrad_1_fat->SetLogy(1);

  c_Dphi_fatlead_fat_1->Update();
  outf->Append(c_Dphi_fatlead_fat_1);





  c_Dphi_fatlead_fat_2->cd(0);
  h_Dphi_fatlead_data_fat_2->SetTitle("");
  h_Dphi_fatlead_data_fat_2->GetYaxis()->SetTitle("# Events");
  h_Dphi_fatlead_data_fat_2->GetYaxis()->SetLabelSize(0.028);
  h_Dphi_fatlead_data_fat_2->GetXaxis()->SetTitle("#Delta #phi = #phi_{2, fat} - #phi_{2, jet}");
  h_Dphi_fatlead_data_fat_2->SetMarkerStyle(20);

  h_Dphi_fatlead_mc_fat_2->SetLineWidth(3);
  h_Dphi_fatlead_mc_fat_2->SetTitle("");
  h_Dphi_fatlead_mc_fat_2->SetFillColor(42);
  h_Dphi_fatlead_mc_fat_2->SetLineColor(42);
  h_Dphi_fatlead_mc_fat_2->GetYaxis()->SetTitle("# Events");
  h_Dphi_fatlead_mc_fat_2->GetYaxis()->SetLabelSize(0.028);
  h_Dphi_fatlead_mc_fat_2->GetXaxis()->SetTitle("#Delta #phi = #phi_{2, fat} - #phi_{2, jet}");
  h_Dphi_fatlead_mc_fat_2->Scale(fDphi_fatlead_data_fat_2_data_weight/fDphi_fatlead_mc_fat_2_mc_weight);

  h_Dphi_fatlead_mc_fat_2->SetMaximum(h_Dphi_fatlead_data_fat_2->GetMaximum()*2);

  h_Dphi_fatlead_mc_fat_2->SetMinimum(0.97);
  h_Dphi_fatlead_mc_fat_2 ->Draw("");
  h_Dphi_fatlead_data_fat_2->Draw("sameEP");

  l_Dphi_fatlead_fat_2->SetFillColor(0);
  l_Dphi_fatlead_fat_2->AddEntry(h_DpT_data_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_Dphi_fatlead_fat_2->AddEntry(h_DpT_data_fat_mc,"QCD  PYTHIA6 Z2","f");
  l_Dphi_fatlead_fat_2->Draw("sames");

  //  c_Nrad_2_fat->SetLogy(1);

  c_Dphi_fatlead_fat_2->Update();
  outf->Append(c_Dphi_fatlead_fat_2);








  c_DpT_data_fat->cd(0);
  h_DpT_data_fat->SetTitle("");
  h_DpT_data_fat->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_DpT_data_fat->GetYaxis()->SetLabelSize(0.028);
  h_DpT_data_fat->GetXaxis()->SetTitle("#Delta p_{T} = p_{T, fat} - p_{T, jet}");
  h_DpT_data_fat->SetMarkerStyle(20);

  h_DpT_data_fat_mc->SetLineWidth(3);
  h_DpT_data_fat_mc->SetTitle("");
  h_DpT_data_fat_mc->SetFillColor(42);
  h_DpT_data_fat_mc->SetLineColor(42);
  h_DpT_data_fat_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_DpT_data_fat_mc->GetYaxis()->SetLabelSize(0.028);
  h_DpT_data_fat_mc->GetXaxis()->SetTitle("#Delta p_{T} = p_{T, fat} - p_{T, jet}");
  h_DpT_data_fat_mc->Scale(fDpT_data_fat_weight/fDpT_data_fat_mc_weight);

  h_DpT_data_fat_mc->SetMaximum(h_DpT_data_fat_mc->GetMaximum()*1.2);

  h_DpT_data_fat_mc->Draw("");
  h_DpT_data_fat->Draw("sameEP");

  l_DpT_data_fat->SetFillColor(0);
  l_DpT_data_fat->AddEntry(h_DpT_data_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  l_DpT_data_fat->AddEntry(h_DpT_data_fat_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DpT_data_fat->Draw("sames");

  //  c_Nrad_1_fat->SetLogy(1);

  c_DpT_data_fat->Update();
  outf->Append(c_DpT_data_fat);

  c_DpT_over_pT_data_fat->cd(0);
  h_DpT_over_pT_data_fat->SetTitle("");
  h_DpT_over_pT_data_fat->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_DpT_over_pT_data_fat->GetYaxis()->SetLabelSize(0.028);
  h_DpT_over_pT_data_fat->GetXaxis()->SetTitle("#Delta p_{T} = p_{T, fat} - p_{T, jet}/p_{T, fat}");
  h_DpT_over_pT_data_fat->SetMarkerStyle(20);

  h_DpT_over_pT_data_fat_mc->SetLineWidth(3);
  h_DpT_over_pT_data_fat_mc->SetTitle("");
  h_DpT_over_pT_data_fat_mc->SetFillColor(42);
  h_DpT_over_pT_data_fat_mc->SetLineColor(42);
  h_DpT_over_pT_data_fat_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_DpT_over_pT_data_fat_mc->GetYaxis()->SetLabelSize(0.028);
  h_DpT_over_pT_data_fat_mc->GetXaxis()->SetTitle("#Delta p_{T} = (p_{T, fat} - p_{T, jet})/p_{T, fat}");
  h_DpT_over_pT_data_fat_mc->Scale(fDpT_over_pT_data_fat_weight/fDpT_over_pT_data_fat_mc_weight);

  h_DpT_over_pT_data_fat_mc->SetMaximum(h_DpT_over_pT_data_fat_mc->GetMaximum()*1.2);

  h_DpT_over_pT_data_fat_mc->Draw("");
  h_DpT_over_pT_data_fat->Draw("sameEP");

  //  l_DpT_data_fat->SetFillColor(0);
  //  l_DpT_data_fat->AddEntry(h_DpT_over_pT_data_fat,"CMS 2011 Data ( 4.7 fb^{-1} )","p");
  //  l_DpT_data_fat->AddEntry(h_DpT_over_pT_data_fat_mc,"QCD  PYTHIA6 Z2 + CMS Simulation","f");
  l_DpT_data_fat->Draw("sames");


  //  c_Nrad_1_fat->SetLogy(1);

  c_DpT_over_pT_data_fat->Update();
  outf->Append(c_DpT_over_pT_data_fat);






















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
  outf->Append(h_nVtx_data);

 outf->Append(h_nVtx_mc);

  c_DijetMass_fat->SaveAs("c_DijetMass_fat.png");
  c_DijetMass_fat_log->SaveAs("c_DijetMass_fat_log.png");

  c_DijetMass_fat_mu10->SaveAs("c_DijetMass_fat_mu10.png");
  c_DijetMass_fat_mu10_log->SaveAs("c_DijetMass_fat_mu10_log.png");


  c_DijetMass_fat_mu20->SaveAs("c_DijetMass_fat_mu20.png");
  c_DijetMass_fat_mu20_log->SaveAs("c_DijetMass_fat_mu20_log.png");

  c_DijetMass_fat_mu30->SaveAs("c_DijetMass_fat_mu30.png");
  c_DijetMass_fat_mu30_log->SaveAs("c_DijetMass_fat_mu30_log.png");

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
  c_fMu->SaveAs("c_fMu_fat.png");

  c_Nrad_1_fat->SaveAs("c_Nrad_1_fat.png");
  c_Nrad_1_fat->SetLogy(1);
  c_Nrad_1_fat->SaveAs("c_Nrad_1_fat_log.png");
  c_Nrad_2_fat->SaveAs("c_Nrad_2_fat.png");
  c_Nrad_2_fat->SetLogy(1);
  c_Nrad_2_fat->SaveAs("c_Nrad_2_fat_log.png");


  double integral1 = h_Nrad_1_fat_mc->Integral(2, h_Nrad_1_fat_mc->GetNbinsX());
  double integral2 = h_Nrad_1_fat->Integral(2, h_Nrad_1_fat->GetNbinsX());
  h_Nrad_1_fat_mc->Scale(integral2/integral1);
  c_Nrad_1_fat->Update();
  c_Nrad_1_fat->SaveAs("c_Nrad_1_fat_normToRad_log.png");
  c_Nrad_1_fat->SetLogy(0);
  c_Nrad_1_fat->Update();
  c_Nrad_1_fat->SaveAs("c_Nrad_1_fat_normToRad.png");

  double integral1 = h_Nrad_2_fat_mc->Integral(2, h_Nrad_2_fat_mc->GetNbinsX());
  double integral2 = h_Nrad_2_fat->Integral(2, h_Nrad_2_fat->GetNbinsX());
  h_Nrad_2_fat_mc->Scale(integral2/integral1);
  c_Nrad_2_fat->Update();
  c_Nrad_2_fat->SaveAs("c_Nrad_2_fat_normToRad_log.png");
  c_Nrad_2_fat->SetLogy(0);
  c_Nrad_2_fat->Update();
  c_Nrad_2_fat->SaveAs("c_Nrad_2_fat_normToRad.png");



  c_DpT_data_fat->SaveAs("c_DpT_data_fat.png");

  c_DpT_data_fat->SetLogy(1);
  c_DpT_data_fat->SaveAs("c_DpT_data_fat_log.png");

  c_DpT_over_pT_data_fat->SaveAs("c_DpT_over_pT_data_fat.png");
  c_DpT_over_pT_data_fat->SetLogy(1);
  c_DpT_over_pT_data_fat->SaveAs("c_DpT_over_pT_data_fat_log.png");
  double integral1 = h_DpT_over_pT_data_fat_mc->Integral(2, h_DpT_over_pT_data_fat_mc->GetNbinsX());
  double integral2 = h_DpT_over_pT_data_fat->Integral(2, h_DpT_over_pT_data_fat->GetNbinsX());
  h_DpT_over_pT_data_fat_mc->Scale(integral2/integral1);
  c_DpT_over_pT_data_fat->Update();
  c_DpT_over_pT_data_fat->SaveAs("c_DpT_over_pT_data_fat_normToRad_log.png");
  c_DpT_over_pT_data_fat->SetLogy(0);
  c_DpT_over_pT_data_fat->Update();
  c_DpT_over_pT_data_fat->SaveAs("c_DpT_over_pT_data_fat_normToRad.png");

  double integral1 = h_DpT_data_fat_mc->Integral(2, h_DpT_data_fat_mc->GetNbinsX());
  double integral2 = h_DpT_data_fat->Integral(2, h_DpT_data_fat->GetNbinsX());
  h_DpT_data_fat_mc->Scale(integral2/integral1);
  c_DpT_data_fat->Update();
  c_DpT_data_fat->SaveAs("c_DpT_data_fat_normToRad_log.png");
  c_DpT_data_fat->SetLogy(0);
  c_DpT_data_fat->Update();
  c_DpT_data_fat->SaveAs("c_DpT_data_fat_normToRad.png");


  c_Deta_fatlead_fat_1->SaveAs("c_Deta_fatlead_fat_1.png");
  c_Deta_fatlead_fat_1->SetLogy(1);
  c_Deta_fatlead_fat_1->SaveAs("c_Deta_fatlead_fat_1_log.png");
  c_Deta_fatlead_fat_2->SaveAs("c_Deta_fatlead_fat_2.png");
  c_Deta_fatlead_fat_2->SetLogy(1);
  c_Deta_fatlead_fat_2->SaveAs("c_Deta_fatlead_fat_2_log.png");

  c_nVtx->SaveAs("c_nVtx_fat.png");

  c_Dphi_fatlead_fat_1->SaveAs("c_Dphi_fatlead_fat_1.png");
  c_Dphi_fatlead_fat_1->SetLogy(1);
  c_Dphi_fatlead_fat_1->SaveAs("c_Dphi_fatlead_fat_1_log.png");
  c_Dphi_fatlead_fat_2->SaveAs("c_Dphi_fatlead_fat_2.png");
  c_Dphi_fatlead_fat_2->SetLogy(1);
  c_Dphi_fatlead_fat_2->SaveAs("c_Dphi_fatlead_fat_2_log.png");

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
  */
  c_MET_over_sumEt->SaveAs("c_MET_over_sumEt.png");
  c_MET_over_sumEt_log->SaveAs("c_MET_over_sumEt_log.png");
  /*
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
  c_fMu->SaveAs("c_fMu.png");
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
  c_fMu->SaveAs("c_fMu.eps");
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
