
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "UserCode/EWKV/interface/MacroUtils.h"
#include "UserCode/EWKV/interface/SmartSelectionMonitor.h"
#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"
#include "UserCode/EWKV/interface/TMVAUtils.h"
#include "UserCode/EWKV/interface/PDFInfo.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TEventList.h"
#include "TROOT.h"
#include "TNtuple.h"
#include <Math/VectorUtil.h>

using namespace std;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >::BetaVector BetaVector;


//
float getAngle(LorentzVector &a, LorentzVector &b)
{
  TVector3 mom1(a.px(),a.py(),a.pz());
  TVector3 mom2(b.px(),b.py(),b.pz());
  double cosine = mom1.Dot(mom2)/(mom1.Mag()*mom2.Mag());
  return acos(cosine);
}

//
std::vector<TString> getDijetCategories(double mjj,double etajj,std::vector<TString> &curTags)
{
  TString mjjCat,etajjCat;
  if(mjj<250)               mjjCat="mjjq016";
  if(mjj>=250 && mjj<350)   mjjCat="mjjq033";
  if(mjj>=350 && mjj<450)   mjjCat="mjjq049";
  if(mjj>=450 && mjj<550)   mjjCat="mjjq066";
  if(mjj>=550 && mjj<750)   mjjCat="mjjq083";
  if(mjj>=750 && mjj<1000)  mjjCat="mjjq092";
  if(mjj>=1000)             mjjCat="mjjq100";
  
  //include new tags
  std::vector<TString> selTags;
  for(size_t i=0; i<curTags.size(); i++)
    {
      TString itag=curTags[i];
      selTags.push_back(itag);
      selTags.push_back(itag+mjjCat);
    }
  return selTags;
}


int main(int argc, char* argv[])
{
  //##############################################
  //########    GLOBAL INITIALIZATION     ########
  //##############################################

  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");

  std::vector<int> jacknifeCfg=runProcess.getParameter<std::vector<int> >("jacknife");
  int jacknife(jacknifeCfg[0]), jacks(jacknifeCfg[1]);
  if(jacknife>0 && jacks>0) cout << "Jacknife will be applied to every " << jacknife << " out of " << jacks << " events" << endl;
  
  TString url=runProcess.getParameter<std::string>("input");
  TString baseDir    = runProcess.getParameter<std::string>("dirName");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  bool filterOnlyEE(false), filterOnlyEMU(false), filterOnlyMUMU(false);
  if(!isMC)
    {
      if(url.Contains("DoubleEle")) filterOnlyEE=true;
      if(url.Contains("DoubleMu"))  filterOnlyMUMU=true;
      if(url.Contains("MuEG"))      filterOnlyEMU=true;
    }
  bool isSingleMuPD(!isMC && url.Contains("SingleMu"));  
  bool isV0JetsMC(isMC && (url.Contains("DYJetsToLL_50toInf") || url.Contains("WJets")));
  bool isSignal(isMC && (url.Contains("VBFNLO") || url.Contains("DYJJ")) );

  TString outTxtUrl= outUrl + "/" + outFileUrl + ".txt";
  FILE* outTxtFile = NULL;
  if(!isMC)outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());

  //residual corrections from Z+1 jet recoil
  std::vector<TGraph *> recoilResidualsGr;
  TFile *fIn=TFile::Open("/afs/cern.ch/user/p/psilva/work/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/data/recoilBalance.root");
  if(fIn){
    TGraph *gr=(TGraph *)fIn->Get("mumurecoilbalancevseta50toInfdydata2mc");
    if(gr) recoilResidualsGr.push_back( gr );
    gr=(TGraph *)fIn->Get("mumurecoilbalancevseta50toInfgdata2mc");
    if(gr) recoilResidualsGr.push_back( gr );
    fIn->Close();
    cout << "Read " << recoilResidualsGr.size() << " residual recoil systematics" << endl;
  }
  
  //Q^2 variations
  fIn=TFile::Open("/afs/cern.ch/user/p/psilva/work/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/data/vbfnloQ2weights.root");
  std::vector<TF1 *> Q2weightsGr;
  if(fIn && isSignal){
    TF1 *gr = (TF1 *)fIn->Get("q2upWgt_func");
    if(gr) Q2weightsGr.push_back(gr);
    gr = (TF1 *)fIn->Get("q2downWgt_func");
    if(gr) Q2weightsGr.push_back(gr);
    fIn->Close();
    cout << "Read " << Q2weightsGr.size() << " Q^2-scale re-weighting functions" << endl; 
  }
  
  //summary ntuple
  //  TNtuple *summaryTuple = new TNtuple("ewkzp2j","ewkzp2j","ch:weight:cnorm:mjj:detajj:dphijj:ystar:hardpt:llr:ystar3:maxcjpt:ncjv:htcjv:ncjv15:htcjv15");
  // summaryTuple->SetDirectory(0);

  //MVA
  bool useMVA = runProcess.getParameter<bool>("useMVA");
  TMVA::Reader *tmvaReader = 0;
  std::vector<string> tmvaMethods;
  std::vector<Float_t> tmvaDiscrVals;
  std::vector<std::string> tmvaVarNames;
  std::vector<Float_t> tmvaVars;
  if(useMVA)
    {
      edm::ParameterSet tmvaInput = runProcess.getParameter<edm::ParameterSet>("tmvaInput");
      std::string weightsDir      = tmvaInput.getParameter<std::string>("weightsDir");
      tmvaMethods                 = tmvaInput.getParameter<std::vector<std::string> >("methodList");
      tmvaDiscrVals.resize(tmvaMethods.size(),0);
      tmvaVarNames                = tmvaInput.getParameter<std::vector<std::string> >("varsList");
      tmvaVars.resize( tmvaVarNames.size(), 0 );

      //start the reader for the variables and methods
      tmvaReader = new TMVA::Reader( "!Color:!Silent" );
      for(size_t ivar=0; ivar<tmvaVarNames.size(); ivar++)   tmvaReader->AddVariable( tmvaVarNames[ivar], &tmvaVars[ivar] );

      //open the file with the method description
      for(size_t im=0; im<tmvaMethods.size(); im++)
	{
	  TString weightsFile = weightsDir + "/TMVAClassification_" + tmvaMethods[im] + TString(".weights.xml");
	  gSystem->ExpandPathName(weightsFile);
	  tmvaReader->BookMVA( tmvaMethods[im], weightsFile);
	}
    }

  //lepton efficiencies
  //LeptonEfficiencySF lepEff2012;

  //systematics
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  std::vector<TString> varNames(1,"");
  size_t nvarsToInclude(1);
  if(runSystematics && isMC)
    {
      varNames.push_back("_jerup"); varNames.push_back("_jerdown");
      varNames.push_back("_jesup"); varNames.push_back("_jesdown");
      varNames.push_back("_puup"); varNames.push_back("_pudown");
      if(isSignal)
	{
	  varNames.push_back("_q2up"); varNames.push_back("_q2down");
	  varNames.push_back("_pdfup"); varNames.push_back("_pdfdown");
	  varNames.push_back("_balanceup"); varNames.push_back("_balancedown");
	}
      nvarsToInclude=varNames.size();
      cout << nvarsToInclude << " systematics will be computed for this analysis" << endl;
    }


  //##############################################
  //########    INITIATING HISTOGRAMS     ########
  //##############################################
  SmartSelectionMonitor mon;

  TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  TH1 *h=mon.addHistogram( new TH1F ("eventflow", ";;Events", 5,0,5) );
  h->GetXaxis()->SetBinLabel(1,"#geq 2 leptons");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(3,"p_{T}^{ll}>50");
  h->GetXaxis()->SetBinLabel(4, "#eta^{ll}<1.44");
  h->GetXaxis()->SetBinLabel(5,"#geq 2 jets"); 

  h=mon.addHistogram( new TH1F ("jeteventflow", ";;Events", 5,0,5) );
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"=2 jets");
  h->GetXaxis()->SetBinLabel(4,"=3 jets");
  h->GetXaxis()->SetBinLabel(5,"#geq 4 jets"); 

  //pileup control
  mon.addHistogram( new TH1F( "nvtx",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "nvtxraw",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F( "rho",";#rho;Events",50,0,25) ); 
  mon.addHistogram( new TH1F( "rho25",";#rho(#eta<2.5);Events",50,0,25) ); 

  //lepton control
  mon.addHistogram( new TH1F("ereliso",           ";RelIso;Leptons",50,0,2) );
  mon.addHistogram( new TH1F("mureliso",           ";RelIso;Leptons",50,0,2) );
  mon.addHistogram( new TH1F( "leadpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "leadeta", ";#eta^{l};Events", 50,-2.6,2.6) );
  mon.addHistogram( new TH1F( "trailerpt", ";p_{T}^{l};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "trailereta", ";#eta^{l};Events", 50,-2.6,2.6) );

  //balance histograms
  for(size_t ireg=0; ireg<3; ireg++){
    TString regStr("");
    if(ireg==1) regStr+="30to50";
    if(ireg==2) regStr+="50toInf";
    mon.addHistogram( new TH1F("recoilbalance"+regStr, "; p_{T}(jet)/p_{T}; Jets", 100,0,5) );
    mon.addHistogram( new TH2F("recoilbalancevseta"+regStr, "; #eta(jet); <p_{T}(jet)/p_{T}>", 100,0,5,100,0,5) );
  }
  
  //
  mon.addHistogram( new TH1F( "qt",        ";p_{T}^{#gamma} [GeV];Events / (1 GeV)",1500,0,1500));
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "zptNM1", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 50,-10,10) );
  mon.addHistogram( new TH1F( "zetaNM1", ";#eta^{ll};Events", 50,-10,10) );
  mon.addHistogram( new TH1F( "zy", ";y^{ll};Events", 50,-6,6) );
  mon.addHistogram( new TH1F( "rapidity", ";y^{ll};Events", 50,0,2) );
  mon.addHistogram( new TH1F( "zyNM1", ";y^{ll};Events", 50,-6,6) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,40,250) );

  //jet control
  mon.addHistogram( new TH1F("jetpt"       , ";p_{T} [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("jeteta"       , ";|#eta|;Events",25,0,5) );
  h=mon.addHistogram( new TH1F("njets",  ";Jet multiplicity (p_{T}>30 GeV);Events",5,0,5) );
  
  //vbf control
  mon.addHistogram( new TH2F("njetsvsavginstlumi",  ";;Jet multiplicity (p_{T}>30 GeV);Events",5,0,5,10,0,5000) );
  mon.addHistogram( new TH1F("vbfcandjeteta"     , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjetpt"      , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjet1eta"    , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjet1pt"     , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjet2eta"    , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjet2pt"     , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjetdeta"    , ";|#Delta #eta|;Jets",                50,0,10) );  
  mon.addHistogram( new TH1F("vbfcandjetseta"    , ";#Sigma |#eta|;Jets",                50,0,15) );  
  mon.addHistogram( new TH1F("vbfcandjetetaprod" , ";#eta_{1} . #eta_{2};Jets",       100,-25,25) );
  mon.addHistogram( new TH1F("vbfhardpt"         , ";Hard p_{T} [GeV];Events",                   25,0,250) );
  mon.addHistogram( new TH1F("vbfspt"         , ";S_{p_{T}};Events",                   50,0,1) );
  h=mon.addHistogram( new TH1F("vbfcjv"          , ";Central jet count;Events",                     5,0,5) );
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"=2 jets");
  h->GetXaxis()->SetBinLabel(4,"=3 jets");
  h->GetXaxis()->SetBinLabel(5,"#geq 4 jets");
  mon.addHistogram( (TH1F *) h->Clone("vbfcjv15") );
  mon.addHistogram( (TH1F *) h->Clone("vbfcjv20") );
  mon.addHistogram( new TH1F("vbfhtcjv15"          , ";H_{T}(p_{T}>15) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfhtcjv20"          , ";H_{T}(p_{T}>20) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfhtcjv"            , ";H_{T}(p_{T}>30) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfmaxcjvjpt"        , ";Central jet gap [GeV];Events",50,0,100) );
  mon.addHistogram( new TH1F("vbfystar3"           , ";y_{j3}-(y_{j1}+y_{j2})/2;Events",100,0,5) );

  Double_t mjjaxis[32];
  mjjaxis[0]=0.01;
  for(size_t i=1; i<20; i++)  mjjaxis[i]   =50*i;        //0-1000
  for(size_t i=0; i<5; i++)   mjjaxis[20+i]=1000+100*i; //1000-1500
  for(size_t i=0; i<=5; i++)   mjjaxis[25+i]=1500+300*i; //1500-5000  
  mjjaxis[31]=5000;
  mon.addHistogram( new TH1F("vbfmjj"            , ";M_{jj} [GeV];Events",               31,mjjaxis) );
  mon.addHistogram( new TH1F("vbfdphijj"         , ";#Delta#phi_{jj};Events",            20,0,3.5) );
  mon.addHistogram( new TH1F("vbfystar"           , ";#eta_{ll}-(#eta_{j1}+#eta_{j2})/2;Events",       50,0,5) );
  mon.addHistogram( new TH1F("vbfpt"             , ";p_{T}(visible) [GeV];Dijets",       50,0,500) );
  mon.addHistogram( new TH1F("met"               , ";E_{T}^{miss} [GeV]; Events",        50,0,500) );
  mon.addHistogram( new TH1F("metL"              , ";Axial E_{T}^{miss} [GeV]; Events",  50,-50,200) );
  
  std::vector<TH1 *> tmvaH;
  for(size_t im=0; im<tmvaMethods.size(); im++)
    tmvaH.push_back( mon.addHistogram( tmva::getHistogramForDiscriminator( tmvaMethods[im] ) ) );

  //statistical analysis
  std::vector<double> optim_Cuts2_jet_pt1; 
  std::vector<double> optim_Cuts2_jet_pt2; 
  for(double jet_pt1=30;jet_pt1<=130;jet_pt1+=10)
    {
      for(double jet_pt2=30;jet_pt2<=jet_pt1;jet_pt2+=10)
	{
	  optim_Cuts2_jet_pt1.push_back(jet_pt1);
	  optim_Cuts2_jet_pt2.push_back(jet_pt2);
	} 
    }
  TH2F* Hoptim_cuts2  =(TH2F*)mon.addHistogram(new TProfile2D("optim_cut2",      ";cut index;variable",       optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(), 2, 0, 2)) ;
  Hoptim_cuts2->GetYaxis()->SetBinLabel(1, "jpt1>");
  Hoptim_cuts2->GetYaxis()->SetBinLabel(2, "jpt2>");
  for(unsigned int index=0;index<optim_Cuts2_jet_pt1.size();index++){
    Hoptim_cuts2->Fill(index,0.0,optim_Cuts2_jet_pt1[index]); 
    Hoptim_cuts2->Fill(index,1.0,optim_Cuts2_jet_pt2[index]); 
  }

  TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;
  for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
  {
    Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
    mon.addHistogram( new TH2F (TString("dijet_deta_shapes")+varNames[ivar],";cut index;|#Delta #eta|;Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),25,0,10) );
    if(tmvaH.size()) 
      for(size_t im=0; im<tmvaH.size(); im++)
	{
	  TString hname(tmvaMethods[im].c_str());
	  mon.addHistogram( new TH2F(hname+"_shapes" +varNames[ivar],";cut index;"+TString(tmvaH[im]->GetXaxis()->GetTitle())+";Events",
				     optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),
				     tmvaH[im]->GetXaxis()->GetNbins(),tmvaH[im]->GetXaxis()->GetXmin(),tmvaH[im]->GetXaxis()->GetXmax()) );
	}
  }
  
  //##############################################
  //######## GET READY FOR THE EVENT LOOP ########
  //##############################################

  //open the file and get events tree
  DataEventSummaryHandler evSummary;
  TFile *file = TFile::Open(url);
  printf("Looping on %s\n",url.Data());
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummary.attach( (TTree *) file->Get(baseDir+"/data") ) ) { file->Close();  return -1; }
  const Int_t totalEntries= evSummary.getEntries();
 
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC){
    TH1F* cutflowH = (TH1F *) file->Get(baseDir+"/cutflow");
    if(cutflowH) cnorm=cutflowH->GetBinContent(1);
    printf("cnorm = %f\n",cnorm);
  }
  Hcutflow->SetBinContent(1,cnorm);
  double xsecWeight = xsec;
  if(!isMC) xsecWeight=1.0;

  //jet energy scale uncertainties 
  TString jesUncFile = runProcess.getParameter<std::string>("jesUncFileName");
  gSystem->ExpandPathName(jesUncFile);
  JetCorrectionUncertainty *totalJESUnc = new JetCorrectionUncertainty(jesUncFile.Data());
  
  //pdf info
  PDFInfo *mPDFInfo=0;
  if(isMC)
    {
      TString pdfUrl(url);
      pdfUrl.ReplaceAll(".root","_pdf.root");
      pdfUrl.ReplaceAll("/MC","/pdf/MC");
      mPDFInfo=new PDFInfo(pdfUrl,"cteq66.LHgrid");
      for(int i=0; i<mPDFInfo->numberPDFs(); i++)
	{
	  TString var("_"); var+=i;
	  mon.addHistogram( new TH1F("vbfcandjetdeta"+var    , ";|#Delta #eta|;Jets",                        50,0,10) );
	  mon.addHistogram( new TH1F("vbfcandjet1eta"+var    , ";#eta;Jets",                                 50,0,5) );
	  mon.addHistogram( new TH1F("vbfcandjet1pt"+var     , ";p_{T} [GeV];Jets",                        50,0,500) );
	  mon.addHistogram( new TH1F("vbfcandjet2eta"+var    , ";#eta;Jets",                                 50,0,5) );
	  mon.addHistogram( new TH1F("vbfcandjet2pt"+var     , ";p_{T} [GeV];Jets",                        50,0,500) );
	}
    }

  //pileup weighting: based on vtx for now...
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC){
    TString puDist(baseDir+"/pileup");
    TH1F* histo = (TH1F *) file->Get(puDist);
    if(!histo) std::cout<<"pileup histogram is null!!!\n";
    for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
    delete histo;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights= isMC ? new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution): 0;
  utils::cmssw::PuShifter_t PuShifters;
  if(isMC) { PuShifters=utils::cmssw::getPUshifters(dataPileupDistribution,0.05); }

  //##############################################
  //########           EVENT LOOP         ########
  //##############################################
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  DuplicatesChecker duplicatesChecker;
  int nDuplicates(0);
  int step(totalEntries/100); 
  for( int iev=0; iev<totalEntries; iev++ ) 
    {
      if(iev%step==0){printf(".");fflush(stdout);}
      if(!isMC && jacknife>0 && jacks>0 && iev%jacks==jacknife) continue;
      
      //##############################################   EVENT LOOP STARTS   ##############################################
      //load the event content from tree
      evSummary.getEntry(iev);
      DataEventSummary &ev = evSummary.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }

      if(isV0JetsMC && ev.nup>5)                          continue; 

      data::PhysicsObjectCollection_t leptons=evSummary.getPhysicsObject(DataEventSummaryHandler::LEPTONS);
      data::PhysicsObjectCollection_t jets=evSummary.getPhysicsObject(DataEventSummaryHandler::JETS);
      data::PhysicsObjectCollection_t met=evSummary.getPhysicsObject(DataEventSummaryHandler::MET);
      data::PhysicsObjectCollection_t gen=evSummary.getPhysicsObject(DataEventSummaryHandler::GENPARTICLES);
      
      //require compatibilitiy of the event with the PD
      bool eeTrigger          = ev.t_bits[0];
      //bool emuTrigger         = ev.t_bits[4] || ev.t_bits[5];
      bool mumuTrigger        = ev.t_bits[2] || ev.t_bits[3];
      if(filterOnlyEE)   {                   mumuTrigger=false; }
      if(filterOnlyEMU)  { eeTrigger=false;  mumuTrigger=false; }
      if(filterOnlyMUMU) { eeTrigger=false;  
	if(isSingleMuPD)   
	  {
	    bool muTrigger = ev.t_bits[6];
	    if( mumuTrigger || !muTrigger ) mumuTrigger= false;
	  }
      }

      //prepare the tag's vectors for histo filling
      std::vector<TString> tags(1,"all");

      //pileup weight
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight            = LumiWeights->weight(ev.ngenITpu);
        TotalWeight_plus  = PuShifters[utils::cmssw::PUUP]->Eval(ev.ngenITpu);
        TotalWeight_minus = PuShifters[utils::cmssw::PUDOWN]->Eval(ev.ngenITpu);
      }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);
      Hcutflow->Fill(5,weight);

      //
      //
      // BELOW FOLLOWS THE ANALYSIS OF THE MAIN SELECTION WITH N-1 PLOTS
      //
      //
      
      //
      // LEPTON ANALYSIS
      //
      data::PhysicsObjectCollection_t selLeptons;
      double llScaleFactor(1.0),llTriggerEfficiency(1.0);
      for(size_t ilep=0; ilep<leptons.size(); ilep++)
	{
	  bool passKin(true),passId(true),passIso(true);
	  int lid=fabs(leptons[ilep].get("id"));
	  TString lepStr( lid==13 ? "mu" : "e");

	  //kinematics
	  float leta = lid==11 ? leptons[ilep].getVal("sceta") : leptons[ilep].eta();
	  if(leptons[ilep].pt()<20)                   passKin=false;
	  if(leta> (lid==11 ? 2.5 : 2.4) )            passKin=false;
	  if(lid==11 && (leta>1.4442 && leta<1.5660)) passKin=false;

	  //id
	  Int_t idbits = leptons[ilep].get("idbits");
	  if(lid==11){
	    if(leptons[ilep].getFlag("isconv"))              passId=false;
	    //if(leptons[ilep].getVal("tk_d0")>0.4)          passId=false;
	    //if(leptons[ilep].getVal("tk_lostInnerHits")>0) passId=false;
	    //if(leptons[ilep].getVal("mvatrig")<0.5)        passId=false;
	    bool isLoose = ((idbits >> 4) & 0x1);
	    if(!isLoose)                                   passId=false;
 	  }
	  else{
	    bool isLoose    = ((idbits >> 8) & 0x1);
	    if(!isLoose)                                   passId=false;
	  }

	  //isolation
	  Float_t gIso    = leptons[ilep].getVal("gIso03");
	  Float_t chIso   = leptons[ilep].getVal("chIso03");
	  Float_t puchIso = leptons[ilep].getVal("puchIso03");  
	  Float_t nhIso   = leptons[ilep].getVal("nhIso03");
	  float relIso= lid==11 ?
	    (TMath::Max(nhIso+gIso-ev.rho*utils::cmssw::getEffectiveArea(11,leptons[ilep].getVal("sceta")),Float_t(0.))+chIso)/leptons[ilep].pt() :
	    (TMath::Max(nhIso+gIso-0.5*puchIso,0.)+chIso)/leptons[ilep].pt()
	    ;
	  if(lid==11){
	    if(relIso>0.15)                                passIso=false;
	  }
	  else{
	    if(relIso>0.12)                                passIso=false;
	  }
	  
	  if(!passId || !passIso || !passKin) continue;
	  selLeptons.push_back(leptons[ilep]);
	}
      std::sort(selLeptons.begin(), selLeptons.end(), data::PhysicsObject_t::sortByPt);

      //at this point check if it's worth continuig
      TString ch("");
      if(selLeptons.size()<2) continue;
      int dilId( leptons[0].get("id") * leptons[1].get("id") );
      if( abs(dilId)==121 && eeTrigger)   ch="ee";
      if( abs(dilId)==169 && mumuTrigger) ch="mumu";
      tags.push_back(ch);
      if(ch=="") continue;
      mon.fillHisto("eventflow",tags,0,weight);
      
      //
      // DILEPTON ANALYSIS
      //
      bool isOs(dilId<0);
      LorentzVector leadingLep(leptons[0].pt()>leptons[1].pt() ? leptons[0]: leptons[1]);
      LorentzVector trailerLep(leptons[0].pt()>leptons[1].pt() ? leptons[1]: leptons[0]);
      LorentzVector zll(leadingLep+trailerLep);
      float zy(zll.Rapidity());
      bool passZmass(fabs(zll.mass()-91)<15);
      bool passZpt(zll.pt()>50);
      bool passZeta(fabs(zy)<1.4442);

      //generator level
      LorentzVector genll;
      for(size_t ig=0; ig<gen.size(); ig++)
	{
	  int pid=abs(gen[ig].get("id"));
	  if(pid!=11 && pid!=13) continue;
	  genll += gen[ig];
	}

      if(passZmass)                         mon.fillHisto("eventflow",tags,1,weight);
      if(passZmass && passZpt)              mon.fillHisto("eventflow",tags,2,weight);
      if(passZmass && passZpt && passZeta)  mon.fillHisto("eventflow",tags,3,weight);
      
      //
      //JET/MET ANALYSIS
      //
      data::PhysicsObjectCollection_t selJets;
      int njets30(0);
      for(size_t ijet=0; ijet<jets.size(); ijet++) 
	{
	  if(jets[ijet].pt()<15 || fabs(jets[ijet].eta())>4.7 ) continue;
	  
	  //cross-clean with selected leptons
	  double minDRlj(9999.);
          for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
            minDRlj = TMath::Min( minDRlj, deltaR(jets[ijet],selLeptons[ilep]) );
          if(minDRlj<0.4) continue;
	  
	  Int_t idbits=jets[ijet].get("idbits");
	  bool passPFloose( ((idbits>>0) & 0x1));
	  if(!passPFloose) continue;
	  
	  //add scale/resolution uncertainties
	  const data::PhysicsObject_t &genJet=jets[ijet].getObject("genJet");
	  std::vector<float> smearPt=utils::cmssw::smearJER(jets[ijet].pt(),jets[ijet].eta(),genJet.pt());
	  jets[ijet].setVal("jer",     isMC ? smearPt[0] : jets[ijet].pt());
	  jets[ijet].setVal("jerup",   isMC ? smearPt[1] : jets[ijet].pt());
	  jets[ijet].setVal("jerdown", isMC ? smearPt[2] : jets[ijet].pt());
	  smearPt=utils::cmssw::smearJES(jets[ijet].pt(),jets[ijet].eta(), totalJESUnc);
	  jets[ijet].setVal("jesup",   isMC ? smearPt[0] : jets[ijet].pt());
	  jets[ijet].setVal("jesdown", isMC ? smearPt[1] : jets[ijet].pt());

	  selJets.push_back(jets[ijet]);
	  if(jets[ijet].pt()>30) njets30++;
	}
      std::sort(selJets.begin(), selJets.end(), data::PhysicsObject_t::sortByPt);
      if(passZmass && passZpt && passZeta && njets30>0)  mon.fillHisto("eventflow",tags,4,weight);
      
      //analyze dijets and activity in the dijet rapidity distance
      float maxPt(0), minPt(0), maxEta(0), minEta(0), maxAbsEta(0), minAbsEta(0);
      float dphijj(0), detajj(0), setajj(0), etaprod(0), ystar(0), mjj(0),ptjj(0),spt(0);
      float hardpt(0);
      int ncjv(0), ncjv15(0),ncjv20(0), htcjv(0), htcjv15(0),htcjv20(0);
      float pt3(0), ystar3(0);
      float ptmiss(0),metL(0);
      if(njets30>=2)
	{
	  LorentzVector jet1=selJets[0];
	  LorentzVector jet2=selJets[1];
	  maxPt=max(jet1.pt(),jet2.pt());
	  minPt=min(jet1.pt(),jet2.pt());
	  maxAbsEta=max(fabs(jet1.eta()),fabs(jet2.eta()));
	  minAbsEta=min(fabs(jet1.eta()),fabs(jet2.eta()));
	  maxEta=max(jet1.eta(),jet2.eta());
	  minEta=min(jet1.eta(),jet2.eta());
	  detajj=fabs(maxEta-minEta);
	  setajj=fabs(maxEta)+fabs(minEta);
	  dphijj=deltaPhi(jet1.phi(),jet2.phi());
	  etaprod=jet1.eta()*jet2.eta(); 
	  ystar=zll.Rapidity()-0.5*(jet1.Rapidity()+jet2.Rapidity());
	  
	  LorentzVector vbfSyst=jet1+jet2;
	  mjj=vbfSyst.mass();
	  ptjj=vbfSyst.pt();
	  spt=vbfSyst.pt()/(jet1.pt()+jet2.pt());

	  LorentzVector hardSyst=vbfSyst+zll; 
	  hardpt=hardSyst.pt();
	  
	  TVector2 boson2(zll.px(),zll.py());
	  TVector2 met2(met[0].px(),met[0].py());
	  ptmiss=met[0].pt();
	  metL=boson2*met2; metL /= -zll.pt();
	  
	  //visible system rest frame
	  //BetaVector vbfBoost         = hardSyst.BoostToCM();
	  //LorentzVector jet1_cm    = ROOT::Math::VectorUtil::boost( jet1, vbfBoost );
	  //LorentzVector jet2_cm    = ROOT::Math::VectorUtil::boost( jet2, vbfBoost );
	  //LorentzVector jj_cm      = ROOT::Math::VectorUtil::boost( vbfSyst, vbfBoost );
	  //LorentzVector z_cm       = ROOT::Math::VectorUtil::boost( zll,  vbfBoost );
	  
	  for(size_t iotherjet=2; iotherjet<selJets.size(); iotherjet++){
	    bool isInRapGap(selJets[iotherjet].eta()>minEta && selJets[iotherjet].eta()<maxEta);
	    if(!isInRapGap) continue;
	    float ipt=selJets[iotherjet].pt();
	    if(ipt>pt3)
	      {
		pt3=ipt;
		ystar3=selJets[iotherjet].Rapidity()-0.5*(jet1.Rapidity()+jet2.Rapidity());
	      }
	    
	    ncjv15++;
	    htcjv15 += ipt;
	    if(ipt>20)
	      {
		ncjv20++;
		htcjv20 += ipt;
		if(ipt>30)
		  {
		    ncjv++;
		    htcjv += ipt;
		  }
	      }
	  }
	}

      //set the variables to be used in the MVA evaluation (independently of its use)
      for(size_t ivar=0; ivar<tmvaVarNames.size(); ivar++) 
	{
	  std::string variable        = tmvaVarNames[ivar];
	  if(variable=="mjj")        tmvaVars[ivar]=mjj;
	  if(variable=="detajj")     tmvaVars[ivar]=detajj;
          if(variable=="spt")        tmvaVars[ivar]=spt;
	}
      if(tmvaReader) {
	for(size_t im=0; im<tmvaMethods.size(); im++)
	  tmvaDiscrVals[im]=tmvaReader->EvaluateMVA( tmvaMethods[im] );
      }
            
      //
      // NOW FOR THE CONTROL PLOTS
      //
      mon.fillHisto("zmass",    tags, zll.mass(), weight);  
      if(passZmass){
	
	//pu control
	mon.fillHisto("nvtx"     ,   tags, ev.nvtx,      weight);
	mon.fillHisto("nvtxraw"  ,   tags, ev.nvtx,      1);
	mon.fillHisto("rho"      ,   tags, ev.rho,       weight);
	
	//Z kinematics control
	mon.fillHisto("zpt"      ,   tags, zll.pt(),     weight);      
	mon.fillHisto("zeta"     ,   tags, zll.eta(),    weight);
	mon.fillHisto("zy"       ,   tags, zy,    weight);
	mon.fillHisto("jeteventflow",tags,njets30,weight);
	
	//balance control
	if(njets30==1){
	  float dphi=deltaPhi(selJets[0].phi(),zll.phi());
	  if(fabs(dphi)>2.7){
	    
	    float recoilPt=selJets[0].pt();
	    float balance(recoilPt/zll.pt());
	    
	    std::vector<TString> zptRegs;
	    zptRegs.push_back("");
	    if(zll.pt()>30 && zll.pt()<50) zptRegs.push_back("30to50");
	    if(zll.pt()>50)                zptRegs.push_back("50toInf");
	    for(size_t ireg=0; ireg<zptRegs.size(); ireg++)
	      {
		mon.fillHisto("recoilbalance"+zptRegs[ireg],tags,balance, weight);
		mon.fillHisto("recoilbalancevseta"+zptRegs[ireg],tags,fabs(selJets[0].eta()), balance, weight);
	      }
	  }
	}
	//end balance control

	if(passZpt && passZeta){
	  
	  //analyze dilepton kinematics
	  mon.fillHisto("leadeta"   ,  tags, leadingLep.eta(), weight);
	  mon.fillHisto("leadpt"    ,  tags, leadingLep.pt(),  weight);
	  mon.fillHisto("trailereta",  tags, trailerLep.eta(), weight);
	  mon.fillHisto("trailerpt",   tags, trailerLep.pt(),  weight);
	  mon.fillHisto("njets",       tags, njets30,          weight);
	  
	  if(njets30>=2)
	    {
	      std::vector<TString> selTags;
	      selTags = getDijetCategories(mjj,detajj,tags);

	      //save for further analysis
	      if(mjj>200) {
		//summaryTuple->Fill(ev.cat,weight*xsecWeight,cnorm,mjj,detajj,dphijj,ystar,hardpt,tmvaDiscrVals[2],ystar3,pt3,ncjv,htcjv,ncjv15,htcjv15);
		for(size_t im=0; im<tmvaMethods.size(); im++) mon.fillHisto(tmvaMethods[im], selTags, tmvaDiscrVals[im], weight);
	      } 
	      
	      mon.fillHisto("qt", selTags, zll.pt(), weight,true);      
	      mon.fillHisto("rapidity"     ,   tags, fabs(zy),    weight);
	      mon.fillHisto("njetsvsavginstlumi", tags, njets30,ev.instLumi,weight);
	      mon.fillHisto("vbfcandjetpt",  selTags, maxPt,weight);
	      mon.fillHisto("vbfcandjetpt",  selTags, minPt,weight);
	      mon.fillHisto("vbfcandjet1pt", selTags, maxPt,weight);
	      mon.fillHisto("vbfcandjet2pt", selTags, minPt,weight);
	      mon.fillHisto("vbfcandjet1eta",     selTags, maxAbsEta, weight);
	      mon.fillHisto("vbfcandjet2eta",     selTags, minAbsEta, weight);
	      mon.fillHisto("vbfcandjeteta",      selTags, maxAbsEta, weight);
	      mon.fillHisto("vbfcandjeteta",      selTags, minAbsEta, weight);
	      mon.fillHisto("vbfcandjetdeta",     selTags, detajj,weight);
	      mon.fillHisto("vbfcandjetseta",     selTags, setajj,weight);
	      mon.fillHisto("vbfcandjetetaprod",  selTags, etaprod,weight);
	      mon.fillHisto("vbfmjj",             selTags, mjj,weight,true);
	      mon.fillHisto("vbfhardpt",          selTags, hardpt,weight);
	      mon.fillHisto("vbfspt",             selTags, spt,weight);
	      mon.fillHisto("vbfdphijj",          selTags, fabs(dphijj),weight);
	      mon.fillHisto("vbfystar",           selTags, ystar,weight);
	      mon.fillHisto("vbfpt",              selTags, ptjj,weight);
	      mon.fillHisto("met",                selTags, ptmiss,weight);
	      mon.fillHisto("metL",               selTags, metL,weight);
	      if(ncjv15){
		mon.fillHisto("vbfmaxcjvjpt",        selTags, pt3,weight);
		mon.fillHisto("vbfystar3",           selTags, ystar3,weight);
	      }
	      mon.fillHisto("vbfcjv",              selTags, ncjv,weight);
	      mon.fillHisto("vbfcjv15",            selTags, ncjv15,weight);
	      mon.fillHisto("vbfcjv20",            selTags, ncjv20,weight);
	      mon.fillHisto("vbfhtcjv",            selTags, htcjv,weight);
	      mon.fillHisto("vbfhtcjv15",          selTags, htcjv15,weight);
	      mon.fillHisto("vbfhtcjv20",          selTags, htcjv20,weight);
	      
	      if(isMC && mPDFInfo)
		{
		  std::vector<float> wgts=mPDFInfo->getWeights(iev);
		  for(size_t ipw=0; ipw<wgts.size(); ipw++) 
		    {
		      TString var("_"); var+=ipw;
		      mon.fillHisto("vbfcandjetdeta"+var,     selTags, fabs(detajj),weight*wgts[ipw]);
		      mon.fillHisto("vbfcandjet1pt"+var,      tags, maxPt,weight*wgts[ipw]);
		      mon.fillHisto("vbfcandjet2pt"+var,      tags, minPt,weight*wgts[ipw]);
		      mon.fillHisto("vbfcandjet1eta"+var,     tags, maxAbsEta,weight*wgts[ipw]);
		      mon.fillHisto("vbfcandjet2eta"+var,     tags, minAbsEta,weight*wgts[ipw]);
		    }
		}
	      	      
	    }//end nAJetsLoose
  
	  //STATISTICAL ANALYSIS
	  float Q2Weight_plus(1.0), Q2Weight_down(1.0);
	  float PDFWeight_plus(1.0), PDFWeight_down(1.0);
	  if(isSignal)
	    {
	      if(mPDFInfo)
		{
		  std::vector<float> wgts=mPDFInfo->getWeights(iev);
		  for(size_t ipw=0; ipw<wgts.size(); ipw++)
		    {
		      PDFWeight_plus = TMath::Max(PDFWeight_plus,wgts[ipw]);
		      PDFWeight_down = TMath::Min(PDFWeight_down,wgts[ipw]);
		    }
		}
	      if(Q2weightsGr.size()==2)
		{
		  Q2Weight_plus = Q2weightsGr[0]->Eval(genll.pt());
		  Q2Weight_down = Q2weightsGr[1]->Eval(genll.pt());
		}
	    }

	  for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
	    float iweight = weight;                                               //nominal
	    if(ivar==5)                        iweight *= TotalWeight_plus;        //pu up
	    if(ivar==6)                        iweight *= TotalWeight_minus;       //pu down
	    if(ivar==7)                        iweight *= Q2Weight_plus;
	    if(ivar==8)                        iweight *= Q2Weight_down;
	    if(ivar==9)                        iweight *= PDFWeight_plus;
	    if(ivar==10)                       iweight *= PDFWeight_down;
	    
	    data::PhysicsObjectCollection_t localSelJets;
	    for(size_t ijet=0; ijet<jets.size(); ijet++){
	      
	      float rawpt=jets[ijet].pt();
	      float pt=rawpt;
	      if(ivar==1) pt=jets[ijet].getVal("jesup");
	      if(ivar==2) pt=jets[ijet].getVal("jesdown");
	      if(ivar==3) pt=jets[ijet].getVal("jerup");
	      if(ivar==4) pt=jets[ijet].getVal("jerdown");
	      if(pt<30 || fabs(jets[ijet].eta())>4.7) continue;
	      
	      Int_t idbits=jets[ijet].get("idbits");
	      bool passPFloose( ((idbits>>0) & 0x1));
	      if(!passPFloose) continue;
	      
	      data::PhysicsObject_t iSelJet(jets[ijet]);
	      iSelJet *= pt/rawpt;
	      localSelJets.push_back( iSelJet );
	    }
	    if(localSelJets.size()<2)  continue;
	    std::sort(localSelJets.begin(), localSelJets.end(),  data::PhysicsObject_t::sortByPt);

	    //recoil residuals uncertainty
	    if( (ivar==11 || ivar==12) && recoilResidualsGr.size()==2)
	      {
		for(size_t ijet=0; ijet<2; ijet++)
		  {
		    float abseta=TMath::Abs(localSelJets[ijet].eta());
		    float sfEnvelope=TMath::Max( TMath::Abs(1-recoilResidualsGr[0]->Eval(abseta)),
						 TMath::Abs(1-recoilResidualsGr[1]->Eval(abseta)) );
		    localSelJets[ijet] *= (ivar==11 ? 1+sfEnvelope : 1-sfEnvelope);
		  }
	      }

	    //re-assign the event category;
	    std::vector<TString> locTags;
	    locTags.push_back(ch);
	    for(unsigned int index=0; index<optim_Cuts2_jet_pt1.size();index++)
	      {
		float minJetPt1=optim_Cuts2_jet_pt1[index];
		float minJetPt2=optim_Cuts2_jet_pt2[index];

		bool passLocalJet1Pt(localSelJets[0].pt()>minJetPt1);
		bool passLocalJet2Pt(localSelJets[1].pt()>minJetPt2);
		if(!passLocalJet1Pt || !passLocalJet2Pt) continue; 
		
		LorentzVector vbfSyst=localSelJets[0]+localSelJets[1];
		float mjj=vbfSyst.M();
		float detajj=fabs(localSelJets[0].eta()-localSelJets[1].eta());
		float spt=vbfSyst.pt()/(localSelJets[0].pt()+localSelJets[1].pt());

		std::vector<TString> localSelTags=getDijetCategories(mjj,detajj,locTags);
		mon.fillHisto(TString("dijet_deta_shapes")+varNames[ivar],localSelTags,index,detajj,iweight);
		
		//set the variables to be used in the MVA evaluation (independently of its use)
		for(size_t mvar=0; mvar<tmvaVarNames.size(); mvar++) 
		  {
		    std::string variable        = tmvaVarNames[mvar];
		    if(variable=="mjj")        tmvaVars[mvar]=mjj;
		    if(variable=="detajj")     tmvaVars[mvar]=detajj;
		    if(variable=="spt")        tmvaVars[mvar]=spt;
		  }
		if(tmvaReader)
		  for(size_t im=0; im<tmvaMethods.size(); im++)
		    {
		      float iTmvaDiscrVal=tmvaReader->EvaluateMVA( tmvaMethods[im] );
		      mon.fillHisto(TString(tmvaMethods[im]+"_shapes")+varNames[ivar],localSelTags,index,iTmvaDiscrVal,iweight);
		    }
	      }
	  }
	}//end passZpt && passZeta
	
	//
	//N-1 CONTROL
	//
	if(           passZeta && (njets30>=2))   { mon.fillHisto("zptNM1"      ,   tags, zll.pt(),     weight); }
	if(passZpt             && (njets30>=2))   { mon.fillHisto("zetaNM1"     ,   tags, zll.eta(),    weight);  mon.fillHisto("zyNM1"     , tags, zy,    weight);}
	if(passZpt && passZeta && (njets30>=2))
	  {
	    mon.fillHisto  ("mjjvsdetajj",       tags, detajj, mjj, weight);
	    mon.fillHisto  ("detajjvsmjj",       tags, mjj,    detajj, weight);
	  }

      }//end passZmass
  }
  
  printf("\n"); 
  file->Close();
  
  //##############################################
  //########     SAVING HISTO TO FILE     ########
  //##############################################
  //save control plots to file
  outUrl += "/";
  outUrl += outFileUrl + ".root";
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

  //save summary tuple
  outUrl.ReplaceAll(".root","_summary.root");
  ofile=TFile::Open(outUrl,"recreate");
  //summaryTuple->SetDirectory(ofile);
  //summaryTuple->Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





