import time
import re

from CMGTools.RootTools.RootInit import *
import CMGTools.RootTools.Style as Style

class TreeComparator(object):

    def __init__(self, tree1, tree2):
        print 'comparator:'
        self.trees = {}
        self.styles = {}
        self.leaveList = []
        self.addTree('tree1', tree1)
        self.addTree('tree2', tree2)
        self.setStyles()
        
    def addTree(self, key, tree, pdf=False):
        print 'adding ', key, tree.GetName()
        if pdf:
            tree.SetWeight( 1./tree.GetEntries(), 'global')
        self.trees[key] = tree
        if len( self.leaveList ) == 0:
            self.leaveList = self.leaves( key )
        self.styles[key] = Style.nextStyle()
        #        elif self.leaveList != self.leaves( key ):
        #            raise ValueError( 'TTree list of leaves not compatible' )
        
        
    def setStyles(self):
        self.styles['tree1'] = Style.sBlack
        self.styles['tree2'] = Style.sBlueSquares
        Style.sBlueSquares.markerStyle = 25
        
    def draw(self, var, cut=''):
        same = ''
        ok = True
        for key, tree in self.trees.iteritems():
            tree.Draw( var, cut, same)
            self.styles[key].formatHisto( tree.GetHistogram() )
            if tree.GetHistogram().GetEntries() == 0:
                print 'no entry in',key
                ok = False
            tree.GetHistogram().Sumw2()
            if same == '' and ok:
                same = 'same'
        gPad.Modified()
        gPad.Update()
        gPad.SaveAs( var + '.png')

    def drawAll( self, pattern='.*', cut=''):
        pat = re.compile(pattern)
        for leaf in self.leaveList:
            if not pat.match( leaf ):
                # print 'skipping',leaf
                continue
            print 'drawing',leaf
            self.draw( leaf, cut )
            time.sleep(1)
        
    def leaves(self, key):
        tree = self.trees[key]
        return [ leaf.GetName() for leaf in tree.GetListOfLeaves() ]

if __name__ == '__main__':

    import sys
    name = sys.argv[1]
    tree1 = Chain( name, sys.argv[2])
    tree2 = Chain( name, sys.argv[3])
    tc = TreeComparator( tree1, tree2 )
