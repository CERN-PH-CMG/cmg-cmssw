#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <unordered_map>

using namespace cmg;

namespace {
    std::unordered_map<std::string,cmg::Muon::MuonID> _muon_id_map;
    const std::unordered_map<std::string,cmg::Muon::MuonID> & fill_muon_id_map() {
        if (_muon_id_map.empty()) {
            _muon_id_map["POG_ID_Soft"] = cmg::Muon::POG_ID_Soft;
            _muon_id_map["POG_ID_Loose"] = cmg::Muon::POG_ID_Loose;
            _muon_id_map["POG_ID_Tight"] = cmg::Muon::POG_ID_Tight;
            _muon_id_map["POG_ID_HighPt"] = cmg::Muon::POG_ID_HighPt;
            _muon_id_map["POG_PFIso_Loose"] = cmg::Muon::POG_PFIso_Loose;
            _muon_id_map["POG_PFIso_Tight"] = cmg::Muon::POG_PFIso_Tight;
        }
        return _muon_id_map;
    }
}

bool cmg::Muon::muonID(MuonID id, const reco::Vertex *vtx) const 
{
    const pat::Muon &mu = **sourcePtr();
    switch(id) {
        case POG_ID_Loose:  
                return muon::isLooseMuon(mu);
        case POG_ID_Soft:   
                if (vtx == 0) throw cms::Exception("InvalidArgument", "POG_ID_Soft requires a vertex");
                return muon::isSoftMuon(mu, *vtx);
        case POG_ID_Tight:  
                if (vtx == 0) throw cms::Exception("InvalidArgument", "POG_ID_Tight requires a vertex");
                return muon::isTightMuon(mu, *vtx);
        case POG_ID_HighPt: 
                if (vtx == 0) throw cms::Exception("InvalidArgument", "POG_ID_HighPt requires a vertex");
                throw cms::Exception("NotImplemented", "Improved tune P not yet implemented for 626");
                return false;
                // return muon::isHighPtMuon(mu, *vtx, reco::improvedTuneP);
        case POG_PFIso_Loose:
                return (mu.pfIsolationR04().sumChargedHadronPt + std::max(mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt,0.0)) < 0.20 * mu.pt();
        case POG_PFIso_Tight:
                return (mu.pfIsolationR04().sumChargedHadronPt + std::max(mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5*mu.pfIsolationR04().sumPUPt,0.0)) < 0.12 * mu.pt();
    }
    throw cms::Exception("InvalidArgument", "Unrecognized Muon ID choice");
}
bool cmg::Muon::muonID(const std::string &id, const reco::Vertex *vtx) const 
{
    static const std::unordered_map<std::string,MuonID> & id_map = fill_muon_id_map();
    auto match = id_map.find(id);
    if (match == id_map.end()) throw cms::Exception("InvalidArgument", id+" muon ID not known");
    return muonID(match->second, vtx);
}
bool cmg::Muon::muonID(const char *id, const reco::Vertex *vtx) const 
{
    return muonID(std::string(id), vtx);
}


/// charged hadron vetoes    
reco::isodeposit::AbsVetos Muon::chargedHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.0001 ));
  return vetos;
}
/// charged particle vetoes (e, mu, h+-)
reco::isodeposit::AbsVetos Muon::chargedAllVetos() const {
  return chargedHadronVetos();
}
/// pile-up charged hadron vetoes, used for dbeta corrections
reco::isodeposit::AbsVetos Muon::puChargedHadronVetos() const {
  return chargedHadronVetos();
}
/// neutral hadron vetoes
reco::isodeposit::AbsVetos Muon::neutralHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.01 ));
  vetos.push_back(new reco::isodeposit::ThresholdVeto( 0.5 ));
  return vetos;
}
/// photon vetoes
reco::isodeposit::AbsVetos Muon::photonVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.01 ));
  vetos.push_back(new reco::isodeposit::ThresholdVeto( 0.5 ));
  return vetos;
}
