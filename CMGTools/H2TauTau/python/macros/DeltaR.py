#ROOTTOOLS
import math
import copy

def deltaR2( e1, p1, e2, p2):
    de = e1 - e2
    dp = deltaPhi(p1, p2)
    return de*de + dp*dp


def deltaR( *args ):
    return math.sqrt( deltaR2(*args) )


def deltaPhi( p1, p2):
    '''Computes delta phi, handling periodic limit conditions.'''
    res = p1 - p2
    while res > math.pi:
        res -= 2*math.pi
    while res < -math.pi:
        res += 2*math.pi
    return res


def cleanObjectCollection2( objects, masks, deltaRMin ):
    '''Masks objects using a deltaR cut, another algorithm (same results).'''
    if len(objects)==0:
        return objects
    deltaR2Min = deltaRMin*deltaRMin
    cleanObjects = copy.copy( objects )
    for mask in masks:
        tooClose = []
        for idx, object in enumerate(cleanObjects):
            dR2 = deltaR2( object.eta(), object.phi(),
                           mask.eta(), mask.phi() )
            if dR2 < deltaR2Min:
                tooClose.append( idx )
        nRemoved = 0
        for idx in tooClose:
            # yes, everytime an object is removed, the list of objects is updated
            # so need to update the index accordingly.
            # example: to remove : ele 1 and 2
            #  first, ele 1 is removed
            #  -> ele 2 is now at index 1
            # one should again remove the element at index 1
            idx -= nRemoved
            del cleanObjects[idx]
            nRemoved += 1 
    return cleanObjects


def cleanObjectCollection( objects, masks, deltaRMin ):
    '''Masks objects using a deltaR cut.'''
    if len(objects)==0:
        return objects
    deltaR2Min = deltaRMin*deltaRMin
    cleanObjects = []
    for object in objects:
        ok = True 
        for mask in masks:
            dR2 = deltaR2( object.eta(), object.phi(),
                           mask.eta(), mask.phi() )
            if dR2 < deltaR2Min:
                ok = False
        if ok:
            cleanObjects.append( object )
    return cleanObjects

if __name__ == '__main__':

    import sys
    args = sys.argv[1:]
    fargs = map( float, args )

    print 'dR2 = ', deltaR2( *fargs )
    print 'dR = ', deltaR( *fargs )
    

    
