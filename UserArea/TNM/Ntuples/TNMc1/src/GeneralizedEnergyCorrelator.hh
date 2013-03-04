//  Generalized Energy Correlator Package
//  Version 0.1.1 (Dec 3, 2012)

#ifndef __GENERALIZEDENERGYCORRELATOR_HH__
#define __GENERALIZEDENERGYCORRELATOR_HH__

#include "fastjet/FunctionOfPseudoJet.hh"

#include <string>
#include <climits>


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh


enum ecmode {
   pT_R,
   E_Omega
};


class GeneralizedEnergyCorrelator : public FunctionOfPseudoJet<double> {

private:

   int _N;
   double _beta;
   ecmode _mode;

public:

   GeneralizedEnergyCorrelator(int N, double beta, ecmode mode = pT_R) : _N(N), _beta(beta), _mode(mode) {};
   
   double result(const PseudoJet& jet) const;

   double energy(const PseudoJet& jet) const {
      if (_mode == pT_R) {
         return jet.perp();
      }  else if (_mode == E_Omega) {
         return jet.e();
      } else {
         assert(false);
         return NAN;
      }
   
   }
   
   double angle(const PseudoJet& jet1,const PseudoJet& jet2) const {
      if (_mode == pT_R) {
         return jet1.delta_R(jet2);
      } else if (_mode == E_Omega) {
         // doesn't seem to be a fastjet built in for this
         double dot = jet1.px()*jet2.px() + jet1.py()*jet2.py() + jet1.pz()*jet2.pz();
         double norm1 = sqrt(jet1.px()*jet1.px() + jet1.py()*jet1.py() + jet1.pz()*jet1.pz());
         double norm2 = sqrt(jet2.px()*jet2.px() + jet2.py()*jet2.py() + jet2.pz()*jet2.pz());
         
         double costheta = dot/(norm1 * norm2);
         if (costheta > 1.0) costheta = 1.0; // Need to handle case of numerical overflow
         return acos(costheta);    
           
      } else {
         assert(false);
         return NAN;
      }
   }


};

inline double GeneralizedEnergyCorrelator::result(const PseudoJet& jet) const {

   std::vector<fastjet::PseudoJet> particles = jet.constituents();

   double answer = 0.0;

   if (_N == 0) {
      return 1.0;
   } else if (_N == 1) {
      for (unsigned int i = 0; i < particles.size(); i++) {
         answer += energy(particles[i]);
      }
   } else if (_N == 2) {
      for (unsigned int i = 0; i < particles.size(); i++) {
         for (unsigned int j = i; j < particles.size(); j++) {
            answer += energy(particles[i])
                        * energy(particles[j])
                        * pow(angle(particles[i],particles[j]), _beta);
         }
      }   
   
   } else if (_N == 3) {
      for (unsigned int i = 0; i < particles.size(); i++) {
         for (unsigned int j = i; j < particles.size(); j++) {
            for (unsigned int k = j; k < particles.size(); k++) {
               answer += energy(particles[i])
                        * energy(particles[j])
                        * energy(particles[k])
                        * pow(angle(particles[i],particles[j])
                              * angle(particles[i],particles[k])
                              * angle(particles[j],particles[k]), _beta);
            }
         }
      }
   } else if (_N == 4) {
      for (unsigned int i = 0; i < particles.size(); i++) {
         for (unsigned int j = i; j < particles.size(); j++) {
            for (unsigned int k = j; k < particles.size(); k++) {
               for (unsigned int l = k; l < particles.size(); l++) {
                  answer += energy(particles[i])
                        * energy(particles[j])
                        * energy(particles[k])
                        * energy(particles[l])
                        * pow(angle(particles[i],particles[j])
                           * angle(particles[i],particles[k])
                           * angle(particles[j],particles[k])
                           * angle(particles[i],particles[l])
                           * angle(particles[j],particles[l])
                           * angle(particles[k],particles[l]), _beta);
               }
            }
         }
      }

   
   } else {
      std::cerr << "GeneralizedEnergyCorrelator is only hard coded for N = 0,1,2,3,4"  << std::endl;
      assert(false);
   }

   return answer;

}


class GeneralizedEnergyCorrelatorRatio : public FunctionOfPseudoJet<double> {

private:

   int _N;
   double _beta;
   ecmode _mode;

public:

   GeneralizedEnergyCorrelatorRatio(int N, double beta, ecmode mode = pT_R) : _N(N), _beta(beta), _mode(mode) {};
   
   double result(const PseudoJet& jet) const;

};


inline double GeneralizedEnergyCorrelatorRatio::result(const PseudoJet& jet) const {

   double numerator = GeneralizedEnergyCorrelator(_N - 1, _beta, _mode).result(jet) * GeneralizedEnergyCorrelator(_N + 1, _beta, _mode).result(jet);
   double denominator = pow(GeneralizedEnergyCorrelator(_N, _beta, _mode).result(jet), 2.0);

   return numerator/denominator;

}



FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif  // __GENERALIZEDENERGYCORRELATOR_HH__
