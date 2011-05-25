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
      leadHadrHCalEnergy(UnSet(double)),
      leadHadrECalEnergy(UnSet(double)),
      leadCandHCalEnergy(UnSet(double)),
      leadChargedHadrMvaEPi(UnSet(double)),
      leadCandMvaEPi(UnSet(double)),
      leadChargedHadrTrkPt(UnSet(double)),
      leadNeutralCandPt(UnSet(double)),
      numberChargedHadr(UnSet(int)),
      numberGamma(UnSet(int)),
      decayMode(UnSet(int)){
      
      for(Int_t i=0;i<NCMGTAUIDS;i++)	
	tauIDname[i]="";
      
    }

    virtual ~Tau(){
    }
    



    Float_t getLeadHadrHCalEnergy() const {return leadHadrHCalEnergy;}
    Float_t getLeadHadrECalEnergy() const {return leadHadrECalEnergy;}
    Float_t getLeadCandHCalEnergy() const {return leadCandHCalEnergy;}
    Float_t getLeadChargedHadrMvaEPi() const {return leadChargedHadrMvaEPi;}
    Float_t getLeadCandMvaEPi() const {return leadCandMvaEPi;}
    Float_t getLeadChargedHadrTrkPt() const {return leadChargedHadrTrkPt;}
    Float_t getLeadNeutralCandPt() const {return leadNeutralCandPt;}
    Int_t getNumberChargedHadr() const {return numberChargedHadr;}
    Int_t getNumberGamma() const {return numberGamma;}


    Bool_t  getTauID(string idname) const { 
      for(Int_t i=0;i<NCMGTAUIDS;i++)
	if(tauIDname[i]==idname) return 1;

      return 0;
    }

    Int_t getDecayMode() const {return decayMode;}
    

    friend class cmg::TauFactory;
	
  private:
    
    Float_t leadHadrHCalEnergy;
    Float_t leadHadrECalEnergy;
    Float_t leadCandHCalEnergy;
    Float_t leadChargedHadrMvaEPi;
    Float_t leadCandMvaEPi;
    Float_t leadChargedHadrTrkPt;
    Float_t leadNeutralCandPt;
    Int_t numberChargedHadr;
    Int_t numberGamma;

    string tauIDname[NCMGTAUIDS];

    Int_t decayMode;

  };

}

#endif /*TAU_H_*/
