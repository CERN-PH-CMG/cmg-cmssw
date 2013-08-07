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

    def __init__(self, name, files, triggers=None, fraction=1):
        self.name = name 
        self.files = files
        self.isMC = False
        self.isData = False
        self.isEmbed = False
        self.fraction = fraction
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


class EmbedComponent( Component ):

    def __init__(self, name, files, triggers, tauEffWeight, muEffWeight, fraction=1):
        super(EmbedComponent, self).__init__(name, files, triggers, fraction)
        self.isEmbed = True
        self.tauEffWeight = tauEffWeight
        self.muEffWeight = muEffWeight

    def getWeight( self, intLumi = None):
        return Weight( genNEvents = -1,
                       xSection = None,
                       genEff = -1,
                       intLumi = None,
                       addWeight = 1/float(self.fraction) ) 

        
class DataComponent( Component ):

    def __init__(self, name, files, intLumi, triggers, fraction=1):
        super(DataComponent, self).__init__(name, files, triggers, fraction)
        self.isData = True 
        self.intLumi = intLumi

    def getWeight( self, intLumi = None):
        return Weight( genNEvents = -1,
                       xSection = None,
                       genEff = -1,
                       intLumi = self.intLumi,
                       addWeight = 1/float(self.fraction) ) 


class MCComponent( Component ): 

    def __init__(self, name, files, xSection, nGenEvents,
                 triggers, vertexWeight, tauEffWeight, muEffWeight,
                 effCorrFactor, fraction=1):
        super(MCComponent, self).__init__(name, files, triggers, fraction)
        self.isMC = True
        self.xSection = xSection
        self.nGenEvents = nGenEvents
        self.vertexWeight = vertexWeight
        self.tauEffWeight = tauEffWeight
        self.muEffWeight = muEffWeight
        self.effCorrFactor = effCorrFactor
        self.jetScale = 1  
        self.jetSmear = 0  
        # self.tauTriggerTOC = tauTriggerTOC
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
                       genEff = 1/self.effCorrFactor,
                       addWeight = 1/float(self.fraction) )

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

    import sys
    import imp
    
    cfgFileName = sys.argv[1]
    file = open( cfgFileName, 'r' )
    cfg = imp.load_source( 'cfg', cfgFileName, file)

    print cfg.config


