#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/CMGTools/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJetComponent.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2.h"
#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2Fwd.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "AnalysisDataFormats/CMGTools/interface/Photon.h"

#include <vector>

namespace {
  struct CMGTools_AnalysisDataFormats {

    cmg::AbstractPhysicsObject a_;
    std::vector<cmg::AbstractPhysicsObject> av;
    edm::Wrapper<cmg::AbstractPhysicsObject> ae;
    edm::Wrapper<std::vector<cmg::AbstractPhysicsObject> > aev;

    cmg::DiHemisphere dh_;
    std::vector<cmg::DiHemisphere> dhv;
    edm::Wrapper<cmg::DiHemisphere> wdh;
    edm::Wrapper<std::vector<cmg::DiHemisphere> > wdhv;

    cmg::DiPhoton dp_;
    std::vector<cmg::DiPhoton> dpv;
    edm::Wrapper<cmg::DiPhoton> wdp;
    edm::Wrapper<std::vector<cmg::DiPhoton> > wdpv;

    cmg::DiElectron de_;
    std::vector<cmg::DiElectron> dev;
    edm::Wrapper<cmg::DiElectron> ede;
    edm::Wrapper<std::vector<cmg::DiElectron> > edev;

    cmg::DiMuon d_;
    std::vector<cmg::DiMuon> dv;
    edm::Wrapper<cmg::DiMuon> ed;
    edm::Wrapper<std::vector<cmg::DiMuon> > edv;
    
    cmg::DiJet dj_;
    std::vector<cmg::DiJet> djv;
    edm::Wrapper<cmg::DiJet> edj;
    edm::Wrapper<std::vector<cmg::DiJet> > edjv;

    cmg::DiPFJet dpfj_;
    std::vector<cmg::DiPFJet> dpfjv;
    edm::Wrapper<cmg::DiPFJet> epfdj;
    edm::Wrapper<std::vector<cmg::DiPFJet> > epfdjv;
    
    cmg::DiTau dt_;
    std::vector<cmg::DiTau> dtv;
    edm::Wrapper<cmg::DiTau> edt;
    edm::Wrapper<std::vector<cmg::DiTau> > edtv;

    cmg::Electron e_;
    std::vector<cmg::Electron> ev;
    edm::Wrapper<cmg::Electron> ee;
    edm::Wrapper<std::vector<cmg::Electron> > eev;

    cmg::Muon m_;
    std::vector<cmg::Muon> mv;
    edm::Wrapper<cmg::Muon> em;
    edm::Wrapper<std::vector<cmg::Muon> > emv;

    cmg::Tau t;
    std::vector<cmg::Tau> tv;
    edm::Wrapper<cmg::Tau> et;
    edm::Wrapper<std::vector<cmg::Tau> > etv;

    cmg::BaseJet j_;
    std::vector<cmg::BaseJet> jv;
    edm::Wrapper<cmg::BaseJet> ej;
    edm::Wrapper<std::vector<cmg::BaseJet> > ejv;

    cmg::PFJet pfj_;
    std::vector<cmg::PFJet> pfjv;
    edm::Wrapper<cmg::PFJet> epfj;
    edm::Wrapper<std::vector<cmg::PFJet> > epfjv;

    cmg::PFJetComponent  jetComp_;
    std::vector< cmg::PFJetComponent > vJetComps_;
    
    cmg::QuadMuon qm_;
    std::vector<cmg::QuadMuon> qmv;
    edm::Wrapper<cmg::QuadMuon> eqm;
    edm::Wrapper<std::vector<cmg::QuadMuon> > eqmv;
    
    cmg::BaseMET met_;
    std::vector<cmg::BaseMET> metv_;
    edm::Wrapper<cmg::BaseMET> wmet_;
    edm::Wrapper<std::vector<cmg::BaseMET> > wmetv_;

    static const cmg::TriBool u_ = cmg::tbUnset;
    static const cmg::TriBool f_ = cmg::tbFalse;
    static const cmg::TriBool t_ = cmg::tbTrue;
    
    cmg::PFCand pfcand_;
    std::vector<cmg::PFCand> pfcandv_;
    edm::Wrapper<cmg::PFCand> wpfcand_;
    edm::Wrapper<std::vector<cmg::PFCand> > wpfcandv_;
    
    cmg::GenParticle genpart_;
    std::vector<cmg::GenParticle> genpartv_;
    edm::Wrapper<cmg::GenParticle> wgenpart_;
    edm::Wrapper<std::vector<cmg::GenParticle> > wgenpartv_;
    
    cmg::GenJet genjet_;
    std::vector<cmg::GenJet> genjetv_;
    edm::Wrapper<cmg::GenJet> wgenjet_;
    edm::Wrapper<std::vector<cmg::GenJet> > wgenjetv_;
    
    cmg::WENu wen;
    std::vector<cmg::WENu> wenv_;
    edm::Wrapper<cmg::WENu> wwen_;
    edm::Wrapper<std::vector<cmg::WENu> > wwenv_;
    
    cmg::WMuNu wmun;
    std::vector<cmg::WMuNu> wmunv_;
    edm::Wrapper<cmg::WMuNu> wwmun_;
    edm::Wrapper<std::vector<cmg::WMuNu> > wwmunv_;
    
    cmg::WTauNu wtn;
    std::vector<cmg::WTauNu> wtnv_;
    edm::Wrapper<cmg::WTauNu> wwtn_;
    edm::Wrapper<std::vector<cmg::WTauNu> > wwtnv_;

    cmg::MultiObject mp_;
    std::vector<cmg::MultiObject> vmp_;
    edm::Wrapper<cmg::MultiObject> wmp_;
    edm::Wrapper<std::vector<cmg::MultiObject> > wvmp_;

    cmg::Hemisphere hp_;
    std::vector<cmg::Hemisphere> vhp_;
    edm::Wrapper<cmg::Hemisphere> whp_;
    edm::Wrapper<std::vector<cmg::Hemisphere> > wvhp_;

    cmg::Photon pp_;
    std::vector<cmg::Photon> vpp_;
    edm::Wrapper<cmg::Photon> wpp_;
    edm::Wrapper<std::vector<cmg::Photon> > wvpp_;

    /// create dictionaries for Jet Jet objects
    cmg::VBFEventJetJet dummyVBFEventJetJet;
    edm::Wrapper<cmg::VBFEventJetJet> dummyVBFEventJetJetWrapper;
    cmg::VBFEventJetJetCollection dummyVBFEventJetJetCollection;
    edm::Wrapper<cmg::VBFEventJetJetCollection> dummyVBFEventJetJetCollectionWrapper;

    /// create dictionaries for Jet PFCandidate objects
    cmg::VBFEventJetJet dummyVBFEventJetPFCand;
    edm::Wrapper<cmg::VBFEventJetPFCand> dummyVBFEventJetPFCandWrapper;
    cmg::VBFEventJetPFCandCollection dummyVBFEventJetPFCandCollection;
    edm::Wrapper<cmg::VBFEventJetPFCandCollection> dummyVBFEventJetPFCandCollectionWrapper;

    /// create dictionaries for Jet PFParticle objects
    cmg::VBFEventJetJet dummyVBFEventJetPFPart;
    edm::Wrapper<cmg::VBFEventJetPFPart> dummyVBFEventJetPFPartWrapper;
    cmg::VBFEventJetPFPartCollection dummyVBFEventJetPFPartCollection;
    edm::Wrapper<cmg::VBFEventJetPFPartCollection> dummyVBFEventJetPFPartCollectionWrapper;

    /// create dictionaries for PFJet PFJet objects
    cmg::VBFEventPFJetPFJet dummyVBFEventPFJetPFJet;
    edm::Wrapper<cmg::VBFEventPFJetPFJet> dummyVBFEventPFJetPFJetWrapper;
    cmg::VBFEventPFJetPFJetCollection dummyVBFEventPFJetPFJetCollection;
    edm::Wrapper<cmg::VBFEventPFJetPFJetCollection> dummyVBFEventPFJetPFJetCollectionWrapper;

    /// create dictionaries for PFJet PFCandidate objects
    cmg::VBFEventPFJetPFJet dummyVBFEventPFJetPFCand;
    edm::Wrapper<cmg::VBFEventPFJetPFCand> dummyVBFEventPFJetPFCandWrapper;
    cmg::VBFEventPFJetPFCandCollection dummyVBFEventPFJetPFCandCollection;
    edm::Wrapper<cmg::VBFEventPFJetPFCandCollection> dummyVBFEventPFJetPFCandCollectionWrapper;

    /// create dictionaries for GenJet GenJet objects
    cmg::VBFEventGenJetGenJet dummyVBFEventGenJetGenJet;
    edm::Wrapper<cmg::VBFEventGenJetGenJet> dummyVBFEventGenJetGenJetWrapper;
    cmg::VBFEventGenJetGenJetCollection dummyVBFEventGenJetGenJetCollection;
    edm::Wrapper<cmg::VBFEventGenJetGenJetCollection> dummyVBFEventGenJetGenJetCollectionWrapper;

    /// used by the VBFEvents
    edm::PtrVector<reco::GenJet> dummyGenJetPtrVector;
    edm::PtrVector<pat::Jet> dummyPATJetPtrVector;
    edm::PtrVector<reco::PFCandidate> dummyPFCandPtrVector;
    edm::PtrVector<pat::PFParticle> dummyPFPartPtrVector;
    
    /// create dictionaries for the composite objects from PFAnalysis
    PATMuPair dummyPATMuPair_;
    std::vector<PATMuPair> dummyPATMuPairVector_;
    edm::Wrapper<PATMuPair> dummyPATMuPairWrapper_;
    edm::Wrapper<std::vector<PATMuPair> > dummyPATMuPairVectorWrapper_;
    
    PATMuonNuPair dummyPATMuNuPair_;
    std::vector<PATMuonNuPair> dummyPATMuNuPairVector_;
    edm::Wrapper<PATMuonNuPair> dummyPATMuNuPairWrapper_;
    edm::Wrapper<std::vector<PATMuonNuPair> > dummyPATMuNuPairVectorWrapper_;
    
  };
  

}
