from ROOT import *
import sys

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

if outmjj > 1000 and outmjj < 1500 :
    mjjlow = 1000
    mjjhigh = 1500

if outmjj > 1500 and outmjj < 2000 :
    mjjlow = 1500
    mjjhigh = 2000

if outmjj > 2000 and outmjj < 3000 :
    mjjlow = 2000
    mjjhigh = 3000

if outmjj > 3000 and outmjj < 4000 :
    mjjlow = 3000
    mjjhigh = 4000

if outmjj == 1000 or outmjj == 1500 or outmjj == 2000 or outmjj == 3000 \
        or outmjj == 4000 :
    mjjlow = outmjj
    mjjhigh = outmjj

inputlow = TFile( inputRoot + str(mjjlow) + suffix + '.root' )
inputhigh = TFile( inputRoot + str(mjjhigh) + suffix +  '.root' )
output = TFile( inputRoot +'OUT' + str(outmjj) + suffix + '.root', 'recreate')

print sys.argv[1], outmjj, mjjlow, mjjhigh

histnames= ["dijet_mass_0mtag",
            "dijet_mass_1mtag_0mdtag",
            "dijet_mass_1mtag_1mdtag",
            "dijet_mass_2mtag_0mdtag",
            "dijet_mass_2mtag_1mdtag",
            "dijet_mass_2mtag_2mdtag",
            #"dijet_mass_1mtag_0nstag",
            #"dijet_mass_1mtag_1nstag",
            #"dijet_mass_2mtag_0nstag",
            #"dijet_mass_2mtag_1nstag",
            #"dijet_mass_2mtag_2nstag",
            #"dijet_mass_0mtag",
            #"dijet_mass_1mtag_1lptag",
            #"dijet_mass_1mtag_1hptag",
            #"dijet_mass_2mtag_2lptag",
            #"dijet_mass_2mtag_1hptag_1lptag",
            #"dijet_mass_2mtag_2hptag",
            ]

hists=[]

for histname in histnames:

 flow = inputlow.Get( histname )
 flow.SetName( 'low' )
 fhigh = inputhigh.Get( histname )
 fhigh.SetName( 'high' ) 
 
 if mjjhigh != mjjlow:
     integral=flow.Integral() + (fhigh.Integral() - flow.Integral())*(outmjj - mjjlow)/float(mjjhigh - mjjlow)
 else:
     integral=flow.Integral()
 
 flow.Scale( 1/flow.Integral() )
 fhigh.Scale( 1/fhigh.Integral() )
 
 print fhigh.Integral()
 #N = f1000.GetNbinsX()
 
 output.cd()
 #su = 0.
 foutmjj = TH1F(histname, histname, 7000, 0, 7000 )
 hists += [foutmjj]
 #foutmjjfrac = TH1F( 'frac'+str(outmjj), 'frac'+str(outmjj), 10000, 0.3, 1.3)
 #foutmjjfit =  TH1F('doublefit'+str(outmjj), 'doublefit'+str(outmjj), 7000, 0, 7000 )
 
 su = 0
 su1 = 0.0
 for i in range(7001) :
     
     x = 0.3 + i/7000.0
     masslow = x*mjjlow
     masshigh = x*mjjhigh
     #print masshigh
     massout = x*outmjj
     prob1 = flow.GetBinContent( flow.FindBin(masslow) ) 
     prob2 = fhigh.GetBinContent( fhigh.FindBin(masshigh) ) 
    # print x, prob1, prob2
     if outmjj != 1000 and outmjj != 1500 and outmjj != 2000 and outmjj != 3000 \
              and outmjj != 4000 : 
         prob = prob1 + (prob2 - prob1)*(massout - mjjlow)/float(mjjhigh - mjjlow)
     else :
         prob =( prob1 + prob2 )/2.0
 #    print x, prob
     #foutmjjfrac.SetBinContent(i+1, max(0,prob))
     foutmjj.SetBinContent( foutmjj.FindBin(massout+0.5), max(0,prob))
     su += prob
     su1 += prob2
 print su
 print su1

 foutmjj.Scale( integral/foutmjj.Integral() )
 
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
 
 
 