#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Utilities/interface/Exception.h>

#include "../interface/QGLikelihoodCalculatorOld.h"

#include "TMath.h"

using namespace std;

QGLikelihoodCalculatorOld::QGLikelihoodCalculatorOld(const TString dataDir, Bool_t useChs){
  TString chs = "";	
  if(useChs) chs = "_CHS";

  names.push_back("nPFCand_QC_ptCut");
  names.push_back("ptD_QC");
  names.push_back("axis2_QC");
 
  for(vector<TString>::iterator iStr = names.begin(); iStr!=names.end(); iStr++){
    JCP[(*iStr)+".quark"] = new JetCorrectorParameters(edm::FileInPath(dataDir + *iStr + "Jet0" + chs + ".txt").fullPath() ,"quark");
    JCP[(*iStr)+".gluon"] = new JetCorrectorParameters(edm::FileInPath(dataDir + *iStr + "Jet0" + chs + ".txt").fullPath() ,"gluon");
		
    JCP[(*iStr)+".F.quark"] = new JetCorrectorParameters(edm::FileInPath(dataDir + *iStr + "Jet0" + chs + "_F.txt").fullPath() ,"quark");
    JCP[(*iStr)+".F.gluon"] = new JetCorrectorParameters(edm::FileInPath(dataDir + *iStr + "Jet0" + chs + "_F.txt").fullPath() ,"gluon");
  }
  //check that provided files are for correct variables:
	
  for(vector<TString>::iterator iStr = names.begin(); iStr!=names.end(); iStr++){
    if(JCP[(*iStr)+".quark"]->definitions().level() != "QGL_"+(*iStr)+"_quark") throw cms::Exception("QuarkGluonTagger Config File Error")<<"quark section of file\'"<< *iStr <<"\' is not of the proper format. Check input files. First="<<JCP[(*iStr)+".quark"]->definitions().level()<<"- Second="<<(string("QGL")+(*iStr)+"_quark") <<"-";
    if(JCP[(*iStr)+".gluon"]->definitions().level() != "QGL_"+(*iStr)+"_gluon") throw cms::Exception("QuarkGluonTagger Config File Error")<<"gluon section of file\'"<< *iStr <<"\' is not of the proper format. Check input files.";
    if(JCP[(*iStr)+".F.quark"]->definitions().level() != "QGL_"+(*iStr)+"_F_quark") throw cms::Exception("QuarkGluonTagger Config File Error")<<"quark section of file\'"<< *iStr <<"_F\' is not of the proper format. Check input files.";
    if(JCP[(*iStr)+".F.gluon"]->definitions().level() != "QGL_"+(*iStr)+"_F_gluon") throw cms::Exception("QuarkGluonTagger Config File Error")<<"gluon section of file\'"<< *iStr <<"_F\' is not of the proper format. Check input files.";
  }

  for(vector<TString>::iterator iStr = names.begin(); iStr != names.end(); iStr++){
    SJC[(*iStr)+".quark"] = new SimpleJetCorrector(*JCP[(*iStr)+".quark"]);
    SJC[(*iStr)+".gluon"] = new SimpleJetCorrector(*JCP[(*iStr)+".gluon"]);
		
    SJC[(*iStr)+".F.quark"] = new SimpleJetCorrector(*JCP[(*iStr)+".F.quark"]);
    SJC[(*iStr)+".F.gluon"] = new SimpleJetCorrector(*JCP[(*iStr)+".F.gluon"]);
  }
}

//--------- Destructor ----------------------------------------------------------------------------------------
QGLikelihoodCalculatorOld::~QGLikelihoodCalculatorOld() {
  for(map<TString,JetCorrectorParameters*>::iterator it = JCP.begin(); it!=JCP.end(); it++) delete it->second;
  for(map<TString,SimpleJetCorrector*>::iterator it = SJC.begin(); it!=SJC.end(); it++) delete it->second;
	
  JCP.clear();
  SJC.clear();
}

Float_t QGLikelihoodCalculatorOld::QGvalue(map<TString, Float_t> variables){
  if( variables["pt"] < 20.) return -1.;

  std::vector<Float_t> v_pt_rho;
  v_pt_rho.push_back( variables["pt"] );
  v_pt_rho.push_back( variables["rhoIso"] );

  std::vector<Float_t> v_nPFCand_QC_ptCut;
  v_nPFCand_QC_ptCut.push_back( variables["mult"]);

  if(variables["axis2"] <=0 ) return -2.1;
  std::vector<Float_t> v_axis2_QC;
  v_axis2_QC.push_back( -TMath::Log( variables["axis2"]));

  std::vector<Float_t> v_ptD_QC;
  v_ptD_QC.push_back( variables["ptD"]);

  Float_t qProb=1.0;
  Float_t gProb=1.0;
	
  if(fabs(variables["eta"])<2.5){ //CENTRAL REGION
    qProb*=SJC["ptD_QC.quark"]->correction(v_pt_rho,v_ptD_QC);
    gProb*=SJC["ptD_QC.gluon"]->correction(v_pt_rho,v_ptD_QC);
	
    qProb*=SJC["nPFCand_QC_ptCut.quark"]->correction(v_pt_rho,v_nPFCand_QC_ptCut);
    gProb*=SJC["nPFCand_QC_ptCut.gluon"]->correction(v_pt_rho,v_nPFCand_QC_ptCut);
	
    qProb*=SJC["axis2_QC.quark"]->correction(v_pt_rho,v_axis2_QC);
    gProb*=SJC["axis2_QC.gluon"]->correction(v_pt_rho,v_axis2_QC);

  } else if(fabs(variables["eta"])>=2.5) {
    qProb*=SJC["ptD_QC.F.quark"]->correction(v_pt_rho,v_ptD_QC);
    gProb*=SJC["ptD_QC.F.gluon"]->correction(v_pt_rho,v_ptD_QC);
	
    qProb*=SJC["axis2_QC.F.quark"]->correction(v_pt_rho,v_axis2_QC);
    gProb*=SJC["axis2_QC.F.gluon"]->correction(v_pt_rho,v_axis2_QC);

    qProb*=SJC["nPFCand_QC_ptCut.F.quark"]->correction(v_pt_rho,v_nPFCand_QC_ptCut);
    gProb*=SJC["nPFCand_QC_ptCut.F.gluon"]->correction(v_pt_rho,v_nPFCand_QC_ptCut);
  }

  Float_t QGLikelihood = ( (qProb+gProb) > 0.) ? qProb/(qProb+gProb) : -1 ;
  return QGLikelihood;
}



