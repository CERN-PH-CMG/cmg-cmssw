{

  bool bRebinTrans = false;

  TFile *_file0 = TFile::Open("Qstar3500_EfficiencyStudy.root");
  

  TCanvas* canvas = new TCanvas("canvas", "M(Q*) = 3.5 TeV, Partons and Gen jets", 600, 600);
 canvas->Divide(2,2);

     canvas->cd(1);

  

 

  TH1F *vFrame = gPad->DrawFrame(0,0.,1800.,350.);
  vFrame->SetTitle("Partons pT");
  vFrame->SetTitleSize(0.2);
  vFrame->SetXTitle("pT (GeV)");
  vFrame->GetYaxis()->SetLabelSize(0.03);
  vFrame->GetYaxis()->SetTitleSize(0.04);
  vFrame->GetYaxis()->SetTitleOffset(1.2);
  vFrame->GetXaxis()->SetLabelSize(0.03);
  vFrame->GetXaxis()->SetTitleSize(0.04);
  vFrame->GetXaxis()->SetTitleOffset(0.95);
  vFrame->SetYTitle("#events");



    TH1F* PartonPt1 = (TH1F*) gDirectory->Get("PartonPt1;1");
    TH1F* PartonPt2 = (TH1F*) gDirectory->Get("PartonPt2;1");
    
    PartonPt2->SetLineColor(kRed);
    PartonPt2->SetLineStyle(2);


    
    PartonPt1->SetStats(0);
    PartonPt2->SetStats(0);

    //    LongShapeFullSim->SetTitle(sTitle.c_str());
    //    LongShapeFastSim->SetTitle(sTitle.c_str());
    vFrame->DrawClone();
    PartonPt1->Draw("SAME");
    PartonPt2->Draw("SAME");

    TLegend* Legend = new TLegend(0.15, 0.6, 0.5., 0.8);
    Legend->AddEntry(PartonPt1, "Parton 1", "l");
    Legend->AddEntry(PartonPt2, "Parton 2", "l");
    Legend->DrawClone();

    canvas->cd(2);

  TH1F *vFrame2 = gPad->DrawFrame(0,0.,1800.,350.);
  vFrame2->SetTitle("Partons pT");
  vFrame2->SetTitleSize(0.2);
  vFrame2->SetXTitle("pT (GeV)");
  vFrame2->GetYaxis()->SetLabelSize(0.03);
  vFrame2->GetYaxis()->SetTitleSize(0.04);
  vFrame2->GetYaxis()->SetTitleOffset(1.2);
  vFrame2->GetXaxis()->SetLabelSize(0.03);
  vFrame2->GetXaxis()->SetTitleSize(0.04);
  vFrame2->GetXaxis()->SetTitleOffset(0.95);
  vFrame2->SetYTitle("#events");




    TH1F* GenPt1 = (TH1F*) gDirectory->Get("GenPt1;1");
    TH1F* GenPt2 = (TH1F*) gDirectory->Get("GenPt2;1");
    
    GenPt2->SetLineColor(kRed);
    GenPt2->SetLineStyle(2);    


    
    GenPt1->SetStats(0);
    GenPt2->SetStats(0);

    //    LongShapeFullSim->SetTitle(sTitle.c_str());
    //    LongShapeFastSim->SetTitle(sTitle.c_str());

    vFrame2->SetTitle("Gen pT");
    vFrame2->DrawClone();
    GenPt1->DrawClone("SAME");
    GenPt2->DrawClone("SAME");
  
    TLegend* Legend = new TLegend(0.15, 0.6, 0.5., 0.8);
    Legend->AddEntry(GenPt1, "Gen jet 1", "l");
    Legend->AddEntry(GenPt2, "Gen jet 2", "l");
    Legend->DrawClone();


    canvas->cd(3);

  TH1F *vFrame3 = gPad->DrawFrame(0,0.,1800.,350.);
  vFrame3->SetTitle("Partons pT");
  vFrame3->SetTitleSize(0.2);
  vFrame3->SetXTitle("pT (GeV)");
  vFrame3->GetYaxis()->SetLabelSize(0.03);
  vFrame3->GetYaxis()->SetTitleSize(0.04);
  vFrame3->GetYaxis()->SetTitleOffset(1.2);
  vFrame3->GetXaxis()->SetLabelSize(0.03);
  vFrame3->GetXaxis()->SetTitleSize(0.04);
  vFrame3->GetXaxis()->SetTitleOffset(0.95);
  vFrame3->SetYTitle("#events");



    TH1F* GenPt1 = (TH1F*) gDirectory->Get("GenPt1_withEtaMatching;1");
    TH1F* GenPt2 = (TH1F*) gDirectory->Get("GenPt2_withEtaMatching;1");
    
    GenPt2->SetLineColor(kRed);
    GenPt2->SetLineStyle(2);  


    
    GenPt1->SetStats(0);
    GenPt2->SetStats(0);

    //    LongShapeFullSim->SetTitle(sTitle.c_str());
    //    LongShapeFastSim->SetTitle(sTitle.c_str());
    vFrame3->SetTitle("Gen pT (both jets matched to partons #Delta #eta < 0.5)");
    vFrame3->DrawClone();
    GenPt1->DrawClone("SAME");
    GenPt2->DrawClone("SAME");

    canvas->cd(4);

  TH1F *vFrame4 = gPad->DrawFrame(0,0.,1800.,350.);
  vFrame4->SetTitle("Partons pT");
  vFrame4->SetTitleSize(0.2);
  vFrame4->SetXTitle("pT (GeV)");
  vFrame4->GetYaxis()->SetLabelSize(0.03);
  vFrame4->GetYaxis()->SetTitleSize(0.04);
  vFrame4->GetYaxis()->SetTitleOffset(1.2);
  vFrame4->GetXaxis()->SetLabelSize(0.03);
  vFrame4->GetXaxis()->SetTitleSize(0.04);
  vFrame4->GetXaxis()->SetTitleOffset(0.95);
  vFrame4->SetYTitle("#events");






    TH1F* GenPt1 = (TH1F*) gDirectory->Get("GenPt1_withoutEtaMatching;1");
    TH1F* GenPt2 = (TH1F*) gDirectory->Get("GenPt2_withoutEtaMatching;1");
    
    GenPt2->SetLineColor(kRed);
    GenPt2->SetLineStyle(2);  


    
    GenPt1->SetStats(0);
    GenPt2->SetStats(0);

    //    LongShapeFullSim->SetTitle(sTitle.c_str());
    //    LongShapeFastSim->SetTitle(sTitle.c_str());
    vFrame->SetTitle("Gen pT (both jets not-matched to partons #Delta #eta < 0.5)");
    vFrame->DrawClone();
    GenPt1->DrawClone("SAME");
    GenPt2->DrawClone("SAME");

   canvas->SaveAs("Qstar35TeV_JetPt.png");


    TCanvas* canvas1 = new TCanvas("canvas1", "M(Q*) = 3.5 TeV, #theta*", 600, 600);
    canvas1->Divide(2,2);
    
    canvas1->cd(1);
    

    TH1F* Part = (TH1F*) gDirectory->Get("absCosThetaStar;1");
    TH1F* Part1 = (TH1F*) gDirectory->Get("absCosThetaStar_withEtaCuts;1");
    TH1F* Part2 = (TH1F*) gDirectory->Get("absCosThetaStar_withDEtaCuts;1");

    Part1->SetLineColor(kMagenta);
    Part1->SetLineStyle(2);


    Part2->SetFillColor(kGreen);
    Part2->SetLineColor(kGreen);
    Part2->SetLineStyle(3);

  TH1F *vFrame5 = gPad->DrawFrame(0,0.,1.,300.);
  vFrame5->SetTitle("|cos(#theta*)|");
  vFrame5->SetTitleSize(0.2);
  vFrame5->SetXTitle("");
  vFrame5->GetYaxis()->SetLabelSize(0.03);
  vFrame5->GetYaxis()->SetTitleSize(0.04);
  vFrame5->GetYaxis()->SetTitleOffset(1.2);
  vFrame5->GetXaxis()->SetLabelSize(0.03);
  vFrame5->GetXaxis()->SetTitleSize(0.04);
  vFrame5->GetXaxis()->SetTitleOffset(0.95);
  vFrame5->SetYTitle("#events");

  vFrame5->Draw();
  Part2->Draw("SAME");
  Part->Draw("SAME");
  Part1->Draw("SAME");

  TLegend* Legend = new TLegend(0.15, 0.75, 0.5., 0.9);
  Legend->AddEntry(Part, "No cuts", "l");
  Legend->AddEntry(Part1, "|#eta| < 2.5", "l");
  Legend->AddEntry(Part2, "|#Delta #eta| < 1.3", "f");
  Legend->DrawClone();


   canvas1->cd(3);
    

    TH1F* Gen = (TH1F*) gDirectory->Get("genAbsCosThetaStar;1");

    Gen->SetLineColor(kRed);
    Gen->SetLineStyle(2);

  TH1F *vFrame6 = gPad->DrawFrame(0,0.,1.,300.);
  vFrame6->SetTitle("|cos(#theta* gen)|");
  vFrame6->SetTitleSize(0.2);
  vFrame6->SetXTitle("");
  vFrame6->GetYaxis()->SetLabelSize(0.03);
  vFrame6->GetYaxis()->SetTitleSize(0.04);
  vFrame6->GetYaxis()->SetTitleOffset(1.2);
  vFrame6->GetXaxis()->SetLabelSize(0.03);
  vFrame6->GetXaxis()->SetTitleSize(0.04);
  vFrame6->GetXaxis()->SetTitleOffset(0.95);
  vFrame6->SetYTitle("#events");

  vFrame6->Draw();
  Part->DrawClone("SAME");
  Gen->DrawClone("SAME");

  TLegend* Legend = new TLegend(0.15, 0.20, 0.5., 0.35);
  Legend->AddEntry(Part, "Partons", "l");
  Legend->AddEntry(Gen, "Gen jets", "l");
  Legend->DrawClone();

    canvas1->cd(4);
    

    TH1F* Gen = (TH1F*) gDirectory->Get("genAbsCosThetaStar_withoutEtaMatching;1");
    TH1F* Part = (TH1F*) gDirectory->Get("absCosThetaStar_withoutEtaMatching;1");


    Gen->SetLineColor(kRed);
    Gen->SetLineStyle(2);


 

  TH1F *vFrame6 = gPad->DrawFrame(0,0.,1.,300.);
  vFrame6->SetTitle("|cos(#theta*)| (both jets matched to partons #Delta #eta < 0.5)");
  vFrame6->SetTitleSize(0.2);
  vFrame6->SetXTitle("");
  vFrame6->GetYaxis()->SetLabelSize(0.03);
  vFrame6->GetYaxis()->SetTitleSize(0.04);
  vFrame6->GetYaxis()->SetTitleOffset(1.2);
  vFrame6->GetXaxis()->SetLabelSize(0.03);
  vFrame6->GetXaxis()->SetTitleSize(0.04);
  vFrame6->GetXaxis()->SetTitleOffset(0.95);
  vFrame6->SetYTitle("#events");

  vFrame6->Draw();
  Part->Draw("SAME");
  Gen->Draw("SAME");

  TLegend* Legend = new TLegend(0.15, 0.20, 0.5., 0.35);
  Legend->AddEntry(Part, "Partons", "l");
  Legend->AddEntry(Gen, "Gen jets", "l");
  Legend->DrawClone();







    canvas1->cd(2);
    

    TH1F* Gen = (TH1F*) gDirectory->Get("ResonanceMass_WithMatching;1");
    TH1F* Gen1 = (TH1F*) gDirectory->Get("ResonanceMass_WithoutMatching;1");


    Gen1->SetLineColor(kRed);
    Gen1->SetLineStyle(2);


 

  TH1F *vFrame7 = gPad->DrawFrame(0,0.,4000.,700.);
  vFrame7->SetTitle("Real resonance mass");
  vFrame7->SetTitleSize(0.2);
  vFrame7->SetXTitle("M_{R} (GeV)");
  vFrame7->GetYaxis()->SetLabelSize(0.03);
  vFrame7->GetYaxis()->SetTitleSize(0.04);
  vFrame7->GetYaxis()->SetTitleOffset(1.2);
  vFrame7->GetXaxis()->SetLabelSize(0.03);
  vFrame7->GetXaxis()->SetTitleSize(0.04);
  vFrame7->GetXaxis()->SetTitleOffset(0.95);
  vFrame7->SetYTitle("#events");

  vFrame7->Draw();
  Gen->Draw("SAME");
  Gen1->Draw("SAME");

  TLegend* Legend = new TLegend(0.15, 0.30, 0.7, 0.60);
  Legend->AddEntry(Gen, "Real M(R) for matched jets", "l");
  Legend->AddEntry(Gen1, "Real M(R) for unmatched jets", "l");
  Legend->DrawClone();

  canvas1->SaveAs("Qstar35TeV_CosThetaStar.png");


}
