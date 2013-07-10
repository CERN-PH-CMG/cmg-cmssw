TString gPrefix = "";

const int ntrig1mu = 5;
const int trig1mu[ntrig1mu] = { 8, 12, 17, 24, 40 };
TH2 *FR_qcd1mu_mu[ntrig1mu], *FR_qcd1mu_el[ntrig1mu];

enum LType { EL=0, MU=1, NLTypes = 2 };
enum FRType { MEDIUM=0, TIGHT=1, LOOSE=2, TEST=3, TEST3=4, TEST4=5, TEST5=6, TEST6=7, TESTA=8, TESTB=9, TESTA2=10, TESTB2=11, FRTypes=12 };
TH2 *FR_ttl[FRTypes][NLTypes];
TH2 *FR_zl[FRTypes][NLTypes];
TH2 *FR_zb[FRTypes][NLTypes];
TH2 *FR_qcd[FRTypes][NLTypes];
TH2 *FR_qcdj[FRTypes][NLTypes];
TH2 *d_FR_zl[FRTypes][NLTypes];
TH2 *d_FR_zl[FRTypes][NLTypes];
TH2 *d_FR_zb[FRTypes][NLTypes];
TH2 *d_FR_qcd[FRTypes][NLTypes];
TH2 *d_FR_qcdj[FRTypes][NLTypes];

TH2 *d_FR_zl[FRTypes][NLTypes];

TH2 *FR_merge[FRTypes][NLTypes];
TH2 *d_FR_merge[FRTypes][NLTypes];

bool getFrAndErr(TH2 *fr, double x, int by, double &val, double &err) {
    if (fr == 0) return false;
    int bx = fr->GetXaxis()->FindBin(x);
    if (bx == 0 || bx > fr->GetNbinsX()) return false;
    val = fr->GetBinContent(bx,by);
    err = fr->GetBinError(bx,by);
    return (val != 0 && err != 0);
}

void loadData(TString iPrefix, int trig, int what) {
    TFile *fMC = TFile::Open(trig == 1 ? "fakeRates_TTJets_MC.root" : "fakeRates_TTJets_MC_NonTrig.root" );
    if (fMC) {
        FR_ttl[TIGHT ][EL] = (TH2*) fMC->Get(iPrefix+"_tight_el");
        FR_ttl[LOOSE ][EL] = (TH2*) fMC->Get(iPrefix+"_loose_el");
        FR_ttl[TIGHT ][MU] = (TH2*) fMC->Get(iPrefix+"_tight_mu");
        FR_ttl[LOOSE ][MU] = (TH2*) fMC->Get(iPrefix+"_loose_mu");
        FR_ttl[TEST  ][EL] = (TH2*) fMC->Get(iPrefix+"_tight2_el");
        FR_ttl[TEST  ][MU] = (TH2*) fMC->Get(iPrefix+"_tight2_mu");
        FR_ttl[MEDIUM][EL] = (TH2*) fMC->Get(iPrefix+"_el");
        FR_ttl[MEDIUM][MU] = (TH2*) fMC->Get(iPrefix+"_mu");
        if (what > 4 && what != 9) {
            if (what == 6) {
                FR_ttl[TEST3 ][EL] = (TH2*) fMC->Get(iPrefix+"_tight3_el");
                FR_ttl[TEST3 ][MU] = (TH2*) fMC->Get(iPrefix+"_tight3_mu");
                FR_ttl[TEST4 ][EL] = (TH2*) fMC->Get(iPrefix+"_tight4_el");
                FR_ttl[TEST4 ][MU] = (TH2*) fMC->Get(iPrefix+"_tight4_mu");
                FR_ttl[TEST5 ][EL] = (TH2*) fMC->Get(iPrefix+"_tight5_el");
                FR_ttl[TEST5 ][MU] = (TH2*) fMC->Get(iPrefix+"_tight5_mu");
                FR_ttl[TEST6 ][EL] = (TH2*) fMC->Get(iPrefix+"_tight6_el");
                FR_ttl[TEST6 ][MU] = (TH2*) fMC->Get(iPrefix+"_tight6_mu");
            }
            if (what == 7) {
                FR_ttl[TESTA ][EL] = (TH2*) fMC->Get(iPrefix+"_tightA_el");
                FR_ttl[TESTA ][MU] = (TH2*) fMC->Get(iPrefix+"_tightA_mu");
                FR_ttl[TESTB ][EL] = (TH2*) fMC->Get(iPrefix+"_tightB_el");
                FR_ttl[TESTB ][MU] = (TH2*) fMC->Get(iPrefix+"_tightB_mu");
                FR_ttl[TESTA2][EL] = (TH2*) fMC->Get(iPrefix+"_tightA2_el");
                FR_ttl[TESTA2][MU] = (TH2*) fMC->Get(iPrefix+"_tightA2_mu");
                FR_ttl[TESTB2][EL] = (TH2*) fMC->Get(iPrefix+"_tightB2_el");
                FR_ttl[TESTB2][MU] = (TH2*) fMC->Get(iPrefix+"_tightB2_mu");
            }
        }

    }

    TFile *fQCD = TFile::Open(trig == 1 ? "fakeRates_QCDMu_MC.root" : "fakeRates_QCDMu_MC_NonTrig.root");
    if ((what <= 0 || what == 1) && fQCD) {
        FR_qcd[TIGHT ][EL] = (TH2*) fQCD->Get(iPrefix+"_tight_el");
        FR_qcd[MEDIUM][EL] = (TH2*) fQCD->Get(iPrefix+"_el");
        FR_qcd[LOOSE ][EL] = (TH2*) fQCD->Get(iPrefix+"_loose_el");
        FR_qcd[TIGHT ][MU] = (TH2*) fQCD->Get(iPrefix+"_tight_mu");
        FR_qcd[MEDIUM][MU] = (TH2*) fQCD->Get(iPrefix+"_mu");
        FR_qcd[LOOSE ][MU] = (TH2*) fQCD->Get(iPrefix+"_loose_mu");
        FR_qcd[TEST  ][EL] = (TH2*) fQCD->Get(iPrefix+"_tight2_el");
        FR_qcd[TEST  ][MU] = (TH2*) fQCD->Get(iPrefix+"_tight2_mu");
        FR_qcd[TEST3 ][EL] = (TH2*) fQCD->Get(iPrefix+"_tight2m_el");
        FR_qcd[TEST3 ][MU] = (TH2*) fQCD->Get(iPrefix+"_tight2m_mu");
    } 
    if ((what <= 0 || what == 2) && fQCD) {
        FR_qcdj[TIGHT ][EL] = (TH2*) fQCD->Get(iPrefix+"j_tight_el");
        FR_qcdj[MEDIUM][EL] = (TH2*) fQCD->Get(iPrefix+"j_el");
        FR_qcdj[LOOSE ][EL] = (TH2*) fQCD->Get(iPrefix+"j_loose_el");
        FR_qcdj[TIGHT ][MU] = (TH2*) fQCD->Get(iPrefix+"j_tight_mu");
        FR_qcdj[MEDIUM][MU] = (TH2*) fQCD->Get(iPrefix+"j_mu");
        FR_qcdj[LOOSE ][MU] = (TH2*) fQCD->Get(iPrefix+"j_loose_mu");
        FR_qcdj[TEST  ][EL] = (TH2*) fQCD->Get(iPrefix+"j_tight2_el");
        FR_qcdj[TEST  ][MU] = (TH2*) fQCD->Get(iPrefix+"j_tight2_mu");
    }


    //TFile *fQCD1Mu = TFile::Open("fakeRates_QCDMu_MC_SingleMu.root");
    //for (int it = 0; it < ntrig1mu; ++it) {
    //    FR_qcd1mu_mu[it] = (TH2*) fQCD1Mu->Get(Form("%s_TagMu%d_mu", iPrefix.Data(), trig1mu[it]));
    //    FR_qcd1mu_el[it] = (TH2*) fQCD1Mu->Get(Form("%s_TagMu%d_el", iPrefix.Data(), trig1mu[it]));
    //}

    TFile *fDQCD = 0;
    if (iPrefix == "FR")  fDQCD  = TFile::Open(trig == 1 ? "frFitsSimple.root"         : "frFitsSimple_TagMuL.root");
    if (iPrefix == "FRC") fDQCD  = TFile::Open(trig == 1 ? "frFitsSimpleJustIso.root" : "frFitsSimpleJustIso_TagMuL.root");
    if (iPrefix == "FR")  fDQCD2 = TFile::Open(trig == 1 ? "frFitsSimpleBTight.root" : 0);
    if ((what <= 0 || what == 1 || what == 9) && fDQCD) {
        d_FR_qcd[TIGHT ][EL] = (TH2*) fDQCD->Get(iPrefix+"_tight_el");
        d_FR_qcd[MEDIUM][EL] = (TH2*) fDQCD->Get(iPrefix+"_el");
        d_FR_qcd[LOOSE ][EL] = (TH2*) fDQCD->Get(iPrefix+"_loose_el");
        d_FR_qcd[TIGHT ][MU] = (TH2*) fDQCD->Get(iPrefix+"_tight_mu");
        d_FR_qcd[MEDIUM][MU] = (TH2*) fDQCD->Get(iPrefix+"_mu");
        d_FR_qcd[LOOSE ][MU] = (TH2*) fDQCD->Get(iPrefix+"_loose_mu");
        if (fDQCD2) {
            d_FR_qcd[TEST][EL] = (TH2*) fDQCD2->Get(iPrefix+"_tight_el");
            d_FR_qcd[TEST][MU] = (TH2*) fDQCD2->Get(iPrefix+"_tight_mu");
        }
        if (what == 9) {
            TFile *fDQCDSS = TFile::Open("frFitsSimpleLLSS.root");
            TFile *fDQCDOS = TFile::Open("frFitsSimpleLLOS.root");
            d_FR_qcd[TEST3][EL] = (TH2*) fDQCDSS->Get(iPrefix+"_tight_el");
            d_FR_qcd[TEST3][MU] = (TH2*) fDQCDSS->Get(iPrefix+"_tight_mu");
            d_FR_qcd[TEST4][EL] = (TH2*) fDQCDOS->Get(iPrefix+"_tight_el");
            d_FR_qcd[TEST4][MU] = (TH2*) fDQCDOS->Get(iPrefix+"_tight_mu");
        }
    }
    TFile *fDQCDJ = 0;
    if (iPrefix == "FR")  fDQCDJ = TFile::Open(trig == 1 ? "frFitsSimpleJet.root"         : "frFitsSimpleJet_TagMuL.root");
    if (iPrefix == "FRC") fDQCDJ = TFile::Open(trig == 1 ? "frFitsSimpleJetJustIso.root" : "frFitsSimpleJetJustIso_TagMuL.root");
    if (iPrefix == "FR")  fDQCDJ2 = TFile::Open(trig == 1 ? "frFitsSimpleJetBTight.root" : 0);
    if ((what <= 0 || what == 2) && fDQCDJ) {
        d_FR_qcdj[TIGHT ][EL] = (TH2*) fDQCDJ->Get(iPrefix+"_tight_el");
        d_FR_qcdj[MEDIUM][EL] = (TH2*) fDQCDJ->Get(iPrefix+"_el");
        d_FR_qcdj[LOOSE ][EL] = (TH2*) fDQCDJ->Get(iPrefix+"_loose_el");
        d_FR_qcdj[TIGHT ][MU] = (TH2*) fDQCDJ->Get(iPrefix+"_tight_mu");
        d_FR_qcdj[MEDIUM][MU] = (TH2*) fDQCDJ->Get(iPrefix+"_mu");
        d_FR_qcdj[LOOSE ][MU] = (TH2*) fDQCDJ->Get(iPrefix+"_loose_mu");
        if (fDQCD2) {
            d_FR_qcdj[TEST][EL] = (TH2*) fDQCDJ2->Get(iPrefix+"_tight_el");
            d_FR_qcdj[TEST][MU] = (TH2*) fDQCDJ2->Get(iPrefix+"_tight_mu");
        }
    }


    TFile *fZMC = TFile::Open("fakeRates_Z_DYJets_MC.root");
    if ((what == 0 || what == 3) && fZMC) {
        FR_zl[TIGHT ][EL] = (TH2*) fZMC->Get(iPrefix+"_tight_el");
        FR_zl[MEDIUM][EL] = (TH2*) fZMC->Get(iPrefix+"_el");
        FR_zl[LOOSE ][EL] = (TH2*) fZMC->Get(iPrefix+"_loose_el");
        //FR_zl[TIGHT ][MU] = (TH2*) fZMC->Get(iPrefix+"_tight_mu");
        //FR_zl[MEDIUM][MU] = (TH2*) fZMC->Get(iPrefix+"_mu");
        //FR_zl[LOOSE ][MU] = (TH2*) fZMC->Get(iPrefix+"_loose_mu");
    }

    TFile *fZbMC = TFile::Open("fakeRates3D_DYJets_MC.root");
    if (0 && fZbMC) {
        FR_zb[TIGHT ][EL] = (TH2*) fZbMC->Get(iPrefix+"_tight_el");
        FR_zb[MEDIUM][EL] = (TH2*) fZbMC->Get(iPrefix+"_el");
        FR_zb[LOOSE ][EL] = (TH2*) fZbMC->Get(iPrefix+"_loose_el");
        FR_zb[TIGHT ][MU] = (TH2*) fZbMC->Get(iPrefix+"_tight_mu");
        FR_zb[MEDIUM][MU] = (TH2*) fZbMC->Get(iPrefix+"_mu");
        FR_zb[LOOSE ][MU] = (TH2*) fZbMC->Get(iPrefix+"_loose_mu");
    }

    TFile *fZData = TFile::Open("fakeRates_Z_Data.root");
    if ((what == 0 || what == 3) && fZData) {
        d_FR_zl[TIGHT ][EL] = (TH2*) fZData->Get(iPrefix+"_tight_el");
        d_FR_zl[MEDIUM][EL] = (TH2*) fZData->Get(iPrefix+"_el");
        d_FR_zl[LOOSE ][EL] = (TH2*) fZData->Get(iPrefix+"_loose_el");
        //d_FR_zl[TIGHT ][MU] = (TH2*) fZData->Get(iPrefix+"_tight_mu");
        //d_FR_zl[MEDIUM][MU] = (TH2*) fZData->Get(iPrefix+"_mu");
        //d_FR_zl[LOOSE ][MU] = (TH2*) fZData->Get(iPrefix+"_loose_mu");
    }

    TFile *fZbData = TFile::Open("fakeRates_Zb_Data.root");
    if (0 && fZbData) {
        d_FR_zb[TIGHT ][EL] = (TH2*) fZbData->Get(iPrefix+"_tight_el");
        d_FR_zb[MEDIUM][EL] = (TH2*) fZbData->Get(iPrefix+"_el");
        d_FR_zb[LOOSE ][EL] = (TH2*) fZbData->Get(iPrefix+"_loose_el");
        d_FR_zb[TIGHT ][MU] = (TH2*) fZbData->Get(iPrefix+"_tight_mu");
        d_FR_zb[MEDIUM][MU] = (TH2*) fZbData->Get(iPrefix+"_mu");
        d_FR_zb[LOOSE ][MU] = (TH2*) fZbData->Get(iPrefix+"_loose_mu");
    }



    TFile *fZ3lData = TFile::Open("fakeRates_Z3l_Data.root");
    if (0 && fZ3lData) {
    }

    TFile *fMergeM = TFile::Open("fakeRates_merged.root",      "RECREATE");
    TFile *fMergeD = TFile::Open("fakeRates_merged_Data.root", "RECREATE");
    float ptbins_merge[101]; 
    for (int i = 0; i <= 100; ++i) { ptbins_merge[i] = 5.0 * exp( i*log(100.0/5.0)/100. ); }
    float etabins_mu[3] = { 0, 1.5,   2.5 };
    float etabins_el[4] = { 0, 0.8, 1.479, 2.5 };
    for (int i = 0;  i < FRTypes; ++i) {
        for (int l = 0; l < NLTypes; ++l) {
            if (FR_ttl[i][l] == 0) continue;
            fMergeM->cd();
            FR_merge[i][l]   = new TH2F(FR_ttl[i][l]->GetName(), "", 100, ptbins_merge, (l==EL ? 3 : 2), (l==EL ? etabins_el : etabins_mu));
            fMergeD->cd();
            d_FR_merge[i][l] = new TH2F(FR_ttl[i][l]->GetName(), "", 100, ptbins_merge, (l==EL ? 3 : 2), (l==EL ? etabins_el : etabins_mu));
            int nbin = 0;
            for (int bx = 1, nbx = FR_merge[i][l]->GetNbinsX(), nby = FR_merge[i][l]->GetNbinsY(); bx <= nbx; ++bx) {
                for (int by = 1; by <= nby; ++by) {
                    double x = FR_merge[i][l]->GetXaxis()->GetBinCenter(bx);

                    int ndata = 0; 
                    double sumwdata = 0, sumfwdata = 0;
                    double sumwmc   = 0, sumfwmc   = 0, sumewmc = 0;
                    double val, err, mcval, mcerr;
                    if (getFrAndErr(d_FR_qcd[i][l], x, by, val, err) && getFrAndErr(FR_qcd[i][l], x, by, mcval, mcerr)) {
                        ndata++;
                        sumwdata  += 1.0/(err*err);
                        sumfwdata += val/(err*err);
                        sumwmc    += 1.0/(err*err);
                        sumfwmc   += mcval/(err*err);
                        sumewmc    += pow(mcerr/(err*err),2);
                    }
                    if (getFrAndErr(d_FR_qcdj[i][l], x, by, val, err) && getFrAndErr(FR_qcdj[i][l], x, by, mcval, mcerr)) {
                        ndata++;
                        sumwdata  += 1.0/(err*err);
                        sumfwdata += val/(err*err);
                        sumwmc    += 1.0/(err*err);
                        sumfwmc   += mcval/(err*err);
                        sumewmc    += pow(mcerr/(err*err),2);
                    }
                    if (getFrAndErr(d_FR_zl[i][l], x, by, val, err) && getFrAndErr(FR_zl[i][l], x, by, mcval, mcerr)) {
                        ndata++;
                        sumwdata  += 1.0/(err*err);
                        sumfwdata += val/(err*err);
                        sumwmc    += 1.0/(err*err);
                        sumfwmc   += mcval/(err*err);
                        sumewmc   += pow(mcerr/(err*err),2);
                    }
                    if (ndata > 0) {
                        nbin++;
                        d_FR_merge[i][l]->SetBinContent(bx,by, sumfwdata/sumwdata);
                        d_FR_merge[i][l]->SetBinError(  bx,by, sqrt(1.0/sumwdata));
                          FR_merge[i][l]->SetBinContent(bx,by, sumfwmc  /sumwmc  );
                          FR_merge[i][l]->SetBinError(  bx,by, sqrt(sumewmc/(sumwmc*sumwmc)));
                    }
                }

            }
            if (nbin > 0) {
                fMergeM->WriteTObject(FR_merge[i][l]);
                fMergeD->WriteTObject(d_FR_merge[i][l]);
            } else {
                delete   FR_merge[i][l];   FR_merge[i][l] = 0;
                delete d_FR_merge[i][l]; d_FR_merge[i][l] = 0;
            }
        }
    }
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
    int j = 0;
    for (int i = 0, n = ret->GetN(); i < n; ++i, ++j) {
        ret->SetPoint(j, ax->GetBinCenter(i+1), h2d->GetBinContent(i+1,ieta+1));
        if (ist == 2) {
            double syst = 0.4;
            //if ax->GetBinLowEdge(i+1) < 10
            ret->SetPointError(j, 0.5*ax->GetBinWidth(i+1), 0.5*ax->GetBinWidth(i+1),
                                  syst*h2d->GetBinContent(i+1,ieta+1), syst*h2d->GetBinContent(i+1,ieta+1));
        } else {
            ret->SetPointError(j, 0.5*ax->GetBinWidth(i+1), 0.5*ax->GetBinWidth(i+1),
                                  h2d->GetBinError(i+1,ieta+1), h2d->GetBinError(i+1,ieta+1));
        }
        if (j > 0 && ret->GetY()[j] == ret->GetY()[j-1] && ret->GetEYhigh()[j] == ret->GetEYhigh()[j-1] && ret->GetEYlow()[j] == ret->GetEYlow()[j-1]) {
            double xmin = ret->GetX()[j-1]-ret->GetEXlow()[j-1];
            double xmax = ret->GetX()[j  ]+ret->GetEXhigh()[j];
            ret->GetX()[j-1]      = 0.5*(xmax+xmin);
            ret->GetEXlow()[j-1]  = 0.5*(xmax-xmin);
            ret->GetEXhigh()[j-1] = 0.5*(xmax-xmin);
            --j;
        }
    }
    if (j != n) ret->Set(j);

    ret->SetLineColor(color);
    ret->SetMarkerColor(color);
    if (ist == 1 || ist == 2) {
        ret->SetLineWidth(1);
        ret->SetFillColor(color);
        ret->SetFillStyle(ifill ? ifill : (ist == 1 ? 3004 : 1001));
        ret->Draw("E5 SAME");
        ret->Draw("E2 SAME");
    } else {
        if (ist == 0) {
            ret->SetLineWidth(2);
        } else {
            ret->SetLineWidth(4);
            ret->SetMarkerSize(1.6);
        }
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

const char *ETALBL2[2] = { "b", "e" };
const char *ETALBL3[3] = { "cb", "fb", "ec" };
const char *ietalbl(int ieta, LType ilep) {
    if (ilep == EL) return ETALBL3[ieta];
    else return ETALBL2[ieta];
}
const char *etaspam(int ieta, LType ilep) { 
    if (ilep == MU) {
        return Form("|#eta| %s 1.5", (ieta ? ">" : "<"));
    } else {
        switch (ieta) {
            case 0: return "0 < |#eta| < 0.8";
            case 1: return "0.8 < |#eta| < 1.479";
            case 2: return "|#eta| > 1.479";
        }
    }
    return "NON CAPISCO";
}

void stackFRs(LType lep, FRType wp, int ieta, int idata=0, int merge = 0) {
    if (ieta > 1 && lep == MU) return;

    frame->Draw();

    TGraph *ttl40 = drawSlice(FR_ttl[wp][lep], ieta,  kAzure-9, 2);
    TGraph *ttl = drawSlice(FR_ttl[wp][lep],  ieta,  62, 1, 1001);
    TGraph *ttlX = 0; //wp == TIGHT ? drawSlice(FR_ttl[TEST][lep],   ieta,  1, 1, 3004) : 0;
    TGraph *ttl1 = wp == TIGHT ? drawSlice(FR_ttl[MEDIUM][lep],  ieta,  209) : 0;
    //TGraph *ttl2 = wp == TIGHT ? drawSlice(FR_ttl[TEST ][lep],  ieta,  1) : 0;
    TGraph *ttl4 = wp == TIGHT ? drawSlice(FR_ttl[TEST4][lep],  ieta,  97, 1, 3005) : 0;
    TGraph *ttl6 = wp == TIGHT ? drawSlice(FR_ttl[TEST6][lep],  ieta, 213, 1, 3007) : 0;
    TGraph *ttl3 = wp == TIGHT ? drawSlice(FR_ttl[TEST3][lep],  ieta, 206) : 0;
    TGraph *ttl5 = wp == TIGHT ? drawSlice(FR_ttl[TEST5][lep],  ieta, 213) : 0;
    TGraph *ttlA  = wp == TIGHT ? drawSlice(FR_ttl[TESTA ][lep],  ieta, 214) : 
                   (wp == TEST  ? drawSlice(FR_ttl[TESTA2][lep],  ieta, 214) : 0);
    TGraph *ttlB  = wp == TIGHT ? drawSlice(FR_ttl[TESTB ][lep],  ieta, 206) : 
                   (wp == TEST  ? drawSlice(FR_ttl[TESTB2][lep],  ieta, 206) : 0);
    TGraph *zl  = merge != 1 ? drawSlice(FR_zl[wp][lep],   ieta,  97, 1, 3006) : 0;
    TGraph *zb  = merge != 1 ? drawSlice(FR_zb[wp][lep],   ieta, 222, 1, 3004) : 0;
    TGraph *qcd2 =  (wp == TEST && merge != 1 && idata == 0? drawSlice(FR_qcd[TEST3][lep],  ieta, 100, 1, 3007) : 0);
    TGraph *qcd = merge != 1 ? drawSlice(FR_qcd[wp][lep],  ieta, 213, 1, 3005) : 0;
    TGraph *qcj = merge != 1 ? drawSlice(FR_qcdj[wp][lep], ieta, 100, 1, 3007) : 0;
    TGraph *mrg = merge != 0 ? drawSlice(FR_merge[wp][lep],ieta,   1, 1, 3005) : 0;
 
    frame->Draw("AXIS SAME");

    int nplots = (ttl!=0)+(zl!=0)+(zb!=0)+(qcd!=0)+(qcd2!=0)+(qcj!=0)+(mrg!=0)+(ttl6 ? 4 : 0)+(ttlB ? 2 : 0)+(ttlX!=0);
    TLegend *leg = newLeg(.27,.85-0.05*nplots,.49,.9);
    leg->AddEntry(ttl,  "MC tt+l", "F");
    leg->AddEntry(ttl40, "  #pm 40%", "LF");
    if (ttlX) leg->AddEntry(ttlX,  "(w/ 2B)", "F");
    if (ttl1) leg->AddEntry(ttl1,  "(3l or 2j)", "LPE");
    //if (ttl2) leg->AddEntry(ttl2,  "b tight", "LPE");
    if (ttl3) leg->AddEntry(ttl3,  "3l b loose", "LPE");
    if (ttl4) leg->AddEntry(ttl4,  "3l b tight", "F");
    if (ttl5) leg->AddEntry(ttl5,  "2l b loose", "LPE");
    if (ttl6) leg->AddEntry(ttl6,  "2l b tight", "F");
    if (ttlA) leg->AddEntry(ttlA,  "not B", "LPE");
    if (ttlB) leg->AddEntry(ttlB,  "from B", "LPE");
    if (zl) leg->AddEntry(zl, "MC Z+l",   "F");
    if (zb) leg->AddEntry(zb, "MC Z+l 3D", "F");
    if (qcd) leg->AddEntry(qcd, "MC qcd #mu", "F");
    if (qcd2) leg->AddEntry(qcd2, " (CSV M)", "F");
    if (qcj) leg->AddEntry(qcj, "MC qcd bj", "F");
    if (mrg) leg->AddEntry(mrg, "MC merge", "F");
    //leg->AddEntry(drawSlice(FR_qcdj[wp][lep], ieta, 209), "MC qcd j", "LP");
    leg->Draw();
    if (idata) {
        TGraph *dqS = wp == TIGHT && merge != 1 ? drawSlice(d_FR_qcd[TEST3][lep],  ieta,  4, 1, 3005) : 0;
        TGraph *dqO = wp == TIGHT && merge != 1 ? drawSlice(d_FR_qcd[TEST4][lep],  ieta,  2, 1, 3004) : 0;
        TGraph *dq  = merge != 1 ? drawSlice(d_FR_qcd[wp][lep],  ieta,   1) : 0;
        TGraph *dqj = merge != 1 ? drawSlice(d_FR_qcdj[wp][lep], ieta, 206) : 0;
        TGraph *dzl = merge != 1 ? drawSlice(d_FR_zl[wp][lep], ieta, 206) : 0;
        TGraph *dzb = merge != 1 ? drawSlice(d_FR_zb[wp][lep], ieta, 206) : 0;
        TGraph *dmg = merge != 0 ? drawSlice(d_FR_merge[wp][lep], ieta, 209, 3) : 0;
        int nplots = (dq!=0)+(dqj!=0)+(dzl!=0)+(dzb!=0)+(dmg!=0)+(dqS!=0?2:0);
        TLegend *leg = newLeg(.55,.87-0.05*nplots,.87,.9);
        if (dq)  leg->AddEntry(dq, "Data qcd #mu", "LPE");
        if (dqS) leg->AddEntry(dqS, "same-sign", "F");
        if (dqO) leg->AddEntry(dqO, "oppo-sign", "F");
        if (dqj) leg->AddEntry(dqj, "Data qcd bj", "LPE");
        if (dzl) leg->AddEntry(dzl, "Data Z+l #mu", "LPE");
        if (dzb) leg->AddEntry(dzb, "Data Z+b+l #mu", "LPE");
        if (dmg) leg->AddEntry(dmg, "Data merge", "LPE");
        leg->Draw();
        cmsprelim(.55, .81-0.05*nplots, .85, .86-0.05*nplots, 22, etaspam(ieta,lep), 0.045);
    } else {
        cmsprelim(.55, .8, .87, .88, 22, etaspam(ieta,lep), 0.045);
    }
    cmsprelim(.21, .945, .40, .995, 12, "CMS Preliminary"); 
    cmsprelim(.48, .945, .96, .995, 32, "#sqrt{s} = 8 TeV, L = 19.6 fb^{-1}");
    const char *pName[NLTypes] = { "el", "mu" };
    const char *wpName[FRTypes] = { "", "_tight", "_loose", "_test" };
    c1->Print(Form("ttH_plots/250513/FR_QCD_Simple_v2/stacks/%s/%s%s_%s.png", gPrefix.Data(), pName[lep], wpName[wp], ietalbl(ieta,lep)));
    c1->Print(Form("ttH_plots/250513/FR_QCD_Simple_v2/stacks/%s/%s%s_%s.pdf", gPrefix.Data(), pName[lep], wpName[wp], ietalbl(ieta,lep)));
}

void stackFRsMu(int ieta,  int idata=0, int merge=0) { stackFRs(MU, MEDIUM, ieta, idata, merge); }
void stackFRsEl(int ieta,  int idata=0, int merge=0) { stackFRs(EL, MEDIUM, ieta, idata, merge); } 
void stackFRsMuT(int ieta, int idata=0, int merge=0) { stackFRs(MU, TIGHT,  ieta, idata, merge); }
void stackFRsElT(int ieta, int idata=0, int merge=0) { stackFRs(EL, TIGHT,  ieta, idata, merge); }
void stackFRsMuL(int ieta, int idata=0, int merge=0) { stackFRs(MU, LOOSE,  ieta, idata, merge); }
void stackFRsElL(int ieta, int idata=0, int merge=0) { stackFRs(EL, LOOSE,  ieta, idata, merge); }

void stackFRs1MuMu(int ieta, int idata=0, int merge=0) {
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
    c1->Print(Form("ttH_plots/250513/FR_QCD_Simple_v2/stacks/%s/mu_%s_1mu.png", gPrefix.Data(), ietalbl(ieta)));
    c1->Print(Form("ttH_plots/250513/FR_QCD_Simple_v2/stacks/%s/mu_%s_1mu.pdf", gPrefix.Data(), ietalbl(ieta)));
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
    c1->Print(Form("ttH_plots/250513/FR_QCD_Simple_v2/stacks/%s/el_%s_1mu.png", gPrefix.Data(), ietalbl(ieta)));
    c1->Print(Form("ttH_plots/250513/FR_QCD_Simple_v2/stacks/%s/el_%s_1mu.pdf", gPrefix.Data(), ietalbl(ieta)));
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
    int merge = 0;
    switch (what) {
        case 0: loadData("FR",itrigg,what); gPrefix = "";           break;
        case 1: loadData("FR",itrigg,what); gPrefix = "qcdmu/"; break;
        case 2: loadData("FR",itrigg,what); gPrefix = "qcdj/";    break;
        case 3: loadData("FR",itrigg,what); gPrefix = "z3l/";    break;
        case 4: loadData("FR",itrigg,0);    gPrefix = "merged/"; merge = 1;  break;
        case 5: loadData("FR",itrigg,5);    gPrefix = "nothing/"; break;
        case 6: loadData("FR",itrigg,6);    gPrefix = "nothing-more/"; break;
        case 7: loadData("FR",itrigg,7);    gPrefix = "origin/"; break;
        case 9: loadData("FR",itrigg,9);    gPrefix = "ss-os/"; break;
        case -1: loadData("FRC",itrigg,0);    gPrefix = "cutbased/"; break;
    }
    if (idata) gPrefix += "with_data/";
    switch (itrigg) {
        case 0: gPrefix += "non_trig/"; break;
        case 1: break;
        case 2: gPrefix += "tag_trig/"; break;
    }

    gSystem->Exec("mkdir -p ttH_plots/250513/FR_QCD_Simple_v2/stacks/"+gPrefix);
    gSystem->Exec("cp /afs/cern.ch/user/g/gpetrucc/php/index.php ttH_plots/250513/FR_QCD_Simple_v2/stacks/"+gPrefix);
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
    for (int i = 0; i < 3; ++i) {
        //frame->GetYaxis()->SetRangeUser(0.0,0.6);
        //frame->GetYaxis()->SetTitle("fake rate (#mu, medium)");
        //frame->GetXaxis()->SetRangeUser(5,80);
        //stackFRsMu(i, idata);
        //frame->GetYaxis()->SetTitle("fake rate (e, medium)");
        //frame->GetXaxis()->SetRangeUser(7,80);
        //stackFRsEl(i, idata);
        //frame->GetYaxis()->SetRangeUser(0.0,0.6);
        //stackFRs1MuMu(i, idata);
        //stackFRs1MuEl(i, idata);
        //frame->GetXaxis()->SetRangeUser(10,80);

        frame->GetYaxis()->SetRangeUser(0.0,0.2+0.2*(i==0));
        frame->GetYaxis()->SetTitle("fake rate (#mu, tight)");
        frame->GetXaxis()->SetRangeUser(5,80);
        stackFRsMuT(i, idata, merge);
        frame->GetYaxis()->SetTitle("fake rate (e, tight)");
        frame->GetXaxis()->SetRangeUser(7,80);
        stackFRsElT(i, idata, merge);

        frame->GetXaxis()->SetRangeUser(5,80);
        frame->GetYaxis()->SetRangeUser(0.0,1.0);
        frame->GetYaxis()->SetTitle("fake rate (#mu, loose)");
        frame->GetXaxis()->SetRangeUser(5,80);
        stackFRsMuL(i, idata, merge);
        frame->GetYaxis()->SetTitle("fake rate (e, loose)");
        frame->GetXaxis()->SetRangeUser(7,80);
        stackFRsElL(i, idata, merge);

        frame->GetYaxis()->SetRangeUser(0.0,0.2+0.2*(i==0)+0.3*(idata));
        frame->GetYaxis()->SetTitle("b-tight fake rate (#mu, tight)");
        frame->GetXaxis()->SetRangeUser(5,80);
        stackFRs(MU, TEST, i, idata, merge);
        frame->GetYaxis()->SetTitle("b-tight fake rate (e, tight)");
        frame->GetXaxis()->SetRangeUser(7,80);
        stackFRs(EL, TEST, i, idata, merge);

  
    }
}
