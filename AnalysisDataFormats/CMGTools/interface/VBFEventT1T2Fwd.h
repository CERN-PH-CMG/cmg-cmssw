#ifndef __AnalysisDataFormats_VBFEventT1T2Fwd_h__
#define __AnalysisDataFormats_VBFEventT1T2Fwd_h__

// #include "AnalysisDataFormats/PFAnalyses/interface/VBFEventT1T2.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PFParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"


#include <vector>


namespace cmg {

typedef VBFEventT1T2<pat::Jet, pat::Jet> VBFEventJetJet;
typedef VBFEventT1T2<pat::Jet, reco::PFCandidate> VBFEventJetPFCand;
typedef VBFEventT1T2<pat::Jet, pat::PFParticle> VBFEventJetPFPart;
typedef VBFEventT1T2<reco::PFJet, reco::PFJet> VBFEventPFJetPFJet;
typedef VBFEventT1T2<reco::PFJet, reco::PFCandidate> VBFEventPFJetPFCand;
typedef VBFEventT1T2<reco::GenJet, reco::GenJet> VBFEventGenJetGenJet;


/// collection of VBFEventJetJet objects
typedef std::vector<VBFEventJetJet> VBFEventJetJetCollection;
typedef std::vector<VBFEventJetPFCand> VBFEventJetPFCandCollection;
typedef std::vector<VBFEventJetPFPart> VBFEventJetPFPartCollection;
typedef std::vector<VBFEventPFJetPFJet> VBFEventPFJetPFJetCollection;
typedef std::vector<VBFEventPFJetPFCand> VBFEventPFJetPFCandCollection;
typedef std::vector<VBFEventGenJetGenJet> VBFEventGenJetGenJetCollection;

/// persistent reference to a VBFEventJetJet object
typedef edm::Ref<VBFEventJetJetCollection> VBFEventJetJetRef;
typedef edm::Ref<VBFEventJetPFCandCollection> VBFEventJetPFCandRef;
typedef edm::Ref<VBFEventJetPFPartCollection> VBFEventJetPFPartRef;
typedef edm::Ref<VBFEventPFJetPFJetCollection> VBFEventPFJetPFJetRef;
typedef edm::Ref<VBFEventPFJetPFCandCollection> VBFEventPFJetPFCandRef;
typedef edm::Ref<VBFEventGenJetGenJetCollection> VBFEventGenJetGenJetRef;


/// references to VBFEventJetJet collection
typedef edm::RefProd<VBFEventJetJetCollection> VBFEventJetJetRefProd;
typedef edm::RefProd<VBFEventJetPFCandCollection> VBFEventJetPFCandRefProd;
typedef edm::RefProd<VBFEventJetPFPartCollection> VBFEventJetPFPartRefProd;
typedef edm::RefProd<VBFEventPFJetPFJetCollection> VBFEventPFJetPFJetRefProd;
typedef edm::RefProd<VBFEventPFJetPFCandCollection> VBFEventPFJetPFCandRefProd;
typedef edm::RefProd<VBFEventGenJetGenJetCollection> VBFEventGenJetGenJetRefProd;


/// vector of references to VBFEventJetJet objects all in the same collection
typedef edm::RefVector<VBFEventJetJetCollection> VBFEventJetJetRefVector;
typedef edm::RefVector<VBFEventJetPFCandCollection> VBFEventJetPFCandRefVector;
typedef edm::RefVector<VBFEventJetPFPartCollection> VBFEventJetPFPartRefVector;
typedef edm::RefVector<VBFEventPFJetPFJetCollection> VBFEventPFJetPFJetRefVector;
typedef edm::RefVector<VBFEventPFJetPFCandCollection> VBFEventPFJetPFCandRefVector;
typedef edm::RefVector<VBFEventGenJetGenJetCollection> VBFEventGenJetGenJetRefVector;

}

#endif
 
