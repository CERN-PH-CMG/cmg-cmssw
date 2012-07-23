void Load(){

  gROOT->ProcessLine(".L Sample.C+");

  gROOT->ProcessLine(".L TauMuPlotter.C+");
  gROOT->ProcessLine(".L configTauMu2011.C+");
  gROOT->ProcessLine(".L configTauMu2012.C+");

  gROOT->ProcessLine(".L TauElePlotter.C+");
  gROOT->ProcessLine(".L configTauEle2011.C+");
  gROOT->ProcessLine(".L configTauEle2012.C+");
}
