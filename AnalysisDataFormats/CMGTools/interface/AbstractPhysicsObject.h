#ifndef ABSTRACTPHYSICSOBJECT_H_
#define ABSTRACTPHYSICSOBJECT_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include <memory>
#include <ostream>
#include <vector>

namespace cmg{

  /// The baseclass for all of the PhysicsObject data classes.
  class AbstractPhysicsObject : public reco::LeafCandidate {

  public:
    explicit AbstractPhysicsObject(const reco::Candidate& cand):
      reco::LeafCandidate(cand){
    }
    //need to define a copy constructor because of first constructor
    AbstractPhysicsObject(const AbstractPhysicsObject& other):
      reco::LeafCandidate(other),
      selections(other.selections){
    }      
    AbstractPhysicsObject(){
    }
    virtual ~AbstractPhysicsObject(){}
      
    //interface for itteration over daughters
    struct AbstractPhysicsObjectVisitor{
      virtual void visit(AbstractPhysicsObject const *) = 0;
    };
    //this should be updated for compound objects
    virtual void accept(AbstractPhysicsObjectVisitor* v) const{
      v->visit(this);
    }
    
    //define the default printing behaviour
    virtual std::ostream& printOn(std::ostream&) const;
    
    //default for sorting and comparison
    virtual bool operator<(const AbstractPhysicsObject& other) const;
    virtual bool operator<=(const AbstractPhysicsObject& other) const{
        return ( operator<(other) || operator==(other) );
    }
    virtual bool operator>(const AbstractPhysicsObject& other) const{
        return !( operator<(other) || operator==(other) );
    }
    virtual bool operator>=(const AbstractPhysicsObject& other) const{
        return ( operator>(other) || operator==(other) );
    }
    virtual bool operator==(const AbstractPhysicsObject& other) const;
    virtual bool operator!=(const AbstractPhysicsObject& other) const {
      return !operator==(other);
    }
    
    void addSelection(const std::string& name, const bool value){
      selections.push_back(name);
      selections.set(name,value);	
    }
    std::ostream& printSelections(std::ostream&, const std::string indent = "") const;
    bool getSelection(const std::string& s) const{
      //TODO: Understand how to make this a TriBool - i.e. unset? 
      return selections[s];
    }
    bool getSelection(const char* s) const{
      return getSelection(std::string(s));
    }
    typedef std::vector<std::string> Strings;
    Strings getSelectionNames() const{
      return selections.strings();           
    }
	
    typedef std::vector<AbstractPhysicsObject const *> Daughters;
    Daughters getDaughters(const bool include_this = false) const;
    	
  protected:
    ///Used to copy selections
    pat::strbitset selections;


  }; 



}

//a generic print stream operator for PhysicsObjects
std::ostream& operator<<(std::ostream&, const cmg::AbstractPhysicsObject&);

#endif /*ABSTRACTPHYSICSOBJECT_H_*/
