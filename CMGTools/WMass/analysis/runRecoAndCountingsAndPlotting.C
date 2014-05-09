#include <iostream>
#include <fstream>

void runRecoAndCountingsAndPlotting(){

  gROOT->SetBatch();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);

  // ============================================================== //
  // ============================================================== //
  // ============================================================== //
  // STEERING PARAMETERS
  
  // TString foldername = "RecoCountingPlotting_test44X_testRecoil";
  // TString foldername = "RecoCountingPlotting_test44X_tightMuNeg";
  // TString foldername = "results_test44X_bugfix_Run2011A";
  // TString foldername = "results_test44X_testtempl";
  TString foldername = "results_test44X_testpython";
  // TString foldername = "RecoCountingPlotting_test44X_newPileupSF_gen";
  // TString foldername = "results_test44X_testnpu";
  
  int usePileupSF = 1; // 0=no, 1=yes
  int useEffSF = 1; // 0=no, 1=yes
  int useMomentumCorr = 1; // 0=none, 1=Rochester, 2=MuscleFit
  int smearRochCorrByNsigma = 0;
  // CHOOSE WETHER IS MC CLOSURE OR NOT (half statistics used as DATA, half as MC)
  bool IS_MC_CLOSURE_TEST= 0; 
  bool normalize_lumi_MC_CLOSURE_TEST = 1;
  double intLumi_MC_CLOSURE_TEST_fb = 5.1;// data = 4.7499 prescaled trigger, 5.1 unprescaled; works only if normalize_lumi_MC_CLOSURE_TEST is TRUE
  bool useAlsoGenPforSig= 0;
        
  TString ZMass = "91.1876"; // 91.1876
  TString WMassCentral_MeV = "80385"; // 80385
  TString WMassStep_MeV = "50"; // 15
  TString WMassNSteps = "10"; // 60
  TString etaMuonNSteps = "1"; // 5
  // TString etaMaxMuons = "0.6"; // 0.6, 0.8, 1.2, 1.6, 2.1
  TString etaMaxMuons = "0.6"; // 0.6, 0.8, 1.2, 1.6, 2.1

  bool runWanalysis = 0;
  bool runZanalysis = 0;
  
  bool mergeEWKbkg   = 0;

  bool ExtractNumbers = 0;
  
  bool runWSigBkgFit = 0;
  bool runZSigBkgFit = 0;
  
  bool run_Z_MCandDATAcomparisons_stack = 0;
  bool run_W_MCandDATAcomparisons_stack = 0;
  bool run_Z_MCandDATAcomparison = 0;
  bool run_W_MCandDATAcomparison = 0;

  bool run_ZvsWlike_comparisonMC   = 0;
  bool run_ZvsWlike_comparisonDATA = 0;

  bool runWandZcomparisonMC   = 0;
  bool runWandZcomparisonDATA = 0;
  
  
  // PRODUCE R(X)=W/Z DISTRIBUTION TO REWEIGHT Z in DATA
  bool runR_WdivZ= 0;
  // PRODUCE TEMPLATES, i.e. Z(DATA)*R(X)
  bool run_BuildSimpleTemplates= 0;
  bool run_BuildEvByEvTemplates= 0;
  
  // PERFORM W MASS FIT
  bool runPrepareDataCards     = 1;
  bool run_MassFit       = 0;
  int fitType            = 0; // 0 = ROOT, 1 = CUSTOM

  // END STEERING PARAMETERS
  // ============================================================== //
  // ============================================================== //
  // ============================================================== //
  
  if(IS_MC_CLOSURE_TEST==1){ 
    foldername+="_MCclosureTest";
    useEffSF=0;
    usePileupSF=0;
  }
  if(useMomentumCorr==1) foldername+="_RochCorr";
  else if(useMomentumCorr==2) foldername+="_MuscleFitCorr";
  if(useEffSF==1) foldername+="_EffSFCorr";
  if(usePileupSF==1) foldername+="_PileupSFCorr";
  static const int nsamples=10;
  enum                            {  WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD     ,  DATA  };
  TString     sample[nsamples]  = { "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"    , "DATA" };
  int      isMCorDATA[nsamples] = {        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,   0      ,    1   };
  double       CS_pb[nsamples]  = {    31314  ,      31314 ,   2895.6   ,      2895.6 ,   157.51 ,   0.119  ,   4.514  ,   0.596 , 84679.3  ,   -1   };
                                                                                                                                              // Run2011A  Run2011B  JSON  TRIGGER GOOD VTX
  // double       Nevts[nsamples]  = { 81053834  ,   81053834 ,  36209629  ,    36209629 , 59136395 , 1103468  , 1197558  , 1221134 , 24958039 , (50367238+91137428)*0.8894*0.3991*0.9996   }; // 42X
  double       Nevts[nsamples]  = { 81293448  ,   81293448 ,  36225556  ,    36225556 , 59314581 , 1103468  , 1197558  , 1221134 , 24958039 ,  53056567/* (50367238+91137428)*0.8894*0.4217*0.9996 */   }; // 44X
  double int_lumi_fb[nsamples]  = {        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,        0 ,   5.1  /* 4.7499 */};
  double Nevts_Per_fb[nsamples] = {        0  ,          0 ,        0   ,           0 ,        0 ,       0  ,       0  ,       0 ,   0      ,    0   };

  TString common_folder = "/afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/cfg/";

  TString fWana_str[nsamples] = {
    common_folder+"batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree_SignalRecoSkimmed.root",
    common_folder+"batch_W_Trg/WJets/WTreeProducer/WTreeProducer_tree_FakeRecoSkimmed.root",
    common_folder+"batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree_SignalRecoSkimmed.root",
    common_folder+"batch_DY_Trg/DYJets/WTreeProducer/WTreeProducer_tree_FakeRecoSkimmed.root",
    common_folder+"batch_TT_Trg/TTJets/WTreeProducer/WTreeProducer_tree.root",
    common_folder+"batch_VV_Trg/ZZJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
    common_folder+"batch_VV_Trg/WWJetsTo2L2Nu/WTreeProducer/WTreeProducer_tree.root",
    common_folder+"batch_VV_Trg/WZJetsTo3LNu/WTreeProducer/WTreeProducer_tree.root",
    common_folder+"batch_QCD_Trg/QCD15/WTreeProducer/WTreeProducer_tree.root",
    common_folder+"batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/WTreeProducer/WTreeProducer_tree.root"
  };  
  TString fZana_str[nsamples] = {
    common_folder+"batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree_SignalRecoSkimmed.root",
    common_folder+"batch_W_Trg/WJets/ZTreeProducer/ZTreeProducer_tree_FakeRecoSkimmed.root",
    common_folder+"batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree_SignalRecoSkimmed.root",
    common_folder+"batch_DY_Trg/DYJets/ZTreeProducer/ZTreeProducer_tree_FakeRecoSkimmed.root",
    common_folder+"batch_TT_Trg/TTJets/ZTreeProducer/ZTreeProducer_tree.root",
    common_folder+"batch_VV_Trg/ZZJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
    common_folder+"batch_VV_Trg/WWJetsTo2L2Nu/ZTreeProducer/ZTreeProducer_tree.root",
    common_folder+"batch_VV_Trg/WZJetsTo3LNu/ZTreeProducer/ZTreeProducer_tree.root",
    common_folder+"batch_QCD_Trg/QCD15/ZTreeProducer/ZTreeProducer_tree.root",
    common_folder+"batch_DATA_Trg/data_Run2011AB_Nov8Nov19ReReco_v1/ZTreeProducer/ZTreeProducer_tree.root"
  };
  
  cout << gSystem->WorkingDirectory() << endl;
  cout << Form(".! cp runRecoAndCountingsAndPlotting.C JobOutputs/%s/",foldername.Data()) << endl;
  gROOT->ProcessLine(Form(".! cp runRecoAndCountingsAndPlotting.C JobOutputs/%s/",foldername.Data()));

  if(runWanalysis || runZanalysis || ExtractNumbers || run_BuildSimpleTemplates){
    for(int i=0;i<nsamples;i++){
    // for(int i=8;i<9;i++){
    
      // if(i<8) continue; // TEMPORARY
      // if(sample[i]!="DATA") continue; // TEMPORARY
      // cout << sample[i] << " " << IS_MC_CLOSURE_TEST << " " << (sample[i]!="WJetsSig") << " " << (sample[i]!="DYJetsSig") << endl;
      if(IS_MC_CLOSURE_TEST && (sample[i]!="WJetsSig" && sample[i]!="DYJetsSig")) continue; // TEMPORARY
      TString jobID= Form("test_numbers_%s",sample[i].Data());
      
      cout << endl;
      cout << "ANALYZING jobID= " << jobID << endl;
      
      TString WfileDATA= fWana_str[i];
      TString ZfileDATA= fZana_str[i];
      if(sample[i].Contains("WJetsSig") && useAlsoGenPforSig) WfileDATA.ReplaceAll("_SignalRecoSkimmed","");
      else if(sample[i].Contains("DYJetsSig") && useAlsoGenPforSig) ZfileDATA.ReplaceAll("_SignalRecoSkimmed","");

      int tot_N_evts = Nevts[i];

      if(CS_pb[i] >0) int_lumi_fb[i] = Nevts[i]/CS_pb[i]/1e3;
      // Nevts_Per_fb[i] = Nevts[i]/int_lumi_fb[i];
      double WfileDATA_lumi_SF = IS_MC_CLOSURE_TEST ? (normalize_lumi_MC_CLOSURE_TEST ? intLumi_MC_CLOSURE_TEST_fb/int_lumi_fb[i] : 1) : int_lumi_fb[DATA]/int_lumi_fb[i];
      double ZfileDATA_lumi_SF = IS_MC_CLOSURE_TEST ? (normalize_lumi_MC_CLOSURE_TEST ? intLumi_MC_CLOSURE_TEST_fb/int_lumi_fb[i] : 1) : int_lumi_fb[DATA]/int_lumi_fb[i];
      cout << "lumi_SF= " << Form("%e",ZfileDATA_lumi_SF) << endl;
      // END OF STEERING PARAMETERS

      ///////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////
      
      // RUNNING CODE

      TString outputdir;
      if(jobID.Data()=="") outputdir="dummytest";
      else outputdir=jobID.Data();
      TString filename_outputdir = Form("JobOutputs/%s/%s",foldername.Data(),outputdir.Data());
      
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

      gSystem->ChangeDirectory("AnalysisCode/");
      
      if(runWanalysis){
        // cout << "IS_MC_CLOSURE_TEST "<<IS_MC_CLOSURE_TEST << " isMCorDATA " << isMCorDATA[i] << " useMomentumCorr " << useMomentumCorr << " smearRochCorrByNsigma " << smearRochCorrByNsigma << endl;
        // gSystem->CompileMacro("MuScleFitCorrector.C");
        gROOT->ProcessLine(".! touch *.*");
        gSystem->CompileMacro("rochcor_44X_v3.C");
        // gSystem->CompileMacro("rochcor_42X.C");
        gSystem->CompileMacro("Wanalysis.C");
        cout << "processing line "<< Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample[i].Contains("WJetsSig")?useAlsoGenPforSig:0) << endl;
        gROOT->ProcessLine(Form("Wanalysis wDATA(\"%s\",%f,%d)",WfileDATA.Data(),WfileDATA_lumi_SF,sample[i].Contains("WJetsSig")?useAlsoGenPforSig:0));
        cout << "processing line "<< Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[i].Data()) << endl;
        gROOT->ProcessLine(Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[i].Data()));
        gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir.Data(),filename_outputdir.Data()));
        gROOT->ProcessLine(Form(".! cp Wanalysis.\* ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
        if(IS_MC_CLOSURE_TEST && sample[i].Contains("WJetsSig")){
          cout << Form("making direcory ../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())) << endl;
          gSystem->MakeDirectory(Form("../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())));
          TString filename_outputdir2 = Form("JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()));
          cout << Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
          gROOT->ProcessLine(Form("wDATA.Loop(%d,%d,\"../%s\",%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
          gROOT->ProcessLine(Form(".! mv ../%s/Wanalysis.root ../%s/WanalysisOnDATA.root",filename_outputdir2.Data(),filename_outputdir2.Data()));
        }
      }

      if(runZanalysis){
        gROOT->ProcessLine(".! touch *.*");
        // gSystem->CompileMacro("MuScleFitCorrector.cc");
        gSystem->CompileMacro("rochcor_44X_v3.C");
        // gSystem->CompileMacro("rochcor_42X.C");
        gSystem->CompileMacro("Zanalysis.C");
        cout << "processing line "<< Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample[i].Contains("DYJetsSig")?useAlsoGenPforSig:0) << endl;
        gROOT->ProcessLine(Form("Zanalysis zDATA(\"%s\",%f,%d)",ZfileDATA.Data(),ZfileDATA_lumi_SF,sample[i].Contains("DYJetsSig")?useAlsoGenPforSig:0));
        cout << Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[i].Data()) << endl;
        gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[i].Data()));
        gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/ZanalysisOnDATA.root",filename_outputdir.Data(),filename_outputdir.Data()));
        gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
        if(IS_MC_CLOSURE_TEST && sample[i].Contains("DYJetsSig")){
          cout << Form("making direcory ../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())) << endl;
          gSystem->MakeDirectory(Form("../JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data())));
          TString filename_outputdir2 = Form("JobOutputs/%s/%s",foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()));
          cout << Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
          gROOT->ProcessLine(Form("zDATA.Loop(%d,%d,\"../%s\",0,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,1,filename_outputdir2.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
          gROOT->ProcessLine(Form(".! mv ../%s/Zanalysis.root ../%s/ZanalysisOnDATA.root",filename_outputdir2.Data(),filename_outputdir2.Data()));
        }
      }
      
      // gSystem->ChangeDirectory("../MCcode/");
      
      if(ExtractNumbers){
        gROOT->ProcessLine(".! touch *.*");
        gROOT->ProcessLine(Form(".! root -l -b -q \'R_WdivZ.C(\"../JobOutputs/%s/%s\",1,%d)\' > numbers.txt",foldername.Data(),outputdir.Data(),tot_N_evts));
        // gROOT->ProcessLine(Form(".! root -l -b -q \'R_WdivZ.C(\"../JobOutputs/%s/%s\",0,%d)\'",foldername.Data(),outputdir.Data(),tot_N_evts));
        // gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/%s/R_WdivZ_OnMC.root",foldername.Data(),outputdir.Data()));
        gROOT->ProcessLine(Form(".! mv \*.txt ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
        // gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/%s/R_WdivZ_OnMC.root",foldername.Data(),outputdir.Data()));
        // gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
        gROOT->ProcessLine(Form(".! cp R_WdivZ.C ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
      }

      if(run_BuildSimpleTemplates){
        gROOT->ProcessLine(".! touch *.*");
        cout << "running " << Form(".! root -l -b -q \'BuildSimpleTemplates.C(\"../JobOutputs/%s/%s\")\'",foldername.Data(),outputdir.Data()) << endl;
        gROOT->ProcessLine(Form(".! root -l -b -q \'BuildSimpleTemplates.C(\"../JobOutputs/%s/%s\")\'",foldername.Data(),outputdir.Data()));
        // gROOT->ProcessLine(Form(".! mv \*.txt ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
        // gROOT->ProcessLine(Form(".! cp R_WdivZ.C ../JobOutputs/%s/%s/",foldername.Data(),outputdir.Data()));
      }
      
      gSystem->ChangeDirectory("../");
      
    }
  }
  
  gSystem->ChangeDirectory("AnalysisCode/");
  
  if(runR_WdivZ){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("../MCcode/");
    gROOT->ProcessLine(Form(".x R_WdivZ.C(\"../JobOutputs/%s\")",foldername.Data()));
    gROOT->ProcessLine(Form(".! mv R_WdivZ_OnMC.root ../JobOutputs/%s/R_WdivZ_OnMC.root",foldername.Data()));
    gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp R_WdivZ.C ../JobOutputs/%s/",foldername.Data()));
    // gSystem->ChangeDirectory("../AnalysisCode/");
  }

  if(run_BuildEvByEvTemplates){  
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->CompileMacro("Templates_from_ZanalysisOnDATA.C");
    // gROOT->ProcessLine(Form("Templates_from_ZanalysisOnDATA zTEMPLATESOnDATA(\"%s\",\"../JobOutputs/%s/\",%f)",ZfileDATA.Data(),outputdir.Data(),ZfileDATA_lumi_SF));
    // gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d)",IS_MC_CLOSURE_TEST));
    // // gROOT->ProcessLine(Form(".! mv \*.root ../JobOutputs/%s/",outputdir.Data()));
    // gROOT->ProcessLine(Form(".! cp Templates_from_ZanalysisOnDATA.C ../JobOutputs/%s/",outputdir.Data()));
    gSystem->CompileMacro("rochcor_44X_v3.C");
    // gSystem->CompileMacro("rochcor_42X.C");
    // gSystem->CompileMacro("MuScleFitCorrector.cc");
    gSystem->CompileMacro("Zanalysis.C");
    // cout << "int_lumi_fb[IS_MC_CLOSURE_TEST? DYJetsSig : DATA]= "<<(IS_MC_CLOSURE_TEST? intLumi_MC_CLOSURE_TEST_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1) << endl;
    cout << Form("Zanalysis zTEMPLATESOnDATA(\"%s\",%.f)",fZana_str[IS_MC_CLOSURE_TEST? DYJetsSig : DATA].Data(),IS_MC_CLOSURE_TEST? intLumi_MC_CLOSURE_TEST_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1) << endl;
    gROOT->ProcessLine(Form("Zanalysis zTEMPLATESOnDATA(\"%s\",%.f)",fZana_str[IS_MC_CLOSURE_TEST? DYJetsSig : DATA].Data(),IS_MC_CLOSURE_TEST? intLumi_MC_CLOSURE_TEST_fb/(Nevts[DYJetsSig]/CS_pb[DYJetsSig]/1e3) : 1));
    // gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d,1,%s,1)",IS_MC_CLOSURE_TEST,filename_outputdir.Data()));
    cout << "processing line " << Form("zTEMPLATESOnDATA.Loop(%d,1,\"../JobOutputs/%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,foldername.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()) << endl;
    gROOT->ProcessLine(Form("zTEMPLATESOnDATA.Loop(%d,1,\"../JobOutputs/%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,foldername.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[DATA].Data()));
                                            // // ".Loop(%d,%d,\"../%s\",1,%d,%d,%d,%d,\"%s\")",IS_MC_CLOSURE_TEST,isMCorDATA[i],filename_outputdir.Data(),useMomentumCorr,smearRochCorrByNsigma,useEffSF,usePileupSF,sample[i].Data()));
    // // gROOT->ProcessLine(Form(".! mv Zanalysis.root ../JobOutputs/%s/ZanalysisOnDATA.root",outputdir.Data()));
    gROOT->ProcessLine(Form(".! mv ../JobOutputs/%s/Zanalysis.root ../JobOutputs/%s/Templates_from_ZanalysisOnDATA.root",foldername.Data(),foldername.Data()));
    gROOT->ProcessLine(Form(".! cp Zanalysis.\* ../JobOutputs/%s/",foldername.Data()));
  }
  
  if(run_MassFit){  
    gROOT->ProcessLine(".! touch *.*");
    gROOT->ProcessLine(Form(".x TemplateFit.C(\"../JobOutputs/%s/%s/WanalysisOnDATA.root\",\"../JobOutputs/%s/Templates_from_ZanalysisOnDATA.root\",\"../JobOutputs/%s/R_WdivZ_OnMC.root\",%d",
                        foldername.Data(),Form("test_numbers_%s",sample[DATA].Data()),foldername.Data(),foldername.Data(),fitType));
    gROOT->ProcessLine(Form(".! mv FitResults.root ../JobOutputs/%s/FitResults%s.root",foldername.Data(),fitType==0?"_ROOTchi2Fit":"_CustomChi2Fit"));
    gROOT->ProcessLine(Form(".! cp TemplateFit.C ../JobOutputs/%s/",foldername.Data()));
  }
  
  gSystem->ChangeDirectory("../PlottingCode");

  if(run_ZvsWlike_comparisonMC){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DYJetsSig].Data());
    gROOT->ProcessLine(Form(".x PlotZvsWlikeDistributionsDATA.C(\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZvsWlikeDistributionsDATA.C ../JobOutputs/%s/MC_ZandWlikeComparisonPlots",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  if(run_ZvsWlike_comparisonDATA){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotZvsWlikeDistributionsDATA.C(\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZvsWlikeDistributionsDATA.C ../JobOutputs/%s/DATA_ZandWlikeComparisonPlots",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  
  if(runWandZcomparisonMC){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DYJetsSig].Data());
    TString jobIDW= Form("test_numbers_%s",sample[WJetsSig].Data());
    gROOT->ProcessLine(Form(".x PlotWvsZdistributionsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data(),foldername.Data(),jobIDW.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  
  if(runWandZcomparisonDATA){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    TString jobIDZ= Form("test_numbers_%s",sample[DATA].Data());
    TString jobIDW= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotWvsZdistributionsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDZ.Data(),foldername.Data(),jobIDW.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/DATA_WandZcomparisonPlots",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  
  if(run_Z_MCandDATAcomparison){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    TString jobIDMC= Form("test_numbers_%s",sample[DYJetsSig].Data());
    TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotZdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZdistributionsMCvsDATA.C ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  if(run_Z_MCandDATAcomparisons_stack){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    TString jobIDMCsig= Form("../JobOutputs/%s/test_numbers_%s",foldername.Data(),sample[DYJetsSig].Data());
    TString jobIDMCEWK= Form("../JobOutputs/%s/test_numbers_EWK",foldername.Data());
    TString jobIDMCTT= Form("../JobOutputs/%s/test_numbers_%s",foldername.Data(),sample[TTJets].Data());
    TString jobIDMCQCD= Form("../JobOutputs/%s/test_numbers_%s",foldername.Data(),sample[QCD].Data());
    TString jobIDDATA= Form("../JobOutputs/%s/test_numbers_%s",foldername.Data(),sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotZdistributionsMCvsDATA_stack.C(\"%s/\",\"%s/\",\"%s/\",\"%s/\",\"%s/\"",jobIDMCsig.Data(),jobIDMCEWK.Data(),jobIDMCTT.Data(),jobIDMCQCD.Data(),jobIDDATA.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotZdistributionsMCvsDATA_stack.C ../JobOutputs/%s/ZcomparisonPlots_MCvsDATA",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  
  if(run_W_MCandDATAcomparison){
    gROOT->ProcessLine(".! touch *.*");
    // gSystem->ChangeDirectory("DATAcode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/WcomparisonPlots_MCvsDATA",foldername.Data()));
    TString jobIDMC= Form("test_numbers_%s",sample[WJetsSig].Data());
    TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
    gROOT->ProcessLine(Form(".x PlotWdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
    gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/WcomparisonPlots_MCvsDATA",foldername.Data()));
    gROOT->ProcessLine(Form(".! cp PlotWdistributionsMCvsDATA.C ../JobOutputs/%s/WcomparisonPlots_MCvsDATA",foldername.Data()));
    // gSystem->ChangeDirectory("../");
  }
  
  if(mergeEWKbkg){  
    gROOT->ProcessLine(".! touch *.*");
    gSystem->ChangeDirectory("../utils/");
    // cout << Form("../JobOutputs/%s/test_numbers_EWK/",foldername.Data()) << endl;
    gSystem->MakeDirectory(Form("../JobOutputs/%s/test_numbers_EWK/",foldername.Data()));
    cout << gSystem->WorkingDirectory() << endl;
    gROOT->ProcessLine(Form(".! sh merge_MC.sh \"../JobOutputs/%s/\" ",foldername.Data()));
    
    // // gSystem->ChangeDirectory("../DATAcode/");
    // gSystem->MakeDirectory(Form("../JobOutputs/%s/ZcomparisonPlots_EWKvsDATA",foldername.Data()));
    // TString jobIDMC= Form("test_numbers_EWK");
    // TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
    // gROOT->ProcessLine(Form(".x PlotZdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
    // gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/ZcomparisonPlots_EWKvsDATA",foldername.Data()));
    // gROOT->ProcessLine(Form(".! cp PlotZdistributionsMCvsDATA.C ../JobOutputs/%s/ZcomparisonPlots_EWKvsDATA",foldername.Data()));
    // gSystem->MakeDirectory(Form("../JobOutputs/%s/WcomparisonPlots_EWKvsDATA",foldername.Data()));
    // TString jobIDMC= Form("test_numbers_EWK");
    // TString jobIDDATA= Form("test_numbers_%s",sample[DATA].Data());
    // gROOT->ProcessLine(Form(".x PlotWdistributionsMCvsDATA.C(\"../JobOutputs/%s/%s/\",\"../JobOutputs/%s/%s/\"",foldername.Data(),jobIDMC.Data(),foldername.Data(),jobIDDATA.Data()));
    // gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/WcomparisonPlots_EWKvsDATA",foldername.Data()));
    // gROOT->ProcessLine(Form(".! cp PlotWdistributionsMCvsDATA.C ../JobOutputs/%s/WcomparisonPlots_EWKvsDATA",foldername.Data()));
    // // gSystem->ChangeDirectory("../");
  
  }
  
  if(runWSigBkgFit){
    gROOT->ProcessLine(".! touch *.*");
    gSystem->ChangeDirectory("../SignalExtraction/");  
    gROOT->ProcessLine(Form(".! source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitWm.C+\\\(\\\"%s\\\"\\\)",foldername.Data()));
    gSystem->ChangeDirectory("../");  
  
  }
  if(runZSigBkgFit){
    gROOT->ProcessLine(".! touch *.*");
    gSystem->ChangeDirectory("../SignalExtraction/");  
    gROOT->ProcessLine(Form(".! source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.28.00h/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh; root -l -b -q rootlogon.C fitZmm.C+\\\(\\\"%s\\\"\\\)",foldername.Data()));

    gSystem->ChangeDirectory("../");  
  
  }

  if(runPrepareDataCards){
    gROOT->ProcessLine(".! touch *.*");
    // cout << gSystem->WorkingDirectory() << endl;
    gSystem->ChangeDirectory("../AnalysisCode/");
    gSystem->MakeDirectory(Form("../JobOutputs/%s/DataCards",foldername.Data()));
    TString samples_datacards;
    // for(int i=0;i<nsamples;i++){
      // samples_datacards+=sample[i];
      // if(i<nsamples-1) samples_datacards+="-";
    // }
    cout << "running " << Form(".x prepareDatacards.C(\"../JobOutputs/%s\",\"%s\")",foldername.Data(),samples_datacards.Data()) << endl;
    cout << endl;
    gROOT->ProcessLine(Form(".x prepareDatacards.C(\"../JobOutputs/%s\",\"%s\")",foldername.Data(),samples_datacards.Data()));
    // gROOT->ProcessLine(Form(".! mv \*.png ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    // gROOT->ProcessLine(Form(".! cp PlotWvsZdistributionsDATA.C ../JobOutputs/%s/MC_WandZcomparisonPlots",foldername.Data()));
    gSystem->ChangeDirectory("../");  
  
  }
  
}
