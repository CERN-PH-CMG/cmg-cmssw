#include "UserCode/EWKV/interface/UEAnalysis.h"

using namespace std;

//
UEAnalysis::UEAnalysis(SmartSelectionMonitor &mon)
  : mon_(&mon)
{
  TString recType[]={"raw","t1","t12"};
  for(size_t i=0; i<3; i++){
    mon_->addHistogram( new TH2F("thrustresponse_"+recType[i],";#phi(#vec{H}_{T}+#vec{S}_{T}) [rad]; #phi(t#bar{t}) [rad];Events",50,-3.4,3.4,50,-3.4,3.4) );
    mon_->addHistogram( new TProfile("thrustprofile_"+recType[i],";|#vec{H}_{T}+#vec{S}_{T}| [GeV];#Delta#phi(#vec{H}_{T}+#vec{S}_{T},t#bar{t}) [rad];Events",25,0.,250.) );
  }
  
  mon_->addHistogram( new TH1F("ptttbar",";t#bar{t} transverse momentum [GeV];Events",25,0,250));
  mon_->addHistogram( new TH1F("mtttbar",";t#bar{t} transverse mass [GeV];Events",25,0,1000));

  ueReg_.push_back("");
  ueReg_.push_back("away");
  ueReg_.push_back("toward");
  ueReg_.push_back("transverse");
  for(size_t ireg=0; ireg<ueReg_.size(); ireg++)
    {
      mon_->addHistogram( new TH1F("nch"+ueReg_[ireg],";Charged particles;Events",30,0,30));
      mon_->addHistogram( new TH1F("ptflux"+ueReg_[ireg],";Charged p_{T} flux [GeV];Events",25,0,50));
      mon_->addHistogram( new TH1F("avgptflux"+ueReg_[ireg],";Average p_{T} flux [GeV];Events",25,0,5));

      mon_->addHistogram( new TH2F("nchprofpt"+ueReg_[ireg],";t#bar{t} transverse momentum [GeV];Charged particles;Events",25,0,250, 30,0,30));
      mon_->addHistogram( new TH2F("ptfluxprofpt"+ueReg_[ireg],";t#bar{t} transverse momentum [GeV];Charged p_{T} flux [GeV];Events",25,0,250,25,0,50));
      mon_->addHistogram( new TH2F("avgptfluxprofpt"+ueReg_[ireg],";t#bar{t} transverse momentum [GeV];Average p_{T} flux [GeV];Events",25,0,250,25,0,5));

      mon_->addHistogram( new TH2F("nchprofmt"+ueReg_[ireg],";t#bar{t} transverse mass [GeV];Charged particles;Events",25,0,1000,30,0,30));
      mon_->addHistogram( new TH2F("ptfluxprofmt"+ueReg_[ireg],";t#bar{t} transverse mass [GeV];Charged p_{T} flux [GeV];Events",25,0,1000,25,0,50));
      mon_->addHistogram( new TH2F("avgptfluxprofmt"+ueReg_[ireg],";t#bar{t} transverse mass [GeV];Average p_{T} flux [GeV];Events",25,0,1000,25,0,5));

      if(ireg==0)
	mon_->addHistogram( new TH2F("ptfluxprofphi",";#Delta#phi[^{0}];Charged p_{T} flux [GeV];Events",25,0,180,25,0,50));
    }
}

//
void UEAnalysis::analyze(data::PhysicsObjectCollection_t &leptons, 
			 data::PhysicsObjectCollection_t &jets,
			 data::PhysicsObjectCollection_t &met, 
			 data::PhysicsObjectCollection_t &pf,
			 data::PhysicsObjectCollection_t &gen,
			 float weight)
{
  float minPFpt(0.5), maxPFeta(2.1); 

  //check the category
  int lid1(leptons[0].get("id")), lid2(leptons[1].get("id"));
  std::vector<TString> ch;
  if     (abs(lid1)*abs(lid2)==11*11 || abs(lid1)*abs(lid2)==13*13) ch.push_back("ll");
  else if(abs(lid1)*abs(lid2)==11*13) ch.push_back("emu");
  else return;

  //control the resolution
  LorentzVector top,antitop;
  for(size_t igen=0; igen<gen.size(); igen++)
    {
      if(gen[igen].get("id")==6)  top=gen[igen];
      if(gen[igen].get("id")==-6) antitop=gen[igen];
    }
  LorentzVector htlep=leptons[0]+leptons[1]+jets[0]+jets[1];
  LorentzVector gen_ttbar=top+antitop;
  if(gen_ttbar.pt()>0)
    {
      mon_->fillHisto("thrustresponse_raw",  ch, (htlep+met[0]).phi(), gen_ttbar.phi(), weight);
      mon_->fillHisto("thrustresponse_t1",   ch, (htlep+met[2]).phi(), gen_ttbar.phi(), weight);
      mon_->fillHisto("thrustresponse_t12",  ch, (htlep+met[3]).phi(), gen_ttbar.phi(), weight);
      mon_->fillProfile("thrustprofile_raw", ch, (htlep+met[0]).pt(), deltaPhi((htlep+met[0]).phi(), gen_ttbar.phi()), weight);
      mon_->fillProfile("thrustprofile_t1",  ch, (htlep+met[2]).pt(), deltaPhi((htlep+met[2]).phi(), gen_ttbar.phi()), weight);
      mon_->fillProfile("thrustprofile_t12", ch, (htlep+met[3]).pt(), deltaPhi((htlep+met[3]).phi(), gen_ttbar.phi()), weight);
    }

  //using raw met as the estimator
  LorentzVector rec_ttbar=htlep+met[0];
  
  //study UE with charged PF
  std::vector<int> chCount(4,0);
  std::vector<float> chFlux(4,0);
  for(size_t ipfn=0; ipfn<pf.size(); ipfn++)
    {
      if(pf[ipfn].get("charge")==0) continue;
      
      //remove if it belongs to a tag jet
      bool belongsToTagJet(false);
      for(size_t ijet=0; ijet<2; ijet++)
	{
	  size_t pfstart=jets[ijet].get("pfstart");
	  size_t pfend=jets[ijet].get("pfend");
	  if(ipfn>=pfstart && ipfn<=pfend) belongsToTagJet=true;
	}
      if(belongsToTagJet) continue;
      
      //remove if matching a selected lepton
      double minDRpfl(9999.);
      for(size_t ilep=0; ilep<2; ilep++)
	minDRpfl = TMath::Min( minDRpfl, deltaR(pf[ipfn],leptons[ilep]) );
      if(minDRpfl<0.05) continue;
      
      
      if(pf[ipfn].pt()<minPFpt || fabs(pf[ipfn].eta())>maxPFeta) continue;
      
      float dphi=deltaPhi(pf[ipfn].phi(),rec_ttbar.phi())*180/TMath::Pi();
      size_t regIdx=3;
      if(dphi>120) regIdx=1;
      if(dphi<60)  regIdx=2;
      chCount[0]++;                  chCount[regIdx]++;
      chFlux[0] += pf[ipfn].pt();    chFlux[regIdx] += pf[ipfn].pt();
    
      mon_->fillHisto("ptfluxprofphi",    ch, dphi, pf[ipfn].pt(),  weight);
    }
  mon_->fillHisto("ptttbar",  ch, rec_ttbar.pt(), weight);
  mon_->fillHisto("mtttbar",  ch, rec_ttbar.Mt(), weight);
  for(size_t ireg=0; ireg<4; ireg++)
    {
      float cts(chCount[ireg]);
      float flux(chFlux[ireg]);
      float normFlux(cts>0?flux/cts:0);
      
      mon_->fillHisto("nch"+ueReg_[ireg],             ch, cts,                      weight);
      mon_->fillHisto("ptflux"+ueReg_[ireg],          ch, flux ,                    weight);
      mon_->fillHisto("avgptflux"+ueReg_[ireg],       ch, normFlux,                 weight);

      mon_->fillHisto("nchprofpt"+ueReg_[ireg],       ch, rec_ttbar.pt(), cts,      weight);
      mon_->fillHisto("ptfluxprofpt"+ueReg_[ireg],    ch, rec_ttbar.pt(), flux,     weight);
      mon_->fillHisto("avgptfluxprofpt"+ueReg_[ireg], ch, rec_ttbar.pt(), normFlux, weight);
      mon_->fillHisto("nchprofmt"+ueReg_[ireg],       ch, rec_ttbar.Mt(), cts,      weight);
      mon_->fillHisto("ptfluxprofmt"+ueReg_[ireg],    ch, rec_ttbar.Mt(), flux,     weight);
      mon_->fillHisto("avgptfluxprofmt"+ueReg_[ireg], ch, rec_ttbar.Mt(), normFlux, weight);
    }
}
