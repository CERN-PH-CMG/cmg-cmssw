
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

  edm::InputTag src_;
  TH1D* histWeight_;
  bool verbose_;
};

VertexWeightProducer::VertexWeightProducer(const edm::ParameterSet& ps):
  src_(ps.getParameter<edm::InputTag>("src")), 
  verbose_(ps.getUntrackedParameter<bool>("verbose",false)) {


  TFile fileData( ps.getParameter<std::string>("inputHistData").c_str() );  
  if(fileData.IsZombie())
    throw cms::Exception("VertexWeightProducer")<<" bad input Data file "<<fileData.GetName();

  TFile fileMC( ps.getParameter<std::string>("inputHistMC").c_str() );
  if(fileMC.IsZombie())
    throw cms::Exception("VertexWeightProducer")<<" bad input MC file "<<fileMC.GetName();
 
  TH1D* histData = (TH1D*)fileData.Get("pileup");
  if(!histData) 
    throw cms::Exception("VertexWeightProducer")<<"Data histogram doesn't exist in file "<<fileData.GetName();

  TH1D* histMC = (TH1D*)fileMC.Get("pileup");
  if(!histMC) 
    throw cms::Exception("VertexWeightProducer")<<"MC histogram doesn't exist in file "<<fileMC.GetName();


  //Normalize to 1
  histData->Scale(1./histData->Integral());
  histMC->Scale(1./histMC->Integral());

  //set binning to the one with less bins
  int nbins=histData->GetNbinsX()<histMC->GetNbinsX() ? histData->GetNbinsX() : histMC->GetNbinsX();    
  histWeight_ = new TH1D("histWeight","",nbins,-0.5,nbins-0.5);
  for(int ib = 1; ib<=nbins; ++ib ) {
    if(histMC->GetBinContent(ib)>0.0) histWeight_->SetBinContent( ib,  histData->GetBinContent(ib)/histMC->GetBinContent(ib) );
    else  histWeight_->SetBinContent( ib,0.0);
  }


  produces<double>();

}



VertexWeightProducer::~VertexWeightProducer() {
  delete histWeight_;
}


void VertexWeightProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(src_, PupInfo);
 
  double mcPUPWeight = 0.;//default weight is set to 0 in case npv is out of range
  int npv=-1;
  for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) 
    if(PVI->getBunchCrossing() == 0) npv = PVI->getPU_NumInteractions();

  if(  0<= npv && npv < histWeight_->GetNbinsX()  ) 
    mcPUPWeight = histWeight_->GetBinContent( npv+1 ); //npv=0 corresponds to bin # 1
  
  if( verbose_ )
    cout<<" npv="<<npv
	<<" weight="<<mcPUPWeight
	<<" histXmin="<<histWeight_->GetXaxis()->GetXmin()
	<<" histXmax="<<histWeight_->GetXaxis()->GetXmax()
	<<endl;

  std::auto_ptr<double> output( new double( mcPUPWeight ) ); 
  iEvent.put( output );

}

DEFINE_FWK_MODULE(VertexWeightProducer);

