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
float getIntegral(TH1 *h,double min=-1,double max=-1)
{
  if(h==0) return 0;
  int firstbin=0;
  int lastbin=h->GetXaxis()->GetNbins()+1;
  
  if(min!=-1 && max>min)
    {
      firstbin=h->GetXaxis()->FindBin(min);
      firstbin=h->GetXaxis()->FindBin(max);
    }

  return h->Integral(firstbin,lastbin);
}


//
float getScaleFactor(TH1 *h1, TH1 *h2,double minVal,double maxVal)
{
  if(h1==0 || h2==0) return 0;
  if(h2->Integral()==0 || h1->Integral()==0) return 0;

  float sf=getIntegral(h1,minVal,maxVal)/getIntegral(h2,minVal,maxVal);
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
void getGammaWeights(TString inputFile="plotter_raw.root",bool isData=false,string var2dName="eta")
{

  string cats[]={"eq0jets","eq1jets","geq2jets","vbf"};
  TString catLabels[]={"=0 jets", "=1 jets", "#geq 2 jets","VBF"};
  const size_t ncats=sizeof(cats)/sizeof(string);


  string modDir="|M_{ll}-91|<15 GeV-c^{2}";
  string dataDir="#gamma+jets";
  string compDir="";
  if(!isData)
    {
      modDir="Z-#gamma^{*}+jets#rightarrow ll";
      dataDir="#gamma+jets (simulation)";
      compDir="";
    }
  
  //plots to retrieve
  std::vector<string> subcats;
  subcats.push_back("photon20");
  subcats.push_back("photon30");
  subcats.push_back("photon50");

  if(isData) 
    {
      subcats.push_back("photon75"); 
      subcats.push_back("photon90"); 
      subcats.push_back("photon125");
      subcats.push_back("photon135");
      subcats.push_back("photon200");
    }
  else
    {
      subcats.push_back("photon75"); 
      subcats.push_back("photon125");
    }
  const size_t nsubcats=subcats.size();
  
  string var2dTitle(var2dName);
  var2dName = "vs"+var2dName;

  //open file and get plots
  TFile *fin = TFile::Open(inputFile);
  std::vector<TH1F *> gammaPt, zPt, ptWeights;
  std::vector<TH2F *> gammaPt2d, zPt2d, ptWeights2d;
  int iplot(0);
  std::vector<string> dilCats;
  dilCats.push_back("ee");
  dilCats.push_back("mumu");

  setStyle();
  TCanvas *cnv2 = getNewCanvas("weightsc","weightsc",false);
  cnv2->Clear();
  cnv2->SetWindowSize(1600,400);
  cnv2->Divide(4,1);
  for(size_t icat=0; icat<ncats; icat++)
    {
      
      TPad *p=(TPad *)cnv2->cd(icat+1);
      p->SetLogx();

      //first reweight individually each pT category
      for(size_t isubcat=0; isubcat<nsubcats; isubcat++)
	{
	  for(size_t idcat=0; idcat<dilCats.size(); idcat++,iplot++)
	    {
	      string plotName=cats[icat]+"_"+subcats[isubcat]+dilCats[idcat]+"qt";
	      
	      TH1F *iGammaPt = (TH1F *)getObjectFromPath( fin, dataDir+"/"+plotName, false);
	      TH2F *iGammaPt2d = (TH2F *)getObjectFromPath( fin, dataDir+"/"+plotName+var2dName, false);
	      double totalGamma=getIntegral(iGammaPt);
	      
	      TH1F *iZPt = (TH1F *)getObjectFromPath( fin, modDir+"/"+plotName, false);
	      TH2F *iZPt2d = (TH2F *)getObjectFromPath( fin, modDir+"/"+plotName+var2dName, false);
	      double totalZ=getIntegral(iZPt);
	      TString newName(cats[icat]+subcats[isubcat]+dilCats[idcat]+"gamma_pt");
	      gammaPt.push_back( (TH1F *) iGammaPt->Clone(newName) );
	      gammaPt[iplot]->SetDirectory(0);
	      gammaPt[iplot]->Scale(1./totalGamma);
	      
	      gammaPt2d.push_back( (TH2F *) iGammaPt2d->Clone(newName+var2dName) );
	      gammaPt2d[iplot]->SetDirectory(0);
	      gammaPt2d[iplot]->Scale(1./totalGamma);
	      
	      newName=cats[icat]+subcats[isubcat]+dilCats[idcat]+"z_pt";
	      zPt.push_back( (TH1F *) iZPt->Clone(newName) );
	      zPt[iplot]->SetDirectory(0);
	      zPt[iplot]->Scale(1./totalZ);
	      
	      zPt2d.push_back( (TH2F *) iZPt2d->Clone(newName+var2dName) );
	      zPt2d[iplot]->SetDirectory(0);
	      zPt2d[iplot]->Scale(1./totalZ);
	  
	      //compute the weights
	      newName=cats[icat]+subcats[isubcat]+dilCats[idcat]+"gammaptweight";
	      ptWeights.push_back( (TH1F *) zPt[iplot]->Clone(newName) );
	      ptWeights[iplot]->SetDirectory(0);
	      ptWeights[iplot]->GetXaxis()->SetTitle("p_{T}^{#gamma} [GeV/c]");
	      ptWeights[iplot]->GetYaxis()->SetTitle("p_{T}^{Z#rightarrow ll}/p_{T}^{#gamma}");
	      ptWeights[iplot]->Divide(gammaPt[iplot]);

	      ptWeights[iplot]->SetMarkerColor(isubcat%3+1);
	      ptWeights[iplot]->SetLineColor(isubcat%3+1);
	      ptWeights[iplot]->SetFillStyle(0);
	      if(idcat==0) ptWeights[iplot]->SetMarkerStyle(24);
	      ptWeights[iplot]->Draw(isubcat==0 ? "e1":"e1same");
	      ptWeights[iplot]->GetYaxis()->SetRangeUser(0,5);

	      newName=cats[icat]+subcats[isubcat]+dilCats[idcat]+"gammapt"+var2dName+"weight";
	      ptWeights2d.push_back( (TH2F *) zPt2d[iplot]->Clone(newName) );
	      ptWeights2d[iplot]->SetDirectory(0);
	      ptWeights2d[iplot]->GetXaxis()->SetTitle("p_{T}^{#gamma} [GeV/c]");
	      ptWeights2d[iplot]->GetYaxis()->SetTitle(var2dTitle.c_str());
	      ptWeights2d[iplot]->GetZaxis()->SetTitle("p_{T}^{Z#rightarrow ll}/p_{T}^{#gamma}");
	      ptWeights2d[iplot]->Divide(gammaPt2d[iplot]);
	    }
	}


      TPaveText *pave = new TPaveText(0.5,0.65,1.0,0.95,"NDC");
      pave->SetBorderSize(0);
      pave->SetFillStyle(0);
      pave->SetTextAlign(32);
      pave->SetTextFont(42);
      pave->AddText(catLabels[icat])->SetTextFont(62);
      pave->Draw();
    }
  fin->Close();
  
  //save
  TString foutName="gammapt"+var2dName;
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
  fout->Close();

  cnv2->SaveAs("plots/gammaptweights.C");
  cnv2->SaveAs("plots/gammaptweights.pdf");

  cout << "*** Gamma pT weights (inclusive/differential) available @ " << foutName << endl;
}


//
void getGammaTemplates(TString inputFile="plotter_weighted.root",TString dilChannel="mumu",bool isData=false)
{

  TString outDir="plots/";
  gSystem->Exec("mkdir -p " + outDir);

  TString theLegend("CMS simulation");
  if(isData) theLegend="CMS preliminary";
  if(dilChannel=="mumu") theLegend += ",#mu#mu events";
  if(dilChannel=="ee")   theLegend += ",ee events";
  if(dilChannel=="ll")   theLegend += ",ee/#mu#mu events";

  string modDir="|M_{ll}-91|<15 GeV-c^{2}";
  string dataDir="#gamma+jets";
  string compDir="Z+#gamma";
  std::string compTitle=compDir;
  if(!isData)
    {
      modDir="Z-#gamma^{*}+jets#rightarrow ll";
      dataDir="#gamma+jets (simulation)";
      //      compDir="Z-#gamma^{*}+jets#rightarrow ll (Madgraph)";
      //      compTitle="Z/#gamma^{*}+jets (Madgraph)";
      compDir="";//Z+#gamma";
      compTitle="";//Z+#gamma";
    }
  
  //plots to retrieve
  std::vector<TString> cats, catLabels;
  //  cats.push_back("photon20");  catLabels.push_back("20<p_{T}^{#gamma}<30");
  // cats.push_back("photon30");  catLabels.push_back("30<p_{T}^{#gamma}<50");
  cats.push_back("photon50");  catLabels.push_back("50<p_{T}^{#gamma}<75");

  if(isData) 
    {
      cats.push_back("photon75");   catLabels.push_back("75<p_{T}^{#gamma}<90");
      cats.push_back("photon90");   catLabels.push_back("90<p_{T}^{#gamma}<125");
      cats.push_back("photon125");  catLabels.push_back("125<p_{T}^{#gamma}<135");
      cats.push_back("photon135");  catLabels.push_back("135<p_{T}^{#gamma}<200");
      cats.push_back("photon200");  catLabels.push_back("p_{T}^{#gamma}>200");
    }
  else
    {
      cats.push_back("photon75");  catLabels.push_back("75<p_{T}^{#gamma}<125");
      cats.push_back("photon125"); catLabels.push_back("p_{T}^{#gamma}>125");
    }


  TString subcats[]={"eq0jets","eq1jets","geq2jets","vbf"};
  TString subcatLabels[]={"=0 jets", "=1 jets", "#geq 2 jets","VBF"};
  TString variables[]={"met","mindphijmet","ht","qt","eta","mt","mindphijmet","dphivmet","redClusteredMet","minAssocChargedMet","minAssocMet","njets"};
  
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
      cnv->SetWindowSize(1600,(ncats-isData)*200);
      cnv->Divide(4,ncats,0,0);
	
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
	      TPad *p= (TPad *) cnv->cd(1+iscat+4*icat);
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

	      if(getIntegral(iGammaVariable)>0) 
		{ 
		  float sf=0;

		  std::pair<int,int> key(icat,iscat);
		  if(variables[iv]=="met")
		    {
		      sf=getScaleFactor(iZVariable,iGammaVariable,0,35);
		      metBasedSF[ key ] = sf;
		    }
		  else if( metBasedSF.find(key) != metBasedSF.end() )
		    sf = metBasedSF[key];

		  if(iZCompVariable)
		    {
		      float icompSf=getIntegral(iZVariable)/(getIntegral(iGammaVariable)+getIntegral(iZCompVariable));
		      iZCompVariable->Scale(icompSf);
		    }

		  iGammaVariable->Scale(sf);
		  if(!kinVariable) iGamma2dVariable->Scale(sf);
		}
		
	      
	      //draw distributions retrieved
	      iZVariable->Draw("hist");
	      iZVariable->GetYaxis()->SetRangeUser(1e-5,10);
	      if(iZCompVariable) iZCompVariable->Draw("histsame");
	      iGammaVariable->Draw("e2same");
	      if(getIntegral(iGammaVariable)>0 and getIntegral(iZVariable)>0)
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
      cnv2->SetWindowSize(1600,400);
      cnv2->Divide(4,1,0,0);
      for(size_t i=1; i<5; i++)
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


    




