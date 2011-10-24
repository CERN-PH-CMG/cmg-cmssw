{

  //  42                  43             44            45              46                  47
  //'HLT_HT400_v8', 'HLT_HT450_v8','HLT_HT500_v8','HLT_HT550_v8', 'HLT_HT600_v1', 'HLT_HT650_v1', 
  //  48                                             49                                 50                                   51
//'HLT_FatJetMass750_DR1p1_Deta2p0_v1', 'HLT_FatJetMass850_DR1p1_Deta2p0_v1', 'HLT_FatJetMass750_DR1p1_Deta2p0_v2', 'HLT_FatJetMass850_DR1p1_Deta2p0_v2'


 TCanvas* TURN_ON_CURVE = new TCanvas();

 TFile *_file0 = TFile::Open("out/res/histograms_data_HT_Run2011B_End_Fat_1778098_178421_DCSonly_short_ak7_ak7.root");
 // TFile *_file0 = TFile::Open("out/res/histograms_data_HT_Run2011A_End_Fat_500GeV_ak7.root");

 TH3F* DijetMass_Trigger_data_fat_Corr = (TH3F* ) _file0->Get("h_DijetMass_Trigger_data_Corr;1");
 TH3F* DijetMass_Trigger_data_fat_Corr_andHT650 = (TH3F* ) _file0->Get("h_DijetMass_Trigger_data_Corr_andHT650;1");
 TH2F* DijetMass_Trigger_data_fat = (TH2F* ) _file0->Get("h_DijetMass_Trigger_data;1");
 
 // TH1D* Fatv2andHt600 = DijetMass_Trigger_data_fat_Corr->ProjectionX("Fatv2andHt600", 51,51,45,45);
 //TH1D* Fatv1andHt600 = DijetMass_Trigger_data_fat_Corr->ProjectionX("Fatv1andHt600", 49,49,45,45);

 // TH1D* ht500and200 = (TH1D* ) _file0->Get("h_DijetMass_data_Trigger_HTFAT_fat");
 // TH1D* ht500and200 = (TH1D* ) _file0->Get("h_DijetMass_data_Trigger_HTFAT_fat");

 // TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 46,46,42,42);
 
 
 TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 47, 47, 43, 43);
 // ht500and200->Add(DijetMass_Trigger_data_fat_Corr->ProjectionX("add", 51,51,43,43));
  //  ht500and200->Add(DijetMass_Trigger_data_fat_Corr->ProjectionX("add", 47,47,43,43));
 //ht500and200->Add(ht500and200, DijetMass_Trigger_data_fat_Corr_andHT650->ProjectionX("add", 51,51,43,43), 1, -1);


 // ht500and200->Add(DijetMass_Trigger_data_fat_Corr->ProjectionX("add1", 45,45,42,42));

 // ht500and200->Add(ht500and200, AK7v1andHt600, 1, -1);
 // ht500and200->Add(ht500and200, AK7v2andHt600, 1, -1);

 TH1D* ht200 = DijetMass_Trigger_data_fat->ProjectionX("ht200", 43,43);
 ht500and200->Sumw2();
 ht200->Sumw2();

 cout << "Monitor = " << ht200->Integral() << endl;

 TH1D* test = DijetMass_Trigger_data_fat->ProjectionY("test");

 for (int i = 1; i <= test->GetNbinsX(); i++){

   cout << "Trigger " << i << " integral = " << test->GetBinContent(i) << endl;

 }



 //ht500and200->Draw();
 DijetMass_Trigger_data_fat->Draw("LEGO");

 TLine* L = new TLine (400, 1, 1400, 1);
 L->SetLineStyle(2);

 TCanvas* c1 = new TCanvas("c1");
 c1->SetLeftMargin(0.15);

 TH1D* PLOTTER = new TH1D("PLOTTER", "HT650 trigger efficiency for Fat R=1.1; M_{12 AK7}; HT650 && HT450 / HT450", 1, 400, 1400);

 PLOTTER->SetStats(0);
 PLOTTER->SetTitleOffset(1.3, "Y");

 TLegend* Le = new TLegend(0.60, 0.70, 0.85, 0.85);
 Le->SetTextSize(0.03);
 Le->AddEntry(ht200, "HT450", "l");
 Le->AddEntry(ht500and200, "HT650 && HT450", "l");


 ht200->SetLineColor(kRed);
 ht200->SetLineStyle(2);
 
 PLOTTER->SetYTitle("#events");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(ht200->GetMaximum()*1.1);

 PLOTTER->DrawCopy();
 ht200->DrawCopy("SAMEH"); 
 ht500and200->DrawCopy("SAMEH");
 Le->Draw();

 c1->SaveAs("v6_HT450andHT650_ZOOM_ak7.png");



 PLOTTER->SetYTitle("HT450 && HT650 / HT450");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);

 ht500and200->Divide(ht500and200, ht200, 1, 1, "B");
 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 
 c1->SaveAs("v6_HT650vsHT450_ak7.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 c1->SaveAs("v6_HT650vsHT450_ZOOM_ak7.png");
 /*
 //------------------------------------------

 TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 18,18,13,13);
 TH1D* ht200 = DijetMass_Trigger_data_fat->ProjectionX("ht200", 13, 13);
 ht500and200->Sumw2();
 ht200->Sumw2();
 //ht500and200->Draw();
 DijetMass_Trigger_data_fat->Draw("LEGO");

 TH1D* PLOTTER = new TH1D("PLOTTER", "HT450 trigger efficiency for Fat R=1.1; M_{12 AK5}; HT450 && HT200 / HT200", 1, 400, 1400);

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
 Le->AddEntry(ht500and200, "HT200 && HT450", "l");
 Le->Draw();


 c1->SaveAs("HT450andHT200.png");

 PLOTTER->SetYTitle("HT450 && HT200 / HT200");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);


 ht500and200->Divide(ht500and200, ht200, 1, 1, "B");
 
 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 
 c1->SaveAs("HT450vsHT200.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 c1->SaveAs("HT450vsHT200_ZOOM.png");



 TH1D* ht500and200 = DijetMass_Trigger_data_fat_Corr->ProjectionX("ht500and200", 18,18,12,12);
 TH1D* ht200 = DijetMass_Trigger_data_fat->ProjectionX("ht200", 12, 12);
 ht500and200->Sumw2();
 ht200->Sumw2();
 //ht500and200->Draw();
 DijetMass_Trigger_data_fat->Draw("LEGO");

 TH1D* PLOTTER = new TH1D("PLOTTER", "HT450 trigger efficiency for Fat R=1.1; M_{12 AK5}; HT450 && HT150 / HT150", 1, 400, 1400);

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
 Le->AddEntry(ht500and200, "HT150 && HT450", "l");
 Le->Draw();


 c1->SaveAs("HT450andHT150.png");

 PLOTTER->SetYTitle("HT450 && HT150 / HT150");
 PLOTTER->SetMinimum(0);
 PLOTTER->SetMaximum(1.1);


 ht500and200->Divide(ht500and200, ht200, 1, 1, "B");
 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 
 c1->SaveAs("HT450vsHT150.png");


 PLOTTER->SetMinimum(0.9);
 PLOTTER->SetMaximum(1.1);

 PLOTTER->DrawCopy();
 L->Draw();
 ht500and200->Draw("SAMEH");
 c1->SaveAs("HT450vsHT150_ZOOM.png");
 */

}
