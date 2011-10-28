
void combinePileUp2invfb(){

  TFile May("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileup.root","read");
  TFile v4("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileup.root","read");
  TFile Aug("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileup.root","read");
  TFile v6("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileup.root","read");
  
  TH1D* MayPileup=(TH1D*)May.Get("pileup");
  TH1D* v4Pileup=(TH1D*)v4.Get("pileup");
  TH1D* AugPileup=(TH1D*)Aug.Get("pileup");
  TH1D* v6Pileup=(TH1D*)v6.Get("pileup");

  TH1D* finalPileup=(TH1D*)MayPileup->Clone();
  finalPileup->Add(v4Pileup);
  finalPileup->Add(AugPileup);
  finalPileup->Add(v6Pileup);
  finalPileup->Scale(1./finalPileup->Integral());

  TFile final("./PileUp_160404-173692_2.1invfb.pileup.root","recreate");
  finalPileup->Write();
  final.Close();
  
}
