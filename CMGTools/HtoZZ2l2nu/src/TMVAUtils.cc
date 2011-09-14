#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "TMVA/MethodCategory.h"

namespace tmva
{
  //
  TMVA::MethodBase *bookMethodWithCategories(TMVA::Factory *factory, TString method,std::vector<TCut> &categDef, std::vector<TString> &categVars)
  {
    TMVA::Types::EMVA kMethod(TMVA::Types::kCuts);
    TString options("");
  
    // cut optimization
    if (method=="Cuts")    
      { kMethod = TMVA::Types::kCuts; options = "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart"; }
    if (method=="CutsD")   
      { kMethod = TMVA::Types::kCuts; options = "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate"; }
    if (method=="CutsPCA")  
      { kMethod = TMVA::Types::kCuts; options = "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA"; }
    if (method=="CutsGA")  
      { kMethod = TMVA::Types::kCuts; options = "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95"; }
    if (method=="CutsSA")   
      { kMethod = TMVA::Types::kCuts; options = "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale"; }
      
    // Likelihood 
    if (method=="Likelihood")     
      { kMethod = TMVA::Types::kLikelihood; options = "H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=30:NSmoothBkg[0]=30:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=40"; }
    if (method=="LikelihoodD")    
      { kMethod = TMVA::Types::kLikelihood; options = "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=40:VarTransform=Decorrelate"; }
    if (method=="LikelihoodPCA")  
      { kMethod = TMVA::Types::kLikelihood; options = "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA"; } 
    if (method=="LikelihoodKDE")  
      { kMethod = TMVA::Types::kLikelihood; options = "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50"; } 
    if (method=="LikelihoodMIX")  
      { kMethod = TMVA::Types::kLikelihood; options = "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50"; } 
      
    // multi-dimensional probability density estimator
    if (method=="PDERS")          
      { kMethod = TMVA::Types::kPDERS; options = "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600"; }
    if (method=="PDERSD")         
      { kMethod = TMVA::Types::kPDERS; options = "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate"; }
    if (method=="PDERSPCA")       
      { kMethod = TMVA::Types::kPDERS; options = "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA"; }
    if (method=="PDEFoam")        
      { kMethod = TMVA::Types::kPDEFoam; options = "H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0333:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T"; }
    if (method=="PDEFoamBoost")   
      { kMethod = TMVA::Types::kPDEFoam; options = "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T"; }
      
    // K-Nearest Neighbour classifier (KNN)
    if (method=="KNN")  
      { kMethod = TMVA::Types::kKNN; options = "H:nkNN=31:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim"; }
      
    // H-Matrix (chi2-squared) method
    if (method=="HMatrix")  
      { kMethod = TMVA::Types::kHMatrix; options= "!H:!V"; }
      
    // Linear discriminant (same as Fisher discriminant)
    if (method=="LD")  
      { kMethod = TMVA::Types::kLD; options = "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10"; }
      
    // Fisher discriminant (same as LD)
    if (method=="Fisher")  
      { kMethod = TMVA::Types::kFisher; options = "H:!V:Fisher:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=40:NsmoothMVAPdf=10"; }
      
    // Fisher with Gauss-transformed input variables
    if (method=="FisherG")   
      { kMethod = TMVA::Types::kFisher; options = "H:!V:VarTransform=Gauss"; }
      
    // Composite classifier: ensemble (tree) of boosted Fisher classifiers
    if (method=="BoostedFisher")   
      { kMethod = TMVA::Types::kFisher; options = "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2"; }
      
    // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
    if (method=="FDA_MC")  
      { kMethod = TMVA::Types::kFDA; options = "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1"; }
    if (method=="FDA_GA")  
      { kMethod = TMVA::Types::kFDA; options = "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1"; }
    if (method=="FDA_SA")  
      { kMethod = TMVA::Types::kFDA; options = "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale"; }
    if (method=="FDA_MT")  
      { kMethod = TMVA::Types::kFDA; options = "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch"; }
    if (method=="FDA_GAMT")  
      { kMethod = TMVA::Types::kFDA; options = "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim"; }
    if (method=="FDA_MCMT")  
      { kMethod = TMVA::Types::kFDA; options = "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20"; }
      
    // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
    if (method=="MLP")       
      { kMethod = TMVA::Types::kMLP;  options = "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator"; }
    if (method=="MLPBFGS")   
      { kMethod = TMVA::Types::kMLP; options = "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator"; }
    if (method=="MLPBNN")    
      { kMethod = TMVA::Types::kMLP; options = "H:!V:NeuronType=tanh:VarTransform=N:NCycles=500:HiddenLayers=N+3:TestRate=5:TrainingMethod=BFGS:UseRegulator"; } 
    if (method=="CFMlpANN")  
      { kMethod = TMVA::Types::kCFMlpANN; options = "!H:!V:NCycles=2000:HiddenLayers=N+1,N"; } // n_cycles:#nodes:#nodes:...  
    if (method=="TMlpANN")   
      { kMethod = TMVA::Types::kTMlpANN;  options = "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3";  } // n_cycles:#nodes:#nodes:...
    
    // Support Vector Machine
    if (method=="SVM")       
      { kMethod = TMVA::Types::kSVM; options = "Gamma=0.25:Tol=0.001:VarTransform=Norm"; }
      
    // Boosted Decision Trees
    if (method=="BDTG")  
      { kMethod = TMVA::Types::kBDT; options = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5"; }
    if (method=="BDT")   
      { kMethod =TMVA::Types::kBDT; options = "!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=20.0"; }
    if (method=="BDTB")  
      { kMethod = TMVA::Types::kBDT; options = "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=25:PruneMethod=NoPruning"; }
    if (method=="BDTD")  
      { kMethod =TMVA::Types::kBDT;  options = "!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=25:PruneMethod=CostComplexity:PruneStrength=25.0:VarTransform=Decorrelate"; }
      
    // RuleFit -- TMVA implementation of Friedman's method
    if (method=="RuleFit")  
      { kMethod = TMVA::Types::kRuleFit; options = "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02"; }
    
    //check if there is more than 1 category
    TMVA::MethodBase *fiCat=0;
    std::cout << "==> Booking: type=" << kMethod << " " <<  method << " " << options << std::endl;
    if(categDef.size()==0)
      {
	fiCat=factory->BookMethod( kMethod, method, options);
      }
    else
      {
        TString CategoryName = "Category_" + method;
        fiCat = factory->BookMethod( TMVA::Types::kCategory,CategoryName,"");
	std::cout << "\t Adding " << categDef.size() << " methods for sub-categories" << std::endl;
	TMVA::MethodCategory *mcat = dynamic_cast<TMVA::MethodCategory*>(fiCat);
	if(mcat==0)
	  std::cout << "\t Unable to cast MethodBase to MethodCategory" << std::endl; 
	else
	  for(size_t i=0; i<categDef.size(); i++)
	    {
	      TString catMethod("Category_"); catMethod +=method; catMethod += "_"; catMethod += (i+1);
	      std::cout << "\t" << categDef[i] << " " << categVars[i] << " " << catMethod << std::endl;
	      mcat->AddMethod( categDef[i], categVars[i], kMethod, catMethod, options);
	    }
      }

    return fiCat;
  }


  //
  TMVA::MethodBase *bookMethod(TMVA::Factory *factory, TString method)
  {
    std::vector<TCut> categDef;
    std::vector<TString> categVars;
    return bookMethodWithCategories(factory,method,categDef,categVars);
  }


  //
  TH1D *getHistogramForDiscriminator(TString method)
  {
    if (method=="Likelihood")    return new TH1D( method,  method + ";" + method + ";Events", 20, 0, 1 );               
    if (method=="LikelihoodD")   return new TH1D( method,  method + ";" + method + ";Events", 20, 0, 1 );
    if (method=="LikelihoodPCA") return new TH1D( method,  method + ";" + method + ";Events", 20, 0, 1 );
    if (method=="LikelihoodKDE") return new TH1D( method,  method + ";" + method + ";Events", 20, 0, 1 );
    if (method=="LikelihoodMIX") return new TH1D( method,  method + ";" + method + ";Events", 20,  0, 1 );
    if (method=="PDERS")         return new TH1D( method,  method + ";" + method + ";Events", 50,  0, 1 );
    if (method=="PDERSD")        return new TH1D( method,  method + ";" + method + ";Events", 50,  0, 1 );
    if (method=="PDERSPCA")      return new TH1D( method,  method + ";" + method + ";Events", 50,  0, 1 );
    if (method=="KNN")           return new TH1D( method,  method + ";" + method + ";Events", 50,  0, 1 );
    if (method=="HMatrix")       return new TH1D( method,  method + ";" + method + ";Events", 50, -0.95, 1.55 );
    if (method=="Fisher")        return new TH1D( method,  method + ";" + method + ";Events", 50, -4, 4 );
    if (method=="FisherG")       return new TH1D( method,  method + ";" + method + ";Events", 50, -1, 1 );
    if (method=="BoostedFisher") return new TH1D( method,  method + ";" + method + ";Events", 50, -2, 2 );
    if (method=="LD")            return new TH1D( method,  method + ";" + method + ";Events", 50, -2, 2 );
    if (method=="MLP")           return new TH1D( method,  method + ";" + method + ";Events", 50, -1.25, 1.5 );
    if (method=="MLPBFGS")       return new TH1D( method,  method + ";" + method + ";Events", 50, -1.25, 1.5 );
    if (method=="MLPBNN")        return new TH1D( method,  method + ";" + method + ";Events", 50, -1.25, 1.5 );
    if (method=="CFMlpANN")      return new TH1D( method,  method + ";" + method + ";Events", 50,  0, 1 );
    if (method=="TMlpANN")       return new TH1D( method,  method + ";" + method + ";Events", 50, -1.3, 1.3 );
    if (method=="BDT")           return new TH1D( method,  method + ";" + method + ";Events", 50, -1. , 1. );
    if (method=="BDTD")          return new TH1D( method,  method + ";" + method + ";Events", 50, -0.8, 0.8 );
    if (method=="BDTG")          return new TH1D( method,  method + ";" + method + ";Events", 50, -1.0, 1.0 );
    if (method=="RuleFit")       return new TH1D( method,  method + ";" + method + ";Events", 50, -2.0, 2.0 );
    if (method=="PDEFoam")       return new TH1D( method,  method + ";" + method + ";Events", 50,  0,   1 );
    if (method=="SVM_Gauss")     return new TH1D( method,  method + ";" + method + ";Events", 50,  0.0, 1.0 );
    if (method=="SVM_Poly")      return new TH1D( method,  method + ";" + method + ";Events", 50,  0.0, 1.0 );
    if (method=="SVM_Lin")       return new TH1D( method,  method + ";" + method + ";Events", 50,  0.0, 1.0 );
    if (method=="FDA_MT")        return new TH1D( method,  method + ";" + method + ";Events", 50, -2.0, 3.0 );
    if (method=="FDA_GA")        return new TH1D( method,  method + ";" + method + ";Events", 50, -2.0, 3.0 );
    if (method=="Category")      return new TH1D( method,  method + ";" + method + ";Events", 50, -2., 2. );
    if (method=="Plugin")        return new TH1D( method,  method + ";" + method + ";Events", 50, -0.8, 0.8 );
    return 0;
  }

}
