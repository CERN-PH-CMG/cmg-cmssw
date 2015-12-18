#include "../includes/common.h"

void BuildSimpleTemplates(TString folder="", int generated_PDF_set=1, int generated_PDF_member=0){

  static const int CutSteps = 8;
  int line_marker_color[CutSteps]={1,1,2,8,4,6,7,15};
  int linestyle[CutSteps]={1,1,1,1,1,1,1,1};
  TString draw_options[CutSteps]={"p","p","p","p","p","p","p","p"};

  TH1D hR_WdivZ_WlikePos[CutSteps][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TCanvas *cR_WdivZ_WlikePos[CutSteps][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TCanvas *cR_WdivZ_WlikePos_all[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  TString Zana_str[CutSteps]    ={"hWlikePos_PtScaled_1_Gen","hWlikePos_PtScaled_2_ZGenMassCut","hWlikePos_PtScaled_3_Mu1GenCut","hWlikePos_PtScaled_4_Mu2GenCut",
    "hWlikePos_PtScaled_5_RecoCut","hWlikePos_PtScaled_6_METCut","hWlikePos_PtScaled_7_RecoilCut","hWlikePos_PtScaled_8_JetCut"};
  TString hR_WdivZana_str[CutSteps]={"hR_WdivZ_WlikePos_1_Gen","hR_WdivZ_WlikePos_2_ZGenMassCut","hR_WdivZ_WlikePos_3_Mu1GenCut","hR_WdivZ_WlikePos_4_Mu2GenCut",
    "hR_WdivZ_WlikePos_5_RecoCut","hR_WdivZ_WlikePos_6_METCut","hR_WdivZ_WlikePos_7_RecoilCut","hR_WdivZ_WlikePos_8_JetCut"};

  TFile *fin=new TFile(Form("%s/../R_WdivZ_OnMC.root",folder.Data()));
  
  TH1D hWlikePosSimpleTemplate[WMass::NVarRecoilCorr][WMass::PDF_members][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D hWlikePosSimpleTemplate_NonScaled[WMass::NVarRecoilCorr][WMass::PDF_members][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TCanvas *cWtemplate[2*WMass::WMassNSteps+1];

  TFile *foutTemplates=new TFile(Form("%s/WSimpleTemplates.root",folder.Data()),"RECREATE");
  TFile *Zana=new TFile(Form("%s/ZanalysisOnDATA.root",folder.Data()));
  
  Zana->Print();
  fin->Print();
  foutTemplates->Print();

  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
      for(int h=0; h<WMass::PDF_members; h++){
        for(int m=0; m<WMass::NVarRecoilCorr; m++){
          cout << "retrieving histo " <<  Form("%s_pdf%d-%d%s_eta%s_%d",Zana_str[CutSteps-1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass) << ", ";
          TH1D*hZ=(TH1D*)Zana->Get(Form("%s_pdf%d-%d%s_eta%s_%d",Zana_str[CutSteps-1].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
          // if(hZ->Integral()>0)hZ->Scale(1/hZ->Integral());
          // TH1D*hR=(TH1D*)fin->Get(Form("hR_WdivZ_eta%s_%d",eta_str.Data(),jWmass));
          cout << "applying reweighting with " <<  Form("%s_eta%s_%d",hR_WdivZana_str[CutSteps-1].Data(),eta_str.Data(),jWmass) << endl;
          TH1D*hR=(TH1D*)fin->Get(Form("%s_eta%s_%d",hR_WdivZana_str[CutSteps-1].Data(),eta_str.Data(),jWmass));
          // if(hR->Integral()>0)hR->Scale(1/hR->Integral());
          hWlikePosSimpleTemplate[m][h][i][j]=(*hZ)*(*hR);
          // hWlikePosSimpleTemplate.Sumw2();
          hWlikePosSimpleTemplate[m][h][i][j].SetName(Form("hWlikePos_PtScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
          hWlikePosSimpleTemplate[m][h][i][j].SetTitle(Form("hWlikePos_PtScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
          hZ->Print();
          hR->Print();
          hWlikePosSimpleTemplate[m][h][i][j].Print();

          cout << "retrieving histo " <<  Form("hWlikePos_PtNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass) << ", ";
          TH1D*hZ_PtNonScaled=(TH1D*)Zana->Get(Form("hWlikePos_PtNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
          // if(hZ_PtNonScaled->Integral()>0)hZ_PtNonScaled->Scale(1/hZ_PtNonScaled->Integral());
          // TH1D*hR_PtNonScaled=(TH1D*)fin->Get(Form("hR_PtNonScaled_WdivZ_eta%s_%d",eta_str.Data(),jWmass));
          cout << "applying reweighting with " <<  Form("hR_WdivZ_WlikePos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass) << endl;
          TH1D*hR_PtNonScaled=(TH1D*)fin->Get(Form("hR_WdivZ_WlikePos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
          // if(hR_PtNonScaled->Integral()>0)hR_PtNonScaled->Scale(1/hR_PtNonScaled->Integral());
          hWlikePosSimpleTemplate_NonScaled[m][h][i][j]=(*hZ_PtNonScaled)*(*hR_PtNonScaled);
          // hWlikePosSimpleTemplate_NonScaled.Sumw2();
          hWlikePosSimpleTemplate_NonScaled[m][h][i][j].SetName(Form("hWlikePos_PtNonScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
          hWlikePosSimpleTemplate_NonScaled[m][h][i][j].SetTitle(Form("hWlikePos_PtnonScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
          hZ_PtNonScaled->Print();
          hR_PtNonScaled->Print();
          hWlikePosSimpleTemplate_NonScaled[m][h][i][j].Print();
          // cWtemplate[j]=new TCanvas(Form("cR_WdivZ_%d",jWmass));
          // TH1D*hZ2=(TH1D*)hZ->Clone(Form("WfromZmu2_pt_%d",jWmass));
          // hZ2->SetMarkerColor(2);
          // hZ2->SetLineColor(2);
          // hZ2->DrawNormalized("");
          // hW->DrawNormalized("same");
        }
      }
    }
  }
  
  foutTemplates->cd();
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      for(int h=0; h<WMass::PDF_members; h++){
        for(int m=0; m<WMass::NVarRecoilCorr; m++){
          hWlikePosSimpleTemplate[m][h][i][j].Write();
          hWlikePosSimpleTemplate_NonScaled[m][h][i][j].Write();
          // cWtemplate[j]->Write();
        }
      }
    }
  }
  foutTemplates->Write();
  foutTemplates->Close();

  
  
}