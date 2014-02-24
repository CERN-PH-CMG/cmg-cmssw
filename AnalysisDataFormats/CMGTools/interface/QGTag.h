#ifndef AnalysisDataFormats_CMGTools_QGTag_h
#define AnalysisDataFormats_CMGTools_QGTag_h

// FW Declaration
namespace cmg { class PFJet; }

namespace QGTag {
    /// LD, LD_CHS: POG reference implementation of LD for non-CHS and CHS jets,
    /// *_CMGVARS:  Same training but using the pre-exising variables in CMG Jets (for backwards compatibility)
    /// VBFHBB:     Re-optimized LD for VBF H->BB
    enum QG_Training   { LD, LD_CHS, LD_CMGVARS, LD_CHS_CMGVARS, VBFHBB };
    enum QG_Correction { None, Data, Pythia, Herwig };

    double getLD(const cmg::PFJet &jet, double rho, QG_Training training, QG_Correction corr = None) ; 
}

#endif
