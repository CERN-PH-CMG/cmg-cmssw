void R_WdivZ(TString folder=""){

  int WMassCentral = 80385;
  int WMassStep = 15;
  static const int WMassNSteps = 60;
  static const int etaMuonNSteps = 5;
  double etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  static const int CutSteps = 8;

  // TH1D hR_WdivZ[etaMuonNSteps][2*WMassNSteps+1];
  TH1D hR_WdivZ_WlikePos[CutSteps][etaMuonNSteps][2*WMassNSteps+1];
  TCanvas *cR_WdivZ_WlikePos[CutSteps][etaMuonNSteps][2*WMassNSteps+1];
  TCanvas *cR_WdivZ_WlikePos_all[etaMuonNSteps][2*WMassNSteps+1];

  TString Wana_str[CutSteps]    ={"hWPos_PtScaled_1_Gen","hWPos_PtScaled_2_ZGenMassCut","hWPos_PtScaled_3_Mu1RecoCut","hWPos_PtScaled_4_Mu2GenCut",
                           "hWPos_PtScaled_5_RecoCut","hWPos_PtScaled_6_METCut","hWPos_PtScaled_7_RecoilCut","hWPos_PtScaled_8_JetCut"};
  TString Zana_str[CutSteps]    ={"hWlikePos_PtScaled_1_Gen","hWlikePos_PtScaled_2_ZGenMassCut","hWlikePos_PtScaled_3_Mu1RecoCut","hWlikePos_PtScaled_4_Mu2GenCut",
                           "hWlikePos_PtScaled_5_RecoCut","hWlikePos_PtScaled_6_METCut","hWlikePos_PtScaled_7_RecoilCut","hWlikePos_PtScaled_8_JetCut"};
  TString hR_WdivZana_str[CutSteps]={"hR_WdivZ_WlikePos_1_Gen","hR_WdivZ_WlikePos_2_ZGenMassCut","hR_WdivZ_WlikePos_3_Mu1RecoCut","hR_WdivZ_WlikePos_4_Mu2GenCut",
                           "hR_WdivZ_WlikePos_5_RecoCut","hR_WdivZ_WlikePos_6_METCut","hR_WdivZ_WlikePos_7_RecoilCut","hR_WdivZ_WlikePos_8_JetCut"};
  TString cR_WdivZana_str[CutSteps]={"cR_WdivZ_WlikePos_1_Gen","cR_WdivZ_WlikePos_2_ZGenMassCut","cR_WdivZ_WlikePos_3_Mu1RecoCut","cR_WdivZ_WlikePos_4_Mu2GenCut",
                           "cR_WdivZ_WlikePos_5_RecoCut","cR_WdivZ_WlikePos_6_METCut","cR_WdivZ_WlikePos_7_RecoilCut","cR_WdivZ_WlikePos_8_JetCut"};

  TFile *fout=new TFile("R_WdivZ_OnMC.root","RECREATE");
  TFile *Wana=new TFile(Form("%s/WanalysisOnMC.root",folder.Data()));
  TFile *Zana=new TFile(Form("%s/ZanalysisOnMC.root",folder.Data()));

  for(int i=0; i<etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    // for(int j=0; j<2*WMassNSteps+1; j++){
    for(int j=60; j<61; j++){
      int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;

      cR_WdivZ_WlikePos_all[i][j]=new TCanvas(Form("cR_WdivZ_WlikePos_all_eta%s_%d",eta_str.Data(),jWmass));
      TLegend *leg = new TLegend(0.2,0.6,0.5,0.9,NULL,"brNDC");
      leg->SetBorderSize(1);
      leg->SetTextFont(62);
      leg->SetLineColor(1);
      leg->SetLineStyle(1);
      leg->SetLineWidth(1);
      leg->SetFillColor(0);
      leg->SetFillStyle(1001);

      if((j+1)%50==1) cout << "etaMuonNStep= " << (i+1) << "/"<< etaMuonNSteps << " WMassNStep= " << (j+1) << "/"<< (2*WMassNSteps+1) << endl;
      
      for(int kcut=0; kcut<CutSteps; kcut++){
        // cout << "etaMuonNStep= " << i << "/"<< etaMuonNSteps << " WMassNStep= " << j << "/"<< (2*WMassNSteps+1) << " CutSteps= " << kcut << "/"<< CutSteps << endl;
    
        TH1D*hWPos_PtScaled=(TH1D*)Wana->Get(Form("%s_eta%s_%d",Wana_str[kcut].Data(),eta_str.Data(),jWmass));
        // hWPos_PtScaled->Scale(1/hWPos_PtScaled->Integral());
        hWPos_PtScaled->Scale(1/hWPos_PtScaled->GetBinContent(hWPos_PtScaled->GetXaxis()->FindBin(1)));
        TH1D*hWlikePos_PtScaled=(TH1D*)Zana->Get(Form("%s_eta%s_%d",Zana_str[kcut].Data(),eta_str.Data(),jWmass));
        // hWlikePos_PtScaled->Scale(1/hWlikePos_PtScaled->Integral());
        hWlikePos_PtScaled->Scale(1/hWlikePos_PtScaled->GetBinContent(hWlikePos_PtScaled->GetXaxis()->FindBin(1)));
        hR_WdivZ_WlikePos[kcut][i][j]=(*hWPos_PtScaled)/(*hWlikePos_PtScaled);
        hR_WdivZ_WlikePos[kcut][i][j].SetName(Form("%s_eta%s_%d",hR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
        hR_WdivZ_WlikePos[kcut][i][j].SetTitle(Form("%s_eta%s_%d",hR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
        cR_WdivZ_WlikePos_all[i][j]->cd();
        // int normfactor_bin = hR_WdivZ_WlikePos[kcut][i][j].GetXaxis()->FindBin(1);
        // double normfactor = hR_WdivZ_WlikePos[kcut][i][j].GetXaxis()->GetBinContent(normfactor_bin);
        // hR_WdivZ_WlikePos[kcut][i][j]->Scale(1/normfactor);
        hR_WdivZ_WlikePos[kcut][i][j].SetLineColor(kcut+1);
        hR_WdivZ_WlikePos[kcut][i][j].SetMarkerColor(kcut+1);
        hR_WdivZ_WlikePos[kcut][i][j].SetMarkerStyle(kcut+19);
        hR_WdivZ_WlikePos[kcut][i][j].SetMarkerSize(0.5);
        hR_WdivZ_WlikePos[kcut][i][j].GetXaxis()->SetRangeUser(0.7,1.6);
        hR_WdivZ_WlikePos[kcut][i][j].GetYaxis()->SetRangeUser(0.6,1.5);
        hR_WdivZ_WlikePos[kcut][i][j].DrawCopy(kcut==0?"":"same");
        TLegendEntry *entry=leg->AddEntry((TH1D*)&hR_WdivZ_WlikePos[kcut][i][j],Form("%s",hR_WdivZana_str[kcut].Data()),"p");
        
        cR_WdivZ_WlikePos[kcut][i][j]=new TCanvas(Form("%s_eta%s_%d",cR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
        cR_WdivZ_WlikePos[kcut][i][j]->cd();
        TH1D*hZ2=(TH1D*)hWlikePos_PtScaled->Clone(Form("%s2_eta%s_%d",Zana_str[kcut].Data(),eta_str.Data(),jWmass));
        hZ2->SetMarkerColor(2);
        hZ2->SetLineColor(2);
        hZ2->DrawNormalized("e");
        hWPos_PtScaled->DrawNormalized("same e");
      }    
      leg->Draw();

    }
  }
  
  fout->cd();
  cout << "Writing output file" << endl;
  for(int i=0; i<etaMuonNSteps; i++){
    // for(int j=0; j<2*WMassNSteps+1; j++){
    for(int j=60; j<61; j++){
      cR_WdivZ_WlikePos_all[i][j]->Write();
      // TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      // int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;
      // if(j==60)cR_WdivZ_WlikePos_all[i][j]->SaveAs(Form("cR_WdivZ_WlikePos_all_eta%s_%d.root",eta_str.Data(),jWmass));
      for(int kcut=0; kcut<CutSteps; kcut++){
        hR_WdivZ_WlikePos[kcut][i][j].Write();
        cR_WdivZ_WlikePos[kcut][i][j]->Write();
      }
    }
  }
  fout->Write();
  fout->Close();
  TFile*f2=new TFile("cR_WdivZ_WlikePos_all.root","RECREATE");
  f2->cd();
  for(int i=0; i<etaMuonNSteps; i++){
    // for(int j=0; j<2*WMassNSteps+1; j++){
    for(int j=60; j<61; j++){
      cR_WdivZ_WlikePos_all[i][j]->Write();
      TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;
      cR_WdivZ_WlikePos_all[i][j]->Write();
      // if(j==60)cR_WdivZ_WlikePos_all[i][j]->SaveAs(Form("cR_WdivZ_WlikePos_all_eta%s_%d.root",eta_str.Data(),jWmass));
      // for(int kcut=0; kcut<CutSteps; kcut++){
        // hR_WdivZ_WlikePos[kcut][i][j].Write();
        // cR_WdivZ_WlikePos[kcut][i][j]->Write();
      // }
    }
  }
  f2->Write();
  f2->Close();
  cout << "Output file closed" << endl;

  return;
  
  // TH1D hWtemplate[etaMuonNSteps][2*WMassNSteps+1];
  // // TCanvas *cWtemplate[2*WMassNSteps+1];

  // TFile *foutTemplates=new TFile("Wtemplates.root","RECREATE");
  // TFile *Zana=new TFile(Form("%s/ZanalysisOnMC.root",folder.Data()));
  // TFile *R_WdivZ=new TFile("R_WdivZ_OnMC.root");

  // for(int i=0; i<etaMuonNSteps; i++){
    // TString eta_str = Form("%.1f",etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    // for(int j=0; j<2*WMassNSteps+1; j++){
      // int jWmass = WMassCentral-(WMassNSteps-j)*WMassStep;
      // TH1D*hZ=(TH1D*)Zana->Get(Form("WfromZmu_pt_eta%s_%d",eta_str.Data(),jWmass));
      // hZ->Scale(1/hZ->Integral());
      // TH1D*hR=(TH1D*)R_WdivZ->Get(Form("hR_WdivZ_eta%s_%d",eta_str.Data(),jWmass));
      // hR->Scale(1/hR->Integral());
      // hWtemplate[i][j]=(*hZ)*(*hR);
      // // hWtemplate.Sumw2();
      // hWtemplate[i][j].SetName(Form("hWtemplates_eta%s_%d",eta_str.Data(),jWmass));
      // hWtemplate[i][j].SetTitle(Form("hWtemplates_eta%s_%d",eta_str.Data(),jWmass));
      // // cWtemplate[j]=new TCanvas(Form("cR_WdivZ_%d",jWmass));
      // // TH1D*hZ2=(TH1D*)hZ->Clone(Form("WfromZmu2_pt_%d",jWmass));
      // // hZ2->SetMarkerColor(2);
      // // hZ2->SetLineColor(2);
      // // hZ2->DrawNormalized("");
      // // hW->DrawNormalized("same");
    // }
  // }
    
  // foutTemplates->cd();
  // for(int i=0; i<etaMuonNSteps; i++){
    // for(int j=0; j<2*WMassNSteps+1; j++){
      // hWtemplate[i][j].Write();
      // // cWtemplate[j]->Write();
    // }
  // }
  // foutTemplates->Write();
  // foutTemplates->Close();

  
  
}