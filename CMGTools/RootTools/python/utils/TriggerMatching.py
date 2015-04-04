import fnmatch
import math
from PhysicsTools.HeppyCore.utils.deltar import deltaR2

def triggerMatched( object, triggerObjects, path, filter,
                    dR2Max=0.089999, dRMax=0., pdgIds=None, index=False):
    '''The default dR2Max is 0.3^2'''
    eta = object.eta()
    phi = object.phi()
    # to speed up the code, could sort the triggerObjects by decreasing pT
    # when they are produced
    for i, trigObj in enumerate(triggerObjects):
        if trigObj.hasPath( path )        and \
           trigObj.hasSelection( filter ) and \
           dRMax == 0.                    and \
           deltaR2( eta, phi, trigObj.eta(), trigObj.phi() ) < dR2Max:
            if pdgIds is None or abs(trigObj.pdgId()) in pdgIds:
                if index : return True, i
                else     : return True
        if trigObj.hasPath( path )        and \
           trigObj.hasSelection( filter ) and \
           dRMax > 0.                     and \
           math.sqrt(deltaR2( eta, phi, trigObj.eta(), trigObj.phi() )) < dRMax:
            if pdgIds is None or abs(trigObj.pdgId()) in pdgIds:
                if index : return True, i
                else     : return True

    if index : return False, -99
    else     : return False


def selTriggerObjects( trigObjs, path, filter=None, skipPath=False):
    '''Selects trigger objects used in path, and passing filter.
    If filter is None, only checks path. path can contain a wildcard.'''
    selObjs = []
    for trigObj in trigObjs:
        #        if not trigObj.hasSelection( path ): continue
        if not (trigObj.hasPath( path ) or skipPath) : continue
        if filter is not None and \
           not trigObj.hasSelection( filter ): continue
        selObjs.append( trigObj )
    return selObjs


## def hasPath( trigObj, path ):
##     '''Returns true if trigObj was used in path (path can contain a wildcard).
##     '''
##     selNames = fnmatch.filter( trigObj.getSelectionNames(), path )
##     if len(selNames)>0:
##         return True
##     else:
##         return False
    

    
