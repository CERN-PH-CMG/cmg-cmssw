import os

# start from a PFAOD file

inputs = [
    'AOD_TTJets.root',
#    'AOD_DoubleMu.root',
#    'AOD_DoubleElectron.root',
#    'AOD_HT.root'
    ]

# run a set of cfgs on that file

cfgs = [
#    'photons_cfg.py',
    'all_cfg.py',
#    'noCastor_cfg.py',
#    'tevMuons_cfg.py',
    'bare_cfg.py',
    'V2_cfg.py',
    'V3_cfg.py',
#    'ak7calo_cfg.py',
#    'caloClusters_cfg.py'
    ]

# cfgs = cfgs[:1]

def prepareTestBench(input):
    outputDir = 'Out_' + os.path.splitext( input )[0]
    print 'preparing output directory', outputDir
    try:
        os.mkdir(outputDir)
    except OSError:
        pass
    def copy( file ):
        os.system( ' '.join( ['cp',file,outputDir] ))
    copy(input)
    map( copy, cfgs)
    copy( 'base.py' )
    return outputDir 

def processInput(input):
    print 'processing', input
    outputDir = prepareTestBench(input) 
    baseDir = os.getcwd()
    os.chdir( outputDir )
    for cfg in cfgs:
        print cfg
        cmd = ['cmsRun', cfg, input]
        os.system( ' '.join(cmd) ) 
    os.chdir( baseDir )
        
for input in inputs:
    processInput(input)

