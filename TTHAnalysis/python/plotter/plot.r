void plot() {
  
  

  // --------------------------------
  // input dir
  // --------------------------------
 
  TString inputDir="TTH_plots/171212/";
  
  
  bool skipSmallBg = false;
  bool mergeWG = true;      // Merge WG
  bool mergeZZ = true;      // Merge ZZ
  bool mergeDY = true;      // Merge DY
  bool mergeVVV = true;     // Merge VVV
  
  
  // -----------------------------------------------------------
  
  if (! TString(gSystem->GetLibraries()).Contains("Number")) {
    gROOT->LoadMacro("$CMSSW_BASE/src/CMGTools/TTHAnalysis/python/plotter/lib/Number.cc+");
    gROOT->LoadMacro("$CMSSW_BASE/src/CMGTools/TTHAnalysis/python/plotter/lib/HistoStack.cc+");
    gROOT->LoadMacro("lib/macros.C");
    gROOT->LoadMacro("lib/stackplot.C");
  }
  
  
  //--------------------------------
  // Set the style
  //--------------------------------

  gStyle->Reset();
  TStyle * style = getStyle("ZZ");

  style->SetMarkerSize(0.8);
  style->cd();


  //-----------------------------------------
  // Samples
  //-----------------------------------------
   
  vector<TString> samples;
  
  
  samples.push_back("TTWJets");   
  samples.push_back("TTZJets");
  samples.push_back("WWWJets");
  samples.push_back("WWZJets");
  samples.push_back("WGs2MU");
  samples.push_back("WGs2E");
  samples.push_back("WGs2Tau");
  samples.push_back("DYJetsM10");
  samples.push_back("DYJetsM50");
  samples.push_back("TTLep");
  samples.push_back("WWJets");
  samples.push_back("WZJets");
  samples.push_back("ZZ2e2mu");
  samples.push_back("ZZ2e2tau");
  samples.push_back("ZZ2mu2tau");
  samples.push_back("ZZTo4mu");
  samples.push_back("ZZTo4e");
  samples.push_back("ZZTo4tau");
  if(!skipSmallBg){
    samples.push_back("TtW");
    samples.push_back("TbartW");
    samples.push_back("TTWWJets");
  }
  //samples.push_back("TTHWW");     //signal splitted in 3 components
  //samples.push_back("TTHZZ");     //signal splitted in 3 components
  //samples.push_back("TTHtautau"); //signal splitted in 3 components
  //samples.push_back("TTHXX");     //signal splitted in 3 components
  samples.push_back("TTH");       //inclusive signal

  //-----------------------------------------
  // Define groups of samples
  //-----------------------------------------

  HistoStack *stacker = new HistoStack();
   
  // Note that "data" is special!                              
  // stacker->setGroup("data",   true, kBlack, 20, 0);      
     

  //stacker->setGroup("TTHWW126",true, kOrange+10,  20, 50);          
  //stacker->setGroup("TTHZZ126",true, kOrange+10,  20, 50);          
  //stacker->setGroup("TTHtautau126",true, kOrange+10,  20, 50);          
  stacker->setGroup("TTH126",true, kOrange+10,  20, 50);          
  stacker->setGroup("TTW", false, kGreen-5,  28, 6);   
  stacker->setGroup("TTZ", false, kSpring+2,  8, 2);
  stacker->setGroup("ZZ", false, kAzure-9, 23, 1);
  stacker->setGroup("WW", false, kBlue-9,   30, 2);
  stacker->setGroup("WZ", false, kViolet-4,    23, 1);
  stacker->setGroup("DY", false, kCyan, 23, 1);
  stacker->setGroup("TT", false, kGray,  21, 3);
  stacker->setGroup("WG", false, kAzure-9, 8, 1);
  stacker->setGroup("VVV", false, kViolet,    28, 1);
  stacker->setGroup("TTWW", false,  kBlue-9,    25, 1);
  stacker->setGroup("Single top", false, kCyan+2,  25, 3);
  
  //--- There's no need to assign data samples to "data" groups, providing
  //--- that the sample name starts with "DoubleMu", "DoubleEle", "DoubleOr", or "data".
  
  stacker->assignToGroup("TTH","TTH126");
  //stacker->assignToGroup("TTHWW","TTHWW126");
  //stacker->assignToGroup("TTHZZ","TTHZZ126");
  //stacker->assignToGroup("TTHtautau","TTHtautau126");
  stacker->assignToGroup("TTWJets", "TTW");
  stacker->assignToGroup("TTZJets", "TTZ");
  stacker->assignToGroup("ZZ2e2mu", "ZZ");
  stacker->assignToGroup("ZZ2e2tau", "ZZ");
  stacker->assignToGroup("ZZ2mu2tau", "ZZ");
  stacker->assignToGroup("ZZTo4mu", "ZZ");
  stacker->assignToGroup("ZZTo4e", "ZZ");
  stacker->assignToGroup("ZZTo4tau", "ZZ");
  stacker->assignToGroup("WWJets", "WW");
  stacker->assignToGroup("WZJets", "WZ");
  stacker->assignToGroup("DYJetsM10", "DY");
  stacker->assignToGroup("DYJetsM50", "DY");
  stacker->assignToGroup("TTLep", "TT");
  stacker->assignToGroup("WGs2MU", "WG");
  stacker->assignToGroup("WGs2E", "WG");
  stacker->assignToGroup("WGs2Tau", "WG");
  stacker->assignToGroup("WWWJets", "VVV");
  stacker->assignToGroup("WWZJets", "VVV");
  stacker->assignToGroup("TTWWJets", "TTWW");
  stacker->assignToGroup("TtW", "Single top");
  stacker->assignToGroup("TbartW", "Single top");

  
  for (int i = 0; i<samples.size();++i) {
    
    TString sName = samples[i];
    // Open the file
    TString fileName = inputDir +"/"+ sName +".root";
    //     cout << fileName << endl;    
    TFile *file = TFile::Open(fileName.Data());


    //----------------------------------------------------------------------
    // Which plots
    //----------------------------------------------------------------------

    
    //TH1F* steps = (TH1F*) file->Get("steps");
    TH1F* steps = (TH1F*) file->Get("steps_2l_em");
    TH1F* jets = (TH1F*) file->Get("jets_2l_mm");
    //TH1F* steps = (TH1F*) file->Get("steps_3l_cat1");
    //TH1F* steps = (TH1F*) file->Get("steps_3l_cat2");

    //----------------------------------------------------------------------
    // Adding the plots to the stacker
    //----------------------------------------------------------------------
    
    stacker->add(sName, "nEvent", steps, 1.);
    stacker->add(sName, "jets_2l_mm", jets , 1.);
    
    
  }

   
  TCanvas* c;
  THStack* s;
  TH1F* d;
   
  //----------------------------------------------------------------------
  // Step Plots
  //----------------------------------------------------------------------
    

  if (false) {
    c = newCanvas("StepPlot");
    c->SetLogy();
    s = stacker->getStack("nEvent");
    d = stacker->getData("nEvent");
    drawStepPlot(s,d,"Cut","Events",1,8);
    stepPlotYield2(s, d,1,8);
  } 


  if (true){
    c = newCanvas("nJets");
    // c->SetLogy();
    s = stacker->getStack("nEvent");
    d = stacker->getData("nEvent");
    drawStack(s, d, 1, "", "", "nJets","a.u.", 70, 110);

  }


  delete stacker;
}


