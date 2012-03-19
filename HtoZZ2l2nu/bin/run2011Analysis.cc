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
  TString varNames[]={"","_jesup","_jesdown","_jer","_puup","_pudown","_renup","_rendown","_factup","_factdown","_btagup","_btagdown"};
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
  //########    CUTS AND MASS POINTS      ########
  //##############################################

  int NmH=19;
  double* mH = new double[NmH]; double*  cutMet = new double[NmH]; double*  cutMTMin= new double[NmH]; double*  cutMTMax = new double[NmH];
                                double* scutMet = new double[NmH]; double* scutMTMin= new double[NmH]; double* scutMTMax = new double[NmH];
  mH[ 0] = 200;  cutMet[ 0] =  56;  cutMTMin[ 0] = 198;  cutMTMax[ 0] = 187;
  mH[ 1] = 225;  cutMet[ 1] =  63;  cutMTMin[ 1] = 213;  cutMTMax[ 1] = 223;
  mH[ 2] = 250;  cutMet[ 2] =  70;  cutMTMin[ 2] = 229;  cutMTMax[ 2] = 258;
  mH[ 3] = 275;  cutMet[ 3] =  77;  cutMTMin[ 3] = 245;  cutMTMax[ 3] = 293;
  mH[ 4] = 300;  cutMet[ 4] =  84;  cutMTMin[ 4] = 260;  cutMTMax[ 4] = 328;
  mH[ 5] = 325;  cutMet[ 5] =  91;  cutMTMin[ 5] = 276;  cutMTMax[ 5] = 364;
  mH[ 6] = 350;  cutMet[ 6] =  98;  cutMTMin[ 6] = 292;  cutMTMax[ 6] = 399;
  mH[ 7] = 375;  cutMet[ 7] = 105;  cutMTMin[ 7] = 308;  cutMTMax[ 7] = 434;
  mH[ 8] = 400;  cutMet[ 8] = 112;  cutMTMin[ 8] = 323;  cutMTMax[ 8] = 470;
  mH[ 9] = 425;  cutMet[ 9] = 119;  cutMTMin[ 9] = 339;  cutMTMax[ 9] = 505;
  mH[10] = 450;  cutMet[10] = 126;  cutMTMin[10] = 355;  cutMTMax[10] = 540;
  mH[11] = 475;  cutMet[11] = 133;  cutMTMin[11] = 370;  cutMTMax[11] = 576;
  mH[12] = 500;  cutMet[12] = 140;  cutMTMin[12] = 386;  cutMTMax[12] = 611;
  mH[13] = 525;  cutMet[13] = 147;  cutMTMin[13] = 402;  cutMTMax[13] = 646;
  mH[14] = 550;  cutMet[14] = 154;  cutMTMin[14] = 417;  cutMTMax[14] = 682;
  mH[15] = 575;  cutMet[15] = 161;  cutMTMin[15] = 433;  cutMTMax[15] = 717;
  mH[16] = 600;  cutMet[16] = 168;  cutMTMin[16] = 449;  cutMTMax[16] = 752;
  mH[17] = 625;  cutMet[17] = 175;  cutMTMin[17] = 464;  cutMTMax[17] = 788;
  mH[18] = 650;  cutMet[18] = 182;  cutMTMin[18] = 480;  cutMTMax[18] = 823;

  mH[ 0] = 200; scutMet[ 0] =  70; scutMTMin[ 0] = 180; scutMTMax[ 0] = 300;
  mH[ 1] = 225; scutMet[ 1] =  70; scutMTMin[ 1] = 180; scutMTMax[ 1] = 300;
  mH[ 2] = 250; scutMet[ 2] =  70; scutMTMin[ 2] = 180; scutMTMax[ 2] = 300;
  mH[ 3] = 275; scutMet[ 3] =  70; scutMTMin[ 3] = 180; scutMTMax[ 3] = 300;
  mH[ 4] = 300; scutMet[ 4] =  80; scutMTMin[ 4] = 250; scutMTMax[ 4] = 350;
  mH[ 5] = 325; scutMet[ 5] =  80; scutMTMin[ 5] = 250; scutMTMax[ 5] = 350;
  mH[ 6] = 350; scutMet[ 6] =  80; scutMTMin[ 6] = 250; scutMTMax[ 6] = 400;
  mH[ 7] = 375; scutMet[ 7] =  80; scutMTMin[ 7] = 250; scutMTMax[ 7] = 400;
  mH[ 8] = 400; scutMet[ 8] =  80; scutMTMin[ 8] = 250; scutMTMax[ 8] = 450;
  mH[ 9] = 425; scutMet[ 9] =  80; scutMTMin[ 9] = 250; scutMTMax[ 9] = 450;
  mH[10] = 450; scutMet[10] =  80; scutMTMin[10] = 250; scutMTMax[10] = 450;
  mH[11] = 475; scutMet[11] =  80; scutMTMin[11] = 250; scutMTMax[11] = 450;
  mH[12] = 500; scutMet[12] =  80; scutMTMin[12] = 250; scutMTMax[12] = 600;
  mH[13] = 525; scutMet[13] =  80; scutMTMin[13] = 250; scutMTMax[13] = 600;
  mH[14] = 550; scutMet[14] =  80; scutMTMin[14] = 250; scutMTMax[14] = 600;
  mH[15] = 575; scutMet[15] =  80; scutMTMin[15] = 250; scutMTMax[15] = 600;
  mH[16] = 600; scutMet[16] =  80; scutMTMin[16] = 250; scutMTMax[16] = 750;
  mH[17] = 625; scutMet[17] =  80; scutMTMin[17] = 250; scutMTMax[17] = 750;
  mH[18] = 650; scutMet[18] =  80; scutMTMin[18] = 250; scutMTMax[18] = 750;
 
  char** mHtxt = new char*[NmH]; for(int ImH=0;ImH<NmH;ImH++){mHtxt[ImH] = new char[255]; sprintf(mHtxt[ImH],"%3.0f",mH[ImH]); }

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
  h->GetXaxis()->SetBinLabel(6,"#delta #phi(jet,E_{T}^{miss})");
  h->GetXaxis()->SetBinLabel(7,"E_{T}^{miss}>80");

  mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 50,-10,10) );
  mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,0,200) );
  mon.addHistogram( new TH1F("nvtx",";Vertices;Events",50,0,50) ); 
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
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1F( "met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "metaftersmear"  , ";E_{T}^{miss} after smear;Events", 50,0,500) );
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 80,150,950) );
  mon.addHistogram( new TH1F( "mtaftersmear"  , ";M_{T} after smear;Events", 80,150,950) );

  for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
      TH1 *cacH = (TH1F *) mon.addHistogram( new TH1F (TString("finaleventflow")+varNames[ivar],";Category;Event count;",NmH+1,0,NmH+1) );
      for(int ImH=0;ImH<NmH;ImH++){
         cacH->GetXaxis()->SetBinLabel(ImH+1,TString("mH=")+mHtxt[ImH]);
         mon.addHistogram( new TH1F (TString("finalmt")+mHtxt[ImH]+varNames[ivar],";M_{T} [GeV/c^{2}];Events;",80,150,950) );
      }
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

   mon.addHistogram( new TH1F ("optim_eventflow1"  , ";cut index;yields" ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) );
   mon.addHistogram( new TH2F ("optim_MT1",";cut index;M_{T} [GeV/c^{2}];",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 80,150,950) );
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
  edm::LumiReWeighting *LumiWeights=0;
  reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC)
    {
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PShiftUp = new reweight::PoissonMeanShifter(+0.6);
      PShiftDown = new reweight::PoissonMeanShifter(-0.6);
    }

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
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

      //##############################################   EVENT LOOP STARTS   ##############################################
      
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      //LorentzVector zll  = isGammaEvent ? gammaEvHandler->massiveGamma("ll") : phys.leptons[0]+phys.leptons[1];
      LorentzVector zll  = phys.leptons[0]+phys.leptons[1];
      LorentzVectorCollection jetsP4, smearJetsP4;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
	{
	  jetsP4.push_back( phys.jets[ijet] );
	  smearJetsP4.push_back( isMC ? METUtils::smearedJet(phys.jets[ijet]):phys.jets[ijet]);
	}

      //categorize events
      TString tag_cat;
      switch(ev.cat){
        case EMU : tag_cat = "emu";   break;
        case MUMU: tag_cat = "mumu";  break;
        case EE  : tag_cat = "ee";    break;
        default  : continue;
      }
      if(isMC && mctruthmode==1 && ev.mccat!=DY_EE && ev.mccat!=DY_MUMU)  continue;
      if(isMC && mctruthmode==2 && ev.mccat!=DY_TAUTAU) continue;

      bool isGammaEvent = false;
      if(gammaEvHandler){
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
      //tags_full.push_back(tag_cat + tag_subcat);

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
      
      //prepare variations 
      LorentzVectorCollection zvvs;
      std::vector<LorentzVectorCollection> jets;
      if(runSystematics) jet::computeVariation(jetsP4,phys.met[0], jets, zvvs, &stdPtResol,&stdEtaResol,&stdPhiResol,&jecUnc);
      zvvs.insert(zvvs.begin(),phys.met[0]);
      jets.insert(jets.begin(),jetsP4);
      std::vector<Float_t>  mts;
      for(size_t ivar=0; ivar<(runSystematics?3:1); ivar++){
          Float_t imt     = METUtils::transverseMass(zll,zvvs[ivar],true);
          mts.push_back(imt);
      }
      
      //run the variations
      for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
          float iweight = weight;                                  //nominal
          if(ivar==4)            iweight *=TotalWeight_plus;        //pu up
          if(ivar==5)            iweight *=TotalWeight_minus;       //pu down
          if(ivar>=6 && isMC_GG) iweight *=ev.hptWeights[ivar-5];   //ren/fact scales

          Float_t zmass=zll.mass();
          Float_t zpt=zll.pt();
          Float_t zeta=zll.eta();
          LorentzVectorCollection &origJetsP4=jets[ivar>3?0:ivar];            
          LorentzVector zvv  = zvvs[ivar>2?0:ivar];
          Float_t mt         = mts[ivar>2?0:ivar];
          int njets(0),nbtags(0);
          Float_t mindphijmet(9999.);
          Float_t btagcut(2.0); if(ivar==10) btagcut=2.03; else if(ivar==11) btagcut=1.97;
          for(size_t ijet=0; ijet<origJetsP4.size(); ijet++){
              Float_t idphijmet=fabs(deltaPhi(zvv.phi(),origJetsP4[ijet].phi()));
              if(idphijmet<mindphijmet) mindphijmet=idphijmet;
              if(origJetsP4[ijet].pt()>30){
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
	  bool passBaseMet(zvv.pt()>70);
          bool passPreselection(passZmass && passZpt && pass3dLeptonVeto && passBveto && passDphijmet);
          
          //##############################################  
          //########         GENERAL PLOTS        ########                                                                                                                  
          //##############################################  

	  //N-1 PLOTS FOR PRESELECTION
          if(ivar==0){
              mon.fillHisto  ("eventflow",tags_full,0,iweight);
              mon.fillHisto  ("zmass"    ,tags_full,zmass,iweight);

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
                  
                          if(passBveto){
                              mon.fillHisto("eventflow",tags_full,4,iweight);
                              mon.fillHisto("njets",tags_full,njets,iweight);
                              mon.fillHisto("mindphijmet",tags_full,mindphijmet,iweight);

                              if(passDphijmet){
                                  mon.fillHisto  ("eventflow",tags_full,5,iweight);
                                  mon.fillHisto("met",tags_full,zvv.pt(),iweight);
                                  mon.fillHisto("mt",tags_full,mt,iweight);
				  LorentzVector metafterSmear(zvv);
				  for(size_t ijet=0; ijet<jetsP4.size(); ijet++) metafterSmear -= (smearJetsP4[ijet]-jetsP4[ijet]);
                                  mon.fillHisto("metaftersmear",tags_full,metafterSmear.pt(),iweight);
				  Float_t mtafterSmear = METUtils::transverseMass(zll,metafterSmear,true);
                                  mon.fillHisto("mtaftersmear",tags_full,mtafterSmear,iweight);
				  
				  if(passBaseMet){
				    mon.fillHisto  ("eventflow",tags_full,6,iweight);
				  }

			      }
                          }
                      }
                  }
              }
          }
          if(!passPreselection) continue;
          
          for(int ImH=0;ImH<NmH;ImH++){
              //CUT AND COUNT ANALYSIS
              if(zvv.pt()>cutMet[ImH] && mt>cutMTMin[ImH] && mt<cutMTMax[ImH])    mon.fillHisto(TString("finaleventflow")+varNames[ivar],tags_full,ImH,iweight);
             //SHAPE ANALYSIS
              if(zvv.pt()>scutMet[ImH] && mt>scutMTMin[ImH] && mt<scutMTMax[ImH]) mon.fillHisto(TString("finalmt")+mHtxt[ImH]+varNames[ivar],tags_full,mt,iweight);
          }

           //Fill histogram for posterior optimization
	  if(ivar==0){
             for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
                 if(zvv.pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index]){
                    mon.fillHisto("optim_eventflow1"          ,tags_full,    index, weight);
                    mon.fillHisto(TString("optim_MT1"),tags_full,index, mt,iweight);
                 }
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





