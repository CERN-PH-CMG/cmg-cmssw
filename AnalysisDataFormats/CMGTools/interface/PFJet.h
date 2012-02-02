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
  
  /** Class representing PFJets in the CMG framework.
      
  In addition to the attributes from the BaseJet mother class, 
  PFJets contain a vector of PFJetComponent, indexed by the particle type 
  defined in reco::PFCandidate. 
  
  Each component contains:
  - fraction of the jet energy carried by this type of constituents (e.g. charged hadrons)
  - number of such constituents in the jet
  - energy sum of such constituents
  - pt sum of such consistuents
  */
  class PFJet : public BaseJet {
  public:
    ///number of PFCandidates types, see PFCandidate.h
    static const unsigned NCANDTYPES;
    
    PFJet() : components_(NCANDTYPES), ptd_(-999) {}
    PFJet(const value& m): BaseJet(m), components_(NCANDTYPES), ptd_(-999) {}

    virtual ~PFJet(){}
    
    /// \return the component corresponding to this type of PFCandidate.
    /// for example, do:
    ///  jet.component( reco::PFCandidate::h)  
    /// or 
    ///  jet.component( 1 ) 
    /// to get the PFJetComponent corresponding to charged hadrons
    const PFJetComponent& component(unsigned ci) const; 
    
    /// \return total number of consistuents in the jet
    const int nConstituents() const;

    /// \return the ptd variable, for quark-gluon jet discrimination 
    /// \sum pt^2 / (\sum pt)^2
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
