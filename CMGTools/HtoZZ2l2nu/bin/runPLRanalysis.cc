#include <iostream>
#include <iomanip>

#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/ProfileInspector.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/SamplingDistPlot.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/RooStatsUtils.h"

#include "RooWorkspace.h"
#include "RooMinuit.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooNLLVar.h"

using namespace std;
using namespace RooFit;
using namespace RooStats;

std::vector<TString> inFilesUrl;
TString wsName="w";
TString dataName="data_obs";
TString mcName="ModelConfig";
Float_t iEcm=8.0;
Float_t iLumi=5041;
Float_t cl=0.68;

struct FitResults_t
{
  float r,rmin,rmax;
  float totalUnc, statUnc;
  std::map<TString,float> uncList;
};

void showPLR(std::vector<std::pair<TString,TGraph *> > &plotList, TString fout, TString header, float rmin, float rmax);

//
int main(int argc, char *argv[])
{
  
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg.find("--in")!=string::npos && i+1<argc) 
      { 
	TString csvInFileUrl=argv[i+1]; i++; 
	TObjArray *tkns=csvInFileUrl.Tokenize(",");
	cout << "List of files to analyse: " << endl;
	for(int itkn=0; itkn<tkns->GetEntriesFast(); itkn++) 
	  {
	    TString ifname(tkns->At(itkn)->GetName());
	    cout << "\t" << ifname << flush;
	    if(ifname.EndsWith(".dat")) { 
	      cout << " -> converting workspace (dat) to ROOT file (root)" << flush;
	      TString shCommand("text2workspace.py " + ifname + " -o ");
	      ifname.ReplaceAll(".dat",".root");
	      shCommand += ifname;
	      gSystem->Exec(shCommand.Data());
	    }
	    cout << endl;
	    inFilesUrl.push_back( ifname );
	  }
	delete tkns;
      }	  
    else if(arg.find("--ws")!=string::npos && i+1<argc)   { wsName=argv[i+1];              i++; cout << "workspace: " << wsName << endl; }
    else if(arg.find("--data")!=string::npos && i+1<argc) { dataName=argv[i+1];            i++; cout << "data: " << dataName << endl; }
    else if(arg.find("--mc")!=string::npos && i+1<argc)   { mcName=argv[i+1];              i++; cout << "model config: " << mcName << endl; }
    else if(arg.find("--cl")!=string::npos && i+1<argc)   { sscanf(argv[i+1],"%f",&cl);    i++; cout << "C.L.: " << cl << endl; }
    else if(arg.find("--ecm")!=string::npos && i+1<argc)  { sscanf(argv[i+1],"%f",&iEcm);  i++; cout << "Ecm: " << iEcm << endl; }
    else if(arg.find("--lumi")!=string::npos && i+1<argc) { sscanf(argv[i+1],"%f",&iLumi); i++; cout << "Lumi: " << iLumi << endl; }
  }
  
  setStyle();
  std::map<TString,FitResults_t> finalResults;
  std::vector<std::pair<TString,TGraph *> > finalPlotList;
  float finalRmin(0), finalRmax(0);
  for(size_t ifile=0; ifile<inFilesUrl.size(); ifile++)
    {
      TFile *inF=TFile::Open(inFilesUrl[ifile]);
      if(inF->IsZombie()) continue;

      TString chTag("inclusive");
      if(inFilesUrl[ifile].Contains("_ee"))          chTag="ee";
      if(inFilesUrl[ifile].Contains("_emu"))         chTag="emu";
      if(inFilesUrl[ifile].Contains("_mumu"))        chTag="mumu";
      if(inFilesUrl[ifile].Contains("_eq0jets"))     chTag+=" (=0 jets)";
      if(inFilesUrl[ifile].Contains("geq1jets"))     chTag+=" (#geq 1 jets)";
      else if(inFilesUrl[ifile].Contains("eq1jets")) chTag+=" (=1 jets)";
      if(inFilesUrl[ifile].Contains("geq2jets"))     chTag+=" (#geq 2 jets)";
      else if(inFilesUrl[ifile].Contains("eq2jets")) chTag+=" (=2 jets)";
      if(inFilesUrl[ifile].Contains("eq3jets"))      chTag+=" (=3 jets)";
      if(inFilesUrl[ifile].Contains("eq4jets"))      chTag+=" (=4 jets)";
      if(inFilesUrl[ifile].Contains("vbf"))          chTag+=" (VBF)";
	
      //check the workspace
      RooWorkspace *w  =(RooWorkspace *)inF->Get(wsName); //  w->Print();
      if(w==0)    { cout << "[Error] workspace : " << wsName << " is not found in " << inFilesUrl[ifile] << endl;  continue; }
      if(w->set("observables")==0 || w->set("POI")==0 || w->set("nuisances")==0 || w->set("globalObservables")==0 ) { cout << "[Error] workspace " << wsName << " is missing observables or POI or nuisances" << endl; continue; }
      RooAbsData *data = w->data(dataName);     
      if(data==0) { cout << "[Error] data " << dataName << " is not found in " << inFilesUrl[ifile] << endl << " workspace contents follow ..." << endl;  w->Print(); continue; }
      ModelConfig* mc = (ModelConfig*) w->obj(mcName);
      if(mc==0)   { cout << "[Error] ModelConfig " << mcName << " is not found in workspace for " << inFilesUrl[ifile] << endl << " workspace contents follow ..." << endl;  w->Print(); continue; }
      
      //check which nuisances are available
      RooArgSet *nuis_params     = (RooArgSet*) w->set("nuisances");
      TIterator *nuis_params_itr = nuis_params->createIterator();
      TObject *nuis_params_obj;
      std::vector<TString> nuisNames;
      while((nuis_params_obj=nuis_params_itr->Next())){
	RooRealVar *nVar=(RooRealVar *)nuis_params_obj;
	nuisNames.push_back(nVar->GetName());
      }
      inF->Close();
      

      //loop over the nuisances: -1 is the inclusive fit, 0 is stat only
      FitResults_t fr;
      std::vector<std::pair<TString,float> > subCategories;
      std::vector<std::pair<TString,TGraph *> > plotList;
      float statUnc(0);
      RooArgSet *profNuis=0;
      for(int in=-1; in<=0*(int)nuisNames.size(); in++)
	{
	  TFile *inF=TFile::Open(inFilesUrl[ifile]);
	  RooWorkspace *w  =(RooWorkspace *)inF->Get(wsName); 
	  RooAbsData *data = w->data(dataName);     
	  ModelConfig* mc = (ModelConfig*) w->obj(mcName);
	  mc->SetObservables(*w->set("observables"));
	  mc->SetParametersOfInterest(*w->set("POI")); 
	  mc->SetNuisanceParameters(*w->set("nuisances"));
	  mc->SetGlobalObservables(*w->set("globalObservables")); 

	  TString test("total");
	  if(in>=0)
	    {
	      if(in==0) test="stat";
	      else      test=nuisNames[in-1];

	      //fix all nuisances except the one being tested
	      TIterator *nuis_params_itr = mc->GetNuisanceParameters()->createIterator();
	      TObject *nuis_params_obj;
	      while((nuis_params_obj=nuis_params_itr->Next())){
		RooRealVar *nVar=(RooRealVar *)nuis_params_obj;
		if(profNuis)
		  {
		    RooRealVar *fitNuis=(RooRealVar *)profNuis->find(nVar->GetName());
		    if(fitNuis && !isnan(float(fitNuis->getVal()))) nVar->setVal(fitNuis->getVal()); 
		    else                                            nVar->setVal(0);
		  }
		if(in==0)                                 nVar->setConstant(kTRUE);
		else if(nuisNames[in-1]!=nVar->GetName()) nVar->setConstant(kTRUE);
	      }
	    }
	  
	  //the following is based on http://root.cern.ch/root/html/tutorials/roostats/StandardProfileLikelihoodDemo.C.html
	  ProfileLikelihoodCalculator pl(*data,*mc);
	  pl.SetConfidenceLevel(cl); 	
	  LikelihoodInterval* interval = pl.GetInterval();
	  if(in<=0) profNuis=(RooArgSet *) mc->GetNuisanceParameters()->snapshot();
	  RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->first();
	  float ir=firstPOI->getVal(); 
	  float irmin=interval->LowerLimit(*firstPOI); 
	  float irmax=interval->UpperLimit(*firstPOI);
	  //	  float relErr=(irmax-irmin)*0.5/ir*100;
	  float relErr=firstPOI->getError()/ir;
	  if(irmin==irmax) { irmin=(1-relErr)*ir; irmax=(1+relErr)*ir; }

	  if(in==0)     statUnc=relErr;
	  else if(in>0) relErr=sqrt(pow(relErr,2)-pow(statUnc,2));

	  if(in<0)      { 
	    fr.r=ir;  fr.rmin=irmin; fr.rmax=irmax; fr.totalUnc=relErr; 

	    //signal significance from null hypothesis
	    /*
	      RooArgSet* nullParams = new RooArgSet("nullParams");
	      nullParams->addClone(firstPOI);
	      nullParams->setRealValue(firstPOI->getName(),0);  // set the value of the parameter for the null hypothesis
	      ProfileLikelihoodCalculator plCalc3(data,modelWithConstraints,poiList,0.05,nullParams);
	      HypoTestResult* plHtr = plCalc3.GetHypoTest();
	      std::cout << "The p-value for the null hypo. is " << plHtr->NullPValue() << std::endl;
	      std::cout << "Corresponding to a signifcance of " << plHtr->Significance() << std::endl;
	    */
	    }
	  if (in==0)    { fr.statUnc=statUnc;}
	  else if(in>0) { fr.uncList[test]=relErr; }

	  if(in<=0)
	    {
	      //this is the most stupid thing ... to get the likelihood curve
	      //from the list of primitives in the canvas, convert the RooCurve to a TGraph
	      //even if the class is derived from it (otherwise it crashes)
	      TCanvas *c= new TCanvas("c","c",600,600);
	      LikelihoodIntervalPlot plot(interval);
	      //plot.SetRange(0,3.5);
	      //plot.SetNPoints(100);	      
	      float maxX(3);
	      if(fr.rmax>3) maxX=6;
	      plot.SetRange(0,maxX);
	      plot.SetNPoints(200);
	      plot.Draw(""); 
	      TIter nextpobj(c->GetListOfPrimitives());
	      TObject *pobj;
	      while ((pobj = nextpobj()))
		{
		  if(pobj==0) break;
		  TString pobjName(pobj->GetName());
		  if(!pobjName.BeginsWith("nll")) continue;
		  RooCurve *nllCurve=(RooCurve *)pobj;
		  TGraph *gr=new TGraph;
		  for(int ipt=0; ipt<nllCurve->GetN(); ipt++)
		    {
		      Double_t ix,iy;
		      nllCurve->GetPoint(ipt,ix,iy);
		      gr->SetPoint(ipt,ix,iy);
		    }		  
		  gr->SetName(chTag);
		  plotList.push_back(std::pair<TString,TGraph *>(test,gr));
		  if(in<0) {
		    finalPlotList.push_back( std::pair<TString,TGraph *>( chTag, gr) );
		    if(chTag.Contains("inclusive") || chTag.Contains("combined") || chTag.Contains("total"))
		      {
			int n = gr->GetN();
			double* y = gr->GetY();
			int locmax = TMath::LocMax(n,y);
			finalRmin=irmin;
			finalRmax=irmax;
		      }
		  }
		}
	      delete c;
	    }

	  delete interval;
	  inF->Close();
	}


      //show the plot
      TString fout("PLR"); fout += (ifile+1);
      showPLR(plotList,fout,chTag,fr.rmin,fr.rmax);
      TString key(chTag);
      if(key=="inclusive" || key=="combined") key="."+key;
      finalResults[key]=fr;
    
      /*
      //
      ProfileInspector p;
      TList* list = p.GetListOfProfilePlots(*data,mc);
      TCanvas* c1 = new TCanvas("c1","ProfileInspectorDemo",1200,1200);
      c1->Divide(list->GetSize()/4+1,4);
      for(int i=0; i<list->GetSize(); ++i){
      c1->cd(i+1);
      TGraph *gr=(TGraph *) list->At(i);
      TGraph *frame=new TGraph;
      frame->SetPoint(0,-0.5,-1.5);
      frame->SetPoint(1,3.5,1.5);
      frame->SetMarkerStyle(1);
      frame->Draw("ap");
      frame->GetYaxis()->SetRangeUser(-1,1);
      frame->GetYaxis()->SetTitle(gr->GetYaxis()->GetTitle());
      frame->GetXaxis()->SetRangeUser(0,3);
      frame->GetXaxis()->SetTitle(gr->GetXaxis()->GetTitle());
      gr->Draw("l");
      }
      c1->SaveAs("~/www/LikelihoodInspector.png");
      return 0;
      */
    }
  
  showPLR(finalPlotList,"PLR","",finalRmin,finalRmax);

  cout << endl << endl;
  cout << "Category &               ";
  for(std::map<TString,FitResults_t>::iterator it = finalResults.begin(); it != finalResults.end(); it++) cout << it ->first <<  " & ";
  cout << " \\\\\\hline" << endl;
  cout << " $\\sigma/\\sigma_{SM}$ &";
  for(std::map<TString,FitResults_t>::iterator it = finalResults.begin(); it != finalResults.end(); it++) cout << it->second.r <<  " & ";
  cout << " \\\\" << endl;
  cout << cl*100 << " \\% C.I. &    ";
  for(std::map<TString,FitResults_t>::iterator it = finalResults.begin(); it != finalResults.end(); it++) cout << "]" << it->second.rmin << "," << it->second.rmax << "[" <<  " & ";
  cout << " \\\\" << endl;
  cout << " Stat. unc. (\\%) &";
  for(std::map<TString,FitResults_t>::iterator it = finalResults.begin(); it != finalResults.end(); it++) cout << it->second.statUnc*100 << " & ";
  cout << " \\\\" << endl;
  cout << " Total unc. (\\%) &";
  for(std::map<TString,FitResults_t>::iterator it = finalResults.begin(); it != finalResults.end(); it++) cout << it->second.totalUnc*100 << " & ";
  cout << " \\\\\\hline" << endl;
  std::map<TString, float> uncList;
  if(finalResults.find("combined") != finalResults.end())       uncList=finalResults["combined"].uncList;
  else if(finalResults.find("inclusive") != finalResults.end()) uncList=finalResults["inclusive"].uncList;
  else if(finalResults.find("total") != finalResults.end())     uncList=finalResults["total"].uncList;
  else uncList=finalResults.begin()->second.uncList;
  for(std::map<TString, float>::iterator it = uncList.begin(); it!= uncList.end(); it++)
    {
      TString systName(it->first); systName.ReplaceAll("_"," ");
      cout << systName << " & ";
      for(std::map<TString,FitResults_t>::iterator itt = finalResults.begin(); itt != finalResults.end(); itt++)
	{
	  if(itt->second.uncList.find(it->first)==itt->second.uncList.end()) cout << "    & ";
	  else cout << setprecision(2) << 100*itt->second.uncList[it->first] << " & ";
	}
      cout << " \\\\" << endl;
    }
  cout << "\\hline" << endl;

  cout << endl << endl;
}
  

//
void showPLR(std::vector<std::pair<TString,TGraph *> > &plotList, TString fout,TString header, float rmin,float rmax)
{
  //order and name the graphs
  std::vector<TGraph *> orderedPlotList;
  for(size_t ipl=0; ipl<plotList.size(); ipl++)
    {
      TGraph *gr=plotList[ipl].second;
      TString tag=plotList[ipl].first; tag.ReplaceAll("mu","#mu");
      gr->SetTitle(tag);
      if(tag=="total" || tag=="inclusive") orderedPlotList.insert(orderedPlotList.begin(),gr);
      else                                 orderedPlotList.push_back(gr);
    }

  //declare the canvas
  TCanvas *c= new TCanvas("c","c",600,600);

  //build the legend
  TLegend *leg=new TLegend(0.2,0.7,0.5,0.9);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  header.ReplaceAll("mu","#mu");
  leg->SetHeader(header);
  //  leg->SetNColumns(plotList.size());

  //plot the PLRs
  Int_t colors[]={kBlue,kGray,kOrange-8,kGreen-7,kMagenta+3,kBlue-10};
  for(size_t ipl=0; ipl<orderedPlotList.size(); ipl++)
    {
      TGraph *gr  = orderedPlotList[ipl];
      TString tag = gr->GetTitle();
      gr->SetLineColor(colors[ipl]);
      gr->SetLineWidth(2);
      gr->Draw(ipl==0?"al":"l");
      leg->AddEntry(gr,tag,"l");
      if(ipl>0) continue;
      float maxX=3;
      if(rmax>3) maxX=6;
      gr->GetXaxis()->SetRangeUser(0,maxX);
      gr->GetXaxis()->SetTitle("#sigma/#sigma_{th.}");
      gr->GetYaxis()->SetRangeUser(0,5);
      gr->GetYaxis()->SetTitle("- log #lambda");
      
      TLine *l=new TLine(0,0.5,maxX,0.5); l->SetLineColor(kGreen); l->Draw();
      l=new TLine(rmin,0.,rmin,0.5); l->SetLineColor(kGreen); l->Draw();
      l=new TLine(rmax,0.,rmax,0.5); l->SetLineColor(kGreen); l->Draw();
    }

  //draw the legend
  leg->Draw("same");
  
  //now the caption
  TPaveText* T = new TPaveText(0.1,0.995,0.84,0.95, "NDC");
  T->SetFillColor(0);
  T->SetFillStyle(0);  T->SetLineColor(0);
  T->SetTextAlign(22);
  char Buffer[1024];
  if(iEcm==0) sprintf(Buffer, "CMS simulation");
  else        sprintf(Buffer, "CMS preliminary, #sqrt{s}=%.1f TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", iEcm, iLumi/1000);
  T->AddText(Buffer);
  T->Draw("same");
  T->SetBorderSize(0);
  
  //all done
  c->Modified();
  c->Update();
  c->SaveAs(fout+".png");
  c->SaveAs(fout+".pdf");
  c->SaveAs(fout+".C");
}
