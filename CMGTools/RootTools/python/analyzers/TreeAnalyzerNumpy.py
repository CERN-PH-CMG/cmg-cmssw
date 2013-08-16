from CMGTools.RootTools.fwlite.Analyzer import Analyzer
from CMGTools.RootTools.statistics.TreeNumpy import TreeNumpy as Tree
from ROOT import TFile

class TreeAnalyzerNumpy( Analyzer ):
    """Base TreeAnalyzerNumpy, to create flat TTrees.

    Check out TestTreeAnalyzer for a concrete example.
    IMPORTANT: FOR NOW, CANNOT RUN SEVERAL TreeAnalyzers AT THE SAME TIME!
    Anyway, you want only one TTree, don't you?"""

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(TreeAnalyzerNumpy,self).__init__(cfg_ana, cfg_comp, looperName)
        fileName = '/'.join([self.dirName,
                             self.name+'_tree.root'])
        self.file = TFile( fileName, 'recreate' )
        self.tree = Tree(self.name,self.name)
        self.declareVariables()
        
    def declareVariables(self):
        print 'TreeAnalyzerNumpy.declareVariables : overload this function.'
        pass

    def write(self):
        super(TreeAnalyzerNumpy, self).write()
        self.file.Write() 

