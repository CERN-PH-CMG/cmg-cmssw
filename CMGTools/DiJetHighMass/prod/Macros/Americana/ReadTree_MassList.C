string outFile("dijet_mass_fat_358pbm1_");

void ReadTree_MassList(char* file, char* outLabel){

  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  cout << file << endl;




  string outFile("dijet_mass_fat_358pbm1_");
  outFile = outFile + "" + outLabel + ".txt";


  TFile *inf_data  = TFile::Open(file);

  ofstream output_text_fat;
  output_text_fat.open(outFile.c_str());
 

  TTree *tr_data_large = (TTree*)inf_data->Get("ak7/ProcessedTree");

  QCDEvent *Event_data = new QCDEvent();
  TBranch *branch = tr_data_large->GetBranch("events");
  branch->SetAddress(&Event_data);

  TFile* temp = new TFile("/tmp/mgouzevi/toto.root","RECREATE");
  temp->cd();

  TCut generalCut = "(fatmjjcor(0) > 740.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3)";

  TTree* tr_data = tr_data_large->CopyTree(generalCut);

  unsigned NEntries_data_SKIMED = tr_data->GetEntries();
  cout<<"Reading TREE: "<< NEntries_data_SKIMED <<" events"<<endl;



  // # of entries -------------------------------------------

  unsigned NEntries_data = tr_data->GetEntries();
  cout<<"Reading TREE: "<<NEntries_data <<" events"<<endl;
  int decade = 0;

  // counters------------------------------------------------

  unsigned data_count(0);
  unsigned vertex1_count(0);
  unsigned vertex2_count(0);
  unsigned runNo_count(0);

  unsigned mass_count(0);
  unsigned eta_count(0);
  unsigned jetID_count(0);

  unsigned pf_mass_count(0);
  unsigned pf_eta_count(0);
  unsigned pf_jetID_count(0);

  unsigned fat_mass_count(0);
  unsigned fat_eta_count(0);
  unsigned fat_eta_count(0);
  unsigned fat_jetID_count(0);

  unsigned mc_count(0);

  // fill text files





  for(unsigned i=0;i<NEntries_data;i++) {
    double progress = 10.0*i/(1.0*NEntries_data);
    int k = TMath::FloorNint(progress);
    
    if (k > decade)
      cout<<10*k<<" %"<<endl;

    decade = k;

    tr_data->GetEntry(i);

    bool vertex_cut1 = (Event_data->evtHdr()).PVndof() >= 4;
    bool vertex_cut2 = (Event_data->evtHdr()).PVz() < 24 ;


    if(vertex_cut1) {
     
      if(vertex_cut2) {
     

	double dPhi_fat = (TMath::Pi()-fabs(fabs((Event_data->fatjet(0)).phi()-(Event_data->fatjet(1)).phi())-TMath::Pi()));

	int nVtx = (Event_data->evtHdr()).nVtx();
	bool eta_cuts = fabs((Event_data->calojet(0)).eta()) < 2.5 && fabs((Event_data->calojet(1)).eta()) < 2.5;
	bool deta_cut = fabs( (Event_data->calojet(0)).eta() - (Event_data->calojet(1)).eta() ) < 1.3;
	bool vertex_cut1 = (Event_data->evtHdr()).PVndof() >= 4;
	bool vertex_cut2 = (Event_data->evtHdr()).PVz() < 24 ;
	bool corMass_cuts = Event_data->calomjjcor(0) > 838.0;
	bool jetID_cuts = (Event_data->calojet(0)).looseID() == true && (Event_data->calojet(1)).looseID() == true;
	bool runNo_cuts = true;
	//    bool runNo_cuts = (Event_data->evtHdr()).runNo() == 163078;

	bool fat_eta_cuts = fabs((Event_data->fatjet(0)).eta()) < 2.5 && fabs((Event_data->fatjet(1)).eta()) < 2.5;
	bool fat_deta_cut = fabs( (Event_data->fatjet(0)).eta() - (Event_data->fatjet(1)).eta() ) < 1.3;
	bool fat_corMass_cuts = Event_data->fatmjjcor(0) > 740; //838.0;
	bool fat_jetID_cuts = (Event_data->fatjet(0)).looseID() == true && (Event_data->fatjet(1)).looseID() == true;
	bool fat_dPhi_cuts = dPhi_fat > TMath::Pi()/3; 

	data_count++;
    
	if(fat_eta_cuts) {
	  if (fat_deta_cut) {
	    fat_eta_count++;
	    if(fat_corMass_cuts) {
	      fat_mass_count++;
	      if(fat_jetID_cuts && fat_dPhi_cuts) {
	    
 
		output_text_fat << Event_data->fatmjjcor(0) << std::endl;
	      }
	    }
	  }
	}
      }
  
    }
  }


  output_text_fat.close();

  std::cout << "# of events = " << data_count << std::endl;
  std::cout << "# of events after runNo cut = " << runNo_count << std::endl;
  std::cout << "# of events after vertex1 cut = " << vertex1_count << std::endl;
  std::cout << "# of events after vertex2 cut = " << vertex2_count << std::endl;
  std::cout << "# of events after eta cut(calo) = " << fat_eta_count << std::endl;
  std::cout << "# of events after mass cut(calo) = " << fat_mass_count << std::endl;
  std::cout << "# of events after all cut(calo) = " << fat_jetID_count << std::endl;

}
