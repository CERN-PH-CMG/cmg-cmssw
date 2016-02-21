#include <iostream>
#include <fstream>
// #include <cmath>
// #include <ctime>

// #include "TH1.h"
// #include "TH2.h"
// #include "TH3.h"
// #include "TCanvas.h"
// #include "TRandom3.h"
#include "TF1.h"
// #include "TFile.h"
#include "TGraph.h"
// #include "TString.h"

using namespace std;

int fit_2H(string filename = "likelihoods.txt")
{
	ifstream infile(filename.c_str());
	if (infile.fail()) {
		cout << "ERROR: Could not open " << filename << endl;
		return 1;
	}
	else cout << "Working on " << filename << endl;

	TGraph* g = new TGraph();

	for (int m=91138; m<=91238; m+=10) {
		double thisNLL;
		infile >> thisNLL;
		//cout << thisNLL << endl;
		g->SetPoint(g->GetN(), m, thisNLL);
	}
	
	const double central_mass = 91188;
	TF1* fun = new TF1("fun","[0] + pow((x-[1])/[2], 2)");
	fun->SetParameters(0, central_mass, 30);
	g->Fit(fun, "MQ");

	TF1* fitted = g->GetFunction("fun");
	double mass = fitted->GetParameter(1);
	double uncert = fitted->GetParameter(2);
	cout << "Mass: " << mass << endl;
	cout << "Uncert.: " << uncert << endl;
	
	return 0;
}

int main(int, char**)
{
	return fit_2H();
}
