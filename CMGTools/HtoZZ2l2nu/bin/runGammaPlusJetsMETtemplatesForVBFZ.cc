#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/PDFInfo.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include <Math/VectorUtil.h>
#include "Math/GenVector/Boost.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TRandom2.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

using namespace std;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >::BetaVector BetaVector;

LorentzVector min(const LorentzVector& a, const LorentzVector& b)
{
  if(a.pt()<=b.pt())return a;
  return b;
}

float getAngle(LorentzVector &a, LorentzVector &b)
{
  TVector3 mom1(a.px(),a.py(),a.pz());
  TVector3 mom2(b.px(),b.py(),b.pz());
  double cosine = mom1.Dot(mom2)/(mom1.Mag()*mom2.Mag());
  return acos(cosine);
}

//
TString getDijetCategory(double mjj,double etajj)
{
  TString mjjCat;
  if(mjj<250)               mjjCat="mjjq016";
  if(mjj>=250 && mjj<350)   mjjCat="mjjq033";
  if(mjj>=350 && mjj<450)   mjjCat="mjjq049";
  if(mjj>=450 && mjj<550)   mjjCat="mjjq066";
  if(mjj>=550 && mjj<750)   mjjCat="mjjq083";
  if(mjj>=750 && mjj<1000)  mjjCat="mjjq092";
  if(mjj>=1000)             mjjCat="mjjq100";
  return mjjCat;
}

//
std::vector<TString> getDijetCategories(double mjj,double etajj,std::vector<TString> &curTags)
{

  TString mjjCat=getDijetCategory(mjj,etajj);

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


//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //check arguments
  if ( argc < 2 ) 
    {
      std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
      return 0;
    }
  
  //get configuration
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  TString url=runProcess.getParameter<std::string>("input");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");

  TRandom2 rndGen;
  EventCategory eventCategoryInst(0);  //0,>=1jet, VBF
  GammaEventHandler gammaEvHandler(runProcess);  
  TTree *bpjetsT = gammaEvHandler.initSummary();
  bpjetsT->SetDirectory(0);

  //   TMVA::Reader *tmvaReader = 0;
  //   if(runProcess.getParameter<std::vector<std::string> >("weightsFile").size()>0)
  //     {
  //       tmvaReader = new TMVA::Reader( "!Color:!Silent" );
  //       tmvaReader->AddVariable( "qt",      &gammaEvHandler.evSummary_.qt );
  //       tmvaReader->AddVariable( "njets",   &gammaEvHandler.evSummary_.njets );
  //       tmvaReader->AddVariable( "nvtx",    &gammaEvHandler.evSummary_.nvtx );
  //       tmvaReader->AddSpectator("cat",     &gammaEvHandler.evSummary_.cat );
  //       tmvaReader->AddSpectator("weight",  &gammaEvHandler.evSummary_.weight );
  //       tmvaReader->BookMVA("KNN_ee","/afs/cern.ch/user/p/psilva/work/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/bin/G/weights/TMVAClassification_ee_KNN.weights.xml");
  //       tmvaReader->BookMVA("KNN_mumu","/afs/cern.ch/user/p/psilva/work/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/bin/G/weights/TMVAClassification_mumu_KNN.weights.xml");
  //     }
  
  //book histograms
  SmartSelectionMonitor mon;
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;

  mon.addHistogram( new TH1F( "nvtx", ";Vertices;Events", 50,0,50) );  
  mon.addHistogram( new TH1F( "rho", ";#rho;Events", 50,0,25) );  

  //  Float_t qtaxis[100];
  //for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5                                                                                                                                   
  //for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195                                                                                                                                   
  //for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340                                                                                                                                    
  //for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976                                        
  //  mon.addHistogram( new TH1F( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
  mon.addHistogram( new TH1F( "qt",        ";p_{T}^{#gamma} [GeV];Events / (1 GeV)",1500,0,1500));
  mon.addHistogram( new TH1F( "qtraw",        ";p_{T}^{#gamma} [GeV];Events / (1 GeV)",1500,0,1500));
  mon.addHistogram( new TH1F( "sihih",";#sigma_{i-#eta,i-#eta} x 100; Photons/(0.03)",100,0,3));
  mon.addHistogram( new TH1F( "iso",";Isolation [GeV]; Photons/(0.1)",100,0,10));
  mon.addHistogram( new TH1F( "minmgtj",    ";min M(#gamma,tag jet) [GeV];Events / (1 GeV)",250,0,500));
  mon.addHistogram( new TH1F( "minmgj",    ";min M(#gamma,jet) [GeV];Events / (1 GeV)",250,0,500));

  //jet control
  mon.addHistogram( new TH1F("jetpt"       , ";p_{T} [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("jeteta"       , ";|#eta|;Events",25,0,5) );
  mon.addHistogram( new TH1F("btags", ";CSV;Events",50,-0.5,1.2) );
  TH1F *h=(TH1F *)mon.addHistogram( new TH1F("njets",  ";Jet multiplicity (p_{T}>30 GeV);Events",5,0,5) );
  TH1 *hb=mon.addHistogram( new TH1F("nbtags",   ";b-tag multiplicity (CSV);Events",5,0,5) );
  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      TString label("");
      if(ibin==h->GetXaxis()->GetNbins()) label +="#geq";
      else                                label +="=";
      label += (ibin-1);
      h->GetXaxis()->SetBinLabel(ibin,label);
      hb->GetXaxis()->SetBinLabel(ibin,label);
    } 
  mon.addHistogram( new TH1F( "rapidity", ";y^{ll};Events", 50,0,2) );

  //vbf control
  int jetIdToApply=JETID_OPT_LOOSE;
  mon.addHistogram( new TH2F("njetsvsavginstlumi",  ";;Jet multiplicity (p_{T}>30 GeV);Events",5,0,5,10,0,5000) );
  mon.addHistogram( new TH1F("vbfcandjeteta"     , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjetpt"      , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjet1eta"    , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjet1pt"     , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjet2eta"    , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjet2pt"     , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjetdeta"    , ";|#Delta #eta|;Jets",                        50,0,10) );
  mon.addHistogram( new TH1F("vbfcandjetetaprod" , ";#eta_{1} . #eta_{2};Jets",       100,-25,25) );
  mon.addHistogram( new TH1F("vbfhardpt"         , ";Hard p_{T} [GeV];Events",                   25,0,250) ); 
  mon.addHistogram( new TH1F("vbfspt"         , ";S_{p_{T}};Events",                   50,0,1) );

  h=(TH1F *)mon.addHistogram( new TH1F("vbfcjv"          , ";Central jet count;Events",                     5,0,5) );
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"=2 jets");
  h->GetXaxis()->SetBinLabel(4,"=3 jets");
  h->GetXaxis()->SetBinLabel(5,"#geq 4 jets");
  mon.addHistogram( (TH1F *) h->Clone("vbfcjv15") );
  mon.addHistogram( (TH1F *) h->Clone("vbfcjv20") );
  mon.addHistogram( new TH1F("vbfmaxcjvjpt"        , ";Central jet gap [GeV];Events",50,0,100) );
  mon.addHistogram( new TH1F("vbfhtcjv15"           , ";H_{T}(p_{T}>15) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfhtcjv20"           , ";H_{T}(p_{T}>20) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfhtcjv"            , ";H_{T}(p_{T}>30) [GeV];Events",50,0,250) );

  Double_t mjjaxis[32];
  mjjaxis[0]=0.01;
  for(size_t i=1; i<20; i++)  mjjaxis[i]   =50*i;        //0-1000
  for(size_t i=0; i<5; i++)   mjjaxis[20+i]=1000+100*i; //1000-1500
  for(size_t i=0; i<=5; i++)   mjjaxis[25+i]=1500+300*i; //1500-3000                                                                                                                                    
  mjjaxis[31]=5000;
  mon.addHistogram( new TH1F("vbfmjj"            , ";M(jet_{1},jet_{2}) [GeV];Events",31,mjjaxis) );
  mon.addHistogram( new TH1F("vbfdphijj"         , ";#Delta#phi;Events",20,0,3.5) );
  mon.addHistogram( new TH1F("vbfzepp"           , ";#eta_{ll}-<#eta_{j}>;Events",50,0,5) );
  mon.addHistogram( new TH1F("vbfpt"             , ";p_{T} [GeV];Dijets",                           50,0,500) );
  mon.addHistogram( new TH1F("met"               , ";E_{T}^{miss} [GeV]; Events",                   50,0,500) );
  mon.addHistogram( new TH1F("metL"              , ";Axial E_{T}^{miss} [GeV]; Events",             50,-50,200) );
  mon.addHistogram( new TH1F("mt"                , ";M_{T}        [GeV]; Events",                   50,0,500) );
  mon.addHistogram( new TH1F("zpt_cm"            , ";p_{T} [GeV];Dileptons",             50,0,500) );
  mon.addHistogram( new TH1F("jjpt_cm"           , ";p_{T} [GeV];Dijets",                50,0,500) );
  mon.addHistogram( new TH1F("jet_beam_cm"       , ";#theta(j,beam line) [rad];Events",  20,0,3.5) );
  mon.addHistogram( new TH1F("z_beam_cm"         , ";#theta(Z,beam line) [rad];Events",  20,0,3.5) );
  mon.addHistogram( new TH1F("jet_z_cm"          , ";#theta(j,Z) [rad];Events",          20,0,3.5) );
  mon.addHistogram( new TH1F("jet_zstar_cm"      , ";#theta^{*}(j,Z) [rad];Events",      20,0,3.5) );
  mon.addHistogram( new TH1F("jet_zstar_asym_cm" , ";|#Delta|#theta| |/#Sigma |#theta| (j,Z); Events"        ,50,0,1) );
  mon.addHistogram( new TH1F("jet_z_asym_cm"     , ";|#Delta|#theta^{*}| |/#Sigma |#theta^{*}| (j,Z); Events",50,0,1) );
  mon.addHistogram( new TH1F("jet_beam_asym_cm" ,  ";|#Delta|#theta| |/#Sigma |#theta| (j,beam line); Events",50,0,1) );

  mon.addHistogram(  new TProfile("detajjvsmjjprof",     ";M_{jj} [GeV];|#Delta #eta_{jj}|",       10,0,3000) );
  mon.addHistogram(  new TH2F    ("detajjvsmjj",         ";M_{jj} [GeV];|#Delta #eta_{jj}|",       10,0,3000,50,0,5) );
  mon.addHistogram(  new TProfile("dphijjvsmjjprof",     ";M_{jj} [GeV];|#Delta #phi_{jj}|",       10,0,3000) );
  mon.addHistogram(  new TH2F    ("dphijjvsmjj",         ";M_{jj} [GeV];|#Delta #phi_{jj}|",       10,0,3000,20,0,3.5) );
  mon.addHistogram(  new TProfile("ht15vsmjjprof",       ";M_{jj} [GeV];H_{T}(p_{T}>15) [GeV]",    10,0,3000) );
  mon.addHistogram(  new TH2F    ("ht15vsmjj",           ";M_{jj} [GeV];H_{T}(p_{T}>15) [GeV]",    10,0,3000,50,0,250) );
  mon.addHistogram(  new TProfile("ht20vsmjjprof",       ";M_{jj} [GeV];H_{T}(p_{T}>20) [GeV]",    10,0,3000) );
  mon.addHistogram(  new TH2F    ("ht20vsmjj",           ";M_{jj} [GeV];H_{T}(p_{T}>20) [GeV]",    10,0,3000,50,0,250) );
  mon.addHistogram(  new TProfile("htvsmjjprof",         ";M_{jj} [GeV];H_{T}(p_{T}>30) [GeV]",    10,0,3000) );
  mon.addHistogram(  new TH2F    ("htvsmjj",             ";M_{jj} [GeV];H_{T}(p_{T}>30) [GeV]",    10,0,3000,50,0,250) );
  mon.addHistogram(  new TProfile("cjv15vsmjjprof",      ";M_{jj} [GeV];Jets(p_{T}>15)",           10,0,3000) );
  mon.addHistogram(  new TH2F    ("cjv15vsmjj",          ";M_{jj} [GeV];Jets(p_{T}>15)",           10,0,3000,5,0,5) );
  mon.addHistogram(  new TProfile("cjv20vsmjjprof",      ";M_{jj} [GeV];Jets(p_{T}>20)",           10,0,3000) );
  mon.addHistogram(  new TH2F    ("cjv20vsmjj",          ";M_{jj} [GeV];Jets(p_{T}>20)",           10,0,3000,5,0,5) );
  mon.addHistogram(  new TProfile("cjvvsmjjprof",        ";M_{jj} [GeV];Jets(p_{T}>30)",           10,0,3000) );
  mon.addHistogram(  new TH2F    ("cjvvsmjj",            ";M_{jj} [GeV];Jets(p_{T}>30)",           10,0,3000,5,0,5) );
  mon.addHistogram(  new TProfile("leadcjvvsmjjprof",    ";M_{jj} [GeV];Leding central jet [GeV]", 10,0,3000) );
  mon.addHistogram(  new TH2F    ("leadcjvvsmjj",        ";M_{jj} [GeV];Leding central jet [GeV]", 10,0,3000,50,0,100) );
  mon.addHistogram(  new TProfile("leadjetvsmjjprof",    ";M_{jj} [GeV];Leading jet [GeV]",        10,0,3000) );
  mon.addHistogram(  new TH2F    ("leadjetvsmjj",        ";M_{jj} [GeV];Leading jet [GeV]",        10,0,3000,50,0,500) );
  mon.addHistogram(  new TProfile("trailerjetvsmjjprof", ";M_{jj} [GeV];Second jet [GeV]",         10,0,3000) );
  mon.addHistogram(  new TH2F    ("trailerjetvsmjj",      ";M_{jj} [GeV];Second jet [GeV]",        10,0,3000,50,0,500) );

  mon.addHistogram(  new TProfile("mjjvsdetajjprof",        ";|#Delta #eta_{jj}|;M_{jj} [GeV]",             10,0,10) );
  mon.addHistogram(  new TH2F    ("mjjvsdetajj",            ";|#Delta #eta_{jj}|;M_{jj} [GeV]",             10,0,10,10,0.,3000.) );
  mon.addHistogram(  new TProfile("dphijjvsdetajjprof",     ";|#Delta #eta_{jj}|;|#Delta #phi_{jj}|",       10,0,10) );
  mon.addHistogram(  new TH2F    ("dphijjvsdetajj",         ";|#Delta #eta_{jj}|;|#Delta #phi_{jj}|",       10,0,10,20,0,3.5) );
  mon.addHistogram(  new TProfile("ht15vsdetajjprof",       ";|#Delta #eta_{jj}|;H_{T}(p_{T}>15) [GeV]",    10,0,10) );
  mon.addHistogram(  new TH2F    ("ht15vsdetajj",           ";|#Delta #eta_{jj}|;H_{T}(p_{T}>15) [GeV]",    10,0,10,50,0,250) );
  mon.addHistogram(  new TProfile("ht20vsdetajjprof",       ";|#Delta #eta_{jj}|;H_{T}(p_{T}>20) [GeV]",    10,0,10) );
  mon.addHistogram(  new TH2F    ("ht20vsdetajj",           ";|#Delta #eta_{jj}|;H_{T}(p_{T}>20) [GeV]",    10,0,10,50,0,250) );
  mon.addHistogram(  new TProfile("htvsdetajjprof",         ";|#Delta #eta_{jj}|;H_{T}(p_{T}>30) [GeV]",    10,0,10) );
  mon.addHistogram(  new TH2F    ("htvsdetajj",             ";|#Delta #eta_{jj}|;H_{T}(p_{T}>30) [GeV]",    10,0,10,50,0,250) );
  mon.addHistogram(  new TProfile("cjv15vsdetajjprof",      ";|#Delta #eta_{jj}|;Jets(p_{T}>15)",           10,0,10) );
  mon.addHistogram(  new TH2F    ("cjv15vsdetajj",          ";|#Delta #eta_{jj}|;Jets(p_{T}>15)",           10,0,10,5,0,5) );
  mon.addHistogram(  new TProfile("cjv20vsdetajjprof",      ";|#Delta #eta_{jj}|;Jets(p_{T}>20)",           10,0,10) );
  mon.addHistogram(  new TH2F    ("cjv20vsdetajj",          ";|#Delta #eta_{jj}|;Jets(p_{T}>20)",           10,0,10,5,0,5) );
  mon.addHistogram(  new TProfile("cjvvsdetajjprof",        ";|#Delta #eta_{jj}|;Jets(p_{T}>30)",           10,0,10) );
  mon.addHistogram(  new TH2F    ("cjvvsdetajj",            ";|#Delta #eta_{jj}|;Jets(p_{T}>30)",           10,0,10,5,0,5) );
  mon.addHistogram(  new TProfile("leadcjvvsdetajjprof",    ";|#Delta #eta_{jj}|;Leding central jet [GeV]", 10,0,10) );
  mon.addHistogram(  new TH2F    ("leadcjvvsdetajj",        ";|#Delta #eta_{jj}|;Leding central jet [GeV]", 10,0,10,50,0,100) );
  mon.addHistogram(  new TProfile("leadjetvsdetajjprof",    ";|#Delta #eta_{jj}|;Leading jet [GeV]",        10,0,10) );
  mon.addHistogram(  new TH2F    ("leadjetvsdetajj",        ";|#Delta #eta_{jj}|;Leading jet [GeV]",        10,0,10,50,0,500) );
  mon.addHistogram(  new TProfile("trailerjetvsdetajjprof", ";|#Delta #eta_{jj}|;Second jet [GeV]",         10,0,10) );
  mon.addHistogram(  new TH2F    ("trailerjetvsdetajj",     ";|#Delta #eta_{jj}|;Second jet [GeV]",         10,0,10,50,0,500) );

  //statistical analysis
  //  std::vector<double> optim_Cuts2_z_pt;
  //std::vector<double> optim_Cuts2_z_eta;
  std::vector<double> optim_Cuts2_jet_pt1; 
  std::vector<double> optim_Cuts2_jet_pt2; 
  //std::vector<double> optim_Cuts2_eta_gap;
  // std::vector<double> optim_Cuts2_dijet_mass;
  //  for(double z_pt=50;z_pt<=100;z_pt+=25)
  //  {
  //   for(int z_etaI=0;z_etaI<3;z_etaI++)
  //    {
  //      double z_eta;
  //       if(z_etaI==0)z_eta=0.5; 
  //       else  if(z_etaI==1)z_eta=1.4442;        
 //       else  if(z_etaI==2)z_eta=2.5;
        
  for(double jet_pt1=30;jet_pt1<=130;jet_pt1+=10)
    {
      for(double jet_pt2=30;jet_pt2<=jet_pt1;jet_pt2+=10)
	{
	  //for(double eta_gap=4.0;eta_gap<=7.0;eta_gap+=0.5)
	  //   {
	  //     for(double dijet_mass=500; dijet_mass<=1500; dijet_mass+=250)
	  //{
	  //           optim_Cuts2_z_pt.push_back(z_pt);
          //           optim_Cuts2_z_eta.push_back(z_eta);
	  optim_Cuts2_jet_pt1.push_back(jet_pt1);
	  optim_Cuts2_jet_pt2.push_back(jet_pt2);
          //           optim_Cuts2_eta_gap.push_back(eta_gap);
          //           optim_Cuts2_dijet_mass.push_back(dijet_mass);
          //         }
          //     }
          // }
	  //}
	} 
    }
  //  TH2F* Hoptim_cuts2  =(TH2F*)mon.addHistogram(new TProfile2D("optim_cut2",      ";cut index;variable",       optim_Cuts2_z_pt.size(),0,optim_Cuts2_z_pt.size(), 6, 0, 6)) ;
  //   Hoptim_cuts2->GetYaxis()->SetBinLabel(1, "zpt>");
  //   Hoptim_cuts2->GetYaxis()->SetBinLabel(2, "zeta<");
  //   Hoptim_cuts2->GetYaxis()->SetBinLabel(3, "jpt1>");
  //   Hoptim_cuts2->GetYaxis()->SetBinLabel(4, "jpt2>");
  //   Hoptim_cuts2->GetYaxis()->SetBinLabel(5, "deta>");
  //  Hoptim_cuts2->GetYaxis()->SetBinLabel(6, "mjj>");
  TH2F* Hoptim_cuts2  =(TH2F*)mon.addHistogram(new TProfile2D("optim_cut2",      ";cut index;variable",       optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(), 2, 0, 2)) ;
  Hoptim_cuts2->GetYaxis()->SetBinLabel(1, "jpt1>");
  Hoptim_cuts2->GetYaxis()->SetBinLabel(2, "jpt2>");
  
  //for(unsigned int index=0;index<optim_Cuts2_z_pt.size();index++){
  for(unsigned int index=0;index<optim_Cuts2_jet_pt1.size();index++){
    // Hoptim_cuts2->Fill(index,0.0,optim_Cuts2_z_pt[index]);   
    //     Hoptim_cuts2->Fill(index,1.0,optim_Cuts2_z_eta[index]);   
    //     Hoptim_cuts2->Fill(index,2.0,optim_Cuts2_jet_pt1[index]); 
    //     Hoptim_cuts2->Fill(index,3.0,optim_Cuts2_jet_pt2[index]); 
    //     Hoptim_cuts2->Fill(index,4.0,optim_Cuts2_eta_gap[index]);
    //     Hoptim_cuts2->Fill(index,5.0,optim_Cuts2_dijet_mass[index]);
    Hoptim_cuts2->Fill(index,0.0,optim_Cuts2_jet_pt1[index]); 
    Hoptim_cuts2->Fill(index,1.0,optim_Cuts2_jet_pt2[index]); 
  }
  mon.addHistogram( new TH2F ("dijet_deta_shapes",";cut index;|#Delta #eta|;Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),25,0,10) );
  mon.addHistogram( new TH2F ("zpt_shapes",";cut index;p_{T} [GeV];Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),50,0,1000) );
  mon.addHistogram( new TH2F ("met_shapes",";cut index;E_{T}^{miss} [GeV];Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),50,0,500) );  
  
  //open the file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  ZZ2l2nuSummaryHandler evSummaryHandler;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }
  float cnorm=1.0;
  if(isMC)
    {
      TH1F* cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      printf("cnorm = %f\n",cnorm);
    }
  Hcutflow->SetBinContent(1,cnorm);

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



  //duplicate checker
  DuplicatesChecker duplicatesChecker;
  int NumberOfDuplicated(0);

  //pileup reweighting
  bool disableJERSmear(false);
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; 
  for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  bool useObservedPU(true);
  if(isMC)
    {
      TString puDist("evAnalyzer/h2zz/pileuptrue");
      if(useObservedPU) puDist="evAnalyzer/h2zz/pileup";
      TH1F* histo = (TH1F *) file->Get(puDist);
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
      if(dataPileupDistribution.size()==0) 
	{ 
	  dataPileupDistribution=mcPileupDistribution; disableJERSmear=true; 
	  cout << "No PU reweighting or JER smearing will be applied" << endl;
	}
    }
  else mcPileupDistribution=dataPileupDistribution;
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())  dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
   
  //run the analysis
  int evStart(0),evEnd(evSummaryHandler.getEntries());
  for( int iev=0; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event)) {  NumberOfDuplicated++; continue; }      

      //interpret event
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool isGammaEvent    = gammaEvHandler.isGood(phys,use2011Id);
      if(!isGammaEvent) continue;
      if((mctruthmode==22||mctruthmode==111))
	{
	  if(isMC)
	    {
	      int npromptGammas = ((ev.mccat>>28)&0xf) ;      
	      //  if(mctruthmode==22  && npromptGammas<1) continue;
	      if(mctruthmode==111 && npromptGammas>0) continue;
	    }
	}

      //event weight
      float weight = 1.0;  
      if(isMC)  { weight = LumiWeights.weight( ev.ngenITpu ); }
      else      { weight = gammaEvHandler.triggerWeight(); }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight);
      Hcutflow->Fill(4,weight);
      Hcutflow->Fill(5,1);

      //check which event type is required to use (dilepton/or photon)
      std::vector<TString> dilCats;
      dilCats.push_back("ee");
      dilCats.push_back("mumu");      
      
      //build the gamma candidate
      float r9               = phys.gammas[0].r9; //*(isMC ? 1.005 : 1.0);
      bool hasElectronVeto   = phys.gammas[0].hasElectronVeto;
      LorentzVector gamma    = phys.gammas[0];
      float gIso             = (TMath::Max(phys.gammas[0].iso2+phys.gammas[0].iso3-pow(0.4,2)*TMath::Pi()*ev.rho,0.)+phys.gammas[0].iso1);
      bool passKinematics    (gamma.pt()>50 && fabs(gamma.eta())<1.4442 && r9<1.0 && r9>0.9 && !hasElectronVeto);      

      //
      // EXTRA LEPTONS
      //
      int nextraleptons(0);
      for(size_t ilep=0; ilep<phys.leptons.size(); ilep++)
	{
	  int lpid=phys.leptons[ilep].pid;
	  
	  //FIXME: looks like sometimes the gamma comes double counted as a soft-id electron ???
	  if(fabs(phys.leptons[ilep].id)==11)
	    {
	      double dr( deltaR(phys.leptons[ilep],gamma) );
	      if(dr<0.1) continue;
	    }

	  if(fabs(phys.leptons[ilep].id)==13)
	    {
	      if(use2011Id)
		{
		  if( (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10) )
		    nextraleptons++;
		}
	      else
		{
		  if(  (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2 && phys.leptons[ilep].pt()>10) )
		    nextraleptons++;
		}
	    }
	  if(fabs(phys.leptons[ilep].id)==11)
	    {
	      if(use2011Id)
		{
		  if( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	      else
		{
		  if( hasObjectId(ev.en_idbits[lpid],EID_LOOSE) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid])<0.15 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	    }
	}
      bool passMultiplicityVetoes (nextraleptons==0);


      //
      // JET/MET
      //
      LorentzVector metP4(phys.met[2]);
      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation( phys.ajets , phys.leptons, metP4, jets, zvvs, &jecUnc);
      if(disableJERSmear) jets[0]=phys.ajets;
      int nbtags(0),njets15(0),njets30(0);
      float mindrbj(99999.),minmbj(99999.);
      PhysicsObjectJetCollection selJets;
      for(size_t ijet=0; ijet<jets[0].size(); ijet++)
        {
	  LorentzVector ijetP4=jets[0][ijet];
	  if(ijetP4.pt()<15 || fabs(ijetP4.eta())>4.7) continue;
	  if(deltaR(ijetP4,gamma)<0.2) continue;
	  
	  bool isGoodJet = hasObjectId(jets[0][ijet].pid,JETID_LOOSE);
	  isGoodJet     &= hasObjectId(jets[0][ijet].pid,jetIdToApply);
	  if(!isGoodJet) continue;
	  selJets.push_back(jets[0][ijet]);	  
	  njets15++;
	  if(ijetP4.pt()>30) {
	    njets30++;
	    LorentzVector bj=ijetP4+gamma;
	    double drbj=deltaR(ijetP4,gamma);
	    if(bj.mass()<minmbj) minmbj=bj.mass();
	    if(drbj<mindrbj)     mindrbj=drbj;
	  }
	  if( ijetP4.pt()>30 && fabs(ijetP4.eta())<2.5) nbtags += (jets[0][ijet].btag2>0.679);
	}
      std::sort(selJets.begin(), selJets.end(), JetPtOrdering);

      bool passBveto              (nbtags==0);
      bool passJet                (njets30>=2);
      bool  passDetajj(false),passMjj(false);
      float hardpt(0), dphijj(0), maxPt(0), minPt(0), maxEta(0), minEta(0), detajj(0), etaprod(0), zepp(0), mjj(0),ptjj(0),spt(0);
      float jet1_beam_cm(0),jet2_beam_cm(0),z_beam_cm(0),jet1_z_cm(0),jet2_z_cm(0),jet1_zstar_cm(0),jet2_zstar_cm(0),zpt_cm(0),jjpt_cm(0);
      int ncjv(0), ncjv15(0),ncjv20(0), htcjv(0), htcjv15(0),htcjv20(0),maxcjpt(0),minmgtj(0);
      std::vector<float> eta3;
      if(passJet)
	{
	  LorentzVector jet1=selJets[0];
	  LorentzVector jet2=selJets[1];
	  LorentzVector vbfSyst=jet1+jet2;
	  mjj=vbfSyst.mass();
	  maxEta=max(jet1.eta(),jet2.eta());
	  minEta=min(jet1.eta(),jet2.eta());
	  detajj=maxEta-minEta;
	  etaprod=maxEta*minEta;
	  LorentzVector mgj1=jet1+gamma;
	  LorentzVector mgj2=jet2+gamma;
	  minmgtj = min(mgj1.mass(),mgj2.mass());
	  spt=vbfSyst.pt()/(jet1.pt()+jet2.pt());
	  
	  passDetajj=(fabs(detajj)>4.5);
	  passMjj=(mjj>450);
	  
	  LorentzVector hardSyst=vbfSyst+gamma; //+zvvs[0]
	  hardpt=hardSyst.pt();
	  dphijj=deltaPhi(jet1.phi(),jet2.phi());
	  maxPt=max(jet1.pt(),jet2.pt());
	  minPt=min(jet1.pt(),jet2.pt());
	  ptjj=vbfSyst.pt();
	  zepp=fabs(gamma.eta()-0.5*(jet1.eta()+jet2.eta()));

	  //visible system rest frame
	  BetaVector vbfBoost         = hardSyst.BoostToCM();
	  LorentzVector jet1_cm    = ROOT::Math::VectorUtil::boost( jet1, vbfBoost );
	  LorentzVector jet2_cm    = ROOT::Math::VectorUtil::boost( jet2, vbfBoost );
	  LorentzVector jj_cm      = ROOT::Math::VectorUtil::boost( vbfSyst, vbfBoost );
	  LorentzVector z_cm       = ROOT::Math::VectorUtil::boost( gamma,  vbfBoost );
	  jet1_beam_cm   = jet1_cm.theta();
	  jet2_beam_cm   = jet2_cm.theta();
	  z_beam_cm      = z_cm.theta();
	  jet1_z_cm      = getAngle(jet1_cm,z_cm);
	  jet2_z_cm      = getAngle(jet2_cm,z_cm);
	  jet1_zstar_cm  = getAngle(jet1_cm,gamma);
	  jet2_zstar_cm  = getAngle(jet2_cm,gamma);
	  zpt_cm         = z_cm.pt(); 
	  jjpt_cm          = jj_cm.pt();

	  maxcjpt=0.;
	  ncjv=0;
	  ncjv15=0;
	  ncjv20=0;
	  htcjv=0;
	  htcjv15=0;
	  htcjv20=0;
	  for(size_t iotherjet=2; iotherjet<selJets.size(); iotherjet++){
	    if(!hasObjectId(selJets[iotherjet].pid,jetIdToApply)) continue;
	    bool isInRapGap(selJets[iotherjet].eta()>minEta || selJets[iotherjet].eta()<maxEta);
	    if(isInRapGap)
	      {
		ncjv15++;
		htcjv15 +=selJets[iotherjet].pt();
		if(selJets[iotherjet].pt()>20)
		  {
		    ncjv20++;
		    htcjv20 +=selJets[iotherjet].pt();
		  }
		if(selJets[iotherjet].pt()>30)
		  {
		    ncjv++;
		    htcjv +=selJets[iotherjet].pt();
		  }
		if(maxcjpt<selJets[iotherjet].pt()) maxcjpt=selJets[iotherjet].pt();
	      }
	    if(selJets[iotherjet].pt()<30)continue;
	    eta3.push_back( selJets[iotherjet].eta() - 0.5*(jet1.eta()+jet2.eta()) );
	  }
	}
    
                    
      //  
      // EVENT SELECTION
      //
      //event category
      //       int eventSubCat    = eventCategoryInst.Get(phys,&selJets);
      //       TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat); 
      //       if(tag_subcat=="geq1jets" || tag_subcat=="vbf")
      // 	{
      // 	  if(njets30==1)      tag_subcat="eq1jets";
      // 	  else if(njets30==2) tag_subcat="eq2jets";
      // 	  else                tag_subcat="geq3jets";
      // 	}
      
     
      //now do the control plots
      if(!passKinematics) continue;
      if(!passMultiplicityVetoes) continue;

      //fill summary
      gammaEvHandler.evSummary_.cat=ev.cat;
      gammaEvHandler.evSummary_.qt=gamma.pt();
      gammaEvHandler.evSummary_.eta=gamma.eta();
      gammaEvHandler.evSummary_.en=gamma.energy();
      gammaEvHandler.evSummary_.njets=njets30;
      gammaEvHandler.evSummary_.weight=(isMC? weight*xsec/cnorm : 1.0);
      gammaEvHandler.evSummary_.nvtx=ev.nvtx;
      gammaEvHandler.evSummary_.mindrbj=mindrbj;
      gammaEvHandler.evSummary_.minmbj=minmbj;
      bpjetsT->Fill();

      dilCats = getDijetCategories(mjj,detajj,dilCats);
      TString dijetCat = getDijetCategory(mjj,detajj);
      if(dijetCat=="mjjq100") dijetCat="mjjq092";
      //now do the control plots
      std::map<TString, float> qtWeights;
      qtWeights = gammaEvHandler.getWeights(phys,dijetCat);

      for(size_t idc=0; idc<dilCats.size(); idc++)
	{

	  float iweight=weight;
	  TString dilCh("mumu");
	  if(dilCats[idc].Contains("ee")) dilCh="ee";
	  //LorentzVector iboson=gammaEvHandler.massiveGamma(dilCats[idc]));
	  //float zmass=iboson.mass();
	  //float knnWeight=1.0;
	  // if(tmvaReader) knnWeight=tmvaReader->EvaluateMVA("KNN_"+dilCats[idc]);
	  //iweight *= knnWeight;
	  iweight*=qtWeights[dilCh];

	  float mt=METUtils::transverseMass(gamma,zvvs[0],true);
	  TVector2 boson2(gamma.px(),gamma.py());
	  TVector2 met2(zvvs[0].px(),zvvs[0].py());
	  float metL=boson2*met2; metL /= -gamma.pt();    

	  std::vector<TString> tags;
	  tags.push_back(dilCats[idc]);
	  //tags.push_back(dilCats[idc]+tag_subcat);

	  mon.fillHisto("nvtx",tags, ev.nvtx,iweight);
	  mon.fillHisto("rho",tags, ev.rho,iweight);
	  mon.fillHisto("njets",tags, njets30,iweight);
	  
	  if(passJet)
	    {
	      //for purity estimates
	      mon.fillHisto("sihih",tags,phys.gammas[0].sihih*100 ,iweight,true);
	      mon.fillHisto("iso",tags, gIso,iweight,true);
	      mon.fillHisto("qtraw",tags, gamma.pt(),weight,true);
	      mon.fillHisto("qt",tags, gamma.pt(),iweight,true);
	      mon.fillHisto("rapidity",tags, fabs(gamma.Rapidity()),iweight,true);
	      mon.fillHisto("njetsvsavginstlumi", tags, njets30,ev.curAvgInstLumi,iweight);
	      mon.fillHisto("minmgtj", tags, minmgtj, weight);      
	      mon.fillHisto("minmgj", tags, minmbj, weight);      
	      mon.fillHisto("vbfcandjetpt",  tags, maxPt,iweight);
	      mon.fillHisto("vbfcandjetpt",  tags, minPt,iweight);
	      mon.fillHisto("vbfcandjet1pt", tags, maxPt,iweight);
	      mon.fillHisto("vbfcandjet2pt", tags, minPt,iweight);
	      mon.fillHisto("vbfcandjet1eta",     tags, max(fabs(maxEta),fabs(minEta)),iweight);
	      mon.fillHisto("vbfcandjet2eta",     tags, min(fabs(maxEta),fabs(minEta)),iweight);
	      mon.fillHisto("vbfcandjeteta",      tags, fabs(maxEta),iweight);
	      mon.fillHisto("vbfcandjeteta",      tags, fabs(minEta),iweight);
	      mon.fillHisto("vbfcandjetdeta",     tags, fabs(detajj),iweight);
	      mon.fillHisto("vbfcandjetetaprod",  tags, etaprod,iweight);
	      mon.fillHisto("vbfmjj",             tags, mjj,iweight,true);

	      if(isMC && mPDFInfo)
		{
		  std::vector<float> wgts=mPDFInfo->getWeights(iev);
		  for(size_t i=0; i<wgts.size(); i++) 
		    {
		      TString var("_"); var+=i;
		      mon.fillHisto("vbfcandjetdeta"+var,     tags, fabs(detajj),iweight*wgts[i]);
		      mon.fillHisto("vbfcandjet1pt"+var,      tags, maxPt,iweight*wgts[i]);
		      mon.fillHisto("vbfcandjet2pt"+var,      tags, minPt,iweight*wgts[i]);
		      mon.fillHisto("vbfcandjet1eta"+var,     tags, max(fabs(maxEta),fabs(minEta)),iweight*wgts[i]);
		      mon.fillHisto("vbfcandjet2eta"+var,     tags, min(fabs(maxEta),fabs(minEta)),iweight*wgts[i]);
		    }
		}


	      mon.fillProfile("mjjvsdetajjprof",  tags, detajj, mjj, iweight);
	      mon.fillHisto  ("mjjvsdetajj",      tags, detajj, mjj, iweight);
	      mon.fillProfile("detajjvsmjjprof",  tags, mjj,    detajj, iweight);
	      mon.fillHisto  ("detajjvsmjj",      tags, mjj,    detajj, iweight);

	      mon.fillProfile("dphijjvsdetajjprof",   tags, detajj, dphijj, iweight);
	      mon.fillHisto  ("dphijjvsdetajj",       tags, detajj, dphijj, iweight);
	      mon.fillProfile("ht15vsdetajjprof",     tags, detajj, htcjv15, iweight);
	      mon.fillHisto  ("ht15vsdetajj",         tags, detajj, htcjv15, iweight);
	      mon.fillProfile("ht20vsdetajjprof",     tags, detajj, htcjv20, iweight);
	      mon.fillHisto  ("ht20vsdetajj",         tags, detajj, htcjv20, iweight);
	      mon.fillProfile("htvsdetajjprof",       tags, detajj, htcjv, iweight);
	      mon.fillHisto  ("htvsdetajj",           tags, detajj, htcjv, iweight);
	      mon.fillProfile("cjv15vsdetajjprof",    tags, detajj, ncjv15, iweight);
	      mon.fillHisto  ("cjv15vsdetajj",        tags, detajj, ncjv15, iweight);
	      mon.fillProfile("cjv20vsdetajjprof",    tags, detajj, ncjv20, iweight);
	      mon.fillHisto  ("cjv20vsdetajj",        tags, detajj, ncjv20, iweight);
	      mon.fillProfile("cjvvsdetajjprof",      tags, detajj, ncjv, iweight);
	      mon.fillHisto  ("cjvvsdetajj",          tags, detajj, ncjv, iweight);
	      mon.fillProfile("leadcjvvsdetajjprof",  tags, detajj, maxcjpt, iweight);
	      mon.fillHisto  ("leadcjvvsdetajj",      tags, detajj, maxcjpt, iweight);
	      mon.fillProfile("leadjetvsdetajjprof",  tags, detajj, selJets[0].pt(), iweight);
	      mon.fillHisto  ("leadjetvsdetajj",      tags, detajj, selJets[0].pt(), iweight);
	      mon.fillProfile("trailerjetvsdetajjprof",  tags, detajj, selJets[1].pt(), iweight);
	      mon.fillHisto  ("trailerjetvsdetajj",      tags, detajj, selJets[1].pt(), iweight);
	      mon.fillHisto("vbfhardpt",           tags, hardpt,iweight);
	      mon.fillHisto("vbfspt",              tags, spt,iweight);
	      mon.fillHisto("vbfdphijj",           tags, fabs(dphijj),iweight);
	      mon.fillHisto("vbfzepp",             tags, zepp,iweight);
	      mon.fillHisto("vbfpt",               tags, ptjj,iweight);
	      mon.fillHisto("met",                 tags,zvvs[0].pt(),iweight);
	      mon.fillHisto("metL",                tags,metL,iweight);
	      mon.fillHisto("mt",                  tags,mt,iweight);
	      mon.fillHisto("zpt_cm",              tags, zpt_cm,iweight);
	      mon.fillHisto("jjpt_cm",             tags, jjpt_cm,iweight);
	      mon.fillHisto("jet_beam_cm",         tags, jet1_beam_cm,iweight);
	      mon.fillHisto("jet_beam_cm",         tags, jet2_beam_cm,iweight);
	      mon.fillHisto("jet_z_cm",            tags, jet1_z_cm,iweight);
	      mon.fillHisto("jet_z_cm",            tags, jet2_z_cm,iweight);
	      mon.fillHisto("jet_zstar_cm",        tags, jet1_zstar_cm,iweight);
	      mon.fillHisto("jet_zstar_cm",        tags, jet2_zstar_cm,iweight);
	      mon.fillHisto("z_beam_cm",           tags, z_beam_cm,iweight);
	      mon.fillHisto("jet_z_asym_cm",       tags, fabs(fabs(jet1_z_cm)-fabs(jet2_z_cm))/fabs(fabs(jet1_z_cm)+fabs(jet2_z_cm)),iweight);
	      mon.fillHisto("jet_beam_asym_cm",    tags, fabs(fabs(jet1_beam_cm)-fabs(jet2_beam_cm))/fabs(fabs(jet1_beam_cm)+fabs(jet1_beam_cm)),iweight);
	      mon.fillHisto("jet_zstar_asym_cm",   tags, fabs(fabs(jet1_zstar_cm)-fabs(jet2_zstar_cm))/fabs(fabs(jet1_zstar_cm)+fabs(jet2_zstar_cm)),iweight);
	      mon.fillHisto("vbfmaxcjvjpt",        tags, maxcjpt,iweight);
	      mon.fillHisto("vbfcjv",              tags, ncjv,iweight);
	      mon.fillHisto("vbfcjv15",            tags, ncjv15,iweight);
	      mon.fillHisto("vbfcjv20",            tags, ncjv20,iweight);
	      mon.fillHisto("vbfhtcjv",            tags, htcjv,iweight);
	      mon.fillHisto("vbfhtcjv15",          tags, htcjv15,iweight);
	      mon.fillHisto("vbfhtcjv20",          tags, htcjv20,iweight);
	    
	      //statistical analysis
	      std::vector<TString> localCtf;
	      localCtf.push_back(dilCats[idc]);
	      //localCtf.push_back(dilCats[idc]+"geq2jets");
	      LorentzVector vbfSyst=selJets[0]+selJets[1];
	      float detajj=fabs(selJets[0].eta()-selJets[1].eta());
	      for(unsigned int index=0; index<optim_Cuts2_jet_pt1.size();index++)
		{
		  //float minZPt=optim_Cuts2_z_pt[index];
		  //float maxZEta=optim_Cuts2_z_eta[index];
		  float minJetPt1=optim_Cuts2_jet_pt1[index];
		  float minJetPt2=optim_Cuts2_jet_pt2[index];
		  //float minEtaGap=optim_Cuts2_eta_gap[index];
		  //float minDijetMass=optim_Cuts2_dijet_mass[index];
		  
		  bool passLocalJet1Pt(selJets[0].pt()>minJetPt1);
		  bool passLocalJet2Pt(selJets[1].pt()>minJetPt2);
		  if(passLocalJet1Pt && passLocalJet2Pt){
		    mon.fillHisto("dijet_deta_shapes",localCtf,index,detajj,iweight);
		    mon.fillHisto("zpt_shapes",localCtf,index,gamma.pt(),iweight);
		    mon.fillHisto("met_shapes",localCtf,index,zvvs[0].pt(),iweight);
		  }		    
		}
	    
	    }//end passJet
	}//end for dilcats
    }//end event loop
  
  //all done with the events file
  file->Close();
  cout << "Sample treated as MC? " << isMC << endl;

  //save histograms to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += outFileUrl + ".root";
  printf("Results save in %s\n", outUrl.Data());

  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  bpjetsT->SetDirectory(ofile);
  bpjetsT->Write();
  ofile->Close();
}  


