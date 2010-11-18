#ifndef DIOBJECT_H_
#define DIOBJECT_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include <vector>

namespace cmg {

template <typename T, typename U> class DiObject;
template <typename T, typename U> class DiObjectFactory;

template< typename T, typename U > 
class DiObject : public AbstractPhysicsObject{
  public:
    
    typedef T type1;
    typedef U type2;
    
    DiObject():
        AbstractPhysicsObject(){
    }
    DiObject(const T& leg1, const U& leg2):
        AbstractPhysicsObject(leg1,leg2),
        leg1_(leg1),
        leg2_(leg2){
    }
    DiObject(const DiObject<T,U>& other):
        AbstractPhysicsObject(other),
        leg1_(other.leg1()),
        leg2_(other.leg2()){
    }
    
    virtual ~DiObject(){}
    
    T leg1() const{
        return leg1_;
    }
    U leg2() const{
        return leg2_;
    }
    
    virtual void accept(AbstractPhysicsObjectVisitor* v) const{
      v->visit(&leg1_);
      v->visit(&leg2_);
    }
    
  private:
    
    T leg1_;
    U leg2_;
    
    friend class cmg::DiObjectFactory<T,U>;
        
};

}



#endif /*DIOBJECT_H_*/
