#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"


BaseFlatNtp::BaseFlatNtp(const edm::ParameterSet & iConfig):
  iEvent_(0),
  file_(0),
  tree_(0),
  eventweight_(0.),
  runnumber_(0),
  lumiblock_(0),
  eventid_(0),
  dataType_(iConfig.getParameter<string>("dataType")),
  pupWeight_(0.),
  pupWeightName_(iConfig.getParameter<edm::InputTag>("pupWeightName")),
  firstRun_(iConfig.getParameter<int>("firstRun")),
  lastRun_(iConfig.getParameter<int>("lastRun")),
  trigpass_(0){


  cout<<"Trigger paths: "<<endl;
  for(long p=1;p<=10;p++){
    edm::InputTag path=iConfig.getParameter<edm::InputTag>((const char*)(TString("trigPath")+p));
    if(path.label()!=""){
      cout<<path.label()<<" "<<path.instance()<<" "<<path.process()<<endl;
      trigPaths_.push_back(new edm::InputTag(path));
    }
  }

}

BaseFlatNtp::~BaseFlatNtp(){
  delete file_;
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){
    delete *path;
  }

}

void BaseFlatNtp::beginJob(){

  cout<<"dataType : "<<dataType_<<endl;
  cout<<"pupWeightName "<<pupWeightName_.label()<<endl;
  cout<<"firstRun : "<<firstRun_<<endl;
  cout<<"lastRun : "<<lastRun_<<endl;


  file_ = new edm::Service<TFileService>();
  //tree_->Branch("",&,"/I");

  tree_ = (*file_)->make<TTree>("tree","tree");
  tree_->Branch("eventweight",&eventweight_,"eventweight/F");
  tree_->Branch("runnumber",&runnumber_,"runnumber/I");
  tree_->Branch("lumiblock",&lumiblock_,"lumiblock/I");
  tree_->Branch("eventid",&eventid_,"eventid/I");
  tree_->Branch("nvtx",&nvtx_,"nvtx/I");//
  tree_->Branch("vtxx",&vtxx_,"vtxx/F");
  tree_->Branch("vtxy",&vtxy_,"vtxy/F");
  tree_->Branch("vtxz",&vtxz_,"vtxz/F");
}


void BaseFlatNtp::analyze(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  fillVariables(iEvent,iSetup);
  if(!applySelections())return;
  fill();
  tree_->Fill();
}

void BaseFlatNtp::endJob(){}



bool BaseFlatNtp::fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  iEvent_=&iEvent;

  runnumber_=iEvent.run();

  iEvent.getByLabel(edm::InputTag("offlinePrimaryVertices"), vertices_);


  ///fill trigger flag
  trigpass_=0;
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  iEvent.getByLabel(edm::InputTag("cmgTriggerObjectSel","","PAT"),trig);
  if(trigPaths_.size()==0)trigpass_=1;//no trigger requirement
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){//cmg ObjetSel
    //cout<<path->label()<<" "<<path->instance()<<" "<<path->process()<<endl;
    //trig->begin()->printSelections(cout);
    if(trig->begin()->hasSelection((*path)->label()))
      if(trig->begin()->getSelection((*path)->label()))
	if(trig->begin()->getPrescale((*path)->label())==1 || dataType_.compare("MC")==0){
	  trigpass_=1;
	}
  }
  //cout<<firstRun_<<" "<<lastRun_<<" "<<runnumber_<<" "<<trigpass_<<endl;

  //get trigger object list for later
  iEvent.getByLabel(edm::InputTag("cmgTriggerObjectListSel","","PAT"),trigObjs_);

  ///Event weight definition starts here:
  pupWeight_=1.;//do not comment out needs to be used.
  if(dataType_.compare("MC")==0){
    edm::Handle<double>  PupWeight;
    iEvent.getByLabel(pupWeightName_,PupWeight);    
    pupWeight_=(*PupWeight);
  }

 
  return 1;
}

bool BaseFlatNtp::applySelections(){

  if(firstRun_!=0) if(runnumber_<firstRun_)return 0;
  if(lastRun_!=0) if(lastRun_<runnumber_)return 0;

  if(!trigpass_) return 0;

  return 1;
}

bool BaseFlatNtp::fill(){

  lumiblock_=iEvent_->luminosityBlock();
  eventid_=iEvent_->id().event();

  nvtx_=vertices_->size();
  vtxx_=vertices_->begin()->x();
  vtxy_=vertices_->begin()->y();
  vtxz_=vertices_->begin()->z();


  return 1;
}


bool BaseFlatNtp::trigObjMatch(float eta, float phi, std::string path, std::string filter){
  for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs_->begin(); obj!=trigObjs_->end(); obj++){//cmg ObjectListSel
    if(obj->hasSelection(path.c_str())//HLT path name
       &&obj->hasSelection(filter.c_str())//last filter
       &&reco::deltaR(eta,phi,obj->eta(),obj->phi())<0.3)
      return 1;      
  }
  
  return 0;
}


void BaseFlatNtp::printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList){
  cout<<" Begin MC print out for event: "<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
  for(std::vector<reco::GenParticle>::const_iterator g=genList->begin(); g!=genList->end(); ++g){    
    cout<<"pdgID = "<<g->pdgId()<<" , pt = "<<g->p4().pt()<<" motherRef="<<g->mother()<<endl;
  }
}


