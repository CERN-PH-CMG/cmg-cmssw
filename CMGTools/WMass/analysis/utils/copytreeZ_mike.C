void copytreeZ_mike() {

  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/";
  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_10_15/";
  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/";
  TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_08_19_53X_8TeV/";
  
  TString fZana_str[7] = {
    "root://eoscms//"+root_folder+"WJetsLL/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DYJetsMM/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"TTJets/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/ZZ/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WW/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WZ/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DATA/ZTreeProducer_tree.root"
  };  
  TString fZana_RecoSkimmed_str[7] = {
    root_folder+"WJetsLL/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DYJetsMM/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"TTJets/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/ZZ/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WW/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WZ/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DATA/ZTreeProducer_tree_RecoSkimmed.root"
  };  

  // int sample = 0;
  for(int sample=0; sample<7; sample++){
    
    if(
       !fZana_RecoSkimmed_str[sample].Contains("DYJetsMM")
       // && !fZana_RecoSkimmed_str[sample].Contains("DATA")
       ) continue;
    
    TFile *oldfile = TFile::Open(Form("%s",fZana_str[sample].Data()));
    TTree *oldtree = (TTree*)oldfile->Get("ZTreeProducer");

    if(sample>2){
      oldtree->SetBranchStatus("ZGen_pt",0);
      oldtree->SetBranchStatus("ZGen_rap",0);
      oldtree->SetBranchStatus("ZGen_phi",0);
      oldtree->SetBranchStatus("ZGen_mass",0);
      oldtree->SetBranchStatus("ZGen_mt",0);
      oldtree->SetBranchStatus("MuPosGen_pt",0);
      oldtree->SetBranchStatus("MuPosGen_eta",0);
      oldtree->SetBranchStatus("MuPosGen_phi",0);
      oldtree->SetBranchStatus("MuPosGen_mass",0);
      oldtree->SetBranchStatus("MuPosGen_charge",0);
      oldtree->SetBranchStatus("MuPosDRGenP",0);
      oldtree->SetBranchStatus("MuNegGen_pt",0);
      oldtree->SetBranchStatus("MuNegGen_eta",0);
      oldtree->SetBranchStatus("MuNegGen_phi",0);
      oldtree->SetBranchStatus("MuNegGen_mass",0);
      oldtree->SetBranchStatus("MuNegGen_charge",0);
      oldtree->SetBranchStatus("MuNegDRGenP",0);
    }
      oldtree->SetBranchStatus("cmgjets_number",0);
      oldtree->SetBranchStatus("cmgjets_pt",0);
      oldtree->SetBranchStatus("cmgjets_eta",0);
      oldtree->SetBranchStatus("cmgjets_phi",0);
      oldtree->SetBranchStatus("cmgmuons_number",0);
      oldtree->SetBranchStatus("cmgmuons_charge",0);
      oldtree->SetBranchStatus("cmgmuons_ID",0);
      oldtree->SetBranchStatus("cmgmuons_Iso",0);
      oldtree->SetBranchStatus("cmgmuons_IsPromt",0);
      oldtree->SetBranchStatus("cmgmuons_IsTrig",0);
      oldtree->SetBranchStatus("cmgmuons_pt",0);
      oldtree->SetBranchStatus("cmgmuons_eta",0);
      oldtree->SetBranchStatus("cmgmuons_phi",0);
      oldtree->SetBranchStatus("cmgelectrons_number",0);
      oldtree->SetBranchStatus("cmgelectrons_pt",0);
      oldtree->SetBranchStatus("cmgelectrons_eta",0);
      oldtree->SetBranchStatus("cmgelectrons_phi",0);
      oldtree->SetBranchStatus("cmgelectrons_TightID",0);
      oldtree->SetBranchStatus("cmgelectrons_TightIso",0);
      oldtree->SetBranchStatus("cmgelectrons_charge",0);
      oldtree->SetBranchStatus("cmgelectrons_IsPromt",0);

    TFile *newfileSig,*newfileFake;
    TTree *newtreeSig,*newtreeFake;

    //Create a new file + a clone of old tree in new file
    TString filenameoutSig = fZana_RecoSkimmed_str[sample];
    TString cutSig = "Z_pt>0";
    filenameoutSig.ReplaceAll("_RecoSkimmed","_MikeRecoSkimmed");
    
    cutSig+="&& evtHasGoodVtx && evtHasTrg && Z_mass>50 && MuPos_charge != MuNeg_charge && \
    ( (MuPosTrg && TMath::Abs(MuPos_eta)<2.1 &&  MuPos_pt>25 && MuPosIsTight && MuPosRelIso<0.12 && MuPos_dxy<0.02 \
    && TMath::Abs(MuNeg_eta)<2.4 && MuNeg_pt>10 ) \
    || ( MuNegTrg && TMath::Abs(MuNeg_eta)<2.1 &&  MuNeg_pt>25 && MuNegIsTight && MuNegRelIso<0.12 && MuNeg_dxy<0.02 \
    && TMath::Abs(MuPos_eta)<2.4 && MuPos_pt>10 ) )";

    
    cout << "creating file " << Form("%s",filenameoutSig.Data()) << endl;
    cout << "with cut " << Form("%s",cutSig.Data()) << endl;
    
    // newfileSig = new TFile(Form("%s",filenameoutSig.Data()),"recreate");
    newfileSig = TFile::Open(Form("Z_tree_temp3.root"),"recreate");
    newtreeSig = oldtree->CopyTree(cutSig.Data(),"");
    
    TBranch *b_Z_pt,*b_Mu_pt,*b_u1,*b_u2;
    Double_t Z_pt,Mu_pt,pt_vis,phi_vis,u1,u2,u1corr,u2corr;

    // if(sample==1 || sample==6){
      // newtreeSig->SetBranchAddress("Z_pt", &Z_pt, &b_Z_pt);
      // newtreeSig->SetBranchAddress("Z_phi", &Mu_pt, &b_Mu_pt);
      // TBranch* aBra1 = newtreeSig->Branch("pt_vis", &pt_vis, "pt_vis/D");
      // TBranch* aBra2 = newtreeSig->Branch("phi_vis", &phi_vis, "phi_vis/D");

      // newtreeSig->SetBranchAddress("u1", &u1, &b_u1);
      // newtreeSig->SetBranchAddress("u2", &u2, &b_u2);
      // TBranch* aBra3 = newtreeSig->Branch("u1corr", &u1corr, "u1corr/D");
      // TBranch* aBra4 = newtreeSig->Branch("u2corr", &u2corr, "u2corr/D");

      // for(int i=0; i<newtreeSig->GetEntries(); i++){
        // newtreeSig->GetEntry(i);
        // // Other calculations here...
        // pt_vis=Z_pt;
        // phi_vis=Mu_pt;
        // aBra1->Fill();
        // aBra2->Fill();
        // u1corr=-u1;
        // u2corr=-u2;
        // aBra3->Fill();
        // aBra4->Fill();
      // } // i
    // // }

    cout << "file " << Form("%s",filenameoutSig.Data()) << " created" << endl;

    // newtreeSig->Print();
    newtreeSig->AutoSave();
    newfileSig->Close();
    delete newfileSig;
    // continue;
    gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp Z_tree_temp3.root %s",filenameoutSig.Data()));
    gROOT->ProcessLine(Form(".! rm Z_tree_temp3.root"));

    delete newfileFake;
    delete oldfile;
  }
}
