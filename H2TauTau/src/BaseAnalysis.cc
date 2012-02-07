#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"


BaseAnalysis::BaseAnalysis():
  TNamed("BaseAnalysis","BaseAnalysis"),
  sample_(NULL),
  verbosity_(0),
  outputpath_("./")
{
}

BaseAnalysis::BaseAnalysis(const char * name):
  TNamed(name,name),
  sample_(NULL),
  verbosity_(0),
  truncateEvents_(100000000),
  printFreq_(100),
  pupWeightName_(""),
  outputpath_("./"),
  smearHistoRes_(0.)
{

}

BaseAnalysis::~BaseAnalysis(){
  
}


bool BaseAnalysis::init(){
  
 
  ///init each sample
  std::vector<Sample*>::const_iterator s=samples_.begin();
  for(int i=0; s!=samples_.end(); ++s, i++){
    samples_[i]->setOutputPath(outputpath_);
    if(!(samples_[i]->init())) return 0;
 
    cout<<"BaseAnalysis Initialized sample "<<samples_[i]->GetName()<<" "<<samples_[i]->GetTitle()<<endl;
  }
  
  return 1;
}

bool BaseAnalysis::createHistos(TString samplename){
  //currently undefined

  return 1;
}

bool BaseAnalysis::addHistos(Sample* s){
  if(!s)return 0;
     
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_runNumberHisto","",1500,160000,175000))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_nVertexHisto","",35,-.5,34.5))))return 0;
  if(!s->addHisto((TH1*)(new TH2F(TString(s->GetName())+"_vertexXYHisto","",100,0.2,0.3,100,0.34,0.44))))return 0;
  if(!s->addHisto((TH1*)(new TH1F(TString(s->GetName())+"_vertexZHisto","",100,-20,20))))return 0;
  
  return 1;
}

bool BaseAnalysis::getHistos(TString tag){
  if(!sample_)return 0;

  if(tag!="")tag=TString("_")+tag;

  if(!(runNumberHisto_=(TH1F*)(sample_->getHisto(TString("runNumberHisto")+tag))))return 0;
  if(!(nVertexHisto_=(TH1F*)(sample_->getHisto(TString("nVertexHisto")+tag))))return 0;
  if(!(vertexXYHisto_=(TH2F*)(sample_->getHisto(TString("vertexXYHisto")+tag))))return 0;
  if(!(vertexZHisto_=(TH1F*)(sample_->getHisto(TString("vertexZHisto")+tag))))return 0;

  return 1;
}


bool BaseAnalysis::fillVariables(const fwlite::Event * event){
    
  runnumber_=event->getRun().run();
  lumiblock_=event->luminosityBlock();
  eventid_=event->id().event();

  event->getByLabel(edm::InputTag("offlinePrimaryVertices"), vertices_);


  ///fill trigger flag
  trigpass_=0;
  edm::Handle< std::vector<cmg::TriggerObject> > trig;
  event->getByLabel(edm::InputTag("cmgTriggerObjectSel","","PAT"),trig);
  if(sample_->getTrigPaths()->size()==0)trigpass_=1;//no trigger requirement
  for(std::vector<std::vector<std::string> >::const_iterator path=sample_->getTrigPaths()->begin(); path!=sample_->getTrigPaths()->end(); path++){
    //trig->begin()->printSelections(cout);
    if(trig->begin()->hasSelection((*path)[0].c_str()))
      if(trig->begin()->getSelection((*path)[0].c_str())){
	trigpass_=1;
      }
  }

  //get trigger object list for later
  event->getByLabel(edm::InputTag("cmgTriggerObjectListSel","","PAT"),trigObjs_);

  ///Event weight definition starts here:
  pupWeight_=1.;//do not comment out needs to be used.
  if(sample_->getDataType()=="MC" || sample_->getDataType()=="MC_SS"
     || sample_->getDataType()=="MCCat" || sample_->getDataType()=="MCCat_SS" 
     || sample_->getDataType()=="Signal"){
    edm::Handle<double>  PupWeight;
    //event->getByLabel(edm::InputTag(pupWeightName_.c_str()), PupWeight);    
    event->getByLabel(edm::InputTag((sample_->getPileupWeight())->c_str()), PupWeight);    
    pupWeight_=(*PupWeight);
  }

  return 1;
}

bool BaseAnalysis::applySelections(){

  if(sample_->getFirstRun()!=0) if(runnumber_<sample_->getFirstRun())return 0;
  if(sample_->getLastRun()!=0) if(sample_->getLastRun()<runnumber_)return 0;

  if(!trigpass_) return 0;

  return 1;
}

bool BaseAnalysis::fillHistos(TString tag){
  if(!BaseAnalysis::getHistos(tag)) return 0;

  runNumberHisto_->Fill(runnumber_);
  
  nVertexHisto_->Fill(vertices_->size(),eventWeight_);
  vertexXYHisto_->Fill(vertices_->begin()->x(),vertices_->begin()->y(),eventWeight_);
  vertexZHisto_->Fill(vertices_->begin()->z(),eventWeight_);

  return 1;
}

bool BaseAnalysis::trigObjMatch(float eta, float phi, std::string path, std::string filter){
  for(std::vector<cmg::TriggerObject>::const_iterator obj=trigObjs_->begin(); obj!=trigObjs_->end(); obj++){
    if(obj->hasSelection(path.c_str())
       &&obj->hasSelection(filter.c_str())
       &&reco::deltaR(eta,phi,obj->eta(),obj->phi())<0.3)
      return 1;      
  }
  
  return 0;
}


void BaseAnalysis::printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList){
  cout<<" Begin MC print out for event: "<<runnumber_<<" "<<lumiblock_<<" "<<eventid_<<endl;
  for(std::vector<reco::GenParticle>::const_iterator g=genList->begin(); g!=genList->end(); ++g){    
    cout<<"pdgID = "<<g->pdgId()<<" , pt = "<<g->p4().pt()<<" motherRef="<<g->mother()<<endl;
  }
}

TH1F* BaseAnalysis::smearHisto(TH1F* h){
  if(!h){
    cout<<"BaseAnalysis::smearHisto : histo is NULL"<<endl;
    return 0;
  }

 
  TH1F* hs=0;
 
  if(smearHistoRes_==0.){//just copy
    hs=(TH1F*)h->Clone(TString(h->GetName())+"smeared");
  }else {
    hs=new TH1F(TString(h->GetName())+"smeared",h->GetTitle(),h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    TF1 gaus("gauss","[0]*exp(-0.5*(x-[1])**2/[2]**2)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    gaus.SetParameter(2,smearHistoRes_);

    for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
      gaus.SetParameter(0,h->GetBinContent(b));
      gaus.SetParameter(1,h->GetBinCenter(b));
      for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
	hs->AddBinContent(bs,gaus.Eval(hs->GetBinCenter(bs)));
      }
    }
    for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
      hs->SetBinError(bs,0.);//not sure this is necessary
    }
    hs->Scale(h->Integral()/hs->Integral());
  }
  
  return hs;
}


void BaseAnalysis::createMCPileUP(){
  std::vector< std::string > filelist;

//   filelist.push_back("/tmp/PFAOD_9966_1_zdV.root");
//   filelist.push_back("/tmp/PFAOD_9967_1_vES.root");
//   filelist.push_back("/tmp/PFAOD_9968_1_xsk.root");
//   filelist.push_back("/tmp/PFAOD_9969_1_FJR.root");
//   filelist.push_back("/tmp/PFAOD_996_0_QU3.root");
//   filelist.push_back("/tmp/PFAOD_9970_1_GpZ.root");
//   filelist.push_back("/tmp/PFAOD_9971_1_gKn.root");
//   filelist.push_back("/tmp/PFAOD_9972_1_AX9.root");
//   filelist.push_back("/tmp/PFAOD_9973_1_Jw3.root");
//   filelist.push_back("/tmp/PFAOD_9974_1_GOd.root");
//   filelist.push_back("/tmp/PFAOD_9975_1_FfF.root");
//   filelist.push_back("/tmp/PFAOD_9976_1_o9x.root");
//   filelist.push_back("/tmp/PFAOD_9977_1_yQM.root");
//   filelist.push_back("/tmp/PFAOD_9978_1_Ib3.root");
//   filelist.push_back("/tmp/PFAOD_9979_1_SfW.root");
//   filelist.push_back("/tmp/PFAOD_997_0_SIl.root");
//   filelist.push_back("/tmp/PFAOD_9980_1_muZ.root");
//   filelist.push_back("/tmp/PFAOD_9981_1_rNU.root");
//   filelist.push_back("/tmp/PFAOD_9982_1_jWS.root");
//   filelist.push_back("/tmp/PFAOD_9983_1_YIQ.root");

//   filelist.push_back("/tmp/PFAOD_9984_1_rz6.root");
//   filelist.push_back("/tmp/PFAOD_9985_1_eyD.root");
//   filelist.push_back("/tmp/PFAOD_9986_1_TFD.root");
//   filelist.push_back("/tmp/PFAOD_9987_1_MZX.root");
//   filelist.push_back("/tmp/PFAOD_9988_1_POR.root");
//   filelist.push_back("/tmp/PFAOD_9989_1_Pbq.root");
//   filelist.push_back("/tmp/PFAOD_998_0_7Ue.root");
//   filelist.push_back("/tmp/PFAOD_9990_1_LDH.root");
//   filelist.push_back("/tmp/PFAOD_9991_1_uhH.root");
//   filelist.push_back("/tmp/PFAOD_9992_1_2MK.root");
//   filelist.push_back("/tmp/PFAOD_9993_1_D40.root");
//   filelist.push_back("/tmp/PFAOD_9994_1_gMg.root");
//   filelist.push_back("/tmp/PFAOD_9995_1_gUm.root");
//   filelist.push_back("/tmp/PFAOD_9996_1_mTE.root");
//   filelist.push_back("/tmp/PFAOD_9997_1_msw.root");
//   filelist.push_back("/tmp/PFAOD_9998_1_Dpn.root");
//   filelist.push_back("/tmp/PFAOD_9999_1_7jU.root");
//   filelist.push_back("/tmp/PFAOD_999_0_Jgy.root");
//   filelist.push_back("/tmp/PFAOD_99_1_Xjk.root");

//    filelist.push_back("/tmp/PFAOD_9501_0_5Pk.root");
//    filelist.push_back("/tmp/PFAOD_9502_0_7wE.root");
//    filelist.push_back("/tmp/PFAOD_9503_0_7jY.root");
//    filelist.push_back("/tmp/PFAOD_9504_0_k8y.root");
//    filelist.push_back("/tmp/PFAOD_9505_0_fVU.root");
//    filelist.push_back("/tmp/PFAOD_9506_1_OrX.root");
//    filelist.push_back("/tmp/PFAOD_9507_1_I0W.root");
//    filelist.push_back("/tmp/PFAOD_9508_0_5Pq.root");
//    filelist.push_back("/tmp/PFAOD_9509_0_y5L.root");
//    filelist.push_back("/tmp/PFAOD_950_0_OfW.root");
//    filelist.push_back("/tmp/PFAOD_9510_0_Z1C.root");
//    filelist.push_back("/tmp/PFAOD_9511_0_i6S.root");
//    filelist.push_back("/tmp/PFAOD_9512_0_F7A.root");
//    filelist.push_back("/tmp/PFAOD_9513_0_C14.root");
//    filelist.push_back("/tmp/PFAOD_9514_0_dQR.root");
//    filelist.push_back("/tmp/PFAOD_9515_0_mGN.root");
//    filelist.push_back("/tmp/PFAOD_9516_0_gkB.root");
//    filelist.push_back("/tmp/PFAOD_9517_0_TCA.root");
//    filelist.push_back("/tmp/PFAOD_9518_0_rIW.root");
//    filelist.push_back("/tmp/PFAOD_9519_0_Cyp.root");

//    filelist.push_back("/tmp/PFAOD_951_0_LaB.root");
//    filelist.push_back("/tmp/PFAOD_9520_1_7MG.root");
//    filelist.push_back("/tmp/PFAOD_9521_1_maZ.root");
//    filelist.push_back("/tmp/PFAOD_9522_1_nQy.root");
//    filelist.push_back("/tmp/PFAOD_9523_1_TQU.root");
//    filelist.push_back("/tmp/PFAOD_9524_1_USc.root");
//    filelist.push_back("/tmp/PFAOD_9525_1_XEI.root");
//    filelist.push_back("/tmp/PFAOD_9526_1_sZS.root");
//    filelist.push_back("/tmp/PFAOD_9527_1_r5f.root");
//    filelist.push_back("/tmp/PFAOD_9528_1_Aum.root");
//    filelist.push_back("/tmp/PFAOD_9529_1_cwB.root");
//    filelist.push_back("/tmp/PFAOD_952_0_4dL.root");
//    filelist.push_back("/tmp/PFAOD_9530_1_D1N.root");
//    filelist.push_back("/tmp/PFAOD_9531_1_gwK.root");
//    filelist.push_back("/tmp/PFAOD_9532_1_idf.root");
//    filelist.push_back("/tmp/PFAOD_9533_1_dMA.root");
//    filelist.push_back("/tmp/PFAOD_9534_1_SMw.root");
//    filelist.push_back("/tmp/PFAOD_9535_1_mp5.root");
//    filelist.push_back("/tmp/PFAOD_9536_1_pWQ.root");

//     filelist.push_back("/tmp/PFAOD_9537_1_IKl.root");
//     filelist.push_back("/tmp/PFAOD_9538_1_CGc.root");
//     filelist.push_back("/tmp/PFAOD_9539_1_KZo.root");
//     filelist.push_back("/tmp/PFAOD_9540_1_5el.root");
//     filelist.push_back("/tmp/PFAOD_9541_1_ZQe.root");
//     filelist.push_back("/tmp/PFAOD_9542_1_eqN.root");
//     filelist.push_back("/tmp/PFAOD_9543_1_iwJ.root");
//     filelist.push_back("/tmp/PFAOD_9544_1_Xu8.root");
//     filelist.push_back("/tmp/PFAOD_9545_1_8se.root");
//     filelist.push_back("/tmp/PFAOD_9546_1_4HM.root");
//     filelist.push_back("/tmp/PFAOD_9547_1_1U2.root");
//     filelist.push_back("/tmp/PFAOD_9548_1_7b8.root");
//     filelist.push_back("/tmp/PFAOD_9549_1_4dN.root");
//     filelist.push_back("/tmp/PFAOD_954_0_ftX.root");
//     filelist.push_back("/tmp/PFAOD_9550_1_xM3.root");
//     filelist.push_back("/tmp/PFAOD_9551_1_QtT.root");
//     filelist.push_back("/tmp/PFAOD_9552_1_yZw.root");
//     filelist.push_back("/tmp/PFAOD_9553_1_Jjw.root");
//     filelist.push_back("/tmp/PFAOD_9554_1_Xc1.root");
//     filelist.push_back("/tmp/PFAOD_953_0_Qa0.root");

    filelist.push_back("/tmp/PFAOD_9555_1_zTX.root");
    filelist.push_back("/tmp/PFAOD_9556_1_Fkk.root");
    filelist.push_back("/tmp/PFAOD_9557_1_AxA.root");
    filelist.push_back("/tmp/PFAOD_9558_1_zUW.root");
    filelist.push_back("/tmp/PFAOD_9559_1_OHu.root");
    filelist.push_back("/tmp/PFAOD_955_0_jqz.root");
    filelist.push_back("/tmp/PFAOD_9560_1_Lin.root");
    filelist.push_back("/tmp/PFAOD_9561_1_o8n.root");
    filelist.push_back("/tmp/PFAOD_9562_1_OsZ.root");
    filelist.push_back("/tmp/PFAOD_9563_1_Nlq.root");
    filelist.push_back("/tmp/PFAOD_9564_1_d3W.root");
    filelist.push_back("/tmp/PFAOD_9565_1_BU7.root");
    filelist.push_back("/tmp/PFAOD_9566_1_0jo.root");
    filelist.push_back("/tmp/PFAOD_9567_1_yne.root");
    filelist.push_back("/tmp/PFAOD_9568_1_HI3.root");
    filelist.push_back("/tmp/PFAOD_9569_1_yUQ.root");
    filelist.push_back("/tmp/PFAOD_956_0_Yzw.root");
    filelist.push_back("/tmp/PFAOD_9570_1_jiS.root");
    filelist.push_back("/tmp/PFAOD_9571_1_vyi.root");



//  for(Int_t i=0;i<=20;i++)
//   filelist.push_back(TString("/tmp/tree_CMG_")+(long)i+".root");

  fwlite::ChainEvent chain(filelist);



  //create a histogram with the MC PUP distribution
  //this must be done with unselected MC events
  //TH1D MCPUPHisto("pileup","",36,-.5,35.5);//must include 0
  TH1D MCPUPHistoRaw("pileupRaw","",50,-.5,49.5);//must include 0
  TH1D MCPUPHisto("pileup","",50,-.5,49.5);//must include 0
  MCPUPHisto.Sumw2();

  int ievt=0;
  for(chain.toBegin(); !chain.atEnd() ; ++chain, ++ievt){
    if(ievt%1000==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();

    //get the number of pile up vertexes
    edm::Handle< std::vector< PileupSummaryInfo > >  PupInfo;
    event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;  
    int npv=-1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
        npv = PVI->getPU_NumInteractions();
        continue;
      }    
    }
    MCPUPHistoRaw.Fill(npv);
    MCPUPHisto.Fill(npv);
    
  }

  TFile F("./MCPUP.root","recreate");
  if(F.IsZombie())return;

  MCPUPHistoRaw.Write();
  
  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  MCPUPHisto.Write();

  F.Close();
  
  return;
}
