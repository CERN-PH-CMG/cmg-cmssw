#ifndef LHEEVENTPRODUCTHELPER_H
#define LHEEVENTPRODUCTHELPER_H
//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for LHEEventProduct
// Created:     Mon Oct  8 16:03:12 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "PhysicsTools/TheNtupleMaker/interface/HelperFor.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
//-----------------------------------------------------------------------------
// Definitions:
//   helper:        object of class LHEEventProductHelper
//   helped object: object of class LHEEventProduct
//
//
// The following variables are automatically defined and available to
//       all methods:
//
//         blockname          name of config. buffer object (config block) 
//         buffername         name of buffer in config block
//         labelname          name of label in config block (for getByLabel)
//         parameter          parameter (as key, value pairs)
//                            accessed as in the following example:
//
//                            string param = parameter("label");
//
//         0. hltconfig       pointer to HLTConfigProvider
//                            Note: this will be zero if HLTConfigProvider
//                                  has not been properly initialized
//
//         1. config          pointer to global ParameterSet object
//         2. event           pointer to the current event
//         3. object          pointer to the current helped object
//         4. oindex          index of current helped object
//
//         5. index           index of item(s) returned by helper.
//                            Note 1: an item is associated with all
//                                    helper methods (think of it as an
//                                    extension of the helped object)
//                                  
//                            Note 2: index may differ from oindex if,
//                                    for a given helped object, the count
//                                    variable (see below) differs from 1.
//
//         6. count           number of items per helped object (default=1)
//                            Note:
//                                  count = 0  ==> current helped object is
//                                                 to be skipped
//
//                                  count = 1  ==> current helped object is
//                                                 to be kept
//
//                                  count > 1  ==> current helped object is
//                                                 associated with "count"
//                                                 items, where each item
//                                                 is associated with all the
//                                                 helper methods
//
//       variables 0-6 are initialized by TheNtupleMaker.
//       variables 0-5 should not be changed.
//       variable    6 can be changed by the helper to control whether a
//                     helped object should be kept or generates more items
//-----------------------------------------------------------------------------

/// A helper for LHEEventProduct.
class LHEEventProductHelper : public HelperFor<LHEEventProduct>
{
public:
  ///
  LHEEventProductHelper();

  virtual ~LHEEventProductHelper();

  // Uncomment if helper class does some event-level analysis
  // virtual void analyzeEvent();

  // Uncomment if helper class does some object-level analysis
  virtual void analyzeObject();

  // ---------------------------------------------------------
  // -- User access methods go here
  // ---------------------------------------------------------
  double mt1() const;
  double mz1() const;

private:
  // -- User internals
  typedef std::vector<std::string>::const_iterator
    comments_const_iterator;
  
  double _mt1, _mz1;

public:
  // ---------------------------------------------------------
  // -- Access Methods
  // ---------------------------------------------------------

  // WARNING: some methods may fail to compile because of coding
  //          problems in one of the CMSSW base classes. If so,
  //          just comment out the offending method and try again.
  
  

  // from LHEEventProduct
  std::size_t comments_size() const { return object->comments_size(); }

  // from LHEEventProduct
  const lhef::HEPEUP hepeup() const { return object->hepeup(); }

  // from LHEEventProduct
  gen::PdfInfo* pdf() const { return (gen::PdfInfo*)object->pdf(); }
};
#endif
