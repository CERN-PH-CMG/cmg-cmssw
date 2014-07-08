#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"
#include <unordered_map>

using namespace cmg;
namespace {
    ElectronEffectiveArea::ElectronEffectiveAreaTarget _electron_EA = ElectronEffectiveArea::kEleEAData2012;

    std::unordered_map<std::string,cmg::Electron::ElectronID> _electron_id_map;
    const std::unordered_map<std::string,cmg::Electron::ElectronID> & fill_electron_id_map() {
        if (_electron_id_map.empty()) {
            _electron_id_map["POG_Cuts_ID_Veto"] = cmg::Electron::POG_Cuts_ID_Veto;
            _electron_id_map["POG_Cuts_ID_Loose"] = cmg::Electron::POG_Cuts_ID_Loose;
            _electron_id_map["POG_Cuts_ID_Medium"] = cmg::Electron::POG_Cuts_ID_Medium;
            _electron_id_map["POG_Cuts_ID_Tight"] = cmg::Electron::POG_Cuts_ID_Tight;
            _electron_id_map["POG_Cuts_Full_Veto"] = cmg::Electron::POG_Cuts_Full_Veto;
            _electron_id_map["POG_Cuts_Full_Loose"] = cmg::Electron::POG_Cuts_Full_Loose;
            _electron_id_map["POG_Cuts_Full_Medium"] = cmg::Electron::POG_Cuts_Full_Medium;
            _electron_id_map["POG_Cuts_Full_Tight"] = cmg::Electron::POG_Cuts_Full_Tight;
            _electron_id_map["POG_TriggerPreselection"] = cmg::Electron::POG_TriggerPreselection;
            _electron_id_map["POG_ConvVeto_Loose"] = cmg::Electron::POG_ConvVeto_Loose;
            _electron_id_map["POG_ConvVeto_Tight"] = cmg::Electron::POG_ConvVeto_Tight;
            _electron_id_map["POG_MVA_ID_NonTrig"] = cmg::Electron::POG_MVA_ID_NonTrig;
            _electron_id_map["POG_MVA_ID_Trig"] = cmg::Electron::POG_MVA_ID_Trig;
            _electron_id_map["POG_MVA_Full_NonTrig"] = cmg::Electron::POG_MVA_Full_NonTrig;
            _electron_id_map["POG_MVA_Full_Trig"] = cmg::Electron::POG_MVA_Full_Trig;
            _electron_id_map["HZZ4L_ID"] = cmg::Electron::HZZ4L_ID;
        }
        return _electron_id_map;
    }
}

bool cmg::Electron::electronID(ElectronID id, const reco::Vertex *vtx, double rho) const 
{
    using std::abs;

    const pat::Electron &ele = **sourcePtr();
    bool isEB = ele.isEB() ? true : false;
    float pt = ele.pt();
    float eta = ele.superCluster()->eta();

    // id variables
    float dEtaIn = deltaEtaSuperClusterTrackAtVtx();
    float dPhiIn = deltaPhiSuperClusterTrackAtVtx();
    float sigmaIEtaIEta = sigmaIetaIeta();
    float hoe = hadronicOverEm();
    float ooemoop = (1.0/ele.ecalEnergy() - ele.eSuperClusterOverP()/ele.ecalEnergy());
    bool  vtxFitConversion = !passConversionVeto();
    float mHits = numberOfHits();
    // impact parameter variables
    float d0vtx = 0.0, dzvtx = 0.0;
    if (id == POG_Cuts_Full_Veto || id == POG_Cuts_Full_Loose || id == POG_Cuts_Full_Medium || id == POG_Cuts_Full_Tight) {
        if (vtx == 0)     throw cms::Exception("InvalidArgument", "POG_Cuts_Full_XXX ids require a vertex");
        d0vtx = ele.gsfTrack()->dxy(vtx->position());
        dzvtx = ele.gsfTrack()->dz(vtx->position());
    } 
    // isolation
    float iso_ch = 0.0, iso_em = 0.0, iso_nh = 0.0;
    if (id == POG_Cuts_Full_Veto || id == POG_Cuts_Full_Loose || id == POG_Cuts_Full_Medium || id == POG_Cuts_Full_Tight || id == POG_MVA_Full_NonTrig || id == POG_MVA_Full_Trig)  {
        if (isUnset(rho)) throw cms::Exception("InvalidArgument", "POG_Cuts_Full_XXX and POG_MVA_Full_XXX ids require a rho value");
        float dr = (id == POG_MVA_Full_NonTrig || id == POG_MVA_Full_Trig) ? 0.4 : 0.3;
        iso_ch = chargedHadronIso(dr);
        iso_em = photonIso(dr);
        iso_nh = neutralHadronIso(dr);
    } else {
        rho = 0;
    }
    switch(id) {
        case POG_Cuts_ID_Veto:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::VETO,   isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, 0.0,   0.0,   0.0,    0.0,    0.0,    0,                0,     0.0);
        case POG_Cuts_ID_Loose:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::LOOSE,  isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, 0.0,   0.0,   0.0,    0.0,    0.0,    0,                0,     0.0);
        case POG_Cuts_ID_Medium:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM, isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, 0.0,   0.0,   0.0,    0.0,    0.0,    0,                0,     0.0);
        case POG_Cuts_ID_Tight:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::TIGHT,  isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, 0.0,   0.0,   0.0,    0.0,    0.0,    0,                0,     0.0);
        case POG_Cuts_Full_Veto:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::VETO,   isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, d0vtx, dzvtx, iso_ch, iso_em, iso_nh, vtxFitConversion, mHits, rho);
        case POG_Cuts_Full_Loose:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::LOOSE,  isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, d0vtx, dzvtx, iso_ch, iso_em, iso_nh, vtxFitConversion, mHits, rho);
        case POG_Cuts_Full_Medium:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM, isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, d0vtx, dzvtx, iso_ch, iso_em, iso_nh, vtxFitConversion, mHits, rho);
        case POG_Cuts_Full_Tight:
            return EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::TIGHT,  isEB, pt, eta, dEtaIn, dPhiIn, sigmaIEtaIEta, hoe, ooemoop, d0vtx, dzvtx, iso_ch, iso_em, iso_nh, vtxFitConversion, mHits, rho);
        case POG_TriggerPreselection:
            return EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERTIGHT, ele);
        case POG_ConvVeto_Loose:
            return mHits <= 1;
        case POG_ConvVeto_Tight:
            return mHits == 1 && vtxFitConversion;
        case POG_MVA_ID_NonTrig:
        case HZZ4L_ID:
            if (pt < 10) {
                if      (abs(eta) < 0.8)   return mvaNonTrigV0() > +0.47f;
                else if (abs(eta) < 1.479) return mvaNonTrigV0() > +0.004f;
                else                       return mvaNonTrigV0() > +0.295f;
            } else {
                if      (abs(eta) < 0.8)   return mvaNonTrigV0() > -0.34;
                else if (abs(eta) < 1.479) return mvaNonTrigV0() > -0.65;
                else                       return mvaNonTrigV0() > +0.60;
            }
        case POG_MVA_Full_NonTrig:
            {
                float AEff = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, eta, _electron_EA);
                if (chargedHadronIso(0.4) + std::max(0.0, neutralHadronIso(0.4)+photonIso(0.4) - rho*AEff) >= 0.4*pt) return false;
                if (abs(dB3D()/edB3D()) >= 4) return false;
                if (pt < 10) {
                    if      (abs(eta) < 0.8)   return mvaNonTrigV0() > +0.47f;
                    else if (abs(eta) < 1.479) return mvaNonTrigV0() > +0.004f;
                    else                       return mvaNonTrigV0() > +0.295f;
                } else {
                    if      (abs(eta) < 0.8)   return mvaNonTrigV0() > -0.34;
                    else if (abs(eta) < 1.479) return mvaNonTrigV0() > -0.65;
                    else                       return mvaNonTrigV0() > +0.60;
                }
            }
         case POG_MVA_ID_Trig:
            if (pt < 20) {
                if      (abs(eta) < 0.8)   return mvaTrigV0() > +0.00f;
                else if (abs(eta) < 1.479) return mvaTrigV0() > +0.10f;
                else                       return mvaTrigV0() > +0.62f;
            } else {
                if      (abs(eta) < 0.8)   return mvaTrigV0() > +0.94;
                else if (abs(eta) < 1.479) return mvaTrigV0() > +0.85;
                else                       return mvaTrigV0() > +0.92;
            }
        case POG_MVA_Full_Trig:
            {
                float AEff = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, eta, _electron_EA);
                if (chargedHadronIso(0.4) + std::max(0.0, neutralHadronIso(0.4)+photonIso(0.4) - rho*AEff) >= 0.15*pt) return false;
                if (mHits != 0 || !vtxFitConversion) return false;
                if (pt < 20) {
                    if      (abs(eta) < 0.8)   return mvaTrigV0() > +0.00f;
                    else if (abs(eta) < 1.479) return mvaTrigV0() > +0.10f;
                    else                       return mvaTrigV0() > +0.62f;
                } else {
                    if      (abs(eta) < 0.8)   return mvaTrigV0() > +0.94;
                    else if (abs(eta) < 1.479) return mvaTrigV0() > +0.85;
                    else                       return mvaTrigV0() > +0.92;
                }
            }
    }
    throw cms::Exception("InvalidArgument", "Unrecognized Electron ID choice");
}
bool cmg::Electron::electronID(const std::string &id, const reco::Vertex *vtx, double rho) const 
{
    static const std::unordered_map<std::string,ElectronID> & id_map = fill_electron_id_map();
    auto match = id_map.find(id);
    if (match == id_map.end()) throw cms::Exception("InvalidArgument", id+" electron ID not known");
    return electronID(match->second, vtx, rho);
}
bool cmg::Electron::electronID(const char *id, const reco::Vertex *vtx, double rho) const 
{
    return electronID(std::string(id), vtx);
}

void cmg::Electron::SetEffectiveAreaForRhoCorrections(int year, bool data) {
    if (year == 2011) {
        if (data == 1) ::_electron_EA = ElectronEffectiveArea::kEleEAData2011;
        else           ::_electron_EA = ElectronEffectiveArea::kEleEAFall11MC;
    } else if (year == 2012) {
        if (data == 1) ::_electron_EA = ElectronEffectiveArea::kEleEAData2011;
        else throw cms::Exception("InvalidArgument", "2012 Electron effective areas are available only for data, use those for MC as well.\n");
    } else {
        throw cms::Exception("InvalidArgument", "Electron effective areas are available only for 2012 (data) and 2011 (data, mc)\n");
    }
}

/// charged hadron vetoes    
reco::isodeposit::AbsVetos Electron::chargedHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  if( fabs( (*(this->sourcePtr()))->superCluster()->eta() ) > 1.479 ){
    vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.015 ));
  }
  return vetos;
}
/// charged particle vetoes (e, mu, h+-)
reco::isodeposit::AbsVetos Electron::chargedAllVetos() const {
  return chargedHadronVetos();
}
/// pile-up charged hadron vetoes, used for dbeta corrections
reco::isodeposit::AbsVetos Electron::puChargedHadronVetos() const {
  return chargedHadronVetos();
}
/// neutral hadron vetoes
reco::isodeposit::AbsVetos Electron::neutralHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  return vetos;
}
/// photon vetoes
reco::isodeposit::AbsVetos Electron::photonVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  if( fabs( (*(this->sourcePtr()))->superCluster()->eta() ) > 1.479 ){
    vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.08 ));
  }
  return vetos;
}
