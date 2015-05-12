TString gTreePath = "/data1/emanuele/monox/TREES_060515_MET200SKIM/%s/treeProducerDarkMatterMonoJet/tree.root";
TString gFriendTreePath = "/data1/emanuele/monox/TREES_060515_MET200SKIM/0_eventvars_mj_v1/evVarFriend_%s.root";

void fillEF(TString hist, TString presel, TString srcut, TString crcut, TString compName) {
  TDirectory *root = gDirectory;
  TFile *f = TFile::Open(Form(gTreePath.Data(),compName.Data()));
  TTree *tree = (TTree*) f->Get("tree");
  tree->AddFriend("mjvars/t",Form(gFriendTreePath.Data(),compName.Data()));

  root->cd();
  TString mycut = presel;
  TString mydencut = Form("((%s) && (%s))", mycut.Data(), crcut.Data());
  TString mynumcut = Form("((%s) && (%s))", mycut.Data(), srcut.Data());

  tree->Draw(Form("min(metNoMu_pt,1000)>>+%s_den", hist.Data()), Form("%s", mydencut.Data()));
  tree->Draw(Form("min(metNoMu_pt,1000)>>+%s_num", hist.Data()), Form("%s", mynumcut.Data()));

  f->Close();

}

void fillWTrivialExtrapolationFactorsFromMC() {
  const int nmet = 9;
  double metbins[nmet+1] = { 200, 225, 250, 275, 300, 325, 350, 400, 500, 1000 };

  TFile *fOut = TFile::Open("extrapolationFactors.root", "RECREATE");
 
  TH1F *EF_W_den = new TH1F("EF_W_den","",nmet,metbins);
  TH1F *EF_W_num = new TH1F("EF_W_num","",nmet,metbins);

  TString baseCut = " ((nJet30 == 1 || nJet30 == 2) && jetclean > 0.5) && (Jet[0]_pt>110) && (nJet30 == 1 || (nJet==2 && abs(dphijj)<2.5)) && (nGamma15V==0) && (nEle10V==0 && nTau15V==0) ";
  
  const char *samples[4] = { "WJetsToLNu_HT100to200", "WJetsToLNu_HT200to400", "WJetsToLNu_HT400to600", "WJetsToLNu_HT600toInf" };
 
  TString hist = "EF_W";
 
  for (int id = 0; id < 4; ++id) { 
    sample = TString(samples[id]);
    std::cout << "Processing Control Region and Signal Region selection on " << sample << std::endl;
    fillEF(hist, baseCut, "nMu10V == 0", "nMu10V > 0 && nMu20T == 1 && nBJetMedium30 == 0", sample);
  }

  TH2 *den = (TH2*) gROOT->FindObject(hist+"_den"); den->Sumw2(); den->Write();
  TH2 *num = (TH2*) gROOT->FindObject(hist+"_num"); num->Sumw2(); num->Write();

  TH2 *ratio = num->Clone(hist);
  ratio->Divide(num,den,1,1,"B");
  ratio->Write();

  fOut->Close();

}
