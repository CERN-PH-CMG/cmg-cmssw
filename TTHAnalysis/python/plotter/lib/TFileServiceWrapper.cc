/** \file
 *
 *  $Date: 2012/06/24 15:31:20 $
 *  $Revision: 1.2 $
 *  \author N. Amapane - CERN
 */

#include "TFileServiceWrapper.h"
#include <iostream>

TDirectory* TFileServiceWrapper::theDir=0;
TFile*      TFileServiceWrapper::theFile=0;

using namespace std;

TFile* TFileServiceWrapper::init(TString filename){
  theFile = TFile::Open(filename.Data(), "RECREATE"); //"NEW");
  return theFile;
}


void TFileServiceWrapper::mkdir(TString dir){
  theDir = theFile->mkdir(dir);  
}


TFileServiceWrapper::~TFileServiceWrapper(){
  //  cout << "Writing histograms in: "; theDir->pwd();
  theDir->cd();
  for (vector<TH1F*>::iterator h =histos.begin(); h!= histos.end(); ++h) {
    (*h)->Write();
  }
}


TH1F* 
TFileServiceWrapper::makeTH1F(const char* name, const char* title, int nbinsx, double xlow, double xup) {
  theDir->cd();
  TH1F* h = new TH1F(name, title, nbinsx, xlow, xup);
  histos.push_back(h);
  return h;  
}


