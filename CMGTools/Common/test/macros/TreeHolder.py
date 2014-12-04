import ROOT as rt

class TreeHolder(object):
    
    def __init__(self, tree, cut = None):
        self.tree = tree

        if cut is None:
            cut = '1'
        self.cut = cut

        self.plots = []
    
        self.index = 0

        import string
        self.allowed = ['_']
        self.allowed.extend(string.letters)
        self.allowed.extend(string.digits)
        
    def getName(self, var, cut, postfix = ''):
        name = '%s_%s_%i_%s' % (var,cut,self.index, postfix)
        self.index += 1
        result = ''
        for n in name:
            if n in self.allowed:
                result += n
        return result 
        
    
    def plot(self, var, cut):

        n = self.getName(var, cut)
        self.tree.Draw('%s>>%s' % (var,n),'%s && %s' % (self.cut,cut),'goff')
        h = rt.gDirectory.Get('%s' % n)

        if h is not None:
            h.SetName(n)
            h.GetXaxis().SetTitle(var)
            self.plots.append(h)

    def write(self, dirName):
        rt.gDirectory.mkdir(dirName)
        rt.gDirectory.cd(dirName)
        for p in self.plots:
            p.Write()
        rt.gDirectory.cd('..')


