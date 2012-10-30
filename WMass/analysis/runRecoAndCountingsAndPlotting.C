
void runRecoAndCountingsAndPlotting(){

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);

  // STEERING PARAMETERS
  
  TString foldername = "SelNumbers_prova2";
    
    bool runWanalysisOnDATA= 0;
    bool runZanalysisOnDATA= 1;

    bool ExtractNumbers= 0;

    bool run_ZvsWlike_comparisonMC = 0;
    bool run_ZvsWlike_comparisonDATA = 0;

    bool runWandZcomparisonMC = 0;
    bool runWandZcomparisonDATA = 0;
    
    
    bool run_Z_MCandDATAcomparison= 0;
    bool run_W_MCandDATAcomparison= 0;

    
  // END STEERING PARAMETERS
    
  static const int nsamples=9;
  enum                            {  WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  DATA };
  TString     sample[nsamples]  = { "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "DATA"};
  double       CS_pb[nsamples]  = {    31314  ,      31314 ,   2895.6   ,      2895.6 ,   157.51 ,   0.119  ,   4.514  ,   0.596 ,   -1   };
  // Run2011A  Run2011B  JSON  TRIGGER GOOD VTX
  double       Nevts[nsamples]  = { 81053834  ,   81053834 ,  36209629  ,    36209629 , 59136395 , 1103468  , 1197558  , 1221134 , (50367238+91137428)*0.8894*0.3995*0.9996   };
  double int_lumi_fb[nsamples]  = {        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 , 4.7499};
  double Nevts_Per_fb[nsamples] = {        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,   0   };
  
  TString fWana_str[nsamples] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed2mt/WJets/WTreeProducer/WTreeProducer_tree_SignalRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed2mt/WJets/WTreeProducer/WTreeProducer_tree_FakeRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJets_10wGen_WanaAndCounts/DYJets/WTreeProducer/WTreeProducer_tree_SignalRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJets_10wGen_WanaAndCounts/DYJets/WTreeProducer/WTreeProducer_tree_FakeRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen/TTJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_data_Run2011AB_Nov8Nov19ReReco_v1/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root"
  };  
  TString fZana_str[nsamples] = {
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJets_10wGen_ZanaAndCounts/WJets/ZTreeProducer/ZTreeProducer_tree_SignalRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJets_10wGen_ZanaAndCounts/WJets/ZTreeProducer/ZTreeProducer_tree_FakeRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGenFixed/DYJets/ZTreeProducer/ZTreeProducer_tree_SignalRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGenFixed/DYJets/ZTreeProducer/ZTreeProducer_tree_FakeRecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen/TTJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_VVJets_10wGen/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root",
    "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_data_Run2011AB_Nov8Nov19ReReco_v1/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root"
  };

  // WJETS
  // TString jobID= "test_numbers_WJETS";
  // TString WfileDATA= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed2mt/WJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root";
  // TString ZfileDATA= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJets_10wGen_ZanaAndCounts/WJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root";
  // int tot_N_evts = 81053834;
  // // DYJETS
  // TString jobID= "test_numbers_DYJETS";
  // TString WfileDATA= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJets_10wGen_WanaAndCounts/DYJets/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root";
  // TString ZfileDATA= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGenFixed/DYJets/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root";
  // int tot_N_evts = 36209629;
  
    
  if(runWanalysisOnDATA || runZanalysisOnDATA || ExtractNumbers){
    for(int i=0;i<nsamples;i++){
    // for(int i=2;i<4;i++){
      
      TString jobID= Form("test_numbers_%s",sample[i].Data());
      
      cout << endl;
      cout << "ANALYZING jobID= " << jobID << endl;
      
      TString WfileDATA= fWana_str[i];
      TString ZfileDATA= fZana_str[i];
      int tot_N_evts = Nevts[i];

      TString ZMass = "91.1876"; // 91.1876
      TString WMassCentral_MeV = "80385"; // 80385
      TString WMassStep_MeV = "100"; // 15
      TString WMassNSteps = "0"; // 60
      TString etaMuonNSteps = "3"; // 5
      TString etaMaxMuons = "0.6, 1.2, 2.1"; // 0.6, 0.8, 1.2, 1.6, 2.1

      if(CS_pb[i] >0) int_lumi_fb[i] = Nevts[i]/CS_pb[i]/1e3;
      // Nevts_Per_fb[i] = Nevts[i]/int_lumi_fb[i];
      double WfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i];
      double ZfileDATA_lumi_SF = int_lumi_fb[DATA]/int_lumi_fb[i];
      cout << "lumi_SF= " << Form("%e",ZfileDATA_lumi_SF) << endl;
      // CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
      bool IS_MC_CLOSURE_TEST= 0; // DO NOT TOUCH IN THIS CASE, ALWAYS LEAVE TO 0
      
      // END OF STEERING PARAMETERS

      ///////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////
      
      // RUNNING CODE

      TString outputdir;
      if(jobID.Data()=="") outputdir="dummytest";
      else outputdir=jobID.Data();
      
      // print working directory
      cout << gSystem->WorkingDirectory() << endl;
      cout << "checking directory "<<Form("JobOutputs/%s/%s",foldername.Data(),outputdir.Data()) << endl;
      // check if the outputfolder is already existing
      int dir_check = gROOT->ProcessLine(Form(".! ls JobOutputs/%s/%s >/dev/null",foldername.Data(),outputdir.Data()));
      // make the outputfolder even if is already existing (nothing happens)
      cout << "making directory "<<Form("JobOutputs/%s/%s",foldername.Data(),outputdir.Data()) << endl;
      gSystem->MakeDirectory(Form("JobOutputs/%s",foldername.Data()));
      gSystem->MakeDirectory(Form("JobOutputs/%s/%s",foldername.Data(),outputdir.Data()));
      
      
      // make the outputfolder trahs were backup of the folder will be stored (overwrite protection!)
      gSystem->MakeDirectory(Form("JobOutputs/%s/%s/trash",foldername.Data(),outputdir.Data()));
      gROOT->ProcessLine(Form(".! cp JobOutputs/%s/%s/\* JobOutputs/%s/%s/trash/",foldername.Data(),outputdir.Data(),foldername.Data(),outputdir.Data()));
      
      
      if( dir_check ==0 ) // if the output folder existed use the same common.h
      gROOT->ProcessLine(Form(".! cp JobOutputs/%s/%s/common.h includes/common.h",foldername.Data(),outputdir.Data()));
      else{ // otherwise build it from this cfg
        gROOT->ProcessLine(Form(".! cp includes/common.h.bkp includes/common.h"));
        gROOT->ProcessLine(Form(".! sh manipulate_parameters.sh \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ",ZMass.Data(),WMassCentral_MeV.Data(),WMassStep_MeV.Data(),WMassNSteps.Data(),etaMuonNSteps.Data(),etaMaxMuons.Data()));
        gROOT->ProcessLine(Form(".! cp includes/common.h JobOutputs/%s/%s/common.h",foldername.Data(),outputdir.Data()));
      }
      

      gSystem->ChangeDirectory("DATAcode/");
      
      if(runWanalysisOnDATA){
        gSystem->CompileMacro("WanalysisOnDATA.C");
        gROOT->ProcessLine(Form("WanalysisOnDATA wDATA(\"%s\",%f)",WfileDATA.Data(),WfileDATA_lumi_SF));
        gROOT->ProcessLine(Form("wDATA.Loop(%d)",IS_MC_CLOSURE_TEST));
        gROOT->ProcessLine(Form(".! mv WanalysisOnDATA.root ../JobOutputs/%s/%s/WanalysisOnDATA.root",foldername.Data(),outputdir.Data()));
        gROOT->ProcessLine(Form(".! cp WanalysisOnDATA.\* ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
      }

      if(runZanalysisOnDATA){
        gSystem->CompileMacro("ZanalysisOnDATA.C");
        gROOT->ProcessLine(Form("ZanalysisOnDATA zDATA(\"%s\",%f)",ZfileDATA.Data(),ZfileDATA_lumi_SF));
        gROOT->ProcessLine(Form("zDATA.Loop(%d)",IS_MC_CLOSURE_TEST));
        gROOT->ProcessLine(Form(".! mv ZanalysisOnDATA.root ../JobOutputs/%s/%s/ZanalysisOnDATA.root",foldername.Data(),outputdir.Data()));
        gROOT->ProcessLine(Form(".! cp ZanalysisOnDATA.\* ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
      }
      
      gSystem->ChangeDirectory("../MCcode/");
      
      if(ExtractNumbers){
        gROOT->ProcessLine(Form(".x R_WdivZ.C(\"../JobOutputs/%s/%s\",1,%d) > numbers.txt",foldername.Data(),outputdir.Data(),tot_N_evts));
        // gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/%s/R_WdivZ_OnMC.root",foldername.Data(),outputdir.Data()));
        gROOT->ProcessLine(Form(".! mv \*.txt ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
        // gROOT->ProcessLine(Form(".! cp R_WdivZ.C ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
      }
      
      gSystem->ChangeDirectory("../");
      
    }
  }
  
  if(run_ZvsWlike_comparisonMC){
    gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DYJetsSig].Data());
    gROOT->ProcessLine(Form(".x PlotZvsWlikeDistributionsDATA.C(\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZvsWlikeDistributionsDATA.C ../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
    gSystem->ChangeDirectory("../");
  }
  if(run_ZvsWlike_comparisonDATA){
    gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotZvsWlikeDistributionsDATA.C(\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZvsWlikeDistributionsDATA.C ../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
    gSystem->ChangeDirectory("../");
  }
  
  if(runWandZcomparisonMC){
    gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DYJetsSig].Data());
    TString jobIDW= Form("test_numbers_%s",sample[WJetsSig].Data());
    gROOT->ProcessLine(Form(".x PlotWvsZdistributionsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data(),foldername.Data(),jobIDW.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    gSystem->ChangeDirectory("../");
  }
  
  if(runWandZcomparisonDATA){
    gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DATA].Data());
    TString jobIDW= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotWvsZdistributionsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data(),foldername.Data(),jobIDW.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    gSystem->ChangeDirectory("../");
  }
  
  if(run_Z_MCandDATAcomparison){
    gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    TString jobIDMC= Form("test_numbers_%s",sample[DYJetsSig].Data());
    TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotZdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZdistributionsMCvsDATA.C ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    gSystem->ChangeDirectory("../");
  }

}
