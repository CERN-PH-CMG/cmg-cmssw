#include "PFAnalyses/RootTools/interface/DataSet.h"
#include "PFAnalyses/RootTools/interface/TmpHistName.h"

#include <TH1F.h> 
#include <sstream>

using namespace std;




DataSet::DataSet( TTree* tree, TFile* file ) :
  tree_(tree), fileRia_(file), fileHistos_(file), intLumi_(1)
{
  ExtractInfo();
}

DataSet::DataSet( TTree* tree, TFile* fileRia, TFile* fileHistos ) :
  tree_(tree), fileRia_(fileRia), fileHistos_(fileHistos), intLumi_(1)
{
  ExtractInfo();
}

void DataSet::Print( ostream& out) const {
  
  out<<"DataSet"<<endl;
  cout<<" tree       : "<<tree_->GetTitle()<<endl;
  tree_->GetListOfLeaves()->Print();
  cout<<" file RIA   : "<<fileRia_->GetName()<<":"<<endl;
  cout<<" file Histos: "<<fileHistos_->GetName()<<":"<<endl;
  // fileHistos_->ls();
}


void DataSet::Draw( const char* var, const char* cut, const char* opt, float fracStat ) {
  if(!tree_) {
    cout<<"tree not initialized!"<<endl;
    return;
  }
  else {
    // in principle not necessary 
    tree_->SetWeight( GetWeight() ); 
    tree_->Draw( var, cut, opt, (int)(fracStat*tree_->GetEntries()) );  
  }
}


TH1* DataSet::DrawHist( const char* histName, const char* opt)  {
  
  TH1* h = GetHist( histName );
  if(h) 
    h->Draw( opt );
  
  return h;
}


TH1* DataSet::GetHist( const char* histName)  {
  
  TH1* htmp = (TH1*) fileHistos_->Get(histName);

  if(!htmp) {
    cout<<"histogram : "<<histName<<" does not exist in file "<<fileHistos_->GetName()<<endl;
    return 0;
  }
  else {
    TH1* h = (TH1*)htmp->Clone( TmpHistName::Name(histName).c_str() );
    h->Scale( GetWeight() );
    return h;
  }
}



void DataSet::ExtractInfo() {
  const char* name = "runInfoAccounting/RunInfoAccounting";
  TTree* tria = (TTree*) fileRia_->Get( name );
  if(!tria) {
    cout<<"cannot get run info accounting tree named "<<name<<" in "<<fileRia_->GetName()<<endl;
    nInitial_ = -1;
    nFinal_ = -1;
    xSection_ = -1;
    genFilterEfficiency_ = -1;
    return;
  }
  else {
    
    double nEventsInFile = 0; 

    tria->SetBranchAddress("initialEvents",&nEventsInFile);
    tria->SetBranchAddress("genCrossSection",&xSection_);
    tria->SetBranchAddress("genFilterEfficiency",&genFilterEfficiency_);
    
    nInitial_ = 0;
    for(int i = 0; i < tria->GetEntries(); i++){
      tria->GetEntry(i);
      nInitial_ += nEventsInFile;
    }
    
    // mmm should average the x section... 
    
    if(xSection_ > 0){//take the MC values
      if (genFilterEfficiency_<0) {
	cout<<"warning: negative gen filter efficiency, setting it to 1"<<endl;
	genFilterEfficiency_ = 1;
      }
      PrintNormInfo();
    }
  }

  UpdateWeights();
} 


void DataSet::PrintNormInfo( std::ostream& out ) const {
  cout<<"nEvents initial : " <<nInitial_<<endl;
  cout<<"gen filter eff  : " <<genFilterEfficiency_<<endl;
  cout<<"gen xsec        : " <<xSection_<<endl;
  cout<<"Int Lumi        : " <<intLumi_<< std::endl;
  cout<<"MC Weight       : " <<GetWeight()<< std::endl;
}


void DataSet::UpdateWeights() {
  double weight = GetWeight();
  tree_->SetWeight( weight, "global"); 
}
