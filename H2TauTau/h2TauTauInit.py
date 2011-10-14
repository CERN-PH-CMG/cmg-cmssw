# to initialize your environment, make sure you have defined the following allias:
# for bash: 
# alias h2TauTauInit='python -i $CMSSW_BASE/src/CMGTools/H2TauTau/h2TauTauInit.py'
# then, do:
# cmgInit 'dir1/your_rootfiles*.root'
# and then:
#

print 'initializing h->tau tau root environment'

# aliases
from CMGTools.RootTools.cmgInit import *
from CMGTools.H2TauTau.macros.h2TauTau import *

aliases = AliasSetter(events, h2TauTau, 'H2TAUTAU')
