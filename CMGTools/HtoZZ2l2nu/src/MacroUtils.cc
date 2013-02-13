#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TCanvas.h"

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
  puup->Scale(pu->Integral()/puup->Integral());
  pudown->Scale(pu->Integral()/pudown->Integral());
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
   }else if(m_gen == 650){    decay_width = 158.00000;
   }else if(m_gen == 700){    decay_width = 199.00000;
   }else if(m_gen == 750){    decay_width = 247.00000;
   }else if(m_gen == 800){    decay_width = 304.00000;
   }else if(m_gen == 850){    decay_width = 371.00000;
   }else if(m_gen == 900){    decay_width = 449.00000;
   }else if(m_gen == 950){    decay_width = 540.00000;
   }else if(m_gen ==1000){    decay_width = 647.00000;
   }

   double s = pow(mass,2);
   double weight_BW = s/pow(m_gen,2) * (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width,2)) / (pow(s-pow(m_gen,2),2)+ pow(m_gen*decay_width*s/(pow(m_gen,2)),2));
   double toReturn = weight_BW;
   if(toReturn<0){toReturn=1.0;  printf("weightVBF<0=%f\n",toReturn);}
   return toReturn;
}

//
double weightNarrowResonnance(std::string SampleName, double m_gen, double mass, double Cprime, double BRnew, TGraph* hLineShapeNominal, TF1 *decayProbPdf){
  if((Cprime<0 || BRnew<0) || (Cprime==0 && BRnew==0)) return 1.0;

   bool isZZ = (SampleName.find("ZZ")!=std::string::npos) ;

   double decay_width = -1;
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
   }else if(m_gen == 650){    decay_width = 158.00000;
   }else if(m_gen == 700){    decay_width = 199.00000;
   }else if(m_gen == 750){    decay_width = 247.00000;
   }else if(m_gen == 800){    decay_width = 304.00000;
   }else if(m_gen == 850){    decay_width = 371.00000;
   }else if(m_gen == 900){    decay_width = 449.00000;
   }else if(m_gen == 950){    decay_width = 540.00000;
   }else if(m_gen ==1000){    decay_width = 647.00000;
   }
	 
   double OverallXSectionScaleFactor = 1.0;//*hLineShapeNominal->Integral();
   decay_width = decay_width * pow(Cprime,2) / (1-BRnew);      
   //      OverallXSectionScaleFactor *= pow(Cprime,2) * (1-BRnew);

   //the CPS shape
   double weight_CPS = hLineShapeNominal->Eval(mass);

   //standard BW
   double toReturn(-1);
   if(decayProbPdf==0)
     {
       double weight_BW = TMath::BreitWigner(mass, m_gen, decay_width);
       toReturn = OverallXSectionScaleFactor * (weight_BW / weight_CPS);   
     }
   else
     {
       //other prob function where [0] is the mass of resonance and [1] is the new width
       // e.g. relativistic BW   
       //TF1 *f=new TF1("relbw","(2*sqrt(2)*[0]*[1]*sqrt(pow([0],2)*(pow([0],2)+pow([1],2)))/(TMath::Pi()*sqrt(pow([0],2)+sqrt(pow([0],2)*(pow([0],2)+pow([1],2))))))/(pow(pow(x,2)-pow([0],2),2)+pow([0]*[1],2))",0,2000);
       decayProbPdf->SetParameter(0,m_gen);
       decayProbPdf->SetParameter(1,decay_width);
       double weight_BW = decayProbPdf->Eval( mass );
       toReturn = OverallXSectionScaleFactor * ( weight_BW / weight_CPS);
     }

   //   printf("%E / %E = %E\n",weight_BW, hLineShapeNominal->Eval(mass), (weight_BW / hLineShapeNominal->Eval(mass)) );
   if(toReturn<0){toReturn=1.0;}
   if(weight_CPS<=0)toReturn = 0.0;
   return toReturn;
}


//
std::pair<TH1 *,TH1 *> generateHistoEnvelope(TH1 *h,bool debug)
{
  std::pair<TH1 *,TH1 *> toReturn(0,0);
  if(h==0) return toReturn;

  //get bin with min. relative error and use as reference
  int imin(1);
  float ymin(h->GetBinContent(1));
  float ymin_err(h->GetBinError(1));
  float xmin(h->GetBinCenter(1));
  for(int ibin=2; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      float x     = h->GetBinCenter(ibin);
      float y     = h->GetBinContent(ibin);
      float y_err = h->GetBinError(ibin);
      if(y==0) continue;
      if(ymin==0)
	{
	  imin     = ibin;
	  xmin     = x;
	  ymin     = y;
	  ymin_err = y_err;
	}
      else if(y_err/y < ymin_err/ymin)
	{
	  imin     = ibin;
	  xmin     = x;
	  ymin     = y;
	  ymin_err = y_err;
	}
    }
  if(ymin==0) return toReturn;

  //now get the max variations to the left and to the right of this point
  int imaxL(imin),           imaxR(imin);
  float ymaxL(ymin),         ymaxR(ymin);
  float ymaxL_err(ymin_err), ymaxR_err(ymin_err);
  float xmaxL(xmin),         xmaxR(xmin);
  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      if(ibin==imin) continue;
      float x     = h->GetBinCenter(ibin);
      float y     = h->GetBinContent(ibin);
      float y_err = h->GetBinError(ibin);
      if(y==0) continue;
      if(x<xmin && ymaxL_err/ymaxL<y_err/y)
	{
	  imaxL     = ibin;
	  xmaxL     = x;
	  ymaxL     = y;
	  ymaxL_err = y_err;
	}
      if(x>xmin && ymaxR_err/ymaxR<y_err/y)
	{
	  imaxR     = ibin;
	  xmaxR     = x;
	  ymaxR     = y;
	  ymaxR_err = y_err;
	}
    }

//   cout << xmin << " " << ymin << " " << ymin_err
//        << " | " << xmaxL << " " << ymaxL << " " << ymaxL_err
//        << " | " << xmaxR << " " << ymaxR << " " << ymaxR_err
//        << endl;

  float aL=(TMath::Max(ymaxL+ymaxL_err,Float_t(0.))/ymaxL - TMath::Max(ymin-ymin_err,Float_t(0.))/ymin)/(xmaxL-xmin);
  float bL= TMath::Max(ymaxL+ymaxL_err,Float_t(0.))/ymaxL -  aL*xmaxL;
  
  float aR=(TMath::Max(ymaxR+ymaxR_err,Float_t(0.))/ymaxR - TMath::Max(ymin-ymin_err,Float_t(0.))/ymin)/(xmaxR-xmin);
  float bR= TMath::Max(ymaxR+ymaxR_err,Float_t(0.))/ymaxR -  aR*xmaxR;
  
  TF1 *shiftUp = new TF1("shiftup","(x<[0])*([1]*x+[2])+(x>=[0])*([3]*x+[4])",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  shiftUp->SetLineColor(kRed);
  shiftUp->SetParameter(0,xmin);
  shiftUp->SetParameter(1,aL);
  shiftUp->SetParameter(2,bL);
  shiftUp->SetParameter(3,aR);
  shiftUp->SetParameter(4,bR);


  aL=(TMath::Max(ymaxL-ymaxL_err,Float_t(0.))/ymaxL - TMath::Max(ymin+ymin_err,Float_t(0.))/ymin)/(xmaxL-xmin);
  bL= TMath::Max(ymaxL-ymaxL_err,Float_t(0.))/ymaxL -  aL*xmaxL;
  
  aR=(TMath::Max(ymaxR-ymaxR_err,Float_t(0.))/ymaxR - TMath::Max(ymin+ymin_err,Float_t(0.))/ymin)/(xmaxR-xmin);
  bR= TMath::Max(ymaxR-ymaxR_err,Float_t(0.))/ymaxR -  aR*xmaxR;


  TF1 *shiftDown = new TF1("shiftdown","(x<[0])*([1]*x+[2])+(x>=[0])*([3]*x+[4])",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  shiftDown->SetLineColor(kBlue);
  shiftDown->SetParameter(0,xmin);
  shiftDown->SetParameter(1,aL);
  shiftDown->SetParameter(2,bL);
  shiftDown->SetParameter(3,aR);
  shiftDown->SetParameter(4,bR);

  
  TH1 *hUp   = (TH1 *) h->Clone("hUp");   hUp->Reset("ICE");   hUp->SetLineColor(kRed);    hUp->SetDirectory(0);
  TH1 *hDown = (TH1 *) h->Clone("hDown"); hDown->Reset("ICE"); hDown->SetLineColor(kBlue); hDown->SetDirectory(0); 
  for(int ibin=1; ibin<=h->GetXaxis()->GetNbins(); ibin++)
    {
      float x=h->GetBinCenter(ibin);
      float y=h->GetBinContent(ibin);

      hUp->SetBinContent(ibin, y*shiftUp->Eval(x) );
      hDown->SetBinContent(ibin, y*shiftDown->Eval(x) );
    }


  if(debug)
    {
      TCanvas *c = new TCanvas("c","c");
      c->Divide(1,2);
      c->cd(1);
      h->Draw("e1");  
      h->GetXaxis()->SetTitle("Fantasy variable");
      h->GetYaxis()->SetTitle("Events (a.u.)");
      hUp->Draw("histsame");
      hDown->Draw("histsame");
      
      c->cd(2);
      shiftUp->Draw();
      shiftUp->GetXaxis()->SetTitle("Fantasy variable");
      shiftUp->GetYaxis()->SetTitle("Relative shift");
      shiftDown->Draw("same");
      shiftUp->GetYaxis()->SetRangeUser(-5,5);
    }

  delete shiftUp;
  delete shiftDown;

  toReturn.first=hUp;
  toReturn.second=hDown;
  return toReturn;
}


