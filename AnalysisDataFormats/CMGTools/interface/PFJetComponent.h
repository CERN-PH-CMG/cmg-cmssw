#ifndef __AnalysisDataFormats_CMGTools_JetComponent__
#define __AnalysisDataFormats_CMGTools_JetComponent__

// #include "DataFormats/Math/interface/LorentzVector.h"
/* #include "AnalysisDataFormats/CMGTools/interface/FloatValue.h" */

#include <vector>
#include <iostream>

namespace cmg {
  
  // typedef math::PtEtaPhiMLorentzVector LorentzVector;
  
  //TODO: simplify the interface - add a Component class? 
  class PFJetComponent {
  public:
    PFJetComponent();
    
    void clear();
      
    void setNumber(int number) {number_=number;}
    void setFraction(float fraction) {fraction_=fraction;}
    void setEnergy(float energy) {energy_=energy;}
    void setPt(float pt) {pt_=pt;};

    int number() const {return number_;}
    float fraction() const {return fraction_;}
    float energy() const {return energy_;}
    float pt() const {return pt_;}

  private:
    int number_;
    float fraction_;
    float energy_;
    float pt_;
  };

  std::ostream& operator<<(std::ostream& out, const PFJetComponent& comp) ;

}
#endif
