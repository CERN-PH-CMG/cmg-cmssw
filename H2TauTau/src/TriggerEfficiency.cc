#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"


double TriggerEfficiency::efficiency(double m, double m0, double sigma, double alpha, double n, double norm) const 
 { 
   const double sqrtPiOver2 = 1.2533141373;
   const double sqrt2 = 1.4142135624;

   double sig = fabs((double) sigma);
   
   double t = (m - m0)/sig ;
   
   if (alpha < 0.)
     t = -t;

   double absAlpha = fabs(alpha / sig);
   double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
   double b = absAlpha - n/absAlpha;

   if (a>=std::numeric_limits<double>::max()) return -1. ;

   double ApproxErf ;
   double arg = absAlpha / sqrt2 ;
   if (arg > 5.) ApproxErf = 1 ;
   else if (arg < -5.) ApproxErf = -1 ;
   else ApproxErf = erf(arg) ;

   double leftArea = (1 + ApproxErf) * sqrtPiOver2 ;
   double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
   double area = leftArea + rightArea;


   if ( t <= absAlpha ){
     arg = t / sqrt2 ;
     if (arg > 5.) ApproxErf = 1 ;
     else if (arg < -5.) ApproxErf = -1 ;
     else ApproxErf = erf(arg) ;
     return norm * (1 + ApproxErf) * sqrtPiOver2 / area ;
   }
   else{
     return norm * (leftArea +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area ;
   }
  
 } 


double TriggerEfficiency::ratio(double pt) const
{
  double r = 0. ;
  if (pt<15) return r ;

  double effdata = efficiency(pt,19.1016,2.54719,6.45551,34.9826,0.933025) ;//2011 data 0.832 fb-1
  double effMC = efficiency(pt,18.3632,1.05792,0.409811,4.28198,0.841393) ; //PU reweighting, Summer11_S4

  if (effMC>0.) r = effdata/effMC ;
  return r ;
}






