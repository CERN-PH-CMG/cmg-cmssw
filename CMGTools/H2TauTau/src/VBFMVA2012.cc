#include "CMGTools/H2TauTau/interface/VBFMVA2012.h"

VBFMVA2012::VBFMVA2012 (const char * weightsFile): vbfvars_(4,0.)
  {
    init (weightsFile) ;
  }

void VBFMVA2012::init (const char * weightsFile)
  {
    reader_  = new TMVA::Reader( "!Color:!Silent" ) ;

    reader_->AddVariable("mjj", &vbfvars_[0]);
    reader_->AddVariable("dEta", &vbfvars_[1]);
    reader_->AddVariable("C1", &vbfvars_[2]);
    reader_->AddVariable("C2", &vbfvars_[3]);

    std::cout << "VBF MVA: using " << weightsFile << " weight file\n" ;
    reader_->BookMVA("BDTG", weightsFile);      
  }
  
  

VBFMVA2012::~VBFMVA2012 () { delete reader_ ; } 



double VBFMVA2012::val (
  double mjj      , // the invariant mass of the two tag jets
  double dEta     , // the pseudorapidity difference between the two tag jets
  double C1       , // the pseudorapidity difference between the *visible* di-tau vector and the closest tag jet
  double C2         // the *visible* pT of the di-tau
)
  {
    vbfvars_[0] = mjj ;
    vbfvars_[1] = dEta ;
    vbfvars_[2] = C1 ;
    vbfvars_[3] = C2 ;

    return reader_->EvaluateMVA(vbfvars_, "BDTG");
  }

