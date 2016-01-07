#!/usr/bin/env python

import ROOT
import optparse




parser = optparse.OptionParser()
parser.add_option("-o","--output",dest="output",default='limitPlot.root',help="Limit plot")

parser.add_option("-x","--minX",dest="minX",type=float,help="minimum x",default=1000.0)
parser.add_option("-X","--maxX",dest="maxX",type=float,help="maximum x",default=5000.0)
parser.add_option("-y","--minY",dest="minY",type=float,help="minimum y",default=0.00001)
parser.add_option("-Y","--maxY",dest="maxY",type=float,help="maximum y",default=1000)
parser.add_option("-b","--blind",dest="blind",type=int,help="Not do observed ",default=1)
parser.add_option("-l","--log",dest="log",type=int,help="Log plot",default=1)

parser.add_option("-t","--titleX",dest="titleX",default='M_{X} [GeV]',help="title of x axis")
parser.add_option("-T","--titleY",dest="titleY",default='#sigma x BR(X #rightarrow WW) [pb]  ',help="title of y axis")

parser.add_option("-p","--paveText",dest="label",default='CMS Preliminary , L = 1.3 fb^{-1}',help="label")



#    parser.add_option("-x","--minMVV",dest="minMVV",type=float,help="minimum MVV",default=1000.0)
#    parser.add_option("-X","--maxMVV",dest="maxMVV",type=float,help="maximum MVV",default=13000.0)






(options,args) = parser.parse_args()
#define output dictionary


f=ROOT.TFile(args[0])
limit=f.Get("limit")
data={}
for event in limit:
    if not (event.mh in data.keys()):
        data[event.mh]={}

    if event.quantileExpected<0:            
        data[event.mh]['obs']=event.limit
    if event.quantileExpected>0.02 and event.quantileExpected<0.03:            
        data[event.mh]['-2sigma']=event.limit
    if event.quantileExpected>0.15 and event.quantileExpected<0.17:            
        data[event.mh]['-1sigma']=event.limit
    if event.quantileExpected>0.49 and event.quantileExpected<0.51:            
        data[event.mh]['exp']=event.limit
    if event.quantileExpected>0.83 and event.quantileExpected<0.85:            
        data[event.mh]['+1sigma']=event.limit
    if event.quantileExpected>0.974 and event.quantileExpected<0.976:            
        data[event.mh]['+2sigma']=event.limit


band68=ROOT.TGraphAsymmErrors()
band68.SetName("band68")
band95=ROOT.TGraphAsymmErrors()
band95.SetName("band95")
bandObs=ROOT.TGraph()
bandObs.SetName("bandObs")

line_plus1=ROOT.TGraph()
line_plus1.SetName("line_plus1")

line_plus2=ROOT.TGraph()
line_plus2.SetName("line_plus2")

line_minus1=ROOT.TGraph()
line_minus1.SetName("line_minus1")

line_minus2=ROOT.TGraph()
line_minus2.SetName("line_minus2")



N=0
for mass,info in data.iteritems():
    band68.SetPoint(N,mass,info['exp'])
    band95.SetPoint(N,mass,info['exp'])
    line_plus1.SetPoint(N,mass,info['+1sigma'])
    line_plus2.SetPoint(N,mass,info['+2sigma'])
    line_minus1.SetPoint(N,mass,info['-1sigma'])
    line_minus2.SetPoint(N,mass,info['-2sigma'])

    bandObs.SetPoint(N,mass,info['obs'])
    band68.SetPointError(N,0.0,0.0,info['exp']-info['-1sigma'],info['+1sigma']-info['exp'])
    band95.SetPointError(N,0.0,0.0,info['exp']-info['-2sigma'],info['+2sigma']-info['exp'])
    N=N+1


band68.Sort()
band95.Sort()
bandObs.Sort()
line_plus1.Sort()    
line_plus2.Sort()    
line_minus1.Sort()    
line_minus2.Sort()    



#plotting information

c=ROOT.TCanvas("c","c")
frame=c.DrawFrame(options.minX,options.minY,options.maxX,options.maxY)
frame.GetXaxis().SetTitle(options.titleX)
frame.GetYaxis().SetTitle(options.titleY)

band68.SetFillColor(ROOT.kGreen)
band68.SetLineWidth(2)
band68.SetLineColor(ROOT.kBlack)
band68.SetLineStyle(1)
band68.SetMarkerStyle(20)

band95.SetFillColor(ROOT.kYellow)

bandObs.SetLineWidth(3)
bandObs.SetLineColor(ROOT.kRed)

line_plus1.SetLineWidth(1)
line_plus1.SetLineColor(ROOT.kGreen-3)

line_plus2.SetLineWidth(1)
line_plus2.SetLineColor(ROOT.kYellow-6)

line_minus1.SetLineWidth(1)
line_minus1.SetLineColor(ROOT.kGreen-3)

line_minus2.SetLineWidth(1)
line_minus2.SetLineColor(ROOT.kYellow-6)


c.cd()
frame.Draw()
band95.Draw("3same")
band68.Draw("3same")
band68.Draw("XPLsame")
line_plus1.Draw("Lsame")
line_plus2.Draw("Lsame")
line_minus1.Draw("Lsame")
line_minus2.Draw("Lsame")
c.SetLogy(options.log)
c.Draw()

pt =ROOT.TPaveText(0.1577181,0.9562937,0.9580537,0.9947552,"brNDC")
pt.SetBorderSize(0)
pt.SetTextAlign(12)
pt.SetFillStyle(0)
pt.SetTextFont(42)
pt.SetTextSize(0.03)
text = pt.AddText(0.01,0.3,options.label)
pt.Draw()
c.Draw()
c.RedrawAxis()

if options.blind==0:
    bandObs.Draw("Lsame")



fout=ROOT.TFile(options.output,"RECREATE")
fout.cd()

c.Write()
band68.Write()
band95.Write()
bandObs.Write()
line_plus1.Write()    
line_plus2.Write()    
line_minus1.Write()    
line_minus2.Write()    

fout.Close()
f.Close()


