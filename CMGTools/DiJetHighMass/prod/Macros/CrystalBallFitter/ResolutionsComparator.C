#include <iostream>
#include <fstream>

TObjArray* aMu = new TObjArray(6);
TObjArray* aSigma = new TObjArray(6);
TObjArray* aRMS = new TObjArray(6);

void ResolutionsComparator(){

  double* RMS = new double[6];
  double* Mass = new double[6];
  double* mu = new double[6];
  double* err_mu = new double[6];
  double* sigma = new double[6];
  double* err_sigma = new double[6];
  double err_mass[6] = {0.01, 0.01, 0.01, 0.01, 0.01, 0.01};

  int nBins = 6;

  for (int j = 0; j < 6; j++){

	string sIn("");

	switch(j){
	case 0: sIn = "histograms_Fat30_mc_RSGravitonToJJ_M-AllMasses_TuneD6T_ak5QQtoQQ_Fat30.txt"; nBins = 6; break;
	case 1: sIn = "histograms_Fat30_mc_QstarToJJ_M-AllMasses_TuneD6T_ak5_Fat30.txt"; nBins = 6; break;
	case 2: sIn = "histograms_Fat30_mc_RSGravitonToJJ_M-AllMasses_TuneD6T_ak5GGtoGG_Fat30.txt"; nBins = 5; break;
	case 3: sIn = "histograms_Fat30_mc_RSGravitonToJJ_M-AllMasses_TuneD6T_ak7QQtoQQ_pf.txt"; nBins = 6; break;
	case 4: sIn = "histograms_Fat30_mc_QstarToJJ_M-AllMasses_TuneD6T_ak7_pf.txt"; nBins = 6; break;
	case 5: sIn = "histograms_Fat30_mc_RSGravitonToJJ_M-AllMasses_TuneD6T_ak7GGtoGG_pf.txt"; nBins = 5; break;
	default: break;
	}

  	ifstream myfile (sIn.c_str());

	cout << sIn.c_str() << endl;

	for (int i = 0; i < nBins; i++) {
	  myfile >> Mass[i] >> mu[i] >> err_mu[i] >> sigma[i] >> err_sigma[i] >> RMS[i];
	  //	  mu[i] = mu[i]/Mass[i]; err_mu[i] = err_mu[i]/Mass[i];
	  cout <<  Mass[i] << " " <<  mu[i] << " " << err_mu[i] << " " << sigma[i] << " " << err_sigma[i] << " " << "RMS " << RMS[i] << endl;
	  mu[i] = mu[i]; err_mu[i] = err_mu[i];
	  cout << mu[i] << " " << err_mu[i] << endl;
	}

  	myfile.close();

	TGraphErrors* gMu = new TGraphErrors(nBins, Mass, mu, err_mass, err_mu);
  	TGraphErrors* gSigma = new TGraphErrors(nBins, Mass, sigma, err_sigma, err_mu);
  	TGraphErrors* gRMS = new TGraphErrors(nBins, Mass, RMS, err_sigma, err_mu);

	gMu->SetMarkerSize(1);
	gMu->SetMarkerColor(j%3+1);
	gMu->SetMarkerStyle(20+j%3);
	gMu->SetLineColor(j%3+1);
	//	gMu->Draw("AP");
	gSigma->SetMarkerSize(1);
	gSigma->SetMarkerColor(j%3+1);
	gSigma->SetMarkerStyle(20+j%3);
	gSigma->SetLineColor(j%3+1);

	gRMS->SetMarkerSize(1);
	gRMS->SetMarkerColor(j%3+1);
	gRMS->SetMarkerStyle(20+j%3);
	gRMS->SetLineColor(j%3+1);

	aMu->AddAt(gMu, j);
	aSigma->AddAt(gSigma, j);
	aRMS->AddAt(gRMS, j);
	
  }

  //  TGraphErrors* gSigma = (TGraphErrors*) aSigma->At(0);
  //  gSigma->Draw("AP");

  TCanvas* C = new TCanvas("C", "canvas", 1000., 500.);
  C->Divide(2,1);


  for (int iDet = 0; iDet < 2; iDet++){

    C->cd(iDet+1);
  
    TH1F *vFrame2 = gPad->DrawFrame(400.0,0.0,4200.,0.19);
    vFrame2->SetTitle("Crystal ball gaussian core resolution");
    vFrame2->SetTitleSize(0.2);
    vFrame2->SetXTitle("Resonance mass (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);

    vFrame2->SetYTitle("#sigma/#mu");
  
    vFrame2->Draw();

    //    gPad->SetLogx(1);

    for (int j = iDet*3; j < (iDet+1)*3; j++){
    
      TGraphErrors* gSigma = (TGraphErrors*) aSigma->At(j);
      if (j==0) gSigma->DrawClone("SAMEP");
      else gSigma->DrawClone("SAMEP");

      TLegend* Legend = new TLegend(0.40, 0.55, 0.85, 0.88);

    }

    if (iDet == 0) {
      Legend->AddEntry((TGraphErrors*) aSigma->At(0 + 3*iDet), "qq->RS->qq: AK5 Wide jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(1 + 3*iDet), "qg->Q*->qg: AK5 Wide jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(2 + 3*iDet), "gg->RS->gg: AK5 Wide jets", "p");
    } else if (iDet == 1) {
      Legend->AddEntry((TGraphErrors*) aSigma->At(0 + 3*iDet), "qq->RS->qq: AK7 jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(1 + 3*iDet), "qg->Q*->qg: AK7 jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(2 + 3*iDet), "gg->RS->gg: AK7 jets", "p");
    }
    

   
    Legend->Draw();

  }
  /*
  for (int iDet = 0; iDet < 2; iDet++){

    C->cd(iDet+3);
  
    TH1F *vFrame = gPad->DrawFrame(400.0,0.1,4200.,0.30);
    vFrame->SetTitle("RMS of the distribution");
    vFrame->SetTitleSize(0.2);
    vFrame->SetXTitle("Resonance mass (GeV)");
    vFrame->GetYaxis()->SetLabelSize(0.03);
    vFrame->GetYaxis()->SetTitleSize(0.04);
    vFrame->GetYaxis()->SetTitleOffset(1.2);
    vFrame->GetXaxis()->SetLabelSize(0.03);
    vFrame->GetXaxis()->SetTitleSize(0.04);
    vFrame->GetXaxis()->SetTitleOffset(0.95);

    vFrame->SetYTitle("RMS/#mu");
  
    vFrame->Draw();

    //    gPad->SetLogx(1);

    for (int j = iDet*3; j < (iDet+1)*3; j++){
    
      TGraphErrors* gRMS = (TGraphErrors*) aRMS->At(j);
      gRMS->DrawClone("SAMEP");

      TLegend* Legend = new TLegend(0.50, 0.55, 0.85, 0.88);

    }

    if (iDet == 0) {
      Legend->AddEntry((TGraphErrors*) aSigma->At(0 + 3*iDet), "qq->RS->qq: AK5 Wide jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(1 + 3*iDet), "qg->Q*->qg: AK5 Wide jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(2 + 3*iDet), "gg->RS->gg: AK5 Wide jets", "p");
    } else if (iDet == 1) {
      Legend->AddEntry((TGraphErrors*) aSigma->At(0 + 3*iDet), "qq->RS->qq: AK7 jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(1 + 3*iDet), "qg->Q*->qg: AK7 Wide jets", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(2 + 3*iDet), "gg->RS->gg: AK7 Wide jets", "p");
    }
    

   
    Legend->Draw();

  }

  */


  C->SaveAs("histograms_Fat30_mc_AllShapes_AllMasses_TuneD6T.png"); 
  

}
