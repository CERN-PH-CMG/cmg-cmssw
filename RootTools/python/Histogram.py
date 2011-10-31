import copy


class Histogram:
    def __init__(self, name, obj, layer=0, stack=True):
        self.name = name 
        self.obj = obj
        self.weighted = copy.deepcopy(self.obj)
        self.layer = layer
        self.stack = stack
        self.SetWeight(1)
        
    def __str__(self):
        return self.name + ' / ' + self.obj.GetName() + ', L=' + str(self.layer)

    def SetWeight(self, weight):
        self.weighted = copy.deepcopy(self.obj)
        self.weight = weight
        self.weighted.Scale(weight)

    def AddEntry(self, legend, legendLine=None):
        if legendLine == None:
            legendLine = self.name
        legend.AddEntry(self.obj, legendLine)

    def Draw(self, opt=''):
        self.weighted.Draw(opt)

    def Integral(self):
        return self.weighted.Integral()

    def DrawNormalized(self):
        self.obj.DrawNormalized()

    def Normalize(self):
        self.obj.Scale( 1/self.obj.Integral() )

