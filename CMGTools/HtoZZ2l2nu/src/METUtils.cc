/*
 *  See header file for a description of this class.
 *
 *  $Date: 2012/04/03 19:56:04 $
 *  $Revision: 1.10 $
 *  \author G. Cerminara & D. Trocino & P. Silva & L. Quertenmont
 */

#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TVector2.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TRandom.h"

using namespace std;

namespace METUtils{


std::pair<TVector2, TVector2> defineThrust(const LorentzVector& l1, double sigmaPt1, const LorentzVector& l2, double sigmaPt2, int& event_categ){
  //define the leading and sub-leading lepton
  const LorentzVector &lepton1( l1.pt() > l2.pt() ? l1 : l2);
  const LorentzVector &lepton2( l1.pt() > l2.pt() ? l2 : l1);
  double dphill=fabs(deltaPhi(lepton1.phi(),lepton2.phi()));
  TVector2 lead(lepton1.px(),lepton1.py());
  TVector2 trailer(lepton2.px(),lepton2.py());
  TVector2 reference(lead);

  //define the thrust and dilepton
  TVector2 dil = lead+trailer;
  TVector2 thr = lead-trailer;

  //define the longitudinal and perpendicular axis
  TVector2 longi,perp;
  if(dphill>=TMath::Pi()/2){
      event_categ=OPENANGLE;
      longi = thr.Unit();
      perp = longi.Rotate(TMath::Pi()/2);
      if(perp*reference<0) perp *= -1;
  }else{
      event_categ=COLLIMATED;
      perp=dil.Unit();
      longi = perp.Rotate(TMath::Pi()/2);
      if(longi*reference<0) longi *= -1;
  }
  return std::pair<TVector2, TVector2>(longi,perp);
}

LorentzVector redMET(RedMetType Type, const LorentzVector& theLepton1, double sigmaPt1, const LorentzVector& theLepton2, double sigmaPt2, const LorentzVectorCollection& theJets, const LorentzVector& theMET, bool isZcandidate, stRedMET* out, double kRecoil_l, double kRecoil_t, double kSigmaPt_l, double kSigmaPt_t, double kPerp){
  LorentzVector clusteredMetP4 = -1*(theLepton1+theLepton2);  for(unsigned int i=0;i<theJets.size();i++){clusteredMetP4 -= theJets[i];}
  return redMET(Type, theLepton1, sigmaPt1, theLepton2, sigmaPt2, clusteredMetP4, theMET, theMET, isZcandidate, out, kRecoil_l, kRecoil_t, kSigmaPt_l, kSigmaPt_t, kPerp);
}

LorentzVector redMET(RedMetType Type, const LorentzVector& theLepton1, double sigmaPt1, const LorentzVector& theLepton2, double sigmaPt2, const LorentzVector& theMET1, const LorentzVector& theMET2, bool isZcandidate, stRedMET* out, double kRecoil_l, double kRecoil_t, double kSigmaPt_l, double kSigmaPt_t, double kPerp){
  return redMET(Type, theLepton1, sigmaPt1, theLepton2, sigmaPt2, theMET1, theMET2, theMET2, isZcandidate, out, kRecoil_l, kRecoil_t, kSigmaPt_l, kSigmaPt_t, kPerp);
}


LorentzVector redMET(RedMetType Type, const LorentzVector& theLepton1, double sigmaPt1, const LorentzVector& theLepton2, double sigmaPt2, const LorentzVector& theMET1, const LorentzVector& theMET2, const LorentzVector& theMET3, bool isZcandidate, stRedMET* out, double kRecoil_l, double kRecoil_t, double kSigmaPt_l, double kSigmaPt_t, double kPerp){
  TVector2 a_l,a_t;
  TVector2 redMETxy;
  double redMET_l,redMET_t;
  double redMET;
  int prefRecminRmet;
  int prefRec_l, prefRec_t;
  double recoilProj_l,recoilProj_t;
  double dileptonProj_l,dileptonProj_t;
  double deltaDileptonProj_l, deltaDileptonProj_t;

  //the dilepton candidate
  TVector2 dil(theLepton1.px()+theLepton2.px(),theLepton1.py()+theLepton2.py());

  //define the thrust
   int event_categ;
  if(!isZcandidate){
       std::pair<TVector2, TVector2> thrust=defineThrust(theLepton1,sigmaPt1,theLepton2,sigmaPt2, event_categ);
       a_l=thrust.first;  
       a_t=thrust.second;
  }else {
       dil = TVector2(theLepton1.px(),theLepton1.py());
       a_t = dil.Unit();
       a_l = a_t.Rotate(TMath::Pi()/2);
  }

  //project the dilepton
  dileptonProj_l = dil*a_l;
  dileptonProj_t = dil*a_t;

  //project the met
  TVector2 pfMET1(theMET1.Px(),theMET1.py());
  TVector2 uncl1=pfMET1+dil;
  double uncl1Proj_l = uncl1*a_l;
  double uncl1Proj_t = uncl1*a_t;
  TVector2 pfMET2(theMET2.Px(),theMET2.py());
  TVector2 uncl2=pfMET2+dil;
  double uncl2Proj_l = uncl2*a_l;
  double uncl2Proj_t = uncl2*a_t;
  TVector2 pfMET3(theMET3.Px(),theMET3.py());
  TVector2 uncl3=pfMET3+dil;
  double uncl3Proj_l = uncl3*a_l;
  double uncl3Proj_t = uncl3*a_t;

  //take the minimum recoil possible depending on the event category type
  recoilProj_l = min(-1.0*uncl1Proj_l, min(-1.0*uncl2Proj_l,-1.0*uncl3Proj_l));
  recoilProj_l=min(recoilProj_l,0.);
  recoilProj_t = min(-1.0*uncl1Proj_t, min(-1.0*uncl2Proj_t,-1.0*uncl3Proj_t));
  recoilProj_t=min(recoilProj_t,0.);   

  //propagate the lepton uncertainty (if larger than unity assume 100% rel. uncertainty)                                                                                                                                                                                                                                    
  if(!isZcandidate){
      double relErrPt1 = min(sigmaPt1/theLepton1.pt(), 1.);
      double relErrPt2 = min(sigmaPt2/theLepton2.pt(), 1.);
      LorentzVector loweredLepton1 = theLepton1*(1.0-relErrPt1);
      LorentzVector loweredLepton2 = theLepton2*(1.0-relErrPt2);
      std::pair<TVector2, TVector2> loweredThrust=defineThrust(loweredLepton1,sigmaPt1,loweredLepton2,sigmaPt2, event_categ);
      float loweredDileptonProj_t = dil*loweredThrust.second;
      deltaDileptonProj_t = loweredDileptonProj_t - dileptonProj_t;
      deltaDileptonProj_l = ( -relErrPt1*TVector2(theLepton1.px(),theLepton1.py()) + relErrPt2*TVector2(theLepton2.px(),theLepton2.py()) )*a_l;
  }else{
      deltaDileptonProj_t=0;
      deltaDileptonProj_l=0;
  }

  double uncl1RedMet_l = dileptonProj_l - kRecoil_l*uncl1Proj_l + kSigmaPt_l*deltaDileptonProj_l;
  double uncl1RedMet_t = dileptonProj_t - kRecoil_t*uncl1Proj_t + kSigmaPt_t*deltaDileptonProj_t;
  double uncl1RedMet   = sqrt(pow(uncl1RedMet_l,2)+pow(uncl1RedMet_t,2));

  double uncl2RedMet_l = dileptonProj_l - kRecoil_l*uncl2Proj_l + kSigmaPt_l*deltaDileptonProj_l;
  double uncl2RedMet_t = dileptonProj_t - kRecoil_t*uncl2Proj_t + kSigmaPt_t*deltaDileptonProj_t;
  double uncl2RedMet   = sqrt(pow(uncl2RedMet_l,2)+pow(uncl2RedMet_t,2));

  double uncl3RedMet_l = dileptonProj_l - kRecoil_l*uncl3Proj_l + kSigmaPt_l*deltaDileptonProj_l;
  double uncl3RedMet_t = dileptonProj_t - kRecoil_t*uncl3Proj_t + kSigmaPt_t*deltaDileptonProj_t;
  double uncl3RedMet   = sqrt(pow(uncl3RedMet_l,2)+pow(uncl3RedMet_t,2));

  if(Type==D0){
     //
     // D0 ORIGINAL VERSION
     //
     //recoil is minimized independently
     recoilProj_l = min(-1.0*uncl1Proj_l, min(-1.0*uncl2Proj_l,-1.0*uncl3Proj_l));
     recoilProj_l = min(recoilProj_l,0.);
     prefRec_l    = ( (-1.0*uncl1Proj_l)< min(-1.*uncl2Proj_l,-1.*uncl3Proj_l) ? CLUSTERED : UNCLUSTERED);

     recoilProj_t = min(-1.0*uncl1Proj_t, min(-1.0*uncl2Proj_t,-1.0*uncl3Proj_t));
     recoilProj_t = min(recoilProj_t,0.);
     prefRec_t    = ( (-1.0*uncl1Proj_t)<min(-1.*(uncl2Proj_t),-1.*(uncl3Proj_t)) ? CLUSTERED : UNCLUSTERED);
  
     //compute the reduced met components
     redMET_l = max( (dileptonProj_l + kRecoil_l*recoilProj_l + kSigmaPt_l*deltaDileptonProj_l), 0.);
     redMET_t = max( (dileptonProj_t + kRecoil_t*recoilProj_t + kSigmaPt_t*deltaDileptonProj_t), 0.);
     redMET   = sqrt( pow(redMET_l,2) + pow(redMET_t,2) );
     redMETxy =redMET_l*a_l+redMET_t*a_t;

  }else if(Type==MINIMIZED){
     //
     // CMS MINIMIZED VERSION
     //   
     if(uncl1RedMet<uncl2RedMet && uncl1RedMet<uncl3RedMet){
        prefRecminRmet         = UNCLUSTERED;
        redMET_l = uncl1RedMet_l;
        redMET_t = uncl1RedMet_t;
     }else if(uncl2RedMet<uncl1RedMet && uncl2RedMet<uncl3RedMet){
        prefRecminRmet         = CLUSTERED;
        redMET_l = uncl2RedMet_l;
        redMET_t = uncl2RedMet_t;
     }else{
        prefRecminRmet         = CLUSTERED;
        redMET_l = uncl3RedMet_l;
        redMET_t = uncl3RedMet_t;
     }
     redMET   = sqrt(pow(redMET_l,2)+pow(redMET_t,2));
     redMETxy =redMET_l*a_l+redMET_t*a_t;

  }else if(INDEPENDENTLYMINIMIZED){
     //
     // CMS INDEPEDENT MINIMIZATION VERSION
     //
     prefRec_l     = (fabs(uncl1RedMet_l) < fabs(uncl2RedMet_l) ? UNCLUSTERED : CLUSTERED );
     prefRec_t     = (fabs(uncl1RedMet_t) < fabs(uncl2RedMet_t) ? UNCLUSTERED : CLUSTERED );

     if(fabs(uncl1RedMet_l)<fabs(uncl2RedMet_l) && fabs(uncl1RedMet_l)<fabs(uncl3RedMet_l)){
        prefRec_l  = UNCLUSTERED;
        redMET_l   = uncl1RedMet_l;
     }else if(fabs(uncl2RedMet_l)<fabs(uncl1RedMet_l) && fabs(uncl2RedMet_l)<fabs(uncl3RedMet_l)){
        prefRec_l  = CLUSTERED;
        redMET_l   = uncl2RedMet_l;
     }else{
        prefRec_l  = CLUSTERED;
        redMET_l   = uncl3RedMet_l;
     }

     if(fabs(uncl1RedMet_t)<fabs(uncl2RedMet_t) && fabs(uncl1RedMet_t)<fabs(uncl3RedMet_t)){
        prefRec_t  = UNCLUSTERED;
        redMET_t   = uncl1RedMet_t;
     }else if(fabs(uncl2RedMet_t)<fabs(uncl1RedMet_t) && fabs(uncl2RedMet_t)<fabs(uncl3RedMet_t)){
        prefRec_t  = CLUSTERED;
        redMET_t   = uncl2RedMet_t;
     }else{
        prefRec_t  = CLUSTERED;
        redMET_t   = uncl3RedMet_t;
     }
     redMET = sqrt(pow(redMET_l,2)+pow(redMET_t,2));
     redMETxy=redMET_l*a_l+redMET_t*a_t;
  }


  if(out){
     out->a_l = a_l;
     out->a_t = a_t;
     out->redMETxy = redMETxy;
     out->redMET_l = redMET_l;
     out->redMET_t = redMET_t;
  }
  return LorentzVector( redMETxy.X(),redMETxy.Y(),0,redMET);  
}




  double getRedMETCut(int eventCategory, int workingpoint){
    double cut = 0;
    switch(eventCategory){
      case EventCategory::EQ0JETS :
	if(workingpoint==MEDIUMWP) cut=30.5;
	if(workingpoint==TIGHTWP)  cut=39.0;
	break;
      case EventCategory::EQ1JETS :
	if(workingpoint==MEDIUMWP) cut=38.8;
	if(workingpoint==TIGHTWP)  cut=52.4;
	break;
      case EventCategory::GEQ2JETS :
	if(workingpoint==MEDIUMWP) cut=50.8;
	if(workingpoint==TIGHTWP)  cut=78.7;
	break;
      case EventCategory::VBF :
	if(workingpoint==MEDIUMWP) cut=57.36;  //the tuned point 56.3
	if(workingpoint==TIGHTWP)  cut=60.20;
	break;
      }
    return cut;
  }


  LorentzVector projectedMET(const LorentzVector& lepton1, const LorentzVector& lepton2, const LorentzVector& met, stRedMET* out){
    TVector2 a_l,a_t;
    double projectedMET_long, projectedMET_perp;
    double projEtMiss;
    int event_categ;
    double minDeltaPhi;

        double dphi1=deltaPhi(lepton1.phi(),met.phi());
        double dphi2=deltaPhi(lepton2.phi(),met.phi());
        minDeltaPhi = (fabs(dphi1) < fabs(dphi2) ? dphi1 : dphi2);
        projEtMiss = met.pt() * ( fabs(minDeltaPhi) > TMath::Pi()/2 ? 1.0 : fabs(TMath::Sin(fabs(minDeltaPhi))) );
        event_categ= ( fabs(minDeltaPhi) > TMath::Pi()/2 ? OPENANGLE : COLLIMATED );

        //project along / transversely to the closest lepton
        LorentzVector refLepton( (fabs(dphi1) < fabs(dphi2) ? lepton1 : lepton2) );
        TVector2 refLeptonPt(refLepton.px(),refLepton.py());
        TVector2 metxy(met.px(),met.py());
        a_l =  refLeptonPt.Unit();
        a_t = a_l.Rotate(TMath::Pi()/2);
        if(a_t*metxy<0) a_t *=-1;
        projectedMET_long = (event_categ==COLLIMATED ? 0. : metxy*a_l);
        projectedMET_perp = metxy*a_t;
        TVector2 projMETxy=a_l*projectedMET_long+a_t*projectedMET_perp;

        if(out){
           out->a_l = a_l;
           out->a_t = a_t;
           out->redMETxy = projMETxy;
           out->redMET_l = projectedMET_long;
           out->redMET_t = projectedMET_perp;
        }

        return LorentzVector( projMETxy.X(),projMETxy.Y(),0,projEtMiss);
  }




  double transverseMass(LorentzVector &visible, LorentzVector &invisible, bool assumeSameMass){
    if(assumeSameMass){
          LorentzVector sum=visible+invisible;
          double tMass = TMath::Power(TMath::Sqrt(TMath::Power(visible.pt(),2)+pow(visible.mass(),2))+TMath::Sqrt(TMath::Power(invisible.pt(),2)+pow(visible.mass(),2)),2);
          tMass-=TMath::Power(sum.pt(),2);
          return TMath::Sqrt(tMass);
      }else{
          double dphi=fabs(deltaPhi(invisible.phi(),visible.phi()));
          return TMath::Sqrt(2*invisible.pt()*visible.pt()*(1-TMath::Cos(dphi)));
      }
      return -1;
  }

  //
  LorentzVector correctForPhiAsymmetry(LorentzVector &met,float sumEt,bool isMC, bool isRunA)
  {
    //cf. https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=174318
    double cx0( isMC ? (isRunA ? -0.09389  : -0.1070    ) : (isRunA ? -0.3365   : -0.3265) );
    double cx1( isMC ? (isRunA ? 0.0001815 : 0.00009587 ) : (isRunA ? 0.004801  : 0.005162) );
    double cy0( isMC ? (isRunA ? 0.1571    : 0.01517    ) : (isRunA ? 0.2578    : -0.1956) );
    double cy1( isMC ? (isRunA ? -0.003710 : -0.003357  ) : (isRunA ? -0.006124 : -0.006299) );

    double px=met.px()-(cx0+cx1*sumEt);
    double py=met.py()-(cy0+cy1*sumEt);
    return LorentzVector(px,py,0,sqrt(px*px+py*py));
  }


  //
  LorentzVector smearedJet(const LorentzVector &origJet, double genJetPt, int mode)
  {
    if(genJetPt<=0) return origJet;

    //smearing factors are described in https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    double eta=fabs(origJet.eta());
    double ptSF(1.0), ptSF_err(0.06);
    if(eta<0.5) { ptSF=1.066; ptSF_err=sqrt(pow(0.007,2)+pow(0.5*(0.07+0.072),2)); }
    else if(eta>=0.5 && eta<1.7) { ptSF=1.191; ptSF_err=sqrt(pow(0.019,2)+pow(0.5*(0.06+0.062),2)); }
    else if(eta>=1.7 && eta<2.3) { ptSF=1.096; ptSF_err=sqrt(pow(0.030,2)+pow(0.5*(0.08+0.085),2)); }
    else if(eta>=2.3 && eta<5.0) { ptSF=1.166; ptSF_err=sqrt(pow(0.050,2)+pow(0.5*(0.19+0.199),2)); }

  //   if(eta<0.5)                  { ptSF=1.052;  ptSF_err=sqrt(pow(0.012,2)+pow(0.5*(0.062+0.061),2)); }
    //     else if(eta>=0.5 && eta<1.1) { ptSF=1.057;  ptSF_err=sqrt(pow(0.012,2)+pow(0.5*(0.056+0.055),2)); }
    //     else if(eta>=1.1 && eta<1.7) { ptSF=1.1096; ptSF_err=sqrt(pow(0.017,2)+pow(0.5*(0.063+0.062),2)); }
    //     else if(eta>=1.7 && eta<2.3) { ptSF=1.134;  ptSF_err=sqrt(pow(0.035,2)+pow(0.5*(0.087+0.085),2)); }
    //     else if(eta>=2.3 && eta<5.0) { ptSF=1.288;  ptSF_err=sqrt(pow(0.127,2)+pow(0.5*(0.155+0.153),2)); }
    if(mode==1) ptSF += ptSF_err;
    if(mode==2) ptSF -= ptSF_err;
    //ptSF=max(0.,(genJetPt+ptSF*(origJet.pt()-genJetPt)))/origJet.pt();                      //deterministic version
    ptSF=max(0.,(genJetPt+gRandom->Gaus(ptSF,ptSF_err)*(origJet.pt()-genJetPt)))/origJet.pt();  //deterministic version
    if(ptSF<=0) return origJet;
    
    double px(origJet.px()*ptSF), py(origJet.py()*ptSF), pz(origJet.pz()), mass(origJet.mass());
    double en = sqrt(mass*mass+px*px+py*py+pz*pz);
    
    //return new kinematics
    return LorentzVector(px,py,pz,en);
  }

  //
  void computeVariation(LorentzVectorCollection& jets, std::vector<double> &genjetsPt, LorentzVector& met,   
			std::vector<LorentzVectorCollection>& jetsVar, LorentzVectorCollection& metsVar,
			JetCorrectionUncertainty *jecUnc)
  {
    jetsVar.clear();
    metsVar.clear();

    int vars[]={JER, JER_UP, JER_DOWN, JES_UP, JES_DOWN};
    for(size_t ivar=0; ivar<sizeof(vars)/sizeof(int); ivar++)
      {
	LorentzVectorCollection newJets;
	LorentzVector newMet(met),jetDiff(0,0,0,0);
	int mode(0); if(ivar==JER_UP) mode=1; if(ivar==JER_DOWN) mode=2;
	for(size_t ijet=0; ijet<jets.size(); ijet++)
	  {
	    LorentzVector iSmearJet=METUtils::smearedJet(jets[ijet],genjetsPt[ijet],mode);
	    if(ivar==JER || ivar==JER_UP || ivar==JER_DOWN)
	      {
		newJets.push_back(iSmearJet);
		jetDiff += (iSmearJet-jets[ijet]);
	      }
	    else if(ivar==JES_UP || ivar==JES_DOWN)
	      {
		double varSign=(ivar==JES_UP ? 1.0 : -1.0 );
		double jetScale(1.0);
		try{
		  jecUnc->setJetEta(jets[ijet].eta());
		  jecUnc->setJetPt(jets[ijet].pt());
		  jetScale = 1.0 + varSign*fabs(jecUnc->getUncertainty(true));  
		}
		catch(std::exception &e){
		  //cout << e.what() << ijet << " " << iSmearJet.pt() << " " << jets[ijet].pt() << " : " << genjetsPt[ijet] << endl;
		}
		LorentzVector newJet = jetScale*iSmearJet;
		newJets.push_back(newJet);
		LorentzVector ijetDiff=(newJet-iSmearJet);
		jetDiff += ijetDiff;
	      } 
	  }
	
	//finish computation of the variation
	jetsVar.push_back(newJets);
	newMet -= jetDiff; metsVar.push_back(newMet);

	//SOME DEBUGING PRINTOUT... CAN BE SAFELY REMOVED
        if(ivar==JER && newMet.pt()>2000){
           printf("%f - %f - %f : ",newMet.pt(), met.pt(), jetDiff.pt());
           for(size_t ijet=0; ijet<jets.size(); ijet++){
              LorentzVector iSmearJet=METUtils::smearedJet(jets[ijet],genjetsPt[ijet],mode);
              printf("Jet%i %f(%f)>%f  ", (int)ijet, jets[ijet].pt(), genjetsPt[ijet], iSmearJet.pt());
           }printf("\n");
        }

      }
  }

  //
  LorentzVector SmearJetFormGen(LorentzVectorCollection& jetsP4, LorentzVector& met, std::vector<double> GenJet, std::vector<LorentzVector>& jetsJer)
  {
     jetsJer.clear();
     LorentzVector jetDiff;
     LorentzVector newMetJer;
     TRandom *gen_ = new TRandom();
     for(int unsigned ijet=0; ijet<jetsP4.size(); ijet++){
       float shift = 999.;
       float smear = -999.;
       // value from https://cms.web.cern.ch/org/jets-and-missing-et-pog
       if (fabs(jetsP4[ijet].eta())<=1.1)                                 smear = 1.066;
       if (fabs(jetsP4[ijet].eta())<=1.7 && fabs(jetsP4[ijet].eta())>1.1) smear = 1.191;
       if (fabs(jetsP4[ijet].eta())<=2.3 && fabs(jetsP4[ijet].eta())>1.7) smear = 1.096;
       if (fabs(jetsP4[ijet].eta())>2.3)                                  smear = 1.166;

       if( GenJet[ijet]!=0. )          shift = (smear-1.) * (jetsP4[ijet].pt() - GenJet[ijet] )/jetsP4[ijet].pt();
       else {
         double expres = ErrEt(jetsP4[ijet].pt(),jetsP4[ijet].eta());
         double relsmear = expres * sqrt(smear*smear-1.);
         shift = gen_->Gaus(0.,relsmear);
       }
       float ptSmeared  = jetsP4[ijet].pt();
       float eneSmeared = jetsP4[ijet].e();
       if( shift>-1 && shift < 2) {
         ptSmeared  *= (1 + shift);
         eneSmeared *= (1 + shift);
       }
       TLorentzVector newJet1; newJet1.SetPtEtaPhiE(ptSmeared,jetsP4[ijet].eta(),jetsP4[ijet].phi(),eneSmeared);
       LorentzVector newJet(newJet1.Px(),newJet1.Py(),newJet1.Pz(),newJet1.E());
       jetDiff += ( newJet -jetsP4[ijet]);
       jetsJer.push_back(newJet);
     }
     //finish computation of the variation
     newMetJer = -jetDiff + met;
     return newMetJer;
  }

  // pfjet resolutions. taken from AN-2010-371
  double ErrEt( double Et, double Eta) {
    
   double InvPerr2;
  
   double N, S, C, m;
   if(fabs(Eta) < 0.5 ) {
     N = 3.96859;
     S = 0.18348;
     C = 0.;
     m = 0.62627;
   } else if( fabs(Eta) < 1. ) {
     N = 3.55226;
     S = 0.24026;
     C = 0.;
     m = 0.52571;
   } else if( fabs(Eta) < 1.5 ) {
     N = 4.54826;
     S = 0.22652;
     C = 0.;
     m = 0.58963;
   } else if( fabs(Eta) < 2. ) {
     N = 4.62622;
     S = 0.23664;
     C = 0.;
     m = 0.48738;
   } else if( fabs(Eta) < 3. ) {
     N = 2.53324;
     S = 0.34306;
     C = 0.;
     m = 0.28662;
   } else if( fabs(Eta) < 5. ) {
     N = 2.95397;
     S = 0.11619;
     C = 0.;
     m = 0.96086;
   }
   // this is the absolute resolution (squared), not sigma(pt)/pt
   // so have to multiply by pt^2, thats why m+1 instead of m-1
   InvPerr2 =  (N * fabs(N) ) + (S * S) * pow(Et, m+1.) + (C * C) * Et * Et ;
   return sqrt(InvPerr2)/Et;
  }
  
}
