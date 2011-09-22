#!/usr/bin/env python

import copy, datetime, inspect, fnmatch, os, re, subprocess, sys, tempfile, time

import castortools
import Das

class Task(object):
    """Base class for Task API"""
    def __init__(self, name, dataset, user, options, instance = None):
        self.name = name
        self.instance = instance
        self.dataset = dataset
        self.user = user
        self.options = options
    def getname(self):
        """The name of the object, using the instance if needed"""
        if self.instance is not None:
            return '%s_%s' % (self.name,self.instance)
        else:
            return self.name
    def addOption(self, parser):
        """A hook for adding things to the parser"""
        pass
    def run(self, input):
        """Basic API for a task. input and output are dictionaries"""
        return {}
    
class ParseOptions(Task):
    """Option parser"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'ParseOptions', dataset, user, options)
        
        self.das = Das.DASOptionParser()
        self.das.parser.add_option("-u", "--user", dest="user", default=os.getlogin(),help='The username to use when looking at mass storage devices')
        self.das.parser.add_option("-w", "--wildcard", dest="wildcard", default='*.root',help='A UNIX style wildcard to specify which files to check')    
        
    def run(self, input):
        self.options, self.dataset = self.das.get_opt()
        self.user = self.options.user
        if not self.dataset: raise Exception('TaskError: No dataset specified')
        return {'Options':self.options, 'Dataset':self.dataset}    

class CheckDatasetExists(Task):
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckDatasetExists', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-l", "--listLevel", dest="listLevel", help="list level", default=False)        
    def run(self, input):
        #same directory as this file
        scriptsDir = os.path.dirname(inspect.getsourcefile(CheckDatasetExists))
        script = os.path.join(scriptsDir,'listSamples.py')
        
        pattern = fnmatch.translate(self.options.wildcard)
        listLevel = self.options.listLevel
        cmd = ['python',script,self.dataset,'-u',self.user]
        if listLevel:
            cmd.extend(['-l',listLevel])
        stdout = subprocess.Popen(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()[0]
        if not stdout:
            raise Exception("Dataset '%s' not found by listSamples.py. Please check." % self.dataset)
        samples = [s for s in stdout.split('\n') if s]
        if not len(samples) == 1:
            if not self.dataset in samples:
                raise Exception("Dataset not unique according to listSamples.py and no exact match found. Samples found were '%s'" % samples)
        if '//' in samples[0]:
            raise Exception("Too many slashes in sample name '%s'. Please check." % samples[0])
        return {'Dataset':samples[0]}

class BaseDataset(Task):
    """Query DAS to find dataset"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'BaseDataset', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-n", "--name", dest="name", default=None,help='The name of the dataset in DAS. Will be guessed if not specified')
    def query(self, dataset):
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
            tokens = dataset.split(os.sep)
            for i in reversed(xrange(len(tokens))):
                ds = os.sep.join(tokens[1:i])
                if not ds: continue
                ds = '/%s' % ds
                exists, data = check(ds)
                self.options.name = ds
                if exists: break
        else:
            exists, data = check(self.options.name)
            if not exists:
                raise Exception("Dataset '%s' not found in Das. Please check." % self.options.name)
        
        if data is None:
            raise Exception("Dataset '%s' not found in Das. Please check." % self.dataset)
        return data
    
    def run(self, input):
        result = {}
        if (hasattr(self.options,'check') and self.options.check) or not hasattr(self.options,'check'):
            output = self.query(self.dataset)
        return {'Name':self.options.name,'Das':output}

class GZipFiles(Task):
    """GZip a list of files"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'GZipFiles', dataset, user, options)
    def gzip(self, fileName):
        import gzip
        output = '%s.gz' % fileName
        
        f_in = open(fileName, 'rb')
        f_out = gzip.open(output, 'wb')
        f_out.writelines(f_in)
        f_out.close()
        f_in.close()
        
        return output
           
    def run(self, input):
        files = input['FilesToCompress']['Files']
        
        compressed = []
        for f in files:
            if f is None or not f: continue
            if os.path.exists(f): compressed.append(self.gzip(f))
        return {'CompressedFiles':compressed}
    
class CleanFiles(Task):
    """Remove a list of files"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CleanFiles', dataset, user, options)
    def run(self, input):
        files = input['FilesToClean']['Files']
        removed = []
        for f in files:
            if f is None or not f: continue
            if os.path.exists(f): os.remove(f)
            removed.append(f)
        return {'CleanedFiles':removed}

class FindOnCastor(Task):
    """Generate a source CFG"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'FindOnCastor', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-d", "--device", dest="device", default='cmst3',help='The storage device to write to')
        parser.add_option("-g", "--group", dest="group", default='CMG',help='The analysis group')        
    def run(self, input):
        topdir = castortools.lfnToCastor('/store/%s/user/%s/%s' % (self.options.device,self.user,self.options.group))
        directory = '%s/%s' % (topdir,self.dataset)
        directory = directory.replace('//','/')
        if not castortools.fileExists(directory):
            raise Exception("Dataset directory '%s' does not exist" % directory)
        return {'Topdir':topdir,'Directory':directory}  

class CheckForMask(Task):
    """Test if a file mask is present"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckForMask', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-c", "--check", dest="check", default=False, action='store_true',help='Check filemask if available')        
    def run(self, input):
        dir = input['FindOnCastor']['Directory']

        mask = "IntegrityCheck"
        file_mask = []  

        report = None
        if (hasattr(self.options,'check') and self.options.check) or not hasattr(self.options,'check'):
            file_mask = castortools.matchingFiles(dir, '^%s_.*\.txt$' % mask)

            if file_mask:
                from edmIntegrityCheck import PublishToFileSystem
                p = PublishToFileSystem(mask)
                report = p.get(dir)
        return {'MaskPresent':report is not None,'Report':report}
    
class CheckForWrite(Task):
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckForWrite', dataset, user, options)
    def run(self, input):
        """Check that the directory is writable"""
        dir = input['FindOnCastor']['Directory']
        if self.options.check:
            
            _, name = tempfile.mkstemp('.txt',text=True)
            testFile = file(name,'w')
            testFile.write('Test file')
            testFile.close()
            
            #this is bad, but castortools is giving me problems
            if not os.system('rfcp %s %s' % (name,dir)):
                fname = '%s/%s' % (dir,os.path.basename(name))
                write = castortools.fileExists(fname)
                if write:
                    os.system('rfrm %s' % fname)
                else:
                    raise Exception("Failed to write to directory '%s'" % dir)
            os.remove(name)
        return {'Directory':dir,'WriteAccess':True}
    
class GenerateMask(Task):
    """Generates a file mask"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'GenerateMask', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-r", "--recursive", dest="resursive", default=False, action='store_true',help='Walk the mass storage device recursively')
        parser.add_option("-p", "--printout", dest="printout", default=False, action='store_true',help='Print a report to stdout')        
    def run(self, input):
        
        report = None
        if self.options.check and not input['CheckForMask']['MaskPresent']:
            from edmIntegrityCheck import IntegrityCheck, PublishToFileSystem
            
            options = copy.deepcopy(self.options)
            options.user = self.user

            if input.has_key('BaseDataset'):
                options.name = input['BaseDataset']['Name']
            else:
                options.name = None
            
            check = IntegrityCheck(self.dataset,options)
            check.test()
            report = check.structured()
            pub = PublishToFileSystem(check)
            pub.publish(report)
        elif input['CheckForMask']['MaskPresent']:
            report = input['CheckForMask']['Report']
        
        return {'MaskPresent':report is not None,'Report':report}

class CreateJobDirectory(Task):
    """Generate a job directory"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CreateJobDirectory', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-o","--output", dest="output", default=None,help='The directory to use locally for job files')           
    def run(self, input):
        if self.options.output is not None:
            output = self.options.output
        else:
            output = '%s_%s' % (self.dataset.replace('/','.'),datetime.datetime.now().strftime("%s"))
            if output.startswith('.'):
                output = output[1:]
        if not os.path.exists(output):
            os.mkdir(output)
        return {'JobDir':output}

class SourceCFG(Task):
    """Generate a source CFG"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'SourceCFG', dataset, user, options)    
    def run(self, input):
        dir = input['FindOnCastor']['Directory']
        jobdir = input['CreateJobDirectory']['JobDir']
        
        pattern = fnmatch.translate(self.options.wildcard)
        files = castortools.matchingFiles( dir, pattern)
        if not files:
            raise Exception("No files matched pattern '%s' in directory '%s'." % (pattern,dir))
        
        if self.options.check and input['CheckForMask']['MaskPresent']:
            if input['GenerateMask']['Report']['FilesGood'] < 1:
                raise Exception('No good files found to run on')
        
        #same directory as this file
        scriptsDir = os.path.dirname(inspect.getsourcefile(SourceCFG))
        script = os.path.join(scriptsDir,'sourceFileList.py')
        
        cmd = ['python',script,dir,pattern]
        if self.options.check:
            cmd.append('-c')
        stdout = subprocess.Popen(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()[0]
        if not stdout:
            raise Exception("No source cfg generated")

        source = os.path.join(jobdir,'source_cfg.py')
        output = file(source,'w')
        output.write(stdout)
        output.close()
        return {'SourceCFG':source}
    
class FullCFG(Task):
    """Generate the job CFG"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'FullCFG', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("--cfg", dest="cfg", default=None, help='The top level CFG to run')           
    def run(self, input):
        
        jobdir = input['CreateJobDirectory']['JobDir']

        if self.options.cfg is None or not os.path.exists(self.options.cfg):
            raise Exception("The file '%s' does not exist. Please check." % self.options.cfg)
        
        config = file(self.options.cfg).read()
        sourceFile = os.path.basename(input['SourceCFG']['SourceCFG'])
        if sourceFile.lower().endswith('.py'):
            sourceFile = sourceFile[:-3]
        
        source = os.path.join(jobdir,'full_cfg.py')
        output = file(source,'w')
        output.write(config)
        output.write('\nprocess.load("%s")\n' % sourceFile)
        output.write('if hasattr(process,"maxEvents"): process.maxEvents.input = cms.untracked.int32(-1)\n')
        output.write('if hasattr(process,"maxLuminosityBlocks"): process.maxLuminosityBlocks.input = cms.untracked.int32(-1)\n')
        output.close()

        return {'FullCFG':source}

class CheckConfig(Task):
    """Check the basic syntax, python path etc"""    
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckConfig', dataset, user, options)
    def run(self, input):
        
        full = input['FullCFG']['FullCFG']
        
        child = subprocess.Popen(['python',full], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = child.communicate()
        if child.returncode != 0:
            raise Exception("Syntax check of cfg failed. Error was '%s'. (%i)" % (stderr,child.returncode))
        return {'Status':'VALID'}   

class RunTestEvents(Task):
    """Run cmsRun but with a small number of events"""    

    def __init__(self, dataset, user, options):
        Task.__init__(self,'RunTestEvents', dataset, user, options)
    def run(self, input):
        
        full = input['FullCFG']['FullCFG']
        jobdir = input['CreateJobDirectory']['JobDir']
        
        config = file(full).read()
        source = os.path.join(jobdir,'test_cfg.py')
        output = file(source,'w')
        output.write(config)
        output.write('process.out.outputCommands = cms.untracked.vstring("drop *")\n')
        output.write('process.maxEvents.input = cms.untracked.int32(5)\n')
        
        output.close()

        pwd = os.getcwd()
        
        error = None
        try:
            os.chdir(jobdir)
            
            child = subprocess.Popen(['cmsRun',os.path.basename(source)], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            stdout, stderr = child.communicate()
            
            if child.returncode != 0:
                error = "Failed to cmsRun with a few events. Error was '%s' (%i)." % (stderr,child.returncode)
        finally:
            os.chdir(pwd)
            
        if error is not None:
            raise Exception(error)

        return {'Status':'VALID','TestCFG':source}
    
class ExpandConfig(Task):
    """Run edmConfigDump"""    

    def __init__(self, dataset, user, options):
        Task.__init__(self,'ExpandConfig', dataset, user, options)
    def run(self, input):
        
        full = input['FullCFG']['FullCFG']
        jobdir = input['CreateJobDirectory']['JobDir']

        config = file(full).read()
        source = os.path.join(jobdir,'test_cfg.py')
        expanded = 'Expanded%s' % os.path.basename(full)
        output = file(source,'w')
        output.write(config)
        output.write("file('%s','w').write(process.dumpPython())\n" % expanded)
        output.close()

        pwd = os.getcwd()
        
        result = {}
        error = None
        try:
            os.chdir(jobdir)
            
            child = subprocess.Popen(['python',os.path.basename(source)], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            stdout, stderr = child.communicate()
            
            if child.returncode != 0:
                error = "Failed to edmConfigDump. Error was '%s' (%i)." % (stderr,child.returncode)
            result['ExpandedFullCFG'] = os.path.join(jobdir,expanded)
            
        finally:
            os.chdir(pwd)
            
        if error is not None:
            raise Exception(error)

        return result
    
class RunCMSBatch(Task):
    """Run cmsBatch"""    

    def __init__(self, dataset, user, options):
        Task.__init__(self,'RunCMSBatch', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("--batch_user", dest="batch_user", help="The user for LSF", default=os.getlogin())
        parser.add_option("--run_batch", dest="run_batch", default=True, action='store_true',help='Run on the batch system')
        parser.add_option("-N", "--numberOfInputFiles", dest="nInput",help="Number of input files per job",default=5,type=int)
        parser.add_option("-q", "--queue", dest="queue", help="The LSF queue to use", default="1nh")        
        parser.add_option("-t", "--tier", dest="tier",
                          help="Tier: extension you can give to specify you are doing a new production. If you give a Tier, your new files will appear in sampleName/tierName, which will constitute a new dataset.",
                          default="")               
    def run(self, input):
        
        find = FindOnCastor(self.dataset,self.options.batch_user,self.options)
        out = find.run({})
        
        full = input['ExpandConfig']['ExpandedFullCFG']
        jobdir = input['CreateJobDirectory']['JobDir']
        
        sampleDir = os.path.join(out['Directory'],self.options.tier)
        sampleDir = castortools.castorToLFN(sampleDir)
        
        cmd = ['cmsBatch.py',str(self.options.nInput),os.path.basename(full),'-o','%s_Jobs' % self.options.tier,'--force']
        cmd.extend(['-r',sampleDir])
        if self.options.run_batch:
            jname = "%s/%s" % (self.dataset,self.options.tier)
            jname = jname.replace("//","/") 
            cmd.extend(['-b',"'bsub -q %s -J %s < ./batchScript.sh | tee job_id.txt'" % (self.options.queue,jname)])
        print " ".join(cmd)
        
        pwd = os.getcwd()
        
        error = None
        try:
            os.chdir(jobdir)
            returncode = os.system(" ".join(cmd))

            if returncode != 0:
                error = "Running cmsBatch failed. Return code was %i." % returncode
        finally:
            os.chdir(pwd)
            
        if error is not None:
            raise Exception(error)

        return {'SampleDataset':"%s/%s" % (self.dataset,self.options.tier),
                'SampleOutputDir':sampleDir,'LSFJobsTopDir':os.path.join(jobdir,'%s_Jobs' % self.options.tier)}

class MonitorJobs(Task):
    """Monitor LSF jobs created with cmsBatch.py"""    
    def __init__(self, dataset, user, options):
        Task.__init__(self,'MonitorJobs', dataset, user, options)
    
    def getjobid(self, job_dir):
        """Parse the LSF output to find the job id"""
        input = os.path.join(job_dir,'job_id.txt')
        result = None
        if os.path.exists(input):
            contents = file(input).read()
            for c in contents.split('\n'):
                if c and re.match('^Job <\\d*> is submitted to queue <.*>',c) is not None:
                    try:
                        result = c.split('<')[1].split('>')[0]
                    except Exception, e:
                        print >> sys.stderr, 'Job ID parsing error',str(e),c
        return result
    
    def monitor(self, jobs, states = ['RUN','PEND']):
        cmd = ['bjobs']
        cmd.extend(jobs.values())
        child = subprocess.Popen(cmd, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = child.communicate()

        result = {}
        if stdout:
            lines = stdout.split('\n')
            if lines:
                for line in lines[1:]:
                    tokens = line.split(' ')
                    if len(tokens) < 10: continue
                    id = tokens[0]
                    status = tokens[3]
                    if status not in states: continue
                    result[id] = status
        return result
    
    def run(self, input):
        
        jobsdir = input['RunCMSBatch']['LSFJobsTopDir']
        if not os.path.exists(jobsdir):
            raise Exception("LSF jobs dir does not exist: '%s'" % jobsdir)
        
        import glob
        subjobs = [s for s in glob.glob("%s/Job_[0-9]*" % jobsdir) if os.path.isdir(s)]
        jobs = {}
        for s in subjobs:
            jobs[s] = self.getjobid(s)

        def checkStatus(stat):
            result = {}
            for j, id in jobs.iteritems():
                if id is None:
                    result[j] = 'UNKNOWN'
                else:
                    if stat.has_key(id):
                        result[j] = stat[id]
                    else:
                        stdout = os.path.join(j,'LSFJOB_%s' % id,'STDOUT')
                        if os.path.exists(stdout):
                            result[j] = stdout
                        else:
                            result[j] = 'FAILED'
            return result
        
        status = self.monitor(jobs)
        count = 0
        while status:
            job_status = checkStatus(status)
            time.sleep(60)
            status = self.monitor(jobs)
            if not (count % 6):
                print '%s: Monitoring %i jobs (%s)' % (self.name,len(status),self.dataset)
            count += 1
            
        return {'LSFJobStatus':checkStatus(status),'LSFJobIDs':jobs}  
    
class CheckJobStatus(Task):
    """Check the job STDOUT"""    
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckJobStatus', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("--output_wildcard", dest="output_wildcard", help="The wildcard to use when testing the output of this production (defaults to same as -w)", default=None)           
    def run(self, input):
        
        job_status = input['MonitorJobs']['LSFJobStatus']

        result = {}
        for j, status in job_status.iteritems():
            valid = True
            if os.path.exists(status):
                for line in file(status):
                    if 'Exception' in line:
                        result[j] = 'Exception'
                        valid = False
                        break
                    elif 'CPU time limit exceeded' in line:
                        result[j] = 'CPUTimeExceeded'
                        valid = False
                        break
                if valid:
                    result[j] = 'VALID'
            else:
                result[j] = status

        #allows a different wildcard in the final check.
        options = copy.deepcopy(self.options)
        if self.options.output_wildcard is not None:
            options.wildcard = self.options.output_wildcard

        mask = GenerateMask(input['RunCMSBatch']['SampleDataset'],self.options.batch_user,options)
        report = mask.run({'CheckForMask':{'MaskPresent':False}})
        report['LSFJobStatusCheck'] = result
        return report
    
class CleanJobFiles(Task):
    """Removes and compresses auto-generated files"""    
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CleanJobFiles', dataset, user, options)
    def run(self, input):
        
        jobdir = input['CreateJobDirectory']['JobDir']
        jobs = input['MonitorJobs']['LSFJobIDs']
        job_status = input['MonitorJobs']['LSFJobStatus']
        
        actions = {'FilesToCompress':{'Files':[]},'FilesToClean':{'Files':[]}}
        
        actions['FilesToClean']['Files'].append(input['ExpandConfig']['ExpandedFullCFG'])
        actions['FilesToClean']['Files'].append(input['RunTestEvents']['TestCFG'])

        import glob
        for rt in glob.iglob('%s/*.root' % jobdir):
            actions['FilesToClean']['Files'].append(rt)
        for pyc in glob.iglob('%s/*.pyc' % jobdir):
            actions['FilesToClean']['Files'].append(pyc)

        for j in jobs:
            actions['FilesToCompress']['Files'].append(os.path.join(j,'run_cfg.py'))
            
            status = job_status[j]
            if os.path.exists(status):
                actions['FilesToCompress']['Files'].append(status)
                
        compress = GZipFiles(self.dataset,self.user,self.options)
        compressed = compress.run(actions)

        clean = CleanFiles(self.dataset,self.user,self.options)
        removed = clean.run(actions)
        return {'Cleaned':removed,'Compressed':compressed}
    
class WriteSavannah(Task):
    """Call the writeSavannah.py script"""    

    def __init__(self, dataset, user, options):
        Task.__init__(self,'WriteSavannah', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-s", "--savuser",action = "store",dest="savuser",
                          help="If Savannah user is different to current user, enter Savannah username here",
                          default=os.getlogin())        
    def run(self, input):
        #ret = os.system('writeSavannah.py -u %s -s %s -T %s' % (self.user,self.options.savuser,self.dataset))
        return {'SavannahWritten':False}

    
if __name__ == '__main__':
    
    dataset = None
    user = os.getlogin()
    options = {}
    
    op = ParseOptions(dataset,user,options)
    op.das.parser.add_option("--max_threads", dest="max_threads", default=None,help='The maximum number of threads to use')
    
    tasks = [CheckDatasetExists(dataset,user,options),
             FindOnCastor(dataset,user,options),
             CheckForMask(dataset,user,options),
             CheckForWrite(dataset,user,options),
             BaseDataset(dataset,user,options),
             GenerateMask(dataset,user,options),
             CreateJobDirectory(dataset,user,options),             
             SourceCFG(dataset,user,options),
             FullCFG(dataset,user,options),
             CheckConfig(dataset,user,options),
             RunTestEvents(dataset,user,options),
             ExpandConfig(dataset,user,options),
             RunCMSBatch(dataset,user,options),
             MonitorJobs(dataset,user,options),
             CheckJobStatus(dataset,user,options),
             CleanJobFiles(dataset,user,options)
             ]
    #allow the tasks to add extra options
    for t in tasks:
        t.addOption(op.das.parser)
    #sigh! treat write savannah differently
    sav = WriteSavannah(dataset,user,options)
    sav.addOption(op.das.parser)
    #get the options
    op.run({})
    
    #these tasks are quick and are done in the main thread (fail early...)
    simple_tasks = [CheckDatasetExists(dataset,user,options),FindOnCastor(dataset,user,options),sav]
    for d in op.dataset:
        for t in simple_tasks:
            t.dataset = d
            t.options = op.options
            t.user = op.user
            t.run({})
    
    def callback(result):
        print 'Production thread done: ',str(result)
    
    def log(output,s, tostdout = True):
        """Brain-dead utility function"""
        if tostdout:
            print s
        print >> output,s
    
    def work(dataset,op_parse,task_list):
        """Do the work for one dataset"""
        
        logfile = '%s.log' % dataset.replace('/','_')
        output = file(logfile,'w')
        
        previous = {}
        for t in task_list:

            t.dataset = dataset
            t.options = op_parse.options
            t.user = op_parse.user
            
            log(output,'%s: [%s] %s:' % (dataset,time.asctime(),t.getname()))
            if t.__doc__:
                log(output,'%s: %s' % (dataset,t.__doc__) )
            try:
                previous[t.getname()] = t.run(previous)
                log(output,'%s: \t%s' % (dataset,previous[t.getname()]),tostdout=False)
            except Exception, e:
                log(output,'%s: [%s] %s FAILED:' % (dataset,time.asctime(),t.getname()))
                log(output,"%s: Error was '%s'" % (dataset,str(e)))

                #TODO: Perhaps some cleaning?
                break
            
        output.close()
        
        return logfile
    
    #submit the main work in a multi-threaded way
    import multiprocessing
    pool = multiprocessing.Pool(processes=op.options.max_threads)
    print op.dataset
    for d in op.dataset:
        pool.apply_async(work, args=(d,copy.deepcopy(op),copy.deepcopy(tasks)),callback=callback)
    pool.close()
    pool.join()

 
        
    