void copytreeW() {

  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_09_14/";
  // TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_10_15/";
  TString root_folder="/eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/";
  
  TString fWana_str[7] = {
    "root://eoscms//"+root_folder+"WJetsLL/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DYJetsMM/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"TTJets/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/ZZ/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WWJetsTo2L2Nu/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WZ/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DATA/WTreeProducer_tree.root"
  };  
  TString fWana_RecoSkimmed_str[7] = {
    root_folder+"WJetsLL/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DYJetsMM/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"TTJets/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/ZZ/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WWJetsTo2L2Nu/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WZ/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DATA/WTreeProducer_tree_RecoSkimmed.root"
  };  

  // int sample = 0;
  for(int sample=0; sample<7; sample++){
  // for(int sample=3; sample<6; sample++){
  // for(int sample=5; sample<6; sample++){

    // if(sample!=1) continue;
    if(
       // !fWana_RecoSkimmed_str[sample].Contains("WJetsLL")
       // && 
       !fWana_RecoSkimmed_str[sample].Contains("DYJetsMM")
       // && !fWana_RecoSkimmed_str[sample].Contains("DATA")
       ) continue;
    // if(sample==0 || sample==6) continue;
    // cout << fZana_str[sample]<< endl;
    // cout << fZana_RecoSkimmed_str[sample]<< endl;
    // continue;
  
    //Get old file, old tree and set top branch address
    TFile *oldfile = TFile::Open(Form("%s",fWana_str[sample].Data()));
    TTree *oldtree = (TTree*)oldfile->Get("WTreeProducer");

    if(sample>2){
      oldtree->SetBranchStatus("WGen_pt",0);
      oldtree->SetBranchStatus("WGen_phi",0);
      oldtree->SetBranchStatus("WGen_rap",0);
      oldtree->SetBranchStatus("WGen_m",0);
      oldtree->SetBranchStatus("WGen_mt",0);
      oldtree->SetBranchStatus("MuGen_pt",0);
      oldtree->SetBranchStatus("MuGen_eta",0);
      oldtree->SetBranchStatus("MuGen_phi",0);
      oldtree->SetBranchStatus("MuGen_mass",0);
      oldtree->SetBranchStatus("MuGen_charge",0);
      oldtree->SetBranchStatus("MuDRGenP",0);
      oldtree->SetBranchStatus("NuGen_pt",0);
      oldtree->SetBranchStatus("NuGen_eta",0);
      oldtree->SetBranchStatus("NuGen_phi",0);
      oldtree->SetBranchStatus("NuGen_mass",0);
      oldtree->SetBranchStatus("NuGen_charge",0);
    }
    // //Create a new file + a clone of old tree in new file
    // TFile *newfile = TFile::Open(Form("%s",fWana_RecoSkimmed_str[sample].Data()),"recreate");
    // TTree *newtree = oldtree->CopyTree("W_pt>0");

    // newtree->Print();
    // newtree->AutoSave();
    // delete oldfile;
    // delete newfile;
    
    TFile *newfileSig,*newfileFake;
    TTree *newtreeSig,*newtreeFake;

    //Create a new file + a clone of old tree in new file
    TString filenameoutSig = fWana_RecoSkimmed_str[sample];
    TString cutSig = "W_pt>0";
    if(sample<2){
      filenameoutSig.ReplaceAll("_RecoSkimmed","_SignalRecoSkimmed");
      cutSig+="&&WGen_pt>0";
    }
    cout << "creating file " << Form("%s",filenameoutSig.Data()) << endl;
    // newfileSig = TFile::Open(Form("%s",filenameoutSig.Data()),"recreate");
    newfileSig = TFile::Open(Form("W_tree_temp.root"),"recreate");
    newtreeSig = oldtree->CopyTree(cutSig.Data());
    
    TBranch *b_Mu_pt,*b_Mu_phi,*b_u1,*b_u2;
    Double_t Mu_pt,Mu_phi,pt_vis,phi_vis,u1,u2,u1corr,u2corr;

    // if(sample==0 || sample==6){
      newtreeSig->SetBranchAddress("Mu_pt", &Mu_pt, &b_Mu_pt);
      newtreeSig->SetBranchAddress("Mu_phi", &Mu_phi, &b_Mu_phi);
      TBranch* aBra1 = newtreeSig->Branch("pt_vis", &pt_vis, "pt_vis/D");
      TBranch* aBra2 = newtreeSig->Branch("phi_vis", &phi_vis, "phi_vis/D");

      newtreeSig->SetBranchAddress("u1", &u1, &b_u1);
      newtreeSig->SetBranchAddress("u2", &u2, &b_u2);
      TBranch* aBra3 = newtreeSig->Branch("u1corr", &u1corr, "u1corr/D");
      TBranch* aBra4 = newtreeSig->Branch("u2corr", &u2corr, "u2corr/D");

      for(int i=0; i<newtreeSig->GetEntries(); i++){
        newtreeSig->GetEntry(i);
        // Other calculations here...
        pt_vis=Mu_pt;
        phi_vis=Mu_phi;
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
    delete newfileSig;
    // continue;
    gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp W_tree_temp.root %s",filenameoutSig.Data()));
    gROOT->ProcessLine(Form(".! rm W_tree_temp.root"));

    if(sample<2){
      TString filenameoutFake = fWana_RecoSkimmed_str[sample];
      TString cutFake = "W_pt>0&&WGen_pt<0";
      filenameoutFake.ReplaceAll("_RecoSkimmed","_FakeRecoSkimmed");
      // newfileFake = TFile::Open(Form("%s",filenameoutFake.Data()),"recreate");
      newfileFake = TFile::Open(Form("W_tree_temp2.root"),"recreate");
      newtreeFake = oldtree->CopyTree(cutFake.Data());

      TBranch *b_Mu_pt,*b_Mu_phi,*b_u1,*b_u2;
      Double_t Mu_pt,Mu_phi,pt_vis,phi_vis,u1,u2,u1corr,u2corr;

      // if(sample==0 || sample==6){
        newtreeFake->SetBranchAddress("Mu_pt", &Mu_pt, &b_Mu_pt);
        newtreeFake->SetBranchAddress("Mu_phi", &Mu_phi, &b_Mu_phi);
        TBranch* aBra1 = newtreeFake->Branch("pt_vis", &pt_vis, "pt_vis/D");
        TBranch* aBra2 = newtreeFake->Branch("phi_vis", &phi_vis, "phi_vis/D");

        newtreeFake->SetBranchAddress("u1", &u1, &b_u1);
        newtreeFake->SetBranchAddress("u2", &u2, &b_u2);
        TBranch* aBra3 = newtreeFake->Branch("u1corr", &u1corr, "u1corr/D");
        TBranch* aBra4 = newtreeFake->Branch("u2corr", &u2corr, "u2corr/D");

        for(int i=0; i<newtreeFake->GetEntries(); i++){
          newtreeFake->GetEntry(i);
          // Other calculations here...
          pt_vis=Mu_pt;
          phi_vis=Mu_phi;
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
      delete newfileFake;
      gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select cp W_tree_temp2.root %s",filenameoutFake.Data()));
      gROOT->ProcessLine(Form(".! rm W_tree_temp2.root"));
    }
    
    
    delete oldfile;
    
    
  }
}
