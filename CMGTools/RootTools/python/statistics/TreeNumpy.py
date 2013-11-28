import numpy
from ROOT import TTree

class TreeNumpy(object):
    
    def __init__(self, name, title):
        self.vars = {}
        self.vecvars = {}
        self.tree = TTree(name, title)
        self.defaults = {}
        self.vecdefaults = {}
        
    def copyStructure(self, tree):
        for branch in tree.GetListOfBranches():
            name = branch.GetName() 
            typeName = branch.GetListOfLeaves()[0].GetTypeName()
            type = float
            if typeName == 'Int_t':
                type = int
            self.var(name, type)            
            
    def var(self, varName,type=float, default=-99, title=None ):
        self.vars[varName]=numpy.zeros(1,type)
        self.defaults[varName] = default
        if type is float  : 
            self.tree.Branch(varName,self.vars[varName],varName+'/D')
        elif type is int    : 
            self.tree.Branch(varName,self.vars[varName],varName+'/I')
        else:
            print 'Unknown type %s for branch %s' % (type, varName)
        if title:
            self.tree.GetBranch(varName).SetTitle(title)

    def vector(self, varName, lenvar, maxlen, type=float, default=-99, title=None ):
        self.vecvars[varName]=numpy.zeros(maxlen,type)
        self.vecdefaults[varName] = default
        if type is float  : 
            self.tree.Branch(varName,self.vecvars[varName],varName+'['+lenvar+']/D')
        elif type is int    : 
            self.tree.Branch(varName,self.vecvars[varName],varName+'['+lenvar+']/I')
        else:
            print 'Unknown type %s for branch %s' % (type, varName)
        if title:
            self.tree.GetBranch(varName).SetTitle(title)

    def reset(self):
        for name,value in self.vars.iteritems():
            value[0]=self.defaults[name]
        for name,value in self.vecvars.iteritems():
            value.fill(self.vecdefaults[name])
            
    def fill(self, varName, value ):
        self.vars[varName][0]=value

    def vfill(self, varName, values ):
        a = self.vecvars[varName]
        for (i,v) in enumerate(values):
            a[i]=v

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
