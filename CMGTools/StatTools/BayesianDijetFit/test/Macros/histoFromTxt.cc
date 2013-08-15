
// Creation of an histogramm from a .txt file


#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "Riostream.h"
#include "TMath.h"

#include <stdio.h>
using namespace std;



void dataHisto () 
{
// opening of the texte file
string name = "/afs/cern.ch/user/h/hprager/private/CMSSW_4_4_2_patch8/src/StatTools/BayesianDijetFit/data/ds_data_2011-nominal.txt";
ifstream stream = name.c_str();

// definition of the bins of the histogramm
Double_t bins[40] = {220, 244, 270, 296, 325, 354, 386, 419, 453, 500, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659};


TH1* h1 = new TH1F("h1", "Data", 39, bins);

Double_t x = 0;
string line;
int n = 0;


// filling the histogramm
while (getline(stream, line)) 
  {
  stream >> x;
  h1->Fill(x);
//  if (x >= 500) n++; //counting the number of event under 500 GeV
  }

//cout << "There are " << n << " events above 500 GeV." << endl;

h1->Draw();

h1->SaveAs("data.root");



}
