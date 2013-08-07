// Program to plot in the same graph the limits obtained with two different programs

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
#include <string.h>
using namespace std;


void plotComparedUpperlim () {



//opening the files
string name1 = "results_0_0.txt";
string name11 = "efficiency.txt";
string name2 = "newResults_0_stephane.txt";
ifstream myStream1 = name1.c_str();
ifstream myStream10 = name11.c_str();
ifstream myStream2 = name2.c_str();



//function to calculate the number of lines in a file
int lineNumber(ifstream & myStream)
{
int n = 0;
string line;
while (getline(myStream, line)) {
//cout << line << endl;
n++;
}
return n;
}



//definition of 7 tables

//4 for Maxime's results
int const n1 = lineNumber(myStream1);
ifstream myStream01 = name1.c_str();
ifstream myStream010 = name11.c_str();
Double_t masses1[n1]; //one for masses
Double_t limits1[n1]; //one for upperlimits
Double_t efficiencies11[n1];
Double_t efficiencies12[n1]; //two for efficiencies

//and 3 for Stéphane's results
int const n2 = lineNumber(myStream2);
ifstream myStream02 = name2.c_str();
Double_t masses2[n2]; //one for masses
Double_t limits2[n2]; //one for upperlimits
Double_t efficiencies2[n2]; //one for efficiencies


//filling 6 of the tables
for (int i=0; i<n1; i++) {myStream01 >> masses1[i] >> limits1[i];}
for (int i=0; i<n1; i++) {myStream010 >> efficiencies11[i];}
for (int i=0; i<n2; i++) {myStream02 >> masses2[i] >> limits2[i] >> efficiencies2[i];}



//filling efficiencies12 with linear interpolations
for (int i=0; i<n1; i++)
  {
  if (masses1[i]<750) {efficiencies12[i]=0;}
  else if (masses1[i]>750 && masses1[i]<1000) {efficiencies12[i]= efficiencies2[0] + (efficiencies2[1] - efficiencies2[0])/(masses2[1] - masses2[0])*(masses1[i] - masses2[0]);}
  else if (masses1[i]==1000) {efficiencies12[i]=efficiencies2[1];}
  else if (masses1[i]>1000 && masses1[i]<1250) {efficiencies12[i]= efficiencies2[1] + (efficiencies2[2]-efficiencies2[1])/(masses2[2]-masses2[1])*(masses1[i]-masses2[1]);}
  else if (masses1[i]>1250 && masses1[i]<1500){efficiencies12[i]= efficiencies2[2] + (efficiencies2[3]-efficiencies2[2])/(masses2[3]-masses2[2])*(masses1[i]-masses2[2]);}
  else if (masses1[i]==1500) {efficiencies12[i]=efficiencies2[3];}
  else if (masses1[i]>1500) {efficiencies12[i]=0;}
  }


//definition of 1 other table with limits/efficiencies for Maxime's results (or 0 if we don't have the efficiency)
Double_t firstRealLimits1[n1];
for (i=0; i<n1; i++)
  {
  if (efficiencies12[i] != 0) {firstRealLimits1[i] = limits1[i]/efficiencies11[i]/efficiencies12[i];}
  else {firstRealLimits1[i] = 0;}
  }


//definition of a 2 tables with only real limits and associated masses
Double_t realMasses1[8];
Double_t realLimits1[8];
int n = 0;
for (i=0; i<n1; i++)
  {
  if (firstRealLimits1[i] != 0)
    {
    realLimits1[n] = firstRealLimits1[i];
    realMasses1[n] = masses1[i];
    n++;
    }
  }


/*
//check that tables are well-filled
for (i=0; i<n2; i++) {cout << masses2[i] << endl;}
cout << endl;
for (i=0; i<n2; i++) {cout << limits2[i] << endl;}
cout << endl;
for (i=0; i<n2; i++) {cout << efficiencies2[i] << endl;}
cout << endl;
for (i=0; i<n1; i++) {cout << masses1[i] << endl;}
cout << endl;
for (i=0; i<n1; i++) {cout << limits1[i] << endl;}
cout << endl;
for (i=0; i<n1; i++) {cout << efficiencies11[i] << endl;}
cout << endl;
for (i=0; i<n1; i++) {cout << efficiencies12[i] << endl;}
cout << endl;
for (i=0; i<8; i++) {cout << realLimits1[i] << endl;}
cout << endl;
*/


//plot twin-graph
TGraph *gr1 = new TGraph (8, realMasses1, realLimits1);
TGraph *gr2 = new TGraph (n2, masses2, limits2);

TH1F* PLOTTER  = new TH1F("PLOTTER", "", 1, 700, 1600);
PLOTTER->SetMaximum(1.5);
PLOTTER->SetMinimum(0);

PLOTTER->SetTitle("Exclusion Limits VS Mass; Mass; Limit");
PLOTTER->Draw();


gr1->SetLineColor(4);
gr1->Draw("SAMEC*");

gr2->SetLineWidth(3);
gr2->SetMarkerStyle(21);
gr2->SetLineColor(2);
gr2->Draw("SAMECP");


TFile plot0("newPlotUpperlimit0.root","recreate");

PLOTTER->Write();

PLOTTER->SaveAs("newPlotUpperlim0.png");

}
