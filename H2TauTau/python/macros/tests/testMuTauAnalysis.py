import unittest
import os
import CMGTools.H2TauTau.macros.MultiLoop as MultiLoop
import imp
import difflib

RUNLOOP = True

class TestMuTauAnalysis(unittest.TestCase):


    setUpDone = False
    
    def setUp(self):
        cfgFileName = '/'.join( [os.environ['CMSSW_BASE'],
                            'src/CMGTools/H2TauTau/Colin/analysis_Dec2_cfg.py'] )
        file = open( cfgFileName, 'r' )
        cfg = imp.load_source( 'cfg', cfgFileName, file)
        self.config = cfg.config
        
        self.testdir = 'Test'
        prog = '/'.join( [os.environ['CMSSW_BASE'],
                          'src/CMGTools/H2TauTau/python/macros/MultiLoop.py'] )

        dataDir = '/data/c/cbern/Data/Analysis/2011'
        self.dataDir = os.path.basename( dataDir )
        os.system( 'ln -sf ' + dataDir)

        cmd = 'python {prog} {dir} {cfg} -N 2000'.format( prog=prog,
                                                          dir=self.testdir,
                                                          cfg=cfgFileName )
        # class Options:
        #    pass
        #options = Options()
        #options.force = True
        #options.nevents = 5000
        #options.iEvent = None
        
        # args = [ self.testdir, cfgFileName ]

        if self.__class__.setUpDone == False and RUNLOOP: 
            if os.path.isdir( self.testdir ) or os.path.isfile( self.testdir ):
                os.system('rm -r ' + self.testdir )
            # MultiLoop.main( options, args )
            # must run it like that or multiprocessing doesn't work
            os.system( cmd ) 
            self.__class__.setUpDone = True
        else:
            print 'SETUP ALREADY DONE.'

    def tearDown(self):
        # os.system( 'rm -r ' + self.testdir )
        # os.system( 'rm ' + self.dataDir )
        pass
        
    def test_outputdirs(self):
        '''The output dir should contain a subdirectory for each component, plus
        DYJets_Fakes, if DYJets is among the selected components.'''
        selCompNames = set([comp.name for comp in self.config.components])
        contents = set()
        for file in os.listdir( self.testdir ):
            fileName = '/'.join( [self.testdir, file] )
            if os.path.isdir( fileName):
                contents.add( file )
        # print '****'
        # print selCompNames
        # print contents
        if 'DYJets' in selCompNames:
            self.assertTrue( selCompNames.issubset(contents) )
            diff = contents.difference( selCompNames)
            self.assertTrue( len(diff) is 1 )
            self.assertTrue(diff.pop() == 'DYJets_Fakes')
        else:
            self.assertTrue( selCompNames == contents )

    def test_logs(self):
        selCompNames = set([comp.name for comp in self.config.components])
        for comp in selCompNames:
            template = open('/'.join( ['Template', comp, 'log.txt'] )).readlines()
            test = open('/'.join( ['Test', comp, 'log.txt'] )).readlines()
            diffres = ''.join(difflib.unified_diff( test, template))
            if diffres != '':
                print '------'
                print comp
                print '*** DIFF ***'
                print diffres
                print '--- DONE ---'
            self.assertTrue( diffres == '') 
            
        
if __name__ == '__main__':
    unittest.main()
