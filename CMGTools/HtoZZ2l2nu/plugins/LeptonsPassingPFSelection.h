#ifndef LeptonsPassingPFSelection_H
#define LeptonsPassingPFSelection_H

/** \class LeptonsPassingPFSelection
 *  No description available.
 *
 *  $Date: $
 *  $Revision: $
 *  \author G. Cerminara - CERN
 */


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

using namespace pat;

class LeptonsPassingPFSelection : public edm::EDProducer {
public:
  /// Constructor
  LeptonsPassingPFSelection(const edm::ParameterSet &iConfig);

  /// Destructor
  virtual ~LeptonsPassingPFSelection();

  // Operations
  /// method to be called at each event
  virtual void produce(edm::Event &iEvent, const edm::EventSetup &iSetup);

protected:

private:
  /// Input collection of muons and of partice flow
  edm::InputTag muons_;
  edm::InputTag pf_;

  StringCutObjectSelector<pat::Muon> *pfCut_;

};
#endif
