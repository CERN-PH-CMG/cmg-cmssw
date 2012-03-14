from CMGTools.RootTools.fwlite.Weight import Weight
import glob

class CFG(object):
    '''Base configuration class. The attributes are used to store parameters of any type'''
    def __init__(self, **kwargs):
        '''All keyword arguments are added as attributes.'''
        self.__dict__.update( **kwargs )
        
    def __str__(self):
        '''A useful printout'''
        header = '{type}: {name}'.format( type=self.__class__.__name__,
                                          name=self.name)
        varlines = ['\t{var:<15}:   {value}'.format(var=var, value=value) \
                    for var,value in sorted(vars(self).iteritems()) \
                    if var is not 'name']
        all = [ header ]
        all.extend(varlines)
        return '\n'.join( all )

class Analyzer( CFG ):
    '''Base analyzer configuration, see constructor'''
    def __init__(self, name, verbose=False, **kwargs):
        '''
        One could for example define the analyzer configuration for a
        di-muon framework.Analyzer.Analyzer in the following way:
        
        ZMuMuAna = cfg.Analyzer(
        "ZMuMuAnalyzer",
        pt1 = 20,
        pt2 = 20,
        iso1 = 0.1,
        iso2 = 0.1,
        eta1 = 2,
        eta2 = 2,
        m_min = 0,
        m_max = 200
        )
        
        Any kinds of keyword arguments can be added.
        The name must be present, and must be well chosen, as it will be used
        by the Looper to find the module containing the Analyzer class.
        This module should be in your PYTHONPATH. If not, modify your python path
        accordingly in your script.
        '''

        self.name = name
        self.verbose = verbose 
        # self.cfg = CFG(**kwargs)
        super(Analyzer, self).__init__(**kwargs)
        

class Sequence( list ):
    '''A list with print functionalities.

    Used to define a sequence of analyzers.'''
    def __str__(self):
        tmp = [] 
        for index, ana in enumerate( self ):
            tmp.append( '{index} :'.format(index=index) )
            tmp.append( '{ana} :'.format(ana=ana) )
        return '\n'.join(tmp)
        

class Component( CFG ):
    '''Base component class.

    See the child classes:
    DataComponent, MCComponent, EmbedComponent
    for more information.'''
    def __init__(self, name, files, triggers=None, **kwargs):
        if isinstance(triggers, basestring):
            triggers = [triggers]
        if type(files) == str:
            files = sorted(glob.glob(files))
        super( Component, self).__init__( name = name,
                                          files = files,
                                          triggers = triggers, **kwargs)
        self.isMC = False
        self.isData = False
        self.isEmbed = False 


class DataComponent( Component ):

    def __init__(self, name, files, intLumi, triggers, json=None):
        super(DataComponent, self).__init__(name, files, triggers)
        self.isData = True 
        self.intLumi = intLumi
        self.json = json

    def getWeight( self, intLumi = None):
        return Weight( genNEvents = -1,
                       xSection = None,
                       genEff = -1,
                       intLumi = self.intLumi,
                       addWeight = 1. ) 

class EmbedComponent( Component ):
    def __init__(self, name, **kwargs ):
        super( EmbedComponent, self).__init__( name = name,
                                               **kwargs )
        # self.tauEffWeight = None
        # self.muEffWeight = None
        #WARNING what to do here ?? 
        self.isEmbed = True

    def getWeight( self, intLumi = None):
        return Weight( genNEvents = -1,
                       xSection = None,
                       genEff = -1,
                       intLumi = None,
                       addWeight = 1. ) 

        
class MCComponent( Component ):
    def __init__(self, name, files, triggers, xSection,
                 nGenEvents, 
                 # vertexWeight,tauEffWeight, muEffWeight,
                 effCorrFactor, **kwargs ):
        super( MCComponent, self).__init__( name = name,
                                            files = files,
                                            triggers = triggers, **kwargs )
        self.xSection = xSection
        self.nGenEvents = nGenEvents
        # self.vertexWeight = vertexWeight
        self.effCorrFactor = effCorrFactor
        # self.tauEffWeight = tauEffWeight
        # self.muEffWeight = muEffWeight
        self.isMC = True

    def getWeight( self, intLumi = None):
        if intLumi is None:
            intLumi = Weight.FBINV
        #COLIN THIS WEIGHT STUFF IS REALLY BAD!!
        # use the existing Weight class or not? guess so...
        return Weight( genNEvents = self.nGenEvents,
                       xSection = self.xSection,
                       intLumi = None,
                       genEff = 1/self.effCorrFactor,
                       addWeight = 1. )

class Config( object ):
    '''Main configuration object, holds a sequence of analyzers, and
    a list of components.'''
    def __init__(self, components, sequence):
        self.components = components
        self.sequence = sequence

    def __str__(self):
        comp = '\n'.join( map(str, self.components))
        sequence = str( self.sequence)
        return '\n'.join([comp, sequence])


if __name__ == '__main__':

    ana1 = Analyzer('ana1',
                    toto = '1',
                    tata = 'a')

    ana2 = Analyzer('ana2',
                    toto = '2',
                    bulu = 'b',
                    protch = ['blah'])

    sequence = Sequence( [ana1, ana2] )
    print sequence

    comp1 = Component( 'DYJets',
                       files='*.root',
                       triggers='HLT_stuff')
    print 
    print comp1

    ecomp = EmbedComponent('Embed',
                           files='*.root',
                           triggers='HLT_stuff')

    print 
    print ecomp
    

    DYJets = MCComponent(
        name = 'DYJets',
        files ='blah_mc.root',
        xSection = 3048.,
        nGenEvents = 34915945,
        triggers = ['HLT_MC'],
        vertexWeight = 1.,
        effCorrFactor = 1 )

    print
    print DYJets

