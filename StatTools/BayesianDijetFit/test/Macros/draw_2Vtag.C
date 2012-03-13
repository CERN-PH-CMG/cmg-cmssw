#include <iostream>
#include <iomanip>

void draw_2Vtag() 
{
  std::cout << "begin" << std::endl;

  
  gROOT->ProcessLine(".L Macros/setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  //  gSystem->Load("libFWCoreFWLite.so");

  TFile *outf = new TFile("xs.root","RECREATE");

  const unsigned int nEBins = 12;
  double minMass = 950.0, maxMass = 2150.0;
  double minXsec = 1e-3, maxXsec = 5e-1;

  double x[nEBins] = {1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000., 2100.};




  // definition of color 

  Int_t ci_g, ci_b;   // for color index setting
  ci_g = TColor::GetColor("#006600");
  ci_b = TColor::GetColor("#000099");

  // definition of canvases

  TCanvas *c_xs_WZ = new TCanvas("c_xs_WZ","c_xs_WZ",459,358,600,602);
  TCanvas *c_xs_WW = new TCanvas("c_xs_WW","c_xs_WW",459,358,600,602);
  TCanvas *c_xs_ZZ = new TCanvas("c_xs_ZZ","c_xs_ZZ",459,358,600,602);
  TCanvas *c_xs_stat_fat = new TCanvas("c_xs_stat_fat","c_xs_stat_fat",459,358,600,602);
  TCanvas *c_xs_sys = new TCanvas("c_xs_sys","c_xs_sys",459,358,600,602);
  TCanvas *c_xs_all_fat = new TCanvas("c_xs_all_fat","c_xs_all_fat",459,358,600,602);
  TCanvas *c_xs_comparison_bw_stat_sys_fat = new TCanvas("c_xs_comparison_bw_stat_sys_fat","c_xs_comparison_bw_stat_sys_fat",459,358,600,602);
  //TCanvas *c_xs_WZ_compare = new TCanvas("c_xs_WZ_compare","c_xs_WZ_compare",459,358,600,602);
  //TCanvas *c_xs_WW_compare = new TCanvas("c_xs_WW_compare","c_xs_WW_compare",459,358,600,602);
  //TCanvas *c_xs_ZZ_compare = new TCanvas("c_xs_ZZ_compare","c_xs_ZZ_compare",459,358,600,602);
  TCanvas *c_xs_comparison_bw_sys_theory_fat = new TCanvas("c_xs_comparison_bw_sys_theory_fat","c_xs_comparison_bw_sys_theory_fat",459,358,600,602);

  TCanvas *c_xs_WZ_sys_E = new TCanvas("c_xs_WZ_sys_E","c_xs_WZ_sys_E",459,358,600,602);
  TCanvas *c_xs_WW_sys_E = new TCanvas("c_xs_WW_sys_E","c_xs_WW_sys_E",459,358,600,602);
  TCanvas *c_xs_ZZ_sys_E = new TCanvas("c_xs_ZZ_sys_E","c_xs_ZZ_sys_E",459,358,600,602);

  // definition of legends

  TLegend *l_xs_theory =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_WZ =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_WW =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");
  TLegend *l_xs_theory_ZZ =  new TLegend(0.6694631,0.7108014,0.9614094,0.9216028,NULL,"brNDC");

  TLegend *l_xs_WZ =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_WW =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_ZZ =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_stat_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_sys =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_all_fat =  new TLegend(0.1694631,0.1585366,0.5419463,0.4808362,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_stat_sys_fat =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_WZ_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_WW_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_ZZ_compare =  new TLegend(0.1694631,0.1585366,0.4479866,0.3362369,NULL,"brNDC");
  TLegend *l_xs_comparison_bw_sys_theory_fat =  new TLegend(0.1694631,0.1585366,0.5016779,0.4111498,NULL,"brNDC");
  TLegend *l_xs_WZ_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_WW_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");
  TLegend *l_xs_ZZ_sys_E =  new TLegend(0.1560403,0.1445993,0.647651,0.4320557,NULL,"brNDC");

  TPaveText *p_xs_WZ_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");
  TPaveText *p_xs_WW_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");
  TPaveText *p_xs_ZZ_sys_E =  new TPaveText(0.1627517,0.4355401,0.3573826,0.5487805,"brNDC");

  TPaveText *p_xs_sys = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");
  TPaveText *p_xs_stat_fat = new TPaveText(0.1694631,0.3432056,0.4379195,0.4930314,"brNDC");

  // settings of canvases

  c_xs_WZ->SetLogy();
  c_xs_WW->SetLogy();
  c_xs_ZZ->SetLogy();
  c_xs_stat_fat->SetLogy();
  c_xs_sys->SetLogy();
  c_xs_all_fat->SetLogy();
  c_xs_comparison_bw_sys_theory_fat->SetLogy();
  c_xs_WZ_sys_E->SetLogy();
  c_xs_WW_sys_E->SetLogy();
  c_xs_ZZ_sys_E->SetLogy();

  // theory values

 const unsigned int en = 4;
 const unsigned int en_short = 3;

 double mjj [en] ={1000., 1500., 2000., 3000.};

  double RSG_WW [en] = {
    //1.512e+2, 1.522e+1, 2.603e+0, 1.672e-1, 1.862e-2, 7.238e-5}; //Herwig++
    /*1.512e+2, 1.522e+1, */0.256, 1.75e-2, 1.83e-3, 2.95e-5}; //Pythia6
 
  double RSG_ZZ[en] = {
    //7.648e+1, 7.678e+0, 1.301e+0, 8.379e-2, 9.310e-3, 1.932e-4}; //Herwig++
    /*7.648e+1, 7.678e+0, */0.128, 8.73e-3, 9.22e-4, 1.47e-5}; //Pythia6

  double mjjshort[en_short] ={1500., 2000., 3000.};
  double RSG_ZZ_ex[en_short] = {
//    /*, */12e-3, 4e-3, 0.12e-3}; //arxiv-hep-ph/0701186 k/MPl=1.0 no eta-cut
    /*, */80e-3, 12e-3, 0.7e-3}; //arxiv-hep-ph/0701186 k/MPl=2.0 no eta-cut

  double Wprime_WZ[en] = {
    /*2.455e+0, 4.010e-1, */1.021e-1, 1.121e-2, 1.697e-3, 1.044e-4};

  double RSG_kfactor[en] = {
    1.0, 1.0, 1.0, 1.0};
//    0.1, 0.1, 0.1, 0.1}; //Wrong units in Herwig++ cross sections

  double Wprime_kfactor[en] = {
    1.0, 1.0, 1.0, 1.0};

  double RSG_WW_acc[en] = {
    0.237, 0.256, 0.288, 0.288};
 
  double RSG_ZZ_acc[en] = {
    0.233, 0.269, 0.298, 0.298};

  double RSG_ZZ_01_acc[en_short] = {
    0.269, 0.298, 0.298};

  double Wprime_WZ_acc[en] = {
    0.357, 0.337, 0.287, 0.287};

  double RSG_WW_eff[en] = {
    0.126*1.1, 0.138*1.1, 0.110*1.1, 0.110*1.1};
 
  double RSG_ZZ_eff[en] = {
    0.168*1.1, 0.179*1.1, 0.179*1.1, 0.179*1.1};

  double Wprime_WZ_eff[en] = {
    0.268*0.96, 0.291*0.96, 0.275*0.96, 0.275*0.96};

  TGraph* RSG_WW_graph = new TGraph(en,mjj,RSG_WW);
  TF1* RSG_WW_f = new TF1("WW","pol2",minMass,maxMass);
  RSG_WW_graph->Fit(RSG_WW_f);

  TGraph* RSG_ZZ_graph = new TGraph(en,mjj,RSG_ZZ);
  TF1* RSG_ZZ_f = new TF1("ZZ","pol2",minMass,maxMass);
  RSG_ZZ_graph->Fit(RSG_ZZ_f);

  TGraph* RSG_ZZ_ex_graph = new TGraph(en,mjjshort,RSG_ZZ_ex);
  TF1* RSG_ZZ_ex_f = new TF1("ZZ_ex","pol2",minMass,maxMass);
  RSG_ZZ_ex_graph->Fit(RSG_ZZ_ex_f);

  TGraph* Wprime_WZ_graph = new TGraph(en,mjj,Wprime_WZ);
  TF1* Wprime_WZ_f = new TF1("WZ","pol2",minMass,maxMass);
  Wprime_WZ_graph->Fit(Wprime_WZ_f);

  TGraph* RSG_WW_eff_graph = new TGraph(en,mjj,RSG_WW_eff);
  TF1* RSG_WW_efficiency = new TF1("WWeff","pol1",minMass,maxMass);
  RSG_WW_eff_graph->Fit(RSG_WW_efficiency);

  TGraph* RSG_ZZ_eff_graph = new TGraph(en,mjj,RSG_ZZ_eff);
  TF1* RSG_ZZ_efficiency = new TF1("ZZeff","pol1",minMass,maxMass);
  RSG_ZZ_eff_graph->Fit(RSG_ZZ_efficiency);

  TGraph* Wprime_WZ_eff_graph = new TGraph(en,mjj,Wprime_WZ_eff);
  TF1* Wprime_WZ_efficiency = new TF1("WZeff","pol1",minMass,maxMass);
  Wprime_WZ_eff_graph->Fit(Wprime_WZ_efficiency);

  cout << "acceptance: " << endl;
  for (int i = 0; i < en; i++){
    cout << RSG_WW_acc[i] << "/" << RSG_ZZ_acc[i] << ", ";
    RSG_WW[i] *= RSG_kfactor[i]*RSG_WW_acc[i];
    RSG_ZZ[i] *= RSG_kfactor[i]*RSG_ZZ_acc[i];
    Wprime_WZ[i] *= Wprime_kfactor[i]*Wprime_WZ_acc[i];
  }
  cout << endl;
  for (int i = 0; i < en_short; i++){
    RSG_ZZ_ex[i] *= RSG_ZZ_01_acc[i];
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

 TFile* file_efficiencies_ZZ = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_RSGraviton_ZZ_ak5_fat.root", "READ");
 TFile* file_efficiencies_ZZ_01 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_RSGraviton_ZZ_01_ak5_fat.root", "READ");
 TFile* file_efficiencies_WW = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_RSGraviton_WW_ak5_fat.root", "READ");
 TFile* file_efficiencies_WZ = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/Resonance_Shapes_RSGraviton_WZ_ak5_fat.root", "READ");

 TH1F* limit_efficiencies_ZZ = file_efficiencies_ZZ->Get("Efficiency;1");
 TH1F* limit_efficiencies_ZZ_01 = file_efficiencies_ZZ_01->Get("Efficiency;1");
 TH1F* limit_efficiencies_WW = file_efficiencies_WW->Get("Efficiency;1");
 TH1F* limit_efficiencies_WZ = file_efficiencies_WZ->Get("Efficiency;1");


 TFile* file_ZZ0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_ZZ_ak5_fat_0.root", "READ");
 TFile* file_ZZ_010 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_ZZ_01_ak5_fat_0.root", "READ");
 TFile* file_WW0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_0.root", "READ");
 TFile* file_WZ0 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WZ_ak5_fat_0.root", "READ");

 
 TH1F* limit_ZZ0 = file_ZZ0->Get("limit;1");
 TH1F* limit_ZZ_010 = file_ZZ_010->Get("limit;1");
 TH1F* limit_WW0 = file_WW0->Get("limit;1");
 TH1F* limit_WZ0 = file_WZ0->Get("limit;1");

 cout << "efficiency: " << endl;
 for (int i = 1; i < limit_efficiencies_ZZ->GetNbinsX()+1; i++){

   // V-tagging efficincy
   cout << RSG_ZZ_efficiency->Eval(limit_efficiencies_ZZ->GetBinCenter(i)) << "/" << RSG_WW_efficiency->Eval(limit_efficiencies_WW->GetBinCenter(i)) << "/" << Wprime_WZ_efficiency->Eval(limit_efficiencies_WZ->GetBinCenter(i)) << ", ";
   limit_efficiencies_ZZ->SetBinContent(i, limit_efficiencies_ZZ->GetBinContent(i) * RSG_ZZ_efficiency->Eval(limit_efficiencies_ZZ->GetBinCenter(i)));
   limit_efficiencies_ZZ_01->SetBinContent(i, limit_efficiencies_ZZ_01->GetBinContent(i) * RSG_ZZ_efficiency->Eval(limit_efficiencies_ZZ_01->GetBinCenter(i)));
   limit_efficiencies_WW->SetBinContent(i, limit_efficiencies_WW->GetBinContent(i) * RSG_WW_efficiency->Eval(limit_efficiencies_WW->GetBinCenter(i)));
   limit_efficiencies_WZ->SetBinContent(i, limit_efficiencies_WZ->GetBinContent(i) * Wprime_WZ_efficiency->Eval(limit_efficiencies_WZ->GetBinCenter(i)));

 }
  cout << endl;

 for (int i = 1; i < limit_ZZ0->GetNbinsX()+1; i++){

   limit_ZZ0->SetBinContent(i, limit_ZZ0->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_ZZ_010->SetBinContent(i, limit_ZZ_010->GetBinContent(i) / limit_efficiencies_ZZ_01->GetBinContent(i));
   limit_WW0->SetBinContent(i, limit_WW0->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WZ0->SetBinContent(i, limit_WZ0->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));

 }

  // fat-stat only --
 double* xs_HT_fat_ZZ_1010 = new double[nEBins];
 double* xs_HT_fat_WW_1010 = new double[nEBins];
 double* xs_HT_fat_WZ_1010 = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_ZZ_1010[i] = limit_ZZ0->GetBinContent(i+1);
   xs_HT_fat_WW_1010[i] = limit_WW0->GetBinContent(i+1);
   xs_HT_fat_WZ_1010[i] = limit_WZ0->GetBinContent(i+1);
 }

 file_ZZ0->Close();
 file_WW0->Close();
 file_WZ0->Close(); 


 cout << "looking differentially for systematics" << endl;

 

 TFile* file_WW_bg = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_1.root", "READ");
 TFile* file_WW_bg2 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_12.root", "READ");
 TFile* file_WW_bg3 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_100.root", "READ");
 TFile* file_WW_bg4 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_101.root", "READ");
 TFile* file_WW_bg5 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_102.root", "READ");
 TFile* file_WW_bg6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_103.root", "READ");
 TFile* file_WW_bg7 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_104.root", "READ");
 TFile* file_WW_bg8 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_105.root", "READ");
 TFile* file_WW_lumi = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_2.root", "READ");
 TFile* file_WW_JES = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_3.root", "READ");
 TFile* file_WW_JER = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_4.root", "READ");

 
 TH1F* limit_WW_bg = file_WW_bg->Get("limit;1");
 TH1F* limit_WW_bg2 = file_WW_bg2->Get("limit;1");
 TH1F* limit_WW_bg3 = file_WW_bg3->Get("limit;1");
 TH1F* limit_WW_bg4 = file_WW_bg4->Get("limit;1");
 TH1F* limit_WW_bg5 = file_WW_bg5->Get("limit;1");
 TH1F* limit_WW_bg6 = file_WW_bg6->Get("limit;1");
 TH1F* limit_WW_bg7 = file_WW_bg7->Get("limit;1");
 TH1F* limit_WW_bg8 = file_WW_bg8->Get("limit;1");
 TH1F* limit_WW_lumi = file_WW_lumi->Get("limit;1");
 TH1F* limit_WW_JES = file_WW_JES->Get("limit;1");
 TH1F* limit_WW_JER = file_WW_JER->Get("limit;1");

 
 for (int i = 1; i < limit_WW_bg->GetNbinsX()+1; i++){

   limit_WW_bg->SetBinContent(i, limit_WW_bg->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg2->SetBinContent(i, limit_WW_bg2->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg3->SetBinContent(i, limit_WW_bg3->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg4->SetBinContent(i, limit_WW_bg4->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg5->SetBinContent(i, limit_WW_bg5->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg6->SetBinContent(i, limit_WW_bg6->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg7->SetBinContent(i, limit_WW_bg7->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_bg8->SetBinContent(i, limit_WW_bg8->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_lumi->SetBinContent(i, limit_WW_lumi->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_JES->SetBinContent(i, limit_WW_JES->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_JER->SetBinContent(i, limit_WW_JER->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
 }
 

  // fat-stat only --
 double* xs_HT_fat_WW_1010_bg = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg2 = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg3 = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg4 = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg5 = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg6 = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg7 = new double[nEBins];
 double* xs_HT_fat_WW_1010_bg8 = new double[nEBins];
 double* xs_HT_fat_WW_1010_lumi = new double[nEBins];
 double* xs_HT_fat_WW_1010_JES = new double[nEBins];
 double* xs_HT_fat_WW_1010_JER = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_WW_1010_bg[i] = limit_WW_bg->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg2[i] = limit_WW_bg2->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg3[i] = limit_WW_bg3->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg4[i] = limit_WW_bg4->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg5[i] = limit_WW_bg5->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg6[i] = limit_WW_bg6->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg7[i] = limit_WW_bg7->GetBinContent(i+1);
   xs_HT_fat_WW_1010_bg8[i] = limit_WW_bg8->GetBinContent(i+1);
   xs_HT_fat_WW_1010_lumi[i] = limit_WW_lumi->GetBinContent(i+1);
   xs_HT_fat_WW_1010_JES[i] = limit_WW_JES->GetBinContent(i+1);
   xs_HT_fat_WW_1010_JER[i] = limit_WW_JER->GetBinContent(i+1);
 }

 file_WW_bg->Close();
 file_WW_bg2->Close();
 file_WW_bg3->Close();
 file_WW_bg4->Close();
 file_WW_bg5->Close();
 file_WW_bg6->Close();
 file_WW_bg7->Close();
 file_WW_bg8->Close();
 file_WW_lumi->Close();
 file_WW_JES->Close();
 file_WW_JER->Close();


 cout << "looking for All systematics" << endl;
  // fat-All_Fat sys.included
 TFile* file_ZZ6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_ZZ_ak5_fat_6.root", "READ");
 TFile* file_ZZ_016 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_ZZ_01_ak5_fat_6.root", "READ");
 TFile* file_WW6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WW_ak5_fat_6.root", "READ");
 TFile* file_WZ6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_limit_RSGraviton_WZ_ak5_fat_6.root", "READ");

 
 TH1F* limit_ZZ6 = file_ZZ6->Get("limit;1");
 TH1F* limit_ZZ_016 = file_ZZ_016->Get("limit;1");
 TH1F* limit_WW6 = file_WW6->Get("limit;1");
 TH1F* limit_WZ6 = file_WZ6->Get("limit;1");

 for (int i = 1; i < limit_ZZ6->GetNbinsX()+1; i++){

   cout << "Reweighting" << endl;
   limit_ZZ6->SetBinContent(i, limit_ZZ6->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_ZZ_016->SetBinContent(i, limit_ZZ_016->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_WW6->SetBinContent(i, limit_WW6->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WZ6->SetBinContent(i, limit_WZ6->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));

 }


  // fat-stat only --
 double* xs_HT_fat_ZZ_1010_sys = new double[nEBins];
 double* xs_HT_fat_ZZ_01_1010_sys = new double[nEBins];
 double* xs_HT_fat_WW_1010_sys = new double[nEBins];
 double* xs_HT_fat_WZ_1010_sys = new double[nEBins];

 for (int i = 0 ; i < nEBins; i++){
   xs_HT_fat_ZZ_1010_sys[i] = limit_ZZ6->GetBinContent(i+1);
   xs_HT_fat_ZZ_01_1010_sys[i] = limit_ZZ_016->GetBinContent(i+1);
   xs_HT_fat_WW_1010_sys[i] = limit_WW6->GetBinContent(i+1);
   xs_HT_fat_WZ_1010_sys[i] = limit_WZ6->GetBinContent(i+1);
 }

 file_ZZ6->Close();
 file_WW6->Close();
 file_WZ6->Close(); 

 // TODO: should be _6 instead of _0
 TFile* file_exp_ZZ6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_expectedlimit50_RSGraviton_ZZ_ak5_fat_6.root", "READ");
 TFile* file_exp_WW6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_expectedlimit50_RSGraviton_WW_ak5_fat_6.root", "READ");
 TFile* file_exp_WZ6 = new TFile("plots_LowAndHighMass_2011_118pbm1_4677pbm1/limit_expectedlimit50_RSGraviton_WZ_ak5_fat_6.root", "READ");


 TH1F* limit_ZZ_twosigma_lowers = file_exp_ZZ6->Get("htwosigma_lowers;1");
 TH1F* limit_ZZ_onesigma_lowers = file_exp_ZZ6->Get("honesigma_lowers;1");
 TH1F* limit_ZZ_medians = file_exp_ZZ6->Get("hmedians;1");
 TH1F* limit_ZZ_onesigma_uppers = file_exp_ZZ6->Get("honesigma_uppers;1");
 TH1F* limit_ZZ_twosigma_uppers = file_exp_ZZ6->Get("htwosigma_uppers;1");
 

 for (int i = 1; i < limit_ZZ_twosigma_lowers->GetNbinsX()+1; i++){

   limit_ZZ_twosigma_lowers->SetBinContent(i, limit_ZZ_twosigma_lowers->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_ZZ_onesigma_lowers->SetBinContent(i, limit_ZZ_onesigma_lowers->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_ZZ_medians->SetBinContent(i, limit_ZZ_medians->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_ZZ_onesigma_uppers->SetBinContent(i, limit_ZZ_onesigma_uppers->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
   limit_ZZ_twosigma_uppers->SetBinContent(i, limit_ZZ_twosigma_uppers->GetBinContent(i) / limit_efficiencies_ZZ->GetBinContent(i));
 }

 
 double* xs_HT_fat_ZZ_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_ZZ_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_ZZ_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_ZZ_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_ZZ_1010_sys_E_u2 = new double[nEBins];


 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_ZZ_1010_sys_E_d2[i] = limit_ZZ_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_ZZ_1010_sys_E_d1[i] = limit_ZZ_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_ZZ_1010_sys_E_m[i] = limit_ZZ_medians->GetBinContent(i+1);
   xs_HT_fat_ZZ_1010_sys_E_u1[i] = limit_ZZ_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_ZZ_1010_sys_E_u2[i] = limit_ZZ_twosigma_uppers->GetBinContent(i+1);

 }


 TH1F* limit_WW_twosigma_lowers = file_exp_WW6->Get("htwosigma_lowers;1");
 TH1F* limit_WW_onesigma_lowers = file_exp_WW6->Get("honesigma_lowers;1");
 TH1F* limit_WW_medians = file_exp_WW6->Get("hmedians;1");
 TH1F* limit_WW_onesigma_uppers = file_exp_WW6->Get("honesigma_uppers;1");
 TH1F* limit_WW_twosigma_uppers = file_exp_WW6->Get("htwosigma_uppers;1");

 for (int i = 1; i < limit_WW_twosigma_lowers->GetNbinsX()+1; i++){

   limit_WW_twosigma_lowers->SetBinContent(i, limit_WW_twosigma_lowers->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_onesigma_lowers->SetBinContent(i, limit_WW_onesigma_lowers->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_medians->SetBinContent(i, limit_WW_medians->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_onesigma_uppers->SetBinContent(i, limit_WW_onesigma_uppers->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
   limit_WW_twosigma_uppers->SetBinContent(i, limit_WW_twosigma_uppers->GetBinContent(i) / limit_efficiencies_WW->GetBinContent(i));
 }


 double* xs_HT_fat_WW_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_WW_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_WW_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_WW_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_WW_1010_sys_E_u2 = new double[nEBins];





 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_WW_1010_sys_E_d2[i] = limit_WW_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_WW_1010_sys_E_d1[i] = limit_WW_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_WW_1010_sys_E_m[i] = limit_WW_medians->GetBinContent(i+1);
   xs_HT_fat_WW_1010_sys_E_u1[i] = limit_WW_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_WW_1010_sys_E_u2[i] = limit_WW_twosigma_uppers->GetBinContent(i+1);

 }


 TH1F* limit_WZ_twosigma_lowers = file_exp_WZ6->Get("htwosigma_lowers;1");
 TH1F* limit_WZ_onesigma_lowers = file_exp_WZ6->Get("honesigma_lowers;1");
 TH1F* limit_WZ_medians = file_exp_WZ6->Get("hmedians;1");
 TH1F* limit_WZ_onesigma_uppers = file_exp_WZ6->Get("honesigma_uppers;1");
 TH1F* limit_WZ_twosigma_uppers = file_exp_WZ6->Get("htwosigma_uppers;1");

 for (int i = 1; i < limit_WZ_twosigma_lowers->GetNbinsX()+1; i++){

   limit_WZ_twosigma_lowers->SetBinContent(i, limit_WZ_twosigma_lowers->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));
   limit_WZ_onesigma_lowers->SetBinContent(i, limit_WZ_onesigma_lowers->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));
   limit_WZ_medians->SetBinContent(i, limit_WZ_medians->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));
   limit_WZ_onesigma_uppers->SetBinContent(i, limit_WZ_onesigma_uppers->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));
   limit_WZ_twosigma_uppers->SetBinContent(i, limit_WZ_twosigma_uppers->GetBinContent(i) / limit_efficiencies_WZ->GetBinContent(i));
 }
 
 double* xs_HT_fat_WZ_1010_sys_E_d2 = new double[nEBins];
 double* xs_HT_fat_WZ_1010_sys_E_d1 = new double[nEBins];
 double* xs_HT_fat_WZ_1010_sys_E_m = new double[nEBins];
 double* xs_HT_fat_WZ_1010_sys_E_u1 = new double[nEBins];
 double* xs_HT_fat_WZ_1010_sys_E_u2 = new double[nEBins];

 
 for (int i = 0 ; i < nEBins; i++){

   xs_HT_fat_WZ_1010_sys_E_d2[i] = limit_WZ_twosigma_lowers->GetBinContent(i+1);
   xs_HT_fat_WZ_1010_sys_E_d1[i] = limit_WZ_onesigma_lowers->GetBinContent(i+1);
   xs_HT_fat_WZ_1010_sys_E_m[i] = limit_WZ_medians->GetBinContent(i+1);
   xs_HT_fat_WZ_1010_sys_E_u1[i] = limit_WZ_onesigma_uppers->GetBinContent(i+1);
   xs_HT_fat_WZ_1010_sys_E_u2[i] = limit_WZ_twosigma_uppers->GetBinContent(i+1);

 }
 
 file_exp_ZZ6->Close();
 file_exp_WW6->Close();
 file_exp_WZ6->Close();


 // 0.16751, 0.15361, 0.14070, 0.12884, 0.11799, 0.10813
 //   0.19590, 0.21457, 
// double WZ_ratio[nEBins] = {0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.44, 0.45, 0.46, 0.47, 0.48}; //{0.5561,0.5116,0.4671, 0.42, 0.38};
// double ZZ_ratio[nEBins] = {0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.56, 0.55, 0.55, 0.53, 0.52};
 
 double xs_HT_pf_WW_compare[nEBins], xs_HT_pf_WZ_compare[nEBins], xs_HT_pf_ZZ_compare[nEBins], xs_HT_fat_WW_compare[nEBins], xs_HT_fat_WZ_compare[nEBins], xs_HT_fat_ZZ_compare[nEBins], xs_comparison_bw_stat_bg_fat[nEBins], xs_comparison_bw_stat_bg2_fat[nEBins], xs_comparison_bw_stat_bg3_fat[nEBins], xs_comparison_bw_stat_bg4_fat[nEBins], xs_comparison_bw_stat_bg5_fat[nEBins], xs_comparison_bw_stat_bg6_fat[nEBins], xs_comparison_bw_stat_bg7_fat[nEBins], xs_comparison_bw_stat_bg8_fat[nEBins], xs_comparison_bw_stat_lumi_fat[nEBins], xs_comparison_bw_stat_JES_fat[nEBins], xs_comparison_bw_stat_JER_fat[nEBins], xs_comparison_bw_stat_sys_fat[nEBins], xs_RSG_for_limit_fat[nEBins], xs_RSG_WW_over_WW_fat[nEBins], xs_string_over_WW_fat[nEBins], xs_diquark_over_ZZ_fat[nEBins], xs_RSG_ZZ_over_ZZ_fat[nEBins], xs_zprime_over_ZZ_fat[nEBins], xs_wprime_over_ZZ_fat[nEBins], xs_Wprime_WZ_over_WZ_fat[nEBins];

 std::cout << "wide stat only" << std::endl;
 for (int  i = 0 ; i < nEBins ; i++) 
   {
     std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_ZZ_1010[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_WW_1010[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_WZ_1010[i] << " \\\\" << std::endl;
     
   }
 
 
 std::cout << "wide sys included" << std::endl;
 for (int  i = 0 ; i < nEBins ; i++) 
   {
     std::cout << std::fixed << setprecision(1) << x[i]/1000.0 <<" & " << std::fixed << setprecision(5) << xs_HT_fat_ZZ_1010_sys[i]<<" & " << std::fixed << setprecision(5) << xs_HT_fat_WW_1010_sys[i] <<" & " << std::fixed << setprecision(5) << xs_HT_fat_WZ_1010_sys[i] << " \\\\" << std::endl;
     
   }
 
////////////////// Histogram definition /////////////////////
/////////////////////////////////////////////////////////////


  // expected limit for WW

  TH1F *h_xs_HT_fat_WW_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_WW_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_WW_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_WW_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

  // expected limit for ZZ

  TH1F *h_xs_HT_fat_ZZ_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_ZZ_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_ZZ_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_ZZ_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

  // expected limit for WZ

  TH1F *h_xs_HT_fat_WZ_1010_sys_E_d2 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_WZ_1010_sys_E_d1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_WZ_1010_sys_E_u1 = new TH1F("","",nEBins,minMass,maxMass);
  TH1F *h_xs_HT_fat_WZ_1010_sys_E_u2 = new TH1F("","",nEBins,minMass,maxMass);

		 

 for (int i = 0; i<nEBins; i++) {
   
   std::cout << i << " & " << std::fixed << setprecision(3) << xs_HT_fat_ZZ_1010_sys[i] << " & " << xs_HT_fat_WW_1010_sys[i] << " & " << xs_HT_fat_WZ_1010_sys[i] << " \\\\  " << std::endl;
   //xs_RSG_for_limit_fat[i] = xs_HT_fat_WZ_1010_sys[i]*WZ_ratio[i] + xs_HT_fat_ZZ_1010_sys[i]*ZZ_ratio[i];
   //xs_rsg_over_WZ_fat[i] = rsgraviton[i]/xs_RSG_for_limit_fat[i];
     
   xs_comparison_bw_stat_bg_fat[i] = xs_HT_fat_WW_1010_bg[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg2_fat[i] = xs_HT_fat_WW_1010_bg2[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg3_fat[i] = xs_HT_fat_WW_1010_bg3[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg4_fat[i] = xs_HT_fat_WW_1010_bg4[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg5_fat[i] = xs_HT_fat_WW_1010_bg5[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg6_fat[i] = xs_HT_fat_WW_1010_bg6[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg7_fat[i] = xs_HT_fat_WW_1010_bg7[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_bg8_fat[i] = xs_HT_fat_WW_1010_bg8[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_lumi_fat[i] = xs_HT_fat_WW_1010_lumi[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_JES_fat[i] = xs_HT_fat_WW_1010_JES[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_JER_fat[i] = xs_HT_fat_WW_1010_JER[i]/xs_HT_fat_WW_1010[i];
   xs_comparison_bw_stat_sys_fat[i] = xs_HT_fat_WW_1010_sys[i]/xs_HT_fat_WW_1010[i];

   xs_RSG_WW_over_WW_fat[i] = RSG_WW_f->Eval(x[i])/xs_HT_fat_WW_1010_sys[i];
   //xs_string_over_WW_fat[i] = string[i]/xs_HT_fat_WW_1010_sys[i];
   //xs_diquark_over_ZZ_fat[i] = diquark[i]/xs_HT_fat_ZZ_1010_sys[i];
   xs_RSG_ZZ_over_ZZ_fat[i] = RSG_ZZ_f->Eval(x[i])/xs_HT_fat_ZZ_1010_sys[i];
   //xs_zprime_over_ZZ_fat[i] = zprime[i]/xs_HT_fat_ZZ_1010_sys[i];
   //xs_wprime_over_ZZ_fat[i] = wprime[i]/xs_HT_fat_ZZ_1010_sys[i];
   xs_Wprime_WZ_over_WZ_fat[i] = Wprime_WZ_f->Eval(x[i])/xs_HT_fat_WZ_1010_sys[i];
   
   h_xs_HT_fat_WW_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_WW_1010_sys_E_d2[i]);
   h_xs_HT_fat_WW_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_WW_1010_sys_E_d1[i]);
   h_xs_HT_fat_WW_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_WW_1010_sys_E_u1[i]);
   h_xs_HT_fat_WW_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_WW_1010_sys_E_u2[i]);

   h_xs_HT_fat_ZZ_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_ZZ_1010_sys_E_d2[i]);
   h_xs_HT_fat_ZZ_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_ZZ_1010_sys_E_d1[i]);
   h_xs_HT_fat_ZZ_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_ZZ_1010_sys_E_u1[i]);
   h_xs_HT_fat_ZZ_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_ZZ_1010_sys_E_u2[i]);

   h_xs_HT_fat_WZ_1010_sys_E_d2->SetBinContent(i+1,xs_HT_fat_WZ_1010_sys_E_d2[i]);
   h_xs_HT_fat_WZ_1010_sys_E_d1->SetBinContent(i+1,xs_HT_fat_WZ_1010_sys_E_d1[i]);
   h_xs_HT_fat_WZ_1010_sys_E_u1->SetBinContent(i+1,xs_HT_fat_WZ_1010_sys_E_u1[i]);
   h_xs_HT_fat_WZ_1010_sys_E_u2->SetBinContent(i+1,xs_HT_fat_WZ_1010_sys_E_u2[i]);

 }

//////////////////////////////////////////////////////
////////////////// TGraph define /////////////////////
//////////////////////////////////////////////////////

// stat only 

  g_xs_HT_fat_WW_1010 = new TGraph(nEBins,x,xs_HT_fat_WW_1010);
  g_xs_HT_fat_WZ_1010 = new TGraph(nEBins,x,xs_HT_fat_WZ_1010);
  g_xs_HT_fat_ZZ_1010 = new TGraph(nEBins,x,xs_HT_fat_ZZ_1010);

  // all sys inlcuded

  g_xs_HT_fat_WW_1010_sys = new TGraph(nEBins,x,xs_HT_fat_WW_1010_sys);
  g_xs_HT_fat_WZ_1010_sys = new TGraph(nEBins,x,xs_HT_fat_WZ_1010_sys);
  g_xs_HT_fat_ZZ_1010_sys = new TGraph(nEBins,x,xs_HT_fat_ZZ_1010_sys);
  g_xs_HT_fat_ZZ_01_1010_sys = new TGraph(nEBins,x,xs_HT_fat_ZZ_01_1010_sys);


  // fractional differnce only

  g_xs_comparison_bw_stat_bg_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg_fat);
  g_xs_comparison_bw_stat_bg2_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg2_fat);
  g_xs_comparison_bw_stat_bg3_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg3_fat);
  g_xs_comparison_bw_stat_bg4_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg4_fat);
  g_xs_comparison_bw_stat_bg5_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg5_fat);
  g_xs_comparison_bw_stat_bg6_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg6_fat);
  g_xs_comparison_bw_stat_bg7_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg7_fat);
  g_xs_comparison_bw_stat_bg8_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_bg8_fat);
  g_xs_comparison_bw_stat_lumi_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_lumi_fat);
  g_xs_comparison_bw_stat_JES_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_JES_fat);
  g_xs_comparison_bw_stat_JER_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_JER_fat);
  g_xs_comparison_bw_stat_sys_fat = new TGraph(nEBins,x,xs_comparison_bw_stat_sys_fat);

  // theory value

  g_xs_RSG_WW = new TGraph(en,mjj,RSG_WW);
  g_xs_RSG_ZZ = new TGraph(en,mjj,RSG_ZZ);
  g_xs_RSG_ZZ_ex = new TGraph(en_short,mjjshort,RSG_ZZ_ex);
  g_xs_Wprime_WZ = new TGraph(en,mjj,Wprime_WZ);
  //g_xs_diquark = new TGraph(en,mjj,diquark);
  //g_xs_string = new TGraph(en,mjj,string);
  //g_xs_zprime = new TGraph(9,mjj,zprime);
  //g_xs_wprime = new TGraph(11,mjj,wprime);
  //g_xs_rsgraviton = new TGraph(8,mjj,rsgraviton);

  // comparison b/w different Jet combinations.

  g_xs_HT_fat_WW_compare = new TGraph(nEBins,x,xs_HT_fat_WW_compare);
  g_xs_HT_fat_WZ_compare = new TGraph(nEBins,x,xs_HT_fat_WZ_compare);
  g_xs_HT_fat_ZZ_compare = new TGraph(nEBins,x,xs_HT_fat_ZZ_compare);
  
  // comparison b/w sys.included cross section value and theory

   g_xs_RSG_WW_over_WW_fat = new TGraph(31,x,xs_RSG_WW_over_WW_fat);
   //g_xs_string_over_WW_fat = new TGraph(nEBins,x,xs_string_over_WW_fat);
   //g_xs_diquark_over_ZZ_fat = new TGraph(31,x,xs_diquark_over_ZZ_fat);
   g_xs_RSG_ZZ_over_ZZ_fat = new TGraph(31,x,xs_RSG_ZZ_over_ZZ_fat);
   //g_xs_zprime_over_ZZ_fat = new TGraph(nEBins,x,xs_zprime_over_ZZ_fat);
   //g_xs_wprime_over_ZZ_fat = new TGraph(nEBins,x,xs_wprime_over_ZZ_fat);
   //g_xs_rsg_over_WZ_fat = new TGraph(5,x,xs_rsg_over_WZ_fat);
   g_xs_Wprime_WZ_over_WZ_fat = new TGraph(31,x,xs_Wprime_WZ_over_WZ_fat);


  // expected limit for WW
  g_xs_HT_fat_WW_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_WW_1010_sys_E_m);

  // expected limit for ZZ
  g_xs_HT_fat_ZZ_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_ZZ_1010_sys_E_m);

  // expected limit for WZ
  g_xs_HT_fat_WZ_1010_sys_E_m = new TGraph(nEBins,x,xs_HT_fat_WZ_1010_sys_E_m);

  // RSG try

  //double x_RSG[nEBins]= {600., 700., 800., 900., 1000.0, 1100.0, 1200.0, 1300., 1400., 1500., 1600., 1700., 1800., 1900., 2000.};
  //g_xs_RSG_for_limit_fat = new TGraph(nEBins,x_RSG,xs_RSG_for_limit_fat);

//////////////////////////////////////////////////////
////////////////// TGraph design /////////////////////
//////////////////////////////////////////////////////

// stat only

  g_xs_HT_fat_WW_1010->SetLineColor(ci_g);
  g_xs_HT_fat_WW_1010->SetLineStyle(6);
  g_xs_HT_fat_WW_1010->SetLineWidth(3);
  g_xs_HT_fat_WW_1010->SetMarkerStyle(24);
  g_xs_HT_fat_WW_1010->SetMarkerColor(ci_g);

  g_xs_HT_fat_WZ_1010->SetLineColor(ci_g);
  g_xs_HT_fat_WZ_1010->SetLineStyle(6);
  g_xs_HT_fat_WZ_1010->SetLineWidth(3);
  g_xs_HT_fat_WZ_1010->SetMarkerStyle(24);
  g_xs_HT_fat_WZ_1010->SetMarkerColor(ci_g);
  
  g_xs_HT_fat_ZZ_1010->SetLineColor(ci_g);
  g_xs_HT_fat_ZZ_1010->SetLineStyle(6);
  g_xs_HT_fat_ZZ_1010->SetLineWidth(3);
  g_xs_HT_fat_ZZ_1010->SetMarkerStyle(24);
  g_xs_HT_fat_ZZ_1010->SetMarkerColor(ci_g);

  // HT-fat all sys

  g_xs_HT_fat_WW_1010_sys->SetLineColor(2);
  g_xs_HT_fat_WW_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_WW_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_WW_1010_sys->SetMarkerStyle(20);
  g_xs_HT_fat_WW_1010_sys->SetMarkerColor(2);

  g_xs_HT_fat_WZ_1010_sys->SetLineColor(ci_g);
  g_xs_HT_fat_WZ_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_WZ_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_WZ_1010_sys->SetMarkerStyle(24);
  g_xs_HT_fat_WZ_1010_sys->SetMarkerColor(ci_g);
  
  g_xs_HT_fat_ZZ_1010_sys->SetLineColor(4);
  g_xs_HT_fat_ZZ_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_ZZ_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_ZZ_1010_sys->SetMarkerStyle(25);
  g_xs_HT_fat_ZZ_1010_sys->SetMarkerColor(4);

  g_xs_HT_fat_ZZ_01_1010_sys->SetLineColor(8);
  g_xs_HT_fat_ZZ_01_1010_sys->SetLineStyle(1);
  g_xs_HT_fat_ZZ_01_1010_sys->SetLineWidth(3);
  g_xs_HT_fat_ZZ_01_1010_sys->SetMarkerStyle(25);
  g_xs_HT_fat_ZZ_01_1010_sys->SetMarkerColor(8);


  // Expected HT-WW stat only

  g_xs_HT_fat_WW_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_WW_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_WW_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_WW_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_WW_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_WW_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_WW_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_WW_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_WW_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_WW_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_WW_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_WW_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_WW_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_WW_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_WW_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_WW_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_WW_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_WW_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_WW_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_WW_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_WW_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_WW_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_WW_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_WW_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_WW_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_WW_1010_sys_E_u2->SetFillStyle(21);

  // Expected HT-ZZ stat only

  g_xs_HT_fat_ZZ_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_ZZ_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_ZZ_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_ZZ_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_ZZ_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_ZZ_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_ZZ_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_ZZ_1010_sys_E_u2->SetFillStyle(21);

  // Expected HT-WZ stat only

  g_xs_HT_fat_WZ_1010_sys_E_m->SetLineColor(1);
  g_xs_HT_fat_WZ_1010_sys_E_m->SetLineStyle(10);
  g_xs_HT_fat_WZ_1010_sys_E_m->SetLineWidth(3);
  g_xs_HT_fat_WZ_1010_sys_E_m->SetMarkerStyle(1);
  g_xs_HT_fat_WZ_1010_sys_E_m->SetMarkerColor(1);

  h_xs_HT_fat_WZ_1010_sys_E_d2->SetLineColor(0);
  h_xs_HT_fat_WZ_1010_sys_E_d2->SetLineStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_d2->SetLineWidth(3);
  h_xs_HT_fat_WZ_1010_sys_E_d2->SetMarkerStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_d2->SetMarkerColor(41);
  h_xs_HT_fat_WZ_1010_sys_E_d2->SetFillColor(10);
  //  h_xs_HT_fat_WZ_1010_sys_E_d2->SetFillStyle(21);

  h_xs_HT_fat_WZ_1010_sys_E_d1->SetLineColor(41);
  h_xs_HT_fat_WZ_1010_sys_E_d1->SetLineStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_d1->SetLineWidth(3);
  h_xs_HT_fat_WZ_1010_sys_E_d1->SetMarkerStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_d1->SetMarkerColor(5);
  h_xs_HT_fat_WZ_1010_sys_E_d1->SetFillColor(41);
  //  h_xs_HT_fat_WZ_1010_sys_E_d1->SetFillStyle(21);

  h_xs_HT_fat_WZ_1010_sys_E_u1->SetLineColor(5);
  h_xs_HT_fat_WZ_1010_sys_E_u1->SetLineStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_u1->SetLineWidth(3);
  h_xs_HT_fat_WZ_1010_sys_E_u1->SetMarkerStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_u1->SetMarkerColor(5);
  h_xs_HT_fat_WZ_1010_sys_E_u1->SetFillColor(5);
  //  h_xs_HT_fat_WZ_1010_sys_E_u1->SetFillStyle(21);

  h_xs_HT_fat_WZ_1010_sys_E_u2->SetLineColor(41);
  h_xs_HT_fat_WZ_1010_sys_E_u2->SetLineStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_u2->SetLineWidth(3);
  h_xs_HT_fat_WZ_1010_sys_E_u2->SetMarkerStyle(1);
  h_xs_HT_fat_WZ_1010_sys_E_u2->SetMarkerColor(41);
  h_xs_HT_fat_WZ_1010_sys_E_u2->SetFillColor(41);
  //  h_xs_HT_fat_WZ_1010_sys_E_u2->SetFillStyle(21);


  // Fractional difference b/w sys and stat.(fat)

  g_xs_comparison_bw_stat_bg_fat->SetLineColor(2);
  g_xs_comparison_bw_stat_bg_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_bg_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg_fat->SetMarkerColor(2);

  g_xs_comparison_bw_stat_bg2_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg2_fat->SetLineStyle(1);
  g_xs_comparison_bw_stat_bg2_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg2_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg2_fat->SetMarkerColor(8);

  g_xs_comparison_bw_stat_bg3_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg3_fat->SetLineStyle(2);
  g_xs_comparison_bw_stat_bg3_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg3_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg3_fat->SetMarkerColor(8);

  g_xs_comparison_bw_stat_bg4_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg4_fat->SetLineStyle(3);
  g_xs_comparison_bw_stat_bg4_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg4_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg4_fat->SetMarkerColor(8);

  g_xs_comparison_bw_stat_bg5_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg5_fat->SetLineStyle(4);
  g_xs_comparison_bw_stat_bg5_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg5_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg5_fat->SetMarkerColor(8);

  g_xs_comparison_bw_stat_bg6_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg6_fat->SetLineStyle(5);
  g_xs_comparison_bw_stat_bg6_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg6_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg6_fat->SetMarkerColor(8);

  g_xs_comparison_bw_stat_bg7_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg7_fat->SetLineStyle(6);
  g_xs_comparison_bw_stat_bg7_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg7_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg7_fat->SetMarkerColor(8);

  g_xs_comparison_bw_stat_bg8_fat->SetLineColor(8);
  g_xs_comparison_bw_stat_bg8_fat->SetLineStyle(7);
  g_xs_comparison_bw_stat_bg8_fat->SetLineWidth(3);
  g_xs_comparison_bw_stat_bg8_fat->SetMarkerStyle(24);
  g_xs_comparison_bw_stat_bg8_fat->SetMarkerColor(8);

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

  g_xs_HT_fat_WW_compare->SetLineColor(ci_g);
  g_xs_HT_fat_WW_compare->SetLineStyle(6);
  g_xs_HT_fat_WW_compare->SetLineWidth(3);
  g_xs_HT_fat_WW_compare->SetMarkerStyle(24);
  g_xs_HT_fat_WW_compare->SetMarkerColor(ci_g);

  g_xs_HT_fat_WZ_compare->SetLineColor(ci_g);
  g_xs_HT_fat_WZ_compare->SetLineStyle(6);
  g_xs_HT_fat_WZ_compare->SetLineWidth(3);
  g_xs_HT_fat_WZ_compare->SetMarkerStyle(24);
  g_xs_HT_fat_WZ_compare->SetMarkerColor(ci_g);
  
  g_xs_HT_fat_ZZ_compare->SetLineColor(ci_g);
  g_xs_HT_fat_ZZ_compare->SetLineStyle(6);
  g_xs_HT_fat_ZZ_compare->SetLineWidth(3);
  g_xs_HT_fat_ZZ_compare->SetMarkerStyle(24);
  g_xs_HT_fat_ZZ_compare->SetMarkerColor(ci_g);

  //g_xs_RSG_for_limit_fat->SetLineWidth(3);
  //g_xs_RSG_for_limit_fat->SetMarkerStyle(24);

  /// Theory values

  //g_xs_string->SetLineColor(9);
  //g_xs_string->SetLineStyle(8);
  //g_xs_string->SetLineWidth(3);

  g_xs_RSG_WW->SetLineColor(1);
  g_xs_RSG_WW->SetLineStyle(2);
  g_xs_RSG_WW->SetLineWidth(3);

  g_xs_RSG_ZZ->SetLineColor(7);
  g_xs_RSG_ZZ->SetLineStyle(3);
  g_xs_RSG_ZZ->SetLineWidth(3);
  
  g_xs_RSG_ZZ_ex->SetLineColor(8);
  g_xs_RSG_ZZ_ex->SetLineStyle(3);
  g_xs_RSG_ZZ_ex->SetLineWidth(3);
  
  g_xs_Wprime_WZ->SetLineColor(2);
  g_xs_Wprime_WZ->SetLineStyle(5);
  g_xs_Wprime_WZ->SetLineWidth(3);
  
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

  //g_xs_string_over_WW_fat->SetLineColor(9);
  //g_xs_string_over_WW_fat->SetLineWidth(3);

  g_xs_RSG_WW_over_WW_fat->SetLineColor(1);
  g_xs_RSG_WW_over_WW_fat->SetLineWidth(3);

  g_xs_RSG_ZZ_over_ZZ_fat->SetLineColor(7);
  g_xs_RSG_ZZ_over_ZZ_fat->SetLineWidth(3);
  
  g_xs_Wprime_WZ_over_WZ_fat->SetLineColor(7);
  g_xs_Wprime_WZ_over_WZ_fat->SetLineWidth(3);
  
  //g_xs_diquark_over_ZZ_fat->SetLineColor(8);
  //g_xs_diquark_over_ZZ_fat->SetLineWidth(3);
  
  //g_xs_wprime_over_ZZ_fat->SetLineColor(46);
  //g_xs_wprime_over_ZZ_fat->SetLineWidth(3);

  //g_xs_zprime_over_ZZ_fat->SetLineColor(38);
  //g_xs_zprime_over_ZZ_fat->SetLineWidth(3);

  //g_xs_rsg_over_WZ_fat->SetLineColor(29);
  //g_xs_rsg_over_WZ_fat->SetLineWidth(3);


///////////////// drawing canvas /////////////////////
//////////////////////////////////////////////////////


  // c_xs_WW

  c_xs_WW->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
   // vFrame->SetLogy();

  g_xs_HT_fat_WW_1010_sys->DrawClone("PL");
  //  g_xs_HT_pf_WW_1010_sys->DrawClone("samePL");
  //  g_xs_HT_calo_WW_1010_sys->DrawClone("samePL");

  //g_xs_string->Draw("sameL");  
  g_xs_RSG_WW->Draw("sameL");
  //g_xs_RSG_ZZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
  //g_xs_Wprime_WZ->Draw("sameL");
	
  l_xs_WW->SetFillColor(0);
  l_xs_WW->SetFillStyle(0);
  l_xs_WW->SetHeader("WW-(Sys)-4.7 fb^{-1}");
  //  l_xs_WW->AddEntry(g_xs_HT_calo_WW_1010,"HT-calo 95% CL","pL");
  //  l_xs_WW->AddEntry(g_xs_HT_pf_WW_1010,"HT-PF 95% CL","pL");
  l_xs_WW->AddEntry(g_xs_HT_fat_WW_1010,"HT-fat 95% CL","pL");

  l_xs_theory->SetFillColor(0);
  l_xs_theory->SetFillStyle(0);
  //l_xs_theory->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory->AddEntry(g_xs_RSG_WW,"RS Graviton -> WW ","L");
  //l_xs_theory->AddEntry(g_xs_RSG_ZZ,"RS Graviton -> ZZ","L");
  //l_xs_theory->AddEntry(g_xs_RSG_ZZ,"(NLO) arXiv:1111.7261v2","");
  //l_xs_theory->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  //l_xs_theory->AddEntry(g_xs_wprime,"W'","L");
  //l_xs_theory->AddEntry(g_xs_zprime,"Z'","L");
  //l_xs_theory->AddEntry(g_xs_rsgraviton,"RS Graviton","L");
  //l_xs_theory->AddEntry(g_xs_Wprime_WZ,"W' -> WZ","L");

  l_xs_WW->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_WW->Update();
  c_xs_WW->SaveAs("Fit_Results/Exclusion_WW_2Vtag.png");
  c_xs_WW->SaveAs("Fit_Results/Exclusion_WW_2Vtag.pdf");

  // c_xs_WZ

  c_xs_WZ->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  g_xs_HT_fat_WZ_1010_sys->DrawClone("PL");
  //  g_xs_HT_pf_WZ_1010->Draw("samePL");
  //  g_xs_HT_calo_WZ_1010->Draw("samePL");

  //g_xs_string->Draw("sameL");  
  //g_xs_RSG_WW->Draw("sameL");
  //g_xs_RSG_ZZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
  g_xs_Wprime_WZ->Draw("sameL");
	
  l_xs_WZ->SetFillColor(0);
  l_xs_WZ->SetFillStyle(0);
  l_xs_WZ->SetHeader("WZ-(Sys)-4.7 fb^{-1}");
  //  l_xs_WZ->AddEntry(g_xs_HT_calo_WZ_1010,"HT-calo 95% CL","pL");
  //  l_xs_WZ->AddEntry(g_xs_HT_pf_WZ_1010,"HT-PF 95% CL","pL");
  l_xs_WZ->AddEntry(g_xs_HT_fat_WZ_1010,"HT-fat 95% CL","pL");

  l_xs_WZ->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_WZ->Update();
  c_xs_WZ->SaveAs("Fit_Results/Exclusion_WZ_2Vtag.png");
  c_xs_WZ->SaveAs("Fit_Results/Exclusion_WZ_2Vtag.pdf");
  
  // c_xs_ZZ
  
  c_xs_ZZ->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");
  // vFrame->SetLogy();

  //  g_xs_HT_calo_ZZ_1010->Draw("PL");
  //  g_xs_HT_pf_ZZ_1010->Draw("samePL");
  g_xs_HT_fat_ZZ_1010_sys->DrawClone("PL");

  //g_xs_string->Draw("sameL");  
  //g_xs_RSG_WW->Draw("sameL");
  g_xs_RSG_ZZ->Draw("sameL");
  g_xs_RSG_ZZ_ex->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
	
  l_xs_ZZ->SetFillColor(0);
  l_xs_ZZ->SetFillStyle(0);
  l_xs_ZZ->SetHeader("ZZ-(Sys)-4.7 fb^{-1}");
  //  l_xs_ZZ->AddEntry(g_xs_HT_calo_ZZ_1010,"HT-calo 95% CL","pL");
  //  l_xs_ZZ->AddEntry(g_xs_HT_pf_ZZ_1010,"HT-PF 95% CL","pL");
  l_xs_ZZ->AddEntry(g_xs_HT_fat_ZZ_1010,"HT-fat 95% CL","pL");
  l_xs_ZZ->Draw("sames");
  l_xs_theory->Draw("sames");

  c_xs_ZZ->Update();
  c_xs_ZZ->SaveAs("Fit_Results/Exclusion_ZZ_2Vtag.png");
  c_xs_ZZ->SaveAs("Fit_Results/Exclusion_ZZ_2Vtag.pdf");

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

  g_xs_HT_fat_WZ_1010->DrawClone("PL");
  g_xs_HT_fat_ZZ_1010->DrawClone("samePL");
  g_xs_HT_fat_WW_1010->DrawClone("samePL");

  //g_xs_string->Draw("sameL");  
  g_xs_RSG_WW->Draw("sameL");
  g_xs_RSG_ZZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
  g_xs_Wprime_WZ->Draw("sameL");
	
  l_xs_stat_fat->SetFillColor(0);
  l_xs_stat_fat->SetFillStyle(0);
  //  l_xs_stat_fat->SetHeader("CMS Preliminary");
  //  l_xs_stat_fat->AddEntry((TObject*)0,"4.7 fb^{-1}","");
  l_xs_stat_fat->SetHeader("95% CL Upper Limit (stat only)");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_WW_1010,"WW","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_ZZ_1010,"ZZ","pL");
  l_xs_stat_fat->AddEntry(g_xs_HT_fat_WZ_1010,"WZ","pL");
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
 
  c_xs_stat_fat->SaveAs("Fit_Results/Exclusion_StatOnly_2Vtag.png");
  c_xs_stat_fat->SaveAs("Fit_Results/Exclusion_StatOnly_2Vtag.pdf");

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

  g_xs_HT_fat_WZ_1010_sys->Draw("PL");
  g_xs_HT_fat_ZZ_1010_sys->Draw("samePL");
  g_xs_HT_fat_ZZ_01_1010_sys->Draw("samePL");
  g_xs_HT_fat_WW_1010_sys->Draw("samePL");

  //g_xs_string->Draw("sameL");  
  g_xs_RSG_WW->Draw("sameL");
  g_xs_RSG_ZZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  
  g_xs_Wprime_WZ->Draw("sameL");
	
  l_xs_sys->SetFillColor(0);
  l_xs_sys->SetFillStyle(0);
  l_xs_sys->SetTextSize(0.03484321);
  //  l_xs_sys->SetHeader("CMS Preliminary");
  //  l_xs_sys->AddEntry((TObject*)0,"4.7 fb^{-1}","");
  l_xs_sys->SetHeader("95% CL Upper Limit (sys)");
  l_xs_sys->AddEntry(g_xs_HT_fat_WW_1010_sys,"WW","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_ZZ_1010_sys,"ZZ","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_ZZ_01_1010_sys,"ZZ (Fast)","pL");
  l_xs_sys->AddEntry(g_xs_HT_fat_WZ_1010_sys,"WZ","pL");
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

  c_xs_sys->SaveAs("Fit_Results/Exclusion_Sys_2Vtag.png");
  c_xs_sys->SaveAs("Fit_Results/Exclusion_Sys_2Vtag.pdf");

  // c_xs_all_fat

  c_xs_all_fat->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,minXsec,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  g_xs_HT_fat_WZ_1010->Draw("PL");
  g_xs_HT_fat_ZZ_1010->Draw("PL");
  g_xs_HT_fat_WW_1010->Draw("samePL");
  g_xs_HT_fat_WZ_1010_sys->Draw("samePL");
  g_xs_HT_fat_ZZ_1010_sys->Draw("samePL");
  g_xs_HT_fat_WW_1010_sys->Draw("samePL");
  /*
  g_xs_string->Draw("PL");  
  g_xs_RSG_WW->Draw("sameL");
  g_xs_RSG_ZZ->Draw("sameL");
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
  l_xs_all_fat->AddEntry(g_xs_HT_fat_WZ_1010,"WZ 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_WW_1010,"WW 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_ZZ_1010,"ZZ 95% CL (stat.only)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_WZ_1010_sys,"WZ 95% CL (sys.included)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_WW_1010_sys,"WW 95% CL (sys.included)","pL");
  l_xs_all_fat->AddEntry(g_xs_HT_fat_ZZ_1010_sys,"ZZ 95% CL (sys.included)","pL");
  l_xs_all_fat->Draw("sames");
  /*
  l_xs_theory->Draw("sames");
  */
  c_xs_all_fat->Update();
  c_xs_all_fat->SaveAs("Fit_Results/Exclusion_Stat_vs_Sys_2Vtag.png");
  c_xs_all_fat->SaveAs("Fit_Results/Exclusion_Stat_vs_Sys_2Vtag.pdf");

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

  //g_xs_string_over_WW_fat->Draw("L");  
  g_xs_RSG_WW_over_WW_fat->Draw("sameL");
  g_xs_RSG_ZZ_over_ZZ_fat->Draw("sameL");
  //g_xs_diquark_over_ZZ_fat->Draw("sameL");
  //g_xs_wprime_over_ZZ_fat->Draw("sameL");  
  //g_xs_zprime_over_ZZ_fat->Draw("sameL");  
  //g_xs_rsg_over_WZ_fat->Draw("sameL");  
  g_xs_Wprime_WZ_over_WZ_fat->Draw("sameL");

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
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_string_over_WW_fat,"String Resonance","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_RSG_WW_over_WW_fat,"RS Graviton -> WW","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_RSG_ZZ_over_ZZ_fat,"RS Graviton -> ZZ","L");
  l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_Wprime_WZ_over_WZ_fat,"W' -> WZ","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_RSG_ZZ_over_ZZ_fat,"(NLO) arXiv:1111.7261v2","");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_diquark_over_ZZ_fat,"E_{6} Diquark","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_wprime_over_ZZ_fat,"W'","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_zprime_over_ZZ_fat,"Z'","L");
  //l_xs_comparison_bw_sys_theory_fat->AddEntry(g_xs_rsg_over_WZ_fat,"RS Graviton","L");
  l_xs_comparison_bw_sys_theory_fat->Draw("sames");
  c_xs_comparison_bw_sys_theory_fat->Update();

  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results/Exclusions_vs_Theory_2Vtag.png");
  c_xs_comparison_bw_sys_theory_fat->SaveAs("Fit_Results/Exclusions_vs_Theory_2Vtag.pdf");
  // c_xs_WW_sys_E

  THStack *h_xs_WW_sys_E = new THStack("h_xs_WW_sys_E","h_xs_WW_sys_E");
  TH1F *htemp_xs_WW_sys_E_u2 = (TH1F*)h_xs_HT_fat_WW_1010_sys_E_u2->Clone("htemp_xs_WW_sys_E_u2");
  TH1F *htemp_xs_WW_sys_E_u1 = (TH1F*)h_xs_HT_fat_WW_1010_sys_E_u1->Clone("htemp_xs_WW_sys_E_u1");  
  TH1F *htemp_xs_WW_sys_E_d1 = (TH1F*)h_xs_HT_fat_WW_1010_sys_E_d1->Clone("htemp_xs_WW_sys_E_d1");   
  htemp_xs_WW_sys_E_u2->Add(h_xs_HT_fat_WW_1010_sys_E_u1,-1);
  htemp_xs_WW_sys_E_u1->Add(h_xs_HT_fat_WW_1010_sys_E_d1,-1);
  htemp_xs_WW_sys_E_d1->Add(h_xs_HT_fat_WW_1010_sys_E_d2,-1);

  h_xs_WW_sys_E->Add(h_xs_HT_fat_WW_1010_sys_E_d2);
  h_xs_WW_sys_E->Add(htemp_xs_WW_sys_E_d1);
  h_xs_WW_sys_E->Add(htemp_xs_WW_sys_E_u1);
  h_xs_WW_sys_E->Add(htemp_xs_WW_sys_E_u2);
  h_xs_WW_sys_E->SetMinimum(minXsec);
  h_xs_WW_sys_E->SetMaximum(maxXsec);

  c_xs_WW_sys_E->cd(0);

  h_xs_HT_fat_WW_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_WW_sys_E->SetTitle("");
  h_xs_HT_fat_WW_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_WW_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_WW_sys_E->Draw("C");

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

  h_xs_WW_sys_E->GetXaxis()->SetTitle("WW Resonance Mass (GeV)");
  h_xs_WW_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");

  // vFrame->SetLogy();

  //  h_xs_HT_fat_WW_1010_sys_E_u1->Draw("sameC");
  //  h_xs_HT_fat_WW_1010_sys_E_d1->Draw("sameC");
  //  h_xs_HT_fat_WW_1010_sys_E_d2->Draw("sameC");  
  g_xs_HT_fat_WW_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_WW_1010_sys->Draw("samePL");

  //g_xs_string->Draw("samePL");  
  g_xs_RSG_WW->Draw("samePL");
  //  g_xs_RSG_ZZ->Draw("sameL");
  //  g_xs_diquark->Draw("sameL");
  //  g_xs_wprime->Draw("sameL");  
  //  g_xs_zprime->Draw("sameL");  
  //  g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_WW_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_WW_sys_E->Draw();

  p_xs_WW_sys_E->SetFillColor(0);
  p_xs_WW_sys_E->SetFillStyle(0);
  p_xs_WW_sys_E->SetBorderSize(0);
  p_xs_WW_sys_E->AddText("CMS (4.7 fb^{-1})");
  p_xs_WW_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_WW_sys_E->Draw("sames");
	
  l_xs_WW_sys_E->SetFillColor(0);
  l_xs_WW_sys_E->SetFillStyle(0);
//  l_xs_WW_sys_E->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_WW_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_WW_sys_E->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  l_xs_WW_sys_E->AddEntry(g_xs_HT_fat_WW_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_WW_sys_E->AddEntry(h_xs_HT_fat_WW_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_WW_sys_E->AddEntry(h_xs_HT_fat_WW_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_WW_sys_E->AddEntry(g_xs_HT_fat_WW_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_WW_sys_E->AddEntry(h_xs_HT_fat_WW_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_WW_sys_E->AddEntry(h_xs_HT_fat_WW_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_WW_sys_E->Draw("sames");
  l_xs_theory_WW->SetFillColor(0);
  l_xs_theory_WW->SetFillStyle(0);
  //l_xs_theory_WW->AddEntry(g_xs_string,"String Resonance","L");
  l_xs_theory_WW->AddEntry(g_xs_RSG_WW,"RS Graviton -> WW","L");
  l_xs_theory_WW->Draw("sames");

  c_xs_WW_sys_E->Update();

  c_xs_WW_sys_E->SaveAs("Fit_Results/Exclusions_WW_withExpected_2Vtag.png");
  c_xs_WW_sys_E->SaveAs("Fit_Results/Exclusions_WW_withExpected_2Vtag.pdf");



  // c_xs_ZZ_sys_E

  THStack *h_xs_ZZ_sys_E = new THStack("h_xs_ZZ_sys_E","h_xs_ZZ_sys_E");
  TH1F *htemp_xs_ZZ_sys_E_u2 = (TH1F*)h_xs_HT_fat_ZZ_1010_sys_E_u2->Clone("htemp_xs_ZZ_sys_E_u2");
  TH1F *htemp_xs_ZZ_sys_E_u1 = (TH1F*)h_xs_HT_fat_ZZ_1010_sys_E_u1->Clone("htemp_xs_ZZ_sys_E_u1");  
  TH1F *htemp_xs_ZZ_sys_E_d1 = (TH1F*)h_xs_HT_fat_ZZ_1010_sys_E_d1->Clone("htemp_xs_ZZ_sys_E_d1");   
  htemp_xs_ZZ_sys_E_u2->Add(h_xs_HT_fat_ZZ_1010_sys_E_u1,-1);
  htemp_xs_ZZ_sys_E_u1->Add(h_xs_HT_fat_ZZ_1010_sys_E_d1,-1);
  htemp_xs_ZZ_sys_E_d1->Add(h_xs_HT_fat_ZZ_1010_sys_E_d2,-1);

  h_xs_ZZ_sys_E->Add(h_xs_HT_fat_ZZ_1010_sys_E_d2);
  h_xs_ZZ_sys_E->Add(htemp_xs_ZZ_sys_E_d1);
  h_xs_ZZ_sys_E->Add(htemp_xs_ZZ_sys_E_u1);
  h_xs_ZZ_sys_E->Add(htemp_xs_ZZ_sys_E_u2);

  h_xs_ZZ_sys_E->SetMinimum(minXsec);
  h_xs_ZZ_sys_E->SetMaximum(maxXsec);

  c_xs_ZZ_sys_E->cd(0);

  h_xs_HT_fat_ZZ_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_ZZ_sys_E->SetTitle("");
  h_xs_HT_fat_ZZ_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_ZZ_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_ZZ_sys_E->Draw("C");
  
  h_xs_ZZ_sys_E->GetXaxis()->SetTitle("ZZ Resonance Mass (GeV)");
  h_xs_ZZ_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");
 
  g_xs_HT_fat_ZZ_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_ZZ_1010_sys->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_RSG_WW->Draw("sameL");
  g_xs_RSG_ZZ->Draw("sameL");
  g_xs_RSG_ZZ_ex->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_ZZ_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_ZZ_sys_E->Draw();
	
  p_xs_ZZ_sys_E->SetFillColor(0);
  p_xs_ZZ_sys_E->SetFillStyle(0);
  p_xs_ZZ_sys_E->SetBorderSize(0);
  p_xs_ZZ_sys_E->AddText("CMS (4.7 fb^{-1})");
  p_xs_ZZ_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_ZZ_sys_E->Draw("sames");

  l_xs_ZZ_sys_E->SetFillColor(0);
  l_xs_ZZ_sys_E->SetFillStyle(0);
  //  l_xs_ZZ_sys_E->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_ZZ_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_ZZ_sys_E->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  l_xs_ZZ_sys_E->AddEntry(g_xs_HT_fat_ZZ_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_ZZ_sys_E->AddEntry(h_xs_HT_fat_ZZ_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_ZZ_sys_E->AddEntry(h_xs_HT_fat_ZZ_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_ZZ_sys_E->AddEntry(g_xs_HT_fat_ZZ_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_ZZ_sys_E->AddEntry(h_xs_HT_fat_ZZ_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_ZZ_sys_E->AddEntry(h_xs_HT_fat_ZZ_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_ZZ_sys_E->Draw("sames");
  l_xs_theory_ZZ->SetFillColor(0);
  l_xs_theory_ZZ->SetFillStyle(0);
  l_xs_theory_ZZ->AddEntry(g_xs_RSG_ZZ,"RS Graviton -> ZZ","L");
  l_xs_theory_ZZ->AddEntry(g_xs_RSG_ZZ_ex,"RS Graviton -> ZZ (k/M_{PL}=2)","L");
  //l_xs_theory_ZZ->AddEntry(g_xs_RSG_ZZ,"(NLO) arXiv:1111.7261v2","");
  //l_xs_theory_ZZ->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  //l_xs_theory_ZZ->AddEntry(g_xs_wprime,"W'","L");
  //l_xs_theory_ZZ->AddEntry(g_xs_zprime,"Z'","L");
  //l_xs_theory_ZZ->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_ZZ->Draw("sames");
  
  c_xs_ZZ_sys_E->Update();

  c_xs_ZZ_sys_E->SaveAs("Fit_Results/Exclusions_ZZ_withExpected_2Vtag.png");
  c_xs_ZZ_sys_E->SaveAs("Fit_Results/Exclusions_ZZ_withExpected_2Vtag.pdf");



  // c_xs_WZ_sys_E

  THStack *h_xs_WZ_sys_E = new THStack("h_xs_WZ_sys_E","h_xs_WZ_sys_E");
  TH1F *htemp_xs_WZ_sys_E_u2 = (TH1F*)h_xs_HT_fat_WZ_1010_sys_E_u2->Clone("htemp_xs_WZ_sys_E_u2");
  TH1F *htemp_xs_WZ_sys_E_u1 = (TH1F*)h_xs_HT_fat_WZ_1010_sys_E_u1->Clone("htemp_xs_WZ_sys_E_u1");  
  TH1F *htemp_xs_WZ_sys_E_d1 = (TH1F*)h_xs_HT_fat_WZ_1010_sys_E_d1->Clone("htemp_xs_WZ_sys_E_d1");   
  htemp_xs_WZ_sys_E_u2->Add(h_xs_HT_fat_WZ_1010_sys_E_u1,-1);
  htemp_xs_WZ_sys_E_u1->Add(h_xs_HT_fat_WZ_1010_sys_E_d1,-1);
  htemp_xs_WZ_sys_E_d1->Add(h_xs_HT_fat_WZ_1010_sys_E_d2,-1);

  h_xs_WZ_sys_E->Add(h_xs_HT_fat_WZ_1010_sys_E_d2);
  h_xs_WZ_sys_E->Add(htemp_xs_WZ_sys_E_d1);
  h_xs_WZ_sys_E->Add(htemp_xs_WZ_sys_E_u1);
  h_xs_WZ_sys_E->Add(htemp_xs_WZ_sys_E_u2);

  h_xs_WZ_sys_E->SetMinimum(minXsec);
  h_xs_WZ_sys_E->SetMaximum(maxXsec);

  c_xs_WZ_sys_E->cd(0);

  h_xs_HT_fat_WZ_1010_sys_E_d2->GetXaxis()->SetRangeUser(minMass,maxMass);
  h_xs_WZ_sys_E->SetTitle("");
  h_xs_HT_fat_WZ_1010_sys_E_d2->SetTitleSize(0.2);
  h_xs_HT_fat_WZ_1010_sys_E_d2->GetXaxis()->SetTitleSize(0.06);


  h_xs_WZ_sys_E->Draw("C");
  
  h_xs_WZ_sys_E->GetXaxis()->SetTitle("WZ Resonance Mass (GeV)");
  h_xs_WZ_sys_E->GetYaxis()->SetTitle("Cross Section #times #it{B} #times #it{A} (pb)");
 
  g_xs_HT_fat_WZ_1010_sys_E_m->Draw("sameC");
  g_xs_HT_fat_WZ_1010_sys->Draw("samePL");

  //  g_xs_string->Draw("samePL");  
  //  g_xs_RSG_WW->Draw("sameL");
  g_xs_Wprime_WZ->Draw("sameL");
  //g_xs_diquark->Draw("sameL");
  //g_xs_wprime->Draw("sameL");  
  //g_xs_zprime->Draw("sameL");  
  //g_xs_rsgraviton->Draw("sameL");  


  //  TGaxis *TX_xs_WZ_sys_E = new TGaxis(500.0,300.0,550.0,300.0,1000.0,4000.0,510,"");
  //  TX_xs_WZ_sys_E->Draw();
	
  p_xs_WZ_sys_E->SetFillColor(0);
  p_xs_WZ_sys_E->SetFillStyle(0);
  p_xs_WZ_sys_E->SetBorderSize(0);
  p_xs_WZ_sys_E->AddText("CMS (4.7 fb^{-1})");
  p_xs_WZ_sys_E->AddText("#sqrt{s} = 7 TeV");
  p_xs_WZ_sys_E->Draw("sames");

  l_xs_WZ_sys_E->SetFillColor(0);
  l_xs_WZ_sys_E->SetFillStyle(0);
  //  l_xs_WZ_sys_E->SetHeader("CMS (4.7 fb^{-1})");
  //  l_xs_WZ_sys_E->SetHeader("CMS Preliminary");
  //  l_xs_WZ_sys_E->AddEntry((TObject*)0,"4.7 fb^{-1}","")
  l_xs_WZ_sys_E->AddEntry(g_xs_HT_fat_WZ_1010_sys,"Observed 95% CL Upper Limit","pL");
  //  l_xs_WZ_sys_E->AddEntry(h_xs_HT_fat_WZ_1010_sys_E_d2,"-2 #sigma","pL");
  //  l_xs_WZ_sys_E->AddEntry(h_xs_HT_fat_WZ_1010_sys_E_d1,"-1 #sigma","pL");
  l_xs_WZ_sys_E->AddEntry(g_xs_HT_fat_WZ_1010_sys_E_m,"Expected 95% CL Upper Limit","pL");
  l_xs_WZ_sys_E->AddEntry(h_xs_HT_fat_WZ_1010_sys_E_u1,"Expected Limit #pm 1#sigma","F");
  l_xs_WZ_sys_E->AddEntry(h_xs_HT_fat_WZ_1010_sys_E_u2,"Expected Limit #pm 2#sigma","F");
  l_xs_WZ_sys_E->Draw("sames");
  l_xs_theory_WZ->SetFillColor(0);
  l_xs_theory_WZ->SetFillStyle(0);
  l_xs_theory_WZ->AddEntry(g_xs_Wprime_WZ,"W' -> WZ","L");
  //l_xs_theory_WZ->AddEntry(g_xs_RSG_WZ,"(NLO) arXiv:1111.7261v2","");
  //l_xs_theory_WZ->AddEntry(g_xs_diquark,"E_{6} Diquark","L");
  //l_xs_theory_WZ->AddEntry(g_xs_wprime,"W'","L");
  //l_xs_theory_WZ->AddEntry(g_xs_zprime,"Z'","L");
  //l_xs_theory_WZ->AddEntry(g_xs_rsgraviton,"RS Graviton","L");

  l_xs_theory_WZ->Draw("sames");
  
  c_xs_WZ_sys_E->Update();

  c_xs_WZ_sys_E->SaveAs("Fit_Results/Exclusions_WZ_withExpected_2Vtag.png");
  c_xs_WZ_sys_E->SaveAs("Fit_Results/Exclusions_WZ_withExpected_2Vtag.pdf");


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
  g_xs_comparison_bw_stat_bg2_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg3_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg4_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg5_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg6_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg7_fat->Draw("samePL");;
  g_xs_comparison_bw_stat_bg8_fat->Draw("samePL");;
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
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg_fat,"bg #pm e1e2 / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg2_fat,"bg AB / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg3_fat,"bg +e1 / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg4_fat,"bg +e2 / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg6_fat,"bg -e1 / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_bg7_fat,"bg -e2 / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_lumi_fat,"Lumi / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JES_fat,"JES / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->AddEntry(g_xs_comparison_bw_stat_JER_fat,"JER / stat","pL");
  l_xs_comparison_bw_stat_sys_fat->Draw("sames");

  c_xs_comparison_bw_stat_sys_fat->Update();

  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results/Exclusions_WW_SysErrorsDependance_2Vtag.png");
  c_xs_comparison_bw_stat_sys_fat->SaveAs("Fit_Results/Exclusions_WW_SysErrorsDependance_2Vtag.pdf");

  // c_xs_WW_compare
  /*
  c_xs_WW_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_WW_compare->Draw("PL");

  l_xs_WW_compare->SetFillColor(0);
  l_xs_WW_compare->SetFillStyle(0);
  l_xs_WW_compare->SetHeader("WW-(Stat.Only)-4.7 fb^{-1}");
  l_xs_WW_compare->AddEntry(g_xs_HT_fat_WW_compare,"HT-fat","pL");

  l_xs_WW_compare->Draw("sames");

  c_xs_WW_compare->Update();
  */
  // c_xs_WZ_compare

  /*c_xs_WZ_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_WZ_compare->Draw("PL");

  l_xs_WZ_compare->SetFillColor(0);
  l_xs_WZ_compare->SetFillStyle(0);
  l_xs_WZ_compare->SetHeader("WZ-(Stat.Only)-4.7 fb^{-1}");
  l_xs_WZ_compare->AddEntry(g_xs_HT_fat_WZ_compare,"HT-fat","pL");

  l_xs_WZ_compare->Draw("sames");

  c_xs_WZ_compare->Update();
  */
  // c_xs_ZZ_compare
  /*
  c_xs_ZZ_compare->cd(0);
  TH1F *vFrame = gPad->DrawFrame(minMass,0.01,maxMass,maxXsec);
  vFrame->SetTitle("");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("Resonance mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.06);
  vFrame->SetYTitle("(each type) / (HT-calo)");
  // vFrame->SetLogy();

  g_xs_HT_fat_ZZ_compare->Draw("PL");

  l_xs_ZZ_compare->SetFillColor(0);
  l_xs_ZZ_compare->SetFillStyle(0);
  l_xs_ZZ_compare->SetHeader("ZZ-(Stat.Only)-4.7 fb^{-1}");
  l_xs_ZZ_compare->AddEntry(g_xs_HT_fat_ZZ_compare,"HT-fat","pL");

  l_xs_ZZ_compare->Draw("sames");

  c_xs_ZZ_compare->Update();
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
  //g_xs_HT_fat_WZ_1010_sys->Draw("samePL");
  g_xs_HT_fat_ZZ_1010_sys->Draw("samePL");
  //  g_xs_string->Draw("sameL");  
  //  g_xs_RSG->Draw("sameL");
  //  g_xs_RSG_ZZ->Draw("sameL");
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
  l_xs_rsg->AddEntry(g_xs_HT_fat_ZZ_1010_sys,"HT-fat ZZ","pL");
  //l_xs_rsg->AddEntry(g_xs_HT_fat_WZ_1010_sys,"HT-fat WZ","pL");
  l_xs_rsg->AddEntry(g_xs_RSG_for_limit_fat,"HT-fat w1*ZZ+w2*WZ","pL");
  l_xs_rsg->AddEntry(g_xs_rsgraviton,"cross section for RSG","pL");

  l_xs_rsg->Draw("sames");
  //  l_xs_theory->Draw("sames");

  c_xs_rsg->Update();
  c_xs_rsg->SaveAs("Fit_Results/Exclusions_WZZZ_Graviton_2Vtag.png");
  c_xs_rsg->SaveAs("Fit_Results/Exclusions_WZZZ_Graviton_2Vtag.pdf");
  */

}
