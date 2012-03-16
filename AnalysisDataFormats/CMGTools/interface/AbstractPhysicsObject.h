#ifndef ABSTRACTPHYSICSOBJECT_H_
#define ABSTRACTPHYSICSOBJECT_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include <algorithm>
#include <memory>
#include <ostream>
#include <vector>

#include "TRegexp.h"

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
    //Version of getSelection that takes a RegExp
    bool getSelectionRegExp(const TRegexp&) const;
    bool getSelectionRegExp(const std::string& s) const{
        return getSelectionRegExp(TRegexp(s.c_str()));
    }
    bool getSelectionRegExp(const char* s) const{
        return getSelectionRegExp(TRegexp(s));
    }
    
    //needed for checking whether selections exist
    bool hasSelection(const std::string& s) const{
        const Strings names = getSelectionNames();
        return std::find(names.begin(),names.end(),s) != names.end();       
    }
    bool hasSelection(const char* s) const{
        return hasSelection(std::string(s));   
    }
    
    void addSelection(const std::string& name, const bool value){
      if(!hasSelection(name)){
        selections.push_back(name);
      }
      selections.set(name,value);   
    }
	
    typedef std::vector<AbstractPhysicsObject const *> Daughters;
    Daughters getDaughters(const bool include_this = false) const;
    	
    virtual Float_t uncOnFourVectorScale() const {return 0;}
    virtual Float_t rawFactor() const {return 1.;}
    
  protected:
    ///Used to copy selections
    pat::strbitset selections;


  }; 



}

//a generic print stream operator for PhysicsObjects
std::ostream& operator<<(std::ostream&, const cmg::AbstractPhysicsObject&);

#endif /*ABSTRACTPHYSICSOBJECT_H_*/
