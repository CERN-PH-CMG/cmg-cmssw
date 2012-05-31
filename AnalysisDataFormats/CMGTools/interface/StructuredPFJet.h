#ifndef _AnalysisDataFormats_CMGTools_StructuredPFJet_H_
#define _AnalysisDataFormats_CMGTools_StructuredPFJet_H_

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

#include <vector>

namespace cmg {

  //forward def needed
  class StructuredPFJet;
  
  /** Class representing PFJets with substructure in the CMG framework.
  */
  class StructuredPFJet : public PFJet {
  public:
    StructuredPFJet() : PFJet() {}
    StructuredPFJet(const value& m): PFJet(m) {}

    virtual ~StructuredPFJet() {}
    
    std::vector<cmg::PFJet> subJets() { return subJetCollection_; }
    
    friend class StructuredPFJetFactory;
    
  private:

    std::vector<cmg::PFJet> subJetCollection_;
    
  };
}

#endif /*JET_H_*/
