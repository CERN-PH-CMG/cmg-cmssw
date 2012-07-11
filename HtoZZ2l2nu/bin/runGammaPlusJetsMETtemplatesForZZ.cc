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

  TRandom2 rndGen;
  EventCategory eventCategoryInst(0);
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor mon;

  mon.addHistogram(  new TProfile("metvsrun"    ,      ";Run number;<MET>",     500, 190000,200000) ) ;
  mon.addHistogram(  new TProfile("metvsavginstlumi",  ";Avg. inst lumi;<MET>", 50,  0,5000));
  mon.addHistogram(  new TProfile("nvtxvsrun",         ";Run number;<Vertices>",     500, 190000,200000) ) ;
  mon.addHistogram(  new TProfile("nvtxvsavginstlumi", ";Avg. inst lumi;<Vertices>", 50,  0,5000));

  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################
  std::vector<double> optim_Cuts1_met; 
  std::vector<double> optim_Cuts1_zpt;
  std::vector<double> optim_Cuts1_zmass;
  for(double met=50;met<100;met+=5.0){
    if(met>80 && int(met)%10!=0)continue;
    for(double pt=30;pt<100;pt+=5){
      if(pt>60 && int(pt)%10!=0)continue;
      for(double zm=5;zm<20;zm+=2.5){
	if(zm>10 && int(2*zm)%5!=0)continue;
	optim_Cuts1_met    .push_back(met);
	optim_Cuts1_zpt    .push_back(pt);
	optim_Cuts1_zmass  .push_back(zm);
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
  mon.addHistogram( new TH1F( "met_met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-100,400) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 15,76,106) );
  TH1 *hj=mon.addHistogram( new TH1F("njets",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  for(int ibin=1; ibin<=hj->GetXaxis()->GetNbins(); ibin++)
    {
      TString label("");
      if(ibin==hj->GetXaxis()->GetNbins()) label +="#geq";
      else                                label +="=";
      label += (ibin-1);
      hj->GetXaxis()->SetBinLabel(ibin,label);
    }
  
  //opten the file and get events tree
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
      
      //interpret event
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool hasEEtrigger = ev.triggerType & 0x1;
      bool hasMMtrigger = (ev.triggerType >> 1 ) & 0x1;

      bool isGammaEvent    = gammaEvHandler.isGood(phys);
      if(duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event)) {  NumberOfDuplicated++; continue; }

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
	  //check if dilepton is good
	  bool isGood(true);
	  for(size_t ilep=0; ilep<2; ilep++)
	    {
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
		  if( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho)<0.15 && phys.leptons[ilep].pt()>10) nextraleptons++;
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
      double mindphijmet(9999.),mindphijmet15(9999.);
      PhysicsObjectJetCollection selJets;
      LorentzVector clusteredMet(gamma);  clusteredMet *= -1;
      LorentzVector mht(0,0,0,0),unclusteredMet(0,0,0,0);
      PhysicsObjectJetCollection & jetsToUse=jets[0];
      if(disableJERSmear) jetsToUse=phys.ajets;
      for(size_t ijet=0; ijet<jetsToUse.size(); ijet++)
        {
	  bool isGoodJet    =hasObjectId(jetsToUse[ijet].pid,JETID_LOOSE);
	  if(!isGoodJet) continue;

	  LorentzVector ijetP4=jetsToUse[ijet];
	  if(isGammaEvent && deltaR(ijetP4,gamma)<0.2) continue;
	  
	  if(ijetP4.pt()<15) njets15++;
	  selJets.push_back(ijetP4);
	  clusteredMet -= ijetP4;
	  mht -= ijetP4;
	  ht += ijetP4.pt();

	  //b-tag
	  if(ijetP4.pt()>20 && fabs(ijetP4.eta())<2.5) nbtags += (jetsToUse[ijet].btag2>0.244);

	  //dphi(jet,MET)
	  double idphijmet=fabs(deltaPhi(metP4.phi(),ijetP4.phi()));	  
	  mindphijmet15=min(idphijmet,mindphijmet15);
  
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
      LorentzVector redMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMet, metP4,true,&redMetOut));
      double redMetL=redMetOut.redMET_l;
      double redMetT=redMetOut.redMET_t;
        
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
      bool passJetVeto            (njets30==0); 
      bool passRedMet(redMet.pt()>70);
      bool passBalance(metP4.pt()/gamma.pt() > 0.4 && metP4.pt()/gamma.pt() < 1.8);
      bool passDphijmet        (mindphijmet>0.5);
      if(njets30==0)  passDphijmet=(mindphijmet15>0.5);
      bool passPreSelection(passBveto && passJetVeto && passDphijmet);
      
      //event category
      int eventSubCat    = eventCategoryInst.Get(phys,&selJets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat); 
      std::vector<TString> subcats;
      subcats.push_back("");
      if(njets30==0)
	{
	  subcats.push_back("eq0jets");
	  if(njets15==0)         subcats.push_back("eq0softjets");
	  else                   subcats.push_back("eq0hardjets");
	  tag_subcat="eq0jets";
	} 
      
      //now do the control plots
      std::map<TString, float> qtWeights;
      if(isGammaEvent) qtWeights = gammaEvHandler.getWeights(phys,tag_subcat);
      
      //now do the control plots
      if(!passMultiplicityVetoes) continue;
      if(!passEB || !passR9 /*|| isConv*/) continue;
      if(!passKinematics) continue;
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  LorentzVector iboson(isGammaEvent ? gammaEvHandler.massiveGamma(dilCats[idc]) : gamma);
	  float zmass=iboson.mass();
	  Float_t mt( METUtils::transverseMass(iboson,metP4,true) );

	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
	  for(size_t isc=0; isc<subcats.size(); isc++)
	    {
	      TString ctf=dilCats[idc]+subcats[isc];	      
	      if(hasElectronVeto || !passR9tight) continue;
	      if(!passBveto) continue;

	      mon.fillHisto("eta",ctf, fabs(gamma.eta()),iweight);
	      mon.fillHisto("qt",ctf, gamma.pt(),iweight,true);
	      mon.fillHisto("nvtx",ctf, ev.nvtx,iweight);
	      mon.fillHisto("rho",ctf, ev.rho,iweight);
	      mon.fillHisto("zmass",ctf, zmass,iweight);
	      mon.fillHisto("njets",ctf, njets30,iweight);

	      if(!passJetVeto) continue;
	      mon.fillHisto("met_met",         ctf, metP4.pt(),iweight);
	      mon.fillHisto("met_redMet",      ctf, redMet.pt(),iweight);
	      mon.fillHisto("met_redMetT",     ctf, redMetL,iweight);
	      mon.fillHisto("met_redMetL",     ctf, redMetT,iweight);
	      mon.fillHisto("mt",              ctf, mt,iweight);

	      if(passRedMet)
		{
		  mon.fillHisto("balance",       ctf, metP4.pt()/gamma.pt(),iweight);
		  if(passBalance)
		    {
		      mon.fillHisto("mindphijmet",ctf,njets30==0 ? mindphijmet15:mindphijmet,weight);
		    }
		}
	      
	      if(!passPreSelection) continue;
	      for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
		float minMet=optim_Cuts1_met[index];
		float minZpt=optim_Cuts1_zpt[index];
		float deltaZ=optim_Cuts1_zmass[index];
		bool passLocalZmass(fabs(gamma.mass()-91)<deltaZ);
		bool passLocalMet(redMet.pt()>minMet);
		bool passLocalZpt(gamma.pt()>minZpt);
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


