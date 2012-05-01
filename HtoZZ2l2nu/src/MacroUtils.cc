#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "TMath.h"
#include "TLorentzVector.h"


LorentzVector min(const LorentzVector& a, const LorentzVector& b){
   if(a.pt()<=b.pt())return a;
   return b;
}


std::string toLatexRounded(double value, double error)
{
  using namespace std;
  if(value==0.0 && error==0.0)return string("");
  
  double power = floor(log10(value));
  if(power<=-3)     {power=power+3;
  }else if(power>=2){power=power-2;
  }else             {power=0;}       
  
   value = value / pow(10,power);
   error = error / pow(10,power);
   int ValueFloating = 1+std::max(-1*log10(error),0.0);
   int ErrorFloating = ValueFloating;
   
   char tmpchar[255];
   if(power!=0){
     sprintf(tmpchar,"$(%.*f\\pm%.*f)\\times 10^{%g}$",ValueFloating,value,ErrorFloating,error,power);
   }else{
     sprintf(tmpchar,"$%.*f\\pm%.*f$",ValueFloating,value,ErrorFloating,error);
   }
   return string(tmpchar);
}


double weightVBF(std::string SampleName, double m_gen, double mass){
   bool isZZ = (SampleName.find("ZZ")!=std::string::npos) ;

   double decay_width = -1;

/*
   double WeightInt  = -1;
         if(m_gen == 130){    decay_width =   0.00487;   if(isZZ){WeightInt = 1.000001; }else{ WeightInt =0.999999; }
   }else if(m_gen == 140){    decay_width =   0.00812;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 150){    decay_width =   0.01730;   if(isZZ){WeightInt = 0.999996; }else{ WeightInt =1.000004; }
   }else if(m_gen == 160){    decay_width =   0.08290;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 170){    decay_width =   0.38000;   if(isZZ){WeightInt = 1.000359; }else{ WeightInt =1.000212; }
   }else if(m_gen == 180){    decay_width =   0.63000;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 190){    decay_width =   1.04000;   if(isZZ){WeightInt = 1.000000; }else{ WeightInt =1.000000; }
   }else if(m_gen == 200){    decay_width =   1.43000;   if(isZZ){WeightInt = 0.999749; }else{ WeightInt =0.999183; }
   }else if(m_gen == 250){    decay_width =   4.04000;   if(isZZ){WeightInt = 0.999749; }else{ WeightInt =0.999183; }
   }else if(m_gen == 300){    decay_width =   8.43000;   if(isZZ){WeightInt = 0.987308; }else{ WeightInt =0.989306; }
   }else if(m_gen == 350){    decay_width =  15.20000;   if(isZZ){WeightInt = 0.987308; }else{ WeightInt =0.989306; }
   }else if(m_gen == 400){    decay_width =  29.20000;   if(isZZ){WeightInt = 0.951238; }else{ WeightInt =0.950328; }
   }else if(m_gen == 450){    decay_width =  46.95000;   if(isZZ){WeightInt = 0.951238; }else{ WeightInt =0.950328; }
   }else if(m_gen == 500){    decay_width =  68.00000;   if(isZZ){WeightInt = 0.881467; }else{ WeightInt =0.883517; }
   }else if(m_gen == 550){    decay_width =  93.15000;   if(isZZ){WeightInt = 0.881467; }else{ WeightInt =0.883517; }
   }else if(m_gen == 600){    decay_width = 123.00000;   if(isZZ){WeightInt = 0.792347; }else{ WeightInt =0.807353; }
   }
*/

   double WeightInt  = -1;
         if(m_gen == 130){    decay_width =   0.00487; 
   }else if(m_gen == 140){    decay_width =   0.00812; 
   }else if(m_gen == 150){    decay_width =   0.01730; 
   }else if(m_gen == 160){    decay_width =   0.08290; 
   }else if(m_gen == 170){    decay_width =   0.38000; 
   }else if(m_gen == 180){    decay_width =   0.63000; 
   }else if(m_gen == 190){    decay_width =   1.04000; 
   }else if(m_gen == 200){    decay_width =   1.43000; 
   }else if(m_gen == 250){    decay_width =   4.04000; 
   }else if(m_gen == 300){    decay_width =   8.43000; 
   }else if(m_gen == 350){    decay_width =  15.20000; 
   }else if(m_gen == 400){    decay_width =  29.20000; 
   }else if(m_gen == 450){    decay_width =  46.95000; 
   }else if(m_gen == 500){    decay_width =  68.00000;
   }else if(m_gen == 550){    decay_width =  93.15000;
   }else if(m_gen == 600){    decay_width = 123.00000;
   }

   double s = pow(mass,2);
   double weight_BW = s/pow(m_gen,2) * (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width,2)) / (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width*s/(pow(m_gen,2)),2));
   double toReturn = weight_BW;
   if(toReturn<0){toReturn=1.0;  printf("weightVBF<0=%f\n",toReturn);}
   return toReturn;
}


//
std::vector<TH1 *> defineOptimizationHistos(int version)
{
  std::vector<TH1 *> optimHistos;
  
  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################
  std::vector<double> optim_Cuts1_met;
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=65;met<160;met+=5.0){
	 if(met>90 && int(met)%10!=0)continue;
         if(met>120 && int(met)%20!=0)continue;
         for(double mtmin_=150;mtmin_<500;mtmin_+=25){
            double mtmin = mtmin_;
            if(mtmin<=150)mtmin=0;
	    if(mtmin>350 && int(mtmin)%50!=0)continue;
            for(double mtmax=mtmin+100;mtmax<mtmin+350;mtmax+=25){
               if(mtmax>=mtmin+325)mtmax=3000;
               if(mtmin==0 && mtmax!=3000)continue;
	       if(mtmin>350 && int(mtmax)%50!=0)continue;
               if(mtmax-mtmin>200 && int(mtmax)%50!=0)continue;
               optim_Cuts1_met    .push_back(met);
               optim_Cuts1_mtmin  .push_back(mtmin);
               optim_Cuts1_mtmax  .push_back(mtmax);
            }
      }
   }
  
   //add last year cut
  optim_Cuts1_met.push_back( 70); optim_Cuts1_mtmin.push_back(229); optim_Cuts1_mtmax.push_back(258);
  optim_Cuts1_met.push_back( 77); optim_Cuts1_mtmin.push_back(245); optim_Cuts1_mtmax.push_back(293);
  optim_Cuts1_met.push_back( 84); optim_Cuts1_mtmin.push_back(260); optim_Cuts1_mtmax.push_back(328);
  optim_Cuts1_met.push_back( 91); optim_Cuts1_mtmin.push_back(276); optim_Cuts1_mtmax.push_back(364);
  optim_Cuts1_met.push_back( 98); optim_Cuts1_mtmin.push_back(292); optim_Cuts1_mtmax.push_back(399);
  optim_Cuts1_met.push_back(105); optim_Cuts1_mtmin.push_back(308); optim_Cuts1_mtmax.push_back(434);
  optim_Cuts1_met.push_back(112); optim_Cuts1_mtmin.push_back(323); optim_Cuts1_mtmax.push_back(470);
  optim_Cuts1_met.push_back(119); optim_Cuts1_mtmin.push_back(339); optim_Cuts1_mtmax.push_back(505);
  optim_Cuts1_met.push_back(126); optim_Cuts1_mtmin.push_back(355); optim_Cuts1_mtmax.push_back(540);
  optim_Cuts1_met.push_back(133); optim_Cuts1_mtmin.push_back(370); optim_Cuts1_mtmax.push_back(576);
  optim_Cuts1_met.push_back(140); optim_Cuts1_mtmin.push_back(386); optim_Cuts1_mtmax.push_back(611);
  optim_Cuts1_met.push_back(147); optim_Cuts1_mtmin.push_back(402); optim_Cuts1_mtmax.push_back(646);
  optim_Cuts1_met.push_back(154); optim_Cuts1_mtmin.push_back(417); optim_Cuts1_mtmax.push_back(682);
  optim_Cuts1_met.push_back(161); optim_Cuts1_mtmin.push_back(433); optim_Cuts1_mtmax.push_back(717);
  optim_Cuts1_met.push_back(168); optim_Cuts1_mtmin.push_back(449); optim_Cuts1_mtmax.push_back(752);
  
  TH1F* Hoptim_cuts1_met     =  new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ; 
  TH1F* Hoptim_cuts1_mtmin   =  new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ;
  TH1F* Hoptim_cuts1_mtmax   =  new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ;
  for(unsigned int index=0;index<optim_Cuts1_met.size();index++){
    Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
    Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
    Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
  }
  optimHistos.push_back(Hoptim_cuts1_met);
  optimHistos.push_back(Hoptim_cuts1_mtmin);
  optimHistos.push_back(Hoptim_cuts1_mtmax);

  return optimHistos;
}
