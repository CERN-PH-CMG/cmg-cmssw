#! /usr/bin/env python 
import os

#command = "dijetStatsNewBackground 2000 0 0 Qstar_ak5_fat30 0"
command = "dijetStatsNewBackground 600 0 0 RSGraviton_HLT_ak5_QQtoQQ_pf 0" 
print command 
os.system(command)


print 'end' 
