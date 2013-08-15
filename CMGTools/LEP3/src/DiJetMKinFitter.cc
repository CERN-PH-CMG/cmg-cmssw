#include "CMGTools/LEP3/interface/DiJetMKinFitter.h"
#include "PhysicsTools/KinFitter/interface/TFitConstraintM.h"
#include "PhysicsTools/KinFitter/interface/TFitParticleEtEtaPhi.h"

#include <TLorentzVector.h>
#include <TString.h>
#include <iostream>

using namespace std;
typedef reco::Candidate::LorentzVector LorentzVector;


DiJetMKinFitter::DiJetMKinFitter( const char* name, const char* title, double mass ) : TKinFitter( TString(name), TString(title) ) {

  name_ = name;
  title_ = title;
  mass_ = mass;

}

std::pair<TLorentzVector,TLorentzVector> DiJetMKinFitter::fit(LorentzVector jet1, LorentzVector jet2 ) {

  this->reset();

  TMatrixD m_jet1(3,3);
  TMatrixD m_jet2(3,3);

  TLorentzVector tjet1 = TLorentzVector(jet1.px(),jet1.py(),jet1.pz(),jet1.energy());
  TLorentzVector tjet2 = TLorentzVector(jet2.px(),jet2.py(),jet2.pz(),jet2.energy());

  m_jet1(0,0) = this->ErrEt (tjet1.Et(), tjet1.Eta()); // et
  m_jet1(1,1) = this->ErrEta(tjet1.Et(), tjet1.Eta()); // eta
  m_jet1(2,2) = this->ErrPhi(tjet1.Et(), tjet1.Eta()); // phi
  m_jet2(0,0) = this->ErrEt (tjet2.Et(), tjet2.Eta()); // et
  m_jet2(1,1) = this->ErrEta(tjet2.Et(), tjet2.Eta()); // eta
  m_jet2(2,2) = this->ErrPhi(tjet2.Et(), tjet2.Eta()); // phi

  TFitParticleEtEtaPhi *fitJet1 = new TFitParticleEtEtaPhi( "Jet1", "Jet1", &tjet1, &m_jet1 );
  TFitParticleEtEtaPhi *fitJet2 = new TFitParticleEtEtaPhi( "Jet2", "Jet2", &tjet2, &m_jet2 );
  
  TFitConstraintM *mCons_jets = new TFitConstraintM( "ZMassConstraint_jets", "ZMass-Constraint", 0, 0 , mass_);
  mCons_jets->addParticles1( fitJet1, fitJet2 );

  
  this->addMeasParticle( fitJet1 );
  this->addMeasParticle( fitJet2 );
  this->addConstraint( mCons_jets );
    

  //Set convergence criteria
  this->setMaxNbIter( 30 ); //number of maximal iterations
  this->setMaxDeltaS( 1e-2 ); //max Delta Chi2
  this->setMaxF( 1e-1 ); //max sum of constraints
  this->setVerbosity(0); //verbosity level

  //Perform the fit
  TKinFitter::fit();


  TLorentzVector tjet1_kinfit(*fitJet1->getCurr4Vec());
  TLorentzVector tjet2_kinfit(*fitJet2->getCurr4Vec());

  //LorentzVector jet1_kinfit = LorentzVector(tjet1_kinfit.Px(),tjet1_kinfit.Py(),tjet1_kinfit.Pz(),tjet1_kinfit.Energy());
  //LorentzVector jet2_kinfit = LorentzVector(tjet2_kinfit.Px(),tjet2_kinfit.Py(),tjet2_kinfit.Pz(),tjet2_kinfit.Energy());

  std::pair<TLorentzVector,TLorentzVector> jetpair;
  jetpair.first  = tjet1_kinfit;
  jetpair.second = tjet2_kinfit;

  delete fitJet1; delete fitJet2; delete mCons_jets;

  return jetpair;



}


double DiJetMKinFitter::getChi2( ) {
  
  //Get the Chi2
  double chi2 =  TKinFitter::getS();
  
  return chi2;
}



// pfjet resolutions. taken from AN-2010-371
double DiJetMKinFitter::ErrEt( double Et, double Eta) {
  
  double InvPerr2;

  double N=0, S=0, C=0, m=0;
  if(fabs(Eta) < 0.5 ) {
    N = 3.96859;
    S = 0.18348;
    C = 0.;
    m = 0.62627;
  } else if( fabs(Eta) < 1. ) {
    N = 3.55226;
    S = 0.24026;
    C = 0.;
    m = 0.52571;
  } else if( fabs(Eta) < 1.5 ) {
    N = 4.54826;
    S = 0.22652;
    C = 0.;
    m = 0.58963;
  } else if( fabs(Eta) < 2. ) {
    N = 4.62622;
    S = 0.23664;
    C = 0.;
    m = 0.48738;
  } else if( fabs(Eta) < 2.5 ) {
    N = 2.53324;
    S = 0.34306;
    C = 0.;
    m = 0.28662;
  } else if( fabs(Eta) < 3. ) {
    N = -3.33814;
    S = 0.73360;
    C = 0.;
    m = 0.08264;
  } else if( fabs(Eta) < 5. ) {
    N = 2.95397;
    S = 0.11619;
    C = 0.;
    m = 0.96086;
  }

  // this is the absolute resolution (squared), not sigma(pt)/pt
  // so have to multiply by pt^2, thats why m+1 instead of m-1
  InvPerr2 =  (N * fabs(N) ) + (S * S) * pow(Et, m+1) + (C * C) * Et * Et ;


  return InvPerr2;

}



//pfjet position resolutions taken from AN 2010-104
double DiJetMKinFitter::ErrEta( double Et, double Eta) {

  double InvPerr2;

  double a=0,b=0,c=0,d=0,e=0;
  if( fabs(Eta) < 0.5 ) {
    a = 487.702;
    b = 1.72868;
    c = 0.0297405;
    d = -487.197;
    e = -1.16389;
  } else if( fabs(Eta) < 1.0 ) {
    a = 277.114;
    b = 1.31746;
    c = 0.0232343;
    d = -276.588;
    e = -1.07289;
  } else if( fabs(Eta) < 1.5 ) {
    a = 19.7603;
    b = 0.406775;
    c = 0.0056006;
    d = -19.1144;
    e = -1.24005;
  } else if( fabs(Eta) < 2.0 ) {
    a = 41.55;
    b = 0.556349;
    c = 0.0094941;
    d = -40.8018;
    e = -1.39179;
  } else if( fabs(Eta) < 2.5 ) {
    a = 0.833363;
    b = 0.0786743;
    c = 0.0036199;
    d = 0.0507317;
    e = -1.5492;
  } else if( fabs(Eta) < 3. ) {
    a = 3.4712;
    b = 0.383594;
    c = 2.6831e-7;
    d = -2.93791;
    e = -0.259687;
  } else {
    std::cout << "Not implemented Eta Err for eta > 3. Exiting." << std::endl;
    exit(1123);
  }
  
  InvPerr2 = sqrt(a*a/(Et * Et) + b*b/Et + c*c) + d/Et + e/pow(Et,1.5);
  InvPerr2 *= InvPerr2;

  return InvPerr2;

}


double DiJetMKinFitter::ErrPhi( double Et, double Eta) {

  double InvPerr2;

  double a=0,b=0,c=0,d=0,e=0;
  if( fabs(Eta) < 0.5 ) {
    a = 926.978;
    b = 2.52747;
    c = 0.0304001;
    d = -926.224;
    e = -1.94117;
  } else if( fabs(Eta) < 1.0 ) {
    a = 3.3251e-6;
    b = 0.063941;
    c = 0.0038759;
    d = 0.301932;
    e = -0.825352;
  } else if( fabs(Eta) < 1.5 ) {
    a = 0.38469;
    b = 0.0755727;
    c = 0.0044353;
    d = 0.453887;
    e = -1.8947;
  } else if( fabs(Eta) < 2.0 ) {
    a = 2.9200e-7;
    b = 0.0718389;
    c = 0.0038557;
    d = 0.403668;
    e = -0.62698;
  } else if( fabs(Eta) < 2.5 ) {
    a = 0.0033663;
    b = 0.0880209;
    c = 0.0023084;
    d = 0.214304;
    e = -0.416353;
  } else if( fabs(Eta) < 3. ) {
    a = 11.1957;
    b = 0.643236;
    c = 0.0071142;
    d = -10.7613;
    e = 0.280927;
  } else {
    std::cout << "Not implemented Phi Err for eta > 3. Exiting." << std::endl;
    exit(1123);
  }
  
  InvPerr2 = sqrt(a*a/(Et * Et) + b*b/Et + c*c) + d/Et + e/pow(Et,1.5);
  InvPerr2 *= InvPerr2;

  return InvPerr2;

}




