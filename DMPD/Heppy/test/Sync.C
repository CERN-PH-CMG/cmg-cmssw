{
  std::string dir="Sync/";
  std::string name[]={"SR", "WCR", "ZCR", "GCR"};
  for(int i=0; i<4; i++) {
    TH1F* h=(TH1F*)_file0->Get((dir+name[i]).c_str());
    std::cout << std::endl << " - " << h->GetTitle() << std::endl;
    for(int k=1; k<=h->GetNbinsX(); k++) {
      std::cout << h->GetXaxis()->GetBinLabel(k) << "\t:  " << h->GetBinContent(k) << std::endl;
    }
  }
  
}
