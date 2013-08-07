#include "CMGTools/Susy/plugins/ElectronIsolationProducer.h"

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"

#include <cmath>
#include <memory>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

typedef std::vector<cmg::Electron> collection;

ElectronIsolationProducer::ElectronIsolationProducer(const edm::ParameterSet& iConfig):
  electronTag_(iConfig.getParameter<edm::InputTag>("electronTag")),
  rhoTag_(iConfig.getParameter<edm::InputTag>("rhoTag")),
  isoCut_(iConfig.getParameter<double>("isoCut")),
  coneSize_(iConfig.getParameter<double>("coneSize")){
  produces<collection>();
}
    
void ElectronIsolationProducer::produce(edm::Event& iEvent, const edm::EventSetup&){
  
  std::auto_ptr<collection> result(new collection);

  edm::Handle<collection> eleHandle;
  iEvent.getByLabel(electronTag_, eleHandle);

  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(rhoTag_, rhoHandle);
  const double rho = *rhoHandle;

  //these are from https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection
  //see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2672.htm for syntax
  const std::vector<double> eta = {0., 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 1000};
  const std::vector<double> area = {0.1, 0.12, 0.085, 0.11, 0.12, 0.12, 0.13};

  for(collection::const_iterator it = eleHandle->begin(); it != eleHandle->end(); ++it){
    const double e = fabs(it->eta());
    
    //find the effective area to use
    double a = 0;
    for(unsigned int i = 0; i < eta.size(); i++){
      if( (e >= eta.at(i) ) && (e < eta.at(i+1) ) ){
	a = area.at(i);
	break;
      } 
    }
    
    //see https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection
    const double dr = coneSize_;
    const double absIso = it->chargedHadronIso(dr) + std::max(0., ( it->neutralHadronIso(dr)+it->photonIso(dr) ) - (a*rho) );

    if( (fabs(absIso)/it->pt()) < isoCut_){
      result->push_back(*it);
    }

  }
  iEvent.put(result);

}

DEFINE_FWK_MODULE(ElectronIsolationProducer);

