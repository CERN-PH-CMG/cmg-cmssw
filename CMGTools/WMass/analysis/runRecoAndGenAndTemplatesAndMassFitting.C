
void runWMassAnalysisOnMC(){

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");

  // STEERING PARAMETERS

  // TString jobID= "test_looser_cuts_masscut";
  // TString jobID= "test_2nd_muon_ptcut30";
  // TString jobID= "test_distrib";
  // TString jobID= "test_customFit";
  TString jobID= "test_closureTest_normalized";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLL_3/WJets/WTreeProducer/WTreeProducer_tree_ok.root";
  // TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_4/DYJets/ZTreeProducer/ZTreeProducer_tree_METcontainslepton.root";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGen/WJets/WTreeProducer/WTreeProducer_tree.root";
  // TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGen/DYJets/ZTreeProducer/ZTreeProducer_tree.root";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed/WJets/WTreeProducer/WTreeProducer_tree.root";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen/TTJets/ZTreeProducer/ZTreeProducer_tree.root";
  // TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_TTJets_10wGen/TTJets/WTreeProducer/WTreeProducer_tree.root";
  // double WfileMC_int_lumi = 3754;
  TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed2mt/WJets/WTreeProducer/WTreeProducer_tree.root";
  double WfileMC_int_lumi = 2588;
  TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGenFixed/DYJets/ZTreeProducer/ZTreeProducer_tree.root";
  double ZfileMC_int_lumi = 12508;
  TString WfileDATA= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_data_Run2011AB_Nov8Nov19ReReco_v1/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree_RecoSkimmed.root";
  double WfileDATA_int_lumi = 4749.9;
  TString ZfileDATA= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_data_Run2011AB_Nov8Nov19ReReco_v1/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree_RecoSkimmed.root";
  double ZfileDATA_int_lumi = 4749.9;
  
  TString ZMass = "91.1876"; // 91.1876
  TString WMassCentral_MeV = "80385"; // 80385
  TString WMassStep_MeV = "100"; // 15
  TString WMassNSteps = "10"; // 60
  TString etaMuonNSteps = "3"; // 5
  TString etaMaxMuons = "0.6, 1.2, 2.1"; // 0.6, 0.8, 1.2, 1.6, 2.1

  // CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
  bool IS_MC_CLOSURE_TEST= 0;
  
  // PRODUCE DISTRIBUTIONS on DATA AND MC
  bool runWanalysisOnMC  = 1;
  bool runZanalysisOnMC  = 1;
  bool runWZPlotsMC      = 0;
  bool runWanalysisOnDATA= 0;
  bool runZanalysisOnDATA= 0;

  // PRODUCE R(X)=W/Z DISTRIBUTION TO REWEIGHT Z in DATA
  bool runR_WdivZ        = 0;

  // PRODUCE TEMPLATES, i.e. Z(DATA)*R(X)
  bool run_BuildTemplates= 1;
  
  // PERFORM W MASS FIT
  bool run_MassFit       = 1;
  double fitmin=0.8; //0.75;
  double fitmax=1.2; //1.2;
  
  // END OF STEERING PARAMETERS

  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
  
    if(IS_MC_CLOSURE_TEST){
      WfileDATA= WfileMC;
      WfileDATA_int_lumi=WfileMC_int_lumi;
      ZfileDATA= ZfileMC;
      ZfileDATA_int_lumi=ZfileMC_int_lumi;
    }

    double max_MC_int_lumi = TMath::Max(WfileMC_int_lumi,ZfileMC_int_lumi);
    WfileMC_int_lumi/=max_MC_int_lumi;
    ZfileMC_int_lumi/=max_MC_int_lumi;
    double max_DATA_int_lumi = TMath::Max(WfileDATA_int_lumi,ZfileDATA_int_lumi);
    WfileDATA_int_lumi/=max_DATA_int_lumi;
    ZfileDATA_int_lumi/=max_DATA_int_lumi;

  
  cout << "WfileMC_int_lumi= " << WfileMC_int_lumi << " ZfileMC_int_lumi= " << ZfileMC_int_lumi << endl;
  
  // RUNNING CODE

  TString outputdir;
  if(jobID.Data()=="") outputdir="dummytest";
  else outputdir=jobID.Data();
  
  // print working directory
  cout << gSystem->WorkingDirectory() << endl;
  cout << "checking directory "<<Form("JobOutputs/%s",outputdir.Data()) << endl;
  // check if the outputfolder is already existing
  int dir_check = gROOT->ProcessLine(Form(".! ls JobOutputs/%s >/dev/null",outputdir.Data()));
  // make the outputfolder even if is already existing (nothing happens)
  cout << "making directory "<<Form("JobOutputs/%s",outputdir.Data()) << endl;
  gSystem->MakeDirectory(Form("JobOutputs/%s",outputdir.Data()));
  
  // make the outputfolder trahs were backup of the folder will be stored (overwrite protection!)
  gSystem->MakeDirectory(Form("JobOutputs/%s/trash",outputdir.Data()));
  gROOT->ProcessLine(Form(".! cp JobOutputs/%s/\* JobOutputs/%s/trash/",outputdir.Data(),outputdir.Data()));
  
  
  if( dir_check ==0 ) // if the output folder existed use the same common.h
    gROOT->ProcessLine(Form(".! cp JobOutputs/%s/common.h includes/common.h",outputdir.Data()));
  else{ // otherwise build it from this cfg
    gROOT->ProcessLine(Form(".! cp includes/common.h.bkp includes/common.h"));
    gROOT->ProcessLine(Form(".! sh manipulate_parameters.sh \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" ",ZMass.Data(),WMassCentral_MeV.Data(),WMassStep_MeV.Data(),WMassNSteps.Data(),etaMuonNSteps.Data(),etaMaxMuons.Data()));
    gROOT->ProcessLine(Form(".! cp includes/common.h JobOutputs/%s/common.h",outputdir.Data()));
  }
  
  // enter in the MCCode directory
  gSystem->ChangeDirectory("MCcode/");

  // run the macros according to the steering parameters above
  if(runWanalysisOnMC){
    gSystem->CompileMacro("WanalysisOnMC.C");
    gROOT->ProcessLine(Form("WanalysisOnMC wMC(\"%s\",%.f)",WfileMC.Data(),WfileMC_int_lumi));
    gROOT->ProcessLine(Form("wMC.Loop(%d)",IS_MC_CLOSURE_TEST));
    gROOT->ProcessLine(Form(".! mv WanalysisOnMC.root ../JobOutputs/%s/WanalysisOnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp WanalysisOnMC.\* ../JobOutputs/%s/",outputdir.Data()));
  }

  if(runZanalysisOnMC){
    gSystem->CompileMacro("ZanalysisOnMC.C");
    gROOT->ProcessLine(Form("ZanalysisOnMC zMC(\"%s\",%.f)",ZfileMC.Data(),ZfileMC_int_lumi));
    gROOT->ProcessLine(Form("zMC.Loop(%d)",IS_MC_CLOSURE_TEST));
    gROOT->ProcessLine(Form(".! mv ZanalysisOnMC.root ../JobOutputs/%s/ZanalysisOnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp ZanalysisOnMC.\* ../JobOutputs/%s/",outputdir.Data()));
  }

  if(runWZPlotsMC){
    gSystem->MakeDirectory(Form("../JobOutputs/%s/WMassPlots",outputdir.Data()));
    gROOT->ProcessLine(Form(".x PlotWZdistributionsMC.C(\"../JobOutputs/%s\")",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/WMassPlots",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWZdistributionsMC.C ../JobOutputs/%s/",outputdir.Data()));
  }
  
  if(runR_WdivZ){
    gROOT->ProcessLine(Form(".x R_WdivZ.C(\"../JobOutputs/%s\")",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/R_WdivZ_OnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp R_WdivZ.C ../JobOutputs/%s/",outputdir.Data()));
  }

  gSystem->ChangeDirectory("../DATAcode/");
  
  if(runWanalysisOnDATA){
    gSystem->CompileMacro("WanalysisOnDATA.C");
    gROOT->ProcessLine(Form("WanalysisOnDATA wDATA(\"%s\",%.f)",WfileMC.Data(),WfileDATA_int_lumi));
    gROOT->ProcessLine(Form("wDATA.Loop(%d)",IS_MC_CLOSURE_TEST));
    gROOT->ProcessLine(Form(".! mv WanalysisOnDATA.root ../JobOutputs/%s/WanalysisOnDATA.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp WanalysisOnDATA.\* ../JobOutputs/%s/",outputdir.Data()));
  }

  if(runZanalysisOnDATA){
    gSystem->CompileMacro("ZanalysisOnDATA.C");
    gROOT->ProcessLine(Form("ZanalysisOnDATA zDATA(\"%s\",%.f)",ZfileMC.Data(),ZfileDATA_int_lumi));
    gROOT->ProcessLine(Form("zDATA.Loop(%d)",IS_MC_CLOSURE_TEST));
    gROOT->ProcessLine(Form(".! mv ZanalysisOnDATA.root ../JobOutputs/%s/ZanalysisOnDATA.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp ZanalysisOnDATA.\* ../JobOutputs/%s/",outputdir.Data()));
  }
    
  gSystem->ChangeDirectory("../Fitcode/");
  
  if(run_BuildTemplates){  
    gSystem->CompileMacro("Templates_from_ZanalysisOnDATA.C");
    gROOT->ProcessLine(Form("Templates_from_ZanalysisOnDATA zTEMPLATESOnDATA(\"%s\",\"../JobOutputs/%s/\",%f)",ZfileDATA.Data(),outputdir.Data(),ZfileDATA_int_lumi));
    gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d)",IS_MC_CLOSURE_TEST));
    gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp Templates_from_ZanalysisOnDATA.C ../JobOutputs/%s/",outputdir.Data()));
  }
  
  if(run_MassFit){  
    gROOT->ProcessLine(Form(".x TemplateFit.C(\"../JobOutputs/%s/WanalysisOnMC.root\",\"../JobOutputs/%s/Templates_from_ZanalysisOnDATA.root\",\"../JobOutputs/%s/R_WdivZ_OnMC.root\",%.f,%.f",
                        outputdir.Data(),outputdir.Data(),outputdir.Data(),fitmin,fitmax));
    gROOT->ProcessLine(Form(".! mv FitResults.root ../JobOutputs/%s/FitResults.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp TemplateFit.C ../JobOutputs/%s/",outputdir.Data()));
  }
  
}