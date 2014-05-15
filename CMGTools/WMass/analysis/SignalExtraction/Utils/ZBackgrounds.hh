#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooExponential.h"
#include "RooCMSShape.h"

class CBackgroundModel
{
public:
  CBackgroundModel():model(0){}
  virtual ~CBackgroundModel() { delete model; }
  RooAbsPdf *model;
};

class CExponential : public CBackgroundModel
{
public:
  CExponential(const char *name, RooRealVar &m);
  ~CExponential();
  RooRealVar *t;
};

class CErfExpo : public CBackgroundModel
{
public:
  CErfExpo(const char *name, RooRealVar &m);
  ~CErfExpo();
  RooRealVar *alfa, *beta, *gamma, *peak; 
};

class CDoubleExp : public CBackgroundModel
{
public:
  CDoubleExp(const char *name, RooRealVar &m);
  ~CDoubleExp();
  RooExponential *exp1, *exp2;
  RooRealVar *t1, *t2, *frac;
};

class CLinearExp : public CBackgroundModel
{
public:
  CLinearExp(const char *name, RooRealVar &m);
  ~CLinearExp();
  RooRealVar *a, *t;
};

class CQuadraticExp : public CBackgroundModel
{
public:
  CQuadraticExp(const char *name, RooRealVar &m);
  ~CQuadraticExp();
  RooRealVar *a1, *a2, *t;
};

//--------------------------------------------------------------------------------------------------
CExponential::CExponential(const char *name, RooRealVar &m)
{
  char vname[50];
  
  sprintf(vname,"t_%s",name);
  t = new RooRealVar(vname,vname,-0.1,-1.,0.);
      
  model = new RooExponential(name,name,m,*t);
}

CExponential::~CExponential()
{
  delete t;
}

//--------------------------------------------------------------------------------------------------
CErfExpo::CErfExpo(const char *name, RooRealVar &m)
{
  char vname[50];

  sprintf(vname,"alfa_%s",name);  alfa  = new RooRealVar(vname,vname,50,5,200);
  sprintf(vname,"beta_%s",name);  beta  = new RooRealVar(vname,vname,0.01,0,10);
  sprintf(vname,"gamma_%s",name); gamma = new RooRealVar(vname,vname,0.1,0,1); 
  
  sprintf(vname,"peak_%s",name);  
  peak = new RooRealVar(vname,vname,91.1876,85,97); 
  peak->setVal(91.1876);
  peak->setConstant(kTRUE);  
  
  model = new RooCMSShape(name,name,m,*alfa,*beta,*gamma,*peak);
}

CErfExpo::~CErfExpo()
{
  delete alfa;
  delete beta;
  delete gamma;
  delete peak;
}

//--------------------------------------------------------------------------------------------------
CDoubleExp::CDoubleExp(const char *name, RooRealVar &m)
{
  char vname[50];
 
  sprintf(vname,"t1_%s",name);   t1   = new RooRealVar(vname,vname,-0.20,-1.,0.);
  sprintf(vname,"t2_%s",name);   t2   = new RooRealVar(vname,vname,-0.05,-1.,0.);
  sprintf(vname,"frac_%s",name); frac = new RooRealVar(vname,vname, 0.50, 0.,1.);
    
  sprintf(vname,"exp1_%s",name);
  exp1 = new RooExponential(vname,vname,m,*t1);
  sprintf(vname,"exp2_%s",name);
  exp2 = new RooExponential(vname,vname,m,*t2);
  
  model = new RooAddPdf(name,name,RooArgList(*exp1,*exp2),RooArgList(*frac));
}

CDoubleExp::~CDoubleExp()
{
  delete exp1;
  delete exp2;
  delete t1;
  delete t2;
  delete frac;
}

//--------------------------------------------------------------------------------------------------
CLinearExp::CLinearExp(const char *name, RooRealVar &m)
{
  char aname[50]; 
  sprintf(aname,"a_%s",name);
  a = new RooRealVar(aname,aname,-0.007,-0.0083,10.);
  //a->setConstant(kTRUE);
  
  char tname[50];
  sprintf(tname,"t_%s",name);
  t = new RooRealVar(tname,tname,-1e-6,-10.,0.); 
  //t->setConstant(kTRUE); 
  
  char formula[200];
  sprintf(formula,"(1+%s*m)*exp(%s*m)",aname,tname);
 
  model = new RooGenericPdf(name,name,formula,RooArgList(m,*a,*t));
}

CLinearExp::~CLinearExp()
{
  delete a;
  delete t;
}

//--------------------------------------------------------------------------------------------------
CQuadraticExp::CQuadraticExp(const char *name, RooRealVar &m)
{
  char a1name[50]; 
  sprintf(a1name,"a1_%s",name);
  a1 = new RooRealVar(a1name,a1name,-0.007,-0.0083,10.);
  //a1->setConstant(kTRUE);
  
  char a2name[50]; 
  sprintf(a2name,"a2_%s",name);
  a2 = new RooRealVar(a2name,a2name,0.0,-10,10);
  //a2->setConstant(kTRUE);
  
  char tname[50];
  sprintf(tname,"t_%s",name);
  t = new RooRealVar(tname,tname,-1e-6,-10.,0.); 
  //t->setConstant(kTRUE); 
  
  char formula[200];
  sprintf(formula,"(1+%s*m+%s*m*m)*exp(%s*m)",a1name,a2name,tname);
 
  model = new RooGenericPdf(name,name,formula,RooArgList(m,*a1,*a2,*t));
}

CQuadraticExp::~CQuadraticExp()
{
  delete a1;
  delete a2;
  delete t;
}
