import math
from ROOT import TLorentzVector       
from CMGTools.RootTools.utils.DeltaR import deltaPhi

class VBF( object ):
    '''Computes and holds VBF quantities'''
    def __init__(self, jets, diLepton):
        '''jets: jets cleaned from the diLepton legs.
        diLepton: the di-tau, for example. Necessary to compute input variables for MVA selection
        '''
        self.jets = jets
        # the MET is taken from the di-lepton, because it can depend on it
        # e.g. recoil corrections, mva met
        self.met = diLepton.met
        self.leadJets = jets[:2] # taking first 2 jets (leading ones)
        self.otherJets = jets[2:]
        self.centralJets = self.findCentralJets( self.leadJets, self.otherJets )

        # mass of the di-jet system
        self.mjj = self.calcP4( self.leadJets ).M()
        # delta eta
        self.deta = self.leadJets[0].eta() - self.leadJets[1].eta()

        # below, the variables for the MVA selection
        # delta phi
        self.dphi = deltaPhi(self.leadJets[0].phi(), self.leadJets[1].phi())
        dijetp4 = self.leadJets[0].p4() + self.leadJets[1].p4()
        # pt of di-jet system
        self.dijetpt = dijetp4.pt()
        # phi of di-jet system
        self.dijetphi = dijetp4.phi()
        # higgs momentum (defined as the di-lepton momentum + the met momentum)
        # don't access longitudinal quantities!
        self.higgsp4 = diLepton.p4() + met.p4()
        # delta phi between dijet system and higgs system
        self.dphidijethiggs = deltaPhi( self.dijetphi, self.higgsp4.phi() )
        # ? 
        self.visjeteta = -99
        # visible higgs pt = di-lepton pt
        self.ptvis = diLepton.pt()
        
    def findCentralJets( self, leadJets, otherJets ):
        '''Finds all jets between the 2 leading jets, for central jet veto.'''
        etamin = leadJets[0].eta()
        etamax = leadJets[1].eta()
        if etamin > etamax:
            etamin, etamax = etamax, etamin
        def isCentral( jet ):
            #COLIN: shouln't I take a margin? 
            eta = jet.eta()
            if etamin < eta and eta < etamax:
                return True
            else:
                return False
        centralJets = filter( isCentral, otherJets )
        return centralJets

    def calcP4(self, jets):
        '''returns the sum p4 of a collection of objects.
        FIXME: remove this function, which is a bit stupid
        '''
        p4 = TLorentzVector()
        for jet in jets:
            p4 += TLorentzVector(jet.px(), jet.py(), jet.pz(), jet.energy())
        return p4

    def __str__(self):
        header = 'VBF : deta={deta:4.2f}, Mjj={mjj:4.2f}, #centjets={ncentjets}'
        header = header.format( deta=self.deta, mjj=self.mjj, ncentjets=len(self.centralJets))
        leadJets = map( str, self.leadJets )
        centralJets = map( str, self.centralJets)
        tmp = [header]
        tmp.append('MVA input variables: dphi={dphi:4.2f}, dijetpt={dijetpt:4.2f}, dijetphi={dijetphi:4.2f}, dphidijethiggs={dphidijethiggs:4.2f}, visjeteta={visjeteta:4.2f}, ptvis={ptvis:4.2f}'.format(
            dphi = self.dphi,
            dijetpt = self.dijetpt,
            dijetphi = self.dijetphi,
            dphidijethiggs = self.dphidijethiggs,
            visjeteta = self.visjeteta,
            ptvis = self.ptvis
            ))
        tmp.append('Leading Jets:')
        tmp.extend( leadJets )
        tmp.append('Central Jets:')
        tmp.extend( centralJets )
        return '\n'.join( tmp )
