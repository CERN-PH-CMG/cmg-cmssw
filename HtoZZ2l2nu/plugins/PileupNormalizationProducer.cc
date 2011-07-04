#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TRandom.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

class PileupNormalizationProducer : public edm::EDProducer {
public:
  PileupNormalizationProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;

private:
  TH1D *puUnfoldedH_;
  bool integerWeightsOnly_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
{
  produces<float>("puWeight");

  //renormalize distributions
  integerWeightsOnly_ = iConfig.getParameter<bool>("integerWeightsOnly");
  std::vector<double> mc   = iConfig.getParameter<std::vector<double> >("mcDistribution");
  std::vector<double> data = iConfig.getParameter<std::vector<double> >("dataDistribution");
  TH1D *dataH = new TH1D("datah","datah",25,0,25);
  TH1D *mcH = new TH1D("mch","mch",25,0,25);
  for(size_t i=0; i<data.size();i++)
    {
      dataH->SetBinContent(i+1,data[i]);
      mcH->SetBinContent(i+1,mc[i]);
    }
  dataH->Scale(1./dataH->Integral());
  mcH->Scale(1./mcH->Integral());

  //compute the weights
  puUnfoldedH_ = (TH1D *)dataH->Clone("puunfoldedweights");
  puUnfoldedH_->Divide(mcH);

  //renormalize to the maximum weight
  if(integerWeightsOnly_)
    {
      float maxweight(0);
      for(int ibin=1; ibin<=puUnfoldedH_->GetXaxis()->GetNbins(); ibin++)
	{
	  float iwgt=puUnfoldedH_->GetBinContent(ibin);
	  if(iwgt<maxweight) continue;
	  maxweight=iwgt;
	}
      if(maxweight>0)
	{
	  for(int ibin=1; ibin<=puUnfoldedH_->GetXaxis()->GetNbins(); ibin++)
	    {
	      float newwgt=puUnfoldedH_->GetBinContent(ibin)/maxweight;
	      puUnfoldedH_->SetBinContent(ibin,newwgt);
	    }
	}
    }

  cout << "pileup re-weighting:  {";
  for(int ibin=1; ibin<=puUnfoldedH_->GetXaxis()->GetNbins(); ibin++)
    cout << puUnfoldedH_->GetBinContent(ibin) << ", ";
  cout << "};" << endl;
}

//
void PileupNormalizationProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;

  auto_ptr<float> puWeight(new float);

  if(iEvent.isRealData())  *puWeight=1.0;
  else
    {

      //get the number of generated pileup events
      edm::Handle<std::vector<PileupSummaryInfo> > puInfoH;
      iEvent.getByType(puInfoH);
      int npuevents(0);
      if(puInfoH.isValid())
	{
	  for(std::vector<PileupSummaryInfo>::const_iterator it = puInfoH->begin(); it != puInfoH->end(); it++)
	    {
	      if(it->getBunchCrossing() !=0) continue;
	      npuevents=it->getPU_NumInteractions();
	    }
	}

      float weight = puUnfoldedH_->GetBinContent( puUnfoldedH_->GetXaxis()->FindBin(npuevents) );
      if(integerWeightsOnly_)
	{
	  float randWeight=gRandom->Uniform();
	  if(randWeight>weight) weight=0;
	  else                  weight=1;
	}
      *puWeight=weight;

    }

  iEvent.put(puWeight,"puWeight");
}

//  
DEFINE_FWK_MODULE(PileupNormalizationProducer);
