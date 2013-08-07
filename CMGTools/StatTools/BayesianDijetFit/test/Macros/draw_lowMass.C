#include <iostream>
#include <iomanip>

void draw_lowMass() 
{
  std::cout << "begin" << std::endl;

  
  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  //  gSystem->Load("libFWCoreFWLite.so");

  TFile *outf = new TFile("xs.root","RECREATE");

  const unsigned int nEBins = 21;
  double minMass = 550.0, maxMass = 2650.0;
  double minXsec = 4e-2, maxXsec = 40.;

  //string sReco("fat30");
  string sReco("pf");
  string sRep("Fit_Results");

  // definition of color 

  Int_t ci_g, ci_b;   // for color index setting
  ci_g = TColor::GetColor("#006600");
  ci_b = TColor::GetColor("#000099");

  // definition of canvases

  TCanvas *c_xs_gg = new TCanvas("c_xs_gg","c_xs_gg",459,358,600,602);
  TCanvas *c_xs_qg = new TCanvas("c_xs_qg","c_xs_qg",459,358,600,602);
  TCanvas *c_xs_qq = new TCanvas("c_xs_qq","c_xs_qq",459,358,600,602);
  TCanvas *c_xs_stat_fat = new TCanvas("c_xs_stat_fat","c_xs_stat_fat",459,358,600,602);
  TCanvas *c_xs_sys = new TCanvas("c_xs_sys","c_xs_sys",459,358,600,602);
  TCanvas *c_xs_all_fat = new TCanvas("c_xs_all_fat","c_xs_all_fat",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_fat = new TCanvas("c_xs_comparison_bw_stat_sys_fat","c_xs_comparison_bw_stat_sys_fat",459,358,600,602);
  //  TCanvas *c_xs_gg_compare = new TCanvas("c_xs_gg_compare","c_xs_gg_compare",459,358,600,602);
  //  TCanvas *c_xs_qg_compare = new TCanvas("c_xs_qg_compare","c_xs_qg_compare",459,358,600,602);
  //  TCanvas *c_xs_qq_compare = new TCanvas("c_xs_qq_compare","c_xs_qq_compare",459,358,600,602);
  TCanvas *c_xs_comparison_bw_sys_theory_fat = new TCanvas("c_xs_comparison_bw_sys_theory_fat","c_xs_comparison_bw_sys_theory_fat",459,358,600,602);

  TCanvas *c_xs_qg_sys_E = new TCanvas("c_xs_qg_sys_E","c_xs_qg_sys_E",459,358,600,602);
  TCanvas *c_xs_qq_sys_E = new TCanvas("c_xs_qq_sys_E","c_xs_qq_sys_E",459,358,600,602);
  TCanvas *c_xs_rsg = new TCanvas("c_xs_rsg","c_xs_rsg",459,358,600,602);

  // definition of legends

  TLegend *l_xs_theory =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qg =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qq =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");

  TLegend *l_xs_gg =  new TLegend(0.1694631,0.1585366,0.50,0.40,NULL,"brNDC");
  TLegend *l_xs_qg =  new TLegend(0.1694631,0.1585366,0.50,0.40,NULL,"brNDC");
  TLegend *l_xs_qq =  new TLegend(0.1694631,0.1585366,0.50,0.40,NULL,"brNDC");
  TLegend *l_xs_stat_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.40,NULL,"brNDC");
  TLegend *l_xs_sys =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_all_fat =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_gg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qq_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_fat =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_qg_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qq_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_rsg =  new TLegend(0.5,0.6,0.8,0.9,NULL,"brNDC");

  TPaveText *p_xs_qg_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.57,"brNDC");
  TPaveText *p_xs_qq_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.57,"brNDC");

  TPaveText *p_xs_sys = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_fat = new TPaveText(0.1694631,0.4032056,0.4379195,0.5530314,"brNDC");

  // settings of canvases

  c_xs_gg->SetLogy();
  c_xs_qg->SetLogy();
  c_xs_qq->SetLogy();
  c_xs_stat_fat->SetLogy();
  c_xs_sys->SetLogy();
  c_xs_all_fat->SetLogy();
  c_xs_comparison_bw_sys_theory_fat->SetLogy();
  c_xs_qg_sys_E->SetLogy();
  c_xs_qq_sys_E->SetLogy();

  // theory values

 const unsigned int en = 21;

 double mjj [en] ={600.,  700., 800., 900., 1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600.};

  double qstar [en] = {
    0.9871E+03,  0.4657E+03, 0.2355E+03, 0.1257E+03, 0.7005E+02, 0.4039E+02, 0.2394E+02,  0.1452E+02, 0.8982E+01,  0.5645E+01,  
    0.3596E+01,  0.2317E+01, 0.1507E+01, 0.9889E+00, 0.6531E+00, 0.4338E+00, 0.2896E+00, 0.1940E+00 ,  0.1304E+00, 0.8782E-01, 0.5925E-01};
 
  double axigluon[en] = {
    0.4395E+03, 0.2215E+03, 0.1193E+03, 0.6750E+02, 0.3967E+02, 0.2400E+02, 0.1486E+02, 0.9370E+01, 0.5998E+01, 0.3887E+01, 
    0.2544E+01, 0.1678E+01, 0.1115E+01, 0.7442E+00, 0.4988E+00, 0.3354E+00, 0.2260E+00, 0.1525E+00, 0.1030E+00, 0.6949E-01, 0.4684E-01};

  double axigluon_kfactor[en] = {
    1.07, 1.08, 1.09, 1.10, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 
    1.17, 1.18, 1.20, 1.21, 1.22, 1.24, 1.25, 1.26, 1.27, 1.29, 1.30};

  for (int i = 0; i < en; i++){
    axigluon[i] *= axigluon_kfactor[i];
  }
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

  double diquark[en] = {
    0.1451E+03, 0.8646E+02, 0.5435E+02, 0.3554E+02, 0.2393E+02, 0.1648E+02, 0.1154E+02, 0.8194E+01, 0.5877E+01, 0.4249E+01,
    0.3090E+01, 0.2258E+01, 0.1656E+01, 0.1217E+01, 0.8953E+00, 0.6591E+00, 0.4852E+00, 0.3569E+00, 0.2622E+00, 0.1922E+00, 0.1406E+00};

  double string[en] = {
    19744.01, 9304.34, 4627.17, 2485.0, 1391.71, 787.875, 473.13, 290.1, 177.6, 111.91,
    72.12, 47.07, 31.05, 20.60, 13.82, 9.117, 6.244, 4.238, 2.881, 1.973, 1.367};

  double zprime[4] = {
    0.1211E+02, 0.6246E+01, 0.3427E+01, 0.1969E+01};
    //0.1172E+01};
  //, 0.2857E+00, 0.1845E+00, 0.1206E+00,
  //  0.7961E-01, 0.5295E-01, 0.3545E-01, 0.2386E-01, 0.1611E-01, 0.1092E-01, 0.7413E-02, 0.5039E-02, 0.3426E-02, 0.2329E-02, 0.1580E-02}; 

  double wprime[6] = {
    0.2125E+02, 0.1120E+02, 0.6263E+01, 0.3661E+01, 0.2212E+01, 0.1372E+01};
    //0.8673E+00., 0.5568E+00, 0.3616E+00, 0.2369E+00,
    //    0.1562E+00, 0.1034E+00, 0.6872E-01, 0.4572E-01, 0.3043E-01, 0.2023E-01, 0.1342E-01, 0.8884E-02, 0.5859E-02, 0.3847E-02, 0.2513E-02};

  double rsgraviton[6] = {
    0.1862E+02, 0.8100E+01, 0.3852E+01, 0.1961E+01, 0.1053E+01, 0.5905E+00};
    //, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01,
    //0.4911E-01, 0.3145E-01, 0.2036E-01, 0.1330E-01, 0.8743E-02, 0.5781E-02, 0.3840E-02, 0.2559E-02, 0.1708E-02, 0.1142E-02, 0.7635E-03};  


 // 0.16751, 0.15361, 0.14070, 0.12884, 0.11799, 0.10813
 //   0.19590, 0.21457, 
  double gg_ratio[16] = {0.38, 0.40, 0.41, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48, 0.49, 0.50, 0.50, 0.51, 0.52, 0.52}; //{0.5561,0.5116,0.4671, 0.42, 0.38};
  double qq_ratio[16] = {0.62, 0.60, 0.59, 0.57, 0.56, 0.55, 0.54, 0.53, 0.52, 0.51, 0.50, 0.50, 0.49, 0.48, 0.48};

  double x[nEBins] = {600., 700., 800., 900., 1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100., 2200., 2300., 2400., 2500., 2600.};




  string sQQ("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_RSGraviton_HLT_ak5_QQtoQQ_"); sQQ = sQQ + "" + sReco + ".root";
  string sQG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_Qstar_HLT_ak5_"); sQG = sQG + "" + sReco + ".root";
  string sGG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_RSGraviton_HLT_ak5_GGtoGG_"); sGG = sGG + "" + sReco + ".root";


  TFile* file_efficiencies_qq = new TFile(sQQ.c_str(), "READ");
  TFile* file_efficiencies_qg = new TFile(sQG.c_str(), "READ");
  TFile* file_efficiencies_gg = new TFile(sGG.c_str(), "READ");

 TH1F* limit_efficiencies_qq = file_efficiencies_qq->Get("Efficiency;1");
 TH1F* limit_efficiencies_qg = file_efficiencies_qg->Get("Efficiency;1");
 TH1F* limit_efficiencies_gg = file_efficiencies_gg->Get("Efficiency;1");

 string sQQ("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_HLT_ak5_QQtoQQ_");  sQQ = sQQ + "" + sReco + "_0.root";
 string sQG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_HLT_ak5_");  sQG = sQG + "" + sReco + "_0.root";
 string sGG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_HLT_ak5_GGtoGG_");  sGG = sGG + "" + sReco + "_0.root";

 TFile* file_qq0 = new TFile(sQQ.c_str(), "READ");
 TFile* file_qg0 = new TFile(sQG.c_str(), "READ");
 TFile* file_gg0 = new TFile(sGG.c_str(), "READ");

 
 TH1F* limit_qq0 = file_qq0->Get("limit;1");
 TH1F* limit_qg0 = file_qg0->Get("limit;1");
 TH1F* limit_gg0 = file_gg0->Get("limit;1");


 for (int i = 1; i < limit_qq0->GetNbinsX()+1; i++){

   limit_qq0->SetBinContent(i, limit_qq0->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qg0->SetBinContent(i, limit_qg0->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_gg0->SetBinContent(i, limit_gg0->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

 }

  // fat-stat only --
 double* xs_HT_fat_qq_1010 = new double[nEBins];
 double* xs_HT_fat_qg_1010 = new double[nEBins];
 double* xs_HT_fat_gg_1010 = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qq_1010[i] = limit_qq0->GetBinContent(i+1);
   xs_HT_fat_qg_1010[i] = limit_qg0->GetBinContent(i+1);
   xs_HT_fat_gg_1010[i] = limit_gg0->GetBinContent(i+1);
 }

 file_qq0->Close();
 file_qg0->Close();
 file_gg0->Close(); 


 cout << "looking differentially for systematics" << endl;

  string sQG_1("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_HLT_ak5_");  sQG_1 = sQG_1 + "" + sReco + "_1.root";
  string sQG_2("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_HLT_ak5_");  sQG_2 = sQG_2 + "" + sReco + "_2.root";
  string sQG_3("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_HLT_ak5_");  sQG_3 = sQG_3 + "" + sReco + "_3.root";
  string sQG_4("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_HLT_ak5_");  sQG_4 = sQG_4 + "" + sReco + "_4.root";


  TFile* file_qg_bg = new TFile(sQG_1.c_str(), "READ");
 TFile* file_qg_lumi = new TFile(sQG_2.c_str(), "READ");
 TFile* file_qg_JES = new TFile(sQG_3.c_str(), "READ");
 TFile* file_qg_JER = new TFile(sQG_4.c_str(), "READ");

 
 TH1F* limit_qg_bg = file_qg_bg->Get("limit;1");
 TH1F* limit_qg_lumi = file_qg_lumi->Get("limit;1");
 TH1F* limit_qg_JES = file_qg_JES->Get("limit;1");
 TH1F* limit_qg_JER = file_qg_JER->Get("limit;1");

 
 for (int i = 1; i < limit_qg_bg->GetNbinsX()+1; i++){

   limit_qg_bg->SetBinContent(i, limit_qg_bg->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_lumi->SetBinContent(i, limit_qg_lumi->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_JES->SetBinContent(i, limit_qg_JES->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_JER->SetBinContent(i, limit_qg_JER->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
 }
 

  // fat-stat only --
 double* xs_HT_fat_qg_1010_bg = new double[nEBins];
 double* xs_HT_fat_qg_1010_lumi = new double[nEBins];
 double* xs_HT_fat_qg_1010_JES = new double[nEBins];
 double* xs_HT_fat_qg_1010_JER = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qg_1010_bg[i] = limit_qg_bg->GetBinContent(i+1);
   xs_HT_fat_qg_1010_lumi[i] = limit_qg_lumi->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JES[i] = limit_qg_JES->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JER[i] = limit_qg_JER->GetBinContent(i+1);
 }

 file_qg_bg->Close();
 file_qg_lumi->Close();
 file_qg_JES->Close();
 file_qg_JER->Close();


 cout << "looking for All systematics" << endl;
  // fat-All_Fat sys.included

 string sQQ("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_HLT_ak5_QQtoQQ_");  sQQ = sQQ + "" + sReco + "_6.root";
 string sQG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_HLT_ak5_");  sQG = sQG + "" + sReco + "_6.root";
 string sGG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_HLT_ak5_GGtoGG_");  sGG = sGG + "" + sReco + "_6.root";

 TFile* file_qq6 = new TFile(sQQ.c_str(), "READ");
 TFile* file_qg6 = new TFile(sQG.c_str(), "READ");
 TFile* file_gg6 = new TFile(sGG.c_str(), "READ");
 
 TH1F* limit_qq6 = file_qq6->Get("limit;1");
 TH1F* limit_qg6 = file_qg6->Get("limit;1");
 TH1F* limit_gg6 = file_gg6->Get("limit;1");

 for (int i = 1; i < limit_qq6->GetNbinsX()+1; i++){

   cout << "Reweighting" << endl;
   limit_qq6->SetBinContent(i, limit_qq6->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qg6->SetBinContent(i, limit_qg6->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_gg6->SetBinContent(i, limit_gg6->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

 }


  // fat-stat only --
 double* xs_HT_fat_qq_1010_sys = new double[nEBins];
 double* xs_HT_fat_qg_1010_sys = new double[nEBins];
 double* xs_HT_fat_gg_1010_sys = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qq_1010_sys[i] = limit_qq6->GetBinContent(i+1);
   xs_HT_fat_qg_1010_sys[i] = limit_qg6->GetBinContent(i+1);
   xs_HT_fat_gg_1010_sys[i] = limit_gg6->GetBinContent(i+1);
 }

 file_qq6->Close();
 file_qg6->Close();
 file_gg6->Close(); 


 string sQQ("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_RSGraviton_HLT_ak5_QQtoQQ_");  sQQ = sQQ + "" + sReco + "_6.root";
 string sQG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_Qstar_HLT_ak5_");  sQG = sQG + "" + sReco + "_6.root";
 string sGG("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_RSGraviton_HLT_ak5_GGtoGG_");  sGG = sGG + "" + sReco + "_6.root";


 TFile* file_exp_qq6 = new TFile(sQQ.c_str(), "READ");
 TFile* file_exp_qg6 = new TFile(sQG.c_str(), "READ");
 TFile* file_exp_gg6 = new TFile(sGG.c_str(), "READ");


 TH1F* limit_qq_twosigma_lowers = file_exp_qq6->Get("htwosigma_lowers;1");
 TH1F* limit_qq_onesigma_lowers = file_exp_qq6->Get("honesigma_lowers;1");
 TH1F* limit_qq_medians = file_exp_qq6->Get("hmedians;1");
 TH1F* limit_qq_onesigma_uppers = file_exp_qq6->Get("honesigma_uppers;1");
 TH1F* limit_qq_twosigma_uppers = file_exp_qq6->Get("htwosigma_uppers;1");
 

 for (int i = 1; i < limit_qq_twosigma_lowers->GetNbinsX()+1; i++){

   limit_qq_twosigma_lowers->SetBinContent(i, limit_qq_twosigma_lowers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_onesigma_lowers->SetBinContent(i, limit_qq_onesigma_lowers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_medians->SetBinContent(i, limit_qq_medians->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_onesigma_uppers->SetBinContent(i, limit_qq_onesigma_uppers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_twosigma_uppers->SetBinContent(i, limit_qq_twosigma_uppers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
 }

 
 double* xs_HT_fat_qq_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_qq_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_qq_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_qq_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_qq_1010_sys_E_u2 = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_qq_1010_sys_E_d2[i] = limit_qq_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qq_1010_sys_E_d1[i] = limit_qq_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qq_1010_sys_E_m[i] = limit_qq_medians->GetBinContent(i+1);
   xs_HT_fat_qq_1010_sys_E_u1[i] = limit_qq_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_qq_1010_sys_E_u2[i] = limit_qq_twosigma_uppers->GetBinContent(i+1);

 }


 TH1F* limit_qg_twosigma_lowers = file_exp_qg6->Get("htwosigma_lowers;1");
 TH1F* limit_qg_onesigma_lowers = file_exp_qg6->Get("honesigma_lowers;1");
 TH1F* limit_qg_medians = file_exp_qg6->Get("hmedians;1");
 TH1F* limit_qg_onesigma_uppers = file_exp_qg6->Get("honesigma_uppers;1");
 TH1F* limit_qg_twosigma_uppers = file_exp_qg6->Get("htwosigma_uppers;1");

 for (int i = 1; i < limit_qg_twosigma_lowers->GetNbinsX()+1; i++){

   limit_qg_twosigma_lowers->SetBinContent(i, limit_qg_twosigma_lowers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_onesigma_lowers->SetBinContent(i, limit_qg_onesigma_lowers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_medians->SetBinContent(i, limit_qg_medians->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_onesigma_uppers->SetBinContent(i, limit_qg_onesigma_uppers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_twosigma_uppers->SetBinContent(i, limit_qg_twosigma_uppers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
 }


 double* xs_HT_fat_qg_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_qg_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_qg_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_qg_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_qg_1010_sys_E_u2 = new double[nEBins];





 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_qg_1010_sys_E_d2[i] = limit_qg_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qg_1010_sys_E_d1[i] = limit_qg_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qg_1010_sys_E_m[i] = limit_qg_medians->GetBinContent(i+1);
   xs_HT_fat_qg_1010_sys_E_u1[i] = limit_qg_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_qg_1010_sys_E_u2[i] = limit_qg_twosigma_uppers->GetBinContent(i+1);

 }


 TH1F* limit_gg_twosigma_lowers = file_exp_gg6->Get("htwosigma_lowers;1");
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

 file_exp_qq6->Close();
 file_exp_qg6->Close();
 file_exp_gg6->Close();



 
 double xs_HT_pf_qg_compare[nEBins], xs_HT_pf_gg_compare[nEBins], xs_HT_pf_qq_compare[nEBins], xs_HT_fat_qg_compare[nEBins], xs_HT_fat_gg_compare[nEBins], xs_HT_fat_qq_compare[nEBins], xs_comparison_bw_stat_bg_fat[nEBins], xs_comparison_bw_stat_lumi_fat[nEBins], xs_comparison_bw_stat_JES_fat[nEBins], xs_comparison_bw_stat_JER_fat[nEBins], xs_comparison_bw_stat_sys_fat[nEBins], xs_RSG_for_limit_fat[15], xs_qstar_over_qg_fat[nEBins], xs_string_over_qg_fat[nEBins], xs_diquark_over_qq_fat[nEBins], xs_axigluon_over_qq_fat[nEBins], xs_zprime_over_qq_fat[4], xs_wprime_over_qq_fat[6], xs_rsg_over_gg_fat[4];

 std::cout << "wide stat only" << std::endl;
 for (int  i = 0 ; i < nEBins ; i++) 
   {
     std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qq_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qg_1010[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010[i] << " \\\\" << std::endl;
     
   }
 
 
 std::cout << "wide sys included" << std::endl;
 for (int  i = 0 ; i < 16 ; i++) 
   {
     std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qq_1010_sys[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qg_1010_sys[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010_sys[i] << " \\\\" << std::endl;
     
   }
 
////////////////// Histogram definition /////////////////////
/////////////////////////////////////////////////////////////


  // expected limit for qg

  TH1F *h_xs_HT_fat_qg_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qg_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qg_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qg_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

  // expected limit for qq

  TH1F *h_xs_HT_fat_qq_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qq_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qq_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_qq_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

		 

 for (int i = 0; i<nEBins; i++) {
   
   std::cout << i << " & " << std::fixed << setprecision(3) << xs_HT_fat_qq_1010_sys[i] << " & " << xs_HT_fat_qg_1010_sys[i] << " & " << xs_HT_fat_gg_1010_sys[i] << " \\\\  " << std::endl;
   if (i < 15) xs_RSG_for_limit_fat[i] = xs_HT_fat_gg_1010_sys[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys[i]*qq_ratio[i];
   if (i < 4) xs_rsg_over_gg_fat[i] = rsgraviton[i]/xs_RSG_for_limit_fat[i];
     
   xs_comparison_bw_stat_bg_fat[i] = xs_HT_fat_qg_1010_bg[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_lumi_fat[i] = xs_HT_fat_qg_1010_lumi[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JES_fat[i] = xs_HT_fat_qg_1010_JES[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JER_fat[i] = xs_HT_fat_qg_1010_JER[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_sys_fat[i] = xs_HT_fat_qg_1010_sys[i]/xs_HT_fat_qg_1010[i];

   xs_qstar_over_qg_fat[i] = qstar[i]/xs_HT_fat_qg_1010_sys[i];
   xs_string_over_qg_fat[i] = string[i]/xs_HT_fat_qg_1010_sys[i];
   xs_diquark_over_qq_fat[i] = diquark[i]/xs_HT_fat_qq_1010_sys[i];
   xs_axigluon_over_qq_fat[i] = axigluon[i]/xs_HT_fat_qq_1010_sys[i];
   if (i < 4) xs_zprime_over_qq_fat[i] = zprime[i]/xs_HT_fat_qq_1010_sys[i];
   if (i < 6) xs_wprime_over_qq_fat[i] = wprime[i]/xs_HT_fat_qq_1010_sys[i];
   
   h_xs_HT_fat_qg_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_d2[i]);
   h_xs_HT_fat_qg_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_d1[i]);
   h_xs_HT_fat_qg_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_u1[i]);
   h_xs_HT_fat_qg_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_u2[i]);

   h_xs_HT_fat_qq_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_d2[i]);
   h_xs_HT_fat_qq_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_d1[i]);
   h_xs_HT_fat_qq_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_u1[i]);
   h_xs_HT_fat_qq_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_u2[i]);

 }

//////////////////////////////////////////////////////
////////////////// TGraph define /////////////////////
//////////////////////////////////////////////////////

// stat only 

  g_xs_HT_fat_qg_1010 = new TGraph(nEBins,x,xs_HT_fat_qg_1010);
  g_xs_HT_fat_gg_1010 = new TGraph(nEBins,x,xs_HT_fat_gg_1010);
  g_xs_HT_fat_qq_1010 = new TGraph(nEBins,x,xs_HT_fat_qq_1010);

  // all sys inlcuded

  g_xs_HT_fat_qg_1010_sys = new TGraph(nEBins,x,xs_HT_fat_qg_1010_sys);
  g_xs_HT_fat_gg_1010_sys = new TGraph(nEBins,x,xs_HT_fat_gg_1010_sys);
  g_xs_HT_fat_qq_1010_sys = new TGraph(nEBins,x,xs_HT_fat_qq_1010_sys);


  // fractional differnce only

  g_xs_comparison_bw_stat_bg_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_fat);
  g_xs_comparison_bw_stat_lumi_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_lumi_fat);
  g_xs_comparison_bw_stat_JES_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_JES_fat);
  g_xs_comparison_bw_stat_JER_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_JER_fat);
  g_xs_comparison_bw_stat_sys_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_sys_fat);

  // theory value

  g_xs_qstar = new TGraph(en,mjj,qstar);
  g_xs_axigluon = new TGraph(en,mjj,axigluon);
  g_xs_diquark = new TGraph(en,mjj,diquark);
  g_xs_string = new TGraph(en,mjj,string);
  g_xs_zprime = new TGraph(4,mjj,zprime);
  g_xs_wprime = new TGraph(6,mjj,wprime);
  g_xs_rsgraviton = new TGraph(4,mjj,rsgraviton);

  // comparison b/w different Jet combinations.

  g_xs_HT_fat_qg_compare = new TGraph(nEBins,x,xs_HT_fat_qg_compare);
  g_xs_HT_fat_gg_compare = new TGraph(nEBins,x,xs_HT_fat_gg_compare);
  g_xs_HT_fat_qq_compare = new TGraph(nEBins,x,xs_HT_fat_qq_compare);
  
  // comparison b/w sys.included cross section value and theory

   g_xs_qstar_over_qg_fat = new TGraph(nEBins,x,xs_qstar_over_qg_fat);
   g_xs_string_over_qg_fat = new TGraph(nEBins,x,xs_string_over_qg_fat);
   g_xs_diquark_over_qq_fat = new TGraph(nEBins,x,xs_diquark_over_qq_fat);
   g_xs_axigluon_over_qq_fat = new TGraph(nEBins,x,xs_axigluon_over_qq_fat);
   g_xs_zprime_over_qq_fat = new TGraph(4,x,xs_zprime_over_qq_fat);
   g_xs_wprime_over_qq_fat = new TGraph(6,x,xs_wprime_over_qq_fat);
   g_xs_rsg_over_gg_fat = new TGraph(4,x,xs_rsg_over_gg_fat);


  // expected limit for qg
  g_xs_HT_fat_qg_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_qg_1010_sys_E_m);

  // expected limit for qq
  g_xs_HT_fat_qq_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_qq_1010_sys_E_m);

  // RSG try

  double x_RSG[16]= {600., 700., 800., 900., 1000.0, 1100.0, 1200.0, 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.};
  g_xs_RSG_for_limit_fat = new TGraph(15,x_RSG,xs_RSG_for_limit_fat);

//////////////////////////////////////////////////////
////////////////// TGraph design /////////////////////
//////////////////////////////////////////////////////

// stat only

  g_xs_HT_fat_qg_1010->SetLineColor(ci_g);
  g_xs_HT_fat_qg_1010->SetLineStyle(6);
  g_xs_HT_fat_qg_1010->SetLineWidth(3);
  g_xs_HT_fat_qg_1010->SetMarkerStyle(24);
  g_xs_HT_fat_qg_1010->SetMarkerColor(ci_g);

  g_xs_HT_fat_gg_1010->SetLineColor(ci_g);
  g_xs_HT_fat_gg_1010->SetLineStyle(6);
  g_xs_HT_fat_gg_1010->SetLineWidth(3);
  g_xs_HT_fat_gg_1010->SetMarkerStyle(24);
  g_xs_HT_fat_gg_1010->SetMarkerColor(ci_g);

  g_xs_HT_fat_qq_1010->SetLineColor(ci_g);
  g_xs_HT_fat_qq_1010->SetLineStyle(6);
  g_xs_HT_fat_qq_1010->SetLineWidth(3);
  g_xs_HT_fat_qq_1010->SetMarkerStyle(24);
  g_xs_HT_fat_qq_1010->SetMarkerColor(ci_g);

  // HT-fat all sys

  g_xs_HT_fat_qg_1010_sys->SetLineColor(2);
  g_xs_HT_fat_qg_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_qg_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_qg_1010_sys->SetMarkerStyle(20);
  g_xs_HT_fat_qg_1010_sys->SetMarkerColor(2);

  g_xs_HT_fat_gg_1010_sys->SetLineColor(ci_g);
  g_xs_HT_fat_gg_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_gg_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_gg_1010_sys->SetMarkerStyle(24);
  g_xs_HT_fat_gg_1010_sys->SetMarkerColor(ci_g);

  g_xs_HT_fat_qq_1010_sys->SetLineColor(4);
  g_xs_HT_fat_qq_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_qq_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_qq_1010_sys->SetMarkerStyle(25);
  g_xs_HT_fat_qq_1010_sys->SetMarkerColor(4);


  // Expected HT-qg stat only

  g_xs_HT_fat_qg_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_qg_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_qg_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_qg_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_qg_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_qg_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_qg_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_qg_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_qg_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_qg_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_qg_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_qg_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_qg_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_qg_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_qg_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_qg_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_qg_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_qg_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_qg_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_qg_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_qg_1010_sys_E_u2->SetFillStyle(21);

  // Expected HT-qq stat only

  g_xs_HT_fat_qq_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_qq_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_qq_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_qq_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_qq_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_qq_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_qq_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_qq_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_qq_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_qq_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_qq_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_qq_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_qq_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_qq_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_qq_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_qq_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_qq_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_qq_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_qq_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_qq_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_qq_1010_sys_E_u2->SetFillStyle(21);


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

  g_xs_HT_fat_qg_compare->SetLineColor(ci_g);
  g_xs_HT_fat_qg_compare->SetLineStyle(6);
  g_xs_HT_fat_qg_compare->SetLineWidth(3);
  g_xs_HT_fat_qg_compare->SetMarkerStyle(24);
  g_xs_HT_fat_qg_compare->SetMarkerColor(ci_g);

  g_xs_HT_fat_gg_compare->SetLineColor(ci_g);
  g_xs_HT_fat_gg_compare->SetLineStyle(6);
  g_xs_HT_fat_gg_compare->SetLineWidth(3);
  g_xs_HT_fat_gg_compare->SetMarkerStyle(24);
  g_xs_HT_fat_gg_compare->SetMarkerColor(ci_g);

  g_xs_HT_fat_qq_compare->SetLineColor(ci_g);
  g_xs_HT_fat_qq_compare->SetLineStyle(6);
  g_xs_HT_fat_qq_compare->SetLineWidth(3);
  g_xs_HT_fat_qq_compare->SetMarkerStyle(24);
  g_xs_HT_fat_qq_compare->SetMarkerColor(ci_g);

  g_xs_RSG_for_limit_fat->SetLineWidth(3);
  g_xs_RSG_for_limit_fat->SetMarkerStyle(24);

  /// Theory values

  g_xs_string->SetLineColor(9);
  g_xs_string->SetLineStyle(8);
  g_xs_string->SetLineWidth(3);

  g_xs_qstar->SetLineColor(1);
  g_xs_qstar->SetLineStyle(2);
  g_xs_qstar->SetLineWidth(3);

  g_xs_axigluon->SetLineColor(7);
  g_xs_axigluon->SetLineStyle(3);
  g_xs_axigluon->SetLineWidth(3);
  
  g_xs_diquark->SetLineColor(8);
  g_xs_diquark->SetLineStyle(9);
  g_xs_diquark->SetLineWidth(3);
  
  g_xs_wprime->SetLineColor(46);
  g_xs_wprime->SetLineStyle(7);
  g_xs_wprime->SetLineWidth(3);

  g_xs_zprime->SetLineColor(38);
  g_xs_zprime->SetLineStyle(5);
  g_xs_zprime->SetLineWidth(3);

  g_xs_rsgraviton->SetLineColor(29);
  g_xs_rsgraviton->SetLineStyle(10);
  g_xs_rsgraviton->SetLineWidth(3);

  /// comparison b/w sys.included 95% CL cross section and theory values

  g_xs_string_over_qg_fat->SetLineColor(9);
  g_xs_string_over_qg_fat->SetLineWidth(3);

  g_xs_qstar_over_qg_fat->SetLineColor(1);
  g_xs_qstar_over_qg_fat->SetLineWidth(3);

  g_xs_axigluon_over_qq_fat->SetLineColor(7);
  g_xs_axigluon_over_qq_fat->SetLineWidth(3);
  
  g_xs_diquark_over_qq_fat->SetLineColor(8);
  g_xs_diquark_over_qq_fat->SetLineWidth(3);
  
  g_xs_wprime_over_qq_fat->SetLineColor(46);
  g_xs_wprime_over_qq_fat->SetLineWidth(3);

  g_xs_zprime_over_qq_fat->SetLineColor(38);
  g_xs_zprime_over_qq_fat->SetLineWidth(3);

  g_xs_rsg_over_gg_fat->SetLineColor(29);
  g_xs_rsg_over_gg_fat->SetLineWidth(3);


///////////////// drawing canvas /////////////////////
//////////////////////////////////////////////////////


  // c_xs_qg

  c_xs_qg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
   // vFrame->SetLogy();

  g_xs_HT_fat_qg_1010_sys->DrawClone("PL");
  //  g_xs_HT_pf_qg_1010_sys->DrawClone("samePL");
  //  g_xs_HT_calo_qg_1010_sys->DrawClone("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qg->SetFillColor(0);
  l_xs_qg->SetFillStyle(0);
  if (sReco.find("fat30") != string::npos) l_xs_qg->SetHeader("Wide jets qg-(Sys)-0.118 fb^{-1}");
  if (sReco.find("pf") != string::npos) l_xs_qg->SetHeader("PF AK5 jets qg-(Sys)-0.118 fb^{-1}");
  //  l_xs_qg->AddEntry(g_xs_HT_calo_qg_1010,"HT-calo 95% CL","pL");
  //  l_xs_qg->AddEntry(g_xs_HT_pf_qg_1010,"HT-PF 95% CL","pL");
  l_xs_qg->AddEntry(g_xs_HT_fat_qg_1010,"HT-fat 95% CL","pL");

  l_xs_theory->SetFillColor(0);
  l_xs_theory->SetFillStyle(0);
  l_xs_theory->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory->AddEntry(g_xs_qstar,"Excited Quark ","L");
  l_xs_theory->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  l_xs_theory->AddEntry(g_xs_axigluon,"(NLO) arXiv:1111.7261v2","");
  l_xs_theory->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory->AddEntry(g_xs_zprime,"Z'","L");
  l_xs_theory->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_qg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qg->Update();
  string sOut_png = sRep + "/Exclusion_qg_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusion_qg_lowMass_"+sReco+".eps";
  c_xs_qg->SaveAs(sOut_png.c_str());
  c_xs_qg->SaveAs(sOut_eps.c_str());

  // c_xs_gg

  c_xs_gg->cd(0);
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
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_gg->SetFillColor(0);
  l_xs_gg->SetFillStyle(0);
  if (sReco.find("fat30") != string::npos) l_xs_gg->SetHeader("Wide jets gg-(Sys)-0.118 fb^{-1}");
  if (sReco.find("pf") != string::npos) l_xs_gg->SetHeader("PF AK5 jets gg-(Sys)-0.118 fb^{-1}");
  //  l_xs_gg->AddEntry(g_xs_HT_calo_gg_1010,"HT-calo 95% CL","pL");
  //  l_xs_gg->AddEntry(g_xs_HT_pf_gg_1010,"HT-PF 95% CL","pL");
  l_xs_gg->AddEntry(g_xs_HT_fat_gg_1010,"HT-fat 95% CL","pL");

  l_xs_gg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_gg->Update();


  string sOut_png = sRep + "/Exclusion_gg_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusion_gg_lowMass_"+sReco+".eps";
  c_xs_gg->SaveAs(sOut_png.c_str());
  c_xs_gg->SaveAs(sOut_eps.c_str());
  // c_xs_qq

  c_xs_qq->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  //  g_xs_HT_calo_qq_1010->Draw("PL");
  //  g_xs_HT_pf_qq_1010->Draw("samePL");
  g_xs_HT_fat_qq_1010_sys->DrawClone("PL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qq->SetFillColor(0);
  l_xs_qq->SetFillStyle(0);
  if (sReco.find("fat30") != string::npos) l_xs_qq->SetHeader("Wide jets qq-(Sys)-0.118 fb^{-1}");
  if (sReco.find("pf") != string::npos) l_xs_qq->SetHeader("PF AK5 jets qq-(Sys)-0.118 fb^{-1}");
  //  l_xs_qq->AddEntry(g_xs_HT_calo_qq_1010,"HT-calo 95% CL","pL");
  //  l_xs_qq->AddEntry(g_xs_HT_pf_qq_1010,"HT-PF 95% CL","pL");
  l_xs_qq->AddEntry(g_xs_HT_fat_qq_1010,"HT-fat 95% CL","pL");
  l_xs_qq->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qq->Update();

  string sOut_png = sRep + "/Exclusion_qq_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusion_qq_lowMass_"+sReco+".eps";
  c_xs_qq->SaveAs(sOut_png.c_str());
  c_xs_qq->SaveAs(sOut_eps.c_str());

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

  g_xs_HT_fat_gg_1010->DrawClone("PL");
  g_xs_HT_fat_qq_1010->DrawClone("samePL");
  g_xs_HT_fat_qg_1010->DrawClone("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_stat_fat->SetFillColor(0);
  l_xs_stat_fat->SetFillStyle(0);
  //  l_xs_stat_fat->SetHeader("CMS Preliminary");
  //  l_xs_stat_fat->AddEntry((TObject*)0,"0.118 fb^{-1}","");
  l_xs_stat_fat->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qg_1010,"Quark-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qq_1010,"Quark-Quark","pL");


  l_xs_qq->SetHeader("qq-(Sys)-0.118 fb^{-1}");

  l_xs_stat_fat->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_fat->AddText("CMS (0.118 fb^{-1})");
  if (sReco.find("fat30") != string::npos)
    p_xs_stat_fat->AddText("Wide jets");
  else 
    p_xs_stat_fat->AddText("PF AK5 jets");

  p_xs_stat_fat->AddText("#sqrt{s} = 7 TeV");
  p_xs_stat_fat->SetFillColor(0);
  p_xs_stat_fat->SetLineColor(0);
  p_xs_stat_fat->SetFillStyle(0);
  p_xs_stat_fat->SetBorderSize(0);
  p_xs_stat_fat->SetTextFont(42);
  p_xs_stat_fat->SetTextSize(0.03);
  p_xs_stat_fat->SetTextAlign(12);
  p_xs_stat_fat->Draw("sames");
  c_xs_stat_fat->Update();
 

  string sOut_png = sRep + "/Exclusion_StatOnly_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusion_StatOnly_lowMass_"+sReco+".eps";
  c_xs_stat_fat->SaveAs(sOut_png.c_str());
  c_xs_stat_fat->SaveAs(sOut_eps.c_str());

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

  g_xs_HT_fat_gg_1010_sys->Draw("PL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");
  g_xs_HT_fat_qg_1010_sys->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_sys->SetFillColor(0);
  l_xs_sys->SetFillStyle(0);
  l_xs_sys->SetTextSize(0.03484321);
  //  l_xs_sys->SetHeader("CMS Preliminary");
  //  l_xs_sys->AddEntry((TObject*)0,"0.118 fb^{-1}","");
  l_xs_sys->SetHeader("95% CL Upper Limit (sys)");
  l_xs_sys->AddEntry(g_xs_HT_fat_gg_1010_sys,"Gluon-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qg_1010_sys,"Quark-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qq_1010_sys,"Quark-Quark","pL");

  l_xs_sys->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_sys->AddText("CMS (0.118 fb^{-1})");
  p_xs_sys->AddText(" #sqrt{s} = 7 TeV");
  if (sReco.find("fat30") != string::npos)
    p_xs_sys->AddText("Wide jets");
  else 
    p_xs_sys->AddText("PF AK5 jets");

  p_xs_sys->SetFillColor(0);
  p_xs_sys->SetLineColor(0);
  p_xs_sys->SetFillStyle(0);
  p_xs_sys->SetBorderSize(0);
  p_xs_sys->SetTextFont(62);
  p_xs_sys->SetTextSize(0.03484321);
  p_xs_sys->SetTextAlign(12);
  p_xs_sys->Draw("sames");
  c_xs_sys->Update();

  string sOut_png = sRep + "/Exclusion_Sys_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusion_Sys_lowMass_"+sReco+".eps";
  c_xs_sys->SaveAs(sOut_png.c_str());
  c_xs_sys->SaveAs(sOut_eps.c_str());

  // c_xs_all_fat

  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  g_xs_HT_fat_gg_1010->Draw("PL");
  g_xs_HT_fat_qq_1010->Draw("samePL");
  g_xs_HT_fat_qg_1010->Draw("samePL");
  g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");
  g_xs_HT_fat_qg_1010_sys->Draw("samePL");
  /*
  g_xs_string->Draw("PL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
  */
  l_xs_all_fat->SetFillColor(0);
  l_xs_all_fat->SetFillStyle(0);
  l_xs_all_fat->SetHeader("CMS (0.118 fb^{-1})");
  if (sReco.find("fat30") != string::npos)
    l_xs_all_fat->SetHeader("CMS (0.118 fb^{-1}) Wide jets");
  else 
    l_xs_all_fat->SetHeader("CMS (0.118 fb^{-1}) PF AK5 jets");

  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"0.118 fb^{-1}","")

  l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010,"gg 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qg_1010,"qg 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qq_1010,"qq 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010_sys,"gg 95% CL (sys.included)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qg_1010_sys,"qg 95% CL (sys.included)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qq_1010_sys,"qq 95% CL (sys.included)","pL");



  l_xs_all_fat->Draw("sames");
  /*
  l_xs_theory->Draw("sames");
  */
  c_xs_all_fat->Update();

  string sOut_png = sRep + "/Exclusion_Stat_vs_Sys_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusion_Stat_vs_Sys_lowMass_"+sReco+".eps";
  c_xs_all_fat->SaveAs(sOut_png.c_str());
  c_xs_all_fat->SaveAs(sOut_eps.c_str());

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

  g_xs_string_over_qg_fat->Draw("L");  
  g_xs_qstar_over_qg_fat->Draw("sameL");
  g_xs_axigluon_over_qq_fat->Draw("sameL");
  g_xs_diquark_over_qq_fat->Draw("sameL");
  g_xs_wprime_over_qq_fat->Draw("sameL");  
  g_xs_zprime_over_qq_fat->Draw("sameL");  
  g_xs_rsg_over_gg_fat->Draw("sameL");  

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

  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS (0.118 fb^{-1}): PF AK5 jets");
  if (sReco.find("fat30") != string::npos)
    l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS (0.118 fb^{-1}): Wide jets");


  //  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_fat->AddEntry((TObject*)0,"0.118 fb^{-1}","")
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_string_over_qg_fat,"String Resonance","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_qstar_over_qg_fat,"Excited Quark","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_axigluon_over_qq_fat,"Axigluon/Coloron","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_axigluon_over_qq_fat,"(NLO) arXiv:1111.7261v2","");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_diquark_over_qq_fat,"E_{6} Diquark","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_wprime_over_qq_fat,"W'","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_zprime_over_qq_fat,"Z'","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_rsg_over_gg_fat,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_fat->Draw("sames");
  c_xs_comparison_bw_sys_theory_fat->Update();

  string sOut_png = sRep + "/Exclusions_vs_Theory_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_vs_Theory_lowMass_"+sReco+".eps";
  string sOut_root = sRep + "/Exclusions_vs_Theory_lowMass_"+sReco+".root";
  c_xs_comparison_bw_sys_theory_fat->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_sys_theory_fat->SaveAs(sOut_eps.c_str());
  c_xs_comparison_bw_sys_theory_fat->SaveAs(sOut_root.c_str());

  // c_xs_qg_sys_E

  THStack *h_xs_qg_sys_E = new THStack("h_xs_qg_sys_E","h_xs_qg_sys_E");
  TH1F *htemp_xs_qg_sys_E_u2 = (TH1F*)h_xs_HT_fat_qg_1010_sys_E_u2->Clone("htemp_xs_qg_sys_E_u2");
  TH1F *htemp_xs_qg_sys_E_u1 = (TH1F*)h_xs_HT_fat_qg_1010_sys_E_u1->Clone("htemp_xs_qg_sys_E_u1");  
  TH1F *htemp_xs_qg_sys_E_d1 = (TH1F*)h_xs_HT_fat_qg_1010_sys_E_d1->Clone("htemp_xs_qg_sys_E_d1");   
  htemp_xs_qg_sys_E_u2->Add(h_xs_HT_fat_qg_1010_sys_E_u1,-1);
  htemp_xs_qg_sys_E_u1->Add(h_xs_HT_fat_qg_1010_sys_E_d1,-1);
  htemp_xs_qg_sys_E_d1->Add(h_xs_HT_fat_qg_1010_sys_E_d2,-1);

  h_xs_qg_sys_E->Add(h_xs_HT_fat_qg_1010_sys_E_d2);
  h_xs_qg_sys_E->Add(htemp_xs_qg_sys_E_d1);
  h_xs_qg_sys_E->Add(htemp_xs_qg_sys_E_u1);
  h_xs_qg_sys_E->Add(htemp_xs_qg_sys_E_u2);
  h_xs_qg_sys_E->SetMinimum(minXsec);
  h_xs_qg_sys_E->SetMaximum(maxXsec);

  c_xs_qg_sys_E->cd(0);

  h_xs_HT_fat_qg_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qg_sys_E->SetTitle("");
  h_xs_HT_fat_qg_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qg_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qg_sys_E->Draw("C");

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

  h_xs_qg_sys_E->GetXaxis()->SetTitle("qg Resonance Mass (GeV)");
  h_xs_qg_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  //  h_xs_HT_fat_qg_1010_sys_E_u1->Draw("sameC");
  //  h_xs_HT_fat_qg_1010_sys_E_d1->Draw("sameC");
  //  h_xs_HT_fat_qg_1010_sys_E_d2->Draw("sameC");  
  g_xs_HT_fat_qg_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_qg_1010_sys->Draw("samePL");

  g_xs_string->Draw("samePL");  
  g_xs_qstar->Draw("sameL");
  //  g_xs_axigluon->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qg_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qg_sys_E->Draw();

  p_xs_qg_sys_E->SetFillColor(0);
  p_xs_qg_sys_E->SetFillStyle(0);
  p_xs_qg_sys_E->SetBorderSize(0);

  p_xs_qg_sys_E->AddText("CMS (0.118 fb^{-1})");
  p_xs_qg_sys_E->AddText("#sqrt{s} = 7 TeV");

  if (sReco.find("fat30") != string::npos)
    p_xs_qg_sys_E->AddText("Wide jets");
  else 
    p_xs_qg_sys_E->AddText("PF AK5 jets");

  p_xs_qg_sys_E->Draw("sames");
	
  l_xs_qg_sys_E->SetFillColor(0);
  l_xs_qg_sys_E->SetFillStyle(0);
//  l_xs_qg_sys_E->SetHeader("CMS (0.118 fb^{-1})");
  //  l_xs_qg_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qg_sys_E->AddEntry((TObject*)0,"0.118 fb^{-1}","")
  l_xs_qg_sys_E->AddEntry(g_xs_HT_fat_qg_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qg_sys_E->AddEntry(g_xs_HT_fat_qg_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qg_sys_E->Draw("sames");
  l_xs_theory_qg->SetFillColor(0);
  l_xs_theory_qg->SetFillStyle(0);
  l_xs_theory_qg->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory_qg->AddEntry(g_xs_qstar,"Excited Quark ","L");
  l_xs_theory_qg->Draw("sames");

  c_xs_qg_sys_E->Update();

  string sOut_png = sRep + "/Exclusions_qg_withExpected_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_qg_withExpected_lowMass_"+sReco+".eps";

  c_xs_qg_sys_E->SaveAs(sOut_png.c_str());
  c_xs_qg_sys_E->SaveAs(sOut_eps.c_str());



  // c_xs_qq_sys_E

  THStack *h_xs_qq_sys_E = new THStack("h_xs_qq_sys_E","h_xs_qq_sys_E");
  TH1F *htemp_xs_qq_sys_E_u2 = (TH1F*)h_xs_HT_fat_qq_1010_sys_E_u2->Clone("htemp_xs_qq_sys_E_u2");
  TH1F *htemp_xs_qq_sys_E_u1 = (TH1F*)h_xs_HT_fat_qq_1010_sys_E_u1->Clone("htemp_xs_qq_sys_E_u1");  
  TH1F *htemp_xs_qq_sys_E_d1 = (TH1F*)h_xs_HT_fat_qq_1010_sys_E_d1->Clone("htemp_xs_qq_sys_E_d1");   
  htemp_xs_qq_sys_E_u2->Add(h_xs_HT_fat_qq_1010_sys_E_u1,-1);
  htemp_xs_qq_sys_E_u1->Add(h_xs_HT_fat_qq_1010_sys_E_d1,-1);
  htemp_xs_qq_sys_E_d1->Add(h_xs_HT_fat_qq_1010_sys_E_d2,-1);

  h_xs_qq_sys_E->Add(h_xs_HT_fat_qq_1010_sys_E_d2);
  h_xs_qq_sys_E->Add(htemp_xs_qq_sys_E_d1);
  h_xs_qq_sys_E->Add(htemp_xs_qq_sys_E_u1);
  h_xs_qq_sys_E->Add(htemp_xs_qq_sys_E_u2);

  h_xs_qq_sys_E->SetMinimum(minXsec);
  h_xs_qq_sys_E->SetMaximum(maxXsec);

  c_xs_qq_sys_E->cd(0);

  h_xs_HT_fat_qq_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qq_sys_E->SetTitle("");
  h_xs_HT_fat_qq_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qq_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qq_sys_E->Draw("C");
  
  h_xs_qq_sys_E->GetXaxis()->SetTitle("qq Resonance Mass (GeV)");
  h_xs_qq_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");
 
  g_xs_HT_fat_qq_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qq_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qq_sys_E->Draw();
	
  p_xs_qq_sys_E->SetFillColor(0);
  p_xs_qq_sys_E->SetFillStyle(0);
  p_xs_qq_sys_E->SetBorderSize(0);


  p_xs_qq_sys_E->AddText("CMS (0.118 fb^{-1})");
  p_xs_qq_sys_E->AddText("#sqrt{s} = 7 TeV");

  if (sReco.find("fat30") != string::npos)
    p_xs_qq_sys_E->AddText("Wide jets");
  else 
    p_xs_qq_sys_E->AddText("PF AK5 jets");


  p_xs_qq_sys_E->Draw("sames");

  l_xs_qq_sys_E->SetFillColor(0);
  l_xs_qq_sys_E->SetFillStyle(0);
  //  l_xs_qq_sys_E->SetHeader("CMS (0.118 fb^{-1})");
  //  l_xs_qq_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_sys_E->AddEntry((TObject*)0,"0.118 fb^{-1}","")
  l_xs_qq_sys_E->AddEntry(g_xs_HT_fat_qq_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qq_sys_E->AddEntry(g_xs_HT_fat_qq_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qq_sys_E->Draw("sames");
  l_xs_theory_qq->SetFillColor(0);
  l_xs_theory_qq->SetFillStyle(0);
  l_xs_theory_qq->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  l_xs_theory_qq->AddEntry(g_xs_axigluon,"(NLO) arXiv:1111.7261v2","");
  l_xs_theory_qq->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory_qq->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory_qq->AddEntry(g_xs_zprime,"Z'","L");
  //  l_xs_theory_qq->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_qq->Draw("sames");
  
  c_xs_qq_sys_E->Update();

  string sOut_png = sRep + "/Exclusions_qq_withExpected_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_qq_withExpected_lowMass_"+sReco+".eps";

  c_xs_qq_sys_E->SaveAs(sOut_png.c_str());
  c_xs_qq_sys_E->SaveAs(sOut_eps.c_str());

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
  vFrame->SetMinimum(0.7);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_comparison_bw_stat_bg_fat->Draw("PL");;
  g_xs_comparison_bw_stat_lumi_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JES_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JER_fat->Draw("samePL");
  g_xs_comparison_bw_stat_sys_fat->Draw("samePL");
	
  l_xs_comparison_bw_stat_sys_fat->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (0.118 fb^{-1}): PF AK5 jets");

  if (sReco.find("fat30") != string::npos)
    l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (0.118 fb^{-1}): Wide jets");

  //  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry((TObject*)0,"0.118 fb^{-1}","")
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();


  string sOut_png = sRep + "/Exclusions_qg_SysErrorsDependance_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_qg_SysErrorsDependance_lowMass_"+sReco+".eps";

  c_xs_comparison_bw_stat_sys_fat->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat->SaveAs(sOut_eps.c_str());

  // c_xs_qg_compare
  /*
  c_xs_qg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qg_compare->Draw("PL");

  l_xs_qg_compare->SetFillColor(0);
  l_xs_qg_compare->SetFillStyle(0);
  l_xs_qg_compare->SetHeader("qg-(Stat.Only)-0.118 fb^{-1}");
  l_xs_qg_compare->AddEntry(g_xs_HT_fat_qg_compare,"HT-fat","pL");

  l_xs_qg_compare->Draw("sames");

  c_xs_qg_compare->Update();

  // c_xs_gg_compare
  
  c_xs_gg_compare->cd(0);
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
  l_xs_gg_compare->SetHeader("gg-(Stat.Only)-0.118 fb^{-1}");
  l_xs_gg_compare->AddEntry(g_xs_HT_fat_gg_compare,"HT-fat","pL");

  l_xs_gg_compare->Draw("sames");

  c_xs_gg_compare->Update();

  // c_xs_qq_compare

  c_xs_qq_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qq_compare->Draw("PL");

  l_xs_qq_compare->SetFillColor(0);
  l_xs_qq_compare->SetFillStyle(0);
  l_xs_qq_compare->SetHeader("qq-(Stat.Only)-0.118 fb^{-1}");
  l_xs_qq_compare->AddEntry(g_xs_HT_fat_qq_compare,"HT-fat","pL");

  l_xs_qq_compare->Draw("sames");

  c_xs_qq_compare->Update();
  */
  // c_xs_rsg

  c_xs_rsg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec, 2050.0, 25.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  g_xs_RSG_for_limit_fat->Draw("PL");
  g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");
  //  g_xs_string->Draw("sameL");  
  //  g_xs_qstar->Draw("sameL");
  //  g_xs_axigluon->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_rsg->SetFillColor(0);
  l_xs_rsg->SetFillStyle(0);
  l_xs_rsg->Clear();
  //  l_xs_rsg->SetHeader("CMS Preliminary");
  //  l_xs_rsg->AddEntry((TObject*)0,"0.118 fb^{-1}","");
  l_xs_rsg->SetHeader("95% CL Upper Limit: AK5 PF jets");
  if (sReco.find("fat30") != string::npos)
    l_xs_rsg->SetHeader("95% CL Upper Limit: Wide jets");


  l_xs_rsg->AddEntry(g_xs_HT_fat_qq_1010_sys,"HT-fat qq","pL");
  l_xs_rsg->AddEntry(g_xs_HT_fat_gg_1010_sys,"HT-fat gg","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_fat,"HT-fat w1*qq+w2*gg","pL");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");

  l_xs_rsg->Draw("sames");
  //  l_xs_theory->Draw("sames");

  c_xs_rsg->Update();

  string sOut_png = sRep + "/Exclusions_ggqq_Graviton_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_ggqq_Graviton_lowMass_"+sReco+".eps";

  c_xs_rsg->SaveAs(sOut_png.c_str());
  c_xs_rsg->SaveAs(sOut_eps.c_str());


}
s_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qq_sys_E->SetTitle("");
  h_xs_HT_fat_qq_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qq_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qq_sys_E->Draw("C");
  
  h_xs_qq_sys_E->GetXaxis()->SetTitle("qq Resonance Mass (GeV)");
  h_xs_qq_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");
 
  g_xs_HT_fat_qq_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qq_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qq_sys_E->Draw();
	
  p_xs_qq_sys_E->SetFillColor(0);
  p_xs_qq_sys_E->SetFillStyle(0);
  p_xs_qq_sys_E->SetBorderSize(0);


  p_xs_qq_sys_E->AddText("CMS (0.118 fb^{-1})");
  p_xs_qq_sys_E->AddText("#sqrt{s} = 7 TeV");

  if (sReco.find("fat30") != string::npos)
    p_xs_qq_sys_E->AddText("Wide jets");
  else 
    p_xs_qq_sys_E->AddText("PF AK5 jets");


  p_xs_qq_sys_E->Draw("sames");

  l_xs_qq_sys_E->SetFillColor(0);
  l_xs_qq_sys_E->SetFillStyle(0);
  //  l_xs_qq_sys_E->SetHeader("CMS (0.118 fb^{-1})");
  //  l_xs_qq_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_sys_E->AddEntry((TObject*)0,"0.118 fb^{-1}","")
  l_xs_qq_sys_E->AddEntry(g_xs_HT_fat_qq_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qq_sys_E->AddEntry(g_xs_HT_fat_qq_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qq_sys_E->Draw("sames");
  l_xs_theory_qq->SetFillColor(0);
  l_xs_theory_qq->SetFillStyle(0);
  l_xs_theory_qq->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  l_xs_theory_qq->AddEntry(g_xs_axigluon,"(NLO) arXiv:1111.7261v2","");
  l_xs_theory_qq->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory_qq->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory_qq->AddEntry(g_xs_zprime,"Z'","L");
  //  l_xs_theory_qq->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_qq->Draw("sames");
  
  c_xs_qq_sys_E->Update();

  string sOut_png = sRep + "/Exclusions_qq_withExpected_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_qq_withExpected_lowMass_"+sReco+".eps";

  c_xs_qq_sys_E->SaveAs(sOut_png.c_str());
  c_xs_qq_sys_E->SaveAs(sOut_eps.c_str());

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
  vFrame->SetMinimum(0.7);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_comparison_bw_stat_bg_fat->Draw("PL");;
  //  g_xs_comparison_bw_stat_bg_alternative_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_lumi_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JES_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JER_fat->Draw("samePL");
  g_xs_comparison_bw_stat_sys_fat->Draw("samePL");
	
  l_xs_comparison_bw_stat_sys_fat->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (0.118 fb^{-1}): PF AK5 jets");

  if (sReco.find("fat30") != string::npos)
    l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (0.118 fb^{-1}): Wide jets");

  //  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry((TObject*)0,"0.118 fb^{-1}","")
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_alternative_fat,"background alt./ stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();


  string sOut_png = sRep + "/Exclusions_qg_SysErrorsDependance_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_qg_SysErrorsDependance_lowMass_"+sReco+".eps";

  c_xs_comparison_bw_stat_sys_fat->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat->SaveAs(sOut_eps.c_str());





  c_xs_comparison_bw_stat_sys_fat_norm->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(sys.included 95% CL #sigma) / (stat.only 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.7);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_comparison_bw_stat_lumi_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JES_fat->Draw("samePL");
  g_xs_comparison_bw_stat_JER_fat->Draw("samePL");
  g_xs_comparison_bw_stat_lumi_fat_norm->Draw("samePL");
  g_xs_comparison_bw_stat_JES_fat_norm->Draw("samePL");
  g_xs_comparison_bw_stat_JER_fat_norm->Draw("samePL");
	
  l_xs_comparison_bw_stat_sys_fat_norm->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_norm->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (0.118 fb^{-1}): PF AK5 jets");

  if (sReco.find("fat30") != string::npos)
    l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (0.118 fb^{-1}): Wide jets");


  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
 l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_lumi_fat_norm,"Lumi Normal / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JES_fat_norm,"JES Normal / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JER_fat_norm,"JER Normal / stat","pL");

  l_xs_comparison_bw_stat_sys_fat_norm->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_norm->Update();


  string sOut_png = sRep + "/Exclusions_qg_SysErrorsDependance_priorDependance_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_qg_SysErrorsDependance_priorDependance_lowMass_"+sReco+".eps";

  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_eps.c_str());




  c_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(noFirstGeVBin 95% CL #sigma) / (all bins 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.5);
  vFrame->SetMaximum(2.7);
  // vFrame->SetLogy();

  g_xs_HT_fat_qq_compare_noFirstGeVBin->Draw("samePL");
  g_xs_HT_fat_qg_compare_noFirstGeVBin->Draw("samePL");
  g_xs_HT_fat_gg_compare_noFirstGeVBin->Draw("samePL");

  l_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->SetFillStyle(0);

    l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (0.118 fb^{-1}): PF AK5 jets");

  if (sReco.find("fat30") != string::npos)
    l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (0.118 fb^{-1}): Wide jets");



  l_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->AddEntry(g_xs_HT_fat_qq_compare_noFirstGeVBin,"qq","pL");
  l_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->AddEntry(g_xs_HT_fat_qg_compare_noFirstGeVBin,"qg","pL");
  l_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->AddEntry(g_xs_HT_fat_gg_compare_noFirstGeVBin,"gg","pL");

  l_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->Update();


  string sOut_png = "Fit_Results/Exclusions_qg_SysErrorsDependance_noFirstGeVBin_lowMass_"+sReco+".png";
  string sOut_eps = "Fit_Results/Exclusions_qg_SysErrorsDependance_noFirstGeVBin_lowMass_"+sReco+".eps";

  c_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat_noFirstGeVBin->SaveAs(sOut_eps.c_str());






  // c_xs_qg_compare
  /*
  c_xs_qg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qg_compare->Draw("PL");

  l_xs_qg_compare->SetFillColor(0);
  l_xs_qg_compare->SetFillStyle(0);
  l_xs_qg_compare->SetHeader("qg-(Stat.Only)-0.118 fb^{-1}");
  l_xs_qg_compare->AddEntry(g_xs_HT_fat_qg_compare,"HT-fat","pL");

  l_xs_qg_compare->Draw("sames");

  c_xs_qg_compare->Update();

  // c_xs_gg_compare
  
  c_xs_gg_compare->cd(0);
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
  l_xs_gg_compare->SetHeader("gg-(Stat.Only)-0.118 fb^{-1}");
  l_xs_gg_compare->AddEntry(g_xs_HT_fat_gg_compare,"HT-fat","pL");

  l_xs_gg_compare->Draw("sames");

  c_xs_gg_compare->Update();

  // c_xs_qq_compare

  c_xs_qq_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qq_compare->Draw("PL");

  l_xs_qq_compare->SetFillColor(0);
  l_xs_qq_compare->SetFillStyle(0);
  l_xs_qq_compare->SetHeader("qq-(Stat.Only)-0.118 fb^{-1}");
  l_xs_qq_compare->AddEntry(g_xs_HT_fat_qq_compare,"HT-fat","pL");

  l_xs_qq_compare->Draw("sames");

  c_xs_qq_compare->Update();
  */
  // c_xs_rsg

  c_xs_rsg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec, 2050.0, 25.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  g_xs_RSG_for_limit_fat->Draw("PL");
  g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");
  //  g_xs_string->Draw("sameL");  
  //  g_xs_qstar->Draw("sameL");
  //  g_xs_axigluon->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_rsg->SetFillColor(0);
  l_xs_rsg->SetFillStyle(0);
  l_xs_rsg->Clear();
  //  l_xs_rsg->SetHeader("CMS Preliminary");
  //  l_xs_rsg->AddEntry((TObject*)0,"0.118 fb^{-1}","");
  l_xs_rsg->SetHeader("95% CL Upper Limit: AK5 PF jets");
  if (sReco.find("fat30") != string::npos)
    l_xs_rsg->SetHeader("95% CL Upper Limit: Wide jets");


  l_xs_rsg->AddEntry(g_xs_HT_fat_qq_1010_sys,"HT-fat qq","pL");
  l_xs_rsg->AddEntry(g_xs_HT_fat_gg_1010_sys,"HT-fat gg","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_fat,"HT-fat w1*qq+w2*gg","pL");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");

  l_xs_rsg->Draw("sames");
  //  l_xs_theory->Draw("sames");

  c_xs_rsg->Update();

  string sOut_png = sRep + "/Exclusions_ggqq_Graviton_lowMass_"+sReco+".png";
  string sOut_eps = sRep + "/Exclusions_ggqq_Graviton_lowMass_"+sReco+".eps";

  c_xs_rsg->SaveAs(sOut_png.c_str());
  c_xs_rsg->SaveAs(sOut_eps.c_str());


}
