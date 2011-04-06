#ifndef ReducedMETComputer_H
#define ReducedMETComputer_H

/** \class ReducedMETComputer
 *  No description available.
 *
 *  $Date: 2011/04/06 08:24:14 $
 *  $Revision: 1.3 $
 *  \author G. Cerminara - CERN
 */

#include <utility>
#include <vector>


class TLorentzVector;

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

  void compute(const TLorentzVector& lepton1, double sigmaPt1,
	       const TLorentzVector& lepton2, double sigmaPt2,
	       const std::vector<TLorentzVector>& jets,
	       const TLorentzVector& met);

  
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

