/*
 *  See header file for a description of this class.
 *
 *  $Date: 2011/09/17 20:35:20 $
 *  $Revision: 1.5 $
 *  \author L. Quertenmont
 */

#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

using namespace std;


EventCategory::EventCategory(){}
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
	    if(phys.jets[ijet].btag2>1.33 || phys.jets[ijet].btag3>1.74) NBJets++; 
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

  if(isVBF          )return VBF;
  if(NJetsCentral>=2)return GEQ2JETS;
  if(NJetsCentral==1)return EQ1JETS;
  return EQ0JETS;
}

//
TString EventCategory::GetLabel(int CategoryType){
  return ZZ2l2nuCategoryLabel[CategoryType];
}

//
TString EventCategory::GetLabel(const PhysicsEvent_t& phys){
  return ZZ2l2nuCategoryLabel[Get(phys)];
}

 


