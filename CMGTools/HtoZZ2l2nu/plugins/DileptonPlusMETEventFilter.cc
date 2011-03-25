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
class DileptonPlusMETEventFilter : public edm::EDFilter {
public:

  explicit DileptonPlusMETEventFilter(const edm::ParameterSet&);
  ~DileptonPlusMETEventFilter();
  
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
DileptonPlusMETEventFilter::DileptonPlusMETEventFilter(const edm::ParameterSet& iConfig) :
  source_( iConfig.getParameter<edm::InputTag>("source") ),
  requiredPaths_( iConfig.getParameter<std::vector<int> >("selectionPaths") ),
  requiredStep_( iConfig.getParameter<int>("selectionStep") )
{
}

//
DileptonPlusMETEventFilter::~DileptonPlusMETEventFilter()
{
}

//
bool DileptonPlusMETEventFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  try
     {
       using namespace edm;

       //retrieve the selection info
       Handle<std::vector<int> > selectionInfo;
       edm::InputTag selInfoTag(source_.label()+":selectionInfo");
       iEvent.getByLabel(selInfoTag, selectionInfo);
       
       int selectionStep = (*selectionInfo)[1];
       if(selectionStep<requiredStep_) return false;
       
       int selectionPath = (*selectionInfo)[0];
       return (find(requiredPaths_.begin(), requiredPaths_.end(), selectionPath) != requiredPaths_.end());
     }
  catch(std::exception &e){
    return false;
  }
  
  return true;
}

//
void DileptonPlusMETEventFilter::beginJob()
{
}

//
void DileptonPlusMETEventFilter::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(DileptonPlusMETEventFilter);
