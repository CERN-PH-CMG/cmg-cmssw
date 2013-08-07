#!/usr/bin/env python

import ROOT
import os,sys
import getopt
from math import sqrt,pow
myscriptpath=os.path.expandvars('${CMSSW_BASE}/bin/${SCRAM_ARCH}')
sys.path.append( myscriptpath  )
from rounding import toLatexRounded

def usage() :
    print ' '
    print 'getTTbarEstimate.py [options]'
    print '  -i : input file'
    print ' '
    
#parse the options
try:
    # retrive command line options
    shortopts  = "i:h?"
    opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
    # print help information and exit:
    print "ERROR: unknown options in argument %s" % sys.argv[1:]
    usage()
    sys.exit(1)

inputFile='plotter.root'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(1)
    elif o in('-i'): inputFile = a
                        
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import formatPlot, showSimplePlot,showPlots, formatForCmsPublic, getNewCanvas, setStyle


rescaleRegions=[
    ["Z selection"             , ["Z",              "~Z"]],
    ["b-veto"                  , ["Z,b",            "~Z,b"]],
    ["3^{rd} lepton veto"      , ["Z,b,=2l",        "~Z,b,=2l"]],
    ["red-E_{T}^{miss}>medium" , ["Z,b,=2l,medium", "~Z,b,=2l,medium"]],
    ["red-E_{T}^{miss}>tight"  , ["Z,b,=2l,tight",  "~Z,b,=2l,tight"]]
    ]

abcdRegions={
    "Z selection"             : ["Z",              "~Z"],
    "b-veto"                  : ["Z,b",            "Z,~b",            "~Z,b",             "~Z,~b"],
    "3^{rd} lepton veto"      : ["Z,b,=2l",        "Z,~b,=2l" ,       "~Z,b,=2l",         "~Z,~b,=2l"],
    "red-E_{T}^{miss}>medium" : ["Z,b,=2l,medium", "Z,~b,=2l,medium", "~Z,b,=2l,medium",  "~Z,~b,=2l,medium"],
    "red-E_{T}^{miss}>tight"  : ["Z,b,=2l,tight",  "Z,~b,=2l,tight",  "~Z,b,=2l,tight",   "~Z,~b,=2l,tight"]
    }

fitfunc = ROOT.TF1('f1','pol0',1,len(rescaleRegions))
fitfunc.SetLineColor(2)
fitfunc.SetLineWidth(1)

eventCategories=['mumueq0jets', 'mumueq1jets', 'mumugeq2jets']
eventCategoryTitles=['=0 jets', '=1 jets',     '#geq 2 jets']
topRescalingHistos=ROOT.TList()
otherRescalingHistos=ROOT.TList()
ievcat=0
for evcat in eventCategories :
    ievcat=ievcat+1
    
    #plots to retrieve
    dataZMassRegionCtr   = None
    dataKeys=[]
    topZMassRegionCtr    = None
    topKeys=[]
    otherZMassRegionCtr = None
    otherKeys=[]

    #open file and get plots
    f = ROOT.TFile.Open(inputFile)
    for k in f.GetListOfKeys() :
        kname=k.GetName()
        if(kname.find("H(")>=0) : continue
    
        h=f.Get(kname+"/" + evcat +"_zmassregionCtr")
        if(kname.find("t#bar{t}")>=0 or kname.find("Single top")>=0):
            topKeys.append(kname)
            if(topZMassRegionCtr is None) :
                topZMassRegionCtr = h.Clone(evcat+'_topctr')
                topZMassRegionCtr.SetDirectory(0)
            else :
                topZMassRegionCtr.Add(h)
        elif(kname.find("data")>=0):
            dataKeys.append(kname)
            if(dataZMassRegionCtr is None) :
                dataZMassRegionCtr = h.Clone(evcat+'_datactr')
                dataZMassRegionCtr.SetDirectory(0)
            else :
                dataZMassRegionCtr.Add(h)
        else:
            otherKeys.append(kname)
            if(otherZMassRegionCtr is None):
                otherZMassRegionCtr = h.Clone(evcat+'_otherctr')
                otherZMassRegionCtr.SetDirectory(0)
            else :
                otherZMassRegionCtr.Add(h)
    f.Close()

    #re-scale factors
    topRescaleFactor = ROOT.TH1F(evcat+'kttbar',eventCategoryTitles[ievcat-1]+';Selection step; N_{out}/N_{in}',len(rescaleRegions),0,len(rescaleRegions))
    formatPlot(topRescaleFactor,1,1,1,20+ievcat,0,True,True,1,0,1)

    otherRescaleFactor = ROOT.TH1F(evcat+'kothers',eventCategoryTitles[ievcat-1]+';Selection step; N_{out}/N_{in}',len(rescaleRegions),0,len(rescaleRegions))
    formatPlot(otherRescaleFactor,1,1,1,20+ievcat,0,True,True,1,0,1)

    ipoint=0
    for reg in rescaleRegions:
        ipoint=ipoint+1
        topYields=[]
        otherYields=[]
        for cat in reg[1] :
            for i in xrange(1,topZMassRegionCtr.GetXaxis().GetNbins()):
                ibinLabel= topZMassRegionCtr.GetXaxis().GetBinLabel(i)
                if( ibinLabel != cat) : continue
                
                topYieldsVal      = topZMassRegionCtr.GetBinContent(i)
                topYieldsValError = topZMassRegionCtr.GetBinError(i)

                otherYieldsVal      = otherZMassRegionCtr.GetBinContent(i)
                otherYieldsValError = otherZMassRegionCtr.GetBinError(i)

                topYields.append( [topYieldsVal, topYieldsValError] )
                otherYields.append( [otherYieldsVal, otherYieldsValError] )

        if(topYields[0][0]>0):
            kTop=topYields[1][0]/topYields[0][0]
            kTopError=sqrt(pow(topYields[1][0]*topYields[0][1],2)
                           +pow(topYields[0][0]*topYields[1][1],2))/pow(topYields[0][0],2)
            topRescaleFactor.SetBinContent(ipoint,kTop);
            topRescaleFactor.SetBinError(ipoint,kTopError)
            topRescaleFactor.GetXaxis().SetBinLabel(ipoint,reg[0])

        if(otherYields[0][0]>0):
            kOther=otherYields[1][0]/otherYields[0][0]
            kOtherError=sqrt(pow(otherYields[1][0]*otherYields[0][1],2)
                           +pow(otherYields[0][0]*otherYields[1][1],2))/pow(otherYields[0][0],2)
            otherRescaleFactor.SetBinContent(ipoint,kOther);
            otherRescaleFactor.SetBinError(ipoint,kOtherError)
            otherRescaleFactor.GetXaxis().SetBinLabel(ipoint,reg[0])


    print '********* ' + reg[0] + ' *************'
    print topRescaleFactor.GetXaxis().GetNbins()
    topRescaleFactor.Fit(fitfunc,'RQ+')
    try :
        fmtValue = toLatexRounded(fitfunc.GetParameter(0),fitfunc.GetParError(0))
        fmtValue = fmtValue.replace("[","(")
        fmtValue = fmtValue.replace("]",")")
        print 'top: ' + fmtValue,
    except :
        print 'top: n/a'
    topRescalingHistos.Add(topRescaleFactor)

    try :
        otherRescaleFactor.Fit(fitfunc,'RQ+')
        fmtValue = toLatexRounded(fitfunc.GetParameter(0),fitfunc.GetParError(0))
        fmtValue = fmtValue.replace("[","(")
        fmtValue = fmtValue.replace("]",")")
        print '  others: ' + fmtValue
    except:
        print '  others: n/a'
    otherRescalingHistos.Add(otherRescaleFactor)
    

#display results
setStyle()

#show the inclusive output
ROOT.gStyle.SetOptFit(0)
ROOT.gStyle.SetOptStat(0)
c = getNewCanvas("kfactors","kfactors",False)
c.SetWindowSize(1000,500)
c.Divide(2,1)
spimpose=ROOT.TList()
stack=ROOT.TList()
p=c.cd(1)
leg=showSimplePlot(p,topRescalingHistos)
formatForCmsPublic(p,leg,'CMS simulation',3)
p=c.cd(2)
leg=showSimplePlot(p,otherRescalingHistos)
leg.Delete()
c.cd()
c.Modified()
c.Update()

raw_input('...')

# #sgow the differential weights
# cdiff = getNewCanvas("diffgammac","diffgammac",False)
# cdiff.SetWindowSize(500,500)
# cdiff.SetCanvasSize(500,500)
# ptWeights[1].Draw("colztext")
# cdiff.cd()
# cdiff.Modified()
# cdiff.Update()

# #save
# raw_input(' *** Any key to end')
# fout = ROOT.TFile.Open('gammaptweight.root','RECREATE')
# fout.cd()
# ptWeights[0].Write()
# ptWeights[1].Write()
# c.Write()
# cdiff.Write()
# fout.Close()

# print ' *** Gamma pT weights available @ gammaptweight.root'

