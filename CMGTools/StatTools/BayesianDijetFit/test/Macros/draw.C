#include <iostream>
#include <iomanip>

void draw() 
{
  std::cout << "begin" << std::endl;

  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");

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
  TCanvas *c_xs_stat_pf = new TCanvas("c_xs_stat_pf","c_xs_stat_pf",459,358,600,602);
  TCanvas *c_xs_stat_calo = new TCanvas("c_xs_stat_calo","c_xs_stat_calo",459,358,600,602);
  TCanvas *c_xs_sys = new TCanvas("c_xs_sys","c_xs_sys",459,358,600,602);
  TCanvas *c_xs_all_fat = new TCanvas("c_xs_all_fat","c_xs_all_fat",459,358,600,602);
  TCanvas *c_xs_all_pf = new TCanvas("c_xs_all_pf","c_xs_all_pf",459,358,600,602);
  TCanvas *c_xs_all_calo = new TCanvas("c_xs_all_calo","c_xs_all_calo",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_fat = new TCanvas("c_xs_comparison_bw_stat_sys_fat","c_xs_comparison_bw_stat_sys_fat",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_pf = new TCanvas("c_xs_comparison_bw_stat_sys_pf","c_xs_comparison_bw_stat_sys_pf",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_calo = new TCanvas("c_xs_comparison_bw_stat_sys_calo","c_xs_comparison_bw_stat_sys_calo",459,358,600,602);
  TCanvas *c_xs_gg_compare = new TCanvas("c_xs_gg_compare","c_xs_gg_compare",459,358,600,602);
  TCanvas *c_xs_qg_compare = new TCanvas("c_xs_qg_compare","c_xs_qg_compare",459,358,600,602);
  TCanvas *c_xs_qq_compare = new TCanvas("c_xs_qq_compare","c_xs_qq_compare",459,358,600,602);
  TCanvas *c_xs_comparison_bw_sys_theory_fat = new TCanvas("c_xs_comparison_bw_sys_theory_fat","c_xs_comparison_bw_sys_theory_fat",459,358,600,602);
  TCanvas *c_xs_comparison_bw_sys_theory_pf = new TCanvas("c_xs_comparison_bw_sys_theory_pf","c_xs_comparison_bw_sys_theory_pf",459,358,600,602);
  TCanvas *c_xs_comparison_bw_sys_theory_calo = new TCanvas("c_xs_comparison_bw_sys_theory_calo","c_xs_comparison_bw_sys_theory_calo",459,358,600,602);

  TCanvas *c_xs_qg_stat_E = new TCanvas("c_xs_qg_stat_E","c_xs_qg_stat_E",459,358,600,602);
  TCanvas *c_xs_qq_stat_E = new TCanvas("c_xs_qq_stat_E","c_xs_qq_stat_E",459,358,600,602);
  TCanvas *c_xs_rsg = new TCanvas("c_xs_rsg","c_xs_rsg",459,358,600,602);

  // definition of legends

  TLegend *l_xs_theory =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qg =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_qq =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");

  TLegend *l_xs_gg =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qg =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qq =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_stat_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_stat_pf =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_stat_calo =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_sys =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_all_fat =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_all_pf =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_all_calo =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_pf =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_calo =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_gg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qg_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_qq_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_fat =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_pf =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_calo =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_qg_stat_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_qq_stat_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_rsg =  new TLegend(0.5,0.6,0.8,0.9,NULL,"brNDC");

  TPaveText *p_xs_qg_stat_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");
  TPaveText *p_xs_qq_stat_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");

  TPaveText *p_xs_sys = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_fat = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_pf = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_calo = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");

  // settings of canvases

  c_xs_gg->SetLogy();
  c_xs_qg->SetLogy();
  c_xs_qq->SetLogy();
  c_xs_stat_fat->SetLogy();
  c_xs_stat_pf->SetLogy();
  c_xs_stat_calo->SetLogy();
  c_xs_sys->SetLogy();
  c_xs_all_fat->SetLogy();
  c_xs_all_pf->SetLogy();
  c_xs_all_calo->SetLogy();
  c_xs_comparison_bw_sys_theory_fat->SetLogy();
  c_xs_comparison_bw_sys_theory_pf->SetLogy();
  c_xs_comparison_bw_sys_theory_calo->SetLogy();
  c_xs_qg_stat_E->SetLogy();
  c_xs_qq_stat_E->SetLogy();

  const unsigned int en = 43;

  // theory values


  double mjj [en] ={ 800.0, 900.0, 1000.0, 1100.0, 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100., 2200., 2300., 2400., 2500., 2600., 2700., 2800., 2900., 3000., 3100., 3200., 3300., 3400., 3500., 3600., 3700., 3800., 3900., 4000., 4100., 4200., 4300., 4400., 4500.};

  double qstar [en] = {0.2355E+03, 0.1257E+03, 0.7005E+02, 0.4039E+02, 0.2394E+02,  0.1452E+02, 0.8982E+01,  0.5645E+01,  0.3596E+01,  0.2317E+01, 0.1507E+01, 0.9889E+00, 0.6531E+00, 0.4338E+00, 0.2896E+00, 0.1940E+00 ,  0.1304E+00, 0.8782E-01, 0.5925E-01,  0.4002E-01,  0.2704E-01, 0.1828E-01, 0.1234E-01, 0.8329E-02, 0.5613E-02,  0.3776E-02, 0.2535E-02, 0.1698E-02,  0.1135E-02, 0.7559E-03, 0.5021E-03, 0.3325E-03, 0.2195E-03, 0.1444E-03, 0.9478E-04, 0.6203E-04, 0.4050E-04, 0.2639E-04};

  double axigluon[en] = {0.1193E+03, 0.6750E+02, 0.3967E+02, 0.2400E+02, 0.1486E+02, 0.9370E+01, 0.5998E+01, 0.3887E+01, 0.2544E+01, 0.1678E+01, 0.1115E+01, 0.7442E+00, 0.4988E+00, 0.3354E+00, 0.2260E+00, 0.1525E+00, 0.1030E+00, 0.6949E-01, 0.4684E-01, 0.3152E-01, 0.2116E-01, 0.1415E-01, 0.9428E-02, 0.6250E-02, 0.4119E-02, 0.2698E-02, 0.1754E-02, 0.1131E-02, 0.7222E-03, 0.4568E-03, 0.2858E-03, 0.1767E-03, 0.1079E-03};

  double diquark[en] = {0.5435E+02, 0.3554E+02, 0.2393E+02, 0.1648E+02, 0.1154E+02, 0.8194E+01, 0.5877E+01, 0.4249E+01, 0.3090E+01, 0.2258E+01, 0.1656E+01, 0.1217E+01, 0.8953E+00, 0.6591E+00, 0.4852E+00, 0.3569E+00, 0.2622E+00, 0.1922E+00, 0.1406E+00, 0.1025E+00, 0.7449E-01, 0.5392E-01, 0.3885E-01, 0.2786E-01, 0.1987E-01, 0.1408E-01, 0.9920E-02, 0.6938E-02, 0.4815E-02, 0.3315E-02, 0.2261E-02, 0.1528E-02, 0.1022E-02};

  double string[en] = {0.4627E+04, 0.2485E+02, 0.1392E+04, 0.7879E+03, 473.13, 290.1, 177.6, 111.91, 72.12, 47.07, 31.05, 20.60, 13.82, 9.117, 6.244, 4.238, 2.881, 1.973, 1.367, 0.93418, 0.644897, 0.445018, 0.304044, 0.212039, 0.143917, 0.099195, 0.066999, 0.0462355, 0.0313552, 0.021399, 0.0141453, 0.0095593, 0.0064255, 0.004227117035341108, 0.002830943528441785, 0.001830831111740912, 0.0012006226542702247, 0.0007818600156759468, 0.0004978553320724371, 0.0003197273428031069, 0.00020157675764867537, 0.00012686973888958498, 0.00008018201235487667};

  double zprime[en] = {0.3427E+01, 0.1969E+01, 0.1172E+01, 0.7171E+00, 0.4486E+00, 0.2857E+00, 0.1845E+00, 0.1206E+00, 0.7961E-01, 0.5295E-01, 0.3545E-01, 0.2386E-01, 0.1611E-01, 0.1092E-01, 0.7413E-02, 0.5039E-02, 0.3426E-02, 0.2329E-02, 0.1580E-02, 0.1070E-02, 0.7231E-03, 0.4867E-03, 0.3261E-03, 0.2174E-03, 0.1440E-03, 0.9477E-04, 0.6190E-04, 0.4007E-04, 0.2570E-04, 0.1631E-04, 0.1024E-04, 0.6349E-05, 0.3889E-05};

  double wprime[en] = {0.6263E+01, 0.3661E+01, 0.2212E+01, 0.1372E+01, 0.8673E+00, 0.5568E+00, 0.3616E+00, 0.2369E+00, 0.1562E+00, 0.1034E+00, 0.6872E-01, 0.4572E-01, 0.3043E-01, 0.2023E-01, 0.1342E-01, 0.8884E-02, 0.5859E-02, 0.3847E-02, 0.2513E-02, 0.1632E-02, 0.1053E-02, 0.6744E-03, 0.4287E-03, 0.2702E-03, 0.1688E-03, 0.1044E-03, 0.6403E-04, 0.3886E-04, 0.2335E-04, 0.1390E-04, 0.8199E-05, 0.4796E-05, 0.2787E-05};

  double rsgraviton[en] = {0.3852E+01, 0.1961E+01, 0.1053E+01, 0.5905E+00, 0.3426E+00, 0.2044E+00, 0.1248E+00, 0.7770E-01, 0.4911E-01, 0.3145E-01, 0.2036E-01, 0.1330E-01, 0.8743E-02, 0.5781E-02, 0.3840E-02, 0.2559E-02, 0.1708E-02, 0.1142E-02, 0.7635E-03, 0.5101E-03, 0.3402E-03, 0.2264E-03, 0.1501E-03, 0.9913E-04, 0.6512E-04, 0.4253E-04, 0.2759E-04, 0.1775E-04, 0.1133E-04, 0.7157E-05, 0.4475E-05, 0.2766E-05, 0.1689E-05};

  double x[32] = {1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0, 3000.0, 3100.0, 3200.0, 3300.0, 3400.0, 3500.0, 3600.0, 3700.0, 3800.0, 3900.0, 4000.0, 4100.0};



  double gg_ratio[3] = {0.5561,0.5116,0.4671};
  double qq_ratio[3] = {0.4439,0.4884,0.5329};

  double xs_HT_pf_qg_compare[32], xs_HT_pf_gg_compare[32], xs_HT_pf_qq_compare[32], xs_HT_fat_qg_compare[32], xs_HT_fat_gg_compare[32], xs_HT_fat_qq_compare[32], xs_comparison_bw_stat_bg_fat[32], xs_comparison_bw_stat_lumi_fat[32], xs_comparison_bw_stat_JES_fat[32], xs_comparison_bw_stat_JER_fat[32], xs_comparison_bw_stat_sys_fat[32], xs_comparison_bw_stat_bg_pf[32], xs_comparison_bw_stat_lumi_pf[32], xs_comparison_bw_stat_JES_pf[32], xs_comparison_bw_stat_JER_pf[32], xs_comparison_bw_stat_sys_pf[32], xs_comparison_bw_stat_bg_calo[32], xs_comparison_bw_stat_lumi_calo[32], xs_comparison_bw_stat_JES_calo[32], xs_comparison_bw_stat_JER_calo[32], xs_comparison_bw_stat_sys_calo[32], xs_RSG_for_limit_fat[3], xs_qstar_over_qg_fat[32], xs_string_over_qg_fat[32], xs_diquark_over_qq_fat[32], xs_axigluon_over_qq_fat[32], xs_zprime_over_qq_fat[32], xs_wprime_over_qq_fat[32], xs_rsg_over_gg_fat[32], xs_RSG_for_limit_pf[3], xs_qstar_over_qg_pf[32], xs_string_over_qg_pf[32], xs_diquark_over_qq_pf[32], xs_axigluon_over_qq_pf[32], xs_zprime_over_qq_pf[32], xs_wprime_over_qq_pf[32], xs_rsg_over_gg_pf[32], xs_RSG_for_limit_calo[3], xs_qstar_over_qg_calo[32], xs_string_over_qg_calo[32], xs_diquark_over_qq_calo[32], xs_axigluon_over_qq_calo[32], xs_zprime_over_qq_calo[32], xs_wprime_over_qq_calo[32], xs_rsg_over_gg_calo[32];


  /*

  std::cout << "wide stat only" << std::endl;
  for (int  i = 0 ; i < 32 ; i++) 
    {
      std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qq_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qg_1010[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010[i] << " \\\\" << std::endl;
      
    }
  std::cout << "pf stat only" << std::endl;
  for (int  i = 0 ; i < 32 ; i++) 
    {
      std::cout << std::fixed << setprecision(2) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_pf_qq_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_pf_qg_1010[i] <<" & " << std::fixed << setprecision(5) << xs_HT_pf_gg_1010[i] << " \\\\" << std::endl;
      
    }

  std::cout << "calo stat only" << std::endl;
  for (int  i = 0 ; i < 32 ; i++) 
    {
      std::cout << std::fixed << setprecision(2) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_calo_qq_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_calo_qg_1010[i] <<" & " << std::fixed << setprecision(5) << xs_HT_calo_gg_1010[i] << " \\\\" << std::endl;
      
    }

  std::cout << "wide sys included" << std::endl;
  for (int  i = 0 ; i < 32 ; i++) 
    {
      std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_qq_1010_sys[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_qg_1010_sys[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_gg_1010_sys[i] << " \\\\" << std::endl;
      
    }
  std::cout << "pf sys included" << std::endl;
  for (int  i = 0 ; i < 32 ; i++) 
    {
      std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_pf_qq_1010_sys[i]<<" & " << std::fixed << setprecision(5) << xs_HT_pf_qg_1010_sys[i] <<" & " << std::fixed << setprecision(5) << xs_HT_pf_gg_1010_sys[i] << " \\\\" << std::endl;
      
    }

  std::cout << "calo sys included" << std::endl;
  for (int  i = 0 ; i < 32 ; i++) 
    {
      std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_calo_qq_1010_sys[i]<<" & " << std::fixed << setprecision(5) << xs_HT_calo_qg_1010_sys[i] <<" & " << std::fixed << setprecision(5) << xs_HT_calo_gg_1010_sys[i] << " \\\\" << std::endl;
      
    }

  */


////////////////// Histogram definition /////////////////////
/////////////////////////////////////////////////////////////


  // expected limit for qg

  TH1F *h_xs_HT_fat_qg_1010_stat_E_d2 = new TH1F("","",32,950.0,4150.0);
  TH1F *h_xs_HT_fat_qg_1010_stat_E_d1 = new TH1F("","",32,950.0,4150.0);
  TH1F *h_xs_HT_fat_qg_1010_stat_E_u1 = new TH1F("","",32,950.0,4150.0);
  TH1F *h_xs_HT_fat_qg_1010_stat_E_u2 = new TH1F("","",32,950.0,4150.0);

  // expected limit for qq

  TH1F *h_xs_HT_fat_qq_1010_stat_E_d2 = new TH1F("","",32,950.0,4150.0);
  TH1F *h_xs_HT_fat_qq_1010_stat_E_d1 = new TH1F("","",32,950.0,4150.0);
  TH1F *h_xs_HT_fat_qq_1010_stat_E_u1 = new TH1F("","",32,950.0,4150.0);
  TH1F *h_xs_HT_fat_qq_1010_stat_E_u2 = new TH1F("","",32,950.0,4150.0);

 for (int i = 0; i<32; i++) {
   
   if (i<16)     std::cout << i+10 << " & " << std::fixed << setprecision(3) << xs_HT_fat_qq_1010_sys[i] << " & " << xs_HT_fat_qg_1010_sys[i] << " & " << xs_HT_fat_gg_1010_sys[i] << " & " << i+26 << " & " << std::fixed << setprecision(4) << xs_HT_fat_qq_1010_sys[i+16] << " & " << xs_HT_fat_qg_1010_sys[i+16] << " & " << xs_HT_fat_gg_1010_sys[i+16] << " \\\\  " << std::endl;
   if (i<3)
     {
       xs_RSG_for_limit_fat[i] = xs_HT_fat_gg_1010_sys[i]*gg_ratio[i] + xs_HT_fat_qq_1010_sys[i]*qq_ratio[i];
       xs_rsg_over_gg_fat[i] = rsgraviton[i+2]/xs_RSG_for_limit_fat[i];
       xs_RSG_for_limit_pf[i] = xs_HT_pf_gg_1010_sys[i]*gg_ratio[i] + xs_HT_pf_qq_1010_sys[i]*qq_ratio[i];
       xs_rsg_over_gg_pf[i] = rsgraviton[i+2]/xs_RSG_for_limit_pf[i];
       xs_RSG_for_limit_calo[i] = xs_HT_calo_gg_1010_sys[i]*gg_ratio[i] + xs_HT_calo_qq_1010_sys[i]*qq_ratio[i];
       xs_rsg_over_gg_calo[i] = rsgraviton[i+2]/xs_RSG_for_limit_calo[i];
     }
     
   xs_HT_pf_qq_compare[i] = xs_HT_pf_qq_1010[i]/xs_HT_calo_qq_1010[i];
   xs_HT_fat_qq_compare[i] = xs_HT_fat_qq_1010[i]/xs_HT_calo_qq_1010[i];
   xs_HT_pf_qg_compare[i] = xs_HT_pf_qg_1010[i]/xs_HT_calo_qg_1010[i];
   xs_HT_fat_qg_compare[i] = xs_HT_fat_qg_1010[i]/xs_HT_calo_qg_1010[i];
   xs_HT_pf_gg_compare[i] = xs_HT_pf_gg_1010[i]/xs_HT_calo_gg_1010[i];
   xs_HT_fat_gg_compare[i] = xs_HT_fat_gg_1010[i]/xs_HT_calo_gg_1010[i];

   xs_comparison_bw_stat_bg_fat[i] = xs_HT_fat_qg_1010_bg[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_lumi_fat[i] = xs_HT_fat_qg_1010_lumi[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JES_fat[i] = xs_HT_fat_qg_1010_JES[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_JER_fat[i] = xs_HT_fat_qg_1010_JER[i]/xs_HT_fat_qg_1010[i];
   xs_comparison_bw_stat_sys_fat[i] = xs_HT_fat_qg_1010_sys[i]/xs_HT_fat_qg_1010[i];

   xs_comparison_bw_stat_bg_pf[i] = xs_HT_pf_qg_1010_bg[i]/xs_HT_pf_qg_1010[i];
   xs_comparison_bw_stat_lumi_pf[i] = xs_HT_pf_qg_1010_lumi[i]/xs_HT_pf_qg_1010[i];
   xs_comparison_bw_stat_JES_pf[i] = xs_HT_pf_qg_1010_JES[i]/xs_HT_pf_qg_1010[i];
   xs_comparison_bw_stat_JER_pf[i] = xs_HT_pf_qg_1010_JER[i]/xs_HT_pf_qg_1010[i];
   xs_comparison_bw_stat_sys_pf[i] = xs_HT_pf_qg_1010_sys[i]/xs_HT_pf_qg_1010[i];

   xs_comparison_bw_stat_bg_calo[i] = xs_HT_calo_qg_1010_bg[i]/xs_HT_calo_qg_1010[i];
   xs_comparison_bw_stat_lumi_calo[i] = xs_HT_calo_qg_1010_lumi[i]/xs_HT_calo_qg_1010[i];
   xs_comparison_bw_stat_JES_calo[i] = xs_HT_calo_qg_1010_JES[i]/xs_HT_calo_qg_1010[i];
   xs_comparison_bw_stat_JER_calo[i] = xs_HT_calo_qg_1010_JER[i]/xs_HT_calo_qg_1010[i];
   xs_comparison_bw_stat_sys_calo[i] = xs_HT_calo_qg_1010_sys[i]/xs_HT_calo_qg_1010[i];

   xs_qstar_over_qg_fat[i] = qstar[i+2]/xs_HT_fat_qg_1010_sys[i];
   xs_string_over_qg_fat[i] = string[i+2]/xs_HT_fat_qg_1010_sys[i];
   xs_diquark_over_qq_fat[i] = diquark[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_axigluon_over_qq_fat[i] = axigluon[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_zprime_over_qq_fat[i] = zprime[i+2]/xs_HT_fat_qq_1010_sys[i];
   xs_wprime_over_qq_fat[i] = wprime[i+2]/xs_HT_fat_qq_1010_sys[i];
   
   xs_qstar_over_qg_pf[i] = qstar[i+2]/xs_HT_pf_qg_1010_sys[i];
   xs_string_over_qg_pf[i] = string[i+2]/xs_HT_pf_qg_1010_sys[i];
   xs_diquark_over_qq_pf[i] = diquark[i+2]/xs_HT_pf_qq_1010_sys[i];
   xs_axigluon_over_qq_pf[i] = axigluon[i+2]/xs_HT_pf_qq_1010_sys[i];
   xs_zprime_over_qq_pf[i] = zprime[i+2]/xs_HT_pf_qq_1010_sys[i];
   xs_wprime_over_qq_pf[i] = wprime[i+2]/xs_HT_pf_qq_1010_sys[i];
   
   xs_qstar_over_qg_calo[i] = qstar[i+2]/xs_HT_calo_qg_1010_sys[i];
   xs_string_over_qg_calo[i] = string[i+2]/xs_HT_calo_qg_1010_sys[i];
   xs_diquark_over_qq_calo[i] = diquark[i+2]/xs_HT_calo_qq_1010_sys[i];
   xs_axigluon_over_qq_calo[i] = axigluon[i+2]/xs_HT_calo_qq_1010_sys[i];
   xs_zprime_over_qq_calo[i] = zprime[i+2]/xs_HT_calo_qq_1010_sys[i];
   xs_wprime_over_qq_calo[i] = wprime[i+2]/xs_HT_calo_qq_1010_sys[i];
   
   h_xs_HT_fat_qg_1010_stat_E_d2->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_d2[i]);
   h_xs_HT_fat_qg_1010_stat_E_d1->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_d1[i]);
   h_xs_HT_fat_qg_1010_stat_E_u1->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_u1[i]);
   h_xs_HT_fat_qg_1010_stat_E_u2->SetBinContent(i+1,xs_HT_fat_qg_1010_stat_E_u2[i]);

   h_xs_HT_fat_qq_1010_stat_E_d2->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_d2[i]);
   h_xs_HT_fat_qq_1010_stat_E_d1->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_d1[i]);
   h_xs_HT_fat_qq_1010_stat_E_u1->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_u1[i]);
   h_xs_HT_fat_qq_1010_stat_E_u2->SetBinContent(i+1,xs_HT_fat_qq_1010_stat_E_u2[i]);

 }

//////////////////////////////////////////////////////
////////////////// TGraph define /////////////////////
//////////////////////////////////////////////////////

 // stat only 

  g_xs_HT_calo_qg_1010 = new TGraph(32,x,xs_HT_calo_qg_1010);
  g_xs_HT_calo_gg_1010 = new TGraph(32,x,xs_HT_calo_gg_1010);
  g_xs_HT_calo_qq_1010 = new TGraph(32,x,xs_HT_calo_qq_1010);

  g_xs_HT_pf_qg_1010 = new TGraph(32,x,xs_HT_pf_qg_1010);
  g_xs_HT_pf_gg_1010 = new TGraph(32,x,xs_HT_pf_gg_1010);
  g_xs_HT_pf_qq_1010 = new TGraph(32,x,xs_HT_pf_qq_1010);

  g_xs_HT_fat_qg_1010 = new TGraph(32,x,xs_HT_fat_qg_1010);
  g_xs_HT_fat_gg_1010 = new TGraph(32,x,xs_HT_fat_gg_1010);
  g_xs_HT_fat_qq_1010 = new TGraph(32,x,xs_HT_fat_qq_1010);

  // all sys inlcuded

  g_xs_HT_fat_qg_1010_sys = new TGraph(32,x,xs_HT_fat_qg_1010_sys);
  g_xs_HT_fat_gg_1010_sys = new TGraph(32,x,xs_HT_fat_gg_1010_sys);
  g_xs_HT_fat_qq_1010_sys = new TGraph(32,x,xs_HT_fat_qq_1010_sys);
  g_xs_HT_pf_qg_1010_sys = new TGraph(32,x,xs_HT_pf_qg_1010_sys);
  g_xs_HT_pf_gg_1010_sys = new TGraph(32,x,xs_HT_pf_gg_1010_sys);
  g_xs_HT_pf_qq_1010_sys = new TGraph(32,x,xs_HT_pf_qq_1010_sys);
  g_xs_HT_calo_qg_1010_sys = new TGraph(32,x,xs_HT_calo_qg_1010_sys);
  g_xs_HT_calo_gg_1010_sys = new TGraph(32,x,xs_HT_calo_gg_1010_sys);
  g_xs_HT_calo_qq_1010_sys = new TGraph(32,x,xs_HT_calo_qq_1010_sys);

  // fractional differnce only

  g_xs_comparison_bw_stat_bg_fat = new TGraph(32,x,xs_comparison_bw_stat_bg_fat);
  g_xs_comparison_bw_stat_lumi_fat = new TGraph(32,x,xs_comparison_bw_stat_lumi_fat);
  g_xs_comparison_bw_stat_JES_fat = new TGraph(32,x,xs_comparison_bw_stat_JES_fat);
  g_xs_comparison_bw_stat_JER_fat = new TGraph(32,x,xs_comparison_bw_stat_JER_fat);
  g_xs_comparison_bw_stat_sys_fat = new TGraph(32,x,xs_comparison_bw_stat_sys_fat);

  g_xs_comparison_bw_stat_bg_pf = new TGraph(32,x,xs_comparison_bw_stat_bg_pf);
  g_xs_comparison_bw_stat_lumi_pf = new TGraph(32,x,xs_comparison_bw_stat_lumi_pf);
  g_xs_comparison_bw_stat_JES_pf = new TGraph(32,x,xs_comparison_bw_stat_JES_pf);
  g_xs_comparison_bw_stat_JER_pf = new TGraph(32,x,xs_comparison_bw_stat_JER_pf);
  g_xs_comparison_bw_stat_sys_pf = new TGraph(32,x,xs_comparison_bw_stat_sys_pf);

  g_xs_comparison_bw_stat_bg_calo = new TGraph(32,x,xs_comparison_bw_stat_bg_calo);
  g_xs_comparison_bw_stat_lumi_calo = new TGraph(32,x,xs_comparison_bw_stat_lumi_calo);
  g_xs_comparison_bw_stat_JES_calo = new TGraph(32,x,xs_comparison_bw_stat_JES_calo);
  g_xs_comparison_bw_stat_JER_calo = new TGraph(32,x,xs_comparison_bw_stat_JER_calo);
  g_xs_comparison_bw_stat_sys_calo = new TGraph(32,x,xs_comparison_bw_stat_sys_calo);

  // theory value

  g_xs_qstar = new TGraph(en,mjj,qstar);
  g_xs_axigluon = new TGraph(en,mjj,axigluon);
  g_xs_diquark = new TGraph(en,mjj,diquark);
  g_xs_string = new TGraph(en,mjj,string);
  g_xs_zprime = new TGraph(9,mjj,zprime);
  g_xs_wprime = new TGraph(11,mjj,wprime);
  g_xs_rsgraviton = new TGraph(8,mjj,rsgraviton);

  // comparison b/w different Jet combinations.

  g_xs_HT_pf_qg_compare = new TGraph(32,x,xs_HT_pf_qg_compare);
  g_xs_HT_pf_gg_compare = new TGraph(32,x,xs_HT_pf_gg_compare);
  g_xs_HT_pf_qq_compare = new TGraph(32,x,xs_HT_pf_qq_compare);
  g_xs_HT_fat_qg_compare = new TGraph(32,x,xs_HT_fat_qg_compare);
  g_xs_HT_fat_gg_compare = new TGraph(32,x,xs_HT_fat_gg_compare);
  g_xs_HT_fat_qq_compare = new TGraph(32,x,xs_HT_fat_qq_compare);
  
  // comparison b/w sys.included cross section value and theory

   g_xs_qstar_over_qg_fat = new TGraph(32,x,xs_qstar_over_qg_fat);
   g_xs_string_over_qg_fat = new TGraph(32,x,xs_string_over_qg_fat);
   g_xs_diquark_over_qq_fat = new TGraph(31,x,xs_diquark_over_qq_fat);
   g_xs_axigluon_over_qq_fat = new TGraph(31,x,xs_axigluon_over_qq_fat);
   g_xs_zprime_over_qq_fat = new TGraph(32,x,xs_zprime_over_qq_fat);
   g_xs_wprime_over_qq_fat = new TGraph(32,x,xs_wprime_over_qq_fat);
   g_xs_rsg_over_gg_fat = new TGraph(3,x,xs_rsg_over_gg_fat);

   g_xs_qstar_over_qg_pf = new TGraph(32,x,xs_qstar_over_qg_pf);
   g_xs_string_over_qg_pf = new TGraph(32,x,xs_string_over_qg_pf);
   g_xs_diquark_over_qq_pf = new TGraph(31,x,xs_diquark_over_qq_pf);
   g_xs_axigluon_over_qq_pf = new TGraph(31,x,xs_axigluon_over_qq_pf);
   g_xs_zprime_over_qq_pf = new TGraph(32,x,xs_zprime_over_qq_pf);
   g_xs_wprime_over_qq_pf = new TGraph(32,x,xs_wprime_over_qq_pf);
   g_xs_rsg_over_gg_pf = new TGraph(3,x,xs_rsg_over_gg_pf);

   g_xs_qstar_over_qg_calo = new TGraph(32,x,xs_qstar_over_qg_calo);
   g_xs_string_over_qg_calo = new TGraph(32,x,xs_string_over_qg_calo);
   g_xs_diquark_over_qq_calo = new TGraph(31,x,xs_diquark_over_qq_calo);
   g_xs_axigluon_over_qq_calo = new TGraph(31,x,xs_axigluon_over_qq_calo);
   g_xs_zprime_over_qq_calo = new TGraph(32,x,xs_zprime_over_qq_calo);
   g_xs_wprime_over_qq_calo = new TGraph(32,x,xs_wprime_over_qq_calo);
   g_xs_rsg_over_gg_calo = new TGraph(3,x,xs_rsg_over_gg_calo);


  // expected limit for qg
  g_xs_HT_fat_qg_1010_stat_E_m = new TGraph(32,x,xs_HT_fat_qg_1010_stat_E_m);

  // expected limit for qq
  g_xs_HT_fat_qq_1010_stat_E_m = new TGraph(32,x,xs_HT_fat_qq_1010_stat_E_m);

  // RSG try

  double x_RSG[3] = {1000.0, 1100.0, 1200.0};
  g_xs_RSG_for_limit_fat = new TGraph(3,x_RSG,xs_RSG_for_limit_fat);

//////////////////////////////////////////////////////
////////////////// TGraph design /////////////////////
//////////////////////////////////////////////////////

// stat only

  g_xs_HT_calo_qg_1010->SetLineColor(1);
  g_xs_HT_calo_qg_1010->SetLineStyle(6);
  g_xs_HT_calo_qg_1010->SetLineWidth(3);
  g_xs_HT_calo_qg_1010->SetMarkerStyle(20);
  g_xs_HT_calo_qg_1010->SetMarkerColor(1);

  g_xs_HT_calo_gg_1010->SetLineColor(1);
  g_xs_HT_calo_gg_1010->SetLineStyle(6);
  g_xs_HT_calo_gg_1010->SetLineWidth(3);
  g_xs_HT_calo_gg_1010->SetMarkerStyle(20);
  g_xs_HT_calo_gg_1010->SetMarkerColor(1);

  g_xs_HT_calo_qq_1010->SetLineColor(1);
  g_xs_HT_calo_qq_1010->SetLineStyle(6);
  g_xs_HT_calo_qq_1010->SetLineWidth(3);
  g_xs_HT_calo_qq_1010->SetMarkerStyle(20);
  g_xs_HT_calo_qq_1010->SetMarkerColor(1);

  g_xs_HT_pf_qg_1010->SetLineColor(2);
  g_xs_HT_pf_qg_1010->SetLineStyle(6);
  g_xs_HT_pf_qg_1010->SetLineWidth(3);
  g_xs_HT_pf_qg_1010->SetMarkerStyle(24);
  g_xs_HT_pf_qg_1010->SetMarkerColor(2);

  g_xs_HT_pf_gg_1010->SetLineColor(2);
  g_xs_HT_pf_gg_1010->SetLineStyle(6);
  g_xs_HT_pf_gg_1010->SetLineWidth(3);
  g_xs_HT_pf_gg_1010->SetMarkerStyle(24);
  g_xs_HT_pf_gg_1010->SetMarkerColor(2);

  g_xs_HT_pf_qq_1010->SetLineColor(2);
  g_xs_HT_pf_qq_1010->SetLineStyle(6);
  g_xs_HT_pf_qq_1010->SetLineWidth(3);
  g_xs_HT_pf_qq_1010->SetMarkerStyle(24);
  g_xs_HT_pf_qq_1010->SetMarkerColor(2);

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

  // HT-pf all sys

  g_xs_HT_pf_qg_1010_sys->SetLineColor(2);
  g_xs_HT_pf_qg_1010_sys->SetLineStyle(1);
  g_xs_HT_pf_qg_1010_sys->SetLineWidth(3);
  g_xs_HT_pf_qg_1010_sys->SetMarkerStyle(24);
  g_xs_HT_pf_qg_1010_sys->SetMarkerColor(2);

  g_xs_HT_pf_gg_1010_sys->SetLineColor(ci_g);
  g_xs_HT_pf_gg_1010_sys->SetLineStyle(1);
  g_xs_HT_pf_gg_1010_sys->SetLineWidth(3);
  g_xs_HT_pf_gg_1010_sys->SetMarkerStyle(24);
  g_xs_HT_pf_gg_1010_sys->SetMarkerColor(ci_g);

  g_xs_HT_pf_qq_1010_sys->SetLineColor(4);
  g_xs_HT_pf_qq_1010_sys->SetLineStyle(1);
  g_xs_HT_pf_qq_1010_sys->SetLineWidth(3);
  g_xs_HT_pf_qq_1010_sys->SetMarkerStyle(24);
  g_xs_HT_pf_qq_1010_sys->SetMarkerColor(4);

  // HT-calo all sys

  g_xs_HT_calo_qg_1010_sys->SetLineColor(2);
  g_xs_HT_calo_qg_1010_sys->SetLineStyle(1);
  g_xs_HT_calo_qg_1010_sys->SetLineWidth(3);
  g_xs_HT_calo_qg_1010_sys->SetMarkerStyle(20);
  g_xs_HT_calo_qg_1010_sys->SetMarkerColor(2);

  g_xs_HT_calo_gg_1010_sys->SetLineColor(ci_g);
  g_xs_HT_calo_gg_1010_sys->SetLineStyle(1);
  g_xs_HT_calo_gg_1010_sys->SetLineWidth(3);
  g_xs_HT_calo_gg_1010_sys->SetMarkerStyle(24);
  g_xs_HT_calo_gg_1010_sys->SetMarkerColor(ci_g);

  g_xs_HT_calo_qq_1010_sys->SetLineColor(4);
  g_xs_HT_calo_qq_1010_sys->SetLineStyle(1);
  g_xs_HT_calo_qq_1010_sys->SetLineWidth(3);
  g_xs_HT_calo_qq_1010_sys->SetMarkerStyle(25);
  g_xs_HT_calo_qq_1010_sys->SetMarkerColor(4);

  // Expected HT-qg stat only

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

  // Expected HT-qq stat only

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

  g_xs_comparison_bw_stat_sys_fat->SetLineColor(1);
  g_xs_comparison_bw_stat_sys_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_sys_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_sys_fat->SetMarkerStyle(20);
  g_xs_comparison_bw_stat_sys_fat->SetMarkerColor(1);

  // Fractional difference b/w sys and stat.(pf)

  g_xs_comparison_bw_stat_bg_pf->SetLineColor(2);
  g_xs_comparison_bw_stat_bg_pf->SetLineStyle(1);
  g_xs_comparison_bw_stat_bg_pf->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_pf->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg_pf->SetMarkerColor(2);

  g_xs_comparison_bw_stat_lumi_pf->SetLineColor(ci_g);
  g_xs_comparison_bw_stat_lumi_pf->SetLineStyle(1);
  g_xs_comparison_bw_stat_lumi_pf->SetLineWidth(3);
  g_xs_comparison_bw_stat_lumi_pf->SetMarkerStyle(25);
  g_xs_comparison_bw_stat_lumi_pf->SetMarkerColor(ci_g);

  g_xs_comparison_bw_stat_JES_pf->SetLineColor(4);
  g_xs_comparison_bw_stat_JES_pf->SetLineStyle(1);
  g_xs_comparison_bw_stat_JES_pf->SetLineWidth(3);
  g_xs_comparison_bw_stat_JES_pf->SetMarkerStyle(26);
  g_xs_comparison_bw_stat_JES_pf->SetMarkerColor(4);

  g_xs_comparison_bw_stat_JER_pf->SetLineColor(46);
  g_xs_comparison_bw_stat_JER_pf->SetLineStyle(1);
  g_xs_comparison_bw_stat_JER_pf->SetLineWidth(3);
  g_xs_comparison_bw_stat_JER_pf->SetMarkerStyle(32);
  g_xs_comparison_bw_stat_JER_pf->SetMarkerColor(46);

  g_xs_comparison_bw_stat_sys_pf->SetLineColor(1);
  g_xs_comparison_bw_stat_sys_pf->SetLineStyle(1);
  g_xs_comparison_bw_stat_sys_pf->SetLineWidth(3);
  g_xs_comparison_bw_stat_sys_pf->SetMarkerStyle(20);
  g_xs_comparison_bw_stat_sys_pf->SetMarkerColor(1);

  // Fractional difference b/w sys and stat.(calo)

  g_xs_comparison_bw_stat_bg_calo->SetLineColor(2);
  g_xs_comparison_bw_stat_bg_calo->SetLineStyle(1);
  g_xs_comparison_bw_stat_bg_calo->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_calo->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg_calo->SetMarkerColor(2);

  g_xs_comparison_bw_stat_lumi_calo->SetLineColor(ci_g);
  g_xs_comparison_bw_stat_lumi_calo->SetLineStyle(1);
  g_xs_comparison_bw_stat_lumi_calo->SetLineWidth(3);
  g_xs_comparison_bw_stat_lumi_calo->SetMarkerStyle(25);
  g_xs_comparison_bw_stat_lumi_calo->SetMarkerColor(ci_g);

  g_xs_comparison_bw_stat_JES_calo->SetLineColor(4);
  g_xs_comparison_bw_stat_JES_calo->SetLineStyle(1);
  g_xs_comparison_bw_stat_JES_calo->SetLineWidth(3);
  g_xs_comparison_bw_stat_JES_calo->SetMarkerStyle(26);
  g_xs_comparison_bw_stat_JES_calo->SetMarkerColor(4);

  g_xs_comparison_bw_stat_JER_calo->SetLineColor(46);
  g_xs_comparison_bw_stat_JER_calo->SetLineStyle(1);
  g_xs_comparison_bw_stat_JER_calo->SetLineWidth(3);
  g_xs_comparison_bw_stat_JER_calo->SetMarkerStyle(32);
  g_xs_comparison_bw_stat_JER_calo->SetMarkerColor(46);

  g_xs_comparison_bw_stat_sys_calo->SetLineColor(1);
  g_xs_comparison_bw_stat_sys_calo->SetLineStyle(1);
  g_xs_comparison_bw_stat_sys_calo->SetLineWidth(3);
  g_xs_comparison_bw_stat_sys_calo->SetMarkerStyle(20);
  g_xs_comparison_bw_stat_sys_calo->SetMarkerColor(1);

  // Fractional difference b/w differnt jet type and dataset.

  g_xs_HT_pf_qg_compare->SetLineColor(2);
  g_xs_HT_pf_qg_compare->SetLineStyle(6);
  g_xs_HT_pf_qg_compare->SetLineWidth(3);
  g_xs_HT_pf_qg_compare->SetMarkerStyle(24);
  g_xs_HT_pf_qg_compare->SetMarkerColor(2);

  g_xs_HT_pf_gg_compare->SetLineColor(2);
  g_xs_HT_pf_gg_compare->SetLineStyle(6);
  g_xs_HT_pf_gg_compare->SetLineWidth(3);
  g_xs_HT_pf_gg_compare->SetMarkerStyle(24);
  g_xs_HT_pf_gg_compare->SetMarkerColor(2);

  g_xs_HT_pf_qq_compare->SetLineColor(2);
  g_xs_HT_pf_qq_compare->SetLineStyle(6);
  g_xs_HT_pf_qq_compare->SetLineWidth(3);
  g_xs_HT_pf_qq_compare->SetMarkerStyle(24);
  g_xs_HT_pf_qq_compare->SetMarkerColor(2);

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

  g_xs_string_over_qg_pf->SetLineColor(9);
  g_xs_string_over_qg_pf->SetLineWidth(3);

  g_xs_qstar_over_qg_pf->SetLineColor(1);
  g_xs_qstar_over_qg_pf->SetLineWidth(3);

  g_xs_axigluon_over_qq_pf->SetLineColor(7);
  g_xs_axigluon_over_qq_pf->SetLineWidth(3);
  
  g_xs_diquark_over_qq_pf->SetLineColor(8);
  g_xs_diquark_over_qq_pf->SetLineWidth(3);
  
  g_xs_wprime_over_qq_pf->SetLineColor(46);
  g_xs_wprime_over_qq_pf->SetLineWidth(3);

  g_xs_zprime_over_qq_pf->SetLineColor(38);
  g_xs_zprime_over_qq_pf->SetLineWidth(3);

  g_xs_rsg_over_gg_pf->SetLineColor(29);
  g_xs_rsg_over_gg_pf->SetLineWidth(3);

  g_xs_string_over_qg_calo->SetLineColor(9);
  g_xs_string_over_qg_calo->SetLineWidth(3);

  g_xs_qstar_over_qg_calo->SetLineColor(1);
  g_xs_qstar_over_qg_calo->SetLineWidth(3);

  g_xs_axigluon_over_qq_calo->SetLineColor(7);
  g_xs_axigluon_over_qq_calo->SetLineWidth(3);
  
  g_xs_diquark_over_qq_calo->SetLineColor(8);
  g_xs_diquark_over_qq_calo->SetLineWidth(3);
  
  g_xs_wprime_over_qq_calo->SetLineColor(46);
  g_xs_wprime_over_qq_calo->SetLineWidth(3);

  g_xs_zprime_over_qq_calo->SetLineColor(38);
  g_xs_zprime_over_qq_calo->SetLineWidth(3);

  g_xs_rsg_over_gg_calo->SetLineColor(29);
  g_xs_rsg_over_gg_calo->SetLineWidth(3);

//////////////////////////////////////////////////////
///////////////// drawing canvas /////////////////////
//////////////////////////////////////////////////////


  // c_xs_qg

  c_xs_qg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_calo_qg_1010->Draw("PL");
  g_xs_HT_pf_qg_1010->Draw("samePL");
  g_xs_HT_fat_qg_1010->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qg->SetFillColor(0);
  l_xs_qg->SetFillStyle(0);
  l_xs_qg->SetHeader("qg-(Stat.Only)-1.0fb^{-1}");
  l_xs_qg->AddEntry(g_xs_HT_calo_qg_1010,"HT-calo 95% CL","pL");
  l_xs_qg->AddEntry(g_xs_HT_pf_qg_1010,"HT-PF 95% CL","pL");
  l_xs_qg->AddEntry(g_xs_HT_fat_qg_1010,"HT-fat 95% CL","pL");

  l_xs_theory->SetFillColor(0);
  l_xs_theory->SetFillStyle(0);
  l_xs_theory->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory->AddEntry(g_xs_qstar,"Excited Quark ","L");
  l_xs_theory->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  l_xs_theory->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory->AddEntry(g_xs_zprime,"Z'","L");
  l_xs_theory->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_qg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qg->Update();

  // c_xs_gg

  c_xs_gg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_calo_gg_1010->Draw("PL");
  g_xs_HT_pf_gg_1010->Draw("samePL");
  g_xs_HT_fat_gg_1010->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_gg->SetFillColor(0);
  l_xs_gg->SetFillStyle(0);
  l_xs_gg->SetHeader("gg-(Stat.Only)-1.0fb^{-1}");
  l_xs_gg->AddEntry(g_xs_HT_calo_gg_1010,"HT-calo 95% CL","pL");
  l_xs_gg->AddEntry(g_xs_HT_pf_gg_1010,"HT-PF 95% CL","pL");
  l_xs_gg->AddEntry(g_xs_HT_fat_gg_1010,"HT-fat 95% CL","pL");

  l_xs_gg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_gg->Update();

  // c_xs_qq

  c_xs_qq->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_calo_qq_1010->Draw("PL");
  g_xs_HT_pf_qq_1010->Draw("samePL");
  g_xs_HT_fat_qq_1010->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_qq->SetFillColor(0);
  l_xs_qq->SetFillStyle(0);
  l_xs_qq->SetHeader("qq-(Stat.Only)-1.0fb^{-1}");
  l_xs_qq->AddEntry(g_xs_HT_calo_qq_1010,"HT-calo 95% CL","pL");
  l_xs_qq->AddEntry(g_xs_HT_pf_qq_1010,"HT-PF 95% CL","pL");
  l_xs_qq->AddEntry(g_xs_HT_fat_qq_1010,"HT-fat 95% CL","pL");
  l_xs_qq->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_qq->Update();

  // c_xs_stat_fat

  c_xs_stat_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_fat_gg_1010->Draw("PL");
  g_xs_HT_fat_qq_1010->Draw("samePL");
  g_xs_HT_fat_qg_1010->Draw("samePL");

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
  //  l_xs_stat_fat->AddEntry((TObject*)0,"1.0fb^{-1}","");
  l_xs_stat_fat->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qg_1010,"Quark-Gluon","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_qq_1010,"Quark-Quark","pL");
  l_xs_stat_fat->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_fat->AddText("CMS (1.0 fb^{-1})"); 
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


  // c_xs_stat_pf

  c_xs_stat_pf->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_pf_gg_1010->Draw("PL");
  g_xs_HT_pf_qq_1010->Draw("samePL");
  g_xs_HT_pf_qg_1010->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_stat_pf->SetFillColor(0);
  l_xs_stat_pf->SetFillStyle(0);
  //  l_xs_stat_pf->SetHeader("CMS Preliminary");
  //  l_xs_stat_pf->AddEntry((TObject*)0,"1.0fb^{-1}","");
  l_xs_stat_pf->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_pf->AddEntry(g_xs_HT_pf_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_pf->AddEntry(g_xs_HT_pf_qg_1010,"Quark-Gluon","pL");
  l_xs_stat_pf->AddEntry(g_xs_HT_pf_qq_1010,"Quark-Quark","pL");
  l_xs_stat_pf->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_pf->AddText("CMS (1.0 fb^{-1})"); 
  p_xs_stat_pf->AddText(" #sqrt{s} = 7 TeV");
  p_xs_stat_pf->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  p_xs_stat_pf->SetFillColor(0);
  p_xs_stat_pf->SetLineColor(0);
  p_xs_stat_pf->SetFillStyle(0);
  p_xs_stat_pf->SetBorderSize(0);
  p_xs_stat_pf->SetTextFont(42);
  p_xs_stat_pf->SetTextSize(0.03);
  p_xs_stat_pf->SetTextAlign(12);
  p_xs_stat_pf->Draw("sames");
  c_xs_stat_pf->Update();


  // c_xs_stat_calo

  c_xs_stat_calo->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_calo_gg_1010->Draw("PL");
  g_xs_HT_calo_qq_1010->Draw("samePL");
  g_xs_HT_calo_qg_1010->Draw("samePL");

  g_xs_string->Draw("sameL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_stat_calo->SetFillColor(0);
  l_xs_stat_calo->SetFillStyle(0);
  //  l_xs_stat_calo->SetHeader("CMS Preliminary");
  //  l_xs_stat_calo->AddEntry((TObject*)0,"1.0fb^{-1}","");
  l_xs_stat_calo->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_calo->AddEntry(g_xs_HT_calo_gg_1010,"Gluon-Gluon","pL");
  l_xs_stat_calo->AddEntry(g_xs_HT_calo_qg_1010,"Quark-Gluon","pL");
  l_xs_stat_calo->AddEntry(g_xs_HT_calo_qq_1010,"Quark-Quark","pL");
  l_xs_stat_calo->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_stat_calo->AddText("CMS (1.0 fb^{-1})"); 
  p_xs_stat_calo->AddText(" #sqrt{s} = 7 TeV");
  p_xs_stat_calo->AddText("|#eta| < 2.5, |#Delta#eta| < 1.3");
  p_xs_stat_calo->SetFillColor(0);
  p_xs_stat_calo->SetLineColor(0);
  p_xs_stat_calo->SetFillStyle(0);
  p_xs_stat_calo->SetBorderSize(0);
  p_xs_stat_calo->SetTextFont(42);
  p_xs_stat_calo->SetTextSize(0.03);
  p_xs_stat_calo->SetTextAlign(12);
  p_xs_stat_calo->Draw("sames");
  c_xs_stat_calo->Update();

  // c_xs_sys

  c_xs_sys->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
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
  //  l_xs_sys->AddEntry((TObject*)0,"1.0fb^{-1}","");
  l_xs_sys->SetHeader("95% CL Upper Limit");
  l_xs_sys->AddEntry(g_xs_HT_fat_gg_1010_sys,"Gluon-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qg_1010_sys,"Quark-Gluon","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_qq_1010_sys,"Quark-Quark","pL");
  l_xs_sys->Draw("sames");
  l_xs_theory->Draw("sames");

  p_xs_sys->AddText("CMS (1.0 fb^{-1})"); 
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

  // c_xs_all_fat

  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
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
  l_xs_all_fat->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_all_fat->SetHeader("CMS Preliminary");
  //  l_xs_all_fat->AddEntry((TObject*)0,"1.0fb^{-1}","")
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


  // c_xs_all_pf

  c_xs_all_pf->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_pf_gg_1010->Draw("PL");
  g_xs_HT_pf_qq_1010->Draw("samePL");
  g_xs_HT_pf_qg_1010->Draw("samePL");
  g_xs_HT_pf_gg_1010_sys->Draw("samePL");
  g_xs_HT_pf_qq_1010_sys->Draw("samePL");
  g_xs_HT_pf_qg_1010_sys->Draw("samePL");
  /*
  g_xs_string->Draw("PL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
  */
  l_xs_all_pf->SetFillColor(0);
  l_xs_all_pf->SetFillStyle(0);
  l_xs_all_pf->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_all_pf->SetHeader("CMS Preliminary");
  //  l_xs_all_pf->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_all_pf->AddEntry(g_xs_HT_pf_gg_1010,"gg 95% CL (stat.only)","pL");
  l_xs_all_pf->AddEntry(g_xs_HT_pf_qg_1010,"qg 95% CL (stat.only)","pL");
  l_xs_all_pf->AddEntry(g_xs_HT_pf_qq_1010,"qq 95% CL (stat.only)","pL");
  l_xs_all_pf->AddEntry(g_xs_HT_pf_gg_1010_sys,"gg 95% CL (sys.included)","pL");
  l_xs_all_pf->AddEntry(g_xs_HT_pf_qg_1010_sys,"qg 95% CL (sys.included)","pL");
  l_xs_all_pf->AddEntry(g_xs_HT_pf_qq_1010_sys,"qq 95% CL (sys.included)","pL");
  l_xs_all_pf->Draw("sames");
  /*
  l_xs_theory->Draw("sames");
  */
  c_xs_all_pf->Update();



  // c_xs_all_calo

  c_xs_all_calo->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
  // vFrame->SetLogy();

  g_xs_HT_calo_gg_1010->Draw("PL");
  g_xs_HT_calo_qq_1010->Draw("samePL");
  g_xs_HT_calo_qg_1010->Draw("samePL");
  g_xs_HT_calo_gg_1010_sys->Draw("samePL");
  g_xs_HT_calo_qq_1010_sys->Draw("samePL");
  g_xs_HT_calo_qg_1010_sys->Draw("samePL");
  /*
  g_xs_string->Draw("PL");  
  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  
  */
  l_xs_all_calo->SetFillColor(0);
  l_xs_all_calo->SetFillStyle(0);
  l_xs_all_calo->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_all_calo->SetHeader("CMS Preliminary");
  //  l_xs_all_calo->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_all_calo->AddEntry(g_xs_HT_calo_gg_1010,"gg 95% CL (stat.only)","pL");
  l_xs_all_calo->AddEntry(g_xs_HT_calo_qg_1010,"qg 95% CL (stat.only)","pL");
  l_xs_all_calo->AddEntry(g_xs_HT_calo_qq_1010,"qq 95% CL (stat.only)","pL");
  l_xs_all_calo->AddEntry(g_xs_HT_calo_gg_1010_sys,"gg 95% CL (sys.included)","pL");
  l_xs_all_calo->AddEntry(g_xs_HT_calo_qg_1010_sys,"qg 95% CL (sys.included)","pL");
  l_xs_all_calo->AddEntry(g_xs_HT_calo_qq_1010_sys,"qq 95% CL (sys.included)","pL");
  l_xs_all_calo->Draw("sames");
  /*
  l_xs_theory->Draw("sames");
  */
  c_xs_all_calo->Update();


  // c_xs_comparison_bw_sys_theory_fat

  c_xs_comparison_bw_sys_theory_fat->cd(0);

  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,10.0);
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
  g_theory_over_sys->SetPoint(0,700.0,1.0);
  g_theory_over_sys->SetPoint(1,4200.0,1.0);
  g_theory_over_sys->Draw("samesl");

  l_xs_comparison_bw_sys_theory_fat->SetFillColor(0);
  l_xs_comparison_bw_sys_theory_fat->SetFillStyle(0);
  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_comparison_bw_sys_theory_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_fat->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_string_over_qg_fat,"String Resonance","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_qstar_over_qg_fat,"Excited Quark","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_axigluon_over_qq_fat,"Axigluon/Coloron","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_diquark_over_qq_fat,"E_{6} Diquark","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_wprime_over_qq_fat,"W'","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_zprime_over_qq_fat,"Z'","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_rsg_over_gg_fat,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_fat->Draw("sames");
  c_xs_comparison_bw_sys_theory_fat->Update();

  // c_xs_comparison_bw_sys_theory_pf

  c_xs_comparison_bw_sys_theory_pf->cd(0);

  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,10.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(theory #sigma) / (sys.included 95% CL #sigma)");
  vFrame->SetMinimum(0.01);
  vFrame->SetMaximum(10.0);
  // vFrame->SetLogy();

  g_xs_string_over_qg_pf->Draw("L");  
  g_xs_qstar_over_qg_pf->Draw("sameL");
  g_xs_axigluon_over_qq_pf->Draw("sameL");
  g_xs_diquark_over_qq_pf->Draw("sameL");
  g_xs_wprime_over_qq_pf->Draw("sameL");  
  g_xs_zprime_over_qq_pf->Draw("sameL");  
  g_xs_rsg_over_gg_pf->Draw("sameL");  

  TGraph *g_theory_over_sys = new TGraph(0);

  g_theory_over_sys->SetFillColor(1);
  g_theory_over_sys->SetLineColor(1);
  g_theory_over_sys->SetLineStyle(3);
  g_theory_over_sys->SetLineWidth(3);
  g_theory_over_sys->SetMarkerStyle(20);
  g_theory_over_sys->SetPoint(0,700.0,1.0);
  g_theory_over_sys->SetPoint(1,4200.0,1.0);
  g_theory_over_sys->Draw("samesl");

  l_xs_comparison_bw_sys_theory_pf->SetFillColor(0);
  l_xs_comparison_bw_sys_theory_pf->SetFillStyle(0);
  l_xs_comparison_bw_sys_theory_pf->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_comparison_bw_sys_theory_pf->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_pf->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_string_over_qg_pf,"String Resonance","L");
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_qstar_over_qg_pf,"Excited Quark","L");
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_axigluon_over_qq_pf,"Axigluon/Coloron","L");
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_diquark_over_qq_pf,"E_{6} Diquark","L");
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_wprime_over_qq_pf,"W'","L");
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_zprime_over_qq_pf,"Z'","L");
  l_xs_comparison_bw_sys_theory_pf->AddEntry(g_xs_rsg_over_gg_pf,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_pf->Draw("sames");
  c_xs_comparison_bw_sys_theory_pf->Update();

  // c_xs_comparison_bw_sys_theory_calo

  c_xs_comparison_bw_sys_theory_calo->cd(0);

  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,10.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(theory #sigma) / (sys.included 95% CL #sigma)");
  vFrame->SetMinimum(0.01);
  vFrame->SetMaximum(10.0);
  // vFrame->SetLogy();

  g_xs_string_over_qg_calo->Draw("L");  
  g_xs_qstar_over_qg_calo->Draw("sameL");
  g_xs_axigluon_over_qq_calo->Draw("sameL");
  g_xs_diquark_over_qq_calo->Draw("sameL");
  g_xs_wprime_over_qq_calo->Draw("sameL");  
  g_xs_zprime_over_qq_calo->Draw("sameL");  
  g_xs_rsg_over_gg_calo->Draw("sameL");  

  TGraph *g_theory_over_sys = new TGraph(0);

  g_theory_over_sys->SetFillColor(1);
  g_theory_over_sys->SetLineColor(1);
  g_theory_over_sys->SetLineStyle(3);
  g_theory_over_sys->SetLineWidth(3);
  g_theory_over_sys->SetMarkerStyle(20);
  g_theory_over_sys->SetPoint(0,700.0,1.0);
  g_theory_over_sys->SetPoint(1,4200.0,1.0);
  g_theory_over_sys->Draw("samesl");

  l_xs_comparison_bw_sys_theory_calo->SetFillColor(0);
  l_xs_comparison_bw_sys_theory_calo->SetFillStyle(0);
  l_xs_comparison_bw_sys_theory_calo->SetHeader("CMS (1.0 fb^{-1})");
  //  l_xs_comparison_bw_sys_theory_calo->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_sys_theory_calo->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_string_over_qg_calo,"String Resonance","L");
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_qstar_over_qg_calo,"Excited Quark","L");
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_axigluon_over_qq_calo,"Axigluon/Coloron","L");
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_diquark_over_qq_calo,"E_{6} Diquark","L");
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_wprime_over_qq_calo,"W'","L");
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_zprime_over_qq_calo,"Z'","L");
  l_xs_comparison_bw_sys_theory_calo->AddEntry(g_xs_rsg_over_gg_calo,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_calo->Draw("sames");
  c_xs_comparison_bw_sys_theory_calo->Update();

  // c_xs_qg_stat_E

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
  h_xs_qg_stat_E->SetMinimum(0.003);
  h_xs_qg_stat_E->SetMaximum(3.0);

  c_xs_qg_stat_E->cd(0);

  h_xs_HT_fat_qg_1010_stat_E_d2->GetXaxis()->SetRangeUser(700.0,4200.0);
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
  g_xs_HT_fat_qg_1010_sys->Draw("samePL");

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
  p_xs_qg_stat_E->AddText("CMS (1.0 fb^{-1})");
  p_xs_qg_stat_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qg_stat_E->Draw("sames");
	
  l_xs_qg_stat_E->SetFillColor(0);
  l_xs_qg_stat_E->SetFillStyle(0);
//  l_xs_qg_stat_E->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_qg_stat_E->SetHeader("CMS Preliminary");
  //  l_xs_qg_stat_E->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_qg_stat_E->AddEntry(g_xs_HT_fat_qg_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_d2,"-2 #sigma","pL");
  //  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_d1,"-1 #sigma","pL");
  l_xs_qg_stat_E->AddEntry(g_xs_HT_fat_qg_1010_stat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qg_stat_E->AddEntry(h_xs_HT_fat_qg_1010_stat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qg_stat_E->Draw("sames");
  l_xs_theory_qg->SetFillColor(0);
  l_xs_theory_qg->SetFillStyle(0);
  l_xs_theory_qg->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory_qg->AddEntry(g_xs_qstar,"Excited Quark ","L");
  l_xs_theory_qg->Draw("sames");

  c_xs_qg_stat_E->Update();


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

  h_xs_qq_stat_E->SetMinimum(0.003);
  h_xs_qq_stat_E->SetMaximum(3.0);

  c_xs_qq_stat_E->cd(0);

  h_xs_HT_fat_qq_1010_stat_E_d2->GetXaxis()->SetRangeUser(700.0,4200.0);
  h_xs_qq_stat_E->SetTitle("");
  h_xs_HT_fat_qq_1010_stat_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_qq_1010_stat_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_qq_stat_E->Draw("C");

  h_xs_qq_stat_E->GetXaxis()->SetTitle("qq Resonance Mass (GeV)");
  h_xs_qq_stat_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");


  c_xs_qq_stat_E->cd(0);
 
  g_xs_HT_fat_qq_1010_stat_E_m->Draw("sameC");
  g_xs_HT_fat_qq_1010_sys->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_qstar->Draw("sameL");
  g_xs_axigluon->Draw("sameL");
  g_xs_diquark->Draw("sameL");
  g_xs_wprime->Draw("sameL");  
  g_xs_zprime->Draw("sameL");  
  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_qq_stat_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_qq_stat_E->Draw();
	
  p_xs_qq_stat_E->SetFillColor(0);
  p_xs_qq_stat_E->SetFillStyle(0);
  p_xs_qq_stat_E->SetBorderSize(0);
  p_xs_qq_stat_E->AddText("CMS (1.0fb^{-1})");
  p_xs_qq_stat_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_qq_stat_E->Draw("sames");

  l_xs_qq_stat_E->SetFillColor(0);
  l_xs_qq_stat_E->SetFillStyle(0);
  //  l_xs_qq_stat_E->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_qq_stat_E->SetHeader("CMS Preliminary");
  //  l_xs_qq_stat_E->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_qq_stat_E->AddEntry(g_xs_HT_fat_qq_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_d2,"-2 #sigma","pL");
  //  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_d1,"-1 #sigma","pL");
  l_xs_qq_stat_E->AddEntry(g_xs_HT_fat_qq_1010_stat_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_qq_stat_E->AddEntry(h_xs_HT_fat_qq_1010_stat_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_qq_stat_E->Draw("sames");
  l_xs_theory_qq->SetFillColor(0);
  l_xs_theory_qq->SetFillStyle(0);
  l_xs_theory_qq->AddEntry(g_xs_axigluon,"Axigluon/Coloron","L");
  l_xs_theory_qq->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  l_xs_theory_qq->AddEntry(g_xs_wprime,"W'","L");
  l_xs_theory_qq->AddEntry(g_xs_zprime,"Z'","L");
  l_xs_theory_qq->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_qq->Draw("sames");

  c_xs_qq_stat_E->Update();

  // c_xs_comparison_bw_stat_sys_fat

  c_xs_comparison_bw_stat_sys_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
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
  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_comparison_bw_stat_sys_fat->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_fat->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_sys_fat, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();

  // c_xs_comparison_bw_stat_sys_pf

  c_xs_comparison_bw_stat_sys_pf->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
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

  g_xs_comparison_bw_stat_bg_pf->Draw("PL");;
  g_xs_comparison_bw_stat_lumi_pf->Draw("samePL");
  g_xs_comparison_bw_stat_JES_pf->Draw("samePL");
  g_xs_comparison_bw_stat_JER_pf->Draw("samePL");
  g_xs_comparison_bw_stat_sys_pf->Draw("samePL");
	
  l_xs_comparison_bw_stat_sys_pf->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_pf->SetFillStyle(0);
  l_xs_comparison_bw_stat_sys_pf->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_comparison_bw_stat_sys_pf->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_pf->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_comparison_bw_stat_sys_pf->AddEntry(g_xs_comparison_bw_stat_sys_pf, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_pf->AddEntry(g_xs_comparison_bw_stat_bg_pf,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_pf->AddEntry(g_xs_comparison_bw_stat_lumi_pf,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_pf->AddEntry(g_xs_comparison_bw_stat_JES_pf,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_pf->AddEntry(g_xs_comparison_bw_stat_JER_pf,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_pf->Draw("sames");

  c_xs_comparison_bw_stat_sys_pf->Update();

  // c_xs_comparison_bw_stat_sys_calo

  c_xs_comparison_bw_stat_sys_calo->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
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

  g_xs_comparison_bw_stat_bg_calo->Draw("PL");;
  g_xs_comparison_bw_stat_lumi_calo->Draw("samePL");
  g_xs_comparison_bw_stat_JES_calo->Draw("samePL");
  g_xs_comparison_bw_stat_JER_calo->Draw("samePL");
  g_xs_comparison_bw_stat_sys_calo->Draw("samePL");
	
  l_xs_comparison_bw_stat_sys_calo->SetFillColor(0);
  l_xs_comparison_bw_stat_sys_calo->SetFillStyle(0);
  l_xs_comparison_bw_stat_sys_calo->SetHeader("CMS (1.0fb^{-1})");
  //  l_xs_comparison_bw_stat_sys_calo->SetHeader("CMS Preliminary");
  //  l_xs_comparison_bw_stat_sys_calo->AddEntry((TObject*)0,"1.0fb^{-1}","")
  l_xs_comparison_bw_stat_sys_calo->AddEntry(g_xs_comparison_bw_stat_sys_calo, "all sys / stat","pL");
  l_xs_comparison_bw_stat_sys_calo->AddEntry(g_xs_comparison_bw_stat_bg_calo,"background / stat","pL");
  l_xs_comparison_bw_stat_sys_calo->AddEntry(g_xs_comparison_bw_stat_lumi_calo,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_calo->AddEntry(g_xs_comparison_bw_stat_JES_calo,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_calo->AddEntry(g_xs_comparison_bw_stat_JER_calo,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_calo->Draw("sames");

  c_xs_comparison_bw_stat_sys_calo->Update();

  // c_xs_qg_compare

  c_xs_qg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_pf_qg_compare->Draw("PL");
  g_xs_HT_fat_qg_compare->Draw("samePL");

  l_xs_qg_compare->SetFillColor(0);
  l_xs_qg_compare->SetFillStyle(0);
  l_xs_qg_compare->SetHeader("qg-(Stat.Only)-1.0fb^{-1}");
  l_xs_qg_compare->AddEntry(g_xs_HT_pf_qg_compare,"HT-PF","pL");
  l_xs_qg_compare->AddEntry(g_xs_HT_fat_qg_compare,"HT-fat","pL");

  l_xs_qg_compare->Draw("sames");

  c_xs_qg_compare->Update();

  // c_xs_gg_compare

  c_xs_gg_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_pf_gg_compare->Draw("PL");
  g_xs_HT_fat_gg_compare->Draw("samePL");

  l_xs_gg_compare->SetFillColor(0);
  l_xs_gg_compare->SetFillStyle(0);
  l_xs_gg_compare->SetHeader("gg-(Stat.Only)-1.0fb^{-1}");
  l_xs_gg_compare->AddEntry(g_xs_HT_pf_gg_compare,"HT-PF","pL");
  l_xs_gg_compare->AddEntry(g_xs_HT_fat_gg_compare,"HT-fat","pL");

  l_xs_gg_compare->Draw("sames");

  c_xs_gg_compare->Update();

  // c_xs_qq_compare

  c_xs_qq_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(700.0,0.01,4200.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_pf_qq_compare->Draw("PL");
  g_xs_HT_fat_qq_compare->Draw("samePL");

  l_xs_qq_compare->SetFillColor(0);
  l_xs_qq_compare->SetFillStyle(0);
  l_xs_qq_compare->SetHeader("qq-(Stat.Only)-1.0fb^{-1}");
  l_xs_qq_compare->AddEntry(g_xs_HT_pf_qq_compare,"HT-PF","pL");
  l_xs_qq_compare->AddEntry(g_xs_HT_fat_qq_compare,"HT-fat","pL");

  l_xs_qq_compare->Draw("sames");

  c_xs_qq_compare->Update();

  // c_xs_rsg

  c_xs_rsg->cd(0);
  TH1F *vFrame = gPad->DrawFrame(800.0,0.01,1400.0,11.0);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance Mass (GeV)");
  vFrame->GetXaxis()->SetTitleFont(42);
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  vFrame->SetMinimum(0.003);
  vFrame->SetMaximum(3.0);
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
  //  l_xs_rsg->SetHeader("CMS Preliminary");
  //  l_xs_rsg->AddEntry((TObject*)0,"1.0fb^{-1}","");
  l_xs_rsg->SetHeader("95% CL Upper Limit");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");
  l_xs_rsg->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_rsg->Update();

}
