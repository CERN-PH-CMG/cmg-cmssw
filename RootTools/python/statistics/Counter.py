import pickle
from CMGTools.RootTools.utils.diclist import diclist

class Counter(diclist):
    
    def __init__(self, name):
        self.name = name
        super(Counter, self).__init__()
        
    def inc(self, level, nentries=1):
        '''Call this function to create a level for this counter,
        or to increment an existing level.
        '''
        if level not in self.dico:
            self.add( level, [level, nentries])
        else:
            self[level][1] += nentries

    def __add__(self, other):
        '''Add two counters (+).'''
        size = max( len(self), len(other))
        # import pdb; pdb.set_trace()
        for i in range(0, size):
            if i>=len(other):
                # this line exists only in this counter, leave it as is
                continue
            elif i>=len(self):
                self.inc( other[i][0], other[i][1] )
            else:
                # exists in both
                if self[i][0] != other[i][0]:  
                    err = ['cannot add these counters:', str(self), str(other)]
                    raise ValueError('\n'.join(err))
                else:
                    self.inc( other[i][0], other[i][1] )
        return self

    def __iadd__(self, other):
        '''Add two counters (+=).'''
        return self.__add__(other)

    def write(self, dirname):
        '''Dump the counter to a pickle file and to a text file in dirname.'''
        pckfname = '{d}/{f}.pck'.format(d=dirname, f=self.name)
        pckfile = open( pckfname, 'w' )
        pickle.dump(self, pckfile)
        txtfile = open( pckfname.replace('.pck', '.txt'), 'w')
        txtfile.write( str(self) )
        txtfile.write( '\n' )
        txtfile.close()
        
    def __str__(self):
        retstr = 'Counter %s :\n' % self.name
        prev = None
        init = None
        for level, count in self:
            if prev == None:
                prev = count
                init = count
            retstr += '\t {level:<40} {count:>9} \t {eff1:4.2f} \t {eff2:6.4f}\n'.format(
                level=level,
                count=count,
                eff1=float(count)/prev,
                eff2=float(count)/init )
            prev = count
        return retstr




class Counters(object):
    '''
    TODO: could be a diclist? 
    '''
    
    def __init__( self ):
        self.counters = []
        self.ranks = {}
        
    def addCounter(self, name):
        self.ranks[ name ] = len( self.counters )
        self.counters.append( Counter(name) ) 

    def counter(self, name):
        return self.counters[ self.ranks[name] ] 

    def write(self, dirname):
        map( lambda x: x.write(dirname), self.counters)

    def __str__(self):
        prints = map( str, self.counters )
        return '\n'.join(prints)
        


if __name__ == '__main__':
    
    c = Counter('Test')
    c.inc('a')
    
    print c

    cs = Counters()
    cs.addCounter('test')
    cs.addCounter('test2')
    cs.counter('test').inc('a')
    cs.counter('test').inc('a')
    cs.counter('test').inc('b')
    cs.counter('test2').inc('a')
    
    print cs

    cs.write('.')
    print 'loading ...'
    file = open('test.pck')
    lcs = pickle.load(file)
    print lcs

    c1 = cs.counter('test2')

    print 'test addition, adding test and test2'
    import copy
    c2 = cs.counter('test')
    c1 += c2 
    print c1

    print 'test addition : incompatible'
    c = Counter('Test3')
    c.inc('b')
    c1 += c
    print c1
