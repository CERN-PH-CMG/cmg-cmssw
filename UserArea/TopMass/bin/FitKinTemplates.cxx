#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooGenericPdf.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooWorkspace.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooFit.h"
#include "Riostream.h"
#include "RooArgList.h"
#include "RooRandom.h"
#include "RooMinuit.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "TProfile.h"
#include "TPaveText.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TIterator.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace RooFit;

//
void showPseudoExperimentResult(std::map<std::string,TH1 *> &results,std::map<std::string,float> & masses, TString observable,TString outputName);

//
int main(int argc, char *argv[])
{
  // load framework libraries 
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //keep RooFit quiet
  RooMsgService::instance().setSilentMode(true);
  RooMsgService::instance().getStream(0).removeTopic(Minimization);
  RooMsgService::instance().getStream(1).removeTopic(Minimization);
  RooMsgService::instance().getStream(1).removeTopic(Fitting);
  RooMsgService::instance().getStream(1).removeTopic(Eval);
  RooMsgService::instance().getStream(1).removeTopic(Integration);
  RooMsgService::instance().getStream(1).removeTopic(NumIntegration);
  
  // initialize command line parser                  
  optutl::CommandLineParser parser ("[FitKinTemplates]");
  parser.addOption("templ",   optutl::CommandLineParser::kString, "input file with templates");
  //parser.addOption("data",       optutl::CommandLineParser::kString, "input file with data");
  parser.addOption("onlySignal",   optutl::CommandLineParser::kBool, "activate to generate pseudo-experiments with signal only");
  parser.addOption("npe",   optutl::CommandLineParser::kInteger, "number of pseudo-experiments per mass point");
  parser.boolValue("onlySignal") = false;
  parser.integerValue("npe")     = 10;
  parser.parseArguments (argc, argv);
  std::string templateInputFile = parser.stringValue("templ");
  //std::string dataInputFile = parser.stringValue("data");
  bool calibrateOnSignalOnly    = parser.boolValue("onlySignal");
  int number_pseudo_experiments = parser.integerValue("npe"); 

  //
  // READ HISTOGRAMS FROM INPUT FILE AND DEFINE CATEGORIES
  //
  cout << "[FitKinTemplates] templates will be retrieved from " << templateInputFile << endl;
  RooCategory templateCategories("categ","categ") ;
  RooCategory templateCategories2("categbkg","categbkg") ; 
  RooCategory templateCategories3("categbs","categbs") ; 
  std::map<std::string,float> masses;
  std::map<std::string,TH1 *> templates;
  TH1 *bckgTemplate=0;
  TFile *fIn=TFile::Open(templateInputFile.c_str()); 
  TIter nextObj(fIn->GetListOfKeys()); 
  TObject *obj=nextObj();
  while(obj!=0) 
  {
    TString key(obj->GetName());
    TH1F *h=(TH1F *)fIn->Get(key)->Clone();  
    h->SetDirectory(0);

    TObjArray *tkns=key.Tokenize("_");
    TString ch      = tkns->At(0)->GetName();
    TString var     = tkns->At(1)->GetName();
    TString proc    = tkns->At(2)->GetName();
    TString massStr = tkns->At(3)->GetName(); 
    float mass=massStr.Atof()/10.;
    if (mass>10){
      templateCategories.defineType(massStr.Data());
      templates[massStr.Data()]=h;
      masses[massStr.Data()]=mass;
    }
    else if (mass==0) { //background alone 
      //clone the histogram or add to the existing one in case of background processes
      if(bckgTemplate==0) { bckgTemplate=(TH1 *)h->Clone("bckgtempl");  bckgTemplate->SetDirectory(0); }
      else                  bckgTemplate->Add(h);
    }
        
    obj=nextObj();
  }
  fIn->Close();
  
  //
  // PREPARE THE COMBINED FIT OF THE TEMPLATE
  //

  //define the dataset
  RooRealVar pt("pt","p_{T}",0,300) ;
  RooRealVar mtop("mtop","m_{top}",173.5,100,200);
  RooDataHist combData("combData","combData",RooArgSet(pt),templateCategories,templates);
  combData.Print();
  
  RooDataHist bckgData("bckgData","bckgData",RooArgSet(pt),Import(*bckgTemplate));
  bckgData.Print();
  
  //lepton-pT base parameterization according to arXiv:1101.4926v2 [hep-ex]
  RooRealVar alpha1("alpha1","#alpha_{1}",1.72,  0,      3); 
  RooRealVar alpha2("alpha2","#alpha_{2}",0,     0.,   0.1);
  RooRealVar alpha3("alpha3","#alpha_{3}",6.2,   0.,      10);
  RooRealVar alpha4("alpha4","#alpha_{4}",0.079, 0.,  0.1);
  RooFormulaVar pfunc("pfunc","p","@0+@1*@2",RooArgSet(alpha1,alpha2,mtop)); 
  RooFormulaVar qfunc("qfunc","q","@0+@1*@2",RooArgSet(alpha3,alpha4,mtop));
  //these parameters are fixed in the CDF paper but a check is made for the stability of the result
  //when pt_wid is varied (pt_thr is the selection threshold to give the rise in the Fermi function part of the pdf) 
  RooRealVar pt_thr("pt_thr","p_{T}^{min}",26);//,26*0.98,26*1.02);
  RooRealVar pt_wid("pt_wid","#delta p_{T}",0.019);//2,0,5);//0.007385 //0.0186 //Fixed value for the background

  TString F_form(  "(1.0/qfunc)*(1.0/TMath::Gamma(1+pfunc,pt_thr/qfunc))*pow(pt/qfunc,pfunc)*(exp(-pt/qfunc)/(1+TMath::Exp((pt_thr-pt)/pt_wid)))"); 
  F_form.ReplaceAll("pt_thr", "@0");
  F_form.ReplaceAll("pt_wid", "@1");
  F_form.ReplaceAll("pt",     "@2");
  F_form.ReplaceAll("pfunc",  "@3");
  F_form.ReplaceAll("qfunc",  "@4");
  RooGenericPdf F_pT("fpt","",F_form.Data(),RooArgSet(pt_thr,pt_wid,pt,pfunc,qfunc)); 

  //use the base PDF and convert to a simultaneous PDF 
  RooWorkspace* w = new RooWorkspace("w") ;
  w->import(RooArgSet(mtop,templateCategories));
  w->import(F_pT);
  w->factory("SIMCLONE::fpt_sim( fpt, $SplitParam({mtop},categ) )") ;
  for(std::map<string,float>::iterator mIt = masses.begin(); mIt != masses.end(); mIt++)
    {
      string ikey="mtop_"+mIt->first; 
      RooRealVar *imtop=w->var(ikey.c_str());
      float imass=mIt->second;
      imtop->setVal(imass);
      imtop->setConstant(kTRUE);
    }
  w->import(combData);
  w->factory("EDIT::fpt_bkg(fpt,pfunc=beta1[1.72,0.,100.],qfunc=beta2[5.2,0.,100.],pt_thr=pt_thr_bckg[26],pt_wid=pt_wid_bkg[3])"); //for the background we use fixed parameters
  w->Print();
  cout<<"workspace*************************************************"<<endl;
     
  //
  // FIT THE TEMPLATES
  //
  RooFitResult *fitres = w->pdf("fpt_sim")->fitTo(combData,Range(20,200),Save(kTRUE),SumW2Error(kTRUE));
  cout<<"Signal fit**************************************************************"<<endl;
  RooFitResult *fitres2 = w->pdf("fpt_bkg")->fitTo(bckgData,Save(kTRUE),Range(20,200),SumW2Error(kTRUE));
  cout<<"Bckg fit****************************************************************"<<endl;

  //
  // PLOT THE RESULTS
  //
  int ncats=templates.size();
  int npadsx=ncats/2+1;
  int npadsy=ncats/2+1;
  while(npadsx*npadsy>ncats+2) npadsy--;
  TCanvas *cPDF=new TCanvas("cPDF","cPDF",npadsx*400,npadsy*400);
  cPDF->Divide(npadsx,npadsy);
  int ipad(1);
  TGraphErrors *gr=new TGraphErrors; gr->SetName("meanevol");
  for(std::map<string,float>::iterator mIt=masses.begin(); mIt != masses.end(); mIt++,ipad++)
    {
      cPDF->cd(ipad);
      string key=mIt->first;
      float imass=mIt->second;

      char title[200]; sprintf(title,"m_{top}=%3.2f GeV",imass);
      RooPlot* frame = pt.frame(Title(title));

      //project the dataset for a given category
      RooDataHist* dataslice = (RooDataHist *)combData.reduce(("categ==categ::"+key).c_str());
      dataslice->plotOn(frame,DataError(RooAbsData::SumW2));
      Int_t np=gr->GetN();

      gr->SetPoint(np,imass,templates[key]->GetMean());
      gr->SetPointError(np,0,templates[key]->GetMeanError());

      //get the PDF corresponding to the required category
      RooCategory theCategory(key.c_str(),key.c_str());
      w->pdf("fpt_sim")->plotOn(frame,Slice(theCategory),ProjWData(pt,*dataslice));
      
      frame->Draw();
      frame->GetXaxis()->SetTitle("p_{T} [GeV]");
      frame->GetYaxis()->SetTitle("Events");
      frame->SetMinimum(0);
    }

  //print the fit in the first canvas
  cPDF->cd(1);
  fitres->Print("v");
  TPaveText *pave = new TPaveText(0.5,0.5,0.88,0.88,"NDC");
  pave->AddText("Fit results");
  pave->AddText("");
  RooArgSet varSet=w->allVars();
  TIterator *iter=varSet.createIterator();
  RooRealVar *var;
  while((var=(RooRealVar*)iter->Next())) {
    if(var->getError()==0) continue;
    TString varName(var->GetName()); 
    if(varName.Contains("mtop") || varName.Contains("beta")) continue;
    char line[100];
    sprintf(line,"%s=%3.3f #pm %3.3f", var->GetTitle(),var->getVal(),var->getError());
    pave->AddText(line); 
  }
  pave->SetBorderSize(0);
  pave->SetFillStyle(0);
  pave->SetTextFont(42);
  pave->Draw();

  //show the profile with a fit
  cPDF->cd(ipad);
  gr->SetMarkerStyle(20);
  gr->Draw("ap");
  gr->Fit("pol1");
  gr->GetXaxis()->SetTitle("m_{top} [GeV]");
  gr->GetXaxis()->SetTitle("<p_{T}> [GeV]");
  TF1 *avgPtEvol=gr->GetFunction("pol1");
  pave = new TPaveText(0.1,0.6,0.5,0.88,"NDC");
  pave->AddText("<p_{T}> parameterization");
  char line[200];
  sprintf(line,"b=%3.3f #pm %3.3f",avgPtEvol->GetParameter(0),avgPtEvol->GetParError(0)); pave->AddText(line);
  sprintf(line,"m=%3.3f #pm %3.3f",avgPtEvol->GetParameter(1),avgPtEvol->GetParError(1)); pave->AddText(line);
  pave->SetBorderSize(0);
  pave->SetFillStyle(0);
  pave->SetTextFont(42);
  pave->Draw();
  cPDF->SaveAs("SignalPDFs.png");  

  //show the background fit
  TCanvas *cPDF2=new TCanvas("cPDF2","cPDF2",400,400);
  char title2[200]; sprintf(title2,"Background");
  RooPlot* frame2 = pt.frame(Title(title2));
  bckgData.plotOn(frame2,DataError(RooAbsData::SumW2));
  w->pdf("fpt_bkg")->plotOn(frame2,ProjWData(pt,bckgData));
  frame2->Draw();
  frame2->GetXaxis()->SetTitle("p_{T} [GeV]");
  frame2->GetYaxis()->SetTitle("Events");
  frame2->SetMinimum(0);
  fitres2->Print("v");
  TPaveText *pave2 = new TPaveText(.55,.75,.9,.88,"brNDC");
  pave2->AddText("Fit results");
  iter=varSet.createIterator();
  while((var=(RooRealVar*)iter->Next())) {
    if(var->getError()==0) continue;
    TString varName(var->GetName()); 
    if(!varName.Contains("beta") && !varName.Contains("pt_wid")) continue;
    sprintf(line,"%s=%3.3f #pm %3.3f", var->GetTitle(),var->getVal(),var->getError());
    pave2->AddText(line); 
  }
  pave2->SetBorderSize(0);
  pave2->SetFillStyle(0);
  pave2->SetTextFont(42);
  pave2->Draw();
  cPDF2->SaveAs("BackgroundPDF.png");

  //
  // NOW PREPARE THE PDF WITH THE FIXED PARAMETERS
  //

  char buf[1000];

  //signal component
  sprintf(buf,"EDIT::fpt_data(fpt,alpha1=a1[%f],alpha2=a2[%f],alpha3=a3[%f],alpha4=a4[%f])",
	  w->var("alpha1")->getVal(),
	  w->var("alpha2")->getVal(),
	  w->var("alpha3")->getVal(),
	  w->var("alpha4")->getVal());
  w->factory(buf);

  //bckg component
  sprintf(buf,"EDIT::bckg_data(fpt_bkg,beta1=b1[%f],beta2=b2[%f])",
	  w->var("beta1")->getVal(),
	  w->var("beta2")->getVal());
  w->factory(buf);
  w->Print();
  //build the sum of background + signal <-  is there a way that this is not hardcoded  
  float nsig_expected  = 13366.0;    
  float ndata_obs      = 16196.0; 
  float nbckg_expected = 2830.0; 
  Double_t Nbckg_MCerr(0);
  Double_t        Nbckg       =  bckgTemplate->IntegralAndError(1,bckgTemplate->GetXaxis()->GetNbins(),Nbckg_MCerr);
  Double_t nbckg_expected_err = sqrt( pow(Nbckg_MCerr,2) + pow( Nbckg*0.30,2) );
  sprintf(buf,"SUM::F_pT_model( Nsig[%f,0,%f]*fpt_data, Nbkg[%f,0,%f]*bckg_data  )", nsig_expected, ndata_obs, nbckg_expected, ndata_obs);
  w->factory(buf) ;

  //create a constraint for the background if your expectation is  nbckg_expected +/- nbckg_expected_err
  sprintf(buf,"Gaussian::bckgContr( Nbkg, mu_bckg[%f], sigma_bckg[%f])", nbckg_expected, nbckg_expected_err);
  w->factory(buf);
  
  // PDF multiplication : the gaussian will be a penalty for cases where the background yields are fluctuating to high or too low
  sprintf(buf,"PROD::F_pT_constr_model(F_pT_model,bckgContr)");
  w->factory(buf);

  //
  // PSEUDO-EXPERIMENTS
  //
  //loop over the different top masses
  (RooRandom::randomGenerator())->SetSeed(0); //not sure why you need this call Cecile
  int pa=0;
  TProfile *pePtProf=new TProfile("hpeptprof","PE control;Generated m_{top} [GeV];<p_{T}>/[GeV] in pseudo-exp.",32,164,180);
  std::map<std::string, TH1 *> mtopMeanH, biasMeanH, mtopShapeH, biasShapeH, uncShapeH, pullShapeH;
  cout << "Starting pseudo-experiments" << endl;
  for(std::map<std::string, TH1 *>::iterator mIt = templates.begin(); mIt != templates.end(); mIt++,pa++)
    {
      cout << mIt->first <<  " with " << number_pseudo_experiments << " experiments" << endl;

      //generate s+b base distribution
      TH1 *spb=(TH1*) mIt->second->Clone("spbh");
      if(!calibrateOnSignalOnly) spb->Add(bckgTemplate);
      float genMass = masses[mIt->first];
      RooDataSet ds("pedata","Pseudo-data",RooArgSet(pt));
      
      //book histograms to control top mass fit
      char title[200]; sprintf(title,"m_{top}=%3.2f GeV",genMass);
      mtopMeanH[mIt->first]  = new TH1F(("mtopmean"+mIt->first).c_str(),  title+TString(";Fitted m_{top} [GeV];Pseudo-experiments"), 100,150,200);
      biasMeanH[mIt->first]  = new TH1F(("biasmean"+mIt->first).c_str(),  title+TString(";Bias [GeV];Pseudo-experiments"),           100,-10,10);
      mtopShapeH[mIt->first] = new TH1F(("mtopshape"+mIt->first).c_str(), title+TString(";Fitted m_{top} [GeV];Pseudo-experiments"), 100,150,200);
      biasShapeH[mIt->first] = new TH1F(("biasshape"+mIt->first).c_str(), title+TString(";Bias [GeV];Pseudo-experiments"),           100,-10,10);
      uncShapeH[mIt->first]  = new TH1F(("uncshape"+mIt->first).c_str(),  title+TString(";#sigma_{stat} [GeV];Pseudo-experiments"),  100,1,2);
      pullShapeH[mIt->first] = new TH1F(("pullshape"+mIt->first).c_str(), title+TString(";Pull ;Pseudo-experiments"),                100,-5,5);

      //generate pseudo-experiments from the s+b base distribution
      for(int pe=0; pe<number_pseudo_experiments; pe++)
      {
         ds.reset();

         //generate as many leptons as you observe in data
         for(int ev=0; ev<ndata_obs; ev++)
         {
            pt.setVal(spb->GetRandom());
            ds.add(RooArgSet(pt));
	 }
	 
	 // MEAN METHOD: derive top mass from the average
	 float avgPt=ds.mean(pt);
	 pePtProf->Fill(genMass,avgPt,1);
	 float massFromMean=avgPtEvol->GetX(avgPt,100,200);
	 mtopMeanH[mIt->first]->Fill(massFromMean);
	 biasMeanH[mIt->first]->Fill(massFromMean-genMass);
	 
         // FIT METHOD
	 w->var("mtop")->setVal(genMass);
	 if(!calibrateOnSignalOnly) w->pdf("F_pT_constr_model")->fitTo(ds,Range(20,200),Extended(kTRUE),Save(kTRUE));
         else                       w->pdf("fpt_data")         ->fitTo(ds,Range(20,200),                Save(kTRUE)); 
	 float massFromFit=w->var("mtop")->getVal();
	 float uncOnMassFromFit=w->var("mtop")->getError();
	 mtopShapeH[mIt->first]->Fill(massFromFit);
	 biasShapeH[mIt->first]->Fill(massFromFit-genMass);
	 pullShapeH[mIt->first]->Fill((massFromFit-genMass)/uncOnMassFromFit);
	 uncShapeH[mIt->first]->Fill(uncOnMassFromFit);
	 
	 //use this to debug the result of a fit
	 //	 if(pe==0)
	 //{
	 //    TCanvas *C=new TCanvas("C","C",600,600);
	 //    RooPlot* frame = pt.frame();
	 // 	     ds.plotOn(frame);
	 // 	     w->pdf("fpt2")->plotOn(frame);
	 // 	     frame->Draw();
	 // 	     frame->GetXaxis()->SetTitle("p_{T} [GeV]");
	 // 	     frame->GetYaxis()->SetTitle("Events");
	 // 	     frame->SetMinimum(0);
	 // 	     TString outName("fitres"); outName += mIt->first; outName+=pe; outName+=".png";
	 // 	     C->SaveAs(outName);
	 //  }
      }
  }  
  
  //control of the quality of PE's
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  TCanvas *cPEControl=new TCanvas("cPEControl","cPEControl",600,600);
  pePtProf->SetMarkerStyle(20);
  pePtProf->GetYaxis()->SetRangeUser(65,70);
  pePtProf->Draw();
  avgPtEvol->Draw("same");
  cPEControl->Modified();
  cPEControl->Update();
  cPEControl->SaveAs("PEControl.png");

  //show the results
  showPseudoExperimentResult(mtopMeanH,  masses, "Fitted m_{top} [GeV]",    "wmtopmean");
  showPseudoExperimentResult(biasMeanH,  masses, "Bias [GeV]",              "wbiasmean");
  showPseudoExperimentResult(mtopShapeH, masses, "Fitted m_{top} [GeV]",    "wmtopshape");
  showPseudoExperimentResult(biasShapeH, masses, "Bias [GeV]",              "wbiasshape");
  showPseudoExperimentResult(uncShapeH,  masses, "Stat. uncertainty [GeV]", "wuncshape");
  showPseudoExperimentResult(pullShapeH, masses, "Pull",                    "wpullshape");
}

//
void showPseudoExperimentResult(std::map<std::string,TH1 *> &results,std::map<std::string,float> & masses, TString observable,TString outputName)
{
  //built the canvas
  int ncats=results.size();
  if(ncats==0) return;
  int npads=ncats/2+1;
  TCanvas *cPDF=new TCanvas("cpe","cpe",npads*400,npads*400);
  cPDF->Divide(npads,npads);
  TGraphErrors *grMeanErr=new TGraphErrors;  grMeanErr->SetName("meangr");
  TGraphErrors *grWidthErr=new TGraphErrors; grWidthErr->SetName("widthgr");
  int pa=1;
  for(std::map<std::string,TH1 *>::iterator resIt=results.begin(); resIt!=results.end(); resIt++,pa++)
    {
      //plot the histogram and fit a gaussian to it
      cPDF->cd(pa);
      resIt->second->Draw("hist");
      resIt->second->Fit("gaus");

      TF1 *gausFit=resIt->second->GetFunction("gaus");
      if(gausFit==0) continue;
      int np=grMeanErr->GetN();
      grMeanErr->SetPoint(np,masses[resIt->first],gausFit->GetParameter(1));
      grMeanErr->SetPointError(np,0,gausFit->GetParError(1));
      grWidthErr->SetPoint(np,masses[resIt->first],gausFit->GetParameter(2));
      grWidthErr->SetPointError(np,0,gausFit->GetParError(2));
    }

  //show a summary of the evolution of the mean and width of the observable as function of the top mass
  TPad *p=(TPad *)cPDF->cd(pa);
  p->Divide(1,2);
  p->cd(1);
  grMeanErr->Draw("ap");
  grMeanErr->GetXaxis()->SetTitle("Generated m_{top} [GeV]");
  grMeanErr->GetYaxis()->SetTitle("<"+observable+">");
  grMeanErr->SetMarkerStyle(kOpenCircle);
  grMeanErr->SetMarkerColor(kBlue);
  p->cd(2);
  grWidthErr->Draw("ap");
  grWidthErr->GetXaxis()->SetTitle("Generated m_{top} [GeV]");
  grWidthErr->GetYaxis()->SetTitle("#sigma "+observable);
  grWidthErr->SetMarkerStyle(kOpenCircle);
  grWidthErr->SetMarkerColor(kBlue);
  cPDF->SaveAs(outputName+".png");
}

