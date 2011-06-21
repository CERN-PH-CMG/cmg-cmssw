#ifndef ProjectedMETComputer_H
#define ProjectedMETComputer_H

/** \class ProjectedMETComputer
 *  No description available.
 *
 *  $Date: 2011/06/06 12:50:19 $
 *  $Revision: 1.4 $
 *  \author G. Cerminara & D. Trocino & P. Silva
 */


#include "Math/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "TMath.h"
#include "TVector2.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ProjectedMETComputer {
public:
  enum EventCategory { COLLIMATED, OPENANGLE };
  
  /// Constructor
  ProjectedMETComputer() : projEtMiss(0), event_categ(OPENANGLE) 
    {
    }
    
    /// implements the algorithm
    inline double compute(const LorentzVector& lepton1, const LorentzVector& lepton2, const LorentzVector& met)
      {
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
	return projEtMiss;
      }

    /// getters
    inline double projectedMET() const { return projEtMiss; }
    inline int getEventCategory() const { return event_categ; }
    inline double minimumDeltaPhi() const { return minDeltaPhi; }
    inline TVector2 projectedMETcartesian()  
      {	
	TVector2 projMetxy=a_l*projectedMET_long+a_t*projectedMET_perp;
	return projMetxy;
      }
    std::pair<double,double> projectedMETComponents() { return std::make_pair(projectedMET_long, projectedMET_perp); }
    TVector2 getAl() { return a_l; }
    TVector2 getAt() { return a_t; }

    ~ProjectedMETComputer() {}
    
 private:

    TVector2 a_l,a_t;
    double projectedMET_long, projectedMET_perp;
    double projEtMiss;
    int event_categ;
    double minDeltaPhi;
};

#endif
