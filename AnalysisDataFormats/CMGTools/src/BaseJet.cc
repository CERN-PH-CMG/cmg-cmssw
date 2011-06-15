#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"

#include <algorithm>

double cmg::BaseJet::btag(const char* s) const{
    //if there is no Btag info then return default
    if(!btag_.size()) return UnSet(double);
    
    const std::string name(s);
    const TagNameArray::const_iterator index = std::find( btagNames_.begin(),btagNames_.end(), name);
    if( index == btagNames_.end() ) return UnSet(double);
    return btag_.at( index - btagNames_.begin() );
}