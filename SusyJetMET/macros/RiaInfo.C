double RiaInfo(TTree* tree){
  

  double initialEvents = 0;
  double genCrossSection = 0;
  double genFilterEfficiency = 0;
  
  tree->SetBranchAddress("initialEvents",&initialEvents);
  tree->SetBranchAddress("genCrossSection",&genCrossSection);
  tree->SetBranchAddress("genFilterEfficiency",&genFilterEfficiency);
  
  double nEvents = 0;
  for(int i = 0; i < tree->GetEntries(); i++){
    tree->GetEntry(i);
    nEvents += initialEvents;
  }
  double weight = 1.0;
  if(genCrossSection > 0){//take the MC values
    if (genFilterEfficiency<0) {
      cout<<"warning: negative gen filter efficiency, setting it to 1"<<endl;
      genFilterEfficiency = 1;
    }
    weight = (genFilterEfficiency*genCrossSection)/nEvents;
    cout<<"nEvents initial : " <<nEvents<<endl;
    cout<<"gen filter eff  : " <<genFilterEfficiency<<endl;
    cout<<"gen xsec        : " <<genCrossSection<<endl;
    cout << "MC Weight: " << weight << std::endl;
  }
  return weight;
}
