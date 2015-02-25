#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "QuarkGluonTagger/EightTeV/interface/QGLikelihoodCalculator.h"
#include "QuarkGluonTagger/EightTeV/interface/QGMLPCalculator.h"

#include "PUWeight.h"

#include <vector>
#include <map>
#include <string>
using namespace std;


std::string treeType = "sunil"; //can be andrea/tom/sunil

bool pthat_reweight = true;


float get_xSecWeight( const std::string& dataset );
float correctNCharged(int nChg_QCJet, float eta);


int main( int argc, char* argv[] ) {


  if( argc<3 ) {
    std::cout << "USAGE: ./makeSunilTreeFlat [DATASET] [SELECTIONTYPE=\"ZJet\" or \"DiJet\"] [DIRECTORY=\"/afs/cern.ch/work/s/sunil/public/forTom/\"]" << std::endl;
    exit(11);
  }


  std::string dataset;
  if( argc>1 ) {
    std::string dataset_str(argv[1]);
    dataset = dataset_str;
  }
  TString dataset_tstr(dataset);


  std::string selectionType;
  if( argc>2 ) {
    std::string selectionType_str(argv[2]);
    selectionType = selectionType_str;
  }

  if( selectionType!="ZJet" && selectionType!="DiJet" ) {
    std::cout << "Invalid selectionType: Only 'ZJet' and 'DiJet' currently supported." << std::endl;
    exit(113);
  }


  std::string dir;
  if( treeType=="sunil" )
    dir="/afs/cern.ch/work/s/sunil/public/forTom/";
  else if( treeType=="andrea" )
    dir="root://eoscms//eos/cms/store/user/amarini/zjets_V00-12/";
  if( argc>3 ) {
    std::string dir_str(argv[3]);
    dir = dir_str;
  }
    

  std::cout << "dataset: " << dataset << std::endl;
  std::cout << "selectionType: " << selectionType << std::endl;
  std::cout << "dir: " << dir << std::endl;

  std::string sunilTreeName = dir;
  if( treeType=="sunil" ) 
    sunilTreeName += "/analysis_" + dataset + ".root/Hbb/events";
  else if( treeType=="andrea" )
    sunilTreeName += "/" + dataset + ".root/accepted/events";

  TChain* sunilTree = new TChain("events");
  sunilTree->Add(sunilTreeName.c_str());
  
  std::cout << "-> Opening: " <<  sunilTreeName << std::endl;
  int nentries = sunilTree->GetEntries();
  std::cout << "-> Tree has: " << nentries << std::endl;


  std::string flatFileName = "sunilFlat_" + selectionType + "_" + dataset;
  if( pthat_reweight && dataset_tstr.Contains("flat") )
    flatFileName += "_ptHatWeight";
  flatFileName += ".root";
  TFile* newFile = TFile::Open(flatFileName.c_str(), "recreate");
  newFile->cd();

  TTree* flatTree = new TTree("tree_passedEvents", "");

  int run;
  int event;
  float genweight;
  float rho;
  float rhoMLP;
  int nvtx;
  int npu;
  std::vector<bool> *triggerResult = 0;
  float jetPt[20];
  float jetEta[20];
  float jetPhi[20];
  float jetEnergy[20];
  float jetBeta[20];
  float jetBtag[20];
  int nCharged[20];
  int nNeutral[20];
  float jetPtD[20];
  float jetPtD_QC[20];
  Float_t jetAxis_QC[2][4];
  Float_t jetAxis[2][4];
  float jetPull_QC[4];
  float jetLead[4];
  int jetPdgId[20];
  int jetMCFlavor[20];
  int jetFlavor[20];
  int nCharged_QC[20];
  int nCharged_ptCut[20];
  int nNeutral_ptCut[20];
  std::vector<int> *partonId=0;
  std::vector<int> *partonSt=0;
  std::vector<float> *partonPt=0;
  std::vector<float> *partonEta=0;
  std::vector<float> *partonPhi=0;
  std::vector<float> *partonE=0;
  float MuEta[2];
  float MuPhi[2];
  float MuPt[2];
  float MuEnergy[2];



  // vars for andrea tree:
  int nJet;
  int isTriggered;
  std::vector<float>* v_jetPt;
  std::vector<float>* v_jetEta;
  std::vector<float>* v_jetPhi;
  std::vector<float>* v_jetEnergy;
  std::vector<float>* v_jetBeta;
  std::vector<float>* v_jetBtag;
  std::vector<float>* v_jetPtD_QC;
  std::vector<float>* v_lepEta;
  std::vector<float>* v_lepPhi;
  std::vector<float>* v_lepPt;
  std::vector<float>* v_lepEnergy;
  std::vector<int>*   v_lepChId;
  std::vector<int>*   v_jetPdgId;
  std::vector<int>*   v_jetMultiplicity;
  std::vector<float>* v_axis2_QC;
  std::vector<int>*   v_jetMCFlavour;

  v_jetPt=0;
  v_jetEta=0;
  v_jetPhi=0;
  v_jetEnergy=0;
  v_jetBeta=0;
  v_jetBtag=0;
  v_jetPtD_QC=0;
  v_lepEta=0;
  v_lepPhi=0;
  v_lepPt=0;
  v_lepEnergy=0;
  v_lepChId=0;
  v_jetPdgId=0;
  v_jetMultiplicity=0;
  v_axis2_QC=0;
  v_jetMCFlavour=0;

  TBranch* b_jetPt;
  TBranch* b_jetEta;
  TBranch* b_jetPhi;
  TBranch* b_jetEnergy;
  TBranch* b_jetBeta;
  TBranch* b_jetBtag;
  TBranch* b_jetPtD_QC;
  TBranch* b_MuEta;
  TBranch* b_MuPhi;
  TBranch* b_MuPt;
  TBranch* b_MuEnergy;
  TBranch* b_lepChId;
  TBranch* b_jetPdgId;
  TBranch* b_jetMultiplicity;
  TBranch* b_axis2_QC;
  TBranch* b_jetMCFlavour;

  if( treeType=="sunil" ) {
    sunilTree->SetBranchAddress("runNo", &run);
    sunilTree->SetBranchAddress("evtNo", &event);
    sunilTree->SetBranchAddress("weight", &genweight);
    sunilTree->SetBranchAddress("nvtx", &nvtx);
    sunilTree->SetBranchAddress("npu", &npu);
    sunilTree->SetBranchAddress("rhoIso", &rho);
    sunilTree->SetBranchAddress("rho", &rhoMLP);
    sunilTree->SetBranchAddress("triggerResult", &triggerResult);

    sunilTree->SetBranchAddress("jetPt", jetPt);
    sunilTree->SetBranchAddress("jetEta", jetEta);
    sunilTree->SetBranchAddress("jetPhi", jetPhi);
    sunilTree->SetBranchAddress("jetEnergy", jetEnergy);
    sunilTree->SetBranchAddress("jetBeta", jetBeta);
    sunilTree->SetBranchAddress("jetBtag", jetBtag);
    sunilTree->SetBranchAddress("jetChgPart", nCharged);
    sunilTree->SetBranchAddress("jetNeutralPart", nNeutral);
    sunilTree->SetBranchAddress("jetPtD", jetPtD);
    sunilTree->SetBranchAddress("jetPtD_QC", jetPtD_QC);
    sunilTree->SetBranchAddress("jetAxis",&jetAxis);
    sunilTree->SetBranchAddress("jetAxis_QC",&jetAxis_QC);
    sunilTree->SetBranchAddress("jetPull_QC",&jetPull_QC);
    sunilTree->SetBranchAddress("jetLead",&jetLead);
    sunilTree->SetBranchAddress("jetChgPart_QC", nCharged_QC);
    sunilTree->SetBranchAddress("jetFlavor", jetFlavor);
    
    sunilTree->SetBranchAddress("jetNeutralPart_ptcut", nNeutral_ptCut);
    sunilTree->SetBranchAddress("partonId",&partonId);
    sunilTree->SetBranchAddress("partonSt",&partonSt);
    sunilTree->SetBranchAddress("partonPt",&partonPt);
    sunilTree->SetBranchAddress("partonEta",&partonEta);
    sunilTree->SetBranchAddress("partonPhi",&partonPhi);
    sunilTree->SetBranchAddress("partonE",&partonE);
    if( selectionType=="ZJet" ) {
      sunilTree->SetBranchAddress("MuPt",MuPt);
      sunilTree->SetBranchAddress("MuEta",MuEta);
      sunilTree->SetBranchAddress("MuPhi",MuPhi);
      sunilTree->SetBranchAddress("MuEnergy",MuEnergy);
    }
  } 
  
  else if( treeType=="andrea" ) {

    sunilTree->SetBranchAddress("runNum", &run);
    sunilTree->SetBranchAddress("nVtx", &nvtx);
    sunilTree->SetBranchAddress("npu", &npu);
    sunilTree->SetBranchAddress("rhoQG", &rho);
    sunilTree->SetBranchAddress("isTriggered", &isTriggered);

    sunilTree->SetBranchAddress("nJets", &nJet);
    sunilTree->SetBranchAddress("jetPt",        &v_jetPt,          &b_jetPt);
    sunilTree->SetBranchAddress("jetEta",       &v_jetEta,         &b_jetEta);
    sunilTree->SetBranchAddress("jetPhi",       &v_jetPhi,         &b_jetPhi);
    sunilTree->SetBranchAddress("jetE",         &v_jetEnergy,      &b_jetEnergy);
    sunilTree->SetBranchAddress("jetBeta",      &v_jetBeta,        &b_jetBeta);
    sunilTree->SetBranchAddress("jetBtag",      &v_jetBtag,        &b_jetBtag);
    sunilTree->SetBranchAddress("jetPdgId",     &v_jetPdgId,       &b_jetPdgId);
    sunilTree->SetBranchAddress("jetMCFlavour", &v_jetMCFlavour,   &b_jetMCFlavour);
    sunilTree->SetBranchAddress("jetQG_axis2_L", &v_axis2_QC,      &b_axis2_QC);
    sunilTree->SetBranchAddress("jetQG_ptD_L",  &v_jetPtD_QC,      &b_jetPtD_QC);
    sunilTree->SetBranchAddress("jetQG_mult_L", &v_jetMultiplicity,&b_jetMultiplicity);
    if( selectionType=="ZJet" ) {                                    
      sunilTree->SetBranchAddress("lepChId",    &v_lepChId,        &b_lepChId);
      sunilTree->SetBranchAddress("lepPt",      &v_lepPt,           &b_MuPt);
      sunilTree->SetBranchAddress("lepEta",     &v_lepEta,          &b_MuEta);
      sunilTree->SetBranchAddress("lepPhi",     &v_lepPhi,          &b_MuPhi);
      sunilTree->SetBranchAddress("lepE",  &v_lepEnergy,       &b_MuEnergy);
    }

  } //if andrea


  float eventWeight;
  float wt_pu;
  float wt_pteta;
  float wt_xsec;
  float rmsCand_QC[20];
  float axis1_QC[20];
  float axis2_QC[20];
  float qglMLPJet[20];
  float qglJet[20];
  float qglNoMultCorrJet[20];
  float qglCorrJet[20];
  float ptZ;

  flatTree->Branch("run", &run, "run/I");
  flatTree->Branch("event", &event, "event/I");
  flatTree->Branch("eventWeight", &eventWeight, "eventWeight/F");
  flatTree->Branch("wt_pu", &wt_pu, "wt_pu/F");
  flatTree->Branch("wt_pteta", &wt_pteta, "wt_pteta/F");
  flatTree->Branch("wt_xsec", &wt_xsec, "wt_xsec/F");
  flatTree->Branch("nvertex", &nvtx, "nvertex/I");
  flatTree->Branch("rhoPF", &rho, "rhoPF/F");
  flatTree->Branch("rhoPF_allJet", &rhoMLP, "rhoPF_allJet/F");
  flatTree->Branch("ptZ", &ptZ, "ptZ/F");
  flatTree->Branch("nJet", &nJet, "nJet/I");
  flatTree->Branch("ptJet", jetPt, "ptJet[nJet]/F");
  flatTree->Branch("etaJet", jetEta, "etaJet[nJet]/F");
  flatTree->Branch("pdgIdJet", jetPdgId, "pdgIdJet[nJet]/I");
  flatTree->Branch("mcFlavorJet", jetMCFlavor, "mcFlavorJet[nJet]/I");
  flatTree->Branch("nChargedJet", nCharged, "nChargedJet[nJet]/I");
  flatTree->Branch("nNeutralJet", nNeutral, "nNeutralJet[nJet]/I");
  flatTree->Branch("ptDJet", jetPtD, "ptDJet[nJet]/F");
  flatTree->Branch("ptD_QCJet", jetPtD_QC, "ptDJet_QC[nJet]/F");
  flatTree->Branch("pull_QCJet", jetPull_QC, "pull_QC[nJet]/F");
  flatTree->Branch("RJet", jetLead, "RJet[nJet]/F");
  flatTree->Branch("axis1_QCJet", axis1_QC, "axis1_QCJet[nJet]/F");
  flatTree->Branch("axis2_QCJet", axis2_QC, "axis2_QCJet[nJet]/F");
  flatTree->Branch("rmsCand_QCJet", rmsCand_QC, "rmsCand_QCJet[nJet]/F");
  flatTree->Branch("nChg_QCJet", nCharged_QC, "nChg_QCJet[nJet]/I");
  flatTree->Branch("nNeutral_ptCutJet", nNeutral_ptCut, "nNeutral_ptCutJet[nJet]/I");
  flatTree->Branch("qgMLPJet", qglMLPJet, "qgMLPJet[nJet]/F");
  flatTree->Branch("qglJet", qglJet, "qglJet[nJet]/F");
  flatTree->Branch("qglNoMultCorrJet", qglNoMultCorrJet, "qglNoMultCorrJet[nJet]/F");
  flatTree->Branch("qglCorrJet", qglCorrJet, "qglCorrJet[nJet]/F");



  std::cout << "-> Booking QGLikelihoodCalculator..." << std::endl;
  QGLikelihoodCalculator* qglc = new QGLikelihoodCalculator("QuarkGluonTagger/EightTeV/data/");
  //std::cout << "-> Booking QGMLPCalculator..." << std::endl;
  //QGMLPCalculator* qgmlp = new QGMLPCalculator("MLP","QuarkGluonTagger/EightTeV/data/", true);


  /* PUWeight* fPUWeight = new PUWeight(-1, "2012", "Summer12");

  TFile* filePU_data = TFile::Open("PU_all_minBias69400.root");
  //TFile* filePU_data = TFile::Open("puProfile_Data12.root");
  TFile* filePU_mc = TFile::Open("puProfile_ZJetsMC.root");
  //TFile* filePU_mc = TFile::Open("puProfile_Summer12_53X.root");
  TH1F* h1_nPU_data = (TH1F*)filePU_data->Get("pileup");
  TH1F* h1_nPU_mc = (TH1F*)filePU_mc->Get("pileup");
  fPUWeight->SetDataHistogram(h1_nPU_data);
  fPUWeight->SetMCHistogram(h1_nPU_mc);
	*/

  std::vector<float> puWeight;
  //puWeight.push_back(0.421312);
  //puWeight.push_back(0.517868);
  //puWeight.push_back(0.618018);
  //puWeight.push_back(0.690921);
  //puWeight.push_back(0.783158);
  //puWeight.push_back(0.878405);
  //puWeight.push_back(0.966719);
  //puWeight.push_back(1.05782);
  //puWeight.push_back(1.13373);
  //puWeight.push_back(1.18274);
  //puWeight.push_back(1.22057);
  //puWeight.push_back(1.23313);
  //puWeight.push_back(1.22958);
  //puWeight.push_back(1.21817);
  //puWeight.push_back(1.20166);
  //puWeight.push_back(1.18285);
  //puWeight.push_back(1.16271);
  //puWeight.push_back(1.14628);
  //puWeight.push_back(1.13439);
  //puWeight.push_back(1.12243);
  //puWeight.push_back(1.11229);
  //puWeight.push_back(1.10403);
  //puWeight.push_back(1.09037);
  //puWeight.push_back(1.07618);
  //puWeight.push_back(1.05775);
  //puWeight.push_back(1.03348);
  //puWeight.push_back(1.00435);
  //puWeight.push_back(0.970657);
  //puWeight.push_back(0.930082);
  //puWeight.push_back(0.88566);
  //puWeight.push_back(0.836182);
  //puWeight.push_back(0.789208);
  //puWeight.push_back(0.737106);
  //puWeight.push_back(0.680498);
  //puWeight.push_back(0.624485);
  //puWeight.push_back(0.572128);
  //puWeight.push_back(0.518744);
  //puWeight.push_back(0.46872);
  //puWeight.push_back(0.420961);
  //puWeight.push_back(0.373511);
  //puWeight.push_back(0.329879);
  //puWeight.push_back(0.291623);
  //puWeight.push_back(0.255549);
  //puWeight.push_back(0.221246);
  //puWeight.push_back(0.189964);
  //puWeight.push_back(0.164114);
  //puWeight.push_back(0.140578);
  //puWeight.push_back(0.118958);
  //puWeight.push_back(0.100604);
  //puWeight.push_back(0.0849639);
  //puWeight.push_back(0.0697544);

 
  puWeight.push_back(0.252009);
  puWeight.push_back(0.327042);
  puWeight.push_back(0.335502);
  puWeight.push_back(0.352291);
  puWeight.push_back(0.324698);
  puWeight.push_back(0.582754);
  puWeight.push_back(0.455286);
  puWeight.push_back(0.441035);
  puWeight.push_back(0.607629);
  puWeight.push_back(0.930019);
  puWeight.push_back(1.3379);
  puWeight.push_back(1.69521);
  puWeight.push_back(1.74041);
  puWeight.push_back(1.54857);
  puWeight.push_back(1.32193);
  puWeight.push_back(1.15754);
  puWeight.push_back(1.07437);
  puWeight.push_back(1.05152);
  puWeight.push_back(1.07105);
  puWeight.push_back(1.11463);
  puWeight.push_back(1.15493);
  puWeight.push_back(1.17791);
  puWeight.push_back(1.18516);
  puWeight.push_back(1.17815);
  puWeight.push_back(1.1528);
  puWeight.push_back(1.10551);
  puWeight.push_back(1.03652);
  puWeight.push_back(0.948613);
  puWeight.push_back(0.844304);
  puWeight.push_back(0.728083);
  puWeight.push_back(0.607242);
  puWeight.push_back(0.489813);
  puWeight.push_back(0.381766);
  puWeight.push_back(0.287126);
  puWeight.push_back(0.207777);
  puWeight.push_back(0.144102);
  puWeight.push_back(0.0957851);
  puWeight.push_back(0.0611744);
  puWeight.push_back(0.0376984);
  puWeight.push_back(0.0226007);
  puWeight.push_back(0.0133203);
  puWeight.push_back(0.00782018);
  puWeight.push_back(0.00464555);
  puWeight.push_back(0.00284065);
  puWeight.push_back(0.00182028);
  puWeight.push_back(0.00123555);
  puWeight.push_back(0.000891118);
  puWeight.push_back(0.000679799);
  puWeight.push_back(0.000543107);
  puWeight.push_back(0.000449514);
  puWeight.push_back(0.000382089);
  puWeight.push_back(0.000331034);
  puWeight.push_back(0.000290923);
  puWeight.push_back(0.00025824);
  puWeight.push_back(0.000230472);
  puWeight.push_back(0.000206238);
  puWeight.push_back(0.000184523);
  puWeight.push_back(0.000164717);
  puWeight.push_back(0.000146364);
  puWeight.push_back(0.000271878);

  //std::string puFileName = (selectionType=="ZJet") ? "PU_rewt_ZJets.root" : "PU_rewt_flatP6.root";
  //TFile *fPU = TFile::Open(puFileName.c_str());
  //std::string puHistName = (selectionType=="ZJet") ? "hist_puWT" : "hist_WT"; //absurd
  //TH1F *hPU = (TH1F*)fPU->Get(puHistName.c_str());

  //TFile* filePtEtaWeights = TFile::Open("Jetpteta_rewt2D_flatP6.root");
  //TH1F* hPtEta_wt = (TH1F*)filePtEtaWeights->Get("hist_WT");

  std::string rhoWeightFileName = "rhoWeights_" + dataset + ".root";
  if( dataset_tstr.Contains("Zjets_Hpp") ) rhoWeightFileName = "rhoWeights_Zjets_Hpp.root";
  std::cout << "-> Opening rho weights file: " << rhoWeightFileName << std::endl;
  TFile* fileRhoWeights = TFile::Open(rhoWeightFileName.c_str());
  TH1F* hPU;
  if( fileRhoWeights!=0 )
    hPU = (TH1F*)fileRhoWeights->Get("rho_weights");


  //std::string ptWeightFileName = "ptWeights_" + dataset + ".root";
  //TH1F* hPt_wt;
  //if( 
  //TFile* filePtWeights = TFile::Open(ptWeightFileName.c_str());
  //if( filePtWeights!=0 )
  //  hPt_wt = (TH1F*)filePtWeights->Get("ptAve_weights");


  std::map<string,long> counter;
  std::cout << "-> Begin loop." << std::endl;

  for( unsigned int ientry=0; ientry<nentries; ++ientry ) {

    sunilTree->GetEntry(ientry);

    if( ientry % 100000 == 0 ) std::cout << "entry: " << ientry << "/" << nentries << std::endl;
	
	counter["A_all"]++;

    bool isMC = (run < 10000);

    // trigger
    if( !isMC ) {
      if(selectionType=="ZJet" ) {
        if( treeType=="andrea" ) {
          if( !(isTriggered&1) ) continue;
        } else {
          if(!triggerResult->at(0)) continue;
        }
      } else {
        if( dataset_tstr.Contains("_MBPD_") && !triggerResult->at(19)) continue;
        if( dataset_tstr.Contains("_JetPD_") && !triggerResult->at(1)) continue;
      }
    }

	counter["BA_trigger"]++;

    if( treeType=="andrea" ) {
      
      if( nJet==0 ) continue;

      // convert:

      MuEta    [0] = (v_lepEta->size()>0) ?    v_lepEta->at(0) : -999.;
      MuPhi    [0] = (v_lepPhi->size()>0) ?    v_lepPhi->at(0) : -999.;
      MuPt     [0] = (v_lepPt->size()>0) ?     v_lepPt->at(0) : -999.;
      MuEnergy [0] = (v_lepEnergy->size()>0) ? v_lepEnergy->at(0) : -999.;
      MuEta    [1] = (v_lepEta->size()>1) ?    v_lepEta->at(1) : -999.;
      MuPhi    [1] = (v_lepPhi->size()>1) ?    v_lepPhi->at(1) : -999.;
      MuPt     [1] = (v_lepPt->size()>1) ?     v_lepPt->at(1) : -999.;
      MuEnergy [1] = (v_lepEnergy->size()>1) ? v_lepEnergy->at(1) : -999.;

      jetPt    [0] = (v_jetPt->size()>0) ?     v_jetPt->at(0) : -999.;
      jetEta   [0] = (v_jetEta->size()>0) ?    v_jetEta->at(0) : -999.;
      jetPhi   [0] = (v_jetPhi->size()>0) ?    v_jetPhi->at(0) : -999.;
      jetEnergy[0] = (v_jetEnergy->size()>0) ? v_jetEnergy->at(0) : -999.;
      jetBeta  [0] = (v_jetBeta->size()>0) ?   v_jetBeta->at(0) : -999.;
      jetBtag  [0] = (v_jetBtag->size()>0) ?   v_jetBtag->at(0) : -999.;
      jetPtD_QC[0] = (v_jetPtD_QC->size()>0) ? v_jetPtD_QC->at(0) : -999.;
      axis2_QC[0]  = (v_axis2_QC->size()>0) ?  v_axis2_QC->at(0) : -999.;

      jetPt    [1] = (v_jetPt->size()>1) ?     v_jetPt->at(1) : -999.;
      jetEta   [1] = (v_jetEta->size()>1) ?    v_jetEta->at(1) : -999.;
      jetPhi   [1] = (v_jetPhi->size()>1) ?    v_jetPhi->at(1) : -999.;
      jetEnergy[1] = (v_jetEnergy->size()>1) ? v_jetEnergy->at(1) : -999.;
      jetBeta  [1] = (v_jetBeta->size()>1) ?   v_jetBeta->at(1) : -999.;
      jetBtag  [1] = (v_jetBtag->size()>1) ?   v_jetBtag->at(1) : -999.;
      jetPtD_QC[1] = (v_jetPtD_QC->size()>1) ? v_jetPtD_QC->at(1) : -999.;
      axis2_QC[1]  = (v_axis2_QC->size()>1) ?  v_axis2_QC->at(1) : -999.;

      
    }


    // event selection:

    if( selectionType=="DiJet" ) {

      if( jetPt[0] < 30.) continue; 
      if( jetPt[1] < 30.) continue;
      //if( jetPt[0] < 20.) continue; 
      //if( jetPt[1] < 20.) continue;
 
	counter["BB_secondJet"]++;

      TLorentzVector jet1, jet2;
      jet1.SetPtEtaPhiE( jetPt[0], jetEta[0], jetPhi[0], jetEnergy[0]);
      jet2.SetPtEtaPhiE( jetPt[1], jetEta[1], jetPhi[1], jetEnergy[1]);
      if( fabs(jet1.DeltaPhi(jet2)) < 2.5 ) continue;
	counter["BC_deltaPhi"]++;

      if( jetPt[2] > 0.3*(jetPt[0]+jetPt[1])/2. )continue;
	counter["BD_3rdJet"]++;

      ptZ = 0.;

    } else if( selectionType=="ZJet" ) {

      if( treeType=="andrea" && v_lepChId->size()<2 ) continue; 
      if( treeType=="andrea" && v_lepChId->at(0)*v_lepChId->at(1) != -4 ) continue; //muons

      if( MuPt[0]<20. ) continue;
      if( MuPt[1]<20. ) continue;
	counter["C_MuPt"]++;

      TLorentzVector mu1, mu2;
      mu1.SetPtEtaPhiE( MuPt[0], MuEta[0], MuPhi[0], MuEnergy[0] );
      mu2.SetPtEtaPhiE( MuPt[1], MuEta[1], MuPhi[1], MuEnergy[1] );

      TLorentzVector Zmm = mu1 + mu2;

      if( Zmm.M()<70. || Zmm.M()>110. ) continue;
	counter["D_invMass"]++;
      if( treeType=="andrea" && nJet<1 ) continue;
      if( jetPt[0]<20. ) continue; 
      TLorentzVector jet;
      jet.SetPtEtaPhiE( jetPt[0], jetEta[0], jetPhi[0], jetEnergy[0] );
      if( fabs(Zmm.DeltaPhi(jet)) < 2.5 ) continue;
	counter["E_deltaPhi"]++;
      if( jetPt[1]>0.3*Zmm.Pt() ) continue;
	counter["F_2ndJet"]++;

      ptZ = Zmm.Pt();

    }

    // common jet ID:
    if( fabs(jetEta[0]) < 2.5 && jetBeta[0] < ( 1.0 -  0.2 * TMath::Log( nvtx - 0.67))) continue; 
    if( jetBtag[0]>0.244 ) continue;
	counter["G_betaStar"]++;
    if( selectionType=="DiJet" ) {
      if( fabs(jetEta[1]) < 2.5 && jetBeta[1] < ( 1.0 -  0.2 * TMath::Log( nvtx - 0.67))) continue; 
      if( jetBtag[1]>0.244 ) continue;
    }
    //if( jetPtD_QC[0]>0.9 ) continue;


    // set event weights:
    wt_pu = 1.;
    wt_pteta = 1.;
    wt_xsec = 1.;

    if( isMC ) {

      // kinematic reweighting only for dijets:
      if( selectionType=="DiJet" ) {

        wt_pteta = genweight;

        //if( pthat_reweight ) {

        //  wt_pteta = genweight;

        //} else {

        //  int ptAveBin = hPt_wt->FindBin( 0.5*(jetPt[0]+jetPt[1]) );
        //  wt_pteta = hPt_wt->GetBinContent(ptAveBin);

        //  //int ptetabin = hPtEta_wt->FindBin(jetPt[0],fabs(jetEta[0]));
        //  //wt_pteta = hPtEta_wt->GetBinContent(ptetabin);
        //
        //}

      }

      wt_xsec =  get_xSecWeight(dataset);

      // pu reweighting based on rho:
      int bin = hPU->FindBin(rho);
      wt_pu = hPU->GetBinContent(bin);

      // official pu reweighting:
      //wt_pu = puWeight[npu];

    }


    eventWeight = wt_xsec*wt_pu*wt_pteta;




    // and now set variables:

    // **** FIRST FOR FIRST JET

    if( treeType=="sunil" ) {

      axis1_QC[0] = jetAxis_QC[0][0];
      axis2_QC[0] = jetAxis_QC[1][0];
      rmsCand_QC[0] = (axis1_QC[0]>0. && axis2_QC[0]>0.) ? sqrt( axis1_QC[0]*axis1_QC[0] + axis2_QC[0]*axis2_QC[0] ) : -1.;

      nJet=0;
      for( unsigned ijet=0; ijet<4; ++ijet )
        if( jetPt[ijet]>20. ) nJet++;

    }




    TLorentzVector thisJet;
    thisJet.SetPtEtaPhiE( jetPt[0], jetEta[0], jetPhi[0], jetEnergy[0] );


    if( treeType=="sunil" ) {

      // match to parton:
      float deltaR_min = 999.;
      int foundPart = -1;
  
      float deltaR_min_charm = 999.;
      int foundPart_charm = -1;
  
      float deltaR_min_bottom = 999.;
      int foundPart_bottom = -1;
  
 	//matching 
//	{
//      for(int iPart=0;iPart<int(partonPt->size());iPart++) {
//  
//        if( partonSt->at(iPart) != 3 ) continue;
//        if( partonPt->at(iPart) < 1. ) continue;
//        if( !( fabs(partonId->at(iPart))<6 || fabs(partonId->at(iPart))>0 || partonId->at(iPart)==21) ) continue;
//        TLorentzVector thisPart;
//        thisPart.SetPtEtaPhiE( partonPt->at(iPart), partonEta->at(iPart), partonPhi->at(iPart), partonE->at(iPart) );
//        float deltaR_part = thisPart.DeltaR(thisJet);
//        if(deltaR_part< deltaR_min) {
//          deltaR_min = deltaR_part;
//          foundPart = iPart;
//        }
//        if( fabs(partonId->at(iPart))==4 && deltaR_part< deltaR_min_charm) {
//          deltaR_min_charm = deltaR_part;
//          foundPart_charm = iPart;
//        }
//        if( fabs(partonId->at(iPart))==5 && deltaR_part< deltaR_min_bottom) {
//          deltaR_min_bottom = deltaR_part;
//          foundPart_bottom = iPart;
//        }
//  
//      }
//  
//  
//      if( deltaR_min_charm<0.3 && foundPart_charm>=0 ) { // priority to charm
//        jetPdgId[0] = partonId->at(foundPart_charm);
//      } else if( deltaR_min_bottom<0.3 && foundPart_bottom>=0 ) { // then to bottom
//        jetPdgId[0] = partonId->at(foundPart_bottom);
//      } else if(deltaR_min < 0.3 && foundPart>=0) {
//        jetPdgId[0] = partonId->at(foundPart);
//      } else {
//        jetPdgId[0] = 0;
//      }
//	}//end matching
	
	jetPdgId[0] = jetFlavor[0];
    }


    int mult = (treeType=="andrea") ? v_jetMultiplicity->at(0) : nCharged_QC[0]+nNeutral_ptCut[0];
    if( !isMC && fabs(jetEta[0])>2.5 ) {
      qglJet[0] = qglc->computeQGLikelihood2012( jetPt[0], jetEta[0], rho, mult-1, jetPtD_QC[0], axis2_QC[0]);
      qglNoMultCorrJet[0] = qglc->computeQGLikelihood2012( jetPt[0], jetEta[0], rho, mult, jetPtD_QC[0], axis2_QC[0]);
      qglCorrJet[0] = qglJet[0];
    } else {
      qglJet[0] = qglc->computeQGLikelihood2012( jetPt[0], jetEta[0], rho, mult, jetPtD_QC[0], axis2_QC[0]);
      qglNoMultCorrJet[0] = qglJet[0];
      qglCorrJet[0] = (fabs(jetEta[0])<1.9) ? qglJet[0] : qglc->computeQGLikelihood2012( jetPt[0], jetEta[0], rho, correctNCharged(nCharged_QC[0],jetEta[0])+nNeutral_ptCut[0], jetPtD_QC[0], axis2_QC[0]);
    }

    std::map<TString,float> variables_MLP;
    variables_MLP["axis1"]=axis1_QC[0];
    variables_MLP["axis2"]=axis2_QC[0];
    variables_MLP["ptD"]=jetPtD_QC[0];
    if( fabs(jetEta[0])<2.5 ) {
      variables_MLP["mult"]=nCharged_QC[0];
    } else {
      if( isMC ) {
        variables_MLP["mult"]=nCharged_ptCut[0]+nNeutral_ptCut[0];
      } else {
        variables_MLP["mult"]=nCharged_ptCut[0]+nNeutral_ptCut[0]-1;
      }
    }
    
    variables_MLP["pt"]=jetPt[0];
    variables_MLP["eta"]=jetEta[0];
    variables_MLP["rho"]=rhoMLP;

    //qglMLPJet[0] = qgmlp->QGvalue(variables_MLP);






    if( selectionType=="DiJet" ) {

      // **** THEN FOR SECOND JET

      if( treeType=="sunil" ) {
        axis1_QC[1] = jetAxis_QC[0][1];
        axis2_QC[1] = jetAxis_QC[1][1];
        rmsCand_QC[1] = (axis1_QC[1]>0. && axis2_QC[1]>0.) ? sqrt( axis1_QC[1]*axis1_QC[1] + axis2_QC[1]*axis2_QC[1] ) : -1.;
      }


      TLorentzVector secondJet;
      secondJet.SetPtEtaPhiE( jetPt[1], jetEta[1], jetPhi[1], jetEnergy[1] );


      if( treeType=="sunil" ) {

        // match to parton:
//	{
//        float deltaR_min = 999.;
//        int foundPart = -1;
//    
//        float deltaR_min_charm = 999.;
//        int foundPart_charm = -1;
//    
//        float deltaR_min_bottom = 999.;
//        int foundPart_bottom = -1;
//    
//    
//        for(int iPart=0;iPart<int(partonPt->size());iPart++) {
//    
//          if( partonSt->at(iPart) != 3 ) continue;
//          if( partonPt->at(iPart) < 1. ) continue;
//          if( !( fabs(partonId->at(iPart))<6 || fabs(partonId->at(iPart))>0 || partonId->at(iPart)==21) ) continue;
//          TLorentzVector thisPart;
//          thisPart.SetPtEtaPhiE( partonPt->at(iPart), partonEta->at(iPart), partonPhi->at(iPart), partonE->at(iPart) );
//          float deltaR_part = thisPart.DeltaR(secondJet);
//          if(deltaR_part< deltaR_min) {
//            deltaR_min = deltaR_part;
//            foundPart = iPart;
//          }
//          if( fabs(partonId->at(iPart))==4 && deltaR_part< deltaR_min_charm) {
//            deltaR_min_charm = deltaR_part;
//            foundPart_charm = iPart;
//          }
//          if( fabs(partonId->at(iPart))==5 && deltaR_part< deltaR_min_bottom) {
//            deltaR_min_bottom = deltaR_part;
//            foundPart_bottom = iPart;
//          }
//    
//        }
//    
//    
//        if( deltaR_min_charm<0.3 && foundPart_charm>=0 ) { // priority to charm
//          jetPdgId[1] = partonId->at(foundPart_charm);
//        } else if( deltaR_min_bottom<0.3 && foundPart_bottom>=0 ) { // then to bottom
//          jetPdgId[1] = partonId->at(foundPart_bottom);
//        } else if(deltaR_min < 0.3 && foundPart>=0) {
//          jetPdgId[1] = partonId->at(foundPart);
//        } else {
//          jetPdgId[1] = 0;
//        }
//	}//end matching	
	jetPdgId[1]=jetFlavor[1];

      } //if sunil

     


      mult = (treeType=="andrea") ? v_jetMultiplicity->at(1) : nCharged_QC[1]+nNeutral_ptCut[1];
      if( !isMC && fabs(jetEta[1])>2.5 ) {
        qglJet[1] = qglc->computeQGLikelihood2012( jetPt[1], jetEta[1], rho, mult-1, jetPtD_QC[1], axis2_QC[1]);
        qglNoMultCorrJet[1] = qglc->computeQGLikelihood2012( jetPt[1], jetEta[1], rho, mult, jetPtD_QC[1], axis2_QC[1]);
        qglCorrJet[1] = qglJet[1];
      } else {
        qglJet[1] = qglc->computeQGLikelihood2012( jetPt[1], jetEta[1], rho, mult, jetPtD_QC[1], axis2_QC[1]);
        qglNoMultCorrJet[1] = qglJet[1];
        qglCorrJet[1] = (fabs(jetEta[1])<1.9) ? qglJet[1] : qglc->computeQGLikelihood2012( jetPt[1], jetEta[1], rho, correctNCharged(nCharged_QC[1],jetEta[1])+nNeutral_ptCut[1], jetPtD_QC[1], axis2_QC[1]);
      }

      std::map<TString,float> variables_MLP;
      variables_MLP["axis1"]=axis1_QC[1];
      variables_MLP["axis2"]=axis2_QC[1];
      variables_MLP["ptD"]=jetPtD_QC[1];
      if( fabs(jetEta[1])<2.5 ) {
        variables_MLP["mult"]=nCharged_QC[1];
      } else {
        if( isMC ) {
          variables_MLP["mult"]=nCharged_ptCut[1]+nNeutral_ptCut[1];
        } else {
          variables_MLP["mult"]=nCharged_ptCut[1]+nNeutral_ptCut[1]-1;
        }
      }
      
      variables_MLP["pt"]=jetPt[1];
      variables_MLP["eta"]=jetEta[1];
      variables_MLP["rho"]=rhoMLP;

      //qglMLPJet[1] = qgmlp->QGvalue(variables_MLP);

    } //second jet in dijets


    flatTree->Fill();

  } // for entries


std::cout<<"Efficiency "<<double(flatTree->GetEntries())/double(sunilTree->GetEntries())<<std::endl;
	long P=0;
	for(std::map<string,long>::iterator it=counter.begin();it!=counter.end();it++)
		{
		long N=it->second;
		if(P!=0)
			{
			std::cout<<"--> Eff Cut "<<it->first<<" "<<double(N)/double(P)<<std::endl;
			}
		P=N;
		}
  newFile->cd();
  flatTree->Write();
  newFile->Close();

  return 0;

}




float get_xSecWeight( const std::string& dataset ) {

  float neventsGen;
  float xsec;

  if( dataset=="Zjets_Hpp_Pt15to20" ) {
    neventsGen = 200000.;
    xsec = 70.55;
  } else if( dataset=="Zjets_Hpp_Pt20to30" ) {
    neventsGen = 150154.;
    xsec = 77.535;
  } else if( dataset=="Zjets_Hpp_Pt30to50" ) {
    neventsGen = 150000.;
    xsec = 62.75;
  } else if( dataset=="Zjets_Hpp_Pt50to80" ) {
    neventsGen = 100160;
    xsec = 28.74;
  } else if( dataset=="Zjets_Hpp_Pt80to120" ) {
    neventsGen = 96000;
    xsec = 9.75;
  } else if( dataset=="Zjets_Hpp_Pt120to170" ) {
    neventsGen = 98560;
    xsec = 2.81;
  } else if( dataset=="Zjets_Hpp_Pt170to230" ) {
    neventsGen = 100000;
    xsec = 0.77;
  } else if( dataset=="Zjets_Hpp_Pt230to300" ) {
    neventsGen = 96640;
    xsec = 0.21;
  } else if( dataset=="Zjets_Hpp_Pt300" ) {
    neventsGen = 90517;
    xsec = 0.089;
  } else { 
    neventsGen=1.;
    xsec=1.;
  }

  return xsec/neventsGen;

}


float correctNCharged(int nChg_QCJet, float eta) {

  float scale = 1.;

  if( fabs(eta)<2.4 && fabs(eta)>1.9 ) {
    // compute the area of the cone outside of the tracker:
    // using the notation found here: http://upload.wikimedia.org/wikipedia/commons/f/fb/Circularsegment.svg
    float R = 0.5;
    float h = fabs(eta)+0.5-2.4;
    float d = R-h;
    float theta = 2.*TMath::ACos(d/R);
    float area_tot = R*R*TMath::Pi();
    float area_outside = R*R*(theta - sin(theta))/2.;
    float fraction_area = area_outside/area_tot;
    scale = 1./(1.-fraction_area);
  }

  return scale*(float)nChg_QCJet;

}

