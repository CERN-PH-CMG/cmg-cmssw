#ifndef CSAMPLE_HH
#define CSAMPLE_HH
#include "TString.h"
//
// helper class to handle sample inputs
//
class CSample
{
public:
  CSample(){}
  ~CSample(){}
  
  TString          label;    // plot item label
  Int_t            color;    // plot item color
  Int_t            linecol;  // plot item line color
  vector<TString>  fnamev;   // ntuple files
  vector<Double_t> xsecv;    // per file cross section
  vector<TString>  jsonv;    // per file JSON file
  vector<Double_t> weightv;  // per file event weight
  
  // data type
  //  0 : MC
  //  1 : mu-el
  //  2 : di-mu
  //  3 : mu
  //  4 : di-el
  //  5 : el
  vector<Int_t> typev;
};

#endif
