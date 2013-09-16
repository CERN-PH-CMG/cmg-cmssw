#ifndef _AnalysisDataFormats_CMGTools_BaseJet_H_
#define _AnalysisDataFormats_CMGTools_BaseJet_H_

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"

#include <string>
#include <boost/array.hpp>

namespace cmg {

  //forward def needed
  class BaseJet;

  class BaseJet : public PhysicsObjectWithPtr< pat::JetPtr > {
  public:

    BaseJet(){}
    BaseJet(const value& m):
      PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(m),
      secvtxMass_(-1.),
      Lxy_(-1.),
      LxyErr_(-1.),
      partonFlavour_(UnSet(Int_t)),
      rawFactor_(1),
      uncOnFourVectorScale_(0.){
        std::fill(btag_.begin(),btag_.end(),UnSet(double));
      }

    virtual ~BaseJet(){}

    /// \setter for the jec uncertainty: needed for the PFJetFactory.cc 
    void setUncOnFourVectorScale(float fUnc) {uncOnFourVectorScale_ = fUnc;}

    /// \return btag discriminator
    double btag(unsigned int index = 0) const{ return index < btag_.size() ? btag_.at(index) : UnSet(double); }
    double btag(const char* s) const;
    double bDiscriminator(const char* s) const{ return btag(s); }

    /// \return secondary vertex b-tagging information
    Float_t secvtxMass() const { return secvtxMass_ ; }
    Float_t Lxy() const { return Lxy_ ; }
    Float_t LxyErr() const { return LxyErr_; }
   
    /// \return the matched MC parton flavour
    Int_t partonFlavour() const{ return partonFlavour_;}
    
    /// \return the jet area 
    float jetArea() const {return jetArea_;} 

    /// \return a correction factor that can be applied to the jet energy or pT to bring
    /// it back to the uncorrected value
    Float_t rawFactor() const {return rawFactor_;}

    /// Uncertainty on four vector energy scale.
    Float_t uncOnFourVectorScale() const {return uncOnFourVectorScale_;}

    friend class BaseJetFactory;
    template <class> friend class JetEnergyCorrector;

    virtual bool isJet() const {
      return true;
    }

  private:
 
    /// b tagging discriminators
    typedef boost::array<double,16> TagArray;
    typedef boost::array<std::string,TagArray::static_size> TagNameArray;
    TagArray btag_;
    TagNameArray btagNames_;
   
    /// b tagging information
    Float_t secvtxMass_;
    Float_t Lxy_;
    Float_t LxyErr_;

    //parton flavour
    Int_t partonFlavour_;

    /// jet area
    float  jetArea_;

    /// Correction factor that can be applied to the jet energy or pT
    /// to bring it back to the uncorrected value.
    Float_t rawFactor_;

    /// The uncertainty on the four-vector scale. This can be used to
    /// scale up/down the four-vector according to the scale
    /// uncertainty by multiplying by (1. +/- uncOnFourVectorScale_).
    Float_t uncOnFourVectorScale_;

  };
}

#endif /*JET_H_*/
