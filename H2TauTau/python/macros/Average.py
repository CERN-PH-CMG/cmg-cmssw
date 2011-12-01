class Average:
    '''Need to implement error and RMS'''
    def __init__(self, name):
        self.name = name
        self.nTot = 0
        self.value = 0
        
    def add(self, value, weight=1.0):
        self.value += value * weight
        self.nTot += weight

    def average( self ):
        ave = self.value / self.nTot
        err = 0
        return ave, err

    def __str__(self):
        tmp = 'Average {name:<15}: {average:5.2f} +- {err:5.2f}'
        ave, err = self.average()
        tmp = tmp.format( name = self.name,
                          average = ave,
                          err = err)
        return tmp

if __name__ == '__main__':
    c = Average('Test')
    c.add( 1, 1 )
    c.add( 3, 2 )
    print c
