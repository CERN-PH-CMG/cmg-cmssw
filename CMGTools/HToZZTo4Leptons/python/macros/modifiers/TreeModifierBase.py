import ROOT

class TreeModifierBase(object):
    def __init__(self):
        print 'Starting'

    def processTreeEvent(self,event,obj):
        pass

    def modifyTree(self,tree):
        pass


    def loop(self,directory):
        #search all trees in the file recursively
        currDir = ROOT.gDirectory
        for key in directory.GetListOfKeys():
            print 'Found key',key.GetName()
            obj = key.ReadObj()                 
            if obj.IsA().InheritsFrom(ROOT.TDirectory.Class()):
                currDir.cd(key.GetName());
                self.loop(obj)
                currDir.cd()

            elif obj.IsA().InheritsFrom(ROOT.TTree.Class()):
                self.modifyTree(obj)
                for event in obj:
                    self.processTreeEvent(event,obj)
                obj.Write('',ROOT.TObject.kOverwrite)    





