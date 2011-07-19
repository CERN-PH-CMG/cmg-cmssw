/*
 *  See header file for a description of this class.
 *
 *  $Date: 2011/07/04 22:53:51 $
 *  $Revision: 1.11 $
 *  \author G. Cerminara & D. Trocino
 */

#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TVector2.h"
#include "TMath.h"

using namespace std;

//
ReducedMETComputer::ReducedMETComputer(double kRecoil_long,
				       double kRecoil_perp,
				       double kSigmaPt_long,
				       double kSigmaPt_perp,
				       double kPerp) :
  kRecoil_long(kRecoil_long),
  kRecoil_perp(kRecoil_perp),
  kSigmaPt_long(kSigmaPt_long),
  kSigmaPt_perp(kSigmaPt_perp),
  kPerp(kPerp) {}


//
ReducedMETComputer::~ReducedMETComputer(){}

//
std::pair<TVector2, TVector2> ReducedMETComputer::defineThrust(const LorentzVector& l1, double sigmaPt1,
							       const LorentzVector& l2, double sigmaPt2)
{
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
  if(dphill>=TMath::Pi()/2)
    {
      event_categ=OPENANGLE;
      longi = thr.Unit();
      perp = longi.Rotate(TMath::Pi()/2);
      if(perp*reference<0) perp *= -1;
    }
  else
    {
      event_categ=COLLIMATED;
      perp=dil.Unit();
      longi = perp.Rotate(TMath::Pi()/2);
      if(longi*reference<0) longi *= -1;
    }
  
  //all done
  return std::pair<TVector2, TVector2>(longi,perp);
}

//
TVector2 ReducedMETComputer::project(const LorentzVector &part)
{
  TVector2 redPart(part.px(),part.py());
  return TVector2( a_l*redPart, a_t*redPart );
}


//
void ReducedMETComputer::compute(const LorentzVector& theLepton1, double sigmaPt1,
				 const LorentzVector& theLepton2, double sigmaPt2,
				 const LorentzVectorCollection& theJets,
				 const LorentzVector& theMET,
				 bool isZcandidate,
				 bool debug) 
{

  //the dilepton candidate
  TVector2 dil(theLepton1.px()+theLepton2.px(),theLepton1.py()+theLepton2.py());

  //define the thrust
  if(!isZcandidate)
    {
       std::pair<TVector2, TVector2> thrust=defineThrust(theLepton1,sigmaPt1,theLepton2,sigmaPt2);
       a_l=thrust.first;  
       a_t=thrust.second;

       //      std::pair<TVector2, TVector2> thrust=defineThrust(theLepton1,sigmaPt1,theLepton2,sigmaPt2);
       //       a_t=dil.Unit();
       //       a_l = a_t.Rotate(TMath::Pi()/2);
    }
  else 
    {
      dil = TVector2(theLepton1.px(),theLepton1.py());
      a_t = dil.Unit();
      a_l = a_t.Rotate(TMath::Pi()/2);
    }

  //project the dilepton
  dileptonProj_long = dil*a_l;
  dileptonProj_perp = dil*a_t;

  //project the jet sum
  sumJetProj_long = 0.;
  sumJetProj_perp = 0.;
  for(vector<LorentzVector>::const_iterator jetit = theJets.begin();
      jetit != theJets.end();
      ++jetit) {
    TVector2 jet((*jetit).Px(), (*jetit).Py());
    //if(jet*a_l < 0) 
    sumJetProj_long += jet*a_l;
    //if(jet*a_t < 0) 
    sumJetProj_perp += jet*a_t;
  }
  
  //project the met
  TVector2 pfMET(theMET.Px(),theMET.py());
  metProj_long = pfMET*a_l;
  metProj_perp = pfMET*a_t;
  TVector2 uncl=pfMET+dil;
  unclProj_long = uncl*a_l;
  unclProj_perp = uncl*a_t;

  //take the minimum recoil possible depending on the event category type
  recoilProj_long = min(sumJetProj_long, -1.*(unclProj_long));
  recoilProj_long=min(recoilProj_long,0.);
  recoilProj_perp = min(sumJetProj_perp, -1.*(unclProj_perp));
  recoilProj_perp=min(recoilProj_perp,0.);   

//   if(event_categ==DILMETPJETSLIKE) 
//     {
//       recoilProj_long=sumJetProj_long;
//       recoilProj_perp=sumJetProj_perp;
//     }      
  
  //propagate the lepton uncertainty (if larger than unity assume 100% rel. uncertainty)
  if(!isZcandidate)
    {
      double relErrPt1 = min(sigmaPt1/theLepton1.pt(), 1.);
      double relErrPt2 = min(sigmaPt2/theLepton2.pt(), 1.);
      LorentzVector loweredLepton1 = theLepton1*(1.0-relErrPt1);
      LorentzVector loweredLepton2 = theLepton2*(1.0-relErrPt2);
      std::pair<TVector2, TVector2> loweredThrust=defineThrust(loweredLepton1,sigmaPt1,loweredLepton2,sigmaPt2);
      float loweredDileptonProj_perp = dil*loweredThrust.second;
      deltaDileptonProj_perp = loweredDileptonProj_perp - dileptonProj_perp;
      deltaDileptonProj_long = ( -relErrPt1*TVector2(theLepton1.px(),theLepton1.py()) + relErrPt2*TVector2(theLepton2.px(),theLepton2.py()) )*a_l;
    }
  else
    {
      deltaDileptonProj_perp=0;
      deltaDileptonProj_long=0;
    }

  //
  // D0 ORIGINAL VERSION
  //
  //recoil is minimized independently
  recoilProj_long = min(sumJetProj_long, -1.*(unclProj_long));
  recoilProj_long=min(recoilProj_long,0.);
  prefRec_long= (sumJetProj_long<-1.*(unclProj_long) ? CLUSTERED : UNCLUSTERED);

  recoilProj_perp = min(sumJetProj_perp, -1.*(unclProj_perp));
  recoilProj_perp=min(recoilProj_perp,0.);   
  prefRec_perp= (sumJetProj_perp<-1.*(unclProj_perp) ? CLUSTERED : UNCLUSTERED);
  
  //compute the reduced met components
  reducedMET_long = max( (dileptonProj_long + kRecoil_long*recoilProj_long + kSigmaPt_long*deltaDileptonProj_long), 0.);
  reducedMET_perp = max( (dileptonProj_perp + kRecoil_perp*recoilProj_perp + kSigmaPt_perp*deltaDileptonProj_perp), 0.);
  redMET = sqrt( pow(reducedMET_long,2) + pow(reducedMET_perp,2) );
  redMETxy=reducedMET_long*a_l+reducedMET_perp*a_t;

  //
  // CMS MINIMIZED VERSION
  //
  double unclRedMet_long = dileptonProj_long - kRecoil_long*unclProj_long + kSigmaPt_long*deltaDileptonProj_long;
  double unclRedMet_perp = dileptonProj_perp - kRecoil_perp*unclProj_perp + kSigmaPt_perp*deltaDileptonProj_perp;
  double unclRedMet      = sqrt(pow(unclRedMet_long,2)+pow(unclRedMet_perp,2));
  
  double cluRedMet_long  = dileptonProj_long + kRecoil_long*sumJetProj_long + kSigmaPt_long*deltaDileptonProj_long;
  double cluRedMet_perp  = dileptonProj_perp + kRecoil_perp*sumJetProj_perp + kSigmaPt_perp*deltaDileptonProj_perp;
  double cluRedMet       = sqrt(pow(cluRedMet_long,2)+pow(cluRedMet_perp,2));   

  prefRecminRmet         = (unclRedMet < cluRedMet ? UNCLUSTERED : CLUSTERED );
  reducedMETminRmet_long = (unclRedMet < cluRedMet ? unclRedMet_long : cluRedMet_long); 
  reducedMETminRmet_perp = (unclRedMet < cluRedMet ? unclRedMet_perp : cluRedMet_perp); 
  redMETminRmet = sqrt(pow(reducedMETminRmet_long,2)+pow(reducedMETminRmet_perp,2));
  redMETminRmetxy=reducedMETminRmet_long*a_l+reducedMETminRmet_perp*a_t;

  //
  // CMS INDEPEDENT MINIMIZATION VERSION
  //
  prefRecIndMin_long        = (fabs(unclRedMet_long) < fabs(cluRedMet_long) ? UNCLUSTERED : CLUSTERED );
  reducedMETIndminRmet_long = (fabs(unclRedMet_long) < fabs(cluRedMet_long) ? unclRedMet_long : cluRedMet_long); 
  prefRecIndMin_perp        = (fabs(unclRedMet_perp) < fabs(cluRedMet_perp) ? UNCLUSTERED : CLUSTERED );
  reducedMETIndminRmet_perp = (fabs(unclRedMet_perp) < fabs(cluRedMet_perp) ? unclRedMet_perp : cluRedMet_perp); 
  redMETIndminRmet = sqrt(pow(reducedMETIndminRmet_long,2)+pow(reducedMETIndminRmet_perp,2));
  redMETIndminRmetxy=reducedMETIndminRmet_long*a_l+reducedMETIndminRmet_perp*a_t;



  
  //debug the event
  if(debug)
    {
//       cout  << "//reco event" << endl
// 	    << "TVector2 l1(" << theLepton1.px() << "," << theLepton1.py() << ");" << endl
// 	    << "TVector2 l2(" << theLepton2.px() << "," << theLepton2.py() << ");" << endl
// 	    << "TVector2 dil(" << theLepton1.px()+theLepton2.px() << "," << theLepton1.py()+theLepton2.py() << ");" << endl
// 	    << "std::vector<TVector2> jets;" << endl;
//       for(vector<LorentzVector>::const_iterator jetit = theJets.begin();
// 	  jetit != theJets.end();
// 	  ++jetit)
// 	cout << "jets.push_back(TVector2(" << (*jetit).Px() << "," << (*jetit).Py() << ") );" << endl;
//       cout << "TVector2 met(" << theMET.Px() << "," << theMET.Py() << ");" << endl
// 	   << "TVector2 unclSum(" << -uncl.Px() << "," << -uncl.Py() << ");" << endl;
//       cout << "//reduced event" << endl
// 	   << "TVector2 a_l(" << a_l.Px() << "," << a_l.Py() << ");" << endl
// 	   << "TVector2 a_t(" << a_t.Px() << "," << a_t.Py() << ");" << endl
// 	   << "TVector2 sumjet_long("<< sumJetProj_long*a_l.Px() << "," << sumJetProj_long*a_l.Py() << ");" << endl
// 	   << "TVector2 sumjet_perp("<< sumJetProj_perp*a_t.Px() << "," << sumJetProj_perp*a_t.Py() << ");" << endl
// 	   << "TVector2 unclProj_long("<< -unclProj_long*a_l.Px() << "," << -unclProj_long*a_l.Py() << ");" << endl
// 	   << "TVector2 unclProj_perp("<< -unclProj_perp*a_t.Px() << "," << -unclProj_perp*a_t.Py() << ");" << endl;
           
//       cout << "//balanced event" << endl
// 	   << "TVector2 dileptonProj_long("<< dileptonProj_long*a_l.Px() << "," << dileptonProj_long*a_l.Py() << ");" << endl
// 	   << "TVector2 dileptonProj_perp("<< dileptonProj_perp*a_t.Px() << "," << dileptonProj_perp*a_t.Py() << ");" << endl
// 	   << "TVector2 recoil_long("<< recoilProj_long*a_l.Px() << "," << recoilProj_long*a_l.Py() << ");" << endl
// 	   << "TVector2 recoil_perp("<< recoilProj_perp*a_t.Px() << "," << recoilProj_perp*a_t.Py() << ");" << endl
// 	   << "TVector2 deltaDileptonProj_long("<< deltaDileptonProj_long*a_l.Px() << "," << deltaDileptonProj_long*a_l.Py() << ");" << endl
// 	   << "TVector2 deltaDileptonProj_perp("<< deltaDileptonProj_perp*a_t.Px() << "," << deltaDileptonProj_perp*a_t.Py() << ");" << endl;
//       cout << "//red met" << endl
// 	   << "TVector2 redmet(" << redMETxy.Px() << "," << redMETxy.Py() << ");" << endl << endl;
    }
}
  


