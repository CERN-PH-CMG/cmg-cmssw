#!/usr/bin/env python
from CMGTools.TTHAnalysis.treeReAnalyzer import *
import re,os

class BTag_SFb:
    def __init__(self,filename):
        self._SFb = {}
        file = open(filename, "r")
        ptminLine = file.readline()
        ptmaxLine = file.readline()
        if not re.match(r"float\s*ptmin\[\]\s*=\s*\{\s*(\d+\,\s*)+\d+\s*\}\s*;\s*\n", ptminLine):
            raise RuntimeError, "malformed ptmin line %s" % ptminLine;
        if not re.match(r"float\s*ptmax\[\]\s*=\s*\{\s*(\d+\,\s*)+\d+\s*\}\s*;\s*\n", ptmaxLine):
            raise RuntimeError, "malformed ptmax line %s" % ptmaxLine;
        for line in file:
            m = re.match(r"\s*Tagger: (\S+[LMT]) within.*", line)
            if m:
                tagger = m.group(1)
                sfbline = file.next()
                m = re.match(r"\s*SFb = (.*);", sfbline)
                self._SFb[tagger] = eval("lambda x : "+m.group(1))
                #print "Found tagger",tagger,": SFb = ",m.group(1)
    def __call__(self,tagger,jet):
        return self._SFb[tagger](jet.pt)
    
class BTag_SFLight:
    def __init__(self,filename):
        self._SFLight = {}
        file = open(filename, "r")
        tagline = re.compile(r'if\s*\(\s*Atagger\s*==\s*"(\S+)"\s*&&\s*sEtamin\s*==\s*"(\S+)"\s*&&\s*sEtamax\s*==\s*"(\S+)"\s*\)');
        sflline = re.compile(r'new\s+TF1\(\s*"(\S+)"\s*,\s*"(.*)"\s*,\s*(\d+\.?\d*)\s*,\s*ptmax\s*\)');
        tagger = None
        for line in file:
            tagm = re.search(tagline, line)
            if tagm:
                (tagger,etamin,etamax) = (tagm.group(1),float(tagm.group(2)),float(tagm.group(3)))
                if tagger not in self._SFLight: 
                    self._SFLight[tagger] = []
                self._SFLight[tagger].append([etamin,etamax,{}])
                continue
            sfm = re.search(sflline, line)
            if sfm:
                self._SFLight[tagger][-1][2][sfm.group(1)] = eval("lambda x: "+sfm.group(2));
    def __call__(self,tagger,jet):
        ae = abs(jet.eta)
        for etamin, etamax, sflights in self._SFLight[tagger]:
            if etamin <= ae and ae <= etamax: 
                return sflights['SFlight'](jet.pt)
        return 1.0 

class BTagSFEvent:
    def __init__(self,sfb,sflight,effhist,WPs=[('CSVM',0.679), ('CSVL',0.244)]):
        self._sfb = sfb
        self._sflight = sflight
        self._efffile = ROOT.TFile.Open(effhist)
        self._effhists = {}
        self._WPs = WPs
        for T,C in self._WPs:
            for (l,f) in (('b',5), ('c',4), ('l',0)):
                self._effhists[(T,f)] = self._efffile.Get("%s_eff_%s" % (l,T))
    def _mceff(self,tagger,j):
        hist = self._effhists[(tagger,j.mcFlavour)]
        xbin = hist.GetXaxis().FindBin(max(min(149.9,j.pt),25.1))
        ybin = hist.GetYaxis().FindBin(max(min(4.9,abs(j.eta)),0.1))
        #print "eff %s for pt %.1f, eta %.1f --> %.3f" % (j.pt,
        return hist.GetBinContent(xbin,ybin)
    def __call__(self,event,debug=False):
        jets = Collection(event,"Jet","nJet25",8)
        num, den = 1.0, 1.0
        for j in jets:
            tagged = False
            for iw,(T,C) in enumerate(self._WPs):
                if j.btagCSV > C: 
                    tagged = True
                    eps = self._mceff(T,j)
                    sf  = self._sfb(T,j) if j.mcFlavour > 0 else self._sflight(T,j)
                    if iw > 0:
                        TT,TC = self._WPs[iw-1]
                        epsT = self._mceff(TT,j)
                        sfT  = self._sfb(TT,j) if j.mcFlavour > 0 else self._sflight(TT,j)
                        num *= max(0, sf*eps - sfT*epsT)
                        den *= max(0, eps - epsT)
                        if debug:
                            print "    jet pt %5.1f eta %+4.2f btag %4.3f mcFlavour %d --> pass %s (eff %.3f, sf %.3f) but fail %s (eff %.3f, sf %.3f) --> event contrib: %.4f" % (j.pt, j.eta, min(1.,max(0.,j.btagCSV)), j.mcFlavour, T, eps, sf, TT, epsT, sfT, max(0, sf*eps - sfT*epsT)/(eps - epsT) if eps-epsT > 0 else 1) 
                    else:
                        if debug:
                            print "    jet pt %5.1f eta %+4.2f btag %4.3f mcFlavour %d --> pass %s (eff %.3f, sf %.3f) --> event contrib: %.4f" % (j.pt, j.eta, min(1.,max(0.,j.btagCSV)), j.mcFlavour, T, eps, sf, sf) 
                        num *= sf*eps
                        den *= eps
                    break
            if not tagged:
                T,C = self._WPs[-1]
                eps = self._mceff(T,j)
                sf  = self._sfb(T,j) if j.mcFlavour > 0 else self._sflight(T,j)
                if debug:
                    print "    jet pt %5.1f eta %+4.2f btag %4.3f mcFlavour %d --> fail %s (eff %.3f, sf %.3f) --> event contrib: %.4f" % (j.pt, j.eta, min(1.,max(0.,j.btagCSV)), j.mcFlavour, T, eps, sf, max(0, 1-sf*eps)/max(0, 1-eps) if eps < 1 else 1)
                num *= max(0, 1-sf*eps)
                den *= max(0, 1-eps)
        return num/den if den != 0 else 1;

bTagSFEvent3WP = BTagSFEvent(
    BTag_SFb("%s/src/CMGTools/TTHAnalysis/data/btag/SFb-pt_payload_Moriond13.txt" % os.environ['CMSSW_BASE']),
    BTag_SFLight("%s/src/CMGTools/TTHAnalysis/data/btag/SFLightFuncs_Moriond2013_ABCD.txt" % os.environ['CMSSW_BASE']),
    "%s/src/CMGTools/TTHAnalysis/data/btag/bTagEff_TTMC.root" % os.environ['CMSSW_BASE'],
    WPs=[('CSVT',0.898), ('CSVM',0.679), ('CSVL',0.244) ]
)


if __name__ == '__main__':
    from sys import argv
    file = ROOT.TFile(argv[1])
    tree = file.Get("ttHLepTreeProducerBase")
    class Tester(Module):
        def __init__(self, name, sfe1,sfe2,sfe3):
            Module.__init__(self,name,None)
            self.sfe1 = sfe1
            self.sfe2 = sfe2
            self.sfe3 = sfe3
        def analyze(self,ev):
            print "\nrun %6d lumi %4d event %d: jets %d" % (ev.run, ev.lumi, ev.evt, ev.nJet25)
            jets = Collection(ev,"Jet","nJet25",8)
            overall1 = self.sfe1(ev)
            overall2 = self.sfe2(ev)
            overall3 = self.sfe3(ev)
            print "overall %d %d %.4f %.4f %.4f" % (ev.nBJetLoose25,ev.nBJetMedium25,overall1,overall2,overall3)
    bTagSFEvent1WP = BTagSFEvent(
        BTag_SFb("%s/src/CMGTools/TTHAnalysis/data/btag/SFb-pt_payload_Moriond13.txt" % os.environ['CMSSW_BASE']),
        BTag_SFLight("%s/src/CMGTools/TTHAnalysis/data/btag/SFLightFuncs_Moriond2013_ABCD.txt" % os.environ['CMSSW_BASE']),
        "%s/src/CMGTools/TTHAnalysis/data/btag/bTagEff_TTMC.root" % os.environ['CMSSW_BASE'],
        WPs=[('CSVL',0.244)]
    )
    bTagSFEvent2WP = BTagSFEvent(
        BTag_SFb("%s/src/CMGTools/TTHAnalysis/data/btag/SFb-pt_payload_Moriond13.txt" % os.environ['CMSSW_BASE']),
        BTag_SFLight("%s/src/CMGTools/TTHAnalysis/data/btag/SFLightFuncs_Moriond2013_ABCD.txt" % os.environ['CMSSW_BASE']),
        "%s/src/CMGTools/TTHAnalysis/data/btag/bTagEff_TTMC.root" % os.environ['CMSSW_BASE'],
        WPs=[('CSVM',0.679), ('CSVL',0.244)]
    )
    el = EventLoop([ Tester("tester", bTagSFEvent1WP, bTagSFEvent2WP, bTagSFEvent3WP) ])
    el.loop([tree], maxEvents = 1000)
