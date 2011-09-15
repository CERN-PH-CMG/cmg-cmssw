import os, sys
import subprocess

import castortools
import Das

class IntegrityCheck(object):
    
    def __init__(self, dataset, options):
        if not dataset.startswith(os.sep):
            dataset = os.sep + dataset

        self.dataset = dataset
        self.options = options
        self.topdir = castortools.lfnToCastor('/store/%s/user/%s/CMG' % (self.options.device,self.options.user))
        self.directory = os.path.join(self.topdir,*self.dataset.split(os.sep))
        
        #event counters
        self.eventsTotal = -1
        self.eventsSeen = 0
        
        self.test_result = None
    
    def query(self):
        """Query DAS to find out how many events are in the dataset"""

        host    = self.options.host
        debug   = self.options.verbose
        idx     = self.options.idx
        limit   = self.options.limit
        
        def check(ds):
            query = 'dataset=%s' % ds
            data = eval(Das.get_data(host, query, idx, limit, debug))
            if data['status'] != 'ok':
                raise Exception("Das query failed: Output is '%s'" % data)
            return (data['data'],data)

        data = None
        if self.options.name is None:
            #guess the dataset name in DBS
            tokens = self.dataset.split(os.sep)
            for i in reversed(xrange(len(tokens))):
                ds = os.sep.join(tokens[1:i])
                if not ds: continue
                ds = '/%s' % ds
                exists, data = check(ds)
                if exists: break
        else:
            exists, data = check(self.options.name)
            if not exists:
                raise Exception("Dataset '%s' not found in Das. Please check." % self.options.name)
        
        if data is None:
            raise Exception("Dataset '%s' not found in Das. Please check." % self.dataset)
        self.eventsTotal = 0
        for result in data['data']:
            self.eventsTotal += result['dataset']['nevents']
        print "Total number of events in dataset is %i" % self.eventsTotal
    
    def test(self):
        if not castortools.fileExists(self.directory):
            raise Exception("The top level directory '%s' for this dataset does not exist" % self.directory)

        self.query()

        test_results = {}

        filesToTest = self.sortByBaseDir(self.listRootFiles(self.directory))
        for dir, filelist in filesToTest.iteritems():
            filemask = {}
            for ff in filelist:
                #if not 'tree' in ff: continue
                fname = os.path.join(dir,ff)
                OK, num = self.testFile(castortools.castorToLFN(fname))
                filemask[ff] = (OK,num)
                if OK:
                    self.eventsSeen += num
            test_results[castortools.castorToLFN(dir)] = filemask
        self.test_result = test_results
    
    def report(self):
        
        if self.test_result is None:
            self.test()
            
        print 'DBS Dataset name: %s' % self.options.name
        print 'Storage path: %s' % self.topdir
        
        for dirname, files in self.test_result.iteritems():
            print 'Directory: %s' % dirname
            for name, status in files.iteritems():
                print '\t\t %s: %s' % (name, str(status))
        print 'Total entries in DBS: %i' % self.eventsTotal
        print 'Total entries in processed files: %i' % self.eventsSeen
        print 'Fraction of dataset processed: %f' % (self.eventsSeen/(1.*self.eventsTotal))
    
    def stageHost(self):
        """Returns the CASTOR instance to use"""
        return os.environ.get('STAGE_HOST','castorcms')

    def listFiles(self,dir):
        """Recursively list a file or directory on castor"""
        cmd = 'dirlist'
        if self.options.resursive:
            cmd = 'dirlistrec'
        files = subprocess.Popen(['xrd',self.stageHost(),cmd, dir], stdout=subprocess.PIPE).communicate()[0]
        result = []
        for f in files.split('\n'):
            s = f.split()
            if s: result.append(tuple(s))
        return result

    def listRootFiles(self,dir):
        """filter out filenames so that they only contain root files"""
        return [f[-1] for f in self.listFiles(dir) if f[-1].lower().endswith('.root')]

    def sortByBaseDir(self,files):
        """Sort files into directories"""
        result = {}
        for f in files:
            dirname = os.path.dirname(f)
            filename = os.path.basename(f)
            if not result.has_key(dirname): result[dirname] = []
            result[dirname].append(filename)
        return result


    def getParseNumberOfEvents(self,output):
        """Parse the output of edmFileUtil to get the number of events found"""
        tokens = output.split(' ')
        result = -2
        try:
            result = int(tokens[-4])
        except ValueError:
            pass
        return result

    def testFile(self,lfn):
        stdout = subprocess.Popen(['edmFileUtil',lfn], stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()[0]
        for error in ["Fatal Root Error","Could not open file","Not a valid collection"]:
            if error in stdout: return (False,-1)
        return (True,self.getParseNumberOfEvents(stdout))

if __name__ == '__main__':
    
    from optparse import OptionParser
    
    das = Das.DASOptionParser()
    das.parser.add_option("-d", "--device", dest="device", default='cmst3',help='The storage device to write to')
    das.parser.add_option("-n", "--name", dest="name", default=None,help='The name of the dataset in DAS. Will be guessed if not specified')
    das.parser.add_option("-r", "--recursive", dest="resursive", default=False, action='store_true',help='Walk the mass storage device recursively')
    das.parser.add_option("-u", "--user", dest="user", default=os.getlogin(),help='The username to use when looking at mass storage devices')
    (opts, datasets) = das.get_opt()
    
    for d in datasets:
        
        check = IntegrityCheck(d,opts)
        check.test()
        check.report()
    
