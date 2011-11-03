class Counter:
    
    def __init__(self, name):
        self.name = name
        self.levels = {}
        
    def inc(self, level):
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
            retstr += '\t %s\t %d\t %2.2f\t %2.2f\n' % (level,
                                                       count,
                                                       float(count)/prev,
                                                       float(count)/init )
            prev = count
        return retstr

if __name__ == '__main__':
    c = Counter('test')
    c.inc('d')
    c.inc('a')
    c.inc('a')
    c.inc('b')
    print c
