
{


  string s;
  ofstream myfile ("dijet_mass_fat_358pbm1_data_HT_All.txt");

  int length =0;
  TObjArray *tokenizedLine; // information from data lines

  
  for (int i = 0; i < 14; i++){
    int fileLength =0;

      string outFile("out/res/dijet_mass_fat_358pbm1_data_HT_PromptReco_");  

    if (i <5){

      outFile = "out/res/dijet_mass_fat_358pbm1_data_HT_PromptReco_";
      outFile = outFile + Form("%d", i) + ".txt";

    } else {
      outFile = "out/res/dijet_mass_fat_358pbm1_data_HT_ReReco_";
      outFile = outFile + Form("%d", i-5) + ".txt";
    }

    cout << outFile.c_str() << endl;

    ifstream input_text_fat;
    ifstream in(outFile.c_str(), ios::in | ios::binary); 
  
  
    while (! in.eof() ){
   
      getline(in, s); 
      tokenizedLine = TString(s).Tokenize("\n");
      //   cout << s.data() << endl;
      if (fileLength%1000 == 0) cout << " fileLength = " << fileLength << " mass = " << s.data() << endl;

      //  TString ss(s);
   
      if (tokenizedLine->GetEntries() < 1 || in.eof()) {
	cout << "MEGA WARNING!!!!!!!!!!! check in.eof() " << in.eof() << endl;
	continue;
      }

      

      float mass = ((TObjString*)tokenizedLine->At(0))->GetString().Atof();
      
      if (mass < 838) continue;

      myfile << mass << "\n";
   
      length++;
      fileLength++;
    }
 
   in.close();
   
    cout << outFile.c_str() << " length = " << fileLength << " total Length = " << length << endl;;
    
 

  }


  myfile.close();
  

}
