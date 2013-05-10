#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <THStack.h>
#include <TEfficiency.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <Math/Functor.h>
#include <Minuit2/Minuit2Minimizer.h>

#define TRIGGERING 1

//#define EVEN_BINS
TCanvas *c1 = 0;
TString gPrefix = "";
TString gPostfix = "";

int ndata = 5;
const int ndata_max = 8;
TFile *fQCD = 0;
TFile *fWJ  = 0;
TFile *fDY[2]  =  { 0, 0 };
TFile *fdata[ndata_max] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

TH1* rebin(TH1 *hist) { 
    if (hist == 0) return 0;
    TH1 *ret = (TH1*) hist->Clone(); 
    ret->Rebin(5);
    return ret;
}

void readAndAdd(TString name, TH1 * &prompt, TH1* &qcd, TH1* &data, const TH1 *ref = 0) {
    double lumi = 19.6-0.1-0.6;
    /*
    std::cout << "readAndAdd(" << name << ")" << std::endl;
    for (int i = 0; i <= 7; ++i) std::cout << "fdata[" << i << "] = " << (fdata[i] ? fdata[i]->GetName() : "NULL") << std::endl;
    std::cout << "fQCD = " << (fQCD ? fQCD->GetName() : "NULL") << std::endl;
    std::cout << "fWJ = " << (fWJ ? fWJ->GetName() : "NULL") << std::endl;
    std::cout << "fDY[" << 0 << "] = " << (fDY[0] ? fDY[0]->GetName() : "NULL") << std::endl;
    std::cout << "fDY[" << 1 << "] = " << (fdata[1] ? fdata[1]->GetName() : "NULL") << std::endl;
    */
    data = rebin((TH1*) fdata[0]->Get(name));
    data->Add(rebin((TH1*) fdata[1]->Get(name)));
    data->Add(rebin((TH1*) fdata[2]->Get(name)));
    if (fdata[3]) { data->Add(rebin((TH1*) fdata[3]->Get(name))); lumi += 0.1; }
    if (fdata[4]) { data->Add(rebin((TH1*) fdata[4]->Get(name))); lumi += 0.6; }
    if (fdata[5]) { data->Add(rebin((TH1*) fdata[5]->Get(name))); }
    if (fdata[6]) { data->Add(rebin((TH1*) fdata[6]->Get(name))); }
    if (fdata[7]) { data->Add(rebin((TH1*) fdata[7]->Get(name))); }
    qcd = rebin((TH1*) fQCD->Get(name)->Clone());
    qcd->Sumw2(); qcd->Scale( lumi*1e3 * (134680.0/21484602) );
    prompt = rebin((TH1*) fWJ->Get(name)->Clone());
    prompt->Sumw2(); prompt->Scale( lumi*1e3 * 36257.2/57e6 );
    TH1 *zed = rebin((TH1*) fDY[0]->Get(name)->Clone());
    zed->Sumw2(); zed->Scale( lumi*1e3 * 3503.7/30.5e6 );
    prompt->Add(zed);
    zed = rebin((TH1*) fDY[1]->Get(name)->Clone());
    zed->Sumw2(); zed->Scale( lumi*1e3 * 915/7.1e6 );
    prompt->Add(zed);
}


void frFromRange(TH1 *num, TH1 *den, double mtmin, double mtmax, double &fr, double &errL, double &errH) {
    double n_num = num->Integral(num->GetXaxis()->FindBin(mtmin), num->GetXaxis()->FindBin(mtmax));
    double n_den = den->Integral(den->GetXaxis()->FindBin(mtmin), den->GetXaxis()->FindBin(mtmax));
    double events = (n_den/den->Integral()*den->GetEntries());
    int inum = events > 1 ? round(n_num*events/n_den) : 0;
    std::cout << "frFromRange(" << num->GetName() << ") " << n_num << "\t" << n_den << "\t" << events << "\t" << inum << std::endl;
    double fup = events >= 1 ? TEfficiency::ClopperPearson(events, inum, 0.683, 1) : 0; 
    double fdn = events >= 1 ? TEfficiency::ClopperPearson(events, inum, 0.683, 0) : 0; 
    fr = n_num/n_den;
    //err = sqrt(fr*(1-fr)/events);
    //fr = 0.5*(fup+fdn); err = 0.5*(fup-fdn);
    errL = fr-fdn; errH = fup - fr;
}
void frFromRange(TH1 *num, TH1 *den, double mtmin, double mtmax, double &fr, double &err) {
    double elo, ehi;
    frFromRange(num,den,mtmin,mtmax,fr,elo,ehi);
    err = 0.5*((fr+ehi)-(fr-elo));
    fr = 0.5*((fr-elo)+(fr+ehi));
}

void integralWithError(TH1 *hist, double mtmin, double mtmax, double &ret, double &err)  {
    int b0 = hist->GetXaxis()->FindBin(mtmin);
    int b1 = hist->GetXaxis()->FindBin(mtmax);
    ret = 0; err = 0;
    for (int b = b0; b <= b1; ++b) {
        ret += hist->GetBinContent(b);
        err += std::pow(hist->GetBinError(b),2);
    }
    err = sqrt(err);
}

bool fitFRMTCorr(TString name, TH1 *hewk_den, TH1 *hdat_num, TH1* hdat_den, double *fr, bool verbose=false) {
    // some math: 
    //   let x[i] = fraction of ewk in the MET bin [i] at denominator, 
    //              and assume the FR does not depend on MET
    //   f_dat[i] = f_ewk * x[i] + f_qcd * ( 1-x[i] )
    // Now x[i]/x[j] = (N_ewk[i]/N_ewk[j]) / (N_[i]/N_[j]),   (N=events at denominator)
    //   the first ratio can be taken from MC, the second is known from data.
    // Taking the measurement in two bins, and factorizing x[i]
    //   f_dat[i] = (f_ewk - f_qcd) * x[i] + f_qcd 
    //   f_dat[j] = (f_ewk - f_qcd) * x[j] + f_qcd 
    // Multiply the second by x[i]/x[j] and subtract
    //   f_dat[i] - x[i]/x[j] * f_dat[j] = (f-f)(x[i]-x[i]/x[j]*x[j]) + f_qcd (1 - x[i]/x[j]) 
    // ---> 
    //    f_qcd = ( f_dat[i] - x[i]/x[j] * f_dat[j] ) / ( 1 - x[i]/x[j] )

    double lo_lo =  0.0, lo_hi = 15.0;
    double hi_lo = 45.0, hi_hi = 80.0;
    if (gPostfix.Contains("TagMu")) { 
        lo_hi = 35.0; 
        hi_lo = 35.0; 
        hi_hi = 100.0; 
    }

    double frdat[2], frdatErr[2]; 
    frFromRange(hdat_num, hdat_den, lo_lo, lo_hi, frdat[0], frdatErr[0]);
    frFromRange(hdat_num, hdat_den, hi_lo, hi_hi, frdat[1], frdatErr[1]);
    
    double newk[2][2], ndat[2][2];
    integralWithError(hewk_den, lo_lo, lo_hi, newk[0][0], newk[0][1]);
    integralWithError(hewk_den, hi_lo, hi_hi, newk[1][0], newk[1][1]);
    integralWithError(hdat_den, lo_lo, lo_hi, ndat[0][0], ndat[0][1]);
    integralWithError(hdat_den, hi_lo, hi_hi, ndat[1][0], ndat[1][1]);

    if (newk[0][0] == 0 || newk[1][0] == 0 || ndat[0][0] == 0 || ndat[1][0] == 0) {
        return false;
    }

    double x0x1 = (newk[0][0]/newk[1][0]) / (ndat[0][0]/ndat[1][0]);
    double x0x1Err = x0x1 * hypot( newk[0][1]/newk[0][0], newk[1][1]/newk[1][0] ); 

    if (verbose) {
        printf("FR low: %.3f   high: %.3f    nP ratio: %5.2f  den ratio: %5.2f    x0/x1 = %.3f +/- %.3f\n",
                frdat[0], frdat[1], newk[0][0]/newk[1][0], ndat[0][0]/ndat[1][0], x0x1, x0x1Err);
        //printf("Error boosting factor for %s: %4.1f  (x0/x1 = %.3f +/- %.3f) \n", name.Data(), 1/(1-x0x1), x0x1, x0x1Err);
    }

    fr[0] = (frdat[0] - x0x1*frdat[1])/(1-x0x1);
    fr[1] = hypot( frdatErr[0] / (1-x0x1), frdatErr[1] * x0x1 / (1-x0x1) );
    fr[2] = fr[1];
    fr[3] = x0x1Err * std::abs(frdat[0] - frdat[1]) / std::pow(1-x0x1,2);
    if (fr[0]-hypot(fr[1],fr[3]) < 0) {
        double frmax = max(fr[0],0.)+hypot(fr[1],fr[3]);
        fr[0] = 0.5*frmax; 
        fr[1] = fr[2] = min(fr[1],0.5*frmax);
        fr[3] = sqrt(0.25*frmax*frmax - fr[1]*fr[1]);
    }
    return true;
}


void fitFRMT(TString name, TH1 *hqcd_num, TH1* hqcd_den, TH1 *hewk_den, TH1 *hdat_num, TH1* hdat_den, double *fr, bool inclusive=false) {
    const int ranges_qcd = 3, ranges_dat = 7;
    double mtmax_qcd[ranges_qcd] = { 60, 30., 15. };
    double mtmax_dat[ranges_dat] = { 60, 45., 30., 20., 15., 10., 5. };
    TGraphAsymmErrors gqcd(ranges_qcd), gqcdA(1), gdat(ranges_dat), gdatA(1), gdatC(1), gdatCS(1);
    double ymax = 0;
    double frqcd, frqcdErrL, frqcdErrH; 
    for (int i = 0; i < ranges_qcd; ++i) {
        double mtmax =  mtmax_qcd[i], mtmin =  i < ranges_qcd-1 ? mtmax_qcd[i+1] : 0;
        if (inclusive) {
            frFromRange(hqcd_num, hqcd_den, 0, mtmax, frqcd, frqcdErrL, frqcdErrH);
            gqcd.SetPoint(i, mtmax, frqcd); 
            gqcd.SetPointError(i, 0, 0, frqcdErrL, frqcdErrH);
        } else {
            frFromRange(hqcd_num, hqcd_den, mtmin, mtmax, frqcd, frqcdErrL, frqcdErrH);
            gqcd.SetPoint(i, 0.5*(mtmin+mtmax), frqcd); 
            gqcd.SetPointError(i, 0.5*(mtmax-mtmin), 0.5*(mtmax-mtmin), frqcdErrL, frqcdErrH);
        }
        ymax = max(ymax, frqcd+1.5*frqcdErrH);
    }
    frFromRange(hqcd_num, hqcd_den, 0, mtmax_qcd[0], frqcd, frqcdErrL, frqcdErrH);
    gqcdA.SetPoint(0, 0.5*mtmax_qcd[0], frqcd); 
    gqcdA.SetPointError(0, 0.5*mtmax_qcd[0], 0.5*mtmax_qcd[0], frqcdErrL, frqcdErrH);
    double frdat, frdatErrL, frdatErrH, frdatSyst; 
    for (int i = 0; i < ranges_dat; ++i) {
        double mtmax =  mtmax_dat[i], mtmin =  i < ranges_dat-1 ? mtmax_dat[i+1] : 0;
        if (inclusive) {
            frFromRange(hdat_num, hdat_den, mtmax, frdat, frdatErrL, frdatErrH);
            gdat.SetPoint(i, mtmax, frdat); 
            gdat.SetPointError(i, 0, 0, frdatErrL, frdatErrH);
        } else {
            frFromRange(hdat_num, hdat_den, mtmin, mtmax, frdat, frdatErrL, frdatErrH);
            gdat.SetPoint(i, 0.5*(mtmin+mtmax), frdat); 
            gdat.SetPointError(i, 0.5*(mtmax-mtmin), 0.5*(mtmax-mtmin), frdatErrL, frdatErrH);
        }
        ymax = max(ymax, frdat+1.5*frdatErrH);
    }
    int avgbins = 3, systbins = 5;
    if (gPostfix.Contains("TagMu")) { avgbins = 5; systbins = 0; }
    frFromRange(hdat_num, hdat_den, 0, mtmax_dat[ranges_dat-avgbins], frdat, frdatErrL, frdatErrH);
    frdatSyst = 0;
    for (int i = 1; i <= systbins; ++i) {
        frdatSyst = max(frdatSyst, fabs(frdat - gdat.GetY()[ranges_dat-i])); 
    }
    gdatA.SetPoint(0, 0.5*mtmax_dat[ranges_dat-avgbins], frdat); 
    gdatA.SetPointError(0, 0.5*mtmax_dat[ranges_dat-avgbins], 0.5*mtmax_dat[ranges_dat-avgbins], hypot(frdatErrL,frdatSyst), hypot(frdatErrH,frdatSyst));

    bool hascorr = ( hewk_den ? fitFRMTCorr(name, hewk_den, hdat_num, hdat_den, fr) : false );
    gdatC.SetPoint( 0, 0.5*mtmax_dat[0], fr[0]); 
    gdatCS.SetPoint(0, 0.5*mtmax_dat[0], fr[0]); 
    gdatC.SetPointError( 0, 0.5*mtmax_dat[0], 0.5*mtmax_dat[0], fr[1], fr[2]);
    gdatCS.SetPointError(0, 0.5*mtmax_dat[0], 0.5*mtmax_dat[0], hypot(fr[1],fr[3]), hypot(fr[2],fr[3]));

    TH1F frame("frame", "frame;E_{T}^{miss} [GeV];Fake rate", 1, 0, mtmax_dat[0]);
    frame.SetMaximum(1.3*ymax); frame.Draw(); gStyle->SetOptStat(0);

    gqcd.SetFillColor(65); gqcd.Draw("E2 SAME");
    gqcdA.SetFillColor(214); gqcdA.SetFillStyle(3004); gqcdA.Draw("E2 SAME");
    gqcdA.SetLineColor(214); gqcdA.SetLineWidth(3); gqcdA.Draw("P SAME");
    gdatA.SetMarkerStyle(0); gdatA.SetMarkerColor(2);
    gdatA.SetFillColor(2); gdatA.SetFillStyle(3005); gdatA.Draw("E2 SAME");
    gdatA.SetLineColor(2); gdatA.SetLineWidth(3); gdatA.Draw("P SAME");
    if (hascorr) {
        gdatCS.SetMarkerStyle(0); gdatCS.SetMarkerColor(223);
        gdatC.SetMarkerStyle(0); gdatC.SetMarkerColor(223);
        gdatCS.SetFillColor(223); gdatCS.SetFillStyle(3005); gdatCS.Draw("E2 SAME");
        gdatC.SetLineColor(223); gdatC.SetLineWidth(3); gdatC.Draw("P SAME");
    }
    gdat.SetMarkerStyle(20); gdat.SetLineWidth(2); gdat.Draw("P SAME");

    c1->Print("ttH_plots/270413/FR_QCD_Simple/fits/"+gPrefix+gPostfix+"/"+name+".png");
    c1->Print("ttH_plots/270413/FR_QCD_Simple/fits/"+gPrefix+gPostfix+"/"+name+".pdf");

    fr[0] = frdat;
    fr[1] = frdatErrL; fr[2] = frdatErrH;
    fr[3] = frdatSyst;
}
void processOneBin(TString name, double *frLoose, double *frTight, bool doCorr) {
    TH1 *hprompt_den = 0, *hprompt_denT = 0, *hprompt_numL = 0, *hprompt_numT = 0;
    TH1 *hqcd_den = 0, *hqcd_denT = 0, *hqcd_numL = 0, *hqcd_numT = 0;
    TH1 *hdata_den = 0, *hdata_denT = 0, *hdata_numL = 0, *hdata_numT = 0;
    readAndAdd(name+"_den", hprompt_den, hqcd_den, hdata_den);
    readAndAdd(name+"_denT", hprompt_denT, hqcd_denT, hdata_denT);
    readAndAdd(name+"_numL", hprompt_numL, hqcd_numL, hdata_numL);
    readAndAdd(name+"_numT", hprompt_numT, hqcd_numT, hdata_numT);

    double mcLoose = hqcd_numL->Integral()/hqcd_den->Integral();
    double mcTight = hqcd_numT->Integral()/hqcd_denT->Integral();
    double mcLooseErr = sqrt(mcLoose*(1-mcLoose)/hqcd_den->GetEntries());
    double mcTightErr = sqrt(mcTight*(1-mcTight)/hqcd_denT->Integral());

    frLoose[0] = 0; frLoose[1] = 1; frLoose[2] = 1; frLoose[3] = 1;
    fitFRMT(name+"_fitL", hqcd_numL, hqcd_den,  doCorr ? hprompt_den  : 0, hdata_numL, hdata_den,  frLoose);
    fitFRMT(name+"_fitT", hqcd_numT, hqcd_denT, doCorr ? hprompt_denT : 0, hdata_numT, hdata_denT, frTight);

    printf("\nFitting %s%s/%s\n",gPrefix.Data(),gPostfix.Data(),name.Data());
    FILE *log = fopen(Form("ttH_plots/270413/FR_QCD_Simple/fits/%s%s/%s.txt",gPrefix.Data(),gPostfix.Data(),name.Data()), "w");
    printf("BEFORE corrections: \n"); fprintf(log, "BEFORE corrections: \n"); 
    printf(      "FR for loose cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frLoose[0], frLoose[2], frLoose[1], frLoose[3], mcLoose, mcLooseErr);
    fprintf(log, "FR for loose cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frLoose[0], frLoose[2], frLoose[1], frLoose[3], mcLoose, mcLooseErr);
    printf(      "FR for tight cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frTight[0], frTight[2], frTight[1], frTight[3], mcTight, mcTightErr);
    fprintf(log, "FR for tight cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frTight[0], frTight[2], frTight[1], frTight[3], mcTight, mcTightErr);

    if (doCorr) {
        bool hasL = fitFRMTCorr(name+"_fitL", hprompt_den, hdata_numL, hdata_den,  frLoose, true);
        bool hasT = fitFRMTCorr(name+"_fitT", hprompt_den, hdata_numT, hdata_denT, frTight, true);
        if (hasL || hasT) {  printf("AFTER corrections: \n"); fprintf(log, "AFTER corrections: \n");  }
        if (hasL) {    
        printf(      "FR for loose cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frLoose[0], frLoose[2], frLoose[1], frLoose[3], mcLoose, mcLooseErr);
        fprintf(log, "FR for loose cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frLoose[0], frLoose[2], frLoose[1], frLoose[3], mcLoose, mcLooseErr);
        }
        if (hasT) {
        printf(      "FR for tight cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frTight[0], frTight[2], frTight[1], frTight[3], mcTight, mcTightErr);
        fprintf(log, "FR for tight cut: %.3f +%.3f/-%.3f (stat) +/- %.3f (syst)\t FR in MC = %.3f +/- %.3f\n", frTight[0], frTight[2], frTight[1], frTight[3], mcTight, mcTightErr);
        }
    }

    fclose(log);
}


void fitFRDistsSimple(int iwhichsel=0, int iwhichid=0) {
    if (iwhichsel == 1) gPrefix = "CutBased";
    TString dPostfix = "", mPostfix = "";
    if (iwhichsel == 2) {
        gPrefix = "";
        gPostfix = "_TagMuL";
        dPostfix = gPostfix;
        mPostfix = "_SingleMu";
    }
    fQCD = TFile::Open("frDistsSimple"+gPrefix+"_QCDMuPt15"+mPostfix+".root");
    fWJ  = TFile::Open("frDistsSimple"+gPrefix+"_WJets"+mPostfix+".root");
    fDY[0]  = TFile::Open("frDistsSimple"+gPrefix+"_DYJetsM50"+mPostfix+".root");
    fDY[1]  = TFile::Open("frDistsSimple"+gPrefix+"_DYJetsM10"+mPostfix+".root");

    TFile *fOut = TFile::Open("frFitsSimple"+gPrefix+gPostfix+".root", "RECREATE");
 
#if TRIGGERING
    const int npt_mu = 8, npt_el = 6, neta = 2;
    double ptbins_mu[npt_mu+1] = { 5.0, 7.0, 8.5, 10, 15, 20, 25, 35, 80 };
    double ptbins_el[npt_el+1] = {        7,      10, 15, 20, 25, 35, 80 };
    double etabins_mu[neta+1] = { 0.0, 1.5,   2.5 };
    double etabins_el[neta+1] = { 0.0, 1.479, 2.5 };
#else
    const int npt_mu = 3, npt_el = 2, neta = 2;
    double ptbins_mu[npt_mu+1] = { 5.0, 7.0, 10, 15 };
    double ptbins_el[npt_el+1] = {      7.0, 10, 15 };
    double etabins_mu[neta+1] = { 0.0, 1.5,   2.5 };
    double etabins_el[neta+1] = { 0.0, 1.479, 2.5 };
#endif



    TH2F *FR_mu = new TH2F("FR_mu", "", npt_mu, ptbins_mu, neta, etabins_mu);
    TH2F *FT_mu = new TH2F("FR_tight_mu", "", npt_mu, ptbins_mu, neta, etabins_mu);
    TH2F *FR_el = new TH2F("FR_el", "", npt_el, ptbins_el, neta, etabins_el);
    TH2F *FT_el = new TH2F("FR_tight_el", "", npt_el, ptbins_el, neta, etabins_el);

    gROOT->ProcessLine(".x /afs/cern.ch/user/g/gpetrucc/cpp/tdrstyle.cc");
    c1 = new TCanvas("c1","c1");
    gStyle->SetOptStat(0);

    TString pdir = "ttH_plots/270413/FR_QCD_Simple/fits/"+gPrefix+gPostfix;
    gSystem->Exec("mkdir -p "+pdir);
    gSystem->Exec("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+pdir);

    double frLoose[4], frTight[4];
    for (int itype = 1; itype <= 13; itype += 12) {
        TString name0 = (itype == 13 ? "FR_MuTag_" : "FR_JetTag_");
        for (int ipdg = 11; ipdg <= 13; ipdg += 2) {
            if (iwhichid != 0 && ipdg != iwhichid) continue;
            for (int i = 0; i < ndata_max; ++i) { 
                if (fdata[i]) fdata[i]->Close(); 
                fdata[i] = NULL; 
            }
            if (gPostfix.Contains("TagMu40")) {
                    fdata[0] = TFile::Open("frDistsSimple"+gPrefix+"_SingleMuAB"+gPostfix+".root");
                    fdata[1] = TFile::Open("frDistsSimple"+gPrefix+"_SingleMuC"+gPostfix+".root");
                    fdata[2] = TFile::Open("frDistsSimple"+gPrefix+"_SingleMuD"+gPostfix+".root");
            } else if (gPostfix.Contains("TagMu")) {
                    fdata[0] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuAB"+gPostfix+".root");
                    fdata[1] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuC"+gPostfix+".root");
                    fdata[2] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuD"+gPostfix+".root");
                    fdata[3] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuRec"+gPostfix+".root");
                    fdata[4] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuBadSIP"+gPostfix+".root");
                    fdata[5] = TFile::Open("frDistsSimple"+gPrefix+"_SingleMuAB"+gPostfix+".root");
                    fdata[6] = TFile::Open("frDistsSimple"+gPrefix+"_SingleMuC"+gPostfix+".root");
                    fdata[7] = TFile::Open("frDistsSimple"+gPrefix+"_SingleMuD"+gPostfix+".root");
            } else {
                if (ipdg == 13) {
                    fdata[0] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuAB"+gPostfix+".root");
                    fdata[1] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuC"+gPostfix+".root");
                    fdata[2] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuD"+gPostfix+".root");
                    fdata[3] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuRec"+gPostfix+".root");
                    fdata[4] = TFile::Open("frDistsSimple"+gPrefix+"_DoubleMuBadSIP"+gPostfix+".root");
                } else {
                    fdata[0] = TFile::Open("frDistsSimple"+gPrefix+"_MuEGAB"+gPostfix+".root");
                    fdata[1] = TFile::Open("frDistsSimple"+gPrefix+"_MuEGC"+gPostfix+".root");
                    fdata[2] = TFile::Open("frDistsSimple"+gPrefix+"_MuEGD"+gPostfix+".root");
                    fdata[3] = TFile::Open("frDistsSimple"+gPrefix+"_MuEGRec"+gPostfix+".root");
                    fdata[4] = TFile::Open("frDistsSimple"+gPrefix+"_MuEGBadSIP"+gPostfix+".root");
                }
            }
            fOut->cd();
            TString name1 = name0 + (ipdg == 13 ? "mu_" : "el_");
            for (int ieta = 0; ieta < neta; ++ieta) {
                double *etabins = (ipdg == 11 ? etabins_el : etabins_mu);
                TString name2 = name1 + Form("eta_%.1f-%.1f_",etabins[ieta],etabins[ieta+1]);
                double *ptbins = (ipdg == 11 ? ptbins_el : ptbins_mu);
                int     npt    = (ipdg == 11 ?    npt_el :    npt_mu);
                for (int ipt = 0; ipt < npt; ++ipt) {
                    TString name = name2 + Form("pt_%.0f-%.0f",ptbins[ipt],ptbins[ipt+1]);
                    if (gPostfix.Contains("TagMu") && ptbins[ipt] >= 30) continue;
                    if (fdata[0]->Get(name+"_den") == 0) continue;
                    processOneBin(name, frLoose, frTight, ptbins[ipt]>=10);
                    TH2 *hFR = (ipdg == 11 ? FR_el : FR_mu);
                    TH2 *hFT = (ipdg == 11 ? FT_el : FT_mu);
                    double fcen = 0.5*((frLoose[0]+frLoose[2])+(frLoose[0]-frLoose[1]));
                    double fsym = 0.5*((frLoose[0]+frLoose[2])-(frLoose[0]-frLoose[1]));
                    hFR->SetBinContent(ipt+1, ieta+1, fcen);
                    hFR->SetBinError(ipt+1, ieta+1, hypot(fsym,frLoose[3]));
                    if (ptbins[ipt] >= 0) {
                        fcen = 0.5*((frTight[0]+frTight[2])+(frTight[0]-frTight[1]));
                        fsym = 0.5*((frTight[0]+frTight[2])-(frTight[0]-frTight[1]));
                        hFT->SetBinContent(ipt+1, ieta+1, fcen);
                        hFT->SetBinError(ipt+1, ieta+1, hypot(fsym,frTight[3]));
                    }
                }
            }
        }
    }
    FR_mu->Write();
    FR_el->Write();
    FT_mu->Write();
    FT_el->Write();

    fOut->Close();
}
