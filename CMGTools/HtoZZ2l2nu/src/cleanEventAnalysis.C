#include "CMGTools/HtoZZ2l2nu/interface/cleanEventAnalysis.h"

using namespace std;

//
CleanEventAnalysis::CleanEventAnalysis(edm::ParameterSet const& params, TFileDirectory& fs)
  : edm::BasicAnalyzer::BasicAnalyzer(params, fs)
{
  TString streams[]={"ee","mumu","emu"};
  TString jetmult[]={"eq0jets","eq1jets","geq2jets"};
  for(size_t istream=0; istream<sizeof(streams)/sizeof(TString); istream++)
    {
      TString cat=streams[istream];
      
      //dilepton control
      results_[cat+"_dilepton_mass"]     = fs.make<TH1F>(cat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.);
      results_[cat+"_dilepton_sumpt"]    = fs.make<TH1F>(cat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.);
      results_[cat+"_dilepton_pt"]       = fs.make<TH1F>(cat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.);
      
      //jet multiplicity
      results_[cat+"_jetpt"]             = fs.make<TH1F>(cat+"_jetpt",";p_{T} [GeV/c]; Jets",100,0,200);
      results_[cat+"_njets"]             = fs.make<TH1F>(cat+"_njets",";Jet multiplicity; Events",4,0,4);
      results_[cat+"_bmult"]             = fs.make<TH1F>(cat+"_bmult",";b tag multiplicity (TCHEL); Events",4,0,4);
      for(int ibin=1; ibin<=((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins(); ibin++)
	{
	  TString ilabel(""); ilabel+=(ibin-1);
	  if(ibin==((TH1F *)results_[cat+"_njets"])->GetXaxis()->GetNbins()) ilabel="#geq"+ilabel;
	  ((TH1F *)results_[cat+"_njets"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	  ((TH1F *)results_[cat+"_bmult"])->GetXaxis()->SetBinLabel(ibin,ilabel);
	}
      results_[cat+"_btags"]             = fs.make<TH1F>(cat+"_btags",";b tags (TCHE); Jets",100,-0.5,2);
      results_[cat+"_met"]               = fs.make<TH1F>(cat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 30,  0.,300.);
      results_[cat+"_metsig"]            = fs.make<TH1F>(cat+"_metsig", ";#slash{E}_{T} significance; Events", 100,  0.,100.);
      
      //split the analysis according to the jet multiplicity
      for(size_t jstream=0; jstream<sizeof(jetmult)/sizeof(TString); jstream++)
	{
	  TString subcat=cat+jetmult[jstream];
	  results_[subcat+"_dilepton_mass"]     = fs.make<TH1F>(subcat+"_dilepton_mass", ";Invariant Mass(l,l') [GeV/c^{2}]; Events", 100, 0.,300.);
	  results_[subcat+"_dilepton_sumpt"]    = fs.make<TH1F>(subcat+"_dilepton_sumpt", ";#Sigma |#vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.);
	  results_[subcat+"_dilepton_pt"]       = fs.make<TH1F>(subcat+"_dilepton_pt", ";|#Sigma #vec{p}_{T}| [GeV/c]; Events", 100, 0.,300.);
	  results_["met2"+subcat+"_mindphi"]    = fs.make<TH1F>("met2"+subcat+"_mindphi", ";min #Delta #phi(#slash{E}_{T},l) [rad]; Events", 100, 0,3.2);
	  results_[subcat+"_met"]               = fs.make<TH1F>(subcat+"_met", ";#slash{E}_{T} [GeV/c]; Events", 100,  0.,300.);
	  results_[subcat+"_metsig"]            = fs.make<TH1F>(subcat+"_metsig", ";#slash{E}_{T} significance; Events", 100,  0.,100.);
	  results_[subcat+"_dilepton2met_dphi"] = fs.make<TH1F>(subcat+"dilepton2met_dphi", ";#Delta #phi [rad]; Events", 100, 0,3.2);
	  results_[subcat+"_relmet"]            = fs.make<TH1F>(subcat+"_relmet", ";#slash{E}_{T,rel} [GeV/c]; Events", 100,  0.,300.);
	  results_[subcat+"_mT"]                = fs.make<TH1F>(subcat+"_mT",";Transverse mass(dilepton,MET) [GeV/c^{2}]; Events",100,0,1000);
	  results_[subcat+"_mT_individual"]     = fs.make<TH1F>(subcat+"_mT_individual",";Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500);
	  results_[subcat+"_mT_corr"]           = new TH2F(subcat+"_mT_corr",";Transverse mass(leading lepton,MET) [GeV/c^{2}];Transverse mass(trailer lepton,MET) [GeV/c^{2}]; Events",50,0,500,50,0,500);
	  results_[subcat+"_mT_individualsum"]  = fs.make<TH1F>(subcat+"_mT_individualsum",";#Sigma Transverse mass(lepton,MET) [GeV/c^{2}]; Events",100,0,500);
	}
    }
}

//
void CleanEventAnalysis::analyze(const edm::EventBase& ev)
{

  try{
    //get objects for this event
    edm::Handle<std::vector<pat::EventHypothesis> > evHandle;
    ev.getByLabel(edm::InputTag("cleanEvent:selectedEvent"),evHandle);
    if(!evHandle.isValid()) return;
    const pat::EventHypothesis &evhyp = (*(evHandle.product()))[0];
    
    edm::Handle<std::vector<reco::Vertex> > vertexHandle;
    ev.getByLabel(edm::InputTag("cleanEvent:selectedVertices"),vertexHandle);
    if(!vertexHandle.isValid()) return;
    
    edm::Handle< std::vector<int> > selInfo;
    ev.getByLabel(edm::InputTag("cleanEvent:selectionInfo"),selInfo);
    if(!selInfo.isValid()) return;
    
    int selPath = (*(selInfo.product()))[0];
    int selStep = (*(selInfo.product()))[1];
    
    //require that a dilepton has benn selected
    if(selPath==0 or selStep<3) return;
    TString istream="mumu";
    if(selPath==2) istream="ee";
    if(selPath==3) istream="emu";
    
    //count the jets in the event
    std::vector<reco::CandidatePtr> seljets= evhyp.all("jet");
    int njets( seljets.size() ), nbjets(0);
    for (pat::eventhypothesis::Looper<pat::Jet> jet = evhyp.loopAs<pat::Jet>("jet"); jet; ++jet) {
      float btag=jet->bDiscriminator("trackCountingHighEffBJetTags");
      if(btag>1.74) nbjets+=1; //loose point
      ((TH1F *)results_[istream+"_btags"])->Fill(btag);
      ((TH1F *)results_[istream+"_jetpt"])->Fill(jet->pt());
    }
    ((TH1F *)results_[istream+"_njets"])->Fill(njets);
    ((TH1F *)results_[istream+"_bmult"])->Fill(nbjets);
    
    int jetbin=njets;
    if(jetbin>2)jetbin=2;
    
    //update the selection stream
    TString substream=istream; 
    if(njets==0)      substream = substream+"eq0jets";
    else if(njets==1) substream = substream+"eq1jets";
    else              substream = substream+"geq2jets";
    
    //basic dilepton kinematics
    reco::CandidatePtr lepton1 = evhyp["leg1"];
    TLorentzVector lepton1P(lepton1->px(),lepton1->py(),lepton1->pz(), lepton1->energy());
    reco::CandidatePtr lepton2 = evhyp["leg2"];
    TLorentzVector lepton2P(lepton2->px(),lepton2->py(),lepton2->pz(), lepton2->energy());
    TLorentzVector dileptonP=lepton1P+lepton2P;
    ((TH1F *)results_[istream+"_dilepton_sumpt"])->Fill(lepton1P.Pt()+lepton2P.Pt());
    ((TH1F *)results_[istream+"_dilepton_pt"])->Fill(dileptonP.Pt());
    ((TH1F *)results_[istream+"_dilepton_mass"])->Fill(dileptonP.M());
    
    //base met kinematics
    const pat::MET *themet=evhyp.getAs<pat::MET>("met");
    TLorentzVector metP(themet->px(),themet->py(),0,themet->pt());
    float metsig(-1);
    try{
      const reco::MET *origMet = dynamic_cast<const reco::MET *>(themet->originalObject());
      metsig=origMet->significance();
    }catch(std::exception &e){
    }
    ((TH1F *)results_[istream+"_met"])->Fill(metP.Pt());
    ((TH1F *)results_[istream+"_metsig"])->Fill(metsig);
    
    //require MET back-to-back to dilepton
    float dphiZ2met=dileptonP.DeltaPhi(metP);
    if(fabs(dphiZ2met)<TMath::Pi()/2) return;
    ((TH1F *)results_[substream+"_met"])->Fill(metP.Pt());
    ((TH1F *)results_[substream+"_dilepton2met_dphi"])->Fill(abs(dphiZ2met));
    ((TH1F *)results_[substream+"_metsig"])->Fill(metsig);
    
    //individual lepton vs MET kinematics
    float dphil2met[]={ fabs(metP.DeltaPhi(lepton1P)), fabs(metP.DeltaPhi(lepton2P)) };
    float mTlmet[]={ TMath::Sqrt(2*metP.Pt()*lepton1P.Pt()*(1-TMath::Cos(dphil2met[0]))) ,   TMath::Sqrt(2*metP.Pt()*lepton2P.Pt()*(1-TMath::Cos(dphil2met[1]))) };
    float dphimin=TMath::MinElement(sizeof(dphil2met)/sizeof(float),dphil2met);
    float relMET=metP.Pt();
    if(fabs(dphimin)<TMath::Pi()/2) relMET = relMET*TMath::Sin(dphimin);
    
    //dilepton vs MET kinematics
    TLorentzVector transvSum=dileptonP + metP;
    float transverseMass=TMath::Power(TMath::Sqrt(TMath::Power(dileptonP.Pt(),2)+pow(dileptonP.M(),2))+TMath::Sqrt(TMath::Power(metP.Pt(),2)+pow(dileptonP.M(),2)),2);
    transverseMass-=TMath::Power(transvSum.Pt(),2);
    transverseMass=TMath::Sqrt(transverseMass);
    
    //control histograms
    ((TH1F *)results_["met2"+substream+"_mindphi"])->Fill(dphimin);
    ((TH1F *)results_[substream+"_relmet"])->Fill(relMET);
    ((TH1F *)results_[substream+"_mT_individual"])->Fill(mTlmet[0]);
    ((TH1F *)results_[substream+"_mT_individual"])->Fill(mTlmet[1]);
    if(lepton1P.Pt()>lepton2P.Pt()) ((TH1F *)results_[substream+"_mT_corr"])->Fill(mTlmet[0],mTlmet[1]);
    else ((TH1F *)results_[substream+"_mT_corr"])->Fill(mTlmet[1],mTlmet[0]);
    ((TH1F *)results_[substream+"_mT_individualsum"])->Fill(mTlmet[0]+mTlmet[1]);
    ((TH1F *)results_[substream+"_dilepton_mass"])->Fill(dileptonP.M());
    ((TH1F *)results_[substream+"_dilepton_sumpt"])->Fill(lepton1P.Pt()+lepton2P.Pt());
    ((TH1F *)results_[substream+"_dilepton_pt"])->Fill(dileptonP.Pt());
    ((TH1F *)results_[substream+"_mT"])->Fill(transverseMass);
    
  }catch(std::exception &e){
  }
}


//
void CleanEventAnalysis::endJob()
{
  //add overflow+underlow
  for(std::map<TString, TObject *>::iterator rIt = results_.begin();
      rIt != results_.end();
      rIt++)
    fixExtremities((TH1F *)rIt->second,true,true);
}
