#ROOTTOOLS
import math
import pickle
from CMGTools.RootTools.utils.diclist import diclist

class Average(object):
    '''Need to implement error and RMS'''
    def __init__(self, name):
        self.name = name
        self.sumw = 0
        self.value = 0
        self.values = []
        
    def add(self, value, weight=1.0):
        self.value += value * weight
        self.sumw += weight
        self.values.append( (value, weight) )        

    def variance(self, ave):
        sumw = 0
        sumw2 = 0
        wsum = 0
        for x, w in self.values:
            delta = x - ave 
            wsum += w * delta * delta
            sumw += w
            sumw2 += w*w
        variance = wsum * sumw / (sumw*sumw - sumw2)
        return variance 
    
    def average( self ):
        ave = None
        err = None 
        if self.sumw:
            ave = self.value / self.sumw
            err = math.sqrt( self.variance( ave )) /  math.sqrt( self.sumw ) 
        return ave, err

    def __add__(self, other):
        '''Add two averages (+).'''
        self.value += other.value
        self.sumw += other.sumw
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
            tmp = 'Average {name:<15}: {average: 8.4f} +- {err:8.4f}'
            # tmp = 'Average {name:<15}: {average: 8.4f}'
            tmp = tmp.format( name = self.name,
                              average = ave,
                              err = err
                              )
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
    # c2.add(10,1)

    sum = c+c2
    print c
    print c2
    print sum
    sum.write('.')

    import random

    c3 = Average('Gauss')
    for i in range(0,100):
        c3.add( random.gauss( 5, 1 ) ) 
    print c3
    # print math.sqrt( c3.variance(c3.average()[0]) )
