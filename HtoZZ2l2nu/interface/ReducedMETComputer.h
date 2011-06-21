#ifndef ReducedMETComputer_H
#define ReducedMETComputer_H

/** \class ReducedMETComputer
 *  No description available.
 *
 *  $Date: 2011/06/06 12:50:19 $
 *  $Revision: 1.4 $
 *  \author G. Cerminara & D. Trocino & P. Silva
 */

#include <utility>
#include <vector>


//class LorentzVector;

#include "Math/LorentzVector.h"
#include "TVector2.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ReducedMETComputer {
public:
  
  enum RedMetType { D0, MINIMIZED };
  enum EventCategory { COLLIMATED, OPENANGLE };
  enum PreferedRecoil { CLUSTERED, UNCLUSTERED };
 
  /// Constructor
  ReducedMETComputer(double kRecoil_long = 1.0,
		     double kRecoil_perp = 1.0,
		     double kSigmaPt_long = 1.0,
		     double kSigmaPt_perp = 1.0,
		     double kPerp = 1.0);

  /// Destructor
  virtual ~ReducedMETComputer();

  //define the axis
  std::pair<TVector2,TVector2> defineThrust(const LorentzVector& lepton1, double sigmaPt1,
					    const LorentzVector& lepton2, double sigmaPt2);

  //getters for the axis
  TVector2 a_l,a_t; 
  inline TVector2 getAl() { return a_l; }
  inline TVector2 getAt() { return a_t; }

  //get a projected vector

  TVector2 project(const LorentzVector &part);

  /// implementation of the algorithm
  void compute(const LorentzVector& lepton1, double sigmaPt1,
	       const LorentzVector& lepton2, double sigmaPt2,
	       const LorentzVectorCollection & jets,
	       const LorentzVector& met,
	       bool debug=false);

  /// get the event category
  int event_categ;
  inline int getEventCategory() { return event_categ; }
  
  //get the 2D vector
  TVector2 redMETminRmetxy,redMETxy;
  inline TVector2 reducedMETcartesian(int redMetType=D0)
  {
    TVector2 xy;
    switch(redMetType)
      {
      case MINIMIZED:
	xy=redMETminRmetxy;
	break;
      case D0: default :
	xy=redMETxy;
	break;
      }
    return xy;
  } 
  
  //get the absolute value
  double redMETminRmet,redMET;
  inline double reducedMET(int redMetType=D0) 
  {  
    double rho;
    switch(redMetType)
      {
      case MINIMIZED:
	rho=redMETminRmet;
	break;
      case D0: default :
	rho=redMET;
	break;
      }
    return rho;
  }
  TVector2 reducedMETcartesian() const { return redMETxy;  }
  
  double reducedMET() const {  return redMET; }

  //get the components
  double reducedMET_long,reducedMET_perp;
  double reducedMETminRmet_long,reducedMETminRmet_perp; 
  inline std::pair<double, double> reducedMETComponents(int redMetType=D0)
    {
      std::pair<double, double> rmetComps;
      switch(redMetType)
	{
	case MINIMIZED:
	  rmetComps=std::make_pair(reducedMETminRmet_long, reducedMETminRmet_perp);
	  break;
	case D0: default:
	  rmetComps=std::make_pair(reducedMET_long, reducedMET_perp);
	  break;
	}
      return rmetComps;
    }
  
  //get the prefered recoil
  int prefRecminRmet;
  int prefRec_long, prefRec_perp;
  inline std::pair<int,int> getPreferedRecoil(int redMetType=D0)
  {
    std::pair<int,int> prefRec;
    switch(redMetType)
      {
      case MINIMIZED:
	prefRec=std::make_pair(prefRecminRmet,prefRecminRmet);
	break;
      case D0: default:
	prefRec=std::make_pair(prefRec_long,prefRec_perp);
	break;
      }
    return prefRec;
  }

  //event decomposition
  double recoilProj_long,recoilProj_perp;
  inline std::pair<double, double> recoilProjComponents() const 
    {
      return std::make_pair(recoilProj_long, recoilProj_perp);
    }
  
  double unclProj_long,unclProj_perp;
  inline std::pair<double, double> unclProjComponents() const 
    {
      return std::make_pair(unclProj_long, unclProj_perp);
    }

  double sumJetProj_long,sumJetProj_perp;  
  inline std::pair<double, double> sumJetProjComponents() const 
    {
      return std::make_pair(sumJetProj_long, sumJetProj_perp);
    }

  double dileptonProj_long,dileptonProj_perp;  
  inline std::pair<double, double> dileptonProjComponents() const 
    {
      return std::make_pair(dileptonProj_long, dileptonProj_perp);
    }

  double metProj_long, metProj_perp;  
  inline std::pair<double, double> metProjComponents() const 
    {
      return std::make_pair(metProj_long,metProj_perp);
    }

  double deltaDileptonProj_long, deltaDileptonProj_perp;
  inline std::pair<double, double> dileptonPtCorrComponents() const 
    {
      return std::make_pair(deltaDileptonProj_long,deltaDileptonProj_perp);
    }
  
 private:
  
  double kRecoil_long;
  double kRecoil_perp;
  double kSigmaPt_long;
  double kSigmaPt_perp;
  double kPerp;
};
#endif

