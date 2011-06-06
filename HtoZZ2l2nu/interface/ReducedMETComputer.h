#ifndef ReducedMETComputer_H
#define ReducedMETComputer_H

/** \class ReducedMETComputer
 *  No description available.
 *
 *  $Date: 2011/04/18 17:41:26 $
 *  $Revision: 1.3 $
 *  \author G. Cerminara & D. Trocino
 */

#include <utility>
#include <vector>


//class LorentzVector;

#include "Math/LorentzVector.h"
#include "TVector2.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ReducedMETComputer {
public:
  /// Constructor
  ReducedMETComputer(double kRecoil_long = 1.0,
		     double kRecoil_perp = 1.0,
		     double kSigmaPt_long = 1.0,
		     double kSigmaPt_perp = 1.0,
		     double kPerp = 1.0);

  /// Destructor
  virtual ~ReducedMETComputer();

  //define the axis
  std::pair<TVector2,TVector2> defineThrust(const LorentzVector& lepton1, const LorentzVector& lepton2);

  //getters for the axis
  TVector2 a_l,a_t; 
  inline TVector2 getAl() { return a_l; }
  inline TVector2 getAt() { return a_t; }

  //get a projected vector
  TVector2 project(const LorentzVector &part);

  //
  void compute(const LorentzVector& lepton1, double sigmaPt1,
	       const LorentzVector& lepton2, double sigmaPt2,
	       const LorentzVectorCollection & jets,
	       const LorentzVector& met,
	       bool debug=false);

  TVector2 reducedMETcartesian() const { 
    return redMETxy; 
  }
  
  double reducedMET() const {
    return redMET;
  }

  std::pair<double, double> reducedMETComponents() const {
    return std::make_pair(reducedMET_long, reducedMET_perp);
  }

  std::pair<double, double> reducedMETUnboundComponents() const {
    return std::make_pair(reducedMETUnbound_long, reducedMETUnbound_perp);
  }
    
  std::pair<double, double> recoilProjComponents() const {
    return std::make_pair(recoilProj_long, recoilProj_perp);
  }

  std::pair<double, double> recoilProjUnboundComponents() const {
    return std::make_pair(recoilProjUnbound_long, recoilProjUnbound_perp);
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

  std::pair<double, double> dileptonPtCorrComponents() const {
    return std::make_pair(deltaLeptonProjCorr_long, deltaLeptonProjCorr_perp);
  }
      
  enum RecoilType{JETLIKE=0,METLIKE};
  std::pair<int, int> recoilType() const {
    int ret_long = JETLIKE; 
    int ret_perp = JETLIKE; 
    if(sumJetProj_long > -1.*metProj_long) ret_long = METLIKE;
    if(sumJetProj_perp > -1.*metProj_perp) ret_perp = METLIKE;
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
  double recoilProj_long, recoilProjUnbound_long;
  double recoilProj_perp, recoilProjUnbound_perp;
  double reducedMET_long, reducedMETUnbound_long;
  double reducedMET_perp, reducedMETUnbound_perp;
  double deltaLeptonProjCorr_long;
  double deltaLeptonProjCorr_perp;
  double redMET;
  TVector2 redMETxy;

};
#endif

