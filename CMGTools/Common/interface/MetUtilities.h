#ifndef PH_METUTILITIES_H
#define PH_METUTILITIES_H

#include <utility>
#include <vector>
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"

class MetUtilities {
 public:
  typedef math::XYZVector         Vector;
  typedef math::XYZTLorentzVector LorentzVector;
  struct JetInfo {
    LorentzVector p4;
    double        mva;
    double        neutFrac;  
  };

  MetUtilities(unsigned workingPointId);
  virtual ~MetUtilities();

  bool              passMVA  (std::pair<LorentzVector,double> iJet);
  LorentzVector    *leadPt   (std::vector<JetInfo> &iJets,bool iFirst);
  int               NJets    (std::vector<JetInfo> &iJets,double iPt);
  double            deltaR   (LorentzVector &iVec1,LorentzVector &iVec2);
  void              cleanJets(std::vector<LorentzVector> &iVis,std::vector<JetInfo> &iJets);
  void              cleanMet (std::vector<LorentzVector> &iVis,std::vector<JetInfo> &iJets,std::pair<LorentzVector,double> &iMet,bool iMVA,bool iAdd);

  std::pair<LorentzVector,double> TKMet   (std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ,int iLowDz);
  std::pair<LorentzVector,double> JetMet  (std::vector<JetInfo> &iJets ,bool iPassMVA);
  std::pair<LorentzVector,double> NoPUMet (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<JetInfo> &iJets,double iDZ);
  std::pair<LorentzVector,double> PUMet   (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<JetInfo> &iJets,double iDZ);
  std::pair<LorentzVector,double> PUCMet  (std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<JetInfo> &iJets,double iDZ);

  std::pair<LorentzVector,double> PFRecoil  (double iSumEt,LorentzVector iVis,std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ);
  std::pair<LorentzVector,double> TKRecoil  (double iSumEt,LorentzVector iVis,std::vector<std::pair<LorentzVector,double> > &iCands,double iDZ);
  std::pair<LorentzVector,double> NoPURecoil(double iSumEt,LorentzVector iVis,
					     std::vector<std::pair<LorentzVector,double> > &iCands,std::vector<JetInfo> &iJets,double iDZ);
  std::pair<LorentzVector,double> PUCRecoil(double iSumEt,LorentzVector iVis,
					    std::vector<std::pair<LorentzVector,double> > &iCands ,std::vector<JetInfo> &iJets,double iDZ);

  std::pair<MetUtilities::LorentzVector,double> Recoil(double iSumEt,double iPtVis,double iPhiVis,std::pair<LorentzVector,double> &iMet); 
  //colin: a friend declaration
  double fJetPtMin;
  unsigned   workingPointId_;
 protected:
  // PU jet identifier 
  double     fMVACut[4][4][4];  //Jet Id MVA
};

#endif
