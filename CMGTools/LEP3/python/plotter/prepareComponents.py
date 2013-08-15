from ROOT import TFile, TTree, TChain
from CMGTools.RootTools.Chain import Chain
from CMGTools.H2TauTau.proto.plotter.embed import embedScaleFactor

keeper = []


def prepareComponents(dir, config, intLumi=500000.):
    '''Selects all components in configuration file. computes the integrated lumi
    from data components, and set it on the MC components.
    '''
    # all components in your configuration object (cfg)
    selComps = dict( [ (comp.name, comp) for comp in config.components ])

    totIntLumi = intLumi
    newSelComps = {}
    
    # loop on all components
    for comp in selComps.values():
        comp.dir = comp.name
        comp.realName = comp.name
        # if comp.name == 'HZVIS':
        #     comp.xSection *= 10
        newSelComps[comp.name] = comp

    for comp in selComps.values():
        if comp.isMC is True:
            comp.intLumi = totIntLumi

    # prepare weight dictionary, with all the components
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in newSelComps.values() ] )

    # attach the corresponding tree to each component
    def attachTree(comps):
        for comp in comps.values():
            fileName = '/'.join([ dir,
                                  comp.dir,
                                  'SixJetTreeProducer',
                                  'SixJetTreeProducer_tree.root'])
            tree = TChain('SixJetTreeProducer')
            tree.Add(fileName)
            comp.tree = tree
    attachTree(newSelComps)

    return newSelComps, weights
    
if __name__ == '__main__':
    import imp
    import sys
    from CMGTools.RootTools.RootTools import * 
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = '%prog <anaDir> <cfgFile>'
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights = prepareComponents(anaDir, cfg.config)
