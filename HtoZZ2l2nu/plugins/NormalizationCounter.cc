#include "CMGTools/HtoZZ2l2nu/plugins/NormalizationCounter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "FWCore/Framework/interface/Run.h"

using namespace std;

//
NormalizationCounter::NormalizationCounter(const edm::ParameterSet& iConfig) : fileChanged_(false)
{
  //book the counters
  DQMStore* store = &*edm::Service<DQMStore>();
  std::vector<std::string> ctrNames = iConfig.getParameter<std::vector<std::string> >("ctrNames");
  for(std::vector<std::string>::iterator it = ctrNames.begin(); it != ctrNames.end(); it++)
    {
      ctrs_[*it] = store->bookFloat("ric_"+*it);
      ctrs_[*it]->Fill(0);
    }
  ctrs_["nFiles"] = store->bookFloat("ric_nFiles");
  ctrs_["nFiles"]->Fill(0);
  crossSection_ = store->bookFloat("ric_crossSection");
  crossSection_->Fill(0);
  genEff_ = store->bookFloat("ric_generatorEff");
  genEff_->Fill(0);
}

//
NormalizationCounter::~NormalizationCounter()
{
}

//
void NormalizationCounter::endJob()
{
  //debug
  cout << "[NormalizationCounter][endJob]" << endl;
  for( std::map<std::string,MonitorElement *>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    cout << ctrIt->first << " " << ctrIt->second->getFloatValue() << endl;
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
	    crossSection_->Fill(crossSection_->getFloatValue()+gen->crossSection());
	    genEff_->Fill(genEff_->getFloatValue()+gen->filterEfficiency());
	  }
      }catch(std::exception &e){
      }

      ctrs_["nFiles"]->Fill(ctrs_["nFiles"]->getFloatValue()+1);
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
  for( std::map<std::string,MonitorElement *>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    {
      try{
	edm::Handle<edm::MergeableCounter> ctrHandle;
	if(!ctrHandle.isValid()) continue;
	if(iLumi.getByLabel(ctrIt->first, ctrHandle))
	  {
	    float curValue=ctrIt->second->getFloatValue();
	    float newValue=ctrHandle->value;
	    ctrIt->second->Fill( curValue+newValue );
	  }
      }catch(std::exception&e){
	cout << "[NormalizationCounter][endLuminosityBlock] failed with " << e.what() << endl;
      }
    }
}


DEFINE_FWK_MODULE(NormalizationCounter);
