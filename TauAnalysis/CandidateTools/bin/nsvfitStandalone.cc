#include "TTree.h"
#include "TFile.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

/**
   \class nsvfitStandalone nsvfitStandalone.cc "TauAnalysis/CandidateTools/bin/nsvfitStandalone.cc"
   \brief Basic example of the use of the standalone version of NSVfit

   This is an example executable to show the use of the standalone version of NSVfit form a flat 
   n-tuple or single event.
*/

void singleEvent()
{
  /* 
     This is a single event for testing in the integration mode.
  */
  // define MET
  Vector MET(11.7491, -51.9172, 0.); 
  // define MET covariance
  TMatrixD covMET(2, 2);
  /*
  covMET[0][0] = 0.;
  covMET[1][0] = 0.;
  covMET[0][1] = 0.;
  covMET[1][1] = 0.;
  */
  covMET[0][0] = 787.352;
  covMET[1][0] = -178.63;
  covMET[0][1] = -178.63;
  covMET[1][1] = 179.545;
  // define lepton four vectors
  NSVfitStandalone::LorentzVector l1( 28.9132, -17.3888, 36.6411, 49.8088); //lepton
  NSVfitStandalone::LorentzVector l2(-24.19  ,  8.77449, 16.9413, 30.8086); //tau
  std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
  measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, l2));
  measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, l1));
  // define algorithm (set the debug level to 3 for testing)
  NSVfitStandaloneAlgorithm algo(measuredTauLeptons, MET, covMET, /*debug=*/0);
  algo.addLogM(false);
  /* 
     the following lines show how to use the different methods on a single event
  */
  // minuit fit method
  //algo.fit();
  // integration by VEGAS (same as function algo.integrate() that has been in use when markov chain integration had not yet been implemented)
  //algo.integrateVEGAS();
  // integration by markov chain MC
  algo.integrateMarkovChain();

  double mass = algo.getMass(); // mass uncertainty not implemented yet
  if(algo.isValidSolution()){
    std::cout << "found mass    = " << mass << std::endl;
  }
  else{
    std::cout << "sorry -- status of NLL is not valid [" << algo.isValidSolution() << "]" << std::endl;
  }
  return;
}

void eventsFromTree(int argc, char* argv[]) 
{
  // parse arguments
  if ( argc < 3 ) {
    std::cout << "Usage : " << argv[0] << " [inputfile.root] [tree_name]" << std::endl;
    return;
  }
  // get intput directory up to one before mass points
  TFile* file = new TFile(argv[1]); 
  // access tree in file
  TTree* tree = (TTree*) file->Get(argv[2]);
  // input variables
  float met, metPhi;
  float covMet11, covMet12; 
  float covMet21, covMet22;
  float l1M, l1Px, l1Py, l1Pz;
  float l2M, l2Px, l2Py, l2Pz;
  float mTrue;
  // branch adresses
  tree->SetBranchAddress("met"          , &met        );
  tree->SetBranchAddress("mphi"         , &metPhi     );
  tree->SetBranchAddress("mcov_11"      , &covMet11   );
  tree->SetBranchAddress("mcov_12"      , &covMet12   );
  tree->SetBranchAddress("mcov_21"      , &covMet21   );
  tree->SetBranchAddress("mcov_22"      , &covMet22   );
  tree->SetBranchAddress("l1_M"         , &l1M        );
  tree->SetBranchAddress("l1_Px"        , &l1Px       );
  tree->SetBranchAddress("l1_Py"        , &l1Py       );
  tree->SetBranchAddress("l1_Pz"        , &l1Pz       );
  tree->SetBranchAddress("l2_M"         , &l2M        );
  tree->SetBranchAddress("l2_Px"        , &l2Px       );
  tree->SetBranchAddress("l2_Py"        , &l2Py       );
  tree->SetBranchAddress("l2_Pz"        , &l2Pz       );
  tree->SetBranchAddress("m_true"       , &mTrue      );
  int nevent = tree->GetEntries();
  for(int i=0; i<nevent; ++i){
    tree->GetEvent(i);
    std::cout << "event " << i+1 << std::endl;
    // setup MET input vector
    NSVfitStandalone::Vector measuredMET(met *TMath::Cos(metPhi), met *TMath::Sin(metPhi), 0); 
    // setup the MET significance
    TMatrixD covMET(2,2);
    covMET[0][0] = covMet11;
    covMET[0][1] = covMet12;
    covMET[1][0] = covMet21;
    covMET[1][1] = covMet22;
    // setup measure tau lepton vectors 
    NSVfitStandalone::LorentzVector l1(l1Px, l1Py, l1Pz, TMath::Sqrt(l1M*l1M+l1Px*l1Px+l1Py*l1Py+l1Pz*l1Pz));
    NSVfitStandalone::LorentzVector l2(l2Px, l2Py, l2Pz, TMath::Sqrt(l2M*l2M+l2Px*l2Px+l2Py*l2Py+l2Pz*l2Pz));
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kLepDecay, l1));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(std::string(argv[2])==std::string("EMu") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, l2));
    // construct the class object from the minimal necesarry information
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET, 1);
    // apply customized configurations if wanted (examples are given below)
    algo.maxObjFunctionCalls(5000);
    //algo.addLogM(false);
    //algo.metPower(0.5)
    // run the fit
    algo.fit();
    // retrieve the results upon success
    std::cout << "... m truth : " << mTrue  << std::endl;
    if(algo.isValidSolution()){
      std::cout << "... m svfit : " << algo.mass() << "+/-" << algo.massUncert() << std::endl;
    }
    else{
      std::cout << "... m svfit : ---" << std::endl;
    }
  }
  return;
}

int main(int argc, char* argv[]) 
{
  //eventsFromTree(argc, argv);
  singleEvent();
  return 0;
}
