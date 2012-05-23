#ifndef _AnalysisDataFormats_CMGTools_Photon_H_
#define _AnalysisDataFormats_CMGTools_Photon_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

/* #include <vector> */

namespace cmg {

  class Photon : public cmg::AbstractPhysicsObject {

  public:

    Photon() {}
    // Constructor for PF Photons
    Photon(pat::PhotonPtr pfPhoton):
      cmg::AbstractPhysicsObject::AbstractPhysicsObject(*pfPhoton),
      isFromMuon_(false) 
	{}
    // Constructor for pat Muons.
    Photon(pat::MuonPtr patMuon):
      cmg::AbstractPhysicsObject::AbstractPhysicsObject(*patMuon),
      isFromMuon_(true) {}

    virtual ~Photon()
      {}

    friend class PhotonFactory;

    virtual bool isPhoton() const {
      return true;
    }

    virtual bool isFromMuon() const {
      return isFromMuon_;
    }

    int charge() const{
      return charge_;
    }
    
    /// charged hadron isolation    
    double chargedHadronIso() const{
      return chargedHadronIso_;
    }
    
    /// charged particle isolation (e, mu, h+-)
    double chargedAllIso() const{
      return chargedAllIso_;
    }
    
    /// pile-up charged hadron isolation, used for dbeta corrections
    double puChargedHadronIso() const{
      return puChargedHadronIso_;
    }
    
    /// neutral hadron isolation
    double neutralHadronIso() const{
      return neutralHadronIso_;
    }
    
    /// photon isolation
    double photonIso() const{
      return photonIso_;
    }
    
    /// absolute isolation. if dBetaFactor > 0, the delta beta correction is applied. 
    /// if allCharged is true, charged hadron isolation is replaced by charged 
    /// particle isolation.
    double absIso(float dBetaFactor=0, int allCharged=0) const{
      // in this case, dbeta correction is asked, but 
      // the input for this correction is not available. 
      // better returning an unphysical result than applying a wrong correction.
      if(dBetaFactor>0 && puChargedHadronIso()<0) return -1;
      
      double neutralIso = neutralHadronIso() + photonIso();
      double corNeutralIso = neutralIso - dBetaFactor * puChargedHadronIso();
      
      double charged = chargedHadronIso();
      if( allCharged ) charged = chargedAllIso();
      
      return charged + ( corNeutralIso>0 ? corNeutralIso : 0 ) ;   
    }
    
    /// relative isolation. if dBetaFactor > 0, the delta beta correction is applied. 
    /// if allCharged is true, charged hadron isolation is replaced by charged 
    /// particle isolation.
    double relIso(float dBetaFactor=0, int allCharged=0) const{
      double abs = absIso(dBetaFactor, allCharged)/this->pt();
      return abs >=0 ? abs : -1;
    }
    
  private:

    /// Photon is now inheriting from Lepton -> protected instead of private.
    //private:
    ///COLIN: why a charge_ datamember here? should already be in a base class
    int charge_;
    
    /// isolation from charged hadrons
    double chargedHadronIso_;
    
    /// isolation from charged particles
    double chargedAllIso_;
    
    /// isolation w/r to charged hadrons from pile-up vertices
    /// used in the delta beta correction
    double puChargedHadronIso_;
    double neutralHadronIso_;
    double photonIso_;
    
    bool isFromMuon_;

  };
}

#endif /*JET_H_*/
