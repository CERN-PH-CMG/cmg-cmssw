#include "CMGTools/RootTools/interface/TmpHistName.h"

#include<sstream>

using namespace std;

unsigned TmpHistName::num_ = 0;

string TmpHistName::Name(const char* name) {
  
  ostringstream newName;
  newName<<name<<"_"<<num_;
  ++num_;
  return newName.str();
}
