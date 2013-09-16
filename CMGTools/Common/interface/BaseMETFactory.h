#ifndef _CMGTools_CommonTools_BaseMetFactory_H_
#define _CMGTools_CommonTools_BaseMetFactory_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "CMGTools/Common/interface/Factory.h"

#include <iostream>
#include <memory>

namespace cmg{

template <class T = reco::Candidate>
class BaseMETFactory : public Factory<cmg::BaseMET>{
  public:
    BaseMETFactory(const edm::ParameterSet& ps):
      inputLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      ptThreshold_(ps.getParameter<double>("ptThreshold"))
      {
    }
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    ///utility method for met calculation
    static void calcET(const reco::Candidate& cand, double* ex, double* ey, double* et);
    
  private:
    const edm::InputTag inputLabel_;
    double ptThreshold_;
  };

///Include a template specialization so that we can also convert the pat::MET - in this case ptThreshold is ignored 
template <>
BaseMETFactory<pat::MET>::event_ptr BaseMETFactory<pat::MET>::create(const edm::Event& iEvent, const edm::EventSetup&){
  // here read a view 
  typedef edm::View<pat::MET> ViewType; 
  edm::Handle< ViewType > inputs;
  iEvent.getByLabel(inputLabel_,inputs);
  
  BaseMETFactory<pat::MET>::event_ptr result(new BaseMETFactory<pat::MET>::collection);
  for(ViewType::const_iterator met = inputs->begin(); met != inputs->end(); ++met) {

    const pat::MET& cand = *met;
    cmg::BaseMET m(cand);
    m.sumEt_ = cand.sumEt();
    if ( cand.genMET() )
      m.genMET_ = cmg::BaseMET::GenMET( cand.genMET()->p4() );
    result->push_back(m);
  }

  return result;
 }
  

template <>
BaseMETFactory<reco::PFMET>::event_ptr BaseMETFactory<reco::PFMET>::create(const edm::Event& iEvent, const edm::EventSetup&){
  // here read a view 
  typedef edm::View<reco::PFMET> ViewType; 
  edm::Handle< ViewType > inputs;
  iEvent.getByLabel(inputLabel_,inputs);
  
  BaseMETFactory<reco::PFMET>::event_ptr result(new BaseMETFactory<reco::PFMET>::collection);
  for(ViewType::const_iterator met = inputs->begin(); met != inputs->end(); ++met) {

    const reco::PFMET& cand = *met;
    cmg::BaseMET m(cand);
    m.sumEt_ = cand.sumEt();
    result->push_back(m);
  }

  return result;
 }
  



template <class T>
void cmg::BaseMETFactory<T>::calcET(const reco::Candidate& cand, double* ex, double* ey, double* et){
    
    double phi = cand.phi();
    double cosphi = cos(phi);
    double sinphi = sin(phi);

    double theta = cand.theta();
    double sintheta = sin(theta);
    
    double E = cand.energy();

    *et = E*sintheta;
    *ex = (*et)*cosphi;
    *ey = (*et)*sinphi;
    
 }


///Recalculate the MET using any collection of candidates
template <class T>
typename cmg::BaseMETFactory<T>::event_ptr cmg::BaseMETFactory<T>::create(const edm::Event& iEvent, const edm::EventSetup&){
  // here read a view 
  typedef typename edm::View<T> ViewType; 
  edm::Handle< ViewType > inputs;
  iEvent.getByLabel(inputLabel_,inputs);
  
  typename cmg::BaseMETFactory<T>::event_ptr result(new typename cmg::BaseMETFactory<T>::collection);

  double sumEx = 0;
  double sumEy = 0;
  double sumEt = 0;
  int sumCharge = 0;  

  for(typename ViewType::const_iterator mi = inputs->begin();
      mi != inputs->end(); ++mi) {

    const reco::Candidate& cand = *mi;

    if(cand.pt()<ptThreshold_ ) continue;

    double et = 0;
    double ex = 0;
    double ey = 0;
    cmg::BaseMETFactory<T>::calcET(cand, &ex, &ey, &et);
    
    sumEx += ex;
    sumEy += ey;
    sumEt += et;
    sumCharge += cand.charge();
             
  }

  double Et = sqrt( sumEx*sumEx + sumEy*sumEy);
  math::XYZTLorentzVector missingEt( -sumEx, -sumEy, 0, Et);

  cmg::BaseMET met(reco::LeafCandidate(sumCharge,missingEt));
  met.sumEt_ = sumEt;
  result->push_back(met);

  return result;
 }
}

#endif /*_CMGTools_CommonTools_BaseMetFactory_H_*/
