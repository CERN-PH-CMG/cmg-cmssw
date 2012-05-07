{

  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  gSystem->Load("libCintex.so");
  ROOT::Cintex::Cintex::Enable();

  string s1 = "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/Resonances/ZprimeToLongLived/ZprimeTo2LongLived_400GeV_50GeV_80mm_ee.root";
  //string s1 = "/tmp/mgouzevi/ZprimeTo2LongLived_400GeV_50GeV_80mm_ee.root";
  TFile* f1  = TFile::Open(s1.data());
  TTree* mainTree = (TTree*) f1->Get("Events");

  gStyle->SetOptStat("neimr");

  TCanvas* c1 = new TCanvas();
  c1->cd();

  mainTree->SetAlias("Part", "recoGenParticles_genParticles__GEN.obj");
  mainTree->SetAlias("Z0_pz", "Part[6].pz()");

  /*
  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.status() >> STATUS(5, -0.5, 4.5)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 32");
  c1->SaveAs("ZprimeToLLTo4e_400GeV_Status.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.status() >> STATUS(5, -0.5, 4.5)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 6000113");
  c1->SaveAs("LLTo4e_50GeV_80mm_status.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.status() >> STATUS(5, -0.5, 4.5)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 6000113");
  c1->SaveAs("LLTo4e_50GeV_80mm_status.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.mass() >> PT(50, 0, 500)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 32 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("ZprimeToLLTo4e_400GeV_Mass.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.pz() >> PT(100, 0, 1000)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 32 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("ZprimeToLLTo4e_400GeV_pz.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.pt() >> PT(50, 0, 500)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 32 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("ZprimeToLLTo4e_400GeV_pt.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.mass() >> PT(20, 40, 60)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 6000113 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("LLTo4e_50GeV_80mm_mass.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.pt() >> PT(50, 0, 500)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 6000113 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("LLTo4e_50GeV_80mm_pt.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.pt() >> PT(200, 0, 400)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 11 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("e_50GeV_80mm_pt.png");

  mainTree->Draw("recoGenParticles_genParticles__GEN.obj.eta() >> ETA(60, -3, 3)", "TMath::Abs(recoGenParticles_genParticles__GEN.obj.pdgId()) == 11 && recoGenParticles_genParticles__GEN.obj.status() == 2");
  c1->SaveAs("e_50GeV_80mm_eta.png");
  */



  
  mainTree->SetAlias("F1_pdgId", "Part[9].pdgId()");
  mainTree->SetAlias("F2_pdgId", "Part[10].pdgId()");
  mainTree->SetAlias("F3_pdgId", "Part[11].pdgId()");
  mainTree->SetAlias("F4_pdgId", "Part[12].pdgId()");

  mainTree->SetAlias("F1_eta", "Part[9].eta()");
  mainTree->SetAlias("F2_eta", "Part[10].eta()");
  mainTree->SetAlias("F3_eta", "Part[11].eta()");
  mainTree->SetAlias("F4_eta", "Part[12].eta()");

  mainTree->SetAlias("F1_pt", "Part[9].pt()");
  mainTree->SetAlias("F2_pt", "Part[10].pt()");
  mainTree->SetAlias("F3_pt", "Part[11].pt()");
  mainTree->SetAlias("F4_pt", "Part[12].pt()");


  mainTree->Draw("Z0_pz >> PZ(100, -3000, 3000)", "");

  TCut LL1_e1 = "TMath::Abs(F1_pdgId) == 11 && F1_pt > 35 && TMath::Abs(F1_eta) < 2";
  TCut LL1_e2 = "TMath::Abs(F2_pdgId) == 11 && F2_pt > 35 && TMath::Abs(F2_eta) < 2";

  TCut LL1 = LL1_e1 && LL1_e2;

  TCut LL2_e1 = "TMath::Abs(F3_pdgId) == 11 && F3_pt > 35 && TMath::Abs(F3_eta) < 2";
  TCut LL2_e2 = "TMath::Abs(F4_pdgId) == 11 && F4_pt > 35 && TMath::Abs(F4_eta) < 2";

  TCut LL2 = LL2_e1 && LL2_e2;

  mainTree->Draw("Z0_pz >> PZ1(100, -3000, 3000)", LL1);
  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassFirstPhotonCut.png");
  
  mainTree->Draw("Z0_pz >> PZ2(100, -3000, 3000)", !LL1 && LL2);
  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassSecondPhotonCut.png");

  mainTree->Draw("doubles_pdfWeights_cteq66_GEN.obj[0]>> PT(30, 0, 2)", "");
   c1->SaveAs("cteq66_weights.png");

  mainTree->Draw("doubles_pdfWeights_MRST2004nlo_GEN.obj[0]>> PT(30, 0, 2)", "");
   c1->SaveAs("MRST2004nlo_weights.png");

   cout << "Effic CTEQ61L = " << (PZ1->Integral() + PZ2->Integral())/PZ->Integral() << endl;
    
 

   mainTree->Draw("Z0_pz >> PZ_cteq66(100, -3000, 3000)", "doubles_pdfWeights_cteq66_GEN.obj[0]");

   TCut cteq66_1 = "doubles_pdfWeights_cteq66_GEN.obj[0]" * (LL1); 

   mainTree->Draw("Z0_pz >> PZ1_cteq66(100, -3000, 3000)", cteq66_1);
  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassFirstPhotonCut_cteq66.png");
  

   TCut cteq66_2 = "doubles_pdfWeights_cteq66_GEN.obj[0]" * (!LL1 && LL2); 
   mainTree->Draw("Z0_pz >> PZ2_cteq66(100, -3000, 3000)", cteq66_2);
  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassFirstPhotonCut_cteq66.png");
  

  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassSecondPhotonCut_cteq66.png");

 cout << "Effic CTEQ66 = " << (PZ1_cteq66->Integral() + PZ2_cteq66->Integral())/PZ_cteq66->Integral() << endl;

   mainTree->Draw("Z0_pz >> PZ_MRST2004nlo(100, -3000, 3000)", "doubles_pdfWeights_MRST2004nlo_GEN.obj[0]");


  TCut MRST2004nlo_1 = "doubles_pdfWeights_MRST2004nlo_GEN.obj[0]" * (LL1); 
  mainTree->Draw("Z0_pz >> PZ1_MRST2004nlo(100, -3000, 3000)", MRST2004nlo_1);
  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassFirstPhotonCut_MRST2004nlo.png");

  TCut MRST2004nlo_2 = "doubles_pdfWeights_MRST2004nlo_GEN.obj[0]" * (!LL1 && LL2); 
  mainTree->Draw("Z0_pz >> PZ2_MRST2004nlo(100, -3000, 3000)", MRST2004nlo_2);

  c1->SaveAs("ZprimeToLLTo4e_400GeV_PassSecondPhotonCut_MRST2004nlo.png");
  
 cout << "Effic MRST2004nlo = " << (PZ1_MRST2004nlo->Integral() + PZ2_MRST2004nlo->Integral())/PZ_MRST2004nlo->Integral() << endl;


}
