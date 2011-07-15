#include "CMGTools/H2TauTau/interface/H2TauTauFitter.h"


H2TauTauFitter::H2TauTauFitter(TString outPutPath):
  TNamed("H2TauTauFitter","H2TauTauFitter"),
  outPutPath_(outPutPath),
  data_(0),
  signalPdf_(0),
  backgroundPdf_(0),
  totalPdf_(0),
  signalYield_("signalYield_","signalYield_",0,1),
  fitResults_(0),
  fitSucceeded_(0){
  
  //create an output directory
  //check if output dir already exists
  //system(TString("mkdir ")+outPutPath_);

  logfile_.open(outPutPath_+"/FitLog.log",ios::out);

}


H2TauTauFitter::~H2TauTauFitter(){
  
  delete totalPdf_;
}



Bool_t H2TauTauFitter::fit(){
  if(!data_ || !signalPdf_ || !backgroundPdf_){ cout<<"H2TauTauFitter::fit() Error: No data_, signalPdf_, or backgroundPdf_ defined"<<endl; return 0;}
  
  if(totalPdf_){ cout<<"H2TauTauFitter::fit() Error: totalPdf_ is already defined, something is wrong."<<endl; return 0;}
  totalPdf_=new RooAddPdf("totalPdf_","totalPdf_",*signalPdf_,*backgroundPdf_,signalYield_);

  fitResults_=totalPdf_->fitTo(*data_);

  fitSucceeded_=1;
  return 1;
}


void H2TauTauFitter::SaveFitResults(){
  if(!fitSucceeded_){ cout<<"H2TauTauFitter::saveFitResults() Error: Trying to save a bad fit."<<endl; return ;}

  TFile OutPutFile(outPutPath_+"/FitResult.root","recreate");
  if(data_)data_->Write(); else cout<<"H2TauTauFitter::saveFitResults Error: data_ doesnt exist"<<endl;
  if(totalPdf_) totalPdf_->Write();  else cout<<"H2TauTauFitter::saveFitResults Error: totalPdf_ doesnt exist"<<endl;
  if(fitResults_) fitResults_->Write();  else cout<<"H2TauTauFitter::saveFitResults Error: fitResults_ doesnt exist"<<endl;
  OutPutFile.ls();
  OutPutFile.Close();
  
  logfile_.close();
  
}
