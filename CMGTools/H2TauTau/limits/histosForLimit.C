void histosForLimit(){
  //defines samples
  gROOT->ProcessLine(".x ../workdir/TauMuConfig.C");
  
  //defines input histograms
  analysis.setOutputPath("/afs/cern.ch/user/b/benitezj/public/V240VVHiggs");

  //normalizes backgrounds for the data
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}  
  if(!analysis.scaleHistos())return 0;
  

  /* ***************************************
    -access backgrounds like this (updated Dec 2)
    -everything should be obtained from class interface/TauMuAnalysis.h
    -individual backgrounds obtained using getSample(), except for ZtoTauTau and QCD 
    -methods return ownership of object
    ****************************************** 
    analysis.getTotalDataSS(TString histoname);//sum of SS Data samples 
    analysis.getTotalData(TString histoname);
    analysis.getSample(TString samplename, TString histoname);//can be used to get histo for any MC or Data sample
    analysis.getQCD(TString histoname);
    analysis.getZToTauTau(TString histoname);//Z-->tau tau (either from MC or Embedded)
    analysis.getTotalBackground(TString histoname);
  */




  char rootfilename[256];
  sprintf(rootfilename, "muTau.root");
  

  //histogram used for fit in limit calcualtion
  TString histoname="diTauMassHisto";
  

  TCanvas C("Canvas");
  C.Print("plots.ps[");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("HiggsGG115",histoname));
  TH1F* SM115_gg =(TH1F*)(htmp->Clone("SM115_gg"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  SM115_gg->Draw();
  C.Print("plots.ps");
  
  TH1F* SM115 = (TH1F*)SM115_gg->Clone("SM115");
  TH1F* SM115_CMS_scale_tUp = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_tUp");
  TH1F* SM115_CMS_scale_tDown = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_tDown");
  TH1F* SM115_CMS_scale_jUp = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_jUp");
  TH1F* SM115_CMS_scale_jDown = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getZToTauTau(histoname));
  TH1F* ZTauTau =(TH1F*)(htmp->Clone("ZTauTau"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  ZTauTau->Draw();
  C.Print("plots.ps");
  
  TH1F* ZTT = (TH1F*)ZTauTau->Clone("ZTT");
  TH1F* ZTT_CMS_scale_tUp = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_tUp");
  TH1F* ZTT_CMS_scale_tDown = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_tDown");
  TH1F* ZTT_CMS_scale_jUp = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_jUp");
  TH1F* ZTT_CMS_scale_jDown = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZToMuMu",histoname));
  TH1F* ZMuMu =(TH1F*)(htmp->Clone("ZMuMu"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  ZMuMu->Draw();
  C.Print("plots.ps");
  
  TH1F* ZL = (TH1F*)ZMuMu->Clone("ZL");
  TH1F* ZL_CMS_scale_tUp = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_tUp");
  TH1F* ZL_CMS_scale_tDown = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_tDown");
  TH1F* ZL_CMS_scale_jUp = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_jUp");
  TH1F* ZL_CMS_scale_jDown = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_jDown");
  
  TH1F* htmp = (TH1F*)(analysis.getSample("ZToLJet",histoname));
  TH1F* ZJets =(TH1F*)(htmp->Clone("ZJets"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  ZJets->Draw();
  C.Print("plots.ps");
  
  TH1F* ZJ = (TH1F*)ZJets->Clone("ZJ");
  TH1F* ZJ_CMS_scale_tUp = (TH1F*)ZJets->Clone("ZJ_CMS_scale_tUp");
  TH1F* ZJ_CMS_scale_tDown = (TH1F*)ZJets->Clone("ZJ_CMS_scale_tDown");
  TH1F* ZJ_CMS_scale_jUp = (TH1F*)ZJets->Clone("ZJ_CMS_scale_jUp");
  TH1F* ZJ_CMS_scale_jDown = (TH1F*)ZJets->Clone("ZJ_CMS_scale_jDown");

  TH1F* htmp = (TH1F*)(analysis.getSample("WJetsToLNu",histoname));
  TH1F* WJets =(TH1F*)(htmp->Clone("WJets"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  WJets->Draw();
  C.Print("plots.ps");

  TH1F* W = (TH1F*)WJets->Clone("W");
  TH1F* W_CMS_scale_tUp = (TH1F*)WJets->Clone("W_CMS_scale_tUp");
  TH1F* W_CMS_scale_tDown = (TH1F*)WJets->Clone("W_CMS_scale_tDown");
  TH1F* W_CMS_scale_jUp = (TH1F*)WJets->Clone("W_CMS_scale_jUp");
  TH1F* W_CMS_scale_jDown = (TH1F*)WJets->Clone("W_CMS_scale_jDown");


  TH1F* htmp = (TH1F*)(analysis.getSample("TTJets",histoname));
  TH1F* TTJ =(TH1F*)(htmp->Clone("TTJ"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  TTJ->Draw();
  C.Print("plots.ps");
  
  TH1F* TT = (TH1F*)TTJ->Clone("TT");
  TH1F* TT_CMS_scale_tUp = (TH1F*)TTJ->Clone("TT_CMS_scale_tUp");
  TH1F* TT_CMS_scale_tDown = (TH1F*)TTJ->Clone("TT_CMS_scale_tDown");
  TH1F* TT_CMS_scale_jUp = (TH1F*)TTJ->Clone("TT_CMS_scale_jUp");
  TH1F* TT_CMS_scale_jDown = (TH1F*)TTJ->Clone("TT_CMS_scale_jDown");
  
  
  TH1F* htmp=analysis.getTotalData(histoname);
  TH1F* data_obs = (TH1F*)(htmp->Clone("data_obs"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  data_obs->Draw();
  C.Print("plots.ps");


  TH1F* htmp=analysis.getTotalDataSS(histoname);
  TH1F* data_obsSS=(TH1F*)(htmp->Clone("data_obsSS"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  data_obsSS->Draw();
  C.Print("plots.ps");
  
  TH1F* htmp=analysis.getQCD(histoname);
  TH1F* QCD_b=(TH1F*)(htmp->Clone("QCD_b"));//rename otherwise default name may conflict below
  delete htmp;
  C.Clear();
  QCD_b->Draw();
  C.Print("plots.ps");

  TH1F* QCD = (TH1F*)QCD_b->Clone("QCD");

  C.Print("plots.ps]");


  ////////////////Histograms file
  TFile output(rootfilename,"recreate");
  TDirectory* dir = output.mkdir("muTau_SM0");
  
  dir->cd();
  SM115->Write();
  SM115_CMS_scale_tUp   ->Write();
  SM115_CMS_scale_tDown ->Write();
  SM115_CMS_scale_jUp   ->Write();
  SM115_CMS_scale_jDown ->Write();
  ZTT ->Write();
  ZTT_CMS_scale_tUp   ->Write();
  ZTT_CMS_scale_tDown ->Write();
  ZTT_CMS_scale_jUp   ->Write();
  ZTT_CMS_scale_jDown ->Write();
  ZL->Write();
  ZL_CMS_scale_tUp   ->Write();
  ZL_CMS_scale_tDown ->Write();
  ZL_CMS_scale_jUp   ->Write();
  ZL_CMS_scale_jDown ->Write();
  ZJ->Write();
  ZJ_CMS_scale_tUp   ->Write();
  ZJ_CMS_scale_tDown ->Write();
  ZJ_CMS_scale_jUp   ->Write();
  ZJ_CMS_scale_jDown ->Write();
  W->Write();
  W_CMS_scale_tUp   ->Write();
  W_CMS_scale_tDown ->Write();
  W_CMS_scale_jUp   ->Write();
  W_CMS_scale_jDown ->Write();
  TT->Write();
  TT_CMS_scale_tUp   ->Write();
  TT_CMS_scale_tDown ->Write();
  TT_CMS_scale_jUp   ->Write();
  TT_CMS_scale_jDown ->Write();
  data_obs->Write();
  QCD->Write();


  output.Close();


  
  //////////////////////////////////////Data Card
  ofstream file;
  file.open("muTau_SM0_template_noVV.txt");

  file << "imax 1" <<endl;
  file << "jmax *" <<endl;
  file << "kmax *" <<endl;
  file << "shapes *  *  "<< rootfilename << "  $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  file << endl;
  file << "observation " << data_obs->Integral()<<endl;
  file<<endl;

  file << "bin muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0 muTau_SM0" << endl;
  file << "process SM115 ZTT QCD W ZJ ZL TT" << endl;
  file << "process 0 1 2 3 4 5 6" << endl;
  file << "rate " << SM115->Integral() << " " << ZTT->Integral() << " " << QCD->Integral() << " " << W->Integral() << " " << ZJ->Integral() << " " << ZL->Integral() << " " << TT->Integral() << endl;
  file << endl;
  file <<"-------------------------------------------------------"<<endl;
  file <<"lumi lnN 1.045 - - - - - - luminosity"<<endl;

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
