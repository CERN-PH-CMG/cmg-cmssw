import os, sys
import array
from ROOT import * 

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

def gaussgauss(xx,pp):
  x   = xx[0];
  N   = pp[0];
  mu  = pp[1];
  sig1 = pp[2];
  sig2 = pp[3];
  if x>=mu:
    return N*exp(-0.5*pow(x-mu,2)/sig1)
  else:
    return N*exp(-0.5*pow(x-mu,2)/sig2)

def fnc_dscb(xx,pp):
  x   = xx[0];
  N   = pp[0];
  mu  = pp[1];
  sig = pp[2];
  a1  = pp[3];
  p1  = pp[4];
  a2  = pp[5];
  p2  = pp[6];

  u   = (x-mu)/sig;
  A1  = TMath.Power(p1/TMath.Abs(a1),p1)*TMath.Exp(-a1*a1/2);
  A2  = TMath.Power(p2/TMath.Abs(a2),p2)*TMath.Exp(-a2*a2/2);
  B1  = p1/TMath.Abs(a1) - TMath.Abs(a1);
  B2  = p2/TMath.Abs(a2) - TMath.Abs(a2);

  result=N;
  if x<0:
      result=-999
  elif (u<-a1):
      result *= A1*TMath.Power(B1-u,-p1);
  elif (u<a2):
      result *= TMath.Exp(-u*u/2);
  else:
      result *= A2*TMath.Power(B2+u,-p2);
  return result;

def fnc_dscb1(xx,pp):
  x   = xx[0];
  N   = pp[0];
  mu  = pp[1];
  sig = pp[2];
  a1  = abs(pp[3]);
  p1  = abs(pp[4]);
  a2  = abs(pp[3]);
  p2  = abs(pp[4]);

  u   = (x-mu)/sig;
  A1  = TMath.Power(p1/TMath.Abs(a1),p1)*TMath.Exp(-a1*a1/2);
  A2  = TMath.Power(p2/TMath.Abs(a2),p2)*TMath.Exp(-a2*a2/2);
  B1  = p1/TMath.Abs(a1) - TMath.Abs(a1);
  B2  = p2/TMath.Abs(a2) - TMath.Abs(a2);

  result=N;
  if x<0:
      result=-999
  elif (u<-a1):
      result *= A1*TMath.Power(B1-u,-p1);
  elif (u<a2):
      result *= TMath.Exp(-u*u/2);
  else:
      result *= A2*TMath.Power(B2+u,-p2);
  return result;

def langaufun(x, par):
   #Fit parameters:
   #par[0]=Width (scale) parameter of Landau density
   #par[1]=Most Probable (MP, location) parameter of Landau density
   #par[2]=Total area (integral -inf to inf, normalization constant)
   #par[3]=Width (sigma) of convoluted Gaussian function
   #
   #In the Landau distribution (represented by the CERNLIB approximation), 
   #the maximum is located at x=-0.22278298 with the location parameter=0.
   #This shift is corrected within this function, so that the actual
   #maximum is identical to the MP parameter.

   # Numeric constants
   invsq2pi = 0.3989422804014;   # (2 pi)^(-1/2)
   mpshift  = -0.22278298;	 # Landau maximum location

   # Control constants
   np = 100.0;      # number of convolution steps
   sc =   5.0;      # convolution extends to +-sc Gaussian sigmas

   # Variables
   sum = 0.0;

   # MP shift correction
   mpc = par[1] - mpshift * par[0]; 

   # Range of convolution integral
   xlow = x[0] - sc * par[3];
   xupp = x[0] + sc * par[3];

   step = (xupp-xlow) / np;

   # Convolution integral of Landau and Gaussian by sum
   for i in range(1,int(np/2.+1)):
      xx = xlow + (i-.5) * step;
      fland = TMath.Landau(xx,mpc,par[0]) / par[0];
      sum += fland * TMath.Gaus(x[0],xx,par[3]);

      xx = xupp - (i-.5) * step;
      fland = TMath.Landau(xx,mpc,par[0]) / par[0];
      sum += fland * TMath.Gaus(x[0],xx,par[3]);

   return (par[2] * step * sum * invsq2pi / par[3]);

if __name__ == '__main__':

  colors=[1,2,3,3,4,4,6,6,7,7,8,8,9,9,10,10]
  styles=[1,1,2,3,2,3,2,3,2,3,2,3,2,3,2,3]
  
  prefix="CMS_2012_10fb_forHCP"
  systematics=["",
        #"_DR3",
        #"_DR4",
        #"_NODR",
  	       #"_NODR_iso06",
        #"_NODR_iso07",
        ]
  categories=["BOOSTED"]#,"VBF"
  variables=["svfitMass"]#,"l1Pt"

  fitfunctions=["pol","crystalball","doublegauss"]#"pol","gauss","crystalball","landau","rayleigh"

  for fitfunction in fitfunctions:
   for category in categories:
    for variable in variables:
     for closure in [True,False]:
      canvas = TCanvas("","",0,0,200,200)
      legend=TLegend(0.5,0.5,0.95,0.90,"")
    
      files=[]
      hists=[]
    
      index=0
      for systematic in systematics:
        print category,systematic
	f=TFile.Open(prefix+systematic+"_"+category+"/"+prefix+systematic+"_"+category+"_QCD_TauTau_mH125_PLOT.root")
	files+=[f]
	if closure:
            hist=f.Get(variable+"/QCDlooseSS").Clone("QCDlooseSS"+systematic)
	else:
            hist=f.Get(variable+"/QCDlooseOS").Clone("QCDlooseOS"+systematic)
	hist.SetTitle(fitfunction)
	hist.SetLineColor(colors[index])
        hist.SetLineStyle(styles[index])
	hist.SetFillStyle(0)
        hist.SetMarkerStyle(0)
        binning=array.array('d')
	if variable=="svfitMass":
            #bins=[20,40,60,80,100,120,140,160,20fitmin,fitmax]
            bins=[20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]
	else:
            bins=[40,60,80,100,120,160,200]
	for bin in bins:
           binning.append(bin)
        #hist=hist.Rebin(len(bins)-1,hist.GetName()+"_rebin",binning)
        hists+=[hist]
	if index==0:
	    hist1=hist
        hist.Scale(hist1.Integral()/hist.Integral())
	hist.GetYaxis().SetRangeUser(0,hist.GetMaximum()*1.5)
	if index==0:
            hist.Draw("he")
	else:	
            hist.Draw("histesame")

        fitmin=0
	fitmax=300
        fitvariations=[]
        fit_gaus = TF1("fitgaus","gaus",fitmin,fitmax)
        hist.Fit(fit_gaus,"R0NQ")

        if fitfunction=="gauss":
         fit = TF1(fitfunction,"gaus",fitmin,fitmax)
 	 fit.SetParameter(0,fit_gaus.GetParameter(0))
 	 fit.SetParameter(1,fit_gaus.GetParameter(1))
	 fit.SetParameter(2,fit_gaus.GetParameter(2))

        if fitfunction=="pol":
         fit = TF1(fitfunction,"exp([0]+[1]*x+[2]*x*x+[3]*x*x*x)",fitmin,fitmax)
 	 fit.SetParameter(0,1)
 	 fit.SetParameter(1,0.01)
	 fit.SetParameter(2,-0.0001)
	 fit.SetParameter(3,0.0000001)

        if fitfunction=="rayleigh":
         fit = TF1(fitfunction,"abs([0])*x*exp(-0.5*pow((x-[1])/[2],2))",fitmin,fitmax)
 	 fit.SetParameter(0,fit_gaus.GetParameter(0))
 	 fit.SetParameter(1,fit_gaus.GetParameter(1))
	 fit.SetParameter(2,fit_gaus.GetParameter(2))

        if fitfunction=="gaussgauss":
	 fit=TF1(fitfunction,gaussgauss,fitmin,fitmax,4)
	 fit.SetParameter(0,fit_gaus.GetParameter(0))
	 fit.SetParameter(1,fit_gaus.GetParameter(1))
	 fit.SetParameter(2,fit_gaus.GetParameter(2))
	 fit.SetParameter(3,fit_gaus.GetParameter(2))
        
        if fitfunction=="doublegauss":
	 fit=TF1(fitfunction,'gaus(0)+gaus(3)',fitmin,fitmax)
	 fit.SetParameter(0,fit_gaus.GetParameter(0)/2.0)
	 fit.SetParameter(1,fit_gaus.GetParameter(1)*1.0)
	 fit.SetParameter(2,fit_gaus.GetParameter(2))
	 fit.SetParameter(3,fit_gaus.GetParameter(0)/2.0)
	 fit.SetParameter(4,fit_gaus.GetParameter(1)*2.0)
	 fit.SetParameter(5,fit_gaus.GetParameter(2))
	 fit.SetParameter(6,fit_gaus.GetParameter(0)/2.0)
	 fit.SetParameter(7,fit_gaus.GetParameter(1)/4.0)
	 fit.SetParameter(8,fit_gaus.GetParameter(2))
        
        if fitfunction=="landau":
	 fit=TF1(fitfunction,'landau',fitmin,fitmax)
	 fit.SetParameter(0,fit_gaus.GetParameter(0))
	 fit.SetParameter(1,fit_gaus.GetParameter(1))
	 fit.SetParameter(2,fit_gaus.GetParameter(2))
        
        if fitfunction=="gausslandau":
	 fit=TF1(fitfunction,langaufun,fitmin,fitmax,4)
	 fit.SetParameter(0,20)
 	 fit.SetParLimits(0,0.001,100)
 	 fit.SetParameter(1,fit_gaus.GetParameter(1))
 	 fit.SetParameter(2,40000)
	 fit.SetParLimits(2,1,100000000)
	 fit.SetParameter(3,fit_gaus.GetParameter(1)/2.)
	 fit.SetParLimits(3,0.001,100)
        
        if fitfunction=="crystalball":
	 fit=TF1(fitfunction,fnc_dscb1,fitmin,fitmax,5)
	 fit.SetParameter(0,fit_gaus.GetParameter(0))
	 fit.SetParameter(1,fit_gaus.GetParameter(1))
	 fit.SetParameter(2,fit_gaus.GetParameter(2))
	 fit.SetParameter(3,2)
	 fit.SetParameter(4,2)
        
	fit.SetTitle("")
        fit.SetLineWidth(2)
        fit.SetLineColor(colors[index])
        fit.SetLineStyle(styles[index])
        fitresult=hist.Fit(fit,"RB0SLQ")
	fitresult.Print()
        fitvariation=TF1(fit)
        fitvariation.Draw("same")
        fitvariations+=[fitvariation]
	index+=1

	if systematic=="_NODR":
            legend.AddEntry(hist,"#Delta R free","l")
	elif "DR3" in systematic:
            legend.AddEntry(hist,"#Delta R < 3","l")
	elif "DR4" in systematic:
            legend.AddEntry(hist,"#Delta R < 4","l")
	elif "iso06" in systematic:
            legend.AddEntry(hist,"#tau iso < 0.6","l")
	elif "iso07" in systematic:
            legend.AddEntry(hist,"#tau iso < 0.7","l")
	elif systematic=="":
            legend.AddEntry(hist,"#Delta R < 2","l")
        
        eigenValues=TVectorD(4)
        eigenVectors=fitresult.GetCovarianceMatrix().EigenVectors(eigenValues)
        eigenValues.Sqrt()
        print "Eigenvalues:",[e for e in eigenValues]

	originalpars=[]
        for par in range(fit.GetNpar()):
	    originalpars+=[fit.GetParameter(par)]
        pars=fit.GetParameters()

	if closure:
            hist2=f.Get(variable+"/QCDtightSS").Clone("QCDtightSS"+systematic)
            #hist2=hist2.Rebin(len(bins)-1,hist2.GetName()+"_rebin",binning)
            hist2.SetLineColor(colors[index])
            hist2.SetLineStyle(styles[index])
            hist2.SetFillStyle(0)
            hist2.SetMarkerStyle(0)
            hists+=[hist2]
            hist2.Scale(hist1.Integral()/hist2.Integral())
            hist2.Draw("histesame")
            legend.AddEntry(hist2,"closure test","l")
            fitvariation2=TF1(fit)
            fitvariation2.SetLineColor(colors[index])
            fitvariation2.SetLineStyle(styles[index])
            fitresult2=hist2.Fit(fitvariation2,"RB0SLQ")
	    fitresult2.Print()
            fitvariation2.Draw("same")
            fitvariations+=[fitvariation2]
            #invertedEigenValues=array.array('d',range(fit.GetNpar()))
            #invertedEigenVectors=eigenVectors.Invert(invertedEigenValues)
            pulls=[]
            for par in range(min(fit.GetNpar(),3)):
	      pull=[]
              for k in range(min(fit.GetNpar(),4)):
                pull+=[(abs(eigenValues(par)*eigenVectors[k][par]),abs(originalpars[k]-fitvariation2.GetParameter(k))/abs(eigenValues(par)*eigenVectors[k][par]))]
	      pulls+=[sorted(pull)[-1][1]]
            print "Systematic pull:",pulls

	index+=1

        for par in range(min(fit.GetNpar(),3)):
            fitvariation=TF1(fit)
            for k in range(fit.GetNpar()):
                pars[k]=originalpars[k]+eigenValues(par)*eigenVectors[k][par]*max(1,pulls[par])
	    #pars[par]=fitvariation.GetParameter(par)+fitvariation.GetParError(par)
	    fitvariation.SetParameters(pars)
            fitvariation.SetLineColor(colors[index])
            fitvariation.SetLineStyle(styles[index])
	    fitvariation.Draw("same")
            index+=1
            legend.AddEntry(fitvariation,"ev"+str(par)+" +1#sigma","l")
	    fitvariations+=[fitvariation]
	    #pars[par]=originalpars[par]
            fitvariation=TF1(fit)
            for k in range(fit.GetNpar()):
                pars[k]=originalpars[k]-eigenValues(par)*eigenVectors[k][par]*max(1,pulls[par])
	    #pars[par]=fitvariation.GetParameter(par)-fitvariation.GetParError(par)
	    fitvariation.SetParameters(pars)
            fitvariation.SetLineColor(colors[index])
            fitvariation.SetLineStyle(styles[index])
	    fitvariation.Draw("same")
            index+=1
            legend.AddEntry(fitvariation,"ev"+str(par)+" -1#sigma","l")
	    fitvariations+=[fitvariation]
	    #pars[par]=originalpars[par]

      legend.SetTextSize(0.04)
      legend.SetFillStyle(0)
      legend.Draw("same")
      if closure:
          canvas.SaveAs("QCDShapeUncertaintyFit_plot_"+category+"_"+variable+"_"+fitfunction+"_closure.pdf")
      else:
          canvas.SaveAs("QCDShapeUncertaintyFit_plot_"+category+"_"+variable+"_"+fitfunction+".pdf")
