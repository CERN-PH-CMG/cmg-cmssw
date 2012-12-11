import ROOT

class LeptonMVA(object):
    def __init__(self, name, variables, weightFile, options="!Color:!V"):
        self._name = name
        self._variables = variables
        self._weightFile = weightFile
        self.tmva = ROOT.PyTMVATool(name, len(variables), weightFile, options)
        for (name, type, function) in variables:
            if type == 'I':
                self.tmva.AddIntVariable(name)
            elif type == 'F':
                self.tmva.AddFloatVariable(name)
            else: raise RuntimeError, "Unknown type '%d'" % type
        self.tmva.DoneInit()
    def __call__(self, lepton):
        for i,(name, type, function) in enumerate(self._variables):
            if   type == 'I': self.tmva.setInt(i, function(lepton))
            elif type == 'F': self.tmva.setFloat(i, function(lepton))
        return self.tmva.getVal()


    
