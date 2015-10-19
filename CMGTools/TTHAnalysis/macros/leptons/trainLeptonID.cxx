void trainLeptonID(TString name, TString train="GoodvsBad") {
    TTree *dSig = (TTree*) _file0->Get("tree");
    TTree *dBg1 = (TTree*) _file1->Get("tree");
    TFile *fOut = new TFile(name+".root","RECREATE");
    TMVA::Factory *factory = new TMVA::Factory(name, fOut, "!V:!Color:Transformations=I");
    
    if (name.Contains("withpt")) {
        factory->AddVariable("LepGood_pt", 'D');
    }

    factory->AddVariable("LepGood_miniRelIsoCharged", 'D');
    factory->AddVariable("LepGood_miniRelIsoNeutral", 'D');
    if (name.Contains("v2")) {
        factory->AddVariable("LepGood_jetPtRelv2", 'D');
        factory->AddVariable("LepGood_jetPtRatio := min(LepGood_jetPtRatio_LepAwareJECv2,1.5)", 'D');
    } else {
        factory->AddVariable("LepGood_jetPtRel", 'D');
        factory->AddVariable("LepGood_jetPtRatio := min(LepGood_jetPtRatio,1.5)", 'D');
    }
    factory->AddVariable("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", 'D');


    if (!name.Contains("NoIP")) {
        factory->AddVariable("LepGood_sip3d", 'D'); 
        factory->AddVariable("LepGood_dxy := log(abs(LepGood_dxy))", 'D');
        factory->AddVariable("LepGood_dz  := log(abs(LepGood_dz))",  'D');
    }

    if (name.Contains("mu")) {
        if (!name.Contains("NoID")) {
	  //factory->AddVariable("LepGood_mvaId",'D');
            factory->AddVariable("LepGood_segmentCompatibility",'D');
        }
    } else if (name.Contains("el")) {
      //electron variables
        if (!name.Contains("NoID")) {
            factory->AddVariable("LepGood_mvaIdPhys14",'D');
        }
    } else { std::cerr << "ERROR: must either be electron or muon." << std::endl; return; }

    // if (name.Contains("FR")) {
    //   factory->AddVariable("LepGood_jetNDau := min(max(LepGood_jetNDau,-1),150)", 'D');
    //   factory->AddVariable("LepGood_jetNDauCharged := min(max(LepGood_jetNDauCharged,-1),150)", 'D');
    //   factory->AddVariable("LepGood_jetNDauPV := min(max(LepGood_jetNDauPV,-1),150)", 'D');
    //   factory->AddVariable("LepGood_jetNDauNotPV := min(max(LepGood_jetNDauNotPV,-1),150)", 'D');
    //   factory->AddVariable("LepGood_jetmaxSignedSip3D := min(LepGood_jetmaxSignedSip3D,0.5)", 'D');
    //   factory->AddVariable("LepGood_jetmaxSip3D := min(LepGood_jetmaxSip3D,0.5)", 'D');
    //   factory->AddVariable("LepGood_jetmaxSignedSip2D := min(LepGood_jetmaxSignedSip2D,0.5)", 'D');
    //   factory->AddVariable("LepGood_jetmaxSip2D := min(LepGood_jetmaxSip2D,0.5)", 'D');
   
    // }

    TCut lepton = "LepGood_miniRelIso < 0.4 && LepGood_sip3d < 8";

    if (name.Contains("mu")) {
        lepton += "abs(LepGood_pdgId) == 13";
        if (name.Contains("pteta")) {
            if (name.Contains("low"))  lepton += "LepGood_pt <= 10";
	    if (name.Contains("medium_b"))  lepton += "LepGood_pt > 10 && LepGood_pt <= 25 && abs(LepGood_eta) <  1.5";
            if (name.Contains("medium_e"))  lepton += "LepGood_pt > 10 && LepGood_pt <= 25 && abs(LepGood_eta) >= 1.5";
            if (name.Contains("high_b")) lepton += "LepGood_pt > 25 && abs(LepGood_eta) <  1.5";
            if (name.Contains("high_e")) lepton += "LepGood_pt > 25 && abs(LepGood_eta) >= 1.5";
        } else if (name.Contains("eta")) {
	    if (name.Contains("eta_b")) lepton += "abs(LepGood_eta) <  1.5";
            if (name.Contains("eta_e")) lepton += "abs(LepGood_eta) >= 1.5";
        }

    } else if (name.Contains("el")) {
        lepton += "abs(LepGood_pdgId) == 11";
        if (name.Contains("pteta")) {
            if (name.Contains("low"))  lepton += "LepGood_pt <= 10";
            if (name.Contains("medium_cb")) lepton += "LepGood_pt >  10 && LepGood_pt <= 25 && abs(LepGood_eta) <   0.8";
            if (name.Contains("medium_fb")) lepton += "LepGood_pt >  10 && LepGood_pt <= 25 && abs(LepGood_eta) >=  0.8 && abs(LepGood_eta) < 1.479";
            if (name.Contains("medium_ec")) lepton += "LepGood_pt >  10 && LepGood_pt <= 25 && abs(LepGood_eta) >=  1.479";
	    if (name.Contains("high_cb")) lepton += "LepGood_pt >  25 && abs(LepGood_eta) <   0.8";
            if (name.Contains("high_fb")) lepton += "LepGood_pt >  25 && abs(LepGood_eta) >=  0.8 && abs(LepGood_eta) < 1.479";
            if (name.Contains("high_ec")) lepton += "LepGood_pt >  25 && abs(LepGood_eta) >=  1.479";
        } else if (name.Contains("eta")) {
	    if (name.Contains("eta_cb")) lepton += "abs(LepGood_eta) <  0.8";
	    if (name.Contains("eta_fb")) lepton += "abs(LepGood_eta) >= 0.8 && abs(LepGood_eta) <  1.479";
            if (name.Contains("eta_ec")) lepton += "abs(LepGood_eta) >= 1.479";
        }

    }

    double wSig = 1.0, wBkg = 1.0;
    factory->AddSignalTree(dSig, wSig);
    factory->AddBackgroundTree(dBg1, wBkg);

    if (name.Contains("w05")) {
        factory->SetWeightExpression("exp(min(max(LepGood_pt,10),25)/17 * (1 - 2*(LepGood_mcMatchId != 0)))");
    }

    factory->PrepareTrainingAndTestTree( lepton+" LepGood_mcMatchId != 0", lepton+" LepGood_mcMatchId == 0", "" );
    //#factory->PrepareTrainingAndTestTree( lepton+" LepGood_mcMatchId != 0", lepton+" LepGood_mcMatchId == 0", "nTrain_signal=20000:nTest_signal=20000:nTrain_background=20000:nTest_background=20000" );


    factory->BookMethod( TMVA::Types::kLD, "LD", "!H:!V:VarTransform=None" );
    
    // Boosted Decision Trees with gradient boosting
    TString BDTGopt = "!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:MaxDepth=8";

    BDTGopt += ":CreateMVAPdfs"; // Create Rarity distribution
    factory->BookMethod( TMVA::Types::kBDT, "BDTG", BDTGopt);

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fOut->Close();
}
