# call this script like this:
#
# python testCuts.py 'TauPlusX_V6/Job*/*tree*root'


print 'initializing h->tau tau root environment'

# aliases  ----------------------------------------------------------

# default CMG aliases:
from CMGTools.RootTools.cmgInit import *
# specific aliases for this analysis
from CMGTools.H2TauTau.macros.h2TauTauAliases import *
aliases = AliasSetter(events, h2TauTauAliases, 'H2TAUTAU')

print
print
print 'Testing cut efficiencies...'

print 'number of entries ', events.GetEntries()

def nEntries( sel ):
    cut = 'tauMu.obj.getSelection("%s")' % sel
    events.Draw('>>'+sel,cut)
    num = gDirectory.Get(sel).GetN()
    return (num, num/float(events.GetEntries())*100)

cut = 'cuts_baseline'
print cut, '%d (%3.2f)' % nEntries( cut )

cut = 'cuts_baseline_muLeg_kinematics'
print cut, '%d (%3.2f)' % nEntries( cut )

cut = 'cuts_baseline_muLeg_iso'
print cut, '%d (%3.2f)' % nEntries( cut )

cut = 'cuts_baseline_muLeg_id'
print cut, '%d (%3.2f)' % nEntries( cut )

cut = 'cuts_baseline_muLeg'
print cut, '%d (%3.2f)' % nEntries( cut )


