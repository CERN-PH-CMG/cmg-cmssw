
import math

class CMSModel(object):

    def electronEfficiency(self, ptc):
        pt = ptc.p4().pt()
        eta = ptc.p4().eta()
        effvalue = 0.
        if pt>5. and abs(eta)<3.:
            effvalue = 0.95
        return effvalue

    def muonEfficiency(self, ptc):
        pt = ptc.p4().pt()
        eta = ptc.p4().eta()
        effvalue = 0.
        if pt>5. and abs(eta)<2.4:
            effvalue = 1.0
        return effvalue

    def electronResolution(self, ptc):
        energy = ptc.p4().energy()
        resolution = 0.03 / math.sqrt(energy)
        return resolution

    def muonResolution(self, ptc):
        pt = ptc.p4().pt()
        resolution = 0.002
        return resolution

    def jetEfficiency(self, ptc):
        return 1.0

    def jetResolution(self, ptc):
        energy = ptc.p4().energy()
        resolution = 0.6 / math.sqrt(energy)
        return resolution


CMS = CMSModel()
