/*
  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  .L runShapeAnalysis.C+
*/


#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TEventList.h"
#include "TROOT.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TList.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TObjArray.h"

#include<iostream>
#include<fstream>
#include<map>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;


TString landsExe("$CMSSW_BASE/src/UserCode/mschen/LandS/test/lands.exe");

#define RUNBAYESIAN(INURL,OUTURL)\
  gSystem->ExpandPathName(landsExe); \
  TString cmd=TString(landsExe + " -M Bayesian -d ") + INURL + TString( " --doExpectation 1 -t 10000 > ") + OUTURL + TString(" "); \
  cout << "Launching : " << cmd << endl;\
  gSystem->Exec(cmd);

#define RUNASYMPTOTIC(INURL,OUTURL)\
  gSystem->ExpandPathName(landsExe); \
  TString cmd=TString(landsExe + " -d ") + INURL + TString(" -M Hybrid --freq --ExpectationHints Asymptotic --scanRs 1 --freq --nToysForCLsb 10000 --nToysForCLb 5000 --seed 1234 -rMax 50 -rMin 0.1 > ") + logUrl + TString(" "); \
  cout << "Launching : " << cmd << endl;				\
  gSystem->Exec(cmd);


using namespace std;

typedef std::pair<TString,TString> RateKey_t;

struct DataCardInputs
{
  TString shapesFile;
  std::vector<TString> ch;
  std::vector<TString> procs;
  std::map<RateKey_t, Double_t> obs;
  std::map<RateKey_t, Double_t> rates;
  std::map<TString, std::map<RateKey_t,Double_t> > systs;
};

DataCardInputs convertHistosForLimits(Int_t mass,TString histo="finalmt",TString url="plotter.root",TString outDir="./", bool runSystematics=true, bool shape=true, Int_t index=-1);
std::vector<TString> buildDataCard(Int_t mass, TString histo="finalmt", TString url="plotter.root",TString outDir="./", bool runSystematics=true, bool shape=true, Int_t index=-1);


int main(int argc, char* argv[])
{
   if(argc<4){printf("not enough arguments\n"); exit(0);}

   int mass; TString histo; TString url; int index=-1;
   sscanf(argv[1],"%i", &mass);
   histo = TString(argv[2]);
   url = TString(argv[3]);
   int runSystematicsTmp = 1;
   if(argc>=5)sscanf(argv[4],"%i", &runSystematicsTmp);
   bool runSystematics = (runSystematicsTmp==1);
   int isShape = 1;
   if(argc>=5)sscanf(argv[5],"%i", &isShape);
   bool shape = (isShape==1);
   if(argc>=6)sscanf(argv[6],"%i", &index);
   printf("runLandS with following arguments: %i - %s - %s - %i\n",mass, histo.Data(), url.Data(), index);

  //prepare the output directory
  TString outDir("H"); outDir += mass;
  if(shape){outDir+=TString("_shape");}else{outDir+=TString("_count");}
  if(index>=0){outDir+=TString("_");outDir+=index;}
  TString mkdirCmd("mkdir -p "); mkdirCmd+=outDir;
  gSystem->Exec(mkdirCmd.Data());

  if(runSystematics){
     mkdirCmd += "/combined";
     gSystem->Exec(mkdirCmd);
   }

  //build the datacard for this mass point
  std::vector<TString> dcUrls = buildDataCard(mass,histo,url,outDir, runSystematics, shape, index);

  if(runSystematics){
     //run the combined limits 
     //need to create a new directory with the exclusive datacards and link the root file with the histos
     for(size_t i=0; i<dcUrls.size(); i++)
     {
        TString cpCmd("cp ");
        cpCmd += dcUrls[i];
        TString newDCUrl(dcUrls[i]); newDCUrl.ReplaceAll(outDir,outDir+"/combined/");
        cpCmd += " " + newDCUrl;
        gSystem->Exec(cpCmd);
     }

     TString lnCmd("ln -sf ");
     lnCmd += outDir;  lnCmd += "/Shapes_"; lnCmd += mass; lnCmd += ".root ";
     lnCmd += outDir;  lnCmd += "/combined/Shapes_"; lnCmd += mass; lnCmd += ".root ";
     gSystem->Exec(lnCmd);

     TString logUrl(outDir+"/combined/Shapes_"); logUrl += mass; logUrl += ".log";
     RUNASYMPTOTIC(outDir+"/combined/*.dat",logUrl);


     //run limits in the exclusive channels
     for(size_t i=0; i<dcUrls.size(); i++)
     {
        TString logUrl(dcUrls[i]); logUrl.ReplaceAll(".dat",".log");
//        RUNASYMPTOTIC(dcUrls[i],logUrl);
     }
  }else{
     //run the combined limits 
     //need to create a new directory with the exclusive datacards and link the root file with the histos
     TString cards = "";
     for(size_t i=0; i<dcUrls.size(); i++){
        cards+=dcUrls[i]+" ";
     }
     TString logUrl(outDir+"/Shapes_"); logUrl += mass; logUrl += ".log";
     RUNASYMPTOTIC(cards,logUrl);
  }
}



//
std::vector<TString>  buildDataCard(Int_t mass, TString histo, TString url, TString outDir, bool runSystematics, bool shape, Int_t index)
{
  std::vector<TString> dcUrls;
  

  //get the datacard inputs 
  DataCardInputs dci = convertHistosForLimits(mass,histo,url,outDir, runSystematics, shape, index);




  //build the datacard separately for each channel
  for(size_t i=1; i<=dci.ch.size(); i++) 
    {
  
      ofstream dcF;
      TString dcName=outDir +"/" + dci.shapesFile;
      dcName.ReplaceAll(".root","_"+dci.ch[i-1]+".dat");
      dcF.open(dcName.Data(),ios::out);

      //header
      dcF << "imax 1" << endl
	  << "jmax *" << endl //<< dci.procs.size() << endl
	  << "kmax *" << endl
	  << "-------------------------------" << endl
	  << "shapes * * " << outDir << "/" << dci.shapesFile << " $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  
      //observations
      dcF << "-------------------------------" << endl
	  << "bin " << dci.ch[i-1] << endl
	  << "Observation " << dci.obs[RateKey_t("obs",dci.ch[i-1])] << endl;
      
      //yields per bin
      dcF << "-------------------------------" << endl
	  << "bin\t ";
      for(size_t j=1; j<=dci.procs.size(); j++) 
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1 << " ";
	}
      dcF << endl
	  << "process\t ";
      for(size_t j=1; j<=dci.procs.size(); j++) 
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << dci.procs[j-1] << " ";
	}
      dcF << endl
	  << "process\t ";
      int procCtr(0);
      for(size_t j=1; j<=dci.procs.size(); j++) 
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << procCtr << " ";
	  procCtr++;
	}
      dcF << endl
	  << "rate\t";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << dci.rates[RateKey_t(dci.procs[j-1],dci.ch[i-1])] << " ";
	}
      dcF << endl;

if(runSystematics){
      
      //systematics
      dcF << "-------------------------------" << endl;
      dcF << "Lumi\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1.045 << " ";
	}
      dcF << endl;
  
      dcF << "trigEff\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1.01 << "  ";
	}
      dcF << endl;

      dcF << "idEff\t lnN\t ";
      for(size_t j=1; j<=dci.procs.size(); j++)
	{
	  if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	  dcF << 1.03 << "  ";
	}
      dcF << endl;
      
      //systematics with shape description
      for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator it=dci.systs.begin(); it!=dci.systs.end(); it++)
	{
	  dcF << it->first << "\t shapeN2\t";
	  for(size_t j=1; j<=dci.procs.size(); j++)
	    {
	      if(dci.rates.find(RateKey_t(dci.procs[j-1],dci.ch[i-1]))==dci.rates.end()) continue;
	      if(it->second.find(RateKey_t(dci.procs[j-1],dci.ch[i-1])) != it->second.end())
		dcF << it->second[RateKey_t(dci.procs[j-1],dci.ch[i-1])];
	      else
		dcF << "-";
	      dcF << " ";
	    }
	  dcF << endl;
	}
}
  
      dcF.close();
      cout << "Data card for " << dci.shapesFile << " and " << dci.ch[i-1] << " channel @ " << dcName << endl;
      dcUrls.push_back(dcName);
    }

  //all done
  return dcUrls;
}

//
DataCardInputs convertHistosForLimits(Int_t mass,TString histo,TString url,TString outDir, bool runSystematics, bool shape, Int_t index)
{
  DataCardInputs dci;

  TString massStr(""); massStr += mass;

  //prepare the output
  dci.shapesFile="Shapes_"+massStr+".root";
  TFile *fout=TFile::Open(outDir + "/" + dci.shapesFile,"recreate");
  fout->mkdir("ee");
  fout->mkdir("mumu");

  //get histos from input file
  std::set<TString> allCh,allProcs,allSysts;
  TFile *fin=TFile::Open(url);
  TIter pnext(fin->GetListOfKeys());
  TObject *p=0;
  while((p=pnext())){

    //format process name
    TString procTitle(p->GetName());
    TString proc(procTitle);
    printf("Proc=%s & mass = %i\n", proc.Data(), mass);

    if(proc.Contains("H(")){
            if(             mass<300 && !proc.Contains("H(200)")) continue;
       else if(mass>=300 && mass<400 && !proc.Contains("H(300)")) continue;
       else if(mass>=400 && mass<500 && !proc.Contains("H(400)")) continue;
       else if(mass>=500 && mass<600 && !proc.Contains("H(500)")) continue;
       else if(mass>=600 && mass<1000&& !proc.Contains("H(600)")) continue;
    }


//    if(proc.Contains("H(") && !proc.Contains(massStr)) continue;
    if(proc.Contains("H")){ proc="asignal"; printf("Signal\n");}
    proc.ReplaceAll("#bar{t}","tbar");
    proc.ReplaceAll("Z-#gamma^{*}+jets#rightarrow ll","dy");
    proc.ReplaceAll("(","");    proc.ReplaceAll(")","");    proc.ReplaceAll("+","");    proc.ReplaceAll(" ","");
    proc.ToLower();
    //if(proc!="data" && proc!="zz" && proc!="ww" && proc!="signal") continue;
    //    if(proc!="data" && proc!="zz" && proc!="wz" && proc!="signal") continue;
    //    if(proc=="data" && proc!="ww" && proc!="wz" && proc!="zz" && proc!="signal") continue;
    //if(proc=="dy" || proc=="ttbar" || proc=="singletop") continue;
//    if(proc=="dy") continue;
    if(proc!="data") allProcs.insert(proc);

    //get histos for process
    TDirectory *pdir = (TDirectory *)fin->Get(p->GetName());
    TIter hnext( pdir->GetListOfKeys() );
    TObject *h=0;
    std::map<TString, TH1 *>hcentral;
    std::map<TString, TObjArray> grvarup, grvardown;

    int count = histo.CountChar('_');
    while((h=hnext()))
      {
	//prune for histo of interest
	TString hname(h->GetName());
	if(!hname.Contains(histo) || !hname.Contains("_")) continue;

	//prune for channels and mass of interest
	TObjArray *tkns=hname.Tokenize("_");	

	TString ch(tkns->At(0)->GetName());      if(ch!="ee" && ch!="mumu") continue;

//	TString massel(tkns->At(1)->GetName());	 if(index<0 && !massel.Contains(massStr)) continue;

	TString syst="";
	if(tkns->GetEntriesFast()>2+count) syst="_"+TString(tkns->At(2+count)->GetName());
	syst.ReplaceAll("down","Down");
	syst.ReplaceAll("up","Up");
	if(syst.Contains("btag")) continue;

	//save histo as $CHANNEL/$PROCESS_$SYSTEMATIC
        TH2* tmp = (TH2*) pdir->Get(hname);
        TH1* hshape = (TH1*) tmp->ProjectionY("_py", index,index);     

        if(!shape){
           hshape = hshape->Rebin(hshape->GetXaxis()->GetNbins(), TString(hshape->GetName())+"_Rebin"); 

           //make sure to also count the underflow and overflow
           double bin  = hshape->GetBinContent(0) + hshape->GetBinContent(1) + hshape->GetBinContent(2);
           double bine = sqrt(hshape->GetBinError(0)*hshape->GetBinError(0) + hshape->GetBinError(1)*hshape->GetBinError(1) + hshape->GetBinError(2)*hshape->GetBinError(2));
           hshape->SetBinContent(0,0);
           hshape->SetBinError  (0,0);
           hshape->SetBinContent(1,bin);
           hshape->SetBinError  (1,bine);
           hshape->SetBinContent(2,0);
           hshape->SetBinError  (2,0);
        }

//	if(hshape->Integral()<=0) continue;
	hshape->SetDirectory(0);
	fout->cd(ch);

        if(syst=="")
	  {
	    //central shape (for data call it data_obs)
	    hshape->SetName(proc); 
	    if(proc=="data")  hshape->SetName("data_obs");
	    hshape->Write();
	  }
	else if(runSystematics && proc!="data" && (syst.Contains("Up") || syst.Contains("Down")))
	  {
	    //write variation to file
	    hshape->SetName(proc+syst);
	    hshape->Write();
	  }
	else if(runSystematics)
	  {
	    //for one sided systematics the down variation mirrors the difference bin by bin
	    hshape->SetName(proc+syst);
	    hshape->Write(proc+syst+"Up");
	    TH1 *hmirrorshape=(TH1 *)hshape->Clone(proc+syst+"Down");
	    hmirrorshape->Add(hcentral[ch],-1);
	    for(int ibin=1; ibin<=hmirrorshape->GetXaxis()->GetNbins(); ibin++)
	      hmirrorshape->SetBinContent(ibin,-hmirrorshape->GetBinContent(ibin));
	    hmirrorshape->Add(hcentral[ch],1);
	    hmirrorshape->Write(proc+syst+"Down");
	  }

        TString systName(syst); 

	if(runSystematics){
	//compute variations to illustrate
	systName.ReplaceAll("Up",""); systName.ReplaceAll("Down","");  systName.ReplaceAll("_","");
	bool doUp(syst.Contains("Up")),doDown(syst.Contains("Down"));
	if(syst=="") { doUp=true; doDown=true; systName="stat"; hcentral[ch]=hshape;}
	else if(!syst.Contains("Up") && !syst.Contains("Down")) { doUp=true; doDown=true; }
	TGraph *grup=0, *grdown=0;
	if(doUp)   {
	  //	  int ic=grvarup[ch].GetEntriesFast()?kRed+grvarup[ch].GetEntriesFast():1;
	  int ic=grvarup[ch].GetEntriesFast()+1;
	  grup = new TGraph;        grup->SetTitle(systName);  grup->SetName(ch+systName+"up"); 
	  grup->SetFillStyle(0);    grup->SetLineWidth(2);  grup->SetLineColor(ic);   grvarup[ch].Add(grup); 
	}
	if(doDown) {
	  //  int ic=grvardown[ch].GetEntriesFast()?kBlue+grvardown[ch].GetEntriesFast():1;
	  int ic=grvardown[ch].GetEntriesFast()+1;
	  grdown = new TGraph;      grdown->SetTitle(systName); grdown->SetName(ch+systName+"down"); 
	  grdown->SetFillStyle(0);  grdown->SetLineWidth(2); grdown->SetLineColor(ic); grvardown[ch].Add(grdown);  
	}	
	for(int ibin=1; ibin<= hcentral[ch]->GetXaxis()->GetNbins(); ibin++)
	  {
	    Double_t x = hcentral[ch]->GetXaxis()->GetBinCenter(ibin);
	    Double_t y = hcentral[ch]->GetBinContent(ibin);
	    Double_t diffUp = (hshape->GetBinContent(ibin)-y);
	    Double_t diffDown = (hshape->GetBinContent(ibin)-y);
	    if(syst=="") { diffUp = hcentral[ch]->GetBinError(ibin); diffDown=-diffUp; }
	    else if(!syst.Contains("Up") && !syst.Contains("Down")) 
	      {
		diffUp = 0.5*fabs(hshape->GetBinContent(ibin)-y);
		diffDown =-diffDown; 
	      }
	    if(y==0) continue;
	    diffUp = diffUp*100./y;
	    diffDown = diffDown*100./y;
	    if(grup) grup->SetPoint(grup->GetN(), x, diffUp );
	    if(grdown) grdown->SetPoint(grdown->GetN(), x, diffDown );
	  }
      }

	//add to datacard
        allCh.insert(ch);
	if(runSystematics && syst!="") allSysts.insert(systName);
//	if(runSystematics && proc!="data" && syst!="" && hshape->Integral()>0)
      if(runSystematics && proc!="data" && syst!="")
	  {
	    TH1 *temp=(TH1 *) hshape->Clone();
	    temp->Add(hcentral[ch],-1);
	    if(temp->Integral()!=0) dci.systs[systName][RateKey_t(proc,ch)]=1.0;
	    delete temp;

	  }
	else if(proc!="data" && syst=="") dci.rates[RateKey_t(proc,ch)]=hshape->Integral();
	else if(proc=="data" && syst=="") dci.obs[RateKey_t("obs",ch)]=hshape->Integral();

      }

    //show results for this process
    TCanvas *c = new TCanvas("c"+proc,"c"+proc);
    c->SetWindowSize(600,600);
    c->Divide(2,3);
    int icol(0);
    for(std::map<TString,TH1*>::iterator it = hcentral.begin(); it!= hcentral.end(); it++,icol++)
      {
	TString ch=it->first;
	TH1 *ihcentral=it->second;

	TPad *ipad=(TPad *)c->cd(icol+1);
	ipad->SetPad(0+icol*0.5,0.6,0.5+icol*0.5,1.0);
	ipad->SetBottomMargin(0);
	ihcentral->Draw("hist");
	ihcentral->SetFillColor(kGray);
	ihcentral->SetFillStyle(1001);
	ihcentral->GetYaxis()->SetTitleSize(0.1);
	ihcentral->GetYaxis()->SetTitleOffset(0.33);
	if(icol==0)
	  {
	    TPaveText *pave=new TPaveText(0.3,0.91,0.7,0.99,"brNDC");
	    pave->SetBorderSize(0);
	    pave->SetFillStyle(0);
	    pave->AddText(procTitle);
	    pave->Draw();
	  }
	TPaveText *pave=new TPaveText(0.7,0.75,0.9,0.9,"brNDC");
	pave->SetBorderSize(0);
	pave->SetFillStyle(0);
	TString chtitle(ch + " events"); chtitle.ReplaceAll("mu","#mu");
	pave->AddText(chtitle)->SetTextFont(42);
	pave->Draw();

	ipad=(TPad *)c->cd(icol+3);    
	ipad->SetPad(0+icol*0.5,0.3,0.5+icol*0.5,0.6);
	ipad->SetTopMargin(0);
	ipad->SetBottomMargin(0);
	TLegend *leg=0;
	if(icol==0)   { leg=new TLegend(0.3,0.2,0.7,0.4); leg->SetBorderSize(0); leg->SetFillStyle(0); leg->SetNColumns(3); leg->SetTextFont(42); }
	for(Int_t i=0; i<grvarup[ch].GetEntriesFast(); i++) {
	  TGraph *igr=(TGraph *)grvarup[ch].At(i);
	  igr->Draw(i==0?"al":"l");
	  if(leg) leg->AddEntry(igr,igr->GetTitle(),"l");
	  if(i) continue;
	  igr->GetXaxis()->SetTitle(ihcentral->GetXaxis()->GetTitle());
	  igr->GetYaxis()->SetRangeUser(-100,100);
	  igr->GetYaxis()->SetTitle("#Delta + %");
	  igr->GetYaxis()->SetTitleSize(0.1);
	  igr->GetYaxis()->SetTitleOffset(0.33);
	  ihcentral->GetXaxis()->SetRangeUser(igr->GetXaxis()->GetXmin(),igr->GetXaxis()->GetXmax());
	}
	if(leg) leg->Draw();

	ipad=(TPad *)c->cd(icol+5);
	ipad->SetPad(0+icol*0.5,0.,0.5+icol*0.5,0.3);
	ipad->SetTopMargin(0);
	ipad->SetBottomMargin(0.15);
	for(Int_t i=0; i<grvardown[ch].GetEntriesFast(); i++){ 
	  TGraph *igr=(TGraph *)grvardown[ch].At(i);
	  igr->Draw(i==0?"al":"l");
	  if(i) continue;
	  igr->GetXaxis()->SetTitle(ihcentral->GetXaxis()->GetTitle());
	  igr->GetYaxis()->SetRangeUser(-100,100);
	  igr->GetYaxis()->SetTitle("#Delta - %");
	  igr->GetXaxis()->SetTitleSize(0.1);
	  igr->GetXaxis()->SetTitleOffset(0.6);
	  igr->GetYaxis()->SetTitleSize(0.1);
	  igr->GetYaxis()->SetTitleOffset(0.45);
	}
      }
    fout->cd();
    c->Write();
  }

  //all done
  fin->Close();
  fout->Close();

  dci.ch.resize(allCh.size());        std::copy(allCh.begin(), allCh.end(),dci.ch.begin());
  dci.procs.resize(allProcs.size());  std::copy(allProcs.begin(), allProcs.end(),dci.procs.begin());



// DEBUGGING
//  printf("-----------------------\n");
//  printf("shapesFile=%s\n",dci.shapesFile.Data());
//  for(unsigned int i=0;i<dci.ch.size();i++){printf("%s - ",dci.ch[i].Data());}printf("\n");
//  for(unsigned int i=0;i<dci.procs.size();i++){printf("%s - ",dci.procs[i].Data());}printf("\n");
//  for(std::map<TString, std::map<RateKey_t,Double_t> >::iterator iter = dci.systs.begin();   iter != dci.systs.end(); ++iter ){
//       printf("%s : ", iter->first.Data());
//       for(std::map<RateKey_t, Double_t>::iterator it = iter->second.begin();   it != iter->second.end(); ++it ){
//                 printf("%s_%s (%f) ", it->first.first.Data(), it->first.second.Data(), it->second);
//       }
//       printf("\n");
//  }
//  printf("-----------------------\n");








  return dci;
}


