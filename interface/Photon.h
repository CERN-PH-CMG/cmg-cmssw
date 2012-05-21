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
      cmg::Lepton<value>::Lepton(m),
      isFromMuon_(false)
      {}
    virtual ~Photon()
      {}

    friend class PhotonFactory;

    virtual bool isPhoton() const {
      return true;
    }

    virtual bool isFromMuon() const {
      return isFromMuon_;
    }

  private:

    bool isFromMuon_;

  };
}

#endif /*JET_H_*/
