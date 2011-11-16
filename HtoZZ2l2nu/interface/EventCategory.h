#ifndef EventCategory_H
#define EventCategory_H

/** \class EventCategory
 *  No description available.
 *
 *  $Date: 2011/11/02 15:32:02 $
 *  $Revision: 1.4 $
 *  \author L. Quertenmont P. Silva
 */



#include <utility>
#include <vector>
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"

class EventCategory {
public:
  bool VBFBinningOnly;
  TString* ZZ2l2nuCategoryLabel;
  enum CategoryType { EQ0JETS, EQ1JETS, GEQ2JETS, VBF };
  
  
  /// Constructor
  EventCategory(bool VBFBinningOnly_=false);
  
  /// Destructor
  virtual ~EventCategory();

  int GetLabelSize(){return VBFBinningOnly ? 3 : 5;}

  int Get(const PhysicsEvent_t& phys,LorentzVectorCollection *variedJetsP4=0);
  TString GetLabel(int CategoryType);
  TString GetLabel(const PhysicsEvent_t& phys);

  int GetCentralJetCount() { return NJetsCentral; }
  int GetVBFJetCount() { return NJetsVBF; }
  TString GetRawLabel(int i){return ZZ2l2nuCategoryLabel[i];}

 private :
  int NJetsCentral,NJetsVBF;

};
#endif

