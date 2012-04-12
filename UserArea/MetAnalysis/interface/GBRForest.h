
#ifndef ROOT_GBRForest
#define ROOT_GBRForest

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// GBRForest                                                            //
//                                                                      //
// A fast minimal implementation of Gradient-Boosted Regression Trees   //
// which has been especially optimized for size on disk and in memory.  //                                                                  
//                                                                      //
// Designed to built from TMVA-trained trees, but could also be         //
// generalized to otherwise-trained trees, classification,              //
//  or other boosting methods in the future                             //
//                                                                      //
//  Josh Bendavid - MIT                                                 //
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include <vector>
#include "GBRTree.h"


  namespace TMVA {
    class MethodBDT;
  }

  class GBRForest : public TNamed {

    public:

       GBRForest();
       GBRForest(const TMVA::MethodBDT *bdt);
       virtual ~GBRForest();
       
       Double_t GetResponse(const Float_t* vector) const;
      
       std::vector<GBRTree*> &Trees() { return fTrees; }
       
    protected:
      Double_t             fInitialResponse;
      std::vector<GBRTree*> fTrees;
    
    ClassDef(GBRForest,1) // Node for the Decision Tree 
  };

//_______________________________________________________________________
inline Double_t GBRForest::GetResponse(const Float_t* vector) const {
  Double_t response = fInitialResponse;
  for (std::vector<GBRTree*>::const_iterator it=fTrees.begin(); it!=fTrees.end(); ++it) {
    response += (*it)->GetResponse(vector);
  }
  return response;
}
  
#endif
