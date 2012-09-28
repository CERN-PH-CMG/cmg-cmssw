#ifndef IndirectTauVeto_h
#define IndirectTauVeto_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

double getLTpt(const pat::Jet* myjet, unsigned int type) ;
unsigned int getCHMdr(const pat::Jet* myjet, double DR, double pt_);
double getLRM(const pat::Jet* myjet);

bool tauVeto(const std::vector<const pat::Jet*> &  myNonBPurJet, double met, double metPhi);


#endif
