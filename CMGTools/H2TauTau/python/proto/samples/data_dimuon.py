import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg

# total 5.05 from Cris, using Nov08 re-reco JSON.
# I'm now using the PR JSON...

totLum = 5050

data_Run2011A = cfg.DataComponent(
    name = 'data_Run2011A',
    files = [],
    intLumi = totLum / 2., # just sharing the total lumi arbitrarily
    triggers = [],
    json = None
    )

data_Run2011B = cfg.DataComponent(
    name = 'data_Run2011B',
    files = [],
    intLumi = totLum / 2.,
    triggers = [],
    json = None
    )



data_list_2011 = [data_Run2011A, data_Run2011B]
