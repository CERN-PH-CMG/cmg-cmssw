#include "CMGTools/Common/interface/METScaler.h"

#include "TVector3.h"
#include <cmath>

cmg::METScaler::METScaler(const edm::ParameterSet& ps):
  jetLabel_(ps.getParameter<edm::InputTag>("jetLabel")),
  metLabel_(ps.getParameter<edm::InputTag>("metLabel")),
  jecUncDirection_(ps.getParameter<double>("jecUncDirection")),
  doType1_(ps.getParameter<bool>("doType1Correction")){
}

cmg::METScaler::event_ptr cmg::METScaler::create(const edm::Event& iEvent, const edm::EventSetup&){

  typedef edm::View<cmg::AbstractPhysicsObject> jet_view;
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
    //scale the MET by the JES uncertainty, either plus or minus
    if(!doType1_){
      const float unc = it->uncOnFourVectorScale();
      //adjust the vector by the pt scale from the JEC
      //std::cout << "Jet (JES): " << it->pt() << ", " << it->eta() << ", " << (1 + (jecUncDirection_*fabs(unc)) ) << std::endl;
      met_i.SetPtEtaPhi((it->pt()*(1 + fabs(unc) )) - it->pt(), 0.0, it->phi());
      met_i *= -jecUncDirection_;
    }else{
      //std::cout << "Jet (rawFactor): " << it->pt() << ", " << it->eta() << ", " << it->rawFactor() << std::endl;
      //or scale the MET by the raw factor
      met_i.SetPtEtaPhi(it->pt() - (it->pt()*it->rawFactor()), 0.0, it->phi());
    }
    scaled += met_i;
  }
  met.SetPx(scaled.Px());
  met.SetPy(scaled.Py());
  met.SetPz(scaled.Pz());

  //std::cout << "METScaler: " << jecUncDirection_ << "," << doType1_ << std::endl;
  //std::cout << "Original: " << modified.px() << "," << modified.py() << "," << modified.pt() << std::endl;
  cmg::METScaler::event_ptr result(new collection);
  modified.setP4(met);
  //std::cout << "Scaled: " << modified.px() << "," << modified.py() << "," << modified.pt() << std::endl;

  result->push_back(modified);
  return result;

}
