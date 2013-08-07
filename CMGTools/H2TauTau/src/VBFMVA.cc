#include "CMGTools/H2TauTau/interface/VBFMVA.h"

VBFMVA::VBFMVA (const char * weightsFile): vbfvars_(8,0.)
  {
    init (weightsFile) ;
  }

void VBFMVA::init (const char * weightsFile)
  {
    reader_  = new TMVA::Reader( "!Color:!Silent" ) ;

    reader_->AddVariable("mjj", &vbfvars_[0]);
    reader_->AddVariable("dEta", &vbfvars_[1]);
    reader_->AddVariable("dPhi", &vbfvars_[2]);
    reader_->AddVariable("ditau_pt", &vbfvars_[3]);
    reader_->AddVariable("dijet_pt", &vbfvars_[4]);
    reader_->AddVariable("dPhi_hj", &vbfvars_[5]);
    reader_->AddVariable("C1", &vbfvars_[6]);
    reader_->AddVariable("C2", &vbfvars_[7]);

    std::cout << "VBF MVA: using " << weightsFile << " weight file\n" ;
    reader_->BookMVA("BDTG", weightsFile);      
  }
  
  

VBFMVA::~VBFMVA () { delete reader_ ; } 



double VBFMVA::val (
  double mjj      , // the invariant mass of the two tag jets
  double dEta     , // the pseudorapidity difference between the two tag jets
  double dPhi     , // the phi difference between the two tag jets
  double ditau_pt , // the vector sum of the pT of the tau + electron/muon + MET
  double dijet_pt , // the vector sum of the pT of the two tag jets
  double dPhi_hj  , // the phi difference between the di-tau vector and the di-jet vector
  double C1       , // the pseudorapidity difference between the *visible* di-tau vector and the closest tag jet
  double C2         // the *visible* pT of the di-tau
)
  {
    vbfvars_[0] = mjj ;
    vbfvars_[1] = dEta ;
    vbfvars_[2] = dPhi ;
    vbfvars_[3] = ditau_pt ;
    vbfvars_[4] = dijet_pt ;
    vbfvars_[5] = dPhi_hj ;
    vbfvars_[6] = C1 ;
    vbfvars_[7] = C2 ;

    return reader_->EvaluateMVA(vbfvars_, "BDTG");
  }

