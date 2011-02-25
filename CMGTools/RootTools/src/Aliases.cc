#include "PFAnalyses/RootTools/interface/Aliases.h"

#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include <TTree.h>

Aliases::Aliases(const char* cfgFileName) {

  using namespace std;

  try {
    PythonProcessDesc builder(cfgFileName);

    boost::shared_ptr<edm::ParameterSet> ps = builder.processDesc()->getProcessPSet();
    
    const edm::ParameterSet& aliases = ps->getParameter< edm::ParameterSet >("aliases");
    
    std::vector<std::string> names = aliases.getParameterNamesForType< std::string >();
    
    for(unsigned i=0; i<names.size(); ++i) {
      std::string aliasSource =  aliases.getParameter< std::string > ( names[i] );
      cout<<names[i]<<" "<<aliasSource<<endl;
      aliases_[names[i] ] = aliasSource;
    }
  }
  catch( const std::exception& err) {
    cerr<<err.what()<<endl;
    exit(1);
  }
}


void Aliases::Print( std::ostream& out ) const {
  out<<"list of aliases: "<<std::endl;
  for (AI ai = aliases_.begin(); ai!=aliases_.end(); ++ai) {
    out<<ai->first<<" "<<ai->second<<std::endl;
  }
} 


void Aliases::SetAliases(TTree* tree) const {
  if( !tree ) return;

  std::cout<<"setting aliases"<<std::endl;
  for (AI ai = aliases_.begin(); ai!=aliases_.end(); ++ai) {
    std::cout<<ai->first<<" "<<ai->second<<std::endl;
    tree->SetAlias( ai->first.c_str(), ai->second.c_str() );
  }
}
