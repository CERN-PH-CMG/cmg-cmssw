import os, fnmatch
import CMGTools.Production.eostools  as eostools 

## ------------------------------------------------------------------------------------------------------------------------------------------------------
def mkReader(chain,redo=False,name="ntuple_reader"):
    from ROOT import gROOT
    if not os.path.isfile("%s.C" % name) or redo:
        chain.MakeClass(name)
    gROOT.LoadMacro("%s.C+" % name)
    
    import ROOT
    return getattr(ROOT,name)(chain)

## ------------------------------------------------------------------------------------------------------------------------------------------------------
def mkChain(files, name="jetanalyzer/tree"):
    from ROOT import TChain
    chain = TChain(name)
    for f in files:
        print "Adding file %s" % f
        chain.AddFile(f)
    return chain

## ------------------------------------------------------------------------------------------------------------------------------------------------------
def getListOfFiles( baseDir, filePattern, expr="{baseDir}/{filePattern}"):
    if baseDir.startswith("/store"):
        return [ "root://eoscms/%s" % f for f in eostools.listFiles( expr.format( baseDir=baseDir, filePattern="" ) )
                 if os.path.basename(f) == filePattern or fnmatch.fnmatch(f,filePattern) ]
    else:
        return glob.glob(expr.format( baseDir=baseDir, filePattern=filePattern))
    
