#ifndef HiggsAnalysis_CombinedLimit_PdfDiagonalizer_h
#define HiggsAnalysis_CombinedLimit_PdfDiagonalizer_h

#include <cstdio>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <RooAbsPdf.h>
#include <RooAddition.h>
#include <RooCustomizer.h>
#include <RooFitResult.h>
#include <RooRealVar.h>
#include <RooWorkspace.h>

// class RooWorkspace;
// struct RooFitResult;
// struct RooAbsPdf;

#include <string>
#include <RooArgList.h>

class PdfDiagonalizer {
    public:
        PdfDiagonalizer(const char *name, RooWorkspace *w, RooFitResult &result);

        RooAbsPdf *diagonalize(RooAbsPdf &pdf) ;
        RooAbsPdf *diagonalizeWithEigenVariations(RooAbsPdf &pdf, RooFitResult &result, int index_eig, int nSigma) ;
        const RooArgList & originalParams() { return parameters_; }
        const RooArgList & diagonalParams() { return eigenVars_; }
    private:
        std::string name_;
        RooArgList  parameters_;
        RooArgList  eigenVars_;
        RooArgList  replacements_;
        // TVectorD& eigen_gl;

};

#endif