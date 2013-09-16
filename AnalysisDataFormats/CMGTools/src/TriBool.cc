#include "AnalysisDataFormats/CMGTools/interface/TriBool.h"

bool cmg::toBool(const cmg::TriBool& b){
		return b == tbTrue;
}

cmg::TriBool cmg::toTriBool(const bool& b){
		return b ? tbTrue : tbFalse;
}