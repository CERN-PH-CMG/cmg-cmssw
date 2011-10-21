
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include <TH1D.h>
#include <TFile.h>

#include <iostream>
#include <string>

using namespace std;

class VertexWeightProducer : public edm::EDProducer{
 public:
  VertexWeightProducer(const edm::ParameterSet& ps);
  virtual ~VertexWeightProducer();

  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:

  TH1* getHistogram(const char* name, TFile* file);
  
  edm::InputTag src_;
  TH1D* histMC_;
  TH1D* histData_;
  TH1D* histWeight_;
  TFile* fileMC_;
  TFile* fileData_;

  bool verbose_;
};

VertexWeightProducer::VertexWeightProducer(const edm::ParameterSet& ps):
  src_(ps.getParameter<edm::InputTag>("src")), 
  verbose_(ps.getUntrackedParameter<bool>("verbose",false)) {

  TFile* fileMC_ = new TFile( ps.getParameter<std::string>("inputHistMC").c_str() );
  TFile* fileData_ = new TFile( ps.getParameter<std::string>("inputHistData").c_str() );
  
  if(fileData_->IsZombie()||fileMC_->IsZombie()){
    cout<<fileData_->GetName()<<" or "<<fileMC_->GetName()<<" not valid "<<endl;
    assert(false);
    //COLIN throw exception
  }

 
  histData_ = (TH1D*) getHistogram("pileup", fileData_);
  histMC_ = (TH1D*) getHistogram("MCPUPHisto", fileMC_);
  
  histData_->Scale( 1/histData_->Integral() );
  histMC_->Scale( 1/histMC_->Integral() );

  // truncating the histogram with the largest number of bins to the shortest. 
  //COLIN do that before or after normalization? 
  int minNbinsX = histData_->GetNbinsX() >= histMC_->GetNbinsX() ? histMC_->GetNbinsX() : histData_->GetNbinsX();

  histWeight_ = new TH1D("weight", "", minNbinsX, 0, minNbinsX);
  for(int ib = 1; ib<=minNbinsX; ++ib ) {
    histWeight_->SetBinContent( ib,  histData_->GetBinContent(ib)/histMC_->GetBinContent(ib) );
  }

  cout<<histWeight_->GetNbinsX()<<" "<<histMC_->GetNbinsX()<<" "<<histData_->GetNbinsX()<<endl;
  
  //   histWeight_->Divide( histMC_ );

  //COLIN throw exception

  produces<double>();

}


TH1* VertexWeightProducer::getHistogram(const char* name, TFile* file) {
  TH1* hist = (TH1*)file->Get( name );
  if(!hist) {
    throw cms::Exception("VertexWeightProducer")<<"histogram "<<name
						<<"doesn't exist in file "<<file->GetName();
  }
  else 
    return hist;
}


VertexWeightProducer::~VertexWeightProducer() {
  delete histWeight_;
}


void VertexWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(src_, PupInfo);
 
  double mcPUPWeight = 1.0;
  int npv=-1;

  for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
    int BX = PVI->getBunchCrossing();    
    if(BX == 0) { 
      npv = PVI->getPU_NumInteractions();
      continue;
    }    
  }
  assert(npv>-1);

  if( npv > histWeight_->GetNbinsX() ) {
    double smallWeight = 0.000000001;
    edm::LogError("VertexWeightProducer")<<"number of interactions: "<<npv<<" > the number of bins in the weight histogram: "<< histWeight_->GetNbinsX() <<". Setting weight to "<<smallWeight;
    mcPUPWeight = smallWeight;
  }
  else 
    mcPUPWeight = histWeight_->GetBinContent( npv+1 ); 
  
  if( verbose_ )
    cout<<npv<<" "<<mcPUPWeight<<" "<<histMC_->GetNbinsX()<<endl;

  std::auto_ptr<double> output( new double( mcPUPWeight ) ); 
  iEvent.put( output );

}

DEFINE_FWK_MODULE(VertexWeightProducer);

