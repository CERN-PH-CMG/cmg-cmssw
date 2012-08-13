#ifndef ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

#include "DataFormats/Math/interface/Point3D.h"

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
    chargedHadronIsoR03_(UnSet(double)),
    chargedAllIsoR03_(UnSet(double)),
    puChargedHadronIsoR03_(UnSet(double)),
    neutralHadronIsoR03_(UnSet(double)),
    photonIsoR03_(UnSet(double)),
    chargedHadronIsoR04_(UnSet(double)),
    chargedAllIsoR04_(UnSet(double)),
    puChargedHadronIsoR04_(UnSet(double)),
    neutralHadronIsoR04_(UnSet(double)),
    photonIsoR04_(UnSet(double)),
    dxy_(UnSet(float)),
    dz_(UnSet(float)){}
    
  virtual ~Lepton(){}
  
  int charge() const{
    return charge_;
  }
                                
  /// charged hadron isolation    
  double chargedHadronIso(double dr=0) const{
    if( dr == 0 ) return chargedHadronIso_;
    else if( dr == 0.3) return chargedHadronIsoR03_;
    else if( dr == 0.4) return chargedHadronIsoR04_;
    else return -1;
  }

  /// charged particle isolation (e, mu, h+-)
  double chargedAllIso(double dr=0) const{
    if( dr == 0) return chargedAllIso_;
    else if ( dr == 0.3 ) return chargedAllIsoR03_;
    else if ( dr == 0.4 ) return chargedAllIsoR04_;
    else return -1;
  }

  /// pile-up charged hadron isolation, used for dbeta corrections
  double puChargedHadronIso(double dr=0) const{
    if( dr == 0) return puChargedHadronIso_;
    else if( dr == 0.3) return puChargedHadronIsoR03_;
    else if( dr == 0.4) return puChargedHadronIsoR04_;
    else return -1;
  }

  /// neutral hadron isolation
  double neutralHadronIso(double dr=0) const{
    if( dr == 0) return neutralHadronIso_;
    else if( dr == 0.3) return neutralHadronIsoR03_;
    else if( dr == 0.4) return neutralHadronIsoR04_;
    else return -1;
  }

  /// photon isolation
  double photonIso(double dr=0) const{
    if( dr == 0) return photonIso_;
    else if( dr == 0.3) return photonIsoR03_;
    else if( dr == 0.4) return photonIsoR04_;
    else return -1;
  }

  /// absolute isolation. if dBetaFactor > 0, the delta beta correction is applied. 
  /// if allCharged is true, charged hadron isolation is replaced by charged 
  /// particle isolation.
  double absIso(float dBetaFactor=0, int allCharged=0, double dr=0) const{
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
  double relIso(float dBetaFactor=0, int allCharged=0, double dr=0) const{
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
  //isolation values from iso-deposit
  double chargedHadronIsoR03_;
  double chargedAllIsoR03_;
  double puChargedHadronIsoR03_;
  double neutralHadronIsoR03_;
  double photonIsoR03_;
  double chargedHadronIsoR04_;
  double chargedAllIsoR04_;
  double puChargedHadronIsoR04_;
  double neutralHadronIsoR04_;
  double photonIsoR04_;

  /// transverse impact parameter
  float dxy_;

  /// longitudinal impact parameter
  float dz_;


};


}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_*/
