print 'initializing h->tau tau root environment'

# aliases
from CMGTools.RootTools.cmgInit import *
from CMGTools.H2TauTau.macros.h2TauTau import *

aliases = AliasSetter(events, h2TauTau, 'H2TAUTAU')

events.Draw('tauMu.obj.mass()>>hWelcome','tauMu.obj.getSelection("cuts_baseline")','',10000)
events.GetHistogram().SetTitle('Welcome to the H#rightarrow#tau#tau analysis!')
gPad.Modified()
