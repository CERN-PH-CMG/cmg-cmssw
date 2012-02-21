#include <iostream>
#include <iomanip>

void draw_1Vtag() 
{
  std::cout << "begin" << std::endl;

  
  gROOT->ProcessLine(".L Macros/setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  //  gSystem->Load("libFWCoreFWLite.so");

  TFile *outf = new TFile("xs.root","RECREATE");

  const unsigned int nEBins = 24;
  double minMass = 950.0, maxMass = 3350.0;
  double minXsec = 1e-3, maxXsec = 5e-1;

  double x[nEBins] = {1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300.};




  // definition of color 

  Int_t ci_g, ci_b;   // for color index setting
  ci_g = TColor::GetColor("#006600");
  ci_b = TColor::GetColor("#000099");

  // definition of canvases

  //TCanvas *c_xs_gg = new TCanvas("c_xs_gg","c_xs_gg",459,358,600,602);
  TCanvas *c_xs_qW = new TCanvas("c_xs_qW","c_xs_qW",459,358,600,602);
  TCanvas *c_xs_qZ = new TCanvas("c_xs_qZ","c_xs_qZ",459,358,600,602);
  TCanvas *c_xs_stat_fat = new TCanvas("c_xs_stat_fat","c_xs_stat_fat",459,358,600,602);
  TCanvas *c_xs_sys = new TCanvas("c_xs_sys","c_xs_sys",459,358,600,602);
  TCanvas *c_xs_all_fat = new TCanvas("c_xs_all_fat","c_xs_all_fat",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_fat = new TCanvas("c_xs_comparison_bw_stat_sys_fat","c_xs_comparison_bw_stat_sys_fat",459,358,600,602);
  //TCanvas *c_xs_gg_compare = new TCanvas("c_xs_gg_compare","c_xs_gg_compare",459,358,600,602);
  //TCanvas *c_xs_qW_compare = new TCanvas("c_xs_qW_compare","c_xs_qW_compare",459,358,600,602);
  //TCanvas *c_xs_qZ_compare = new TCanvas("c_xs_qZ_compare","c_xs_qZ_compare",459,358,600,602);
  TCanvas *c_xs_comparison_bw_sys_theory_fat = new TCanvas("c_xs_comparison_bw_sys_theory_fat","c_xs_comparison_bw_sys_theory_fat",459,358,600,602);

  TCanvas *c_xs_qW_sys_E = new TCanvas("c_xs_qW_sys_E","c_xs_qW_sys_E",459,358,600,602);
  TCanvas *c_xs_qZ_sys_E = new TCanvas("c_xs_qZ_sys_E","c_xs_qZ_sys_E",459,358,600,602);
  //TCanvas *c_xs_rsg = new TCanvas("c_xs_rsg","c_xs_rsg",459,358,600,602);

  // definition of legends

  TLegend *l_xs_theory =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qW =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qZ =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");

  //TLegend *l_xs_gg =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qW =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qZ =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_stat_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_sys =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_all_fat =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  //TLegend *l_xs_gg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qW_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qZ_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_fat =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_qW_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qZ_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_rsg =  new TLegend(0.5,0.6,0.8,0.9,NULL,"brNDC");

  TPaveText *p_xs_qW_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");
  TPaveText *p_xs_qZ_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");

  TPaveText *p_xs_sys = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_fat = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");

  // settings of canvases

  //c_xs_gg->SetLogy();
  c_xs_qW->SetLogy();
  c_xs_qZ->SetLogy();
  c_xs_stat_fat->SetLogy();
  c_xs_sys->SetLogy();
  c_xs_all_fat->SetLogy();
  c_xs_comparison_bw_sys_theory_fat->SetLogy();
  c_xs_qW_sys_E->SetLogy();
  c_xs_qZ_sys_E->SetLogy();

  // theory values

 const unsigned int en = 4;

 double mjj [en] ={1000., 1500., 2000., 3000.};

  double qstar_qW [en] = {
    /*5.207e+2, 7.785e+1, */1.696e+1, 1.411e+0, 1.653e-1, 3.165e-3};
 
  double qstar_qZ[en] = {
    /*1.870e+2, 2.746e+1, */6.045e+0, 4.974e-1, 5.775e-2, 1.072e-3};

  double qstar_kfactor[en] = {
    1.0, 1.0, 1.0, 1.0};

  double qstar_qW_acc[en] = {
    0.455357146813, 0.455357146813, 0.5008267179, 0.5008267179};
 
  double qstar_qZ_acc[en] = {
    0.444742057676, 0.444742057676, 0.4648809582, 0.4648809582};

  double qstar_qW_eff[en] = {
    0.383442270512, 0.383442270512, 0.338461542719, 0.338461542719};
 
  double qstar_qZ_eff[en] = {
    0.477656335365, 0.477656335365, 0.318395214308, 0.318395214308};

  TGraph* qstar_qW_graph = new TGraph(en,mjj,qstar_qW);
  TF1* qstar_qW_f = new TF1("qW","pol2",minMass,maxMass);
  qstar_qW_graph->Fit(qstar_qW_f);

  TGraph* qstar_qZ_graph = new TGraph(en,mjj,qstar_qZ);
  TF1* qstar_qZ_f = new TF1("qZ","pol2",minMass,maxMass);
  qstar_qZ_graph->Fit(qstar_qZ_f);

  TGraph* qstar_qW_eff_graph = new TGraph(en,mjj,qstar_qW_eff);
  TF1* qstar_qW_efficiency = new TF1("qWeff","pol1",minMass,maxMass);
  qstar_qW_eff_graph->Fit(qstar_qW_efficiency);

  TGraph* qstar_qZ_eff_graph = new TGraph(en,mjj,qstar_qZ_eff);
  TF1* qstar_qZ_efficiency = new TF1("qZeff","pol1",minMass,maxMass);
  qstar_qZ_eff_graph->Fit(qstar_qZ_efficiency);

  cout << "acceptance: " << endl;
  for (int i = 0; i < en; i++){
    cout << qstar_qW_acc[i] << "/" << qstar_qZ_acc[i] << ", ";
    qstar_qW[i] *= qstar_kfactor[i]*qstar_qW_acc[i];
    qstar_qZ[i] *= qstar_kfactor[i]*qstar_qZ_acc[i];
  }
  cout << endl;

// 600       1.07
// 700       1.08
// 800       1.09
// 900       1.10
// 1000 1.11 1.11 1.12
// 1100      1.12
// 1200 1.13 1.13 1.14
// 1300      1.14
// 1400 1.15 1.15 1.16
// 1500      1.16
// 1600 1.17 1.17 1.18
// 1700      1.18
// 1800 1.20 1.20 1.21
// 1900      1.21
// 2000 1.22 1.22 1.23

  /*double diquark[en] = {
    0.1451E+03, 0.8646E+02, 0.5435E+02, 0.3554E+02, 0.2393E+02, 0.1648E+02, 0.1154E+02, 0.8194E+01, 0.5877E+01, 0.4249E+01,
    0.4249E+01, 0.3090E+01, 0.2258E+01, 0.1656E+01, 0.1217E+01};

  double string[en] = {
    19744.01, 9304.34, 4627.17, 2485.0, 1391.71, 787.875, 473.13, 290.1, 177.6, 111.91,
    72.12, 47.07, 31.05, 20.60, 13.82};

  double zprime[en] = {
    0.1211E+02, 0.6246E+01, 0.3427E+01, 0.1969E+01, 0.1172E+01, 0.7171E+00, 0.4486E+00, 0.2857E+00, 0.1845E+00, 0.1206E+00,
    0.7961E-01, 0.5295E-01, 0.3545E-01, 0.2386E-01, 0.1611E-01}; 

  double wprime[en] = {
    0.2125E+02, 0.1120E+02, 0.6263E+01, 0.3661E+01, 0.2212E+01, 0.1372E+01, 0.8673E+00, 0.5568E+00, 0.3616E+00, 0.2369E+00,
    0.1562E+00, 0.1034E+00, 0.6872E-01, 0.4572E-01, 0.3043E-01};

  double rsgraviton[en] = {
    0.1862E+02, 0.8100E+01, 0.3852E+01, 0.1961E+01, 0.1053E+01, 0.5905E+00, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01,
    0.4911E-01, 0.3145E-01, 0.2036E-01, 0.1330E-01, 0.8743E-02};  
  */

 TFile* file_efficiencies_qZ = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_Qstar_qZ_ak5_fat.root", "READ");
 TFile* file_efficiencies_qW = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_Qstar_qW_ak5_fat.root", "READ");
 //TFile* file_efficiencies_gg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_RSGraviton_HLT_ak5_GGtoGG_fat.root", "READ");

 TH1F* limit_efficiencies_qZ = file_efficiencies_qZ->Get("Efficiency;1");
 TH1F* limit_efficiencies_qW = file_efficiencies_qW->Get("Efficiency;1");
 //TH1F* limit_efficiencies_gg = file_efficiencies_gg->Get("Efficiency;1");


 TFile* file_qZ0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qZ_ak5_fat_0.root", "READ");
 TFile* file_qW0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qW_ak5_fat_0.root", "READ");
 //TFile* file_gg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_HLT_ak5_GGtoGG_fat_0.root", "READ");

 
 TH1F* limit_qZ0 = file_qZ0->Get("limit;1");
 TH1F* limit_qW0 = file_qW0->Get("limit;1");
 //TH1F* limit_gg0 = file_gg0->Get("limit;1");

 cout << "efficiency: " << endl;
 for (int i = 1; i < limit_efficiencies_qZ->GetNbinsX()+1; i++){

   // V-tagging efficincy
   cout << qstar_qZ_efficiency->Eval(limit_efficiencies_qZ->GetBinCenter(i)) << "/" << qstar_qW_efficiency->Eval(limit_efficiencies_qW->GetBinCenter(i)) << ", ";
   limit_efficiencies_qZ->SetBinContent(i, limit_efficiencies_qZ->GetBinContent(i) * qstar_qZ_efficiency->Eval(limit_efficiencies_qZ->GetBinCenter(i)));
   limit_efficiencies_qW->SetBinContent(i, limit_efficiencies_qW->GetBinContent(i) * qstar_qW_efficiency->Eval(limit_efficiencies_qW->GetBinCenter(i)));

 }
  cout << endl;

 for (int i = 1; i < limit_qZ0->GetNbinsX()+1; i++){

   limit_qZ0->SetBinContent(i, limit_qZ0->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
   limit_qW0->SetBinContent(i, limit_qW0->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   //limit_gg0->SetBinContent(i, limit_gg0->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

 }

  // fat-stat only --
 double* xs_HT_fat_qZ_1010 = new double[nEBins];
 double* xs_HT_fat_qW_1010 = new double[nEBins];
 //double* xs_HT_fat_gg_1010 = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qZ_1010[i] = limit_qZ0->GetBinContent(i+1);
   xs_HT_fat_qW_1010[i] = limit_qW0->GetBinContent(i+1);
   //xs_HT_fat_gg_1010[i] = limit_gg0->GetBinContent(i+1);
 }

 file_qZ0->Close();
 file_qW0->Close();
 //file_gg0->Close(); 


 cout << "looking differentially for systematics" << endl;

 


 TFile* file_qW_bg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qW_ak5_fat_1.root", "READ");
 TFile* file_qW_lumi = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qW_ak5_fat_2.root", "READ");
 TFile* file_qW_JES = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qW_ak5_fat_3.root", "READ");
 TFile* file_qW_JER = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qW_ak5_fat_4.root", "READ");

 
 TH1F* limit_qW_bg = file_qW_bg->Get("limit;1");
 TH1F* limit_qW_lumi = file_qW_lumi->Get("limit;1");
 TH1F* limit_qW_JES = file_qW_JES->Get("limit;1");
 TH1F* limit_qW_JER = file_qW_JER->Get("limit;1");

 
 for (int i = 1; i < limit_qW_bg->GetNbinsX()+1; i++){

   limit_qW_bg->SetBinContent(i, limit_qW_bg->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_lumi->SetBinContent(i, limit_qW_lumi->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_JES->SetBinContent(i, limit_qW_JES->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_JER->SetBinContent(i, limit_qW_JER->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
 }
 

  // fat-stat only --
 double* xs_HT_fat_qW_1010_bg = new double[nEBins];
 double* xs_HT_fat_qW_1010_lumi = new double[nEBins];
 double* xs_HT_fat_qW_1010_JES = new double[nEBins];
 double* xs_HT_fat_qW_1010_JER = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qW_1010_bg[i] = limit_qW_bg->GetBinContent(i+1);
   xs_HT_fat_qW_1010_lumi[i] = limit_qW_lumi->GetBinContent(i+1);
   xs_HT_fat_qW_1010_JES[i] = limit_qW_JES->GetBinContent(i+1);
   xs_HT_fat_qW_1010_JER[i] = limit_qW_JER->GetBinContent(i+1);
 }

 file_qW_bg->Close();
 file_qW_lumi->Close();
 file_qW_JES->Close();
 file_qW_JER->Close();


 cout << "looking for All systematics" << endl;
  // fat-All_Fat sys.included
 TFile* file_qZ6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qZ_ak5_fat_6.root", "READ");
 TFile* file_qW6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_qW_ak5_fat_6.root", "READ");
 //TFile* file_gg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_HLT_ak5_GGtoGG_fat_6.root", "READ");

 
 TH1F* limit_qZ6 = file_qZ6->Get("limit;1");
 TH1F* limit_qW6 = file_qW6->Get("limit;1");
 //TH1F* limit_gg6 = file_gg6->Get("limit;1");

 for (int i = 1; i < limit_qZ6->GetNbinsX()+1; i++){

   cout << "Reweighting" << endl;
   limit_qZ6->SetBinContent(i, limit_qZ6->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
   limit_qW6->SetBinContent(i, limit_qW6->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   //limit_gg6->SetBinContent(i, limit_gg6->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

 }


  // fat-stat only --
 double* xs_HT_fat_qZ_1010_sys = new double[nEBins];
 double* xs_HT_fat_qW_1010_sys = new double[nEBins];
 //double* xs_HT_fat_gg_1010_sys = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qZ_1010_sys[i] = limit_qZ6->GetBinContent(i+1);
   xs_HT_fat_qW_1010_sys[i] = limit_qW6->GetBinContent(i+1);
   //xs_HT_fat_gg_1010_sys[i] = limit_gg6->GetBinContent(i+1);
 }

 file_qZ6->Close();
 file_qW6->Close();
 //file_gg6->Close(); 

 // TODO: should be _6 instead of _0
 TFile* file_exp_qZ6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_expectedlimit50_Qstar_qZ_ak5_fat_6.root", "READ");
 TFile* file_exp_qW6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_expectedlimit50_Qstar_qW_ak5_fat_6.root", "READ");
 //TFile* file_exp_gg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_expectedlimit50_RSGraviton_HLT_ak5_GGtoGG_fat_6.root", "READ");


 TH1F* limit_qZ_twosigma_lowers = file_exp_qZ6->Get("htwosigma_lowers;1");
 TH1F* limit_qZ_onesigma_lowers = file_exp_qZ6->Get("honesigma_lowers;1");
 TH1F* limit_qZ_medians = file_exp_qZ6->Get("hmedians;1");
 TH1F* limit_qZ_onesigma_uppers = file_exp_qZ6->Get("honesigma_uppers;1");
 TH1F* limit_qZ_twosigma_uppers = file_exp_qZ6->Get("htwosigma_uppers;1");
 

 for (int i = 1; i < limit_qZ_twosigma_lowers->GetNbinsX()+1; i++){

   limit_qZ_twosigma_lowers->SetBinContent(i, limit_qZ_twosigma_lowers->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
   limit_qZ_onesigma_lowers->SetBinContent(i, limit_qZ_onesigma_lowers->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
   limit_qZ_medians->SetBinContent(i, limit_qZ_medians->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
   limit_qZ_onesigma_uppers->SetBinContent(i, limit_qZ_onesigma_uppers->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
   limit_qZ_twosigma_uppers->SetBinContent(i, limit_qZ_twosigma_uppers->GetBinContent(i) / limit_efficiencies_qZ->GetBinContent(i));
 }

 
 double* xs_HT_fat_qZ_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_qZ_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_qZ_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_qZ_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_qZ_1010_sys_E_u2 = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_qZ_1010_sys_E_d2[i] = limit_qZ_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qZ_1010_sys_E_d1[i] = limit_qZ_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qZ_1010_sys_E_m[i] = limit_qZ_medians->GetBinContent(i+1);
   xs_HT_fat_qZ_1010_sys_E_u1[i] = limit_qZ_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_qZ_1010_sys_E_u2[i] = limit_qZ_twosigma_uppers->GetBinContent(i+1);

 }


 TH1F* limit_qW_twosigma_lowers = file_exp_qW6->Get("htwosigma_lowers;1");
 TH1F* limit_qW_onesigma_lowers = file_exp_qW6->Get("honesigma_lowers;1");
 TH1F* limit_qW_medians = file_exp_qW6->Get("hmedians;1");
 TH1F* limit_qW_onesigma_uppers = file_exp_qW6->Get("honesigma_uppers;1");
 TH1F* limit_qW_twosigma_uppers = file_exp_qW6->Get("htwosigma_uppers;1");

 for (int i = 1; i < limit_qW_twosigma_lowers->GetNbinsX()+1; i++){

   limit_qW_twosigma_lowers->SetBinContent(i, limit_qW_twosigma_lowers->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_onesigma_lowers->SetBinContent(i, limit_qW_onesigma_lowers->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_medians->SetBinContent(i, limit_qW_medians->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_onesigma_uppers->SetBinContent(i, limit_qW_onesigma_uppers->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
   limit_qW_twosigma_uppers->SetBinContent(i, limit_qW_twosigma_uppers->GetBinContent(i) / limit_efficiencies_qW->GetBinContent(i));
 }


 double* xs_HT_fat_qW_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_qW_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_qW_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_qW_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_qW_1010_sys_E_u2 = new double[nEBins];





 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_qW_1010_sys_E_d2[i] = limit_qW_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qW_1010_sys_E_d1[i] = limit_qW_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qW_1010_sys_E_m[i] = limit_qW_medians->GetBinContent(i+1);
   xs_HT_fat_qW_1010_sys_E_u1[i] = limit_qW_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_qW_1010_sys_E_u2[i] = limit_qW_twosigma_uppers->GetBinContent(i+1);

 }


 /*TH1F* limit_gg_twosigma_lowers = file_exp_gg6->Get("htwosigma_lowers;1");
 TH1F* limit_gg_onesigma_lowers = file_exp_gg6->Get("honesigma_lowers;1");
 TH1F* limit_gg_medians = file_exp_gg6->Get("hmedians;1");
 TH1F* limit_gg_onesigma_uppers = file_exp_gg6->Get("honesigma_uppers;1");
 TH1F* limit_gg_twosigma_uppers = file_exp_gg6->Get("htwosigma_uppers;1");

 for (int i = 1; i < limit_gg_twosigma_lowers->GetNbinsX()+1; i++){

   limit_gg_twosigma_lowers->SetBinContent(i, limit_gg_twosigma_lowers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_onesigma_lowers->SetBinContent(i, limit_gg_onesigma_lowers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_medians->SetBinContent(i, limit_gg_medians->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_onesigma_uppers->SetBinContent(i, limit_gg_onesigma_uppers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_twosigma_uppers->SetBinContent(i, limit_gg_twosigma_uppers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
 }
 
 double* xs_HT_fat_gg_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_gg_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_gg_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_gg_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_gg_1010_sys_E_u2 = new double[nEBins];

 
 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_gg_1010_sys_E_d2[i] = limit_gg_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_gg_1010_sys_E_d1[i] = limit_gg_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_gg_1010_sys_E_m[i] = limit_gg_medians->GetBinContent(i+1);
   xs_HT_fat_gg_1010_sys_E_u1[i] = limit_gg_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_gg_1010_sys_E_u2[i] = limit_gg_twosigma_uppers->GetBinContent(i+1);

 }
 */
 file_exp_qZ6->Close();
 file_exp_qW6->Close();
 //file_exp_gg6->Close();


 // 0.16751, 0.15361, 0.14070, 0.12884, 0.11799, 0.10813
 //   0.19590, 0.21457, 
// double gg_ratio[nEBins] = {0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.45, 0.46, 0.47, 0.48}; //{0.5561,0.5116,0.4671, 0.42, 0.38};
// double qZ_ratio[nEBins] = {0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.55, 0.55, 0.53, 0.52};
 
 double xs_HT_pf_qW_compare[nEBins], xs_HT_pf_gg_compare[nEBins], xs_HT_pf_qZ_compare[nEBins], xs_HT_fat_qW_compare[nEBins], xs_HT_fat_gg_compare[nEBins], xs_HT_fat_qZ_compare[nEBins], xs_comparison_bw_stat_bg_fat[nEBins], xs_comparison_bw_stat_lumi_fat[nEBins], xs_comparison_bw_stat_JES_fat[nEBins], xs_comparison_bw_stat_JER_fat[nEBins], xs_comparison_bw_stat_sys_fat[nEBins], xs_RSG_for_limit_fat[nEBins], xs_qstar_qW_over_qW_fat[nEBins], xs_string_over_qW_fat[nEBins], xs_diquark_over_qZ_fat[nEBins], xs_qstar_qZ_over_qZ_fat[nEBins], xs_zprime_over_qZ_fat[nEBins], xs_wprime_over_qZ_fat[nEBins], xs_rsg_over_gg_fat[nEBins];

 std::cout << "wide stat only" << std::endl;
 for (int  i = 0 ; i < nEBins ; i++) 
   {
     std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qZ_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qW_1010[i]/* <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010[i]*/ << " \\\\" << std::endl;
     
   }
 
 
 std::cout << "wide sys included" << std::endl;
 for (int  i = 0 ; i < nEBins ; i++) 
   {
     std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qZ_1010_sys[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qW_1010_sys[i]/* <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010_sys[i]*/ << " \\\\" << std::endl;
     
   }
 
////////////////// Histogram definition /////////////////////
/////////////////////////////////////////////////////////////


  // expected limit for qW

  TH1F *h_xs_HT_fat_qW_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qW_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qW_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qW_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

  // expected limit for qZ

  TH1F *h_xs_HT_fat_qZ_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qZ_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qZ_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qZ_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

		 

 for (int i = 0; i<nEBins; i++) {
   
   std::cout << i << " & " << std::fixed << setprecision(3) << xs_HT_fat_qZ_1010_sys[i] << " & " << xs_HT_fat_qW_1010_sys[i]/* << " & " << xs_HT_fat_gg_1010_sys[i]*/ << " \\\\  " << std::endl;
   //xs_RSG_for_limit_fat[i] = xs_HT_fat_gg_1010_sys[i]*gg_ratio[i] + xs_HT_fat_qZ_1010_sys[i]*qZ_ratio[i];
   //xs_rsg_over_gg_fat[i] = rsgraviton[i]/xs_RSG_for_limit_fat[i];
     
   xs_comparison_bw_stat_bg_fat[i] = xs_HT_fat_qW_1010_bg[i]/xs_HT_fat_qW_1010[i];
   xs_comparison_bw_stat_lumi_fat[i] = xs_HT_fat_qW_1010_lumi[i]/xs_HT_fat_qW_1010[i];
   xs_comparison_bw_stat_JES_fat[i] = xs_HT_fat_qW_1010_JES[i]/xs_HT_fat_qW_1010[i];
   xs_comparison_bw_stat_JER_fat[i] = xs_HT_fat_qW_1010_JER[i]/xs_HT_fat_qW_1010[i];
   xs_comparison_bw_stat_sys_fat[i] = xs_HT_fat_qW_1010_sys[i]/xs_HT_fat_qW_1010[i];

   xs_qstar_qW_over_qW_fat[i] = qstar_qW_f->Eval(x[i])/xs_HT_fat_qW_1010_sys[i];
   //xs_string_over_qW_fat[i] = string[i]/xs_HT_fat_qW_1010_sys[i];
   //xs_diquark_over_qZ_fat[i] = diquark[i]/xs_HT_fat_qZ_1010_sys[i];
   xs_qstar_qZ_over_qZ_fat[i] = qstar_qZ_f->Eval(x[i])/xs_HT_fat_qZ_1010_sys[i];
   //xs_zprime_over_qZ_fat[i] = zprime[i]/xs_HT_fat_qZ_1010_sys[i];
   //xs_wprime_over_qZ_fat[i] = wprime[i]/xs_HT_fat_qZ_1010_sys[i];
   
   h_xs_HT_fat_qW_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_qW_1010_sys_E_d2[i]);
   h_xs_HT_fat_qW_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_qW_1010_sys_E_d1[i]);
   h_xs_HT_fat_qW_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_qW_1010_sys_E_u1[i]);
   h_xs_HT_fat_qW_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_qW_1010_sys_E_u2[i]);

   h_xs_HT_fat_qZ_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_qZ_1010_sys_E_d2[i]);
   h_xs_HT_fat_qZ_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_qZ_1010_sys_E_d1[i]);
   h_xs_HT_fat_qZ_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_qZ_1010_sys_E_u1[i]);
   h_xs_HT_fat_qZ_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_qZ_1010_sys_E_u2[i]);

 }

//////////////////////////////////////////////////////
////////////////// TGraph define /////////////////////
//////////////////////////////////////////////////////

// stat only 

  g_xs_HT_fat_qW_1010 = new TGraph(nEBins,x,xs_HT_fat_qW_1010);
  //g_xs_HT_fat_gg_1010 = new TGraph(nEBins,x,xs_HT_fat_gg_1010);
  g_xs_HT_fat_qZ_1010 = new TGraph(nEBins,x,xs_HT_fat_qZ_1010);

  // all sys inlcuded

  g_xs_HT_fat_qW_1010_sys = new TGraph(nEBins,x,xs_HT_fat_qW_1010_sys);
  //g_xs_HT_fat_gg_1010_sys = new TGraph(nEBins,x,xs_HT_fat_gg_1010_sys);
  g_xs_HT_fat_qZ_1010_sys = new TGraph(nEBins,x,xs_HT_fat_qZ_1010_sys);


  // fractional differnce only

  g_xs_comparison_bw_stat_bg_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_fat);
  g_xs_comparison_bw_stat_lumi_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_lumi_fat);
  g_xs_comparison_bw_stat_JES_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_JES_fat);
  g_xs_comparison_bw_stat_JER_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_JER_fat);
  g_xs_comparison_bw_stat_sys_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_sys_fat);

  // theory value

  g_xs_qstar_qW = new TGraph(en,mjj,qstar_qW);
  g_xs_qstar_qZ = new TGraph(en,mjj,qstar_qZ);
  //g_xs_diquark = new TGraph(en,mjj,diquark);
  //g_xs_string = new TGraph(en,mjj,string);
  //g_xs_zprime = new TGraph(9,mjj,zprime);
  //g_xs_wprime = new TGraph(11,mjj,wprime);
  //g_xs_rsgraviton = new TGraph(8,mjj,rsgraviton);

  // comparison b/w different Jet combinations.

  g_xs_HT_fat_qW_compare = new TGraph(nEBins,x,xs_HT_fat_qW_compare);
  //g_xs_HT_fat_gg_compare = new TGraph(nEBins,x,xs_HT_fat_gg_compare);
  g_xs_HT_fat_qZ_compare = new TGraph(nEBins,x,xs_HT_fat_qZ_compare);
  
  // comparison b/w sys.included cross section value and theory

   g_xs_qstar_qW_over_qW_fat = new TGraph(31,x,xs_qstar_qW_over_qW_fat);
   //g_xs_string_over_qW_fat = new TGraph(nEBins,x,xs_string_over_qW_fat);
   //g_xs_diquark_over_qZ_fat = new TGraph(31,x,xs_diquark_over_qZ_fat);
   g_xs_qstar_qZ_over_qZ_fat = new TGraph(31,x,xs_qstar_qZ_over_qZ_fat);
   //g_xs_zprime_over_qZ_fat = new TGraph(nEBins,x,xs_zprime_over_qZ_fat);
   //g_xs_wprime_over_qZ_fat = new TGraph(nEBins,x,xs_wprime_over_qZ_fat);
   //g_xs_rsg_over_gg_fat = new TGraph(5,x,xs_rsg_over_gg_fat);


  // expected limit for qW
  g_xs_HT_fat_qW_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_qW_1010_sys_E_m);

  // expected limit for qZ
  g_xs_HT_fat_qZ_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_qZ_1010_sys_E_m);

  // RSG try

  //double x_RSG[nEBins]= {600., 700., 800., 900., 1000.0, 1100.0, 1200.0, 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.};
  //g_xs_RSG_for_limit_fat = new TGraph(nEBins,x_RSG,xs_RSG_for_limit_fat);

//////////////////////////////////////////////////////
////////////////// TGraph design /////////////////////
//////////////////////////////////////////////////////

// stat only

  g_xs_HT_fat_qW_1010->SetLineColor(ci_g);
  g_xs_HT_fat_qW_1010->SetLineStyle(6);
  g_xs_HT_fat_qW_1010->SetLineWidth(3);
  g_xs_HT_fat_qW_1010->SetMarkerStyle(24);
  g_xs_HT_fat_qW_1010->SetMarkerColor(ci_g);

  /*g_xs_HT_fat_gg_1010->SetLineColor(ci_g);
  g_xs_HT_fat_gg_1010->SetLineStyle(6);
  g_xs_HT_fat_gg_1010->SetLineWidth(3);
  g_xs_HT_fat_gg_1010->SetMarkerStyle(24);
  g_xs_HT_fat_gg_1010->SetMarkerColor(ci_g);
  */
  g_xs_HT_fat_qZ_1010->SetLineColor(ci_g);
  g_xs_HT_fat_qZ_1010->SetLineStyle(6);
  g_xs_HT_fat_qZ_1010->SetLineWidth(3);
  g_xs_HT_fat_qZ_1010->SetMarkerStyle(24);
  g_xs_HT_fat_qZ_1010->SetMarkerColor(ci_g);

  // HT-fat all sys

  g_xs_HT_fat_qW_1010_sys->SetLineColor(2);
  g_xs_HT_fat_qW_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_qW_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_qW_1010_sys->SetMarkerStyle(20);
  g_xs_HT_fat_qW_1010_sys->SetMarkerColor(2);

  /*g_xs_HT_fat_gg_1010_sys->SetLineColor(ci_g);
  g_xs_HT_fat_gg_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_gg_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_gg_1010_sys->SetMarkerStyle(24);
  g_xs_HT_fat_gg_1010_sys->SetMarkerColor(ci_g);
  */
  g_xs_HT_fat_qZ_1010_sys->SetLineColor(4);
  g_xs_HT_fat_qZ_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_qZ_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_qZ_1010_sys->SetMarkerStyle(25);
  g_xs_HT_fat_qZ_1010_sys->SetMarkerColor(4);


  // Expected HT-qW stat only

  g_xs_HT_fat_qW_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_qW_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_qW_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_qW_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_qW_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_qW_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_qW_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_qW_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_qW_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_qW_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_qW_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_qW_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_qW_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_qW_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_qW_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_qW_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_qW_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_qW_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_qW_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_qW_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_qW_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_qW_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_qW_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_qW_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_qW_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_qW_1010_sys_E_u2->SetFillStyle(21);

  // Expected HT-qZ stat only

  g_xs_HT_fat_qZ_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_qZ_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_qZ_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_qZ_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_qZ_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_qZ_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_qZ_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_qZ_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_qZ_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_qZ_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_qZ_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_qZ_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_qZ_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_qZ_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_qZ_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_qZ_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_qZ_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_qZ_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_qZ_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_qZ_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_qZ_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_qZ_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_qZ_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_qZ_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_qZ_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_qZ_1010_sys_E_u2->SetFillStyle(21);


  // Fractional difference b/w sys and stat.(fat)

  g_xs_comparison_bw_stat_bg_fat->SetLineColor(2);
  g_xs_comparison_bw_stat_bg_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_bg_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg_fat->SetMarkerColor(2);

  g_xs_comparison_bw_stat_lumi_fat->SetLineColor(ci_g);
  g_xs_comparison_bw_stat_lumi_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_lumi_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_lumi_fat->SetMarkerStyle(25);
  g_xs_comparison_bw_stat_lumi_fat->SetMarkerColor(ci_g);

  g_xs_comparison_bw_stat_JES_fat->SetLineColor(4);
  g_xs_comparison_bw_stat_JES_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_JES_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_JES_fat->SetMarkerStyle(26);
  g_xs_comparison_bw_stat_JES_fat->SetMarkerColor(4);

  g_xs_comparison_bw_stat_JER_fat->SetLineColor(46);
  g_xs_comparison_bw_stat_JER_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_JER_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_JER_fat->SetMarkerStyle(32);
  g_xs_comparison_bw_stat_JER_fat->SetMarkerColor(46);

  g_xs_comparison_bw_stat_sys_fat->SetLineColor(1);
  g_xs_comparison_bw_stat_sys_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_sys_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_sys_fat->SetMarkerStyle(20);
  g_xs_comparison_bw_stat_sys_fat->SetMarkerColor(1);

  // Fractional difference b/w differnt jet type and dataset.

  g_xs_HT_fat_qW_compare->SetLineColor(ci_g);
  g_xs_HT_fat_qW_compare->SetLineStyle(6);
  g_xs_HT_fat_qW_compare->SetLineWidth(3);
  g_xs_HT_fat_qW_compare->SetMarkerStyle(24);
  g_xs_HT_fat_qW_compare->SetMarkerColor(ci_g);

  /*g_xs_HT_fat_gg_compare->SetLineColor(ci_g);
  g_xs_HT_fat_gg_compare->SetLineStyle(6);
  g_xs_HT_fat_gg_compare->SetLineWidth(3);
  g_xs_HT_fat_gg_compare->SetMarkerStyle(24);
  g_xs_HT_fat_gg_compare->SetMarkerColor(ci_g);
  */
  g_xs_HT_fat_qZ_compare->SetLineColor(ci_g);
  g_xs_HT_fat_qZ_compare->SetLineStyle(6);
  g_xs_HT_fat_qZ_compare->SetLineWidth(3);
  g_xs_HT_fat_qZ_compare->SetMarkerStyle(24);
  g_xs_HT_fat_qZ_compare->SetMarkerColor(ci_g);

  //g_xs_RSG_for_limit_fat->SetLineWidth(3);
  //g_xs_RSG_for_limit_fat->SetMarkerStyle(24);

  /// Theory values

  //g_xs_string->SetLineColor(9);
  //g_xs_string->SetLineStyle(8);
  //g_xs_string->SetLineWidth(3);

  g_xs_qstar_qW->SetLineColor(1);
  g_xs_qstar_qW->SetLineStyle(2);
  g_xs_qstar_qW->SetLineWidth(3);

  g_xs_qstar_qZ->SetLineColor(7);
  g_xs_qstar_qZ->SetLineStyle(3);
  g_xs_qstar_qZ->SetLineWidth(3);
  
  //g_xs_diquark->SetLineColor(8);
  //g_xs_diquark->SetLineStyle(9);
  //g_xs_diquark->SetLineWidth(3);
  
  //g_xs_wprime->SetLineColor(46);
  //g_xs_wprime->SetLineStyle(7);
  //g_xs_wprime->SetLineWidth(3);

  //g_xs_zprime->SetLineColor(38);
  //g_xs_zprime->SetLineStyle(5);
  //g_xs_zprime->SetLineWidth(3);

  //g_xs_rsgraviton->SetLineColor(29);
  //g_xs_rsgraviton->SetLineStyle(10);
  //g_xs_rsgraviton->SetLineWidth(3);

  /// comparison b/w sys.included 95% CL cross section and theory values

  //g_xs_string_over_qW_fat->SetLineColor(9);
  //g_xs_string_over_qW_fat->SetLineWidth(3);

  g_xs_qstar_qW_over_qW_fat->SetLineColor(1);
  g_xs_qstar_qW_over_qW_fat->SetLineWidth(3);

  g_xs_qstar_qZ_over_qZ_fat->SetLineColor(7);
  g_xs_qstar_qZ_over_qZ_fat->SetLineWidth(3);
  
  //g_xs_diquark_over_qZ_fat->SetLineColor(8);
  //g_xs_diquark_over_qZ_fat->SetLineWidth(3);
  
  //g_xs_wprime_over_qZ_fat->SetLineColor(46);
  //g_xs_wprime_over_qZ_fat->SetLineWidth(3);

  //g_xs_zprime_over_qZ_fat->SetLineColor(38);
  //g_xs_zprime_over_qZ_fat->SetLineWidth(3);

  //g_xs_rsg_over_gg_fat->SetLineColor(29);
  //g_xs_rsg_over_gg_fat->SetLineWidth(3);


///////////////// drawing canvas /////////////////////
//////////////////////////////////////////////////////


  // c_xs_qW

  c_xs_qW->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
   // vFrame->SetLogy();

  g_xs_HT_fat_qW_1010_sys->DrawClone("PL");
  //  g_xs_HT_pf_qW_1010_sys->DrawClone("samePL");
  //  g_xs_HT_calo_qW_1010_sys->DrawClone("samePL");

  //g_xs_string->Draw("sameL");  
  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qW->SetFillColor(0);
  l_xs_qW->SetFillStyle(0);
  l_xs_qW->SetHeader("qW-(Sys)-4.7 fb^{-1}");
  //  l_xs_qW->AddEntry(g_xs_HT_calo_qW_1010,"HT-calo 95% CL","pL");
  //  l_xs_qW->AddEntry(g_xs_HT_pf_qW_1010,"HT-PF 95% CL","pL");
  l_xs_qW->AddEntry(g_xs_HT_fat_qW_1010,"HT-fat 95% CL","pL");

  l_xs_theory->SetFillColor(0);
  l_xs_theory->SetFillStyle(0);
  //l_xs_theory->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory->AddEntry(g_xs_qstar_qW,"Excited Quark -> qW ","L");
  l_xs_theory->AddEntry(g_xs_qstar_qZ,"Excited Quark -> qZ","L");
  //l_xs_theory->AddEntry(g_xs_qstar_qZ,"(NLO) arXiv:1111.7261v2","");
  //l_xs_theory->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  //l_xs_theory->AddEntry(g_xs_wprime,"W'","L");
  //l_xs_theory->AddEntry(g_xs_zprime,"Z'","L");
  //l_xs_theory->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_qW->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qW->Update();
  c_xs_qW->SaveAs("Fit_Results/Exclusion_qW_1Vtag.png");
  c_xs_qW->SaveAs("Fit_Results/Exclusion_qW_1Vtag.pdf");

  // c_xs_gg

  /*c_xs_gg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  g_xs_HT_fat_gg_1010_sys->DrawClone("PL");
  //  g_xs_HT_pf_gg_1010->Draw("samePL");
  //  g_xs_HT_calo_gg_1010->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_gg->SetFillColor(0);
  l_xs_gg->SetFillStyle(0);
  l_xs_gg->SetHeader("gg-(Sys)-4.7 fb^{-1}");
  //  l_xs_gg->AddEntry(g_xs_HT_calo_gg_1010,"HT-calo 95% CL","pL");
  //  l_xs_gg->AddEntry(g_xs_HT_pf_gg_1010,"HT-PF 95% CL","pL");
  l_xs_gg->AddEntry(g_xs_HT_fat_gg_1010,"HT-fat 95% CL","pL");

  l_xs_gg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_gg->Update();
  c_xs_gg->SaveAs("Fit_Results/Exclusion_gg_1Vtag.png");
  c_xs_gg->SaveAs("Fit_Results/Exclusion_gg_1Vtag.pdf");
  */
  // c_xs_qZ
  
  c_xs_qZ->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  //  g_xs_HT_calo_qZ_1010->Draw("PL");
  //  g_xs_HT_pf_qZ_1010->Draw("samePL");
  g_xs_HT_fat_qZ_1010_sys->DrawClone("PL");

  //g_xs_string->Draw("sameL");  
  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qZ->SetFillColor(0);
  l_xs_qZ->SetFillStyle(0);
  l_xs_qZ->SetHeader("qZ-(Sys)-4.7 fb^{-1}");
  //  l_xs_qZ->AddEntry(g_xs_HT_calo_qZ_1010,"HT-calo 95% CL","pL");
  //  l_xs_qZ->AddEntry(g_xs_HT_pf_qZ_1010,"HT-PF 95% CL","pL");
  l_xs_qZ->AddEntry(g_xs_HT_fat_qZ_1010,"HT-fat 95% CL","pL");
  l_xs_qZ->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qZ->Update();
  c_xs_qZ->SaveAs("Fit_Results/Exclusion_qZ_1Vtag.png");
  c_xs_qZ->SaveAs("Fit_Results/Exclusion_qZ_1Vtag.pdf");

  // c_xs_stat_fat

  c_xs_stat_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  //g_xs_HT_fat_gg_1010->DrawClone("PL");
  g_xs_HT_fat_qZ_1010->DrawClone("PL");
  g_xs_HT_fat_qW_1010->DrawClone("samePL");

  //g_xs_string->Draw("sameL");  
  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_stat_fat->SetFillColor(0);
  l_xs_stat_fat->SetFillStyle(0);
  //  l_xs_stat_fat->SetHeader("CMS Preliminary");
  //  l_xs_stat_fat->AddEntry((TObject*)0,"4.7 fb^{-1}","");
  l_xs_stat_fat->SetHeader("95% CL Upper Limit (stat only)");
  //l_xs_stat_fat->AddEntry(g_xs_HT_fat_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qW_1010,"Quark-W","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qZ_1010,"Quark-Z","pL");
  l_xs_stat_fat->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_fat->AddText("CMS (4.7 fb^{-1})"); 
  p_xs_stat_fat->AddText(" #sqrt{s} = 7 TeV");
  p_xs_stat_fat->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  p_xs_stat_fat->SetFillColor(0);
  p_xs_stat_fat->SetLineColor(0);
  p_xs_stat_fat->SetFillStyle(0);
  p_xs_stat_fat->SetBorderSize(0);
  p_xs_stat_fat->SetTextFont(42);
  p_xs_stat_fat->SetTextSize(0.03);
  p_xs_stat_fat->SetTextAlign(12);
  p_xs_stat_fat->Draw("sames");
  c_xs_stat_fat->Update();
 
  c_xs_stat_fat->SaveAs("Fit_Results/Exclusion_StatOnly_1Vtag.png");
  c_xs_stat_fat->SaveAs("Fit_Results/Exclusion_StatOnly_1Vtag.pdf");

  // c_xs_sys

  c_xs_sys->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  //g_xs_HT_fat_gg_1010_sys->Draw("PL");
  g_xs_HT_fat_qZ_1010_sys->Draw("PL");
  g_xs_HT_fat_qW_1010_sys->Draw("samePL");

  //g_xs_string->Draw("sameL");  
  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_sys->SetFillColor(0);
  l_xs_sys->SetFillStyle(0);
  l_xs_sys->SetTextSize(0.03484321);
  //  l_xs_sys->SetHeader("CMS Preliminary");
  //  l_xs_sys->AddEntry((TObject*)0,"4.7 fb^{-1}","");
  l_xs_sys->SetHeader("95% CL Upper Limit (sys)");
  //l_xs_sys->AddEntry(g_xs_HT_fat_gg_1010_sys,"Gluon-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qW_1010_sys,"Quark-W","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qZ_1010_sys,"Quark-Z","pL");
  l_xs_sys->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_sys->AddText("CMS (4.7 fb^{-1})"); 
  p_xs_sys->AddText(" #sqrt{s} = 7 TeV");
  p_xs_sys->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  p_xs_sys->SetFillColor(0);
  p_xs_sys->SetLineColor(0);
  p_xs_sys->SetFillStyle(0);
  p_xs_sys->SetBorderSize(0);
  p_xs_sys->SetTextFont(62);
  p_xs_sys->SetTextSize(0.03484321);
  p_xs_sys->SetTextAlign(12);
  p_xs_sys->Draw("sames");
  c_xs_sys->Update();

  c_xs_sys->SaveAs("Fit_Results/Exclusion_Sys_1Vtag.png");
  c_xs_sys->SaveAs("Fit_Results/Exclusion_Sys_1Vtag.pdf");

  // c_xs_all_fat

  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  //g_xs_HT_fat_gg_1010->Draw("PL");
  g_xs_HT_fat_qZ_1010->Draw("PL");
  g_xs_HT_fat_qW_1010->Draw("samePL");
  //g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qZ_1010_sys->Draw("samePL");
  g_xs_HT_fat_qW_1010_sys->Draw("samePL");
  /*
  g_xs_string->Draw("PL");  
  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
  */
  l_xs_all_fat->SetFillColor(0);
  l_xs_all_fat->SetFillStyle(0);
  l_xs_all_fat->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  //l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010,"gg 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qW_1010,"qW 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qZ_1010,"qZ 95% CL (stat.only)","pL");
  //l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010_sys,"gg 95% CL (sys.included)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qW_1010_sys,"qW 95% CL (sys.included)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qZ_1010_sys,"qZ 95% CL (sys.included)","pL");
  l_xs_all_fat->Draw("sames");
  /*
  l_xs_theory->Draw("sames");
  */
  c_xs_all_fat->Update();
  c_xs_all_fat->SaveAs("Fit_Results/Exclusion_Stat_vs_Sys_1Vtag.png");
  c_xs_all_fat->SaveAs("Fit_Results/Exclusion_Stat_vs_Sys_1Vtag.pdf");

  // c_xs_comparison_bw_sys_theory_fat

  c_xs_comparison_bw_sys_theory_fat->cd(0);

  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,10.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(theory #sigma) / (sys.included 95% CL #sigma)");
  //  vFrame->SetMaximum(10.0);
  // vFrame->SetLogy();

  //g_xs_string_over_qW_fat->Draw("L");  
  g_xs_qstar_qW_over_qW_fat->Draw("sameL");
  g_xs_qstar_qZ_over_qZ_fat->Draw("sameL");
  //g_xs_diquark_over_qZ_fat->Draw("sameL");
  //g_xs_wprime_over_qZ_fat->Draw("sameL");  
  //g_xs_zprime_over_qZ_fat->Draw("sameL");  
  //g_xs_rsg_over_gg_fat->Draw("sameL");  

  TGraph *g_theory_over_sys = new TGraph(0);

  g_theory_over_sys->SetFillColor(1);
  g_theory_over_sys->SetLineColor(1);
  g_theory_over_sys->SetLineStyle(3);
  g_theory_over_sys->SetLineWidth(3);
  g_theory_over_sys->SetMarkerStyle(20);
  g_theory_over_sys->SetPoint(0,minMass,1.0);
  g_theory_over_sys->SetPoint(1,maxMass,1.0);
  g_theory_over_sys->Draw("samesl");

  l_xs_comparison_bw_sys_theory_fat->SetFillColor(0);
  l_xs_comparison_bw_sys_theory_fat->SetFillStyle(0);
  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_fat->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_string_over_qW_fat,"String Resonance","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_qstar_qW_over_qW_fat,"Excited Quark -> qW","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_qstar_qZ_over_qZ_fat,"Excited Quark -> qZ","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_qstar_qZ_over_qZ_fat,"(NLO) arXiv:1111.7261v2","");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_diquark_over_qZ_fat,"E_{6} Diquark","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_wprime_over_qZ_fat,"W'","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_zprime_over_qZ_fat,"Z'","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_rsg_over_gg_fat,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_fat->Draw("sames");
  c_xs_comparison_bw_sys_theory_fat->Update();

  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results/Exclusions_vs_Theory_1Vtag.png");
  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results/Exclusions_vs_Theory_1Vtag.pdf");
  // c_xs_qW_sys_E

  THStack *h_xs_qW_sys_E = new THStack("h_xs_qW_sys_E","h_xs_qW_sys_E");
  TH1F *htemp_xs_qW_sys_E_u2 = (TH1F*)h_xs_HT_fat_qW_1010_sys_E_u2->Clone("htemp_xs_qW_sys_E_u2");
  TH1F *htemp_xs_qW_sys_E_u1 = (TH1F*)h_xs_HT_fat_qW_1010_sys_E_u1->Clone("htemp_xs_qW_sys_E_u1");  
  TH1F *htemp_xs_qW_sys_E_d1 = (TH1F*)h_xs_HT_fat_qW_1010_sys_E_d1->Clone("htemp_xs_qW_sys_E_d1");   
  htemp_xs_qW_sys_E_u2->Add(h_xs_HT_fat_qW_1010_sys_E_u1,-1);
  htemp_xs_qW_sys_E_u1->Add(h_xs_HT_fat_qW_1010_sys_E_d1,-1);
  htemp_xs_qW_sys_E_d1->Add(h_xs_HT_fat_qW_1010_sys_E_d2,-1);

  h_xs_qW_sys_E->Add(h_xs_HT_fat_qW_1010_sys_E_d2);
  h_xs_qW_sys_E->Add(htemp_xs_qW_sys_E_d1);
  h_xs_qW_sys_E->Add(htemp_xs_qW_sys_E_u1);
  h_xs_qW_sys_E->Add(htemp_xs_qW_sys_E_u2);
  h_xs_qW_sys_E->SetMinimum(minXsec);
  h_xs_qW_sys_E->SetMaximum(maxXsec);

  c_xs_qW_sys_E->cd(0);

  h_xs_HT_fat_qW_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qW_sys_E->SetTitle("");
  h_xs_HT_fat_qW_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qW_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qW_sys_E->Draw("C");

  TLine *line1 = new TLine(1000,0.805,1000,0.102);
  line1->SetLineColor(0);
  line1->SetLineWidth(1);
  line1->Draw("sames");

  TLine *line2 = new TLine(1000,0.098,1000,0.0102);
  line2->SetLineColor(0);
  line2->SetLineWidth(1);
  line2->Draw("sames");

  TLine *line3 = new TLine(1000,0.0098,1000,0.0016);
  line3->SetLineColor(0);
  line3->SetLineWidth(1);
  line3->Draw("sames");

  TLine *line4 = new TLine(4100,0.005,4100,0.0014);
  line4->SetLineColor(0);
  line4->SetLineWidth(1);
  line4->Draw("sames");

  h_xs_qW_sys_E->GetXaxis()->SetTitle("qW Resonance Mass (GeV)");
  h_xs_qW_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  //  h_xs_HT_fat_qW_1010_sys_E_u1->Draw("sameC");
  //  h_xs_HT_fat_qW_1010_sys_E_d1->Draw("sameC");
  //  h_xs_HT_fat_qW_1010_sys_E_d2->Draw("sameC");  
  g_xs_HT_fat_qW_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_qW_1010_sys->Draw("samePL");

  //g_xs_string->Draw("samePL");  
  g_xs_qstar_qW->Draw("samePL");
  //  g_xs_qstar_qZ->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qW_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qW_sys_E->Draw();

  p_xs_qW_sys_E->SetFillColor(0);
  p_xs_qW_sys_E->SetFillStyle(0);
  p_xs_qW_sys_E->SetBorderSize(0);
  p_xs_qW_sys_E->AddText("CMS (4.7 fb^{-1})");
  p_xs_qW_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qW_sys_E->Draw("sames");
	
  l_xs_qW_sys_E->SetFillColor(0);
  l_xs_qW_sys_E->SetFillStyle(0);
//  l_xs_qW_sys_E->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_qW_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qW_sys_E->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  l_xs_qW_sys_E->AddEntry(g_xs_HT_fat_qW_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qW_sys_E->AddEntry(h_xs_HT_fat_qW_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qW_sys_E->AddEntry(h_xs_HT_fat_qW_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qW_sys_E->AddEntry(g_xs_HT_fat_qW_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qW_sys_E->AddEntry(h_xs_HT_fat_qW_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qW_sys_E->AddEntry(h_xs_HT_fat_qW_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qW_sys_E->Draw("sames");
  l_xs_theory_qW->SetFillColor(0);
  l_xs_theory_qW->SetFillStyle(0);
  //l_xs_theory_qW->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory_qW->AddEntry(g_xs_qstar_qW,"Excited Quark -> qW","L");
  l_xs_theory_qW->Draw("sames");

  c_xs_qW_sys_E->Update();

  c_xs_qW_sys_E->SaveAs("Fit_Results/Exclusions_qW_withExpected_1Vtag.png");
  c_xs_qW_sys_E->SaveAs("Fit_Results/Exclusions_qW_withExpected_1Vtag.pdf");



  // c_xs_qZ_sys_E

  THStack *h_xs_qZ_sys_E = new THStack("h_xs_qZ_sys_E","h_xs_qZ_sys_E");
  TH1F *htemp_xs_qZ_sys_E_u2 = (TH1F*)h_xs_HT_fat_qZ_1010_sys_E_u2->Clone("htemp_xs_qZ_sys_E_u2");
  TH1F *htemp_xs_qZ_sys_E_u1 = (TH1F*)h_xs_HT_fat_qZ_1010_sys_E_u1->Clone("htemp_xs_qZ_sys_E_u1");  
  TH1F *htemp_xs_qZ_sys_E_d1 = (TH1F*)h_xs_HT_fat_qZ_1010_sys_E_d1->Clone("htemp_xs_qZ_sys_E_d1");   
  htemp_xs_qZ_sys_E_u2->Add(h_xs_HT_fat_qZ_1010_sys_E_u1,-1);
  htemp_xs_qZ_sys_E_u1->Add(h_xs_HT_fat_qZ_1010_sys_E_d1,-1);
  htemp_xs_qZ_sys_E_d1->Add(h_xs_HT_fat_qZ_1010_sys_E_d2,-1);

  h_xs_qZ_sys_E->Add(h_xs_HT_fat_qZ_1010_sys_E_d2);
  h_xs_qZ_sys_E->Add(htemp_xs_qZ_sys_E_d1);
  h_xs_qZ_sys_E->Add(htemp_xs_qZ_sys_E_u1);
  h_xs_qZ_sys_E->Add(htemp_xs_qZ_sys_E_u2);

  h_xs_qZ_sys_E->SetMinimum(minXsec);
  h_xs_qZ_sys_E->SetMaximum(maxXsec);

  c_xs_qZ_sys_E->cd(0);

  h_xs_HT_fat_qZ_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qZ_sys_E->SetTitle("");
  h_xs_HT_fat_qZ_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qZ_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qZ_sys_E->Draw("C");
  
  h_xs_qZ_sys_E->GetXaxis()->SetTitle("qZ Resonance Mass (GeV)");
  h_xs_qZ_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");
 
  g_xs_HT_fat_qZ_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_qZ_1010_sys->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_qstar_qW->Draw("sameL");
  g_xs_qstar_qZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qZ_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qZ_sys_E->Draw();
	
  p_xs_qZ_sys_E->SetFillColor(0);
  p_xs_qZ_sys_E->SetFillStyle(0);
  p_xs_qZ_sys_E->SetBorderSize(0);
  p_xs_qZ_sys_E->AddText("CMS (4.7 fb^{-1})");
  p_xs_qZ_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qZ_sys_E->Draw("sames");

  l_xs_qZ_sys_E->SetFillColor(0);
  l_xs_qZ_sys_E->SetFillStyle(0);
  //  l_xs_qZ_sys_E->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_qZ_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qZ_sys_E->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  l_xs_qZ_sys_E->AddEntry(g_xs_HT_fat_qZ_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qZ_sys_E->AddEntry(h_xs_HT_fat_qZ_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qZ_sys_E->AddEntry(h_xs_HT_fat_qZ_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qZ_sys_E->AddEntry(g_xs_HT_fat_qZ_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qZ_sys_E->AddEntry(h_xs_HT_fat_qZ_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qZ_sys_E->AddEntry(h_xs_HT_fat_qZ_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qZ_sys_E->Draw("sames");
  l_xs_theory_qZ->SetFillColor(0);
  l_xs_theory_qZ->SetFillStyle(0);
  l_xs_theory_qZ->AddEntry(g_xs_qstar_qZ,"Excited Quark -> qZ","L");
  //l_xs_theory_qZ->AddEntry(g_xs_qstar_qZ,"(NLO) arXiv:1111.7261v2","");
  //l_xs_theory_qZ->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  //l_xs_theory_qZ->AddEntry(g_xs_wprime,"W'","L");
  //l_xs_theory_qZ->AddEntry(g_xs_zprime,"Z'","L");
  //l_xs_theory_qZ->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_qZ->Draw("sames");
  
  c_xs_qZ_sys_E->Update();

  c_xs_qZ_sys_E->SaveAs("Fit_Results/Exclusions_qZ_withExpected_1Vtag.png");
  c_xs_qZ_sys_E->SaveAs("Fit_Results/Exclusions_qZ_withExpected_1Vtag.pdf");

  // c_xs_comparison_bw_stat_sys_fat

  c_xs_comparison_bw_stat_sys_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(sys.included 95% CL #sigma) / (stat.only 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.8);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_comparison_bw_stat_bg_fat->Draw("PL");;
  g_xs_comparison_bw_stat_lumi_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JES_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JER_fat->Draw("samePL");
  g_xs_comparison_bw_stat_sys_fat->Draw("samePL");
	
  l_xs_comparison_bw_stat_sys_fat->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat->SetFillStyle(0);
  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();

  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results/Exclusions_qW_SysErrorsDependance_1Vtag.png");
  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results/Exclusions_qW_SysErrorsDependance_1Vtag.pdf");

  // c_xs_qW_compare
  /*
  c_xs_qW_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qW_compare->Draw("PL");

  l_xs_qW_compare->SetFillColor(0);
  l_xs_qW_compare->SetFillStyle(0);
  l_xs_qW_compare->SetHeader("qW-(Stat.Only)-4.7 fb^{-1}");
  l_xs_qW_compare->AddEntry(g_xs_HT_fat_qW_compare,"HT-fat","pL");

  l_xs_qW_compare->Draw("sames");

  c_xs_qW_compare->Update();
  */
  // c_xs_gg_compare

  /*c_xs_gg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_gg_compare->Draw("PL");

  l_xs_gg_compare->SetFillColor(0);
  l_xs_gg_compare->SetFillStyle(0);
  l_xs_gg_compare->SetHeader("gg-(Stat.Only)-4.7 fb^{-1}");
  l_xs_gg_compare->AddEntry(g_xs_HT_fat_gg_compare,"HT-fat","pL");

  l_xs_gg_compare->Draw("sames");

  c_xs_gg_compare->Update();
  */
  // c_xs_qZ_compare
  /*
  c_xs_qZ_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qZ_compare->Draw("PL");

  l_xs_qZ_compare->SetFillColor(0);
  l_xs_qZ_compare->SetFillStyle(0);
  l_xs_qZ_compare->SetHeader("qZ-(Stat.Only)-4.7 fb^{-1}");
  l_xs_qZ_compare->AddEntry(g_xs_HT_fat_qZ_compare,"HT-fat","pL");

  l_xs_qZ_compare->Draw("sames");

  c_xs_qZ_compare->Update();
  */
  // c_xs_rsg
  /*
  c_xs_rsg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(minXsec);
  vFrame->SetMaximum(maxXsec);
  // vFrame->SetLogy();

  g_xs_RSG_for_limit_fat->Draw("PL");
  //g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qZ_1010_sys->Draw("samePL");
  //  g_xs_string->Draw("sameL");  
  //  g_xs_qstar->Draw("sameL");
  //  g_xs_qstar_qZ->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_rsg->SetFillColor(0);
  l_xs_rsg->SetFillStyle(0);
  l_xs_rsg->Clear();
  //  l_xs_rsg->SetHeader("CMS Preliminary");
  //  l_xs_rsg->AddEntry((TObject*)0,"4.7 fb^{-1}","");
  l_xs_rsg->SetHeader("95% CL Upper Limit");
  l_xs_rsg->AddEntry(g_xs_HT_fat_qZ_1010_sys,"HT-fat qZ","pL");
  //l_xs_rsg->AddEntry(g_xs_HT_fat_gg_1010_sys,"HT-fat gg","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_fat,"HT-fat w1*qZ+w2*gg","pL");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");

  l_xs_rsg->Draw("sames");
  //  l_xs_theory->Draw("sames");

  c_xs_rsg->Update();
  c_xs_rsg->SaveAs("Fit_Results/Exclusions_ggqZ_Graviton_1Vtag.png");
  c_xs_rsg->SaveAs("Fit_Results/Exclusions_ggqZ_Graviton_1Vtag.pdf");
  */

}
