#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include<TROOT.h>



using namespace std;

cmg::RunInfoAccountingAnalyzer::RunInfoAccountingAnalyzer(const edm::ParameterSet& ps) : 
  name_( ps.getUntrackedParameter<std::string>("name","RunInfoAccounting")),
  runInfoAccounting_( *fs_, name_ ), runNumber_(-1), 
  fileChanged_(false), currentRun_(0), nTotal_(0), nPassed_(0)
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
  edm::Handle<GenFilterInfo> genFilter;
  const bool hasGenFilterInfo = iLumi.getByType(genFilter);

  if(!hasGenFilterInfo){
    return; 
  }

  std::cout << "Lumi section " << iLumi.id() << std::endl;
  std::cout << "N total = " << genFilter->numEventsTried() << " N passed = " << genFilter->numEventsPassed() << std::endl;
  std::cout << "Generator filter efficiency = " << genFilter->filterEfficiency() << " +- " << genFilter->filterEfficiencyError() << std::endl;

  // At each lumi block, incrementing these counters with the number of events
  // tried and passed at this lumi.
  // also see the endRun function. 

  nTotal_ += genFilter->numEventsTried();
  nPassed_ += genFilter->numEventsPassed(); 
}


void cmg::RunInfoAccountingAnalyzer::beginRun(edm::Run const& run, edm::EventSetup const&) {
}

void cmg::RunInfoAccountingAnalyzer::endRun(edm::Run const& run, edm::EventSetup const&) {
//   fileChanged_ = true;

  //COLIN need to find out how to get the run number from here
  runNumber_ = 1;
  currentRun_ = &run;
  cout<<"RunInfoAccountingAnalyzer: new file, run="<<runNumber_<<endl;
  runInfoAccounting_.processRunInfo( run, nTotal_, nPassed_);
//   fileChanged_ = false;
}
