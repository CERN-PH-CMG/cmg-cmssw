#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "TMath.h"
#include "TLorentzVector.h"


LorentzVector min(const LorentzVector& a, const LorentzVector& b){
   if(a.pt()<=b.pt())return a;
   return b;
}


std::string toLatexRounded(double value, double error, double systError)
{
  using namespace std;
  if(value==0.0 && error==0.0)return string("");

  double power = floor(log10(value));
  if(power<=-3)     {power=power+3;
  }else if(power>=2){power=power-2;
  }else             {power=0;}

   value = value / pow(10,power);
   error = error / pow(10,power);
   if(systError>=0)systError = systError / pow(10,power);
   int ValueFloating;
   if(systError<0){
      ValueFloating = 1 + std::max(-1*log10(error),0.0);
   }else{
      ValueFloating = 1 + std::max(-1*log10(systError), std::max(-1*log10(error),0.0));
   }
   int ErrorFloating = ValueFloating;

   char tmpchar[255];
   if(power!=0){
     if(systError<0){
        sprintf(tmpchar,"$(%.*f\\pm%.*f)\\times 10^{%g}$",ValueFloating,value,ErrorFloating,error,power);
     }else{
        sprintf(tmpchar,"$(%.*f\\pm%.*f\\pm%.*f)\\times 10^{%g}$",ValueFloating,value,ErrorFloating,error,ErrorFloating,systError,power);
     }

   }else{
     if(systError<0){
        sprintf(tmpchar,"$%.*f\\pm%.*f$",ValueFloating,value,ErrorFloating,error);
     }else{
        sprintf(tmpchar,"$%.*f\\pm%.*f\\pm%.*f$",ValueFloating,value,ErrorFloating,error,ErrorFloating,systError);
     }
   }
   return string(tmpchar);
}


//
PuShifter_t getPUshifters(std::vector< float > &Lumi_distr, float puUnc)
{
  Int_t NBins = Lumi_distr.size();
  TH1F *pu=new TH1F("putmp","",NBins,-0.5,float(NBins)-0.5);
  TH1F *puup=(TH1F *)pu->Clone("puuptmp");
  TH1F *pudown=(TH1F *)pu->Clone("pudowntmp");
  for(size_t i=0; i<Lumi_distr.size(); i++)  pu->SetBinContent(i+1,Lumi_distr[i]);

  for(int ibin=1; ibin<=pu->GetXaxis()->GetNbins(); ibin++)
    {
      Double_t xval=pu->GetBinCenter(ibin);
      TGraph *gr = new TGraph;
      for(int ishift=-3; ishift<3; ishift++)
	{
	  if(ibin+ishift<0) continue;
	  if(ibin+ishift>pu->GetXaxis()->GetNbins()) continue;
	  
	  gr->SetPoint(gr->GetN(),xval+ishift,pu->GetBinContent(ibin+ishift));
	}
      if(gr->GetN()>1)
	{
	  Double_t newval(gr->Eval(xval*(1+puUnc)));
	  pudown->SetBinContent(ibin,newval>0?newval:0.0);
	  newval=gr->Eval(xval*(1-puUnc));
	  puup->SetBinContent(ibin,newval>0?newval:0.0);
	}
      delete gr;
    }
  std::cout << pu->GetMean() << " " << puup->GetMean() << " " << pudown->GetMean() << std::endl;
  std::cout << "getPUshifts will shift average PU by " << puup->GetMean()-pu->GetMean() << " / " << pudown->GetMean()-pu->GetMean() << std::endl; 
  
  puup->Divide(pu);    TGraph *puupWgt = new TGraph(puup);
  pudown->Divide(pu);  TGraph *pudownWgt = new TGraph(pudown);
  delete puup;
  delete pudown;  
  delete pu;
  
  PuShifter_t res(2);
  res[PUDOWN] = pudownWgt;
  res[PUUP]   = puupWgt;
  return res;
}

//
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

