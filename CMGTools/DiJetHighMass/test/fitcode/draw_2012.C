#include <iostream>
#include <iomanip>


const unsigned int nEBins = 39;
double minMass = 850.0, maxMass = 4850.0;

double x[nEBins] = {1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0, 3000.0, 3100.0, 3200.0, 3300.0, 3400.0, 3500.0, 3600.0, 3700.0, 3800.0, 3900.0, 4000.0, 4100.0, 4200., 4300., 4400., 4500., 4600., 4700., 4800.};

void draw_2012() 
{
  std::cout << "begin" << std::endl;

  double xMin = 0.00004;
  double xMax = 50.1;
  
  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  //  gSystem->Load("libFWCoreFWLite.so");

  TFile *outf = new TFile("xs.root","RECREATE");

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
  TCanvas *c_xs_comparison_bw_stat_sys_fat_norm = new TCanvas("c_xs_comparison_bw_stat_sys_fat_norm","c_xs_comparison_bw_stat_sys_fat_norm",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_fat_Bkg = new TCanvas("c_xs_comparison_bw_stat_sys_fat_Bkg","c_xs_comparison_bw_stat_sys_fat_Bkg",459,358,600,602);
  TCanvas *c_xs_comparison_bw_fit_method = new TCanvas("c_xs_comparison_bw_fit_method", "c_xs_comparison_bw_fit_method",459,358,600,602);

  /*
  TCanvas *c_xs_gg_compare = new TCanvas("c_xs_gg_compare","c_xs_gg_compare",459,358,600,602);
  TCanvas *c_xs_qg_compare = new TCanvas("c_xs_qg_compare","c_xs_qg_compare",459,358,600,602);
  TCanvas *c_xs_qq_compare = new TCanvas("c_xs_qq_compare","c_xs_qq_compare",459,358,600,602);
  */
  TCanvas *c_xs_comparison_bw_sys_theory_fat = new TCanvas("c_xs_comparison_bw_sys_theory_fat","c_xs_comparison_bw_sys_theory_fat",459,358,600,602);

  TCanvas *c_xs_qg_stat_E = new TCanvas("c_xs_qg_stat_E","c_xs_qg_stat_E",459,358,600,602);
  TCanvas *c_xs_qq_stat_E = new TCanvas("c_xs_qq_stat_E","c_xs_qq_stat_E",459,358,600,602);
  TCanvas *c_xs_gg_sys_E = new TCanvas("c_xs_gg_sys_E","c_xs_gg_sys_E",459,358,600,602);
  TCanvas *c_xs_qg_sys_E = new TCanvas("c_xs_qg_sys_E","c_xs_qg_sys_E",459,358,600,602);
  TCanvas *c_xs_qq_sys_E = new TCanvas("c_xs_qq_sys_E","c_xs_qq_sys_E",459,358,600,602);
  TCanvas *c_xs_rsg = new TCanvas("c_xs_rsg","c_xs_rsg",459,358,600,602);

  // definition of legends

  TLegend *l_xs_theory =  new TLegend(0.66,0.6408014,0.965,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_gg =  new TLegend(0.66,0.8108014,0.95,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qg =  new TLegend(0.66,0.8108014,0.95,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qq =  new TLegend(0.62,0.7108014,0.92,0.9216028,NULL,"brNDC");

  TLegend *l_xs_gg =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qg =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qq =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_stat_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_sys =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_all_fat =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat_norm =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat_Bkg =  new TLegend(0.20094631,0.1485366,0.50079866,0.4062369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat_Bkg1 =  new TLegend(0.6094631,0.1485366,0.89079866,0.4062369,NULL,"brNDC");

  TLegend *l_xs_comparison_bw_fit_method =  new TLegend(0.50,0.60,0.92,0.92,NULL,"brNDC");

  TLegend *l_xs_gg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qq_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_fat =  new TLegend(0.1694631,0.1585366,0.5016779,0.4611498,NULL,"brNDC");
  TLegend *l_xs_gg_sys_E =  new TLegend(0.145,0.1445993,0.635,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qg_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qq_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qg_stat_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qq_stat_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");

  TLegend *l_xs_rsg =  new TLegend(0.63,0.75,0.93,0.90,NULL,"brNDC");
  TLegend *l_xs_rsg_2 =  new TLegend(0.13,0.75,0.55,0.90,NULL,"brNDC");

  TPaveText *p_xs_RSG_sys_E =  new TPaveText(0.1527517,0.1555401,0.3773826,0.3087805,"brNDC");

  TPaveText *p_xs_gg_sys_E =  new TPaveText(0.158,0.4255401,0.3773826,0.5187805,"brNDC");
  TPaveText *p_xs_qg_sys_E =  new TPaveText(0.158,0.4255401,0.3773826,0.5187805,"brNDC");
  TPaveText *p_xs_qq_sys_E =  new TPaveText(0.158,0.4255401,0.3773826,0.5187805,"brNDC");

  TPaveText *p_xs_qg_stat_E =  new TPaveText(0.1527517,0.4255401,0.3773826,0.5787805,"brNDC");
  TPaveText *p_xs_qq_stat_E =  new TPaveText(0.1527517,0.4255401,0.3773826,0.5787805,"brNDC");

  TPaveText *p_xs_sys = new TPaveText(0.16,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_fat = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");

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

  c_xs_qg_stat_E->SetLogy();
  c_xs_qq_stat_E->SetLogy();

  c_xs_rsg->SetLogy();
  //  c_xs_comparison_bw_stat_sys_fat_Bkg->SetLogy();

  const unsigned int en = 43;

  // theory values


  double mjj [en] ={ 800.0, 900.0, 1000.0, 1100.0, 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300., 4400., 4500., 4600., 4700., 4800., 4900., 5000.};



  double s8 [en] = {0.34423E+03, 0.16418E+03, 0.83184E+02, 0.43586E+02, 0.23843E+02, 0.13421E+02, 0.77323E+01, 0.45571E+01, 0.27610E+01, 0.16887E+01, 0.10432E+01, 0.66050E+00, 0.42122E+00, 0.27105E+00, 0.17578E+00, 0.11572E+00, 0.76806E-01, 0.51318E-01, 0.34438E-01, 0.23367E-01, 0.15996E-01, 0.10943E-01, 0.76160E-02, 0.52788E-02, 0.37180E-02, 0.26289E-02, 0.18646E-02, 0.13433E-02, 0.97133E-03, 0.71160E-03, 0.52494E-03, 0.39094E-03, 0.29345E-03, 0.22235E-03, 0.17078E-03, 0.13204E-03, 0.10346E-03, 0.81597E-04, 0.64931E-04, 0.52342E-04, 0.42106E-04, 0.34290E-04, 0.28036E-04};



  // string are dummy for the moment

  double string [en] = {
6878.0, 3732., 2127., 1258., 767.6, 477.9, 306.0, 197.6, 130.8, 87.29, 
59.45, 40.39, 28.00, 19.52, 13.57, 9.522, 6.702, 4.791, 3.406, 
2.426, 1.757, 1.248, 0.9037, 0.6502, 0.4714, 0.3362, 0.2439, 0.1750, 0.1259, 
0.0899, 0.0648, 0.0464, 0.0333, 0.0236, 0.0167, 0.0119, 0.00849, 0.00590, 0.00414, 
0.00288, 0.00199, 0.00137, 0.000948};



double qstar[en] = { 
0.3445E+03,  0.1890E+03,  0.1082E+03,  0.6411E+02,  0.3909E+02,  0.2440E+02,  0.1554E+02,  0.1007E+02,  0.6613E+01,  0.4399E+01,  
0.2957E+01,  0.2006E+01,  0.1372E+01,  0.9446E+00,  0.6543E+00,  0.4554E+00,  0.3183E+00,  0.2233E+00,  0.1572E+00,  0.1109E+00,  
0.7837E-01,  0.5548E-01,  0.3932E-01,  0.2788E-01,  0.1978E-01,  0.1403E-01,  0.9951E-02,  0.7052E-02,  0.4993E-02,  0.3531E-02,  
0.2494E-02,  0.1758E-02,  0.1238E-02,  0.8692E-03,  0.6092E-03,  0.4260E-03,  0.2972E-03,  0.2068E-03,  0.1436E-03,  0.9948E-04,  
0.6875E-04,  0.4742E-04,  0.3264E-04 
};
double axigluon[en] = { 
0.1631E+03,  0.9480E+02,  0.5725E+02,  0.3564E+02,  0.2274E+02,  0.1479E+02,  0.9775E+01,  0.6547E+01,  0.4434E+01,  0.3031E+01,  
0.2088E+01,  0.1447E+01,  0.1009E+01,  0.7065E+00,  0.4966E+00,  0.3500E+00,  0.2472E+00,  0.1749E+00,  0.1239E+00,  0.8782E-01,  
0.6224E-01,  0.4409E-01,  0.3119E-01,  0.2204E-01,  0.1554E-01,  0.1093E-01,  0.7660E-02,  0.5350E-02,  0.3721E-02,  0.2577E-02,  
0.1775E-02,  0.1215E-02,  0.8268E-03,  0.5585E-03,  0.3746E-03,  0.2491E-03,  0.1642E-03,  0.1072E-03,  0.6939E-04,  0.4441E-04,  
0.2813E-04,  0.1762E-04,  0.1092E-04 
};
double diquark[en] = { 
0.6564E+02,  0.4358E+02,  0.2985E+02,  0.2095E+02,  0.1498E+02,  0.1088E+02,  0.7993E+01,  0.5929E+01,  0.4431E+01,  0.3333E+01,  
0.2518E+01,  0.1910E+01,  0.1453E+01,  0.1108E+01,  0.8461E+00,  0.6466E+00,  0.4943E+00,  0.3779E+00,  0.2887E+00,  0.2204E+00,  
0.1680E+00,  0.1278E+00,  0.9709E-01,  0.7356E-01,  0.5559E-01,  0.4188E-01,  0.3144E-01,  0.2352E-01,  0.1752E-01,  0.1300E-01,  
0.9598E-02,  0.7051E-02,  0.5152E-02,  0.3742E-02,  0.2701E-02,  0.1936E-02,  0.1378E-02,  0.9736E-03,  0.6822E-03,  0.4737E-03,  
0.3259E-03,  0.2219E-03,  0.1495E-03 
};
double zprime[en] = { 
0.4681E+01,  0.2761E+01,  0.1689E+01,  0.1063E+01,  0.6851E+00,  0.4498E+00,  0.2998E+00,  0.2024E+00,  0.1381E+00,  0.9514E-01,  0.6600E-01,  0.4608E-01,  0.3235E-01,  0.2281E-01,  0.1614E-01,  0.1145E-01,  0.8141E-02,  0.5798E-02,  0.4133E-02,  0.2948E-02,  0.2102E-02,  0.1498E-02,  0.1066E-02,  0.7573E-03,  0.5369E-03,  0.3795E-03,  0.2675E-03,  0.1877E-03,  0.1312E-03,  0.9122E-04,  0.6309E-04,  0.4337E-04,  0.2962E-04,  0.2008E-04,  0.1351E-04,  0.9007E-05,  0.5955E-05,  0.3899E-05,  0.2528E-05,  0.1622E-05,  0.1029E-05,  0.6458E-06,  0.4008E-06 
};
double wprime[en] = { 
0.8417E+01,  0.5048E+01,  0.3135E+01,  0.2000E+01,  0.1304E+01,  0.8650E+00,  0.5815E+00,  0.3951E+00,  0.2708E+00,  0.1869E+00,  0.1296E+00,  0.9022E-01,  0.6298E-01,  0.4404E-01,  0.3081E-01,  0.2156E-01,  0.1507E-01,  0.1052E-01,  0.7333E-02,  0.5096E-02,  0.3531E-02,  0.2438E-02,  0.1676E-02,  0.1147E-02,  0.7807E-03,  0.5286E-03,  0.3559E-03,  0.2380E-03,  0.1581E-03,  0.1043E-03,  0.6835E-04,  0.4445E-04,  0.2869E-04,  0.1837E-04,  0.1169E-04,  0.7378E-05,  0.4629E-05,  0.2887E-05,  0.1793E-05,  0.1108E-05,  0.6842E-06,  0.4219E-06,  0.2603E-06 
};
double rsgraviton[en] = { 
0.6068E+01,  0.3154E+01,  0.1730E+01,  0.9910E+00,  0.5883E+00,  0.3595E+00,  0.2251E+00,  0.1438E+00,  0.9349E-01,  0.6167E-01,  0.4117E-01,  0.2778E-01,  0.1891E-01,  0.1296E-01,  0.8944E-02,  0.6202E-02,  0.4318E-02,  0.3017E-02,  0.2113E-02,  0.1482E-02,  0.1041E-02,  0.7318E-03,  0.5142E-03,  0.3610E-03,  0.2532E-03,  0.1772E-03,  0.1237E-03,  0.8609E-04,  0.5969E-04,  0.4122E-04,  0.2832E-04,  0.1936E-04,  0.1315E-04,  0.8874E-05,  0.5946E-05,  0.3951E-05,  0.2604E-05,  0.1701E-05,  0.1100E-05,  0.7044E-06,  0.4464E-06,  0.2798E-06,  0.1735E-06 
};


  double axigluon_kfactor[en] = {
1.09, 1.10, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 
1.20, 1.21, 1.22, 1.24, 1.25, 1.26, 1.27, 1.29, 1.30, 1.32, 
1.33, 1.35, 1.36, 1.39, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 
1.51, 1.53, 1.55, 1.56, 1.57, 1.58, 1.59, 1.60, 1.61, 1.62,
1.63, 1.64, 1.65};
  // after 1.56 I just extrapolated. To be cheked

  for (int i = 0; i < en; i++){
    axigluon[i] *= axigluon_kfactor[i];
  }

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
// 2100      1.24
// 2200 1.25 1.25 1.26
// 2300      1.26
// 2400 1.28 1.27 1.29
// 2500      1.29
// 2600 1.30 1.30 1.32
// 2700      1.32
// 2800 1.33 1.33 1.35
// 2900      1.35
// 3000 1.37 1.36 1.38
// 3100      1.39
// 3200 1.40 1.40 1.41
// 3300      1.42
// 3400 1.43 1.43 1.44
// 3500      1.45
// 3600 1.47 1.47 1.48
// 3700      1.49
// 3800 1.51 1.51 1.52
// 3900      1.53
// 4000 1.55 1.55 1.56


 double x_RSG[5] = {1000.0, 1100.0, 1200.0, 1300., 1400.};




 TFile* file_efficiencies_qq = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/Resonance_Shapes_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30.root", "READ");
 TFile* file_efficiencies_qg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/Resonance_Shapes_Qstar_2012_D6T_ak5_fat30.root", "READ");
 TFile* file_efficiencies_gg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/Resonance_Shapes_RSGraviton_2012_D6T_ak5_GGtoGG_fat30.root", "READ");

 TH1F* limit_efficiencies_qq = (TH1F*) file_efficiencies_qq->Get("Efficiency;1");
 TH1F* limit_efficiencies_qg = (TH1F*) file_efficiencies_qg->Get("Efficiency;1");
 TH1F* limit_efficiencies_gg = (TH1F*) file_efficiencies_gg->Get("Efficiency;1");

 TFile* file_efficiencies_qq_2011 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_RSGraviton_ak5_QQtoQQ_fat30.root", "READ");
 TFile* file_efficiencies_qg_2011 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_Qstar_ak5_fat30.root", "READ");
 TFile* file_efficiencies_gg_2011 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_RSGraviton_ak5_GGtoGG_fat30.root", "READ");

 TH1F* limit_efficiencies_qq_2011 = (TH1F*) file_efficiencies_qq_2011->Get("Efficiency;1");
 TH1F* limit_efficiencies_qg_2011 = (TH1F*) file_efficiencies_qg_2011->Get("Efficiency;1");
 TH1F* limit_efficiencies_gg_2011 = (TH1F*) file_efficiencies_gg_2011->Get("Efficiency;1");


 TFile* file_qq0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_qg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_0.root", "READ");
 TFile* file_gg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_GGtoGG_fat30_0.root", "READ");

 
 TH1F* limit_qq0 = (TH1F*) file_qq0->Get("limit;1");
 TH1F* limit_qg0 = (TH1F*) file_qg0->Get("limit;1");
 TH1F* limit_gg0 = (TH1F*) file_gg0->Get("limit;1");


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



 TFile* file_qq2000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_qg2000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_gg2000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");

 TFile* file_qq2100 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_qg2100 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_0.root", "READ");
 TFile* file_gg2100 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_GGtoGG_fat30_0.root", "READ");
 
 TFile* file_qq10000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_qg10000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_gg10000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");



 TH1F* limit_qq10000 = (TH1F*) file_qq10000->Get("limit;1");
 TH1F* limit_qg10000 = (TH1F*) file_qg10000->Get("limit;1");
 TH1F* limit_gg10000 = (TH1F*) file_gg10000->Get("limit;1");

 TH1F* limit_qq2000 = (TH1F*) file_qq2000->Get("limit;1");
 TH1F* limit_qg2000 = (TH1F*) file_qg2000->Get("limit;1");
 TH1F* limit_gg2000 = (TH1F*) file_gg2000->Get("limit;1");

 TH1F* limit_qq2100 = (TH1F*) file_qq2100->Get("limit;1");
 TH1F* limit_qg2100 = (TH1F*) file_qg2100->Get("limit;1");
 TH1F* limit_gg2100 = (TH1F*) file_gg2100->Get("limit;1");


 for (int i = 1; i < limit_qq2000->GetNbinsX()+1; i++){

   limit_qq10000->SetBinContent(i, limit_qq10000->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qg10000->SetBinContent(i, limit_qg10000->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_gg10000->SetBinContent(i, limit_gg10000->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

   limit_qq2000->SetBinContent(i, limit_qq2000->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qg2000->SetBinContent(i, limit_qg2000->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_gg2000->SetBinContent(i, limit_gg2000->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

   limit_qq2100->SetBinContent(i, limit_qq2100->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qg2100->SetBinContent(i, limit_qg2100->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_gg2100->SetBinContent(i, limit_gg2100->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));


 }

 // fat-stat only --
 // fat-stat only --
 double* xs_HT_fat_qq_1010_JeffereyPrior = new double[nEBins];
 double* xs_HT_fat_qg_1010_JeffereyPrior = new double[nEBins];
 double* xs_HT_fat_gg_1010_JeffereyPrior = new double[nEBins];
 double* xs_HT_fat_qq_1010_nonConservativeFit = new double[nEBins];
 double* xs_HT_fat_qg_1010_nonConservativeFit = new double[nEBins];
 double* xs_HT_fat_gg_1010_nonConservativeFit = new double[nEBins];
 double* xs_HT_fat_qq_1010_excludedFitRange = new double[nEBins];
 double* xs_HT_fat_qg_1010_excludedFitRange = new double[nEBins];
 double* xs_HT_fat_gg_1010_excludedFitRange = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qq_1010_JeffereyPrior[i] = limit_qq10000->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JeffereyPrior[i] = limit_qg10000->GetBinContent(i+1);
   xs_HT_fat_gg_1010_JeffereyPrior[i] = limit_gg10000->GetBinContent(i+1);

   xs_HT_fat_qq_1010_nonConservativeFit[i] = limit_qq2000->GetBinContent(i+1);
   xs_HT_fat_qg_1010_nonConservativeFit[i] = limit_qg2000->GetBinContent(i+1);
   xs_HT_fat_gg_1010_nonConservativeFit[i] = limit_gg2000->GetBinContent(i+1);

   xs_HT_fat_qq_1010_excludedFitRange[i] = limit_qq2100->GetBinContent(i+1);
   xs_HT_fat_qg_1010_excludedFitRange[i] = limit_qg2100->GetBinContent(i+1);
   xs_HT_fat_gg_1010_excludedFitRange[i] = limit_gg2100->GetBinContent(i+1);
 }

 file_qq10000->Close();
 file_qg10000->Close();
 file_gg10000->Close(); 
 file_qq2000->Close();
 file_qg2000->Close();
 file_gg2000->Close(); 
 file_qq2100->Close();
 file_qg2100->Close();
 file_gg2100->Close(); 

 

 cout << "looking differentially for systematics" << endl;


 // TFile* file_qg_bg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1.root", "READ");
 // TFile* file_qg_bg_alternative = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_11.root", "READ");
 // TFile* file_qg_lumi = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_2.root", "READ");
 // TFile* file_qg_JES = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_3.root", "READ");
 // TFile* file_qg_JER = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_4.root", "READ");
 
 // TFile* file_qg_lumi_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1002.root", "READ");
 // TFile* file_qg_JES_norm= new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1003.root", "READ");
 // TFile* file_qg_JER_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1003.root", "READ"); 

 
 TFile* file_qg_bg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_lumi = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_2.root", "READ");
 TFile* file_qg_JES = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_3.root", "READ");
 TFile* file_qg_JER = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_4.root", "READ");
 
 TFile* file_qg_lumi_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_0.root", "READ");
 TFile* file_qg_JES_norm= new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_JER_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 

 TFile* file_qg_bg_FitA = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_Fit2011 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");

 TFile* file_gg_bg_FitA = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_gg_bg_Fit2011 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");

 TFile* file_qq_bg_FitA = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qq_bg_Fit2011 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");

 TFile* file_qg_bg_alternative = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternativeB = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternative1Up = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternative2Up = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternative3Up = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternative1Do = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternative2Do = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");
 TFile* file_qg_bg_alternative3Do = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_1.root", "READ");




 TH1F* limit_qg_bg = (TH1F*) file_qg_bg->Get("limit;1");
 TH1F* limit_qg_bg_alternative = (TH1F*) file_qg_bg_alternative->Get("limit;1"); 
 TH1F* limit_qg_bg_alternativeB = (TH1F*) file_qg_bg_alternativeB->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative1Up = (TH1F*) file_qg_bg_alternative1Up->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative2Up = (TH1F*) file_qg_bg_alternative2Up->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative3Up = (TH1F*) file_qg_bg_alternative3Up->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative1Do = (TH1F*) file_qg_bg_alternative1Do->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative2Do = (TH1F*) file_qg_bg_alternative2Do->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative3Do = (TH1F*) file_qg_bg_alternative3Do->Get("limit;1"); 

 TH1F* limit_qq_bg_FitA = (TH1F*) file_qq_bg_FitA->Get("limit;1"); 
 TH1F* limit_qq_bg_Fit2011 = (TH1F*) file_qq_bg_Fit2011->Get("limit;1"); 

 TH1F* limit_qg_bg_FitA = (TH1F*) file_qg_bg_FitA->Get("limit;1"); 
 TH1F* limit_qg_bg_Fit2011 = (TH1F*) file_qg_bg_Fit2011->Get("limit;1"); 

 TH1F* limit_gg_bg_FitA = (TH1F*) file_gg_bg_FitA->Get("limit;1"); 
 TH1F* limit_gg_bg_Fit2011 = (TH1F*) file_gg_bg_Fit2011->Get("limit;1"); 


 TH1F* limit_qg_lumi = (TH1F*) file_qg_lumi->Get("limit;1");
 TH1F* limit_qg_JES = (TH1F*) file_qg_JES->Get("limit;1");
 TH1F* limit_qg_JER = (TH1F*) file_qg_JER->Get("limit;1");
 
 TH1F* limit_qg_lumi_norm = file_qg_lumi_norm->Get("limit;1");
 TH1F* limit_qg_JES_norm = file_qg_JES_norm->Get("limit;1");
 TH1F* limit_qg_JER_norm = file_qg_JER_norm->Get("limit;1");
 

 for (int i = 1; i < limit_qg_bg->GetNbinsX()+1; i++){

   limit_qg_bg->SetBinContent(i, limit_qg_bg->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_lumi->SetBinContent(i, limit_qg_lumi->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_JES->SetBinContent(i, limit_qg_JES->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_JER->SetBinContent(i, limit_qg_JER->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
 
   limit_qg_lumi_norm->SetBinContent(i, limit_qg_lumi_norm->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_JES_norm->SetBinContent(i, limit_qg_JES_norm->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_JER_norm->SetBinContent(i, limit_qg_JER_norm->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));

   limit_qq_bg_FitA->SetBinContent(i, limit_qq_bg_FitA->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_bg_Fit2011->SetBinContent(i, limit_qq_bg_Fit2011->GetBinContent(i) / limit_efficiencies_qq_2011->GetBinContent(i));

   limit_qg_bg_FitA->SetBinContent(i, limit_qg_bg_FitA->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_Fit2011->SetBinContent(i, limit_qg_bg_Fit2011->GetBinContent(i) / limit_efficiencies_qg_2011->GetBinContent(i));

   limit_gg_bg_FitA->SetBinContent(i, limit_gg_bg_FitA->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_bg_Fit2011->SetBinContent(i, limit_gg_bg_Fit2011->GetBinContent(i) / limit_efficiencies_gg_2011->GetBinContent(i));


   limit_qg_bg_alternative->SetBinContent(i, limit_qg_bg_alternative->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_alternativeB->SetBinContent(i, limit_qg_bg_alternativeB->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_alternative1Up->SetBinContent(i, limit_qg_bg_alternative1Up->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_alternative2Up->SetBinContent(i, limit_qg_bg_alternative2Up->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_alternative3Up->SetBinContent(i, limit_qg_bg_alternative3Up->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));

   limit_qg_bg_alternative1Do->SetBinContent(i, limit_qg_bg_alternative1Do->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_alternative2Do->SetBinContent(i, limit_qg_bg_alternative2Do->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_bg_alternative3Do->SetBinContent(i, limit_qg_bg_alternative3Do->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));

}
 

  // fat-stat only --
 double* xs_HT_fat_qg_1010_bg = new double[nEBins];
 double* xs_HT_fat_qg_1010_lumi = new double[nEBins];
 double* xs_HT_fat_qg_1010_JES = new double[nEBins];
 double* xs_HT_fat_qg_1010_JER = new double[nEBins];

 double* xs_HT_fat_qg_1010_lumi_norm = new double[nEBins];
 double* xs_HT_fat_qg_1010_JES_norm = new double[nEBins];
 double* xs_HT_fat_qg_1010_JER_norm = new double[nEBins];

 double* xs_HT_fat_qq_1010_bg_FitA = new double[nEBins];
 double* xs_HT_fat_qq_1010_bg_Fit2011 = new double[nEBins];

 double* xs_HT_fat_qg_1010_bg_FitA = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_Fit2011 = new double[nEBins];

 double* xs_HT_fat_gg_1010_bg_FitA = new double[nEBins];
 double* xs_HT_fat_gg_1010_bg_Fit2011 = new double[nEBins];

 double* xs_HT_fat_qg_1010_bg_alternative = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternativeB = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternative1Up = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternative2Up = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternative3Up = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternative1Do = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternative2Do = new double[nEBins];
 double* xs_HT_fat_qg_1010_bg_alternative3Do = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qg_1010_bg[i] = limit_qg_bg->GetBinContent(i+1);
   xs_HT_fat_qg_1010_lumi[i] = limit_qg_lumi->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JES[i] = limit_qg_JES->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JER[i] = limit_qg_JER->GetBinContent(i+1);

   xs_HT_fat_qg_1010_lumi_norm[i] = limit_qg_lumi_norm->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JES_norm[i] = limit_qg_JES_norm->GetBinContent(i+1);
   xs_HT_fat_qg_1010_JER_norm[i] = limit_qg_JER_norm->GetBinContent(i+1);

   xs_HT_fat_qq_1010_bg_FitA[i] = limit_qq_bg_FitA->GetBinContent(i+1);
   xs_HT_fat_qq_1010_bg_Fit2011[i] = limit_qq_bg_Fit2011->GetBinContent(i+1);

   xs_HT_fat_qg_1010_bg_FitA[i] = limit_qg_bg_FitA->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_Fit2011[i] = limit_qg_bg_Fit2011->GetBinContent(i+1);

   xs_HT_fat_gg_1010_bg_FitA[i] = limit_gg_bg_FitA->GetBinContent(i+1);
   xs_HT_fat_gg_1010_bg_Fit2011[i] = limit_gg_bg_Fit2011->GetBinContent(i+1);


   xs_HT_fat_qg_1010_bg_alternative[i] = limit_qg_bg_alternative->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternativeB[i] = limit_qg_bg_alternativeB->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternative1Up[i] = limit_qg_bg_alternative1Up->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternative2Up[i] = limit_qg_bg_alternative2Up->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternative3Up[i] = limit_qg_bg_alternative3Up->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternative1Do[i] = limit_qg_bg_alternative1Do->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternative2Do[i] = limit_qg_bg_alternative2Do->GetBinContent(i+1);
   xs_HT_fat_qg_1010_bg_alternative3Do[i] = limit_qg_bg_alternative3Do->GetBinContent(i+1);

 }

 file_qg_bg->Close();
 file_qq_bg_FitA->Close();
 file_qq_bg_Fit2011->Close();
 file_qg_bg_FitA->Close();
 file_qg_bg_Fit2011->Close();
 file_gg_bg_FitA->Close();
 file_gg_bg_Fit2011->Close();
 file_qg_bg_alternative->Close();
 file_qg_bg_alternativeB->Close();
 file_qg_bg_alternative1Up->Close();
 file_qg_bg_alternative2Up->Close();
 file_qg_bg_alternative3Up->Close();
 file_qg_bg_alternative1Do->Close();
 file_qg_bg_alternative2Do->Close();
 file_qg_bg_alternative3Do->Close();

 file_qg_lumi->Close();
 file_qg_JES->Close();
 file_qg_JER->Close();
 file_qg_lumi_norm->Close();
 file_qg_JES_norm->Close();
 file_qg_JER_norm->Close();














 cout << "looking for All systematics" << endl;
  // fat-All_Fat sys.included
 TFile* file_qq6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_6.root", "READ");
 TFile* file_qg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_Qstar_2012_D6T_ak5_fat30_6.root", "READ");
 TFile* file_gg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_limit_RSGraviton_2012_D6T_ak5_GGtoGG_fat30_6.root", "READ");

 
 TH1F* limit_qq6 = (TH1F*) file_qq6->Get("limit;1");
 TH1F* limit_qg6 = (TH1F*) file_qg6->Get("limit;1");
 TH1F* limit_gg6 = (TH1F*) file_gg6->Get("limit;1");

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




 TFile* file_exp_qq0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_expectedlimit50_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_exp_qg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_expectedlimit50_Qstar_2012_D6T_ak5_fat30_0.root", "READ");
 TFile* file_exp_gg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_expectedlimit50_RSGraviton_2012_D6T_ak5_GGtoGG_fat30_0.root", "READ");


 TH1F* limit_qq_twosigma_lowers = (TH1F*) file_exp_qq0->Get("htwosigma_lowers;1");
 TH1F* limit_qq_onesigma_lowers = (TH1F*) file_exp_qq0->Get("honesigma_lowers;1");
 TH1F* limit_qq_medians = (TH1F*) file_exp_qq0->Get("hmedians;1");
 TH1F* limit_qq_onesigma_uppers = (TH1F*) file_exp_qq0->Get("honesigma_uppers;1");
 TH1F* limit_qq_twosigma_uppers = (TH1F*) file_exp_qq0->Get("htwosigma_uppers;1");


 for (int i = 1; i < limit_qq_twosigma_lowers->GetNbinsX()+1; i++){

   limit_qq_twosigma_lowers->SetBinContent(i, limit_qq_twosigma_lowers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_onesigma_lowers->SetBinContent(i, limit_qq_onesigma_lowers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_medians->SetBinContent(i, limit_qq_medians->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_onesigma_uppers->SetBinContent(i, limit_qq_onesigma_uppers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qq_twosigma_uppers->SetBinContent(i, limit_qq_twosigma_uppers->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
 }


 double* xs_HT_fat_qq_1010_stat_E_d2 = new double[nEBins];
 double* xs_HT_fat_qq_1010_stat_E_d1 = new double[nEBins];
 double* xs_HT_fat_qq_1010_stat_E_m = new double[nEBins];
 double* xs_HT_fat_qq_1010_stat_E_u1 = new double[nEBins];
 double* xs_HT_fat_qq_1010_stat_E_u2 = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_qq_1010_stat_E_d2[i] = limit_qq_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qq_1010_stat_E_d1[i] = limit_qq_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qq_1010_stat_E_m[i] = limit_qq_medians->GetBinContent(i+1);
   xs_HT_fat_qq_1010_stat_E_u1[i] = limit_qq_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_qq_1010_stat_E_u2[i] = limit_qq_twosigma_uppers->GetBinContent(i+1);

 }







 TH1F* limit_qg_twosigma_lowers = (TH1F*) file_exp_qg0->Get("htwosigma_lowers;1");
 TH1F* limit_qg_onesigma_lowers = (TH1F*) file_exp_qg0->Get("honesigma_lowers;1");
 TH1F* limit_qg_medians = (TH1F*) file_exp_qg0->Get("hmedians;1");
 TH1F* limit_qg_onesigma_uppers = (TH1F*) file_exp_qg0->Get("honesigma_uppers;1");
 TH1F* limit_qg_twosigma_uppers = (TH1F*) file_exp_qg0->Get("htwosigma_uppers;1");


 for (int i = 1; i < limit_qg_twosigma_lowers->GetNbinsX()+1; i++){

   limit_qg_twosigma_lowers->SetBinContent(i, limit_qg_twosigma_lowers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_onesigma_lowers->SetBinContent(i, limit_qg_onesigma_lowers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_medians->SetBinContent(i, limit_qg_medians->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_onesigma_uppers->SetBinContent(i, limit_qg_onesigma_uppers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_qg_twosigma_uppers->SetBinContent(i, limit_qg_twosigma_uppers->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
 }


 double* xs_HT_fat_qg_1010_stat_E_d2 = new double[nEBins];
 double* xs_HT_fat_qg_1010_stat_E_d1 = new double[nEBins];
 double* xs_HT_fat_qg_1010_stat_E_m = new double[nEBins];
 double* xs_HT_fat_qg_1010_stat_E_u1 = new double[nEBins];
 double* xs_HT_fat_qg_1010_stat_E_u2 = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_qg_1010_stat_E_d2[i] = limit_qg_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qg_1010_stat_E_d1[i] = limit_qg_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_qg_1010_stat_E_m[i] = limit_qg_medians->GetBinContent(i+1);
   xs_HT_fat_qg_1010_stat_E_u1[i] = limit_qg_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_qg_1010_stat_E_u2[i] = limit_qg_twosigma_uppers->GetBinContent(i+1);

 }






TH1F* limit_gg_twosigma_lowers = (TH1F*) file_exp_gg0->Get("htwosigma_lowers;1");
 TH1F* limit_gg_onesigma_lowers = (TH1F*) file_exp_gg0->Get("honesigma_lowers;1");
 TH1F* limit_gg_medians = (TH1F*) file_exp_gg0->Get("hmedians;1");
 TH1F* limit_gg_onesigma_uppers = (TH1F*) file_exp_gg0->Get("honesigma_uppers;1");
 TH1F* limit_gg_twosigma_uppers = (TH1F*) file_exp_gg0->Get("htwosigma_uppers;1");


 for (int i = 1; i < limit_gg_twosigma_lowers->GetNbinsX()+1; i++){

   limit_gg_twosigma_lowers->SetBinContent(i, limit_gg_twosigma_lowers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_onesigma_lowers->SetBinContent(i, limit_gg_onesigma_lowers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_medians->SetBinContent(i, limit_gg_medians->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_onesigma_uppers->SetBinContent(i, limit_gg_onesigma_uppers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
   limit_gg_twosigma_uppers->SetBinContent(i, limit_gg_twosigma_uppers->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));
 }


 double* xs_HT_fat_gg_1010_stat_E_d2 = new double[nEBins];
 double* xs_HT_fat_gg_1010_stat_E_d1 = new double[nEBins];
 double* xs_HT_fat_gg_1010_stat_E_m = new double[nEBins];
 double* xs_HT_fat_gg_1010_stat_E_u1 = new double[nEBins];
 double* xs_HT_fat_gg_1010_stat_E_u2 = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_gg_1010_stat_E_d2[i] = limit_gg_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_gg_1010_stat_E_d1[i] = limit_gg_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_gg_1010_stat_E_m[i] = limit_gg_medians->GetBinContent(i+1);
   xs_HT_fat_gg_1010_stat_E_u1[i] = limit_gg_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_gg_1010_stat_E_u2[i] = limit_gg_twosigma_uppers->GetBinContent(i+1);

 }


 file_exp_qq0->Close();
 file_exp_qg0->Close();
 file_exp_gg0->Close();





 TFile* file_exp_qq6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_expectedlimit50_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30_6.root", "READ");
 TFile* file_exp_qg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_expectedlimit50_Qstar_2012_D6T_ak5_fat30_6.root", "READ");
 TFile* file_exp_gg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_SpB/limit_expectedlimit50_RSGraviton_2012_D6T_ak5_GGtoGG_fat30_6.root", "READ");


 TH1F* limit_qq_twosigma_lowers = (TH1F*) file_exp_qq6->Get("htwosigma_lowers;1");
 TH1F* limit_qq_onesigma_lowers = (TH1F*) file_exp_qq6->Get("honesigma_lowers;1");
 TH1F* limit_qq_medians = (TH1F*) file_exp_qq6->Get("hmedians;1");
 TH1F* limit_qq_onesigma_uppers = (TH1F*) file_exp_qq6->Get("honesigma_uppers;1");
 TH1F* limit_qq_twosigma_uppers = (TH1F*) file_exp_qq6->Get("htwosigma_uppers;1");
 

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


 TH1F* limit_qg_twosigma_lowers = (TH1F*) file_exp_qg6->Get("htwosigma_lowers;1");
 TH1F* limit_qg_onesigma_lowers = (TH1F*) file_exp_qg6->Get("honesigma_lowers;1");
 TH1F* limit_qg_medians = (TH1F*) file_exp_qg6->Get("hmedians;1");
 TH1F* limit_qg_onesigma_uppers = (TH1F*) file_exp_qg6->Get("honesigma_uppers;1");
 TH1F* limit_qg_twosigma_uppers = (TH1F*) file_exp_qg6->Get("htwosigma_uppers;1");

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


 TH1F* limit_gg_twosigma_lowers = (TH1F*) file_exp_gg6->Get("htwosigma_lowers;1");
 TH1F* limit_gg_onesigma_lowers = (TH1F*) file_exp_gg6->Get("honesigma_lowers;1");
 TH1F* limit_gg_medians = (TH1F*) file_exp_gg6->Get("hmedians;1");
 TH1F* limit_gg_onesigma_uppers = (TH1F*) file_exp_gg6->Get("honesigma_uppers;1");
 TH1F* limit_gg_twosigma_uppers = (TH1F*) file_exp_gg6->Get("htwosigma_uppers;1");

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


 // 0.16751, 0.15361, 0.14070, 0.12884, 0.11799, 0.10813
 //   0.19590, 0.21457, 
 double gg_ratio[5] = {0.44, 0.45, 0.46, 0.47, 0.48}; //{0.5561,0.5116,0.4671, 0.42, 0.38};
 double qq_ratio[5] = {0.56, 0.55,0.55, 0.53, 0.52};

 double xs_HT_pf_qg_compare[nEBins], xs_HT_pf_gg_compare[nEBins], xs_HT_pf_qq_compare[nEBins], xs_HT_fat_qg_compare[nEBins], xs_HT_fat_gg_compare[nEBins], xs_HT_fat_qq_compare[nEBins], xs_comparison_bw_stat_bg_fat[nEBins], xs_comparison_bw_stat_lumi_fat[nEBins], xs_comparison_bw_stat_JES_fat[nEBins], xs_comparison_bw_stat_JER_fat[nEBins], xs_comparison_bw_stat_sys_fat[nEBins];

 double xs_comparison_bw_stat_bg_alternative_fat[nEBins], xs_comparison_bw_stat_bg_alternativeB_fat[nEBins], xs_comparison_bw_stat_bg_alternative1Up_fat[nEBins], xs_comparison_bw_stat_bg_alternative2Up_fat[nEBins], xs_comparison_bw_stat_bg_alternative3Up_fat[nEBins], xs_comparison_bw_stat_bg_alternative1Do_fat[nEBins], xs_comparison_bw_stat_bg_alternative2Do_fat[nEBins], xs_comparison_bw_stat_bg_alternative3Do_fat[nEBins];

 double  xs_comparison_bw_stat_bg_FitA_fat[nEBins], xs_comparison_bw_stat_bg_Fit2011_fat[nEBins];

 double xs_HT_fat_qg_compare_JeffereyPrior[nEBins], xs_HT_fat_gg_compare_JeffereyPrior[nEBins], xs_HT_fat_qq_compare_JeffereyPrior[nEBins];
 double xs_HT_fat_qg_compare_nonConservativeFit[nEBins], xs_HT_fat_gg_compare_nonConservativeFit[nEBins], xs_HT_fat_qq_compare_nonConservativeFit[nEBins];
 double xs_HT_fat_qg_compare_excludedFitRange[nEBins], xs_HT_fat_gg_compare_excludedFitRange[nEBins], xs_HT_fat_qq_compare_excludedFitRange[nEBins];

 double xs_RSG_for_limit_fat[5], xs_qstar_over_qg_fat[nEBins], xs_string_over_qg_fat[nEBins], xs_diquark_over_qq_fat[nEBins], xs_axigluon_over_qq_fat[nEBins], xs_zprime_over_qq_fat[nEBins], xs_wprime_over_qq_fat[nEBins], xs_rsg_over_gg_fat[nEBins], xs_s8_over_gg_fat[nEBins];


 double xs_RSG_for_limit_stat_fat[5], xs_qstar_over_qg_stat_fat[nEBins], xs_string_over_qg_stat_fat[nEBins], xs_diquark_over_qq_stat_fat[nEBins], xs_axigluon_over_qq_stat_fat[nEBins], xs_zprime_over_qq_stat_fat[nEBins], xs_wprime_over_qq_stat_fat[nEBins], xs_rsg_over_gg_stat_fat[nEBins], xs_s8_over_gg_stat_fat[nEBins];

 double xs_RSG_for_limit_stat_fat_E_m[5], xs_RSG_for_limit_sys_fat_E_m[5], xs_RSG_for_limit_sys_fat_E_d2[5], xs_RSG_for_limit_sys_fat_E_d1[5], xs_RSG_for_limit_sys_fat_E_u1[5], xs_RSG_for_limit_sys_fat_E_u2[5], xs_rsg_over_gg_stat_fat[5], xs_rsg_over_gg_stat_fat_E_m[5], xs_rsg_over_gg_sys_fat_E_m[5];

 double xs_comparison_bw_stat_lumi_fat_norm[nEBins], xs_comparison_bw_stat_JES_fat_norm[nEBins], xs_comparison_bw_stat_JER_fat_norm[nEBins];
 double xs_RSG_for_limit_expected_fat[5], xs_qstar_over_qg_expected_fat[nEBins], xs_string_over_qg_expected_fat[nEBins], xs_diquark_over_qq_expected_fat[nEBins], xs_axigluon_over_qq_expected_fat[nEBins], xs_zprime_over_qq_expected_fat[nEBins], xs_wprime_over_qq_expected_fat[nEBins], xs_rsg_over_gg_expected_fat[nEBins],  xs_s8_over_gg_expected_fat[nEBins];

 double xs_RSG_for_limit_stat_expected_fat[5], xs_qstar_over_qg_stat_expected_fat[nEBins], xs_string_over_qg_stat_expected_fat[nEBins], xs_diquark_over_qq_stat_expected_fat[nEBins], xs_axigluon_over_qq_stat_expected_fat[nEBins], xs_zprime_over_qq_stat_expected_fat[nEBins], xs_wprime_over_qq_stat_expected_fat[nEBins], xs_rsg_over_gg_stat_expected_fat[nEBins],  xs_s8_over_gg_stat_expected_fat[nEBins];

  std::cout << "wide stat only" << std::endl;
  for (int  i = 0 ; i < nEBins ; i++) 
    {
      std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qq_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qg_1010[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010[i] << " \\\\" << std::endl;
      
    }


  std::cout << "wide sys included" << std::endl;
  for (int  i = 0 ; i < nEBins  ; i++) 
    {
      if (i<7) std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(2) << xs_HT_fat_qq_1010_sys[i]<<" & " << std::fixed << setprecision(2) << xs_HT_fat_qg_1010_sys[i] <<" & " << std::fixed << setprecision(2) << xs_HT_fat_gg_1010_sys[i] << " \\\\" << std::endl;
      if (i>6 && i < 19) std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(3) << xs_HT_fat_qq_1010_sys[i]<<" & " << std::fixed << setprecision(3) << xs_HT_fat_qg_1010_sys[i] <<" & " << std::fixed << setprecision(3) << xs_HT_fat_gg_1010_sys[i] << " \\\\" << std::endl;
      if (i>18) std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(4) << xs_HT_fat_qq_1010_sys[i]<<" & " << std::fixed << setprecision(4) << xs_HT_fat_qg_1010_sys[i] <<" & " << std::fixed << setprecision(4) << xs_HT_fat_gg_1010_sys[i] << " \\\\" << std::endl;      
    }



////////////////// Histogram definition /////////////////////
/////////////////////////////////////////////////////////////


  // expected limit for gg

  TH1F *h_xs_HT_fat_gg_1010_sys_E_d2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_gg_1010_sys_E_d1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_gg_1010_sys_E_u1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_gg_1010_sys_E_u2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);

  // expected limit for qg

  TH1F *h_xs_HT_fat_qg_1010_sys_E_d2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qg_1010_sys_E_d1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qg_1010_sys_E_u1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qg_1010_sys_E_u2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);

  // expected limit for qq
        
  TH1F *h_xs_HT_fat_qq_1010_sys_E_d2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qq_1010_sys_E_d1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qq_1010_sys_E_u1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qq_1010_sys_E_u2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);


  // expected limit for qg

  TH1F *h_xs_HT_fat_qg_1010_stat_E_d2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qg_1010_stat_E_d1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qg_1010_stat_E_u1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qg_1010_stat_E_u2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);

  // expected limit for qq
  TH1F *h_xs_HT_fat_qq_1010_stat_E_d2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qq_1010_stat_E_d1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qq_1010_stat_E_u1 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);
  TH1F *h_xs_HT_fat_qq_1010_stat_E_u2 = new TH1F("","",nEBins,x[0]-50,x[nEBins-1]+50);   

  // expected limit for RSG
  TH1F *h_xs_RSG_for_limit_sys_fat_E_d2 = new TH1F("","",5,950,1450);
  TH1F *h_xs_RSG_for_limit_sys_fat_E_d1 = new TH1F("","",5,950,1450);
  TH1F *h_xs_RSG_for_limit_sys_fat_E_u1 = new TH1F("","",5,950,1450);
  TH1F *h_xs_RSG_for_limit_sys_fat_E_u2 = new TH1F("","",5,950,1450);


 for (int i = 0; i<nEBins; i++) {
   
   if (i<16)     std::cout << i+10 << " & " << std::fixed << setprecision(3) << xs_HT_fat_qq_1010_sys[i] << " & " << xs_HT_fat_qg_1010_sys[i] << " & " << xs_HT_fat_gg_1010_sys[i] << " & " << i+26 << " & " << std::fixed << setprecision(4) << xs_HT_fat_qq_1010_sys[i+16] << " & " << xs_HT_fat_qg_1010_sys[i+16] << " & " << xs_HT_fat_gg_1010_sys[i+16] << " \\\\  " << std::endl;
   if (i<5)
     {
       cout << "Filling bin i " << endl;

       xs_RSG_for_limit_stat_fat[i] = xs_HT_fat_gg_1010[i]*gg_ratio[i] + xs_HT_fat_qq_1010[i]*qq_ratio[i];
       xs_RSG_for_limit_stat_fat_E_m[i] = xs_HT_fat_gg_1010_stat_E_m[i]*gg_ratio[i] + xs_HT_fat_qq_1010_stat_E_m[i]*qq_ratio[i];

       xs_RSG_for_limit_sys_fat_E_d2[i] = xs_HT_fat_gg_1010_sys_E_d2[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys_E_d2[i]*qq_ratio[i];
       xs_RSG_for_limit_sys_fat_E_d1[i] = xs_HT_fat_gg_1010_sys_E_d1[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys_E_d1[i]*qq_ratio[i];
       xs_RSG_for_limit_sys_fat_E_m[i] = xs_HT_fat_gg_1010_sys_E_m[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys_E_m[i]*qq_ratio[i];
       xs_RSG_for_limit_sys_fat_E_u1[i] = xs_HT_fat_gg_1010_sys_E_u1[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys_E_u1[i]*qq_ratio[i];
       xs_RSG_for_limit_sys_fat_E_u2[i] = xs_HT_fat_gg_1010_sys_E_u2[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys_E_u2[i]*qq_ratio[i];

       xs_RSG_for_limit_fat[i] = xs_HT_fat_gg_1010_sys[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys[i]*qq_ratio[i];
       xs_rsg_over_gg_fat[i] = rsgraviton[i+2]/xs_RSG_for_limit_fat[i];
       xs_rsg_over_gg_stat_fat[i] = rsgraviton[i+2]/xs_RSG_for_limit_stat_fat[i];
       xs_rsg_over_gg_stat_fat_E_m[i] = rsgraviton[i+2]/xs_RSG_for_limit_stat_fat_E_m[i];
       xs_rsg_over_gg_sys_fat_E_m[i] = rsgraviton[i+2]/xs_RSG_for_limit_sys_fat_E_m[i];
     }
     
   xs_HT_fat_qq_compare_JeffereyPrior[i] = xs_HT_fat_qq_1010_JeffereyPrior[i]/xs_HT_fat_qq_1010[i];
   xs_HT_fat_qg_compare_JeffereyPrior[i] = xs_HT_fat_qg_1010_JeffereyPrior[i] /xs_HT_fat_qg_1010[i];
   xs_HT_fat_gg_compare_JeffereyPrior[i] = xs_HT_fat_gg_1010_JeffereyPrior[i]/xs_HT_fat_gg_1010[i];

   xs_HT_fat_qq_compare_nonConservativeFit[i] = xs_HT_fat_qq_1010_nonConservativeFit[i]/xs_HT_fat_qq_1010[i];
   xs_HT_fat_qg_compare_nonConservativeFit[i] = xs_HT_fat_qg_1010_nonConservativeFit[i] /xs_HT_fat_qg_1010[i];
   xs_HT_fat_gg_compare_nonConservativeFit[i] = xs_HT_fat_gg_1010_nonConservativeFit[i]/xs_HT_fat_gg_1010[i];

   xs_HT_fat_qq_compare_excludedFitRange[i] = xs_HT_fat_qq_1010_excludedFitRange[i]/xs_HT_fat_qq_1010[i];
   xs_HT_fat_qg_compare_excludedFitRange[i] = xs_HT_fat_qg_1010_excludedFitRange[i] /xs_HT_fat_qg_1010[i];
   xs_HT_fat_gg_compare_excludedFitRange[i] = xs_HT_fat_gg_1010_excludedFitRange[i]/xs_HT_fat_gg_1010[i];


   xs_comparison_bw_stat_bg_fat[i] = xs_HT_fat_qg_1010_bg[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_lumi_fat[i] = xs_HT_fat_qg_1010_lumi[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JES_fat[i] = xs_HT_fat_qg_1010_JES[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JER_fat[i] = xs_HT_fat_qg_1010_JER[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_sys_fat[i] = xs_HT_fat_qg_1010_sys[i]/xs_HT_fat_qg_1010[i];

   xs_comparison_bw_stat_bg_FitA_fat[i] = xs_HT_fat_qg_1010_bg_FitA[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_Fit2011_fat[i] = xs_HT_fat_qg_1010_bg_Fit2011[i]/xs_HT_fat_qg_1010[i];

   xs_comparison_bw_stat_bg_alternative_fat[i] = xs_HT_fat_qg_1010_bg_alternative[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternativeB_fat[i] = xs_HT_fat_qg_1010_bg_alternativeB[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternative1Up_fat[i] = xs_HT_fat_qg_1010_bg_alternative1Up[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternative2Up_fat[i] = xs_HT_fat_qg_1010_bg_alternative2Up[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternative3Up_fat[i] = xs_HT_fat_qg_1010_bg_alternative3Up[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternative1Do_fat[i] = xs_HT_fat_qg_1010_bg_alternative1Do[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternative2Do_fat[i] = xs_HT_fat_qg_1010_bg_alternative2Do[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_bg_alternative3Do_fat[i] = xs_HT_fat_qg_1010_bg_alternative3Do[i]/xs_HT_fat_qg_1010[i];


   xs_comparison_bw_stat_lumi_fat_norm[i] = xs_HT_fat_qg_1010_lumi_norm[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JES_fat_norm[i] = xs_HT_fat_qg_1010_JES_norm[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JER_fat_norm[i] = xs_HT_fat_qg_1010_JER_norm[i]/xs_HT_fat_qg_1010[i];


   xs_qstar_over_qg_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010_sys[i];
   xs_string_over_qg_fat[i] = string[i+2]/xs_HT_fat_qg_1010_sys[i];
   xs_diquark_over_qq_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_axigluon_over_qq_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_zprime_over_qq_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_wprime_over_qq_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_sys[i];

   xs_s8_over_gg_fat[i] = s8[i+2]/xs_HT_fat_gg_1010_sys[i];
   
   xs_qstar_over_qg_stat_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010[i];
   xs_string_over_qg_stat_fat[i] = string[i+2]/xs_HT_fat_qg_1010[i];
   xs_diquark_over_qq_stat_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010[i];
   xs_axigluon_over_qq_stat_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010[i];
   xs_zprime_over_qq_stat_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010[i];
   xs_wprime_over_qq_stat_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010[i];

   xs_s8_over_gg_stat_fat[i] = s8[i+2]/xs_HT_fat_gg_1010[i];


   xs_qstar_over_qg_expected_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010_sys_E_m[i];
   xs_string_over_qg_expected_fat[i] = string[i+2]/xs_HT_fat_qg_1010_sys_E_m[i];
   xs_diquark_over_qq_expected_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];
   xs_axigluon_over_qq_expected_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];
   xs_zprime_over_qq_expected_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];
   xs_wprime_over_qq_expected_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];

   xs_s8_over_gg_expected_fat[i] = s8[i+2]/xs_HT_fat_gg_1010_sys_E_m[i];


   xs_qstar_over_qg_stat_expected_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010_stat_E_m[i];
   xs_string_over_qg_stat_expected_fat[i] = string[i+2]/xs_HT_fat_qg_1010_stat_E_m[i];
   xs_diquark_over_qq_stat_expected_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   xs_axigluon_over_qq_stat_expected_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   xs_zprime_over_qq_stat_expected_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   xs_wprime_over_qq_stat_expected_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   
   xs_s8_over_gg_stat_expected_fat[i] = s8[i+2]/xs_HT_fat_gg_1010_stat_E_m[i];

   //xs_HT_fat_gg_1010_sys_E_m


   h_xs_HT_fat_gg_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_gg_1010_sys_E_d2[i]);
   h_xs_HT_fat_gg_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_gg_1010_sys_E_d1[i]);
   h_xs_HT_fat_gg_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_gg_1010_sys_E_u1[i]);
   h_xs_HT_fat_gg_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_gg_1010_sys_E_u2[i]);

   h_xs_HT_fat_qg_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_d2[i]);
   h_xs_HT_fat_qg_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_d1[i]);
   h_xs_HT_fat_qg_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_u1[i]);
   h_xs_HT_fat_qg_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_qg_1010_sys_E_u2[i]);

   h_xs_HT_fat_qq_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_d2[i]);
   h_xs_HT_fat_qq_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_d1[i]);
   h_xs_HT_fat_qq_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_u1[i]);
   h_xs_HT_fat_qq_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_qq_1010_sys_E_u2[i]);

   h_xs_HT_fat_qg_1010_stat_E_d2->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_d2[i]);
   h_xs_HT_fat_qg_1010_stat_E_d1->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_d1[i]);
   h_xs_HT_fat_qg_1010_stat_E_u1->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_u1[i]);
   h_xs_HT_fat_qg_1010_stat_E_u2->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_u2[i]);

   h_xs_HT_fat_qq_1010_stat_E_d2->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_d2[i]);
   h_xs_HT_fat_qq_1010_stat_E_d1->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_d1[i]);
   h_xs_HT_fat_qq_1010_stat_E_u1->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_u1[i]);
   h_xs_HT_fat_qq_1010_stat_E_u2->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_u2[i]);
   if (i<5){
     h_xs_RSG_for_limit_sys_fat_E_d2->SetBinContent(i+1,xs_RSG_for_limit_sys_fat_E_d2[i]);
     h_xs_RSG_for_limit_sys_fat_E_d1->SetBinContent(i+1,xs_RSG_for_limit_sys_fat_E_d1[i]);
     h_xs_RSG_for_limit_sys_fat_E_u1->SetBinContent(i+1,xs_RSG_for_limit_sys_fat_E_u1[i]);
     h_xs_RSG_for_limit_sys_fat_E_u2->SetBinContent(i+1,xs_RSG_for_limit_sys_fat_E_u2[i]);
   }

 }

 double excl_sys[8], excl_stat[8], excl_exp_sys[8], excl_exp_stat[8];

 cout << "" << endl;
 cout << "------------------------------------------" << endl; 
 cout << "Stat only limits" << endl;
 cout << "------------------------------------------" << endl;
 cout << "" << endl;


 excl_stat[0] = findExclusion(xs_string_over_qg_stat_fat, string("Exclusion String stat"), nEBins);
 excl_stat[1] = findExclusion(xs_qstar_over_qg_stat_fat, string("Exclusion Qstar stat"), nEBins);
 excl_stat[2] = findExclusion(xs_axigluon_over_qq_stat_fat, string("Exclusion Axigluon stat"), nEBins);
 excl_stat[3] = findExclusion(xs_diquark_over_qq_stat_fat, string("Exclusion Diquark stat"), nEBins);
 excl_stat[4] = findExclusion(xs_wprime_over_qq_stat_fat, string("Exclusion W' stat"), nEBins);
 excl_stat[5] = findExclusion(xs_zprime_over_qq_stat_fat, string("Exclusion Z' stat"), nEBins);
 excl_stat[6] = findExclusion(xs_rsg_over_gg_stat_fat, string("Exclusion RSGraviton stat"), 5);
 excl_stat[7] = findExclusion(xs_s8_over_gg_stat_fat, string("Exclusion s8 stat"), nEBins);


 cout << "" << endl;
 cout << "------------------------------------------" << endl;
 cout << "Limits with systematics" << endl;
 cout << "------------------------------------------" << endl;
 cout << "" << endl;


 excl_sys[0] = findExclusion(xs_string_over_qg_fat, string("Exclusion String sys"), nEBins);
 excl_sys[1] = findExclusion(xs_qstar_over_qg_fat, string("Exclusion Qstar sys"), nEBins);
 excl_sys[2] = findExclusion(xs_axigluon_over_qq_fat, string("Exclusion Axigluon sys"), nEBins);
 excl_sys[3] = findExclusion(xs_diquark_over_qq_fat, string("Exclusion Diquark sys"), nEBins);
 excl_sys[4] = findExclusion(xs_wprime_over_qq_fat, string("Exclusion W' sys"), nEBins);
 excl_sys[5] = findExclusion(xs_zprime_over_qq_fat, string("Exclusion Z' sys"), nEBins);
 excl_sys[6] = findExclusion(xs_rsg_over_gg_fat, string("Exclusion RSGraviton"), 5);
 excl_sys[7] = findExclusion(xs_s8_over_gg_fat, string("Exclusion s8"), nEBins);


 cout << "" << endl;
 cout << "------------------------------------------" << endl;
 cout << "Expected stat only limits" << endl;
 cout << "------------------------------------------" << endl;
 cout << "" << endl;


 excl_exp_stat[0] = findExclusion(xs_string_over_qg_stat_expected_fat, string("Exclusion String stat"), nEBins);
 excl_exp_stat[1] = findExclusion(xs_qstar_over_qg_stat_expected_fat, string("Exclusion Qstar stat"), nEBins);
 excl_exp_stat[2] = findExclusion(xs_axigluon_over_qq_stat_expected_fat, string("Exclusion Axigluon stat"), nEBins);
 excl_exp_stat[3] = findExclusion(xs_diquark_over_qq_stat_expected_fat, string("Exclusion Diquark stat"), nEBins);
 excl_exp_stat[4] = findExclusion(xs_wprime_over_qq_stat_expected_fat, string("Exclusion W' stat"), nEBins);
 excl_exp_stat[5] = findExclusion(xs_zprime_over_qq_stat_expected_fat, string("Exclusion Z' stat"), nEBins);
 excl_exp_stat[6] = findExclusion(xs_rsg_over_gg_stat_fat_E_m, string("Exclusion RSGraviton"), 5);
 excl_exp_stat[7] = findExclusion(xs_s8_over_gg_stat_expected_fat, string("Exclusion s8"), nEBins);



 cout << "" << endl;
 cout << "------------------------------------------" << endl;
 cout << "Expected limits with sytematics" << endl;
 cout << "------------------------------------------" << endl;
 cout << "" << endl;


 excl_exp_sys[0] = findExclusion(xs_string_over_qg_expected_fat, string("Excl_usion String sys"), nEBins);
 excl_exp_sys[1] = findExclusion(xs_qstar_over_qg_expected_fat, string("Exclusion Qstar sys"), nEBins);
 excl_exp_sys[2] = findExclusion(xs_axigluon_over_qq_expected_fat, string("Exclusion Axigluon sys"), nEBins);
 excl_exp_sys[3] = findExclusion(xs_diquark_over_qq_expected_fat, string("Exclusion Diquark sys"), nEBins);
 excl_exp_sys[4] = findExclusion(xs_wprime_over_qq_expected_fat, string("Exclusion W' sys"), nEBins);
 excl_exp_sys[5] = findExclusion(xs_zprime_over_qq_expected_fat, string("Exclusion Z' sys"), nEBins);
 excl_exp_sys[6] = findExclusion(xs_rsg_over_gg_sys_fat_E_m, string("Exclusion RSGraviton"), 5);
 excl_exp_sys[7] = findExclusion(xs_s8_over_gg_expected_fat, string("Exclusion s8"), nEBins);

 cout << "" << endl;
 cout << "------------------------------------------" << endl;
 cout << "Stat only limits" << endl;
 cout << "Exclusion | Expected Exclusion" << endl;
 cout << "------------------------------------------" << endl;
 cout << "\\hline" << endl;
 cout << "String Resonance (S) & qg  & [1.0, " << Form("%.2f", excl_stat[0]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[0]/1000.) << "] \\\\" << endl;
 cout << "Excited Quark (q*) & qg  & [1.0, " << Form("%.2f", excl_stat[1]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[1]/1000.) << "] \\\\" << endl;
 cout << "E_{6} Diquark (D) & qq  & [1.0, " << Form("%.2f", excl_stat[3]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[3]/1000.) << "] \\\\" << endl;
 cout << "Axigluon (A)/Coloron (C) & qq  & [1.0, " << Form("%.2f", excl_stat[2]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[2]/1000.) << "] \\\\" << endl;
 cout << "s8 & gg  & [1.0, " << Form("%.2f", excl_stat[7]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[7]/1000.) << "] \\\\" << endl;
 cout << "W' (W') & qq  & [1.0, " << Form("%.2f", excl_stat[4]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[4]/1000.) << "] \\\\" << endl;
 cout << "Z' (Z') & qq  & [1.0, " << Form("%.2f", excl_stat[5]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[5]/1000.) << "] \\\\" << endl;
 cout << "RS Graviton (RSG) & qq+gg  & [1.0, " << Form("%.2f", excl_stat[6]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[6]/1000.) << "] \\\\" << endl;

 cout << "" << endl;
 cout << "------------------------------------------" << endl;
 cout << "Sys limits" << endl;
 cout << "Exclusion | Expected Exclusion" << endl;
 cout << "------------------------------------------" << endl;
 cout << "" << endl;
 cout << "\\hline" << endl;
 cout << "String Resonance (S) & qg  & [1.0, " << Form("%.2f", excl_sys[0]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[0]/1000.) << "] \\\\" << endl;
 cout << "Excited Quark (q*) & qg  & [1.0, " << Form("%.2f", excl_sys[1]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[1]/1000.) << "] \\\\" << endl;
 cout << "E6 Diquark (D) & qq  & [1.0, " << Form("%.2f", excl_sys[3]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[3]/1000.) << "] \\\\" << endl;
 cout << "Axigluon (A)/Coloron (C) & qq  & [1.0, " << Form("%.2f", excl_sys[2]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[2]/1000.) << "] \\\\" << endl;
 cout << "s8 & gg  & [1.0, " << Form("%.2f", excl_sys[7]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[7]/1000.) << "] \\\\" << endl;
 cout << "W' (W') & qq  & [1.0, " << Form("%.2f", excl_sys[4]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[4]/1000.) << "] \\\\" << endl;
 cout << "Z' (Z') & qq  & [1.0, " << Form("%.2f", excl_sys[5]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[5]/1000.) << "] \\\\" << endl;
 cout << "RS Graviton (RSG) & qq+gg  & [1.0, " << Form("%.2f", excl_sys[6]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[6]/1000.) << "] \\\\" << endl;



 cout << "" << endl;
 cout << "------------------------------------------" << endl;
 cout << "Sys limits" << endl;
 cout << "Exclusion stat only | Exclusion" << endl;
 cout << "------------------------------------------" << endl;
 cout << "" << endl;
 cout << "String Resonance (S) & qg  & [1.0, " << Form("%.2f", excl_stat[0]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[0]/1000.) << "] \\\\" << endl;
 cout << "Excited Quark (q*) & qg  & [1.0, " << Form("%.2f", excl_stat[1]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[1]/1000.) << "] \\\\" << endl;
 cout << "E_{6} Diquark (D) & qq  & [1.0, " << Form("%.2f", excl_stat[3]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[3]/1000.) << "] \\\\" << endl;
 cout << "Axigluon (A)/Coloron (C) & qq  & [1.0, " << Form("%.2f", excl_stat[2]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[2]/1000.) << "] \\\\" << endl;
 cout << "s8 & gg  & [1.0, " << Form("%.2f", excl_stat[7]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[7]/1000.) << "] \\\\" << endl;
 cout << "W' (W') & qq  & [1.0, " << Form("%.2f", excl_stat[4]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[4]/1000.) << "] \\\\" << endl;
 cout << "Z' (Z') & qq  & [1.0, " << Form("%.2f", excl_stat[5]/1000.)  << "]  & [1.13," << Form("%.2f", excl_sys[5]/1000.) << "] \\\\" << endl;
 cout << "RS Graviton (RSG) & qq+gg  & [1.0, " << Form("%.2f", excl_stat[6]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[6]/1000.) << "] \\\\" << endl;

//////////////////////////////////////////////////////
////////////////// TGraph define /////////////////////
//////////////////////////////////////////////////////

// stat only 

  g_xs_HT_fat_qg_1010 = new TGraph(nEBins,x,xs_HT_fat_qg_1010);
  g_xs_HT_fat_gg_1010 = new TGraph(nEBins,x,xs_HT_fat_gg_1010);
  g_xs_HT_fat_qq_1010 = new TGraph(nEBins,x,xs_HT_fat_qq_1010);


  g_xs_HT_fat_qg_1010_bg_FitA = new TGraph(nEBins,x,xs_HT_fat_qg_1010_bg_FitA);
  g_xs_HT_fat_gg_1010_bg_FitA = new TGraph(nEBins,x,xs_HT_fat_gg_1010_bg_FitA);
  g_xs_HT_fat_qq_1010_bg_FitA = new TGraph(nEBins,x,xs_HT_fat_qq_1010_bg_FitA);

  g_xs_HT_fat_qg_1010_bg_Fit2011 = new TGraph(nEBins,x,xs_HT_fat_qg_1010_bg_Fit2011);
  g_xs_HT_fat_gg_1010_bg_Fit2011 = new TGraph(nEBins,x,xs_HT_fat_gg_1010_bg_Fit2011);
  g_xs_HT_fat_qq_1010_bg_Fit2011 = new TGraph(nEBins,x,xs_HT_fat_qq_1010_bg_Fit2011);

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

  g_xs_comparison_bw_stat_bg_FitA_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_FitA_fat);
  g_xs_comparison_bw_stat_bg_Fit2011_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_Fit2011_fat);

  g_xs_comparison_bw_stat_bg_alternative_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative_fat);
  g_xs_comparison_bw_stat_bg_alternativeB_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternativeB_fat);
  g_xs_comparison_bw_stat_bg_alternative1Up_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative1Up_fat);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative2Up_fat);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative3Up_fat);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative1Do_fat);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative2Do_fat);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_alternative3Do_fat);



  g_xs_comparison_bw_stat_lumi_fat_norm = new TGraph(nEBins,x,xs_comparison_bw_stat_lumi_fat_norm);
  g_xs_comparison_bw_stat_JES_fat_norm = new TGraph(nEBins,x,xs_comparison_bw_stat_JES_fat_norm);
  g_xs_comparison_bw_stat_JER_fat_norm = new TGraph(nEBins,x,xs_comparison_bw_stat_JER_fat_norm);

  // theory value

  g_xs_qstar = new TGraph(en,mjj,qstar);
  g_xs_axigluon = new TGraph(en,mjj,axigluon);
  g_xs_diquark = new TGraph(en,mjj,diquark);
  g_xs_string = new TGraph(en,mjj,string);
  g_xs_zprime = new TGraph(13,mjj,zprime);
  g_xs_wprime = new TGraph(16,mjj,wprime);
  g_xs_s8 = new TGraph(en,mjj,s8);
  g_xs_rsgraviton = new TGraph(10,mjj,rsgraviton);

  // comparison b/w different Jet combinations.

  g_xs_HT_fat_qg_compare = new TGraph(nEBins,x,xs_HT_fat_qg_compare);
  g_xs_HT_fat_gg_compare = new TGraph(nEBins,x,xs_HT_fat_gg_compare);
  g_xs_HT_fat_qq_compare = new TGraph(nEBins,x,xs_HT_fat_qq_compare);
  
  g_xs_HT_fat_qg_compare_JeffereyPrior = new TGraph(nEBins,x,xs_HT_fat_qg_compare_JeffereyPrior);
  g_xs_HT_fat_gg_compare_JeffereyPrior = new TGraph(nEBins,x,xs_HT_fat_gg_compare_JeffereyPrior);
  g_xs_HT_fat_qq_compare_JeffereyPrior = new TGraph(nEBins,x,xs_HT_fat_qq_compare_JeffereyPrior);

  g_xs_HT_fat_qg_compare_nonConservativeFit = new TGraph(nEBins,x,xs_HT_fat_qg_compare_nonConservativeFit);
  g_xs_HT_fat_gg_compare_nonConservativeFit = new TGraph(nEBins,x,xs_HT_fat_gg_compare_nonConservativeFit);
  g_xs_HT_fat_qq_compare_nonConservativeFit = new TGraph(nEBins,x,xs_HT_fat_qq_compare_nonConservativeFit);

  g_xs_HT_fat_qg_compare_excludedFitRange = new TGraph(nEBins,x,xs_HT_fat_qg_compare_excludedFitRange);
  g_xs_HT_fat_gg_compare_excludedFitRange = new TGraph(nEBins,x,xs_HT_fat_gg_compare_excludedFitRange);
  g_xs_HT_fat_qq_compare_excludedFitRange = new TGraph(nEBins,x,xs_HT_fat_qq_compare_excludedFitRange);


  // comparison b/w sys.included cross section value and theory

   g_xs_qstar_over_qg_fat = new TGraph(nEBins,x,xs_qstar_over_qg_fat);
   g_xs_string_over_qg_fat = new TGraph(nEBins,x,xs_string_over_qg_fat);
   g_xs_diquark_over_qq_fat = new TGraph(nEBins,x,xs_diquark_over_qq_fat);
   g_xs_axigluon_over_qq_fat = new TGraph(nEBins,x,xs_axigluon_over_qq_fat);
   g_xs_zprime_over_qq_fat = new TGraph(nEBins,x,xs_zprime_over_qq_fat);
   g_xs_wprime_over_qq_fat = new TGraph(nEBins,x,xs_wprime_over_qq_fat);
   g_xs_s8_over_gg_fat = new TGraph(nEBins,x,xs_s8_over_gg_fat);
   g_xs_rsg_over_gg_fat = new TGraph(5,x,xs_rsg_over_gg_fat);


  // expected limit for gg
  g_xs_HT_fat_gg_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_gg_1010_sys_E_m);

  // expected limit for qg
  g_xs_HT_fat_qg_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_qg_1010_sys_E_m);

  // expected limit for qq
  g_xs_HT_fat_qq_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_qq_1010_sys_E_m);

 // expected limit for qg
  g_xs_HT_fat_qg_1010_stat_E_m = new TGraph(nEBins,x,xs_HT_fat_qg_1010_stat_E_m);

  // expected limit for qq
  g_xs_HT_fat_qq_1010_stat_E_m = new TGraph(nEBins,x,xs_HT_fat_qq_1010_stat_E_m);

  // RSG try

 
  g_xs_RSG_for_limit_fat = new TGraph(5,x_RSG,xs_RSG_for_limit_fat);
  g_xs_RSG_for_limit_stat_fat_E_m  = new TGraph(5,x_RSG,xs_RSG_for_limit_stat_fat_E_m);
  g_xs_RSG_for_limit_sys_fat_E_m  = new TGraph(5,x_RSG,xs_RSG_for_limit_sys_fat_E_m);

//////////////////////////////////////////////////////
////////////////// TGraph design /////////////////////
//////////////////////////////////////////////////////

// stat only

  g_xs_HT_fat_qg_1010->SetLineColor(2);
  g_xs_HT_fat_qg_1010->SetLineStyle(6);
  g_xs_HT_fat_qg_1010->SetLineWidth(3);
  g_xs_HT_fat_qg_1010->SetMarkerStyle(20);
  g_xs_HT_fat_qg_1010->SetMarkerColor(2);

  g_xs_HT_fat_gg_1010->SetLineColor(ci_g);
  g_xs_HT_fat_gg_1010->SetLineStyle(6);
  g_xs_HT_fat_gg_1010->SetLineWidth(3);
  g_xs_HT_fat_gg_1010->SetMarkerStyle(24);
  g_xs_HT_fat_gg_1010->SetMarkerColor(ci_g);

  g_xs_HT_fat_qq_1010->SetLineColor(4);
  g_xs_HT_fat_qq_1010->SetLineStyle(6);
  g_xs_HT_fat_qq_1010->SetLineWidth(3);
  g_xs_HT_fat_qq_1010->SetMarkerStyle(25);
  g_xs_HT_fat_qq_1010->SetMarkerColor(4);


  g_xs_HT_fat_qg_1010_bg_FitA->SetLineColor(kMagenta);
  g_xs_HT_fat_qg_1010_bg_FitA->SetLineStyle(2);
  g_xs_HT_fat_qg_1010_bg_FitA->SetLineWidth(3);
  g_xs_HT_fat_qg_1010_bg_FitA->SetMarkerStyle(26);
  g_xs_HT_fat_qg_1010_bg_FitA->SetMarkerColor(kMagenta);

  g_xs_HT_fat_gg_1010_bg_FitA->SetLineColor(kGreen);
  g_xs_HT_fat_gg_1010_bg_FitA->SetLineStyle(2);
  g_xs_HT_fat_gg_1010_bg_FitA->SetLineWidth(3);
  g_xs_HT_fat_gg_1010_bg_FitA->SetMarkerStyle(27);
  g_xs_HT_fat_gg_1010_bg_FitA->SetMarkerColor(kGreen);

  g_xs_HT_fat_qq_1010_bg_FitA->SetLineColor(kBlue);
  g_xs_HT_fat_qq_1010_bg_FitA->SetLineStyle(2);
  g_xs_HT_fat_qq_1010_bg_FitA->SetLineWidth(3);
  g_xs_HT_fat_qq_1010_bg_FitA->SetMarkerStyle(28);
  g_xs_HT_fat_qq_1010_bg_FitA->SetMarkerColor(kBlue);


  g_xs_HT_fat_qg_1010_bg_Fit2011->SetLineColor(kMagenta);
  g_xs_HT_fat_qg_1010_bg_Fit2011->SetLineStyle(2);
  g_xs_HT_fat_qg_1010_bg_Fit2011->SetLineWidth(3);
  g_xs_HT_fat_qg_1010_bg_Fit2011->SetMarkerStyle(29);
  g_xs_HT_fat_qg_1010_bg_Fit2011->SetMarkerColor(kMagenta);

  g_xs_HT_fat_gg_1010_bg_Fit2011->SetLineColor(kGreen);
  g_xs_HT_fat_gg_1010_bg_Fit2011->SetLineStyle(2);
  g_xs_HT_fat_gg_1010_bg_Fit2011->SetLineWidth(3);
  g_xs_HT_fat_gg_1010_bg_Fit2011->SetMarkerStyle(30);
  g_xs_HT_fat_gg_1010_bg_Fit2011->SetMarkerColor(kGreen);

  g_xs_HT_fat_qq_1010_bg_Fit2011->SetLineColor(kBlue);
  g_xs_HT_fat_qq_1010_bg_Fit2011->SetLineStyle(2);
  g_xs_HT_fat_qq_1010_bg_Fit2011->SetLineWidth(3);
  g_xs_HT_fat_qq_1010_bg_Fit2011->SetMarkerStyle(31);
  g_xs_HT_fat_qq_1010_bg_Fit2011->SetMarkerColor(kBlue);



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






  // Expected HT-gg sys

  g_xs_HT_fat_gg_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_gg_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_gg_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_gg_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_gg_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_gg_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_gg_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_gg_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_gg_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_gg_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_gg_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_gg_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_gg_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_gg_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_gg_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_gg_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_gg_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_gg_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_gg_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_gg_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_gg_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_gg_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_gg_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_gg_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_gg_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_gg_1010_sys_E_u2->SetFillStyle(21);




  // Expected HT-qg sys

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

  // Expected HT-qq sys

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






  







 // Expected HT-qg stat

  g_xs_HT_fat_qg_1010_stat_E_m->SetLineColor(1);
  g_xs_HT_fat_qg_1010_stat_E_m->SetLineStyle(10);
  g_xs_HT_fat_qg_1010_stat_E_m->SetLineWidth(3);
  g_xs_HT_fat_qg_1010_stat_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_qg_1010_stat_E_m->SetMarkerColor(1);

  h_xs_HT_fat_qg_1010_stat_E_d2->SetLineColor(0);
  h_xs_HT_fat_qg_1010_stat_E_d2->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_d2->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_stat_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_qg_1010_stat_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_qg_1010_stat_E_d2->SetFillStyle(21);

  h_xs_HT_fat_qg_1010_stat_E_d1->SetLineColor(41);
  h_xs_HT_fat_qg_1010_stat_E_d1->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_d1->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_stat_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_qg_1010_stat_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_qg_1010_stat_E_d1->SetFillStyle(21);

  h_xs_HT_fat_qg_1010_stat_E_u1->SetLineColor(5);
  h_xs_HT_fat_qg_1010_stat_E_u1->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_u1->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_stat_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_qg_1010_stat_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_qg_1010_stat_E_u1->SetFillStyle(21);

  h_xs_HT_fat_qg_1010_stat_E_u2->SetLineColor(41);
  h_xs_HT_fat_qg_1010_stat_E_u2->SetLineStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_u2->SetLineWidth(3);
  h_xs_HT_fat_qg_1010_stat_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_qg_1010_stat_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_qg_1010_stat_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_qg_1010_stat_E_u2->SetFillStyle(21);

  // Expected HT-qq stat

  g_xs_HT_fat_qq_1010_stat_E_m->SetLineColor(1);
  g_xs_HT_fat_qq_1010_stat_E_m->SetLineStyle(10);
  g_xs_HT_fat_qq_1010_stat_E_m->SetLineWidth(3);
  g_xs_HT_fat_qq_1010_stat_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_qq_1010_stat_E_m->SetMarkerColor(1);

  h_xs_HT_fat_qq_1010_stat_E_d2->SetLineColor(0);
  h_xs_HT_fat_qq_1010_stat_E_d2->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_d2->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_stat_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_qq_1010_stat_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_qq_1010_stat_E_d2->SetFillStyle(21);

  h_xs_HT_fat_qq_1010_stat_E_d1->SetLineColor(41);
  h_xs_HT_fat_qq_1010_stat_E_d1->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_d1->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_stat_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_qq_1010_stat_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_qq_1010_stat_E_d1->SetFillStyle(21);

  h_xs_HT_fat_qq_1010_stat_E_u1->SetLineColor(5);
  h_xs_HT_fat_qq_1010_stat_E_u1->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_u1->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_stat_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_qq_1010_stat_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_qq_1010_stat_E_u1->SetFillStyle(21);

  h_xs_HT_fat_qq_1010_stat_E_u2->SetLineColor(41);
  h_xs_HT_fat_qq_1010_stat_E_u2->SetLineStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_u2->SetLineWidth(3);
  h_xs_HT_fat_qq_1010_stat_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_qq_1010_stat_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_qq_1010_stat_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_qq_1010_stat_E_u2->SetFillStyle(21);


















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

  g_xs_comparison_bw_stat_bg_FitA_fat->SetLineColor(46);
  g_xs_comparison_bw_stat_bg_FitA_fat->SetLineStyle(2);
  g_xs_comparison_bw_stat_bg_FitA_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_FitA_fat->SetMarkerStyle(30);
  g_xs_comparison_bw_stat_bg_FitA_fat->SetMarkerColor(46);

  g_xs_comparison_bw_stat_bg_Fit2011_fat->SetLineColor(kBlue);
  g_xs_comparison_bw_stat_bg_Fit2011_fat->SetLineStyle(3);
  g_xs_comparison_bw_stat_bg_Fit2011_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_Fit2011_fat->SetMarkerStyle(28);
  g_xs_comparison_bw_stat_bg_Fit2011_fat->SetMarkerColor(kBlue);


  g_xs_comparison_bw_stat_bg_alternative_fat->SetLineColor(kMagenta);
  g_xs_comparison_bw_stat_bg_alternative_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_bg_alternative_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative_fat->SetMarkerStyle(27);
  g_xs_comparison_bw_stat_bg_alternative_fat->SetMarkerColor(kMagenta);
	
  g_xs_comparison_bw_stat_bg_alternativeB_fat->SetLineColor(46);
  g_xs_comparison_bw_stat_bg_alternativeB_fat->SetLineStyle(4);
  g_xs_comparison_bw_stat_bg_alternativeB_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternativeB_fat->SetMarkerStyle(28);
  g_xs_comparison_bw_stat_bg_alternativeB_fat->SetMarkerColor(46);

  g_xs_comparison_bw_stat_bg_alternative1Up_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative1Up_fat->SetLineStyle(2);
  g_xs_comparison_bw_stat_bg_alternative1Up_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative1Up_fat->SetMarkerStyle(29);
  g_xs_comparison_bw_stat_bg_alternative1Up_fat->SetMarkerColor(kGreen);

  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetLineStyle(3);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetMarkerStyle(30);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetMarkerColor(kGreen);

  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetLineStyle(4);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetMarkerStyle(31);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetMarkerColor(kGreen);

  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetLineStyle(5);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetMarkerStyle(29);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetMarkerColor(kGreen);

  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetLineStyle(6);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetMarkerStyle(30);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetMarkerColor(kGreen);

  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetLineStyle(7);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetMarkerStyle(31);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetMarkerColor(kGreen);


  g_xs_comparison_bw_stat_lumi_fat_norm->SetLineColor(ci_g);
  g_xs_comparison_bw_stat_lumi_fat_norm->SetLineStyle(2);
  g_xs_comparison_bw_stat_lumi_fat_norm->SetLineWidth(3);
  g_xs_comparison_bw_stat_lumi_fat_norm->SetMarkerStyle(25);
  g_xs_comparison_bw_stat_lumi_fat_norm->SetMarkerColor(ci_g);

  g_xs_comparison_bw_stat_JES_fat_norm->SetLineColor(4);
  g_xs_comparison_bw_stat_JES_fat_norm->SetLineStyle(2);
  g_xs_comparison_bw_stat_JES_fat_norm->SetLineWidth(3);
  g_xs_comparison_bw_stat_JES_fat_norm->SetMarkerStyle(26);
  g_xs_comparison_bw_stat_JES_fat_norm->SetMarkerColor(4);

  g_xs_comparison_bw_stat_JER_fat_norm->SetLineColor(46);
  g_xs_comparison_bw_stat_JER_fat_norm->SetLineStyle(2);
  g_xs_comparison_bw_stat_JER_fat_norm->SetLineWidth(3);
  g_xs_comparison_bw_stat_JER_fat_norm->SetMarkerStyle(32);
  g_xs_comparison_bw_stat_JER_fat_norm->SetMarkerColor(46);

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


  g_xs_HT_fat_qg_compare_JeffereyPrior->SetLineColor(1);
  g_xs_HT_fat_qg_compare_JeffereyPrior->SetLineStyle(1);
  g_xs_HT_fat_qg_compare_JeffereyPrior->SetLineWidth(3);
  g_xs_HT_fat_qg_compare_JeffereyPrior->SetMarkerStyle(30);
  g_xs_HT_fat_qg_compare_JeffereyPrior->SetMarkerColor(1);

  g_xs_HT_fat_gg_compare_JeffereyPrior->SetLineColor(2);
  g_xs_HT_fat_gg_compare_JeffereyPrior->SetLineStyle(1);
  g_xs_HT_fat_gg_compare_JeffereyPrior->SetLineWidth(3);
  g_xs_HT_fat_gg_compare_JeffereyPrior->SetMarkerStyle(30);
  g_xs_HT_fat_gg_compare_JeffereyPrior->SetMarkerColor(2);

  g_xs_HT_fat_qq_compare_JeffereyPrior->SetLineColor(3);
  g_xs_HT_fat_qq_compare_JeffereyPrior->SetLineStyle(1);
  g_xs_HT_fat_qq_compare_JeffereyPrior->SetLineWidth(3);
  g_xs_HT_fat_qq_compare_JeffereyPrior->SetMarkerStyle(30);
  g_xs_HT_fat_qq_compare_JeffereyPrior->SetMarkerColor(3);



  g_xs_HT_fat_qg_compare_nonConservativeFit->SetLineColor(1);
  g_xs_HT_fat_qg_compare_nonConservativeFit->SetLineStyle(1);
  g_xs_HT_fat_qg_compare_nonConservativeFit->SetLineWidth(3);
  g_xs_HT_fat_qg_compare_nonConservativeFit->SetMarkerStyle(24);
  g_xs_HT_fat_qg_compare_nonConservativeFit->SetMarkerColor(1);

  g_xs_HT_fat_gg_compare_nonConservativeFit->SetLineColor(2);
  g_xs_HT_fat_gg_compare_nonConservativeFit->SetLineStyle(1);
  g_xs_HT_fat_gg_compare_nonConservativeFit->SetLineWidth(3);
  g_xs_HT_fat_gg_compare_nonConservativeFit->SetMarkerStyle(24);
  g_xs_HT_fat_gg_compare_nonConservativeFit->SetMarkerColor(2);

  g_xs_HT_fat_qq_compare_nonConservativeFit->SetLineColor(3);
  g_xs_HT_fat_qq_compare_nonConservativeFit->SetLineStyle(1);
  g_xs_HT_fat_qq_compare_nonConservativeFit->SetLineWidth(3);
  g_xs_HT_fat_qq_compare_nonConservativeFit->SetMarkerStyle(24);
  g_xs_HT_fat_qq_compare_nonConservativeFit->SetMarkerColor(3);


  g_xs_HT_fat_qg_compare_excludedFitRange->SetLineColor(1);
  g_xs_HT_fat_qg_compare_excludedFitRange->SetLineStyle(2);
  g_xs_HT_fat_qg_compare_excludedFitRange->SetLineWidth(3);
  g_xs_HT_fat_qg_compare_excludedFitRange->SetMarkerStyle(25);
  g_xs_HT_fat_qg_compare_excludedFitRange->SetMarkerColor(1);

  g_xs_HT_fat_gg_compare_excludedFitRange->SetLineColor(2);
  g_xs_HT_fat_gg_compare_excludedFitRange->SetLineStyle(2);
  g_xs_HT_fat_gg_compare_excludedFitRange->SetLineWidth(3);
  g_xs_HT_fat_gg_compare_excludedFitRange->SetMarkerStyle(25);
  g_xs_HT_fat_gg_compare_excludedFitRange->SetMarkerColor(2);

  g_xs_HT_fat_qq_compare_excludedFitRange->SetLineColor(3);
  g_xs_HT_fat_qq_compare_excludedFitRange->SetLineStyle(2);
  g_xs_HT_fat_qq_compare_excludedFitRange->SetLineWidth(3);
  g_xs_HT_fat_qq_compare_excludedFitRange->SetMarkerStyle(25);
  g_xs_HT_fat_qq_compare_excludedFitRange->SetMarkerColor(3);






  g_xs_RSG_for_limit_fat->SetLineWidth(3);
  g_xs_RSG_for_limit_fat->SetMarkerStyle(26);
  g_xs_RSG_for_limit_fat->SetLineColor(kRed);
  g_xs_RSG_for_limit_fat->SetMarkerColor(kRed);

  g_xs_RSG_for_limit_sys_fat_E_m->SetLineColor(1);
  g_xs_RSG_for_limit_sys_fat_E_m->SetLineStyle(10);
  g_xs_RSG_for_limit_sys_fat_E_m->SetLineWidth(3);
  g_xs_RSG_for_limit_sys_fat_E_m->SetMarkerStyle(1);
  g_xs_RSG_for_limit_sys_fat_E_m->SetMarkerColor(1);

  h_xs_RSG_for_limit_sys_fat_E_d2->SetLineColor(0);
  h_xs_RSG_for_limit_sys_fat_E_d2->SetLineStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_d2->SetLineWidth(3);
  h_xs_RSG_for_limit_sys_fat_E_d2->SetMarkerStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_d2->SetMarkerColor(41);
  h_xs_RSG_for_limit_sys_fat_E_d2->SetFillColor(10);
  //  h_xs_RSG_for_limit_sys_fat_E_d2->SetFillStyle(21);

  h_xs_RSG_for_limit_sys_fat_E_d1->SetLineColor(41);
  h_xs_RSG_for_limit_sys_fat_E_d1->SetLineStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_d1->SetLineWidth(3);
  h_xs_RSG_for_limit_sys_fat_E_d1->SetMarkerStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_d1->SetMarkerColor(5);
  h_xs_RSG_for_limit_sys_fat_E_d1->SetFillColor(41);
  //  h_xs_RSG_for_limit_sys_fat_E_d1->SetFillStyle(21);

  h_xs_RSG_for_limit_sys_fat_E_u1->SetLineColor(5);
  h_xs_RSG_for_limit_sys_fat_E_u1->SetLineStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_u1->SetLineWidth(3);
  h_xs_RSG_for_limit_sys_fat_E_u1->SetMarkerStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_u1->SetMarkerColor(5);
  h_xs_RSG_for_limit_sys_fat_E_u1->SetFillColor(5);
  //  h_xs_RSG_for_limit_sys_fat_E_u1->SetFillStyle(21);

  h_xs_RSG_for_limit_sys_fat_E_u2->SetLineColor(41);
  h_xs_RSG_for_limit_sys_fat_E_u2->SetLineStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_u2->SetLineWidth(3);
  h_xs_RSG_for_limit_sys_fat_E_u2->SetMarkerStyle(1);
  h_xs_RSG_for_limit_sys_fat_E_u2->SetMarkerColor(41);
  h_xs_RSG_for_limit_sys_fat_E_u2->SetFillColor(41);



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

  g_xs_s8->SetLineColor(kMagenta);
  g_xs_s8->SetLineStyle(8);
  g_xs_s8->SetLineWidth(3);

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

  g_xs_s8_over_gg_fat->SetLineColor(kMagenta);
  g_xs_s8_over_gg_fat->SetLineWidth(3);

  g_xs_rsg_over_gg_fat->SetLineColor(29);
  g_xs_rsg_over_gg_fat->SetLineWidth(3);


///////////////// drawing canvas /////////////////////
//////////////////////////////////////////////////////


  // c_xs_qg

  c_xs_qg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);

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
  g_xs_s8->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  


  
  l_xs_qg->SetFillColor(0);
  l_xs_qg->SetFillStyle(0);
  l_xs_qg->SetHeader("qg-(Sys)-4.0 fb^{-1}");
  //  l_xs_qg->AddEntry(g_xs_HT_calo_qg_1010,"HT-calo 95% CL","pL");
  //  l_xs_qg->AddEntry(g_xs_HT_pf_qg_1010,"HT-PF 95% CL","pL");
  l_xs_qg->AddEntry(g_xs_HT_fat_qg_1010,"HT-fat 95% CL","pL");

  l_xs_theory->SetTextSize(0.03);
  l_xs_theory->SetFillColor(0);
  l_xs_theory->SetFillStyle(0);
  l_xs_theory->AddEntry(g_xs_string,"string","L");
  l_xs_theory->AddEntry(g_xs_qstar,"Excited Quark ","L");
  l_xs_theory->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  //  l_xs_theory->AddEntry(g_xs_axigluon,"(NLO) arXiv:1111.7261v2","");
  l_xs_theory->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory->AddEntry(g_xs_s8,"s8","L");
  l_xs_theory->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory->AddEntry(g_xs_zprime,"Z'","L");
  l_xs_theory->AddEntry(g_xs_rsgraviton,"RS Graviton","L");


  l_xs_qg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qg->Update();
  c_xs_qg->SaveAs("Fit_Results_2012/Exclusion_qg.png");
  c_xs_qg->SaveAs("Fit_Results_2012/Exclusion_qg.eps");

  // c_xs_gg
  
  c_xs_gg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
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
  g_xs_s8->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_gg->SetFillColor(0);
  l_xs_gg->SetFillStyle(0);
  l_xs_gg->SetHeader("gg-(Sys)-4.0 fb^{-1}");
  //  l_xs_gg->AddEntry(g_xs_HT_calo_gg_1010,"HT-calo 95% CL","pL");
  //  l_xs_gg->AddEntry(g_xs_HT_pf_gg_1010,"HT-PF 95% CL","pL");
  l_xs_gg->AddEntry(g_xs_HT_fat_gg_1010,"HT-fat 95% CL","pL");

  l_xs_gg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_gg->Update();
  c_xs_gg->SaveAs("Fit_Results_2012/Exclusion_gg.png");
  c_xs_gg->SaveAs("Fit_Results_2012/Exclusion_gg.eps");
  // c_xs_qq

  c_xs_qq->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
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
  g_xs_s8->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qq->SetFillColor(0);
  l_xs_qq->SetFillStyle(0);
  l_xs_qq->SetHeader("qq-(Sys)-4.0 fb^{-1}");
  //  l_xs_qq->AddEntry(g_xs_HT_calo_qq_1010,"HT-calo 95% CL","pL");
  //  l_xs_qq->AddEntry(g_xs_HT_pf_qq_1010,"HT-PF 95% CL","pL");
  l_xs_qq->AddEntry(g_xs_HT_fat_qq_1010,"HT-fat 95% CL","pL");
  l_xs_qq->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qq->Update();
  c_xs_qq->SaveAs("Fit_Results_2012/Exclusion_qq.png");
  c_xs_qq->SaveAs("Fit_Results_2012/Exclusion_qq.eps");
  
  // c_xs_stat_fat

  c_xs_stat_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
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
  g_xs_s8->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_stat_fat->SetFillColor(0);
  l_xs_stat_fat->SetFillStyle(0);
  //  l_xs_stat_fat->SetHeader("CMS Preliminary");
  //  l_xs_stat_fat->AddEntry((TObject*)0,"4.0 fb^{-1}","");
  l_xs_stat_fat->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qg_1010,"Quark-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qq_1010,"Quark-Quark","pL");
  l_xs_stat_fat->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_fat->AddText("CMS (4.0 fb^{-1})"); 
  p_xs_stat_fat->AddText(" #sqrt{s} = 8 TeV");
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
 
  c_xs_stat_fat->SaveAs("Fit_Results_2012/Exclusion_StatOnly.png");
  c_xs_stat_fat->SaveAs("Fit_Results_2012/Exclusion_StatOnly.eps");

  // c_xs_sys

  c_xs_sys->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
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
  g_xs_s8->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_sys->SetFillColor(0);
  l_xs_sys->SetFillStyle(0);
  l_xs_sys->SetTextSize(0.028);
  //  l_xs_sys->SetHeader("CMS Preliminary");
  //  l_xs_sys->AddEntry((TObject*)0,"4.0 fb^{-1}","");
  l_xs_sys->SetHeader("95% CL Upper Limit");
  l_xs_sys->AddEntry(g_xs_HT_fat_gg_1010_sys,"Gluon-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qg_1010_sys,"Quark-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qq_1010_sys,"Quark-Quark","pL");
  l_xs_sys->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_sys->AddText("CMS Preliminary (4.0 fb^{-1})"); 
  p_xs_sys->AddText("#sqrt{s} = 8 TeV");
  p_xs_sys->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  p_xs_sys->SetFillColor(0);
  p_xs_sys->SetLineColor(0);
  p_xs_sys->SetFillStyle(0);
  p_xs_sys->SetBorderSize(0);
  p_xs_sys->SetTextFont(62);
  p_xs_sys->SetTextSize(0.028);
  p_xs_sys->SetTextAlign(12);
  p_xs_sys->Draw("sames");
  c_xs_sys->Update();

  c_xs_sys->SaveAs("Fit_Results_2012/Exclusion_Sys.png");
  c_xs_sys->SaveAs("Fit_Results_2012/Exclusion_Sys.eps");

  // c_xs_all_fat

  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
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
  l_xs_all_fat->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"4.0 fb^{-1}","")
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
  c_xs_all_fat->SaveAs("Fit_Results_2012/Exclusion_Stat_vs_Sys.png");
  c_xs_all_fat->SaveAs("Fit_Results_2012/Exclusion_Stat_vs_Sys.eps");





 // c_xs_all_fat

  c_xs_all_fat->Clear();
  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
  // vFrame->SetLogy();

  g_xs_HT_fat_gg_1010_sys->Draw("PL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");
  g_xs_HT_fat_qg_1010_sys->Draw("samePL");
  g_xs_HT_fat_gg_1010_bg_FitA->Draw("samePL");
  g_xs_HT_fat_qq_1010_bg_FitA->Draw("samePL");
  g_xs_HT_fat_qg_1010_bg_FitA->Draw("samePL");
  
  g_xs_string->Draw("PL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
  
  l_xs_all_fat->Clear();
  l_xs_all_fat->SetFillColor(0);
  l_xs_all_fat->SetFillStyle(0);
  l_xs_all_fat->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010,"gg 95% CL with sys.","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qg_1010,"qg 95% CL with sys.","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qq_1010,"qq 95% CL with sys.","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010_bg_FitA,"gg 95% CL (Fit A with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qg_1010_bg_FitA,"qg 95% CL (Fit A with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qq_1010_bg_FitA,"qq 95% CL (Fit A with sys.)","pL");
  l_xs_all_fat->Draw("sames");
  
  l_xs_theory->Draw("sames");
  
  c_xs_all_fat->Update();
  //  c_xs_all_fat->SaveAs("Fit_Results_2012/Exclusion_StatDefault_vs_FitA_withSys.png");
  //  c_xs_all_fat->SaveAs("Fit_Results_2012/Exclusion_StatDefault_vs_FitA_withSys.eps");







 // c_xs_all_fat

  c_xs_all_fat->Clear();
  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(xMin);
  vFrame->SetMaximum(xMax);
  // vFrame->SetLogy();

  g_xs_HT_fat_gg_1010_sys->Draw("PL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");
  g_xs_HT_fat_qg_1010_sys->Draw("samePL");
  g_xs_HT_fat_gg_1010_bg_Fit2011->Draw("samePL");
  g_xs_HT_fat_qq_1010_bg_Fit2011->Draw("samePL");
  g_xs_HT_fat_qg_1010_bg_Fit2011->Draw("samePL");
  
  g_xs_string->Draw("PL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
  

  l_xs_all_fat->Clear();
  l_xs_all_fat->SetFillColor(0);
  l_xs_all_fat->SetFillStyle(0);
  l_xs_all_fat->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010,"gg 95% CL (2012 with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qg_1010,"qg 95% CL (2012 with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qq_1010,"qq 95% CL (2012 with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_gg_1010_bg_Fit2011,"gg 95% CL (2011 with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qg_1010_bg_Fit2011,"qg 95% CL (2011 with sys.)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_qq_1010_bg_Fit2011,"qq 95% CL (2011 with sys.)","pL");
  l_xs_all_fat->Draw("sames");
  
  l_xs_theory->Draw("sames");
  
  c_xs_all_fat->Update();
  c_xs_all_fat->SaveAs("Fit_Results_2012/Exclusion_2012_vs_2011_withSys.png");
  c_xs_all_fat->SaveAs("Fit_Results_2012/Exclusion_2012_vs_2011_withSys.eps");














  // c_xs_comparison_bw_sys_theory_fat

  c_xs_comparison_bw_sys_theory_fat->cd(0);

  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,10.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(theory #sigma) / (sys.included 95% CL #sigma)");
  vFrame->SetMinimum(0.01);
  vFrame->SetMaximum(10.0);
  // vFrame->SetLogy();

  g_xs_string_over_qg_fat->Draw("L");  
  g_xs_qstar_over_qg_fat->Draw("sameL");
  g_xs_axigluon_over_qq_fat->Draw("sameL");
  g_xs_diquark_over_qq_fat->Draw("sameL");
  g_xs_wprime_over_qq_fat->Draw("sameL");  
  g_xs_zprime_over_qq_fat->Draw("sameL");  
  g_xs_s8_over_gg_fat->Draw("sameL");  
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

  l_xs_comparison_bw_sys_theory_fat->SetTextSize(0.03);
  l_xs_comparison_bw_sys_theory_fat->SetFillColor(0);
  l_xs_comparison_bw_sys_theory_fat->SetFillStyle(0);
  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS Preliminary (4.0 fb^{-1})");
  //  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_fat->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_string_over_qg_fat,"String","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_qstar_over_qg_fat,"Excited Quark","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_axigluon_over_qq_fat,"Axigluon/Coloron","L");
  //  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_axigluon_over_qq_fat,"(NLO) arXiv:1111.7261v2","");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_diquark_over_qq_fat,"E_{6} Diquark","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_s8_over_gg_fat,"s8","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_wprime_over_qq_fat,"W'","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_zprime_over_qq_fat,"Z'","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_rsg_over_gg_fat,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_fat->Draw("sames");
  c_xs_comparison_bw_sys_theory_fat->Update();

  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results_2012/Exclusions_vs_Theory.png");
  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results_2012/Exclusions_vs_Theory.eps");
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
  h_xs_qg_sys_E->SetMinimum(xMin);
  h_xs_qg_sys_E->SetMaximum(xMax);

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
  p_xs_qg_sys_E->AddText("CMS Preliminary (4.0 fb^{-1})");
  p_xs_qg_sys_E->AddText("#sqrt{s} = 8 TeV");
  p_xs_qg_sys_E->SetTextSize(0.028);
  p_xs_qg_sys_E->SetTextAlign(12);
  p_xs_qg_sys_E->Draw("sames");
	
  l_xs_qg_sys_E->SetFillColor(0);
  l_xs_qg_sys_E->SetFillStyle(0);
//  l_xs_qg_sys_E->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_qg_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qg_sys_E->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_qg_sys_E->AddEntry(g_xs_HT_fat_qg_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qg_sys_E->AddEntry(g_xs_HT_fat_qg_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qg_sys_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qg_sys_E->Draw("sames");
  l_xs_theory_qg->SetFillColor(0);
  l_xs_theory_qg->SetFillStyle(0);
  l_xs_theory_qg->SetTextSize(0.035);
  l_xs_theory_qg->AddEntry(g_xs_string,"String","L");
  l_xs_theory_qg->AddEntry(g_xs_qstar,"Excited Quark ","L");
  l_xs_theory_qg->Draw("sames");

  c_xs_qg_sys_E->Update();

  c_xs_qg_sys_E->SaveAs("Fit_Results_2012/Exclusions_qg_withExpected.png");
  c_xs_qg_sys_E->SaveAs("Fit_Results_2012/Exclusions_qg_withExpected.eps");




  THStack *h_xs_gg_sys_E = new THStack("h_xs_gg_sys_E","h_xs_gg_sys_E");
  TH1F *htemp_xs_gg_sys_E_u2 = (TH1F*)h_xs_HT_fat_gg_1010_sys_E_u2->Clone("htemp_xs_gg_sys_E_u2");
  TH1F *htemp_xs_gg_sys_E_u1 = (TH1F*)h_xs_HT_fat_gg_1010_sys_E_u1->Clone("htemp_xs_gg_sys_E_u1");  
  TH1F *htemp_xs_gg_sys_E_d1 = (TH1F*)h_xs_HT_fat_gg_1010_sys_E_d1->Clone("htemp_xs_gg_sys_E_d1");   
  htemp_xs_gg_sys_E_u2->Add(h_xs_HT_fat_gg_1010_sys_E_u1,-1);
  htemp_xs_gg_sys_E_u1->Add(h_xs_HT_fat_gg_1010_sys_E_d1,-1);
  htemp_xs_gg_sys_E_d1->Add(h_xs_HT_fat_gg_1010_sys_E_d2,-1);

  h_xs_gg_sys_E->Add(h_xs_HT_fat_gg_1010_sys_E_d2);
  h_xs_gg_sys_E->Add(htemp_xs_gg_sys_E_d1);
  h_xs_gg_sys_E->Add(htemp_xs_gg_sys_E_u1);
  h_xs_gg_sys_E->Add(htemp_xs_gg_sys_E_u2);
  h_xs_gg_sys_E->SetMinimum(xMin);
  h_xs_gg_sys_E->SetMaximum(xMax);

  c_xs_gg_sys_E->cd(0);

  h_xs_HT_fat_gg_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_gg_sys_E->SetTitle("");
  h_xs_HT_fat_gg_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_gg_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_gg_sys_E->Draw("C");

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

  h_xs_gg_sys_E->GetXaxis()->SetTitle("gg Resonance Mass (GeV)");
  h_xs_gg_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  gPad->SetLogy();


  g_xs_HT_fat_gg_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_gg_1010_sys->Draw("samePL");

  g_xs_s8->Draw("samePL");  


  p_xs_gg_sys_E->SetFillColor(0);
  p_xs_gg_sys_E->SetFillStyle(0);
  p_xs_gg_sys_E->SetBorderSize(0);
  p_xs_gg_sys_E->AddText("CMS Preliminary (4.0 fb^{-1})");
  p_xs_gg_sys_E->AddText("#sqrt{s} = 8 TeV");
  p_xs_gg_sys_E->SetTextSize(0.028);
  p_xs_gg_sys_E->SetTextAlign(12);
  p_xs_gg_sys_E->Draw("sames");
	
  l_xs_gg_sys_E->SetFillColor(0);
  l_xs_gg_sys_E->SetFillStyle(0);
//  l_xs_gg_sys_E->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_gg_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_gg_sys_E->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_gg_sys_E->AddEntry(g_xs_HT_fat_gg_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_gg_sys_E->AddEntry(h_xs_HT_fat_gg_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_gg_sys_E->AddEntry(h_xs_HT_fat_gg_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_gg_sys_E->AddEntry(g_xs_HT_fat_gg_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_gg_sys_E->AddEntry(h_xs_HT_fat_gg_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_gg_sys_E->AddEntry(h_xs_HT_fat_gg_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_gg_sys_E->Draw("sames");
  l_xs_theory_gg->SetFillColor(0);
  l_xs_theory_gg->SetFillStyle(0);
  l_xs_theory_gg->SetTextSize(0.035);
  l_xs_theory_gg->AddEntry(g_xs_s8,"s8 Resonance","L");
  l_xs_theory_gg->Draw("sames");

  c_xs_gg_sys_E->Update();

  c_xs_gg_sys_E->SaveAs("Fit_Results_2012/Exclusions_gg_withExpected.png");
  c_xs_gg_sys_E->SaveAs("Fit_Results_2012/Exclusions_gg_withExpected.eps");
























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

  h_xs_qq_sys_E->SetMinimum(xMin);
  h_xs_qq_sys_E->SetMaximum(xMax);

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
  p_xs_qq_sys_E->AddText("CMS Preliminary (4.0 fb^{-1})");
  p_xs_qq_sys_E->AddText("#sqrt{s} = 8 TeV");
  p_xs_qq_sys_E->SetTextSize(0.028);
  p_xs_qq_sys_E->SetTextAlign(12);
  p_xs_qq_sys_E->Draw("sames");

  l_xs_qq_sys_E->SetFillColor(0);
  l_xs_qq_sys_E->SetFillStyle(0);
  //  l_xs_qq_sys_E->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_qq_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_sys_E->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_qq_sys_E->AddEntry(g_xs_HT_fat_qq_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qq_sys_E->AddEntry(g_xs_HT_fat_qq_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qq_sys_E->AddEntry(h_xs_HT_fat_qq_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qq_sys_E->Draw("sames");
  l_xs_theory_qq->SetFillColor(0);
  l_xs_theory_qq->SetTextSize(0.035);
  l_xs_theory_qq->SetFillStyle(0);
  l_xs_theory_qq->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  //  l_xs_theory_qq->AddEntry(g_xs_axigluon,"(NLO) arXiv:1111.7261v2","");
  l_xs_theory_qq->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory_qq->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory_qq->AddEntry(g_xs_zprime,"Z'","L");
  //  l_xs_theory_qq->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_qq->Draw("sames");
  
  c_xs_qq_sys_E->Update();

  c_xs_qq_sys_E->SaveAs("Fit_Results_2012/Exclusions_qq_withExpected.png");
  c_xs_qq_sys_E->SaveAs("Fit_Results_2012/Exclusions_qq_withExpected.eps");

  // c_xs_comparison_bw_stat_sys_fat









  THStack *h_xs_qg_stat_E = new THStack("h_xs_qg_stat_E","h_xs_qg_stat_E");
  TH1F *htemp_xs_qg_stat_E_u2 = (TH1F*)h_xs_HT_fat_qg_1010_stat_E_u2->Clone("htemp_xs_qg_stat_E_u2");
  TH1F *htemp_xs_qg_stat_E_u1 = (TH1F*)h_xs_HT_fat_qg_1010_stat_E_u1->Clone("htemp_xs_qg_stat_E_u1");  
  TH1F *htemp_xs_qg_stat_E_d1 = (TH1F*)h_xs_HT_fat_qg_1010_stat_E_d1->Clone("htemp_xs_qg_stat_E_d1");   
  htemp_xs_qg_stat_E_u2->Add(h_xs_HT_fat_qg_1010_stat_E_u1,-1);
  htemp_xs_qg_stat_E_u1->Add(h_xs_HT_fat_qg_1010_stat_E_d1,-1);
  htemp_xs_qg_stat_E_d1->Add(h_xs_HT_fat_qg_1010_stat_E_d2,-1);

  h_xs_qg_stat_E->Add(h_xs_HT_fat_qg_1010_stat_E_d2);
  h_xs_qg_stat_E->Add(htemp_xs_qg_stat_E_d1);
  h_xs_qg_stat_E->Add(htemp_xs_qg_stat_E_u1);
  h_xs_qg_stat_E->Add(htemp_xs_qg_stat_E_u2);
  h_xs_qg_stat_E->SetMinimum(xMin);
  h_xs_qg_stat_E->SetMaximum(xMax);

  c_xs_qg_stat_E->cd(0);

  h_xs_HT_fat_qg_1010_stat_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qg_stat_E->SetTitle("");
  h_xs_HT_fat_qg_1010_stat_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qg_1010_stat_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qg_stat_E->Draw("C");

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

  h_xs_qg_stat_E->GetXaxis()->SetTitle("qg Resonance Mass (GeV)");
  h_xs_qg_stat_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  //  h_xs_HT_fat_qg_1010_stat_E_u1->Draw("sameC");
  //  h_xs_HT_fat_qg_1010_stat_E_d1->Draw("sameC");
  //  h_xs_HT_fat_qg_1010_stat_E_d2->Draw("sameC");  
  g_xs_HT_fat_qg_1010_stat_E_m->Draw("sameC");
  g_xs_HT_fat_qg_1010->Draw("samePL");

  g_xs_string->Draw("samePL");  
  g_xs_qstar->Draw("sameL");
  //  g_xs_axigluon->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qg_stat_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qg_stat_E->Draw();

  p_xs_qg_stat_E->SetFillColor(0);
  p_xs_qg_stat_E->SetFillStyle(0);
  p_xs_qg_stat_E->SetBorderSize(0);
  p_xs_qg_stat_E->AddText("CMS (4.0 fb^{-1})");
  p_xs_qg_stat_E->AddText("#sqrt{s} = 8 TeV");
  p_xs_qg_stat_E->AddText("qg: statistical only");
  p_xs_qg_stat_E->Draw("sames");
	
  l_xs_qg_stat_E->SetFillColor(0);
  l_xs_qg_stat_E->SetFillStyle(0);
//  l_xs_qg_stat_E->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_qg_stat_E->SetHeader("CMS Preliminary");
  //  l_xs_qg_stat_E->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_qg_stat_E->AddEntry(g_xs_HT_fat_qg_1010,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qg_stat_E->AddEntry(g_xs_HT_fat_qg_1010_stat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qg_stat_E->Draw("sames");
  l_xs_theory_qg->Draw("sames");

  c_xs_qg_stat_E->Update();

  c_xs_qg_stat_E->SaveAs("Fit_Results_2012/Exclusions_qg_withExpected_statonly.png");
  c_xs_qg_stat_E->SaveAs("Fit_Results_2012/Exclusions_qg_withExpected_statonly.eps");



  // c_xs_qq_stat_E

  THStack *h_xs_qq_stat_E = new THStack("h_xs_qq_stat_E","h_xs_qq_stat_E");
  TH1F *htemp_xs_qq_stat_E_u2 = (TH1F*)h_xs_HT_fat_qq_1010_stat_E_u2->Clone("htemp_xs_qq_stat_E_u2");
  TH1F *htemp_xs_qq_stat_E_u1 = (TH1F*)h_xs_HT_fat_qq_1010_stat_E_u1->Clone("htemp_xs_qq_stat_E_u1");  
  TH1F *htemp_xs_qq_stat_E_d1 = (TH1F*)h_xs_HT_fat_qq_1010_stat_E_d1->Clone("htemp_xs_qq_stat_E_d1");   
  htemp_xs_qq_stat_E_u2->Add(h_xs_HT_fat_qq_1010_stat_E_u1,-1);
  htemp_xs_qq_stat_E_u1->Add(h_xs_HT_fat_qq_1010_stat_E_d1,-1);
  htemp_xs_qq_stat_E_d1->Add(h_xs_HT_fat_qq_1010_stat_E_d2,-1);

  h_xs_qq_stat_E->Add(h_xs_HT_fat_qq_1010_stat_E_d2);
  h_xs_qq_stat_E->Add(htemp_xs_qq_stat_E_d1);
  h_xs_qq_stat_E->Add(htemp_xs_qq_stat_E_u1);
  h_xs_qq_stat_E->Add(htemp_xs_qq_stat_E_u2);

  h_xs_qq_stat_E->SetMinimum(xMin);
  h_xs_qq_stat_E->SetMaximum(xMax);

  c_xs_qq_stat_E->cd(0);

  h_xs_HT_fat_qq_1010_stat_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_qq_stat_E->SetTitle("");
  h_xs_HT_fat_qq_1010_stat_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qq_1010_stat_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qq_stat_E->Draw("C");
  
  h_xs_qq_stat_E->GetXaxis()->SetTitle("qq Resonance Mass (GeV)");
  h_xs_qq_stat_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");
 
  g_xs_HT_fat_qq_1010_stat_E_m->Draw("sameC");
  g_xs_HT_fat_qq_1010->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qq_stat_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qq_stat_E->Draw();
	
  p_xs_qq_stat_E->SetFillColor(0);
  p_xs_qq_stat_E->SetFillStyle(0);
  p_xs_qq_stat_E->SetBorderSize(0);
  p_xs_qq_stat_E->AddText("CMS (4.0 fb^{-1})");
  p_xs_qq_stat_E->AddText("#sqrt{s} = 8 TeV");
  p_xs_qq_stat_E->AddText("qq: statistical only");
  p_xs_qq_stat_E->SetTextSize(0.028);
  p_xs_qq_stat_E->SetTextAlign(12);
  p_xs_qq_stat_E->Draw("sames");

  l_xs_qq_stat_E->SetFillColor(0);
  l_xs_qq_stat_E->SetFillStyle(0);
  //  l_xs_qq_stat_E->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_qq_stat_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_stat_E->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_qq_stat_E->AddEntry(g_xs_HT_fat_qq_1010,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_d2,"-2 #sigma","pL");
  //  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_d1,"-1 #sigma","pL");
  l_xs_qq_stat_E->AddEntry(g_xs_HT_fat_qq_1010_stat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qq_stat_E->Draw("sames");

  l_xs_theory_qq->Draw("sames");
  
  c_xs_qq_stat_E->Update();

  c_xs_qq_stat_E->SaveAs("Fit_Results_2012/Exclusions_qq_withExpected_statonly.png");
  c_xs_qq_stat_E->SaveAs("Fit_Results_2012/Exclusions_qq_withExpected_statonly.eps");























  c_xs_comparison_bw_stat_sys_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
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
  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (4.0 fb^{-1})");
  //  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry((TObject*)0,"4.0 fb^{-1}","")
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();

  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results_2012/Exclusions_qg_SysErrorsDependance.png");
  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results_2012/Exclusions_qg_SysErrorsDependance.eps");






  c_xs_comparison_bw_stat_sys_fat_norm->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
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

  l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (4.0 fb^{-1})");


  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_lumi_fat_norm,"Lumi Normal / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JES_fat_norm,"JES Normal / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JER_fat_norm,"JER Normal / stat","pL");

  l_xs_comparison_bw_stat_sys_fat_norm->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_norm->Update();


  string sOut_png = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_priorDependance.png";
  string sOut_eps = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_priorDependance.eps";

  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_eps.c_str());











  c_xs_comparison_bw_stat_sys_fat_norm->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(stat only alt prior 95% CL #sigma) / (stat.only 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.7);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_HT_fat_qq_compare_JeffereyPrior->Draw("samePL");

	
  l_xs_comparison_bw_stat_sys_fat_norm->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_norm->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (4.0 fb^{-1})");


  l_xs_comparison_bw_stat_sys_fat_norm->Clear();
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_HT_fat_qq_compare_JeffereyPrior,"qq: Jeffrey prior / Flat","pL");

  l_xs_comparison_bw_stat_sys_fat_norm->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_norm->Update();


  string sOut_png = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_signalPriorDependance.png";
  string sOut_eps = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_signalPriorDependance.eps";

  //  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_png.c_str());
  //  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_eps.c_str());









  c_xs_comparison_bw_fit_method->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(Alt. fit methods 95% CL #sigma) / (stat.only 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.3);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_fat_qq_compare_nonConservativeFit->Draw("samePL");
  g_xs_HT_fat_qq_compare_excludedFitRange->Draw("samePL");

  g_xs_HT_fat_qg_compare_nonConservativeFit->Draw("samePL");
  g_xs_HT_fat_qg_compare_excludedFitRange->Draw("samePL");

  g_xs_HT_fat_gg_compare_nonConservativeFit->Draw("samePL");
  g_xs_HT_fat_gg_compare_excludedFitRange->Draw("samePL");
	
  l_xs_comparison_bw_fit_method->SetFillColor(0);
  l_xs_comparison_bw_fit_method->SetFillStyle(0);

  l_xs_comparison_bw_fit_method->SetHeader("CMS (4.0 fb^{-1})");


  l_xs_comparison_bw_fit_method->AddEntry(g_xs_HT_fat_qq_compare_nonConservativeFit,"qq: bkg+sig. stat","pL");
  l_xs_comparison_bw_fit_method->AddEntry( g_xs_HT_fat_qq_compare_excludedFitRange,"qq: bkg+sig. sys","pL");

  l_xs_comparison_bw_fit_method->AddEntry(g_xs_HT_fat_qg_compare_nonConservativeFit,"qg: bkg+sig. stat","pL");
  l_xs_comparison_bw_fit_method->AddEntry( g_xs_HT_fat_qg_compare_excludedFitRange,"qg: bkg+sig. sys","pL");

  l_xs_comparison_bw_fit_method->AddEntry(g_xs_HT_fat_gg_compare_nonConservativeFit,"gg: bkg+sig. stat","pL");
  l_xs_comparison_bw_fit_method->AddEntry( g_xs_HT_fat_gg_compare_excludedFitRange,"gg: bkg+sig. sys","pL");


  l_xs_comparison_bw_fit_method->Draw("sames");

  c_xs_comparison_bw_fit_method->Update();


  string sOut_png = "Fit_Results_2012/Exclusions_qg_fitMethodDependance.png";
  string sOut_eps = "Fit_Results_2012/Exclusions_qg_fitMethodDependance.eps";

  //  c_xs_comparison_bw_fit_method->SaveAs(sOut_png.c_str());
  // c_xs_comparison_bw_fit_method->SaveAs(sOut_eps.c_str());





















  c_xs_comparison_bw_stat_sys_fat_Bkg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(Model+Stat. 95% CL #sigma) / (Stat. 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.5);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  //  g_xs_comparison_bw_stat_sys_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg_fat->Draw("samePL");
  //  g_xs_comparison_bw_stat_bg_alternative_fat->Draw("samePL");
  //  g_xs_comparison_bw_stat_bg_alternativeB_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative1Up_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_FitA_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_Fit2011_fat->Draw("samePL");

  l_xs_comparison_bw_stat_sys_fat_Bkg->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg->SetFillStyle(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg1->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg1->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat_Bkg->SetHeader("CMS (4.0 fb^{-1})");

  //  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_fat,"Average eig. background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_FitA_fat,"background alt. A/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_Fit2011_fat,"background alt. B/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative1Up_fat,"background alt. V1 Up/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative2Up_fat,"background alt. V2 Up/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative3Up_fat,"background alt. V3 Up/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative1Do_fat,"background alt. V1 Do/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative2Do_fat,"background alt. V2 Do/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative3Do_fat,"background alt. V3 Do/ stat","pL");


  l_xs_comparison_bw_stat_sys_fat_Bkg->Draw("sames");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_Bkg->Update();


  string sOut_png = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_Bkg.png";
  string sOut_eps = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_Bkg.eps";

  //  c_xs_comparison_bw_stat_sys_fat_Bkg->SaveAs(sOut_png.c_str());
  // c_xs_comparison_bw_stat_sys_fat_Bkg->SaveAs(sOut_eps.c_str());



 







  c_xs_comparison_bw_stat_sys_fat_Bkg->cd(0);
  c_xs_comparison_bw_stat_sys_fat_Bkg->Clear();
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(Model+Stat. 95% CL #sigma) / (Stat. 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.5);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_comparison_bw_stat_sys_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative_fat->Draw("samePL");
  //  g_xs_comparison_bw_stat_bg_alternativeB_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_FitA_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_Fit2011_fat->Draw("samePL");

  l_xs_comparison_bw_stat_sys_fat_Bkg->Clear();

  l_xs_comparison_bw_stat_sys_fat_Bkg->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat_Bkg->SetHeader("CMS (4.0 fb^{-1})");

  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_fat,"Average eig. background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_alternative_fat,"Average alt. background/ stat","pL");
  //  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_alternativeB_fat,"background alt. AB/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_FitA_fat,"background alt. A/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_Fit2011_fat,"background alt. B/ stat","pL");

  l_xs_comparison_bw_stat_sys_fat_Bkg->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_Bkg->Update();


  string sOut_png = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_BkgAlt.png";
  string sOut_eps = "Fit_Results_2012/Exclusions_qg_SysErrorsDependance_BkgAlt.eps";

  //  c_xs_comparison_bw_stat_sys_fat_Bkg->SaveAs(sOut_png.c_str());
  // c_xs_comparison_bw_stat_sys_fat_Bkg->SaveAs(sOut_eps.c_str());



























  /*
  // c_xs_qg_compare

  c_xs_qg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qg_compare->Draw("PL");

  l_xs_qg_compare->SetFillColor(0);
  l_xs_qg_compare->SetFillStyle(0);
  l_xs_qg_compare->SetHeader("qg-(Stat.Only)-4.0 fb^{-1}");
  l_xs_qg_compare->AddEntry(g_xs_HT_fat_qg_compare,"HT-fat","pL");

  l_xs_qg_compare->Draw("sames");

  c_xs_qg_compare->Update();

  // c_xs_gg_compare

  c_xs_gg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_gg_compare->Draw("PL");

  l_xs_gg_compare->SetFillColor(0);
  l_xs_gg_compare->SetFillStyle(0);
  l_xs_gg_compare->SetHeader("gg-(Stat.Only)-4.0 fb^{-1}");
  l_xs_gg_compare->AddEntry(g_xs_HT_fat_gg_compare,"HT-fat","pL");

  l_xs_gg_compare->Draw("sames");

  c_xs_gg_compare->Update();

  // c_xs_qq_compare

  c_xs_qq_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_qq_compare->Draw("PL");

  l_xs_qq_compare->SetFillColor(0);
  l_xs_qq_compare->SetFillStyle(0);
  l_xs_qq_compare->SetHeader("qq-(Stat.Only)-4.0 fb^{-1}");
  l_xs_qq_compare->AddEntry(g_xs_HT_fat_qq_compare,"HT-fat","pL");

  l_xs_qq_compare->Draw("sames");

  c_xs_qq_compare->Update();
  */
  // c_xs_rsg

  
  c_xs_rsg->cd();

  
  TH1F *vFrame = gPad->DrawFrame(990.0,0.08,1410.0,4.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("RS Graviton Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetNdivisions(510);


  

  THStack *h_xs_RSG_sys_E = new THStack("h_xs_RSG_sys_E","h_xs_RSG_sys_E");
  TH1F *htemp_xs_RSG_sys_E_u2 = (TH1F*)h_xs_RSG_for_limit_sys_fat_E_u2->Clone("htemp_xs_RSG_sys_E_u2");
  TH1F *htemp_xs_RSG_sys_E_u1 = (TH1F*)h_xs_RSG_for_limit_sys_fat_E_u1->Clone("htemp_xs_RSG_sys_E_u1");  
  TH1F *htemp_xs_RSG_sys_E_d1 = (TH1F*)h_xs_RSG_for_limit_sys_fat_E_d1->Clone("htemp_xs_RSG_sys_E_d1");   
  htemp_xs_RSG_sys_E_u2->Add(h_xs_RSG_for_limit_sys_fat_E_u1,-1);
  htemp_xs_RSG_sys_E_u1->Add(h_xs_RSG_for_limit_sys_fat_E_d1,-1);
  htemp_xs_RSG_sys_E_d1->Add(h_xs_RSG_for_limit_sys_fat_E_d2,-1);
 

  h_xs_RSG_sys_E->Add(h_xs_RSG_for_limit_sys_fat_E_d2);
  h_xs_RSG_sys_E->Add(htemp_xs_RSG_sys_E_d1);
  h_xs_RSG_sys_E->Add(htemp_xs_RSG_sys_E_u1);
  h_xs_RSG_sys_E->Add(htemp_xs_RSG_sys_E_u2);

  h_xs_RSG_sys_E->Draw("sameC");
  vFrame->Draw("sameAXIS");
  

  gPad->Update();

  g_xs_RSG_for_limit_sys_fat_E_m->Draw("sameC");



  g_xs_RSG_for_limit_fat->Draw("samePL");
  g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");


  g_xs_rsgraviton->Draw("sameL");  

  l_xs_rsg->SetFillColor(0);
  l_xs_rsg->SetFillStyle(0);
  l_xs_rsg->Clear();
  l_xs_rsg->SetTextSize(0.025);

  l_xs_rsg->AddEntry(g_xs_HT_fat_qq_1010_sys,"qq Observed 95% CL Upper Limit","pL");
  l_xs_rsg->AddEntry(g_xs_HT_fat_gg_1010_sys,"gg Observed 95% CL Upper Limit","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_fat,"Observed 95% CL Upper Limit","pL");
  l_xs_rsg->DrawClone("sames");

  l_xs_rsg_2->SetFillColor(0);
  l_xs_rsg_2->SetFillStyle(0);
  l_xs_rsg_2->Clear();
  l_xs_rsg_2->SetTextSize(0.025);
  l_xs_rsg_2->AddEntry(g_xs_RSG_for_limit_sys_fat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_rsg_2->AddEntry(h_xs_RSG_for_limit_sys_fat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_rsg_2->AddEntry(h_xs_RSG_for_limit_sys_fat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_rsg_2->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");
  l_xs_rsg_2->Draw("sames");


  p_xs_RSG_sys_E->SetFillColor(0);
  p_xs_RSG_sys_E->SetFillStyle(0);
  p_xs_RSG_sys_E->SetBorderSize(0);
  p_xs_RSG_sys_E->SetLineColor(0);
  p_xs_RSG_sys_E->SetTextSize(0.03);
  p_xs_RSG_sys_E->SetTextAlign(12);
  p_xs_RSG_sys_E->AddText("CMS Preliminary (4.0 fb^{-1})");
  p_xs_RSG_sys_E->AddText("#sqrt{s} = 8 TeV");
  p_xs_RSG_sys_E->Draw("sames");


  //  c_xs_rsg->SaveAs("Fit_Results_2012/Exclusions_ggqq_Graviton.png");
  // c_xs_rsg->SaveAs("Fit_Results_2012/Exclusions_ggqq_Graviton.eps");
  










  c_xs_rsg->cd();
  c_xs_rsg->Clear();

  
  TH1F *vFrame = gPad->DrawFrame(990.0,0.08,1410.0,50.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("RS Graviton Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetNdivisions(510);


  

  THStack *h_xs_RSG_sys_E = new THStack("h_xs_RSG_sys_E","h_xs_RSG_sys_E");
  TH1F *htemp_xs_RSG_sys_E_u2 = (TH1F*)h_xs_RSG_for_limit_sys_fat_E_u2->Clone("htemp_xs_RSG_sys_E_u2");
  TH1F *htemp_xs_RSG_sys_E_u1 = (TH1F*)h_xs_RSG_for_limit_sys_fat_E_u1->Clone("htemp_xs_RSG_sys_E_u1");  
  TH1F *htemp_xs_RSG_sys_E_d1 = (TH1F*)h_xs_RSG_for_limit_sys_fat_E_d1->Clone("htemp_xs_RSG_sys_E_d1");   
  htemp_xs_RSG_sys_E_u2->Add(h_xs_RSG_for_limit_sys_fat_E_u1,-1);
  htemp_xs_RSG_sys_E_u1->Add(h_xs_RSG_for_limit_sys_fat_E_d1,-1);
  htemp_xs_RSG_sys_E_d1->Add(h_xs_RSG_for_limit_sys_fat_E_d2,-1);
 

  h_xs_RSG_sys_E->Add(h_xs_RSG_for_limit_sys_fat_E_d2);
  h_xs_RSG_sys_E->Add(htemp_xs_RSG_sys_E_d1);
  h_xs_RSG_sys_E->Add(htemp_xs_RSG_sys_E_u1);
  h_xs_RSG_sys_E->Add(htemp_xs_RSG_sys_E_u2);

  h_xs_RSG_sys_E->Draw("sameC");
  vFrame->Draw("sameAXIS");
  

  gPad->Update();

  g_xs_RSG_for_limit_sys_fat_E_m->Draw("sameC");



  g_xs_RSG_for_limit_fat->Draw("samePL");

  g_xs_rsgraviton->SetLineColor(38);
  g_xs_rsgraviton->Draw("sameL");  



  l_xs_rsg->Clear();

  l_xs_rsg->SetTextSize(0.03);
  l_xs_rsg->AddEntry(h_xs_RSG_for_limit_sys_fat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_rsg->AddEntry(h_xs_RSG_for_limit_sys_fat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_rsg->DrawClone("sames");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"RS Graviton","pL");

  l_xs_rsg_2->SetTextSize(0.03);
  l_xs_rsg_2->SetFillColor(0);
  l_xs_rsg_2->SetFillStyle(0);
  l_xs_rsg_2->Clear();
  l_xs_rsg_2->AddEntry(g_xs_RSG_for_limit_fat,"Observed 95% CL Upper Limit","pL");
  l_xs_rsg_2->AddEntry(g_xs_RSG_for_limit_sys_fat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_rsg_2->Draw("sames");

  p_xs_RSG_sys_E->Draw("sames");


  c_xs_rsg->SaveAs("Fit_Results_2012/Exclusions_ggqq_Graviton_only.png");
  c_xs_rsg->SaveAs("Fit_Results_2012/Exclusions_ggqq_Graviton_only.eps");











 

}





double findExclusion(double* Exclusion, string sOut, unsigned int nBins){

  //  cout << "Looking for exclusion " << endl;

  int iMin = -2, iMax = -2;

  for (unsigned int j = 0; j < nBins; j++){

    if (Exclusion[j]-1 > 0) iMin = j;
    else if (Exclusion[j]-1<0 && iMin == j-1) iMax = j;

  }

  if (iMax < 0) iMax = nBins -1;

  double dM = x[iMax]-x[iMax-1];
  double dXs = Exclusion[iMax-1] - Exclusion[iMax];
  double dXs1 = Exclusion[iMax-1]-1.;
  double dM1 = dM/dXs*dXs1;

  double maxXsec = abs((x[iMax-1]+dM1)/10.)*10;

  cout << sOut.c_str() << " [ " << x[0] << ", " << x[iMax] << " ]" << endl;
  cout << sOut.c_str() << " [ " << x[0] << ", " << maxXsec << " ]" << endl;
  
  return maxXsec;

}

