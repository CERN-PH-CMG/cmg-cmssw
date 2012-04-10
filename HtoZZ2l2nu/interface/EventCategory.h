#ifndef EventCategory_H
#define EventCategory_H

/** \class EventCategory
 *  No description available.
 *
 *  $Date: 2012/04/07 13:19:56 $
 *  $Revision: 1.6 $
 *  \author L. Quertenmont P. Silva
 */



#include <utility>
#include <vector>
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

class EventCategory {
public:
  int mode;
  int NStates;
  TString* ZZ2l2nuCategoryLabel;
  enum CategoryType { EQ0JETS, EQ1JETS, GEQ2JETS, VBF };
  
  
  /// Constructor
  EventCategory(int mode=0);
  
  /// Destructor
  virtual ~EventCategory();

  int GetLabelSize(){return NStates;}

  int Get(const PhysicsEvent_t& phys,LorentzVectorCollection *variedJetsP4=0);
  TString GetLabel(int CategoryType);
  TString GetLabel(const PhysicsEvent_t& phys);

  int GetCentralJetCount() { return NJetsCentral; }
  int GetVBFJetCount() { return NJets; }
  TString GetRawLabel(int i){return ZZ2l2nuCategoryLabel[i];}

 private :
  int NJetsCentral,NJets;

};
#endif

