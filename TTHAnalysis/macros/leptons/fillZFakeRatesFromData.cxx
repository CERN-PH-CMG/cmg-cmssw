
TString gTreePath = "/data/gpetrucc/8TeV/ttH/TREES_270413_HADD/%s/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root";
void fillFR(TString hist, TString cut, TString pass, TString compName, int maxLep) {
    TDirectory *root = gDirectory;
    TFile *f = TFile::Open(Form(gTreePath.Data(),compName.Data()));
    TTree *tree = (TTree*) f->Get("ttHLepTreeProducerBase");
    root->cd();
    for (int i = 3; i <= maxLep; ++i) {
        TString mycut = cut;   mycut.ReplaceAll("%d", Form("%d",i));
        TString mypass = pass; mypass.ReplaceAll("%d", Form("%d",i));
        tree->Draw(Form("abs(LepGood%d_eta):min(LepGood%d_pt,99.9)>>+%s_den", i,i, hist.Data()),
                   Form("%s", mycut.Data()));
        tree->Draw(Form("abs(LepGood%d_eta):min(LepGood%d_pt,99.9)>>+%s_num", i,i, hist.Data()),
                   Form("(%s) && (%s)", mycut.Data(), mypass.Data()));
    }
    f->Close();
    TH2 *den = (TH2*) gROOT->FindObject(hist+"_den"); den->Sumw2(); den->Write();
    TH2 *num = (TH2*) gROOT->FindObject(hist+"_num"); num->Sumw2(); num->Write();
    TH2 *ratio = num->Clone(hist);
    ratio->Divide(num,den,1,1,"B");
    ratio->Write();
}
void fillBaseWeights(TString hist, TString cut, TString pass, TString compName, int maxLep) {
    TDirectory *root = gDirectory;
    TFile *f = TFile::Open(Form(gTreePath.Data(),compName.Data()));
    TTree *tree = (TTree*) f->Get("ttHLepTreeProducerBase");
    root->cd();
    for (int i = 1; i <= maxLep; ++i) {
        TString mycut = cut;   mycut.ReplaceAll("%d", Form("%d",i));
        TString mypass = pass; mypass.ReplaceAll("%d", Form("%d",i));
        tree->Draw(Form("TMath::Max(TMath::Min(LepGood%d_jetBTagCSV,0.999),0)>>+%s", i, hist.Data()),  mycut.Data());
    }
    TH2 *h = (TH2*) gROOT->FindObject(hist); 
    f->Close();
    h->Write();
}

void fillZFakeRatesFromData() {
    gROOT->ProcessLine(".L ../../python/plotter/functions.cc+");

    const int npt_mu = 4, npt_el = 3, neta = 2;
    double ptbins_mu[npt_mu+1] = { 5.0, 7.0, 10, 15, 30 };
    double ptbins_el[npt_el+1] = {      7.0, 10, 15, 30 };
    //double etabins_mu[neta+1] = { 0.0, 0.7, 1.5,   2.0,  2.5 };
    //double etabins_el[neta+1] = { 0.0, 0.7, 1.479, 2.0,  2.5 };
    double etabins_mu[neta+1] = { 0.0, 1.5,   2.5 };
    double etabins_el[neta+1] = { 0.0, 1.479, 2.5 };

    TFile *fOut = TFile::Open("fakeRates_Z3l_Data.root", "RECREATE");
    const int  nsels = 3;
    const char *sels[nsels] = { "FR", "FRC", "FRH" };
    for (int is = 0; is < nsels; ++is) {
        TH2F *FR_mu_den = new TH2F(Form("%s_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_mu_num = new TH2F(Form("%s_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight_mu_den = new TH2F(Form("%s_tight_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight_mu_num = new TH2F(Form("%s_tight_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_el_den = new TH2F(Form("%s_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_el_num = new TH2F(Form("%s_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight_el_den = new TH2F(Form("%s_tight_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight_el_num = new TH2F(Form("%s_tight_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
    }
    //TH1 *w_el = new TH1F("W_btag_el", "CSV", 20, 0, 1);
    //TH1 *w_el = new TH1F("W_btag_mu", "CSV", 20, 0, 1);

    TString baseCut = " nLepGood == 3 && LepGood1_pdgId+LepGood2_pdgId == 0 && ";
    baseCut += "minMllAFAS > 12 && abs(mZ1-91.2) < 10 && ";
    baseCut += "met*0.00397 + mhtJet25*0.00265 < 0.15 && ";
    //baseCut += "met*0.00397 + mhtJet25*0.00265 < 0.3 && ";
    baseCut += "abs(mass_2(LepGood1_pt,LepGood1_eta,LepGood1_phi,LepGood1_mass,LepGood2_pt,LepGood2_eta,LepGood2_phi,LepGood2_mass)-mZ1) < 0.001 && ";
    baseCut += "nBJetLoose25 == 1 && ";

    TString baseCutT = " (LepGood%d_tightCharge > (abs(LepGood%d_pdgId) == 11)) && ";
    baseCutT += "LepGood%d_innerHits*(abs(LepGood%d_pdgId) == 11) == 0 && "; // require to be zero if the lepton is an electron
    baseCutT += "(LepGood%d_convVeto==0)*(abs(LepGood%d_pdgId) == 11) == 0 && ";

    TString sample = "";
    const char *samples[10] = { "DoubleMuAB", "DoubleMuC", "DoubleMuD", "DoubleMuRec", "DoubleMuBadSIP",
                               "DoubleElectronAB", "DoubleElectronC", "DoubleElectronD", "DoubleElectronRec", "DoubleElectronBadSIP" };
    for (int id = 0; id < 10; ++id) { 
        sample = TString(samples[id]);
        //fillBaseWeights("W_btag_el", baseCut + "abs(LepGood%d_pdgId) == 11", "LepGood%d_pt > 10 && LepGood%d_mva < 0.25", sample, 4);
        //fillBaseWeights("W_btag_mu", baseCut + "abs(LepGood%d_pdgId) == 13", "LepGood%d_pt > 10 && LepGood%d_mva < 0.25", sample, 4);

        std::cout << "Processing MVA selection on " << sample << std::endl;
        fillFR("FR_el", baseCut + "abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.35", sample, 3);
        fillFR("FR_mu", baseCut + "abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.35", sample, 3);
        //fillFR("FR_tight_el", baseCutT + "abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 3);
        //fillFR("FR_tight_mu", baseCutT + "abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);

#if 0
        std::cout << "Processing cut-based selection on " << sample << std::endl;
        fillFR("FRC_el", baseCut + "abs(LepGood%d_pdgId) == 11 && (abs(LepGood%d_eta)<1.4442 || abs(LepGood%d_eta)>1.5660)", "LepGood%d_relIso < 0.25 && LepGood%d_tightId > 0.0 && abs(LepGood%d_dxy) < 0.04 && abs(LepGood%d_innerHits) <= 0", sample, triggering ? 2 : 4);
        fillFR("FRC_mu", baseCut + "abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.2", sample, triggering ? 2 : 4);
        fillFR("FRC_tight_el", baseCutT + "abs(LepGood%d_pdgId) == 11 && (abs(LepGood%d_eta)<1.4442 || abs(LepGood%d_eta)>1.5660)", "LepGood%d_relIso < 0.15 && LepGood%d_tightId > 0.0 && abs(LepGood%d_dxy) < 0.02 && abs(LepGood%d_innerHits) <= 0", sample, triggering ? 2 : 4);
        fillFR("FRC_tight_mu", baseCutT + "abs(LepGood%d_pdgId) == 13 && abs(LepGood%d_eta) < 2.1", "LepGood%d_relIso < 0.12 && LepGood%d_tightId   && abs(LepGood%d_dxy) < 0.2 && abs(LepGood%d_dz) < 0.5", sample, triggering ? 2 : 4);


        std::cout << "Processing hybrid selection on " << sample << std::endl;
        fillFR("FRH_el", baseCut + "abs(LepGood%d_pdgId) == 11", "LepGood%d_relIso < 0.25 && LepGood%d_sip3d < 6 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_mu", baseCut + "abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.25 && LepGood%d_sip3d < 6 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_tight_el", baseCutT + "abs(LepGood%d_pdgId) == 11", "LepGood%d_relIso < 0.2 && LepGood%d_sip3d < 4 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_tight_mu", baseCutT + "abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.2 && LepGood%d_sip3d < 4 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_el", baseCut + "abs(LepGood%d_pdgId) == 11", "LepGood%d_sip3d < 10 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_mu", baseCut + "abs(LepGood%d_pdgId) == 13", "LepGood%d_sip3d < 10 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_tight_el", baseCutT + "abs(LepGood%d_pdgId) == 11", "LepGood%d_sip3d < 3 && LepGood%d_jetBTagCSV < 20.2", sample, 4);
        fillFR("FRH_tight_mu", baseCutT + "abs(LepGood%d_pdgId) == 13", "LepGood%d_sip3d < 3 && LepGood%d_jetBTagCSV < 20.2", sample, 4);
#else
#endif
    }

    fOut->Close();
}
