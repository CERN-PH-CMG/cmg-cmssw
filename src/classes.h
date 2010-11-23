#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
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

#include <vector>

namespace {
  struct CMGTools_AnalysisDataFormats {

    cmg::AbstractPhysicsObject a_;
    std::vector<cmg::AbstractPhysicsObject> av;
    edm::Wrapper<cmg::AbstractPhysicsObject> ae;
    edm::Wrapper<std::vector<cmg::AbstractPhysicsObject> > aev;

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
    
	
  };
  

}
