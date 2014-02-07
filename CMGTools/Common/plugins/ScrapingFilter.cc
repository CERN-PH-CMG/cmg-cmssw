
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/Jet.h"

// http://cmslxr.fnal.gov/lxr/source/DPGAnalysis/Skims/interface/FilterOutScraping.h
// http://cmslxr.fnal.gov/lxr/source/DPGAnalysis/Skims/src/FilterOutScraping.cc

class ScrapingFilter : public edm::EDFilter {

  public:

    explicit ScrapingFilter(const edm::ParameterSet & iConfig);
    ~ScrapingFilter() {}

  private:

    virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
    
    bool taggingMode_;
    //bool applyfilter;
    bool debugOn;
    double thresh;
    unsigned int numtrack;
    edm::InputTag tracks_;
  
    reco::TrackBase::TrackQuality _trackQuality;

};


ScrapingFilter::ScrapingFilter(const edm::ParameterSet & iConfig) {

  //applyfilter = iConfig.getUntrackedParameter<bool>("applyfilter",true);
  debugOn      = iConfig.getUntrackedParameter<bool>("debugOn",false);
  thresh       = iConfig.getUntrackedParameter<double>("thresh",0.25);
  numtrack     = iConfig.getUntrackedParameter<unsigned int>("numtrack",10);
  tracks_      = iConfig.getUntrackedParameter<edm::InputTag>("src",edm::InputTag("generalTracks"));
  taggingMode_ = iConfig.getParameter<bool>("TaggingMode");

  produces<bool>("Result");
}


bool ScrapingFilter::filter(edm::Event & iEvent, const edm::EventSetup & iSetup) {

   bool accepted = false;
   float fraction = 0;  
   // get GeneralTracks collection
 
   edm::Handle<reco::TrackCollection> tkRef;
   iEvent.getByLabel(tracks_,tkRef);	
   const reco::TrackCollection* tkColl = tkRef.product();
 
   //std::cout << "Total Number of Tracks " << tkColl->size() << std::endl;
   
   int numhighpurity=0;
   _trackQuality = reco::TrackBase::qualityByName("highPurity");
 
   if(tkColl->size()>numtrack){ 
     reco::TrackCollection::const_iterator itk = tkColl->begin();
     reco::TrackCollection::const_iterator itk_e = tkColl->end();
     for(;itk!=itk_e;++itk){
       // std::cout << "HighPurity?  " << itk->quality(_trackQuality) << std::endl;
       if(itk->quality(_trackQuality)) numhighpurity++;
     }
     fraction = (float)numhighpurity/(float)tkColl->size();
     if(fraction>thresh) accepted=true;
   }else{
     //if less than 10 Tracks accept the event anyway	 
     accepted= true;
   }
   
   
   if (debugOn) {
     std::cout << "FilterOutScraping_debug: Run " << iEvent.id().run() << " Event " << iEvent.id().event() 
               << " Lumi Block " << iEvent.luminosityBlock() << " Bunch Crossing " << iEvent.bunchCrossing() 
	       << " Fraction " << fraction << " NTracks " << tkColl->size() << " Accepted " << accepted << std::endl;
   }
  
   //if (applyfilter)
   //  return accepted;
   //else
   //  return true;
 

  bool result = accepted;
  std::auto_ptr<bool> pOut(new bool(result) ); 
  iEvent.put( pOut, "Result" ); 

  //if (!result)
  //   std::cout << "Failed ScrapingFilter: "
  //	     << iEvent.id().run() << " : " << iEvent.id().luminosityBlock() << " : " << iEvent.id().event()
  //	     << std::endl;

  if(taggingMode_) return true; else return result;  
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ScrapingFilter);
