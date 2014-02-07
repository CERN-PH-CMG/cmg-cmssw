#ifndef ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

#include <cmath>
#include <vector>

namespace cmg
{

//forward def needed
class Electron;
class ElectronFactory;

class Electron : public cmg::Lepton<pat::ElectronPtr>{
 public:
  
  Electron(){}

  Electron(const value& e):
    cmg::Lepton<value>::Lepton(e),
    mva_(UnSet(float)),
    mvaTrigV0_(UnSet(float)),
    mvaNonTrigV0_(UnSet(float)),
    sigmaIetaIeta_(UnSet(Double_t)),
    deltaPhiSuperClusterTrackAtVtx_(UnSet(Double_t)),
    deltaEtaSuperClusterTrackAtVtx_(UnSet(Double_t)),
    hadronicOverEm_(UnSet(Double_t)),
    numberOfHits_(UnSet(Double_t)),
    convDist_(UnSet(Double_t)),
    convDcot_(UnSet(Double_t)),
    isEcalDriven_(UnSet(cmg::TriBool)),
    chargedAllIsoWithConeVeto_(UnSet(Double_t)),
    passConversionVeto_(UnSet(bool)){
  }

    virtual ~Electron(){
    }
    
    float mva() const{
      return mva_;
    }
    float mvaTrigV0() const{
      return mvaTrigV0_;
    }
    float mvaNonTrigV0() const{
      return mvaNonTrigV0_;
    }
    Double_t sigmaIetaIeta() const{
      return sigmaIetaIeta_;
    }
    Double_t deltaPhiSuperClusterTrackAtVtx() const{
      return deltaPhiSuperClusterTrackAtVtx_;
    }
    Double_t deltaEtaSuperClusterTrackAtVtx() const{
      return deltaEtaSuperClusterTrackAtVtx_;
    }
    Double_t hadronicOverEm() const{
      return hadronicOverEm_;
    }
    Double_t numberOfHits() const{
      return numberOfHits_;
    }
    Double_t convDist() const{
        return convDist_;   
    }
    Double_t convDcot() const{
        return convDcot_;
    }
    cmg::TriBool isConv() const{
        cmg::TriBool result = UnSet(cmg::TriBool);
        if(!cmg::isUnset(convDist()) && !cmg::isUnset(convDcot())){
            result = cmg::toTriBool(fabs(convDist()) < 0.02 && fabs(convDcot()) < 0.02);
        }
        return result;
    }
    cmg::TriBool ecalDriven() const{
        return isEcalDriven_;
    }
    // from LeafCandidate
    virtual bool isElectron() const {
      return true;
    }
    bool passConversionVeto() const {
      return passConversionVeto_;
    }
    Double_t chargedAllIsoWithConeVeto() const {
      return chargedAllIsoWithConeVeto_;
    }

    /// charged hadron vetoes    
    reco::isodeposit::AbsVetos chargedHadronVetos() const;
    /// charged particle vetoes (e, mu, h+-)
    reco::isodeposit::AbsVetos chargedAllVetos() const;
    /// pile-up charged hadron vetoes, used for dbeta corrections
    reco::isodeposit::AbsVetos puChargedHadronVetos() const;
    /// neutral hadron vetoes
    reco::isodeposit::AbsVetos neutralHadronVetos() const;
    /// photon vetoes
    reco::isodeposit::AbsVetos photonVetos() const;
    
private:
    
    // That's the PF mva
    float mva_;
    // That's new mva for trigger object
    float mvaTrigV0_;
    // That's new mva for non-trigger object
    float mvaNonTrigV0_;
    // variables needed for WP selections
    Double_t sigmaIetaIeta_;
    Double_t deltaPhiSuperClusterTrackAtVtx_;
    Double_t deltaEtaSuperClusterTrackAtVtx_;
    Double_t hadronicOverEm_;
    Double_t numberOfHits_;
    Double_t convDist_;
    Double_t convDcot_;
    cmg::TriBool isEcalDriven_;
    Double_t chargedAllIsoWithConeVeto_;
    bool passConversionVeto_;
          
    friend class cmg::ElectronFactory;    

};

}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_ELECTRON_H_*/
