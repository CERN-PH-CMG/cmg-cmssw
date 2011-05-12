#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"

#include <sstream>

#include "TCanvas.h"
#include "TVector2.h"
#include "TTree.h"
#include "TAxis.h"

#include "RooNLLVar.h"
#include "RooProdPdf.h"
#include "RooMinuit.h"
#include "RooDataSet.h"
#include "RooMinuit.h"
#include "RooPlot.h"

using namespace std;

//
ReducedMETFitter::ReducedMETFitter(const edm::ParameterSet &iConfig, size_t maxJets)
{
  TString phiFileName = iConfig.getParameter<std::string>("phiResolFileName"); 
  gSystem->ExpandPathName(phiFileName);
  TString ptFileName  = iConfig.getParameter<std::string>("ptResolFileName");  
  gSystem->ExpandPathName(ptFileName);
  
  //init the jet resolution models
  stdJetPhiResol_ = new JetResolution(phiFileName.Data(),false);
  stdJetPtResol_ = new JetResolution(ptFileName.Data(),false); 

  //dataset is the balance of the system in the long/perp direction
  //it is the same for any event as te balance is centered at 0
  float balance_long_val(0), balance_perp_val(0);
  TTree* tree = new TTree("tree","tree") ;
  tree->Branch("balance_long",&balance_long_val,"balance_long/F") ;
  tree->Branch("balance_perp",&balance_perp_val,"balance_perp/F") ;
  tree->Fill();
  balance_long = new RooRealVar("balance_long","balance_long",0);
  balance_perp = new RooRealVar("balance_perp","balance_perp",0);
  dataset = new RooDataSet("data","data",RooArgSet(*balance_perp,*balance_long),RooFit::Import(*tree));
  tree->Delete();
}

//
void ReducedMETFitter::compute(const LorentzVector &lep1, float sigmaPt1,
			       const LorentzVector &lep2, float sigmaPt2,
			       const LorentzVectorCollection &jets,
			       const LorentzVector &met)
{


   sigmaPt1=0.01*lep1.pt(); //fixme
   sigmaPt2=0.01*lep2.pt();

   //
   // 1. define the bisector
   //
   TVector2 lepton1(lep1.Px(), lep2.Py());
   TVector2 lepton2(lep1.Px(), lep2.Py());;
   TVector2 bisector = (lepton1.Unit()+lepton2.Unit()).Unit();
   TVector2 bisector_perp(bisector.Py(), -bisector.Px());
   if(lepton1 * bisector_perp < 0)  bisector_perp *= -1;
   RooRealVar px_bisect("px_bisect","px_bisect",bisector.X());
   RooRealVar py_bisect("py_bisect","py_bisect",bisector.Y());
   RooRealVar px_bisect_perp("px_bisect_perp","px_bisect_perp",bisector_perp.X());
   RooRealVar py_bisect_perp("py_bisect_perp","py_bisect_perp",bisector_perp.Y());

   //
   // 2. define the dilepton
   //
   
   //lepton #1
   RooRealVar avgpt_lep1("avgpt_lep1","avgpt_lep1",lep1.pt());
   RooRealVar sigmapt_lep1("sigmapt_lep1","sigmapt_lep1",sigmaPt1);
   RooRealVar pt_lep1("pt_lep1","pt_lep1",lep1.pt(),max(float(lep1.pt()-3*sigmaPt1),float(0)),lep1.pt()+3*sigmaPt1);
   RooGaussian ptconst_lep1("ptconst_lep1","ptconst_lep1",pt_lep1,avgpt_lep1,sigmapt_lep1);
   RooRealVar phi_lep1("phi_lep1","phi_lep1",lep1.phi());
   RooFormulaVar px_lep1("px_lep1","@0*cos(@1)",RooArgSet(pt_lep1,phi_lep1));
   RooFormulaVar py_lep1("py_lep1","@0*sin(@1)",RooArgSet(pt_lep1,phi_lep1));
  
   //lepton #2
   RooRealVar avgpt_lep2("avgpt_lep2","avgpt_lep2",lep2.pt());
   RooRealVar sigmapt_lep2("sigmapt_lep2","sigmapt_lep2",sigmaPt2);
   RooRealVar pt_lep2("pt_lep2","pt_lep2",lep2.pt(),max(float(lep2.pt()-3*sigmaPt2),float(0)),lep2.pt()+3*sigmaPt2);
   RooGaussian ptconst_lep2("ptconst_lep2","ptconst_lep2",pt_lep2,avgpt_lep2,sigmapt_lep2);
   RooRealVar phi_lep2("phi_lep2","phi_lep2",lep2.phi());
   RooFormulaVar px_lep2("px_lep2","@0*cos(@1)",RooArgSet(pt_lep2,phi_lep2));
   RooFormulaVar py_lep2("py_lep2","@0*sin(@1)",RooArgSet(pt_lep2,phi_lep2));
   
   //dilepton
   RooFormulaVar px_dilept("px_dilept","@0+@1",RooArgSet(px_lep1,px_lep2));
   RooFormulaVar pxErr_dilept("pxErr_dilept","sqrt((@0*cos(@2))^2+(@1*cos(@3))^2)",RooArgSet(sigmapt_lep1,sigmapt_lep2,phi_lep1,phi_lep2));
   RooFormulaVar py_dilept("py_dilept","@0+@1",RooArgSet(py_lep1,py_lep2));
   RooFormulaVar pyErr_dilept("pyErr_dilept","sqrt((@0*sin(@2))^2+(@1*sin(@3))^2)",RooArgSet(sigmapt_lep1,sigmapt_lep2,phi_lep1,phi_lep2));

   //
   // 3. define the reduced MET
   // 
   float centralLongVal=-px_dilept.getVal()*px_bisect.getVal()-py_dilept.getVal()*py_bisect.getVal();
   RooFormulaVar balance_long_err("balance_long_err","sqrt((@0*@2)^2+(@1*@3)^2)", RooArgSet(pxErr_dilept, pyErr_dilept, px_bisect, py_bisect));
   float sigmaLongIni=balance_long_err.getVal();
   RooRealVar redMet_long("redMet_long","redMet_long",centralLongVal,centralLongVal-10*sigmaLongIni,centralLongVal+10*sigmaLongIni);
   RooFormulaVar balance_long_avg("balance_long_avg","@0*@2+@1*@3+@4", RooArgSet(px_dilept, py_dilept, px_bisect, py_bisect,redMet_long));
   RooGaussian balance_long_model("balance_long_model","balance_long_model",*balance_long,balance_long_avg,balance_long_err);

   float centralPerpVal=-px_dilept.getVal()*px_bisect_perp.getVal()-py_dilept.getVal()*py_bisect_perp.getVal();
   RooFormulaVar balance_perp_err("balance_perp_err","sqrt((@0*@2)^2+(@1*@3)^2)", RooArgSet(pxErr_dilept, pyErr_dilept, px_bisect_perp, py_bisect_perp));
   float sigmaPerpIni=balance_long_err.getVal();
   RooRealVar redMet_perp("redMet_perp","redMet_perp",centralPerpVal,centralPerpVal-10*sigmaPerpIni,centralPerpVal+10*sigmaPerpIni);
   RooFormulaVar balance_perp_avg("balance_perp_avg","@0*@2+@1*@3+@4", RooArgSet(px_dilept, py_dilept, px_bisect_perp, py_bisect_perp,redMet_perp));
   RooGaussian balance_perp_model("balance_perp_model","balance_perp_model",*balance_perp,balance_perp_avg,balance_perp_err);   
   

   //define the model (product of all PDFs)
   RooArgList resolConstraintsList;
   resolConstraintsList.add(ptconst_lep1);
   resolConstraintsList.add(ptconst_lep2);
   RooArgSet allPdfs;
   allPdfs.add(balance_long_model);
   allPdfs.add(balance_perp_model);
   allPdfs.add(resolConstraintsList);
   RooProdPdf prodPdf("prodPdf","redMetmodel",allPdfs);

   //define the likelihood and fit it
   RooNLLVar *nll = (RooNLLVar *) prodPdf.createNLL(*dataset,RooFit::Constrain(resolConstraintsList));
   RooMinuit min(*nll) ;
   min.migrad() ;
   min.hesse() ;
   RooFitResult* r1 = min.save() ;
   r1->Print("v");


   //debug
   /*
   TCanvas *c=new TCanvas("c1","c1");
   c->cd();
   c->SetWindowSize(600,600);
   c->SetGridx();
   c->SetGridy();
   c->Divide(2,2);
   c->cd(1);
   RooPlot *frame = min.contour(redMet_long,redMet_perp,1,2,3) ;
   frame->SetTitle("Contour for 1s,2s,3s for red-MET L and red-MET T") ;
   frame->Draw();
   
   c->cd(2);
   frame = redMet_perp.frame(RooFit::Title("Transverse"),RooFit::Range(redMet_perp.getVal()-3*redMet_perp.getError(),redMet_perp.getVal()-3*redMet_perp.getError()));
   nll->plotOn(frame,RooFit::ShiftToZero());
   //   RooAbsReal* pll_redMet_perp = nll->createProfile(redMet_perp);
   //   pll_redMet_perp->plotOn(frame,RooFit::LineColor(kRed));
   frame->Draw();
   frame->GetXaxis()->SetTitleOffset(0.8);
   frame->GetYaxis()->SetTitle("-log(L/L_{max})");
   frame->GetYaxis()->SetTitleOffset(1);
   
   c->cd(3);
   frame = redMet_long.frame(RooFit::Title("Longitudinal"),RooFit::Range(redMet_long.getVal()-3*redMet_long.getError(),redMet_long.getVal()-3*redMet_long.getError()));
   nll->plotOn(frame,RooFit::ShiftToZero());
   //   RooAbsReal* pll_redMet_long = nll->createProfile(redMet_long);
   //   pll_redMet_long->plotOn(frame,RooFit::LineColor(kRed));
   frame->Draw();
   frame->GetXaxis()->SetTitleOffset(0.8);
   frame->GetYaxis()->SetTitle("-log(L/L_{max})");
   frame->GetYaxis()->SetTitleOffset(1);
   
   c->SaveAs("c1.C");
   */   


//   /*
//   //jets
//   stringstream sumJet_formula;
//   stringstream sumJetErr_formula;
//   sumJetErr_formula << "sqrt(";
  
//   RooArgList px_sum_args;
//   RooArgList py_sum_args;

//   RooArgList pxErr_sum_args;
//   RooArgList pyErr_sum_args;

//   for(size_t ijet=0; ijet<jets.size(); ijet++)
//     {

//       TVector2 jet(jets[ijet].Px(), jets[ijet].Py());
//       //       if(jet*bisector < 0 || jet*bisector_perp < 0) {

//       TString name("jet"); name+=(ijet+1);

//       TF1 *ptresolModel=stdJetPtResol_[ijet]->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt());
//       RooRealVar *ptresol=new RooRealVar("ptresol_"+name,"ptresol_"+name,0.1,2.);
//       RooAbsPdf *bindPtResolModel = RooFit::bindPdf(ptresolModel,*ptresol);
//       resolConstraintsList.add(*bindPtResolModel);
//       RooRealVar *avgpt_jet=new RooRealVar("avgpt_"+name,"avgpt_"+name,jets[ijet].pt());
//       RooFormulaVar *pt_jet = new RooFormulaVar("pt_"+name,"@0*@1",RooArgSet(*ptresol,*avgpt_jet));

//       TF1 *phiresolModel=stdJetPhiResol_[ijet]->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt());
//       RooRealVar *phiresol = new RooRealVar("phiresol_"+name,"phiresol_"+name,0.1,1.0);
//       RooAbsPdf *bindPhiResolModel = RooFit::bindPdf(phiresolModel,*phiresol);
//       resolConstraintsList.add(*bindPhiResolModel);
//       RooRealVar *avgphi_jet=new RooRealVar("avgphi_"+name,"avgphi_"+name,jets[ijet].phi());
//       RooFormulaVar *phi_jet= new RooFormulaVar("phi_"+name,"@0+@1",RooArgSet(*phiresol,*avgphi_jet));

//       cout << ptresolModel->GetRandom() << " " << phiresolModel->GetRandom() << endl;
      
//       RooFormulaVar *px_jet=new RooFormulaVar("px_"+name,"@0*cos(@1)",RooArgSet(*pt_jet,*phi_jet));
//       RooFormulaVar *py_jet= new RooFormulaVar("py_"+name,"@0*sin(@1)",RooArgSet(*pt_jet,*phi_jet));
//       px_sum_args.add(*px_jet);
//       py_sum_args.add(*py_jet);

//       RooFormulaVar *px_err_jet=new RooFormulaVar("px_err_"+name,"@0*cos(@1)",RooArgSet(*ptresol,*phi_jet));
//       RooFormulaVar *py_err_jet= new RooFormulaVar("py_err_"+name,"@0*sin(@1)",RooArgSet(*ptresol,*phi_jet));
//       pxErr_sum_args.add(*px_err_jet);
//       pyErr_sum_args.add(*py_err_jet);

//       if(ijet == 0) {
// 	sumJet_formula << "@";
// 	sumJetErr_formula << "(@";
//       } else {
// 	sumJet_formula << "+@";
// 	sumJetErr_formula << "+(@";
//       }
//       sumJet_formula << ijet;
//       sumJetErr_formula << ijet << "^2)";
      
//       // 	if(jet*bisector < 0) {
//       // 	  sumJetProj_long += jet*bisector;
//       // 	}
//       // 	if(jet*bisector_perp < 0) {
//       // 	  sumJetProj_perp += jet*bisector_perp;
//       // 	}
      
//       //       }
//     }
  
//   sumJetErr_formula << ")";

//   RooFormulaVar sumJetX("sumJetX",sumJet_formula.str().c_str(),px_sum_args);
//   RooFormulaVar sumJetY("sumJetY",sumJet_formula.str().c_str(),py_sum_args);

//   RooFormulaVar sumJetXErr("sumJetXErr",sumJetErr_formula.str().c_str(),pxErr_sum_args);
//   RooFormulaVar sumJetYErr("sumJetYErr",sumJetErr_formula.str().c_str(),pyErr_sum_args);


//   //red met
//   RooRealVar redMet_long("redMet_long","redMet_long",0., -200, 200);
//   RooRealVar redMet_perp("redMet_perp","redMet_perp",0., -200, 200);

//   RooFormulaVar redMet_long_avg("redMet_long_avg","(@0+@2)*@4+(@1+@3)*@5", RooArgSet(sumJetX, sumJetY, px_dilept, py_dilept, px_bisect, py_bisect));
//   RooFormulaVar redMet_perp_avg("redMet_perp_avg","(@0+@2)*@4+(@1+@3)*@5", RooArgSet(sumJetX, sumJetY, px_dilept, py_dilept, px_bisect_perp, py_bisect_perp));
  
//   RooFormulaVar redMet_long_err("redMet_long_err","sqrt((@0^2+@2^2)*@4^2+(@1^2+@3^2)*@5^2)", RooArgSet(sumJetXErr, sumJetYErr, pxErr_dilept, pyErr_dilept, px_bisect, py_bisect));
//   RooFormulaVar redMet_perp_err("redMet_perp_err","sqrt((@0^2+@2^2)*@4^2+(@1^2+@3^2)*@5^2)", RooArgSet(sumJetXErr, sumJetYErr, pxErr_dilept, pyErr_dilept, px_bisect_perp, py_bisect_perp));

//   RooGaussian redMet_long_gaussian("redMet_long_gaussian","redMet_long_gaussian",redMet_long,redMet_long_avg,redMet_long_err);
//   RooGaussian redMet_perp_gaussian("redMet_perp_gaussian","redMet_perp_gaussian",redMet_perp,redMet_perp_avg,redMet_perp_err);
//   */



  
//   //the model and the likelihood
//   RooArgSet allPdfs(redMet_long_gaussian, redMet_perp_gaussian);
//   allPdfs.add(resolConstraintsList);
//   RooProdPdf prodPdf("prodPdf","redMetmodel",allPdfs);
//   //  prodPdf.Print("v");

//   RooNLLVar *nll = (RooNLLVar *)prodPdf.createNLL(ds, RooFit::Constrain(resolConstraintsList));
//   //nll->Print("v");

//   //fit it
//   /*
//   RooMinuit min(*nll) ;
//   min.migrad() ;
//   min.hesse() ;
//   RooFitResult* r1 = min.save() ;
//   r1->Print("v");
//   */

//   //  if(fitType==0) modelconstr=new RooProdPdf("modelconstr"+tag,"model x product of constrains",RooArgSet(*mhfc,*model.alpha2_constrain,*model.alpha0_constrain,*model.eq_constrain));
//   //      if(fitType_==0) nll = modelconstr->createNLL(*ds,Constrain(RooArgSet(*model.alpha2,*model.alpha0,*model.eq_constrain)));

}

