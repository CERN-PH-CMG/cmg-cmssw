#include <iostream>
#include <boost/shared_ptr.hpp>

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TGraph.h"

using namespace std;

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

struct stPDFval{
  stPDFval() {}
  stPDFval(const stPDFval& arg) : 
    qscale(arg.qscale),
    x1(arg.x1),
    x2(arg.x2),
    id1(arg.id1),
    id2(arg.id2){
  }
  Float_t qscale;
  Float_t x1;
  Float_t x2;
  int id1;
  int id2;
};



//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //get configuration
  TString url("root://eoscms//eos/cms/store/cmst3/user/jueugste//pdf_a16_merged/TTJets_MassiveBinDECAYHistos.root");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  
  //INITIALIZE THE PDF TOOL
  string pdfSets[]   = {"cteq66.LHgrid","MSTW2008nlo68cl.LHgrid","NNPDF20_100.LHgrid"};
  std::vector<Int_t>   nPdfVars;
  //const size_t nPdfSets=sizeof(pdfSets)/sizeof(string);
  const size_t nPdfSets=1;
  for(size_t ipdf=0; ipdf<nPdfSets; ipdf++)  
    {
      LHAPDF::initPDFSet(ipdf+1, pdfSets[ipdf]);
      nPdfVars.push_back( LHAPDF::numberPDF(ipdf+1) );
    }

  //open the INPUT file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  TTree *evTree=(TTree *)file->Get("ana/toptree");  
  stPDFval evPdf;
  evTree->SetBranchAddress("qscale", &evPdf.qscale);
  evTree->SetBranchAddress("x1",     &evPdf.x1);
  evTree->SetBranchAddress("x2",     &evPdf.x2);
  evTree->SetBranchAddress("id1",    &evPdf.id1);
  evTree->SetBranchAddress("id2",    &evPdf.id2);
  gROOT->cd();

  //create the OUTPUTFILE AND TREE
  TString outdir("./pdf");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += outFileUrl + "_pdf.root";
  printf("PDF weights will be saved in %s\n", outUrl.Data());
  TFile *ofile=TFile::Open(outUrl, "recreate");  
  TH1F *qscale=new TH1F("qscale",";Q^{2};Events",1000,0.,1000.); 
  qscale->SetDirectory(ofile);
  gROOT->cd(); 


  //loop over events
  std::vector<stPDFval> pdfvals;
  int evStart(0),evEnd(evTree->GetEntriesFast());
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  for( int iev=evStart; iev<evEnd; iev++){
    if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}
    evTree->GetEntry(iev);
    pdfvals.push_back(evPdf);
    qscale->Fill(evPdf.qscale);
  }printf("\n\n\n");
  
  //all done with the events file
  file->Close();

  //Q^2 scale histogram
  ofile->cd();
  float meanScale=qscale->GetMean();
  cout << "<Q^2>=" << meanScale << endl;
  TGraph *nomScale=new TGraph(qscale); nomScale->SetName("qscale_nom");
  nomScale->Write();
  qscale->Write();

  printf("Loop on PDF sets and variations\n");   
  for(size_t ipdf=0; ipdf<nPdfSets; ipdf++){
    for(int i=0; i <(nPdfVars[ipdf]+1); ++i){

      LHAPDF::usePDFMember(ipdf+1,i);
      char nameBuf[256];sprintf(nameBuf,"%s_var%d", pdfSets[ipdf].c_str(), i);
      //printf("%30s:", nameBuf);
     
      //create the output tree
      float pdfWgt(0);
      TTree *pdfT = new TTree(nameBuf,"pdf");
      pdfT->Branch("w", &pdfWgt, "w/F");
      pdfT->SetDirectory(ofile);
      
      for(unsigned int v=0; v<pdfvals.size(); v++){ 
	double xpdf1 = LHAPDF::xfx(ipdf+1, pdfvals[v].x1, pdfvals[v].qscale, pdfvals[v].id1)/pdfvals[v].x1;
	double xpdf2 = LHAPDF::xfx(ipdf+1, pdfvals[v].x2, pdfvals[v].qscale, pdfvals[v].id2)/pdfvals[v].x2;
	pdfWgt = xpdf1 * xpdf2;
	pdfT->Fill();
      }
      ofile->cd();
      pdfT->Write();
    }
    
    printf("Done\n");
  }

  ofile->Close();
  printf("All done\n");fflush(stdout);
}  


