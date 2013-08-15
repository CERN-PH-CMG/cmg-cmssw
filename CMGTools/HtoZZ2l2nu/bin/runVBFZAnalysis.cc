#include <iostream>
#include <boost/shared_ptr.hpp>

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/EfficiencyMap.h"
#include "CMGTools/HtoZZ2l2nu/interface/LeptonEfficiencySF.h"
#include "CMGTools/HtoZZ2l2nu/interface/PDFInfo.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

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
#include <Math/VectorUtil.h>

using namespace std;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >::BetaVector BetaVector;


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

  //   if(fabs(etajj)<1.2)                      etajjCat="etajjq016";
  //   if(fabs(etajj)>=1.2 && fabs(etajj)<2)    etajjCat="etajjq033";
  //   if(fabs(etajj)>=2   && fabs(etajj)<2.6)  etajjCat="etajjq049";
  //   if(fabs(etajj)>=2.6 && fabs(etajj)<3.2)  etajjCat="etajjq066";
  //   if(fabs(etajj)>=3.2)                     etajjCat="etajjq092";
  
  //include new tags
  std::vector<TString> selTags;
  for(size_t i=0; i<curTags.size(); i++)
    {
      TString itag=curTags[i];
      selTags.push_back(itag);
      selTags.push_back(itag+mjjCat);
      //      selTags.push_back(itag+etajjCat);
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

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");

  TString url=runProcess.getParameter<std::string>("input");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  int fType(0);
  if(url.Contains("DoubleEle")) fType=EE;
  if(url.Contains("DoubleMu"))  fType=MUMU;
  if(url.Contains("MuEG"))      fType=EMU;
  if(url.Contains("SingleMu"))  fType=MUMU;
  bool isSingleMuPD(!isMC && url.Contains("SingleMu"));  
  bool isV0JetsMC(isMC && url.Contains("0Jets"));  

  TString outTxtUrl= outUrl + "/" + outFileUrl + ".txt";
  FILE* outTxtFile = NULL;
  if(!isMC)outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());

  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //lepton efficiencies
  LeptonEfficiencySF lepEff2012;

  //jet energy scale uncertainties
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  //systematics
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  TString varNames[]={"",
		      "_jerup","_jerdown",
		      "_jesup","_jesdown",
		      "_lesup","_lesdown",
		      "_puup","_pudown"};
  size_t nvarsToInclude(1);
  if(runSystematics && isMC)
    {
      cout << "Systematics will be computed for this analysis" << endl;
      nvarsToInclude=sizeof(varNames)/sizeof(TString);
    }


  //##############################################
  //########    INITIATING HISTOGRAMS     ########
  //##############################################
  SmartSelectionMonitor mon;

  TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  TH1 *h=mon.addHistogram( new TH1F ("eventflow", ";;Events", 5,0,5) );
  h->GetXaxis()->SetBinLabel(1,"Trigger");
  h->GetXaxis()->SetBinLabel(2,"#geq 2 leptons");
  h->GetXaxis()->SetBinLabel(3,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(4,"p_{T}^{ll}>50, #eta^{ll}<1.44");
  //h->GetXaxis()->SetBinLabel(5,"3^{rd}-lepton veto"); 
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

//   Float_t qtaxis[100];
//   for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5                                                                                                                                   
//   for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195                                                                                                                                   
//   for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340                                                                                                                                    
//   for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976                                        
//   mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV];Events / (2.5 GeV)",99,qtaxis));
  mon.addHistogram( new TH1F( "qt",        ";p_{T}^{#gamma} [GeV];Events / (1 GeV)",1500,0,1500));
  mon.addHistogram( new TH1F( "et",        ";E_{T}^{#gamma} [GeV];Events / (1 GeV)",1500,0,1500));
  mon.addHistogram( new TH1F( "minmgtj",    ";min M(#gamma,tag jet) [GeV];Events / (1 GeV)",250,0,500));
  mon.addHistogram( new TH1F( "minmgj",    ";min M(#gamma,jet) [GeV];Events / (1 GeV)",250,0,500));
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
  int jetIdToApply=JETID_OPT_LOOSE;
  mon.addHistogram( new TH2F("njetsvsavginstlumi",  ";;Jet multiplicity (p_{T}>30 GeV);Events",5,0,5,10,0,5000) );
  mon.addHistogram( new TH1F("vbfcandjeteta"     , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjetpt"      , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjet1eta"    , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjet1pt"     , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjet2eta"    , ";#eta;Jets",                                 50,0,5) );
  mon.addHistogram( new TH1F("vbfcandjet2pt"     , ";p_{T} [GeV];Jets",                        50,0,500) );
  mon.addHistogram( new TH1F("vbfcandjetdeta"    , ";|#Delta #eta|;Jets",                50,0,10) );  
  mon.addHistogram( new TH1F("vbfcandjetetaprod" , ";#eta_{1} . #eta_{2};Jets",       100,-25,25) );

  mon.addHistogram( new TH1F("vbfhardpt"         , ";Hard p_{T} [GeV];Events",                   25,0,250) );
  mon.addHistogram( new TH1F("vbfspt"         , ";S_{p_{T}};Events",                   50,0,1) );
  mon.addHistogram( new TH1F("vbfdetalj"         , ";min #Delta #eta_{lj};Jets",               50,0,4) );
  h=mon.addHistogram( new TH1F("vbfcjv"          , ";Central jet count;Events",                     5,0,5) );
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"=2 jets");
  h->GetXaxis()->SetBinLabel(4,"=3 jets");
  h->GetXaxis()->SetBinLabel(5,"#geq 4 jets");
  mon.addHistogram( (TH1F *) h->Clone("vbfcjv15") );
  mon.addHistogram( (TH1F *) h->Clone("vbfcjv20") );
  mon.addHistogram( new TH1F("vbfmaxcjvjpt"        , ";Central jet gap [GeV];Events",50,0,100) );
  mon.addHistogram( new TH1F("vbfhtcjv15"          , ";H_{T}(p_{T}>15) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfhtcjv20"          , ";H_{T}(p_{T}>20) [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F("vbfhtcjv"            , ";H_{T}(p_{T}>30) [GeV];Events",50,0,250) );

  Double_t mjjaxis[32];
  mjjaxis[0]=0.01;
  for(size_t i=1; i<20; i++)  mjjaxis[i]   =50*i;        //0-1000
  for(size_t i=0; i<5; i++)   mjjaxis[20+i]=1000+100*i; //1000-1500
  for(size_t i=0; i<=5; i++)   mjjaxis[25+i]=1500+300*i; //1500-5000  
  mjjaxis[31]=5000;
  mon.addHistogram( new TH1F("vbfmjj"            , ";M_{jj} [GeV];Events",               31,mjjaxis) );
  mon.addHistogram( new TH1F("vbfdphijj"         , ";#Delta#phi_{jj};Events",            20,0,3.5) );
  mon.addHistogram( new TH1F("vbfzepp"           , ";#eta_{ll}-<#eta_{j}>;Events",       50,0,5) );
  mon.addHistogram( new TH1F("vbfpt"             , ";p_{T}(visible) [GeV];Dijets",       50,0,500) );
  mon.addHistogram( new TH1F("met"               , ";E_{T}^{miss} [GeV]; Events",        50,0,500) );
  mon.addHistogram( new TH1F("metL"              , ";Axial E_{T}^{miss} [GeV]; Events",  50,-50,200) );
  mon.addHistogram( new TH1F("mt"                , ";M_{T}        [GeV]; Events",        50,0,500) );
  mon.addHistogram( new TH1F("zpt_cm"            , ";p_{T} [GeV];Dileptons",             50,0,500) );
  mon.addHistogram( new TH1F("jjpt_cm"           , ";p_{T} [GeV];Dijets",                50,0,500) );
  mon.addHistogram( new TH1F("jet_beam_cm"       , ";#theta(j,beam line) [rad];Events",  20,0,3.5) );
  mon.addHistogram( new TH1F("z_beam_cm"         , ";#theta(Z,beam line) [rad];Events",  20,0,3.5) );
  mon.addHistogram( new TH1F("jet_z_cm"          , ";#theta(j,Z) [rad];Events",          20,0,3.5) );
  mon.addHistogram( new TH1F("jet_zstar_cm"      , ";#theta^{*}(j,Z) [rad];Events",      20,0,3.5) );
  mon.addHistogram( new TH1F("jet_zstar_asym_cm" , ";|#Delta|#theta| |/#Sigma |#theta| (j,Z); Events"        ,50,0,1) );
  mon.addHistogram( new TH1F("jet_z_asym_cm"     , ";|#Delta|#theta^{*}| |/#Sigma |#theta^{*}| (j,Z); Events",50,0,1) );
  mon.addHistogram( new TH1F("jet_beam_asym_cm" ,  ";|#Delta|#theta| |/#Sigma |#theta| (j,beam line); Events",50,0,1) );

  mon.addHistogram(  new TH2F    ("detajjvsmjj",         ";M_{jj} [GeV];|#Delta #eta_{jj}|",       10,0,3000,50,0.,10.) );
  mon.addHistogram(  new TH2F    ("dphijjvsmjj",         ";M_{jj} [GeV];|#Delta #phi_{jj}|",       10,0,3000,20,0,3.5) );
  mon.addHistogram(  new TH2F    ("ht15vsmjj",           ";M_{jj} [GeV];H_{T}(p_{T}>15) [GeV]",    10,0,3000,50,0,250) );
  mon.addHistogram(  new TH2F    ("ht20vsmjj",           ";M_{jj} [GeV];H_{T}(p_{T}>20) [GeV]",    10,0,3000,50,0,250) );
  mon.addHistogram(  new TH2F    ("htvsmjj",             ";M_{jj} [GeV];H_{T}(p_{T}>30) [GeV]",    10,0,3000,50,0,250) );
  mon.addHistogram(  new TH2F    ("cjv15vsmjj",          ";M_{jj} [GeV];Jets(p_{T}>15)",           10,0,3000,5,0,5) );
  mon.addHistogram(  new TH2F    ("cjv20vsmjj",          ";M_{jj} [GeV];Jets(p_{T}>20)",           10,0,3000,5,0,5) );
  mon.addHistogram(  new TH2F    ("cjvvsmjj",            ";M_{jj} [GeV];Jets(p_{T}>30)",           10,0,3000,5,0,5) );
  mon.addHistogram(  new TH2F    ("leadcjvvsmjj",        ";M_{jj} [GeV];Leding central jet [GeV]", 10,0,3000,50,0,100) );
  mon.addHistogram(  new TH2F    ("leadjetvsmjj",        ";M_{jj} [GeV];Leading jet [GeV]",        10,0,3000,50,0,500) );
  mon.addHistogram(  new TH2F    ("trailerjetvsmjj",      ";M_{jj} [GeV];Second jet [GeV]",        10,0,3000,50,0,500) );

  mon.addHistogram(  new TH2F    ("mjjvsdetajj",            ";|#Delta #eta_{jj}|;M_{jj} [GeV]",             50,0,10,10,0.,3000.) );
  mon.addHistogram(  new TH2F    ("dphijjvsdetajj",         ";|#Delta #eta_{jj}|;|#Delta #phi_{jj}|",       10,0,10,20,0,3.5) );
  mon.addHistogram(  new TH2F    ("ht15vsdetajj",           ";|#Delta #eta_{jj}|;H_{T}(p_{T}>15) [GeV]",    10,0,10,50,0,250) );
  mon.addHistogram(  new TH2F    ("ht20vsdetajj",           ";|#Delta #eta_{jj}|;H_{T}(p_{T}>20) [GeV]",    10,0,10,50,0,250) );
  mon.addHistogram(  new TH2F    ("htvsdetajj",             ";|#Delta #eta_{jj}|;H_{T}(p_{T}>30) [GeV]",    10,0,10,50,0,250) );
  mon.addHistogram(  new TH2F    ("cjv15vsdetajj",          ";|#Delta #eta_{jj}|;Jets(p_{T}>15)",           10,0,10,5,0,5) );
  mon.addHistogram(  new TH2F    ("cjv20vsdetajj",          ";|#Delta #eta_{jj}|;Jets(p_{T}>20)",           10,0,10,5,0,5) );
  mon.addHistogram(  new TH2F    ("cjvvsdetajj",            ";|#Delta #eta_{jj}|;Jets(p_{T}>30)",           10,0,10,5,0,5) );
  mon.addHistogram(  new TH2F    ("leadcjvvsdetajj",        ";|#Delta #eta_{jj}|;Leding central jet [GeV]", 10,0,10,50,0,100) );
  mon.addHistogram(  new TH2F    ("leadjetvsdetajj",        ";|#Delta #eta_{jj}|;Leading jet [GeV]",        10,0,10,50,0,500) );
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

  TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;
  for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
  {
    Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
    //    mon.addHistogram( new TH2F (TString("dijet_mass_shapes")+varNames[ivar],";cut index;M_Z [GeV];#events (/1GeV)",optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size(),120,0,3000) );
    mon.addHistogram( new TH2F (TString("dijet_deta_shapes")+varNames[ivar],";cut index;|#Delta #eta|;Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),25,0,10) );
    mon.addHistogram( new TH2F (TString("zpt_shapes")+varNames[ivar],";cut index;p_{T} [GeV];Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),50,0,1000) );
    mon.addHistogram( new TH2F (TString("met_shapes")+varNames[ivar],";cut index;E_{T}^{miss} [GeV];Events",optim_Cuts2_jet_pt1.size(),0,optim_Cuts2_jet_pt1.size(),50,0,500) );
  }
  
  //##############################################
  //######## GET READY FOR THE EVENT LOOP ########
  //##############################################

  //open the file and get events tree
  ZZ2l2nuSummaryHandler evSummaryHandler;
  TFile *file = TFile::Open(url);
  printf("Looping on %s\n",url.Data());
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ){
      file->Close();
      return -1;
  }


  //check run range to compute scale factor (if not all entries are used)
  const Int_t totalEntries= evSummaryHandler.getEntries();
  float rescaleFactor( evEnd>0 ?  float(totalEntries)/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=totalEntries;
  if(evStart > evEnd ){
      file->Close();
      return -1;
  }

  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC){
      TH1F* cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      if(rescaleFactor>0) cnorm /= rescaleFactor;
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

  //pileup weighting: based on vtx for now...
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  bool useObservedPU(true);
  bool disableJERSmear(false);
  if(isMC){
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
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights=0;
  PuShifter_t PuShifters;
  //  reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC){
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PuShifters=getPUshifters(dataPileupDistribution,0.05);
  }

  //event Categorizer
  EventCategory eventCategoryInst(0); //inclusive
  GammaEventHandler gammaEvHandler(runProcess);  
  TTree *bpjetsT = gammaEvHandler.initSummary();
  bpjetsT->SetDirectory(0);

  //##############################################
  //########           EVENT LOOP         ########
  //##############################################
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  DuplicatesChecker duplicatesChecker;
  int nDuplicates(0);
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      
      //event category
      bool sameFlavor(ev.cat==MUMU || ev.cat==EE);
      TString tag_cat;
      switch(ev.cat){
         case MUMU : tag_cat = "mumu";  break;
         case EE   : tag_cat = "ee";    break;
         case EMU  : tag_cat = "emu";   break;
         default   : continue;
      }
      //      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) && !isZZ2l2nu(ev.mccat) ) continue;
      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
      if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;
      if(isV0JetsMC && ev.mc_nup>5)                          continue; 

      //require compatibilitiy of the event with the PD
      bool hasTrigger(false);
      bool hasEEtrigger = ev.triggerType & 0x1;
      bool hasMMtrigger = (ev.triggerType >> 1 ) & 0x1;
      bool hasEMtrigger = (ev.triggerType >> 2 ) & 0x1;
      bool hasMtrigger  = (ev.triggerType >> 3 ) & 0x1;
      if(!isMC){
	  if(ev.cat!=fType) continue;

	  if(ev.cat==EE   && !hasEEtrigger) continue;
	  if(ev.cat==MUMU && !(hasMMtrigger||hasMtrigger) ) continue;
	  if(ev.cat==EMU  && !hasEMtrigger) continue;

	  //this is a safety veto for the single mu PD
	  if(isSingleMuPD) {
	    if(!hasMtrigger) continue;
	    if(hasMtrigger && hasMMtrigger) continue;
	  }

	  hasTrigger=true;
      }
      else 
	{
	  if(ev.cat==EE   && hasEEtrigger)                   hasTrigger=true;
	  if(ev.cat==MUMU && (hasMMtrigger || hasMtrigger) ) hasTrigger=true;
	  if(ev.cat==EMU  && hasEMtrigger)                   hasTrigger=true;
	}
      
      
      //prepare the tag's vectors for histo filling
      std::vector<TString> tags(1,"all");
      std::vector<TString> selTags;

      //pileup weight
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight            = LumiWeights->weight(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
        TotalWeight_plus  = PuShifters[PUUP]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
        TotalWeight_minus = PuShifters[PUDOWN]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
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
      LorentzVector lep1=phys.leptons[0];
      LorentzVector lep2=phys.leptons[1];
      LorentzVector zll(lep1+lep2);
      float zy(zll.Rapidity());
      if(isMC && mctruthmode==14442 && fabs(zy)>1.4442) continue;
      if(isMC && mctruthmode==50000 && fabs(zy)<1.4442) continue;
      bool passIdAndIso(true);
      bool passZmass(fabs(zll.mass()-91)<15);
      bool passZpt(zll.pt()>50);
      bool passZeta(fabs(zy)<1.4442);
      double llScaleFactor(1.0),llTriggerEfficiency(1.0);
      for(int ilep=0; ilep<2; ilep++)
	{
	  TString lepStr( fabs(phys.leptons[ilep].id)==13 ? "mu" : "e");
	  
	  //generator level matching
	  //int matchid(0);
	  LorentzVector genP4(0,0,0,0);
	  for(size_t igl=0;igl<phys.genleptons.size(); igl++) 
	    {
	      if(deltaR(phys.genleptons[igl],phys.leptons[ilep])>0.1) continue;
	      genP4=phys.genleptons[igl];
	      //  matchid=phys.genleptons[igl].id;
	    }
	  
	  //id and isolation
	  int lpid=phys.leptons[ilep].pid;
	  float relIso2011    = phys.leptons[ilep].relIsoRho(ev.rho);
	  float relIso = (lepStr=="mu") ? 
	    phys.leptons[ilep].pfRelIsoDbeta(): 
	    phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid]);
	  
	  bool hasGoodId(false), isIso(false);
	  if(fabs(phys.leptons[ilep].id)==13)
	    {
	      if(!use2011Id)
		{
		  hasGoodId=hasObjectId(ev.mn_idbits[lpid], MID_TIGHT);
		  isIso=(relIso<0.12);
		  // llScaleFactor *= lepEff2012.getLeptonEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),13).first;
		  //llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
		}
	      else
		{
		  hasGoodId=hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011);
		  isIso=relIso2011<0.15;
		  llScaleFactor *= muonScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
		  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
		}
	    }
	  else
	    {
	      if(!use2011Id)
		{
		  float sceta=ev.en_sceta[lpid];
		  hasGoodId = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM,
							  phys.leptons[ilep].isEBElectron(sceta),
							  phys.leptons[ilep].pt(), phys.leptons[ilep].eta(),
							  ev.en_detain[lpid],  ev.en_dphiin[lpid], ev.en_sihih[lpid], ev.en_hoe[lpid],
							  ev.en_ooemoop[lpid], phys.leptons[ilep].d0, phys.leptons[ilep].dZ,
							  0., 0., 0.,
							  !hasObjectId(ev.en_idbits[lpid], EID_CONVERSIONVETO),phys.leptons[ilep].trkLostInnerHits,ev.rho);
		  hasGoodId &= !phys.leptons[ilep].isInCrackElectron(sceta);
		  isIso=(relIso<0.15);
		  //llScaleFactor *= lepEff2012.getLeptonEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),11).first;
		  //llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
		}
	      else
		{
		  hasGoodId=hasObjectId(ev.en_idbits[lpid], EID_VBTF2011);
		  isIso=relIso2011<0.1;
		  llScaleFactor *= muonScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
		  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
		}
	    }
	  if(!hasGoodId)  passIdAndIso=false;
	  else if(!isIso) passIdAndIso=false;     
	  
	  if(hasGoodId)	    mon.fillHisto(lepStr+"reliso",     tags, relIso,   weight);
	}
      //if(isMC) weight *= llScaleFactor*llTriggerEfficiency;
      LorentzVector leadingLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[0]: phys.leptons[1]);
      LorentzVector trailerLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[1]: phys.leptons[0]);

      //
      // 3rd LEPTON ANALYSIS
      //
      int nextraleptons(0);
      std::vector<LorentzVector> extraLeptonsP4;
      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++){
	//lepton type
	bool isGood(false);
	int lpid=phys.leptons[ilep].pid;
	if(fabs(phys.leptons[ilep].id)==13){
	  if(!use2011Id){
	    isGood = (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2);
	  }else{
	    isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
	  }
	}else{
	  if(!use2011Id){
	    isGood = ( hasObjectId(ev.en_idbits[lpid],EID_LOOSE) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid])<0.15 && phys.leptons[ilep].pt()>10);
	  }else{
	    isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
	  }
	    }
	nextraleptons += isGood;
	
	if(!isGood) continue;
	extraLeptonsP4.push_back( phys.leptons[ilep] );
      }
      bool pass3dLeptonVeto(nextraleptons==0);
      
      //
      //JET/MET ANALYSIS
      //
      std::vector<PhysicsObjectJetCollection> variedAJets;
      LorentzVectorCollection zvvs;
      METUtils::computeVariation(phys.ajets, phys.leptons, phys.met[2], variedAJets, zvvs, &jecUnc);
      PhysicsObjectJetCollection aJets= variedAJets[0];
      if(disableJERSmear) aJets=phys.ajets;
      PhysicsObjectJetCollection aGoodIdJets;
      int nAJetsLoose(0), nAJetsLoose15(0);
      float mindrbj(99999.),minmbj(99999.);
      for(size_t ijet=0; ijet<aJets.size(); ijet++) 
	{
	  if(aJets[ijet].pt()<15 || fabs(aJets[ijet].eta())>4.7 ) continue;
	  
	  bool isGoodJet = hasObjectId(aJets[ijet].pid,JETID_LOOSE);
	  isGoodJet     &= hasObjectId(aJets[ijet].pid,jetIdToApply);
	  if(!isGoodJet) continue;
	  aGoodIdJets.push_back(aJets[ijet]);
	  if(aJets[ijet].pt()>15) nAJetsLoose15++;
	  if(aJets[ijet].pt()>30 )
	    {
	      nAJetsLoose++;
	      LorentzVector bj=aJets[ijet]+zll;
	      double drbj=deltaR(aJets[ijet],zll);
	      if(bj.mass()<minmbj) minmbj=bj.mass();
	      if(drbj<mindrbj)     mindrbj=drbj;
	    }
	}
      std::sort(aGoodIdJets.begin(), aGoodIdJets.end(), JetPtOrdering);

      float mt=METUtils::transverseMass(zll,zvvs[0],true);
      TVector2 boson2(zll.px(),zll.py());
      TVector2 met2(zvvs[0].px(),zvvs[0].py());
      float metL=boson2*met2; metL /= -zll.pt();    
      float detalj(0);
      float hardpt(0), dphijj(0), maxPt(0), minPt(0), maxEta(0), minEta(0), detajj(0), etaprod(0), zepp(0), mjj(0),ptjj(0),spt(0);
      float jet1_beam_cm(0),jet2_beam_cm(0),z_beam_cm(0),jet1_z_cm(0),jet2_z_cm(0),jet1_zstar_cm(0),jet2_zstar_cm(0),zpt_cm(0),jjpt_cm(0);
      int ncjv(0), ncjv15(0),ncjv20(0), htcjv(0), htcjv15(0),htcjv20(0),maxcjpt(0),minmgtj(0);
      std::vector<float> eta3;

      if(nAJetsLoose>=2)
	{
	  LorentzVector jet1=aGoodIdJets[0];
	  LorentzVector jet2=aGoodIdJets[1];
	  LorentzVector vbfSyst=jet1+jet2;
	  mjj=vbfSyst.mass();
	  maxEta=max(jet1.eta(),jet2.eta());
	  minEta=min(jet1.eta(),jet2.eta());
	  detajj=maxEta-minEta;
	  etaprod=maxEta*minEta;
	  LorentzVector mgj1=jet1+zll;
	  LorentzVector mgj2=jet2+zll;
	  minmgtj = min(mgj1.mass(),mgj2.mass());
	  spt=vbfSyst.pt()/(jet1.pt()+jet2.pt());
	  
	  float detal1j=TMath::Min( fabs(jet1.eta()-leadingLep.eta()), fabs(jet2.eta()-leadingLep.eta()) );
	  float detal2j=TMath::Min( fabs(jet1.eta()-trailerLep.eta()), fabs(jet2.eta()-trailerLep.eta()) );
	  detalj=TMath::Min(detal1j,detal2j);

	  LorentzVector hardSyst=vbfSyst+zll; //+zvvs[0]
	  hardpt=hardSyst.pt();
	  dphijj=deltaPhi(jet1.phi(),jet2.phi());
	  maxPt=max(jet1.pt(),jet2.pt());
	  minPt=min(jet1.pt(),jet2.pt());
	  ptjj=vbfSyst.pt();
	  zepp=fabs(zy-0.5*(jet1.eta()+jet2.eta()));
	  
	  //visible system rest frame
	  BetaVector vbfBoost         = hardSyst.BoostToCM();
	  LorentzVector jet1_cm    = ROOT::Math::VectorUtil::boost( jet1, vbfBoost );
	  LorentzVector jet2_cm    = ROOT::Math::VectorUtil::boost( jet2, vbfBoost );
	  LorentzVector jj_cm      = ROOT::Math::VectorUtil::boost( vbfSyst, vbfBoost );
	  LorentzVector z_cm       = ROOT::Math::VectorUtil::boost( zll,  vbfBoost );
	  jet1_beam_cm   = jet1_cm.theta();
	  jet2_beam_cm   = jet2_cm.theta();
	  z_beam_cm      = z_cm.theta();
	  jet1_z_cm      = getAngle(jet1_cm,z_cm);
	  jet2_z_cm      = getAngle(jet2_cm,z_cm);
	  jet1_zstar_cm  = getAngle(jet1_cm,zll);
	  jet2_zstar_cm  = getAngle(jet2_cm,zll);
	  zpt_cm         = z_cm.pt(); 
	  jjpt_cm          = jj_cm.pt();

	  maxcjpt=0.;
	  ncjv=0;
	  ncjv15=0;
	  ncjv20=0;
	  htcjv=0;
	  htcjv15=0;
	  htcjv20=0;
	  for(size_t iotherjet=2; iotherjet<aGoodIdJets.size(); iotherjet++){
	    if(!hasObjectId(aGoodIdJets[iotherjet].pid,jetIdToApply)) continue;
	    bool isInRapGap(aGoodIdJets[iotherjet].eta()>minEta || aGoodIdJets[iotherjet].eta()<maxEta);
	    if(isInRapGap)
	      {
		ncjv15++;
		htcjv15 +=aGoodIdJets[iotherjet].pt();
		if(aGoodIdJets[iotherjet].pt()>20)
		  {
		    ncjv20++;
		    htcjv20 +=aGoodIdJets[iotherjet].pt();
		  }
		if(aGoodIdJets[iotherjet].pt()>30)
		  {
		    ncjv++;
		    htcjv +=aGoodIdJets[iotherjet].pt();
		  }
		if(maxcjpt<aGoodIdJets[iotherjet].pt()) maxcjpt=aGoodIdJets[iotherjet].pt();
	      }
	    if(aGoodIdJets[iotherjet].pt()<30)continue;
	    eta3.push_back( aGoodIdJets[iotherjet].eta() - 0.5*(jet1.eta()+jet2.eta()) );
	  }
	}


      //event categories
      tags.push_back(tag_cat);
      if(sameFlavor) tags.push_back("ll");
      
      //CONTROL PLOTS
      if(hasTrigger) mon.fillHisto("eventflow",tags,0,weight);
      else           continue;
      
      if(passIdAndIso) mon.fillHisto("eventflow",tags,1,weight);
      else             continue;
      
      mon.fillHisto("zmass",    tags, zll.mass(), weight);  
      if(passZmass){

	//pu control
	mon.fillHisto("nvtx"     ,   tags, ev.nvtx,      weight);
	mon.fillHisto("nvtxraw"  ,   tags, ev.nvtx,      1);
	mon.fillHisto("rho"      ,   tags, ev.rho,       weight);
	mon.fillHisto("rho25"    ,   tags, ev.rho25Neut, weight);

	//basic Z and pu control
	mon.fillHisto("eventflow",   tags, 2,            weight);
	mon.fillHisto("zpt"      ,   tags, zll.pt(),     weight);      
	mon.fillHisto("zeta"     ,   tags, zll.eta(),    weight);
	mon.fillHisto("zy"     ,   tags, zy,    weight);
	mon.fillHisto("jeteventflow",tags,nAJetsLoose,weight);
	  
	if(passZpt && passZeta){
	  mon.fillHisto  ("eventflow",tags,3,weight);
	  
	  //analyze dilepton kinematics
	  mon.fillHisto("leadeta"     ,   tags, leadingLep.eta()   ,weight);
	  mon.fillHisto("leadpt"      ,   tags, leadingLep.pt()    ,weight);
	  mon.fillHisto("trailereta"  ,   tags, trailerLep.eta()   ,weight);
	  mon.fillHisto("trailerpt"   ,   tags, trailerLep.pt()    ,weight);

	  //analyze also 3 lepton category
	  mon.fillHisto("nleptons",tags,2+nextraleptons,weight);
	  if(nextraleptons==1) 
	    {
	      mon.fillHisto("thirdleptoneta",   tags,fabs(extraLeptonsP4[0].eta())   ,weight);
	      mon.fillHisto("thirdleptonpt" ,   tags,extraLeptonsP4[0].pt()     ,weight);
	      mon.fillHisto("mt3",tags,METUtils::transverseMass(extraLeptonsP4[0],zvvs[0],false),weight);
	    }
	  mon.fillHisto("njets",    tags, nAJetsLoose,weight);
	  
	  //fill summary
	  gammaEvHandler.evSummary_.cat=ev.cat;
	  gammaEvHandler.evSummary_.qt=zll.pt();
	  gammaEvHandler.evSummary_.eta=zy;
	  gammaEvHandler.evSummary_.en=zll.energy();
	  gammaEvHandler.evSummary_.njets=nAJetsLoose;
	  gammaEvHandler.evSummary_.weight=(isMC? weight*xsec/cnorm : 1.0);
	  gammaEvHandler.evSummary_.nvtx=ev.nvtx;
	  gammaEvHandler.evSummary_.mindrbj=mindrbj;
	  gammaEvHandler.evSummary_.minmbj=minmbj;
	  bpjetsT->Fill();

	  if(nAJetsLoose>=2)
	    {
	      mon.fillHisto("eventflow",tags,4,weight);
	      //if(mjj>100 && (spt>0.99 || spt<0.1))
	      //fprintf(outTxtFile,"%d:%d:%d %f\n",ev.run,ev.lumi,ev.event,spt);
		
	      selTags = getDijetCategories(mjj,detajj,tags);
	      mon.fillHisto("minmgtj", selTags, minmgtj, weight);      
	      mon.fillHisto("minmgj", selTags, minmbj, weight);      
	      mon.fillHisto("qt", selTags, zll.pt(), weight,true);      
	      mon.fillHisto("rapidity"     ,   tags, fabs(zy),    weight);
	      mon.fillHisto("et", selTags, zll.Et(), weight,true);      
	      mon.fillHisto("njetsvsavginstlumi", tags, nAJetsLoose,ev.curAvgInstLumi,weight);
	      mon.fillHisto("vbfcandjetpt",  selTags, maxPt,weight);
	      mon.fillHisto("vbfcandjetpt",  selTags, minPt,weight);
	      mon.fillHisto("vbfcandjet1pt", selTags, maxPt,weight);
	      mon.fillHisto("vbfcandjet2pt", selTags, minPt,weight);
	      mon.fillHisto("vbfcandjet1eta",     selTags, max(fabs(maxEta),fabs(minEta)),weight);
	      mon.fillHisto("vbfcandjet2eta",     selTags, min(fabs(maxEta),fabs(minEta)),weight);
	      mon.fillHisto("vbfcandjeteta",      selTags, fabs(maxEta),weight);
	      mon.fillHisto("vbfcandjeteta",      selTags, fabs(minEta),weight);
	      mon.fillHisto("vbfcandjetdeta",     selTags, detajj,weight);
	      mon.fillHisto("vbfcandjetetaprod",  selTags, etaprod,weight);
	      mon.fillHisto("vbfmjj",              selTags, mjj,weight,true);
	      mon.fillHisto("vbfhardpt",           selTags, hardpt,weight);
	      mon.fillHisto("vbfspt",           selTags, spt,weight);
	      mon.fillHisto("vbfdphijj",           selTags, fabs(dphijj),weight);
	      mon.fillHisto("vbfzepp",             selTags, zepp,weight);
	      mon.fillHisto("vbfpt",               selTags, ptjj,weight);
	      mon.fillHisto("met",                 selTags, zvvs[0].pt(),weight);
	      mon.fillHisto("metL",                selTags, metL,weight);
	      mon.fillHisto("mt",                  selTags, mt,weight);
	      mon.fillHisto("vbfdetalj",           selTags, detalj,weight);
	      mon.fillHisto("zpt_cm",              selTags, zpt_cm,weight);
	      //  mon.fillHisto("jjpt_cm",             selTags, jjpt_cm,weight);
	      //  mon.fillHisto("jet_beam_cm",         selTags, jet1_beam_cm,weight);
	      //  mon.fillHisto("jet_beam_cm",         selTags, jet2_beam_cm,weight);
	      //  mon.fillHisto("jet_z_cm",            selTags, jet1_z_cm,weight);
	      //  mon.fillHisto("jet_z_cm",            selTags, jet2_z_cm,weight);
	      //  mon.fillHisto("jet_zstar_cm",        selTags, jet1_zstar_cm,weight);
	      //  mon.fillHisto("jet_zstar_cm",        selTags, jet2_zstar_cm,weight);
	      //  mon.fillHisto("z_beam_cm",           selTags, z_beam_cm,weight);
	      //  mon.fillHisto("jet_z_asym_cm",       selTags, fabs(fabs(jet1_z_cm)-fabs(jet2_z_cm))/fabs(fabs(jet1_z_cm)+fabs(jet2_z_cm)),weight);
	      //  mon.fillHisto("jet_beam_asym_cm",    selTags, fabs(fabs(jet1_beam_cm)-fabs(jet2_beam_cm))/fabs(fabs(jet1_beam_cm)+fabs(jet1_beam_cm)),weight);
	      //  mon.fillHisto("jet_zstar_asym_cm",   selTags, fabs(fabs(jet1_zstar_cm)-fabs(jet2_zstar_cm))/fabs(fabs(jet1_zstar_cm)+fabs(jet2_zstar_cm)),weight);
	      mon.fillHisto("vbfmaxcjvjpt",        selTags, maxcjpt,weight);
	      mon.fillHisto("vbfcjv",              selTags, ncjv,weight);
	      mon.fillHisto("vbfcjv15",            selTags, ncjv15,weight);
	      mon.fillHisto("vbfcjv20",            selTags, ncjv20,weight);
	      mon.fillHisto("vbfhtcjv",            selTags, htcjv,weight);
	      mon.fillHisto("vbfhtcjv15",          selTags, htcjv15,weight);
	      mon.fillHisto("vbfhtcjv20",          selTags, htcjv20,weight);
	     
	      if(isMC && mPDFInfo)
		{
		  std::vector<float> wgts=mPDFInfo->getWeights(iev);
		  for(size_t i=0; i<wgts.size(); i++) 
		    {
		      TString var("_"); var+=i;
		      mon.fillHisto("vbfcandjetdeta"+var,     selTags, fabs(detajj),weight*wgts[i]);
		      mon.fillHisto("vbfcandjet1pt"+var,      tags, maxPt,weight*wgts[i]);
		      mon.fillHisto("vbfcandjet2pt"+var,      tags, minPt,weight*wgts[i]);
		      mon.fillHisto("vbfcandjet1eta"+var,     tags, max(fabs(maxEta),fabs(minEta)),weight*wgts[i]);
		      mon.fillHisto("vbfcandjet2eta"+var,     tags, min(fabs(maxEta),fabs(minEta)),weight*wgts[i]);
		    }
		}
	      	      
	    }//end nAJetsLoose
  
	  //STATISTICAL ANALYSIS
	  for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
	    float iweight = weight;                                               //nominal
	    if(ivar==7)                        iweight *=TotalWeight_plus;        //pu up
	    if(ivar==8)                        iweight *=TotalWeight_minus;       //pu down

	    int localNAJetsLoose(0);
	    PhysicsObjectJetCollection &varJets=variedAJets[ivar>4 ? 0  : ivar];
	    LorentzVector zvv    = zvvs[ivar>4 ? 0 : ivar];
	    PhysicsObjectJetCollection localSelJets;
	    for(size_t ijet=0; ijet<varJets.size(); ijet++){
	      if(varJets[ijet].pt()<30 || fabs(aJets[ijet].eta())>4.7) continue;
	      if(!hasObjectId(varJets[ijet].pid,JETID_LOOSE)) continue;
	      if(!hasObjectId(varJets[ijet].pid,jetIdToApply)) continue;
	      localSelJets.push_back( varJets[ijet] );
	      localNAJetsLoose++;
	    }
	    if(localNAJetsLoose<2) continue;
	    std::sort(localSelJets.begin(), localSelJets.end(), JetPtOrdering);

	    //re-assign the event category;
	    
	    std::vector<TString> locTags;
	    locTags.push_back(tag_cat);
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
		std::vector<TString> localSelTags=getDijetCategories(mjj,detajj,locTags);
		mon.fillHisto(TString("dijet_deta_shapes")+varNames[ivar],localSelTags,index,detajj,iweight);
		mon.fillHisto(TString("zpt_shapes")+varNames[ivar],localSelTags,index,zll.pt(),iweight);
		mon.fillHisto(TString("met_shapes")+varNames[ivar],localSelTags,index,zvv.pt(),iweight);
	      }
	  }
	}//end passZpt && passZeta
	
	
	//
	//N-1 CONTROL
	//
	if(           passZeta && (nAJetsLoose>=2))   { mon.fillHisto("zptNM1"      ,   selTags, zll.pt(),     weight); }
	if(passZpt             && (nAJetsLoose>=2))   { mon.fillHisto("zetaNM1"     ,   selTags, zll.eta(),    weight);  mon.fillHisto("zyNM1"     , selTags, zy,    weight);}
	if(passZpt && passZeta && (nAJetsLoose>=2))
	  {
	    mon.fillHisto  ("mjjvsdetajj",       selTags, detajj, mjj, weight);
	    mon.fillHisto  ("detajjvsmjj",       selTags, mjj,    detajj, weight);
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
  bpjetsT->SetDirectory(ofile);
  bpjetsT->Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





