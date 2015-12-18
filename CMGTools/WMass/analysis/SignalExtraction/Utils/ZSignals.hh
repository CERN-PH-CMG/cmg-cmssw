#include "TROOT.h"
#include "TH1D.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooGaussian.h"
#include "RooFFTConvPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooVoigtianShape.h"
#include "RooKeysPdf.h"

class CSignalModel
{
public:
  CSignalModel():model(0){}
  virtual ~CSignalModel(){ delete model; }
  RooAbsPdf *model;
};

class CBreitWignerConvCrystalBall : public CSignalModel
{
public:
  CBreitWignerConvCrystalBall(const char *name, RooRealVar &m);
  ~CBreitWignerConvCrystalBall();
  RooRealVar     *mass, *width;
  RooBreitWigner *bw;
  RooRealVar     *mean, *sigma, *alpha, *n;
  RooCBShape     *cb;
};

class CMCTemplateConvGaussian : public CSignalModel
{
public:
  CMCTemplateConvGaussian(const char *name, RooRealVar &m, TH1D* hist, int intOrder=1);
  ~CMCTemplateConvGaussian();
  RooRealVar  *mean, *sigma;
  RooGaussian *gaus;
  TH1D        *inHist;
  RooDataHist *dataHist;
  RooHistPdf  *histPdf;
};

class CVoigtianCBShape : public CSignalModel
{
public:
  CVoigtianCBShape(const char *name, RooRealVar &m);
  ~CVoigtianCBShape();
  RooRealVar *mass, *width;
  RooRealVar *sigma, *alpha, *n;
};

class CMCDatasetConvGaussian : public CSignalModel
{
public:
  CMCDatasetConvGaussian(const char *name, RooRealVar &m, TTree* tree);
  ~CMCDatasetConvGaussian();
  RooRealVar  *mean, *sigma;
  RooGaussian *gaus;
  TTree       *inTree;
  RooDataSet  *dataSet;
  RooKeysPdf  *keysPdf;
};

//--------------------------------------------------------------------------------------------------
CBreitWignerConvCrystalBall::CBreitWignerConvCrystalBall(const char *name, RooRealVar &m)
{
  char vname[50];
  
  sprintf(vname,"mass_%s",name);
  mass = new RooRealVar(vname,vname,91,80,100);    
  mass->setVal(91.1876);
  mass->setConstant(kTRUE);
  
  sprintf(vname,"width_%s",name);
  width = new RooRealVar(vname,vname,2.5,0.1,10);    
  width->setVal(2.4952);
  width->setConstant(kTRUE);
  
  sprintf(vname,"bw_%s",name);
  bw = new RooBreitWigner(vname,vname,m,*mass,*width);

  sprintf(vname,"mean_%s",name);  mean  = new RooRealVar(vname,vname,0,-10,10);
  sprintf(vname,"sigma_%s",name); sigma = new RooRealVar(vname,vname,1,0.1,5);
  sprintf(vname,"alpha_%s",name); alpha = new RooRealVar(vname,vname,5,0,20);
  sprintf(vname,"n_%s",name);	  n     = new RooRealVar(vname,vname,1,0,10);
 
//  n->setVal(1.0);
//  n->setConstant(kTRUE);
  
  sprintf(vname,"cb_%s",name);
  cb = new RooCBShape(vname,vname,m,*mean,*sigma,*alpha,*n);
        
  model = new RooFFTConvPdf(name,name,m,*bw,*cb);
}

CBreitWignerConvCrystalBall::~CBreitWignerConvCrystalBall()
{
  delete mass;
  delete width;
  delete bw;
  delete mean;
  delete sigma;
  delete alpha;
  delete n;
  delete cb;
}

//--------------------------------------------------------------------------------------------------
CMCTemplateConvGaussian::CMCTemplateConvGaussian(const char *name, RooRealVar &m, TH1D* hist, int intOrder)
{  
  char vname[50];  
  
  sprintf(vname,"mean_%s",name);  mean  = new RooRealVar(vname,vname,0,-10,10);
  sprintf(vname,"sigma_%s",name); sigma = new RooRealVar(vname,vname,2,0,5);
  sprintf(vname,"gaus_%s",name);  gaus  = new RooGaussian(vname,vname,m,*mean,*sigma);

  sprintf(vname,"inHist_%s",hist->GetName());
  inHist = (TH1D*)hist->Clone(vname);
  
  sprintf(vname,"dataHist_%s",name); dataHist = new RooDataHist(vname,vname,RooArgSet(m),inHist);
  sprintf(vname,"histPdf_%s",name);  histPdf  = new RooHistPdf(vname,vname,m,*dataHist,intOrder);
  
  model = new RooFFTConvPdf(name,name,m,*histPdf,*gaus);
}

CMCTemplateConvGaussian::~CMCTemplateConvGaussian()
{
  delete mean;
  delete sigma;
  delete gaus;
  delete inHist;
  delete dataHist;
  delete histPdf;
}

//--------------------------------------------------------------------------------------------------
CVoigtianCBShape::CVoigtianCBShape(const char *name, RooRealVar &m)
{
  char vname[50];
  
  sprintf(vname,"mass_%s",name);
  mass = new RooRealVar(vname,vname,91,80,100);
    
  sprintf(vname,"width_%s",name);
  width = new RooRealVar(vname,vname,2.5,0.1,10);    
  width->setVal(2.4952);
  width->setConstant(kTRUE);
  
  sprintf(vname,"sigma_%s",name); sigma = new RooRealVar(vname,vname,1,0.1,10);
  sprintf(vname,"alpha_%s",name); alpha = new RooRealVar(vname,vname,5,0,20);
  
  sprintf(vname,"n_%s",name);     
  n = new RooRealVar(vname,vname,1,0,10);
  n->setVal(1.0);
  
  model = new RooVoigtianShape(name,name,m,*mass,*sigma,*alpha,*n,*width,0);  
}

CVoigtianCBShape::~CVoigtianCBShape()
{
  delete mass;
  delete width;
  delete sigma;
  delete alpha;
  delete n;
}

//--------------------------------------------------------------------------------------------------
CMCDatasetConvGaussian::CMCDatasetConvGaussian(const char *name, RooRealVar &m, TTree* tree)
{  
  char vname[50];  

  sprintf(vname,"mean_%s",name);  mean  = new RooRealVar(vname,vname,0,-10,10);
  sprintf(vname,"sigma_%s",name); sigma = new RooRealVar(vname,vname,2,0,5);
  sprintf(vname,"gaus_%s",name);  gaus  = new RooGaussian(vname,vname,m,*mean,*sigma);
  
  sprintf(vname,"inTree_%s",tree->GetName());
  inTree = (TTree*)tree->Clone(vname);
  
  sprintf(vname,"dataSet_%s",name); dataSet = new RooDataSet(vname,vname,inTree,RooArgSet(m));
  sprintf(vname,"keysPdf_%s",name); keysPdf = new RooKeysPdf(vname,vname,m,*dataSet,RooKeysPdf::NoMirror,1);

  model = new RooFFTConvPdf(name,name,m,*keysPdf,*gaus);
}

CMCDatasetConvGaussian::~CMCDatasetConvGaussian()
{
  delete mean;
  delete sigma;
  delete gaus;
  delete inTree;
  delete dataSet;
  delete keysPdf;
}
