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

  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  int fType(0);
  if(url.Contains("DoubleEle")) fType=EE;
  if(url.Contains("DoubleMu"))  fType=MUMU;
  if(url.Contains("MuEG"))      fType=EMU;
  if(url.Contains("SingleMu"))  fType=MUMU;
  bool isSingleMuPD(!isMC && url.Contains("SingleMu")); 

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
  std::vector<double> optim_Cuts1_jetthr;
  for(double met=50;met<100;met+=5.0){
    if(met>80 && int(met)%10!=0)continue;
    for(double pt=30;pt<100;pt+=5){
      if(pt>60 && int(pt)%10!=0)continue;
      for(double zm=5;zm<20;zm+=2.5){
	if(zm>10 && int(2*zm)%5!=0)continue;
	for(double jetpt=10; jetpt<20; jetpt+=5)
	  {
	    optim_Cuts1_met    .push_back(met);
	    optim_Cuts1_zpt    .push_back(pt);
	    optim_Cuts1_zmass  .push_back(zm);
	    optim_Cuts1_jetthr.push_back(jetpt);
	  }
      }
    }
  }
  
  TH1F* Hoptim_cuts1_met    =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_zpt    =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_zpt"  , ";cut index;zpt"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_zmass  =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_zm"  , ";cut index;zmass"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  TH1F* Hoptim_cuts1_jetthr =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_jetthr",  ";cut index;jet thr", optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
  for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
    Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
    Hoptim_cuts1_zpt  ->Fill(index, optim_Cuts1_zpt[index]);
    Hoptim_cuts1_zmass  ->Fill(index, optim_Cuts1_zmass[index]);
    Hoptim_cuts1_jetthr ->Fill(index, optim_Cuts1_jetthr[index]);
  }
  mon.addHistogram( new TH2F ("mt_shapes",";cut index;M_{T} [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  mon.addHistogram( new TH2F ("met_shapes",";cut index;met [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  mon.addHistogram( new TH2F ("zpt_shapes",";cut index;Z p_{T} [GeV/c^{2}];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
  TH2F *h=(TH2F *) mon.addHistogram( new TH2F ("nonresbckg_ctrl",";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
  h->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
  h->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
  h->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
  h->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
  h->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
  h->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");
  
  //VBF optimization
  std::vector<double> optim_Cuts2_jet1_pt;
  std::vector<double> optim_Cuts2_jet2_pt; 
  std::vector<double> optim_Cuts2_eta_gap;
  std::vector<double> optim_Cuts2_dijet_mass;
  for(double jet1_pt=30;jet1_pt<60;jet1_pt+=10)  
    {
      for(double jet2_pt=30;jet2_pt<=jet1_pt;jet2_pt+=10) 
	{
	  for(double eta_gap=3.5;eta_gap<=5.0;eta_gap+=0.5)
            {
              for(double dijet_mass=400; dijet_mass<=1000; dijet_mass+=50)
		{
		  optim_Cuts2_jet1_pt.push_back(jet1_pt);
		  optim_Cuts2_jet2_pt.push_back(jet2_pt);
		  optim_Cuts2_eta_gap.push_back(eta_gap);
		  optim_Cuts2_dijet_mass.push_back(dijet_mass);
		}
            }
	}
    } 
   TH1F* Hoptim_cuts2_jet1_pt=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_jet1_pt",";cut index;jet1_pt",optim_Cuts2_jet1_pt.size(),0,optim_Cuts2_jet1_pt.size())) ;
   TH1F* Hoptim_cuts2_jet2_pt=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_jet2_pt",";cut index;jet2_pt",optim_Cuts2_jet2_pt.size(),0,optim_Cuts2_jet2_pt.size())) ;
   TH1F* Hoptim_cuts2_eta_gap=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_eta_gap",";cut index;eta_gap",optim_Cuts2_eta_gap.size(),0,optim_Cuts2_eta_gap.size())) ;
   TH1F* Hoptim_cuts2_dijet_mass=(TH1F*)mon.addHistogram(new TH1F("optim_cut2_dijet_mass",";cut index;dijet_mass", optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size()));
   for(unsigned int index=0;index<optim_Cuts2_jet1_pt.size();index++){
     Hoptim_cuts2_jet1_pt->Fill(index,optim_Cuts2_jet1_pt[index]);   
     Hoptim_cuts2_jet2_pt->Fill(index,optim_Cuts2_jet2_pt[index]); 
     Hoptim_cuts2_eta_gap->Fill(index,optim_Cuts2_eta_gap[index]);
     Hoptim_cuts2_dijet_mass->Fill(index,optim_Cuts2_dijet_mass[index]);
   }
   //   mon.addHistogram( new TH2F ("dijet_mass_shapes",";cut index;M_{Jet1,Jet2} [GeV];#events (/50GeV)",optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size(),200,0.,1200) );
   mon.addHistogram( new TH2F ("dijet_mass_shapes",";cut index;M_Z [GeV];#events (/1GeV)",optim_Cuts2_dijet_mass.size(),0,optim_Cuts2_dijet_mass.size(),120,0,3000) );
  

  //##############################################
  //######## CONTROL PLOTS FOR SELECTION #########
  //##############################################
  Float_t qtaxis[100];
  for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5
  for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195
  for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340
  for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976
  mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
  mon.addHistogram( new TH1D( "balance", ";E_{T}^{miss}/q_{T};Events", 25,0,5) );
  mon.addHistogram( new TH1D( "eta",       ";#eta;Events", 50,0,2.6) );  
  mon.addHistogram( new TH1F( "nvtx", ";Vertices;Events", 50,0,50) );  
  mon.addHistogram( new TH1F( "rho", ";#rho;Events", 50,0,25) );  
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mt_final"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "met_met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_met_final"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet_final"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet15"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet20"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "met_redMetL_final"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-100,400) );
  mon.addHistogram(  new TProfile("metvsrho",    "#rho", 50,0,25));
  mon.addHistogram(  new TProfile("met15vsrho",  "#rho", 50,0,25));
  mon.addHistogram(  new TProfile("met20vsrho",  "#rho", 50,0,25));
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 15,76,106) );
  mon.addHistogram( new TH1F( "selsync", ";Synchronization step;Events", 4,0,4) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
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
  TString jetIds[]={"pf","pfpuloose","pfpumvaloose"};
  for(size_t i=0; i<sizeof(jetIds)/sizeof(TString); i++)
    {
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjeteta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjetdeta"       , ";|#Delta #eta|;Jets",50,0,10) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjetpt"       , ";p_{T} [GeV/c];Jets",50,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet1eta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet1pt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet2eta"       , ";#eta;Jets",50,0,5) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfcandjet2pt"       , ";p_{T} [GeV/c];Jets",50,0,500) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhardpt"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
      TH1 *hcjv=mon.addHistogram( new TH1F(jetIds[i]+"vbfcjv"       , ";Central jet count;Events",3,0,3) );
      hcjv->GetXaxis()->SetBinLabel(1,"=0 jets");
      hcjv->GetXaxis()->SetBinLabel(2,"=1 jets");
      hcjv->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
      mon.addHistogram( new TH1F(jetIds[i]+"vbfhtcjv"       , ";Central jet H_{T} [GeV/c];Events",50,0,250) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfpremjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfmjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",60,0,3000) );
      mon.addHistogram( new TH1F(jetIds[i]+"vbfdphijj"       , ";#Delta#phi;Events",20,0,3.5) );
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

      //interpret event
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool hasEEtrigger = ev.triggerType & 0x1;
      bool hasMMtrigger = (ev.triggerType >> 1 ) & 0x1;
      bool hasMtrigger  = (ev.triggerType >> 3 ) & 0x1;
      bool isGammaEvent    = gammaEvHandler.isGood(phys);

      //event weight
      float weight = 1.0;  
      if(isMC)                  { weight = LumiWeights.weight( ev.ngenITpu ); }
      //if(!isMC && isGammaEvent) { weight = gammaEvHandler.getTriggerPrescaleWeight(); }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight);
      Hcutflow->Fill(4,weight);
      Hcutflow->Fill(5,1);

      //check which event type is required to use (dilepton/or photon)
      if(mctruthmode==22 && !isGammaEvent ) continue;
      if(mctruthmode==1  && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;
      std::vector<TString> dilCats;
      
      //build the gamma candidate
      LorentzVector gamma(0,0,0,0);
      float r9(0),sietaieta(0);
      bool hasTrkVeto(false),isConv(false),hasElectronVeto(false),hasConvUnsafeElectronVeto(false);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  r9               = phys.gammas[0].r9*(isMC ? 1.005 : 1.0);
	  sietaieta        = phys.gammas[0].sihih;
	  hasElectronVeto  = phys.gammas[0].hasElectronVeto;
	  hasConvUnsafeElectronVeto = phys.gammas[0].hasConvUnsafeElectronVeto;
	  hasTrkVeto       = phys.gammas[0].hasCtfTrkVeto;
	  gamma            = phys.gammas[0];
	  isConv=(phys.gammas[0].isConv);
	}
      else
	{
	  if(!isMC){
	    if(ev.cat!=fType) continue;
	    if(ev.cat==EE   && !hasEEtrigger) continue;
	    if(ev.cat==MUMU && !(hasMMtrigger||hasMtrigger) ) continue;
	    //this is a safety veto for the single mu PD
	    if(isSingleMuPD) {
	      if(!hasMtrigger) continue;
	      if(hasMtrigger && hasMMtrigger) continue;
	    }
	  }

	  //check if dilepton is good
	  bool isGood(true);
	  float mindphilmet(99999.);
	  for(size_t ilep=0; ilep<2; ilep++)
	    {
	      float dphilmet=fabs(deltaPhi(phys.leptons[ilep].phi(),phys.met[0].phi()));
	      mindphilmet=min(dphilmet,mindphilmet);

	      gamma += phys.leptons[ilep];
	      int lpid=phys.leptons[ilep].pid;
	      if(fabs(phys.leptons[ilep].id)==13)
		{
		  if(use2011Id)
		    {
		      float relIso=phys.leptons[ilep].relIsoRho(ev.rho);
		      if( !hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) || relIso>0.15) isGood=false;
		    }
		  else
		    {
		      float relIso=phys.leptons[ilep].pfRelIsoDbeta();
		      if( !hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) || relIso>0.2) isGood=false;
		    }
		}
	      if(fabs(phys.leptons[ilep].id)==11)
		{
		  if(use2011Id)
		    {
		      float relIso=phys.leptons[ilep].relIsoRho(ev.rho);
		      if(!hasObjectId(ev.en_idbits[lpid], EID_VBTF2011) || relIso>0.1) isGood=false;
		    }
		  else
		    {
		      float relIso=phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho);
		      if(!hasObjectId(ev.en_idbits[lpid],EID_MEDIUM) || relIso>0.15) isGood=false;
		    }
		}
	    }
	  if(!isGood) continue;
	  if(fabs(gamma.mass()-91)>15) continue;
	  bool passLeptonMisReconstruction(use2011Id || phys.met[0].pt()<60 || (phys.met[0].pt()>60 && mindphilmet>0.2));
	  //if(!passLeptonMisReconstruction) continue;
	  if(ev.cat==MUMU) dilCats.push_back("mumu");
	  if(ev.cat==EE)   dilCats.push_back("ee");
	}

      //
      // EXTRA LEPTONS
      //
      int nextraleptons(0);
      for(size_t ilep=(isGammaEvent ? 0 : 2); ilep<phys.leptons.size(); ilep++)
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
		  if( hasObjectId(ev.en_idbits[lpid],EID_LOOSE) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho)<0.15 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	    }
	}
      
      //
      // JET/MET
      //
      LorentzVector metP4(phys.met[0]);
      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation(phys.ajets, phys.leptons, metP4, jets, zvvs, &jecUnc);
      metP4=zvvs[0];
      
      //count the jets
      int nbtags(0),njets30(0),njets15(0);
      double ht(0);
      double mindphijmet(9999.),mindphijmet20(9999.);
      PhysicsObjectJetCollection selJets;
      LorentzVector clusteredMet(gamma);  clusteredMet *= -1;
      LorentzVector clustered15Met(clusteredMet),clustered20Met(clusteredMet);
      LorentzVector mht(0,0,0,0),unclusteredMet(0,0,0,0);
      PhysicsObjectJetCollection & jetsToUse=jets[0];
      if(disableJERSmear) jetsToUse=phys.ajets;
      for(size_t ijet=0; ijet<jetsToUse.size(); ijet++)
        {
	  bool isGoodJet    =hasObjectId(jetsToUse[ijet].pid,JETID_LOOSE);
	  if(!isGoodJet) continue;

	  LorentzVector ijetP4=jetsToUse[ijet];
	  if(isGammaEvent && deltaR(ijetP4,gamma)<0.2) continue;

	  selJets.push_back(jetsToUse[ijet]);
	  clusteredMet -= ijetP4;
	  if(ijetP4.pt()>15) { clustered15Met -= ijetP4; njets15++; }
          if(ijetP4.pt()>20) { clustered20Met -= ijetP4;            }
	  mht -= ijetP4;
	  ht += ijetP4.pt();

	  //b-tag
	  if(ijetP4.pt()>20 && fabs(ijetP4.eta())<2.5) nbtags += (jetsToUse[ijet].btag2>0.244);

	  //dphi(jet,MET)
	  double idphijmet=fabs(deltaPhi(metP4.phi(),ijetP4.phi()));	  
	  if(ijetP4.pt()>20) mindphijmet20=min(idphijmet,mindphijmet20);
	  
	  if(ijetP4.pt()<30) continue;
	  njets30++;
	  mindphijmet=min(idphijmet,mindphijmet);
	}
      
      //other mets
      unclusteredMet = metP4-clusteredMet;
      LorentzVector nullP4(0,0,0,0);
      LorentzVector assocMetP4 = phys.met[1];
      LorentzVector min3Met=min(metP4, min(assocMetP4,clusteredMet)) ;
      METUtils::stRedMET redMetOut;
      LorentzVector redMet15=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clustered15Met, zvvs[0],true,&redMetOut);
      LorentzVector redMet20=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clustered20Met, zvvs[0],true,&redMetOut);
      LorentzVector redMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMet, metP4,true,&redMetOut));
      double redMetL=redMetOut.redMET_l;
      double redMetT=redMetOut.redMET_t;
      float balance=metP4.pt()/gamma.pt();
      if(njets30>0)
	{
	  LorentzVector recoil=metP4;
          for(size_t ijet=0; ijet<=selJets.size(); ijet++) recoil+=selJets[ijet];
          balance=recoil.pt()/gamma.pt();
	}
        
      //  
      // EVENT SELECTION
      //
      bool passMultiplicityVetoes (nextraleptons==0);
      bool passKinematics         (gamma.pt()>30);
      if(isGammaEvent && !isMC)    passKinematics &= (gamma.pt()>gammaEvHandler. triggerThr());
      bool passEB                 (!isGammaEvent || fabs(gamma.eta())<1.4442); 
      bool passR9                 (!isGammaEvent || r9<1.0);
      bool passR9tight            (true); //!isGammaEvent || r9>0.85); 
      bool passBveto              (nbtags==0);
      //       bool passJetVeto            (njets30==0); 
      bool passRedMet(redMet.pt()>70);
      bool passBalance(balance > 0.4 && balance < 1.8);
      bool passDphijmet        (mindphijmet>0.5);
      if(njets30==0)  passDphijmet=(mindphijmet20>0.5);
      bool passShapePreSelection(passBveto /*&& passJetVeto*/ && passDphijmet);
      
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
      if(!passEB || !passR9 /*|| isConv*/) continue;
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  TString ctf=dilCats[idc];
	  //sync with std selection
	  if(!isGammaEvent)
	    {
	      mon.fillHisto("selsync",ctf,0,weight);
	      if(passKinematics)
		{
		  mon.fillHisto("selsync",ctf,1,weight);
		  if(passMultiplicityVetoes)
		    {
		      mon.fillHisto("selsync",ctf,2,weight);
		      if(passBveto)  mon.fillHisto("selsync",ctf,3,weight);
		    }
		}
	    }
	  if(!passKinematics) continue;
	  if(!passMultiplicityVetoes) continue;
	  if(hasElectronVeto || !passR9tight) continue;
	  if(!passBveto) continue;
	  
	  //fill the histograms now
	  LorentzVector iboson(isGammaEvent ? gammaEvHandler.massiveGamma(dilCats[idc]) : gamma);
	  float zmass=iboson.mass();
	  Float_t mt( METUtils::transverseMass(iboson,metP4,true) );
	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
	  for(size_t isc=0; isc<subcats.size(); isc++)
	    {
	      TString ctf=dilCats[idc]+subcats[isc];	      
	      
	      mon.fillHisto("zpt"    ,ctf, gamma.pt(),  iweight);      //this is for synch
	      mon.fillHisto("eta",ctf, fabs(gamma.eta()),iweight);
	      mon.fillHisto("qt",ctf, gamma.pt(),iweight,true);
	      mon.fillHisto("nvtx",ctf, ev.nvtx,iweight);
	      mon.fillHisto("rho",ctf, ev.rho,iweight);
	      mon.fillHisto("zmass",ctf, zmass,iweight);
	      mon.fillHisto("njets",ctf, njets30,iweight);

	      //if(!passJetVeto) continue;
	      mon.fillHisto("met_met",         ctf, metP4.pt(),iweight);
	      mon.fillHisto("met_redMet",      ctf, redMet.pt(),iweight);
	      mon.fillHisto("met_redMet15",    ctf,redMet15.pt(),iweight);
	      mon.fillHisto("met_redMet20",    ctf,redMet20.pt(),iweight);
	      mon.fillProfile("metvsrho",  ctf, ev.rho, redMet.pt(), iweight);
	      mon.fillProfile("met15vsrho",  ctf, ev.rho, redMet15.pt(), iweight);
	      mon.fillProfile("met20vsrho",  ctf, ev.rho, redMet20.pt(), iweight);
	      mon.fillHisto("met_redMetT",     ctf, redMetL,iweight);
	      mon.fillHisto("met_redMetL",     ctf, redMetT,iweight);
	      mon.fillHisto("mt",              ctf, mt,iweight);

	      //VBF monitoring
	      if(njets30>=2)
		{
		  for(size_t ijetid=0; ijetid<sizeof(jetIds)/sizeof(TString); ijetid++)
		    {
		      int jetIdToApply(JETID_LOOSE);
		      if(jetIds[ijetid]=="pfpuloose")    jetIdToApply=JETID_CUTBASED_LOOSE;
		      if(jetIds[ijetid]=="pfpumvaloose") jetIdToApply=JETID_OPT_LOOSE;
		      if(!hasObjectId(selJets[0].pid,jetIdToApply) || !hasObjectId(selJets[0].pid,jetIdToApply)) continue; 
		      
		      LorentzVector vbfSyst=selJets[0]+selJets[1];
		      LorentzVector hardSyst=vbfSyst+gamma; //+zvvs[0]
		      float hardpt=hardSyst.pt();
		      float dphijj=deltaPhi(selJets[0].phi(),selJets[1].phi());
		      double maxEta=max(selJets[0].eta(),selJets[1].eta());
		      double minEta=min(selJets[0].eta(),selJets[1].eta());
		      float detajj=maxEta-minEta;
		      mon.fillHisto(jetIds[ijetid]+"vbfcandjetpt",       ctf, fabs(selJets[0].pt()),iweight);
		      mon.fillHisto(jetIds[ijetid]+"vbfcandjetpt",       ctf, fabs(selJets[1].pt()),iweight);
		      mon.fillHisto(jetIds[ijetid]+"vbfcandjet1pt", ctf, max(selJets[0].pt(),selJets[1].pt()),iweight);
		      mon.fillHisto(jetIds[ijetid]+"vbfcandjet2pt", ctf, min(selJets[0].pt(),selJets[1].pt()),iweight);
		      
		      int ncjv(0);
		      float htcjv(0);
		      if(selJets[0].pt()>30 && selJets[1].pt()>30) {
			for(size_t iotherjet=2; iotherjet<selJets.size(); iotherjet++){
			  if(selJets[iotherjet].pt()<30 || selJets[iotherjet].eta()<minEta || selJets[iotherjet].eta()>maxEta) continue;
			  if(!hasObjectId(selJets[iotherjet].pid,jetIdToApply)) continue;
			  htcjv+= selJets[iotherjet].pt();
			  ncjv++;
			}
			mon.fillHisto(jetIds[ijetid]+"vbfcjv",ctf,ncjv,iweight);
			mon.fillHisto(jetIds[ijetid]+"vbfhtcjv",ctf,htcjv,iweight);
			
			if(ncjv==0){
			  mon.fillHisto(jetIds[ijetid]+"vbfcandjet1eta",     ctf, max(fabs(maxEta),fabs(minEta)),iweight);
			  mon.fillHisto(jetIds[ijetid]+"vbfcandjet2eta",     ctf, min(fabs(maxEta),fabs(minEta)),iweight);
			  mon.fillHisto(jetIds[ijetid]+"vbfcandjeteta",      ctf, fabs(maxEta),iweight);
			  mon.fillHisto(jetIds[ijetid]+"vbfcandjeteta",      ctf, fabs(minEta),iweight);
			  mon.fillHisto(jetIds[ijetid]+"vbfcandjetdeta",     ctf, fabs(detajj),iweight);
			  mon.fillHisto(jetIds[ijetid]+"vbfpremjj",          ctf, vbfSyst.mass(),iweight);
			  if(fabs(detajj)>4.5){
			    mon.fillHisto(jetIds[ijetid]+"vbfmjj",             ctf, vbfSyst.mass(),iweight);
			    if(vbfSyst.mass()>450){
			      mon.fillHisto(jetIds[ijetid]+"vbfhardpt",     ctf, hardpt,iweight);
			      mon.fillHisto(jetIds[ijetid]+"vbfdphijj",     ctf, fabs(dphijj),iweight);
			    }
			  }
			}
		      }
		      
		      
		      //stat analysis
		      if(ijetid==0)
			{
			  for(unsigned int index=0; index<optim_Cuts2_jet1_pt.size();index++){
			    float minJet1Pt    = optim_Cuts2_jet1_pt[index];
			    float minJet2Pt    = optim_Cuts2_jet2_pt[index];
			    float minEtaGap    = optim_Cuts2_eta_gap[index];
			    float minDijetMass = optim_Cuts2_dijet_mass[index];
			    bool passLocalZmass(isGammaEvent || fabs(gamma.mass()-91)<15);
			    //bool passLocalRedMet(redMet.pt()>60);
			    bool passLocalJet1Pt(selJets[0].pt()>minJet1Pt);
			    bool passLocalJet2Pt(selJets[1].pt()>minJet2Pt);
			    bool passLocalEtaGap(fabs(selJets[0].eta()-selJets[1].eta())>minEtaGap);
			    float mjj((selJets[0]+selJets[1]).M());
			    bool passLocalDijetMass(mjj>minDijetMass);
			    //bool passLocalPreselection(passLocalZmass && passLocalRedMet && passLocalJet1Pt && passLocalJet2Pt && passLocalEtaGap && passLocalDijetMass);		    
			    //if(passLocalPreselection) mon.fillHisto("dijet_mass_shapes",ctf,index,(selJets[0]+selJets[1]).M(),iweight);
			    if(passLocalJet1Pt && passLocalJet2Pt && passLocalEtaGap && passLocalDijetMass && passLocalZmass && ncjv==0)  
			    {
			      mon.fillHisto("dijet_mass_shapes", ctf, index, mjj, iweight);
			      //			      if(index==1)
			      //				cout << " ev.run==" << ev.run << " && ev.lumi==" << ev.lumi << " ev.event=="<< ev.event << endl;
			      // 				cout << ctf << " " << njets30 << " " << gamma.pt() <<  " " 
			      // 				     << selJets[0].pt() << " " << selJets[1].pt() << " " << fabs(selJets[0].eta()-selJets[1].eta()) 
			      // 				     << " " << minEtaGap << " " << mjj << " " << ncjv << " " << iweight << endl;
			    }
			  }
			}
		    }
		}
	      //end VBF control
	      
	      if(passRedMet)
		{
		  mon.fillHisto("balance",       ctf, metP4.pt()/gamma.pt(),iweight);
		  if(passBalance)
		    {
		      mon.fillHisto("mindphijmet",ctf,njets30==0 ? mindphijmet20:mindphijmet,iweight);
		      if(passDphijmet)
			{
			  mon.fillHisto("mt_final",         ctf, metP4.pt(),iweight);
			  mon.fillHisto("met_met_final",         ctf, metP4.pt(),iweight);
			  mon.fillHisto("met_redMet_final",      ctf, redMet.pt(),iweight);
			  mon.fillHisto("met_redMetL_final",      ctf, redMetL,iweight);
			}
		    }
		}
	      
	      if(!passShapePreSelection) continue;
	      for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
		float minMet=optim_Cuts1_met[index];
		float minZpt=optim_Cuts1_zpt[index];
		float deltaZ=optim_Cuts1_zmass[index];
		float jetthr=optim_Cuts1_jetthr[index];
		bool passLocalZmass(fabs(zmass-91)<deltaZ);
		bool passLocalZpt(gamma.pt()>minZpt);
		bool passLocalMet(redMet.pt()>minMet);
		if(jetthr==15) passLocalMet=(redMet15.pt()>minMet);
		if(jetthr==20) passLocalMet=(redMet20.pt()>minMet);
		if(passLocalZmass && passLocalMet && passLocalZpt) 
		  { 
		    mon.fillHisto("met_shapes",ctf,index,redMet.pt(),iweight);
		    mon.fillHisto("mt_shapes",ctf,index,mt,iweight);
		    mon.fillHisto("zpt_shapes",ctf,index,gamma.pt(),iweight);
		  }
	      }
	    }
	}
}
  
  //all done with the events file
  file->Close();
  cout << "Sample treated as MC? " << isMC << endl;

  //save histograms to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();
}  


