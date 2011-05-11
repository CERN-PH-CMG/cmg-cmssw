#include "TMath.h"


void ReadTree()
{
  // style definition -------------------------------

  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  // file definition ----------------------------------
  // TFile *input_data = TFile::Open("dcap://cmsgridftp.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/lpcjj/DijetMass/2010Nov23rd_35p777pbm1_newcorrection/ProcessedTree_Jet_combined.root", "READ");
  //  TFile *input_data = TFile::Open("/uscmst1b_scratch/lpc1/3DayLifetime/chiyoung/Nov23rd_newcorrection/ProcessedTree_Jet_combined.root", "READ");
  TFile *inf_data  = TFile::Open("/uscmst1b_scratch/lpc1/3DayLifetime/chiyoung/2011Apr24/2010/ProcessedTree_Combined.root");
  TFile *outf = new TFile("histogrmas_33p684pbm1.root","RECREATE");
  TFile *inf_mc = TFile::Open("dcap://cmsgridftp.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/resilient/chiyoung/Spring10_QCDDijet/ProcessedTree_ak7calo_QCDDijet.root", "READ");

  TTree *tr_data = (TTree*)inf_data->Get("ak7/ProcessedTree");
  TTree *tr_mc = (TTree*)inf_mc->Get("tr");

  // p_T and mass boundaries ---------------------------
  
  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 
  double ptBoundaries[55] = {1, 9, 18, 29, 40, 53, 67, 81, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3500};
  // Canvas definition ----------------------------------

  TCanvas *c_DijetMass = new TCanvas("c_DijetMass","c_DijetMass",459,358,600,602);
  TCanvas *c_DijetMass_log = new TCanvas("c_DijetMass_log","c_DijetMass_log",459,358,600,602);
  TCanvas *c_DijetMass_ratio = new TCanvas("c_DijetMass_ratio","c_DijetMass_ratio",459,358,600,602);
  TCanvas *c_DijetMass_ratio_log = new TCanvas("c_DijetMass_ratio_log","c_DijetMass_ratio_log",459,358,600,602);
  TCanvas *c_DijetMass_MI = new TCanvas("c_DijetMass_MI","c_DijetMass_MI",459,358,600,602);
  TCanvas *c_DijetMass_MI_log = new TCanvas("c_DijetMass_MI_log","c_DijetMass_MI_log",459,358,600,602);
  TCanvas *c_DijetMass_normalized_MI = new TCanvas("c_DijetMass_normalized_MI","c_DijetMass_normalized_MI",459,358,600,602);
  TCanvas *c_DijetMass_normalized_MI_log = new TCanvas("c_DijetMass_normalized_MI_log","c_DijetMass_normalized_MI_log",459,358,600,602);
  TCanvas *c_Eta_Phi_Scatter = new TCanvas("c_Eta_Phi_Scatter","c_Eta_Phi_Scatter",459,378,600,602);
  TCanvas *c_Eta_Phi_Scatter_1 = new TCanvas("c_Eta_Phi_Scatter_1","c_Eta_Phi_Scatter_1",459,378,600,602);
  TCanvas *c_Eta_Phi_Scatter_2 = new TCanvas("c_Eta_Phi_Scatter_2","c_Eta_Phi_Scatter_2",459,378,600,602);
  TCanvas *c_MET_over_sumEt = new TCanvas("c_MET_over_sumEt","c_MET_over_sumEt",459,378,600,602);
  TCanvas *c_MET_over_sumEt_log = new TCanvas("c_MET_over_sumEt_log","c_MET_over_sumEt_log",459,378,600,602);
  TCanvas *c_EMF = new TCanvas("c_EMF","c_EMF",459,378,600,602);
  TCanvas *c_DPhi = new TCanvas("c_DPhi","c_DPhi",459,378,600,602);
  TCanvas *c_DPhi_log = new TCanvas("c_DPhi_log","c_DPhi_log",459,378,600,602);
  TCanvas *c_Eta = new TCanvas("c_Eta","c_Eta",459,378,600,602);
  TCanvas *c_Eta_1 = new TCanvas("c_Eta_1","c_Eta_1",459,378,600,602);
  TCanvas *c_Eta_2 = new TCanvas("c_Eta_2","c_Eta_2",459,378,600,602);
  TCanvas *c_DEta = new TCanvas("c_DEta","c_DEta",459,378,600,602);
  TCanvas *c_Phi = new TCanvas("c_Phi","c_Phi",459,378,600,602);
  TCanvas *c_Phi_1 = new TCanvas("c_Phi_1","c_Phi_1",459,378,600,602);
  TCanvas *c_Phi_2 = new TCanvas("c_Phi_2","c_Phi_2",459,378,600,602);
  TCanvas *c_n90hits = new TCanvas("c_n90hits","c_n90hits",459,378,600,602);
  TCanvas *c_n90hits_log = new TCanvas("c_n90hits_log","c_n90hits_log",459,378,600,602);
  TCanvas *c_fHPD = new TCanvas("c_fHPD","c_fHPD",459,378,600,602);
  TCanvas *c_CorPt = new TCanvas("c_corPt","c_corPt",459,378,600,602);
  TCanvas *c_CorPt_1 = new TCanvas("c_corPt_1","c_corPt_1",459,378,600,602);
  TCanvas *c_CorPt_2 = new TCanvas("c_corPt_2","c_corPt_2",459,378,600,602);
  TCanvas *c_CorPt_log = new TCanvas("c_corPt_log","c_corPt_log",459,378,600,602);
  TCanvas *c_CorPt_log_1 = new TCanvas("c_corPt_log_1","c_corPt_log_1",459,378,600,602);
  TCanvas *c_CorPt_log_2 = new TCanvas("c_corPt_log_2","c_corPt_log_2",459,378,600,602);
  TCanvas *c_nTrkVtx = new TCanvas("c_nTrkVtx","c_nTrkVtx",459,378,600,602);
  TCanvas *c_nTrkCalo = new TCanvas("c_nTrkCalo","c_nTrkCalo",459,378,600,602);

  // Canvas Setting

  c_DijetMass_ratio->SetGridx();
  c_DijetMass_ratio->SetGridy();
  c_DijetMass_ratio_log->SetGridx();
  c_DijetMass_ratio_log->SetGridy();
  c_Eta_Phi_Scatter->SetRightMargin(0.1157718);

  // histogrmas definition ------------------------------

  TH1 *h_DijetMass_data = new TH1F("h_DijetMass_data","DijetMass_data",83, xAxis1);
  TH1 *h_DijetMass_mc = new TH1F("h_DijetMass_mc","DijetMass_mc",83, xAxis1);
  TH1 *h_DijetMass_ratio = new TH1F("h_DijetMass_ratio","DijetMass_ratio",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVe1_data = new TH1F("h_DijetMass_MI_nPVe1_data","DijetMass_MI_nPVe1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg1_data = new TH1F("h_DijetMass_MI_nPVg1_data","DijetMass_MI_nPVg1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVe1_mc = new TH1F("h_DijetMass_MI_nPVe1_mc","DijetMass_MI_nPVe1_mc",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg1_mc = new TH1F("h_DijetMass_MI_nPVg1_mc","DijetMass_MI_nPVg1_mc",83, xAxis1);
  TH2 *h_Eta_Phi_Scatter_data = new TH2F("h_Eta_Phi_Scatter","Eta_Phi_Scatter",100,-3,3,100,-3.2,3.2);
  TH2 *h_Eta_Phi_Scatter_data_1 = new TH2F("h_Eta_Phi_Scatter_1","Eta_Phi_Scatter_1",100,-3,3,100,-3.2,3.2);
  TH2 *h_Eta_Phi_Scatter_data_2 = new TH2F("h_Eta_Phi_Scatter_2","Eta_Phi_Scatter_2",100,-3,3,100,-3.2,3.2);
  TH1 *h_MET_over_sumEt_data = new TH1F("h_MET_over_sumEt_data","MET_over_sumEt_data",25,0.0,1.0001);
  TH1 *h_MET_over_sumEt_mc = new TH1F("h_MET_over_sumEt_mc","MET_over_sumEt_mc",25,0.0,1.0001);
  TH1 *h_EMF_data = new TH1F("h_EMF_data","EMF_data",25,0.0,1.0001);
  TH1 *h_EMF_mc = new TH1F("h_EMF_mc","EMF_mc",25,0.0,1.0001);
  TH1 *h_DPhi_data = new TH1F("h_DPhi_data","DPhi_data",25,0.0,3.142);
  TH1 *h_DPhi_mc = new TH1F("h_DPhi_mc","DPhi_mc",25,0.0,3.142);
  TH1 *h_Eta_data = new TH1F("h_Eta_data","Eta_data",60,-3.0,3.0);
  TH1 *h_Eta_data_1 = new TH1F("h_Eta_data_1","Eta_data_1",60,-3.0,3.0);
  TH1 *h_Eta_data_2 = new TH1F("h_Eta_data_2","Eta_data_2",60,-3.0,3.0);
  TH1 *h_Eta_mc = new TH1F("h_Eta_mc","Eta_mc",60,-3.0,3.0);
  TH1 *h_Eta_mc_1 = new TH1F("h_Eta_mc_1","Eta_mc_1",60,-3.0,3.0);
  TH1 *h_Eta_mc_2 = new TH1F("h_Eta_mc_2","Eta_mc_2",60,-3.0,3.0);
  TH1 *h_DEta_data = new TH1F("h_DEta_data","DEta_data",15,0.,1.5);
  TH1 *h_DEta_mc = new TH1F("h_DEta_mc","DEta_mc",15,0.,1.5);
  TH1 *h_Phi_data = new TH1F("h_Phi_data","Phi_data",18,-3.142,3.142);
  TH1 *h_Phi_data_1 = new TH1F("h_Phi_data_1","Phi_data_1",18,-3.142,3.142);
  TH1 *h_Phi_data_2 = new TH1F("h_Phi_data_2","Phi_data_2",18,-3.142,3.142);
  TH1 *h_Phi_mc = new TH1F("h_Phi_mc","Phi_mc",18,-3.142,3.142);
  TH1 *h_Phi_mc_1 = new TH1F("h_Phi_mc_1","Phi_mc_1",18,-3.142,3.142);
  TH1 *h_Phi_mc_2 = new TH1F("h_Phi_mc_2","Phi_mc_2",18,-3.142,3.142);
  TH1 *h_n90hits_data = new TH1F("h_n90hits_data","n90hits_data",400,-0.5,399.5);
  TH1 *h_n90hits_mc = new TH1F("n90hits_mc","n90hits_mc",400,-0.5,399.5);
  TH1 *h_fHPD_data = new TH1F("h_fHPD_data","fHPD_data",25,0.0,1.0001);
  TH1 *h_fHPD_mc = new TH1F("h_fHPD_mc","fHPD_mc",25,0.0,1.0001);
  TH1 *h_corPt_data = new TH1F("h_corPt_data","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_1 = new TH1F("h_corPt_data_1","corPt_data_1",54,ptBoundaries);
  TH1 *h_corPt_data_2 = new TH1F("h_corPt_data_2","corPt_data_2",54,ptBoundaries);
  TH1 *h_corPt_mc = new TH1F("h_corPt_mc","corPt_mc",54,ptBoundaries);
  TH1 *h_corPt_mc_1 = new TH1F("h_corPt_mc_1","corPt_mc_1",54,ptBoundaries);
  TH1 *h_corPt_mc_2 = new TH1F("h_corPt_mc_2","corPt_mc_2",54,ptBoundaries);
  TH1 *h_nTrkVtx_data = new TH1F("h_nTrkVtx_data","nTrkVtx_data",900,-0.5,899.5);
  TH1 *h_nTrkVtx_mc = new TH1F("nTrkVtx_mc","nTrkVtx_mc",900,-0.5,899.5);
  TH1 *h_nTrkCalo_data = new TH1F("h_nTrkCalo_data","nTrkCalo_data",100,-0.5,99.5);
  TH1 *h_nTrkCalo_mc = new TH1F("nTrkCalo_mc","nTrkCalo_mc",100,-0.5,99.5);
  //  TH1 *h_JCFVtx_data = new TH1F("h_JCFVtx_data","JCFVtx_data",25,-1.5,1.5);
  //  TH1 *h_JCFVtx_mc = new TH1F("h_JCFVtx_mc","JCFVtx_mc",25,-1.5,1.5);
  //  TH1 *h_JCFCalo_data = new TH1F("h_JCFCalo_data","JCFCalo_data",4000,-0.5,999.5);
  //  TH1 *h_JCFCalo_mc = new TH1F("h_JCFCalo_mc","JCFCalo_mc",4000,-0.5,999.5);

  //  TH1F *hPFCorMjj = new TH1F("PFCorMjj","PFCorMjj",500,0,5000);

  // TLegend definition -----------------------------------------------------

  TLegend *l_DijetMass = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_MI = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_MI_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_normalized_MI = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_DijetMass_normalized_MI_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_MET_over_sumEt = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_MET_over_sumEt_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_EMF = new TLegend(0.3456376,0.1602787,0.8573826,0.3292683,NULL,"brNDC");
  TLegend *l_DPhi = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_DPhi_log = new TLegend(0.20,0.70,0.70,0.90);
  TLegend *l_Eta = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_1 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_Eta_2 = new TLegend(0.2902685,0.1707317,0.8053691,0.325784,NULL,"brNDC");
  TLegend *l_DEta = new TLegend(0.2567114,0.2195122,0.7651007,0.4198606,NULL,"brNDC");
  TLegend *l_Phi = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_1 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_Phi_2 = new TLegend(0.4295302,0.184669,0.9379195,0.3850174,NULL,"brNDC");
  TLegend *l_n90hits = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_n90hits_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_fHPD = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_log = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_log_1 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_corPt_log_2 = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_nTrkVtx = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_nTrkCalo = new TLegend(0.43,0.70,0.93,0.90);
  TLegend *l_JCFCalo = new TLegend(0.43,0.70,0.93,0.90);

  // TPave definition -------------------------------------

  TPaveText *pt_DijetMass_ratio = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_DijetMass_ratio_log = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Eta_Phi_Scatter = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Eta_Phi_Scatter_1 = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Eta_Phi_Scatter_2 = new TPaveText(0.003355705,0.003484321,0.5033557,0.06271777,"blNDC");
  TPaveText *pt_Phi = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_1 = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");
  TPaveText *pt_Phi_2 = new TPaveText(0.1744966,0.695122,0.6744966,0.9198606,"blNDC");

  // data access -------------------------------------------

  QCDEvent *Event_data = new QCDEvent();
  TBranch *branch = tr_data->GetBranch("event");
  branch->SetAddress(&Event_data);
  
  // MC access ---------------------------------------------

  EVENT_VAR_mc eventVar_mc;
  CALOJET_VAR_mc jetVar_mc[2];

  TBranch *b_eventVar_mc = (TBranch*)tr_mc->GetBranch("eventVar");
  TBranch *b_jet1_mc     = (TBranch*)tr_mc->GetBranch("jet1Var");
  TBranch *b_jet2_mc     = (TBranch*)tr_mc->GetBranch("jet2Var");

  b_eventVar_mc->SetAddress(&eventVar_mc);
  b_jet1_mc    ->SetAddress(&jetVar_mc[0]);
  b_jet2_mc    ->SetAddress(&jetVar_mc[1]);

  // # of entries -------------------------------------------

  unsigned NEntries_data = tr_data->GetEntries();
  cout<<"Reading TREE: "<<NEntries_data <<" events"<<endl;
  int decade = 0;
  
  unsigned NEntries_mc = tr_mc->GetEntries();

  // counters------------------------------------------------

  unsigned data_count(0);
  unsigned vertex1_count(0);
  unsigned vertex2_count(0);
  unsigned mass_count(0);
  unsigned eta_count(0);
  unsigned jetID_count(0);
  unsigned runNo_count(0);
  unsigned mc_count(0);

  // fill histograms-----------------------------------------

  for(unsigned i=0;i<NEntries_data;i++) {
    double progress = 10.0*i/(1.0*NEntries_data);
    int k = TMath::FloorNint(progress); 

    if (k > decade) 
      cout<<10*k<<" %"<<endl;

    decade = k;          

    tr_data->GetEntry(i);

    int nVtx = (Event_data->evtHdr()).nVtx();
    bool eta_cuts = fabs((Event_data->calojet(0)).eta()) < 2.5 && fabs((Event_data->calojet(1)).eta()) < 2.5;
    bool deta_cut = fabs( (Event_data->calojet(0)).eta() - (Event_data->calojet(1)).eta() ) < 1.3;
    bool vertex_cut1 = (Event_data->evtHdr()).PVndof() >= 4;
    bool vertex_cut2 = (Event_data->evtHdr()).PVz() < 24 ;
    bool corMass_cuts = Event_data->calomjjcor(0) > 649.0;
    bool jetID_cuts = ((((Event_data->calojet(0)).emf() > 0.01) || (fabs((Event_data->calojet(0)).eta())) > 2.6) && ((Event_data->calojet(0)).n90hits() > 1) && ((Event_data->calojet(0)).fHPD() < 0.98)  ) && ((((Event_data->calojet(1)).emf() > 0.01) || (fabs((Event_data->calojet(1)).eta())) > 2.6) && ((Event_data->calojet(1)).n90hits() > 1) && ((Event_data->calojet(1)).fHPD() < 0.98 ) );
    bool runNo_cuts = true;


    data_count++; 
    if (runNo_cuts) {
      runNo_count++;
      if(vertex_cut1) {
        vertex1_count++;
        if(vertex_cut2) {
          vertex2_count++;
          if(eta_cuts) {
            if (deta_cut) {
              eta_count++;
              if(corMass_cuts) {
                mass_count++;
                if(jetID_cuts) {
                  jetID_count++;
                  h_DijetMass_data->Fill(Event_data->calomjjcor(0));
                  if (nVtx==1) h_DijetMass_MI_nPVe1_data->Fill(Event_data->calomjjcor(0));
                  else h_DijetMass_MI_nPVg1_data->Fill(Event_data->calomjjcor(0));
                  h_Eta_Phi_Scatter_data->Fill((Event_data->calojet(0)).eta(),(Event_data->calojet(0)).phi());
                  h_Eta_Phi_Scatter_data->Fill((Event_data->calojet(1)).eta(),(Event_data->calojet(1)).phi());
                  h_Eta_Phi_Scatter_data_1->Fill((Event_data->calojet(0)).eta(),(Event_data->calojet(0)).phi());
                  h_Eta_Phi_Scatter_data_2->Fill((Event_data->calojet(1)).eta(),(Event_data->calojet(1)).phi());
                  h_MET_over_sumEt_data->Fill((Event_data->calomet()).met_o_sumet());
                  h_EMF_data->Fill((Event_data->calojet(0)).emf());
                  h_EMF_data->Fill((Event_data->calojet(1)).emf());
                  h_DPhi_data->Fill(3.14159265358979323846-fabs(fabs((Event_data->calojet(0)).phi()-(Event_data->calojet(1)).phi())-3.14159265358979323846));
                  h_Eta_data->Fill((Event_data->calojet(0)).eta());
                  h_Eta_data->Fill((Event_data->calojet(1)).eta());
                  h_Eta_data_1->Fill((Event_data->calojet(0)).eta());
                  h_Eta_data_2->Fill((Event_data->calojet(1)).eta());
                  h_DEta_data->Fill(fabs((Event_data->calojet(0)).eta()-(Event_data->calojet(1)).eta()));
                  h_Phi_data->Fill((Event_data->calojet(0)).phi());
                  h_Phi_data->Fill((Event_data->calojet(1)).phi());
                  h_Phi_data_1->Fill((Event_data->calojet(0)).phi());
                  h_Phi_data_2->Fill((Event_data->calojet(1)).phi());
                  h_n90hits_data->Fill((Event_data->calojet(0)).n90hits());
                  h_n90hits_data->Fill((Event_data->calojet(1)).n90hits());
                  h_fHPD_data->Fill((Event_data->calojet(0)).fHPD());
                  h_fHPD_data->Fill((Event_data->calojet(1)).fHPD());
                  h_corPt_data->Fill((Event_data->calojet(0)).ptCor());
                  h_corPt_data->Fill((Event_data->calojet(1)).ptCor());
                  h_corPt_data_1->Fill((Event_data->calojet(0)).ptCor());
                  h_corPt_data_2->Fill((Event_data->calojet(1)).ptCor());
                  h_nTrkVtx_data->Fill((Event_data->calojet(0)).nTrkVtx());
                  h_nTrkVtx_data->Fill((Event_data->calojet(1)).nTrkVtx());
                  h_nTrkCalo_data->Fill((Event_data->calojet(0)).nTrkCalo());
                  h_nTrkCalo_data->Fill((Event_data->calojet(1)).nTrkCalo());  
                          
                  //            h_JCFCalo_data->Fill(jetVar_data[0].chf);
                  //            h_JCFCalo_data->Fill(jetVar_data[1].chf);
		  
                }
              }
            }
          }
        }
      }
    }




    /*
    cout<<"Event #"<<i<<":"<<endl;
    cout<<"PFMass = "<<Event_data->pfmjjcor(0)<<", JEC up:  "<<Event_data->pfmjjcor(1)<<", JEC down: "<<Event_data->pfmjjcor(-1)<<endl;
    cout<<"CALOMass = "<<Event_data->calomjjcor(0)<<", JEC up:  "<<Event_data->calomjjcor(1)<<", JEC down: "<<Event_data->calomjjcor(-1)<<endl;
    cout<<"PFJets: "<<Event_data->nPFJets()<<endl;
    for(unsigned j=0;j<Event_data->nPFJets();j++) {
      cout<<j<<" pt = "<<(Event_data->pfjet(j)).ptCor()<<", cor = "<<(Event_data->pfjet(j)).cor()<<", unc = "<<(Event_data->pfjet(j)).unc()<<endl;
    }
    cout<<"CaloJets: "<<Event_data->nCaloJets()<<endl;
    for(unsigned j=0;j<Event_data->nCaloJets();j++) {
      cout<<j<<" pt = "<<(Event_data->calojet(j)).ptCor()<<", cor = "<<(Event_data->calojet(j)).cor()<<", unc = "<<(Event_data->calojet(j)).unc()<<endl;
    }

    cout<<"HLT Objects: "<<Event_data->nHLTObj()<<endl;
    for(unsigned j=0;j<Event_data->nHLTObj();j++) {
      cout<<j<<" pt = "<<(Event_data->hltobj(j)).pt()<<endl;
    }
    cout<<"L1 Objects: "<<Event_data->nL1Obj()<<endl;
    for(unsigned j=0;j<Event_data->nL1Obj();j++) {
      cout<<j<<" pt = "<<(Event_data->l1obj(j)).pt()<<endl;
    }

    */
    //    hPFCorMjj->Fill(Event_data->pfmjjcor(0));
  }

 
  std::cout << "# of events = " << data_count << std::endl;
  std::cout << "# of events after runNo cut = " << runNo_count << std::endl;
  std::cout << "# of events after vertex1 cut = " << vertex1_count << std::endl;
  std::cout << "# of events after vertex2 cut = " << vertex2_count << std::endl;
  std::cout << "# of events after eta cut = " << eta_count << std::endl;
  std::cout << "# of events after mass cut = " << mass_count << std::endl;
  std::cout << "# of events after all cut = " << jetID_count << std::endl;
  
  // copy data dijet mass distribution histogram
  
  TH1F *h_DijetMass_normalized_MI_nPVe1_data    =(TH1F*)h_DijetMass_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_normalized_MI_nPVg1_data    =(TH1F*)h_DijetMass_MI_nPVg1_data->Clone();

  // fill MC histograms.

  for(unsigned i=0;i<NEntries_mc;i++) {
    tr_mc->GetEntry(i);

    float wt = eventVar_mc.weight;
    int nPV = eventVar_mc.nPV;
    bool vertex_cut1 = eventVar_mc.PVndof >= 4;
    bool vertex_cut2 = fabs(eventVar_mc.PVz) < 24;
    bool cuts = (eventVar_mc.njets > 1 && fabs(eventVar_mc.PVz) < 24.0 && eventVar_mc.PVndof >= 4 &&  fabs(jetVar_mc[0].eta)<2.5 && fabs(jetVar_mc[1].eta)<2.5);
    bool deta_cut = (fabs(jetVar_mc[0].eta-jetVar_mc[1].eta))<1.3;
//    bool deta_cut = true;
    bool corMass_cuts = eventVar_mc.corMass > 649.0;
    bool jetID_cuts = jetVar_mc[0].passLooseID && jetVar_mc[1].passLooseID;
    // bool jetID_cuts =  jetVar_mc[0].n90hits>1 && jetVar_mc[1].n90hits>1 && jetVar_mc[0].fHPD<0.98 && jetVar_mc[1].fHPD<0.98 && jetVar_mc[0].emf > 0.01 && jetVar_mc[1].emf > 0.01;
    if (vertex_cut1) {
      if(vertex_cut2) {
        if(cuts) {
          if (deta_cut) {
            if(corMass_cuts) {
              if(jetID_cuts) {
                h_DijetMass_mc->Fill(eventVar_mc.corMass,eventVar_mc.weight);
                if (nPV==1) h_DijetMass_MI_nPVe1_mc->Fill(eventVar_mc.corMass,eventVar_mc.weight);
                else h_DijetMass_MI_nPVg1_mc->Fill(eventVar_mc.corMass,eventVar_mc.weight);
                h_MET_over_sumEt_mc->Fill(eventVar_mc.met_over_sumet,eventVar_mc.weight);
                h_EMF_mc->Fill(jetVar_mc[0].emf,eventVar_mc.weight);
                h_EMF_mc->Fill(jetVar_mc[1].emf,eventVar_mc.weight);
                h_DPhi_mc->Fill(3.14159265358979323846-fabs(fabs(jetVar_mc[0].phi-jetVar_mc[1].phi)-3.14159265358979323846),eventVar_mc.weight);
                h_Eta_mc->Fill(jetVar_mc[0].eta,eventVar_mc.weight);
                h_Eta_mc->Fill(jetVar_mc[1].eta,eventVar_mc.weight);
                h_Eta_mc_1->Fill(jetVar_mc[0].eta,eventVar_mc.weight);
                h_Eta_mc_2->Fill(jetVar_mc[1].eta,eventVar_mc.weight);
                h_DEta_mc->Fill(fabs(jetVar_mc[0].eta-jetVar_mc[1].eta),eventVar_mc.weight);
                h_Phi_mc->Fill(jetVar_mc[0].phi,eventVar_mc.weight);
                h_Phi_mc->Fill(jetVar_mc[1].phi,eventVar_mc.weight);
                h_Phi_mc_1->Fill(jetVar_mc[0].phi,eventVar_mc.weight);
                h_Phi_mc_2->Fill(jetVar_mc[1].phi,eventVar_mc.weight);
                h_n90hits_mc->Fill(jetVar_mc[0].n90hits,eventVar_mc.weight);
                h_n90hits_mc->Fill(jetVar_mc[1].n90hits,eventVar_mc.weight);
                h_fHPD_mc->Fill(jetVar_mc[0].fHPD,eventVar_mc.weight);
                h_fHPD_mc->Fill(jetVar_mc[1].fHPD,eventVar_mc.weight);
                h_corPt_mc->Fill(jetVar_mc[0].corPt,eventVar_mc.weight);
                h_corPt_mc->Fill(jetVar_mc[1].corPt,eventVar_mc.weight);
                h_corPt_mc_1->Fill(jetVar_mc[0].corPt,eventVar_mc.weight);
                h_corPt_mc_2->Fill(jetVar_mc[1].corPt,eventVar_mc.weight);
                h_nTrkVtx_mc->Fill(jetVar_mc[0].nTrkVtx,eventVar_mc.weight);
                h_nTrkVtx_mc->Fill(jetVar_mc[1].nTrkVtx,eventVar_mc.weight);
                h_nTrkCalo_mc->Fill(jetVar_mc[0].nTrkCalo,eventVar_mc.weight);
                h_nTrkCalo_mc->Fill(jetVar_mc[1].nTrkCalo,eventVar_mc.weight);
                //      h_JCFCalo_mc->Fill(jetVar_mc[0].chf);
                //      h_JCFCalo_mc->Fill(jetVar_mc[1].chf);
                mc_count++;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "# of events after all cut (MC) = " << mc_count << std::endl;

  /*
  tr_data->Draw("eventVar.corMass>>h_DijetMass_data",cuts);
  tr_mc->Draw("eventVar.corMass>>h_DijetMass_mc","eventVar.weight"*(cuts));
  */

  h_DijetMass_data->Sumw2();
  h_DijetMass_MI_nPVe1_data->Sumw2();
  h_DijetMass_MI_nPVg1_data->Sumw2();
  h_MET_over_sumEt_data->Sumw2();
  h_EMF_data->Sumw2();
  h_DPhi_data->Sumw2();
  h_Eta_data->Sumw2();
  h_Eta_data_1->Sumw2();
  h_Eta_data_2->Sumw2();
  h_DEta_data->Sumw2();
  h_Phi_data->Sumw2();
  h_Phi_data_1->Sumw2();
  h_Phi_data_2->Sumw2();
  h_n90hits_data->Sumw2();
  h_fHPD_data->Sumw2();
  h_corPt_data->Sumw2();
  h_corPt_data_1->Sumw2();
  h_corPt_data_2->Sumw2();
  h_nTrkVtx_data->Sumw2();
  h_nTrkCalo_data->Sumw2();

  float DijetMass_data_weight=h_DijetMass_data->Integral();
  float DijetMass_mc_weight=h_DijetMass_mc->Integral();
  float DijetMass_nPVe1_data_weight=h_DijetMass_MI_nPVe1_data->Integral();
  float DijetMass_nPVg1_data_weight=h_DijetMass_MI_nPVg1_data->Integral();
  float DijetMass_nPVe1_mc_weight=h_DijetMass_MI_nPVe1_mc->Integral();
  float DijetMass_nPVg1_mc_weight=h_DijetMass_MI_nPVg1_mc->Integral();
  float MET_over_sumEt_data_weight=h_MET_over_sumEt_data->Integral();
  float MET_over_sumEt_mc_weight=h_MET_over_sumEt_mc->Integral();
  float EMF_data_weight=h_EMF_data->Integral();
  float EMF_mc_weight=h_EMF_mc->Integral();
  float DPhi_data_weight=h_DPhi_data->Integral();
  float DPhi_mc_weight=h_DPhi_mc->Integral();
  float Eta_data_weight=h_Eta_data->Integral();
  float Eta_data_weight_1=h_Eta_data_1->Integral();
  float Eta_data_weight_2=h_Eta_data_2->Integral();
  float Eta_mc_weight=h_Eta_mc->Integral();
  float Eta_mc_weight_1=h_Eta_mc_1->Integral();
  float Eta_mc_weight_2=h_Eta_mc_2->Integral();
  float DEta_data_weight=h_Eta_data->Integral();
  float DEta_mc_weight=h_Eta_mc->Integral();
  float Phi_data_weight=h_Phi_data->Integral();
  float Phi_data_weight_1=h_Phi_data_1->Integral();
  float Phi_data_weight_2=h_Phi_data_2->Integral();
  float Phi_mc_weight=h_Phi_mc->Integral();
  float Phi_mc_weight_1=h_Phi_mc_1->Integral();
  float Phi_mc_weight_2=h_Phi_mc_2->Integral();
  float n90hits_data_weight=h_n90hits_data->Integral();
  float n90hits_mc_weight=h_n90hits_mc->Integral();
  float fHPD_data_weight=h_fHPD_data->Integral();
  float fHPD_mc_weight=h_fHPD_mc->Integral();
  float corPt_data_weight=h_corPt_data->Integral();
  float corPt_data_weight_1=h_corPt_data_1->Integral();
  float corPt_data_weight_2=h_corPt_data_2->Integral();
  float corPt_mc_weight=h_corPt_mc->Integral();
  float corPt_mc_weight_1=h_corPt_mc_1->Integral();
  float corPt_mc_weight_2=h_corPt_mc_2->Integral();
  float nTrkVtx_data_weight=h_nTrkVtx_data->Integral();
  float nTrkVtx_mc_weight=h_nTrkVtx_mc->Integral();
  float nTrkCalo_data_weight=h_nTrkCalo_data->Integral();
  float nTrkCalo_mc_weight=h_nTrkCalo_mc->Integral();
  //  float JCFCalo_data_weight=h_JCFCalo_data->Integral();
  //  float JCFCalo_mc_weight=h_JCFCalo_mc->Integral();



  c_DijetMass->cd(0);
  h_DijetMass_data->SetTitle("");
  h_DijetMass_data->SetLineWidth(3);
  h_DijetMass_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_data->SetMarkerStyle(20);

  h_DijetMass_mc->SetTitle("");
  h_DijetMass_mc->SetLineWidth(3);
  h_DijetMass_mc->SetLineColor(42);
  h_DijetMass_mc->SetFillColor(42);
  h_DijetMass_mc->GetYaxis()->SetTitle("Events");
  h_DijetMass_mc->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_mc->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_mc->Scale(DijetMass_data_weight/DijetMass_mc_weight);

  h_DijetMass_mc->Draw("");
  h_DijetMass_data->Draw("sameEP");

  l_DijetMass->SetFillColor(0);
  l_DijetMass->AddEntry( h_DijetMass_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_DijetMass->AddEntry( h_DijetMass_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_DijetMass->Draw("same");

  c_DijetMass->Update();
  outf->Append(c_DijetMass);

  c_DijetMass_log->cd(0);

  TH1F *h_DijetMass_data_log                    =(TH1F*)h_DijetMass_data->Clone();
  TH1F *h_DijetMass_mc_log                      =(TH1F*)h_DijetMass_mc->Clone();

  c_DijetMass_log->SetLogy();

  h_DijetMass_mc_log->SetMinimum(0.2);

  h_DijetMass_mc_log->Draw("");
  h_DijetMass_data_log->Draw("sameEP");

  l_DijetMass_log->SetFillColor(0);
  l_DijetMass_log->AddEntry((TObject*)0,"2011 data","");
  l_DijetMass_log->AddEntry( h_DijetMass_data_log,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_DijetMass_log->AddEntry( h_DijetMass_mc_log,"QCD  PYTHIA + CMS Simulation","f");
  l_DijetMass_log->Draw("same");

  c_DijetMass_log->Update();
  outf->Append(c_DijetMass_log);
  /*
  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data->GetBinContent(i);
    Phi_sum+=h_Phi_data->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data->GetBinContent(i)-average,2);
  }
  */
  c_DijetMass_ratio->cd(0);

  //  TH1F h_DijetMass_mc_clone_for_ratio=h_DijetMass_mc->clone();

  //  h_DijetMass_mc_clone_for_ratio->Sumw2();

  h_DijetMass_ratio->Divide(h_DijetMass_data,h_DijetMass_mc,1.0,1.0,"b");

  for(int ratio_check=1 ; ratio_check <= 83 ; ratio_check++) {
    float ratio_data = h_DijetMass_data->GetBinContent(ratio_check);
    float ratio_mc = h_DijetMass_mc->GetBinContent(ratio_check);
    float ratio_data_over_mc = h_DijetMass_data->GetBinContent(ratio_check);
    if (ratio_mc == 0 || ratio_data == 0) continue;
    //    std::cout << "Data bin = " << ratio_check << ", bincontent = " << ratio_data << ", binerror = " << pow(ratio_data,0.5) << ", mc bin = " << ratio_check << ", bincontent = " << ratio_mc << ", binerror = " << pow(ratio_mc,0.5) << ", expected bin error = " << ratio_data/ratio_mc*pow(1/ratio_data+1/ratio_mc,0.5)<<std::endl;
    //    std::cout << "bin = " << ratio_check << ", GetBinError = " << h_DijetMass_ratio->GetBinError(ratio_check) << std::endl;
    h_DijetMass_ratio->SetBinError(ratio_check,ratio_data/ratio_mc*pow(1/ratio_data+1/ratio_mc,0.5));
    //    std::cout << "bin = " << ratio_check << ", GetBinError = " << h_DijetMass_ratio->GetBinError(ratio_check) << std::endl;
  }


  h_DijetMass_ratio->SetTitle("");
  h_DijetMass_ratio->SetLineWidth(3);
  h_DijetMass_ratio->GetYaxis()->SetTitle("Data/MC");
  h_DijetMass_ratio->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_ratio->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_ratio->SetMarkerStyle(20);
  h_DijetMass_ratio->SetMinimum(0.0);

  h_DijetMass_ratio->Draw("EP");

  TText *t_DijetMass_ratio_1 = pt_DijetMass_ratio->AddText("CMS 2011 Data ( 33.684pb^{-1} )");

  pt_DijetMass_ratio->Draw("sames");

  c_DijetMass_ratio->Update();
  outf->Append(c_DijetMass_ratio);

  c_DijetMass_ratio_log->cd(0);

  TH1F *h_DijetMass_ratio_log                   =(TH1F*)h_DijetMass_ratio->Clone();

  c_DijetMass_ratio_log->SetLogy();

  h_DijetMass_ratio_log->SetMinimum(0.5);
  h_DijetMass_ratio_log->Draw("EP");


  TText *t_DijetMass_ratio_log_1 = pt_DijetMass_ratio_log->AddText("CMS 2011 Data ( 33.684pb^{-1} )");

  pt_DijetMass_ratio_log->Draw("sames");


  c_DijetMass_ratio_log->Update();
  outf->Append(c_DijetMass_ratio_log);

  c_DijetMass_MI->cd(0);
  h_DijetMass_MI_nPVe1_data->SetTitle("");
  h_DijetMass_MI_nPVe1_data->SetLineWidth(3);
  h_DijetMass_MI_nPVe1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVe1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVe1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVe1_data->SetMarkerStyle(20);

  h_DijetMass_MI_nPVg1_data->SetTitle("");
  h_DijetMass_MI_nPVg1_data->SetLineWidth(1);
  h_DijetMass_MI_nPVg1_data->SetLineColor(2);
  h_DijetMass_MI_nPVg1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVg1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVg1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVg1_data->SetMarkerStyle(20);
  h_DijetMass_MI_nPVg1_data->SetMarkerColor(2);

  h_DijetMass_MI_nPVe1_mc->SetTitle("");
  h_DijetMass_MI_nPVe1_mc->SetLineWidth(3);
  h_DijetMass_MI_nPVe1_mc->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVe1_mc->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVe1_mc->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVe1_mc->Scale(DijetMass_nPVe1_data_weight/DijetMass_nPVe1_mc_weight);

  h_DijetMass_MI_nPVg1_mc->SetTitle("");
  h_DijetMass_MI_nPVg1_mc->SetLineWidth(3);
  h_DijetMass_MI_nPVg1_mc->SetLineColor(2);
  h_DijetMass_MI_nPVg1_mc->GetYaxis()->SetTitle("Events");
  h_DijetMass_MI_nPVg1_mc->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_MI_nPVg1_mc->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_MI_nPVg1_mc->Scale(DijetMass_nPVg1_data_weight/DijetMass_nPVg1_mc_weight);


  h_DijetMass_MI_nPVe1_data->Draw("EP");
  h_DijetMass_MI_nPVg1_data->Draw("sameEP");
  h_DijetMass_MI_nPVe1_mc->Draw("same");
  h_DijetMass_MI_nPVg1_mc->Draw("same");
  l_DijetMass_MI->SetFillColor(0);
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVe1_data,"CMS 2011 Data ( 33.684pb^{-1} ) with 1 primary vertex","p");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVg1_data,"CMS 2011 Data ( 33.684pb^{-1} ) with more than 1 primary vertex","p");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVe1_mc,"QCD  PYTHIA + CMS Simulation with 1 primary vertex","f");
  l_DijetMass_MI->AddEntry( h_DijetMass_MI_nPVg1_mc,"QCD  PYTHIA + CMS Simulation with more than 1 primary vertex","f");
  l_DijetMass_MI->Draw("same");

  c_DijetMass_MI->Update();
  outf->Append(c_DijetMass_MI);

  c_DijetMass_MI_log->cd(0);

  TH1F *h_DijetMass_MI_nPVe1_data_log           =(TH1F*)h_DijetMass_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_MI_nPVg1_data_log           =(TH1F*)h_DijetMass_MI_nPVg1_data->Clone();
  TH1F *h_DijetMass_MI_nPVe1_mc_log             =(TH1F*)h_DijetMass_MI_nPVe1_mc->Clone();
  TH1F *h_DijetMass_MI_nPVg1_mc_log             =(TH1F*)h_DijetMass_MI_nPVg1_mc->Clone();

  c_DijetMass_MI_log->SetLogy();

  h_DijetMass_MI_nPVe1_data_log->SetMinimum(0.5);

  h_DijetMass_MI_nPVe1_data_log->Draw("EP");
  h_DijetMass_MI_nPVg1_data_log->Draw("sameEP");
  h_DijetMass_MI_nPVe1_mc_log->Draw("same");
  h_DijetMass_MI_nPVg1_mc_log->Draw("same");
  l_DijetMass_MI_log->SetFillColor(0);
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVe1_data_log,"CMS 2011 Data ( 33.684pb^{-1} ) with 1 primary vertex","p");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVg1_data_log,"CMS 2011 Data ( 33.684pb^{-1} ) with more than 1 primary vertex","p");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVe1_mc_log,"QCD  PYTHIA + CMS Simulation with 1 primary vertex","f");
  l_DijetMass_MI_log->AddEntry( h_DijetMass_MI_nPVg1_mc_log,"QCD  PYTHIA + CMS Simulation with more than 1 primary vertex","f");
  l_DijetMass_MI_log->Draw("same");

  c_DijetMass_MI_log->Update();
  outf->Append(c_DijetMass_MI_log);

  c_DijetMass_normalized_MI->cd(0);
  h_DijetMass_normalized_MI_nPVe1_data->SetTitle("");
  h_DijetMass_normalized_MI_nPVe1_data->SetLineWidth(3);
  h_DijetMass_normalized_MI_nPVe1_data->SetLineColor(42);
  h_DijetMass_normalized_MI_nPVe1_data->SetFillColor(42);
  h_DijetMass_normalized_MI_nPVe1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_normalized_MI_nPVe1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_normalized_MI_nPVe1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_normalized_MI_nPVe1_data->Scale(DijetMass_nPVg1_data_weight/DijetMass_nPVe1_data_weight);

  h_DijetMass_normalized_MI_nPVg1_data->SetTitle("");
  h_DijetMass_normalized_MI_nPVg1_data->SetLineWidth(3);
  h_DijetMass_normalized_MI_nPVg1_data->GetYaxis()->SetTitle("Events");
  h_DijetMass_normalized_MI_nPVg1_data->GetYaxis()->SetLabelSize(0.03);
  h_DijetMass_normalized_MI_nPVg1_data->GetXaxis()->SetTitle("DijetMass");
  h_DijetMass_normalized_MI_nPVg1_data->SetMarkerStyle(20);


  h_DijetMass_normalized_MI_nPVe1_data->Draw("");
  h_DijetMass_normalized_MI_nPVg1_data->Draw("sameEP");
  l_DijetMass_normalized_MI->SetFillColor(0);
  l_DijetMass_normalized_MI->AddEntry( h_DijetMass_normalized_MI_nPVe1_data,"CMS 2011 Data ( 33.684pb^{-1} ) with 1 primary vertex","f");
  l_DijetMass_normalized_MI->AddEntry( h_DijetMass_normalized_MI_nPVg1_data,"CMS 2011 Data ( 33.684pb^{-1} ) with morethan 1 primary vertex","p");
  l_DijetMass_normalized_MI->Draw("same");

  c_DijetMass_normalized_MI->Update();
  outf->Append(c_DijetMass_normalized_MI);

  c_DijetMass_normalized_MI_log->cd(0);

  TH1F *h_DijetMass_normalized_MI_nPVe1_data_log=(TH1F*)h_DijetMass_normalized_MI_nPVe1_data->Clone();
  TH1F *h_DijetMass_normalized_MI_nPVg1_data_log=(TH1F*)h_DijetMass_normalized_MI_nPVg1_data->Clone();

  c_DijetMass_normalized_MI_log->SetLogy();

  h_DijetMass_normalized_MI_nPVe1_data_log->SetMinimum(0.5);

  h_DijetMass_normalized_MI_nPVe1_data_log->Draw("");
  h_DijetMass_normalized_MI_nPVg1_data_log->Draw("sameEP");
  l_DijetMass_normalized_MI_log->SetFillColor(0);
  l_DijetMass_normalized_MI_log->AddEntry( h_DijetMass_normalized_MI_nPVe1_data_log,"CMS 2011 Data ( 33.684pb^{-1} ) with 1 primary vertex","f");
  l_DijetMass_normalized_MI_log->AddEntry( h_DijetMass_normalized_MI_nPVg1_data_log,"CMS 2011 Data ( 33.684pb^{-1} ) with morethan 1 primary vertex","p");
  l_DijetMass_normalized_MI_log->Draw("same");

  c_DijetMass_normalized_MI_log->Update();
  outf->Append(c_DijetMass_normalized_MI_log);

  // Eta_Phi_scatter for two leading jets

  c_Eta_Phi_Scatter->cd(0);

  TPaletteAxis *palette = new TPaletteAxis(3.079823,-3.213588,3.345898,3.186412,h_Eta_Phi_Scatter_data);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.007);
  palette->SetLabelSize(0.04);
  palette->SetTitleOffset(1);
  palette->SetTitleSize(0.06);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  h_Eta_Phi_Scatter_data->GetListOfFunctions()->Add(palette,"br");
  h_Eta_Phi_Scatter_data->SetTitle("");
  h_Eta_Phi_Scatter_data->SetMarkerStyle(20);
  h_Eta_Phi_Scatter_data->SetMarkerSize(0.2);
  h_Eta_Phi_Scatter_data->GetYaxis()->SetTitle("#phi");
  h_Eta_Phi_Scatter_data->GetXaxis()->SetTitle("#eta");
  h_Eta_Phi_Scatter_data->Draw("COLZ");

  pt_Eta_Phi_Scatter->SetBorderSize(0);
  pt_Eta_Phi_Scatter->SetFillColor(0);

  TText *t_Eta_Phi_Scatter = pt_Eta_Phi_Scatter->AddText("CMS 2011 Data ( 33.684pb^{-1} )");

  pt_Eta_Phi_Scatter->Draw("sames");

  c_Eta_Phi_Scatter->Update();
  outf->Append(h_Eta_Phi_Scatter_data);

  // Eta_Phi_scatter for first leading jets

  c_Eta_Phi_Scatter_1->cd(0);

  TPaletteAxis *palette = new TPaletteAxis(3.079823,-3.213588,3.345898,3.186412,h_Eta_Phi_Scatter_data_1);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.007);
  palette->SetLabelSize(0.04);
  palette->SetTitleOffset(1);
  palette->SetTitleSize(0.06);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  h_Eta_Phi_Scatter_data_1->GetListOfFunctions()->Add(palette,"br");
  h_Eta_Phi_Scatter_data_1->SetTitle("");
  h_Eta_Phi_Scatter_data_1->SetMarkerStyle(20);
  h_Eta_Phi_Scatter_data_1->SetMarkerSize(0.2);
  h_Eta_Phi_Scatter_data_1->GetYaxis()->SetTitle("#phi");
  h_Eta_Phi_Scatter_data_1->GetXaxis()->SetTitle("#eta");
  h_Eta_Phi_Scatter_data_1->Draw("COLZ");

  pt_Eta_Phi_Scatter_1->SetBorderSize(0);
  pt_Eta_Phi_Scatter_1->SetFillColor(0);

  TText *t_Eta_Phi_Scatter_1 = pt_Eta_Phi_Scatter_1->AddText("CMS 2011 Data ( 33.684pb^{-1} )");

  pt_Eta_Phi_Scatter_1->Draw("sames");

  c_Eta_Phi_Scatter_1->Update();
  outf->Append(h_Eta_Phi_Scatter_data_1);


  // Eta_Phi_scatter for second leading jets

  c_Eta_Phi_Scatter_2->cd(0);

  TPaletteAxis *palette = new TPaletteAxis(3.079823,-3.213588,3.345898,3.186412,h_Eta_Phi_Scatter_data_2);
  palette->SetLabelColor(1);
  palette->SetLabelFont(42);
  palette->SetLabelOffset(0.007);
  palette->SetLabelSize(0.04);
  palette->SetTitleOffset(1);
  palette->SetTitleSize(0.06);
  palette->SetFillColor(100);
  palette->SetFillStyle(1001);

  h_Eta_Phi_Scatter_data_2->GetListOfFunctions()->Add(palette,"br");
  h_Eta_Phi_Scatter_data_2->SetTitle("");
  h_Eta_Phi_Scatter_data_2->SetMarkerStyle(20);
  h_Eta_Phi_Scatter_data_2->SetMarkerSize(0.2);
  h_Eta_Phi_Scatter_data_2->GetYaxis()->SetTitle("#phi");
  h_Eta_Phi_Scatter_data_2->GetXaxis()->SetTitle("#eta");
  h_Eta_Phi_Scatter_data_2->Draw("COLZ");

  pt_Eta_Phi_Scatter_2->SetBorderSize(0);
  pt_Eta_Phi_Scatter_2->SetFillColor(0);

  TText *t_Eta_Phi_Scatter_2 = pt_Eta_Phi_Scatter_2->AddText("CMS 2011 Data ( 33.684pb^{-1} )");

  pt_Eta_Phi_Scatter_2->Draw("sames");

  c_Eta_Phi_Scatter_2->Update();
  outf->Append(h_Eta_Phi_Scatter_data_2);

  // MET/SemET

  c_MET_over_sumEt->cd(0);
  h_MET_over_sumEt_data->SetTitle("");
  h_MET_over_sumEt_data->GetYaxis()->SetTitle("Events");
  h_MET_over_sumEt_data->GetYaxis()->SetLabelSize(0.03);
  h_MET_over_sumEt_data->GetXaxis()->SetTitle("#slash{E}_{T}/#sum_{}E_{T}");
  h_MET_over_sumEt_data->SetMarkerStyle(20);


  h_MET_over_sumEt_mc->SetLineWidth(3);
  h_MET_over_sumEt_mc->SetTitle("");
  h_MET_over_sumEt_mc->SetFillColor(42);
  h_MET_over_sumEt_mc->SetLineColor(42);
  h_MET_over_sumEt_mc->GetYaxis()->SetTitle("Events");
  h_MET_over_sumEt_mc->GetYaxis()->SetLabelSize(0.03);
  h_MET_over_sumEt_mc->GetXaxis()->SetTitle("#slash{E}_{T}/#sum_{}E_{T}");
  h_MET_over_sumEt_mc->Scale(MET_over_sumEt_data_weight/MET_over_sumEt_mc_weight);

  h_MET_over_sumEt_mc->Draw("");
  h_MET_over_sumEt_data->Draw("sameEP");

  l_MET_over_sumEt->SetFillColor(0);
  l_MET_over_sumEt->AddEntry(h_MET_over_sumEt_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_MET_over_sumEt->AddEntry(h_MET_over_sumEt_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_MET_over_sumEt->Draw("sames");

  c_MET_over_sumEt->Update();
  outf->Append(c_MET_over_sumEt);

  c_MET_over_sumEt_log->cd(0);

  TH1F *h_MET_over_sumEt_data_log               =(TH1F*)h_MET_over_sumEt_data->Clone();
  TH1F *h_MET_over_sumEt_mc_log                 =(TH1F*)h_MET_over_sumEt_mc->Clone();

  c_MET_over_sumEt_log->SetLogy();

  h_MET_over_sumEt_mc_log->SetMinimum(0.5);

  h_MET_over_sumEt_mc_log->Draw("");
  h_MET_over_sumEt_data_log->Draw("sameEP");

  l_MET_over_sumEt_log->SetFillColor(0);
  l_MET_over_sumEt_log->AddEntry(h_MET_over_sumEt_data_log,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_MET_over_sumEt_log->AddEntry(h_MET_over_sumEt_mc_log,"QCD  PYTHIA + CMS Simulation","f");
  l_MET_over_sumEt_log->Draw("sames");

  c_MET_over_sumEt_log->Update();
  outf->Append(c_MET_over_sumEt_log);


  c_EMF->cd(0);
  h_EMF_data->SetTitle("");
  h_EMF_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_EMF_data->GetYaxis()->SetLabelSize(0.03);
  h_EMF_data->GetXaxis()->SetTitle("Jet EMF");
  h_EMF_data->SetMarkerStyle(20);
  h_EMF_mc->SetLineWidth(3);
  h_EMF_mc->SetTitle("");
  h_EMF_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_EMF_mc->GetYaxis()->SetLabelSize(0.03);
  h_EMF_mc->GetXaxis()->SetTitle("Jet EMF");
  h_EMF_mc->SetFillColor(42);
  h_EMF_mc->SetLineColor(42);
  h_EMF_mc->Scale(EMF_data_weight/EMF_mc_weight);

  h_EMF_mc->Draw("");
  h_EMF_data->Draw("sameEP");

  l_EMF->SetFillColor(0);
  l_EMF->AddEntry(h_EMF_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_EMF->AddEntry(h_EMF_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_EMF->Draw("sames");

  c_EMF->Update();
  outf->Append(c_EMF);

  c_DPhi->cd(0);
  h_DPhi_data->SetTitle("");
  h_DPhi_data->GetYaxis()->SetTitle("Events");
  h_DPhi_data->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_data->GetXaxis()->SetTitle("#Delta #phi");
  h_DPhi_data->SetMarkerStyle(20);

  h_DPhi_mc->SetLineWidth(3);
  h_DPhi_mc->SetTitle("");
  h_DPhi_mc->SetFillColor(42);
  h_DPhi_mc->SetLineColor(42);
  h_DPhi_mc->GetYaxis()->SetTitle("events");
  h_DPhi_mc->GetYaxis()->SetLabelSize(0.03);
  h_DPhi_mc->GetXaxis()->SetTitle("#Delta #phi");
  h_DPhi_mc->Scale(DPhi_data_weight/DPhi_mc_weight);

  h_DPhi_mc->Draw("");
  h_DPhi_data->Draw("sameEP");

  l_DPhi->SetFillColor(0);
  l_DPhi->AddEntry(h_DPhi_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_DPhi->AddEntry(h_DPhi_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_DPhi->Draw("sames");

  c_DPhi->Update();
  outf->Append(c_DPhi);

  c_DPhi_log->cd(0);

  TH1F *h_DPhi_data_log                         =(TH1F*)h_DPhi_data->Clone();
  TH1F *h_DPhi_mc_log                           =(TH1F*)h_DPhi_mc->Clone();

  c_DPhi_log->SetLogy();

  h_DPhi_mc_log->SetMinimum(0.5);

  h_DPhi_mc_log->Draw("");
  h_DPhi_data_log->Draw("sameEP");

  l_DPhi_log->SetFillColor(0);
  l_DPhi_log->AddEntry(h_DPhi_data_log,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_DPhi_log->AddEntry(h_DPhi_mc_log,"QCD  PYTHIA + CMS Simulation","f");
  l_DPhi_log->Draw("sames");

  c_DPhi_log->Update();
  outf->Append(c_DPhi_log);

  // Eta distribution for two leading jets

  c_Eta->cd(0);
  h_Eta_data->SetTitle("");
  h_Eta_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Eta_data->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_data->SetMarkerStyle(20);

  h_Eta_mc->SetLineWidth(3);
  h_Eta_mc->SetTitle("");
  h_Eta_mc->SetFillColor(42);
  h_Eta_mc->SetLineColor(42);
  h_Eta_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Eta_mc->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_mc->Scale(Eta_data_weight/Eta_mc_weight);

  h_Eta_mc->Draw("");
  h_Eta_data->Draw("sameEP");

  l_Eta->SetFillColor(0);
  l_Eta->AddEntry(h_Eta_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Eta->AddEntry(h_Eta_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_Eta->AddEntry(h_Eta_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Eta->AddEntry(h_Eta_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_Eta->Draw("sames");

  c_Eta->Update();
  outf->Append(c_Eta);

  // Eta distribution for first leading jets

  c_Eta_1->cd(0);
  h_Eta_data_1->SetTitle("");
  h_Eta_data_1->GetYaxis()->SetTitle("first leading jets");
  h_Eta_data_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_1->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_data_1->SetMarkerStyle(20);

  h_Eta_mc_1->SetLineWidth(3);
  h_Eta_mc_1->SetTitle("");
  h_Eta_mc_1->SetFillColor(42);
  h_Eta_mc_1->SetLineColor(42);
  h_Eta_mc_1->GetYaxis()->SetTitle("first leading jets");
  h_Eta_mc_1->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_1->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_mc_1->Scale(Eta_data_weight_1/Eta_mc_weight_1);

  h_Eta_mc_1->Draw("");
  h_Eta_data_1->Draw("sameEP");

  l_Eta_1->SetFillColor(0);
  l_Eta_1->AddEntry(h_Eta_data_1,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Eta_1->AddEntry(h_Eta_mc_1,"QCD  PYTHIA + CMS Simulation","f");
  l_Eta_1->Draw("sames");

  c_Eta_1->Update();
  outf->Append(c_Eta_1);

  // Eta distribution for second leading jets

  c_Eta_2->cd(0);
  h_Eta_data_2->SetTitle("");
  h_Eta_data_2->GetYaxis()->SetTitle("second leading jets");
  h_Eta_data_2->GetYaxis()->SetLabelSize(0.03);
  h_Eta_data_2->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_data_2->SetMarkerStyle(20);

  h_Eta_mc_2->SetLineWidth(3);
  h_Eta_mc_2->SetTitle("");
  h_Eta_mc_2->SetFillColor(42);
  h_Eta_mc_2->SetLineColor(42);
  h_Eta_mc_2->GetYaxis()->SetTitle("second leading jets");
  h_Eta_mc_2->GetYaxis()->SetLabelSize(0.03);
  h_Eta_mc_2->GetXaxis()->SetTitle("Jet #eta");
  h_Eta_mc_2->Scale(Eta_data_weight_2/Eta_mc_weight_2);

  h_Eta_mc_2->Draw("");
  h_Eta_data_2->Draw("sameEP");

  l_Eta_2->SetFillColor(0);
  l_Eta_2->AddEntry(h_Eta_data_2,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Eta_2->AddEntry(h_Eta_mc_2,"QCD  PYTHIA + CMS Simulation","f");
  l_Eta_2->Draw("sames");

  c_Eta_2->Update();
  outf->Append(c_Eta_2);

  // Delta Eta b/w two leading jets

  c_DEta->cd(0);
  h_DEta_data->SetTitle("");
  h_DEta_data->GetYaxis()->SetTitle("Events");
  h_DEta_data->GetYaxis()->SetLabelSize(0.03);
  h_DEta_data->GetXaxis()->SetTitle("Jet #Delta#eta");
  h_DEta_data->SetMarkerStyle(20);

  h_DEta_mc->SetLineWidth(3);
  h_DEta_mc->SetTitle("");
  h_DEta_mc->SetFillColor(42);
  h_DEta_mc->SetLineColor(42);
  h_DEta_mc->GetYaxis()->SetTitle("Events");
  h_DEta_mc->GetYaxis()->SetLabelSize(0.03);
  h_DEta_mc->GetXaxis()->SetTitle("Jet #Delta#eta");
  h_DEta_mc->Scale(DEta_data_weight/DEta_mc_weight);
  h_DEta_mc->Draw("");
  h_DEta_data->Draw("sameEP");

  l_DEta->SetFillColor(0);
  l_DEta->AddEntry(h_DEta_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_DEta->AddEntry(h_DEta_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_DEta->Draw("sames");

  c_DEta->Update();
  outf->Append(c_DEta);

  // phi distribution for two leading jets

  c_Phi->cd(0);


  float average = Phi_data_weight/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data->GetBinContent(i);
    Phi_sum+=h_Phi_data->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data->GetBinContent(i)-average,2);
  }

  h_Phi_data->SetTitle("");
  h_Phi_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Phi_data->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_data->SetMarkerStyle(20);

  h_Phi_mc->SetLineWidth(3);
  h_Phi_mc->SetTitle("");
  h_Phi_mc->SetFillColor(42);
  h_Phi_mc->SetLineColor(42);
  h_Phi_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_Phi_mc->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_mc->Scale(Phi_data_weight/Phi_mc_weight);
  h_Phi_mc->SetMinimum(average*0.8);
  h_Phi_mc->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi = new TGraph(2);

  g_phi->SetFillColor(1);
  g_phi->SetLineColor(2);
  g_phi->SetLineStyle(3);
  g_phi->SetLineWidth(3);
  g_phi->SetMarkerStyle(20);
  g_phi->SetPoint(0,-3.2,average);
  g_phi->SetPoint(1,3.2,average);
  h_Phi_mc->Draw("");
  h_Phi_data->Draw("sameEP");
  g_phi->Draw("l");


  pt_Phi->SetBorderSize(0);
  pt_Phi->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_1 = pt_Phi->AddText(s_average);
  TText *t_phi_2 = pt_Phi->AddText(s_rms);
  TText *t_phi_3 = pt_Phi->AddText(s_rms_percentage);

  pt_Phi->Draw("sames");

  l_Phi->SetFillColor(0);
  l_Phi->AddEntry(h_Phi_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Phi->AddEntry(h_Phi_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_Phi->Draw("sames");

  c_Phi->Update();
  outf->Append(c_Phi);

  // phi distribution for first leading jets

  c_Phi_1->cd(0);


  float average = Phi_data_weight_1/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_1->GetBinContent(i);
    Phi_sum+=h_Phi_data_1->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_1->GetBinContent(i)-average,2);
  }
  h_Phi_data_1->SetTitle("");
  h_Phi_data_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_data_1->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_1->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_data_1->SetMarkerStyle(20);

  h_Phi_mc_1->SetLineWidth(3);
  h_Phi_mc_1->SetTitle("");
  h_Phi_mc_1->SetFillColor(42);
  h_Phi_mc_1->SetLineColor(42);
  h_Phi_mc_1->GetYaxis()->SetTitle("first leading jets");
  h_Phi_mc_1->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_1->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_mc_1->Scale(Phi_data_weight_1/Phi_mc_weight_1);
  h_Phi_mc_1->SetMinimum(average*0.8);
  h_Phi_mc_1->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi_1 = new TGraph(2);

  g_phi_1->SetFillColor(1);
  g_phi_1->SetLineColor(2);
  g_phi_1->SetLineStyle(3);
  g_phi_1->SetLineWidth(3);
  g_phi_1->SetMarkerStyle(20);
  g_phi_1->SetPoint(0,-3.2,average);
  g_phi_1->SetPoint(1,3.2,average);

  h_Phi_mc_1->Draw("");
  h_Phi_data_1->Draw("sameEP");
  g_phi_1->Draw("l");


  pt_Phi_1->SetBorderSize(0);
  pt_Phi_1->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_1 = pt_Phi_1->AddText(s_average);
  TText *t_phi_2 = pt_Phi_1->AddText(s_rms);
  TText *t_phi_3 = pt_Phi_1->AddText(s_rms_percentage);

  pt_Phi_1->Draw("sames");

  l_Phi_1->SetFillColor(0);
  l_Phi_1->AddEntry(h_Phi_data_1,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Phi_1->AddEntry(h_Phi_mc_1,"QCD  PYTHIA + CMS Simulation","f");
  l_Phi_1->Draw("sames");

  c_Phi_1->Update();
  outf->Append(c_Phi_1);

  // phi distribution for second leading jets

  c_Phi_2->cd(0);


  float average = Phi_data_weight_2/18.0;

  float Phi_value[18];
  float Phi_sum(0.0);
  float Phi_square_sum(0.0);
  float rms(0.0);

  for (i=1;i<=18;i++) {
    Phi_value[i-1]=h_Phi_data_2->GetBinContent(i);
    Phi_sum+=h_Phi_data_2->GetBinContent(i);
    Phi_square_sum+=pow(h_Phi_data_2->GetBinContent(i)-average,2);
  }

  h_Phi_data_2->SetTitle("");
  h_Phi_data_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_data_2->GetYaxis()->SetLabelSize(0.03);
  h_Phi_data_2->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_data_2->SetMarkerStyle(20);

  h_Phi_mc_2->SetLineWidth(3);
  h_Phi_mc_2->SetTitle("");
  h_Phi_mc_2->SetFillColor(42);
  h_Phi_mc_2->SetLineColor(42);
  h_Phi_mc_2->GetYaxis()->SetTitle("second leading jets");
  h_Phi_mc_2->GetYaxis()->SetLabelSize(0.03);
  h_Phi_mc_2->GetXaxis()->SetTitle("Jet #phi");
  h_Phi_mc_2->Scale(Phi_data_weight_2/Phi_mc_weight_2);
  h_Phi_mc_2->SetMinimum(average*0.8);
  h_Phi_mc_2->SetMaximum(average*1.2);

  rms = sqrt(Phi_square_sum/18.0);

  TGraph *g_phi_2 = new TGraph(2);

  g_phi_2->SetFillColor(1);
  g_phi_2->SetLineColor(2);
  g_phi_2->SetLineStyle(3);
  g_phi_2->SetLineWidth(3);
  g_phi_2->SetMarkerStyle(20);
  g_phi_2->SetPoint(0,-3.2,average);
  g_phi_2->SetPoint(1,3.2,average);

  h_Phi_mc_2->Draw("");
  h_Phi_data_2->Draw("sameEP");
  g_phi_2->Draw("l");


  pt_Phi_2->SetBorderSize(0);
  pt_Phi_2->SetFillColor(0);

  char s_average[100],s_rms[100],s_rms_percentage[100];

  sprintf(s_average,"average = %f",average);
  sprintf(s_rms,"rms = %f",rms);
  sprintf(s_rms_percentage,"rms/average = %f",rms/average);

  TText *t_phi_1 = pt_Phi_2->AddText(s_average);
  TText *t_phi_2 = pt_Phi_2->AddText(s_rms);
  TText *t_phi_3 = pt_Phi_2->AddText(s_rms_percentage);

  pt_Phi_2->Draw("sames");

  l_Phi_2->SetFillColor(0);
  l_Phi_2->AddEntry(h_Phi_data_2,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_Phi_2->AddEntry(h_Phi_mc_2,"QCD  PYTHIA + CMS Simulation","f");
  l_Phi_2->Draw("sames");

  c_Phi_2->Update();
  outf->Append(c_Phi_2);

  // n90hits distribution for two jets

  c_n90hits->cd(0);
  h_n90hits_data->SetTitle("");
  h_n90hits_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_n90hits_data->GetYaxis()->SetLabelSize(0.03);
  h_n90hits_data->GetXaxis()->SetTitle("Jet n90hits");
  h_n90hits_data->SetMarkerStyle(20);

  h_n90hits_mc->SetLineWidth(3);
  h_n90hits_mc->SetTitle("");
  h_n90hits_mc->SetFillColor(42);
  h_n90hits_mc->SetLineColor(42);
  h_n90hits_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_n90hits_mc->GetYaxis()->SetLabelSize(0.03);
  h_n90hits_mc->GetXaxis()->SetTitle("Jet n90hits");
  h_n90hits_mc->Scale(n90hits_data_weight/n90hits_mc_weight);

  h_n90hits_mc->Draw("");
  h_n90hits_data->Draw("sameEP");

  l_n90hits->SetFillColor(0);
  l_n90hits->AddEntry(h_n90hits_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_n90hits->AddEntry(h_n90hits_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_n90hits->Draw("sames");

  c_n90hits->Update();
  outf->Append(c_n90hits);

  c_n90hits_log->cd(0);

  TH1F *h_n90hits_data_log                      =(TH1F*)h_n90hits_data->Clone();
  TH1F *h_n90hits_mc_log                        =(TH1F*)h_n90hits_mc->Clone();

  c_n90hits_log->SetLogy();

  h_n90hits_mc_log->SetMinimum(0.5);

  h_n90hits_mc_log->Draw("");
  h_n90hits_data_log->Draw("sameEP");

  l_n90hits_log->SetFillColor(0);
  l_n90hits_log->AddEntry(h_n90hits_data_log,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_n90hits_log->AddEntry(h_n90hits_mc_log,"QCD  PYTHIA + CMS Simulation","f");
  l_n90hits_log->Draw("sames");

  c_n90hits_log->Update();
  outf->Append(c_n90hits_log);

  c_fHPD->cd(0);
  h_fHPD_data->SetTitle("");
  h_fHPD_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_fHPD_data->GetYaxis()->SetLabelSize(0.03);
  h_fHPD_data->GetXaxis()->SetTitle("Jet fHPD");
  h_fHPD_data->SetMarkerStyle(20);

  h_fHPD_mc->SetLineWidth(3);
  h_fHPD_mc->SetTitle("");
  h_fHPD_mc->SetFillColor(42);
  h_fHPD_mc->SetLineColor(42);
  h_fHPD_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_fHPD_mc->GetYaxis()->SetLabelSize(0.03);
  h_fHPD_mc->GetXaxis()->SetTitle("Jet fHPD");
  h_fHPD_mc->Scale(fHPD_data_weight/fHPD_mc_weight);

  h_fHPD_mc->Draw("");
  h_fHPD_data->Draw("sameEP");

  l_fHPD->SetFillColor(0);
  l_fHPD->AddEntry(h_fHPD_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_fHPD->AddEntry(h_fHPD_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_fHPD->Draw("sames");

  c_fHPD->Update();
  outf->Append(c_fHPD);

  // corrected pt distribution for two leading jets

  c_corPt->cd(0);
  h_corPt_data->SetTitle("");
  h_corPt_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_corPt_data->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_data->SetMarkerStyle(20);

  h_corPt_mc->SetLineWidth(3);
  h_corPt_mc->SetTitle("");
  h_corPt_mc->SetFillColor(42);
  h_corPt_mc->SetLineColor(42);
  h_corPt_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_corPt_mc->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_mc->Scale(corPt_data_weight/corPt_mc_weight);

  h_corPt_mc->Draw("");
  h_corPt_data->Draw("sameEP");
  l_corPt->SetFillColor(0);
  l_corPt->AddEntry(h_corPt_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_corPt->AddEntry(h_corPt_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_corPt->Draw("sames");

  c_corPt->Update();
  outf->Append(c_corPt);

  c_corPt_log->cd(0);

  TH1F *h_corPt_data_log                        =(TH1F*)h_corPt_data->Clone();
  TH1F *h_corPt_mc_log                          =(TH1F*)h_corPt_mc->Clone();

  c_corPt_log->SetLogy();

  h_corPt_mc_log->SetMinimum(0.5);

  h_corPt_mc_log->Draw("");
  h_corPt_data_log->Draw("sameEP");

  l_corPt_log->SetFillColor(0);
  l_corPt_log->AddEntry((TObject*)0,"2011 data","");
  l_corPt_log->AddEntry(h_corPt_data_log,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_corPt_log->AddEntry(h_corPt_mc_log,"QCD  PYTHIA + CMS Simulation","f");
  l_corPt_log->Draw("sames");

  c_corPt_log->Update();
  outf->Append(c_corPt_log);

  // corrected pt distribution for first leading jets

  c_corPt_1->cd(0);
  h_corPt_data_1->SetTitle("");
  h_corPt_data_1->GetYaxis()->SetTitle("first leading jets");
  h_corPt_data_1->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_1->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_data_1->SetMarkerStyle(20);

  h_corPt_mc_1->SetLineWidth(3);
  h_corPt_mc_1->SetTitle("");
  h_corPt_mc_1->SetFillColor(42);
  h_corPt_mc_1->SetLineColor(42);
  h_corPt_mc_1->GetYaxis()->SetTitle("first leading jets");
  h_corPt_mc_1->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_1->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_mc_1->Scale(corPt_data_weight_1/corPt_mc_weight_1);

  h_corPt_mc_1->Draw("");
  h_corPt_data_1->Draw("sameEP");

  l_corPt_1->SetFillColor(0);
  l_corPt_1->AddEntry(h_corPt_data_1,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_corPt_1->AddEntry(h_corPt_mc_1,"QCD  PYTHIA + CMS Simulation","f");
  l_corPt_1->Draw("sames");

  c_corPt_1->Update();
  outf->Append(c_corPt_1);

  c_corPt_log_1->cd(0);

  TH1F *h_corPt_data_log_1                        =(TH1F*)h_corPt_data_1->Clone();
  TH1F *h_corPt_mc_log_1                          =(TH1F*)h_corPt_mc_1->Clone();

  c_corPt_log_1->SetLogy();

  h_corPt_mc_log_1->SetMinimum(0.5);

  h_corPt_mc_log_1->Draw("");
  h_corPt_data_log_1->Draw("sameEP");

  l_corPt_log_1->SetFillColor(0);
  l_corPt_log_1->AddEntry((TObject*)0,"2011 data","");
  l_corPt_log_1->AddEntry(h_corPt_data_log_1,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_corPt_log_1->AddEntry(h_corPt_mc_log_1,"QCD  PYTHIA + CMS Simulation","f");
  l_corPt_log_1->Draw("sames");

  c_corPt_log_1->Update();
  outf->Append(c_corPt_log_1);

  // corrected pt distribution for second leading jets

  c_corPt_2->cd(0);
  h_corPt_data_2->SetTitle("");
  h_corPt_data_2->GetYaxis()->SetTitle("second leading jets");
  h_corPt_data_2->GetYaxis()->SetLabelSize(0.03);
  h_corPt_data_2->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_data_2->SetMarkerStyle(20);

  h_corPt_mc_2->SetLineWidth(3);
  h_corPt_mc_2->SetTitle("");
  h_corPt_mc_2->SetFillColor(42);
  h_corPt_mc_2->SetLineColor(42);
  h_corPt_mc_2->GetYaxis()->SetTitle("second leading jets");
  h_corPt_mc_2->GetYaxis()->SetLabelSize(0.03);
  h_corPt_mc_2->GetXaxis()->SetTitle("Jet cor P_{T}");
  h_corPt_mc_2->Scale(corPt_data_weight_2/corPt_mc_weight_2);

  h_corPt_mc_2->Draw("");
  h_corPt_data_2->Draw("sameEP");

  l_corPt_2->SetFillColor(0);
  l_corPt_2->AddEntry(h_corPt_data_2,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_corPt_2->AddEntry(h_corPt_mc_2,"QCD  PYTHIA + CMS Simulation","f");
  l_corPt_2->Draw("sames");

  c_corPt_2->Update();
  outf->Append(c_corPt_2);

  c_corPt_log_2->cd(0);

  TH1F *h_corPt_data_log_2                        =(TH1F*)h_corPt_data_2->Clone();
  TH1F *h_corPt_mc_log_2                          =(TH1F*)h_corPt_mc_2->Clone();

  c_corPt_log_2->SetLogy();

  h_corPt_mc_log_2->SetMinimum(0.5);

  h_corPt_mc_log_2->Draw("");
  h_corPt_data_log_2->Draw("sameEP");

  l_corPt_log_2->SetFillColor(0);
  l_corPt_log_2->AddEntry((TObject*)0,"2011 data","");
  l_corPt_log_2->AddEntry(h_corPt_data_log_2,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_corPt_log_2->AddEntry(h_corPt_mc_log_2,"QCD  PYTHIA + CMS Simulation","f");
  l_corPt_log_2->Draw("sames");

  c_corPt_log_2->Update();
  outf->Append(c_corPt_log_2);

  // nTrkVtx distribution for two leading jets

  c_nTrkVtx->cd(0);
  h_nTrkVtx_data->SetTitle("");
  h_nTrkVtx_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkVtx_data->GetYaxis()->SetLabelSize(0.03);
  h_nTrkVtx_data->GetXaxis()->SetTitle("nTrkVtx");
  h_nTrkVtx_data->SetMarkerStyle(20);

  h_nTrkVtx_mc->SetLineWidth(3);
  h_nTrkVtx_mc->SetTitle("");
  h_nTrkVtx_mc->SetFillColor(42);
  h_nTrkVtx_mc->SetLineColor(42);
  h_nTrkVtx_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkVtx_mc->GetYaxis()->SetLabelSize(0.03);
  h_nTrkVtx_mc->GetXaxis()->SetTitle("nTrkVtx");
  h_nTrkVtx_mc->Scale(nTrkVtx_data_weight/nTrkVtx_mc_weight);

  h_nTrkVtx_mc->Draw("");
  h_nTrkVtx_data->Draw("sameEP");

  l_nTrkVtx->SetFillColor(0);
  l_nTrkVtx->AddEntry(h_nTrkVtx_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_nTrkVtx->AddEntry(h_nTrkVtx_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_nTrkVtx->Draw("sames");

  c_nTrkVtx->Update();
  outf->Append(c_nTrkVtx);

  c_nTrkCalo->cd(0);
  h_nTrkCalo_data->SetTitle("");
  h_nTrkCalo_data->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkCalo_data->GetYaxis()->SetLabelSize(0.03);
  h_nTrkCalo_data->GetXaxis()->SetTitle("nTrkCalo");
  h_nTrkCalo_data->SetMarkerStyle(20);

  h_nTrkCalo_mc->SetLineWidth(3);
  h_nTrkCalo_mc->SetTitle("");
  h_nTrkCalo_mc->SetFillColor(42);
  h_nTrkCalo_mc->SetLineColor(42);
  h_nTrkCalo_mc->GetYaxis()->SetTitle("Jets(two leading jets)");
  h_nTrkCalo_mc->GetYaxis()->SetLabelSize(0.03);
  h_nTrkCalo_mc->GetXaxis()->SetTitle("nTrkCalo");
  h_nTrkCalo_mc->Scale(nTrkCalo_data_weight/nTrkCalo_mc_weight);

  h_nTrkCalo_mc->Draw("");
  h_nTrkCalo_data->Draw("sameEP");

  l_nTrkCalo->SetFillColor(0);
  l_nTrkCalo->AddEntry(h_nTrkCalo_data,"CMS 2011 Data ( 33.684pb^{-1} )","p");
  l_nTrkCalo->AddEntry(h_nTrkCalo_mc,"QCD  PYTHIA + CMS Simulation","f");
  l_nTrkCalo->Draw("sames");

  c_nTrkCalo->Update();
  outf->Append(c_nTrkCalo);

  outf->Append(h_DijetMass_data);
  outf->Append(h_DijetMass_MI_nPVe1_data);
  outf->Append(h_DijetMass_MI_nPVg1_data);
  outf->Append(h_MET_over_sumEt_data);
  outf->Append(h_EMF_data);
  outf->Append(h_DPhi_data);
  outf->Append(h_Eta_data);
  outf->Append(h_Eta_data_1);
  outf->Append(h_Eta_data_2);
  outf->Append(h_DEta_data);
  outf->Append(h_Phi_data);
  outf->Append(h_Phi_data_1);
  outf->Append(h_Phi_data_2);
  outf->Append(h_n90hits_data);
  outf->Append(h_fHPD_data);
  outf->Append(h_corPt_data);
  outf->Append(h_corPt_data_1);
  outf->Append(h_corPt_data_2);
  outf->Append(h_nTrkVtx_data);
  outf->Append(h_nTrkCalo_data);
  outf->Append(h_DijetMass_normalized_MI_nPVe1_data);
  outf->Append(h_DijetMass_normalized_MI_nPVg1_data);

  outf->Append(h_DijetMass_mc);
  outf->Append(h_DijetMass_MI_nPVe1_mc);
  outf->Append(h_DijetMass_MI_nPVg1_mc);
  outf->Append(h_MET_over_sumEt_mc);
  outf->Append(h_EMF_mc);
  outf->Append(h_DPhi_mc);
  outf->Append(h_Eta_mc);
  outf->Append(h_Eta_mc_1);
  outf->Append(h_Eta_mc_2);
  outf->Append(h_DEta_mc);
  outf->Append(h_Phi_mc);
  outf->Append(h_Phi_mc_1);
  outf->Append(h_Phi_mc_2);
  outf->Append(h_n90hits_mc);
  outf->Append(h_fHPD_mc);
  outf->Append(h_corPt_mc);
  outf->Append(h_corPt_mc_1);
  outf->Append(h_corPt_mc_2);
  outf->Append(h_nTrkVtx_mc);
  outf->Append(h_nTrkCalo_mc);

  c_DijetMass->SaveAs("c_DijetMass.png");
  c_DijetMass_log->SaveAs("c_DijetMass_log.png");
  c_DijetMass_ratio->SaveAs("c_DijetMass_ratio.png");
  c_DijetMass_ratio_log->SaveAs("c_DijetMass_ratio_log.png");
  c_DijetMass_MI->SaveAs("c_DijetMass_MI.png");
  c_DijetMass_MI_log->SaveAs("c_DijetMass_MI_log.png");
  c_DijetMass_normalized_MI->SaveAs("c_DijetMass_normalized_MI.png");
  c_DijetMass_normalized_MI_log->SaveAs("c_DijetMass_normalized_MI_log.png");
  c_Eta_Phi_Scatter->SaveAs("c_Eta_Phi_Scatter.png");
  c_Eta_Phi_Scatter_1->SaveAs("c_Eta_Phi_Scatter_1.png");
  c_Eta_Phi_Scatter_2->SaveAs("c_Eta_Phi_Scatter_2.png");
  c_MET_over_sumEt->SaveAs("c_MET_over_sumEt.png");
  c_MET_over_sumEt_log->SaveAs("c_MET_over_sumEt_log.png");
  c_EMF->SaveAs("c_EMF.png");
  c_DPhi->SaveAs("c_DPhi.png");
  c_DPhi_log->SaveAs("c_DPhi_log.png");
  c_Eta->SaveAs("c_Eta.png");
  c_Eta_1->SaveAs("c_Eta_1.png");
  c_Eta_2->SaveAs("c_Eta_2.png");
  c_DEta->SaveAs("c_DEta.png");
  c_Phi->SaveAs("c_Phi.png");
  c_Phi_1->SaveAs("c_Phi_1.png");
  c_Phi_2->SaveAs("c_Phi_2.png");
  c_n90hits->SaveAs("c_n90hits.png");
  c_n90hits_log->SaveAs("c_n90hits_log.png");
  c_fHPD->SaveAs("c_fHPD.png");
  c_CorPt->SaveAs("c_CorPt.png");
  c_CorPt_1->SaveAs("c_CorPt_1.png");
  c_CorPt_2->SaveAs("c_CorPt_2.png");
  c_CorPt_log->SaveAs("c_CorPt_log.png");
  c_CorPt_log_1->SaveAs("c_CorPt_log_1.png");
  c_CorPt_log_2->SaveAs("c_CorPt_log_2.png");
  c_nTrkVtx->SaveAs("c_nTrkVtx.png");
  c_nTrkCalo->SaveAs("c_nTrkCalo.png");

  c_DijetMass->SaveAs("c_DijetMass.eps");
  c_DijetMass_log->SaveAs("c_DijetMass_log.eps");
  c_DijetMass_ratio->SaveAs("c_DijetMass_ratio.eps");
  c_DijetMass_ratio_log->SaveAs("c_DijetMass_ratio_log.eps");
  c_DijetMass_MI->SaveAs("c_DijetMass_MI.eps");
  c_DijetMass_MI_log->SaveAs("c_DijetMass_MI_log.eps");
  c_DijetMass_normalized_MI->SaveAs("c_DijetMass_normalized_MI.eps");
  c_DijetMass_normalized_MI_log->SaveAs("c_DijetMass_normalized_MI_log.eps");
  c_Eta_Phi_Scatter->SaveAs("c_Eta_Phi_Scatter.eps");
  c_Eta_Phi_Scatter_1->SaveAs("c_Eta_Phi_Scatter_1.eps");
  c_Eta_Phi_Scatter_2->SaveAs("c_Eta_Phi_Scatter_2.eps");
  c_MET_over_sumEt->SaveAs("c_MET_over_sumEt.eps");
  c_MET_over_sumEt_log->SaveAs("c_MET_over_sumEt_log.eps");
  c_EMF->SaveAs("c_EMF.eps");
  c_DPhi->SaveAs("c_DPhi.eps");
  c_DPhi_log->SaveAs("c_DPhi_log.eps");
  c_Eta->SaveAs("c_Eta.eps");
  c_Eta_1->SaveAs("c_Eta_1.eps");
  c_Eta_2->SaveAs("c_Eta_2.eps");
  c_DEta->SaveAs("c_DEta.eps");
  c_Phi->SaveAs("c_Phi.eps");
  c_Phi_1->SaveAs("c_Phi_1.eps");
  c_Phi_2->SaveAs("c_Phi_2.eps");
  c_n90hits->SaveAs("c_n90hits.eps");
  c_n90hits_log->SaveAs("c_n90hits_log.eps");
  c_fHPD->SaveAs("c_fHPD.eps");
  c_CorPt->SaveAs("c_CorPt.eps");
  c_CorPt_1->SaveAs("c_CorPt_1.eps");
  c_CorPt_2->SaveAs("c_CorPt_2.eps");
  c_CorPt_log->SaveAs("c_CorPt_log.eps");
  c_CorPt_log_1->SaveAs("c_CorPt_log_1.eps");
  c_CorPt_log_2->SaveAs("c_CorPt_log_2.eps");
  c_nTrkVtx->SaveAs("c_nTrkVtx.eps");
  c_nTrkCalo->SaveAs("c_nTrkCalo.eps");

  //hPFCorMjj->Draw();
  outf->Write();
}


typedef struct 
{
  int   passLooseID;
  int   nTrkVtx;
  int   nTrkCalo;
  int   n90;
  int   n90hits;
  float rawPt;
  float corPt;
  float eta;
  float etaD;
  float y;
  float phi;
  float rawE;
  float corE;
  float trkVtxPt;
  float trkCaloPt;
  float fHPD;
  float fRBX;
  float shapeVar;
  float sigmaEta;
  float sigmaPhi;
  float emf;
  float chf;
  float gendR;
  float genPt;
  float genEta;
  float genPhi;
  float corRsp;
  float rawRsp;
} CALOJET_VAR_mc;

typedef struct 
{
  int   runNo;
  int   evtNo;
  int   lumi;
  int   bunch;
  int   njets;
  int   passLooseHcalNoise;
  int   passTightHcalNoise;
  int   nPV;
  int   PVntracks;
  float PVz;
  float PVx;
  float PVy;
  float PVndof;
  float PVchi2;
  float PVnormalizedChi2;
  float corMass;
  float rawMass; 
  float met;
  float metNoHF;
  float sumet;
  float sumetNoHF;
  float met_over_sumet; 
  float met_over_sumet_NoHF;
  float eemf;
  float echf;
  float pthat;
  float weight;
} EVENT_VAR_mc;
