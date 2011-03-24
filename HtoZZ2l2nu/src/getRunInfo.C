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
	  if(it->name == "ric_startCounter")           evInfo[INITIAL_EVTS] += it->object;
	  else if(it->name == "ric_endCounter" )       evInfo[FINAL_EVTS] += it->object;
	  else if(it->name == "ric_crossSection" )     evInfo[XSECTION] += it->object;
	  else if(it->name == "ric_filterEfficiency" ) evInfo[GENERATOR_EFF] += it->object;
	}
    }

  //take the average for cross section and filter efficiency...
  if(nRuns>0)
    {
      evInfo[XSECTION] /= nRuns;
      evInfo[GENERATOR_EFF] /= nRuns;
    }
     
  //return summary
  return evInfo;
}

