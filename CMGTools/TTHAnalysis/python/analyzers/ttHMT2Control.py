import operator
import itertools
import copy
import types

from math import *

from ROOT import TLorentzVector

from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.fwlite.Event import Event
from CMGTools.RootTools.statistics.Counter import Counter, Counters
from CMGTools.RootTools.fwlite.AutoHandle import AutoHandle

#from CMGTools.RootTools.physicsobjects.Photon import Photon
#from CMGTools.RootTools.physicsobjects.Electron import Electron
#from CMGTools.RootTools.physicsobjects.Muon import Muon

from CMGTools.RootTools.utils.DeltaR import *


class ttHMT2Control( Analyzer ):


    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(ttHMT2Control,self).__init__(cfg_ana,cfg_comp,looperName)



    def makeGammaObjects(self, event):

        import ROOT

        ## with Central Jets                                                                                                                                                                                                      
        gamma_objects25 = [ j for j in event.gamma_cleanJets if j.pt() > 25 ] + event.selectedLeptons
        gamma_objects30 = [ j for j in event.gamma_cleanJets if j.pt() > 30 ] + event.selectedLeptons
        gamma_objects40  = [ j for j in event.gamma_cleanJetsAll if j.pt() > 40 and abs(j.eta()) < 2.5 ] + [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]
        gamma_objects40j = [ j for j in event.gamma_cleanJets if j.pt() > 40 ]

        event.gamma_htJet25 = sum([x.pt() for x in gamma_objects25])
        event.gamma_mhtJet25vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects25])) , -1.*(sum([x.py() for x in gamma_objects25])), 0, 0 )
        event.gamma_mhtPhiJet25 = event.mhtJet25vec.phi()
        event.gamma_mhtJet25 = event.mhtJet25vec.pt()

        event.gamma_htJet30 = sum([x.pt() for x in gamma_objects30])
        event.gamma_mhtJet30vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects30])) , -1.*(sum([x.py() for x in gamma_objects30])), 0, 0 )
        event.gamma_mhtJet30 = event.mhtJet30vec.pt()
        event.gamma_mhtPhiJet30 = event.mhtJet30vec.phi()

        event.gamma_htJet40 = sum([x.pt() for x in gamma_objects40])
        event.gamma_mhtJet40vec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects40])) , -1.*(sum([x.py() for x in gamma_objects40])), 0, 0 )
        event.gamma_mhtJet40 = event.gamma_mhtJet40vec.pt()
        event.gamma_mhtPhiJet40 = event.gamma_mhtJet40vec.phi()

        event.gamma_htJet40j = sum([x.pt() for x in gamma_objects40j])
        event.gamma_mhtJet40jvec = ROOT.reco.Particle.LorentzVector(-1.*(sum([x.px() for x in gamma_objects40j])) , -1.*(sum([x.py() for x in gamma_objects40j])), 0, 0 )
        event.gamma_mhtJet40j = event.gamma_mhtJet40jvec.pt()
        event.gamma_mhtPhiJet40j = event.gamma_mhtJet40jvec.phi()

         # MET + photon                                                                                                                                                                                                           
        event.gamma_met = ROOT.reco.Particle.LorentzVector( event.met.px(), event.met.py(), 0, 0 )
        event.gamma_metNoPU = ROOT.reco.Particle.LorentzVector( event.metNoPU.px(), event.metNoPU.py(), 0, 0 )
        for gamma in event.selectedPhotonsCentral:
            event.gamma_met = ROOT.reco.Particle.LorentzVector( event.gamma_met.px() + gamma.px(), event.gamma_met.py() + gamma.py() , 0, 0 )
            event.gamma_metNoPU = ROOT.reco.Particle.LorentzVector( event.gamma_metNoPU.px() + gamma.px(), event.gamma_metNoPU.py() + gamma.py() , 0, 0 )

        # look for minimal deltaPhi between MET and four leading jets with pt>40 and eta<2.4                                                                                                                                      
        event.gamma_deltaPhiMin = 999.
        for n,j in enumerate(gamma_objects40j):
            if n>3:  break
            thisDeltaPhi = abs( deltaPhi( j.phi(), event.gamma_met.phi() ) )
            if thisDeltaPhi < event.gamma_deltaPhiMin : event.gamma_deltaPhiMin = thisDeltaPhi

        # absolute value of the vectorial difference between met and mht                                                                                                                                                          
        gamma_diffMetMht_had_vec = ROOT.reco.Particle.LorentzVector(event.gamma_mhtJet40jvec.px()-event.gamma_met.px(), event.gamma_mhtJet40jvec.py()-event.gamma_met.py(), 0, 0 )
        event.gamma_diffMetMht_had = sqrt( gamma_diffMetMht_had_vec.px()*gamma_diffMetMht_had_vec.px() + gamma_diffMetMht_had_vec.py()*gamma_diffMetMht_had_vec.py() )

        gamma_diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.gamma_mhtJet40vec.px()-event.gamma_met.px(), event.gamma_mhtJet40vec.py()-event.gamma_met.py(), 0, 0 )
        event.gamma_diffMetMht = sqrt( gamma_diffMetMht_vec.px()*gamma_diffMetMht_vec.px() + gamma_diffMetMht_vec.py()*gamma_diffMetMht_vec.py() )



    def makeZllObjects(self, event):

        import ROOT

        vetoLeptons = [ l for l in event.selectedLeptons if l.pt() > 10 and abs(l.eta()) < 2.5 ]

        # MET + zll                                                                                                                                                                                                               
        event.zll_deltaPhiMin = -999.
        event.zll_met_pt = -999.
        event.zll_met_phi = -999.
        event.zll_diffMetMht = -999.
        event.zll_mhtJet40j = -999.
        event.zll_mhtPhiJet40j = -999.
        event.zll_invmass = -999.

        if len(vetoLeptons)==2:
            event.zll_met = ROOT.reco.Particle.LorentzVector( event.met.px(), event.met.py(), 0, 0 )
            for l in vetoLeptons:
                event.zll_met = ROOT.reco.Particle.LorentzVector( event.zll_met.px() + l.px(), event.zll_met.py() + l.py() , 0, 0 )

            event.zll_met_pt = event.zll_met.pt()
            event.zll_met_phi = event.zll_met.phi()

            # defining mht as hadronic mht                                                                                                                                                                                        
            event.zll_mhtJet40j = event.mhtJet40j
            event.zll_mhtPhiJet40j = event.mhtPhiJet40j

            # look for minimal deltaPhi between MET and four leading jets with pt>40 and |eta|<2.4                                                                                                                                
            event.zll_deltaPhiMin = 999.
            objects40jc = [ j for j in event.cleanJets if j.pt() > 40 and abs(j.eta())<2.5 ]
            for n,j in enumerate(objects40jc):
                if n>3:  break
                thisDeltaPhi = abs( deltaPhi( j.phi(), event.zll_met.phi() ) )
                if thisDeltaPhi < event.zll_deltaPhiMin : event.zll_deltaPhiMin = thisDeltaPhi

            # absolute value of the vectorial difference between met and mht                                                                                                                                                      
            zll_diffMetMht_vec = ROOT.reco.Particle.LorentzVector(event.mhtJet40jvec.px()-event.zll_met.px(), event.mhtJet40jvec.py()-event.zll_met.py(), 0, 0 )
            event.zll_diffMetMht = sqrt( zll_diffMetMht_vec.px()*zll_diffMetMht_vec.px() + zll_diffMetMht_vec.py()*zll_diffMetMht_vec.py() )

            # di-lepton invariant mass                                                                                                                                                                                            
            zll_p4 = ROOT.reco.Particle.LorentzVector( 0, 0, 0, 0 )
            for l in vetoLeptons:
                zll_p4 += l.p4()

            event.zll_invmass = zll_p4.M()



    def process(self, iEvent, event):
        self.readCollections( iEvent )
        self.makeGammaObjects(event)                                                                                                                                                                                             
        self.makeZllObjects(event)

        return True
