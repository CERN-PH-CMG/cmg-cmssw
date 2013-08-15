#include "CMGTools/PFSim/plugins/PFSimFastJetProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>


using namespace std;
using namespace edm;


//TODO function copied from fastjet. remove or improve that. 
void print_jets (const fastjet::ClusterSequence & clust_seq, 
                 const vector<fastjet::PseudoJet> & jets) {

  // sort jets into increasing pt
  vector<fastjet::PseudoJet> sorted_jets = sorted_by_pt(jets);  

  // label the columns
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", 
         "phi", "pt", "n constituents");
  
  // print out the details for each jet
  for (unsigned int i = 0; i < sorted_jets.size(); i++) {
    int n_constituents = clust_seq.constituents(sorted_jets[i]).size();
    printf("%5u %15.8f %15.8f %15.8f %8u\n",
           i, sorted_jets[i].rap(), sorted_jets[i].phi(),
           sorted_jets[i].perp(), n_constituents);
  }

}

PFSimFastJetProducer::PFSimFastJetProducer(const edm::ParameterSet& iConfig) {
  
  particleSrc_ = iConfig.getParameter<InputTag>("particleSrc");

  verbose_ = 
    iConfig.getUntrackedParameter<bool>("verbose",false);
  jetPtThreshold_ = 
    iConfig.getParameter<double>("jetPtThreshold");


  produces< Jets > ();
}

PFSimFastJetProducer::~PFSimFastJetProducer() {}


void PFSimFastJetProducer::beginJob() {
  double Rparam = 0.5;
  fastjet::Strategy strategy = fastjet::Best;
  fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
  jet_def_ =  fastjet::JetDefinition(fastjet::antikt_algorithm, Rparam, recomb_scheme, strategy);
}


void PFSimFastJetProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  edm::Handle< Particles> ptcs;
  iEvent.getByLabel(particleSrc_, ptcs);

  // now translating simulated particles into types that can be stored in the EDM event
  auto_ptr< Jets > outPtr( new Jets );

  vector<fastjet::PseudoJet> input_particles(ptcs->size());
  for(unsigned i=0; i<ptcs->size(); ++i) {
    const Particle& ptc = ptcs->at(i);
    double px = ptc.px();
    double py = ptc.py();
    double pz = ptc.pz();
    double E =  ptc.energy();
    input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }

  fastjet::ClusterSequence clust_seq(input_particles, jet_def_);

  // extract the inclusive jets with pt over the threshold
  double ptmin = jetPtThreshold_; //TODO parameter
  vector<fastjet::PseudoJet> jets = clust_seq.inclusive_jets(ptmin);

  if(verbose_)
    cout << "inclusive jets with pt > "<< jetPtThreshold_ <<" GeV\n";
  vector<fastjet::PseudoJet> sorted_jets = sorted_by_pt(jets);  
  for (unsigned int i = 0; i < sorted_jets.size(); i++) {
    const fastjet::PseudoJet& jet = sorted_jets[i];
    reco::LeafCandidate::LorentzVector p4(jet.px(), jet.py(), jet.pz(), jet.E());
    float charge = 0.; //TODO deal with charge and pdgId
    outPtr->push_back( Particle(charge, p4) ); 
    if (verbose_) cout<<"\t"<<jet.pt()<<" "<<jet.eta()<<endl;
  }


  iEvent.put( outPtr  );
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFSimFastJetProducer);
