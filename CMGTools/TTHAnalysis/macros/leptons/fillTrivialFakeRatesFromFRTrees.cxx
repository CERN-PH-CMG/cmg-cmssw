TString gTreePath = "/afs/cern.ch/user/g/gpetrucc/w/ttH/TREES_250513_FR/%s/ttHLepFRAnalyzer/ttHLepFRAnalyzer_tree.root";
void fillFR(TString hist, TString cut, TString pass, TString compName) {
    TDirectory *root = gDirectory;
    TFile *f = TFile::Open(Form(gTreePath.Data(),compName.Data()));
    TTree *tree = (TTree*) f->Get("ttHLepFRAnalyzer");
    root->cd();
    tree->Draw(Form("abs(Probe_eta):min(Probe_pt,99.9)>>+%s_den", hist.Data()),
               cut.Data());
    tree->Draw(Form("abs(Probe_eta):min(Probe_pt,99.9)>>+%s_num", hist.Data()),
               Form("(%s) && (%s)", cut.Data(), pass.Data()));
    f->Close();
    TH2 *den = (TH2*) gROOT->FindObject(hist+"_den"); den->Sumw2(); den->Write();
    TH2 *num = (TH2*) gROOT->FindObject(hist+"_num"); num->Sumw2(); num->Write();
    TH2 *ratio = num->Clone(hist);
    ratio->Divide(num,den,1,1,"B");
    ratio->Write();
}

void fillTrivialFakeRatesFromFRTrees(int triggering=1) {
    const int npt_mu = 8, npt_el = 7, neta = 2;
    const int npt_muL = 8, npt_elL = 6;
    double ptbins_mu[npt_mu+1] = { 5.0, 7.0, 8.5, 10, 15, 20, 25, 35, 80 };
    double ptbins_el[npt_el+1] = {        7, 8.5, 10, 15, 20, 25, 35, 80 };
    double etabins_mu[neta+1] = { 0.0, 1.5,   2.5 };
    double etabins_el[neta+1] = { 0.0, 1.479, 2.5 };
    //double etabins_mu[neta+1] = { 0.0, 0.7, 1.5,   2.0,  2.5 };
    //double etabins_el[neta+1] = { 0.0, 0.7, 1.479, 2.0,  2.5 };

    TString fileName = "";
    switch(triggering) {
        case 0: fileName = "fakeRates_QCDMu_MC_NonTrig.root"; break;
        case 1: fileName = "fakeRates_QCDMu_MC.root"; break;
        case 2: fileName = "fakeRates_QCDMu_MC_SingleMu.root"; break;
    }
    TFile *fOut = TFile::Open(fileName, "RECREATE");
    const int  nsels = 3;
    const char *sels[nsels] = { "FR", "FRC", "FRH" };
    for (int is = 0; is < nsels; ++is) {
        //TH2F *FR_mu_den = new TH2F(Form("%s_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        //TH2F *FR_mu_num = new TH2F(Form("%s_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight_mu_den = new TH2F(Form("%s_tight_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_tight_mu_num = new TH2F(Form("%s_tight_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_loose_mu_den = new TH2F(Form("%s_loose_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_loose_mu_num = new TH2F(Form("%s_loose_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        //TH2F *FR_el_den = new TH2F(Form("%s_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        //TH2F *FR_el_num = new TH2F(Form("%s_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight_el_den = new TH2F(Form("%s_tight_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_tight_el_num = new TH2F(Form("%s_tight_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_loose_el_den = new TH2F(Form("%s_loose_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_loose_el_num = new TH2F(Form("%s_loose_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        //TH2F *FRj_mu_den = new TH2F(Form("%sj_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        //TH2F *FRj_mu_num = new TH2F(Form("%sj_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FRj_tight_mu_den = new TH2F(Form("%sj_tight_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FRj_tight_mu_num = new TH2F(Form("%sj_tight_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FRj_loose_mu_den = new TH2F(Form("%sj_loose_mu_den",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FRj_loose_mu_num = new TH2F(Form("%sj_loose_mu_num",sels[is]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FRj_tight_el_den = new TH2F(Form("%sj_tight_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FRj_tight_el_num = new TH2F(Form("%sj_tight_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FRj_loose_el_den = new TH2F(Form("%sj_loose_el_den",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FRj_loose_el_num = new TH2F(Form("%sj_loose_el_num",sels[is]),"",npt_el,ptbins_el,neta,etabins_el);
    }
    const int ntrig1mu = 5;
    const int trig1mu[ntrig1mu] = { 8, 12, 17, 24, 40 };
    for (int it = 0; it < ntrig1mu; ++it) {
        TH2F *FR_mu_den = new TH2F(Form("%s_TagMu%d_mu_den",sels[0],trig1mu[it]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_mu_num = new TH2F(Form("%s_TagMu%d_mu_num",sels[0],trig1mu[it]),"",npt_mu,ptbins_mu,neta,etabins_mu);
        TH2F *FR_el_den = new TH2F(Form("%s_TagMu%d_el_den",sels[0],trig1mu[it]),"",npt_el,ptbins_el,neta,etabins_el);
        TH2F *FR_el_num = new TH2F(Form("%s_TagMu%d_el_num",sels[0],trig1mu[it]),"",npt_el,ptbins_el,neta,etabins_el);
    }

    TString baseCut = "Probe_mcMatchId == 0 && ";
    //baseCut += "Probe_mcMatchAny >= 2 && ";

    TString baseCutL = baseCut + "tagType == 13 && ";
    //baseCutL += "TagLepton_sip3d > 7 && ";
    //baseCutL += "Probe_pt/(TagLepton_pt*(1+TagLepton_relIso)) < 1 && abs(dphi_tp) > 2.0 && ";
    baseCutL += "Probe_pt/(TagLepton_pt*(1+TagLepton_relIso)) < 1.5 && abs(dphi_tp) > 1.0 && ";
    //baseCutL += "(abs(Probe_pdgId) == 11 || Probe_pt < 14 || TagLepton_pt > 16) && ";
    TString baseCutJ = baseCut + "tagType == 1 && ";
    baseCutJ += "Probe_pt/TagJet_pt < 1 && abs(dphi_tp) > 2.0 && ";

    if (triggering == 1) {
        baseCutL += "(Trig_Pair_2Mu || Trig_Pair_MuEG) &&";
        //baseCutJ += "(Trig_Probe_Mu5 || Trig_Event_Mu40 || Trig_Probe_1ElL) &&";
        baseCutJ += "(Trig_Probe_Mu5 || Trig_Probe_1ElT || Trig_Event_Mu40) &&";
    }

    TString tightC = ""; //"(Probe_tightCharge > (abs(Probe_pdgId) == 11)) && ";
    tightC += "Probe_innerHits*(abs(Probe_pdgId) == 11) == 0 && "; // require to be zero if the lepton is an electron
    tightC += "(Probe_convVeto==0)*(abs(Probe_pdgId) == 11) == 0 && "; // require to be zero if the lepton is an electron
    //fillFR("FR_el",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >= 0.35", "QCDMuPt15");
    //fillFR("FR_mu",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >= 0.35", "QCDMuPt15");
    fillFR("FR_loose_el", baseCutL +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >= -0.30", "QCDMuPt15");
    fillFR("FR_loose_mu", baseCutL +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >= -0.30", "QCDMuPt15");
    fillFR("FR_tight_el", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >= 0.70", "QCDMuPt15");
    fillFR("FR_tight_mu", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >= 0.70", "QCDMuPt15");

    //fillFR("FRj_mu",       baseCutJ +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >=  0.35", "QCDMuPt15");
    //fillFR("FRj_mu",       baseCutJ +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >=  0.35", "QCDMuPt15");
    fillFR("FRj_loose_mu", baseCutJ +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >= -0.30", "QCDMuPt15");
    fillFR("FRj_loose_el", baseCutJ +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >= -0.30", "QCDElPt30To80");
    fillFR("FRj_loose_el", baseCutJ +          "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >= -0.30", "QCDElPt80To170");
    fillFR("FRj_tight_mu", baseCutJ + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >=  0.70", "QCDMuPt15");
    fillFR("FRj_tight_el", baseCutJ + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >=  0.70", "QCDElPt30To80");
    fillFR("FRj_tight_el", baseCutJ + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >=  0.70", "QCDElPt80To170");

#if 1
    TString tightCB = tightC + "Probe_sip3d < 4 && Probe_tightCharge > (abs(Probe_pdgId) == 11) && ";
    fillFR("FRC_tight_el",  baseCutL + tightCB + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11",                  "Probe_relIso < 0.12",  "QCDMuPt15");
    fillFR("FRC_tight_mu",  baseCutL + tightCB + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13 && Probe_tightId", "Probe_relIso < 0.12 ", "QCDMuPt15");
    fillFR("FRCj_tight_el", baseCutJ + tightCB + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11",                  "Probe_relIso < 0.12",  "QCDMuPt15");
    fillFR("FRCj_tight_mu", baseCutJ + tightCB + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13 && Probe_tightId", "Probe_relIso < 0.12 ", "QCDMuPt15");
#endif
#if 0
    fillFR("FRC_el",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11 && (abs(Probe_eta)<1.4442 || abs(Probe_eta)>1.5660)", "Probe_relIso03/Probe_pt < 0.25 && Probe_tightId > 0.0 && abs(Probe_dxy) < 0.04 && abs(Probe_innerHits) <= 0", "QCDMuPt15");
    fillFR("FRC_mu",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_relIso < 0.2", "QCDMuPt15");
    fillFR("FRC_tight_el", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11 && (abs(Probe_eta)<1.4442 || abs(Probe_eta)>1.5660)", "Probe_relIso03/Probe_pt < 0.1 && Probe_tightId > 0 && abs(Probe_dxy) < 0.02 && abs(Probe_innerHits) <= 0", "QCDMuPt15");
    fillFR("FRC_tight_mu", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13 && abs(Probe_eta) < 2.1", "Probe_relIso < 0.12 && Probe_tightId && abs(Probe_dxy) < 0.2 && abs(Probe_dz) < 0.5", "QCDMuPt15");

    //fillFR("FRCj_mu",       baseCutJ + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_relIso < 0.2", "QCDMuPt15");
    //fillFR("FRCj_tight_mu", baseCutJ + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13 && abs(Probe_eta) < 2.1", "Probe_relIso < 0.12 && Probe_tightId", "QCDMuPt15");
#endif

#if 0
    for (int it = 0; it < ntrig1mu; ++it) {
        TString trigCut = TString::Format("Trig_Tag_Mu%d && ", trig1mu[it]);
        fillFR(Form("FR_TagMu%d_el",trig1mu[it]), baseCutL + trigCut + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_mva >= 0.35", "QCDMuPt15");
        fillFR(Form("FR_TagMu%d_mu",trig1mu[it]), baseCutL + trigCut + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_mva >= 0.35", "QCDMuPt15");
    }
#endif

#if 0
    fillFR("FRH_el",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_relIso < 0.25 && Probe_sip3d < 6 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRH_mu",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_relIso < 0.25 && Probe_sip3d < 6 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRH_tight_el", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", "Probe_relIso < 0.2 && Probe_sip3d < 4 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRH_tight_mu", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_relIso < 0.2 && Probe_sip3d < 4 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");

    fillFR("FRHj_mu",       baseCutJ + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_relIso < 0.2 && Probe_sip3d < 6 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRHj_tight_mu", baseCutJ + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", "Probe_relIso < 0.25 && Probe_sip3d < 4 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRH_el",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", " Probe_sip3d < 10 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRH_mu",       baseCutL + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", " Probe_sip3d < 10 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRH_tight_el", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 11", " Probe_sip3d < 10 && Probe_jetBTagCSV < 0.2", "QCDMuPt15");
    fillFR("FRH_tight_mu", baseCutL + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", " Probe_sip3d < 10 && Probe_jetBTagCSV < 0.2", "QCDMuPt15");
    fillFR("FRHj_mu",       baseCutJ + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", " Probe_sip3d < 10 && Probe_jetBTagCSV < 0.3", "QCDMuPt15");
    fillFR("FRHj_tight_mu", baseCutJ + tightC + "Probe_mcMatchId == 0 && abs(Probe_pdgId) == 13", " Probe_sip3d < 10 && Probe_jetBTagCSV < 0.2", "QCDMuPt15");
#else
#endif


    fOut->Close();
}
