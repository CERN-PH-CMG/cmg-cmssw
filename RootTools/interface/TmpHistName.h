#ifndef __PFAnalyses_RootTools_TmpHistName__
#define __PFAnalyses_RootTools_TmpHistName__

#include<string>

class TmpHistName {
  
 public:
  static std::string Name(const char* );
  
 private:
  static unsigned num_;
};

#endif
