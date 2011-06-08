#include "TMath.h"
TFile *inf_data;
TFile *outf;
string sTmp("/tmp/mgouzevi/tmp");

void ReadTree_JES(char* file, char* outLabel)
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
  outFile = outFile + "" + outLabel + "_JES.root";
  outf = new TFile(outFile.c_str(),"RECREATE");

  sTmp = sTmp + "_" + outLabel + ".root";


  TFile *inf_data  = TFile::Open(file);//"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  
  

 



  TTree *tr_data_large = (TTree*)inf_data->Get("ak7/ProcessedTree");
  //  TTree *tr_data = ntupleR2S->CopyTree();


  // p_T and mass boundaries ---------------------------
  
  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 
  double ptBoundaries[55] = {1, 9, 18, 29, 40, 53, 67, 81, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3500};

  Double_t tAxis[22];
  for (unsigned i = 0; i<22; i++) tAxis[i] = i*1.0-0.5;

  // histogrmas definition ------------------------------


  TH1 *h_Pthat_data = new TH1F("h_Pthat_data","PtHat_data",200, 0, 2000);


  // ---------- Calo jets ---------------------

  TH1 *h_DijetMass_data = new TH1F("h_DijetMass_data","DijetMass_data",83, xAxis1);
  TH1 *h_DijetMass_data_UP = new TH1F("h_DijetMass_data_UP","DijetMass_data_UP",83, xAxis1);
  TH1 *h_DijetMass_data_DW = new TH1F("h_DijetMass_data_DW","DijetMass_data_DW",83, xAxis1);
  TH2D *p_DijetMass_data_UP = new TH2D("p_DijetMass_data_UP","DijetMass_data_JES_UP",83, xAxis1, 100, -0.1, 0.1);
  TH2D *p_DijetMass_data_DW = new TH2D("p_DijetMass_data_DW","DijetMass_data_JES_DW",83, xAxis1, 100, -0.1, 0.1);



  // --------- PF Jets ------------------------
  TH1 *h_DijetMass_data_pf = new TH1F("h_DijetMass_data_pf","DijetMass_data_pf",83, xAxis1);
  TH1 *h_DijetMass_data_UP_pf = new TH1F("h_DijetMass_data_UP_pf","DijetMass_data_UP_pf",83, xAxis1);
  TH1 *h_DijetMass_data_DW_pf = new TH1F("h_DijetMass_data_DW_pf","DijetMass_data_DW_pf",83, xAxis1);
  TH2D *p_DijetMass_data_UP_pf = new TH2D("p_DijetMass_data_UP_pf","DijetMass_data_pf_JES_UP",83, xAxis1, 100, -0.1, 0.1);
  TH2D *p_DijetMass_data_DW_pf = new TH2D("p_DijetMass_data_DW_pf","DijetMass_data_pf_JES_DW",83, xAxis1, 100, -0.1, 0.1);


  // --------- Fat Jets ------------------------
  TH1 *h_DijetMass_data_fat = new TH1F("h_DijetMass_data_fat","DijetMass_data_fat",83, xAxis1); 
  TH1 *h_DijetMass_data_UP_fat = new TH1F("h_DijetMass_data_UP_fat","DijetMass_data_UP_fat",83, xAxis1);
  TH1 *h_DijetMass_data_DW_fat = new TH1F("h_DijetMass_data_DW_fat","DijetMass_data_DW_fat",83, xAxis1);
  TH2D *p_DijetMass_data_UP_fat = new TH2D("p_DijetMass_data_UP_fat","DijetMass_data_fat_JES_UP",83, xAxis1, 100, -0.1, 0.1);
  TH2D *p_DijetMass_data_DW_fat = new TH2D("p_DijetMass_data_DW_fat","DijetMass_data_fat_JES_DW",83, xAxis1, 100, -0.1, 0.1);



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
  
  TCut generalCut = "((fatmjjcor(-1) > 740.0 || fatmjjcor(0) > 740.0 || fatmjjcor(1) > 740.0) &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || ( (pfmjjcor(0) > 838.0 || pfmjjcor(-1) > 838.0 || pfmjjcor(1) > 838.0 )> 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3) || (( calomjjcor(-1) > 788.0 || calomjjcor(0) > 788.0 || calomjjcor(1) > 788.0 ) &&  fabs( (calojet(0)).eta() - (calojet(1)).eta() ) < 1.3)";

  //  TCut generalCut = "(fatmjjcor(0) > 740.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3)";

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

	  
	  bool eta_cuts = fabs((Event_data->calojet(0)).eta()) < 2.5 && fabs((Event_data->calojet(1)).eta()) < 2.5;
	  bool deta_cut = fabs( (Event_data->calojet(0)).eta() - (Event_data->calojet(1)).eta() ) < 1.3;
	  bool corMass_cuts = Event_data->calomjjcor(0) > 788.0;
	  bool corMass_cuts_up = Event_data->calomjjcor(1) > 788.0;
	  bool corMass_cuts_dw = Event_data->calomjjcor(-1) > 788.0;
	  bool jetID_cuts = (Event_data->calojet(0)).looseID() == true && (Event_data->calojet(1)).looseID() == true;
	  

	  bool pf_eta_cuts = fabs((Event_data->pfjet(0)).eta()) < 2.5 && fabs((Event_data->pfjet(1)).eta()) < 2.5;
	  bool pf_deta_cut = fabs( (Event_data->pfjet(0)).eta() - (Event_data->pfjet(1)).eta() ) < 1.3;
	  bool pf_corMass_cuts = Event_data->pfmjjcor(0) > 838.0;
	  bool pf_corMass_cuts_up = Event_data->pfmjjcor(1) > 838.0;
	  bool pf_corMass_cuts_dw = Event_data->pfmjjcor(-1) > 838.0;

	  bool pf_jetID_cuts = (Event_data->pfjet(0)).looseID() == true && (Event_data->pfjet(1)).looseID() == true;

	  bool fat_eta_cuts = fabs((Event_data->fatjet(0)).eta()) < 2.5 && fabs((Event_data->fatjet(1)).eta()) < 2.5;
	  bool fat_deta_cut = fabs( (Event_data->fatjet(0)).eta() - (Event_data->fatjet(1)).eta() ) < 1.3;
	  bool fat_corMass_cuts = Event_data->fatmjjcor(0) > 740; //838.0;
	  bool fat_corMass_cuts_up = Event_data->fatmjjcor(1) > 740; //838.0;
	  bool fat_corMass_cuts_dw = Event_data->fatmjjcor(-1) > 740; //838.0;
	  bool fat_jetID_cuts = (Event_data->fatjet(0)).looseID() == true && (Event_data->fatjet(1)).looseID() == true;


	  
	  // Calo Jets
          if(eta_cuts) {
            if (deta_cut) {
              eta_count++;
	      if(jetID_cuts) {
		jetID_count++;
           
		if(corMass_cuts) {
		  h_DijetMass_data->Fill(Event_data->calomjjcor(0));
		  p_DijetMass_data_UP->Fill(Event_data->calomjjcor(0), (Event_data->calomjjcor(1)-Event_data->calomjjcor(0))/Event_data->calomjjcor(0));
		  p_DijetMass_data_DW->Fill(Event_data->calomjjcor(0), (Event_data->calomjjcor(-1)-Event_data->calomjjcor(0))/Event_data->calomjjcor(0));
		}
		if(corMass_cuts_up)   h_DijetMass_data_UP->Fill(Event_data->calomjjcor(1));
		if(corMass_cuts_dw)   h_DijetMass_data_DW->Fill(Event_data->calomjjcor(-1));
                		  

              }
            }
          }
	  


	  //PF jets
          if(pf_eta_cuts) {
            if (pf_deta_cut) {
              pf_eta_count++;
                if(pf_jetID_cuts) {
                  pf_jetID_count++;
		  if(pf_corMass_cuts) {
		    h_DijetMass_data_pf->Fill(Event_data->pfmjjcor(0));
		    p_DijetMass_data_UP_pf->Fill(Event_data->pfmjjcor(0), (Event_data->pfmjjcor(1)-Event_data->pfmjjcor(0))/Event_data->pfmjjcor(0));
		    p_DijetMass_data_DW_pf->Fill(Event_data->pfmjjcor(0), (Event_data->pfmjjcor(-1)-Event_data->pfmjjcor(0))/Event_data->pfmjjcor(0));
		  }
		  if(pf_corMass_cuts_up)  h_DijetMass_data_UP_pf->Fill(Event_data->pfmjjcor(1));
		  if(pf_corMass_cuts_dw)  h_DijetMass_data_DW_pf->Fill(Event_data->pfmjjcor(-1));
		}

            }
           }

	  if(fat_eta_cuts) {
            if (fat_deta_cut) {
              fat_eta_count++;
                if(fat_jetID_cuts) {
                  fat_jetID_count++;
		  if(fat_corMass_cuts) {
		    h_DijetMass_data_fat->Fill(Event_data->fatmjjcor(0));
		    p_DijetMass_data_UP_fat->Fill(Event_data->fatmjjcor(0), (Event_data->fatmjjcor(1)-Event_data->fatmjjcor(0))/Event_data->fatmjjcor(0));
		    p_DijetMass_data_DW_fat->Fill(Event_data->fatmjjcor(0), (Event_data->fatmjjcor(-1)-Event_data->fatmjjcor(0))/Event_data->fatmjjcor(0));
		  }
		  if(fat_corMass_cuts_up) h_DijetMass_data_UP_fat->Fill(Event_data->fatmjjcor(1));
		  if(fat_corMass_cuts_dw) h_DijetMass_data_DW_fat->Fill(Event_data->fatmjjcor(-1));

	      }
	    }
	  }




        }
      }
    }

  }






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
  h_DijetMass_data_UP->Write();
  h_DijetMass_data_DW->Write();
  p_DijetMass_data_UP->Write();
  p_DijetMass_data_DW->Write();
  // --------- PF --------

  h_DijetMass_data_pf->Write();
  h_DijetMass_data_UP_pf->Write();
  h_DijetMass_data_DW_pf->Write();
  p_DijetMass_data_UP_pf->Write();
  p_DijetMass_data_DW_pf->Write();

 // --------- Fat -------

  h_DijetMass_data_fat->Write();
  h_DijetMass_data_UP_fat->Write();
  h_DijetMass_data_DW_fat->Write();
  p_DijetMass_data_UP_fat->Write();
  p_DijetMass_data_DW_fat->Write();

  cout << "Written" << endl;

  //  delete branch_data;
  // delete tr_data;
  //  delete Event_data;

  outf->Close();
  //  inf_data->Close();


}

