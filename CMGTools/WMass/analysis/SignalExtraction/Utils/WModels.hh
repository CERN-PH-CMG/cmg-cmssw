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
