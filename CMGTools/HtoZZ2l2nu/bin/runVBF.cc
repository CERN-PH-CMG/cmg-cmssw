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
 
  //Renormalization
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ) ;

  
  //VBF
  TH1F* h;
  h = new TH1F ("NEventsInc", ";Selection cut;Events", 12,0,12);
  h->GetXaxis()->SetBinLabel(1,"All");
  h->GetXaxis()->SetBinLabel(2,"2 highPt-Jets");
  h->GetXaxis()->SetBinLabel(3,"dEta");
  h->GetXaxis()->SetBinLabel(4,"inv. Mass");
  h->GetXaxis()->SetBinLabel(5,"Central lepton");
  h->GetXaxis()->SetBinLabel(6,"Jet Veto");
  h->GetXaxis()->SetBinLabel(7,"BJet Veto");
  h->GetXaxis()->SetBinLabel(8,"3rd Lepton Veto");
  h->GetXaxis()->SetBinLabel(9,"central dilepton");
  h->GetXaxis()->SetBinLabel(10,"met>50GeV");
  h->GetXaxis()->SetBinLabel(11,"met>60GeV");
  h->GetXaxis()->SetBinLabel(12,"met>70GeV");
  mon.addHistogram( h );
  mon.addHistogram( new TH1F ("MinEta", ";VBFMinEta", 100, 0,10) );                                                                                                                                                             
  mon.addHistogram( new TH1F ("MaxEta", ";VBFMaxEta", 100, 0,10) );     
  mon.addHistogram( new TH2F ("MinEtavspu", ";VBFMinEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH2F ("MaxEtavspu", ";VBFMaxEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH2F ("ThirdJetPtvspu", ";VBFThirdJetPtvspu",35, 0, 35, 100, 0,30) );
  mon.addHistogram( new TH2F ("ThirdJetEtavspu", ";VBFThirdJetEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH1F ("dEtaInc", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("iMassInc",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("cenLeptonVetoInc",";Central Lepton Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("cen30JetVetoInc",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("NBJets30Inc", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH1F ("dEtaNM1C", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("iMassNM1C",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("cenLeptonVetoNM1C",";Central Lepton Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("cen30JetVetoNM1C",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("NBJets30NM1C", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH2F ("dEtaiMassNM1C", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  mon.addHistogram( new TH2F ("dEtaiMassNM1C2", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  mon.addHistogram( new TH1F ("totalvspu", ";#generated vertices", 35, 0,35) );                        
  mon.addHistogram( new TH1F ("tagvspu_noveto", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("tagvspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("tag3rd20vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("tag3rd25vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("totalvspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("tagvspu_noveto_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("tagvspu_cen", ";#generated vertices", 35, 0,35) ); 
  mon.addHistogram( new TH1F ("totalvspu_fwd", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("tagvspu_fwd", ";#generated vertices", 35, 0,35) ); 

  mon.addHistogram( new TH2F ("dEtavspu", ";#generated vertices;#Delta #eta", 35, 0,35,  60, -6,6) );

  mon.addHistogram( new TH2F ("NCenJet15vspu", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("NCenJet30vspu", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );

  mon.addHistogram( new TH1F ("NJets15", ";NJets15", 20, 0,20) );
  mon.addHistogram( new TH2F ("NJets15vspu", ";NJets15vspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("NJets30", ";NJets30", 20, 0,20) );
  mon.addHistogram( new TH2F ("NJets30vspu", ";NJets30vspu", 35, 0,35, 40, 0,40) );

  mon.addHistogram( new TH1F ("JetEta15", ";JetEta15", 100, -5,  5) );
  mon.addHistogram( new TH2F ("JetEta15vspu", ";JetEta15vspu", 35, 0,35, 100, -5,5) );
  mon.addHistogram( new TH1F ("JetEta30", ";JetEta30", 100, -5,  5) );
  mon.addHistogram( new TH2F ("JetEta30vspu", ";JetEta15vspu", 35, 0,35, 100, -5,5) );

  mon.addHistogram( new TH2F ("JetPtvsZpT", ";zpT,JetpT", 10, 0,  100, 25, 30, 90) );


  mon.addHistogram( new TH1F ("pumva", ";pumva", 50, -1,1) );
  mon.addHistogram( new TH2F ("pumvavspu", ";pumvavspu", 35, 0,35, 50, -1,1) );





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
      
      //categorize events
      TString tag_cat;
      switch(ev.cat){
      case EMU : tag_cat = "emu";   continue; break; //skip emu events
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
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
	{
	  jetsP4.push_back( phys.jets[ijet] );
	  genJetsPt.push_back( phys.jets[ijet].genPt);
	}
      //base raw METs
      LorentzVector rawZvv(phys.met[0]);
      LorentzVector rawClusteredMet(zll);            rawClusteredMet *= -1;
      for(size_t ijet=0; ijet<jetsP4.size(); ijet++) rawClusteredMet -= jetsP4[ijet];
      LorentzVector rawRedMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, rawClusteredMet, rawZvv,false));
      Float_t rawMt( METUtils::transverseMass(zll,rawZvv,true) );
      
      //prepare variations (first variation is the baseline, corrected for JER) 
      LorentzVectorCollection zvvs,redMets;
      std::vector<Float_t>  mts,mt3s,redMetLs,redMetTs;
      std::vector<LorentzVectorCollection> jets;
      METUtils::computeVariation(jetsP4, genJetsPt, rawZvv, jets, zvvs, &jecUnc);
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
	
	Float_t zmassraw=zllraw.mass();
	Float_t zmass=zll.mass();
	Float_t zpt=zll.pt();
	Float_t zeta=zll.eta();
	LorentzVectorCollection &origJetsP4=jets[0];            
	LorentzVector zvv    = zvvs[0];
	LorentzVector redMet = redMets[0];
	Float_t redMetL      = redMetLs[0];
	Float_t redMetT      = redMetTs[0];
	Float_t mt           = mts[0];
	Float_t mt3          = mt3s[0]; 
	int njets(0),nbtags(0);
	Float_t mindphijmet(9999.);
	Float_t btagcut(2.0); 
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
	  
	  //Fill histogram for posterior optimization, or for control regions
        bool passPreselection             (passZmass && passZpt && pass3dLeptonVeto && passDphijmet && passBveto);
        float iweight = weight;                                               //nominal


      //##############################################
      //########           VBF PLOTS          ########
      //##############################################


      for(int jetColl=0;jetColl<4;jetColl++){
         TString jetCollName="";
         PhysicsObjectJetCollection jets;
         if(jetColl%2==0){
            jetCollName="_STD";
            jets = phys.ajets;               
         }else{
            jetCollName="_CHS";
            jets = phys.jets;
         }

         if(jetColl>=2){
            jetCollName += "andZPT";
         }


         //prepare the tag's vectors for histo filling
         std::vector<TString> tags_full;
         tags_full.push_back(TString("all") + jetCollName);
         tags_full.push_back(tag_cat + jetCollName);

         bool isVBF        = false;
         bool VBFPass2Jet30   = false;
         bool VBFPassdEtaCut  = false;
         bool VBFPassiMCut    = false;
         bool VBFPassBJetVeto = false;
         bool VBFPassJetVeto  = false;
         bool VBFPassLeptonIn = false;
         bool VBFPassCentralDilep = false;

         double  VBFMinEta = 0;                                                                                                                                                                                                                 
         double  VBFMaxEta = 0;       
         double  VBFdEta = -1;
         int     VBFCentral15Jets = 0;
         int     VBFCentral30Jets = 0;
         int     VBFCentralLeptons = 0;
         int     VBFNBJets=0;
         LorentzVector VBFSyst;

//         if(passZmass && passZpt && pass3dLeptonVeto){
         if(passZmass && pass3dLeptonVeto && (jetColl<2 || (jetColl>=2 && passZpt))){
            int NJets15 = 0;
            int NJets30 = 0;
            for(size_t ijet=0; ijet<jets.size(); ijet++){
               PhysicsObject_Jet jet = jets[ijet]; 

               if(jet.pt()<15)continue;
               NJets15++;
               mon.fill1DHisto("JetEta15", tags_full, jet.eta(), iweight);
               mon.fill2DHisto("JetEta15vspu", tags_full, ev.ngenITpu, jet.eta(), iweight);

               if(jet.pt()<30)continue;
               NJets30++;
               mon.fill1DHisto("JetEta30", tags_full, jet.eta(), iweight);
               mon.fill2DHisto("JetEta30vspu", tags_full, ev.ngenITpu, jet.eta(), iweight);

               mon.fill2DHisto("JetPtvsZpT",  tags_full, zpt, jet.pt(), iweight);
            } 
            mon.fill1DHisto("NJets15"      ,tags_full, NJets15, iweight);
            mon.fill1DHisto("NJets30"      ,tags_full, NJets30, iweight);
            mon.fill2DHisto("NJets15vspu",tags_full, ev.ngenITpu,  NJets15 ,iweight);
            mon.fill2DHisto("NJets30vspu",tags_full, ev.ngenITpu,  NJets30 ,iweight);


            for(unsigned int vbfSelection=0;vbfSelection<3;vbfSelection++){
               TString vbfSelName="";
               switch(vbfSelection){
                  case 0 : vbfSelName="_LooseVBF"  ;break;
                  case 1 : vbfSelName="_MediumVBF" ;break;
                  case 2 : vbfSelName="_TightVBF"  ;break;
		  default: vbfSelName="_UnknownVBF";break;
               }

               std::vector<TString> tags_full2;
               for(unsigned int i=0;i<tags_full.size();i++){tags_full2.push_back(tags_full[i]+vbfSelName); }



               if(jets.size()>=2){
                   VBFSyst =  jets[0] + jets[1];
                   VBFdEta = fabs(   jets[0].eta() -    jets[1].eta()); if(jets[0].eta()* jets[1].eta()>0)VBFdEta*=-1;
                   double MinEta, MaxEta;

                   if(jets[0].eta()<jets[1].eta()){MinEta=jets[0].eta(); MaxEta=jets[1].eta();}else{MinEta=jets[1].eta(); MaxEta=jets[0].eta();}
                   if(isGammaEvent){  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
                   }else{             if(zll.eta()>MinEta && zll.eta()<MaxEta) VBFCentralLeptons=2;
                   }

                   for(size_t ijet=2; ijet<jets.size(); ijet++){
                       if(jets[ijet].pt()<15)continue;
                       if(jets[ijet].eta()>MinEta && jets[ijet].eta()<MaxEta)VBFCentral15Jets++;
                       if(jets[ijet].pt()<30)continue; 
                       if(jets[ijet].eta()>MinEta && jets[ijet].eta()<MaxEta)VBFCentral30Jets++; 
                       if(jets[ijet].btag1>1.7) VBFNBJets++;
                   }

                   switch(vbfSelection){
                      case 2:{
                         VBFPass2Jet30   = (jets [0].pt()>30.0 && jets [1].pt()>30.0);
                         VBFPassdEtaCut  = VBFPass2Jet30 && (fabs(VBFdEta)>4.0);
                         VBFPassiMCut    = VBFPass2Jet30 && (VBFSyst.M()>500);
                      }break;

                      case 1:{
                         VBFPass2Jet30   = (jets [0].pt()>30.0 && jets [1].pt()>30.0);
                         VBFPassdEtaCut  = VBFPass2Jet30 && (fabs(VBFdEta)>3.5);
                         VBFPassiMCut    = VBFPass2Jet30 && (VBFSyst.M()>350); 
                      }break;

                      default:{
                         VBFPass2Jet30   = (jets [0].pt()>25.0 && jets [1].pt()>25.0);
                         VBFPassdEtaCut  = VBFPass2Jet30 && (fabs(VBFdEta)>3.5);
                         VBFPassiMCut    = VBFPass2Jet30 && (VBFSyst.M()>340);
                         }break;
                   }

                   VBFPassLeptonIn = (VBFCentralLeptons==2);
                   VBFPassJetVeto  = (VBFCentral30Jets==0);
                   VBFPassBJetVeto = (VBFNBJets==0);
                   VBFPassCentralDilep = (zll.eta()>MinEta && zll.eta()<MaxEta);
                   isVBF        = (VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto && VBFPassJetVeto && VBFPassLeptonIn && VBFPassCentralDilep);
                   VBFMinEta = std::min(fabs(jets[0].eta()), fabs(jets[1].eta()));
                   VBFMaxEta = std::max(fabs(jets[0].eta()), fabs(jets[1].eta()));

                   if(VBFPassdEtaCut && VBFPassiMCut){
                       mon.fill1DHisto("pumva", tags_full2, jets[0].pumva,iweight);
                       mon.fill2DHisto("pumvavspu", tags_full2, ev.ngenITpu,  jets[0].pumva,iweight);
                   }

              }

 
                   //VBF control
                if(true                                                                                        )mon.fill1DHisto("NEventsInc"       ,tags_full2,    0                ,iweight);
                if(VBFPass2Jet30                                                                                  )mon.fill1DHisto("NEventsInc"       ,tags_full2,    1                ,iweight);
                if(VBFPassdEtaCut                                                                                 )mon.fill1DHisto("NEventsInc"       ,tags_full2,    2                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fill1DHisto("NEventsInc"       ,tags_full2,    3                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fill1DHisto("NEventsInc"       ,tags_full2,    4                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fill1DHisto("NEventsInc"       ,tags_full2,    5                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fill1DHisto("NEventsInc"       ,tags_full2,    6                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto )mon.fill1DHisto("NEventsInc"       ,tags_full2,    7                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep)mon.fill1DHisto("NEventsInc"       ,tags_full2,    8                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && zvv.pt()>50)mon.fill1DHisto("NEventsInc"       ,tags_full2,    9                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && zvv.pt()>60)mon.fill1DHisto("NEventsInc"       ,tags_full2,    10                ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && zvv.pt()>70)mon.fill1DHisto("NEventsInc"       ,tags_full2,    11                ,iweight);


                if(VBFPass2Jet30                                                                                  )mon.fill1DHisto("dEtaInc"          ,tags_full2,    fabs(VBFdEta)    ,iweight);
                if(VBFPassdEtaCut                                                                                 )mon.fill1DHisto("iMassInc"         ,tags_full2,    VBFSyst.M()      ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fill1DHisto("cenLeptonVetoInc" ,tags_full2,    VBFCentralLeptons,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fill1DHisto("cen30JetVetoInc"  ,tags_full2,    VBFCentral30Jets ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fill1DHisto("NBJets30Inc"      ,tags_full2,    VBFNBJets        ,iweight);
    
                if(               VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fill1DHisto("dEtaNM1C"         ,tags_full2,    fabs(VBFdEta)    ,iweight);
                if(VBFPassdEtaCut              && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fill1DHisto("iMassNM1C"        ,tags_full2,    VBFSyst.M()      ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut                 && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fill1DHisto("cenLeptonVetoNM1C",tags_full2,    VBFCentralLeptons,iweight);
   
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                && VBFPassBJetVeto )mon.fill1DHisto("cen30JetVetoNM1C" ,tags_full2,    VBFCentral30Jets ,iweight);
                if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                 )mon.fill1DHisto("NBJets30NM1C"     ,tags_full2,    VBFNBJets        ,iweight);
                if(                            VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto )mon.fill2DHisto("dEtaiMassNM1C"  ,tags_full2,    fabs(VBFdEta), VBFSyst.M(), iweight);
                if(zvv.pt()>50 &&                VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto )mon.fill2DHisto("dEtaiMassNM1C2" ,tags_full2,    fabs(VBFdEta), VBFSyst.M(), iweight);


                mon.fill1DHisto("totalvspu"      ,tags_full2,    ev.ngenITpu   ,iweight);
                if(fabs(VBFMinEta)<2.4){
                   mon.fill1DHisto("totalvspu_cen"      ,tags_full2,    ev.ngenITpu   ,iweight);
                }else{
                   mon.fill1DHisto("totalvspu_fwd"      ,tags_full2,    ev.ngenITpu   ,iweight);
                }

   	           if(VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut){
                   mon.fill1DHisto("tagvspu_noveto"      ,tags_full2,    ev.ngenITpu   ,iweight);

                   if(fabs(VBFMinEta)<2.4){
                      mon.fill1DHisto("tagvspu_noveto_cen"      ,tags_full2,    ev.ngenITpu   ,iweight);
                   }else{
                      mon.fill1DHisto("tagvspu_noveto_fwd"      ,tags_full2,    ev.ngenITpu   ,iweight);
                   } 
                }    

                if(isVBF){
                   mon.fill1DHisto("MinEta"      ,tags_full2,                VBFMinEta ,iweight);  
                   mon.fill2DHisto("MinEtavspu"  ,tags_full2, ev.ngenITpu,   VBFMinEta ,iweight);
                   mon.fill1DHisto("MaxEta"      ,tags_full2,                VBFMaxEta ,iweight);
                   mon.fill2DHisto("MaxEtavspu"  ,tags_full2, ev.ngenITpu,   VBFMaxEta ,iweight);

                   if(jets.size()>2){
                       mon.fill2DHisto("ThirdJetPtvspu"  ,tags_full2, ev.ngenITpu, jets[2].pt()   ,iweight);
                       if(jets[2].pt()>20){
                       mon.fill2DHisto("ThirdJetEtavspu"  ,tags_full2, ev.ngenITpu, fabs(jets[2].eta())   ,iweight);
                      }
                   }

                   mon.fill1DHisto("tagvspu"      ,tags_full2,    ev.ngenITpu   ,iweight); 
                   if(fabs(VBFMinEta)<2.4){
                      mon.fill1DHisto("tagvspu_cen"      ,tags_full2,    ev.ngenITpu   ,iweight);
                   }else{
                      mon.fill1DHisto("tagvspu_fwd"      ,tags_full2,    ev.ngenITpu   ,iweight);
                   }

                   if(jets.size()<=2 || jets[2].pt()<=20)mon.fill1DHisto("tag3rd20vspu"      ,tags_full2,    ev.ngenITpu   ,iweight);
                   if(jets.size()<=2 || jets[2].pt()<=25)mon.fill1DHisto("tag3rd25vspu"      ,tags_full2,    ev.ngenITpu   ,iweight);
                }


                if(VBFPass2Jet30 && VBFPassiMCut)mon.fill2DHisto("dEtavspu", tags_full2, ev.ngenITpu, VBFdEta,iweight);

                if(VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto){
                   mon.fill2DHisto("NCenJet15vspu"  , tags_full2, ev.ngenITpu, VBFCentral15Jets,iweight);
                   mon.fill2DHisto("NCenJet30vspu"  , tags_full2, ev.ngenITpu, VBFCentral30Jets,iweight);
                }    

            }//VBF selection cut

         }//end preselection

      }//end of jetColl loop
      
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





