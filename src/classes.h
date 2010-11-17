#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/CMGTools/interface/DiMuon.h"
#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Generic.h"
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

    cmg::DiMuon d_;
    std::vector<cmg::DiMuon> dv;
    edm::Wrapper<cmg::DiMuon> ed;
    edm::Wrapper<std::vector<cmg::DiMuon> > edv;

    cmg::Electron e_;
    std::vector<cmg::Electron> ev;
    edm::Wrapper<cmg::Electron> ee;
    edm::Wrapper<std::vector<cmg::Electron> > eev;

    cmg::Muon m_;
    std::vector<cmg::Muon> mv;
    edm::Wrapper<cmg::Muon> em;
    edm::Wrapper<std::vector<cmg::Muon> > emv;
    
    cmg::BaseJet j_;
    std::vector<cmg::BaseJet> jv;
    edm::Wrapper<cmg::BaseJet> ej;
    edm::Wrapper<std::vector<cmg::BaseJet> > ejv;

    cmg::Generic o_;
    std::vector< cmg::Generic > ov_;
    edm::Wrapper< cmg::Generic > ow_;
    edm::Ptr< cmg::Generic > op_;
    edm::Wrapper<std::vector< cmg::Generic > > ovw_;
	
    cmg::PFJet pfj_;
    std::vector<cmg::PFJet> pfjv;
    edm::Wrapper<cmg::PFJet> epfj;
    edm::Wrapper<std::vector<cmg::PFJet> > epfjv;

    cmg::PFJetComponent  jetComp_;
    std::vector< cmg::PFJetComponent > vJetComps_;
    
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
	
  };
  

}
