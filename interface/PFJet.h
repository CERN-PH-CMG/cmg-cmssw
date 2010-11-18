#ifndef _AnalysisDataFormats_CMGTools_PFJet_H_
#define _AnalysisDataFormats_CMGTools_PFJet_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJetComponent.h"

#include <vector>

namespace cmg {

  //forward def needed
  class PFJet;
  
  class PFJet : public BaseJet {
  public:
    
    static const unsigned NCANDTYPES;
    
    PFJet() : components_(NCANDTYPES) {}
    PFJet(const value& m): BaseJet(m), components_(NCANDTYPES) {}

    virtual ~PFJet(){}
    
    const PFJetComponent& component(unsigned ci) const; 

    const int nConstituents() const;

    friend class PFJetFactory;
    
  private:
    std::vector<PFJetComponent> components_; 
 
    static PFJetComponent dummy_;

  };
}

#endif /*JET_H_*/
