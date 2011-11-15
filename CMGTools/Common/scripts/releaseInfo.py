#!/bin/env python

import sys
import subprocess
import operator
import pprint as pp

widerPrinter = pp.PrettyPrinter(width=120, indent=2)
pprint = widerPrinter.pprint

relInfoFileName = 'test.py'


def showtags():
    '''Get showtags information.'''
    showtags = subprocess.Popen( ['showtags'], stdout=subprocess.PIPE).communicate()[0]
    packtags = []
    for line in showtags.split('\n'):
        if line is '':
            continue
        words = line.split()
        if len(words)!=2:
            continue
        tag, package = words
        packtags.append( (package, tag) )
    return sorted(packtags)


def relInfoTags(fileName):
    '''Get tags from a relinfo file'''
    relInfoDict = eval( file(fileName).read() )
    packtags = []
    for cvsmod, tagpack in relInfoDict.iteritems():
        packtags.append( (tagpack[1], tagpack[0]) )
    return sorted(packtags), relInfoDict

localArea = showtags()
print 'local area:'
pprint(localArea)

print '-'*70

relInfo, relInfoDict = relInfoTags( relInfoFileName )
print 'rel info'
pprint(relInfo)


print '-'*70

localAreaSet = set( [ packtag[0] for packtag in localArea ] )
relInfoSet = set( [packtag[0] for packtag in relInfo] )

print 'elements in local area but not in relinfo:'
localOnly = localAreaSet.difference(relInfoSet) 
pprint(localOnly)

print 'elements in relinfo but not in local area:'
relInfoOnly = relInfoSet.difference(localAreaSet)
pprint(relInfoOnly)



new_relInfo = []
localAreaPacks = [ pack for pack, tag in localArea]
for cvsmod, tagpack in relInfoDict.items():
    oldtag = tagpack[0]
    oldpack = tagpack[1]
    #print 'OLD:', oldpack, oldtag
    newtag = None
    for pack, tag in localArea:
        if pack == oldpack:
            newtag = tag
            break
    if newtag is None:
        print 'Package', oldpack, 'not in local area -> removed from relinfo'
        continue
    # print 'NEW', oldpack, newtag
    new_relInfo.append( (oldpack, cvsmod, newtag ) )

# now adding the tags that are in the local area but not in the old release info
for package, tag in localArea:
    if package in localOnly:
        new_relInfo.append( (package, package, tag ) ) 
        
    # print localAreaPacks.get( pack )

pprint( sorted(new_relInfo) )


