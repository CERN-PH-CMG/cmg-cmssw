#include "CMGTools/Common/interface/RunInfoAccounting.h"

#include "DataFormats/Histograms/interface/MEtoEDMFormat.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

cmg::RunInfoAccounting::RunInfoAccounting(TFileDirectory& myDir, 
					  const std::string& name):
  initialEvents_(-1.0),
  finalEvents_(0),
  preselectionWeight_(0),
  genCrossSection_(0),
  genFilterEfficiency_(0),
  eventWeight_(0),
  tree_(0){
	
  tree_ = myDir.make<TTree>(name.c_str(),"A tree of the run info");
  tree_->Branch("initialEvents",&initialEvents_,"initialEvents/D");
  tree_->Branch("finalEvents",&finalEvents_,"finalEvents/D");
  tree_->Branch("preselectionWeight",&preselectionWeight_,"preselectionWeight/D");
  tree_->Branch("eventWeight",&eventWeight_,"eventWeight/D");
  tree_->Branch("genCrossSection",&genCrossSection_,"genCrossSection/D");
  tree_->Branch("genFilterEfficiency",&genFilterEfficiency_,"genFilterEfficiency/D");
	
}

void cmg::RunInfoAccounting::processRunInfo(const edm::Run& aRun,
					    unsigned int nTotal, 
					    unsigned int nPassed ){
	
  //   //start by finding the pre-selection efficiency 	
  //   typedef MEtoEDM<double> MEtoEDMD;
  
 
//   edm::Handle<MEtoEDMD> hist;
//   if(!aRun.getByLabel(edm::InputTag("MEtoEDMConverter","MEtoEDMConverterRun"),hist)){
//     std::cout<<"RunInfoAccounting: No MEtoEDM<double> object found. No problem if you're reading data :)" << std::endl;
//     return;
//   }
//   else{
//     std::cout<<"RunInfoAccounting: book keeping weighting information"<<std::endl;
//   }
  
//   initialEvents_ = -1.0;
//   finalEvents_ = DBL_MAX;
  
//   const MEtoEDMD::MEtoEdmObjectVector objects = hist->getMEtoEdmObject();
//   for(MEtoEDMD::MEtoEdmObjectVector::const_iterator it = objects.begin(); it != objects.end(); ++it ){
//     if(it->name == "initialEvents"){
//       initialEvents_ = it->object;
//     }
//     if(it->object < finalEvents_){
//       finalEvents_ = it->object;
//     }
//   }
//   preselectionWeight_ = finalEvents_/initialEvents_;


  
//   initialEvents_ = nTotal;   
  initialEvents_ = nTotal;   
  finalEvents_ = nPassed; 
  preselectionWeight_ = finalEvents_/initialEvents_;

  std::cout<<"RunInfoAccounting: storing cross-section information"<<std::endl;
  
  eventWeight_ = -1;
  genCrossSection_ = -1;
  genFilterEfficiency_ = -1;
  edm::Handle<GenRunInfoProduct> gen;
  if(aRun.getByLabel(edm::InputTag("generator"),gen)){
    genCrossSection_ = gen->crossSection();
    genFilterEfficiency_ = gen->filterEfficiency();
  	
    eventWeight_ = (genCrossSection_*genFilterEfficiency_)/initialEvents_;
  }else{
    eventWeight_ = 1.0;
  }

  tree_->Fill();	
	
}
