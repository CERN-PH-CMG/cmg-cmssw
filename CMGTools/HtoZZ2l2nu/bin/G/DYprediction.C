#include "TFile.h"
#include "TH2F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"

#include <fstream>
#include<strstream>
#include <iostream> 
#include <string> 
#include <map>

using namespace std;

TString outDir("Img/");
strstream report;
TStyle *tdrStyle;

struct Shape_t
{
  TH1* data, *totalBckg;
  std::map<TString, TH1 *> bckg, signal;
};

void showShape(const Shape_t &shape,TString SaveName,bool is2011,int model,TFile *closeF=0);
void setTDRStyle();


Int_t findClosestPoint(TGraph *gr,Double_t xToMatch){
 
  Int_t PointNumber =0;
  Double_t x,y;
  gr->GetPoint(0,x,y);
  Double_t Delta = TMath::Abs(xToMatch-x);
  for (Int_t i=1; i<gr->GetN();i++){
    gr->GetPoint(i,x,y);
    if (TMath::Abs(xToMatch-x)>Delta) continue;
    Delta = TMath::Abs(xToMatch-x);
    PointNumber=i;
  }
  return PointNumber;
}

//
void fixExtremities(TH1* h,bool addOverflow, bool addUnderflow)
{
  if(h==0) return;

  if(addUnderflow){
      double fbin  = h->GetBinContent(0) + h->GetBinContent(1);
      double fbine = sqrt(h->GetBinError(0)*h->GetBinError(0)
                          + h->GetBinError(1)*h->GetBinError(1));
      h->SetBinContent(1,fbin);
      h->SetBinError(1,fbine);
      h->SetBinContent(0,0);
      h->SetBinError(0,0);
    }
  
  if(addOverflow){  
      int nbins = h->GetNbinsX();
      double fbin  = h->GetBinContent(nbins) + h->GetBinContent(nbins+1);
      double fbine = sqrt(h->GetBinError(nbins)*h->GetBinError(nbins) 
                          + h->GetBinError(nbins+1)*h->GetBinError(nbins+1));
      h->SetBinContent(nbins,fbin);
      h->SetBinError(nbins,fbine);
      h->SetBinContent(nbins+1,0);
      h->SetBinError(nbins+1,0);
    }
}



//
void addToShape(Shape_t &a, Shape_t &b,int sign=+1)
{
  if(a.data && b.data)           a.data->Add(b.data,sign);
  if(a.totalBckg && b.totalBckg) a.totalBckg->Add(b.totalBckg,sign);
  for(std::map<TString, TH1 *>::iterator it = a.bckg.begin(); it != a.bckg.end(); it++)
    {
      if(b.bckg.find(it->first)==b.bckg.end()) continue;
      it->second->Add( b.bckg[it->first], sign );
    }
  for(std::map<TString, TH1 *>::iterator it = b.bckg.begin(); it != b.bckg.end(); it++)
    {
      if(a.bckg.find(it->first)!=a.bckg.end()) continue;
      TH1 *h=(TH1 *)it->second->Clone();
      h->SetDirectory(0);
      h->Scale(sign);
      a.bckg[it->first]=h;
    }
  for(std::map<TString, TH1 *>::iterator it = a.signal.begin(); it != a.signal.end(); it++)
    {
      if(b.signal.find(it->first)==b.signal.end()) continue;
      it->second->Add( b.signal[it->first], sign );
    }
  for(std::map<TString, TH1 *>::iterator it = b.signal.begin(); it != b.signal.end(); it++)
    {
      if(a.signal.find(it->first)!=b.signal.end()) continue;
      TH1 *h=(TH1 *)it->second->Clone();
      h->SetDirectory(0);
      h->Scale(sign);
      a.signal[it->first]=h;
    }
}

//
Shape_t cloneShape(Shape_t &orig,TString newName)
{
  Shape_t newShape;
  newShape.data      = (TH1 *) orig.data->Clone(newName+"data");           newShape.data->SetDirectory(0);
  newShape.totalBckg = (TH1 *) orig.totalBckg->Clone(newName+"totalbckg"); newShape.totalBckg->SetDirectory(0);
  for(std::map<TString, TH1 *>::iterator it = orig.bckg.begin(); it != orig.bckg.end(); it++)
    {
      TH1 *h=(TH1 *)it->second->Clone(newName+it->first); h->SetDirectory(0);
      newShape.bckg[it->first]=h;
    }
  for(std::map<TString, TH1 *>::iterator it = orig.signal.begin(); it != orig.signal.end(); it++)
    {
      TH1 *h=(TH1 *)it->second->Clone(newName+it->first); h->SetDirectory(0);
      newShape.signal[it->first]=h;
    }
  return newShape;
}


//
enum SubtractionTypes {NOSUBTRACTION, HALVE, EWKSUBTRACTION, EWKSUBTRACTIONHALVE};
enum ModelType { HZZ,ZZ,VBFZ};
void DYprediction(int subtractType=NOSUBTRACTION,int model=VBFZ,TString closureFile="")
{
  setTDRStyle();
 
  TFile *closeF=0;
  if(closureFile!="") 
    {
      closeF=TFile::Open(closureFile);
      if(closeF!=0 && closeF->IsZombie()) closeF=0;
    }
	

  TString llFile,gammaFile;
  std::vector<std::string> histos,dilSignal,dilcats,gcats;
  if(model==VBFZ) 
    {
      //      gammaFile="/afs/cern.ch/user/p/psilva/work/vbfz/plotter_vbfz_gamma_qt_2012.root";
      //gammaFile="/afs/cern.ch/user/p/psilva/work/vbfz/plotter_vbfz_gamma_nofit_qt_2012.root";
      gammaFile="/afs/cern.ch/user/p/psilva/work/vbfz/plotter_vbfz_gamma_qt_2012.root";
      llFile="~/work/vbfz/plotter_vbfz_2012.root";

      histos.push_back("vbfcandjetdeta");      
      histos.push_back("qt");
      histos.push_back("zy");
      histos.push_back("vbfmjj");
      histos.push_back("vbfcandjet1eta");
      histos.push_back("vbfcandjet2eta");
      histos.push_back("vbfcandjetetaprod");
      histos.push_back("vbfspt");
      histos.push_back("vbfcandjet1pt");
      histos.push_back("vbfcandjet2pt");
      histos.push_back("vbfcjv15");
      histos.push_back("vbfhtcjv15");
      histos.push_back("vbfcjv20");
      histos.push_back("vbfhtcjv20");
      histos.push_back("vbfcjv");
      histos.push_back("vbfhtcjv");
      histos.push_back("vbfdphijj");
      histos.push_back("vbfhardpt");
      histos.push_back("vbfmaxcjvjpt");
      histos.push_back("met");
      histos.push_back("metL");
      histos.push_back("dijet_deta_shapes");

      //      dilSignal.push_back("#alpha^{4}_{EW}-ll (VBFNLO)");
      dilSignal.push_back("#alpha^{4}_{EW}-ll (MG)");
      
      dilcats.push_back("");
      dilcats.push_back("mjjq016");
      dilcats.push_back("mjjq033");
      dilcats.push_back("mjjq049");
      dilcats.push_back("mjjq066");
      dilcats.push_back("mjjq083");
      dilcats.push_back("mjjq092");
      dilcats.push_back("mjjq100");

      gcats.push_back("");
      gcats.push_back("mjjq016");
      gcats.push_back("mjjq033");
      gcats.push_back("mjjq049");
      gcats.push_back("mjjq066");
      gcats.push_back("mjjq083");
      gcats.push_back("mjjq092");
      gcats.push_back("mjjq100");
    }
  else
    {
      //HIGGS 2012
      //gammaFile = "~/work/htozz/53x_rereco/gamma/2012/qt/plotter.root";
      // llFile    = "~/work/htozz/53x_rereco/ll/2012/plotter.root";
      
      //SM ZZ
      //gammaFile="~lpernie/public/4Pedro/gamma_plotter.root";
      //gammaFile="~lpernie/public/4Pedro/gamma_plotter_fit.root";
      //llFile="~lpernie/public/4Pedro/dil_plotter.root";

  
      //HIGGS 2011
      gammaFile = "~/work/htozz/53x/gamma/2011/qt/plotter.root";
      //gammaFile = "~/work/htozz/53x/gamma/2011/qt_smooth/plotter.root";
      llFile    = "~/work/htozz/53x/ll/2011/plotter.root";

      
      if(model==HZZ)
	{
	  histos.push_back("met_met");
	  /*
	  histos.push_back("met_met_blind");                
	  histos.push_back("met_metNM1");
	  
 	  histos.push_back("mt");     
 	  histos.push_back("mt_blind");     
 	  histos.push_back("mtNM1");     

  	  histos.push_back("pfvbfcandjeteta");
  	  histos.push_back("pfvbfcandjetpt");
  	  histos.push_back("pfvbfcandjetdeta");
  	  histos.push_back("pfvbfmjj");
 	  histos.push_back("pfvbfpremjj");
 	  histos.push_back("pfvbfcjv");
	
  	  histos.push_back("mindphijmet_0");            
  	  histos.push_back("mindphijmet_25");            
  	  histos.push_back("mindphijmet");            
	  histos.push_back("mindphijmetNM1");                     
	  histos.push_back("mt_shapes");                     
	  histos.push_back("met_shapes");                     
	  */
  
	  dilSignal.push_back("ggH(600)#rightarrow ZZ");
	  dilSignal.push_back("qqH(600)#rightarrow ZZ");
	  dilSignal.push_back("ggH(300)#rightarrow ZZ");
	  dilSignal.push_back("qqH(300)#rightarrow ZZ");
      
	  dilcats.push_back("eq0jets");
	  dilcats.push_back("geq1jets");
	  dilcats.push_back("vbf");
	  dilcats.push_back("");

	  //gcats.push_back("eq0softjets");  //0 soft jets to be subtracted
	  gcats.push_back("eq0jets");
	  gcats.push_back("geq1jets");
	  gcats.push_back("vbf");
	  gcats.push_back("");
	}
      else
	{
	  histos.push_back("met_met");
	  histos.push_back("zpt_rebin_final");
	  dilcats.push_back("eq0jets");
	  gcats.push_back("eq0jets");
	}
    };


  bool is2011(llFile.Contains("2011"));

  string ch[]     = {"ee","mumu"};
  const size_t nchs=sizeof(ch)/sizeof(string);
  const size_t nhistos=histos.size();
  const size_t ndilcats=dilcats.size();
  std::vector<string> dilprocs;
  std::vector<int> dilColors;
  if(model==VBFZ)
    {
      dilprocs.push_back("VV");           dilColors.push_back(590);
      dilprocs.push_back("Top");          dilColors.push_back(8);
      dilprocs.push_back("QCD/W+jets");   dilColors.push_back(400);
    }
  else
    {
      dilprocs.push_back("WW#rightarrow 2l2#nu"); dilColors.push_back(592);
      if(model==HZZ) dilprocs.push_back("ZZ");  
      else          dilprocs.push_back("ZZ#rightarrow 2l2#nu");  
      dilColors.push_back(590);
      dilprocs.push_back("WZ#rightarrow 3l#nu");  dilColors.push_back(596);
      dilprocs.push_back("EWK Z+2jets");          dilColors.push_back(594);
      dilprocs.push_back("t#bar{t}");             dilColors.push_back(8);
      dilprocs.push_back("Single top");           dilColors.push_back(824);
      dilprocs.push_back("W#rightarrow l#nu");    dilColors.push_back(809);
    }
  dilprocs.push_back("data"); dilColors.push_back(1);
  const size_t ndilprocs=dilprocs.size();
  const size_t nDilSignals=dilSignal.size();
  const size_t ngcats=gcats.size();
  string gprocs[]={"EWK","data (#gamma)"};
  const size_t ngprocs=sizeof(gprocs)/sizeof(string);

  std::map<string,Shape_t> shapesMap,gShapesMap;
  std::map<string, std::map<string,float> >   scaleFactors;

  //get dilepton histos from files
  TFile *llIn=TFile::Open(llFile);
  for(size_t ich=0; ich<nchs; ich++)
    {
      for(size_t icat=0; icat<ndilcats; icat++)
	{
	  for(size_t ih=0; ih<nhistos; ih++)
	    {
	      Shape_t m_shape;
	      for(size_t iproc=0; iproc<ndilprocs; iproc++)
		{
		  string hname=dilprocs[iproc]+"/"+ch[ich]+dilcats[icat]+"_"+histos[ih];
		  TH1 *h=(TH1 *)llIn->Get(hname.c_str());
		  if(h==0)  { cout << "Missing " << hname << endl; continue; }
		  h->SetTitle(dilprocs[iproc].c_str());
		  fixExtremities(h,true,true);

		  //detach and save (also signal)
		  h->SetDirectory(0);
		  if(dilprocs[iproc].find("data") != string::npos) 
		    {
		      m_shape.data=h;
		      
		      for(size_t isig=0; isig<nDilSignals; isig++)
			{
			  string hsigname = dilSignal[isig]+"/"+ch[ich]+dilcats[icat]+"_"+histos[ih];
			  TH1 *hsig       = (TH1 *)llIn->Get(hsigname.c_str());
			  if(hsig==0) { cout << "Missing " << hsigname << endl; continue; }
			  fixExtremities(hsig,true,true);
			  hsig->SetTitle(dilSignal[isig].c_str());
			  hsig->SetDirectory(0);
			  hsig->SetLineColor(1+isig/2);
			  hsig->SetLineStyle(1+isig%2);
			  hsig->SetLineWidth(2);
			 //  if(model==VBFZ)
			  // 			    {
			  // 			      if(ch[ich]=="ee")        hsig->Scale(is2011 ? 0.985 : 0.842);
			  // 			      else if(ch[ich]=="mumu") hsig->Scale(is2011 ? 1.106 : 0.558 );
			  // 			    }
			  m_shape.signal[dilSignal[isig]]=hsig;
			}
		    }
		  else
		    {
		      h->SetFillColor(dilColors[iproc]);
		      if(m_shape.bckg.size()==0)
			{
			  string newname(ch[ich]+dilcats[icat]+histos[ih]+"bckg");
			  m_shape.totalBckg=(TH1*)h->Clone(newname.c_str()); 
			  m_shape.totalBckg->Reset("ICE");
			  m_shape.totalBckg->SetDirectory(0);
			}
		      if(h->Integral())m_shape.totalBckg->Add(h);
		      m_shape.bckg[dilprocs[iproc]]=h;
		    }
		}

	      //save
	      string key=ch[ich]+dilcats[icat]+"_"+histos[ih];
	      shapesMap[key]=m_shape;
	    }
	}
    }
  llIn->Close();

  //get gamma histos from file
  TFile *gIn=TFile::Open(gammaFile);
  for(size_t ich=0; ich<nchs; ich++)
    {
      for(size_t icat=0; icat<ngcats; icat++)
	{
	  for(size_t ih=0; ih<nhistos; ih++)
	    {
	      Shape_t m_shape;
	      for(size_t iproc=0; iproc<ngprocs; iproc++)
		{
		  string hname=gprocs[iproc]+"/"+ch[ich]+gcats[icat]+"_"+histos[ih];
		  TH1 *h=(TH1 *)gIn->Get(hname.c_str());
		  if(h==0) { cout << " Missing: " << hname <<endl; continue; }
		  fixExtremities(h,true,true);
		  h->SetDirectory(0);
		  h->SetName( TString("g") + h->GetName() );

		  if(gprocs[iproc].find("data") != string::npos) {
		    if(model==VBFZ) h->SetTitle("QCD Z");
		    else            h->SetTitle("Z+jets");
		    m_shape.data=h; 
		  }
		  else
		    {
		      if(m_shape.bckg.size()==0 && h->Integral())
			{
			  string newname(ch[ich]+gcats[icat]+histos[ih]+"contbckg");
			  m_shape.totalBckg=(TH1*)h->Clone(newname.c_str()); 
			  m_shape.totalBckg->Reset("ICE");
			  m_shape.totalBckg->SetDirectory(0);
			}
		      if(h->Integral()) m_shape.totalBckg->Add(h);
		      m_shape.bckg[gprocs[iproc]]=h;
		    }
		}
	      string key=ch[ich]+gcats[icat]+"_"+histos[ih];
	      gShapesMap[key]=m_shape;
	    }
	}
    }
  gIn->Close();

  
  //now compute the normalization factors
  for(std::map<string,Shape_t>::iterator it=gShapesMap.begin(); it != gShapesMap.end(); it++)
    {
      int normBin(-1);
      TH1 *normH=it->second.data;

      string distNormKey("");

      ////normalization factor (from MET<50)
      if(
	 it->first.find("met_met")  != string::npos
	 && it->first.find("blind") == string::npos
	 && it->first.find("NM1")   == string::npos
	 )   
	{ 
	  distNormKey="met"; 	
	  normBin=normH->GetXaxis()->FindBin(50); 
	  if(it->first.find("vbf")) normBin=normH->GetXaxis()->FindBin(40); 
	}

      
      /*      
    ////normalization factor (from red-MET<35)
    if(it->first.find("met_redMet")!=string::npos) { distNormKey="redMet"; normBin=normH->GetXaxis()->FindBin(35); }
    
    ////normalization factor (from deta_jj<3)
    if(it->first.find("jetdeta")!=string::npos && it->first.find("NM1")==string::npos) { 	  distNormKey="jetdeta"; normBin=normH->GetXaxis()->FindBin(3); }
    
    ////normalization factor (from m_jj<500)
    if(it->first.find("mjj")!=string::npos)    { distNormKey="mjj"; normBin=normH->GetXaxis()->FindBin(500); }
    if(it->first.find("vbfcjv")!=string::npos)    { distNormKey="cjv"; normBin=normH->GetXaxis()->FindBin(0.); }
    if(it->first.find("qt")!=string::npos)    { distNormKey="qt"; normBin=normH->GetXaxis()->FindBin(100); }
      */
      ////normalization factor (from mindphijmet<0.5)
      //  if(it->first.find("mindphijmet_0")!=string::npos)          { distNormKey="mindphijmet_0"; normBin=normH->GetXaxis()->FindBin(0.5); } 
      //       else if(it->first.find("mindphijmet_25")!=string::npos)    { distNormKey="mindphijmet_25"; normBin=normH->GetXaxis()->FindBin(0.5); } 
      //       else if(it->first.find("mindphijmet_50")!=string::npos)    { distNormKey="mindphijmet_50"; normBin=normH->GetXaxis()->FindBin(0.5); } 
      //       else if(it->first.find("mindphijmetNM1")!=string::npos)    { distNormKey="mindphijmetNM1"; normBin=normH->GetXaxis()->FindBin(0.5); } 
      //       else if(it->first.find("mindphijmet")!=string::npos)       { distNormKey="mindphijmet"; normBin=normH->GetXaxis()->FindBin(0.5); } 
      
      if(normBin<0) continue;
      if(shapesMap.find(it->first)==shapesMap.end()) continue;
      TString normKey(it->first);
      normKey = normKey.Tokenize("_")->At(0)->GetName();
      //cout << normKey << "\t will be normalized from " << normH->GetXaxis()->GetTitle() << "<" << normH->GetXaxis()->GetBinCenter(normBin) << flush;
      Shape_t &dilMetShape = shapesMap[it->first];
      float sf=dilMetShape.data->Integral(1,normBin)/normH->Integral(1,normBin);
      //cout << "\t scale-factor=" << sf << endl;
      scaleFactors[distNormKey][string(normKey.Data())]=sf;
    }


  //now produce the final histos
  TFile *gOut = TFile::Open("gamma_out.root","RECREATE");
  TDirectory *gOutDir = gOut->mkdir("Instr. background (data)");
  for(std::map<string,Shape_t>::iterator it=shapesMap.begin(); it!=shapesMap.end(); it++) 
   {
     string normKey("mumu"); 
     if(it->first.find("ee")!=string::npos)             normKey="ee";
     if(it->first.find("eq0jets_")!=string::npos)       normKey+="eq0jets";
     if(it->first.find("geq1jets_")!=string::npos)      normKey+="geq1jets";
     else if(it->first.find("eq1jets_")!=string::npos)  normKey+="eq1jets";
     if(it->first.find("geq2jets_")!=string::npos)      normKey+="geq2jets";
     if(it->first.find("vbf_")!=string::npos)           normKey+="vbf";
     if(it->first.find("vbf0_")!=string::npos)           normKey+="vbf0";
     if(it->first.find("vbf1_")!=string::npos)           normKey+="vbf1";
     if(it->first.find("vbf2_")!=string::npos)           normKey+="vbf2";
     
     //normalization factor
     float sf(1.0);
     if(it->first.find("met_redMet")!= string::npos || it->first.find("mt_redMet_shapes")!=string::npos) sf=scaleFactors["redMet"][normKey];
     //     else if(it->first.find("mindphijmet") != string::npos)                                              sf=scaleFactors["mindphijmet"][normKey];
     else                                                                                                sf=scaleFactors["met"][normKey];
     if(model==VBFZ)
       {
	 sf=1.0;
       }
     if(sf==0) continue;
     if(gShapesMap.find(it->first)==gShapesMap.end()) cout << "BUG: " << it->first << " not found in gamma sample..." << endl;
     Shape_t &gShape=gShapesMap[it->first];

     TH1 *corrGammaH=(TH1 *)gShape.data->Clone((it->first+"corrg").c_str());
     corrGammaH->SetDirectory(0);
     

     if(it->first.find("balance")!= string::npos && (subtractType==HALVE || subtractType==EWKSUBTRACTION  || subtractType==EWKSUBTRACTIONHALVE))
       {
	 corrGammaH->Scale(0.5);
	 for(int ibin=1; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++) 
	   {
	     corrGammaH->SetBinError(ibin,corrGammaH->GetBinContent(ibin));
	   }
       }

     //do the subtraction for met related variables when MET>70
     if(it->first.find("mt_shapes")!= string::npos  || it->first.find("mt_redMet_shapes")!= string::npos 
	/*|| it->first.find("_mt")!= string::npos*/
	|| it->first.find("met_") != string::npos || it->first.find("dijet_mass_shapes")!=string::npos || it->first.find("dijet_deta_shapes")!=string::npos)
       {

	 if(closeF && it->first.find("dijet_deta_shapes")!=string::npos)
	   {
	     TString key("vbfcandjetdeta_func");
	     if(it->first.find("mjjq016")!=string::npos) key="mumumjjq016_"+key;
	     if(it->first.find("mjjq033")!=string::npos) key="mumumjjq033_"+key;
	     if(it->first.find("mjjq049")!=string::npos) key="mumumjjq049_"+key;
	     if(it->first.find("mjjq066")!=string::npos) key="mumumjjq066_"+key;
	     if(it->first.find("mjjq083")!=string::npos) key="mumumjjq083_"+key;
	     if(it->first.find("mjjq092")!=string::npos) key="mumumjjq092_"+key;
	     if(it->first.find("mjjq100")!=string::npos) key="mumumjjq100_"+key;
	     TF1 *closureUncGr=(TF1 *)closeF->Get(key);
	     cout << "Correcting with " << key << " " << closureUncGr << endl;
	     for(int ibin=1; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	       {
		 for(int jbin=1; jbin<=corrGammaH->GetYaxis()->GetNbins(); jbin++)
		   {
		     Double_t val=corrGammaH->GetBinContent(ibin,jbin);
		     Double_t valerr=corrGammaH->GetBinError(ibin,jbin);
		     if(closureUncGr){
		       float relErr=fabs(1-closureUncGr->Eval( corrGammaH->GetYaxis()->GetBinCenter(jbin) ));
		       valerr=sqrt(pow(valerr,2)+pow(relErr*val,2));
		     }
		     corrGammaH->SetBinError(ibin,jbin,valerr);
		   }
	       }
	   }

	 bool isTH2( corrGammaH->InheritsFrom("TH2") );
	 //cout << it->first << " " << isTH2 << endl;


	 if(subtractType==HALVE || subtractType==EWKSUBTRACTIONHALVE) {
	   int fbin( isTH2 ? 1 : corrGammaH->GetXaxis()->FindBin(80) );
	   if(it->first.find("vbf")!=string::npos && !isTH2)    fbin = corrGammaH->GetXaxis()->FindBin(50);
	   else if(it->first.find("_mt") != string::npos &&  !isTH2) fbin = corrGammaH->GetXaxis()->FindBin(350);
	   //int fbin( isTH2 ? 1 : corrGammaH->GetXaxis()->FindBin(60) );
	   for(int ibin=fbin; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	     {
	       if(isTH2)
		 {
		   for(int jbin=1; jbin<=corrGammaH->GetYaxis()->GetNbins(); jbin++)
		     {
		       Double_t val=corrGammaH->GetBinContent(ibin,jbin)/2;
		       Double_t valerr=val;
		       if(model==VBFZ) valerr=corrGammaH->GetBinError(ibin); //0.3*val;
		       corrGammaH->SetBinContent(ibin,jbin,val);
		       corrGammaH->SetBinError(ibin,jbin,valerr);
		     }
		 }
	       else
		 {
		   Double_t val=corrGammaH->GetBinContent(ibin)/2;
		   Double_t valerr=val;
		   if(model==VBFZ) valerr=0.5*val;
		   corrGammaH->SetBinContent(ibin,val);
		   corrGammaH->SetBinError(ibin,valerr);
		 }
	     }
	 }
	 
	 if(subtractType==EWKSUBTRACTION || subtractType==EWKSUBTRACTIONHALVE )
	   {
	     corrGammaH->Add( gShape.totalBckg,-1);
	     for(int ibin=1; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	       {
		 if(isTH2)
		   {
		     for(int jbin=1; jbin<=corrGammaH->GetYaxis()->GetNbins(); jbin++)
		       {
			 Double_t val=corrGammaH->GetBinContent(ibin,jbin);
			 if(val<0)  corrGammaH->SetBinContent(ibin,jbin,0.);
		       }
		   }
		 else
		   {
		     Double_t val=corrGammaH->GetBinContent(ibin);
                     if(val<0)  corrGammaH->SetBinContent(ibin,0.);
		   }
	       }
	   }
       }

     corrGammaH->Scale(sf);
     if(it->first.find("mindphijmet")!= string::npos)
       {
 	 int normBin=corrGammaH->GetXaxis()->FindBin(0.5);
	 float newSF = it->second.data->Integral(1,normBin)/(corrGammaH->Integral(1,normBin)+it->second.totalBckg->Integral(1,normBin));
 	 corrGammaH->Scale(newSF);
        }

     corrGammaH->SetFillColor(831);
     //cout << it->second.totalBckg->GetName() << " " << corrGammaH->GetName() << " " << sf << endl;
     it->second.totalBckg->Add(corrGammaH);
     it->second.bckg["Instr. background (data)"]=corrGammaH;
     showShape(it->second,"final_"+it->first,is2011,model,closeF);
     gOutDir->cd();
     TString keyToWrite(it->first.c_str());
     corrGammaH->Write(keyToWrite);

     if(it->first.find("mumu")!= string::npos)
       {
	 TString keyToGet(it->first);
	 keyToGet=keyToGet.ReplaceAll("mumu","ee");
	 Shape_t &eeShape=shapesMap[keyToGet.Data()];
	 addToShape(eeShape,it->second);
	 keyToGet=keyToGet.ReplaceAll("ee","");
	 showShape(eeShape,keyToGet.Data(),is2011,model,closeF);
       }
   }
  gOut->Close();


  //save as page
  //ofstream ofile;
  //  ofile.open (outDir+"/index.html");
  //ofile << report.str() << endl;
  //ofile.close();

  if(closeF) closeF->Close();
}

//
void showShape(const Shape_t &shape,TString SaveName,bool is2011,int model, TFile *closeF)
{
  if(shape.data->InheritsFrom("TH2"))
    {
      cout << "Skipping plot for: " << shape.data->GetName() << endl;
      return;
    }
  TCanvas* c1 = new TCanvas(SaveName,SaveName,800,800);
  c1->cd();

  TPad* t1 = new TPad("t1","t1", 0.0, 0.20, 1.0, 1.0);  t1->Draw();  t1->cd();
  if(model!=VBFZ) t1->SetLogy(true);
  
  TLegend* legA  = new TLegend(0.15,0.85,0.9,0.92);
  if(model!= VBFZ) legA = new TLegend(0.4,0.65,0.94,0.92);

  bool canvasIsFilled(false);
  THStack *stack=0;
  TH1 *mc=0,*vbfmc=0,*instrBckg=0,*nrbBckg=0;
  TGraphErrors *closureUnc=0;
  TF1 *closureUncGr=0;
  if(closeF)
    {
      TString key(SaveName);
      key.ReplaceAll("final","");
      key.ReplaceAll("_ee","");
      key.ReplaceAll("_mumu","");
      closureUnc  =(TGraphErrors *)closeF->Get("mumu"+key+"_unc");
      closureUncGr=(TF1 *)         closeF->Get("mumu"+key+"_func");
      cout << key << " " << closureUnc << " " << closureUncGr << endl;
    }
  TGraphAsymmErrors *mcgr=0;
  if(shape.bckg.size())
    {
      mc=(TH1 *)shape.totalBckg->Clone("mc");
      vbfmc=(TH1 *)shape.totalBckg->Clone("vbfmc"); vbfmc->Reset("ICE");
      stack = new THStack("stack","stack");
 
      //include VBF Z in the stack
      if(model==VBFZ && !SaveName.EndsWith("50"))
	{
	  for(std::map<TString , TH1 *>::const_iterator it=shape.signal.begin(); it!= shape.signal.end(); it++)
	    {
	      TH1 *h=it->second;
	      h->SetFillColor(809);
	      h->SetLineStyle(1);
	      h->SetLineColor(1);
	      stack->Add(h,"HIST");
	      legA->AddEntry(h,h->GetTitle(),"F");
	    }
	}


      //instrumental background (may be affected by different corrections)
      for(std::map<TString,TH1 *>::const_reverse_iterator it = shape.bckg.rbegin(); it != shape.bckg.rend(); it++)
        {
          TString itit(it->second->GetTitle());
	  if(!itit.Contains("QCD Z")) continue;
	  instrBckg=it->second;
	  
	  for(int ibin=1; ibin<=instrBckg->GetXaxis()->GetNbins(); ibin++)
	    {
	      float x     = instrBckg->GetXaxis()->GetBinCenter(ibin);
	      float xerr  = instrBckg->GetXaxis()->GetBinWidth(ibin)/2;
	      float y     = instrBckg->GetBinContent(ibin);
	      float yerr  = instrBckg->GetBinError(ibin);
	      
	      if(closureUncGr)
		{

		  Double_t pdfRelUnc=0.02;

		  //no correction
		  Double_t relCorrection=1.0;

		  //correct based on parameterization
		  Double_t pVal = closureUncGr->Eval( instrBckg->GetXaxis()->GetBinCenter(ibin) );
		  //Double_t relCorrection = pVal;
		  Double_t relUnc = pVal>1 ? pVal-1 : 1-pVal;
		  relUnc = sqrt( relUnc*relUnc + pdfRelUnc*pdfRelUnc);		  
		  
		  //assign uncertainty from statistical unc. of the closure + 2% from PDF tests
		  /*
		  Int_t iclose = findClosestPoint(closureUnc,x);
		  Double_t xclose,yclose;
		  closureUnc->GetPoint(iclose,xclose,yclose);
		  Double_t relCorrection(yclose);
		  Double_t relUnc = yclose >0 ? closureUnc->GetErrorY(iclose)/yclose : 0;
		  relUnc = sqrt( relUnc*relUnc + pdfRelUnc*pdfRelUnc);
		  */

		  y *= relCorrection;
		  yerr= y*min(max(sqrt(pow(relUnc,2)+pow(yerr/y,2)),0.),1.0);
		}

	      instrBckg->SetBinContent(ibin,x,y);
	      instrBckg->SetBinError(ibin,yerr);
	    }
	  
	  //re-scale
	  if(model==VBFZ && mc && shape.data)
	    {
	      float totalMC= mc->Integral();
	      float totalData=shape.data->Integral();
	      float totalInstr=instrBckg->Integral();
	      if(totalMC>0 && totalInstr>0) {
		float sf=totalData/totalMC;
		float sfdy=1.0+(sf-1.0)*totalMC/totalInstr;
		instrBckg->Scale(sfdy);
	      }
	    }
	}
      

      //now stack all the backgrounds
      for(std::map<TString,TH1 *>::const_reverse_iterator it = shape.bckg.rbegin(); it != shape.bckg.rend(); it++)
        {
	  TH1 *h = it->second;
	  vbfmc->Add(h);
	  
          if(h->Integral()<=0) continue;
          TString itit(h->GetTitle());
 	  if(itit.Contains("t#bar{t}") || itit.Contains("Single top") || itit.Contains("W#rightarrow") || itit.Contains("WW") 
	     || (model==VBFZ && (itit.Contains("ZZ") || itit.Contains("WZ") ) ) )
	    {
 	      if(nrbBckg==0) { 
		nrbBckg = (TH1 *) h->Clone("nrb"); 
		nrbBckg->SetTitle("Top,W,VV");
		legA->AddEntry(nrbBckg,nrbBckg->GetTitle(),"F");
		nrbBckg->SetDirectory(0); 
		stack->Add(nrbBckg,"HIST"); 
	      }
 	      else { 
		nrbBckg ->Add( h ); 
	      }
 	    }
	  else
	    {
	      stack->Add(h,"HIST");
	      legA->AddEntry(h,itit,"F");
	    }
	}

      
      stack->Draw();
      stack->GetXaxis()->SetTitle(mc->GetXaxis()->GetTitle());
      stack->GetYaxis()->SetTitle(mc->GetYaxis()->GetTitle());
      stack->SetMinimum(0.1);
      stack->SetMaximum(1.5*max(mc->GetMaximum(),shape.data->GetMaximum()));

      mc->Reset("ICE");
      mc->Add((TH1 *)stack->GetStack()->At( stack->GetStack()->GetEntriesFast()-1) );
      mc->SetDirectory(0);
      mcgr=new TGraphAsymmErrors(mc);
      mcgr->SetFillStyle(3001);//3427);
      mcgr->SetFillColor(kGray+1);
      mcgr->SetMarkerStyle(1);
      mcgr->Draw("e2p");
      
      canvasIsFilled=true;
    }
  
  //data
  if(shape.data)
    {
      shape.data->Draw(canvasIsFilled ? "E1same" : "E1");
      legA->AddEntry(shape.data,shape.data->GetTitle(),"P");
      canvasIsFilled=true;

      if(model==VBFZ)
	{
	  TPaveText *pave = new TPaveText(0.7,0.75,0.9,0.82,"NDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(32);
	  pave->SetTextFont(42);
	  pave->SetTextAlign(21);
	  char buf[100];
	  sprintf(buf,"#chi^{2}/ndof : %3.2f", shape.data->Chi2Test(mc,"WWCHI2/NDF") );
	  pave->AddText(buf);
	  //sprintf(buf,"Prob: %3.3f", shape.data->Chi2Test(mc,"WW"));
	  //pave->AddText(buf);
	  sprintf(buf,"K-S prob: %3.2f", shape.data->KolmogorovTest(mc,"") );
	  pave->AddText(buf);
	  pave->Draw();
	}
    }

  std::map<TString , TH1 *>::const_iterator it=shape.signal.begin();
  if(model==VBFZ) it++;
  for(; it!= shape.signal.end(); it++)
    {
      TH1 *h=it->second;
      h->Draw(canvasIsFilled ? "histsame" : "hist");
      legA->AddEntry(h,h->GetTitle(),"L");
      canvasIsFilled=true;
    }
    
  TPaveText* T = new TPaveText(0.12,0.93,0.6,0.97, "brNDC");
  T->SetFillColor(0);
  T->SetFillStyle(0);  
  T->SetLineColor(0);
  T->SetTextAlign(11);
  char Buffer[1024]; 
  if(is2011)
    sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5051./1000);
  else
    sprintf(Buffer, "CMS preliminary, #sqrt{s}=8 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 19600./1000);
  T->AddText(Buffer);
  T->SetBorderSize(0);
  T->Draw("same");
  
  legA->SetFillColor(0); 
  legA->SetFillStyle(0); 
  legA->SetLineColor(0);
  legA->SetTextFont(42);
  legA->SetTextAlign(12);
  if(model==VBFZ)  legA->SetNColumns(5);
  else             legA->SetNColumns(3);
  legA->SetBorderSize(0);
  legA->Draw("same");
  

  //ratio canvas
  TH1 *ratio=0;
  TGraphAsymmErrors *denRelUnc=0;
  if(shape.data && mc)
    {
      c1->cd();
      TPad* t2 = new TPad("t2","t2", 0.0, 0.0, 1.0, 0.2);     t2->Draw();
      t2->cd();  t2->SetTopMargin(0);   t2->SetBottomMargin(0.5);
      ratio = (TH1*)shape.data->Clone("RatioHistogram");
      ratio->SetDirectory(0);
      ratio->Divide(mc);
      
      TH1D *denRelUncH=(TH1D *) mc->Clone("mcrelunc");
      denRelUncH->Reset("ICE");       
      denRelUncH->Draw();
      
      denRelUnc=new TGraphAsymmErrors;
      denRelUnc->SetLineColor(1);
      denRelUnc->SetFillStyle(3001);
      denRelUnc->SetFillColor(kGray);
      denRelUnc->SetMarkerColor(1);
      denRelUnc->SetMarkerStyle(1);
      for(int ip=0; ip<mcgr->GetN(); ip++)
	{
	  Double_t x,y;
	  mcgr->GetPoint(ip,x,y);
	  Double_t xLo=mcgr->GetErrorXlow(ip);
	  Double_t xHi=mcgr->GetErrorXhigh(ip);
	  Double_t yLo=y!=0?mcgr->GetErrorYlow(ip)/y:0.;
	  Double_t yHi=y!=0?mcgr->GetErrorYhigh(ip)/y:0.;
	  denRelUnc->SetPoint(ip,x,1.0);
	  denRelUnc->SetPointError(ip,xLo,xHi,yLo,yHi);
	}
      denRelUnc->Draw("3");
          
      float xmin=denRelUncH->GetXaxis()->GetXmin();
      float xmax=denRelUncH->GetXaxis()->GetXmax();
      TLine *l = new TLine(xmin,0.5,xmax,0.5);
      l->SetLineColor(kRed);
      l->Draw();
      l = new TLine(xmin,1.5,xmax,1.5);
      l->SetLineColor(kRed);
      l->Draw();

      ratio->Draw("e1 same");
      

      float yscale = (1.0-0.2)/(0.18-0);       
      denRelUncH->GetYaxis()->SetTitle("Data/#Sigma Bckg");
      denRelUncH->SetMinimum(0.4);
      denRelUncH->SetMaximum(1.6);
      denRelUncH->GetXaxis()->SetTitle("");
      denRelUncH->GetXaxis()->SetTitleOffset(1.3);
      denRelUncH->GetXaxis()->SetLabelSize(0.033*yscale);
      denRelUncH->GetXaxis()->SetTitleSize(0.036*yscale);
      denRelUncH->GetXaxis()->SetTickLength(0.03*yscale);
      denRelUncH->GetYaxis()->SetTitleOffset(0.3);
      denRelUncH->GetYaxis()->SetNdivisions(5);
      denRelUncH->GetYaxis()->SetLabelSize(0.033*yscale);
      denRelUncH->GetYaxis()->SetTitleSize(0.036*yscale);
    }
   
  c1->cd();
  c1->Modified();
  c1->Update();
  if(SaveName.BeginsWith("_")) SaveName="inc"+SaveName;
  TString outName(SaveName);
  if(is2011) outName += "_2011";
  c1->SaveAs(outDir+"/"+outName+".png");
  c1->SaveAs(outDir+"/"+outName+".pdf");
  c1->SaveAs(outDir+"/"+outName+".C");
  //report << "<img src=\"" << outName << ".png\"/>\n" << endl;


  if(!SaveName.Contains("hardpt") && !SaveName.Contains("dphijj") &&
     !SaveName.Contains("jetdeta") && !SaveName.Contains("cjv")) return;
  
  //subtract data
  c1->Clear();
  c1->SetWindowSize(600,600);
  c1->SetLogy(false);
  
  TLegend* legB  = new TLegend(0.15,0.85,0.9,0.92);

  TH1 *diff = (TH1*)shape.data->Clone("DiffHistogram");
  diff->SetDirectory(0);
  diff->Add(vbfmc,-1);

  TH1 *vbfmcwithsyst=(TH1 *)vbfmc->Clone("vbfmcwithsyst");
  for(int ibin=1; ibin<=vbfmcwithsyst->GetXaxis()->GetNbins(); ibin++)
    {
      float unc=pow(denRelUnc->GetErrorY(ibin-1)*vbfmcwithsyst->GetBinContent(ibin,1),2);
      unc += pow(vbfmcwithsyst->GetBinError(ibin),2);
      vbfmcwithsyst->SetBinError(ibin,sqrt(unc));
    }

  TH1 *diffWithSyst = (TH1*)shape.data->Clone("DiffWithSystHistogram");
  diffWithSyst->SetDirectory(0);
  diffWithSyst->Add(vbfmcwithsyst,-1);
  //  if(doRebin) diffWithSyst->Rebin(2);
  TGraphErrors *diffWithSystGr=new TGraphErrors(diffWithSyst);
  diffWithSystGr->SetFillStyle(3427);
  diffWithSystGr->SetFillColor(kGray+1);
  diffWithSystGr->SetMarkerStyle(1);
  diffWithSystGr->SetMarkerColor(kGray+1);
  for(int ibin=1; ibin<=diffWithSyst->GetXaxis()->GetNbins(); ibin++){
    diffWithSystGr->SetPointError(ibin-1,diffWithSyst->GetXaxis()->GetBinWidth(ibin)/2.0, diffWithSyst->GetBinError(ibin));
  }
  
  //if(doRebin) diff->Rebin(2);
  diffWithSystGr->Draw("a20");
  diffWithSystGr->GetXaxis()->SetRangeUser(vbfmcwithsyst->GetXaxis()->GetXmin(),vbfmcwithsyst->GetXaxis()->GetXmax());
  diffWithSystGr->GetXaxis()->SetTitle(diffWithSyst->GetXaxis()->GetTitle());
  diffWithSystGr->GetYaxis()->SetTitle(diffWithSyst->GetYaxis()->GetTitle());
  diff->Draw("same");
  legB->AddEntry(diff,"residuals","P");

  //signal
  TH1 *hframe=0;
  for(std::map<TString , TH1 *>::const_iterator sit=shape.signal.begin(); sit!= shape.signal.end(); sit++)
    {
      TH1 *h=(TH1 *)sit->second->Clone();
      h->SetFillStyle(3001);
      h->Draw(canvasIsFilled ? "histsame" : "hist");
      //if(doRebin) h->Rebin(2);
      if(!canvasIsFilled) hframe=h;
      legB->AddEntry(h,h->GetTitle(),"F");
    }

  legB->SetNColumns(2);
  legB->SetFillColor(0); 
  legB->SetFillStyle(0); 
  legB->SetLineColor(0);
  legB->SetTextFont(42);
  legB->SetTextAlign(11); 
  legB->SetTextSize(0.05);
  legB->Draw("same");

  T->SetBorderSize(0);
  T->Draw();
  SaveName.ReplaceAll("hardpt","hardpt_sub");
  SaveName.ReplaceAll("dphijj","dphijj_sub");
  SaveName.ReplaceAll("deta","deta_sub");
  SaveName.ReplaceAll("cjv","cjv_sub");
  c1->SaveAs(outDir+"/"+SaveName+".png");
  c1->SaveAs(outDir+"/"+SaveName+".pdf");
  c1->SaveAs(outDir+"/"+SaveName+".C");
}


void setTDRStyle() {
  tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  // tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.02);

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.25);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

  gStyle->SetErrorX(0.5);
  gStyle->SetPadTopMargin   (0.06);
  gStyle->SetPadBottomMargin(0.12);
  //  gStyle->SetPadRightMargin (0.16);
  tdrStyle->SetPadRightMargin(0.05);
  gStyle->SetPadLeftMargin  (0.14);
  gStyle->SetTitleSize(0.04, "XYZ");
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetTitleYOffset(1.45);
  gStyle->SetPalette(1);
  gStyle->SetNdivisions(505);

}
