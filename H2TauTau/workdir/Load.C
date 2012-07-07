void Load(){

  gROOT->ProcessLine(".L Sample.C+");
  gROOT->ProcessLine(".L TauMuPlotter.C+");
  gROOT->ProcessLine(".L tauMuConfig2011Reload.C+");
  gROOT->ProcessLine(".L TauElePlotter.C+");
  gROOT->ProcessLine(".L tauEleConfig2011Reload.C+");

  gROOT->ProcessLine(".L tauMuConfig2012.C+");
}
