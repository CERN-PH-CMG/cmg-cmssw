
// Program to plot a single curve in a graph with legend

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


void plotSingleUpperlim () {



//opening the files
string name1 = "results_4_0_aggressiveFit.txt";
string name11 = "efficiency.txt";
ifstream myStream1 = name1.c_str();
ifstream myStream11 = name11.c_str();


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

//definition of 2 other tables with limit/efficiencies
Double_t realLimits1[n1];
for (i=0; i<n1; i++) {realLimits1[i] = limits1[i]/efficiencies1[i];}


/*
//check that tables are well-filled

for (i=0; i<n1; i++) {cout << masses1[i] << endl;}
for (i=0; i<n1; i++) {cout << limits1[i] << endl;}
for (i=0; i<n1; i++) {cout << efficiencies1[i] << endl;}
for (i=0; i<n1; i++) {cout << realLimits1[i] << endl;}
*/



//plot graph 

TCanvas *c0 = new TCanvas("c0");

TH1F* PLOTTER  = new TH1F("", "", 1, 400, 2100); // bounds of the x-axis
PLOTTER->SetMaximum(0.06); // bounds of the y-axis
PLOTTER->SetMinimum(0);
PLOTTER->SetTitle("Exclusion Limits VS Mass; Mass (GeV); Limit"); 
PLOTTER->Draw();
PLOTTER->SetStats(kFALSE);

TGraph *gr1 = new TGraph (n1, masses1, realLimits1);
gr1->SetLineWidth(2);
gr1->SetLineColor(4);
gr1->Draw("SAMEC*");

// legend
leg = new TLegend(0.35,0.8,0.89,0.89);
//   leg->SetHeader("Legend");
   leg->AddEntry(gr1,"JER systematic errors","l");
   leg->Draw();

// another box with extra text
pt = new TPaveText(0.69,0.93,0.89,0.99, "NDC");
pt->SetFillColor(0); // text is black on white
pt->SetTextSize(0.04); 
pt->SetTextAlign(12);
text = pt->AddText("Aggressive Fit");
pt->Draw();


string name("aggressiveUpperlimit4");

string nameroot = name + ".root";
string namepng = name + ".png";
string namepdf = name + ".pdf";

TFile plot0(nameroot.c_str(),"recreate");

c0->Write();
// saving the graph in different format
c0->SaveAs(namepng.c_str());
c0->SaveAs(namepdf.c_str());

}
