#include "PhysicsTools/SelectorUtils/interface/CutApplicatorBase.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

class GsfEleRelatTkIsoCut : public CutApplicatorBase {
public:
  GsfEleRelatTkIsoCut(const edm::ParameterSet& c) :
    CutApplicatorBase(c),
    _relatTkisoCutValueEB(c.getParameter<double>("relatTKisoCutValueEB")),
    _relatTkisoCutValueEE(c.getParameter<double>("relatTKisoCutValueEE")),
    _barrelCutOff(c.getParameter<double>("barrelCutOff")) {
  }
  
  result_type operator()(const reco::GsfElectronPtr&) const override final;

  CandidateType candidateType() const override final { 
    return ELECTRON; 
  }

private:
  const double _relatTkisoCutValueEB,_relatTkisoCutValueEE,_barrelCutOff;
};

DEFINE_EDM_PLUGIN(CutApplicatorFactory,
		  GsfEleRelatTkIsoCut,
		  "GsfEleRelatTkIsoCut");

CutApplicatorBase::result_type 
GsfEleRelatTkIsoCut::
operator()(const reco::GsfElectronPtr& cand) const{  
  const float relatTkisoCutValue =
    ( std::abs(cand->superCluster()->position().eta()) < _barrelCutOff ? 
      _relatTkisoCutValueEB : _relatTkisoCutValueEE );
  return (cand->dr03TkSumPt()/cand->et()) < relatTkisoCutValue;
}
