#ROOTTOOLS
import pickle
from CMGTools.RootTools.utils.diclist import diclist

class Average(object):
    '''Need to implement error and RMS'''
    def __init__(self, name):
        self.name = name
        self.nTot = 0
        self.value = 0
        
    def add(self, value, weight=1.0):
        self.value += value * weight
        self.nTot += weight

    def average( self ):
        ave = None
        err = None 
        if self.nTot:
            ave = self.value / self.nTot
            err = 0
        return ave, err

    def __add__(self, other):
        '''Add two averages (+).'''
        self.value += other.value
        self.nTot += other.nTot
        return self

    def __iadd__(self, other):
        '''Add two averages (+=).'''
        return self.__add__(other)

    def write(self, dirname):
        '''Dump the average to a pickle file and to a text file in dirname.'''
        pckfname = '{d}/{f}.pck'.format(d=dirname, f=self.name)
        pckfile = open( pckfname, 'w' )
        pickle.dump(self, pckfile)
        txtfile = open( pckfname.replace('.pck', '.txt'), 'w')
        txtfile.write( str(self) )
        txtfile.write( '\n' )
        txtfile.close()

    def __str__(self):
        ave, err = self.average()
        tmp = None
        if ave is not None:
            tmp = 'Average {name:<15}: {average: 8.4f} +- {err:5.2f}'
            tmp = tmp.format( name = self.name,
                              average = ave,
                              err = err)
        else:
            tmp = 'Average {name:<15}: undefined (call Average.add)'.format( name = self.name)
        return tmp


class Averages(object):
    
    def __init__( self ):
        self.averages = []
        self.ranks = {}
        
    def addAverage(self, name):
        self.ranks[ name ] = len( self.averages )
        self.averages.append( Average(name) ) 

    def average(self, name):
        return self.averages[ self.ranks[name] ] 


class Averages(diclist):
    def write(self, dirname):
        map( lambda x: x.write(dirname), self)


if __name__ == '__main__':
    c = Average('TestAve')
    c.add( 1, 1 )
    c.add( 3, 2 )

    c2 = Average('TestAve2')
    c2.add(10,1)

    sum = c+c2
    print c
    print c2
    print sum
    sum.write('.')
