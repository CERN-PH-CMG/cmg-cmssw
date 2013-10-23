#ROOTTOOLS
class Counter:
    
    def __init__(self, name):
        self.name = name
        self.levels = {}
        
    def inc(self, level):
        '''Call this function to create a level for this counter,
        or to increment an existing level.

        If you take care to give level names sorted in the way you want,
        the Counter class can generate a cut flow table.
        Just print the counter to get the table.

        For example:
          count = Counter("Test")
          count.inc("a: all events")
          count.inc("b: pt cut")
          count.inc("c: tighter pt cut")
          count.inc("d: final selection")
        
        '''
        if level in self.levels:
            self.levels[level] += 1
        else:
            self.levels[level] = 1
            
    def __str__(self):
        retstr = 'Counter %s :\n' % self.name
        prev = None
        init = None
        for level, count in sorted(self.levels.iteritems()):
            if prev == None:
                prev = count
                init = count
            retstr += '\t {level:<40} {count:>9} \t {eff1:4.2f} \t {eff2:6.4f}\n'.format(level=level,
                                                                                count=count,
                                                                                eff1=float(count)/prev,
                                                                                eff2=float(count)/init )
            prev = count
        return retstr




class Counters:
    
    def __init__( self ):
        self.counters = []
        self.ranks = {}
        
    def addCounter(self, name):
        self.ranks[ name ] = len( self.counters )
        self.counters.append( Counter(name) ) 

    def counter(self, name):
        return self.counters[ self.ranks[name] ] 



if __name__ == '__main__':
    c = Counter('Test')
    c.inc('a: All events')
    c.inc('a: All events')
    c.inc('b: pt cut')
    c.inc('a: All events')
    c.inc('b: pt cut')
    print c

    cs = Counters()
    cs.addCounter('test')
    print cs.counters
