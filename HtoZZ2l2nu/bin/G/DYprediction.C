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

void showShape(const Shape_t &shape,TString SaveName,bool is2011,int model);
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
enum SubtractionTypes {NOSUBTRACTION, HALVE, EWKSUBTRACTION, EWKSUBTRACTIONHALVE};
enum ModelType { HZZ,ZZ,VBFZ};
void getDYprediction(int subtractType=NOSUBTRACTION,int model=VBFZ)
{
  setTDRStyle();
 

  TString llFile,gammaFile;
  std::vector<std::string> histos,dilSignal,dilcats,gcats;
  if(model==VBFZ) 
    {
      //      gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2012/nvtx/plotter.root";
      //      llFile="../../plotter_vbfz_2012.root";

      gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2011/nvtx/plotter.root";
      llFile="../../plotter_vbfz_2011.root";
      
      histos.push_back("pfpuloosevbfcandjetdeta");
      histos.push_back("pfpuloosevbfcandjet1pt");
      histos.push_back("pfpuloosevbfcandjet2pt");
      histos.push_back("pfpuloosevbfcandjet1eta");
      histos.push_back("pfpuloosevbfcandjet2eta");
      histos.push_back("pfpuloosevbfcjv");
      histos.push_back("pfpuloosevbfpremjj");
      histos.push_back("pfpuloosevbfmjj");
      histos.push_back("pfpuloosevbfdphijj");
      histos.push_back("pfpuloosevbfhardpt");
      histos.push_back("pfpuloosevbfmjj50");
      histos.push_back("pfpuloosevbfhardpt50");
      histos.push_back("pfpuloosevbfdphijj50");
      histos.push_back("dijet_mass_shapes");
      //histos.push_back("vbfz_mjj_shapes");

      //dilSignal.push_back("VBF Z");
      //dilSignal.push_back("VBF Z (interference)");
      dilSignal.push_back("#alpha^{4}_{EW}-ll");

      //dilcats.push_back("geq1jets");
      // dilcats.push_back("vbf");
      dilcats.push_back("");

      //gcats.push_back("eq1jets"); gcats.push_back("eq2jets"); gcats.push_back("geq3jets"); //to be merged
      // gcats.push_back("vbf");
      gcats.push_back("");
    }
  else
    {
      //FIXME
      //  TString llFile="../../test/results/plotter_2012.root";
      // TString llFile="/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_4_4_3/src/CMGTools/HtoZZ2l2nu/test/plotter2012.root";
      // TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2012/qt/plotter.root";
      //  TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2012/nvtx/plotter.root";
      //  TString llFile="../../plotter_zz_2012.root"; 
      
      //  TString llFile="../../test/results/plotter_2011.root";
      //  TString llFile="/afs/cern.ch/user/q/querten/workspace/public/HZZ2l2v/CMSSW_4_4_3/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root";
      //  TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2011/qt/plotter.root";
      //  TString gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2011/nvtx/plotter.root";
      // TString llFile="../../plotter_zz_2011.root";

      gammaFile="/afs/cern.ch/user/p/psilva/work/gamma/2011/nvtx/plotter.root";
      llFile="../../test/results/plotter.root";
      
      //   string histos[] = {//"met_met",
      //     //    "met_redMet"//,
      //     "met_min3Met"//,
      // 		     //"met_redMet15",
      // 		     //"met_redMet20",
      // 		     // "balance",
      // 		     //"mt",
      // 		     //"mindphijmet",
      // 		     // "pfvbfpremjj","pfvbfcandjetdeta","pfvbfmjj"
      // 		     //,"pfvbfcjv", "pfvbfhardpt",
      // 		     //"mt_shapes"//,
      // 		     //"zpt_shapes",
      // 		     // "met_shapes"
      dilSignal.push_back("ggH(350)#rightarrow ZZ");
      dilSignal.push_back("qqH(350)#rightarrow ZZ");
      //string dilcats[]= {"eq0jets","eq1jets","geq2jets","vbf",""};
      //string gcats[]= {"eq0jets","eq0softjets","eq1jets","eq2jets","geq3jets","vbf",""};   // -> 2+3 jets to be merged, 0 soft jets to be subtracted
    };


  bool is2011(llFile.Contains("2011"));

  string ch[]     = {"ee","mumu"};
  const size_t nchs=sizeof(ch)/sizeof(string);
  const size_t nhistos=histos.size();
  const size_t ndilcats=dilcats.size();
  string dilprocs[]={"WW#rightarrow 2l2#nu","ZZ","WZ#rightarrow 3l#nu","t#bar{t}","Single top","W#rightarrow l#nu","data"};
  Int_t dilColors[]={592, 590, 596, 8, 824, 809, 1 };
  const size_t ndilprocs=sizeof(dilprocs)/sizeof(string);
  const size_t nDilSignals=dilSignal.size();
  const size_t ngcats=gcats.size();
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
		  if(h==0)  { cout << "Missing " << hname << endl; continue; }
		  if(histos[ih]=="mt_shapes" || histos[ih]=="dijet_mass_shapes")
		    {
		      cout << h->GetXaxis()->GetNbins() << endl;
		      cout << h->InheritsFrom("TH2")<< endl;
		    }
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
			  if(hsig==0) { cout << "Missing " << hsigname << endl; continue; }
			  hsig->SetTitle(dilSignal[isig].c_str());
			  if(dilSignal[isig].find("VBF Z")!=string::npos)
			    {
			      if(dilSignal[isig].find("interf")==string::npos) hsig->SetTitle("#alpha_{EW}^{4}-Z");			  
			      else                                             hsig->SetTitle("#alpha_{EW}^{4}-ll");			  
			    }
			  hsig->SetDirectory(0);
			  hsig->SetLineColor(1);
			  hsig->SetLineStyle(1+isig);
			  hsig->SetLineWidth(2);
			  if(model==VBFZ)
			    {
			      if(ch[ich]=="ee")        hsig->Scale(is2011 ? 0.985 : 0.842);
			      else if(ch[ich]=="mumu") hsig->Scale(is2011 ? 1.106 : 0.558 );
			    }
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
		  if(h==0) { cout << hname <<endl; continue; }
		  		  
		  //detach and save
		  h->SetDirectory(0);
		  if(gprocs[iproc].find("data") != string::npos) {
		    if(model==VBFZ) h->SetTitle("QCD Z");
		    else            h->SetTitle("#splitline{Instr. bkg}{(data)}");
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

	      //finalize shape
	      string normKey(ch[ich]);
	      TH1 *normH=0;
	      if(model==VBFZ && (gcats[icat]=="eq2jets" || gcats[icat]=="geq3jets"))
		{
                  TString normKeyPostFix("geq1jets");
                  normKey += normKeyPostFix;
                  string keyToGet(ch[ich]+normKeyPostFix+"_"+histos[ih]);
                  if(gShapesMap.find(keyToGet)!= gShapesMap.end())
                    {
                      Shape_t &shapeToCorrect=gShapesMap[keyToGet];
                      addToShape(shapeToCorrect,m_shape);
		      cout << keyToGet << " " <<
			
endl;
                      normH=shapeToCorrect.data;
                    }
		}
	      else if(gcats[icat]=="geq3jets" || gcats[icat]=="eq0softjets")
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
		  if(model==VBFZ && gcats[icat]=="eq1jets") 
		    {
		      key=ch[ich]+"geq1jets"+"_"+histos[ih];
		      normKey += "geq1jets";
		    }		  
		  else if(gcats[icat]=="eq2jets") 
		    {
		      key=ch[ich]+"geq2jets"+"_"+histos[ih];
		      normKey += "geq2jets";
		    }
		  else
		    normKey += gcats[icat];
		 
		  gShapesMap[key]=m_shape;
		  normH=m_shape.data;
		}
	      
	      int normBin(-1);
	      ////normalization factor (from MET<50)
	      if(model==HZZ && histos[ih].find("met_met")!=string::npos)  normBin=normH->GetXaxis()->FindBin(50);
	      ////normalization factor (from red-MET<40)
	      if(model==ZZ && histos[ih].find("met_redMet")!=string::npos) normBin=normH->GetXaxis()->FindBin(40);
	      ////normalization factor (from deta_jj<3)
	      //if(model==VBFZ && histos[ih].find("jetdeta")!=string::npos) normBin=normH->GetXaxis()->FindBin(3);
	      if(model==VBFZ && histos[ih].find("premjj")!=string::npos) normBin=normH->GetXaxis()->FindBin(500);

	      if(normBin<0) continue;
	      cout << histos[ih] << " " << normBin << " " << " " << normKey << flush;
	      Shape_t &dilMetShape = shapesMap[normKey+"_"+histos[ih]];
	      float sf=dilMetShape.data->Integral(1,normBin)/normH->Integral(1,normBin);
	      cout << " " << sf << endl;
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
     if(it->first.find("ee")!=string::npos)             normKey="ee";
     if(it->first.find("eq0jets_")!=string::npos)       normKey+="eq0jets";
     if(it->first.find("geq1jets_")!=string::npos)      normKey+="geq1jets";
     else if(it->first.find("eq1jets_")!=string::npos)  normKey+="eq1jets";
     if(it->first.find("geq2jets_")!=string::npos)      normKey+="geq2jets";
     if(it->first.find("vbf_")!=string::npos)           normKey+="vbf";
     float sf = scaleFactors[normKey];
     
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
     if(it->first.find("mt_shapes")!= string::npos || it->first.find("met_") != string::npos || it->first.find("dijet_mass_shapes")!=string::npos)
       {
	 bool isTH2( corrGammaH->InheritsFrom("TH2") );
	
	 if(subtractType==HALVE || subtractType==EWKSUBTRACTIONHALVE) {
	   //int fbin( isTH2 ? 1 : corrGammaH->GetXaxis()->FindBin(70) );
	   int fbin( isTH2 ? 1 : corrGammaH->GetXaxis()->FindBin(60) );
	   for(int ibin=fbin; ibin<=corrGammaH->GetXaxis()->GetNbins(); ibin++)
	     {
	       if(isTH2)
		 {
		   for(int jbin=1; jbin<=corrGammaH->GetYaxis()->GetNbins(); jbin++)
		     {
		       Double_t val=corrGammaH->GetBinContent(ibin,jbin)/2;
		       Double_t valerr=val;
		       if(model==VBFZ) valerr=0.3*val;
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
       	 float newSF = it->second.data->Integral(1,normBin)/corrGammaH->Integral(1,normBin);
	 corrGammaH->Scale(newSF);
       }

     corrGammaH->SetFillColor(831);
     cout << it->second.totalBckg->GetName() << " " << corrGammaH->GetName() << " " << sf << endl;
     it->second.totalBckg->Add(corrGammaH);
     it->second.bckg["Instr. background (data)"]=corrGammaH;
     showShape(it->second,"final_"+it->first,is2011,model);
     gOutDir->cd();
     TString keyToWrite(it->first.c_str());
     //     keyToWrite.ReplaceAll("eeeq","ee_eq");
     // keyToWrite.ReplaceAll("mumueq","mumu_eq");
     //     keyToWrite.ReplaceAll("eq0jets","");
     corrGammaH->Write(keyToWrite);//it->first.c_str());

     if(it->first.find("mumu")!= string::npos)
       {
	 TString keyToGet(it->first);
	 keyToGet=keyToGet.ReplaceAll("mumu","ee");
	 Shape_t &eeShape=shapesMap[keyToGet.Data()];
	 addToShape(eeShape,it->second);
	 keyToGet=keyToGet.ReplaceAll("ee","");
	 showShape(eeShape,keyToGet.Data(),is2011,model);
       }
   }
  gOut->Close();

}

//
void showShape(const Shape_t &shape,TString SaveName,bool is2011,int model)
{
  if(shape.data->InheritsFrom("TH2"))
    {
      cout << "Skipping plot for: " << shape.data->GetName() << endl;
      return;
    }
  TCanvas* c1 = new TCanvas(SaveName,SaveName,800,800);
  c1->cd();

  TPad* t1 = new TPad("t1","t1", 0.0, 0.20, 1.0, 1.0);  t1->Draw();  t1->cd();
  if(!SaveName.EndsWith("50")) t1->SetLogy(true);
  
  TLegend* legA  = new TLegend(0.845,0.2,0.99,0.99, "NDC");

  bool canvasIsFilled(false);
  THStack *stack=0;
  TH1 *mc=0,*vbfmc=0;
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

     TH1 *nrbBckg=0;
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
		if(model==VBFZ) nrbBckg->SetTitle("Top,W,VV");
		else            nrbBckg->SetTitle("#splitline{Non resonant}{bkg (data)}"); 
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

      //include VBF Z in the stack
      if(model==VBFZ && SaveName.EndsWith("50"))
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

 
      stack->Draw();
      stack->GetXaxis()->SetTitle(mc->GetXaxis()->GetTitle());
      stack->GetYaxis()->SetTitle(mc->GetYaxis()->GetTitle());
      stack->SetMinimum(1);//mc->GetMinimum());
      //if(SaveName.Contains("vbf") && !SaveName.Contains("novbf")) stack->SetMinimum(1e-2);
      stack->SetMaximum(1.5*max(mc->GetMaximum(),shape.data->GetMaximum()));

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
  if(model!=VBFZ)
    {
      for(std::map<TString , TH1 *>::const_iterator it=shape.signal.begin(); it!= shape.signal.end(); it++)
	{
	  TH1 *h=it->second;
	  h->Draw(canvasIsFilled ? "histsame" : "hist");
	  legA->AddEntry(h,h->GetTitle(),"L");
	  canvasIsFilled=true;
	}
    }



  TPaveText* T = new TPaveText(0.1,0.995,0.9,0.96, "NDC");
  T->SetFillColor(0);
  T->SetFillStyle(0);  T->SetLineColor(0);
  T->SetTextAlign(22);
  char Buffer[1024]; 
  if(is2011)
    sprintf(Buffer, "CMS preliminary, #sqrt{s}=7 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5051./1000);
  else
    sprintf(Buffer, "CMS preliminary, #sqrt{s}=8 TeV, #scale[0.5]{#int} L=%.1f fb^{-1}", 5041./1000);
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
      
      TGraphErrors *systGr=0;
      if(model==VBFZ)
	{
	  TH1 *ratiosyst=(TH1*)shape.data->Clone("RatioHistogramSyst");
	  ratiosyst->SetDirectory(0);
	  TH1 *mcwithsyst=(TH1 *)mc->Clone("mcsyst");
	  for(int ibin=1; ibin<=mcwithsyst->GetXaxis()->GetNbins(); ibin++)
	    {
	      float unc=pow(vbfmc->GetBinContent(ibin,1)*0.3,2);
	      unc += pow(mcwithsyst->GetBinError(ibin),2);
	      mcwithsyst->SetBinError(ibin,sqrt(unc));
	    }
	  ratiosyst->Divide(mcwithsyst);
	  systGr=new TGraphErrors(ratiosyst);
	  systGr->SetFillStyle(3001);
	  systGr->SetFillColor(kGray+1);
	  systGr->SetMarkerStyle(1);
	  systGr->SetMarkerColor(kGray+1);
	  for(int ibin=1; ibin<=ratiosyst->GetXaxis()->GetNbins(); ibin++){
	    systGr->SetPointError(ibin-1,ratiosyst->GetXaxis()->GetBinWidth(ibin)/2.0, ratiosyst->GetBinError(ibin));
	  }
	}

      TAxis *yaxis=0,*xaxis=0;
      if(systGr) { 
	systGr->Draw("a20");
	yaxis=systGr->GetYaxis(); xaxis=systGr->GetXaxis();
	xaxis->SetRangeUser(ratio->GetXaxis()->GetXmin(),ratio->GetXaxis()->GetXmax());
	ratio->Draw("E1same");
      }
      else{
	ratio->Draw("e1");
	yaxis=ratio->GetYaxis(); xaxis=ratio->GetXaxis();
      }
      yaxis->SetRangeUser(0.5,1.5);
      yaxis->SetTitle("Data/#Sigma Bckg");
      xaxis->SetTitle("");
      xaxis->SetTitleOffset(1.3);
      xaxis->SetLabelSize(0.033*yscale);
      xaxis->SetTitleSize(0.036*yscale);
      xaxis->SetTickLength(0.03*yscale);
      yaxis->SetTitleOffset(0.3);
      yaxis->SetNdivisions(5);
      yaxis->SetLabelSize(0.033*yscale);
      yaxis->SetTitleSize(0.036*yscale);



    }
   
  c1->cd();
  c1->Modified();
  c1->Update();
  if(SaveName.BeginsWith("_")) SaveName="inc"+SaveName;
  c1->SaveAs(SaveName+".png");
  c1->SaveAs(SaveName+".pdf");

  if(!SaveName.Contains("hardpt") && !SaveName.Contains("dphijj") && !SaveName.Contains("mjj") ) return;
  bool doRebin(SaveName.Contains("mjj"));

  //subtract data
  c1->Clear();
  c1->SetWindowSize(600,600);
  c1->SetLogy(false);

  TLegend* legB  = new TLegend(0.845,0.2,0.99,0.99, "NDC");

  TH1 *diff = (TH1*)shape.data->Clone("DiffHistogram");
  diff->SetDirectory(0);
  diff->Add(vbfmc,-1);
  TH1 *vbfmcwithsyst=(TH1 *)vbfmc->Clone("vbfmcwithsyst");
  for(int ibin=1; ibin<=vbfmcwithsyst->GetXaxis()->GetNbins(); ibin++)
    {
      float unc=pow(vbfmcwithsyst->GetBinContent(ibin,1)*0.3,2);
      unc += pow(vbfmcwithsyst->GetBinError(ibin),2);
      vbfmcwithsyst->SetBinError(ibin,sqrt(unc));
    }
  TH1 *diffWithSyst = (TH1*)shape.data->Clone("DiffWithSystHistogram");
  diffWithSyst->SetDirectory(0);
  diffWithSyst->Add(vbfmcwithsyst,-1);
  if(doRebin) diffWithSyst->Rebin(4);
  TGraphErrors *diffWithSystGr=new TGraphErrors(diffWithSyst);
  diffWithSystGr->SetFillStyle(3427);
  diffWithSystGr->SetFillColor(kGray+1);
  diffWithSystGr->SetMarkerStyle(1);
  diffWithSystGr->SetMarkerColor(kGray+1);
  for(int ibin=1; ibin<=diffWithSyst->GetXaxis()->GetNbins(); ibin++){
    diffWithSystGr->SetPointError(ibin-1,diffWithSyst->GetXaxis()->GetBinWidth(ibin)/2.0, diffWithSyst->GetBinError(ibin));
  }
  
  if(doRebin) diff->Rebin(4);
  diffWithSystGr->Draw("a20");
  diffWithSystGr->GetXaxis()->SetRangeUser(vbfmcwithsyst->GetXaxis()->GetXmin(),vbfmcwithsyst->GetXaxis()->GetXmax());
  diff->Draw("same");
  legB->AddEntry(diff,"residuals","P");

  //signal
  TH1 *hframe=0;
  for(std::map<TString , TH1 *>::const_iterator it=shape.signal.begin(); it!= shape.signal.end(); it++)
    {
      TH1 *h=(TH1 *)it->second->Clone();
      h->SetFillStyle(3001);
      h->Draw(canvasIsFilled ? "histsame" : "hist");
      if(doRebin) h->Rebin(4);
      if(!canvasIsFilled) hframe=h;
      legB->AddEntry(h,h->GetTitle(),"F");
    }


 
  //  hframe->GetYaxis()->SetRangeUser(1.05*diff->GetMinimum(),diff->GetMaximum()*1.05);

  legB->SetFillColor(0); legB->SetFillStyle(0); legB->SetLineColor(0);
  legB->SetHeader("");
  legB->Draw("same");
  legB->SetTextFont(42);

  T->SetBorderSize(0);
  T->Draw();
  SaveName.ReplaceAll("hardpt","hardpt_sub");
  SaveName.ReplaceAll("dphijj","dphijj_sub");
  SaveName.ReplaceAll("mjj","mjj_sub");
  c1->SaveAs(SaveName+".png");
  c1->SaveAs(SaveName+".pdf");
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

  gStyle->SetPadTopMargin   (0.06);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadRightMargin (0.16);
  gStyle->SetPadLeftMargin  (0.14);
  gStyle->SetTitleSize(0.04, "XYZ");
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetTitleYOffset(1.45);
  gStyle->SetPalette(1);
  gStyle->SetNdivisions(505);

}
