#include "CMGTools/HtoZZ2l2nu/plugins/RunInfoCounter.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;

//
RunInfoCounter::RunInfoCounter(const edm::ParameterSet& iConfig)
{
  //book the counters
  DQMStore* store = &*edm::Service<DQMStore>();
  std::vector<std::string> ctrNames = iConfig.getParameter<std::vector<std::string> >("ctrs");
  for(std::vector<std::string>::iterator it = ctrNames.begin(); it != ctrNames.end(); it++)
    {
      ctrs_[*it] = store->bookFloat("ric_"+*it);
      ctrs_[*it]->Fill(0);
    }
}

//
RunInfoCounter::~RunInfoCounter()
{
}

//
void RunInfoCounter::endJob()
{
  //compute the average for cross section and generator filter efficiency
  if(totalLumis_->getFloatValue()==0) return;
  float normFactor=totalLumis_->getFloatValue();
  if(normFactor>0)
    {
      crossSection_->Fill(crossSection_->getFloatValue()/normFactor);
      genEff_->Fill(genEff_->getFloatValue()/normFactor);
    }

  //debug
  cout << "[RunInfoCounter][endJob]" << endl;
  for( std::map<std::string,MonitorElement *>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    cout << ctrIt->first << " " << ctrIt->second->getFloatValue() << endl;
  cout << "<Cross section>: " << crossSection_->getFloatValue() << endl
       << "<Filter efficiency>: " << genEff_->getFloatValue() << endl;

}

//
void RunInfoCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
 
   


   //get generator level info
   //   try{
   //     edm::Handle<GenRunInfoProduct> gen;
   //     if(aRun.getByLabel(edm::InputTag("generator"),gen))
   //       {
   //	 crossSection_->Fill(crossSection_->getFloatValue()+gen->crossSection());
   //	 genEff_->Fill(genEff_->getFloatValue()+gen->filterEfficiency());
   //       }
   //   }catch(std::exception &e){
   //   }
}

//
void RunInfoCounter::endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup)
{
  //update the counters
  for( std::map<std::string,MonitorElement *>::iterator ctrIt = ctrs_.begin();
       ctrIt != ctrs_.end();
       ctrIt++)
    {
      edm::Handle<edm::MergeableCounter> ctrHandle;
      iLumi.getByLabel(ctrIt->first, ctrHandle);
      ctrIt->second->Fill( ctrHandle->value+ctrIt->second->getFloatValue() );
    }
  if(totalLumis_)   totalLumis_->Fill(totalLumis_->getFloatValue()+1.0);
}


DEFINE_FWK_MODULE(RunInfoCounter);
