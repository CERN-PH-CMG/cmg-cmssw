{


  TCanvas* C = new TCanvas("C", "canvas", 1200., 400.);
  C->Divide(3,1);


  vector<string> vNames;
  TObjArray* histos = new TObjArray();
  vector<string> sHeader; 
  sHeader.push_back("qq->RS->qq");
  sHeader.push_back("qg->Q*->qg"); 
  sHeader.push_back("gg->RS->gg");

  for (int j = 0; j < 3; j++){
    histos->Clear();
    switch(j){
    case 0:
      vNames.clear();
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak7QQtoQQ.root"); 
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak7QQtoQQ.root"); 
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak5QQtoQQ.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak7QQtoQQ.root"); break;
    case 1: 
      vNames.clear();
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-1200_TuneD6T_ak5.root");
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-1200_TuneD6T_ak7.root"); 
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-2000_TuneD6T_ak5.root");
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-2000_TuneD6T_ak7.root"); 
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-4000_TuneD6T_ak5.root");
      vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-4000_TuneD6T_ak7.root"); break;
    case 2: 
      vNames.clear();
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak7GGtoGG.root"); 
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak7GGtoGG.root"); 
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak5GGtoGG.root");
      vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak7GGtoGG.root"); break;
    default: break;
    }
    
    for (int i = 0; i < 6; i++){

      TFile *file = TFile::Open(vNames[i].c_str());
	  
      TH1D* hMass;

      if (vNames[i].find("ak7") != string::npos) {
	hMass =  (TH1D*) file->Get("h_DijetMass_data_fat;");
	hMass->SetLineColor(i/2+1);

      }
      else {
	hMass = (TH1D*) file->Get("h_DijetMass_data_pf;");
	hMass->SetLineColor(i/2+1);
	hMass->SetLineStyle(2);
      }
	  
      double integral = hMass->Integral();
      hMass->Scale(1./integral);

      histos->Add(hMass);

    }
	    
    C->cd(j+1);

    TH1F *vFrame2 = gPad->DrawFrame(0.0,0.0,5000.,0.30);
    vFrame2->SetTitle(sHeader[j].c_str());
    vFrame2->SetTitleSize(0.2);
    vFrame2->SetXTitle("Reconstructed Resonance mass (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);
	
    vFrame2->SetYTitle("Fraction");
	
    vFrame2->DrawClone();

    for (int i = 0; i < 6; i++){
      ((TH1D*) histos->At(i))->DrawClone("SAME");
    }



    if (j == 2) {
      TLegend* Legend = new TLegend(0.40, 0.55, 0.87, 0.88);
      Legend->AddEntry((TH1D*) histos->At(0), "1.2 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(1), "1.2 TeV: AK7 jets", "l");
      Legend->AddEntry((TH1D*) histos->At(2), "2.0 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(3), "2.0 TeV: AK7 jets", "l");
      Legend->AddEntry((TH1D*) histos->At(4), "4.0 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(5), "4.0 TeV: AK7 jets", "l");
      Legend->Draw();
    }  

  }    


  C->SaveAs("histograms_Fat30_mc_Fat30vsAK7_TuneD6T.png"); 


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
      hMass->SetLineColor(i+1);

      
	  
      double integral = hMass->Integral();
      hMass->Scale(1./integral);

      histos->Add(hMass);

    }
	    
    C->cd(j+1);

    TH1F *vFrame2 = gPad->DrawFrame(0.0,0.0,5000.,0.30);
    vFrame2->SetTitle(sHeader[j].c_str());
    vFrame2->SetTitleSize(0.2);
    vFrame2->SetXTitle("Reconstructed Resonance mass (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);
	
    vFrame2->SetYTitle("Fraction");
	
    vFrame2->DrawClone();

    for (int i = 0; i < 6; i++){
      ((TH1D*) histos->At(i))->DrawClone("SAME");
    }



    if (j == 2) {
      TLegend* Legend = new TLegend(0.40, 0.55, 0.87, 0.88);
      Legend->AddEntry((TH1D*) histos->At(0), "0.5 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(1), "0.7 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(2), "1.2 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(3), "2.0 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(4), "3.5 TeV: AK5 Wide jets", "l");
      Legend->AddEntry((TH1D*) histos->At(5), "4.0 TeV: AK5 Wide jets", "l");
      Legend->Draw();
    }  

    


  }    


  C->SaveAs("histograms_Fat30_mc_Fat30_TuneD6T.png"); 











}
