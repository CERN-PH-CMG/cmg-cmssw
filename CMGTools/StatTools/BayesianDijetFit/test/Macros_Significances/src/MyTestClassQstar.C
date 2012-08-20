#define MyTestClass_cxx
#include "MyTestClassQstar.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void MyTestClass::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L MyTestClass.C
//      Root > MyTestClass t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch



  //## CREATE OUTPUT FILE ##

  TFile *f = new TFile("signalQstar.root","recreate");

  //## CREATE HISTOGRAMS ## 
  TH1F* hist_mass = new TH1F("hist_mass_Qstar","Invariant Mass for q*",100,0,1500);
  hist_mass->Sumw2();
  TCanvas *Canvas_1 = new TCanvas("Canvas_1","Canvas_1",11,51,700,500);
  hist_mass->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass->GetYaxis()->SetTitle("number of events");
  hist_mass->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg1 = new TLegend(0.4, 0.7, 0.5, 0.75);
 
  TH1F* hist_DeltaEta = new TH1F("hist_DeltaEta_Qstar","#Delta#eta for q*",100,0,2);
  hist_DeltaEta->Sumw2();
  TCanvas *Canvas_2 = new TCanvas("Canvas_2","Canvas_2",11,51,700,500);
  hist_DeltaEta->GetXaxis()->SetTitle("#Delta#eta");
  hist_DeltaEta->GetYaxis()->SetTitle("number of events");
  hist_DeltaEta->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg2 = new TLegend(0.2, 0.55, 0.3, 0.6);

  TH1F* hist_ExpDeltaEta = new TH1F("hist_ExpDeltaEta_Qstar","e^{#Delta#eta}  for q*",100,0,5);
  hist_ExpDeltaEta->Sumw2();
  TCanvas *Canvas_3 = new TCanvas("Canvas_3","Canvas_3",11,51,700,500);
  hist_ExpDeltaEta->GetXaxis()->SetTitle("e^{#Delta#eta}");
  hist_ExpDeltaEta->GetYaxis()->SetTitle("number of events");
  hist_ExpDeltaEta->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg3 = new TLegend(0.45, 0.45, 0.55, 0.5);

  TH1F* hist_CosThetaStar = new TH1F("hist_CosThetaStar_Qstar","cos#theta* for q*",100,0,1);
  hist_CosThetaStar->Sumw2();
  TCanvas *Canvas_4 = new TCanvas("Canvas_4","Canvas_4",11,51,700,500);
  hist_CosThetaStar->GetXaxis()->SetTitle("cos#theta*");
  hist_CosThetaStar->GetYaxis()->SetTitle("number of events");
  hist_CosThetaStar->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg4 = new TLegend(0.2, 0.55, 0.3, 0.6);

  TH1F* hist_mass1 = new TH1F("hist_mass1_Qstar","M1: Invariant mass with 0<#Delta#eta<0.5 for q*",100,0,1500);
  hist_mass1->Sumw2();
  TCanvas *Canvas_5 = new TCanvas("Canvas_5","Canvas_5",11,51,700,500);
  hist_mass1->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass1->GetYaxis()->SetTitle("number of events");
  hist_mass1->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg5 = new TLegend(0.3, 0.7, 0.5, 0.8);

  TH1F* hist_mass2 = new TH1F("hist_mass2_Qstar","M2: Invariant mass with 0.5<#Delta#eta<1 for q*",100,0,1500);
  hist_mass2->Sumw2();
  TCanvas *Canvas_6 = new TCanvas("Canvas_6","Canvas_6",11,51,700,500);
  hist_mass2->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass2->GetYaxis()->SetTitle("number of events");
  hist_mass2->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg6 = new TLegend(0.3, 0.7, 0.5, 0.8);

  TH1F* hist_mass3 = new TH1F("hist_mass3_Qstar","M3: Invariant mass with 1<#Delta#eta<1.3 for q*",100,0,1500);
  hist_mass3->Sumw2();
  TCanvas *Canvas_7 = new TCanvas("Canvas_7","Canvas_7",11,51,700,500);
  hist_mass3->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass3->GetYaxis()->SetTitle("number of events");
  hist_mass3->GetYaxis()->SetTitleOffset(1.5);
  TLegend* leg7 = new TLegend(0.3, 0.7, 0.5, 0.8);

 
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   //--------------------------------
   //TO RUN OVER THE FIRST 100 EVENTS
   //--------------------------------
   //for (Long64_t jentry=0; jentry<10000;jentry++) {

   //--------------------------------
   //TO RUN OVER THE FIRST ALL EVENTS
   //--------------------------------
   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


      if(jentry < 10 || jentry%1000 == 0) std::cout << "MyTestClass::Loop: Event Number = " << jentry << "/" << nentries << std::endl;
      //       std::endl;
      //       std::cout << "--- MyTestClass::Loop: Event Number = " << jentry << "/" << nentries << std::endl;

      // HLTPFJetPt , HLTPFJetEta , HLTPFJetPhi , HLTPFJetEnergy are vectors of doubles
      // the jets in the vector are ordered in decreasing pT, i.e. from the highest to the smallest.

      //## A few examples (uncomment to see the printout): ##

      //       //Loop over all the jets
      //       for (int jet_idx=0; jet_idx < HLTPFJetPt->size() ; jet_idx++ )
      // 	{	  
      // 	  cout << "jet index: " << jet_idx+1
      // 	       << " jet Pt: " << HLTPFJetPt->at(jet_idx)
      // 	       << " jet Eta: " << HLTPFJetEta->at(jet_idx)
      // 	       << " jet Phi: " << HLTPFJetPhi->at(jet_idx)
      // 	       << " jet Energy: " << HLTPFJetEnergy->at(jet_idx)
      // 	       << endl;	    
      // 	}
/*
            //leading (in pT) jet = jet1
             cout << "==> jet1 : " 
      	   << " jet1 Pt: " << HLTPFJetPt->at(0)
       	   << " jet1 Eta: " << HLTPFJetEta->at(0)
       	   << " jet1 Phi: " << HLTPFJetPhi->at(0)
       	   << " jet1 Energy: " << HLTPFJetEnergy->at(0)
       	   << endl;	    
      
             //second leading (in pT) jet = jet2
             cout << "==> jet2 : " 
       	   << " jet2 Pt: " << HLTPFJetPt->at(1)
       	   << " jet2 Eta: " << HLTPFJetEta->at(1)
       	   << " jet2 Phi: " << HLTPFJetPhi->at(1)
       	   << " jet2 Energy: " << HLTPFJetEnergy->at(1)
       	   << endl;	    
 */          

 
      //Create TLorentzVectors for two leading jets
      //see http://root.cern.ch/root/html/TLorentzVector.html
      TLorentzVector jet1;
      TLorentzVector jet2;
      //see http://root.cern.ch/root/html/TLorentzVector.html#TLorentzVector:SetPtEtaPhiE
      jet1.SetPtEtaPhiE( HLTPFJetPt->at(0), HLTPFJetEta->at(0), HLTPFJetPhi->at(0), HLTPFJetEnergy->at(0) );
      jet2.SetPtEtaPhiE( HLTPFJetPt->at(1), HLTPFJetEta->at(1), HLTPFJetPhi->at(1), HLTPFJetEnergy->at(1) );


 // select wide jets
/*
 vector<double> DeltaR1;
 vector<double> DeltaR2;
 vector<double> DeltaEta1;
 vector<double> DeltaEta2;
 vector<double> DeltaPhi1;
 vector<double> DeltaPhi2;
 for (int i=0; i<HLTPFJetEnergy->size(); i++)
 { 
 DeltaR1.push_back(100.);
 DeltaR2.push_back(100.);
 DeltaEta1.push_back(0.);
 DeltaEta2.push_back(0.);
 DeltaPhi1.push_back(0.);
 DeltaPhi2.push_back(0.);
 }

 for (int i=2; i<HLTPFJetEnergy->size(); i++)
 {
     if(HLTPFJetPt->at(i)>10 && TMath::Abs(HLTPFJetEta->at(i))<2.5)
     {
       DeltaEta1[i]=HLTPFJetEta->at(i)-HLTPFJetEta->at(0);
       DeltaEta2[i]=HLTPFJetEta->at(i)-HLTPFJetEta->at(1);
       DeltaPhi1[i]=HLTPFJetPhi->at(i)-HLTPFJetPhi->at(0);
       DeltaPhi2[i]=HLTPFJetPhi->at(i)-HLTPFJetPhi->at(1);
       DeltaR1[i]=TMath::Sqrt(DeltaEta1[i]**2-DeltaPhi1[i]**2);
       DeltaR2[i]=TMath::Sqrt(DeltaEta2[i]**2-DeltaPhi2[i]**2);
     }
 }

 TLorentzVector *jet= new TLorentzVector[HLTPFJetEta->size()];
      for (int i=2; i<HLTPFJetEnergy->size(); i++)
      {
         jet[i].SetPtEtaPhiE( HLTPFJetPt->at(i), HLTPFJetEta->at(i), HLTPFJetPhi->at(i), HLTPFJetEnergy->at(i) );
      }

 for (int i=2; i<HLTPFJetEnergy->size(); i++)
 {
     if (DeltaR1[i]<1.1) jet1=jet1+jet[i];
     if (DeltaR2[i]<1.1) jet2=jet2+jet[i];
 }

*/
      //dijet system      	
      TLorentzVector dijet;
      dijet = jet1+jet2;

      //Dijet mass
      double mass = dijet.M();

      //Fill histograms
 Double_t DeltaEta=TMath::Abs(HLTPFJetEta->at(0)-HLTPFJetEta->at(1));
 Double_t Eta1=TMath::Abs(HLTPFJetEta->at(0));
 Double_t Eta2=TMath::Abs(HLTPFJetEta->at(1));
 Double_t ExpDeltaEta=TMath::Exp(DeltaEta);
 Double_t CosThetaStar=(TMath::Exp(DeltaEta)-1)/(TMath::Exp(DeltaEta)+1);

 if(DeltaEta<1.3 && Eta1<2.5 && Eta2<2.5  && mass>550)
{
      hist_mass->Fill(mass);
      if(DeltaEta<0.5) hist_mass1->Fill(mass);
      if(DeltaEta>0.5 &&  DeltaEta<1) hist_mass2->Fill(mass);
      if(DeltaEta>1 && DeltaEta<1.3) hist_mass3->Fill(mass);
      hist_DeltaEta->Fill(DeltaEta);
      hist_ExpDeltaEta->Fill(ExpDeltaEta);
      hist_CosThetaStar->Fill(CosThetaStar);
} 
     
   }
/*
 TF1 *f1 = new TF1("func1","([0]*TMath::Power(1-x/TMath::Sqrt(8000),[1]))/(TMath::Power(x/TMath::Sqrt(8000),[2]+[3]*TMath::Log(x/TMath::Sqrt(8000))))",0,8000);
 f1->SetParameters(0.001,2.,-2.,5.);
*/

 Canvas_1->cd(0);
  // hist_mass->Fit("func1");
   hist_mass->Draw();
   leg1->AddEntry(hist_mass,"  q*","l");
   leg1->Draw();
   hist_mass->Write();

 Canvas_2->cd(0);
   hist_DeltaEta->Draw();
   leg2->AddEntry(hist_DeltaEta,"  q*","l");
   leg2->Draw();
   hist_DeltaEta->Write();

 Canvas_3->cd(0);
   hist_ExpDeltaEta->Draw();
   leg3->AddEntry(hist_ExpDeltaEta,"  q*","l");
   leg3->Draw();
   hist_ExpDeltaEta->Write();

 Canvas_4->cd(0);
   hist_CosThetaStar->Draw();
   leg4->AddEntry(hist_CosThetaStar,"  q*","l");
   leg4->Draw();
   hist_CosThetaStar->Write();

 Canvas_5->cd(0);
 hist_mass1->Draw();
 leg5->AddEntry(hist_mass1,"q*   0<#Delta#eta<0.5","l");
 //leg5->AddEntry((TObject*)0,"","");
 leg5->Draw();
 hist_mass1->Write();
 Canvas_6->cd(0);
 hist_mass2->Draw();
 leg6->AddEntry(hist_mass3,"q*   0.5<#Delta#eta<1","l");
 //leg6->AddEntry((TObject*)0,"","");
 leg6->Draw();
 hist_mass2->Write();
 Canvas_7->cd(0);
 hist_mass3->Draw();
 leg7->AddEntry(hist_mass3,"q*   1<#Delta#eta<1.3","l");
 //leg7->AddEntry((TObject*)0,"","");
 leg7->Draw();
 hist_mass3->Write();

/*
  TH1F *hist_mass1_ratio = (TH1F*)hist_mass1->Clone("hist_mass1_Qstar_ratio");
  hist_mass1_ratio->Sumw2(); 
  TCanvas *Canvas_8 = new TCanvas("Canvas_8","Canvas_8",11,51,700,500);
  hist_mass1_ratio->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass1_ratio->SetTitle("M1/M (signal)");
  TLegend* leg8 = new TLegend(0.15, 0.6, 0.6, 0.85);

  TH1F *hist_mass2_ratio = (TH1F*)hist_mass2->Clone("hist_mass2_Qstar_ratio");
  hist_mass2_ratio->Sumw2(); 
  TCanvas *Canvas_9 = new TCanvas("Canvas_9","Canvas_9",11,51,700,500);
  hist_mass2_ratio->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass2_ratio->SetTitle("M2/M (signal)");
  TLegend* leg9 = new TLegend(0.15, 0.6, 0.6, 0.85);

  TH1F *hist_mass3_ratio = (TH1F*)hist_mass3->Clone("hist_mass3_Qstar_ratio");
  hist_mass3_ratio->Sumw2(); 
  TCanvas *Canvas_10 = new TCanvas("Canvas_10","Canvas_10",11,51,700,500);
  hist_mass3_ratio->GetXaxis()->SetTitle("Invariant Mass (GeV)");
  hist_mass3_ratio->SetTitle("M3/M (signal)");
  TLegend* leg10 = new TLegend(0.15, 0.6, 0.6, 0.85);

  double Integral1=hist_mass1_ratio->Integral();
  hist_mass1_ratio->Scale(1/Integral1);
 
  double Integral2=hist_mass2_ratio->Integral();
  hist_mass2_ratio->Scale(1/Integral2);

  double Integral3=hist_mass3_ratio->Integral();
  hist_mass3_ratio->Scale(1/Integral3);

  cout<<"integral1: "<<hist_mass1_ratio->Integral()<<endl;
  cout<<"integral2: "<<hist_mass2_ratio->Integral()<<endl;
  cout<<"integral3: "<<hist_mass3_ratio->Integral()<<endl;


 TF1 *f1= new TF1("func1","[0]+[1]*x");
 f1->SetParameters(1,0);

 TF1 *f2= new TF1("func2","[0]+[1]*x");
 f2->SetParameters(1,0);

 TF1 *f3= new TF1("func3","[0]+[1]*x");
 f3->SetParameters(1,0);

 gStyle->SetOptFit(111); 

 Canvas_8->cd(0);
 hist_mass1_ratio->Divide(hist_mass1_ratio,hist_mass,1,1,"B");
 hist_mass1_ratio->Fit("func1");
 hist_mass1_ratio->Draw();
 leg8->AddEntry(hist_mass1_ratio,"ratio of the invariant mass for 0<DeltaEta<0.5","l");
 leg8->AddEntry((TObject*)0,"to the invariant mass for 0<DeltaEta<1.3","");
 leg8->AddEntry((TObject*)0,"","");
 leg8->AddEntry(func1,"linear fit (p0+p1*x)","l");
 leg8->Draw();
 //leg1->Write();
 //hist_mass1_ratio->Rebin();
 hist_mass1_ratio->Write();
 Canvas_9->cd(0);
 hist_mass2_ratio->Divide(hist_mass2_ratio,hist_mass,1,1,"B");
 hist_mass2_ratio->Fit("func2");
 hist_mass2_ratio->Draw();
 leg9->AddEntry(hist_mass2_ratio,"ratio of the invariant mass for 0.5<DeltaEta<1","l");
 leg9->AddEntry((TObject*)0,"to the invariant mass for 0<DeltaEta<1.3","");
 leg9->AddEntry((TObject*)0,"","");
 leg9->AddEntry(func2,"linear fit (p0+p1*x)","l");
 leg9->Draw();
 //leg2->Write();
 //hist_mass2_ratio->Rebin();
 hist_mass2_ratio->Write();
 Canvas_10->cd(0);
 hist_mass3_ratio->Divide(hist_mass3_ratio,hist_mass,1,1,"B");
 hist_mass3_ratio->Fit("func3");
 hist_mass3_ratio->Draw();
 leg10->AddEntry(hist_mass3_ratio,"ratio of the invariant mass for 1<DeltaEta<1.3","l");
 leg10->AddEntry((TObject*)0,"to the invariant mass for 0<DeltaEta<1.3","");
 leg10->AddEntry((TObject*)0,"","");
 leg10->AddEntry(func3,"linear fit (p0+p1*x)","l");
 leg10->Draw();
 //leg3->Write();
 //hist_mass3_ratio->Rebin();
 hist_mass3_ratio->Write();
*/
Double_t Ntot=hist_mass->GetEntries();
Double_t N1=hist_mass1->GetEntries();
Double_t N2=hist_mass2->GetEntries();
Double_t N3=hist_mass3->GetEntries();

cout<<"for 0<DeltaEta<0.5: "<<N1/Ntot*100<<" %"<<endl;
cout<<"for 0.5<DeltaEta<1: "<<N2/Ntot*100<<" %"<<endl;
cout<<"for 1<DeltaEta<1.3: "<<N3/Ntot*100<<" %"<<endl;


}
