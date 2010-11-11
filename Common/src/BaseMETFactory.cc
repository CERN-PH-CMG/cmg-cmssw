#include "CMGTools/Common/interface/BaseMETFactory.h"

#include <iostream>

cmg::BaseMETFactory::event_ptr cmg::BaseMETFactory::create(const edm::Event& iEvent, 
							   const edm::EventSetup&) const{
  // here read a view 
  typedef edm::View<reco::Candidate> ViewType; 
  edm::Handle< ViewType > inputs;
  iEvent.getByLabel(inputLabel_,inputs);
  
  cmg::BaseMETFactory::event_ptr result(new cmg::BaseMETFactory::collection);

  double sumEx = 0;
  double sumEy = 0;
  double sumEt = 0;

  for(ViewType::const_iterator mi = inputs->begin();
      mi != inputs->end(); ++mi) {

    const reco::Candidate& cand = *mi;

    
    double phi = cand.phi();
    double cosphi = cos(phi);
    double sinphi = sin(phi);

    double theta = cand.theta();
    double sintheta = sin(theta);
    
    double E = cand.energy();

    double et = E*sintheta;
    double ex = et*cosphi;
    double ey = et*sinphi;
    
    sumEx += ex;
    sumEy += ey;
    sumEt += et;
  		     
  }

  double Et = sqrt( sumEx*sumEx + sumEy*sumEy);
  math::XYZTLorentzVector missingEt( -sumEx, -sumEy, 0, Et);

  // now turning it into a PtEtaPhiLorentzVector to make a physics object
  // that's suboptimal...
  reco::Particle::PolarLorentzVector metLV(missingEt);
  
  cmg::BaseMET met( metLV, sumEt );
  result->push_back(met);

  return result;
}
