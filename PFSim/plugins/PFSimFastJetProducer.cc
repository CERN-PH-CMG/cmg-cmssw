#include "CMGTools/PFSim/plugins/PFSimFastJetProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>


using namespace std;
using namespace edm;


//TODO function copied from fastjet put that in the class, 
// and enable under debug flag
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


  produces< Jets > ();
}

PFSimFastJetProducer::~PFSimFastJetProducer() {}


void PFSimFastJetProducer::beginJob() {
  double Rparam = 0.5;
  fastjet::Strategy strategy = fastjet::Best;
  fastjet::RecombinationScheme recomb_scheme = fastjet::E_scheme;
  jet_def_ =  fastjet::JetDefinition(fastjet::kt_algorithm, Rparam, recomb_scheme, strategy);
}


void PFSimFastJetProducer::produce(Event& iEvent, 
				    const EventSetup& iSetup) {
  
  edm::Handle< Particles> ptcs;
  iEvent.getByLabel(particleSrc_, ptcs);

  // now translating simulated particles into types that can be stored in the EDM event
  auto_ptr< Jets > outPtr( new Jets );

  vector<fastjet::PseudoJet> input_particles(ptcs->size());
  for(unsigned i=0; i<ptcs->size(); ++i) {
    const cmg::SimpleParticle& ptc = ptcs->at(i);
    double px = ptc.px();
    double py = ptc.py();
    double pz = ptc.pz();
    double E =  ptc.energy();
    input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }

  fastjet::ClusterSequence clust_seq(input_particles, jet_def_);

  // extract the inclusive jets with pt > 5 GeV
  double ptmin = 50.0; //TODO parameter
  vector<fastjet::PseudoJet> inclusive_jets = clust_seq.inclusive_jets(ptmin);

  
  //TODO optional printout
  // print them out
  cout << "Printing inclusive jets with pt > "<< ptmin<<" GeV\n";
  cout << "---------------------------------------\n";
  print_jets(clust_seq, inclusive_jets);
  cout << endl;
 

  //TODO translate and write

  iEvent.put( outPtr  );
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFSimFastJetProducer);
