#include "UserCode/EWKV/interface/MacroUtils.h"
#include "TH1F.h"
#include "TSystem.h"

namespace utils
{
  namespace cmssw
  {

    //
    MuScleFitCorrector *getMuonCorrector(TString baseDir,TString url)
    {
      bool is7TeV(url.Contains("7TeV"));
      bool isData(url.Contains("Data"));
      bool is2012D(url.Contains("2012D"));
      TString fName("");
      if(is7TeV) return 0;
      if(!is7TeV && !isData) fName="MuScleFit_2012_MC_53X_smearReReco.txt";
      if(!is7TeV && isData)
	{
	  if(is2012D) fName="MuScleFit_2012D_DATA_ReReco_53X.txt";
	  else        fName="MuScleFit_2012ABC_DATA_ReReco_53X.txt";
	}
      fName=baseDir+"/"+fName;
      return new MuScleFitCorrector(fName);
    }

    //
    FactorizedJetCorrector *getJetCorrector(TString baseDir, bool isMC)
    {
      gSystem->ExpandPathName(baseDir);
      TString pf(isMC ? "MC" : "Data");

      //order matters: L1 -> L2 -> L3 (-> Residuals)
      std::vector<std::string> jetCorFiles;
      std::cout << baseDir+"/"+pf+"_L1FastJet_AK5PFchs.txt" << std::endl;
      jetCorFiles.push_back((baseDir+"/"+pf+"_L1FastJet_AK5PFchs.txt").Data());
      jetCorFiles.push_back((baseDir+"/"+pf+"_L2Relative_AK5PFchs.txt").Data());
      jetCorFiles.push_back((baseDir+"/"+pf+"_L3Absolute_AK5PFchs.txt").Data());
      if(!isMC) jetCorFiles.push_back((baseDir+"/"+pf+"_L2L3Residual_AK5PFchs.txt").Data());

      //init the parameters for correction
      std::vector<JetCorrectorParameters> corSteps;
      for(size_t i=0; i<jetCorFiles.size(); i++) corSteps.push_back(JetCorrectorParameters(jetCorFiles[i]));

      //return the corrector
      return new FactorizedJetCorrector(corSteps);
    }
 
    //
    const reco::Candidate *getGeneratorFinalStateFor(const reco::Candidate *p)
    {
      if(p==0) return 0;
      
      const reco::Candidate *prevState=p;
      do{	
	const reco::Candidate *nextState=0;
	int nDaughters = prevState->numberOfDaughters();
	for(int iDaughter=0; iDaughter<nDaughters; iDaughter++)
	  {
	    const reco::Candidate *dau = prevState->daughter(iDaughter);
	    if(dau==0) continue;
	    if(dau->pdgId()!= p->pdgId()) continue;
	    nextState=dau;	   
	    break;
	  }
	if(nextState==0) break;
	if(nextState==prevState) break;
	prevState=nextState;
      }while(1);
      
      return prevState;
    }

    //
    bool isBhadron(int pdgId)
    {
      int absid=abs(pdgId);
      return ( (absid>= 5122 && absid<= 5554) ||    //baryons
	       (absid>=20513 && absid<=20543) ||    //mesons
	       (absid>=10511 && absid<=10543) || 
	       (absid>=  511 && absid<=  545) ||
	       (absid>=  551 && absid<=  557) ||    //bbar mesons
	       (absid>=10551 && absid<=10557) ||
	       (absid>=100551 && absid<=100557) ||
	       (absid>=110551 && absid<=110557) ||
	       (absid>=200551 && absid<=200557) ||
	       (absid>=210551 && absid<=210557) );
    }

    //cf. https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    std::vector<float> smearJER(float pt, float eta, float genPt)
    {
      std::vector<float> toReturn(3,pt);
      if(genPt<=0) return toReturn;
      
      //
      eta=fabs(eta);
      double ptSF(1.0), ptSF_err(0.06);
      if(eta<0.5)                  { ptSF=1.052; ptSF_err=sqrt(pow(0.012,2)+pow(0.5*(0.062+0.061),2)); }
      else if(eta>=0.5 && eta<1.1) { ptSF=1.057; ptSF_err=sqrt(pow(0.012,2)+pow(0.5*(0.056+0.055),2)); }
      else if(eta>=1.1 && eta<1.7) { ptSF=1.096; ptSF_err=sqrt(pow(0.017,2)+pow(0.5*(0.063+0.062),2)); }
      else if(eta>=1.7 && eta<2.3) { ptSF=1.134; ptSF_err=sqrt(pow(0.035,2)+pow(0.5*(0.087+0.085),2)); }
      else if(eta>=2.3 && eta<5.0) { ptSF=1.288; ptSF_err=sqrt(pow(0.127,2)+pow(0.5*(0.155+0.153),2)); }
      
      toReturn[0]=TMath::Max(0.,(genPt+ptSF*(pt-genPt)));
      toReturn[1]=TMath::Max(0.,(genPt+(ptSF+ptSF_err)*(pt-genPt)));
      toReturn[2]=TMath::Max(0.,(genPt+(ptSF-ptSF_err)*(pt-genPt)));
      return toReturn;
    }

    //
    std::vector<float> smearJES(float pt, float eta, JetCorrectionUncertainty *jecUnc)
    {
      jecUnc->setJetEta(eta);
      jecUnc->setJetPt(pt);
      float relShift=fabs(jecUnc->getUncertainty(true));
      std::vector<float> toRet;
      toRet.push_back((1.0+relShift)*pt);
      toRet.push_back((1.0-relShift)*pt);
      return toRet;
    }


    //
    PuShifter_t getPUshifters(std::vector< float > &Lumi_distr, float puUnc)
    {
      Int_t NBins = Lumi_distr.size();
      TH1F *pu=new TH1F("putmp","",NBins,-0.5,float(NBins)-0.5);
      TH1F *puup=(TH1F *)pu->Clone("puuptmp");
      TH1F *pudown=(TH1F *)pu->Clone("pudowntmp");
      for(size_t i=0; i<Lumi_distr.size(); i++)  pu->SetBinContent(i+1,Lumi_distr[i]);
      
      for(int ibin=1; ibin<=pu->GetXaxis()->GetNbins(); ibin++)
	{
	  Double_t xval=pu->GetBinCenter(ibin);
	  TGraph *gr = new TGraph;
	  for(int ishift=-3; ishift<3; ishift++)
	    {
	      if(ibin+ishift<0) continue;
	      if(ibin+ishift>pu->GetXaxis()->GetNbins()) continue;
	      
	      gr->SetPoint(gr->GetN(),xval+ishift,pu->GetBinContent(ibin+ishift));
	    }
	  if(gr->GetN()>1)
	    {
	      Double_t newval(gr->Eval(xval*(1+puUnc)));
	      pudown->SetBinContent(ibin,newval>0?newval:0.0);
	      newval=gr->Eval(xval*(1-puUnc));
	      puup->SetBinContent(ibin,newval>0?newval:0.0);
	    }
	  delete gr;
	}
      puup->Scale(pu->Integral()/puup->Integral());
      pudown->Scale(pu->Integral()/pudown->Integral());
      std::cout << "getPUshifts will shift average PU by " << puup->GetMean()-pu->GetMean() << " / " << pudown->GetMean()-pu->GetMean() << std::endl; 
      
      puup->Divide(pu);    TGraph *puupWgt = new TGraph(puup);
      pudown->Divide(pu);  TGraph *pudownWgt = new TGraph(pudown);
      delete puup;
      delete pudown;  
      delete pu;
      
      PuShifter_t res(2);
      res[PUDOWN] = pudownWgt;
      res[PUUP]   = puupWgt;
      return res;
    }
    

    //
    Float_t getEffectiveArea(int id,float eta,int cone,TString isoSum)
    {
      Float_t Aeff(1.0);
      if(abs(id)==11)
	{
	  if(fabs(eta)<1.0)                         Aeff=(cone==3? 0.100 : 0.180);
	  else if(fabs(eta)>1.0 && fabs(eta)<1.479) Aeff=(cone==3? 0.120 : 0.200);
	  else if(fabs(eta)>1.479 && fabs(eta)<2.0) Aeff=(cone==3? 0.085 : 0.150);
	  else if(fabs(eta)>2.0 && fabs(eta)<2.2)   Aeff=(cone==3? 0.110 : 0.190);
	  else if(fabs(eta)>2.2 && fabs(eta)<2.3)   Aeff=(cone==3? 0.120 : 0.210);
	  else if(fabs(eta)>2.3 && fabs(eta)<2.4)   Aeff=(cone==3? 0.120 : 0.220);
	  else Aeff=0.14;
	}
      else if(abs(id)==22){
	if(isoSum=="chIso"){
	  if(fabs(eta)<1.0)                         Aeff=0.012;
          else if(fabs(eta)>1.0 && fabs(eta)<1.479) Aeff=0.010;
          else if(fabs(eta)>1.479 && fabs(eta)<2.0) Aeff=0.014;
          else if(fabs(eta)>2.0 && fabs(eta)<2.2)   Aeff=0.012;
          else if(fabs(eta)>2.2 && fabs(eta)<2.3)   Aeff=0.016;
          else if(fabs(eta)>2.3 && fabs(eta)<2.4)   Aeff=0.020;
          else                                      Aeff=0.012;
	}
	if(isoSum=="nhIso"){
	  if(fabs(eta)<1.0)                         Aeff=0.030;
          else if(fabs(eta)>1.0 && fabs(eta)<1.479) Aeff=0.057;
          else if(fabs(eta)>1.479 && fabs(eta)<2.0) Aeff=0.039;
          else if(fabs(eta)>2.0 && fabs(eta)<2.2)   Aeff=0.015;
          else if(fabs(eta)>2.2 && fabs(eta)<2.3)   Aeff=0.024;
          else if(fabs(eta)>2.3 && fabs(eta)<2.4)   Aeff=0.039;
          else                                      Aeff=0.072;
	}
	if(isoSum=="gIso"){
	  if(fabs(eta)<1.0)                         Aeff=0.148;
          else if(fabs(eta)>1.0 && fabs(eta)<1.479) Aeff=0.130;
          else if(fabs(eta)>1.479 && fabs(eta)<2.0) Aeff=0.112;
          else if(fabs(eta)>2.0 && fabs(eta)<2.2)   Aeff=0.216;
          else if(fabs(eta)>2.2 && fabs(eta)<2.3)   Aeff=0.262;
          else if(fabs(eta)>2.3 && fabs(eta)<2.4)   Aeff=0.260;
          else                                      Aeff=0.266;
	}
      }
      return Aeff;
    }
  }
}
