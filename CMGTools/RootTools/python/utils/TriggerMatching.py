from CMGTools.RootTools.utils.DeltaR import deltaR2


def triggerMatched( object, triggerObjects, path, filter, dR2Max=0.089999):
    '''The default dR2Max is 0.3^2'''
    eta = object.eta()
    phi = object.phi()
    # to speed up the code, could sort the triggerObjects by decreasing pT
    # when they are produced
    for trigObj in triggerObjects:
        # import pdb; pdb.set_trace()
        if trigObj.hasSelection( path ) and \
               trigObj.hasSelection( filter ) and \
               deltaR2( eta, phi, trigObj.eta(), trigObj.phi() ) < dR2Max:
            return True
    return False

    
