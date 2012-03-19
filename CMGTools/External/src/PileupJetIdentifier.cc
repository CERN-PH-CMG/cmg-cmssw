#include "../interface/PileupJetIdentifier.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

//// // ------------------------------------------------------------------------------------------
//// const float large_val = std::numeric_limits<float>::max();

// ------------------------------------------------------------------------------------------
PileupJetIdentifier::PileupJetIdentifier() 
{
///	initVariables();
}

//// // ------------------------------------------------------------------------------------------
//// PileupJetIdentifier::PileupJetIdentifier(const PileupJetIdentifier &p)
//// {
//// 	initVariables();
//// 	for(variables_list_t::const_iterator it=variables_.begin(); 
//// 	    it!=variables_.end(); ++it ) {
//// 		*it->second.first = *p.variables_.find( it->first )->second.first;
//// 	}
//// }

//// // ------------------------------------------------------------------------------------------
//// #define INIT_VARIABLE(NAME,TMVANAME,VAL)	\
//// 	NAME ## _ = VAL; \
//// 	variables_[ # NAME   ] = std::make_pair(& NAME ## _, VAL);
//// 
//// // ------------------------------------------------------------------------------------------
//// void PileupJetIdentifier::initVariables()
//// {
////   	INIT_VARIABLE(mva        , "", -100.);
//// 	INIT_VARIABLE(jetPt      , "jspt_1", 0.);
//// 	INIT_VARIABLE(jetEta     , "jseta_1", large_val);
//// 	INIT_VARIABLE(jetPhi     , "jsphi_1", large_val);
//// 	INIT_VARIABLE(jetM       , "jm_1", 0.);
//// 
//// 	INIT_VARIABLE(nCharged   , "", 0.);
//// 	INIT_VARIABLE(nNeutrals  , "", 0.);
//// 	
//// 	INIT_VARIABLE(chgEMfrac  , "", 0.);
//// 	INIT_VARIABLE(neuEMfrac  , "", 0.);
//// 	INIT_VARIABLE(chgHadrfrac, "", 0.);
//// 	INIT_VARIABLE(neuHadrfrac, "", 0.);
//// 	
//// 	INIT_VARIABLE(d0         , "jd0_1"    , -1000.);   
//// 	INIT_VARIABLE(dZ         , "jdZ_1"    , -1000.);  
//// 	INIT_VARIABLE(nParticles , "npart_1"  , 0.);  
//// 	
//// 	INIT_VARIABLE(leadPt     , "lpt_1"    , 0.);  
//// 	INIT_VARIABLE(leadEta    , "leta_1"   , large_val);  
//// 	INIT_VARIABLE(leadPhi    , "lphi_1"   , large_val);  
//// 	INIT_VARIABLE(secondPt   , "spt_1"    , 0.);  
//// 	INIT_VARIABLE(secondEta  , "seta_1"   , large_val);  
//// 	INIT_VARIABLE(secondPhi  , "sphi_1"   , large_val);  
//// 	INIT_VARIABLE(leadNeutPt , "lnept_1"    , 0.);  
//// 	INIT_VARIABLE(leadNeutEta, "lneeta_1"   , large_val);  
//// 	INIT_VARIABLE(leadNeutPhi, "lnephi_1"   , large_val);  
//// 	INIT_VARIABLE(leadEmPt   , "lempt_1"  , 0.);  
//// 	INIT_VARIABLE(leadEmEta  , "lemeta_1" , large_val);  
//// 	INIT_VARIABLE(leadEmPhi  , "lemphi_1" , large_val);  
//// 	INIT_VARIABLE(leadChPt   , "lchpt_1"  , 0.);  
//// 	INIT_VARIABLE(leadChEta  , "lcheta_1" , large_val);  
//// 	INIT_VARIABLE(leadChPhi  , "lchphi_1" , large_val);  
//// 	INIT_VARIABLE(leadFrac   , "lLfr_1"   , 0.);  
//// 	
//// 	INIT_VARIABLE(dRLeadCent , "drlc_1"   , 0.);  
//// 	INIT_VARIABLE(dRLead2nd  , "drls_1"   , 0.);  
//// 	INIT_VARIABLE(dRMean     , "drm_1"    , 0.);  
//// 	INIT_VARIABLE(dRMeanNeut , "drmne_1"  , 0.);  
//// 	INIT_VARIABLE(dRMeanEm   , "drem_1"   , 0.);  
//// 	INIT_VARIABLE(dRMeanCh   , "drch_1"   , 0.);  
//// 	
//// 	INIT_VARIABLE(ptD        , "", 0.);
//// 
//// 	INIT_VARIABLE(secondFrac  ,"" ,0.);  
//// 	INIT_VARIABLE(thirdFrac   ,"" ,0.);  
//// 	INIT_VARIABLE(fourthFrac  ,"" ,0.);  
//// 
//// 	INIT_VARIABLE(leadChFrac    ,"" ,0.);  
//// 	INIT_VARIABLE(secondChFrac  ,"" ,0.);  
//// 	INIT_VARIABLE(thirdChFrac   ,"" ,0.);  
//// 	INIT_VARIABLE(fourthChFrac  ,"" ,0.);  
//// 
//// 	INIT_VARIABLE(leadNeutFrac    ,"" ,0.);  
//// 	INIT_VARIABLE(secondNeutFrac  ,"" ,0.);  
//// 	INIT_VARIABLE(thirdNeutFrac   ,"" ,0.);  
//// 	INIT_VARIABLE(fourthNeutFrac  ,"" ,0.);  
//// 
//// 	INIT_VARIABLE(leadEmFrac    ,"" ,0.);  
//// 	INIT_VARIABLE(secondEmFrac  ,"" ,0.);  
//// 	INIT_VARIABLE(thirdEmFrac   ,"" ,0.);  
//// 	INIT_VARIABLE(fourthEmFrac  ,"" ,0.);  
//// 
//// 	INIT_VARIABLE(jetW  ,"" ,1.);  
//// 	INIT_VARIABLE(etaW  ,"" ,1.);  
//// 	INIT_VARIABLE(phiW  ,"" ,1.);  
//// 
//// 	INIT_VARIABLE(majW  ,"" ,1.);  
//// 	INIT_VARIABLE(minW  ,"" ,1.);  
//// 
//// 	INIT_VARIABLE(frac01    ,"" ,0.);  
//// 	INIT_VARIABLE(frac02    ,"" ,0.);  
//// 	INIT_VARIABLE(frac03    ,"" ,0.);  
//// 	INIT_VARIABLE(frac04    ,"" ,0.);  
//// 	INIT_VARIABLE(frac05   ,"" ,0.);  
//// 	
//// 	INIT_VARIABLE(chFrac01    ,"" ,0.);  
//// 	INIT_VARIABLE(chFrac02    ,"" ,0.);  
//// 	INIT_VARIABLE(chFrac03    ,"" ,0.);  
//// 	INIT_VARIABLE(chFrac04    ,"" ,0.);  
//// 	INIT_VARIABLE(chFrac05   ,"" ,0.);  
//// 
//// 	INIT_VARIABLE(neutFrac01    ,"" ,0.);  
//// 	INIT_VARIABLE(neutFrac02    ,"" ,0.);  
//// 	INIT_VARIABLE(neutFrac03    ,"" ,0.);  
//// 	INIT_VARIABLE(neutFrac04    ,"" ,0.);  
//// 	INIT_VARIABLE(neutFrac05   ,"" ,0.);  
//// 
//// 	INIT_VARIABLE(emFrac01    ,"" ,0.);  
//// 	INIT_VARIABLE(emFrac02    ,"" ,0.);  
//// 	INIT_VARIABLE(emFrac03    ,"" ,0.);  
//// 	INIT_VARIABLE(emFrac04    ,"" ,0.);  
//// 	INIT_VARIABLE(emFrac05   ,"" ,0.);  
//// 	
//// }
//// 
//// #undef INIT_VARIABLE


// ------------------------------------------------------------------------------------------
PileupJetIdentifier::~PileupJetIdentifier() 
{
}

//// // ------------------------------------------------------------------------------------------
//// std::string PileupJetIdentifier::dumpVariables() const
//// {
//// 	std::stringstream out;
//// 	for(variables_list_t::const_iterator it=variables_.begin(); 
//// 	    it!=variables_.end(); ++it ) {
//// 		out << std::setw(15) << it->first << std::setw(3) << "=" 
//// 		    << std::setw(5) << *it->second.first 
//// 		    << " (" << std::setw(5) << it->second.second << ")" << std::endl;
//// 	}
//// 	return out.str();
//// }
//// 
//// // ------------------------------------------------------------------------------------------
//// void PileupJetIdentifier::resetVariables()
//// {
//// 	for(variables_list_t::iterator it=variables_.begin(); 
//// 	    it!=variables_.end(); ++it ) {
//// 		*it->second.first = it->second.second;
//// 	}
//// }
