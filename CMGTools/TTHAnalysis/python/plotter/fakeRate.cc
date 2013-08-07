#include <TH2.h>
#include <TFile.h>
#include <cmath>
#include <iostream>
#include <string>
#include <map>

TH2 * FR_mu = 0;
TH2 * FR2_mu = 0;
TH2 * FR_el = 0;
TH2 * FR2_el = 0;
TH2 * QF_el = 0;


bool loadFRHisto(const std::string &histoName, const char *file, const char *name) {
    TH2 **histo = 0;
    if (histoName == "FR_mu") histo = & FR_mu;
    if (histoName == "FR_el") histo = & FR_el;
    if (histoName == "FR2_mu") histo = & FR2_mu;
    if (histoName == "FR2_el") histo = & FR2_el;
    if (histoName == "QF_el") histo = & QF_el;
    if (histo == 0)  {
        std::cerr << "ERROR: histogram " << histoName << " is not defined in fakeRate.cc." << std::endl;
        return 0;
    }

    if (*histo != 0) delete *histo;
    TFile *f = TFile::Open(file);
    if (f->Get(name) == 0) {
        std::cerr << "ERROR: could not find " << name << " in " << file << std::endl;
        *histo = 0;
    } else {
        *histo = (TH2*) f->Get(name)->Clone(name);
        (*histo)->SetDirectory(0);
    }
    f->Close();
    return histo != 0;
}

float fakeRateWeight_2lss(float l1pt, float l1eta, int l1pdgId, float l1mva,
                         float l2pt, float l2eta, int l2pdgId, float l2mva, float WP) 
{
    int nfail = (l1mva < WP)+(l2mva < WP);
    switch (nfail) {
        case 1: {
            double fpt,feta; int fid;
            if (l1mva < l2mva) { fpt = l1pt; feta = std::abs(l1eta); fid = abs(l1pdgId); }
            else               { fpt = l2pt; feta = std::abs(l2eta); fid = abs(l2pdgId); }
            TH2 *hist = (fid == 11 ? FR_el : FR_mu);
            int ptbin  = std::max(1, std::min(hist->GetNbinsX(), hist->GetXaxis()->FindBin(fpt)));
            int etabin = std::max(1, std::min(hist->GetNbinsY(), hist->GetYaxis()->FindBin(feta)));
            double fr = hist->GetBinContent(ptbin,etabin);
            return fr/(1-fr);
        }
        case 2: {
            TH2 *hist1 = (abs(l1pdgId) == 11 ? FR_el : FR_mu);
            int ptbin1  = std::max(1, std::min(hist1->GetNbinsX(), hist1->GetXaxis()->FindBin(l1pt)));
            int etabin1 = std::max(1, std::min(hist1->GetNbinsY(), hist1->GetYaxis()->FindBin(std::abs(l1eta))));
            double fr1 = hist1->GetBinContent(ptbin1,etabin1);
            TH2 *hist2 = (abs(l2pdgId) == 11 ? FR_el : FR_mu);
            int ptbin2  = std::max(1, std::min(hist2->GetNbinsX(), hist2->GetXaxis()->FindBin(l2pt)));
            int etabin2 = std::max(1, std::min(hist2->GetNbinsY(), hist2->GetYaxis()->FindBin(std::abs(l2eta))));
            double fr2 = hist2->GetBinContent(ptbin2,etabin2);
            return -fr1*fr2/((1-fr1)*(1-fr2));
        }
        default: return 0;
    }
}
float fakeRateWeight_2lssSyst(float l1pt, float l1eta, int l1pdgId, float l1mva,
                         float l2pt, float l2eta, int l2pdgId, float l2mva, float WP, 
                         float mu_barrel_lowpt, float mu_barrel_highpt, float mu_endcap_lowpt, float mu_endcap_highpt,
                         float el_barrel_lowpt, float el_barrel_highpt, float el_endcap_lowpt, float el_endcap_highpt)
{
    int nfail = (l1mva < WP)+(l2mva < WP);
    switch (nfail) {
        case 1: {
            double fpt,feta; int fid;
            if (l1mva < l2mva) { fpt = l1pt; feta = std::abs(l1eta); fid = abs(l1pdgId); }
            else               { fpt = l2pt; feta = std::abs(l2eta); fid = abs(l2pdgId); }
            TH2 *hist = (fid == 11 ? FR_el : FR_mu);
            int ptbin  = std::max(1, std::min(hist->GetNbinsX(), hist->GetXaxis()->FindBin(fpt)));
            int etabin = std::max(1, std::min(hist->GetNbinsY(), hist->GetYaxis()->FindBin(feta)));
            double fr = hist->GetBinContent(ptbin,etabin);
            if (fid == 11) fr *= (feta < 1.5 ? (fpt < 20 ? el_barrel_lowpt : el_barrel_highpt) :
                                               (fpt < 20 ? el_endcap_lowpt : el_endcap_highpt) );
            else /*==13*/  fr *= (feta < 1.5 ? (fpt < 20 ? mu_barrel_lowpt : mu_barrel_highpt) :
                                               (fpt < 20 ? mu_endcap_lowpt : mu_endcap_highpt) );
            return fr/(1-fr);
        }
        case 2: {
            TH2 *hist1 = (abs(l1pdgId) == 11 ? FR_el : FR_mu);
            int ptbin1  = std::max(1, std::min(hist1->GetNbinsX(), hist1->GetXaxis()->FindBin(l1pt)));
            int etabin1 = std::max(1, std::min(hist1->GetNbinsY(), hist1->GetYaxis()->FindBin(std::abs(l1eta))));
            double fr1 = hist1->GetBinContent(ptbin1,etabin1);
            if (abs(l1pdgId) == 11) fr1 *= (std::abs(l1eta) < 1.5 ? (l1pt < 20 ? el_barrel_lowpt : el_barrel_highpt) :
                                                                    (l1pt < 20 ? el_endcap_lowpt : el_endcap_highpt) );
            else /*==13*/           fr1 *= (std::abs(l1eta) < 1.5 ? (l1pt < 20 ? mu_barrel_lowpt : mu_barrel_highpt) :
                                                                    (l1pt < 20 ? mu_endcap_lowpt : mu_endcap_highpt) );
            TH2 *hist2 = (abs(l2pdgId) == 11 ? FR_el : FR_mu);
            int ptbin2  = std::max(1, std::min(hist2->GetNbinsX(), hist2->GetXaxis()->FindBin(l2pt)));
            int etabin2 = std::max(1, std::min(hist2->GetNbinsY(), hist2->GetYaxis()->FindBin(std::abs(l2eta))));
            double fr2 = hist2->GetBinContent(ptbin2,etabin2);
            if (abs(l2pdgId) == 11) fr2 *= (std::abs(l2eta) < 1.5 ? (l2pt < 20 ? el_barrel_lowpt : el_barrel_highpt) :
                                                                    (l2pt < 20 ? el_endcap_lowpt : el_endcap_highpt) );
            else /*==13*/           fr2 *= (std::abs(l2eta) < 1.5 ? (l2pt < 20 ? mu_barrel_lowpt : mu_barrel_highpt) :
                                                                    (l2pt < 20 ? mu_endcap_lowpt : mu_endcap_highpt) );
            return -fr1*fr2/((1-fr1)*(1-fr2));
        }
        default: return 0;
    }
}

bool passND_LooseDen(float l1pt, float l1eta, int l1pdgId, float relIso, float dxy, float dz, float tightId) 
{
    if (fabs(l1pdgId) == 13) {
        return l1pt >= 10;
    } else {
        return l1pt >= 10 && (fabs(l1eta)<1.4442 || fabs(l1eta)>1.5660);
    }
}

bool passND_Loose(float l1pt, float l1eta, int l1pdgId, float relIso, float dxy, float dz, float tightId) 
{
    if (fabs(l1pdgId) == 13) {
        return l1pt >= 10 && 
               relIso < 0.2;
    } else {
        return l1pt >= 10 && (fabs(l1eta)<1.4442 || fabs(l1eta)>1.5660) &&
               tightId > 0.5 && relIso < 0.2 && fabs(dxy) < 0.04;
    }
}

bool passND_TightDen(float l1pt, float l1eta, int l1pdgId, float relIso, float dxy, float dz, float tightId) 
{
    if (fabs(l1pdgId) == 13) {
        return l1pt >= 20 && fabs(l1eta) <= 2.1;
    } else {
        return l1pt >= 20 && (fabs(l1eta)<1.4442 || fabs(l1eta)>1.5660);
    }
}

bool passND_Tight(float l1pt, float l1eta, int l1pdgId, float relIso, float dxy, float dz, float tightId) 
{
    if (fabs(l1pdgId) == 13) {
        return l1pt >= 20 && fabs(l1eta) <= 2.1 && 
               tightId != 0 && relIso < 0.12 && fabs(dxy) < 0.2 && fabs(dz) < 0.5;
    } else {
        return l1pt >= 20 && (fabs(l1eta)<1.4442 || fabs(l1eta)>1.5660) &&
               tightId > 0.5 && relIso < 0.1 && fabs(dxy) < 0.02;
    }
}

float fakeRateWeight_2lss_ND(float l1pt, float l1eta, int l1pdgId, float l1relIso, float l1dxy, float l1dz, float l1tightId,
                          float l2pt, float l2eta, int l2pdgId, float l2relIso, float l2dxy, float l2dz, float l2tightId, int WP) 
{
    switch (WP) {
        case 11: {// loose-loose
            bool l1L = passND_Loose(l1pt, l1eta, l1pdgId, l1relIso, l1dxy, l1dz, l1tightId);
            bool l2L = passND_Loose(l2pt, l2eta, l2pdgId, l2relIso, l2dxy, l2dz, l2tightId);
            TH2 *hist1 = (abs(l1pdgId) == 11 ? FR_el : FR_mu);
            int ptbin1  = std::max(1, std::min(hist1->GetNbinsX(), hist1->GetXaxis()->FindBin(l1pt)));
            int etabin1 = std::max(1, std::min(hist1->GetNbinsY(), hist1->GetYaxis()->FindBin(std::abs(l1eta))));
            double fr1 = hist1->GetBinContent(ptbin1,etabin1);
            TH2 *hist2 = (abs(l2pdgId) == 11 ? FR_el : FR_mu);
            int ptbin2  = std::max(1, std::min(hist2->GetNbinsX(), hist2->GetXaxis()->FindBin(l2pt)));
            int etabin2 = std::max(1, std::min(hist2->GetNbinsY(), hist2->GetYaxis()->FindBin(std::abs(l2eta))));
            double fr2 = hist2->GetBinContent(ptbin2,etabin2);
            if      ( l1L &&  l2L) return 0;
            else if ( l1L && !l2L) return fr2/(1-fr2);
            else if (!l1L &&  l2L) return fr1/(1-fr1);
            else if (!l1L && !l2L) return -fr1*fr2/((1-fr1)*(1-fr2));
        }; 
        case 22: {// tight-tight 
            bool l1T = passND_Tight(l1pt, l1eta, l1pdgId, l1relIso, l1dxy, l1dz, l1tightId);
            bool l2T = passND_Tight(l2pt, l2eta, l2pdgId, l2relIso, l2dxy, l2dz, l2tightId);
            TH2 *hist1 = (abs(l1pdgId) == 11 ? FR2_el : FR2_mu);
            int ptbin1  = std::max(1, std::min(hist1->GetNbinsX(), hist1->GetXaxis()->FindBin(l1pt)));
            int etabin1 = std::max(1, std::min(hist1->GetNbinsY(), hist1->GetYaxis()->FindBin(std::abs(l1eta))));
            double fr1 = hist1->GetBinContent(ptbin1,etabin1);
            TH2 *hist2 = (abs(l2pdgId) == 11 ? FR2_el : FR2_mu);
            int ptbin2  = std::max(1, std::min(hist2->GetNbinsX(), hist2->GetXaxis()->FindBin(l2pt)));
            int etabin2 = std::max(1, std::min(hist2->GetNbinsY(), hist2->GetYaxis()->FindBin(std::abs(l2eta))));
            double fr2 = hist2->GetBinContent(ptbin2,etabin2);
            if      ( l1T &&  l2T) return 0;
            else if ( l1T && !l2T) return fr2/(1-fr2);
            else if (!l1T &&  l2T) return fr1/(1-fr1);
            else if (!l1T && !l2T) return -fr1*fr2/((1-fr1)*(1-fr2));
        }; 
        default: {
            static int _once = 0;
            if (_once++ == 0) { std::cerr << "ERROR, unknown WP " << WP << std::endl; }
        }

    }
    return 0;
}

float chargeFlipWeight_2lss(float l1pt, float l1eta, int l1pdgId, 
                             float l2pt, float l2eta, int l2pdgId) 
{
    if (l1pdgId * l2pdgId > 0) return 0.;
    double w = 0;
    if (abs(l1pdgId) == 11) {
        int ptbin  = std::max(1, std::min(QF_el->GetNbinsX(), QF_el->GetXaxis()->FindBin(l1pt)));
        int etabin = std::max(1, std::min(QF_el->GetNbinsY(), QF_el->GetYaxis()->FindBin(std::abs(l1eta))));
        w += QF_el->GetBinContent(ptbin,etabin);
    }
    if (abs(l2pdgId) == 11) {
        int ptbin  = std::max(1, std::min(QF_el->GetNbinsX(), QF_el->GetXaxis()->FindBin(l2pt)));
        int etabin = std::max(1, std::min(QF_el->GetNbinsY(), QF_el->GetYaxis()->FindBin(std::abs(l2eta))));
        w += QF_el->GetBinContent(ptbin,etabin);
    }
    return w;
}

float chargeFlipBin_2lss(float l1pt, float l1eta) {
    if (std::abs(l1eta) < 1.479) {
        return (l1pt < 20 ? 0 : (l1pt < 50 ? 1 : 2));
    } else {
        return (l1pt < 20 ? 3 : (l1pt < 50 ? 4 : 5));
    }
}


float fakeRateWeight_3l(float l1pt, float l1eta, int l1pdgId, float l1mva,
                        float l2pt, float l2eta, int l2pdgId, float l2mva,
                        float l3pt, float l3eta, int l3pdgId, float l3mva,
                        float WP)
{
    /// 3 pass: weight  0
    /// 1 fail: weight +f/(1-f)
    /// 2 fail: weight -f*f/(1-f)(1-f)
    //  3 fail: weight +f*f*f/((1-f)(1-f)(1-f)
    //  so, just multiply up factors of -f/(1-f) for each failure
    float mvas[]={l1mva, l2mva, l3mva};
    float pts[]={l1pt, l2pt, l3pt};
    float etas[]={fabs(l1eta), fabs(l2eta), fabs(l3eta)};
    int pdgids[]={l1pdgId, l2pdgId, l3pdgId};
    float ret = -1.0f;
    for (unsigned int i = 0; i < 3 ; ++i) {
        if (mvas[i] < WP) {
	    TH2 *hist = (abs(pdgids[i]) == 11 ? FR_el : FR_mu);
            int ptbin  = std::max(1, std::min(hist->GetNbinsX(), hist->GetXaxis()->FindBin(pts[i])));
            int etabin = std::max(1, std::min(hist->GetNbinsY(), hist->GetYaxis()->FindBin(etas[i])));
            double fr = hist->GetBinContent(ptbin,etabin);
            ret *= -fr/(1.0f-fr);
        }
    }
    if (ret == -1.0f) ret = 0.0f;
    return ret;
}

float fakeRateWeight_4l_2wp(float l1pt, float l1eta, int l1pdgId, float l1mva,
                        float l2pt, float l2eta, int l2pdgId, float l2mva,
                        float l3pt, float l3eta, int l3pdgId, float l3mva,
                        float l4pt, float l4eta, int l4pdgId, float l4mva,
                        float WP, float WP2)
{
    /// 4 pass: weight  0
    /// 1 fail: weight +f/(1-f)
    /// 2 fail: weight -f*f/(1-f)(1-f)
    //  3 fail: weight +f*f*f/((1-f)(1-f)(1-f)
    //  so, just multiply up factors of -f/(1-f) for each failure
    //  hope it works also for 4l....
    float mvas[]={l1mva-WP, l2mva-WP, l3mva-WP2, l4mva-WP2};
    float pts[]={l1pt, l2pt, l3pt, l4pt};
    float etas[]={fabs(l1eta), fabs(l2eta), fabs(l3eta), fabs(l4eta)};
    int pdgids[]={l1pdgId, l2pdgId, l3pdgId, l4pdgId};
    float ret = -1.0f;
    for (unsigned int i = 0; i < 4 ; ++i) {
        if (mvas[i] < 0) {
	    TH2 *hist = (i <= 1 ? (abs(pdgids[i]) == 11 ? FR_el : FR_mu) : (abs(pdgids[i]) == 11 ? FR2_el : FR2_mu));
            int ptbin  = std::max(1, std::min(hist->GetNbinsX(), hist->GetXaxis()->FindBin(pts[i])));
            int etabin = std::max(1, std::min(hist->GetNbinsY(), hist->GetYaxis()->FindBin(etas[i])));
            double fr = hist->GetBinContent(ptbin,etabin);
            ret *= -fr/(1.0f-fr);
        }
    }
    if (ret == -1.0f) ret = 0.0f;
    return ret;
}



void fakeRate() {}
