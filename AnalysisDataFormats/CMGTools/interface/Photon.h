#ifndef _AnalysisDataFormats_CMGTools_Photon_H_
#define _AnalysisDataFormats_CMGTools_Photon_H_

/* #include "DataFormats/Math/interface/LorentzVector.h" */
#include "DataFormats/PatCandidates/interface/Photon.h"

/* #include "AnalysisDataFormats/CMGTools/interface/TriBool.h" */
/* #include "AnalysisDataFormats/CMGTools/interface/UnSet.h" */

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

/* #include <vector> */

namespace cmg {

  // Forward declaration needed.
  class Photon;

  class Photon : public PhysicsObjectWithPtr< pat::PhotonPtr > {
  public:

    Photon() {}
    Photon(const value& m):
      PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(m)
      {}
    virtual ~Photon()
      {}

    friend class PhotonFactory;

    virtual bool isPhoton() const {
      return true;
    }

  private:

  };
}

#endif /*JET_H_*/
