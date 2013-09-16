#ifndef FATJETFACTORY_H_
#define FATJETFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/FatJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"

#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/MultiObjectSettingTool.h"

#include <TRandom3.h>
#include <algorithm>
#include <set>
#include <utility>

namespace cmg {

  class FatJetFactory : public cmg::Factory<cmg::FatJet>,
    public SettingTool<std::vector<edm::Ptr<reco::Candidate> >, cmg::FatJet> {
  public:

    FatJetFactory(const edm::ParameterSet& ps):
      collectionLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      Rmax_(ps.getParameter<double>("Rmax"))
        {}

      // Need to override from Factory to insert "typename".
      typedef cmg::Factory< cmg::FatJet >::event_ptr event_ptr;
      virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

      void set(std::vector<edm::Ptr<reco::Candidate> > const& input,
               cmg::FatJet& object) const;

      virtual ~FatJetFactory() {};

  private:
      const edm::InputTag collectionLabel_;
      MultiObjectSettingTool const multiObjectFactory_;
      const double Rmax_;
  };
}

#endif /*FATJETFACTORY_H_*/
