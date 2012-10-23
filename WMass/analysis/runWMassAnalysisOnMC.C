
void runWMassAnalysisOnMC(){

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");

  // STEERING PARAMETERS

  // TString jobID= "test_looser_cuts_masscut";
  // TString jobID= "test_2nd_muon_ptcut30";
  // TString jobID= "test_distrib";
  TString jobID= "test_binsize";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLL_3/WJets/WTreeProducer/WTreeProducer_tree_ok.root";
  // TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_4/DYJets/ZTreeProducer/ZTreeProducer_tree_METcontainslepton.root";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGen/WJets/WTreeProducer/WTreeProducer_tree.root";
  // TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGen/DYJets/ZTreeProducer/ZTreeProducer_tree.root";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed/WJets/WTreeProducer/WTreeProducer_tree.root";
  TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGenFixed2mt/WJets/WTreeProducer/WTreeProducer_tree.root";
  TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGenFixed/DYJets/ZTreeProducer/ZTreeProducer_tree.root";
  TString WfileDATA= WfileMC;
  TString ZfileDATA= ZfileMC;
  
  double etamax=2.1; //1.2; // NOT USED AT THE MOMENT
    
  TString ZMass = "91.1876"; // 91.1876
  TString WMassCentral_MeV = "80385"; // 80385
  TString WMassStep_MeV = "100"; // 15
  TString WMassNSteps = "1"; // 60
  TString etaMuonNSteps = "3"; // 5
  TString etaMaxMuons = "0.6, 1.2, 2.1"; // 0.6, 0.8, 1.2, 1.6, 2.1

  bool runWanalysisOnMC  = 0;
  bool runZanalysisOnMC  = 0;
  bool runWZPlotsMC      = 0;
  
  bool runR_WdivZ        = 0;

  bool run_BuildTemplates= 0;
  bool run_MassFit       = 1;
  double fitmin=0.8; //0.75;
  double fitmax=1.2; //1.2;
  
  // END OF STEERING PARAMETERS

  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
  
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
    gSystem->CompileMacro("Wanalysis.C");
    gROOT->ProcessLine(Form("Wanalysis w(\"%s\",%.2f)",WfileMC.Data(),etamax));
    gROOT->ProcessLine("w.Loop()");
    gROOT->ProcessLine(Form(".! mv WanalysisOnMC.root ../JobOutputs/%s/WanalysisOnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp Wanalysis.\* ../JobOutputs/%s/",outputdir.Data()));
  }

  if(runZanalysisOnMC){
    gSystem->CompileMacro("Zanalysis.C");
    gROOT->ProcessLine(Form("Zanalysis z(\"%s\",%.2f)",ZfileMC.Data(),etamax));
    gROOT->ProcessLine("z.Loop()");
    gROOT->ProcessLine(Form(".! mv ZanalysisOnMC.root ../JobOutputs/%s/ZanalysisOnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../JobOutputs/%s/",outputdir.Data()));
  }
  
  if(runWZPlotsMC){
    gSystem->MakeDirectory(Form("../JobOutputs/%s/WMassPlots",outputdir.Data()));
    gROOT->ProcessLine(Form(".x PlotWZdistributions.C(\"../JobOutputs/%s\")",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/WMassPlots",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWZdistributions.C ../JobOutputs/%s/",outputdir.Data()));
  }
  
  if(runR_WdivZ){
    gROOT->ProcessLine(Form(".x R_WdivZ.C(\"../JobOutputs/%s\")",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/R_WdivZ_OnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv Wtemplates.root ../JobOutputs/%s/Wtemplates.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp R_WdivZ.C ../JobOutputs/%s/",outputdir.Data()));
  }

  gSystem->ChangeDirectory("../Fitcode/");
  
  if(run_BuildTemplates){  
    gSystem->CompileMacro("Templates_from_Zanalysis.C");
    gROOT->ProcessLine(Form("Templates_from_Zanalysis z2(\"%s\",\"../JobOutputs/%s/\")",ZfileDATA.Data(),outputdir.Data()));
    gROOT->ProcessLine("z2.Loop()");
    gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp Templates_from_Zanalysis.C ../JobOutputs/%s/",outputdir.Data()));
  }
  
  if(run_MassFit){  
    gROOT->ProcessLine(Form(".x TemplateFit.C(\"../JobOutputs/%s/WanalysisOnMC.root\",\"../JobOutputs/%s/Templates_from_ZanalysisOnMC.root\",%.2f,%.2f",
                        outputdir.Data(),outputdir.Data(),fitmin,fitmax));
    gROOT->ProcessLine(Form(".! mv FitResults.root ../JobOutputs/%s/FitResults.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! cp TemplateFit.C ../JobOutputs/%s/",outputdir.Data()));
  }
  
}