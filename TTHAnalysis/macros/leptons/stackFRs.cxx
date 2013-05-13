TString gPrefix = "";
int neta = 2;

#if 0
TH2 *FR_tight_ttl_el = (TH2*) TFile::Open("../../data/fakerate/FR_ttl2lss_e.root")->Get("FR_e");
TH2 *FR_tight_ttl_mu = (TH2*) TFile::Open("../../data/fakerate/FR_ttl2lss_m.root")->Get("FR_m");
TH2 *FR_ttl_el = (TH2*) TFile::Open("../../data/fakerate/FR_ttl_e.root")->Get("FR_e");
TH2 *FR_ttl_mu = (TH2*) TFile::Open("../../data/fakerate/FR_ttl_m.root")->Get("FR_m");
TH2 *FR_zl_el = (TH2*) TFile::Open("../../data/fakerate/FR_zl_e.root")->Get("FR_e");
TH2 *FR_zl_mu = (TH2*) TFile::Open("../../data/fakerate/FR_zl_m.root")->Get("FR_m");
TH2 *FR_zlw_el = (TH2*) TFile::Open("../../data/fakerate/FR_zlw_e.root")->Get("FR_e");
TH2 *FR_zlw_mu = (TH2*) TFile::Open("../../data/fakerate/FR_zlw_m.root")->Get("FR_m");
#else
TH2 *FR_tight_ttl_el = 0;
TH2 *FR_tight_ttl_mu = 0;
TH2 *FR_ttl_el = 0;
TH2 *FR_ttl_mu = 0;
TH2 *FR_zl_el = 0;
TH2 *FR_zl_mu = 0;
TH2 *FR_tight_zl_el = 0;
TH2 *FR_tight_zl_mu = 0;
TH2 *FR_zlw_el = 0;
TH2 *FR_zlw_mu = 0;
#endif

TH2 *FR_tight_qcd_el = 0;
TH2 *FR_tight_qcd_mu = 0;
TH2 *FR_qcd_el = 0;
TH2 *FR_qcd_mu = 0;
TH2 *FR_tight_qcdj_mu = 0;
TH2 *FR_qcdj_mu = 0;
TH2 *d_FR_tight_qcd_el = 0;
TH2 *d_FR_tight_qcd_mu = 0;
TH2 *d_FR_qcd_el = 0;
TH2 *d_FR_qcd_mu = 0;

TH2 *d_FR_tight_zl_el = 0;
TH2 *d_FR_tight_zl_mu = 0;
TH2 *d_FR_zl_el = 0;
TH2 *d_FR_zl_mu = 0;

const int ntrig1mu = 5;
const int trig1mu[ntrig1mu] = { 8, 12, 17, 24, 40 };
TH2 *FR_qcd1mu_mu[ntrig1mu], *FR_qcd1mu_el[ntrig1mu];

enum LType { EL=0, MU=1, NLTypes = 2 };
enum FRType { LOOSE=0, TIGHT=1, FRTypes=2 };
TH2 *FR_ttl[FRTypes][NLTypes];
TH2 *FR_zl[FRTypes][NLTypes];
TH2 *FR_qcd[FRTypes][NLTypes];
TH2 *d_FR_zl[FRTypes][NLTypes];
TH2 *d_FR_qcd[FRTypes][NLTypes];

void loadData(TString iPrefix, int trig) {
    TFile *fMC = TFile::Open(trig == 1 ? "fakeRates_TTJets_MC.root" : "fakeRates_TTJets_MC_NonTrig.root" );
    FR_tight_ttl_el = (TH2*) fMC->Get(iPrefix+"_tight_el");
    FR_tight_ttl_mu = (TH2*) fMC->Get(iPrefix+"_tight_mu");
    FR_ttl_el = (TH2*) fMC->Get(iPrefix+"_el");
    FR_ttl_mu = (TH2*) fMC->Get(iPrefix+"_mu");

    TFile *fQCD = TFile::Open(trig == 1 ? "fakeRates_QCDMu_MC.root" : "fakeRates_QCDMu_MC_NonTrig.root");
    FR_tight_qcd_el = (TH2*) fQCD->Get(iPrefix+"_tight_el");
    FR_tight_qcd_mu = (TH2*) fQCD->Get(iPrefix+"_tight_mu");
    FR_qcd_el = (TH2*) fQCD->Get(iPrefix+"_el");
    FR_qcd_mu = (TH2*) fQCD->Get(iPrefix+"_mu");
    FR_tight_qcdj_mu = (TH2*) fQCD->Get(iPrefix+"j_tight_mu");
    FR_qcdj_mu = (TH2*) fQCD->Get(iPrefix+"j_mu");

    TFile *fQCD1Mu = TFile::Open("fakeRates_QCDMu_MC_SingleMu.root");
    for (int it = 0; it < ntrig1mu; ++it) {
        FR_qcd1mu_mu[it] = (TH2*) fQCD1Mu->Get(Form("%s_TagMu%d_mu", iPrefix.Data(), trig1mu[it]));
        FR_qcd1mu_el[it] = (TH2*) fQCD1Mu->Get(Form("%s_TagMu%d_el", iPrefix.Data(), trig1mu[it]));
    }

    TFile *fDQCD = 0;
    if (iPrefix == "FR")  fDQCD = TFile::Open(trig == 1 ? "frFitsSimple.root"         : "frFitsSimple_TagMuL.root");
    if (iPrefix == "FRC") fDQCD = TFile::Open(trig == 1 ? "frFitsSimpleCutBased.root" : "frFitsSimpleCutBased_TagMuL.root");
    d_FR_tight_qcd_el = (TH2*) fDQCD->Get("FR_tight_el");
    d_FR_tight_qcd_mu = (TH2*) fDQCD->Get("FR_tight_mu");
    d_FR_qcd_el = (TH2*) fDQCD->Get("FR_el");
    d_FR_qcd_mu = (TH2*) fDQCD->Get("FR_mu");

    TFile *fZMC = TFile::Open("fakeRates_DYJets_MC.root");
    if (0 && fZMC) {
        FR_zl_el = (TH2*) fZMC->Get(iPrefix+"_el");
        FR_zl_mu = (TH2*) fZMC->Get(iPrefix+"_mu");
        FR_tight_zl_el = (TH2*) fZMC->Get(iPrefix+"_tight_el");
        FR_tight_zl_mu = (TH2*) fZMC->Get(iPrefix+"_tight_mu");
    }

    TFile *fZ3lData = TFile::Open("fakeRates_Z3l_Data.root");
    if (0 && fZ3lData) {
        d_FR_tight_zl_el = (TH2*) fZ3lData->Get(iPrefix+"_tight_el");
        d_FR_tight_zl_mu = (TH2*) fZ3lData->Get(iPrefix+"_tight_mu");
        d_FR_zl_el = (TH2*) fZ3lData->Get(iPrefix+"_el");
        d_FR_zl_mu = (TH2*) fZ3lData->Get(iPrefix+"_mu");
    }
    FR_ttl[LOOSE][EL] = FR_ttl_el; FR_ttl[LOOSE][MU] = FR_ttl_mu; FR_ttl[TIGHT][EL] = FR_tight_ttl_el; FR_ttl[TIGHT][MU] = FR_tight_ttl_mu;
    FR_qcd[LOOSE][EL] = FR_qcd_el; FR_qcd[LOOSE][MU] = FR_qcd_mu; FR_qcd[TIGHT][EL] = FR_tight_qcd_el; FR_qcd[TIGHT][MU] = FR_tight_qcd_mu;
    FR_zl[LOOSE][EL] = FR_zl_el; FR_zl[LOOSE][MU] = FR_zl_mu; FR_zl[TIGHT][EL] = FR_tight_zl_el; FR_zl[TIGHT][MU] = FR_tight_zl_mu;
    d_FR_qcd[LOOSE][EL] = d_FR_qcd_el; d_FR_qcd[LOOSE][MU] = d_FR_qcd_mu; d_FR_qcd[TIGHT][EL] = d_FR_tight_qcd_el; d_FR_qcd[TIGHT][MU] = d_FR_tight_qcd_mu;
    d_FR_zl[LOOSE][EL] = d_FR_zl_el; d_FR_zl[LOOSE][MU] = d_FR_zl_mu; d_FR_zl[TIGHT][EL] = d_FR_tight_zl_el; d_FR_zl[TIGHT][MU] = d_FR_tight_zl_mu;
}

void cmsprelim(double x1=0.75, double y1=0.40, double x2=0.95, double y2=0.48, const int align=12, const char *text="CMS Preliminary", float textSize=0.033) { 
    TPaveText *cmsprel = new TPaveText(x1,y1,x2,y2,"NDC");
    cmsprel->SetTextSize(textSize);
    cmsprel->SetFillColor(0);
    cmsprel->SetFillStyle(0);
    cmsprel->SetLineStyle(0);
    cmsprel->SetLineColor(0);
    cmsprel->SetTextAlign(align);
    cmsprel->SetTextFont(42);
    cmsprel->AddText(text);
    cmsprel->Draw("same");
}



TGraph* drawSlice(TH2 *h2d, int ieta, int color, int ist=0, int ifill=0) {
    if (h2d == 0) return 0;
    TGraphAsymmErrors *ret = new TGraphAsymmErrors(h2d->GetNbinsX());
    TAxis *ax = h2d->GetXaxis();
    for (int i = 0, n = ret->GetN(); i < n; ++i) {
        ret->SetPoint(i, ax->GetBinCenter(i+1), h2d->GetBinContent(i+1,ieta+1));
        if (ist == 2) {
            ret->SetPointError(i, 0.5*ax->GetBinWidth(i+1), 0.5*ax->GetBinWidth(i+1),
                                  0.4*h2d->GetBinContent(i+1,ieta+1), 0.4*h2d->GetBinContent(i+1,ieta+1));
        } else {
            ret->SetPointError(i, 0.5*ax->GetBinWidth(i+1), 0.5*ax->GetBinWidth(i+1),
                                  h2d->GetBinError(i+1,ieta+1), h2d->GetBinError(i+1,ieta+1));
        }
    }
    ret->SetLineColor(color);
    ret->SetMarkerColor(color);
    if (ist) {
        ret->SetLineWidth(1);
        ret->SetFillColor(color);
        ret->SetFillStyle(ifill ? ifill : (ist == 1 ? 3004 : 1001));
        ret->Draw("E5 SAME");
        ret->Draw("E2 SAME");
    } else {
        ret->SetLineWidth(2);
        ret->Draw("P SAME");
    }
    return ret;
}


TCanvas *c1 = 0;
TH1 *frame = 0;
TLegend *newLeg(double x1, double y1, double x2, double y2, double textSize=0.035) {
    TLegend *ret = new TLegend(x1,y1,x2,y2);
    ret->SetTextFont(42);
    ret->SetFillColor(0);
    ret->SetShadowColor(0);
    ret->SetTextSize(textSize);
    return ret;
}

const char *ETALBL4[4] = { "b1", "b2", "e1", "e2" };
const char *ETALBL2[2] = { "b", "e" };
const char *ietalbl(int ieta) {
    if (neta == 4) return ETALBL4[ieta];
    else return ETALBL2[ieta];
}

void stackFRs(LType lep, FRType wp, int ieta, int idata=0) {
    frame->Draw();
    TLegend *leg = newLeg(.27,.7,.49,.9);
    TGraph *ttl  = drawSlice(FR_ttl[wp][lep], ieta, 64, 1, 1001);
    TGraph *zl  = drawSlice(FR_zl[wp][lep],  ieta, 222, 1, 3004);
    TGraph *qcd = drawSlice(FR_qcd[wp][lep], ieta, 213, 1, 3345);
    //TGraph *ttl4 = drawSlice(FR_ttl[wp][lep], ieta, 64, 2);
    leg->AddEntry(ttl,  "MC tt+l", "F");
    //leg->AddEntry(ttl4, "  #pm 40%", "LF");
    if (zl) leg->AddEntry(zl, "MC Z+b+l", "LF");
    leg->AddEntry(qcd, "MC qcd #mu", "F");
    //leg->AddEntry(drawSlice(FR_qcdj[wp][lep], ieta, 209), "MC qcd j", "LP");
    leg->Draw();
    if (idata) {
        TLegend *leg = newLeg(.55,.8,.87,.9);
        TGraph *dq = drawSlice(d_FR_qcd[wp][lep], ieta, 1);
        TGraph *dz = drawSlice(d_FR_zl[wp][lep], ieta, 209);
        if (dq) leg->AddEntry(dq, "Data qcd #mu", "LPE");
        if (dz) leg->AddEntry(dz, "Data Z+b+l #mu", "LPE");
        leg->Draw();
        cmsprelim(.55, .73, .87, .79, 22, Form("|#eta| %s %s", ieta ? ">" : "<", lep==EL ? "1.479" : "1.5"), 0.045);
    } else {
        cmsprelim(.55, .8, .87, .88, 22, Form("|#eta| %s %s", ieta ? ">" : "<", lep==EL ? "1.479" : "1.5"), 0.045);
    }
    cmsprelim(.21, .945, .40, .995, 12, "CMS Preliminary"); 
    cmsprelim(.48, .945, .96, .995, 32, "#sqrt{s} = 8 TeV, L = 19.6 fb^{-1}");
    const char *pName[NLTypes] = { "el", "mu" };
    const char *wpName[FRTypes] = { "", "_tight" };
    c1->Print(Form("ttH_plots/270413/FR_QCD_Simple/stacks/%s/%s%s_%s.png", gPrefix.Data(), pName[lep], wpName[wp], ietalbl(ieta)));
    c1->Print(Form("ttH_plots/270413/FR_QCD_Simple/stacks/%s/%s%s_%s.pdf", gPrefix.Data(), pName[lep], wpName[wp], ietalbl(ieta)));
}

void stackFRsMu(int ieta, int idata=0) {
    stackFRs(MU, LOOSE, ieta, idata);
}

void stackFRsEl(int ieta, int idata=0) {
    stackFRs(EL, LOOSE, ieta, idata);
} 

void stackFRsMuT(int ieta, int idata=0) {
    stackFRs(MU, TIGHT, ieta, idata);
}

void stackFRsElT(int ieta, int idata=0) {
    stackFRs(EL, TIGHT, ieta, idata);
}

void stackFRs1MuMu(int ieta, int idata=0) {
    frame->Draw();
    TLegend *leg = newLeg(.2,.65,.42,.9);
    //leg->AddEntry(drawSlice(FR_ttl_mu, ieta, 4, true), "MC tt+l", "LF");
    leg->AddEntry(drawSlice(FR_qcd_mu, ieta, 1, true), "MC NoT", "LP");
    for (int it = 0; it < ntrig1mu; ++it) {
        leg->AddEntry(drawSlice(FR_qcd1mu_mu[it], ieta, 206+4*it), Form("MC Mu%d",trig1mu[it]), "LP");
    }
    leg->Draw();
    if (idata) {
        TLegend *leg = newLeg(.55,.8,.87,.9);
        //leg->AddEntry(drawSlice(d_FR_qcd_mu, ieta, 1), "Data qcd #mu", "LP");
        leg->Draw();
    }
    c1->Print(Form("ttH_plots/270413/FR_QCD_Simple/stacks/%s/mu_%s_1mu.png", gPrefix.Data(), ietalbl(ieta)));
    c1->Print(Form("ttH_plots/270413/FR_QCD_Simple/stacks/%s/mu_%s_1mu.pdf", gPrefix.Data(), ietalbl(ieta)));
}

void stackFRs1MuEl(int ieta, int idata=0) {
    frame->Draw();
    TLegend *leg = newLeg(.2,.65,.42,.9);
    leg->AddEntry(drawSlice(FR_qcd_el, ieta, 1, true), "MC NoT", "LP");
    for (int it = 0; it < ntrig1mu; ++it) {
        leg->AddEntry(drawSlice(FR_qcd1mu_el[it], ieta, 206+4*it), Form("MC Mu%d",trig1mu[it]), "LP");
    }
    leg->Draw();
    if (idata) {
        TLegend *leg = newLeg(.55,.8,.87,.9);
        //leg->AddEntry(drawSlice(d_FR_qcd_el, ieta, 1), "Data qcd #mu", "LP");
        leg->Draw();
    }
    c1->Print(Form("ttH_plots/270413/FR_QCD_Simple/stacks/%s/el_%s_1mu.png", gPrefix.Data(), ietalbl(ieta)));
    c1->Print(Form("ttH_plots/270413/FR_QCD_Simple/stacks/%s/el_%s_1mu.pdf", gPrefix.Data(), ietalbl(ieta)));
}

void initCanvas(double w=600, double h=600, double lm=0.21, double rm=0.04) {
    gStyle->SetCanvasDefW(w); //Width of canvas
    gStyle->SetPaperSize(w/h*20.,20.);
    c1 = new TCanvas("c1","c1");
    c1->cd();
    c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));
    c1->SetLeftMargin(lm); 
    c1->SetRightMargin(rm);
    c1->SetGridy(0); c1->SetGridx(0);
}



void stackFRs(int what=0, int idata=0, int itrigg=1) {
    switch (what) {
        case 0: loadData("FR",itrigg);  gPrefix = "";           break;
        case 1: loadData("FRC",itrigg); gPrefix = "cut_based/"; break;
        case 2: loadData("FRH",itrigg); gPrefix = "hybrid/";    break;
    }
    if (idata) gPrefix += "with_data/";
    switch (itrigg) {
        case 0: gPrefix += "non_trig/"; break;
        case 1: break;
        case 2: gPrefix += "tag_trig/"; break;
    }

    gSystem->Exec("mkdir -p ttH_plots/270413/FR_QCD_Simple/stacks/"+gPrefix);
    gROOT->ProcessLine(".x ~gpetrucc/cpp/tdrstyle.cc");
    gStyle->SetOptStat(0);
    initCanvas();
    c1->SetLogx(1);
    frame = new TH1F("frame",";p_{T} (GeV);Fake rate",100,5, itrigg ? 80 : 80);
    frame->GetYaxis()->SetRangeUser(0.,1.);
    frame->GetYaxis()->SetDecimals(1);
    frame->GetXaxis()->SetDecimals(1);
    frame->GetXaxis()->SetMoreLogLabels(1);
    frame->GetXaxis()->SetLabelOffset(-0.01);
    frame->GetYaxis()->SetTitleOffset(1.40);
    for (int i = 0; i < neta; ++i) {
        frame->GetXaxis()->SetRangeUser(5,80);
        frame->GetYaxis()->SetRangeUser(0.0,0.6);
        frame->GetYaxis()->SetTitle("fake rate (#mu, loose WP)");
        stackFRsMu(i, idata);
        frame->GetYaxis()->SetTitle("fake rate (e, loose WP)");
        stackFRsEl(i, idata);
        //frame->GetYaxis()->SetRangeUser(0.0,0.6);
        //stackFRs1MuMu(i, idata);
        //stackFRs1MuEl(i, idata);
        frame->GetXaxis()->SetRangeUser(10,80);
        frame->GetYaxis()->SetRangeUser(0.0,0.3);
        frame->GetYaxis()->SetTitle("fake rate (#mu, tight WP)");
        stackFRsMuT(i, idata);
        frame->GetYaxis()->SetTitle("fake rate (e, tight WP)");
        stackFRsElT(i, idata);
    }
}
