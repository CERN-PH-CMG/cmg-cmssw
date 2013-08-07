{


  TCanvas* C = new TCanvas("C", "canvas", 1200., 400.);
  C->SetFillColor(10);
  C->Divide(3,1);



   vector<string> vNames;
   TObjArray* histos = new TObjArray();
   vector<string> sHeader; 
   sHeader.push_back("1 TeV");
   sHeader.push_back("2 TeV"); 
   sHeader.push_back("3 TeV");

//   for (int j = 0; j < 3; j++){
//     histos->Clear();
//     switch(j){
//     case 0:
//       vNames.clear();
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak5QQtoQQ.root");
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak7QQtoQQ.root"); 
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak5QQtoQQ.root");
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak7QQtoQQ.root"); 
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak5QQtoQQ.root");
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak7QQtoQQ.root"); break;
//     case 1: 
//       vNames.clear();
//       vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-1200_TuneD6T_ak5.root");
//       vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-1200_TuneD6T_ak7.root"); 
//       vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-2000_TuneD6T_ak5.root");
//       vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-2000_TuneD6T_ak7.root"); 
//       vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-4000_TuneD6T_ak5.root");
//       vNames.push_back("histograms_Fat30_mc_QstarToJJ_M-4000_TuneD6T_ak7.root"); break;
//     case 2: 
//       vNames.clear();
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak5GGtoGG.root");
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-1200_TuneD6T_ak7GGtoGG.root"); 
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak5GGtoGG.root");
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-2000_TuneD6T_ak7GGtoGG.root"); 
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak5GGtoGG.root");
//       vNames.push_back("histograms_Fat30_mc_RSGravitonToJJ_M-4000_TuneD6T_ak7GGtoGG.root"); break;
//     default: break;
//     }
    
//     for (int i = 0; i < 6; i++){

//       TFile *file = TFile::Open(vNames[i].c_str());
	  
//       TH1D* hMass;

//       if (vNames[i].find("ak5") != string::npos) {
// 	hMass =  (TH1D*) file->Get("h_DijetMass_data_fat;");
// 	hMass->SetLineColor(i/2+1);

//       }
//       else {
// 	hMass = (TH1D*) file->Get("h_DijetMass_data_pf;");
// 	hMass->SetLineColor(i/2+1);
// 	hMass->SetLineStyle(2);
//       }
	  
//       double integral = hMass->Integral();
//       hMass->Scale(1./integral);

//       histos->Add(hMass);

//     }
	    
//     C->cd(j+1);

//     TH1F *vFrame2 = gPad->DrawFrame(0.0,0.0,5000.,0.30);
//     vFrame2->SetTitle(sHeader[j].c_str());
//     vFrame2->SetTitleSize(0.2);
//     vFrame2->SetXTitle("Reconstructed Resonance mass (GeV)");
//     vFrame2->GetYaxis()->SetLabelSize(0.03);
//     vFrame2->GetYaxis()->SetTitleSize(0.04);
//     vFrame2->GetYaxis()->SetTitleOffset(1.2);
//     vFrame2->GetXaxis()->SetLabelSize(0.03);
//     vFrame2->GetXaxis()->SetTitleSize(0.04);
//     vFrame2->GetXaxis()->SetTitleOffset(0.95);
	
//     vFrame2->SetYTitle("Probability");
	
//     vFrame2->DrawClone();

//     for (int i = 0; i < 6; i++){
//       ((TH1D*) histos->At(i))->DrawClone("SAME");
//     }



//     if (j == 2) {
//       TLegend* Legend = new TLegend(0.40, 0.55, 0.87, 0.88);
//       Legend->AddEntry((TH1D*) histos->At(0), "1.2 TeV: AK5 Wide jets", "l");
//       Legend->AddEntry((TH1D*) histos->At(1), "1.2 TeV: AK7 jets", "l");
//       Legend->AddEntry((TH1D*) histos->At(2), "2.0 TeV: AK5 Wide jets", "l");
//       Legend->AddEntry((TH1D*) histos->At(3), "2.0 TeV: AK7 jets", "l");
//       Legend->AddEntry((TH1D*) histos->At(4), "4.0 TeV: AK5 Wide jets", "l");
//       Legend->AddEntry((TH1D*) histos->At(5), "4.0 TeV: AK7 jets", "l");

//       Legend->Draw();
//     }  

//   }    


//   C->SaveAs("histograms_Fat30_mc_Fat30vsAK7_TuneD6T.png"); 


   for (int k = 0; k < 2; k++){
     for (int j = 1; j < 4; j++){
       histos->Clear();
       
       switch(k){
       case 0:
	 vNames.clear();
	 vNames.push_back(Form("histograms_RSGravitonToGG_kMpl_0p1_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 vNames.push_back(Form("histograms_RSGravitonToGG_kMpl_0p4_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 //      vNames.push_back(Form("histograms_RSGravitonToGG_kMpl_0p4_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 vNames.push_back(Form("histograms_RSGravitonToGG_kMpl_0p7_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 //      vNames.push_back(Form("histograms_RSGravitonToGG_kMpl_0p7_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 vNames.push_back(Form("histograms_RSGravitonToGG_kMpl_1p0_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 break;
       case 1:
	 vNames.clear();
	 vNames.push_back(Form("histograms_RSGravitonToQQ_kMpl_0p1_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 vNames.push_back(Form("histograms_RSGravitonToQQ_kMpl_0p4_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 //      vNames.push_back(Form("histograms_RSGravitonToQQ_kMpl_0p4_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 vNames.push_back(Form("histograms_RSGravitonToQQ_kMpl_0p7_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 //      vNames.push_back(Form("histograms_RSGravitonToQQ_kMpl_0p7_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 vNames.push_back(Form("histograms_RSGravitonToQQ_kMpl_1p0_M_%d000_TuneZ2_7TeV_pythia6_FASTSIM_ak5.root",j));
	 break;
       default:
	 break;
       }
       
    
       for (int i = 0; i < 4; i++){
	 
	 cout << "i = " << i << endl;
	 
	 TFile *file = TFile::Open(vNames[i].c_str());
	 
	 TH1D* hMass;
	 
	 
	 hMass = (TH1D*) file->Get("h_DijetMass_data_fat;");
	 if (!hMass) {
	   histos->Add(hMass);continue;
	 }
	 if (i != 4 ) hMass->SetLineColor(i+1);
	 else hMass->SetLineColor(7);
	 
	 
	 double integral = hMass->Integral();
	 hMass->Scale(1./integral);
	 
	 histos->Add(hMass);

       }
    
       C->cd(j);
    
       TH1F *vFrame2 = gPad->DrawFrame(0.0,0.0,(j+1)*1000.,0.30);
       vFrame2->SetTitle(sHeader[j-1].c_str());
       vFrame2->SetTitleSize(0.2);
       if (k==0) vFrame2->SetXTitle("Quark-Quark Resonance mass (GeV)");
       if (k==1) vFrame2->SetXTitle("Gluon-Gluon Resonance mass (GeV)");
       vFrame2->GetYaxis()->SetLabelSize(0.03);
       vFrame2->GetYaxis()->SetTitleSize(0.04);
       vFrame2->GetYaxis()->SetTitleOffset(1.2);
       vFrame2->GetXaxis()->SetLabelSize(0.03);
       vFrame2->GetXaxis()->SetTitleSize(0.04);
       vFrame2->GetXaxis()->SetTitleOffset(0.95);
       
       vFrame2->SetYTitle("Probability");
       
       vFrame2->DrawClone();
      
       for (int i = 0; i < 4; i++){
	 if (histos->At(i)) ((TH1D*) histos->At(i))->DrawClone("SAME");
       }


    
       if (j == 3) {
	 TLegend* Legend = new TLegend(0.20, 0.55, 0.6, 0.88,NULL,"brNDC");
	 Legend->AddEntry((TH1D*) histos->At(0), "k = 0.1", "l");
	 Legend->AddEntry((TH1D*) histos->At(1), "k = 0.4", "l");
	 Legend->AddEntry((TH1D*) histos->At(2), "k = 0.7", "l");
	 Legend->AddEntry((TH1D*) histos->At(3), "k = 1.0", "l");
	 Legend->SetFillColor(0);
	 Legend->SetFillStyle(0);
       Legend->Draw("sames");
       }  
        
       
     }

     if (k == 0) C->SaveAs("histograms_Fat30_mc_TuneD6T_RSGtoGG_2D.png"); 
     if (k == 1) C->SaveAs("histograms_Fat30_mc_TuneD6T_RSGtoQQ_2D.png"); 

   }

}
