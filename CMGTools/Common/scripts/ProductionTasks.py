import datetime, inspect, fnmatch, os, subprocess, tempfile

import castortools
import Das

class Task(object):
    """Base class for Task API"""
    def __init__(self, name, dataset, user, options):
        self.name = name
        self.dataset = dataset
        self.user = user
        self.options = options
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
        return {'options':self.options, 'dataset':self.dataset}    

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
            raise Exception("Dataset not unique according to listSamples.py. Samples found were '%s'" % samples)
        if '//' in samples[0]:
            raise Exception("Too many slashes in sample name '%s'. Please check." % samples[0])
        return {'dataset':samples[0]}

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
        if self.options.check:
            output = self.query(self.dataset)
            input['name'] = self.options.name
            input['das'] = output
        return input

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
        return {'topdir':topdir,'directory':directory}  

class CheckForMask(Task):
    """Test if a file mask is present"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckForMask', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("-c", "--check", dest="check", default=False, action='store_true',help='Check filemask if available')        
    def run(self, input):
        dir = input['directory']

        mask = "IntegrityCheck"
        file_mask = []  

        report = None
        if self.options.check:
            file_mask = castortools.matchingFiles(dir, '^%s_.*\.txt$' % mask)

            if file_mask:
                from edmIntegrityCheck import PublishToFileSystem
                p = PublishToFileSystem(mask)
                report = p.get(dir)
        
        input['MaskPresent'] = report is not None
        input['Report'] = report
        return input
    
class CheckForWrite(Task):
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckForWrite', dataset, user, options)
    def run(self, input):
        """Check that the directory is writable"""
        dir = input['directory']
        if self.options.check and not input['MaskPresent']:
            
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
                    print 'can write to',dir
                else:
                    raise Exception("Failed to write to directory '%s'" % dir)
            os.remove(name)
        return input  
    
class GenerateMask(Task):
    """Generates a file mask"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'GenerateMask', dataset, user, options)
    def run(self, input):
        
        report = None
        if self.options.check and not input['MaskPresent']:
            from edmIntegrityCheck import IntegrityCheck, PublishToFileSystem
            
            check = IntegrityCheck(self.dataset,self.options)
            check.test()
            report = check.structured()
            pub = PublishToFileSystem(check)
            pub.publish(report)
        
        input['MaskPresent'] = report is not None
        input['Report'] = report
        return input

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
        input['JobDir'] = output
        return input

class SourceCFG(Task):
    """Generate a source CFG"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'SourceCFG', dataset, user, options)    
    def run(self, input):
        dir = input['directory']
        jobdir = input['JobDir']
        
        pattern = fnmatch.translate(self.options.wildcard)
        files = castortools.matchingFiles( dir, pattern)
        if not files:
            raise Exception("No files matched pattern '%s' in directory '%s'." % (pattern,dir))
        
        if self.options.check and input['MaskPresent']:
            if input['Report']['FilesGood'] < 1:
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
        input['SourceCFG'] = source
        return input
    
class FullCFG(Task):
    """Generate the job CFG"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'FullCFG', dataset, user, options)
    def addOption(self, parser):
        parser.add_option("--cfg", dest="cfg", default=None, help='The top level CFG to run')           
    def run(self, input):
        
        jobdir = input['JobDir']

        if self.options.cfg is None or not os.path.exists(self.options.cfg):
            raise Exception("The file '%s' does not exist. Please check." % self.options.cfg)
        
        config = file(self.options.cfg).read()
        sourceFile = os.path.basename(input['SourceCFG'])
        if sourceFile.lower().endswith('.py'):
            sourceFile = sourceFile[:-3]
        
        source = os.path.join(jobdir,'full_cfg.py')
        output = file(source,'w')
        output.write(config)
        output.write('\nprocess.load("%s")\n' % sourceFile)
        output.write('process.maxEvents.input = cms.untracked.int32(-1)\n')
        output.write('process.maxLuminosityBlocks.input = cms.untracked.int32(-1)\n')
        output.close()

        input['FullCFG'] = source
        return input

class CheckConfig(Task):
    """Check the basic syntax, python path etc"""    
    def __init__(self, dataset, user, options):
        Task.__init__(self,'CheckConfig', dataset, user, options)
    def run(self, input):
        
        full = input['FullCFG']
        
        child = subprocess.Popen(['python',full], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        stdout, stderr = child.communicate()
        if child.returncode != 0:
            raise Exception("Syntax check of cfg failed. Error was '%s'. (%i)" % (stderr,child.returncode))
        return input   

class RunTestEvents(Task):
    """Run cmsRun but with a small number of events"""    

    def __init__(self, dataset, user, options):
        Task.__init__(self,'RunTestEvents', dataset, user, options)
    def run(self, input):
        
        full = input['FullCFG']
        jobdir = input['JobDir']
        
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

        return input
    
 
    
if __name__ == '__main__':
    
    dataset = None
    user = os.getlogin()
    options = {}
    
    op = ParseOptions(dataset,user,options)
    
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
             RunTestEvents(dataset,user,options)
             ]
    #allow the tasks to add extra options
    for t in tasks:
        t.addOption(op.das.parser)
    
    op.run({})
    for d in op.dataset:
        previous = {}
        for t in tasks:

            t.dataset = d
            t.options = op.options
            t.user = op.user
        
            print t.name
            previous = t.run(previous)
            print '\t',previous
        
        
    