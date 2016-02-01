#include <cmath>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"


int xbins2D, ybins2D;
float xmin2D, xmax2D, ymin2D, ymax2D; 
TH2* binning2D_; 
std::vector<float> ptCorr, ptErrCorr;
TH2* ptCorr2DI, *ptErrCorr2DI;

void initBinning2D(int nbinsx, const float *binsx, int nbinsy, const float *binsy, bool makeMassErrPlots=false) {
    xbins2D = nbinsx; ybins2D = nbinsy;
    xmin2D = binsx[0]+1e-3; xmax2D = binsx[nbinsx]-1.e-3;
    ymin2D = binsy[0]+1e-3; ymax2D = binsy[nbinsy]-1.e-3;
    binning2D_ = new TH2F("binning2D", "binning2D",  nbinsx, binsx, nbinsy, binsy) ;
    ptCorr2DI = new TH2D("ptCorr2DI", "ptCorr2DI",  nbinsx, binsx, nbinsy, binsy) ;
    ptErrCorr2DI = new TH2D("ptErrCorr2DI", "ptErrCorr2DI",  nbinsx, binsx, nbinsy, binsy) ;
    binning2D_->SetDirectory(0);
    ptCorr2DI->SetDirectory(0);
    ptErrCorr2DI->SetDirectory(0);
    ptCorr    = std::vector<float>(nbinsx*nbinsy, 1.f);
    ptErrCorr = std::vector<float>(nbinsx*nbinsy, 1.f);
    for (int ix = 1; ix <= nbinsx; ++ix) { for (int iy = 1; iy <= nbinsy; ++iy) {
        ptCorr2DI->SetBinContent(ix,iy, 1.0);
        ptErrCorr2DI->SetBinContent(ix,iy, 1.0);
    } }
}
void updateCorr2D(int xbin, int ybin, float ptCorrScale, float ptErrCorrScale) {
    ptCorr[xbin+xbins2D*ybin] *= ptCorrScale;
    ptErrCorr[xbin+xbins2D*ybin] *= ptErrCorrScale;
    ptCorr2DI->SetBinContent(xbin+1, ybin+1, ptCorr[xbin+xbins2D*ybin]);
    ptErrCorr2DI->SetBinContent(xbin+1, ybin+1, ptErrCorr[xbin+xbins2D*ybin]);
}
float getPtCorr2D(int xbin, int ybin) { return ptCorr[xbin+xbins2D*ybin]; }
float getPtErrCorr2D(int xbin, int ybin) { return ptErrCorr[xbin+xbins2D*ybin]; }

int bin2D(float pt, float eta) {
    int binx = min(max(binning2D_->GetXaxis()->FindBin(pt),  1), xbins2D)-1;
    int biny = min(max(binning2D_->GetYaxis()->FindBin(eta), 1), ybins2D)-1;
    return binx+xbins2D*biny;
}
int bins2D() {
    return xbins2D*ybins2D;
}
float massCorr2D(float pt1, float ptErr1, float eta1, float phi1, float mass1, float pt2, float ptErr2, float eta2, float phi2) {
    int bin1 = bin2D(pt1,eta1), bin2 = bin2D(pt2,eta2);
    typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMVector;
    PtEtaPhiMVector p41(pt1*ptCorr[bin1],eta1,phi1,mass1);
    PtEtaPhiMVector p42(pt2*ptCorr[bin2],eta2,phi2,mass1);
    return (p41+p42).M();
}
float massCorr2DI(float pt1, float ptErr1, float eta1, float phi1, float mass1, float pt2, float ptErr2, float eta2, float phi2) {
    float ptCorr1 = ptCorr2DI->Interpolate(std::min(std::max(pt1,xmin2D),xmax2D),std::min(std::max(eta1,ymin2D),ymax2D));
    float ptCorr2 = ptCorr2DI->Interpolate(std::min(std::max(pt2,xmin2D),xmax2D),std::min(std::max(eta2,ymin2D),ymax2D));
    typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMVector;
    PtEtaPhiMVector p41(pt1*ptCorr1,eta1,phi1,mass1);
    PtEtaPhiMVector p42(pt2*ptCorr2,eta2,phi2,mass1);
    return (p41+p42).M();
}
float massErrCorr2D(float pt1, float ptErr1, float eta1, float phi1, float mass1, float pt2, float ptErr2, float eta2, float phi2) {
    int bin1 = bin2D(pt1,eta1), bin2 = bin2D(pt2,eta2);
    typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMVector;
    PtEtaPhiMVector p41(pt1*ptCorr[bin1],eta1,phi1,mass1);
    PtEtaPhiMVector p42(pt2*ptCorr[bin2],eta2,phi2,mass1);
    float mass = (p41+p42).M();
    return 0.5 * mass * hypot((ptErr1/pt1)*ptErrCorr[bin1], (ptErr2/pt2)*ptErrCorr[bin2]);
}
float massErrCorr2DI(float pt1, float ptErr1, float eta1, float phi1, float mass1, float pt2, float ptErr2, float eta2, float phi2) {
    float ptCorr1 = ptCorr2DI->Interpolate(std::min(std::max(pt1,xmin2D),xmax2D),std::min(std::max(eta1,ymin2D),ymax2D));
    float ptCorr2 = ptCorr2DI->Interpolate(std::min(std::max(pt2,xmin2D),xmax2D),std::min(std::max(eta2,ymin2D),ymax2D));
    float ptErrCorr1 = ptErrCorr2DI->Interpolate(std::min(std::max(pt1,xmin2D),xmax2D),std::min(std::max(eta1,ymin2D),ymax2D));
    float ptErrCorr2 = ptErrCorr2DI->Interpolate(std::min(std::max(pt2,xmin2D),xmax2D),std::min(std::max(eta2,ymin2D),ymax2D));
    typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMVector;
    PtEtaPhiMVector p41(pt1*ptCorr1,eta1,phi1,mass1);
    PtEtaPhiMVector p42(pt2*ptCorr2,eta2,phi2,mass1);
    float mass = (p41+p42).M();
    return 0.5 * mass * hypot((ptErr1/pt1)*ptErrCorr1, (ptErr2/pt2)*ptErrCorr2);
}


void iterativeLeptonCalibration() {}
