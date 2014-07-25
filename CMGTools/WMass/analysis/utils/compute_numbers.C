void compute_numbers(){

  static const int nsamples=7;
  enum                            {   WJets ,   DYJets ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  DATA };
  TString     sample[nsamples]  = {  "WJets",  "DYJets",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "DATA"};
  double       CS_pb[nsamples]  = {   31314 ,   2895.6 ,   157.51 ,   0.119  ,   4.514  ,   0.596 ,   -1   };
                                                                                                 // Run2011A  Run2011B  JSON  TRIGGER GOOD VTX
  double       Nevts[nsamples]  = {81053834 , 36209629 , 59136395 , 1103468  , 1197558  , 1221134 , (50367238+91137428)*0.8894*0.3995*0.9996   };
  double int_lumi_fb[nsamples]  = {       0 ,        0 ,        0 ,       0  ,       0  ,       0 , 4.7499};
  double Nevts_Per_fb[nsamples] = {       0 ,        0 ,        0 ,       0  ,       0  ,       0 ,   0   };
  
  TFile *fWana[nsamples], *fZana[nsamples];
  TString fWana_str[nsamples] = {
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed2mt/WJets/WTreeProducer/WTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJets_10wGen_WanaAndCounts/DYJets/WTreeProducer/WTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen/TTJets/WTreeProducer/WTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_data_Run2011AB_Nov8Nov19ReReco_v1/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree.root"
              };  
  TString fZana_str[nsamples] = {
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJets_10wGen_ZanaAndCounts/WJets/ZTreeProducer/ZTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGenFixed/DYJets/ZTreeProducer/ZTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen/TTJets/ZTreeProducer/ZTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree.root",
                "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_data_Run2011AB_Nov8Nov19ReReco_v1/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree.root"
               };
  TTree *Wtree[nsamples], *Ztree[nsamples];
  
  TString Wcutname1="W Only 1 trig Mu: ";
  TString Wcutstring1="evtHasGoodVtx&&evtHasTrg";
  TString Wcutname2="Trg Mu is Tight, Iso, pt>30,eta<2.1 : ";
  TString Wcutstring2="evtHasGoodVtx&&evtHasTrg";
  TString Zcutname1="Z Only 1 trig Mu: ";
  TString Zcutstring1="evtHasGoodVtx&&evtHasTrg";
  TString Zcutname2="Z Only 1 trig Mu: ";
  TString cutstring2="evtHasGoodVtx&&evtHasTrg";
  
  cout << endl;
  for(int i=0;i<nsamples;i++){
    if(CS_pb[i] >0) int_lumi_fb[i] = Nevts[i]/CS_pb[i]/1e3;
    Nevts_Per_fb[i] = Nevts[i]/int_lumi_fb[i];
    fWana[i] = new TFile(Form("%s",fWana_str[i].Data()));
    fZana[i] = new TFile(Form("%s",fZana_str[i].Data()));
    cout << "Sample " << sample[i] << endl;
    cout << "CS_pb= " << CS_pb[i] << " Nevts= " << Nevts[i] << " int_lumi_fb= " << int_lumi_fb[i] << " Nevts_Per_fb= " << Nevts_Per_fb[i] << endl;
    cout << "W ana " ;
    fWana[i]->Print();
    Wtree[i]= (TTree*) fWana[i]->Get("WTreeProducer");
    cout << "Tot ntuple entries: " << Long64_t (Wtree[i]->GetEntries()) << endl;
    double n1 = ((double) Wtree[i]->GetEntries(Wcutstring1)) / ((double) Nevts[i] ) ;
    cout << Wcutname1 << n1 << endl;
    cout << endl;
    
    cout << "Z ana ";
    fZana[i]->Print();
    Ztree[i]= (TTree*) fZana[i]->Get("ZTreeProducer");
    cout << "Tot ntuple entries: " << Long64_t (Wtree[i]->GetEntries()) << endl;
    double n1 = ((double) Ztree[i]->GetEntries(Zcutstring1)) / ((double) Nevts[i] ) ;
    cout << "Z OS Mu PAIR ( >= 1 trig): " << n1 << endl;
    cout << endl;
    cout << endl;
  }
  
 

}