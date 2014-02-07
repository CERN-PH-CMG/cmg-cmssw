#!/bin/env python

import sys
import subprocess
import operator
import pprint as pp

widerPrinter = pp.PrettyPrinter(width=120, indent=2)
pprint = widerPrinter.pprint



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
    '''Get tags from a relinfo file.

    The return tuple is not very nice...
    Will need to be updated anyway when the conversion to the new format is complete'''
    relInfo = eval( file(fileName).read() )
    packtags = []
    for package, cvsmod, tag in relInfo:
        packtags.append( (package, cvsmod, tag) )
    return sorted(packtags)



def relInfo( fileName, verbose = False):

    sep_line = '-'*70
    
    # getting the package, tag list from showtags
    localArea = showtags()
    if verbose:
        print 'local area:'
        pprint(localArea)
        print sep_line

    # get the release_info (to be updated)
    relInfo = relInfoTags( relInfoFileName )
    if verbose:
        print 'rel info'
        pprint(relInfo)
        print sep_line

    # find the tags that are in the local area but not in the release info
    localAreaSet = set( [ packtag[0] for packtag in localArea ] )
    relInfoSet = set( [packtag[0] for packtag in relInfo] )
    localOnly = localAreaSet.difference(relInfoSet) 
    relInfoOnly = relInfoSet.difference(localAreaSet)
    if verbose:
        print 'elements in local area but not in relinfo:'
        pprint(localOnly)
        print 
        print 'elements in relinfo but not in local area:'
        pprint(relInfoOnly)
        print sep_line
        
    # build the new release_info.
    # we start from the information from the old one, which
    # knows about the cvs module to use for each package
    new_relInfo = []
    localAreaPacks = [ pack for pack, tag in localArea]
    for oldpack, cvsmod, oldtag in relInfo:
        #print 'OLD:', oldpack, oldtag
        newtag = None
        for pack, tag in localArea:
            if pack == oldpack:
                newtag = tag
                break
        if newtag is None:
            print '# Package', oldpack, 'not in local area -> removed from relinfo'
            continue
        # print 'NEW', oldpack, newtag
        new_relInfo.append( (oldpack, cvsmod, newtag ) )

    # now adding the tags that are in the local area
    # but not in the old release info.
    # for these ones, we just use the package name as cvs module.
    added = []
    for package, tag in localArea:
        if package in localOnly:
            new_relInfo.append( (package, package, tag ) )
            added.append( (package, tag) )

    pprint( sorted(new_relInfo) )

    if len(added)>0:
        print '# The following packages were not in the input release_info.'
        print '# Please make sure that the CVS module is correct!'
        for package, tag in added:
            print '#\t', package.ljust(40), tag 
    
if __name__ == '__main__':

    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog  <release_info_file>
    Prints a version of the release_info updated with the information from the local area.
    """
    parser.add_option("-v", "--verbose", dest="verbose", default=False,
                      help='verbose')

    (options,args) = parser.parse_args()

    if len(args)!=1:
        print 'please supply a release_info file.'
        sys.exit(1)

    relInfoFileName = args[0]
    relInfo( relInfoFileName, options.verbose ) 

