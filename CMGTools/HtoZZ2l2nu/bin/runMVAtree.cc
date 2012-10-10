#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "CMGTools/HtoZZ2l2nu/interface/EfficiencyMap.h"

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

TTree *hzz_tree     = new TTree("hzz_tree","Train and Test Tree");
double hzz_lept1_PT;
double hzz_lept2_PT;
double hzz_lept1_ETA;
double hzz_lept2_ETA;
double hzz_lept1_PHI;
double hzz_lept2_PHI;
double hzz_lept1_DetIso;
double hzz_lept2_DetIso;
double hzz_lept1_PFIso2012;
double hzz_lept2_PFIso2012;
double hzz_lept1_PFIsoDbeta;
double hzz_lept2_PFIsoDbeta;
double hzz_Z_Mass;
double hzz_Z_PT;
double hzz_Z_Eta;
double hzz_Z_Phi;
double PUWeight;
double hzz_PFMET;
double hzz_MET_Phi;
double hzz_TransMass_Higgs;
double hzz_Jet_PT;
double hzz_Jet_ETA;
double hzz_Jet_PHI;
double hzz_Jet_Energy;
int ev_category;
int hzz_N_Jets;
double hzz_dPhi_JetMet;
int hzz_lept1_ID;
int hzz_lept2_ID;
double event_weight;
int ev_num;
int run_num;
int lumi_num;
bool vbf_flag;
double hzz_Jet_SumPT;
double hzz_lept1_gIso;
double hzz_lept2_gIso;
double hzz_lept1_chIso;
double hzz_lept2_chIso;
double hzz_lept1_nhIso;
double hzz_lept2_nhIso;
double hzz_lept1_nIso;
double hzz_lept2_nIso;
double hzz_rho;
double lineshape_weight;
double hzz_BTag;
double hzz_RedMet;
double hzz_RedMetL;
double hzz_RedMetT;
double hzz_RawMet;
double hzz_MVAMet;



TBranch *b_hzz_Jet_SumPT = hzz_tree->Branch("hzz_Jet_SumPT",&hzz_Jet_SumPT,"hzz_Jet_SumPT/D");
TBranch *b_hzz_lept1_ID = hzz_tree->Branch("hzz_lept1_ID", &hzz_lept1_ID ,"hzz_lept1_ID/I");
TBranch *b_hzz_lept2_ID = hzz_tree->Branch("hzz_lept2_ID", &hzz_lept2_ID ,"hzz_lept2_ID/I");
TBranch *b_hzz_lept1_PT = hzz_tree->Branch("hzz_lept1_PT", &hzz_lept1_PT ,"hzz_lept1_PT/D");
TBranch *b_hzz_lept2_PT = hzz_tree->Branch("hzz_lept2_PT", &hzz_lept2_PT ,"hzz_lept2_PT/D");
TBranch *b_hzz_lept1_ETA = hzz_tree->Branch("hzz_lept1_ETA", &hzz_lept1_ETA ,"hzz_lept1_ETA/D");
TBranch *b_hzz_lept2_ETA = hzz_tree->Branch("hzz_lept2_ETA", &hzz_lept2_ETA ,"hzz_lept2_ETA/D");
TBranch *b_hzz_lept1_PHI = hzz_tree->Branch("hzz_lept1_PHI", &hzz_lept1_PHI ,"hzz_lept1_PHI/D");
TBranch *b_hzz_lept2_PHI = hzz_tree->Branch("hzz_lept2_PHI", &hzz_lept2_PHI ,"hzz_lept2_PHI/D");
TBranch *b_hzz_lept1_DetIso = hzz_tree->Branch("hzz_lept1_DetIso", &hzz_lept1_DetIso ,"hzz_lept1_DetIso/D");
TBranch *b_hzz_lept2_DetIso = hzz_tree->Branch("hzz_lept2_DetIso", &hzz_lept2_DetIso ,"hzz_lept2_DetIso/D");
TBranch *b_hzz_lept1_PFIso2012 = hzz_tree->Branch("hzz_lept1_PFIso2012", &hzz_lept1_PFIso2012 ,"hzz_lept1_PFIso2012/D");
TBranch *b_hzz_lept2_PFIso2012 = hzz_tree->Branch("hzz_lept2_PFIso2012", &hzz_lept2_PFIso2012 ,"hzz_lept2_PFIso2012/D");
TBranch *b_hzz_lept1_PFIsoDbeta = hzz_tree->Branch("hzz_lept1_PFIsoDbeta", &hzz_lept1_PFIsoDbeta ,"hzz_lept1_PFIsoDbeta/D");
TBranch *b_hzz_lept2_PFIsoDbeta = hzz_tree->Branch("hzz_lept2_PFIsoDbeta", &hzz_lept2_PFIsoDbeta ,"hzz_lept2_PFIsoDbeta/D");
TBranch *b_hzz_Z_Mass = hzz_tree->Branch("hzz_Z_Mass", &hzz_Z_Mass ,"hzz_Z_Mass/D");
TBranch *b_hzz_Z_PT = hzz_tree->Branch("hzz_Z_PT", &hzz_Z_PT ,"hzz_Z_PT/D");
TBranch *b_hzz_Z_Eta = hzz_tree->Branch("hzz_Z_Eta", &hzz_Z_Eta ,"hzz_Z_Eta/D");
TBranch *b_hzz_Z_Phi = hzz_tree->Branch("hzz_Z_Phi", &hzz_Z_Phi ,"hzz_Z_Phi/D");
TBranch *b_hzz_PFMET = hzz_tree->Branch("hzz_PFMET", &hzz_PFMET ,"hzz_PFMET/D");
TBranch *b_hzz_MET_Phi = hzz_tree->Branch("hzz_MET_Phi", &hzz_MET_Phi ,"hzz_MET_Phi/D");
TBranch *b_PUWeight = hzz_tree->Branch("PUWeight", &PUWeight ,"PUWeight/D");
TBranch *b_hzz_TransMass_Higgs = hzz_tree->Branch("hzz_TransMass_Higgs", &hzz_TransMass_Higgs ,"hzz_TransMass_Higgs/D");
TBranch *b_hzz_Jet_PT = hzz_tree->Branch("hzz_Jet_PT", &hzz_Jet_PT ,"hzz_Jet_PT/D");
TBranch *b_hzz_Jet_ETA = hzz_tree->Branch("hzz_Jet_ETA", &hzz_Jet_ETA ,"hzz_Jet_ETA/D");
TBranch *b_hzz_Jet_PHI = hzz_tree->Branch("hzz_Jet_PHI", &hzz_Jet_PHI ,"hzz_Jet_PHI/D");
TBranch *b_hzz_Jet_Energy = hzz_tree->Branch("hzz_Jet_Energy", &hzz_Jet_Energy ,"hzz_Jet_Energy/D");
TBranch *b_ev_category = hzz_tree->Branch("ev_category", &ev_category ,"ev_category/I");
TBranch *b_hzz_N_Jets = hzz_tree->Branch("hzz_N_Jets", &hzz_N_Jets , "hzz_N_Jets/I");
TBranch *b_hzz_dPhi_JetMet = hzz_tree->Branch("hzz_dPhi_JetMet", &hzz_dPhi_JetMet , "hzz_dPhi_JetMet/D");
TBranch *b_event_weight = hzz_tree->Branch("event_weight", &event_weight ,"event_weight/D");
TBranch *b_ev_num = hzz_tree->Branch("ev_num", &ev_num , "ev_num/I");
TBranch *b_run_num = hzz_tree->Branch("run_num", &run_num , "run_num/I");
TBranch *b_lumi_num = hzz_tree->Branch("lumi_num", &lumi_num , "lumi_num/I");
TBranch *b_vbf_flag = hzz_tree->Branch("vbf_flag", &vbf_flag , "vbf_flag/B");
TBranch *b_hzz_lept1_gIso = hzz_tree->Branch("hzz_lept1_gIso", &hzz_lept1_gIso , "hzz_lept1_gIso/D");
TBranch *b_hzz_lept2_gIso = hzz_tree->Branch("hzz_lept2_gIso", &hzz_lept2_gIso , "hzz_lept2_gIso/D");
TBranch *b_hzz_lept1_chIso = hzz_tree->Branch("hzz_lept1_chIso", &hzz_lept1_chIso , "hzz_lept1_chIso/D");
TBranch *b_hzz_lept2_chIso = hzz_tree->Branch("hzz_lept2_chIso", &hzz_lept2_chIso , "hzz_lept2_chIso/D");
TBranch *b_hzz_lept1_nhIso = hzz_tree->Branch("hzz_lept1_nhIso", &hzz_lept1_nhIso , "hzz_lept1_nhIso/D");
TBranch *b_hzz_lept2_nhIso = hzz_tree->Branch("hzz_lept2_nhIso", &hzz_lept2_nhIso , "hzz_lept2_nhIso/D");
TBranch *b_hzz_lept1_nIso = hzz_tree->Branch("hzz_lept1_nIso", &hzz_lept1_nIso , "hzz_lept1_nIso/D");
TBranch *b_hzz_lept2_nIso = hzz_tree->Branch("hzz_lept2_nIso", &hzz_lept2_nIso , "hzz_lept2_nIso/D");
TBranch *b_hzz_rho = hzz_tree->Branch("hzz_rho", &hzz_rho , "hzz_rho/D");
TBranch *b_lineshape_weight = hzz_tree->Branch("lineshape_weight", &lineshape_weight , "lineshape_weight/D");
TBranch *b_hzz_BTag = hzz_tree->Branch("hzz_BTag", &hzz_BTag , "hzz_BTag/D");
TBranch *b_hzz_RedMet = hzz_tree->Branch("hzz_RedMet", &hzz_RedMet , "hzz_RedMet/D");
TBranch *b_hzz_RedMetL = hzz_tree->Branch("hzz_RedMetL", &hzz_RedMetL , "hzz_RedMetL/D");
TBranch *b_hzz_RedMetT = hzz_tree->Branch("hzz_RedMetT", &hzz_RedMetT , "hzz_RedMetT/D");
TBranch *b_hzz_RawMet = hzz_tree->Branch("hzz_RawMet", &hzz_RawMet , "hzz_RawMet/D");
TBranch *b_hzz_MVAMet = hzz_tree->Branch("hzz_MVAMet", &hzz_MVAMet , "hzz_MVAMet/D");



  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  double xsec=runProcess.getParameter<double>("xsec");

std::cout << "xsec =  " << xsec << std::endl;

  bool isMC = runProcess.getParameter<bool>("isMC");
  bool runBlinded = runProcess.getParameter<bool>("runBlinded");
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
  
  TString outTxtUrl= outUrl + "/" + outFileUrl + ".txt";
  FILE* outTxtFile = NULL;
  //if(!isMC)
  outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());
  
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
  

  // this is disabled for the moment
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isMC_GG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isMC_VBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  TFile *fin;
  int cmEnergy(8);
  if(url.Contains("7TeV")) cmEnergy=7;
  std::vector<TGraph *> hWeightsGrVec,hLineShapeGrVec;

  if(isMC_GG){  
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,4);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
  
    //H pT
    if(cmEnergy==7){
      TString hqtWeightsFileURL = runProcess.getParameter<std::vector<std::string> >("hqtWeightsFile")[0]; gSystem->ExpandPathName(hqtWeightsFileURL);
      fin=TFile::Open(hqtWeightsFileURL);
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
    }    
  }else if(isMC_VBF){
    size_t VBFStringpos =  string(url.Data()).find("VBF");
    string StringMass = string(url.Data()).substr(VBFStringpos+6,4);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    VBFString = string(url.Data()).substr(VBFStringpos);
  }

  
 //LINE SHAPE
 TString lineShapeWeightsFileURL = runProcess.getParameter<std::vector<std::string> >("hqtWeightsFile")[1]; gSystem->ExpandPathName(lineShapeWeightsFileURL);
 fin=TFile::Open(lineShapeWeightsFileURL);
 if(fin){
   char buf[100]; sprintf(buf,"Higgs%d_%dTeV/",int(HiggsMass),cmEnergy);
   cout << "Line shape weights (and uncertainties) will be applied from " << buf << " @ " << lineShapeWeightsFileURL << endl;
   TString wgts[]={"rwgtpint","rwgtpint_up","rwgtpint_down","rwgt"};
   for(size_t i=0; i<4; i++)
     {
       TGraph *gr= (TGraph *) fin->Get(buf+wgts[i]);
       if(gr) hLineShapeGrVec.push_back((TGraph *)gr->Clone());
     }
 }
 fin->Close();
 delete fin;


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


  //pileup weighting: based on vtx for now...
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  bool useObservedPU(true);
  //bool useObservedPU(use2011Id);
  if(!use2011Id && url.Contains("toZZto2L")) useObservedPU=true;
  if(isMC){
    TString puDist("evAnalyzer/h2zz/pileuptrue");
    if(useObservedPU) puDist="evAnalyzer/h2zz/pileup";
    TH1F* histo = (TH1F *) file->Get(puDist);
    if(!histo)std::cout<<"pileup histogram is null!!!\n";
    for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
    delete histo;
    if(dataPileupDistribution.size()==0) dataPileupDistribution=mcPileupDistribution;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights=0;
  PuShifter_t PuShifters;
    reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC){
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PuShifters=getPUshifters(dataPileupDistribution,0.05);
            PShiftUp = new reweight::PoissonMeanShifter(+0.8);
            PShiftDown = new reweight::PoissonMeanShifter(-0.8);
  }



  //event Categorizer
//  EventCategory eventCategoryInst(0); //inclusive analysis
  EventCategory eventCategoryInst(2); //VBF, no VBF

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

bool mustBlind(false);

      //##############################################   EVENT LOOP STARTS   ##############################################
   
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
//      if(runBlinded && !isMC && evSummaryHandler.hasSpoilerAlert(!isMC)) continue;
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
       mustBlind = (!isMC && runBlinded && evSummaryHandler.hasSpoilerAlert(!isMC));
 

    
event_weight = xsec/cnorm;
//std::cout <<"event weight = " << event_weight << std::endl;      
//std::cout << ev.event << " "<< ev.run << " " << ev.lumi << std::endl;
      //categorize events
//      bool vbfFlag(false);
      bool vbfFlag(false);
      TString tag_cat;
      switch(ev.cat){
      case EMU : tag_cat = "emu";   break;
      case MUMU: tag_cat = "mumu";  break;
      case EE  : tag_cat = "ee";    break;
      default  : continue;
      }
      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
      if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;


     //require compatibilitiy of the event with the PD
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
      }

      
      int eventSubCat  = eventCategoryInst.Get(phys,&phys.ajets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);

      //prepare the tag's vectors for histo filling
      std::vector<TString> tags_full;
      tags_full.push_back("all");
      tags_full.push_back(tag_cat);
     if(tag_subcat=="vbf") vbfFlag = true;


//if(vbfFlag == 1) {
//cout <<"vbf flag = " << vbfFlag << endl;
//cout << "********************************************************"<<endl;
//}

      //pileup and Higgs pT weight
      //float weight=ev.puWeight;
      //pileup weight
      float weight = 1.0;
      float noLShapeWeight=1.0;
      float signalWeight=1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      float lShapeWeights[4]={1.0,1.0,1.0,1.0};


      if(isMC){

	if(isMC_VBF){ signalWeight = weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );  weight*=signalWeight; }
//cout << " weight = " << weight << endl;
        if(isMC_GG) {
        for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) 
	 ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
	 weight *= ev.hptWeights[0];
//cout << "weight (ev.hptWeights[0]) = " << weight << endl;
        }
	for(size_t iwgt=0; iwgt<hLineShapeGrVec.size(); iwgt++)
	  lShapeWeights[iwgt]=hLineShapeGrVec[iwgt]->Eval(phys.genhiggs[0].mass());
	noLShapeWeight=weight;
	weight *= lShapeWeights[0];
//cout << "weight after line shape = " << weight << endl;
	
      }


//=================================================== Lepton Analysis ===================================================

      bool passIds(true);
      bool pass3dLeptonVeto(true);
      int nextraleptons(0);
      double llScaleFactor(1.0),llTriggerEfficiency(1.0);
      LorentzVector lep1=phys.leptons[0];
      float relIso1 = phys.leptons[0].relIsoRho(ev.rho);
//cout << "relIso1 = " << relIso1 << endl;
//cout << "particle id = " << phys.leptons[0].id << endl;
      int lpid=phys.leptons[0].pid;
      if(fabs(phys.leptons[0].id)==13)      passIds &= hasObjectId(ev.mn_idbits[lpid], MID_TIGHT);
      else if(fabs(phys.leptons[0].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_MEDIUM);
//      else if(fabs(phys.leptons[0].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_VBTF2011);



      LorentzVector lep2=phys.leptons[1];
      float relIso2 = phys.leptons[1].relIsoRho(ev.rho);
      lpid=phys.leptons[1].pid;
      if(fabs(phys.leptons[1].id)==13)      passIds &= hasObjectId(ev.mn_idbits[lpid], MID_TIGHT);
      else if(fabs(phys.leptons[1].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_MEDIUM); 
//      else if(fabs(phys.leptons[1].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_VBTF2011); 

if(fabs(phys.leptons[0].id)==13) 
{
if(!use2011Id)
   llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);
}

if(fabs(phys.leptons[0].id)==11)
{
if(!use2011Id)
 //  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);
  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);

}

if(fabs(phys.leptons[1].id)==13)  
{
if(!use2011Id)
   llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[1].pt(),fabs(phys.leptons[1].eta()),2012);
}

if(fabs(phys.leptons[1].id)==11)
{
if(!use2011Id)
 //  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);
  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[1].pt(),fabs(phys.leptons[1].eta()),2012);

}

//cout << " llTriggerEfficiency = " << llTriggerEfficiency <<  endl;


     LorentzVector zll=lep1+lep2;

// 3rd LEPTON ANALYSIS
	      //
	      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++){
		  //lepton type
		  bool isGood(false);
		  int lpid=phys.leptons[ilep].pid;
		  if(fabs(phys.leptons[ilep].id)==13){
		      if(!use2011Id){
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT) && phys.leptons[ilep].pt()>3);
		      }else{
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT2011) && phys.leptons[ilep].pt()>3);
		      }
	          }else{
		      if(!use2011Id){
			  isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
		      }else{
			  isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
		      }
		  }
		  nextraleptons += isGood;
		  
		  if(!isGood) continue;
	      }
	      
	      pass3dLeptonVeto=(nextraleptons==0);
      
      bool pass3rdLeptonVeto(ev.ln==0);

//cout << " pass3dLeptonVeto = " << pass3dLeptonVeto << endl;


//========================================= JET/MET ANALYSIS ===================================================================

      //MET variables
      LorentzVector rawMetP4=phys.met[2];
      LorentzVector fullTypeIMetP4=phys.met[0];
      LorentzVector typeIMetP4=rawMetP4;
      for(size_t ijet=0; ijet<phys.ajets.size();ijet++)
        {
          LorentzVector jetP4=phys.ajets[ijet];
          LorentzVector rawJetP4=jetP4*phys.ajets[ijet].rawCorrFact;
          typeIMetP4 -= (jetP4-rawJetP4);
        }
      LorentzVector mvaMetP4=phys.met[7];

//      std::vector<PhysicsObjectJetCollection> variedAJets;
  //    LorentzVectorCollection zvvs;


    std::vector<PhysicsObjectJetCollection> variedAJets;
     LorentzVectorCollection zvvs;
     METUtils::computeVariation(phys.ajets, phys.leptons, rawMetP4, variedAJets, zvvs, &jecUnc); //analyze the leptons

                  LorentzVector aClusteredMetP4(zll); aClusteredMetP4 *= -1;
      
      //analyze JET/MET
      LorentzVectorCollection jetsP4;
      std::vector<double> genJetsPt;
     int nbtags(0);
double btag_discrm = 0;
double tmp = -999.; 
      for(size_t ijet=0; ijet<phys.ajets.size(); ijet++)
	{
	  jetsP4.push_back( phys.ajets[ijet] );
	  genJetsPt.push_back( phys.ajets[ijet].genPt);
	  if(phys.ajets[ijet].pt()>30 && fabs(phys.ajets[ijet].eta())<2.5) 
{
//cout << "phys.ajets[ijet].btag3 = " << phys.ajets[ijet].btag3 << endl;
tmp = phys.ajets[ijet].btag3;
//cout << "phys.ajets[ijet].btag3 = " << tmp << endl;
if(tmp > btag_discrm) { btag_discrm = tmp ; } 
nbtags += (phys.ajets[ijet].btag3>0.275);
	} 
}

//cout << "Maximum value of B-tag = " << btag_discrm << endl;
//cout << "****************************************************************" <<endl; 
      bool passBveto(nbtags==0);
//      bool passBveto(nbtags==0);

//cout << " passBveto = " << passBveto << endl;

                METUtils::stRedMET aRedMetOut;
                LorentzVector aRedMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, aClusteredMetP4, zvvs[0],false,&aRedMetOut);
                double aRedMetL=aRedMetOut.redMET_l;
                double aRedMetT=aRedMetOut.redMET_t;
                double aMT=METUtils::transverseMass(zll,zvvs[0],true);
                double amvaMT=METUtils::transverseMass(zll,mvaMetP4,true);


//cout << "reduced MET = " << aRedMet.pt() << endl;
//cout << "Type 1 MET = " << zvvs[0].pt() << endl;
//cout << "RAW MET = " << rawMetP4.pt() << endl;
//cout << "MVA MET = " << mvaMetP4.pt() << endl;




if(mustBlind == 1) {cout << "mustBlind = " << mustBlind << endl;      }

//============================================================ FILLING OF BRANCHES===========================================================


//	if(passIds && pass3rdLeptonVeto && passBveto && mustBlind == 0) {
      if(passIds && pass3rdLeptonVeto &&  mustBlind == 0) {



hzz_RedMet = aRedMet.pt();
hzz_RedMetL = aRedMetL;
hzz_RedMetT = aRedMetT;
hzz_RawMet = rawMetP4.pt();
hzz_MVAMet = mvaMetP4.pt();
hzz_BTag = btag_discrm;
lineshape_weight = weight;
hzz_lept1_gIso = phys.leptons[0].gIso;
hzz_lept2_gIso = phys.leptons[1].gIso;
hzz_lept1_chIso = phys.leptons[0].chIso;
hzz_lept2_chIso = phys.leptons[1].chIso;
hzz_lept1_nhIso = phys.leptons[0].nhIso;
hzz_lept2_nhIso = phys.leptons[1].nhIso;
hzz_rho = ev.rho25;

	vbf_flag = vbfFlag;
	ev_num = ev.event;
	run_num = ev.run;
	lumi_num = ev.lumi;
      hzz_lept1_ID = fabs(phys.leptons[0].id);
      hzz_lept1_PT = lep1.pt();
      hzz_lept1_ETA = lep1.eta();
      hzz_lept1_PHI = lep1.phi();
      hzz_lept1_DetIso = phys.leptons[0].relIsoRho(ev.rho);
      hzz_lept1_PFIsoDbeta = phys.leptons[0].pfRelIsoDbeta();
      hzz_lept1_PFIso2012 = phys.leptons[0].ePFRelIsoCorrected2012(ev.rho);

      hzz_lept2_ID = fabs(phys.leptons[1].id);
      hzz_lept2_PT = lep2.pt();
      hzz_lept2_ETA = lep2.eta();
      hzz_lept2_PHI = lep2.phi();
      hzz_lept2_DetIso = phys.leptons[1].relIsoRho(ev.rho);
      hzz_lept2_PFIsoDbeta = phys.leptons[1].pfRelIsoDbeta();
      hzz_lept2_PFIso2012 = phys.leptons[1].ePFRelIsoCorrected2012(ev.rho);

      hzz_Z_Mass = zll.mass();
      hzz_Z_PT = zll.pt();
      hzz_Z_Eta = zll.eta();
      hzz_Z_Phi = zll.phi();
      PUWeight = weight;
      hzz_PFMET = zvvs[0].pt();
      hzz_MET_Phi = zvvs[0].phi();
//      hzz_TransMass_Higgs = mts[0];
      hzz_TransMass_Higgs = aMT;
      ev_category = ev.cat;

      //cout << " Z mass =  " << hzz_Z_Mass << endl;
      //cout << " PU Weight =  " << weight << endl;
      //cout << " PFMET = " << zvvs[0].pt() << endl;
      //cout << " lepton 1 PT =  " << lep1.pt() << "Particle ID = " << phys.leptons[0].pid << " lepton 1 PT =  " << phys.leptons[0].pt()<< endl;
      //cout << " Event category =  " << ev.cat << endl;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Jets Filling ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//PhysicsObjectJetCollection correctedJets = phys.ajets;
//cout << " Number of Jets = " << correctedJets.size() << endl;
vector<int>jetmarker;
jetmarker.clear();
double tmpPT = -1.0;
double sumet = 0.;
int njets = 0;
Float_t mindphijmet(9999.);
for(size_t corjet=0; corjet<phys.ajets.size(); corjet++) {
if(phys.ajets[corjet].pt() > 30. ) {
Float_t idphijmet=fabs(deltaPhi(hzz_MET_Phi,phys.ajets[corjet].phi()));
if(idphijmet<mindphijmet) mindphijmet=idphijmet;
//cout << " Delta Phi of Jet and MET = " << idphijmet << endl;
//if(correctedJets[corjet].pt() > 30. ) {
double jetpt = phys.ajets[corjet].pt();
double jetE = phys.ajets[corjet].energy();
sumet = jetpt+sumet;
if(jetpt > tmpPT) {tmpPT = jetpt; }
//cout << "PT of Jets = " << phys.ajets[corjet].pt() << endl;
njets++;
jetmarker.push_back(corjet);
} // PT requirement on Jets
        } // for loop on Jets
//cout<< "sumet = " << sumet <<endl;
//cout << " Number of Jets = " << hzz_N_Jets << endl;
//cout << "***************************************************************" <<endl;
hzz_Jet_PT = tmpPT;
hzz_N_Jets = njets;
//cout << " Leading Jet PT = " << hzz_Jet_PT << endl;
//cout << " Number of Jets = " << hzz_N_Jets << endl;
//hzz_dPhi_JetMet = mindphijmet;
hzz_dPhi_JetMet = mindphijmet;
hzz_Jet_SumPT = sumet;
//cout << " Minimum dPhi of jet and Met = " << hzz_dPhi_JetMet << endl;
//cout << "***********************************************************************************************************" << endl;
      hzz_tree->Fill();
}
  }
  
  printf("\n"); 
  file->Close();

  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
//  mon.Write();
hzz_tree->Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  
