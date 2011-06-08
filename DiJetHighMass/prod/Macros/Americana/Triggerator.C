{


 TCanvas* TURN_ON_CURVE = new TCanvas();

 TFile *_file0 = TFile::Open("out/res/FullSample/histograms_data_HT_All.root");
 TH3F* DijetMass_Trigger_data_fat_Corr = (TH3F* ) _file0->Get("h_DijetMass_Trigger_data_fat_Corr;1");
 TH2F* DijetMass_Trigger_data_fat = (TH2F* ) _file0->Get("h_DijetMass_Trigger_data_fat;1");
 
 TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 28,28,24,24);
 TH1D* ht200 = DijetMass_Trigger_data_fat->ProjectionX("ht200", 24,24);
 ht500and200->Sumw2();
 ht200->Sumw2();

 cout << "Monitor = " << ht200->Integral() << endl;



 //ht500and200->Draw();
 DijetMass_Trigger_data_fat->Draw("LEGO");

 TLine* L = new TLine (400, 1, 1400, 1);
 L->SetLineStyle(2);

 TCanvas* c1 = new TCanvas("c1");
 c1->SetLeftMargin(0.15);

 TH1D* PLOTTER = new TH1D("PLOTTER", "HT550 trigger efficiency for Fat R=1.1; M_{12 Fat}; HT550 && HT350 / HT350", 1, 400, 1400);

 PLOTTER->SetStats(0);
 PLOTTER->SetTitleOffset(1.3, "Y");

 TLegend* Le = new TLegend(0.60, 0.70, 0.85, 0.85);
 Le->SetTextSize(0.03);
 Le->AddEntry(ht200, "HT350", "l");
 Le->AddEntry(ht500and200, "HT350 && HT550", "l");


 ht200->SetLineColor(kRed);
 ht200->SetLineStyle(2);
 
 PLOTTER->SetYTitle("#events");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(ht200->GetMaximum()*1.1);

 PLOTTER->DrawCopy();
 ht200->DrawCopy("SAMEH"); 
 ht500and200->DrawCopy("SAMEH");
 Le->Draw();

 c1->SaveAs("HT550andHT350.png");


 PLOTTER->SetYTitle("HT550 && HT350 / HT350");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);

 ht500and200->Divide(ht500and200, ht200, 1, 1, "B");
 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 
 c1->SaveAs("HT550vsHT350.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 c1->SaveAs("HT550vsHT350_ZOOM.png");
 /*
 //------------------------------------------

 TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 18,18,13,13);
 TH1D* ht200 = DijetMass_Trigger_data_fat->ProjectionX("ht200", 13, 13);
 ht500and200->Sumw2();
 ht200->Sumw2();
 //ht500and200->Draw();
 DijetMass_Trigger_data_fat->Draw("LEGO");

 TH1D* PLOTTER = new TH1D("PLOTTER", "HT500 trigger efficiency for Fat R=1.1; M_{12 Fat}; HT500 && HT200 / HT200", 1, 400, 1400);

 PLOTTER->SetStats(0);

 ht200->SetLineColor(kRed);
 ht200->SetLineStyle(2);
 
 PLOTTER->SetYTitle("#events");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(ht200->GetMaximum()*1.1);

 PLOTTER->DrawCopy();
 ht200->DrawCopy("SAMEH"); 
 ht500and200->DrawCopy("SAMEH");

 Le->Clear();
 Le->AddEntry(ht200, "HT200", "l");
 Le->AddEntry(ht500and200, "HT200 && HT500", "l");
 Le->Draw();


 c1->SaveAs("HT500andHT200.png");

 PLOTTER->SetYTitle("HT500 && HT200 / HT200");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);


 ht500and200->Divide(ht500and200, ht200, 1, 1, "B");
 
 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 
 c1->SaveAs("HT500vsHT200.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 c1->SaveAs("HT500vsHT200_ZOOM.png");



 TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 18,18,12,12);
 TH1D* ht200 = DijetMass_Trigger_data_fat->ProjectionX("ht200", 12, 12);
 ht500and200->Sumw2();
 ht200->Sumw2();
 //ht500and200->Draw();
 DijetMass_Trigger_data_fat->Draw("LEGO");

 TH1D* PLOTTER = new TH1D("PLOTTER", "HT500 trigger efficiency for Fat R=1.1; M_{12 Fat}; HT500 && HT150 / HT150", 1, 400, 1400);

 PLOTTER->SetStats(0);

 ht200->SetLineColor(kRed);
 ht200->SetLineStyle(2);
 

 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(ht200->GetMaximum()*1.1);

 PLOTTER->SetYTitle("#events");

 PLOTTER->DrawCopy();
 ht200->DrawCopy("SAMEH"); 
 ht500and200->DrawCopy("SAMEH");

 Le->Clear();
 Le->AddEntry(ht200, "HT150", "l");
 Le->AddEntry(ht500and200, "HT150 && HT500", "l");
 Le->Draw();


 c1->SaveAs("HT500andHT150.png");

 PLOTTER->SetYTitle("HT500 && HT150 / HT150");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);


 ht500and200->Divide(ht500and200, ht200, 1, 1, "B");
 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 
 c1->SaveAs("HT500vsHT150.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 c1->SaveAs("HT500vsHT150_ZOOM.png");
 */

}
