{

  // 1 = qq, 2 = qg, 3 = gg
  double kind = 3;

  TCanvas* c = new TCanvas();

  if (kind == 1){
    TFile *_file0 = TFile::Open("plots_QQ/limit_limit_0.root");
    TFile *_file1 = TFile::Open("plots_QQ/limit_limit_1.root");
    TFile *_file2 = TFile::Open("plots_QQ/limit_limit_2.root");
    TFile *_file3 = TFile::Open("plots_QQ/limit_limit_3.root");
    TFile *_file4 = TFile::Open("plots_QQ/limit_limit_4.root");
    TFile *_file6 = TFile::Open("plots_QQ/limit_limit_6.root");
  }
  if (kind == 2){
    TFile *_file0 = TFile::Open("plots_Qstar/limit_limit_0.root");
    TFile *_file1 = TFile::Open("plots_Qstar/limit_limit_1.root");
    TFile *_file2 = TFile::Open("plots_Qstar/limit_limit_2.root");
    TFile *_file3 = TFile::Open("plots_Qstar/limit_limit_3.root");
    TFile *_file4 = TFile::Open("plots_Qstar/limit_limit_4.root");
    TFile *_file6 = TFile::Open("plots_Qstar/limit_limit_6.root");
  }
  if (kind == 3){
    TFile *_file0 = TFile::Open("plots_GG/limit_limit_0.root");
    TFile *_file1 = TFile::Open("plots_GG/limit_limit_1.root");
    TFile *_file2 = TFile::Open("plots_GG/limit_limit_2.root");
    TFile *_file3 = TFile::Open("plots_GG/limit_limit_3.root");
    TFile *_file4 = TFile::Open("plots_GG/limit_limit_4.root");
    TFile *_file6 = TFile::Open("plots_GG/limit_limit_6.root");
  }


  TH1F* limitStat = (TH1F*) _file0->Get("limit;1");
  TH1F* limitModel = (TH1F*) _file1->Get("limit;1");
  TH1F* limitLumi = (TH1F*) _file2->Get("limit;1");
  TH1F* limitJES = (TH1F*) _file3->Get("limit;1");
  TH1F* limitJER = (TH1F*) _file4->Get("limit;1");
  TH1F* limitTotal = (TH1F*) _file6->Get("limit;1");

  if (kind == 1) TH1F* modelXsec = (TH1F*) _file0->Get("axigluon;1");
  if (kind == 2) TH1F* modelXsec = (TH1F*) _file0->Get("qstarXsec;1");
  if (kind == 3) TH1F* modelXsec = (TH1F*) _file0->Get("rsgXsec;1");


  limitModel->SetLineColor(kRed);
  limitLumi->SetLineColor(kGreen);
  limitJES->SetLineColor(kBlue);
  limitJER->SetLineColor(kMagenta);
  limitTotal->SetLineColor(kBlack);
  limitTotal->SetLineWidth(2);

  modelXsec->SetLineColor(kBlack);
  modelXsec->SetLineStyle(3);
  modelXsec->SetLineWidth(2);


  TLegend* legend= new TLegend(0.13,0.60,0.40,0.89,"");

  if (kind == 1) legend->SetHeader("CMS Data QQ (4.677 fb^{-1})");
  if (kind == 2) legend->SetHeader("CMS Data QG (4.477 fb^{-1})");
  if (kind == 3) legend->SetHeader("CMS Data GG (4.477 fb^{-1})");

  legend->AddEntry(limitModel,"Background / stat","l");
  legend->AddEntry(limitLumi,"Lumi / stat","l");
  legend->AddEntry(limitJES,"JES / stat","l");
  legend->AddEntry(limitJER,"JER / stat","l");
  legend->AddEntry(limitTotal,"Total / stat","l");
    
  if (kind == 1) legend->AddEntry(modelXsec,"A->qq","l");
  if (kind == 2) legend->AddEntry(modelXsec,"Q*->qg","l");
  if (kind == 3) legend->AddEntry(modelXsec,"RSG->gg","l");


  TH1F *vFrame2 = gPad->DrawFrame(950.0, 0.9, 4000.0, 1.3);
  vFrame2->SetTitle("");
  vFrame2->SetXTitle("Resonance Mass (GeV)");
  vFrame2->GetXaxis()->SetTitleSize(0.06);
  vFrame2->SetYTitle("(Sys. incl. 95% CL #sigma)/(Stat. only 95% CL #sigma)");
  //  vFrame2->GetYaxis()->SetTitleSize(0.16);
  //  vFrame2->GetYaxis()->SetLabelSize(0.10);
  vFrame2->GetXaxis()->SetTitleOffset(0.9);
  vFrame2->GetXaxis()->SetTitleSize(0.05);
  vFrame2->GetXaxis()->SetLabelSize(0.05);

  TLine* L = new TLine(950.0, 1.0, 4000.0, 1.0);
  L->SetLineWidth(2);
  L->SetLineStyle(2);

  limitModel->Divide(limitStat);
  limitLumi->Divide(limitStat);
  limitJES->Divide(limitStat);
  limitJER->Divide(limitStat);
  limitTotal->Divide(limitStat);
  modelXsec->Divide(limitStat);


  L->Draw();
  //    limitStat->Draw("SAME");
  
  limitModel->Draw("SAMEL");
  limitLumi->Draw("SAMEL");
  limitJES->Draw("SAMEL");
  limitJER->Draw("SAMEL");    
  limitTotal->Draw("SAMEL");

  modelXsec->Draw("SAMEC");
  
  legend->Draw();
 
  if (kind == 1){
    c->SaveAs("plots_compar/FatAk54fbFat30_qq_SysVsNonSys.png");
    c->SaveAs("plots_compar/FatAk54fbFat30_qq_SysVsNonSys.eps");
  }
  if (kind == 2){
    c->SaveAs("plots_compar/FatAk54fbFat30_qg_SysVsNonSys.png");
    c->SaveAs("plots_compar/FatAk54fbFat30_qg_SysVsNonSys.eps");
  }
  if (kind == 3){
    c->SaveAs("plots_compar/FatAk54fbFat30_gg_SysVsNonSys.png");
    c->SaveAs("plots_compar/FatAk54fbFat30_gg_SysVsNonSys.eps");
  }

}
  
