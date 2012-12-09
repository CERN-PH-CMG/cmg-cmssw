
void analyzer() {


  if (! TString(gSystem->GetLibraries()).Contains("TreeReader")) {
    gROOT->Macro("lib/loader.C");
    gROOT->LoadMacro("$CMSSW_BASE/src/CMGTools/TTHAnalysis/python/plotter/TreeReader.cc+");
  }


  // INPUT TREES
 
  //TString inputDir="root://lxcms00//data3/cristina_area/TTH/TREES_081212/";
  //TString inputDir="/data3/cristina_area/TTH/TREES_081212/";
  TString inputDir="/afs/cern.ch/work/b/botta/TTHAnalysis/BE_031212/CMGTools/CMSSW_5_3_5/src/CMGTools/TTHAnalysis/cfg/TREES_081212/";
  TString outDir = "TTH_plots/081212/";

  float lumi = 20000.;
  vector<component> components;
  components.push_back(component("TTH",       0.1271));
  components.push_back(component("TTH",       0.1271,  "tautau"));
  components.push_back(component("TTH",       0.1271,  "ZZ"));
  components.push_back(component("TTH",       0.1271,  "WW"));
  //components.push_back(component("TTH",       0.1271,  "XX"));
  components.push_back(component("TTWJets",   0.232));
  components.push_back(component("TTZJets",   0.208));
  components.push_back(component("WWWJets",   0.08217));
  components.push_back(component("WWZJets",   0.0633));
  components.push_back(component("WGs2MU",    1.914));
  components.push_back(component("WGs2E",     5.873));
  components.push_back(component("WGs2Tau",   0.336));
  components.push_back(component("TTWWJets",  0.002037)); 
  components.push_back(component("DYJetsM10", 915)); 
  components.push_back(component("DYJetsM50", 3503.7)); 
  components.push_back(component("TTLep",     23.64));
  components.push_back(component("WWJets",    5.995));
  components.push_back(component("WZJets",    1.057));
  components.push_back(component("ZZ2e2mu",   0.1767));
  components.push_back(component("ZZ2e2tau",  0.1767));
  components.push_back(component("ZZ2mu2tau", 0.1767));
  components.push_back(component("ZZTo4mu",   0.07691));   
  components.push_back(component("ZZTo4e",    0.07691));
  components.push_back(component("ZZTo4tau",  0.07691));
  components.push_back(component("TtW",       11.73));
  components.push_back(component("TbartW",    11.73));

  TString mkdircmd = "mkdir -p ";
  gSystem->Exec(mkdircmd+outDir);

  for (vector<component>::const_iterator comp = components.begin(); comp!=components.end(); ++comp) {    
    component c = (*comp);
    TString name = c.name;
    float xsect = c.xsec;
    TString mode = c.mode;
    TString filepath= inputDir + "/" + name + "/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root";
    TString counterFilePath= inputDir + "/" + name + "/skimAnalyzerCount/SkimReport.txt";

    //-- Extract nevt_gen
    double nevt_gen = getCounter(counterFilePath.Data(),"All Events");
    float weight = xsect*lumi/nevt_gen;

    cout << name << " " << mode << " : xsect= " << xsect << " nevt_gen= " << nevt_gen << " weight =" << weight << endl;

    // Create output file for this sample
    TFile* outputFile = new TFile(outDir+"/"+name+mode+".root", "RECREATE");

    // Get the tree
    TChain *tree = new TChain("ttHLepTreeProducerBase");
    tree->Add(filepath);
    float entries = tree->GetEntries();
    cout << "Entries: " << entries << " #events: " << entries*weight << endl;

    TreeReader reader(tree, weight);
    reader.Loop(mode);

    outputFile->Close();

    cout << endl;
  }
}
 
