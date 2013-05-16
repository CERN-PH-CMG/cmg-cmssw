#include "../test_numbers_DATA/common.h"

void merge_results(){
  
  TFile *fout = new TFile(Form("likelihood_results.root"),"RECREATE");
  
  // for(int i=0; i<WMass::etaMuonNSteps; i++){
  for(int i=0; i<1; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    
    int npoint=0;

    TGraph *result = new TGraph();
    result->SetName(Form("likelihood_results_eta%s",eta_str.Data()));
    result->SetTitle(Form("likelihood_results_eta%s",eta_str.Data()));

    TGraph *result_NonScaled = new TGraph();
    result_NonScaled->SetName(Form("likelihood_result_NonScaled_eta%s",eta_str.Data()));
    result_NonScaled->SetTitle(Form("likelihood_result_NonScaled_eta%s",eta_str.Data()));

    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
  
      // std::ifstream fileNames(Form("dummy_datacard_Wmass_MuPos_eta%s_%d.log",eta_str.Data(),jWmass));
      string StringFromFile;
      TString TStringFromFile;
      double likelihood_val;
      int ncol;
      // while (!fileNames.eof()){
        // getline (fileNames,StringFromFile);
        // TStringFromFile = StringFromFile.c_str();
        // if(TStringFromFile.Contains("nll S+B ->")){
          // break;
        // }
      // }
      // TObjArray* LineColumns = TStringFromFile.Tokenize(" ");
      // ncol = LineColumns->GetEntries();
      // if(ncol<3){
        // cout << Form("problem while analyzing fit result in dummy_datacard_Wmass_MuPos_eta%s_%d.log",eta_str.Data(),jWmass) << endl;
        // return;
      // }
      // TString str_icol = ((TObjString *)LineColumns->At(3))->GetString();
      // likelihood_val = (double) (str_icol.Atof());
      // if(likelihood_val<0) result->SetPoint(npoint,jWmass,likelihood_val);

      
      std::ifstream fileNames_NonScaled(Form("dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.log",eta_str.Data(),jWmass));
      // string StringFromFile;
      // TString TStringFromFile;
      while (!fileNames_NonScaled.eof()){
        getline (fileNames_NonScaled,StringFromFile);
        TStringFromFile = StringFromFile.c_str();
        // if(TStringFromFile.Contains("nll S+B ->")){
        if(TStringFromFile.Contains("-2 ln Q_{TEV}")){
          break;
        }
      }
      TObjArray* LineColumns = TStringFromFile.Tokenize(" ");
      ncol = LineColumns->GetEntries();
      // str_icol = ((TObjString *)LineColumns->At(3))->GetString();
      str_icol = ((TObjString *)LineColumns->At(4))->GetString();
      likelihood_val = (double) (str_icol.Atof());
      cout << jWmass << " LIKELIHOOD VALUE= "<<likelihood_val << endl;
      result_NonScaled->SetPoint(npoint,jWmass,likelihood_val);

      npoint++;
  
    }

    result->SetMarkerStyle(20);
    result->SetMarkerSize(1);
    result->Write();
    result->Delete();
    result_NonScaled->SetMarkerStyle(20);
    result_NonScaled->SetMarkerSize(1);
    result_NonScaled->Write();
    result_NonScaled->Delete();
    
  }
  
  fout->Write();
  
  fout->Delete();

}