{
	
gSystem->Load("libFWCoreFWLite.so");
AutoLibraryLoader::enable();
gSystem->Load("libCintex.so");
ROOT::Cintex::Cintex::Enable();

  
  //gROOT->Macro("initTree.C");
  

 TFile *_file0 = TFile::Open("root://eoscms//eos/cms/store/cmst3/user/mgouzevi/CMG/QstarToJJ_M-3500_TuneD6T_7TeV_pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/CCB57855-4493-E011-A915-00215E222370.root");


 

  //  initCMG("ANA", &chain);
 
  Events->SetAlias("Parton", "recoGenParticles_genParticles__HLT.obj");
  Events->SetAlias("GenJets", "recoGenJets_ak5GenJets__HLT.obj");

  Events->SetAlias("Eta1", "Parton[7].eta()");
  Events->SetAlias("Eta2", "Parton[8].eta()");
  Events->SetAlias("absDEta", "TMath::Abs(Eta1-Eta2)");
  Events->SetAlias("chi", "TMath::Exp(TMath::Abs(Eta1-Eta2))");
  Events->SetAlias("absCosThetaStar", "(chi-1)/(chi+1)");


  Events->SetAlias("genEta1", "GenJets[0].eta()");
  Events->SetAlias("genEta2", "GenJets[1].eta()");
  Events->SetAlias("genAbsDEta", "TMath::Abs(genEta1-genEta2)");
  Events->SetAlias("genChi", "TMath::Exp(TMath::Abs(genEta1-genEta2))");
  Events->SetAlias("genAbsCosThetaStar", "(genChi-1)/(genChi+1)");


  Events->Draw("Parton[6].mass() >> ResonanceMass_WithMatching(80, 0, 4000.0)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[0].eta()-Parton[8].eta()) < 0.5) && (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[1].eta()-Parton[8].eta()) < 0.5)");
 
  Events->Draw("Parton[6].mass() >> ResonanceMass_WithoutMatching(80, 0, 4000.0)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[0].eta()-Parton[8].eta()) > 0.5) || (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[1].eta()-Parton[8].eta()) > 0.5)");


 

  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar(50, 0., 1.)");
  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withEtaCuts(50, 0., 1.)", "TMath::Abs(genEta1) < 2.5 && TMath::Abs(genEta2) < 2.5");
  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withDEtaCuts(50, 0., 1.)", "genAbsDEta < 1.3");
  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withEtaCuts_DEtaCuts(50, 0., 1.)", "genAbsDEta < 1.3 && TMath::Abs(genEta1) < 2.5 && TMath::Abs(genEta2) < 2.5");

  TCanvas* c1 = new TCanvas();
  Events->Draw("Parton[7].pt() >> PartonPt1(90, 0., 1800.)");

  Events->Draw("Parton[8].pt() >> PartonPt2(90, 0., 1800.)");

  Events->Draw("GenJets[0].pt() >> GenPt1(90, 0., 1800.)");

  Events->Draw("GenJets[1].pt() >> GenPt2(90, 0., 1800.)");

  Events->Draw("GenJets[0].pt() >> GenPt1_withEtaMatching(90, 0., 1800.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[0].eta()-Parton[8].eta()) < 0.5) && (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[1].eta()-Parton[8].eta()) < 0.5)");

  Events->Draw("GenJets[1].pt() >> GenPt2_withEtaMatching(90, 0., 1800.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[0].eta()-Parton[8].eta()) < 0.5) && (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[1].eta()-Parton[8].eta()) < 0.5)");

  Events->Draw("GenJets[0].pt() >> GenPt1_withoutEtaMatching(90, 0., 1800.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[0].eta()-Parton[8].eta()) > 0.5) || (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[1].eta()-Parton[8].eta()) > 0.5)");

  Events->Draw("GenJets[1].pt() >> GenPt2_withoutEtaMatching(90, 0., 1800.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[0].eta()-Parton[8].eta()) > 0.5) || (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[1].eta()-Parton[8].eta()) > 0.5)");




  Events->Draw("absCosThetaStar >> absCosThetaStar_withoutEtaMatching(50, 0., 1.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[0].eta()-Parton[8].eta()) > 0.5) || (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[1].eta()-Parton[8].eta()) > 0.5)");


  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withoutEtaMatching(50, 0., 1.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[0].eta()-Parton[8].eta()) > 0.5) || (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[1].eta()-Parton[8].eta()) > 0.5)");


  Events->Draw("absCosThetaStar >> absCosThetaStar_withEtaMatching(50, 0., 1.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[0].eta()-Parton[8].eta()) < 0.5) && (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[1].eta()-Parton[8].eta()) < 0.5)");


  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withEtaMatching(50, 0., 1.)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[0].eta()-Parton[8].eta()) < 0.5) && (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[1].eta()-Parton[8].eta()) < 0.5)");



  Events->Draw("absCosThetaStar >> absCosThetaStar(50, 0., 1.)");
  Events->Draw("absCosThetaStar >> absCosThetaStar_withDEtaCuts(50, 0., 1.)", "absDEta < 1.3");
  Events->Draw("absCosThetaStar >> absCosThetaStar_withEtaCuts(50, 0., 1.)", "TMath::Abs(Eta1) < 2.5 && TMath::Abs(Eta2) < 2.5");
  Events->Draw("absCosThetaStar >> absCosThetaStar_withEtaCuts_withDEtaCuts(50, 0., 1.)", "TMath::Abs(Eta1) < 2.5 && TMath::Abs(Eta2) < 2.5 && absDEta < 1.3");
 
  double total = absCosThetaStar->Integral();
  double EtaCuts = absCosThetaStar_withEtaCuts->Integral();
  double dEtaCuts = absCosThetaStar_withDEtaCuts->Integral();
  double Eta_dEtaCuts = absCosThetaStar_withEtaCuts_withDEtaCuts->Integral();



  cout << "total = " << total << " Eta Cuts = " << EtaCuts << " DEtaCuts = " << dEtaCuts << "Eta and DEta cuts = " << Eta_dEtaCuts << 
endl; 

  Events->Draw("Parton[6].mass() >> ResonanceMass_WithMatching(80, 0, 4000.0)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[0].eta()-Parton[8].eta()) < 0.5) && (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) < 0.5 || TMath::Abs(GenJets[1].eta()-Parton[8].eta()) < 0.5)");
  
  Events->Draw("Parton[6].mass() >> ResonanceMass_WithoutMatching(80, 0, 4000.0)", "(TMath::Abs(GenJets[0].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[0].eta()-Parton[8].eta()) > 0.5) || (TMath::Abs(GenJets[1].eta()-Parton[7].eta()) > 0.5 && TMath::Abs(GenJets[1].eta()-Parton[8].eta()) > 0.5)");


 

  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar(50, 0., 1.)");
  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withEtaCuts(50, 0., 1.)", "TMath::Abs(genEta1) < 2.5 && TMath::Abs(genEta2) < 2.5");
  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withDEtaCuts(50, 0., 1.)", "genAbsDEta < 1.3");
  Events->Draw("genAbsCosThetaStar >> genAbsCosThetaStar_withEtaCuts_DEtaCuts(50, 0., 1.)", "genAbsDEta < 1.3 && TMath::Abs(genEta1) < 2.5 && TMath::Abs(genEta2) < 2.5");
 
 
  TFile* f = new TFile("Qstar3500_EfficiencyStudy.root", "RECREATE");


  TH1F* ResonanceMass_WithMatching = _file0->Get("ResonanceMass_WithMatching");
  TH1F* ResonanceMass_WithoutMatching = _file0->Get("ResonanceMass_WithoutMatching");
  TH1F* PartonPt1 = _file0->Get("PartonPt1");
  TH1F* PartonPt2 = _file0->Get("PartonPt2");
  TH1F* GenPt1 = _file0->Get("GenPt1");  
  TH1F* GenPt2 = _file0->Get("GenPt2");
  TH1F* GenPt1_withEtaMatching = _file0->Get("GenPt1_withEtaMatching");
  TH1F* GenPt2_withEtaMatching = _file0->Get("GenPt2_withEtaMatching");
  TH1F* GenPt1_withoutEtaMatching = _file0->Get("GenPt1_withoutEtaMatching");
  TH1F* GenPt2_withoutEtaMatching = _file0->Get("GenPt2_withoutEtaMatching");
  TH1F* absCosThetaStar_withoutEtaMatching = _file0->Get("absCosThetaStar_withoutEtaMatching");
  TH1F* genAbsCosThetaStar_withoutEtaMatching = _file0->Get("genAbsCosThetaStar_withoutEtaMatching");
  TH1F* absCosThetaStar_withEtaMatching = _file0->Get("absCosThetaStar_withEtaMatching");
  TH1F* genAbsCosThetaStar = _file0->Get("genAbsCosThetaStar");
  TH1F* absCosThetaStar = _file0->Get("absCosThetaStar");
  TH1F* absCosThetaStar_withDEtaCuts = _file0->Get("absCosThetaStar_withDEtaCuts");
  TH1F* absCosThetaStar_withEtaCuts = _file0->Get("absCosThetaStar_withEtaCuts");
  TH1F* absCosThetaStar_withEtaCuts_withDEtaCuts = _file0->Get("absCosThetaStar_withEtaCuts_withDEtaCuts");

  ResonanceMass_WithMatching->Write();
  ResonanceMass_WithoutMatching->Write();
  PartonPt1->Write();
  PartonPt2->Write();
  GenPt1->Write();  
  GenPt2->Write();
  GenPt1_withEtaMatching->Write();
  GenPt2_withEtaMatching->Write();
  GenPt1_withoutEtaMatching->Write();
  GenPt2_withoutEtaMatching->Write();
  absCosThetaStar_withoutEtaMatching->Write();
  genAbsCosThetaStar_withoutEtaMatching->Write();
  absCosThetaStar_withEtaMatching->Write();
  genAbsCosThetaStar->Write();
  absCosThetaStar->Write();
  absCosThetaStar_withDEtaCuts->Write();
  absCosThetaStar_withEtaCuts->Write();
  absCosThetaStar_withEtaCuts_withDEtaCuts->Write();
  
  f->Close();

  _file0->Close();
  


}


	
