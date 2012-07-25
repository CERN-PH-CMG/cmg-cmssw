
// Program to plot the limits obtained with pseudo-experiences

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


void plotUpperlimPEs () {

int numPEs = 5;

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


//opening the files
string name1 = "results_0_50.txt";
string name11 = "efficiency.txt";
ifstream myStream1 = name1.c_str();
ifstream myStream11 = name11.c_str();



//definition of 3 tables for the results
int const n1 = lineNumber(myStream1);
ifstream myStream01 = name1.c_str();
ifstream myStream011 = name11.c_str();
Double_t masses1[n1]; //one for masses
Double_t limits1[n1]; //one for upperlimits 
Double_t efficiencies1[n1]; //one for efficiencies 



//filling the 3 tables
for (int i=0; i<n1; i++) {myStream01 >> masses1[i] >> limits1[i];}
for (int i=0; i<n1; i++) {myStream011 >> efficiencies1[i];}


//definition of 1 other table with limit/efficiencies
Double_t realLimits1[n1];
for (i=0; i<(n1/numPEs); i++)
  {
  for (j=0; j<numPEs; j++)
    {
    realLimits1[i*numPEs+j] = limits1[i*numPEs+j]*efficiencies1[j];
    }
  }



//check that tables are well-filled

//for (i=0; i<n1; i++) {cout << masses1[i] << endl;}
//for (i=0; i<n1; i++) {cout << limits1[i] << endl;}
//for (i=0; i<n1; i++) {cout << efficiencies1[i] << endl;}
//for (i=0; i<n1; i++) {cout << realLimits1[i] << endl;}



//plot twin-graph 
TGraph *gr1 = new TGraph (n1, masses1, realLimits1);
TGraph *gr2 = new TGraph (n2, masses2, realLimits2);
TCanvas *c0 = new TCanvas("c1","Two Graphs");


gr1->SetLineColor(4);
gr1->Draw("A*");

TFile plot0("plotUpperlimit0.root","recreate");

c0->Write();

c0->SaveAs("plotUpperlim0.png");

}
