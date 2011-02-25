#include "CMGTools/RootTools/interface/DataSets.h"
#include "CMGTools/RootTools/interface/TmpHistName.h"

#include <TH1F.h> 
#include <sstream>

using namespace std;





void DataSets::Print( ostream& out) const {
  out<<"DataSets"<<endl;
}


void DataSets::ScaleIntLumi() {
  
  for( DI di = this->begin(); di!=this->end(); ++di) {
    (*di)->SetIntLumi( intLumi_ ); 
  }
}
