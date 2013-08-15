{


  double xs_HT_fat_qg_1010_stat_E_m[32] = {0.88007299999999999, 0.72074400000000005, 0.58098099999999997, 0.46875, 0.38393500000000003, 0.32017800000000002, 0.25702599999999998, 0.21265600000000001, 0.17543, 0.14471999999999999, 0.12062100000000001, 0.102604, 0.087550199999999995, 0.076736299999999993, 0.065682900000000002, 0.057352300000000002, 0.049532100000000003, 0.042091000000000003, 0.037045500000000002, 0.032063500000000002, 0.028200099999999999, 0.024153600000000001, 0.021962300000000001, 0.018797999999999999, 0.016769599999999999, 0.0148472, 0.0131711, 0.0114614, 0.0102585, 0.0094824100000000001, 0.0085951199999999995, 0.0078771099999999997};
  double bin[32];

 double eff_qg[32] = {0.752614, 0.829898, 0.87992, 0.91429, 0.936309, 0.951325, 0.962299, 0.971395, 0.975795, 0.9811, 0.985793, 0.988608, 0.988827, 0.992177, 0.996037, 0.99614, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

 for (int i = 0; i < 32; i++) {
   bin[i] = 1.0 + i*0.1;
   xs_HT_fat_qg_1010_stat_E_m[i]/=eff_qg[i];
   //xs_HT_fat_qg_1010_stat_E_m[i] *= 1.2;
 }

 double scale_fact[4]={109.4/70.05, 8.924/5.645, 1.034/0.6531, 0.01954/0.01234};

 double bin_4[4]={1, 1.5, 2.0, 3.0};
 double xs_CMS[4]={109.4, 8.92, 1.0, 0.020};
 double xs_CMS_Corr[4]={131.8, 10.7, 1.2, 0.024};
 double xs_Atlas_MRST[4]={191.6, 14.2, 1.6, 0.028};
 double xs_Atlas_CTEQ[4]={153.2, 11.6, 1.3, 0.025};

 for (int i = 0; i < 4; i++) {

   xs_CMS[i] /= scale_fact[i];
   xs_CMS_Corr[i] /= scale_fact[i];
   xs_Atlas_MRST[i] /= scale_fact[i];
   xs_Atlas_CTEQ[i] /= scale_fact[i];

 }



  TGraph* G = new TGraph(32, bin, xs_HT_fat_qg_1010_stat_E_m);
  TGraph* gxs_CMS = new TGraph(4, bin_4, xs_CMS);
  TGraph* gxs_CMS_Corr = new TGraph(4, bin_4, xs_CMS_Corr);
  TGraph* gxs_Atlas_MRST = new TGraph(4, bin_4, xs_Atlas_MRST);
  TGraph* gxs_Atlas_CTEQ = new TGraph(4, bin_4, xs_Atlas_CTEQ);

  gxs_CMS->SetMarkerStyle(23);
  gxs_CMS->SetMarkerSize(1);
  gxs_CMS->SetMarkerColor(kRed);
  gxs_CMS->SetLineColor(kRed);

  gxs_CMS_Corr->SetMarkerStyle(20);
  gxs_CMS_Corr->SetMarkerSize(1);
  gxs_CMS_Corr->SetMarkerColor(kMagenta);
  gxs_CMS_Corr->SetLineColor(kMagenta);

  gxs_Atlas_MRST->SetMarkerStyle(21);
  gxs_Atlas_MRST->SetMarkerSize(1);
  gxs_Atlas_MRST->SetMarkerColor(kBlue);
  gxs_Atlas_MRST->SetLineColor(kBlue);

  gxs_Atlas_CTEQ->SetMarkerStyle(22);
  gxs_Atlas_CTEQ->SetMarkerSize(1);
  gxs_Atlas_CTEQ->SetMarkerColor(kGreen);
  gxs_Atlas_CTEQ->SetLineColor(kGreen);

  gxs_Atlas_MRST->SetTitle("Dijet Mass Shape analysis; M(Resoncance) / GeV; \sigma * Acc *  Br");

  gxs_Atlas_MRST->Draw("APL");
  gxs_CMS->Draw("SAMEPL");
  gxs_CMS_Corr->Draw("SAMEPL");
  gxs_Atlas_CTEQ->Draw("SAMEPL");
  G->Draw("SAMEL");

  gPad->SetLogy(1);



  TLine* LC = new TLine(2.68, 0.02, 2.68, 1);
  LC->SetLineColor(kBlue);
  LC->SetLineStyle(2);

  LC->Draw();

  TLine* LA = new TLine(2.81, 0.02, 2.81, 1);
  LA->SetLineColor(kRed);
  LA->SetLineStyle(3);

  LA->Draw();

  TLegend* T = new TLegend(0.5, 0.6, 0.85, 0.85);
  T->AddEntry(G, "CMS exp. limit", "l");
  T->AddEntry(gxs_CMS, "CMS Q*->qg CTEQ6", "p");
  T->AddEntry(gxs_CMS_Corr, "CMS Q*->qX (BF rew.) CTEQ6", "p");
  T->AddEntry(gxs_Atlas_CTEQ, "ATLAS Q*->qX CTEQ6", "p");
  T->AddEntry(gxs_Atlas_MRST, "ATLAS Q*->qX MRST07", "p");
  T->AddEntry(LC, "CMS expected", "l");
  T->AddEntry(LA, "ATLAS expected", "l");



  T->Draw();
}
