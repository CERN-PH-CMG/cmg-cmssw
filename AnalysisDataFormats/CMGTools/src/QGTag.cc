#include "AnalysisDataFormats/CMGTools/interface/QGTag.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "QuarkGluonTagger/EightTeV/interface/QGLikelihoodCalculator.h"
#include "QuarkGluonTagger/EightTeV/interface/QGSyst.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

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

        QGSyst & smearPythia() {
            static QGSyst _smear = QGSyst();
            _smear.ReadDatabaseDoubleMin(edm::FileInPath("QuarkGluonTagger/EightTeV/data/SystDatabase.txt").fullPath());
            return _smear;
        }

        QGSyst & smearHerwig() {
            static QGSyst _smear = QGSyst();
            _smear.ReadDatabaseDoubleMin(edm::FileInPath("QuarkGluonTagger/EightTeV/data/SystDatabase_Hpp.txt").fullPath());
            return _smear;
        }

    }
}

double QGTag::getLD(const cmg::PFJet &jet, double rho, QGTag::QG_Training training, QGTag::QG_Correction corr, const std::string &trueFlavour) 
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
        case Data:
            return ret;
            return ret;
        case Pythia:
        case Herwig:
            if (training == LD || training == LD_CHS || training == LD_CMGVARS || training == LD_CHS_CMGVARS) {
                QGSyst &smear = (corr == Pythia ? QGTag::internal::smearPythia() : QGTag::internal::smearHerwig());
                if (trueFlavour == "auto") {
                    if      (abs(jet.partonPdgId()) == 21) return smear.Smear(jet.pt(), jet.eta(), rho, ret, "gluon");
                    else if (abs(jet.partonPdgId()) != 0 ) return smear.Smear(jet.pt(), jet.eta(), rho, ret, "quark");
                    else                                   return smear.Smear(jet.pt(), jet.eta(), rho, ret, "all");
                } else {
                    return smear.Smear(jet.pt(), jet.eta(), rho, ret, trueFlavour);
                }
            } else {    
                throw cms::Exception("MC Correction not implemented for the given training");
            }
        default:
            throw cms::Exception("Correction not implemented");
    }
    return ret;
}

