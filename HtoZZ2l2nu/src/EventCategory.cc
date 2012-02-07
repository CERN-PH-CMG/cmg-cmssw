/*
 *  See header file for a description of this class.
 *
 *  $Date: 2011/11/30 13:46:40 $
 *  $Revision: 1.8 $
 *  \author L. Quertenmont
 */

#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

using namespace std;


EventCategory::EventCategory(bool VBFBinningOnly_){
     VBFBinningOnly = VBFBinningOnly_;
     if(VBFBinningOnly){
        ZZ2l2nuCategoryLabel = new TString[3];
        ZZ2l2nuCategoryLabel[0] = "novbf";
        ZZ2l2nuCategoryLabel[1] = "vbf";
        ZZ2l2nuCategoryLabel[2] = "";
     }else{
        ZZ2l2nuCategoryLabel = new TString[5];
        ZZ2l2nuCategoryLabel[0] = "eq0jets";
        ZZ2l2nuCategoryLabel[1] = "eq1jets";
        ZZ2l2nuCategoryLabel[2] = "geq2jets";
        ZZ2l2nuCategoryLabel[3] = "vbf";
        ZZ2l2nuCategoryLabel[4] = "";
     }
}
EventCategory::~EventCategory(){}

//
int EventCategory::Get(const PhysicsEvent_t& phys, LorentzVectorCollection *variedJetsP4)
{
  bool isGamma(phys.cat>3);
  
  NJetsCentral  = 0;
  NJetsVBF = 0;
  
  //check jet variation
  if(variedJetsP4!=0)
    {
      if(variedJetsP4->size()!= phys.jets.size())
	{
	  cout << "[EventCategory][Get] jet variation passed with different size: reverting to standard collection" << endl;
	  variedJetsP4=0;
	}
    }
  
  for(size_t ijet=0; ijet<phys.jets.size(); ijet++) 
    {
      float jpt=phys.jets[ijet].pt();
      float jeta=phys.jets[ijet].eta();
      if(variedJetsP4!=0)
	{
	  jpt=(*variedJetsP4)[ijet].pt();
	  jeta=(*variedJetsP4)[ijet].eta();
	}
      
      if(jpt<30)continue;
      if(fabs(jeta)<2.5){NJetsCentral++;}
      NJetsVBF++;
    }
  
  bool isVBF = false;
  if(NJetsVBF>=2)
    {
      LorentzVector VBFSyst = phys.jets[0] + phys.jets[1];
      double j1eta=phys.jets[0].eta() ;
      double j2eta=phys.jets[1].eta();
      if(variedJetsP4!=0) 
	{
	  VBFSyst=(*variedJetsP4)[0]+(*variedJetsP4)[1];
	  j1eta=(*variedJetsP4)[0].eta();
	  j2eta=(*variedJetsP4)[1].eta();
	}
      double dEta = fabs(j1eta-j2eta);
      
      int NCentralJet = 0;  int NCentralLepton = 0;  int NBJets = 0;
      double MaxEta, MinEta;
      if(j1eta<j2eta) { MinEta=j1eta; MaxEta=j2eta;}
      else            { MinEta=j2eta; MaxEta=j1eta;}
      for(size_t ijet=2; ijet<phys.jets.size(); ijet++) 
	{
	  float jpt=phys.jets[ijet].pt();
	    float jeta=phys.jets[ijet].eta();
	    if(variedJetsP4!=0)
	      {
		jpt=(*variedJetsP4)[ijet].pt();
		jeta=(*variedJetsP4)[ijet].eta();
	      }
	    if(jpt<30)continue; 
	    if(jeta>MinEta && jeta<MaxEta) NCentralJet++;  
	    if(phys.jets[ijet].btag1>1.7) NBJets++;              //TCHEL
	    //if(phys.jets[ijet].btag2>0.244) NBJets++;          //CSVL
	}
      
      if(!isGamma)
	{
	  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)NCentralLepton++;
	  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)NCentralLepton++;
	}
      else
	{
	  if(phys.gammas[0].eta()>MinEta && phys.gammas[0].eta()<MaxEta)NCentralLepton=2;
	}
      isVBF = (dEta>3.5) && (VBFSyst.M()>450) && (NCentralJet==0) && (NBJets==0) && (NCentralLepton==2);
      //       isVBF = (dEta>4.0) && (VBFSyst.M()>600) && (NCentralJet==0) && (NBJets==0) && (NCentralLepton==2);
    }

  if(isVBF)       return VBF;
  if(NJetsVBF>=2) return GEQ2JETS;
  if(NJetsVBF==1) return EQ1JETS;
  return EQ0JETS;
}

//
TString EventCategory::GetLabel(int CategoryType){
  if(VBFBinningOnly){
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
