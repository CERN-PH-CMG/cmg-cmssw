#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

BaseFlatNtp::BaseFlatNtp(const edm::ParameterSet & iConfig):
  dataPeriodFlag_(iConfig.getParameter<int>("dataPeriodFlag")),
  verticesListTag_(iConfig.getParameter<edm::InputTag>("verticesListTag")),
  trigPathsListTag_(iConfig.getParameter<edm::InputTag>("trigPathsListTag")),
  trigObjsListTag_(iConfig.getParameter<edm::InputTag>("trigObjsListTag")),
  pupWeightName_(iConfig.getParameter<edm::InputTag>("pupWeightName")),
  firstRun_(iConfig.getParameter<int>("firstRun")),
  lastRun_(iConfig.getParameter<int>("lastRun")),
  dataType_(iConfig.getParameter<int>("dataType")),
  iEvent_(0),
  file_(0),
  tree_(0),
  pupWeight_(0.),
  eventweight_(0.),
  runnumber_(0),
  lumiblock_(0),
  eventid_(0),
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
  tree_->Branch("pupWeight",&pupWeight_,"pupWeight/F");
  tree_->Branch("eventweight",&eventweight_,"eventweight/F");
  tree_->Branch("runnumber",&runnumber_,"runnumber/I");
  tree_->Branch("lumiblock",&lumiblock_,"lumiblock/I");
  tree_->Branch("eventid",&eventid_,"eventid/I");
  tree_->Branch("npu",&npu_,"npu/I");//
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

  iEvent.getByLabel(verticesListTag_,vertices_);
  nvtx_=vertices_->size();  
  PV_=&(*(vertices_->begin()));

  ///fill trigger flag
  trigpass_=0;
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  iEvent.getByLabel(trigPathsListTag_,trig);
  if(trigPaths_.size()==0)trigpass_=1;//no trigger requirement
  //trig->begin()->printSelections(cout);
  for(std::vector<edm::InputTag *>::const_iterator path=trigPaths_.begin(); path!=trigPaths_.end(); path++){//cmg ObjetSel
    //cout<<path->label()<<" "<<path->instance()<<" "<<path->process()<<endl;
    if(trig->begin()->hasSelection((*path)->label()))
      if(trig->begin()->getSelection((*path)->label()))
	if(trig->begin()->getPrescale((*path)->label())==1 || dataType_==0){
	  trigpass_=1;
	}
  }
  //cout<<firstRun_<<" "<<lastRun_<<" "<<runnumber_<<" "<<trigpass_<<endl;

  //get trigger object list for later
  iEvent.getByLabel(trigObjsListTag_,trigObjs_);


  ///Event weight definition starts here:
  pupWeight_=1.;//do not comment out needs to be used.
  npu_=-1;
  if(dataType_==0 && (pupWeightName_.label()).compare("")!=0){//if no vertex weight name is provided then leave weight to 1
    edm::Handle<double>  PupWeight;
    iEvent.getByLabel(pupWeightName_,PupWeight);    
    pupWeight_=(*PupWeight);

    //get the number of pile up vertexes
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
      int BX = PVI->getBunchCrossing();
      if(BX == 0) {
	if(dataPeriodFlag_==2011)npu_ = PVI->getPU_NumInteractions();
	if(dataPeriodFlag_==2012)npu_ = PVI->getTrueNumInteractions();
      }
    }
  }

 
  return 1;
}

bool BaseFlatNtp::applySelections(){

  if(firstRun_!=0) if(runnumber_<firstRun_)return 0;
  if(lastRun_!=0) if(lastRun_<runnumber_)return 0;

  if(!trigpass_) return 0;
  if(nvtx_==0) return 0;
  
  return 1;
}

bool BaseFlatNtp::fill(){

  lumiblock_=iEvent_->luminosityBlock();
  eventid_=iEvent_->id().event();
 
  vtxx_=PV_->x();
  vtxy_=PV_->y();
  vtxz_=PV_->z();


  return 1;
}


bool BaseFlatNtp::trigObjMatch(float eta, float phi, std::string path, std::string filter){

  if(filter.compare("")==0) return 1;//no trigger matching required

  for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs_->begin(); obj!=trigObjs_->end(); obj++){
    if(obj->hasSelection(path.c_str())){//HLT path name
      //obj->printSelections(cout);
      if(obj->hasSelection(filter.c_str())){//last filter
	if(reco::deltaR(eta,phi,obj->eta(),obj->phi())<0.3){
	  //obj->printSelections(cout);	  
	  return 1;      
	}
      }
    }
  }
  
  return 0;
}


void BaseFlatNtp::printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList){
  cout<<" Begin MC print out for event: "<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
  for(std::vector<reco::GenParticle>::const_iterator g=genList->begin(); g!=genList->end(); ++g){    
    cout<<"pdgID = "<<g->pdgId()<<" , pt = "<<g->p4().pt()<<" motherRef="<<g->mother()<<endl;
  }
}


