#include <iostream>
#include <iomanip>


const unsigned int nEBins = 34;
double minMass = 850.0, maxMass = 4350.0;

double x[nEBins] = {1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0, 3000.0, 3100.0, 3200.0, 3300.0, 3400.0, 3500.0, 3600.0, 3700.0, 3800.0, 3900.0, 4000.0, 4100.0, 4200., 4300.};

void draw() 
{
  std::cout << "begin" << std::endl;

  double xMin = 0.0005;
  double xMax = 1.5;
  
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


  /*
  TCanvas *c_xs_gg_compare = new TCanvas("c_xs_gg_compare","c_xs_gg_compare",459,358,600,602);
  TCanvas *c_xs_qg_compare = new TCanvas("c_xs_qg_compare","c_xs_qg_compare",459,358,600,602);
  TCanvas *c_xs_qq_compare = new TCanvas("c_xs_qq_compare","c_xs_qq_compare",459,358,600,602);
  */
  TCanvas *c_xs_comparison_bw_sys_theory_fat = new TCanvas("c_xs_comparison_bw_sys_theory_fat","c_xs_comparison_bw_sys_theory_fat",459,358,600,602);

  TCanvas *c_xs_qg_stat_E = new TCanvas("c_xs_qg_stat_E","c_xs_qg_stat_E",459,358,600,602);
  TCanvas *c_xs_qq_stat_E = new TCanvas("c_xs_qq_stat_E","c_xs_qq_stat_E",459,358,600,602);
  TCanvas *c_xs_qg_sys_E = new TCanvas("c_xs_qg_sys_E","c_xs_qg_sys_E",459,358,600,602);
  TCanvas *c_xs_qq_sys_E = new TCanvas("c_xs_qq_sys_E","c_xs_qq_sys_E",459,358,600,602);
  TCanvas *c_xs_rsg = new TCanvas("c_xs_rsg","c_xs_rsg",459,358,600,602);

  // definition of legends

  TLegend *l_xs_theory =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qg =  new TLegend(0.66,0.7108014,0.95,0.9216028,NULL,"brNDC");
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

  TLegend *l_xs_gg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qq_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_fat =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_qg_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qq_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qg_stat_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qq_stat_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");

  TLegend *l_xs_rsg =  new TLegend(0.6,0.50,0.93,0.93,NULL,"brNDC");

  TPaveText *p_xs_RSG_sys_E =  new TPaveText(0.1527517,0.1555401,0.3773826,0.3087805,"brNDC");

  TPaveText *p_xs_qg_sys_E =  new TPaveText(0.1527517,0.4255401,0.3773826,0.5787805,"brNDC");
  TPaveText *p_xs_qq_sys_E =  new TPaveText(0.1527517,0.4255401,0.3773826,0.5787805,"brNDC");

  TPaveText *p_xs_qg_stat_E =  new TPaveText(0.1527517,0.4255401,0.3773826,0.5787805,"brNDC");
  TPaveText *p_xs_qq_stat_E =  new TPaveText(0.1527517,0.4255401,0.3773826,0.5787805,"brNDC");

  TPaveText *p_xs_sys = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
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

  const unsigned int en = 38;

  // theory values


  double mjj [en] ={ 800.0, 900.0, 1000.0, 1100.0, 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300., 4400., 4500.};

  double qstar [en] = {
0.2355E+03, 0.1257E+03, 0.7005E+02, 0.4039E+02, 0.2394E+02,  0.1452E+02, 0.8982E+01,  0.5645E+01,  0.3596E+01,  0.2317E+01, 
0.1507E+01, 0.9889E+00, 0.6531E+00, 0.4338E+00, 0.2896E+00, 0.1940E+00 ,  0.1304E+00, 0.8782E-01, 0.5925E-01,  0.4002E-01,  
0.2704E-01, 0.1828E-01, 0.1234E-01, 0.8329E-02, 0.5613E-02,  0.3776E-02, 0.2535E-02, 0.1698E-02,  0.1135E-02, 0.7559E-03, 
0.5021E-03, 0.3325E-03, 0.2195E-03, 0.1444E-03, 0.9478E-04, 0.6203E-04, 0.4050E-04, 0.2639E-04};

  double axigluon[en] = {
0.1193E+03, 0.6750E+02, 0.3967E+02, 0.2400E+02, 0.1486E+02, 0.9370E+01, 0.5998E+01, 0.3887E+01, 0.2544E+01, 0.1678E+01, 
0.1115E+01, 0.7442E+00, 0.4988E+00, 0.3354E+00, 0.2260E+00, 0.1525E+00, 0.1030E+00, 0.6949E-01, 0.4684E-01, 0.3152E-01, 
0.2116E-01, 0.1415E-01, 0.9428E-02, 0.6250E-02, 0.4119E-02, 0.2698E-02, 0.1754E-02, 0.1131E-02, 0.7222E-03, 0.4568E-03, 
0.2858E-03, 0.1767E-03, 0.1079E-03};

  double axigluon_kfactor[en] = {
1.09, 1.10, 1.11, 1.12, 1.13, 1.14, 1.15, 1.16, 1.17, 1.18, 
1.20, 1.21, 1.22, 1.24, 1.25, 1.26, 1.27, 1.29, 1.30, 1.32, 
1.33, 1.35, 1.36, 1.39, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 
1.51, 1.53, 1.55};

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


  double diquark[en] = {
0.5435E+02, 0.3554E+02, 0.2393E+02, 0.1648E+02, 0.1154E+02, 0.8194E+01, 0.5877E+01, 0.4249E+01, 0.3090E+01, 0.2258E+01, 
0.1656E+01, 0.1217E+01, 0.8953E+00, 0.6591E+00, 0.4852E+00, 0.3569E+00, 0.2622E+00, 0.1922E+00, 0.1406E+00, 0.1025E+00, 
0.7449E-01, 0.5392E-01, 0.3885E-01, 0.2786E-01, 0.1987E-01, 0.1408E-01, 0.9920E-02, 0.6938E-02, 0.4815E-02, 0.3315E-02, 
0.2261E-02, 0.1528E-02, 0.1022E-02};

  double string[en] = {
0.4627E+04, 0.2485E+02, 0.1392E+04, 0.7879E+03, 473.13, 290.1, 177.6, 111.91, 72.12, 47.07, 
31.05, 20.60, 13.82, 9.117, 6.244, 4.238, 2.881, 1.973, 1.367, 0.93418, 
0.644897, 0.445018, 0.304044, 0.212039, 0.143917, 0.099195, 0.066999, 0.0462355, 0.0313552, 0.021399, 
0.0141453, 0.0095593, 0.0064255, 0.004227117035341108, 0.002830943528441785, 0.001830831111740912, 0.0012006226542702247, 0.0007818600156759468};//, 0.0004978553320724371, 0.0003197273428031069, 0.00020157675764867537, 0.00012686973888958498, 0.00008018201235487667};

  double zprime[en] = {
    0.3427E+01, 0.1969E+01, 0.1172E+01, 0.7171E+00, 0.4486E+00, 0.2857E+00, 0.1845E+00, 0.1206E+00, 0.7961E-01, 0.5295E-01, 
    0.3545E-01, 0.2386E-01, 0.1611E-01, 0.1092E-01, 0.7413E-02, 0.5039E-02, 0.3426E-02, 0.2329E-02, 0.1580E-02, 0.1070E-02, 
    0.7231E-03, 0.4867E-03, 0.3261E-03, 0.2174E-03, 0.1440E-03, 0.9477E-04, 0.6190E-04, 0.4007E-04, 0.2570E-04, 0.1631E-04, 
    0.1024E-04, 0.6349E-05, 0.3889E-05};

  double wprime[en] = {
    0.6263E+01, 0.3661E+01, 0.2212E+01, 0.1372E+01, 0.8673E+00, 0.5568E+00, 0.3616E+00, 0.2369E+00, 0.1562E+00, 0.1034E+00,
    0.6872E-01, 0.4572E-01, 0.3043E-01, 0.2023E-01, 0.1342E-01, 0.8884E-02, 0.5859E-02, 0.3847E-02, 0.2513E-02, 0.1632E-02, 
    0.1053E-02, 0.6744E-03, 0.4287E-03, 0.2702E-03, 0.1688E-03, 0.1044E-03, 0.6403E-04, 0.3886E-04, 0.2335E-04, 0.1390E-04, 
    0.8199E-05, 0.4796E-05, 0.2787E-05};

  double rsgraviton[en] = {
    0.3852E+01, 0.1961E+01, 0.1053E+01, 0.5905E+00, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01, 0.4911E-01, 0.3145E-01, 
    0.2036E-01, 0.1330E-01, 0.8743E-02, 0.5781E-02, 0.3840E-02, 0.2559E-02, 0.1708E-02, 0.1142E-02, 0.7635E-03, 0.5101E-03, 
    0.3402E-03, 0.2264E-03, 0.1501E-03, 0.9913E-04, 0.6512E-04, 0.4253E-04, 0.2759E-04, 0.1775E-04, 0.1133E-04, 0.7157E-05, 
    0.4475E-05, 0.2766E-05, 0.1689E-05};

 double x_RSG[5] = {1000.0, 1100.0, 1200.0, 1300., 1400.};




 TFile* file_efficiencies_qq = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_RSGraviton_ak5_QQtoQQ_fat30.root", "READ");
 TFile* file_efficiencies_qg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_Qstar_ak5_fat30.root", "READ");
 TFile* file_efficiencies_gg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/Resonance_Shapes_RSGraviton_ak5_GGtoGG_fat30.root", "READ");

 TH1F* limit_efficiencies_qq = (TH1F*) file_efficiencies_qq->Get("Efficiency;1");
 TH1F* limit_efficiencies_qg = (TH1F*) file_efficiencies_qg->Get("Efficiency;1");
 TH1F* limit_efficiencies_gg = (TH1F*) file_efficiencies_gg->Get("Efficiency;1");


 TFile* file_qq0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_qg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_0.root", "READ");
 TFile* file_gg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_GGtoGG_fat30_0.root", "READ");

 
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



 TFile* file_qq2000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_qg2000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_Qstar_ak5_fat30_0.root", "READ");
 TFile* file_gg2000 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_ak5_GGtoGG_fat30_0.root", "READ");

 
 TH1F* limit_qq2000 = (TH1F*) file_qq2000->Get("limit;1");
 TH1F* limit_qg2000 = (TH1F*) file_qg2000->Get("limit;1");
 TH1F* limit_gg2000 = (TH1F*) file_gg2000->Get("limit;1");


 for (int i = 1; i < limit_qq2000->GetNbinsX()+1; i++){

   limit_qq2000->SetBinContent(i, limit_qq2000->GetBinContent(i) / limit_efficiencies_qq->GetBinContent(i));
   limit_qg2000->SetBinContent(i, limit_qg2000->GetBinContent(i) / limit_efficiencies_qg->GetBinContent(i));
   limit_gg2000->SetBinContent(i, limit_gg2000->GetBinContent(i) / limit_efficiencies_gg->GetBinContent(i));

 }

 // fat-stat only --
 double* xs_HT_fat_qq_1010_noFirstGeVBin = new double[nEBins];
 double* xs_HT_fat_qg_1010_noFirstGeVBin = new double[nEBins];
 double* xs_HT_fat_gg_1010_noFirstGeVBin = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_qq_1010_noFirstGeVBin[i] = limit_qq2000->GetBinContent(i+1);
   xs_HT_fat_qg_1010_noFirstGeVBin[i] = limit_qg2000->GetBinContent(i+1);
   xs_HT_fat_gg_1010_noFirstGeVBin[i] = limit_gg2000->GetBinContent(i+1);
 }

 file_qq2000->Close();
 file_qg2000->Close();
 file_gg2000->Close(); 


 

 cout << "looking differentially for systematics" << endl;


 // TFile* file_qg_bg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1.root", "READ");
 // TFile* file_qg_bg_alternative = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_11.root", "READ");
 // TFile* file_qg_lumi = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_2.root", "READ");
 // TFile* file_qg_JES = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_3.root", "READ");
 // TFile* file_qg_JER = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_4.root", "READ");
 
 // TFile* file_qg_lumi_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1002.root", "READ");
 // TFile* file_qg_JES_norm= new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1003.root", "READ");
 // TFile* file_qg_JER_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_1003.root", "READ"); 

 
 TFile* file_qg_bg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_1.root", "READ");
 TFile* file_qg_lumi = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_2.root", "READ");
 TFile* file_qg_JES = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_3.root", "READ");
 TFile* file_qg_JER = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_4.root", "READ");
 


 TFile* file_qg_lumi_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_1002.root", "READ");
 TFile* file_qg_JES_norm= new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_1003.root", "READ");
 TFile* file_qg_JER_norm = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_1004.root", "READ");
 
 TFile* file_qg_bg_alternative = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_11.root", "READ");
 TFile* file_qg_bg_alternativeB = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_12.root", "READ");
 TFile* file_qg_bg_alternative1Up = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_100.root", "READ");
 TFile* file_qg_bg_alternative2Up = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_101.root", "READ");
 TFile* file_qg_bg_alternative3Up = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_102.root", "READ");
 TFile* file_qg_bg_alternative1Do = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_103.root", "READ");
 TFile* file_qg_bg_alternative2Do = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_104.root", "READ");
 TFile* file_qg_bg_alternative3Do = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_105.root", "READ");





 TH1F* limit_qg_bg = (TH1F*) file_qg_bg->Get("limit;1");
 TH1F* limit_qg_bg_alternative = (TH1F*) file_qg_bg_alternative->Get("limit;1"); 
 TH1F* limit_qg_bg_alternativeB = (TH1F*) file_qg_bg_alternativeB->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative1Up = (TH1F*) file_qg_bg_alternative1Up->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative2Up = (TH1F*) file_qg_bg_alternative2Up->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative3Up = (TH1F*) file_qg_bg_alternative3Up->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative1Do = (TH1F*) file_qg_bg_alternative1Do->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative2Do = (TH1F*) file_qg_bg_alternative2Do->Get("limit;1"); 
 TH1F* limit_qg_bg_alternative3Do = (TH1F*) file_qg_bg_alternative3Do->Get("limit;1"); 

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
 TFile* file_qq6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_QQtoQQ_fat30_6.root", "READ");
 TFile* file_qg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_Qstar_ak5_fat30_6.root", "READ");
 TFile* file_gg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_limit_RSGraviton_ak5_GGtoGG_fat30_6.root", "READ");

 
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




 TFile* file_exp_qq0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_RSGraviton_ak5_QQtoQQ_fat30_0.root", "READ");
 TFile* file_exp_qg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_Qstar_ak5_fat30_0.root", "READ");
 TFile* file_exp_gg0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_RSGraviton_ak5_GGtoGG_fat30_0.root", "READ");


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





 TFile* file_exp_qq6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_RSGraviton_ak5_QQtoQQ_fat30_6.root", "READ");
 TFile* file_exp_qg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_Qstar_ak5_fat30_6.root", "READ");
 TFile* file_exp_gg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1_new/limit_expectedlimit50_RSGraviton_ak5_GGtoGG_fat30_6.root", "READ");


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

 double xs_HT_fat_qg_compare_noFirstGeVBin[nEBins], xs_HT_fat_gg_compare_noFirstGeVBin[nEBins], xs_HT_fat_qq_compare_noFirstGeVBin[nEBins];

 double xs_RSG_for_limit_fat[5], xs_qstar_over_qg_fat[nEBins], xs_string_over_qg_fat[nEBins], xs_diquark_over_qq_fat[31], xs_axigluon_over_qq_fat[31], xs_zprime_over_qq_fat[nEBins], xs_wprime_over_qq_fat[nEBins], xs_rsg_over_gg_fat[nEBins];
 double xs_RSG_for_limit_stat_fat[5], xs_qstar_over_qg_stat_fat[nEBins], xs_string_over_qg_stat_fat[nEBins], xs_diquark_over_qq_stat_fat[31], xs_axigluon_over_qq_stat_fat[31], xs_zprime_over_qq_stat_fat[nEBins], xs_wprime_over_qq_stat_fat[nEBins], xs_rsg_over_gg_stat_fat[nEBins];

 double xs_RSG_for_limit_stat_fat_E_m[5], xs_RSG_for_limit_sys_fat_E_m[5], xs_RSG_for_limit_sys_fat_E_d2[5], xs_RSG_for_limit_sys_fat_E_d1[5], xs_RSG_for_limit_sys_fat_E_u1[5], xs_RSG_for_limit_sys_fat_E_u2[5], xs_rsg_over_gg_stat_fat[5], xs_rsg_over_gg_stat_fat_E_m[5], xs_rsg_over_gg_sys_fat_E_m[5];

 double xs_comparison_bw_stat_lumi_fat_norm[nEBins], xs_comparison_bw_stat_JES_fat_norm[nEBins], xs_comparison_bw_stat_JER_fat_norm[nEBins];
 double xs_RSG_for_limit_expected_fat[5], xs_qstar_over_qg_expected_fat[nEBins], xs_string_over_qg_expected_fat[nEBins], xs_diquark_over_qq_expected_fat[31], xs_axigluon_over_qq_expected_fat[31], xs_zprime_over_qq_expected_fat[nEBins], xs_wprime_over_qq_expected_fat[nEBins], xs_rsg_over_gg_expected_fat[nEBins];
 double xs_RSG_for_limit_stat_expected_fat[5], xs_qstar_over_qg_stat_expected_fat[nEBins], xs_string_over_qg_stat_expected_fat[nEBins], xs_diquark_over_qq_stat_expected_fat[31], xs_axigluon_over_qq_stat_expected_fat[31], xs_zprime_over_qq_stat_expected_fat[nEBins], xs_wprime_over_qq_stat_expected_fat[nEBins], xs_rsg_over_gg_stat_expected_fat[nEBins];

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
     
   xs_HT_fat_qq_compare_noFirstGeVBin[i] = xs_HT_fat_qq_1010_noFirstGeVBin[i]/xs_HT_fat_qq_1010[i];
   xs_HT_fat_qg_compare_noFirstGeVBin[i] = xs_HT_fat_qg_1010_noFirstGeVBin[i] /xs_HT_fat_qg_1010[i];
   xs_HT_fat_gg_compare_noFirstGeVBin[i] = xs_HT_fat_gg_1010_noFirstGeVBin[i]/xs_HT_fat_gg_1010[i];


   xs_comparison_bw_stat_bg_fat[i] = xs_HT_fat_qg_1010_bg[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_lumi_fat[i] = xs_HT_fat_qg_1010_lumi[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JES_fat[i] = xs_HT_fat_qg_1010_JES[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JER_fat[i] = xs_HT_fat_qg_1010_JER[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_sys_fat[i] = xs_HT_fat_qg_1010_sys[i]/xs_HT_fat_qg_1010[i];

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
   if (i<31) xs_diquark_over_qq_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_sys[i];
   if (i<31) xs_axigluon_over_qq_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_zprime_over_qq_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_wprime_over_qq_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_sys[i];
   
   xs_qstar_over_qg_stat_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010[i];
   xs_string_over_qg_stat_fat[i] = string[i+2]/xs_HT_fat_qg_1010[i];
   if (i<31) xs_diquark_over_qq_stat_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010[i];
   if (i<31) xs_axigluon_over_qq_stat_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010[i];
   xs_zprime_over_qq_stat_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010[i];
   xs_wprime_over_qq_stat_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010[i];


   xs_qstar_over_qg_expected_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010_sys_E_m[i];
   xs_string_over_qg_expected_fat[i] = string[i+2]/xs_HT_fat_qg_1010_sys_E_m[i];
   if (i<31) xs_diquark_over_qq_expected_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];
   if (i<31) xs_axigluon_over_qq_expected_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];
   xs_zprime_over_qq_expected_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];
   xs_wprime_over_qq_expected_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_sys_E_m[i];


   xs_qstar_over_qg_stat_expected_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010_stat_E_m[i];
   xs_string_over_qg_stat_expected_fat[i] = string[i+2]/xs_HT_fat_qg_1010_stat_E_m[i];
   if (i<31) xs_diquark_over_qq_stat_expected_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   if (i<31) xs_axigluon_over_qq_stat_expected_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   xs_zprime_over_qq_stat_expected_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   xs_wprime_over_qq_stat_expected_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_stat_E_m[i];
   


   //xs_HT_fat_gg_1010_sys_E_m



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

 double excl_sys[7], excl_stat[7], excl_exp_sys[7], excl_exp_stat[7];
 
 cout << "Stat only limits" << endl;

 excl_stat[0] = findExclusion(xs_string_over_qg_stat_fat, string("Exclusion String stat"), nEBins);
 excl_stat[1] = findExclusion(xs_qstar_over_qg_stat_fat, string("Exclusion Qstar stat"), nEBins);
 excl_stat[2] = findExclusion(xs_axigluon_over_qq_stat_fat, string("Exclusion Axigluon stat"), 31);
 excl_stat[3] = findExclusion(xs_diquark_over_qq_stat_fat, string("Exclusion Diquark stat"), 31);
 excl_stat[4] = findExclusion(xs_wprime_over_qq_stat_fat, string("Exclusion W' stat"), nEBins);
 excl_stat[5] = findExclusion(xs_zprime_over_qq_stat_fat, string("Exclusion Z' stat"), nEBins);
 excl_stat[6] = findExclusion(xs_rsg_over_gg_stat_fat, string("Exclusion RSGraviton stat"), 5);

 cout << "Limits with systematics" << endl;

 excl_sys[0] = findExclusion(xs_string_over_qg_fat, string("Exclusion String sys"), nEBins);
 excl_sys[1] = findExclusion(xs_qstar_over_qg_fat, string("Exclusion Qstar sys"), nEBins);
 excl_sys[2] = findExclusion(xs_axigluon_over_qq_fat, string("Exclusion Axigluon sys"), 31);
 excl_sys[3] = findExclusion(xs_diquark_over_qq_fat, string("Exclusion Diquark sys"), 31);
 excl_sys[4] = findExclusion(xs_wprime_over_qq_fat, string("Exclusion W' sys"), nEBins);
 excl_sys[5] = findExclusion(xs_zprime_over_qq_fat, string("Exclusion Z' sys"), nEBins);
 excl_sys[6] = findExclusion(xs_rsg_over_gg_fat, string("Exclusion RSGraviton"), 5);
 
 cout << "Expected stat only limits" << endl;

 excl_exp_stat[0] = findExclusion(xs_string_over_qg_stat_expected_fat, string("Exclusion String stat"), nEBins);
 excl_exp_stat[1] = findExclusion(xs_qstar_over_qg_stat_expected_fat, string("Exclusion Qstar stat"), nEBins);
 excl_exp_stat[2] = findExclusion(xs_axigluon_over_qq_stat_expected_fat, string("Exclusion Axigluon stat"), 31);
 excl_exp_stat[3] = findExclusion(xs_diquark_over_qq_stat_expected_fat, string("Exclusion Diquark stat"), 31);
 excl_exp_stat[4] = findExclusion(xs_wprime_over_qq_stat_expected_fat, string("Exclusion W' stat"), nEBins);
 excl_exp_stat[5] = findExclusion(xs_zprime_over_qq_stat_expected_fat, string("Exclusion Z' stat"), nEBins);
 excl_exp_stat[6] = findExclusion(xs_rsg_over_gg_stat_fat_E_m, string("Exclusion RSGraviton"), 5);

 cout << "Expected limits with sytematics" << endl;

 excl_exp_sys[0] = findExclusion(xs_string_over_qg_expected_fat, string("Excl_usion String sys"), nEBins);
 excl_exp_sys[1] = findExclusion(xs_qstar_over_qg_expected_fat, string("Exclusion Qstar sys"), nEBins);
 excl_exp_sys[2] = findExclusion(xs_axigluon_over_qq_expected_fat, string("Exclusion Axigluon sys"), 31);
 excl_exp_sys[3] = findExclusion(xs_diquark_over_qq_expected_fat, string("Exclusion Diquark sys"), 31);
 excl_exp_sys[4] = findExclusion(xs_wprime_over_qq_expected_fat, string("Exclusion W' sys"), nEBins);
 excl_exp_sys[5] = findExclusion(xs_zprime_over_qq_expected_fat, string("Exclusion Z' sys"), nEBins);
 excl_exp_sys[6] = findExclusion(xs_rsg_over_gg_sys_fat_E_m, string("Exclusion RSGraviton"), 5);

 cout << "Stat only limits" << endl;
 cout << "Exclusion | Expected Exclusion" << endl;


 cout << "\\hline" << endl;
 cout << "M(S)  & [1.0, " << Form("%.2f", excl_stat[0]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[0]/1000.) << "] \\\\" << endl;
 cout << "M(q*)  & [1.0, " << Form("%.2f", excl_stat[1]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[1]/1000.) << "] \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "qq QCD resonances & & \\" << endl;
 cout << "\\hline" << endl;
 cout << "M(D)  & [1.0, " << Form("%.2f", excl_stat[3]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[3]/1000.) << "] \\\\" << endl;
 cout << "M(A)  & [1.0, " << Form("%.2f", excl_stat[2]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[2]/1000.) << "] \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "qq EW resonances & & \\" << endl;
 cout << "\\hline" << endl;
 cout << "M(W')  & [1.0, " << Form("%.2f", excl_stat[4]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[4]/1000.) << "] \\\\" << endl;
 cout << "M(Z')  & [1.0, " << Form("%.2f", excl_stat[5]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_stat[5]/1000.) << "] \\\\"  << endl;
 cout << "\\hline" << endl;
 cout << "qq+gg RS gravitons & & \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "M(RSG) & [1.0, " << Form("%.2f", excl_stat[6]/1000.)  <<  "]  & [1.0," << Form("%.2f", excl_exp_stat[6]/1000.) << "] \\\\"  << endl;


 cout << "Sys limits" << endl;
 cout << "Exclusion | Expected Exclusion" << endl;

 cout << "\\hline" << endl;
 cout << "M(S)  & [1.0, " << Form("%.2f", excl_sys[0]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[0]/1000.) << "] \\\\" << endl;
 cout << "M(q*)  & [1.0, " << Form("%.2f", excl_sys[1]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[1]/1000.) << "] \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "qq QCD resonances & & \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "M(D)  & [1.0, " << Form("%.2f", excl_sys[3]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[3]/1000.) << "] \\\\"  << endl;
 cout << "M(A)  & [1.0, " << Form("%.2f", excl_sys[2]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[2]/1000.) << "] \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "qq EW resonances & & \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "M(W')  & [1.0, " << Form("%.2f", excl_sys[4]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[4]/1000.) << "] \\\\" << endl;
 cout << "M(Z')  & [1.0, " << Form("%.2f", excl_sys[5]/1000.)  << "]  & [1.0," << Form("%.2f", excl_exp_sys[5]/1000.) << "] \\\\"  << endl;
 cout << "\\hline" << endl;
 cout << "qq+gg RS gravitons & & \\\\" << endl;				
 cout << "\\hline" << endl;
 cout << "M(RSG) & [1.0, " << Form("%.2f", excl_sys[6]/1000.)  <<  "]  & [1.0," << Form("%.2f", excl_exp_sys[6]/1000.) << "] \\\\"  << endl;

 cout << "Sys limits" << endl;
 cout << "Exclusion stat only | Exclusion" << endl;

 cout << "\\hline" << endl;
 cout << "M(S)  & [1.0, " << Form("%.2f", excl_stat[0]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[0]/1000.) << "] \\\\" << endl;
 cout << "M(q*)  & [1.0, " << Form("%.2f", excl_stat[1]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[1]/1000.) << "] \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "qq QCD resonances & & \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "M(D)  & [1.0, " << Form("%.2f", excl_stat[3]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[3]/1000.) << "] \\\\"  << endl;
 cout << "M(A)  & [1.0, " << Form("%.2f", excl_stat[2]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[2]/1000.) << "] \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "qq EW resonances & & \\\\" << endl;
 cout << "\\hline" << endl;
 cout << "M(W')  & [1.0, " << Form("%.2f", excl_stat[4]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[4]/1000.) << "] \\\\" << endl;
 cout << "M(Z')  & [1.0, " << Form("%.2f", excl_stat[5]/1000.)  << "]  & [1.0," << Form("%.2f", excl_sys[5]/1000.) << "] \\\\"  << endl;
 cout << "\\hline" << endl;
 cout << "qq+gg RS gravitons & & \\\\" << endl;				
 cout << "\\hline" << endl;
 cout << "M(RSG) & [1.0, " << Form("%.2f", excl_stat[6]/1000.)  <<  "]  & [1.0," << Form("%.2f", excl_sys[6]/1000.) << "] \\\\"  << endl;
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
  g_xs_diquark = new TGraph(33,mjj,diquark);
  g_xs_string = new TGraph(en,mjj,string);
  g_xs_zprime = new TGraph(14,mjj,zprime);
  g_xs_wprime = new TGraph(15,mjj,wprime);
  g_xs_rsgraviton = new TGraph(10,mjj,rsgraviton);

  // comparison b/w different Jet combinations.

  g_xs_HT_fat_qg_compare = new TGraph(nEBins,x,xs_HT_fat_qg_compare);
  g_xs_HT_fat_gg_compare = new TGraph(nEBins,x,xs_HT_fat_gg_compare);
  g_xs_HT_fat_qq_compare = new TGraph(nEBins,x,xs_HT_fat_qq_compare);
  
  g_xs_HT_fat_qg_compare_noFirstGeVBin = new TGraph(nEBins,x,xs_HT_fat_qg_compare_noFirstGeVBin);
  g_xs_HT_fat_gg_compare_noFirstGeVBin = new TGraph(nEBins,x,xs_HT_fat_gg_compare_noFirstGeVBin);
  g_xs_HT_fat_qq_compare_noFirstGeVBin = new TGraph(nEBins,x,xs_HT_fat_qq_compare_noFirstGeVBin);


  // comparison b/w sys.included cross section value and theory

   g_xs_qstar_over_qg_fat = new TGraph(31,x,xs_qstar_over_qg_fat);
   g_xs_string_over_qg_fat = new TGraph(nEBins,x,xs_string_over_qg_fat);
   g_xs_diquark_over_qq_fat = new TGraph(31,x,xs_diquark_over_qq_fat);
   g_xs_axigluon_over_qq_fat = new TGraph(31,x,xs_axigluon_over_qq_fat);
   g_xs_zprime_over_qq_fat = new TGraph(nEBins,x,xs_zprime_over_qq_fat);
   g_xs_wprime_over_qq_fat = new TGraph(nEBins,x,xs_wprime_over_qq_fat);
   g_xs_rsg_over_gg_fat = new TGraph(5,x,xs_rsg_over_gg_fat);



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


  g_xs_comparison_bw_stat_bg_alternative_fat->SetLineColor(kMagenta);
  g_xs_comparison_bw_stat_bg_alternative_fat->SetLineStyle(4);
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

  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetLineColor(kYellow);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetLineStyle(3);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetMarkerStyle(30);
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->SetMarkerColor(kYellow);

  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetLineColor(kBlue);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetLineStyle(4);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetMarkerStyle(31);
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->SetMarkerColor(kBlue);

  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetLineColor(kGreen);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetLineStyle(5);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetMarkerStyle(29);
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->SetMarkerColor(kGreen);

  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetLineColor(kYellow);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetLineStyle(6);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetMarkerStyle(30);
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->SetMarkerColor(kYellow);

  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetLineColor(kBlue);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetLineStyle(7);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetMarkerStyle(31);
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->SetMarkerColor(kBlue);


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


  g_xs_HT_fat_qg_compare_noFirstGeVBin->SetLineColor(1);
  g_xs_HT_fat_qg_compare_noFirstGeVBin->SetLineStyle(1);
  g_xs_HT_fat_qg_compare_noFirstGeVBin->SetLineWidth(3);
  g_xs_HT_fat_qg_compare_noFirstGeVBin->SetMarkerStyle(24);
  g_xs_HT_fat_qg_compare_noFirstGeVBin->SetMarkerColor(1);

  g_xs_HT_fat_gg_compare_noFirstGeVBin->SetLineColor(2);
  g_xs_HT_fat_gg_compare_noFirstGeVBin->SetLineStyle(1);
  g_xs_HT_fat_gg_compare_noFirstGeVBin->SetLineWidth(3);
  g_xs_HT_fat_gg_compare_noFirstGeVBin->SetMarkerStyle(24);
  g_xs_HT_fat_gg_compare_noFirstGeVBin->SetMarkerColor(2);

  g_xs_HT_fat_qq_compare_noFirstGeVBin->SetLineColor(3);
  g_xs_HT_fat_qq_compare_noFirstGeVBin->SetLineStyle(1);
  g_xs_HT_fat_qq_compare_noFirstGeVBin->SetLineWidth(3);
  g_xs_HT_fat_qq_compare_noFirstGeVBin->SetMarkerStyle(24);
  g_xs_HT_fat_qq_compare_noFirstGeVBin->SetMarkerColor(3);






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
  g_xs_rsgraviton->Draw("sameL");  


  
  l_xs_qg->SetFillColor(0);
  l_xs_qg->SetFillStyle(0);
  l_xs_qg->SetHeader("qg-(Sys)-4.677 fb^{-1}");
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
  c_xs_qg->SaveAs("Fit_Results/Exclusion_qg.png");
  c_xs_qg->SaveAs("Fit_Results/Exclusion_qg.eps");

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
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_gg->SetFillColor(0);
  l_xs_gg->SetFillStyle(0);
  l_xs_gg->SetHeader("gg-(Sys)-4.677 fb^{-1}");
  //  l_xs_gg->AddEntry(g_xs_HT_calo_gg_1010,"HT-calo 95% CL","pL");
  //  l_xs_gg->AddEntry(g_xs_HT_pf_gg_1010,"HT-PF 95% CL","pL");
  l_xs_gg->AddEntry(g_xs_HT_fat_gg_1010,"HT-fat 95% CL","pL");

  l_xs_gg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_gg->Update();
  c_xs_gg->SaveAs("Fit_Results/Exclusion_gg.png");
  c_xs_gg->SaveAs("Fit_Results/Exclusion_gg.eps");
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
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qq->SetFillColor(0);
  l_xs_qq->SetFillStyle(0);
  l_xs_qq->SetHeader("qq-(Sys)-4.677 fb^{-1}");
  //  l_xs_qq->AddEntry(g_xs_HT_calo_qq_1010,"HT-calo 95% CL","pL");
  //  l_xs_qq->AddEntry(g_xs_HT_pf_qq_1010,"HT-PF 95% CL","pL");
  l_xs_qq->AddEntry(g_xs_HT_fat_qq_1010,"HT-fat 95% CL","pL");
  l_xs_qq->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qq->Update();
  c_xs_qq->SaveAs("Fit_Results/Exclusion_qq.png");
  c_xs_qq->SaveAs("Fit_Results/Exclusion_qq.eps");
  
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
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_stat_fat->SetFillColor(0);
  l_xs_stat_fat->SetFillStyle(0);
  //  l_xs_stat_fat->SetHeader("CMS Preliminary");
  //  l_xs_stat_fat->AddEntry((TObject*)0,"4.677 fb^{-1}","");
  l_xs_stat_fat->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qg_1010,"Quark-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qq_1010,"Quark-Quark","pL");
  l_xs_stat_fat->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_fat->AddText("CMS (4.677 fb^{-1})"); 
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
 
  c_xs_stat_fat->SaveAs("Fit_Results/Exclusion_StatOnly.png");
  c_xs_stat_fat->SaveAs("Fit_Results/Exclusion_StatOnly.eps");

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
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_sys->SetFillColor(0);
  l_xs_sys->SetFillStyle(0);
  l_xs_sys->SetTextSize(0.03484321);
  //  l_xs_sys->SetHeader("CMS Preliminary");
  //  l_xs_sys->AddEntry((TObject*)0,"4.677 fb^{-1}","");
  l_xs_sys->SetHeader("95% CL Upper Limit (sys)");
  l_xs_sys->AddEntry(g_xs_HT_fat_gg_1010_sys,"Gluon-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qg_1010_sys,"Quark-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qq_1010_sys,"Quark-Quark","pL");
  l_xs_sys->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_sys->AddText("CMS (4.677 fb^{-1})"); 
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

  c_xs_sys->SaveAs("Fit_Results/Exclusion_Sys.png");
  c_xs_sys->SaveAs("Fit_Results/Exclusion_Sys.eps");

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
  l_xs_all_fat->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"4.677 fb^{-1}","")
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
  c_xs_all_fat->SaveAs("Fit_Results/Exclusion_Stat_vs_Sys.png");
  c_xs_all_fat->SaveAs("Fit_Results/Exclusion_Stat_vs_Sys.eps");

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
  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_fat->AddEntry((TObject*)0,"4.677 fb^{-1}","")
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

  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results/Exclusions_vs_Theory.png");
  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results/Exclusions_vs_Theory.eps");
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
  p_xs_qg_sys_E->AddText("CMS (4.677 fb^{-1})");
  p_xs_qg_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qg_sys_E->AddText("qg: with systematics");
  p_xs_qg_sys_E->Draw("sames");
	
  l_xs_qg_sys_E->SetFillColor(0);
  l_xs_qg_sys_E->SetFillStyle(0);
//  l_xs_qg_sys_E->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_qg_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qg_sys_E->AddEntry((TObject*)0,"4.677 fb^{-1}","")
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

  c_xs_qg_sys_E->SaveAs("Fit_Results/Exclusions_qg_withExpected.png");
  c_xs_qg_sys_E->SaveAs("Fit_Results/Exclusions_qg_withExpected.eps");



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
  p_xs_qq_sys_E->AddText("CMS (4.677 fb^{-1})");
  p_xs_qq_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qq_sys_E->AddText("qq: with systematics");
  p_xs_qq_sys_E->Draw("sames");

  l_xs_qq_sys_E->SetFillColor(0);
  l_xs_qq_sys_E->SetFillStyle(0);
  //  l_xs_qq_sys_E->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_qq_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_sys_E->AddEntry((TObject*)0,"4.677 fb^{-1}","")
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

  c_xs_qq_sys_E->SaveAs("Fit_Results/Exclusions_qq_withExpected.png");
  c_xs_qq_sys_E->SaveAs("Fit_Results/Exclusions_qq_withExpected.eps");

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
  p_xs_qg_stat_E->AddText("CMS (4.677 fb^{-1})");
  p_xs_qg_stat_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qg_stat_E->AddText("qg: statistical only");
  p_xs_qg_stat_E->Draw("sames");
	
  l_xs_qg_stat_E->SetFillColor(0);
  l_xs_qg_stat_E->SetFillStyle(0);
//  l_xs_qg_stat_E->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_qg_stat_E->SetHeader("CMS Preliminary");
  //  l_xs_qg_stat_E->AddEntry((TObject*)0,"4.677 fb^{-1}","")
  l_xs_qg_stat_E->AddEntry(g_xs_HT_fat_qg_1010,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_qg_stat_E->AddEntry(g_xs_HT_fat_qg_1010_stat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qg_stat_E->Draw("sames");
  l_xs_theory_qg->Draw("sames");

  c_xs_qg_stat_E->Update();

  c_xs_qg_stat_E->SaveAs("Fit_Results/Exclusions_qg_withExpected_statonly.png");
  c_xs_qg_stat_E->SaveAs("Fit_Results/Exclusions_qg_withExpected_statonly.eps");



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
  p_xs_qq_stat_E->AddText("CMS (4.677 fb^{-1})");
  p_xs_qq_stat_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qq_stat_E->AddText("qq: statistical only");
  p_xs_qq_stat_E->Draw("sames");

  l_xs_qq_stat_E->SetFillColor(0);
  l_xs_qq_stat_E->SetFillStyle(0);
  //  l_xs_qq_stat_E->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_qq_stat_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_stat_E->AddEntry((TObject*)0,"4.677 fb^{-1}","")
  l_xs_qq_stat_E->AddEntry(g_xs_HT_fat_qq_1010,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_d2,"-2 #sigma","pL");
  //  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_d1,"-1 #sigma","pL");
  l_xs_qq_stat_E->AddEntry(g_xs_HT_fat_qq_1010_stat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qq_stat_E->Draw("sames");

  l_xs_theory_qq->Draw("sames");
  
  c_xs_qq_stat_E->Update();

  c_xs_qq_stat_E->SaveAs("Fit_Results/Exclusions_qq_withExpected_statonly.png");
  c_xs_qq_stat_E->SaveAs("Fit_Results/Exclusions_qq_withExpected_statonly.eps");























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
  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (4.677 fb^{-1})");
  //  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry((TObject*)0,"4.677 fb^{-1}","")
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();

  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results/Exclusions_qg_SysErrorsDependance.png");
  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results/Exclusions_qg_SysErrorsDependance.eps");






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

  l_xs_comparison_bw_stat_sys_fat_norm->SetHeader("CMS (4.677 fb^{-1})");


  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
 l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_lumi_fat_norm,"Lumi Normal / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JES_fat_norm,"JES Normal / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_norm->AddEntry(g_xs_comparison_bw_stat_JER_fat_norm,"JER Normal / stat","pL");

  l_xs_comparison_bw_stat_sys_fat_norm->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_norm->Update();


  string sOut_png = "Fit_Results/Exclusions_qg_SysErrorsDependance_priorDependance.png";
  string sOut_eps = "Fit_Results/Exclusions_qg_SysErrorsDependance_priorDependance.eps";

  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat_norm->SaveAs(sOut_eps.c_str());

  c_xs_comparison_bw_stat_sys_fat_Bkg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,11.);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(noFirstGeVBin 95% CL #sigma) / (all bins 95% CL #sigma)");
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.60);
  vFrame->SetMinimum(0.5);
  vFrame->SetMaximum(1.3);
  // vFrame->SetLogy();

  g_xs_comparison_bw_stat_sys_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternativeB_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative1Up_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative2Up_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative3Up_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative1Do_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative2Do_fat->Draw("samePL");
  g_xs_comparison_bw_stat_bg_alternative3Do_fat->Draw("samePL");


  l_xs_comparison_bw_stat_sys_fat_Bkg->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg->SetFillStyle(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg1->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_fat_Bkg1->SetFillStyle(0);

  l_xs_comparison_bw_stat_sys_fat_Bkg->SetHeader("CMS (4.677 fb^{-1})");

  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_alternative_fat,"background alt. ABC/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg->AddEntry(g_xs_comparison_bw_stat_bg_alternativeB_fat,"background alt. AB/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative1Up_fat,"background alt. V1 Up/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative2Up_fat,"background alt. V2 Up/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative3Up_fat,"background alt. V3 Up/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative1Do_fat,"background alt. V1 Do/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative2Do_fat,"background alt. V2 Do/ stat","pL");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->AddEntry(g_xs_comparison_bw_stat_bg_alternative3Do_fat,"background alt. V3 Do/ stat","pL");


  l_xs_comparison_bw_stat_sys_fat_Bkg->Draw("sames");
  l_xs_comparison_bw_stat_sys_fat_Bkg1->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat_Bkg->Update();


  string sOut_png = "Fit_Results/Exclusions_qg_SysErrorsDependance_Bkg.png";
  string sOut_eps = "Fit_Results/Exclusions_qg_SysErrorsDependance_Bkg.eps";

  c_xs_comparison_bw_stat_sys_fat_Bkg->SaveAs(sOut_png.c_str());
  c_xs_comparison_bw_stat_sys_fat_Bkg->SaveAs(sOut_eps.c_str());



 


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
  l_xs_qg_compare->SetHeader("qg-(Stat.Only)-4.677 fb^{-1}");
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
  l_xs_gg_compare->SetHeader("gg-(Stat.Only)-4.677 fb^{-1}");
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
  l_xs_qq_compare->SetHeader("qq-(Stat.Only)-4.677 fb^{-1}");
  l_xs_qq_compare->AddEntry(g_xs_HT_fat_qq_compare,"HT-fat","pL");

  l_xs_qq_compare->Draw("sames");

  c_xs_qq_compare->Update();
  */
  // c_xs_rsg

  c_xs_rsg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(990.0,0.1,1410.0,4.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");

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

  g_xs_RSG_for_limit_sys_fat_E_m->Draw("sameC");
  g_xs_RSG_for_limit_fat->Draw("samePL");
  g_xs_HT_fat_gg_1010_sys->Draw("samePL");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");


  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_rsg->SetFillColor(0);
  l_xs_rsg->SetFillStyle(0);
  l_xs_rsg->Clear();
  //  l_xs_rsg->SetHeader("CMS Preliminary");
  //  l_xs_rsg->AddEntry((TObject*)0,"4.677 fb^{-1}","");
  l_xs_rsg->AddEntry(g_xs_HT_fat_qq_1010_sys,"HT-fat qq","pL");
  l_xs_rsg->AddEntry(g_xs_HT_fat_gg_1010_sys,"HT-fat gg","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_fat,"HT-fat w1*qq+w2*gg","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_sys_fat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_rsg->AddEntry(h_xs_RSG_for_limit_sys_fat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_rsg->AddEntry(h_xs_RSG_for_limit_sys_fat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");

  l_xs_rsg->Draw("sames");
  //  l_xs_theory->Draw("sames");

  p_xs_RSG_sys_E->SetFillColor(0);
  p_xs_RSG_sys_E->SetFillStyle(0);
  p_xs_RSG_sys_E->SetBorderSize(0);
  p_xs_RSG_sys_E->AddText("CMS (4.677 fb^{-1})");
  p_xs_RSG_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_RSG_sys_E->Draw("sames");


  c_xs_rsg->Update();
  c_xs_rsg->SaveAs("Fit_Results/Exclusions_ggqq_Graviton.png");
  c_xs_rsg->SaveAs("Fit_Results/Exclusions_ggqq_Graviton.eps");


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

