from pprint import pprint

from CMGTools.H2TauTau.macros.TurnOnCurve import TurnOnCurve
from CMGTools.H2TauTau.macros.Weight import Weight

# need to think about cut definition
# same kind of cuts for tau, e, mu
# specific cuts can be added for each of them
class Cuts(object):

    def __str__(self):
        header = 'Cuts: '
        varlines = ['\t{var:<15}:   {value}'.format(var=var, value=value) \
                    for var,value in sorted( vars(self).iteritems() )]
        all = [ header ]
        all.extend(varlines)
        return '\n'.join( all )


class Component(object):

    def __init__(self, name, files, triggers=None ):
        self.name = name 
        self.files = files
        if isinstance(triggers, basestring):
            self.triggers = [triggers]
        else:
            self.triggers = triggers


    def __str__(self):
        header = 'Component: ' + self.name
        varlines = ['\t{var:<15}:   {value}'.format(var=var, value=value) \
                    for var,value in sorted(vars(self).iteritems())]
        all = [ header ]
        all.extend(varlines)
        return '\n'.join( all )

        
class DataComponent( Component ):

    def __init__(self, name, files, intLumi, triggers ):
        super(DataComponent, self).__init__(name, files, triggers)
        self.isMC = False
        self.intLumi = intLumi

    def getWeight( self, intLumi = None):
        return Weight( genNEvents = -1,
                       xSection = None,
                       genEff = -1,
                       intLumi = self.intLumi ) 

class MCComponent( Component ): 

    def __init__(self, name, files, xSection, nGenEvents,
                 triggers, vertexWeight, effCorrFactor, tauTriggerTOC):
        super(MCComponent, self).__init__(name, files, triggers)
        self.isMC = True
        self.xSection = xSection
        self.nGenEvents = nGenEvents
        self.vertexWeight = vertexWeight
        self.effCorrFactor = effCorrFactor
        self.tauTriggerTOC = tauTriggerTOC
        #COLIN: need to adapt turn on curve to period, see Jose's code
        # self.tauTriggerTOC = TurnOnCurve()

    def getWeight( self, intLumi = None):
        if intLumi is None:
            intLumi = Weight.FBINV
        #COLIN THIS WEIGHT STUFF IS REALLY BAD!!
        # use the existing Weight class or not? guess so...
        return Weight( genNEvents = self.nGenEvents,
                       xSection = self.xSection,
                       intLumi = None,
                       genEff = 1/self.effCorrFactor )

class Period( object ):

    def __init__(self, components):
        self.components = components

class Config(object):

    def __init__(self, components, cuts):
        self.components = components
        self.cuts = cuts

    def __str__(self):
        comp = '\n'.join( map(str, self.components))
        cuts = str( self.cuts)
        return '\n'.join([comp, cuts])


if __name__ == '__main__':

    baseDir = '2011'
    filePattern = '*fullsel*root'

    mc_triggers = None
    data_triggers = ['HLT_Data']

    # cut values 
    cuts = Cuts()
    inf = float('+inf')
    cuts.tauPt = ( 20, inf )
    cuts.muPt = ( 17, inf )
    cuts.muEta =  (-1., 1.)
    cuts.vbfJetPt = (30, inf)
    cuts.vbfMjj = (400, inf)
    cuts.vbfDeta = (4, inf)
    cuts.boostedJetPt = (150, inf)

    # global MC weighting factors
    mc_mu_weight = 0.968
    mc_tau_weight = 0.92
    mc_weight = mc_mu_weight * mc_tau_weight

    # the following depends on the period... 
    # event MC weighting factors
    mc_vertexWeight = 'vertexWeight2011AB'

    WJets = MCComponent( name = 'WJets',
                         files ='{baseDir}/WJets/{filePattern}'.format(baseDir=baseDir,
                                                                       filePattern=filePattern),
                         xSection = 10000.,
                         nGenEvents = 999999,
                         triggers = mc_triggers,
                         weight = mc_weight,
                         vertexWeight = mc_vertexWeight )

    data = DataComponent( name = 'data',
                          files ='{baseDir}/data/{filePattern}'.format(baseDir=baseDir,
                                                                       filePattern=filePattern),
                          intLumi = 990.,
                          triggers = data_triggers )

    config = Config( components = [WJets, data],
                     cuts = cuts )

    print config

    period2011A = Period( data )

