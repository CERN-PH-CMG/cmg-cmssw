{


 TCanvas* PT_HAT = new TCanvas();

 TFile *_file0 = TFile::Open("out/res/histograms_mc_120to170.root");
 TH1D* QCD_Z2_120to170 = (TH1D* ) _file0->Get("h_Pthat_data;1");

 TFile *_file1 = TFile::Open("out/res/histograms_mc_170to300.root");
 TH1D* QCD_Z2_170to300 = (TH1D* ) _file1->Get("h_Pthat_data;1");

 TFile *_file2 = TFile::Open("out/res/histograms_mc_300to470.root");
 TH1D* QCD_Z2_300to470 = (TH1D* ) _file2->Get("h_Pthat_data;1");


 TFile *_file3 = TFile::Open("out/res/histograms_mc_470to600.root");
 TH1D* QCD_Z2_470to600 = (TH1D* ) _file3->Get("h_Pthat_data;1");


 TFile *_file4 = TFile::Open("out/res/histograms_mc_600to800.root");
 TH1D* QCD_Z2_600to800 = (TH1D* ) _file4->Get("h_Pthat_data;1");


 TFile *_file5 = TFile::Open("out/res/histograms_mc_800to1000.root");
 TH1D* QCD_Z2_800to1000 = (TH1D* ) _file5->Get("h_Pthat_data;1");


 TFile *_file6 = TFile::Open("out/res/histograms_mc_1000to1400.root");
 TH1D* QCD_Z2_1000to1400 = (TH1D* ) _file6->Get("h_Pthat_data;1");


 TFile *_file7 = TFile::Open("out/res/histograms_mc_1400to1800.root");
 TH1D* QCD_Z2_1400to1800 = (TH1D* ) _file7->Get("h_Pthat_data;1");


 TFile *_file8 = TFile::Open("out/res/histograms_mc_1800toinf.root");
 TH1D* QCD_Z2_1800toinf = (TH1D* ) _file8->Get("h_Pthat_data;1");


 QCD_Z2_120to170->SetLineColor(1);
 QCD_Z2_170to300->SetLineColor(2);

 QCD_Z2_300to470->SetLineColor(3);
 QCD_Z2_470to600->SetLineColor(4);
 QCD_Z2_600to800->SetLineColor(5);
 QCD_Z2_800to1000->SetLineColor(6);
 QCD_Z2_1000to1400->SetLineColor(7);
 QCD_Z2_1400to1800->SetLineColor(8);
 QCD_Z2_1800toinf->SetLineColor(9);

//  float events[8] = {5684160.0, 6336960.0, 4034162.0, 3939510.0, 4161710.0, 2169144.0, 2043020.0, 1058720.0};
float events[8] = {5671529., 6308796., 3050637., 3939510., 4161710., 2169144., 2043020., 1056368.};
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

 PT_HAT->cd();
 stacks->Draw();

PT_HAT->SetLogy(1);
 PT_HAT->SaveAs("PT_HAT_WITHNEVENT_logS.png");


 TFile *_file0 = TFile::Open("out/res/histograms_mc_120to170.root");
 TH1D* QCD_Z2_120to170 = (TH1D* ) _file0->Get("h_Cuts_Fat;1");

 TFile *_file1 = TFile::Open("out/res/histograms_mc_170to300.root");
 TH1D* QCD_Z2_170to300 = (TH1D* ) _file1->Get("h_Cuts_Fat;1");

 TFile *_file2 = TFile::Open("out/res/histograms_mc_300to470.root");
 TH1D* QCD_Z2_300to470 = (TH1D* ) _file2->Get("h_Cuts_Fat;1");


 TFile *_file3 = TFile::Open("out/res/histograms_mc_470to600.root");
 TH1D* QCD_Z2_470to600 = (TH1D* ) _file3->Get("h_Cuts_Fat;1");


 TFile *_file4 = TFile::Open("out/res/histograms_mc_600to800.root");
 TH1D* QCD_Z2_600to800 = (TH1D* ) _file4->Get("h_Cuts_Fat;1");


 TFile *_file5 = TFile::Open("out/res/histograms_mc_800to1000.root");
 TH1D* QCD_Z2_800to1000 = (TH1D* ) _file5->Get("h_Cuts_Fat;1");


 TFile *_file6 = TFile::Open("out/res/histograms_mc_1000to1400.root");
 TH1D* QCD_Z2_1000to1400 = (TH1D* ) _file6->Get("h_Cuts_Fat;1");


 TFile *_file7 = TFile::Open("out/res/histograms_mc_1400to1800.root");
 TH1D* QCD_Z2_1400to1800 = (TH1D* ) _file7->Get("h_Cuts_Fat;1");


 TFile *_file8 = TFile::Open("out/res/histograms_mc_1800toinf.root");
 TH1D* QCD_Z2_1800toinf = (TH1D* ) _file8->Get("h_Cuts_Fat;1");

 QCD_Z2_170to300->Scale(xs[0]/events[0]);
 QCD_Z2_300to470->Scale(xs[1]/events[1]);
 QCD_Z2_470to600->Scale(xs[2]/events[2]);
 QCD_Z2_600to800->Scale(xs[3]/events[3]);
 QCD_Z2_800to1000->Scale(xs[4]/events[4]);
 QCD_Z2_1000to1400->Scale(xs[5]/events[5]);
 QCD_Z2_1400to1800->Scale(xs[6]/events[6]);
 QCD_Z2_1800toinf->Scale(xs[7]/events[7]);

 TH1D* hSelection = new TH1D();
 QCD_Z2_170to300->Copy(*hSelection);

 hSelection->Add(QCD_Z2_300to470);
 hSelection->Add(QCD_Z2_470to600);
 hSelection->Add(QCD_Z2_600to800);
 hSelection->Add(QCD_Z2_800to1000);
 hSelection->Add(QCD_Z2_1000to1400);
 hSelection->Add(QCD_Z2_1400to1800);
 hSelection->Add(QCD_Z2_1800toinf);


 std::cout << "# of events = " << hSelection->GetBinContent(1) << std::endl;
 std::cout << "# of events after vertex cut = " << hSelection->GetBinContent(2) << std::endl;
 std::cout << "# of events after mass cut  = " << hSelection->GetBinContent(3) << std::endl;
 std::cout << "# of events after eta cut  = " << hSelection->GetBinContent(4) << std::endl;
 std::cout << "# of events after all cuts = " << hSelection->GetBinContent(5) << std::endl;

 std::cout << "# of events (%) = " << hSelection->GetBinContent(1)/hSelection->GetBinContent(1)*100 << std::endl;
 std::cout << "# of events after vertex cut (%) = " << hSelection->GetBinContent(2)/hSelection->GetBinContent(1)*100 << std::endl;
 std::cout << "# of events after mass cut (%)  = " << hSelection->GetBinContent(3)/hSelection->GetBinContent(1)*100 << std::endl;
 std::cout << "# of events after eta cut (%)  = " << hSelection->GetBinContent(4)/hSelection->GetBinContent(1)*100 << std::endl;
 std::cout << "# of events after all cuts (%) = " << hSelection->GetBinContent(5)/hSelection->GetBinContent(1)*100 << std::endl;



}
