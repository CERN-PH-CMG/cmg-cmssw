#ifndef Utils_H
#define Utils_H

/** \class Utils
 *  No description available.
 *
 *  $Date: $
 *  $Revision: $
 *  \author G. Cerminara - CERN
 */

#include "TLorentzVector.h"

namespace pat {
  class Muon;
}

class LorenzVector;

class Utils {
public:
  /// Constructor
  Utils();

  /// Destructor
  virtual ~Utils();

  // Operations
  static double computeRelIsolation(const pat::Muon *muon);

  static int muonType(const pat::Muon *muon);

//   static TLorentzVector convert(const LorenzVector& original);

protected:

private:

};
#endif

