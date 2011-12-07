
TH1F* changeBinning(TH1F* hinput, int rebin,float xmax){
  int nbins=(int)(xmax);
  TString hname=hinput->GetName();
  TH1F* houtput=new TH1F("htmp",hinput->GetTitle(),nbins,0,xmax);
  for(Int_t b=1;b<nbins;b++){
    houtput->SetBinContent(b,hinput->GetBinContent(b));
    houtput->SetBinError(b,hinput->GetBinError(b));
  }
  delete hinput;

  houtput->Rebin(rebin);
  houtput->SetName(hname);
  return houtput;
}

void histosForLimit(){
  //defines samples
  gROOT->ProcessLine(".x ../workdir/TauMuConfig.C");
  
  //defines input histograms
  analysis.setOutputPath("/afs/cern.ch/user/b/benitezj/public/V240AB");

  //normalizes backgrounds for the data
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}  
  if(!analysis.scaleHistos())return 0;
  

  /* ***************************************
    -access backgrounds like this (updated Dec 2)
    -everything should be obtained from class interface/TauMuAnalysis.h
    -individual backgrounds obtained using getSample(), except for ZtoTauTau and QCD 
    -methods return ownership of object --> delete objects
    ****************************************** 
    analysis.getTotalDataSS(TString histoname);//sum of SS Data samples 
    analysis.getTotalData(TString histoname);
    analysis.getSample(TString samplename, TString histoname);//can be used to get histo for any MC or Data sample
    analysis.getQCD(TString histoname);
    analysis.getZToTauTau(TString histoname);//Z-->tau tau (either from MC or Embedded)
    analysis.getTotalBackground(TString histoname);
  */




  //histogram used for fit in limit calcualtion
  TString histoname="diTauMassHisto_SM0";//"diTauMassHisto_SM1" "diTauMassHisto_SM2"
  

  TCanvas C("Canvas");
  C.Print("plots.ps[");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("HiggsGG115",histoname));
  TH1F* SM115 =(TH1F*)(htmp->Clone("SM115"));//rename otherwise default name may conflict below
  SM115=changeBinning(SM115,10,300);//use 10GeV wide bins from 0-->300
  delete htmp;
  C.Clear();
  SM115->SetTitle("SM115");
  SM115->Draw();
  C.Print("plots.ps");
  
//   TH1F* SM115_CMS_scale_tUp = (TH1F*)SM115->Clone("SM115_CMS_scale_tUp");
//   TH1F* SM115_CMS_scale_tDown = (TH1F*)SM115->Clone("SM115_CMS_scale_tDown");
//   TH1F* SM115_CMS_scale_jUp = (TH1F*)SM115->Clone("SM115_CMS_scale_jUp");
//   TH1F* SM115_CMS_scale_jDown = (TH1F*)SM115->Clone("SM115_CMS_scale_jDown");

  TH1F* htmp = (TH1F*)(analysis.getSample("HiggsVBF115",histoname));
  TH1F* VBF115 =(TH1F*)(htmp->Clone("VBF115"));//rename otherwise default name may conflict below
  VBF115=changeBinning(VBF115,10,300);
  delete htmp;
  C.Clear();
  VBF115->SetTitle("VBF115");
  VBF115->Draw();
  C.Print("plots.ps");
  
//   TH1F* VBF115_CMS_scale_tUp = (TH1F*)VBF115->Clone("VBF115_CMS_scale_tUp");
//   TH1F* VBF115_CMS_scale_tDown = (TH1F*)VBF115->Clone("VBF115_CMS_scale_tDown");
//   TH1F* VBF115_CMS_scale_jUp = (TH1F*)VBF115->Clone("VBF115_CMS_scale_jUp");
//   TH1F* VBF115_CMS_scale_jDown = (TH1F*)VBF115->Clone("VBF115_CMS_scale_jDown");

  
  TH1F* htmp = (TH1F*)(analysis.getZToTauTau(histoname));
  TH1F* ZTT =(TH1F*)(htmp->Clone("ZTT"));//rename otherwise default name may conflict below
  ZTT=changeBinning(ZTT,10,300);
  delete htmp;
  C.Clear();
  ZTT->SetTitle("ZTT");
  ZTT->Draw();
  C.Print("plots.ps");
  
//   TH1F* ZTT_CMS_scale_tUp = (TH1F*)ZTT->Clone("ZTT_CMS_scale_tUp");
//   TH1F* ZTT_CMS_scale_tDown = (TH1F*)ZTT->Clone("ZTT_CMS_scale_tDown");
//   TH1F* ZTT_CMS_scale_jUp = (TH1F*)ZTT->Clone("ZTT_CMS_scale_jUp");
//   TH1F* ZTT_CMS_scale_jDown = (TH1F*)ZTT->Clone("ZTT_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZToMuMu",histoname));
  TH1F* ZL =(TH1F*)(htmp->Clone("ZL"));//rename otherwise default name may conflict below
  ZL=changeBinning(ZL,10,300);
  delete htmp;
  C.Clear();
  ZL->SetTitle("ZL");
  ZL->Draw();
  C.Print("plots.ps");
  
//   TH1F* ZL_CMS_scale_tUp = (TH1F*)ZL->Clone("ZL_CMS_scale_tUp");
//   TH1F* ZL_CMS_scale_tDown = (TH1F*)ZL->Clone("ZL_CMS_scale_tDown");
//   TH1F* ZL_CMS_scale_jUp = (TH1F*)ZL->Clone("ZL_CMS_scale_jUp");
//   TH1F* ZL_CMS_scale_jDown = (TH1F*)ZL->Clone("ZL_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZToLJet",histoname));
  TH1F* ZJ =(TH1F*)(htmp->Clone("ZJ"));//rename otherwise default name may conflict below
  ZJ=changeBinning(ZJ,10,300);
  delete htmp;
  C.Clear();
  ZJ->SetTitle("ZJ");
  ZJ->Draw();
  C.Print("plots.ps");
  
//   TH1F* ZJ_CMS_scale_tUp = (TH1F*)ZJ->Clone("ZJ_CMS_scale_tUp");
//   TH1F* ZJ_CMS_scale_tDown = (TH1F*)ZJ->Clone("ZJ_CMS_scale_tDown");
//   TH1F* ZJ_CMS_scale_jUp = (TH1F*)ZJ->Clone("ZJ_CMS_scale_jUp");
//   TH1F* ZJ_CMS_scale_jDown = (TH1F*)ZJ->Clone("ZJ_CMS_scale_jDown");

  TH1F* htmp = (TH1F*)(analysis.getSample("WJetsToLNu",histoname));
  TH1F* W =(TH1F*)(htmp->Clone("W"));//rename otherwise default name may conflict below
  W=changeBinning(W,10,300);
  delete htmp;
  C.Clear();
  W->SetTitle("W");
  W->Draw();
  C.Print("plots.ps");

//   TH1F* W_CMS_scale_tUp = (TH1F*)W->Clone("W_CMS_scale_tUp");
//   TH1F* W_CMS_scale_tDown = (TH1F*)W->Clone("W_CMS_scale_tDown");
//   TH1F* W_CMS_scale_jUp = (TH1F*)W->Clone("W_CMS_scale_jUp");
//   TH1F* W_CMS_scale_jDown = (TH1F*)W->Clone("W_CMS_scale_jDown");


  TH1F* htmp = (TH1F*)(analysis.getSample("TTJets",histoname));
  TH1F* TT =(TH1F*)(htmp->Clone("TT"));//rename otherwise default name may conflict below
  TT=changeBinning(TT,10,300);
  delete htmp;
  C.Clear();
  TT->SetTitle("TT");
  TT->Draw();
  C.Print("plots.ps");
  
//   TH1F* TT_CMS_scale_tUp = (TH1F*)TT->Clone("TT_CMS_scale_tUp");
//   TH1F* TT_CMS_scale_tDown = (TH1F*)TT->Clone("TT_CMS_scale_tDown");
//   TH1F* TT_CMS_scale_jUp = (TH1F*)TT->Clone("TT_CMS_scale_jUp");
//   TH1F* TT_CMS_scale_jDown = (TH1F*)TT->Clone("TT_CMS_scale_jDown");
    

  TH1F* htmp = (TH1F*)(analysis.getSample("WW",histoname));
  TH1F* WW =(TH1F*)(htmp->Clone("WW"));//rename otherwise default name may conflict below
  WW=changeBinning(WW,10,300);
  delete htmp;
  C.Clear();
  WW->SetTitle("WW");
  WW->Draw();
  C.Print("plots.ps");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("WZ",histoname));
  TH1F* WZ =(TH1F*)(htmp->Clone("WZ"));//rename otherwise default name may conflict below
  WZ=changeBinning(WZ,10,300);
  delete htmp;
  C.Clear();
  WZ->SetTitle("WZ");
  WZ->Draw();
  C.Print("plots.ps");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZZ",histoname));
  TH1F* ZZ =(TH1F*)(htmp->Clone("ZZ"));//rename otherwise default name may conflict below
  ZZ=changeBinning(ZZ,10,300);
  delete htmp;
  C.Clear();
  ZZ->SetTitle("ZZ");
  ZZ->Draw();
  C.Print("plots.ps");
  
  TH1F* VV =(TH1F*)(WW->Clone("VV"));
  VV->Add(WZ);
  VV->Add(ZZ);
  
  TH1F* htmp=analysis.getQCD(histoname);
  TH1F* QCD=(TH1F*)(htmp->Clone("QCD"));//rename otherwise default name may conflict below
  QCD=changeBinning(QCD,10,300);
  delete htmp;
  C.Clear();
  QCD->SetTitle("QCD");
  QCD->Draw();
  C.Print("plots.ps");

  
  TH1F* htmp=analysis.getTotalBackground(histoname);
  TH1F* bkg_obs = (TH1F*)(htmp->Clone("bkg_obs"));//rename otherwise default name may conflict below
  bkg_obs=changeBinning(bkg_obs,10,300);
  delete htmp;
  
  TH1F* htmp=analysis.getTotalData(histoname);
  TH1F* data_obs = (TH1F*)(htmp->Clone("data_obs"));//rename otherwise default name may conflict below
  data_obs=changeBinning(data_obs,10,300);
  delete htmp;

  C.Clear();
  data_obs->Draw("pe");
  bkg_obs->Draw("histsame");
  C.Print("plots.ps");
  cout<<" Data yield = "<<data_obs->Integral()<<"  Background yield = "<<bkg_obs->Integral()<<endl;


  TH1F* htmp=analysis.getTotalDataSS(histoname);
  TH1F* data_obsSS=(TH1F*)(htmp->Clone("data_obsSS"));//rename otherwise default name may conflict below
  data_obsSS=changeBinning(data_obsSS,10,300);
  delete htmp;
  C.Clear();
  data_obsSS->SetTitle("data_obsSS");
  data_obsSS->Draw();
  C.Print("plots.ps");

  C.Print("plots.ps]");


  ////////////////Histograms file

  char rootfilename[256];
  sprintf(rootfilename, "muTau.root");  
  TFile output(rootfilename,"recreate");
  TDirectory* dir = output.mkdir("muTau_SM0");
  
  dir->cd();
  SM115->Write();
//   SM115_CMS_scale_tUp   ->Write();
//   SM115_CMS_scale_tDown ->Write();
//   SM115_CMS_scale_jUp   ->Write();
//   SM115_CMS_scale_jDown ->Write();

  VBF115->Write();
//   VBF115_CMS_scale_tUp   ->Write();
//   VBF115_CMS_scale_tDown ->Write();
//   VBF115_CMS_scale_jUp   ->Write();
//   VBF115_CMS_scale_jDown ->Write();

  ZTT ->Write();
//   ZTT_CMS_scale_tUp   ->Write();
//   ZTT_CMS_scale_tDown ->Write();
//   ZTT_CMS_scale_jUp   ->Write();
//   ZTT_CMS_scale_jDown ->Write();

  ZL->Write();
//   ZL_CMS_scale_tUp   ->Write();
//   ZL_CMS_scale_tDown ->Write();
//   ZL_CMS_scale_jUp   ->Write();
//   ZL_CMS_scale_jDown ->Write();

  ZJ->Write();
//   ZJ_CMS_scale_tUp   ->Write();
//   ZJ_CMS_scale_tDown ->Write();
//   ZJ_CMS_scale_jUp   ->Write();
//   ZJ_CMS_scale_jDown ->Write();

  W->Write();
//   W_CMS_scale_tUp   ->Write();
//   W_CMS_scale_tDown ->Write();
//   W_CMS_scale_jUp   ->Write();
//   W_CMS_scale_jDown ->Write();

  TT->Write();
//   TT_CMS_scale_tUp   ->Write();
//   TT_CMS_scale_tDown ->Write();
//   TT_CMS_scale_jUp   ->Write();
//   TT_CMS_scale_jDown ->Write();

  VV->Write();

  QCD->Write();

  data_obs->Write();

  output.Close();


  
  //////////////////////////////////////Data Card
  ofstream file;
  file.open("muTau_SM0.txt");

  file << "imax 1" <<endl;
  file << "jmax *" <<endl;
  file << "kmax *" <<endl;
  file << "shapes *  *  "<< rootfilename << "  $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  file << endl;
  file << "observation " << data_obs->Integral()<<endl;
  file<<endl;

  file << "bin muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0" << endl;
  file << "process SM115 VBF115 ZTT QCD W ZJ ZL TT VV" << endl;
  file << "process -1 0 1 2 3 4 5 6 7" << endl;
  file << "rate " << SM115->Integral() << " "<< VBF115->Integral()  << " " << ZTT->Integral() << " " << QCD->Integral() << " " << W->Integral() 
       << " " << ZJ->Integral() << " " << ZL->Integral() << " " << TT->Integral() << " " << VV->Integral()  << endl;
  file << endl;

  file <<"-------------------------------------------------------"<<endl;
  file <<"lumi lnN 1.045 1.045 - - 1.045 1.045 1.045 1.045 1.045 luminosity"<<endl;
  file <<"CMS_htt_zttNorm lnN - - 1.033 - - 1.033 1.033 - - ZTT Scale"<<endl;

  file <<"CMS_htt_muTau_SM0_QCDNorm  gmN  "<<(int)(QCD->Integral()/analysis.getQCDOStoSSRatio())<<"     -          -              -                  "<<analysis.getQCDOStoSSRatio()<<"                   -                    -                     -                    -           -     QCD Background"<<endl;
  file <<"CMS_htt_muTau_SM0_WNorm    gmN "<<(int)(W->Integral()/analysis.getWJetsSignalToSBFraction())<<"     -         -               -                    -                   "<<analysis.getWJetsSignalToSBFraction()<<"              -                     -                    -        -       W Background"<<endl;


//   file << "bin                                   muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0          muTau_SM0" << endl;
//   file << "process                                SM115              ZTT                QCD                 W                  ZJ                 ZL                 TT" << endl;
//   file << "process                                  0                 1                  2                  3                  4                  5                  6" << endl;
//   file << "rate                                     " << SM115->Integral() << "              " << ZTT->Integral() << "             " << QCD->Integral() << "            " << W->Integral() << "             " << ZJ->Integral() << "             " << ZL->Integral() << "            " << TT->Integral() << endl;
//   file << endl;
//   file <<"--------------------------------------------------------------------------------------------------------------"<<endl;
//   file <<"lumi                             lnN   1.045                     -                     -                     -                    -                     -                    -               luminosity"<<endl;
//   file <<"CMS_eff_m                        lnN   1.020                   1.020                   -                     -                  1.020                 1.020                1.020             muon ID /HLT"<<endl;
//   file <<"CMS_eff_t                        lnN   1.060                   1.060                   -                     -                    -                     -                  1.060             Tau IDf"<<endl;
//   file <<"CMS_scale_t                    shape   1.000                   1.000                   -                   1.000                1.000                 1.000                1.000             shape"<<endl;
//   file <<"CMS_htt_zttNorm                  lnN     -                     1.033                   -                     -                  1.033                 1.033                  -               ZTT Scale"<<endl;
//   file <<"CMS_htt_ttbarNorm                lnN     -                        -                    -                     -                    -                     -                  1.12              TTbar background"<<endl;
//   file <<"CMS_htt_DiBosonNorm              lnN     -                        -                    -                     -                    -                     -                    -               DiBoson background"<<endl;
//  file <<"CMS_htt_muTau_SM0_QCDNorm  gmN  "<<QCD->Integral()/analysis.getQCDOStoSSRatio()<<"     -                        -                   1.11                   -                    -                     -                    -               QCD Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_QCDSyst        lnN     -                        -                   1.02                   -                    -                     -                    -               QCD Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_WNorm    gmN "<<W->Integral()/analysis.getWJetsSignalToSBFraction()<<"     -                        -                    -                   0.317277              -                     -                    -               W Background"<<endl;
//   file <<"CMS_htt_muTau_SM0_WSyst          lnN     -                        -                    -                   1.06                   -                     -                    -               W Background" <<endl;
//   file <<"CMS_htt_muTau_ZJetFakeTau        lnN     -                        -                    -                     -                  1.12                    -                    -               Z(jet->tau) background"<<endl;
//   file <<"CMS_htt_muTau_ZLeptonFakeTau     lnN     -                        -                    -                     -                    -                   1.258                  -               Z(l->tau)   background"<<endl;
//   file <<"CMS_scale_j                    shape    1.0                     1.0                    -                   1.0                  1.0                   1.0                  1.0              shape and norm from jets"<<endl;
//   file <<"pdf_qqbar                        lnN     -                        -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"pdf_gg                           lnN   1.03                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"QCDscale_ggH                     lnN   1.12                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"QCDscale_qqH                     lnN      -                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;
//   file <<"UEPS                             lnN   0.96                       -                    -                     -                    -                     -                    -               PDF VBF"<<endl;

  file.close();
  
}
