from ROOT import TFile, TTree, TChain
from CMGTools.RootTools.Chain import Chain
import pickle

keeper = []

def prepareComponents(dir, config, aliases=None, embed=True,
                      channel='TauMu', higgsMass=None, forcedLumi=None):
    '''Selects all components in configuration file. computes the integrated lumi
    from data components, and set it on the MC components.
    '''
    # all components in your configuration object (cfg)
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
    zComps = {}

    if aliases:
        aliases['DYJets'] = 'Ztt' 
    else:
        aliases = {'DYJets':'Ztt'}
    aliases['WJetsSoup'] = 'WJets'
    
    totIntLumi = 0
    newSelComps = {}
    embedComps = []
    
    # loop on all components
    for comp in selComps.values():
        if comp.isEmbed:
            embedComps.append(comp)
        comp.dir = comp.name
        if comp.name.startswith('Higgs'):
            if higgsMass is not None:
                if comp.name.find(higgsMass)==-1:
                    continue
                else:
                    comp.addWeight = 5.0
            else:
                comp.xSection = 1 # for datacards
        if comp.name.startswith('zdata'):
            zComps[comp.name] = comp
            # disabling is probably not necessary 
            comp.disabled = True
            continue
        comp.realName = comp.name
        comp.name = comp.name.replace('_Up','')
        comp.name = comp.name.replace('_Down','')
        alias = aliases.get(comp.name, None)
        if alias:
            comp.name = alias
        # do we already have a component with this name?
        # existingComp = newSelComps.get(comp.name, None)
        # import pdb; pdb.set_trace()
        # if existingComp is None:
        newSelComps[comp.name] = comp
        # else:
        #    import pdb; pdb.set_trace()
        #    newSelComps[comp.name] += comp
        if comp.isData is True:
            print comp.name, comp.intLumi
            totIntLumi += comp.intLumi

    # newSelComps now contains the actual components for H->tau tau
    # z->mu mu data is put in zComps

    if totIntLumi == 0 and forcedLumi:
        totIntLumi = forcedLumi
    for comp in selComps.values():
        if comp.isMC is True:
            comp.intLumi = totIntLumi

    # prepare weight dictionary, with all the components
    weights = dict( [ (comp.name,comp.getWeight()) \
                      for comp in newSelComps.values() ] )
    weights.update( dict( [ (comp.name,comp.getWeight()) \
                      for comp in zComps.values() ] ) )

    # attach the corresponding tree to each component
    def attachTree(comps, channel):
        treeName = 'ZJetsTreeProducer'
        fileName = '{treeName}_tree.root'.format(treeName=treeName)
        for comp in comps.values():
            fullName = '/'.join([ dir,
                                  comp.dir,
                                  treeName,
                                  fileName] )
            tree = TChain(treeName)
            tree.Add(fullName)
            comp.tree = tree
    attachTree(zComps,'MuMu')
    attachTree(newSelComps, channel)

    return newSelComps, weights, zComps
    
if __name__ == '__main__':
    import imp
    import sys
    from CMGTools.RootTools.RootTools import * 
    from CMGTools.H2TauTau.proto.plotter.categories_TauMu import *
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option("-C", "--channel", 
                      dest="channel", 
                      help="TauEle or TauMu",
                      default='TauMu')
    (options,args) = parser.parse_args()
    if len(args) != 2:
        parser.print_help()
        sys.exit(1)
    
    anaDir = args[0]
    cfgFileName = args[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    aliases = {'WJetsExt':'WJets'}
    # aliases = None
    
    selComps, weights, zComps = prepareComponents(anaDir, cfg.config, aliases, True, options.channel) 
