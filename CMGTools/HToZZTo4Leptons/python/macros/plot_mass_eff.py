from CMGTools.RootTools.PyRoot import *


keeper = []

def initTree( fileName ):
    file = TFile(fileName)
    keeper.append( file )
    tree = file.Get('FourLeptonTreeProducer')
    return tree

tree1 = initTree( sys.argv[1] )
tree2 = initTree( sys.argv[2] )


def draw( var, cut ):
    tree1.Draw( var, cut )
    tree2.Draw( var, cut, 'same')
