{

  // 1 = qq, 2 = qg, 3 = gg
  double kind = 3;

  TCanvas* c = new TCanvas();

  c->Divide(1,2,0,0,0);

  p1_1 = (TPad*)c->GetPad(1);
  p1_1->SetPad(0.01,0.22,0.99,0.98);
  p1_1->SetLogy();
  p1_1->SetRightMargin(0.05);
  p1_1->SetTopMargin(0.05);

  c->cd(2);
  p1_2 = (TPad*)c->GetPad(2);
  p1_2->SetPad(0.01,0.02,0.99,0.24);
  p1_2->SetBottomMargin(0.35);
  p1_2->SetRightMargin(0.05);
  p1_2->SetGridx();
  p1_2->SetTickx(10);
  p1_2->SetGridy();
  p1_2->SetTicky(2);

  if (kind == 2){
    TFile *_file0 = TFile::Open("plots_Qstar_ChiyoungWAccCorr/limit_limit_0.root");
    TFile *_file1 = TFile::Open("plots_Qstar_ChiyoungWAccCorr/limit_limit_6.root");
    TFile *_file2 = TFile::Open("plots_Qstar_4677pbm1/limit_limit_0.root");
    TFile *_file3 = TFile::Open("plots_Qstar_4677pbm1/limit_limit_6.root");
    TFile *_file4 = TFile::Open("plots_Qstar_4677pbm1/limit_expectedlimit_6.root");
    TFile *_eff = TFile::Open("plots_Qstar_4677pbm1/Resonance_Shapes_Qstar_ak5_fat30.root");
  }
  if (kind == 1){

    TFile *_file0 = TFile::Open("plots_QQ_ChiyoungWAccCorr/limit_limit_0.root");
    TFile *_file1 = TFile::Open("plots_QQ_ChiyoungWAccCorr/limit_limit_6.root"); 
    TFile *_file2 = TFile::Open("plots_QQ_4677pbm1/limit_limit_0.root");
    TFile *_file3 = TFile::Open("plots_QQ_4677pbm1/limit_limit_6.root"); 
    TFile *_file4 = TFile::Open("plots_QQ_4677pbm1/limit_expectedlimit_6.root");
    TFile *_eff = TFile::Open("plots_QQ_4677pbm1/Resonance_Shapes_RSGraviton_ak5_QQtoQQ_fat30.root");

  }
  if (kind == 3){
    TFile *_file0 = TFile::Open("plots_GG_ChiyoungWAccCorr/limit_limit_0.root");
    TFile *_file1 = TFile::Open("plots_GG_ChiyoungWAccCorr/limit_limit_6.root"); 
    TFile *_file2 = TFile::Open("plots_GG_4677pbm1/limit_limit_0.root");
    TFile *_file3 = TFile::Open("plots_GG_4677pbm1/limit_limit_6.root"); 
    TFile *_file4 = TFile::Open("plots_GG_4677pbm1/limit_expectedlimit_6.root");
    TFile *_eff = TFile::Open("plots_GG_4677pbm1/Resonance_Shapes_RSGraviton_ak5_GGtoGG_fat30.root");
  }

  TH1F* limit1fb_stat = (TH1F*) _file0->Get("limit;1");
  TH1F* limit1fb_sys = (TH1F*) _file1->Get("limit;1");
  TH1F* limit4fb_stat = (TH1F*) _file2->Get("limit;1");
  TH1F* limit4fb_sys = (TH1F*) _file3->Get("limit;1");
 
  TH1F* htwosigma_lower = (TH1F*) _file4->Get("htwosigma_lowers;1");
  TH1F* honesigma_lower = (TH1F*) _file4->Get("honesigma_lowers;1");
  TH1F* honesigma_upper = (TH1F*) _file4->Get("honesigma_uppers;1");
  TH1F* htwosigma_upper = (TH1F*) _file4->Get("htwosigma_uppers;1");

  TH1F* eff = (TH1F*) _eff->Get("Efficiency;1");
  limit4fb_stat->Divide(eff);
  limit4fb_sys->Divide(eff);

  htwosigma_lower->Divide(eff);
  honesigma_lower->Divide(eff);
  honesigma_upper->Divide(eff);
  htwosigma_upper->Divide(eff);


  if (kind == 3) {
    TH1F* rsgXsec = (TH1F*) _file1->Get("rsgXsec;1");
    rsgXsec->SetLineWidth(2);
  }
  if (kind == 2) {
    TH1F* qstarXsec = (TH1F*) _file1->Get("qstarXsec;1");
    qstarXsec->SetLineWidth(2);
    qstarXsec->SetLineColor(kBlue);
  }
  if (kind == 1) {
    TH1F* wprime = (TH1F*) _file2->Get("wprime;1");
    TH1F* zprime = (TH1F*) _file2->Get("zprime;1");
    TH1F* axigluon = (TH1F*) _file2->Get("axigluon;1");
    wprime->SetLineWidth(2);
    wprime->SetLineColor(30);
    zprime->SetLineWidth(2);
    axigluon->SetLineWidth(2);
    axigluon->SetLineColor(kBlue);
  }

  limit1fb_stat->SetLineColor(kRed);
  limit1fb_sys->SetLineColor(kMagenta);
  limit1fb_sys->SetLineStyle(3);


  limit4fb_stat->SetLineColor(kBlack);
  limit4fb_sys->SetLineColor(kBlue);
  limit4fb_sys->SetLineStyle(3);

  p1_1->cd();

  TH1F *vFrame = p1_1->DrawFrame(943.0,0.001,4000.0,3.0);
  vFrame->SetTitle("");
  vFrame->SetXTitle("Dijet Mass (GeV)");
  vFrame->GetXaxis()->SetTitleSize(0.00);
  vFrame->GetXaxis()->SetLabelSize(0.00);
  vFrame->SetYTitle("d#sigma/dm (pb/GeV)");
  vFrame->GetYaxis()->SetTitleSize(0.05);
  

  htwosigma_upper->Draw("SAME");
  honesigma_upper->Draw("SAME");
  honesigma_lower->Draw("SAME");
  htwosigma_lower->Draw("SAME");



  limit1fb_stat->DrawClone("SAME");
  limit1fb_sys->DrawClone("SAME");
  limit4fb_stat->DrawClone("SAME");
  limit4fb_sys->DrawClone("SAME");

  legend= new TLegend(0.55,0.55,0.94,0.95,"");
  legend->SetHeader("Statistical only limits");

  if (kind == 1) {
    legend->SetHeader("CMS Data QQ");
    wprime->DrawClone("SAMEL");
    zprime->DrawClone("SAMEL"); 
    axigluon->DrawClone("SAMEL"); 
  }
  if (kind == 2) {
    legend->SetHeader("CMS Data QG");
    qstarXsec->DrawClone("SAMEL");
  }
  if (kind == 3) {
    legend->SetHeader("CMS Data GG");   
    rsgXsec->DrawClone("SAMEL");
  }

  legend->AddEntry(limit1fb_stat,"Fat AK5 1.010 fb^{-1} pt>10 stat","l");
  legend->AddEntry(limit1fb_sys,"Fat AK5 1.010 fb^{-1} pt>10 sys","l");
  legend->AddEntry(limit4fb_stat,"Fat AK5 4.677 fb^{-1} pt>30 stat","l");
  legend->AddEntry(limit4fb_sys,"Fat AK5 4.677 fb^{-1} pt>30 sys","l");

  if (kind == 1) {
    legend->AddEntry(axigluon,"A -> qq","l");
    legend->AddEntry(wprime,"W' -> qq","l");
    legend->AddEntry(zprime,"Z' -> qq","l");
  }
  if (kind == 2) {
    legend->AddEntry(qstarXsec,"Q* -> qg","l");
  }
  if (kind == 3) {
    legend->AddEntry(rsgXsec,"RSG -> gg","l");
  }    



  legend->Draw();

  gPad->SetLogy(1);

  p1_2->cd();



  TH1F *vFrame2 = p1_2->DrawFrame(943.0, 0.1, 4000.0, 6);
  vFrame2->SetTitle("");
  vFrame2->SetXTitle("Resonance Mass (GeV)");
  vFrame2->GetXaxis()->SetTitleSize(0.06);
  vFrame2->SetYTitle("Limit/4fb_sys");
  vFrame2->GetYaxis()->SetTitleSize(0.10);
  vFrame2->GetYaxis()->SetTitleOffset(0.20);
  vFrame2->GetYaxis()->SetLabelSize(0.10);
  vFrame2->GetXaxis()->SetTitleOffset(0.90);
  vFrame2->GetXaxis()->SetTitleSize(0.18);
  vFrame2->GetXaxis()->SetLabelSize(0.18);
  vFrame2->GetYaxis()->SetNdivisions(010);
  
  limit1fb_stat->Divide(limit4fb_sys);
  limit1fb_sys->Divide(limit4fb_sys);
  limit4fb_stat->Divide(limit4fb_sys);
 
  if (kind == 1){
    axigluon->Divide(limit4fb_sys);
    wprime->Divide(limit4fb_sys);
    zprime->Divide(limit4fb_sys);
 
    axigluon->Draw("SAMEL");
    wprime->Draw("SAMEL");
    zprime->Draw("SAMEL");
  }
  if (kind == 2){
    qstarXsec->Divide(limit4fb_sys);
    qstarXsec->Draw("SAMEL");
  }
  if (kind == 3){
    rsgXsec->Divide(limit4fb_sys);
    rsgXsec->Draw("SAMEL");
  }

  limit4fb_sys->Divide(limit4fb_sys);

  limit1fb_stat->Draw("SAMEL");
  limit1fb_sys->Draw("SAMEL");
  limit4fb_stat->Draw("SAMEL");

  limit4fb_sys->Draw("SAMEL");


  if (kind ==3){

    c->SaveAs("plots_compar/FatAk54fbFat30_gg_FullStats.png");
    c->SaveAs("plots_compar/FatAk54fbFat30_gg_FullStats.eps");

  }
  if (kind ==2){
    c->SaveAs("plots_compar/FatAk54fbFat30_qg_FullStats.png");
    c->SaveAs("plots_compar/FatAk54fbFat30_qg_FullStats.eps");
  }
  if (kind ==1){
    c->SaveAs("plots_compar/FatAk54fbFat30_qq_FullStats.png");
    c->SaveAs("plots_compar/FatAk54fbFat30_qq_FullStats.eps");

  }

}
  
