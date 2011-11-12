#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"

#include "TRandom.h"

class PileupNormalizationProducer : public edm::EDProducer 
{
public:

  PileupNormalizationProducer(const edm::ParameterSet &iConfig) ;
  virtual void produce( edm::Event &iEvent, const edm::EventSetup &iSetup) ;
  
private:

  edm::LumiReWeighting LumiWeights_;
  double maxWeight_;
  bool use3D_;
};


using namespace std;


//
PileupNormalizationProducer::PileupNormalizationProducer(const edm::ParameterSet &iConfig)
  :  LumiWeights_(iConfig.getParameter<std::string>("mc"),  iConfig.getParameter<std::string>("data"), "pileup","pileup"),
     use3D_(false)
{
  produces<float>("puWeight");
  produces<float>("renPuWeight");

  use3D_ = iConfig.getParameter<bool>("use3D");

  std::string puWeightFile=iConfig.getParameter<std::string>("puWeightFile");

  maxWeight_=0;
  if(use3D_)
    {
      if(puWeightFile.size()==0) LumiWeights_.weight3D_init();
      else                        LumiWeights_.weight3D_init(puWeightFile);
      for(int nm1=0; nm1<=30; nm1++)
	for(int n0=0; n0<=30; n0++)
	  for(int np1=0; np1<=30; np1++)
	    maxWeight_ = max( LumiWeights_.weight3D( nm1,n0,np1) , maxWeight_);
    }
  else
    {
      for(int n0=0; n0<=30; n0++)
	maxWeight_ = max( LumiWeights_.weight(n0) , maxWeight_);
      
    }
  
  cout << "[PileupNormalizationProducer] max wgt: " << maxWeight_ << endl;
}

//
void PileupNormalizationProducer::produce(edm::Event &iEvent, const edm::EventSetup &iSetup) 
{
  using namespace std;
  using namespace edm;

  auto_ptr<float> puWeight(new float);
  auto_ptr<float> renPuWeight(new float);

  if(iEvent.isRealData())  
    {
      *puWeight=1.0;
      *renPuWeight=1.0;
    }
  else
    {
      //get the number of generated pileup events
      edm::Handle<std::vector<PileupSummaryInfo> > PupInfo;
      iEvent.getByType(PupInfo);
      
      int nm1 = -1; int n0 = -1; int np1 = -1;
      for(std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) 
	{
	  int BX = PVI->getBunchCrossing();
	  if(BX == -1) nm1 += PVI->getPU_NumInteractions();
	  if(BX == 0)  n0 += PVI->getPU_NumInteractions();
	  if(BX == 1)  np1 += PVI->getPU_NumInteractions();
	}
      
      float weight = use3D_ ? LumiWeights_.weight3D( nm1,n0,np1) : LumiWeights_.weight(n0) ;
      float renWeight=1.0;
      if(maxWeight_>0)
	{
	  float randWeight=gRandom->Uniform();
          if(randWeight>weight/maxWeight_) renWeight=0;
	}
      *puWeight = weight;
      *renPuWeight = renWeight;
    }

  //put in the event
  iEvent.put(puWeight,"puWeight");
  iEvent.put(renPuWeight,"renPuWeight");
}

//  
DEFINE_FWK_MODULE(PileupNormalizationProducer);
