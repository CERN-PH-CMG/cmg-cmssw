class Counter:
    
    def __init__(self, name):
        self.name = name
        self.levels = {}
        
    def inc(self, level):
        '''Call this function to create a level for this counter,
        or to increment an existing level.

        If you take care to give level names sorted in the way you want,
        the Counter class can generate cut flow table.
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
            retstr += '\t %s\t %d\t %2.3f\t %2.3f\n' % (level,
                                                        count,
                                                        float(count)/prev,
                                                        float(count)/init )
            prev = count
        return retstr

if __name__ == '__main__':
    c = Counter('Test')
    c.inc('a: All events')
    c.inc('a: All events')
    c.inc('b: pt cut')
    c.inc('a: All events')
    c.inc('b: pt cut')
    print c
