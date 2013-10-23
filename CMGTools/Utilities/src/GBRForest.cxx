#include "CMGTools/Utilities/interface/GBRForest.h"
//#include <iostream>
#include "TMVA/DecisionTree.h"
#include "TMVA/MethodBDT.h"



//_______________________________________________________________________
GBRForest::GBRForest() : 
  fInitialResponse(0.)
{

}

//_______________________________________________________________________
GBRForest::~GBRForest() 
{
}

//_______________________________________________________________________
GBRForest::GBRForest(const TMVA::MethodBDT *bdt)
{
  
  if (bdt->DoRegression()) {
    fInitialResponse = bdt->GetBoostWeights().front();
  }
  else {
    fInitialResponse = 0.;
  }
  
  const std::vector<TMVA::DecisionTree*> &forest = bdt->GetForest();
  fTrees.reserve(forest.size());
  for (std::vector<TMVA::DecisionTree*>::const_iterator it=forest.begin(); it!=forest.end(); ++it) {
    fTrees.push_back(GBRTree(*it));
  }
  
}




