// system include files
#include <memory>
#include <sstream>
#include <iostream>
#include <string>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Version/interface/GetReleaseVersion.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class Passed : public edm::EDFilter {
   public:
      explicit Passed(const edm::ParameterSet&);
      ~Passed();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      
      int passed_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
Passed::Passed(const edm::ParameterSet& iConfig)
  : passed_ (iConfig.getUntrackedParameter<int>("passed",1))
{
  produces<int>();
}


Passed::~Passed()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
Passed::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   iEvent.put( std::auto_ptr<int>(new int(passed_)) );
   return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
Passed::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Passed::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool 
Passed::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true; // why is this necessary?
}

// ------------ method called when ending the processing of a run  ------------
bool 
Passed::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
Passed::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
Passed::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

//define this as a plug-in
DEFINE_FWK_MODULE(Passed);
