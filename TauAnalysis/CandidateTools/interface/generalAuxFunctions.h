#ifndef TauAnalysis_DQMTools_generalAuxFunctions_h
#define TauAnalysis_DQMTools_generalAuxFunctions_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <string>
#include <vector>

std::string replace_string(const std::string&, const std::string&, const std::string&, unsigned, unsigned, int&);

std::string format_vstring(const std::vector<std::string>&);
std::string format_vdouble(const std::vector<double>&);
std::string format_vfloat(const std::vector<float>&);
std::string format_vint(const std::vector<int>&);
std::string format_vunsigned(const std::vector<unsigned>&);
std::string format_vbool(const std::vector<bool>&);
std::string format_vInputTag(const std::vector<edm::InputTag>&);

template <class T>
void readCfgParameter(const edm::ParameterSet& cfgParSet, std::map<std::string, T>& def)
{
  std::vector<std::string> cfgParNames = cfgParSet.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator cfgParName = cfgParNames.begin(); 
	cfgParName != cfgParNames.end(); ++cfgParName ) {
    edm::ParameterSet cfgParDef = cfgParSet.getParameter<edm::ParameterSet>(*cfgParName);
    
    def.insert(std::pair<std::string, T>(*cfgParName, T(*cfgParName, cfgParDef)));
  }
}

template <class T>
void copyCfgParameter(const edm::ParameterSet& cfgParSet_source, const std::string& cfgParName_source,
		      edm::ParameterSet& cfgParSet_target, std::string cfgParName_target = "")
{
  if ( cfgParName_target == "" ) cfgParName_target = cfgParName_source;
  const T& cfgParValue = cfgParSet_source.getParameter<T>(cfgParName_source);
  cfgParSet_target.addParameter(cfgParName_target, cfgParValue);
}

#endif
