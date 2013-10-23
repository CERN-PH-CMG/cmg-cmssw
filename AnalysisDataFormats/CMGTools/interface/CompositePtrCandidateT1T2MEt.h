#ifndef __AnalysisDataFormats_PFAnalyses_CompositeRefCandidateT1T2MEt_h__
#define __AnalysisDataFormats_PFAnalyses_CompositeRefCandidateT1T2MEt_h__

/** \class CompositeRefCandidateT1T2MEt
 *
 * Combination of leptonic and hadronic decay products 
 * of a pair of tau leptons plus missing transverse momentum 
 * (representing the undetected momentum carried away by the neutrinos 
 *  produced in the two tau decays) 
 * 
 * \authors Colin Bernet,
 *          Michal Bluj,
 *          Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: CompositePtrCandidateT1T2MEt.h,v 1.1 2011/02/18 14:14:50 wreece Exp $
 *
 */

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h" 
#include "DataFormats/Common/interface/Ptr.h"

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEt : public reco::LeafCandidate 
{
  typedef edm::Ptr<T1> T1Ptr;
  typedef edm::Ptr<T2> T2Ptr;

 public:

  /// default constructor
  CompositePtrCandidateT1T2MEt() {}

  /// constructor with MEt
  CompositePtrCandidateT1T2MEt(const T1Ptr leg1, const T2Ptr leg2, const reco::CandidatePtr met)
    : leg1_(leg1), leg2_(leg2), met_(met) {}

  /// constructor without MEt
  CompositePtrCandidateT1T2MEt(const T1Ptr leg1, const T2Ptr leg2)
    : leg1_(leg1), leg2_(leg2) {}

  /// destructor
  ~CompositePtrCandidateT1T2MEt() {}

  /// access to daughter particles
  const T1Ptr leg1() const { return leg1_; }
  const T2Ptr leg2() const { return leg2_; }
  
  /// access to gen. momenta
  reco::Candidate::LorentzVector p4gen() const { return p4Leg1gen() + p4Leg2gen(); }
  reco::Candidate::LorentzVector p4VisGen() const { return p4VisLeg1gen() + p4VisLeg2gen(); }

  /// access to gen. mother particles
  /// (undecayed tau leptons)
  const reco::Candidate::LorentzVector& p4Leg1gen() const { return p4Leg1gen_; }
  const reco::Candidate::LorentzVector& p4Leg2gen() const { return p4Leg2gen_; }

  /// access to visible gen. daughter particles
  /// (electrons, muons, kaons, charged and neutral pions produced in tau decay)
  const reco::Candidate::LorentzVector& p4VisLeg1gen() const { return p4VisLeg1gen_; }
  const reco::Candidate::LorentzVector& p4VisLeg2gen() const { return p4VisLeg2gen_; }

  /// energy ratio of visible gen. daughter/mother particles
  double x1gen() const { return ( p4Leg1gen_.energy() > 0. ) ? p4VisLeg1gen_.energy()/p4Leg1gen_.energy() : -1.; }
  double x2gen() const { return ( p4Leg2gen_.energy() > 0. ) ? p4VisLeg2gen_.energy()/p4Leg2gen_.energy() : -1.; }
  
  /// return the number of source particle-like Candidates
  /// (the candidates used to construct this Candidate)       
  /// MET does not count. 
  size_t numberOfSourceCandidatePtrs() const { return 2; }

  /// return a Ptr to one of the source Candidates                                                               
  /// (the candidates used to construct this Candidate)                                                         
  reco::CandidatePtr sourceCandidatePtr( size_type i ) const {
    if(i==0) return leg1();
    else if(i==1) return leg2();
    else assert(0);
  }

  /// access to missing transverse momentum
  const reco::CandidatePtr met() const { return met_; }

  // get sum of charge of visible decay products
  // (not need to declare it in CompositePtrCandidateT1T2MEt; 
  //  already declared in Candidate base-class)

  /// get four-momentum of visible decay products
  const reco::Candidate::LorentzVector& p4Vis() const { return p4Vis_; }

  /// get four-momentum and scaling factors for momenta of visible decay products
  /// computed by collinear approximation
  const reco::Candidate::LorentzVector& p4CollinearApprox() const { return p4CollinearApprox_; }
  double x1CollinearApprox() const { return x1CollinearApprox_; }
  double x2CollinearApprox() const { return x2CollinearApprox_; }
  bool collinearApproxIsValid() const { return collinearApproxIsValid_; }

  /// get the four-momentum and x1-x2 (xi = visible energy fraction of the ith-tau)
  /// computed by improved coll. approx.
  const reco::Candidate::LorentzVector& p4ImprovedCollinearApprox() const { return p4ImprovedCollinearApprox_; }
  double x1ImprovedCollinearApprox() const { return x1ImprovedCollinearApprox_; }
  double x2ImprovedCollinearApprox() const { return x2ImprovedCollinearApprox_; }
  /// get the scale-factor to re-weight the mass of the diTaus rescued in the 
  /// improved coll.approx. This scale factor is a function of the visible diTau-pT
  /// The functional dependence can be configured by the user in the _cfg.
  double scaleFactor() const { return 1./scaleFactor_;}
  /// is the improved coll. approx. valid?
  bool ImprovedCollinearApproxIsValid() const { return ImprovedCollinearApproxIsValid_; }
  /// flag to discriminate which collinear approximation is valid:
  //     0 : coll. approx. and improved coll. approx. are false
  //     1 : coll. approx. is true 
  //     2 : coll. approx. is false but improved coll. approx. (not-back-to-back legs) is true 
  //     3 : coll. approx. is false but improved coll. approx. (back-to-back legs) is true 
  int validityCollinearApproxFlag() const { return validityCollinearApproxFlag_; }

  /// get "pseudo" four-momentum computed by CDF method
  /// (for a description of the method, see e.g. CDF note 8972)
  const reco::Candidate::LorentzVector& p4CDFmethod() const { return p4CDFmethod_; }

  /// get transverse mass of visible decay products + missing transverse momentum
  double mt12MET() const { return mt12MET_; }

  /// get transverse mass of first/second
  /// visible decay product + missing transverse momentum
  double mt1MET() const { return mt1MET_; }
  double mt2MET() const { return mt2MET_; }

  /// get acoplanarity angle (angle in transverse plane) between visible decay products
  double dPhi12() const { return dPhi12_; } 

  /// get separation in eta-phi between visible decay products
  double dR12() const { return dR12_; }

  /// get minimal/maximal pseudo-rapidity of visible decay products
  double visEtaMin() const { return visEtaMin_; }
  double visEtaMax() const { return visEtaMax_; }
  
  /// get acoplanarity angle (angle in transverse plane) between first/second
  /// visible decay product and missing transverse momentum
  double dPhi1MET() const { return dPhi1MET_; }
  double dPhi2MET() const { return dPhi2MET_; }

  /// get values of CDF-"zeta" variables
  /// (indicating the consistency of the missing transverse momentum observed in an event
  ///  with the hypothesis of originating from tau neutrinos)
  double pZeta() const { return pZeta_; }
  double pZetaVis() const { return pZetaVis_; }

  /// clone  object
  CompositePtrCandidateT1T2MEt<T1,T2>* clone() const { return new CompositePtrCandidateT1T2MEt<T1,T2>(*this); }

  friend std::ostream& operator<<(std::ostream& out, const CompositePtrCandidateT1T2MEt<T1,T2>& dic) {
    out << "Di-Candidate m = " << dic.mass();
    return out;
  }

  ///***** SV VERTEX METHOD ****
  double svMethodNLL() const { return svNLL_; };
  /// Access to daughter vertices
  const reco::Candidate::Point& vertexLeg1() const { return vertexLeg1_; };
  const reco::Candidate::Point& vertexLeg2() const { return vertexLeg2_; };

  /// Access to generator daughter vertices
  const reco::Candidate::Point& vertexLeg1gen() const { return vertexLeg1gen_; };
  const reco::Candidate::Point& vertexLeg2gen() const { return vertexLeg2gen_; };

  /// Invisible four momenta computed by SV method
  const reco::Candidate::LorentzVector& p4Leg1InvisFromSV() const { return p4NuSVLeg1_; };
  const reco::Candidate::LorentzVector& p4Leg2InvisFromSV() const { return p4NuSVLeg2_; };

  /// Visible four momenta computed by SV method.  The visible momentum 
  /// is recomputed at the found secondary vertex
  const reco::Candidate::LorentzVector& p4Leg1VisFromSV() const { return p4VisSVLeg1_; };
  const reco::Candidate::LorentzVector& p4Leg2VisFromSV() const { return p4VisSVLeg2_; };

  /// Get the total four vector as computed by the SVFit
  const reco::Candidate::LorentzVector& p4SVFit() const { return p4SVmethod_; };

  /// Compute three vector sum of missing energies
  reco::Candidate::Vector mETFromSVTotal() const {
     return p4Leg1InvisFromSV().Vect() + p4Leg2InvisFromSV().Vect(); };

  /// Access to SV-fitted primary vertex
  const reco::Candidate::Point& primaryVertex() const { return pv_; };

  /// Access to true primary vertex
  const reco::Candidate::Point& primaryVertexGen() const { return pvGen_; };

  /// Compute total leg (vis + invis) direction
  reco::Candidate::Vector p3Leg1TotalFromVertices() const { return (vertexLeg1_ - pv_); };
  reco::Candidate::Vector p3Leg2TotalFromVertices() const { return (vertexLeg2_ - pv_); };

  /// Compute total generator leg (vis + invis) direction
  reco::Candidate::Vector p3Leg1TotalFromGenVertices() const { return (vertexLeg1gen_ - pvGen_); };
  reco::Candidate::Vector p3Leg2TotalFromGenVertices() const { return (vertexLeg2gen_ - pvGen_); };

  /// Get reconstructed flight distance of mother particle
  double leg1DecayLength() const { return sqrt(p3Leg1TotalFromVertices().Mag2()); };
  double leg2DecayLength() const { return sqrt(p3Leg2TotalFromVertices().Mag2()); };

  double probLeg1DecayLength() const { return exp(-1.788*leg1DecayLength()/(8.711e-3*leg1()->p4().P())); };
  double probLeg2DecayLength() const { return exp(-1.788*leg2DecayLength()/(8.711e-3*leg2()->p4().P())); };

  /// Get true flight distance of mother particle
  double leg1DecayLengthGen() const { return sqrt(p3Leg1TotalFromGenVertices().Mag2()); };
  double leg2DecayLengthGen() const { return sqrt(p3Leg2TotalFromGenVertices().Mag2()); };

 private:
  
  /// allow only CompositePtrCandidateT1T2MEtAlgorithm to change values of data-members
  template<typename T1_type, typename T2_type> friend class CompositePtrCandidateT1T2MEtAlgorithm; 

  /// set gen. four-momenta
  void setP4Leg1gen(const reco::Candidate::LorentzVector& p4) { p4Leg1gen_ = p4; }
  void setP4Leg2gen(const reco::Candidate::LorentzVector& p4) { p4Leg2gen_ = p4; }
  void setP4VisLeg1gen(const reco::Candidate::LorentzVector& p4) { p4VisLeg1gen_ = p4; }
  void setP4VisLeg2gen(const reco::Candidate::LorentzVector& p4) { p4VisLeg2gen_ = p4; }

  /// set four-momentum of visible decay products
  void setP4Vis(const reco::Candidate::LorentzVector& p4) { p4Vis_ = p4; } 
  /// set four-momentum and scaling factors for momenta of visible decay products
  /// computed by collinear approximation
  void setCollinearApproxQuantities(const reco::Candidate::LorentzVector& p4, double x1, double x2, bool isValid, int flag)
  {
    p4CollinearApprox_ = p4;
    x1CollinearApprox_ = x1;
    x2CollinearApprox_ = x2;
    collinearApproxIsValid_ = isValid;
    validityCollinearApproxFlag_ = flag;
  }
  /// set four-momentum and scaling factors for momenta of visible decay products
  /// computed by Improved collinear approximation
  void setImprovedCollinearApproxQuantities(const reco::Candidate::LorentzVector& p4, double x1, double x2, double scaleFactor, bool isValid, int flag)
  {
    p4ImprovedCollinearApprox_ = p4;
    x1ImprovedCollinearApprox_ = x1;
    x2ImprovedCollinearApprox_ = x2;
    scaleFactor_ = scaleFactor;
    ImprovedCollinearApproxIsValid_ = isValid;
    validityCollinearApproxFlag_ = flag;
  }
  /// set "pseudo" four-momentum computed by CDF method
  /// (for a description of the method, see e.g. CDF note 8972)
  void setP4CDFmethod(const reco::Candidate::LorentzVector& p4) { p4CDFmethod_ = p4; }
  /// set transverse mass of visible decay products + missing transverse momentum
  void setMt12MET(double mt) { mt12MET_ = mt; }
  /// set transverse mass of first/second
  /// visible decay product + missing transverse momentum
  void setMt1MET(double mt) { mt1MET_ = mt; }
  void setMt2MET(double mt) { mt2MET_ = mt; }
  /// set acoplanarity angle (angle in transverse plane) between visible decay products
  void setDPhi12(double dPhi) { dPhi12_ = dPhi; }
  /// set separation in eta-phi between visible decay products
  void setDR12(double dR) { dR12_ = dR; }
  /// set minimal/maximal pseudo-rapidity of visible decay products
  void setVisEtaMin(double eta) { visEtaMin_ = eta; }
  void setVisEtaMax(double eta) { visEtaMax_ = eta; }
  /// set acoplanarity angle (angle in transverse plane) between first/second
  /// visible decay product and missing transverse momentum
  void setDPhi1MET(double dPhi) { dPhi1MET_ = dPhi; }
  void setDPhi2MET(double dPhi) { dPhi2MET_ = dPhi; }
  /// set values of CDF-"zeta" variables
  void setPzeta(double pZeta) { pZeta_ = pZeta; }
  void setPzetaVis(double pZetaVis) { pZetaVis_ = pZetaVis; }

  /// references/pointers to decay products
  T1Ptr leg1_;
  T2Ptr leg2_;
  reco::CandidatePtr met_;

  /// gen. four-momenta
  reco::Candidate::LorentzVector p4Leg1gen_;
  reco::Candidate::LorentzVector p4Leg2gen_;
  reco::Candidate::LorentzVector p4VisLeg1gen_;
  reco::Candidate::LorentzVector p4VisLeg2gen_;

  /// four-momentum of visible decay products
  reco::Candidate::LorentzVector p4Vis_;
  /// four-momentum and scaling factors for momenta of visible decay products computed by collinear approximation
  reco::Candidate::LorentzVector p4CollinearApprox_;
  bool collinearApproxIsValid_;
  double x1CollinearApprox_;
  double x2CollinearApprox_;

  /// four-momentum of visible decay products
  reco::Candidate::LorentzVector p4ImprovedCollinearApprox_;
  /// four-momentum and x1-x2 fractions computed by improved collinear approximation
  bool ImprovedCollinearApproxIsValid_;
  double x1ImprovedCollinearApprox_;
  double x2ImprovedCollinearApprox_;
  /// scale factors for diTau mass computed in the improved coll. approx.
  double scaleFactor_;
  /// flag to discriminate which approximation is valid
  int validityCollinearApproxFlag_;
  

  /// "pseudo" four-momentum computed by CDF method
  reco::Candidate::LorentzVector p4CDFmethod_;
  /// transverse mass of visible decay products + missing transverse momentum
  double mt12MET_;
  /// transverse mass of first/second visible decay product + missing transverse momentum
  double mt1MET_;
  double mt2MET_;
  /// acoplanarity angle (angle in transverse plane) between visible decay products
  double dPhi12_;
  /// separation in eta-phi between visible decay products
  double dR12_;
  /// minimal/maximal pseudo-rapidity of visible decay products
  double visEtaMin_;
  double visEtaMax_;
  /// acoplanarity angle (angle in transverse plane) between first/second
  /// visible decay product and missing transverse momentum
  double dPhi1MET_;
  double dPhi2MET_;
  /// CDF-"zeta" variables
  double pZeta_;
  double pZetaVis_;


  //// *** SV METHOD SUPPORT
  void setVertexLeg1(const reco::Candidate::Point& vtx) { vertexLeg1_ = vtx; };
  void setVertexLeg2(const reco::Candidate::Point& vtx) { vertexLeg2_ = vtx; };

  void setVertexLeg1Gen(const reco::Candidate::Point& vtx) { vertexLeg1gen_ = vtx; };
  void setVertexLeg2Gen(const reco::Candidate::Point& vtx) { vertexLeg2gen_ = vtx; };

  void setPV(const reco::Candidate::Point& pv) { pv_ = pv; };
  void setPVGen(const reco::Candidate::Point& pvGen) { pvGen_ = pvGen; };

  void setNuSVLeg1(const reco::Candidate::LorentzVector& p4) { p4NuSVLeg1_ = p4; };
  void setNuSVLeg2(const reco::Candidate::LorentzVector& p4) { p4NuSVLeg2_ = p4; };

  // The SV method refits the momentum of the visible products at the vertex
  void setVisSVLeg1(const reco::Candidate::LorentzVector& p4) { p4VisSVLeg1_ = p4; };
  void setVisSVLeg2(const reco::Candidate::LorentzVector& p4) { p4VisSVLeg2_ = p4; };

  void setSVNLL(double nll) { svNLL_ = nll; };

  void computeSVTotal() { p4SVmethod_ = 
     p4VisSVLeg1_ + p4NuSVLeg1_ +
     p4VisSVLeg2_ + p4NuSVLeg2_; };

  double svNLL_;
  
  // Secondary reconstructed vertices
  reco::Candidate::Point vertexLeg1_;
  reco::Candidate::Point vertexLeg2_;
  reco::Candidate::Point pv_;

  // Generator vertices
  reco::Candidate::Point vertexLeg1gen_;
  reco::Candidate::Point vertexLeg2gen_;
  reco::Candidate::Point pvGen_;

  reco::Candidate::LorentzVector p4NuSVLeg1_;
  reco::Candidate::LorentzVector p4NuSVLeg2_;

  reco::Candidate::LorentzVector p4VisSVLeg1_;
  reco::Candidate::LorentzVector p4VisSVLeg2_;

  reco::Candidate::LorentzVector p4SVmethod_;

};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEt<reco::Candidate, reco::Candidate> DiCandidatePair;
typedef CompositePtrCandidateT1T2MEt<pat::Electron, pat::Tau> PATElecTauPair;
typedef CompositePtrCandidateT1T2MEt<pat::Muon, pat::Tau> PATMuTauPair;
typedef CompositePtrCandidateT1T2MEt<pat::Tau, pat::Tau> PATDiTauPair;
typedef CompositePtrCandidateT1T2MEt<pat::Electron, pat::Muon> PATElecMuPair;
typedef CompositePtrCandidateT1T2MEt<pat::Electron, pat::Electron> PATElecPair;
typedef CompositePtrCandidateT1T2MEt<pat::Muon, pat::Muon> PATMuPair;


#endif
