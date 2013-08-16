//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for LHEEventProduct
// Created:     Mon Oct  8 16:03:12 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/LHEEventProductHelper.h"
//-----------------------------------------------------------------------------
using namespace std;

//-----------------------------------------------------------------------------
// This constructor is called once per job
LHEEventProductHelper::LHEEventProductHelper()
  : HelperFor<LHEEventProduct>() {}
    
LHEEventProductHelper::~LHEEventProductHelper() {}

// -- Called once per event
//void LHEEventProductHelper::analyzeEvent()
//{
//
//}

// -- Called once per object
void LHEEventProductHelper::analyzeObject()
{

  comments_const_iterator c_begin = object->comments_begin();
  comments_const_iterator c_end = object->comments_end();

  size_t found;
  std::istringstream iss;
  std::string smaller;
  for( comments_const_iterator cit=c_begin; cit!=c_end; ++cit) {
    //size_t found = (*cit).find("model");
    //mSugra string: "model msugra_1620_300_40_m500_1"
    //if( found != std::string::npos) {
    //         std::cout << *cit << std::endl;
    //}
    //cout << *cit << endl;
    size_t foundLength = (*cit).size();
    //cout << foundLength << endl;
    
    //T2tt models (e.g. T2tt_375.0_175.0)
    found = (*cit).find("T2tt");
    //std::cout << "T2tt found = " << found << std::endl;
    if(found < foundLength){
      smaller = (*cit).substr(found+1,foundLength);
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      //
      iss.str(smaller);
      iss >> _mt1;
      iss.clear();
      //
      found = smaller.find("_");
      smaller = smaller.substr(found+1,smaller.size());
      iss.str(smaller);
      iss >> _mz1;
      iss.clear();
      //cout << _mt1 << " " << _mz1 << endl;
    }
    
  }
  

}

// -- User access methods
//double LHEEventProductHelper::someMethod()  const
//{
//  return  //-- some-value --
//}
double LHEEventProductHelper::mt1()  const
{
  return  _mt1;
}
double LHEEventProductHelper::mz1()  const
{
  return  _mz1;
}
