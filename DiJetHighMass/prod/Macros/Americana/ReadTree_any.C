
#include "TMath.h"
TFile *inf_data;
TFile *outf;
string sTmp("/tmp/mgouzevi/tmp");

void ReadTree_any(char* file, char* outLabel)
{
  // style definition -------------------------------

  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  //  TFile *inf_data  = TFile::Open("rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  //  TFile *outf = new TFile("histogrmas_data_all.root","RECREATE");
  string outFile("histograms_");
  outFile = outFile + "" + outLabel + ".root";
  outf = new TFile(outFile.c_str(),"RECREATE");

  sTmp = sTmp + "_" + outLabel + ".root";


  TFile *inf_data  = TFile::Open(file);//"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  
  

 



  TTree *tr_data_large = (TTree*)inf_data->Get("ak7/ProcessedTree");
  //  TTree *tr_data = ntupleR2S->CopyTree();


  // p_T and mass boundaries ---------------------------
  
  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 
  double ptBoundaries[55] = {1, 9, 18, 29, 40, 53, 67, 81, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3500};

  Double_t tAxis[32];
  for (unsigned i = 0; i<32; i++) tAxis[i] = i*1.0-0.5;

  // histogrmas definition ------------------------------


  TH1 *h_Pthat_data = new TH1F("h_Pthat_data","PtHat_data",200, 0, 2000);


  // ---------- Calo jets ---------------------

  TH1 *h_DijetMass_data = new TH1F("h_DijetMass_data","DijetMass_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVe1_data = new TH1F("h_DijetMass_MI_nPVe1_data","DijetMass_MI_nPVe1_data",83, xAxis1);
  TH1 *h_DijetMass_MI_nPVg1_data = new TH1F("h_DijetMass_MI_nPVg1_data","DijetMass_MI_nPVg1_data",83, xAxis1);
  TH2 *h_Eta_Phi_Scatter_data = new TH2F("h_Eta_Phi_Scatter","Eta_Phi_Scatter",100,-3,3,100,-3.2,3.2);
  TH2 *h_Eta_Phi_Scatter_data_1 = new TH2F("h_Eta_Phi_Scatter_1","Eta_Phi_Scatter_1",100,-3,3,100,-3.2,3.2);
  TH2 *h_Eta_Phi_Scatter_data_2 = new TH2F("h_Eta_Phi_Scatter_2","Eta_Phi_Scatter_2",100,-3,3,100,-3.2,3.2);
  TH1 *h_MET_over_sumEt_data = new TH1F("h_MET_over_sumEt_data","MET_over_sumEt_data",25,0.0,1.0001);
  TH1 *h_EMF_data = new TH1F("h_EMF_data","EMF_data",25,0.0,1.0001);
  TH1 *h_DPhi_data = new TH1F("h_DPhi_data","DPhi_data",25,0.0,3.142);
  TH1 *h_Eta_data = new TH1F("h_Eta_data","Eta_data",60,-3.0,3.0);
  TH1 *h_Eta_data_1 = new TH1F("h_Eta_data_1","Eta_data_1",60,-3.0,3.0);
  TH1 *h_Eta_data_2 = new TH1F("h_Eta_data_2","Eta_data_2",60,-3.0,3.0);

  TH1 *h_DEta_data = new TH1F("h_DEta_data","DEta_data",15,0.,1.5);
  TH1 *h_Phi_data = new TH1F("h_Phi_data","Phi_data",18,-3.142,3.142);
  TH1 *h_Phi_data_1 = new TH1F("h_Phi_data_1","Phi_data_1",18,-3.142,3.142);
  TH1 *h_Phi_data_2 = new TH1F("h_Phi_data_2","Phi_data_2",18,-3.142,3.142);

  TH1 *h_n90hits_data = new TH1F("h_n90hits_data","n90hits_data",400,-0.5,399.5);
  TH1 *h_fHPD_data = new TH1F("h_fHPD_data","fHPD_data",25,0.0,1.0001);
  TH1 *h_corPt_data = new TH1F("h_corPt_data","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_1 = new TH1F("h_corPt_data_1","corPt_data_1",54,ptBoundaries);
  TH1 *h_corPt_data_2 = new TH1F("h_corPt_data_2","corPt_data_2",54,ptBoundaries);
  TH1 *h_nTrkVtx_data = new TH1F("h_nTrkVtx_data","nTrkVtx_data",900,-0.5,899.5);
  TH1 *h_nTrkCalo_data = new TH1F("h_nTrkCalo_data","nTrkCalo_data",100,-0.5,99.5);




  // --------- PF Jets ------------------------
  TH1 *h_DijetMass_data_pf = new TH1F("h_DijetMass_data_pf","DijetMass_data_pf",83, xAxis1);
  TH1 *h_DEta_data_pf = new TH1F("h_DEta_data_pf","DEta_data_pf",15,0.,1.5);
  TH1 *h_DPhi_data_pf = new TH1F("h_DPhi_data_pf","DPhi_data_pf",25,0.0,3.142);
  TH1 *h_Eta_data_pf = new TH1F("h_Eta_data_pf","Eta_data_pf",60,-3.0,3.0);
  TH1 *h_Eta_data_pf_1 = new TH1F("h_Eta_data_pf_1","Eta_data_pf_1",60,-3.0,3.0);
  TH1 *h_Eta_data_pf_2 = new TH1F("h_Eta_data_pf_2","Eta_data_pf_2",60,-3.0,3.0);
  TH1 *h_Phi_data_pf = new TH1F("h_Phi_data_pf","Phi_data_pf",18,-3.142,3.142);
  TH1 *h_Phi_data_pf_1 = new TH1F("h_Phi_data_pf_1","Phi_data_pf_1",18,-3.142,3.142);
  TH1 *h_Phi_data_pf_2 = new TH1F("h_Phi_data_pf_2","Phi_data_pf_2",18,-3.142,3.142);
  TH1 *h_corPt_data_pf = new TH1F("h_corPt_data_pf","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_pf_1 = new TH1F("h_corPt_data_pf_1","corPt_data_pf_1",54,ptBoundaries);
  TH1 *h_corPt_data_pf_2 = new TH1F("h_corPt_data_pf_2","corPt_data_pf_2",54,ptBoundaries);

  TProfile2D* p_RvsEta_data_pf = new TProfile2D("p_RvsEta_data_pf", ";#eta;fraction", 12, -3, 3, 4, -0.5, 3.5);
  TProfile2D* p_RvsPt_barrel_data_pf = new TProfile2D("p_RvsPt_barrel_data_pf", "Barrel;p_{T} (GeV/c);fraction", 50, 0, 200, 4, -0.5, 3.5 );
  TProfile2D* p_RvsPt_endcaps_data_pf   = new TProfile2D("p_RvsPt_endcaps_data_pf", "Endcaps;p_{T} (GeV/c);fraction", 50, 0, 200, 4, -0.5, 3.5 );
  TProfile2D* p_RvsPhi_barrel_data_pf   = new TProfile2D("p_RvsPhi_barrel_data_pf", "Barrel;#phi;fraction", 50, -3.2, 3.2, 4, -0.5, 3.5 );
  TProfile2D* p_RvsPhi_endcaps_data_pf   = new TProfile2D("p_RvsPhi_endcaps_data_pf", "Endcaps;#phi;fraction", 50, -3.2, 3.2, 4, -0.5, 3.5 );

  TH1 *h_fCh_data_pf = new TH1F("h_fCh_data_pf","Charged Fraction",25,0.0,1.0001);
  TH1 *h_fNh_data_pf = new TH1F("h_fNh_data_pf","Neutral Hadron Fraction",25,0.0,1.0001);
  TH1 *h_fPh_data_pf = new TH1F("h_fPh_data_pf","Photon Fraction",25,0.0,1.0001);
  TH1 *h_fEl_data_pf = new TH1F("h_fEl_data_pf","Electrons Fraction",25,0.0,1.0001);

  TH1 *h_fCh_data_pf_1 = new TH1F("h_fCh_data_pf_1","Charged Fraction jet1",25,0.0,1.0001);
  TH1 *h_fNh_data_pf_1 = new TH1F("h_fNh_data_pf_1","Neutral Hadron Fraction jet1",25,0.0,1.0001);
  TH1 *h_fPh_data_pf_1 = new TH1F("h_fPh_data_pf_1","Photon Fraction jet1",25,0.0,1.0001);
  TH1 *h_fEl_data_pf_1 = new TH1F("h_fEl_data_pf_1","Electrons Fraction jet1",25,0.0,1.0001);

  TH1 *h_fCh_data_pf_2 = new TH1F("h_fCh_data_pf_2","Charged Fraction jet1",25,0.0,1.0001);
  TH1 *h_fNh_data_pf_2 = new TH1F("h_fNh_data_pf_2","Neutral Hadron Fraction jet1",25,0.0,1.0001);
  TH1 *h_fPh_data_pf_2 = new TH1F("h_fPh_data_pf_2","Photon Fraction jet1",25,0.0,1.0001);
  TH1 *h_fEl_data_pf_2 = new TH1F("h_fEl_data_pf_2","Electrons Fraction jet1",25,0.0,1.0001);


  // --------- Fat Jets ------------------------

  TH1 *h_MET_over_sumEt_fat_data = new TH1F("h_MET_over_sumEt_fat_data","MET_over_sumEt_fat_data",25,0.0,1.0001);

  TH1 *h_DijetMass_data_fat = new TH1F("h_DijetMass_data_fat","DijetMass_data_fat",83, xAxis1); 
  TH2 *h_DijetMass_Trigger_data_fat = new TH2F("h_DijetMass_Trigger_data_fat","DijetMass_Trigger_data_fat",83, xAxis1, 31, -0.5, 30.5); 
  TH3 *h_DijetMass_Trigger_data_fat_Corr = new TH3F("h_DijetMass_Trigger_data_fat_Corr","DijetMass_Trigger_data_fat",83, xAxis1, 31, tAxis, 31, tAxis); 


  TH1 *h_DEta_data_fat = new TH1F("h_DEta_data_fat","DEta_data_fat",15,0.,1.5);
  TH1 *h_DPhi_data_fat = new TH1F("h_DPhi_data_fat","DPhi_data_fat",25,0.0,3.142);
  TH1 *h_Eta_data_fat = new TH1F("h_Eta_data_fat","Eta_data_fat",60,-3.0,3.0);
  TH1 *h_Eta_data_fat_1 = new TH1F("h_Eta_data_fat_1","Eta_data_fat_1",60,-3.0,3.0);
  TH1 *h_Eta_data_fat_2 = new TH1F("h_Eta_data_fat_2","Eta_data_fat_2",60,-3.0,3.0);
  TH1 *h_Phi_data_fat = new TH1F("h_Phi_data_fat","Phi_data_fat",18,-3.142,3.142);
  TH1 *h_Phi_data_fat_1 = new TH1F("h_Phi_data_fat_1","Phi_data_fat_1",18,-3.142,3.142);
  TH1 *h_Phi_data_fat_2 = new TH1F("h_Phi_data_fat_2","Phi_data_fat_2",18,-3.142,3.142);
  TH1 *h_corPt_data_fat = new TH1F("h_corPt_data_fat","corPt_data",54,ptBoundaries);
  TH1 *h_corPt_data_fat_1 = new TH1F("h_corPt_data_fat_1","corPt_data_fat_1",54,ptBoundaries);
  TH1 *h_corPt_data_fat_2 = new TH1F("h_corPt_data_fat_2","corPt_data_fat_2",54,ptBoundaries);

  TH1 *h_FatDiJet_EventYield = new TH1F("h_FatDiJet_EventYield","h_FatDiJet_EventYield",6500,160000.5,166500.5);
  TH1 *h_PFDiJet_EventYield = new TH1F("h_PFDiJet_EventYield","h_PFDiJet_EventYield",6500,160000.5,166500.5);

  TH1 *h_Cuts_Fat = new TH1F("h_Cuts_Fat","h_Cuts_Fat",5, -0.5, 4.5);
  TH1 *h_Cuts_Fat_Names = new TH1F("h_Cuts_Fat_Names","h_Cuts_Fat_Names",1, 0, 1);

  h_Cuts_Fat_Names->SetBit(TH1::kCanRebin);
  string cutNames[5]={string("data_count"), string("good_vertex_selection"), string("DiFatJet_MassCut"), string("DiFatJet_EtaCuts"), string("LeadingJets_Id")};
  for(unsigned i=0;i<5;i++){
    cout << cutNames[i].c_str() << endl;
    h_Cuts_Fat_Names->Fill(cutNames[i].c_str(),1);
  }


  // data access -------------------------------------------

  QCDEvent *Event_data = new QCDEvent();
  TBranch *branch_data = tr_data_large->GetBranch("events");
  branch_data->SetAddress(&Event_data);

  // # of entries -------------------------------------------

  unsigned NEntries_data = tr_data_large->GetEntries();
  cout<<"Reading TREE: "<<NEntries_data <<" events"<<endl;
  int decade = 0;



  TFile* temp = new TFile(sTmp.c_str(),"RECREATE");
  temp->cd();
  
  //  TCut generalCut = "(fatmjjcor(0) > 838.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3) || (calomjjcor(0) > 788.0 &&  fabs( (calojet(0)).eta() - (calojet(1)).eta() ) < 1.3)";

  TCut generalCut = "";//"(fatmjjcor(0) > 740.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3)";

  TTree* tr_data = tr_data_large->CopyTree(generalCut);

  unsigned NEntries_data_SKIMED = tr_data->GetEntries();
  cout<<"Reading TREE: "<< NEntries_data_SKIMED <<" events"<<endl;

  // counters------------------------------------------------

  unsigned data_count(0);
  unsigned vertex1_count(0);
  unsigned vertex2_count(0);
  unsigned runNo_count(0);

  unsigned mass_count(0);
  unsigned eta_count(0);
  unsigned jetID_count(0);

  unsigned pf_mass_count(0);
  unsigned pf_eta_count(0);
  unsigned pf_jetID_count(0);

  unsigned fat_mass_count(0);
  unsigned fat_eta_count(0);
  unsigned fat_jetID_count(0);


  unsigned mc_count(0);


  // fill histograms-----------------------------------------

  for(unsigned i=0; i < NEntries_data_SKIMED;i++) {
    
    if (i%1000 == 0) {
      cout << "Is " << i << " mass is " <<  Event_data->fatmjjcor(0) << endl;
      
    }


    double progress = 100.0*i/(1.0*NEntries_data);
    int k = TMath::FloorNint(progress); 


    if (k > decade) 
      cout<<100*k<<" %"<<endl;

    decade = k;          
    
    tr_data->GetEntry(i);

    h_Pthat_data->Fill((Event_data->evtHdr()).pthat());

//    continue;
    /*
    bool light_cut0 =  Event_data->fatmjjcor(0) > 838.0;
    if (!light_cut0) continue;
    bool light_cut1 =  fabs( (Event_data->fatjet(0)).eta() - (Event_data->fatjet(1)).eta() ) < 1.3;
    if (!light_cut1) continue;
    */

    int nVtx = (Event_data->evtHdr()).nVtx();

    bool vertex_cut1 = (Event_data->evtHdr()).PVndof() >= 4;
    bool vertex_cut2 = (Event_data->evtHdr()).PVz() < 24 ;

//    bool jetID_cuts = ((((Event_data->calojet(0)).emf() > 0.01) || (fabs((Event_data->calojet(0)).eta())) > 2.6) && ((Event_data->calojet(0)).n90hits() > 1) && ((Event_data->calojet(0)).fHPD() < 0.98)  ) && ((((Event_data->calojet(1)).emf() > 0.01) || (fabs((Event_data->calojet(1)).eta())) > 2.6) && ((Event_data->calojet(1)).n90hits() > 1) && ((Event_data->calojet(1)).fHPD() < 0.98 ) );
    bool runNo_cuts = true;
    //    bool runNo_cuts = (Event_data->evtHdr()).runNo() == 163078;

 


    data_count++; 
    if (runNo_cuts) {
      runNo_count++;
      if(vertex_cut1) {
        vertex1_count++;
        if(vertex_cut2) {
          vertex2_count++;

	  /*
	  bool eta_cuts = fabs((Event_data->calojet(0)).eta()) < 2.5 && fabs((Event_data->calojet(1)).eta()) < 2.5;
	  bool deta_cut = fabs( (Event_data->calojet(0)).eta() - (Event_data->calojet(1)).eta() ) < 1.3;
	  bool corMass_cuts = Event_data->calomjjcor(0) > 788.0;
	  bool jetID_cuts = (Event_data->calojet(0)).looseID() == true && (Event_data->calojet(1)).looseID() == true;
	  */

	  double dPhi_fat = (TMath::Pi()-fabs(fabs((Event_data->fatjet(0)).phi()-(Event_data->fatjet(1)).phi())-TMath::Pi()));

	  bool pf_eta_cuts = fabs((Event_data->pfjet(0)).eta()) < 2.5 && fabs((Event_data->pfjet(1)).eta()) < 2.5;
	  bool pf_deta_cut = fabs( (Event_data->pfjet(0)).eta() - (Event_data->pfjet(1)).eta() ) < 1.3;
	  bool pf_corMass_cuts = Event_data->pfmjjcor(0) > 838.0;
	  bool pf_jetID_cuts = (Event_data->pfjet(0)).looseID() == true && (Event_data->pfjet(1)).looseID() == true;

	  bool fat_eta_cuts = fabs((Event_data->fatjet(0)).eta()) < 2.5 && fabs((Event_data->fatjet(1)).eta()) < 2.5;
	  bool fat_deta_cut = fabs( (Event_data->fatjet(0)).eta() - (Event_data->fatjet(1)).eta() ) < 1.3;
	  bool fat_corMass_cuts = Event_data->fatmjjcor(0) > 740.0;
	  bool fat_jetID_cuts = (Event_data->fatjet(0)).looseID() == true && (Event_data->fatjet(1)).looseID() == true;
	  bool fat_dPhi_cuts = dPhi_fat > TMath::Pi()/3;

	  /*
	  // Calo Jets
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
                  h_Eta_data_1->Fill((Event_data->calojet(0)).eta());
                  h_Eta_data_2->Fill((Event_data->calojet(1)).eta());
                  h_DEta_data->Fill(fabs((Event_data->calojet(0)).eta()-(Event_data->calojet(1)).eta()));
                  h_Phi_data_1->Fill((Event_data->calojet(0)).phi());
                  h_Phi_data_2->Fill((Event_data->calojet(1)).phi());
                  h_n90hits_data->Fill((Event_data->calojet(0)).n90hits());
                  h_n90hits_data->Fill((Event_data->calojet(1)).n90hits());
                  h_fHPD_data->Fill((Event_data->calojet(0)).fHPD());
                  h_fHPD_data->Fill((Event_data->calojet(1)).fHPD());
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
	  */


	  //PF jets
          if(pf_eta_cuts) {
            if (pf_deta_cut) {
              pf_eta_count++;
              if(pf_corMass_cuts) {
                pf_mass_count++;
                if(pf_jetID_cuts) {
                  pf_jetID_count++;
                  h_DijetMass_data_pf->Fill(Event_data->pfmjjcor(0));
                  h_DPhi_data_pf->Fill(3.14159265358979323846-fabs(fabs((Event_data->pfjet(0)).phi()-(Event_data->pfjet(1)).phi())-3.14159265358979323846));
                  h_Eta_data_pf_1->Fill((Event_data->pfjet(0)).eta());
                  h_Eta_data_pf_2->Fill((Event_data->pfjet(1)).eta());
                  h_DEta_data_pf->Fill(fabs((Event_data->pfjet(0)).eta()-(Event_data->pfjet(1)).eta()));
                  h_Phi_data_pf_1->Fill((Event_data->pfjet(0)).phi());
                  h_Phi_data_pf_2->Fill((Event_data->pfjet(1)).phi());
                  h_corPt_data_pf_1->Fill((Event_data->pfjet(0)).ptCor());
                  h_corPt_data_pf_2->Fill((Event_data->pfjet(1)).ptCor()); 
		  h_PFDiJet_EventYield->Fill((Event_data->evtHdr()).runNo());

		  for (int iCh = 0; iCh < 4; iCh++){
	
		    double frac = 0;
		    switch(iCh){
		    case 0: frac = Event_data->pfjet(0).chf(); break;//p_RvsEta->Fill(
		    case 1: frac = Event_data->pfjet(0).nhf(); break;//p_RvsEta->Fill(
		    case 2: frac = Event_data->pfjet(0).phf(); break;//p_RvsEta->Fill(
		    case 3: frac = Event_data->pfjet(0).elf(); break;//p_RvsEta->Fill(
		    default: break;
		    }
		    
		    p_RvsEta_data_pf->Fill(Event_data->pfjet(0).eta(), iCh, frac);
		    p_RvsPt_barrel_data_pf->Fill(Event_data->pfjet(0).ptCor(), iCh, frac);
		    p_RvsPt_endcaps_data_pf->Fill(Event_data->pfjet(0).ptCor(), iCh, frac);
		    p_RvsPhi_barrel_data_pf->Fill(Event_data->pfjet(0).phi(), iCh, frac);
		    p_RvsPhi_endcaps_data_pf->Fill(Event_data->pfjet(0).phi(), iCh, frac);

		    p_RvsEta_data_pf->Fill(Event_data->pfjet(1).eta(), iCh, frac);
		    p_RvsPt_barrel_data_pf->Fill(Event_data->pfjet(1).ptCor(), iCh, frac);
		    p_RvsPt_endcaps_data_pf->Fill(Event_data->pfjet(1).ptCor(), iCh, frac);
		    p_RvsPhi_barrel_data_pf->Fill(Event_data->pfjet(1).phi(), iCh, frac);
		    p_RvsPhi_endcaps_data_pf->Fill(Event_data->pfjet(1).phi(), iCh, frac);
		  }

		  h_fCh_data_pf_1->Fill((Event_data->pfjet(0)).chf());
		  h_fNh_data_pf_1->Fill((Event_data->pfjet(0)).nhf());
		  h_fPh_data_pf_1->Fill((Event_data->pfjet(0)).phf());
		  h_fEl_data_pf_1->Fill((Event_data->pfjet(0)).elf());

		  h_fCh_data_pf_2->Fill((Event_data->pfjet(1)).chf());
		  h_fNh_data_pf_2->Fill((Event_data->pfjet(1)).nhf());
		  h_fPh_data_pf_2->Fill((Event_data->pfjet(1)).phf());
		  h_fEl_data_pf_2->Fill((Event_data->pfjet(1)).elf());

		}
              }
            }
           }

	 
	  if(fat_corMass_cuts) {
	      fat_mass_count++;
	      if(fat_eta_cuts) {
		if (fat_deta_cut) {
		  fat_eta_count++;
		  if(fat_jetID_cuts && fat_dPhi_cuts) {
		    h_MET_over_sumEt_fat_data->Fill((Event_data->pfmet()).met_o_sumet());
                     
		    fat_jetID_count++;
		    h_DijetMass_data_fat->Fill(Event_data->fatmjjcor(0));
		    h_DPhi_data_fat->Fill(dPhi_fat);
		    h_Eta_data_fat_1->Fill((Event_data->fatjet(0)).eta());
		    h_Eta_data_fat_2->Fill((Event_data->fatjet(1)).eta());
		    h_DEta_data_fat->Fill(fabs((Event_data->fatjet(0)).eta()-(Event_data->fatjet(1)).eta()));
		    h_Phi_data_fat_1->Fill((Event_data->fatjet(0)).phi());
		    h_Phi_data_fat_2->Fill((Event_data->fatjet(1)).phi());
		    h_corPt_data_fat_1->Fill((Event_data->fatjet(0)).ptCor());
		    h_corPt_data_fat_2->Fill((Event_data->fatjet(1)).ptCor()); 
		    h_FatDiJet_EventYield->Fill((Event_data->evtHdr()).runNo());

		  
		    for(unsigned itrig=0;itrig<Event_data->nTriggers();itrig++){
		      if (Event_data->fired(itrig) > 0)
			  h_DijetMass_Trigger_data_fat->Fill(Event_data->fatmjjcor(0), itrig);
		    
			for(unsigned jtrig=0;jtrig<Event_data->nTriggers();jtrig++){

			      if (Event_data->fired(itrig) > 0 && Event_data->fired(jtrig) > 0)
				  h_DijetMass_Trigger_data_fat_Corr->Fill(Event_data->fatmjjcor(0), itrig, jtrig);
      			}
		   }
		  
		}
	      }
	    }
	  }




        }
      }
    }

  }

  h_Cuts_Fat->SetBinContent(1, data_count);
  h_Cuts_Fat->SetBinContent(2, vertex2_count);
  h_Cuts_Fat->SetBinContent(3, fat_mass_count);
  h_Cuts_Fat->SetBinContent(4, fat_eta_count);
  h_Cuts_Fat->SetBinContent(5, fat_jetID_count);



  // ---------- Calo ---------
  h_Eta_data->Add(h_Eta_data_1);
  h_Eta_data->Add(h_Eta_data_2);
  h_Phi_data->Add(h_Phi_data_1);
  h_Phi_data->Add(h_Phi_data_2);
  h_corPt_data->Add(h_corPt_data_1);
  h_corPt_data->Add(h_corPt_data_2);

  // ----------  PF  ---------
  h_Eta_data_pf->Add(h_Eta_data_pf_1);
  h_Eta_data_pf->Add(h_Eta_data_pf_2);
  h_Phi_data_pf->Add(h_Phi_data_pf_1);
  h_Phi_data_pf->Add(h_Phi_data_pf_2);
  h_corPt_data_pf->Add(h_corPt_data_pf_1);
  h_corPt_data_pf->Add(h_corPt_data_pf_2);
 
  h_fCh_data_pf->Add(h_fCh_data_pf_1);
  h_fCh_data_pf->Add(h_fCh_data_pf_2);
  h_fNh_data_pf->Add(h_fNh_data_pf_1);
  h_fNh_data_pf->Add(h_fNh_data_pf_2);
  h_fPh_data_pf->Add(h_fPh_data_pf_1);
  h_fPh_data_pf->Add(h_fPh_data_pf_2);
  h_fEl_data_pf->Add(h_fEl_data_pf_1);
  h_fEl_data_pf->Add(h_fEl_data_pf_2);

  // ----------  FAT  ---------
  h_Eta_data_fat->Add(h_Eta_data_fat_1);
  h_Eta_data_fat->Add(h_Eta_data_fat_2);
  h_Phi_data_fat->Add(h_Phi_data_fat_1);
  h_Phi_data_fat->Add(h_Phi_data_fat_2);
  h_corPt_data_fat->Add(h_corPt_data_fat_1);
  h_corPt_data_fat->Add(h_corPt_data_fat_2);
 





  std::cout << "# of events = " << data_count << std::endl;
  std::cout << "# of events after runNo cut = " << runNo_count << std::endl;
  std::cout << "# of events after vertex1 cut = " << vertex1_count << std::endl;
  std::cout << "# of events after vertex2 cut = " << vertex2_count << std::endl;
  std::cout << "# of events after eta cut(calo) = " << eta_count << std::endl;
  std::cout << "# of events after mass cut(calo) = " << mass_count << std::endl;
  std::cout << "# of events after all cut(calo) = " << jetID_count << std::endl;

  std::cout << "# of events after eta cut(pf) = " << pf_eta_count << std::endl;
  std::cout << "# of events after mass cut(pf) = " << pf_mass_count << std::endl;
  std::cout << "# of events after all cut(pf) = " << pf_jetID_count << std::endl;

  std::cout << "# of events after eta cut(fat) = " << fat_eta_count << std::endl;
  std::cout << "# of events after mass cut(fat) = " << fat_mass_count << std::endl;
  std::cout << "# of events after all cut(fat) = " << fat_jetID_count << std::endl;

  outf->cd();


  // -------- GEN INFO -------

  h_Pthat_data->Write();

  
  // --------- Calo --------

  //  h_DijetMass_data->Write();

  
  h_DijetMass_data->Write();
  
  h_DijetMass_MI_nPVe1_data->Write();
  h_DijetMass_MI_nPVg1_data->Write();
  h_Eta_Phi_Scatter_data->Write();
  h_Eta_Phi_Scatter_data_1->Write();
  h_Eta_Phi_Scatter_data_2->Write();
  h_MET_over_sumEt_data->Write();
  h_EMF_data->Write();
  h_DPhi_data->Write();
  h_Eta_data->Write();
  h_Eta_data_1->Write();
  h_Eta_data_2->Write();
  h_DEta_data->Write();
  h_Phi_data->Write();
  h_Phi_data_1->Write();
  h_Phi_data_2->Write();
  h_n90hits_data->Write();
  h_fHPD_data->Write();
  h_corPt_data->Write();
  h_corPt_data_1->Write();
  h_corPt_data_2->Write();
  h_nTrkVtx_data->Write();
  h_nTrkCalo_data->Write();
  h_nTrkVtx_data->Write();


  // --------- PF --------

  h_DijetMass_data_pf->Write();
  h_DEta_data_pf->Write();
  h_DPhi_data_pf->Write();

  h_Phi_data_pf->Write();
  h_Phi_data_pf_1->Write();
  h_Phi_data_pf_2->Write();

  h_Eta_data_pf->Write();
  h_Eta_data_pf_1->Write();
  h_Eta_data_pf_2->Write();
  h_corPt_data_pf->Write();
  h_corPt_data_pf_1->Write();
  h_corPt_data_pf_2->Write();

  h_fCh_data_pf->Write();
  h_fNh_data_pf->Write();
  h_fPh_data_pf->Write();
  h_fEl_data_pf->Write();
  h_fCh_data_pf_1->Write();
  h_fNh_data_pf_1->Write();
  h_fPh_data_pf_1->Write();
  h_fEl_data_pf_1->Write();
  h_fCh_data_pf_2->Write();
  h_fNh_data_pf_2->Write();
  h_fPh_data_pf_2->Write();
  h_fEl_data_pf_2->Write();

  p_RvsEta_data_pf->Write();
  p_RvsPt_barrel_data_pf->Write();
  p_RvsPt_endcaps_data_pf->Write();
  p_RvsPhi_barrel_data_pf->Write();
  p_RvsPhi_endcaps_data_pf->Write();
  h_PFDiJet_EventYield->Write();

 // --------- Fat -------

  h_DijetMass_Trigger_data_fat->Write();
  h_DijetMass_Trigger_data_fat_Corr->Write();

  h_DijetMass_data_fat->Write();
  h_DEta_data_fat->Write();
  h_DPhi_data_fat->Write();

  h_Phi_data_fat->Write();
  h_Phi_data_fat_1->Write();
  h_Phi_data_fat_2->Write();

  h_Eta_data_fat->Write();
  h_Eta_data_fat_1->Write();
  h_Eta_data_fat_2->Write();
  h_corPt_data_fat->Write();
  h_corPt_data_fat_1->Write();
  h_corPt_data_fat_2->Write();
  
  h_FatDiJet_EventYield->Write();
  h_Cuts_Fat->Write();
  h_Cuts_Fat_Names->Write();
  h_MET_over_sumEt_fat_data->Write();

  cout << "Written" << endl;

  //  delete branch_data;
  // delete tr_data;
  //  delete Event_data;

  outf->Close();
  //  inf_data->Close();


}

