void copytreeZ() {

  // TString fZana_str[7] = {
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TT_Trg/TTJets/ZTreeProducer/ZTreeProducer_tree.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree.root"
  // };
  // TString fZana_RecoSkimmed_str[7] = {
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TT_Trg/TTJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VV_Trg/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    // "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root"
  // };  
  TString root_folder="/eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
  TString fZana_str[7] = {
    "root://eoscms//"+root_folder+"WJets/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DYJets/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"TTJets/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/ZZ/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WW/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WZ/ZTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DATA/ZTreeProducer_tree.root"
  };  
  TString fZana_RecoSkimmed_str[7] = {
    root_folder+"WJets/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DYJets/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"TTJets/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/ZZ/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WW/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WZ/ZTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DATA/ZTreeProducer_tree_RecoSkimmed.root"
  };  

  // int sample = 0;
  for(int sample=0; sample<7; sample++){
  // for(int sample=6; sample<7; sample++){
  
  if(sample!=6) continue;
  // cout << fZana_str[sample]<< endl;
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
    newtreeSig = oldtree->CopyTree(cutSig.Data());
    cout << "file " << Form("%s",filenameoutSig.Data()) << " created" << endl;

    // newtreeSig->Print();
    newtreeSig->AutoSave();
    newfileSig->Close();
    delete newfileSig;
    gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.2.30/bin/eos.select cp Z_tree_temp.root %s",filenameoutSig.Data()));
    gROOT->ProcessLine(Form(".! rm Z_tree_temp.root"));

    if(sample<2){
      TString filenameoutFake = fZana_RecoSkimmed_str[sample];
      TString cutFake = "Z_pt>0&&ZGen_pt<0";
      filenameoutFake.ReplaceAll("_RecoSkimmed","_FakeRecoSkimmed");
      cout << "creating file " << Form("%s",filenameoutFake.Data()) << endl;
      // newfileFake = new TFile(Form("%s",filenameoutFake.Data()),"recreate");
      newfileFake = TFile::Open(Form("Z_tree_temp2.root"),"recreate");
      newtreeFake = oldtree->CopyTree(cutFake.Data());
      
      // newtreeFake->Print();
      newtreeFake->AutoSave();
      newfileFake->Close();
      gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.2.30/bin/eos.select cp Z_tree_temp2.root %s",filenameoutFake.Data()));
      gROOT->ProcessLine(Form(".! rm Z_tree_temp2.root"));

    }
    
    
    delete newfileFake;
    delete oldfile;
  }
}
