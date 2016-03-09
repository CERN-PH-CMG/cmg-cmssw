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

void fit_2H(string filename = "likelihoods.txt", string results_filename = "results.txt", bool recreate = false)
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
	infile.close();
	
	const double central_mass = 91188;
	TF1* fun = new TF1("fun","[0] + pow((x-[1])/[2], 2)");
	fun->SetParameters(0, central_mass, 30);
	g->Fit(fun, "MQ");

	TF1* fitted = g->GetFunction("fun");
	double mass = fitted->GetParameter(1);
	double uncert = fitted->GetParameter(2);
	
	ios_base::openmode mode = ios_base::app;
	if (recreate) mode = ios_base::out;
	ofstream outfile(results_filename.c_str(), mode);
	outfile << filename << endl;
	outfile << "Mass: " << mass << endl;
	outfile << "Uncert.: " << uncert << endl << endl;
	outfile.close();
}

int main(int, char**)
{
	fit_2H();
	return 0;
}
