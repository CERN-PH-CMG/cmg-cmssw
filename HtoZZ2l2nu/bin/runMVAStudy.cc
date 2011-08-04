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
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/TransverseMassComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ProjectedMETComputer.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"

using namespace std;

//
int main(int argc, char *argv[])
{

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
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  int nJetsBin   = runProcess.getParameter<int>("nJetsBin");
  std::vector<int> evCategories = runProcess.getParameter<std::vector<int> >("evCategories");
  TString url    = runProcess.getParameter<std::string>("input");
  TString output = runProcess.getParameter<std::string>("output");
  std::vector<std::string> methodList = runProcess.getParameter<std::vector<std::string> >("methodList");
  std::vector<std::string> varsList = runProcess.getParameter<std::vector<std::string> >("varsList");
  TString selCut = runProcess.getParameter<std::string>("selCut");

  std::vector<std::string> procList = runProcess.getParameter<std::vector<std::string> >("procList");
  std::vector<int>         procType = runProcess.getParameter<std::vector<int> >("procType");
  std::vector<double>      procWeight = runProcess.getParameter<std::vector<double> >("procWeight");

  //
  // get signal and background trees
  //
  TFile *fin=TFile::Open(url);
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
  TFile* outputFile = TFile::Open( output, "RECREATE" );

  //create the factory object. 
  TString outTag(gSystem->BaseName(output));
  outTag.ReplaceAll(".root","");
  TMVA::Factory *factory = new TMVA::Factory( outTag.Data(), outputFile,"!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
  for(std::vector<std::string>::iterator it = varsList.begin(); it != varsList.end(); it++)  factory->AddVariable( *it, *it, "", 'F');

  const unsigned int nVariables = varsList.size();
  std::vector<Double_t> vars( nVariables );
  cout << "==> Start TMVAClassification with " << methodList.size() << " methods and " << nVariables << " variables" << endl;

  //reduced met computer
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
 
  // counters
  int nsigtrain(0), nsigtest(0), nbkgtrain(0), nbkgtest(0);
  ZZ2l2nuSummaryHandler evSummaryHandler;
  for(size_t iproc=0; iproc<procList.size(); iproc++)
    {
      //train proc
      evSummaryHandler.attachToTree(procTrees[iproc]);
      for (Int_t i=0; i<evSummaryHandler.getEntries(); i++) 
	{
	  evSummaryHandler.getEntry(i);
	  ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
	  PhysicsEvent_t phys=getPhysicsEventFrom(ev);

	  if(find(evCategories.begin(),evCategories.end(),ev.cat)== evCategories.end()) continue;    
	  int jetbin=ev.jn;
	  if(jetbin>2) jetbin=2;
	  if(jetbin != nJetsBin) continue;

	  //the kinematics
	  LorentzVector zll = phys.leptons[0]+phys.leptons[1];
	  double dphill     = deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
	  double drll       = deltaR(phys.leptons[0],phys.leptons[1]);
	  double mindrlz    = min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
	  double maxdrlz    = max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
	  LorentzVector zvv = phys.met[0];

	  LorentzVectorCollection jetsp4;
	  for(size_t ijet=0; ijet<phys.jets.size(); ijet++) jetsp4.push_back( phys.jets[ijet] );
	  
	  //reduced met
	  rmetComp.compute(phys.leptons[0],0,phys.leptons[1], 0, jetsp4, zvv );
	  double redmetL = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
	  double redmetT = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
	  double redmet  = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
	  
	  //update the variables
	  int varCounter(0);
	  for(std::vector<std::string>::iterator it = varsList.begin(); it != varsList.end(); it++) 
	    {
	      if(*it=="lep1pt")   vars[varCounter++] = phys.leptons[0].pt();
	      if(*it=="lep2pt")   vars[varCounter++] = phys.leptons[1].pt();    
	      if(*it=="dphill")   vars[varCounter++] = dphill;
	      if(*it=="drll")     vars[varCounter++] = drll;
	      if(*it=="mindrlz")  vars[varCounter++] = mindrlz;
	      if(*it=="maxdrlz")  vars[varCounter++] = maxdrlz;
	      if(*it=="mll")      vars[varCounter++] = zll.mass();
	      if(*it=="redmet")   vars[varCounter++] = redmet;
	      if(*it=="redmetL")  vars[varCounter++] = redmetL;
	      if(*it=="redmetT")  vars[varCounter++] = redmetT;
	    }

	  double weight=procWeight[iproc]*ev.weight;
	  if(procType[iproc]==1)
	    {
	      if ( i%2 == 0 ){ factory->AddSignalTrainingEvent( vars, weight ); nsigtrain++; }
	      else           { factory->AddSignalTestEvent    ( vars, weight ); nsigtest++; }
	    }
	  else if(procType[iproc]==2) 
	    {
	      if ( i%2 == 0 ){ factory->AddBackgroundTrainingEvent( vars, weight ); nbkgtrain++; }
	      else           { factory->AddBackgroundTestEvent    ( vars, weight ); nbkgtest++;  }
	    }
	}
    }
  cout << "\t Signal split in " << nsigtrain << " training events + " << nsigtest  << " test events" << endl
       << "\t Background split in " << nbkgtrain << " training events + " << nbkgtest  << " test events" << endl;
     
  // Set individual event weights (the variables must exist in the original TTree)
  factory->SetSignalWeightExpression    ("scale1fb");
  factory->SetBackgroundWeightExpression("scale1fb");

  //use random splitting
  factory->PrepareTrainingAndTestTree( TCut(selCut), TCut(selCut),"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );
  
  //
  // check the methods to use
  //
  // cut optimization
  if (find(methodList.begin(),methodList.end(),"Cuts") != methodList.end())   
    factory->BookMethod( TMVA::Types::kCuts, "Cuts","!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
  if (find(methodList.begin(),methodList.end(),"CutsD") != methodList.end())  
    factory->BookMethod( TMVA::Types::kCuts, "CutsD","!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );
  if (find(methodList.begin(),methodList.end(),"CutsPCA") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kCuts, "CutsPCA","!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );
  if (find(methodList.begin(),methodList.end(),"CutsGA") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kCuts, "CutsGA","H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
  if (find(methodList.begin(),methodList.end(),"CutsSA") != methodList.end())  
    factory->BookMethod( TMVA::Types::kCuts, "CutsSA","!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

  // Likelihood 
  if (find(methodList.begin(),methodList.end(),"Likelihood") != methodList.end())    
    factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood","H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=30:NSmoothBkg[0]=30:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=40" );
  if (find(methodList.begin(),methodList.end(),"LikelihoodD") != methodList.end())   
    factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD","!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=40:VarTransform=Decorrelate" );
  if (find(methodList.begin(),methodList.end(),"LikelihoodPCA") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA","!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" ); 
  if (find(methodList.begin(),methodList.end(),"LikelihoodKDE") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE","!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 
  if (find(methodList.begin(),methodList.end(),"LikelihoodMIX") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX","!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" ); 

  // multi-dimensional probability density estimator
  if (find(methodList.begin(),methodList.end(),"PDERS") != methodList.end())         
    factory->BookMethod( TMVA::Types::kPDERS, "PDERS","!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );
  if (find(methodList.begin(),methodList.end(),"PDERSD") != methodList.end())        
    factory->BookMethod( TMVA::Types::kPDERS, "PDERSD","!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );
  if (find(methodList.begin(),methodList.end(),"PDERSPCA") != methodList.end())      
    factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA","!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );
  if (find(methodList.begin(),methodList.end(),"PDEFoam") != methodList.end())       
    factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam","H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0333:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );
  if (find(methodList.begin(),methodList.end(),"PDEFoamBoost") != methodList.end())  
    factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoamBoost","!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

  // K-Nearest Neighbour classifier (KNN)
  if (find(methodList.begin(),methodList.end(),"KNN") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kKNN, "KNN","H:nkNN=31:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

  // H-Matrix (chi2-squared) method
  if (find(methodList.begin(),methodList.end(),"HMatrix") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V" );

  // Linear discriminant (same as Fisher discriminant)
  if (find(methodList.begin(),methodList.end(),"LD") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

  // Fisher discriminant (same as LD)
  if (find(methodList.begin(),methodList.end(),"Fisher") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=40:NsmoothMVAPdf=10" );

  // Fisher with Gauss-transformed input variables
  if (find(methodList.begin(),methodList.end(),"FisherG") != methodList.end())  
    factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

  // Composite classifier: ensemble (tree) of boosted Fisher classifiers
  if (find(methodList.begin(),methodList.end(),"BoostedFisher") != methodList.end())  
    factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher", "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2" );
  
  // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
  if (find(methodList.begin(),methodList.end(),"FDA_MC") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFDA, "FDA_MC","H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );
  if (find(methodList.begin(),methodList.end(),"FDA_GA") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFDA, "FDA_GA","H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );
  if (find(methodList.begin(),methodList.end(),"FDA_SA") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFDA, "FDA_SA","H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
  if (find(methodList.begin(),methodList.end(),"FDA_MT") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFDA, "FDA_MT","H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );
  if (find(methodList.begin(),methodList.end(),"FDA_GAMT") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT","H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );
  if (find(methodList.begin(),methodList.end(),"FDA_MCMT") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT","H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

  // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
  if (find(methodList.begin(),methodList.end(),"MLP") != methodList.end())      
    factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );
  if (find(methodList.begin(),methodList.end(),"MLPBFGS") != methodList.end())  
    factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );
  if (find(methodList.begin(),methodList.end(),"MLPBNN") != methodList.end())   
    factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+3:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); 
  if (find(methodList.begin(),methodList.end(),"CFMlpANN") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  
  if (find(methodList.begin(),methodList.end(),"TMlpANN") != methodList.end())  
    factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...

  // Support Vector Machine
  if (find(methodList.begin(),methodList.end(),"SVM") != methodList.end())      
    factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

  // Boosted Decision Trees
  if (find(methodList.begin(),methodList.end(),"BDTG") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kBDT, "BDTG","!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5" );
  if (find(methodList.begin(),methodList.end(),"BDT") != methodList.end())  
    factory->BookMethod(TMVA::Types::kBDT,"BDT","!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20.0");
  if (find(methodList.begin(),methodList.end(),"BDTB") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kBDT, "BDTB","!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=25:PruneMethod=NoPruning" );
  if (find(methodList.begin(),methodList.end(),"BDTD") != methodList.end()) 
    factory->BookMethod(TMVA::Types::kBDT,"BDTD","!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=25:PruneMethod=CostComplexity:PruneStrength=25.0:VarTransform=Decorrelate");

  // RuleFit -- TMVA implementation of Friedman's method
  if (find(methodList.begin(),methodList.end(),"RuleFit") != methodList.end()) 
    factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit","H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

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
