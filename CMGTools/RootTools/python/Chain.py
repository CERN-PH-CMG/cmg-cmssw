import glob

from ROOT import TChain


class Chain( object ):
    def __init__(self, treeName, pattern ):
        self.files = []
        self.chain = TChain(treeName)
        nFiles = 0
        for file in glob.glob(pattern):
            self.chain.Add(file)
            nFiles += 1
        if nFiles==0:
            raise ValueError('no matching file name')

    def __getattr__(self, attr):
        return getattr(self.chain, attr)

if __name__ == '__main__':

    import sys

    treeName = sys.argv[1]
    pattern = sys.argv[2]
    chain = Chain( treeName, pattern )
