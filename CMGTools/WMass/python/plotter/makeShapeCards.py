#!/usr/bin/env python
from CMGTools.WMass.plotter.mcAnalysis import *
import re, sys, os, os.path
systs = {}

from optparse import OptionParser
parser = OptionParser(usage="%prog [options] mc.txt cuts.txt var bins systs.txt ")
addMCAnalysisOptions(parser)
parser.add_option("-o",   "--out",    dest="outname", type="string", default=None, help="output name") 
parser.add_option("--od", "--outdir", dest="outdir", type="string", default=None, help="output name") 
parser.add_option("-v", "--verbose",  dest="verbose",  default=0,  type="int",    help="Verbosity level (0 = quiet, 1 = verbose, 2+ = more)")
parser.add_option("--masses", dest="masses", default=None, type="string", help="produce results for all these masses")
parser.add_option("--mass-int-algo", dest="massIntAlgo", type="string", default="sigmaBR", help="Interpolation algorithm for nearby masses") 
parser.add_option("--asimov", dest="asimov", action="store_true", help="Asimov")
parser.add_option("--2d-binning-function",dest="binfunction", type="string", default=None, help="Function used to bin the 2D histogram: nbins:func, where func(x,y) = bin in [1,nbins]")
parser.add_option("--infile",dest="infile", type="string", default=None, help="File to read histos from")
parser.add_option("--savefile",dest="savefile", type="string", default=None, help="File to save histos to")
parser.add_option("--floatProcesses",dest="floatProcesses", type="string", default=None, help="Completely float the yields of these processes")
parser.add_option("--groupSystematics",dest="groupSystematics", type="string", nargs=2, default=None, help="Group systematics by [groupname pattern]")

(options, args) = parser.parse_args()
options.weight = True
options.final  = True
options.allProcesses  = True

if "/functions_cc.so" not in ROOT.gSystem.GetLibraries(): 
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/functions.cc+" % os.environ['CMSSW_BASE']);

mca  = MCAnalysis(args[0],options)
cuts = CutsFile(args[1],options)

truebinname = os.path.basename(args[1]).replace(".txt","") if options.outname == None else options.outname
binname = truebinname if truebinname[0] not in "234" else "ttH_"+truebinname
print binname
outdir  = options.outdir+"/" if options.outdir else ""

masses = [ 20 ]
if options.masses:
    masses = [ float(x) for x in open(options.masses) ]

floatProcs = []
if options.floatProcesses:
    floatProcs = [str(x) for x in options.floatProcesses]

def file2map(x):
    ret = {}; headers = []
    for x in open(x,"r"):
        cols = x.split()
        if len(cols) < 2: continue
        if "BR2" in x: # skip the errors
            cols = [cols[0]] + [c for (i,c) in enumerate(cols) if i % 3 == 1]
        if "mH" in x:
            headers = [i.strip() for i in cols[1:]]
        else:
            fields = [ float(i) for i in cols ]
            ret[fields[0]] = dict(zip(headers,fields[1:]))
    return ret
YRpath = os.environ['CMSSW_RELEASE_BASE']+"/src/HiggsAnalysis/CombinedLimit/data/lhc-hxswg/sm/";
#XStth = file2map(YRpath+"xs/8TeV/8TeV-ttH.txt")
BRhvv = file2map(YRpath+"br/BR2bosons.txt")
BRhff = file2map(YRpath+"br/BR2fermions.txt")
def mkspline(table,column,sf=1.0):
    pairs = [ (x,c[column]/sf) for (x,c) in table.iteritems() ]
    pairs.sort()
    x,y = ROOT.std.vector('double')(), ROOT.std.vector('double')()
    for xi,yi in pairs:
        x.push_back(xi) 
        y.push_back(yi) 
    spline = ROOT.ROOT.Math.Interpolator(x,y);
    spline._x = x
    spline._y = y
    return spline
def mkOneSpline():
    pairs = [ (100*x,1) for x in range(10) ]
    pairs.sort()
    x,y = ROOT.std.vector('double')(), ROOT.std.vector('double')()
    for xi,yi in pairs:
        x.push_back(xi) 
        y.push_back(yi) 
    spline = ROOT.ROOT.Math.Interpolator(x,y);
    spline._x = x
    spline._y = y
    return spline
splines = {
    'ttH' : mkOneSpline(),
    'hww' : mkOneSpline(),
    'hzz' : mkOneSpline(),
    'htt' : mkOneSpline(),
#    'ttH' : mkspline(XStth, "XS_pb",   0.1271 * 1.00757982823 ), ## get 1 at 125.7
#    'hww' : mkspline(BRhvv, "H_WW",    2.15e-01 ),
#    'hzz' : mkspline(BRhvv, "H_ZZ",    2.64e-02 ),
#    'htt' : mkspline(BRhff, "H_tautau",6.32e-02 ),
}
def getYieldScale(mass,process):
    if "ttH_" not in process: return 1.0
    scale = splines['ttH'].Eval(mass)
    for dec in "hww","hzz","htt":
        if dec in process: 
            scale *= splines[dec].Eval(mass)
            if 'efficiency_'+dec in splines:
                scale *= splines['efficiency_'+dec].Eval(mass)
            break
    return scale 

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
    for p in mca.listSignals() + mca.listBackgrounds():
        if p in report: tomerge.append(report[p])
    report['data_obs'] = mergePlots("x_data_obs", tomerge) 
else:
    report['data_obs'] = report['data'].Clone("x_data_obs") 

allyields = dict([(p,h.Integral()) for p,h in report.iteritems()])
procs = []; iproc = {}
for i,s in enumerate(mca.listSignals()):
    if allyields[s] == 0: continue
    procs.append(s); iproc[s] = i-len(mca.listSignals())+1
for i,b in enumerate(mca.listBackgrounds()):
    if allyields[b] == 0: continue
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
            if re.match(binmap+"$",truebinname) == None: continue
            if name not in systs: systs[name] = []
            systs[name].append((re.compile(procmap+"$"),amount))
        elif field[4] in ["envelop","shapeOnly","templates","alternateShape","alternateShapeOnly"] or '2D' in field[4]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap+"$",truebinname) == None: continue
            if name not in systs: systsEnv[name] = []
            systsEnv[name].append((re.compile(procmap+"$"),amount,field[4]))
        elif field[4] in ["stat_foreach_shape_bins"]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap+"$",truebinname) == None: continue
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
        if mca._projection != None and effect not in ["-","0","1"]:
            if "/" in effect:
                e1, e2 = effect.split("/")
                effect = "%.3f/%.3f" % (mca._projection.scaleSyst(name, float(e1)), mca._projection.scaleSyst(name, float(e2)))
            else:
                effect = str(mca._projection.scaleSyst(name, float(effect)))
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
        for entry in systsEnv[name]:
            procmap,amount,mode = entry[:3]
            if re.match(procmap, p): effect = float(amount) if mode not in ["templates","alternateShape", "alternateShapeOnly"] else amount
            print "EFFECT = ",effect
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
            nbinx = nominal.GetNbinsX()
            xmin = nominal.GetXaxis().GetBinCenter(1)
            xmax = nominal.GetXaxis().GetBinCenter(nbinx)
            if '2D' in mode:
                if 'TH2' not in nominal.ClassName(): raise RuntimeError, 'Trying to use 2D shape systs on a 1D histogram'
                nbiny = nominal.GetNbinsY()
                ymin = nominal.GetYaxis().GetBinCenter(1)
                ymax = nominal.GetYaxis().GetBinCenter(nbiny)
            c1def = lambda x: 2*(x-0.5) # straight line from (0,-1) to (1,+1)
            c2def = lambda x: 1 - 8*(x-0.5)**2 # parabola through (0,-1), (0.5,~1), (1,-1)
            if '2D' not in mode:
                if 'TH1' not in nominal.ClassName(): raise RuntimeError, 'Trying to use 1D shape systs on a 2D histogram'+nominal.ClassName()+" "+nominal.GetName()
                for b in xrange(1,nbinx+1):
                    x = (nominal.GetBinCenter(bx)-xmin)/(xmax-xmin)
                    c1 = c1def(x)
                    c2 = c2def(x)
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
                                    print "WARNING: for process %s in truebinname %s, bin %d has zero yield or zero error." % (p,truebinname,bin)
                                break
                            if (effect*nominal.GetBinError(bin)<0.1*sqrt(nominal.GetBinContent(bin)+0.04)):
                                if options.verbose: print 'skipping stat_foreach_shape_bins %s %d because it is irrelevant'%(p,bin)
                                break
                            p0Up = nominal.Clone("%s_%s_%s_%s_bin%dUp"% (nominal.GetName(),name,truebinname,p,bin))
                            p0Dn = nominal.Clone("%s_%s_%s_%s_bin%dDown"% (nominal.GetName(),name,truebinname,p,bin))
                            p0Up.SetBinContent(bin,nominal.GetBinContent(bin)+effect*nominal.GetBinError(bin))
                            p0Dn.SetBinContent(bin,nominal.GetBinContent(bin)**2/p0Up.GetBinContent(bin))
                            report[str(p0Up.GetName())[2:]] = p0Up
                            report[str(p0Dn.GetName())[2:]] = p0Dn
                            systsEnv2["%s_%s_%s_bin%d"%(name,truebinname,p,bin)] = (dict([(_p,"1" if _p==p else "-") for _p in procs]),dict([(_p,"1" if _p==p else "-") for _p in procs]),"templates")
                            break # otherwise you apply more than once to the same bin if more regexps match
            elif 'TH2' in nominal.ClassName():
                for binx in xrange(1,nominal.GetNbinsX()+1):
                    for biny in xrange(1,nominal.GetNbinsY()+1):
                        for binmatch in morefields[0]:
                            if re.match(binmatch+"$",'%d,%d'%(binx,biny)):
                                if nominal.GetBinContent(binx,biny) == 0 or nominal.GetBinError(binx,biny) == 0:
                                    if nominal.Integral() != 0: 
                                        print "WARNING: for process %s in truebinname %s, bin %d,%d has zero yield or zero error." % (p,truebinname,binx,biny)
                                    break
                                if (effect*nominal.GetBinError(binx,biny)<0.1*sqrt(nominal.GetBinContent(binx,biny)+0.04)):
                                    if options.verbose: print 'skipping stat_foreach_shape_bins %s %d,%d because it is irrelevant'%(p,binx,biny)
                                    break
                                p0Up = nominal.Clone("%s_%s_%s_%s_bin%d_%dUp"% (nominal.GetName(),name,truebinname,p,binx,biny))
                                p0Dn = nominal.Clone("%s_%s_%s_%s_bin%d_%dDown"% (nominal.GetName(),name,truebinname,p,binx,biny))
                                p0Up.SetBinContent(binx,biny,nominal.GetBinContent(binx,biny)+effect*nominal.GetBinError(binx,biny))
                                p0Dn.SetBinContent(binx,biny,nominal.GetBinContent(binx,biny)**2/p0Up.GetBinContent(binx,biny))
                                report[str(p0Up.GetName())[2:]] = p0Up
                                report[str(p0Dn.GetName())[2:]] = p0Dn
                                systsEnv2["%s_%s_%s_bin%d_%d"%(name,truebinname,p,binx,biny)] = (dict([(_p,"1" if _p==p else "-") for _p in procs]),dict([(_p,"1" if _p==p else "-") for _p in procs]),"templates")
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

## create efficiency scale factors, if needed
if len(masses) > 1 and options.massIntAlgo not in [ "sigmaBR","noeff"]:
    x = ROOT.std.vector('double')()
    y = { "hww": ROOT.std.vector('double')(), "hzz": ROOT.std.vector('double')(), "htt": ROOT.std.vector('double')() }
    for m in 110,115,120,130,135,140:
        x.push_back(m)
        for d in "htt","hww","hzz":
            p = "ttH_%s_%d" % (d,m)
            h0 = report[p] 
            h  = report["ttH_%s" % d] 
            #print "efficiency scale factor %s @ %d = %.3f" % (d,m,h.Integral()/h0.Integral())
            y[d].push_back(h.Integral()/h0.Integral())
        if m == 120:
            x.push_back(125)
            y["hww"].push_back(1)
            y["htt"].push_back(1)
            y["hzz"].push_back(1)
    for d in "htt","hww":#,"hzz": h->ZZ is bad
        splines["efficiency_"+d] = ROOT.ROOT.Math.Interpolator(x,y[d])

if len(masses) > 1:
    for mass in masses:
        smass = str(mass).replace(".0","")
        for p in "ttH_hww ttH_hzz ttH_htt".split():
            scale = getYieldScale(mass,p)
            posts = ['']
            for name,(effmap0,effmap12,mode) in systsEnv.iteritems():
                if re.match('envelop.*',mode) and effmap0[p] != "-": 
                    posts += [ "_%s%d%s" % (name,i,d) for (i,d) in [(0,'Up'),(0,'Down'),(1,'Up'),(1,'Down'),(2,'Up'),(2,'Down')]]
                elif re.match('shapeOnly2D.*',mode): 
                    posts += [ "_%s%d%s" % (name,i,d) for (i,d) in [(1,'Up'),(1,'Down'),]]
                elif effmap0[p]  != "-":
                    posts += [ "_%s%s" % (name,d) for d in ['Up','Down']]
                elif effmap12[p] != "-":
                    posts += [ "_%s%d%s" % (name,i,d) for (i,d) in [(1,'Up'),(1,'Down'),(2,'Up'),(2,'Down')]]
            for post in posts:
                template = report["%s%s" % (p,post)].Clone("x_%s%s%s" % (p,smass,post))
                template.Scale(scale)
                if options.massIntAlgo == "full":
                    pythias = [ 110,115,120,125,130,135,140]
                    pythias.sort(key = lambda x : abs(x-mass))
                    mpythia1 = pythias[0]
                    mpythia2 = pythias[1]
                    if mpythia1 > mpythia2: (mpythia1, mpythia2) = (mpythia2, mpythia1)
                    #print "for mass %s, take closest pythias from %d, %d" % (mass, mpythia1, mpythia2)
                    norm = template.Integral()
                    h0_m0 = report[p]
                    h0_m1  = report[("%s_%d" % (p,mpythia1)) if mpythia1 != 125 else p]
                    h0_m2  = report[("%s_%d" % (p,mpythia2)) if mpythia2 != 125 else p]
                    w1 = abs(mass-mpythia2)/abs(mpythia1-mpythia2)
                    w2 = abs(mass-mpythia1)/abs(mpythia1-mpythia2)
                    for b in xrange(1,template.GetNbinsX()+1):
                        avg = w1*h0_m1.GetBinContent(b) + w2*h0_m2.GetBinContent(b)
                        ref = h0_m0.GetBinContent(b)
                        if avg > 0 and ref > 0: 
                            template.SetBinContent(b, template.GetBinContent(b) * avg/ref)
                        #print "bin %d: m1 %7.4f   m2 %7.4f  avg %7.4f   ref %7.4f " % (b, h0_m1.GetBinContent(b), h0_m2.GetBinContent(b), avg, ref)
                    template.Scale(norm/template.Integral())
                    #exit(0)
                report["%s%s%s" % (p,smass,post)] = template
                #print "created x_%s%s%s" % (p,mass,post)
if len(masses) > 1: 
    if not os.path.exists(outdir+"/common"): os.mkdir(outdir+"/common")
for mass in masses:
    smass = str(mass).replace(".0","")
    myout = outdir
    if len(masses) > 1:
        myout += "%s/" % smass 
        if not os.path.exists(myout): os.mkdir(myout)
        myyields = dict([(k,getYieldScale(mass,k)*v) for (k,v) in allyields.iteritems()]) 
        datacard = open(myout+binname+".card.txt", "w"); 
        datacard.write("## Datacard for cut file %s (mass %s)\n"%(args[1],mass))
        datacard.write("shapes *        * ../common/%s.input.root x_$PROCESS x_$PROCESS_$SYSTEMATIC\n" % binname)
        datacard.write("shapes ttH_hww  * ../common/%s.input.root x_$PROCESS$MASS x_$PROCESS$MASS_$SYSTEMATIC\n" % binname)
        datacard.write("shapes ttH_hzz  * ../common/%s.input.root x_$PROCESS$MASS x_$PROCESS$MASS_$SYSTEMATIC\n" % binname)
        datacard.write("shapes ttH_htt  * ../common/%s.input.root x_$PROCESS$MASS x_$PROCESS$MASS_$SYSTEMATIC\n" % binname)
    else:
        myyields = dict([(k,v) for (k,v) in allyields.iteritems()]) 
        if not os.path.exists(myout): os.mkdir(myout)
        datacard = open(myout+binname+".card.txt", "w"); 
        datacard.write("## Datacard for cut file %s\n"%args[1])
        datacard.write("shapes *        * %s.input.root x_$PROCESS x_$PROCESS_$SYSTEMATIC\n" % binname)
    datacard.write('##----------------------------------\n')
    datacard.write('bin         %s\n' % binname)
    datacard.write('observation %s\n' % myyields['data_obs'])
    datacard.write('##----------------------------------\n')
    klen = max([7, len(binname)]+[len(p) for p in procs])
    kpatt = " %%%ds "  % klen
    fpatt = " %%%d.%df " % (klen,3)
    datacard.write('##----------------------------------\n')
    datacard.write('bin             '+(" ".join([kpatt % binname  for p in procs]))+"\n")
    datacard.write('process         '+(" ".join([kpatt % p        for p in procs]))+"\n")
    datacard.write('process         '+(" ".join([kpatt % iproc[p] for p in procs]))+"\n")
    datacard.write('rate            '+(" ".join([fpatt % myyields[p] for p in procs]))+"\n")
    datacard.write('##----------------------------------\n')
    for name,effmap in systs.iteritems():
        datacard.write(('%-12s lnN' % name) + " ".join([kpatt % effmap[p]   for p in procs]) +"\n")
    for name,(effmap0,effmap12,mode) in systsEnv.iteritems():
        if mode == "templates":
            datacard.write(('%-10s shape' % name) + " ".join([kpatt % effmap0[p]  for p in procs]) +"\n")
        if re.match('envelop.*',mode):
            datacard.write(('%-10s shape' % (name+"0")) + " ".join([kpatt % effmap0[p]  for p in procs]) +"\n")
        if any([re.match(x+'.*',mode) for x in ["envelop", "shapeOnly"]]):
            datacard.write(('%-10s shape' % (name+"1")) + " ".join([kpatt % effmap12[p] for p in procs]) +"\n")
            if "shapeOnly2D" not in mode:
                datacard.write(('%-10s shape' % (name+"2")) + " ".join([kpatt % effmap12[p] for p in procs]) +"\n")
    for fp in floatProcs:
        datacard.write('norm_'+fp+' rateParam * '+fp+' 1 [0.5,1.5]\n')
    if options.groupSystematics:
        datacard.write(options.groupSystematics[0]+' group = '+(" ".join([name for name,(effmap0,effmap12,mode) in systsEnv.iteritems() if options.groupSystematics[1] in name]))+"\n")
if len(masses) > 1:
    myout = outdir
    myyields = dict([(k,-1 if "ttH" in k else v) for (k,v) in allyields.iteritems()]) 
    if not os.path.exists(myout): os.mkdir(myout)
    datacard = open(myout+binname+".card.txt", "w"); 
    datacard.write("## Datacard for cut file %s (all massess, taking signal normalization from templates)\n")
    datacard.write("shapes *        * common/%s.input.root x_$PROCESS x_$PROCESS_$SYSTEMATIC\n" % binname)
    datacard.write("shapes ttH_hww  * common/%s.input.root x_$PROCESS$MASS x_$PROCESS$MASS_$SYSTEMATIC\n" % binname)
    datacard.write("shapes ttH_hzz  * common/%s.input.root x_$PROCESS$MASS x_$PROCESS$MASS_$SYSTEMATIC\n" % binname)
    datacard.write("shapes ttH_htt  * common/%s.input.root x_$PROCESS$MASS x_$PROCESS$MASS_$SYSTEMATIC\n" % binname)
    datacard.write('##----------------------------------\n')
    datacard.write('bin         %s\n' % binname)
    datacard.write('observation %s\n' % myyields['data_obs'])
    datacard.write('##----------------------------------\n')
    klen = max([7, len(binname)]+[len(p) for p in procs])
    kpatt = " %%%ds "  % klen
    fpatt = " %%%d.%df " % (klen,3)
    datacard.write('##----------------------------------\n')
    datacard.write('bin             '+(" ".join([kpatt % binname  for p in procs]))+"\n")
    datacard.write('process         '+(" ".join([kpatt % p        for p in procs]))+"\n")
    datacard.write('process         '+(" ".join([kpatt % iproc[p] for p in procs]))+"\n")
    datacard.write('rate            '+(" ".join([fpatt % myyields[p] for p in procs]))+"\n")
    datacard.write('##----------------------------------\n')
    for name,effmap in systs.iteritems():
        datacard.write(('%-12s lnN' % name) + " ".join([kpatt % effmap[p]   for p in procs]) +"\n")
    for name,(effmap0,effmap12,mode) in systsEnv.iteritems():
        if mode == "templates":
            datacard.write(('%-10s shape' % name) + " ".join([kpatt % effmap0[p]  for p in procs]) +"\n")
        if re.match('envelop.*',mode):
            datacard.write(('%-10s shape' % (name+"0")) + " ".join([kpatt % effmap0[p]  for p in procs]) +"\n")
        if any([re.match(x+'.*',mode) for x in ["envelop", "shapeOnly"]]):
            datacard.write(('%-10s shape' % (name+"1")) + " ".join([kpatt % effmap12[p] for p in procs]) +"\n")
            datacard.write(('%-10s shape' % (name+"2")) + " ".join([kpatt % effmap12[p] for p in procs]) +"\n")
    datacard.close()
    print "Wrote to ",myout+binname+".card.txt"
    if options.verbose:
        print "="*120
        os.system("cat %s.card.txt" % (myout+binname));
        print "="*120

myout = outdir+"/common/" if len(masses) > 1 else outdir;
workspace = ROOT.TFile.Open(myout+binname+".input.root", "RECREATE")
for n,h in report.iteritems():
    if options.verbose: print "\t%s (%8.3f events)" % (h.GetName(),h.Integral())
    workspace.WriteTObject(h,h.GetName())
workspace.Close()

print "Wrote to ",myout+binname+".input.root"

