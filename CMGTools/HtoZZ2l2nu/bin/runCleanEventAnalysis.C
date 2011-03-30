#include "CMGTools/HtoZZ2l2nu/interface/CleanEventAnalysis.h"
#include "PhysicsTools/UtilAlgos/interface/FWLiteAnalyzerWrapper.h"

typedef fwlite::AnalyzerWrapper<CleanEventAnalysis> WrappedFWLiteCleanEventAnalyzer;

int main(int argc, char* argv[]) 
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // only allow one argument for this simple example which should be the python cfg file
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  // get the python configuration                                                                                                      
  PythonProcessDesc builder(argv[1]);
  WrappedFWLiteCleanEventAnalyzer an(*(builder.processDesc()->getProcessPSet()), std::string("evAnalyzer"), std::string("plots"));
  an.beginJob();
  an.analyze();
  an.endJob();
  return 0;
}
