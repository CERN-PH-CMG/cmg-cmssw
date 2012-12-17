#include "../includes/common.h"
#include <iostream>
#include <fstream>

static const int Nsamples=13;
enum                                 {  WJetsSig ,  WJetsFake ,  DYJetsSig ,  DYJetsFake ,   TTJets ,   ZZJets ,   WWJets ,  WZJets ,  QCD     ,  DATA  ,  EWK  ,  EWKTT  ,  MCDATALIKE  };
TString     samples_str[Nsamples]  = { "WJetsSig", "WJetsFake", "DYJetsSig", "DYJetsFake",  "TTJets",  "ZZJets",  "WWJets", "WZJets", "QCD"    , "DATA" , "EWK" , "EWKTT" , "MCDATALIKE" };

void DataCardsParametrization(TString folder, TString sample){

  cout << "folder= " << folder << endl;
  
  // TOKENIZE SAMPLES
  TFile* finTemplatesZ[Nsamples], *finTemplatesW[Nsamples];
  TH1D *TemplatesZ_NonScaled_bins[Nsamples][WMass::etaMuonNSteps][70], *TemplatesZ_NonScaled[Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *TemplatesW_NonScaled_bins[Nsamples][WMass::etaMuonNSteps][70], *TemplatesW_NonScaled[Nsamples][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *TemplatesZ_NonScaled_2Dbins[Nsamples][WMass::etaMuonNSteps];
  
  TFile *finDATA = new TFile(Form("%s/DataCards/datacards_DATA.root",folder.Data()));
  TFile *foutDATA = new TFile(Form("%s/DataCards/datacards_DATA_parametrized.root",folder.Data()),"RECREATE");
  
  for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
    for(int isample=0; isample<Nsamples;isample++){
    
      TemplatesZ_NonScaled_2Dbins[isample][ieta]=new TH2D(Form("TemplatesZ_NonScaled_2Dbins_%s_%s",samples_str[isample].Data(),eta_str.Data()),Form("TemplatesZ_NonScaled_2Dbins_%s_%s",samples_str[isample].Data(),eta_str.Data()),
                                                          2*WMass::WMassNSteps+1,WMass::WMassCentral_MeV-(WMass::WMassNSteps)*WMass::WMassStep_MeV,WMass::WMassCentral_MeV+(WMass::WMassNSteps+1)*WMass::WMassStep_MeV,
                                                          70,1,71);
      
      for(int ibinhisto=11; ibinhisto<70; ibinhisto++){

        TemplatesZ_NonScaled_bins[isample][ieta][ibinhisto]=new TH1D(Form("TemplatesZ_NonScaled_bins_%s_%s_%d",samples_str[isample].Data(),eta_str.Data(),ibinhisto),Form("TemplatesZ_NonScaled_bins_%s_%s_%d",samples_str[isample].Data(),eta_str.Data(),ibinhisto),2*WMass::WMassNSteps+1,WMass::WMassCentral_MeV-(WMass::WMassNSteps)*WMass::WMassStep_MeV,WMass::WMassCentral_MeV+(WMass::WMassNSteps+1)*WMass::WMassStep_MeV);
        // TemplatesW_NonScaled_bins[isample][ieta][ibinhisto]=new TH1D(Form("TemplatesW_NonScaled_bins_%s_%s_%d",samples_str[isample].Data(),eta_str.Data(),ibinhisto),Form("TemplatesW_NonScaled_bins_%s_%s_%d",samples_str[isample].Data(),eta_str.Data(),ibinhisto),2*WMass::WMassNSteps+1,WMass::WMassCentral_MeV-(WMass::WMassNSteps)*WMass::WMassStep_MeV,WMass::WMassCentral_MeV+(WMass::WMassNSteps+1)*WMass::WMassStep_MeV);
    
      }
    }
  }
  for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
    
    for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
      
      cout << "eta cut " << WMass::etaMaxMuons[ieta] << " jWmass= " << jWmass << endl;
      finDATA->cd(Form("MuPos_eta%s/%d",eta_str.Data(),jWmass));

      TString Z_histoname_NonScaled[2*Nsamples], W_histoname_NonScaled[2*Nsamples];
      
      for(int isample=0; isample<Nsamples;isample++){
      
        if(!samples_str[isample].Contains("WJetsSig")){
          Z_histoname_NonScaled[isample] = Form("Z_%s_NonScaled",samples_str[isample].Data());
          
          TemplatesZ_NonScaled[isample][ieta][jmass]=(TH1D*) finDATA->Get(Form("MuPos_eta%s/%d/%s",eta_str.Data(),jWmass,Z_histoname_NonScaled[isample].Data()));
          // cout << Form("MuPos_eta%s/%d/%s",eta_str.Data(),jWmass,Z_histoname_NonScaled[isample].Data()) << " ";
          // TemplatesZ_NonScaled[isample][ieta][jmass]->Print();
        }
        if(!samples_str[isample].Contains("DYJetsSig")){
          W_histoname_NonScaled[isample] = samples_str[isample] == "DATA" ? "data_obs_NonScaled" : Form("W_%s_NonScaled",samples_str[isample].Data());
          // cout << Form("MuPos_eta%s/%d/%s",eta_str.Data(),jWmass,W_histoname_NonScaled[isample].Data()) << " ";
          TemplatesW_NonScaled[isample][ieta][jmass]=(TH1D*) finDATA->Get(Form("MuPos_eta%s/%d/%s",eta_str.Data(),jWmass,W_histoname_NonScaled[isample].Data()));
          // TemplatesW_NonScaled[isample][ieta][jmass]->Print();
        }
        
        
        
      }
    }
  }

  for(int ibinhisto=11; ibinhisto<70; ibinhisto++){
    for(int isample=0; isample<Nsamples;isample++){
      for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
        TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
        for(int jmass=0; jmass<2*WMass::WMassNSteps+1; jmass++){
          int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-jmass)*WMass::WMassStep_MeV;
          
          if(!samples_str[isample].Contains("WJetsSig")){
            // cout << isample << " " << ieta << " " << ibinhisto << " " << jWmass << " " <<  TemplatesZ_NonScaled[isample][ieta][jmass]->GetBinContent(ibinhisto) << endl;
            // cout << TemplatesZ_NonScaled_bins[isample][ieta][ibinhisto]->GetName()<<"->Fill("<<jWmass<<","<<Form("%f",TemplatesZ_NonScaled[isample][ieta][jmass]->GetBinContent(ibinhisto))<<")"<<endl;
            TemplatesZ_NonScaled_bins[isample][ieta][ibinhisto]->Fill(jWmass,TemplatesZ_NonScaled[isample][ieta][jmass]->GetBinContent(ibinhisto));
            // TemplatesZ_NonScaled_2Dbins[isample][ieta]->Print();
            TemplatesZ_NonScaled_2Dbins[isample][ieta]->SetBinContent(TemplatesZ_NonScaled_2Dbins[isample][ieta]->GetXaxis()->FindBin(jWmass),ibinhisto,TemplatesZ_NonScaled[isample][ieta][jmass]->GetBinContent(ibinhisto));
          }
          
          // if(!samples_str[isample].Contains("DYJetsSig")){
            // cout << TemplatesW_NonScaled_bins[isample][ieta][ibinhisto]->GetName()<<"->Fill("<<jWmass<<","<<TemplatesW_NonScaled[isample][ieta][jmass]->GetBinContent(ibinhisto)<<")"<<endl;
            // TemplatesW_NonScaled_bins[isample][ieta][ibinhisto]->Fill(jWmass,TemplatesW_NonScaled[isample][ieta][jmass]->GetBinContent(ibinhisto));
          // }
          
        }
      }
    }
  }

  
  foutDATA->cd();
  // for(int ieta=0; ieta<WMass::etaMuonNSteps; ieta++){
  for(int ieta=0; ieta<1; ieta++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[ieta]); eta_str.ReplaceAll(".","p");
    
    for(int isample=0; isample<Nsamples;isample++){
      if(!samples_str[isample].Contains("WJetsSig")){
        TemplatesZ_NonScaled_2Dbins[isample][ieta]->Print();
        TemplatesZ_NonScaled_2Dbins[isample][ieta]->Write();
        TemplatesZ_NonScaled_2Dbins[isample][ieta]->Delete();
      }
    }
    
    for(int ibinhisto=11; ibinhisto<70; ibinhisto++){
    // for(int ibinhisto=19; ibinhisto<20; ibinhisto++){
    
    TDirectory *sub_dir = foutDATA->mkdir(Form("MuPos_eta%s_bin%d",eta_str.Data(),ibinhisto));
    foutDATA->cd(Form("MuPos_eta%s_bin%d",eta_str.Data(),ibinhisto));
    
      for(int isample=0; isample<Nsamples;isample++){
      // for(int isample=4; isample<5;isample++){

        if(!samples_str[isample].Contains("WJetsSig")){
          TemplatesZ_NonScaled_bins[isample][ieta][ibinhisto]->Write();
          TemplatesZ_NonScaled_bins[isample][ieta][ibinhisto]->Delete();
        }
        // if(!samples_str[isample].Contains("DYJetsSig")){
          // TemplatesW_NonScaled_bins[isample][ieta][ibinhisto]->Write();
          // TemplatesW_NonScaled_bins[isample][ieta][ibinhisto]->Delete();
        // }
    
      }
    }
  }

  // foutDATA->Write();
  foutDATA->Close();

  cout << "program ended..." << endl;
}
