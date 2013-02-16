#include <TH2.h>
#include <TFile.h>
#include <cmath>
#include <iostream>
#include <string>

TH2 * FR_2lss_mu = 0;
TH2 * FR_2lss_el = 0;

bool loadFRHisto(const std::string &histoName, const char *file, const char *name) {
    TH2 **histo = 0;
    if (histoName == "FR_2lss_mu") histo = & FR_2lss_mu;
    if (histoName == "FR_2lss_el") histo = & FR_2lss_el;
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
                         float l2pt, float l2eta, int l2pdgId, float l2mva) 
{
    int nfail = (l1mva < -0.2)+(l2mva < -0.2);
    switch (nfail) {
        case 1: {
            double fpt,feta; int fid;
            if (l1mva < l2mva) { fpt = l1pt; feta = std::abs(l1eta); fid = abs(l1pdgId); }
            else               { fpt = l2pt; feta = std::abs(l2eta); fid = abs(l2pdgId); }
            TH2 *hist = (fid == 11 ? FR_2lss_el : FR_2lss_mu);
            int ptbin  = std::max(1, std::min(hist->GetNbinsX(), hist->GetXaxis()->FindBin(fpt)));
            int etabin = std::max(1, std::min(hist->GetNbinsY(), hist->GetYaxis()->FindBin(feta)));
            double fr = hist->GetBinContent(ptbin,etabin);
            return fr/(1-fr);
        }
        case 2: {
            TH2 *hist1 = (abs(l1pdgId) == 11 ? FR_2lss_el : FR_2lss_mu);
            int ptbin1  = std::max(1, std::min(hist1->GetNbinsX(), hist1->GetXaxis()->FindBin(l1pt)));
            int etabin1 = std::max(1, std::min(hist1->GetNbinsY(), hist1->GetYaxis()->FindBin(std::abs(l1eta))));
            double fr1 = hist1->GetBinContent(ptbin1,etabin1);
            TH2 *hist2 = (abs(l2pdgId) == 11 ? FR_2lss_el : FR_2lss_mu);
            int ptbin2  = std::max(1, std::min(hist2->GetNbinsX(), hist2->GetXaxis()->FindBin(l2pt)));
            int etabin2 = std::max(1, std::min(hist2->GetNbinsY(), hist2->GetYaxis()->FindBin(std::abs(l2eta))));
            double fr2 = hist2->GetBinContent(ptbin2,etabin2);
            return fr1*fr2/((1-fr1)*(1-fr2));
        }
        default: return 0;
    }
}


void fakeRate() {}
