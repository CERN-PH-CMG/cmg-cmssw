from ROOT import TFile, TTree, TChain
from CMGTools.RootTools.Chain import Chain
from CMGTools.H2TauTau.proto.plotter.embed import embedScaleFactor

keeper = []


def prepareComponents(dir, config):
    '''Selects all components in configuration file. computes the integrated lumi
    from data components, and set it on the MC components.
    '''
    # all components in your configuration object (cfg)
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    zComps = {}
    
    aliases = {'DYJets':'Ztt'}
    
    
    totIntLumi = 0
    newSelComps = {}
    embedComps = []
    for comp in selComps.values():
        if comp.isEmbed:
            embedComps.append(comp)
        comp.dir = comp.name
        if comp.name.startswith('zdata'):
            zComps[comp.name] = comp
            # disabling is probably not necessary 
            comp.disabled = True
            continue
        alias = aliases.get(comp.name, None)
        if alias:
            comp.name = alias
        newSelComps[comp.name] = comp
        if comp.isData is True:
            print comp.name, comp.intLumi
            totIntLumi += comp.intLumi

    
    for comp in selComps.values():
        if comp.isMC is True:
            comp.intLumi = totIntLumi
    
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in newSelComps.values() ] )
    weights.update( dict( [ (comp.name,comp.getWeight()) \
                      for comp in zComps.values() ] ) )

    def attachTree(comps, channel):
        for comp in comps.values():
            fileName = '/'.join([ dir,
                                  comp.dir,
                                  'H2TauTauTreeProducer{channel}'.format(channel=channel),
                                  'H2TauTauTreeProducer{channel}_tree.root'.format(channel=channel)])
##             file = TFile(fileName)
##            keeper.append(file)
##            tree = file.Get('H2TauTauTreeProducer{channel}'.format(channel=channel))
            tree = TChain('H2TauTauTreeProducer{channel}'.format(channel=channel))
            tree.Add(fileName)
            comp.tree = tree
    attachTree(zComps,'MuMu')
    attachTree(newSelComps, 'TauMu')

    # import pdb; pdb.set_trace()
    eh, zh, embedFactor = embedScaleFactor(newSelComps)
    for comp in embedComps:
        comp.embedFactor = embedFactor
 
    return newSelComps, weights, zComps
    
if __name__ == '__main__':
    import imp
    import sys
    from CMGTools.RootTools.RootTools import * 
    from CMGTools.H2TauTau.proto.plotter.categories import *

    anaDir = sys.argv[1]
    cfgFileName = sys.argv[2]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    selComps, weights, zComps = prepareComponents(anaDir, cfg.config ) 
