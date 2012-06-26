void Load(){

  gROOT->ProcessLine(".L Sample.C+");
  gROOT->ProcessLine(".L TauMuPlotter.C+");
  gROOT->ProcessLine(".L tauMuConfig2011Reload.C+");
}
