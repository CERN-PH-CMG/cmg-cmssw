#ifndef TAU_H_
#define TAU_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

#include <vector>
#include <string>

using namespace std;

namespace cmg
{

#define NCMGTAUIDS 20

  //forward def needed
  class Tau;
  class TauFactory;
  
  class Tau : public cmg::Lepton<pat::TauPtr>{
  public:
    
    Tau(){
    }

    Tau(const value& m):
      cmg::Lepton<value>::Lepton(m),
      numberChargedHadr_(UnSet(int)),
      leadChargedHadrPt_(UnSet(double)),
      leadChargedHadrCharge_(UnSet(int)),
      leadChargedHadrHCalEnergy_(UnSet(double)),
      leadChargedHadrECalEnergy_(UnSet(double)),
      leadChargedHadrMvaEPi_(UnSet(double)),
      leadChargedHadrVertex_(reco::TrackBase::Point()),
      numberGamma_(UnSet(int)),
      leadNeutralCandPt_(UnSet(double)),
      leadNeutralCandECalEnergy_(UnSet(double)),
      isolation_(UnSet(double)),
      decayMode_(UnSet(int)){
      
      for(int i=0;i<NCMGTAUIDS;i++)	
	tauID_[i]="";
      
    }

    virtual ~Tau(){
    }
    

    //track related properties
    int numberChargedHadr() const {return numberChargedHadr_;}
    float leadChargedHadrPt() const {return leadChargedHadrPt_;}
    float leadChargedHadrCharge() const {return leadChargedHadrCharge_;}
    float leadChargedHadrHCalEnergy() const {return leadChargedHadrHCalEnergy_;}
    float leadChargedHadrECalEnergy() const {return leadChargedHadrECalEnergy_;}
    float leadChargedHadrMvaEPi() const {return leadChargedHadrMvaEPi_;}
    reco::TrackBase::Point leadChargedHadrVertex() const {return leadChargedHadrVertex_;}

    //neutral related properties
    int numberGamma() const {return numberGamma_;}
    float leadNeutralCandPt() const {return leadNeutralCandPt_;}
    float leadNeutralCandECalEnergy() const {return leadNeutralCandECalEnergy_;}

    //general properties
    float isolation() const {return isolation_;}
    int decayMode() const {return decayMode_;}
    bool tauID(string idname) const { 
      for(int i=0;i<NCMGTAUIDS;i++) if(tauID_[i]==idname) return 1;
      return 0;
    } 

    
    friend class cmg::TauFactory;
	
  private:


    int numberChargedHadr_;
    float leadChargedHadrPt_;
    float leadChargedHadrCharge_;
    float leadChargedHadrHCalEnergy_;
    float leadChargedHadrECalEnergy_;
    float leadChargedHadrMvaEPi_;
    reco::TrackBase::Point leadChargedHadrVertex_;

    int numberGamma_;
    float leadNeutralCandPt_;   
    float leadNeutralCandECalEnergy_;   
    
    float isolation_;
    int decayMode_;
    string tauID_[NCMGTAUIDS];
  };

}

#endif /*TAU_H_*/
