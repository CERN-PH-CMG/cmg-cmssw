class CFG(object):
    def __init__(self, **kwargs):
        self.__dict__.update( **kwargs )
        
    def __str__(self):
        # I might want to get this function by default for all of my classes
        header = '{type}: {name}'.format( type=self.__class__.__name__,
                                          name=self.name)
        varlines = ['\t{var:<15}:   {value}'.format(var=var, value=value) \
                    for var,value in sorted(vars(self).iteritems()) \
                    if var is not 'name']
        all = [ header ]
        all.extend(varlines)
        return '\n'.join( all )

class Analyzer( CFG ):
    def __init__(self, name, verbose=False, **kwargs):
        self.name = name
        self.verbose = verbose 
        # self.cfg = CFG(**kwargs)
        super(Analyzer, self).__init__(**kwargs)
        

class Sequence( list ):
    '''A list with print functionalities. For analyzer sequence.'''
    def __str__(self):
        tmp = [] 
        for index, ana in enumerate( self ):
            tmp.append( '{index} :'.format(index=index) )
            tmp.append( '{ana} :'.format(ana=ana) )
        return '\n'.join(tmp)
        

class Component( CFG ):
    def __init__(self, name, files, triggers=None):
        if isinstance(triggers, basestring):
            triggers = [triggers]
        super( Component, self).__init__( name = name,
                                          files = files,
                                          triggers = triggers)
        self.isMC = False
        self.isData = False
        self.isEmbed = False 


class DataComponent( Component ):

    def __init__(self, name, files, intLumi, triggers):
        super(DataComponent, self).__init__(name, files, triggers)
        self.isData = True 
        self.intLumi = intLumi

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

        
class MCComponent( Component ):
    def __init__(self, name, files, triggers, xSection, nGenEvents,
                 vertexWeight, tauEffWeight, muEffWeight, effCorrFactor ):
        super( MCComponent, self).__init__( name = name,
                                            files = files,
                                            triggers = triggers )
        self.xSection = xSection
        self.nGenEvents = nGenEvents
        self.vertexWeight = vertexWeight
        self.effCorrFactor = effCorrFactor
        self.tauEffWeight = tauEffWeight
        self.muEffWeight = muEffWeight
        self.isMC = True


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

