/*
 *  See header file for a description of this class.
 *
 *  $Date: 2011/05/20 07:52:07 $
 *  $Revision: 1.4 $
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
std::pair<TVector2, TVector2> ReducedMETComputer::defineThrust(const LorentzVector& l1, const LorentzVector& l2)
{
  //define the leading and sub-leading lepton
  const LorentzVector &lepton1( l1.pt() > l2.pt() ? l1 : l2);
  const LorentzVector &lepton2( l1.pt() > l2.pt() ? l2 : l1);
  double dphill=fabs(deltaPhi(lepton1.phi(),lepton2.phi()));
  TVector2 lead(lepton1.px(),lepton1.py());
  TVector2 trailer(lepton2.px(),lepton2.py());
  
  //define the thrust and dilepton
  TVector2 dil = lead+trailer;
  TVector2 thr = lead-trailer;
  
  //define the longitudinal
  TVector2 longi;
  if(dphill>=TMath::Pi()/2)  longi = thr.Unit();
  else                       longi = TVector2(0,0);

  //define the perpedicular axis
  TVector2 perp;
  if(dphill<TMath::Pi()/2) perp=dil.Unit();
  else{
    perp = longi.Rotate(TMath::Pi()/2);
    if(perp*lead<0) perp *= -1;
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
				 bool debug) 
{

  //define the thrust
  std::pair<TVector2, TVector2> thrust=defineThrust(theLepton1,theLepton2);
  a_l=thrust.first;  a_t=thrust.second;
  TVector2 dil(theLepton1.px()+theLepton2.px(),theLepton1.py()+theLepton2.py());
  dileptonProj_long = dil*a_l;
  dileptonProj_perp = dil*a_t;

  //project the jet sum
  sumJetProj_long = 0.;
  sumJetProj_perp = 0.;
  for(vector<LorentzVector>::const_iterator jetit = theJets.begin();
      jetit != theJets.end();
      ++jetit) {
    TVector2 jet((*jetit).Px(), (*jetit).Py());
    if(jet*a_l < 0) sumJetProj_long += jet*a_l;
    if(jet*a_t < 0) sumJetProj_perp += jet*a_t;
  }

  //project the met
  TVector2 met(theMET.Px(), theMET.Py());
  met += dil;
  metProj_long = met*a_l;
  metProj_perp = met*a_t;

  //take the minimum recoil possible
  recoilProj_long = min(sumJetProj_long, -1.*(metProj_long));
  recoilProj_long = min(recoilProj_long, 0.);
  recoilProj_perp = min(sumJetProj_perp, -1.*(metProj_perp));
  recoilProj_perp = min(recoilProj_perp, 0.);
  if(sumJetProj_long) recoilProjUnbound_long = min(sumJetProj_long, -1.*(metProj_long));
  else                recoilProjUnbound_long=-1.*(metProj_long);
  if(sumJetProj_perp) recoilProjUnbound_perp = min(sumJetProj_perp, -1.*(metProj_perp));
  else                recoilProjUnbound_perp=-1.*(metProj_perp);

  //propagate the lepton uncertainty (if larger than unity assume 100% rel. uncertainty)
  double relErrPt1 = min(sigmaPt1/theLepton1.pt(), 1.);
  double relErrPt2 = min(sigmaPt2/theLepton2.pt(), 1.);
  LorentzVector loweredLepton1 = theLepton1*(1.0-relErrPt1);
  LorentzVector loweredLepton2 = theLepton2*(1.0-relErrPt2);
  std::pair<TVector2, TVector2> loweredThrust=defineThrust(loweredLepton1,loweredLepton2);
  float loweredDileptonProj_perp = dil*loweredThrust.second;
  float deltaDileptonProj_perp = loweredDileptonProj_perp - dileptonProj_perp;
  float deltaDileptonProj_long = ( -relErrPt1*TVector2(theLepton1.px(),theLepton1.py()) + relErrPt2*TVector2(theLepton2.px(),theLepton2.py()) )*a_l;

  //compute the reduced met components
  reducedMET_long = max((dileptonProj_long + kRecoil_long*recoilProj_long + kSigmaPt_long*deltaDileptonProj_long),0.);
  reducedMET_perp = max((dileptonProj_perp + kRecoil_perp*recoilProj_perp + kSigmaPt_perp*deltaDileptonProj_perp),0.);
  reducedMETUnbound_long = dileptonProj_long + kRecoil_long*recoilProjUnbound_long + kSigmaPt_long*deltaDileptonProj_long;
  reducedMETUnbound_perp = dileptonProj_perp + kRecoil_perp*recoilProjUnbound_perp + kSigmaPt_perp*deltaDileptonProj_perp;


  //the modulo and the cartesian components of redMET
  redMET = sqrt(reducedMET_long*reducedMET_long + kPerp*reducedMET_perp*reducedMET_perp);
  redMETxy=reducedMET_long*a_l+reducedMET_perp*a_t;

  //debug the event
  if(debug)
    {
      cout  << "//reco event" << endl
	    << "TVector2 l1(" << theLepton1.px() << "," << theLepton1.py() << ");" << endl
	    << "TVector2 l2(" << theLepton2.px() << "," << theLepton2.py() << ");" << endl
	    << "TVector2 dil(" << theLepton1.px()+theLepton2.px() << ";" << theLepton1.py()+theLepton2.py() << ");" << endl
	    << "std::vector<TVector2> jets;" << endl;
      for(vector<LorentzVector>::const_iterator jetit = theJets.begin();
	  jetit != theJets.end();
	  ++jetit)
	cout << "jets.push_back(TVector2(" << (*jetit).Px() << "," << (*jetit).Py() << ");" << endl;
      cout << "TVector2 met(" << theMET.Px() << "," << theMET.Py() << ");" << endl
	   << "TVector2 calomet(" << met.Px() << "," << met.Py() << ");" << endl;
      
      cout << "//reduced event" << endl
	   << "TVector2 a_l(" << a_l.Px() << "," << a_l.Py() << ");" << endl
	   << "TVector2 a_t(" << a_t.Px() << "," << a_t.Py() << ");" << endl
	   << "TVector2 sumjet_long("<< sumJetProj_long*a_l.Px() << "," << sumJetProj_long*a_l.Py() << ");" << endl
	   << "TVector2 sumjet_perp("<< sumJetProj_perp*a_t.Px() << "," << sumJetProj_perp*a_t.Py() << ");" << endl
	   << "TVector2 metProj_long("<< -metProj_long*a_l.Px() << "," << -metProj_long*a_l.Py() << ");" << endl
	   << "TVector2 metProj_perp("<< -metProj_perp*a_t.Px() << "," << -metProj_perp*a_t.Py() << ");" << endl;
      
      cout << "//balanced event" << endl
	   << "TVector2 dileptonProj_long("<< dileptonProj_long*a_l.Px() << "," << dileptonProj_long*a_l.Py() << ");" << endl
	   << "TVector2 dileptonProj_perp("<< dileptonProj_perp*a_t.Px() << "," << dileptonProj_perp*a_t.Py() << ");" << endl
	   << "TVector2 recoil_long("<< recoilProj_long*a_l.Px() << "," << recoilProj_long*a_l.Py() << ");" << endl
	   << "TVector2 recoil_perp("<< recoilProj_perp*a_t.Px() << "," << recoilProj_perp*a_t.Py() << ");" << endl
	   << "TVector2 deltaDileptonProj_long("<< deltaDileptonProj_long*a_l.Px() << "," << deltaDileptonProj_long*a_l.Py() << ");" << endl
	   << "TVector2 deltaDileptonProj_perp("<< deltaDileptonProj_perp*a_t.Px() << "," << deltaDileptonProj_perp*a_t.Py() << ");" << endl;
      
      cout << "//red met" << endl
	   << "TVector2 redmet(" << redMETxy.Px() << "," << redMETxy.Py() << ");" << endl << endl;
    }
}
  


