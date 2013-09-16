#ifndef ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include "DataFormats/Math/interface/Point3D.h"

#include "DataFormats/RecoCandidate/interface/IsoDepositDirection.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include <vector>

namespace cmg
{

//forward def needed
template <class LeptonType> class Lepton;
template <class LeptonType> class LeptonSettingTool;

template <class LeptonType>
class Lepton : public cmg::PhysicsObjectWithPtr< LeptonType >{
 public:
  
  Lepton(){}

  Lepton(const LeptonType& o):
    cmg::PhysicsObjectWithPtr<LeptonType>::PhysicsObjectWithPtr(o),
    charge_(UnSet(int)),
    chargedHadronIso_(UnSet(double)),
    chargedAllIso_(UnSet(double)),
    puChargedHadronIso_(UnSet(double)),
    neutralHadronIso_(UnSet(double)),
    photonIso_(UnSet(double)),
    dxy_(UnSet(float)),
    dz_(UnSet(float)){}
    
  virtual ~Lepton(){}
  
  int charge() const{
    return charge_;
  }
                                
  /// charged hadron isolation    
  double chargedHadronIso(double dr=-1.0) const; 
  /// charged particle isolation (e, mu, h+-)
  double chargedAllIso(double dr=-1.0) const;
  /// pile-up charged hadron isolation, used for dbeta corrections
  double puChargedHadronIso(double dr=-1.0) const;
  /// neutral hadron isolation
  double neutralHadronIso(double dr=-1.0) const;
  /// photon isolation
  double photonIso(double dr=-1.0) const;

  /// charged hadron vetoes    
  virtual reco::isodeposit::AbsVetos chargedHadronVetos() const { 
    reco::isodeposit::AbsVetos vetos;
    return vetos;
  }
  /// charged particle vetoes (e, mu, h+-)
  virtual reco::isodeposit::AbsVetos chargedAllVetos() const {
    reco::isodeposit::AbsVetos vetos;
    return vetos;
  }
  /// pile-up charged hadron vetoes, used for dbeta corrections
  virtual reco::isodeposit::AbsVetos puChargedHadronVetos() const { 
    reco::isodeposit::AbsVetos vetos;
    return vetos;
  }
  /// neutral hadron vetoes
  virtual reco::isodeposit::AbsVetos neutralHadronVetos() const {
    reco::isodeposit::AbsVetos vetos;
    return vetos;
  }
  /// photon vetoes
  virtual reco::isodeposit::AbsVetos photonVetos() const {
    reco::isodeposit::AbsVetos vetos;
    return vetos;
  }

  /// absolute isolation. if dBetaFactor > 0, the delta beta correction is applied. 
  /// if allCharged is true, charged hadron isolation is replaced by charged 
  /// particle isolation.
  double absIso(float dBetaFactor=0, int allCharged=0, double dr=-1.0) const{
    // in this case, dbeta correction is asked, but 
    // the input for this correction is not available. 
    // better returning an unphysical result than applying a wrong correction.
    if(dBetaFactor>0 && puChargedHadronIso(dr)<0) return -1;

    double neutralIso = neutralHadronIso(dr) + photonIso(dr);
    double corNeutralIso = neutralIso - dBetaFactor * puChargedHadronIso(dr);

    double charged = chargedHadronIso(dr);
    if( allCharged ) charged = chargedAllIso(dr);

    return charged + ( corNeutralIso>0 ? corNeutralIso : 0 ) ;
  }

  /// relative isolation. if dBetaFactor > 0, the delta beta correction is applied. 
  /// if allCharged is true, charged hadron isolation is replaced by charged 
  /// particle isolation.
  double relIso(float dBetaFactor=0, int allCharged=0, double dr=-1.0) const{
    double abs = absIso(dBetaFactor, allCharged, dr)/this->pt();
    return abs >=0 ? abs : -1;
  }

  /// transverse impact parameter
  float dxy() const{
    return dxy_;
  }
  
  /// transverse impact parameter with respect to a given vertex
  float dxy(const math::XYZPoint& vertex) const{
    return (- (this->vx()-vertex.x()) * this->py() + (this->vy()-vertex.y()) * this->px() ) / this->pt();
  }

  /// longitudinal impact parameter
  float dz() const{
    return dz_;
  }

  /// longitudinal impact parameter
  float dz(const math::XYZPoint& vertex) const{
    return (this->vz()-vertex.z()) - ((this->vx()-vertex.x())*this->px()+(this->vy()-vertex.y())*this->py())/this->pt() * this->pz()/this->pt();
  }
  
  /// 3d impact parameter and error
  float dB3D() const{
    return dB3D_;
  }
  float edB3D() const{
    return edB3D_;
  }

  
  friend class cmg::LeptonSettingTool<LeptonType>;
	
 protected:
  /// 3d impact parameter 
  float dB3D_;

  /// error on 3d impact parameter
  float edB3D_;

  /// Photon is now inheriting from Lepton -> protected instead of private.
  //private:
  ///COLIN: why a charge_ datamember here? should already be in a base class
  int charge_;

  /// isolation from charged hadrons
  double chargedHadronIso_;

  /// isolation from charged particles
  double chargedAllIso_;
  
  /// isolation w/r to charged hadrons from pile-up vertices
  /// used in the delta beta correction
  double puChargedHadronIso_;
  double neutralHadronIso_;
  double photonIso_;

  /// transverse impact parameter
  float dxy_;

  /// longitudinal impact parameter
  float dz_;


};

/// charged hadron isolation
template <class LeptonType>
double Lepton<LeptonType>::chargedHadronIso(double dr) const {
  if( dr > 0 ){
    return (*(this->sourcePtr()))->isoDeposit(pat::PfChargedHadronIso)->depositAndCountWithin( dr, chargedHadronVetos() ).first;
  }else{
    return chargedHadronIso_;
  }
}

/// charged particle isolation (e, mu, h+-)
template <class LeptonType>
double Lepton<LeptonType>::chargedAllIso(double dr) const {
  if( dr > 0 ){
    return (*(this->sourcePtr()))->isoDeposit(pat::PfChargedAllIso)->depositAndCountWithin( dr, chargedAllVetos() ).first;
  }else{
    return chargedAllIso_;
  }
}

/// pile-up charged hadron isolation, used for dbeta corrections
template <class LeptonType>
double Lepton<LeptonType>::puChargedHadronIso(double dr) const {
  if( dr > 0 ){
    return (*(this->sourcePtr()))->isoDeposit(pat::PfPUChargedHadronIso)->depositAndCountWithin( dr, puChargedHadronVetos() ).first;
  }else{
    return puChargedHadronIso_;
  }
}

/// neutral hadron isolation
template <class LeptonType>
double Lepton<LeptonType>::neutralHadronIso(double dr) const {
  if( dr > 0 ){
    return (*(this->sourcePtr()))->isoDeposit(pat::PfNeutralHadronIso)->depositAndCountWithin( dr, neutralHadronVetos() ).first;
  }else{
    return neutralHadronIso_;
  }
}

/// photon isolation
template <class LeptonType>
double Lepton<LeptonType>::photonIso(double dr) const {
  if( dr > 0 ){
    return (*(this->sourcePtr()))->isoDeposit(pat::PfGammaIso)->depositAndCountWithin( dr, photonVetos() ).first;
  }else{
    return photonIso_;
  }
}

}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_*/
