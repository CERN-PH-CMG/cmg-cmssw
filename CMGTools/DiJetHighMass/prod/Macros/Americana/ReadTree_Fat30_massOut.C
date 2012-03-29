
#include "TMath.h"
TFile *inf_data;
TFile *outf;
string sTmp("/tmp/mgouzevi/tmp");
string sChannel("");

double M_PI = TMath::Pi();

ofstream outFileTxt_fat;
ofstream outFileTxt_MuPt10_fat;



void ReadTree_Fat30_massOut(char* file, char* outLabel, char* c_channel)
{
  // style definition -------------------------------
  gROOT->ProcessLine(".L setDefaultStyle.C");
  gROOT->ProcessLine("setDefaultStyle()");
  gROOT->ProcessLine("#include <vector>");
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  string channel(c_channel);

  //  TFile *inf_data  = TFile::Open("rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  //  TFile *outf = new TFile("histogrmas_data_all.root","RECREATE");
  string outFile("histograms_");
  outFile = outFile + "" + outLabel + "_" + channel + ".root";
  sTmp = sTmp + "_" + outLabel + "_" + channel + ".root";
  sChannel = sChannel + "" + channel + "/ProcessedTree";


  string nameFileTxt_fat("massFat_");
  nameFileTxt_fat = nameFileTxt_fat + "" + outLabel + "_" + channel + ".txt";

  string  nameFileTxt_MuPt10_fat("massFat_MuPt10_");
  nameFileTxt_MuPt10_fat = nameFileTxt_MuPt10_fat + "" + outLabel + "_" + channel + ".txt";


  outFileTxt_fat.open (nameFileTxt_fat.c_str());
  outFileTxt_MuPt10_fat.open(nameFileTxt_MuPt10_fat.c_str());

  TFile *inf_data  = TFile::Open(file);//"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root");
  
  

 



  TTree *tr_data_large = (TTree*)inf_data->Get(sChannel.c_str());
  //  TTree *tr_data = ntupleR2S->CopyTree();


  // p_T and mass boundaries ---------------------------
  
  Double_t xAxis1[84] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 
			 74,  88, 103, 119, 137, 156, 176, 197, 220, 244, 
			 270, 296, 325, 354, 386, 419, 453, 489, 526, 565, 
			 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7000}; 
  double ptBoundaries[55] = {1, 9, 18, 29, 40, 53, 67, 81, 97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248, 1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500, 2640, 2787, 2941, 3103, 3273, 3500};

  Double_t tAxis[72];
  for (unsigned i = 0; i<72; i++) tAxis[i] = i*1.0-0.5;

  // histogrmas definition ------------------------------


  QCDEvent *Event_data = new QCDEvent();
  TBranch *branch_data = tr_data_large->GetBranch("events");
  branch_data->SetAddress(&Event_data);

  // # of entries -------------------------------------------

  unsigned NEntries_data = tr_data_large->GetEntries();
  cout<<"Reading TREE: "<<NEntries_data <<" events"<<endl;
  int decade = 0;



  TFile* temp = new TFile(sTmp.c_str(),"RECREATE");
  temp->cd();
  
  //TCut generalCut = "";

  //  TCut generalCut = "( evtHdr().runNo() > 171440 && evtHdr().runNo() < 171460 )";
  TCut generalCut = "(fatmjjcor(0) > 800.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 2.5) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3)";

  //  TCut generalCut = "(fatmjjcor(0) > 740.0 &&  fabs( (fatjet(0)).eta() - (fatjet(1)).eta() ) < 1.3) || (pfmjjcor(0) > 838.0 &&  fabs( (pfjet(0)).eta() - (pfjet(1)).eta() ) < 1.3)";

  //  TCut generalCut = "(  ( fatmjjcor(0) > 500.0 &&  fabs(fatjet(0).eta() - fatjet(1).eta()) < 1.3 ) || ( pfmjjcor(0) > 500.0 &&  fabs( pfjet(0).eta() - pfjet(1).eta()) < 1.3 ) ||  ( calomjjcor(0) > 500.0 &&  fabs(calojet(0).eta() - calojet(1).eta()) < 1.3 ) )";// && ( evtHdr().runNo() < 149443 )";


  //  TTree* tr_data = tr_data_large;
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
    
    if (i%10000 == 0) {
      cout << "Is " << i << " mass is " <<  Event_data->fatmjjcor(0) << endl;
      
    }
    
    

    double progress = 100.0*i/(1.0*NEntries_data_SKIMED);
    int k = TMath::FloorNint(progress); 


    if (k > decade) 
      cout<<k<<" %"<<endl;

    decade = k;          
    
    tr_data->GetEntry(i);

    int nVtx = (Event_data->evtHdr()).nVtx();

    bool vertex_cut1 = (Event_data->evtHdr()).PVndof() >= 4;
    bool vertex_cut2 = (Event_data->evtHdr()).PVz() < 24 ;

    bool runNo_cuts = (Event_data->evtHdr()).runNo() > 160000 || (Event_data->evtHdr()).runNo() < 10;

 


    data_count++; 
    if (runNo_cuts) {
      runNo_count++;
      if(vertex_cut1) {
        vertex1_count++;
        if(vertex_cut2) {
          vertex2_count++;
	  

	  double rmax = 1.1;

	  // ----------- Main calculation ------------

	  reco::Particle::LorentzVector cand1 = Event_data->pfjet(0).p4()*Event_data->pfjet(0).cor();
	  reco::Particle::LorentzVector cand2 = Event_data->pfjet(1).p4()*Event_data->pfjet(1).cor();

	  reco::Particle::LorentzVector Fat11_Pt30_1 = cand1;
	  reco::Particle::LorentzVector Fat11_Pt30_2 = cand2;

	  int N1 = 0, N2 = 0, N1N2 = 0;

	  
	  for(unsigned j = 2; j<Event_data->nPFJets(); j++) {
	    reco::Particle::LorentzVector cand = Event_data->pfjet(j).p4()*Event_data->pfjet(j).cor();

	    if(fabs(cand.eta()) > 2.5 || !Event_data->pfjet(j).looseID() || cand.pt() < 30) continue;
	    
	    double dR1 = deltaR(cand1.eta(),cand.eta(),cand1.phi(),cand.phi());
	    double dR2 = deltaR(cand2.eta(),cand.eta(),cand2.phi(),cand.phi());
	    
	    if (dR1 < rmax && dR1 < dR2) {
	      Fat11_Pt30_1 += cand;
	      N1++;
	    }
	    if (dR2 < rmax && dR2 <= dR1) {
	      Fat11_Pt30_2 += cand;
	      N2++;
	    }
	    if (dR2 < rmax && dR1 < rmax) {
	      N1N2++;
	    }

	  }

	  reco::Particle::LorentzVector DiFat11_Pt30 = Fat11_Pt30_1;
	  DiFat11_Pt30 += Fat11_Pt30_2;



	  // ------------- Define Variables ------------


	  double fatPt1 = Fat11_Pt30_1.pt();
	  double fatPt2 = Fat11_Pt30_2.pt();


	  double fatPhi1 = Fat11_Pt30_1.phi();
	  double fatPhi2 = Fat11_Pt30_2.phi();
	  double dPhi_fat = (TMath::Pi()-fabs(fabs(fatPhi1-fatPhi2)-TMath::Pi()));


	  double fatMass = DiFat11_Pt30.mass();

	  double fatEta1 = Fat11_Pt30_1.eta();
	  double fatEta2 = Fat11_Pt30_2.eta();
	  double deltaFatEta = fabs( fatEta1 - fatEta2 );
	
	  double pfEta1 = Event_data->pfjet(0).p4().eta();
	  double pfEta2 = Event_data->pfjet(1).p4().eta();

	  bool fat_eta_cuts = fabs(fatEta1) < 2.5 && fabs(fatEta2) < 2.5  && fabs(pfEta1) < 2.5 && fabs(pfEta2) < 2.5;
	  bool fat_deta_cut =  deltaFatEta< 1.3;
	  bool fat_corMass_cuts = fatMass > 890.0;
	  bool fat_jetID_cuts = (Event_data->pfjet(0)).tightID() && (Event_data->pfjet(1)).tightID();
	  bool fat_dPhi_cuts = dPhi_fat > TMath::Pi()/3;

	  bool sumEt_cuts = (Event_data->pfmet()).sumet() < 7000.;

	  double muonMomentum1 = Event_data->pfjet(0).pt()*Event_data->pfjet(0).muf();
	  double muonMomentum2 = Event_data->pfjet(1).pt()*Event_data->pfjet(1).muf();


	  if(fat_corMass_cuts) {
	    fat_mass_count++;
	    if(fat_eta_cuts) {

	      if (fat_deta_cut) {
		fat_eta_count++;
		if(fat_jetID_cuts && fat_dPhi_cuts && sumEt_cuts ) {

		  outFileTxt_fat << fatMass << endl;
		  if (muonMomentum1 > 10 || muonMomentum2 > 10)
		    outFileTxt_MuPt10_fat << fatMass << endl;
			   
		}
	      }
	    }
	  }




	  
	}
      }

    }

  }
  

  outFileTxt_fat.close();
  outFileTxt_MuPt10_fat.close();


}




double deltaPhi2(double phi1, double phi2) { 
  double result = phi1 - phi2;
  while (result > double(M_PI)) result -= double(2*M_PI);
  while (result <= -double(M_PI)) result += double(2*M_PI);
  return result*result;
}


double deltaEta2(double eta1, double eta2) { 
  double result = eta1 - eta2;
  return result*result;
}

double deltaR(double eta1, double eta2, double phi1, double phi2 ){
  return sqrt( deltaEta2(eta1, eta2) + deltaPhi2(phi1, phi2) );

}
