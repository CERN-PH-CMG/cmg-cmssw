#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"

#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

namespace LHAPDF {
  void initPDFSet(int nset, const std::string& filename, int member=0);
  int numberPDF(int nset);
  void usePDFMember(int nset, int member);
  double xfx(int nset, double x, double Q, int fl);
  double getXmin(int nset, int member);
  double getXmax(int nset, int member);
  double getQ2min(int nset, int member);
  double getQ2max(int nset, int member);
  void extrapolate(bool extrapolate=true);
}


using namespace std;


//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //check arguments
  if ( argc < 2 ) 
    {
      std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
      return 0;
    }
  
  //get configuration
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  bool isMC = runProcess.getParameter<bool>("isMC");
  if(!isMC) {
    cout << "Sample is MC...nothing to be done here" << endl;
    return -1;
  }
  TString url=runProcess.getParameter<std::string>("input");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");

  //open the file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  TString dirname = runProcess.getParameter<std::string>("dirName");
  ZZ2l2nuSummaryHandler evSummaryHandler;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }
  
  string pdfSets[]   = {"cteq66.LHgrid","MSTW2008nlo68cl.LHgrid","NNPDF20_100.LHgrid"};
  Int_t   nPdfVars[] = {44,              41,                      100};
  //const size_t nPdfSets=sizeof(pdfSets)/sizeof(string);
  const size_t nPdfSets=1;
  for(size_t ipdf=0; ipdf<nPdfSets; ipdf++)  LHAPDF::initPDFSet(1, pdfSets[ipdf]);
  
  //output tree
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += outFileUrl + "_pdf.root";
  printf("PDF weights will be saved in %s\n", outUrl.Data());
  TFile *ofile=TFile::Open(outUrl, "recreate");  
  std::vector<float *> pdfWgts;
  TTree *pdfT = new TTree("pdf","pdf");
  for(size_t ipdf=0; ipdf<nPdfSets; ipdf++) 
    {
      pdfWgts.push_back( new float(nPdfVars[ipdf] ) );
      char nameBuf[200],typeBuf[200];
      sprintf(nameBuf,"%s_wgts",       pdfSets[ipdf].c_str() );
      sprintf(typeBuf,"%s_wgts[%d]/F", pdfSets[ipdf].c_str(), nPdfVars[ipdf]);
      pdfT->Branch(nameBuf, pdfWgts[ipdf], typeBuf);
    }
  pdfT->SetDirectory(ofile);
  pdfT->SetAutoSave(1000000);

  //loop over events
  int evStart(0),evEnd(evSummaryHandler.getEntries());
  for( int iev=0; iev<evEnd; iev++)
    {
      if(iev%100==0) { printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd))); cout << flush; }
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();

      try{
	
	for(size_t ipdf=0; ipdf<nPdfSets; ipdf++)
	  {
	    LHAPDF::usePDFMember(1,ipdf);
	    double xpdf1 = LHAPDF::xfx(1, ev.x1, ev.qscale, ev.id1);
	    double xpdf2 = LHAPDF::xfx(1, ev.x2, ev.qscale, ev.id2);
	    double w0 = xpdf1 * xpdf2;
	    for(int i=1; i <=nPdfVars[ipdf]; ++i){
	      try{
		LHAPDF::usePDFMember(1,i);
		double xpdf1_new = LHAPDF::xfx(1, ev.x1, ev.qscale, ev.id1);
		double xpdf2_new = LHAPDF::xfx(1, ev.x2, ev.qscale, ev.id2);
		pdfWgts[ipdf][i-1]=xpdf1_new * xpdf2_new / w0;
	      }catch(...){
	      }
	    }
	    pdfT->Fill();
	  }
      }catch(...){
	cout << "Skipping event #" << iev << endl; 
      }
    }
  
  //all done with the events file
  file->Close();

  //save PDF variations to file
  ofile->cd();
  pdfT->Write();
  ofile->Close();
}  


