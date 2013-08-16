{

  string sNum, sDen, sTitle, sOut;

    TFile *_file0 = TFile::Open("histograms_mc_ZprimeToBBbar_M-2000_ak5.root");

  for (int i =0; i < 3; i++){

    switch(i){
    case 0 : sNum = "h_MuptgenAndrec_vs_gen_pf;1"; sDen = "h_Muptgen_vs_gen_pf;1"; sTitle = "Effic. #mu ; pT(#mu} (GeV); Rec&&Gen/Gen"; sOut = "EfficMuonPt.png"; break;
    case 1 : sNum = "h_MuJetptgenAndrec_vs_gen_pf;1"; sDen = "h_MuJetptgen_vs_gen_pf;1"; sTitle = "Effic. #mu ; pT(jet} (GeV); Rec&&Gen/Gen"; sOut = "EfficMuonJetPt_MuPt10.png"; break;
    case 2 : sNum = "h_MuptgenAndrec_vs_gen_MaxPt_pf;1"; sDen = "h_Muptgen_vs_gen_MaxPt_pf;1"; sTitle = "Effic. at least 1 #mu/evt ; pT(jet 1} (GeV); Rec&&Gen/Gen"; sOut = "EfficEventMuonJetPt_MuPt10.png"; break;
    default: break;
    }
    
    
    TH1D* h_Num = _file0->Get(sNum.c_str());
    
    TH1D* h_Den = _file0->Get(sDen.c_str());
    
    TCanvas* C = new TCanvas();


    h_Num->Sumw2();
    h_Den->Sumw2();

    h_Num->Divide(h_Num, h_Den, 1, 1, "B");

    h_Num->SetTitleOffset(0.7, "Y");
    h_Num->SetTitleOffset(0.7, "X");
    h_Num->SetTitle(sTitle.c_str());

    h_Num->Draw("HE");

    C->SaveAs(sOut.c_str());
  
  }

  gStyle->SetPalette(1);

  for (int i =0; i < 2; i++){

    switch(i){
    case 0 : sNum = "h_Muphi_etagenAndrec_vs_gen_pf;1"; sDen = "h_Muphi_etagen_vs_gen_pf;1"; sTitle = "Effic. #mu ; #phi; #eta; Rec&&Gen/Gen"; sOut = "EfficMuonPhiEta.png"; break;
    case 1 : sNum = "h_MuptgenAndrec_vs_gen_MuR_pf;1"; sDen = "h_Muptgen_vs_gen_MuR_pf;1"; sTitle = "Effic. #mu ; pT(#mu) (GeV); #Delta R;  Rec&&Gen/Gen"; sOut = "EfficMuonPt_DeltaR.png"; break;
    case 2 : sNum = "h_Mupt_etagenAndrec_vs_gen_pf;1"; sDen = "h_Mupt_etagen_vs_gen_MuR_pf;1"; sTitle = "Effic. #mu ; pT(#mu) (GeV); #eta;  Rec&&Gen/Gen"; sOut = "EfficMuonPtEta.png"; break;
      //    case 2 : sNum = "h_MuptgenAndrec_vs_gen_MaxPt_pf;1"; sDen = "h_Muptgen_vs_gen_MaxPt_pf;1"; sTitle = "Effic. at least 1 #muon/evt ; pT(jet 1} (GeV); Rec&&Gen/Gen"; sOut = "EfficEventMuonJetPt_MuPt10.png"; break;
    default: break;
    }
    
    
    TH2D* h_Num2D = _file0->Get(sNum.c_str());
    
    TH2D* h_Den2D = _file0->Get(sDen.c_str());
    
    TCanvas* C = new TCanvas();


    h_Num2D->Sumw2();
    h_Den2D->Sumw2();



    h_Num2D->SetTitleOffset(0.7, "Y");
    h_Num2D->SetTitleOffset(0.7, "X");
    h_Num2D->SetTitle(sTitle.c_str());

    h_Num2D->Draw("COLZ");

    string sNOut("N_"); sNOut = sNOut + "" + sOut;
    

    C->SaveAs(sNOut.c_str());

    h_Num2D->Divide(h_Num2D, h_Den2D, 1, 1, "B");

    C->SaveAs(sOut.c_str());
  
  }


  
}
