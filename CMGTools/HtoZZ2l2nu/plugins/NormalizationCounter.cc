#include "CMGTools/HtoZZ2l2nu/plugins/NormalizationCounter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "FWCore/Framework/interface/Run.h"

using namespace std;

//
NormalizationCounter::NormalizationCounter(const edm::ParameterSet& iConfig) : fileChanged_(false)
{
  //book the counters
  std::vector<std::string> ctrNames = iConfig.getParameter<std::vector<std::string> >("ctrNames");
  for(std::vector<std::string>::iterator it = ctrNames.begin(); it != ctrNames.end(); it++)
    {
      ctrs_[*it]=0;
    }
  ctrs_["nFiles"] = 0;
  ctrs_["crossSection"] = 0;
  ctrs_["generatorEff"] = 0;
}

//
NormalizationCounter::~NormalizationCounter()
{
}

//
void NormalizationCounter::endJob()
{
  DQMStore* store = &*edm::Service<DQMStore>();

  //debug
  cout << "[NormalizationCounter][endJob]" << endl;
  for( std::map<std::string,float>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    {
      cout << ctrIt->first << " " << ctrIt->second << endl;
      MonitorElement *h=store->bookFloat("ric_"+ctrIt->first);
      h->Fill(ctrIt->second);
    }
}

//
void NormalizationCounter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  if(!fileChanged_) return;
  try
    {
      //get generator level info   
      const edm::Run &run = iEvent.getRun();
      try{
	edm::Handle<GenRunInfoProduct> gen;
	if(run.getByLabel(edm::InputTag("generator"),gen))
	  {
	    ctrs_["crossSection"]+=gen->crossSection();
	    ctrs_["generatorEff"]+=gen->filterEfficiency();
	  }
      }catch(std::exception &e){
      }
      ctrs_["nFiles"]+=1;
    }catch(std::exception &e){
    cout << "[NormalizationCounter][analyze] failed with " << e.what() << endl;
  }
  fileChanged_=false;
}


//
void NormalizationCounter::respondToOpenInputFile(edm::FileBlock const& fb) 
{
  fileChanged_ = true;
}

//
void NormalizationCounter::respondToCloseInputFile(edm::FileBlock const& fb) 
{
}


//
void NormalizationCounter::beginLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
}

//
void NormalizationCounter::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  //update the counters
  for( std::map<std::string,float>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    {
      try{
	edm::Handle<edm::MergeableCounter> ctrHandle;
	if(iLumi.getByLabel(ctrIt->first, ctrHandle))
	  {
	    float newValue=ctrHandle->value;
	    ctrs_[ctrIt->first]+=newValue;
	  }
      }catch(std::exception&e){
	cout << "[NormalizationCounter][endLuminosityBlock] failed with " << e.what() << endl;
      }
    }
}


DEFINE_FWK_MODULE(NormalizationCounter);
