#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/FileBlock.h"

#include<TROOT.h>



using namespace std;

cmg::RunInfoAccountingAnalyzer::RunInfoAccountingAnalyzer(const edm::ParameterSet& ps) : 
  name_( ps.getUntrackedParameter<std::string>("name","RunInfoAccounting")),
  runInfoAccounting_( *fs_, name_ ), runNumber_(-1), 
  fileChanged_(false), currentRun_(0), nTotal_(0), nPassed_(0), nGenTotal_(0), nGenPassed_(0),
  genFilterInfoSrc_(ps.getParameter<edm::InputTag>("genFilterInfoSrc") ),
  filterInfoSrc_(ps.getParameter<edm::InputTag>("filterInfoSrc") ),
  runOnMC_(ps.getParameter<bool>("runOnMC") )
{}

void cmg::RunInfoAccountingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    
 //  if( fileChanged_ ) {

//     cout<<"file changed"<<endl;

//     runNumber_ = iEvent.run();
//     currentRun_ = &(iEvent.getRun());
//     cout<<"RunInfoAccountingAnalyzer: new file, run="<<runNumber_<<endl;
//     runInfoAccounting_.processRunInfo( iEvent.getRun() );
//     fileChanged_ = false;
//   }
}
    
void cmg::RunInfoAccountingAnalyzer::respondToOpenInputFile(edm::FileBlock const& fb) {
//   cout<<"opening file"<<endl;

//   TTree *runTree = fb.runTree();

//   runTree->Print();

//   edm::Run* run; 
//   runTree->SetBranchAddress( "Runs", &run );
//   runTree->GetEntry(0);
  
//   cout<<runTree->GetEntries()<<endl;

//   fileChanged_ = true;

}

void cmg::RunInfoAccountingAnalyzer::respondToCloseInputFile(edm::FileBlock const& fb) {
}

void cmg::RunInfoAccountingAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const& lumi, 
							  edm::EventSetup const&) {
}

void cmg::RunInfoAccountingAnalyzer::endLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const&) {
 
  // At each lumi block, incrementing these counters with the number of events
  // tried and passed at this lumi.
  // also see the endRun function. 

  edm::Handle<GenFilterInfo> filter;
  iLumi.getByLabel(filterInfoSrc_, filter);

  std::cout<<"Preselection Filter Efficiency: "<<std::endl;
  printFilterInfo(iLumi, *filter);

  nTotal_ += filter->numEventsTried();
  nPassed_ += filter->numEventsPassed(); 

  // getting the number of events being generated, and passing the filter after generation. 
  // this numbers will be used to derive the generator filtering efficiency

  edm::Handle<GenFilterInfo> genFilter;
  iLumi.getByLabel(genFilterInfoSrc_, genFilter);

  if( genFilter.failedToGet() ) {
    if( runOnMC_ ) {
      std::cout<<"Failed to get GenFilterInfo for filtering after generation."<<std::endl;
      std::cout<<"you're probably reading an old sample or a relval. You will need handle the generator filter efficiency by hand when normalizing your MC sample to the data."<<std::endl;
    }
    nGenTotal_ += 0;
    nGenPassed_ += 0; 
  }
  else {
    nGenTotal_ += genFilter->numEventsTried();
    nGenPassed_ += genFilter->numEventsPassed(); 

    std::cout<<"Gen Filter Efficiency: "<<std::endl;
    printFilterInfo(iLumi, *genFilter);
  }
}

void cmg::RunInfoAccountingAnalyzer::printFilterInfo(const edm::LuminosityBlock& iLumi, const GenFilterInfo& filter) {
  
  std::cout <<"Lumi section " << iLumi.id() << std::endl;
  std::cout <<"N total = " << filter.numEventsTried() << " N passed = " << filter.numEventsPassed() << std::endl;
  std::cout <<"Filter efficiency = " << filter.filterEfficiency() << " +- " << filter.filterEfficiencyError() << std::endl;  
}

void cmg::RunInfoAccountingAnalyzer::beginRun(edm::Run const& run, edm::EventSetup const&) {
}

void cmg::RunInfoAccountingAnalyzer::endRun(edm::Run const& run, edm::EventSetup const&) {
//   fileChanged_ = true;

  //COLIN need to find out how to get the run number from here
  runNumber_ = 1;
  currentRun_ = &run;
  cout<<"RunInfoAccountingAnalyzer: end of run, run="<<runNumber_<<endl;
  runInfoAccounting_.processRunInfo( run, nTotal_, nPassed_, nGenTotal_, nGenPassed_);
//   fileChanged_ = false;
}
