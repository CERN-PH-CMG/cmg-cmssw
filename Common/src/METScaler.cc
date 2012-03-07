#include "CMGTools/Common/interface/METScaler.h"

#include "TVector3.h"
#include <cmath>

cmg::METScaler::METScaler(const edm::ParameterSet& ps):
  jetLabel_(ps.getParameter<edm::InputTag>("jetLabel")),
  metLabel_(ps.getParameter<edm::InputTag>("metLabel")),
  jecUncDirection_(ps.getParameter<double>("jecUncDirection")){
}

cmg::METScaler::event_ptr cmg::METScaler::create(const edm::Event& iEvent, const edm::EventSetup&){

  typedef edm::View<cmg::BaseJet> jet_view;
  edm::Handle<jet_view> jetH;
  iEvent.getByLabel(jetLabel_,jetH);

  edm::Handle<collection> metH;
  iEvent.getByLabel(metLabel_,metH);

  cmg::BaseMET modified(metH->at(0));
  cmg::BaseMET::LorentzVector met(modified.p4());

  //the MET vector to be modified
  TVector3 scaled;
  scaled.SetPtEtaPhi(met.pt(),met.eta(),met.phi());

  //loop over the jets and adjust the MET to compensate
  TVector3 met_i;
  for(jet_view::const_iterator it = jetH->begin(); it != jetH->end(); ++it){
    const float unc = it->uncOnFourVectorScale();
    //adjust the vector by the pt scale from the JEC
    met_i.SetPtEtaPhi(fabs(unc),0.0,it->phi());
    //do we add or substract (caution!)
    met_i *= jecUncDirection_;
    scaled += met_i;
  }
  met.SetPx(scaled.Px());
  met.SetPy(scaled.Py());
  met.SetPz(scaled.Pz());

  //std::cout << "Original: " << modified.px() << "," << modified.py() << "," << modified.pt() << std::endl;
  cmg::METScaler::event_ptr result(new collection);
  modified.setP4(met);
  //std::cout << "Scaled: " << modified.px() << "," << modified.py() << "," << modified.pt() << std::endl;

  result->push_back(modified);
  return result;

}
