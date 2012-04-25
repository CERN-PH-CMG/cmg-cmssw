void DrawJetIdEfficiency(string etaRange  = "TK", 
			 string ptRange   = "pt20to30",
			 string inputdir  = "rootfiles/V00-00-08",
			 string outputdir = "./",
			 bool nvtxRw      = false)
{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1); 
  gStyle->SetOptStat(1110);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleBorderSize(0);
   
  string infile1 = inputdir+"/histos_DYJetsToLL_pfjets_"+etaRange+"_"+ptRange+".root";
  string infile2 = inputdir+"/histos_DoubleMu2011_pfjets_"+etaRange+"_"+ptRange+".root";
  string dirName = outputdir+"/"+etaRange+"_"+ptRange;
  string fileType = "png";

  if (nvtxRw){
    infile1 = inputdir+"/histos_DYJetsToLL_pfjets_"+etaRange+"_"+ptRange+"_reweightedNvtx.root";
    dirName = outputdir+"/"+etaRange+"_"+ptRange+"_reweightedNvtx";
  }
  
  gSystem->mkdir(dirName.c_str(),true);

  cout << "Producing efficiency plots for: " << infile1 << " and " << infile2 << endl;

  
  TFile* f[2]; 
  f[0] = new TFile(infile1.c_str()); //MC
  f[1] = new TFile(infile2.c_str()); //DATA

  TH1F *hEff_vs_JetPt[2][3][3]; 
  TH1F *hEff_vs_JetPt[2][3][3]; 
  TH1F *hEff_vs_JetPt_philv1Id[2][3][3]; 

  TH1F *hEff_vs_JetEta[2][3][3]; 
  TH1F *hEff_vs_JetEta[2][3][3]; 
  TH1F *hEff_vs_JetEta[2][3][3]; 
  
  char hname[100];
  std::string suffLevel[3] = {"Tight","Medium","Loose"};
  std::string suffId[3]    = {"simpleId","fullId","philv1Id"};
 
  for ( int ifile = 0; ifile < 2 ; ifile++ ){
    for ( int iid = 0; iid < 3 ; iid++ ){
      for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
 	sprintf(hname,"efficiency/hEff_vs_JetPt_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_JetPt[ifile][iid][ilevel] = (TH1F*)f[ifile]->Get(hname);
	hEff_vs_JetPt[ifile][iid][ilevel] ->GetXaxis()->SetTitle("jet p^{T} (GeV)");
	hEff_vs_JetPt[ifile][iid][ilevel] ->GetYaxis()->SetTitle("efficiency");

 	sprintf(hname,"efficiency/hEff_vs_JetEta_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_JetEta[ifile][iid][ilevel] = (TH1F*)f[ifile]->Get(hname);
	hEff_vs_JetEta[ifile][iid][ilevel] ->GetXaxis()->SetTitle("jet #eta");
	hEff_vs_JetEta[ifile][iid][ilevel] ->GetYaxis()->SetTitle("efficiency");

 	if ( ifile == 0 ){
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetLineColor(2);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetLineColor(2);
 	}
 	if ( ifile == 1 ){
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetMarkerStyle(20);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetMarkerStyle(20);
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetMarkerSize(0.8);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetMarkerSize(0.8);
 	}

      }
    }
  }
  
  TLegend *leg = new TLegend(0.5, 0.2, 0.8, 0.35);
  leg ->SetBorderSize(0);
  leg ->SetFillStyle(0);
  leg -> AddEntry(hEff_vs_JetPt[0][0][0],"MC","L");
  leg -> AddEntry(hEff_vs_JetPt[1][0][0],"DATA","PL");


  TCanvas *cEffJetPt[3][3];
  TCanvas *cEffJetEta[3][3];

  char cname[100];

  for ( int iid = 0; iid < 3 ; iid++ ){
    for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
   
      sprintf(cname,"cEff_vs_JetPt_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      
      cEffJetPt[iid][ilevel] = new TCanvas(cname,cname);
      cEffJetPt[iid][ilevel]->cd();
      hEff_vs_JetPt[0][iid][ilevel] ->Draw("histo");
      hEff_vs_JetPt[1][iid][ilevel] ->Draw("esame");
      leg->Draw("same");
      string myname =  (dirName+"/"+cname+"."+fileType).c_str();
      cEffJetPt[iid][ilevel]->Print(myname.c_str(),fileType.c_str());
          
      sprintf(cname,"cEff_vs_JetEta_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      cEffJetEta[iid][ilevel] = new TCanvas(cname,cname);
      cEffJetEta[iid][ilevel]->cd();
      hEff_vs_JetEta[0][iid][ilevel] ->Draw("histo");
      hEff_vs_JetEta[1][iid][ilevel] ->Draw("esame");
      leg->Draw("same");

      string myname2 =  (dirName+"/"+cname+"."+fileType).c_str();
      cEffJetEta[iid][ilevel]->Print(myname2.c_str(),fileType.c_str());


    }
  }
  

}
