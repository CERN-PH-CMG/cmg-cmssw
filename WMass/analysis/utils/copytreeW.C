void copytreeW() {

  TString fWana_str[7] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TT_Trg/TTJets/WTreeProducer/WTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree.root"
  };  
  TString fWana_RecoSkimmed_str[7] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TT_Trg/TTJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root"
  };  

  // int sample = 0;
  for(int sample=0; sample<7; sample++){
  // for(int sample=6; sample<7; sample++){

    if(sample!=0) continue;
    // cout << fZana_str[sample]<< endl;
    // cout << fZana_RecoSkimmed_str[sample]<< endl;
    // continue;
  
    //Get old file, old tree and set top branch address
    TFile *oldfile = new TFile(Form("%s",fWana_str[sample].Data()));
    TTree *oldtree = (TTree*)oldfile->Get("WTreeProducer");

    if(sample>2) oldtree->SetBranchStatus("WGen_pt",0);
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
    
    // //Create a new file + a clone of old tree in new file
    // TFile *newfile = new TFile(Form("%s",fWana_RecoSkimmed_str[sample].Data()),"recreate");
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
    newfileSig = new TFile(Form("%s",filenameoutSig.Data()),"recreate");
    newtreeSig = oldtree->CopyTree(cutSig.Data());
    cout << "file " << Form("%s",filenameoutSig.Data()) << " created" << endl;

    newtreeSig->Print();
    newtreeSig->AutoSave();
    delete newfileSig;

    if(sample<2){
      TString filenameoutFake = fWana_RecoSkimmed_str[sample];
      TString cutFake = "W_pt>0&&WGen_pt<0";
      filenameoutFake.ReplaceAll("_RecoSkimmed","_FakeRecoSkimmed");
      newfileFake = new TFile(Form("%s",filenameoutFake.Data()),"recreate");
      newtreeFake = oldtree->CopyTree(cutFake.Data());

      newtreeFake->Print();
      newtreeFake->AutoSave();
      delete newfileFake;
    }
    
    
    delete oldfile;
    
    
  }
}
