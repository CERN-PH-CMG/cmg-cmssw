#include "CMGTools/DiJetHighMass/interface/DiJetHistograms.h"

void cmg::DiJetHistograms::defineHistograms(){

  using namespace std;

  if(!histosInitialized_){
   
    int nbins = 83;
    float BINS[84] = {
      1,    3,    6,    10,   16,   23,   31,   40,   50,   61, 
      74,   88,   103,  119,  137,  156,  176,  197,  220,  244, 
      270,  296,  325,  354,  386,  419,  453,  489,  526,  565, 
      606,  649,  693,  740,  788,  838,  890,  944,  1000, 1058, 
      1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687, 1770, 
      1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 
      2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 
      4337, 4509, 4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 
      6328, 6564, 6808, 7000
    };

    add1DHistogram( "E", ";E (GeV)", 100, 0., 500, file_);
    add1DHistogram( "pT", ";p_{T} (GeV/c)", 100, 0., 500, file_);
    add1DHistogram( "eta", ";#eta", 50, -5, 5, file_);
    add1DHistogram( "phi", ";#phi", 50, -3.2, 3.2, file_);
    add1DHistogram( "Mass", ";Mass (GeV)", nbins, BINS, file_);
    add1DHistogram( "MT", ";M_{T} (GeV)", 100, 0., 150, file_);
    
    histosInitialized_ = true;
  }
  
}

void cmg::DiJetHistograms::fill(const type& cand){
  fill1DHistogram( "E",  cand.energy() );
  fill1DHistogram( "pT", cand.pt() );
  fill1DHistogram( "eta", cand.eta() );
  fill1DHistogram( "phi", cand.phi() );
  fill1DHistogram( "Mass", cand.mass() );  
  fill1DHistogram( "MT", cand.mt() );  
}
