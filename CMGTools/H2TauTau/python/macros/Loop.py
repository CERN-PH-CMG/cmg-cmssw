import os
import sys
import pprint

from CMGTools.H2TauTau.macros.H2TauTauInit import *

from DataFormats.FWLite import Events, Handle

pattern = sys.argv[1:]

print 'running on: '
pprint.pprint(pattern)

events = Events( pattern )


for i,event in enumerate(events):
    if i%100 ==0:
        print 'event', i
        
    diTauHandle = Handle('std::vector<cmg::DiObject<cmg::Tau,cmg::Muon>>')
    event.getByLabel('cmgTauMuBaselineSel', diTauHandle)
    diTaus = diTauHandle.product()
