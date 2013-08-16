#include "common_stuff.h"
#include <TLorentzVector.h>


///////////////////////////////////////////////////////////////

// TO LET SPECIFY ALSO THE AXIS TITLES IN THE STRING AND THEN KEEP THE HISTO NAME AND TITLE WITHOUT THEM
TString common_stuff::getCompleteTitleReturnName(std::string title){

  TString completeTitle=Form("%s",title.c_str());
  
  TObjArray* completeTitleTokenized = completeTitle.Tokenize(";");
  
  // int completeTitleTokenized = completeTitleTokenized->GetEntries();
  int ncol = completeTitleTokenized->GetEntries();
  // for(int icol=1; icol<ncol;icol++){
    TString OnlyTitle = ((TObjString *)completeTitleTokenized->At(0))->GetString(); // first tokenized object, i.e. the title
  // }
  
  return OnlyTitle;


}

///////////////////////////////////////////////////////////////

void common_stuff::plot1D(std::string title, double xval, double weight, std::map<std::string, TH1D*> &allhistos, int numbinsx, double xmin, double xmax)
{

  std::map<std::string, TH1D*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new  one
    {
      std::string histoName = (std::string) (common_stuff::getCompleteTitleReturnName(title)).Data();
      TH1D* currentHisto= new TH1D(histoName.c_str(), title.c_str(), numbinsx, xmin, xmax);
      currentHisto->Sumw2();
      currentHisto->Fill(xval, weight);
      allhistos.insert(std::pair<std::string, TH1D*> (histoName,currentHisto) );
    }
  else // exists already, so just fill it                                                                            
    {
      (*iter).second->Fill(xval, weight);
    }
}

///////////////////////////////////////////////////////////////

void common_stuff::plot1D(std::string title, double xval, double weight, std::map<std::string, TH1D*> &allhistos, int numbinsx, double *xarray)
{

  std::map<std::string, TH1D*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new  one
    {
      std::string histoName = (std::string) (common_stuff::getCompleteTitleReturnName(title)).Data();
      TH1D* currentHisto= new TH1D(histoName.c_str(), title.c_str(), numbinsx, xarray);
      currentHisto->Sumw2();
      currentHisto->Fill(xval, weight);
      allhistos.insert(std::pair<std::string, TH1D*> (histoName,currentHisto) );
    }
  else // exists already, so just fill it                                                                            
    {
      (*iter).second->Fill(xval, weight);
    }
}

///////////////////////////////////////////////////////////////

void common_stuff::plot2D(std::string title, double xval, double yval, double weight, std::map<std::string, TH2D*> &allhistos, int numbinsx, double xmin, double xmax, int numbinsy, double ymin, double ymax)
{

  std::map<std::string, TH2D*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
    {
      TH2D* currentHisto= new TH2D(((TString)common_stuff::getCompleteTitleReturnName(title)).Data(), title.c_str(), numbinsx, xmin, xmax, numbinsy, ymin, ymax);
      currentHisto->Sumw2();
      currentHisto->Fill(xval, yval, weight);
      allhistos.insert(std::pair<std::string, TH2D*> (title,currentHisto) );
    }
  else // exists already, so just fill it
    {
      (*iter).second->Fill(xval, yval, weight);
    }

  return;

}

///////////////////////////////////////////////////////////////

void common_stuff::plot2D(std::string title, double xval, double yval, double weight, std::map<std::string, TH2D*> &allhistos, int numbinsx, double *xarray, int numbinsy, double *yarray)
{

  std::map<std::string, TH2D*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one
    {
      TH2D* currentHisto= new TH2D(((TString)common_stuff::getCompleteTitleReturnName(title)).Data(), title.c_str(), numbinsx, xarray, numbinsy, yarray);
      currentHisto->Sumw2();
      currentHisto->Fill(xval, yval, weight);
      allhistos.insert(std::pair<std::string, TH2D*> (title,currentHisto) );
    }
  else // exists already, so just fill it
    {
      (*iter).second->Fill(xval, yval, weight);
    }

  return;

}

///////////////////////////////////////////////////////////////

void common_stuff::writeOutHistos(TFile *fout, std::map<std::string, TH1D*> h_1d, std::map<std::string, TH2D*> h_2d)
{

  fout->cd();
  
  std::map<std::string, TH1D*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write(); 
    delete it1d->second;
  }

  std::map<std::string, TH2D*>::iterator it2d;
  for(it2d=h_2d.begin(); it2d!=h_2d.end(); it2d++) {
    it2d->second->Write(); 
    delete it2d->second;
  }
 

}

///////////////////////////////////////////////////////////////

