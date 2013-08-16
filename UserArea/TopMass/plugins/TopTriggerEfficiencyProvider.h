#ifndef TOPTRIGGERWEIGHTPROVIDER_H
#define TOPTRIGGERWEIGHTPROVIDER_H

/**                                                                                                                                                                        
 * \class TopTriggerWeightProvider                                                                                                                                   
 *                                                                                                                                                                         
 * \brief Abstract: Class to read the trigger efficiency weights and apply them on MC. Note that this class does not check whether you are running on Data or MC, this is the responsibility of the analyst.
 *                  In the class the various datataking periods have been taken into account. One should note however that also the cut selection changes during datataking, and this class assumes the analyst is running the correct cutset for each run period. The provided weights also assume that the analyst is comparing with data with the correct trigger bit, depending on the run period                 
 *                                                                                                                                                                         
 * \author Kelly Beernaert, Ghent University, v1 08/11/2012                                                                                                                
 */


#include <stdlib.h>
#include <cstring>
#include <string>
#include <cassert>
#include <sstream>
#include <stdio.h>
#include <TString.h>
#include <cstdlib>
#include <iostream>
#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TRandom2.h"
#include "TSystem.h"
#include "TVector.h"
#include "TObjArray.h"

class TopTriggerEfficiencyProvider{
 public:
  
  TopTriggerEfficiencyProvider();
  ~TopTriggerEfficiencyProvider();
  
  double get_weight(double lep_pt, double lep_eta, double jet_pt, double jet_eta, int npvertices, int njets, bool LepIsMuon, bool complete_dataset, double fractionA = 0., double fractionB = 0., double fractionC = 0., double fractionD = 0.);
  
 protected:
  double make_MuonArray(int x, int y, int par, int Run);    
  double make_ElectronArray(int x, int y, int par, int Run);
  double make_JetArray(int x, int y, int z, double fractionDatasets[4]);
  double GetMuonWeight(double pt, double eta, int npvertices, int njets, double fractionDatasets[4]);
  double GetElectronWeight(double pt, double eta, int npvertices, double fractionDatasets[4]);
  double GetJetWeight(double pt, double eta, int njets, double fractionDatasets[4]);
  double TurnOn(double x, double par[4]);
  double VFunction(int x, double par[2]);
};

#endif
