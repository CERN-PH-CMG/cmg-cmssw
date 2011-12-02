void histosForLimit(){
  //defines samples
  gROOT->ProcessLine(".x ../workdir/TauMuConfig.C");
  
  //defines input histograms
  analysis.setOutputPath("/afs/cern.ch/user/b/benitezj/public/V240TauTrigger");

  //normalizes backgrounds for the data
  if(!analysis.init()){cout<<" could not init"<<endl;return 0;}  
  if(!analysis.scaleHistos())return 0;
  

  /* ***************************************
    -access backgrounds like this (updated Dec 2)
    -everything should be obtained from class interface/TauMuAnalysis.h
    -individual backgrounds obtained using getSample(), except for ZtoTauTau and QCD 
    ****************************************** 
    analysis.getTotalDataSS(TString histoname);//sum of SS Data samples 
    analysis.getTotalData(TString histoname);
    analysis.getSample(TString samplename, TString histoname);//can be used to get histo for any MC or Data sample
    analysis.getQCD(TString histoname);
    analysis.getZToTauTau(TString histoname);//Z-->tau tau (either from MC or Embedded)
    analysis.getTotalBackground(TString histoname);
  */


  ofstream file;
  file.open("yields.txt");
  
  TFile* output = TFile::Open("muTau.root","recreate");
  TDirectory* dir = output->mkdir("muTau_SM0");
  
  //histogram used for fit in limit calcualtion
  TString histoname="diTauMassHisto";
  

  TCanvas C("Canvas");
  C.Print("plots.ps[");
  
  TH1F* SM115_gg = new TH1F("H","",2000,0,1000);//(TH1F*)(analysis.getSample("Higgs",histoname));
  C.Clear();
  SM115_gg->Draw();
  C.Print("plots.ps");
  
  TH1F* SM115 = (TH1F*)SM115_gg->Clone("SM115");
  TH1F* SM115_CMS_scale_tUp = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_tUp");
  TH1F* SM115_CMS_scale_tDown = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_tDown");
  TH1F* SM115_CMS_scale_jUp = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_jUp");
  TH1F* SM115_CMS_scale_jDown = (TH1F*)SM115_gg->Clone("SM115_CMS_scale_jDown");
  
  TH1F* ZTauTau = (TH1F*)(analysis.getZToTauTau(histoname));
  C.Clear();
  ZTauTau->Draw();
  C.Print("plots.ps");
  
  TH1F* ZTT = (TH1F*)ZTauTau->Clone("ZTT");
  TH1F* ZTT_CMS_scale_tUp = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_tUp");
  TH1F* ZTT_CMS_scale_tDown = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_tDown");
  TH1F* ZTT_CMS_scale_jUp = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_jUp");
  TH1F* ZTT_CMS_scale_jDown = (TH1F*)ZTauTau->Clone("ZTT_CMS_scale_jDown");
  
  TH1F* ZMuMu = (TH1F*)(analysis.getSample("ZToMuMu",histoname));
  C.Clear();
  ZMuMu->Draw();
  C.Print("plots.ps");
  
  TH1F* ZL = (TH1F*)ZMuMu->Clone("ZL");
  TH1F* ZL_CMS_scale_tUp = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_tUp");
  TH1F* ZL_CMS_scale_tDown = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_tDown");
  TH1F* ZL_CMS_scale_jUp = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_jUp");
  TH1F* ZL_CMS_scale_jDown = (TH1F*)ZMuMu->Clone("ZL_CMS_scale_jDown");
  
  TH1F* ZJets = (TH1F*)(analysis.getSample("ZToLJet",histoname));
  C.Clear();
  ZJets->Draw();
  C.Print("plots.ps");
  
  TH1F* ZJ = (TH1F*)ZJets->Clone("ZJ");
  TH1F* ZJ_CMS_scale_tUp = (TH1F*)ZJets->Clone("ZJ_CMS_scale_tUp");
  TH1F* ZJ_CMS_scale_tDown = (TH1F*)ZJets->Clone("ZJ_CMS_scale_tDown");
  TH1F* ZJ_CMS_scale_jUp = (TH1F*)ZJets->Clone("ZJ_CMS_scale_jUp");
  TH1F* ZJ_CMS_scale_jDown = (TH1F*)ZJets->Clone("ZJ_CMS_scale_jDown");

  TH1F* WJets = (TH1F*)(analysis.getSample("WJetsToLNu",histoname));
  C.Clear();
  WJets->Draw();
  C.Print("plots.ps");

  TH1F* W = (TH1F*)WJets->Clone("W");
  TH1F* W_CMS_scale_tUp = (TH1F*)WJets->Clone("W_CMS_scale_tUp");
  TH1F* W_CMS_scale_tDown = (TH1F*)WJets->Clone("W_CMS_scale_tDown");
  TH1F* W_CMS_scale_jUp = (TH1F*)WJets->Clone("W_CMS_scale_jUp");
  TH1F* W_CMS_scale_jDown = (TH1F*)WJets->Clone("W_CMS_scale_jDown");


  TH1F* TTJ = (TH1F*)(analysis.getSample("TTJets",histoname));
  C.Clear();
  TTJ->Draw();
  C.Print("plots.ps");
  
  TH1F* TT = (TH1F*)TTJ->Clone("TT");
  TH1F* TT_CMS_scale_tUp = (TH1F*)TTJ->Clone("TT_CMS_scale_tUp");
  TH1F* TT_CMS_scale_tDown = (TH1F*)TTJ->Clone("TT_CMS_scale_tDown");
  TH1F* TT_CMS_scale_jUp = (TH1F*)TTJ->Clone("TT_CMS_scale_jUp");
  TH1F* TT_CMS_scale_jDown = (TH1F*)TTJ->Clone("TT_CMS_scale_jDown");
  
  
  TH1F* data=analysis.getTotalData(histoname);
  C.Clear();
  data->Draw();
  C.Print("plots.ps");

  TH1F* data_obs = (TH1F*)data->Clone("data_obs");

  //TH1F* data_obsSS=analysis.getDataSS(histoname);
  //C.Clear();
  //data_obsSS->Draw();
  //C.Print("plots.ps");
  
  TH1F* QCD_b=analysis.getQCD(histoname);
  C.Clear();
  QCD_b->Draw();
  C.Print("plots.ps");

  TH1F* QCD = (TH1F*)QCD_b->Clone("QCD");

  C.Print("plots.ps]");

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

  file << "observation " << data_obs->Integral()<<endl;
  file << "bin " << "muTau_SM0 " << "muTau_SM0 " << "muTau_SM0 " << "muTau_SM0 " << "muTau_SM0 " << "muTau_SM0 " << "muTau_SM0 " << endl;
  file << "process " << "SM115 " << "ZTT " << "QCD " << "W " << "ZJ " << "ZL " << "TT " << endl;
  file << "process " << "0 " << "1 " << "2 " << "3 " << "4 " << "5 " << "6 " << endl;
  file << "rate " << SM115->Integral() << " " << ZTT->Integral() << " " << QCD->Integral() << " " << W->Integral() << " " << ZJ->Integral() << " " << ZL->Integral() << " " << TT->Integral() << endl;
  
  
  file << "WJets_BR" << '\t' << W->Integral()/0.31296<<endl;
  file << "QCD_BR" << '\t' << QCD->Integral()/1.06<<endl;
  
}
