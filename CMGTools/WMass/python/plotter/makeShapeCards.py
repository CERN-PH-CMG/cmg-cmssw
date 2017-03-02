#!/usr/bin/env python
from CMGTools.MonoXAnalysis.plotter.mcAnalysis import *
import ROOT
import re, sys, os, os.path, copy
import numpy as np

if "/functions_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/MonoXAnalysis/python/plotter/functions.cc+" % os.environ['CMSSW_BASE']);

systs = {}

from optparse import OptionParser
parser = OptionParser(usage="%prog [options] mc.txt cuts.txt var bins systs.txt ")
addMCAnalysisOptions(parser)
parser.add_option("-o",   "--out",    dest="outname", type="string", default=None, help="output name") 
parser.add_option("--od", "--outdir", dest="outdir", type="string", default=None, help="output name") 
parser.add_option("-v", "--verbose",  dest="verbose",  default=0,  type="int",    help="Verbosity level (0 = quiet, 1 = verbose, 2+ = more)")
parser.add_option("--asimov", dest="asimov", action="store_true", help="Asimov")
parser.add_option("--unbinned", dest="unbinned", action="store_true", help="Unbinned fit")
parser.add_option("--2d-binning-function",dest="binfunction", type="string", default=None, help="Function used to bin the 2D histogram: nbins:func, where func(x,y) = bin in [1,nbins]")
parser.add_option("--infile",dest="infile", type="string", default=None, help="File to read histos from")
parser.add_option("--savefile",dest="savefile", type="string", default=None, help="File to save histos to")
parser.add_option("--region",dest="region", type="string", default="SR", help="Phase space defined by the selection (SR,ZM,ZE,WE,WM,GJ)")
parser.add_option("--processesFromCR",dest="processesFromCR",action="append", default=[],help="For these processes, include a global normalization from control region for each bin of the shape")
parser.add_option("--correlateProcessCR",dest="correlateProcessCR",action="append", default=[],help="For each process to correlate to the signal region, add [process,signalregion,alphahist,filewherealphahist]")
parser.add_option("--appendWorkspace",dest="appendWorkspace", type="string", default=None,help="Do not create a new workspace, but import new objects into an existing one in this file")

(options, args) = parser.parse_args()
options.weight = True
options.final  = True
options.allProcesses  = True

mca  = MCAnalysis(args[0],options)
cuts = CutsFile(args[1],options)

binname = os.path.basename(args[1]).replace(".txt","") if options.outname == None else options.outname
outdir  = options.outdir+"/" if options.outdir else ""

class SafeWorkspaceImporter():
    """Class that provides the RooWorkspace::import method, but makes sure we call the proper
       overload of it, since in ROOT 6 sometimes PyROOT calls the wrong one"""
    def __init__(self,wsp):
        self.wsp = wsp
        self.imp = getattr(wsp,"import")
    def __call__(self,*args):
        if len(args) != 1:
            self.imp(*args)
        elif args[0].Class().InheritsFrom("RooAbsReal") or args[0].Class().InheritsFrom("RooArgSet") or args[0].Class().InheritsFrom("RooAbsData") or args[0].Class().InheritsFrom("RooCategory"):
            self.imp(args[0], ROOT.RooCmdArg()) # force the proper overload to be called
        else:
            self.imp(*args)

def addCorrelatedShape(process,var,region,workspace,hist):
    bins = []
    for b in range(1,hist.GetNbinsX()+1):
        bin_rrv = ROOT.RooRealVar(process+'_'+region+'_bin'+str(b),"",hist.GetBinContent(b), 0., hist.GetBinContent(b)*5.0)
        bins.append(bin_rrv)

    # for some ROOT memory handling, adding the RooRealVars to the RooArgList after creation doesn't work
    binlist = ROOT.RooArgList()
    for b in range(1,hist.GetNbinsX()+1): binlist.add(bins[b-1])

    procnorm = process+'_'+region+'_norm'
    phist = ROOT.RooParametricHist(process+'_'+region,"",var,binlist,hist)
    norm = ROOT.RooAddition(procnorm,"",binlist)
    _import = SafeWorkspaceImporter(workspace)
    _import(phist,ROOT.RooFit.RecycleConflictNodes())
    _import(norm,ROOT.RooFit.RecycleConflictNodes())

def addCorrelatedShapeFromSR(process,var,thisregion,correlatedRegion,workspace,hist,alphahist_fullerr,filealpha):
    tfile = ROOT.TFile.Open(filealpha)
    h_alphahist_fullerr = tfile.Get(alphahist_fullerr)
    h_alphahist_fullerr.SetDirectory(None)

    crbins = []; rbins = []; rerrbins = [];
    for b in range(1,hist.GetNbinsX()+1):
        crbin_rrv = ROOT.RooRealVar(process+'_'+correlatedRegion+'_bin'+str(b),"",hist.GetBinContent(b), 0., hist.GetBinContent(b)*5.0)
        crbins.append(crbin_rrv)
        # central value of the transfer factor
        rbin_rrv = ROOT.RooRealVar('r_'+process+'_'+thisregion+'_bin'+str(b),"",h_alphahist_fullerr.GetBinContent(b))
        rbins.append(rbin_rrv)
        # nuisance parameter in the fit limited to +/-5 sigma
        extreme = min(5.,h_alphahist_fullerr.GetBinContent(b)/h_alphahist_fullerr.GetBinError(b))
        rerrbin_rrv = ROOT.RooRealVar(process+'_'+thisregion+'_bin'+str(b)+'_Runc',"",0,-extreme,extreme)
        rerrbins.append(rerrbin_rrv)

    bins = []
    formula_comp = ROOT.RooArgList()
    for b in range(1,hist.GetNbinsX()+1):
        formula_comp.removeAll()
        formula_comp.add(crbins[b-1])
        formula_comp.add(rbins[b-1])
        formula_comp.add(rerrbins[b-1])
        # make the formula of the transfer from SR including the total error on alpha
        formula_toterr = "@0/(@1*abs(1+"+str(h_alphahist_fullerr.GetBinError(b)/h_alphahist_fullerr.GetBinContent(b))+"*@2))"

        bin_rfv = ROOT.RooFormulaVar(process+'_'+thisregion+'_bin'+str(b),"",formula_toterr, formula_comp)
        bins.append(bin_rfv)

    # for some ROOT memory handling, adding the RooRealVars to the RooArgList after creation doesn't work
    binlist = ROOT.RooArgList()
    for b in range(1,hist.GetNbinsX()+1):        
        binlist.add(bins[b-1])

    procnorm = process+'_'+thisregion+'_norm'
    phist = ROOT.RooParametricHist(process+'_'+thisregion,"",var,binlist,hist)
    norm = ROOT.RooAddition(procnorm,"",binlist)
    _import = SafeWorkspaceImporter(workspace)
    _import(phist,ROOT.RooFit.RecycleConflictNodes())
    _import(norm,ROOT.RooFit.RecycleConflictNodes())

def addTemplate(process,varlist,region,workspace,hist):
    data_hist = ROOT.RooDataHist(process+'_'+region,"",varlist,hist)
    _import = SafeWorkspaceImporter(workspace)
    _import(data_hist)

def getBinnedVar(varname,hist):
    binBoundaries = [hist.GetBinLowEdge(b) for b in range(1,hist.GetNbinsX()+2)]
    binning = ROOT.RooBinning(len(binBoundaries)-1,np.array(binBoundaries),varname+'_binning')
    rrv = ROOT.RooRealVar(varname,varname,hist.GetXaxis().GetXmin(),hist.GetXaxis().GetXmax())
    rrv.setBinning(binning)
    return rrv

import math
def rebin2Dto1D(h,funcstring):
    nbins,fname = funcstring.split(':',1)
    func = getattr(ROOT,fname)
    nbins = int(nbins)
    goodname = h.GetName()
    h.SetName(goodname+"_oldbinning")
    newh = ROOT.TH1D(goodname,h.GetTitle(),nbins,0.5,nbins+0.5)
    x = h.GetXaxis()
    y = h.GetYaxis()
    allowed = range(1,nbins+1)
    if 'TH2' not in h.ClassName(): raise RuntimeError, "Calling rebin2Dto1D on something that is not TH2"
    for i in xrange(x.GetNbins()):
        for j in xrange(y.GetNbins()):
            bin = int(func(x.GetBinCenter(i+1),y.GetBinCenter(j+1)))
            if bin not in allowed: raise RuntimeError, "Binning function gives not admissible result"
            newh.SetBinContent(bin,newh.GetBinContent(bin)+h.GetBinContent(i+1,j+1))
            newh.SetBinError(bin,math.hypot(newh.GetBinError(bin),h.GetBinError(i+1,j+1)))
    for bin in range(1,nbins+1):
        if newh.GetBinContent(bin)<0:
            print 'Warning: cropping to zero bin %d in %s (was %f)'%(bin,newh.GetName(),newh.GetBinContent(bin))
            newh.SetBinContent(bin,0)
    newh.SetLineWidth(h.GetLineWidth())
    newh.SetLineStyle(h.GetLineStyle())
    newh.SetLineColor(h.GetLineColor())
    return newh

masses = [ 125.0 ]

myout = outdir+"/common/" if len(masses) > 1 else outdir;

report={}
if options.infile!=None:
    infile = ROOT.TFile(myout+binname+".bare.root","read")
    for p in mca.listSignals(True)+mca.listBackgrounds(True)+['data']:
        h = infile.Get(p)
        if h: report[p] = h
else:
    report = mca.getPlotsRaw("x", args[2], args[3], cuts.allCuts(), nodata=options.asimov)

if options.savefile!=None:
    savefile = ROOT.TFile(myout+binname+".bare.root","recreate")
    for n,h in report.iteritems(): savefile.WriteTObject(h,n)
    savefile.Close()

if options.asimov:
    tomerge = []
    for p in mca.listBackgrounds():
        if p in report: tomerge.append(report[p])
    report['data_obs'] = mergePlots("x_data_obs", tomerge) 
else:
    report['data_obs'] = report['data'].Clone("x_data_obs") 

allyields = dict([(p,h.Integral()) for p,h in report.iteritems()])
procs = []; iproc = {}
signals, backgrounds = [], []
for i,s in enumerate(mca.listSignals()):
    if allyields[s] == 0: continue
    signals.append(s)
    procs.append(s); iproc[s] = i-len(mca.listSignals())+1
for i,b in enumerate(mca.listBackgrounds()):
    if allyields[b] == 0: continue
    backgrounds.append(b)
    procs.append(b); iproc[b] = i+1

systs = {}
systsEnv = {}
for sysfile in args[4:]:
    for line in open(sysfile, 'r'):
        if re.match("\s*#.*", line): continue
        line = re.sub("#.*","",line).strip()
        if len(line) == 0: continue
        field = [f.strip() for f in line.split(':')]
        if len(field) < 4:
            raise RuntimeError, "Malformed line %s in file %s"%(line.strip(),sysfile)
        elif len(field) == 4 or field[4] == "lnN":
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap+"$",binname) == None: continue
            if name not in systs: systs[name] = []
            systs[name].append((re.compile(procmap+"$"),amount))
        elif field[4] in ["envelop","shapeOnly","templates","alternateShape","alternateShapeOnly"] or '2D' in field[4]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap+"$",binname) == None: continue
            if name not in systs: systsEnv[name] = []
            systsEnv[name].append((re.compile(procmap+"$"),amount,field[4]))
        elif field[4] in ["stat_foreach_shape_bins"]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap+"$",binname) == None: continue
            if name not in systsEnv: systsEnv[name] = []
            systsEnv[name].append((re.compile(procmap+"$"),amount,field[4],field[5].split(',')))
        else:
            raise RuntimeError, "Unknown systematic type %s" % field[4]
    if options.verbose:
        print "Loaded %d systematics" % len(systs)
        print "Loaded %d envelop systematics" % len(systsEnv)

for name in systs.keys():
    effmap = {}
    for p in procs:
        effect = "-"
        for (procmap,amount) in systs[name]:
            if re.match(procmap, p): effect = amount
        effmap[p] = effect
    systs[name] = effmap

systsEnv1 = {}
for name in systsEnv.keys():
    modes = [entry[2] for entry in systsEnv[name]]
    for _m in modes:
        if _m!=modes[0]: raise RuntimeError, "Not supported"
    if not (any([re.match(x+'.*',modes[0]) for x in ["envelop","shapeOnly"]])): continue # do only this before rebinning
    effmap0  = {}
    effmap12 = {}
    for p in procs:
        effect = "-"
        effect0  = "-"
        effect12 = "-"
        for (procmap,amount,mode) in systsEnv[name]:
            if re.match(procmap, p): effect = float(amount) if mode not in ["templates","alternateShape", "alternateShapeOnly"] else amount
        if mca._projection != None and effect not in ["-","0","1",1.0,0.0] and type(effect) == type(1.0):
            effect = mca._projection.scaleSyst(name, effect)
        if effect == "-" or effect == "0": 
            effmap0[p]  = "-" 
            effmap12[p] = "-" 
            continue
        if any([re.match(x+'.*',mode) for x in ["envelop","shapeOnly"]]):
            nominal = report[p]
            p0up = nominal.Clone(nominal.GetName()+"_"+name+"0Up"  ); p0up.Scale(effect)
            p0dn = nominal.Clone(nominal.GetName()+"_"+name+"0Down"); p0dn.Scale(1.0/effect)
            p1up = nominal.Clone(nominal.GetName()+"_"+name+"1Up"  );
            p1dn = nominal.Clone(nominal.GetName()+"_"+name+"1Down");
            p2up = nominal.Clone(nominal.GetName()+"_"+name+"2Up"  );
            p2dn = nominal.Clone(nominal.GetName()+"_"+name+"2Down");
            nbin = nominal.GetNbinsX()
            xmin = nominal.GetBinCenter(1)
            xmax = nominal.GetBinCenter(nbin)
            if '2D' in mode:
                if 'TH2' not in nominal.ClassName(): raise RuntimeError, 'Trying to use 2D shape systs on a 1D histogram'
                nbiny = nominal.GetNbinsY()
                ymin = nominal.GetYaxis().GetBinCenter(1)
                ymax = nominal.GetYaxis().GetBinCenter(nbiny)
            c1def = lambda x: 2*(x-0.5) # straight line from (0,-1) to (1,+1)
            c2def = lambda x: 1 - 8*(x-0.5)**2 # parabola through (0,-1), (0.5,~1), (1,-1)
            if '2D' not in mode:
                if 'TH1' not in nominal.ClassName(): raise RuntimeError, 'Trying to use 1D shape systs on a 2D histogram'+nominal.ClassName()+" "+nominal.GetName()
                for b in xrange(1,nbin+1):
                    x = (nominal.GetBinCenter(b)-xmin)/(xmax-xmin)
                    c1 = 2*(x-0.5)         # straight line from (0,-1) to (1,+1)
                    c2 = 1 - 8*(x-0.5)**2  # parabola through (0,-1), (0.5,~1), (1,-1)
                    p1up.SetBinContent(b, p1up.GetBinContent(b) * pow(effect,+c1))
                    p1dn.SetBinContent(b, p1dn.GetBinContent(b) * pow(effect,-c1))
                    p2up.SetBinContent(b, p2up.GetBinContent(b) * pow(effect,+c2))
                    p2dn.SetBinContent(b, p2dn.GetBinContent(b) * pow(effect,-c2))
            else: # e.g. shapeOnly2D_1.25X_0.83Y with effect == 1 will do an anti-correlated shape distorsion of the x and y axes by 25% and -20% respectively
                parsed = mode.split('_')
                if len(parsed)!=3 or parsed[0]!="shapeOnly2D" or effect!=1: raise RuntimeError, 'Incorrect option parsing for shapeOnly2D: %s %s'%(mode,effect)
                effectX = float(parsed[1].strip('X'))
                effectY = float(parsed[2].strip('Y'))
                for bx in xrange(1,nbinx+1):
                    for by in xrange(1,nbiny+1):
                        x = (nominal.GetXaxis().GetBinCenter(bx)-xmin)/(xmax-xmin)
                        y = (nominal.GetYaxis().GetBinCenter(by)-ymin)/(ymax-ymin)
                        c1X = c1def(x)
                        c2X = c2def(x)
                        c1Y = c1def(y)
                        c2Y = c2def(y)
                        p1up.SetBinContent(bx,by, p1up.GetBinContent(bx,by) * pow(effectX,+c1X) * pow(effectY,+c1Y))
                        p1dn.SetBinContent(bx,by, p1dn.GetBinContent(bx,by) * pow(effectX,-c1X) * pow(effectY,-c1Y))
                        p2up.SetBinContent(bx,by, nominal.GetBinContent(bx,by))
                        p2dn.SetBinContent(bx,by, nominal.GetBinContent(bx,by))
#                        p2up.SetBinContent(bx,by, p2up.GetBinContent(bx,by) * pow(effectX,+c2X) * pow(effectY,+c2Y))
#                        p2dn.SetBinContent(bx,by, p2dn.GetBinContent(bx,by) * pow(effectX,-c2X) * pow(effectY,-c2Y))
            p1up.Scale(nominal.Integral()/p1up.Integral())
            p1dn.Scale(nominal.Integral()/p1dn.Integral())
            p2up.Scale(nominal.Integral()/p2up.Integral())
            p2dn.Scale(nominal.Integral()/p2dn.Integral())
            if "shapeOnly" not in mode:
                report[p+"_"+name+"0Up"]   = p0up
                report[p+"_"+name+"0Down"] = p0dn
                effect0 = "1"
            report[p+"_"+name+"1Up"]   = p1up
            report[p+"_"+name+"1Down"] = p1dn
#            report[p+"_"+name+"2Up"]   = p2up
#            report[p+"_"+name+"2Down"] = p2dn
            effect12 = "1"
            # useful for plotting
            for h in p0up, p0dn, p1up, p1dn, p2up, p2dn: 
                h.SetFillStyle(0); h.SetLineWidth(2)
            for h in p1up, p1dn: h.SetLineColor(4)
            for h in p2up, p2dn: h.SetLineColor(2)
        effmap0[p]  = effect0 
        effmap12[p] = effect12 
    systsEnv1[name] = (effmap0,effmap12,mode)

if options.binfunction:
    newhistos={}
    _to_be_rebinned={}
    for n,h in report.iteritems(): _to_be_rebinned[h.GetName()]=h
    for n,h in _to_be_rebinned.iteritems():
        thisname = h.GetName()
        newhistos[thisname]=rebin2Dto1D(h,options.binfunction)
    for n,h in report.iteritems(): report[n] = newhistos[h.GetName().replace('_oldbinning','')]
    allyields = dict([(p,h.Integral()) for p,h in report.iteritems()])
    procs = []; iproc = {}
    for i,s in enumerate(mca.listSignals()):
        if allyields[s] == 0: continue
        procs.append(s); iproc[s] = i-len(mca.listSignals())+1
    for i,b in enumerate(mca.listBackgrounds()):
        if allyields[b] == 0: continue
        procs.append(b); iproc[b] = i+1

systsEnv2={}
for name in systsEnv.keys():
    modes = [entry[2] for entry in systsEnv[name]]
    for _m in modes:
        if _m!=modes[0]: raise RuntimeError, "Not supported"
    if (any([re.match(x+'.*',modes[0]) for x in ["envelop","shapeOnly"]])): continue # do only this before rebinning
    effmap0  = {}
    effmap12 = {}
    for p in procs:
        effect = "-"
        effect0  = "-"
        effect12 = "-"
        for entry in systsEnv[name]:
            procmap,amount,mode = entry[:3]
            if re.match(procmap, p):
                effect = float(amount) if mode not in ["templates","alternateShape", "alternateShapeOnly"] else amount
                morefields=entry[3:]
        if mca._projection != None and effect not in ["-","0","1",1.0,0.0] and type(effect) == type(1.0):
            effect = mca._projection.scaleSyst(name, effect)
        if effect == "-" or effect == "0": 
            effmap0[p]  = "-" 
            effmap12[p] = "-" 
            continue
        if mode in ["stat_foreach_shape_bins"]:
            if mca._projection != None:
                raise RuntimeError,'mca._projection.scaleSystTemplate not implemented in the case of stat_foreach_shape_bins'
            nominal = report[p]
            if 'TH1' in nominal.ClassName():
                for bin in xrange(1,nominal.GetNbinsX()+1):
                    for binmatch in morefields[0]:
                        if re.match(binmatch+"$",'%d'%bin):
                            if nominal.GetBinContent(bin) == 0 or nominal.GetBinError(bin) == 0:
                                if nominal.Integral() != 0: 
                                    print "WARNING: for process %s in binname %s, bin %d has zero yield or zero error." % (p,binname,bin)
                                break
                            if (effect*nominal.GetBinError(bin)<0.1*sqrt(nominal.GetBinContent(bin)+0.04)):
                                if options.verbose: print 'skipping stat_foreach_shape_bins %s %d because it is irrelevant'%(p,bin)
                                break
                            p0Up = nominal.Clone("%s_%s_%s_%s_bin%dUp"% (nominal.GetName(),name,binname,p,bin))
                            p0Dn = nominal.Clone("%s_%s_%s_%s_bin%dDown"% (nominal.GetName(),name,binname,p,bin))
                            p0Up.SetBinContent(bin,nominal.GetBinContent(bin)+effect*nominal.GetBinError(bin))
                            p0Dn.SetBinContent(bin,nominal.GetBinContent(bin)**2/p0Up.GetBinContent(bin))
                            report[str(p0Up.GetName())[2:]] = p0Up
                            report[str(p0Dn.GetName())[2:]] = p0Dn
                            systsEnv2["%s_%s_%s_bin%d"%(name,binname,p,bin)] = (dict([(_p,"1" if _p==p else "-") for _p in procs]),dict([(_p,"1" if _p==p else "-") for _p in procs]),"templates")
                            break # otherwise you apply more than once to the same bin if more regexps match
            elif 'TH2' in nominal.ClassName():
                for binx in xrange(1,nominal.GetNbinsX()+1):
                    for biny in xrange(1,nominal.GetNbinsY()+1):
                        for binmatch in morefields[0]:
                            if re.match(binmatch+"$",'%d,%d'%(binx,biny)):
                                if nominal.GetBinContent(binx,biny) == 0 or nominal.GetBinError(binx,biny) == 0:
                                    if nominal.Integral() != 0: 
                                        print "WARNING: for process %s in binname %s, bin %d,%d has zero yield or zero error." % (p,binname,binx,biny)
                                    break
                                if (effect*nominal.GetBinError(binx,biny)<0.1*sqrt(nominal.GetBinContent(binx,biny)+0.04)):
                                    if options.verbose: print 'skipping stat_foreach_shape_bins %s %d,%d because it is irrelevant'%(p,binx,biny)
                                    break
                                p0Up = nominal.Clone("%s_%s_%s_%s_bin%d_%dUp"% (nominal.GetName(),name,binname,p,binx,biny))
                                p0Dn = nominal.Clone("%s_%s_%s_%s_bin%d_%dDown"% (nominal.GetName(),name,binname,p,binx,biny))
                                p0Up.SetBinContent(binx,biny,nominal.GetBinContent(binx,biny)+effect*nominal.GetBinError(binx,biny))
                                p0Dn.SetBinContent(binx,biny,nominal.GetBinContent(binx,biny)**2/p0Up.GetBinContent(binx,biny))
                                report[str(p0Up.GetName())[2:]] = p0Up
                                report[str(p0Dn.GetName())[2:]] = p0Dn
                                systsEnv2["%s_%s_%s_bin%d_%d"%(name,binname,p,binx,biny)] = (dict([(_p,"1" if _p==p else "-") for _p in procs]),dict([(_p,"1" if _p==p else "-") for _p in procs]),"templates")
                                break # otherwise you apply more than once to the same bin if more regexps match
        elif mode in ["templates"]:
            nominal = report[p]
            p0Up = report["%s_%s_Up" % (p, effect)]
            p0Dn = report["%s_%s_Dn" % (p, effect)]
            if not p0Up or not p0Dn: 
                raise RuntimeError, "Missing templates %s_%s_(Up,Dn) for %s" % (p,effect,name)
            p0Up.SetName("%s_%sUp"   % (nominal.GetName(),name))
            p0Dn.SetName("%s_%sDown" % (nominal.GetName(),name))
            if p0Up.Integral()<=0 or p0Dn.Integral()<=0:
                if p0Up.Integral()<=0 and p0Dn.Integral()<=0: raise RuntimeError, 'ERROR: both template variations have negative or zero integral: %s, Nominal %f, Up %f, Down %f'%(p,nominal.Integral(),p0Up.Integral(),p0Dn.Integral())
                print 'Warning: I am going to fix a template prediction that would have negative or zero integral: %s, Nominal %f, Up %f, Down %f'%(p,nominal.Integral(),p0Up.Integral(),p0Dn.Integral())
                for b in xrange(1,nominal.GetNbinsX()+1):
                    y0 = nominal.GetBinContent(b)
                    yA = p0Up.GetBinContent(b) if p0Up.Integral()>0 else p0Dn.GetBinContent(b)
                    yM = y0
                    if (y0 > 0 and yA > 0):
                        yM = y0*y0/yA
                    elif yA == 0:
                        yM = 2*y0
                    if p0Up.Integral()>0: p0Dn.SetBinContent(b, yM)
                    else: p0Up.SetBinContent(b, yM)
                print 'The integral is now: %s, Nominal %f, Up %f, Down %f'%(p,nominal.Integral(),p0Up.Integral(),p0Dn.Integral())
            report[str(p0Up.GetName())[2:]] = p0Up
            report[str(p0Dn.GetName())[2:]] = p0Dn
            effect0  = "1"
            effect12 = "-"
            if mca._projection != None:
                mca._projection.scaleSystTemplate(name,nominal,p0Up)
                mca._projection.scaleSystTemplate(name,nominal,p0Dn)
        elif mode in ["alternateShape", "alternateShapeOnly"]:
            nominal = report[p]
            alternate = report[effect]
            if mca._projection != None:
                mca._projection.scaleSystTemplate(name,nominal,alternate)
            alternate.SetName("%s_%sUp" % (nominal.GetName(),name))
            if mode == "alternateShapeOnly":
                alternate.Scale(nominal.Integral()/alternate.Integral())
            mirror = nominal.Clone("%s_%sDown" % (nominal.GetName(),name))
            for b in xrange(1,nominal.GetNbinsX()+1):
                y0 = nominal.GetBinContent(b)
                yA = alternate.GetBinContent(b)
                yM = y0
                if (y0 > 0 and yA > 0):
                    yM = y0*y0/yA
                elif yA == 0:
                    yM = 2*y0
                mirror.SetBinContent(b, yM)
            if mode == "alternateShapeOnly":
                # keep same normalization
                mirror.Scale(nominal.Integral()/mirror.Integral())
            else:
                # mirror normalization
                mnorm = (nominal.Integral()**2)/alternate.Integral()
                mirror.Scale(mnorm/alternate.Integral())
            report[alternate.GetName()] = alternate
            report[mirror.GetName()] = mirror
            effect0  = "1"
            effect12 = "-"
        effmap0[p]  = effect0 
        effmap12[p] = effect12 
    if mode not in ["stat_foreach_shape_bins"]: systsEnv2[name] = (effmap0,effmap12,mode)

systsEnv = {}
systsEnv.update(systsEnv1)
systsEnv.update(systsEnv2)

for mass in masses:
    smass = str(mass).replace(".0","")
    myout = outdir
    myout += "%s/" % mass
    myyields = dict([(k,v) for (k,v) in allyields.iteritems()]) 
    if not os.path.exists(myout): os.mkdir(myout)
    datacard = open(myout+binname+".card.txt", "w"); 
    datacard.write("## Datacard for cut file %s\n"%args[1])
    klen = max([7, len(binname)]+[len(p) for p in procs])
    kpatt = " %%%ds "  % klen
    fpatt = " %%%d.%df " % (klen,3)
    datacard.write('##----------------------------------\n')
    if options.unbinned:
        wsfile = binname+".input.root" if options.appendWorkspace == None else options.appendWorkspace
        myunbinnedyields = {}
        for proc in procs:
            myunbinnedyields[proc] = -1
            if len(options.processesFromCR):
                for p0 in options.processesFromCR:
                    for p in p0.split(","): 
                        if re.match(p+"$", proc):  myunbinnedyields[proc] = 1
            datacard.write(('shapes %-10s %-7s %-20s' % (proc,binname,wsfile))+" w:"+ proc + "_" + options.region)
            if myunbinnedyields[proc]==-1: datacard.write("%30s" % ("     w:"+ proc + "_" + options.region + "$SYSTEMATIC\n"))
            else:  datacard.write("\n")
        if len(options.correlateProcessCR):
            for p0 in options.correlateProcessCR:
                corr_proc = p0.split(",")[0]
                datacard.write(('shapes %-10s %-7s %-20s' % (corr_proc,binname,wsfile))+" w:"+ corr_proc + "_" + options.region+"\n")
        datacard.write(('shapes %-10s %-7s %-20s' % ("data_obs",binname,wsfile))+" w:data_obs_" + options.region+"\n")
        datacard.write('##----------------------------------\n')
        datacard.write('bin         %s\n' % binname)
        datacard.write('observation -1\n')
        datacard.write('##----------------------------------\n')
        datacard.write('##----------------------------------\n')
        if len(options.correlateProcessCR):
            for p0 in options.correlateProcessCR:
                pars = p0.split(",")
                corr_proc = pars[0]
                datacard.write('bin                          '+(" ".join([kpatt % binname  for p in procs + [""]]))+"\n")
                datacard.write('process                      '+(" ".join([kpatt % p        for p in procs]))+(kpatt % corr_proc)+"\n")
                datacard.write('process                      '+(" ".join([kpatt % iproc[p] for p in procs]))+(kpatt % str(len(procs)+1))+"\n")
                datacard.write('rate                         '+(" ".join([kpatt % myunbinnedyields[p] for p in procs]))+(kpatt % "1")+"\n")
        else:
            datacard.write('bin                          '+(" ".join([kpatt % binname  for p in procs]))+"\n")
            datacard.write('process                      '+(" ".join([kpatt % p        for p in procs]))+"\n")
            datacard.write('process                      '+(" ".join([kpatt % iproc[p] for p in procs]))+"\n")
            datacard.write('rate                         '+(" ".join([kpatt % myunbinnedyields[p] for p in procs]))+"\n")
        datacard.write('##----------------------------------\n')
    else:
        datacard.write("shapes *        * %s.input.root x_$PROCESS x_$PROCESS_$SYSTEMATIC\n" % binname)
        datacard.write('##----------------------------------\n')
        datacard.write('bin         %s\n' % binname)
        datacard.write('observation %s\n' % myyields['data_obs'])
        datacard.write('##----------------------------------\n')
        datacard.write('##----------------------------------\n')
        datacard.write('bin                          '+(" ".join([kpatt % binname  for p in procs]))+"\n")
        datacard.write('process                      '+(" ".join([kpatt % p        for p in procs]))+"\n")
        datacard.write('process                      '+(" ".join([kpatt % iproc[p] for p in procs]))+"\n")
        datacard.write('rate                         '+(" ".join([fpatt % myyields[p] for p in procs]))+"\n")
        datacard.write('##----------------------------------\n')
    dummy_syst = '' if not len(options.correlateProcessCR) else (kpatt % '-')
    for name,effmap in systs.iteritems():
        datacard.write(('%-25s lnN' % name) + " ".join([kpatt % effmap[p]   for p in procs]) + dummy_syst +"\n")
    for name,(effmap0,effmap12,mode) in systsEnv.iteritems():
        if mode == "templates":
            datacard.write(('%-10s shape' % name) + " ".join([kpatt % effmap0[p]  for p in procs]) + dummy_syst + "\n")
        if re.match('envelop.*',mode):
            datacard.write(('%-10s shape' % (name+"0")) + " ".join([kpatt % effmap0[p]  for p in procs]) + dummy_syst + "\n")
        if any([re.match(x+'.*',mode) for x in ["envelop", "shapeOnly"]]):
            datacard.write(('%-10s shape' % (name+"1")) + " ".join([kpatt % effmap12[p] for p in procs]) + dummy_syst + "\n")
            if "shapeOnly2D" not in mode:
                datacard.write(('%-10s shape' % (name+"2")) + " ".join([kpatt % effmap12[p] for p in procs]) + dummy_syst + "\n")
    if len(options.correlateProcessCR):
        for p0 in options.correlateProcessCR:
            corr_proc = p0.split(",")[0]
            nbins = next(report.itervalues()).GetNbinsX()
            for b in range(1,nbins+1): 
                datacard.write(('%-20s param    %-7d %-7d' % ("_".join([corr_proc,options.region,("bin%d"%b),"Runc"]),0,1 )) + "\n")

for mass in masses:
    myout = outdir + ("%s/" % mass)
    if options.appendWorkspace == None:
        fout = myout+binname+".input.root"
        input_file = ROOT.TFile.Open(fout,"recreate")
        workspace = ROOT.RooWorkspace("w","workspace")
    else:
        fout = myout+"/"+options.appendWorkspace
        input_file = ROOT.TFile.Open(fout,"update")
        workspace = input_file.Get("w")

    var = getBinnedVar("x",report.itervalues().next())
    varlist = ROOT.RooArgList(var)

    if len(options.correlateProcessCR):
        for p0 in options.correlateProcessCR:
            pars = p0.split(",")
            proc = pars[0]; corr_region = pars[1]; alphahist = pars[2]; filealpha = pars[3]
            addCorrelatedShapeFromSR(proc,var,options.region,corr_region,workspace,h,alphahist,filealpha)
    
    for n,h in report.iteritems():
        if options.verbose > 0: print "\t%s (%8.3f events)" % (h.GetName(),h.Integral())
        proc = "_".join((h.GetName()).split("_")[1:])
        if proc == "data": continue
        simpleTemplate = True
        if len(options.processesFromCR):
            for p0 in options.processesFromCR:
                for p in p0.split(","):
                    if re.match(p+"$", proc): 
                        simpleTemplate = False
                        addCorrelatedShape(proc,var,options.region,workspace,h)
        if simpleTemplate: 
            print "adding template for process ",proc
            addTemplate(proc,varlist,options.region,workspace,h)

    workspace.writeToFile(fout,ROOT.kTRUE)

    if options.verbose > -1:
        print "Wrote to ",fout
