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

#include <TString.h>

#define NCMGTAUIDS 20

using namespace std;

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
      leadNeutralCandPt(UnSet(double)),
      numberChargedHadr(UnSet(int)),
      numberGamma(UnSet(int)),
      decayMode(UnSet(int)){
      
      for(Int_t i=0;i<NCMGTAUIDS;i++){	
	tauIDname[i]="";
	tauIDvalue[i]=0;
      }
      
    }

    virtual ~Tau(){
    }
    



    Bool_t  getTauID(TString idname){
      for(Int_t i=0;i<NCMGTAUIDS;i++)
	if(tauIDname[i]==idname)
	  return tauIDvalue[i];

      ///if id was not found print the available id's
      cout<<"cmg::Tau::getTauID  this taudID="<<idname<<" is not found "<<endl;
      cout<<"The following tauIDs are available:";
      for(Int_t i=0;i<NCMGTAUIDS;i++)
	if(tauIDname[i]!="")cout<<" "<<tauIDname[i]<<",";
      cout<<endl;

      return 0;
    }


    Float_t getLeadHadrHCalEnergy(){return leadHadrHCalEnergy;}
    Float_t getLeadHadrECalEnergy(){return leadHadrECalEnergy;}
    Float_t getLeadCandHCalEnergy(){return leadCandHCalEnergy;}
    Float_t getLeadChargedHadrMvaEPi(){return leadChargedHadrMvaEPi;}
    Float_t getLeadCandMvaEPi(){return leadCandMvaEPi;}
    Float_t getLeadChargedHadrTrkPt(){return leadChargedHadrTrkPt;}
    Float_t getLeadNeutralCandPt(){return leadNeutralCandPt;}
    Int_t getNumberChargedHadr(){return numberChargedHadr;}
    Int_t getNumberGamma(){return numberGamma;}


    Int_t getDecayMode(){return decayMode;}
    

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

    TString tauIDname[NCMGTAUIDS];
    Bool_t tauIDvalue[NCMGTAUIDS];

    void setTauID(TString idname,Bool_t idvalue){
      //check the idname is not already set
      for(Int_t i=0;i<NCMGTAUIDS;i++){	
	if(tauIDname[i]==idname){
	  cout<<"cmg::Tau::setTauID  this taudID="<<idname<<" is being duplicated."<<endl;
	  exit(0);
	}
      }
 
      //add it to the next available slot
      for(Int_t i=0;i<NCMGTAUIDS;i++){	
	if(tauIDname[i]==""){
	  tauIDname[i]=idname;
	  tauIDvalue[i]=idvalue;
	  break;
	}
	if(i==NCMGTAUIDS-1){
	  cout<<"cmg::Tau::setTauID  Trying to add too many tauIDs"<<endl;
	  exit(0);
	}
      }
      
    }



    Int_t decayMode;

  };

}

#endif /*TAU_H_*/
