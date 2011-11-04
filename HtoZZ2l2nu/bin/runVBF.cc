#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/TransverseMassComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaPhi.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "Math/GenVector/Boost.h"

using namespace std;

//
int main(int argc, char* argv[])
{
 // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }
  
  //configure
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //control Histos
  SelectionMonitor controlHistos;

  //the Higgs pT
  TString hptvars[]={"","std","renup","rendown","facup","facdown"};
  Float_t hptaxis[]={0,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,15.,20.,25.,30.,35.,40.,45.,50.,
		     60.,70.,80.,100.,150,200.,250.,300.,350.,400.,500,600.,700.,800.,1000.};
  for(size_t ivar=0; ivar<sizeof(hptvars)/sizeof(TString); ivar++)
    controlHistos.addHistogram(  new TH1D ("hpt"+hptvars[ivar], ";p_{T}^{H};Events", sizeof(hptaxis)/sizeof(Float_t)-1,hptaxis) );
  
  TString tiers[]={"gen","reco"};
  for(size_t itier=0; itier<2; itier++)
    {
      controlHistos.addHistogram( new TH1D (tiers[itier]+"zpt", ";"+tiers[itier]+" p_{T}^{ll};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"zvvpt", ";"+tiers[itier]+" p_{T}^{#nu#nu};Events", 100,0,400) );
      controlHistos.addHistogram( new TH1D (tiers[itier]+"njets", ";"+tiers[itier]+" Jets;Jets",4,0,4) );

      if(itier)
	{
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelSTD", ";"+tiers[itier]+" Selection;Events", 11, 0.,11.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelSTDMet", ";"+tiers[itier]+" red-met;Events", 100, 0.,500.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelSTDMT", ";"+tiers[itier]+" MT;Events", 100, 0.,1000.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW", ";"+tiers[itier]+" Selection;Events", 11, 0.,11.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEWMet", ";"+tiers[itier]+" red-met;Events", 100, 0.,500.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEWMT", ";"+tiers[itier]+" MT;Events", 100, 0.,1000.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW2", ";"+tiers[itier]+" Selection;Events", 11, 0.,11.) ); 
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW2Met", ";"+tiers[itier]+" red-met;Events", 100, 0.,500.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW2MT", ";"+tiers[itier]+" MT;Events", 100, 0.,1000.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW3", ";"+tiers[itier]+" Selection;Events", 11, 0.,11.) ); 
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW3Met", ";"+tiers[itier]+" red-met;Events", 100, 0.,500.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW3MT", ";"+tiers[itier]+" MT;Events", 100, 0.,1000.) );


	  controlHistos.addHistogram( new TH1D (tiers[itier]+"mt", ";"+tiers[itier]+" M_{T};Events", 100, 100.,900.) );
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"prefrecoil", ";"+tiers[itier]+" h_{T};Events", 100,0,200) );
	  controlHistos.addHistogram( new TH1D (tiers[itier]+"cmmt", ";"+tiers[itier]+" M_{T}(pCM);Events", 100, 100.,900.) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzzdeltapt", ";p_{T}^{ll}-p_{T}^{vv}(pCM)", 100,-250,250) );
          controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzzsumpt", ";p_{T}^{ll}+p_{T}^{vv}(pCM)", 100,0,1000) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzllpt", ";p_{T}^{ll}(pCM)", 100,0,500) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzvvpt", ";p_{T}^{#nu#nu}(pCM)", 100,0,500) );
          controlHistos.addHistogram(  new TH2D (tiers[itier]+"cmzllptvszvvpt", ";p_{T}^{ll}(pCM);p_{T}^{#nu#nu}(pCM)", 100,0,500, 100,0,500) );


	  controlHistos.addHistogram( new TH1D (tiers[itier]+"cmmt400", ";"+tiers[itier]+" M_{T}(pCM 400);Events", 100, 100.,900.) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzzdeltapt400", ";p_{T}^{ll}-p_{T}^{vv}(pCM 400)", 100,-250,250) );
          controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzzsumpt400", ";p_{T}^{ll}+p_{T}^{vv}(pCM)", 100,0,1000) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzllpt400", ";p_{T}^{ll}(pCM 400)", 100,0,500) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"cmzvvpt400", ";p_{T}^{#nu#nu}(pCM 400)", 100,0,500) );
          controlHistos.addHistogram(  new TH2D (tiers[itier]+"cmzllptvszvvpt400", ";p_{T}^{ll}(pCM);p_{T}^{#nu#nu}(pCM)", 100,0,500, 100,0,500) );

	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"dilcmzvvpt", ";red E_{T}^{miss}(dilepton CM)", 100,0,250) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"dilcmdphistar", ";#Delta#phi(red E_{T}^{miss},p_{T,lab}^{Z})", 100,-3.2,3.2) );
	  controlHistos.addHistogram(  new TH1D (tiers[itier]+"dilcmdphistarrecoil", ";#Delta#phi(red E_{T}^{miss},h_{T,lab})", 100,-3.2,3.2) );

          controlHistos.addHistogram(  new TH2D (tiers[itier]+"zllptvszvvpt", ";p_{T}^{ll};p_{T}^{#nu#nu}", 100,0,500, 100,0,500) );



          controlHistos.addHistogram( new TH1F (tiers[itier]+"pfmet", ";"+tiers[itier]+" pf-E_{T}^{miss};Events", 100, 0.,500.) ); 
	  controlHistos.addHistogram( new TH1F (tiers[itier]+"redmet", ";"+tiers[itier]+" red-E_{T}^{miss};Events", 100, 0.,500.) );
	  controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetL", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};Events", 100, -200.,200.) );
	  controlHistos.addHistogram( new TH1F (tiers[itier]+"redmetT", ";"+tiers[itier]+" red-E_{T}^{miss,#perp};Events", 100, -200.,200.) );
	  controlHistos.addHistogram( (TH1D *)(new TH2D (tiers[itier]+"redmetcomps", ";"+tiers[itier]+" red-E_{T}^{miss,#parallel};red-E_{T}^{miss,#perp};Events", 100, -251.,249,100, -251.,249.) ) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"redmet5", ";"+tiers[itier]+" red-E_{T}^{miss};Events", 100, 0.,500.) );


          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFcmmt", ";"+tiers[itier]+" M_{T}(pCM);Events", 100, 100.,900.) );
          controlHistos.addHistogram(  new TH1D (tiers[itier]+"VBFcmdeltazz", ";p_{T}^{ll}-p_{T}^{vv}(pCM)", 100,-250,250) );
          controlHistos.addHistogram(  new TH1D (tiers[itier]+"VBFcmzllpt", ";p_{T}^{ll}(pCM)", 100,0,500) );
          controlHistos.addHistogram(  new TH1D (tiers[itier]+"VBFcmzvvpt", ";p_{T}^{#nu#nu}(pCM)", 100,0,500) );


          //VBF
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFNEvents", ";Selection cut;Events", 10,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"BefVBFNJets30", ";NJets (pT>30);Events", 10,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"BefVBFredmet", ";red-E_{T}^{miss};Events", 100,0,500) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"BefVBFmet", ";E_{T}^{miss};Events", 100,0,500) );


          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFMinEta", ";min(Jet1 |#eta|, Jet2 |#eta|));Events", 25,0,6) );
          controlHistos.addHistogram( new TH2D (tiers[itier]+"VBFEta1Eta2", ";#eta Jet1;#eta Jet2", 50,-5.5,5.5, 50,-5.5,5.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFdEta", ";"+tiers[itier]+"VBF #Delta#eta", 100, 0,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFdEtaSigned", ";"+tiers[itier]+"VBF #Delta#eta x -Sign(VBFEta1*VBFEta2)", 100, -6.,15) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFiMass",";"+tiers[itier]+"VBF Inv. Mass", 100, 0.,2000) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFBTagJet1", ";BTagging Jet1;Events", 100,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFBTagJet2", ";BTagging Jet2;Events", 100,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFNBJets30", ";N BJets (pT>30);Events", 10,0,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFcen30JetVeto",";"+tiers[itier]+"VBF Central 30 Jet Veto", 10, 0.,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFcenLeptonVeto",";"+tiers[itier]+"VBF Central Lepton Veto", 10, 0.,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFredmet", ";red-E_{T}^{miss};Events", 100,0,500) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFmet", ";E_{T}^{miss};Events", 100,0,500) );
          controlHistos.addHistogram( new TH2D (tiers[itier]+"VBFmetvsredmet", ";E_{T}^{miss};red-E_{T}^{miss}", 100,0,500, 100,0,500) );


          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFNEventsInc", ";Selection cut;Events", 10,0,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFiMassInc",";"+tiers[itier]+"VBF Inv. Mass", 100, 0.,2000) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFcen30JetVetoInc",";"+tiers[itier]+"VBF Central 30 Jet Veto", 10, 0.,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFcenLeptonVetoInc",";"+tiers[itier]+"VBF Central Lepton Veto", 10, 0.,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFBTagJet1Inc", ";BTagging Jet1;Events", 100,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFBTagJet2Inc", ";BTagging Jet2;Events", 100,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFNBJets30Inc", ";N BJets (pT>30);Events", 10,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFredmetInc", ";red-E_{T}^{miss};Events", 100,0,500) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFmetInc", ";E_{T}^{miss};Events", 100,0,500) );
          controlHistos.addHistogram( new TH2D (tiers[itier]+"VBFmetvsredmetInc", ";E_{T}^{miss};red-E_{T}^{miss}", 100,0,500, 100,0,500) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFmtInc", ";M_{T};Events", 100, 100.,900.) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFprefrecoilInc", ";h_{T};Events", 100,0,200) );

          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFNEventsNM1C", ";Selection cut;Events", 10,0,10) );
          controlHistos.addHistogram( new TH2D (tiers[itier]+"VBFEta1Eta2NM1C", ";#eta Jet1;#eta Jet2", 50,-5.5,5.5, 50,-5.5,5.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFdEtaNM1C", ";"+tiers[itier]+"VBF #Delta#eta", 100, 0,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFdEtaSignedNM1C", ";"+tiers[itier]+"VBF #Delta#eta x -Sign(VBFEta1*VBFEta2)", 100, -6.,15) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFiMassNM1C",";"+tiers[itier]+"VBF Inv. Mass", 100, 0.,2000) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFBTagJet1NM1C", ";BTagging Jet1;Events", 100,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFBTagJet2NM1C", ";BTagging Jet2;Events", 100,0,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFNBJets30NM1C", ";N BJets (pT>30);Events", 10,0,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFcen30JetVetoNM1C",";"+tiers[itier]+"VBF Central 30 Jet Veto", 10, 0.,10) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFcenLeptonVetoNM1C",";"+tiers[itier]+"VBF Central Lepton Veto", 10, 0.,10) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFredmetNM1C", ";red-E_{T}^{miss};Events", 100,0,500) );
          controlHistos.addHistogram( new TH1D (tiers[itier]+"VBFmetNM1C", ";E_{T}^{miss};Events", 100,0,500) );


          controlHistos.addHistogram( new TH2D (tiers[itier]+"VBFj1j2Phi", ";#phi Jet1;#phi Jet2", 100,-3.5,3.5, 100,-3.5,3.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl1l2dPhi", ";"+tiers[itier]+"Lepton1-Lepton2 #Delta#phi", 100, -3.5,3.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl1j1dPhi", ";"+tiers[itier]+"Lepton1-Jet1 #Delta#phi", 100, -3.5,3.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl2j2dPhi", ";"+tiers[itier]+"Lepton2-Jet2 #Delta#phi", 100, -3.5,3.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl1j2dPhi", ";"+tiers[itier]+"Lepton1-Jet2 #Delta#phi", 100, -3.5,3.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl2j1dPhi", ";"+tiers[itier]+"Lepton2-Jet1 #Delta#phi", 100, -3.5,3.5) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFj1j2dPhi", ";"+tiers[itier]+"Jet1-Jet2 #Delta#phi", 100, -3.5,3.5) );

          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl1l2Mass", ";"+tiers[itier]+"Lepton1-Lepton2 Mass", 100, 0, 1000) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl1j1Mass", ";"+tiers[itier]+"Lepton1-Jet1 Mass"   , 100, 0, 1000) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl2j2Mass", ";"+tiers[itier]+"Lepton2-Jet2 Mass"   , 100, 0, 1000) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl1j2Mass", ";"+tiers[itier]+"Lepton1-Jet2 Mass"   , 100, 0, 1000) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFl2j1Mass", ";"+tiers[itier]+"Lepton2-Jet1 Mass"   , 100, 0, 1000) );
          controlHistos.addHistogram( new TH1F (tiers[itier]+"VBFj1j2Mass", ";"+tiers[itier]+"Jet1-Jet2 Mass"      , 100, 0, 1000) );
	}
      else
	{
          controlHistos.addHistogram( new TH1D (tiers[itier]+"PU", ";"+tiers[itier]+" PU;Events", 30, 0.,30.) );

	  controlHistos.addHistogram( new TH1D (tiers[itier]+"m", ";"+tiers[itier]+" M;Events", 100, 100.,500.) );
	}
    }
  controlHistos.addHistogram(  new TH1D ("deltaprefrecoil", ";h_{T} - p_{T}^{gen}(H)", 100,-100,100) );
  controlHistos.addHistogram(  new TH1D ("deltaphiprefrecoil", ";#phi(h_{T}) - #phi^{gen}(H)", 100,0,3.2) );


  //replicate monitor for categories
  TString cats[]={"ee","emu","mumu"};
  TString subcats[]={"","vbf", "eq0jets","eq1jets","geq2jets"};
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(size_t isubcat=0;isubcat<sizeof(subcats)/sizeof(TString); isubcat++)
	controlHistos.initMonitorForStep(cats[icat]+subcats[isubcat]);
    }
  
  //Start the reduced met computer
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rmetCompVBF(1., 1., 1., 1., 1.);


  //aux
  TransverseMassComputer mtComp;

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

  //check run range
  float rescaleFactor( evEnd>0 ?  float(evSummaryHandler.getEntries())/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=evSummaryHandler.getEntries();
  if(evStart > evEnd ) 
    {
      file->Close();
       return -1;
    }

  //run the analysis
  for( int iev=evStart; iev<evEnd; iev++){
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      //      if(phys.genhiggs.size()==0) continue;

      //event categories
      TString evcat("");
      if(ev.cat==EMU)  evcat="emu";
      if(ev.cat==MUMU) evcat="mumu";
      if(ev.cat==EE)   evcat="ee";
      TString cats[]={"all",evcat};


      //weights for this event
      float puweight=ev.weight;
      float weight=puweight*ev.hptWeights[0];
      if(!isMC) weight=1;
     
      //the higgs      
      LorentzVector higgs; if(phys.genhiggs.size()>=1) higgs= phys.genhiggs[0];

      //z kinematics
      LorentzVector zll;if(phys.leptons.size()>=2)zll=phys.leptons[0]+phys.leptons[1];
      LorentzVector genzll;if(phys.genleptons.size()>=2)genzll=phys.genleptons[0]+phys.genleptons[1];
      LorentzVector zvv=phys.met[0];
      LorentzVector genzvv=phys.genmet[0];
      float transverseMass = mtComp.compute(zll,zvv,true);
 
      //higgs recoil
      LorentzVector clusteredRecoil(0,0,0,0);
      LorentzVector unclusteredRecoil=zvv+zll;
      LorentzVector vertexRecoil = phys.vtx;
      vertexRecoil *=-1;

      //jet activity
      LorentzVectorCollection jetsp4, genjets, alljetsp4, allgenjets;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++) {
        if(fabs(phys.jets[ijet].eta())<2.5){
	   jetsp4.push_back(phys.jets[ijet]);
   	   clusteredRecoil += phys.jets[ijet];
        }
        alljetsp4.push_back(phys.jets[ijet]);
      }
      clusteredRecoil *=-1;
      for(size_t ijet=0; ijet<phys.genjets.size(); ijet++){ allgenjets.push_back(phys.genjets[ijet]);  if(phys.genjets[ijet].eta()<2.5) genjets.push_back(phys.genjets[ijet]);}

           
      //redmet
//      rmetComp.compute(phys.leptons[0], phys.leptons[0].ptErr, phys.leptons[1], phys.leptons[1].ptErr, jetsp4, zvv );
      rmetComp.compute(phys.leptons[0], 0, phys.leptons[1], 0, jetsp4, zvv );
      double rmet = rmetComp.reducedMET(ReducedMETComputer::MINIMIZED);
      double rmetT = rmetComp.reducedMETComponents(ReducedMETComputer::MINIMIZED).first;
      double rmetL = rmetComp.reducedMETComponents(ReducedMETComputer::MINIMIZED).second;      
      int prefrecoil = rmetComp.getPreferedRecoil(ReducedMETComputer::MINIMIZED).second;
      LorentzVector redMetp4(rmetComp.reducedMETcartesian(ReducedMETComputer::MINIMIZED).Px(),
			     rmetComp.reducedMETcartesian(ReducedMETComputer::MINIMIZED).Py(),
			     0,
			     rmet);


//      rmetCompVBF.compute(phys.leptons[0], phys.leptons[0].ptErr, phys.leptons[1], phys.leptons[1].ptErr, alljetsp4, zvv );
      rmetCompVBF.compute(phys.leptons[0], 0, phys.leptons[1], 0, alljetsp4, zvv );
      double rmetVBF = rmetCompVBF.reducedMET(ReducedMETComputer::MINIMIZED);
      double rmetTVBF = rmetCompVBF.reducedMETComponents(ReducedMETComputer::MINIMIZED).first;
      double rmetLVBF = rmetCompVBF.reducedMETComponents(ReducedMETComputer::MINIMIZED).second;
      int prefrecoilVBF = rmetCompVBF.getPreferedRecoil(ReducedMETComputer::MINIMIZED).second;
      LorentzVector redMetp4VBF(rmetCompVBF.reducedMETcartesian(ReducedMETComputer::MINIMIZED).Px(),
                             rmetCompVBF.reducedMETcartesian(ReducedMETComputer::MINIMIZED).Py(),
                             0,
                             rmetVBF);



      //
      LorentzVector recoilEstimate(prefrecoil==ReducedMETComputer::CLUSTERED ? clusteredRecoil : unclusteredRecoil );

      LorentzVector unclRecoil200(recoilEstimate.px(),recoilEstimate.py(), 0,sqrt(pow(200,2)+pow(recoilEstimate.pt(),2)));
      ROOT::Math::Boost cmboost200(unclRecoil200.BoostToCM());
      LorentzVector cm200Zll(cmboost200(zll));
      LorentzVector cm200Zvv(cmboost200(zvv));
      float transverseMass200 = mtComp.compute(cm200Zll,cm200Zvv,true);

      LorentzVector unclRecoil400(recoilEstimate.px(),recoilEstimate.py(), 0,sqrt(pow(400,2)+pow(recoilEstimate.pt(),2)));
      ROOT::Math::Boost cmboost400(unclRecoil400.BoostToCM());
      LorentzVector cm400Zll(cmboost400(zll));
      LorentzVector cm400Zvv(cmboost400(zvv));
      float transverseMass400 = mtComp.compute(cm400Zll,cm400Zvv,true);

      
      ROOT::Math::Boost dilcmboost(zll.BoostToCM());
      LorentzVector dilcmRedMet(dilcmboost(redMetp4));
      float dphistar=deltaPhi(dilcmRedMet.phi(),zll.phi());
      float dphistarrec=deltaPhi(dilcmRedMet.phi(),recoilEstimate.phi());

      // cross-check (generator level)
      //       LorentzVector genRecoil2D(higgs);
      //       ROOT::Math::Boost cmgenboost(genRecoil2D.BoostToCM());
      //       LorentzVector cmGenZll(cmgenboost(genzll));
      //       LorentzVector cmGenZvv(cmgenboost(genzvv));


      //VBF selection  (Assuming jet are pt-ordered)  RECO PART 
      double NJets15=0.0, NJets30=0.0;
      for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>15)NJets15++;  if(alljetsp4[ijet].pt()>30)NJets30++;}

      bool isVBF = false;
      bool Pass2Jet30 = false;
      bool PassdEtaCut = false;
      bool PassiMCut = false;
      bool PassBJetVeto = false;
      bool PassJetVeto = false;
      bool PassLeptonIn = false;
      bool PassRedMET = false;

      double    VBFdEta = -1;
      int VBFCentralJets = 0, VBFCentral30Jets = 0;
      int VBFCentralLeptons = 0;
      int VBFNBJets=0;
     LorentzVector VBFSyst;

      if(alljetsp4.size()>=2 && alljetsp4 [0].pt()>30.0 && alljetsp4 [1].pt()>30.0){
         VBFSyst =  alljetsp4[0] + alljetsp4[1];
         VBFdEta = fabs(   alljetsp4[0].eta() -    alljetsp4[1].eta()); if(alljetsp4[0].eta()* alljetsp4[1].eta()>0)VBFdEta*=-1;
         int VBFCentralJets = 0, VBFCentral30Jets = 0;
         double MinEta, MaxEta;
         if(alljetsp4[0].eta()<alljetsp4[1].eta()){MinEta=alljetsp4[0].eta(); MaxEta=alljetsp4[1].eta();}else{MinEta=alljetsp4[1].eta(); MaxEta=alljetsp4[0].eta();}
         for(size_t ijet=2; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].eta()>MinEta && alljetsp4[ijet].eta()<MaxEta){VBFCentralJets++; if(alljetsp4[ijet].pt()>30)VBFCentral30Jets++; } }
         if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
         for(size_t ijet=2; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && phys.jets[ijet].btag1>1.7)VBFNBJets++; }             

         Pass2Jet30 = true;
         PassdEtaCut = (fabs(VBFdEta)>4.0);// && fabs(alljetsp4[0].eta())>0.5 && fabs(alljetsp4[1].eta())>0.5);
         PassiMCut   = (VBFSyst.M()>600);
         PassBJetVeto = (VBFNBJets==0);
         PassJetVeto = (VBFCentral30Jets==0);
         PassLeptonIn = (VBFCentralLeptons==2);
//       PassRedMET  = phys.met[0].pt()>50;//true;//(rmet>40);
         PassRedMET  =(rmet>50);
         isVBF = (PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn);
      }

      //subcategory
      TString subcat("vbf");
      if(!isVBF){
      if(jetsp4.size()==0) subcat="eq0jets";
      if(jetsp4.size()==1) subcat="eq1jets";
      if(jetsp4.size()>1)  subcat="geq2jets";
      }
      TString subCats[]={"",subcat};

      EventCategory Categorizer;
      TString label = Categorizer.GetLabel(phys);
      
      //subcategory analysis
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	for(size_t isc=0; isc<sizeof(subCats)/sizeof(TString); isc++)
	  {
	    TString ctf=cats[ic]+subCats[isc];

          

            //VBF
            if(PassdEtaCut                                                                          )controlHistos.fillHisto("recoVBFNEvents" ,ctf, 2 ,weight);
            if(               PassiMCut                                                             )controlHistos.fillHisto("recoVBFNEvents" ,ctf, 3 ,weight);
            if(                            PassBJetVeto                                             )controlHistos.fillHisto("recoVBFNEvents" ,ctf, 4 ,weight);
            if(                                            PassJetVeto                              )controlHistos.fillHisto("recoVBFNEvents" ,ctf, 5 ,weight);
            if(                                                           PassLeptonIn              )controlHistos.fillHisto("recoVBFNEvents" ,ctf, 6 ,weight);
            if(                                                                           PassRedMET)controlHistos.fillHisto("recoVBFNEvents" ,ctf, 7 ,weight);

                                                                                                     controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 0 ,weight);
            if(Pass2Jet30                                                                           )controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 1 ,weight);
            if(PassdEtaCut                                                                          )controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 2 ,weight);
            if(PassdEtaCut && PassiMCut                                                             )controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 3 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto                                             )controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 4 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto                              )controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 5 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 6 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 7 ,weight);

            if(               PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFNEventsNM1C" ,ctf, 2 ,weight);
            if(PassdEtaCut &&              PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFNEventsNM1C" ,ctf, 3 ,weight);
            if(PassdEtaCut && PassiMCut &&                 PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFNEventsNM1C" ,ctf, 4 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto &&                PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFNEventsNM1C" ,ctf, 5 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto &&                 PassRedMET)controlHistos.fillHisto("recoVBFNEventsNM1C" ,ctf, 6 ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFNEventsNM1C" ,ctf, 7 ,weight);

            if(PassdEtaCut                                                                          )controlHistos.fillHisto("recoVBFMinEta",ctf,    std::min(fabs(alljetsp4[0].eta()),fabs(alljetsp4[1].eta())),weight);
            if(PassdEtaCut                                                                          )controlHistos.fillHisto("recoVBFiMassInc",ctf,    VBFSyst.M(),weight);
            if(PassdEtaCut && PassiMCut                                                             )controlHistos.fillHisto("recoVBFBTagJet1Inc" ,ctf,    phys.jets[0].btag1,weight);
            if(PassdEtaCut && PassiMCut                                                             )controlHistos.fillHisto("recoVBFBTagJet2Inc" ,ctf,    phys.jets[1].btag1,weight);
            if(PassdEtaCut && PassiMCut                                                             )controlHistos.fillHisto("recoVBFNBJets30Inc" ,ctf,    VBFNBJets              ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto                                             )controlHistos.fillHisto("recoVBFcen30JetVetoInc",ctf,    VBFCentral30Jets,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto                              )controlHistos.fillHisto("recoVBFcenLeptonVetoInc" ,ctf, VBFCentralLeptons ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFredmetInc",ctf, rmet,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFmetInc",ctf, phys.met[0].pt(),weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fill2DHisto("recoVBFmetvsredmetInc",ctf,    phys.met[0].pt(), rmet ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFprefrecoilInc",ctf, recoilEstimate.pt(),weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFmtInc",ctf, transverseMass,weight);

            if(               PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fill2DHisto("recoVBFEta1Eta2NM1C",ctf,    alljetsp4[0].eta(), alljetsp4[1].eta(),weight);
            if(               PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFdEtaNM1C" ,ctf,    fabs(VBFdEta),weight);
            if(               PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFdEtaSignedNM1C" ,ctf,    VBFdEta,weight);
            if(PassdEtaCut &&              PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFiMassNM1C",ctf,    VBFSyst.M(),weight);
            if(PassdEtaCut && PassiMCut &&                 PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFBTagJet1NM1C" ,ctf,    phys.jets[0].btag1,weight);
            if(PassdEtaCut && PassiMCut &&                 PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFBTagJet2NM1C" ,ctf,    phys.jets[1].btag1,weight);
            if(PassdEtaCut && PassiMCut &&                 PassJetVeto && PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFNBJets30NM1C" ,ctf,    VBFNBJets              ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto &&                PassLeptonIn && PassRedMET)controlHistos.fillHisto("recoVBFcen30JetVetoNM1C",ctf,    VBFCentral30Jets,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto &&                 PassRedMET)controlHistos.fillHisto("recoVBFcenLeptonVetoNM1C" ,ctf,    VBFCentralLeptons              ,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFredmetNM1C",ctf, rmet,weight);
            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn              )controlHistos.fillHisto("recoVBFmetNM1C",ctf, phys.met[0].pt(),weight);

            controlHistos.fillHisto("recoBefVBFNJets30",ctf, NJets30,weight);
            controlHistos.fillHisto("recoBefVBFredmet",ctf, rmet,weight);
            controlHistos.fillHisto("recoBefVBFmet",ctf, phys.met[0].pt(),weight);
            controlHistos.fillHisto("recoVBFNEventsInc" ,ctf, 0 ,weight);
              
            if(Pass2Jet30){
               controlHistos.fill2DHisto("recoVBFEta1Eta2",ctf,    alljetsp4[0].eta(), alljetsp4[1].eta(),weight); 
               controlHistos.fillHisto("recoVBFdEta" ,ctf,    fabs(VBFdEta),weight);
               controlHistos.fillHisto("recoVBFdEtaSigned" ,ctf,    VBFdEta,weight);
               controlHistos.fillHisto("recoVBFiMass",ctf,    VBFSyst.M(),weight);
               controlHistos.fillHisto("recoVBFBTagJet1" ,ctf,    phys.jets[0].btag1,weight);
               controlHistos.fillHisto("recoVBFBTagJet2" ,ctf,    phys.jets[1].btag1,weight);
               controlHistos.fillHisto("recoVBFNBJets30" ,ctf,    VBFNBJets              ,weight);
               controlHistos.fillHisto("recoVBFcen30JetVeto",ctf,    VBFCentral30Jets,weight);
               controlHistos.fillHisto("recoVBFcenLeptonVeto",ctf, VBFCentralLeptons ,weight);
               controlHistos.fillHisto("recoVBFredmet",ctf, rmet,weight);
               controlHistos.fillHisto("recoVBFmet",ctf, phys.met[0].pt(),weight);
               controlHistos.fill2DHisto("recoVBFmetvsredmet",ctf,    phys.met[0].pt(), rmet ,weight);
            }

            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn){
               controlHistos.fillHisto("recoVBFcmmt",ctf, transverseMass200,weight);
               controlHistos.fillHisto("recoVBFcmdeltazz",ctf, cm200Zll.pt()-cm200Zvv.pt(),weight);
               controlHistos.fillHisto("recoVBFcmzllpt",ctf, cm200Zll.pt(),weight);
               controlHistos.fillHisto("recoVBFcmzvvpt",ctf, cm200Zvv.pt(),weight);
            }

//            if(PassdEtaCut && PassiMCut && PassBJetVeto && PassJetVeto && PassLeptonIn && PassRedMET){
            if(Pass2Jet30){
               controlHistos.fill2DHisto("recoVBFj1j2Phi",ctf,    alljetsp4[0].phi(),alljetsp4[1].phi(),weight);

               controlHistos.fillHisto("recoVBFl1l2dPhi" ,ctf, deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi()) ,weight);
               controlHistos.fillHisto("recoVBFl1j1dPhi" ,ctf, deltaPhi(phys.leptons[0].phi(),alljetsp4[0].phi()) ,weight);
               controlHistos.fillHisto("recoVBFl2j2dPhi" ,ctf, deltaPhi(phys.leptons[1].phi(),alljetsp4[1].phi()) ,weight);
               controlHistos.fillHisto("recoVBFl1j2dPhi" ,ctf, deltaPhi(phys.leptons[0].phi(),alljetsp4[1].phi()) ,weight);
               controlHistos.fillHisto("recoVBFl2j1dPhi" ,ctf, deltaPhi(phys.leptons[1].phi(),alljetsp4[0].phi()) ,weight);
               controlHistos.fillHisto("recoVBFj1j2dPhi" ,ctf, deltaPhi(alljetsp4[0].phi(),alljetsp4[1].phi()) ,weight);

               controlHistos.fillHisto("recoVBFl1l2Mass" ,ctf, (phys.leptons[0]+phys.leptons[1]).M() ,weight);
               controlHistos.fillHisto("recoVBFl1j1Mass" ,ctf, (phys.leptons[0]+alljetsp4[0]).M() ,weight);
               controlHistos.fillHisto("recoVBFl2j2Mass" ,ctf, (phys.leptons[1]+alljetsp4[1]).M() ,weight);
               controlHistos.fillHisto("recoVBFl1j2Mass" ,ctf, (phys.leptons[0]+alljetsp4[1]).M() ,weight);
               controlHistos.fillHisto("recoVBFl2j1Mass" ,ctf, (phys.leptons[1]+alljetsp4[0]).M() ,weight);
               controlHistos.fillHisto("recoVBFj1j2Mass" ,ctf, (alljetsp4[0]+alljetsp4[1]).M() ,weight);
            }


            /////////////////////// MAKE YIELDS OF EVENTS
//          controlHistos.addHistogram( new TH1D (tiers[itier]+"SelSTD", ";"+tiers[itier]+" Selection;Events", 10, 0.,10.) );
  //        controlHistos.addHistogram( new TH1D (tiers[itier]+"SelNEW", ";"+tiers[itier]+" Selection;Events", 10, 0.,10.) );


            controlHistos.fillHisto("recoSelSTD" ,ctf, 0 ,weight);
            if(phys.leptons[0].pt()>20 && phys.leptons[1].pt()>20){
               controlHistos.fillHisto("recoSelSTD" ,ctf, 1 ,weight);
               if(fabs(zll.mass()-91.1876)<15){
                  controlHistos.fillHisto("recoSelSTD" ,ctf, 2 ,weight);
                  int NAdditionalLeptons = 0; for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)NAdditionalLeptons++;}
                  if(NAdditionalLeptons==0){
                     controlHistos.fillHisto("recoSelSTD" ,ctf, 3 ,weight);
                     if(zll.pt()>25){
                        controlHistos.fillHisto("recoSelSTD" ,ctf, 4 ,weight);
                        int NBJets=0; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && phys.jets[ijet].btag1>2.0)NBJets++; }
                        if(NBJets==0){
                           controlHistos.fillHisto("recoSelSTD" ,ctf, 5 ,weight);
                           float dPhiCut=-1; float METCut=69;  float MTMinCut=216; float MTMaxCut=99999;
//                           if(higgs.mass()>350){dPhiCut=-1;METCut=141; MTMinCut=336;}
//                           if(higgs.mass()<350){dPhiCut=0.62;METCut=69; MTMinCut=216;  MTMaxCut=272;}
                           dPhiCut=-1;METCut=141; MTMinCut=336;

                           float MindPhi=9999; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi()))<MindPhi)MindPhi=fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi())); }
                           if(MindPhi>dPhiCut){
                              controlHistos.fillHisto("recoSelSTD" ,ctf, 6 ,weight);
                              controlHistos.fillHisto("recoSelSTDMT" ,ctf, transverseMass ,weight);

                              if(transverseMass>MTMinCut && transverseMass<MTMaxCut){
                                 controlHistos.fillHisto("recoSelSTD" ,ctf, 7 ,weight);
                                 controlHistos.fillHisto("recoSelSTDMet" ,ctf, phys.met[0].pt() ,weight);

                                 if(phys.met[0].pt()>METCut){
                                    controlHistos.fillHisto("recoSelSTD" ,ctf, 8 ,weight);
                                    controlHistos.fillHisto("recoSelSTD" ,ctf, 9 ,weight);
                                 }
                              }
                           }                                                   
                        }
                     }
                  }
               }
            }



            controlHistos.fillHisto("recoSelNEW" ,ctf, 0 ,weight);
            if(phys.leptons[0].pt()>20 && phys.leptons[1].pt()>20){
               controlHistos.fillHisto("recoSelNEW" ,ctf, 1 ,weight);
               if(fabs(zll.mass()-91.1876)<15){
                  controlHistos.fillHisto("recoSelNEW" ,ctf, 2 ,weight);
                  int NAdditionalLeptons = 0; for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)NAdditionalLeptons++;}
                  if(NAdditionalLeptons==0){
                     controlHistos.fillHisto("recoSelNEW" ,ctf, 3 ,weight);
                     if(zll.pt()>25){
                        controlHistos.fillHisto("recoSelNEW" ,ctf, 4 ,weight);
                        int NBJets=0; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && phys.jets[ijet].btag1>2.0)NBJets++; }
                        if(NBJets==0){
                           controlHistos.fillHisto("recoSelNEW" ,ctf, 5 ,weight);
                           float dPhiCut=-1; float METCut=69;  float MTMinCut=216; float MTMaxCut=99999;
//                           if(higgs.mass()>350){dPhiCut=-1;METCut=141; MTMinCut=336;}
//                           if(higgs.mass()<350){dPhiCut=0.62;METCut=69; MTMinCut=216;  MTMaxCut=272;}
                           dPhiCut=-1;METCut=141; MTMinCut=336;

//                           float MindPhi=9999; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi()))<MindPhi)MindPhi=fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi())); }
//                           if(MindPhi>dPhiCut){
                              controlHistos.fillHisto("recoSelNEW" ,ctf, 6 ,weight);
                              controlHistos.fillHisto("recoSelNEWMT" ,ctf, transverseMass ,weight);

                              if(transverseMass>MTMinCut && transverseMass<MTMaxCut){
                                 controlHistos.fillHisto("recoSelNEW" ,ctf, 7 ,weight);
                                 controlHistos.fillHisto("recoSelNEWMet" ,ctf, rmet ,weight);

                                 if(rmet>METCut){
                                    controlHistos.fillHisto("recoSelNEW" ,ctf, 8 ,weight);
                                    controlHistos.fillHisto("recoSelNEW" ,ctf, 9 ,weight);
                                 }
                              }
//                           }
                        }
                     }
                  }
               }
            }






            controlHistos.fillHisto("recoSelNEW2" ,ctf, 0 ,weight);
            if(phys.leptons[0].pt()>20 && phys.leptons[1].pt()>20){
               controlHistos.fillHisto("recoSelNEW2" ,ctf, 1 ,weight);
               if(fabs(zll.mass()-91.1876)<15){
                  controlHistos.fillHisto("recoSelNEW2" ,ctf, 2 ,weight);
                  int NAdditionalLeptons = 0; for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)NAdditionalLeptons++;}
                  if(NAdditionalLeptons==0){
                     controlHistos.fillHisto("recoSelNEW2" ,ctf, 3 ,weight);
                     if(zll.pt()>25){
                        controlHistos.fillHisto("recoSelNEW2" ,ctf, 4 ,weight);
                        int NBJets=0; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && phys.jets[ijet].btag1>2.0)NBJets++; }
                        if(NBJets==0){
                           controlHistos.fillHisto("recoSelNEW2" ,ctf, 5 ,weight);
                           float dPhiCut=-1; float METCut=69;  float MTMinCut=216; float MTMaxCut=99999;
//                           if(higgs.mass()>350){dPhiCut=-1;METCut=141; MTMinCut=336;}
//                           if(higgs.mass()<350){dPhiCut=0.62;METCut=69; MTMinCut=216;  MTMaxCut=272;}
                           dPhiCut=-1;METCut=141; MTMinCut=336;

                             if(jetsp4.size()==0){METCut=165;}
                             if(jetsp4.size()==1){METCut=145;}
                             if(jetsp4.size()>=2){METCut=130;}


//                           float MindPhi=9999; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi()))<MindPhi)MindPhi=fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi())); }
//                           if(MindPhi>dPhiCut){
                              controlHistos.fillHisto("recoSelNEW2" ,ctf, 6 ,weight);
                              controlHistos.fillHisto("recoSelNEW2MT" ,ctf, transverseMass ,weight);

                              if(transverseMass>MTMinCut && transverseMass<MTMaxCut){
                                 controlHistos.fillHisto("recoSelNEW2" ,ctf, 7 ,weight);
                                 controlHistos.fillHisto("recoSelNEW2Met" ,ctf, rmet ,weight);

                                 if(rmet>METCut){
                                    controlHistos.fillHisto("recoSelNEW2" ,ctf, 8 ,weight);
                                    controlHistos.fillHisto("recoSelNEW2" ,ctf, 9 ,weight);
                                 }
                              }
//                           }
                        }
                     }
                  }
               }
            }




            controlHistos.fillHisto("recoSelNEW3" ,ctf, 0 ,weight);
            if(phys.leptons[0].pt()>20 && phys.leptons[1].pt()>20){
               controlHistos.fillHisto("recoSelNEW3" ,ctf, 1 ,weight);
               if(fabs(zll.mass()-91.1876)<15){
                  controlHistos.fillHisto("recoSelNEW3" ,ctf, 2 ,weight);
                  int NAdditionalLeptons = 0; for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)NAdditionalLeptons++;}
                  if(NAdditionalLeptons==0){
                     controlHistos.fillHisto("recoSelNEW3" ,ctf, 3 ,weight);
                     if(zll.pt()>25){
                        controlHistos.fillHisto("recoSelNEW3" ,ctf, 4 ,weight);
                        int NBJets=0; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && phys.jets[ijet].btag1>2.0)NBJets++; }
                        if(NBJets==0){
                           controlHistos.fillHisto("recoSelNEW3" ,ctf, 5 ,weight); 
                           float dPhiCut=-1; float METCut=69;  float MTMinCut=216; float MTMaxCut=99999;
//                           if(higgs.mass()>350){dPhiCut=-1;METCut=141; MTMinCut=336;}
//                           if(higgs.mass()<350){dPhiCut=0.62;METCut=69; MTMinCut=216;  MTMaxCut=272;}
                           dPhiCut=-1;METCut=141; MTMinCut=336;

                             if(jetsp4.size()==0){METCut=165;}
                             if(jetsp4.size()==1){METCut=145;}
                             if(jetsp4.size()>=2){METCut=130;}
                             if(isVBF){METCut=50;}


//                           float MindPhi=9999; for(size_t ijet=0; ijet<alljetsp4.size();ijet++){ if(alljetsp4[ijet].pt()>30 && fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi()))<MindPhi)MindPhi=fabs(deltaPhi(alljetsp4[ijet].phi(), phys.met[0].phi())); }
//                           if(MindPhi>dPhiCut){
                              controlHistos.fillHisto("recoSelNEW3" ,ctf, 6 ,weight);
                              controlHistos.fillHisto("recoSelNEW3MT" ,ctf, transverseMass ,weight);

                              if(transverseMass>MTMinCut && transverseMass<MTMaxCut){
                                 controlHistos.fillHisto("recoSelNEW3" ,ctf, 7 ,weight);
                                 controlHistos.fillHisto("recoSelNEW3Met" ,ctf, rmet ,weight);

                                 if(rmet>METCut){
                                    controlHistos.fillHisto("recoSelNEW3" ,ctf, 8 ,weight);
                                    controlHistos.fillHisto("recoSelNEW3" ,ctf, 9 ,weight);
                                 }
                              }
//                           }
                        }
                     }
                  }
               }
            }


            ///////////////////////





	    //higgs pT
	    for(size_t ivar=0; ivar<sizeof(hptvars)/sizeof(TString); ivar++)
	      {
		double newweight(puweight);
		if(ivar) newweight *= ev.hptWeights[ivar-1];
		controlHistos.fillHisto("hpt"+hptvars[ivar],ctf, higgs.pt(),newweight,true);  
	      }
            controlHistos.fillHisto("genPU",ctf, ev.ngenITpu,weight);
	    
	    controlHistos.fillHisto("recozpt",ctf, zll.pt(),weight);  
	    controlHistos.fillHisto("genzpt",ctf, genzll.pt(),weight);  
	    controlHistos.fillHisto("recozvvpt",ctf, zvv.pt(),weight);  
	    controlHistos.fillHisto("genzvvpt",ctf, genzvv.pt(),weight);  
	    controlHistos.fillHisto("reconjets",ctf, jetsp4.size(),weight);      	    
	    controlHistos.fillHisto("gennjets",ctf, genjets.size(),weight);    
	    controlHistos.fillHisto("recomt",ctf, transverseMass,weight);      
	    controlHistos.fillHisto("genm",ctf, higgs.mass(),weight);      

            controlHistos.fill2DHisto("recozllptvszvvpt",ctf, zll.pt(), genzll.pt(),weight);


	    //reduced MET
	    controlHistos.fill2DHisto("recoredmetcomps", ctf,rmetL,rmetT,weight);
	    controlHistos.fillHisto("recoredmetL", ctf,rmetL,weight);
	    controlHistos.fillHisto("recoredmetT", ctf,rmetT,weight);
	    controlHistos.fillHisto("recoredmet", ctf,rmet,weight);	
            controlHistos.fillHisto("recopfmet", ctf,phys.met[0].pt(),weight);
            controlHistos.fillHisto("recoredmet5", ctf,rmetVBF,weight);     


	    controlHistos.fillHisto("recodilcmzvvpt",ctf,dilcmRedMet.pt(),weight);
	    controlHistos.fillHisto("recodilcmdphistar",ctf,dphistar,weight);
	    controlHistos.fillHisto("recodilcmdphistarrecoil",ctf,dphistarrec,weight);




	    //higgs recoil
	    controlHistos.fillHisto("recoprefrecoil",ctf, recoilEstimate.pt(),weight);  
	    controlHistos.fillHisto("deltaprefrecoil",ctf, recoilEstimate.pt()-higgs.pt(),weight);  
	    controlHistos.fillHisto("deltaphiprefrecoil",ctf, fabs(deltaPhi(recoilEstimate.phi(),higgs.phi())),weight);  

	    //pseudo-CM distributions
	    controlHistos.fillHisto("recocmmt",ctf, transverseMass200,weight);  
	    controlHistos.fillHisto("recocmzzdeltapt",ctf, cm200Zll.pt()-cm200Zvv.pt(),weight);  
            controlHistos.fillHisto("recocmzzsumpt",ctf, cm200Zll.pt()+cm200Zvv.pt(),weight);
	    controlHistos.fillHisto("recocmzllpt",ctf, cm200Zll.pt(),weight);  
	    controlHistos.fillHisto("recocmzvvpt",ctf, cm200Zvv.pt(),weight);  
            controlHistos.fill2DHisto("recocmzllptvszvvpt",ctf, cm200Zll.pt(), cm200Zvv.pt(),weight);

	    
	    controlHistos.fillHisto("recocmmt400",ctf, transverseMass400,weight);  
	    controlHistos.fillHisto("recocmzzdeltapt400",ctf, cm400Zll.pt()-cm400Zvv.pt(),weight);  
            controlHistos.fillHisto("recocmzzsumpt400",ctf, cm400Zll.pt()+cm400Zvv.pt(),weight);
	    controlHistos.fillHisto("recocmzllpt400",ctf, cm400Zll.pt(),weight);  
	    controlHistos.fillHisto("recocmzvvpt400",ctf, cm400Zvv.pt(),weight);  
            controlHistos.fill2DHisto("recocmzllptvszvvpt400",ctf, cm400Zll.pt(), cm400Zvv.pt(),weight);

            //InvariantMass
	  }
    }

    TH1* pu = controlHistos.getHisto("genPU");
    pu->Scale(1.0/pu->Integral());
    for(int b=1;b<=pu->GetNbinsX();b++){printf(" %f, ",pu->GetXaxis()->GetBinCenter(b));}
    for(int b=1;b<=pu->GetNbinsX();b++){printf(" %f, ",pu->GetBinContent(b));}
 
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/mumu/mumu_cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      if(rescaleFactor>0) cnorm /= rescaleFactor;
    }

  //all done with the events file
  file->Close();

  //save to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  cout << outUrl << " " << cnorm << endl;
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  std::map<TString, TDirectory *> outDirs;
  outDirs["all"]=baseOutDir->mkdir("all");
  outDirs["ee"]=baseOutDir->mkdir("ee");
  outDirs["emu"]=baseOutDir->mkdir("emu");
  outDirs["mumu"]=baseOutDir->mkdir("mumu");
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      TString icat("all");
      if(it->first.BeginsWith("ee")) icat="ee";
      if(it->first.BeginsWith("emu")) icat="emu";
      if(it->first.BeginsWith("mumu")) icat="mumu";
      outDirs[icat]->cd();
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
        {
	  if(isMC && cnorm>0) hit->second->Scale(1./cnorm);
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH2")
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") )
	    fixExtremities(hit->second,true,true);
	  hit->second->Write();

        }
    }

  ofile->Close();
}  
