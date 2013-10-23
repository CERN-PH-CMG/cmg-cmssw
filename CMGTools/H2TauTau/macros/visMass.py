from CMGTools.H2TauTau.macros.h2TauTau import *

aliases = AliasSetter(events, h2TauTau, 'H2TAUTAU')

events.Draw('tauMu.obj.mass()','tauMu.obj.mass()>10')
