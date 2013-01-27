#include <smearer.h>
#include <cmath>
using std::abs;
using std::exp;
using std::log;

double scaleIpVarsMC(double ipvar, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny) {
    if (abs(pdgId) == 13) {
        if (mcMatchId > 0 || mcMatchAny <= 1) {
            return ipvar * (abs(eta) < 1.5 ? 1.04 : 1.10);
        } else {
            return ipvar * 0.95;
        }
    } else {
        if (mcMatchId > 0 || mcMatchAny <= 1) {
            return ipvar * (abs(eta) < 1.479 ? 1.02 : 1.07);
        } else {
            return ipvar * 0.95;
        }
    }
}
double scaleSip3dMC(double sip3d, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny) {
    return scaleIpVarsMC(sip3d,pdgId,pt,eta,mcMatchId,mcMatchAny);
}
double scaleDzMC(double dz, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny) {
    return scaleIpVarsMC(dz,pdgId,pt,eta,mcMatchId,mcMatchAny);
}
double scaleDxyMC(double dxy, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny) {
    return scaleIpVarsMC(dxy,pdgId,pt,eta,mcMatchId,mcMatchAny);
}

void mcCorrections() {}
