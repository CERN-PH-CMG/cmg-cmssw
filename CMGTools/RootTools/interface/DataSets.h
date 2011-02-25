#ifndef __PFAnalyses_RootTools_DataSets__
#define __PFAnalyses_RootTools_DataSets__


/*! 
  \class DataSets
 
  \author Colin Bernet, september 2010
*/

#include "PFAnalyses/RootTools/interface/DataSet.h"



#include <vector>
#include <iostream>



class DataSets : public std::vector<DataSet*> {
 public:
  
  typedef std::vector<DataSet*> DataSetCollection; 
  typedef std::vector<DataSet*>::iterator DI; 
  typedef std::vector<DataSet*>::const_iterator DCI; 
  
  /// default constructor
  DataSets() :  intLumi_(-1), xSection_(-1) {}

  DataSets( double intLumi, double xSection=-1 ) : intLumi_(intLumi), xSection_(xSection) {}
 
  /// destructor
  virtual ~DataSets() {}
  
  void SetIntLumi( double intLumi ) { intLumi_ = intLumi; ScaleIntLumi(); }

  void SetXSection( double xSection ) {  xSection_ = xSection; }

  void Print( std::ostream& out) const;

 private:

  void ScaleIntLumi();

  double    intLumi_;
  double    xSection_;

 };
#endif

