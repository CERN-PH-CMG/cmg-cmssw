from CMGTools.RootTools.RootTools import * 
from ROOT import TFile

files = []

def plot(fileName):
    files.append( TFile(fileName) ) 
    pu = files[-1].Get('pileup')
    pu.Scale( 1/pu.Integral() )
    pu.Draw()

