#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"
#include "TROOT.h"

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

struct stPDFval{
   double qscale;
   double x1;
   double x2;
   double id1;
   double id2;
};

//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //check arguments
  if ( argc < 2 ){
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

  //INITIALIZE THE PDF TOOL
  string pdfSets[]   = {"cteq66.LHgrid","MSTW2008nlo68cl.LHgrid","NNPDF20_100.LHgrid"};
  Int_t   nPdfVars[] = {44,              41,                      100};
  //const size_t nPdfSets=sizeof(pdfSets)/sizeof(string);
  const size_t nPdfSets=1;
  for(size_t ipdf=0; ipdf<nPdfSets; ipdf++)  LHAPDF::initPDFSet(1, pdfSets[ipdf]);


  //open the INPUT file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  TString dirname = runProcess.getParameter<std::string>("dirName");
  ZZ2l2nuSummaryHandler evSummaryHandler;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ){
      file->Close();
      return -1;
  }
  gROOT->cd();

  //create the OUTPUTFILE AND TREE
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += outFileUrl + "_pdf.root";
  printf("PDF weights will be saved in %s\n", outUrl.Data());
  TFile *ofile=TFile::Open(outUrl, "recreate");  
  gROOT->cd(); 


  //loop over events
  std::vector<stPDFval> pdfvals;
  int evStart(0),evEnd(evSummaryHandler.getEntries());
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();
      stPDFval valForPDF;
      valForPDF.qscale = ev.qscale;
      valForPDF.x1     = ev.x1;
      valForPDF.x2     = ev.x2;      
      valForPDF.id1     = ev.id1;
      valForPDF.id2     = ev.id2;
      
      pdfvals.push_back(valForPDF);
   }printf("\n\n\n");
  //all done with the events file
  file->Close();

  
   printf("Loop on PDF sets and variations\n");   
   for(size_t ipdf=0; ipdf<nPdfSets; ipdf++){
      for(int i=0; i <(nPdfVars[ipdf]+1); ++i){
         char nameBuf[256];sprintf(nameBuf,"%s_var%d", pdfSets[ipdf].c_str(), i);
         printf("%30s:", nameBuf);
         LHAPDF::usePDFMember(ipdf,i);

         //create the output tree
         float pdfWgt;
         TTree *pdfT = new TTree(nameBuf,"pdf");
         pdfT->Branch("w", &pdfWgt, "F");
         pdfT->SetDirectory(ofile);

         for(unsigned int v=0; v<pdfvals.size(); v++){
            double xpdf1 = LHAPDF::xfx(1, pdfvals[v].x1, pdfvals[v].qscale, pdfvals[v].id1);
            double xpdf2 = LHAPDF::xfx(1, pdfvals[v].x2, pdfvals[v].qscale, pdfvals[v].id2);
            pdfWgt = xpdf1 * xpdf2;
            pdfT->Fill();
         }
         ofile->cd();
         pdfT->Write();
         printf("Done\n");
      }
  }

  ofile->Close();
  printf("All done\n");fflush(stdout);
}  


