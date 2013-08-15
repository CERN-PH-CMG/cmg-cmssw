#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
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
  TH1F *h=(TH1F*) mon.addHistogram( new TH1F ("eventflow", ";Step;Events", 4,0,4) );
  h->GetXaxis()->SetBinLabel(1,"M_{ll}>50");
  h->GetXaxis()->SetBinLabel(2,"3^{rd}-lepton veto");
  h->GetXaxis()->SetBinLabel(3,"b-veto");
  h->GetXaxis()->SetBinLabel(4,"#delta #phi(jet,E_{T}^{miss})>0.5");
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 100,40,250) );
  mon.addHistogram( new TH1F("nvtx",";Vertices;Events",50,0,50) ); 

  std::map<TString,TString> metTypes;
  metTypes["met"]                = "E_{T}^{miss}";
  metTypes["centralMet"]         = "central-E_{T}^{miss}";
  metTypes["assocChargedMet"]    = "assoc-E_{T}^{miss}(charged)";
  metTypes["assocMet"]           = "assoc-E_{T}^{miss}";
  metTypes["clusteredMet"]       = "clustered-E_{T}^{miss}";
  metTypes["minAssocChargedMet"] = "min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
  metTypes["minAssocMet"]        = "min(E_{T}^{miss},assoc-E_{T}^{miss})";
  metTypes["min3Met"]            = "min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redMet"]             = "red(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["red3Met"]            = "red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["projMet"]            = "proj-E_{T}^{miss}";
  metTypes["minProjAssocChargedMet"] = "min(proj-E_{T}^{miss},proj-assoc-E_{T}^{miss}(charged))";
  metTypes["minProjAssocMet"]        = "min(proj-E_{T}^{miss},proj-assoc-E_{T}^{miss})";
  for(int ireg=0; ireg<2; ireg++)
    {
      TString reg(ireg==0 ? "" : "sideband");
      mon.addHistogram( new TH1F( "zeta"+reg, ";#eta^{ll};Events", 50,-10,10) );
      mon.addHistogram( new TH1F( "zpt"+reg, ";p_{T}^{ll};Events", 50,0,500) );
      mon.addHistogram( new TH1F("njets"+reg ,";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
      for(size_t ibin=1; ibin<=5; ibin++){
	TString label("");
	if(ibin==5) label +="#geq";
	else        label +="=";
	label += (ibin-1);
	mon.getHisto("njets"+reg)->GetXaxis()->SetBinLabel(ibin,label);
      }

      mon.addHistogram( new TH1F( "met_"+reg+"redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
      mon.addHistogram( new TH1F( "met_"+reg+"redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
      mon.addHistogram( new TH2F( "met_"+reg+"metvsassocChargedMet", ";E_{T}^{miss};assoc-E_{T}^{miss}(charged)", 50,0,500,50,0,500) );
      mon.addHistogram( new TH2F( "met_"+reg+"metvsassocMet", ";E_{T}^{miss};assoc-E_{T}^{miss}", 50,0,500,50,0,500) );
      mon.addHistogram( new TH2F( "met_"+reg+"metvsclusteredMet", ";E_{T}^{miss};clustered-E_{T}^{miss}", 50,0,500,50,0,500) );
      mon.addHistogram( new TH2F( "met_"+reg+"metvscentralMet", ";E_{T}^{miss};central-E_{T}^{miss}", 50,0,500,50,0,500) );
      for(std::map<TString,TString>::iterator it = metTypes.begin(); it != metTypes.end(); it++)
	{
	  mon.addHistogram( new TH1F( "met_"+reg+it->first  , ";"+it->second+";Events", 50,0,500) );
	  mon.addHistogram( new TH2F( "met_"+reg+it->first+"_vspu", ";Pileup events;"+it->second+";Events", 35,0,35,200,0,500) );
	}
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
  EventCategory eventCategoryInst(2); //vbf binning
  //EventCategory eventCategoryInst(3); //jet+vbf binning


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
      LorentzVector zll(lep1+lep2);
      if(!isMC)
	{
	  //apply regression corrections for SC energy
	  float l1corr( fabs(phys.leptons[0].id)==ELECTRON ? phys.leptons[0].ensf : 1.0); if(l1corr==0) l1corr=1;
	  float l2corr( fabs(phys.leptons[1].id)==ELECTRON ? phys.leptons[1].ensf : 1.0); if(l2corr==0) l2corr=1;
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
      LorentzVector assocMetP4(phys.met[6]);//phys.met[1]);      
      LorentzVector assocChargedMetP4(phys.met[5]);
      LorentzVector centralMetP4(phys.met[3]);
      LorentzVectorCollection zvvs;
      std::vector< std::vector<PhysicsObject_Jet> > jets;
      //std::vector<LorentzVectorCollection> jets;
      METUtils::computeVariation(phys.ajets, phys.leptons, zvvRaw, jets, zvvs, &jecUnc);
      LorentzVector zvv(zvvs[0]);
      LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
      int njets(0),nbtags(0);
      float mindphijmet(9999.);
      for(size_t ijet=0; ijet<jets[0].size(); ijet++)
	{
	  clusteredMetP4 -= jets[0][ijet];
	  nbtags +=(phys.ajets[ijet].btag2>0.244);
	  if(jets[0][ijet].pt()>30) njets++;
	  float idphijmet=fabs(deltaPhi(zvv.phi(),jets[0][ijet].phi()));
	  mindphijmet=min(idphijmet,mindphijmet);
	}
      cout << zvv.pt() << " " << phys.met[0].pt() << endl;
      METUtils::stRedMET redMetOut; 
      LorentzVector redMet    = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4, zvv,false,&redMetOut);
      double redMetL          = redMetOut.redMET_l;
      double redMetT          = redMetOut.redMET_t;
      LorentzVector red3Met   = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4 , clusteredMetP4, zvv, false);
      LorentzVector projMet   = METUtils::projectedMET(lep1,lep2,zvv);
      LorentzVector projACMet = METUtils::projectedMET(lep1,lep2,assocChargedMetP4);
      LorentzVector projAMet  = METUtils::projectedMET(lep1,lep2,assocMetP4);

      std::map<TString,LorentzVector> mets;
      mets["met"]                = zvv;
      mets["centralMet"]         = centralMetP4;
      mets["assocChargedMet"]    = assocChargedMetP4;
      mets["assocMet"]           = assocMetP4;
      mets["clusteredMet"]       = clusteredMetP4;
      mets["minAssocChargedMet"] = min(zvv,assocChargedMetP4);
      mets["minAssocMet"]        = min(zvv,assocMetP4);
      mets["min3Met"]            = min(zvv,min(assocMetP4,clusteredMetP4));
      mets["redMet"]             = redMet;
      mets["red3Met"]            = red3Met;
      mets["projMet"]            = projMet;
      mets["minProjAssocChargedMet"] = min(projMet,projACMet);
      mets["minProjAssocMet"]        = min(projMet,projAMet);

      
      //kinematics
      Float_t zmass=zll.mass();
      Float_t zpt=zll.pt();
      Float_t zeta=zll.eta();
      if(zmass<50) continue; // this is required by the HZZ skim anyhow
      bool passZmass(fabs(zmass-91)<15);
      bool passDphijmet(mindphijmet>0.5); 
      bool pass3dLeptonVeto(true); int nExtraLep(0); for(unsigned int i=2;i<phys.leptons.size();i++) { if(phys.leptons[i].pt()>10){ nExtraLep++; pass3dLeptonVeto=false;} }
      bool passBveto(nbtags==0);
      float iweight=weight;

      mon.fillHisto  ("eventflow",tags_full,0,iweight);
      mon.fillHisto  ("zmass"    ,tags_full,zmass,iweight);
      mon.fillHisto  ("nvtx"     ,tags_full,ev.nvtx,iweight);

      TString reg(passZmass?"":"sideband");      
      mon.fillHisto("zeta"+reg     ,tags_full,zeta   ,iweight);
      mon.fillHisto("zpt"+reg      ,tags_full,zpt     ,iweight);

      if(pass3dLeptonVeto){
	mon.fillHisto("eventflow",tags_full,1,iweight);
	if(passBveto){
	  mon.fillHisto("eventflow",tags_full,2,iweight);
	  mon.fillHisto("njets"+reg,tags_full,njets,iweight);
	  mon.fillHisto("mindphijmet"+reg,tags_full,mindphijmet,iweight);
	  if(passDphijmet){
	    mon.fillHisto("eventflow",tags_full,3,iweight);
	    for(std::map<TString,LorentzVector>::iterator it = mets.begin(); it!= mets.end(); it++)
	      {
		mon.fillHisto("met_"+reg+it->first,tags_full,it->second.pt(),iweight);
		mon.fillHisto("met_"+reg+it->first+"_vspu",tags_full,ev.ngenITpu,it->second.pt(),iweight);
	      }
	    mon.fillHisto("met_"+reg+"metvsassocChargedMet", tags_full,mets["met"].pt(),mets["assocChargedMet"].pt(),iweight);
	    mon.fillHisto("met_"+reg+"metvsassocMet",        tags_full,mets["met"].pt(),mets["assocMet"].pt(),       iweight);
	    mon.fillHisto("met_"+reg+"metvsclusteredMet",    tags_full,mets["met"].pt(),mets["clusteredMet"].pt(),   iweight);
	    mon.fillHisto("met_"+reg+"metvscentralMet",      tags_full,mets["met"].pt(),mets["centralMet"].pt(),     iweight);
	    
	    mon.fillHisto("met_"+reg+"redMetL",tags_full,redMetL,iweight);
	    mon.fillHisto("met_"+reg+"redMetT",tags_full,redMetT,iweight);
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





