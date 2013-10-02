#include "CMGTools/Common/interface/MultiObjectSettingTool.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaPhi.h"

//--------------------------------------------------------------------

void cmg::MultiObjectSettingTool::set(std::vector<edm::Ptr<reco::Candidate> > const& input,
        cmg::MultiObject* const object) const
{
    object->deltaPhiStar_ = deltaPhiStar(input);
    object->ht_ = ht(input);
}

//--------------------------------------------------------------------

// This is the delta-phi^star variable as defined in formula 5 (p. 10)
// of CMS AN-2010/242.
// NOTE: This is a delta-phi in [0, pi].
double cmg::MultiObjectSettingTool::deltaPhiStar(std::vector<
        edm::Ptr<reco::Candidate> >
        const& input) const
{
    double deltaPhiStar = cmg::unsetD;

    typedef math::XYZVector Vector;
    Vector negSumMom;
    for (std::vector<edm::Ptr<reco::Candidate> >::const_iterator it = input.begin(),
            end = input.end(); it != end; ++it) {
        negSumMom -= (*it)->momentum();
    }

    double deltaPhiMin = std::numeric_limits<double>::max();
    for (std::vector<edm::Ptr<reco::Candidate> >::const_iterator it = input.begin(),
            end = input.end(); it != end; ++it) {
        Vector tmp = negSumMom + (*it)->momentum();
        double deltaPhi = reco::deltaPhi(tmp.phi(), (*it)->phi());
        deltaPhi = fabs(deltaPhi);
        if (deltaPhi < deltaPhiMin) {
            deltaPhiMin = deltaPhi;
        }
    }

    deltaPhiStar = deltaPhiMin;
    return deltaPhiStar;
}

//this is the sum of the pts of the objects forming the MultiObject
double cmg::MultiObjectSettingTool::ht(std::vector<
        edm::Ptr<reco::Candidate> >
        const& input) const {
    double pt_sum = 0;
    for (std::vector<edm::Ptr<reco::Candidate> >::const_iterator it = input.begin(),
            end = input.end(); it != end; ++it) {
        pt_sum += (*it)->pt();
    }
    return pt_sum;
}

//--------------------------------------------------------------------
