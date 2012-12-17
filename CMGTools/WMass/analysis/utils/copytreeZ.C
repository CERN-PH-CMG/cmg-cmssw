void copytreeZ() {

  TString fZana_str[7] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TT_Trg/TTJets/ZTreeProducer/ZTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree.root"
  };
  TString fZana_RecoSkimmed_str[7] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TT_Trg/TTJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root"
  };  

  // int sample = 0;
  for(int sample=0; sample<7; sample++){
  // for(int sample=6; sample<7; sample++){
  
  if(sample!=0) continue;
  // cout << fZana_str[sample]<< endl;
  // cout << fZana_RecoSkimmed_str[sample]<< endl;
  // continue;
    
    //Get old file, old tree and set top branch address
    TFile *oldfile = new TFile(Form("%s",fZana_str[sample].Data()));
    TTree *oldtree = (TTree*)oldfile->Get("ZTreeProducer");

    // oldtree->SetBranchStatus("WGen_pt",0);
    
    if(sample>2) oldtree->SetBranchStatus("ZGen_pt",0);
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
    newfileSig = new TFile(Form("%s",filenameoutSig.Data()),"recreate");
    newtreeSig = oldtree->CopyTree(cutSig.Data());
    cout << "file " << Form("%s",filenameoutSig.Data()) << " created" << endl;

    newtreeSig->Print();
    newtreeSig->AutoSave();
    newfileSig->Close();
    delete newfileSig;


    if(sample<2){
      TString filenameoutFake = fZana_RecoSkimmed_str[sample];
      TString cutFake = "Z_pt>0&&ZGen_pt<0";
      filenameoutFake.ReplaceAll("_RecoSkimmed","_FakeRecoSkimmed");
      cout << "creating file " << Form("%s",filenameoutFake.Data()) << endl;
      newfileFake = new TFile(Form("%s",filenameoutFake.Data()),"recreate");
      newtreeFake = oldtree->CopyTree(cutFake.Data());
      newtreeFake->Print();
      newtreeFake->AutoSave();
      newfileFake->Close();

    }
    
    
    delete newfileFake;
    delete oldfile;
  }
}
