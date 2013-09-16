import numpy
from ROOT import TTree

class TreeNumpy(object):
    
    def __init__(self, name, title):
        self.vars = {}
        self.tree = TTree(name, title)
        
    def copyStructure(self, tree):
        for branch in tree.GetListOfBranches():
            name = branch.GetName() 
            typeName = branch.GetListOfLeaves()[0].GetTypeName()
            type = float
            if typeName == 'Int_t':
                type = int
            self.var(name, type)            
            
    def var(self, varName,type=float ):
        self.vars[varName]=numpy.zeros(1,type)
        if type is float  : 
            self.tree.Branch(varName,self.vars[varName],varName+'/D')
        elif type is int    : 
            self.tree.Branch(varName,self.vars[varName],varName+'/I')
        else:
            print 'Unknown type '
            
    def reset(self):
        for name,value in self.vars.iteritems():
            value[0]=-99
            
    def fill(self, varName, value ):
        self.vars[varName][0]=value

if __name__=='__main__':
    
    from ROOT import TFile

    f = TFile('TreeNumpy.root','RECREATE')
    t = TreeNumpy('Colin', 'Another test tree')
    t.var('a')
    t.var('b')

    t.fill('a', 3)
    t.fill('a', 4)
    t.fill('b', 5)
    t.tree.Fill()

    f.Write()
    f.Close()
