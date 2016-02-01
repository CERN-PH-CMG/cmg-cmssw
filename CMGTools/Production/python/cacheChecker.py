from CMGTools.Production.dataset import createDataset

class CacheChecker:
    def __init__(self):
        pass
    def checkComp(self,comp,verbose=False):
        dataset = comp.dataset
        run_range = comp.run_range
        return self.check(dataset,run_range,verbose=verbose)
    def check(self,dataset,run_range,verbose=False):
        fromCache = createDataset("CMS", dataset, ".*root", readcache=True,  run_range=run_range)
        fromDAS   = createDataset("CMS", dataset, ".*root", readcache=False, run_range=run_range)
        filesCache = sorted(list(set(fromCache.listOfGoodFiles())))
        filesDAS   = sorted(list(set(fromDAS.listOfGoodFiles())))
        if verbose: print "%s %r: files cache %d, dbs %d, match %s" % (
            dataset, run_range, len(filesCache), len(filesDAS), 
            "OK" if ( filesCache == filesDAS ) else "FAIL") 
        return filesCache == filesDAS

if __name__ == "__main__":
    checker = CacheChecker()
    #from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import DoubleMuon_Run2015C_Prompt_50ns
    #checker.checkComp(DoubleMuon_Run2015C_Prompt_50ns, verbose=True)

        
