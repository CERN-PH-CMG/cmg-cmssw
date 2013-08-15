#ifndef EFFICIENCY_H
#define EFFICIENCY_H

class Efficiency {
	private :
		int nPtBins;
		int nEtaBins;
		double * ptBins;
		double * etaBins;
		double * values;
		Efficiency(const Efficiency &);
		Efficiency & operator=(const Efficiency &);
	public :
		Efficiency(int nPtBins, const double * ptBins,
				int nEtaBins, const double * etaBins,
				const double * values);
		~Efficiency();
		virtual double getEfficiency(double pt, double eta) const;
};

const double lumi2011 = 5051;
const double lumi2011A = 2312;
const double lumi2011B = 2739;
const double lumiDoubleMu7 = 216;
const double lumiMu13_Mu8 = 3950;
const double lumiMu17_Mu8 = 885;

const double ptBins[5] = {20, 30, 40, 60, 100};
const double triggerPtBins[5] = {20, 30, 40, 50, 100};
const double electronEtaBins[3] = {0.0, 1.5, 2.5};
const double muonEtaBins[3] = {0.0, 1.2, 2.4};
const double muonTriggerEtaBins[5] = {0.0, 0.8, 1.2, 2.1, 2.4};

const double DoubleElectron_Run2011A_Reco[8] = {1.002, 1.000, 1.000, 1.004, 0.994, 1.005, 1.003, 1.013};
const double DoubleElectron_Run2011B_Reco[8] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // fake
const double DoubleElectron_Run2011A_Id[8] = {0.996, 0.996, 0.995, 0.992, 1.000, 0.996, 1.001, 1.002};
const double DoubleElectron_Run2011B_Id[8] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // fake
const double DoubleElectron_Run2011A_Isol[8] = {0.989, 0.997, 0.999, 0.999, 1.001, 0.999, 1.000, 0.998};
const double DoubleElectron_Run2011B_Isol[8] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // fake

const double SingleMu_Run2011A_Reco[8] = {0.996, 0.998, 0.998, 0.997, 0.991, 0.997, 0.997, 0.990};
const double SingleMu_Run2011B_Reco[8] = {0.995, 0.996, 0.996, 0.976, 0.979, 0.976, 0.975, 0.967};
const double SingleMu_Run2011A_Id[8] = {0.999, 0.999, 0.998, 0.998, 0.997, 1.003, 0.995, 0.991};
const double SingleMu_Run2011B_Id[8] = {0.997, 0.998, 0.998, 0.999, 0.987, 0.990, 0.990, 0.987};
const double SingleMu_Run2011A_Isol[8] = {1.000, 1.000, 1.000, 1.001, 0.999, 1.001, 1.000, 1.001};
const double SingleMu_Run2011B_Isol[8] = {0.999, 1.000, 1.000, 1.001, 1.002, 1.002, 1.000, 1.000};
const double HLT_DoubleMu7_May10ReReco[16] = {0.9779, 0.9750, 0.9772, 0.9786, 0.9411, 0.9474, 0.9477, 0.9454,
												0.9586, 0.9577, 0.9584, 0.9540, 0.9341, 0.9253, 0.9118, 0.9224};	
const double HLT_Mu13_Mu8_2011A[16] = {0.9768, 0.9764, 0.9768, 0.9760, 0.9607, 0.9591, 0.9625, 0.9590,
										0.9568, 0.9565, 0.9561, 0.9539, 0.8830, 0.8943, 0.8986, 0.8931};
const double HLT_Mu13_Mu8_2011B[16] = {0.9748, 0.9742, 0.9739, 0.9752, 0.9572, 0.9549, 0.9556, 0.9559,
										0.9448, 0.9462, 0.9477, 0.9434, 0.8564, 0.8651, 0.8616, 0.8601};
const double HLT_Mu17_Mu8_2011B[16] = {0.9741, 0.9738, 0.9738, 0.9746, 0.9529, 0.9534, 0.9535, 0.9557,
										0.9432, 0.9448, 0.9458, 0.9425, 0.8511, 0.8629, 0.8596, 0.8574};

const static Efficiency electronRecoScaleFactors2011A(4, ptBins, 2, electronEtaBins, DoubleElectron_Run2011A_Reco);
const static Efficiency electronRecoScaleFactors2011B(4, ptBins, 2, electronEtaBins, DoubleElectron_Run2011B_Reco);
const static Efficiency electronIdScaleFactors2011A(4, ptBins, 2, electronEtaBins, DoubleElectron_Run2011A_Id);
const static Efficiency electronIdScaleFactors2011B(4, ptBins, 2, electronEtaBins, DoubleElectron_Run2011B_Id);
const static Efficiency electronIsolScaleFactors2011A(4, ptBins, 2, electronEtaBins, DoubleElectron_Run2011A_Isol);
const static Efficiency electronIsolScaleFactors2011B(4, ptBins, 2, electronEtaBins, DoubleElectron_Run2011B_Isol);

const static Efficiency muonRecoScaleFactors2011A(4, ptBins, 2, muonEtaBins, SingleMu_Run2011A_Reco);
const static Efficiency muonRecoScaleFactors2011B(4, ptBins, 2, muonEtaBins, SingleMu_Run2011B_Reco);
const static Efficiency muonIdScaleFactors2011A(4, ptBins, 2, muonEtaBins, SingleMu_Run2011A_Id);
const static Efficiency muonIdScaleFactors2011B(4, ptBins, 2, muonEtaBins, SingleMu_Run2011B_Id);
const static Efficiency muonIsolScaleFactors2011A(4, ptBins, 2, muonEtaBins, SingleMu_Run2011A_Isol);
const static Efficiency muonIsolScaleFactors2011B(4, ptBins, 2, muonEtaBins, SingleMu_Run2011B_Isol);

const static Efficiency hltDoubleMu7(4, triggerPtBins, 4, muonTriggerEtaBins, HLT_DoubleMu7_May10ReReco);
const static Efficiency hltMu13Mu8_2011A(4, triggerPtBins, 4, muonTriggerEtaBins, HLT_Mu13_Mu8_2011A);
const static Efficiency hltMu13Mu8_2011B(4, triggerPtBins, 4, muonTriggerEtaBins, HLT_Mu13_Mu8_2011B);
const static Efficiency hltMu17Mu8(4, triggerPtBins, 4, muonTriggerEtaBins, HLT_Mu17_Mu8_2011B);

double getEfficiency(const Efficiency & eff2011A, const Efficiency & eff2011B, double pt, double eta);

double electronRecoScaleFactor(double pt, double eta);
double electronIdScaleFactor(double pt, double eta);
double electronIsolScaleFactor(double pt, double eta);
double electronScaleFactor(double pt, double eta,int era=2011);
double electronTriggerEfficiency(double pt, double eta, int era=2011);

double muonRecoScaleFactor(double pt, double eta);
double muonIdScaleFactor(double pt, double eta);
double muonIsolScaleFactor(double pt, double eta);
double muonScaleFactor(double pt, double eta, int era=2011);
double muonTriggerEfficiency(double pt, double eta, int era=2011);

#endif // EFFICIENCY_H
