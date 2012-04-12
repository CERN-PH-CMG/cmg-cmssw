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

  //systematic uncertainties
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
  TString varNames[]={"","_jesup","_jesdown","_jerup","_jerdown","_puup","_pudown","_renup","_rendown","_factup","_factdown","_btagup","_btagdown"};
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
    TString hqtWeightsFileURL = runProcess.getParameter<std::string>("hqtWeightsFile"); gSystem->ExpandPathName(hqtWeightsFileURL);
    TFile *fin=TFile::Open(hqtWeightsFileURL);
    if(fin){
      cout << "HpT weights (and uncertainties) will be applied from: " << hqtWeightsFileURL << endl;
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
  TH1F *h=(TH1F*) mon.addHistogram( new TH1F ("eventflow", ";Step;Events", 7,0,7) );
  h->GetXaxis()->SetBinLabel(1,"Preselected");
  h->GetXaxis()->SetBinLabel(2,"|M-M_{Z}|<15");
  h->GetXaxis()->SetBinLabel(3,"Z_{pT}>25");
  h->GetXaxis()->SetBinLabel(4,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(5,"b-veto");
  h->GetXaxis()->SetBinLabel(6,"#delta #phi(jet,E_{T}^{miss})>0.5");
  h->GetXaxis()->SetBinLabel(7,"E_{T}^{miss}>70");

  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 50,-10,10) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,40,250) );
  mon.addHistogram( new TH1F( "zmassraw", ";M^{ll} (raw);Events", 100,40,250) );
  mon.addHistogram( new TH1F("nvtx",";Vertices;Events",50,0,50) ); 
  mon.addHistogram( new TH1F("njets"        ,";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nbtags", ";b-tag multiplicity; Events", 5,0,5) );  
  for(size_t ibin=1; ibin<=5; ibin++){
    TString label("");
    if(ibin==5) label +="#geq";
    else        label +="=";
    label += (ibin-1);
    mon.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
    mon.getHisto("nbtags")->GetXaxis()->SetBinLabel(ibin,label);
  }
  mon.addHistogram( new TH1F("btagvetosel", ";b-tag discriminator; Events", 6,0,6) );  
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",20,0,4) );
  mon.addHistogram( new TH1F( "wzdecaymode", ";W decay mode (gen level);Events",20,0,20) );
  mon.addHistogram( new TH1F( "met_met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_metRaw"  , ";E_{T}^{miss} (raw);Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "met_redMetRaw"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) (raw);Events", 50,0,500) );
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mtRaw"  , ";M_{T} (raw);Events", 100,0,1000) );

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> optim_Cuts1_met;
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=50;met<200;met+=5.0){
	 if(met>100 && int(met)%10!=0)continue;
         if(met>140 && int(met)%20!=0)continue;
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
  //add last year cut
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


   TH1F* Hoptim_cuts1_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
      Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
      Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
      Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
   }

   TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;
   for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
   {
       Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
       mon.addHistogram( new TH2F (TString("mt_shapes")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 80,150,950) );
       if(ivar==0)mon.addHistogram( new TH2F (TString("mt_shapesBTagSB")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 80,150,950) );
       mon.addHistogram( new TH2F (TString("mt_redMet_shapes")+varNames[ivar],";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 80,150,950) );
       mon.addHistogram( new TH2F (TString("mt3")+varNames[ivar],";cut index;M_{T}^{3rd lepton} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 50,0,250) );
       TH2F *h=(TH2F *) mon.addHistogram( new TH2F ("nonresbckg_ctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
       h->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
       h->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
       h->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
       h->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
       h->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
       h->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");
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
  edm::LumiReWeighting *LumiWeights=0;
  reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC)
    {
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PShiftUp = new reweight::PoissonMeanShifter(+0.8);
      PShiftDown = new reweight::PoissonMeanShifter(-0.8);
    }

  //event Categorizer
//  EventCategory eventCategoryInst(0); //inclusive analysis
//  EventCategory eventCategoryInst(1); //jet binning
//  EventCategory eventCategoryInst(2); //vbf binning
  EventCategory eventCategoryInst(3); //jet+vbf binning



  //##############################################
  //########           EVENT LOOP         ########
  //##############################################

 
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
   
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      
      
      //categorize events
      TString tag_cat;
      switch(ev.cat){
      case EMU : tag_cat = "emu";   break;
      case MUMU: tag_cat = "mumu";  break;
      case EE  : tag_cat = "ee";    break;
      default  : continue;
      }
      //      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) && !isZZ2l2nu(ev.mccat) ) continue;
      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
      if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;
      

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
      std::vector<TString> tags_full;
      tags_full.push_back("all");
      tags_full.push_back(tag_cat);
//      if(tag_subcat=="vbf") tags_full.push_back(tag_cat+"_"+tag_subcat);
      if(tag_subcat!="")tags_full.push_back(tag_cat + tag_subcat);
      if(tag_subcat=="geq2jets" || tag_subcat=="vbf")tags_full.push_back(tag_cat + "geq2jetsInc");
      if(tag_subcat!="vbf")tags_full.push_back(tag_cat + "novbf");

      //pileup and Higgs pT weight
      //float weight=ev.puWeight;
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight = LumiWeights->weight( ev.ngenITpu );
        TotalWeight_plus = PShiftUp->ShiftWeight( ev.ngenITpu );
        TotalWeight_minus = PShiftDown->ShiftWeight( ev.ngenITpu );
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


      //analyze the leptons
      LorentzVector lep1=phys.leptons[0];
      LorentzVector lep2=phys.leptons[1];
      LorentzVector zllraw=lep1+lep2;
      LorentzVector zll(zllraw);
      if(!isMC)
	{
	  //apply regression corrections for SC energy
	  float l1corr( fabs(phys.leptons[0].id)==ELECTRON ? ev.en_corren[phys.leptons[0].pid]/phys.leptons[0].energy() : 1.0); if(l1corr==0) l1corr=1;
	  float l2corr( fabs(phys.leptons[1].id)==ELECTRON ? ev.en_corren[phys.leptons[1].pid]/phys.leptons[1].energy() : 1.0); if(l2corr==0) l2corr=1;
	  zll = LorentzVector(l1corr*lep1+l2corr*lep2);
	}

      //analyze JET/MET
      LorentzVectorCollection jetsP4;
      std::vector<double> genJetsPt;
      for(size_t ijet=0; ijet<phys.ajets.size(); ijet++)
	{
	  jetsP4.push_back( phys.ajets[ijet] );
	  genJetsPt.push_back( phys.ajets[ijet].genPt);
	}
      //base raw METs
      LorentzVector zvvRaw(phys.met[0]);
      LorentzVector rawClusteredMet(zll);            rawClusteredMet *= -1;
      for(size_t ijet=0; ijet<jetsP4.size(); ijet++) rawClusteredMet -= jetsP4[ijet];
      LorentzVector redMetRaw(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, rawClusteredMet, zvvRaw,false));
      Float_t mtRaw( METUtils::transverseMass(zll,zvvRaw,true) );
      
      //prepare variations (first variation is the baseline, corrected for JER) 
      LorentzVectorCollection zvvs,redMets;
      std::vector<Float_t>  mts,mt3s,redMetLs,redMetTs;
      std::vector<LorentzVectorCollection> jets;
      METUtils::computeVariation(jetsP4, genJetsPt, zvvRaw, jets, zvvs, &jecUnc);
      for(size_t ivars=0; ivars<zvvs.size(); ivars++)
	{
	  LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
	  for(size_t ijet=0; ijet<jets[ivars].size(); ijet++) clusteredMetP4 -= jets[ivars][ijet];
	  METUtils::stRedMET redMetOut; 
	  redMets.push_back(   METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4, zvvs[ivars],false,&redMetOut) );
	  redMetLs.push_back( redMetOut.redMET_l );
	  redMetTs.push_back( redMetOut.redMET_t );
	  mts.push_back(METUtils::transverseMass(zll,zvvs[ivars],true));
	  mt3s.push_back(phys.leptons.size()>2 ? METUtils::transverseMass(phys.leptons[2],zvvs[ivars],false) : 0. );
	}
      
      //
      //run the variations
      //
      for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
	float iweight = weight;                                               //nominal
	if(ivar==5)                        iweight *=TotalWeight_plus;        //pu up
	if(ivar==6)                        iweight *=TotalWeight_minus;       //pu down
	if(ivar<=10 && ivar>=7 && isMC_GG) iweight *=ev.hptWeights[ivar-6];   //ren/fact scales
	
	Float_t zmassraw=zllraw.mass();
	Float_t zmass=zll.mass();
	Float_t zpt=zll.pt();
	Float_t zeta=zll.eta();
	LorentzVectorCollection &origJetsP4=jets[ivar>4?0:ivar];            
	LorentzVector zvv    = zvvs[ivar>4?0:ivar];
	LorentzVector redMet = redMets[ivar>4?0:ivar];
	Float_t redMetL      = redMetLs[ivar>4?0:ivar];
	Float_t redMetT      = redMetTs[ivar>4?0:ivar];
	Float_t mt           = mts[ivar>4?0:ivar];
	Float_t mt3          = mt3s[ivar>4?0:ivar]; 
	int nBtaggedVsDisc[6]={0,0,0,0,0,0};
	int njets(0),nbtags(0);
	Float_t mindphijmet(9999.);
	Float_t btagcut(2.0); if(ivar==10) btagcut=2.03; else if(ivar==11) btagcut=1.97;
	for(size_t ijet=0; ijet<origJetsP4.size(); ijet++){
	  Float_t idphijmet=fabs(deltaPhi(zvv.phi(),origJetsP4[ijet].phi()));
	  if(idphijmet<mindphijmet) mindphijmet=idphijmet;
	  if(origJetsP4[ijet].pt()>30){
	    njets++;
	    nbtags += (phys.ajets[ijet].btag1>btagcut);
	    nBtaggedVsDisc[0] +=(phys.ajets[ijet].btag1>1.7);
	    nBtaggedVsDisc[1] +=(phys.ajets[ijet].btag1>2.0);
	    nBtaggedVsDisc[2] +=(phys.ajets[ijet].btag1>3.3);
	    nBtaggedVsDisc[3] +=(phys.ajets[ijet].btag2>0.244);
	    nBtaggedVsDisc[4] +=(phys.ajets[ijet].btag2>0.679);
	    nBtaggedVsDisc[5] +=(phys.ajets[ijet].btag2>0.898);
	  }
	}
	
	//##############################################
	//########     PRESELECTION             ########
	//##############################################
	if(zmass<40) continue; // this is required by the HZZ skim anyhow
	bool passZmass(fabs(zmass-91)<15);
	bool isZsideBand( (zmass>40 && zmass<70) || (zmass>110 && zmass<200));
	bool isZsideBandPlus( (zmass>110 && zmass<200));
	bool passDphijmet(mindphijmet>0.5); 
	bool passZpt(zpt>55);
	bool pass3dLeptonVeto(true); int nExtraLep(0); for(unsigned int i=2;i<phys.leptons.size();i++) { if(phys.leptons[i].pt()>10){ nExtraLep++; pass3dLeptonVeto=false;} }
	bool passBveto(nbtags==0);
	bool passBaseMet(zvv.pt()>70);
     
    
	//##############################################  
	//########         GENERAL PLOTS        ########                                                                                                                  
	//##############################################  

	//N-1 PLOTS FOR PRESELECTION
	if(ivar==0){
	  mon.fillHisto  ("eventflow",tags_full,0,iweight);
	  mon.fillHisto  ("zmass"    ,tags_full,zmass,iweight);
	  mon.fillHisto  ("zmassraw"    ,tags_full,zmassraw,iweight);

	  if(passZmass){
	    mon.fillHisto("eventflow",tags_full,1,iweight);
	    mon.fillHisto  ("zeta"     ,tags_full,zeta   ,iweight);
	    mon.fillHisto  ("zpt"      ,tags_full,zpt     ,iweight);
	    mon.fillHisto  ("nvtx"     ,tags_full,ev.nvtx,iweight);

	    if(passZpt){
	      mon.fillHisto("eventflow",tags_full,2,iweight);
                      
	      if(pass3dLeptonVeto){
		mon.fillHisto("eventflow",tags_full,3,iweight);
		mon.fillHisto("nbtags",tags_full, nbtags,iweight);
		for(int ibtag=0; ibtag<6; ibtag++)
		  {
		    if(nBtaggedVsDisc[ibtag]==0)
		      mon.fillHisto("btagvetosel",tags_full,ibtag,iweight);
		  }

		if(passBveto){
		  mon.fillHisto("eventflow",tags_full,4,iweight);
		  mon.fillHisto("njets",tags_full,njets,iweight);
		  mon.fillHisto("mindphijmet",tags_full,mindphijmet,iweight);
		  //mon.fillHisto("wzdecaymode",tags_full,getWZdecayMode(ev).first,iweight);

		  if(passDphijmet){
		    mon.fillHisto("eventflow",tags_full,5,iweight);
		    mon.fillHisto("met_met",tags_full,zvv.pt(),iweight);
		    mon.fillHisto("met_metRaw",tags_full,zvvRaw.pt(),iweight);
		    mon.fillHisto("met_redMet",tags_full,redMet.pt(),iweight);
		    mon.fillHisto("met_redMetL",tags_full,redMetT,iweight);
		    mon.fillHisto("met_redMetT",tags_full,redMetL,iweight);
		    mon.fillHisto("met_redMetRaw",tags_full,redMetRaw.pt(),iweight);
		    mon.fillHisto("mt",tags_full,mt,iweight);
		    mon.fillHisto("mtRaw",tags_full,mtRaw,iweight);
				  
		    if(passBaseMet){
		      mon.fillHisto  ("eventflow",tags_full,6,iweight);
		    }
		  }
		}
	      }
	    }
	  }
	}

	  
	  //Fill histogram for posterior optimization, or for control regions
          bool passPreselection             (passZmass && passZpt && pass3dLeptonVeto && passDphijmet && passBveto);
	  bool passPreselectionMbvetoMzmass (             passZpt && pass3dLeptonVeto && passDphijmet             );          
	  bool passPreselectionM3dlep       (passZmass && passZpt                     && passDphijmet && passBveto);
          for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
	    
	    if(redMet.pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
	      if(passPreselection                                        )          mon.fillHisto(TString("mt_redMet_shapes")+varNames[ivar],tags_full,index, mt,iweight);
	  	    
	    if(zvv.pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
	      {
		if(passPreselection                                                    )   mon.fillHisto(TString("mt_shapes")+varNames[ivar],tags_full,index, mt,iweight);
                if(passPreselectionMbvetoMzmass && passZmass   && !passBveto && ivar==0)   mon.fillHisto(TString("mt_shapesBTagSB")+varNames[ivar],tags_full,index, mt,iweight);
		if(passPreselectionM3dlep       && !pass3dLeptonVeto && nExtraLep==1   )   mon.fillHisto(TString("mt3")+varNames[ivar],tags_full,index, mt3,iweight);
		if(passPreselectionMbvetoMzmass && passZmass         && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,0,iweight);
		if(passPreselectionMbvetoMzmass && isZsideBand       && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,1,iweight);
		if(passPreselectionMbvetoMzmass && isZsideBandPlus   && passBveto      )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,2,iweight);
		if(passPreselectionMbvetoMzmass && passZmass         && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,3,iweight);
		if(passPreselectionMbvetoMzmass && isZsideBand       && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,4,iweight);
		if(passPreselectionMbvetoMzmass && isZsideBandPlus   && !passBveto     )   mon.fillHisto("nonresbckg_ctrl"+varNames[ivar],tags_full,index,5,iweight);
	     }
          }
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





