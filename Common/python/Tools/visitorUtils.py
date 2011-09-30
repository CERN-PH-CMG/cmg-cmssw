import FWCore.ParameterSet.Config as cms

import re

# This module provides some visitors for handling postfixes in PF2PAT
# @author wreece

class InputTagVisitor(object):
    """Iterates over the tree and finds any parameters that must be updated"""

    def processInputTag(self, input):
        """hook for subclasses"""
        return input

    def _params(self,objectLabel,params):
        
        def input(n,v):
            if isinstance(v,cms.InputTag):
                v = self.processInputTag(v)
            elif isinstance(v,cms.VInputTag) or isinstance(v,cms.VPSet):
                for i in xrange(len(v)):
                    v[i] = input(n,v[i])
            elif isinstance(v,cms.PSet):
                for nn,vv in v.parameters_().iteritems():
                    setattr(v,nn,input(nn,vv))
            return v

        for name, value in params.iteritems():
            params[name] = input(name,value)
        return params
            
    def enter(self,v):

        # print 'enter ', v
        
        if ( isinstance(v,cms.EDFilter) or
             isinstance(v,cms.EDProducer) or
             isinstance(v,cms.EDAnalyzer)):

            # print 'is of the good type'
            
            p = self._params(v.label(),v.parameters_())
            #set the parameters back on the object
            for name, value in p.iteritems():
                setattr(v,name,value)

                
    def leave(self,v):
        pass


class InputTagPostfixVisitor(InputTagVisitor):
    """
Looks at the 'moduleLabel' of every InputTag.
It it ends with 'old_postfix', it replaces it with 'new_postfix.   
    """
    def __init__(self, old_postfix, new_postfix):
        InputTagVisitor.__init__(self)
        
        self.old_postfix = old_postfix
        self.new_postfix = new_postfix
        self.pattern = re.compile( '(.*)(%s)$' % old_postfix)
        
    def processInputTag(self, v):
        label = v.getModuleLabel()
        if label:
            match = self.pattern.match( label )
            # print label, self.pattern, '(%s)$' % self.old_postfix
            if match:
                # print 'match', label, match.group(1), match.group(2)
                new = match.group(1)+self.new_postfix
                # print new
                v.setModuleLabel(new)
                # the logic below was bugged, could replace
                # AK5LCCMG by AK5LC, when asking to replace AK5 by AK5LC
                # index = label.rfind(self.old_postfix)
                # if index > -1:
                #    new = '%s%s' % (label[0:index],self.new_postfix)
                #    v.setModuleLabel(new)
        return v

def replacePostfix(sequencable, old_postfix, new_postfix):
    '''Utility function to run the InputTagPostfixVisitor visitor'''
    
    v = InputTagPostfixVisitor(old_postfix, new_postfix)
    sequencable.visit(v)
    return sequencable

if __name__ == '__main__':
    from PhysicsTools.PatAlgos.patTemplate_cfg import *
    
    process.cmgElectron  = cms.EDFilter("ElectronPOProducer",
    cfg = cms.PSet(
        leptonFactory = cms.PSet(
            vertexType = cms.int32(1),
            photonsIsoPar = cms.PSet(
                coneSize = cms.double(0.4),
                vetoes = cms.VPSet(cms.PSet(
                    dEta = cms.double(0.1),
                    dPhi = cms.double(0.2),
                    type = cms.string('RectangularEtaPhiVeto')
                ))
            ),
            useIsoDeposits = cms.bool(False),
            useParticleFlowIso = cms.bool(True),
            chargedHadronIsoPar = cms.PSet(
                coneSize = cms.double(0.4),
                vetoes = cms.VPSet()
            ),
            vertexCollection = cms.InputTag("offlineBeamSpot"),
            neutralHadronIsoPar = cms.PSet(
                coneSize = cms.double(0.4),
                vetoes = cms.VPSet()
            )
        ),
        inputCollection = cms.InputTag("selectedPatElectronsPFlow")
    ),
    cuts = cms.PSet(
        isEB = cms.string('sourcePtr().isEB()'),
        isEE = cms.string('sourcePtr().isEE()'),
        ecalDriven = cms.string('ecalDriven() == 1')
    )
)
    
    process.seq = cms.Sequence(process.cmgElectron)
    
    print 'Before:',process.cmgElectron.cfg.inputCollection
    replacePostfix(process.seq,'PFlow','Foo')
    print 'After:',process.cmgElectron.cfg.inputCollection


            
