TString gTreePath = "/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD/%s/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root";
void fillFR(TString hist, TString cut, TString pass, TString compName, int maxLep) {
    TDirectory *root = gDirectory;
    TFile *f = TFile::Open(Form(gTreePath.Data(),compName.Data()));
    TTree *tree = (TTree*) f->Get("ttHLepTreeProducerBase");
    root->cd();
    for (int i = 1; i <= maxLep; ++i) {
        TString mycut = cut;   mycut.ReplaceAll("%d", Form("%d",i));
        TString mypass = pass; mypass.ReplaceAll("%d", Form("%d",i));
        tree->Draw(Form("abs(LepGood%d_eta):min(LepGood%d_pt,79.9)>>+%s_den", i,i, hist.Data()),
                   Form("%s", mycut.Data()));
        tree->Draw(Form("abs(LepGood%d_eta):min(LepGood%d_pt,79.9)>>+%s_num", i,i, hist.Data()),
                   Form("(%s) && (%s)", mycut.Data(), mypass.Data()));
    }
    f->Close();
    TH2 *den = (TH2*) gROOT->FindObject(hist+"_den"); den->Sumw2(); den->Write();
    TH2 *num = (TH2*) gROOT->FindObject(hist+"_num"); num->Sumw2(); num->Write();
    TH2 *ratio = num->Clone(hist);
    ratio->Divide(num,den,1,1,"B");
    ratio->Write();
}

void fillTrivialFakeRatesFromMC(int triggering=1) {
    const int npt_mu = 12, npt_el = 10, neta_mu = 2, neta_el = 3;
    double ptbins_mu[npt_mu+1] = { 5.0, 7.0, 8.5, 10, 12.5, 15, 17.5, 20, 25, 30, 40, 60, 80 };
    double ptbins_el[npt_el+1] = { 7, 8.5, 10, 12.5, 15, 20, 25, 30, 45, 60, 80 };
    double etabins_mu[neta_mu+1] = { 0.0, 1.5,   2.5 };
    double etabins_el[neta_el+1] = { 0.0, 0.8, 1.479, 2.5 };
    const int npt2_mu = 5, npt2_el = 4;
    double ptbins2_mu[npt2_mu+1] = { 5.0, 8.5, 15, 25, 45, 80 };
    double ptbins2_el[npt2_el+1] = {        7, 10, 20, 35, 80 };


    gROOT->ProcessLine(".L ../../python/plotter/fakeRate.cc+");

    TFile *fOut = TFile::Open(triggering ? "fakeRates_TTJets_MC.root" :  "fakeRates_TTJets_MC_NonTrig.root", "RECREATE");
    //TFile *fOut = TFile::Open("fakeRates_TTLep_MC.root", "RECREATE");
    const int  nsels = 1;
    const char *sels[nsels] = { "FR" };
    for (int is = 0; is < nsels; ++is) {
        //TH2F *FR_mu_den = new TH2F(Form("%s_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        //TH2F *FR_mu_num = new TH2F(Form("%s_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        //TH2F *FR_el_den = new TH2F(Form("%s_el_den",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        //TH2F *FR_el_num = new TH2F(Form("%s_el_num",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_loose_el_den = new TH2F(Form("%s_loose_el_den",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_loose_el_num = new TH2F(Form("%s_loose_el_num",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_loose_mu_den = new TH2F(Form("%s_loose_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_loose_mu_num = new TH2F(Form("%s_loose_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tight_el_den = new TH2F(Form("%s_tight_el_den",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tight_el_num = new TH2F(Form("%s_tight_el_num",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tight_mu_den = new TH2F(Form("%s_tight_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tight_mu_num = new TH2F(Form("%s_tight_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);

        TH2F *FR_loose2_mu_den = new TH2F(Form("%s_loose2_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_loose2_mu_num = new TH2F(Form("%s_loose2_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_loose2_el_den = new TH2F(Form("%s_loose2_el_den",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);
        TH2F *FR_loose2_el_num = new TH2F(Form("%s_loose2_el_num",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);
        TH2F *FR_tight2_mu_den = new TH2F(Form("%s_tight2_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tight2_mu_num = new TH2F(Form("%s_tight2_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tight2_el_den = new TH2F(Form("%s_tight2_el_den",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);
        TH2F *FR_tight2_el_num = new TH2F(Form("%s_tight2_el_num",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);

        // SB fakerates
        TH2F *FR_tightSB_el_den = new TH2F(Form("%s_tightSB_el_den",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tightSB_el_num = new TH2F(Form("%s_tightSB_el_num",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tightSB_mu_den = new TH2F(Form("%s_tightSB_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSB_mu_num = new TH2F(Form("%s_tightSB_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSB2_mu_den = new TH2F(Form("%s_tightSB2_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSB2_mu_num = new TH2F(Form("%s_tightSB2_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSB2_el_den = new TH2F(Form("%s_tightSB2_el_den",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);
        TH2F *FR_tightSB2_el_num = new TH2F(Form("%s_tightSB2_el_num",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);

        // FR's with SIP < 4
        TH2F *FR_tightSip4_mu_num = new TH2F(Form("%s_tightSip4_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSip4_mu_den = new TH2F(Form("%s_tightSip4_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSip42_mu_den = new TH2F(Form("%s_tightSip42_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSip42_mu_num = new TH2F(Form("%s_tightSip42_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        // FR's for SUS13
        TH2F *FR_tightSUS13_mu_num = new TH2F(Form("%s_tightSUS13_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSUS13_mu_den = new TH2F(Form("%s_tightSUS13_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSUS13_el_num = new TH2F(Form("%s_tightSUS13_el_num",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tightSUS13_el_den = new TH2F(Form("%s_tightSUS13_el_den",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tightSUS132_el_den = new TH2F(Form("%s_tightSUS132_el_den",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);
        TH2F *FR_tightSUS132_el_num = new TH2F(Form("%s_tightSUS132_el_num",sels[is]),"",npt2_el,ptbins2_el,neta_el,etabins_el);
        TH2F *FR_tightSUS132_mu_den = new TH2F(Form("%s_tightSUS132_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSUS132_mu_num = new TH2F(Form("%s_tightSUS132_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        // FR's for SUS13
        TH2F *FR_tightSUS13C_mu_num = new TH2F(Form("%s_tightSUS13C_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSUS13C_mu_den = new TH2F(Form("%s_tightSUS13C_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightSUS13C_el_num = new TH2F(Form("%s_tightSUS13C_el_num",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);
        TH2F *FR_tightSUS13C_el_den = new TH2F(Form("%s_tightSUS13C_el_den",sels[is]),"",npt_el,ptbins_el,neta_el,etabins_el);

        // Alternate FRs for muons
        TH2F *FR_tightMVA05_mu_num = new TH2F(Form("%s_tightMVA05_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA05_mu_den = new TH2F(Form("%s_tightMVA05_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA052_mu_den = new TH2F(Form("%s_tightMVA052_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA052_mu_num = new TH2F(Form("%s_tightMVA052_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA03_mu_num = new TH2F(Form("%s_tightMVA03_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA03_mu_den = new TH2F(Form("%s_tightMVA03_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA032_mu_den = new TH2F(Form("%s_tightMVA032_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA032_mu_num = new TH2F(Form("%s_tightMVA032_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA00_mu_num = new TH2F(Form("%s_tightMVA00_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA00_mu_den = new TH2F(Form("%s_tightMVA00_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA002_mu_den = new TH2F(Form("%s_tightMVA002_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVA002_mu_num = new TH2F(Form("%s_tightMVA002_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm03_mu_num = new TH2F(Form("%s_tightMVAm03_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm03_mu_den = new TH2F(Form("%s_tightMVAm03_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm032_mu_den = new TH2F(Form("%s_tightMVAm032_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm032_mu_num = new TH2F(Form("%s_tightMVAm032_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm05_mu_num = new TH2F(Form("%s_tightMVAm05_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm05_mu_den = new TH2F(Form("%s_tightMVAm05_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm052_mu_den = new TH2F(Form("%s_tightMVAm052_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm052_mu_num = new TH2F(Form("%s_tightMVAm052_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm07_mu_num = new TH2F(Form("%s_tightMVAm07_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm07_mu_den = new TH2F(Form("%s_tightMVAm07_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm072_mu_den = new TH2F(Form("%s_tightMVAm072_mu_den",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
        TH2F *FR_tightMVAm072_mu_num = new TH2F(Form("%s_tightMVAm072_mu_num",sels[is]),"",npt2_mu,ptbins2_mu,neta_mu,etabins_mu);
    }

    TString baseCut = " ";
    if (triggering) baseCut += "nLepGood == 2 && ";
    baseCut += "minMllAFAS > 12 && ";
    baseCut += " (nLepGood > 2 || (nJet25 >= 3 && LepGood1_pdgId*LepGood2_pdgId > 0)) && ";
    TString chargeCutT = " (LepGood%d_tightCharge > (abs(LepGood%d_pdgId) == 11)) && ";
    TString baseCutT = baseCut;
    baseCutT += "LepGood%d_innerHits*(abs(LepGood%d_pdgId) == 11) == 0 && "; // require to be zero if the lepton is an electron
    baseCutT += "(LepGood%d_convVeto==0)*(abs(LepGood%d_pdgId) == 11) == 0 && ";
    //baseCutT += "(abs(LepGood%d_pdgId) == 11 || LepGood%d_tightId) && ";
    baseCutT += chargeCutT;

    TString moreCut2 = "nBJetMedium25 >= 2 &&";
    TString sip4cut  = "LepGood%d_sip3d <  4 && ";
    TString sus13_el = "passEgammaTightMVA(LepGood%d_pt,LepGood%d_eta,LepGood%d_tightId) && abs(LepGood%d_dxy) < 0.0100  && abs(LepGood%d_dz) < 0.1 && (abs(LepGood%d_eta) < 1.4442 || abs(LepGood%d_eta) > 1.566) && ";
    TString sus13_mu = "LepGood%d_tightId && LepGood%d_tightCharge > 0  && abs(LepGood%d_dxy) < 0.0050 && abs(LepGood%d_dz) < 0.1 && ";

    TString SBden = "(LepGood%d_mva > -0.7 && LepGood%d_mva < 0.5 || LepGood%d_mva > 0.7) && ";

    TString sample = "TTJets";
    const char *samples[7] = { "TTJets", "TTLep", "TtW", "TbartW", "TTJetsLep", "TTJetsSem", "TTJetsHad" };
    for (int id = 0; id < 7; ++id) { 
        sample = TString(samples[id]);
        //if (sample != "T?TJetsSem") continue;
        //fillBaseWeights(?"W_btag_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_pt > 10 && LepGood%d_mva < 0.25", sample, 4);
        //fillBaseWeights(?"W_btag_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_pt > 10 && LepGood%d_mva < 0.25", sample, 4);

        std::cout << "Processing MVA selection on " << sample << std::endl;
#if 0
        //fillFR("FR_el",       baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= -0.3", sample, 3);
        //fillFR("FR_mu",       baseCutT + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.3", sample, 3);
        fillFR("FR_loose_el", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= -0.30", sample, 4);
        fillFR("FR_loose_mu", baseCut + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.30", sample, 4);
        fillFR("FR_tight_mu",  baseCutT +             "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tight_el",  baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tight2_el", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tight2_mu", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_loose2_el", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= -0.3", sample, 3);
        fillFR("FR_loose2_mu", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.3", sample, 3);
#endif

#if 0
        fillFR("FR_tightSB_mu",  baseCutT + SBden +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tightSB_el",  baseCutT + SBden +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tightSB2_el", baseCutT + SBden + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tightSB2_mu", baseCutT + SBden + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
#endif

#if 0
        fillFR("FR_tightSip4_mu",  baseCutT + sip4cut +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
        fillFR("FR_tightSip42_mu", baseCutT + sip4cut + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.70", sample, 3);
#endif
#if 0
        fillFR("FR_tightSUS13_mu",  baseCutT + sus13_mu +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.1", sample, 3);
        fillFR("FR_tightSUS13_el",  baseCutT + sus13_el +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_relIso < 0.1", sample, 3);
        fillFR("FR_tightSUS132_el", baseCutT + sus13_mu + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11", "LepGood%d_relIso < 0.1", sample, 3);
        fillFR("FR_tightSUS132_mu", baseCutT + sus13_el + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_relIso < 0.1", sample, 3);
#endif
#if 0
        fillFR("FR_tightSUS13C_mu",  baseCutT + sus13_mu +  "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13",  "LepGood%d_chargedIso/LepGood%d_pt < 0.05", sample, 3);
        fillFR("FR_tightSUS13C_el",  baseCutT + sus13_el +  "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 11",  "LepGood%d_chargedIso/LepGood%d_pt < 0.05", sample, 3);
#endif
#if 1
        fillFR("FR_tightMVA05_mu",   baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.50", sample, 3);
        fillFR("FR_tightMVA052_mu",  baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.50", sample, 3);
        fillFR("FR_tightMVA03_mu",   baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.30", sample, 3);
        fillFR("FR_tightMVA032_mu",  baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.30", sample, 3);
        fillFR("FR_tightMVA00_mu",   baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.00", sample, 3);
        fillFR("FR_tightMVA002_mu",  baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= 0.00", sample, 3);
        fillFR("FR_tightMVAm03_mu",  baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.3", sample, 3);
        fillFR("FR_tightMVAm032_mu", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.3", sample, 3);
        fillFR("FR_tightMVAm05_mu",  baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.5", sample, 3);
        fillFR("FR_tightMVAm052_mu", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.5", sample, 3);
        fillFR("FR_tightMVAm07_mu",  baseCutT +            "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.7", sample, 3);
        fillFR("FR_tightMVAm072_mu", baseCutT + moreCut2 + "LepGood%d_mcMatchId == 0 && abs(LepGood%d_pdgId) == 13", "LepGood%d_mva >= -0.7", sample, 3);
#endif


    }

    fOut->Close();
}
