vector<double> N;
vector<double> SIGMA;

void LumiYield(){

TFile *_file0 = TFile::Open("out/res/FullSample/histograms_data_HT_All.root");

TH1F* hTRIG = (TH1F* ) _file0->Get("h_FatDiJet_EventYield;1");
 TH1F* hNUM = new TH1F();

 TH1F* hMON = new TH1F();
 hTRIG->Copy(*hMON);
 hMON->Scale(0);

 hTRIG->Copy(*hNUM);

 fillHisto(hMON);

   

 hTRIG->SetMinimum(0);
  //  hTRIG->SetTitle("Event Yield [ p_{T,1}, p_{T,2} > 170, 50 GeV, |#eta_{1,2}|<3, |#Delta #eta| < 3 ] ; Run ; N_{events}/lumi (pb)");

 hTRIG->SetTitleOffset(1.1, "Y");
 hTRIG->SetTitleSize(0.05, "Y");
 hTRIG->SetTitleOffset(0.9, "X");
 hTRIG->SetTitleSize(0.05, "X");

 hTRIG->SetMarkerStyle(20);
 hTRIG->SetMarkerSize(1);

 hTRIG->SetTitle("Dijet events; Run Number; N_{events}");

 hTRIG->Sumw2();

 hTRIG->Draw("P");
 gPad->SetLeftMargin(0.12);
 gPad->SaveAs("hLumiYield_eventsNumber.png");

 TF1* func = new TF1("func", "[0]");
 func->SetLineColor(kRed);

 hTRIG->SetTitle("Dijet events Lumi Yield; Run Number; N_{events}/lumi (pb^{-1})");
 hMON->Sumw2();
 hTRIG->Divide(hMON);
 hTRIG->SetMaximum(hTRIG->GetMaximum()*1.1);
 hTRIG->Draw("P");
 hTRIG->Fit(func);

 gPad->SetLeftMargin(0.12);
 gPad->SaveAs("hLumiYield.png");

 double mean = func->GetParameter(0);

 for (int i = 1; i < hNUM->GetNbinsX()+1; i++){

    float content = hTRIG->GetBinContent(i);
    float error = hTRIG->GetBinError(i);


    if(content > 1 && fabs(content-mean) > 2*error) cout << "run = " << hNUM->GetBinCenter(i) << " Nevents = " << hNUM->GetBinContent(i) << " lumi = " << hMON->GetBinContent(i) << " pull = " << (content-mean)/error<< endl;

  } 
  
   
                                    
TH1F* hTRIG = (TH1F* ) _file0->Get("h_PFDiJet_EventYield;1");
 TH1F* hNUM = new TH1F();

 TH1F* hMON = new TH1F();
 hTRIG->Copy(*hMON);
 hMON->Scale(0);
 hNUM->Scale(0);
 hTRIG->Copy(*hNUM);

 fillHisto(hMON);

   

 hTRIG->SetMinimum(0);
  //  hTRIG->SetTitle("Event Yield [ p_{T,1}, p_{T,2} > 170, 50 GeV, |#eta_{1,2}|<3, |#Delta #eta| < 3 ] ; Run ; N_{events}/lumi (pb)");

 hTRIG->SetTitleOffset(1.1, "Y");
 hTRIG->SetTitleSize(0.05, "Y");
 hTRIG->SetTitleOffset(0.9, "X");
 hTRIG->SetTitleSize(0.05, "X");

 hTRIG->SetMarkerStyle(20);
 hTRIG->SetMarkerSize(1);

 hTRIG->SetTitle("Dijet events; Run Number; N_{events}");

 hTRIG->Sumw2();

 hTRIG->Draw("P");
 gPad->SetLeftMargin(0.12);
 gPad->SaveAs("hLumiYield_eventsNumber_pf.png");

 TF1* func = new TF1("func", "[0]");
 func->SetLineColor(kRed);

 hTRIG->SetTitle("Dijet events Lumi Yield; Run Number; N_{events}/lumi (pb^{-1})");
 hMON->Sumw2();
 hTRIG->Divide(hMON);
 hTRIG->SetMaximum(hTRIG->GetMaximum()*1.1);
 hTRIG->Draw("P");
 hTRIG->Fit(func);

 gPad->SetLeftMargin(0.12);
 gPad->SaveAs("hLumiYield_pf.png");

 double mean = func->GetParameter(0);

 for (int i = 1; i < hNUM->GetNbinsX()+1; i++){

    float content = hTRIG->GetBinContent(i);
    float error = hTRIG->GetBinError(i);


    if(content-mean > 2*error) cout << "run = " << hNUM->GetBinCenter(i) << " Nevents = " << hNUM->GetBinContent(i) << " lumi = " << hMON->GetBinContent(i) << " pull = " << (content-mean)/error<< endl;

  } 
  









}


void fillHisto(TH1F* Histo){
  
  Histo->Scale(0);
  Histo->Sumw2();
  ///  for (int i = 1; i < Histo->GetNbinsX()+1; i++){

    //    Histo->SetBinContent(i, 1e-10);
  //  }  TH1F* Histo = new TH1F("Histo", "",  150000.5 - 139999.5, 139999.5, 150000.5);
  ifstream in("overallLumiSummary_All.txt", ios::in | ios::binary); 
  ofstream myfile ("LumiOut.txt");
  if(!in || !myfile.is_open()) { 
    cout << "Cannot open file.\n"; 
    return 1; 
  } 
  
  string s;
  while (! in.eof() ){

    getline(in, s); 
  
    cout << s.data() << endl;

    //  TString ss(s);
    TObjArray *tokenizedLine; // information from data lines
    tokenizedLine = TString(s).Tokenize("|");

    if (tokenizedLine->GetEntries() < 5) continue;
    
    if(((TObjString*)tokenizedLine->At(0))->GetString()->Contains("[Warning]")) continue;
   
 
    int run = ((TObjString*)tokenizedLine->At(0))->GetString().Atoi();
    float lumi = ((TObjString*)tokenizedLine->At(4))->GetString().Atof();
    // float lumi = ((TObjString*)tokenizedLine->At(4))->GetString().Atof();

    //  int 
 
    for(int i=0; i<tokenizedLine->GetEntries(); i++) // show values read from file 
      //value[i] = 
      cout << ((TObjString*)tokenizedLine->At(i))->GetString().Data() << endl;//.Atof();    cout << tokenizedLine[i]->Data() << " "; 
  

    
   
    if (run > 1) {
      
      myfile << run << "\t" << lumi << "\n";
      Histo->Fill(run, lumi/1000000);
    }
  }

  in.close();
  myfile.close();
  
  for (int i = 1; i < Histo->GetNbinsX()+1; i++){

    float content = Histo->GetBinContent(i);
    Histo->SetBinError(i, content*0.1);
  } 
  
  //  Histo->Draw();


  cout << "END"<< endl;

 

}
