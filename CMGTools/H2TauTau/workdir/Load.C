void Load(){

  gROOT->ProcessLine(".L Sample.C+");

  gROOT->ProcessLine(".L TauMuPlotter.C+");
  gROOT->ProcessLine(".L tauMuConfig2011.C+");
  gROOT->ProcessLine(".L tauMuConfig2012.C+");

  gROOT->ProcessLine(".L TauElePlotter.C+");
  gROOT->ProcessLine(".L tauEleConfig2011.C+");
  gROOT->ProcessLine(".L tauEleConfig2012.C+");
}
