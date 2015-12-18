void FitRecoil(){

  TFile *fZMC = new TFile("/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/analysis/JobOutputs/RecoCountingPlotting_test44X_testRecoil_RochCorr_EffSFCorr_PileupSFCorr/output_DYJetsSig/ZanalysisOnDATA.root");
  TH2D*hu1vsZpt_WlikePos=(TH2D*)fZMC->Get("hu1vsZpt_WlikePos_Sig_eta2p1");
  TH2D*hu2vsZpt_WlikePos=(TH2D*)fZMC->Get("hu2vsZpt_WlikePos_Sig_eta2p1");
  
  TF1*doublegaus=new TF1("doublegaus","gaus(0)+gaus(3)",-100,100);
  doublegaus->SetParameter(0,1e4);
  doublegaus->SetParameter(1,0);
  doublegaus->SetParameter(2,10);
  doublegaus->SetParameter(3,1e4);
  doublegaus->SetParameter(4,0);
  doublegaus->SetParameter(5,10);

  int passo = 1;
  for(int i=1; i<hu1vsZpt_WlikePos->GetNbinsX(); i=i+passo){
    TH1D*hu1=(TH1D*)hu1vsZpt_WlikePos->ProjectionY(Form("hu1_%d",i),i,i+passo,"e");
    hu1->Fit("doublegaus");
    hu1->Draw();
    c1->SaveAs(Form("hu1_%d.png",i));
  }
  
  
  // TObjArray u1Slices;
  // hu1vsZpt_WlikePos->FitSlicesY(doublegaus,0,-1,0,"SG4",&u1Slices);
  
  // TCanvas*c1=new TCanvas();
  // u1Slices[0]->Draw();
  // c1->SaveAs("u1Slices0.png");
  // u1Slices[1]->Draw();
  // c1->SaveAs("u1Slices1.png");
  // u1Slices[2]->Draw();
  // c1->SaveAs("u1Slices2.png");
  // u1Slices[3]->Draw();
  // c1->SaveAs("u1Slices3.png");
  // u1Slices[4]->Draw();
  // c1->SaveAs("u1Slices4.png");
  // u1Slices[5]->Draw();
  // c1->SaveAs("u1Slices5.png");
  // u1Slices[6]->Draw();
  // c1->SaveAs("u1Slices6.png");

  // TObjArray u2Slices;
  // hu2vsZpt_WlikePos->FitSlicesY(doublegaus,0,-1,0,"G4",&u2Slices);
  
  // TCanvas*c1=new TCanvas();
  // u2Slices[0]->Draw();
  // c1->SaveAs("u2Slices0.png");
  // u2Slices[1]->Draw();
  // c1->SaveAs("u2Slices1.png");
  // u2Slices[2]->Draw();
  // c1->SaveAs("u2Slices2.png");
  // u2Slices[3]->Draw();
  // c1->SaveAs("u2Slices3.png");
  // u2Slices[4]->Draw();
  // c1->SaveAs("u2Slices4.png");
  // u2Slices[5]->Draw();
  // c1->SaveAs("u2Slices5.png");
  // u2Slices[6]->Draw();
  // c1->SaveAs("u2Slices6.png");

  
}