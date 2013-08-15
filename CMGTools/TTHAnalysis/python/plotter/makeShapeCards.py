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
(options, args) = parser.parse_args()
options.weight = True
options.final  = True

mca  = MCAnalysis(args[0],options)
cuts = CutsFile(args[1],options)

binname = os.path.basename(args[1]).replace(".txt","") if options.outname == None else options.outname
outdir  = options.outdir+"/" if options.outdir else ""

report = mca.getPlotsRaw("x", args[2], args[3], cuts.allCuts(), nodata=True)
report['data_obs'] = mergePlots("x_data_obs", [p for n,p in report.iteritems()])

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
        line = re.sub("#.*","",line)
        field = [f.strip() for f in line.split(':')]
        if len(field) < 4:
            raise RuntimeError, "Malformed line %s in file %s"%(line.strip(),sysfile)
        elif len(field) == 4 or field[4] == "lnN":
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap,binname) == None: continue
            if name not in systs: systs[name] = []
            systs[name].append((re.compile(procmap),amount))
        elif field[4] in ["envelop","shapeOnly"]:
            (name, procmap, binmap, amount) = field[:4]
            if re.match(binmap,binname) == None: continue
            if name not in systs: systsEnv[name] = []
            systsEnv[name].append((re.compile(procmap),amount,field[4]))
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

for name in systsEnv.keys():
    effmap0  = {}
    effmap12 = {}
    for p in procs:
        effect = "-"
        effect0  = "-"
        effect12 = "-"
        for (procmap,amount,mode) in systsEnv[name]:
            if re.match(procmap, p): effect = float(amount)
        if effect != "-":
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
        effmap0[p]  = effect0 
        effmap12[p] = effect12 
    systsEnv[name] = (effmap0,effmap12)


datacard = open(outdir+binname+".card.txt", "w"); 
datacard.write("## Datacard for cut file %s\n"%args[1])
datacard.write("shapes *        * %s.input.root x_$PROCESS x_$PROCESS_$SYSTEMATIC\n" % binname)
#datacard.write("shapes data_obs * %s.input.root x_data    \n" % binname)
datacard.write('##----------------------------------\n')
datacard.write('bin         %s\n' % binname)
datacard.write('observation %s\n' % allyields['data_obs'])
datacard.write('##----------------------------------\n')
klen = max([7, len(binname)]+[len(p) for p in procs])
kpatt = " %%%ds "  % klen
fpatt = " %%%d.%df " % (klen,3)
datacard.write('##----------------------------------\n')
datacard.write('bin             '+(" ".join([kpatt % binname  for p in procs]))+"\n")
datacard.write('process         '+(" ".join([kpatt % p        for p in procs]))+"\n")
datacard.write('process         '+(" ".join([kpatt % iproc[p] for p in procs]))+"\n")
datacard.write('rate            '+(" ".join([fpatt % allyields[p] for p in procs]))+"\n")
datacard.write('##----------------------------------\n')
for name,effmap in systs.iteritems():
    datacard.write(('%-12s lnN' % name) + " ".join([kpatt % effmap[p]   for p in procs]) +"\n")
for name,(effmap0,effmap12) in systsEnv.iteritems():
    datacard.write(('%-10s shape' % (name+"0")) + " ".join([kpatt % effmap0[p]  for p in procs]) +"\n")
    datacard.write(('%-10s shape' % (name+"1")) + " ".join([kpatt % effmap12[p] for p in procs]) +"\n")
    datacard.write(('%-10s shape' % (name+"2")) + " ".join([kpatt % effmap12[p] for p in procs]) +"\n")


datacard.close()
print "Wrote to ",outdir+binname+".card.txt"
if options.verbose:
    print "="*120
    os.system("cat %s.card.txt" % (outdir+binname));
    print "="*120

workspace = ROOT.TFile.Open(outdir+binname+".input.root", "RECREATE")
for n,h in report.iteritems():
    if options.verbose: print "\t%s (%8.3f events)" % (h.GetName(),h.Integral())
    workspace.WriteTObject(h,h.GetName())
workspace.Close()

print "Wrote to ",outdir+binname+".input.root"

