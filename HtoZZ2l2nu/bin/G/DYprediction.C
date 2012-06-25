#include "TFile.h"
#include "TH2F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TGraphErrors.h"

#include <iostream> 
#include <string> 
#include <map>

using namespace std;

TStyle *tdrStyle;

struct Shape_t
{
  TH1* data, *totalBckg;
  std::map<TString, TH1 *> bckg, signal;
};

void showShape(const Shape_t &shape,TString SaveName);
void setTDRStyle();


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
      if(a.bckg.find(it->first)!=b.bckg.end()) continue;
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
enum SubtractionTypes {NOSUBTRACTION, HALVE, EWKSUBTRACTION};
void getDYprediction(int subtractType=NOSUBTRACTION)
{
  setTDRStyle();

  TString llFile="../../test/results/plotter_2012.root";
  TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2012/nvtx/plotter.root";
  
  //TString llFile="../../test/results/plotter_2011.root";
  // TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2011/nvtx/plotter.root";
  
  string ch[]     = {"ee","mumu"};
  const size_t nchs=sizeof(ch)/sizeof(string);
  string histos[] = {"met_met","mt",
		     //		     "mindphijmet",
		     // "pfvbfpremjj",
		     // "pfvbfcandjetdeta","pfvbfmjj","pfvbfcjv", "pfvbfhardpt"
		     //		     "mt_shapes"
  };
  const size_t nhistos=sizeof(histos)/sizeof(string);
  string dilcats[]= {"eq0jets","eq1jets","geq2jets","vbf",""};
  const size_t ndilcats=sizeof(dilcats)/sizeof(string);
  string dilprocs[]={"WW#rightarrow 2l2#nu","ZZ","WZ#rightarrow 3l#nu","t#bar{t}","Single top","W#rightarrow l#nu","data"};
  Int_t dilColors[]={592, 590, 596, 8, 824, 809, 1 };
  const size_t ndilprocs=sizeof(dilprocs)/sizeof(string);
  string dilSignal[]={"ggH(350)#rightarrow ZZ","qqH(350)#rightarrow ZZ"};
  const size_t nDilSignals=sizeof(dilSignal)/sizeof(string);
  string gcats[]= {"eq0jets","eq0softjets","eq1jets","eq2jets","geq3jets","vbf",""};   // -> 2+3 jets to be merged, 0 soft jets to be subtracted
  const size_t ngcats=sizeof(gcats)/sizeof(string);
  string gprocs[]={"Z#gamma#rightarrow#nu#nu#gamma","W#gamma#rightarrowl#nu#gamma","W#rightarrow l#nu","data (#gamma)"};
  const size_t ngprocs=sizeof(gprocs)/sizeof(string);

  std::map<string,Shape_t> shapesMap,gShapesMap;

  //get histos from files
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
		  if(h==0) continue; //{ cout << hname << endl; continue; }
		  if(histos[ih]=="mt_shapes") cout << h->GetXaxis()->GetNbins() << endl;
		  //	  if(histos[ih]=="mt_shapes")
		    //	    h = ((TH2 *)h)->ProjectionY((string(h->GetName())+"proj").c_str(),cutIndex,cutIndex);
		  h->SetTitle(dilprocs[iproc].c_str());
		  
		  //detach and save (also signal)
		  h->SetDirectory(0);
		  if(dilprocs[iproc].find("data") != string::npos) 
		    {
		      m_shape.data=h;

		      for(size_t isig=0; isig<nDilSignals; isig++)
			{
			  string hsigname = dilSignal[isig]+"/"+ch[ich]+dilcats[icat]+"_"+histos[ih];
			  TH1 *hsig       = (TH1 *)llIn->Get(hsigname.c_str());
			  if(hsig==0) { cout << hsigname << endl; continue; }
			  hsig->SetTitle(dilSignal[isig].c_str());
			  hsig->SetDirectory(0);
			  hsig->SetLineColor(1);
			  hsig->SetLineStyle(1+isig);
			  hsig->SetLineWidth(2);
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

  //add the gamma estimation
  std::map<string,float> scaleFactors;
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
		  if(h==0) continue;

		  //detach and save
		  h->SetDirectory(0);
		  if(gprocs[iproc].find("data") != string::npos) { h->SetTitle("#splitline{Instr. bkg}{(data)}"); m_shape.data=h; }
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
	      
	      //finalize shape
	      string normKey(ch[ich]);
	      TH1 *normH=0;
	      if(gcats[icat]=="geq3jets" || gcats[icat]=="eq0softjets")
		{
		  int sign(+1);
		  TString normKeyPostFix("geq2jets");
		  if(gcats[icat]=="eq0softjets") { sign=-1; normKeyPostFix="eq0jets"; }
		  normKey += normKeyPostFix;
		  
		  string keyToGet(ch[ich]+normKeyPostFix+"_"+histos[ih]);
		  if(gShapesMap.find(keyToGet)!= gShapesMap.end())
		    {
		      Shape_t &shapeToCorrect=gShapesMap[keyToGet];
		      addToShape(shapeToCorrect,m_shape,sign);
		      normH=shapeToCorrect.data;
		    }
		}
	      else 
		{
		  string key=ch[ich]+gcats[icat]+"_"+histos[ih];
		  
		  if(gcats[icat]=="eq2jets") 
		    {
		      key=ch[ich]+"geq2jets"+"_"+histos[ih];
		      normKey += "geq2jets";
		    }
		  else
		    normKey += gcats[icat];
		 
		  gShapesMap[key]=m_shape;
		  normH=m_shape.data;
		}
	      
	      //normalization factor (from MET<50)
	      if(histos[ih].find("met_met")==string::npos) continue;
	      int normBin=normH->GetXaxis()->FindBin(50);
	      
	      Shape_t &dilMetShape = shapesMap[normKey+"_"+histos[ih]];
	      float sf=dilMetShape.data->Integral(1,normBin)/normH->Integral(1,normBin);

	      scaleFactors[normKey]=sf;
	    }
	}
    }
  gIn->Close();


  //
  TFile *gOut = TFile::Open("gamma_out.root","RECREATE");
  TDirectory *gOutDir = gOut->mkdir("Instr. background (data)");
  for(std::map<string,Shape_t>::iterator it=shapesMap.begin(); it!=shapesMap.end(); it++) 
   {
     string normKey("mumu"); 
     if(it->first.find("ee")!=string::npos)       normKey="ee";
     if(it->first.find("eq0jets_")!=string::npos)  normKey+="eq0jets";
     if(it->first.find("eq1jets_")!=string::npos)  normKey+="eq1jets";
     if(it->first.find("geq2jets_")!=string::npos) normKey+="geq2jets";
     if(it->first.find("vbf_")!=string::npos)      normKey+="vbf";
     float sf = scaleFactors[normKey];
     
     if(sf==0) continue;
     if(gShapesMap.find(it->first)==gShapesMap.end()) cout << "BUG: " << it->first << " not found in gamma sample..." << endl;
     Shape_t &gShape=gShapesMap[it->first];
     TH1 *corrGammaH=(TH1 *)gShape.data->Clone((it->first+"dy").c_str());
     corrGammaH->SetDirectory(0);

     
     //do the subtraction for met related variables when MET>70
     if(it->first.find("mt_shapes")!= string::npos || it->first.find("met_") != string::npos)
       {
	 bool isTH2( corrGammaH->InheritsFrom("TH2") );
	 if(subtractType==HALVE) {
	   int fbin( isTH2 ? 1 : corrGammaH->GetXaxis()->FindBin(70) );
	   for(int ibin=fbin; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	     {
	       if(isTH2)
		 {
		   for(int jbin=1; jbin<=corrGammaH->GetYaxis()->GetNbins(); jbin++)
		     {
		       Double_t val=corrGammaH->GetBinContent(ibin,jbin)/2;
		       Double_t valerr=corrGammaH->GetBinError(ibin,jbin)/2;
		       corrGammaH->SetBinContent(ibin,jbin,val);
		       corrGammaH->SetBinError(ibin,jbin,valerr);
		     }
		 }
	       else
		 {
		   Double_t val=corrGammaH->GetBinContent(ibin)/2;
		   Double_t valerr=corrGammaH->GetBinError(ibin)/2;
		   corrGammaH->SetBinContent(ibin,val);
		   corrGammaH->SetBinError(ibin,valerr);
		 }
	     }
	 }

	 if(subtractType==EWKSUBTRACTION)
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
     corrGammaH->SetFillColor(831);
     it->second.totalBckg->Add(corrGammaH);
     it->second.bckg["Instr. background (data)"]=corrGammaH;
     showShape(it->second,"final_"+it->first);
     gOutDir->cd();
     corrGammaH->Write(it->first.c_str());
     if(it->first.find("mumu")!= string::npos)
       {
	 TString keyToGet(it->first);
	 keyToGet=keyToGet.ReplaceAll("mumu","ee");
	 Shape_t &shapeToCorrect=shapesMap[keyToGet.Data()];
	 addToShape(shapeToCorrect,it->second,1);
	 keyToGet=keyToGet.ReplaceAll("ee","");
	 showShape(shapeToCorrect,keyToGet.Data());
       }
     
   }
  gOut->Close();

}

//
void showShape(const Shape_t &shape,TString SaveName)
{
  if(shape.data->InheritsFrom("TH2")) return;

  TCanvas* c1 = new TCanvas(SaveName,SaveName,800,800);
  c1->SetWindowSize(600,600);
  c1->cd();
  
  TPad* t1 = new TPad("t1","t1", 0.0, 0.20, 1.0, 1.0);  t1->Draw();  t1->cd();
  t1->SetLogy(true);
  TLegend* legA  = new TLegend(0.60,0.6,0.95,0.99, "NDC");
  legA->SetNColumns(2);

  bool canvasIsFilled(false);
  THStack *stack=0;
  TH1 *mc=0;
  if(shape.bckg.size())
    {
      mc=(TH1 *)shape.totalBckg->Clone("mc");
      stack = new THStack("stack","stack");
      TH1 *nrbBckg=0;
      for(std::map<TString,TH1 *>::const_reverse_iterator it = shape.bckg.rbegin(); it != shape.bckg.rend(); it++)
        {
	  TH1 *h = it->second;
          if(h->Integral()<=0) continue;
          TString itit(h->GetTitle());
 	  if(itit.Contains("t#bar{t}") || itit.Contains("Single top") || itit.Contains("W#rightarrow") || itit.Contains("WW") )
 	    {
 	      if(nrbBckg==0) { nrbBckg = (TH1 *) h->Clone("nrb"); nrbBckg->SetTitle("#splitline{Non resonant}{bkg (data)}"); legA->AddEntry(nrbBckg,nrbBckg->GetTitle(),"F"); nrbBckg->SetDirectory(0); stack->Add(nrbBckg,"HIST"); }
 	      else           { nrbBckg ->Add( h ); }
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
      stack->SetMinimum(1e-1);//mc->GetMinimum());
      if(SaveName.Contains("vbf") && !SaveName.Contains("novbf")) stack->SetMinimum(1e-2);
      stack->SetMaximum(1.1*mc->GetMaximum());

      mc->Reset("ICE");
      mc->Add((TH1 *)stack->GetStack()->At( stack->GetStack()->GetEntriesFast()-1) );
      mc->SetDirectory(0);

      TGraphErrors *mcgr=new TGraphErrors;
      for(int ibin=1; ibin<=mc->GetXaxis()->GetNbins(); ibin++)
	{
	  mcgr->SetPoint(ibin-1,mc->GetXaxis()->GetBinCenter(ibin),mc->GetBinContent(ibin));
	  mcgr->SetPointError(ibin-1,mc->GetXaxis()->GetBinWidth(ibin)/2,mc->GetBinError(ibin));
	}
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
    }

  //signal
  for(std::map<TString , TH1 *>::const_iterator it=shape.signal.begin(); it!= shape.signal.end(); it++)
    {
      TH1 *h=it->second;
      h->Draw(canvasIsFilled ? "histsame" : "hist");
      legA->AddEntry(h,h->GetTitle(),"L");
      canvasIsFilled=true;
    }



  TPaveText* T = new TPaveText(0.1,0.995,0.9,0.96, "NDC");
  T->SetFillColor(0);
  T->SetFillStyle(0);  T->SetLineColor(0);
  T->SetTextAlign(22);
  //   char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5045./1000);
    char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=8 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 3947./1000);
    T->AddText(Buffer);
  T->Draw("same");
  
  legA->SetFillColor(0); legA->SetFillStyle(0); legA->SetLineColor(0);
  legA->SetHeader("");
  legA->Draw("same");
  legA->SetTextFont(42);
  
  TH1 *ratio=0;
  if(shape.data && mc)
    {
      c1->cd();
      TPad* t2 = new TPad("t2","t2", 0.0, 0.0, 1.0, 0.2);     t2->Draw();
      t2->cd();
      //      t2->SetGridy(true);
      t2->SetTopMargin(0);   t2->SetBottomMargin(0.5);
      float yscale = (1.0-0.2)/(0.18-0);
      ratio = (TH1*)shape.data->Clone("RatioHistogram");
      ratio->SetDirectory(0);
      ratio->Divide(mc);
      ratio->GetYaxis()->SetTitle("Data/#Sigma Bckg");
      ratio->GetXaxis()->SetTitle("");
      ratio->SetMinimum(0);
      ratio->SetMaximum(2.2);
      ratio->GetXaxis()->SetTitleOffset(1.3);
      ratio->GetXaxis()->SetLabelSize(0.033*yscale);
      ratio->GetXaxis()->SetTitleSize(0.036*yscale);
      ratio->GetXaxis()->SetTickLength(0.03*yscale);
      ratio->GetYaxis()->SetTitleOffset(0.3);
      ratio->GetYaxis()->SetNdivisions(5);
      // ratio->GetXaxis()->SetRangeUser(0,350);
      ratio->GetYaxis()->SetLabelSize(0.033*yscale);
      ratio->GetYaxis()->SetTitleSize(0.036*yscale);
      ratio->Draw("E1");
    }
   
  c1->cd();
  c1->Modified();
  c1->Update();
  c1->SaveAs(SaveName+".png");
c1->SaveAs(SaveName+".pdf");
  // c1->SaveAs(SaveName+".C");
  //  delete c1;
  //   if(mc)    delete mc;
  //   if(stack) delete stack;
  //   if(ratio) delete ratio;
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

}
