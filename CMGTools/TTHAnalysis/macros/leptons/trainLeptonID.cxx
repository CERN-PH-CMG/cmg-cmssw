void trainLeptonID(TString name, TString train="GB") {
    TTree *dSig = (TTree*) _file0->Get("leptonTree");
    TTree *dBg1 = (TTree*) _file1->Get("leptonTree");
    TFile *fOut = new TFile(name+".root","RECREATE");
    TMVA::Factory *factory = new TMVA::Factory(name, fOut, "!V:Color");

    //factory->AddSpectator("pdgId", 'I');
    //factory->AddSpectator("charge", 'I');
    //factory->AddSpectator("mcId", 'I');
    //factory->AddSpectator("pt", 'D');
    //factory->AddSpectator("eta", 'D');

    if (!name.Contains("NoIP")) {
        factory->AddVariable("sip3d", 'D');
        factory->AddVariable("dxy := min(abs(dxy),0.2)", 'D');
        factory->AddVariable("dz  := min(abs(dz),0.3)",  'D');
    }
    factory->AddVariable("relIso", 'D');
    factory->AddVariable("jetPtRatio := min(jetPtRatio,1)", 'D');
    factory->AddVariable("jetBTagCSV := max(jetBTagCSV,0)", 'D');
    //factory->AddVariable("ptRelJet", 'D');
    //factory->AddVariable("jetDR", 'D');
   
    factory->AddSignalTree(dSig, 1.0);
    factory->AddBackgroundTree( dBg1, 1.0);

    if (train=="GB") {
        factory->PrepareTrainingAndTestTree( "mcId > 0", "mcId == 0", "" );
    } else if (train=="PB") {
        factory->PrepareTrainingAndTestTree( "mcId > 1 && pt < 35", "mcId == 0 && pt < 35", "" );
    } else if (train=="pB") {
        factory->PrepareTrainingAndTestTree( "mcId > 0 && sip3d < 3.5 && pt < 35 ", "mcId == 0 && sip3d < 3.5 && pt < 35", "" );
    } else if (train=="tB") {
        factory->PrepareTrainingAndTestTree( "mcId > 0 && sip3d > 3.5 && sip3d < 6.0 && pt < 35 ", "mcId == 0 && sip3d > 3.5 && sip3d < 6.0 && pt < 35", "" );
    } else if (train=="TB") {
        factory->PrepareTrainingAndTestTree( "mcId == 1 && pt < 35", "mcId == 0 && pt < 35", "" );
    } else {
        factory->PrepareTrainingAndTestTree( "mcId > 0", "mcId == 0", "" );
    }

    factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmooth=1:NAvEvtPerBin=50");

    // Linear discriminant (same as Fisher, but also performing regression)
    factory->BookMethod( TMVA::Types::kLD, "LD", "!H:!V:VarTransform=None" );

    // Boosted Decision Trees with bagging
    //factory->BookMethod( TMVA::Types::kLD, "LDB", "!H:!V:Boost_Num=10:Boost_Type=Bagging:Boost_Transform=linear" );
    // Cut optmisation using Genetic Algorithm
    // factory->BookMethod( TMVA::Types::kCuts, "CutsGA", "!H:!V:FitMethod=GA" );

    // Boosted Decision Trees with gradient boosting
    factory->BookMethod( TMVA::Types::kBDT, "BDTG",
            "!H:!V:NTrees=200:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=8" );

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fOut->Close();
}
