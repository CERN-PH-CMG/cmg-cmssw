#include "PhysicsTools/SelectorUtils/interface/CutApplicatorBase.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

class GsfEleRelatHcalIsoCut : public CutApplicatorBase {
public:
  GsfEleRelatHcalIsoCut(const edm::ParameterSet& c) :
    CutApplicatorBase(c),
    _relatHcalisoCutValueEB(c.getParameter<double>("relatHCALisoCutValueEB")),
    _relatHcalisoCutValueEE(c.getParameter<double>("relatHCALisoCutValueEE")),
    _barrelCutOff(c.getParameter<double>("barrelCutOff")) {
  }
  
  result_type operator()(const reco::GsfElectronPtr&) const override final;

  CandidateType candidateType() const override final { 
    return ELECTRON; 
  }

private:
  const double _relatHcalisoCutValueEB,_relatHcalisoCutValueEE,_barrelCutOff;
};

DEFINE_EDM_PLUGIN(CutApplicatorFactory,
		  GsfEleRelatHcalIsoCut,
		  "GsfEleRelatHcalIsoCut");

CutApplicatorBase::result_type 
GsfEleRelatHcalIsoCut::
operator()(const reco::GsfElectronPtr& cand) const{  
  const float relatHcalisoCutValue =
    ( std::abs(cand->superCluster()->position().eta()) < _barrelCutOff ? 
      _relatHcalisoCutValueEB : _relatHcalisoCutValueEE );
  return (cand->dr03HcalTowerSumEt()/cand->et()) < relatHcalisoCutValue;
}
