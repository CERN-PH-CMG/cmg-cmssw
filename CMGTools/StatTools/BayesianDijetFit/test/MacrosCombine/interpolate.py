from ROOT import *
import ROOT as r
import sys
import array

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
  if (u<-a1):
      result *= A1*TMath.Power(B1-u,-p1);
  elif (u<a2):
      result *= TMath.Exp(-u*u/2);
  else:
      result *= A2*TMath.Power(B2+u,-p2);
  return result;

frac1 = 0.8 
frac2 = 1.2

inputRoot = sys.argv[1]
outmjj = int( sys.argv[2] )
if len(sys.argv)>3:
   suffix=sys.argv[3]
else:
   suffix=""

ngenevents=[30000,30000,30000,30000,30000,30000,30000,30000]

if "Qstar" in inputRoot:
  masses=[1000,1500,2000,3000,4000]
  if outmjj >= 1000 and outmjj < 1500 :
    mjjlow = 1000
    mjjhigh = 1500
  if outmjj >= 1500 and outmjj < 2000 :
    mjjlow = 1500
    mjjhigh = 2000
  if outmjj >= 2000 and outmjj < 3000 :
    mjjlow = 2000
    mjjhigh = 3000
  if outmjj >= 3000 and outmjj <= 4000 :
    mjjlow = 3000
    mjjhigh = 4000
elif "Bulk" in inputRoot:
  masses=[1000,1500,1800,1900,2000,2200,2500]
  ngenevents=[49992,45992,36807,47995,48995,49745,46612]
  if outmjj >= 1000 and outmjj < 1500 :
    mjjlow = 1000
    mjjhigh = 1500
  if outmjj >= 1500 and outmjj < 1800 :
    mjjlow = 1500
    mjjhigh = 1800
  if outmjj >= 1800 and outmjj < 1900 :
    mjjlow = 1800
    mjjhigh = 1900
  if outmjj >= 1900 and outmjj < 2000 :
    mjjlow = 1900
    mjjhigh = 2000
  if outmjj >= 2000 and outmjj < 2200 :
    mjjlow = 2000
    mjjhigh = 2200
  if outmjj >= 2200 and outmjj <= 3000 :
    mjjlow = 2200
    mjjhigh = 2500
elif "pythia" in inputRoot or "Wprime" in inputRoot:
  masses=[1000,1500,1800,2000,2200,2500,3000]
  if outmjj >= 1000 and outmjj < 1500 :
    mjjlow = 1000
    mjjhigh = 1500
  if outmjj >= 1500 and outmjj < 1800 :
    mjjlow = 1500
    mjjhigh = 1800
  if outmjj >= 1800 and outmjj < 2000 :
    mjjlow = 1800
    mjjhigh = 2000
  if outmjj >= 2000 and outmjj < 2200 :
    mjjlow = 2000
    mjjhigh = 2200
  if outmjj >= 2200 and outmjj < 2500 :
    mjjlow = 2200
    mjjhigh = 2500
  if outmjj >= 2500 and outmjj <= 3000 :
    mjjlow = 2500
    mjjhigh = 3000
else:
  masses=[1000,1500,2000,3000]
  if outmjj >= 1000 and outmjj < 1500 :
    mjjlow = 1000
    mjjhigh = 1500
  if outmjj >= 1500 and outmjj < 2000 :
    mjjlow = 1500
    mjjhigh = 2000
  if outmjj >= 2000 and outmjj <= 3000 :
    mjjlow = 2000
    mjjhigh = 3000

inputlow = TFile( inputRoot + str(mjjlow) + suffix + '.root' )
inputhigh = TFile( inputRoot + str(mjjhigh) + suffix +  '.root' )
output = TFile( inputRoot +'OUT' + str(outmjj) + suffix + '.root', 'recreate')

print sys.argv[1], outmjj, mjjlow, mjjhigh

histnames= ["DijetMassHighPuriVV",
            "DijetMassMediumPuriVV",
            "DijetMassLowPuriVV",
            "DijetMassHighPuriqV",
            "DijetMassMediumPuriqV",
            "DijetMassLowPuriqV",
            ]

hists=[]

for histname in histnames:
 flow = inputlow.Get( histname )
 if not flow: continue
 flow.SetName( 'low' )
 fhigh = inputhigh.Get( histname )
 fhigh.SetName( 'high' ) 
 
 output.cd()
 #su = 0.
 foutmjj = TH1F(histname, histname, 7000, 0, 7000 )
 hists += [foutmjj]
 #foutmjjfrac = TH1F( 'frac'+str(outmjj), 'frac'+str(outmjj), 10000, 0.3, 1.3)
 #foutmjjfit =  TH1F('doublefit'+str(outmjj), 'doublefit'+str(outmjj), 7000, 0, 7000 )

 old_interpolation=False

 if old_interpolation:
   # interpolate the shape linearly
   su = 0
   su1 = 0.0
   for i in range(7001) :
     x = 0.0 + i/7000.0*2.0 # interpolation range: [0*mass,2*mass]
     masslow = x*mjjlow
     masshigh = x*mjjhigh
     #print masshigh
     massout = x*outmjj
     prob1 = flow.GetBinContent( flow.FindBin(masslow) ) 
     prob2 = fhigh.GetBinContent( fhigh.FindBin(masshigh) ) 
     #print x, prob1, prob2
     prob = prob1 + (prob2 - prob1)*(massout - mjjlow)/float(mjjhigh - mjjlow)
     #print x, prob
     #foutmjjfrac.SetBinContent(i+1, max(0,prob))
     foutmjj.SetBinContent( foutmjj.FindBin(massout), max(0,prob))
     su += prob
     su1 += prob2
   #print su
   #print su1

   # interpolate the peak height smoothly
   xvalues=r.vector('double')()
   yvalues=r.vector('double')()
   for x in masses:
     inputf = TFile( inputRoot + str(x) + suffix + '.root' )
     f = inputf.Get( histname )
     xvalues.push_back(x)
     yvalues.push_back(f.Integral(f.FindBin(x*0.8),f.FindBin(x*1.2))/ngenevents[masses.index(x)]*30000.)
   interpolator=r.Math.Interpolator(xvalues,yvalues)
   integral=interpolator.Eval(outmjj)
   foutmjj.Scale( integral/foutmjj.Integral(foutmjj.FindBin(outmjj*0.8),foutmjj.FindBin(outmjj*1.2)) )
 else:
   # interpolate the peak height+shape smoothly
   xvalues=r.vector('double')()
   yvalues=[]
   npoints=20000
   rebin=1
   for i in range(npoints+1) :
     yvalues+=[r.vector('double')()]
   for m in masses:
     inputf = TFile( inputRoot + str(m) + suffix + '.root' )
     f = inputf.Get( histname )
     xvalues.push_back(m)
     for i in range(npoints+1):
       x=0.0 + i/float(npoints)*2.0
       yvalues[i].push_back(f.Integral(f.FindBin(m*x)-rebin+1,f.FindBin(m*x)+rebin-1)/float(2*rebin-1)/ngenevents[masses.index(m)]*30000.)
   for i in range(npoints+1):
     x=0.0 + i/float(npoints)*2.0
     interpolator=r.Math.Interpolator(xvalues,yvalues[i])
     foutmjj.SetBinContent(foutmjj.FindBin(outmjj*x),max(0,interpolator.Eval(outmjj)))

 print foutmjj.Integral(foutmjj.FindBin(outmjj*0.8),foutmjj.FindBin(outmjj*1.2)), flow.Integral(flow.FindBin(mjjlow*0.8),flow.FindBin(mjjlow*1.2)), fhigh.Integral(fhigh.FindBin(mjjhigh*0.8),fhigh.FindBin(mjjhigh*1.2))

 # fit not used currently
 fit_gaus=TF1('fit gaus','gaus',outmjj*frac1, outmjj*frac2 )
 #foutmjj.Fit(fit_gaus,"R0N")
 #print "fit mean, width", fit_gaus.GetParameter(1), fit_gaus.GetParameter(2)
 fit=TF1('fit',fnc_dscb, outmjj*0.3, outmjj*1.3, 7)
 fit.SetTitle("")
 fit.SetParameter(0,fit_gaus.GetParameter(0))
 fit.SetParameter(1,fit_gaus.GetParameter(1))
 fit.SetParameter(2,fit_gaus.GetParameter(2))
 fit.SetParameter(3,2)
 fit.SetParameter(4,1)
 fit.SetParameter(5,2)
 fit.SetParameter(6,1)
 fit.SetLineWidth(1)
 #fit.SetLineStyle(1)
 #foutmjj.Fit(fit, "RON")
 
 
 
 for k in range(1001) :
     x = 0.3 + k/1000.0
     massout = x*outmjj
     #foutmjjfit.SetBinContent( foutmjjfit.FindBin(massout+0.5), fit.Eval(massout) )
 
 
output.cd()
output.Write()
output.Close()
 
 
 