
// Program to plot on the same graph the averrage of some results and another curve

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


void plotUpperlimAverrage () {


//function to calculate the number of lines in a file
int lineNumber(ifstream & myStream)
{
int n = 0;
string line;
while (getline(myStream, line)) {
n++;
}
return n;
}



string efficiency = "efficiency.txt";
ifstream efficiencyStream = efficiency.c_str();
int const neff = lineNumber(efficiencyStream);
ifstream efficiencyStream0 = efficiency.c_str();
Double_t efficiencies[neff];
for (int i=0; i<neff; i++) efficiencyStream0 >> efficiencies[i];


// ************  0  **************

//opening the file
string name0 = "negativeResults_0_0.txt";
ifstream myStream0 = name0.c_str();
int const n0 = lineNumber(myStream0);
ifstream myStream00 = name0.c_str();
//definition of 2 tables
Double_t masses0[n0]; //one for masses
Double_t limits0[n0]; //one for upperlimits 
//filling the 2 tables
for (int i=0; i<n0; i++) myStream00 >> masses0[i] >> limits0[i];
//definition of one other table with limit/efficiency
Double_t realLimits0[n0];
for (i=0; i<n0; i++) realLimits0[i] = limits0[i]/efficiencies[i];


// ************  1  **************

string name1 = "negativeResults_1_0.txt";
ifstream myStream1 = name1.c_str();
int const n1 = lineNumber(myStream1);
ifstream myStream01 = name1.c_str();
Double_t masses1[n1]; 
Double_t limits1[n1]; 
for (int i=0; i<n1; i++) myStream01 >> masses1[i] >> limits1[i];
Double_t realLimits1[n1];
for (i=0; i<n1; i++) realLimits1[i] = limits1[i]/efficiencies[i];


// ************  2  **************

string name2 = "negativeResults_2_0.txt";
ifstream myStream2 = name2.c_str();
int const n2 = lineNumber(myStream2);
ifstream myStream02 = name2.c_str();
Double_t masses2[n2]; 
Double_t limits2[n2]; 
for (int i=0; i<n2; i++) myStream02 >> masses2[i] >> limits2[i];
Double_t realLimits2[n2];
for (i=0; i<n2; i++) realLimits2[i] = limits2[i]/efficiencies[i];


// ************  3  **************

string name3 = "negativeResults_3_0.txt";
ifstream myStream3 = name3.c_str();
int const n3 = lineNumber(myStream3);
ifstream myStream03 = name3.c_str();
Double_t masses3[n3]; 
Double_t limits3[n3]; 
for (int i=0; i<n3; i++) myStream03 >> masses3[i] >> limits3[i];
Double_t realLimits3[n3];
for (i=0; i<n3; i++) realLimits3[i] = limits3[i]/efficiencies[i];


// ************  4  **************

string name4 = "negativeResults_4_0.txt";
ifstream myStream4 = name4.c_str();
int const n4 = lineNumber(myStream4);
ifstream myStream04 = name4.c_str();
Double_t masses4[n4]; 
Double_t limits4[n4]; 
for (int i=0; i<n4; i++) myStream04 >> masses4[i] >> limits4[i];
Double_t realLimits4[n4];
for (i=0; i<n4; i++) realLimits4[i] = limits4[i]/efficiencies[i];


/*
// ************  5  **************

string name5 = "negativeResults_5_0.txt";
ifstream myStream5 = name5.c_str();
int const n5 = lineNumber(myStream5);
ifstream myStream05 = name5.c_str();
Double_t masses5[n5]; 
Double_t limits5[n5]; 
for (int i=0; i<n5; i++) myStream05 >> masses5[i] >> limits5[i];
Double_t realLimits5[n5];
for (i=0; i<n5; i++) realLimits5[i] = limits5[i]/efficiencies[i];
*/

// ************  6  **************

string name6 = "negativeResults_6_0.txt";
ifstream myStream6 = name6.c_str();
int const n6 = lineNumber(myStream6);
ifstream myStream06 = name6.c_str();
Double_t masses6[n6]; 
Double_t limits6[n6]; 
for (int i=0; i<n6; i++) myStream06 >> masses6[i] >> limits6[i];
Double_t realLimits6[n6];
for (i=0; i<n6; i++) realLimits6[i] = limits6[i]/efficiencies[i];


// ************  11  **************

string name11 = "negativeResults_11_0.txt";
ifstream myStream11 = name11.c_str();
int const n11 = lineNumber(myStream11);
ifstream myStream011 = name11.c_str();
Double_t masses11[n11]; 
Double_t limits11[n11]; 
for (int i=0; i<n11; i++) myStream011 >> masses11[i] >> limits11[i];
Double_t realLimits11[n11];
for (i=0; i<n11; i++) realLimits11[i] = limits11[i]/efficiencies[i];


// ************  12  **************

string name12 = "negativeResults_12_0.txt";
ifstream myStream12 = name12.c_str();
int const n12 = lineNumber(myStream12);
ifstream myStream012 = name12.c_str();
Double_t masses12[n12]; 
Double_t limits12[n12]; 
for (int i=0; i<n12; i++) myStream012 >> masses12[i] >> limits12[i];
Double_t realLimits12[n12];
for (i=0; i<n12; i++) realLimits12[i] = limits12[i]/efficiencies[i];


// ************  13  **************

string name13 = "negativeResults_13_0.txt";
ifstream myStream13 = name13.c_str();
int const n13 = lineNumber(myStream13);
ifstream myStream013 = name13.c_str();
Double_t masses13[n13]; 
Double_t limits13[n13]; 
for (int i=0; i<n13; i++) myStream013 >> masses13[i] >> limits13[i];
Double_t realLimits13[n13];
for (i=0; i<n13; i++) realLimits13[i] = limits13[i]/efficiencies[i];


// ************  14  **************

string name14 = "negativeResults_14_0.txt";
ifstream myStream14 = name14.c_str();
int const n14 = lineNumber(myStream14);
ifstream myStream014 = name14.c_str();
Double_t masses14[n14]; 
Double_t limits14[n14]; 
for (int i=0; i<n14; i++) myStream014 >> masses14[i] >> limits14[i];
Double_t realLimits14[n14];
for (i=0; i<n14; i++) realLimits14[i] = limits14[i]/efficiencies[i];



//Definition af a table a the averrage
Double_t averrage[n0];
for (i=0; i<n0; i++) averrage[i] = (realLimits0[i] + realLimits13[i] + realLimits14[i])/3;



TCanvas *c1 = new TCanvas("c1","Two Graphs");

TGraph *gr11 = new TGraph (n11, masses11, realLimits11);
TGraph *Averrage = new TGraph(n0, masses0, averrage);

Averrage->SetLineColor(4);
Averrage->Draw("AC*");

gr11->SetLineWidth(3);
gr11->SetMarkerStyle(21);
gr11->SetLineColor(2);

gr11->Draw("CP");


TFile plot1("negativeAverrage0+13+14=11.root","recreate");

c1->Write();

c1->SaveAs("negativeAverrage0+13+14=11.png");


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

}
