#ifndef TFileServiceWrapper_h
#define TFileServiceWrapper_h

/** \class TFileServiceWrapper
 *
 *  No description available.
 *
 *  $Date: 2012/06/19 22:39:46 $
 *  $Revision: 1.1 $
 *  \author N. Amapane - CERN
 */

#include <TH1F.h>
#include <TFile.h>
#include <TString.h>
#include <vector>

class TFileServiceWrapper {
 public:
  /// Constructor
  TFileServiceWrapper(){}

  /// Destructor
  virtual ~TFileServiceWrapper();
  
  // Operations
  TH1F* makeTH1F(const char* name, const char* title, int nbinsx, double xlow, double xup);

  static TFile* init(TString filename);

  static void mkdir(TString dir);

 private:
  static TDirectory* theDir;
  static TFile* theFile;
  std::vector <TH1F*> histos;

};
#endif

