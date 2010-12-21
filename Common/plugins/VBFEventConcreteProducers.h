#ifndef __CMGTools_Common_VBFEventConcreteProducers__
#define __CMGTools_Common_VBFEventConcreteProducers__
/** 
 *
 * \authors Colin Bernet,
 *          Artur Kalinowski
 *
 *
 */

#include "CMGTools/Common/interface/VBFEventT1T2Producer.h"
#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PFParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "FWCore/Framework/interface/MakerMacros.h"


typedef VBFEventT1T2Producer<pat::Jet, pat::Jet> VBFEventJetJetProducer;
typedef VBFEventT1T2Producer<pat::Jet, reco::PFCandidate> VBFEventJetPFCandProducer;
typedef VBFEventT1T2Producer<pat::Jet, pat::PFParticle> VBFEventJetPFPartProducer;
typedef VBFEventT1T2Producer<reco::PFJet, reco::PFJet> VBFEventPFJetPFJetProducer;
typedef VBFEventT1T2Producer<reco::PFJet, reco::PFCandidate> VBFEventPFJetPFCandProducer;
typedef VBFEventT1T2Producer<reco::GenJet, reco::GenJet> VBFEventGenJetGenJetProducer;


#endif
