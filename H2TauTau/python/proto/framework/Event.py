class Event(object):

    def __init__(self, iEv):
        self.iEv = iEv
        #WARNING do I really want to define the weight here?
        self.eventWeight = 1
        
    def __str__(self):
        # I might want to get such a function by default for all of my classes
        header = '{type}:'.format( type=self.__class__.__name__)
        varlines = []
        for var,value in sorted(vars(self).iteritems()):
            # if hasattr(value, '__dict__'):
            #    value = str( vars(value) )
            tmp = None
            try:
                if str(iter( value )).startswith('<ROOT.reco::candidate'):
                    # a single reco::Candidate appears to be iterable...
                    # here, I want to consider it as an object, not a sequence.
                    raise TypeError('is not a vector')
                tmp = map(str, value)
            except TypeError:
                tmp = value
            varlines.append( '\t{var:<15}:   {value}'.format(var=var, value=tmp) )
        all = [ header ]
        all.extend(varlines)
        return '\n'.join( all )

