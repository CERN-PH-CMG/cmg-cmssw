{


  TCanvas* C = new TCanvas("C", "canvas", 1200., 400.);
  C->SetFillColor(10);
  C->Divide(3,1);


  vector<string> vNames;
  TObjArray* histos = new TObjArray();
  vector<string> sHeader; 
  sHeader.push_back("CMS Simulation: MBH=MD");
  sHeader.push_back("CMS Simulation: MD=2"); 
  sHeader.push_back("CMS Simulation: MD=4");

  for (int j = 0; j < 3; j++){
    histos->Clear();
    switch(j){
    case 0:
      vNames.clear();
      for (int k = 0; k < 3; k++){
	int iMD = 2*k+1;
	vNames.push_back(Form("histograms_qbh5%d%d_ak5.root",iMD,iMD));
	vNames.push_back(Form("histograms_qbh10%d%d_ak5.root",iMD,iMD));
      }
      break;
    case 1: 
      vNames.clear();
      vNames.push_back(Form("histograms_qbh5230_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh10230_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh5245_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh10245_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh5260_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh10260_ak5.root",iMD,iMD));
      break;
    case 2: 
      vNames.clear();
      vNames.push_back(Form("histograms_qbh5445_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh10445_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh5450_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh10450_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh5460_ak5.root",iMD,iMD));
      vNames.push_back(Form("histograms_qbh10460_ak5.root",iMD,iMD));
      break;
    default: break;
    }
    
    for (int i = 0; i < 6; i++){

      TFile *file = TFile::Open(vNames[i].c_str());
	  
      TH1D* hMass;

      if (vNames[i].find("qbh5") != string::npos) {
	hMass =  (TH1D*) file->Get("h_DijetMass_data_fat;1");
	hMass->SetLineColor(i/2+1);
	hMass->SetLineStyle(1);

      }
      else {
	hMass = (TH1D*) file->Get("h_DijetMass_data_fat;1");
	hMass->SetLineColor(i/2+1);
	hMass->SetLineStyle(2);
      }
	  
      double integral = hMass->Integral();
      hMass->Scale(1./integral);

      histos->Add(hMass);

    }
	    
    C->cd(j+1);

    TH1F *vFrame2 = gPad->DrawFrame(0.0,0.0,7000.,0.40);
    vFrame2->SetTitle(sHeader[j].c_str());
    vFrame2->SetTitleSize(0.2);
    vFrame2->SetXTitle("Reconstructed Resonance mass (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);
	
    vFrame2->SetYTitle("Probability");
	
    vFrame2->DrawClone();

    for (int i = 0; i < 6; i++){
      ((TH1D*) histos->At(i))->DrawClone("SAME");
    }



    if (j == 0) {
      TLegend* Legend = new TLegend(0.13, 0.55, 0.60, 0.88,NULL,"brNDC");
      Legend->AddEntry((TH1D*) histos->At(0), "MD=MBH=1.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(1), "MD=MBH=1.0 TeV, n=6", "l");
      Legend->AddEntry((TH1D*) histos->At(2), "MD=MBH=3.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(3), "MD=MBH=3.0 TeV, n=6", "l");
      Legend->AddEntry((TH1D*) histos->At(4), "MD=MBH=5.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(5), "MD=MBH=5.0 TeV: n=6", "l");
    }  

    if (j == 1) {
      TLegend* Legend = new TLegend(0.13, 0.55, 0.60, 0.88,NULL,"brNDC");
      Legend->AddEntry((TH1D*) histos->At(0), "MBH=3.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(1), "MBH=3.0 TeV, n=6", "l");
      Legend->AddEntry((TH1D*) histos->At(2), "MBH=4.5 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(3), "MBH=4.5 TeV, n=6", "l");
      Legend->AddEntry((TH1D*) histos->At(4), "MBH=6.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(5), "MBH=6.0 TeV: n=6", "l");

    }  
    if (j == 2) {
      TLegend* Legend = new TLegend(0.13, 0.55, 0.60, 0.88,NULL,"brNDC");
      Legend->AddEntry((TH1D*) histos->At(0), "MBH=4.5 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(1), "MBH=4.5 TeV, n=6", "l");
      Legend->AddEntry((TH1D*) histos->At(2), "MBH=5.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(3), "MBH=5.0 TeV, n=6", "l");
      Legend->AddEntry((TH1D*) histos->At(4), "MBH=6.0 TeV, n=1", "l");
      Legend->AddEntry((TH1D*) histos->At(5), "MBH=6.0 TeV: n=6", "l");
    } 

    Legend->SetFillColor(0);
    Legend->SetFillStyle(0);
    //    Legend->SetLineColor(0);
    Legend->DrawClone("sames");

  }    


  C->SaveAs("histograms_Fat30_mc_QBH_TuneD6T.png"); 

/*
  for (int j = 0; j < 3; j++){
    histos->Clear();
    switch(j){
    case 0:
      vNames.clear();
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-500_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-700_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-3500_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak5QQtoQQ.root"); break; 
    case 1: 
      vNames.clear();
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-500_TuneD6T_ak5.root");
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-700_TuneD6T_ak5.root"); 
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-1200_TuneD6T_ak5.root");
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-2000_TuneD6T_ak5.root"); 
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-3500_TuneD6T_ak5.root");
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-4000_TuneD6T_ak5.root"); break;
    case 2: 
      vNames.clear();
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-500_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-700_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-3500_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak5GGtoGG.root"); break; 
    default: break;
    }
    
    for (int i = 0; i < 6; i++){

      TFile *file = TFile::Open(vNames[i].c_str());
	  
      TH1D* hMass;

      
      hMass = (TH1D*) file->Get("h_DijetMass_data_fat;");
      if (i != 4 ) hMass->SetLineColor(i+1);
      else hMass->SetLineColor(7);
      
	  
      double integral = hMass->Integral();
      hMass->Scale(1./integral);

      histos->Add(hMass);

    }
	    
    C->cd(j+1);

    TH1F *vFrame2 = gPad->DrawFrame(0.0,0.0,5000.,0.30);
    vFrame2->SetTitle(sHeader[j].c_str());
    vFrame2->SetTitleSize(0.2);
    if (j==0) vFrame2->SetXTitle("Quark-Quark Resonance mass (GeV)");
    if (j==1) vFrame2->SetXTitle("Quark-Gluon Resonance mass (GeV)");
    if (j==2) vFrame2->SetXTitle("Gluon-Gluon Resonance mass (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);
	
    vFrame2->SetYTitle("Probability");
	
    vFrame2->DrawClone();

    for (int i = 0; i < 6; i++){
      ((TH1D*) histos->At(i))->DrawClone("SAME");
    }



    if (j == 2) {
      TLegend* Legend = new TLegend(0.40, 0.55, 0.87, 0.88);
      Legend->AddEntry((TH1D*) histos->At(0), "0.5 TeV", "l");
      Legend->AddEntry((TH1D*) histos->At(1), "0.7 TeV", "l");
      Legend->AddEntry((TH1D*) histos->At(2), "1.2 TeV", "l");
      Legend->AddEntry((TH1D*) histos->At(3), "2.0 TeV", "l");
      Legend->AddEntry((TH1D*) histos->At(4), "3.5 TeV", "l");
      Legend->AddEntry((TH1D*) histos->At(5), "4.0 TeV", "l");
      Legend->Draw();
    }  

    


  }    


  C->SaveAs("histograms_Fat30_mc_Fat30_TuneD6T.png"); 


*/








}
