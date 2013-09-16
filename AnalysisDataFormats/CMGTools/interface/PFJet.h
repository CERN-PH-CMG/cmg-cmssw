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

    PFJet() : components_(NCANDTYPES), ptd_(-999), rms_(UnSet(float)), beta_(UnSet(float)), vtxpt_(UnSet (float)), vtx3dL_(UnSet (float)), vtx3deL_(UnSet (float)), axis_major_(UnSet (float)), axis_minor_(UnSet (float)), pt_max_(UnSet (float)), tana_(UnSet (float)), ttheta_(UnSet (float)), pull_(UnSet (float)), fmax_charged_(UnSet (float)), fmax_neutral_(UnSet (float)), fmax_(UnSet (float)), n_charged_(UnSet (int)), n_neutral_(UnSet (int)), n_charged_ptcut_(UnSet (int)), n_neutral_ptcut_(UnSet (int)),       axis_major_qc_(UnSet (float)), axis_minor_qc_(UnSet (float)), ptd_qc_(UnSet (float)), pull_qc_(UnSet (float)), fmax_charged_qc_(UnSet (float)), n_charged_qc_(UnSet (int)), n_charged_ptcut_qc_(UnSet (int)), girth_(UnSet (int)), girth_charged_(UnSet (int)) {}

      PFJet(const value& m): BaseJet(m), components_(NCANDTYPES), ptd_(-999), rms_(UnSet(float)), vtxpt_(UnSet (float)), vtx3dL_(UnSet (float)), vtx3deL_(UnSet (float)), axis_major_(UnSet (float)), axis_minor_(UnSet (float)), pt_max_(UnSet (float)), tana_(UnSet (float)), ttheta_(UnSet (float)), pull_(UnSet (float)), fmax_charged_(UnSet (float)), fmax_neutral_(UnSet (float)), fmax_(UnSet (float)), n_charged_(UnSet (int)), n_neutral_(UnSet (int)), n_charged_ptcut_(UnSet (int)), n_neutral_ptcut_(UnSet (int)),       axis_major_qc_(UnSet (float)), axis_minor_qc_(UnSet (float)), ptd_qc_(UnSet (float)), pull_qc_(UnSet (float)), fmax_charged_qc_(UnSet (float)), n_charged_qc_(UnSet (int)), n_charged_ptcut_qc_(UnSet (int)), girth_(UnSet (int)), girth_charged_(UnSet (int)) {}
	
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

    float vtxPt()         const {return vtxpt_;}
    float vtx3dL()        const {return vtx3dL_;}
    float vtx3deL()       const {return vtx3deL_;}
    float axisMajor()     const {return axis_major_;}
    float axisMinor()     const {return axis_minor_;}
    float axisMajorQC()   const {return axis_major_qc_;}
    float axisMinorQC()   const {return axis_minor_qc_;}
    float tanAxisAngle()  const {return tana_;}
    float thrustAngle()   const {return ttheta_;}
    float pull()          const {return pull_;}
    float pullQC()        const {return pull_qc_;}
    float ptMax()         const {return pt_max_;}
    float fmax()          const {return fmax_;}
    float fmaxCharged()   const {return fmax_charged_;}
    float fmaxChargedQC() const {return fmax_charged_qc_;}
    float fmaxNeutral()   const {return fmax_neutral_;}
    int nCharged()        const {return n_charged_;}
    int nChargedQC()      const {return n_charged_qc_;}
    int nChargedPtCut()   const {return n_charged_ptcut_;}
    int nChargedPtCutQC() const {return n_charged_ptcut_qc_;}
    int nNeutral()        const {return n_neutral_;}
    int nNeutralPtCut()   const {return n_neutral_ptcut_;}
    float ptdQC()         const {return ptd_qc_;}
    float girth()         const {return girth_;}
    float girth_charged() const {return girth_charged_;}

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
    bool passPuJetId(const std::string & name, PileupJetIdentifier::Id level) const { 
      return PileupJetIdentifier::passJetId(puId(name),level); 
    } 
/*     bool passPuJetId(const std::string & name, int level) const {  */
/*       return PileupJetIdentifier::passJetId(puId(name),static_cast<PileupJetIdentifier::Id>(level));  */
/*     }  */
    bool passPuJetId(const char* name, PileupJetIdentifier::Id level) const {
      return PileupJetIdentifier::passJetId(puId(std::string(name)),level);
    }
    bool passLooseFullPuJetId() const {
      return PileupJetIdentifier::passJetId( puId("full"), PileupJetIdentifier::kLoose ); 
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
    typedef boost::array<int,10> PuIdArray;
    typedef boost::array<float,PuIdArray::static_size> PuMvaArray;
    typedef boost::array<std::string,PuIdArray::static_size> PuIdNames;
    PuIdArray puIds_;
    PuMvaArray puMvas_;
    PuIdNames puIdNames_;
    float rms_;
    float beta_;

    // variables associated to a secondary vertex inside a jet
    float vtxpt_;
    float vtx3dL_;
    float vtx3deL_;

    // variables needed for extended quark-gluon tagger
    float axis_major_;
    float axis_minor_; 
    float pt_max_;
    float tana_;
    float ttheta_;
    float pull_;
    float fmax_charged_;
    float fmax_neutral_;
    float fmax_;
    int n_charged_;
    int n_neutral_;
    int n_charged_ptcut_;
    int n_neutral_ptcut_;
   
    // variables with quality cuts  
    float axis_major_qc_;
    float axis_minor_qc_; 
    float ptd_qc_;
    float pull_qc_;
    float fmax_charged_qc_;
    int n_charged_qc_;
    int n_charged_ptcut_qc_;

    //variables for QGdiscrimination
    float girth_;
    float girth_charged_;

  };
}

#endif /*JET_H_*/
