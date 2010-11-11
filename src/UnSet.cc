#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

cmg::TriBool cmg::unset(const cmg::TriBool&){
		return cmg::unsetTB;
}
double cmg::unset(const double&){
	return cmg::unsetD;
}
float cmg::unset(const float&){
	return cmg::unsetF;
}
unsigned int cmg::unset(const unsigned int&){
	return cmg::unsetUI;
}
int cmg::unset(const int&){
		return cmg::unsetI;
}
unsigned long cmg::unset(const unsigned long&){
	return cmg::unsetUL;
}
long cmg::unset(const long&){
	return cmg::unsetL;
}
char const* const cmg::unset(const char*){
	return cmg::unsetS;
}