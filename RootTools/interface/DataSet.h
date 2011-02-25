#ifndef __PFAnalyses_RootTools_DataSet__
#define __PFAnalyses_RootTools_DataSet__


/*! 
  \class DataSet
 
  \author Colin Bernet, september 2010
*/

#include "CMGTools/RootTools/interface/Chain.h"


#include <TTree.h>
#include <TFile.h>

#include <map>
#include <string>

#include <iostream>


class DataSet {
 public:
  /// default constructor
  DataSet() : 
    tree_(0), fileRia_(0), fileHistos_(0), 
    nInitial_(-1), nFinal_(-1), xSection_(-1), intLumi_(1) {}

  DataSet( TTree* tree, TFile* file );
  DataSet( TTree* tree, TFile* fileRia, TFile* fileHistos );
  
  /// destructor
  virtual ~DataSet() {}
  
  /// not really necessary? 
  void SetIntLumi( double intLumi ) { 
    intLumi_ = intLumi; 
    UpdateWeights();
  }

  void SetXSection( double xSection ) { 
    xSection_ = xSection; 
    UpdateWeights();
  }

  void SetNInitial( int nInitial) {
    nInitial_ = static_cast<int>(nInitial);
    UpdateWeights();
  }

  void ExtractInfo(); 
  
  double GetWeight() const { 
    if(nInitial_>0)
      return (genFilterEfficiency_*xSection_*intLumi_)/nInitial_; 
    else 
      return 1;
  }

  void Draw( const char* var, const char* cut = "", const char* opt = "", float fracStat = 1);

  /// draw an histogram 
  TH1* DrawHist(const char* histName, const char* opt=""); 
  
  /// get an histogram 
  TH1* GetHist(const char* histName); 

  TTree*   GetTree() {return tree_;}

  TFile*   GetFileRia() {return fileRia_;}
  TFile*   GetFileHistos() {return fileHistos_;}

  /// print all internal information.
  void Print(std::ostream& out=std::cout) const;
  
  /// print normalization information 
  void PrintNormInfo( std::ostream& out=std::cout ) const;

 private:

  void UpdateWeights(); 

  /// what happens if 2 datasets made with the same TTree??
  TTree*   tree_;
  TFile*   fileRia_;
  TFile*   fileHistos_;

  double    nInitial_;
  double    nFinal_;
  double    xSection_;
  double    genFilterEfficiency_;
  double    intLumi_;

 };
#endif

