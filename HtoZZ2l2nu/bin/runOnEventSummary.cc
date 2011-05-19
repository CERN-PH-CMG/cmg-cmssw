#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

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
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }
  
  //configure
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //control histos
  TString cats[]={"ee","mumu"};
  std::map<TString , TH1D *> controlHistos;
  size_t ncats=sizeof(cats)/sizeof(TString);
  for(size_t icat=0; icat<ncats; icat++)
    {
      controlHistos[cats[icat]+"_rhovsnpu"] = (TH1D *) formatPlot( new TH2F (cats[icat]+"_rhovsnpu", ";Pileup; #rho; Events", 25, 0.,25,100, 0.,10.), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_ecalisovsnpu"] = (TH1D *) formatPlot( new TH2F (cats[icat]+"_ecalisovsnpu", ";Pileup; Isolation; Events", 25, 0.,25,100, 0.,10.), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_hcalisovsnpu"] = (TH1D *) formatPlot( new TH2F (cats[icat]+"_hcalisovsnpu", ";Pileup; Isolation; Events", 25, 0.,25,100, 0.,10.), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_caloisovsnpu"] = (TH1D *) formatPlot( new TH2F (cats[icat]+"_caloisovsnpu", ";Pileup; Isolation; Events", 25, 0.,25,100, 0.,10.), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_trackisovsnpu"] = (TH1D *) formatPlot( new TH2F (cats[icat]+"_trackisovsnpu", ";Pileup; Isolation; Events", 25, 0.,25,100, 0.,10.), 1,1,1,20,0,true,true,1,1,1 );
      
      controlHistos[cats[icat]+"_rhovsnpu_prof"] = (TH1D *) formatPlot( new TProfile (cats[icat]+"_rhovsnpu_prof", ";Pileup; <#rho>; Events", 25, 0.,25), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_ecalisovsnpu_prof"] = (TH1D *) formatPlot( new TProfile (cats[icat]+"_ecalisovsnpu_prof", ";Pileup; <Isolation>; Events", 25, 0.,25), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_hcalisovsnpu_prof"] = (TH1D *) formatPlot( new TProfile (cats[icat]+"_hcalisovsnpu_prof", ";Pileup; <Isolation>; Events", 25, 0.,25), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_caloisovsnpu_prof"] = (TH1D *) formatPlot( new TProfile (cats[icat]+"_caloisovsnpu_prof", ";Pileup; <Isolation>; Events", 25, 0.,25), 1,1,1,20,0,true,true,1,1,1 );
      controlHistos[cats[icat]+"_trackisovsnpu_prof"] = (TH1D *) formatPlot( new TProfile (cats[icat]+"_trackisovsnpu_prof", ";Pileup; <Isolation>; Events", 25, 0.,25), 1,1,1,20,0,true,true,1,1,1 );
    }

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
	    case 11: case -11: case 13: case -13:
	      TString cat="ee";
	      if(ev.id[ipart]==13 || ev.id[ipart]==-13) cat="mumu";
	      TString histos[]={"","_prof"};
	      for(size_t ihisto=0; ihisto<2; ihisto++)
		{
		  controlHistos[cat+"_rhovsnpu"+histos[ihisto]]->Fill(ev.ngenpu,ev.rho);
		  controlHistos[cat+"_ecalisovsnpu"+histos[ihisto]]->Fill(ev.ngenpu,ev.info2[ipart]);
		  controlHistos[cat+"_hcalisovsnpu"+histos[ihisto]]->Fill(ev.ngenpu,ev.info3[ipart]);
		  controlHistos[cat+"_caloisovsnpu"+histos[ihisto]]->Fill(ev.ngenpu,ev.info2[ipart]+ev.info3[ipart]); 
		  controlHistos[cat+"_trackisovsnpu"+histos[ihisto]]->Fill(ev.ngenpu,ev.info4[ipart]);
		}
	      leptons.push_back( PhysicsObject(p4,ev.info1[ipart]) );
	      break;
	    }
	}
      
      if(mets[0].first.pt() > 100.) {
	//fit reduced met
	std::auto_ptr<RooFitResult> fitResults = rmet.compute(leptons[0].first,leptons[0].second,
							      leptons[1].first,leptons[1].second,
							      jetsp4,
							      mets[0].first);
	cout << "-------------------" << endl;
	cout << "RedMET: " << rmet.reducedMET().first << " +/- " << rmet.reducedMET().second << endl;
	cout << "RedMET_long: " << rmet.reducedMET_long().first << " +/- " << rmet.reducedMET_long().second << endl;
	cout << "RedMET_perp: " << rmet.reducedMET_perp().first << " +/- " << rmet.reducedMET_perp().second << endl;
	cout << "jetRecoil_perp: " << rmet.jetRecoil_perp().first << " +/- " << rmet.jetRecoil_perp().second << endl;
	cout << "jetRecoil_long: " << rmet.jetRecoil_long().first << " +/- " << rmet.jetRecoil_long().second << endl;

	cout << "NLL: " << fitResults->minNll() << " status: " << fitResults->status() << endl;
      
	//dump event info
	cout << "[" << ev.run << ":" << ev.lumi << ":" << ev.event << "]" << endl
	     << "Lepton #1: (" << leptons[0].first.pt() << ";" << leptons[0].first.eta() << ";" << leptons[0].first.phi() << ")" << endl  
	     << "Lepton #2: (" << leptons[1].first.pt() << ";" << leptons[1].first.eta() << ";" << leptons[1].first.phi() << ")" << endl;
	for(size_t ijet=0; ijet<jets.size(); ijet++)
	  cout << "Jet    #" << (ijet+1) << ": (" << jets[ijet].first.pt() << ";" << jets[ijet].first.eta() << ";" << jets[ijet].first.phi() << ")" << endl; 
	cout << "MET      : (" << mets[0].first.pt() << ";" << mets[0].first.phi() << ")" << endl;
      }
    }
  file->Close();

  
  //save to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  cout << outUrl << endl;
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  for( size_t icat=0; icat<ncats; icat++)
    {
      baseOutDir->cd();
      baseOutDir->mkdir( cats[icat] )->cd();
      for(std::map<TString,TH1D *>::iterator hIt = controlHistos.begin(); hIt != controlHistos.end(); hIt++)
        {
          if(!hIt->first.BeginsWith(cats[icat])) continue;
          hIt->second->Write();
        }
    }
  ofile->Close();
}  
