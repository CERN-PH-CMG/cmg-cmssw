#ifndef __PFAnalyses_RootTools_Chain__
#define __PFAnalyses_RootTools_Chain__

/* Colin Bernet, september 2002

This class just adds a 'pattern' constructor to TChain.

TChain::TChain(const char* name, const char* pattern) will use `ls
pattern` to find all files matching pattern. If they are root files,
and if the contain a tree named name, this tree will be chained to
this.

20100927 JH
Modified this class to work with CASTOR files. At the same time
modified things to use the pattern functionality from TChain.

*/

#include "TChain.h"
#include <vector>
#include <string>

class Chain : public TChain {

public:
  // If the testFiles flags is set, all files are opened after being
  // added to the chain to verify that the requested tree
  // exists. Slow, but it may avoid problems later on.
  Chain(const char* chainname, const char* pattern,
        const char* legendEntry="", bool const testFiles=true);
  virtual ~Chain() {}
  std::vector<std::string> files() const;
  const char* legendEntry() const {return legendEntry_.c_str();}

private:
  std::string legendEntry_;
};

#endif
