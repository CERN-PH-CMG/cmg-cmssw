
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/TransverseMassComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ProjectedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/JetEnergyUncertaintyComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/DuplicatesChecker.h"

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
#include "TEventList.h"

#include "Math/GenVector/Boost.h"
 
using namespace std;

//
TH1F *getHistogramForVariable(TString variable)
{
  TH1F *h=0;
  if(variable=="dphill")         h = new TH1F( variable, ";#Delta#phi(l^{(1)},l^{(2)});Events",100,-3.2,3.2);
  if(variable=="dphizleadl")     h = new TH1F( variable, ";#Delta#phi(l^{(1)},Z);Events / (0.1 rad)",15,0,1.5);
  if(variable=="detall")         h = new TH1F( variable, ";#Delta#eta(l^{(1)},l^{(2)});Events",100,-5,5);
  if(variable=="drll")           h = new TH1F( variable, ";#DeltaR(l^{(1)},l^{(2)}) / (0.1 rad);Events",20,0,6);
  if(variable=="mindrlz")        h = new TH1F( variable, ";min #DeltaR(l,Z);Events",100,0,6);
  if(variable=="maxdrlz")        h = new TH1F( variable, ";max #DeltaR(l,Z);Events",100,0,6);
  if(variable=="ptl1")           h = new TH1F( variable, ";p_{T}(l^{(1)});Events", 100,0,1000);
  if(variable=="ptl2")           h = new TH1F( variable, ";p_{T}(l^{(2)});Events", 100,0,1000);
  if(variable=="ptsum")          h = new TH1F( variable, ";#Sigma p_{T}(l^{(i)});Events", 100,0,1000);
  if(variable=="mtl1")           h = new TH1F( variable, ";M_{T}(l^{(1)},E_{T}^{miss});Events", 100,0,1000);
  if(variable=="mtl2")           h = new TH1F( variable, ";M_{T}(l^{(2)},E_{T}^{miss});Events", 100,0,1000);
  if(variable=="mtsum")          h = new TH1F( variable, ";#Sigma M_{T}(l^{(i)},E_{T}^{miss});Events / 20 GeV/c^{2}", 50,0,1000);

  if(variable=="zmass")          h = new TH1F( variable, ";M^{ll};Events", 100,91-31,91+31);
  if(variable=="zpt")            h = new TH1F( variable, ";p_{T}^{ll};Events", 100,0,400);
  if(variable=="zeta")           h = new TH1F( variable, ";#eta^{ll};Events", 100,-5,5);
  if(variable=="met")            h = new TH1F( variable, ";E_{T}^{miss};Events", 100,0,500);
  if(variable=="dphizz")         h = new TH1F( variable, ";#Delta#phi(Z_{ll},E_{T}^{miss});Events",100,-3.2,3.2);
  if(variable=="metoverzpt")     h = new TH1F( variable, ";type I E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9);


  if(variable=="redMetoverzpt")  h = new TH1F( variable,  ";red-E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9);

  if(variable=="projMet")        h = new TH1F( variable, ";projected E_{T}^{miss};Events", 100,0,500);
  if(variable=="projMetoverzpt") h = new TH1F( variable, ";projected E_{T}^{miss}/p_{T}(Z);Events", 100,-1,9);

  return h;
}


double VBFreweighting (double decay_width, double m_gen, double mass){

	double weight_BW;
	double s = pow(mass,2);
	weight_BW = s/pow(m_gen,2) * (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width,2)) / (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width*s/(pow(m_gen,2)),2));

	return(weight_BW);
}

LorentzVector min(const LorentzVector& a, const LorentzVector& b){
   if(a.pt()<=b.pt())return a;
   return b;
}

//
int main(int argc, char* argv[])
{
  SelectionMonitor controlHistos; //plot storage

  //start computers
  ProjectedMETComputer pmetComp;
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rAmetComp(1., 1., 1., 1., 1.);
  //ReducedMETFitter rmetFitter(runProcess);
  TransverseMassComputer mtComp;
  EventCategory eventClassifComp;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }


  std::map<double,double> SampleHiggsWidth;    std::map<string,double> SampleHiggsWeightInt;
  SampleHiggsWidth[130]=   0.00487;            SampleHiggsWeightInt["VBFtoH130toZZto2L2Nu.root"]=1.000001;   SampleHiggsWeightInt["VBFtoH130toWWto2L2Nu.root"]=0.999999;
  SampleHiggsWidth[140]=   0.00812;            SampleHiggsWeightInt["VBFtoH140toZZto2L2Nu.root"]=1.000000;   SampleHiggsWeightInt["VBFtoH140toWWto2L2Nu.root"]=1.000000;
  SampleHiggsWidth[150]=   0.0173;             SampleHiggsWeightInt["VBFtoH150toZZto2L2Nu.root"]=0.999996;   SampleHiggsWeightInt["VBFtoH150toWWto2L2Nu.root"]=1.000004;
  SampleHiggsWidth[160]=   0.0829;             SampleHiggsWeightInt["VBFtoH160toZZto2L2Nu.root"]=1.000000;   SampleHiggsWeightInt["VBFtoH160toWWto2L2Nu.root"]=1.000000;
  SampleHiggsWidth[170]=   0.38;               SampleHiggsWeightInt["VBFtoH170toZZto2L2Nu.root"]=1.000359;   SampleHiggsWeightInt["VBFtoH170toWWto2L2Nu.root"]=1.000212;
  SampleHiggsWidth[180]=   0.63;               SampleHiggsWeightInt["VBFtoH180toZZto2L2Nu.root"]=1.000000;   SampleHiggsWeightInt["VBFtoH180toWWto2L2Nu.root"]=1.000000;
  SampleHiggsWidth[190]=   1.04;               SampleHiggsWeightInt["VBFtoH190toZZto2L2Nu.root"]=1.000000;   SampleHiggsWeightInt["VBFtoH190toWWto2L2Nu.root"]=1.000000;
  SampleHiggsWidth[200]=   1.43;               SampleHiggsWeightInt["VBFtoH200toZZto2L2Nu.root"]=0.999749;   SampleHiggsWeightInt["VBFtoH200toWWto2L2Nu.root"]=0.999183;
  SampleHiggsWidth[300]=   8.43;               SampleHiggsWeightInt["VBFtoH300toZZto2L2Nu.root"]=0.987308;   SampleHiggsWeightInt["VBFtoH300toWWto2L2Nu.root"]=0.989306;
  SampleHiggsWidth[400]=  29.2;                SampleHiggsWeightInt["VBFtoH400toZZto2L2Nu.root"]=0.951238;   SampleHiggsWeightInt["VBFtoH400toWWto2L2Nu.root"]=0.950328;
  SampleHiggsWidth[500]=  68;                  SampleHiggsWeightInt["VBFtoH500toZZto2L2Nu.root"]=0.881467;   SampleHiggsWeightInt["VBFtoH500toWWto2L2Nu.root"]=0.883517;
  SampleHiggsWidth[600]= 123;                  SampleHiggsWeightInt["VBFtoH600toZZto2L2Nu.root"]=0.792347;   SampleHiggsWeightInt["VBFtoH600toWWto2L2Nu.root"]=0.807353;



  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");

  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);

  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //jet description
  TString etaFileName = runProcess.getParameter<std::string>("etaResolFileName"); gSystem->ExpandPathName(etaFileName);
  JetResolution stdEtaResol(etaFileName.Data(),false);
  TString phiFileName = runProcess.getParameter<std::string>("phiResolFileName"); gSystem->ExpandPathName(phiFileName);
  JetResolution stdPhiResol(phiFileName.Data(),false);
  TString ptFileName  = runProcess.getParameter<std::string>("ptResolFileName");  gSystem->ExpandPathName(ptFileName);
  JetResolution stdPtResol(ptFileName.Data(),true);
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());
  jet::UncertaintyComputer jcomp(&stdPtResol,&stdEtaResol,&stdPhiResol,&jecUnc);
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  if(runSystematics) { cout << "Systematics will be computed for this analysis" << endl; }

  //TMVA configuration
  TMVA::Reader *tmvaReader = 0;
  bool useMVA                             = runProcess.getParameter<bool>("useMVA");
  edm::ParameterSet tmvaInput             = runProcess.getParameter<edm::ParameterSet>("tmvaInput");
  std::vector<std::string> methodList     = tmvaInput.getParameter<std::vector<std::string> >("methodList");
  std::vector<std::string> varsList       = tmvaInput.getParameter<std::vector<std::string> >("varsList");
  std::vector<int> evCategories           = tmvaInput.getParameter<std::vector<int> >("evCategories");
  std::string weightsDir                  = tmvaInput.getParameter<std::string>("weightsDir");
  std::string studyTag                    = tmvaInput.getParameter<std::string>("studyTag");

  std::vector<Float_t> discriResults(methodList.size(),0);
  Float_t pdeFoamError(0), pdeFoamSig(0), fisherProb(0), fisherRarity(0);
  std::vector<Float_t> tmvaVars(varsList.size()+1,0);

  //control histograms for variables of interest are booked by default
  //   for(size_t ivar=0; ivar<varsList.size(); ivar++) 
  //     {
  //       TH1 *h=getHistogramForVariable( varsList[ivar] );
  //       controlHistos.addHistogram( h );
  //     }
  if(useMVA)
    {
      std::cout << "==> Start TMVA Classification with " << methodList.size() << " methods and " << varsList.size() << " variables" << std::endl;

      //start the reader for the variables and methods
      tmvaReader = new TMVA::Reader( "!Color:!Silent" );
      for(size_t ivar=0; ivar<varsList.size(); ivar++)   tmvaReader->AddVariable( varsList[ivar], &tmvaVars[ivar] );
      tmvaReader->AddSpectator("eventCategory", &tmvaVars[varsList.size()]);

      //book the methods
      for(size_t imet=0; imet<methodList.size(); imet++)
	{
          //open the file with the method description
	  TString weightFile = weightsDir + "/" + studyTag + ( evCategories.size()>1 ? "_Category_" + methodList[imet] : "") + TString(".weights.xml");
          gSystem->ExpandPathName(weightFile);

	  tmvaReader->BookMVA(methodList[imet], weightFile);
	  TH1 *h=tmva::getHistogramForDiscriminator( methodList[imet] );
	  controlHistos.addHistogram( h );
	  controlHistos.addHistogram( (TH1 *) h->Clone( h->GetName()+TString("tight") ) );
	  if(methodList[imet]=="PDEFoam") 
	    {
	      controlHistos.addHistogram( new TH1F( "PDEFoam_Err",    "PDEFoam error",        100,  0, 1 ) );
	      controlHistos.addHistogram( new TH1F( "PDEFoam_Sig",    "PDEFoam significance", 100,  0, 10 ) );
	    }
	  else if(methodList[imet]=="Fisher") 
	    {
	      controlHistos.addHistogram( new TH1F( "Fisher_Proba",  "Fisher_Proba",  100, 0, 1 ) );
	      controlHistos.addHistogram( new TH1F( "Fisher_Rarity", "Fisher_Rarity", 100, 0, 1 ) );
	    }
	}
    }
  
  
  //book the control histograms
  TH1F *h=new TH1F ("eventflow", ";Step;Events", 8,0,8);
  h->GetXaxis()->SetBinLabel(2,"Preselected");
  h->GetXaxis()->SetBinLabel(3,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(4,"Z_{pT}>25");
  h->GetXaxis()->SetBinLabel(5,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(6,"b-veto");
  h->GetXaxis()->SetBinLabel(7,"red-E_{T}^{miss}>medium");
  h->GetXaxis()->SetBinLabel(8,"red-E_{T}^{miss}>tight");
  controlHistos.addHistogram( h );

  h=new TH1F ("eventCategory", ";Event Category;Events", 4,0,4);
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
  h->GetXaxis()->SetBinLabel(4,"VBF");
  controlHistos.addHistogram( h );

  //vertex
  controlHistos.addHistogram( new TH1F("nvtx",";Vertices;Events",25,0,25) );

  //dilepton control 
  controlHistos.addHistogram( getHistogramForVariable("zeta") );
  controlHistos.addHistogram( getHistogramForVariable("zpt") );
  controlHistos.addHistogram( new TH1F("zrank",";Z hardness;Events",5,0,5) );
  h=(TH1F *)getHistogramForVariable("zmass");
  controlHistos.addHistogram( h );
  h=new TH1F ("zmassregionCtr", ";Selection region;Events", 18,0,18);
  h->GetXaxis()->SetBinLabel(1, "Z");
  h->GetXaxis()->SetBinLabel(2, "Z,b");
  h->GetXaxis()->SetBinLabel(3, "Z,b,=2l");
  h->GetXaxis()->SetBinLabel(4, "Z,b,=2l,medium");
  h->GetXaxis()->SetBinLabel(5, "Z,b,=2l,tight");
  h->GetXaxis()->SetBinLabel(6, "~Z");
  h->GetXaxis()->SetBinLabel(7, "~Z,b");
  h->GetXaxis()->SetBinLabel(8, "~Z,b,=2l");
  h->GetXaxis()->SetBinLabel(9, "~Z,b,=2l,medium");
  h->GetXaxis()->SetBinLabel(10,"~Z,b,=2l,tight");
  h->GetXaxis()->SetBinLabel(11,"Z,~b");
  h->GetXaxis()->SetBinLabel(12,"Z,~b,=2l");
  h->GetXaxis()->SetBinLabel(13,"Z,~b,=2l,medium");
  h->GetXaxis()->SetBinLabel(14,"Z,~b,=2l,tight");
  h->GetXaxis()->SetBinLabel(15,"~Z,~b");
  h->GetXaxis()->SetBinLabel(16,"~Z,~b,=2l");
  h->GetXaxis()->SetBinLabel(17,"~Z,~b,=2l,medium");
  h->GetXaxis()->SetBinLabel(18,"~Z,~b,=2l,tight");
  controlHistos.addHistogram( h );

  controlHistos.addHistogram( getHistogramForVariable("dphill") );
  controlHistos.addHistogram( getHistogramForVariable("drll") );
  controlHistos.addHistogram( getHistogramForVariable("dphizleadl") );
  controlHistos.addHistogram( getHistogramForVariable("mindrlz") );
  controlHistos.addHistogram( getHistogramForVariable("maxdrlz") );
  controlHistos.addHistogram( getHistogramForVariable("ptsum") );
  controlHistos.addHistogram( getHistogramForVariable("mtsum") );
  controlHistos.addHistogram( getHistogramForVariable("mtl1") );
  controlHistos.addHistogram( getHistogramForVariable("mtl2") );
  controlHistos.addHistogram( getHistogramForVariable("mt") );

  //object multiplicity
  h=new TH1F("nleptons",";Leptons;Events",3,0,3);
  h->GetXaxis()->SetBinLabel(1,"=2 leptons");
  h->GetXaxis()->SetBinLabel(2,"=3 leptons");
  h->GetXaxis()->SetBinLabel(3,"#geq 4 leptons");
  controlHistos.addHistogram( h );
  
  controlHistos.addHistogram(new TH1F("njets",";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  controlHistos.addHistogram( new TH1F("njets3leptons",";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  controlHistos.addHistogram( new TH2F ("njetsvspu", ";Pileup events;Jets;Events", 25,0,25,5,0,5) );  
  controlHistos.addHistogram(  new TH2F ("njetsincvspu", ";Pileup events;Jets;Events", 25,0,25,5,0,5) );   
  for(size_t ibin=1; ibin<=5; ibin++)
    {
      TString label("");
      if(ibin==5) label +="#geq";
      else        label +="=";
      label += (ibin-1);
      controlHistos.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
      controlHistos.getHisto("njets3leptons")->GetXaxis()->SetBinLabel(ibin,label);
      controlHistos.getHisto("njetsvspu")->GetXaxis()->SetBinLabel(ibin,label);
      controlHistos.getHisto("njetsincvspu")->GetXaxis()->SetBinLabel(ibin,label);
    }

 
  h = new TH1F ("nbtags", ";b-tag multiplicity; Events", 3,0,3);  
  h->GetXaxis()->SetBinLabel(1,"0 b-tags");
  h->GetXaxis()->SetBinLabel(2,"1 b-tags");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 b-tags");
  controlHistos.addHistogram( h );
  controlHistos.addHistogram( new TH1F ("nbtags3leptons", ";b-tag multiplicity; Events", 3,0,3) );

  //used to optimize the b-veto
  h=new TH1F ("npassbveto", ";Pass b-tag veto; Events", 6,0,6);
  h->GetXaxis()->SetBinLabel(1,"SSVHEM");
  h->GetXaxis()->SetBinLabel(2,"TCHEL");
  h->GetXaxis()->SetBinLabel(3,"TCHE>2");
  h->GetXaxis()->SetBinLabel(4,"JBP");
  h->GetXaxis()->SetBinLabel(5,"SSVHEM || TCHEL");
  h->GetXaxis()->SetBinLabel(6,"SSVHEM || JBPL");
  controlHistos.addHistogram( h );


  //used for GenLevel
  controlHistos.addHistogram( new TH1F( "genHiggsPt"  , ";gen Higgs p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genHiggsMass", ";gen Higgs Mass ;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genHiggsMassRaw", ";gen Higgs Mass Before VBF reweighting;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genzllPt"  , ";gen Zll p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzvvPt"  , ";gen Zvv p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzwinHiggsPt"  , ";gen Higgs p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzwinHiggsMass", ";gen Higgs Mass ;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genzwinHiggsMassRaw", ";gen Higgs Mass Before VBF reweighting;Events",100,0,600) );
  controlHistos.addHistogram( new TH1F( "genzwinzllPt"  , ";gen Zll p_{T};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "genzwinzvvPt"  , ";gen Zvv p_{T};Events",100,0,200) );

  controlHistos.addHistogram( new TH1F( "CMzllP"  , ";CM Zll p};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "CMzvvP"  , ";CM Zvv p};Events",100,0,200) );
  controlHistos.addHistogram( new TH1F( "CMDeltazP"  , ";CM Zll p - Zvv p;Events",200,-200,200) );
  controlHistos.addHistogram( new TH1F( "CMiMass"  , ";CM Zll p - Zvv p;Events",100,0,600) );

  controlHistos.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",100,0,3.4) );
  controlHistos.addHistogram( new TH1F( "minmtjmet", ";min M_{T}(jet,E_{T}^{miss}) [GeV/c^{2}];Events",100,0,250) );
  controlHistos.addHistogram( new TH1F( "mindrjz", ";min #DeltaR(jet,Z);Events",100,0,6) );
  controlHistos.addHistogram( new TH1F( "minmjz", ";min M(jet,Z) [GeV/c^{2}];Events",100,0,500) );

  //met control
  std::map<TString,TString> metTypes;
  metTypes["met"]                 = "E_{T}^{miss}";
//  metTypes["projMet"]             = "proj-E_{T}^{miss}";
//  metTypes["minProjMet"]          = "min-proj{E_{T}^{miss},assoc E_{T}^{miss} (charged)}";
  metTypes["redMet"]              = "red-E_{T}^{miss}";
//  metTypes["redMetD0"]            = "red-E_{T}^{miss}(D0)";
  metTypes["assocChargedMet"]     = "assoc E_{T}^{miss} (charged)";
  metTypes["minAssocChargedMet"]  = "min{E_{T}^{miss},assoc E_{T}^{miss} (charged)}";
  metTypes["centralMet"]          = "central-E_{T}^{miss}";
//  metTypes["minCentralMet"]       = "min{E_{T}^{miss},central-E_{T}^{miss}}";
//  metTypes["assocOtherVertexMet"] = "assoc-E_{T}^{miss} (other vtx)";
//  metTypes["cleanMet"]            = "clean-E_{T}^{miss}";
//  metTypes["minCleanMet"]         = "min{E_{T}^{miss},clean-E_{T}^{miss}}";
  metTypes["assocMet"]            = "assoc-E_{T}^{miss}";
  metTypes["minAssocMet"]         = "min{E_{T}^miss,assoc-E_{T}^{miss})";
//  metTypes["superMinMet"]         = "min{E_{T}^{miss},assoc-E_{T}^{miss},clean-E_{T}^{miss},central-E_{T}^{miss}}";
  metTypes["redMinAssocMet"]         = "red{min{E_{T}^miss,assoc-E_{T}^{miss}}, unclustered E_{T}^{miss})";
  metTypes["assocMet5"]            = "assoc-E_{T}^{miss} (pT>5)";
  metTypes["assocMet10"]           = "assoc-E_{T}^{miss} (pT>10)";
  metTypes["minAssocFwdMet"]         = "min{E_{T}^miss,assoc-E_{T}^{miss} + Fwd)";
  metTypes["assocFwdMet"]            = "assoc-E_{T}^{miss} + Fwd";
  metTypes["assocFwdMet5"]            = "assoc-E_{T}^{miss} + Fwd (pT>5)";
  metTypes["assocFwdMet10"]           = "assoc-E_{T}^{miss} + Fwd (pT>10)";

  std::map<TString,LorentzVector> metTypeValues;
  for(std::map<TString,TString>::iterator it = metTypes.begin(); it!= metTypes.end(); it++)
    {
      metTypeValues[it->first]=LorentzVector(0,0,0,0);
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first, ";"+it->second+";Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"vspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
//      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"phimetphijet", ";#phi "+it->second+";#phi jet", 10,0,3.15,10,0,3.15) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first+"mindphijmet", ";min #Delta#phi(jet,"+it->second+");Events",10,0,3.15) );
      controlHistos.addHistogram( new TH1F( TString("metL_") + it->first, ";Long: "+it->second+";Events", 160,-300,500) );
      controlHistos.addHistogram( new TH2F( TString("metL_") + it->first+"vspu", ";Pileup events;Long: "+it->second+";Events", 25,0,25,160,-300,500) );  
      controlHistos.addHistogram( new TH1F( TString("metT_") + it->first, ";Trans: "+it->second+";Events", 160,-300,500) );
//      controlHistos.addHistogram( new TH2F( TString("metT_") + it->first+"vspu", ";Pileup events;Trans :"+it->second+";Events", 25,0,25,160,-300,500) );
//      controlHistos.addHistogram( new TH2F( TString("metLT_") + it->first, ";Long: " +it->second+";Trans: " + it->second, 160,-300,500,160,-300,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first + "zpt", ";" +it->second+"; z p_{T}", 40,0,200,40,0,200) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "minzpt", ";min(z p_{T}, "  +it->second+");Events", 100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "geq080zpt", ";"+it->second+">0.8 z p_{T};Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"geq080zptvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "geq060zpt", ";"+it->second+">0.6 z p_{T};Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"geq060zptvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "geq040zpt", ";"+it->second+">0.4 z p_{T};Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"geq040zptvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );

      controlHistos.addHistogram( new TH2F( TString("met_") + it->first + "pfmet", ";" +it->second+"; pfmet", 40,0,200,40,0,200) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "leq120pfmet", ";"+it->second+"<1.2 pfmet;Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"leq120pfmetvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "leq140pfmet", ";"+it->second+"<1.4 pfmet;Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"leq140pfmetvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
      controlHistos.addHistogram( new TH1F( TString("met_") + it->first + "leq160pfmet", ";"+it->second+"<1.6 z pfmet;Events", 100,0,500) );
      controlHistos.addHistogram( new TH2F( TString("met_") + it->first+"leq160pfmetvspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
    }
  controlHistos.addHistogram( new TH2F ("itpuvsootpu", ";In-Time Pileup; Out-of-time Pileup;Events", 30,0,30,30,0,30) );
  controlHistos.addHistogram( new TH2F ("redMetcomps", ";red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 50, -251.,249,50, -251.,249.) );
  controlHistos.addHistogram( new TH1F( "redMetL", ";red-E_{T}^{miss,#parallel};Events", 100,-250,250) );
  controlHistos.addHistogram( new TH1F( "redMetT", ";red-E_{T}^{miss,#perp};Events", 100,-250,250) );
  controlHistos.addHistogram( new TH2F ("metvstkmet", ";E_{T}^{miss};tk-E_{T}^{miss}", 50,0,500,50,0,500) );  
  controlHistos.addHistogram( new TH2F ("metvsclusteredMet", ";E_{T}^{miss};clustered-E_{T}^{miss}", 50,0,500,50,0,500) );  
  controlHistos.addHistogram( new TH2F ("metvscentralMet", ";E_{T}^{miss};central-E_{T}^{miss}", 50,0,500,50,0,500) );  
  
  controlHistos.addHistogram( new TH1F("sumEt",                    ";#Sigma E_{T} [GeV];Events", 100,0,2000) );
  controlHistos.addHistogram( new TH1F("chSumEt",                    ";#Sigma E_{T}^{charged} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("neutSumEt",                    ";#Sigma E_{T}^{neutral} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("primVertexSumEt",          ";Primary Vertex : #Sigma E_{T} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("otherVertexSumEt",         ";Other Vertices : #Sigma_{vtx} #Sigma E_{T} [GeV];Events", 100,0,1000) );
  controlHistos.addHistogram( new TH1F("chSumEtFrac",              ";#Sigma E_{T}^{charged}/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("neutSumEtFrac",            ";#Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );

  TString ootConds[]={"LowOOTpu","MediumOOTpu","HighOOTpu","VeryHighOOTpu"};
  for(size_t i=0; i<sizeof(ootConds)/sizeof(TString); i++)
    {
      controlHistos.addHistogram( new TH1F("sumEt"+ootConds[i],           ";#Sigma E_{T} [GeV];Events", 100,0,2000) );
      controlHistos.addHistogram( new TH1F("neutSumEtFrac"+ootConds[i],   ";#Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );
    }

  controlHistos.addHistogram( new TH1F("centralSumEtFrac",         ";#Sigma E_{T}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("centralChSumEtFrac",       ";#Sigma E_{T}^{charged}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("centralNeutSumEtFrac",     ";#Sigma E_{T}^{neutral}(|#eta|<2.4)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("chPrimVertexSumEtFrac",    ";#Sigma E_{T}^{charged}(PV)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("neutPrimVertexSumEtFrac",  ";#Sigma E_{T}^{neutral}(PV)/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("chOtherVertexSumEtFrac",   ";#Sigma_{oth.vtx} #Sigma E_{T}^{charged}/#Sigma E_{T};Events", 100,0,1.) );
  controlHistos.addHistogram( new TH1F("neutOtherVertexSumEtFrac", ";#Sigma_{oth.vtx} #Sigma E_{T}^{neutral}/#Sigma E_{T};Events", 100,0,1.) );

  //cut optimization plots
  controlHistos.addHistogram( new TH1F ("cutOptMediumdphill", ";#delta #phi^{l,l};#red-E_{T}^{miss,#parallel};",25,0.,3.2) );
  controlHistos.addHistogram( new TH2F ("cutOptMediumsummtvsredMetL", ";#Sigma M_{T};#red-E_{T}^{miss,#parallel};",50,0,2000,25, -251.,249) );
  
  //cut and count
  std::vector<TString> varsToInclude;
  varsToInclude.push_back("");
  varsToInclude.push_back("zpt25");
  if(runSystematics)
    {
      varsToInclude.push_back("jer");
      varsToInclude.push_back("jesup");
      varsToInclude.push_back("jesdown");
      varsToInclude.push_back("hfactup");
      varsToInclude.push_back("hfactdown");
      varsToInclude.push_back("hrenup");
      varsToInclude.push_back("hrendown");
      varsToInclude.push_back("flatpu");
    }
  for(size_t ivar=0; ivar<varsToInclude.size(); ivar++)
    {
      h = new TH1F (varsToInclude[ivar]+"finaleventflow",";Category;Event count;",8,0,8);
      h->GetXaxis()->SetBinLabel(1,"m=130");
      h->GetXaxis()->SetBinLabel(2,"m=150");
      h->GetXaxis()->SetBinLabel(3,"m=170");
      h->GetXaxis()->SetBinLabel(4,"m=200");
      h->GetXaxis()->SetBinLabel(5,"m=300");
      h->GetXaxis()->SetBinLabel(6,"m=400");
      h->GetXaxis()->SetBinLabel(7,"m=500");
      h->GetXaxis()->SetBinLabel(8,"m=600");
      controlHistos.addHistogram( h );
    }
   
  
  //VBF
  h = new TH1F ("VBFNEventsInc", ";Selection cut;Events", 15,0,15);
  h->GetXaxis()->SetBinLabel(1,"All");
  h->GetXaxis()->SetBinLabel(2,"2Jets (30GeV)");
  h->GetXaxis()->SetBinLabel(3,"dEta");
  h->GetXaxis()->SetBinLabel(4,"inv. Mass");
  h->GetXaxis()->SetBinLabel(5,"Central lepton");
  h->GetXaxis()->SetBinLabel(6,"Jet Veto");
  h->GetXaxis()->SetBinLabel(7,"BJet Veto");
  controlHistos.addHistogram( h );
  controlHistos.addHistogram( new TH1F ("VBFdEtaInc", ";#Delta#eta", 100, 0,10) );
  controlHistos.addHistogram( new TH1F ("VBFiMassInc",";Inv. Mass", 100, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFcenLeptonVetoInc",";Central Lepton Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFcen30JetVetoInc",";Central 30 Jet Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFNBJets30Inc", ";N BJets (pT>30);Events", 10,0,10) );
  controlHistos.addHistogram( new TH1F ("VBFdEtaNM1C", ";#Delta#eta", 100, 0,10) );
  controlHistos.addHistogram( new TH1F ("VBFiMassNM1C",";Inv. Mass", 100, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFcenLeptonVetoNM1C",";Central Lepton Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFcen30JetVetoNM1C",";Central 30 Jet Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFNBJets30NM1C", ";N BJets (pT>30);Events", 10,0,10) );
  controlHistos.addHistogram( new TH2F ("VBFdEtaiMassNM1C", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  controlHistos.addHistogram( new TH2F ("VBFdEtaiMassNM1C2", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFdEta3rdlepton", ";#Delta#eta", 100, 0,10) );
  controlHistos.addHistogram( new TH1F ("VBFiMass3rdlepton",";Inv. Mass", 100, 0.,2000) );
  controlHistos.addHistogram( new TH1F ("VBFcen30JetVeto3rdlepton",";Central 30 Jet Veto", 10, 0.,10) );
  controlHistos.addHistogram( new TH1F ("VBFNBJets303rdlepton", ";N BJets (pT>30);Events", 10,0,10) );

  //replicate monitor for interesting categories
//  TString cats[]={"ee","emu","mumu"};
  TString cats[]={"ee","mumu"};
  TString subCats[]={"","eq0jets","eq1jets","geq2jets","vbf"};
  //TString subCats[]={"","1to3vtx","4to9vtx","geq10vtx"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    for(size_t isubcat=0;isubcat<sizeof(subCats)/sizeof(TString); isubcat++)
      controlHistos.initMonitorForStep(cats[icat]+subCats[isubcat]);
  
  //open the file and get events tree
  ZZ2l2nuSummaryHandler evSummaryHandler;
  TFile *file = TFile::Open(url);
  printf("Looping on %s\n",url.Data());
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }

  //check run range to compute scale factor (if not all entries are used)
  const Int_t totalEntries= evSummaryHandler.getEntries();
  float rescaleFactor( evEnd>0 ?  float(totalEntries)/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=totalEntries;
  if(evStart > evEnd ) 
    {
      file->Close();
      return -1;
    }

  //check PU normalized entries 
  evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  TEventList *elist = (TEventList*)gDirectory->Get("elist");
  const Int_t normEntries = (elist==0 ? 0 : elist->GetN()); 
  if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;

  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/mumu/mumu_cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      if(rescaleFactor>0) cnorm /= rescaleFactor;
    }
  cout << "xSec x Br=" << xsec << " total entries=" << totalEntries << " normalized PU entries=" << normEntries << " obtained from " << cnorm << " generated events" 
       << " => L=" << cnorm/xsec << " /pb" << endl; 
  
  //Event Duplicates checker init
  DuplicatesChecker duplicatesChecker;

  double VBFWEIGHTINTEGRAL = 0;

    
  // init summary tree (unweighted events for MVA training) 
  bool saveSummaryTree = runProcess.getParameter<bool>("saveSummaryTree");
  if(!isMC) saveSummaryTree=false; //temporary for now
  TFile *spyFile=0;
  TDirectory *spyDir=0;
  ofstream *outf=0;
  ZZ2l2nuSummaryHandler *spyHandler=0;
  float summaryWeight(1);
  if(saveSummaryTree && normEntries>0)
    {
      summaryWeight = xsec * float(totalEntries) / (cnorm * float(normEntries) );
      spyHandler = new ZZ2l2nuSummaryHandler;
      spyFile = TFile::Open(outUrl + "/EventSummaries.root","UPDATE");
      TString evtag=gSystem->BaseName(url);
      evtag.ReplaceAll(".root","");
      spyFile->rmdir(evtag);
      spyDir = spyFile->mkdir(evtag);
      TTree *outT = evSummaryHandler.getTree()->CloneTree(0);
      outT->SetDirectory(spyDir);
      spyHandler->initTree(outT,false);
    }  
  if(!isMC) outf=new ofstream("highmetevents.txt",ios::app);  
    
  
  //run the analysis
  unsigned int NumberOfDuplicated = 0;
  for( int iev=evStart; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();

      //uncomment if you are suspicious
      //       if(duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event)){
      //            //printf("event %i-%i-%i is duplicated\n", ev.run, ev.lumi, ev.event);
      //            NumberOfDuplicated++;
      //            continue;
      //       }

      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      float weight=ev.weight;
      if(!isMC) weight=1;
      else if(ev.hptWeights[0]>1e-6) weight *= ev.hptWeights[0];
      double VBFWeight=1.0;

      string FileName = string(url.Data());
      size_t VBFStringpos = FileName.find("VBF");
      if(VBFStringpos!=string::npos){
        string StringMass = FileName.substr(VBFStringpos+6,3);
        double SampleHiggsMass;
        sscanf(StringMass.c_str(),"%lf",&SampleHiggsMass);
        if(iev==evStart)printf("###\nSAMPLE MASS IS %f --> Weight=%f\n###\n",SampleHiggsMass,SampleHiggsWeightInt[FileName.substr(VBFStringpos)]);
        VBFWeight = VBFreweighting(SampleHiggsWidth[SampleHiggsMass], SampleHiggsMass, phys.genhiggs[0].mass() )  / SampleHiggsWeightInt[FileName.substr(VBFStringpos)];
        weight*= VBFWeight;
        VBFWEIGHTINTEGRAL += VBFWeight;
      }
  
      //
      //event categories
      //
      TString evcat("");
      if(ev.cat==dilepton::EMU)  continue;//evcat="emu";
      if(ev.cat==dilepton::MUMU) evcat="mumu";
      if(ev.cat==dilepton::EE)   evcat="ee";
     
      int eventCategory = eventClassifComp.Get(phys);
      TString subcat    = eventClassifComp.GetLabel(eventCategory);

      //MC truth
      LorentzVector genzll(0,0,0,0), genzvv(0,0,0,0), genhiggs(0,0,0,0);
      if(isMC && phys.genleptons.size()){
         genzll=phys.genleptons[0]+phys.genleptons[1];
         genzvv=phys.genmet[0];
         genhiggs = phys.genhiggs[0];
      }

      //start analysis by the jet kinematics so that one can loop over JER/JES/b-tag systematic variations
      LorentzVector metP4=phys.met[0];
      LorentzVector assocChargedMetP4=phys.met[1];
      LorentzVector assocMetP4=phys.met[3];
      LorentzVector centralMetP4=phys.met[5];
      LorentzVector cleanMetP4=phys.met[6];
      LorentzVector assocOtherVertexMetP4=phys.met[7];
      LorentzVector assocFwdMetP4=phys.met[8];
      LorentzVector assocMet5P4=phys.met[9];
      LorentzVector assocFwdMet5P4=phys.met[10];
      LorentzVector assocMet10P4=phys.met[11];
      LorentzVector assocFwdMet10P4=phys.met[12];

      //count jets and b-tags
      int njets(0),njetsinc(0);
      int nbtags(0),        nbtags_ssvhem(0),        nbtags_tchel(0),        nbtags_tche2(0),        nbtags_jbpl(0),        nbtags_ssvhemORtchel(0);
      LorentzVectorCollection jetsP4;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) 
	{
	  jetsP4.push_back( phys.jets[ijet] );
	  njetsinc++;
	  if(fabs(phys.jets[ijet].eta())<2.5)
	    {
	      njets++;
	      bool passTCHEL(phys.jets[ijet].btag1>1.7);
	      bool passTCHE2(phys.jets[ijet].btag1>2.0);
	      bool passJBPL(phys.jets[ijet].btag2>1.33);
	      bool passSSVHEM(phys.jets[ijet].btag3>1.74);
	      if(phys.jets[ijet].pt()>30)
		{
		  nbtags          += (passJBPL || passSSVHEM);
		  nbtags_ssvhem   += passSSVHEM;
		  nbtags_tchel    += passTCHEL;
		  nbtags_tche2    += passTCHE2;
		  nbtags_jbpl     += passJBPL;
		  nbtags_ssvhemORtchel += (passTCHEL || passSSVHEM);
		}
	    }
	}

      //z+met kinematics
      LorentzVector zll  = phys.leptons[0]+phys.leptons[1];
      LorentzVector zvv  = metP4;

      Float_t dphill     = deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      Float_t detall     = phys.leptons[0].eta()-phys.leptons[1].eta();
      Float_t drll       = deltaR(phys.leptons[0],phys.leptons[1]);
      Float_t mindrlz    = min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t maxdrlz    = max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t ptl1       = phys.leptons[0].pt();
      Float_t ptl2       = phys.leptons[1].pt();
      Float_t ptsum      = ptl1+ptl2;
      Float_t mtl1       = mtComp.compute(phys.leptons[0],zvv,false);
      Float_t mtl2       = mtComp.compute(phys.leptons[1],zvv,false);
      Float_t mtsum      = mtl1+mtl2;
      Float_t zmass      = zll.mass();
      Float_t zpt        = zll.pt();
      Float_t zeta       = zll.eta();
      Float_t met        = zvv.pt();
      Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
      Float_t mt         = mtComp.compute(zll,zvv,true);
      Float_t dphizleadl = ptl1>ptl2 ? deltaPhi(phys.leptons[0].phi(),zll.phi()) : deltaPhi(phys.leptons[1].phi(),zll.phi()) ;

      //redmet
      rmetComp.compute(phys.leptons[0],0,phys.leptons[1], 0, jetsP4, zvv );
      Float_t redMet_d0  = rmetComp.reducedMET(ReducedMETComputer::D0);
      Float_t redMetL_d0  = rmetComp.reducedMETComponents(ReducedMETComputer::D0).second;
      Float_t redMetT_d0  = rmetComp.reducedMETComponents(ReducedMETComputer::D0).first;
      Float_t redMetX_d0  = rmetComp.reducedMETcartesian(ReducedMETComputer::D0).X();
      Float_t redMetY_d0  = rmetComp.reducedMETcartesian(ReducedMETComputer::D0).Y();
      Float_t redMet         = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t redMetL        = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t redMetT        = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t redMetX        = rmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t redMetY        = rmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      rAmetComp.compute(phys.leptons[0],0,phys.leptons[1], 0, jetsP4, min(zvv,assocMetP4) );
      Float_t redminAssocMet    = rAmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t redminAssocMetL   = rAmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t redminAssocMetT   = rAmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t redminAssocMetX   = rAmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t redminAssocMetY   = rAmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();



 
      //projected met
      Float_t projMet        = pmetComp.compute(phys.leptons[0], phys.leptons[1], zvv );
      Float_t projAssocChargedMet     = pmetComp.compute(phys.leptons[0],phys.leptons[1], assocChargedMetP4);
      
      Float_t cleanMet            = cleanMetP4.pt();
      Float_t centralMet          = centralMetP4.pt();
      Float_t assocChargedMet     = assocChargedMetP4.pt();
      Float_t assocMet            =  assocMetP4.pt();
      Float_t assocOtherVertexMet = assocOtherVertexMetP4.pt();

              //met control
              metTypeValues["met"]                 = zvv;
              metTypeValues["projMet"]             = LorentzVector(projMet,0,0,0);
              metTypeValues["minProjMet"]          = min(metTypeValues["projMet"],LorentzVector(projAssocChargedMet,0,0,0) );
              metTypeValues["redMet"]              = LorentzVector(redMetX,redMetY,0,redMet); 
              metTypeValues["redMetD0"]            = LorentzVector(redMetX_d0, redMetY_d0, 0, redMet_d0);
              metTypeValues["assocChargedMet"]     = assocChargedMetP4;
              metTypeValues["minAssocChargedMet"]  = min(zvv,assocChargedMetP4);
              metTypeValues["centralMet"]          = centralMetP4;
              metTypeValues["minCentralMet"]       = min(zvv,centralMetP4);    
              metTypeValues["assocOtherVertexMet"] = assocOtherVertexMetP4;    
              metTypeValues["cleanMet"]            = cleanMetP4;
              metTypeValues["minCleanMet"]         = min(zvv,cleanMetP4);
              metTypeValues["assocMet"]            = assocMetP4;
              metTypeValues["minAssocMet"]         = min(zvv,assocMetP4);
              metTypeValues["superMinMet"]         = min( metTypeValues["minAssocMet"],  min(metTypeValues["minCleanMet"], metTypeValues["centralMet"]) );
              metTypeValues["redMinAssocMet"]      = LorentzVector(redminAssocMetX,redminAssocMetY,0,redminAssocMet);
              metTypeValues["assocMet5"]           = assocMet5P4;
              metTypeValues["assocMet10"]          = assocMet10P4;
              metTypeValues["assocFwdMet"]         = assocFwdMetP4;
              metTypeValues["assocFwdMet5"]        = assocFwdMet5P4;
              metTypeValues["assocFwdMet10"]       = assocFwdMet10P4;
              metTypeValues["minAssocFwdMet"]      = min(zvv,metTypeValues["assocFwdMet"]);

      std::map<TString,double> metTypeValuesminJetdphi;
      std::map<TString,double> metTypeValuesminJetphi;
      for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++){metTypeValuesminJetdphi[it->first] = 9999.0; metTypeValuesminJetphi[it->first] = 9999.0;}
      int zrank(0);
      double mindphijmet(9999.), minmtjmet(9999.),mindrjz(9999.), minmjz(9999.);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) 
	{
	  if(phys.jets[ijet].pt()>zll.pt()) zrank++;

	  double dphijmet=fabs(deltaPhi(zvv.phi(),phys.jets[ijet].phi()));

          for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++){
             double tmpdphijmet=fabs(deltaPhi(it->second.phi(),phys.jets[ijet].phi()));             
             if(metTypeValuesminJetdphi[it->first]>tmpdphijmet){metTypeValuesminJetdphi[it->first]=tmpdphijmet;  metTypeValuesminJetphi[it->first] = phys.jets[ijet].phi();}
          }


	  double mtjmet=mtComp.compute(phys.jets[ijet],zvv,false);
	  if(mtjmet<minmtjmet) minmtjmet=mtjmet;

	  double drjz=deltaR(zll,phys.jets[ijet]);
	  if(mindrjz>drjz) mindrjz=drjz;
	  LorentzVector jz=phys.jets[ijet]+zll;
	  double mjz=jz.mass();
	  if(mjz<minmjz) minmjz=mjz;
	}





      //vbf variables 
      bool isVBF        = false;
      bool Pass2Jet30   = false;
      bool PassdEtaCut  = false;
      bool PassiMCut    = false;
      bool PassBJetVeto = false;
      bool PassJetVeto  = false;
      bool PassLeptonIn = false;

      double  VBFdEta = -1;
      int     VBFCentral30Jets = 0;
      int     VBFCentralLeptons = 0;
      int     VBFNBJets=0;
      LorentzVector VBFSyst;

      if(phys.jets.size()>=2 && phys.jets [0].pt()>30.0 && phys.jets [1].pt()>30.0)
	{
	  VBFSyst =  phys.jets[0] + phys.jets[1];
	  VBFdEta = fabs(   phys.jets[0].eta() -    phys.jets[1].eta()); if(phys.jets[0].eta()* phys.jets[1].eta()>0)VBFdEta*=-1;
	  int VBFCentral30Jets = 0;
	  double MinEta, MaxEta;
	  if(phys.jets[0].eta()<phys.jets[1].eta()){MinEta=phys.jets[0].eta(); MaxEta=phys.jets[1].eta();}else{MinEta=phys.jets[1].eta(); MaxEta=phys.jets[0].eta();}
	  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
	  for(size_t ijet=2; ijet<phys.jets.size(); ijet++) {
	    if(phys.jets[ijet].pt()<30)continue; 
	    if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral30Jets++; 
	    if(phys.jets[ijet].btag2>1.33 || phys.jets[ijet].btag3>1.74)VBFNBJets++; 
	  }
	  
	  Pass2Jet30   = true;
	  PassdEtaCut  = (fabs(VBFdEta)>4.0);
	  PassiMCut    = (VBFSyst.M()>500);
	  PassLeptonIn = (VBFCentralLeptons==2);
	  PassJetVeto  = (VBFCentral30Jets==0);
	  PassBJetVeto = (VBFNBJets==0);
	  isVBF        = (Pass2Jet30 && PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn);
	}
     

      //
      //MVA evaluation
      //
      for(size_t ivar=0; ivar<varsList.size(); ivar++) 
	{
	  std::string variable=varsList[ivar];
	  if(variable=="dphill")         tmvaVars[ivar]=dphill;
	  if(variable=="detall")         tmvaVars[ivar]=detall;
	  if(variable=="drll")           tmvaVars[ivar]=drll;
	  if(variable=="mindrlz")        tmvaVars[ivar]=mindrlz;
	  if(variable=="maxdrlz")        tmvaVars[ivar]=maxdrlz;
	  if(variable=="ptl1")           tmvaVars[ivar]=ptl1;
	  if(variable=="ptl2")           tmvaVars[ivar]=ptl2;
	  if(variable=="ptsum")          tmvaVars[ivar]=ptsum;
	  if(variable=="mtl1")           tmvaVars[ivar]=mtl1;
	  if(variable=="mtl2")           tmvaVars[ivar]=mtl2;
	  if(variable=="mtsum")          tmvaVars[ivar]=mtsum;
	  if(variable=="zmass")          tmvaVars[ivar]=zmass;
	  if(variable=="zpt")            tmvaVars[ivar]=zpt;
	  if(variable=="zeta")           tmvaVars[ivar]=zeta;
	  if(variable=="met")            tmvaVars[ivar]=met;
	  if(variable=="dphizz")         tmvaVars[ivar]=dphizz;
          if(variable=="mt")             tmvaVars[ivar]=mt;
	  if(variable=="redMet")         tmvaVars[ivar]=redMet;
	  if(variable=="redMetL")        tmvaVars[ivar]=redMetL;
	  if(variable=="redMetT")        tmvaVars[ivar]=redMetT;
	  if(variable=="projMet")        tmvaVars[ivar]=projMet;
          if(variable=="dphizleadl")     tmvaVars[ivar]=dphizleadl;
	}
      tmvaVars[varsList.size()] = eventCategory;

      if(useMVA)
	{
	  //evaluate the methods
	  for(size_t imet=0; imet<methodList.size(); imet++)
	    {
	      discriResults[imet]=tmvaReader->EvaluateMVA( methodList[imet] );
	     
	      //per-event error
	      if (methodList[imet]=="PDEFoam")
		{
		  pdeFoamError = tmvaReader->GetMVAError();
		  pdeFoamSig   = (pdeFoamError>1.e-50 ? discriResults[imet]/pdeFoamError : 0.);
		}
	      //probability for Fisher discriminant
	      if (methodList[imet]=="Fisher") 
		{
		  fisherProb   = tmvaReader->GetProba("Fisher");
		  fisherRarity = tmvaReader->GetRarity("Fisher");
		}
	    }
	}

      //
      // event pre-selection
      //
      bool passZmass(fabs(zmass-91)<15);
      bool passSideBand( !passZmass && fabs(zmass-91)<30 );

      bool passZpt(zpt>25);

      bool pass3dLeptonVeto(ev.ln==0);
      bool passBveto(nbtags_tche2==0);//nbtags==0);
      bool passMediumRedMet(redMet>rmetComp.getCut(eventCategory,ReducedMETComputer::MEDIUMWP));
      bool passTightRedMet(redMet>rmetComp.getCut(eventCategory,ReducedMETComputer::TIGHTWP));
      
      bool passBaseCuts(passZmass && pass3dLeptonVeto && passBveto && passMediumRedMet);
      std::vector<int> zmassRegionBins;
      if(passZmass)                                                                 zmassRegionBins.push_back(0);
      if(passZmass    && !passBveto)                                                zmassRegionBins.push_back(1);
      if(passZmass    && !passBveto && pass3dLeptonVeto)                            zmassRegionBins.push_back(2);
      if(passZmass    && !passBveto && pass3dLeptonVeto && passMediumRedMet)        zmassRegionBins.push_back(3);
      if(passZmass    && !passBveto && pass3dLeptonVeto && passTightRedMet)         zmassRegionBins.push_back(4);
      if(passSideBand)                                                              zmassRegionBins.push_back(5);
      if(passSideBand && !passBveto)                                                zmassRegionBins.push_back(6);
      if(passSideBand && !passBveto && pass3dLeptonVeto)                            zmassRegionBins.push_back(7);
      if(passSideBand && !passBveto && pass3dLeptonVeto && passMediumRedMet)        zmassRegionBins.push_back(8);
      if(passSideBand && !passBveto && pass3dLeptonVeto && passTightRedMet)         zmassRegionBins.push_back(9);
      if(passZmass    && passBveto)                                                 zmassRegionBins.push_back(10);
      if(passZmass    && passBveto && pass3dLeptonVeto)                             zmassRegionBins.push_back(11);
      if(passZmass    && passBveto && pass3dLeptonVeto && passMediumRedMet)         zmassRegionBins.push_back(12);
      if(passZmass    && passBveto && pass3dLeptonVeto && passTightRedMet)          zmassRegionBins.push_back(13);
      if(passSideBand && passBveto)                                                 zmassRegionBins.push_back(14);
      if(passSideBand && passBveto && pass3dLeptonVeto)                             zmassRegionBins.push_back(15);
      if(passSideBand && passBveto && pass3dLeptonVeto && passMediumRedMet)         zmassRegionBins.push_back(16);
      if(passSideBand && passBveto && pass3dLeptonVeto && passTightRedMet)          zmassRegionBins.push_back(17);


      //
      // compute systematic variations for JET/MET
      //
      LorentzVectorCollection metVars;
      std::vector<LorentzVectorCollection> jetVars;
      std::vector<int>eventCategoryVars;
      std::vector<Float_t> redMetVar, redMetLVar, mtsumsVar;
      std::vector<bool> passMediumRedMetVars, passTightRedMetVars;
      if(runSystematics)
	{
	  jcomp.compute(jetsP4,phys.met[0]);

	  metVars.push_back( jcomp.getVariedMet(jet::UncertaintyComputer::JER) );
	  jetVars.push_back( jcomp.getVariedJets(jet::UncertaintyComputer::JER) );

	  metVars.push_back( jcomp.getVariedMet(jet::UncertaintyComputer::JES_UP) );
	  jetVars.push_back( jcomp.getVariedJets(jet::UncertaintyComputer::JES_UP) );

	  metVars.push_back( jcomp.getVariedMet(jet::UncertaintyComputer::JES_DOWN) );
	  jetVars.push_back( jcomp.getVariedJets(jet::UncertaintyComputer::JES_DOWN) );

	  for(size_t ivar=0; ivar<3; ivar++)
	    {
	      eventCategoryVars.push_back( eventClassifComp.Get(phys, &(jetVars[ivar])) );
	      rmetComp.compute(phys.leptons[0],0,phys.leptons[1], 0, jetVars[ivar], metVars[ivar]);
	      redMetVar.push_back(rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED));
	      redMetLVar.push_back(rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second);
	      Float_t imtsum     = mtComp.compute(phys.leptons[0],metVars[ivar],false) + mtComp.compute(phys.leptons[1],metVars[ivar],false);
	      mtsumsVar.push_back(imtsum);

	      int ivarEvCat= eventCategoryVars[ivar];
	      passMediumRedMetVars.push_back(redMetVar[ivar]>rmetComp.getCut(ivarEvCat,ReducedMETComputer::MEDIUMWP));
	      passTightRedMetVars.push_back(redMetVar[ivar]>rmetComp.getCut(ivarEvCat,ReducedMETComputer::TIGHTWP));
	    }
	}


//      ROOT::Math::Boost cmboost(genhiggs.BoostToCM());
//      LorentzVector cmzll(cmboost(zll));
//      LorentzVector cmzvv(cmboost(zvv));

      LorentzVector transverseHiggs(genhiggs.px(),genhiggs.py(), 0,sqrt(pow(genhiggs.mass(),2)+pow(genhiggs.pt(),2)));
      ROOT::Math::Boost cmboost(transverseHiggs.BoostToCM());
      LorentzVector cmzll(cmboost(zll));
      LorentzVector cmzvv(cmboost(zvv));

      
      //fill control histograms
      TString catsToFill[]={"all",evcat};
      std::vector<TString> subCatsToFill;
      subCatsToFill.push_back("");
      subCatsToFill.push_back(subcat);
//       if(ev.nvtx<4)        subCatsToFill.push_back("1to3vtx");
//       else if(ev.nvtx>=10) subCatsToFill.push_back("geq10vtx");
//       else                 subCatsToFill.push_back("4to9vtx");
      for(size_t ic=0; ic<sizeof(catsToFill)/sizeof(TString); ic++)
	{
	  for(size_t isc=0; isc<subCatsToFill.size(); isc++)
	    {
	      TString ctf=catsToFill[ic]+subCatsToFill[isc];

              controlHistos.fillHisto("genHiggsPt"      ,ctf,    genhiggs.pt()   ,weight);
              controlHistos.fillHisto("genHiggsMass"    ,ctf,    genhiggs.mass() ,weight);
              controlHistos.fillHisto("genHiggsMassRaw" ,ctf,    genhiggs.mass() ,weight/VBFWeight);
              controlHistos.fillHisto("genzllPt"        ,ctf,    genzll.pt()     ,weight);
              controlHistos.fillHisto("genzvvPt"        ,ctf,    genzvv.pt()     ,weight);

              if(passZmass){
              controlHistos.fillHisto("genzwinHiggsPt"      ,ctf,    genhiggs.pt()   ,weight);
              controlHistos.fillHisto("genzwinHiggsMass"    ,ctf,    genhiggs.mass() ,weight);
              controlHistos.fillHisto("genzwinHiggsMassRaw" ,ctf,    genhiggs.mass() ,weight/VBFWeight);
              controlHistos.fillHisto("genzwinzllPt"        ,ctf,    genzll.pt()     ,weight);
              controlHistos.fillHisto("genzwinzvvPt"        ,ctf,    genzvv.pt()     ,weight);
              }
 
	      
	      //event pre-selection
	      if(!passZmass && !passSideBand)                                      continue;
	      
	      //VBF control
	      if(true                                                                                       )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    0                ,weight);
	      if(Pass2Jet30                                                                                 )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    1                ,weight);
	      if(PassdEtaCut                                                                                )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    2                ,weight);
	      if(PassdEtaCut && PassiMCut                                                                   )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    3                ,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn                                                   )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    4                ,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto                                    )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    5                ,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto && PassBJetVeto                    )controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    6                ,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto && PassBJetVeto && pass3dLeptonVeto)controlHistos.fillHisto("VBFNEventsInc"       ,ctf,    7                ,weight);
	      
	      if(Pass2Jet30                                                              )controlHistos.fillHisto("VBFdEtaInc"          ,ctf,    fabs(VBFdEta)    ,weight);
	      if(PassdEtaCut                                                             )controlHistos.fillHisto("VBFiMassInc"         ,ctf,    VBFSyst.M()      ,weight);
	      if(PassdEtaCut && PassiMCut                                                )controlHistos.fillHisto("VBFcenLeptonVetoInc" ,ctf,    VBFCentralLeptons,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn                                )controlHistos.fillHisto("VBFcen30JetVetoInc"  ,ctf,    VBFCentral30Jets ,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto                 )controlHistos.fillHisto("VBFNBJets30Inc"      ,ctf,    VBFNBJets        ,weight);
	      
	      if(               PassiMCut && PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fillHisto("VBFdEtaNM1C"         ,ctf,    fabs(VBFdEta)    ,weight);
	      if(PassdEtaCut              && PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fillHisto("VBFiMassNM1C"        ,ctf,    VBFSyst.M()      ,weight);
	      if(PassdEtaCut && PassiMCut                 && PassJetVeto && PassBJetVeto )controlHistos.fillHisto("VBFcenLeptonVetoNM1C",ctf,    VBFCentralLeptons,weight);
	      if(Pass2Jet30                                              && PassBJetVeto && !pass3dLeptonVeto)                                       
		{
		  controlHistos.fillHisto("VBFcen30JetVeto3rdlepton"  ,ctf,    VBFCentral30Jets ,weight);
		  controlHistos.fillHisto("VBFNBJets303rdlepton"      ,ctf,    VBFNBJets        ,weight);
		  controlHistos.fillHisto("VBFdEta3rdlepton"          ,ctf,    fabs(VBFdEta)    ,weight);
		  controlHistos.fillHisto("VBFiMass3rdlepton"         ,ctf,    VBFSyst.M()      ,weight);
		}
	      
	      if(PassdEtaCut && PassiMCut && PassLeptonIn                && PassBJetVeto )controlHistos.fillHisto("VBFcen30JetVetoNM1C" ,ctf,    VBFCentral30Jets ,weight);
	      if(PassdEtaCut && PassiMCut && PassLeptonIn && PassJetVeto                 )controlHistos.fillHisto("VBFNBJets30NM1C"     ,ctf,    VBFNBJets        ,weight);
	      if(                            PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fill2DHisto("VBFdEtaiMassNM1C"  ,ctf,    fabs(VBFdEta), VBFSyst.M(), weight);
	      if(redMet>50 &&                PassLeptonIn && PassJetVeto && PassBJetVeto )controlHistos.fill2DHisto("VBFdEtaiMassNM1C2" ,ctf,    fabs(VBFdEta), VBFSyst.M(), weight);
	      
	      //inclusive control
	      controlHistos.fillHisto("eventflow",ctf,1,weight);
	      controlHistos.fillHisto("nvtx",ctf,ev.nvtx,weight);
	      
	      //dilepton control plots
	      controlHistos.fillHisto("zmass",ctf,zmass,weight);
	      for(std::vector<int>::iterator regIt = zmassRegionBins.begin(); regIt<zmassRegionBins.end(); regIt++) controlHistos.fillHisto("zmassregionCtr",ctf,*regIt,weight);
	      controlHistos.fillHisto("zeta",ctf,zll.eta(),weight);
	      controlHistos.fillHisto("zpt",ctf,zll.pt(),weight);
	      controlHistos.fillHisto("zrank",ctf,zrank,weight);
	      controlHistos.fill2DHisto("zptvszeta", ctf,zll.pt(),zll.eta(),weight);
	      controlHistos.fillHisto("dphill",ctf,dphill,weight);
	      controlHistos.fillHisto("mindrlz",ctf,mindrlz,weight);
	      controlHistos.fillHisto("maxdrlz",ctf,maxdrlz,weight);
	      controlHistos.fillHisto("ptsum",ctf,ptsum,weight);
	      controlHistos.fillHisto("mtl1",ctf,mtl1,weight);
	      controlHistos.fillHisto("mtl2",ctf,mtl2,weight);
              controlHistos.fillHisto("mt",ctf,mt,weight);
	      
	      //Z window
	      if(!passZmass)  continue;
	      controlHistos.fillHisto("eventflow",ctf,2,weight);
	     
              if(!passZpt) continue;
              controlHistos.fillHisto("eventflow",ctf,3,weight);
 
	      //extra leptons
	      controlHistos.fillHisto("nleptons",ctf,ev.ln,weight);
	      if(!pass3dLeptonVeto)
		{
		  controlHistos.fillHisto("njets3leptons",ctf,njets,weight);
		  controlHistos.fillHisto("nbtags3leptons",ctf,nbtags,weight);
		  if(passBveto)
		    {
		      
		    }
		  continue;
		}
	      controlHistos.fillHisto("eventflow",ctf,4,weight);
	      
	      //jet control
	      controlHistos.fillHisto("njets",ctf,njets,weight);
	      controlHistos.fill2DHisto("njetsvspu",ctf,ev.ngenITpu,njets,weight);
	      controlHistos.fill2DHisto("njetsincvspu",ctf,ev.ngenITpu,njetsinc,weight);
	      controlHistos.fillHisto("nbtags",ctf, nbtags,weight);
	      controlHistos.fillHisto("npassbveto",ctf,0, (nbtags_ssvhem==0)*weight);
	      controlHistos.fillHisto("npassbveto",ctf,1, (nbtags_tchel==0)*weight);
	      controlHistos.fillHisto("npassbveto",ctf,2, (nbtags_tche2==0)*weight);
	      controlHistos.fillHisto("npassbveto",ctf,3, (nbtags_jbpl==0)*weight);
	      controlHistos.fillHisto("npassbveto",ctf,4, (nbtags_ssvhemORtchel==0)*weight);
	      controlHistos.fillHisto("npassbveto",ctf,5, (nbtags==0)*weight);
	      controlHistos.fillHisto("zmassctrl",ctf,passBveto+2*passMediumRedMet,weight);
	      if(!passBveto) continue;
	      controlHistos.fillHisto("eventflow",ctf,5,weight);


              if(isMC && genhiggs.pt()>0){
              controlHistos.fillHisto("CMzllP"      ,ctf,    cmzll.pt()   ,weight);
              controlHistos.fillHisto("CMzvvP"      ,ctf,    cmzvv.pt()   ,weight);
              controlHistos.fillHisto("CMDeltazP"   ,ctf,    cmzll.pt()-cmzvv.pt()   ,weight);
              controlHistos.fillHisto("CMiMass"     ,ctf,    mtComp.compute(cmzll,cmzvv,true)   ,weight);
              }

     
/* 
	      //met control
	      metTypeValues["met"]                 = zvv;
	      metTypeValues["projMet"]             = LorentzVector(projMet,0,0,0);
	      metTypeValues["minProjMet"]          = min(metTypeValues["projMet"],LorentzVector(projAssocChargedMet,0,0,0) );
	      metTypeValues["redMet"]              = LorentzVector(redMetL,redMetT,0,0);
	      metTypeValues["redMetD0"]            = LorentzVector(redMetL_d0, redMetT_d0, 0, 0);
	      metTypeValues["assocChargedMet"]     = assocChargedMetP4;
	      metTypeValues["minAssocChargedMet"]  = min(zvv,assocChargedMetP4);
	      metTypeValues["centralMet"]          = centralMetP4;
	      metTypeValues["minCentralMet"]       = min(zvv,centralMetP4);
	      metTypeValues["assocOtherVertexMet"] = assocOtherVertexMetP4;
	      metTypeValues["cleanMet"]            = cleanMetP4;
	      metTypeValues["minCleanMet"]         = min(zvv,cleanMetP4);
	      metTypeValues["assocMet"]            = assocMetP4;
	      metTypeValues["minAssocMet"]         = min(zvv,assocMetP4);
	      metTypeValues["superMinMet"]         = min(
							 metTypeValues["minAssocMet"],
							 min(
							     metTypeValues["minCleanMet"],
							     metTypeValues["centralMet"])
							 );
*/
	      for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++) 	  
		{
                  if(it->second.pt()>50 && metTypeValuesminJetdphi[it->first]<10){
                     controlHistos.fillHisto(TString("met_") + it->first+"mindphijmet",ctf,metTypeValuesminJetdphi[it->first], weight);
//                     controlHistos.fill2DHisto(TString("met_") + it->first+"phimetphijet", ctf,it->second.phi(),metTypeValuesminJetphi[it->first],weight);
                  }


                  if(mindphijmet<0.3)continue;

		  controlHistos.fillHisto(TString("met_") + it->first, ctf,it->second.pt(),weight);
		  controlHistos.fill2DHisto(TString("met_") + it->first+"vspu", ctf,ev.ngenITpu,it->second.pt(),weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"zpt", ctf,it->second.pt(),zpt,weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"minzpt", ctf,std::min(it->second.pt(),zll.pt()),weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"geq080zpt", ctf,it->second.pt()>=0.8*zll.pt() ? it->second.pt() : 0.0,weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"geq080zptvspu", ctf,ev.ngenITpu, it->second.pt()>=0.8*zll.pt() ? it->second.pt() : 0.0,weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"geq060zpt", ctf,it->second.pt()>=0.6*zll.pt() ? it->second.pt() : 0.0,weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"geq060zptvspu", ctf,ev.ngenITpu,it->second.pt()>=0.6*zll.pt() ? it->second.pt() : 0.0,weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"geq040zpt", ctf,it->second.pt()>=0.4*zll.pt() ? it->second.pt() : 0.0,weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"geq040zptvspu", ctf,ev.ngenITpu,it->second.pt()>=0.4*zll.pt() ? it->second.pt() : 0.0,weight);


                  controlHistos.fill2DHisto(TString("met_") + it->first+"pfmet", ctf,it->second.pt(),zvv.pt(),weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"leq120pfmet", ctf, it->second.pt()<=1.2*zvv.pt() ? zvv.pt() : 0.0,weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"leq120pfmetvspu", ctf,ev.ngenITpu,it->second.pt()<=1.2*zvv.pt() ? zvv.pt() : 0.0,weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"leq140pfmet", ctf,it->second.pt()<=1.4*zvv.pt() ? zvv.pt() : 0.0,weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"leq140pfmetvspu", ctf,ev.ngenITpu,it->second.pt()<=1.4*zvv.pt() ? zvv.pt() : 0.0,weight);
                  controlHistos.fillHisto(TString("met_") + it->first+"leq160pfmet", ctf,it->second.pt()<=1.6*zvv.pt() ? zvv.pt() : 0.0,weight);
                  controlHistos.fill2DHisto(TString("met_") + it->first+"leq160pfmetvspu", ctf,ev.ngenITpu,it->second.pt()<=1.6*zvv.pt() ? zvv.pt() : 0.0,weight);


                  TVector2 zll2DLong  = TVector2(zll.px()/zll.pt(), zll.py()/zll.pt());
                  TVector2 zll2DTrans = zll2DLong.Rotate(TMath::Pi()/2);
                  double LongMET  = zll2DLong .Px()*it->second.px() + zll2DLong .Py()*it->second.py();
                  double TransMET = zll2DTrans.Px()*it->second.px() + zll2DTrans.Py()*it->second.py();
                  controlHistos.fillHisto(TString("metL_") + it->first, ctf,LongMET,weight);
		  controlHistos.fill2DHisto(TString("metL_") + it->first+"vspu", ctf,ev.ngenITpu,LongMET,weight);
                  controlHistos.fillHisto(TString("metT_") + it->first, ctf,TransMET,weight);
//                  controlHistos.fill2DHisto(TString("metT_") + it->first+"vspu", ctf,ev.ngenITpu,TransMET,weight);
//                  controlHistos.fill2DHisto(TString("metLT_") + it->first, ctf,LongMET,TransMET,weight);
		}

	      controlHistos.fill2DHisto("metvstkmet", ctf,met,assocChargedMet,weight);
	      controlHistos.fill2DHisto("metvsclusteredMet", ctf,met,assocMet,weight);
	      controlHistos.fill2DHisto("metvscentralMet", ctf,met,centralMet,weight);
	      controlHistos.fillHisto("redMetL", ctf,redMetL,weight);
	      controlHistos.fillHisto("redMetT", ctf,redMetT,weight);
	      controlHistos.fillHisto("redMetcomps", ctf,redMetL,redMetT,weight);	

              //fixme: redo properly the ntuples and remove this stupid mapping
            float leptonSumEt      = phys.leptons[0].pt()+phys.leptons[1].pt();
            float sumEt            = ev.sumEt           -leptonSumEt;
            float sumEtcentral     = ev.sumEtcentral    -leptonSumEt;
            float chSumEtcentral   = ev.chsumEtcentral  -leptonSumEt;
            float neutSumEtcentral = ev.neutsumEtcentral;
            float chSumEt          = ev.chsumEt         -leptonSumEt;
            float neutsumEt        = ev.neutsumEt;

	      if(sumEt>0)
		{
		  controlHistos.fillHisto("sumEt",                ctf,sumEt,weight);
		  controlHistos.fillHisto("chSumEt",              ctf,chSumEt,weight);
		  controlHistos.fillHisto("neutSumEt",            ctf,neutsumEt,weight);
		  controlHistos.fillHisto("primVertexSumEt",      ctf,ev.primVertexSumEt,weight);
		  controlHistos.fillHisto("otherVertexSumEt",     ctf,ev.otherVertexSumEt,weight);
		  if(isMC)
		    {
		      double sigma        = sqrt(double(ev.ngenITpu));
		      double minCentralPu = -sigma;
		      double maxCentralPu = sigma;
		      double maxHighPu    = 2*sigma;
		      double puDiff       = double(ev.ngenOOTpu-ev.ngenITpu);
		      TString ootCond("");
		      if(puDiff>=maxHighPu)         ootCond="VeryHighOOTpu";
		      else if(puDiff>=maxCentralPu) ootCond="HighOOTpu";
		      else if(puDiff>=minCentralPu) ootCond="MediumOOTpu";
		      else                          ootCond="LowOOTpu";
		      

		      controlHistos.fillHisto("sumEt"+ootCond,ctf,sumEt,weight);
		      controlHistos.fillHisto("neutSumEtFrac"+ootCond,ctf,neutsumEt/sumEt,weight);
		      controlHistos.fill2DHisto("itpuvsootpu",ctf,ev.ngenITpu,ev.ngenOOTpu,weight);
		    }
		  controlHistos.fillHisto("chSumEtFrac",          ctf,chSumEt/sumEt,weight);
		  controlHistos.fillHisto("neutSumEtFrac",        ctf,neutsumEt/sumEt,weight);
		  controlHistos.fillHisto("centralSumEtFrac",     ctf,sumEtcentral/sumEt,weight);
		  controlHistos.fillHisto("centralChSumEtFrac",   ctf,chSumEtcentral/sumEt,weight);
		  controlHistos.fillHisto("centralNeutSumEtFrac", ctf,neutSumEtcentral/sumEt,weight);
		  controlHistos.fillHisto("chPrimVertexSumEtFrac",          ctf,ev.primVertexChSumEt/sumEt,weight);
		  controlHistos.fillHisto("neutPrimVertexSumEtFrac",        ctf,ev.primVertexNeutSumEt/sumEt,weight);
		  controlHistos.fillHisto("chOtherVertexSumEtFrac",          ctf,ev.otherVertexChSumEt/sumEt,weight);
		  controlHistos.fillHisto("neutOtherVertexSumEtFrac",        ctf,ev.otherVertexNeutSumEt/sumEt,weight);
		 
		}

	      controlHistos.fillHisto("mindphijmet",ctf,mindphijmet,weight);
	      controlHistos.fillHisto("minmtjmet",ctf,minmtjmet,weight);
	      controlHistos.fillHisto("mindrjz",ctf,mindrjz,weight);
	      controlHistos.fillHisto("minmjz",ctf,minmjz,weight);
	      
	      
	      //sample is selected
	      if(passMediumRedMet) 
		{
		  controlHistos.fillHisto("eventflow",ctf,6,weight);
		  controlHistos.fillHisto("eventCategory",ctf,eventCategory,weight);
		  controlHistos.fillHisto("cutOptMediumdphill",ctf,fabs(dphill));
		  controlHistos.fill2DHisto("cutOptMediumsummtvsredMetL",ctf,mtsum,redMetL,weight);
		  
		  if(passTightRedMet)    controlHistos.fillHisto("eventflow",ctf,7,weight);
		}
	      
	      //
	      // CUT & COUNT ANALYSIS
	      //
//	      //final selection (cut and count analysis)
//              bool pass130( passMediumRedMet && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetL)>50 && mtsum>150);
//              bool pass150( passMediumRedMet && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetL)>50 && mtsum>150);
//              bool pass170( passMediumRedMet && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetL)>50 && mtsum>150);
//	      bool pass200( passMediumRedMet && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetL)>50 && mtsum>150);
//	      bool pass300( passTightRedMet  && fabs(dphill)<2.5                      && redMetL>75       && mtsum>200);
//	      bool pass400( passTightRedMet  && fabs(dphill)<2.0                      && redMetL>75       && mtsum>300);
//	      bool pass500( passTightRedMet  && fabs(dphill)<2.0                      && redMetL>100      && mtsum>400);
//	      bool pass600( passTightRedMet  && fabs(dphill)<1.5                      && redMetL>150 && mtsum>450);

              bool pass130( zvv.pt()>69  && mindphijmet>0.62 && mt>216 && mt<272);
              bool pass150( zvv.pt()>69  && mindphijmet>0.62 && mt>216 && mt<272);
              bool pass170( zvv.pt()>69  && mindphijmet>0.62 && mt>216 && mt<272);
              bool pass200( zvv.pt()>69  && mindphijmet>0.62 && mt>216 && mt<272);
              bool pass300( zvv.pt()>83  && mindphijmet>0.28 && mt>242 && mt<320);
              bool pass400( zvv.pt()>112 && mindphijmet>0.00 && mt>392 && mt<386);
              bool pass500( zvv.pt()>141 && mindphijmet>0.00 && mt>336          );
              bool pass600( zvv.pt()>170 && mindphijmet>0.00 && mt>377          );

	      if(subcat=="vbf")
		{
                  pass130 = passMediumRedMet;
                  pass150 = passMediumRedMet;
                  pass170 = passMediumRedMet;
		  pass200 = passMediumRedMet;
		  pass300 = redMet>65;
		  pass400 = redMet>65;
		  pass500 = redMet>90;
		  pass600 = redMet>90;
		}
              if(pass130) controlHistos.fillHisto("finaleventflow",ctf,0,weight);
              if(pass150) controlHistos.fillHisto("finaleventflow",ctf,1,weight);
              if(pass170) controlHistos.fillHisto("finaleventflow",ctf,2,weight);
	      if(pass200) controlHistos.fillHisto("finaleventflow",ctf,3,weight);
	      if(pass300) controlHistos.fillHisto("finaleventflow",ctf,4,weight);
	      if(pass400) controlHistos.fillHisto("finaleventflow",ctf,5,weight);
	      if(pass500) controlHistos.fillHisto("finaleventflow",ctf,6,weight);
	      if(pass600) controlHistos.fillHisto("finaleventflow",ctf,7,weight);


              if(zpt>25){
                 if(pass130) controlHistos.fillHisto("zpt25finaleventflow",ctf,0,weight);
                 if(pass150) controlHistos.fillHisto("zpt25finaleventflow",ctf,1,weight);
                 if(pass170) controlHistos.fillHisto("zpt25finaleventflow",ctf,2,weight);
                 if(pass200) controlHistos.fillHisto("zpt25finaleventflow",ctf,3,weight);
                 if(pass300) controlHistos.fillHisto("zpt25finaleventflow",ctf,4,weight);
                 if(pass400) controlHistos.fillHisto("zpt25finaleventflow",ctf,5,weight);
                 if(pass500) controlHistos.fillHisto("zpt25finaleventflow",ctf,6,weight);
                 if(pass600) controlHistos.fillHisto("zpt25finaleventflow",ctf,7,weight);
              }



	      //systematic variations (computed per jet bin so fill only once) 		  
	      if(isc==0 && runSystematics)
		{
		  //jet energy scale related variations
		  TString jetVarNames[]={"jer","jesup","jesdown"};
		  for(size_t ivar=0; ivar<3; ivar++)
		    {
		      TString isubcat    = eventClassifComp.GetLabel( eventCategoryVars[ivar] );
		      TString ictf= catsToFill[ic]+isubcat;
                      bool ipass130( passMediumRedMetVars[ivar] && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetLVar[ivar])>50 && mtsumsVar[ivar]>150);
                      bool ipass150( passMediumRedMetVars[ivar] && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetLVar[ivar])>50 && mtsumsVar[ivar]>150);
                      bool ipass170( passMediumRedMetVars[ivar] && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetLVar[ivar])>50 && mtsumsVar[ivar]>150);
		      bool ipass200( passMediumRedMetVars[ivar] && fabs(dphill)<2.75 && fabs(dphill)>1.0 && fabs(redMetLVar[ivar])>50 && mtsumsVar[ivar]>150);
		      bool ipass300( passTightRedMetVars[ivar]  && fabs(dphill)<2.5                      && redMetLVar[ivar]>75       && mtsumsVar[ivar]>200);
		      bool ipass400( passTightRedMetVars[ivar]  && fabs(dphill)<2.0                      && redMetLVar[ivar]>75       && mtsumsVar[ivar]>300);
		      bool ipass500( passTightRedMetVars[ivar]  && fabs(dphill)<2.0                      && redMetLVar[ivar]>100      && mtsumsVar[ivar]>400);
		      bool ipass600( passTightRedMetVars[ivar]  && fabs(dphill)<1.5                      && redMetLVar[ivar]>150      && mtsumsVar[ivar]>450);
		      if(subcat=="vbf")
			{
                          ipass130 = passMediumRedMetVars[ivar];
                          ipass150 = passMediumRedMetVars[ivar];
                          ipass170 = passMediumRedMetVars[ivar];
			  ipass200 = passMediumRedMetVars[ivar];
			  ipass300 = passMediumRedMetVars[ivar];
			  ipass400 = passMediumRedMetVars[ivar];
			  ipass500 = passMediumRedMetVars[ivar];
			  ipass600 = passMediumRedMetVars[ivar];
			}
                      if(ipass130) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,0,weight);
                      if(ipass150) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,1,weight);
                      if(ipass170) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,2,weight);
		      if(ipass200) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,3,weight);
		      if(ipass300) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,4,weight);
		      if(ipass400) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,5,weight);
		      if(ipass500) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,6,weight);
		      if(ipass600) controlHistos.fillHisto(jetVarNames[ivar]+"finaleventflow",ictf,7,weight);
		    }
		 
		  //re-weighting variations (Higgs, pileup scenario)
		  TString wgtVarNames[]={"hrenup","hrendown","hfactup","hfactdown","flatpu"};
		  Float_t rwgtVars[]={ev.weight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renUp],
				      ev.weight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_renDown],
				      ev.weight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factUp],
				      ev.weight*ev.hptWeights[ZZ2l2nuSummary_t::hKfactor_factDown],
				      ev.hptWeights[ZZ2l2nuSummary_t::hKfactor]};
		  for(size_t ivar=0; ivar<sizeof(wgtVarNames)/sizeof(TString); ivar++)
                    {
		      TString ictf= catsToFill[ic]+subcat;
                      if(pass130) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,0,rwgtVars[ivar]);
                      if(pass150) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,1,rwgtVars[ivar]);
                      if(pass170) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,2,rwgtVars[ivar]);
		      if(pass200) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,3,rwgtVars[ivar]);
		      if(pass300) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,4,rwgtVars[ivar]);
		      if(pass400) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,5,rwgtVars[ivar]);
		      if(pass500) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,6,rwgtVars[ivar]);
		      if(pass600) controlHistos.fillHisto(wgtVarNames[ivar]+"finaleventflow",ictf,7,rwgtVars[ivar]);
		    }
		}

		  
		  
	      //
	      // MVA ANALYSIS
	      //
	      if(!passMediumRedMet) continue;
	      //control for discriminators evaluated
	      controlHistos.fillHisto("dphizleadl",ctf,dphizleadl,weight);
	      controlHistos.fillHisto("drll",ctf,drll,weight);
	      controlHistos.fillHisto("mtsum",ctf,mtsum,weight);
	      //for(size_t ivar=0; ivar<varsList.size(); ivar++)  controlHistos.fillHisto(varsList[ivar],ctf,tmvaVars[ivar],weight);
	      for(size_t imet=0; imet<methodList.size(); imet++)
		{
		  controlHistos.fillHisto(methodList[imet],ctf,discriResults[imet],weight);
		  if(passTightRedMet) controlHistos.fillHisto(methodList[imet]+"tight",ctf,discriResults[imet],weight);
		  
		  //per-event error
		  if (methodList[imet]=="PDEFoam")
		    {
		      controlHistos.fillHisto("PDEFoam_Err",ctf,pdeFoamError,weight);
		      controlHistos.fillHisto("PDEFoam_Sig",ctf,pdeFoamSig,weight);
		    }
		  //probability for Fisher discriminant
		  if (methodList[imet]=="Fisher") 
		    {
		      controlHistos.fillHisto("Fisher_Proba",ctf, fisherProb, weight);
		      controlHistos.fillHisto("Fisher_Rarity",ctf, fisherRarity, weight);
		    }
		}
	    }
	}
    
  
      //
      // summary tree for the selected events
      //
      if(saveSummaryTree && (passBaseCuts || isVBF))
	{
	  //update the pass and wiehgt fields
	  ev.pass=passMediumRedMet+passTightRedMet;
	  ev.weight=summaryWeight*weight;		      
	  spyHandler->fillTree();
	  
	  //write in twiki format the intersting event
	  if(outf && passTightRedMet)	
	    {
	      *outf << "<b>"<< subcat << " event</b> @ " << url << "<br/>" << std::endl;
	      *outf << "%$Run=" <<  ev.run << "$% %$Lumi=" << ev.lumi << "$% %$Event=" << ev.event <<"$%" << "<br/>" << std::endl;
	      
	      *outf << "<i>Leptons</i>" << "<br/>" << std::endl;
	      for(size_t ilep=0; ilep<2; ilep++)
		*outf << "%$l_{" << ilep+1 << "}=" << phys.leptons[ilep].id << "$% "
		      << "%$p_T=" << phys.leptons[ilep].pt() << "$% "
		      << "%$\\eta=" << phys.leptons[ilep].eta() << "$% "
		      << "%$\\phi=" << phys.leptons[ilep].phi() << "$% "
		      << "%$I_{neut}=" << phys.leptons[ilep].iso1 << "$% "
		      << "%$I_{ch}=" << phys.leptons[ilep].iso2  << "$% "
		      << "%$I_{pho}=" << phys.leptons[ilep].iso3 << "$% " << "<br/>" << std::endl; 
	      
	      *outf << "<i>Dilepton</i>" << "<br/>" << std::endl;
	      *outf  << "%$p_{T}^{ll}=" << zll.pt() << "$% "
		     << "%$\\eta^{ll}=" << zll.eta() << "$% "
		     << "%$\\phi^{ll}=" << zll.phi() << "$% "
		     << "%$m^{ll}=" << zll.mass() << "$% "   
		     << "%$\\Delta R(l,l)=" << drll << "$% "
		     << "%$\\Delta\\phi(l,l)=" << dphill << "$% " << "<br/>" << std::endl;
	      
	      *outf << "<i>Missing transverse energy</i>" << "<br/>" << std::endl;
	      *outf << "%$E_{T}^{miss}=" << met << "$% "
		    << "%$\\phi=" << zvv.phi() << "$% " << "<br/>" << std::endl;
	      *outf << "%$red-E_{T}^{miss}=" << redMet << "$% "
		    << "%$l="<< redMetL << "$% "
		    << "%$t=" << redMetT << "$% " << "<br/>" << std::endl;
	      
	      *outf << "<i>Transverse mass</i>" << "<br/>" << std::endl;
	      *outf << "%$\\sum M_T(l,E_{T}^{miss})=" << mtl1+mtl2 << "$% "
		    << "%$M_T(Z,E_{T}^{miss})=" << mt << "$% " << "<br/>" << std::endl; 
	      
	      *outf << "<i>Jet activity</i>" << "<br/>" << std::endl;
	      *outf << "%$N {jets}(p_T>15)= " << ev.jn << "$% "
		    << "%$\\rho=" << ev.rho << "$% "  << "<br/>" << std::endl;
	      
	      *outf << "------" << endl;
	    }
	}
    }
    
  
  //all done with the events file
  file->Close();

  //save control plots to file
  outUrl += "/";
  outUrl += gSystem->BaseName(url);

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  std::map<TString, TDirectory *> outDirs;
  outDirs["all"]=baseOutDir->mkdir("all");
  outDirs["ee"]=baseOutDir->mkdir("ee");
//  outDirs["emu"]=baseOutDir->mkdir("emu");
  outDirs["mumu"]=baseOutDir->mkdir("mumu");
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      TString icat("all");
      if(it->first.BeginsWith("ee")) icat="ee";
      if(it->first.BeginsWith("emu")) continue;//icat="emu";
      if(it->first.BeginsWith("mumu")) icat="mumu";
      outDirs[icat]->cd();
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
	{
	  if(isMC && cnorm>0) hit->second->Scale(1./cnorm);
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH2")
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") )
	    fixExtremities(hit->second,true,true);
	  hit->second->Write();

	}
    }
  ofile->Close();

  if(saveSummaryTree)
    {
      spyDir->cd();
      spyHandler->getTree()->Write();
      spyFile->Write();
      spyFile->Close();
      if(!isMC) outf->close();
    }

  printf("TotalNumber of duplicated is %i/%i = %f%%\n",NumberOfDuplicated,evEnd,(100.0*NumberOfDuplicated)/evEnd);
  printf("VBF WEIGHT INTEGRAL = %f\n",VBFWEIGHTINTEGRAL/evEnd);
}  
