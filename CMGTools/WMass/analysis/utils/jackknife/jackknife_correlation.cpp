#include <iostream>
#include <fstream>
#include <cmath>
// #include <ctime>

// #include "TH1.h"
// #include "TH2.h"
// #include "TH3.h"
// #include "TCanvas.h"
// #include "TRandom3.h"
// #include "TFile.h"
// #include "TGraph.h"
// #include "TString.h"

using namespace std;


/* Base offsets (why, why?)
 * 0.028	36
 * 0.038	32
 * 0.032	41
 * 0.033	41
 */

int jackknife_correlation(const int nfiles)
{
	const int fitvars = 4;
	const double offsets_base[fitvars] = {0.028, 0.038, 0.032, 0.033};
	const string fitvar_str[fitvars] = {"Pt", "Mt", "MET", "MtLin"};
	
	string folder_basepath = "JobOutputs/jackknife_";
	
	double covmat[fitvars][fitvars];
	
	for (int filenum=1; filenum<nfiles; ++filenum) {
		string filepath = folder_basepath + to_string(filenum) + "/fit_results.log";
		ifstream infile = ifstream(filepath);
		infile.ignore(1024, '\n');
		double offsets[fitvars];
		for (int off = 0; off < fitvars; off++) {
			infile >> offsets[off];
			offsets[off] -= offsets_base[off];
			infile.ignore(1024, '\n');
		}
		for (int i=0; i<fitvars; ++i) {
			for (int j=0; j<fitvars ; j++) {
				double newterm = offsets[i]*offsets[j];
				covmat[i][j] += newterm;
			}
		}
	}
	double sigmas[fitvars];
	for (int i=0; i<fitvars; ++i) {
		sigmas[i] = sqrt(covmat[i][i]/nfiles);
	}

	double corrmat[fitvars][fitvars];

	cout << '\t';
	for (int i=0; i<fitvars; ++i) cout << fitvar_str[i] << '\t';
	cout << endl;
	for (int i=0; i<fitvars; ++i) {
		cout << fitvar_str[i] << '\t';
		for (int j=0; j<fitvars ; j++) {
			corrmat[i][j] = covmat[i][j]/nfiles/sigmas[i]/sigmas[j];
			printf("%.4f\t", corrmat[i][j]);;
		}
		cout << endl;
	}
	return 0;
}

int main(int, char**)
{
	return jackknife_correlation(2080);
}
