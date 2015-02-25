void trainLeptonID(TString name, TString train="GoodvsBad") {
    TTree *dSig = (TTree*) _file0->Get("tree");
    TTree *dBg1 = (TTree*) _file1->Get("tree");
    TFile *fOut = new TFile(name+".root","RECREATE");
    TMVA::Factory *factory = new TMVA::Factory(name, fOut, "!V:!Color:Transformations=I");
    
    if (!name.Contains("pteta")) {
        factory->AddSpectator("LepGood_pt", 'D');
        factory->AddSpectator("LepGood_abseta := abs(eta)", 'D');
    }

    TString allvars = ""; 
    //common variables
    factory->AddVariable("LepGood_neuRelIso03 := LepGood_relIso03 - LepGood_chargedHadRelIso03", 'D'); allvars += "neuRelIso03";
    factory->AddVariable("LepGood_chRelIso03 := LepGood_chargedHadRelIso03", 'D');        allvars += ":chRelIso03";
    factory->AddVariable("LepGood_jetDR := min(LepGood_jetDR,0.5)", 'D');         allvars += ":jetDR";
    factory->AddVariable("LepGood_jetPtRatio := min(LepGood_jetPtRatio,1.5)", 'D');   allvars += ":jetPtRatio";
    factory->AddVariable("LepGood_jetBTagCSV := max(LepGood_jetBTagCSV,0)", 'D');   allvars += ":jetBTagCSV";


    if (!name.Contains("NoIP")) {
        factory->AddVariable("LepGood_sip3d", 'D'); 
        factory->AddVariable("LepGood_dxy := log(abs(LepGood_dxy))", 'D');
        factory->AddVariable("LepGood_dz  := log(abs(LepGood_dz))",  'D');
        allvars += ":sip3d:dxy:dz";
    }

    if (name.Contains("mu")) {
        if (!name.Contains("NoID")) {
	  //factory->AddVariable("LepGood_mvaId",'D');    allvars += ":mvaId";
            factory->AddVariable("LepGood_segmentCompatibility",'D');    allvars += ":segmentCompatibility";
        }
    } else if (name.Contains("el")) {
      //electron variables
        if (!name.Contains("NoID")) {
            factory->AddVariable("LepGood_mvaId",'D');     allvars += ":mvaId";           
        }
    } else { std::cerr << "ERROR: must either be electron or muon." << std::endl; return; }

    if (name.Contains("SV")) {
        factory->AddVariable("LepGood_hasSV", 'D');   allvars += ":hasSV";
        factory->AddVariable("LepGood_svRedPt := min(max( LepGood_svRedPt, -10),100)", 'D');   allvars += ":svRedPt";
        factory->AddVariable("LepGood_svRedM := min(max( LepGood_svRedM, -1),6)", 'D');   allvars += ":svRedM";
        factory->AddVariable("LepGood_svM := min(max( LepGood_svM, -1),6)", 'D');   allvars += ":svM";
        factory->AddVariable("LepGood_svPt := min(max( LepGood_svPt, -10),100)", 'D');   allvars += ":svPt";
        factory->AddVariable("LepGood_svSip3d := min(max( LepGood_svSip3d, -10),100)", 'D');   allvars += ":svSip3d";
        factory->AddVariable("LepGood_svLepSip3d := min(max( LepGood_svLepSip3d, -10),5)", 'D');   allvars += ":svLepSip3d";
        factory->AddVariable("LepGood_svNTracks := min(max( LepGood_svNTracks, -1),10)", 'D');   allvars += ":svNTracks";
        factory->AddVariable("LepGood_svChi2n := min(max( LepGood_svChi2n, -1),10)", 'D');   allvars += ":svChi2n";
        factory->AddVariable("LepGood_svDxy := min(max( LepGood_svDxy, 0),4)", 'D');   allvars += ":svDxy";
    }
   
    TCut lepton = (name.Contains("mu") ? "abs(LepGood_pdgId) == 13 && (LepGood_innerTrackValidHitFraction >= 0.80 &&  LepGood_segmentCompatibility>0.303 + (0.451-0.303)*(!(LepGood_chi2LocalPosition < 12 && LepGood_globalTrackChi2 > 0 && LepGood_globalTrackChi2 < 3 && LepGood_trkKink < 20)))" : "abs(LepGood_pdgId) == 11");

    if (name.Contains("mu")) {
        if (name.Contains("pteta")) {
            if (name.Contains("low"))  lepton += "LepGood_pt <= 10";
	    if (name.Contains("medium_b"))  lepton += "LepGood_pt > 10 && LepGood_pt <= 25 && abs(LepGood_eta) <  1.5";
            if (name.Contains("medium_e"))  lepton += "LepGood_pt > 10 && LepGood_pt <= 25 && abs(LepGood_eta) >= 1.5";
            if (name.Contains("high_b")) lepton += "LepGood_pt > 25 && abs(LepGood_eta) <  1.5";
            if (name.Contains("high_e")) lepton += "LepGood_pt > 25 && abs(LepGood_eta) >= 1.5";
        }
    } else if (name.Contains("el")) {
        if (name.Contains("pteta")) {
            if (name.Contains("low"))  lepton += "LepGood_pt <= 10";
            if (name.Contains("medium_cb")) lepton += "LepGood_pt >  10 && LepGood_pt <= 25 && abs(LepGood_eta) <   0.8";
            if (name.Contains("medium_fb")) lepton += "LepGood_pt >  10 && LepGood_pt <= 25 && abs(LepGood_eta) >=  0.8 && abs(LepGood_eta) < 1.479";
            if (name.Contains("medium_ec")) lepton += "LepGood_pt >  10 && LepGood_pt <= 25 && abs(LepGood_eta) >=  1.479";
	    if (name.Contains("high_cb")) lepton += "LepGood_pt >  25 && abs(LepGood_eta) <   0.8";
            if (name.Contains("high_fb")) lepton += "LepGood_pt >  25 && abs(LepGood_eta) >=  0.8 && abs(LepGood_eta) < 1.479";
            if (name.Contains("high_ec")) lepton += "LepGood_pt >  25 && abs(LepGood_eta) >=  1.479";
        }
    }

    double wSig = 1.0, wBkg = 1.0;
    factory->AddSignalTree(dSig, wSig);
    factory->AddBackgroundTree(dBg1, wBkg);

    // re-weighting to approximately match n(jet) multiplicity of signal
    //factory->SetWeightExpression("puWeight*((good>0)+(good<=0)*pow(nJet25,2.36))");
    //factory->SetWeightExpression("puWeight");

    if (train=="GoodvsBad") {
        factory->PrepareTrainingAndTestTree( lepton+" LepGood_mcMatchId != 0", lepton+" LepGood_mcMatchId == 0", "" );
    } else if (train=="GoodvsLight") {
      factory->PrepareTrainingAndTestTree( lepton+" LepGood_mcMatchId != 0", lepton+" LepGood_mcMatchId == 0 && LepGood_mcMatchAny <= 1", "" ); //check mcMatchAny with new def
    }  else { 
        std::cerr << "ERROR: No idea of what training you want." << std::endl; return; 
    }


    factory->BookMethod( TMVA::Types::kLD, "LD", "!H:!V:VarTransform=None" );
    
    // Boosted Decision Trees with gradient boosting
    TString BDTGopt = "!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=8";

    BDTGopt += ":CreateMVAPdfs"; // Create Rarity distribution
    factory->BookMethod( TMVA::Types::kBDT, "BDTG", BDTGopt);

    if (name.Contains("mu") && !name.Contains("pteta")) {
    TMVA::MethodCategory* BDTG_Cat_pteta = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta","" ));
    BDTG_Cat_pteta->AddMethod("LepGood_pt <= 10", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt > 10 && LepGood_pt <= 25 && abs(LepGood_eta) < 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_ptLepGood_eta_medium_b", BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt > 10 && LepGood_pt <= 25 && abs(LepGood_eta) > 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_medium_e", BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt > 25 && abs(LepGood_eta) < 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_b", BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt > 25 && abs(LepGood_eta) > 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_e", BDTGopt);

    } else if (name.Contains("el") && !name.Contains("pteta")){
    TMVA::MethodCategory* BDTG_Cat_pteta = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta","" ));
    BDTG_Cat_pteta->AddMethod("LepGood_pt <= 10",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt  > 10 && LepGood_pt <= 25 && abs(LepGood_eta) <  0.8",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_medium_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt  > 10 && LepGood_pt <= 25 && abs(LepGood_eta) <  1.479 && abs(LepGood_eta) >= 0.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_medium_fb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt  > 10 && LepGood_pt <= 25 && abs(LepGood_eta) >= 1.479",                    allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_medium_e", BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt  > 25 && abs(LepGood_eta) <  0.8",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt  > 25 && abs(LepGood_eta) <  1.479 && abs(LepGood_eta) >= 0.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_fb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("LepGood_pt  > 25 && abs(LepGood_eta) >= 1.479",                    allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_e", BDTGopt);
    }

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fOut->Close();
}
