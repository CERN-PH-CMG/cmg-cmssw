void trainLeptonID(TString name, TString train="GB") {
    TTree *dSig = (TTree*) _file0->Get("rec/t");
    TTree *dBg1 = (TTree*) _file1->Get("rec/t");
    TFile *fOut = new TFile(name+".root","RECREATE");
    TMVA::Factory *factory = new TMVA::Factory(name, fOut, "!V:!Color");

    //factory->AddSpectator("charge", 'I');
    //factory->AddSpectator("mcId", 'I');
    //factory->AddSpectator("mva", 'D');
    if (!name.Contains("pteta")) {
        factory->AddSpectator("pt", 'D');
        factory->AddSpectator("abseta := abs(eta)", 'D');
    }

    TString allvars = ""; 
    //factory->AddVariable("relIso", 'D'); allvars += "relIso";
    factory->AddVariable("neuRelIso := relIso - chargedIso/pt", 'D'); allvars += "neuRelIso";
    factory->AddVariable("chRelIso := chargedIso/pt", 'D');        allvars += ":chRelIso";
    factory->AddVariable("jetDR_in := min(dr_in,0.5)", 'D');         allvars += ":jetDR_in";
    factory->AddVariable("jetPtRatio_in := min(ptf_in,1.5)", 'D');   allvars += ":jetPtRatio_in";
    factory->AddVariable("jetBTagCSV_in := max(CSV_in,0)", 'D');   allvars += ":jetBTagCSV_in";
    //factory->AddVariable("jetDR_out := min(dr_out,5)", 'D');       allvars += ":jetDR_out";
    //factory->AddVariable("jetPtRatio_out := min(ptf_out,1.5)", 'D'); allvars += ":jetPtRatio_out";
    //factory->AddVariable("jetBTagCSV_out := max(CSV_out,0)", 'D'); allvars += ":jetBTagCSV_out";


    if (!name.Contains("NoIP")) {
        factory->AddVariable("sip3d", 'D'); 
        factory->AddVariable("dxy := log(abs(dxy))", 'D');
        factory->AddVariable("dz  := log(abs(dz))",  'D');
        allvars += ":sip3d:dxy:dz";
    }

    if (name.Contains("mu")) {
        if (!name.Contains("NoID")) {
            //factory->AddVariable("tightId",'D');    allvars += ":tightId";
            //factory->AddVariable("innerHits",'D');  allvars += ":innerHits";
        }
    } else if (name.Contains("el")) {
        if (!name.Contains("NoID")) {
            factory->AddVariable("mvaId",'D');     allvars += ":mvaId";
            factory->AddVariable("innerHits",'D'); allvars += ":innerHits";
        }
    } else { std::cerr << "ERROR: must either be electron or muon." << std::endl; return; }
   

    TCut lepton = (name.Contains("mu") ? "abs(pdgId) == 13" : "abs(pdgId) == 11");

    if (name.Contains("mu")) {
        if (name.Contains("pteta")) {
            if (name.Contains("low_b"))  lepton += "pt <= 15 && abs(eta) <  1.5";
            if (name.Contains("low_e"))  lepton += "pt <= 15 && abs(eta) >= 1.5";
            if (name.Contains("high_b")) lepton += "pt >  15 && abs(eta) <  1.5";
            if (name.Contains("high_e")) lepton += "pt >  15 && abs(eta) >= 1.5";
        }
    } else if (name.Contains("el")) {
        if (name.Contains("pteta")) {
            if (name.Contains("low_cb"))  lepton += "pt <= 10 && abs(eta) <   0.8";
            if (name.Contains("low_fb"))  lepton += "pt <= 10 && abs(eta) >=  0.8 && abs(eta) < 1.479";
            if (name.Contains("low_ec"))  lepton += "pt <= 10 && abs(eta) >=  1.479";
            if (name.Contains("high_cb")) lepton += "pt >  10 && abs(eta) <   0.8";
            if (name.Contains("high_fb")) lepton += "pt >  10 && abs(eta) >=  0.8 && abs(eta) < 1.479";
            if (name.Contains("high_ec")) lepton += "pt >  10 && abs(eta) >=  1.479";
        }
    }

    double wSig = 1.0, wBkg = 1.0;
    factory->AddSignalTree(dSig, wSig);
    factory->AddBackgroundTree(dBg1, wBkg);

    // re-weighting to approximately match n(jet) multiplicity of signal
    //factory->SetWeightExpression("puWeight*((good>0)+(good<=0)*pow(nJet25,2.36))");
    factory->SetWeightExpression("puWeight");

    if (train=="GB") {
        factory->PrepareTrainingAndTestTree( lepton+" good > 0", lepton+" good <= 0", "" );
    } else if (train=="gb") {
        factory->PrepareTrainingAndTestTree( lepton+" good > 10", lepton+" good <= 0", "" );
    } else if (train=="tB") {
        //factory->PrepareTrainingAndTestTree( lepton+" good > 10 && sip3d < 3.5", lepton+" good == 0 && sip3d < 3.5", "" );
        //factory->PrepareTrainingAndTestTree( lepton+" good > 10 && sip3d > 3.5 && sip3d < 6", lepton+" good <= 0 && sip3d > 3.5 && sip3d < 6", "" );
    } else { 
        std::cerr << "ERROR: No idea of what training you want." << std::endl; return; 
    }

    //factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmooth=1:NAvEvtPerBin=50");

    factory->BookMethod( TMVA::Types::kLD, "LD", "!H:!V:VarTransform=None" );

    // Cut optmisation using Genetic Algorithm
    // factory->BookMethod( TMVA::Types::kCuts, "CutsGA", "!H:!V:FitMethod=GA" );

    // Boosted Decision Trees with gradient boosting
    TString BDTGopt = "!H:!V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=8";
    // BDTGopt = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=2000:PruneMethod=CostComplexity:PruneStrength=50:NNodesMax=5";
    // BDTGopt = "!H:!V:NTrees=6000:BoostType=Grad:Shrinkage=0.50:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=2000:PruneMethod=CostComplexity:PruneStrength=50:NNodesMax=5";
    if (name.Contains("_t1.1")) {
        BDTGopt = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=9:MaxDepth=8";
    } else if (name.Contains("_t1.5")) {
        BDTGopt = "!H:!V:NTrees=2000:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:MaxDepth=5";
    } else if (name.Contains("_t1")) {
        BDTGopt = "!H:!V:NTrees=400:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=9:MaxDepth=8";
    } else if (name.Contains("_t0")) {
        BDTGopt = "!H:!V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=9:MaxDepth=8";
    } else {
        if (name.Contains("mu_pteta_high_b") || name.Contains("el_pteta_high_cb")) {
            // very high stat. use more trees (preset t1.1)
            BDTGopt = "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=9:MaxDepth=8";
        } else if (name.Contains("mu_pteta_low_e") || name.Contains("el_pteta_low")) {
            // very low stat. use less trees (t0) to avoid overtrain
            BDTGopt = "!H:!V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=9:MaxDepth=8";
        } else {
            // default (t1)
            BDTGopt = "!H:!V:NTrees=400:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=200:nEventsMin=100:NNodesMax=9:MaxDepth=8";
        }
    }
    // TString BDTGopt = "!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=8";

    BDTGopt += ":CreateMVAPdfs"; // Create Rarity distribution
    factory->BookMethod( TMVA::Types::kBDT, "BDTG", BDTGopt);

    if (name.Contains("mu") && !name.Contains("pteta")) {
    TMVA::MethodCategory* BDTG_Cat_pteta = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta","" ));
    BDTG_Cat_pteta->AddMethod("pt < 15 && abs(eta) < 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_b",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt < 15 && abs(eta) > 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_e",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt > 15 && abs(eta) < 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_b", BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt > 15 && abs(eta) > 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_e", BDTGopt);

    /*
    TMVA::MethodCategory* BDTG_Cat_pteta2 = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta2","" ));
    BDTG_Cat_pteta2->AddMethod("pt < 15 && abs(eta) < 1.2", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta2_low_b",  BDTGopt);
    BDTG_Cat_pteta2->AddMethod("pt < 15 && abs(eta) > 1.2", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta2_low_e",  BDTGopt);
    BDTG_Cat_pteta2->AddMethod("pt > 15 && abs(eta) < 1.2", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta2_high_b", BDTGopt);
    BDTG_Cat_pteta2->AddMethod("pt > 15 && abs(eta) > 1.2", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta2_high_e", BDTGopt);

    TMVA::MethodCategory* BDTG_Cat_pteta3 = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta3","" ));
    BDTG_Cat_pteta3->AddMethod("pt < 15 && abs(eta) < 1.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta3_low_b",  BDTGopt);
    BDTG_Cat_pteta3->AddMethod("pt < 15 && abs(eta) > 1.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta3_low_e",  BDTGopt);
    BDTG_Cat_pteta3->AddMethod("pt > 15 && abs(eta) < 1.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta3_high_b", BDTGopt);
    BDTG_Cat_pteta3->AddMethod("pt > 15 && abs(eta) > 1.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta3_high_e", BDTGopt);
    */
    } else if (name.Contains("el") && !name.Contains("pteta")){
    TMVA::MethodCategory* BDTG_Cat_pteta = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta","" ));
    BDTG_Cat_pteta->AddMethod("pt <= 10 && abs(eta) <  0.8",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt <= 10 && abs(eta) <  1.479 && abs(eta) >= 0.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_fb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt <= 10 && abs(eta) >= 1.479",                    allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_e", BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt  > 10 && abs(eta) <  0.8",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt  > 10 && abs(eta) <  1.479 && abs(eta) >= 0.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_fb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt  > 10 && abs(eta) >= 1.479",                    allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_e", BDTGopt);

    /*
    */
    }

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fOut->Close();
}
