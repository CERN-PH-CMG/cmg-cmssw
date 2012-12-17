void runZanalysis(TString ZfileDATA, double ZfileDATA_lumi_SF, TString sample, int useAlsoGenPforSig, int IS_MC_CLOSURE_TEST, int isMCorDATA, TString filename_outputdir, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePileupSF, int run_BuildEvByEvTemplates)
{

  // gSystem->CompileMacro("MuScleFitCorrector.cc");
  // gSystem->CompileMacro("rochcor_44X_v3.C");
  // gSystem->CompileMacro("rochcor_42X.C");
  // gSystem->CompileMacro("Zanalysis.C","kO-");
  gROOT->ProcessLine(".L rochcor_44X_v3.C+");
  gROOT->ProcessLine(".L Zanalysis.C+");
  cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
  gROOT->ProcessLine(Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample.Contains("DYJetsSig")?useAlsoGenPforSig:0));
  cout << Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()) << endl;
  gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),run_BuildEvByEvTemplates,useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()));

  TString outputfilename = run_BuildEvByEvTemplates ? "WEvByEvTemplates" : "ZanalysisOnDATA";
  gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/%s.root",filename_outputdir.Data(),filename_outputdir.Data(),outputfilename.Data()));
  gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../%s/",filename_outputdir.Data()));
  // // if(IS_MC_CLOSURE_TEST and sample.contains("DYJetsSig")){
    // // cout << Form("making direcory ../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())) << endl;
    // // gSystem->MakeDirectory(Form("../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())));
    // // TString filename_outputdir2 = Form("JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()));
    // // cout << Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
    // // gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
    // // gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/ZanalysisOnDATA.root",filename_outputdir2.Data(),filename_outputdir2.Data()));
  // // }




  // gSystem->CompileMacro("Wanalysis.C");
  // cout << "processing line "<< Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0) << endl;
  // // gROOT->ProcessLine(Form("Wanalysis wDATA(\"%s\",%f,%d); wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0,IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()));
  // gROOT->ProcessLine(Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample.Contains("WJetsSig")?useAlsoGenPforSig:0));
  // cout << "processing line "<< Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()) << endl;
  // gROOT->ProcessLine(Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA,filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample.Data()));
  // gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir.Data(),filename_outputdir.Data()));
  // gROOT->ProcessLine(Form(".! cp Wanalysis.\* ../JobOutputs/%s/",filename_outputdir.Data()));

}
