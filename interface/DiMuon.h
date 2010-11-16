#ifndef DIMUON_H_
#define DIMUON_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

#include <vector>

namespace cmg{

  //forward def needed
  class DiMuon;
  class DiMuonFactory;

  class DiMuon : public AbstractPhysicsObject{
  public:
    DiMuon(const cmg::Muon& leg1_, const cmg::Muon& leg2_):
      AbstractPhysicsObject( leg1_, leg2_ ),
      _leg1(leg1_),
      _leg2(leg2_){
    }
    DiMuon():
      AbstractPhysicsObject(),
	  _leg1(),
	   _leg2(){
    }
		
	cmg::Muon leg1() const{
	  return _leg1;
	}
	cmg::Muon leg2() const{
	  return _leg2;
	}
		
	
  private:
	//store by value
	cmg::Muon _leg1;
	cmg::Muon _leg2;
	
  };

}

#endif /*DIMUON_H_*/
