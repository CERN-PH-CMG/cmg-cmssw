from CMGTools.RootTools.PyRoot import *
import sys	
from ROOT import gROOT, TCanvas, TF1, TGraph2D
gROOT.Reset()

class Fitter(object):

    def __init__( self, basename, ptmin, ptmax, etamin, etamax, tree):
        # include ptmin and ptmax in the name
        self.name = basename+str(etamin)+'<jet1_eta<'+str(etamax)+' && '+str(ptmin)+'<jet1_pt<'+str(ptmax)
        self.ptmin = ptmin
        self.ptmax = ptmax
        self.etamin = etamin
        self.etamax = etamax
        self.tree = tree
        self.hist = self.buildnfit('rec')

    def buildnfit(self, ext):
        hname = '_'.join([self.name,ext])
        #for neutral hadrons
        #h = TH1F( hname, hname, 100, -1, 3)
        #for photons
        h = TH1F( hname, hname, 100, 0.5, 1.5)
        h.Sumw2()
        h.SetStats(0)
        h.SetXTitle('p_{T,rec}/p_{T,gen}')
        var = 'jet1_{ext}_energy/jet1_energy>>{hname}'.format(
            ext=ext, hname=hname
            )
        strptmax = str(ptmax)
        strptmin = str(ptmin)
        stretamax = str(etamax)
        stretamin = str(etamin)		
        cut = 'jet1_eta>'+stretamin+' && jet1_eta<'+stretamax+' && jet1_energy>'+strptmin+' && jet1_energy<'+strptmax	
        self.tree.Draw(var, cut, 'goff')
        print "========================================================="
        print cut
##         mgaus = TF1( 'mgaus','[0]*exp(-0.5*((x-[2])/[1])^2)',0.9,1.1)
##         mgaus.SetParameter(0, 15);
##         mgaus.SetParameter(2, 1.);
##         mgaus.SetParameter(1, 0.02);
##         mgaus.SetParName(0, "Constant")
##         mgaus.SetParName(1, "Resolution")
        h.Fit('gaus')
        return h	

## #ptbins = [
##    #(0, 10),
##    (10, 15),
##    (15, 20),
##    (20, 25),
##    (25, 30),
##    (30, 35),
##    (35, 40),
##    (40, 45),
##    (45, 50),
##    (50, 55),
##    (55, 60),
##    (60, 65),
##    (65, 70),
##    (70, 75),
##    (75, 80),
##    (80, 85),
##    (85, 90),
##    (90, 95),
##    (95, 100)
##    ]

ptbins = [
    (10, 20),	
    (20, 30),
    (30, 40),
    (40, 50),
    (50, 60),
    (60, 70),
    (70, 80),
    (80, 90),
    (90, 100),
    ]	

etabins = [
    (-1, -0.8),
    (-0.8, -0.6),
    (-0.6, -0.4),
    (-0.4, -0.2),
    (-0.2, 0.),
    (0., 0.2),
    (0.2, 0.4),
    (0.4, 0.6),
    (0.6, 0.8),
    (0.8, 1.),
    ]	

chain = Chain(None, sys.argv[1])

i=1
ptlist=[]
etalist=[]
resolutionlist=[]

resmap2 = TH2F('resmap2', 'resmap2', 9, 10, 100, 10, -1, 1)

ptbin = 0
for ptmin, ptmax in ptbins:
  #raw_input("")
  ptbin += 1 
  etabin = 0
  for etamin, etamax in etabins:
    # raw_input("")
    etabin += 1
    c1 = Fitter('fit', ptmin, ptmax, etamin, etamax, chain)
    fun = c1.hist.GetFunction('gaus')
    sigma = fun.GetParameter(2)
    ptlist=ptlist+[ptmin+(ptmax-ptmin)/2]
    if (etamax-etamin)>0:
        etalist=etalist+[etamax+(etamin-etamax)/2]
    else:
        etalist=etalist+[etamin+(etamax-etamin)/2] 	
    resolutionlist.append(sigma)
    print ptbin, etabin, sigma
    resmap2.SetBinContent(ptbin, etabin, sigma)

resmap2.Draw('colz')
## N=1
## resmap = TGraph2D()
## while(N<len(resolutionlist)):
##     global N
##     x = ptlist[N]
##     y = etalist[N]
##     z = resolutionlist[N]
##     resmap.SetPoint(N, x, y, z)
##     N=N+1
## resmap.GetXaxis().SetTitle("Pt [GeV]");	
## resmap.GetYaxis().SetTitle("eta");	
## resmap.GetZaxis().SetTitle("sigma");
## resmap.SetNameTitle("c1", "Resolution map for photons")
## resmap.GetXaxis().SetRangeUser(0, 100)
## resmap.GetYaxis().SetRangeUser(-1, 1)
## raw_input("")	
## resmap.Draw("colz")	

