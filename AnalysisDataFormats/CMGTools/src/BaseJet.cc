#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include <unordered_map>

#include <algorithm>

namespace {
    std::unordered_map<std::string,cmg::BaseJet::BTagWP> _btag_wp_map;
    const std::unordered_map<std::string,cmg::BaseJet::BTagWP> & fill_btag_wp_map() {
        if (_btag_wp_map.empty()) {
            _btag_wp_map["TCHEL"] = cmg::BaseJet::TCHEL;
            _btag_wp_map["TCHEM"] = cmg::BaseJet::TCHEM;
            _btag_wp_map["TCHPT"] = cmg::BaseJet::TCHPT;
            _btag_wp_map["JPL"] = cmg::BaseJet::JPL;
            _btag_wp_map["JPM"] = cmg::BaseJet::JPM;
            _btag_wp_map["JPT"] = cmg::BaseJet::JPT;
            _btag_wp_map["CSVL"] = cmg::BaseJet::CSVL;
            _btag_wp_map["CSVM"] = cmg::BaseJet::CSVM;
            _btag_wp_map["CSVT"] = cmg::BaseJet::CSVT;
        }
        return _btag_wp_map;
    }
}

double cmg::BaseJet::btag(const char* s) const{
    //if there is no Btag info then return default
    if(!btag_.size()) return UnSet(double);
    
    const std::string name(s);
    const TagNameArray::const_iterator index = std::find( btagNames_.begin(),btagNames_.end(), name);
    if( index == btagNames_.end() ) return UnSet(double);
    return btag_.at( index - btagNames_.begin() );
}

bool cmg::BaseJet::btagWP(BTagWP wp) const {
    switch(wp) {
        case TCHEL: return bDiscriminator("trackCountingHighEffBJetTags") > 1.7;
        case TCHEM: return bDiscriminator("trackCountingHighEffBJetTags") > 3.3;
        case TCHPT: return bDiscriminator("trackCountingHighPurBJetTags") > 3.41;
        case JPL: return bDiscriminator("jetProbabilityBJetTags") > 0.275;
        case JPM: return bDiscriminator("jetProbabilityBJetTags") > 0.545;
        case JPT: return bDiscriminator("jetProbabilityBJetTags") > 0.790;
        case CSVL: return bDiscriminator("combinedSecondaryVertexBJetTags") > 0.244;
        case CSVM: return bDiscriminator("combinedSecondaryVertexBJetTags") > 0.679;
        case CSVT: return bDiscriminator("combinedSecondaryVertexBJetTags") > 0.898;
    }
    throw cms::Exception("InvalidArgument", "Unrecognized BTag WP choice");
}

bool cmg::BaseJet::btagWP(const std::string &wp) const {
    const std::unordered_map<std::string,cmg::BaseJet::BTagWP> & wp_map = fill_btag_wp_map();
    auto match = wp_map.find(wp);
    if (match == wp_map.end()) throw cms::Exception("InvalidArgument", wp+" btag WP not known");
    return btagWP(match->second);
}
bool cmg::BaseJet::btagWP(const char *wp) const 
{
    return btagWP(std::string(wp));
}


