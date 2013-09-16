#ifndef __CMGTools_Common_CMGJetPUIDSelectorDefinition__
#define __CMGTools_Common_CMGJetPUIDSelectorDefinition__
 
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/GenericPhysicsObjectSelectorDefinition.h"

#include <vector>

namespace cmg {
  
    class CMGJetPUIDSelectorDefinition : public GenericPhysicsObjectSelectorDefinition<cmg::PFJet>
    {
    public:
        CMGJetPUIDSelectorDefinition ( const edm::ParameterSet & cfg ) :
            GenericPhysicsObjectSelectorDefinition (cfg),
            idName_(cfg.getParameter<std::string>("puIDName"))
            {
                const std::vector<edm::ParameterSet>& vpset = cfg.getParameter<std::vector<edm::ParameterSet> >("puJetIDParams");
                // Unpack the VPSet into vectors
                for(std::vector<edm::ParameterSet>::const_iterator pset = vpset.begin(); pset != vpset.end(); ++pset)
                {
                    minPt_.push_back(pset->getParameter<double>("minPt"));
                    maxPt_.push_back(pset->getParameter<double>("maxPt"));
                    // eta values must be sorted in increasing abs(eta)
                    maxEtas_.push_back(pset->getParameter< std::vector<double> >("maxEtas")); 
                    minDiscs_.push_back(pset->getParameter< std::vector<double> >("minDiscs"));
                }
            }

            void select( const HandleToCollection & hc, const edm::Event & e, const edm::EventSetup& s) 
            {
                selected_.clear();
                unsigned key=0;

                for(collection::const_iterator pfc = hc->begin(); pfc != hc->end(); ++pfc, ++key) 
                {
                    float mva = pfc->puMva(idName_);
                    float pt = pfc->pt();
                    float abseta = fabs(pfc->eta());
                    for (size_t iPt = 0; iPt < minPt_.size(); ++iPt)
                    {
                        if (pt >= minPt_[iPt] && pt < maxPt_[iPt])
                        {
                            for (size_t iEta = 0; iEta < maxEtas_[iPt].size(); ++iEta)
                            {
                                if (abseta < maxEtas_[iPt][iEta])
                                {
                                    if(mva >minDiscs_[iPt][iEta]) 
                                    {
                                        selected_.push_back( *pfc );
                                        reco::CandidatePtr ptrToMother( hc, key );
                                        selected_.back().setSourceCandidatePtr( ptrToMother );
                                    }
                                break; // exclusive eta ranges
                                }
                            }
                            break; // exclusive pt ranges
                        }
                    }
                }
            }


    private:
        std::string idName_;

        std::vector<double> minPt_;
        std::vector<double> maxPt_;

        std::vector< std::vector<double> > maxEtas_;
        std::vector< std::vector<double> > minDiscs_;

    };

}

#endif
