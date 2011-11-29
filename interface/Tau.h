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


namespace cmg
{

#define NCMGTAUIDS 20
#define NCMGTAUUSERDATA 50

  //forward def needed
  class Tau;
  class TauFactory;
  
  class Tau : public cmg::Lepton<pat::TauPtr>{
  public:
    
    Tau(){
    }

    Tau(const value& m):
      cmg::Lepton<value>::Lepton(m),
      leadChargedHadrPt_(UnSet(double)),
      leadChargedHadrCharge_(UnSet(int)),
      leadChargedHadrHCalEnergy_(UnSet(double)),
      leadChargedHadrECalEnergy_(UnSet(double)),
      leadChargedHadrMvaEPi_(UnSet(double)),
      leadChargedHadrVertex_(reco::TrackBase::Point()),
      leadNeutralCandPt_(UnSet(double)),
      leadNeutralCandECalEnergy_(UnSet(double)),
      particleIso_(UnSet(double)),
      trackIso_(UnSet(double)),
      gammaIso_(UnSet(double)),
      decayMode_(UnSet(int)),
      genJetp4_(math::XYZTLorentzVector(0.,0.,0.,0.)),
      genJetCharge_(0),
      genJetDecayMode_(""),
      genTaup4_(math::XYZTLorentzVector(0.,0.,0.,0.))
    {
      
      for(int i=0;i<NCMGTAUIDS;i++)	
	tauID_[i]="";
      
      for(int i=0;i<NCMGTAUUSERDATA;i++){
	userData_[i]=0.0;
	userDataName_[i]="";
      }
      
    }

    virtual ~Tau(){
    }
    

    //track related properties
    float leadChargedHadrPt() const {return leadChargedHadrPt_;}
    float leadChargedHadrCharge() const {return leadChargedHadrCharge_;}
    float leadChargedHadrHCalEnergy() const {return leadChargedHadrHCalEnergy_;}
    float leadChargedHadrECalEnergy() const {return leadChargedHadrECalEnergy_;}
    float leadChargedHadrMvaEPi() const {return leadChargedHadrMvaEPi_;}
    reco::TrackBase::Point leadChargedHadrVertex() const {return leadChargedHadrVertex_;}

    //neutral related properties
    float leadNeutralCandPt() const {return leadNeutralCandPt_;}
    float leadNeutralCandECalEnergy() const {return leadNeutralCandECalEnergy_;}

    //general properties
    float particleIso() const {return particleIso_;}
    float trackIso() const {return trackIso_;}
    float gammaIso() const {return gammaIso_;}
    int decayMode() const {return decayMode_;}
    float eOverP() const {return (leadChargedHadrECalEnergy() + leadChargedHadrHCalEnergy()) * sin( theta() ) / leadChargedHadrPt();}

    //tau ID's
    bool tauID(const std::string& idname) const { 
      for(int i=0;i<NCMGTAUIDS;i++)
	if(tauID_[i]==idname) return 1; //passed
      return 0;//failed
    } 

    bool tauID(const char* idname) const {
      return tauID( std::string(idname) );
    }

    //user data
    float userData(const std::string& dataname) const {
      for(int i=0;i<NCMGTAUUSERDATA;i++)
	if(dataname==userDataName_[i])return userData_[i];
      std::cout<<"cmg::Tau userData : "<<dataname<<" not found. Available are : ";
      for(int i=0;i<NCMGTAUUSERDATA;i++)
	std::cout<<userData_[i]<<", ";
      std::cout<<std::endl;
      return 0.0;
    }

    //generator info filled when the candidate is truth-matched
    math::XYZTLorentzVector genJetp4() const {return genJetp4_;}
    int genJetCharge() const {return genJetCharge_;}
    std::string genJetDecayMode() const {return genJetDecayMode_;}
    math::XYZTLorentzVector genTaup4() const {return genTaup4_;}


    friend class cmg::TauFactory;
	
  private:


    float leadChargedHadrPt_;
    float leadChargedHadrCharge_;
    float leadChargedHadrHCalEnergy_;
    float leadChargedHadrECalEnergy_;
    float leadChargedHadrMvaEPi_;
    reco::TrackBase::Point leadChargedHadrVertex_;
    
    float leadNeutralCandPt_;   
    float leadNeutralCandECalEnergy_;   
   
    float particleIso_;
    float trackIso_;
    float gammaIso_;
    int decayMode_;
    std::string tauID_[NCMGTAUIDS];


    float userData_[NCMGTAUUSERDATA];
    std::string userDataName_[NCMGTAUUSERDATA];
    bool addUserData(const std::string& name,float value){
      for(int i=0;i<NCMGTAUUSERDATA;i++)
	if(userDataName_[i]==name){
	  std::cout<<"cmg::Tau::addUserData  "<<name<<" already exists"<<std::endl;
	  return 0;
	}      
      for(int i=0;i<NCMGTAUUSERDATA;i++)
	if(userDataName_[i]==""){
	  userDataName_[i]=name;
	  userData_[i]=value;
	  return 1;
	}
      std::cout<<"cmg::Tau::addUserData unable to save user data: "<<name<<std::endl;
      return 0;
    }
    
    
    math::XYZTLorentzVector genJetp4_;
    int genJetCharge_;
    std::string genJetDecayMode_;
    math::XYZTLorentzVector genTaup4_;


  };

}

#endif /*TAU_H_*/
