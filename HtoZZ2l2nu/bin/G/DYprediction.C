#include "TFile.h"
#include "TH2F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TStyle.h"

#include <iostream> 
#include <string> 
#include <map>

using namespace std;

TStyle *tdrStyle;

struct Shape_t
{
  TH1* data, *totalBckg;
  std::vector<TH1 *> bckg, signal;
  //the key corresponds to the proc name
  //the key is the name of the variation: e.g. jesup, jesdown, etc.
  std::map<TString,std::vector<std::pair<TString, TH1*> > > bckgVars, signalVars;
};

void showShape(const Shape_t &shape,TString SaveName);
void setTDRStyle();

//
void getDYprediction(bool doSubtraction=true)
{
  setTDRStyle();

  //  TString llFile="../../test/results/plotter.root";
  // TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2012/nvtx/plotter.root";
  TString llFile="../../test/results/plotter_2012.root";
  TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2012/nvtx/plotter.root";

  string ch[]     = {"ee","mumu"};
  const size_t nchs=sizeof(ch)/sizeof(string);
  string histos[] = {"met_met","mt","mindphijmet","met_redMet","pfvbfpremjj","pfvbfcandjetdeta","pfvbfmjj","pfvbfcjv"};
  const size_t nhistos=sizeof(histos)/sizeof(string);
  string dilcats[]= {"eq0jets","eq1jets","geq2jets","vbf",""};//,"novbf"};
  const size_t ndilcats=sizeof(dilcats)/sizeof(string);//-1;
  string dilprocs[]={"WW#rightarrow 2l2#nu","ZZ","WZ#rightarrow 3l#nu","t#bar{t}","Single top","W#rightarrow l#nu","data"};
  Int_t dilColors[]={592, 590, 596, 8, 824, 809, 1 };
  string dilSignal("H(300)#rightarrow ZZ");
  int cutIndex=411;
  const size_t ndilprocs=sizeof(dilprocs)/sizeof(string);
  string gcats[]= {"eq0jets",/*"eq0softjets",*/"eq1jets","eq2jets","geq3jets","vbf",""};   // -> 2+3 jets to be merged
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

		      string hsigname=dilSignal+"/"+ch[ich]+dilcats[icat]+"_"+histos[ih];
		      TH1 *hsig=(TH1 *)llIn->Get(hsigname.c_str());
		      if(hsig==0) { cout << hsigname << endl; continue; }
		      //    if(histos[ih]=="mt_shapes")
		      //			hsig = ((TH2 *)hsig)->ProjectionY((string(hsig->GetName())+"proj").c_str(),cutIndex,cutIndex);
		      hsig->SetTitle(dilSignal.c_str());
		      hsig->SetDirectory(0);
		      hsig->SetLineColor(1);
		      hsig->SetLineWidth(2);
		      m_shape.signal.push_back(hsig);
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
		      m_shape.bckg.push_back(h);
		    }
		}

	      //save
	      string key=ch[ich]+dilcats[icat]+"_"+histos[ih];
	      cout << "Shapes: " << key << endl;
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
		  cout << hname << endl;
		  TH1 *h=(TH1 *)gIn->Get(hname.c_str());
		  if(h==0) { cout << gprocs[iproc] << endl; continue; }
		  cout << h->Integral() << endl;
		  if(histos[ih]=="mt_shapes") cout << h->GetXaxis()->GetNbins() << endl;
		    //		    h = ((TH2 *)h)->ProjectionY((string(h->GetName())+"proj").c_str(),cutIndex,cutIndex);
		  
		  //detach and save
		  h->SetDirectory(0);
		  if(gprocs[iproc].find("data") != string::npos) { h->SetTitle("#splitline{Instr. bkg}{(data)}"); m_shape.data=h; cout << "here" << endl; }
		  else
		    {
		      cout << "or here?" << endl;
		      if(m_shape.bckg.size()==0 && h->Integral())
			{
			  string newname(ch[ich]+gcats[icat]+histos[ih]+"contbckg");
			  m_shape.totalBckg=(TH1*)h->Clone(newname.c_str()); 
			  m_shape.totalBckg->Reset("ICE");
			  m_shape.totalBckg->SetDirectory(0);
			}
		      if(h->Integral())m_shape.totalBckg->Add(h);
		      m_shape.bckg.push_back(h);
		      cout << "not here?" << endl;
		    }
		}
	      
	      //save
	      string normKey(ch[ich]);
	      cout << "Saving" << normKey << endl;
	      TH1 *normH=0;
	      if(gcats[icat]=="geq3jets")
		{
		  Shape_t &shape2jets=gShapesMap[ch[ich]+"geq2jets"+"_"+histos[ih]];
		  shape2jets.data->Add(m_shape.data);
		  shape2jets.totalBckg->Add(m_shape.totalBckg);
		  normKey += "geq2jets";
		  normH=shape2jets.data;
		}
	      else if(gcats[icat]=="eq0softjets")
		{
		  Shape_t &shape0jets=gShapesMap[ch[ich]+"eq0jets"+"_"+histos[ih]];
		  shape0jets.data->Add(m_shape.data,-1);
		  shape0jets.totalBckg->Add(m_shape.totalBckg,-1);
		  normKey += "eq0jets";
		  normH=shape0jets.data;
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
      /*
      //create the no VBF category
      for(size_t ih=0; ih<nhistos; ih++)
	{
	  Shape_t noVBFDil_shape,noVBFG_shape; 
	  for(size_t icat=0; icat<ndilcats; icat++)
	    {
	      if(dilcats[icat]=="vbf") continue;
	      string key=ch[ich]+dilcats[icat]+"_"+histos[ih];
	      
	      //gamma sample
	      if(gShapesMap.find(key) != gShapesMap.end())
		{
		  Shape_t &gShape   = gShapesMap[key];
		  if(noVBFG_shape.bckg.size()==0)
		    {
		      string newname(ch[ich]+"novbf"+histos[ih]+"gammadata");
		      noVBFG_shape.data = (TH1 *) gShape.data->Clone(newname.c_str());
		      noVBFG_shape.data->Reset("ICE");
		      noVBFG_shape.data->SetDirectory(0);
		      
		      newname=ch[ich]+"novbf"+histos[ih]+"totalbckg";
		      noVBFG_shape.totalBckg=(TH1*) gShape.totalBckg->Clone(newname.c_str());
		      noVBFG_shape.totalBckg->Reset("ICE");
		      noVBFG_shape.totalBckg->SetDirectory(0);
		      for(size_t ibckg=0; ibckg<gShape.bckg.size(); ibckg++)
			{
			  newname=ch[ich]+"novbf"+histos[ih]+gShape.bckg[ibckg]->GetName();
			  TH1 *h=(TH1*) gShape.bckg[ibckg]->Clone(newname.c_str());
			  h->Reset("ICE");
			  h->SetDirectory(0);
			  noVBFG_shape.bckg.push_back(h);
			}
		    }
		  noVBFG_shape.data->Add(gShape.data);
		  noVBFG_shape.totalBckg->Add(gShape.totalBckg);
		  for(size_t ibckg=0; ibckg<gShape.bckg.size(); ibckg++) noVBFG_shape.bckg[ibckg]->Add(gShape.bckg[ibckg]);
		}

	      //similar for the dilepton
	      if(shapesMap.find(key) != shapesMap.end())
		{
		  Shape_t &dilShape = shapesMap[key];
		  if(noVBFDil_shape.bckg.size()==0)
                    {
                      string newname(ch[ich]+"novbf"+histos[ih]+"data");
                      noVBFDil_shape.data = (TH1 *) dilShape.data->Clone(newname.c_str());
                      noVBFDil_shape.data->Reset("ICE");
                      noVBFDil_shape.data->SetDirectory(0);

		      newname=ch[ich]+"novbf"+histos[ih]+"totalbckg";
		      noVBFDil_shape.totalBckg=(TH1*) dilShape.totalBckg->Clone(newname.c_str());
		      noVBFDil_shape.totalBckg->Reset("ICE");
		      noVBFDil_shape.totalBckg->SetDirectory(0);
		      for(size_t ibckg=0; ibckg<dilShape.bckg.size(); ibckg++)
			{
                          newname=ch[ich]+"novbf"+histos[ih]+dilShape.bckg[ibckg]->GetName();
                          TH1 *h=(TH1*) dilShape.bckg[ibckg]->Clone(newname.c_str());
                          h->Reset("ICE");
                          h->SetDirectory(0);
                          noVBFDil_shape.bckg.push_back(h);
                        }

		      for(size_t isig=0; isig<dilShape.signal.size(); isig++)
			{
                          newname=ch[ich]+"novbf"+histos[ih]+dilShape.signal[isig]->GetName();
                          TH1 *h=(TH1*) dilShape.signal[isig]->Clone(newname.c_str());
                          h->Reset("ICE");
                          h->SetDirectory(0);
                          noVBFDil_shape.signal.push_back(h);
                        }
		    }
                  noVBFDil_shape.data->Add(dilShape.data);
                  noVBFDil_shape.totalBckg->Add(dilShape.totalBckg);
                  for(size_t ibckg=0; ibckg<dilShape.bckg.size(); ibckg++) noVBFDil_shape.bckg[ibckg]->Add(dilShape.bckg[ibckg]);
		  for(size_t isig=0; isig<dilShape.signal.size(); isig++) noVBFDil_shape.signal[isig]->Add(dilShape.signal[isig]);
		}
	    }
	  
	  string key=ch[ich]+"novbf_"+histos[ih];
	  shapesMap[key]=noVBFDil_shape;
	  gShapesMap[key]=noVBFG_shape;
	}
      */
    }
  /*
  //create the inclusive
  for(size_t ih=0; ih<nhistos; ih++)
    {
      for(size_t icat=0; icat<ndilcats+1; icat++)
	{
	  Shape_t incG_shape, incDil_shape;
	  for(size_t ich=0; ich<nchs; ich++)
	    {
	      string key=ch[ich]+dilcats[icat]+"_"+histos[ih];
	     
	      //gamma sample
	      if(gShapesMap.find(key) != gShapesMap.end())
		{
		  Shape_t &gShape   = gShapesMap[key];
		  if(incG_shape.bckg.size()==0)
		    {
		      string newname(ch[ich]+histos[ih]+"gammadata");
		      incG_shape.data = (TH1 *) gShape.data->Clone(newname.c_str());
		      incG_shape.data->Reset("ICE");
		      incG_shape.data->SetDirectory(0);

		      newname=ch[ich]+histos[ih]+"totalbckg";
		      incG_shape.totalBckg=(TH1*) gShape.totalBckg->Clone(newname.c_str());
		      incG_shape.totalBckg->Reset("ICE");
		      incG_shape.totalBckg->SetDirectory(0);
		      for(size_t ibckg=0; ibckg<gShape.bckg.size(); ibckg++)
			{
			  newname=ch[ich]+histos[ih]+gShape.bckg[ibckg]->GetName();
			  TH1 *h=(TH1*) gShape.bckg[ibckg]->Clone(newname.c_str());
			  h->Reset("ICE");
			  h->SetDirectory(0);
			  incG_shape.bckg.push_back(h);
			}
		    }
		  incG_shape.data->Add(gShape.data);
		  incG_shape.totalBckg->Add(gShape.totalBckg);
		  for(size_t ibckg=0; ibckg<gShape.bckg.size(); ibckg++) incG_shape.bckg[ibckg]->Add(gShape.bckg[ibckg]);
		}

	      //similar for the dilepton
	      if(shapesMap.find(key) != shapesMap.end())
		{
		  Shape_t &dilShape = shapesMap[key];
		  if(incDil_shape.bckg.size()==0)
                    {
                      string newname(ch[ich]+histos[ih]+"data");
                      incDil_shape.data = (TH1 *) dilShape.data->Clone(newname.c_str());
                      incDil_shape.data->Reset("ICE");
                      incDil_shape.data->SetDirectory(0);

		      newname=ch[ich]+histos[ih]+"totalbckg";
		      incDil_shape.totalBckg=(TH1*) dilShape.totalBckg->Clone(newname.c_str());
		      incDil_shape.totalBckg->Reset("ICE");
		      incDil_shape.totalBckg->SetDirectory(0);
		      for(size_t ibckg=0; ibckg<dilShape.bckg.size(); ibckg++)
			{
                          newname=ch[ich]+histos[ih]+dilShape.bckg[ibckg]->GetName();
                          TH1 *h=(TH1*) dilShape.bckg[ibckg]->Clone(newname.c_str());
                          h->Reset("ICE");
                          h->SetDirectory(0);
                          incDil_shape.bckg.push_back(h);
                        }
		      for(size_t isig=0; isig<dilShape.signal.size(); isig++)
			{
                          newname=ch[ich]+"novbf"+histos[ih]+dilShape.signal[isig]->GetName();
                          TH1 *h=(TH1*) dilShape.signal[isig]->Clone(newname.c_str());
                          h->Reset("ICE");
                          h->SetDirectory(0);
                          incDil_shape.signal.push_back(h);
                        }
                    }
                  incDil_shape.data->Add(dilShape.data);
                  incDil_shape.totalBckg->Add(dilShape.totalBckg);
                  for(size_t ibckg=0; ibckg<dilShape.bckg.size(); ibckg++) incDil_shape.bckg[ibckg]->Add(dilShape.bckg[ibckg]);
		  for(size_t isig=0; isig<dilShape.signal.size(); isig++) incDil_shape.signal[isig]->Add(dilShape.signal[isig]);
		}
	    }
	  
	  string key=dilcats[icat]+"_"+histos[ih];
	  cout << key << endl;
	  shapesMap[key]=incDil_shape;
	  gShapesMap[key]=incG_shape;
	}
	}
  */

  gIn->Close();
  cout << "Here?" << endl;

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

     cout << sf << " " << normKey << " " << it->first << endl;
     if(sf==0) continue;
     if(gShapesMap.find(it->first)==gShapesMap.end()) cout << "BUG: " << it->first << " not found in gamma sample..." << endl;
     Shape_t &gShape=gShapesMap[it->first];
     TH1 *corrGammaH=(TH1 *)gShape.data->Clone((it->first+"dy").c_str());
     corrGammaH->SetDirectory(0);

     if(doSubtraction) {
       corrGammaH->Add( gShape.totalBckg,-1);
       for(int ibin=1; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	 {
	   Double_t val=corrGammaH->GetBinContent(ibin);
	   if(val<0)  corrGammaH->SetBinContent(ibin,0);
	 }
     }
     /*
     else if(normKey.find("met_met")!= string::npos)
       {
	 int startbin=corrGammaH->GetXaxis()->FindBin(60);
	 for(int ibin=startbin; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	   {
	     double val=corrGammaH->GetBinContent(ibin);
	     // double err=corrGammaH->GetBinError(ibin);
	     corrGammaH->SetBinContent(ibin,val*0.5);
	     corrGammaH->SetBinError(ibin,val);
	   }
       }
     */
     corrGammaH->Scale(sf);
     corrGammaH->SetFillColor(831);
     it->second.totalBckg->Add(corrGammaH);
     it->second.bckg.push_back(corrGammaH);
     if(it->first.find("mindphijmet")!=string::npos)
       {
	 double sfcorr = it->second.data->Integral(1,it->second.data->FindBin(0.5))/it->second.totalBckg->Integral(1,it->second.totalBckg->FindBin(0.5));
	 // cout << sfcorr << " applied " << endl; 
	 it->second.totalBckg->Add(corrGammaH,-1);
	 corrGammaH->Scale(sfcorr);
	 it->second.totalBckg->Add(corrGammaH);
       }
     showShape(it->second,it->first);
     
     gOutDir->cd();
     corrGammaH->Write(it->first.c_str());
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
      for(size_t i=0; i<shape.bckg.size(); i++)
        {
          if(shape.bckg[i]->Integral()<=0) continue;
          TString itit(shape.bckg[i]->GetTitle());
 	  if(itit.Contains("t#bar{t}") || itit.Contains("Single top") || itit.Contains("W#rightarrow") || itit.Contains("WW") )
 	    {
 	      if(nrbBckg==0) { nrbBckg = (TH1 *) shape.bckg[i]->Clone("nrb"); nrbBckg->SetTitle("#splitline{Non resonant}{bkg (data)}"); legA->AddEntry(nrbBckg,nrbBckg->GetTitle(),"F"); nrbBckg->SetDirectory(0); stack->Add(nrbBckg,"HIST"); }
 	      else           { nrbBckg ->Add( shape.bckg[i] ); }
 	    }
	  else
	    {
	      stack->Add(shape.bckg[i],"HIST");
	      legA->AddEntry(shape.bckg[i],shape.bckg[i]->GetTitle(),"F");
	    }
	}
      
      stack->Draw();
      stack->GetXaxis()->SetTitle(mc->GetXaxis()->GetTitle());
      stack->GetYaxis()->SetTitle(mc->GetYaxis()->GetTitle());
      //      stack->GetXaxis()->SetRangeUser(0,350);
      stack->SetMinimum(1e-1);//mc->GetMinimum());
      stack->SetMaximum(1.1*mc->GetMaximum());

      mc->Reset("ICE");
      mc->Add((TH1 *)stack->GetStack()->At( stack->GetStack()->GetEntriesFast()-1) );
      mc->SetDirectory(0);
      mc->SetFillStyle(3001);//3427);
      mc->SetFillColor(kGray+1);
      mc->SetMarkerStyle(1);
      mc->Draw("e4same");

      canvasIsFilled=true;
    }
  if(shape.data)
    {
      shape.data->Draw(canvasIsFilled ? "E1same" : "E1");
      legA->AddEntry(shape.data,shape.data->GetTitle(),"P");
      canvasIsFilled=true;
    }
  for(size_t ip=0; ip<shape.signal.size(); ip++)
    {
      shape.signal[ip]->Draw(canvasIsFilled ? "histsame" : "hist");
      legA->AddEntry(shape.signal[ip],shape.signal[ip]->GetTitle(),"L");
      canvasIsFilled=true;
    }


  TPaveText* T = new TPaveText(0.1,0.995,0.9,0.96, "NDC");
  T->SetFillColor(0);
  T->SetFillStyle(0);  T->SetLineColor(0);
  T->SetTextAlign(22);
  //  char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5045./1000);
  char Buffer[1024]; sprintf(Buffer, "CMS preliminary, #sqrt{s}=8 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 1616./1000);
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
