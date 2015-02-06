#include <cmath>
#include "TH1D.h"
#include "../interface/QGSyst.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "DrawBase.h"


bool doubleMin = false;


void drawSinglePlot( DrawBase* db, TH1D* h1_data, TH1D* h1_qgl, TH1D* h1_qglSyst, float ptMin, float ptMax, float etaMin, float etaMaxi, float rhoMin, float rhoMax );

int main( int argc, char* argv[] ) {


  std::string outfilename = "prova_doubleMin.root";

  if( argc>1 ) {
    std::string doubleMin_src(argv[1]);
    if( doubleMin_src != "false" ) {
      doubleMin = true;
      std::cout << "-> Switching ON double min" << std::endl;
      outfilename = "prova_doubleMin.root";
    }
  }


  TFile* file_puWeights = TFile::Open("puWeights.root");
  TH1D* h1_puweights = (TH1D*)file_puWeights->Get("puweights");


  TFile* file = TFile::Open("/afs/cern.ch/user/a/amarini/work/GluonTag/ZJet/ZJet_DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1_2.root");
  TTree* tree = (TTree*)file->Get("tree_passedEvents");

  //TFile* file_data = TFile::Open("/afs/cern.ch/user/a/amarini/work/GluonTag/ZJet/ZJet_DoubleMu-Run2012AB.root");
  TChain* tree_data = new TChain("tree_passedEvents");
  tree_data->Add("/afs/cern.ch/user/a/amarini/work/GluonTag/ZJet/ZJet_DoubleMu-Run2012*.root");
  //TTree* tree_data = (TTree*)file_data->Get("tree_passedEvents");


  float rho;
  tree->SetBranchAddress("rhoPF", &rho );
  float ptJet;
  tree->SetBranchAddress("ptJet0", &ptJet );
  float etaJet;
  tree->SetBranchAddress("etaJet0", &etaJet );
  int pdgIdPartJet;
  tree->SetBranchAddress("pdgIdPartJet0", &pdgIdPartJet );
  float qglJet;
  tree->SetBranchAddress("QGLHisto", &qglJet );
  int nvertex; 
  tree->SetBranchAddress("nvertex", &nvertex );
  float deltaPhi_jet;
  tree->SetBranchAddress("deltaPhi_jet", &deltaPhi_jet );
  float mZ;
  tree->SetBranchAddress("mZ", &mZ );
  float ptZ;
  tree->SetBranchAddress("ptZ", &ptZ );
  float ptD_QCJet;
  tree->SetBranchAddress("ptD_QCJet0", &ptD_QCJet );
  float betaStarJet;
  tree->SetBranchAddress("betaStarJet0", &betaStarJet );
  bool btagged;
  tree->SetBranchAddress("btagged", &btagged );


  QGSyst qgsyst;
  if( doubleMin )
    qgsyst.ReadDatabaseDoubleMin("../data/SystDatabase_doubleMin.txt");
  else
    qgsyst.ReadDatabase("../data/SystDatabase.txt");
  qgsyst.SetTagger("QGLHisto");


  TFile* outfile = TFile::Open(outfilename.c_str(), "RECREATE");
  outfile->cd(); 

  TH1D* h1_ptZ = new TH1D("ptZ", "", 100, 0., 100.);
  h1_ptZ->Sumw2();
  TH1D* h1_rho = new TH1D("rho", "", 40, 0., 40.);
  h1_rho->Sumw2();

  TH1D* h1_qglJet_pt3050_eta02_rho040 = new TH1D("qglJet_pt3050_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt3050_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt3050_eta02_rho040 = new TH1D("qglJetSyst_pt3050_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt3050_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt5080_eta02_rho040 = new TH1D("qglJet_pt5080_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt5080_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt5080_eta02_rho040 = new TH1D("qglJetSyst_pt5080_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt5080_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt80120_eta02_rho040 = new TH1D("qglJet_pt80120_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt80120_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt80120_eta02_rho040 = new TH1D("qglJetSyst_pt80120_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt80120_eta02_rho040->Sumw2();

  TH1D* h1_qglJet_pt120250_eta02_rho040 = new TH1D("qglJet_pt120250_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt120250_eta02_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt120250_eta02_rho040 = new TH1D("qglJetSyst_pt120250_eta02_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt120250_eta02_rho040->Sumw2();


  TH1D* h1_qglJet_pt3050_eta35_rho040 = new TH1D("qglJet_pt3050_eta35_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt3050_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt3050_eta35_rho040 = new TH1D("qglJetSyst_pt3050_eta35_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt3050_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt5080_eta35_rho040 = new TH1D("qglJet_pt5080_eta35_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt5080_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt5080_eta35_rho040 = new TH1D("qglJetSyst_pt5080_eta35_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt5080_eta35_rho040->Sumw2();

  TH1D* h1_qglJet_pt80120_eta35_rho040 = new TH1D("qglJet_pt80120_eta35_rho040", "", 100, 0., 1.0001);
  h1_qglJet_pt80120_eta35_rho040->Sumw2();
  TH1D* h1_qglJetSyst_pt80120_eta35_rho040 = new TH1D("qglJetSyst_pt80120_eta35_rho040", "", 100, 0., 1.0001);
  h1_qglJetSyst_pt80120_eta35_rho040->Sumw2();



  int nentries = tree->GetEntries();

  for( unsigned iEntry = 0; iEntry < nentries; ++iEntry ) {

    tree->GetEntry(iEntry);

    if( rho>40. ) continue;
    if( mZ<70. || mZ>110. ) continue;
    if( deltaPhi_jet < 3.1415-0.5 ) continue;
    if( ptD_QCJet <=0. ) continue;
    if( fabs(etaJet)<2.5 && betaStarJet > 0.2 * log( (float)nvertex - 0.67) ) continue;
    if( btagged ) continue;

    float puweight = h1_puweights->GetBinContent((int)rho);

    h1_ptZ->Fill( ptZ, puweight );
    h1_rho->Fill( rho, puweight );

    bool smearJet = true;

    std::string type = "all";
    if( doubleMin ) {
      if( fabs(pdgIdPartJet)>0 && fabs(pdgIdPartJet)<6 ) type = "quark";
      else if( pdgIdPartJet==21 ) type = "gluon";
      else { // both 0 and -999
        //smearJet = false;
        type = "gluon";
        //std::cout << "Unknown jet PDG ID (" << pdgIdPartJet << "). Will use gluon instead." << std::endl;
      }
    }

    float qglJetSyst = (smearJet) ? qgsyst.Smear(ptJet, etaJet, rho, qglJet, type) : qglJet;

    if( fabs(etaJet)<2. ) {

      if( ptJet > 30. && ptJet < 50. ) {

        h1_qglJet_pt3050_eta02_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt3050_eta02_rho040->Fill( qglJetSyst, puweight );

      } else if( ptJet > 50. && ptJet < 80. ) {

        h1_qglJet_pt5080_eta02_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt5080_eta02_rho040->Fill( qglJetSyst, puweight );

      } else if( ptJet > 80. && ptJet < 120. ) {

        h1_qglJet_pt80120_eta02_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt80120_eta02_rho040->Fill( qglJetSyst, puweight );

      } else if( ptJet > 120. && ptJet < 250. ) {

        h1_qglJet_pt120250_eta02_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt120250_eta02_rho040->Fill( qglJetSyst, puweight );

      } 

    } else if( fabs(etaJet)>3. && fabs(etaJet)<5. ) {

      if( ptJet > 30. && ptJet < 50. ) {

        h1_qglJet_pt3050_eta35_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt3050_eta35_rho040->Fill( qglJetSyst, puweight );

      } else if( ptJet > 50. && ptJet < 80. ) {

        h1_qglJet_pt5080_eta35_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt5080_eta35_rho040->Fill( qglJetSyst, puweight );

      } else if( ptJet > 80. && ptJet < 120. ) {

        h1_qglJet_pt80120_eta35_rho040->Fill( qglJet, puweight );
        h1_qglJetSyst_pt80120_eta35_rho040->Fill( qglJetSyst, puweight );

      } 

    } // eta

  }


  // now switch to data:
  tree_data->SetBranchAddress("nvertex", &nvertex );
  tree_data->SetBranchAddress("mZ", &mZ );
  tree_data->SetBranchAddress("ptZ", &ptZ );
  tree_data->SetBranchAddress("deltaPhi_jet", &deltaPhi_jet );
  tree_data->SetBranchAddress("rhoPF", &rho );
  tree_data->SetBranchAddress("ptJet0", &ptJet );
  tree_data->SetBranchAddress("etaJet0", &etaJet );
  tree_data->SetBranchAddress("ptD_QCJet0", &ptD_QCJet );
  tree_data->SetBranchAddress("QGLHisto", &qglJet );
  float qglJetFwd;
  tree_data->SetBranchAddress("QGLHistoFwd", &qglJetFwd );
  tree_data->SetBranchAddress("betaStarJet0", &betaStarJet );


  TH1D* h1_data_ptZ = new TH1D("data_ptZ", "", 100, 0., 100.);
  h1_data_ptZ->Sumw2();
  TH1D* h1_data_rho = new TH1D("data_rho", "", 40, 0., 40.);
  h1_data_rho->Sumw2();

  TH1D* h1_data_qglJet_pt3050_eta02_rho040 = new TH1D("data_qglJet_pt3050_eta02_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt3050_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt5080_eta02_rho040 = new TH1D("data_qglJet_pt5080_eta02_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt5080_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt80120_eta02_rho040 = new TH1D("data_qglJet_pt80120_eta02_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt80120_eta02_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt120250_eta02_rho040 = new TH1D("data_qglJet_pt120250_eta02_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt120250_eta02_rho040->Sumw2();


  TH1D* h1_data_qglJet_pt3050_eta35_rho040 = new TH1D("data_qglJet_pt3050_eta35_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt3050_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt5080_eta35_rho040 = new TH1D("data_qglJet_pt5080_eta35_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt5080_eta35_rho040->Sumw2();

  TH1D* h1_data_qglJet_pt80120_eta35_rho040 = new TH1D("data_qglJet_pt80120_eta35_rho040", "", 100, 0., 1.0001);
  h1_data_qglJet_pt80120_eta35_rho040->Sumw2();


  int nentries_data = tree_data->GetEntries();

  for( unsigned iEntry = 0; iEntry < nentries_data; ++iEntry ) {

    tree_data->GetEntry(iEntry);

    if( rho>40. ) continue;
    if( mZ<70. || mZ>110. ) continue;
    if( deltaPhi_jet < 3.1415-0.5 ) continue;
    if( ptD_QCJet <=0. ) continue;
    if( fabs(etaJet)<2.5 && betaStarJet > 0.2 * log( (float)nvertex - 0.67) ) continue;
    if( btagged ) continue;

    h1_data_ptZ->Fill( ptZ );
    h1_data_rho->Fill( rho );

    if( fabs(etaJet)<2. ) {

      if( ptJet > 30. && ptJet < 50. ) {

        h1_data_qglJet_pt3050_eta02_rho040->Fill( qglJet );

      } else if( ptJet > 50. && ptJet < 80. ) {

        h1_data_qglJet_pt5080_eta02_rho040->Fill( qglJet );

      } else if( ptJet > 80. && ptJet < 120. ) {

        h1_data_qglJet_pt80120_eta02_rho040->Fill( qglJet );

      } else if( ptJet > 120. && ptJet < 250. ) {

        h1_data_qglJet_pt120250_eta02_rho040->Fill( qglJet );

      } 

    } else if( fabs(etaJet)>3. && fabs(etaJet)<5. ) {

      if( ptJet > 30. && ptJet < 50. ) {

        h1_data_qglJet_pt3050_eta35_rho040->Fill( qglJetFwd );

      } else if( ptJet > 50. && ptJet < 80. ) {

        h1_data_qglJet_pt5080_eta35_rho040->Fill( qglJetFwd );

      } else if( ptJet > 80. && ptJet < 120. ) {

        h1_data_qglJet_pt80120_eta35_rho040->Fill( qglJetFwd );

      } 

    } // eta

  }


  DrawBase* db = new DrawBase("provaSyst");
  
  drawSinglePlot( db, h1_data_qglJet_pt3050_eta02_rho040, h1_qglJet_pt3050_eta02_rho040, h1_qglJetSyst_pt3050_eta02_rho040, 30., 50., 0., 2., 0., 40.);
  drawSinglePlot( db, h1_data_qglJet_pt5080_eta02_rho040, h1_qglJet_pt5080_eta02_rho040, h1_qglJetSyst_pt5080_eta02_rho040, 50., 80., 0., 2., 0., 40.);
  drawSinglePlot( db, h1_data_qglJet_pt80120_eta02_rho040,h1_qglJet_pt80120_eta02_rho040, h1_qglJetSyst_pt80120_eta02_rho040, 80., 120., 0., 2., 0., 40.);
  drawSinglePlot( db, h1_data_qglJet_pt120250_eta02_rho040,h1_qglJet_pt120250_eta02_rho040, h1_qglJetSyst_pt120250_eta02_rho040, 120., 250., 0., 2., 0., 40.);

  drawSinglePlot( db, h1_data_qglJet_pt3050_eta35_rho040, h1_qglJet_pt3050_eta35_rho040, h1_qglJetSyst_pt3050_eta35_rho040, 30., 50., 3., 5., 0., 40.);
  drawSinglePlot( db, h1_data_qglJet_pt5080_eta35_rho040, h1_qglJet_pt5080_eta35_rho040, h1_qglJetSyst_pt5080_eta35_rho040, 50., 80., 3., 5., 0., 40.);
  drawSinglePlot( db, h1_data_qglJet_pt80120_eta35_rho040,h1_qglJet_pt80120_eta35_rho040, h1_qglJetSyst_pt80120_eta35_rho040, 80., 120., 3., 5., 0., 40.);



  outfile->cd();
 
  h1_ptZ->Write();
  h1_rho->Write();

  h1_data_ptZ->Write();
  h1_data_rho->Write();

  h1_qglJet_pt3050_eta02_rho040->Write();
  h1_qglJetSyst_pt3050_eta02_rho040->Write();
  
  h1_qglJet_pt5080_eta02_rho040->Write();
  h1_qglJetSyst_pt5080_eta02_rho040->Write();
  
  h1_qglJet_pt80120_eta02_rho040->Write();
  h1_qglJetSyst_pt80120_eta02_rho040->Write();

  h1_qglJet_pt120250_eta02_rho040->Write();
  h1_qglJetSyst_pt120250_eta02_rho040->Write();

  h1_qglJet_pt3050_eta35_rho040->Write();
  h1_qglJetSyst_pt3050_eta35_rho040->Write();
  
  h1_qglJet_pt5080_eta35_rho040->Write();
  h1_qglJetSyst_pt5080_eta35_rho040->Write();
  
  h1_qglJet_pt80120_eta35_rho040->Write();
  h1_qglJetSyst_pt80120_eta35_rho040->Write();


  h1_data_qglJet_pt3050_eta02_rho040->Write();
  
  h1_data_qglJet_pt5080_eta02_rho040->Write();
  
  h1_data_qglJet_pt80120_eta02_rho040->Write();

  h1_data_qglJet_pt120250_eta02_rho040->Write();

  h1_data_qglJet_pt3050_eta35_rho040->Write();
  
  h1_data_qglJet_pt5080_eta35_rho040->Write();
  
  h1_data_qglJet_pt80120_eta35_rho040->Write();

  outfile->Close();
  


  return 0;
  
}


void drawSinglePlot( DrawBase* db, TH1D* h1_data, TH1D* h1_qgl, TH1D* h1_qglSyst, float ptMin, float ptMax, float etaMin, float etaMax, float rhoMin, float rhoMax ) {


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  h1_qgl->Rebin(2);
  h1_qglSyst->Rebin(2);
  h1_data->Rebin(2);

  h1_qgl->SetLineStyle(2);
  h1_qgl->SetLineWidth(2);
  h1_qglSyst->SetLineWidth(2);
  h1_data->SetMarkerStyle(20);

  float ymax = h1_qgl->GetMaximum() / h1_qgl->Integral();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 1.0001, 10, 0., 1.4*ymax);
  h2_axes->SetXTitle("QG LD");
  h2_axes->SetYTitle("Normalized to Unity");

  h1_qglSyst->SetLineColor(kRed);

  h2_axes->Draw();
  h1_qgl->DrawNormalized("Histo same");
  h1_qglSyst->DrawNormalized("Histo same");
  h1_data->DrawNormalized("P same");

  char legendTitle[500];
  sprintf( legendTitle, "p_{T}(%.0f-%.0f), #eta(%.1f-%.1f), #rho(%.0f-%.0f)", ptMin, ptMax, etaMin, etaMax, rhoMin, rhoMax);

  TLegend* legend = new TLegend(0.25, 0.7, 0.75, 0.9, legendTitle);
  legend->SetFillColor(0);
  legend->SetTextSize(0.038);
  legend->AddEntry( h1_data, "Data", "p" );
  legend->AddEntry( h1_qgl, "Before Smearing", "L" );
  legend->AddEntry( h1_qglSyst, "After Smearing", "L" );
  legend->Draw("same");

  std::string doubleMin_str = "";
  if( doubleMin ) doubleMin_str = "DM";

  char canvasName[500];
  sprintf( canvasName, "prova%s_pt%.0f_%.0f_eta%.0f_%.0f_rho%.0f_%.0f.eps", doubleMin_str.c_str(), ptMin, ptMax, etaMin, etaMax, rhoMin, rhoMax);

  TPaveText* label_top = db->get_labelTop();
  label_top->Draw("same");

  c1->SaveAs(canvasName);

  delete c1;
  delete legend;
  delete h2_axes;

}
