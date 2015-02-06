#ifndef _CMGTools_Common_Cut_H_
#define _CMGTools_Common_Cut_H_

#include<vector>
#include<string>

template< class Selector >
class Cut {
 public:
  
  typedef Selector SelectorType;

  Cut() {}
  Cut( const std::string& name, const std::string& cutString = ""): 
    name_(name), cutString_(cutString), selector_(cutString) {}

    
  const std::string& name() const {return name_;}
  const std::string& cutString() const {return cutString_;}
  const Selector&    selector() const {return selector_;}

  friend std::ostream& operator<<(std::ostream& out, const Cut& cut) {
    if(!out) return out; 
    out<<"cut: "<<cut.name_<<" : "<<cut.cutString_;
    return out;
  }

 private:
  std::string       name_;
  std::string       cutString_;
  Selector          selector_;
};


template<class CutType >
class CutTree {
 public:
  /// a Node is a sub-cuttree. Each CutTree can contain several CutTrees.
  typedef typename std::vector< CutTree<CutType> > Nodes;
  typedef typename Nodes::const_iterator NI;

  /// Type for the set of leaves, each of them being a Cut. 
  typedef typename std::vector< CutType > Cuts;
  typedef typename Cuts::const_iterator CI;
  
  CutTree() {}
  CutTree( const std::string& name ) : name_( name ) {}

  void addNode( const CutTree& cutTree ) {nodes_.push_back(cutTree); }
  void addCut( const CutType& cut ) {cuts_.push_back(cut); }

  CutTree& lastNode() { return nodes_.back(); }

  const Cuts& cuts() const {return cuts_;}
  const Nodes& nodes() const {return nodes_;}
  const char* name() const {return name_.c_str();}

  class Visitor{
  public:
    virtual void accept( const CutTree*) = 0;	
  };		
  
  void visit(Visitor* v) const{
    // visiting all nodes
    for(NI it = nodes_.begin(); it != nodes_.end(); ++it){
      it->visit(v);	
    }	
    
    // doing actual work
    v->accept(this);    
  }

  void print() const{
    std::cout << "Cut tree: " <<name_<<std::endl;
    for(CI it = cuts_.begin(); it != cuts_.end(); ++it){
      std::cout<< "\t"<<(*it)<<std::endl;	
    }	
    
  }

 private:
  /// Cuts directly declared at this level
  Cuts cuts_;

  /// Sub-CutTrees
  Nodes nodes_;
  
  /// name of this CutTree
  std::string name_;
};

template<class CutTreeType>
class CutTreePrinter : public CutTreeType::Visitor {
 public:
  virtual void accept(const CutTreeType* c){
    c->print();	
  }
};

template<class CutTreeType, class ObjectType>
class CutTreeTester : public CutTreeType::Visitor {
 public:
  
  typedef ObjectType object_type;
  
  CutTreeTester( object_type* object ) : object_( object ) {}

  virtual void accept(const CutTreeType* cutTree){
/*     std::cout<<"testing "<<(*object_)<<" with tree "<<cutTree->name()<<std::endl; */

    const typename CutTreeType::Cuts & cuts = cutTree->cuts();	

    bool andResult = true; 
    for( typename CutTreeType::CI ic = cuts.begin(); ic!=cuts.end(); ++ic ) {
/*       std::cout<<" accessing "<<(*ic)<<std::endl; */
      
      const typename CutTreeType::Cuts::value_type::SelectorType& 
	selector = ic->selector();

      bool result = selector( *object_ );
      object_->addSelection( ic->name(), result);
/*       std::cout<<"result "<<result<<std::endl; */
      andResult = andResult && result;
    }

    // need to fold in the result of all subTrees 
    const typename CutTreeType::Nodes & nodes = cutTree->nodes();	    
    for( typename CutTreeType::NI in = nodes.begin(); in!=nodes.end();++in) {
      bool result = object_->getSelection( in->name() ); 
      andResult = andResult && result;
    }

    object_->addSelection( cutTree->name(), andResult ); 
  }

 private:
  object_type*  object_;
  
};



#endif 
