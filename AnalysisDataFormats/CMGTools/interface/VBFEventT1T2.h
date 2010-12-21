#ifndef __AnalysisDataFormats_VBFEventT1T2_h__
#define __AnalysisDataFormats_VBFEventT1T2_h__
/** \class VBFEventT1T2
 *
 * \authors Colin Bernet,
 *          Artur Kalinowski
 *
 *
 */

#include <vector>

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/Candidate/interface/LeafCandidate.h" 
#include "DataFormats/Common/interface/Ptr.h"

#include "Math/VectorUtil.h"

namespace cmg {

template<typename T1, typename T2> class VBFEventT1T2 : public reco::LeafCandidate{

 public:

  typedef edm::Ptr<T1> T1Ptr;
  typedef edm::Ptr<T2> T2Ptr;
  typedef edm::PtrVector<T2> T2PtrVector;
    
  /// default constructor
  VBFEventT1T2() : etaMin_(10), etaMax_(-10), margin_(-1) {}

  /// constructor with MEt
 VBFEventT1T2(const T1Ptr leg1, const T1Ptr leg2)
   : leg1_(leg1), leg2_(leg2) { init();}
  
  /// default constructor
  ~VBFEventT1T2(){}
  
  /// access to tagging objects
  const T1Ptr leg1() const { return leg1_; }
  const T1Ptr leg2() const { return leg2_; }

  /// access to objects in the eta gap
  const T2PtrVector & vetoObjects() const {return vetoObjects_;}

  /// access to the veto object
  const T2Ptr vetoObject(uint i=0) const {
    if(vetoObjects_.size()<i+1) return T2Ptr();
    else return vetoObjects_[i];}

 /// access to the veto object, which is separated from particular 4-vector
  const T2Ptr vetoObject(math::XYZTLorentzVector p4, float deltaR) const {
    for(uint i=0;i<vetoObjects_.size();++i){
      if(ROOT::Math::VectorUtil::DeltaR(p4,vetoObject(i)->p4())>deltaR)
	return vetoObject(i);
    }
    return T2Ptr();
  }

  /// return the Zepperfeld variable: eta_j3 - (etaMin + etaMax )/2
  float etaZepperfeld(uint i = 0) const {
    if(!vetoObject(i)) return -99;
    else return vetoObject(i)->p4().Eta() - (etaMax_ + etaMin_)/2.0;
  }

  /// get invariant mass of the tagging objects system
  double mjj() const { return (leg1()->p4() + leg2()->p4()).M(); }

  /// get delta eta between tagging objects
  float dEta() const { return etaMax_ - etaMin_; }

  /// get delta phi between tagging objects
  float dPhi() const { return ROOT::Math::VectorUtil::DeltaPhi(leg1()->p4(),leg2()->p4()); }

  /// get max eta of the tagging objects
  float etaMax() const { return etaMax_; }

  /// get min eta of the tagging objects
  float etaMin() const { return etaMin_; }

  /// get eta margin for the eta gap definition
  float etaMargin() const { return margin_; }

  ///set eta gap margin
  void setEtaMargin(float margin){margin_ = margin;}

  /// add object to the central/veto objects collection
  //void addVetoObject(T2Ptr vetoObj){ vetoObjects_.push_back(vetoObj);}

  /// set the veto collection
  void setVetoCollection(T2PtrVector & tmpObjCollection){vetoObjects_ = tmpObjCollection; }

  //void clear(); 

 private:

  /// initialise datamembers
  void init(){
    
    float eta1 = leg1()->p4().eta();
    float eta2 = leg2()->p4().eta();

    if(eta1>eta2){
      etaMax_ = eta1;
      etaMin_ = eta2;
    }
    else{
      etaMax_ = eta2;
      etaMin_ = eta1;
    }   
  }

  /// references/pointers to tagging objects
  T1Ptr leg1_;
  T1Ptr leg2_;

  /// references/pointers vector to central(veto) objects
  T2PtrVector vetoObjects_;
  
  float etaMin_;
  float etaMax_;

  float margin_; 

};

}

#endif
