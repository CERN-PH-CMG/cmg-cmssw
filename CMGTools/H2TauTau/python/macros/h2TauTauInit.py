# call me like this:
#
#  python -i h2TauTauInit.py 'TauPlusX_V6/Job*/*tree*root' TauPlusX_V6
#

from CMGTools.RootTools.cmgInit import *
from CMGTools.H2TauTau.macros.h2TauTauAliases import *

print 'initializing h->tau tau root environment'

# aliases  ----------------------------------------------------------

# default CMG aliases:
# specific aliases for this analysis

def h2TauTauInit(pattern):
    (events,lumi) = cmgInit(pattern)
    aliases = AliasSetter(events, h2TauTauAliases, 'H2TAUTAU')
    return events,lumi

# plotting ----------------------------------------------------------

# This file contains some default histograms, and shows how
# python classes can be used to manage histograms 
from CMGTools.H2TauTau.macros.H2TauTauHistograms import *

# the first argument is the pattern of input root files,see cmgInit.py
# the second argument is the name of your histograms, e.g. data, signal, WJets, whatever
# the output root file will start with this name.
## allHists = H2TauTauHistograms( sys.argv[2] )

## nEvents = 999999
## allHists.fillHistos( events, 'tauMu.obj.getSelection("cuts_baseline")',nEvents)

## gDirectory.ls()

## # styles are defined in CMGTools.RootTools.Style
## allHists.formatHistos( sBlack )
## allHists.diTau.h_vismass.Draw()
## allHists.Write()

