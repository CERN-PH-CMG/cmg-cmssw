#ifndef __CMGTools_Common_interface_SpecificIsolation__
#define __CMGTools_Common_interface_SpecificIsolation__


#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>


/**
   \class Specific isolation
   \brief Holds parameters (vetoes, cone size) for specific isolation
*/

class SpecificIsolation {
  
 public:
  
  // default construcor
  SpecificIsolation() {}
  
  // destructor: free the AbsVetoes
  virtual ~SpecificIsolation(){
    for(unsigned i = 0; i< AbsVetos_.size(); i++){
      delete AbsVetos_[i]; 
    }
  }

  // init from a pset
  void initialize( const edm::ParameterSet& pset )
  {
    coneSize_ = pset.getParameter<double>("coneSize");

    // lower pT threshold is defined as a veto. 
    setVetoesParam( pset );
    createAbsVetoes();
  }
  
  const double coneSize() const{
    return coneSize_;
  }

  const std::map< std::string , std::vector<double> >& vetoes(){
    return vetoesParam_;
  }

  // set parameters for vetoes from a pset: implemented in the .cc
  void setVetoesParam( const edm::ParameterSet& pset );

  // create the AbsVetos: implemented in the .cc
  void createAbsVetoes();

  // return the AbsVetoes
  reco::isodeposit::AbsVetos getAbsVetoes() const{
    return AbsVetos_;
  }

  // print out of the parameters
  friend std::ostream& operator<<(std::ostream& out, const SpecificIsolation& isoDepParam);

 private:
  
  /// cone size
  double coneSize_;

  /// vetoes with names and cuts
  std::map< std::string, std::vector<double> >  vetoesParam_;

  /// AbsVetoes
  reco::isodeposit::AbsVetos AbsVetos_;

};


#endif 
