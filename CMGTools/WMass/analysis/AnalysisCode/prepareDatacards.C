#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "../includes/common.h"

static const int Nsamples=13;
enum                                 {  WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD     ,  DATA  ,  EWK  ,  EWKTT  ,  MCDATALIKE  };
TString     samples_str[Nsamples]  = { "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"    , "DATA" , "EWK" , "EWKTT" , "MCDATALIKE" };

// double xmin=0.8, xmax=1.2;
double xmin=0.65, xmax=2;

void prepareDatacards(TString folder, TString syst_folder, TString sample, int generated_PDF_set=1, int generated_PDF_member=0){

  cout << "folder= " << folder << endl;
  cout << "samples= " << sample << endl;
  
  // TOKENIZE SAMPLES
  // std::vector<TString> samples_str;
  TFile* finTemplatesZ[Nsamples], *finTemplatesW[Nsamples];
  TH1D *TemplatesZ[WMass::NtoysMomCorr][WMass::PDF_members][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *TemplatesZ_NonScaled[WMass::NtoysMomCorr][WMass::PDF_members][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH1D *TemplatesW[WMass::NtoysMomCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *TemplatesW_NonScaled[WMass::NtoysMomCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *TemplatesW[WMass::NtoysMomCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *TemplatesW_NonScaled[WMass::NtoysMomCorr][WMass::PDF_members][WMass::NFitVar][Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // vector <*TH1D>
  // std::vector<std::vector<std::vector< std::vector< std::vector< std::vector< *TH1D > > > > > >;
  // TObjArray* samples_obj = sample.Tokenize("-");
  // int Nsamples = samples_obj->GetEntries();
  
  // LOAD ALL THE HISTOS FROM THE VARIOUS FILES IN MEMORY
  for(int isample=0; isample<Nsamples;isample++){
    // samples_str.push_back( ((TObjString *)samples_obj->At(isample))->GetString() );
    finTemplatesZ[isample] = new TFile(Form("%s/test_numbers_%s/WSimpleTemplates.root",folder.Data(),samples_str[isample].Data()));
    finTemplatesW[isample] = new TFile(Form("%s/test_numbers_%s/WanalysisOnDATA.root",folder.Data(),samples_str[isample].Data()));
    finTemplatesZ[isample]->Print();
    finTemplatesW[isample]->Print();
    for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
      for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
        int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
        
        for(int h=0; h<WMass::PDF_members; h++){
            for(int m=0; m<WMass::NtoysMomCorr; m++){
            TemplatesZ[m][h][isample][ieta][jmass] = (TH1D*) finTemplatesZ[isample]->Get(Form("hWlikePos_PtScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass));
            TemplatesZ_NonScaled[m][h][isample][ieta][jmass] = (TH1D*) finTemplatesZ[isample]->Get(Form("hWlikePos_PtNonScaled_RWeighted_SimpleTemplates_pdf%d-%d%s_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass));
            // cout << Form("hWlikePos_PtNonScaled_RWeighted_SimpleTemplates_pdf%d-%d_eta%s_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data(),jWmass) << endl;
            // TemplatesZ_NonScaled[h][isample][ieta][jmass]->Print();
            for(int k=0;k<3;k++){
              TemplatesW[m][h][k][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hWPos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass));
              // TemplatesW_NonScaled[h][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hWPos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
              
              // N.B. IS ALWAYS THE SAME TEMPLATE!!! MASS INDEX IS FIXED TO MC GENERATED W MASS, i.e. WMass::WMassCentral_MeV)
              // TemplatesW_NonScaled[h][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hWPos_PtNonScaled_8_JetCut_eta%s_%d",eta_str.Data(),WMass::WMassCentral_MeV));
              TemplatesW_NonScaled[m][h][k][isample][ieta][jmass] = (TH1D*) finTemplatesW[isample]->Get(Form("hWPos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass));
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
  

  // PROCESS THE HISTOS AND STORE THEM IN A SUITABLE FILE, STORE NORMALIZATIONS IN A SEPARATE TEXT FILE
  TFile *foutDATA = new TFile(Form("%s/DataCards/datacards_DATA.root",folder.Data()),"RECREATE");
  ofstream outTXTfile;
  outTXTfile.open(Form("%s/DataCards/datacard_Wmass_MuPos_normalizations.txt",folder.Data()));
  
  // LOOP OVER MAX-ETA BINS
    
  for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
    outTXTfile << "-----------------------" << endl;
    outTXTfile << "-----------------------" << endl;
    // outTXTfile << "MuPos with |eta| < " << WMass::etaMaxMuons[ieta] << " PDF " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets)<<"-"<<h<< endl;
    outTXTfile << "MuPos with |eta| < " << WMass::etaMaxMuons[ieta] << endl;
    outTXTfile << "-----------------------" << endl;
    outTXTfile << "-----------------------" << endl;
    outTXTfile << endl;
    // TDirectory *channel_dir = foutDATA->mkdir(Form("MuPos_pdf%d-%d_eta%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data()));
    TDirectory *channel_dir = foutDATA->mkdir(Form("MuPos_eta%s",eta_str.Data()));
    // foutDATA->cd(Form("MuPos_pdf%d-%d_eta%s/",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,eta_str.Data()));
    foutDATA->cd(Form("MuPos_eta%s",eta_str.Data()));

    //LOOP OVER w MASS BINS
    for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
      
      // double fitrange_Scaling = double(jWmass)/double(WMass::WMassCentral_MeV);
      double fitrange_Scaling = 1;
      
      cout << "eta cut " << WMass::etaMaxMuons[ieta]<< " jWmass= " << jWmass; fflush(stdout);
      TDirectory *mass_dir = channel_dir->mkdir(Form("%d",jWmass));
      mass_dir->cd();
      
      for(int h=0; h<WMass::PDF_members; h++){
        cout << " PDF " << (WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets)<<"-"<<h; fflush(stdout);
        for(int m=0; m<WMass::NtoysMomCorr; m++){
          if(WMass::NtoysMomCorr>1) cout << " MomScale toy " << m; fflush(stdout);
          // gDirectory->pwd();
          // TH1D*data_obs=new TH1D("data_obs","data_obs",TemplatesW[h][DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)-TemplatesW[h][DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),xmin,xmax);
          outTXTfile << "-----------------------" << endl;
          outTXTfile << "Mass hypothesis  " << jWmass << " PDF " << Form("%d-%d%s",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,WMass::NtoysMomCorr>1?Form(" MomCorrToy%d",m):"") << endl;
          outTXTfile << "-----------------------" << endl;
          outTXTfile << endl;

          TString Z_histoname[2*Nsamples], W_histoname[2*Nsamples];
          TString Z_histoname_NonScaled[2*Nsamples], W_histoname_NonScaled[2*Nsamples];
          double Z_integrals[2*Nsamples], W_integrals[2*Nsamples];
          
          //LOOP OVER THE SAMPLES
          for(int isample=0; isample<Nsamples;isample++){
            for(int k=0;k<3;k++){
          
              // DEFINE NEW HISTO NAMES
              Z_histoname[isample] = Form("Z_%s_%s_pdf%d-%d%s",samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"");
              W_histoname[isample] = samples_str[isample] == "DATA" ? Form("data_obs_%s",WMass::FitVar_str[k].Data()) : Form("W_%s_%s_pdf%d-%d%s",samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"");
              // DECLARE NEW HISTOS
              TH1D*Ztempl=new TH1D(Z_histoname[isample],Z_histoname[isample],TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling)-TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*fitrange_Scaling);
              TH1D*Wtempl=new TH1D(W_histoname[isample],W_histoname[isample],TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling)-TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*fitrange_Scaling);
              // SAME AS BEFORE FOR NON-SCALED VARIABLES
              Z_histoname_NonScaled[isample] = Form("Z_%s_%s_NonScaled_pdf%d-%d%s",samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"");
              W_histoname_NonScaled[isample] = samples_str[isample] == "DATA" ? "data_obs_NonScaled" : Form("W_%s_%sNonScaled_pdf%d-%d%s",samples_str[isample].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"");
              TH1D*Ztempl_NonScaled=new TH1D(Z_histoname_NonScaled[isample],Z_histoname_NonScaled[isample],TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*80/2*fitrange_Scaling)-TemplatesZ[m][h][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*80/2*fitrange_Scaling);
              TH1D*Wtempl_NonScaled=new TH1D(W_histoname_NonScaled[isample],W_histoname_NonScaled[isample],TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*80/2*fitrange_Scaling)-TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling),xmin*fitrange_Scaling,xmax*80/2*fitrange_Scaling);
              // TH1D*Ztempl=(TH1D*)TemplatesZ[h][isample][ieta][jmass]->Clone(Z_histoname[isample]);
              // TH1D*Wtempl=(TH1D*)TemplatesW[h][isample][ieta][jmass]->Clone(W_histoname[isample]);

              int ibin=1;      
              double z_templ_sign = 1 /* -1 */;
              // if( Z_histoname[isample].Contains("DYJetsSig") || Z_histoname[isample].Contains("DATA") ) z_templ_sign = 1;
              for(int ibinhisto=TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmin*fitrange_Scaling); ibinhisto<TemplatesW[m][h][k][isample][ieta][jmass]->GetXaxis()->FindBin(xmax*fitrange_Scaling); ibinhisto++){
                Ztempl->SetBinContent(ibin,z_templ_sign*TemplatesZ[m][h][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                Wtempl->SetBinContent(ibin,TemplatesW[m][h][k][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                Ztempl_NonScaled->SetBinContent(ibin,z_templ_sign*TemplatesZ_NonScaled[m][h][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                Wtempl_NonScaled->SetBinContent(ibin,TemplatesW_NonScaled[m][h][k][isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
                ibin++;
              }
              if(!samples_str[isample].Contains("WJetsSig")){
                Ztempl->Write();
                Ztempl_NonScaled->Write();
              }
              if(!samples_str[isample].Contains("DYJetsSig")){
                Wtempl->Write();
                Wtempl_NonScaled->Write();
              }
              
              outTXTfile << Ztempl->GetName();
              int nspaces1 = 15 - Z_histoname[isample].Length();
              for(int ispace=0;ispace<nspaces1;ispace++) outTXTfile << " ";
              Z_integrals[isample] = Ztempl->Integral();
              outTXTfile << Z_integrals[isample];

              std::ostringstream s; s << Ztempl->Integral();
              TString number_str = s.str();
              int nspaces2 = 15 - number_str.Length();

              for(int ispace=0;ispace<nspaces2;ispace++) outTXTfile << " ";
              outTXTfile << Wtempl->GetName();
              for(int ispace=0;ispace<nspaces1;ispace++) outTXTfile << " ";
              W_integrals[isample] = Wtempl->Integral();
              outTXTfile << W_integrals[isample] << endl;

              // Wtempl_NonScaled->Print();
              // cout << "integral= " << Wtempl_NonScaled->Integral() << " mean= " << Wtempl_NonScaled->GetMean() << " rms= " << Wtempl_NonScaled->GetRMS() << endl;

              
              Wtempl->Delete();
              Ztempl->Delete();
              Wtempl_NonScaled->Delete();
              Ztempl_NonScaled->Delete();
            }
          }
          outTXTfile << endl;
          
          // // PREPARE DUMMY DATACARD

          // PREPARE DUMMY DATACARD NON SCALED WITH BACKGROUND WITHOUT Z DATADRIVEN
          for(int k=0;k<3;k++){

            ofstream DummyDatacard;
            DummyDatacard.open(Form("%s/DataCards/dummy_datacard_Wmass_MuPos_pdf%d-%d%s_eta%s_%d_%sNonScaled.txt",folder.Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",eta_str.Data(),jWmass,WMass::FitVar_str[k].Data()));
            
            DummyDatacard << "shapes   *          *   datacards_DATA.root $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
            // cout << "syst_folder.Data()= -" << syst_folder.Data() << "- syst_folder.Length()= " << syst_folder.Length() << endl;
            if(syst_folder.Length()<15){
              DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/"<<(WMass::WMassCentral_MeV)<<Form("/W_MCDATALIKE_%sNonScaled_pdf%d-%d%s",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",0):"") << endl;
            }else{
              // DummyDatacard << "shapes   data_obs   *   /afs/cern.ch/work/p/perrozzi/private/CMGTools/CMGTools/CMSSW_4_4_4/src/CMGTools/WMass/analysis/JobOutputs/test_controlplots_RochCorr_EffSFCorr_PileupSFCorr/DataCards/datacards_DATA.root $CHANNEL/"<<(WMass::WMassCentral_MeV)<<"/W_MCDATALIKE_NonScaled" << endl;
              DummyDatacard << "shapes   data_obs   *   "<<Form("../../%s/DataCards/datacards_DATA.root",syst_folder.Data()) << " $CHANNEL/"<<(WMass::WMassCentral_MeV)<<Form("/W_MCDATALIKE_%sNonScaled_pdf%d-%d%s",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,0, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",0):"") << endl;
            }
            DummyDatacard << Form("shapes   W_WJetsSig_%sNonScaled_ALT   *   datacards_DATA.root $CHANNEL/",WMass::FitVar_str[k].Data())<<(WMass::WMassCentral_MeV-WMass::WMassNSteps*WMass::WMassStep_MeV)<<Form("/W_WJetsSig_%sNonScaled_pdf%d-%d%s",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"") << endl;
            DummyDatacard << "----------------------------------------" << endl;
            // DummyDatacard << "bin                 MuPos_pdf"<<(WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets)<<"-"<<h<<"_eta"<<eta_str<< endl;
            DummyDatacard << "bin                 MuPos_eta"<<eta_str<< endl;
            DummyDatacard << "observation              -1 " << endl;
            DummyDatacard << "----------------------------------------" << endl;
            DummyDatacard << "bin            MuPos_eta"<<eta_str<<"          MuPos_eta"<<eta_str<<"         MuPos_eta"<<eta_str<< endl;
            DummyDatacard << Form("process        W_WJetsSig_%sNonScaled_pdf%d-%d%s        W_WJetsSig_%sNonScaled_ALT        W_EWKTT_%sNonScaled_pdf%d-%d%s        ",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"",WMass::FitVar_str[k].Data(),WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h, WMass::NtoysMomCorr>1?Form("_MomCorrToy%d",m):"") << endl;   
            DummyDatacard << "process               -1                             0                            1   " << endl;
            DummyDatacard << "rate                 -1                             -1                            -1  " << endl;
            // cout          << "rate                 -1                             -1                          -1    " << endl;
            DummyDatacard << "--------------------------------------------------------------------------------" << endl;
            DummyDatacard << "lumi    lnN    1.044    1.044   1.044 " << endl;
            DummyDatacard.close();
          }


          // ofstream DummyDatacard;
          // DummyDatacard.open(Form("%s/DataCards/dummy_datacard_Wmass_MuPos_eta%s_%d.txt",folder.Data(),eta_str.Data(),jWmass));
          
          // DummyDatacard << "shapes   *          *   datacards_DATA.root $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
          // // DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/$MASS/W_MCDATALIKE" << endl;
          // DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/80299/W_MCDATALIKE" << endl;
          // // DummyDatacard << "shapes data_PDG   *   datacards_DATA.root $CHANNEL/80385/W_MCDATALIKE  $CHANNEL/80385/data_$SYSTEMATIC" << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard << "bin                 MuPos_eta"<<eta_str<<"" << endl;
          // // DummyDatacard << "observation         " << Form("%f",W_integrals[MCDATALIKE]) << endl;
          // DummyDatacard << "observation              -1 " << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard << "bin            MuPos_eta"<<eta_str<<"       MuPos_eta"<<eta_str<<"" << endl;
          // DummyDatacard << "process        Z_DYJetsSig        W_EWKTT" << endl;   
          // DummyDatacard << "process           0                 1     " << endl;
          // // DummyDatacard << "rate           "<<Form("%f",Z_integrals[DYJetsSig])<<"           "<<Form("%f",W_integrals[EWKTT]) << endl;
          // DummyDatacard << "rate             -1                -1 " << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard.close();

          // // PREPARE DUMMY DATACARD NON SCALED
          // ofstream DummyDatacard;
          // DummyDatacard.open(Form("%s/DataCards/dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt",folder.Data(),eta_str.Data(),jWmass));
          
          // DummyDatacard << "shapes   *          *   datacards_DATA.root $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
          // // DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/$MASS/W_MCDATALIKE_NonScaled" << endl;
          // DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/80299/W_MCDATALIKE_NonScaled" << endl;
          // // DummyDatacard << shapes data_PDG   *   datacards_DATA.root $CHANNEL/80385/W_MCDATALIKE  $CHANNEL/80385/data_$SYSTEMATIC" << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard << "bin                 MuPos_eta"<<eta_str<<"" << endl;
          // // DummyDatacard << "observation         " << Form("%f",W_integrals[MCDATALIKE]) << endl;
          // DummyDatacard << "observation              -1 " << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard << "bin            MuPos_eta"<<eta_str<<"       MuPos_eta"<<eta_str<<"" << endl;
          // DummyDatacard << "process        Z_DYJetsSig_NonScaled        W_EWKTT_NonScaled" << endl;   
          // DummyDatacard << "process           0                 1     " << endl;
          // // DummyDatacard << "rate           "<<Form("%f",Z_integrals[DYJetsSig])<<"           "<<Form("%f",W_integrals[EWKTT]) << endl;
          // DummyDatacard << "rate             -1                -1 " << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard.close();

          // // PREPARE DUMMY DATACARD NON SCALED WITH BACKGROUND
          // ofstream DummyDatacard;
          // DummyDatacard.open(Form("%s/DataCards/dummy_datacard_Wmass_MuPos_eta%s_%d_NonScaled.txt",folder.Data(),eta_str.Data(),jWmass));
          
          // DummyDatacard << "shapes   *          *   datacards_DATA.root $CHANNEL/$MASS/$PROCESS $CHANNEL/$MASS/$PROCESS_$SYSTEMATIC" << endl;
          // DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/$MASS/W_MCDATALIKE_NonScaled" << endl;
          // DummyDatacard << "shapes   W_EWK_NonScaled_ALT   *   datacards_DATA.root $CHANNEL/"<<WMass::WMassCentral_MeV-(WMass::WMassNSteps-(WMass::WMassNSteps+1))*WMass::WMassStep_MeV<<"/W_EWK_NonScaled" << endl;
          // // DummyDatacard << "shapes   data_obs   *   datacards_DATA.root $CHANNEL/80299/W_MCDATALIKE_NonScaled" << endl;
          // // DummyDatacard << shapes data_PDG   *   datacards_DATA.root $CHANNEL/80385/W_MCDATALIKE  $CHANNEL/80385/data_$SYSTEMATIC" << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // DummyDatacard << "bin                 MuPos_eta"<<eta_str<<"" << endl;
          // // DummyDatacard << "observation         " << Form("%f",W_integrals[MCDATALIKE]) << endl;
          // DummyDatacard << "observation              -1 " << endl;
          // DummyDatacard << "----------------------------------------" << endl;
          // // DummyDatacard << "bin            MuPos_eta"<<eta_str<<"       MuPos_eta"<<eta_str<<"" << endl;
          // // DummyDatacard << "process        Z_DYJetsSig_NonScaled        W_EWKTT_NonScaled" << endl;   
          // // DummyDatacard << "process           0                 1     " << endl;
          // // DummyDatacard << "rate             -1                -1 " << endl;
          // // DummyDatacard << "bin            MuPos_eta"<<eta_str<<"       MuPos_eta"<<eta_str<<"       MuPos_eta"<<eta_str<< endl;
          // // DummyDatacard << "process        Z_DYJetsSig_NonScaled        W_EWK_NonScaled        W_TTJets_NonScaled" << endl;   
          // // DummyDatacard << "process               0                           1                      2     " << endl;
          // // DummyDatacard << "rate                 -1                          -1                     -1 " << endl;
          // DummyDatacard << "bin            MuPos_eta"<<eta_str<<"             MuPos_eta"<<eta_str<<"            MuPos_eta"<<eta_str<<"            MuPos_eta"<<eta_str<<"            MuPos_eta"<<eta_str<< endl;
          // DummyDatacard << "process        Z_MCDATALIKE_NonScaled       Z_EWKTT_NonScaled        W_EWK_NonScaled        W_TTJets_NonScaled        W_EWK_NonScaled_ALT" << endl;   
          // DummyDatacard << "process               2                           1                      -1                       3                       0     " << endl;
          // DummyDatacard << "rate                 -1                     "<< -TemplatesZ_NonScaled[EWKTT][ieta][jmass]->Integral()<<"              -1                     -1                     " << TemplatesW_NonScaled[EWK][ieta][WMass::WMassNSteps+1]->Integral() << endl;
          // cout << "rate                 -1                     "<< -TemplatesZ_NonScaled[EWKTT][ieta][jmass]->Integral()<<"              -1                     -1                     " << TemplatesW_NonScaled[EWK][ieta][WMass::WMassNSteps+1]->Integral() << endl;
          // DummyDatacard << "--------------------------------------------------------------------------------" << endl;
          // DummyDatacard << "lumi    lnN    1.044    1.044   1.044    1.044   1.044" << endl;
          // DummyDatacard.close();
        
        }
      }
      cout << endl;
    }
    cout << endl;
    outTXTfile << endl;
    outTXTfile << endl;
  }
  foutDATA->Write();
  foutDATA->Close();
  foutDATA->Delete();
  
  outTXTfile.close();

      
  // TFile *foutMC_DATALIKE = new TFile(Form("%s/DataCards/datacards_MC_DATALIKE.root",folder.Data()),"RECREATE");
  
  // for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
    // TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
    // TDirectory *channel_dir = foutMC_DATALIKE->mkdir(Form("MuPos_eta%s",eta_str.Data()));
    // foutMC_DATALIKE->cd(Form("MuPos_eta%s/",eta_str.Data()));
    // for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
      // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
      // TDirectory *mass_dir = channel_dir->mkdir(Form("%d",jWmass));
      // mass_dir->cd();
      // for(int isample=0; isample<Nsamples;isample++){
        // if (samples_str[isample] == "DATA") continue;
        
        // TString Z_histoname = Form("Z_%s",samples_str[isample].Data());
        // TString W_histoname = samples_str[isample].Contains("MCDATALIKE") ? "data_obs" : Form("W_%s",samples_str[isample].Data());
        // TH1D*Ztempl=new TH1D(Z_histoname,Z_histoname,TemplatesZ[isample][ieta][jmass]->GetXaxis()->FindBin(xmax)-TemplatesZ[isample][ieta][jmass]->GetXaxis()->FindBin(xmin),xmin,xmax);
        // TH1D*Wtempl=new TH1D(W_histoname,W_histoname,TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmax)-TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmin),xmin,xmax);
        // // TH1D*Ztempl=(TH1D*)TemplatesZ[isample][ieta][jmass]->Clone(Z_histoname);
        // // TH1D*Wtempl=(TH1D*)TemplatesW[isample][ieta][jmass]->Clone(W_histoname);

        // int ibin=1;      
        // for(int ibinhisto=TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){
          // Ztempl->SetBinContent(ibin,TemplatesZ[isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
          // Wtempl->SetBinContent(ibin,TemplatesW[isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
          // ibin++;
        // }
        // if(!samples_str[isample].Contains("WJetsSig")) Ztempl->Write();
        // if(!samples_str[isample].Contains("DYJetsSig")) Wtempl->Write();
        // Wtempl->Delete();
        // Ztempl->Delete();
      // }
    // }
  // }
  // foutMC_DATALIKE->Write();
  // foutMC_DATALIKE->Close();



  // TFile *foutMC_SIGNAL_ONLY = new TFile(Form("%s/DataCards/datacards_MC_SIGNAL_ONLY.root",folder.Data()),"RECREATE");
      
  // for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
    // TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
    // TDirectory *channel_dir = foutMC_SIGNAL_ONLY->mkdir(Form("MuPos_eta%s",eta_str.Data()));
    // foutMC_SIGNAL_ONLY->cd(Form("MuPos_eta%s/",eta_str.Data()));
    // for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
      // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
      // TDirectory *mass_dir = channel_dir->mkdir(Form("%d",jWmass));
      // mass_dir->cd();
      // for(int isample=0; isample<Nsamples;isample++){
        // if ( !( samples_str[isample].Contains("WJetsSig") || samples_str[isample].Contains("DYJetsSig") ) ) continue;
        
        // TString Z_histoname = Form("Z_%s",samples_str[isample].Data());
        // TString W_histoname = samples_str[isample].Contains("WJetsSig") ? "data_obs" : Form("W_%s",samples_str[isample].Data());
        // TH1D*Ztempl=new TH1D(Z_histoname,Z_histoname,TemplatesZ[isample][ieta][jmass]->GetXaxis()->FindBin(xmax)-TemplatesZ[isample][ieta][jmass]->GetXaxis()->FindBin(xmin),xmin,xmax);
        // TH1D*Wtempl=new TH1D(W_histoname,W_histoname,TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmax)-TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmin),xmin,xmax);
        // // TH1D*Ztempl=(TH1D*)TemplatesZ[isample][ieta][jmass]->Clone(Z_histoname);
        // // TH1D*Wtempl=(TH1D*)TemplatesW[isample][ieta][jmass]->Clone(W_histoname);

        // int ibin=1;      
        // for(int ibinhisto=TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[isample][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){
          // Ztempl->SetBinContent(ibin,TemplatesZ[isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
          // Wtempl->SetBinContent(ibin,TemplatesW[isample][ieta][jmass]->Integral(ibinhisto,ibinhisto));
          // ibin++;
        // }
        // if ( !samples_str[isample].Contains("WJetsSig") ) Ztempl->Write();
        // if ( !samples_str[isample].Contains("DYJetsSig") ) Wtempl->Write();
        // Wtempl->Delete();
        // Ztempl->Delete();        
      // }
    // }
  // }
  // foutMC_SIGNAL_ONLY->Write();
  // foutMC_SIGNAL_ONLY->Close();
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
    // {  
      // ofstream outTXTfile;
      // outTXTfile.open (Form("%s/DataCards/datacard_Wmass_MuPos_eta%s_%d.txt",folder.Data(),eta_str.Data(),jWmass));
      // outTXTfile << "\# W mass hypothesis " << jWmass << " MeV, measurement performed with positive muons \|eta\|\< " << WMass::etaMaxMuons[ieta] << endl;
      // outTXTfile << "imax " << (TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax) - TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin)) <<" number of channels" << endl;
      // outTXTfile << "jmax " << (Nsamples-3)*2 << " number of backgrounds" << endl;
      // outTXTfile << "kmax 1  number of nuisance parameters (sources of systematical uncertainties)" << endl;
      // outTXTfile << "------------" << endl;
      // outTXTfile << "\# we have just one channel, in which we observe " << TemplatesW[DATA][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) << " events, Z template contains " << TemplatesZ[DATA][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) << " events after reweighting" << endl;
      // outTXTfile << "bin\t\t\t\t\t  ";
      
      // // DATA (observation
      // int ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){
        // outTXTfile << ibin <<"   \t";
        // ibin++;
      // }
      // outTXTfile << endl;
      // outTXTfile << "observation\t";
      // ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){
        // outTXTfile << TemplatesW[DATA][ieta][jmass]->Integral(ibinhisto,ibinhisto) <<"\t";
        // ibin++;
      // }
      // outTXTfile << endl;

      // // TEMPLATE BINS
      // outTXTfile << "------------" << endl;
      // outTXTfile << "\# now we list the expected events for signal and all backgrounds in that bin" << endl;
      // outTXTfile << "\# the second 'process' line must have a positive number for backgrounds, and 0 for signal" << endl;
      // outTXTfile << "bin\t\t\t\t\t  ";
      // ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){
        // for(int isample=0; isample<(Nsamples-3)*2+1; isample++) outTXTfile << ibin <<"  \t\t";
        // ibin++;
      // }
      // outTXTfile << endl;

      // // TEMPLATE PROCESS NAMES
      // outTXTfile << "process \t\t  ";
      // ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){

        // outTXTfile << "Z_"<<samples_str[DATA] <<" ";
        // // outTXTfile << "Z_"<<samples_str[WJetsSig] <<" ";
        // outTXTfile << "Z_"<<samples_str[WJetsFake] <<" ";
        // outTXTfile << "Z_"<<samples_str[DYJetsFake] <<" ";
        // outTXTfile << "Z_"<<samples_str[TTJets] <<" ";
        // outTXTfile << "Z_"<<samples_str[WWJets] <<" ";
        // outTXTfile << "Z_"<<samples_str[WZJets] <<" ";
        // outTXTfile << "Z_"<<samples_str[ZZJets] <<" ";
        // outTXTfile << "Z_"<<samples_str[QCD] <<" ";

        // outTXTfile << "W_"<<samples_str[WJetsFake] <<" ";
        // // outTXTfile << "W_"<<samples_str[DYJetsSig] <<" ";
        // outTXTfile << "W_"<<samples_str[DYJetsFake] <<" ";
        // outTXTfile << "W_"<<samples_str[TTJets] <<" ";
        // outTXTfile << "W_"<<samples_str[WWJets] <<" ";
        // outTXTfile << "W_"<<samples_str[WZJets] <<" ";
        // outTXTfile << "W_"<<samples_str[ZZJets] <<" ";
        // outTXTfile << "W_"<<samples_str[QCD] <<" ";
        // ibin++;
      // }
      // outTXTfile << endl;

      // // TEMPLATE PROCESS NUMBER
      // outTXTfile << "process \t\t  ";
      // ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){
        // for(int isample=0; isample<(Nsamples-3)*2+1; isample++) outTXTfile << isample <<"  \t\t";
        // ibin++;
      // }
      // outTXTfile << endl;

      // // TEMPLATE PROCESS RATE
      // outTXTfile << "rate \t\t\t  ";
      // ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax); ibinhisto++){

        // // outTXTfile << TemplatesW[DATA][ieta][jmass]->GetBinContent(ibinhisto) <<"\t";
        
        // double norm_SF = (TemplatesW[DATA][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[WJetsFake][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[DYJetsSig][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[DYJetsFake][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[TTJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[WWJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[WZJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[ZZJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesW[QCD][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) ) / 
        //                 (TemplatesZ[DATA][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[WJetsSig][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[WJetsFake][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[DYJetsFake][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[TTJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[WWJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[WZJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[ZZJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) - TemplatesZ[QCD][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) );
        // //double norm_SF = TemplatesW[DATA][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) /
        //               (TemplatesW[WJetsFake][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[DYJetsSig][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[DYJetsFake][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[TTJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[WWJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[WZJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[ZZJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesW[QCD][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) +
        //             TemplatesZ[DATA][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[WJetsSig][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[WJetsFake][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[DYJetsFake][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[TTJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[WWJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[WZJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[ZZJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) double sf_WDATA = TemplatesZ[QCD][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)) );
        
        // double sf_W_DATA       = TemplatesW[DATA][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_WJetsFake  = TemplatesW[WJetsFake][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_DYJetsSig  = TemplatesW[DYJetsSig][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_DYJetsFake = TemplatesW[DYJetsFake][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax)); 
        // double sf_W_TTJets     = TemplatesW[TTJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_WWJets     = TemplatesW[WWJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_WZJets     = TemplatesW[WZJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_ZZJets     = TemplatesW[ZZJets][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_W_QCD        = TemplatesW[QCD][ieta][jmass]->Integral(TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        
        // double sf_Z_DATA = TemplatesZ[DATA][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_WJetsSig = TemplatesZ[WJetsSig][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_WJetsFake = TemplatesZ[WJetsFake][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_DYJetsFake = TemplatesZ[DYJetsFake][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_TTJets = TemplatesZ[TTJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_WWJets = TemplatesZ[WWJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_WZJets = TemplatesZ[WZJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_ZZJets = TemplatesZ[ZZJets][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));
        // double sf_Z_QCD = TemplatesZ[QCD][ieta][jmass]->Integral(TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin),TemplatesZ[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax));

        // double norm_SF = sf_W_DATA / 
        // ( (sf_Z_DATA - (sf_Z_WJetsSig + sf_Z_WJetsFake + sf_Z_DYJetsFake + sf_Z_TTJets + sf_Z_WWJets + sf_Z_WZJets + sf_Z_ZZJets + sf_Z_QCD)
        // + (sf_W_WJetsFake + sf_W_DYJetsSig + sf_W_DYJetsFake + sf_W_TTJets + sf_W_WWJets + sf_W_WZJets + sf_W_ZZJets + sf_W_QCD) ) );
        
        
        
        // // cout << "sf_W_DATA= " << sf_W_DATA << " sf_W_WJetsFake= " << sf_W_WJetsFake << " sf_W_DYJetsSig= " << sf_W_DYJetsSig << " sf_W_DYJetsFake= " << sf_W_DYJetsFake
        // // << " sf_W_TTJets= " << sf_W_TTJets << " sf_W_WWJets= " << sf_W_WWJets << " sf_W_WZJets= " << sf_W_WZJets << " sf_W_ZZJets= " << sf_W_ZZJets << " sf_W_QCD= " << sf_W_QCD 
        // // << endl;
        // // cout << "sf_Z_DATA= " << sf_Z_DATA << " sf_Z_WJetsSig= " << sf_Z_WJetsSig << " sf_Z_WJetsFake= " << sf_Z_WJetsFake << " sf_Z_DYJetsFake= " << sf_Z_DYJetsFake
        // // << " sf_Z_TTJets= " << sf_Z_TTJets << " sf_Z_WWJets= " << sf_Z_WWJets << " sf_Z_WZJets= " << sf_Z_WZJets << " sf_Z_ZZJets= " << sf_Z_ZZJets << " sf_Z_QCD= " << sf_Z_QCD 
        // // << endl;

        // // cout << "norm_SF= " << norm_SF << " norm_SF2= " << norm_SF2 << endl;
        
        // // outTXTfile << "Z_"<<samples_str[DATA] <<" ";
        // outTXTfile << TemplatesZ[DATA][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // // outTXTfile << "Z_"<<samples_str[WJetsSig] <<" ";
        // // outTXTfile << -TemplatesZ[WJetsSig][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[WJetsFake] <<" ";
        // outTXTfile << -TemplatesZ[WJetsFake][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[DYJetsFake] <<" ";
        // outTXTfile << -TemplatesZ[DYJetsFake][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[TTJets] <<" ";
        // outTXTfile << -TemplatesZ[TTJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[WWJets] <<" ";
        // outTXTfile << -TemplatesZ[WWJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[WZJets] <<" ";
        // outTXTfile << -TemplatesZ[WZJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[ZZJets] <<" ";
        // outTXTfile << -TemplatesZ[ZZJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "Z_"<<samples_str[QCD] <<" ";
        // // outTXTfile << -TemplatesZ[QCD][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // outTXTfile << -0.001 <<" ";

        // // outTXTfile << "W_"<<samples_str[WJetsFake] <<" ";
        // outTXTfile << TemplatesW[WJetsFake][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "W_"<<samples_str[DYJetsSig] <<" ";
        // // outTXTfile << TemplatesW[DYJetsSig][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // // outTXTfile << "W_"<<samples_str[DYJetsFake] <<" ";
        // outTXTfile << TemplatesW[DYJetsFake][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "W_"<<samples_str[TTJets] <<" ";
        // outTXTfile << TemplatesW[TTJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "W_"<<samples_str[WWJets] <<" ";
        // outTXTfile << TemplatesW[WWJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "W_"<<samples_str[WZJets] <<" ";
        // outTXTfile << TemplatesW[WZJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "W_"<<samples_str[ZZJets] <<" ";
        // outTXTfile << TemplatesW[ZZJets][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // // outTXTfile << "W_"<<samples_str[QCD] <<" ";
        // // outTXTfile << TemplatesW[QCD][ieta][jmass]->Integral(ibinhisto,ibinhisto)*norm_SF <<" ";
        // outTXTfile << 0.001 <<" ";

        // ibin++;
      // }
      // outTXTfile << endl;

      // outTXTfile << "------------"<< endl;
      // outTXTfile << "lumi    lnN    1.0001 \t";
      // ibin=0;
      // for(int ibinhisto=TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmin); ibinhisto<TemplatesW[DATA][ieta][jmass]->GetXaxis()->FindBin(xmax) - 1; ibinhisto++){
        // for(int i=0;i<(Nsamples-3)*2+2; i++) outTXTfile << " - \t";
        // ibin++;
      // }
      // outTXTfile << endl;
      
      // outTXTfile.close();
      
    // }
  // }

  cout << "program ended..." << endl;
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
