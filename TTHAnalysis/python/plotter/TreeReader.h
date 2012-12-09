#include "lib/TTHTree.h"

class TreeReader: public TTHTree {
  
public:
  TreeReader(TChain *tree, float weight_) : 
    TTHTree(tree),
    weight(weight_) {
  }

  ~TreeReader(){};

  void Loop(TString mode = "");

private: 
  float weight;

};

#if defined(__MAKECINT__)  
#pragma link C++ class TreeReader;
#endif
