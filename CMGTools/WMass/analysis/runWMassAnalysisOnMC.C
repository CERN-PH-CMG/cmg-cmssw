
void runWMassAnalysisOnMC(){

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");

  // STEERING PARAMETERS

  TString jobID= "test_masscut50_2pt20";
  // TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLL_3/WJets/WTreeProducer/WTreeProducer_tree_ok.root";
  TString WfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_WJetsToLNu_10wGen/WJets/WTreeProducer/WTreeProducer_tree.root";
  // TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_4/DYJets/ZTreeProducer/ZTreeProducer_tree_METcontainslepton.root";
  TString ZfileMC= "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/batch_DYJetsToLL_10wGen/DYJets/ZTreeProducer/ZTreeProducer_tree.root";
  
  double etamax=2.1; //1.2;
  
  double fitmin=0.8; //0.75;
  double fitmax=1.4; //1.2;
  
  bool runWanalysis     = 1;
  bool runZanalysis     = 1;
  bool runR_WdivZ       = 1;
  bool runR_TemplateFit = 0;
  
  // END OF STEERING PARAMETERS

  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
  
  // RUNNING CODE

  TString outputdir;
  if(jobID.Data()=="") outputdir="dummytest";
  else outputdir=jobID.Data();
  
  // outputdir+=Form("_etamax%.1f",etamax);
  
  cout << gSystem->WorkingDirectory() << endl;
  cout << "making directory "<<Form("JobOutputs/%s",outputdir.Data()) << endl;
  gSystem->MakeDirectory(Form("JobOutputs/%s",outputdir.Data()));
  gSystem->ChangeDirectory("MCcode/");

  if(runWanalysis){
    gSystem->CompileMacro("Wanalysis.C");
    gROOT->ProcessLine(Form("Wanalysis w(\"%s\",%.2f)",WfileMC.Data(),etamax));
    gROOT->ProcessLine("w.Loop()");
    gROOT->ProcessLine(Form(".! mv WanalysisOnMC.root ../JobOutputs/%s/WanalysisOnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv Wanalysis.\* ../JobOutputs/%s/",outputdir.Data()));
  }

  if(runZanalysis){
    gSystem->CompileMacro("Zanalysis.C");
    gROOT->ProcessLine(Form("Zanalysis z(\"%s\",%.2f)",ZfileMC.Data(),etamax));
    gROOT->ProcessLine("z.Loop()");
    gROOT->ProcessLine(Form(".! mv ZanalysisOnMC.root ../JobOutputs/%s/ZanalysisOnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv Zanalysis.\* ../JobOutputs/%s/",outputdir.Data()));
  }
  
  if(runR_WdivZ){
    gROOT->ProcessLine(Form(".x R_WdivZ.C(\"../JobOutputs/%s\")",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/R_WdivZ_OnMC.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv Wtemplates.root ../JobOutputs/%s/Wtemplates.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv R_WdivZ.C ../JobOutputs/%s/",outputdir.Data()));
  }

  if(runR_TemplateFit){  
    gSystem->ChangeDirectory("../Fitcode/");
    gROOT->ProcessLine(Form(".x TemplateFit.C(\"../JobOutputs/%s/WanalysisOnMC.root\",\"../JobOutputs/%s/Wtemplates.root\",%.2f,%.2f)",
                        outputdir.Data(),outputdir.Data(),fitmin,fitmax));
    gROOT->ProcessLine(Form(".! mv FitResults.root ../JobOutputs/%s/FitResults.root",outputdir.Data()));
  }
  
}