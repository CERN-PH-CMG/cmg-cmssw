
bool createMCPUPHisto(){

  //create a histogram with the MC PUP distribution
  //this must be done with unselected MC events
  TH1D MCPUPHisto("pileup","",36,-.5,35.5);//must include 0
  MCPUPHisto.Sumw2();

  std::vector<std::string> filelist;
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_0.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_10.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_100.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1001.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1002.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1003.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1004.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1005.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1006.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1007.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1008.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1009.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_101.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1010.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1011.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1012.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1013.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1014.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1015.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1016.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1017.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1018.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1019.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_102.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1020.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1021.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1022.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1023.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1024.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1025.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1026.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1027.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1028.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1029.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_103.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1030.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1031.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1032.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1033.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1034.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1035.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1036.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1037.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1038.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1039.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_104.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1040.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1041.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1042.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1043.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1044.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1045.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1046.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1047.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1048.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1049.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_105.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1050.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1051.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1052.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1053.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1054.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1055.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1056.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1057.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1058.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1059.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_106.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1060.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1061.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1062.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1063.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1064.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1065.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1066.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1067.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1068.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1069.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_107.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1070.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1071.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1072.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1073.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1074.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1075.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1076.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1077.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1078.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1079.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_108.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1080.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1081.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1082.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1083.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1084.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1085.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1086.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1087.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1088.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1089.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_109.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1090.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1091.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1092.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1093.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1094.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1095.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1096.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1097.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1098.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1099.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_11.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_110.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1100.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1101.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1102.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1103.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1104.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1105.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1106.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1107.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1108.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1109.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_111.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1110.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1111.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1112.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1113.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1114.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1115.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1116.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1117.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1118.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1119.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_112.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1120.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1121.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1122.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1123.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1124.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1125.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1126.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1127.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1128.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1129.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_113.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1130.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1131.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1132.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1133.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1134.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1135.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1136.root");
  filelist.push_back("/tmp/benitezj/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0/tree_CMG_1137.root");


  fwlite::ChainEvent chain(filelist);

  Int_t ievt=0;
  for(chain.toBegin(); !chain.atEnd() ; ++chain, ++ievt){
    if(ievt%printFreq_==0)cout<<ievt<<" done"<<endl;
    const fwlite::Event * event = chain.event();

    //get the number of pile up vertexes
    edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    event->getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;  
    int npv=-1;
    for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {    
      int BX = PVI->getBunchCrossing();    
      if(BX == 0) { 
        npv = PVI->getPU_NumInteractions();
        continue;
      }    
    }
    MCPUPHisto.Fill(npv);
    
  }

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}



bool createMCPUP3DHisto(){

  TH1D MCPUPHisto("pileup","",25,-.5,24.5);//must include 0
  MCPUPHisto.Sumw2();

  //"true" distribution taken from here: 
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities#3D_Reweighting

  // Summer11 PU_S4, distribution obtained by averaging the number of interactions
  // in each beam crossing to estimate the true mean.  
  Double_t PoissonIntDist[25] = {
    0.104109,
    0.0703573,
    0.0698445,
    0.0698254,
    0.0697054,
    0.0697907,
    0.0696751,
    0.0694486,
    0.0680332,
    0.0651044,
    0.0598036,
    0.0527395,
    0.0439513,
    0.0352202,
    0.0266714,
    0.019411,
    0.0133974,
    0.00898536,
    0.0057516,
    0.00351493,
    0.00212087,
    0.00122891,
    0.00070592,
    0.000384744,
    0.000219377
  };


//   // I think this corresponds to the "true" distribution, check the number of bins
//   // Distribution used for Fall2011 MC.  
//   Double_t Fall2011[50] = {
//     0.003388501,
//     0.010357558,
//     0.024724258,
//     0.042348605,
//     0.058279812,
//     0.068851751,
//     0.072914824,
//     0.071579609,
//     0.066811668,
//     0.060672356,
//     0.054528356,
//     0.04919354,
//     0.044886042,
//     0.041341896,
//     0.0384679,
//     0.035871463,
//     0.03341952,
//     0.030915649,
//     0.028395374,
//     0.025798107,
//     0.023237445,
//     0.020602754,
//     0.0180688,
//     0.015559693,
//     0.013211063,
//     0.010964293,
//     0.008920993,
//     0.007080504,
//     0.005499239,
//     0.004187022,
//     0.003096474,
//     0.002237361,
//     0.001566428,
//     0.001074149,
//     0.000721755,
//     0.000470838,
//     0.00030268,
//     0.000184665,
//     0.000112883,
//     6.74043E-05,
//     3.82178E-05,
//     2.22847E-05,
//     1.20933E-05,
//     6.96173E-06,
//     3.4689E-06,
//     1.96172E-06,
//     8.49283E-07,
//     5.02393E-07,
//     2.15311E-07,
//     9.56938E-08
//   };




  for(Int_t b=0;b<25;b++)
    MCPUPHisto.SetBinContent(b,PoissonIntDist[b]);

  MCPUPHisto.Scale(1./MCPUPHisto.Integral());
  TFile F("./MCPUP3D.root","recreate");
  if(F.IsZombie())return 0;
  MCPUPHisto.Write();
  F.Close();

  return 1;
}
