#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>

std::string replace_string(const std::string& src, const std::string& keyword, const std::string& parameter, 
			   unsigned minReplacements, unsigned maxReplacements, int& errorFlag)
{
  std::string modSrc = src;
//--- search for first occuremce of keyword in source string
  std::string::size_type pos = modSrc.find(keyword);

  unsigned numReplacements = 0;

  while ( pos != std::string::npos ) {
     modSrc.replace(pos, keyword.length(), parameter);
     ++numReplacements;
//--- do not search keyword within parameter which has been replaced
//    (in order to allow keyword to be present in parameter
//     and void running into an infinite loop in that case)
     pos += parameter.length();

//--- search for next occuremce of keyword in source string
     pos = modSrc.find(keyword, pos);
  }
  if ( (numReplacements < minReplacements) ||
       (numReplacements > maxReplacements) ) {
    edm::LogError ("replace_string") << " Failed to replace parameter = " << parameter << " in src = " << src << ","
				     << " numReplacements = " << numReplacements 
				     << " (min = " << minReplacements << ", max = " << maxReplacements << ") !!";
    errorFlag = 1;
  }
  return modSrc;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template <typename T>
std::string format_vT(const std::vector<T>& vT)
{
  std::ostringstream os;
  
  os << "{ ";

  unsigned numEntries = vT.size();
  for ( unsigned iEntry = 0; iEntry < numEntries; ++iEntry ) {
    os << vT[iEntry];
    if ( iEntry < (numEntries - 1) ) os << ", ";
  }

  os << " }";
  
  return os.str();
}

std::string format_vstring(const std::vector<std::string>& vs)
{
  return format_vT(vs);
}

std::string format_vdouble(const std::vector<double>& vd)
{
  return format_vT(vd);
}

std::string format_vfloat(const std::vector<float>& vf)
{
  return format_vT(vf);
}

std::string format_vint(const std::vector<int>& vi)
{
  return format_vT(vi);
}

std::string format_vunsigned(const std::vector<unsigned>& vu)
{
  return format_vT(vu);
}

std::string format_vbool(const std::vector<bool>& vb)
{
  return format_vT(vb);
}

std::string format_vInputTag(const std::vector<edm::InputTag>& vit)
{
  std::vector<std::string> vit_string;
  for ( std::vector<edm::InputTag>::const_iterator vit_i = vit.begin();
	vit_i != vit.end(); ++vit_i ) {
    vit_string.push_back(vit_i->label());
  }
  return format_vT(vit_string);
}
