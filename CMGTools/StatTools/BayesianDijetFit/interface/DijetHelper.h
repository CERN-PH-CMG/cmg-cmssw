#ifndef __DIJET_HELPER_H__
#define __DIJET_HELPER_H__

#include <string>
#include <vector>

class RooDataHist;
class RooDataSet;
class RooAbsPdf;
class RooRealVar;
class RooAbsData;
class RooFitResult;
class TH1D;
class RooAbsReal;

class RooWorkspace;

namespace DijetHelper {

  RooDataHist* makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double *binsx);
  RooDataHist* makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi);
  RooDataHist* makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, TH1D* hist);

  RooDataHist* makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double *binsx);
  RooDataHist* makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi);
  RooDataHist* makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, TH1D* hist);

  RooDataSet* makeUnbinnedData(std::string filename, std::string objectname, RooRealVar* var);

  double calcPDF1DIntegral(RooAbsPdf* pdf, RooRealVar* var, double min, double max);

  RooFitResult* doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, RooAbsReal* nsig, RooAbsReal* nbkg, int nbins, double *binsx, const char* range, RooWorkspace* ws=0, bool verbose = false);
  RooFitResult* doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, RooAbsReal* nsig, RooAbsReal* nbkg, int nbins, double xlo, double xhi, const char* range, RooWorkspace* ws=0, bool verbose = false);

  double getParFinalValue(RooFitResult* fit, std::string parname);

  void printVal(const RooRealVar& var);
  double calcPoissonError(double mu);

  double getMinMaxDeviations(RooAbsPdf* pdf, RooAbsPdf* pdfb, RooAbsPdf* pdfc, RooRealVar* var, double normalization, int nbins, double *binsx);

  void getQuantiles(std::vector<double>& limits, double &median_, std::pair<double, double>& onesigma_, std::pair<double, double>& twosigma_);

}


#endif
