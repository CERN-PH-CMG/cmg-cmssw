/*
 *  See header file for a description of this class.
 *
 *  $Date: 2012/04/07 13:19:56 $
 *  $Revision: 1.10 $
 *  \author L. Quertenmont
 */

#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

using namespace std;


EventCategory::EventCategory(int mode_){
     mode = mode_;
     if(mode==1){
        NStates = 3;
        ZZ2l2nuCategoryLabel = new TString[NStates];
        ZZ2l2nuCategoryLabel[0] = "eq0jets";
        ZZ2l2nuCategoryLabel[1] = "eq1jets";
        ZZ2l2nuCategoryLabel[2] = "geq2jets";
     }else if(mode==2){
        NStates = 2;
        ZZ2l2nuCategoryLabel = new TString[NStates];
        ZZ2l2nuCategoryLabel[0] = "novbf";
        ZZ2l2nuCategoryLabel[1] = "vbf";
     }else if(mode==3){
        NStates = 4;
        ZZ2l2nuCategoryLabel = new TString[NStates];
        ZZ2l2nuCategoryLabel[0] = "eq0jets";
        ZZ2l2nuCategoryLabel[1] = "eq1jets";
        ZZ2l2nuCategoryLabel[2] = "geq2jets";
        ZZ2l2nuCategoryLabel[3] = "vbf";
     }else{
        mode = 0;
        NStates = 1;
        ZZ2l2nuCategoryLabel = new TString[NStates];
        ZZ2l2nuCategoryLabel[0] = "";
     }
}
EventCategory::~EventCategory(){}

//
int EventCategory::Get(const PhysicsEvent_t& phys, LorentzVectorCollection* variedJetsP4)
{
  bool isGamma(phys.cat>3);
  
  NJetsCentral  = 0;
  NJets = 0;
  PhysicsObjectJetCollection jets = phys.ajets;

 
  //check jet variation
//  if(variedJetsP4!=0)
//  {
//      if(variedJetsP4->size()!= jets.size()){
//	  cout << "[EventCategory][Get] jet variation passed with different size: reverting to standard collection" << endl;
//	  variedJetsP4=0;
//      }
//  }
  
  for(size_t ijet=0; ijet<jets.size(); ijet++){
      float jpt=jets[ijet].pt();
      float jeta=jets[ijet].eta();
//    if(variedJetsP4!=0){
//	  jpt=(*variedJetsP4)[ijet].pt();
//	  jeta=(*variedJetsP4)[ijet].eta();
//    }
      
      if(jpt<=30)continue;
      NJets++;
      if(fabs(jeta)<=2.5){NJetsCentral++;}
  }

  bool isVBF = false;
  if(NJets>=2){
      LorentzVector VBFSyst = jets[0] + jets[1];
      double j1eta=jets[0].eta() ;
      double j2eta=jets[1].eta();
//      if(variedJetsP4!=0){
//	  VBFSyst=(*variedJetsP4)[0]+(*variedJetsP4)[1];
//	  j1eta=(*variedJetsP4)[0].eta();
//	  j2eta=(*variedJetsP4)[1].eta();
//      }
      double dEta = fabs(j1eta-j2eta);
      
      int NCentralJet = 0;  int NCentralLepton = 0;  int NBJets = 0;
      double MaxEta, MinEta;
      if(j1eta<j2eta) { MinEta=j1eta; MaxEta=j2eta;}
      else            { MinEta=j2eta; MaxEta=j1eta;}
      for(size_t ijet=2; ijet<jets.size(); ijet++){
	  float jpt=jets[ijet].pt();
          float jeta=jets[ijet].eta();
	  if(variedJetsP4!=0){
		jpt=(*variedJetsP4)[ijet].pt();
		jeta=(*variedJetsP4)[ijet].eta();
	    }
	    if(jpt<30)continue; 
	    if(jeta>MinEta && jeta<MaxEta) NCentralJet++;  
	    if(jets[ijet].btag1>2.0) NBJets++;
      }
      
      if(!isGamma){
	  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)NCentralLepton++;
	  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)NCentralLepton++;
      }else{
	  if(phys.gammas[0].eta()>MinEta && phys.gammas[0].eta()<MaxEta)NCentralLepton=2;
      }
      isVBF = (dEta>4.0) && (VBFSyst.M()>500) && (NCentralJet==0) && (NBJets==0) && (NCentralLepton==2);
    }



  switch(mode){
     case 1:{
        if(NJets>=2) return 2;
        if(NJets==1) return 1;
        return 0;
     }break;
     case 2:{
	if(isVBF)return 1;
	return 0;
     }break;
     case 3:{
        if(isVBF)return 3;
        if(NJets>=2) return 2;
        if(NJets==1) return 1;
        return 0;
     }break;
     case 0:
     default:{
        return 0;      
     }break;
  }
}

//
TString EventCategory::GetLabel(int CategoryType){
  if(mode==0){
     if(CategoryType<=2)return ZZ2l2nuCategoryLabel[0];
     return ZZ2l2nuCategoryLabel[1];
  }else{
     return ZZ2l2nuCategoryLabel[CategoryType];
  }
}

//
TString EventCategory::GetLabel(const PhysicsEvent_t& phys){
  return GetLabel(Get(phys));
}
