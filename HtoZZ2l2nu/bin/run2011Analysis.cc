#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

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
#include "TEventList.h"
#include "TROOT.h"
 
using namespace std;



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
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);

  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");
  
  TString outTxtUrl= outUrl + "/" + gSystem->BaseName(url) + ".txt";
  FILE* outTxtFile = NULL;
  if(!isMC)outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());

  //handler for gamma processes
  GammaEventHandler *gammaEvHandler=0;
  if(mctruthmode==22){
     isMC=false;
     gammaEvHandler = new GammaEventHandler(runProcess);
  }

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

  //systematics
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  TString varNames[]={"","jesup","jesdown","jer","puup","pudown","renup","rendown","factup","factdown","btagup","btagdown"};
  size_t nvarsToInclude(1);
  if(runSystematics) 
    { 
      cout << "Systematics will be computed for this analysis" << endl;
      nvarsToInclude=sizeof(varNames)/sizeof(TString);
    }
  
  //isMC_VBF sample ?
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isMC_GG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isMC_VBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  std::vector<TGraph *> hWeightsGrVec;
  if(isMC_GG){
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
    TFile *fin=TFile::Open("~psilva/public/HtoZZ/HiggsQtWeights.root");
    if(fin){
	for(int ivar=0; ivar<5; ivar++){
	  double ren=HiggsMass; if(ivar==ZZ2l2nuSummary_t::hKfactor_renDown)  ren/=2; if(ivar==ZZ2l2nuSummary_t::hKfactor_renUp)  ren *= 2;
	  double fac=HiggsMass; if(ivar==ZZ2l2nuSummary_t::hKfactor_factDown) fac/=2; if(ivar==ZZ2l2nuSummary_t::hKfactor_factUp) fac *= 2;
	  char buf[100]; sprintf(buf,"kfactors_mh%3.0f_ren%3.0f_fac%3.0f",HiggsMass,ren,fac);
	  TGraph *gr= (TGraph *) fin->Get(buf);
	  if(gr) hWeightsGrVec.push_back((TGraph *)gr->Clone());
	}
	fin->Close();
	delete fin;
    }
  }else if(isMC_VBF){
    size_t VBFStringpos =  string(url.Data()).find("VBF");
    string StringMass = string(url.Data()).substr(VBFStringpos+6,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    VBFString = string(url.Data()).substr(VBFStringpos);
  }
  
  //##############################################
  //########    INITIATING HISTOGRAMS     ########
  //##############################################
  SmartSelectionMonitor mon;
  TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ) ;
  TH1F *h=(TH1F*) mon.addHistogram( new TH1F ("eventflow", ";Step;Events", 6,0,6) );
  h->GetXaxis()->SetBinLabel(1,"Preselected");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(3,"Z_{pT}>25");
  h->GetXaxis()->SetBinLabel(4,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(5,"b-veto");
  h->GetXaxis()->SetBinLabel(6,"#delta #phi(jet,E_{T}^{miss})");

  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 100,-5,5) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 100,0,400) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,91-31,91+31) );
  mon.addHistogram( new TH1F("nvtx",";Vertices;Events",25,0,25) ); 
  mon.addHistogram( new TH1F("njets"        ,";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nbtags", ";b-tag multiplicity; Events", 5,0,5) );  
  for(size_t ibin=1; ibin<=5; ibin++){
    TString label("");
    if(ibin==5) label +="#geq";
    else        label +="=";
    label += (ibin-1);
    mon.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
    mon.getHisto("nbtags")->GetXaxis()->SetBinLabel(ibin,label);
  }
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",100,0,3.4) );
  mon.addHistogram( new TH1F( "met"  , ";E_{T}^{miss};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );

  for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
    {
      TH1 *cacH = (TH1F *) mon.addHistogram( new TH1F (varNames[ivar]+"finaleventflow",";Category;Event count;",9,0,9) );
      cacH->GetXaxis()->SetBinLabel(1,"m=250");
      cacH->GetXaxis()->SetBinLabel(2,"m=300");
      cacH->GetXaxis()->SetBinLabel(3,"m=350");
      cacH->GetXaxis()->SetBinLabel(4,"m=400");
      cacH->GetXaxis()->SetBinLabel(5,"m=450");
      cacH->GetXaxis()->SetBinLabel(6,"m=500");
      cacH->GetXaxis()->SetBinLabel(7,"m=550");
      cacH->GetXaxis()->SetBinLabel(8,"m=600");
    
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt250",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt300",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt350",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt400",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt450",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt500",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt550",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
      mon.addHistogram( new TH1F (varNames[ivar]+"finalmt600",";M_{T} [GeV/c^{2}];Events;",100,0,1000) );
    } 

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> optim_Cuts1_met;
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=50;met<190;met+=2.0){
         for(double mtmin=220;mtmin<460;mtmin+=20){
            for(double mtmax=mtmin+50;mtmax<820;mtmax+=50){
               if(mtmax>=820)mtmax=3000;
               optim_Cuts1_met    .push_back(met);
               optim_Cuts1_mtmin  .push_back(mtmin);
               optim_Cuts1_mtmax  .push_back(mtmax);
            }
      }
   }

   mon.addHistogram ( new TH1F ("optim_eventflow1"  , ";cut index;yields" ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) );
   TH1F* Hoptim_cuts1_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
      Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
      Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
      Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
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


  //pileup weighting
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC){
      TH1F* histo = (TH1F *) file->Get("evAnalyzer/h2zz/pileup");
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
  reweight::PoissonMeanShifter PShiftUp(+0.6);
  reweight::PoissonMeanShifter PShiftDown(-0.6);


  //check PU normalized entries 
  //evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  //TEventList *elist = (TEventList*)gDirectory->Get("elist");
  //const Int_t normEntries = (elist==0 ? 0 : elist->GetN()); 
  //if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;
 

  //event Categorizer
  EventCategory eventCategoryInst(true);


  //##############################################
  //########           EVENT LOOP         ########
  //##############################################

 
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  for( int iev=evStart; iev<evEnd; iev++)  
    {
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
      
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      //LorentzVector zll  = isGammaEvent ? gammaEvHandler->massiveGamma("ll") : phys.leptons[0]+phys.leptons[1];
      LorentzVector zll  = phys.leptons[0]+phys.leptons[1];
      LorentzVectorCollection jetsP4;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) jetsP4.push_back( phys.jets[ijet] );

      //categorize events
      TString tag_cat;
      switch(ev.cat)
	{
	case EMU : tag_cat = "emu";   break;
	case MUMU: tag_cat = "mumu";  break;
	case EE  : tag_cat = "ee";    break;
	default  : continue;
	}
      if(isMC && mctruthmode==1 && ev.mccat!=DY_EE && ev.mccat!=DY_MUMU)  continue;
      if(isMC && mctruthmode==2 && ev.mccat!=DY_TAUTAU) continue;

      bool isGammaEvent = false;
      if(gammaEvHandler)
	{
          isGammaEvent=gammaEvHandler->isGood(phys);
          if(mctruthmode==22 && !isGammaEvent) continue;
          tag_cat = "gamma";
	}
     
      int eventSubCat  = eventCategoryInst.Get(phys);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);

      //prepare the tag's vectors for histo filling
      std::vector<TString> tags_all;
      tags_all.push_back("all");

      std::vector<TString> tags_cat;
      tags_cat.push_back("all");
      tags_cat.push_back(tag_cat);

      std::vector<TString> tags_full;
      tags_full.push_back("all");
      tags_full.push_back(tag_cat);
      tags_full.push_back(tag_cat + tag_subcat);

      //pileup and Higgs pT weight
      //float weight=ev.puWeight;
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight = LumiWeights.weight( ev.ngenITpu );
        TotalWeight_plus = PShiftUp.ShiftWeight( ev.ngenITpu );
        TotalWeight_minus = PShiftDown.ShiftWeight( ev.ngenITpu );
        if(isMC_VBF) weight *= weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );         
        if(isMC_GG)  {
	  for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
	  weight *= ev.hptWeights[0];
	}
      }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);
      
      //prepare variations 
      LorentzVectorCollection zvvs;
      std::vector<LorentzVectorCollection> jets;
      if(runSystematics) jet::computeVariation(jetsP4,phys.met[0], jets, zvvs, &stdPtResol,&stdEtaResol,&stdPhiResol,&jecUnc);
      zvvs.insert(zvvs.begin(),phys.met[0]);
      jets.insert(jets.begin(),jetsP4);
      std::vector<Float_t>  mts;
      for(size_t ivar=0; ivar<runSystematics?3:1; ivar++)
	{
	  Float_t imt     = METUtils::transverseMass(zll,zvvs[ivar],true);
	  mts.push_back(imt);
	}
      
      //run the variations
      for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
	{
	  float iweight = weight;                                  //nominal
	  if(ivar==4)            iweight *=TotalWeight_plus;        //pu up
	  if(ivar==5)            iweight *=TotalWeight_minus;       //pu down
	  if(ivar>=6 && isMC_GG) iweight *=ev.hptWeights[ivar-5];   //ren/fact scales

	  Float_t zmass=zll.mass();
	  Float_t zpt=zll.pt();
	  Float_t zeta=zll.eta();
	  LorentzVectorCollection &origJetsP4=jets[ivar>2?0:ivar];	    
	  LorentzVector zvv  = zvvs[ivar>2?0:ivar];
	  Float_t mt         = mts[ivar>2?0:ivar];
	  int njets(0),nbtags(0);
	  Float_t mindphijmet(9999.);
	  Float_t btagcut(2.0); if(ivar==10) btagcut=2.1; else btagcut=1.9;
	  for(size_t ijet=0; ijet<origJetsP4.size(); ijet++)
	    {
	      Float_t idphijmet=fabs(deltaPhi(zvv.phi(),origJetsP4[ijet].phi()));
	      if(idphijmet<mindphijmet) mindphijmet=idphijmet;
	      if(origJetsP4[ijet].pt()>30) 
		{
		  njets++;
		  nbtags += (phys.jets[ijet].btag1>btagcut);
		}
	    }
    
	  //##############################################
	  //########     PRESELECTION             ########
	  //##############################################
	  bool passZmass(fabs(zmass-91)<15);
	  bool passDphijmet(mindphijmet>0.5); 
	  bool passZpt(zpt>55);
	  bool pass3dLeptonVeto(true); for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)pass3dLeptonVeto=false;}
	  bool passBveto(nbtags==0);
	  bool passPreselection(passZmass && passZpt && pass3dLeptonVeto && passBveto && passDphijmet);
	  
	  //###############################################
	  //#### SELECTION FOR CUT AND COUNT ANALYSIS #####
	  //###############################################
	  bool passCaCM250(passPreselection && zvv.pt()>70 && mt>222 && mt<272);
	  bool passCaCM300(passPreselection && zvv.pt()>79 && mt>264 && mt<331);
	  bool passCaCM350(passPreselection && zvv.pt()>95 && mt>298 && mt<393);
	  bool passCaCM400(passPreselection && zvv.pt()>115 && mt>327 && mt<460);
	  bool passCaCM450(passPreselection && zvv.pt()>134 && mt>354 && mt<531);
	  bool passCaCM500(passPreselection && zvv.pt()>150 && mt>382 && mt<605);
	  bool passCaCM550(passPreselection && zvv.pt()>159 && mt>413 && mt<684);
	  bool passCaCM600(passPreselection && zvv.pt()>160 && mt>452 && mt<767);
	  	  
	  //###############################################
	  //##### SELECTION FOR SHAPE BASED ANALYSIS ######
	  //###############################################
	  bool passShapeM250( passPreselection && zvv.pt()>70 && mt>180 && mt<300); 
	  bool passShapeM300( passPreselection && zvv.pt()>80 && mt>250 && mt<350); 
	  bool passShapeM350( passPreselection && zvv.pt()>80 && mt>250 && mt<400); 
	  bool passShapeM400( passPreselection && zvv.pt()>80 && mt>250 && mt<450); 
	  bool passShapeM450( passShapeM400);
	  bool passShapeM500( passPreselection && zvv.pt()>80 && mt>250 && mt<600); 
	  bool passShapeM550( passShapeM500);
	  bool passShapeM600( passPreselection && zvv.pt()>80 && mt>250 && mt<750); 
	  
	  //##############################################  
	  //########         GENERAL PLOTS        ########                                                                                                                  
	  //##############################################  
	  if(ivar==0)
	    {
	      //N-1 PLOTS FOR PRESELECTION
	      mon.fillHisto  ("eventflow",tags_full,0,iweight);
	      mon.fillHisto  ("zmass"    ,tags_full,zmass,iweight);
	      if(passZmass)
		{
		  mon.fillHisto("eventflow",tags_full,1,iweight);
		  mon.fillHisto  ("zeta"     ,tags_full,zeta   ,iweight);
		  mon.fillHisto  ("zpt"      ,tags_full,zpt     ,iweight);
		  mon.fillHisto  ("nvtx"     ,tags_full,ev.nvtx,iweight);
		  if(passZpt)
		    {
		      mon.fillHisto("eventflow",tags_full,2,iweight);
		      
		      if(pass3dLeptonVeto )
			{
			  mon.fillHisto("eventflow",tags_full,3,iweight);
			  mon.fillHisto("nbtags",tags_full, nbtags,iweight);
		  
			  if(passBveto)
			    {
			      mon.fillHisto("eventflow",tags_full,4,iweight);
			      mon.fillHisto("njets",tags_full,njets,iweight);
			      mon.fillHisto("mindphijmet",tags_full,mindphijmet,iweight);
			      if(passDphijmet)
				{
				  mon.fillHisto  ("eventflow",tags_full,5,iweight);
				  mon.fillHisto("met",tags_full,zvv.pt(),iweight);
				  mon.fillHisto("mt",tags_full,zvv.pt(),iweight);
				}
			    }
			}
		    }
		}
	    }
	  if(!passPreselection) continue;
	  	  
	  //CUT AND COUNT ANALYSIS
	  if(passCaCM250) mon.fillHisto("finaleventflow",tags_full,0,iweight); 
	  if(passCaCM300) mon.fillHisto("finaleventflow",tags_full,1,iweight); 
	  if(passCaCM350) mon.fillHisto("finaleventflow",tags_full,2,iweight); 
	  if(passCaCM400) mon.fillHisto("finaleventflow",tags_full,3,iweight); 
	  if(passCaCM450) mon.fillHisto("finaleventflow",tags_full,4,iweight); 
	  if(passCaCM500) mon.fillHisto("finaleventflow",tags_full,5,iweight); 
	  if(passCaCM550) mon.fillHisto("finaleventflow",tags_full,6,iweight); 
	  if(passCaCM600) mon.fillHisto("finaleventflow",tags_full,7,iweight); 
     
	  //SHAPE ANALYSIS
	  if(passShapeM250) mon.fillHisto(varNames[ivar]+"finalmt250",tags_full,mt,iweight);
	  if(passShapeM300) mon.fillHisto(varNames[ivar]+"finalmt300",tags_full,mt,iweight);
	  if(passShapeM350) mon.fillHisto(varNames[ivar]+"finalmt350",tags_full,mt,iweight);
	  if(passShapeM400) mon.fillHisto(varNames[ivar]+"finalmt400",tags_full,mt,iweight);
	  if(passShapeM450) mon.fillHisto(varNames[ivar]+"finalmt450",tags_full,mt,iweight);
	  if(passShapeM500) mon.fillHisto(varNames[ivar]+"finalmt500",tags_full,mt,iweight);
	  if(passShapeM550) mon.fillHisto(varNames[ivar]+"finalmt550",tags_full,mt,iweight);
	  if(passShapeM600) mon.fillHisto(varNames[ivar]+"finalmt600",tags_full,mt,iweight);
	}

      //##############################################   EVENT LOOP ENDS   ##############################################
    }
  
  printf("\n"); 
  file->Close();
 

  //##############################################
  //########     SAVING HISTO TO FILE     ########
  //##############################################
  //save control plots to file
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





