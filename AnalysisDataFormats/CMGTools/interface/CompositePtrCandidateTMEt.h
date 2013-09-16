#ifndef __AnalysisDataFormats_PFAnalyses_CompositeRefCandidateTMEt_h__
#define __AnalysisDataFormats_PFAnalyses_CompositeRefCandidateTMEt_h__

/** \class CompositeRefCandidateTMEt
 *
 * Combination of visible tau decay products with missing transverse momentum 
 * (representing the undetected momentum carried away 
 *  the neutrino produced in a W --> tau nu decay and the neutrinos produced in the tau decay)
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: CompositePtrCandidateTMEt.h,v 1.1 2011/02/18 14:14:50 wreece Exp $
 *
 */

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/Candidate/interface/LeafCandidate.h" 
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/MET.h"
template<typename T>
class CompositePtrCandidateTMEt : public reco::LeafCandidate 
{
  typedef edm::Ptr<T> TPtr;
  typedef edm::Ptr<pat::MET> METPtr;

 public:

  /// default constructor
  CompositePtrCandidateTMEt() {}

  /// constructor with MEt
  CompositePtrCandidateTMEt(const TPtr visDecayProducts, const METPtr met)
    : visDecayProducts_(visDecayProducts), met_(met) {recoilDPhi_=0;diLeptonVetoMass_=0;diLeptonVetoPtAsymmetry_=0;}

  /// destructor
  ~CompositePtrCandidateTMEt() {}

  /// access to daughter particles
  const TPtr visDecayProducts() const { return visDecayProducts_; }
 
  /// access to missing transverse momentum
  const METPtr met() const { return met_; }

  /// return the number of source particle-like Candidates
  /// (the candidates used to construct this Candidate)       
  /// MET does not count. 
  size_t numberOfSourceCandidatePtrs() const { return 1; }

  /// return a Ptr to one of the source Candidates                                                               
  /// (the candidates used to construct this Candidate)                                                         
  reco::CandidatePtr sourceCandidatePtr( size_type i ) const {
    if(i==0) return visDecayProducts();
    else assert(0);
  }

  /// get transverse mass of visible decay products + missing transverse momentum
  double mt() const { return mt_; }

  /// get acoplanarity angle (angle in transverse plane) between visible decay products 
  /// and missing transverse momentum 
  double dPhi() const { return dPhi_; } 

  const reco::Candidate::LorentzVector recoil() const {return recoil_;}
  double recoilDPhi() const {return recoilDPhi_;}

  double diLeptonVetoMass() const {return diLeptonVetoMass_;}
  double diLeptonVetoPtAsymmetry() const {return diLeptonVetoPtAsymmetry_;}

  /// clone  object
  CompositePtrCandidateTMEt<T>* clone() const { return new CompositePtrCandidateTMEt<T>(*this); }

 private:
  
  /// allow only CompositePtrCandidateTMEtAlgorithm to change values of data-members
  template<typename T_type> friend class CompositePtrCandidateTMEtAlgorithm; 

  /// set transverse mass of visible decay products + missing transverse momentum
  void setMt(double mt) { mt_ = mt; }
  /// set acoplanarity angle (angle in transverse plane) between visible decay products 
  /// and missing transverse momentum
  void setDPhi(double dPhi) { dPhi_ = dPhi; }
  
  void setRecoil(const reco::Candidate::LorentzVector& recoil) {recoil_ = recoil; }
  void setRecoilDPhi(double dPhi) {recoilDPhi_ = dPhi;}

  void setDiLeptonVetoValues(double mass,double ptAsymm) {diLeptonVetoMass_ = mass; diLeptonVetoPtAsymmetry_ = ptAsymm;   }


  /// references/pointers to decay products and missing transverse momentum
  reco::Candidate::LorentzVector recoil_;
  double recoilDPhi_;

  TPtr visDecayProducts_;
  METPtr met_;

  /// transverse mass of visible decay products + missing transverse momentum
  double mt_;
  /// acoplanarity angle (angle in transverse plane) between visible decay products
  /// and missing transverse momentum
  double dPhi_;


  double diLeptonVetoMass_;
  double diLeptonVetoPtAsymmetry_;


};

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

typedef CompositePtrCandidateTMEt<pat::Tau> PATTauNuPair;
typedef CompositePtrCandidateTMEt<pat::Muon> PATMuonNuPair;
typedef CompositePtrCandidateTMEt<pat::Electron> PATElectronNuPair;

#endif
