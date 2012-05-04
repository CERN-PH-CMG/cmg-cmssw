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
#include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"

#include <vector>
#include <string>


namespace cmg
{

#define NCMGTAUIDS 20


  //forward def needed
  class Tau;
  class TauFactory;
  
  class Tau : public cmg::Lepton<pat::TauPtr>{
  public:
    
    typedef cmg::SimpleParticle Constituent; 
    typedef std::vector< Constituent > Constituents;

    Tau(){
    }

    Tau(const value& m):
      cmg::Lepton<value>::Lepton(m),
      leadChargedHadrPt_(UnSet(double)),
      leadChargedHadrCharge_(UnSet(int)),
      leadChargedHadrHcalEnergy_(UnSet(double)),
      leadChargedHadrEcalEnergy_(UnSet(double)),
      leadChargedHadrMvaEPi_(UnSet(double)),
      leadChargedHadrVertex_(reco::TrackBase::Point()),
      leadNeutralCandPt_(UnSet(double)),
      //COLIN Ecal should be ECAL or Ecal. Same for Hcal
      leadNeutralCandEcalEnergy_(UnSet(double)),
      particleIso_(UnSet(double)),
/*       trackIso_(UnSet(double)), */
/*       gammaIso_(UnSet(double)), */
      isolationPFChargedHadrCandsPtSum_( UnSet(double) ),
      isolationPFNeutralHadrCandsPtSum_( UnSet(double) ),
      isolationPFGammaCandsEtSum_( UnSet(double) ),
      decayMode_(UnSet(int)),
      tauID_(NCMGTAUIDS, 0),
      tauIDLabels_(NCMGTAUIDS, ""),      
      jetRefp4_(math::XYZTLorentzVector(0.,0.,0.,0.)),
      genJetp4_(math::XYZTLorentzVector(0.,0.,0.,0.)),
      genJetCharge_(0),
      genJetDecayMode_(""),
      genTaup4_(math::XYZTLorentzVector(0.,0.,0.,0.))
    {
      
      //      for(int i=0;i<NCMGTAUIDS;i++)	
      //	tauID_[i]="";
      
      
    }

    virtual ~Tau(){
    }
    
    float leadChargedHadrPt() const {return leadChargedHadrPt_;}
    float leadChargedHadrCharge() const {return leadChargedHadrCharge_;}
    float leadChargedHadrHcalEnergy() const {return leadChargedHadrHcalEnergy_;}
    float leadChargedHadrEcalEnergy() const {return leadChargedHadrEcalEnergy_;}
    float leadChargedHadrMvaEPi() const {return leadChargedHadrMvaEPi_;}
    reco::TrackBase::Point leadChargedHadrVertex() const {return leadChargedHadrVertex_;}

    float leadNeutralCandPt() const {return leadNeutralCandPt_;}
    float leadNeutralCandEcalEnergy() const {return leadNeutralCandEcalEnergy_;}

    float particleIso() const {return particleIso_;}
    float isolationPFChargedHadrCandsPtSum() const {return isolationPFChargedHadrCandsPtSum_;}
    float isolationPFNeutralHadrCandsPtSum() const {return isolationPFNeutralHadrCandsPtSum_;}
    float isolationPFGammaCandsEtSum() const {return isolationPFGammaCandsEtSum_;}
    int decayMode() const {return decayMode_;}
    float eOverP() const {return (leadChargedHadrEcalEnergy() + leadChargedHadrHcalEnergy()) * sin( theta() ) / leadChargedHadrPt();}

    const Constituents& signalPFCands() const {return signalPFCands_;}
    const Constituent& leadPFChargedHadrCand() const {return leadPFChargedHadrCand_;}

    //tau ID's
    float tauID(const std::string& idname) const { 
      for(unsigned i=0;i<tauIDLabels_.size();i++)
	if(tauIDLabels_[i]==idname) 
	  return tauID_[i]; //passed
      return -99;//failed
    } 

    float tauID(const char* idname) const {
      return tauID( std::string(idname) );
    }

    //info about the original PFJet
    math::XYZTLorentzVector jetRefp4() const {return jetRefp4_;}

    //generator info filled when the candidate is truth-matched
    math::XYZTLorentzVector genJetp4() const {return genJetp4_;}
    int genJetCharge() const {return genJetCharge_;}
    std::string genJetDecayMode() const {return genJetDecayMode_;}
    math::XYZTLorentzVector genTaup4() const {return genTaup4_;}


    friend class cmg::TauFactory;
	
  private:


    float leadChargedHadrPt_;
    float leadChargedHadrCharge_;
    float leadChargedHadrHcalEnergy_;
    float leadChargedHadrEcalEnergy_;
    float leadChargedHadrMvaEPi_;
    reco::TrackBase::Point leadChargedHadrVertex_;
    
    float leadNeutralCandPt_;   
    float leadNeutralCandEcalEnergy_;   
   
    float particleIso_;
/*     float trackIso_; */
/*     float gammaIso_; */
    float isolationPFChargedHadrCandsPtSum_;
    float isolationPFNeutralHadrCandsPtSum_;
    float isolationPFGammaCandsEtSum_;
    int decayMode_;
    // std::string tauID_[NCMGTAUIDS];
    std::vector<float>        tauID_;
    std::vector<std::string>  tauIDLabels_;
    
    math::XYZTLorentzVector jetRefp4_;

    math::XYZTLorentzVector genJetp4_;
    int genJetCharge_;
    std::string genJetDecayMode_;
    math::XYZTLorentzVector genTaup4_;

    Constituent   leadPFChargedHadrCand_;
    Constituents  signalPFCands_;
  };

}

#endif /*TAU_H_*/
