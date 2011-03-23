#include "CMGTools/HtoZZ2l2nu/plugins/NormalizationCounter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "FWCore/Framework/interface/Run.h"

using namespace std;

//
NormalizationCounter::NormalizationCounter(const edm::ParameterSet& iConfig) : fileChanged_(false), nFiles_(0)
{
  //book the counters
  DQMStore* store = &*edm::Service<DQMStore>();
  std::vector<std::string> ctrNames = iConfig.getParameter<std::vector<std::string> >("ctrNames");
  for(std::vector<std::string>::iterator it = ctrNames.begin(); it != ctrNames.end(); it++)
    {
      ctrs_[*it] = store->bookFloat("ric_"+*it);
      ctrs_[*it]->Fill(0);
    }
  crossSection_ = store->bookFloat("ric_crossSection");
  crossSection_->Fill(0);
  genEff_ = store->bookFloat("ric_generatorEff");
  genEff_->Fill(0);
}

//
NormalizationCounter::~NormalizationCounter()
{
  //compute the average for cross section and generator filter efficiency
  float normFactor(nFiles_);
  if(normFactor>0)
    {
      crossSection_->Fill(crossSection_->getFloatValue()/normFactor);
      genEff_->Fill(genEff_->getFloatValue()/normFactor);
    }

  //debug
  cout << "[NormalizationCounter][endJob]" << endl;
  for( std::map<std::string,MonitorElement *>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    cout << ctrIt->first << " " << ctrIt->second->getFloatValue() << endl;
  cout << "<Cross section>: " << crossSection_->getFloatValue() << endl
       << "<Filter efficiency>: " << genEff_->getFloatValue() << endl;
}

//
void NormalizationCounter::endJob()
{
}

//
void NormalizationCounter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  if(!fileChanged_) return;
 
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

  nFiles_++;
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
	if(iLumi.getByLabel(ctrIt->first, ctrHandle))
	  {
	    ctrIt->second->Fill( ctrIt->second->getFloatValue()+ctrHandle->value );
	  }
      }catch(std::exception&e){
      }
    }
}


DEFINE_FWK_MODULE(NormalizationCounter);
