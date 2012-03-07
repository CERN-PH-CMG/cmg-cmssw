#include "StatTools/BayesianDijetFit/interface/DijetHelper.h"

#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooBinning.h"
#include "RooAbsReal.h"
#include "RooRandom.h"

#include "TH1.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "unistd.h"

#include "RooWorkspace.h"
#include "TFile.h"

using namespace RooFit;

RooDataHist* DijetHelper::makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double *binsx)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram",nbins, binsx);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedData(filename, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram", nbins, xlo, xhi);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedData(filename, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, TH1D* hist)
{
  // open a file
  ifstream is;
  is.open(filename.c_str(), ifstream::in);

  // fill the histogram
  while(is.good()) {
    double temp;
    is >> temp;
    if(!is.good()) break;
    hist->Fill(temp);
  }
  string sout("Data_Shapes.root");


  TFile * output = new TFile(sout.c_str(),"RECREATE");
  hist->Write();
  output->Close();

  // create a new binned data object
  RooDataHist *binnedData = new RooDataHist(objectname.c_str(),"binned data", RooArgSet(*var), Import(*hist, kFALSE));

  return binnedData;
}


RooDataHist* DijetHelper::makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double *binsx)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram",nbins, binsx);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedPseudoData(pdf, nevents, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi)
{
  // create the histogram
  TString hname=TString("h")+objectname.c_str();
  TH1D* dataHist=new TH1D(hname,"data histogram", nbins, xlo, xhi);

  // use the helper function to create binned data from histogram
  return DijetHelper::makeBinnedPseudoData(pdf, nevents, objectname, var, dataHist);
}

RooDataHist* DijetHelper::makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, TH1D* hist)
{
  // generate a pseudo dataset

  unsigned int seed1 = (unsigned int)getpid();
  unsigned int seed2 = (unsigned int)time(NULL);
  unsigned int randomSeed_ = seed1*31+seed2;
  RooRandom::randomGenerator()->SetSeed(randomSeed_);

  RooDataSet* dataset = pdf->generate(RooArgSet(*var), RooFit::NumEvents(nevents), RooFit::Name("tempdata"));

  // create a histogram out of the template data histogram and empty it
  if(hist->GetXaxis()->GetXbins()->GetArray()) {
    std::cerr << "You cannot use variable binning!!" << std::endl;
    assert(0);
  }
  RooBinning binning(hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
  var->setBinning(binning);
  hist = (TH1D*)dataset->fillHistogram(hist, RooArgList(*var));

  // create the new histogrammed data
  RooDataHist* datahist=new RooDataHist(objectname.c_str(), "binned pseudodata", RooArgSet(*var), RooFit::Import(*hist, kFALSE));

  // delete temporaries
  delete dataset;
  delete hist;

  return datahist;
}

RooDataSet* DijetHelper::makeUnbinnedData(std::string filename, std::string objectname, RooRealVar* var)
{
  // create the dataset
  RooDataSet* data = new RooDataSet(objectname.c_str(), "unbinned data", RooArgSet(*var));
  
  // fill the dataset
  ifstream is;
  is.open(filename.c_str(), ifstream::in);
  while(is.good()) {
    double temp;
    is >> temp;
    if(!is.good()) break;
    var->setVal(temp);
    data->add(RooArgSet(*var));
  }
  return data;
}

double DijetHelper::calcPDF1DIntegral(RooAbsPdf* pdf, RooRealVar* var, double min, double max)
{
  var->setRange("signal",min,max);
  RooAbsReal* pdfIntegral = pdf->createIntegral(*var, NormSet(*var), Range("signal"));
  return pdfIntegral->getVal();
}


RooFitResult* doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, RooAbsReal* nsig, RooAbsReal* nbkg, int nbins, double xlo, double xhi, const char* range, RooWorkspace* ws, bool verbose)
{
  double *binsx=new double[nbins+1];
  for(int i=0; i<nbins+1; i++) {
    binsx[i]=xlo+i*(xhi-xlo)/nbins;
  }
  RooFitResult* result=DijetHelper::doFit(name, pdf, data, var, nsig, nbkg, nbins, binsx, range, ws, verbose);
  delete[] binsx;
  return result;
}

RooFitResult* DijetHelper::doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, RooAbsReal* nsig, RooAbsReal* nbkg, int nbins, double *binsx, const char* range, RooWorkspace* ws, bool verbose)
{

  int printLevel = -1;
  if (verbose) printLevel = 1;

  TString label=name.c_str();
  RooFitResult *fit = pdf->fitTo(*data, Save(kTRUE), Extended(kTRUE), Strategy(2), PrintLevel(printLevel), RooFit::Range(range));
  if (verbose) fit->Print();


    // plot fit
    TCanvas* cfit = new TCanvas(TString("cfit")+label,"fit",500,500);
    RooPlot* plot = var->frame();
    data->plotOn(plot, Binning(RooBinning(nbins, binsx)));
    pdf->plotOn(plot, LineColor(kBlue+2), RooFit::NormRange(range), RooFit::Range(range));
    pdf->plotOn(plot, RooFit::Components("signal"), RooFit::LineColor(kBlue+2), RooFit::NormRange(range), RooFit::Range(range));
    pdf->plotOn(plot, RooFit::Components("background"), RooFit::LineColor(kBlue+2), RooFit::LineStyle(kDotted), RooFit::NormRange(range), RooFit::Range(range));
    pdf->paramOn(plot, Layout(0.43, 0.88, 0.92), Format("NEU",AutoPrecision(1)) ); 
    gPad->SetLogy();
    gPad->SetGrid(1,1);
    plot->GetYaxis()->SetRangeUser(0.1,plot->GetMaximum()*2.0);
    plot->GetYaxis()->SetTitleOffset(1.2);
    plot->Draw();


  if(ws) {
    std::cout << "ASDF JPC: ";
    printVal(*ws->var("xs"));
    TFile* rootfile=new TFile(TString("jpc_")+label+TString(".root"), "RECREATE");
    double p1val=ws->var("p1")->getVal(); double p1err=ws->var("p1")->getError(); bool p1const=ws->var("p1")->isConstant();
    double p2val=ws->var("p2")->getVal(); double p2err=ws->var("p2")->getError(); bool p2const=ws->var("p2")->isConstant();
    double p3val=ws->var("p3")->getVal(); double p3err=ws->var("p3")->getError(); bool p3const=ws->var("p3")->isConstant();
    ws->var("p1")->setVal(std::max(0.0,p1val-p1err));
    ws->var("p2")->setVal(p2val+p2err);
    ws->var("p3")->setVal(p3val+p3err);
    ws->var("p1")->setConstant(true);
    ws->var("p2")->setConstant(true);
    ws->var("p3")->setConstant(true);
    fit = pdf->fitTo(*data, Save(kTRUE), Extended(kTRUE), Strategy(2), PrintLevel(printLevel));

    std::cout << "ASDF JPC: ";
    printVal(*ws->var("xs"));
    pdf->plotOn(plot, LineColor(kGreen+2));
    pdf->plotOn(plot, RooFit::Components("signal"), RooFit::LineColor(kGreen+2));
    pdf->plotOn(plot, RooFit::Components("background"), RooFit::LineColor(kGreen+2), RooFit::LineStyle(kDotted));
    ws->var("p1")->setVal(std::max(0.0,p1val+p1err));
    ws->var("p2")->setVal(p2val-p2err);
    ws->var("p3")->setVal(p3val-p3err);
    fit = pdf->fitTo(*data, Save(kTRUE), Extended(kTRUE), Strategy(2), PrintLevel(printLevel));
    std::cout << "ASDF JPC: ";
    printVal(*ws->var("xs"));
    pdf->plotOn(plot, LineColor(kRed+2));
    pdf->plotOn(plot, RooFit::Components("signal"), RooFit::LineColor(kRed+2));
    pdf->plotOn(plot, RooFit::Components("background"), RooFit::LineColor(kRed+2), RooFit::LineStyle(kDotted));
    rootfile->cd();
    plot->Write();
    rootfile->Close();

    // set everything back
    ws->var("p1")->setVal(p1val); ws->var("p1")->setConstant(p1const);
    ws->var("p2")->setVal(p2val); ws->var("p2")->setConstant(p2const);
    ws->var("p3")->setVal(p3val); ws->var("p3")->setConstant(p3const);
  }

  if (verbose) cfit->SaveAs(label+".gif");
  delete plot;

  // calculate pull and diff
  RooArgSet observables(*var);
  TH1* hist=data->createHistogram(TString("dataHist")+name,*var,Binning(RooBinning(nbins,binsx)));
  TH1D* pull=(TH1D*)hist->Clone(TString("pull")+name);
  TH1D* diff=(TH1D*)hist->Clone(TString("diff")+name);
  double chi2=0;
  for(int i=1; i<=hist->GetNbinsX(); i++) {
    double content = hist->GetBinContent(i);
    double error = content>25 ? sqrt(content) :
      (TMath::ChisquareQuantile(0.95,2*(content+1)-TMath::ChisquareQuantile(0.05,2*content)))/4.0;
    
    // evaluate the pdf
    var->setVal(hist->GetBinCenter(i));
    double fitval = pdf->getVal(&observables)*(nsig->getVal()+nbkg->getVal())*hist->GetBinWidth(i);
    
    // calculate the pull and diff
    double pullval=error==0 ? 0 : (content-fitval)/error;
    double diffval=(content-fitval)/fitval;
    double differr=error/fitval;
    chi2+=fabs(pullval);
    
    pull->SetBinContent(i, pullval);
    pull->SetBinError(i, 1.0);
    diff->SetBinContent(i, diffval);
    diff->SetBinError(i, differr);
  }
    
  // draw pull
  TCanvas* cpull = new TCanvas(TString("cpull")+label,"pull",500,500);
  pull->SetMarkerStyle(20);
  pull->Draw("E");
  pull->GetYaxis()->SetTitle("Pull");
  pull->SetMaximum(4.0);
  pull->SetMinimum(-4.0);
  pull->GetYaxis()->SetRangeUser(-4.,4.);
  char title[100];
  sprintf(title, "#chi^{2} = %2.2f", chi2);
  TLatex tex;
  tex.SetNDC();
  tex.SetTextAlign(12);
  tex.SetTextSize(0.04);
  tex.DrawLatex(0.22,0.88, title);
  if (verbose) cpull->SaveAs(label+"_pull.gif");
  
  // draw diff
  TCanvas* cdiff = new TCanvas(TString("cdiff")+label,"(data-fit)/fit",500,500);
  diff->SetMarkerStyle(20);
  diff->GetYaxis()->SetTitle("(data-fit)/fit");
  diff->SetMaximum(0.3);
  diff->SetMinimum(-0.3);
  diff->Draw("E");
  tex.DrawLatex(0.22,0.88, title);
  if (verbose) cdiff->SaveAs(label+"_diff.gif");
  

  return fit;
}


double DijetHelper::getParFinalValue(RooFitResult* fit, std::string parname)
{
  // find a parameter
  RooArgList args=fit->floatParsFinal();
  for(int i=0; i<args.getSize(); i++) {
    TString name(args.at(i)->GetName());
    std::cout << "Found name=" << name << std::endl;
    if(name.CompareTo(parname.c_str())==0) {
      RooRealVar* v=dynamic_cast<RooRealVar*>(args.at(i));
      return (v->getVal());
    }
  }
  std::cout << "Could not find parameter " << parname << std::endl;
  return 0.0;
}
 
void DijetHelper::printVal(const RooRealVar& var)
{
  std::cout << var.GetName() << "=" << var.getVal() << "+/-" << var.getError()
	    << " from " << var.getMin() << " to " << var.getMax() 
	    << "; isConst=" << var.isConstant() << std::endl;
  return;
}

double DijetHelper::calcPoissonError(double mu)
{
  double error=sqrt(mu);
  if(mu<25)
    error=(TMath::ChisquareQuantile(0.95,2*(mu+1)-TMath::ChisquareQuantile(0.05,2*mu)))/4.0;
  return error;
}

double DijetHelper::getMinMaxDeviations(RooAbsPdf* pdf, RooAbsPdf* pdfb, RooAbsPdf* pdfc, RooRealVar* var, double normalization, int nbins, double *binsx)
{
  double result=0.0;
  double maxdiff=0.0;
  for(int i=0; i<nbins; i++) {
    RooArgSet s(*var);
    double val=normalization*DijetHelper::calcPDF1DIntegral(pdf, var, binsx[i], binsx[i+1]);
    double valb=normalization*DijetHelper::calcPDF1DIntegral(pdfb, var, binsx[i], binsx[i+1]);
    double valc=normalization*DijetHelper::calcPDF1DIntegral(pdfc, var, binsx[i], binsx[i+1]);
    double diffb=fabs(val-valb)/val;
    double diffc=fabs(val-valc)/val;
    if(diffb>maxdiff) maxdiff=diffb;
    if(diffc>maxdiff) maxdiff=diffc;
    double relerror=DijetHelper::calcPoissonError(val)/val;

    printf("%3d %7.1f %13.5f %13.5f %13.5f %11.5f %11.5f %11.5f %11.5f\n", i, binsx[i], val, valb, valc, diffb, diffc, maxdiff, relerror);
    if(maxdiff>relerror && maxdiff>result) result=maxdiff;
  }
  return result;
}



void DijetHelper::getQuantiles(std::vector<double>& limits, double &median_, std::pair<double, double>& onesigma_, std::pair<double, double>& twosigma_) {
  unsigned int nit=limits.size();
  if(nit==0) return;

  // sort the vector with limits
  std::sort(limits.begin(), limits.end());

  // median for the expected limit
  median_ = TMath::Median(nit, &limits[0]);

  // quantiles for the expected limit bands
  double prob[4]; // array with quantile boundaries
  prob[0] = 0.021;
  prob[1] = 0.159;
  prob[2] = 0.841;
  prob[3] = 0.979;

  // array for the results
  double quantiles[4];
  TMath::Quantiles(nit, 4, &limits[0], quantiles, prob); // evaluate quantiles
  onesigma_.first=quantiles[1];
  onesigma_.second=quantiles[2];
  twosigma_.first=quantiles[0];
  twosigma_.second=quantiles[3];
    
  return;
}
