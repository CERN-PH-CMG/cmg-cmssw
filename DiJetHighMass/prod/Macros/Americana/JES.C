{
  TCanvas* C = new TCanvas();
  
 
  //  TFile *_file0 = TFile::Open("out/res/4fb/Fat30/WithFatJetsMass/histograms_Fat30_summer11_mc_ak5.root");
  TFile *_file0 = TFile::Open("/afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/CMGTools/DiJetHighMass/data/histograms_mc_weightpileup_pt_4June_pfweight_ak5.root");

  TProfile* p_dMass_vs_DijetMass_data_fat_up = (TProfile*) _file0->Get("p_dMass_vs_DijetMass_data_fat_up;1");
  TProfile* p_dMass_vs_DijetMass_data_fat_do = (TProfile*) _file0->Get("p_dMass_vs_DijetMass_data_fat_do;1");

  p_dMass_vs_DijetMass_data_fat_up->ProjectionX("h_dMass_vs_DijetMass_data_fat_up");
  p_dMass_vs_DijetMass_data_fat_do->ProjectionX("h_dMass_vs_DijetMass_data_fat_do");

  
  h_dMass_vs_DijetMass_data_fat_do->Scale(-100.);
  h_dMass_vs_DijetMass_data_fat_up->Scale(100.);

  h_dMass_vs_DijetMass_data_fat_up->SetMaximum(3.0);
  h_dMass_vs_DijetMass_data_fat_up->SetMinimum(-3.0);

  h_dMass_vs_DijetMass_data_fat_up->SetStats(0);
  
  h_dMass_vs_DijetMass_data_fat_up->SetTitle("JES Uncertainty at 8 TeV; Dijet Mass (GeV); JES Uncertainty (%)");
  h_dMass_vs_DijetMass_data_fat_up->GetXaxis()->SetTitleFont(42);
  h_dMass_vs_DijetMass_data_fat_up->GetXaxis()->SetTitleSize(0.05);
  h_dMass_vs_DijetMass_data_fat_up->GetXaxis()->SetTitleOffset(0.8);
  h_dMass_vs_DijetMass_data_fat_up->GetYaxis()->SetTitleFont(42);
  h_dMass_vs_DijetMass_data_fat_up->GetYaxis()->SetTitleSize(0.05);
  h_dMass_vs_DijetMass_data_fat_up->GetYaxis()->SetTitleOffset(0.7);

  h_dMass_vs_DijetMass_data_fat_up->SetFillColor(kYellow);
  h_dMass_vs_DijetMass_data_fat_do->SetFillColor(kYellow);
  h_dMass_vs_DijetMass_data_fat_up->Draw("H");
  h_dMass_vs_DijetMass_data_fat_do->Draw("SAMEH");



  TLine* L = new TLine(0., 0., 7000., 0.);
  L->SetLineStyle(2);
  L->SetLineWidth(2);
  L->Draw();

  TLine* LUp = new TLine(0., 1.25, 7000., 1.25);
  LUp->SetLineStyle(3);
  LUp->SetLineWidth(2);
  LUp->Draw();

  TLine* LDo = new TLine(0., -1.25, 7000., -1.25);
  LDo->SetLineStyle(3);
  LDo->SetLineWidth(2);
  LDo->Draw();

  C->SaveAs("JESUncertainty_2012.png");

}
