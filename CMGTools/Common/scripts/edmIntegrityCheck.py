#!/usr/bin/env python

import copy, datetime, fnmatch, json, os, shutil, sys, tempfile
import subprocess

import castortools
import Das

class PublishToFileSystem(object):
    
    def __init__(self, parent):
        if type(parent) == type(""):
            self.parent = parent
        else:
            self.parent = parent.__class__.__name__
    
    def publish(self, report):
        for path in report['PathList']:
            _, name = tempfile.mkstemp('.txt',text=True)
            json.dump(report, file(name,'w'), sort_keys=True, indent=4)
            
            fname = '%s_%s.txt' % (self.parent,report['DateCreated'])
            castor_path = castortools.lfnToCastor(path)
            
            #this is bad, but castortools is giving me problems
            if not os.system('rfcp %s %s' % (name,castor_path)):
                os.system('rfrename %s/%s %s/%s' % (castor_path,os.path.basename(name),castor_path,fname))
                os.system('rfchmod 644 %s/%s' % (castor_path,fname)) #needed so others can read these files - helps the production system
                print "File published: '%s/%s'" % (castor_path,fname)
                os.remove(name)
            else:
                pathhash = path.replace('/','.')
                hashed_name = 'PublishToFileSystem-%s-%s' % (pathhash,fname)
                shutil.move(name, hashed_name)
                print >>sys.stderr, "Cannot write to directory '%s' - written to local file '%s' instead." % (castor_path,hashed_name)
                
    def read(self, lfn, local = False):
        """Reads a report from storage"""
        if local:
            cat = file(lfn).read()
        else:
            cat = castortools.cat(castortools.lfnToCastor(lfn))
        return json.loads(cat)
    
    def get(self,dir):
        """Finds the lastest file and reads it"""
        re = '^%s_.*\.txt$' % self.parent
        files = castortools.matchingFiles(dir, re)
        files = sorted([ (os.path.basename(f),f) for f in files])
        if not files: return None
        return self.read(files[-1][1])
                

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

        from ProductionTasks import BaseDataset
        base = BaseDataset(self.dataset, self.options.user, self.options)

        data = None
        output = base.run({})
        if output.has_key('Das'):
            self.options.name = output['Name']
            data = output['Das']
            
        if data is None:
            raise Exception("Dataset '%s' not found in Das. Please check." % self.dataset)
        self.eventsTotal = 0
        
        #there can be multiple datasets with the same name. If so we take the most recent
        #eliminates double counting of entries in DAS and so we get the fractions right
        datasets = []
        for result in data['data']:
            datasets.append( (result['das']['expire'], result) )
        datasets.sort()
        if datasets:
            self.eventsTotal = datasets[-1][1]['dataset']['nevents']
    
    def stripDuplicates(self):
        
        import re
        
        filemask = {}
        for dirname, files in self.test_result.iteritems():
            for name, status in files.iteritems():
                fname = os.path.join(dirname,name)
                filemask[fname] = status
        
        def isCrabFile(name):
            dir, fname = os.path.split(name)
            base, ext = os.path.splitext(fname)
            return re.match(".*_\d+_\d+_\w+$",base) is not None, base
        def getCrabIndex(base):
            tokens = base.split('_')
            if len(tokens) > 2:
                return (int(tokens[-3]),int(tokens[-2]))
            return None
            
        files = {}
        
        mmin = 1000000000
        mmax = -100000000
        for f in filemask:
            isCrab, base = isCrabFile(f)
            if isCrab:
                index = getCrabIndex(base)
                if index is not None:
                    jobid, retry = index
                    
                    mmin = min(mmin,jobid)
                    mmax = max(mmax,jobid)
                    if files.has_key(jobid) and filemask[f][0]:
                        files[jobid].append((retry, f))
                    elif filemask[f][0]:
                        files[jobid] = [(retry, f)]

        good_duplicates = {}
        bad_jobs = set()
        sum_dup = 0
        for i in xrange(mmin,mmax+1):
            if files.has_key(i):
                duplicates = files[i]
                duplicates.sort()

                fname = duplicates[-1][1]
                if len(duplicates) > 1:
                    for d in duplicates[:-1]:
                        good_duplicates[d[1]] = filemask[d[1]][1]
                        sum_dup += good_duplicates[d[1]]
            else:
                bad_jobs.add(i)
        return good_duplicates, sorted(list(bad_jobs)), sum_dup
    
    def test(self):
        if not castortools.fileExists(self.directory):
            raise Exception("The top level directory '%s' for this dataset does not exist" % self.directory)

        self.query()

        test_results = {}

        filesToTest = self.sortByBaseDir(self.listRootFiles(self.directory))
        for dir, filelist in filesToTest.iteritems():
            filemask = {}
            #apply a UNIX wildcard if specified
            filtered = filelist
            if self.options.wildcard is not None:
                filtered = fnmatch.filter(filelist,self.options.wildcard)
            count = 0
            for ff in filtered:
                fname = os.path.join(dir,ff)
                if self.options.printout:
                    print '[%i/%i]\t Checking %s...' % (count,len(filtered),fname),
                OK, num = self.testFile(castortools.castorToLFN(fname))
                filemask[ff] = (OK,num)
                if self.options.printout:
                    print (OK,num)
                if OK:
                    self.eventsSeen += num
                count += 1
            test_results[castortools.castorToLFN(dir)] = filemask
        self.test_result = test_results

        self.duplicates, self.bad_jobs, sum_dup = self.stripDuplicates()
        #remove duplicate entries from the event count
        self.eventsSeen -= sum_dup
    
    def report(self):
        
        if self.test_result is None:
            self.test()
            
        print 'DBS Dataset name: %s' % self.options.name
        print 'Storage path: %s' % self.topdir
        
        for dirname, files in self.test_result.iteritems():
            print 'Directory: %s' % dirname
            for name, status in files.iteritems():
                fname = os.path.join(dirname,name)
                if not fname in self.duplicates:
                    print '\t\t %s: %s' % (name, str(status))
                else:
                    print '\t\t %s: %s (Valid duplicate)' % (name, str(status))
        print 'Total entries in DBS: %i' % self.eventsTotal
        print 'Total entries in processed files: %i' % self.eventsSeen
        print 'Fraction of dataset processed: %f' % (self.eventsSeen/(1.*self.eventsTotal))
        if self.bad_jobs:
            print "Bad Crab Jobs: '%s'" % ','.join([str(j) for j in self.bad_jobs])
        
    def structured(self):
        
        if self.test_result is None:
            self.test()
        
        totalGood = 0
        totalBad = 0

        report = {'data':{},
                  'ReportVersion':2,
                  'PrimaryDataset':self.options.name,
                  'Name':self.dataset,
                  'PhysicsGroup':'CMG',
                  'Status':'VALID',
                  'TierList':[],
                  'AlgoList':[],
                  'RunList':[],
                  'PathList':[],
                  'Topdir':self.topdir,
                  'StageHost':self.stageHost(),
                  'CreatedBy':self.options.user,
                  'DateCreated':datetime.datetime.now().strftime("%s"),
                  'Files':{}}
        
        for dirname, files in self.test_result.iteritems():
            report['PathList'].append(dirname)
            for name, status in files.iteritems():
                fname = os.path.join(dirname,name)
                report['Files'][fname] = status
                if status[0]:
                    totalGood += 1
                else:
                    totalBad += 1
                
        report['PrimaryDatasetEntries'] = self.eventsTotal
        report['PrimaryDatasetFraction'] = (self.eventsSeen/(1.*self.eventsTotal))
        report['FilesEntries'] = self.eventsSeen

        report['FilesGood'] = totalGood
        report['FilesBad'] = totalBad
        report['FilesCount'] = totalGood + totalBad
        
        report['BadJobs'] = self.bad_jobs
        report['ValidDuplicates'] = self.duplicates

        return report
    
    def stageHost(self):
        """Returns the CASTOR instance to use"""
        return os.environ.get('STAGE_HOST','castorcms')

    def listFiles(self,dir):
        """Recursively list a file or directory on castor"""
        return castortools.listFiles(dir,self.options.resursive)

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
    das.parser.add_option("-p", "--printout", dest="printout", default=False, action='store_true',help='Print a report to stdout')    
    das.parser.add_option("-r", "--recursive", dest="resursive", default=False, action='store_true',help='Walk the mass storage device recursively')
    das.parser.add_option("-u", "--user", dest="user", default=os.getlogin(),help='The username to use when looking at mass storage devices')
    das.parser.add_option("-w", "--wildcard", dest="wildcard", default=None,help='A UNIX style wildcard to specify which files to check')    
    (opts, datasets) = das.get_opt()
    
    for d in datasets:
        
        #allows us to specify the user in the dataset string
        op = copy.deepcopy(opts)
        tokens = d.split('%')
        if len(tokens) == 2:
            op.user = tokens[0]
            d = tokens[1]
        
        check = IntegrityCheck(d,op)
        check.test()
        if op.printout:
            check.report()
        report = check.structured()
        pub = PublishToFileSystem(check)
        pub.publish(report)

