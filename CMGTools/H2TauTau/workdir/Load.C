void Load(){

  gROOT->ProcessLine(".L Sample.C+");

  gROOT->ProcessLine(".L TauMuPlotter.C+");

  gROOT->ProcessLine(".L TauElePlotter.C+");

  //gROOT->ProcessLine(".L weightedCombine.C+");
}

