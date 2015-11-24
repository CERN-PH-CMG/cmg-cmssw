def autoAAA(selectedComponents):
    import re
    from CMGTools.Production import changeComponentAccessMode
    from CMGTools.Production.localityChecker import LocalityChecker
    tier2Checker = LocalityChecker("T2_CH_CERN", datasets="/*/*/MINIAOD*")
    for comp in selectedComponents:
        if not hasattr(comp,'dataset'): continue
        if not re.match("/[^/]+/[^/]+/MINIAOD(SIM)?", comp.dataset): continue
        if "/store/" not in comp.files[0]: continue
        if re.search("/store/(group|user|cmst3)/", comp.files[0]): continue
        if not tier2Checker.available(comp.dataset):
            print "Dataset %s is not available, will use AAA" % comp.dataset
            changeComponentAccessMode.convertComponent(comp, "root://cms-xrd-global.cern.ch/%s")

def redefineRunRange(selectedComponents,run_range):
    from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import kreator
    from math import ceil
    for comp in selectedComponents:
        if comp.isMC or not hasattr(comp, 'dataset') or comp.dataset.count("/") != 3: continue
        pre = len(comp.files)
        comp.files = kreator.getFiles(comp.dataset,"CMS",".*root",run_range=run_range)
        comp.splitFactor = max(1, int(ceil(comp.splitFactor * len(comp.files)/float(pre))) )

def printSummary(selectedComponents):
    print "%-55s | %8s %12s | %7s | %8s %11s | %11s" % ("Component", "N(files)", "N(k ev)", "N(jobs)", "file/job", "k ev/job", "lumi eq [1/fb]")
    print "%-55s | %8s %12s | %7s | %8s %11s | %11s" % (55*"-", 8*"-", 12*"-", 7*"-", 8*"-", 11*"-", 11*"-")
    totj, totf, tote = (0,0,0);
    for comp in sorted(selectedComponents, key = lambda c:c.name):
        njobs = min(comp.splitFactor,len(comp.files)) if getattr(comp,'fineSplitFactor',1) == 1 else comp.fineSplitFactor*len(comp.files) 
        nev   = getattr(comp, 'dataset_entries', 0)
        lumi  = nev/(1.e3 * comp.xSection) if comp.isMC and getattr(comp,'xSection',0) > 0 else 0
        totj += njobs; totf += len(comp.files); tote += nev     
        print "%-55s | %8d %12.3f | %7d | %8.2f %11.3f | %11.3f " % (comp.name, len(comp.files), nev/1000., njobs, len(comp.files)/float(njobs), (nev/njobs if njobs else 0)/1000, lumi)
    print "%-55s | %8s %12s | %7s | %8s %11s | %11s" % (55*"-", 8*"-", 12*"-", 7*"-", 8*"-", 11*"-", 11*"-")
    print "%-55s | %8d %12.3f | %7d | %8.2f %11.3f |" % ("TOTAL", totf, tote/1000., totj, totf/totj, tote/totj/1000.)

def autoConfig(selectedComponents,sequence,services=[],xrd_aggressive=2):
    import PhysicsTools.HeppyCore.framework.config as cfg
    from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
    from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
    from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
    event_class = EOSEventsWithDownload
    EOSEventsWithDownload.aggressive = xrd_aggressive 
    if getHeppyOption("nofetch") or getHeppyOption("isCrab"):
        event_class = Events
    return cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = services,  
                     events_class = event_class)

def insertEventSelector(sequence):
    if not sequence: raise RuntimeError, "to apply an event selection, I need a sequence"
    from CMGTools.HToZZ4L.analyzers.hzz4lCore_modules_cff import eventSelector
    eventSelector.toSelect = [ eval("("+x.replace(":",",")+")") for x in getHeppyOption('events').split(",") ]
    sequence.insert(0, eventSelector)
    print "Will select events ",eventSelector.toSelect

def doTest1(comp, url=None, sequence=None, cache=False):
    from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption
    comp.files = [ url if url else comp.files[0] ]
    if cache:
        import os
        tmpfil = os.path.expandvars("/tmp/$USER/%s" % os.path.basename(comp.files[0]))
        if not os.path.exists(tmpfil):
            os.system("xrdcp %s %s" % (comp.files[0],tmpfil))
        comp.files = [ tmpfil ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1 if getHeppyOption('single') else 5
    if getHeppyOption('events'): insertEventSelector(sequence)
    return [ comp ]

def doTestN(test, selectedComponents):
    if test == '2':
        for comp in selectedComponents:
            comp.files = comp.files[:1]
            comp.splitFactor = 1
            comp.fineSplitFactor = 1
    elif test == '3':
        for comp in selectedComponents:
            comp.files = comp.files[:3]
            comp.splitFactor = 1
            comp.fineSplitFactor = 3
 
