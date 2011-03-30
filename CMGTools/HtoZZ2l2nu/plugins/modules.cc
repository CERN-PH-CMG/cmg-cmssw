#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

#include "CMGTools/HtoZZ2l2nu/interface/cleanEventAnalysis.h"
#include "PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h"
typedef edm::AnalyzerWrapper<CleanEventAnalysis> WrappedEDCleanEventAnalyzer;
DEFINE_FWK_MODULE(WrappedEDCleanEventAnalyzer);
