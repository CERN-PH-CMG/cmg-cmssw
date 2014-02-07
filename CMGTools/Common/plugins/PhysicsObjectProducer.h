#ifndef _CMGTools_Common_PhysicsObjectProducer_H_
#define _CMGTools_Common_PhysicsObjectProducer_H_

#include "DataFormats/Common/interface/OrphanHandle.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CMGTools/Common/interface/Cut.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

template<class FactoryClass>
class PhysicsObjectProducer : public edm::EDFilter{
 public:
  typedef Cut< typename FactoryClass::selector > CutType;
  typedef CutTree< CutType > CutTreeType;
  typedef CutTreeTester< CutTreeType, typename FactoryClass::object_type> CutTreeTesterType;

  PhysicsObjectProducer(const edm::ParameterSet& ps):
    ps_(ps),
    factory_(ps.getParameter<edm::ParameterSet>("cfg")), 
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)),
    cutTree_("All Cuts") {
    
    // std::cout<<"Initializing PhysicsObjectProducer for "<<ps<<std::endl;
    factory_.produces(this);//call hook for "produces"

    edm::ParameterSet cutsPSet = ps_.getParameter< edm::ParameterSet >("cuts");
    unpack( cutsPSet, "cuts", cutTree_); 

    // CutTreePrinter<CutTreeType> printer;
    // cutTree_.visit( &printer ); 

  }
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
  //hooks that factories can use
  virtual bool beginRun(edm::Run& iRun, edm::EventSetup const& iSetup){
    return factory_.beginRun(iRun,iSetup);
  }
  virtual bool endRun(edm::Run& iRun, edm::EventSetup const& iSetup){
    return factory_.endRun(iRun,iSetup);
  }
  virtual bool beginLuminosityBlock(edm::LuminosityBlock& iLumi, edm::EventSetup const& iSetup){
    return factory_.beginLuminosityBlock(iLumi,iSetup);
  }
  virtual bool endLuminosityBlock(edm::LuminosityBlock& iLumi, edm::EventSetup const& iSetup){
    return factory_.endLuminosityBlock(iLumi,iSetup);
  }

 private:
  
  /// vector of cuts. in the pair:
  /// first is the name of the parameter in the python module
  /// second is the corresponding cut string
/*   typedef std::vector< std::pair< std::string, std::string > > Cuts; */
/*   typedef std::vector< std::pair< std::string, std::string > >::const_iterator CI; */

  /// unpacks a parameterset to find all cuts, and fill them in cuts
  void unpack( const edm::ParameterSet& ps, 
	       const std::string& baseName, 
	       CutTreeType& cut) const; 

  /// prints all cuts 
  void printCuts( std::ostream& out = std::cout ) const; 

  const edm::ParameterSet ps_;
  FactoryClass factory_;
  bool verbose_;
  
  CutTreeType cutTree_;
};


template<class FactoryClass>
bool PhysicsObjectProducer<FactoryClass>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){


  typename FactoryClass::event_ptr outputObjects = factory_.create(iEvent,iSetup);
  if(outputObjects.get()){

    // the following was sorting by increasing pT! deactivating it. 
    // we need to preserve the official ordering, and to be sure 
    // where to find the leading, sub-leading, etc objects 
/*     if(outputObjects->size() > 1){ */
/*       //we sort according to the comparitors in AbstractPhysicsObject */
/*       std::sort(outputObjects->begin(),outputObjects->end()); */
/*     } */
		
    for(unsigned int i = 0; i < outputObjects->size(); i++) {
      CutTreeTesterType tester( & (outputObjects->at(i)) ); 
      cutTree_.visit( &tester );
    }

    if(verbose_) {
      for(typename FactoryClass::collection::iterator c = outputObjects->begin();c != outputObjects->end(); ++c){
	std::cout << "Produced object: " << *c << std::endl;
	std::cout << "Selections applied: " << std::endl;
	
	c->printSelections(std::cout);
      }
    }
    iEvent.put(outputObjects);
  }
  return true;
}


template<class FactoryClass>
void PhysicsObjectProducer<FactoryClass>::unpack( const edm::ParameterSet& ps, 
						  const std::string& baseName, 
						  PhysicsObjectProducer<FactoryClass>::CutTreeType& cutTree ) const {
  
/*   std::cout<<"unpacking"<<std::endl; */
  
/*   std::cout<<ps<<std::endl; */

  // all strings found are added as cuts. 
  std::vector<std::string> cutStrings = ps.getParameterNamesForType<std::string>();
    
  
  for( unsigned i=0; i<cutStrings.size(); ++i) {
/*     std::cout<<"cut "<<cutName<<std::endl; */
    std::string cutName = baseName; 
    cutName += "_";
    cutName += cutStrings[i];
    std::string cutString = ps.getParameter< std::string >( cutStrings[i] );
    
    cutTree.addCut( CutType( cutName, cutString ) );
  }

  // now dealing with parameter sets included in the current parameter set
  std::vector<std::string> subPSets = ps.getParameterNamesForType<edm::ParameterSet>();
  for( unsigned i=0; i<subPSets.size(); ++i) {
    edm::ParameterSet subPSet = ps.getParameter< edm::ParameterSet >( subPSets[i] );
    std::string newBaseName = baseName;
    newBaseName += "_";
    newBaseName += subPSets[i];
    
/*     cutTree.addNode( CutTreeType(subPSets[i]) );    */
    cutTree.addNode( CutTreeType( newBaseName )) ;

    unpack( subPSet, newBaseName, cutTree.lastNode() );
  }
}

template<class FactoryClass>
void PhysicsObjectProducer<FactoryClass>::printCuts( std::ostream& out ) const {
  
  out<<"Cuts :             "<<std::endl;
  out<<"-------------------"<<std::endl;
/*   for( unsigned i=0; i<cutTree_.size(); ++i) { */
/*     out<<"\t"<<cutTree_[i].name()<<"\t"<<cutTree_[i].<<std::endl; */
/*   } */
}


#endif /*PHYSICSOBJECTPRODUCER_H_*/
