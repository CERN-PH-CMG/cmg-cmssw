void derivePtSF(bool doMad, bool doPos, bool doClosure) {

  TString isClosure="";
  if(doClosure) isClosure="_PtSFCorr0";

  TString isPos="Pos";
  if(!doPos) isPos="Neg";

  TFile* finZPtSF_DATA = new TFile(Form("../JobOutputs/WEIGHTfolder_mu%s_tkmet_ewk0_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PileupSFCorr/output_DATA/ZanalysisOnDATA.root",isPos.Data()));
  TFile* finZPtSF_BKG = new TFile(Form("../JobOutputs/WEIGHTfolder_mu%s_tkmet_ewk0_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PileupSFCorr/output_EWKTT/ZanalysisOnDATA.root",isPos.Data()));

  TString isMad="output_DYJetsPow";
  if(doMad) isMad="output_DYJetsMadSig";  

  TFile* finZPtSF_Signal = new TFile(Form("../JobOutputs/WEIGHTfolder_mu%s_tkmet_ewk0_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr%s_PileupSFCorr/%s/ZanalysisOnDATA.root",isPos.Data(),isClosure.Data(),isMad.Data()));
  TH1D* hZPtSF_Signal=(TH1D*) finZPtSF_Signal->Get(Form("hWlike%s_ZpT_8_JetCut_pdf229800-0_eta0p9_91188",isPos.Data())); hZPtSF_Signal->Sumw2();
  hZPtSF_Signal->Smooth(5);  

  TH1D* hZPtSF_DATA=(TH1D*) finZPtSF_DATA->Get(Form("hWlike%s_ZpT_8_JetCut_pdf229800-0_eta0p9_91188",isPos.Data())); hZPtSF_DATA->Sumw2();
  TH1D* hZPtSF_BKG=(TH1D*) finZPtSF_BKG->Get(Form("hWlike%s_ZpT_8_JetCut_pdf229800-0_eta0p9_91188",isPos.Data())); hZPtSF_BKG->Sumw2();
  hZPtSF_BKG->Scale(-1);
  TH1D* hZPtSF_DATA_sub = hZPtSF_DATA->Clone();
  hZPtSF_DATA_sub->Add(hZPtSF_BKG);
  hZPtSF_DATA_sub->Smooth(5);  

  cout << "  signal " << hZPtSF_Signal->GetEntries() << endl;
  cout << "  DATA " << hZPtSF_DATA->GetEntries() << endl;

  ///////  

  hZPtSF_DATA_sub->Scale(1./hZPtSF_DATA_sub->Integral());
  hZPtSF_Signal->Scale(1./hZPtSF_Signal->Integral());
  hZPtSF_DATA_sub->Divide(hZPtSF_Signal);
  hZPtSF_DATA_sub->Smooth(5);  
  hZPtSF_DATA_sub->Draw("p e");  
  hZPtSF_DATA_sub->GetXaxis()->SetRangeUser(0.,30.);
  hZPtSF_DATA_sub->GetYaxis()->SetRangeUser(0.5,1.5);
  hZPtSF_DATA_sub->GetXaxis()->SetTitle("Z p_{T}");
  if(doMad) hZPtSF_DATA_sub->SetLineColor(2);
  if(doMad) hZPtSF_DATA_sub->SetMarkerColor(2);

  TFile *fout = TFile::Open(Form("Zpt_%s_%s%s.root",isMad.Data(),isPos.Data(),isClosure.Data()),"RECREATE");
  hZPtSF_DATA_sub->Write();
  fout->Write();
  fout->Close();  

  ///////  

}

void deriveSF() {

  gStyle->SetOptStat(0100);

  bool doClosure=false;
  bool doPos=true;
  derivePtSF(true,doPos,doClosure);
  derivePtSF(false,doPos,doClosure);

  doPos=false;
  derivePtSF(true,doPos,doClosure);
  derivePtSF(false,doPos,doClosure);

  doClosure=true;
  doPos=true;
  derivePtSF(true,doPos,doClosure);
  derivePtSF(false,doPos,doClosure);

}
