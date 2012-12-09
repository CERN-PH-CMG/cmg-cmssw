
//This script is supposed to bea

{
  // gInterpreter->AddIncludePath(srcpath+"lib/");
  gSystem->cd("lib");
  gROOT->LoadMacro("TTHTree.C+");
  gROOT->LoadMacro("getCounter.cc+");
  gInterpreter->GenerateDictionary("vector<component>","component.h;vector");
  gInterpreter->GenerateDictionary("component","component.h");
  gSystem->cd("..");
}

