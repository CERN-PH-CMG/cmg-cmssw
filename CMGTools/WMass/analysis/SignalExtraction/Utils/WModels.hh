#include <TH1D.h>
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"

class CLucaModel0
{
public:
  CLucaModel0():model(0){}
  CLucaModel0(const char *name, RooRealVar &x);
  ~CLucaModel0() {
    delete a0;
    delete a1;
    delete a2;
    delete sigma1;
    delete sigma2;
    delete model;
  }
  RooRealVar *a0,*a1,*a2,*sigma1,*sigma2;
  RooGenericPdf *model;
};

class CPepeModel0
{
public:
  CPepeModel0():model(0){}
  CPepeModel0(const char *name, RooRealVar &x);
  ~CPepeModel0() {
    delete sigma;
    delete model;
  }
  RooRealVar *a0,*sigma;
  RooGenericPdf *model;
};

class CPepeModel1
{
public:
  CPepeModel1():model(0){}
  CPepeModel1(const char *name, RooRealVar &x);
  ~CPepeModel1() {
    delete sigma;
    delete a1;
    delete a2;
    delete mean1;
    delete model;
  }
  RooRealVar *sigma, *sigma2, *a1, *a2, *mean1;
  RooGenericPdf *model;
};

class CPepeModel2
{
public:
  CPepeModel2():model(0){}
  CPepeModel2(const char *name, RooRealVar &x);
  ~CPepeModel2() {
    delete a0;
    delete a1;
    delete a2;
    delete a3;
    delete model;
  }
  RooRealVar *a0, *a1, *a2, *a3;
  RooGenericPdf *model;
};

class CHistModel
{
public:
  CHistModel():model(0){}
  CHistModel(const char *name, RooRealVar &x, TH1D* hist, int intOrder=1);
  virtual ~CHistModel() { 
    delete inHist;
    delete dataHist;
    delete model;
  }
  TH1D        *inHist;
  RooDataHist *dataHist;
  RooHistPdf  *model;
};

class RhhCruijffPdf
{
public:
  RhhCruijffPdf():model(0){}
  RhhCruijffPdf(const char *name, RooRealVar &x);
  ~RhhCruijffPdf() {
    delete m0;
    delete sigmaL;
    delete sigmaR;
    delete alphaL;
    delete alphaR;
    delete model;
  }
  RooAbsReal *m0, *sigmaL, *sigmaR, *alphaL, *alphaR;
  // RooRealVar *a0, *a1, *a2, *a3;
  RooGenericPdf *model;
};

class TsallisPdf
{
public:
  TsallisPdf():model(0){}
  TsallisPdf(const char *name, RooRealVar &x);
  ~TsallisPdf() {
    delete Cq;
    delete q;
    delete T;
  }
  RooRealVar *Cq, *q, *T;
  // RooRealVar *a0, *a1, *a2, *a3;
  RooGenericPdf *model;
};

class ExpPdf
{
public:
  ExpPdf():model(0){}
  ExpPdf(const char *name, RooRealVar &x);
  ~ExpPdf() {
    delete Cq;
    delete q;
    // delete T;
  }
  RooRealVar *Cq, *q/* , *T */;
  // RooRealVar *a0, *a1, *a2, *a3;
  RooGenericPdf *model;
};

//--------------------------------------------------------------------------------------------------
CLucaModel0::CLucaModel0(const char *name, RooRealVar &x)
{
  char sigma1Name[50]; sprintf(sigma1Name,"sigma1_%s",name); sigma1 = new RooRealVar(sigma1Name,sigma1Name,10,2.5,20);
  char sigma2Name[50]; sprintf(sigma2Name,"sigma2_%s",name); sigma2 = new RooRealVar(sigma2Name,sigma2Name,10,2.5,20);
  char a0Name[50]; sprintf(a0Name,"a0_%s",name); a0 = new RooRealVar(a0Name,a0Name,15,5,20);
  char a1Name[50]; sprintf(a1Name,"a1_%s",name); a1 = new RooRealVar(a1Name,a1Name,5,0,20);
  char a2Name[50]; sprintf(a2Name,"a2_%s",name); a2 = new RooRealVar(a2Name,a2Name,15,10,20);
  
  // f(x) = (x)/s^2*exp[-(x)^2 / s^2]
  char formula[200];
  sprintf(formula,
          "(%s*%s)/TMath::Power(%s,2)*exp(-TMath::Power((%s-%s)/(%s+TMath::Sqrt(%s*%s)),2))",
      x.GetName(),x.GetName(),
                              sigma1Name,
                                                  x.GetName(),a0Name,
                                                                    sigma1Name,
                                                                            x.GetName(),a1Name
         );
  
  char vname[50];
  sprintf(vname,"Luca0Pdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula,RooArgSet(x,*a0,*a1,*sigma1));
}

//--------------------------------------------------------------------------------------------------
CPepeModel0::CPepeModel0(const char *name, RooRealVar &x)
{
  char sigmaName[50]; sprintf(sigmaName,"sigma_%s",name); sigma = new RooRealVar(sigmaName,sigmaName,10,5,20);
  char a0Name[50]; sprintf(a0Name,"a0_%s",name); a0 = new RooRealVar(a0Name,a0Name,15,10,20);
  
  // f(x) = (x)*exp[-(x-a0)^2 / s^2]
  char formula[200];
  sprintf(formula,
          "(%s)*(%s)*exp(-(%s-%s)*(%s-%s)/%s/%s)",
      x.GetName(),
      x.GetName(),
      // a0Name,
                    x.GetName(),a0Name,
                              x.GetName(),a0Name,
                                sigmaName,
                                       sigmaName);

  // // f(x) = x*exp[-x^2 / s^2]
  // char formula[200];
  // sprintf(formula,
          // "%s*%s*exp(-%s*%s/%s/%s)",
	  // x.GetName(),
	  // x.GetName(),
	  // x.GetName(),x.GetName(),
	  // sigmaName,sigmaName);
  
  char vname[50];
  sprintf(vname,"pepe0Pdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula,RooArgSet(x,*a0,*sigma));
}

// //--------------------------------------------------------------------------------------------------
// CPepeModel1::CPepeModel1(const char *name, RooRealVar &x)
// {
  // char sigmaName[50]; sprintf(sigmaName,"sigma_%s",name); sigma = new RooRealVar(sigmaName,sigmaName,10,5,20);
  // char a1Name[50];    sprintf(a1Name,   "a1_%s",   name); a1    = new RooRealVar(a1Name,a1Name,0.01,-1,1);
  
  // // f(x) = x*exp[-x^2 / (s + ax)^2] = x*exp[-x*x/(s*s + 2*a*x + a*a*x*x)]
  // char formula[200];
  // sprintf(formula,
          // "%s*exp(-%s*%s/(%s*%s + 2*%s*%s*%s + %s*%s*%s*%s))",
	  // x.GetName(),
	  // x.GetName(),x.GetName(),
	  // sigmaName,sigmaName,
	  // sigmaName,a1Name,x.GetName(),
	  // a1Name,a1Name,x.GetName(),x.GetName());
  
  // char vname[50];
  // sprintf(vname,"pepe1Pdf_%s",name);  
  // model = new RooGenericPdf(vname,vname,formula,RooArgSet(x,*sigma,*a1));
// }

//--------------------------------------------------------------------------------------------------
CPepeModel1::CPepeModel1(const char *name, RooRealVar &x)
{
  char sigmaName[50]; sprintf(sigmaName,"sigma_%s",name); sigma = new RooRealVar(sigmaName,sigmaName,5,1.5,20);
  char a1Name[50];    sprintf(a1Name,   "a1_%s",   name); a1    = new RooRealVar(a1Name,a1Name,0.01,-1,1);
  char a2Name[50];    sprintf(a2Name,   "a2_%s",   name); a2    = new RooRealVar(a2Name,a2Name,5,0,20);
  char sigmaName2[50]; sprintf(sigmaName2,"sigma2_%s",name); sigma2 = new RooRealVar(sigmaName2,sigmaName2,5,0.5,10);
  char mean1name[50]; sprintf(mean1name,"mean1_%s",name); mean1 = new RooRealVar(mean1name,mean1name,30,10,80);
  
  // // f(x) = x*x*x*exp[-x^2 / (s + ax)^2] = x*exp[-x*x/(s*s + 2*a*x + a*a*x*x)]
  // char formula[200];
  // sprintf(formula,
          // "%s*%s*%s*exp(-%s*%s/(%s*%s + 2*%s*%s*%s + %s*%s*%s*%s)) + %s*exp(-(%s-%s)*(%s-%s)/%s/%s)",
	  // x.GetName(),x.GetName(),x.GetName(),
	  // x.GetName(),x.GetName(),
	  // sigmaName,sigmaName,
	  // sigmaName,a1Name,x.GetName(),
	  // a1Name,a1Name,x.GetName(),x.GetName()
    	  // ,a2Name
	  // ,x.GetName()
    // ,mean1name
	  // ,x.GetName(),mean1name
	  // ,sigmaName,sigmaName
    // );
    
  // f(x) = x*exp[-x^2 / (s + ax)^2] = x*exp[-x*x/(s*s + 2*a*x + a*a*x*x)]
  char formula[200];
  sprintf(formula,
          "%s*exp(-%s*%s/(%s*%s + 2*%s*%s*%s + %s*%s*%s*%s)) + %s*exp(-(%s-%s)*(%s-%s)/%s/%s)",
	     x.GetName(),
	        x.GetName(),x.GetName(),
	                 sigmaName,sigmaName,
	                         sigmaName,a1Name,x.GetName(),
	                                a1Name,a1Name,x.GetName(),x.GetName()
                                                            ,a2Name
                                                              ,x.GetName(),mean1name
                                                                       ,x.GetName(),mean1name
                                                                               ,sigmaName,sigmaName
    );


  char vname[50];
  sprintf(vname,"pepe1Pdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula,RooArgSet(x,*sigma,*a1,*sigma2,*a2,*mean1));
}

//--------------------------------------------------------------------------------------------------
CPepeModel2::CPepeModel2(const char *name, RooRealVar &x)
{
  char a0Name[50]; sprintf(a0Name, "a0_%s", name); a0 = new RooRealVar(a0Name,a0Name,5,0,10);
  char a1Name[50]; sprintf(a1Name, "a1_%s", name); a1 = new RooRealVar(a1Name,a1Name,0.1,-1,1);
  char a2Name[50]; sprintf(a2Name, "a2_%s", name); a2 = new RooRealVar(a2Name,a2Name,1,0,10);
  char a3Name[50]; sprintf(a3Name, "a3_%s", name); a3 = new RooRealVar(a3Name,a3Name,100,70,400);
  
  // f(x) = x*exp[-x^2 / a*x*x + b*x + c]
  char formula[300];
  sprintf(formula,
          "%s + %s*exp(-%s*%s/(%s*%s*%s + %s*%s + %s))",
        a0Name,
          x.GetName(),
              x.GetName(),x.GetName(),
                      a1Name,x.GetName(),x.GetName(),
                                a2Name,x.GetName(),
                                              a3Name);
      
  char vname[50];
  sprintf(vname,"pepe2Pdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula,RooArgSet(x,*a0,*a1,*a2,*a3));
}

//--------------------------------------------------------------------------------------------------
CHistModel::CHistModel(const char *name, RooRealVar &x, TH1D* hist, int intOrder)
{
  char vname[100];
  
  sprintf(vname,"inHist_%s",name);   inHist   = (TH1D*)hist->Clone(vname);  
  sprintf(vname,"dataHist_%s",name); dataHist = new RooDataHist(vname,vname,RooArgSet(x),inHist);
  sprintf(vname,"histPdf_%s",name);  model    = new RooHistPdf(vname,vname,x,*dataHist,intOrder); 
}

//--------------------------------------------------------------------------------------------------
RhhCruijffPdf::RhhCruijffPdf(const char *name, RooRealVar &x)
{
  // char mName[50]; sprintf(mName, "m_%s", name); m = new RooRealVar(mName,mName,5,0,10);
  char m0Name[50]; sprintf(m0Name, "m0_%s", name); m0 = new RooRealVar(m0Name,m0Name,67,65,75);
  char sigmaLName[50]; sprintf(sigmaLName, "sigmaL_%s", name); sigmaL = new RooRealVar(sigmaLName,sigmaLName,3,0.1,10);
  char sigmaRName[50]; sprintf(sigmaRName, "sigmaR_%s", name); sigmaR = new RooRealVar(sigmaRName,sigmaRName,7,0.1,10);
  char alphaLName[50]; sprintf(alphaLName, "alphaL_%s", name); alphaL = new RooRealVar(alphaLName,alphaLName,1e-2,1e-6,1);
  char alphaRName[50]; sprintf(alphaRName, "alphaR_%s", name); alphaR = new RooRealVar(alphaRName,alphaRName,0.1,0.0001,1);
  
  TString dx = Form("(%s-%s)",x.GetName(),m0Name) ;
  TString sigma = Form("(%s<0 ? %s: %s)",dx.Data(),sigmaLName,sigmaRName) ;
  TString alpha = Form("(%s<0 ? %s: %s)",dx.Data(),alphaLName,alphaRName) ;
  TString f = Form("(2*%s*%s + %s*%s*%s)",sigma.Data(),sigma.Data(),alpha.Data(),dx.Data(),dx.Data()) ;
  // return exp(-dx*dx/f) ;
  TString formula = Form("exp(-%s*%s/%s)",dx.Data(),dx.Data(),f.Data());
      
  cout << formula << endl;
  
  char vname[50];
  sprintf(vname,"CruijffPdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula.Data(),RooArgSet(x,*m0,*sigmaL,*sigmaR,*alphaL,*alphaR));
}

//--------------------------------------------------------------------------------------------------
TsallisPdf::TsallisPdf(const char *name, RooRealVar &x)
{
  // char mName[50]; sprintf(mName, "m_%s", name); m = new RooRealVar(mName,mName,5,0,10);
  char CqName[50]; sprintf(CqName, "Cq_%s", name); Cq = new RooRealVar(CqName,CqName,9,0,10);
  char qName[50]; sprintf(qName, "q_%s", name); q = new RooRealVar(qName,qName,5,0.001,10);
  char TName[50]; sprintf(TName, "T_%s", name); T = new RooRealVar(TName,TName,1,0.01,5);

  TString formula = Form("%s*pow(1-(1-%s)*%s/%s,1/(1-%s))",CqName,qName,x.GetName(),TName,qName);
      
  cout << formula << endl;
  
  char vname[50];
  sprintf(vname,"CruijffPdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula.Data(),RooArgSet(x,*Cq,*q,*T));
}


//--------------------------------------------------------------------------------------------------
ExpPdf::ExpPdf(const char *name, RooRealVar &x)
{
  // char mName[50]; sprintf(mName, "m_%s", name); m = new RooRealVar(mName,mName,5,0,10);
  // char CqName[50]; sprintf(CqName, "Cq_%s", name); Cq = new RooRealVar(CqName,CqName,9,8.9,9.1);
  // char qName[50]; sprintf(qName, "q_%s", name); q = new RooRealVar(qName,qName,0.18,0.15,0.2);
  char CqName[50]; sprintf(CqName, "Cq_%s", name); Cq = new RooRealVar(CqName,CqName,9,5,15);
  char qName[50]; sprintf(qName, "q_%s", name); q = new RooRealVar(qName,qName,0.18,0.01,0.5);
  // char TName[50]; sprintf(TName, "T_%s", name); T = new RooRealVar(TName,TName,1,0.01,5);

  TString formula = Form("%s*exp(-%s*%s)",CqName,qName,x.GetName());
      
  cout << formula << endl;
  
  char vname[50];
  sprintf(vname,"ExpPdf_%s",name);  
  model = new RooGenericPdf(vname,vname,formula.Data(),RooArgSet(x,*Cq,*q/* ,*T */));
}
