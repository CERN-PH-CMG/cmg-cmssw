/* -*- mode: c++ -*- */

#include "RooWorkspace.h"
#include "RooHist.h"
#include "RooGenericPdf.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooProdPdf.h"
#include "RooUniform.h"
#include "RooJeffreysPrior.h"
#include "RooNumIntConfig.h"
#include "RooBinning.h"
#include "RooLognormal.h"
#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooPlot.h"
#include "RooMinuit.h"

#include "RooStats/BayesianCalculator.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/ProposalHelper.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"

#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"
#include "RooFunctor.h"
#include "RooFunctor1DBinding.h"

#include "TStopwatch.h"
#include "TFile.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TUnixSystem.h"

#include <iostream>
#include <fstream>
#include <string>

#include "StatTools/BayesianDijetFit/interface/DijetHelper.h"
#include "StatTools/BayesianDijetFit/interface/LineShapeDensityPdf.h"
#include "StatTools/BayesianDijetFit/interface/JPMCCalculator.h"
#include "StatTools/BayesianDijetFit/interface/Qstar_qg_3.h"

#include "TMatrix.h"
#include "TVector.h"
#include "TIterator.h"

using namespace RooFit;
using namespace RooStats;
using namespace DijetHelper;
using namespace std;

int main(int argc, char* argv[])
{

  ofstream resultats;

  vector<double> upper_limits;

  // this is needed to setup the auto-compile for the workspace
  gSystem->SetIncludePath("-I$ROOFITSYS/include");
  // Declare magic numbers here

  double MININVMASS = 500.; 
  double MAXINVMASS = 2000.;


  std::string DATASETFN="../data/ds_data_2011-nominal.txt";


  // histogram binning (for display only)
  const int NBINS=54;
  Double_t BOUNDARIES[NBINS] = { 220, 244, 270, 296, 325, 354, 386, 419, 453,
				 489, 526, 565, 606, 649, 693, 740, 788, 838,
				 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383,
				 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132,
				 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147,
	                         3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509};
  
  // start the timer
  TStopwatch t;
  t.Start();

// Definition of the arguments of the programm
  if(argc<2) return -1;
  double signalMass = atof(argv[1]);
  int statlevel = atoi(argv[2]); // 0=stat only+fit with (a), 1=background, 2=lumi, 3=JES, 4=JER, 5=JES+JER, 6=lumi+JES+JER, 11=stat only+fit with (a), (b) and (c), 12=stat only+fit with (a) and (b), 13=stat only+fit with (b), 14=stat only+fit with (c)
  int numPEs = argc>=4 ? atoi(argv[3]) : 0; // number of pseudo-experience (Monte-Carlo)
//  const char* cResonance = argc>=5 ? argv[4] : "No Resonance"; //useless because this programm work only with ZprimeToTTbar
  bool verbose_ = argc>=5 ? atoi(argv[4]) : 1;
  bool doAggressiveBkgFit = argc>=6 ? atoi(argv[5]) : 1; // fit S+B (as if S=0)
  bool yieldIncluded = argc>=7 ? atoi(argv[6]) : 1; // include the Yield parameter (=sqrt(JEC^2 + signal^2)) in the calcul of the relative error on luminosity
  bool negativeSignalAuthorized = argc>=8 ? atoi(argv[7]) : 0; //authorize the signal to be negative for the fit
  int nIteration = argc>=9 ? atoi(argv[8]) : 1; // number of iteration for the bayesian method
  std::string sResonance("ZprimeToTTbar");
  int iResonance = 201; // correspond to Zprime to TTbar



  double LUMI = 4974.;
  double LUMIERROR = 0; // relative error on luminosity
  double JES=1.0;        // JES "value"
  double JER=1.0;        // JER "value"
  double JESERROR=0.022;   // relative error on JES
  double JERERROR=0.1;   // relative error on JER
  double NSIGCUTOFF=3.0; // number of +/- "sigma" to cutoff integration of nuisance parameters
  bool USELOGNORM=true;  // use lognormal or gaussian nuisance prior pdfs
  bool USEJEFFREYS=false;  // use jeffreys instead of flat prior
  bool USENEWCOVARIANCE=true;  // Dinkos code for the fit eigenvectors
  bool WRITE=false;


 //Calcul of LUMIERROR for each mass from 800 to 1500

//*****************************************************************************************

 //opening the files
 string name0;
 if (doAggressiveBkgFit) {name0 = "../data/newAggressiveSystematic.txt";}
 else {name0 = "../data/newSystematic.txt";}


 std::ifstream myStream(name0.c_str());
 
 //Definition of 8 tables for the different 
 int const n0 = 4;
 Double_t masses[n0]; //one for masses
 Double_t effMus[n0]; //one for 
 Double_t effEs[n0]; //one for 
 Double_t lumiMus[n0]; //one for 
 Double_t bTaggings[n0]; //one for 
 Double_t JECs[n0]; //one for 
 Double_t bkgs[n0]; //one for backgroung pdf
 Double_t signals[n0]; //one for signal pdf

 //filling the 8 tables with the content of the text file
 for (int i=0; i<n0; i++) {myStream >> masses[i] >> effMus[i] >> effEs[i] >> lumiMus[i] >> bTaggings[i] >> JECs[i] >> bkgs[i] >> signals[i];}

/*
for (int i=0; i<n0; i++) cout << masses[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << effMus[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << effEs[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << lumiMus[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << bTaggings[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << JECs[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << bkgs[i] << endl;
cout << endl;
for (int i=0; i<n0; i++) cout << signals[i] << endl;
cout << endl;
*/


 //Definition of 8 tables for the different 
 Double_t allMasses[8] = {800, 900, 1000, 1100, 1200, 1300, 1400, 1500}; //one for masses
 Double_t allEffMus[8]; //one for 
 Double_t allEffEs[8]; //one for 
 Double_t allLumiMus[8]; //one for 
 Double_t allBTaggings[8]; //one for 
 Double_t allJECs[8]; //one for 
 Double_t allBkgs[8]; //one for backgroung pdf
 Double_t allSignals[8]; //one for signal pdf

 for (int i=0; i<8; i++)
  {

  if (allMasses[i]>750 && allMasses[i]<1000) 
    {
    allEffMus[i] = effMus[0] + (effMus[1] - effMus[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    allEffEs[i] = effEs[0] + (effEs[1] - effEs[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    allLumiMus[i] = lumiMus[0] + (lumiMus[1] - lumiMus[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    allBTaggings[i] = bTaggings[0] + (bTaggings[1] - bTaggings[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    allJECs[i] = JECs[0] + (JECs[1] - JECs[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    allBkgs[i] = bkgs[0] + (bkgs[1] - bkgs[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    allSignals[i] = signals[0] + (signals[1] - signals[0])/(masses[1] - masses[0])*(allMasses[i] - masses[0]); 
    }

  else if (allMasses[i]==1000) 
    {
    allEffMus[i]=effMus[2]; 
    allEffEs[i]=effEs[2]; 
    allLumiMus[i]=lumiMus[2]; 
    allBTaggings[i]=bTaggings[2]; 
    allJECs[i]=JECs[2]; 
    allBkgs[i]=bkgs[2]; 
    allSignals[i]=signals[2]; 
    }

  else if (allMasses[i]>1000 && allMasses[i]<1250) 
    {
    allEffMus[i] = effMus[1] + (effMus[2] - effMus[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    allEffEs[i] = effEs[1] + (effEs[2] - effEs[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    allLumiMus[i] = lumiMus[1] + (lumiMus[2] - lumiMus[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    allBTaggings[i] = bTaggings[1] + (bTaggings[2] - bTaggings[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    allJECs[i] = JECs[1] + (JECs[2] - JECs[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    allBkgs[i] = bkgs[1] + (bkgs[2] - bkgs[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    allSignals[i] = signals[1] + (signals[2] - signals[1])/(masses[2] - masses[1])*(allMasses[i] - masses[1]); 
    }

  else if (allMasses[i]>1250 && allMasses[i]<1500)
    {
    allEffMus[i] = effMus[2] + (effMus[3] - effMus[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 
    allEffEs[i] = effEs[2] + (effEs[3] - effEs[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 
    allLumiMus[i] = lumiMus[2] + (lumiMus[3] - lumiMus[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 
    allBTaggings[i] = bTaggings[2] + (bTaggings[3] - bTaggings[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 
    allJECs[i] = JECs[2] + (JECs[3] - JECs[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 
    allBkgs[i] = bkgs[2] + (bkgs[3] - bkgs[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 
    allSignals[i] = signals[2] + (signals[3] - signals[2])/(masses[3] - masses[2])*(allMasses[i] - masses[2]); 

    allSignals[i] = allSignals[2] + (allSignals[3] - allSignals[2])/(masses[3] - masses[2])*(allMasses[i] - allMasses[2]); 
    }

  else if (allMasses[i]==1500) 
    {
    allEffMus[i]=effMus[3]; 
    allEffEs[i]=effEs[3]; 
    allLumiMus[i]=lumiMus[3]; 
    allBTaggings[i]=bTaggings[3]; 
    allJECs[i]=JECs[3]; 
    allBkgs[i]=bkgs[3]; 
    allSignals[i]=signals[3]; 
    }
  }
 
 //Definition and filling of a last table with real systematics (quadratic sums)

 Double_t realLUMIERRORS[8];

 if (yieldIncluded)
   for (int i=0; i<8; i++) realLUMIERRORS[i] = sqrt( pow(allEffMus[i],2) + pow(allEffEs[i],2) + pow(allLumiMus[i],2) + pow(allBTaggings[i],2) + pow(allJECs[i],2) + pow(allSignals[i],2) );
 else for (int i=0; i<8; i++) realLUMIERRORS[i] = sqrt( pow(allEffMus[i],2) + pow(allEffEs[i],2) + pow(allLumiMus[i],2) + pow(allBTaggings[i],2) );

/*
for (int i=0; i<8; i++) cout << allEffMus[i] << endl;
cout << endl;
for (int i=0; i<8; i++) cout << allEffEs[i] << endl;
cout << endl;
for (int i=0; i<8; i++) cout << allLumiMus[i] << endl;
cout << endl;
for (int i=0; i<8; i++) cout << allBTaggings[i] << endl;
cout << endl;
for (int i=0; i<8; i++) cout << allJECs[i] << endl;
cout << endl;
for (int i=0; i<8; i++) cout << allBkgs[i] << endl;
cout << endl;
for (int i=0; i<8; i++) cout << allSignals[i] << endl;
cout << endl;
*/

  //Selection of the LUMIERROR adapted to signalMass

  for (int i=0; i<=8; i++)
    {
    if (i<8)
      {
      if (signalMass == allMasses[i]) 
        {
        LUMIERROR = realLUMIERRORS[i];
        cout << " LUMIERROR = " << LUMIERROR << endl;
        break;
        }
      }
    else
      {
      cout << "Illegal mass !" << endl << "Mass has to be 800, 900, 1000, 1100, 1200, 1300, 1400 or 1500 (GeV)." << endl;
      return -1;
      }
    }

//*****************************************************************************************


  WRITE = verbose_;

  

  // creation and opening of the text file for the result

//*****************************************************************************************

  string name;

  if (negativeSignalAuthorized)
    {
    if (yieldIncluded)
      {
      if (doAggressiveBkgFit)
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_aggressiveFit_withYield_negativeSignal_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        else name = Form("limit_%d_%d_%d_aggressiveFit_withYield_negativeSignal_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      else
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_withYield_negativeSignal_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
          else name = Form("limit_%d_%d_%d_withYield_negativeSignal_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      }
    else
      {
      if (doAggressiveBkgFit)
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_aggressiveFit_negativeSignal_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        else name = Form("limit_%d_%d_%d_aggressiveFit_negativeSignal_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      else
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_negativeSignal_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        else name = Form("limit_%d_%d_%d_negativeSignal_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      }
    }
  else
    {
    if (yieldIncluded)
      {
      if (doAggressiveBkgFit)
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_aggressiveFit_withYield_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        else name = Form("limit_%d_%d_%d_aggressiveFit_withYield_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      else
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_withYield_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
          else name = Form("limit_%d_%d_%d_withYield_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      }
    else
      {
      if (doAggressiveBkgFit)
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_aggressiveFit_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        else name = Form("limit_%d_%d_%d_aggressiveFit.txt_0%.0f",statlevel, numPEs, nIteration, signalMass);
        }
      else
        {
        if (signalMass>=1000) name = Form("limit_%d_%d_%d_%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        else name = Form("limit_%d_%d_%d_0%.0f.txt",statlevel, numPEs, nIteration, signalMass);
        }
      }
    }

  resultats.open(name.c_str());


//*****************************************************************************************



  // setup label
  char label[100];
  sprintf(label, "%2.0f_%d", signalMass, statlevel);

  // setup workspace
  RooWorkspace *ws = new RooWorkspace("ws");

  // observable
  ws->factory("invmass[0]");
  RooRealVar* invmass=ws->var("invmass");
  invmass->setRange(MININVMASS, MAXINVMASS);

  // background
  int number_of_parameters = 4;
  ws->factory("EXPR::background('pow(1.0-invmass/7000.0,p1)/pow(invmass/7000.0,p2+p3*log(invmass/7000.0))', p1[7.460,-30,30], p2[5.882,-20,20], p3[0.106,-5,5],invmass)");
  ws->factory("EXPR::backgroundb('pow(1-invmass/7000.0+pb3*(invmass/7000.0)*(invmass/7000.0),pb1)/pow(invmass/7000,pb2)',pb1[8.41,0,100],pb2[5.20,-100,100], pb3[-0.017,-1,1],invmass)");
  ws->factory("EXPR::backgroundc('pow(1-invmass/7000.0,pc1)/pow(invmass/7000,pc2)',pc1[7.8,-100,100],pc2[5.4,-100,100],pc3[0,0,0],invmass)");

  // data
  int ndatabins=(MAXINVMASS-MININVMASS);
  RooDataHist* binnedData=makeBinnedData(DATASETFN, "binnedData", invmass, ndatabins, MININVMASS, MAXINVMASS);
  ws->import(*binnedData);
  
  // signal rate
  if(USELOGNORM) ws->factory("RooLognormal::lumi_prior(lumi[0], lumiM0[0], lumiK0[1])");
  else           ws->factory("RooGaussian::lumi_prior(lumi[0], lumiM0[0], lumiK0[1])");
  ws->var("lumi")->setVal(LUMI);
  ws->var("lumi")->setRange(std::max(0.0, LUMI-NSIGCUTOFF*LUMI*LUMIERROR), LUMI+NSIGCUTOFF*LUMI*LUMIERROR);
  ws->var("lumi")->setConstant();
  ws->var("lumiM0")->setVal(LUMI);
  if(USELOGNORM) ws->var("lumiK0")->setVal(LUMIERROR+1);
  else           ws->var("lumiK0")->setVal(LUMI*LUMIERROR);
  ws->factory("prod::nsig(xs[0], lumi)");

  ws->factory("iResonance[1]");
  ws->var("iResonance")->setVal(iResonance);

  // signal nuisance parameters
  ws->factory("sigMass[1000]");
  ws->var("sigMass")->setVal(signalMass);
  if(USELOGNORM) ws->factory("RooLognormal::sigMassDelta_prior(sigMassDelta[0], sigMassDeltaM0[0], sigMassDeltaK0[1])");
  else           ws->factory("RooGaussian::sigMassDelta_prior(sigMassDelta[0], sigMassDeltaM0[0], sigMassDeltaK0[1])");
  if(USELOGNORM) ws->factory("RooLognormal::sigWidthDelta_prior(sigWidthDelta[0], sigWidthDeltaM0[0], sigWidthDeltaK0[1])");
  else           ws->factory("RooGaussian::sigWidthDelta_prior(sigWidthDelta[0], sigWidthDeltaM0[0], sigWidthDeltaK0[1])");
  ws->var("sigMassDelta")->setVal(JES);
  ws->var("sigWidthDelta")->setVal(JER);
  ws->var("sigMassDelta")->setRange(std::max(0.0, JES-NSIGCUTOFF*JESERROR), JES+NSIGCUTOFF*JESERROR);
  ws->var("sigWidthDelta")->setRange(std::max(0.0, JER-NSIGCUTOFF*JERERROR), JER+NSIGCUTOFF*JERERROR);
  ws->var("sigMassDeltaM0")->setVal(JES);
  ws->var("sigWidthDeltaM0")->setVal(JER);
  if(USELOGNORM) ws->var("sigMassDeltaK0")->setVal(JESERROR+1);
  else           ws->var("sigMassDeltaK0")->setVal(JESERROR);
  if(USELOGNORM) ws->var("sigWidthDeltaK0")->setVal(JERERROR+1);
  else           ws->var("sigWidthDeltaK0")->setVal(JERERROR);
  ws->var("sigMassDelta")->setConstant(true);
  ws->var("sigWidthDelta")->setConstant(true);


  // signal shape
  ws->factory("Qstar_qg_3::signal(invmass, sigMassDelta, sigWidthDelta, sigMass, iResonance)");

  // model
  ws->factory("SUM::modela(nsig*signal, nbkg[1000,0,1E8]*background)");
  ws->factory("SUM::modelb(nsig*signal, nbkg*backgroundb)");
  ws->factory("SUM::modelc(nsig*signal, nbkg*backgroundc)");
  ws->defineSet("observables","invmass");
  ws->defineSet("POI","xs");

  if(USEJEFFREYS)
    {
      ws->factory("RooJeffreysPrior::xs_prior(modela,xs,invmass)");
      ws->pdf("xs_prior")->specialIntegratorConfig(kTRUE)->getConfigSection("RooIntegrator1D").setRealValue("maxSteps",3);
    }
  else ws->factory("RooUniform::xs_prior(xs)");

  if(statlevel==0) ws->factory("PROD::prior(xs_prior)");
  if(statlevel==1) ws->factory("PROD::prior(xs_prior)");
  if(statlevel==2) ws->factory("PROD::prior(xs_prior,lumi_prior)");
  if(statlevel==3) ws->factory("PROD::prior(xs_prior,sigMassDelta_prior)");
  if(statlevel==4) ws->factory("PROD::prior(xs_prior,sigWidthDelta_prior)");
  if(statlevel==5) ws->factory("PROD::prior(xs_prior,lumi_prior,sigMassDelta_prior)");
  if(statlevel==6) ws->factory("PROD::prior(xs_prior,lumi_prior,sigMassDelta_prior,sigWidthDelta_prior)");
  if(statlevel==11 || statlevel==12 || statlevel==13 || statlevel==14) ws->factory("PROD::prior(xs_prior)");

  if(statlevel==0) ws->defineSet("nuisSet","");
  if(statlevel==1) ws->defineSet("nuisSet","");
  if(statlevel==2) ws->defineSet("nuisSet","lumi");
  if(statlevel==3) ws->defineSet("nuisSet","sigMassDelta");
  if(statlevel==4) ws->defineSet("nuisSet","sigWidthDelta");
  if(statlevel==5) ws->defineSet("nuisSet","lumi,sigMassDelta");
  if(statlevel==6) ws->defineSet("nuisSet","lumi,sigMassDelta,sigWidthDelta");
  if(statlevel==11 || statlevel==12 || statlevel==13 || statlevel==14) ws->defineSet("nuisSet","");

  // do a background-only fit first
  // exclude window +20% -20% units in width
  double mininvmass=ws->var("invmass")->getMin();
  double maxinvmass=ws->var("invmass")->getMax();
  ws->var("invmass")->setRange("FULL", mininvmass,maxinvmass);
  RooFitResult* fit_for_covariance;
  RooFitResult* fit;

  fit_for_covariance=doFit(std::string("bfita")+label, ws->pdf("modela"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);

  fit=doFit(std::string("bfitb")+label, ws->pdf("modelb"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);

  fit=doFit(std::string("bfitc")+label, ws->pdf("modelc"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);

  // integrate over model to get xs estimate as input to the B+S fit
  double pdfIntegral=ws->var("nbkg")->getVal()*calcPDF1DIntegral(ws->pdf("modela"), invmass, signalMass*0.9, signalMass*1.1);
  double maxXS = sqrt(pdfIntegral)*5/ws->var("lumi")->getVal();
  RooRealVar* xs=ws->var("xs");
  if (signalMass==1000) maxXS = maxXS*2.0*5;
  if (signalMass>1099 && signalMass<1401) maxXS = maxXS*2.1*5;
  if (signalMass==1500) maxXS = maxXS*2.0*5;
  if (signalMass>1599 && signalMass<1601) maxXS = maxXS*2.5*5;
  if (signalMass>1699 && signalMass<2201) maxXS = maxXS*3.2*5;
  if (signalMass>2299 && signalMass<3401) maxXS = maxXS*4.0*5;
  if (signalMass>3499 && signalMass<3501) maxXS = maxXS*4.2*5;
  if (signalMass>3599 && signalMass<3701) maxXS = maxXS*4.0*5;
  if (signalMass>3799 && signalMass<3801) maxXS = maxXS*4.8*5;
  if (signalMass>3899) maxXS = maxXS*8.0*5;
  if (negativeSignalAuthorized) {xs->setRange(-maxXS,maxXS);} //allow the signal to be negative
  else {xs->setRange(0,maxXS);} //forbidd the signal to be negative
  xs->setVal(maxXS/5.0);

  if(!doAggressiveBkgFit) {
    // Fit s+b with signal floating
    xs->setConstant(false);
    fit=doFit(std::string("bsfita")+label, ws->pdf("modela"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);
    // Rerun s+b fit with fixed signal, to get a proper covariance matrix for the bg parameters
    xs->setConstant(true);
    fit_for_covariance=doFit(std::string("bfita")+label, ws->pdf("modela"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);
    xs->setConstant(false);
  }

  double nbkgValInit=ws->var("nbkg")->getVal();
  double p1ValInit=ws->var("p1")->getVal();
  double p2ValInit=ws->var("p2")->getVal();
  double p3ValInit=ws->var("p3")->getVal();


  for(int PE=0; PE<=numPEs; PE++) {
    if(numPEs>0 && PE==0) continue; // skip the 0th entry if we are running PEs

    system( "ps -v | grep RSS" );
    system( "ps -v | grep dijetStats" );
    

    // replace with pseudodata if we are doing more than zero PE's
    if(numPEs>0) {
      ws->var("nbkg")->setVal(nbkgValInit);
      ws->var("p1")->setVal(p1ValInit);
      ws->var("p2")->setVal(p2ValInit);
      ws->var("p3")->setVal(p3ValInit);
      binnedData=makeBinnedPseudoData(ws->pdf("background"), ws->var("nbkg")->getVal(), "binnedData", invmass, ndatabins, MININVMASS, MAXINVMASS);
      ws->import(*binnedData, kTRUE);
    }
    
    // do the background+signal fit
    xs->setConstant(false);
    char pelabel[120];
    sprintf(pelabel, "%s_pe%d", label, PE);

    if(doAggressiveBkgFit) {
      ws->var("p1")->setConstant(true);
      ws->var("p2")->setConstant(true);
      ws->var("p3")->setConstant(true);
      ws->var("pb1")->setConstant(true);
      ws->var("pb2")->setConstant(true);
      ws->var("pb3")->setConstant(true);
      ws->var("pc1")->setConstant(true);
      ws->var("pc2")->setConstant(true);
      ws->var("pc3")->setConstant(true);
      ws->var("nbkg")->setConstant(true);
      
      fit=doFit(std::string("bsfita")+pelabel, ws->pdf("modela"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);
    } else
      {
	fit=doFit(std::string("bsfita")+pelabel, ws->pdf("modela"), binnedData, invmass, ws->function("nsig"), ws->var("nbkg"), NBINS-1, BOUNDARIES, "FULL", 0, verbose_);
      }

    // set parameters for limit calculation
    if(statlevel==1 || statlevel==11 || statlevel==12 || statlevel==13 || statlevel==14) {
    } else if(statlevel==2) {
      ws->var("lumi")->setConstant(false);
    } else if(statlevel==3) {
      ws->var("sigMassDelta")->setConstant(false);
    } else if(statlevel==4) {
      ws->var("sigWidthDelta")->setConstant(false);
    } else if(statlevel==5) {
      ws->var("lumi")->setConstant(false);
      ws->var("sigMassDelta")->setConstant(false);
    } else if(statlevel==6) {
      ws->var("lumi")->setConstant(false);
      ws->var("sigMassDelta")->setConstant(false);
      ws->var("sigWidthDelta")->setConstant(false);
    }


    //allow the signal to be negative

    if (negativeSignalAuthorized)
      {
      if (xs->getVal()<0)
        xs->setVal(0);
      xs->setRange(0,maxXS);
      }

    
    // setup model config
    ModelConfig modelConfigA("modelConfigA");
    modelConfigA.SetWorkspace(*ws);
    modelConfigA.SetPdf(*ws->pdf("modela"));
    modelConfigA.SetPriorPdf(*ws->pdf("prior"));
    modelConfigA.SetParametersOfInterest(*ws->set("POI"));
    modelConfigA.SetNuisanceParameters(*ws->set("nuisSet"));
    ModelConfig modelConfigB("modelConfigB");
    modelConfigB.SetWorkspace(*ws);
    modelConfigB.SetPdf(*ws->pdf("modelb"));
    modelConfigB.SetPriorPdf(*ws->pdf("prior"));
    modelConfigB.SetParametersOfInterest(*ws->set("POI"));
    modelConfigB.SetNuisanceParameters(*ws->set("nuisSet"));
    ModelConfig modelConfigC("modelConfigC");
    modelConfigC.SetWorkspace(*ws);
    modelConfigC.SetPdf(*ws->pdf("modelc"));
    modelConfigC.SetPriorPdf(*ws->pdf("prior"));
    modelConfigC.SetParametersOfInterest(*ws->set("POI"));
    modelConfigC.SetNuisanceParameters(*ws->set("nuisSet"));
    
    ws->import(modelConfigA);
    ws->import(modelConfigB);
    ws->import(modelConfigC);
    ws->Print();
    printVal(*ws->var("xs"));
    printVal(*ws->var("nbkg"));
    printVal(*ws->var("lumi"));
    printVal(*ws->var("lumiM0"));
    printVal(*ws->var("lumiK0"));
    printVal(*ws->var("sigMassDelta"));
    printVal(*ws->var("sigMassDeltaM0"));
    printVal(*ws->var("sigMassDeltaK0"));
    printVal(*ws->var("sigWidthDelta"));
    printVal(*ws->var("sigWidthDeltaM0"));
    printVal(*ws->var("sigWidthDeltaK0"));
    printVal(*ws->var("p1"));
    printVal(*ws->var("p2"));
    printVal(*ws->var("p3"));
    printVal(*ws->var("pb1"));
    printVal(*ws->var("pb2"));
    printVal(*ws->var("pb3"));
    printVal(*ws->var("pc1"));
    printVal(*ws->var("pc2"));
    printVal(*ws->var("pc3"));
    if (verbose_) ws->writeToFile("ws.root");
    else if (!verbose_ && WRITE){
      string title("ws");
      title = title + "_" + sResonance + "_" + Form("%f", signalMass) + "_" + Form("%d",statlevel) + ".root";
      ws->writeToFile(title.c_str());
    }

    double lower=-1, upper=-1;
    int niters=nIteration;
    double alpha=0.05;
    double lstail=0.0;
    JPMCCalculator mcA(*binnedData, modelConfigA);
    mcA.SetNumIterations(niters);
    mcA.SetConfidenceLevel(1-alpha);
    mcA.SetLeftSideTailFraction(lstail);
    JPMCCalculator mcB(*binnedData, modelConfigB);
    mcB.SetNumIterations(niters);
    mcB.SetConfidenceLevel(1-alpha);
    mcB.SetLeftSideTailFraction(lstail);
    JPMCCalculator mcC(*binnedData, modelConfigC);
    mcC.SetNumIterations(niters);
    mcC.SetConfidenceLevel(1-alpha);
    mcC.SetLeftSideTailFraction(lstail);
    
    double p1val=ws->var("p1")->getVal(); //double p1err=ws->var("p1")->getError();
    double p2val=ws->var("p2")->getVal(); //double p2err=ws->var("p2")->getError();
    double p3val=ws->var("p3")->getVal(); //double p3err=ws->var("p3")->getError();


    TH1D* histA=dynamic_cast<TH1D*>(mcA.GetPosteriorHist()->Clone("histA"));


    if(statlevel==1 || statlevel==5 || statlevel==6 || statlevel==7 || statlevel==9 || statlevel==11 || statlevel==12 || statlevel==13 || statlevel==14) {

      if (statlevel==11 || statlevel==6){
        TH1D* histB=dynamic_cast<TH1D*>(mcB.GetPosteriorHist()->Clone("histB"));
        TH1D* histC=dynamic_cast<TH1D*>(mcC.GetPosteriorHist()->Clone("histC"));
	histA->Add(histB);
	histA->Add(histC);
	delete histB;
	delete histC;
      }
      
      else if (statlevel==12){
	TH1D* histB=dynamic_cast<TH1D*>(mcB.GetPosteriorHist()->Clone("histB"));
	histA->Add(histB);
	delete histB;
      }
      
      else if (statlevel==13){
	TH1D* histB=dynamic_cast<TH1D*>(mcB.GetPosteriorHist()->Clone("histB"));
	delete histA;
	histA=histB;
      }
      
      else if (statlevel==14){
	TH1D* histC=dynamic_cast<TH1D*>(mcC.GetPosteriorHist()->Clone("histC"));
	delete histA;
	histA=histC;
      }
      
      else {
        // Code taken from TFitResult.RandomizePars
        Int_t nPar = number_of_parameters; // fit_for_covariance->floatParsFinal().getSize();
        // calculate the elements of the upper-triangular matrix L that gives Lt*L = C
        // where Lt is the transpose of L (the "square-root method")
        TMatrix L(nPar,nPar);
        for(Int_t iPar= 0; iPar < nPar; iPar++) {
     	  // calculate the diagonal term first
     	  L(iPar,iPar)= fit_for_covariance->covarianceMatrix()(iPar,iPar);
     	  for(Int_t k= 0; k < iPar; k++) {
     	    Double_t tmp= L(k,iPar);
     	    L(iPar,iPar)-= tmp*tmp;
     	  }
     	  L(iPar,iPar)= sqrt(L(iPar,iPar));
     	  // then the off-diagonal terms
     	  for(Int_t jPar= iPar+1; jPar < nPar; jPar++) {
     	    L(iPar,jPar)= fit_for_covariance->covarianceMatrix()(iPar,jPar);
     	    for(Int_t k= 0; k < iPar; k++) {
     	      L(iPar,jPar)-= L(k,iPar)*L(k,jPar);
     	    }
     	    L(iPar,jPar)/= L(iPar,iPar);
     	  }
        }
        // remember Lt
        TMatrix* _Lt= new TMatrix(TMatrix::kTransposed,L);
        TVectorD eigenValues(4);
        TMatrixD eigenVectors=fit_for_covariance->covarianceMatrix().EigenVectors(eigenValues);
	if(USENEWCOVARIANCE)
	    eigenValues.Sqrt(); // Turn variances into errors 
 
         histA->Scale(3); // 3 times the central value plus up/down variations along 3 axes
         TVector g(nPar);
	 for(Int_t v=1; v<nPar;v++){
          if(USENEWCOVARIANCE)
          {
	      for(Int_t k= 0; k < nPar; k++) g(k)=eigenValues(v)*eigenVectors[k][v]*(eigenVectors[0][v])/fabs(eigenVectors[0][v]); 
	  } else {
	      for(Int_t k= 0; k < nPar; k++) g(k)=eigenVectors[v][k];
              // multiply this vector by Lt to introduce the appropriate correlations
              g*= (*_Lt);
	  }
	  ws->var("p1")->setVal(p1val+g(1));
	  ws->var("p2")->setVal(p2val+g(2));
	  ws->var("p3")->setVal(p3val+g(3));
	  TH1D* histAHi=dynamic_cast<TH1D*>(mcA.GetPosteriorHistForce()->Clone("histAHi"));
	  histA->Add(histAHi);
	  delete histAHi;
          if(USENEWCOVARIANCE)
          {
	      for(Int_t k= 0; k < nPar; k++) g(k)=-eigenValues(v)*eigenVectors[k][v]*(eigenVectors[0][v])/fabs(eigenVectors[0][v]); 
	  } else {
	      for(Int_t k= 0; k < nPar; k++) g(k)=-eigenVectors[v][k];
              // multiply this vector by Lt to introduce the appropriate correlations
              g*= (*_Lt);
	  }
	  ws->var("p1")->setVal(p1val+g(1));
	  ws->var("p2")->setVal(p2val+g(2));
	  ws->var("p3")->setVal(p3val+g(3));
	  TH1D* histALo=dynamic_cast<TH1D*>(mcA.GetPosteriorHistForce()->Clone("histALo"));
	  histA->Add(histALo);
	  delete histALo;
	 }      
      }
    }



    TH1* hPosteriorHist=histA;

    if (verbose_) {
      TCanvas* canvas = new TCanvas("mcPost","Posterior Distribution",500,500);
      hPosteriorHist->Draw();
      canvas->SaveAs(TString("MCpost")+pelabel+".root");
      delete canvas;
    }

    // Calculate the interval by hand
    double lowerCutOff = lstail*alpha;
    double upperCutOff = 1.-(1.-lstail)*alpha;
    TH1D* hCdf = (TH1D*)hPosteriorHist->Clone("hCdf");
    for(int bin=1; bin<=hCdf->GetNbinsX()+1; ++bin) {
      double last=hCdf->GetBinContent(bin-1);
      double next=hPosteriorHist->GetBinContent(bin);
      hCdf->SetBinContent(bin, last+next);
    }
    hCdf->Scale(1./hPosteriorHist->GetSumOfWeights());
    
    for(int bin=1; bin<=hCdf->GetNbinsX(); bin++) {
      if(lowerCutOff<=hCdf->GetBinContent(bin)) {
	lower=hCdf->GetBinLowEdge(bin);
	break;
      }
    }
    
    for(int bin=hCdf->GetNbinsX(); bin>=1; --bin) {
      if(upperCutOff>=hCdf->GetBinContent(bin)) {
	upper=hCdf->GetBinLowEdge(bin+1);
	break;
      }
    }
    delete hCdf;
    delete histA;

    // print the results
    cout << "PE = " << PE << " ; "
         << "m0 = " << signalMass << " ; "
	 << "statlevel = " << statlevel << " ; "
	 << "lowerlimit = " << lower << " ; "
	 << "upperlimit = " << upper << endl;

    if (PE > 0) upper_limits.push_back(upper);
    
    resultats << signalMass << "\t" << upper << endl;

    t.Print();  

  }


  double median_ = 0;
  std::pair<double, double> onesigma_;
  std::pair<double, double> twosigma_;

  DijetHelper::getQuantiles(upper_limits, median_, onesigma_, twosigma_);

  cout << " maxXS = " << maxXS << " ; "
       << "m0 = " << signalMass << " ; "
       << " statlevel = " << statlevel << " ; "
       << " twosigma_lower = " << twosigma_.first << " ; "
       << " onesigma_lower = " << onesigma_.first << " ; "
       << " median = " << median_ << " ; "
       << " onesigma_upper = " << onesigma_.second << " ; "
       << " twosigma_upper = " << twosigma_.second << endl << endl << endl;


resultats.close();

}


