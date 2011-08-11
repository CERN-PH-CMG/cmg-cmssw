#ifndef ReducedMETComputer_H
#define ReducedMETComputer_H

/** \class ReducedMETComputer
 *  No description available.
 *
 *  $Date: 2011/07/01 08:36:56 $
 *  $Revision: 1.8 $
 *  \author G. Cerminara & D. Trocino & P. Silva
 */

#include <utility>
#include <vector>


//class LorentzVector;

#include "Math/LorentzVector.h"
#include "TVector2.h"

#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

class ReducedMETComputer {
public:
  
  enum RedMetType { D0, MINIMIZED, INDEPENDENTLYMINIMIZED };
  enum DileptonTopology { COLLIMATED, OPENANGLE };
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
	       bool isZcandidate=false,
	       bool debug=false);

  /// get the event category
  int event_categ;
  inline int getEventCategory() { return event_categ; }
  
  //get the 2D vector
  TVector2 redMETminRmetxy,redMETxy,redMETIndminRmetxy;
  inline TVector2 reducedMETcartesian(int redMetType=D0)
  {
    TVector2 xy;
    switch(redMetType)
      {
      case MINIMIZED:
	xy=redMETminRmetxy;
	break;
      case INDEPENDENTLYMINIMIZED:
	xy=redMETIndminRmetxy;
	break;
      case D0: default :
	xy=redMETxy;
	break;
      }
    return xy;
  } 
  
  //get the absolute value
  double redMETIndminRmet,redMETminRmet,redMET;
  inline double reducedMET(int redMetType=D0) 
  {  
    double rho;
    switch(redMetType)
      {
      case MINIMIZED:
	rho=redMETminRmet;
	break;
      case INDEPENDENTLYMINIMIZED:
	rho=redMETIndminRmet;
	break;
      case D0: default :
	rho=redMET;
	break;
      }
    return rho;
  }

  //get the components
  double reducedMET_long,reducedMET_perp;
  double reducedMETminRmet_long,reducedMETminRmet_perp; 
  double reducedMETIndminRmet_long,reducedMETIndminRmet_perp; 
  inline std::pair<double, double> reducedMETComponents(int redMetType=D0)
    {
      std::pair<double, double> rmetComps;
      switch(redMetType)
	{
	case MINIMIZED:
	  rmetComps=std::make_pair(reducedMETminRmet_long, reducedMETminRmet_perp);
	  break;
	case INDEPENDENTLYMINIMIZED:
	  rmetComps=std::make_pair(reducedMETIndminRmet_long,reducedMETIndminRmet_perp);
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
  int prefRecIndMin_long, prefRecIndMin_perp;
  inline std::pair<int,int> getPreferedRecoil(int redMetType=D0)
  {
    std::pair<int,int> prefRec;
    switch(redMetType)
      {
      case MINIMIZED:
	prefRec=std::make_pair(prefRecminRmet,prefRecminRmet);
	break;
      case INDEPENDENTLYMINIMIZED:
	prefRec=std::make_pair(prefRecIndMin_long,prefRecIndMin_perp);
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

  enum WorkingPoints { MEDIUMWP, TIGHTWP };
  double getCut(int eventCategory, int workingpoint=MEDIUMWP)
  {
    double cut(0);
    switch(eventCategory)
      {
      case EventCategory::EQ0JETS :
	if(workingpoint==MEDIUMWP) cut=30.04;
	if(workingpoint==TIGHTWP)  cut=39.4;
	break;
      case EventCategory::EQ1JETS :
	if(workingpoint==MEDIUMWP) cut=44.0;
	if(workingpoint==TIGHTWP)  cut=54.5;
	break;
      case EventCategory::GEQ2JETS :
	if(workingpoint==MEDIUMWP) cut=50.35;
	if(workingpoint==TIGHTWP)  cut=81.2;
	break;
      case EventCategory::VBF :
	if(workingpoint==MEDIUMWP) cut=57.36;
	if(workingpoint==TIGHTWP)  cut=60.2;
	break;
      }
    return cut;
  }
  
 private:
  
  double kRecoil_long;
  double kRecoil_perp;
  double kSigmaPt_long;
  double kSigmaPt_perp;
  double kPerp;
};
#endif

