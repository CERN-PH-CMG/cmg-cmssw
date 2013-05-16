#include <iostream>
#include <boost/shared_ptr.hpp>

#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"

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
#include "CMGTools/HtoZZ2l2nu/interface/EfficiencyMap.h"
#include "CMGTools/HtoZZ2l2nu/interface/LeptonEfficiencySF.h"

#include "CMGTools/HtoZZ2l2nu/src/MuScleFitCorrector_v4_1/MuScleFitCorrector.h"
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

    bool use2011Id = runProcess.getParameter<bool>("is2011");
    bool useCHS(true);
    bool useJERsmearing(true); 
    bool useJetsOnlyInTracker(false); 
    bool usePUsubJetId(true); 
    // invert ajets <-> jets ...
    // 2011:  ajets -> non CHS,  jets -> CHS
    // 2012:  ajets -> CHS,      jets -> non CHS
    if(use2011Id) useCHS = !useCHS; 
    cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

    bool isMC       = runProcess.getParameter<bool>("isMC");
    int mctruthmode = runProcess.getParameter<int>("mctruthmode");

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
    int evStart     = runProcess.getParameter<int>("evStart");
    int evEnd       = runProcess.getParameter<int>("evEnd");
    TString dirname = runProcess.getParameter<std::string>("dirName");

    //jet energy scale uncertainties
    TString uncFile = runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
    JetCorrectionUncertainty jecUnc(uncFile.Data());

    //systematics
    bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");
    TString varNames[]={"",
	  "_jerup","_jerdown",
	  "_jesup","_jesdown",
	  "_umetup","_umetdown",
	  "_lesup","_lesdown",
	  "_puup","_pudown",
	  "_btagup","_btagdown",
	  "_sherpaup","_sherpadown"};
    size_t nvarsToInclude(1);
    if(runSystematics)
    {
	  cout << "Systematics will be computed for this analysis" << endl;
	  nvarsToInclude=sizeof(varNames)/sizeof(TString);
    }

    // Muon scale/resolution corrections
    //TString fitParametersFile = "/afs/cern.ch/work/t/trocino/Work/ZZllnn/aTGC/Analysis/2012-11-16/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/src/MuScleFitCorrector_v3/"; 
    TString fitParametersFile = "/afs/cern.ch/work/l/lpernie/ZZllvv/16_Nov/CMSSW_5_3_3_patch2/src/CMGTools/HtoZZ2l2nu/src/MuScleFitCorrector_v4_1/"; 
    if(use2011Id) {
	  if(isMC) fitParametersFile += "MuScleFit_2011_MC_44X.txt";
	  else     fitParametersFile += "MuScleFit_2011_DATA_44X.txt";
    }
    else {
	  if(isMC) fitParametersFile += "MuScleFit_2012_MC_53X.txt"; // CHANGE!!!
	  else {
		if( url.Contains("2012D") ) fitParametersFile += "MuScleFit_2012D_DATA_53X.txt";
		else                        fitParametersFile += "MuScleFit_2012ABC_DATA_53X.txt";
	  }
    }
    MuScleFitCorrector *corrector_ = new MuScleFitCorrector(fitParametersFile);

    //##############################################
    //########    INITIATING HISTOGRAMS     ########
    //##############################################
    SmartSelectionMonitor mon;

    // pileup control
    mon.addHistogram( new TH1F( "nvtx",";Vertices;Events",50,0,50) ); 
    mon.addHistogram( new TH1F( "nvtx_dy",";Vertices;Events",50,0,50) ); 
    mon.addHistogram( new TH1F("npfjets_dy",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
    mon.addHistogram( new TH1F( "nvtxraw",";Vertices;Events",50,0,50) ); 
    mon.addHistogram( new TH1F( "rho",";#rho;Events",50,0,25) ); 
    mon.addHistogram( new TH1F( "rho25",";#rho(#eta<2.5);Events",50,0,25) ); 
    mon.addHistogram(  new TProfile("metvsrun"    ,      "Run number",     600, 190000,196000) ) ;
    mon.addHistogram(  new TProfile("metvsavginstlumi",  "Avg. inst lumi", 60,  400,1000));
    mon.addHistogram(  new TProfile("nvtxvsrun",         "Run number",     600, 190000,196000) ) ;
    mon.addHistogram(  new TProfile("nvtxvsavginstlumi", "Avg. inst lumi", 60,  400,1000));
    mon.addHistogram( new TH1F( "RunDep_Yields", ";Run;Events",4000,170000,210000) );
    mon.addHistogram( new TProfile( "RunDep_Met", ";Run;<Met>",4000,170000,210000) );

    TH1F* Hcutflow  = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;
    TH1F *h=(TH1F*) mon.addHistogram( new TH1F ("eventflow", ";;Events", 11,0,11) );
    h->GetXaxis()->SetBinLabel(1,"Trigger");
    h->GetXaxis()->SetBinLabel(2,"#geq 2 leptons");
    h->GetXaxis()->SetBinLabel(3,"#geq 2 iso leptons");
    h->GetXaxis()->SetBinLabel(4,"|M-M_{Z}|<7.5");
    h->GetXaxis()->SetBinLabel(5,"p_{T}^{ll}>45");
    h->GetXaxis()->SetBinLabel(6,"3^{rd}-lepton veto");
    h->GetXaxis()->SetBinLabel(7,"Jet veto"); 
    h->GetXaxis()->SetBinLabel(8,"b-veto"); 
    h->GetXaxis()->SetBinLabel(9,"#Delta #phi(jet,E_{T}^{miss})>0.5");
    h->GetXaxis()->SetBinLabel(10,"E_{T}^{miss}>65");
    h->GetXaxis()->SetBinLabel(11,"0.4<E_{T}^{miss}/p_{T}^{ll}<1.8");

    h=(TH1F*) mon.addHistogram( new TH1F ("eventflow_gamma", ";;Events", 4,0,4) );
    //h->GetXaxis()->SetBinLabel(1,"Trigger");
    //h->GetXaxis()->SetBinLabel(2,"#geq 2 leptons");
    //h->GetXaxis()->SetBinLabel(3,"#geq 2 iso leptons");
    //h->GetXaxis()->SetBinLabel(4,"|M-M_{Z}|<15");
    //h->GetXaxis()->SetBinLabel(5,"p_{T}^{ll}>55");
    //h->GetXaxis()->SetBinLabel(6,"3^{rd}-lepton veto");
    //h->GetXaxis()->SetBinLabel(7,"Jet veto"); 
    h->GetXaxis()->SetBinLabel(1,"b-veto"); 
    h->GetXaxis()->SetBinLabel(2,"#Delta #phi(jet,E_{T}^{miss})>0.5");
    h->GetXaxis()->SetBinLabel(3,"E_{T}^{miss}>65");
    h->GetXaxis()->SetBinLabel(4,"0.4<E_{T}^{miss}/p_{T}^{ll}<1.8");

    mon.addHistogram(new TH1F("DR_leptons_before_ee",           ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_before_mumu",           ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("Pt_leptons_before_ee",           ";Z PT;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_before_mumu",           ";Z PT;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH2F("Pt_vs_DR_before_ee",             ";Z PT ; DR",50,60.,400.,50,0.15,1.5) );
    mon.addHistogram(new TH2F("Pt_vs_DR_before_mumu",           ";Z PT ; DR",50,60.,400.,50,0.15,1.5) );
    mon.addHistogram(new TH1F("DR_leptons_after_ee",           ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_after_mumu",           ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_afterID_ee",           ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_afterID_mumu",           ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_afterTriIDISO_ee",      ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_afterTriIDISO_mumu",    ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("Pt_leptons_afterTriIDISO_ee",      ";Z PT lept;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_afterTriIDISO_mumu",    ";Z PT lept;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("DR_leptons_afterTri_ee",      ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("DR_leptons_afterTri_mumu",    ";DR lept;Efficiency",50,0.,1.) );
    mon.addHistogram(new TH1F("Pt_leptons_afterTri_ee",      ";Z PT lept;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_afterTri_mumu",    ";Z PT lept;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_after_ee",           ";Z PT;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_after_mumu",           ";Z PT;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_afterID_ee",           ";Z PT;Efficiency",25,0.,400.) );
    mon.addHistogram(new TH1F("Pt_leptons_afterID_mumu",           ";Z PT;Efficiency",25,0.,400.) );
    mon.addHistogram( new TH1F( "leadpt", ";p_{T}^{l};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "leadeta", ";#eta^{l};Events", 50,-2.6,2.6) );
    mon.addHistogram( new TH1F( "trailerpt", ";p_{T}^{l};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "trailereta", ";#eta^{l};Events", 50,-2.6,2.6) );

    Float_t qtaxis[100];
    for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5                                                                                                                                   
    for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195                                                                                                                                   
    for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340                                                                                                                                    
    for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976                                        
    mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
    mon.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 50,0,500) );
    //double zpt_bins[] = {0., 20., 40., 60., 80., 100., 150., 200., 300., 400., 600., 800.}; 
    double zpt_bins[] = {0., 20., 40., 60., 80., 100., 200., 400., 800.}; 
    const int n_zpt_bins = sizeof(zpt_bins)/sizeof(double) - 1; 
    double zpt_bins2[] = {0., 20., 40., 60., 80., 100., 200., 400.}; 
    const int n_zpt_bins2 = sizeof(zpt_bins2)/sizeof(double) - 1;
    double zpt_bins_lastBin[] = {200., 400., 800.}; 
    const int n_zpt_bins_lastBin = sizeof(zpt_bins_lastBin)/sizeof(double) - 1; 
    mon.addHistogram( new TH1F( "zpt_rebin", ";p_{T}^{ll};Events", n_zpt_bins, zpt_bins) );
    mon.addHistogram( new TH1F( "zpt_rebin2", ";p_{T}^{ll};Events", n_zpt_bins2, zpt_bins2) );
    mon.addHistogram( new TH1F( "zpt_dy", ";p_{T}^{ll};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "zpt_noJet", ";p_{T}^{ll};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "zptGen_noJet", ";p_{T}^{ll};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "zptNM1", ";p_{T}^{ll};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 50,-10,10) );
    mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,40,250) );
    mon.addHistogram( new TH1F( "zmassNM1", ";M^{ll};Events", 100,40,250) );

    // 3rd lepton control 
    mon.addHistogram( new TH1F( "thirdleptonpt", ";p_{T}^{l};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "thirdleptoneta", ";#eta^{l};Events", 50,-2.6,2.6) );
    mon.addHistogram( new TH1F( "nleptons", ";Leptons;Events", 3,2,4) );
    mon.addHistogram( new TH1F( "nleptons_final", ";Leptons;Events", 3,2,4) );
    mon.addHistogram( new TH1F( "nleptonsNM1", ";Leptons;Events", 3,2,4) );
    mon.addHistogram( new TH1F( "mt3" , ";M_{T}^{3rd lepton} [GeV/c^{2}];Events",20,0,200) );

    // background control
    mon.addHistogram( new TH1F( "Ctrl_Estimation_RedMet",";RedMet [GeV];Events",40,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_Estimation_Mass",";Mass [GeV];Events",40,45,250) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_RedMet",";RedMet [GeV];Events",100,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_zmass","Mass [GeV];Ctrl_WZ;Events",100,40,170) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_Mt","Trasv. Mass [GeV];Ctrl_WZ;Events",100,0.,130) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_Mt1","Trasv. Mass [GeV];Ctrl_WZ;Events",100,0.,130) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_Mt2","Trasv. Mass [GeV];Ctrl_WZ;Events",100,0.,130) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_Mt3","Trasv. Mass [GeV];Ctrl_WZ;Events",100,0.,130) );
    mon.addHistogram( new TH1F( "Ctrl_WZ_Mt4","Trasv. Mass [GeV];Ctrl_WZ;Events",100,0.,130) );
    mon.addHistogram( new TH1F( "Ctrl_T_zmass",";Mass [GeV];Events",50,40,300) );
    mon.addHistogram( new TH1F( "Ctrl_Tside_RedMet",";RedMet [GeV];Events",50,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet",";RedMet [GeV];Events",50,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet1",";RedMet [GeV];Events",50,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet2",";RedMet [GeV];Events",40,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet2b",";RedMet [GeV];Events",25,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet2c",";RedMet [GeV];Events",25,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet3",";RedMet [GeV];Events",25,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet4",";RedMet [GeV];Events",25,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet5",";RedMet [GeV];Events",25,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet5_reb",";RedMet [GeV];Events",20,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet5_reb2",";RedMet [GeV];Events",15,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_RedMet6",";RedMet [GeV];Events",30,0,300) );
    mon.addHistogram( new TH1F( "Ctrl_WW_zmass",";Mass [GeV];Events",30,0,200) );


    //jet control
    mon.addHistogram( new TH1F("pfjetpt"       , ";p_{T} [GeV/c];Events",50,0,250) );
    mon.addHistogram( new TH1F("pfjeteta"       , ";|#eta|;Events",25,0,5) );
    mon.addHistogram( new TH2F("npfjetsvspu",          ";Pileup interactions;Jet multiplicity (p_{T}>30 GeV/c);Events",50,0,50,5,0,5) );
    h=(TH1F *)mon.addHistogram( new TH1F("npfjets",  ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
    for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
	  TString label("");
	  if(ibin==h->GetXaxis()->GetNbins()) label +="#geq";
	  else                                label +="=";
	  label += (ibin-1);
	  h->GetXaxis()->SetBinLabel(ibin,label);
    } 
    mon.addHistogram( new TH1F("npfjets_pres",  ";Jet multiplicity after preselection (p_{T}>30 GeV/c);Events",5,0,5) );
    mon.addHistogram( new TH1F("npfjetsNM1",    ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
    mon.addHistogram( new TH1F("pfjetbeta"    , ";#beta;Events",50,0,1) );
    mon.addHistogram( new TH1F("pfjetmva"     , ";MVA;Events",50,-1,1) );
    mon.addHistogram( new TH1F("bpfjetstags",     ";b tags;Events",50,0,1) );
    mon.addHistogram( new TH1F("otherpfjetstags", ";udscg tags;Events",50,0,1) );
    mon.addHistogram( new TH1F("pfjetstags",         ";Discriminator;Events",50,0,1) );
    mon.addHistogram( new TH1F("npfjetsbtags",    ";b-tag multiplicity;Events",5,0,5) );
    mon.addHistogram( new TH1F("npfjetsbtagsNM1",    ";b-tag multiplicity;Events",5,0,5) );


    //MET control
    mon.addHistogram( new TH1F( "mindphilmet", ";min(#Delta#phi(lepton,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphilmet_aftPhijPt", ";min(#Delta#phi(lepton,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphilmet_aftPhij", ";min(#Delta#phi(lepton,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphilmet_aftMet", ";min(#Delta#phi(lepton,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "maxdphilmet", ";max(#Delta#phi(lepton,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphijmet_0", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphijmet_25", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphijmet_50", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphijmet_aftPhil", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1F( "mindphijmetNM1", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
    mon.addHistogram( new TH1D( "balancedif",    ";|E_{T}^{miss}-q_{T}|/q_{T};Events", 5,0,1.0) );
    mon.addHistogram( new TH1D( "balance",    ";E_{T}^{miss}/q_{T};Events", 25,0,5) );
    mon.addHistogram( new TH1D( "balanceNM1", ";E_{T}^{miss}/q_{T};Events", 25,0,5) );
    mon.addHistogram( new TH2D( "met_mindphilmet"  , ";E_{T}^{miss};min(#Delta#phi(lepton,E_{T}^{miss});Events", 50,0,250,40,0,4) );
    mon.addHistogram( new TH2D( "metoverlpt_mindphilmet"  , ";E_{T}^{miss}/p_{T}^{lepton};min(#Delta#phi(lepton,E_{T}^{miss});Events", 50,0,2,40,0,4) );
    mon.addHistogram( new TH1F( "met_metSB",        ";E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_met",          ";E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_metL",          ";Axial-E_{T}^{miss};Events", 50,-50,150) );
    mon.addHistogram( new TH1F( "met_metCompL",          ";Long. PF E_{T}^{miss};Events", 75,-250,500) );
    mon.addHistogram( new TH1F( "met_metCompT",          ";Trans. PF E_{T}^{miss};Events", 75,-250,500) );
    mon.addHistogram( new TH1F( "met_metNM1",          ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetNM1",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetD0NM1",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    //met studies
    mon.addHistogram( new TH1F( "met_metNM1_JESUp",          ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetNM1_JESUp",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetD0NM1_JESUp",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_metNM1_JESDown",          ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetNM1_JESDown",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetD0NM1_JESDown",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_metNM1_PUUp",          ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetNM1_PUUp",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetD0NM1_PUUp",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_metNM1_PUDown",          ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetNM1_PUDown",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetD0NM1_PUDown",       ";Reduced E_{T}^{miss};Events", 50,0,500) );

    mon.addHistogram( new TH2F( "metvsmt",          ";E_{T}^{miss};M_{T};Events", 100,0,500,200,0,1000) );
    mon.addHistogram( new TH2F( "typeImetvstypeImt",          ";type I E_{T}^{miss};type I M_{T};Events", 100,0,500,200,0,1000) );
    mon.addHistogram( new TH1F( "met_metbeforedphilmet",          ";E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_met_blind",    ";E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_mvamet",       ";MVA E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_mvamet_blind",       ";MVA E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_typeImet",     ";Type I E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_typeImet_blind",     ";Type I E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_fulltypeImet", ";Type I E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_fulltypeImetbeforedphilmet", ";Type I E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_fulltypeImet_blind", ";Type I E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMet",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMet_ctrl",       ";Reduced E_{T}^{miss};Events", 50,0,200) );
    mon.addHistogram( new TH1F( "met_redMet_ctrl_final",       ";Reduced E_{T}^{miss};Events", 50,0,200) );
    //double redMet_bins[] = {0., 20., 40., 60., 80., 100., 200., 400., 800.}; 
    double redMet_bins[] = {0., 20., 40., 60., 80., 100., 150., 400., 800.}; 
    const int n_redMet_bins = sizeof(redMet_bins)/sizeof(double) - 1; 
    double redMet_bins2[] = {0., 20., 40., 60., 80., 100., 150., 400.}; 
    const int n_redMet_bins2 = sizeof(redMet_bins2)/sizeof(double) - 1; 
    mon.addHistogram( new TH1F( "met_redMet_rebin",       ";Reduced E_{T}^{miss};Events", n_redMet_bins,redMet_bins) );
    mon.addHistogram( new TH1F( "met_redMet_rebin2",       ";Reduced E_{T}^{miss};Events", n_redMet_bins2,redMet_bins2) );
    mon.addHistogram( new TH1F( "met_redMet_phi_dy",       ";Reduced E_{T}^{miss} #phi;Events", 50,-6.,6.) );
    mon.addHistogram( new TH1F( "met_redMet_blind",       ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetL",      ";Long. Reduced E_{T}^{miss};Events", 75,-250,500) );
    mon.addHistogram( new TH1F( "met_redMetT",      ";Trans. Reduced E_{T}^{miss};Events", 75,-250,500) );
    mon.addHistogram( new TH1F( "met_clusteredMetL",";Long. clustered E_{T}^{miss};Events", 75,-250,500) );
    mon.addHistogram( new TH1F( "met_clusteredMetT",";Trans. clustered E_{T}^{miss};Events", 75,-250,500) );
    mon.addHistogram( new TH1F( "met_met3leptons",  ";E_{T}^{miss};Events", 50,0,500) );
    //mon.addHistogram( new TH2F( "met_met_vspu",       ";Pileup events; E_{T}^{miss};Events", 50,0,50,50,0,250) );
    //mon.addHistogram( new TH2F( "met_mvamet_vspu",    ";Pileup events; MVA E_{T}^{miss};Events", 50,0,50,50,0,250) );
    //mon.addHistogram( new TH2F( "met_redMet_vspu",    ";Pileup events; Reduced E_{T}^{miss};Events", 50,0,50,50,0,250) );
    //mon.addHistogram( new TH2F( "met_typeImet_vspu",  ";Pileup events; Type I E_{T}^{miss};Events", 50,0,50,50,0,250) );
    //mon.addHistogram( new TH2F( "met_redMetCHS_vspu", ";Pileup events; Type I E_{T}^{miss};Events", 50,0,50,50,0,250) );
    mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
    mon.addHistogram( new TH1F( "mtNM1"  , ";M_{T};Events", 100,0,1000) );
    mon.addHistogram( new TH1F( "mt_blind"  , ";M_{T};Events", 100,0,1000) );
    mon.addHistogram( new TH1F( "typeImt"  , ";M_{T};Events", 100,0,1000) );

    // Final distributions
    mon.addHistogram( new TH1F( "mt_final"  , ";M_{T};Events", 25,0,1000) );
    mon.addHistogram( new TH1F( "zpt_final", ";p_{T}^{ll};Events", 25,0,500) );
    mon.addHistogram( new TH1F( "zpt_rebin_final", ";p_{T}^{ll};Events", n_zpt_bins, zpt_bins) );
    mon.addHistogram( new TH1F( "zpt_rebin2_final", ";p_{T}^{ll};Events", n_zpt_bins2, zpt_bins2) );
    mon.addHistogram( new TH1F( "zpt_rebin_final_lastBin", ";p_{T}^{ll};Events", n_zpt_bins_lastBin, zpt_bins_lastBin) );
    mon.addHistogram( new TH1F( "met_met_final"  , ";E_{T}^{miss};Events", 25,0,250) );
    mon.addHistogram( new TH1F( "met_redMet_final"  , ";Reduced E_{T}^{miss};Events", 50,0,500) );
    mon.addHistogram( new TH1F( "met_redMetL_final"  , ";Longitudinal Reduced E_{T}^{miss};Events", 25,-100,400) );
    mon.addHistogram( new TH1F( "met_redMet_rebin_final", ";Reduced E_{T}^{miss};Events", n_redMet_bins,redMet_bins) );
    mon.addHistogram( new TH1F( "met_redMet_rebin2_final", ";Reduced E_{T}^{miss};Events", n_redMet_bins2,redMet_bins2) );

    //optimization
    std::vector<double> optim_Cuts1_met; 
    std::vector<double> optim_Cuts1_zpt;
    std::vector<double> optim_Cuts1_zmass;
    //std::vector<double> optim_Cuts1_jetthr;
    for(double met=50;met<140;met+=5.0) {
	  if(met>80 && int(met)%10!=0) continue;
	  for(double pt=30;pt<100;pt+=5) {
		if(pt>60 && int(pt)%10!=0) continue;
		for(double zm=5;zm<20;zm+=2.5) {
		    if(zm>10 && int(2*zm)%5!=0) continue;
		    //for(double jetpt=10; jetpt<20; jetpt+=5) {
		    optim_Cuts1_met   .push_back(met);
		    optim_Cuts1_zpt   .push_back(pt);
		    optim_Cuts1_zmass .push_back(zm);
		    //optim_Cuts1_jetthr.push_back(jetpt);
		    //}
		}
	  }
    }

    //make it as a TProfile so hadd does not change the value
    TProfile* Hoptim_cuts1_met    =  (TProfile*) mon.addHistogram( new TProfile ("optim_cut1_met",    ";cut index;met",     optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
    TProfile* Hoptim_cuts1_zpt    =  (TProfile*) mon.addHistogram( new TProfile ("optim_cut1_zpt",    ";cut index;zpt",     optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
    TProfile* Hoptim_cuts1_zmass  =  (TProfile*) mon.addHistogram( new TProfile ("optim_cut1_zm",     ";cut index;zmass",   optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
    for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
	  Hoptim_cuts1_met   ->Fill(index, optim_Cuts1_met[index]);    
	  Hoptim_cuts1_zpt   ->Fill(index, optim_Cuts1_zpt[index]);
	  Hoptim_cuts1_zmass ->Fill(index, optim_Cuts1_zmass[index]);
    }

    TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;

    for(size_t ivar=0; ivar<nvarsToInclude; ivar++)
    {
	  Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);

	  mon.addHistogram( new TH1F( "zptvar"+varNames[ivar], ";p_{T}^{ll} [GeV/c];Events", 100,0,1000) );

	  Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);

	  // for ZZ analysis (use Zpt or MET shape)
	  mon.addHistogram( new TH2F (TString("redMet_shapes")+varNames[ivar],";cut index;red-MET [GeV];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
	  mon.addHistogram( new TH2F (TString("redMet_rebin_shapes")+varNames[ivar],";cut index;red-MET [GeV];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), n_redMet_bins,redMet_bins) );
	  mon.addHistogram( new TH2F (TString("zpt_shapes")+varNames[ivar],";cut index;Z p_{T} [GeV/c];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), 160,0,800) );
	  mon.addHistogram( new TH2F (TString("zpt_rebin_shapes")+varNames[ivar],";cut index;Z p_{T} [GeV/c];#events (/5GeV)",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(), n_zpt_bins, zpt_bins) );

	  TH2F *h2=(TH2F *) mon.addHistogram( new TH2F ("redMet_shapes_NRBctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
	  h2->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");

	  h2=(TH2F *) mon.addHistogram( new TH2F ("redMet_rebin_shapes_NRBctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
	  h2->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");

	  h2=(TH2F *) mon.addHistogram( new TH2F ("zpt_shapes_NRBctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
	  h2->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");

	  h2=(TH2F *) mon.addHistogram( new TH2F ("zpt_rebin_shapes_NRBctrl"+varNames[ivar],";cut index;Selection region;Events",optim_Cuts1_met.size(),0,optim_Cuts1_met.size(),6,0,6) );
	  h2->GetYaxis()->SetBinLabel(1,"M_{in}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(2,"M_{out}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(3,"M_{out+}^{ll}/=0 b-tags");
	  h2->GetYaxis()->SetBinLabel(4,"M_{in}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(5,"M_{out}^{ll}/#geq 1 b-tag");
	  h2->GetYaxis()->SetBinLabel(6,"M_{out+}^{ll}/#geq 1 b-tag");
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
    if(isMC){
	  LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
	  PuShifters=getPUshifters(dataPileupDistribution,0.05);
    }

    //event Categorizer
    EventCategory eventCategoryInst(4); //jet(0,>=1)+vbf binning
    LeptonEfficiencySF lsf(use2011Id ? 2011:2012); 

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

cout<<"START_0_0 "<<ev.cat<<endl;
	  //event category
	  bool isSameFlavor(ev.cat==MUMU || ev.cat==EE);
	  TString tag_cat;
	  switch(ev.cat){
		case MUMU : tag_cat = "mumu";  break;
		case EE   : tag_cat = "ee";    break;
		case EMU  : tag_cat = "emu";   break;
		default   : continue;
	  }

cout<<"START_0_1 "<<endl;
	  //      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) && !isZZ2l2nu(ev.mccat) ) continue;
	  if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
	  if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;

cout<<"START_0_2 "<<endl;
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
		if(ev.cat==EE   && hasEEtrigger) hasTrigger=true;
		if(ev.cat==MUMU && (hasMMtrigger || hasMtrigger) ) hasTrigger=true;
		if(ev.cat==EMU  && hasEMtrigger) hasTrigger=true;

		if(use2011Id) hasTrigger = true; 
	  }

cout<<"START_1"<<endl;
	  //prepare the tag's vectors for histo filling
	  std::vector<TString> tags(1,"all");

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

	  //MET variables
	  LorentzVector rawMetP4=phys.met[2];
	  LorentzVector fullTypeIMetP4=phys.met[0];
	  LorentzVector mvaMetP4=phys.met[7];

	  //apply JER base corrections to jets (and compute associated variations on the MET variable)
	  // std PF
	  std::vector<PhysicsObjectJetCollection> variedAJets;
	  LorentzVectorCollection zvvs;
	  PhysicsObjectJetCollection &recoJets = ( useCHS ? phys.ajets : phys.jets);
	  METUtils::computeVariation(recoJets, phys.leptons, rawMetP4, variedAJets, zvvs, &jecUnc);
	  if(!useJERsmearing) zvvs[0] = rawMetP4; // this stinks a bit...

	  //
	  // LEPTON ANALYSIS
	  //
	  LorentzVector lep1=phys.leptons[0];
	  LorentzVector lep2=phys.leptons[1];
	  if(fabs(phys.leptons[0].id)==13) {
		TLorentzVector tmpLep1(lep1.Px(), lep1.Py(), lep1.Pz(), lep1.E()); 
		int tmpCh1 = phys.leptons[0].id < 0.0 ? -1 : 1; // "id" has same sign as charge here (opposite to "genid")
		corrector_->applyPtCorrection(tmpLep1, tmpCh1);
		if( isMC /*&& (!use2011Id)*/ ) corrector_->applyPtSmearing(tmpLep1, tmpCh1); 
		lep1 = LorentzVector(tmpLep1.Px(), tmpLep1.Py(), tmpLep1.Pz(), tmpLep1.E()); 
	  }
	  if(fabs(phys.leptons[1].id)==13) {
		TLorentzVector tmpLep2(lep2.Px(), lep2.Py(), lep2.Pz(), lep2.E()); 
		int tmpCh2 = phys.leptons[0].id < 0.0 ? -1 : 1; // "id" has same sign as charge here (opposite to "genid")
		corrector_->applyPtCorrection(tmpLep2, tmpCh2);
		if( isMC /*&& (!use2011Id)*/ ) corrector_->applyPtSmearing(tmpLep2, tmpCh2); 
		lep2 = LorentzVector(tmpLep2.Px(), tmpLep2.Py(), tmpLep2.Pz(), tmpLep2.E()); 
	  }

	  LorentzVector zll(lep1+lep2);
	  if( fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 ) mon.fillHisto("DR_leptons_before_ee",tags,deltaR(lep1,lep2),weight);
	  if( fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 ) mon.fillHisto("DR_leptons_before_mumu",tags,deltaR(lep1,lep2),weight);
	  if( fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 ) mon.fillHisto("Pt_leptons_before_ee",tags,(lep1+lep2).Pt(),weight);
	  if( fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 ) mon.fillHisto("Pt_leptons_before_mumu",tags,(lep1+lep2).Pt(),weight);
	  if( fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 ) mon.fillHisto("Pt_vs_DR_before_ee",tags,(lep1+lep2).Pt(),deltaR(lep1,lep2),weight);
	  if( fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 ) mon.fillHisto("Pt_vs_DR_before_mumu",tags,(lep1+lep2).Pt(),deltaR(lep1,lep2),weight);
	  bool passId(true);
	  bool passIdAndIso(true);
	  bool passZmass(fabs(zll.mass()-91)<7.5);
	  bool isZsideBand( (zll.mass()>40 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200));
	  bool isZsideBandPlus( (zll.mass()>110 && zll.mass()<200));
	  bool passZpt(zll.pt()>40);

	  //check alternative selections for the dilepton
	  double llScaleFactor(1.0),llTriggerEfficiency(1.0);
	  LorentzVector genZP4(0,0,0,0); // for checks on Sherpa ZZ 
	  int genmatchid[2] = {-1, -1}; 
	  double genmatchdr[2] = {0.1, 0.1}; 
	  for(int ilep=0; ilep<2; ilep++){
		TString lepStr( fabs(phys.leptons[ilep].id)==13 ? "mu" : "e");

		//generator level matching
		//  int matchid(0);
		//LorentzVector genP4(0,0,0,0);
		for(size_t igl=0;igl<phys.genleptons.size(); igl++) 
		{
		    //if(deltaR(phys.genleptons[igl],phys.leptons[ilep])>0.1) continue;
		    if(ilep==1 && int(igl)==genmatchid[0]) continue; 
		    if(deltaR(phys.genleptons[igl],phys.leptons[ilep])<genmatchdr[ilep]) {
			  genmatchdr[ilep] = deltaR(phys.genleptons[igl],phys.leptons[ilep]);
			  genmatchid[ilep] = igl; 
		    }
		}
		if(genmatchid[0]>-1 && genmatchid[1]>-1) {
		    //genP4=phys.genleptons[igl];
		    //matchid=phys.genleptons[igl].id;
		    genZP4 = phys.genleptons[0] + phys.genleptons[1];
		}

		//id and isolation
		int lpid=phys.leptons[ilep].pid;
		float relIso2011    = phys.leptons[ilep].relIsoRho(ev.rho);
		float relIso = (lepStr=="mu") ? 
		    phys.leptons[ilep].pfRelIsoDbeta() :
		    phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho);
		std::vector<int> passIds;
		std::map<int,bool> passIsos;
		bool hasGoodId(false), isIso(false);
		if(fabs(phys.leptons[ilep].id)==13)
		{
		    if( hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) )    { passIds.push_back(0); passIsos[0]=(relIso<0.2); }
		    if( hasObjectId(ev.mn_idbits[lpid], MID_TIGHT) )    { passIds.push_back(1); passIsos[1]=(relIso<0.2); if(!use2011Id) { hasGoodId=true; isIso=passIsos[0]; } }
		    if( hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) ) { passIds.push_back(2); passIsos[2]=(relIso2011<0.15); if(use2011Id) {hasGoodId=true; isIso=passIsos[2];} }
		    if( hasObjectId(ev.mn_idbits[lpid], MID_SOFT) )     { passIds.push_back(3); passIsos[3]=true;}
		    llScaleFactor       *= lsf.getLeptonEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),13).first;

		    if(use2011Id) 
		    {
			  try{
				//llScaleFactor *= muonScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
				llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
			  }catch(std::string &e){
			  }
		    }
		    else
		    {
			  //llScaleFactor *= 1;
			  llTriggerEfficiency *= 1.0; //muonTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
		    }
		}
		else
		{
		    int wps[]={EgammaCutBasedEleId::LOOSE,EgammaCutBasedEleId::MEDIUM, EID_VBTF2011, EgammaCutBasedEleId::VETO};
		    llScaleFactor       *= lsf.getLeptonEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),11).first;
		    for(int iwp=0; iwp<4; iwp++)
		    {
			  if(iwp==2 && hasObjectId(ev.en_idbits[lpid], EID_VBTF2011)) 
			  { 
				passIds.push_back(2); passIsos[2]=(relIso2011<0.10); 
				if(use2011Id) 
				{ 
				    hasGoodId=true; isIso=passIsos[2]; 
				    try{
					  //llScaleFactor *= electronScaleFactor(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
					  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()));
				    } catch(std::string &e){
				    }
				}
			  }
			  else
			  {
				bool passWp = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::WorkingPoint(wps[iwp]),
					  (fabs(phys.leptons[ilep].eta())<1.4442),
					  phys.leptons[ilep].pt(), phys.leptons[ilep].eta(),
					  ev.en_detain[lpid],  ev.en_dphiin[lpid], ev.en_sihih[lpid], ev.en_hoe[lpid],
					  ev.en_ooemoop[lpid], phys.leptons[ilep].d0, phys.leptons[ilep].dZ,
					  0., 0., 0.,
					  !hasObjectId(ev.en_idbits[lpid], EID_CONVERSIONVETO),0,ev.rho);
				if(passWp) { 
				    passIds.push_back(iwp); 
				    passIsos[iwp]=(relIso<0.15);
				    if(wps[iwp]==EgammaCutBasedEleId::MEDIUM && !use2011Id){  hasGoodId=true; isIso=passIsos[iwp]; }
				}
				if(!use2011Id)
				{
				    //llScaleFactor *= 1;
				    llTriggerEfficiency *= 1.0; //electronTriggerEfficiency(phys.leptons[ilep].pt(),fabs(phys.leptons[ilep].eta()),2012);
				}
			  }
		    }
		}
		if(!hasGoodId)  { passId=false; passIdAndIso=false; }
		if(!isIso) passIdAndIso=false;     

		//        fill control histograms (constrained to the Z mass)
		if(passZmass && isSameFlavor){
		    if(hasGoodId){
			  mon.fillHisto(lepStr+"reliso",     tags, use2011Id? relIso2011 : relIso,   weight);
		    }
		}
	  }
	  cout<<"START"<<endl;
	  if( hasTrigger ) cout<<"hasTrigger"<<endl;
	  if(passIdAndIso) cout<<"passIdAndIso"<<endl;
	  if( passIdAndIso && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("DR_leptons_after_ee", tags,deltaR(lep1,lep2),weight);
	  if( passIdAndIso && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("DR_leptons_after_mumu",tags,deltaR(lep1,lep2),weight);
	  if( hasTrigger && passIdAndIso && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("DR_leptons_afterTriIDISO_ee", tags,deltaR(lep1,lep2),weight);
	  if( hasTrigger && passIdAndIso && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("DR_leptons_afterTriIDISO_mumu",tags,deltaR(lep1,lep2),weight);
	  if( hasTrigger && passIdAndIso && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("Pt_leptons_afterTriIDISO_ee", tags,(lep1+lep2).Pt(),weight);
	  if( hasTrigger && passIdAndIso && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("Pt_leptons_afterTriIDISO_mumu",tags,(lep1+lep2).Pt(),weight);
	  if( hasTrigger && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("DR_leptons_afterTri_ee", tags,deltaR(lep1,lep2),weight);
	  if( hasTrigger && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("DR_leptons_afterTri_mumu",tags,deltaR(lep1,lep2),weight);
	  if( hasTrigger && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("Pt_leptons_afterTri_ee", tags,(lep1+lep2).Pt(),weight);
	  if( hasTrigger && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("Pt_leptons_afterTri_mumu",tags,(lep1+lep2).Pt(),weight);
	  if( passId && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("DR_leptons_afterID_ee", tags,deltaR(lep1,lep2),weight);
	  if( passId && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("DR_leptons_afterID_mumu",tags,deltaR(lep1,lep2),weight);
	  if( passIdAndIso && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("Pt_leptons_after_ee", tags,(lep1+lep2).Pt(),weight);
	  if( passIdAndIso && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("Pt_leptons_after_mumu",tags,(lep1+lep2).Pt(),weight);
	  if( passId && fabs(phys.leptons[0].id)==11 && fabs(phys.leptons[1].id)==11 )  mon.fillHisto("Pt_leptons_afterID_ee", tags,(lep1+lep2).Pt(),weight);
	  if( passId && fabs(phys.leptons[0].id)==13 && fabs(phys.leptons[1].id)==13 )  mon.fillHisto("Pt_leptons_afterID_mumu",tags,(lep1+lep2).Pt(),weight);
	  tags.push_back(tag_cat);
	  if(tag_cat=="mumu" || tag_cat=="ee") tags.push_back("ll"); 


	  //
	  // 3rd LEPTON ANALYSIS
	  //
	  bool pass3dLeptonVeto(true);
	  int nextraleptons(0);
	  std::vector<LorentzVector> extraLeptonsP4;
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
		extraLeptonsP4.push_back( phys.leptons[ilep] );
	  }
	  pass3dLeptonVeto=(nextraleptons==0);


	  //
	  //STD PF JET ANALYSIS
	  //
	  bool passJetveto(true);
	  bool passBveto(true);
	  bool passRedMet(true);
	  bool passDphijmet(true);
	  bool passBalanceCut(true);
	  PhysicsObjectJetCollection &aJets = ( useJERsmearing ? variedAJets[0] : recoJets );
	  PhysicsObjectJetCollection aGoodIdJets;
	  LorentzVector aClusteredMetP4(zll); aClusteredMetP4 *= -1;
	  int nABtags(0),nAJetsGood30(0),nAJetsGood15(0);
	  float mindphijmet(999999.),mindphijmet15(999999.);
	  for(size_t ijet=0; ijet<aJets.size(); ijet++) 
	  {
		if(aJets[ijet].pt()<15) continue;
		nAJetsGood15++;

		float idphijmet( fabs(deltaPhi(aJets[ijet].phi(),zvvs[0].phi()) ) );
		if(aJets[ijet].pt()>15) if(idphijmet<mindphijmet15)  mindphijmet15=idphijmet;
		if(aJets[ijet].pt()>30) if(idphijmet<mindphijmet)  mindphijmet=idphijmet;

		bool isGoodJet = hasObjectId(aJets[ijet].pid,JETID_LOOSE); 
		if(usePUsubJetId) isGoodJet = hasObjectId(aJets[ijet].pid,JETID_CUTBASED_LOOSE); 
		mon.fillHisto("pfjetbeta",     tags,aJets[ijet].beta,     weight);
		mon.fillHisto("pfjetmva",      tags,aJets[ijet].pumva,    weight);
		if(!isGoodJet) continue;


		aClusteredMetP4 -= aJets[ijet];	  

		if(useJetsOnlyInTracker && fabs(aJets[ijet].eta())>2.5) continue; 

		aGoodIdJets.push_back(aJets[ijet]);
		if(aJets[ijet].pt()>30)nAJetsGood30++;

		if(aJets[ijet].pt()>20 && fabs(aJets[ijet].eta())<2.5)  nABtags += (aJets[ijet].btag2>0.244);
	  }
	  passJetveto=(nAJetsGood30==0);
	  passBveto=(nABtags==0); 
	  passDphijmet=(mindphijmet>0.5);
	  if(nAJetsGood30==0) passDphijmet=(mindphijmet15>0.5);
	  passBalanceCut=(zvvs[0].pt()/zll.pt()>0.4 && zvvs[0].pt()/zll.pt()<1.8);


	  //ad-hoc cut for obvious correlations between MET and a lepton
	  double dphil1met=fabs(deltaPhi(lep1.phi(),zvvs[0].phi()));
	  double dphil2met=fabs(deltaPhi(lep2.phi(),zvvs[0].phi()));
	  bool passLMetVeto(true);
	  if(!use2011Id && zvvs[0].pt()>60 && min(dphil1met,dphil2met)<0.2) passLMetVeto=false;

	  //other mets
	  METUtils::stRedMET aRedMetOut;
	  LorentzVector null(0,0,0,0);
	  LorentzVector aRedMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, zll, 0, null, 0, aClusteredMetP4, zvvs[0], true, &aRedMetOut);
	  LorentzVector d0RedMet=METUtils::redMET(METUtils::D0, zll, 0, null, 0, aClusteredMetP4, zvvs[0], true, &aRedMetOut);
	  double aRedMetL=aRedMetOut.redMET_l;
	  double aRedMetT=aRedMetOut.redMET_t;
	  TVector2 aClusteredMet2(aClusteredMetP4.px(),aClusteredMetP4.py());
	  double clusteredMetL=aRedMetOut.a_l*aClusteredMet2;
	  double clusteredMetT=aRedMetOut.a_t*aClusteredMet2;
	  passRedMet=(aRedMet.pt()>65);

	  //transverse masses
	  double aMT=METUtils::transverseMass(zll,zvvs[0],true);
	  double balanceDif=fabs(zvvs[0].pt()-zll.pt())/zll.pt();
	  TVector2 dil2(zll.px(),zll.py());
	  TVector2 met2(zvvs[0].px(),zvvs[0].py());
	  double axialMet=dil2*met2; axialMet /= -zll.pt();    
	  double pfMetCompL = aRedMetOut.a_l*met2; 
	  double pfMetCompT = aRedMetOut.a_t*met2; 

	  //
	  //RUN PRESELECTION AND CONTROL PLOTS
	  //
	  if(isMC && use2011Id) weight *= llScaleFactor*llTriggerEfficiency;
	  if(hasTrigger)                 {mon.fillHisto("eventflow",tags,0,weight);}
	  if(hasTrigger && passId)       {mon.fillHisto("eventflow",tags,1,weight);}
	  if(hasTrigger && passIdAndIso) {mon.fillHisto("eventflow",tags,2,weight);}
	  else continue;

	  // Control Regions
	  bool estimated_Et  ( passZpt && passDphijmet && pass3dLeptonVeto && passBalanceCut && isZsideBand && !passBveto);
	  bool estimated_m  ( passZpt && passDphijmet && pass3dLeptonVeto && passBalanceCut && passRedMet && !passBveto);
	  bool WZ_ctrl_pt ( passZpt && nextraleptons==1 && passZmass && passBveto );
	  bool WZ_ctrl_m  ( passZpt && nextraleptons==1 && passBveto );
	  bool T_ctrl     ( passZpt && nABtags>0 && isZsideBand );
	  bool T_ctrl_side( passZpt && nABtags>0 );
	  bool WW_ctrl    ( passZpt && passBveto && (nAJetsGood15==0) && passBalanceCut && passDphijmet && pass3dLeptonVeto && isZsideBand );
	  bool WW_ctrl_1  ( passZpt  );
	  bool WW_ctrl_2  ( passZpt && passBveto );
	  bool WW_ctrl_2b ( passZpt && passBveto && (nAJetsGood15==0) );
	  bool WW_ctrl_2c ( passZpt  && passBveto && (nAJetsGood15==0) && passDphijmet );
	  bool WW_ctrl_3  ( passZpt  && passBveto && passBalanceCut && (nAJetsGood15==0) );
	  bool WW_ctrl_4  ( passZpt  && passBveto && passBalanceCut && (nAJetsGood15==0) && passDphijmet);
	  bool WW_ctrl_5  ( passZpt && passBveto && passBalanceCut && (nAJetsGood15==0) && passDphijmet && pass3dLeptonVeto);
	  bool WW_ctrl_m  ( passZpt && passRedMet  && passBveto && passBalanceCut && (nAJetsGood15==0) && passDphijmet && pass3dLeptonVeto);
	  if(estimated_Et)  mon.fillHisto("Ctrl_Estimation_RedMet",tags,aRedMet.pt(),weight);
	  if(estimated_m)  mon.fillHisto("Ctrl_Estimation_Mass",tags,zll.mass(),weight);
	  if(WZ_ctrl_pt)  mon.fillHisto("Ctrl_WZ_RedMet",tags,aRedMet.pt(),weight);
	  if(WZ_ctrl_m){
		mon.fillHisto("Ctrl_WZ_zmass",tags,zll.mass(),weight);
		if(passZmass) mon.fillHisto("Ctrl_WZ_Mt", tags, sqrt((2*zvvs[0].pt()*extraLeptonsP4[0].pt())*(1-cos(deltaPhi(zvvs[0].phi(),extraLeptonsP4[0].phi())))), weight);
		if(passZmass) mon.fillHisto("Ctrl_WZ_Mt1", tags, sqrt((2*zvvs[0].pt()*extraLeptonsP4[0].pt())*(1-cos(deltaPhi(zvvs[0].phi(),extraLeptonsP4[0].phi())))), weight);
	  }
	  if(passZmass && passRedMet && nextraleptons==1) mon.fillHisto("Ctrl_WZ_Mt2", tags, sqrt((2*zvvs[0].pt()*extraLeptonsP4[0].pt())*(1-cos(deltaPhi(zvvs[0].phi(),extraLeptonsP4[0].phi())))), weight);
	  if(passZmass  && aRedMet.pt()>50 && nextraleptons==1) mon.fillHisto("Ctrl_WZ_Mt3", tags, sqrt((2*zvvs[0].pt()*extraLeptonsP4[0].pt())*(1-cos(deltaPhi(zvvs[0].phi(),extraLeptonsP4[0].phi())))), weight);
	  if(passZmass  && aRedMet.pt()>70 && nextraleptons==1) mon.fillHisto("Ctrl_WZ_Mt4", tags, sqrt((2*zvvs[0].pt()*extraLeptonsP4[0].pt())*(1-cos(deltaPhi(zvvs[0].phi(),extraLeptonsP4[0].phi())))), weight);

	  if(T_ctrl)      mon.fillHisto("Ctrl_T_zmass",tags,zll.mass(),weight);
	  if(T_ctrl_side) mon.fillHisto("Ctrl_Tside_RedMet",tags,aRedMet.pt(),weight);
	  if(WW_ctrl)     mon.fillHisto("Ctrl_WW_RedMet",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_1)   mon.fillHisto("Ctrl_WW_RedMet1",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_2)   mon.fillHisto("Ctrl_WW_RedMet2",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_2b)  mon.fillHisto("Ctrl_WW_RedMet2b",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_2c)  mon.fillHisto("Ctrl_WW_RedMet2c",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_3)   mon.fillHisto("Ctrl_WW_RedMet3",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_4)   mon.fillHisto("Ctrl_WW_RedMet4",tags,aRedMet.pt(),weight);
	  if(WW_ctrl_5){  mon.fillHisto("Ctrl_WW_RedMet5",tags,aRedMet.pt(),weight);
		mon.fillHisto("Ctrl_WW_RedMet5_reb",tags,aRedMet.pt(),weight);
		mon.fillHisto("Ctrl_WW_RedMet5_reb2",tags,aRedMet.pt(),weight);
	  }
	  if(WW_ctrl_m)   mon.fillHisto("Ctrl_WW_zmass",tags,zll.mass(),weight);
	  if(WW_ctrl_m)   mon.fillHisto("Ctrl_WW_RedMet6",tags,zll.mass(),weight);

	  //Sherpa control 
	  // N.B. 7 TeV:  Sherpa  Zpt>12,  MG Zpt>50  --> cut Zpt>50 to fill the plot
	  //      8 TeV:  Sherpa  Zpt>12,  MG Zpt>12  --> no need to cut
	  if( (!use2011Id) || zll.mass()>50. ) {
		mon.fillHisto("npfjets_pres", tags, nAJetsGood30,weight);
	  }

	  //Main event selection
	  mon.fillHisto("zmass",       tags, zll.mass(), weight);  
	  if(passZmass){
		mon.fillHisto("eventflow",   tags, 3,            weight); 

		mon.fillHisto("nvtx"     ,   tags, ev.nvtx,      weight);
		mon.fillHisto("nvtxraw"  ,   tags, ev.nvtx,      1);
		mon.fillHisto("rho"      ,   tags, ev.rho,       weight);
		mon.fillHisto("rho25"    ,   tags, ev.rho25Neut, weight);

		mon.fillHisto("zpt"      ,   tags, zll.pt(),     weight);      
		mon.fillHisto("zpt_rebin",   tags, zll.pt(),     weight);      
		mon.fillHisto("zpt_rebin2",   tags, zll.pt(),     weight);      
		if(nAJetsGood30==0) mon.fillHisto("zpt_noJet", tags, zll.pt(), weight);      
		if(nAJetsGood30==0 && genmatchid[0]>-1 && genmatchid[1]>-1 && genmatchid[0]!=genmatchid[1]) mon.fillHisto("zptGen_noJet", tags, genZP4.pt(), weight);      
		mon.fillHisto("zeta"     ,   tags, zll.eta(),    weight);

		if(passZpt){
		    mon.fillHisto("eventflow",tags,4,weight);

		    //analyze lepton kinematics
		    LorentzVector leadingLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[0]: phys.leptons[1]);
		    LorentzVector trailerLep(phys.leptons[0].pt()>phys.leptons[1].pt() ? phys.leptons[1]: phys.leptons[0]);
		    mon.fillHisto("leadeta"     ,   tags, leadingLep.eta()   ,weight);
		    mon.fillHisto("leadpt"      ,   tags, leadingLep.pt()    ,weight);
		    mon.fillHisto("trailereta"  ,   tags, trailerLep.eta()   ,weight);
		    mon.fillHisto("trailerpt"   ,   tags, trailerLep.pt()    ,weight);
		    mon.fillHisto("nleptons",tags,2+nextraleptons,weight);
		    for(size_t iel=0; iel<extraLeptonsP4.size(); iel++)
		    {
			  mon.fillHisto("thirdleptonpt" ,   tags,extraLeptonsP4[iel].pt()     ,weight);
			  mon.fillHisto("thirdleptoneta",   tags,extraLeptonsP4[iel].eta()   ,weight);
		    }

		    if(pass3dLeptonVeto){
			  mon.fillHisto("eventflow",tags,5,weight); 

			  //pre-tagged jet control
			  for(size_t ij=0; ij<aGoodIdJets.size(); ij++)
			  {
				mon.fillHisto("pfjetpt",  tags, aGoodIdJets[ij].pt(),weight);
				mon.fillHisto("pfjeteta",  tags, fabs(aGoodIdJets[ij].eta()),weight);
			  }

			  //final jet control
			  mon.fillHisto("npfjets",              tags, nAJetsGood30,weight);
			  mon.fillHisto("npfjetsvspu",          tags, ev.ngenITpu, nAJetsGood30,weight);

			  // jet veto
			  if(passJetveto) {	    

				//sub-divide for good jets
				int eventSubCat  = eventCategoryInst.Get(phys,&aGoodIdJets);
				TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);
				tags.push_back(tag_cat+tag_subcat);
				if(tag_cat=="mumu" || tag_cat=="ee") tags.push_back(string("ll")+tag_subcat);  

				mon.fillHisto("eventflow",tags,6,weight);

				//pre-tagged jet control
				for(size_t ij=0; ij<aGoodIdJets.size(); ij++)
				{
				    if(aGoodIdJets[ij].pt()<30 || fabs(aGoodIdJets[ij].eta())>2.5) continue;
				    if(isMC)
				    {
					  if(fabs(aGoodIdJets[ij].flavid)==5) mon.fillHisto("bpfjetstags",     tags, aGoodIdJets[ij].btag2, weight);
					  else                                mon.fillHisto("otherpfjetstags", tags, aGoodIdJets[ij].btag2, weight);
				    }
				    mon.fillHisto("pfjetstags",  tags, aGoodIdJets[ij].btag2,weight);
				}
				mon.fillHisto("npfjetsbtags",  tags, nABtags ,weight);

				//b-veto
				if(passBveto)
				{
				    mon.fillHisto("eventflow",tags,/*6*/ 7,weight);
				    mon.fillHisto("eventflow_gamma",tags,1,weight);

				    //used to re-weight g+jets
				    mon.fillHisto("qt"      ,   tags, zll.pt(),     weight, true);      
				    mon.fillHisto("nvtx_dy"     ,   tags, ev.nvtx,      weight);
				    mon.fillHisto("zpt_dy"      ,   tags, zll.pt(),     weight); 
				    mon.fillHisto("npfjets_dy",              tags, nAJetsGood30,weight);

				    if(zvvs[0].pt()>0)  mon.fillHisto("mindphijmet_0",  tags, nAJetsGood30==0 ? mindphijmet15:mindphijmet,weight);
				    if(zvvs[0].pt()>25) mon.fillHisto("mindphijmet_25", tags, nAJetsGood30==0 ? mindphijmet15:mindphijmet,weight);
				    if(zvvs[0].pt()>50) mon.fillHisto("mindphijmet_50", tags, nAJetsGood30==0 ? mindphijmet15:mindphijmet,weight);
				    if(zvvs[0].pt()>50) mon.fillHisto("mindphijmet",    tags, nAJetsGood30==0 ? mindphijmet15:mindphijmet,weight);
				    if(zvvs[0].pt()>50 && passLMetVeto) mon.fillHisto("mindphijmet_aftPhil",    tags, nAJetsGood30==0 ? mindphijmet15:mindphijmet,weight);
				    mon.fillHisto("mindphilmet",tags, min(dphil1met,dphil2met) ,weight);
				    if(passDphijmet) mon.fillHisto("mindphilmet_aftPhij",tags, min(dphil1met,dphil2met) ,weight);
				    if(passDphijmet && zvvs[0].pt()>60 ) mon.fillHisto("mindphilmet_aftPhijPt",tags, min(dphil1met,dphil2met) ,weight);
				    mon.fillHisto("maxdphilmet",tags, max(dphil1met,dphil2met) ,weight);
				    mon.fillHisto("met_metbeforedphilmet",         tags,  zvvs[0].pt(),  weight);
				    mon.fillHisto("met_mindphilmet",tags,zvvs[0].pt(),min(dphil1met,dphil2met),weight);

				    if(passDphijmet && passLMetVeto) 
				    {
					  mon.fillHisto("eventflow",tags,/*7*/ 8,weight); 
					  mon.fillHisto("eventflow_gamma",tags,2,weight);

					  mon.fillHisto("deltaleptonpt",tags, leadingLep.pt()-trailerLep.pt()    ,weight);
					  mon.fillHisto("deltazpt",tags, zll.pt()-zvvs[0].pt(),weight);
					  mon.fillHisto("balance",tags, zvvs[0].pt()/zll.pt(),weight);
					  mon.fillHisto("balancedif",tags, balanceDif,weight);

					  mon.fillHisto("met_met",         tags,  zvvs[0].pt(),  weight);
					  mon.fillHisto("met_metL",    tags,  axialMet,  weight);
					  mon.fillHisto("met_mvamet",      tags,  mvaMetP4.pt(), weight);
					  mon.fillHisto("met_typeImet",    tags,  fullTypeIMetP4.pt(),  weight);
					  mon.fillHisto("met_redMet",tags,aRedMet.pt(),weight);
					  mon.fillHisto("met_redMet_ctrl",tags,aRedMet.pt(),weight);
					  mon.fillHisto("met_redMet_rebin",tags,aRedMet.pt(),weight);
					  mon.fillHisto("met_redMet_rebin2",tags,aRedMet.pt(),weight);
					  mon.fillHisto("met_redMet_phi_dy",tags,aRedMet.phi(),weight);
					  mon.fillHisto("met_redMetL",tags,aRedMetT,weight);
					  mon.fillHisto("met_redMetT",tags,aRedMetL,weight);
					  mon.fillHisto("met_clusteredMetL",tags,clusteredMetL,weight);
					  mon.fillHisto("met_clusteredMetT",tags,clusteredMetT,weight);
					  mon.fillHisto("met_metCompL",tags,pfMetCompL,weight);
					  mon.fillHisto("met_metCompT",tags,pfMetCompT,weight);

					  if(passRedMet){
						mon.fillHisto("mindphilmet_aftMet",tags, min(dphil1met,dphil2met) ,weight);
						mon.fillHisto("eventflow",tags,/*8*/ 9,weight); 
						mon.fillHisto("eventflow_gamma",tags,3,weight);

						if(passBalanceCut) {
						    mon.fillHisto("eventflow",tags,10,weight); 
						    mon.fillHisto("eventflow_gamma",tags,4,weight);

						    // Final distributions
						    mon.fillHisto("mt_final",          tags, aMT,          weight);
						    mon.fillHisto("zpt_final",         tags, zll.pt(),     weight);
						    mon.fillHisto("zpt_rebin_final",   tags, zll.pt(),     weight);
						    mon.fillHisto("zpt_rebin2_final",  tags, zll.pt(),     weight);
						    if( zll.pt()>200. ) mon.fillHisto("zpt_rebin_final_lastBin",   tags, zll.pt(),     weight);
						    mon.fillHisto("met_met_final",     tags, zvvs[0].pt(), weight);
						    mon.fillHisto("met_redMet_final",  tags, aRedMet.pt(), weight);
						    mon.fillHisto("met_redMet_rebin_final", tags, aRedMet.pt(), weight);
						    mon.fillHisto("met_redMet_rebin2_final", tags, aRedMet.pt(), weight);
						    mon.fillHisto("met_redMetL_final", tags, aRedMetL,     weight);	
						    mon.fillHisto("met_redMet_ctrl_final", tags, aRedMet.pt(), weight);	
						}//end passBalanceCut
					  } // end passRedMet
				    }//end passDphijmet
				}//end passBveto
			  }//end passJetveto
		    }//3lept
		}//end passZpt
	  }//end passZmass


	  if(passZmass && passZpt && pass3dLeptonVeto && passJetveto && passBveto && passDphijmet && passBalanceCut && passRedMet) { mon.fillHisto("mtNM1", tags, aMT ,weight); }
	  if(             passZpt && pass3dLeptonVeto && passJetveto && passBveto && passDphijmet && passBalanceCut && passRedMet) { mon.fillHisto("zmassNM1",tags,zll.mass(),weight); }
	  if(passZmass            && pass3dLeptonVeto && passJetveto && passBveto && passDphijmet && passBalanceCut && passRedMet) { mon.fillHisto("zptNM1",tags,zll.pt(),weight); }
	  if(passZmass && passZpt                     && passJetveto && passBveto && passDphijmet && passBalanceCut && passRedMet) { mon.fillHisto("nleptonsNM1",tags,2+nextraleptons,weight); }
	  if(passZmass && passZpt && pass3dLeptonVeto                && passBveto && passDphijmet && passBalanceCut && passRedMet) { mon.fillHisto("npfjetsNM1", tags,nAJetsGood30,weight); }
	  if(passZmass && passZpt && pass3dLeptonVeto && passJetveto              && passDphijmet && passBalanceCut && passRedMet) { mon.fillHisto("npfjetsbtagsJPNM1", tags, nABtags ,weight); }
	  if(passZmass && passZpt && pass3dLeptonVeto && passJetveto && passBveto                 && passBalanceCut && passRedMet) { mon.fillHisto("mindphijmetNM1", tags, nAJetsGood30==0 ? mindphijmet15:mindphijmet ,weight);  }
	  if(passZmass && passZpt && pass3dLeptonVeto && passJetveto && passBveto && passDphijmet                   && passRedMet) { mon.fillHisto("balanceNM1",tags,zvvs[0].pt()/zll.pt(),weight); }
	  if(passZmass && passZpt && pass3dLeptonVeto && passJetveto && passBveto && passDphijmet && passBalanceCut              ) { 
		mon.fillHisto("met_metNM1", tags, zvvs[0].pt() ,weight); 
		mon.fillHisto("met_redMetNM1", tags, aRedMet.pt(),weight); 
		mon.fillHisto("met_redMetD0NM1", tags, d0RedMet.pt(),weight); 
	  }

	  //MET control in the sideband
	  bool passSB( ((zll.mass()>40 && zll.mass()<70) || (zll.mass()>110 && zll.mass()<200)) && zll.pt()>55 );      
	  if(passSB && pass3dLeptonVeto && passDphijmet && !passBveto && passLMetVeto) mon.fillHisto("met_metSB",tags,zvvs[0].pt(),weight);

	  //
	  // HISTOS FOR STATISTICAL ANALYSIS (include systematic variations)
	  //
	  //Fill histogram for posterior optimization, or for control regions
	  for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
		float iweight = weight;                                               //nominal
		if(ivar==9)                         iweight *=TotalWeight_plus;        //pu up
		if(ivar==10)                        iweight *=TotalWeight_minus;       //pu down

		float Sherpa_weight(1.);
		if(use2011Id && (ivar==13 || ivar==14)){
		    if( zll.pt()< 20) Sherpa_weight = 1.;
		    else if( zll.pt()< 40) Sherpa_weight = 1.;
		    else if( zll.pt()< 60) Sherpa_weight = 0.993417;
		    else if( zll.pt()< 80) Sherpa_weight = 1.02522;
		    else if( zll.pt()< 100) Sherpa_weight = 0.97613;
		    else if( zll.pt()< 200) Sherpa_weight = 0.997818;
		    else if( zll.pt()< 400) Sherpa_weight = 1.12877;
		    else Sherpa_weight = 1.;
		    if(ivar==14) Sherpa_weight = (1./Sherpa_weight);
		}
		if(!use2011Id && (ivar==13 || ivar==14)){
		    if( zll.pt()< 20) Sherpa_weight = 1.;
		    else if( zll.pt()< 40) Sherpa_weight = 1.;
		    else if( zll.pt()< 60) Sherpa_weight = 1.03351;
		    else if( zll.pt()< 80) Sherpa_weight = 0.955868;
		    else if( zll.pt()< 100) Sherpa_weight = 0.887468;
		    else if( zll.pt()< 200) Sherpa_weight = 0.87193;
		    else if( zll.pt()< 400) Sherpa_weight = 0.561093;
		    else Sherpa_weight = 1.;
		    if(ivar==14) Sherpa_weight = (1./Sherpa_weight);
		}

		//recompute MET/MT if JES/JER was varied
		LorentzVector zvv = zvvs[ivar>8 ? 0 : ivar];
		PhysicsObjectJetCollection &varJets = ( ivar<=4 ? variedAJets[ivar] : ( useJERsmearing ? variedAJets[0] : recoJets ) );
		PhysicsObjectJetCollection tightVarJets;
		LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
		bool passLocalJetveto(true);
		bool passLocalBveto(true); //bool passLocalBveto(passBveto);
		bool passLocalDphijmet(true);
		bool passLocalBalanceCut(true);
		float localmindphijmet(999999.),localmindphijmet15(999999.);
		int localNAJetsGood30(0);
		for(size_t ijet=0; ijet<varJets.size(); ijet++){
		    if(varJets[ijet].pt()<15) continue;

		    // dphi
		    float idphijmet( fabs(deltaPhi(varJets[ijet].phi(),zvv.phi()) ) );
		    if(varJets[ijet].pt()>15) if(idphijmet<localmindphijmet15) localmindphijmet15 = idphijmet;
		    if(varJets[ijet].pt()>30) if(idphijmet<localmindphijmet)   localmindphijmet   = idphijmet;

		    bool isGoodJet = hasObjectId(aJets[ijet].pid,JETID_LOOSE); 
		    if(usePUsubJetId) isGoodJet = hasObjectId(aJets[ijet].pid,JETID_CUTBASED_LOOSE); 
		    if(!isGoodJet) continue;

		    clusteredMetP4 -= varJets[ijet];

		    if(useJetsOnlyInTracker && fabs(varJets[ijet].eta())>2.5) continue; 

		    tightVarJets.push_back( varJets[ijet] );
		    if(varJets[ijet].pt()>30)localNAJetsGood30++;

		    if(varJets[ijet].pt()>20 && fabs(varJets[ijet].eta())<2.5) 
		    {
			  if(ivar==11)      passLocalBveto &= (varJets[ijet].btag2<0.250); 
			  else if(ivar==12) passLocalBveto &= (varJets[ijet].btag2<0.240); 
			  else              passLocalBveto &= (varJets[ijet].btag2<0.244); 
		    }
		}
		passLocalJetveto=(localNAJetsGood30==0); 
		passLocalDphijmet=(localmindphijmet>0.5);
		if(localNAJetsGood30==0) passLocalDphijmet=(localmindphijmet15>0.5);
		passLocalBalanceCut=(zvv.pt()/zll.pt()>0.4 && zvv.pt()/zll.pt()<1.8);

		double dphil1met=fabs(deltaPhi(lep1.phi(),zvv.phi()));
		double dphil2met=fabs(deltaPhi(lep2.phi(),zvv.phi()));
		bool passLocalLMetVeto(true);
		if(!use2011Id && zvv.pt()>60 && min(dphil1met,dphil2met)<0.2) passLMetVeto=false;

		float mt = METUtils::transverseMass(zll,zvv,true);
		LorentzVector nullP4(0,0,0,0);
		LorentzVector redMet = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, zll, 0, nullP4, 0, clusteredMetP4, zvv, true);

		// with standard Z mass, Z pt, RedMet (will be variated later)
		bool passPreselection(passZmass && passZpt && pass3dLeptonVeto && passLocalJetveto && passLocalBveto && passLocalDphijmet && passLocalLMetVeto && passLocalBalanceCut && passRedMet);
		bool passPreselectionMbvetoMzmass(passZpt && pass3dLeptonVeto && passLocalJetveto && passLocalDphijmet && passLocalLMetVeto && passLocalBalanceCut && passRedMet);          

		//re-assign the event category if jets were varied
		int eventSubCat  = eventCategoryInst.Get(phys,&tightVarJets);
		TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);
		tags.clear();
		tags.push_back(tag_cat);
		tags.push_back(tag_cat+tag_subcat);
		if(tag_cat=="mumu" || tag_cat=="ee") tags.push_back(string("ll")+tag_subcat);

		//fill shapes
		for(unsigned int index=0;index<optim_Cuts1_met.size();index++){

		    float minMet=optim_Cuts1_met[index];
		    float minZpt=optim_Cuts1_zpt[index];
		    float deltaZ=optim_Cuts1_zmass[index];

		    bool passLocalRedMet(redMet.pt()>minMet); 
		    bool passLocalZmass(fabs(zll.mass()-91)<deltaZ);
		    bool passLocalZpt(zll.pt()>minZpt);

		    passPreselection = (passLocalZmass && passLocalZpt && pass3dLeptonVeto && passLocalJetveto && passLocalBveto && passLocalDphijmet && passLocalBalanceCut && passLocalRedMet);
		    passPreselectionMbvetoMzmass = (passLocalZpt && pass3dLeptonVeto && passLocalJetveto && passLocalDphijmet && passLocalBalanceCut && passLocalRedMet); 
		    bool passPreselectionMjvetoMbvetoMzmass = (passLocalZpt && pass3dLeptonVeto && passLocalDphijmet && passLocalBalanceCut && passLocalRedMet); 
		    if(ivar==13 || ivar==14){ iweight*=Sherpa_weight;
			  if( passPreselection ) {
				mon.fillHisto(TString("redMet_shapes")+varNames[ivar],tags,index, redMet.pt(),iweight);
				mon.fillHisto(TString("redMet_rebin_shapes")+varNames[ivar],tags,index, redMet.pt(),iweight);
				mon.fillHisto(TString("zpt_shapes")+varNames[ivar],tags,index, zll.pt(),iweight);
				mon.fillHisto(TString("zpt_rebin_shapes")+varNames[ivar],tags,index, zll.pt(),iweight);
			  }
			  if( passPreselectionMjvetoMbvetoMzmass && passLocalZmass && passLocalJetveto && passLocalBveto ) {
				mon.fillHisto("redMet_shapes_NRBctrl"+varNames[ivar],tags,index,0,iweight);
				mon.fillHisto("redMet_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,0,iweight);
				mon.fillHisto("zpt_shapes_NRBctrl"+varNames[ivar],tags,index,0,iweight);
				mon.fillHisto("zpt_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,0,iweight);
			  }
			  if( passPreselectionMjvetoMbvetoMzmass && isZsideBand && passLocalJetveto && passLocalBveto ) {
				mon.fillHisto("redMet_shapes_NRBctrl"+varNames[ivar],tags,index,1,iweight);
				mon.fillHisto("redMet_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,1,iweight);
				mon.fillHisto("zpt_shapes_NRBctrl"+varNames[ivar],tags,index,1,iweight);
				mon.fillHisto("zpt_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,1,iweight);
			  }
			  if( passPreselectionMjvetoMbvetoMzmass && isZsideBandPlus && passLocalJetveto && passLocalBveto ) {
				mon.fillHisto("redMet_shapes_NRBctrl"+varNames[ivar],tags,index,2,iweight);
				mon.fillHisto("redMet_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,2,iweight);
				mon.fillHisto("zpt_shapes_NRBctrl"+varNames[ivar],tags,index,2,iweight);
				mon.fillHisto("zpt_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,2,iweight);
			  }
			  if( passPreselectionMjvetoMbvetoMzmass && passLocalZmass && !passLocalBveto ) {
				mon.fillHisto("redMet_shapes_NRBctrl"+varNames[ivar],tags,index,3,iweight);
				mon.fillHisto("redMet_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,3,iweight);
				mon.fillHisto("zpt_shapes_NRBctrl"+varNames[ivar],tags,index,3,iweight);
				mon.fillHisto("zpt_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,3,iweight);
			  }
			  if( passPreselectionMjvetoMbvetoMzmass && isZsideBand && !passLocalBveto ) {
				mon.fillHisto("redMet_shapes_NRBctrl"+varNames[ivar],tags,index,4,iweight);
				mon.fillHisto("redMet_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,4,iweight);
				mon.fillHisto("zpt_shapes_NRBctrl"+varNames[ivar],tags,index,4,iweight);
				mon.fillHisto("zpt_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,4,iweight);
			  }
			  if( passPreselectionMjvetoMbvetoMzmass && isZsideBandPlus && !passLocalBveto ) {
				mon.fillHisto("redMet_shapes_NRBctrl"+varNames[ivar],tags,index,5,iweight);
				mon.fillHisto("redMet_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,5,iweight);
				mon.fillHisto("zpt_shapes_NRBctrl"+varNames[ivar],tags,index,5,iweight);
				mon.fillHisto("zpt_rebin_shapes_NRBctrl"+varNames[ivar],tags,index,5,iweight);
			  }
		    }
		}
		if(passZmass && passZpt && pass3dLeptonVeto && passJetveto && passBveto && passDphijmet && passBalanceCut){
		    if(ivar==9){
			  mon.fillHisto("met_metNM1_PUUp", tags, zvvs[0].pt() ,iweight); 
			  mon.fillHisto("met_redMetNM1_PUUp", tags, aRedMet.pt(),iweight); 
			  mon.fillHisto("met_redMetD0NM1_PUUp", tags, d0RedMet.pt(),iweight); 
		    }
		    if(ivar==10){
			  mon.fillHisto("met_metNM1_PUDown", tags, zvvs[0].pt() ,iweight);
			  mon.fillHisto("met_redMetNM1_PUDown", tags, aRedMet.pt(),iweight);
			  mon.fillHisto("met_redMetD0NM1_PUDown", tags, d0RedMet.pt(),iweight);
		    }
		    if(ivar==3){
			  mon.fillHisto("met_metNM1_JESUp", tags, zvvs[0].pt() ,iweight); 
			  mon.fillHisto("met_redMetNM1_JESUp", tags, aRedMet.pt(),iweight); 
			  mon.fillHisto("met_redMetD0NM1_JESUp", tags, d0RedMet.pt(),iweight); 
		    }
		    if(ivar==4){
			  mon.fillHisto("met_metNM1_JESDown", tags, zvvs[0].pt() ,iweight);
			  mon.fillHisto("met_redMetNM1_JESDown", tags, aRedMet.pt(),iweight);
			  mon.fillHisto("met_redMetD0NM1_JESDown", tags, d0RedMet.pt(),iweight);
		    }
		}
	  }
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
    ofile->Close();

    if(outTxtFile)fclose(outTxtFile);
}  





