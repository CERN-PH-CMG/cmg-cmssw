#ifndef __CMGTools_Common_GenericPhysicsObjectSelectorDefinition__
#define __CMGTools_Common_GenericPhysicsObjectSelectorDefinition__

/**
   \class    pf2pat::GenericPhysicsObjectSelectorDefinition 
   \brief    Selects PhysicsObjects basing on cuts provided with string cut parser

   \author   Giovanni Petrucciani
   \version  $Id: GenericPhysicsObjectSelectorDefinition.h,v 1.5 2013/06/12 16:23:09 steggema Exp $
*/

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "boost/iterator/transform_iterator.hpp"
#include <functional>

#include <vector>

namespace cmg {
  
  template< class T >
  struct GenericPhysicsObjectSelectorDefinition {

    typedef std::vector< T > collection;
    typedef edm::Handle< collection > HandleToCollection;
    typedef collection  container;
    
    struct Pointer : public std::unary_function<T,const T *> { 
      const T * operator()(const T &c) const { return &c; } 
    };

    typedef boost::transform_iterator<Pointer,typename container::const_iterator> const_iterator;

    const_iterator begin() const { return const_iterator(selected_.begin()); }
    
    const_iterator end() const { return const_iterator(selected_.end()); }
    
    size_t size() const { return selected_.size(); }

    const container& selected() const {return selected_;}

    GenericPhysicsObjectSelectorDefinition ( const edm::ParameterSet & cfg ) :
      selector_( cfg.getParameter< std::string >( "cut" ) ) { }
    
    void select( const HandleToCollection & hc, 
		 const edm::Event & e,
		 const edm::EventSetup& s) {
      selected_.clear();
      
      unsigned key=0;
      for( typename collection::const_iterator pfc = hc->begin(); 
	   pfc != hc->end(); ++pfc, ++key) {
	
	if( selector_(*pfc) ) {
	  selected_.push_back( T(*pfc) );
	  reco::CandidatePtr ptrToMother( hc, key );
	  selected_.back().setSourceCandidatePtr( ptrToMother );
	}
      }
    }
    
    private:
    StringCutObjectSelector<T> selector_;
  protected:
    container selected_;

  };
}

#endif
