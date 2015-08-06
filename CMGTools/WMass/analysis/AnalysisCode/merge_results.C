#include <fstream>
#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "../common.h"
#include "../common2.h"
#include <TROOT.h>

void merge_results(int generated_PDF_set=1, int generated_PDF_member=0, TString WorZ="W", int RecoilCorrVarDiagoParU1orU2fromDATAorMC=0){

  bool some_fit_failed = false;
  
  int m_start = WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  int m_end = WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];

  TString original;
  std::vector<TString> tokenized;
  original = WorZ;
  TString     WCharge_str[]={"Pos","Neg"};

  ofstream outTXTfile,outTXTfile2;
  outTXTfile.open(Form("results.log"));
  outTXTfile2.open(Form("results_chi2.log"));
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<LineColumns->GetEntriesFast();j++)
  {
    tokenized.push_back(((TObjString *)LineColumns->At(j))->GetString());
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  
  for(unsigned int itoken=0; itoken<tokenized.size(); itoken++){


    TRandom3 *initmass = new TRandom3(0); 
    // TString WorZ = "Z"; // "Z" or "W"
    WorZ = tokenized.at(itoken); // "Z" or "W"
    TString Wlike = WorZ.Contains("W")?"":"like";
    int charges = WorZ.Contains("W")?2:1;
    int massCentral_MeV = WorZ.Contains("W")?WMass2::WMassCentral_MeV:WMass2::ZMassCentral_MeV;
    

    gStyle->SetOptStat(0);
    
    TFile *fout = new TFile(Form("likelihood_results_W%s.root",Wlike.Data()),"RECREATE");
    
      TGraph *result;
      TCanvas*c_chi2;
      TF1*ffit[WMass::etaMuonNSteps][WMass::NFitVar][2];
      // TGraph *result_NonScaled[WMass::PDF_members][WMass::NFitVar];
      TGraph *result_NonScaled[WMass::etaMuonNSteps][m_end-m_start][WMass::PDF_members][WMass::NFitVar][2];
      double deltaM[WMass::etaMuonNSteps][m_end-m_start][WMass::PDF_members][WMass::NFitVar][2];
      double deltaMmin[WMass::etaMuonNSteps][WMass::NFitVar][2]={{{0}}},deltaMmax[WMass::etaMuonNSteps][WMass::NFitVar][2]={{{0}}};
      double deltaMnegSummed[WMass::etaMuonNSteps][WMass::NFitVar][2]={{{0}}},deltaMposSummed[WMass::etaMuonNSteps][WMass::NFitVar][2]={{{0}}},deltaMSummed[WMass::etaMuonNSteps][WMass::NFitVar][2]={{{0}}},deltaMJuan[WMass::etaMuonNSteps][WMass::NFitVar][2]={{{0}}};
      TH1D*h_deltaM_PDF[WMass::etaMuonNSteps][WMass::NFitVar][2];
      TGraphErrors*g_deltaM_PDF[WMass::etaMuonNSteps][WMass::NFitVar][2];

    for(int c=0; c<charges; c++){
      cout <<"merging W"<<Wlike<<" " << WCharge_str[c].Data() << endl;
      
      if(WMass::PDF_members>1 || m_end>1){
        for(int k=0;k<WMass::NFitVar;k++){
          for(int i=0; i<WMass::etaMuonNSteps; i++){
            TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
            // TString eta_str = Form("%.1f",WMass::etaMaxMuons[0]); eta_str.ReplaceAll(".","p");

            g_deltaM_PDF[i][k][c] = new TGraphErrors();
            g_deltaM_PDF[i][k][c]->SetName(Form("g_deltaM_PDF_W%s%s_%sNonScaled_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),eta_str.Data()));
            g_deltaM_PDF[i][k][c]->SetTitle(Form("g_deltaM_PDF_W%s%s_%sNonScaled_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),eta_str.Data()));
            g_deltaM_PDF[i][k][c]->SetMarkerStyle(20);
            h_deltaM_PDF[i][k][c] = new TH1D(Form("h_deltaM_PDF_W%s%s_%sNonScaled_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),eta_str.Data()),Form("h_deltaM_PDF_%sNonScaled_eta%s",WMass::FitVar_str[k].Data(),eta_str.Data()),200,-50,50);

          }
        }
      }
      
      for(int h=0; h<WMass::PDF_members; h++){
        for(int m=m_start; m<m_end; m++){
            TString Kalmantoys_str = "";
            if(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0) Kalmantoys_str = Form("_RecoilCorrVar%d",m);
            cout << Form("fitting RecoilCorrVar%d",m) << endl;
          for(int n=0; n<WMass::KalmanNvariations; n++){
            // for(int i=0; i<WMass::etaMuonNSteps; i++){
            cout << "using pdf " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets) << "-"<<h<< endl;
            
            // for(int i=0; i<1; i++){
            for(int i=0; i<WMass::etaMuonNSteps; i++){
            
              TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
              cout << "merging pdf eta bin= " << i << endl;
              
              for(int k=0;k<WMass::NFitVar;k++){

                cout << "variable= " << k << endl;
              
                int npoint=0;

                // result = new TGraph();
                // result->SetName(Form("likelihood_results_%s_eta%s",WMass::FitVar_str[k].Data(),eta_str.Data()));
                // result->SetTitle(Form("likelihood_results_%s_eta%s",WMass::FitVar_str[k].Data(),eta_str.Data()));

                result_NonScaled[i][m-m_start][h][k][c] = new TGraph();
                result_NonScaled[i][m-m_start][h][k][c]->SetName(Form("likelihood_result_W%s%s_%sNonScaled_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,Kalmantoys_str.Data(),WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()));
                result_NonScaled[i][m-m_start][h][k][c]->SetTitle(Form("likelihood_result_W%s%s_%sNonScaled_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,Kalmantoys_str.Data(),WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()));

                std::vector<double> l_res;
                double lmin=0, lmax=0, deriv1=0, deriv2=0, deriv3=0;

                for(int j=0; j<2*WMass2::WMassNSteps+1; j++){
                  // int jWmass = WMass2::WMassCentral_MeV-(WMass2::WMassNSteps-j)*WMass2::WMassStep_MeV;
                  int jWmass = WorZ.Contains("Z")? WMass2::Zmass_values_array[j] : WMass2::Wmass_values_array[j];

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

                  cout << Form("dummy_datacard_Wmass_MuPos_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.log ",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):""),WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data());
                  TString test1 = Form("dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.log",Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):""),WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data());
                  // cout << test1 << endl;
                  std::ifstream fileNames_NonScaled(test1.Data());
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
                  if(!TStringFromFile.Contains("-2 ln Q_{TEV}")){
                    cout << "\nTStringFromFile= " << TStringFromFile << endl;
                    cout << "\n ERROR: COULDN'T FIND FIT RESULT IN "<< 
                    Form("dummy_datacard_Wmass_MuPos_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.log ",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):""),WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data())
                    << " resubmitting and quitting" << endl;
                    TString outfilename_str = Form("submit_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.sh",Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data());
                    gROOT->ProcessLine(".! bsub -C 0 -u pippo123 -q 1nh -J runMfit "+outfilename_str);                    
                    // gROOT->ProcessLine(".! sh "+outfilename_str);
                    some_fit_failed = true;
                    continue;
                  }
                  LineColumns = TStringFromFile.Tokenize(" ");
                  ncol = LineColumns->GetEntries();
                  // str_icol = ((TObjString *)LineColumns->At(3))->GetString();
                  TString str_icol = ((TObjString *)LineColumns->At(4))->GetString();
                  likelihood_val = (double) (str_icol.Atof());
                  // cout << jWmass << " LIKELIHOOD VALUE= "<<likelihood_val << " lmin= " << lmin << " lmax=" << lmax << endl;
                  cout << " " << jWmass << " LIKELIHOOD VALUE= "<< likelihood_val << endl;
                  l_res.push_back(likelihood_val);
                  if(likelihood_val<lmin) lmin=likelihood_val;
                  if(likelihood_val>lmax) lmax=likelihood_val;
                  
                  if(npoint==0) deriv1=likelihood_val;
                  else if(npoint==WMass2::WMassNSteps) deriv2=likelihood_val;
                  else if(npoint==2*WMass2::WMassNSteps) deriv3=likelihood_val;

                  npoint++;
              
                }
                if(some_fit_failed) continue;
                cout << endl;

                double l_offset;
                if(lmax>0)
                  l_offset=-lmax;
                else
                  l_offset=-lmin;
                // cout << "lmin= " << lmin << " lmax= " << lmax << endl;  
                for(int j=0; j<2*WMass2::WMassNSteps+1; j++){
                  // int jWmass = WMass2::WMassCentral_MeV-(WMass2::WMassNSteps-j)*WMass2::WMassStep_MeV;
                  int jWmass = WorZ.Contains("Z")? WMass2::Zmass_values_array[j] : WMass2::Wmass_values_array[j];

                  // cout << "result_NonScaled[i][h][k]->SetPoint("<<j<<","<<jWmass<<","<<(lmax>0 ? -l_res.at(j) -l_offset : l_res.at(j) -l_offset) <<");"<<endl;
                  result_NonScaled[i][m-m_start][h][k][c]->SetPoint(j,jWmass,deriv3-2*deriv2+deriv1<0 ? -l_res.at(j) -l_offset : l_res.at(j) -l_offset );
                }
                // cout << "second derivative= " << (deriv3-2*deriv2+deriv1) << endl;
                
                
                // result->SetMarkerStyle(20);
                // result->SetMarkerSize(1);
                // result->Write();
                result_NonScaled[i][m-m_start][h][k][c]->SetMarkerStyle(20);
                result_NonScaled[i][m-m_start][h][k][c]->SetMarkerSize(1);
                result_NonScaled[i][m-m_start][h][k][c]->Write();
                
                c_chi2=new TCanvas(Form("c_chi2_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),Form("c_chi2_W%s_%s_pdf%d-%d%s%s_eta%s",WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()));
                ffit[i][k][c]=new TF1(Form("ffit_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),Form("[0]+TMath::Power((x-[1])/[2],2)"),70,100);
                ffit[i][k][c]->SetParameter(0,result_NonScaled[i][m-m_start][h][k][c]->GetMinimum());
                ffit[i][k][c]->SetParameter(1,WorZ.Contains("W")?80410:91170);
                ffit[i][k][c]->SetParameter(2,10); // IF FIT DOES NOT CONVERGE, CHANGE THIS PARAMETER BY LOOKING AT THE CHI2 VS MASS DISTRIBUTION (~value for which Delta_chi2 = 1)
                // ffit[i][k]->SetParameter(2,1e4); // IF FIT DOES NOT CONVERGE, CHANGE THIS PARAMETER BY LOOKING AT THE CHI2 VS MASS DISTRIBUTION (~value for which Delta_chi2 = 1)
                ffit[i][k][c]->SetLineColor(2);
                ffit[i][k][c]->SetLineWidth(1);
                int fires = result_NonScaled[i][m-m_start][h][k][c]->Fit(Form("ffit_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),"WEM");
                cout << "fitres= " << fires << " chi2= " << ffit[i][k][c]->GetChisquare() << " ndof= " << ffit[i][k][c]->GetNDF() << " chi2/ndof= " << (ffit[i][k][c]->GetChisquare()/ffit[i][k][c]->GetNDF()) << endl;
                int attempts=0;
                while((ffit[i][k][c]->GetChisquare()/ ffit[i][k][c]->GetNDF())>1*(WorZ.Contains("W")?1:3) && attempts<5){
                  double rmass = initmass->Gaus(WorZ.Contains("W")?80410:91100, 200);
                  ffit[i][k][c]->SetParameter(0,result_NonScaled[i][m-m_start][h][k][c]->GetMinimum());
                  ffit[i][k][c]->SetParameter(1, rmass );
                  ffit[i][k][c]->SetParameter(2,TMath::Abs(initmass->Gaus(0, 1+5*(WorZ.Contains("W")?attempts-1:attempts/5)))); // IF FIT DOES NOT CONVERGE, CHANGE THIS PARAMETER BY LOOKING AT THE CHI2 VS MASS DISTRIBUTION (~value for which Delta_chi2 = 1)
                  ffit[i][k][c]->SetLineColor(2);
                  ffit[i][k][c]->SetLineWidth(1);
                  result_NonScaled[i][m-m_start][h][k][c]->Fit(Form("ffit_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),"WEM");
                  cout << "refitted with init mass: " << rmass << " chi2 norm: " << (ffit[i][k][c]->GetChisquare()/ ffit[i][k][c]->GetNDF()) << endl;
                  attempts++;
                }
                for(int ifit=0;ifit<50;ifit++){
                  if((ffit[i][k][c]->GetChisquare()/ ffit[i][k][c]->GetNDF())>1e3 || TMath::Abs(ffit[i][k][c]->GetParameter(1) - (massCentral_MeV))>1e5 ){
                    // ffit[i][k][c]->SetParameter(0,result_NonScaled[i][m-m_start][h][k][c]->GetMinimum());
                    // ffit[i][k][c]->SetParameter(1, initmass->Gaus(80410, 200); );
                    result_NonScaled[i][m-m_start][h][k][c]->Fit(Form("ffit_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),"QWEM");
                    }
                }
                result_NonScaled[i][m-m_start][h][k][c]->Fit(Form("ffit_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),"WEM");
                
                result_NonScaled[i][m-m_start][h][k][c]->Draw("ap");
                TLatex *text,*text2;
                text = new TLatex(0.25,0.7,Form("Best M_{W%s} with %s= %.1f #pm %.1f MeV", Wlike.Data(), WMass::FitVar_str[k].Data(),ffit[i][k][c]->GetParameter(1),ffit[i][k][c]->GetParameter(2)));
                text->SetNDC();
                text->Draw();
                text = new TLatex(0.25,0.6,Form("#DeltaM_{W%s} = %.1f MeV", Wlike.Data(), (ffit[i][k][c]->GetParameter(1) - (massCentral_MeV) )));
                text->SetNDC();
                text->Draw();
                // text2 = new TLatex(0.25,0.6,Form("Best #chi^{2} ratio = %.1f", ffit[i][k]->GetParameter(0) ));
                // text2->SetNDC();
                // text2->Draw();
                cout << Form("Best M_W%s value with %s = %.1f +/- %.1f MeV,\t DeltaM_W%s = %.1f +/- %.1f MeV,\t chi2/ndof= %.2f",Wlike.Data(),WMass::FitVar_str[k].Data(), ffit[i][k][c]->GetParameter(1), ffit[i][k][c]->GetParameter(2),Wlike.Data(), (ffit[i][k][c]->GetParameter(1) - (massCentral_MeV)), ffit[i][k][c]->GetParameter(2),(ffit[i][k][c]->GetChisquare()/ffit[i][k][c]->GetNDF())) << endl;
                outTXTfile2 << Form("Best M_W%s %s value with %s = %.1f +/- %.1f MeV,\t DeltaM_W%s = %.1f +/- %.1f MeV,\t chi2/ndof= %.2f",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(), ffit[i][k][c]->GetParameter(1), ffit[i][k][c]->GetParameter(2),Wlike.Data(), (ffit[i][k][c]->GetParameter(1) - (massCentral_MeV)), ffit[i][k][c]->GetParameter(2),(ffit[i][k][c]->GetChisquare()/ffit[i][k][c]->GetNDF())) << endl;
                outTXTfile << Form("%.1f\t +/-\t %.1f",(ffit[i][k][c]->GetParameter(1) - (massCentral_MeV)), TMath::Abs(ffit[i][k][c]->GetParameter(2))) << endl;
                // cout << "Best chi2 ratio value = " << ffit[i][k]->GetParameter(0) << endl;
                // cout << "Measured mass points chi2 min = " << chi2min << " max = " << chi2max << endl;
                
                c_chi2->Write();
                
                // result_NonScaled[i][h][k]->Delete();
                // result->Delete();
                
              }
              if(some_fit_failed) continue;
         
              TCanvas *c_summary=new TCanvas(Form("c_summary_W%s%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()),Form("c_summary_pdf%d-%d%s%s_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data())); 
              c_summary->SetGridy();
              c_summary->SetGridx();
              TH2D*frame=new TH2D("frame",Form("pdf %d-%d %s eta %s;M_{W%s} (MeV); -2ln(L/L_{ref})",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),Wlike.Data()),2*WMass2::WMassNSteps+1,WorZ.Contains("Z")? WMass2::Zmass_values_array[0] : WMass2::Wmass_values_array[0],WorZ.Contains("Z")? WMass2::Zmass_values_array[2*WMass2::WMassNSteps] : WMass2::Wmass_values_array[2*WMass2::WMassNSteps],10,0,5);
              frame->Draw();
              TLine *l=new TLine((massCentral_MeV),0,(massCentral_MeV),5);
              l->SetLineStyle(6);
              l->SetLineColor(4);
              l->SetLineWidth(2);
              l->Draw();

              TLegend *leg1 = new TLegend(0.51,0.7,0.89,0.89);
              leg1->SetFillColor(10);  leg1->SetBorderSize(1);
              leg1->SetTextSize(0.035);
              leg1->AddEntry(l, Form("gen M_{W%s} (%d MeV)",Wlike.Data(),(massCentral_MeV)), "l");

              TF1*f[WMass::NFitVar];
              for(int k=0;k<WMass::NFitVar;k++){
                // cout << result_NonScaled[i][h][k]->GetParameter(1) << " " << ffit[i][k]->GetParameter(2) << endl;
                // ffit[i][k]->Draw("");
                f[k] = (TF1*) result_NonScaled[i][m-m_start][h][k][c]->GetFunction(Form("ffit_W%s%s_%s_pdf%d-%d%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data()));
                f[k]->SetLineColor(k+1);
                f[k]->SetLineWidth(2);
                f[k]->Draw("same");
                leg1->AddEntry(f[k], Form("%s Fit (#DeltaM_{W%s} = %.1f #pm %0.1f MeV)",WMass::FitVar_str[k].Data(), Wlike.Data(),(f[k]->GetParameter(1) - (massCentral_MeV)),f[k]->GetParameter(2)), "l");
                // result_NonScaled[i][h][k]->Draw("l same");
                // ffit[i][k]->SetLineColor(4);
                // ffit[i][k]->SetLineWidth(4);
                // ffit[i][k]->Draw(k>0?"same":"");
                // result_NonScaled[i][h][k]->SaveAs("test_func");
                deltaM[i][m-m_start][h][k][c]=f[k]->GetParameter(1) - (massCentral_MeV);
                if(deltaM[i][m-m_start][h][k][c]<deltaMmin[i][k][c]) deltaMmin[i][k][c]=deltaM[i][m-m_start][h][k][c];
                if(deltaM[i][m-m_start][h][k][c]>deltaMmax[i][k][c]) deltaMmax[i][k][c]=deltaM[i][m-m_start][h][k][c];

                if(h>0 || RecoilCorrVarDiagoParU1orU2fromDATAorMC>0){
                  if(deltaM[i][m-m_start][h][k][c]<0) deltaMnegSummed[i][k][c] = TMath::Sqrt(deltaMnegSummed[i][k][c]*deltaMnegSummed[i][k][c] + deltaM[i][m-m_start][h][k][c]*deltaM[i][m-m_start][h][k][c]);
                  if(deltaM[i][m-m_start][h][k][c]>0) deltaMposSummed[i][k][c] = TMath::Sqrt(deltaMposSummed[i][k][c]*deltaMposSummed[i][k][c] + deltaM[i][m-m_start][h][k][c]*deltaM[i][m-m_start][h][k][c]);
                  deltaMSummed[i][k][c] = TMath::Sqrt(deltaMSummed[i][k][c]*deltaMSummed[i][k][c] + deltaM[i][m-m_start][h][k][c]*deltaM[i][m-m_start][h][k][c]);
                  if(h%2==0)
                    deltaMJuan[i][k][c] = TMath::Sqrt( deltaMJuan[i][k][c]*deltaMJuan[i][k][c] + (deltaM[i][m-m_start][h][k][c]-deltaM[i][m-m_start][h-1][k][c])*(deltaM[i][m-m_start][h][k][c]-deltaM[i][m-m_start][h-1][k][c]));
                }
                
                if(WMass::PDF_members>1){
                  g_deltaM_PDF[i][k][c]->SetPoint(h,h,deltaM[i][m-m_start][h][k][c]);
                  g_deltaM_PDF[i][k][c]->SetPointError(h,0,f[k]->GetParameter(2));
                  h_deltaM_PDF[i][k][c]->Fill(deltaM[i][m-m_start][h][k][c]);
                }else if(m_end>1){
                  g_deltaM_PDF[i][k][c]->SetPoint(m,m,deltaM[i][m-m_start][h][k][c]);
                  g_deltaM_PDF[i][k][c]->SetPointError(m,0,f[k]->GetParameter(2));
                  h_deltaM_PDF[i][k][c]->Fill(deltaM[i][m-m_start][h][k][c]);
                }
              }
              leg1->Draw("same");
              c_summary->Write();

            }
            // }
          }
        }
      }
      if(some_fit_failed) continue;
      
      for(int i=0; i<WMass::etaMuonNSteps; i++){
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
        for(int k=0;k<WMass::NFitVar;k++){
          if(WMass::PDF_members>1){

            cout << endl;
            int usedpdf = WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets;
            cout << Form("PDF %d with %s: #DeltaM_{W%s, min}= %.1f MeV, #DeltaM_{W,max}= %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMmin[i][k][c], deltaMmax[i][k][c]) << endl;
            double denominator = 1;

            TCanvas*c1= new TCanvas(Form("c_deltaM_PDF_W%s%s_%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),eta_str.Data()),Form("c_deltaM_PDF_W%s%s_%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),eta_str.Data()));
            
            g_deltaM_PDF[i][k][c]->Draw("ape");
            TLatex *text;
            text = new TLatex(0.2,0.85,Form("W%s%s, PDF %d with %s: #DeltaM_{W%s, min}= %.1f MeV, #DeltaM_{W%s,max}= %.1f MeV",Wlike.Data() , WCharge_str[c].Data(), WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMmin[i][k][c], Wlike.Data(),deltaMmax[i][k][c]));
            text->SetTextSize(0.035);
            text->SetNDC();
            text->Draw();
            
            if(usedpdf==232000 || usedpdf==229800){ // NNPDF, see ref. http://arxiv.org/pdf/1301.6754v1.pdf formulae 2.23 and 2.25
              // cout << Form("PDF %d with %s: #DeltaM_{W} square summed = -%.1f MeV, #DeltaM_{W} square summed = %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), deltaMnegSummed[i][k]/denominator, deltaMposSummed[i][k]/denominator) << endl;
              denominator = TMath::Sqrt(WMass::PDF_members/2-1);
              cout << Form("PDF %d with %s: #DeltaM_{W%s} square summed = %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMSummed[i][k][c]/denominator) << endl;
              text = new TLatex(0.2,0.8,Form("PDF %d with %s: #DeltaM_{W%s} square summed = %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMSummed[i][k][c]/denominator));
              text->SetTextSize(0.035);
              text->SetNDC();
              text->Draw();
            }else{
              cout << Form("PDF %d with %s: #DeltaM_{W%s} as of J. Rojo = %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(),Wlike.Data(), (deltaMJuan[i][k][c]/2/denominator)) << endl;
              text = new TLatex(0.2,0.8,Form("PDF %d with %s: #DeltaM_{W%s} as of J. Rojo = %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), Wlike.Data(),(deltaMJuan[i][k][c]/2/denominator)));
              text->SetTextSize(0.035);
              text->SetNDC();
              text->Draw();
            }

            g_deltaM_PDF[i][k][c]->Write();
            h_deltaM_PDF[i][k][c]->Write();
            c1->Write();
            
            // mean = TMath::Mean(n, &v[0]);
            // vector<double> v;
            // // std::generate(v.begin(), v.end(), 1);
            // if(k==1)
              // for(int h=0; h<WMass::PDF_members; h++){
                // // v.push_back(deltaM[i][h][k]);
                // cout << deltaM[i][h][k] << endl;
              // }
            // double meanWmass= TMath::Mean(v.begin(), v.end());
            // cout << "meanWmass= " << meanWmass << endl;
          }
          if(m_end>1){
            cout << endl;
            // int usedpdf = WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets;
            cout << Form("MomCorr toys with %s: #DeltaM_{W%s, min}= %.1f MeV, #DeltaM_{W%s,max}= %.1f MeV", WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMmin[i][k][c], Wlike.Data(),deltaMmax[i][k][c]) << endl;
            double denominator = 1;

            TCanvas*c1= new TCanvas(Form("c_deltaM_PDF_%s_eta%s",WMass::FitVar_str[k].Data(),eta_str.Data()),Form("c_deltaM_PDF_%s_eta%s",WMass::FitVar_str[k].Data(),eta_str.Data()));
            g_deltaM_PDF[i][k][c]->Draw("ape");
            TLatex *text;
            text = new TLatex(0.2,0.85,Form("MomCorr toys with %s: #DeltaM_{W%s, min}= %.1f MeV, #DeltaM_{W%s,max}= %.1f MeV", WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMmin[i][k][c], Wlike.Data(),deltaMmax[i][k][c]));
            text->SetTextSize(0.035);
            text->SetNDC();
            text->Draw();
            
            // if(usedpdf==232000){ // NNPDF, see ref. http://arxiv.org/pdf/1301.6754v1.pdf formulae 2.23 and 2.25
              // cout << Form("PDF %d with %s: #DeltaM_{W} square summed = -%.1f MeV, #DeltaM_{W} square summed = %.1f MeV", WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets, WMass::FitVar_str[k].Data(), deltaMnegSummed[i][k]/denominator, deltaMposSummed[i][k]/denominator) << endl;
              // denominator = TMath::Sqrt(WMass::NVarRecoilCorr/2-1);
              cout << Form("MomCorr toys with %s: #DeltaM_{W%s} square summed = %.1f MeV", WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMSummed[i][k][c]/denominator) << endl;
              text = new TLatex(0.2,0.8,Form("MomCorr toys with %s: #DeltaM_{W%s} square summed = %.1f MeV", WMass::FitVar_str[k].Data(), Wlike.Data(),deltaMSummed[i][k][c]/denominator));
              text->SetTextSize(0.035);
              text->SetNDC();
              text->Draw();
            // }else{
              // cout << Form("MomCorr toys with %s: #DeltaM_{W} as of J. Rojo = %.1f MeV", WMass::FitVar_str[k].Data(), (deltaMJuan[i][k]/2/denominator)) << endl;
              // text = new TLatex(0.2,0.8,Form("MomCorr toys with %s: #DeltaM_{W} as of J. Rojo = %.1f MeV", WMass::FitVar_str[k].Data(), (deltaMJuan[i][k]/2/denominator)));
              // text->SetTextSize(0.035);
              // text->SetNDC();
              // text->Draw();
            // }

            g_deltaM_PDF[i][k][c]->Write();
            h_deltaM_PDF[i][k][c]->Write();
            c1->Write();

          }
        }
      }
    }
      
      fout->Write();
      
      fout->Delete();
  }
  
  if(some_fit_failed) cout << "\nTHERE WERE SOME FAILED FITS, WHICH HAVE BEEN RESUBMITTED. RUN AGAIN THE MERGING STEP WHEN THEY FINISH" << endl;
  // outTXTfile.Close();
  // outTXTfile2.Close();
}
