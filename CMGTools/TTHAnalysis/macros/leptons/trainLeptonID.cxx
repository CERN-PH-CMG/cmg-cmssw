void trainLeptonID(TString name, TString train="GoodvsBad") {
    TTree *dSig = (TTree*) _file0->Get("rec/t");
    TTree *dBg1 = (TTree*) _file1->Get("rec/t");
    TFile *fOut = new TFile(name+".root","RECREATE");
    TMVA::Factory *factory = new TMVA::Factory(name, fOut, "!V:!Color");

    
    if (!name.Contains("pteta")) {
        factory->AddSpectator("pt", 'D');
        factory->AddSpectator("abseta := abs(eta)", 'D');
    }

    TString allvars = ""; 
    //common variables
    factory->AddVariable("neuRelIso03 := relIso03 - chargedHadRelIso03", 'D'); allvars += "neuRelIso03";
    factory->AddVariable("chRelIso03 := chargedHadRelIso03", 'D');        allvars += ":chRelIso03";
    factory->AddVariable("jetDR := min(jetDR,0.5)", 'D');         allvars += ":jetDR";
    factory->AddVariable("jetPtRatio := min(jetPtRatio,1.5)", 'D');   allvars += ":jetPtRatio";
    factory->AddVariable("jetBTagCSV := max(jetBTagCSV,0)", 'D');   allvars += ":jetBTagCSV";


    if (!name.Contains("NoIP")) {
        factory->AddVariable("sip3d", 'D'); 
        factory->AddVariable("dxy := log(abs(dxy))", 'D');
        factory->AddVariable("dz  := log(abs(dz))",  'D');
        allvars += ":sip3d:dxy:dz";
    }

    if (name.Contains("mu")) {
        if (!name.Contains("NoID")) {
            factory->AddVariable("mvaId",'D');    allvars += ":mvaId";
            
        }
    } else if (name.Contains("el")) {
      //electron variables
        if (!name.Contains("NoID")) {
            factory->AddVariable("mvaId",'D');     allvars += ":mvaId";
            
        }
    } else { std::cerr << "ERROR: must either be electron or muon." << std::endl; return; }

    if (name.Contains("SV")) {
        factory->AddVariable("hasSV", 'D');   allvars += ":hasSV";
        factory->AddVariable("svRedPt := min(max( svRedPt, -10),100)", 'D');   allvars += ":svRedPt";
        factory->AddVariable("svRedM := min(max( svRedM, -1),6)", 'D');   allvars += ":svRedM";
        factory->AddVariable("svM := min(max( svM, -1),6)", 'D');   allvars += ":svM";
        factory->AddVariable("svPt := min(max( svPt, -10),100)", 'D');   allvars += ":svPt";
        factory->AddVariable("svSip3d := min(max( svSip3d, -10),100)", 'D');   allvars += ":svSip3d";
        factory->AddVariable("svLepSip3d := min(max( svLepSip3d, -10),5)", 'D');   allvars += ":svLepSip3d";
        factory->AddVariable("svNTracks := min(max( svNTracks, -1),10)", 'D');   allvars += ":svNTracks";
        factory->AddVariable("svChi2n := min(max( svChi2n, -1),10)", 'D');   allvars += ":svChi2n";
        factory->AddVariable("svDxy := min(max( svDxy, 0),4)", 'D');   allvars += ":svDxy";
    }
   

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

    if (train=="GoodvsBad") {
        factory->PrepareTrainingAndTestTree( lepton+" mcMatchId > 0", lepton+" mcMatchId <= 0", "" );
    } else if (train=="GoodvsLight") {
        factory->PrepareTrainingAndTestTree( lepton+" mcMatchId > 0", lepton+" mcMatchId <= 0 && mcMatchAny <= 1", "" );
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
    BDTG_Cat_pteta->AddMethod("pt < 15 && abs(eta) < 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_b",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt < 15 && abs(eta) > 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_e",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt > 15 && abs(eta) < 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_b", BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt > 15 && abs(eta) > 1.5", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_e", BDTGopt);

    } else if (name.Contains("el") && !name.Contains("pteta")){
    TMVA::MethodCategory* BDTG_Cat_pteta = dynamic_cast<TMVA::MethodCategory*>(factory->BookMethod( TMVA::Types::kCategory, "BDTG_Cat_pteta","" ));
    BDTG_Cat_pteta->AddMethod("pt <= 10 && abs(eta) <  0.8",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt <= 10 && abs(eta) <  1.479 && abs(eta) >= 0.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_fb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt <= 10 && abs(eta) >= 1.479",                    allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_low_e", BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt  > 10 && abs(eta) <  0.8",                      allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_cb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt  > 10 && abs(eta) <  1.479 && abs(eta) >= 0.8", allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_fb",  BDTGopt);
    BDTG_Cat_pteta->AddMethod("pt  > 10 && abs(eta) >= 1.479",                    allvars, TMVA::Types::kBDT, "BDTG_Cat_pteta_high_e", BDTGopt);
    }

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fOut->Close();
}
