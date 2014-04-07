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

    /// The POG_xx_ID_yy are only the ID part, not including isolation, impact parameter and conv veto
    ///     POG_xx_Full_yy include also the isolation, impact parameter and conv. veto
    ///     POG_ConvVeto_Loose = (missing hits <= 1)
    ///     POG_ConvVeto_Tight = (missing hits == 1 && vertex-based conversion)
    enum ElectronID { POG_Cuts_ID_Veto,   POG_Cuts_Full_Veto,
                      POG_Cuts_ID_Loose,  POG_Cuts_Full_Loose,
                      POG_Cuts_ID_Medium, POG_Cuts_Full_Medium, 
                      POG_Cuts_ID_Tight,  POG_Cuts_Full_Tight, 
                      POG_TriggerPreselection,
                      POG_ConvVeto_Loose, POG_ConvVeto_Tight,
                      POG_MVA_ID_NonTrig, POG_MVA_Full_NonTrig,
                      POG_MVA_ID_Trig,    POG_MVA_Full_Trig,
                      HZZ4L_ID// == POG_MVA_ID_NonTrig 
                    };
    bool electronID(ElectronID id, const reco::Vertex *vtx = 0, double rho=UnSet(double)) const ;
    bool electronID(const std::string &id, const reco::Vertex *vtx = 0, double rho=UnSet(double)) const ;
    bool electronID(const char        *id, const reco::Vertex *vtx = 0, double rho=UnSet(double)) const ;
    /// provide a copy of it so that we can override electronID in python and still be able to call the C++
    bool electronID_cpp_(const std::string &id, const reco::Vertex *vtx = 0, double rho=UnSet(double)) const { return electronID(id,vtx,rho); }

    static void SetEffectiveAreaForRhoCorrections(int year=2012, bool data=1);

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
