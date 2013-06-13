#include "CMGTools/PFSim/interface/Simulator.h"
#include "CMGTools/PFSim/interface/CMS.h"


using namespace std;


static const int PDGCacheMax = 50000;


PFSim::Simulator::Simulator(bool verbose) :
  firstEvent_(true),
  chargeP_( PDGCacheMax, 0 ),
  chargeM_( PDGCacheMax, 0 ),
  detector_( new CMS() ), 
  uniform_(0.0, 1.0),
  verbose_(verbose)
{}


void PFSim::Simulator::simulate( const HepMC::GenEvent& event, 
				 const ParticleDataTable& pdt ) {
  Particles particles(event.particles_size());
  for(HepMC::GenEvent::particle_const_iterator iter = event.particles_begin(); 
      iter!= event.particles_end(); ++iter) {
    const HepMC::GenParticle& inptc = **iter;
    float charge = 99.;
    particles.push_back( Particle(Particle::X, inptc.pdg_id(), charge, inptc.momentum() ) );
    particles.back().setStatus( inptc.status() );
  }
  return simulate( particles, pdt );
}



void PFSim::Simulator::simulate( const Particles& particles, 
				 const ParticleDataTable& pdt ) {


  if (firstEvent_) {
    cachePDT( pdt );
    firstEvent_ = false; 
  }

  simParticles_.clear();

  // loop over all particles
  for(unsigned i=0; i<particles.size(); ++i) {

    const Particle& inptc = particles[i];
    // cout << inptc.status() << " "<< inptc.pdg_id() << endl;
 
    if( inptc.status() != 1 ) continue; 

    Particle::Type type = Particle::X;
    int charge = chargeTimesThree( inptc.pdg_id() );    

    switch( abs( inptc.pdg_id() )  ) {
    case 12:
    case 14:
    case 16: // neutrinos
      // invisible particles (need to add other fancy stuff)
      continue; 
    case 11: // electrons
      type = Particle::e;
      break;
    case 13: // muons
      type = Particle::mu;
      break; 
    case 3312: //TODO Xi-, Xibar+
    case 3112: //TODO Sigma-, Sigmabar+
    case 3222: //TODO Sigma+, Sigmabar-
    case 211: // pi+-
    case 321: // K+-
    case 2212: // p
      type = Particle::h; 
      break;
    case 3322: //TODO Xi0, Xibar0
    case 3122: //TODO Lambda0
    case 130:  // KLong
    case 310:  //TODO KShort
    case 2112: // n
      type = Particle::h0; 
      break;
    case 22: // gamma
      type = Particle::gamma;
      break;
    default: // we don't know it, and decide to make a neutral hadron.
      cerr<<"unknown stable particle: "<<inptc.pdg_id()<<endl;
      type = Particle::h0; 
      break;      
    } 

    // if (!(type==Particle::mu && fabs(inptc.momentum().eta())>2.4)) continue;
    // if (!(type==Particle::mu)) continue;

    float resolution = detector_->resolution(type, inptc.momentum());

    std::normal_distribution<float> gaus(1.0, resolution);
    float smearFactor = gaus(generator_);
    HepMC::FourVector smearedMom( inptc.momentum().x()*smearFactor, 
				  inptc.momentum().y()*smearFactor,
				  inptc.momentum().z()*smearFactor,
				  inptc.momentum().t()*smearFactor );
    
    //TODO smear what? in other words should the mass change? 
    //TODO smear eta and phi
    //TODO eff and smear or smear and eff? 
   
    float efficiency = detector_->efficiency(type, smearedMom);

    bool accepted = false;
    if(efficiency> 1-1e-6)
      accepted = true; // we don't need to use a random number in this case (eff=1.)
    else if(efficiency<1e-6)
      accepted = false;
    else {
      float effscore = uniform_(generator_);
      if (effscore<=efficiency)
	accepted = true;
    }

    if (verbose_) {
      cout<<type<<" "<<inptc.pdg_id()<<endl;
      cout<<"\t"<<"E res = "<<resolution<<" E smear="<<smearFactor<<endl;
      cout<<"\t"<<"before  "<<inptc.momentum().perp()<<" "<<inptc.momentum().e()
	  <<" "<<inptc.momentum().eta()<<" "<<inptc.momentum().m()<<endl;
      cout<<"\t"<<"after   "<<smearedMom.perp()<<" "<<smearedMom.e()
	  <<" "<<smearedMom.eta()<<" "<<smearedMom.m()<<endl;
      cout<<"\t"<<"pass="<<accepted<<" eff "<<efficiency<<endl;
    }
    
    if(accepted) {
      //TODO attach model information to output particles, at least during debugging
      //TODO implement verbosity tools
      Particle aPart = Particle(type, inptc.pdg_id(), charge, smearedMom );
      simParticles_.push_back(aPart);   
    
      // fake neutral hadrons from upper fluctuations of the h+- energy deposit
      // in the calorimeters.
      
      if( type==Particle::h ) {
	float resolution = detector_->resolution(Particle::h0, 
						 inptc.momentum());	
	std::normal_distribution<float> gaus(1.0, resolution);
	float smearFactor = gaus(generator_);
	int nsigma = 1;
	float threshold = 1.0 + nsigma*resolution;
	if(verbose_)       
	  cout<<"\tFake h0: h scale="<<smearFactor<<" thresh="<<threshold<<endl; 
	if ( smearFactor > threshold ) {
	  float excess = smearFactor-1.0;
	  HepMC::FourVector h0Mom( inptc.momentum().x()* excess, 
				   inptc.momentum().y()* excess,
				   inptc.momentum().z()* excess,
				   inptc.momentum().t()* excess );
	  Particle aPart = Particle(Particle::h0, 0, 0, h0Mom );
	  simParticles_.push_back(aPart);   
	  if(verbose_) {
	    cout<<"\t\tcreating h0: E="<<h0Mom.e()<<", excess="<<excess<<" eta="<<h0Mom.eta()<<endl; 
	  }
	}
	//TODO also absorbe overlapping neutral hadrons. Certainly not as important.
      }
    } else { //not accepted
      
      // neutral particles are really not seen.
      if (type==Particle::gamma || type==Particle::h0)
	continue;

      // charged hadrons are seen as neutral hadrons

      if (type==Particle::h) {
	float resolution = detector_->resolution(Particle::h0, 
						 inptc.momentum());	
	std::normal_distribution<float> gaus(1.0, resolution);
	float smearFactor = gaus(generator_);
	HepMC::FourVector h0Mom( inptc.momentum().x()* smearFactor, 
				 inptc.momentum().y()* smearFactor,
				 inptc.momentum().z()* smearFactor,
				 inptc.momentum().t()* smearFactor );

	// now need to check if the h0 is accepted
	float efficiency = detector_->efficiency(Particle::h0, h0Mom);
	bool accepted = false;
	if(efficiency> 1-1e-6)
	  accepted = true; // we don't need to use a random number in this case (eff=1.)
	else if(efficiency<1e-6)
	  accepted = false;
	else {
	  float effscore = uniform_(generator_);
	  if (effscore<=efficiency)
	    accepted = true;
	}
	
	if (accepted) {
	  Particle aPart = Particle(Particle::h0, 0, 0, h0Mom );
	  simParticles_.push_back(aPart);   
	  if(verbose_) 
	    cout<<"\t\tunseen h, creating h0: E="<<h0Mom.e()<<" eta="<<h0Mom.eta()<<endl; 	
	} else if (verbose_)
	    cout<<"\t\tunseen h, NOT creating h0: E="<<h0Mom.e()<<" eta="<<h0Mom.eta()<<endl; 	      
      }

      //TODO electrons are seen as photons (?)

      //TODO muons... (depends)
    }
  }
}



int PFSim::Simulator::chargeTimesThree( int id ) const {
  if( std::abs( id ) < PDGCacheMax ) 
    return id > 0 ? chargeP_[ id ] : chargeM_[ - id ];
  map<int, int>::const_iterator f = chargeMap_.find( id );
  if ( f == chargeMap_.end() )  {
    return HepPDT::ParticleID(id).threeCharge();
  }
  return f->second;
}



void PFSim::Simulator::cachePDT( const ParticleDataTable& pdt) {
  for( HepPDT::ParticleDataTable::const_iterator p = pdt.begin(); 
       p != pdt.end(); ++ p ) {
    const HepPDT::ParticleID & id = p->first;
    int pdgId = id.pid(), apdgId = std::abs( pdgId );
    int q3 = id.threeCharge();
    if ( apdgId < PDGCacheMax && pdgId>0 ) {
      chargeP_[ apdgId ] = q3;
      chargeM_[ apdgId ] = -q3;
    } else if ( apdgId < PDGCacheMax ) {
      chargeP_[ apdgId ] = -q3;
      chargeM_[ apdgId ] = q3;
    } else {
      chargeMap_[ pdgId ] = q3;
      chargeMap_[ -pdgId ] = -q3;
    }
  }  
}

