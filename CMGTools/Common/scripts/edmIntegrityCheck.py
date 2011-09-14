import os, sys
import subprocess

import Das

def stageHost():
    """Returns the CASTOR instance to use"""
    return os.environ.get('STAGE_HOST','castorcms')

def listFiles(dir):
    """Recursively list a file or directory on castor"""
    files = subprocess.Popen(['xrd',stageHost(),'dirlistrec', dir], stdout=subprocess.PIPE).communicate()[0]
    result = []
    for f in files.split('\n'):
        s = f.split()
        if s: result.append(tuple(s))
    return result

def listRootFiles(dir):
    """filter out filenames so that they only contain root files"""
    return [f[-1] for f in listFiles(dir) if f[-1].lower().endswith('.root')]

def sortByBaseDir(files):
    """Sort files into directories"""
    result = {}
    for f in files:
        dirname = os.path.dirname(f)
        filename = os.path.basename(f)
        if not result.has_key(dirname): result[dirname] = []
        result[dirname].append(filename)
    return result

def stripFileName(lfn, strip = '/castor/cern.ch/cms'):
    if lfn.startswith(strip):
        lfn = lfn[len(strip):]
    return lfn

def testFileExists(lfn):
    output = subprocess.Popen(['nsls',lfn], stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()[0]
    if "No such file or directory" in output:
        return False
    else:
        return True

def getParseNumberOfEvents(output):
    """Parse the output of edmFileUtil to get the number of events found"""
    tokens = output.split(' ')
    result = -2
    try:
        result = int(tokens[-4])
    except ValueError:
        pass
    return result

def testFile(lfn):
    stdout = subprocess.Popen(['edmFileUtil',lfn], stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()[0]
    for error in ["Fatal Root Error","Could not open file","Not a valid collection"]:
        if error in stdout: return (False,-1)
    return (True,getParseNumberOfEvents(stdout))

if __name__ == '__main__':
    
    from optparse import OptionParser
    
    das = Das.DASOptionParser()
    das.parser.add_option("-d", "--dataset", dest="dataset", default='',help='Test the input reletive to this dataset')
    (opts, files) = das.get_opt()
    
    eventsTotal = -1
    if opts.dataset:
        host    = opts.host
        debug   = opts.verbose
        query   = opts.query
        idx     = opts.idx
        limit   = opts.limit
        
        if not query:
            query = 'dataset=%s' % opts.dataset
        data = eval(Das.get_data(host, query, idx, limit, debug))
        if data['status'] != 'ok':
            raise Exception("Das query failed: Output is '%s'" % data)
        if not data['data']:
            raise Exception("Dataset '%s' not found in DAS: Please check.")
        eventsTotal = 0
        for result in data['data']:
            eventsTotal += result['dataset']['nevents']
        print "Total number of events in dataset is %i" % eventsTotal

    eventsSeen = 0
    for f in files:
        filesToTest = sortByBaseDir(listRootFiles(f))
        for dir, filelist in filesToTest.iteritems():
            filemask = {}
            for ff in filelist:
                #if not 'tree' in ff: continue
                fname = os.path.join(dir,ff)
                test = testFile(stripFileName(fname))
                filemask[ff] = test
                if test[1]>0:
                    eventsSeen += test[1]
            print dir,filemask
    
    print "Total number of events seen is %i" % eventsSeen
    if opts.dataset:
        print "Fraction of full dataset seen: %f" % (eventsSeen/(1.*eventsTotal))
