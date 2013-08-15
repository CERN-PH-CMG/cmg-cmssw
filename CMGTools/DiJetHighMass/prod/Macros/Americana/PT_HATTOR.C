{


 TCanvas* PT_HAT = new TCanvas();

 TFile *_file1 = TFile::Open("out/res/histograms_Fat30_summer11_mc_170to300_ak5.root");
 TH1D* QCD_Z2_170to300 = (TH1D* ) _file1->Get("h_Pthat_data;1");

 TFile *_file2 = TFile::Open("out/res/histograms_Fat30_summer11_mc_300to470_ak5.root");
 TH1D* QCD_Z2_300to470 = (TH1D* ) _file2->Get("h_Pthat_data;1");


 TFile *_file3 = TFile::Open("out/res/histograms_Fat30_summer11_mc_470to600_ak5.root");
 TH1D* QCD_Z2_470to600 = (TH1D* ) _file3->Get("h_Pthat_data;1");


 TFile *_file4 = TFile::Open("out/res/histograms_Fat30_summer11_mc_600to800_ak5.root");
 TH1D* QCD_Z2_600to800 = (TH1D* ) _file4->Get("h_Pthat_data;1");


 TFile *_file5 = TFile::Open("out/res/histograms_Fat30_summer11_mc_800to1000_ak5.root");
 TH1D* QCD_Z2_800to1000 = (TH1D* ) _file5->Get("h_Pthat_data;1");


 TFile *_file6 = TFile::Open("out/res/histograms_Fat30_summer11_mc_1000to1400_ak5.root");
 TH1D* QCD_Z2_1000to1400 = (TH1D* ) _file6->Get("h_Pthat_data;1");


 TFile *_file7 = TFile::Open("out/res/histograms_Fat30_summer11_mc_1400to1800_ak5.root");
 TH1D* QCD_Z2_1400to1800 = (TH1D* ) _file7->Get("h_Pthat_data;1");


 TFile *_file8 = TFile::Open("out/res/histograms_Fat30_summer11_mc_1800toinf_ak5.root");
 TH1D* QCD_Z2_1800toinf = (TH1D* ) _file8->Get("h_Pthat_data;1");


 // QCD_Z2_120to170->SetLineColor(1);
 QCD_Z2_170to300->SetLineColor(2);

 QCD_Z2_300to470->SetLineColor(3);
 QCD_Z2_470to600->SetLineColor(4);
 QCD_Z2_600to800->SetLineColor(5);
 QCD_Z2_800to1000->SetLineColor(6);
 QCD_Z2_1000to1400->SetLineColor(7);
 QCD_Z2_1400to1800->SetLineColor(8);
 QCD_Z2_1800toinf->SetLineColor(9);

//  float events[8] = {5684160.0, 6336960.0, 4034162.0, 3939510.0, 4161710.0, 2169144.0, 2043020.0, 1058720.0};
//float events[8] = { 2.63377e+07, 3.71793e+07, 2.36974e+07, 2.51463e+07, 2.41864e+07, 4.71894e+06, 1.23429e+07, 1.63987e+06};
 float events[8] = { 5.972300e+06, 6.30528e+06, 3.99298e+06 , 4.2242e+06, 4.36015e+06   , 1.21863e+06 , 2.05816e+06, 278220};
                    

//float events[8] = {5671529., 6308796., 3050637., 3939510., 4161710., 2169144., 2043020., 1056368.};
float xs[8] = {2.426e+04, 1.168e+03, 7.022e+01, 1.555e+01, 1.844e+00, 3.321e-01, 1.087e-02, 3.575e-04};

// QCD_Z2_120to170->Scale(xs[0]/events[0]);
 QCD_Z2_170to300->Scale(xs[0]/events[0]);
 QCD_Z2_300to470->Scale(xs[1]/events[1]);
 QCD_Z2_470to600->Scale(xs[2]/events[2]);
 QCD_Z2_600to800->Scale(xs[3]/events[3]);
 QCD_Z2_800to1000->Scale(xs[4]/events[4]);
 QCD_Z2_1000to1400->Scale(xs[5]/events[5]);
 QCD_Z2_1400to1800->Scale(xs[6]/events[6]);
 QCD_Z2_1800toinf->Scale(xs[7]/events[7]);


 THStack* stacks = new THStack("pthat", "pthat");
// stacks->Add( QCD_Z2_120to170);
 stacks->Add( QCD_Z2_170to300);
 stacks->Add( QCD_Z2_300to470);
 stacks->Add( QCD_Z2_470to600);
 stacks->Add( QCD_Z2_600to800);
 stacks->Add( QCD_Z2_800to1000);
 stacks->Add( QCD_Z2_1000to1400);
 stacks->Add( QCD_Z2_1400to1800);
 stacks->Add( QCD_Z2_1800toinf);

 stacks->SetMinimum(0.00001);
 stacks->SetMaximum(5*1e3);

 PT_HAT->cd();
 stacks->Draw();

 PT_HAT->SetLogy(1);
 PT_HAT->SaveAs("PT_HAT_WITHNEVENT_logS.png");

 PT_HAT->SetLogy(0);
 PT_HAT->SaveAs("PT_HAT_WITHNEVENT.png");


}
//                             170to300     300to470      470to600     600to800    800to1000      1000to1400     1400to1800   1800toinf

//cout << "float events[8] = { 5.57024e+06, 6.30528e+06, 3.99298e+06 , 4.2242e+06, 4.36015e+06   , 1.21863e+06 , 2.05816e+06, 278220)
