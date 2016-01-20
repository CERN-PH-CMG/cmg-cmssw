#!/usr/bin/env python
from CMGTools.TTHAnalysis.plotter.mcAnalysis import *
import re, sys, os, os.path
systs = {}

from optparse import OptionParser
parser = OptionParser(usage="%prog [options] mc.txt cuts.txt var bins systs.txt ")
addMCAnalysisOptions(parser)
parser.add_option("-o",   "--out",    dest="outname", type="string", default=None, help="output name") 
parser.add_option("--od", "--outdir", dest="outdir", type="string", default=None, help="output name") 
parser.add_option("-v", "--verbose",  dest="verbose",  default=0,  type="int",    help="Verbosity level (0 = quiet, 1 = verbose, 2+ = more)")
parser.add_option("--asimov", dest="asimov", action="store_true", help="Asimov")
parser.add_option("--postfix-pred",dest="postfixmap", type="string", default=[], action="append", help="Function to apply to prediction, to correct it before running limits")

(options, args) = parser.parse_args()
options.weight = True
options.final  = True
options.allProcesses  = True

def cutCentralValueAtZero(mca,cut,pname,oldplots):
    if pname=='data': return
    oldplot = oldplots[pname]
    for b in xrange(1,oldplot.GetNbinsX()+1):
        if oldplot.GetBinContent(b)<=0:
            old = oldplot.GetBinContent(b),oldplot.GetBinError(b)
            oldplot.SetBinContent(b,1e-5)
            oldplot.SetBinError(b,1e-6)
            if old!=(0,0): print 'cutCentralValueAtZero: Fixing bin %d in %s: set to %f +/- %f (was %f +/- %f)'%(b,pname,oldplot.GetBinContent(b),oldplot.GetBinError(b),old[0],old[1])

def takeFakesPredictionFromMC(mca,cut,pname,oldplots):
    if pname=='data': return
    oldplot = oldplots[pname]
    for b in xrange(1,oldplot.GetNbinsX()+1):
        if oldplot.GetBinContent(b)<=0:
            old= oldplot.GetBinContent(b),oldplot.GetBinError(b)
            oldplot.SetBinContent(b,1e-5)
            new = oldplots['_special_TT_foremptybins'].GetBinContent(b)
            oldplot.SetBinError(b,new if new>1e-4 else 1e-6)
            print 'takeFakesPredictionFromMC: Fixing bin %d in %s: set to %f +/- %f (was %f +/- %f)'%(b,pname,oldplot.GetBinContent(b),oldplot.GetBinError(b),old[0],old[1])

def normTo1Observed(mca,cut,pname,oldplots):
    if pname=='data': return
    oldplot = oldplots[pname]
    old = oldplot.Integral()
    oldplot.Scale(1.0/old)
    print 'normTo1Observed: Normalizing %s to 1 (was %f)'%(pname,old)

def compilePostFixMap(inlist,relist):
    for m in inlist:
        dset,function = m.split("=")
        if dset[-1] == "*": dset = dset[:-1]
        else: raise RuntimeError, 'Incorrect plotscalemap format: %s'%m
        relist.append((re.compile(dset.strip()+"$"),globals()[function]))
postfixes = []
compilePostFixMap(options.postfixmap,postfixes)

mca  = MCAnalysis(args[0],options)
cuts = CutsFile(args[1],options)

binname = os.path.basename(args[1]).replace(".txt","") if options.outname == None else options.outname
outdir  = options.outdir+"/" if options.outdir else ""

report = mca.getPlotsRaw("x", args[2], args[3], cuts.allCuts(), nodata=options.asimov)


for post in postfixes:
    for rep in report:
        if re.match(post[0],rep): post[1](mca,cuts.allCuts(),rep,report)
if '_special_TT_foremptybins' in report: del report['_special_TT_foremptybins']

#def fixClopperPearsonForXG0b(mca,cut,pname,oldplot):
#    for b in xrange(1,oldplot.GetNbinsX()+1):
#        cut_b = CutsFile([['mymergedcuts',cut]],ignoreEmptyOptionsEnforcement=True)
#        cut_b.add('bin%d'%(b+1),"((%s)==%d)"%(options.doPrintOutNev,(b+1)))
#        eventcounts = mca.getYields(cut_b,makeSummary=True)
#        for p in eventcounts: eventcounts[p] = dict(eventcounts[p])['all']
#        nev = eventcounts[pname][2]
#        if nev>1: nulla
        

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
    if (s not in allyields) or allyields[s] == 0: continue
    signals.append(s)
    procs.append(s); iproc[s] = i-len(mca.listSignals())+1
for i,b in enumerate(mca.listBackgrounds()):
    if (b not in allyields) or allyields[b] == 0: continue
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
            if re.match(binmap,binname) == None: continue
            if name not in systs: systs[name] = []
            systs[name].append((re.compile(procmap),amount))
        elif field[4] in ["envelop","shapeOnly","templates","alternateShapeOnly"]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap,binname) == None: continue
            if name not in systsEnv: systsEnv[name] = []
            systsEnv[name].append((re.compile(procmap),amount,field[4]))
        elif field[4] in ["lnN_in_shape_bins","stat_foreach_shape_bins"]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap,binname) == None: continue
            if name not in systsEnv: systsEnv[name] = []
            systsEnv[name].append((re.compile(procmap),amount,field[4],field[5].split(',')))
        else:
            raise RuntimeError, "Unknown systematic type %s" % field[4]
    if options.verbose > 0:
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

for name in systsEnv.keys():
    effmap0  = {}
    effmap12 = {}
    for p in procs:
        effect = "-"
        effect0  = "-"
        effect12 = "-"
        for entry in systsEnv[name]:
            (procmap,amount,mode) = entry[:3]
            if re.match(procmap, p): effect = float(amount) if mode not in ["templates","alternateShape", "alternateShapeOnly"] else amount
            morefields=entry[3:]
        if mca._projection != None and effect not in ["-","0","1",1.0,0.0] and type(effect) == type(1.0):
            effect = mca._projection.scaleSyst(name, effect)
        if effect == "-" or effect == "0": 
            effmap0[p]  = "-" 
            effmap12[p] = "-" 
            continue
        if mode in ["envelop","shapeOnly"]:
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
            for b in xrange(1,nbin+1):
                x = (nominal.GetBinCenter(b)-xmin)/(xmax-xmin)
                c1 = 2*(x-0.5)         # straight line from (0,-1) to (1,+1)
                c2 = 1 - 8*(x-0.5)**2  # parabola through (0,-1), (0.5,~1), (1,-1)
                p1up.SetBinContent(b, p1up.GetBinContent(b) * pow(effect,+c1))
                p1dn.SetBinContent(b, p1dn.GetBinContent(b) * pow(effect,-c1))
                p2up.SetBinContent(b, p2up.GetBinContent(b) * pow(effect,+c2))
                p2dn.SetBinContent(b, p2dn.GetBinContent(b) * pow(effect,-c2))
            if mode != "shapeOnly":
                report[p+"_"+name+"0Up"]   = p0up
                report[p+"_"+name+"0Down"] = p0dn
                effect0 = "1"
            report[p+"_"+name+"1Up"]   = p1up
            report[p+"_"+name+"1Down"] = p1dn
            report[p+"_"+name+"2Up"]   = p2up
            report[p+"_"+name+"2Down"] = p2dn
            effect12 = "1"
            # useful for plotting
            for h in p0up, p0dn, p1up, p1dn, p2up, p2dn: 
                h.SetFillStyle(0); h.SetLineWidth(2)
            for h in p1up, p1dn: h.SetLineColor(4)
            for h in p2up, p2dn: h.SetLineColor(2)
        elif mode in ["templates"]:
            nominal = report[p]
            p0Up = report["%s_%s_Up" % (p, effect)]
            p0Dn = report["%s_%s_Dn" % (p, effect)]
            if not p0Up or not p0Dn: 
                raise RuntimeError, "Missing templates %s_%s_(Up,Dn) for %s" % (p,effect,name)
            p0Up.SetName("%s_%sUp"   % (nominal.GetName(),name))
            p0Dn.SetName("%s_%sDown" % (nominal.GetName(),name))
            report[str(p0Up.GetName())[2:]] = p0Up
            report[str(p0Dn.GetName())[2:]] = p0Dn
            effect0  = "1"
            effect12 = "-"
            if mca._projection != None:
                mca._projection.scaleSystTemplate(name,nominal,p0Up)
                mca._projection.scaleSystTemplate(name,nominal,p0Dn)
        elif mode in ["lnN_in_shape_bins"]:
            nominal = report[p]
            p0Up = nominal.Clone("%s_%sUp"% (nominal.GetName(),name))
            p0Dn = nominal.Clone("%s_%sDn"% (nominal.GetName(),name))
            for bin in xrange(1,nominal.GetNbinsX()+1):
                for binmatch in morefields[0]:
                    if re.match(binmatch,'%d'%bin):
                        p0Up.SetBinContent(bin,p0Up.GetBinContent(bin)*effect)
                        p0Up.SetBinError(bin,p0Up.GetBinError(bin)*effect)
                        p0Dn.SetBinContent(bin,p0Dn.GetBinContent(bin)/effect)
                        p0Dn.SetBinError(bin,p0Dn.GetBinError(bin)/effect)
                        break # otherwise you apply more than once to the same bin if more regexps match
            p0Up.SetName("%s_%sUp"   % (nominal.GetName(),name))
            p0Dn.SetName("%s_%sDown" % (nominal.GetName(),name))
            report[str(p0Up.GetName())[2:]] = p0Up
            report[str(p0Dn.GetName())[2:]] = p0Dn
            effect0  = "1"
            effect12 = "-"
            if mca._projection != None:
                mca._projection.scaleSystTemplate(name,nominal,p0Up)
                mca._projection.scaleSystTemplate(name,nominal,p0Dn)
        elif mode in ["stat_foreach_shape_bins"]:
            nominal = report[p]
            for bin in xrange(1,nominal.GetNbinsX()+1):
                for binmatch in morefields[0]:
                    if re.match(binmatch,'%d'%bin):
                        p0Up = nominal.Clone("%s_%s_%s_bin%dUp"% (nominal.GetName(),name,p,bin))
                        p0Dn = nominal.Clone("%s_%s_%s_bin%dDown"% (nominal.GetName(),name,p,bin))
                        p0Up.SetBinContent(bin,p0Up.GetBinContent(bin)+effect*p0Up.GetBinError(bin))
                        p0Up.SetBinError(bin,p0Up.GetBinError(bin)*(p0Up.GetBinContent(bin)/nominal.GetBinContent(bin) if nominal.GetBinContent(bin)!=0 else 1))
                        p0Dn.SetBinContent(bin,max(1e-5,p0Dn.GetBinContent(bin)-effect*p0Dn.GetBinError(bin)))
                        p0Dn.SetBinError(bin,p0Dn.GetBinError(bin)*(p0Dn.GetBinContent(bin)/nominal.GetBinContent(bin) if nominal.GetBinContent(bin)!=0 else 1))
                        report[str(p0Up.GetName())[2:]] = p0Up
                        report[str(p0Dn.GetName())[2:]] = p0Dn
                        break # otherwise you apply more than once to the same bin if more regexps match
            effect0  = "1"
            effect12 = "-"
            if mca._projection != None:
                raise RuntimeError,'mca._projection.scaleSystTemplate not implemented in the case of stat_foreach_shape_bins'
###                mca._projection.scaleSystTemplate(name,nominal,p0Up) # should be implemented differently
###                mca._projection.scaleSystTemplate(name,nominal,p0Dn) # should be implemented differently
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
    systsEnv[name] = (effmap0,effmap12,mode)


for signal in mca.listSignals():
    myout = outdir
    myout += "%s/" % signal 
    myprocs = ( backgrounds + [ signal ] ) if signal in signals else backgrounds
    if not os.path.exists(myout): os.system("mkdir -p "+myout)
    myyields = dict([(k,v) for (k,v) in allyields.iteritems()]) 
    datacard = open(myout+binname+".card.txt", "w"); 
    datacard.write("## Datacard for cut file %s (signal %s)\n"%(args[1],signal))
    datacard.write("## Event selection: \n")
    for cutline in str(cuts).split("\n"):  datacard.write("##   %s\n" % cutline)
    if signal not in signals: datacard.write("## NOTE: no signal contribution found with this event selection.\n")
    datacard.write("shapes *        * ../common/%s.input.root x_$PROCESS x_$PROCESS_$SYSTEMATIC\n" % binname)
    datacard.write('##----------------------------------\n')
    datacard.write('bin         %s\n' % binname)
    datacard.write('observation %s\n' % myyields['data_obs'])
    datacard.write('##----------------------------------\n')
    klen = max([7, len(binname)]+[len(p) for p in myprocs])
    kpatt = " %%%ds "  % klen
    fpatt = " %%%d.%df " % (klen,3)
    datacard.write('##----------------------------------\n')
    datacard.write('bin             '+(" ".join([kpatt % binname     for p in myprocs]))+"\n")
    datacard.write('process         '+(" ".join([kpatt % p           for p in myprocs]))+"\n")
    datacard.write('process         '+(" ".join([kpatt % iproc[p]    for p in myprocs]))+"\n")
    datacard.write('rate            '+(" ".join([fpatt % myyields[p] for p in myprocs]))+"\n")
    datacard.write('##----------------------------------\n')
    for name,effmap in systs.iteritems():
        datacard.write(('%-12s lnN' % name) + " ".join([kpatt % effmap[p]   for p in myprocs]) +"\n")
    for name,(effmap0,effmap12,mode) in systsEnv.iteritems():
        if mode in ["templates","lnN_in_shape_bins"]:
            datacard.write(('%-10s shape' % name) + " ".join([kpatt % effmap0[p] for p in myprocs]) +"\n")
        if mode == "stat_foreach_shape_bins":
            nbins = None
            for p in myprocs:
                if nbins and nbins!=report[p].GetNbinsX(): raise RuntimeError,'Histos with different number of bins for different processes'
                nbins = report[p].GetNbinsX()
                if effmap0[p] in ['-','0']: continue
                for bin in xrange(1,nbins+1):
                    datacard.write(('%-10s shape' % ("%s_%s_bin%d"%(name,p,bin))) + " ".join([kpatt % effmap0[_p] for _p in myprocs]) +"\n")
        if mode == "envelop":
            datacard.write(('%-10s shape' % (name+"0")) + " ".join([kpatt % effmap0[p]  for p in myprocs]) +"\n")
        if mode in ["envelop", "shapeOnly"]:
            datacard.write(('%-10s shape' % (name+"1")) + " ".join([kpatt % effmap12[p] for p in myprocs]) +"\n")
            datacard.write(('%-10s shape' % (name+"2")) + " ".join([kpatt % effmap12[p] for p in myprocs]) +"\n")
    if options.verbose > -1:
        print "Wrote to ",myout+binname+".card.txt"
    if options.verbose > 0:
        print "="*120
        os.system("cat %s.card.txt" % (myout+binname));
        print "="*120

myout = outdir+"/common/";
if not os.path.exists(myout): os.system("mkdir -p "+myout)
workspace = ROOT.TFile.Open(myout+binname+".input.root", "RECREATE")
for n,h in report.iteritems():
    if options.verbose > 0: print "\t%s (%8.3f events)" % (h.GetName(),h.Integral())
    workspace.WriteTObject(h,h.GetName())
workspace.Close()

if options.verbose > -1:
    print "Wrote to ",myout+binname+".input.root"
