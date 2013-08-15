# /usr/bin/env python 
import os

masses = [800, 900, 1000, 1200, 1300, 1500]
statlevels = [6]
fits = [0]
yields = [1]
negativeSignals = [1]
nIterations = [30]
numPEs = 0

for fit in fits:

    for Yield in yields:

        for negativeSignal in negativeSignals:

            for nIteration in nIterations:

                for statlevel in statlevels:

                    for mass in masses:

                        command = "TTbarStats " + str(mass) + " " + str(statlevel) + " " + str(numPEs) + " 0 " + str(fit) + " " + str(Yield) + " " + str(negativeSignal) + " " + str(nIteration)
                        print command 
                        os.system(command) 



print 'end' 



