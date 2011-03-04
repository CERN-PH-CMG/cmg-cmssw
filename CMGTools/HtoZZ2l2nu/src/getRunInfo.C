#include "CMGTools/HtoZZ2l2nu/interface/getRunInfo.h"

using namespace std;

//
std::vector<float> getRunInfo(TFile *f)
{
  std::vector<float> evInfo(4,0);

  //get the info on the number of events/generator info for each run
  fwlite::Run r(f);
  unsigned int nRuns(0);
  for( ;r.isValid();++r) 
    {
      nRuns++;
      edm::Handle<MEtoEDM<double> > hist;
      try{
	r.getByLabel(edm::InputTag("MEtoEDMConverter","MEtoEDMConverterRun"),hist);
      }catch(std::exception &e){
	cout << "[getRunInfo] " << e.what() << endl;
      }
      
      const MEtoEDM<double>::MEtoEdmObjectVector objects = hist->getMEtoEdmObject();
      for(MEtoEDM<double>::MEtoEdmObjectVector::const_iterator it = objects.begin(); it != objects.end(); ++it )
	{
	  if(it->name == "initialEvents") evInfo[INITIAL_EVTS] += it->object;
	  else if(it->name == "finalEvents" ) evInfo[FINAL_EVTS] += it->object;
	  else continue;
	}

      //get generator level cross section
      edm::Handle<GenRunInfoProduct> genInfo;
      try{
	r.getByLabel(edm::InputTag("generator"),genInfo);
	evInfo[XSECTION] += genInfo->crossSection();
	evInfo[GENERATOR_EFF] += genInfo->filterEfficiency();
      }catch(std::exception &e){
	cout << "[getRunInfo] " << e.what() << endl;
      }
    }

  //take the average for cross section ...
  if(nRuns>0)
    {
      evInfo[XSECTION] /= nRuns;
      evInfo[GENERATOR_EFF] /= nRuns;
    }
  


  //return summary
  return evInfo;
}
