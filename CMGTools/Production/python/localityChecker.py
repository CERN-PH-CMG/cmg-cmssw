import os, os.path, json, sys, urllib2, time

class LocalityChecker:
    def __init__(self, node="T2_CH_CERN", datasets=None, cacheTime=4*3600, cacheDir="~/.cmgdataset"):#, datasets="/*/*/*AOD*"):
        self.data = None
        self.node = node
        self.datasets = datasets
        if cacheDir:
            cacheDir = os.path.expandvars(os.path.expanduser(cacheDir))
            if datasets:
                self.cacheFile = "%s/phedex_%s-%s.json" % (cacheDir, node, datasets.replace("/","_").replace("*","+"));
            else:
                self.cacheFile = "%s/phedex_%s.json" % (cacheDir, node);
        else:
            self.cacheFile = None
        self.cacheTime = cacheTime 
    def available(self,dataset):
        if self.data == None: self.data = self.initData()
        return dataset in self.data
    def initData(self):
        isBatch = ('LSB_JOBID' in os.environ)
        if self.cacheTime > 0 and self.cacheFile:
            if os.path.exists(self.cacheFile):
                age = time.time() - os.path.getmtime(self.cacheFile)
                if isBatch and age >= self.cacheTime:
                    sys.stderr.write("WARNING: Phedex cache validity expired, but will ignore that since I'm runing in batch\n")
                if age < self.cacheTime or isBatch:
                    try:
                        data = json.load(open(self.cacheFile))
                        sys.stderr.write("Reading phedex query from cache: %d samples, %.1f hours old\n" % (len(data), age/3600))
                        return data
                    except:
                        pass
                else:
                    sys.stderr.write("Phedex query cache is %.1f hours old, will re-make the query\n" % (age/3600))
        if isBatch:
            raise RuntimeError, "Trying to do a DAS query while in a LXBatch job (env variable LSB_JOBID defined)"
        sys.stderr.write("Querying Phedex for node %s, datasets %s; this may take some time...\n" % (self.node, self.datasets))
        start = time.time()
        urlquery = 'https://cmsweb.cern.ch/phedex/datasvc/json/prod/subscriptions?node='+self.node+'&create_since=0'
        if self.datasets: urlquery += "&dataset=%s" % self.datasets.replace("/","%2F")
        text = urllib2.urlopen(urlquery).read()
        if not text: 
            raise RuntimeError, "Error executing phedex query: "+urlquery
        sys.stderr.write(" ... query took %.1f seconds\n" % (time.time()-start))
        jsondata = json.loads(text)
        datasets = jsondata['phedex']['dataset']
        data = {}; errors = 0
        for d in datasets:
            if 'subscription' not in d: 
                if 'block' in d:
                    for b in d['block']:
                        if 'subscription' not in b:
                            errors += 1
                            if errors < 10:
                                sys.stderr.write("Bad block %s in %s\n" % (b,d))
                        for s in b['subscription']:
                            if s['percent_files'] != 100 or s['percent_bytes'] != 100: continue 
                            data[b['name']] = True 
                else:
                    errors += 1
                    if errors < 10:
                        sys.stderr.write("Bad item %s\n" % (d))
                continue # skip blocks
            for s in d['subscription']:
                if s['percent_files'] != 100 or s['percent_bytes'] != 100: continue 
                data[d['name']] = True 
        if errors: 
            sys.stderr.write("%d errors found. Raw phedex query output saved in phedex_error_dump.json\n");
            dump = open("phedex_error_dump.json","w"); dump.write(text); dump.close()
        if self.cacheFile:
            out = open(self.cacheFile, "w")
            out.write(json.dumps(data))
            out.close()
        return data

if __name__ == "__main__":
    checker = LocalityChecker()#datasets="/DYJetsToLL*/RunIISpring15DR74*/MINIAODSIM")
    for S in [
        '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM',
        '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM', ]:
        print S, checker.available(S)
        
