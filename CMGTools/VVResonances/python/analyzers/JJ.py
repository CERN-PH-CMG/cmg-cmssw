from CMGTools.VVResonances.analyzers.EventInterpretationBase import *
from CMGTools.VVResonances.tools.Pair import Pair
import itertools
class JJ( EventInterpretationBase ):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(JJ,self).__init__(cfg_ana, cfg_comp, looperName)

    def process(self, event):
        super(JJ,self).process(event)

        output=[]

        if self.cfg_ana.doCHS:
            cleanedPackedCandidates = filter(lambda x: x.fromPV(0) ,event.packedCandidatesForJets)
        else:
            cleanedPackedCandidates = event.packedCandidatesForJets    

        #create Fat Jets 
        selectedFatJets = self.makeFatJets(cleanedPackedCandidates)
        if self.isMC:
            self.matchSubJets(selectedFatJets,event.genwzquarks)
            
            for j1,j2 in itertools.combinations(selectedFatJets,2):
                if j1.softDropJet.mass()<j2.softDropJet.mass():
                    VV=Pair(j1,j2)
                else:
                    VV=Pair(j2,j1)

                if self.selectPair(VV):
                    selected = {'pair':VV}
                    remainingCands =self.removeJetFootPrint([j1,j2],event.packedCandidatesForJets)
                    selected['satelliteJets']=self.makeSatelliteJets(remainingCands)
                    #add VBF info
                    self.vbfTopology(selected)
                    output.append(selected)                   
        if len(output)>0:
            output = sorted(output,key = lambda x: x['pair'].leg1.pt()+x['pair'].leg2.pt(),reverse=True)
        setattr(event,'JJ'+self.cfg_ana.suffix,output)
        return True
