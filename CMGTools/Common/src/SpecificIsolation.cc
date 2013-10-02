#include "CMGTools/Common/interface/SpecificIsolation.h"


void SpecificIsolation::setVetoesParam( const edm::ParameterSet& pset ){
  
    std::vector<edm::ParameterSet> theVetoes 
      = pset.getParameter< std::vector< edm::ParameterSet > >("vetoes");
 
    for( unsigned i=0; i < theVetoes.size(); ++i) {

      const edm::ParameterSet& ps = theVetoes[i];
      std::vector<double> param;

      std::string type = ps.getParameter<std::string>("type");

      // ThresholdVeto: lower bound on the pT of deposits
      if( type == "ThresholdVeto" ){
	double threshold = ps.getParameter<double>("threshold");
	param.push_back( threshold );
	vetoesParam_.insert( make_pair(type, param ) );
      }
      // RectangularEtaPhiVeto: veto a rectangle in the eta-phi plane around the lepton
      if( type == "RectangularEtaPhiVeto" ){
	double dEta = ps.getParameter<double>("dEta");
	double dPhi = ps.getParameter<double>("dPhi");
	param.push_back( dEta );
	param.push_back( dPhi );
	vetoesParam_.insert( make_pair(type, param ) );
      }
      // ConeVeto: veto a cone in the eta-phi plane around the lepton
      if( type == "ConeVeto" ){
	double deltaR =  ps.getParameter<double>("deltaR");
	param.push_back( deltaR );
	vetoesParam_.insert( make_pair(type, param ) );
      }
      // add more vetoes here 
    }
  }


void SpecificIsolation::createAbsVetoes(){

  typedef std::map< std::string, std::vector<double> >::const_iterator IT;

  for(IT it = vetoesParam_.begin(); it != vetoesParam_.end() ; ++it ){
    if(it->first == "ThresholdVeto"){
      AbsVetos_.push_back( new reco::isodeposit::ThresholdVeto( (it->second)[0] ) );
    }
    if(it->first == "RectangularEtaPhiVeto"){
      AbsVetos_.push_back( new reco::isodeposit::RectangularEtaPhiVeto(reco::isodeposit::Direction() , - (it->second)[0], (it->second)[0] , -(it->second)[1],(it->second)[1] ));
    }
    if(it->first == "ConeVeto"){
      AbsVetos_.push_back( new reco::isodeposit::ConeVeto( reco::isodeposit::Direction(), (it->second)[0] ) );
    }
  }

}



std::ostream& operator<<(std::ostream& out, const SpecificIsolation& isoDepParam){

  typedef std::map< std::string, std::vector<double> >::const_iterator IT;

    if(!out) return out;
    out<<"isolation cone= "<<isoDepParam.coneSize_<<". Vetoes: ";
    for(IT it = isoDepParam.vetoesParam_.begin(); it != isoDepParam.vetoesParam_.end() ; ++it ){
      if(it->first == "ThresholdVeto") out<<it->first<<" with EtMin= "<<(it->second)[0];
      if(it->first == "ConeVeto") out<<it->first<<" with deltaR= "<<(it->second)[0];
      if(it->first == "RectangularEtaPhiVeto") out<<it->first<<" with dEta="<<(it->second)[0]<<", dPhi= "<<(it->second)[1];
      out<<", ";
    }
    return out;
  }
