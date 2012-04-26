#ifndef _AnalysisDataFormats_CMGTools_PFJet_H_
#define _AnalysisDataFormats_CMGTools_PFJet_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "CMGTools/External/interface/PileupJetIdentifier.h"
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
    
    PFJet() : components_(NCANDTYPES), ptd_(-999), rms_(UnSet(float)), beta_(UnSet(float)) {}
    PFJet(const value& m): BaseJet(m), components_(NCANDTYPES), ptd_(-999), rms_(UnSet(float)), beta_(UnSet(float)) {}
	
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

    /// jet width
    float rms() const { return rms_; } 

    /// fraction of charged hadron sum pt from charged hadrons from the primary vertex
    float beta() const { return beta_; } 

    /// mva ouptut value for pile-up jet id (name should be "full", "simple" or "cut-based")
    float puMva(const std::string & name) const;
    float puMva(const char* name) const {return puMva(std::string(name));}
    /// name should be "full", "simple" or "cut-based".
    /// for this mva, returns an integeger containing 3 bits, one for each working point (loose-bit2, medium-bit1, tight-bit0)
    int   puId(const std::string & name) const;
    int   puId(const char* name) const {return puId(std::string(name));}

    /// returns true if the working point level is passed for mva name:
    /// - level = loose, medium, tight
    /// - name = "full", "simple" or "cut-based".
    /// see  PileupJetIdentifier for more information
    bool passPuJetId(const std::string & name, PileupJetIdentifier::Id level) const { return PileupJetIdentifier::passJetId(puId(name),level); } 
    bool passPuJetId(const char* name, PileupJetIdentifier::Id level) const { 
      return PileupJetIdentifier::passJetId(puId(std::string(name)),level); 
    } 

    friend class PFJetFactory;
    
  private:
    int puIdIndex(const std::string & name) const;
    float puMva(int i) const { return puMvas_[i]; }
    int   puId(int i) const { return puIds_[i]; }
    
    ///contains one PFJetComponent for each type of PFCandidate. 
    ///see PFJetComponent
    std::vector<PFJetComponent> components_; 

    /// just a dummy PFJetComponent, returned by the component accessor
    /// in case of problems
    static PFJetComponent dummy_;
    
    float ptd_;

    // Pile-Up discrimination
    typedef boost::array<int,3> PuIdArray;
    typedef boost::array<float,PuIdArray::static_size> PuMvaArray;
    typedef boost::array<std::string,PuIdArray::static_size> PuIdNames;
    PuIdArray puIds_;
    PuMvaArray puMvas_;
    PuIdNames puIdNames_;
    float rms_;
    float beta_;
    
  };
}

#endif /*JET_H_*/
