{
  
  gROOT->Reset();
  
  TDatime da(2011,01,01,12,00,00);
  gStyle->SetTimeOffset(da.Convert());
  
  ct = new TCanvas("ct","Time on axis");
  
  ht2 = new TH1F("ht2","ht2",365,0.,365.*86400);
  for (Int_t i=1;i<365;i++) {
    ht2->SetBinContent(i,i);
  }
  
  ct->cd(2);
  ht2->GetXaxis()->SetLabelSize(0.04);
  ht2->GetXaxis()->SetTimeDisplay(1);
  ht2->GetXaxis()->SetNdivisions(12);
  ht2->GetXaxis()->SetTimeFormat("%d\/%m\/%y");
  ht2->Draw();

}
