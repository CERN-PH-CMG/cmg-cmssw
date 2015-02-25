#ifndef RecoMET_METPUSubtraction_PFMETAlgorithmMVA_h
#define RecoMET_METPUSubtraction_PFMETAlgorithmMVA_h

/** \class PFMETAlgorithmMVA
 *
 * MVA based algorithm for computing the particle-flow missing Et
 *
 * \authors Phil Harris, CERN
 *          Christian Veelken, LLR
 *
 */

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/EgammaObjects/interface/GBRForest.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoMET/METPUSubtraction/interface/mvaMEtUtilities.h"

//#include <TMatrixD.h>
#include <Math/SMatrix.h>

#include <string>
#include <vector>
#include <ostream>

class PFMETAlgorithmMVA 
{
  
 public:

  PFMETAlgorithmMVA(const edm::ParameterSet& cfg);
  ~PFMETAlgorithmMVA();

  void initialize(const edm::EventSetup&);

  void setHasPhotons(bool hasPhotons) { hasPhotons_ = hasPhotons; }

  void setInput(const std::vector<reco::PUSubMETCandInfo>&,
		const std::vector<reco::PUSubMETCandInfo>&,
		const std::vector<reco::PUSubMETCandInfo>&,
		const std::vector<reco::Vertex::Point>&);

  void evaluateMVA();

  reco::Candidate::LorentzVector getMEt()    const { return mvaMEt_;    }
  const reco::METCovMatrix&    getMEtCov() const { return mvaMEtCov_; }

  double getU()     const { return mvaOutputU_;    }
  double getDPhi()  const { return mvaOutputDPhi_;  }
  double getCovU1() const { return mvaOutputCovU1_; }
  double getCovU2() const { return mvaOutputCovU2_; }
  
  void print(std::ostream&) const;

 protected:
  const std::string updateVariableNames(std::string input);
  const GBRForest* loadMVAfromFile(const edm::FileInPath& inputFileName, const std::string& mvaName);
  const GBRForest* loadMVAfromDB(const edm::EventSetup& es, const std::string& mvaName);


  const Float_t evaluateU();
  const Float_t evaluateDPhi();
  const Float_t evaluateCovU1();
  const Float_t evaluateCovU2();

  mvaMEtUtilities utils_;
    
  std::string mvaNameU_;
  std::string mvaNameDPhi_;
  std::string mvaNameCovU1_;
  std::string mvaNameCovU2_;

  int    mvaType_;
  bool   hasPhotons_;
  double dZcut_;
  Float_t* createFloatVector(std::vector<std::string> variableNames);
  const Float_t GetResponse(const GBRForest *Reader, std::vector<std::string> &variableNames);
  void computeMET();
  std::map<std::string, Float_t> var_;


  Float_t* mvaInputU_;
  Float_t* mvaInputDPhi_;
  Float_t* mvaInputCovU1_;
  Float_t* mvaInputCovU2_;
  
  Float_t mvaOutputU_;
  Float_t mvaOutputDPhi_;
  Float_t mvaOutputCovU1_;
  Float_t mvaOutputCovU2_;

  std::vector<std::string> varForU_;
  std::vector<std::string> varForDPhi_;
  std::vector<std::string> varForCovU1_;
  std::vector<std::string> varForCovU2_;


  double sumLeptonPx_;
  double sumLeptonPy_;
  double chargedSumLeptonPx_;
  double chargedSumLeptonPy_;

  reco::Candidate::LorentzVector mvaMEt_;
  //TMatrixD mvaMEtCov_;
  reco::METCovMatrix mvaMEtCov_;

  const GBRForest* mvaReaderU_;
  const GBRForest* mvaReaderDPhi_;
  const GBRForest* mvaReaderCovU1_;
  const GBRForest* mvaReaderCovU2_;

  bool loadMVAfromDB_;

  edm::ParameterSet cfg_;
};
#endif
