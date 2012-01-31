from CMGTools.H2TauTau.proto.framework.diclist import diclist

class Counter(diclist):
    
    def __init__(self, name):
        self.name = name
        super(Counter, self).__init__()
        
    def inc(self, level):
        '''Call this function to create a level for this counter,
        or to increment an existing level.
        '''
        if level not in self.dico:
            self.add( level, [level, 1])
        else:
            self[level][1] += 1
            
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




class Counters:
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



if __name__ == '__main__':
    c = Counter('Test')
    c.inc('All events')
    c.inc('All events')
    c.inc('pt cut')
    c.inc('All events')
    c.inc('pt cut')
    print c

    cs = Counters()
    cs.addCounter('test')
    print cs.counters
