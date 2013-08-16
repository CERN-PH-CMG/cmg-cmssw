{


 TCanvas* MuonsElectrons = new TCanvas();
 MuonsElectrons->SetFillColor(10);
 MuonsElectrons->Divide(2,1);


 // TFile *_file0 = TFile::Open("out/res/4fb/Fat30/large_MuEFraction/histograms_data_HT_Run2011AB_160404_180252_Fat30_largeMuE_ak5_4974pbm1.root");
 TFile *_file0 = TFile::Open("out/res/4fb/Fat30/largeSumEt_over_Et/histograms_data_HT_Run2011AB_160404_180252_Fat30_largeMet_ak5_4974pbm1.root");
 TH1D* DijetMassMuE = (TH1D* ) _file0->Get("h_DijetMass_data_fat;1");

 TFile *_file1 = TFile::Open("out/res/4fb/Fat30/histograms_data_HT_Run2011AB_160404_180252_Fat30_ak5_4974pbm1.root");
 TH1D* DijetMass = (TH1D* ) _file1->Get("h_DijetMass_data_fat;1");


 DijetMassMuE->SetLineColor(kYellow);
 DijetMassMuE->SetFillColor(kYellow);

 DijetMass->SetLineColor(kGreen);
 DijetMass->SetFillColor(kGreen);

 DijetMass->Add(DijetMassMuE, -1);


 THStack* stacks = new THStack("DijetMass", "DijetMass");
// stacks->Add( QCD_Z2_120to170);
 stacks->Add( DijetMassMuE);
 stacks->Add( DijetMass);

 stacks->SetMinimum(0.9);
 stacks->SetMaximum(3e5);

 // stacks->SetTitle("Fraction of events with Muons or Electrons; M(12, Wide); #Evt.;");
 stacks->SetTitle("Fraction of events with MET/SumEt; M(12, Wide); #Evt.;");

 MuonsElectrons->cd(1);
 stacks->DrawClone();

 gPad->SetLogy(1);

 MuonsElectrons->cd(2);

 DijetMass->Add(DijetMassMuE, 1);
 DijetMassMuE->Divide(DijetMass);

 DijetMassMuE->Scale(100.);

 DijetMassMuE->SetMaximum(1);
 DijetMassMuE->SetMinimum(0);

 
 // DijetMassMuE->SetTitle("Fraction of events with Muons or Electrons; M(12, Wide); %;");
 DijetMassMuE->SetTitle("Fraction of events with MET/SumEt > 0.35; M(12, Wide); %;");
 DijetMassMuE->SetTitleOffset(0.7, "X");
 DijetMassMuE->SetTitleOffset(0.7, "Y");

 DijetMassMuE->SetFillStyle(0);
 DijetMassMuE->SetLineColor(1);

 TH1D* Error = new TH1D(*DijetMass);

 DijetMass->Sumw2();
 
 for (int i = 0; i < DijetMass->GetNbinsX(); i++){
   
   double dError = DijetMass->GetBinError(i)/(DijetMass->GetBinContent(i)+1e-10)*100;

   if (dError < 1e-10) continue;

   cout <<"Error" << dError << endl;

   Error->SetBinContent(i, dError);

 }

 TLegend *leg = new TLegend(0.60,0.77,0.98,0.90);
 // leg->SetTextSize(0.03146853);
 leg->SetLineColor(1);
 leg->SetLineStyle(1);
 leg->SetLineWidth(1);
 leg->SetFillColor(0);
 // leg->AddEntry(DijetMassMuE,"Mu/e jets","PL"); 
 leg->AddEntry(DijetMassMuE,"LargeMET","PL"); 
 leg->AddEntry(Error,"Tot. Stat error","PL"); 


 Error->SetLineColor(kRed);
 Error->SetFillStyle(0);

 DijetMassMuE->DrawClone();
 Error->Draw("SAME");
 leg->Draw("same");


 // MuonsElectrons->SaveAs("DijetMass_MuEContribution.png");

 MuonsElectrons->SaveAs("DijetMass_LargeMet.png");

 

}

