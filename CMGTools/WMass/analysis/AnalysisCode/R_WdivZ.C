#include "../includes/common.h"

void R_WdivZ(TString folder="", int OnlyCounts=0, int tot_N_evts=1, int generated_PDF_set=1, int generated_PDF_member=0){

  if(OnlyCounts==1) cout << "COMPUTING NUMBERS ONLY" << endl;
  // return;

  // int WMassCentral_MeV = 80385;
  // int WMassStep_MeV = 15;
  // static const int WMassNSteps = 60;
  // static const int etaMuonNSteps = 5;
  // double etaMaxMuons[etaMuonNSteps] = {0.6, 0.8, 1.2, 1.6, 2.1};
  static const int CutSteps = 8;
  // int line_marker_color[CutSteps]={1,1,2,3,4,1,6,13};
  int line_marker_color[CutSteps]={1,1,2,8,4,6,7,15};
  // int linestyle[CutSteps]={1,1,2,3,1,1,1,1};
  int linestyle[CutSteps]={1,1,1,1,1,1,1,1};
  // TString draw_options[CutSteps]={"histo","histo","p","histo","p","p","histo","histo"};
  TString draw_options[CutSteps]={"p","p","p","p","p","p","p","p"};

  // TH1D hR_WdivZ[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D hR_WdivZ_WlikePos[CutSteps][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], hR_WdivZ_WlikePos_scaled[CutSteps][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D hR_WdivZ_WlikePos_NonPtScaled[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], hR_WdivZ_WlikePos_scaled_NonPtScaled[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TCanvas *cR_WdivZ_WlikePos[CutSteps][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TCanvas *cR_WdivZ_WlikePos_all[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  // cR_WdivZ_WlikePos_all[i][j]=new TCanvas(Form("cR_WdivZ_WlikePos_all_pdf%d-%d_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jWmass),Form("cR_WdivZ_WlikePos_all_pdf%d-%d_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jWmass),700,700);
  
  TString Wana_str[CutSteps]    ={"hWPos_PtScaled_1_Gen","hWPos_PtScaled_2_ZGenMassCut","hWPos_PtScaled_3_Mu1GenCut","hWPos_PtScaled_4_Mu2GenCut",
    "hWPos_PtScaled_5_RecoCut","hWPos_PtScaled_6_METCut","hWPos_PtScaled_7_RecoilCut",Form("hWPos_PtScaled_8_JetCut_pdf%d-%d%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,generated_PDF_member, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar0"):"")};
  TString Zana_str[CutSteps]    ={"hWlikePos_PtScaled_1_Gen","hWlikePos_PtScaled_2_ZGenMassCut","hWlikePos_PtScaled_3_Mu1GenCut","hWlikePos_PtScaled_4_Mu2GenCut",
    "hWlikePos_PtScaled_5_RecoCut","hWlikePos_PtScaled_6_METCut","hWlikePos_PtScaled_7_RecoilCut",Form("hWlikePos_PtScaled_8_JetCut_pdf%d-%d%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,generated_PDF_member, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar0"):"")};
  TString hR_WdivZana_str[CutSteps]={"hR_WdivZ_WlikePos_1_Gen","hR_WdivZ_WlikePos_2_ZGenMassCut","hR_WdivZ_WlikePos_3_Mu1GenCut","hR_WdivZ_WlikePos_4_Mu2GenCut",
    "hR_WdivZ_WlikePos_5_RecoCut","hR_WdivZ_WlikePos_6_METCut","hR_WdivZ_WlikePos_7_RecoilCut","hR_WdivZ_WlikePos_8_JetCut"};
  TString cR_WdivZana_str[CutSteps]={"cR_WdivZ_WlikePos_1_Gen","cR_WdivZ_WlikePos_2_ZGenMassCut","cR_WdivZ_WlikePos_3_Mu1GenCut","cR_WdivZ_WlikePos_4_Mu2GenCut",
    "cR_WdivZ_WlikePos_5_RecoCut","cR_WdivZ_WlikePos_6_METCut","cR_WdivZ_WlikePos_7_RecoilCut","cR_WdivZ_WlikePos_8_JetCut"};

  TFile *fout;
  if(!OnlyCounts) fout=new TFile("R_WdivZ_OnMC.root","RECREATE");
  TFile *Wana=new TFile(OnlyCounts ? Form("%s/WanalysisOnDATA.root",folder.Data()) : Form("%s/output_WJetsSig/WanalysisOnDATA.root",folder.Data()));
  TFile *Zana=new TFile(OnlyCounts ? Form("%s/ZanalysisOnDATA.root",folder.Data()) : Form("%s/output_DYJetsSig/ZanalysisOnDATA.root",folder.Data()));

  
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;

      if(!OnlyCounts){
        cR_WdivZ_WlikePos_all[i][j]=new TCanvas(Form("cR_WdivZ_WlikePos_all_eta%s_%d",eta_str.Data(),jWmass),Form("cR_WdivZ_WlikePos_all_eta%s_%d",eta_str.Data(),jWmass),700,700);
        cR_WdivZ_WlikePos_all[i][j]->SetGridx(); cR_WdivZ_WlikePos_all[i][j]->SetGridy(); cR_WdivZ_WlikePos_all[i][j]->SetTickx(); cR_WdivZ_WlikePos_all[i][j]->SetTicky();
        TLegend *leg = new TLegend(0.2,0.6,0.5,0.9,NULL,"brNDC");
        leg->SetBorderSize(1);
        leg->SetTextFont(62);
        leg->SetLineColor(1);
        leg->SetLineStyle(1);
        leg->SetLineWidth(1);
        leg->SetFillColor(0);
        leg->SetFillStyle(1001);
        
        // if((j+1)%50==1) 
        cout << "etaMuonNStep= " << (i+1) << "/"<< WMass::etaMuonNSteps << " WMassNStep= " << (j+1) << "/"<< (2*WMass::WMassNSteps+1) << endl;

      }
      
      TH1D*hWPos_NonPtScaled=(TH1D*)Wana->Get(Form("hWPos_PtNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,generated_PDF_member, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar0"):"",eta_str.Data(),jWmass));
      TH1D*hWlikePos_NonPtScaled=(TH1D*)Zana->Get(Form("hWlikePos_PtNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,generated_PDF_member, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar0"):"",eta_str.Data(),jWmass));
      if(!OnlyCounts){
        hR_WdivZ_WlikePos_NonPtScaled[i][j]=(*hWPos_NonPtScaled)/(*hWlikePos_NonPtScaled);
        hR_WdivZ_WlikePos_NonPtScaled[i][j].SetName(Form("hR_WdivZ_WlikePos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
        hR_WdivZ_WlikePos_NonPtScaled[i][j].SetTitle(Form("hR_WdivZ_WlikePos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
      }
      for(int kcut=0; kcut<CutSteps; kcut++){
        // cout << "etaMuonNStep= " << i << "/"<< WMass::etaMuonNSteps << " WMassNStep= " << j << "/"<< (2*WMass::WMassNSteps+1) << " CutSteps= " << kcut << "/"<< CutSteps << endl;
        
        TH1D*hWPos_PtScaled=(TH1D*)Wana->Get(Form("%s_eta%s_%d",Wana_str[kcut].Data(),eta_str.Data(),jWmass));
        if(!OnlyCounts) cout << Form("%s_eta%s_%d",Wana_str[kcut].Data(),eta_str.Data(),jWmass) << " Entries= " << hWPos_PtScaled->GetEntries() << endl;
        // hWPos_PtScaled->Scale(1/hWPos_PtScaled->Integral());
        TH1D*hWlikePos_PtScaled=(TH1D*)Zana->Get(Form("%s_eta%s_%d",Zana_str[kcut].Data(),eta_str.Data(),jWmass));
        // hWlikePos_PtScaled->Scale(1/hWlikePos_PtScaled->Integral());
        if(!OnlyCounts) cout << Form("%s_eta%s_%d",Zana_str[kcut].Data(),eta_str.Data(),jWmass) << " Entries= " << hWlikePos_PtScaled->GetEntries() << endl;

        if(!OnlyCounts){
          hR_WdivZ_WlikePos[kcut][i][j]=(*hWPos_PtScaled)/(*hWlikePos_PtScaled);
          hR_WdivZ_WlikePos[kcut][i][j].SetName(Form("%s_eta%s_%d",hR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
          hR_WdivZ_WlikePos[kcut][i][j].SetTitle(Form("%s_eta%s_%d",hR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
          hWPos_PtScaled->Scale(hWPos_PtScaled->Integral()>0 ? 1/hWPos_PtScaled->GetBinContent(hWPos_PtScaled->GetXaxis()->FindBin(1)) : 1);
          hWlikePos_PtScaled->Scale(hWlikePos_PtScaled->Integral()>0 ? 1/hWlikePos_PtScaled->GetBinContent(hWlikePos_PtScaled->GetXaxis()->FindBin(1)) : 1);
          hR_WdivZ_WlikePos_scaled[kcut][i][j]=(*hWPos_PtScaled)/(*hWlikePos_PtScaled);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetName(Form("%s_eta%s_%d_scaled",hR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetTitle(Form("%s_eta%s_%d_scaled",hR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass));
          cR_WdivZ_WlikePos_all[i][j]->cd();
          // int normfactor_bin = hR_WdivZ_WlikePos_scaled[kcut][i][j].GetXaxis()->FindBin(1);
          // double normfactor = hR_WdivZ_WlikePos_scaled[kcut][i][j].GetXaxis()->GetBinContent(normfactor_bin);
          // hR_WdivZ_WlikePos_scaled[kcut][i][j]->Scale(1/normfactor);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetLineColor(line_marker_color[kcut]);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetMarkerColor(line_marker_color[kcut]);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetLineStyle(linestyle[kcut]);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetLineWidth(2);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetMarkerStyle(20);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].SetMarkerSize(0.7);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].GetXaxis()->SetRangeUser(0.7,1.6);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].GetYaxis()->SetRangeUser(0.6,1.5);
          hR_WdivZ_WlikePos_scaled[kcut][i][j].DrawCopy(kcut==0?"histo":Form("%s sames",draw_options[kcut].Data()));
          TString legend_name = hR_WdivZana_str[kcut];
          legend_name.ReplaceAll("hR_WdivZ_WlikePos_","");
          TLegendEntry *entry=leg->AddEntry((TH1D*)&hR_WdivZ_WlikePos_scaled[kcut][i][j],legend_name,"p");
          
          cR_WdivZ_WlikePos[kcut][i][j]=new TCanvas(Form("%s_eta%s_%d",cR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass),Form("%s_eta%s_%d",cR_WdivZana_str[kcut].Data(),eta_str.Data(),jWmass),700,700);
          cR_WdivZ_WlikePos[kcut][i][j]->cd();
          TH1D*hZ2=(TH1D*)hWlikePos_PtScaled->Clone(Form("%s2_eta%s_%d",Zana_str[kcut].Data(),eta_str.Data(),jWmass));
          hZ2->SetMarkerColor(2);
          hZ2->SetLineColor(2);
          hZ2->DrawNormalized("e");
          hWPos_PtScaled->DrawNormalized("sames e");
        }
      }    
      if(!OnlyCounts){
        cR_WdivZ_WlikePos_all[i][j]->cd();
        leg->Draw();
      }

    }
  }
  
  if(!OnlyCounts){
    fout->cd();
    cout << "Writing output file" << endl;
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        cR_WdivZ_WlikePos_all[i][j]->Write();
        // TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
        // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
        // if(j==60)cR_WdivZ_WlikePos_all[i][j]->SaveAs(Form("cR_WdivZ_WlikePos_all_eta%s_%d.root",eta_str.Data(),jWmass));
        for(int kcut=0; kcut<CutSteps; kcut++){
          hR_WdivZ_WlikePos[kcut][i][j].Write();
          cR_WdivZ_WlikePos[kcut][i][j]->Write();
        }
        hR_WdivZ_WlikePos_NonPtScaled[i][j].Write();
      }
    }
    fout->Write();
    fout->Close();
    TFile*f2=new TFile("cR_WdivZ_WlikePos_all.root","RECREATE");
    f2->cd();
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        cR_WdivZ_WlikePos_all[i][j]->Write();
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
        int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
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
  }else{
    
    cout << endl;
    for(int i=0; i<WMass::etaMuonNSteps; i++){
    // for(int i=WMass::etaMuonNSteps-1; i>=0; i--){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");

      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;

        cout << endl;
        cout << Form("mass %d eta %s",jWmass,eta_str.Data()) << endl;

      cout << endl;
      cout << "STARTING W ANALYSIS NUMBERS" << endl;

      for(int kcut=4; kcut<CutSteps; kcut++){
          
          // cout << Form("mass %d eta %s",jWmass,eta_str.Data()) << endl;
          TH1D*hWPos_PtScaled=(TH1D*)Wana->Get(Form("%s_eta%s_%d",Wana_str[kcut].Data(),eta_str.Data(),jWmass));
          cout << Form("%s ",Wana_str[kcut].Data()) << "\t\t Events= " << (hWPos_PtScaled->GetEntries()) << "\t\t Eff= " << (hWPos_PtScaled->GetEntries()/tot_N_evts) << endl;

        }
        cout << endl;
        cout << endl;

        cout << "STARTING Z ANALYSIS NUMBERS" << endl;
        // cout << endl;
      
        // cout << Form("mass %d eta %s",jWmass,eta_str.Data()) << endl;
        
        for(int kcut=4; kcut<CutSteps; kcut++){
          
          // cout << Form("mass %d eta %s",jWmass,eta_str.Data()) << endl;
          TH1D*hWlikePos_PtScaled=(TH1D*)Zana->Get(Form("%s_eta%s_%d",Zana_str[kcut].Data(),eta_str.Data(),jWmass));
          cout << Form("%s ",Zana_str[kcut].Data()) << "\t\t Events= " << (hWlikePos_PtScaled->GetEntries()) << "\t\t Eff= " << (hWlikePos_PtScaled->GetEntries()/tot_N_evts) << endl;

        }
      }
      cout << endl;
      cout << endl;
    }

  }
  // // return;
  
  // TH1D hWlikePosSimpleTemplate[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // // TCanvas *cWtemplate[2*WMass::WMassNSteps+1];

  // TFile *foutTemplates=new TFile("Wtemplates.root","RECREATE");
  // // TFile *Zana=new TFile(Form("%s/ZanalysisOnMC.root",folder.Data()));
  // // TFile *Zana=new TFile(OnlyCounts ? Form("%s/ZanalysisOnDATA.root",folder.Data()) : Form("%s/output_DYJetsSig/ZanalysisOnDATA.root",folder.Data()));
 // // TFile *R_WdivZ=new TFile("R_WdivZ_OnMC.root");

  // for(int i=0; i<WMass::etaMuonNSteps; i++){
    // TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    // for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
      // TH1D*hZ=(TH1D*)Zana->Get(Form("hWlikePos_PtScaled_RWeighted_Templates_eta%s_%d",eta_str.Data(),jWmass));
      // hZ->Scale(1/hZ->Integral());
      // TH1D*hR=(TH1D*)fout->Get(Form("hR_WdivZ_eta%s_%d",eta_str.Data(),jWmass));
      // hR->Scale(1/hR->Integral());
      // hWlikePosSimpleTemplate[i][j]=(*hZ)*(*hR);
      // // hWlikePosSimpleTemplate.Sumw2();
      // hWlikePosSimpleTemplate[i][j].SetName(Form("hWlikePos_PtScaled_RWeighted_SimpleTemplates_eta%s_%d",eta_str.Data(),jWmass));
      // hWlikePosSimpleTemplate[i][j].SetTitle(Form("hWlikePos_PtScaled_RWeighted_SimpleTemplates_eta%s_%d",eta_str.Data(),jWmass));
      // // cWtemplate[j]=new TCanvas(Form("cR_WdivZ_%d",jWmass));
      // // TH1D*hZ2=(TH1D*)hZ->Clone(Form("WfromZmu2_pt_%d",jWmass));
      // // hZ2->SetMarkerColor(2);
      // // hZ2->SetLineColor(2);
      // // hZ2->DrawNormalized("");
      // // hW->DrawNormalized("same");
    // }
  // }
  
  // foutTemplates->cd();
  // for(int i=0; i<WMass::etaMuonNSteps; i++){
  // for(int j=0; j<2*WMass::WMassNSteps+1; j++){
  // hWlikePosSimpleTemplate[i][j].Write();
  // // cWtemplate[j]->Write();
  // }
  // }
  // foutTemplates->Write();
  // foutTemplates->Close();

  
  
}