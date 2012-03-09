from CMGTools.RootTools.utils.DeltaR import deltaR2


def triggerMatched( object, triggerObjects, path, filter, dR2Max=0.089999):
    '''The default dR2Max is 0.3^2'''
    eta = object.eta()
    phi = object.phi()
    # to speed up the code, could sort the triggerObjects by decreasing pT
    # when they are produced
    for trigObj in triggerObjects:
        if trigObj.hasSelection( path ) and \
               trigObj.hasSelection( filter ) and \
               deltaR2( eta, phi, trigObj.eta(), trigObj.phi() ) < dR2Max:
            return True
    return False


def selTriggerObjects( trigObjs, path, filter=None):
    selObjs = []
    for trigObj in trigObjs:
        if not trigObj.hasSelection( path ): continue
        if filter is not None and \
           not trigObj.hasSelection( filter ): continue
        selObjs.append( trigObj )
    return selObjs
   
