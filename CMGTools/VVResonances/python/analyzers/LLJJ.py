from CMGTools.VVResonances.analyzers.EventInterpretationBase import *
from CMGTools.VVResonances.tools.Pair import Pair

class LLJJ( EventInterpretationBase ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(LLJJ,self).__init__(cfg_ana, cfg_comp, looperName)

    def process(self, event):
        super(LLJJ,self).process(event)

        output=[]
        #read the W
        for w in event.LL:
            leptons = [w.leg1,w.leg2]

            cleanedPackedCandidates = self.removeLeptonFootPrint(leptons,event.packedCandidatesForJets)

            if self.cfg_ana.doCHS:
                cleanedPackedCandidates = filter(lambda x: x.fromPV(0) ,cleanedPackedCandidates)

            #apply selections
            selectedFatJets = self.makeFatJets(cleanedPackedCandidates)
            if self.isMC:
                self.matchSubJets(selectedFatJets,event.genwzquarks)

            for fat in selectedFatJets:
                VV = Pair(w,fat)
                if self.selectPair(VV):
                    selected = {'pair':VV}
                    remainingCands =self.removeJetFootPrint([fat],cleanedPackedCandidates)
                    selected['satelliteJets']=self.makeSatelliteJets(remainingCands)
                    #add VBF info
                    self.vbfTopology(selected)
                    output.append(selected)                   
#                    import pdb;pdb.set_trace()
        if len(output)>0:
            output = sorted(output,key = lambda x: x['pair'].leg2.pt(),reverse=True)
#            print 'Masses',output[0]['pair'].p4().M(),(output[0]['pair'].leg1.alternateLV+output[0]['pair'].leg2.p4()).M() , 'Delta',abs(output[0]['pair'].leg1.pz()+output[0]['pair'].leg2.pz()),abs(output[0]['pair'].leg1.alternateLV.pz()+output[0]['pair'].leg2.pz())
        setattr(event,'LLJJ'+self.cfg_ana.suffix,output)
        return True
