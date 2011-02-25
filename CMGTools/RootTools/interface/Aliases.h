#ifndef __PFAnalyses_RootTools_Aliases__
#define __PFAnalyses_RootTools_Aliases__


/*! 
  \class Aliases
 
  \author Colin Bernet, september 2010
*/

#include "CMGTools/RootTools/interface/DataSet.h"



#include <map>
#include <string>
#include <iostream>

class TTree;

class Aliases {
 public:
  
  
  /// default constructor
  Aliases()  {}

  Aliases(const char* cfgFileName);
 
  /// destructor
  virtual ~Aliases() {}
  
  void Print(std::ostream& out=std::cout) const;
  
  void SetAliases( TTree* tree ) const;  
  
 private:
  typedef std::map< std::string, std::string > AliasMap;
  typedef AliasMap::const_iterator AI; 
  AliasMap aliases_;

 };
#endif

