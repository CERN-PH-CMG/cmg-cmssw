import cPickle, math, optparse, os, os.path, shutil, copy
from PhysicsTools.HeppyCore.framework.heppy_loop import split

def reSplitChunk(compname,splitFactor,options):
    try:
        comp = cPickle.load(open("%s/config.pck" % compname))
    except:
        raise RuntimeError, "Pickle file %s/config.pck does not exist or can't be read" % compname
    workDir = compname+".dir"
    if os.path.exists(workDir):
        raise RuntimeError, "directory %s exists" % workDir
    os.mkdir(workDir)
    shutil.move(compname, "%s/source" % workDir)
    if splitFactor == -1:
        comp.splitFactor = len(comp.files)
        comp.fineSplitFactor = 1 
    elif splitFactor < -1:
        comp.splitFactor = 1
        comp.fineSplitFactor = -splitFactor
    else:
        comp.splitFactor = splitFactor
        comp.fineSplitFactor = 1
    comps = split([comp])
    for i, ci in enumerate(comps):
        print "Comp %s: file %s, fineSplit %s" % (ci.name, ci.files, getattr(ci, 'fineSplit', None))
        os.mkdir("%s/%s" % (workDir,ci.name))
        for f in [ "options.json", "batchScript.sh", "pycfg.py" ]:
            if os.path.exists("%s/source/%s" % (workDir, f)):
                shutil.copy("%s/source/%s" % (workDir, f), "%s/%s/%s" % (workDir, ci.name, f))
            fout = open("%s/%s/config.pck" % (workDir, ci.name), 'w')
            cPickle.dump(ci,fout)
            fout.close()
                
 
if __name__ == '__main__':
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-v", dest="verbose", action="store_const", const = 1, default=0, help="be verbose")
    parser.add_option("-n", "--splitFactor", dest="splitFactor", type="int", default=-1, help="New split factor. -1 for one file per job, -N for fineSplit N")
    (options, args) = parser.parse_args()
    for a in args:
        reSplitChunk(a,options.splitFactor,options)
