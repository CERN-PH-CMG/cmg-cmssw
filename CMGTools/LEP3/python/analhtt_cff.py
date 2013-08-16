import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg

#fourJetAna = cfg.Analyzer(
#    'FourJetAnalyzer',
#    npfj = (3,5),       # at least 8 PF particle in 3 of the 4 jets
#    ntkj = (4,1),       # at least 1 PF charged hadron in 4 of the 4 jets
#    minM = 0.5,         # No jet with a jet mass smaller than 2.5 GeV
#    mVis = 180.,        # total visible mass in excess of 180 GeV,
#    chi2 = 1000.,       # total chi**2 of the beta4 "fit" smaller than 1000. (i.e., all fit energies > 0.)    
#    pair_mass = 0.,    # No jet pair should have a mass smaller than 30 GeV
#    pair_cijkl = 1000.,  # Tue smallest sum of cos(ij)+cos(kl) should be larger than -1.1
#    pair_sumtet = 0., # The sum of the four smallest jet-jet angles must be larger than 350 degrees
#    h_mass = 105.,       # The Higgs candidate mass should be larger than 90 GeV
#    z_mass = (80.,110.),     # The Z candidate jet pair should have a mass closer to the Z mass than 100 GeV
#    )

httAna = cfg.Analyzer('httanalyzer',
  npfj = (2,3),       # at least 3 PF particle in both the nontau jets
  ntkj = (2,1),       # at least 1 track in both nontau jets
#  minM = 0.5,         # minimum jet mass
  minM = 1.5,         # minimum jet mass
  mVis = 180.,        # minim visible mass (not applied)
  chi2 = 1000000.,
  minE = 10.,
  h_mass = 40.,   # to see the z peak....
  z_mass = (70.,110.),
)

httAnapftau = cfg.Analyzer('httanalyzerpftau',
  npfj = (2,3),       # at least 3 PF particle in both the nontau jets
  ntkj = (2,1),       # at least 1 track in both nontau jets
#  minM = 0.5,         # minimum jet mass
  minM = 1.5,         # minimum jet mass
  mVis = 180.,        # minim visible mass (not applied)
  chi2 = 1000000.,
  minE = 10.,
  h_mass = 40.,   # to see the z peak....
  z_mass = (70.,110.),
)


def createTreeProducer( ana ):
    tp = cfg.Analyzer( '_'.join( ['htttreeproducer','httanalyzer'] ),


                       anaName = 'httanalyzer'
                       )
    return tp

def createTreeProducerpftau( ana ):
    tp = cfg.Analyzer( '_'.join( ['htttreeproducerpftau','httanalyzerpftau'] ),


                       anaName = 'httanalyzerpftau'
                       )
    return tp


# for debugging 
stopper = cfg.Analyzer(
    'Stopper'
    )

sequence_orig = cfg.Sequence([
    httAna,
    createTreeProducer( httAna ),
    ])

sequence_pftau = cfg.Sequence([
    httAnapftau,
    createTreeProducerpftau( httAnapftau ),
    ])

sequence=sequence_pftau
