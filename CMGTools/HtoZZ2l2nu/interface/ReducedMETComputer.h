#ifndef ReducedMETComputer_H
#define ReducedMETComputer_H

/** \class ReducedMETComputer
 *  No description available.
 *
 *  $Date: 2011/04/06 08:36:23 $
 *  $Revision: 1.1 $
 *  \author G. Cerminara & D. Trocino
 */

#include <utility>
#include <vector>


//class LorentzVector;

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;


class ReducedMETComputer {
public:
  /// Constructor
  ReducedMETComputer(double kRecoil_long = 2.0,
		     double kRecoil_perp = 2.0,
		     double kSigmaPt_long = 2.5,
		     double kSigmaPt_perp = 2.5,
		     double kPerp = 1);

  /// Destructor
  virtual ~ReducedMETComputer();

  void compute(const LorentzVector& lepton1, double sigmaPt1,
	       const LorentzVector& lepton2, double sigmaPt2,
	       const std::vector<LorentzVector>& jets,
	       const LorentzVector& met);

  
  double reducedMET() const {
    return redMET;
  }

  std::pair<double, double> reducedMETComponents() const {
    return std::make_pair(reducedMET_long, reducedMET_perp);
  }
    
  std::pair<double, double> recoilProjComponents() const {
    return std::make_pair(recoilProj_long, recoilProj_perp);
  }

  std::pair<double, double> metProjComponents() const {
    return std::make_pair(metProj_long, metProj_perp);
  }

  std::pair<double, double> sumJetProjComponents() const {
    return std::make_pair(sumJetProj_long, sumJetProj_perp);
  }

  std::pair<double, double> dileptonProjComponents() const {
    return std::make_pair(dileptonProj_long, dileptonProj_perp);
  }
  
  std::pair<int, int> recoilType() const {
    int ret_long = 0; // sumJetProj_long < -1.*metProj_long
    int ret_perp = 0; // sumJetProj_perp < -1.*metProj_perp
    if(sumJetProj_long > -1.*metProj_long) ret_long = 1;
    if(sumJetProj_perp > -1.*metProj_perp) ret_perp = 1;
    return std::make_pair(ret_long, ret_perp);
  }

protected:

private:
  
  double kRecoil_long;
  double kRecoil_perp;
  double kSigmaPt_long;
  double kSigmaPt_perp;
  double kPerp;

  double dileptonProj_long;
  double dileptonProj_perp;
  double sumJetProj_long;
  double sumJetProj_perp;
  double metProj_long;
  double metProj_perp;
  double recoilProj_long;
  double recoilProj_perp;
  double reducedMET_long;
  double reducedMET_perp;
  double redMET;
  

};
#endif

