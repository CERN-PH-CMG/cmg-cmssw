#ifndef CMGMuonCleanerBySegmentsAlgo_h
#define CMGMuonCleanerBySegmentsAlgo_h

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include <vector>

class CMGMuonCleanerBySegmentsAlgo {
    public:
        CMGMuonCleanerBySegmentsAlgo(double sharedFraction=0.499, const std::string & preselection="track.isNonnull", const std::string & passthrough="isGlobalMuon && numberOfMatches >= 2") :
            sharedFraction_(sharedFraction), 
            preselection_(preselection,true),
            passthrough_(passthrough,true) {}
        ~CMGMuonCleanerBySegmentsAlgo() ;

        /// Return a vector of boolean marking the muons to be considered clean
        std::vector<bool> clean(const std::vector<cmg::Muon> &muons) const ;

        /// Return a vector of boolean marking the muons to be considered clean
        std::vector<bool> clean(const std::vector<pat::Muon> &muons) const ;
    
        int sharedSegments(const cmg::Muon &mu1, const cmg::Muon &mu2) const ;
    private: 
        /// Fraction of shared segments
        double sharedFraction_;

        /// Preselection cut
        StringCutObjectSelector<pat::Muon> preselection_;
        /// Always-accept cut
        StringCutObjectSelector<pat::Muon> passthrough_;

        /// Methods
        bool isSameMuon(const pat::Muon &mu1, const pat::Muon &mu2) const ;
        bool isBetterMuon(const pat::Muon &mu1, bool mu1PF, const pat::Muon &mu2, bool mu2PF) const ;
};

#endif
