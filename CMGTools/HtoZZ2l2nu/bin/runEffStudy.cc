#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"


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
 
using namespace std;
int main(int argc, char* argv[])
{
  SelectionMonitor controlHistos;

  EventCategory eventClassifComp(true);

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);

  bool isMC = runProcess.getParameter<bool>("isMC");
  double xsec = runProcess.getParameter<double>("xsec");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");

  //pileup weighting
  edm::LumiReWeighting LumiWeights(runProcess.getParameter<std::string>("mcpileup"), runProcess.getParameter<std::string>("datapileup"), "pileup","pileup");

  //tree info
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //check sample type
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isGG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isVBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  if(isGG){
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
  }
  else if(isVBF){
     size_t VBFStringpos =  string(url.Data()).find("VBF");
     string StringMass = string(url.Data()).substr(VBFStringpos+6,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
     VBFString = string(url.Data()).substr(VBFStringpos);
  }
  printf("isGG = %i     isVBF=%i  HiggsMass=%i\n",(int)isGG, (int)isVBF,(int)HiggsMass);

  //book histos 
  controlHistos.addHistogram(new TH1F( "eventflow", ";Cutflow;Events", 4,0,4) );
  controlHistos.addHistogram(new TH1F( "zmass", ";M^{ll};Events", 100,91-31,91+31) );
  controlHistos.addHistogram( new TH1F( "zeta", ";#eta^{ll};Events", 100,-5,5) );
  controlHistos.addHistogram( new TH1F( "zpt", ";p_{T}^{ll};Events", 100,0,400) );
  controlHistos.addHistogram( new TH1F( "dphill", ";#Delta#phi(l^{(1)},l^{(2)});Events",100,0,3.2) );
  controlHistos.addHistogram( new TH1F( "detall", ";#Delta#eta(l^{(1)},l^{(2)});Events",100,0,5) );
  controlHistos.addHistogram( new TH1F( "drll",   ";#DeltaR(l^{(1)},l^{(2)}) / (0.1 rad);Events",20,0,6) );
  
  for(int i=0; i<=6; i++)
    {
      TString idbit(""); if(i) idbit +=i;
      controlHistos.addHistogram( new TH1F( "l1pt" + idbit, ";p_{T}^{1} [GeV/c];Events", 20,0,500) );  
      controlHistos.addHistogram( new TH1F( "l2pt" + idbit, ";p_{T}^{2} [GeV/c];Events", 20,0,500) );
    }

  controlHistos.addHistogram( new TH2F( "l1pfisovspu",   ";PF isolation/p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );
  controlHistos.addHistogram( new TH2F( "l1pfisocorrvspu",   ";PF isolation with #beta corr./p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );
  controlHistos.addHistogram( new TH2F( "l1detisovspu",   ";Detector isolation/p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );
  controlHistos.addHistogram( new TH2F( "l1detisocorrvspu",   ";Detector isolation with #rho corr./p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );

  controlHistos.addHistogram( new TH2F( "l2pfisovspu",   ";PF isolation/p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );
  controlHistos.addHistogram( new TH2F( "l2pfisocorrvspu",   ";PF isolation with #beta corr./p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );
  controlHistos.addHistogram( new TH2F( "l2detisovspu",   ";Detector isolation/p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );
  controlHistos.addHistogram( new TH2F( "l2detisocorrvspu",   ";Detector isolation with #rho corr./p_{T}^{1};Pileup;Events",30,0,0.3,30,0,30) );

  controlHistos.addHistogram( new TH2F( "pfiso1vsdrll",   ";PF isolation/p_{T}^{1};#DeltaR(l^{(1)},l^{(2)});Events",30,0,0.3,20,0,6) );
  controlHistos.addHistogram( new TH2F( "pfiso2vsdrll",   ";PF isolation/p_{T}^{2};#DeltaR(l^{(1)},l^{(2)});Events",30,0,0.3,20,0,6) );


  TString cats[]={"ee","mumu","all"};
  TString* subCats=new TString[eventClassifComp.GetLabelSize()];
  for(int i=0;i<eventClassifComp.GetLabelSize();i++) subCats[i] = eventClassifComp.GetRawLabel(i);
  for(size_t icat=0;icat<sizeof(cats)/sizeof(TString); icat++)
    {
      for(int isubcat=0;isubcat<eventClassifComp.GetLabelSize(); isubcat++)
	{
	  TString ctf=cats[icat]+subCats[isubcat];
	  controlHistos.initMonitorForStep(ctf);
	}
    }
  
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
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F* cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      printf("yields will be divided by: %f\n",cnorm);
    }
  
  
  //run the analysis
  for( int iev=0; iev<evSummaryHandler.getEntries(); iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100.*iev/float(evSummaryHandler.getEntries())));
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      
      // event category
      std::vector<TString> evcats;
      if(ev.cat==EMU)  continue;
      if(ev.cat==MUMU) evcats.push_back("mumu");
      if(ev.cat==EE)   evcats.push_back("ee");
      int eventCategory = eventClassifComp.Get(phys);
      TString subcat    = eventClassifComp.GetLabel(eventCategory);

      //MC truth
      float weight=1.0;
      LorentzVector genzll(0,0,0,0), genzvv(0,0,0,0), genhiggs(0,0,0,0);
      if(isMC && phys.genleptons.size())
	{
	  genzll=phys.genleptons[0]+phys.genleptons[1];
	  genzvv=phys.genmet[0];
	  genhiggs = phys.genhiggs[0];
	}

      //z+met kinematics      
      PhysicsObject_Lepton l1 = phys.leptons[0].pt() > phys.leptons[1].pt() ? phys.leptons[0] : phys.leptons[1];
      Float_t l1pfiso = l1.gIso+l1.chIso+l1.nhIso; 
      Float_t l1pfisocorr = l1.chIso+max(l1.gIso+l1.nhIso-0.5*l1.puchIso,0.);
      Float_t l1detiso = l1.ecalIso+l1.trkIso+l1.hcalIso; 
      Float_t l1detisocorr = l1.trkIso+max(l1.ecalIso+l1.trkIso-0.3*0.3*3.1415*ev.rho,0.);
      
      PhysicsObject_Lepton l2 = phys.leptons[0].pt() > phys.leptons[1].pt() ? phys.leptons[1] : phys.leptons[0];
      Float_t l2pfiso = l2.gIso+l2.chIso+l2.nhIso; 
      Float_t l2pfisocorr = l2.chIso+max(l2.gIso+l2.nhIso-0.5*l2.puchIso,0.);
      Float_t l2detiso = l2.ecalIso+l2.trkIso+l2.hcalIso; 
      Float_t l2detisocorr = l2.trkIso+max(l2.ecalIso+l2.trkIso-0.3*0.3*3.1415*ev.rho,0.);

      LorentzVector zll = l1+l2;
      LorentzVector zvv = phys.met[0];
      Float_t dphill     = deltaPhi(l1.phi(),l2.phi());
      Float_t detall     = l1.eta()-l2.eta();
      Float_t drll       = deltaR(l1,l2);
      Float_t zpt        = zll.pt();
      Float_t zmass      = zll.mass();
      Float_t zeta       = zll.eta();
      Float_t met        = zvv.pt();
      Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
      Float_t mt         = METUtils::transverseMass(zll,zvv,true);
      Float_t dphizleadl = deltaPhi(l1.phi(),zll.phi());
      

      bool passZmass(fabs(zmass-91)<15);
      
      std::vector<int> idbits;
      if(ev.cat==EE) { idbits.push_back(3); idbits.push_back(1);  idbits.push_back(8); idbits.push_back(10); }
      else           { idbits.push_back(0); idbits.push_back(1);  idbits.push_back(2); idbits.push_back(3);  }

      //fill control histograms
      std::vector<TString> catsToFill;
      catsToFill.push_back("all");
      for(size_t ic=0; ic<evcats.size(); ic++) catsToFill.push_back(evcats[ic]);
      std::vector<TString> subCatsToFill;
      subCatsToFill.push_back("");
      subCatsToFill.push_back(subcat);
      for(size_t ic=0; ic<catsToFill.size(); ic++){
	for(size_t isc=0; isc<subCatsToFill.size(); isc++){
	  
	  TString ctf=catsToFill[ic]+subCatsToFill[isc];  
	  
	  controlHistos.fillHisto("eventflow",ctf,0,weight);
	  controlHistos.fillHisto("zmass",ctf,zmass,weight);
	  if(!passZmass) continue;    
	  
	  controlHistos.fillHisto("eventflow",ctf,1,weight);
	  controlHistos.fillHisto("zeta",ctf,zeta,weight);
	  controlHistos.fillHisto("zpt",ctf,zpt,weight);
	  controlHistos.fillHisto("dphill",ctf,fabs(dphill),weight);
	  controlHistos.fillHisto("detall",ctf,fabs(detall),weight);
	  controlHistos.fillHisto("drll",ctf,drll,weight);
	  controlHistos.fillHisto("l1pt",ctf,l1.pt(),weight);
	  controlHistos.fillHisto("l2pt",ctf,l2.pt(),weight);
	  for(size_t ib=0; ib<idbits.size(); ib++)
	    {
	      TString idtag(""); idtag += (ib+1);
	      int l1id(l1.pid);
	      if(((l1id >> idbits[ib]) & 0x1)) controlHistos.fillHisto("l1pt"+idtag,ctf,l1.pt(),weight);
	      int l2id(l2.pid);
	      if(((l2id >> idbits[ib]) & 0x1)) controlHistos.fillHisto("l2pt"+idtag,ctf,l2.pt(),weight);

	    }
	  controlHistos.fill2DHisto("pfiso1vsdrll",ctf,l1pfiso/l1.pt(),drll,weight);
	  controlHistos.fill2DHisto("pfiso2vsdrll",ctf,l2pfiso/l2.pt(),drll,weight);

	  controlHistos.fill2DHisto("l1pfisovspu",ctf,l1pfiso/l1.pt(),ev.ngenITpu,weight);
	  controlHistos.fill2DHisto("l1pfisocorrvspu",ctf,l1pfisocorr/l1.pt(),ev.ngenITpu,weight);
	  controlHistos.fill2DHisto("l1detisovspu",ctf,l1detiso/l1.pt(),ev.ngenITpu,weight);
	  controlHistos.fill2DHisto("l1detisocorrvspu",ctf,l1detisocorr/l1.pt(),ev.ngenITpu,weight);

	  controlHistos.fill2DHisto("l2pfisovspu",ctf,l2pfiso/l2.pt(),ev.ngenITpu,weight);
	  controlHistos.fill2DHisto("l2pfisocorrvspu",ctf,l2pfisocorr/l2.pt(),ev.ngenITpu,weight);
	  controlHistos.fill2DHisto("l2detisovspu",ctf,l2detiso/l2.pt(),ev.ngenITpu,weight);
	  controlHistos.fill2DHisto("l2detisocorrvspu",ctf,l2detisocorr/l2.pt(),ev.ngenITpu,weight);

	}
      } 
  }

  
  //all done with the events file
  file->Close();
  
  //save control plots to file
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  std::map<TString, TDirectory *> outDirs;
  outDirs["all"]=baseOutDir->mkdir("all");
  outDirs["ee"]=baseOutDir->mkdir("ee");
  outDirs["mumu"]=baseOutDir->mkdir("mumu");
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++){
    TString icat("all");
    if(it->first.BeginsWith("ee")) icat="ee";
    if(it->first.BeginsWith("emu")) continue;
    if(it->first.BeginsWith("mumu")) icat="mumu";
    outDirs[icat]->cd();
    for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++){
      hit->second->Write();
    }
  }
  ofile->Close();
}  





