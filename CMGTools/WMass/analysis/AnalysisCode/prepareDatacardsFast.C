#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "common.h"
#include "common2.h"

using namespace std;

void prepareDatacardsFast(TString folder, TString template_folder, TString SignalSample, int generated_PDF_set=1, int generated_PDF_member=0, TString WorZ="W", int RecoilCorrVarDiagoParU1orU2fromDATAorMC=0){

  int m_start = WMass::RecoilCorrIniVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  int m_end = WMass::RecoilCorrNVarDiagoParU1orU2fromDATAorMC_[RecoilCorrVarDiagoParU1orU2fromDATAorMC];
  // int m_end = 9;

  cout << "m_start= " << m_start << " m_end= " << m_end << endl;

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

    static const int Nsamples=23;
    enum                             {  DATA, WJetsPowPlus,  WJetsPowNeg,  WJetsMadSig,  WJetsMadFake,  DYJetsPow,  DYJetsMadSig,  DYJetsMadFake,   TTJets,   ZZJets,   WWJets,  WZJets,  QCD, T_s, T_t, T_tW, Tbar_s, Tbar_t, Tbar_tW, EWK, EWKTT, MCDATALIKEPOW, MCDATALIKEMAD  };
    TString samples_str[Nsamples]  = { "DATA" , "WJetsPowPlus", "WJetsPowNeg", "WJetsMadSig", "WJetsMadFake", "DYJetsPow", "DYJetsMadSig", "DYJetsMadFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets",   "QCD", "T_s", "T_t", "T_tW", "Tbar_s", "Tbar_t", "Tbar_tW", "EWK", "EWKTT", "MCDATALIKEPOW", "MCDATALIKEMAD" };
    TString WCharge_str[]={"Pos","Neg"};

    cout << "folder= " << folder << endl;
    cout << "SignalSample= " << SignalSample << endl;

    WorZ = tokenized.at(itoken); // "Z" or "W"
    TString Wlike = WorZ.Contains("W")?"":"like";
    int charges = WorZ.Contains("W")?2:1;
    cout << "WorZ= " << WorZ << endl;

    // TOKENIZE SAMPLES
    TFile* finTemplatesW[Nsamples];
    TH1D *TemplatesW_NonScaled[2][m_end-m_start][WMass::KalmanNvariations][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

    // LOAD ALL THE HISTOS FROM THE VARIOUS FILES IN MEMORY
    for(int isample=0; isample<Nsamples;isample++){
      finTemplatesW[isample] = new TFile(Form("%s/output_%s/%sanalysisOnDATA.root",folder.Data(),samples_str[isample].Data(),WorZ.Data()));
      finTemplatesW[isample]->Print();
      for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
        for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
          // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
          int jWmass = WorZ.Contains("Z")? WMass::Zmass_values_array[jmass] : WMass::Wmass_values_array[jmass];

          for(int h=0; h<WMass::PDF_members; h++){
            for(int m=m_start; m<m_end; m++){
              for(int n=0; n<WMass::KalmanNvariations; n++){
                for(int k=0;k<WMass::NFitVar;k++){
                  for(int c=0;c<charges;c++){
                    // TemplatesW[c][m][n][h][k][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hW%s%s_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",eta_str.Data(),jWmass));

                    TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hW%s%s_%sNonScaled_8_JetCut_pdf%d-%d%s%s_eta%s_%d",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass));
                    if(TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]) TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]->Print();
                  }
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
            for(int m=m_start; m<m_end; m++){
              for(int n=0; n<WMass::KalmanNvariations; n++){
                for(int k=0;k<WMass::NFitVar;k++){
                  for(int c=0;c<charges;c++){

                    if(TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]){
                      double int_hist_data = 0;
                      if(TemplatesW_NonScaled[c][0][0][0][k][isample][ieta][WMass::WMassNSteps])
                        int_hist_data =TemplatesW_NonScaled[c][0][0][0][k][isample][ieta][WMass::WMassNSteps]->Integral();
                      double int_hist_mcdatalike = TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]->Integral();
                      double norm_factor_to_match_data = int_hist_mcdatalike>0 && int_hist_data>0 ? int_hist_data/int_hist_mcdatalike : 1;

                      TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]->Scale(norm_factor_to_match_data);
                      cout <<"c "<<c<<" m "<<m<<" n "<<n<<" h "<<h<<" k "<<k<<" isample "<<samples_str[isample]<<" ieta "<<ieta<<" jmass "<<jmass<<" norm "<<norm_factor_to_match_data<<endl;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    int histcounter = 0;
    int dcardcounter = 0;

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
        outTXTfile << "Mu"<<Wlike.Data()<<WCharge_str[c].Data()<< " with |eta| < " << WMass::etaMaxMuons[ieta] << endl;
        outTXTfile << "-----------------------" << endl;
        outTXTfile << "-----------------------" << endl;
        outTXTfile << endl;
        TDirectory *channel_dir = foutDATA->mkdir(Form("Mu%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),eta_str.Data()));
        foutDATA->cd(Form("Mu%s%s_eta%s",Wlike.Data(),WCharge_str[c].Data(),eta_str.Data()));

        //LOOP OVER w MASS BINS
        for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
          // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
          int jWmass = WorZ.Contains("Z")? WMass::Zmass_values_array[jmass] : WMass::Wmass_values_array[jmass];

          // double fitrange_Scaling = 1;

          cout << "W"<<Wlike.Data()<<WCharge_str[c]<<" eta cut " << WMass::etaMaxMuons[ieta]<< " jWmass= " << jWmass; fflush(stdout);
          TDirectory *mass_dir = channel_dir->mkdir(Form("%d",jWmass));
          mass_dir->cd();

          for(int h=0; h<WMass::PDF_members; h++){
            cout << " PDF " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets) << "-" << h << endl;
            for(int m=m_start; m<m_end; m++){
              for(int n=0; n<WMass::KalmanNvariations; n++){
                if(RecoilCorrVarDiagoParU1orU2fromDATAorMC>0) cout << "Recoil eigen " << m << '\t'; fflush(stdout);
                if(WMass::KalmanNvariations>1) cout << "KalmanVar " << n << '\t'; fflush(stdout);
                // gDirectory->pwd();
                outTXTfile << "-----------------------" << endl;
                outTXTfile << "Mass hypothesis  " << jWmass << " PDF " << Form("%d-%d%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form(" RecoilCorrVar%d",m):"") << endl;
                outTXTfile << "-----------------------" << endl;
                outTXTfile << endl;

                TString Z_histoname[2*Nsamples], W_histoname[2*Nsamples];
                TString Z_histoname_NonScaled[2*Nsamples], W_histoname_NonScaled[2*Nsamples];
                // double Z_integrals[2*Nsamples];
                double W_integrals[2*Nsamples];

                //LOOP OVER THE SAMPLES
                for(int isample=0; isample<Nsamples;isample++){
                  for(int k=0;k<WMass::NFitVar;k++){

                    if(!TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]) continue;
                    histcounter++;

                    // DECLARE NEW HISTOS
                    TH1D* Wtempl_NonScaled;

                    // To BE CHECKED
                    // W_histoname[isample] = samples_str[isample] == "DATA" ? Form("data_obs_W%s%s_%s",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data()) : Form("W%s%s_%s_%s_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"");
                    // Wtempl=new TH1D(W_histoname[isample],W_histoname[isample],TemplatesW[c][m][n][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling)-TemplatesW[c][m][n][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*fitrange_Scaling);
                    W_histoname_NonScaled[isample] = samples_str[isample] == "DATA" ? Form("data_obs_W%s%s_%sNonScaled",Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data()) : Form("W%s%s_%s_%sNonScaled_pdf%d-%d%s%s",Wlike.Data(),WCharge_str[c].Data(),samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"");
                    // Wtempl_NonScaled=new TH1D(W_histoname_NonScaled[isample],W_histoname_NonScaled[isample],50, WMass::fit_xmin[k],WMass::fit_xmax[k]);
                    Wtempl_NonScaled=(TH1D*)TemplatesW_NonScaled[c][m][n][h][k][isample][ieta][jmass]->Clone(W_histoname_NonScaled[isample]);
                    Wtempl_NonScaled->SetName(W_histoname_NonScaled[isample]);
                    // cout << "W_histoname_NonScaled[isample]="<<W_histoname_NonScaled[isample]<<endl;
                    Wtempl_NonScaled->SetTitle(W_histoname_NonScaled[isample]);

                    Wtempl_NonScaled->Write();

                    int nspaces1 = 50 - W_histoname_NonScaled[isample].Length();
                    // outTXTfile << Wtempl->GetName();
                    outTXTfile << Wtempl_NonScaled->GetName();
                    for(int ispace=0;ispace<nspaces1;ispace++) outTXTfile << " ";
                    // W_integrals[isample] = Wtempl->Integral();
                    W_integrals[isample] = Wtempl_NonScaled->Integral();
                    outTXTfile << W_integrals[isample] << endl;

                    Wtempl_NonScaled->Delete();
                  }
                }
                outTXTfile << endl;

              }
            }
          }
        }
        cout << endl;
        outTXTfile << endl;
        outTXTfile << endl;
      }


      for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
        for(int jmass=0; jmass<2*WMass2::WMassNSteps+1; jmass++){
          int jWmass = WorZ.Contains("Z")? WMass2::Zmass_values_array[jmass] : WMass2::Wmass_values_array[jmass];
          for(int h=0; h<WMass::PDF_members; h++){
            for(int m=m_start; m<m_end; m++){
              for(int n=0; n<WMass::KalmanNvariations; n++){

                // PREPARE DATACARD
                TString SigSample_str = "MadSig";
                // PowPlus,  PowNeg,  MadSig,  Pow,  MadSig
                if(SignalSample.Contains("POWHEG")){
                  SigSample_str = "Pow";
                    if(WorZ.Contains("W")) SigSample_str+=WCharge_str[c];
                }

                // PREPARE DATACARD NON SCALED WITH BACKGROUND WITHOUT Z DATADRIVEN
                for(int k=0;k<WMass::NFitVar;k++){

                  ofstream Datacard;
                  Datacard.open(Form("%s/DataCards/dummy_datacard_Wmass_Mu%s%s_pdf%d-%d%s%s_eta%s_%d_%sNonScaled.txt",folder.Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));

                  if(template_folder.Length()<5){
                    Datacard << "shapes   *          *   datacards_DATA"<<(WorZ.Contains("W")?"":"_Wlike")<<".root $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
                  }else{
                    Datacard << "shapes   *          *   "<<Form("../%s/DataCards/datacards_DATA%s.root",template_folder.Data(),WorZ.Contains("W")?"":"_Wlike") << " $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
                  }

                  // Datacard << "shapes   data_obs   *   datacards_DATA"<<(WorZ.Contains("W")?"":"_Wlike")<<".root $CHANNEL/"<<(WMass2::WMassCentral_MeV)<<Form("/W%s%s_MCDATALIKE%s_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),SignalSample.Contains("POWHEG")?"POW":"MAD",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"") << endl;
                  Datacard << "shapes   data_obs   *   datacards_DATA"<<(WorZ.Contains("W")?"":"_Wlike")<<".root $CHANNEL/"<<(WorZ.Contains("W") ? WMass2::WMassCentral_MeV : WMass2::ZMassCentral_MeV)<<Form("/W%s%s_MCDATALIKE%s_%sNonScaled_pdf%d-%d%s%s",Wlike.Data(),WCharge_str[c].Data(),SignalSample.Contains("POWHEG")?"POW":"MAD",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"",WMass::KalmanNvariations>1?Form("_KalmanVar%d",n):"") << endl;

                  if(template_folder.Length()<5){
                    // Datacard << Form("shapes   W%s%s_%sJets%s_%sNonScaled_ALT   *   datacards_DATA%s.root $CHANNEL/",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WorZ.Contains("W")?"":"_Wlike")<<(WMass2::WMassCentral_MeV-WMass2::WMassNSteps*WMass2::WMassStep_MeV)<<Form("/W%s%s_%sJets%s_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, RecoilCorrVarDiagoParU1orU2fromDATAorMC>0?Form("_RecoilCorrVar%d",m):"") << endl;
                    Datacard << Form("shapes   W%s%s_%sJets%s_%sNonScaled_ALT   *   datacards_DATA%s.root $CHANNEL/",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WorZ.Contains("W")?"":"_Wlike")<<(WorZ.Contains("Z")? WMass::Zmass_values_array[0] : WMass::Wmass_values_array[0])<<Form("/W%s%s_%sJets%s_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, "") << endl;
                  }else{
                    // Datacard << Form("shapes   W%s%s_%sJets%s_%sNonScaled_ALT   *   ../%s/DataCards/datacards_DATA%s.root $CHANNEL/",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),template_folder.Data(),WorZ.Contains("W")?"":"_Wlike")<<(WMass2::WMassCentral_MeV-WMass2::WMassNSteps*WMass2::WMassStep_MeV)<<Form("/W%s%s_%sJets%s_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, "") << endl;
                    Datacard << Form("shapes   W%s%s_%sJets%s_%sNonScaled_ALT   *   ../%s/DataCards/datacards_DATA%s.root $CHANNEL/",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),template_folder.Data(),WorZ.Contains("W")?"":"_Wlike")<<(WorZ.Contains("Z")? WMass::Zmass_values_array[0] : WMass::Wmass_values_array[0])<<Form("/W%s%s_%sJets%s_%sNonScaled_pdf%d-%d%s",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, "") << endl;

                  }

                  Datacard << "---------------------------------------------------------------------------------" << endl;
                  Datacard << "bin            Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<< endl;
                  Datacard << "observation          -1 " << endl;
                  Datacard << "---------------------------------------------------------------------------------" << endl;
                  Datacard << "bin            Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<<"          Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<<"         Mu"<<Wlike.Data()<<WCharge_str[c].Data()<<"_eta"<<eta_str<< endl;
                  Datacard << Form("process        W%s%s_%sJets%s_%sNonScaled_pdf%d-%d%s        W%s%s_%sJets%s_%sNonScaled_ALT        W%s%s_EWKTT_%sNonScaled_pdf%d-%d%s        ",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, "",Wlike.Data(),WCharge_str[c].Data(),WorZ.Contains("W")?"W":"DY",SigSample_str.Data(),WMass::FitVar_str[k].Data(),Wlike.Data(),WCharge_str[c].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, "") << endl;
                  Datacard << "process              -1                            0                            1" << endl;
                  Datacard << "rate                 -1                           -1                           -1" << endl;
                  Datacard.close();

                  dcardcounter++;
                }
              }
            }
          }
        }
      }

      outTXTfile.close();
    }

    foutDATA->Write();
    foutDATA->Close();
    foutDATA->Delete();

    cout << "Program ended, " << histcounter << " histograms found and " << dcardcounter << " datacards written" << endl;

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
