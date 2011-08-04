#include "CMGTools/H2TauTau/interface/DiTauAnalysis.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

// ClassImp(DiTauAnalysis)
// ;

DiTauAnalysis::DiTauAnalysis():
  BaseAnalysis(),
  inputTag_(""),
  nDiTauHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauEtaHisto_(NULL)
{
}

DiTauAnalysis::DiTauAnalysis(const char * name):
  BaseAnalysis(name),
  inputTag_("cmgDiTauSelClean"),
  nDiTauHisto_(NULL),
  diTauMassHisto_(NULL),
  diTauEtaHisto_(NULL)
{
 
}

DiTauAnalysis::~DiTauAnalysis(){
}


bool DiTauAnalysis::init(){
  
  if(!BaseAnalysis::init())return 0;

  int si=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s, si++){
    if(!(samples_[si]->init())) return 0;

    if(!samples_[si]->addHisto((TH1*)(new TH1F("nDiTauHisto","",5,-.5,4.5))))return 0;
    if(!samples_[si]->addHisto((TH1*)(new TH1F("diTauMassHisto","",300,0,300))))return 0;
    if(!samples_[si]->addHisto((TH1*)(new TH1F("diTauEtaHisto","",120,-6,6))))return 0;
    cout<<"DiTauAnalysis  Initialized sample "<<samples_[si]->GetName()<<endl;
  }
  
  return 1;
}


bool DiTauAnalysis::getHistos(Sample* s){
  if(!s) return 0;
  
  if(!BaseAnalysis::getHistos(s))return 0;
  
  nDiTauHisto_=(TH1F*)(s->getHisto("nDiTauHisto"));
  diTauMassHisto_=(TH1F*)(s->getHisto("diTauMassHisto"));
  diTauEtaHisto_=(TH1F*)(s->getHisto("diTauEtaHisto"));
  if( !nDiTauHisto_ || !diTauMassHisto_ || !diTauEtaHisto_ ) {
    cout<<"DiTauAnalysis histos not present"<<endl;    return 0;
  }

  return 1;
}



bool DiTauAnalysis::fill(edm::EventBase const & event ){
  
  //fill the event level histograms
  if(!BaseAnalysis::fill(event)) return 0;

  //fill the diTau histograms
  edm::InputTag particletag_(inputTag_.c_str());
  edm::Handle< std::vector<cmg::DiTau> > vec;
  event.getByLabel(particletag_, vec);
  Int_t icand=0;
  for(std::vector<cmg::DiTau>::const_iterator cand=vec->begin(); cand!=vec->end(); ++cand, icand++){
    diTauMassHisto_->Fill(cand->mass());
    diTauEtaHisto_->Fill(cand->eta());
  }
  nDiTauHisto_->Fill(icand);


 
  return 1;
}


bool DiTauAnalysis::analyze(TString samplename){

  Sample* sample=0;
  int si=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s, si++){
    if(samples_[si]->GetName()==samplename) sample=samples_[si];
  }
  if(!sample){cout<<"sample "<<samplename<<" was not found"<<endl; return 0;}

  if(!getHistos(sample))return 0;

  if(!sample->getEvents()){
    cout<<" No fwlit::ChainEvent found in sample "<<sample->GetName()<<endl;
    return 0;
  }
  fwlite::ChainEvent chain=*(sample->getEvents());
  //note: cannot use the pointer to the chain in the sample, event loop crashes after first file

  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() && ievt <  truncateEvents_; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    edm::EventBase const & event = chain;

    if(!fill(event)) return 0;

  }

  return 1;
}


bool DiTauAnalysis::scaleWeightHistos(Sample* s){

  if(!BaseAnalysis::scaleWeightHistos(s))return 0;


  return 1;
}


bool DiTauAnalysis::end(){
  int si=0;
  for(std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s,si++){
    if(!scaleWeightHistos(samples_[si]))return 0;
    samples_[si]->save();
  }

  return 1;
}
