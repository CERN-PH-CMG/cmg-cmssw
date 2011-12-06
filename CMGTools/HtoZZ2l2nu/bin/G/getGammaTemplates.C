/*
  gSystem->Load("libCMGToolsHtoZZ2l2nu.so");
  .L getGammaTemplates.C+
 */
#include <iostream>


#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"

#include "TSystem.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TPaveText.h"

//
TObject* getObjectFromPath(TDirectory* File, std::string Path, bool GetACopy=false)
{
   TObject* toReturn = NULL;
   size_t pos = Path.find("/");
   if(pos < std::string::npos){
      std::string firstPart = Path.substr(0,pos);
      std::string endPart   = Path.substr(pos+1,Path.length());
      TDirectory* TMP = (TDirectory*)File->Get(firstPart.c_str());
      toReturn =  getObjectFromPath(TMP,endPart,GetACopy);
   }else{
      toReturn = File->Get(Path.c_str());
   }

   if(!toReturn)       printf("BUG: %s\n",Path.c_str());
   else if(GetACopy){ toReturn = toReturn->Clone();  }
   return toReturn;
}

//
std::pair<float,float> getIntegral(TH1 *h,double min=-1,double max=-1,TString opt="")
{
  if(h==0) return std::pair<float,float>(0.,0.);
  int firstbin=0;
  int lastbin=h->GetXaxis()->GetNbins()+1;
  
  if(min!=-1)
    {
      firstbin=h->GetXaxis()->FindBin(min);
    }
  if(max>0)
    {
      lastbin=h->GetXaxis()->FindBin(max);
    }
  Double_t val(0),val_err(0);
  val=h->IntegralAndError(firstbin,lastbin,val_err,opt);
  return std::pair<float,float>(val,val_err);
}


//
float getScaleFactor(TH1 *h1, TH1 *h2,double minVal,double maxVal)
{
  if(h1==0 || h2==0) return 0;
  if(h2->Integral()==0 || h1->Integral()==0) return 0;

  float sf=getIntegral(h1,minVal,maxVal).first/getIntegral(h2,minVal,maxVal).first;
  if(h1->GetEntries()>100 && h2->GetEntries()>50)
    {
      TH1 *ratio=(TH1 *) h1->Clone("ratiotmp");
      ratio->Divide(h2);
      ratio->Fit("pol0","R+","",minVal,maxVal);
      sf=ratio->GetFunction("pol0")->GetParameter(0);
      ratio->Delete();
    }
  cout << "***************************" << endl;
  cout << sf << endl;
  cout << h1->GetEntries() << " " << h2->GetEntries() << endl;
  return sf;
}



//
TH1 *deriveGammaWeightsFrom(TH1 *z, TH1 *g)
{
  if(z==0 || g==0) return 0;
  TString wgtName=g->GetName();  wgtName += "weight";
  TH1 *wgt=(TH1F *) z->Clone(wgtName);
  wgt->SetDirectory(0);
  wgt->GetXaxis()->SetTitle("p_{T}^{#gamma} [GeV/c]");
  wgt->GetYaxis()->SetTitle("p_{T}^{Z#rightarrow ll}/p_{T}^{#gamma}");
  wgt->Divide(g);
  return wgt;
}


//
void estimateGammaBackground(TString inputFile="plotter_weighted.root", string baseCat="eq0jets", string dilChannel="mumu",float cut=70)
{

  TFile *fin = TFile::Open(inputFile);
  string modDir="Z-#gamma^{*}+jets#rightarrow ll";
  string dataDir="data";

  string trigCats[]={"photon55","photon75","photon90","photon125","photon135"};
  TString cutStr(""); cutStr += (int)cut;

  std::vector<string> metDirs;
  metDirs.push_back("Z+#gamma#rightarrow l^{+}l^{-}#gamma");
  metDirs.push_back("W+#gamma");
  metDirs.push_back("W+jets");
  metDirs.push_back("Z+#gamma#rightarrow#nu#nu#gamma");

  float totalgamma(0), totalzmc(0),totalmet(0);
  float totalgammaerr(0), totalzmcerr(0),totalmeterr(0);
  float totalbckg(0), totalbckgerr(0);
  for(size_t icat=0; icat<3; icat++)
    {
      //derive the scale factor for the photon+jets
      string baseName=baseCat;
      baseName += "_";
      baseName+=trigCats[icat]+dilChannel;
      //string baseName=trigCats[icat]+dilChannel;
      TH1F *dphidata = (TH1F *)getObjectFromPath( fin, dataDir+"/"+baseName+string("mindphijmet"+cutStr), true);
      dphidata->SetDirectory(0);
      TH1F *dphimc = (TH1F *)getObjectFromPath( fin, modDir+"/"+baseName+string("mindphijmet"+cutStr), true);
      dphimc->SetDirectory(0);

      TH1F *dphimet= (TH1F *)getObjectFromPath( fin, metDirs[0]+"/"+baseName+string("mindphijmet"+cutStr), true);
      dphimet->SetDirectory(0);
      for(size_t i=1; i<metDirs.size(); i++)
	dphimet->Add((TH1F *)getObjectFromPath( fin, metDirs[i]+"/"+baseName+string("mindphijmet"+cutStr), false) );
      
      std::pair<float,float> dataAbove05=getIntegral(dphidata,0.);//0.5);
      std::pair<float,float> mcAbove05=getIntegral(dphimc,0.);//.5);
      std::pair<float,float> metAbove05=getIntegral(dphimet,0.);//.5);

      totalgamma    += dataAbove05.first;
      totalgammaerr += pow(dataAbove05.second,2);

      totalzmc      += mcAbove05.first;
      totalzmcerr   += pow(mcAbove05.second,2);

      totalmet      += metAbove05.first;
      totalmeterr   += pow(metAbove05.second,2);

      totalbckg+=max(float(totalgamma-totalmet),float(0.));
      totalbckgerr+=totalgammaerr+totalmeterr;
    }
  totalgammaerr=sqrt(totalgammaerr);
  totalzmcerr=sqrt(totalzmcerr);
  totalmeterr=sqrt(totalmeterr);
  totalbckgerr=sqrt(totalbckgerr);

  cout << totalgamma << " +/- " << totalgammaerr << endl
       << totalmet   << " +/- " << totalmeterr   << endl
       << totalzmc   << " +/- " << totalzmcerr   << endl
       << totalbckg  << " +/- " << totalbckgerr << endl;

  
  fin->Close();
}



//
void getGammaWeights(TString inputFile="mc_raw.root",bool isData=false,string var2dName="nvtx")
{

  string cats[]={"eq0jets","eq1jets","geq2jets","vbf"};
  TString catLabels[]={"=0 jets", "=1 jets", "#geq 2 jets","VBF"};
  const size_t ncats=sizeof(cats)/sizeof(string);


  string modDir="Z-#gamma^{*}+jets#rightarrow ll";
  std::vector<string> dataDirs;
  if(!isData)
    {
      dataDirs.push_back("#gamma+jets");
      dataDirs.push_back("QCD");
      dataDirs.push_back("Di-photon");
      //      dataDirs.push_back("Z+#gamma#rightarrow l^{+}l^{-}#gamma");
      // dataDirs.push_back("W+#gamma");
      dataDirs.push_back("W+jets");
      //  dataDirs.push_back("Z+#gamma#rightarrow#nu#nu#gamma");
    }
  else
    {
      dataDirs.push_back("data");
    }
  
  //plots to retrieve
  string var2dTitle(var2dName);
  var2dName = "vs"+var2dName;

  //open file and get plots
  TFile *fin = TFile::Open(inputFile);
  std::vector<TH1F *> gammaPt, zPt, ptWeights;
  std::vector<TH2F *> gammaPt2d, zPt2d, ptWeights2d;

  std::vector<string> dilCats; 
  dilCats.push_back("ee");
  dilCats.push_back("mumu");

  std::vector<TH1F *> massLines;
  TH1F *hinc=0;
  for(size_t idcat=0; idcat<dilCats.size(); idcat++)
    {
      TH1F *h=(TH1F *) getObjectFromPath(fin,modDir+"/"+dilCats[idcat]+"zmass",true);
      h->SetDirectory(0);
      h->SetBinContent(1,0);
      h->SetBinContent(h->GetXaxis()->GetNbins(),0);
      massLines.push_back( h );
      if(hinc==0) {
	hinc=(TH1F*)h->Clone("llzmass");
	hinc->SetDirectory(0);
      }
      else
	hinc->Add(h);
    }
  massLines.push_back(hinc);


  setStyle();
  TCanvas *cnv2 = getNewCanvas("weightsc","weightsc",false);
  cnv2->Clear();
  cnv2->SetWindowSize(ncats*400,400);
  cnv2->Divide(ncats,1);
  for(size_t icat=0; icat<ncats; icat++)
    {
      
      TPad *p=(TPad *)cnv2->cd(icat+1);
      p->SetLogx();
      p->SetLogy();

      //build the plots for each dilepton channel (and sum inclusively)
      TH1F *incGammaPt=0,   *incZPt=0;
      TH2F *incGammaPt2d=0, *incZPt2d=0;
      for(size_t idcat=0; idcat<dilCats.size(); idcat++)
	{
	  string baseName=cats[icat]+"_";
	  
	  //gamma plots
	  TH1F *iGammaPt=0;
	  TH2F *iGammaPt2d=0;
	  for(size_t idir=0; idir<dataDirs.size(); idir++)
	    {
	      if(iGammaPt==0)
		{
		  iGammaPt = (TH1F *)getObjectFromPath( fin, dataDirs[idir]+"/"+baseName+dilCats[idcat]+"qt", true);
		  iGammaPt->SetDirectory(0);
		  iGammaPt2d = (TH2F *)getObjectFromPath( fin, dataDirs[idir]+"/"+baseName+dilCats[idcat]+"qt"+var2dName, true);
		  iGammaPt2d->SetDirectory(0);
		}
	      else
		{
		  iGammaPt->Add( (TH1F *)getObjectFromPath( fin, dataDirs[idir]+"/"+baseName+dilCats[idcat]+"qt", true) );
		  iGammaPt2d->Add( (TH2F *)getObjectFromPath( fin, dataDirs[idir]+"/"+baseName+dilCats[idcat]+"qt"+var2dName, true) );
		}
	    }
	  if(incGammaPt==0)
	    {
	      incGammaPt   = (TH1F *) iGammaPt->Clone(TString(baseName+"llqt"));
	      incGammaPt->SetDirectory(0);
	      incGammaPt2d = (TH2F *) iGammaPt2d->Clone(TString(baseName+"llqt"+var2dName));
	      incGammaPt2d->SetDirectory(0);
	    }
	      
	  //z plots
	  TH1F *iZPt   = (TH1F *)getObjectFromPath( fin, modDir+"/"+baseName+dilCats[idcat]+"qt");
	  iZPt = (TH1F *) iZPt->Clone(string(modDir+"/"+baseName+dilCats[idcat]+"zqt").c_str());
	  iZPt->SetDirectory(0);
	  TH2F *iZPt2d   = (TH2F *)getObjectFromPath( fin, modDir+"/"+baseName+dilCats[idcat]+"qt"+var2dName);
	  iZPt2d = (TH2F *) iZPt2d->Clone(string(modDir+"/"+baseName+dilCats[idcat]+"qt"+var2dName).c_str());
	  iZPt2d->SetDirectory(0);
	  if(incZPt==0)
	    {
	      incZPt   = (TH1F *) iZPt->Clone(TString(baseName+"llzqt"));
	      incZPt->SetDirectory(0);
	      incZPt2d = (TH2F *) iZPt2d->Clone(TString(baseName+"llzqt"+var2dName));
	      incZPt2d->SetDirectory(0);
	    }
	  else
	    {
	      incZPt->Add(iZPt);
	      incZPt2d->Add(iZPt2d);
	    }

	  //compute the weights
	  TH1F *iPtWeight=(TH1F *) deriveGammaWeightsFrom( iZPt, iGammaPt );
	  TH2F *iPtWeight2d= (TH2F *) deriveGammaWeightsFrom( iZPt2d, iGammaPt2d);
	      
	  //store 
	  gammaPt.push_back( iGammaPt );
	  gammaPt2d.push_back( iGammaPt2d );
	  zPt.push_back( iZPt );
	  zPt2d.push_back( iZPt2d );
	  ptWeights.push_back( iPtWeight );
	  ptWeights2d.push_back( iPtWeight2d );
	      
	  //draw the 1d weights
	  iPtWeight->SetMarkerColor(icat%3+1);
	  iPtWeight->SetLineColor(icat%3+1);
	  iPtWeight->SetFillStyle(0);
	  iPtWeight->SetMarkerStyle(idcat==0 ? 24 : 20);
	      
	  iPtWeight->Draw(idcat==0 ? "e1":"e1same");
	  iPtWeight->GetYaxis()->SetRangeUser(1e-4,10);
	  iPtWeight->GetXaxis()->SetRangeUser(10,1000);
	}
      
      //compute the inclusive weights
      TH1F *ptWeight=(TH1F *) deriveGammaWeightsFrom( incZPt, incGammaPt );
      TH2F *ptWeight2d= (TH2F *) deriveGammaWeightsFrom( incZPt2d, incGammaPt2d);
      
      //store
      gammaPt.push_back( incGammaPt );
      gammaPt2d.push_back( incGammaPt2d );
      zPt.push_back( incZPt );
      zPt2d.push_back( incZPt2d );
      ptWeights.push_back(ptWeight);
      ptWeights2d.push_back(ptWeight2d);
      
      TPaveText *pave = new TPaveText(0.25,0.75,0.45,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText(catLabels[icat])->SetTextFont(62);
      pave->Draw();
    }
  fin->Close();


  
  //save histograms to file
  TString foutName="gammaqt"+var2dName;
  foutName += "weight.root";
  if(isData) foutName = "data_"+foutName;
  else       foutName = "mc_"+foutName;
  TFile *fout = TFile::Open(foutName,"RECREATE");
  fout->cd();
  for(size_t iplot=0; iplot<gammaPt.size(); iplot++)
    {
      gammaPt[iplot]->Write();
      gammaPt2d[iplot]->Write();
      zPt[iplot]->Write();
      zPt2d[iplot]->Write();
      ptWeights[iplot]->Write();
      ptWeights2d[iplot]->Write();
  }
  for(size_t iplot=0; iplot<massLines.size(); iplot++)  massLines[iplot]->Write();
  fout->Close();

  //save canvas
  cnv2->SaveAs("plots/gammaptweights.C");
  cnv2->SaveAs("plots/gammaptweights.png");
  
  cout << "*** Gamma pT weights (inclusive/differential) available @ " << foutName << endl;
}



//
void getGammaTemplates(TString inputFile="/data/psilva/Higgs/ntuples_2011.11.01/plotter/mc_weighted_njets.root",TString dilChannel="mumu",bool isData=false)
{

  TString outDir="plots/";
  gSystem->Exec("mkdir -p " + outDir);

  bool applySF( inputFile.Contains("_eta"));

  TString theLegend("CMS simulation");
  if(isData) theLegend="CMS preliminary";
//   if(dilChannel=="mumu") theLegend += ",#mu#mu events";
//   if(dilChannel=="ee")   theLegend += ",ee events";
//   if(dilChannel=="ll")   
  theLegend += ",ee/#mu#mu events";
    
  string modDir="|M_{ll}-91|<15 GeV-c^{2}";
  string dataDir="#gamma+jets";
  string compDir="Z+#gamma";
  std::string compTitle=compDir;
  if(!isData)
    {
      modDir="Z-#gamma^{*}+jets#rightarrow ll";
      dataDir="#gamma+jets";
      //      compDir="Z-#gamma^{*}+jets#rightarrow ll (Madgraph)";
      //      compTitle="Z/#gamma^{*}+jets (Madgraph)";
      compDir="";//Z+#gamma";
      compTitle="";//Z+#gamma";
    }
  
  //plots to retrieve
  std::vector<TString> cats, catLabels;
  cats.push_back("photon20");  catLabels.push_back("20<p_{T}^{#gamma}<30");
  cats.push_back("photon30");  catLabels.push_back("30<p_{T}^{#gamma}<50");
  cats.push_back("photon50");  catLabels.push_back("50<p_{T}^{#gamma}<75");
  cats.push_back("photon75");   catLabels.push_back("75<p_{T}^{#gamma}<90");
  cats.push_back("photon90");   catLabels.push_back("90<p_{T}^{#gamma}<125");
  cats.push_back("photon125");  catLabels.push_back("125<p_{T}^{#gamma}<135");
  cats.push_back("photon135");  catLabels.push_back("135<p_{T}^{#gamma}<200");
  //    cats.push_back("photon200");  catLabels.push_back("p_{T}^{#gamma}>200");
  

  TString subcats[]={"eq0jets","eq1jets","geq2jets"};//,"vbf"};
  TString subcatLabels[]={"=0 jets", "=1 jets", "#geq 2 jets","VBF"};
  TString variables[]={"met","ht","qt","eta","mt","mindphijmet","dphivmet","redClusteredMet","minAssocChargedMet","njets"};
  
  //output
  TFile *fout = TFile::Open("gammaTemplates.root","RECREATE");
  fout->Close();
  
  TFile *fin = TFile::Open(inputFile);
    
  setStyle();
  const size_t ncats=cats.size();
  const size_t nsubcats=sizeof(subcats)/sizeof(TString);
  const size_t nvars=sizeof(variables)/sizeof(TString);
  std::map<std::pair<int,int>, float> metBasedSF;
  for(size_t iv=0; iv<nvars; iv++)
    {
      TCanvas *cnv = getNewCanvas(variables[iv]+"c",variables[iv]+"c",false);
      cnv->Clear();
      cnv->SetWindowSize(nsubcats*400,(ncats-isData)*200);
      cnv->Divide(nsubcats,ncats,0,0);
	
      std::vector<TH1*>gammaVariables, zVariables, zCompVariables;
      TH1 *gammaSum=0,*zSum=0,*zCompSum=0;
      TH2F *gamma2dSum=0, *z2dSum=0;

      bool kinVariable(false);
      if(variables[iv]=="eta" 
	 || variables[iv]=="qt"
	 || variables[iv]=="ht"
	 || variables[iv]=="mt"
	 || variables[iv]=="dphill"
	 || variables[iv]=="ptjets"
	 || variables[iv]=="njets"  	 
	 || variables[iv]=="minmjv"
	 || variables[iv]=="metoverqt"
	 || variables[iv]=="redClusteredMetoverqt"
	 || variables[iv]=="evtctr"
	 || variables[iv]=="mindphijmet" 
	 || variables[iv]=="dphivmet"
	 ) kinVariable=true;

      for(size_t icat=0; icat<ncats; icat++)
	{
	  for(size_t iscat=0; iscat<nsubcats; iscat++)
	    {
	      TPad *p= (TPad *) cnv->cd(1+iscat+nsubcats*icat);
	      if(iscat==0)          p->SetLeftMargin(0.12);
	      if(iscat==nsubcats-1) p->SetRightMargin(0.05);
	      if(icat==ncats-1)     p->SetBottomMargin(0.15);
	      	      
	      string ivar("");
	      if(!kinVariable) ivar+="met_"; 
	      ivar+=variables[iv];
	      
	      //gamma variable
	      string ipath=dataDir +"/";
	      ipath += subcats[iscat].Data();
	      ipath +="_"; 
	      ipath += cats[icat].Data();
	      TH1 *iGammaVariable = 0;
	      if(dilChannel!="ll") iGammaVariable=(TH1 *)getObjectFromPath( fin, ipath+dilChannel.Data()+ivar, true);
	      else
		{
		  iGammaVariable=(TH1 *)getObjectFromPath( fin, ipath+"mumu"+ivar, true);
		  iGammaVariable->Add((TH1 *)getObjectFromPath( fin, ipath+"ee"+ivar));
		}
	      if(iGammaVariable==0) continue;
	      iGammaVariable->SetDirectory(0);
	      iGammaVariable->SetFillStyle(0);
	      iGammaVariable->SetLineColor(1);
	      iGammaVariable->SetFillColor(1);
	      iGammaVariable->SetMarkerSize(0.8);
	      iGammaVariable->SetMarkerColor(1);
	      iGammaVariable->SetTitle("#gamma+jets");
	      TString newName=TString("gamma")+subcats[iscat]+TString("_")+cats[icat]+dilChannel;
	      iGammaVariable->SetName(newName);
	      TH2 *iGamma2dVariable = 0;
	      if(dilChannel!="ll") iGamma2dVariable=(TH2 *)getObjectFromPath( fin, ipath+dilChannel.Data()+ivar+"vspu", true);
              else
                {
                  iGamma2dVariable=(TH2 *)getObjectFromPath( fin, ipath+"mumu"+ivar+"vspu", true);
                  if(iGamma2dVariable) iGamma2dVariable->Add((TH2 *)getObjectFromPath( fin, ipath+"ee"+ivar+"vspu"));
                }
	      
	      //Z variable
	      ipath=modDir +"/";
	      ipath += subcats[iscat].Data();
	      ipath +="_"; 
	      ipath += cats[icat].Data();
	      TH1 *iZVariable = 0;
	      if(dilChannel!="ll") iZVariable=(TH1 *)getObjectFromPath( fin, ipath+dilChannel.Data()+ivar, true);
	      else
		{
		  iZVariable=(TH1 *)getObjectFromPath( fin, ipath+"mumu"+ivar, true);
		  iZVariable->Add((TH1 *)getObjectFromPath( fin, ipath+"ee"+ivar));
		}
	      if(iZVariable==0) continue;
	      iZVariable->SetLineColor(kRed);
	      iZVariable->SetFillStyle(0);
	      iZVariable->SetFillColor(kRed);
	      iZVariable->SetMarkerColor(kRed);
	      iZVariable->SetDirectory(0);
	      iZVariable->SetTitle("Z/#gamma^{*}+jets");
	      newName=TString("z")+subcats[iscat]+TString("_")+cats[icat]+dilChannel;
	      iZVariable->SetName(newName);
	      TH2 *iZ2dVariable=0;
	      if(dilChannel!="ll") iZ2dVariable=(TH2 *)getObjectFromPath( fin, ipath+dilChannel.Data()+ivar+"vspu", true);
              else
                {
                  iZ2dVariable=(TH2 *)getObjectFromPath( fin, ipath+"mumu"+ivar+"vspu",true);
                  if(iZ2dVariable) iZ2dVariable->Add((TH2 *)getObjectFromPath( fin, ipath+"ee"+ivar+"vspu"));
                }
	      
	      
	      //comparison
	      TH1 *iZCompVariable = 0;
	      if(compDir.size()>0)
		{
// 		  iZCompVariable = (TH1 *) getObjectFromPath(fin,compDir+"/"+idist,true );
// 		  iZCompVariable->SetDirectory(0);
// 		  iZCompVariable->SetTitle(compTitle.c_str());
// 		  iZCompVariable->SetFillStyle(0);
// 		  iZCompVariable->SetFillColor(0);
// 		  iZCompVariable->SetLineColor(1);
// 		  iZCompVariable->SetMarkerColor(1);
// 		  iZCompVariable->SetMarkerStyle(1);
// 		  iZCompVariable->SetLineStyle(2);
// 		  iZCompVariable->SetLineWidth(2);
		}

	      if(getIntegral(iGammaVariable).first>0) 
		{ 
		  float sf=1;
		  
		  std::pair<int,int> key(icat,iscat);
		  if(variables[iv]=="met")
		    {
		      //sf=getScaleFactor(iZVariable,iGammaVariable,0,35);
		      // metBasedSF[ key ] = sf;
		      metBasedSF[ key ] = 1.0;
		    }
		  else if( metBasedSF.find(key) != metBasedSF.end() )
		    sf = metBasedSF[key];

		  if(iZCompVariable)
		    {
		      float icompSf=getIntegral(iZVariable).first/(getIntegral(iGammaVariable).first+getIntegral(iZCompVariable).first);
		      iZCompVariable->Scale(icompSf);
		    }
		  if(applySF)
		    {
		      iGammaVariable->Scale(sf);
		      if(!kinVariable) iGamma2dVariable->Scale(sf);
		    }
		}
		
	      
	      //draw distributions retrieved
	      iZVariable->Draw("hist");
	      iZVariable->GetYaxis()->SetRangeUser(1e-5,10);
	      if(iZCompVariable) iZCompVariable->Draw("histsame");
	      iGammaVariable->Draw("e2same");
	      if(getIntegral(iGammaVariable).first>0 and getIntegral(iZVariable).first>0)
		{
		  TPaveText *pave = new TPaveText(0.5,0.65,1.0,0.95,"NDC");
		  pave->SetBorderSize(0);
		  pave->SetFillStyle(0);
		  pave->SetTextAlign(32);
		  pave->SetTextFont(42);
		  pave->AddText(catLabels[icat])->SetTextFont(62);
		  pave->AddText(subcatLabels[iscat])->SetTextFont(62);
		  pave->AddText("");
		  char buf[100];
		  sprintf(buf,"#chi^{2}/ndof %3.3f", iZVariable->Chi2Test(iGammaVariable,"WWCHI2/NDF") );
		  pave->AddText(buf);
		  pave->Draw();
		}
	      p->SetLogy();
	      p->Modified();
	      p->Update();

	      if(gammaVariables.size()<=iscat)
		{
		  gammaVariables.push_back( (TH1 *) iGammaVariable->Clone(subcats[iscat]+"_gamma_"+variables[iv]) );
		  gammaVariables[iscat]->Reset("ICE");
		  gammaVariables[iscat]->SetDirectory(0);
		  
		  zVariables.push_back( (TH1 *) iZVariable->Clone(subcats[iscat]+"_z_"+variables[iv]) );
		  zVariables[iscat]->Reset("ICE");
		  zVariables[iscat]->SetDirectory(0);
		  
		  if(iZCompVariable)
		    {
		      zCompVariables.push_back( (TH1 *) iZCompVariable->Clone(subcats[iscat]+"_zcomp_"+variables[iv]) );
		      zCompVariables[iscat]->Reset("ICE");
		      zCompVariables[iscat]->SetDirectory(0);
		    }

		  if(gammaSum==0)
		    {
		      gammaSum =  (TH1 *)iGammaVariable->Clone(subcats[iscat]+"_gammasum_"+variables[iv]);
		      gammaSum->Reset("ICE");
		      gammaSum->SetDirectory(0);
		 
		      if(!kinVariable)
			{
			  gamma2dSum = (TH2F *) iGamma2dVariable->Clone("gammasum_"+variables[iv]+"vspu");
			  gamma2dSum->Reset("ICE");
			  gamma2dSum->SetDirectory(0);
			}
		      
		      zSum =  (TH1 *)iZVariable->Clone(subcats[iscat]+"_zsum_"+variables[iv]);
		      zSum->Reset("ICE");
		      zSum->SetDirectory(0);

		      if(iZCompVariable)
			{
			  zCompSum =  (TH1 *)iZCompVariable->Clone(subcats[iscat]+"_zcompsum_"+variables[iv]);
			  zCompSum->Reset("ICE");
			  zCompSum->SetDirectory(0);
			}
		    }
		}
	      
	      gammaVariables[iscat]->Add(iGammaVariable);
	      gammaSum->Add(iGammaVariable);
	      if(!kinVariable) gamma2dSum->Add(iGamma2dVariable);
	      zVariables[iscat]->Add(iZVariable);
	      zSum->Add(iZVariable);
	      if(iZCompVariable)
		{
		  zCompVariables[iscat]->Add(iZCompVariable);
		  zCompSum->Add(iZCompVariable);
		}
	    }
	}


      //save 2d
      fout = TFile::Open("gammaTemplates.root","UPDATE");
      fout->cd();
      if(!kinVariable) gamma2dSum->Write();
      fout->Close();
      
      cnv->cd();
      cnv->Modified();
      cnv->Update();
      cnv->SaveAs(outDir+variables[iv]+".png");
      cnv->SaveAs(outDir+variables[iv]+".C");

      //#inclusive template canvas
      TCanvas *cnv2 = getNewCanvas(variables[iv]+"templc",variables[iv]+"templc",false);
      cnv2->Clear();
      cnv2->SetWindowSize(400*nsubcats,600);
      cnv2->Divide(nsubcats,1,0,0);
      for(size_t i=1; i<nsubcats+1; i++)
	{
	  TList *stack  = new TList;
	  TList *data = new TList;
	  TList *spimpose = new TList;
	  stack->Add(zVariables[i-1]);    	 
	  data->Add(gammaVariables[i-1]);
	  if(zCompVariables.size()>=i) spimpose->Add(zCompVariables[i-1]);

	  TPad *pad=(TPad *)cnv2->cd(i);
	  TLegend *leg=showPlotsAndMCtoDataComparison(pad,*stack,*spimpose,*data);
	  if(i==1) formatForCmsPublic(pad,leg,theLegend,2);
	  else     leg->Delete();
	  
	  pad->cd(1);
	  TPaveText *pave = new TPaveText(0.65,0.75,0.95,0.95,"NDC");
	  pave->SetBorderSize(0);
	  pave->SetFillStyle(0);
	  pave->SetTextAlign(32);
	  pave->SetTextFont(42);
	  pave->AddText(subcatLabels[i-1])->SetTextFont(62);
	  pave->AddText("");
	  pave->AddLine();
	  char buf[100];
	  sprintf(buf,"#chi^{2}/ndof %3.3f", zVariables[i-1]->Chi2Test(gammaVariables[i-1],"WWCHI2/NDF"));
	  pave->AddText(buf);
	  pave->Draw();
	}
      
      cnv2->cd();
      cnv2->Modified();
      cnv2->Update();
      cnv2->SaveAs(outDir+variables[iv]+"inc.png");
      cnv2->SaveAs(outDir+variables[iv]+"inc.C");
      
      //inclusive template canvas
      TCanvas *cnv3 = getNewCanvas(variables[iv]+"inctemplc",variables[iv]+"inctemplc",false);
      cnv3->Clear();
      cnv3->SetWindowSize(600,600);
      TList *stack  = new TList;
      TList *data = new TList;
      TList *spimpose = new TList;
      stack->Add(zSum);
      data->Add(gammaSum);
      if(zCompSum) spimpose->Add(zCompSum);
      TLegend *leg=showPlotsAndMCtoDataComparison(cnv3,*stack,*spimpose,*data);
      TPad *pad=(TPad *)cnv3->cd(1);      
      formatForCmsPublic(pad,leg,theLegend,2);
      //zSum->GetXaxis()->SetRangeUser(0,250);
      zSum->GetYaxis()->SetRangeUser(1e-5,10);

      TPaveText *pave = new TPaveText(0.65,0.75,0.95,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText("Inclusive")->SetTextFont(62);
      pave->AddText("");
      pave->AddLine();
      char buf[100];
      sprintf(buf,"#chi^{2}/ndof %3.3f", zSum->Chi2Test(gammaSum,"WWCHI2/NDF"));
      pave->AddText(buf);
      pave->Draw();
      
      cnv3->cd();
      cnv3->Modified();
      cnv3->Update();
      cnv3->SaveAs(outDir+variables[iv]+"suminc.png");
      cnv3->SaveAs(outDir+variables[iv]+"suminc.C");
    }

  fin->Close();
}


//   
void getContaminationTable(TString inputFile="plotter_weighted.root", string histoname="finaleventflowmet", string dilChannel="mumu")
{
  
  TFile *fin = TFile::Open(inputFile);
  string modDir="Z-#gamma^{*}+jets#rightarrow ll";
  string dataDir="data";
  
  string trigCats[]={"eq0jets","eq1jets","geq2jets","vbf"};
  //string trigCats[]={"vbf"};
  int ntrigcats=sizeof(trigCats)/sizeof(string);

  std::vector<string> metDirs;
  metDirs.push_back("Z+#gamma#rightarrow l^{+}l^{-}#gamma");
  metDirs.push_back("W+#gamma");
  metDirs.push_back("W+jets");
  metDirs.push_back("Z+#gamma#rightarrow#nu#nu#gamma");

  TH1F *evtflowdata=0,*evtflowmc=0,*evtflowmet=0,*evtflowfinal=0;
  for(size_t icat=0; icat<ntrigcats; icat++)
    {
      //derive the scale factor for the photon+jets
      string baseName=trigCats[icat]+"_"+dilChannel;
      //string baseName=dilChannel;
      
      TH1F *ievtflowdata = (TH1F *)getObjectFromPath( fin, dataDir+"/"+baseName+histoname);
      if(evtflowdata==0) 
	{
	  evtflowdata = (TH1F *) ievtflowdata->Clone("evtflowdata");
	  evtflowdata->SetDirectory(0);
	}
      else
	evtflowdata->Add(ievtflowdata);

      TH1F *ievtflowmc = (TH1F *)getObjectFromPath( fin, modDir+"/"+baseName+histoname);
      if(evtflowmc==0)
	{
	  evtflowmc = (TH1F *) ievtflowmc->Clone("evtflowmc");
	  evtflowmc->SetDirectory(0);
	}
      else
	evtflowmc->Add(ievtflowmc);

      TH1F *ievtflowmet = 0;
      if(metDirs.size())
	{
	  ievtflowmet=(TH1F *)getObjectFromPath( fin, metDirs[0]+"/"+baseName+histoname,true)->Clone("ievtflowmet");
	  ievtflowmet->SetDirectory(0);
	  if(evtflowmet==0)
	    {
	      evtflowmet = (TH1F *) ievtflowmet->Clone("evtflowmc");
	      evtflowmet->SetDirectory(0);
	    }
	  else
	    evtflowmet->Add(ievtflowmet);
	  for(size_t i=1; i<metDirs.size(); i++)
	    {
	      TH1F *h = (TH1F *)getObjectFromPath( fin, metDirs[i]+"/"+baseName+histoname, false);
	      evtflowmet->Add(h);
	      ievtflowmet->Add(h);
	    }
	}
      
      if(evtflowfinal==0)
	{
	  evtflowfinal=(TH1F *) ievtflowdata->Clone("evtflowfinal");
	  evtflowfinal->SetDirectory(0);
	  evtflowfinal->Reset("ICE");
	}
      
      for(int ibin=1; ibin<=evtflowfinal->GetXaxis()->GetNbins(); ibin++)
	{
	  float totalGamma    = ievtflowdata->GetBinContent(ibin);

	  float totalMet    = ievtflowmet?ievtflowmet->GetBinContent(ibin):0;
	  float totalMetErr = ievtflowmet?ievtflowmet->GetBinError(ibin):0;

	  float totalbckg    = max(float(totalGamma-totalMet),float(0.));
	  float totalbckgerr = sqrt(totalGamma+pow(totalMetErr,2));
	  if(ibin==4) cout <<  totalGamma << " " << totalMet << " " << totalbckg << endl;
	  
	  float curTotalFinal = evtflowfinal->GetBinContent(ibin)+totalbckg;
	  float curTotalErr   = sqrt( pow(evtflowfinal->GetBinError(ibin),2) + pow(totalbckgerr,2) );
	  
	  evtflowfinal->SetBinContent(ibin,curTotalFinal);
	  evtflowfinal->SetBinError(ibin,curTotalErr);
	}
    }

  for(int ibin=1; ibin<=evtflowfinal->GetXaxis()->GetNbins(); ibin++)
    {
       cout << evtflowmc->GetXaxis()->GetBinLabel(ibin) << "\t | "
	   << evtflowmc->GetBinContent(ibin) 
 	   << "+/-" << evtflowmc->GetBinError(ibin) << "\t | "
	   << evtflowfinal->GetBinContent(ibin) 
	   << " +" << evtflowfinal->GetBinError(ibin)
	   << " -" << min(evtflowfinal->GetBinError(ibin),evtflowfinal->GetBinContent(ibin)) << endl;
    }
  
  fin->Close();
}






