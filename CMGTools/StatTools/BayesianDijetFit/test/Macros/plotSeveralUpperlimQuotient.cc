
// program to plot several upperlimit quotient (limit[statlevel=i]/limit[statlevel=0])

#include <iostream>
#include <sstream>
#include <fstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "Riostream.h"
#include "TMath.h"
#include "TGraph.h"

#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
using namespace std;


void plotSeveralUpperlimQuotient () {


// function to calculate the number of lines in a file
int lineNumber(ifstream & myStream)
{
int n = 0;
string line;
while (getline(myStream, line)) {
n++;
}
return n;
}



TCanvas *c0 = new TCanvas("Quotient");


// ************  0  **************

//opening the file
string name0 = "results_0_0.txt"; //name of the file
ifstream myStream0 = name0.c_str();
int const n0 = lineNumber(myStream0);
ifstream myStream00 = name0.c_str();
//definition of 2 tables
Double_t masses0[n0]; //one for masses
Double_t limits0[n0]; //one for upperlimits 
//filling the 2 tables
for (int i=0; i<n0; i++) myStream00 >> masses0[i] >> limits0[i];
//definition of one other table with limit0/limit0
Double_t quotientLimits0[n0];
for (i=0; i<n0; i++) quotientLimits0[i] = limits0[i]/limits0[i];
//plot graph 
//TGraph *gr0 = new TGraph (n0, masses0, quotientLimits0);
//gr0->SetLineColor(0);
//gr0->SetLineWidth(2);
//gr0->SetMarkerStyle(20);
//gr0->Draw("AC*");


// ************  1  **************

string name1 = "results_1_0.txt";
ifstream myStream1 = name1.c_str();
int const n1 = lineNumber(myStream1);
ifstream myStream01 = name1.c_str();
Double_t masses1[n1]; 
Double_t limits1[n1]; 
for (int i=0; i<n1; i++) myStream01 >> masses1[i] >> limits1[i];
Double_t quotientLimits1[n1];
for (i=0; i<n1; i++) quotientLimits1[i] = limits1[i]/limits0[i];
TGraph *gr1 = new TGraph (n1, masses1, quotientLimits1);
gr1->SetLineColor(1);
gr1->SetLineWidth(2);
gr1->SetMarkerStyle(21);
gr1->Draw("ALP"); //the window is based on this 


// ************  2  **************

string name2 = "results_2_0.txt";
ifstream myStream2 = name2.c_str();
int const n2 = lineNumber(myStream2);
ifstream myStream02 = name2.c_str();
Double_t masses2[n2]; 
Double_t limits2[n2]; 
for (int i=0; i<n2; i++) myStream02 >> masses2[i] >> limits2[i];
Double_t quotientLimits2[n2];
for (i=0; i<n2; i++) quotientLimits2[i] = limits2[i]/limits0[i];
TGraph *gr2 = new TGraph (n2, masses2, quotientLimits2);
gr2->SetLineColor(2);
gr2->SetLineWidth(2);
gr2->SetMarkerStyle(22);
gr2->Draw("LP");


// ************  3  **************

string name3 = "results_3_0.txt";
ifstream myStream3 = name3.c_str();
int const n3 = lineNumber(myStream3);
ifstream myStream03 = name3.c_str();
Double_t masses3[n3]; 
Double_t limits3[n3]; 
for (int i=0; i<n3; i++) myStream03 >> masses3[i] >> limits3[i];
Double_t quotientLimits3[n3];
for (i=0; i<n3; i++) quotientLimits3[i] = limits3[i]/limits0[i];
TGraph *gr3 = new TGraph (n3, masses3, quotientLimits3);
gr3->SetLineColor(3);
gr3->SetLineWidth(2);
gr3->SetMarkerStyle(23);
gr3->Draw("LP");


// ************  4  **************

string name4 = "results_4_0.txt";
ifstream myStream4 = name4.c_str();
int const n4 = lineNumber(myStream4);
ifstream myStream04 = name4.c_str();
Double_t masses4[n4]; 
Double_t limits4[n4]; 
for (int i=0; i<n4; i++) myStream04 >> masses4[i] >> limits4[i];
Double_t quotientLimits4[n4];
for (i=0; i<n4; i++) quotientLimits4[i] = limits4[i]/limits0[i];
TGraph *gr4 = new TGraph (n4, masses4, quotientLimits4);
gr4->SetLineColor(4);
gr4->SetLineWidth(2);
gr4->SetMarkerStyle(24);
gr4->Draw("LP");

/*
// ************  5  **************

string name5 = "results_5_0.txt";
ifstream myStream5 = name5.c_str();
int const n5 = lineNumber(myStream5);
ifstream myStream05 = name5.c_str();
Double_t masses5[n5]; 
Double_t limits5[n5]; 
for (int i=0; i<n5; i++) myStream05 >> masses5[i] >> limits5[i];
Double_t quotientLimits5[n5];
for (i=0; i<n5; i++) quotientLimits5[i] = limits5[i]/limits0[i];
TGraph *gr5 = new TGraph (n5, masses5, quotientLimits5);
gr5->SetLineColor(5);
gr5->SetLineWidth(2);
gr5->SetMarkerStyle(25);
gr5->Draw("LP");
*/

// ************  6  **************

string name6 = "results_6_0.txt";
ifstream myStream6 = name6.c_str();
int const n6 = lineNumber(myStream6);
ifstream myStream06 = name6.c_str();
Double_t masses6[n6]; 
Double_t limits6[n6]; 
for (int i=0; i<n6; i++) myStream06 >> masses6[i] >> limits6[i];
Double_t quotientLimits6[n6];
for (i=0; i<n6; i++) quotientLimits6[i] = limits6[i]/limits0[i];
TGraph *gr6 = new TGraph (n6, masses6, quotientLimits6);
gr6->SetLineColor(6);
gr6->SetLineWidth(2);
gr6->SetMarkerStyle(26);
gr6->Draw("LP");


//Definition af a table a the sum of the quotient
Double_t sum[n0];
for (i=0; i<n0; i++) 
  {
  sum[i] = quotientLimits1[i] + quotientLimits2[i] + quotientLimits3[i] + quotientLimits4[i] - quotientLimits6[i] - 3;
  }



TCanvas *c1 = new TCanvas("Sum");

TGraph *Sum = new TGraph(n0, masses0, sum);
Sum->Draw("A*");

TFile plot1("quotientSum.root","recreate");

c1->Write();

c1->SaveAs("quotientSum.png");


/*
//check that tables are well-filled

for (i=0; i<n0; i++) {cout << masses0[i] << endl;}
for (i=0; i<n0; i++) {cout << limits0[i] << endl;}
for (i=0; i<n0; i++) {cout << quotientLimits0[i] << endl;}
cout << endl;

for (i=0; i<n1; i++) {cout << masses1[i] << endl;}
for (i=0; i<n1; i++) {cout << limits1[i] << endl;}
for (i=0; i<n1; i++) {cout << quotientLimits1[i] << endl;}
cout << endl;

for (i=0; i<n2; i++) {cout << masses2[i] << endl;}
for (i=0; i<n2; i++) {cout << limits2[i] << endl;}
for (i=0; i<n2; i++) {cout << quotientLimits2[i] << endl;}
cout << endl;

for (i=0; i<n3; i++) {cout << masses3[i] << endl;}
for (i=0; i<n3; i++) {cout << limits3[i] << endl;}
for (i=0; i<n3; i++) {cout << quotientLimits3[i] << endl;}
cout << endl;

for (i=0; i<n4; i++) {cout << masses4[i] << endl;}
for (i=0; i<n4; i++) {cout << limits4[i] << endl;}
for (i=0; i<n4; i++) {cout << quotientLimits4[i] << endl;}
cout << endl;

for (i=0; i<n5; i++) {cout << masses5[i] << endl;}
for (i=0; i<n5; i++) {cout << limits5[i] << endl;}
for (i=0; i<n5; i++) {cout << quotientLimits5[i] << endl;}
cout << endl;

for (i=0; i<n6; i++) {cout << masses6[i] << endl;}
for (i=0; i<n6; i++) {cout << limits6[i] << endl;}
for (i=0; i<n6; i++) {cout << quotientLimits6[i] << endl;}
cout << endl;
*/


TFile plot0("plotAllUpperlimQuotient.root","recreate");

c0->Write();

c0->SaveAs("plotAllUpperlimQuotient.png");

}
