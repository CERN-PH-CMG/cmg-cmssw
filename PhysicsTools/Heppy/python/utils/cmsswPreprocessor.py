import os                                                               
import re 
import imp
import timeit
import subprocess
from math import ceil
from PhysicsTools.HeppyCore.framework.config import CFG
from PhysicsTools.Heppy.utils.edmUtils import edmFileLs
class CmsswPreprocessor :
	def __init__(self,configFile,command="cmsRun", addOrigAsSecondary=True, prefetch=False) :
		self.configFile=configFile
		self.command=command
		self.addOrigAsSecondary=addOrigAsSecondary
		self.prefetch=prefetch
		self.garbageFiles=[]
	def prefetchOneXrootdFile(self,fname):
		tmpdir = os.environ['TMPDIR'] if 'TMPDIR' in os.environ else "/tmp"
		rndchars  = "".join([hex(ord(i))[2:] for i in os.urandom(8)])
		localfile = "%s/%s-%s.root" % (tmpdir, os.path.basename(fname).replace(".root",""), rndchars)
		try:
		    print "Fetching %s to local path %s " % (fname,localfile)
		    start = timeit.default_timer()
		    subprocess.check_output(["xrdcp","-f","-N",fname,localfile])
		    print "Time used for transferring the file locally: %s s" % (timeit.default_timer() - start)
		    return (localfile,True)
		except:
		    print "Could not save file locally, will run from remote"
		    if os.path.exists(localfile): os.remove(localfile) # delete in case of incomplete transfer
		    return (fname,False)
	def maybePrefetchFiles(self,component):
		newfiles = []
		component._preprocessor_tempFiles = []
		for fn in component.files:
		    if self.prefetch and fn.startswith("root://"):
		        (newfile,istemp) = self.prefetchOneXrootdFile(fn)
		        newfiles.append(newfile)
		        if istemp: 
		            component._preprocessor_tempFiles.append(newfile)
		    else:
		        newfiles.append(fn)
		component.files = newfiles
	def endLoop(self,component):
		for fname in component._preprocessor_tempFiles:
		    print "Removing local cache file ",fname
		    os.remove(fname)
		component._preprocessor_tempFiles = []
	def run(self,component,wd,firstEvent,nEvents):
		if firstEvent != 0: raise RuntimeError, "The preprocessor can't skip events at the moment"
                fineSplitIndex, fineSplitFactor = getattr(component, 'fineSplit', (1,1))
                if fineSplitFactor > 1:
                    if len(component.files) != 1:
                        raise RuntimeError, "Any component with fineSplit > 1 is supposed to have just a single file, while %s has %s" % (component.name, component.files)
                    evtsInFile = edmFileLs(component.files[0])['events']
                    if nEvents in (None, -1) or nEvents > evtsInFile: nEvents =  evtsInFile
                    nEvents = int(ceil(nEvents/float(fineSplitFactor)))
                    firstEvent = fineSplitIndex * nEvents
                    # Now we will run on these events, and the output will contain only those
                    # Thus, we switch off fine-split in the component
                    component.fineSplit = (1,1)
		if nEvents is None:
			nEvents = -1
		self.maybePrefetchFiles(component)
		cmsswConfig = imp.load_source("cmsRunProcess",os.path.expandvars(self.configFile))
		inputfiles= []
		for fn in component.files :
			if not re.match("file:.*",fn) and not re.match("root:.*",fn) :
				fn="file:"+fn
			inputfiles.append(fn)
		cmsswConfig.process.source.fileNames = inputfiles
		cmsswConfig.process.maxEvents.input = nEvents
		cmsswConfig.process.source.skipEvents = cmsswConfig.cms.untracked.uint32(firstEvent)
		#fixme: implement skipEvent / firstevent

		outfilename=wd+"/cmsswPreProcessing.root"
		# for outName in cmsswConfig.process.endpath.moduleNames():
		for module in cmsswConfig.process.endpaths.viewvalues():
			for outName in module.moduleNames():
				out = getattr(cmsswConfig.process,outName)
    			out.fileName = outfilename

		if not hasattr(component,"options"):
			component.options = CFG(name="postCmsrunOptions")
                #use original as primary and new as secondary 
                #component.options.inputFiles= component.files
		#component.options.secondaryInputFiles=[outfilename]

                #use new as primary and original as secondary
                if self.addOrigAsSecondary:
                    component.options.secondaryInputFiles= component.files
		component.options.inputFiles=[outfilename]
                component.files=[outfilename]

		configfile=wd+"/cmsRun_config.py"
		f = open(configfile, 'w')
		f.write(cmsswConfig.process.dumpPython())
		f.close()
		runstring="%s %s >& %s/cmsRun.log" % (self.command,configfile,wd)
		print "Running pre-processor: %s " %runstring
                ret=os.system(runstring)
                if ret != 0:
                     print "CMSRUN failed"
                     exit(ret)
		return component
