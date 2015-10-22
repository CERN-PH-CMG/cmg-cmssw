#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <math.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include <TGraphAsymmErrors.h>
//#include <TLorentzVector.h>
#include <TMinuit.h>
#include "TKey.h"

#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooArgSet.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooDataHist.h>
#include <RooSimultaneous.h>
#include <RooNumConvPdf.h>
#include <RooMsgService.h>

#include <RooBreitWigner.h>
#include <RooCBShape.h>
#include <RooExponential.h>

#include <Math/Functor.h>
#include <Fit/Fitter.h>



using namespace std;
using namespace RooFit;

struct binSt{

  binSt() {};
  binSt(vector<float> pt, vector<float> eta) {
    binsPt=pt;
    binsEta=eta;

    nBPt=binsPt.size();
    nBEta=binsEta.size();
  }

  vector<float> binsPt;
  vector<float> binsEta;

  int nBPt;
  int nBEta;

  int getNProb() {return nBPt*nBEta;}
  int getNBPt() {return nBPt;}
  int getNBEta() {return nBEta;}

};

RooNumConvPdf* shapeZ(string tag, RooRealVar* x) {

  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);

  RooRealVar* mZ0=new RooRealVar( ("m_Z0_"+tag).c_str(),"Z0 mass", 91.188, "GeV/c^{2}" );
  RooRealVar* gammaZ0=new RooRealVar( ("gamma_Z0_"+tag).c_str(), "Z0 width",2.4952, "GeV/c^{2}" );
  RooBreitWigner* bw0=new RooBreitWigner( ("bw0"+tag).c_str(),"true BW",*x, *mZ0, *gammaZ0);

  RooRealVar* cb_bias=new RooRealVar( ("cbb_"+tag).c_str(), "bias",0.07, -3.0, 3.0 );
  RooRealVar* cb_width=new RooRealVar( ("cbw_"+tag).c_str(),"width", 1.,0.0,5 );
  RooRealVar* cb_alpha=new RooRealVar( ("cba_"+tag).c_str(),"alpha", 1.2,0.03,2.0 );
  RooRealVar* cb_power=new RooRealVar( ("cbn_"+tag).c_str(),"power", 5 );

  RooCBShape* cb_pdf=new RooCBShape( ("cb_pdf_"+tag).c_str(), "CB shape",
				     *x,*cb_bias, *cb_width, *cb_alpha, *cb_power );

  RooNumConvPdf* bw=new RooNumConvPdf( ("bw_"+tag).c_str(),"Convolution", *x, *cb_pdf, *bw0 );

  return bw;
}


RooAbsPdf* shapeSB(string tag, RooRealVar* x, RooRealVar* nSig, RooRealVar* nBkg) {


  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);

  RooNumConvPdf* bw = shapeZ(tag, x);

  RooRealVar* exp_tau=new RooRealVar( ("expt_"+tag).c_str(), "tau", -0.05, -40., -0.04);
  RooExponential* exp_pdf=new RooExponential( ("exp_pdf_"+tag).c_str(), "bkg shape", *x, *exp_tau );

  //RooRealVar* n_Z=new RooRealVar( ("N_{sig} "+tag).c_str(),"n Z events",501.0, 500., 10000000.);
  // RooRealVar* n_bkg=new RooRealVar( ("N_{bkg} "+tag).c_str(),"n bkg events", 3., 0., 600000.);
  RooArgList* listPdf=new RooArgList( *exp_pdf, *bw );
  RooArgList* listPdfVal=new RooArgList( *nBkg, *nSig );
  RooAddPdf* bw_tot=new RooAddPdf( "bw_EBEB_MC", "PDF ee", *listPdf, *listPdfVal );

  return bw_tot;

}


vector<float> doSingleFit(TH1* histo, bool isData) {

  RooMsgService::instance().getStream(1).removeTopic(RooFit::Eval); // 1 for INFO
  RooMsgService::instance().getStream(1).removeTopic(RooFit::NumIntegration);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::DataHandling);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Minimization);
  RooMsgService::instance().getStream(1).removeTopic(RooFit::Fitting);

  RooRealVar mass("mass","m_{ll}",60,120,"GeV");
  RooDataHist data("hist","hist",mass,histo);

  vector<float> v(4,0);

  if(data.sumEntries() < 0.001 || data.numEntries() <10 ) {
    v[0]=histo->Integral(histo->GetXaxis()->FindBin(70), histo->GetXaxis()->FindBin(110) );
    v[1]=sqrt( v[0] ); //not true for MC
    v[2]=histo->Integral()-v[0];
    v[3]=sqrt( v[2] );
    cout<<"result:\t"<<histo->GetName()<<"\t"<<v[0]<<"\t"<<v[1]<<endl;
    return v;
  }

  RooRealVar nSig("nSig","nSig",data.sumEntries(),0,10000000);
  RooRealVar nBkg("nBkg","nBkg",1.,0,10000000);

  // ostringstream os;
  // os<<categ;
  string os=(string)histo->GetName();
  RooAbsPdf* shape=shapeSB( ("s"+os),&mass, &nSig, &nBkg);

  RooFitResult* result;
  result = shape->fitTo( data ,RooFit::SumW2Error(kFALSE), RooFit::Save(kTRUE), RooFit::PrintLevel(-1) );

  double N=nSig.getVal();
  double eN=nSig.getError();

  double NB=nBkg.getVal();
  double eNB=nBkg.getError();

  cout<<"result:\t"<<histo->GetName()<<"\t"<<N<<"\t"<<eN<<endl;

  TCanvas* c=new TCanvas( ("c"+os).c_str(),("c"+os).c_str());
  RooPlot* frame=mass.frame();
  data.plotOn(frame);
  shape->plotOn(frame);
  frame->Draw();

  FILE *test=fopen( "plots", "r" );
  if( test==0 ) system( "mkdir plots");
  else fclose( test );

  string name="plots/fitData_";
  if(!isData) name="plots/fitMC_";
  c->SaveAs( (name+os+".png").c_str() );

  delete frame;
  delete c;

  delete shape;
  //delete data;


  v[0]=N;
  v[1]=eN;
  v[2]=NB;
  v[3]=eNB;

  return v;
}

void appendDataBase(string name, vector<float> vs, bool appendDb, string dbname) {
  ofstream ofile(dbname.c_str(), ios::out | (appendDb?(ios::app):(ios::trunc)) );
  ofile<<name<<"\t"<<vs[0]<<"\t"<<vs[1]<<"\t"<<vs[2]<<"\t"<<vs[3]<<endl;
}


map<string, vector<float> > doFits(string file, bool isData, bool appendDb,
				   string dbName, string singleCateg) {

  bool appDb=appendDb;

  TFile* f=new TFile(file.c_str(), "read");

  //scan the file content

  string name;
  vector<float> vs;
  map<string, vector<float> > vals;

  TIter nextkey(f->GetListOfKeys());
  TKey *key;
  while ((key = ((TKey*)nextkey()))) {
    TObject* obj = key->ReadObj();
    if( obj==nullptr ) continue;

    if(((string)obj->IsA()->GetName()).substr(0,3)=="TH1") {
      string name=(string)(obj->GetName());

      if(name.find("Pt_")==string::npos) continue;
      if( (isData && name.find("data_")==string::npos) ||
	  (!isData && name.find("DY_")==string::npos) )continue;

      if(singleCateg!="" && name.find(singleCateg)==string::npos) continue;

      vs = doSingleFit((TH1*)obj, isData);
      vals[ name ] = vs;

      if(appendDb) appendDataBase(name, vs, appDb, dbName);
      if(appDb==false) appDb=true; //otherwise we overwrite the file

    }

    if( (string)obj->IsA()->GetName()!=("TDirectoryFile") ) continue;
    TIter nextkeyD( ((TDirectory*)obj)->GetListOfKeys() );
    TKey *keyD;
    while ((keyD = ((TKey*)nextkeyD()))) {
      TObject* objD = keyD->ReadObj();
      if( objD==nullptr ) continue;

      if(((string)objD->IsA()->GetName()).substr(0,3)!="TH1") {

	name=(string)(obj->GetName());
	if(name.find("Pt_")==string::npos) continue;
	if( (isData && name.find("data_")==string::npos) ||
	    (!isData && name.find("DY_")==string::npos) )continue;

	if(singleCateg!="" && name.find(singleCateg)==string::npos) continue;

	vs = doSingleFit((TH1*)objD, isData);
	vals[ name ] = vs;

	if(appendDb) appendDataBase(name, vs, appDb, dbName);
	if(appDb==false) appDb=true; //otherwise we overwrite the file
      }

    }

  }

  return vals;

}


vector<float> parseCateg(string cat) {

  std::replace( cat.begin(), cat.end(), '_', ' ');
  istringstream iss(cat);
  vector<string> tks;
  copy(istream_iterator<string>(iss),
       istream_iterator<string>(),
       back_inserter<vector<string> >(tks));

  //cout<<cat<<endl;

  float pt1=atof( tks[2].c_str() );
  float pt2=atof( tks[6].c_str() );
  float eta1=atof( tks[4].c_str() );
  float eta2=atof( tks[8].c_str() );
  int os=atoi( tks[9].c_str() );

  float tmp;
  if(pt2>pt1) { //re-ordering if needed for pt
    tmp=pt2;
    pt2=pt1;
    pt1=tmp;
  }
  if(pt2==pt1 && eta2<eta1) { //re-ordering if needed for eta
    tmp=eta2;
    eta2=eta1;
    eta1=tmp;
  }

  vector<float> bin(5,0);
  bin[0]=pt1;
  bin[1]=eta1;
  bin[2]=pt2;
  bin[3]=eta2;
  bin[4]=os;

  return bin;
}



float getErr(float m1, float m2, float e1, float e2) {

  float e=pow(e1/m1,2)+pow(e2/m2,2);
  if(m1==0) return 1;
  return (m1/m2)*sqrt(e);
}


// function Object to be minimized
struct Chi2 {

  vector<std::pair<vector<float>, vector<int> > > _vals;

  void setPoint(float val, float eval, float p1, float p2) {

    vector<float> vals(2,0);
    vector<int> bins(2,0);

    vals[0]=val;
    vals[1]=eval;
    bins[0]=p1;
    bins[1]=p2;

    std::pair<vector<float>, vector<int> > p(vals,bins);
    _vals.push_back(p);

  }

  // implementation of the function to be minimized
  double operator() (const double * param) {
    double chi2=0;

    float val,eval;
    int p1,p2;
    for(unsigned int ip=0;ip<_vals.size();ip++) {
      val=_vals[ip].first[0];
      eval=_vals[ip].first[1];
      p1=_vals[ip].second[0];
      p2=_vals[ip].second[1];

      if(eval==0) eval=val;
      chi2 += pow( val-(param[p1]+param[p2]), 2)/pow(eval,2);
    }
    //cout<<" chi2: "<<chi2<<endl;
    return chi2;
  }

};

void fillPoints(vector<float>& binsPt, vector<float>& binsEta,
		const map<string, float> yields, const map<string, float> eyields,
		const map<string, vector<float> >& bins, Chi2& chi2 ) {

  std::sort(binsPt.begin(), binsPt.end() );
  std::sort(binsEta.begin(), binsEta.end() );


  //build the number of probabilities needed
  //  int nProb=binsPt.size()*binsEta.size(); //lower boundary exists but not higher boundary
  int nBEta=binsEta.size();

  //then, fill
  int bPt1, bEta1, bPt2, bEta2;
  int p1, p2;
  float prob, eprob, yd, yn, eyn, eyd;
  map<string, vector<float> >::const_iterator itv;
  map<string, float>::const_iterator it,it2;
  for(itv=bins.begin();itv!=bins.end();++itv) {

    if(itv->first.find("SS")!=string::npos) continue; //only looping over the OS categs

    //super ugly...
    bPt1=-1;bPt2=-1;
    for(size_t i=0;i<binsPt.size()-1;++i) {
      if(itv->second[0]>=binsPt[i] && itv->second[0]<binsPt[i+1])
	{ bPt1=i; }
      if(itv->second[2]>=binsPt[i] && itv->second[2]<binsPt[i+1])
	{ bPt2=i; }
    }
    if(bPt1==-1) bPt1=binsPt.size()-1;
    if(bPt2==-1) bPt2=binsPt.size()-1;

    bEta1=-1;bEta2=-1;
    for(size_t i=0;i<binsEta.size()-1;++i) {
      if(itv->second[1]>=binsEta[i] && itv->second[1]<binsEta[i+1])
	{ bEta1=i; }
      if(itv->second[3]>=binsEta[i] && itv->second[3]<binsEta[i+1])
	{ bEta2=i; }
    }

    if(bEta1==-1) bEta1=binsEta.size()-1;
    if(bEta2==-1) bEta2=binsEta.size()-1;

    p1=bPt1*nBEta + bEta1;
    p2=bPt2*nBEta + bEta2;

    it=yields.find( itv->first );
    it2=yields.find( (itv->first.substr(0, itv->first.size()-2)+"SS") );
    yd=it->second;
    yn=it2->second;

    it=eyields.find( itv->first );
    it2=eyields.find( (itv->first.substr(0, itv->first.size()-2)+"SS") );
    eyd=it->second;
    eyn=it2->second;

    prob=yn/yd;
    eprob=getErr(yn, yd, eyn, eyd);

    //FIXME
    if(yd==0) {prob=0; eprob=1;}

    chi2.setPoint( prob, eprob, p1, p2);

    if(p1==p2) {
      cout<<" CHECK probability ("<<itv->second[0]<<","<<itv->second[1]
	  <<") : "<<yn<<"+-"<<eyn<<" / "<<yd<<"+-"<<eyd<<" ==> "<<prob<<" +- "<<eprob<<endl;
    }

    // cout<<itv->first<<" ==> "<<bPt1<<"/"<<bEta1<<":"<<p1<<"   "<<bPt2<<"/"<<bEta2<<":"<<p2<<endl;
    //" :: "<<yn<<"  "<<yd<<"  "<<yn/yd<<"      "
    	// <<itv->second[0]<<"  "<<itv->second[1]<<"  "
    	// <<itv->second[2]<<"  "<<itv->second[3]<<endl;

  }

}

binSt setPointsFromDB(string file, Chi2& chi2) {

  ifstream categs(file.c_str(), ios::in);
  string line;

  vector<float> binsPt;
  vector<float> binsEta;

  map<string, float> yields;
  map<string, float> eyields;
  map<string, vector<float> > bins;

  //first read
  vector<float> bin;
  float y, ey;
  while(getline(categs, line))
    {
      istringstream iss(line);
      vector<string> tks;
      copy(istream_iterator<string>(iss),
	   istream_iterator<string>(),
	   back_inserter<vector<string> >(tks));

      bin = parseCateg(tks[0]);
      y= atof(tks[1].c_str() );
      ey= atof(tks[2].c_str() );

      bool flag=true;
      for(size_t i=0;i<binsPt.size();i++) {
	if(binsPt[i]==bin[0]) { flag=false; break;}
      }
      if(flag) binsPt.push_back(bin[0]);

      flag=true;
      for(size_t i=0;i<binsEta.size();i++) {
	if(binsEta[i]==bin[1]) { flag=false; break;}
      }
      if(flag) binsEta.push_back(bin[1]);

      bins[tks[0]] = bin;
      yields[tks[0]] = y;
      eyields[tks[0]] = ey;
    }

  fillPoints( binsPt, binsEta, yields, eyields, bins, chi2);

  binSt binstruct(binsPt, binsEta);
  return binstruct;

}


binSt setPoints(map<string, vector<float> > vals, Chi2& chi2) {

  //first read the categories
  vector<float> binsPt;
  vector<float> binsEta;

  map<string, float> yields;
  map<string, float> eyields;
  map<string, vector<float> > bins;

  vector<float> bin;
  float y, ey;

  map<string, vector<float> >::const_iterator it;
  for(it=vals.begin();it!=vals.end();++it) {

    bin = parseCateg(it->first);
    y= it->second[0];
    ey= it->second[1];

    bool flag=true;
    for(size_t i=0;i<binsPt.size();i++) {
      if(binsPt[i]==bin[0]) { flag=false; break;}
    }
    if(flag) binsPt.push_back(bin[0]);

    flag=true;
    for(size_t i=0;i<binsEta.size();i++) {
      if(binsEta[i]==bin[1]) { flag=false; break;}
    }
    if(flag) binsEta.push_back(bin[1]);

    bins[it->first] = bin;
    yields[it->first] = y;
    eyields[it->first] = ey;
  }


 fillPoints( binsPt, binsEta, yields, eyields, bins, chi2);

 binSt binsstruct(binsPt, binsEta);
 return binsstruct;
}


int main(int argc, char* argv[]) {

  string file;
  bool isData=false;
  bool isRootFile=true;
  string singleCateg="";
  bool appendDb=true;
  string dbName="database.db";

  char c;

  while ((c = getopt(argc, argv, "f:d:s:D:a:n:h")) != -1 ) {
    switch (c) {
      //case 'd': { file=optarg; break;}
    case 'f': { file=string(optarg); break;}
    case 'd': { isRootFile=bool(1-atoi(optarg)); break;}
    case 's': { singleCateg=string(optarg); break;}
    case 'D': { isData=bool(atoi(optarg)); break;}
    case 'a': { appendDb=bool(atoi(optarg)); break;}
    case 'n': { dbName=string(optarg); break;}
    case 'h': {
      cout<<"configuration options:\n -f : file to read (root or ASCII) \n -d proceed with a database reading instead of making fits (0 per default). \n -s <categ> perform a fit over a single Z category. \n -D run on data (0 per default). \n -a do not store the numbers into a b (1 per default). \n -n set the database file name for reading (database.db per default). \n -h help \n"<<endl;
      return 0; }
    default : {
      cout<<"configuration options:\n -f : file to read (root or ASCII) \n -d proceed with a database reading instead of making fits (0 per default). \n -s <categ> perform a fit over a single Z category. \n -D run on data (0 per default). \n -a do not store the numbers into a b (1 per default). \n -n set the database file name for reading (database.db per default). \n -h help \n"<<endl;
      return 0; }
    }
  }

  //==============================================



  Chi2 chi2;binSt bins;
  if(!isRootFile) { //read the DB file
    bins=setPointsFromDB(file, chi2);
  }
  else { //read root file
    map<string, vector<float> > vals=doFits(file, isData, appendDb,
					    dbName, singleCateg);
    bins= setPoints(vals,chi2);
  }


  //perform the final fit ====================
  int nvars=bins.getNProb();

  ROOT::Fit::Fitter  fitter;
  ROOT::Math::Functor fcn(chi2,nvars);

  //bloody ROOT and lack of vector handling
  double* vars= new double[nvars];
  fitter.SetFCN(fcn,vars);

  // set step sizes and limits
  for (int i=0; i<nvars; ++i) {
    fitter.Config().ParSettings(i).SetStepSize(0.000001);
    fitter.Config().ParSettings(i).SetLimits(0,0.2);
  }

  bool ok = fitter.FitFCN();
  if (!ok) {
    cout<<" The final fit did not converged properly, please check your data and the read/written database "<<endl;
    return 1;
  }

  fitter.CalculateMinosErrors();
  fitter.CalculateHessErrors();
  const ROOT::Fit::FitResult & result = fitter.Result();
  result.Print(std::cout);

  const double * parFit = result.GetParams();
  const double * parErrs = result.GetErrors();
  cout<<"probabilities (i/ptbin/etabin) and fit result"<<endl;
  for(int i=0;i<nvars;i++)
    cout<<i<<"    "<<i/bins.nBEta<<"   "<<i%bins.nBEta<<" ==> "<<parFit[i]<<" +- "<<parErrs[i]<<endl;

  size_t p=file.find(".root");
  if(!isRootFile) p=file.find(".db");
  string tag=file.substr(0, p);

  string fname=tag+(isData?"_data":"_MC")+".root";
  TFile* f=new TFile(fname.c_str(),"recreate");
  f->cd();

  //adding the last bin boundaries, bloody root...
  double* binsPt=new double[ bins.nBPt+1 ];
  double* binsEta=new double[ bins.nBEta+1 ];

  for(size_t ib=0;ib<max( bins.nBPt, bins.nBEta );ib++) {
    if(ib<bins.nBPt) binsPt[ib]= bins.binsPt[ib];
    if(ib<bins.nBEta) binsEta[ib]= bins.binsEta[ib];
  }
  binsPt[bins.nBPt] = 1000;
  binsEta[bins.nBEta] = 2.5;

  TH2F* h=new TH2F("chargeMisId","chargeMisId;p_{T}(e) [GeV];#eta(e) ",bins.nBPt,binsPt,bins.nBEta,binsEta);
  for(int i=0;i<nvars;i++) {
    h->SetBinContent((i/bins.nBEta)+1,(i%bins.nBEta)+1, parFit[i]);
    h->SetBinError((i/bins.nBEta)+1,(i%bins.nBEta)+1, parErrs[i]);
  }

  f->Write();
  f->Close();

}
