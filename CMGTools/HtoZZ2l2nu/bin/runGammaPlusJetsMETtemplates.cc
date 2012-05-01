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
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  TRandom2 rndGen;
  //  EventCategory eventClassifComp;
  EventCategory eventClassifComp(3);
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor mon;

  std::vector<double> optim_Cuts1_met;
  std::vector<double> optim_Cuts1_mtmin;
  std::vector<double> optim_Cuts1_mtmax;
  for(double met=65;met<160;met+=5.0){
    if(met>90 && int(met)%10!=0)continue;
    if(met>120 && int(met)%20!=0)continue;
    for(double mtmin_=150;mtmin_<500;mtmin_+=25){
      double mtmin = mtmin_;
      if(mtmin<=150)mtmin=0;
      if(mtmin>350 && int(mtmin)%50!=0)continue;
      for(double mtmax=mtmin+100;mtmax<mtmin+350;mtmax+=25){
	if(mtmax>=mtmin+325)mtmax=3000;
	if(mtmin==0 && mtmax!=3000)continue;
	if(mtmin>350 && int(mtmax)%50!=0)continue;
	if(mtmax-mtmin>200 && int(mtmax)%50!=0)continue;
	optim_Cuts1_met    .push_back(met);
	optim_Cuts1_mtmin  .push_back(mtmin);
	optim_Cuts1_mtmax  .push_back(mtmax);
      }
    }
  }
  optim_Cuts1_met.push_back( 70); optim_Cuts1_mtmin.push_back(229); optim_Cuts1_mtmax.push_back(258);
  optim_Cuts1_met.push_back( 77); optim_Cuts1_mtmin.push_back(245); optim_Cuts1_mtmax.push_back(293);
  optim_Cuts1_met.push_back( 84); optim_Cuts1_mtmin.push_back(260); optim_Cuts1_mtmax.push_back(328);
  optim_Cuts1_met.push_back( 91); optim_Cuts1_mtmin.push_back(276); optim_Cuts1_mtmax.push_back(364);
  optim_Cuts1_met.push_back( 98); optim_Cuts1_mtmin.push_back(292); optim_Cuts1_mtmax.push_back(399);
  optim_Cuts1_met.push_back(105); optim_Cuts1_mtmin.push_back(308); optim_Cuts1_mtmax.push_back(434);
  optim_Cuts1_met.push_back(112); optim_Cuts1_mtmin.push_back(323); optim_Cuts1_mtmax.push_back(470);
  optim_Cuts1_met.push_back(119); optim_Cuts1_mtmin.push_back(339); optim_Cuts1_mtmax.push_back(505);
  optim_Cuts1_met.push_back(126); optim_Cuts1_mtmin.push_back(355); optim_Cuts1_mtmax.push_back(540);
  optim_Cuts1_met.push_back(133); optim_Cuts1_mtmin.push_back(370); optim_Cuts1_mtmax.push_back(576);
  optim_Cuts1_met.push_back(140); optim_Cuts1_mtmin.push_back(386); optim_Cuts1_mtmax.push_back(611);
  optim_Cuts1_met.push_back(147); optim_Cuts1_mtmin.push_back(402); optim_Cuts1_mtmax.push_back(646);
  optim_Cuts1_met.push_back(154); optim_Cuts1_mtmin.push_back(417); optim_Cuts1_mtmax.push_back(682);
  optim_Cuts1_met.push_back(161); optim_Cuts1_mtmin.push_back(433); optim_Cuts1_mtmax.push_back(717);
  optim_Cuts1_met.push_back(168); optim_Cuts1_mtmin.push_back(449); optim_Cuts1_mtmax.push_back(752);
  optim_Cuts1_met.push_back(0);   optim_Cuts1_mtmin.push_back(0); optim_Cuts1_mtmax.push_back(0);

  const size_t nOptimCuts=optim_Cuts1_met.size();
  TH1F* Hoptim_cuts1_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,nOptimCuts,0,nOptimCuts) ) ;
  TH1F* Hoptim_cuts1_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,nOptimCuts,0,nOptimCuts) ) ;
  TH1F* Hoptim_cuts1_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,nOptimCuts,0,nOptimCuts) ) ;
  for(unsigned int index=0;index<nOptimCuts;index++){
    Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
    Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
    Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
  }


  mon.addHistogram( new TH1F ("njets",     ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nsoftjets", ";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nbtags",    ";b-tag multiplicity; Events", 5,0,5) );
  for(size_t ibin=1; ibin<=5; ibin++){
    TString label("");
    if(ibin==5) label +="#geq";
    else        label +="=";
    label += (ibin-1);
    mon.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
    mon.getHisto("nbtags")->GetXaxis()->SetBinLabel(ibin,label);
  }
  
  TString subCats[]={"eq0softjets","eq0hardjets","eq0jets","eq1jets","eq2jets","geq3jets","vbf"};
  const size_t nSubCats=sizeof(subCats)/sizeof(TString);
  for(size_t isc=0; isc<nSubCats; isc++)
    {
      TString postfix(subCats[isc]);
      mon.addHistogram( new TH1F ("r9_"+postfix, ";R9;Events", 100,0.8,1) );
      mon.addHistogram( new TH1F ("sietaieta_"+postfix, ";#sigma i#eta i#eta;Events", 100,0,0.03) );
      mon.addHistogram( new TH1F( "mindphijmet_"+postfix, ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
      mon.addHistogram( new TH1F ("trkveto_"+postfix, ";Track veto;Events", 2,0,2) );
     
      mon.addHistogram( new TH1D( "qt_"+postfix,        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 100,0,500) );
      mon.addHistogram( new TH1D( "metoverqt_"+postfix, ";E_{T}^{miss}/p_{T}^{#gamma} [GeV/c];Events", 100,0,3) );
      mon.addHistogram( new TH1D( "eta_"+postfix,       ";#eta;Events", 50,0,2.6) );  
      mon.addHistogram( new TH1F( "nvtx_"+postfix, ";Vertices;Events", 30,0,30) );  
      mon.addHistogram( new TH1F( "mt_"+postfix  , ";M_{T};Events", 100,0,1000) );
      mon.addHistogram( new TH1F( "met_rawmet_"+postfix  , ";E_{T}^{miss} (raw);Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_met_"+postfix  , ";E_{T}^{miss};Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_rawRedMet_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) (raw);Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_min3Met_"+postfix  , ";min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_redMet_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_redMetL_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
      mon.addHistogram( new TH1F( "met_redMetT_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
      mon.addHistogram( new TH1F( "zmass_"+postfix, ";M^{ll};Events", 15,76,106) );
      mon.addHistogram( new TH2F( TString("met_met_") + postfix + "_vspu"       , ";Vertices;E_{T}^{miss};Events", 50,0,50,50,0,500) );
      mon.addHistogram( new TH2F( TString("met_min3Met_") + postfix + "_vspu"       , ";Vertices;min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );
      mon.addHistogram( new TH2F( TString("met_redMet_") + postfix + "_vspu"       , ";Vertices;red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );

      mon.addHistogram( new TH2F ("mt_shapes_"+postfix, ";cut index;M_{T} [GeV/c^{2}];",nOptimCuts,0,nOptimCuts, 32,150,950) );  
  }

  mon.addHistogram( new TH1F( "vbfmass", ";M(jet_{1},jet_{2});Events", 40,0,2000) );
  mon.addHistogram( new TH1F( "vbfdphi", ";#delta #phi(jet_{1},jet_{2});Events", 0,-3.14,3.14) );
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
  
  


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

  DuplicatesChecker duplicatesChecker;
  
  //pileup reweighting
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; 
  for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC)
    {
      TH1F* histo = (TH1F *) file->Get("evAnalyzer/h2zz/pileup");
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
    }
  else mcPileupDistribution=dataPileupDistribution;
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())  dataPileupDistribution.push_back(0.0);
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
   
  //run the analysis
  std::set<int> trigList;
  int evStart(0),evEnd(evSummaryHandler.getEntries());
  for( int iev=0; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      
      //interpret event
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool isGammaEvent    = gammaEvHandler.isGood(phys);

      if(duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event))
	{
	  printf("event %i-%i-%i is duplicated\n", ev.run, ev.lumi, ev.event);
	  //NumberOfDuplicated++;
	  continue;
	}

      //check which event type is required to use (dilepton/or photon)
      if(mctruthmode==22 && !isGammaEvent) continue;
      if(mctruthmode==1  && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;

      //event categories
      std::vector<TString> dilCats;
      LorentzVector gamma(0,0,0,0);
      int triggerThr(0);
      float r9(0),sietaieta(0);
      bool hasTrkVeto(false),isConv(false);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  dilCats.push_back("ll");
	  r9               = phys.gammas[0].r9*(isMC ? 1.005 : 1.0);
	  sietaieta        = phys.gammas[0].sihih;
	  hasTrkVeto       = phys.gammas[0].hasCtfTrkVeto;
	  gamma            = phys.gammas[0];
	  triggerThr       = gammaEvHandler.triggerThr();
	  isConv=(phys.gammas[0].isConv);
	}
      else
	{
	  gamma=phys.leptons[0]+phys.leptons[1];
	  if(fabs(gamma.mass()-91)>15) continue;
	  if(ev.cat==MUMU) dilCats.push_back("mumu");
	  if(ev.cat==EE)   dilCats.push_back("ee");
	  dilCats.push_back("ll");
	  triggerThr=gammaEvHandler.findTriggerCategoryFor( gamma.pt() );
	  if(triggerThr<0) continue;
	}
      
      //jet/MET
      LorentzVector metP4(phys.met[0]);
      int nbtags(0),njets30(0),njets15(0);
      double mindphijmet(9999.);
      PhysicsObjectJetCollection jetsP4;
      std::vector<double> genJetsPt;
      LorentzVector rawClusteredMet(gamma); 
      rawClusteredMet *= -1;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
        {
	  LorentzVector ijetP4=phys.jets[ijet];
	  if(ijetP4.pt()<15 || fabs(ijetP4.eta())>5) continue;
	  if(isGammaEvent && deltaR(ijetP4,gamma)<0.4) continue;
	  njets15++;
	  jetsP4.push_back(ijetP4);
	  genJetsPt.push_back(phys.jets[ijet].genPt);
	  rawClusteredMet -= ijetP4;
	  double idphijmet=fabs(deltaPhi(metP4.phi(),ijetP4.phi()));
	  mindphijmet=min(idphijmet,mindphijmet);
	  if(ijetP4.pt()>30)
	    {
	      if(fabs(ijetP4.eta())<2.5) nbtags += (phys.jets[ijet].btag1>2.0);
	      njets30++;
	    }
	}
      TString subcat("eq"); subcat+=njets30; subcat+= "jets";
      if(njets30>=3) subcat="geq3jets";
      int evType=eventClassifComp.Get(phys,&phys.jets);
      if(evType==EventCategory::VBF) subcat="vbf";


      //
      LorentzVector nullP4(0,0,0,0);
      LorentzVector rawZvv(phys.met[0]);
      LorentzVector assocMetP4        = phys.met[1];
      LorentzVector rawRedMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, rawClusteredMet, rawZvv,true));
      LorentzVectorCollection zvvs,redMets,min3Mets;
      std::vector<Float_t>  redMetLs,redMetTs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation(jetsP4, rawZvv, jets, zvvs, &jecUnc);
      for(size_t ivars=0; ivars<zvvs.size(); ivars++)
	{
          LorentzVector clusteredMetP4(gamma); clusteredMetP4 *= -1;
          for(size_t ijet=0; ijet<jets[ivars].size(); ijet++) clusteredMetP4 -= jets[ivars][ijet];
	  METUtils::stRedMET redMetOut;
	  redMets.push_back( METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMetP4, zvvs[ivars],true,&redMetOut) );
          redMetLs.push_back( redMetOut.redMET_l );
          redMetTs.push_back( redMetOut.redMET_t );
	  min3Mets.push_back( min(zvvs[ivars], min(assocMetP4,clusteredMetP4)) );
	}

      //event weight
      float weight = 1.0;
      if(isMC)                  { weight = LumiWeights.weight( ev.ngenITpu ); }
      //if(!isMC && isGammaEvent) { weight = gammaEvHandler.getTriggerPrescaleWeight(); }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight);
      Hcutflow->Fill(4,weight);
      Hcutflow->Fill(5,1);



      
      //select event
      bool passMultiplicityVetoes (isGammaEvent ? (phys.leptons.size()==0 && ev.ln==0 && phys.gammas.size()==1) : (ev.ln==0) );
      bool passKinematics         (gamma.pt()>55);
      bool passEB                 (!isGammaEvent || fabs(gamma.eta())<1.4442);
      bool passR9                 (!isGammaEvent || r9<1.0);
      bool passR9tight            (!isGammaEvent || r9>0.85); 
      bool passBveto              (nbtags==0);
      bool passMinDphiJmet        (mindphijmet>0.5);
      bool passSMZZ(njets30==0 && redMets[0].pt()>50 && zvvs[0].pt()/gamma.pt() > 0.4 && zvvs[0].pt()/gamma.pt() < 1.8 && passBveto && passMinDphiJmet);  

    
      //control plots
      std::map<TString, float> qtWeights;
      if(isGammaEvent) qtWeights = gammaEvHandler.getWeights(phys,subcat);
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  LorentzVector iboson(isGammaEvent ? gammaEvHandler.massiveGamma(dilCats[idc]) : gamma);
	  float zmass=iboson.mass();
	  //Float_t rawMt( METUtils::transverseMass(iboson,metP4,true) );
	  Float_t mt( METUtils::transverseMass(iboson,zvvs[0],true) );

	  TString ctf=dilCats[idc];
	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
	  if(!passMultiplicityVetoes) continue;
	  if(!passEB || !passR9 || isConv) continue;
	  if(!passKinematics) continue;

	  mon.fillHisto("eta_"+subcat,ctf, fabs(gamma.eta()),iweight);
	  mon.fillHisto("qt_"+subcat,ctf, gamma.pt(),iweight);
	  mon.fillHisto("nvtx_"+subcat,ctf, ev.nvtx,iweight);
	  mon.fillHisto("zmass_"+subcat,ctf, zmass,iweight);

	  mon.fillHisto("nbtags",ctf, nbtags,iweight);
	  if(!passBveto) continue;
	  
	  mon.fillHisto("njets",ctf, njets30,iweight);
	  mon.fillHisto("nsoftjets",ctf, njets15,iweight);

	  std::vector<TString> subCatsToFill;
	  subCatsToFill.push_back(subcat);
	  subCatsToFill.push_back("");
	  if(njets30==0) subCatsToFill.push_back(njets15==0 ? "eq0softjets" : "eq0hardjets");
	  
	  for(size_t isc=0; isc<subCatsToFill.size(); isc++)
	    {	 
	      if(isGammaEvent)  
		{
		  mon.fillHisto("r9_"+subCatsToFill[isc],ctf, r9,iweight);
		  mon.fillHisto("sietaieta_"+subCatsToFill[isc],ctf, sietaieta,iweight);
		  mon.fillHisto("trkveto_"+subCatsToFill[isc],ctf, hasTrkVeto,iweight);
		}

	      if(hasTrkVeto || !passR9tight) continue;
	      if(zvvs[0].pt()>70) mon.fillHisto("mindphijmet_"+subCatsToFill[isc],ctf, mindphijmet,iweight);	      
	      if(passMinDphiJmet) continue;
	      mon.fillHisto("met_rawmet_"+subCatsToFill[isc],ctf, rawZvv.pt(),iweight);
	      mon.fillHisto("metoverqt_"+subCatsToFill[isc],ctf, zvvs[0].pt()/gamma.pt(),iweight);
	      mon.fillHisto("met_met_"+subCatsToFill[isc],ctf, zvvs[0].pt(),iweight);
	      mon.fillHisto("met_met_"+subCatsToFill[isc]+"_vspu",ctf, ev.nvtx,zvvs[0].pt(),iweight);
	      mon.fillHisto("met_min3Met_"+subCatsToFill[isc],ctf, min3Mets[0].pt(),iweight);
	      mon.fillHisto("met_min3Met_"+subCatsToFill[isc]+"_vspu",ctf, ev.nvtx,min3Mets[0].pt(),iweight);
	      mon.fillHisto("met_rawRedMet_"+subCatsToFill[isc],ctf, rawRedMet.pt(),iweight);
	      mon.fillHisto("met_redMet_"+subCatsToFill[isc],ctf, redMets[0].pt(),iweight);
	      mon.fillHisto("met_redMet_"+subCatsToFill[isc]+"_vspu",ctf, ev.nvtx,redMets[0].pt(),iweight);
	      mon.fillHisto("met_redMetT_"+subCatsToFill[isc],ctf, redMetLs[0],iweight);
	      mon.fillHisto("met_redMetL_"+subCatsToFill[isc],ctf, redMetTs[0],iweight);
	      mon.fillHisto("mt_"+subCatsToFill[isc],ctf,mt,iweight);

	      if(passSMZZ)
		{
		  mon.fillHisto("metoverqt_"+subCatsToFill[isc],"ZZ"+ctf, zvvs[0].pt()/gamma.pt(),iweight);
		  mon.fillHisto("met_met_"+subCatsToFill[isc],"ZZ"+ctf, zvvs[0].pt(),iweight);
		  mon.fillHisto("met_redMet_"+subCatsToFill[isc],"ZZ"+ctf, redMets[0].pt(),iweight);
		  mon.fillHisto("met_redMetT_"+subCatsToFill[isc],"ZZ"+ctf, redMetLs[0],iweight);
		  mon.fillHisto("met_redMetL_"+subCatsToFill[isc],"ZZ"+ctf, redMetTs[0],iweight);
		}

	      if(subCatsToFill[isc]=="vbf")
		{
		  LorentzVector VBFSyst=jetsP4[0]+jetsP4[1];
		  mon.fillHisto("vbfmass",ctf,VBFSyst.mass(),iweight);
		  mon.fillHisto("vbdphi",ctf,deltaPhi(jetsP4[0].phi(),jetsP4[1].phi()));
		}

	      for(unsigned int index=0;index<nOptimCuts;index++){
		if(index==nOptimCuts-1 && passSMZZ)
		  mon.fill2DHisto("mt_shapes_"+subCatsToFill[isc],ctf,index, mt,iweight);
		else if ( index<nOptimCuts-1 && zvvs[0].pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
		  mon.fill2DHisto("mt_shapes_"+subCatsToFill[isc],ctf,index, mt,iweight);
	      }
	    }
	}
    }

  //all done with the events file
  file->Close();
  
  for(std::set<int>::iterator it = trigList.begin();
      it != trigList.end();
      it++)
    cout << *it << " ";
  cout << endl;
  cout << "Sample treated as MC? " << isMC << endl;

  //save histograms to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

}  


