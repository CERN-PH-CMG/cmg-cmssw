#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

typedef std::pair<LorentzVector,float> PhysicsObject; 
typedef std::vector<PhysicsObject> PhysicsObjectCollection;

using namespace std;

//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //start the reduced met fitter
  ReducedMETFitter rmet(runProcess);
  
  //open the file and get directory
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  ZZ2l2nuSummaryHandler evSummaryHandler;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }

  //check run range
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=evSummaryHandler.getEntries();
  if(evStart > evEnd ) 
    {
      file->Close();
      return -1;
    }
  
  //run the analysis
  for( int iev=evStart; iev<evEnd; iev++)
    {
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();

      //temp
      if(ev.pfmet_pt<150) continue;

      //get the physics object
      PhysicsObjectCollection leptons, jets, mets;
      LorentzVectorCollection jetsp4;
      for(Int_t ipart=0; ipart<ev.nparticles; ipart++)
	{
	  LorentzVector p4(ev.px[ipart],ev.py[ipart],ev.pz[ipart],ev.en[ipart]);
	  switch( ev.id[ipart] )
	    {
	    case 0:
	      mets.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    case 1:
	      jetsp4.push_back(p4);
	      jets.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    default:
	      leptons.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    }
	}
      
      //fit reduced met
      rmet.compute(leptons[0].first,leptons[0].second,
      		   leptons[1].first,leptons[1].second,
		   jetsp4,
		   mets[0].first);
      
      //dump event info
      cout << "[" << ev.run << ":" << ev.lumi << ":" << ev.event << "]" << endl
	   << "Lepton #1: (" << leptons[0].first.pt() << ";" << leptons[0].first.eta() << ";" << leptons[0].first.phi() << ")" << endl  
	   << "Lepton #2: (" << leptons[1].first.pt() << ";" << leptons[1].first.eta() << ";" << leptons[1].first.phi() << ")" << endl;
      for(size_t ijet=0; ijet<jets.size(); ijet++)
	cout << "Jet    #" << (ijet+1) << ": (" << jets[ijet].first.pt() << ";" << jets[ijet].first.eta() << ";" << jets[ijet].first.phi() << ")" << endl; 
      cout << "MET      : (" << ev.pfmet_pt << ";" << ev.pfmet_phi << ")" << endl;
    }
}  
