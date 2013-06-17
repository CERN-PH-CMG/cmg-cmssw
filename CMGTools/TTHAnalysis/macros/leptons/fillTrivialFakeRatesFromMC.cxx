TString gTreePath = "/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD/%s/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root";
void fillFR(TString hist, TString cut, TString pass, TString compName, int maxLep) {
    TDirectory *root = gDirectory;
    TFile *f = TFile::Open(Form(gTreePath.Data(),compName.Data()));
    TTree *tree = (TTree*) f->Get("ttHLepTreeProducerBase");
    root->cd();
    for (int i = 1; i <= maxLep; ++i) {
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

void fillTrivialFakeRatesFromMC(int triggering=1) {
    const int npt_mu = 12, npt_el = 10, neta = 2;
    double ptbins_mu[npt_mu+1] = { 5.0, 7.0, 8.5, 10, 12.5, 15, 17.5, 20, 25, 30, 40, 60, 80 };
    double ptbins_el[npt_el+1] = { 7, 8.5, 10, 12.5, 15, 20, 25, 30, 45, 60, 80 };
    //double etabins_mu[neta+1] = { 0.0, 0.7, 1.5,   2.0,  2.5 };
    //double etabins_el[neta+1] = { 0.0, 0.7, 1.479, 2.0,  2.5 };
    double etabins_mu[neta+1] = { 0.0, 1.5,   2.5 };
    double etabins_el[neta+1] = { 0.0, 1.479, 2.5 };

    TFile *fOut = TFile::Open(triggering ? "fakeRates_TTJets_MC.root" :  "fakeRates_TTJets_MC_NonTrig.root", "RECREATE");
    //TFile *fOut = TFile::Open("fakeRates_TTLep_MC.root", "RECREATE");
    const int  nsels = 3;
    const char *sels[nsels] = { "FR", "FRC", "FRH" };
    for (int is = 0; is < nsels; ++is) {
        TH2F *FR_mu_den = new TH2F(Form("%s_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_mu_num = new TH2F(Form("%s_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight_mu_den = new TH2F(Form("%s_tight_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight_mu_num = new TH2F(Form("%s_tight_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight2_mu_den = new TH2F(Form("%s_tight2_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight2_mu_num = new TH2F(Form("%s_tight2_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_loose_mu_num = new TH2F(Form("%s_loose_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_loose_mu_den = new TH2F(Form("%s_loose_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_el_den = new TH2F(Form("%s_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_el_num = new TH2F(Form("%s_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight_el_den = new TH2F(Form("%s_tight_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight_el_num = new TH2F(Form("%s_tight_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight2_el_den = new TH2F(Form("%s_tight2_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight2_el_num = new TH2F(Form("%s_tight2_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_loose_el_den = new TH2F(Form("%s_loose_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_loose_el_num = new TH2F(Form("%s_loose_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight3_mu_num = new TH2F(Form("%s_tight3_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight3_mu_den = new TH2F(Form("%s_tight3_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight3_el_num = new TH2F(Form("%s_tight3_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight3_el_den = new TH2F(Form("%s_tight3_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight4_mu_num = new TH2F(Form("%s_tight4_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight4_mu_den = new TH2F(Form("%s_tight4_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight4_el_num = new TH2F(Form("%s_tight4_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight4_el_den = new TH2F(Form("%s_tight4_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight5_mu_num = new TH2F(Form("%s_tight5_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight5_mu_den = new TH2F(Form("%s_tight5_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight5_el_num = new TH2F(Form("%s_tight5_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight5_el_den = new TH2F(Form("%s_tight5_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight6_mu_num = new TH2F(Form("%s_tight6_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight6_mu_den = new TH2F(Form("%s_tight6_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight6_el_num = new TH2F(Form("%s_tight6_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight6_el_den = new TH2F(Form("%s_tight6_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
    }
    //TH1 *w_el = new TH1F("W_btag_el", "CSV", 20, 0, 1);
    //TH1 *w_el = new TH1F("W_btag_mu", "CSV", 20, 0, 1);

    TString baseCut = " ";
    if (triggering) baseCut += "nLepGood == 2 && ";
    //baseCut += "(nLepGood == 2 || LepGood3_mvaId < -0.2) && LepGood1_charge*LepGood2_charge > 0 && abs(LepGood2_pdgId) == 13 && LepGood2_tightCharge && ";
    //baseCut += "(nLepGood == 2 || LepGood3_mvaId < -0.2) && LepGood1_charge*LepGood2_charge > 0 && abs(LepGood2_pdgId) != 13 && ";
    //baseCut += "abs(LepGood2_pdgId) == 13 && (nGenLeps + nGenLepsFromTau == 1 && LepGood2_mcMatchId == 1) && ";
    //baseCut += "abs(LepGood2_pdgId) == 13 && LepGood1_mcMatchId == 0 &&  ";
    //baseCut += "LepGood%d_mcMatchAny >= 2 && ";
    TString chargeCutT = " (LepGood%d_tightCharge > (abs(LepGood%d_pdgId) == 11)) && ";
    TString baseCutT = " ";
    baseCutT += "LepGood%d_innerHits*(abs(LepGood%d_pdgId) == 11) == 0 && "; // require to be zero if the lepton is an electron
    baseCutT += "(LepGood%d_convVeto==0)*(abs(LepGood%d_pdgId) == 11) == 0 && ";
    TString moreCut3 = "nJet25 >= 3 && ";
    TString moreCut4 = "nJet25 >= 3 && (nBJetLoose25 >= 2 || nBJetMedium25 >= 1) && ";
    TString moreCut5 = "nJet25 >= 3 && (nBJetLoose25 >= 2) && ";
    TString moreCut6 = "nJet25 >= 3 && (nBJetMedium25 >= 2) && ";

    TString sample = "TTJets";
    const char *samples[7] = { "TTJets", "TTLep", "TtW", "TbartW", "TTJetsLep", "TTJetsSem", "TTJetsHad" };
    for (int id = 0; id < 7; ++id) { 
        sample = TString(samples[id]);
        //fillBaseWeights("W_btag_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_pt > 10 && LepGood%d_mva < 0.25", sample, 4);
        //fillBaseWeights("W_btag_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_pt > 10 && LepGood%d_mva < 0.25", sample, 4);

        std::cout << "Processing MVA selection on " << sample << std::endl;
        fillFR("FR_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.35", sample, 3);
        fillFR("FR_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.35", sample, 3);
        fillFR("FR_tight_el",  baseCutT +              "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tight_mu",  baseCutT +              "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tight2_el", baseCutT + chargeCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight2_mu", baseCutT + chargeCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight3_el", baseCutT + moreCut3 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight3_mu", baseCutT + moreCut3 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight4_el", baseCutT + moreCut4 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight4_mu", baseCutT + moreCut4 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight5_el", baseCutT + moreCut5 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight5_mu", baseCutT + moreCut5 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight6_el", baseCutT + moreCut6 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 2);
        fillFR("FR_tight6_mu", baseCutT + moreCut6 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 2);

        fillFR("FR_loose_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= -0.30", sample, 4);
        fillFR("FR_loose_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.30", sample, 4);

#if 1
        TString tightCB = baseCutT + "LepGood%d_sip3d < 4 && LepGood%d_tightCharge > (abs(LepGood%d_pdgId) == 11) && ";
        fillFR("FRC_tight_el",  tightCB + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11",                      "LepGood%d_relIso < 0.12",  sample, 3);
        fillFR("FRC_tight_mu",  tightCB + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13 && LepGood%d_tightId", "LepGood%d_relIso < 0.12 ", sample, 3);
#endif

#if 0
        std::cout << "Processing cut-based selection on " << sample << std::endl;
        fillFR("FRC_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11 && (abs(LepGood%d_eta)<1.4442 || abs(LepGood%d_eta)>1.5660)", "LepGood%d_relIso03/LepGood%d_pt < 0.2 && LepGood%d_tightId > 0.0 && abs(LepGood%d_dxy) < 0.04 && abs(LepGood%d_innerHits) <= 0", sample, triggering ? 2 : 4);
        fillFR("FRC_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.2", sample, triggering ? 2 : 4);
        fillFR("FRC_tight_el", baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11 && (abs(LepGood%d_eta)<1.4442 || abs(LepGood%d_eta)>1.5660)", "LepGood%d_relIso03/LepGood%d_pt < 0.1 && LepGood%d_tightId > 0.0 && abs(LepGood%d_dxy) < 0.02 && abs(LepGood%d_innerHits) <= 0", sample, triggering ? 2 : 4);
        fillFR("FRC_tight_mu", baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13 && abs(LepGood%d_eta) < 2.1", "LepGood%d_relIso < 0.12 && LepGood%d_tightId   && abs(LepGood%d_dxy) < 0.2 && abs(LepGood%d_dz) < 0.5", sample, triggering ? 2 : 4);
#endif


#if 0
        std::cout << "Processing hybrid selection on " << sample << std::endl;
        fillFR("FRH_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_relIso < 0.25 && LepGood%d_sip3d < 6 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.25 && LepGood%d_sip3d < 6 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_tight_el", baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_relIso < 0.2 && LepGood%d_sip3d < 4 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_tight_mu", baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.2 && LepGood%d_sip3d < 4 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_sip3d < 10 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_sip3d < 10 && LepGood%d_jetBTagCSV < 0.3", sample, 4);
        fillFR("FRH_tight_el", baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_sip3d < 3 && LepGood%d_jetBTagCSV < 20.2", sample, 4);
        fillFR("FRH_tight_mu", baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_sip3d < 3 && LepGood%d_jetBTagCSV < 20.2", sample, 4);
#else
#endif
    }

    fOut->Close();
}
