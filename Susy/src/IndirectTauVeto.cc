#include "CMGTools/Susy/interface/IndirectTauVeto.h"

double getLTpt(const pat::Jet* myjet, unsigned int type) {
  
  double LTpt=0.;
  double LTeta=999.;
  double LTphi=999.;
  double LTrap=999.;
  //  double LTpt=9999.;

  
  const std::vector<reco::PFCandidatePtr>&         constituents    = myjet->getPFConstituents();
  
  const unsigned int                                  numConstituents =constituents.size();
  
  for (unsigned int iCon = 0; iCon < numConstituents; ++iCon) {
    
    const reco::PFCandidatePtr&               constituent     =  constituents[iCon];
    
    if((constituent->particleId()==reco::PFCandidate::h) || (constituent->particleId()==reco::PFCandidate::mu) || (constituent->particleId()==reco::PFCandidate::e)) {
      
      if(constituent->pt()>LTpt) {
	
	LTpt=constituent->pt();
	LTeta=constituent->eta();
	LTphi=constituent->phi();
//	LTrap=constituent->p4().Rapidity();
	LTrap=constituent->rapidity();
	
      }
      
    }

  }
  
  if(type==1) return LTpt;
  if(type==2) return LTeta;
  if(type==3) return LTphi;
  if(type==4) return LTrap;

  return 0;  

}


unsigned int getCHMdr(const pat::Jet* myjet, double DR, double pt_) {
  
  unsigned int CHM=0;
  //  double LTpt=0.;
  //  double LTpt=9999.;

  const std::vector<reco::PFCandidatePtr>&         constituents    = myjet->getPFConstituents();

  const unsigned int                                  numConstituents =constituents.size();
  
  //  double LTeta=getLTpt(myjet,2);
  double LTphi=getLTpt(myjet,3);
  double LTrap=getLTpt(myjet,4);

  double riJ=deltaR( myjet->rapidity(), myjet->phi(),LTrap,LTphi);

  if(riJ>0.1) {
    CHM=9999.;
    return CHM;
  }

  ////////
  ////////
  ////////

  for (unsigned int iCon = 0; iCon < numConstituents; ++iCon) {
    
    const reco::PFCandidatePtr&               constituent     =  constituents[iCon];
                
    if((constituent->particleId()==reco::PFCandidate::e) || (constituent->particleId()==reco::PFCandidate::mu) || (constituent->particleId()==reco::PFCandidate::h)) {      

      double ri=deltaR(constituent->rapidity(),constituent->phi(),LTrap,LTphi);
      
      if(pt_==0 && ri<DR) CHM++;
      if(DR==0 && constituent->pt()>pt_) CHM++;
      if((pt_!=0) && (DR!=0) && ri<DR && constituent->pt()>pt_) CHM++;
      //    CHM++;

    }

  }
  
  return CHM;

}


double getLRM(const pat::Jet* myjet) {
  
  double LRM=0.;

  const std::vector<reco::PFCandidatePtr>&         constituents    = myjet->getPFConstituents();

  const unsigned int                                  numConstituents =constituents.size();
  
  /////  double LTeta=getLTpt(myjet,2);
  double LTphi=getLTpt(myjet,3);
  double LTrap=getLTpt(myjet,4);

  double riJ=deltaR( myjet->rapidity(), myjet->phi(),LTrap,LTphi);

  if(riJ>0.1) {
    LRM=9999.;
    return LRM;
  }

  for (unsigned int iCon = 0; iCon < numConstituents; ++iCon) {
    
    const reco::PFCandidatePtr&               constituent     =  constituents[iCon];
    
    if((constituent->particleId()==reco::PFCandidate::e) || (constituent->particleId()==reco::PFCandidate::mu) || (constituent->particleId()==reco::PFCandidate::h)) {      

      double ri=deltaR(constituent->rapidity(),constituent->phi(),LTrap,LTphi);
      
      LRM = LRM + constituent->pt()*ri;
      
    }
    
  }

  LRM=LRM/myjet->pt();

  return LRM;

}


bool tauVeto(const std::vector<const pat::Jet*> &  myNonBPurJet, double met, double metPhi){
  
  bool foundTau=false;
  
  for ( unsigned int i = 0; i < myNonBPurJet.size(); ++i) {     

    double lrm=getLRM(myNonBPurJet.at(i));
    unsigned int CHM03_pt1=getCHMdr(myNonBPurJet.at(i), 0.3, 1.);

    double dPhi=fabs(deltaPhi(myNonBPurJet.at(i)->phi(),metPhi));   
    double mtW_jMet=sqrt(2*(myNonBPurJet.at(i)->pt()*met)*(1-cos(dPhi)));

    if(mtW_jMet>90 ) continue;

    //    if(CHM03_pt1>4) continue;
   
    double jetVariation1=(CHM03_pt1>4 && myNonBPurJet.at(i)->pt()<80);
    double jetVariation2=(CHM03_pt1>5 && myNonBPurJet.at(i)->pt()>=80);
    
    if(jetVariation1 || jetVariation2) continue;

    double lrmCut=0.05;
    if(myNonBPurJet.at(i)->pt()<40) lrmCut=-0.000875*myNonBPurJet.at(i)->pt()+0.0875;

    if(lrm>lrmCut) continue;
    
    foundTau=true;
    
  }

  return foundTau;
  
}

