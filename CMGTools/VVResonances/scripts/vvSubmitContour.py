#!/usr/bin/env python

import ROOT
import os, sys, re, optparse,pickle,shutil,json




parser = optparse.OptionParser()

parser.add_option("-m","--mass",dest="mass",type=float,help="mass",default=1800.0)

parser.add_option("-n","--points",dest="points",type=int,help="points",default=10000)
parser.add_option("-N","--pointsPerJob",dest="perJob",type=int,help="points per job",default=100)

parser.add_option("-o","--options",dest="options",help="Combine Options",default='')
parser.add_option("-q","--queue",dest="queue",help="Batch Queue",default='8nh')
(options,args) = parser.parse_args()



STEPS = int(options.points/options.perJob)


points=[]

for i in range(0,STEPS+1):
    points.append(i*options.perJob)


for i in range(0,len(points)-1):

    mini=points[i]
    maxi=points[i+1]

    f=open("submitContour_{i}.sh".format(i=i),'w')
    execScript = 'cd {cwd} \n'.format(cwd=os.getcwd())
    execScript += 'eval `scramv1 runtime -sh` \n'
    execScript += "combine -m {mass} -M MultiDimFit --algo=grid {options} --points={points} --firstPoint={mini} --lastPoint={maxi} {file}\n".format(points=options.points,mass=options.mass,options=options.options,file=args[0],mini=mini,maxi=maxi)
    f.write(execScript)
    f.close()
    os.system('chmod +x submitContour_{i}.sh'.format(i=i))

    if options.queue!="local":
        os.system('bsub -q {queue} submitContour_{i}.sh '.format(queue=options.queue,i=i))
    else:    
        os.system('sh submitContour_{i}.sh '.format(i=i))





