void DrawJetIdEfficiency(string etaRange  = "TK", 
			 string ptRange   = "pt20to30",
			 string inputdir  = "rootfiles/V00-00-08",
			 string outputdir = "./",
			 bool nvtxRw      = false)
{
  gROOT->Reset();
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1); 
  gStyle->SetOptStat(0);
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
  TH1F *hEff_vs_JetEta[2][3][3]; 
  TH1F *hEff_vs_NumberOfVertices[2][3][3]; 

  TH1F *hRatio_vs_JetPt[3][3]; 
  TH1F *hRatio_vs_JetEta[3][3]; 
  TH1F *hRatio_vs_NumberOfVertices[3][3]; 

  int rebinEta  = 2;
  int rebinPt   = 2;
  int rebinNvtx = 1;


  char hname[100];
  std::string suffLevel[3] = {"Tight","Medium","Loose"};
  std::string suffId[3]    = {"simpleId","fullId","cutbasedId"};
 
  for ( int ifile = 0; ifile < 2 ; ifile++ ){
    for ( int iid = 0; iid < 3 ; iid++ ){
      for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
 	sprintf(hname,"efficiency/hEff_vs_JetPt_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_JetPt[ifile][iid][ilevel] = (TH1F*)f[ifile]->Get(hname);
	hEff_vs_JetPt[ifile][iid][ilevel] ->GetXaxis()->SetTitle("jet p^{T} (GeV)");
	hEff_vs_JetPt[ifile][iid][ilevel] ->GetYaxis()->SetTitle("efficiency");
	hEff_vs_JetPt[ifile][iid][ilevel] ->Rebin(rebinPt);
 	hEff_vs_JetPt[ifile][iid][ilevel] ->Scale(1./rebinPt);

 	sprintf(hname,"efficiency/hEff_vs_JetEta_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_JetEta[ifile][iid][ilevel] = (TH1F*)f[ifile]->Get(hname);
	hEff_vs_JetEta[ifile][iid][ilevel] ->GetXaxis()->SetTitle("jet #eta");
	hEff_vs_JetEta[ifile][iid][ilevel] ->GetYaxis()->SetTitle("efficiency");
	hEff_vs_JetEta[ifile][iid][ilevel] ->Rebin(rebinEta);
 	hEff_vs_JetEta[ifile][iid][ilevel] ->Scale(1./rebinEta);


 	sprintf(hname,"efficiency/hEff_vs_NumberOfVertices_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
	hEff_vs_NumberOfVertices[ifile][iid][ilevel] = (TH1F*)f[ifile]->Get(hname);
	hEff_vs_NumberOfVertices[ifile][iid][ilevel] ->GetXaxis()->SetTitle("number of vertices");
	hEff_vs_NumberOfVertices[ifile][iid][ilevel] ->GetYaxis()->SetTitle("efficiency");

 	if ( ifile == 0 ){
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetLineColor(kPink+1);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetLineColor(kPink+1);
	  hEff_vs_NumberOfVertices[ifile][iid][ilevel]->SetLineColor(kPink+1);
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetMarkerColor(kPink+1);
	  hEff_vs_JetEta[ifile][iid][ilevel]->SetMarkerColor(kPink+1);
	  hEff_vs_NumberOfVertices[ifile][iid][ilevel]->SetMarkerColor(kPink+1);
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetFillColor(kPink+1);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetFillColor(kPink+1);
	  hEff_vs_NumberOfVertices[ifile][iid][ilevel]->SetFillColor(kPink+1);
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetFillStyle(3001);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetFillStyle(3001);
	  hEff_vs_NumberOfVertices[ifile][iid][ilevel]->SetFillStyle(3001);

 	}
 	if ( ifile == 1 ){
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetMarkerStyle(20);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetMarkerStyle(20);
 	  hEff_vs_NumberOfVertices[ifile][iid][ilevel]->SetMarkerStyle(20);
	  hEff_vs_JetPt[ifile][iid][ilevel] ->SetMarkerSize(0.8);
 	  hEff_vs_JetEta[ifile][iid][ilevel]->SetMarkerSize(0.8);
	  hEff_vs_NumberOfVertices[ifile][iid][ilevel]->SetMarkerSize(0.8);
 	}

      }
    }
  }
  
  TLegend *leg = new TLegend(0.5, 0.2, 0.8, 0.35);
  leg ->SetBorderSize(0);
  leg ->SetFillStyle(0);
  leg -> AddEntry(hEff_vs_JetPt[0][0][0],"MC","FL");
  leg -> AddEntry(hEff_vs_JetPt[1][0][0],"DATA","PL");


  TCanvas *cEffJetPt[3][3];
  TCanvas *cEffJetEta[3][3];
  TCanvas *cEffNumberOfVertices[3][3];

  char cnamePt[200];
  char cnameEta[200];
  char cnameNvtx[200];

  for ( int iid = 0; iid < 3 ; iid++ ){
    for ( int ilevel = 0; ilevel < 3 ; ilevel++ ){
   
      // -- vs Pt
      sprintf(cnamePt,"cEff_vs_JetPt_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      cEffJetPt[iid][ilevel] = new TCanvas(cnamePt,cnamePt);
      TPad *cLowerPt  = new TPad("pad_0","pad_0",0.00,0.00,1.00,0.35);
      TPad *cUpperPt  = new TPad("pad_2","pad_2",0.00,0.35,1.00,1.00);
      cLowerPt->SetBottomMargin(0.25); 
      cLowerPt->SetTopMargin(0.1);
      cUpperPt->SetBottomMargin(0.13); 
      cUpperPt->SetTopMargin(0.1);
      cLowerPt->Draw();
      cUpperPt->Draw();
    
      float FontSCF  = cUpperPt->GetHNDC()/cLowerPt->GetHNDC(); 
      float tYoffset = 0.6; 
      float labSize  = 0.06;

      cUpperPt-> cd();
      hEff_vs_JetPt[0][iid][ilevel] ->GetXaxis()->SetLabelSize(labSize);
      hEff_vs_JetPt[0][iid][ilevel] ->GetXaxis()->SetTitleSize(labSize);
      hEff_vs_JetPt[0][iid][ilevel] ->GetYaxis()->SetLabelSize(labSize);
      hEff_vs_JetPt[0][iid][ilevel] ->GetYaxis()->SetTitleSize(labSize);
      hEff_vs_JetPt[0][iid][ilevel] ->GetYaxis()->SetTitleOffset(tYoffset);
      hEff_vs_JetPt[0][iid][ilevel] ->GetXaxis()->SetRangeUser(10.,100.);
      hEff_vs_JetPt[0][iid][ilevel] ->Draw("E4");
      hEff_vs_JetPt[1][iid][ilevel] ->Draw("esame");
      leg->Draw("same");
      
      cLowerPt-> cd();
      cLowerPt-> SetGridy();
      hRatio_vs_JetPt[iid][ilevel] = (TH1F*)hEff_vs_JetPt[1][iid][ilevel] -> Clone();
      hRatio_vs_JetPt[iid][ilevel] ->Divide( hEff_vs_JetPt[0][iid][ilevel] );
      hRatio_vs_JetPt[iid][ilevel] ->SetNdivisions(505);
      hRatio_vs_JetPt[iid][ilevel] ->SetTitle("");
      hRatio_vs_JetPt[iid][ilevel] ->GetXaxis()->SetLabelSize(labSize*FontSCF);
      hRatio_vs_JetPt[iid][ilevel] ->GetXaxis()->SetTitleSize(labSize*FontSCF);
      hRatio_vs_JetPt[iid][ilevel] ->GetYaxis()->SetLabelSize(labSize*FontSCF);
      hRatio_vs_JetPt[iid][ilevel] ->GetYaxis()->SetTitleSize(labSize*FontSCF);
      hRatio_vs_JetPt[iid][ilevel] ->GetYaxis()->SetTitleOffset(tYoffset/FontSCF);
      hRatio_vs_JetPt[iid][ilevel] ->GetXaxis()->SetRangeUser(10.,100.);
      hRatio_vs_JetPt[iid][ilevel] ->GetYaxis()->SetRangeUser(0.7,1.3);
      hRatio_vs_JetPt[iid][ilevel] ->GetYaxis()->SetTitle("data/MC");
      hRatio_vs_JetPt[iid][ilevel] ->Draw("e");
            
      string myname =  (dirName+"/"+cnamePt+"."+fileType).c_str();
      cEffJetPt[iid][ilevel]->Print(myname.c_str(),fileType.c_str());
      

      // -- vs Eta
      sprintf(cnameEta,"cEff_vs_JetEta_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      cEffJetEta[iid][ilevel] = new TCanvas(cnameEta,cnameEta);
     
      TPad *cLowerEta  = new TPad("pad_0","pad_0",0.00,0.00,1.00,0.35);
      TPad *cUpperEta  = new TPad("pad_2","pad_2",0.00,0.35,1.00,1.00);
      cLowerEta->SetBottomMargin(0.25); 
      cLowerEta->SetTopMargin(0.1);
      cUpperEta->SetBottomMargin(0.13); 
      cUpperEta->SetTopMargin(0.1);
      cLowerEta->Draw();
      cUpperEta->Draw();
    
      cUpperEta-> cd();
      hEff_vs_JetEta[0][iid][ilevel] ->GetXaxis()->SetLabelSize(labSize);
      hEff_vs_JetEta[0][iid][ilevel] ->GetXaxis()->SetTitleSize(labSize);
      hEff_vs_JetEta[0][iid][ilevel] ->GetYaxis()->SetLabelSize(labSize);
      hEff_vs_JetEta[0][iid][ilevel] ->GetYaxis()->SetTitleSize(labSize);
      hEff_vs_JetEta[0][iid][ilevel] ->GetYaxis()->SetTitleOffset(tYoffset);
      hEff_vs_JetEta[0][iid][ilevel] ->Draw("E4");
      hEff_vs_JetEta[1][iid][ilevel] ->Draw("esame");
      leg->Draw("same");
      
      cLowerEta-> cd();
      cLowerEta-> SetGridy();
      hRatio_vs_JetEta[iid][ilevel] = (TH1F*)hEff_vs_JetEta[1][iid][ilevel] -> Clone();
      hRatio_vs_JetEta[iid][ilevel] ->Divide( hEff_vs_JetEta[0][iid][ilevel] );
      hRatio_vs_JetEta[iid][ilevel] ->SetNdivisions(505);
      hRatio_vs_JetEta[iid][ilevel] ->SetTitle("");
      hRatio_vs_JetEta[iid][ilevel] ->GetXaxis()->SetLabelSize(labSize*FontSCF);
      hRatio_vs_JetEta[iid][ilevel] ->GetXaxis()->SetTitleSize(labSize*FontSCF);
      hRatio_vs_JetEta[iid][ilevel] ->GetYaxis()->SetLabelSize(labSize*FontSCF);
      hRatio_vs_JetEta[iid][ilevel] ->GetYaxis()->SetTitleSize(labSize*FontSCF);
      hRatio_vs_JetEta[iid][ilevel] ->GetYaxis()->SetTitleOffset(tYoffset/FontSCF);
      hRatio_vs_JetEta[iid][ilevel] ->GetYaxis()->SetRangeUser(0.7,1.3);
      hRatio_vs_JetEta[iid][ilevel] ->GetYaxis()->SetTitle("data/MC");
      hRatio_vs_JetEta[iid][ilevel] ->Draw("e");
      
      string myname2 =  (dirName+"/"+cnameEta+"."+fileType).c_str();
      cEffJetEta[iid][ilevel]->Print(myname2.c_str(),fileType.c_str());


      // -- vs Nvtx
      
      sprintf(cnameNvtx,"cEff_vs_NumberOfVertices_%s_%s", suffId[iid].c_str(), suffLevel[ilevel].c_str());
      cout << cnameNvtx << endl;
      cEffNumberOfVertices[iid][ilevel] = new TCanvas(cnameNvtx,cnameNvtx);
     
      TPad *cLowerNvtx  = new TPad("pad_0","pad_0",0.00,0.00,1.00,0.35);
      TPad *cUpperNvtx  = new TPad("pad_2","pad_2",0.00,0.35,1.00,1.00);
      cLowerNvtx->SetBottomMargin(0.25); 
      cLowerNvtx->SetTopMargin(0.1);
      cUpperNvtx->SetBottomMargin(0.13); 
      cUpperNvtx->SetTopMargin(0.1);
      cLowerNvtx->Draw();
      cUpperNvtx->Draw();
    
      cUpperNvtx-> cd();
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetXaxis()->SetLabelSize(labSize);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetXaxis()->SetTitleSize(labSize);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetYaxis()->SetLabelSize(labSize);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetYaxis()->SetTitleSize(labSize);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetYaxis()->SetTitleOffset(tYoffset);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetXaxis()->SetRangeUser(1,30);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->GetYaxis()->SetRangeUser(0.,1.2);
      hEff_vs_NumberOfVertices[0][iid][ilevel] ->Draw("E4");
      hEff_vs_NumberOfVertices[1][iid][ilevel] ->Draw("esame");
      leg->Draw("same");
      
      cLowerNvtx-> cd();
      cLowerNvtx-> SetGridy();
      hRatio_vs_NumberOfVertices[iid][ilevel] = (TH1F*)hEff_vs_NumberOfVertices[1][iid][ilevel] -> Clone();
      hRatio_vs_NumberOfVertices[iid][ilevel] ->Divide( hEff_vs_NumberOfVertices[0][iid][ilevel] );
      hRatio_vs_NumberOfVertices[iid][ilevel] ->SetNdivisions(505);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->SetTitle("");
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetXaxis()->SetLabelSize(labSize*FontSCF);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetXaxis()->SetTitleSize(labSize*FontSCF);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetYaxis()->SetLabelSize(labSize*FontSCF);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetYaxis()->SetTitleSize(labSize*FontSCF);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetYaxis()->SetTitleOffset(tYoffset/FontSCF);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetXaxis()->SetRangeUser(0.,30);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetYaxis()->SetRangeUser(0.7,1.3);
      hRatio_vs_NumberOfVertices[iid][ilevel] ->GetYaxis()->SetTitle("data/MC");
      hRatio_vs_NumberOfVertices[iid][ilevel] ->Draw("e");
      
      string myname3 =  (dirName+"/"+cnameNvtx+"."+fileType).c_str();
      cEffNumberOfVertices[iid][ilevel]->Print(myname3.c_str(),fileType.c_str());


    }
  }
  

}
