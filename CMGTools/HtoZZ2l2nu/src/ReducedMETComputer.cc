/*
 *  See header file for a description of this class.
 *
 *  $Date: 2011/04/18 17:41:26 $
 *  $Revision: 1.3 $
 *  \author G. Cerminara & D. Trocino
 */

#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "TVector2.h"

using namespace std;


ReducedMETComputer::ReducedMETComputer(double kRecoil_long,
				       double kRecoil_perp,
				       double kSigmaPt_long,
				       double kSigmaPt_perp,
				       double kPerp) :   kRecoil_long(kRecoil_long),
							 kRecoil_perp(kRecoil_perp),
							 kSigmaPt_long(kSigmaPt_long),
							 kSigmaPt_perp(kSigmaPt_perp),
							 kPerp(kPerp) {}


ReducedMETComputer::~ReducedMETComputer(){}





void ReducedMETComputer::compute(const LorentzVector& theLepton1, double sigmaPt1,
				 const LorentzVector& theLepton2, double sigmaPt2,
				 const std::vector<LorentzVector>& theJets,
				 const LorentzVector& theMET) {
  
  // reset
  dileptonProj_long = 0.;
  dileptonProj_perp = 0.;
  sumJetProj_long = 0.;
  sumJetProj_perp = 0.;
  metProj_long = 0.;
  metProj_perp = 0.;
  recoilProj_long = 0.;
  recoilProj_perp = 0.;
  reducedMET_long = 0.;
  reducedMET_perp = 0.;
  deltaLeptonProjCorr_long = 0.;
  deltaLeptonProjCorr_perp = 0.;
  redMET = 0.;
  




  TVector2 lepton1(theLepton1.Px(), theLepton1.Py());
  TVector2 lepton2(theLepton2.Px(), theLepton2.Py());;
  
  TVector2 bisector = (lepton1.Unit()+lepton2.Unit()).Unit();
  TVector2 bisector_perp(bisector.Py(), -bisector.Px());
  if(lepton1 * bisector_perp < 0) {
    bisector_perp *= -1;
  }
  
  TVector2 dilepton = lepton1 + lepton2;
  
  dileptonProj_long = dilepton*bisector;
  dileptonProj_perp = dilepton*bisector_perp;

  sumJetProj_long = 0.;
  sumJetProj_perp = 0.;

  bool doAllJets = true; // FIXME: configurable
  bool doUseMET = false; //FIXME: configurable

  for(vector<LorentzVector>::const_iterator jetit = theJets.begin();
      jetit != theJets.end();
      ++jetit) {
    TVector2 jet((*jetit).Px(), (*jetit).Py());
    if(jet*bisector < 0 || doAllJets) {
      sumJetProj_long += jet*bisector;
    }
    if(jet*bisector_perp < 0 || doAllJets) {
      sumJetProj_perp += jet*bisector_perp;
    }
  }


  TVector2 met(theMET.Px(), theMET.Py());
  TVector2 caloOnlyMET = met + dilepton; // FIXME: what about electrons?

  metProj_long = caloOnlyMET*bisector;
  metProj_perp = caloOnlyMET*bisector_perp;

  if(doUseMET) {
    recoilProj_long = min(sumJetProj_long, -1.*metProj_long);
    recoilProj_perp = min(sumJetProj_perp, -1.*metProj_perp);
  } else {
    recoilProj_long = sumJetProj_long;
    recoilProj_perp = sumJetProj_perp;
  }
  
  // -- corrections for lepton Pt uncertainties

  // relative Pt uncert. (maximum 100%)
  double relErrPt1 = min(sigmaPt1/lepton1.Mod(), 1.);
  double relErrPt2 = min(sigmaPt2/lepton2.Mod(), 1.);
  
  // rescale for relative error
  TVector2 leptonCorr1 = lepton1*(1.-relErrPt1);
  TVector2 leptonCorr2 = lepton2*(1.-relErrPt2);
  TVector2 dileptonCorr = leptonCorr1 + leptonCorr2;
  
  // compute the delta of the projections (must be a negative number -> conservative)
  double dileptonProjCorr_long = dileptonCorr*bisector;
  deltaLeptonProjCorr_long = dileptonProjCorr_long - dileptonProj_long;
  // we fluctuate independently the 2 leptons and sum the contribution to get the biggest possible negative fluctuation on the perp component
  deltaLeptonProjCorr_perp = (relErrPt2*lepton2 - relErrPt1*lepton1)*bisector_perp;
  
  // -- compute the 2 components of the reduced MET variable
  reducedMET_long = max(dileptonProj_long + kRecoil_long*recoilProj_long + kSigmaPt_long*deltaLeptonProjCorr_long, 0.);
  reducedMET_perp = max(dileptonProj_perp + kRecoil_perp*recoilProj_perp + kSigmaPt_perp*deltaLeptonProjCorr_perp, 0.);
  
  
  redMET = sqrt(reducedMET_long*reducedMET_long + kPerp*reducedMET_perp*reducedMET_perp);

}
  


