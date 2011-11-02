#include "CMGTools/HtoZZ2l2nu/interface/JetEnergyUncertaintyComputer.h"
#include "TMath.h"
#include "TLorentzVector.h"

namespace jet
{

  void UncertaintyComputer::compute(LorentzVectorCollection &jets, LorentzVector &met)
  {
    jetsVar.clear();
    metsVar.clear();
    int vars[]={JES_UP, JES_DOWN, JER};
    for(size_t ivar=0; ivar<sizeof(vars)/sizeof(int); ivar++)
      {
	LorentzVectorCollection newJets;
	LorentzVector newMet(0,0,0,0),jetDiff(0,0,0,0);
	for(size_t ijet=0; ijet<jets.size(); ijet++)
	  {
	    if(ivar==JER)
	      {
		double ptScaleRes = (ptResol_->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt())->GetRandom()-1.0);
		double etaRes = etaResol_->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt())->GetRandom();
		double phiRes = phiResol_->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt())->GetRandom();
		double newpt = (1.0+ptScaleRes)*jets[ijet].pt();
		double neweta = etaRes+jets[ijet].eta();
		double newphi = phiRes+jets[ijet].phi();
		TLorentzVector pjet(0,0,0,0);
		pjet.SetPtEtaPhiM(newpt,neweta,newphi,jets[ijet].mass());
		LorentzVector newJet(pjet.Px(),pjet.Py(),pjet.Pz(),pjet.E());
		newJets.push_back(newJet);
		jetDiff += (newJet-jets[ijet]);
	      }
	    else if(ivar==JES_UP || ivar==JES_DOWN)
	      {
		double varSign=(ivar==JES_UP ? 1.0 : -1.0 );
		jecUnc_->setJetEta(jets[ijet].eta());                                                                                                                                                                                  
		jecUnc_->setJetPt(jets[ijet].pt());                                                                                                                                                                                    
		double jetScale = 1.0 + varSign*jecUnc_->getUncertainty(true);  
		LorentzVector newJet = jetScale*jets[ijet];
		newJets.push_back(newJet);
		jetDiff += (newJet-jets[ijet]);
	      } 
	  }

	//finish computation of the variation
	newMet = -jetDiff +met;
	jetsVar.push_back(newJets);
	metsVar.push_back(newMet);
     }
  }


}
