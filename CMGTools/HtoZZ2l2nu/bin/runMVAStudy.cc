#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;

//
int main(int argc, char *argv[])
{
  //init computers
  EventCategory eventClassifComp;

  //load fwlite
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  if ( argc < 2 ) 
    {
      std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
      return 0;
    }

  //
  //configure      
  //
  const edm::ParameterSet &cfg = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  edm::ParameterSet runProcess = cfg.getParameter<edm::ParameterSet>("tmvaInput");
  std::vector<int> evCategories = runProcess.getParameter<std::vector<int> >("evCategories");
  TString url    = runProcess.getParameter<std::string>("input");
  TString studyTag = runProcess.getParameter<std::string>("studyTag");
  std::vector<std::string> methodList = runProcess.getParameter<std::vector<std::string> >("methodList");
  std::vector<std::string> varsList = runProcess.getParameter<std::vector<std::string> >("varsList");
  //variables for method definition (for now all methods share the same variables: todo split per category)
  TString varsString("");
  for(std::vector<std::string>::iterator it = varsList.begin(); it != varsList.end(); it++)
    {
      if(it != varsList.begin()) varsString += ":";
      varsString += *it;
    }
  std::vector<TString> categVars(evCategories.size(),varsString);

  //selection cuts for exclusive categories
  std::vector<TCut> categDef(evCategories.size());
  for(size_t i=0; i<evCategories.size(); i++)
    {
      TString catDef("eventCategory=="); catDef += i;
      categDef[i]=TCut(catDef);
    }

  std::vector<std::string> procList = runProcess.getParameter<std::vector<std::string> >("procList");
  std::vector<int>         procType = runProcess.getParameter<std::vector<int> >("procType");
  std::vector<double>      procWeight = runProcess.getParameter<std::vector<double> >("procWeight");

  //
  // get signal and background trees
  //
  cout << url << endl;
  TFile *fin=TFile::Open(url);
  if(fin==0) return -1;
  std::vector<TTree *> procTrees;
  for(size_t iproc=0; iproc<procList.size(); iproc++)  
    {
      TString tname(procList[iproc]+"/data");
      procTrees.push_back( (TTree *) fin->Get(tname) );
    }
  cout << "==> Retrieved " << procTrees.size() << " inputs for study" << endl;

  //  
  //load TMVA and define methods 
  //
  TMVA::Tools::Instance();

  //output for TMVA objects
  TFile* outputFile = TFile::Open( studyTag + ".root", "RECREATE" );

  //create the factory object. 
  TMVA::Factory *factory = new TMVA::Factory( studyTag.Data(), outputFile,"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
  for(std::vector<std::string>::iterator it = varsList.begin(); it != varsList.end(); it++)  factory->AddVariable( *it, *it, "", 'F');
  factory->AddSpectator( "eventCategory" );

  const unsigned int nVariables = varsList.size()+1;
  std::vector<Double_t> tmvaVars( nVariables );
  cout << "==> Start TMVAClassification with " << methodList.size() << " methods x " << evCategories.size() << " categories and " << nVariables-1 << " variables" << endl;

  // counters
  int nsigtrain(0), nsigtest(0), nbkgtrain(0), nbkgtest(0);
  ZZ2l2nuSummaryHandler evSummaryHandler;
  for(size_t iproc=0; iproc<procList.size(); iproc++)
    {
      //train proc
      cout << procTrees[iproc] << endl;
      evSummaryHandler.attachToTree(procTrees[iproc]);
      for (Int_t i=0; i<evSummaryHandler.getEntries(); i++) 
	{
	  evSummaryHandler.getEntry(i);
	  ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
	  PhysicsEvent_t phys=getPhysicsEventFrom(ev);
	  LorentzVectorCollection jetsP4;
	  for(size_t ijet=0; ijet<phys.jets.size(); ijet++) jetsP4.push_back( phys.jets[ijet] );
	  
	  //consider only ee/mumu
	  if(ev.cat!=1 && ev.cat!=2) continue;

	  //require loose wp at least
	  //	  if(ev.pass==0) continue;

	  //classify the event
	  int eventCategory= eventClassifComp.Get(phys);
	  
	  //z+met kinematics
	  LorentzVector zll=phys.leptons[0]+phys.leptons[1];
	  LorentzVector zvv=phys.met[0];
	  Float_t dphill     = deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
	  Float_t detall     = phys.leptons[0].eta()-phys.leptons[1].eta();
	  Float_t drll       = deltaR(phys.leptons[0],phys.leptons[1]);
	  Float_t mindrlz    = min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
	  Float_t maxdrlz    = max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
	  Float_t ptl1       = phys.leptons[0].pt();
	  Float_t ptl2       = phys.leptons[1].pt();
	  Float_t ptsum      = ptl1+ptl2;
	  Float_t mtl1       = METUtils::transverseMass(phys.leptons[0],zvv,false);
	  Float_t mtl2       = METUtils::transverseMass(phys.leptons[1],zvv,false);
	  Float_t mtsum      = mtl1+mtl2;
	  Float_t zmass      = zll.mass();
	  Float_t zpt        = zll.pt();
	  Float_t zeta       = zll.eta();
	  Float_t met        = zvv.pt();
	  Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
	  Float_t mt         = METUtils::transverseMass(zll,zvv,true);
	  Float_t metoverzpt = met/zpt;
          Float_t dphizleadl = ptl1>ptl2 ? deltaPhi(phys.leptons[0].phi(),zll.phi()) : deltaPhi(phys.leptons[1].phi(),zll.phi()) ;
  
	  //redmet
          METUtils::stRedMET redMetInfo;
          LorentzVector redMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, phys.leptons[0], 0, phys.leptons[1], 0, jetsP4             , zvv                , false, &redMetInfo);
	  Float_t redMet         = redMetP4.pt();
	  Float_t redMetL        = redMetInfo.redMET_l;
	  Float_t redMetT        = redMetInfo.redMET_t;
	  Float_t redMetoverzpt  = redMet/zpt;
	  Float_t redMetLplusZpt = redMetL+zpt;

	  //projected met
	  Float_t projMet        =  METUtils::projectedMET(phys.leptons[0], phys.leptons[1], zvv).pt();
	  Float_t projMetoverzpt = projMet/zpt;
	  
	  //update the variables
	  int varCounter(0);
	  for(std::vector<std::string>::iterator it = varsList.begin(); it != varsList.end(); it++) 
	    {
	      if(*it=="dphill")         tmvaVars[varCounter++]=dphill;
	      if(*it=="detall")         tmvaVars[varCounter++]=detall;
	      if(*it=="drll")           tmvaVars[varCounter++]=drll;
	      if(*it=="mindrlz")        tmvaVars[varCounter++]=mindrlz;
	      if(*it=="maxdrlz")        tmvaVars[varCounter++]=maxdrlz;
	      if(*it=="ptl1")           tmvaVars[varCounter++]=ptl1;
	      if(*it=="ptl2")           tmvaVars[varCounter++]=ptl2;
	      if(*it=="ptsum")          tmvaVars[varCounter++]=ptsum;
	      if(*it=="mtl1")           tmvaVars[varCounter++]=mtl1;
	      if(*it=="mtl2")           tmvaVars[varCounter++]=mtl2;
	      if(*it=="mtsum")          tmvaVars[varCounter++]=mtsum;
	      if(*it=="mt")             tmvaVars[varCounter++]=mt;
	      
	      if(*it=="zmass")          tmvaVars[varCounter++]=zmass;
	      if(*it=="zpt")            tmvaVars[varCounter++]=zpt;
	      if(*it=="zeta")           tmvaVars[varCounter++]=zeta;
	      if(*it=="met")            tmvaVars[varCounter++]=met;
	      if(*it=="dphizz")         tmvaVars[varCounter++]=dphizz;
              if(*it=="mt")             tmvaVars[varCounter++]=mt;
	      if(*it=="metoverzpt")     tmvaVars[varCounter++]=metoverzpt;
              if(*it=="dphizleadl")     tmvaVars[varCounter++]=dphizleadl;
	  
	      if(*it=="redMet")         tmvaVars[varCounter++]=redMet;
	      if(*it=="redMetL")        tmvaVars[varCounter++]=redMetL;
	      if(*it=="redMetT")        tmvaVars[varCounter++]=redMetT;
	      if(*it=="redMetoverzpt")  tmvaVars[varCounter++]=redMetoverzpt;
	      if(*it=="redMetLplusZpt")  tmvaVars[varCounter++]=redMetLplusZpt;
	      if(*it=="projMet")        tmvaVars[varCounter++]=projMet;
	      if(*it=="projMetoverzpt") tmvaVars[varCounter++]=projMetoverzpt;
	    }
	  tmvaVars[varCounter++] = eventCategory;

	  double weight=procWeight[iproc]*ev.puWeight;
	  if(procType[iproc]==1)
	    {
	      if ( i%2 == 0 ){ factory->AddSignalTrainingEvent( tmvaVars, weight ); nsigtrain++; }
	      else           { factory->AddSignalTestEvent    ( tmvaVars, weight ); nsigtest++; }
	    }
	  else if(procType[iproc]==2) 
	    {
	      if ( i%2 == 0 ){ factory->AddBackgroundTrainingEvent( tmvaVars, weight ); nbkgtrain++; }
	      else           { factory->AddBackgroundTestEvent    ( tmvaVars, weight ); nbkgtest++;  }
	    }
	}
    }
  cout << "\t Signal split in " << nsigtrain << " training events + " << nsigtest  << " test events" << endl
       << "\t Background split in " << nbkgtrain << " training events + " << nbkgtest  << " test events" << endl;
   
  //use random splitting
  TCut selCut( categDef.size()==1 ?  categDef[0] : "");
  factory->PrepareTrainingAndTestTree(selCut, selCut,"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

  //
  // book the methods to use (for the moment all variables are passed to all category methods - > needs change)
  //
  for(size_t i=0; i<methodList.size(); i++)  tmva::bookMethodWithCategories(factory,methodList[i],categDef,categVars);

  //
  // now train, test and evaluate
  //
  std::cout << "\t Start training" << std::endl;
  factory->TrainAllMethods();
  std::cout << "\t Start test" << std::endl;
  factory->TestAllMethods();
  std::cout << "\t Start evaluate" << std::endl;
  factory->EvaluateAllMethods();

  std::cout << "\t Results available in: " << outputFile->GetName() << std::endl;
  std::cout << "\t runMVAStudy is done!" << std::endl;

  // close files and release memory
  outputFile->Close();
  fin->Close();
  delete factory;
}
