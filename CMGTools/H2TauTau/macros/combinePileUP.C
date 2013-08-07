
void combinePileUp(){

  ////pileup histograms taken from here:
  /// https://twiki.cern.ch/twiki/bin/viewauth/CMS/Collisions2011Analysis
  
//   ///files used upto Nov29, 2011
//   TFile May("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileup.root","read");
//   TFile v4("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileup.root","read");
//   TFile Aug("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileup.root","read");
//   TFile v6("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileup.root","read");
//   TFile R2011B("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileup_v2.root","read");
  
  //New files for 68mb crossection
  TFile May("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileup_v2.root","read");
  TFile v4("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileup_v2.root","read");
  TFile Aug("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileup_v2.root","read");
  TFile v6("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileup_v2.root","read");
  TFile R2011B1("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_175832-177515_PromptReco_JSON.pileup_v2.root","read");
  TFile R2011B2("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileup_v2.root","read");
  TFile R2011B3("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileup_v2.root","read");
  

  TH1D* MayPileup=(TH1D*)May.Get("pileup");
  TH1D* v4Pileup=(TH1D*)v4.Get("pileup");
  TH1D* AugPileup=(TH1D*)Aug.Get("pileup");
  TH1D* v6Pileup=(TH1D*)v6.Get("pileup");
  TH1D* R2011B1Pileup=(TH1D*)R2011B1.Get("pileup");
  TH1D* R2011B2Pileup=(TH1D*)R2011B2.Get("pileup");
  TH1D* R2011B3Pileup=(TH1D*)R2011B3.Get("pileup");

  TH1D* finalPileup=(TH1D*)MayPileup->Clone();
  finalPileup->Add(v4Pileup);
  finalPileup->Add(AugPileup);
  finalPileup->Add(v6Pileup);
  finalPileup->Add(R2011B1Pileup);
  finalPileup->Add(R2011B2Pileup);
  finalPileup->Add(R2011B3Pileup);
  finalPileup->Scale(1./finalPileup->Integral());

  //TFile final("./Pileup_160404-173692_2.1invfb.pileup.root","recreate");
  //TFile final("./Pileup_2011B.pileup.root","recreate");
  TFile final("./Pileup_160404-180252_4.6invfb.pileup.root","recreate");
  finalPileup->Write();
  final.Close();
  
}



void combinePileUp3D(){
  
  //New files for 68mb crossection
  TFile May("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_160404-163869_7TeV_May10ReReco_Collisions11_JSON_v3.pileupTruth_v2_finebin.root","read");
  TFile v4("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_165088-167913_7TeV_PromptReco_JSON.pileupTruth_v2_finebin.root","read");
  TFile Aug("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_170249-172619_7TeV_ReReco5Aug_Collisions11_JSON_v2.pileupTruth_v2_finebin.root","read");
  TFile v6("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_172620-173692_PromptReco_JSON.pileupTruth_v2_finebin.root","read");
  TFile R2011B1("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_175832-177515_PromptReco_JSON.pileupTruth_v2_finebin.root","read");
  TFile R2011B2("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_177718_178078_7TeV_PromptReco_Collisons11_JSON.pileupTruth_v2_finebin.root","read");                                                                                              
  TFile R2011B3("/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Cert_178098-180252_7TeV_PromptReco_Collisions11_JSON.pileupTruth_v2_finebin.root","read");
                                                                                                
  TH1D* MayPileup=(TH1D*)May.Get("pileup");
  TH1D* v4Pileup=(TH1D*)v4.Get("pileup");
  TH1D* AugPileup=(TH1D*)Aug.Get("pileup");
  TH1D* v6Pileup=(TH1D*)v6.Get("pileup");
  TH1D* R2011B1Pileup=(TH1D*)R2011B1.Get("pileup");
  TH1D* R2011B2Pileup=(TH1D*)R2011B2.Get("pileup");
  TH1D* R2011B3Pileup=(TH1D*)R2011B3.Get("pileup");

  TH1D* finalPileup=(TH1D*)MayPileup->Clone();
  finalPileup->Add(v4Pileup);
  finalPileup->Add(AugPileup);
  finalPileup->Add(v6Pileup);
  finalPileup->Add(R2011B1Pileup);
  finalPileup->Add(R2011B2Pileup);
  finalPileup->Add(R2011B3Pileup);

  ////MayPileup+v4Pileup+AugPileup+v6Pileup
  //TFile final("./Pileup3D_160404-173692_2.1invfb.pileup.root","recreate");

  ////R2011B1Pileup+R2011B2Pileup+R2011B3Pileup
  //TFile final("./Pileup3D_2011B.pileup.root","recreate");

  ////MayPileup+v4Pileup+AugPileup+v6Pileup+R2011B1Pileup+R2011B2Pileup+R2011B3Pileup
  TFile final("./Pileup3D_160404-180252_4.6invfb.pileup.root","recreate");

  finalPileup->Scale(1./finalPileup->Integral());
  finalPileup->Write();
  final.Close();
  
}
