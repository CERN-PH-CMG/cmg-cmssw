
TH1 *ptHistoGood = 0, *ptHistoBad = 0;
TH1* initWeights1(TTree *tree, TTree *refTree, TCut  cut) {
    double pts[999]; int npt = 0; 
    for (double pt =  5;  pt <   10; pt += 1.0) pts[++npt] = pt;
    for (double pt =  10; pt <   25; pt += 2.5) pts[++npt] = pt;
    for (double pt =  25; pt <   50; pt += 5.0) pts[++npt] = pt;
    for (double pt =  50; pt <  100; pt += 10)  pts[++npt] = pt;
    for (double pt = 100; pt <= 200; pt += 25)  pts[++npt] = pt;
    TH1F *ptNum = new TH1F("ptNum","ptNum",npt-1,pts);
    refTree->Draw("pt>>ptNum",cut,"GOFF");
    TH1F *ptDen = new TH1F("ptDen","ptDen",npt-1,pts);
    tree->Draw("pt>>ptDen",cut,"GOFF");
    TH1* ret = (TH1*) ptNum->Clone("ptNum");
    ret->Divide(ptDen);
    return ret;
}

void initWeights(TTree *sigTree, TTree *bkgTree, TCut cut) {
    TFile *refGood = TFile::Open("treeTTH.root");
    TFile *refBad = TFile::Open("treeTTLep.root");
    TTree *tGood = (TTree*) refGood->Get("rec/t");
    TTree *tBad = (TTree*) refBad->Get("rec/t");
    ptHistoGood = initWeights1(sigTree, tGood, cut + "good > 0");
    ptHistoGood->Draw();
    c1->Print("~/public_html/drop/plots/wpt_good.png");
    //ptHistoBad = initWeights1(bkgTree, tBad, Form("(%s && good <= 0)*pow(nJet25,2.36)",(const char *)cut));
    ptHistoBad = initWeights1(bkgTree, tBad, cut);
    ptHistoBad->Draw();
    c1->Print("~/public_html/drop/plots/wpt_bad.png");
}

double weight(double pt, int good) {
    TH1 *h = (good > 0 ? ptHistoGood : ptHistoBad);
    int i = h->GetXaxis()->FindBin(pt);
    if (i == h->GetNbinsX()+1) --i;
    else if (i == 0) ++i;
    return h->GetBinContent(i);
}

void uniformWeight(TTree *sigTree, TTree *bkgTree, TCut cut, double &wSig, double &wBkg) {
    TFile *refGood = TFile::Open("treeTTH.root");
    TFile *refBad = TFile::Open("treeTTLep.root");
    TTree *tGood = (TTree*) refGood->Get("rec/t");
    TTree *tBad = (TTree*) refBad->Get("rec/t");
    TH1F *htemp = new TH1F("htemp","",1,0.5,1.5);
    sigTree->Draw("1>>htemp",cut + "good > 0"); 
    double nSig = htemp->GetBinContent(1);
    bkgTree->Draw("1>>htemp",cut + "good < 0"); 
    double nBkg = htemp->GetBinContent(1);

    tGood->Draw("1>>htemp",cut + "good <= 0"); 
    double rSig = htemp->GetBinContent(1);
    tBad->Draw("1>>htemp",Form("(%s && good <= 0)*pow(nJet25,2.36)",(const char *)cut)); 
    double rBkg = htemp->GetBinContent(1);
    
    std::cout << "Weight for good: "  << rSig/nSig << std::endl;
    std::cout << "Weight for bad: "  << rBkg/nBkg << std::endl;
    wSig = rSig/nSig; wBkg = rBkg/nBkg;
    //return TString::Format("(good > 0)*%g  + (good <=0)*%g", rSig/nSig, rBkg/nBkg);
}

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
    uniformWeight(dSig, dBg1, lepton, wSig, wBkg);
    factory->AddSignalTree(dSig, wSig);
    factory->AddBackgroundTree(dBg1, wBkg);

    // re-weighting to approximately match n(jet) multiplicity of signal
    //factory->SetWeightExpression("puWeight*((good>0)+(good<=0)*pow(nJet25,2.36))");
    //factory->SetWeightExpression("((good>0)+(good<=0)*pow(nJet25,2.36))");
    //factory->SetWeightExpression("weight(pt,good)");
    //factory->SetWeightExpression("puWeight");
    //initWeights(dSig,dBg1,lepton);

    if (train=="GB") {
        factory->PrepareTrainingAndTestTree( lepton+" good > 0", lepton+" good <= 0", "" );
    } else if (train=="pB") {
        factory->PrepareTrainingAndTestTree( lepton+" good > 0 && sip3d < 3.5", lepton+" good == 0 && sip3d < 3.5", "" );
    } else if (train=="tB") {
        factory->PrepareTrainingAndTestTree( lepton+" good > 0 && sip3d > 3.5 && sip3d < 6", lepton+" good <= 0 && sip3d > 3.5 && sip3d < 6", "" );
    } else { 
        std::cerr << "ERROR: No idea of what training you want." << std::endl; return; 
    }

    //factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmooth=1:NAvEvtPerBin=50");

    factory->BookMethod( TMVA::Types::kLD, "LD", "!H:!V:VarTransform=None" );

    // Cut optmisation using Genetic Algorithm
    // factory->BookMethod( TMVA::Types::kCuts, "CutsGA", "!H:!V:FitMethod=GA" );

    // Boosted Decision Trees with gradient boosting
    //TString BDTGopt = "!H:!V:NTrees=200:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=8";
    TString BDTGopt = "!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:!UseBaggedGrad:nCuts=2000:nEventsMin=100:NNodesMax=9:UseNvars=9:PruneStrength=5:PruneMethod=CostComplexity:MaxDepth=8";
    ///  !H:!V:NTrees=6000:BoostType=Grad:Shrinkage=0.50:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=2000:PruneMethod=CostComplexity:PruneStrength=50:NNodesMax=5"

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
