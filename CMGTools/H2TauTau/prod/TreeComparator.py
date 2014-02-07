from CMGTools.RootTools.RootInit import *
import CMGTools.RootTools.Style as Style

class TreeComparator(object):

    def __init__(self, tree1, tree2):
        print 'comparator:'
        self.trees = {}
        self.leaveList = []
        self.addTree('tree1', tree1)
        self.addTree('tree2', tree2)
        
    def addTree(self, key, tree):
        print 'adding ', key, tree.GetName()
        tree.SetWeight( 1./tree.GetEntries(), 'global')
        self.trees[key] = tree
        if len( self.leaveList ) == 0:
            self.leaveList = self.leaves( key )
#        elif self.leaveList != self.leaves( key ):
#            raise ValueError( 'TTree list of leaves not compatible' )
        
    def draw(self, var, cut=''):
        same = ''
        for tree in self.trees.values():
            tree.Draw( var, cut, same)
            Style.nextStyle().formatHisto( tree.GetHistogram() )
            if same == '':
                same = 'same'

    def drawAll( self, cut=''):
        for leaf in self.leaveList:
            self.draw( leaf, cut )
        
    def leaves(self, key):
        tree = self.trees[key]
        return [ leaf.GetName() for leaf in tree.GetListOfLeaves() ]

if __name__ == '__main__':

    import sys
    name = sys.argv[1]
    tree1 = Chain( name, sys.argv[2])
    tree2 = Chain( name, sys.argv[3])
    tc = TreeComparator( tree1, tree2 )
