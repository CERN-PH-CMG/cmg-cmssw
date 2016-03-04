#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "../includes/common.h"

void prepareDatacards(TString folder, TString syst_folder, TString sample, int generated_PDF_set=1, int generated_PDF_member=0, TString WorZ="W"){

  TString original;
  std::vector<TString> tokenized;
  original = WorZ;
      
  TObjArray* LineColumns = original.Tokenize(",");
  
  for(int j=0;j<LineColumns->GetEntriesFast();j++)
  {
    tokenized.push_back(((TObjString *)LineColumns->At(j))->GetString());
    // cout << "tokenized["<<j<<"]= " << tokenized[j] << endl;
  }
  
  for(unsigned int itoken=0; itoken<tokenized.size(); itoken++){

    static const int Nsamples=13;
    enum                                 {  WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD     ,  DATA  ,  EWK  ,  EWKTT  ,  MCDATALIKE  };
    TString     samples_str[Nsamples]  = { "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"    , "DATA" , "EWK" , "EWKTT" , "MCDATALIKE" };
    TString     WCharge_str[]={"Pos","Neg"};


    cout << "folder= " << folder << endl;
    cout << "samples= " << sample << endl;
    
    WorZ = tokenized.at(itoken); // "Z" or "W"
    TString Wlike = WorZ.Contains("W")?"":"like";
    int charges = WorZ.Contains("W")?2:1;
    cout << "WorZ= " << WorZ << endl;
    
    // double xmin=0.8, xmax=1.2;
    // double xmin=0.65, xmax=2;
    // double xmin=(30.*(1+(int)(1==1)))/(WorZ.Contains("W")?WMass::WMassCentral_MeV:91178.6/1e3), xmax=(50.*(1+(int)(1==1)))/(WorZ.Contains("W")?WMass::WMassCentral_MeV:91187.6/1e3);
    double xmin=(30.*(1+(int)(1==1)))/(WMass::WMassCentral_MeV/1e3), xmax=(50.*(1+(int)(1==1)))/(WMass::WMassCentral_MeV/1e3);
    
    // TOKENIZE SAMPLES
    // std::vector<TString> samples_str;
    TFile* finTemplatesZ[Nsamples], *finTemplatesW[Nsamples];
    TH1D *TemplatesZ[WMass::NVarRecoilCorr][WMass::PDF_members][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *TemplatesZ_NonScaled[WMass::NVarRecoilCorr][WMass::PDF_members][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
    // TH1D *TemplatesW[WMass::NVarRecoilCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *TemplatesW_NonScaled[WMass::NVarRecoilCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
    TH1D *TemplatesW[2][WMass::NVarRecoilCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *TemplatesW_NonScaled[2][WMass::NVarRecoilCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
    // vector <*TH1D>
    // std::vector<std::vector<std::vector< std::vector< std::vector< std::vector< *TH1D > > > > > >;
    // TObjArray* samples_obj = sample.Tokenize("-");
    // int Nsamples = samples_obj->GetEntries();
    
    // LOAD ALL THE HISTOS FROM THE VARIOUS FILES IN MEMORY
    for(int isample=0; isample<Nsamples;isample++){
      // samples_str.push_back( ((TObjString *)samples_obj->At(isample))->GetString() );
      // finTemplatesZ[isample] = new TFile(Form("%s/output_%s/WSimpleTemplates.root",folder.Data(),samples_str[isample].Data()));
      finTemplatesW[isample] = new TFile(Form("%s/output_%s/%sanalysisOnDATA.root",folder.Data(),samples_str[isample].Data(),WorZ.Data()));
      // finTemplatesZ[isample]->Print();
      finTemplatesW[isample]->Print();
      for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
        for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
          int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
          
          for(int h=0; h<WMass::PDF_members; h++){
              for(int m=0; m<WMass::NVarRecoilCorr; m++){
              // TemplatesZ[m][h][isample][ieta][jmass] = (TH1D*) finTemplatesZ[isample]->Get(Form("hWlikePos_PtScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
              // TemplatesZ_NonScaled[m][h][isample][ieta][jmass] = (TH1D*) finTemplatesZ[isample]->Get(Form("hWlikePos_PtNonScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
              // cout << Form("hWlikePos_PtNonScaled_RWeighted_SimpleTemplates_pdf%d-%d_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jWmass) << endl;
              // TemplatesZ_NonScaled[h][isample][ieta][jmass]->Print();
              for(int k=0;k<WMass::NFitVar;k++){
                for(int c=0;c<charges;c++){
                  TemplatesW[c][m][h][k][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hW%s%s_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
                  // TemplatesW_NonScaled[h][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hWPos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
                  
                  // N.B. IS ALWAYS THE SAME TEMPLATE!!! MASS INDEX IS FIXED TO MC GENERATED W MASS, i.e. WMass::WMassCentral_MeV)
                  // TemplatesW_NonScaled[h][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hWPos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),WMass::WMassCentral_MeV));
                  
                  TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hW%s%s_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));
                  // TemplatesZ[h][isample][ieta][jmass]->Print();
                  // TemplatesW[h][isample][ieta][jmass]->Print();
                  
                  // TemplatesW_NonScaled[h][k][isample][ieta][jmass]->Print();
                  // cout << TemplatesW_NonScaled[h][k][isample][ieta][jmass]->Integral() << " "<< TemplatesW_NonScaled[h][k][isample][ieta][jmass]->GetMean() << " "<< TemplatesW_NonScaled[h][k][isample][ieta][jmass]->GetRMS() << endl;
                }
              }
            }
          }
          
        }
      }
      
    }
    for(int isample=0; isample<Nsamples;isample++){
      for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
        for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
          for(int h=0; h<WMass::PDF_members; h++){
            for(int m=0; m<WMass::NVarRecoilCorr; m++){
              for(int k=0;k<WMass::NFitVar;k++){
                for(int c=0;c<charges;c++){
                                      // TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->Print();
                  // double int_hist_data = TemplatesW_NonScaled[c][0][0][k][DATA][ieta][WMass::WMassNSteps]->Integral();
                  
                  if(TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]){
                    double int_hist_data = TemplatesW_NonScaled[c][0][0][k][isample][ieta][WMass::WMassNSteps]->Integral();
                    double int_hist_mcdatalike = TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->Integral();
                    double norm_factor_to_match_data = int_hist_mcdatalike>0 ? int_hist_data/int_hist_mcdatalike : 1;
                    
                    TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->Scale(norm_factor_to_match_data);
                    cout <<"c "<<c<<" m "<<m<<" h "<<" "<<h<<" k "<<k<<" isample "<<samples_str[isample]<<" ieta "<<ieta<<" jmass "<<jmass<<" norm "<<norm_factor_to_match_data<<endl;
                  }
                }
              }
            }
          }
        }
      }
    }
    // return;
    // PROCESS THE HISTOS AND STORE THEM IN A SUITABLE FILE, STORE NORMALIZATIONS IN A SEPARATE TEXT FILE
    TFile *foutDATA = new TFile(Form("%s/DataCards/datacards_DATA%s.root",folder.Data(),WorZ.Contains("W")?"":"_Wlike"),"RECREATE");
    
    for(int c=0; c<charges; c++){
      ofstream outTXTfile;
      outTXTfile.open(Form("%s/DataCards/datacard_Wmass_Mu%s%s_normalizations.txt",folder.Data(),Wlike.Data(),WCharge_str[c].Data()));
      
      // LOOP OVER MAX-ETA BINS
    
      for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
        outTXTfile << "-----------------------" << endl;
        outTXTfile << "-----------------------" << endl;
        // outTXTfile << "MuPos with |eta| < " << WMass::etaMaxMuons[ieta] << " PDF " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets)<<"-"<<h<< endl;
        outTXTfile << "Mu"<<Wlike.Data()<<WCharge_str[c].Data()<< " with |eta| < " << WMass::etaMaxMuons[ieta] << endl;
        outTXTfile << "-----------------------" << endl;
        outTXTfile << "-----------------------" << endl;
        outTXTfile << endl;
        // TDirectory *channel_dir = foutDATA->mkdir(Form("MuPos_pdf%d-%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data()));
        TDirectory *channel_dir = foutDATA->mkdir(Form("Mu%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),eta_str.Data()));
        // foutDATA->cd(Form("MuPos_pdf%d-%d_eta%s/",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data()));
        foutDATA->cd(Form("Mu%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),eta_str.Data()));
        
        //LOOP OVER w MASS BINS
        for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
          int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
          
          // double fitrange_Scaling = double(jWmass)/double(WMass::WMassCentral_MeV);
          double fitrange_Scaling = 1;
          
          cout << "W"<<Wlike.Data()<<WCharge_str[c]<<" eta cut " << WMass::etaMaxMuons[ieta]<< " jWmass= " << jWmass; fflush(stdout);
          TDirectory *mass_dir = channel_dir->mkdir(Form("%d",jWmass));
          mass_dir->cd();
          
          for(int h=0; h<WMass::PDF_members; h++){
            cout << " PDF " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets)<<"-"<<h; fflush(stdout);
            for(int m=0; m<WMass::NVarRecoilCorr; m++){
              if(WMass::NVarRecoilCorr>1) cout << " MomScale toy " << m; fflush(stdout);
              // gDirectory->pwd();
              // TH1D*data_obs=new TH1D("data_obs","data_obs",TemplatesW[h][DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)-TemplatesW[h][DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),xmin,xmax);
              outTXTfile << "-----------------------" << endl;
              outTXTfile << "Mass hypothesis  " << jWmass << " PDF " << Form("%d-%d%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NVarRecoilCorr>1?Form(" MomCorrToy%d",m):"") << endl;
              outTXTfile << "-----------------------" << endl;
              outTXTfile << endl;

              TString Z_histoname[2*Nsamples], W_histoname[2*Nsamples];
              TString Z_histoname_NonScaled[2*Nsamples], W_histoname_NonScaled[2*Nsamples];
              double Z_integrals[2*Nsamples], W_integrals[2*Nsamples];
              
              //LOOP OVER THE SAMPLES
              for(int isample=0; isample<Nsamples;isample++){
                // if(isample==0)cout << "\n";
                // cout << "using " << samples_str[isample].Data() << endl;
                if(samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD")) continue;
                
                for(int k=0;k<WMass::NFitVar;k++){
                  // DEFINE NEW HISTO NAMES
                  // Z_histoname[isample] = Form("Z_%s_%s_pdf%d-%d%s",samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"");
                  W_histoname[isample] = samples_str[isample] == "DATA" ? Form("data_obs_W%s%s_%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data()) : Form("W%s%s_%s_%s_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"");
                  // cout << W_histoname[isample] << endl;
                  // DECLARE NEW HISTOS
                  TH1D*Ztempl,*Wtempl;
                  TH1D*Ztempl_NonScaled, *Wtempl_NonScaled;
                  // SAME AS BEFORE FOR NON-SCALED VARIABLES
                  // if(!(samples_str[isample].Contains("WJetsSig") || samples_str[isample].Contains("QCD"))){
                    // Ztempl=new TH1D(Z_histoname[isample],Z_histoname[isample],TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling)-TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*fitrange_Scaling);
                    // Z_histoname_NonScaled[isample] = Form("Z_%s_%s_NonScaled_pdf%d-%d%s",samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"");
                    // Ztempl_NonScaled=new TH1D(Z_histoname_NonScaled[isample],Z_histoname_NonScaled[isample],TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*80/2*fitrange_Scaling)-TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*80/2*fitrange_Scaling);
                  // }
                  if(!(samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsSig":"WJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD"))){
                    Wtempl=new TH1D(W_histoname[isample],W_histoname[isample],TemplatesW[c][m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling)-TemplatesW[c][m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*fitrange_Scaling);
                    W_histoname_NonScaled[isample] = samples_str[isample] == "DATA" ? Form("data_obs_W%s%s_%sNonScaled",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data()) : Form("W%s%s_%s_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"");
                    // Wtempl_NonScaled=new TH1D(W_histoname_NonScaled[isample],W_histoname_NonScaled[isample],TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*80/2*fitrange_Scaling)-TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*80/2*fitrange_Scaling);
                    Wtempl_NonScaled=new TH1D(W_histoname_NonScaled[isample],W_histoname_NonScaled[isample],50, WMass::fit_xmin[k],WMass::fit_xmax[k]);
                  }
                  
                  // TH1D*Ztempl=(TH1D*)TemplatesZ[h][isample][ieta][jmass]->Clone(Z_histoname[isample]);
                  // TH1D*Wtempl=(TH1D*)TemplatesW[h][isample][ieta][jmass]->Clone(W_histoname[isample]);
                  int ibin=1;      
                  double z_templ_sign = 1 /* -1 */;
                  // if( Z_histoname[isample].Contains("DYJetsSig") || Z_histoname[isample].Contains("DATA") ) z_templ_sign = 1;
                  int start_bin=1, end_bin=2;
                  
                  if(!(samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsSig":"WJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD"))){
                    // start_bin = TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling);
                    // end_bin = TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling);
                    start_bin = TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(WMass::fit_xmin[k]/* /(WMass::WMassCentral_MeV/1e3) */);
                    end_bin = TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(WMass::fit_xmax[k]/* /(WMass::WMassCentral_MeV/1e3) */-1e-10);
                  // }else{
                    // start_bin = TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling);
                    // end_bin = TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling);
                    // cout<<WMass::fit_xmin[k] << " " << start_bin << " " << WMass::fit_xmax[k] << " " << end_bin << endl;
                  }
                    
                  
                  for(int ibinhisto=start_bin; ibinhisto<end_bin; ibinhisto++){
                    // if(!(samples_str[isample].Contains("WJetsSig") || samples_str[isample].Contains("QCD"))){
                      // Ztempl->SetBinContent(ibin,z_templ_sign*TemplatesZ[m][h][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                      // Ztempl_NonScaled->SetBinContent(ibin,z_templ_sign*TemplatesZ_NonScaled[m][h][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                    // }
                    if(!(samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsSig":"WJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD"))){
                      Wtempl->SetBinContent(ibin,TemplatesW[c][m][h][k][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                      Wtempl_NonScaled->SetBinContent(ibin,TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                      // cout << "inspecting after filling, ibinhisto="<<ibinhisto<<" start_bin="<<start_bin<< " end_bin="<<end_bin<<endl;
                      // TemplatesW_NonScaled[c][m][h][k][isample][ieta][jmass]->Print();
                      // Wtempl_NonScaled->Print();
                    }
                    ibin++;
                  }

                  // if(!(samples_str[isample].Contains("WJetsSig") || samples_str[isample].Contains("QCD"))){
                    // Ztempl->Write();
                    // Ztempl_NonScaled->Write();
                  // }
                  if(!(samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsSig":"WJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD"))){
                    // Wtempl->Write();
                    // Wtempl_NonScaled->Scale(norm_factor_to_match_data);
                    Wtempl_NonScaled->Write();
                  }
                  
                  if(!(samples_str[isample].Contains(WorZ.Contains("W")?"WJetsSig":"DYJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsSig":"WJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD"))){
                    // outTXTfile << Ztempl->GetName();
                    // int nspaces1 = 15 - Z_histoname[isample].Length();
                    // for(int ispace=0;ispace<nspaces1;ispace++) outTXTfile << " ";
                      // Z_integrals[isample] = Ztempl->Integral();
                      // outTXTfile << Z_integrals[isample];

                    // std::ostringstream s; s << Ztempl->Integral();
                    // TString number_str = s.str();
                    // int nspaces2 = 15 - number_str.Length();

                    int nspaces1 = 15 - W_histoname[isample].Length();
                    // for(int ispace=0;ispace<nspaces2;ispace++) outTXTfile << " ";
                    outTXTfile << Wtempl->GetName();
                    for(int ispace=0;ispace<nspaces1;ispace++) outTXTfile << " ";
                    W_integrals[isample] = Wtempl->Integral();
                    outTXTfile << W_integrals[isample] << endl;
                  }

                  // Wtempl_NonScaled->Print();
                  // cout << "integral= " << Wtempl_NonScaled->Integral() << " mean= " << Wtempl_NonScaled->GetMean() << " rms= " << Wtempl_NonScaled->GetRMS() << endl;

                  
                  // if(!(samples_str[isample].Contains("WJetsSig") || samples_str[isample].Contains("QCD"))){
                    // Ztempl->Delete();
                    // Ztempl_NonScaled->Delete();
                  // }
                  if(!(samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsSig":"WJetsSig") || samples_str[isample].Contains(WorZ.Contains("W")?"DYJetsFake":"WJetsFake") || samples_str[isample].Contains("QCD"))){
                    Wtempl->Delete();
                    Wtempl_NonScaled->Delete();
                  }
                }
              }
              outTXTfile << endl;
              
              // // PREPARE DUMMY DATACARD

              // PREPARE DUMMY DATACARD NON SCALED WITH BACKGROUND WITHOUT Z DATADRIVEN
              for(int k=0;k<WMass::NFitVar;k++){

                ofstream DummyDatacard;
                DummyDatacard.open(Form("%s/DataCards/dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s_eta%s_%d_%sNonScaled.txt",folder.Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));
                
                DummyDatacard << "shapes   *          *   datacards_DATA"<<(WorZ.Contains("W")?"":"_Wlike")<<".root $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
                // cout << "syst_folder.Data()= -" << syst_folder.Data() << "- syst_folder.Length()= " << syst_folder.Length() << endl;
                if(syst_folder.Length()<15){
                  DummyDatacard << "shapes   data_obs   *   datacards_DATA"<<(WorZ.Contains("W")?"":"_Wlike")<<".root $CHANNEL/"<<(WMass::WMassCentral_MeV)<<Form("/W%s%s_MCDATALIKE_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",0):"") << endl;
                }else{
                  // DummyDatacard << "shapes   data_obs   *   /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/analysis/JobOutputs/test_controlplots_RochCorr_EffSFCorr_PileupSFCorr/DataCards/datacards_DATA.root $CHANNEL/"<<(WMass::WMassCentral_MeV)<<"/W_MCDATALIKE_NonScaled" << endl;
                  DummyDatacard << "shapes   data_obs   *   "<<Form("../../%s/DataCards/datacards_DATA%s.root",syst_folder.Data(),WorZ.Contains("W")?"":"_Wlike") << " $CHANNEL/"<<(WMass::WMassCentral_MeV)<<Form("/W%s%s_MCDATALIKE_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",0):"") << endl;
                }
                DummyDatacard << Form("shapes   W%s%s_%sJetsSig_%sNonScaled_ALT   *   datacards_DATA%s.root $CHANNEL/",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",WMass::FitVar_str[k].Data(),WorZ.Contains("W")?"":"_Wlike")<<(WMass::WMassCentral_MeV-WMass::WMassNSteps*WMass::WMassStep_MeV)<<Form("/W%s%s_%sJetsSig_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"") << endl;
                DummyDatacard << "----------------------------------------" << endl;
                // DummyDatacard << "bin                 MuPos_pdf"<<(WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets)<<"-"<<h<<"_eta"<<eta_str<< endl;
                DummyDatacard << "bin                 Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<< endl;
                DummyDatacard << "observation              -1 " << endl;
                DummyDatacard << "----------------------------------------" << endl;
                DummyDatacard << "bin            Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<<"          Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<<"         Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<< endl;
                DummyDatacard << Form("process        W%s%s_%sJetsSig_%sNonScaled_pdf%d-%d%s        W%s%s_%sJetsSig_%sNonScaled_ALT        W%s%s_EWKTT_%sNonScaled_pdf%d-%d%s        ",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",WMass::FitVar_str[k].Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"") << endl;   
                // DummyDatacard << Form("process        W%s%s_%sJetsSig_%sNonScaled_pdf%d-%d%s        W%s%s_%sJetsSig_%sNonScaled_ALT        W%s%s_WJetsFake_%sNonScaled_pdf%d-%d%s        ",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",WMass::FitVar_str[k].Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NVarRecoilCorr>1?Form("_RecoilCorrVar%d",m):"") << endl;   
                DummyDatacard << "process               -1                             0                            1   " << endl;
                DummyDatacard << "rate                 -1                             -1                            -1  " << endl;
                // cout          << "rate                 -1                             -1                          -1    " << endl;
                DummyDatacard << "--------------------------------------------------------------------------------" << endl;
                DummyDatacard << "lumi    lnN    1.044    1.044   1.044 " << endl;
                DummyDatacard.close();
              }

            }
          }
          cout << endl;
        }
        cout << endl;
        outTXTfile << endl;
        outTXTfile << endl;
      }
      outTXTfile.close();
    }
    
    foutDATA->Write();
    foutDATA->Close();
    foutDATA->Delete();
    
    cout << "program ended..." << endl;
    
  }
}




// bin              1     1     1     1
// process         ggH  qqWW  ggWW  others
// process          0     1     2     3
// rate           1.47  0.63  0.06  0.22
// ------------
// # then we list the independent sources of uncertainties, and give their effect (syst. error)
// # on each process and bin
// lumi    lnN    1.11    -   1.11    -    lumi affects both signal and gg->WW (mc-driven). lnN = lognormal
// xs_ggH  lnN    1.16    -     -     -    gg->H cross section double sf_WDATA = signal efficiency double sf_WDATA = other minor ones.
// WW_norm gmN 4    -   0.16    -     -    WW estimate of 0.64 comes from sidebands: 4 events in sideband times 0.16 (=> ~50% statistical uncertainty)
// xs_ggWW lnN      -     -   1.50    -    50% uncertainty on gg->WW cross section
// bg_others lnN    -     -     -   1.30   30% uncertainty on the rest of the backgrounds
