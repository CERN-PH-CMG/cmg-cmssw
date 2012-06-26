#include "CMGTools/HtoZZ2l2nu/interface/EfficiencyMap.h"
#include <string>
#include <sstream>

using std::string;
using std::stringstream;

Efficiency::Efficiency(int nptbins, const double * ptbins,
		int netabins, const double * etabins,
		const double * vals) : nPtBins(nptbins), nEtaBins(netabins) {

	ptBins = new double[nPtBins + 1];
	for (int i = 0; i < nPtBins + 1; ++i)
		ptBins[i] = ptbins[i];

	etaBins = new double[nEtaBins + 1];
	for (int i = 0; i < nEtaBins + 1; ++i)
		etaBins[i] = etabins[i];

	int nBins = nPtBins * nEtaBins;
	values = new double[nBins];
	for (int i = 0; i < nBins; ++i)
		values[i] = vals[i];
}

Efficiency::~Efficiency() {
	delete ptBins;
	delete etaBins;
	delete values;
}

double Efficiency::getEfficiency(double pt, double eta) const {
	int ptBin = -1;
	for (int i = 0; i < nPtBins; ++i) {
		if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
			ptBin = i;
			break;
		}
	}
	if (ptBin == -1 && pt >= ptBins[nPtBins])
		ptBin = nPtBins - 1;

	int etaBin = -1;
	for (int i = 0; i < nEtaBins; ++i) {
		if (eta >= etaBins[i] && eta < etaBins[i + 1]) {
			etaBin = i;
			break;
		}
	}

	if (ptBin < 0 || etaBin < 0) {
		stringstream ss;
		ss << "ERROR: Can't calculate efficiency for given pt (" << pt << ") and eta (" << eta << ")!";
		throw ss.str();
	}

	return values[etaBin * nPtBins + ptBin];
}


double getEfficiency(const Efficiency & eff2011A, const Efficiency & eff2011B, double pt, double eta) {
	return ( lumi2011A / lumi2011 ) * eff2011A.getEfficiency(pt, eta)
		+ ( lumi2011B / lumi2011 ) * eff2011B.getEfficiency(pt, eta);
}

double muonRecoScaleFactor(double pt, double eta) {
	return getEfficiency(muonRecoScaleFactors2011A, muonRecoScaleFactors2011B, pt, eta);
}

double muonIdScaleFactor(double pt, double eta) {
	return getEfficiency(muonIdScaleFactors2011A, muonIdScaleFactors2011B, pt, eta);
}

double muonIsolScaleFactor(double pt, double eta) {
	return getEfficiency(muonIsolScaleFactors2011A, muonIsolScaleFactors2011B, pt, eta);
}

double muonScaleFactor(double pt, double eta,int era) {
  switch(era)
    {
    case 2012: 
      return 1.0; 
      break;
    default : 
      return muonRecoScaleFactor(pt, eta) *  muonIdScaleFactor(pt, eta) *  muonIsolScaleFactor(pt, eta);
      break;
    }
  return 1.0;
}

double electronRecoScaleFactor(double pt, double eta) {
	return getEfficiency(electronRecoScaleFactors2011A, electronRecoScaleFactors2011B, pt, eta);
}

double electronIdScaleFactor(double pt, double eta) {
	return getEfficiency(electronIdScaleFactors2011A, electronIdScaleFactors2011B, pt, eta);
}

double electronIsolScaleFactor(double pt, double eta) {
	return getEfficiency(electronIsolScaleFactors2011A, electronIsolScaleFactors2011B, pt, eta);
}

double electronScaleFactor(double pt, double eta,int era) {
  switch(era)
    {
    case 2012:
      return 1;
      break;
    default:
      return electronRecoScaleFactor(pt, eta) *  electronIdScaleFactor(pt, eta) *  electronIsolScaleFactor(pt, eta);
      break;
    }
  return 1;
}

double muonTriggerEfficiency(double pt, double eta, int era) {
  switch(era)
    {
    case 2011:
      return ( lumiDoubleMu7 / lumi2011 ) * hltDoubleMu7.getEfficiency(pt, eta)
	+ ( (lumi2011A - lumiDoubleMu7) / lumi2011 ) * hltMu13Mu8_2011A.getEfficiency(pt, eta)
	+ ( (lumi2011B - lumiMu17_Mu8) / lumi2011 ) * hltMu13Mu8_2011B.getEfficiency(pt, eta)
	+ ( lumiMu17_Mu8 / lumi2011 ) * hltMu17Mu8.getEfficiency(pt, eta);
      break;
    case 2012:
      return 0.965;
      break;
    }
  return 1;
}

double electronTriggerEfficiency(double pt, double eta,int era) {
  switch(era)
    {
    case 2012:
      return 0.98; 
      break;
    case 2011:
      return 0.96;  //use top dileptons estimate cf. TOP-11-005
      //return 1.0; // fake
      break;
    }
  return 1.0;
}
