/*
 *  See header file for a description of this class.
 *
 *  $Date: 2011/08/05 10:37:05 $
 *  $Revision: 1.12 $
 *  \author L. Quertenmont
 */

#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

using namespace std;


EventCategory::EventCategory(){}
EventCategory::~EventCategory(){}


int EventCategory::Get(const PhysicsEvent_t& phys){
    int NJetsEta25  = 0;
    int NJetsEtaMax = 0;
    for(size_t ijet=0; ijet<phys.jets.size(); ijet++) {
      if(phys.jets[ijet].pt()<30)continue;
      if(fabs(phys.jets[ijet].eta())<2.5){NJetsEta25++;}
      NJetsEtaMax++;
    }

    bool isVBF = false;
    if(NJetsEtaMax>=2){
       LorentzVector VBFSyst = phys.jets[0] + phys.jets[1];
       double dEta = fabs(phys.jets[0].eta() - phys.jets[1].eta());
       int NCentralJet = 0;  int NCentralLepton = 0;  int NBJets = 0;
       double MaxEta, MinEta;if(phys.jets[0].eta()<phys.jets[1].eta()){MinEta=phys.jets[0].eta(); MaxEta=phys.jets[1].eta();}else{MinEta=phys.jets[1].eta(); MaxEta=phys.jets[0].eta();}
       for(size_t ijet=2; ijet<phys.jets.size(); ijet++) {if(phys.jets[ijet].pt()<30)continue; if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)NCentralJet++;  if(phys.jets[ijet].btag1>1.7)NBJets++; }
       if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)NCentralLepton++;
       if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)NCentralLepton++;
       isVBF = (dEta>4.0) && (VBFSyst.M()>600) && (NCentralJet==0) && (NBJets==0) && (NCentralLepton==2);
    }

    if(isVBF        )return VBF;
    if(NJetsEta25>=2)return GEQ2JETS;
    if(NJetsEta25==1)return EQ1JETS;
    return EQ0JETS;
}

TString EventCategory::GetLabel(int CategoryType){
   return ZZ2l2nuCategoryLabel[CategoryType];
}

TString EventCategory::GetLabel(const PhysicsEvent_t& phys){
   return ZZ2l2nuCategoryLabel[Get(phys)];
}

 


