#ifndef UNSET_H_
#define UNSET_H_

#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"

namespace cmg{
	
	const TriBool unsetTB = tbUnset;
	
	const double unsetD = -9999.;
	const double unsetF = -9999.;
	
	const unsigned int unsetUI = 0;
	const int unsetI = -9999;
	
	const unsigned long unsetUL = 0;
	const long unsetL = -9999;
	
	char const* const unsetS = "";
	
	TriBool unset(const TriBool&);
	double unset(const double&);
	float unset(const float&);
	unsigned int unset(const unsigned int&);
	int unset(const int&);
	unsigned long unset(const unsigned long&);
	long unset(const long&);
	char const* const unset(const char*);

#define UnSet(T) cmg::unset( T() )
    
    template<class T>
    bool isUnset(const T& v){
        return fabs(v - UnSet(T)) < 1e-6;
    }
	
}

#endif /*UNSET_H_*/
