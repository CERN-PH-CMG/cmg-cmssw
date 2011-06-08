{

  string files[8] = {"out/res/histograms_mc_170to300_JES.root","out/res/histograms_mc_300to470_JES.root",  "out/res/histograms_mc_470to600_JES.root", "out/res/histograms_mc_600to800_JES.root", "out/res/histograms_mc_800to1000_JES.root", "out/res/histograms_mc_1000to1400_JES.root", "out/res/histograms_mc_1400to1800_JES.root", "out/res/histograms_mc_1800toinf_JES.root"};

  TCanvas* PT_HAT = new TCanvas("PT_HAT", "PT_HAT", 1300, 1000);
  PT_HAT->SetFillColor(10);

 TObjArray* Mean = new TObjArray();
 TObjArray* UP = new TObjArray();
 TObjArray* DW = new TObjArray();

 TObjArray* Mean_pf = new TObjArray();
 TObjArray* UP_pf = new TObjArray();
 TObjArray* DW_pf = new TObjArray();

 TObjArray* Mean_fat = new TObjArray();
 TObjArray* UP_fat = new TObjArray();
 TObjArray* DW_fat = new TObjArray();

 TObjArray* UP2 = new TObjArray();
 TObjArray* DW2 = new TObjArray();

 TObjArray* UP2_pf = new TObjArray();
 TObjArray* DW2_pf = new TObjArray();

 TObjArray* UP2_fat = new TObjArray();
 TObjArray* DW2_fat = new TObjArray();

 TH1D *hMean = new TH1D(); 
 TH1D *hMean_pf = new TH1D(); 
 TH1D *hMean_fat = new TH1D(); 

 TH1D *hUP = new TH1D(); 
 TH1D *hUP_pf = new TH1D(); 
 TH1D *hUP_fat = new TH1D(); 

 TH1D *hDW = new TH1D(); 
 TH1D *hDW_pf = new TH1D(); 
 TH1D *hDW_fat = new TH1D(); 

 TH2D *h2UP = new TH2D(); 
 TH2D *h2UP_pf = new TH2D(); 
 TH2D *h2UP_fat = new TH2D(); 

 TH2D *h2DW = new TH2D(); 
 TH2D *h2DW_pf = new TH2D(); 
 TH2D *h2DW_fat = new TH2D(); 






 TH1D* PLOTTER = new TH1D("PLOTTER", "JES", 1, 780, 3000);
 PLOTTER->SetMinimum(1e-2);
 PLOTTER->SetStats(0);

 float events[8] = {5671529., 6308796., 3050637., 3939510., 4161710., 2169144., 2043020., 1056368.};
  float xs[8] = {2.426e+04, 1.168e+03, 7.022e+01, 1.555e+01, 1.844e+00, 3.321e-01, 1.087e-02, 3.575e-04};

 for (int i = 0; i < 8; i++){
   
   TFile *_file0 = TFile::Open(files[i].c_str());
   Mean->Add((TH1D* ) _file0->Get("h_DijetMass_data;1"));
   Mean_pf->Add((TH1D* ) _file0->Get("h_DijetMass_data_pf;1"));
   Mean_fat->Add((TH1D* ) _file0->Get("h_DijetMass_data_fat;1"));

   UP->Add((TH1D* ) _file0->Get("h_DijetMass_data_UP;1"));
   UP_pf->Add((TH1D* ) _file0->Get("h_DijetMass_data_UP_pf;1"));
   UP_fat->Add((TH1D* ) _file0->Get("h_DijetMass_data_UP_fat;1"));

   DW->Add((TH1D* ) _file0->Get("h_DijetMass_data_DW;1"));
   DW_pf->Add((TH1D* ) _file0->Get("h_DijetMass_data_DW_pf;1"));
   DW_fat->Add((TH1D* ) _file0->Get("h_DijetMass_data_DW_fat;1"));
 
   UP2->Add((TH2D* ) _file0->Get("p_DijetMass_data_UP;1"));
   UP2_pf->Add((TH2D* ) _file0->Get("p_DijetMass_data_UP_pf;1"));
   UP2_fat->Add((TH2D* ) _file0->Get("p_DijetMass_data_UP_fat;1"));

   DW2->Add((TH2D* ) _file0->Get("p_DijetMass_data_DW;1"));
   DW2_pf->Add((TH2D* ) _file0->Get("p_DijetMass_data_DW_pf;1"));
   DW2_fat->Add((TH2D* ) _file0->Get("p_DijetMass_data_DW_fat;1"));

}

 TFile *_file0 = TFile::Open("out/res/histograms_data_HT.root");
 TH1D* MeanData = (TH1D* ) _file0->Get("h_DijetMass_data;1");
 TH1D* MeanData_pf = (TH1D* ) _file0->Get("h_DijetMass_data_pf;1");
 TH1D* MeanData_fat = (TH1D* ) _file0->Get("h_DijetMass_data_fat;1");

 
 (Mean->At(0))->Copy(*hMean); hMean->Scale(0);
 (Mean->At(0))->Copy(*hMean_pf); hMean_pf->Scale(0);
 (Mean->At(0))->Copy(*hMean_fat); hMean_fat->Scale(0);
 (Mean->At(0))->Copy(*hUP); hUP->Scale(0);
 (Mean->At(0))->Copy(*hUP_pf); hUP_pf->Scale(0);
 (Mean->At(0))->Copy(*hUP_fat); hUP_fat->Scale(0);
 (Mean->At(0))->Copy(*hDW); hDW->Scale(0);
 (Mean->At(0))->Copy(*hDW_pf); hDW_pf->Scale(0);
 (Mean->At(0))->Copy(*hDW_fat); hDW_fat->Scale(0);

 (UP2->At(0))->Copy(*h2UP); h2UP->Scale(0);
 (UP2->At(0))->Copy(*h2UP_pf); h2UP_pf->Scale(0);
 (UP2->At(0))->Copy(*h2UP_fat); h2UP_fat->Scale(0);
 (DW2->At(0))->Copy(*h2DW); h2DW->Scale(0);
 (DW2->At(0))->Copy(*h2DW_pf); h2DW_pf->Scale(0);
 (DW2->At(0))->Copy(*h2DW_fat); h2DW_fat->Scale(0);

 for (int i = 0; i < 8; i++){
   hMean->Add((TH1D*) Mean->At(i), xs[i]/events[i]); 
   hMean_pf->Add((TH1D*) Mean_pf->At(i), xs[i]/events[i]); 
   hMean_fat->Add((TH1D*) Mean_fat->At(i), xs[i]/events[i]); 
   
   hUP->Add((TH1D*) UP->At(i), xs[i]/events[i]); 
   hUP_pf->Add((TH1D*) UP_pf->At(i), xs[i]/events[i]); 
   hUP_fat->Add((TH1D*) UP_fat->At(i), xs[i]/events[i]); 

   hDW->Add((TH1D*) DW->At(i), xs[i]/events[i]); 
   hDW_pf->Add((TH1D*) DW_pf->At(i), xs[i]/events[i]); 
   hDW_fat->Add((TH1D*) DW_fat->At(i), xs[i]/events[i]); 


   h2UP->Add((TH2D*) UP2->At(i), xs[i]/events[i]); 
   h2UP_pf->Add((TH2D*) UP2_pf->At(i), xs[i]/events[i]); 
   h2UP_fat->Add((TH2D*) UP2_fat->At(i), xs[i]/events[i]); 

   h2DW->Add((TH2D*) DW2->At(i), xs[i]/events[i]); 
   h2DW_pf->Add((TH2D*) DW2_pf->At(i), xs[i]/events[i]); 
   h2DW_fat->Add((TH2D*) DW2_fat->At(i), xs[i]/events[i]); 
 }


 hUP->SetFillColor(kYellow);
 hUP_pf->SetFillColor(kYellow);
 hUP_fat->SetFillColor(kYellow);

 hUP->SetLineColor(kYellow);
 hUP_pf->SetLineColor(kYellow);
 hUP_fat->SetLineColor(kYellow);

 hDW->SetFillColor(10);
 hDW_pf->SetFillColor(10);
 hDW_fat->SetFillColor(10);

 hDW->SetLineColor(kYellow);
 hDW_pf->SetLineColor(kYellow);
 hDW_fat->SetLineColor(kYellow);


   TFile *outf = new TFile("JES.root","RECREATE");

   hUP->Write("hUP");
   hUP_pf->Write("hUP_pf");
   hUP_fat->Write("hUP_fat");
   
   hDW->Write("hDW");
   hDW_pf->Write("hDW_pf");
   hDW_fat->Write("hDW_fat");

   hMean->Write("hMean");
   hMean_pf->Write("hMean_pf");
   hMean_fat->Write("hMean_fat");
   outf->Close();



   PT_HAT->cd();

 MeanData_pf->Divide(MeanData);
 

 PLOTTER->SetMaximum(1.3);
 PLOTTER->SetMinimum(0.7);

 PLOTTER->SetTitle("Calo/PF data; M_{jj}; Spectrum(PF)/Spectrum(Calo) data;");

 PLOTTER->DrawCopy();


 TLine* L = new TLine(780, 1, 3000, 1);
 L->SetLineStyle(2);
 L->SetLineColor(kRed);
 L->SetLineWidth(2);
 L->Draw("SAME");
 MeanData_pf->Draw("SAME"); 
 
 TLine* L1 = new TLine(780, 1.03, 3000, 1.03);
 L1->SetLineStyle(3);
 L1->SetLineColor(kBlue);
 L1->SetLineWidth(2);
 L1->Draw("SAME");
 MeanData_pf->Draw("SAME");

 PT_HAT->SaveAs("PFoverCALO_data.png");

 PT_HAT->Clear();
   PT_HAT->cd();


   TH1D* hMean_pf_rate = new TH1D();
   hMean_pf->Copy(*hMean_pf_rate);
   hMean_pf_rate->Divide(hMean);

 
 

 PLOTTER->SetMaximum(1.3);
 PLOTTER->SetMinimum(0.7);

 PLOTTER->SetTitle("Calo/PF PYTHIA D6T; M_{jj}; Spectrum(PF)/Spectrum(Calo) PYTHIA D6T;");

 PLOTTER->DrawCopy();


 TLine* L = new TLine(780, 1, 3000, 1);
 L->SetLineStyle(2);
 L->SetLineColor(kRed);
 L->SetLineWidth(2);
 L->Draw("SAME");

 
 TLine* L1 = new TLine(780, 1.03, 3000, 1.03);
 L1->SetLineStyle(3);
 L1->SetLineColor(kBlue);
 L1->SetLineWidth(2);
 L1->Draw("SAME");
 hMean_pf_rate->Draw("SAME");

 PT_HAT->SaveAs("PFoverCALO_PYTHIAD6T.png");




 PT_HAT->cd();
 PT_HAT->Divide(2,3);


 PT_HAT->cd(1);
 gPad->SetLogy(1);

 PLOTTER->SetMaximum(hUP->GetMaximum()*1.2);
 PLOTTER->SetMinimum(1e-2);
 PLOTTER->DrawCopy();

 hUP->DrawCopy("SAME");
 hMean->DrawCopy("SAME");
 hDW->DrawCopy("SAME");
 
 PT_HAT->cd(2);
 PLOTTER->SetMaximum(1.3);
 PLOTTER->SetMinimum(0.7);
 PLOTTER->DrawCopy();

 hUP->Divide(hMean);
 hDW->Divide(hMean);
 hMean->Divide(hMean);
 hUP->DrawCopy("SAME");
 hMean->DrawCopy("SAME");
 hDW->DrawCopy("SAME");




 PT_HAT->cd(3);
 PLOTTER->SetMaximum(hUP_pf->GetMaximum()*1.2);
 PLOTTER->SetMinimum(1e-2);
 PLOTTER->DrawCopy();

 gPad->SetLogy(1);
 hUP_pf->DrawCopy("SAME");
 hMean_pf->DrawCopy("SAME");
 hDW_pf->DrawCopy("SAME");
 
 PT_HAT->cd(4);
 PLOTTER->SetMaximum(1.3);
 PLOTTER->SetMinimum(0.7);
 PLOTTER->DrawCopy();

 hUP_pf->Divide(hMean_pf);
 hDW_pf->Divide(hMean_pf);
 hMean_pf->Divide(hMean_pf);
 hUP_pf->DrawCopy("SAME");
 hMean_pf->DrawCopy("SAME");
 hDW_pf->DrawCopy("SAME");



 PT_HAT->cd(5);

 PLOTTER->SetMaximum(hUP_fat->GetMaximum()*1.2);
 PLOTTER->SetMinimum(1e-2);
 PLOTTER->DrawCopy();
 gPad->SetLogy(1);
 hUP_fat->DrawCopy("SAME");
 hMean_fat->DrawCopy("SAME");
 hDW_fat->DrawCopy("SAME");
 
 PT_HAT->cd(6);

 PLOTTER->SetMaximum(1.3);
 PLOTTER->SetMinimum(0.7);
 PLOTTER->DrawCopy();


 hUP_fat->Divide(hMean_fat);
 hDW_fat->Divide(hMean_fat);
 hMean_fat->Divide(hMean_fat);
 hUP_fat->DrawCopy("SAME");
 hMean_fat->DrawCopy("SAME");
 hDW_fat->DrawCopy("SAME");


 PT_HAT->SaveAs("JES.png");



   PT_HAT->Clear();

   hUP->SetFillColor(kRed);
   hUP->SetLineColor(kRed);

   hUP_pf->SetFillColor(kYellow);
   hUP_pf->SetLineColor(kYellow);

   hDW_pf->SetFillColor(kRed);
   hDW_pf->SetLineColor(kRed);

   PLOTTER->SetTitle("JES Calo (Red), PF (Yellow) ; M_{jj}; JES;");

   PLOTTER->DrawCopy();
   hUP->DrawCopy("SAME");
   hUP_pf->DrawCopy("SAME");
   hDW_pf->DrawCopy("SAME");
   hDW->DrawCopy("SAME");
   //   PLOTTER->DrawCopy("ASAME");


 PT_HAT->SaveAs("JES_FLAVORS.png");



   PT_HAT->Clear();

   hUP->SetFillColor(kRed);
   hUP->SetLineColor(kRed);

   hUP_fat->SetFillColor(kGreen);
   hUP_fat->SetLineColor(kGreen);

   hDW_fat->SetFillColor(kRed);
   hDW_fat->SetLineColor(kRed);

   PLOTTER->SetTitle("JES Calo (Red), FAT PF (GREEN) ; M_{jj}; JES;");

   PLOTTER->DrawCopy();
   hUP->DrawCopy("SAME");
   hUP_fat->DrawCopy("SAME");
   hDW_fat->DrawCopy("SAME");
   hDW->DrawCopy("SAME");
   //   PLOTTER->DrawCopy("ASAME");


 PT_HAT->SaveAs("JES_FLAVORS_FAT.png");


  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 

  TProfile* pUP = new TProfile("pUP", "pUP", 83, xAxis1, -0.1, 0.1); 
  TProfile* pUP_pf = new TProfile("pUP_pf", "pUP_pf", 83, xAxis1, -0.1, 0.1); 
  TProfile* pUP_fat = new TProfile("pUP_fat", "pUP_fat", 83, xAxis1, -0.1, 0.1); 

  TProfile* pDW = new TProfile("pDW", "pDW", 83, xAxis1, -0.1, 0.1); 
  TProfile* pDW_pf = new TProfile("pDW_pf", "pDW_pf", 83, xAxis1, -0.1, 0.1); 
  TProfile* pDW_fat = new TProfile("pDW_fat", "pDW_fat", 83, xAxis1, -0.1, 0.1); 


  for (int i = 0; i < h2UP->GetNbinsX(); i++){
    double mass = h2UP->GetXaxis()->GetBinCenter(i);
    for (int j = 0; j < h2UP->GetNbinsY(); j++){
      double corr = h2UP->GetYaxis()->GetBinCenter(j);
      pUP->Fill(mass, corr, h2UP->GetBinContent(i,j));
      pUP_pf->Fill(mass, corr, h2UP_pf->GetBinContent(i,j));
      pUP_fat->Fill(mass, corr, h2UP_fat->GetBinContent(i,j));
      pDW->Fill(mass, corr, h2DW->GetBinContent(i,j));
      pDW_pf->Fill(mass, corr, h2DW_pf->GetBinContent(i,j));
      pDW_fat->Fill(mass, corr, h2DW_fat->GetBinContent(i,j));

    }
  }

  TH1D* PLOTTER = new TH1D("PLOTTER", "JES", 1, 780, 3000);
  PLOTTER->SetMinimum(-0.03);
  PLOTTER->SetMaximum(0.03);
  PLOTTER->SetStats(0);

  pUP->SetLineColor(kRed);
  pDW->SetLineColor(kRed);

  pUP_pf->SetLineColor(kBlue);
  pDW_pf->SetLineColor(kBlue);

  pUP_fat->SetLineColor(kBlack);
  pDW_fat->SetLineColor(kBlack);



  TLegend* Le = new TLegend(0.40, 0.40, 0.6, 0.6);
  Le->SetTextSize(0.03);
  Le->AddEntry(pUP, "Calo JES", "l");
  Le->AddEntry(pUP_pf, "PF JES", "l");
  Le->AddEntry(pUP_fat, "Fat PF JES", "l");

   PT_HAT->Clear();
  PT_HAT->cd();

  PLOTTER->Draw();
  pUP->Draw("SAME");
  pDW->Draw("SAME");
  pUP_pf->Draw("SAME");
  pDW_pf->Draw("SAME");
  pUP_fat->Draw("SAME");
  pDW_fat->Draw("SAME");

  Le->Draw();

  PT_HAT->SaveAs("Variations.png");

 // TF1* spectrum = TF1("Spectrum"p[1]*TMath::Power(1-*x/7000.,p[2])/TMath::Power(*x/7000.,p[3]+p[4]*log(*x/7000.));

}
