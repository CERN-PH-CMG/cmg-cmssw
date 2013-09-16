import os

class Output( object ):

    def __init__(self, name ):
        self.name = name
        self._MakeOutputDir()
        self.histoLists = {}

    def _MakeOutputDir(self):
        index = 0
        name = self.name
        while True:
            try:
                # print 'mkdir', self.name
                os.mkdir( name )
                break
            except OSError:
                index += 1
                name = '%s_%d' % (self.name, index)
        self.name = name

    def Write(self):
        for histoList in self.histoLists.values():
            histoList.Write()
