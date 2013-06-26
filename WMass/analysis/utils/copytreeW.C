void copytreeW() {

  // TString root_folder="/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/";
  // TString fWana_str[7] = {
    // root_folder+"batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree.root",
    // root_folder+"batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree.root",
    // root_folder+"batch_TT_Trg/TTJets/WTreeProducer/WTreeProducer_tree.root",
    // root_folder+"batch_VV_Trg/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
    // root_folder+"batch_VV_Trg/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
    // root_folder+"batch_VV_Trg/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree.root",
    // root_folder+"batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree.root"
  // };  
  // TString fWana_RecoSkimmed_str[7] = {
    // root_folder+"batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    // root_folder+"batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    // root_folder+"batch_TT_Trg/TTJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    // root_folder+"batch_VV_Trg/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    // root_folder+"batch_VV_Trg/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    // root_folder+"batch_VV_Trg/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    // root_folder+"batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root"
  // };  

  // TString root_folder="~/eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
  // /afs/cern.ch/project/eos/installation/0.2.30/bin/eos.select cp 
  // /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/analysis/utils/W_tree_temp.root 
  // /eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/WJets/WTreeProducer_tree_SignalRecoSkimmed.root
  TString root_folder="/eos/cms/store/cmst3/user/perrozzi/CMG/ntuples_2012_12_20/";
  TString fWana_str[7] = {
    "root://eoscms//"+root_folder+"WJets/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DYJets/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"TTJets/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/ZZ/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WW/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"VVJets/WZ/WTreeProducer_tree.root",
    "root://eoscms//"+root_folder+"DATA/WTreeProducer_tree.root"
  };  
  TString fWana_RecoSkimmed_str[7] = {
    root_folder+"WJets/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DYJets/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"TTJets/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/ZZ/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WW/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"VVJets/WZ/WTreeProducer_tree_RecoSkimmed.root",
    root_folder+"DATA/WTreeProducer_tree_RecoSkimmed.root"
  };  

  // int sample = 0;
  for(int sample=0; sample<7; sample++){
  // for(int sample=0; sample<1; sample++){
  // for(int sample=5; sample<6; sample++){

    // if(sample!=5) continue;
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
    cout << "file " << Form("%s",filenameoutSig.Data()) << " created" << endl;

    // newtreeSig->Print();
    newtreeSig->AutoSave();
    delete newfileSig;
    gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.2.30/bin/eos.select cp W_tree_temp.root %s",filenameoutSig.Data()));
    gROOT->ProcessLine(Form(".! rm W_tree_temp.root"));

    if(sample<2){
      TString filenameoutFake = fWana_RecoSkimmed_str[sample];
      TString cutFake = "W_pt>0&&WGen_pt<0";
      filenameoutFake.ReplaceAll("_RecoSkimmed","_FakeRecoSkimmed");
      // newfileFake = TFile::Open(Form("%s",filenameoutFake.Data()),"recreate");
      newfileFake = TFile::Open(Form("W_tree_temp2.root"),"recreate");
      newtreeFake = oldtree->CopyTree(cutFake.Data());

      // newtreeFake->Print();
      newtreeFake->AutoSave();
      delete newfileFake;
      gROOT->ProcessLine(Form(".! /afs/cern.ch/project/eos/installation/0.2.30/bin/eos.select cp W_tree_temp2.root %s",filenameoutFake.Data()));
      gROOT->ProcessLine(Form(".! rm W_tree_temp2.root"));
    }
    
    
    delete oldfile;
    
    
  }
}
