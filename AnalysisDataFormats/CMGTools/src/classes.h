#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/CMGTools/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"
#include "AnalysisDataFormats/CMGTools/interface/FatJet.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/StructuredPFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJetComponent.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "AnalysisDataFormats/CMGTools/interface/Photon.h"
#include "AnalysisDataFormats/CMGTools/interface/NamedObject.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include <vector>

#include <TMatrixD.h>

namespace {
  struct CMGTools_AnalysisDataFormats {

    cmg::METSignificance metsig_;
    edm::Wrapper<cmg::METSignificance> metsige_;
    std::vector<cmg::METSignificance> metsigv_;
    edm::Wrapper<std::vector<cmg::METSignificance> > metsigve_;

    TMatrixD  matrix_;
    edm::Wrapper<TMatrixD> matrixe_;

    cmg::AbstractPhysicsObject a_;
    std::vector<cmg::AbstractPhysicsObject> av;
    edm::Wrapper<cmg::AbstractPhysicsObject> ae;
    edm::Wrapper<std::vector<cmg::AbstractPhysicsObject> > aev;
    
    cmg::NamedObject no_;
    std::vector<cmg::NamedObject> nov;
    edm::Wrapper<cmg::NamedObject> noe;
    edm::Wrapper<std::vector<cmg::NamedObject> > nowv;

    cmg::DiPFCandidate dpfc_;
    std::vector<cmg::DiPFCandidate> dpfcv;
    edm::Wrapper<cmg::DiPFCandidate> wdpfc;
    edm::Wrapper<std::vector<cmg::DiPFCandidate> > wdpfcv;

    cmg::DiHemisphere dh_;
    std::vector<cmg::DiHemisphere> dhv;
    edm::Wrapper<cmg::DiHemisphere> wdh;
    edm::Wrapper<std::vector<cmg::DiHemisphere> > wdhv;

    cmg::DiFatJet dmp_;
    std::vector<cmg::DiFatJet> vdmp_;
    edm::Wrapper<cmg::DiFatJet> wdmp_;
    edm::Wrapper<std::vector<cmg::DiFatJet> > wdvmp_;

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
    edm::Ptr< cmg::Muon > mp;
    std::vector<cmg::Muon> mv;
    edm::Wrapper<cmg::Muon> em;
    edm::Wrapper<std::vector<cmg::Muon> > emv;

    cmg::SimpleParticle sp;
    std::vector<cmg::SimpleParticle> spv;    
    edm::Wrapper<cmg::SimpleParticle> wsp;
    edm::Wrapper<std::vector<cmg::SimpleParticle> > wvsp;

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
    boost::array<int,10> intarray5;
    boost::array<float,10> floatarray5;
    boost::array<std::string,10> stringarray5;

    cmg::StructuredPFJet spfj_;
    std::vector<cmg::StructuredPFJet> spfjv;
    edm::Wrapper<cmg::StructuredPFJet> espfj;
    edm::Wrapper<std::vector<cmg::StructuredPFJet> > espfjv;

    cmg::PFJetComponent  jetComp_;
    std::vector< cmg::PFJetComponent > vJetComps_;



    cmg::QuadMuon qm_;
    std::vector<cmg::QuadMuon> qmv;
    edm::Wrapper<cmg::QuadMuon> eqm;
    edm::Wrapper<std::vector<cmg::QuadMuon> > eqmv;

    cmg::QuadElectron qe_;
    std::vector<cmg::QuadElectron> qev;
    edm::Wrapper<cmg::QuadElectron> eqe;
    edm::Wrapper<std::vector<cmg::QuadElectron> > eqev;

    cmg::DiMuonDiElectron qme_;
    std::vector<cmg::DiMuonDiElectron> qmev;
    edm::Wrapper<cmg::DiMuonDiElectron> eqme;
    edm::Wrapper<std::vector<cmg::DiMuonDiElectron> > eqmev;

    cmg::TriMuonMET tm_;
    std::vector<cmg::TriMuonMET> vtm;
    edm::Wrapper<cmg::TriMuonMET> etm;
    edm::Wrapper<std::vector<cmg::TriMuonMET> > tmvw;


    cmg::BaseMET met_;
    std::vector<cmg::BaseMET> metv_;
    edm::Wrapper<cmg::BaseMET> wmet_;
    edm::Wrapper<std::vector<cmg::BaseMET> > wmetv_;

    static const cmg::TriBool u_ = cmg::tbUnset;
    static const cmg::TriBool f_ = cmg::tbFalse;
    static const cmg::TriBool t_ = cmg::tbTrue;

    cmg::PFCandidate pfcand_;
    std::vector<cmg::PFCandidate> pfcandv_;
    edm::Wrapper<cmg::PFCandidate> wpfcand_;
    edm::Wrapper<std::vector<cmg::PFCandidate> > wpfcandv_;

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

    cmg::FatJet fj_;
    std::vector<cmg::FatJet> vfj_;
    edm::Wrapper<cmg::FatJet> wfj_;
    edm::Wrapper<std::vector<cmg::FatJet> > wvfj_;

    cmg::Hemisphere hp_;
    std::vector<cmg::Hemisphere> vhp_;
    edm::Wrapper<cmg::Hemisphere> whp_;
    edm::Wrapper<std::vector<cmg::Hemisphere> > wvhp_;

    cmg::Photon pp_;
    std::vector<cmg::Photon> vpp_;
    edm::Wrapper<cmg::Photon> wpp_;
    edm::Wrapper<std::vector<cmg::Photon> > wvpp_;

    cmg::TriggerObject topp_;
    cmg::TriggerObject::prescaleMap topp_map_;
    std::vector<cmg::TriggerObject> vtopp_;
    edm::Wrapper<cmg::TriggerObject> wtopp_;
    edm::Ptr<pat::TriggerObject> ptopp_;
    edm::Ptr<cmg::TriggerObject> pptopp_;
    edm::Wrapper<std::vector<cmg::TriggerObject> > wvtopp_;
        
    /// create dictionaries for the composite objects from PFAnalysis
    PATMuPair dummyPATMuPair_;
    std::vector<PATMuPair> dummyPATMuPairVector_;
    edm::Wrapper<PATMuPair> dummyPATMuPairWrapper_;
    edm::Wrapper<std::vector<PATMuPair> > dummyPATMuPairVectorWrapper_;
    
    PATMuonNuPair dummyPATMuNuPair_;
    std::vector<PATMuonNuPair> dummyPATMuNuPairVector_;
    edm::Wrapper<PATMuonNuPair> dummyPATMuNuPairWrapper_;
    edm::Wrapper<std::vector<PATMuonNuPair> > dummyPATMuNuPairVectorWrapper_;
    

    //for Tau-Tau
    cmg::TauEle taue;
    std::vector<cmg::TauEle> taue_;
    edm::Wrapper<cmg::TauEle> wtaue_;
    edm::Wrapper<std::vector<cmg::TauEle> > wtauev_;

    cmg::TauMu taumu;
    std::vector<cmg::TauMu> taumu_;
    edm::Wrapper<cmg::TauMu> wtaumu_;
    edm::Wrapper<std::vector<cmg::TauMu> > wtaumuv_;

    cmg::MuEle mue;
    std::vector<cmg::MuEle> mue_;
    edm::Wrapper<cmg::MuEle> wmue_;
    edm::Wrapper<std::vector<cmg::MuEle> > wmuev_;

    // Correspondence between PFCandidates and Primary Vertices
    std::vector<reco::Vertex> vtxvec_;
    std::vector<edm::Ref<std::vector<reco::Vertex>,reco::Vertex,edm::refhelper::FindUsingAdvance<std::vector<reco::Vertex>,reco::Vertex> > > vtxrefvec_;
    edm::ValueMap<edm::Ref<std::vector<reco::Vertex>,reco::Vertex,edm::refhelper::FindUsingAdvance<std::vector<reco::Vertex>,reco::Vertex> > > vtxvmap_;
    edm::Wrapper<edm::ValueMap<edm::Ref<std::vector<reco::Vertex>,reco::Vertex,edm::refhelper::FindUsingAdvance<std::vector<reco::Vertex>,reco::Vertex> > > > vtxvmapw_;
    reco::VertexRefVector vtxrefvec2_;
    edm::ValueMap<reco::VertexRefVector> vtxvmap2_;
    edm::Wrapper<edm::ValueMap<reco::VertexRefVector> > vtxvmapw2_;

  };
  

}
