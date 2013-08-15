{


 TCanvas* TURN_ON_CURVE = new TCanvas();

 TFile *_file0 = TFile::Open("out/res/histograms_data_HT_Run2011B_178098_180252_WithAllTriggers_short_Fat30_ak5.root");


 TH3F* DijetMass_Trigger_data_fat_Corr = (TH3F* ) _file0->Get("h_DijetMass_Trigger_data_fat_Corr;1");
                                                                        
 TH2F* DijetMass_Trigger_data_fat = (TH2F* ) _file0->Get("h_DijetMass_Trigger_data_fat;1");
 
 
 TH1D* htMONandTRIG = DijetMass_Trigger_data_fat_Corr->ProjectionX("htMONandTRIG", 51, 51, 44, 44);


 TH1D* htMON = DijetMass_Trigger_data_fat->ProjectionX("htMON", 44,44);
 htMONandTRIG->Sumw2();
 htMON->Sumw2();

 TCanvas* c1 = new TCanvas("c1");
 c1->SetLeftMargin(0.15);

 TH1D* PLOTTER = new TH1D("PLOTTER", "Fat850 trigger efficiency for fat ; M_{12 Fat}; Fat850 && HT500 / HT500", 1, 400, 1400);

 PLOTTER->SetStats(0);
 PLOTTER->SetTitleOffset(1.3, "Y");

 TLegend* Le = new TLegend(0.60, 0.70, 0.85, 0.85);
 Le->SetTextSize(0.03);
 Le->AddEntry(htMON, "MON", "l");
 Le->AddEntry(htMONandTRIG, "TRIG && MON", "l");


 htMON->SetLineColor(kRed);
 htMON->SetLineStyle(2);
 
 PLOTTER->SetYTitle("#events");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(htMON->GetMaximum()*1.1);

 PLOTTER->DrawCopy();
 htMON->DrawCopy("SAMEH"); 
 htMONandTRIG->DrawCopy("SAMEH");
 Le->Draw();

 c1->SaveAs("v6_HT500andFat850_ZOOM_fat.png");



 PLOTTER->SetYTitle("HT500 && Fat850 / HT500");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);

 htMONandTRIG->Divide(htMONandTRIG, htMON, 1, 1, "B");
 PLOTTER->DrawCopy();

 TLine* L = new TLine (400, 1, 1400, 1);
 L->SetLineStyle(2);
 L->Draw();
 htMONandTRIG->Draw("SAMEH");
 
 c1->SaveAs("v6_Fat850vsHT500_fat.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 htMONandTRIG->Draw("SAMEH");
 c1->SaveAs("v6_Fat850vsHT500_ZOOM_fat.png");


}
