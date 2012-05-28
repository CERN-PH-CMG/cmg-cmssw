#!/bin/env python

import time
import sys
import os
import copy
from CMGTools.RootTools.RootInit import *
from CMGTools.RootTools.Style import sBlack, sBlue, styleSet
from ROOT import TTree


from optparse import OptionParser

parser = OptionParser()
parser.usage = '%prog <list of files>'

parser.add_option("-m", "--min",
                  dest="ymin",
                  default=0.0,help='y min')
parser.add_option("-M", "--max",
                  dest="ymax",
                  default=1.1,help='y max')

options, args = parser.parse_args()
if len(args)<2:
    print 'provide at least 2 arguments (the files containing the trees you want to compare)'
    sys.exit(1)

options.ymin = float(options.ymin)
options.ymax = float(options.ymax)

files = args



styles = [sBlue, sBlack]

keeper = []

def getTree( fileName ):
    print 'setup', fileName
    if not os.path.isfile(fileName):
        raise ValueError(fileName + 'does not exist')
    file = TFile( fileName)
    keeper.extend( [file] )
    trees = []
    for k in file.GetListOfKeys():
        obj = file.Get(k.GetName())
        if isinstance(obj,TTree):
            trees.append( obj )
    if len(trees)==0:
        print 'no tree in file', fileName
        sys.exit(1)
    elif len(trees)>1:
        print 'too many trees in file', fileName
        sys.exit(1)
    print 'loading', obj.GetName(), 'from', file.GetName()
    return trees[0]

trees = [] 
for file in files:
    trees.append( getTree(file) )


def draw(var, cut='', norm=False, rebin=None, nbins=None, xmin=None, xmax=None ):
    same = ''
    if nbins is not None:
        if xmin is None or xmax is None:
            raise ValueError('if you specify nbins, you also should specify xmin and xmax')
    for index, tree in enumerate(trees):
        theVar = var
        if same == '' and nbins:
            theVar = '{var}>>{hist}({nbins},{min},{max})'.format(
                var = var, 
                hist = 'hist',
                nbins = nbins,
                min = min,
                max = max)
        tree.Draw(theVar, cut, same)
        hist = tree.GetHistogram()
        print tree.GetName(), hist.Integral()
        if same == '':
            hist.Sumw2()
        if norm:
            hist.Scale( 1/hist.Integral() )
            hist.GetYaxis().SetRangeUser(0,1)
        if rebin is not None and same=='':
            hist.Rebin(rebin)
        if index < len(styles):
            styles[index].formatHisto( hist )
        hist.Draw(same)
        if same == '':
            same = 'same'
        

areFriends = False

def friendAlias(index):
    return 'T{index}'.format(index=index)

def makeFriends():
    entries = map( TTree.GetEntries, trees )
    sameNumEntries = all( x==entries[0] for x in entries )
    if not sameNumEntries:
        print 'cannot call this function if your trees have different numbers of entries'
        return None
    mtree = trees[0]
    for index, tree in enumerate(trees):
        if tree == mtree: continue
        mtree.AddFriend( tree, friendAlias(index) )
    areFriends = True
    return mtree

def drawDiff(var, cut=''):
    mtree = trees[0]
    if not areFriends:
        mtree = makeFriends()
    same = ''
    histograms = {}
    leaves = [leave.GetName() for leave in mtree.GetListOfBranches()]
    if not var in leaves:
        import pdb; pdb.set_trace()
        raise ValueError( var + ' is not in reference tree.')
    for index, tree in enumerate(trees):
        if tree == mtree: continue
        leaves = [leave.GetName() for leave in tree.GetListOfBranches()]        
        if not var in leaves:
            raise ValueError( var + ' is not in reference tree ' + friendAlias(index))
        diffvar = '{var}-{ovar}'.format(var=var,
                                        ovar = '.'.join([friendAlias(index), var]))
        print diffvar
        mtree.Draw(diffvar, cut, same)
        hist = mtree.GetHistogram()
        histograms['.'.join([friendAlias(index), var])] = hist
        if index < len(styles):
            styles[index].formatHisto( hist )        
        if same == '':
            same = 'same'
    return histograms


def fullDiff(cut=''):
    mtree = trees[0]
    for var in (l.GetName() for l in mtree.GetListOfLeaves()) :
        # print 'diffing', var
        histograms = drawDiff( var, cut )
        gPad.Modified()
        gPad.Update()
        for name, histo in histograms.iteritems():
            if histo.GetMean()!=0 or histo.GetRMS()!=0:
                print 'print difference spotted in', name
        # time.sleep(0.1)
    

