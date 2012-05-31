#ifndef EDMEVENTHELPEREXTRA_H
#define EDMEVENTHELPEREXTRA_H
//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for edm::Event
// Created:     Tue May  8 09:28:47 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include "PhysicsTools/TheNtupleMaker/interface/HelperFor.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "TLorentzVector.h"
//-----------------------------------------------------------------------------
// Definitions:
//   helper:        object of class EventHelperExtra
//   helped object: object of class edm::Event
//
//
// The following variables are automatically defined and available to
//       all methods:
//
//         blockname          name of config. buffer object (config block) 
//         buffername         name of buffer in config block
//         labelname          name of label in config block (for getByLabel)
//         parameter          parameter (as key, value pairs)
//                            accessed as in the following example:
//
//                            string param = parameter("label");
//
//         0. hltconfig       pointer to HLTConfigProvider
//                            Note: this will be zero if HLTConfigProvider
//                                  has not been properly initialized
//
//         1. config          pointer to global ParameterSet object
//         2. event           pointer to the current event
//         3. object          pointer to the current helped object
//         4. oindex          index of current helped object
//
//         5. index           index of item(s) returned by helper.
//                            Note 1: an item is associated with all
//                                    helper methods (think of it as an
//                                    extension of the helped object)
//                                  
//                            Note 2: index may differ from oindex if,
//                                    for a given helped object, the count
//                                    variable (see below) differs from 1.
//
//         6. count           number of items per helped object (default=1)
//                            Note:
//                                  count = 0  ==> current helped object is
//                                                 to be skipped
//
//                                  count = 1  ==> current helped object is
//                                                 to be kept
//
//                                  count > 1  ==> current helped object is
//                                                 associated with "count"
//                                                 items, where each item
//                                                 is associated with all the
//                                                 helper methods
//
//       variables 0-6 are initialized by TheNtupleMaker.
//       variables 0-5 should not be changed.
//       variable    6 can be changed by the helper to control whether a
//                     helped object should be kept or generates more items
//-----------------------------------------------------------------------------

namespace edm
{
  /// A helper class for edm::Event.
  class EventHelperExtra : public HelperFor<edm::Event>
  {
  public:
	///
	EventHelperExtra();

	virtual ~EventHelperExtra();

	// Uncomment if this class does some event-level analysis
	virtual void analyzeEvent();
	 
	// Uncomment if this class does some object-level analysis
	// virtual void analyzeObject();

	// ---------------------------------------------------------
	// -- User access methods go here
	// ---------------------------------------------------------

	
  private:
    // -- User internals
        // dijet invariant mass:
        double dijet_invmass_;
        double dijetCHS_invmass_;
        double dijetCHSpruned_invmass_;

	// Normal jets:
	// wide jet 1:
	double wj1_pt_;
	double wj1_eta_;
	double wj1_phi_;
	double wj1_energy_;
	double wj1_mass_;
	int wj1_nconst_;
	// wide jet 2:
	double wj2_pt_;
	double wj2_eta_;
	double wj2_phi_;
	double wj2_energy_;
	double wj2_mass_;
	int wj2_nconst_;
	// Number of common constituents
	int wj1wj2_nconst_;
        double wj1wj2_invmass_;
	// CHS jets:
	// wide jet 1:
	double wj1CHS_pt_;
	double wj1CHS_eta_;
	double wj1CHS_phi_;
	double wj1CHS_energy_;
	double wj1CHS_mass_;
	int wj1CHS_nconst_;
	// wide jet 2:
	double wj2CHS_pt_;
	double wj2CHS_eta_;
	double wj2CHS_phi_;
	double wj2CHS_energy_;
	double wj2CHS_mass_;
	int wj2CHS_nconst_;
	// Number of common constituents
	int wj1wj2CHS_nconst_;
        double wj1wj2CHS_invmass_;


  public:
    // ---------------------------------------------------------
    // -- Access Methods
    // ---------------------------------------------------------

        // dijet invariant mass:
        double dijet_invmass() const;
        double dijetCHS_invmass() const;
        double dijetCHSpruned_invmass() const;

	// Normal jets:
	// wide jet 1:
	double wj1_pt() const;
	double wj1_eta() const;
	double wj1_phi() const;
	double wj1_energy() const;
	double wj1_mass() const;
	int wj1_nconst() const;
	// wide jet 2:
	double wj2_pt() const;
	double wj2_eta() const;
	double wj2_phi() const;
	double wj2_energy() const;
	double wj2_mass() const;
	int wj2_nconst() const;
	// Number of common constituents
	int wj1wj2_nconst() const;
        double wj1wj2_invmass() const;
	// CHS jets:
	// wide jet 1:
	double wj1CHS_pt() const;
	double wj1CHS_eta() const;
	double wj1CHS_phi() const;
	double wj1CHS_energy() const;
	double wj1CHS_mass() const;
	int wj1CHS_nconst() const;
	// wide jet 2:
	double wj2CHS_pt() const;
	double wj2CHS_eta() const;
	double wj2CHS_phi() const;
	double wj2CHS_energy() const;
	double wj2CHS_mass() const;
	int wj2CHS_nconst() const;
	// Number of common constituents
	int wj1wj2CHS_nconst() const;
        double wj1wj2CHS_invmass() const;


  };
}
#endif
