#ifndef DIOBJECTUPDATEFACTORY_H_
#define DIOBJECTUPDATEFACTORY_H_

#include "CMGTools/Common/interface/DiObjectFactory.h"

namespace cmg{

  // T is for example a di-object<U, S>
  template< typename T>
  class DiObjectUpdateFactory : public cmg::Factory< T > {

  public:
    
    DiObjectUpdateFactory(const edm::ParameterSet& ps):
      diObjectFactory_( ps ),
      diObjectLabel_(ps.getParameter<edm::InputTag>("diObjectCollection")),
      metLabel_(ps.getParameter<edm::InputTag>("metCollection")){}

    //need to override from Factory to insert "typename"
    typedef typename cmg::Factory< T >::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    
  private:

    const DiObjectFactory< typename T::type1, typename T::type2 > diObjectFactory_;
    const edm::InputTag diObjectLabel_;
    const edm::InputTag metLabel_;
  };
  
} // namespace cmg


template< typename T >
typename cmg::DiObjectUpdateFactory<T>::event_ptr cmg::DiObjectUpdateFactory<T>::create(const edm::Event& iEvent, const edm::EventSetup&){
    
  typedef std::vector< T > collection;
  typedef edm::View<reco::LeafCandidate> met_collection;
  
  edm::Handle<collection> diObjects;
  iEvent.getByLabel(diObjectLabel_,diObjects);
   
  edm::Handle<met_collection> metCands;
  iEvent.getByLabel(metLabel_,metCands);
  
/*   if(metCands->empty() ) { */
/*     throw cms::Exception("MET collection is empty."); */
/*   } */
  if(metCands->size() != diObjects->size() ) {
    throw cms::Exception("MET and Di-object collections should have the same size.");
  }

  typename cmg::DiObjectUpdateFactory<T>::event_ptr result(new collection);
  
  unsigned index = 0;
  for(typename collection::const_iterator it = diObjects->begin(); it != diObjects->end(); ++it, ++index ){
    const T& diObject = *it; 
    result->push_back(diObject);
    assert( index < metCands->size() );
    diObjectFactory_.set( std::make_pair(result->back().leg1(), result->back().leg2()),
			  metCands->at(index), & result->back() );
  }
  
  return result;
}


#endif /*DIOBJECTUPDATEFACTORY_H_*/
