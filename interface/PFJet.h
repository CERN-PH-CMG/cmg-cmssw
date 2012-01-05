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
    ///number of PFCandidates types, see PFCandidate.h
    static const unsigned NCANDTYPES;
    
    PFJet() : components_(NCANDTYPES), ptd_(-999) {}
    PFJet(const value& m): BaseJet(m), components_(NCANDTYPES), ptd_(-999) {}

    virtual ~PFJet(){}
    
    const PFJetComponent& component(unsigned ci) const; 

    const int nConstituents() const;
    float ptd() const {return ptd_;}

    friend class PFJetFactory;
    
  private:
    ///contains one PFJetComponent for each type of PFCandidate. 
    ///see PFJetComponent
    std::vector<PFJetComponent> components_; 

    /// just a dummy PFJetComponent, returned by the component accessor
    /// in case of problems
    static PFJetComponent dummy_;
    
    float ptd_;
  };
}

#endif /*JET_H_*/
