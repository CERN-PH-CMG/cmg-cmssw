{

TFile *_file0 = TFile::Open("out/res/FullSample/histograms_data_HT_All.root");
TH1F* hSelection = (TH1F* ) _file0->Get("h_Cuts_Fat;1");

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
