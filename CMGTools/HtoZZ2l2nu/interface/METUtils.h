#ifndef METUtils_H
#define METUtils_H

/** \class METUtils
 *  No description available.
 *
 *  $Date: 2012/04/01 09:28:28 $
 *  $Revision: 1.5 $
 *  \author G. Cerminara & D. Trocino & P. Silva & L. Quertenmont & L. Pernie
 */

#include <utility>
#include <vector>

//class LorentzVector;

#include "Math/LorentzVector.h"
#include "TVector2.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;



namespace METUtils{

  struct stRedMET{
     TVector2 a_l;
     TVector2 a_t;
     TVector2 redMETxy;
     double redMET_l;
     double redMET_t;
  };
  
  enum RedMetType { D0, MINIMIZED, INDEPENDENTLYMINIMIZED };
  enum DileptonTopology { COLLIMATED, OPENANGLE };
  enum PreferedRecoil { CLUSTERED, UNCLUSTERED };
  enum WorkingPoints { MEDIUMWP, TIGHTWP };

  std::pair<TVector2,TVector2> defineThrust(const LorentzVector& lepton1, double sigmaPt1,  const LorentzVector& lepton2, double sigmaPt2, int& event_categ);

  LorentzVector redMET(RedMetType Type, const LorentzVector& lepton1, double sigmaPt1, const LorentzVector& lepton2, double sigmaPt2, const LorentzVectorCollection & jets, const LorentzVector& met, bool isZcandidate=false,  stRedMET* out=NULL, double kRecoil_long = 1.0,  double kRecoil_perp = 1.0,  double kSigmaPt_long = 1.0, double kSigmaPt_perp = 1.0, double kPerp = 1.0);
  LorentzVector redMET(RedMetType Type, const LorentzVector& lepton1, double sigmaPt1, const LorentzVector& lepton2, double sigmaPt2, const LorentzVector& met1,           const LorentzVector& met2, bool isZcandidate=false, stRedMET* out=NULL, double kRecoil_long = 1.0,  double kRecoil_perp = 1.0,  double kSigmaPt_long = 1.0, double kSigmaPt_perp = 1.0, double kPerp = 1.0);
  LorentzVector redMET(RedMetType Type, const LorentzVector& lepton1, double sigmaPt1, const LorentzVector& lepton2, double sigmaPt2, const LorentzVector& met1,           const LorentzVector& met2, const LorentzVector& met3,  bool isZcandidate=false, stRedMET* out=NULL, double kRecoil_long = 1.0,  double kRecoil_perp = 1.0,  double kSigmaPt_long = 1.0, double kSigmaPt_perp = 1.0, double kPerp = 1.0);

/*
  /// get the event category
  inline int getEventCategory() { return event_categ; }
  
  //get the 2D vector
  inline TVector2 reducedMETcartesian(int redMetType=D0){
    switch(redMetType){
      case MINIMIZED:
	return redMETminRmetxy;
	break;
      case INDEPENDENTLYMINIMIZED:
	return redMETIndminRmetxy;
	break;
      case D0: default :
	return redMETxy;
	break;
    }
  } 
  
  //get the absolute value
  inline double reducedMET(int redMetType=D0){  
    switch(redMetType){
      case MINIMIZED:
	return redMETminRmet;
	break;
      case INDEPENDENTLYMINIMIZED:
	return redMETIndminRmet;
	break;
      case D0: default :
	return redMET;
	break;
      }
  }

  //get the components
  inline std::pair<double, double> reducedMETComponents(int redMetType=D0){
      std::pair<double, double> rmetComps;
      switch(redMetType){
	case MINIMIZED:
	  return std::make_pair(reducedMETminRmet_long, reducedMETminRmet_perp);
	  break;
	case INDEPENDENTLYMINIMIZED:
	  return std::make_pair(reducedMETIndminRmet_long,reducedMETIndminRmet_perp);
	  break;
	case D0: default:
	  return std::make_pair(reducedMET_long, reducedMET_perp);
	  break;
	}
    }
  
  //get the prefered recoil
  inline std::pair<int,int> getPreferedRecoil(int redMetType=D0){
    switch(redMetType){
      case MINIMIZED:
	return std::make_pair(prefRecminRmet,prefRecminRmet);
	break;
      case INDEPENDENTLYMINIMIZED:
	return std::make_pair(prefRecIndMin_long,prefRecIndMin_perp);
	break;
      case D0: default:
	return std::make_pair(prefRec_long,prefRec_perp);
	break;
      }
  }

  //event decomposition
  inline std::pair<double, double> recoilProjComponents()     const{    return std::make_pair(recoilProj_long, recoilProj_perp);      }
  inline std::pair<double, double> unclProjComponents()       const{    return std::make_pair(unclProj_long, unclProj_perp);          }
  inline std::pair<double, double> sumJetProjComponents()     const{    return std::make_pair(sumJetProj_long, sumJetProj_perp);      }
  inline std::pair<double, double> dileptonProjComponents()   const{    return std::make_pair(dileptonProj_long, dileptonProj_perp);  }
  inline std::pair<double, double> metProjComponents()        const{    return std::make_pair(metProj_long,metProj_perp);             }
  inline std::pair<double, double> dileptonPtCorrComponents() const{   return std::make_pair(deltaDileptonProj_long,deltaDileptonProj_perp);    }
*/
  double getRedMETCut(int eventCategory, int workingpoint=MEDIUMWP);
  LorentzVector projectedMET(const LorentzVector& lepton1, const LorentzVector& lepton2, const LorentzVector& met, stRedMET* out=NULL);
  double transverseMass(LorentzVector &visible, LorentzVector &invisible, bool assumeSameMass=false);

  LorentzVector smearedJet(const LorentzVector &origJet, double genJetPt, int mode=0);

  enum UncertaintyVariations { JER, JER_UP, JER_DOWN, JES_UP, JES_DOWN};
  void computeVariation(LorentzVectorCollection& jets, std::vector<double> &genjetsPt, LorentzVector& met,
                        std::vector<LorentzVectorCollection>& jetsVar, LorentzVectorCollection& metsVar,
			JetCorrectionUncertainty *jecUnc);

  LorentzVector SmearJetFormGen(LorentzVectorCollection& jets, LorentzVector& met, std::vector<double> GenJet, std::vector<LorentzVector>& jetsJer);
  double ErrEt( double Et, double Eta);
}
#endif

