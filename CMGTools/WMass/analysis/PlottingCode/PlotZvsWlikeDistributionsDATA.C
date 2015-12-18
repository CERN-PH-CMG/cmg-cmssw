#include "../includes/common.h"
#include "../AnalysisCode/common_stuff.C"

void PlotZvsWlikeDistributionsDATA(TString folderZ=""){

  cout << "PlotZvsWlikeDistributionsDATA.C working dir: " << gSystem->WorkingDirectory() << endl;


  TFile *fZ=new TFile(Form("%s/ZanalysisOnDATA.root",folderZ.Data()));
  fZ->Print();
    
  common_stuff::plotAndSaveHisto1D(fZ,"hMuMETpt_WlikePos_Sig_eta2p1",fZ,"hpfMET_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fZ,"hpfMETphi_WlikePos_Sig_eta2p1",fZ,"hMuMETphi_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fZ,"hZ_pt_Sig_eta2p1",fZ,"hWlikePos_pt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fZ,"hZ_phi_Sig_eta2p1",fZ,"hWlikePos_phi_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fZ,"hZ_mt_Sig_eta2p1",fZ,"hWlikePos_mt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);

}
