#!/bin/env python

import os
import sys
import imp
import re
import subprocess
import inspect
import operator
import pprint
import shutil
import difflib

from CMGTools.Production.datasetToSource import *
from CMGTools.Production.dataset import Dataset

patcmg_cfg = 'PATCMG_cfg.py'
cmg_cfg = 'CMG_cfg.py'
pattuple = 'patTuple.root'
cmgtuple = 'cmgTuple.root'

dataset_owner_mc = 'cmgtools'
# dataset_name_mc = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5'
dataset_name_mc = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v2/AODSIM/V5'


dataset_owner_data = 'CMS'
# dataset_name_data = '/TauPlusX/Run2011A-PromptReco-v4/AOD/V5'
dataset_name_data = '/TauPlusX/Run2012B-PromptReco-v1/AOD'
patExt = '/PAT_CMG_V5_4_0'

maxEvents = 3000

import inspect

def printArgs(frame):
    args, _, _, values = inspect.getargvalues(frame)
    print 'function name "%s"' % inspect.getframeinfo(frame)[2]
    for i in args:
        print "    %s = %s" % (i, values[i])

def checkRootFile( file ):
    fileForEdmf = file
    if not file.startswith('/store'):
        file = ':'.join(['file',file])
    edmf = ['edmFileUtil', '-P', file]
    out = subprocess.Popen(edmf, stdout=subprocess.PIPE, stderr=subprocess.PIPE ).communicate()[0]
    # print out
    (nevents, kbytesPerEvent, branches) = fileAna(out.split('\n'))
    if nevents == 0:
        raise ValueError('no event in ' + file )
    return (nevents, kbytesPerEvent, branches)


def fileAna( text ):
    pattern = re.compile('.* (\d+) events, (\d+) bytes.*')
    branches = []
    bytes = -1
    bytesPerEvent = -1
    for line in text:
        # print 'LINE', line
        m = pattern.match(line)
        if m:
            # import pdb; pdb.set_trace()
            nevents = int(m.group(1))
            bytesPerEvent = int(m.group(2))
        else:
            words = line.split()
            if len(words)>0 and words[0] == 'Branch':
                branches.append( (words[5], int(words[9]) ) )
    return (nevents, bytesPerEvent / 1024 / nevents, branches)        

 
class FileInfo( object ):
    def __init__(self, file):
        self.fileName = file
        self.nevents, self.kbytesPerEvent, self.branches = checkRootFile(file)
        provDumpCmd = ['edmProvDump', file]
        self.provDump = subprocess.Popen(provDumpCmd,
                                         stdout=subprocess.PIPE,
                                         stderr=subprocess.PIPE ).communicate()[0]
        self.theLog = []
        
    def write(self, dir=None):
        if dir is None:
            self.dir = os.path.splitext( self.fileName )[0] # remove .root
        else:
            self.dir = dir
        try: 
            os.mkdir( self.dir )
        except OSError:
            shutil.rmtree( self.dir )
            os.mkdir( self.dir )
        oldPwd = os.getcwd()
        os.chdir( self.dir )
        out1 = open('fileinfo.txt', 'w')
        out1.write( str(self) )
        out1.write('\n\n')
        out1.write( '\n'.join(self.theLog) )
        out1.write('\n')        
        out1.close()
        out2 = open('provdump.txt', 'w')
        out2.write( ''.join(self.provDump) )
        out2.write('\n')        
        out2.close()        
        os.chdir( oldPwd )
        
    def branchesBySize(self):
        '''branches, sorted by decreasing size'''
        return sorted( self.branches, key=operator.itemgetter(1), reverse=True)
    
    def __str__(self):
        lines = []
        lines.append( 'file    = {file}'.format(file=self.fileName) )
        lines.append( 'nevents = {nevents}'.format(nevents = self.nevents) )
        lines.append( 'kb/evt  = {kbpe}'.format(kbpe = self.kbytesPerEvent) )
        lines.append( 'branch list:' )
        for br in self.branchesBySize()[:20]:
            lines.append('\t' + str(br) )
        lines.append('\t...')
        return '\n'.join( lines )
    
    def log(self, astr):
        print astr
        self.theLog.append( str(astr) )
        
    def compare(self, other):
        theLines = []
        self.log('FILE 1 (TESTED)')
        self.log(self)
        self.log('FILE 2 (REFERENCE)')
        self.log(other)
        # assert(self.nevents == other.nevents)
        if self.kbytesPerEvent > 1.5*other.kbytesPerEvent:
            self.log('SIZE LARGER THAN 1.5*REFERENCE')
        sb = set( [branch for branch, size in self.branches] )
        ob = set( [branch for branch, size in other.branches] )  
        sbonly = sb - ob
        obonly = ob - sb
        if len(sbonly)>0:
            self.log('NEW COLLECTIONS')
            for br in sbonly:
                self.log('\t'+br)
        if len(obonly)>0:
            self.log('REMOVED COLLECTIONS')
            for br in obonly:
                self.log('\t'+br)
        

def rreplace(s, old, new, occurrence):
    '''replace the last occurence occurences of old by new in s.'''
    li = s.rsplit(old, occurrence)
    return new.join(li)


def makeInputCfg(outDirName, cfg, mc):
    infile = open(cfg)
    mc = str(mc)
    outCfgName = '/'.join([outDirName, cfg])
    # outCfgName = outCfgName.replace('_cfg.py','_{mc}_cfg.py'.format(mc=mc))
    outCfgName = rreplace( outCfgName, '_cfg.py', '_{mc}_cfg.py'.format(mc=mc), 1 )
    ofile = open( outCfgName, 'w' )
    for line in infile:
        line = line.rstrip()
        words = line.split()
        if len(words)==3 and words[0] == 'runOnMC' and words[1] == '=':
            line = line.replace( words[2], mc )
        ofile.write(line)
        ofile.write('\n')
    ofile.close()
    return outCfgName
         

def runCfg( cfg, idString, run=True, cmd='', mc=True, file=None ):


    print '*************************'
    frame = inspect.currentframe()
    printArgs(frame)
    print '*************************'
    
    outDirName = '{cfg}_{idString}'.format(cfg=cfg, idString=idString)
    try: 
        os.mkdir( outDirName )
    except OSError:
        shutil.rmtree( outDirName )
        os.mkdir( outDirName )
    
    cfg = makeInputCfg(outDirName, cfg, mc )

    handle = open(cfg, 'r')
    cfo = imp.load_source("pycfg", cfg, handle)
    process = cfo.process

    dsow = 'cmgtools_group'
    dsna = dataset_name_mc
    if mc is False:
        dsna = dataset_name_data
    if file is None:
        process.source = datasetToSource( dsow, dsna )
    else:
        process.source.fileNames = [':'.join(['file',file])]

    process.maxEvents =  cms.untracked.PSet( input = cms.untracked.int32(maxEvents) )

    outCfgFileName = 'run_cfg.py'
    cfgFile = open( '/'.join([outDirName, outCfgFileName]),'w')
    cfgFile.write('import FWCore.ParameterSet.Config as cms\n\n')
    cfgFile.write( process.dumpPython() )
    cfgFile.write( '\n' )
    cfgFile.close()

    if run:
        oldPwd = os.getcwd()
        os.chdir( outDirName )
        cmsRunCmd = ['cmsRun', outCfgFileName]
        # if cmd == 'nohup':
        #    cmsRunCmd.insert(0, cmd)
        #    cmsRunCmd.append('&')
        out = subprocess.Popen(cmsRunCmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE ).communicate()
        print out[0]
        print out[1]
        os.chdir( oldPwd )

    return outDirName





if __name__ == '__main__':

    # outDir_PATCMGT3_MC = runCfg( patcmg_cfg, 'PATCMGT3_MC', run=True, cmd='nohup', mc=True)
    # outDir_PATCMGT3_DATA = runCfg( patcmg_cfg, 'PATCMGT3_DATA', run=True, cmd='nohup', mc=False)

    
    # outDir_PATCMGT3_MC = runCfg( patcmg_cfg, 'PATCMGT3_MC', run=True, cmd='', mc=True)
    # checkRootFile(  outDir_PATCMGT3_MC + '/patTuple.root' )
    
    #outDir_PATCMGT3_MC = 'PATCMG_44X_cfg.py_PATCMGT3_MC'
    # outDir_CMGONPAT_MC = runCfg( cmg_cfg, 'CMGONPAT', run=True, cmd ='', mc=True,
    #                             file='/'.join(['..', outDir_PATCMGT3_MC, 'patTuple.root']) )     
    # checkRootFile(  outDir_CMGONPAT_MC + '/cmgTuple.root' )
    # fileInfo_CMGONPAT_MC = FileInfo( outDir_CMGONPAT_MC + '/cmgTuple.root' )

    print 'extracting reference MC pat-tuple information'
    ds_REF_PAT = Dataset('/'.join([dataset_name_mc,patExt]),
                         dataset_owner_mc, 'patTuple.*root')
    file_REF_PAT = ds_REF_PAT.listOfGoodFiles()[0]
    finf_REF_PAT = FileInfo( file_REF_PAT )

    print 'extracting reference MC cmg-tuple information'
    ds_REF_CMG = Dataset('/'.join([dataset_name_mc,patExt]),
                         dataset_owner_mc, 'cmgTuple.*root')
    file_REF_CMG = ds_REF_CMG.listOfGoodFiles()[0]
    finf_REF_CMG = FileInfo( file_REF_CMG )

##     print 'recreating MC PAT+CMG tuple from PFAOD', maxEvents, 'events... (can take time!)'
##     # FIXME: automatically get the PFAOD corresponding to the ref CMG+PAT 
    outDir_PATCMGT3_MC = runCfg( patcmg_cfg, 'PATCMGT3_MC', run=True, cmd='', mc=True)
    # outDir_PATCMGT3_MC = 'PATCMG_44X_cfg.py_PATCMGT3_MC'
    finf_NEW_PAT = FileInfo( outDir_PATCMGT3_MC + '/patTuple.root')
    finf_NEW_CMG = FileInfo( outDir_PATCMGT3_MC + '/cmgTuple.root')
    
    finf_NEW_PAT.compare( finf_REF_PAT )
    finf_NEW_CMG.compare( finf_REF_CMG )
    finf_NEW_PAT.write()
    finf_NEW_CMG.write()
    
    print 'recreating MC CMG tuple from local PAT tuple'
    outDir_CMG_MC = runCfg( cmg_cfg, 'CMG_MC', run=True, cmd='', mc=True,
                            file = '/'.join(['..', outDir_PATCMGT3_MC, 'patTuple.root']) )
    
    finf_CMG = FileInfo( outDir_CMG_MC + '/cmgTuple_reload.root')
    finf_CMG.compare( finf_NEW_CMG )
    finf_CMG.write()
