#include "AnalysisDataFormats/CMGTools/interface/QGTag.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "QuarkGluonTagger/EightTeV/interface/QGLikelihoodCalculator.h"
#include "QuarkGluonTagger/EightTeV/interface/QGSyst.h"

namespace QGTag {
    namespace internal {

        QGLikelihoodCalculator & calc() {
            static QGLikelihoodCalculator _calc = QGLikelihoodCalculator("QuarkGluonTagger/EightTeV/data/",false);
            return _calc;
        } 

        QGLikelihoodCalculator & calcCHS() {
            static QGLikelihoodCalculator _calc = QGLikelihoodCalculator("QuarkGluonTagger/EightTeV/data/",true);
            return _calc;
        } 

    }
}

double QGTag::getLD(const cmg::PFJet &jet, double rho, QGTag::QG_Training training, QGTag::QG_Correction corr) 
{
    double ret = -999.0;
    switch (training) {
        case LD: 
            ret = QGTag::internal::calc().computeQGLikelihood2012(jet.pt(), jet.eta(), rho, jet.multPOGQGL(), jet.ptdPOGQGL(), jet.axis2POGQGL());
            break;
        case LD_CHS: 
            ret = QGTag::internal::calcCHS().computeQGLikelihood2012(jet.pt(), jet.eta(), rho, jet.multPOGQGL(), jet.ptdPOGQGL(), jet.axis2POGQGL());
            break;
        case LD_CMGVARS: 
            ret = QGTag::internal::calc().computeQGLikelihood2012(jet.pt(), jet.eta(), rho, jet.nChargedQC() + jet.nNeutralPtCut(), jet.ptdQC(), jet.axisMinorQC());
            break;
        case LD_CHS_CMGVARS: 
            ret = QGTag::internal::calcCHS().computeQGLikelihood2012(jet.pt(), jet.eta(), rho, jet.nChargedQC() + jet.nNeutralPtCut(), jet.ptdQC(), jet.axisMinorQC());
            break;
        default:
            throw cms::Exception("Training not implemented");
    }
    switch (corr) {
        case None:
            return ret;
        default:
            throw cms::Exception("Correction not implemented");
    }
}

