void trainMVA_2lss(TString name) {
    TString Path = "/afs/cern.ch/user/g/gpetrucc/w/TREES_74X_25ns_ForLepMVATrain";
    //gROOT->ProcessLine(".L ../../../python/plotter/functions.cc+");

    TFile *fOut = new TFile(name+".root","RECREATE");
    TMVA::Factory *factory = new TMVA::Factory(name, fOut, "!V:!Color:Transformations=I");

    TFile *fSig = TFile::Open(Path+"/TTHnobb_pow/treeProducerSusyMultilepton/tree.root");
    TTree *tSig = (TTree *) fSig->Get("tree");
    tSig->AddFriend("sf/t", Path+"/37_gioLepMVATests_v2/evVarFriend_TTHnobb_pow.root");
    tSig->AddFriend("sf/t", Path+"/39_evVars2lss_v0/evVarFriend_TTHnobb_pow.root");
    factory->AddSignalTree(tSig, 1.0);

    TFile *fBkg = TFile::Open(Path+"/TT_pow/treeProducerSusyMultilepton/tree.root");
    TTree *tBkg = (TTree *) fBkg->Get("tree");
    tBkg->AddFriend("sf/t", Path+"/37_gioLepMVATests_v2/evVarFriend_TT_pow.root");
    tBkg->AddFriend("sf/t", Path+"/39_evVars2lss_v0/evVarFriend_TT_pow.root");
    factory->AddBackgroundTree(tBkg, 1.0);

    
    TCut all = "nLepGood >= 2 && LepGood_charge[0] == LepGood_charge[1] && nBJetMedium25 >= 1 && nJet25 >= 3 && LepGood_pt[0] > 20  && LepGood_pt[1] > 10";
                     // && abs(LepGood_pdgId[0]) == 13 && abs(LepGood_pdgId[1]) == 13";
    if (name.Contains("MVAU_M")) {
        all += "LepGood_mvaSusyPHYS14withpt_v2[0] > 0.65 && LepGood_mvaSusyPHYS14withpt_v2[1] > 0.65";
    } else if (name.Contains("MVAU_L")) {
        all += "LepGood_mvaSusyPHYS14withpt_v2[0] > 0.30 && LepGood_mvaSusyPHYS14withpt_v2[1] > 0.30";
    } else if (name.Contains("MVAP_M")) {
        all += "LepGood_mvaSusyPHYS14withpt_v2_w05[0] > 0.4 && LepGood_mvaSusyPHYS14withpt_v2_w05[1] > 0.4";
    } else if (name.Contains("MVAP_L")) {
        all += "LepGood_mvaSusyPHYS14withpt_v2_w05[0] > 0.0 && LepGood_mvaSusyPHYS14withpt_v2_w05[1] > 0.0";
    }

    // Dileptons
    factory->AddVariable("lep2AbsEta := abs(LepGood_eta[1])", 'F');
    factory->AddVariable("lep2Pt := min(LepGood_pt[1], 200)", 'F');
    factory->AddVariable("mhtJet25 := min(mhtJet25, 300)", 'F');
    factory->AddVariable("mindr_lep2_jet", 'F');
    factory->AddVariable("MT_met_lep1", 'F');
    factory->AddVariable("htJet25 := min(htJet25, 1000)", 'F');

    factory->SetWeightExpression("1");
    factory->PrepareTrainingAndTestTree( all, all, "SplitMode=Random" );

    factory->BookMethod( TMVA::Types::kLD, "LD", "!H:!V:VarTransform=None:CreateMVAPdfs" );
    factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", "!H:!V:!TransformOutput" );

    TString BDTGopt = "!H:!V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=5";

    BDTGopt += ":CreateMVAPdfs"; // Create Rarity distribution
    factory->BookMethod( TMVA::Types::kBDT, "BDTG", BDTGopt);

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fOut->Close();
}
