#include "CMGTools/TauTest/interface/TauEfficiency.h"

void TauEfficiency::processEvents(Int_t nfiles){
    
  std::vector<string> files;
  int countfiles=0;
  int fileidx=1;
  while(countfiles<nfiles&&fileidx<1000){
    fileidx++;
    TString fname=filepath_+(long)fileidx+".root";
    
    //check if the file is good
    struct stat st;
    if(stat(fname.Data(),&st) != 0) continue;    
    TFile file(fname.Data(),"read");
    if(file.IsZombie()) continue;
    if(!file.GetListOfKeys()) continue;
    if(file.GetListOfKeys()->GetSize()==0) continue;

    files.push_back((string)(fname));
    countfiles++;
  }
  fwlite::ChainEvent ev(files);
  
  
  TH1F HPU("HPU","",50,-0.5,49.5); HPU.GetXaxis()->SetTitle("# of pile up interactions"); HPU.Sumw2();
  
  TH1F HptGen("HptGen","",100,0,100); HptGen.GetXaxis()->SetTitle("gen. #tau  jet  p_{T} (GeV) "); HptGen.Sumw2();
  TH1F Hpt("Hpt","",100,0,100); Hpt.GetXaxis()->SetTitle(" gen. #tau  jet  p_{T} (GeV) "); Hpt.Sumw2();
  
  TH1F HetaGen("HetaGen","",60,-3,3); HetaGen.GetXaxis()->SetTitle("gen. #tau  jet  #eta");  HetaGen.Sumw2();
  TH1F Heta("Heta","",60,-3,3); Heta.GetXaxis()->SetTitle(" #eta  ");  Heta.Sumw2();

  TH1F Hpu2030Gen("Hpu2030Gen","",50,-0.5,49.5); Hpu2030Gen.GetXaxis()->SetTitle("# of pile up interactions"); Hpu2030Gen.Sumw2();
  TH1F Hpu2030("Hpu2030","",50,-0.5,49.5); Hpu2030.GetXaxis()->SetTitle("# of pile up interactions"); Hpu2030.Sumw2();

  TH1F Hpu3040Gen("Hpu3040Gen","",50,-0.5,49.5); Hpu3040Gen.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040Gen.Sumw2();
  TH1F Hpu3040("Hpu3040","",50,-0.5,49.5); Hpu3040.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040.Sumw2();

  TH1F Hpu4050Gen("Hpu4050Gen","",50,-0.5,49.5); Hpu4050Gen.GetXaxis()->SetTitle("# of pile up interactions"); Hpu4050Gen.Sumw2();
  TH1F Hpu4050("Hpu4050","",50,-0.5,49.5); Hpu4050.GetXaxis()->SetTitle("# of pile up interactions"); Hpu4050.Sumw2();

  TH1F Hpu5060Gen("Hpu5060Gen","",50,-0.5,49.5); Hpu5060Gen.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060Gen.Sumw2();
  TH1F Hpu5060("Hpu5060","",50,-0.5,49.5); Hpu5060.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060.Sumw2();
  
  //for isolation efficiency
  TH1F HptVLooseIso("HptVLooseIso","",100,0,100); HptVLooseIso.GetXaxis()->SetTitle(" gen. #tau  jet  p_{T} (GeV) "); HptVLooseIso.Sumw2();
  TH1F HetaVLooseIso("HetaVLooseIso","",60,-3,3); HetaVLooseIso.GetXaxis()->SetTitle(" #eta  ");  HetaVLooseIso.Sumw2();
  TH1F Hpu2030VLooseIso("Hpu2030VLooseIso","",50,-0.5,49.5); Hpu2030VLooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu2030VLooseIso.Sumw2();
  TH1F Hpu3040VLooseIso("Hpu3040VLooseIso","",50,-0.5,49.5); Hpu3040VLooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040VLooseIso.Sumw2();
  TH1F Hpu4050VLooseIso("Hpu4050VLooseIso","",50,-0.5,49.5); Hpu4050VLooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu4050VLooseIso.Sumw2();
  TH1F Hpu5060VLooseIso("Hpu5060VLooseIso","",50,-0.5,49.5); Hpu5060VLooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060VLooseIso.Sumw2();

  TH1F HptLooseIso("HptLooseIso","",100,0,100); HptLooseIso.GetXaxis()->SetTitle(" gen. #tau  jet  p_{T} (GeV) "); HptLooseIso.Sumw2();
  TH1F HetaLooseIso("HetaLooseIso","",60,-3,3); HetaLooseIso.GetXaxis()->SetTitle(" #eta  ");  HetaLooseIso.Sumw2();
  TH1F Hpu2030LooseIso("Hpu2030LooseIso","",50,-0.5,49.5); Hpu2030LooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu2030LooseIso.Sumw2();
  TH1F Hpu3040LooseIso("Hpu3040LooseIso","",50,-0.5,49.5); Hpu3040LooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040LooseIso.Sumw2();
  TH1F Hpu4050LooseIso("Hpu4050LooseIso","",50,-0.5,49.5); Hpu4050LooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu4050LooseIso.Sumw2();
  TH1F Hpu5060LooseIso("Hpu5060LooseIso","",50,-0.5,49.5); Hpu5060LooseIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060LooseIso.Sumw2();

  TH1F HptMediumIso("HptMediumIso","",100,0,100); HptMediumIso.GetXaxis()->SetTitle(" gen. #tau  jet  p_{T} (GeV) "); HptMediumIso.Sumw2();
  TH1F HetaMediumIso("HetaMediumIso","",60,-3,3); HetaMediumIso.GetXaxis()->SetTitle(" #eta  ");  HetaMediumIso.Sumw2();
  TH1F Hpu2030MediumIso("Hpu2030MediumIso","",50,-0.5,49.5); Hpu2030MediumIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu2030MediumIso.Sumw2();
  TH1F Hpu3040MediumIso("Hpu3040MediumIso","",50,-0.5,49.5); Hpu3040MediumIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040MediumIso.Sumw2();
  TH1F Hpu4050MediumIso("Hpu4050MediumIso","",50,-0.5,49.5); Hpu4050MediumIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu4050MediumIso.Sumw2();
  TH1F Hpu5060MediumIso("Hpu5060MediumIso","",50,-0.5,49.5); Hpu5060MediumIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060MediumIso.Sumw2();

  TH1F HptTightIso("HptTightIso","",100,0,100); HptTightIso.GetXaxis()->SetTitle(" gen. #tau  jet  p_{T} (GeV) "); HptTightIso.Sumw2();
  TH1F HetaTightIso("HetaTightIso","",60,-3,3); HetaTightIso.GetXaxis()->SetTitle(" #eta  ");  HetaTightIso.Sumw2();
  TH1F Hpu2030TightIso("Hpu2030TightIso","",50,-0.5,49.5); Hpu2030TightIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu2030TightIso.Sumw2();
  TH1F Hpu3040TightIso("Hpu3040TightIso","",50,-0.5,49.5); Hpu3040TightIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu3040TightIso.Sumw2();
  TH1F Hpu4050TightIso("Hpu4050TightIso","",50,-0.5,49.5); Hpu4050TightIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu4050TightIso.Sumw2();
  TH1F Hpu5060TightIso("Hpu5060TightIso","",50,-0.5,49.5); Hpu5060TightIso.GetXaxis()->SetTitle("# of pile up interactions"); Hpu5060TightIso.Sumw2();


  //loop over events.
  int ZToTauTauEvents=0;
  Int_t ievt=0;
  for(ev.toBegin(); !ev.atEnd() ; ++ev, ++ievt){
    edm::EventBase const & event = ev;
    if(ievt%100==1)cout<<"event "<<ievt<<endl;

    //get the genparticles 
    event.getByLabel(edm::InputTag("genParticlesStatus3"),genParticles_);  
    event.getByLabel(edm::InputTag("genJetSel"),genJets_);  
    event.getByLabel(edm::InputTag("genLeptonsStatus1"),genLeptons1_);  

    //determine the gen Boson
    bosonType_=0;
    genJet_[0]=0;
    genJet_[1]=0;


    if(effOrFake_==1){
      findGenTaus();
      if(!bosonType_==5)  continue; //Use Z--> TauTau for efficiency
      ZToTauTauEvents++;
    } else if(effOrFake_==2) findGenJets(); //Use jets after removing any leptons 
    else return;
    
    //remove gen jets out of the eta acceptance
    if(genJet_[0])if(fabs(genJet_[0]->eta())>2.3)genJet_[0]=0;
    if(genJet_[1])if(fabs(genJet_[1]->eta())>2.3)genJet_[1]=0;


    /////////////////get the number of pile up vertexes
    edm::Handle< std::vector< PileupSummaryInfo > >  PupInfo;
    event.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;  
    int npv=-1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
        npv = PVI->getPU_NumInteractions();
        continue;
      }    
    }
    HPU.Fill(npv);

    edm::Handle< std::vector<cmg::Tau> > candvec;
    event.getByLabel(edm::InputTag("cmgTauSel"),candvec);
    
    ///kinematics of the generated taus
    for(int i=0;i<2;i++){
      const cmg::PhysicsObjectWithPtr<edm::Ptr<reco::GenJet> > * genTauJet=genJet_[i];
      if(genTauJet){
	HptGen.Fill(genTauJet->pt());
	HetaGen.Fill(genTauJet->eta());
	if(20.<genTauJet->pt()&&genTauJet->pt()<30.) Hpu2030Gen.Fill(npv);
	if(30.<genTauJet->pt()&&genTauJet->pt()<40.) Hpu3040Gen.Fill(npv);
	if(40.<genTauJet->pt()&&genTauJet->pt()<50.) Hpu4050Gen.Fill(npv);
	if(50.<genTauJet->pt()&&genTauJet->pt()<60.) Hpu5060Gen.Fill(npv);

	Int_t icand=0;
	for(std::vector<cmg::Tau>::const_iterator recoTau=candvec->begin(); recoTau!=candvec->end(); ++recoTau, icand++){
	  if(reco::deltaR(recoTau->eta(),recoTau->phi(),genTauJet->eta(),genTauJet->phi())>0.3) continue;
      
	  Hpt.Fill(genTauJet->pt());
	  Heta.Fill(genTauJet->eta());
	  if(20<genTauJet->pt()&&genTauJet->pt()<30) Hpu2030.Fill(npv);
	  if(30<genTauJet->pt()&&genTauJet->pt()<40) Hpu3040.Fill(npv);
	  if(40<genTauJet->pt()&&genTauJet->pt()<50) Hpu4050.Fill(npv);
	  if(50<genTauJet->pt()&&genTauJet->pt()<60) Hpu5060.Fill(npv);
						
	  //with isolation
	  if(recoTau->tauID("byVLooseCombinedIsolationDeltaBetaCorr") < 0.5) continue;
	  HptVLooseIso.Fill(genTauJet->pt());
	  HetaVLooseIso.Fill(genTauJet->eta());
	  if(20<genTauJet->pt()&&genTauJet->pt()<30) Hpu2030VLooseIso.Fill(npv);
	  if(30<genTauJet->pt()&&genTauJet->pt()<40) Hpu3040VLooseIso.Fill(npv);
	  if(40<genTauJet->pt()&&genTauJet->pt()<50) Hpu4050VLooseIso.Fill(npv);
	  if(50<genTauJet->pt()&&genTauJet->pt()<60) Hpu5060VLooseIso.Fill(npv);

	  if(recoTau->tauID("byLooseCombinedIsolationDeltaBetaCorr") < 0.5) continue;
	  HptLooseIso.Fill(genTauJet->pt());
	  HetaLooseIso.Fill(genTauJet->eta());
	  if(20<genTauJet->pt()&&genTauJet->pt()<30) Hpu2030LooseIso.Fill(npv);
	  if(30<genTauJet->pt()&&genTauJet->pt()<40) Hpu3040LooseIso.Fill(npv);
	  if(40<genTauJet->pt()&&genTauJet->pt()<50) Hpu4050LooseIso.Fill(npv);
	  if(50<genTauJet->pt()&&genTauJet->pt()<60) Hpu5060LooseIso.Fill(npv);

	  if(recoTau->tauID("byMediumCombinedIsolationDeltaBetaCorr") < 0.5) continue;
	  HptMediumIso.Fill(genTauJet->pt());
	  HetaMediumIso.Fill(genTauJet->eta());
	  if(20<genTauJet->pt()&&genTauJet->pt()<30) Hpu2030MediumIso.Fill(npv);
	  if(30<genTauJet->pt()&&genTauJet->pt()<40) Hpu3040MediumIso.Fill(npv);
	  if(40<genTauJet->pt()&&genTauJet->pt()<50) Hpu4050MediumIso.Fill(npv);
	  if(50<genTauJet->pt()&&genTauJet->pt()<60) Hpu5060MediumIso.Fill(npv);

	  if(recoTau->tauID("byTightCombinedIsolationDeltaBetaCorr") < 0.5) continue;
	  HptTightIso.Fill(genTauJet->pt());
	  HetaTightIso.Fill(genTauJet->eta());
	  if(20<genTauJet->pt()&&genTauJet->pt()<30) Hpu2030TightIso.Fill(npv);
	  if(30<genTauJet->pt()&&genTauJet->pt()<40) Hpu3040TightIso.Fill(npv);
	  if(40<genTauJet->pt()&&genTauJet->pt()<50) Hpu4050TightIso.Fill(npv);
	  if(50<genTauJet->pt()&&genTauJet->pt()<60) Hpu5060TightIso.Fill(npv);

	}
      }
    }

  }

  cout<<" ZToTauTauEvents = "<<ZToTauTauEvents<<endl;
 

  TFile RootFile("TauEfficiency.root","recreate");
  HPU.Write();
  HptGen.Write();
  Hpt.Write();
  HetaGen.Write();
  Heta.Write();
  Hpu2030Gen.Write();
  Hpu2030.Write();
  Hpu3040Gen.Write();
  Hpu3040.Write();
  Hpu4050Gen.Write();
  Hpu4050.Write();
  Hpu5060Gen.Write();
  Hpu5060.Write();
  
  HptVLooseIso.Write();
  HetaVLooseIso.Write();
  Hpu2030VLooseIso.Write();
  Hpu3040VLooseIso.Write();
  Hpu4050VLooseIso.Write();
  Hpu5060VLooseIso.Write();

  HptLooseIso.Write();
  HetaLooseIso.Write();
  Hpu2030LooseIso.Write();
  Hpu3040LooseIso.Write();
  Hpu4050LooseIso.Write();
  Hpu5060LooseIso.Write();

  HptMediumIso.Write();
  HetaMediumIso.Write();
  Hpu2030MediumIso.Write();
  Hpu3040MediumIso.Write();
  Hpu4050MediumIso.Write();
  Hpu5060MediumIso.Write();

  HptTightIso.Write();
  HetaTightIso.Write();
  Hpu2030TightIso.Write();
  Hpu3040TightIso.Write();
  Hpu4050TightIso.Write();
  Hpu5060TightIso.Write();


  RootFile.ls();
  RootFile.Close();

}


void TauEfficiency::findGenTaus(){


  genBosonLeg_[0]=0;
  genBosonLeg_[1]=0;
  
  //determine type of generated event
  int genTaus=0;
  int genMuons=0;
  int genElectrons=0;
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if(abs(g->pdgId())==11 ) genElectrons++;
    if(abs(g->pdgId())==13 ) genMuons++;
    if(abs(g->pdgId())==15 ) genTaus++;
  }
  if(genElectrons==2)   bosonType_=1; //Z-->ee,mumu,tautau,
  if(genMuons==2)       bosonType_=3;
  if(genTaus==2)        bosonType_=5;
  if(genElectrons==1)   bosonType_=11;// W-->e nu, mu nu, tau mu
  if(genMuons==1)       bosonType_=13;
  if(genTaus==1)        bosonType_=15;
 
  //get the leptons from the genBoson
  for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
    if((g->pdgId()==11 || g->pdgId()==13 || g->pdgId()==15 ) ) genBosonLeg_[0]=&(*g);
    if((g->pdgId()==-11 || g->pdgId()==-13 || g->pdgId()==-15 ) ) genBosonLeg_[1]=&(*g);
  }      

  //find the gen jets corresponding to the taus    
  for(std::vector<cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > >::const_iterator g=genJets_->begin(); g!=genJets_->end(); ++g){    
    if(genBosonLeg_[0])if(reco::deltaR(genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi(),g->eta(),g->phi())<0.3) genJet_[0]=&(*g);
    if(genBosonLeg_[1])if(reco::deltaR(genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi(),g->eta(),g->phi())<0.3) genJet_[1]=&(*g);
  } 

  //dont use taus decaying leptonically
  for(std::vector<reco::GenParticle>::const_iterator g=genLeptons1_->begin(); g!=genLeptons1_->end(); ++g){    
    if(genBosonLeg_[0])
      if(reco::deltaR(genBosonLeg_[0]->eta(),genBosonLeg_[0]->phi(),g->eta(),g->phi())<0.3
	 && ( abs(g->pdgId())==11  || abs(g->pdgId())==13  )){
	genJet_[0]=0;
      }
    if(genBosonLeg_[1])
      if(reco::deltaR(genBosonLeg_[1]->eta(),genBosonLeg_[1]->phi(),g->eta(),g->phi())<0.3
	 && ( abs(g->pdgId())==11  || abs(g->pdgId())==13  ) ){
	genJet_[1]=0;
      }
  }
  
}


void TauEfficiency::findGenJets(){
  //find the leading and subleading gen jets //Note these jets already have a pt cut >15

  //first remove the leptons from the jets
  std::vector<const cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > * > cleanGenJets;
  for(std::vector<cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > >::const_iterator g=genJets_->begin(); g!=genJets_->end(); ++g){    
    bool islepton=0;
    for(std::vector<reco::GenParticle>::const_iterator l=genLeptons1_->begin(); l!=genLeptons1_->end(); ++l)    
      if( abs(l->pdgId())==11  || abs(l->pdgId())==13 || abs(l->pdgId())==15 )
	if(reco::deltaR(g->eta(),g->phi(),l->eta(),l->phi())<0.3) islepton=1;
    for(std::vector<reco::GenParticle>::const_iterator l=genParticles_->begin(); l!=genParticles_->end(); ++l)
      if( abs(l->pdgId())==11  || abs(l->pdgId())==13 || abs(l->pdgId())==15 )
	if(reco::deltaR(g->eta(),g->phi(),l->eta(),l->phi())<0.3) islepton=1;
    
    if(islepton)continue;
    cleanGenJets.push_back(&(*g));
  } 
  
  if(cleanGenJets.size()>0){
    genJet_[0]=cleanGenJets[0];
    for(std::vector<const cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > *>::const_iterator g=cleanGenJets.begin(); g!=cleanGenJets.end(); ++g)
      if((*g)->pt()>genJet_[0]->pt())genJet_[0]=*g;
  }
  if(cleanGenJets.size()>1){
    genJet_[1]=cleanGenJets[0];
    for(std::vector<const cmg::PhysicsObjectWithPtr<edm::Ptr< reco::GenJet > > *>::const_iterator g=cleanGenJets.begin(); g!=cleanGenJets.end(); ++g)
      if((*g)->pt()>genJet_[1]->pt()&&(*g)->pt()<genJet_[0]->pt())genJet_[1]=*g;
  }

}


 
//     for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
//       if((abs(g->pdgId())==23 || abs(g->pdgId())==24) && genBoson_==NULL )
//         genBoson_=&(*g);
//     }

//     if(genBoson_){      
//       //determine type of generated event
//       int genTaus=0;
//       int genMuons=0;
//       int genElectrons=0;
//       for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
//         if(abs(g->pdgId())==11 && g->mother()==genBoson_) genElectrons++;
//         if(abs(g->pdgId())==13 && g->mother()==genBoson_) genMuons++;
//         if(abs(g->pdgId())==15 && g->mother()==genBoson_) genTaus++;
//       }
//       if(abs(genBoson_->pdgId())==23 && genElectrons==2)   bosonType_=1; //Z-->ee,mumu,tautau,
//       if(abs(genBoson_->pdgId())==23 && genMuons==2)       bosonType_=3;
//       if(abs(genBoson_->pdgId())==23 && genTaus==2)        bosonType_=5;
//       if(abs(genBoson_->pdgId())==24 && genElectrons==1)   bosonType_=11;// W-->e nu, mu nu, tau mu
//       if(abs(genBoson_->pdgId())==24 && genMuons==1)       bosonType_=13;
//       if(abs(genBoson_->pdgId())==24 && genTaus==1)        bosonType_=15;

//       //get the leptons from the genBoson
//       for(std::vector<reco::GenParticle>::const_iterator g=genParticles_->begin(); g!=genParticles_->end(); ++g){    
//         if((g->pdgId()==11 || g->pdgId()==13 || g->pdgId()==15 ) && g->mother()==genBoson_) genBosonL1_=&(*g);
//         if((g->pdgId()==-11 || g->pdgId()==-13 || g->pdgId()==-15 ) && g->mother()==genBoson_) genBosonL2_=&(*g);
//       }      

//     }
  
