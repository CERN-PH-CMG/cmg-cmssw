#ifndef _AnalysisDataFormats_CMGTools_Photon_H_
#define _AnalysisDataFormats_CMGTools_Photon_H_

/* #include "DataFormats/Math/interface/LorentzVector.h" */
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"

/* #include "AnalysisDataFormats/CMGTools/interface/TriBool.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/UnSet.h" */

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

/* #include <vector> */

namespace cmg {

  // Forward declaration needed.
  class Photon;

  class Photon : public cmg::Lepton<pat::PhotonPtr> {
  public:

    Photon() {}
    Photon(const value& m):
      cmg::Lepton<value>::Lepton(m)
      {}
    virtual ~Photon()
      {}

    friend class PhotonFactory;

    virtual bool isPhoton() const {
      return true;
    }

    /// charged hadron isolation    
    double chargedHadronVeto() const{
      return chargedHadronVeto_;
    }
    
    /// neutral hadron isolation
    double neutralHadronVeto() const{
      return neutralHadronVeto_;
    }
    
    /// photon isolation
    double photonVeto() const{
      return photonVeto_;
    }
    
    /// Corrected relative isolation (adding the vetoes to the photon itself). 
    double relIsoCor(float dBetaFactor=0, int allCharged=0) const{
      double corPt = this->pt() + this->chargedHadronVeto() + this->neutralHadronVeto() + this->photonVeto();
      double abs = absIso(dBetaFactor, allCharged)/corPt;
      return abs >=0 ? abs : -1;
    }
    
  private:

    double chargedHadronVeto_;
    double neutralHadronVeto_;
    double photonVeto_;

  };
}

#endif /*JET_H_*/
