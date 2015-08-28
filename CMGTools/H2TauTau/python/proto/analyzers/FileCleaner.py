import os

from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer


class FileCleaner(Analyzer):

    '''Gets tau decay mode efficiency weight and puts it in the event'''

    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(FileCleaner, self).__init__(cfg_ana, cfg_comp, looperName)

    def process(self, event):
        pass

    def endLoop(self, setup):
        print self, self.__class__
        super(FileCleaner, self).endLoop(setup)
        for comp in setup.config.components:
            for f in comp.files:
                os.remove(f)
