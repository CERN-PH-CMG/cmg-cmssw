#ifndef TAU_H_
#define TAU_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Lepton.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"


#include <vector>

namespace cmg
{

  //forward def needed
  class Tau;
  class TauFactory;
  
  class Tau : public cmg::Lepton<pat::TauPtr>{
  public:
    
    Tau(){
    }

    Tau(const value& m):
      cmg::Lepton<value>::Lepton(m),
      leadHadrHCalEnergy(UnSet(double)),
      leadHadrECalEnergy(UnSet(double)),
      leadCandHCalEnergy(UnSet(double)),
      leadChargedHadrMvaEPi(UnSet(double)),
      leadCandMvaEPi(UnSet(double)),
      leadChargedHadrTrkPt(UnSet(double)),
      numberChargedHadr(UnSet(int)),
      numberGamma(UnSet(int)),
      tauIDHPSagainstElectronLoose(0),
      tauIDHPSagainstElectronMedium(0),
      tauIDHPSagainstElectronTight(0),
      tauIDHPSagainstMuonLoose(0),
      tauIDHPSagainstMuonTight(0),
      tauIDHPSbyLooseIsolation(0),
      tauIDHPSbyMediumIsolation(0),
      tauIDHPSbyTightIsolation(0),
      tauIDHPSbyVLooseIsolation(0),
      tauIDHPSdecayModeFinding(0),
      decayMode(UnSet(int)){
    }

    virtual ~Tau(){
    }
    


    Float_t getLeadHadrHCalEnergy(){return leadHadrHCalEnergy;}
    Float_t getLeadHadrECalEnergy(){return leadHadrECalEnergy;}
    Float_t getLeadCandHCalEnergy(){return leadCandHCalEnergy;}
    Float_t getLeadChargedHadrMvaEPi(){return leadChargedHadrMvaEPi;}
    Float_t getLeadCandMvaEPi(){return leadCandMvaEPi;}
    Float_t getLeadChargedHadrTrkPt(){return leadChargedHadrTrkPt;}
    Int_t getNumberChargedHadr(){return numberChargedHadr;}
    Int_t getNumberGamma(){return numberGamma;}

    Bool_t getTauIDHPSagainstElectronLoose(){return tauIDHPSagainstElectronLoose;}
    Bool_t getTauIDHPSagainstElectronMedium(){return tauIDHPSagainstElectronMedium;}
    Bool_t getTauIDHPSagainstElectronTight(){return tauIDHPSagainstElectronTight;}
    Bool_t getTauIDHPSagainstMuonLoose(){return tauIDHPSagainstMuonLoose;}
    Bool_t getTauIDHPSagainstMuonTight(){return tauIDHPSagainstMuonTight;}
    Bool_t getTauIDHPSbyLooseIsolation(){return tauIDHPSbyLooseIsolation;}
    Bool_t getTauIDHPSbyMediumIsolation(){return tauIDHPSbyMediumIsolation;}
    Bool_t getTauIDHPSbyTightIsolation(){return tauIDHPSbyTightIsolation;}
    Bool_t getTauIDHPSbyVLooseIsolation(){return tauIDHPSbyVLooseIsolation;}
    Bool_t getTauIDHPSdecayModeFinding(){return tauIDHPSdecayModeFinding;}

    Int_t getDecayMode(){return decayMode;}
    

    friend class cmg::TauFactory;
	
  private:
    
    Float_t leadHadrHCalEnergy;
    Float_t leadHadrECalEnergy;
    Float_t leadCandHCalEnergy;
    Float_t leadChargedHadrMvaEPi;
    Float_t leadCandMvaEPi;
    Float_t leadChargedHadrTrkPt;
    Int_t numberChargedHadr;
    Int_t numberGamma;

    Bool_t tauIDHPSagainstElectronLoose;
    Bool_t tauIDHPSagainstElectronMedium;
    Bool_t tauIDHPSagainstElectronTight;
    Bool_t tauIDHPSagainstMuonLoose;
    Bool_t tauIDHPSagainstMuonTight;
    Bool_t tauIDHPSbyLooseIsolation;
    Bool_t tauIDHPSbyMediumIsolation;
    Bool_t tauIDHPSbyTightIsolation;
    Bool_t tauIDHPSbyVLooseIsolation;
    Bool_t tauIDHPSdecayModeFinding;


    Int_t decayMode;

  };

}

#endif /*TAU_H_*/
