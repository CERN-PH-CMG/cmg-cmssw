#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETFitter.h"

//
ReducedMETFitter::ReducedMETFitter(const edm::ParameterSet &iConfig, size_t maxJets)
{
  TString phiFileName = iConfig.getParameter<std::string>("phiResolFileName"); 
  gSystem->ExpandPathName(phiFileName);
  TString ptFileName  = iConfig.getParameter<std::string>("ptResolFileName");  
  gSystem->ExpandPathName(ptFileName);
  
  //init the jet resolution models
  for(size_t ijet=0; ijet<maxJets; ijet++)
    { 
      stdJetPhiResol_.push_back( new JetResolution(phiFileName.Data(),false) );
      stdJetPtResol_.push_back( new JetResolution(ptFileName.Data(),false) ); 
    }
}

//
void ReducedMETFitter::compute(const LorentzVector &lep1, float sigmaPt1,
			       const LorentzVector &lep2, float sigmaPt2,
			       const LorentzVectorCollection &jets,
			       const LorentzVector &met)
{

  //constraints for resolutions
  RooArgList resolConstraintsList;

  //lepton 1
  RooRealVar avgpt_lep1("avgpt_lep1","avgpt_lep1",lep1.pt());
  RooRealVar sigmapt_lep1("sigmapt_lep1","sigmapt_lep1",sigmaPt1);
  RooRealVar pt_lep1("pt_lep1","pt_lep1",lep1.pt(),0,lep1.pt()+3*sigmaPt1);
  RooGaussian ptconst_lep1("ptconst_lep1","ptconst_lep1",pt_lep1,avgpt_lep1,sigmapt_lep1);
  resolConstraintsList.add(ptconst_lep1);
  RooRealVar phi_lep1("phi_lep1","phi_lep1",lep1.phi());
  RooFormulaVar px_lep1("px_lep1","@0*cos(@1)",RooArgSet(pt_lep1,phi_lep1));
  RooFormulaVar py_lep1("py_lep1","@0*sin(@1)",RooArgSet(pt_lep1,phi_lep1));

  //lepton 2
  RooRealVar avgpt_lep2("avgpt_lep2","avgpt_lep2",lep2.pt());
  RooRealVar sigmapt_lep2("sigmapt_lep2","sigmapt_lep2",sigmaPt2);
  RooRealVar pt_lep2("pt_lep2","pt_lep2",lep2.pt(),0,lep2.pt()+3*sigmaPt2);
  RooGaussian ptconst_lep2("ptconst_lep2","ptconst_lep2",pt_lep2,avgpt_lep2,sigmapt_lep2);
  resolConstraintsList.add(ptconst_lep1);
  RooRealVar phi_lep2("phi_lep2","phi_lep2",lep2.phi());
  RooFormulaVar px_lep2("px_lep2","@0*cos(@1)",RooArgSet(pt_lep2,phi_lep2));
  RooFormulaVar py_lep2("py_lep2","@0*sin(@1)",RooArgSet(pt_lep2,phi_lep2));

  //jets
  for(size_t ijet=0; ijet<jets.size(); ijet++)
    {
      TString name("jet"); name+=(ijet+1);

      TF1 *ptresolModel=stdJetPtResol_[ijet]->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt());
      RooRealVar *ptresol=new RooRealVar("ptresol_"+name,"ptresol_"+name,0,10);
      RooAbsPdf *bindPtResolModel = RooFit::bindPdf(ptresolModel,*ptresol);
      resolConstraintsList.add(*bindPtResolModel);
      RooRealVar *avgpt_jet=new RooRealVar("avgpt_"+name,"avgpt_"+name,jets[ijet].pt());
      RooFormulaVar *pt_jet = new RooFormulaVar("pt_"+name,"@0*@1",RooArgSet(*ptresol,*avgpt_jet));

      TF1 *phiresolModel=stdJetPhiResol_[ijet]->resolutionEtaPt(jets[ijet].eta(),jets[ijet].pt());
      RooRealVar *phiresol = new RooRealVar("phiresol_"+name,"phiresol_"+name,0,10);
      RooAbsPdf *bindPhiResolModel = RooFit::bindPdf(phiresolModel,*phiresol);
      resolConstraintsList.add(*bindPhiResolModel);
      RooRealVar *avgphi_jet=new RooRealVar("avgpt_"+name,"avgpt_"+name,jets[ijet].phi());
      RooFormulaVar *phi_jet= new RooFormulaVar("phi_"+name,"@0+@1",RooArgSet(*phiresol,*avgphi_jet));
      
      RooFormulaVar *px_jet=new RooFormulaVar("px_"+name,"@0*cos(@1)",RooArgSet(*pt_jet,*phi_jet));
      RooFormulaVar *py_jet= new RooFormulaVar("py_"+name,"@0*sin(@1)",RooArgSet(*pt_jet,*phi_jet));
    }

  //  if(fitType==0) modelconstr=new RooProdPdf("modelconstr"+tag,"model x product of constrains",RooArgSet(*mhfc,*model.alpha2_constrain,*model.alpha0_constrain,*model.eq_constrain));
  //      if(fitType_==0) nll = modelconstr->createNLL(*ds,Constrain(RooArgSet(*model.alpha2,*model.alpha0,*model.eq_constrain)));
}

