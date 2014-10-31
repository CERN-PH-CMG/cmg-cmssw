void copytreeZ() {

  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/";
  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_10_15/";
  TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/";
  
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
  // for(int sample=3; sample<6; sample++){
  
    // if(sample!=1 && sample!=6) continue;
    // if(sample!=6) continue;
    // if(sample==1 || sample==6) continue;
    // cout << fZana_str[sample]<< endl;
    // if(!fZana_RecoSkimmed_str[sample].Contains("TTJets")) continue;
    if(
      // !fZana_RecoSkimmed_str[sample].Contains("WJetsLL")
       // && 
       !fZana_RecoSkimmed_str[sample].Contains("DYJetsMM")
       // && 
       // !fZana_RecoSkimmed_str[sample].Contains("DATA")
       ) continue;
    // cout << fZana_RecoSkimmed_str[sample]<< endl;
    // continue;
      
    //Get old file, old tree and set top branch address
    // TFile *oldfile = new TFile(Form("%s",fZana_str[sample].Data()));
    TFile *oldfile = TFile::Open(Form("%s",fZana_str[sample].Data()));
    TTree *oldtree = (TTree*)oldfile->Get("ZTreeProducer");

    // oldtree->SetBranchStatus("WGen_pt",0);
    
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

    TFile *newfileSig,*newfileFake;
    TTree *newtreeSig,*newtreeFake;

    //Create a new file + a clone of old tree in new file
    TString filenameoutSig = fZana_RecoSkimmed_str[sample];
    TString cutSig = "Z_pt>0";
    if(sample<2){
      filenameoutSig.ReplaceAll("_RecoSkimmed","_SignalRecoSkimmed");
      cutSig+="&&ZGen_pt>0";
    }
    cout << "creating file " << Form("%s",filenameoutSig.Data()) << endl;
    // newfileSig = new TFile(Form("%s",filenameoutSig.Data()),"recreate");
    newfileSig = TFile::Open(Form("Z_tree_temp.root"),"recreate");
    newtreeSig = oldtree->CopyTree(cutSig.Data(),"");
    
    TBranch *b_Z_pt,*b_Mu_pt,*b_u1,*b_u2;
    Double_t Z_pt,Mu_pt,pt_vis,phi_vis,u1,u2,u1corr,u2corr;

    // if(sample==1 || sample==6){
      newtreeSig->SetBranchAddress("Z_pt", &Z_pt, &b_Z_pt);
      newtreeSig->SetBranchAddress("Z_phi", &Mu_pt, &b_Mu_pt);
      TBranch* aBra1 = newtreeSig->Branch("pt_vis", &pt_vis, "pt_vis/D");
      TBranch* aBra2 = newtreeSig->Branch("phi_vis", &phi_vis, "phi_vis/D");

      newtreeSig->SetBranchAddress("u1", &u1, &b_u1);
      newtreeSig->SetBranchAddress("u2", &u2, &b_u2);
      TBranch* aBra3 = newtreeSig->Branch("u1corr", &u1corr, "u1corr/D");
      TBranch* aBra4 = newtreeSig->Branch("u2corr", &u2corr, "u2corr/D");

      for(int i=0; i<newtreeSig->GetEntries(); i++){
        newtreeSig->GetEntry(i);
        // Other calculations here...
        pt_vis=Z_pt;
        phi_vis=Z_phi;
        aBra1->Fill();
        aBra2->Fill();
        u1corr=-u1;
        u2corr=-u2;
        aBra3->Fill();
        aBra4->Fill();
      } // i
    // }

    cout << "file " << Form("%s",filenameoutSig.Data()) << " created" << endl;

    // newtreeSig->Print();
    newtreeSig->AutoSave();
    newfileSig->Close();
    delete newfileSig;
    // continue;
    gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp Z_tree_temp.root %s",filenameoutSig.Data()));
    gROOT->ProcessLine(Form(".! rm Z_tree_temp.root"));

    if(sample<2){
      TString filenameoutFake = fZana_RecoSkimmed_str[sample];
      TString cutFake = "Z_pt>0&&ZGen_pt<0";
      filenameoutFake.ReplaceAll("_RecoSkimmed","_FakeRecoSkimmed");
      cout << "creating file " << Form("%s",filenameoutFake.Data()) << endl;
      // newfileFake = new TFile(Form("%s",filenameoutFake.Data()),"recreate");
      newfileFake = TFile::Open(Form("Z_tree_temp2.root"),"recreate");
      newtreeFake = oldtree->CopyTree(cutFake.Data());
      
      TBranch *b_Z_pt,*b_Mu_pt,*b_u1,*b_u2;
      Double_t Z_pt,Mu_pt,pt_vis,phi_vis,u1,u2,u1corr,u2corr;

      // if(sample==1 || sample==6){
        newtreeFake->SetBranchAddress("Z_pt", &Z_pt, &b_Z_pt);
        newtreeFake->SetBranchAddress("Z_phi", &Mu_pt, &b_Mu_pt);
        TBranch* aBra1 = newtreeFake->Branch("pt_vis", &pt_vis, "pt_vis/D");
        TBranch* aBra2 = newtreeFake->Branch("phi_vis", &phi_vis, "phi_vis/D");

        newtreeFake->SetBranchAddress("u1", &u1, &b_u1);
        newtreeFake->SetBranchAddress("u2", &u2, &b_u2);
        TBranch* aBra3 = newtreeFake->Branch("u1corr", &u1corr, "u1corr/D");
        TBranch* aBra4 = newtreeFake->Branch("u2corr", &u2corr, "u2corr/D");

        for(int i=0; i<newtreeFake->GetEntries(); i++){
          newtreeFake->GetEntry(i);
          // Other calculations here...
          pt_vis=Z_pt;
          phi_vis=Z_phi;
          aBra1->Fill();
          aBra2->Fill();
          u1corr=-u1;
          u2corr=-u2;
          aBra3->Fill();
          aBra4->Fill();
        } // i
      // }

      // newtreeFake->Print();
      newtreeFake->AutoSave();
      newfileFake->Close();
      gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp Z_tree_temp2.root %s",filenameoutFake.Data()));
      gROOT->ProcessLine(Form(".! rm Z_tree_temp2.root"));

    }
    
    
    delete newfileFake;
    delete oldfile;
  }
}
