#ifndef ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_
#define ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

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
  double chargedHadronIso() const{
    return chargedHadronIso_;
  }

  /// charged particle isolation (e, mu, h+-)
  double chargedAllIso() const{
    return chargedAllIso_;
  }
  
  /// pile-up charged hadron isolation, used for dbeta corrections
  double puChargedHadronIso() const{
    return puChargedHadronIso_;
  }
  
  /// neutral hadron isolation
  double neutralHadronIso() const{
    return neutralHadronIso_;
  }
  
  /// photon isolation
  double photonIso() const{
    return photonIso_;
  }
  
  /// absolute isolation. if dBetaFactor > 0, the delta beta correction is applied. 
  /// if allCharged is true, charged hadron isolation is replaced by charged 
  /// particle isolation.
  double absIso(float dBetaFactor=0, int allCharged=0) const{
    // in this case, dbeta correction is asked, but 
    // the input for this correction is not available. 
    // better returning an unphysical result than applying a wrong correction.
    if(dBetaFactor>0 && puChargedHadronIso()<0) return -1;

    double neutralIso = neutralHadronIso() + photonIso();
    double corNeutralIso = neutralIso - dBetaFactor * puChargedHadronIso();

    double charged = chargedHadronIso();
    if( allCharged ) charged = chargedAllIso();

    return charged + ( corNeutralIso>0 ? corNeutralIso : 0 ) ;   
  }
  
  /// relative isolation. if dBetaFactor > 0, the delta beta correction is applied. 
  /// if allCharged is true, charged hadron isolation is replaced by charged 
  /// particle isolation.
  double relIso(float dBetaFactor=0, int allCharged=0) const{
    double abs = absIso(dBetaFactor, allCharged)/this->pt();
    return abs >=0 ? abs : -1;
  }
    
  /// transverse impact parameter
  float dxy() const{
    return dxy_;
  }

  /// longitudinal impact parameter
  float dz() const{
    return dz_;
  }
  
  friend class cmg::LeptonSettingTool<LeptonType>;
	
 protected:
  /// 3d impact parameter 
  float dB3D_;

  /// error on 3d impact parameter
  float edB3D_;

 private:
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


}

#endif /*ANALYSISDATAFORMATS_CMGTOOLS_LEPTON_H_*/
