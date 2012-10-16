#ifndef HEMIFACTORY_H_
#define HEMIFACTORY_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/HemiBalanceAlgos.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/MultiObjectSettingTool.h"

#include <memory>
#include <utility>

namespace cmg {

  typedef std::vector<edm::InputTag> VInputTag;

  class HemisphereFactory : public Factory<cmg::Hemisphere>,
    public SettingTool<std::vector<edm::Ptr<reco::Candidate> >, cmg::Hemisphere> {

  public:
    HemisphereFactory(const edm::ParameterSet& ps) :
      hemisphereLabel_(ps.getParameter<VInputTag>("inputCollection")),
      maxNCand_(ps.getParameter<uint32_t>("maxCand")),
      minObjectsPerHemi0_(ps.getUntrackedParameter<uint32_t>("minObjectsPerHemi0", 0)),	
      minObjectsPerHemi1_(ps.getUntrackedParameter<uint32_t>("minObjectsPerHemi1", 0)),
      keepAll_(ps.getUntrackedParameter<bool>("keepAll", false)),
      balance_(new cmg::PtBalance){
        if(ps.exists("balanceAlgorithm")){
            if(ps.getParameter<uint32_t>("balanceAlgorithm") == cmg::Balance::MassBalance){
                balance_ = std::auto_ptr<cmg::Balance>(new cmg::MassBalance);
            }else if(ps.getParameter<uint32_t>("balanceAlgorithm") == cmg::Balance::TopMassBalance){
                balance_ = std::auto_ptr<cmg::Balance>(new cmg::TopMassBalance);
            }
        }
      }

    typedef cmg::Factory<cmg::Hemisphere>::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&,
                             const edm::EventSetup&);
    void set(std::vector<edm::Ptr<reco::Candidate> > const& input,
             cmg::Hemisphere& object) const;

  private:
    VInputTag const hemisphereLabel_;
    const size_t maxNCand_;
    const unsigned int minObjectsPerHemi0_;
    const unsigned int minObjectsPerHemi1_;
    const bool keepAll_;
    MultiObjectSettingTool const multiObjectFactory_;
    std::auto_ptr<cmg::Balance> balance_;

  };

} // namespace cmg

#endif /*HEMIFACTORY_H_*/
