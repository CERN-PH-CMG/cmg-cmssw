float fM_1    = 0;
float fPt_1   = 0;
float fPhi_1  = 0;
float fEta_1  = 0;
float fM_2    = 0;
float fPt_2   = 0;
float fPhi_2  = 0;
float fEta_2  = 0;
float fMSV    = 0;
float fMSVF   = 0;
float fPtSV   = 0;
float fPhiSV  = 0;
float fYSV    = 0;
float fMet    = 0;
float fMPhi   = 0;
float fMVis   = 0;
float fPtVis  = 0;
float fYVis   = 0;
float fPhiVis   = 0;

void AddVisMassOne(std::string iName,std::string iVal) { 
  TTree *lOTree = new TTree(iVal.c_str(),iVal.c_str()); 
  lOTree->Branch("l1_M" ,&fM_1);
  lOTree->Branch("l1_Px",&fPt_1);
  lOTree->Branch("l1_Py",&fPhi_1);
  lOTree->Branch("l1_Pz",&fEta_1);
  lOTree->Branch("l2_M" ,&fM_2);
  lOTree->Branch("l2_Px",&fPt_2);
  lOTree->Branch("l2_Py",&fPhi_2);
  lOTree->Branch("l2_Pz",&fEta_2);

  lOTree->Branch("pt_sv" ,&fPtSV);
  lOTree->Branch("y_sv"  ,&fYSV);

  lOTree->Branch("m_sv"  ,&fMSV);
  lOTree->Branch("m_svfit"  ,&fMSVF);
  lOTree->Branch("phi_sv",&fPhiSV);
  lOTree->Branch("met"   ,&fMet);
  lOTree->Branch("mphi"  ,&fMPhi);
  lOTree->Branch("m_vis" ,&fMVis); 
  lOTree->Branch("pt_vis",&fPtVis); 
  lOTree->Branch("y_vis",&fYVis); 
  lOTree->Branch("phi_vis",&fPhiVis); 

  TFile *lFile = new TFile(iName.c_str());
  TTree *lTree = (TTree*) lFile->FindObjectAny(iVal.c_str());
  lTree->SetBranchAddress("l1_M" ,&fM_1);
  lTree->SetBranchAddress("l1_Px",&fPt_1);
  lTree->SetBranchAddress("l1_Py",&fPhi_1);
  lTree->SetBranchAddress("l1_Pz",&fEta_1);
  lTree->SetBranchAddress("l2_M" ,&fM_2);
  lTree->SetBranchAddress("l2_Px",&fPt_2);
  lTree->SetBranchAddress("l2_Py",&fPhi_2);
  lTree->SetBranchAddress("l2_Pz",&fEta_2);

  lTree->SetBranchAddress("pt_sv" ,&fPtSV);
  lTree->SetBranchAddress("y_sv"  ,&fYSV);
  lTree->SetBranchAddress("m_sv"  ,&fMSV);
  lTree->SetBranchAddress("m_svfit"  ,&fMSVF);
  lTree->SetBranchAddress("phi_sv",&fPhiSV);
  lTree->SetBranchAddress("met"   ,&fMet);
  lTree->SetBranchAddress("mphi"  ,&fMPhi);

  for(int i0 = 0; i0 < lTree->GetEntries(); i0++) { 
    lTree->GetEntry(i0);
    TLorentzVector lL0; lL0.SetPtEtaPhiM(fPt_1,fEta_1,fPhi_1,fM_1);
    TLorentzVector lL1; lL1.SetPtEtaPhiM(fPt_2,fEta_2,fPhi_2,fM_2);
    lL0      += lL1;
    fMVis    = lL0.M();
    fPtVis   = lL0.Pt();
    fYVis    = lL0.Rapidity();
    fPhiVis  = lL0.Phi();
    lOTree->Fill();
  } 
  //lOTree->Write();
}       

void AddVisMass() {  
  TFile* lOFile = new TFile("OutputEMu.root",   "RECREATE");  AddVisMassOne("zjets_mc_2.root","EMu");     lOFile->Write(); lOFile->Close();
  TFile* lOFile = new TFile("OutputETau.root",  "RECREATE");  AddVisMassOne("zjets_mc_2.root","ETau");    lOFile->Write(); lOFile->Close();
  TFile* lOFile = new TFile("OutputMuTau.root", "RECREATE");  AddVisMassOne("zjets_mc_2.root","MuTau");   lOFile->Write(); lOFile->Close();
}
