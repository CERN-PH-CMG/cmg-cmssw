#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "TMath.h"
#include "TLorentzVector.h"


LorentzVector min(const LorentzVector& a, const LorentzVector& b){
   if(a.pt()<=b.pt())return a;
   return b;
}


std::string toLatexRounded(double value, double error)
{
  using namespace std;
  if(value==0.0 && error==0.0)return string("");
  
  double power = floor(log10(value));
  if(power<=-3)     {power=power+3;
  }else if(power>=2){power=power-2;
  }else             {power=0;}       
  
   value = value / pow(10,power);
   error = error / pow(10,power);
   int ValueFloating = 1+std::max(-1*log10(error),0.0);
   int ErrorFloating = ValueFloating;
   
   char tmpchar[255];
   if(power!=0){
     sprintf(tmpchar,"$(%.*f\\pm%.*f)\\times 10^{%g}$",ValueFloating,value,ErrorFloating,error,power);
   }else{
     sprintf(tmpchar,"$%.*f\\pm%.*f$",ValueFloating,value,ErrorFloating,error);
   }
   return string(tmpchar);
}


double weightVBF(std::string SampleName, double m_gen, double mass){
   bool isZZ = (SampleName.find("ZZ")!=std::string::npos) ;

   double decay_width = -1;
   double WeightInt  = -1;
         if(m_gen == 130){    decay_width =   0.00487;   if(isZZ){WeightInt = 1.000001; }else{ WeightInt =0.999999; }
   }else if(m_gen == 140){    decay_width =   0.00812;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 150){    decay_width =   0.01730;   if(isZZ){WeightInt = 0.999996; }else{ WeightInt =1.000004; }
   }else if(m_gen == 160){    decay_width =   0.08290;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 170){    decay_width =   0.38000;   if(isZZ){WeightInt = 1.000359; }else{ WeightInt =1.000212; }
   }else if(m_gen == 180){    decay_width =   0.63000;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 190){    decay_width =   1.04000;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 200){    decay_width =   1.43000;   if(isZZ){WeightInt = 0.999749; }else{ WeightInt =0.999183; }
   }else if(m_gen == 300){    decay_width =   8.43000;   if(isZZ){WeightInt = 0.987308; }else{ WeightInt =0.989306; }
   }else if(m_gen == 400){    decay_width =  29.20000;   if(isZZ){WeightInt = 0.951238; }else{ WeightInt =0.950328; }
   }else if(m_gen == 500){    decay_width =  68.00000;   if(isZZ){WeightInt = 0.881467; }else{ WeightInt =0.883517; }
   }else if(m_gen == 600){    decay_width = 123.00000;   if(isZZ){WeightInt = 0.792347; }else{ WeightInt =0.807353; }
   }

   double s = pow(mass,2);
   double weight_BW = s/pow(m_gen,2) * (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width,2)) / (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width*s/(pow(m_gen,2)),2));
   return weight_BW / WeightInt;
}


namespace jet
{
   void computeVariation(LorentzVectorCollection& jets, LorentzVector& met,  std::vector<LorentzVectorCollection>& jetsVar, LorentzVectorCollection& metsVar, JetResolution* ptResol, JetResolution* etaResol, JetResolution* phiResol, JetCorrectionUncertainty* jecUnc){
    jetsVar.clear();
    metsVar.clear();

    int vars[]={JES_UP, JES_DOWN, JER};
    //    std::vector<double> cen,eta,up,down;
    for(size_t ivar=0; ivar<sizeof(vars)/sizeof(int); ivar++)
      {
	LorentzVectorCollection newJets;
	LorentzVector newMet(0,0,0,0),jetDiff(0,0,0,0),jetSum(0,0,0,0);
	for(size_t ijet=0; ijet<jets.size(); ijet++)
	  {
	    if(ivar==JER)
	      {
		//		eta.push_back(jets[ijet].eta()); cen.push_back(jets[ijet].pt()); jetSum += jets[ijet];
		double ptScaleRes = (ptResol->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt())->GetRandom()-1.0);
		//double etaRes = etaResol->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt())->GetRandom();
		//double phiRes = phiResol->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt())->GetRandom();
		double newpt = (1.0+ptScaleRes)*jets[ijet].pt();
		//double neweta = etaRes+jets[ijet].eta();
		//double newphi = phiRes+jets[ijet].phi();
		TLorentzVector pjet(0,0,0,0);
		//		pjet.SetPtEtaPhiM(newpt,neweta,newphi,jets[ijet].mass());
		pjet.SetPtEtaPhiM(newpt,jets[ijet].eta(),jets[ijet].phi(),jets[ijet].mass());
		LorentzVector newJet(pjet.Px(),pjet.Py(),pjet.Pz(),pjet.E());
		newJets.push_back(newJet);
		jetDiff += (newJet-jets[ijet]);
	      }
	    else if(ivar==JES_UP || ivar==JES_DOWN)
	      {
		double varSign=(ivar==JES_UP ? 1.0 : -1.0 );
		jecUnc->setJetEta(jets[ijet].eta());                                                                                                                                                                                  
		jecUnc->setJetPt(jets[ijet].pt());                                                                                                                                                                                    
		double jetScale = 1.0 + varSign*fabs(jecUnc->getUncertainty(true));  
		LorentzVector newJet = jetScale*jets[ijet];
		newJets.push_back(newJet);
		LorentzVector ijetDiff=(newJet-jets[ijet]);
		jetDiff += ijetDiff;
		// 		if(ivar==JES_UP) up.push_back(newJet.pt());
		// 		if(ivar==JES_DOWN) down.push_back(newJet.pt());
		jetSum += newJet;
	      } 
	  }

	//finish computation of the variation
	newMet = -jetDiff +met;
	jetsVar.push_back(newJets);
	metsVar.push_back(newMet);
	// 	if(ivar==JES_UP)        { up.push_back(jetSum.pt()); up.push_back(newMet.pt());}
	// 	else if(ivar==JES_DOWN) { down.push_back(jetSum.pt()); down.push_back(newMet.pt());}
	// 	else                    { cen.push_back(jetSum.pt()); cen.push_back(met.pt()); eta.push_back(0); eta.push_back(0); }
      }
    
    //     for(size_t i=0; i<cen.size(); i++)
    //       {
    // 	std::cout << cen[i] << " " << eta[i] << "\t" << up[i]-cen[i] << "\t" << down[i]-cen[i] << std::endl;
    //      }
}

}
