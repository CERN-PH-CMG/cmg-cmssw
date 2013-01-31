#include <iostream>
#include <boost/shared_ptr.hpp>

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

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "Math/GenVector/Boost.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TRandom2.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

using namespace std;

LorentzVector min(const LorentzVector& a, const LorentzVector& b)
{
  if(a.pt()<=b.pt())return a;
  return b;
}

TString getJetRegion(float eta)
{
  TString reg("TK");
  if(fabs(eta)>2.5)  reg="HEin";
  if(fabs(eta)>2.75) reg="HEout";
  if(fabs(eta)>3)    reg="HF";
  return reg;
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
  bool useCHS(true);
  bool useJERsmearing(true); 
  bool useJetsOnlyInTracker(true); 
  bool usePUsubJetId(true); 
  // invert ajets <-> jets ...
  // 2011:  ajets -> non CHS,  jets -> CHS
  // 2012:  ajets -> CHS,      jets -> non CHS
  if(use2011Id) useCHS = !useCHS; 

  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");

  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  TString url=runProcess.getParameter<std::string>("input");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");

  bool isV0JetsMC(isMC && url.Contains("0Jets"));

  TRandom2 rndGen;
  EventCategory eventCategoryInst(4);  //0,>=1jet, VBF
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor mon;
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################
  std::vector<double> optim_Cuts1_met; 
  std::vector<double> optim_Cuts1_zpt;
  std::vector<double> optim_Cuts1_zmass;
  //std::vector<double> optim_Cuts1_jetthr;
  for(double met=50;met<140;met+=5.0){
    if(met>80 && int(met)%10!=0)continue;
    for(double pt=30;pt<100;pt+=5){
      if(pt>60 && int(pt)%10!=0)continue;
      for(double zm=5;zm<20;zm+=2.5){
	if(zm>10 && int(2*zm)%5!=0)continue;
	//for(double jetpt=10; jetpt<20; jetpt+=5) {
	optim_Cuts1_met    .push_back(met);
	optim_Cuts1_zpt    .push_back(pt);
	optim_Cuts1_zmass  .push_back(zm);
	//optim_Cuts1_jetthr.push_back(jetpt);
	//}
      }
    }
  }
  
  TH1F* Hoptim_cuts1_met    =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_zpt    =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_zpt"  , ";cut index;zpt"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_zmass  =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_zm"  , ";cut index;zmass"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
    Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
    Hoptim_cuts1_zpt  ->Fill(index, optim_Cuts1_zpt[index]);
    Hoptim_cuts1_zmass  ->Fill(index, optim_Cuts1_zmass[index]);
  }

  //double redMet_bins[] = {0., 20., 40., 60., 80., 100., 200., 400., 800.}; 
  double redMet_bins[] = {0., 20., 40., 60., 80., 100., 150., 400., 800.}; 
  double zpt_bins[] = {0., 20., 40., 60., 80., 100., 200., 400., 800.};
  const int n_redMet_bins = sizeof(redMet_bins)/sizeof(double) - 1; 
  mon.addHistogram( new TH1F( "met_redMet_rebin",       ";Reduced E_{T}^{miss};Events", n_redMet_bins,redMet_bins) );
  mon.addHistogram( new TH1F( "met_redMet_rebin_final", ";Reduced E_{T}^{miss};Events", n_redMet_bins,redMet_bins) );
 const int n_zpt_bins = sizeof(zpt_bins)/sizeof(double) - 1;
  mon.addHistogram( new TH2F ("mt_shapes",";cut index;M_{T} [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  mon.addHistogram( new TH2F ("met_shapes",";cut index;met [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  mon.addHistogram( new TH2F ("zpt_shapes",";cut index;Z p_{T} [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  mon.addHistogram( new TH2F ("redMet_shapes",";cut index;Red Met [GeV/c];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  mon.addHistogram( new TH2F ("zpt_rebin_shapes",";cut index;Z p_{T} [GeV/c];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), n_zpt_bins, zpt_bins) );
  mon.addHistogram( new TH2F ("redMet_rebin_shapes",";cut index;red-MET [GeV];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), n_redMet_bins,redMet_bins) );
  TH2F *h=(TH2F *) mon.addHistogram( new TH2F ("nonresbckg_ctrl",";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
  h->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
  h->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
  h->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
  h->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
  h->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
  h->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");
  

  //##############################################
  //######## CONTROL PLOTS FOR SELECTION #########
  //##############################################
  TH1F *h1=(TH1F*) mon.addHistogram( new TH1F ("eventflow_gamma", ";;Events", 4,0,4) );
  //h1->GetXaxis()->SetBinLabel(1,"Trigger");
  //h1->GetXaxis()->SetBinLabel(2,"#geq 2 leptons");
  //h1->GetXaxis()->SetBinLabel(3,"#geq 2 iso leptons");
  //h1->GetXaxis()->SetBinLabel(4,"|M-M_{Z}|<15");
  //h1->GetXaxis()->SetBinLabel(5,"p_{T}^{ll}>55");
  //h1->GetXaxis()->SetBinLabel(6,"3^{rd}-lepton veto");
  //h1->GetXaxis()->SetBinLabel(7,"Jet veto"); 
  h1->GetXaxis()->SetBinLabel(1,"b-veto"); 
  h1->GetXaxis()->SetBinLabel(2,"#Delta #phi(jet,E_{T}^{miss})>0.5");
  h1->GetXaxis()->SetBinLabel(3,"E_{T}^{miss}>65");
  h1->GetXaxis()->SetBinLabel(4,"0.4<E_{T}^{miss}/p_{T}^{ll}<1.8");

  Float_t qtaxis[100];
  for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5
  for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195
  for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340
  for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976
  mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
  mon.addHistogram( new TH1D( "balance", ";E_{T}^{miss}/q_{T};Events", 25,0,5) );
  mon.addHistogram( new TH1D( "balancedif",    ";|E_{T}^{miss}-q_{T}|/q_{T};Events", 5,0,1.0) );
  mon.addHistogram( new TH1D( "eta",       ";#eta;Events", 50,0,2.6) );  
  mon.addHistogram( new TH1F( "nvtx_dy", ";Vertices;Events", 50,0,50) );  
  mon.addHistogram( new TH1F( "rho", ";#rho;Events", 50,0,25) );  
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
  //mon.addHistogram( new TH1F( "mt_final"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "met_met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_metL",          ";Axial-E_{T}^{miss};Events", 50,-50,150) );
  mon.addHistogram( new TH1F( "met_metCompL",          ";Long. PF E_{T}^{miss};Events", 75,-250,500) );
  mon.addHistogram( new TH1F( "met_metCompT",          ";Trans. PF E_{T}^{miss};Events", 75,-250,500) );
  //mon.addHistogram( new TH1F( "met_met_final"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet_phi_dy"  , ";red(E_{T}^{miss} #phi;Events", 100,-6.,6.) );
  //mon.addHistogram( new TH1F( "met_redMet_final"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  //mon.addHistogram( new TH1F( "met_redMetL_final"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "met_redMetL"  , ";Long. Reduced E_{T}^{miss};Events", 75,-250,500) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";Trans. Reduced E_{T}^{miss};Events", 75,-250,500) );
  mon.addHistogram( new TH1F( "met_clusteredMetL",";Long. clustered E_{T}^{miss};Events", 75,-250,500) );
  mon.addHistogram( new TH1F( "met_clusteredMetT",";Trans. clustered E_{T}^{miss};Events", 75,-250,500) );
  mon.addHistogram(  new TProfile("metvsrho",    "#rho", 50,0,25));
  mon.addHistogram(  new TProfile("met15vsrho",  "#rho", 50,0,25));
  mon.addHistogram(  new TProfile("met20vsrho",  "#rho", 50,0,25));
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 15,76,106) );
  mon.addHistogram( new TH1F( "selsync", ";Synchronization step;Events", 4,0,4) );
  mon.addHistogram( new TH1F( "zpt_dy", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );

  // Final distributions
  mon.addHistogram( new TH1F( "mt_final"  , ";M_{T};Events", 25,0,1000) );
  mon.addHistogram( new TH1F( "zpt_final", ";p_{T}^{ll};Events", 25,0,500) );
  mon.addHistogram( new TH1F( "met_met_final"  , ";E_{T}^{miss};Events", 25,0,250) );
  mon.addHistogram( new TH1F( "met_redMet_final"  , ";Reduced E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL_final"  , ";Longitudinal Reduced E_{T}^{miss};Events", 25,-100,400) );
  mon.addHistogram( new TH1F("npfjets_dy",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );

 mon.addHistogram( new TH1F( "zpt_rebin", ";p_{T}^{ll};Events", n_zpt_bins, zpt_bins) );
 mon.addHistogram( new TH1F( "zpt_rebin_final", ";p_{T}^{ll};Events", n_zpt_bins, zpt_bins) );

  TH1 *hj=mon.addHistogram( new TH1F("njets",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  for(int ibin=1; ibin<=hj->GetXaxis()->GetNbins(); ibin++)
    {
      TString label("");
      if(ibin==hj->GetXaxis()->GetNbins()) label +="#geq";
      else                                label +="=";
      label += (ibin-1);
      hj->GetXaxis()->SetBinLabel(ibin,label);
    }

  //vbf control
  TString jetIds[]={"pf"};//,"pfpuloose","pfpumvaloose"};
  for(size_t i=0; i<sizeof(jetIds)/sizeof(TString); i++)
    {
      mon.addHistogram( new TH2F(jetIds[i]+"njetsvsavginstlumi",  ";;Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5,10,0,5000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjeteta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjetdeta"       , ";|#Delta #eta|;Jets",50,0,10) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjetpt"       , ";p_{T} [GeV/c];Jets",50,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet1eta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet1pt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet2eta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet2pt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhardpt"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhardpt50"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      TH1 *hcjv=mon.addHistogram( new TH1F(jetIds[i]+"vbfcjv"       , ";Central jet count;Events",3,0,3) );
      hcjv->GetXaxis()->SetBinLabel(1,"=0 jets");
      hcjv->GetXaxis()->SetBinLabel(2,"=1 jets");
      hcjv->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhtcjv"       , ";Central jet H_{T} [GeV/c];Events",50,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfpremjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfmjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfdphijj"       , ";#Delta#phi;Events",20,0,3.5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfmjj50"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfdphijj50"       , ";#Delta#phi;Events",20,0,3.5) );
    }

  
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

  //duplicate checker
  DuplicatesChecker duplicatesChecker;
  int NumberOfDuplicated(0);

  //pileup reweighting
  bool disableJERSmear(false);
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; 
  for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  bool useObservedPU(use2011Id);
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

      if((mctruthmode==22||mctruthmode==111))
	{
	  if(isMC)
	    {
	      int npromptGammas = ((ev.mccat>>28)&0xf) ;      
	      if(mctruthmode==22  && npromptGammas<1) continue;
	      if(mctruthmode==111 && npromptGammas>0) continue;
	    }
	}
      else continue;
      if(isV0JetsMC && ev.mc_nup>5)                          continue;


      //interpret event
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool isGammaEvent    = gammaEvHandler.isGood(phys,use2011Id);
      if(!isGammaEvent) continue;

      //event weight
      float weight = 1.0;  
      if(isMC)                  { weight = LumiWeights.weight( ev.ngenITpu ); }
      //if(!isMC && isGammaEvent) { weight = gammaEvHandler.triggerWeight(); }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight);
      Hcutflow->Fill(4,weight);
      Hcutflow->Fill(5,1);

      //check which event type is required to use (dilepton/or photon)
      std::vector<TString> dilCats;
      dilCats.push_back("ee");
      dilCats.push_back("mumu");      
      dilCats.push_back("ll");      
      
      //build the gamma candidate
      float r9               = phys.gammas[0].r9*(isMC ? 1.005 : 1.0);
      //float sietaieta        = phys.gammas[0].sihih;
      bool hasElectronVeto   = phys.gammas[0].hasElectronVeto;
      LorentzVector gamma    = phys.gammas[0];
      //bool isConv=(phys.gammas[0].isConv);

      //
      // EXTRA LEPTONS
      //
      int nextraleptons(0);
      for(size_t ilep=0; ilep<phys.leptons.size(); ilep++)
	{
	  int lpid=phys.leptons[ilep].pid;
	  
	  //FIXME: looks like sometimes the gamma comes double counted as a soft-id electron ???
	  if(isGammaEvent && fabs(phys.leptons[ilep].id)==11)
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
      

      //
      // JET/MET
      //
      LorentzVector metP4(phys.met[2]);
      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation( ( useCHS ? phys.ajets : phys.jets) , phys.leptons, metP4, jets, zvvs, &jecUnc);
      metP4=zvvs[0];
      if( !useJERsmearing ) metP4=phys.met[2];
      
      //count the jets
      int nbtags(0),njets30(0),njets15(0);
      double ht(0);
      double mindphijmet(9999.), mindphijmet15(9999.);
      PhysicsObjectJetCollection selJets;
      LorentzVector clusteredMet(gamma);  clusteredMet *= -1;
      LorentzVector mht(0,0,0,0),unclusteredMet(0,0,0,0);
      PhysicsObjectJetCollection & jetsToUse= (useJERsmearing ? jets[0] : ( useCHS ? phys.ajets : phys.jets) );
      if(disableJERSmear) jetsToUse=phys.ajets;
      for(size_t ijet=0; ijet<jetsToUse.size(); ijet++)
        {
	  LorentzVector ijetP4=jetsToUse[ijet];
	  if(ijetP4.pt()<15) continue;
	  if(deltaR(ijetP4,gamma)<0.2) continue;

	  double idphijmet( fabs(deltaPhi(jetsToUse[ijet].phi(),zvvs[0].phi()) ) );
	  if( jetsToUse[ijet].pt()>15 ) if(idphijmet<mindphijmet15)  mindphijmet15=idphijmet;
	  if( jetsToUse[ijet].pt()>30 ) if(idphijmet<mindphijmet) mindphijmet=idphijmet;

	  bool isGoodJet = hasObjectId(jetsToUse[ijet].pid,JETID_LOOSE); 
	  if(usePUsubJetId)  isGoodJet =hasObjectId(jetsToUse[ijet].pid,JETID_CUTBASED_LOOSE);
	  if(!isGoodJet) continue;

	  clusteredMet -= ijetP4;
	  mht -= ijetP4;
	  ht += ijetP4.pt();
	  
	  if(useJetsOnlyInTracker && fabs(ijetP4.eta())>2.5) continue;

	  njets15++;
	  if(ijetP4.pt()>30) njets30++;
	  selJets.push_back(jetsToUse[ijet]);

	  if( ijetP4.pt()>20 && fabs(ijetP4.eta())<2.5) nbtags += (jetsToUse[ijet].btag2>0.244);
	}
      
      //other mets
      unclusteredMet = metP4-clusteredMet;
      LorentzVector nullP4(0,0,0,0);
      //LorentzVector assocMetP4 = phys.met[1];
      //LorentzVector min3Met=min(metP4, min(assocMetP4,clusteredMet)) ;
      METUtils::stRedMET redMetOut;
      LorentzVector redMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMet, metP4,true, &redMetOut));
      double redMetL=redMetOut.redMET_l;
      double redMetT=redMetOut.redMET_t;
      TVector2 clusteredMet2(clusteredMet.px(),clusteredMet.py());
      double clusteredMetL=redMetOut.a_l*clusteredMet2;
      double clusteredMetT=redMetOut.a_t*clusteredMet2;
      float balance=metP4.pt()/gamma.pt();

      //ATLAS variables
      TVector2 dil2(gamma.px(),gamma.py());
      TVector2 met2(zvvs[0].px(),zvvs[0].py());
      double axialMet=dil2*met2; axialMet /= -gamma.pt();
      double balanceDif=fabs(zvvs[0].pt()-gamma.pt())/gamma.pt();
      double pfMetCompL = redMetOut.a_l*met2; 
      double pfMetCompT = redMetOut.a_t*met2; 
              
      //  
      // EVENT SELECTION
      //
      bool passMultiplicityVetoes (nextraleptons==0);
      bool passKinematics         (gamma.pt()> 45 && fabs(gamma.eta())<1.4442 && r9<1.0 && r9>0.9 && !hasElectronVeto);
      if(!isMC) passKinematics &= (gamma.pt()>gammaEvHandler. triggerThr());
      bool passBveto              (nbtags==0);
      bool passJetVeto            (njets30==0 && njets15>0); 
      bool passRedMet(redMet.pt()> 65 );
      bool passBalance(balance > 0.4 && balance < 1.8);
      bool passDphijmet=(mindphijmet>0.5);
      if(njets30==0)  passDphijmet=(mindphijmet15>0.5);
            
      //event category
      int eventSubCat    = eventCategoryInst.Get(phys,&selJets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat); 
      std::vector<TString> subcats;
      subcats.push_back("");
      if(tag_subcat=="eq0jets") {
	if(njets15==0)         subcats.push_back("eq0softjets");
	else                   subcats.push_back("eq0hardjets");
      } 
      else if(tag_subcat=="geq1jets")
	{
	  if(njets30==1) tag_subcat="eq1jets";
	  else if(njets30==2) tag_subcat="eq2jets";
	  else tag_subcat="geq3jets";
	}
      subcats.push_back(tag_subcat);
      
      //now do the control plots
      std::map<TString, float> qtWeights;
      if(isGammaEvent) qtWeights = gammaEvHandler.getWeights(phys,tag_subcat);
      
      //now do the control plots
      if(!passKinematics) continue;
      if(!passMultiplicityVetoes) continue;
      if(!passJetVeto) continue;
      if(!passBveto) continue;

      //fill the histograms now
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  LorentzVector iboson(gammaEvHandler.massiveGamma(dilCats[idc]));
	  float zmass=iboson.mass();
//cout<<"MASSA: "<<zmass<<endl;
	  Float_t mt( METUtils::transverseMass(iboson,metP4,true) );
	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
	  for(size_t isc=0; isc<subcats.size(); isc++)
	    {
	      TString ctf=dilCats[idc]+subcats[isc];	      
	  
	      mon.fillHisto("eventflow_gamma", ctf,1,iweight);
	      mon.fillHisto("zpt_dy",ctf, gamma.pt(),  iweight); 
	      mon.fillHisto("zpt_rebin",ctf, gamma.pt(),  iweight); 
	      mon.fillHisto("eta",ctf, fabs(gamma.eta()),iweight);
	      mon.fillHisto("qt",ctf, gamma.pt(),iweight,true);
	      mon.fillHisto("nvtx_dy",ctf, ev.nvtx,iweight);
	      mon.fillHisto("rho",ctf, ev.rho,iweight);
	      mon.fillHisto("zmass",ctf, zmass,iweight);
	      mon.fillHisto("njets",ctf, njets30,iweight);
	      mon.fillHisto("npfjets_dy",ctf, njets30,iweight);
	      mon.fillHisto("mindphijmet",ctf,njets30==0 ? mindphijmet15:mindphijmet,iweight);	  

	      if(passDphijmet)
		{
		  mon.fillHisto("eventflow_gamma", ctf,2,iweight);
		  mon.fillHisto("met_metL",    ctf,  axialMet,  iweight);
		  mon.fillHisto("met_met",         ctf, metP4.pt(),iweight);
		  mon.fillHisto("met_redMet",      ctf, redMet.pt(),iweight);
		  mon.fillHisto("met_redMet_rebin",ctf, redMet.pt(),iweight);
		  mon.fillHisto("met_redMet_phi_dy",      ctf, redMet.phi(),iweight);
		  mon.fillProfile("metvsrho",  ctf, ev.rho, redMet.pt(), iweight);
		  mon.fillHisto("met_redMetT",     ctf, redMetL,iweight);
		  mon.fillHisto("met_redMetL",     ctf, redMetT,iweight);
		  mon.fillHisto("met_clusteredMetL",     ctf, clusteredMetL,iweight);
		  mon.fillHisto("met_clusteredMetT",     ctf, clusteredMetT,iweight);
		  mon.fillHisto("mt",              ctf, mt,iweight);
		  mon.fillHisto("met_metCompL",     ctf, pfMetCompL,iweight);
		  mon.fillHisto("met_metCompT",     ctf, pfMetCompT,iweight);

		  mon.fillHisto("balance",       ctf, metP4.pt()/gamma.pt(),iweight);
		  mon.fillHisto("balancedif", ctf, balanceDif,iweight);	      
	      
		  if(passRedMet)
		    {
		      mon.fillHisto("eventflow_gamma", ctf,3,iweight);
		      if(passBalance) {
			mon.fillHisto("eventflow_gamma", ctf,4,iweight);
			mon.fillHisto("mt_final",         ctf, metP4.pt(),iweight);
			mon.fillHisto("zpt_final",        ctf, gamma.pt(),iweight);
			mon.fillHisto("zpt_rebin_final",    ctf, gamma.pt(),  iweight); 
			mon.fillHisto("met_met_final",         ctf, metP4.pt(),iweight);
			mon.fillHisto("met_redMet_final",      ctf, redMet.pt(),iweight);
			mon.fillHisto("met_redMet_rebin_final", ctf, redMet.pt(), iweight);
			mon.fillHisto("met_redMetL_final",      ctf, redMetL,iweight);
		      }
		    }//end passRedMet
	  
		  
		  //for shape analysis
		  if(!passBalance) continue;
		  for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
		    float minMet=optim_Cuts1_met[index];
		    float minZpt=optim_Cuts1_zpt[index];
		    float deltaZ=optim_Cuts1_zmass[index];
		    bool passLocalZmass(fabs(zmass-91)<deltaZ);
		    bool passLocalZpt(gamma.pt()>minZpt);
		    bool passLocalMet(redMet.pt()>minMet);
//cout<<"gamma.pt() before: "<<" "<<zmass<<" -91< "<<deltaZ<<" ||  "<<gamma.pt()<<" >  "<<minZpt<<"  ||  "<<redMet.pt()<<" > "<<minMet<<endl;
		    if(/*passLocalZmass &&*/ passLocalMet && passLocalZpt) 
		      { 
//cout<<"gamma.pt() AFTER : "<<gamma.pt()<<" "<<mt<<endl;
			mon.fillHisto("mt_shapes",ctf,index,mt,iweight);
			mon.fillHisto("zpt_shapes",ctf,index,gamma.pt(),iweight);
			mon.fillHisto("redMet_rebin_shapes",ctf,index, redMet.pt(),iweight);
			mon.fillHisto("zpt_rebin_shapes",ctf,index,gamma.pt(),iweight);
			mon.fillHisto("redMet_shapes",ctf,index,redMet.pt(),iweight);
		      }
		  }	
		}//passDphijmet
	    }//end for subcats
	}//end for event
    }
  
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
  ofile->Close();
}  


