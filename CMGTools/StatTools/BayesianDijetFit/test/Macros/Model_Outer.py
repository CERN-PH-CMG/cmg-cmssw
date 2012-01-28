#! /usr/bin/env python 
import os

command = "dijetStatsNewBackground 2000 0 0 Qstar_ak5_fat30" 
print command 
os.system(command)
os.system("cp Resonance_Shapes_Qstar_ak5_fat30.root plots_Qstar_4677pbm1")

command = "dijetStatsNewBackground 2000 0 0 RSGraviton_ak5_GGtoGG_fat30" 
print command 
os.system(command)
os.system("cp Resonance_Shapes_RSGraviton_ak5_GGtoGG_fat30.root plots_GG_4677pbm1")

command = "dijetStatsNewBackground 2000 0 0 RSGraviton_ak5_QQtoQQ_fat30" 
print command 
os.system(command) 
os.system("cp Resonance_Shapes_RSGraviton_ak5_QQtoQQ_fat30.root plots_QQ_4677pbm1")

print 'end' 
