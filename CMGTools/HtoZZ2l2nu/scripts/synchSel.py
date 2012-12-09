#!/usr/bin/python

import sys

def parseFile(url) :
    ev_dict={}
    for l in open(url, "r").readlines() :
        f = [int(float(a)) for a in l.split()]
        if(len(f)<3) : continue
        ev_dict[f[0]+f[1]+f[2]]=f
    return ev_dict

if(len(sys.argv)<3):
    print 'synchSel.py file_A file_B'
    print 'Prints differences found between the two selection files'
    sys.exit(-1)
    
logA=parseFile(sys.argv[1])
logB=parseFile(sys.argv[2])

report='[Differences found: Event Lumi Run < A=%s > B=%s]\n'%(sys.argv[1],sys.argv[2])

#missing runs in B + differences in selection
for i in logA.items() :

    key=i[0]
    try:
        ilogB=logB[key]
        for n in xrange(3,len(ilogB)) :
            if(ilogB[n]==i[1][n]) : continue
            report += '%d %d %d < %d > %d @ field=%d\n'%(i[1][0],i[1][1],i[1][2],i[1][n],ilogB[n],n-3)
            break
    except:
        report += '%d %d %d            > missing \n'%(i[1][0],i[1][1],i[1][2])

#missing runs in A
for i in logB.items() :
    key=i[0]
    try:
        logA[key]
    except:
        report += '%d %d %d < missing \n'%(i[1][0],i[1][1],i[1][2])

print '\n%s \n'%report
