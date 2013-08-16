#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TLegend.h" 
#include "TPaveText.h"
#include "TGraph.h"
#include "TLine.h"

TFile *fOut;
TString pdfUrl="";

class LxyTemplate{
public:

  LxyTemplate(TString tag, TString kin="pt")
    : m_tag(tag), m_kin(kin), kinSumH(0), lxySumH(0)
  {
    m_kin.ReplaceAll("pt","p_{T} [GeV]");
    m_kin.ReplaceAll("gamma","#gamma");
  }

  void addCategory(float etaMin, float etaMax, int kinBins)
  {
    TString name(m_tag+"_"); name += loLimits.size(); name += "_"; name += kinBins;
    loLimits.push_back(etaMin);
    hiLimits.push_back(etaMax);

    TH1F *h = new TH1F(name+"_kinvar",";"+m_kin+"; b-quarks",kinBins,0.,300.);
    h->SetDirectory(0);
    h->Sumw2();
    kinH.push_back(h);

    h = new TH1F(name+ "_lxy",";L_{xy} [cm];b-quarks", 100,0.,5.);
    h->SetDirectory(0);
    h->Sumw2();
    lxyH.push_back(h);

    h = new TH1F(name+ "_mass", "; SecVtx Mass [GeV]; Events / (0.2 GeV)", 50, 0.,10.);
    h->SetDirectory(0);
    h->Sumw2();
    massH.push_back(h);
  }
  
  void doStandardCategories(int kinBins)
  {
    addCategory(0,  1.1,kinBins);
    //     addCategory(0,  0.45,kinBins);
    //     addCategory(0.45,0.9,kinBins); 
    //     addCategory(0.9,1.1,kinBins);    
    //     //addCategory(1.1,1.5,kinBins);    
    //     //addCategory(1.5,2.5,kinBins);
  }
  
  void fill(float kin,float eta,float lxy,float mass,float wgt=1.0)
  {
    float abseta=fabs(eta);
    for(size_t i=0; i<hiLimits.size(); i++)
      {
	if(abseta<=loLimits[i] || abseta>hiLimits[i]) continue;
	if(kinH.size()<=i) continue;
	kinH[i]->Fill(kin,wgt);
	lxyH[i]->Fill(lxy,wgt);
	massH[i]->Fill(mass,wgt);
      }
  }

  float weightToTarget( float kin, float eta, const LxyTemplate &target)
  {
    float weight(0.);
    float abseta=fabs(eta);
    for(size_t i=0; i<hiLimits.size(); i++)
      {
	if(abseta<=loLimits[i] || abseta>hiLimits[i]) continue;
	if(target.kinH.size()<=i) continue;

	const TH1F *targetH=target.kinH[i];
	TH1F *thisH=kinH[i];
	Int_t ibin=thisH->FindBin(kin);
	weight=targetH->GetBinContent(ibin)/thisH->GetBinContent(ibin);
	if(isnan(weight)) weight=0;
	break;
     }
    return weight;
  }
  
  void sumUp( bool append=false)
  {
    kinSumH = (TH1F *) kinH[0]->Clone( kinH[0]->GetName()+TString("_total")); kinSumH->SetDirectory(0); kinSumH->Reset("ICE");
    lxySumH = (TH1F *) lxyH[0]->Clone( lxyH[0]->GetName()+TString("_total")); lxySumH->SetDirectory(0); lxySumH->Reset("ICE");
    massSumH = (TH1F *) massH[0]->Clone( massH[0]->GetName()+TString("_total")); massSumH->SetDirectory(0); massSumH->Reset("ICE");
    for(size_t ih=0; ih<kinH.size(); ih++)
      {
	kinSumH->Add(kinH[ih]);
	lxySumH->Add(lxyH[ih]);
	massSumH->Add(massH[ih]);
      }
    if(append){
      loLimits.push_back( loLimits[0] );
      hiLimits.push_back( hiLimits[hiLimits.size()-1] );
      kinH.push_back(kinSumH);
      lxyH.push_back(lxySumH);
      massH.push_back(massSumH);
    }
  }

  TString m_tag,m_kin;
  std::vector<float> loLimits,hiLimits;
  std::vector<TH1F *> kinH, lxyH, massH;
  TH1F *kinSumH, *lxySumH, *massSumH;
};


//
std::map<TString,std::vector<float> > RunJetClassification(TString ctrlUrl,TString targetUrl, TString kin="pt",Int_t kinBins=10,Float_t maxDRbj=0.3,std::vector<int> *optimBins=0, int filterChannel=-1, TString tag="")
{   
  std::map<TString,std::vector<float> > res;
  Int_t pflav,chcat;
  Float_t gamma,pt,eta,lxy,mass,bqdR;
  Float_t jetpt1, jetpt2,jetpt3, jetpt4;

  cout << "************" << tag << endl;

  TString eosPrefix("root://eoscms//eos/cms/");
  if(ctrlUrl.BeginsWith("/store"))   ctrlUrl   = eosPrefix + ctrlUrl;
  if(targetUrl.BeginsWith("/store")) targetUrl = eosPrefix + targetUrl;

  TString chSuf("inc");
  if(filterChannel==1)  chSuf="mumu";
  if(filterChannel==2)  chSuf="ee";
  if(filterChannel==3)  chSuf="emu";

  //fill the target templates
  cout << "Filling target templates" << endl; 
  LxyTemplate target("target"+chSuf,kin);
  if(optimBins==0) target.doStandardCategories(kinBins);
  else{
    target.addCategory(0,1.1, (*optimBins)[0] );
    //     target.addCategory(0,  0.45, (*optimBins)[0] );
    //     target.addCategory(0.45,0.9, (*optimBins)[1] );
    //     target.addCategory(0.9,1.1, (*optimBins)[2] );
    //     //      target.addCategory(1.1,1.5, (*optimBins)[3] );
    //     //  target.addCategory(1.5,2.5, (*optimBins)[4] );
  }
  TFile *targetF=TFile::Open(targetUrl);
  TTree *theTree = (TTree *)targetF->Get("ana/toptree");
  theTree->SetBranchAddress( "bqpt",    &pt );
  theTree->SetBranchAddress( "bqgamma", &gamma );
  theTree->SetBranchAddress( "bqeta",   &eta );
  theTree->SetBranchAddress( "lxy",     &lxy ); 
  theTree->SetBranchAddress( "secvtxmass", &mass );
  theTree->SetBranchAddress( "chcat",   &chcat );
  theTree->SetBranchAddress( "bqdR",    &bqdR );
  for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
    theTree->GetEntry(ievt);
    Float_t abseta=TMath::Abs(eta);
    if(lxy<=0) continue;
    if(filterChannel>0 && chcat!=filterChannel) continue;
    float kinVar=gamma;
    if(kin=="pt") kinVar=pt;
    target.fill(kinVar,abseta,lxy,mass);
  }
  targetF->Close();
  
  //fill the control templates
  cout << "Filling control templates" << endl;
  LxyTemplate ctrl("ctrl"+chSuf,kin);
  if(optimBins==0) ctrl.doStandardCategories(kinBins);
  else{
    ctrl.addCategory(0,  1.1, (*optimBins)[0] );

    //     ctrl.addCategory(0,  0.45, (*optimBins)[0] );
    //     ctrl.addCategory(0.45,0.9, (*optimBins)[1] );
    //     ctrl.addCategory(0.9,1.1, (*optimBins)[2] );
    //     //  ctrl.addCategory(1.1,1.5, (*optimBins)[3] );
    //    //  ctrl.addCategory(1.5,2.5, (*optimBins)[4] );
  }
  TFile *ctrlF = TFile::Open( ctrlUrl );
  theTree = (TTree*) ctrlF->Get("ana/toptree");
  theTree->SetBranchAddress( "bqpt", &pt );
  theTree->SetBranchAddress( "bqdR", &bqdR );
  theTree->SetBranchAddress( "bqgamma", &gamma );
  theTree->SetBranchAddress( "bqeta", &eta );
  theTree->SetBranchAddress( "jetpt1", &jetpt1 );
  theTree->SetBranchAddress( "jetpt2", &jetpt2 );
  theTree->SetBranchAddress( "jetpt3", &jetpt3 );
  theTree->SetBranchAddress( "jetpt4", &jetpt4 );
  theTree->SetBranchAddress( "partonflav", &pflav );
  theTree->SetBranchAddress( "lxy", &lxy );
  theTree->SetBranchAddress( "secvtxmass", &mass );
  for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
    theTree->GetEntry(ievt);
    Float_t abseta=TMath::Abs(eta);
    if(lxy<=0 || abs(pflav)!=5 || fabs(bqdR)>maxDRbj) continue;
    int njets( (jetpt1>30) + (jetpt2>30) + (jetpt3>30) + (jetpt4>30));
    if(njets<2) continue;
    
    float kinVar=gamma;
    if(kin=="pt")    kinVar=pt;
    ctrl.fill(kinVar,abseta,lxy,mass);
  }
  
  //derive the weighted templates
  cout << "Filling weighted templates" << endl;
  LxyTemplate templ("templates"+chSuf,kin);
  if(optimBins==0) templ.doStandardCategories(kinBins);
  else{
    templ.addCategory(0,  1.1, (*optimBins)[0] );
    //     templ.addCategory(0,  0.45, (*optimBins)[0] );
    //     templ.addCategory(0.45,0.9, (*optimBins)[1] );
    //     templ.addCategory(0.9,1.1, (*optimBins)[2] );
    //     //  templ.addCategory(1.1,1.5, (*optimBins)[3] );
    //     //  templ.addCategory(1.5,2.5, (*optimBins)[4] );
  }
  
  //add pdf variations
  Float_t pdfWgts[45];
  TFile *pdfF=0;
  std::vector<TH1F *> pdfVarH;
  for(int ivar=0; ivar<=44; ivar++)
    {
      pdfWgts[ivar]=0;
      if(pdfUrl!="")
	{
	  TString pf(""); pf+=ivar;
	  if(pdfF==0) { pdfF=TFile::Open(pdfUrl); cout << "Retrieving PDF variations from " << pdfUrl << endl; }
	  TTree *itree=(TTree *)pdfF->Get("cteq66.LHgrid_var"+pf);
	  itree->SetBranchAddress("w",&pdfWgts[ivar]);
	  theTree->AddFriend(itree);

	  TH1F *h = new TH1F("inc_bjetlxy_ttbarsystpdf"+pf+"_1725",";L_{xy} [cm];b-quarks", 100,0.,5.);
	  h->SetDirectory(0);
	  h->Sumw2();
	  pdfVarH.push_back(h);
	}
    }
  if(pdfVarH.size()) cout << "Reweighting for " << pdfVarH.size() << " variations" << endl;

  for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {
    
    theTree->GetEntry(ievt);
    Float_t abseta=TMath::Abs(eta);
    if(lxy<=0 || abs(pflav)!=5 || fabs(bqdR)>maxDRbj) continue;
    int njets( (jetpt1>30) + (jetpt2>30) + (jetpt3>30) + (jetpt4>30));
    if(njets<2) continue;
      
    float kinVar=gamma;
    if(kin=="pt")    kinVar=pt;
    float wgt=ctrl.weightToTarget(kinVar,abseta,target);
    templ.fill(kinVar,abseta,lxy,mass,wgt);

    for(size_t ipdfvar=0; ipdfvar<pdfVarH.size(); ipdfvar++) 
      {
	if(lxy<5) pdfVarH[ipdfvar]->Fill(lxy,wgt*pdfWgts[ipdfvar]/pdfWgts[0]);
      }

  }
  ctrlF->Close();
  if(pdfF) pdfF->Close();

  fOut->cd();
  TDirectory *outDir=0;
  TString dirName("");
  if(tag!="") 
    {
      dirName = chSuf;
      outDir  = fOut->GetDirectory(dirName);
      cout << chSuf << " " << outDir << " " << fOut << endl;
    }
  else
    {
      dirName =kin+"bins"; 
      if(optimBins==0) dirName += ctrl.kinH[0]->GetXaxis()->GetNbins();
      else             dirName += "optim";
      dirName += chSuf;
    }
  if(outDir==0) outDir=fOut->mkdir(dirName);


  //show the results
  if(optimBins!=0)
    {
      target.sumUp(true);
      ctrl.sumUp(true);
      templ.sumUp(true);
    }
  for(size_t ibin=0; ibin<target.loLimits.size(); ibin++)
    {
      TString cname("c"); 
      if(optimBins==0)  cname+=ibin; 
      else              cname+="optim";
      cname += "_"; cname += ctrl.kinH[ibin]->GetXaxis()->GetNbins();
      cname += chSuf;
      TCanvas *c=new TCanvas(cname,cname,800,400);
      c->Divide(2,1);
      c->cd(1);
      ctrl.kinH[ibin]->SetLineColor(kGray);
      ctrl.kinH[ibin]->SetFillStyle(3001);
      ctrl.kinH[ibin]->SetFillColor(kGray);   
      ctrl.kinH[ibin]->Scale(templ.kinH[ibin]->Integral()/ctrl.kinH[ibin]->Integral());
      ctrl.kinH[ibin]->Draw("hist");
      templ.kinH[ibin]->Draw("hist same");
      target.kinH[ibin]->Draw("e1 same");
      float maxVal=TMath::Max(TMath::Max(ctrl.kinH[ibin]->GetMaximum(),templ.kinH[ibin]->GetMaximum()), target.kinH[ibin]->GetMaximum());
      ctrl.kinH[ibin]->GetYaxis()->SetRangeUser(0,maxVal);

      char buf[200];
      sprintf(buf,"#splitline{CMS simulation}{%3.2f < |#eta| < %3.2f}",target.loLimits[ibin],target.hiLimits[ibin]);

      TLegend *leg = new TLegend(0.6,0.7,0.9,0.85,buf,"brNDC");
      leg->AddEntry(target.kinH[ibin],"target","P");
      leg->AddEntry(ctrl.kinH[ibin],  "template","L");
      leg->SetNColumns(2);
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);
      leg->SetTextFont(42);
      leg->Draw();

      c->cd(2);
      ctrl.lxyH[ibin]->SetLineColor(kGray);
      ctrl.lxyH[ibin]->SetFillStyle(3001);
      ctrl.lxyH[ibin]->SetFillColor(kGray);   
      ctrl.lxyH[ibin]->Scale(templ.lxyH[ibin]->Integral()/ctrl.lxyH[ibin]->Integral());
      ctrl.lxyH[ibin]->Draw("hist");
      templ.lxyH[ibin]->Draw("hist same");
      target.lxyH[ibin]->Draw("e1 same");
      maxVal=TMath::Max(TMath::Max(ctrl.lxyH[ibin]->GetMaximum(),templ.lxyH[ibin]->GetMaximum()), target.lxyH[ibin]->GetMaximum());
      ctrl.lxyH[ibin]->GetYaxis()->SetRangeUser(0,maxVal);

      TPaveText *txt = new TPaveText(0.6,0.7,0.9,0.85,"brNDC");
      txt->SetBorderSize(0);
      txt->SetFillStyle(0);
      txt->SetTextFont(42);
      float chi2=target.lxyH[ibin]->Chi2Test(templ.lxyH[ibin],"WWCHI2/NDF");
      sprintf(buf,"#chi^{2}/NDF=%3.1f, K-S=%3.1f",    chi2, target.lxyH[ibin]->KolmogorovTest(ctrl.lxyH[ibin],"X"));
      txt->AddText(buf);
      sprintf(buf,"<L_{xy}^{target}>=%3.3f #pm %3.3f",target.lxyH[ibin]->GetMean(), target.lxyH[ibin]->GetMeanError());
      txt->AddText(buf);
      sprintf(buf,"<L_{xy}^{template}>=%3.3f #pm %3.3f",  templ.lxyH[ibin]->GetMean(),   templ.lxyH[ibin]->GetMeanError());
      txt->AddText(buf);
      txt->Draw();

      //compute the quantiles: 0.25, 0.5, 0.75
      const Int_t nq = 4;
      Double_t xq[nq];  
      for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
      Double_t templq[nq],targetq[nq];  // array to contain the quantiles
      target.lxyH[ibin]->GetQuantiles(nq,targetq,xq);
      templ.lxyH[ibin]->GetQuantiles(nq,templq,xq);

      //save the results
      res["lolim"]     .push_back( templ.loLimits[ibin] );
      res["hilim"]     .push_back( templ.hiLimits[ibin] );
      res["chi2"]      .push_back( chi2 );
      res["dmean"]     .push_back( templ.lxyH[ibin]->GetMean()-target.lxyH[ibin]->GetMean() );
      res["dmean_err"] .push_back( sqrt(pow(target.lxyH[ibin]->GetMeanError(),2)+pow(templ.lxyH[ibin]->GetMeanError(),2)) );
      res["dq025"]     .push_back( targetq[0]-templq[0] );
      res["dq05"]      .push_back( targetq[1]-templq[1] );
      res["dq075"]     .push_back( targetq[2]-templq[2] );

      //save to file
      outDir->cd();
      if(tag=="")
	{
	  target.lxyH[ibin]->Write();
	  templ.lxyH[ibin]->Write();
	  target.massH[ibin]->Write();
	  templ.massH[ibin]->Write();
	  c->Write();
	}
      else if(ibin==target.loLimits.size()-1)
	{
	  cout << target.lxyH[ibin]->GetName() << endl;
	  TString outName("lxy_ttbar");
	  if(tag.IsDigit()) outName += "_"+tag;
	  else              outName += "syst"+tag+"_1725";
	  target.lxyH[ibin]->Write(chSuf+"_bjetorig"+outName);
	  templ.lxyH[ibin]->Write(chSuf+"_bjet"+outName);
	  target.massH[ibin]->Write(chSuf+"_bjetmasorig"+outName);
	  templ.massH[ibin]->Write(chSuf+"_bjetmass"+outName);
	  c->Write(chSuf+"_canvas"+outName);
	}

      //write pdf variations if available
      for(size_t ipdfvar=0; ipdfvar<pdfVarH.size(); ipdfvar++) pdfVarH[ipdfvar]->Write();
    }

  return res;
} 


//
void RunOptimization(TString iFile,TString jFile, TString kin, Float_t maxDRbj)
{
  std::map<TString, std::vector<TGraph *> > optimGr;
  std::map<TString, std::vector<TLine *> > optimThr;
 
  int minBins=1;
  int maxBins=20;
  int binStep=1;
  fOut = TFile::Open("JetKinematicsOptimization.root","RECREATE");
  for(int ibin=minBins; ibin<=maxBins; ibin+= binStep){

    std::map<TString,std::vector<float> > res = RunJetClassification(iFile,jFile,kin,ibin, maxDRbj);

    std::vector<float > & loLimits=res["lolim"];
    std::vector<float > & hiLimits=res["hilim"];
    for(size_t i=0; i<loLimits.size(); i++)
      {
	char buf[200];
	sprintf(buf,"%3.1f < |#eta| < %3.1f",loLimits[i],hiLimits[i]);
	if(optimGr.find(buf)==optimGr.end())
	  {
	    TGraph *gr = new TGraph; gr->SetMarkerStyle(20+i); gr->SetTitle(buf); gr->SetFillStyle(0); 
	    TString suf("_gr"); suf += i;
	    std::vector<TGraph *> catGr;
	    catGr.push_back( (TGraph *)gr->Clone("chi2"+suf) );
	    catGr.push_back( (TGraph *)gr->Clone("mean"+suf) );
	    catGr.push_back( (TGraph *)gr->Clone("dq025"+suf) );
	    catGr.push_back( (TGraph *)gr->Clone("dq05"+suf) );
	    catGr.push_back( (TGraph *)gr->Clone("dq75"+suf) );
	    optimGr[buf]=catGr;
	    delete gr;
	    
	    std::vector<TLine *> catThr;
	    catThr.push_back( new TLine(minBins,1.0,                  maxBins,1.0) );
	    catThr.push_back( new TLine(minBins,res["dmean_err"][i],  maxBins,res["dmean_err"][i]) );
	    catThr.push_back( new TLine(minBins,res["dmean_err"][i],  maxBins,res["dmean_err"][i]) );
	    catThr.push_back( new TLine(minBins,res["dmean_err"][i],  maxBins,res["dmean_err"][i]) );
	    catThr.push_back( new TLine(minBins,res["dmean_err"][i],  maxBins,res["dmean_err"][i]) );
	    optimThr[buf]=catThr;
	  }
	Int_t np=optimGr[buf][0]->GetN();
	optimGr[buf][0]->SetPoint(np,ibin,res["chi2"][i]);
	optimGr[buf][1]->SetPoint(np,ibin,fabs(res["dmean"][i]));
	optimGr[buf][2]->SetPoint(np,ibin,fabs(res["dq025"][i]));
	optimGr[buf][3]->SetPoint(np,ibin,fabs(res["dq05"][i]));
	optimGr[buf][4]->SetPoint(np,ibin,fabs(res["dq075"][i]));
      }
  }
  
  //now show results
  TCanvas *c= new TCanvas("c","c",1000,1000);
  c->Divide(5,optimGr.size());
  int ip(1);
  std::map< TString, float > optimResult;
  int icat(0);
  TH1F *optimH = new TH1F("optimh",";Category ;Optimal bin",optimGr.size(),0,optimGr.size());
  std::map<int,float> binTotalChi2;
  for(std::map<TString, std::vector<TGraph *> >::iterator it = optimGr.begin(); it!= optimGr.end(); it++,icat++)
    {
      std::map<TString,std::pair<float,float> > optimBins;

      for(size_t igr=0; igr<it->second.size(); igr++, ip++)
	{
	  c->cd(ip);
	  it->second[igr]->Draw("alp");
	  it->second[igr]->GetXaxis()->SetLabelSize(0.05);
	  it->second[igr]->GetXaxis()->SetTitleSize(0.07);
	  it->second[igr]->GetXaxis()->SetTitleOffset(0.7);
	  it->second[igr]->GetXaxis()->SetTitle("# bins");
	  it->second[igr]->GetXaxis()->SetNdivisions(5);
	  it->second[igr]->GetYaxis()->SetLabelSize(0.05);
	  it->second[igr]->GetYaxis()->SetTitleSize(0.07);
	  it->second[igr]->GetYaxis()->SetTitleOffset(0.75);
	  it->second[igr]->GetYaxis()->SetNdivisions(5);

	  TString ctrlTitle("#chi^{2}/dof");
	  if(igr==1) ctrlTitle="#Delta <L_{xy}> [cm]";
	  if(igr==2) ctrlTitle="#Delta q_{0.25}(L_{xy}) [cm]";
	  if(igr==3) ctrlTitle="#Delta q_{0.5}(L_{xy}) [cm]";
	  if(igr==4) ctrlTitle="#Delta q_{0.75}(L_{xy}) [cm]";
	  it->second[igr]->GetYaxis()->SetRangeUser(0,3);
	  if(igr>0) 	  it->second[igr]->GetYaxis()->SetRangeUser(0,0.08);
	  it->second[igr]->GetYaxis()->SetTitle(ctrlTitle);
	  
	  optimThr[it->first][igr]->SetLineColor(kGray);
	  optimThr[it->first][igr]->Draw();
	  
	  if(igr==0)
	    {
	      TPaveText *txt = new TPaveText(0.2,0.8,0.8,0.9,"NDC");
	      txt->SetBorderSize(0);
	      txt->SetFillStyle(0);
	      txt->SetTextFont(42);
	      txt->AddText(it->first);
	      txt->Draw();
	    }
	  else if(igr==2 || igr==3 || igr==4)
	    {
	      // cf. refernces in http://mathworld.wolfram.com/StatisticalMedian.html
	      for(int pt=0;pt<it->second[igr]->GetN(); pt++)
		{
		  Double_t x,y;
		  it->second[igr]->GetPoint(pt,x,y);
		  if(binTotalChi2.find(pt)==binTotalChi2.end()) binTotalChi2[pt]=0.;
		  binTotalChi2[pt]=binTotalChi2[pt]+pow(y,2);
		}
	    }
	}

      //find the bin with best chi2
      int optimBin(0);
      float minchi2(99999999999999.); 
      for(std::map<int,float>::iterator it=binTotalChi2.begin(); it!=binTotalChi2.end(); it++)
	{
	  if(it->second>minchi2) continue;
	  optimBin=it->first;
	  minchi2=it->second;
	}
      cout << " | optimal: " << optimBin << " | chi^2:" << minchi2 << endl;
      optimResult[ it->first ]=optimBin;
      optimH->Fill(icat, optimBin );
      optimH->GetXaxis()->SetBinLabel(icat+1 ,it->first);
    }
  c->Modified();
  c->Update();

  fOut->cd();
  optimH->Write();
  c->Write();
  std::vector<int> optimBins;
  for(int ibin=1; ibin<=optimH->GetXaxis()->GetNbins(); ibin++) optimBins.push_back( optimH->GetBinContent(ibin) );
  RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins);

  //now derive the target templates for the exclusive channels
  RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins,1);
  RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins,2);
  RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins,3);

  fOut->Close();
}

//
void ClassifyJetKinematics(TString iFile,TString jFile, TString kin, Float_t maxDRbj,TString optFile="",TString tag="", bool doPDF=false)
{
  
  TString pdfVarFileName=gSystem->BaseName(iFile);
  pdfVarFileName.ReplaceAll(".root","_pdf.root");
  pdfVarFileName = "/afs/cern.ch/user/p/psilva/public/lxy/"+pdfVarFileName;
  if(doPDF && !gSystem->AccessPathName(pdfVarFileName)) pdfUrl=pdfVarFileName;

  if(optFile=="") 
    {
      RunOptimization(iFile,jFile,kin,maxDRbj);
    }
  else{
    
    std::vector<int> optimBins;
    
    //get optimization result
    TFile *inF = TFile::Open(optFile);
    if(inF==0) { cout << "Can't open optimization file: " << optFile << endl; return; }
    TH1F *optimH=(TH1F *) inF->Get("optimh");
    if(optimH)
      {
	for(int ibin=1; ibin<=optimH->GetXaxis()->GetNbins(); ibin++) 
	  {
	    optimBins.push_back( optimH->GetBinContent(ibin) );
	  }
      }
    inF->Close();
    if(optimBins.size()==0) { cout << "Failed to read optimization result from  "<< optFile << endl; return; }

    //build the templates
    fOut = TFile::Open("LxyTemplates.root","UPDATE");
    RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins,-1, tag);
    RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins, 1, tag);
    RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins, 2, tag);
    RunJetClassification(iFile,jFile,kin,0, maxDRbj, &optimBins, 3, tag);
    fOut->cd();
    fOut->Close();
  }


}
