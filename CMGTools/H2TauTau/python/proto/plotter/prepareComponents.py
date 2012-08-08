from ROOT import TFile, TTree, TChain
from CMGTools.RootTools.Chain import Chain
from CMGTools.H2TauTau.proto.plotter.embed import embedScaleFactor
import pickle

keeper = []

def readPickles(dir, config, embed=True, channel='TauMu',higgsMass=None):
    selComps = dict( [ (comp.name, comp) for comp in config.components ])
#    print selComps
    pickleFiles = {}
    for comp in selComps.values():
        fileName = '/'.join([ dir,
                              comp.dir,
                              'TauEleAnalyzer/DiLepton.pck'])

        pickleFiles[comp.name] = pickle.load( open( fileName, 'rb' ) )
    return pickleFiles


def prepareComponents(dir, config, aliases=None, embed=True, channel='TauMu', higgsMass=None):
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

    # compute integrated luminosity
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
        for comp in comps.values():
            fileName = '/'.join([ dir,
                                  comp.dir,
                                  'H2TauTauTreeProducer{channel}'.format(channel=channel),
                                  'H2TauTauTreeProducer{channel}_tree.root'.format(channel=channel)])
            tree = TChain('H2TauTauTreeProducer{channel}'.format(channel=channel))
            tree.Add(fileName)
            comp.tree = tree
    attachTree(zComps,'MuMu')
    attachTree(newSelComps, channel)

    # compute the embedded sample weighting factor
    if embed and newSelComps.get('Ztt', False):
        eh, zh, embedFactor = embedScaleFactor(newSelComps)
        for comp in embedComps:
            # import pdb; pdb.set_trace()
            comp.embedFactor = embedFactor
 
    pickles = readPickles(dir, config, embed, channel, higgsMass)
#    print 'TTJets',pickles['TTJets']['all events'][1]

    for pick in pickles:
        print 'reading pickle file',pick,pickles[pick]['all events'][1]
        newSelComps[pick].totEvents = pickles[pick]['all events'][1]


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
