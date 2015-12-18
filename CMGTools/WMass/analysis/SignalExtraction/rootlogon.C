{    
  // if(gSystem->Getenv("CMSSW_VERSION")) {
     // cout << endl;
     // cout << " *** RooFit version in CMSSW is too old! ***" << endl;
     // cout << "  Please use a non-CMSSW environment." << endl;
     // cout << endl;
     
  // } else {
    // gSystem->Load("libRooFit");
    
    gROOT->Macro("Utils/RooVoigtianShape.cc+");
    gROOT->Macro("Utils/RooCMSShape.cc+");

    gROOT->Macro("Utils/CPlot.cc++");
    gROOT->Macro("Utils/MitStyleRemix.cc++");  
  // }
               
  // Show which process needs debugging
  gInterpreter->ProcessLine(".! ps |grep root.exe");
}
