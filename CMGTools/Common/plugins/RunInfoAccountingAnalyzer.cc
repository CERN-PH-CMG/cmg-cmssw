#include "CMGTools/Common/plugins/RunInfoAccountingAnalyzer.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/FileBlock.h"

#include<TROOT.h>



using namespace std;

cmg::RunInfoAccountingAnalyzer::RunInfoAccountingAnalyzer(const edm::ParameterSet& ps) : 
  name_( ps.getUntrackedParameter<std::string>("name","RunInfoAccounting")),
  runInfoAccounting_( *fs_, name_ ), runNumber_(-1), fileChanged_(false), currentRun_(0)
{}

void cmg::RunInfoAccountingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  if( fileChanged_ ) {

   runNumber_ = iEvent.run();
   currentRun_ = &(iEvent.getRun());
   cout<<"RunInfoAccountingAnalyzer: new file, run="<<runNumber_<<endl;
   runInfoAccounting_.processRunInfo( iEvent.getRun() );
   fileChanged_ = false;
  }
}
    
void cmg::RunInfoAccountingAnalyzer::respondToOpenInputFile(edm::FileBlock const& fb) {
//   cout<<"opening file"<<endl;

//   TTree *runTree = fb.runTree();

//   runTree->Print();

//   edm::Run* run; 
//   runTree->SetBranchAddress( "Runs", &run );
//   runTree->GetEntry(0);
  
//   cout<<runTree->GetEntries()<<endl;

  fileChanged_ = true;

}

void cmg::RunInfoAccountingAnalyzer::respondToCloseInputFile(edm::FileBlock const& fb) {
}

void cmg::RunInfoAccountingAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const& lumi, 
							  edm::EventSetup const&) {
//   cout<<"begin lumi"<<endl;
  
}

void cmg::RunInfoAccountingAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
//   cout<<"end lumi"<<endl;

}


void cmg::RunInfoAccountingAnalyzer::beginRun(edm::Run const& run, edm::EventSetup const&) {
}
