{
  TCanvas* C = new TCanvas();
  
  //  TFile *_file0 = TFile::Open("out/res/histograms_summer11_mc_ak5.root");
  //  TFile *_file0 = TFile::Open("out/res/old/histograms_data_HT_928fb_Fat_ak5.root");
  //  TFile *_file0 = TFile::Open("out/res/histograms_data_HT_35pb_2010_Fat_ak5.root");
  // TFile *_file0 = TFile::Open("out/res/histograms_data_HT_101fb_Fat_ak5.root");
  //  TFile *_file0 = TFile::Open("out/res/histograms_data_HT_068fb_Fat_ak5.root");
  //  TFile *_file0 = TFile::Open("out/res/histograms_data_Jet_Run2011A_May10_ak5.root");
  //  TFile *_file0 = TFile::Open("out/res/4fb/histograms_data_HT_Run2011B_178098_180252_ak5.root");
  //TFile *_file0 = TFile::Open("out/res/histograms_data_HT_101fb_PileUpStudies_ak5.root");
  // TFile *_file0 = TFile::Open("histograms_data_HT_Run2011B_178098_180252_WithMoreTriggers_short_PileUpStudies_ak5.root");
  //TFile *_file0 = TFile::Open("histograms_136033_167784_101fb_JetMassCuts_SingleJet_ak5.root");

  //  TFile *_file0 = TFile::Open("histograms_Run2011B_PileUpStudies_SingleJet_ak5.root");
  //  TFile *_file0 = TFile::Open("histograms_data_SingleJet_2011B_PileUpStudies_ak5.root");

  //  TFile *_file0 = TFile::Open("out/res/4fb/Fat30/histograms_data_HT_Run2011B_178098_180252_Fat30_ak5.root");
  // TFile *_file0 = TFile::Open("out/res/4fb/Fat30/histograms_data_HT_Run2011A_160404_167784_Fat30_ak5.root");
  //TFile *_file0 = TFile::Open("out/res/4fb/Fat30/histograms_data_HT_Run2011AB_160404_180252_Fat30_4444pbm1_ak5.root");
  TFile *_file0 = TFile::Open("histograms_4june_ak5.root");


  TH1D* h_DijetMass_MI_nPVg10_data_fat = (TH1D*) _file0->Get("h_DijetMass_MI_nPVg24_data_fat;1");
  TH1D* h_DijetMass_MI_nPVl3_data_fat = (TH1D*) _file0->Get("h_DijetMass_MI_nPVl1_9_data_fat;1");
  
  h_DijetMass_MI_nPVl3_data_fat->Scale(1000000.);
  h_DijetMass_MI_nPVg10_data_fat->Scale(1000000.);

    h_DijetMass_MI_nPVl3_data_fat->Rebin();
  //  h_DijetMass_MI_nPVl3_data_fat->Rebin();

    h_DijetMass_MI_nPVg10_data_fat->Rebin();
  //  h_DijetMass_MI_nPVg10_data_fat->Rebin();

      


  TH1D* h_DijetMass_MI_nPVg10_data_pf = (TH1D*) _file0->Get("h_DijetMass_MI_nPVg24_data_pf;1");
  TH1D* h_DijetMass_MI_nPVl3_data_pf = (TH1D*) _file0->Get("h_DijetMass_MI_nPVl1_9_data_pf;1");
  
  h_DijetMass_MI_nPVl3_data_pf->Scale(1000000.);
  h_DijetMass_MI_nPVg10_data_pf->Scale(1000000.);

   h_DijetMass_MI_nPVl3_data_pf->Rebin();
  //  h_DijetMass_MI_nPVl3_data_pf->Rebin();

   h_DijetMass_MI_nPVg10_data_pf->Rebin();
   //  h_DijetMass_MI_nPVg10_data_pf->Rebin();

   /*
   h_DijetMass_MI_nPVl3_data_fat->Scale(10000);
   h_DijetMass_MI_nPVg10_data_fat->Scale(10000);
   h_DijetMass_MI_nPVl3_data_pf->Scale(10000);
   h_DijetMass_MI_nPVg10_data_pf->Scale(10000);
   */

   h_DijetMass_MI_nPVl3_data_pf->Sumw2();
   h_DijetMass_MI_nPVg10_data_pf->Sumw2();
   h_DijetMass_MI_nPVl3_data_fat->Sumw2();
   h_DijetMass_MI_nPVg10_data_fat->Sumw2();




  TH1D* PLOTTER = new TH1D("PLOTTER", "PLOTTER", 1, 700, 2300);



  //  TF1* L1 = new TF1("L1", "[0]+[1]*x");
  TF1* L1 = new TF1("L1", "[0]");
  L1->SetLineStyle(2);




  //  TF1* L2 = new TF1("L2", "[0]+[1]*x");
  TF1* L2 = new TF1("L2", "[0]");
  L2->SetLineColor(kRed);
  L2->SetLineStyle(2);



  TF1* L4 = new TF1("L4", "[0]");
  L4->SetLineColor(kRed);
  L4->SetLineStyle(2);



  h_DijetMass_MI_nPVl3_data_fat->Divide(h_DijetMass_MI_nPVl3_data_pf);
  h_DijetMass_MI_nPVg10_data_fat->Divide(h_DijetMass_MI_nPVg10_data_pf);

  PLOTTER->SetStats(0);
  PLOTTER->SetMinimum(1.0);
  PLOTTER->SetMaximum(3);


  h_DijetMass_MI_nPVg10_data_fat->Fit(L1, "", "", 948, 2300);
  h_DijetMass_MI_nPVl3_data_fat->Fit(L2, "", "", 948, 2300);
  

  //PLOTTER->SetTitle("Impact of pilu-up on Fat jets MC; M (GeV); Fat/PF");
  PLOTTER->SetTitle("Impact of pilu-up on Fat jets in 2012; M (GeV); Fat/PF");


  PLOTTER->Draw();
  

  h_DijetMass_MI_nPVl3_data_fat->SetLineColor(kRed);

  h_DijetMass_MI_nPVl3_data_fat->Draw("SAME");
  h_DijetMass_MI_nPVg10_data_fat->Draw("SAME");

  TLegend* L  = new TLegend(0.3, 0.7, 0.8, 0.9);
  double rate = (L2->GetParameter(0)-L1->GetParameter(0))/L2->GetParameter(0);
  string name(Form("    Pile-Up Effect = %.0f", fabs(rate)*100));
  name = name + " %";
  L->SetHeader(name.c_str());

  string name = Form("Fat/PF = %.2f : nVtx <= 3", L2->GetParameter(0));
  


  L->AddEntry(h_DijetMass_MI_nPVl3_data_fat, name.c_str(), "l");


  string name = Form("Fat/PF = %.2f : nVtx > 10", L1->GetParameter(0));

  L->AddEntry(h_DijetMass_MI_nPVg10_data_fat, name.c_str(), "l");
  L->Draw();

  //  C->SaveAs("PileUpImpact_lower3_greater10Vtx_MC.png");
  C->SaveAs("PileUpImpact_lower3_greater10Vtx_PileUpStudies_Fat30_2012.png");




  TH2D* h_nVtx_DijetMass_pf = (TH2D*) _file0->Get("h_nVtx_DijetMass_pf;1");
  TH2D* h_nVtx_DijetMass_fat = (TH2D*) _file0->Get("h_nVtx_DijetMass_fat;1");

  h_nVtx_DijetMass_pf->Scale(1000000.);
  h_nVtx_DijetMass_fat->Scale(1000000.);

  //  h_nVtx_DijetMass_pf->RebinX();
  //  h_nVtx_DijetMass_fat->RebinX();
  h_nVtx_DijetMass_pf->RebinY();
  h_nVtx_DijetMass_fat->RebinY();
  //  h_nVtx_DijetMass_pf->RebinY();
  //  h_nVtx_DijetMass_fat->RebinY();
  /*
  h_nVtx_DijetMass_pf->Scale(10000.);
  h_nVtx_DijetMass_fat->Scale(10000.);
  */

  h_nVtx_DijetMass_pf->Sumw2();
  h_nVtx_DijetMass_fat->Sumw2();


  h_nVtx_DijetMass_fat->Divide(h_nVtx_DijetMass_pf);
  h_nVtx_DijetMass_fat->SetMaximum(3.0);
  h_nVtx_DijetMass_fat->SetMinimum(0);


  TF2* L3 = new TF2("L3", "[0]+ [1]*x + 0*y");
  h_nVtx_DijetMass_fat->Fit(L3);

  h_nVtx_DijetMass_fat->Draw("LEGO");

  TH1D* Mass = h_nVtx_DijetMass_fat->ProjectionY("Mass", 1,1);
 
  TGraphErrors* g = new TGraphErrors(7);
  
  TCanvas* myCanvas = new TCanvas("myCanvas","myCanvas",1000,1000);
  myCanvas->Divide(3,3);

  for (int i = 1; i < 8; i++){
    myCanvas.cd(i);
    if (i == 1) Mass->DrawCopy();
    else Mass->DrawCopy("SAMEH");
    Mass = h_nVtx_DijetMass_fat->ProjectionY(Form("Mass_%d",i), i,i);
    Mass->SetLineColor(i);
    Mass->Fit(L4, "", "", 948, 2300);
    g->SetPoint(i, i*4-2, L4->GetParameter(0));
    g->SetPointError(i, 1, 0.001);
  }
  
  
  C->cd();

  TH1D* PLOTTER = new TH1D("PLOTTER", "PLOTTER", 1, 0, 30);
  PLOTTER->SetMaximum(1.8);
  PLOTTER->SetMinimum(0.7);
  g->SetMarkerSize(1);
  g->SetMarkerStyle(20);

  PLOTTER->Draw();
  g->Draw("SAMEP");
  
  
  TF1* L1 = new TF1("L1", "[0]+[1]*x");
  L1->SetParameters(1.47, 0.001*1.47);
  L1->SetLineStyle(2);
  L1->SetLineColor(kRed);
  g->Fit(L1, "", "", 0, 30);

  PLOTTER->SetTitle("Fat/PF rate dependance  data: 2012; nVtx; Fat/PF");
  PLOTTER->SetStats(0);
  
  TLegend* L  = new TLegend(0.3, 0.2, 0.8, 0.4);
  double rate = L1->GetParameter(1)/L1->GetParameter(0);
  string name(Form("    Pile-Up Effect = %.1f", rate*100));
  name = name + " % / vtx";
  L->SetHeader(name.c_str());
  L->Draw();
  
  C->SaveAs("PileUpImpact_PileUpStudies_Fat30_2012.png");
  

  /*


  TH2D* h_nVtx_DijetMass_pf = (TH2D*) _file0->Get("h_nVtx_DijetMass_pf;1");
  TH2D* h_nVtx_DijetMass_fat = (TH2D*) _file0->Get("h_nVtx_DijetMass_minPt20_betaStar_data_fat;1");

  h_nVtx_DijetMass_pf->RebinX();
  h_nVtx_DijetMass_fat->RebinX();
  //  h_nVtx_DijetMass_pf->RebinX();
  //  h_nVtx_DijetMass_fat->RebinX();
  //  h_nVtx_DijetMass_pf->RebinY();
  // h_nVtx_DijetMass_fat->RebinY();
  //  h_nVtx_DijetMass_pf->RebinY();
  //  h_nVtx_DijetMass_fat->RebinY();

  h_nVtx_DijetMass_pf->Sumw2();
  h_nVtx_DijetMass_fat->Sumw2();


  h_nVtx_DijetMass_fat->Divide(h_nVtx_DijetMass_pf);
  h_nVtx_DijetMass_fat->SetMaximum(1.8);
  h_nVtx_DijetMass_fat->SetMinimum(1.2);



  TF2* L3 = new TF2("L3", "[0]+ [1]*x + 0*y");
  h_nVtx_DijetMass_fat->Fit(L3);

  h_nVtx_DijetMass_fat->Draw("LEGO");

  TH1D* Mass = h_nVtx_DijetMass_fat->ProjectionY("Mass", 1,1);
 
  TH1D* PLOTTER = new TH1D("PLOTTER", "PLOTTER", 1, 840, 1500);
  PLOTTER->SetMaximum(1.8);
  PLOTTER->SetMinimum(1.2);

  PLOTTER->Draw();

  TGraphErrors* g = new TGraphErrors(7);

  for (int i = 1; i < 8; i++){
    
    Mass = h_nVtx_DijetMass_fat->ProjectionY("Mass", i,i);
    Mass->SetLineColor(i);
    Mass->Fit(L4, "", "", 948, 2300);
    Mass->DrawCopy("SAMEH");
    g->SetPoint(i, i*2-1, L4->GetParameter(0));
    g->SetPointError(i, 1, L4->GetParError(0));
  }
  
  TH1D* PLOTTER = new TH1D("PLOTTER", "PLOTTER", 1, 0, 15);
  PLOTTER->SetMaximum(1.8);
  PLOTTER->SetMinimum(0.0);
  
  PLOTTER->Draw();
  g->Draw("SAMEPE");
  
  
  TF1* L1 = new TF1("L1", "[0]+[1]*x");
  L1->SetLineStyle(2);
  g->Fit(L1);

  PLOTTER->SetTitle("Fat/PF rate dependance 2011BSingleJet data: MC; nVtx; Fat/PF");
  PLOTTER->SetStats(0);
  
  TLegend* L  = new TLegend(0.3, 0.2, 0.8, 0.4);
  double rate = L1->GetParameter(1)/L1->GetParameter(0);
  string name(Form("    Pile-Up Effect = %.1f", rate*100));
  name = name + " % / vtx";
  L->SetHeader(name.c_str());
  L->Draw();
  
  C->SaveAs("PileUpImpact_data_2011BSingleJet_PT20GeV_betaStar_PileUpStudies_MC.png");
  */

}
