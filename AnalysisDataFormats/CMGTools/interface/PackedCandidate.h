#ifndef __AnalysisDataFormats_CMGTools_PackedCandidate_h__
#define __AnalysisDataFormats_CMGTools_PackedCandidate_h__

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/iterator_imp_specific.h"
/* #include "DataFormats/Math/interface/PtEtaPhiMass.h" */

namespace cmg {
  class PackedCandidate : public reco::Candidate {
  public:
    /// collection of daughter candidates                                                 
    typedef reco::CandidateCollection daughters;
    /// Lorentz vector                                                                    
    typedef math::XYZTLorentzVector LorentzVector;
    /// Lorentz vector                                                                    
    typedef math::PtEtaPhiMLorentzVector PolarLorentzVector;
    /// point in the space                                                                
    typedef math::XYZPoint Point;
    /// point in the space                                                                
    typedef math::XYZVector Vector;

    typedef unsigned int index;

    /// default constructor  
  PackedCandidate()
    : p4_(0,0,0,0), p4c_(0,0,0,0), vertex_(0,0,0), pdgId_(0), fromPV_(0), unpacked_(false) { }                                                             

  explicit PackedCandidate( const reco::Candidate & c, bool fromPV)
    : p4_(c.pt(), c.eta(), c.phi(), c.mass()), p4c_(p4_), vertex_(c.vertex()), pdgId_(c.pdgId()), fromPV_(fromPV), unpacked_(true) { pack(); }

  explicit PackedCandidate( const PolarLorentzVector &p4, const Point &vtx, int pdgId, bool fromPV)
    : p4_(p4), p4c_(p4_), vertex_(vtx), pdgId_(pdgId), fromPV_(fromPV), unpacked_(true) { pack(); }

  explicit PackedCandidate( const LorentzVector &p4, const Point &vtx, int pdgId, bool fromPV)
    : p4_(p4.Pt(), p4.Eta(), p4.Phi(), p4.M()), p4c_(p4), vertex_(vtx), pdgId_(pdgId), fromPV_(fromPV), unpacked_(true) { pack(); }
 
 
    
    
    /// destructor
    virtual ~PackedCandidate();
    /// first daughter const_iterator
    virtual const_iterator begin() const;
    /// last daughter const_iterator
    virtual const_iterator end() const;
    /// first daughter iterator
    virtual iterator begin();
    /// last daughter iterator
    virtual iterator end();
    /// number of daughters
    virtual size_t numberOfDaughters() const;
    /// return daughter at a given position (throws an exception)
    virtual const reco::Candidate * daughter( size_type ) const;
    /// number of mothers
    virtual size_t numberOfMothers() const;
    /// return mother at a given position (throws an exception)
    virtual const reco::Candidate * mother( size_type ) const;
    /// return daughter at a given position (throws an exception)
    virtual reco::Candidate * daughter( size_type );
    /// return daughter with a specified role name
    virtual reco::Candidate * daughter(const std::string& s );
    /// return daughter with a specified role name                                        
    virtual const reco::Candidate * daughter(const std::string& s ) const;
    /// return the number of source Candidates                                            
    /// ( the candidates used to construct this Candidate)                                
    virtual size_t numberOfSourceCandidatePtrs() const {return 0;}
    /// return a Ptr to one of the source Candidates                                      
    /// ( the candidates used to construct this Candidate)                                
    virtual reco::CandidatePtr sourceCandidatePtr( size_type i ) const {
      return reco::CandidatePtr();
    }

    /// electric charge
    virtual int charge() const {
      switch (abs(pdgId_)) {
      case 211: return (pdgId_>0)-(pdgId_<0);
      case 11:  return (-1)*(pdgId_>0)-(pdgId_<0); //e
      case 13:  return (-1)*(pdgId_>0)-(pdgId_<0); //mu
      case 15:  return (-1)*(pdgId_>0)-(pdgId_<0); //tau
      case 24:  return (-1)*(pdgId_>0)-(pdgId_<0); //W
      default:  return 0;  //FIXME: charge is not defined
      }
    }
    /// set electric charge                                                               
    virtual void setCharge( int charge) {}
    /// electric charge                                                                   
    virtual int threeCharge() const {return charge()*3;}
    /// set electric charge                                                               
    virtual void setThreeCharge( int threecharge) {}
    /// four-momentum Lorentz vecto r                                                      
    virtual const LorentzVector & p4() const { if (!unpacked_) unpack(); return p4c_; }  
    /// four-momentum Lorentz vector                                                      
    virtual const PolarLorentzVector & polarP4() const { if (!unpacked_) unpack(); return p4_; }
    /// spatial momentum vector                                                           
    virtual Vector momentum() const  { if (!unpacked_) unpack(); return p4c_.Vect(); }
    /// boost vector to boost a Lorentz vector                                            
    /// to the particle center of mass system                                             
    virtual Vector boostToCM() const { if (!unpacked_) unpack(); return p4c_.BoostToCM(); }
    /// magnitude of momentum vector                                                      
    virtual double p() const { if (!unpacked_) unpack(); return p4c_.P(); }
    /// energy                                                                            
    virtual double energy() const { if (!unpacked_) unpack(); return p4c_.E(); }
    /// transverse energy                                                                 
    virtual double et() const { if (!unpacked_) unpack(); return p4_.Et(); }
    /// mass                                                                              
    virtual double mass() const { if (!unpacked_) unpack(); return p4_.M(); }
    /// mass squared                                                                      
    virtual double massSqr() const { if (!unpacked_) unpack(); return p4_.M()*p4_.M(); }

    /// transverse mass                                                                   
    virtual double mt() const { if (!unpacked_) unpack(); return p4_.Mt(); }
    /// transverse mass squared                                                           
    virtual double mtSqr() const { if (!unpacked_) unpack(); return p4_.Mt2(); }
    /// x coordinate of momentum vector                                                   
    virtual double px() const { if (!unpacked_) unpack(); return p4c_.Px(); }
    /// y coordinate of momentum vector                                                   
    virtual double py() const { if (!unpacked_) unpack(); return p4c_.Py(); }
    /// z coordinate of momentum vector                                                   
    virtual double pz() const { if (!unpacked_) unpack(); return p4c_.Pz(); }
    /// transverse momentum                                                               
    virtual double pt() const { if (!unpacked_) unpack(); return p4_.Pt();}
    /// momentum azimuthal angle                                                          
    virtual double phi() const { if (!unpacked_) unpack(); return p4_.Phi(); }
    /// momentum polar angle                                                              
    virtual double theta() const { if (!unpacked_) unpack(); return p4_.Theta(); }
    /// momentum pseudorapidity                                                           
    virtual double eta() const { if (!unpacked_) unpack(); return p4_.Eta(); }
    /// rapidity                                                                          
    virtual double rapidity() const { if (!unpacked_) unpack(); return p4_.Rapidity(); }
    /// rapidity                                                                          
    virtual double y() const { if (!unpacked_) unpack(); return p4_.Rapidity(); }
    /// set 4-momentum                                                                    
    virtual void setP4( const LorentzVector & p4 ) { if (!unpacked_) unpack(); p4_ = PolarLorentzVector(p4.Pt(), p4.Eta(), p4.Phi(), p4.M()); pack(); }
    /// set 4-momentum                                                                    
    virtual void setP4( const PolarLorentzVector & p4 ) { if (!unpacked_) unpack(); p4_ = p4; pack(); }
    /// set particle mass                                                                 
    virtual void setMass( double m ) {
      if (!unpacked_) unpack(); 
      p4_ = PolarLorentzVector(p4_.Pt(), p4_.Eta(), p4_.Phi(), m); 
      pack();
    }
    virtual void setPz( double pz ) {
      if (!unpacked_) unpack(); 
      p4c_ = LorentzVector(p4c_.Px(), p4c_.Py(), pz, p4c_.E());
      p4_  = PolarLorentzVector(p4c_.Pt(), p4c_.Eta(), p4c_.Phi(), p4c_.M());
      pack();
    }
    /// vertex position
    virtual const Point & vertex() const { if (!unpacked_) unpack(); return vertex_; }//{ if (fromPV_) return Point(0,0,0); else return Point(0,0,100); }
    /// x coordinate of vertex position                                                   
    virtual double vx() const  { if (!unpacked_) unpack(); return vertex_.X(); }//{ return 0; }
    /// y coordinate of vertex position                                                   
    virtual double vy() const  { if (!unpacked_) unpack(); return vertex_.Y(); }//{ return 0; }
    /// z coordinate of vertex position                                                   
    virtual double vz() const  { if (!unpacked_) unpack(); return vertex_.Z(); }//{ if (fromPV_) return 0; else return 100; }
    /// set vertex                                                                        
    virtual void setVertex( const Point & vertex ) { if (!unpacked_) unpack(); vertex_ = vertex; pack(); }

    virtual const bool & fromPV() const { return fromPV_; }
    virtual void setFromPV( const bool fromPV )   { fromPV_ = fromPV; }

    /// PDG identifier                                                                    
    /// PDG identifier                                                                    
    virtual int pdgId() const   { return pdgId_; }
    // set PDG identifier                                                                 
    virtual void setPdgId( int pdgId )   { pdgId_ = pdgId; }
    /// status word                                                                       
    virtual int status() const   { return 1; } /*FIXME*/
    /// set status word                                                                   
    virtual void setStatus( int status ) {} /*FIXME*/
    /// long lived flag                                                                   
    static const unsigned int longLivedTag = 0; /*FIXME*/
    /// set long lived flag                                                               
    virtual void setLongLived() {} /*FIXME*/
    /// is long lived?                                                                    
    virtual bool longLived() const;
    /// do mass constraint flag
    static const unsigned int massConstraintTag = 0; /*FIXME*/ 
    /// set mass constraint flag
    virtual void setMassConstraint() {} /*FIXME*/
    /// do mass constraint?
    virtual bool massConstraint() const;

    /// returns a clone of the Candidate object                                           
    virtual PackedCandidate * clone() const  {
      return new PackedCandidate( *this );
    }

    /// chi-squares                                                                                                    
    virtual double vertexChi2() const;
    /** Number of degrees of freedom                                                                                   
     *  Meant to be Double32_t for soft-assignment fitters:                                                            
     *  tracks may contribute to the vertex with fractional weights.                                                   
     *  The ndof is then = to the sum of the track weights.                                                            
     *  see e.g. CMS NOTE-2006/032, CMS NOTE-2004/002                                                                  
     */
    virtual double vertexNdof() const;
    /// chi-squared divided by n.d.o.f.                                                                                
    virtual double vertexNormalizedChi2() const;
    /// (i, j)-th element of error matrix, i, j = 0, ... 2                                                             
    virtual double vertexCovariance(int i, int j) const;
    /// return SMatrix                                                                                                 
    CovarianceMatrix vertexCovariance() const   { CovarianceMatrix m; fillVertexCovariance(m); return m; }
    /// fill SMatrix                                                                                                   
    virtual void fillVertexCovariance(CovarianceMatrix & v) const;
    /// returns true if this candidate has a reference to a master clone.                                              
    /// This only happens if the concrete Candidate type is ShallowCloneCandidate                                      
    virtual bool hasMasterClone() const;
    /// returns ptr to master clone, if existing.                                                                      
    /// Throws an exception unless the concrete Candidate type is ShallowCloneCandidate                                
    virtual const reco::CandidateBaseRef & masterClone() const;
    /// returns true if this candidate has a ptr to a master clone.                                                    
    /// This only happens if the concrete Candidate type is ShallowClonePtrCandidate                                   
    virtual bool hasMasterClonePtr() const;
    /// returns ptr to master clone, if existing.                                                                      
    /// Throws an exception unless the concrete Candidate type is ShallowClonePtrCandidate                             

    virtual const reco::CandidatePtr & masterClonePtr() const;

    /// cast master clone reference to a concrete type
    template<typename Ref>
      Ref masterRef() const { return masterClone().template castTo<Ref>(); }
    /// get a component

    /* template<typename T> T get() const { */
    /*   if ( hasMasterClone() ) return masterClone()->get<T>(); */
    /*   else return reco::get<T>( * this ); */
    /* } */
    /* /// get a component                                                                                                 */
    /* template<typename T, typename Tag> T get() const { */
    /*   if ( hasMasterClone() ) return masterClone()->get<T, Tag>(); */
    /*   else return reco::get<T, Tag>( * this ); */
    /* } */
    /* /// get a component                                                                                                 */
    /* template<typename T> T get( size_type i ) const { */
    /*   if ( hasMasterClone() ) return masterClone()->get<T>( i ); */
    /*   else return reco::get<T>( * this, i ); */
    /* } */
    /* /// get a component                                                                                                 */
    /* template<typename T, typename Tag> T get( size_type i ) const { */
    /*   if ( hasMasterClone() ) return masterClone()->get<T, Tag>( i ); */
    /*   else return reco::get<T, Tag>( * this, i ); */
    /* } */
    /* /// number of components                                                                                            */
    /* template<typename T> size_type numberOf() const { */
    /*   if ( hasMasterClone() ) return masterClone()->numberOf<T>(); */
    /*   else return reco::numberOf<T>( * this ); */
    /* } */
    /* /// number of components                                                                                            */
    /* template<typename T, typename Tag> size_type numberOf() const { */
    /*   if ( hasMasterClone() ) return masterClone()->numberOf<T, Tag>(); */
    /*   else return reco::numberOf<T, Tag>( * this ); */
    /* } */

    /* template<typename S> */
    /*   struct daughter_iterator   { */
    /*     typedef boost::filter_iterator<S, const_iterator> type; */
    /*   }; */

    /* template<typename S> */
    /*   typename daughter_iterator<S>::type beginFilter( const S & s ) const { */
    /*   return boost::make_filter_iterator(s, begin(), end()); */
    /* } */
    /* template<typename S> */
    /*   typename daughter_iterator<S>::type endFilter( const S & s ) const { */
    /*   return boost::make_filter_iterator(s, end(), end()); */
    /* } */


    virtual bool isElectron() const;
    virtual bool isMuon() const;
    virtual bool isStandAloneMuon() const;
    virtual bool isGlobalMuon() const;
    virtual bool isTrackerMuon() const;
    virtual bool isCaloMuon() const;
    virtual bool isPhoton() const;
    virtual bool isConvertedPhoton() const;
    virtual bool isJet() const;

  protected:
    uint16_t packedPt_, packedEta_, packedPhi_, packedM_;
    uint16_t packedX_, packedY_, packedZ_;
    void pack() ;
    void unpack() const ;
 
    /// the four vector                                                 
    mutable PolarLorentzVector p4_;
    mutable LorentzVector p4c_;
    /// vertex position                                                                   
    mutable Point vertex_;
    /// PDG identifier                                                                    
    int pdgId_;
    bool fromPV_;
    // are the vectors unpacked
    mutable bool unpacked_;

    /// check overlap with another Candidate                                              
    virtual bool overlap( const reco::Candidate & ) const;
    template<typename, typename, typename> friend struct component;
    friend class ::OverlapChecker;
    friend class ShallowCloneCandidate;
    friend class ShallowClonePtrCandidate;

  private:
    // const iterator implementation
    typedef reco::candidate::const_iterator_imp_specific<daughters> const_iterator_imp_specific;
    // iterator implementation
    typedef reco::candidate::iterator_imp_specific<daughters> iterator_imp_specific;
  };

}

#endif
