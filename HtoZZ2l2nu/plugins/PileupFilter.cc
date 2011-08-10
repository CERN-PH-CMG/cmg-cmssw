// system include files
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
class PileupFilter : public edm::EDFilter {
public:

  explicit PileupFilter(const edm::ParameterSet&);
  ~PileupFilter();
  
private:

  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag source_;
  std::vector<int> requiredPaths_;
  int requiredStep_;
};


using namespace std;

//
PileupFilter::PileupFilter(const edm::ParameterSet& iConfig) :
  source_( iConfig.getParameter<edm::InputTag>("source") )
{
}

//
PileupFilter::~PileupFilter()
{
}

//
bool PileupFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  try
     {
       using namespace edm;

       //retrieve the selection info
       edm::Handle<float> normPuWeightHandle;
       iEvent.getByLabel(source_, normPuWeightHandle );
       float wgt = *normPuWeightHandle;
       if(wgt!=1.0) return false;
     }
  catch(std::exception &e){
    cout << e.what() << endl;
    return false;
  }
  
  return true;
}

//
void PileupFilter::beginJob()
{
}

//
void PileupFilter::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(PileupFilter);
