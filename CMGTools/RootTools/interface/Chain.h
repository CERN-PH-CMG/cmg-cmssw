#ifndef __PFAnalyses_RootTools_Chain__
#define __PFAnalyses_RootTools_Chain__

/*

This class just adds a 'pattern' constructor to TChain.

Chain::Chain(const char* name, const char* pattern) find all files
matching pattern. For CASTOR areas (i.e., any pattern starting with
'rfio') the builtin TChain wildcarding is used: wildcards only allowed
in the file name, not in the path name. For normal disk areas full
globbing is supported.

*/

#include "TChain.h"
#include <vector>
#include <string>

class Chain : public TChain {
    ClassDef(Chain, 1);
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

  std::vector<std::string> Expand(std::string const& pattern);
};

#endif
